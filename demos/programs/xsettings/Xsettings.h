/**
 *
 * $Id: Xsettings.h,v 1.1 2009/06/18 18:05:51 rwscott Exp $
 *
 * Copyright 2009 Rick Scott <rwscott@users.sourceforge.net>
 *
 **/

#ifndef _XSETTINGS_H
#define _XSETTINGS_H

#include <X11/Intrinsic.h>

typedef enum {
  XSETTINGS_TYPE_INT     = 0,
  XSETTINGS_TYPE_STRING  = 1,
  XSETTINGS_TYPE_COLOR   = 2
} XSettingsType;

typedef enum 
{
  XSETTINGS_ACTION_NONE,
  XSETTINGS_ACTION_NEW,
  XSETTINGS_ACTION_CHANGED,
  XSETTINGS_ACTION_DELETED
} XSettingsAction;

typedef struct XSettingsColorRec {
  unsigned short red, green, blue, alpha;
} XSettingsColor;

typedef struct XSettingsSettingRec {
    XrmQuark name;
    XSettingsType type;
    union {
	int v_int;
	XrmQuark v_string;
	XSettingsColor v_color;
    } data;
    unsigned long last_change_serial;
} XSettingsSetting;

typedef struct XmXsettingsCallbackRec {
    XSettingsAction action;
    XSettingsSetting setting;
} XmXsettingsCallbackStruct;

#ifdef __cplusplus
extern "C" {
#endif

#ifndef XmNxsettingsCallback
#define XmNxsettingsCallback "xsettingsCallback"
#endif
#ifndef XmCXsettingsCallback
#define XmCXsettingsCallback "XsettingsCallback"
#endif
#ifndef XmNnumXsettings
#define XmNnumXsettings "numXsettings"
#endif
#ifndef XmCNumXsettings
#define XmCNumXsettings "NumXsettings"
#endif
#ifndef XmNxsettings
#define XmNxsettings "xsettings"
#endif
#ifndef XmCXsettings
#define XmCXsettings "Xsettings"
#endif

extern WidgetClass xmXsettingsWidgetClass;

typedef struct _XmXsettingsRec *XmXsettingsWidget;
typedef struct _XmXsettingsClassRec *XmXsettingsWidgetClass;

Widget XmCreateXsettings(Widget parent, String name, ArgList arglist, Cardinal argcount);

#ifdef __cplusplus
}
#endif
#endif
