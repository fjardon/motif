#ifndef XSETTINGS_H
#define XSETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _XSettingsBuffer  XSettingsBuffer;
typedef struct _XSettingsColor   XSettingsColor;
typedef struct _XSettingsList    XSettingsList;
typedef struct _XSettingsSetting XSettingsSetting;
typedef struct _XSettingsClient XSettingsClient;
/* Types of settings possible. Enum values correspond to
 * protocol values.
 */
typedef enum
{
  XSETTINGS_TYPE_INT     = 0,
  XSETTINGS_TYPE_STRING  = 1,
  XSETTINGS_TYPE_COLOR   = 2
} XSettingsType;

typedef enum
{
  XSETTINGS_SUCCESS,
  XSETTINGS_NO_MEM,
  XSETTINGS_ACCESS,
  XSETTINGS_FAILED,
  XSETTINGS_NO_ENTRY,
  XSETTINGS_DUPLICATE_ENTRY
} XSettingsResult;

struct _XSettingsBuffer
{
  char byte_order;
  size_t len;
  unsigned char *data;
  unsigned char *pos;
};

struct _XSettingsColor
{
  unsigned short red, green, blue, alpha;
};

struct _XSettingsList
{
  XSettingsSetting *setting;
  XSettingsList *next;
};

struct _XSettingsSetting
{
  char *name;
  XSettingsType type;

  union {
    int v_int;
    char *v_string;
    XSettingsColor v_color;
  } data;

  unsigned long last_change_serial;
};

XSettingsClient *xsettings_client_new           (Display             *display,
						 int                  screen,
						 void                *cb_data);
void             xsettings_client_destroy       (XSettingsClient     *client);

XSettingsResult xsettings_client_get_setting (XSettingsClient   *client,
					      const char        *name,
					      XSettingsSetting **setting);

#define XSETTINGS_PAD(n,m) ((n + m - 1) & (~(m-1)))

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* XSETTINGS_H */
