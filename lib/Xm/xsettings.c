#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <limits.h>
#include <string.h>
#include <X11/Xmd.h>		/* For CARD16 */

#include "XmI.h"
#include "xsettings.h"

#define BYTES_LEFT(buffer) ((buffer)->data + (buffer)->len - (buffer)->pos)
#define XSETTINGS_PAD(n,m) ((n + m - 1) & (~(m-1)))

void
xsettings_setting_free (XSettingsSetting *setting)
{
  if (setting->type == XSETTINGS_TYPE_STRING)
    free (setting->data.v_string);

  if (setting->name)
    free (setting->name);

  free (setting);
}

void
xsettings_list_free (XSettingsList *list)
{
  while (list)
    {
      XSettingsList *next = list->next;

      xsettings_setting_free (list->setting);
      free (list);

      list = next;
    }
}
XSettingsSetting *
xsettings_setting_copy (XSettingsSetting *setting)
{
  XSettingsSetting *result;
  size_t str_len;

  result = malloc (sizeof *result);
  if (!result)
    return NULL;

  str_len = strlen (setting->name);
  result->name = malloc (str_len + 1);
  if (!result->name)
    goto err;

  memcpy (result->name, setting->name, str_len + 1);

  result->type = setting->type;

  switch (setting->type)
    {
    case XSETTINGS_TYPE_INT:
      result->data.v_int = setting->data.v_int;
      break;
    case XSETTINGS_TYPE_COLOR:
      result->data.v_color = setting->data.v_color;
      break;
    case XSETTINGS_TYPE_STRING:
      str_len = strlen (setting->data.v_string);
      result->data.v_string = malloc (str_len + 1);
      if (!result->data.v_string)
	goto err;

      memcpy (result->data.v_string, setting->data.v_string, str_len + 1);
      break;
    }

  result->last_change_serial = setting->last_change_serial;

  return result;

 err:
  if (result->name)
    free (result->name);
  free (result);

  return NULL;
}

XSettingsResult
xsettings_list_insert (XSettingsList    **list,
		       XSettingsSetting  *setting)
{
  XSettingsList *node;
  XSettingsList *iter;
  XSettingsList *last = NULL;

  node = malloc (sizeof *node);
  if (!node)
    return XSETTINGS_NO_MEM;
  node->setting = setting;

  iter = *list;
  while (iter)
    {
      int cmp = strcmp (setting->name, iter->setting->name);

      if (cmp < 0)
	break;
      else if (cmp == 0)
	{
	  free (node);
	  return XSETTINGS_DUPLICATE_ENTRY;
	}

      last = iter;
      iter = iter->next;
    }

  if (last)
    last->next = node;
  else
    *list = node;

  node->next = iter;

  return XSETTINGS_SUCCESS;
}

XSettingsSetting *
xsettings_list_lookup (XSettingsList *list,
		       const char    *name)
{
  XSettingsList *iter;

  iter = list;
  while (iter)
    {
      if (strcmp (name, iter->setting->name) == 0)
	return iter->setting;

      iter = iter->next;
    }

  return NULL;
}

char
xsettings_byte_order (void)
{
  CARD32 myint = 0x01020304;
  return (*(char *)&myint == 1) ? MSBFirst : LSBFirst;
}

struct _XSettingsClient
{
  Display *display;
  int screen;
  void *cb_data;

  Window manager_window;
  Atom manager_atom;
  Atom selection_atom;
  Atom xsettings_atom;

  XSettingsList *settings;
};

static int
ignore_errors (Display *display, XErrorEvent *event)
{
  return True;
}

static char local_byte_order = '\0';


static XSettingsResult
fetch_card16 (XSettingsBuffer *buffer,
	      CARD16          *result)
{
  CARD16 x;

  if (BYTES_LEFT (buffer) < 2)
    return XSETTINGS_ACCESS;

  x = *(CARD16 *)buffer->pos;
  buffer->pos += 2;

  if (buffer->byte_order == local_byte_order)
    *result = x;
  else
    *result = (x << 8) | (x >> 8);

  return XSETTINGS_SUCCESS;
}

static XSettingsResult
fetch_ushort (XSettingsBuffer *buffer,
	      unsigned short  *result)
{
  CARD16 x;
  XSettingsResult r;

  r = fetch_card16 (buffer, &x);
  if (r == XSETTINGS_SUCCESS)
    *result = x;

  return r;
}

static XSettingsResult
fetch_card32 (XSettingsBuffer *buffer,
	      CARD32          *result)
{
  CARD32 x;

  if (BYTES_LEFT (buffer) < 4)
    return XSETTINGS_ACCESS;

  x = *(CARD32 *)buffer->pos;
  buffer->pos += 4;

  if (buffer->byte_order == local_byte_order)
    *result = x;
  else
    *result = (x << 24) | ((x & 0xff00) << 8) | ((x & 0xff0000) >> 8) | (x >> 24);

  return XSETTINGS_SUCCESS;
}

static XSettingsResult
fetch_card8 (XSettingsBuffer *buffer,
	     CARD8           *result)
{
  if (BYTES_LEFT (buffer) < 1)
    return XSETTINGS_ACCESS;

  *result = *(CARD8 *)buffer->pos;
  buffer->pos += 1;

  return XSETTINGS_SUCCESS;
}


static XSettingsList *
parse_settings (unsigned char *data,
		size_t         len)
{
  XSettingsBuffer buffer;
  XSettingsResult result = XSETTINGS_SUCCESS;
  XSettingsList *settings = NULL;
  CARD32 serial;
  CARD32 n_entries;
  CARD32 i;
  XSettingsSetting *setting = NULL;
  char buf[100];

  local_byte_order = xsettings_byte_order ();

  buffer.pos = buffer.data = data;
  buffer.len = len;

  result = fetch_card8 (&buffer, (CARD8*)&buffer.byte_order);
  if (buffer.byte_order != MSBFirst &&
      buffer.byte_order != LSBFirst)
    {
		XtWarning ("Invalid byte order in XSETTINGS property\n");
      	result = XSETTINGS_FAILED;
      	goto out;
    }

  buffer.pos += 3;

  result = fetch_card32 (&buffer, &serial);
  if (result != XSETTINGS_SUCCESS)
    goto out;

  result = fetch_card32 (&buffer, &n_entries);
  if (result != XSETTINGS_SUCCESS)
    goto out;

  for (i = 0; i < n_entries; i++)
    {
      CARD8 type;
      CARD16 name_len;
      CARD32 v_int;
      size_t pad_len;

      result = fetch_card8 (&buffer, &type);
      if (result != XSETTINGS_SUCCESS)
	goto out;

      buffer.pos += 1;

      result = fetch_card16 (&buffer, &name_len);
      if (result != XSETTINGS_SUCCESS)
	goto out;

      pad_len = XSETTINGS_PAD(name_len, 4);
      if (BYTES_LEFT (&buffer) < pad_len)
	{
	  result = XSETTINGS_ACCESS;
	  goto out;
	}

      setting = malloc (sizeof *setting);
      if (!setting)
	{
	  result = XSETTINGS_NO_MEM;
	  goto out;
	}
      setting->type = XSETTINGS_TYPE_INT; /* No allocated memory */

      setting->name = malloc (name_len + 1);
      if (!setting->name)
	{
	  result = XSETTINGS_NO_MEM;
	  goto out;
	}

      memcpy (setting->name, buffer.pos, name_len);
      setting->name[name_len] = '\0';
      buffer.pos += pad_len;

      result = fetch_card32 (&buffer, &v_int);
      if (result != XSETTINGS_SUCCESS)
	goto out;
      setting->last_change_serial = v_int;

      switch (type)
	{
	case XSETTINGS_TYPE_INT:
	  result = fetch_card32 (&buffer, &v_int);
	  if (result != XSETTINGS_SUCCESS)
	    goto out;

	  setting->data.v_int = (INT32)v_int;
	  break;
	case XSETTINGS_TYPE_STRING:
	  result = fetch_card32 (&buffer, &v_int);
	  if (result != XSETTINGS_SUCCESS)
	    goto out;

	  pad_len = XSETTINGS_PAD (v_int, 4);
	  if (v_int + 1 == 0 || /* Guard against wrap-around */
	      BYTES_LEFT (&buffer) < pad_len)
	    {
	      result = XSETTINGS_ACCESS;
	      goto out;
	    }

	  setting->data.v_string = malloc (v_int + 1);
	  if (!setting->data.v_string)
	    {
	      result = XSETTINGS_NO_MEM;
	      goto out;
	    }

	  memcpy (setting->data.v_string, buffer.pos, v_int);
	  setting->data.v_string[v_int] = '\0';
	  buffer.pos += pad_len;

	  break;
	case XSETTINGS_TYPE_COLOR:
	  result = fetch_ushort (&buffer, &setting->data.v_color.red);
	  if (result != XSETTINGS_SUCCESS)
	    goto out;
	  result = fetch_ushort (&buffer, &setting->data.v_color.green);
	  if (result != XSETTINGS_SUCCESS)
	    goto out;
	  result = fetch_ushort (&buffer, &setting->data.v_color.blue);
	  if (result != XSETTINGS_SUCCESS)
	    goto out;
	  result = fetch_ushort (&buffer, &setting->data.v_color.alpha);
	  if (result != XSETTINGS_SUCCESS)
	    goto out;

	  break;
	default:
	  /* Quietly ignore unknown types */
	  break;
	}

      setting->type = type;

      result = xsettings_list_insert (&settings, setting);
      if (result != XSETTINGS_SUCCESS)
	goto out;

      setting = NULL;
    }

 out:

  if (result != XSETTINGS_SUCCESS)
    {
      switch (result)
	{
	case XSETTINGS_NO_MEM:
		XtWarning( "Out of memory reading XSETTINGS property\n");
	  break;
	case XSETTINGS_ACCESS:
		XtWarning("Invalid XSETTINGS property (read off end)\n");
	  break;
	case XSETTINGS_DUPLICATE_ENTRY:
		sprintf(buf,"Duplicate XSETTINGS entry for '%s'\n", setting->name);
		XtWarning(buf);
	case XSETTINGS_FAILED:
	case XSETTINGS_SUCCESS:
	case XSETTINGS_NO_ENTRY:
	  break;
	}

      if (setting)
	xsettings_setting_free (setting);

      xsettings_list_free (settings);
      settings = NULL;

    }

  return settings;
}

static void
read_settings (XSettingsClient *client)
{
  Atom type;
  int format;
  unsigned long n_items;
  unsigned long bytes_after;
  unsigned char *data;
  int result;
  char buf[100];
  int (*old_handler) (Display *, XErrorEvent *);

  XSettingsList *old_list = client->settings;

  client->settings = NULL;

  old_handler = XSetErrorHandler (ignore_errors);
  result = XGetWindowProperty (client->display, client->manager_window,
			       client->xsettings_atom, 0, LONG_MAX,
			       False, client->xsettings_atom,
			       &type, &format, &n_items, &bytes_after, &data);
  XSetErrorHandler (old_handler);

  if (result == Success && type == client->xsettings_atom)
    {
      if (format != 8)
	{
	  sprintf (buf, "Invalid format for XSETTINGS property %d", format);
	  XtWarning(buf);

	}
      else
	client->settings = parse_settings (data, n_items);

      XFree (data);
    }

  xsettings_list_free (old_list);
}

static void
check_manager_window (XSettingsClient *client)
{
  XGrabServer (client->display);

  client->manager_window = XGetSelectionOwner (client->display,
					       client->selection_atom);
  if (client->manager_window)
    XSelectInput (client->display, client->manager_window,
		  PropertyChangeMask | StructureNotifyMask);

  XUngrabServer (client->display);
  XFlush (client->display);

  read_settings (client);
}

XSettingsClient *
xsettings_client_new (Display             *display,
		      int                  screen,
		      void                *cb_data)
{
  XSettingsClient *client;
  char buffer[256];

  client = malloc (sizeof *client);
  if (!client)
    return NULL;

  client->display = display;
  client->screen = screen;
  client->cb_data = cb_data;

  client->manager_window = None;
  client->settings = NULL;

  sprintf(buffer, "_XSETTINGS_S%d", screen);
  client->selection_atom = XInternAtom (display, buffer, False);
  client->xsettings_atom = XInternAtom (display, "_XSETTINGS_SETTINGS", False);
  client->manager_atom = XInternAtom (display, "MANAGER", False);

  check_manager_window (client);

  return client;
}

void
xsettings_client_destroy (XSettingsClient *client)
{
  xsettings_list_free (client->settings);
  free (client);
}

XSettingsResult
xsettings_client_get_setting (XSettingsClient   *client,
			      const char        *name,
			      XSettingsSetting **setting)
{
  read_settings(client);
  XSettingsSetting *search = xsettings_list_lookup (client->settings, name);
  if (search)
    {
      *setting = xsettings_setting_copy (search);
      return *setting ? XSETTINGS_SUCCESS : XSETTINGS_NO_MEM;
    }
  else
    return XSETTINGS_NO_ENTRY;
}

int
XmeGetSetting(Display* display, char *name)
{
	const char *display_str = NULL;
	static XSettingsClient *client = NULL;
        static Boolean client_inited = False;
        XSettingsSetting *setting = NULL;
	Display *disp;
	XSettingsResult xr;

	if (!client_inited)
	{
		disp = XOpenDisplay(NULL);
		client = xsettings_client_new (display, DefaultScreen (display), NULL);
		client_inited = True;
	}

	if (!client)
	{
		return (0);
	}

        xr = xsettings_client_get_setting(client, name, &setting);
	/* In case we will use Display which come as function parameter
	   we will uncomment this comment */
	/*	xsettings_client_destroy (client);*/
	if (xr != XSETTINGS_SUCCESS) 
	{
		return (0);
	}
		
	return (setting->data.v_int);

}

int
XmeGetMultiClickTime(Display* display)
{
	int DoubleClickTime = 0;
	DoubleClickTime = XmeGetSetting(display, "Net/DoubleClickTime");

	if (!DoubleClickTime)
		return(XtGetMultiClickTime(display));

	return (DoubleClickTime);
}
