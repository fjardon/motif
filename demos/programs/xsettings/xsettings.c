/*
 *
 * Copyright 2009 Rick Scott <rwscott@users.sourceforge.net>
 *
 */
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <Xm/XmAll.h>        

#include "Xsettings.h"

#define APP_CLASS "xsettings"

/*
 * Fallbacks: font, label and position go together in this demo 
 */
static String fallbacks[] = {
    /* Since the toolTip resources are now SubResources of the VendorShell,
       the application class _has_ to be prefixed onto the resource in
       order to have the fallback resource work. Originally you could
       simply use "*toolTipPostDelay: 2000". I guess this is the price
       to be paid for making them into Traits :(
     */
    APP_CLASS ".toolTipPostDelay: 2000",
    "*enableThemes: True",
    "*ContainerScrolledWindow.width: 430",
    "*ContainerScrolledWindow.height: 320",
    "*selectColor: DEFAULT_SELECT_COLOR",
    NULL
};

typedef struct {
	//Pixel modified_color;
} AppResources_t, *AppResourcesPtr;
AppResources_t AppResources;

static XtResource resources[] =
{
    //{"modifiedColor", "ModifiedColor", XtRPixel, sizeof(Pixel), XtOffset(AppResourcesPtr, modified_color), XtRString, "red"},
};

typedef struct FlashControlRec {
    Widget w;
    int count;
    XtIntervalId id;
    unsigned char visualEmphasis;
} FlashControl;

static void
okCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
    XtAppSetExitFlag(XtWidgetToApplicationContext(w));
}

static void
cancelTimer(Widget w, XtPointer client_data, XtPointer call_data)
{
FlashControl *fc = client_data;

    XtVaSetValues(w,
    	XmNuserData, NULL,
    	NULL);
    XtRemoveTimeOut(fc->id);
    XtFree((char *)fc);
}

static void
_flash(XtPointer client_data, XtIntervalId *id)
{
FlashControl *fc = client_data;
unsigned char visualEmphasis;
Widget w = fc->w;

    XtVaGetValues(fc->w,
	XmNvisualEmphasis, &visualEmphasis,
	NULL);
    if (fc->count > 0)
    {
	fc->id = XtAppAddTimeOut(XtWidgetToApplicationContext(fc->w),
	    250,
	    _flash,
	    fc);
	fc->count--;
	visualEmphasis = visualEmphasis == XmNOT_SELECTED ? XmSELECTED : XmNOT_SELECTED;
    }
    else
    {
	XtRemoveCallback(fc->w, XmNdestroyCallback, cancelTimer, fc);
	visualEmphasis = fc->visualEmphasis;
	XtVaSetValues(fc->w,
	    XmNuserData, NULL,
	    NULL);
    	XtFree((char *)fc);
    }
    XtVaSetValues(w,
	XmNvisualEmphasis, visualEmphasis,
	NULL);
}

static void
flash(Widget w)
{
    if (XtIsRealized(w))
    {
    FlashControl *fc;

	XtVaGetValues(w,
	    XmNuserData, &fc,
	    NULL);
	if (fc == NULL)
	{
	    fc = XtNew(FlashControl);
	    fc->w = w;
	    XtVaGetValues(fc->w,
		XmNvisualEmphasis, &fc->visualEmphasis,
		NULL);
	    XtAddCallback(fc->w, XmNdestroyCallback, cancelTimer, fc);
	    XtVaSetValues(fc->w,
		XmNuserData, fc,
	    	NULL);
	}
	else
	{
	    XtRemoveTimeOut(fc->id);
	}
	fc->count = 10;
	fc->id = XtAppAddTimeOut(XtWidgetToApplicationContext(w),
	    0,
	    _flash,
	    fc);
    }
}

static void
printSetting(XSettingsSetting *setting, Widget container, Boolean delete)
{
String fullname = NULL;
String tmp;
String tok;
Widget entryParent = NULL;
XmStringTable detail;
Cardinal detailCount;
int numKids;
WidgetList kid = NULL;

    fullname = XtNewString(XrmQuarkToString(setting->name));
    tmp = fullname;

    /*
    printf("%s\n", fullname);
    */
    while ((tok = strtok(tmp, "/")))
    {
    	numKids = XmContainerGetItemChildren(container, entryParent, &kid);
    	for (; numKids > 0; numKids--)
    	{
	    if (strcmp(tok, XtName(kid[numKids - 1])) == 0)
	    {
	    	break;
	    }
    	}
    	if (numKids > 0)
    	{
	    entryParent = kid[numKids - 1];
	    flash(entryParent);
    	}
    	else
    	{
    	Widget new_kid;
    	XmString toolTipString = NULL;

	    if (entryParent)
	    {
	    String format = "%s is type\n%s";
	    String tmp;
	    String type;

	    	switch (setting->type)
	    	{
	    	case XSETTINGS_TYPE_INT:
		    type = "int";
		    break;
	    	case XSETTINGS_TYPE_STRING:
		    type = "char *";
		    break;
	    	case XSETTINGS_TYPE_COLOR:
		    type = "Color";
		    break;
	    	default:
		    type = "Unknown";
		    break;
	    	}
	    	tmp = XtMalloc(strlen(tok) + strlen(type) + strlen(format) + 1);
	    	sprintf(tmp, format, tok, type);
	    	toolTipString = XmStringCreateLocalized(tmp);
	    	XtFree(tmp);
	    }
	    else
	    {
	    String format = "%s related settings";
	    String tmp;

	    	tmp = XtMalloc(strlen(tok) + strlen(format) + 1);
	    	sprintf(tmp, format, tok);
	    	toolTipString = XmStringCreateLocalized(tmp);
	    	XtFree(tmp);
	    }
	    new_kid = XmVaCreateIconGadget(container, tok,
	    	XmNentryParent, entryParent,
	    	XmNtoolTipString, toolTipString,
	    	NULL);
	    XmStringFree(toolTipString);
	    XtManageChild(new_kid);
	    entryParent = new_kid;
    	}
    	tmp = NULL;
    	XtFree((char *)kid);
    	kid = NULL;
    }
    XtFree((char *)kid);
    if (delete)
    {
    	XtDestroyWidget(entryParent);
    }
    else
    {
	detailCount = 0;
	detail = NULL;
	switch (setting->type)
	{
	case XSETTINGS_TYPE_INT:
	    {
	    String tmp = "";
	    int len;

		len = snprintf(tmp, 0, "%i", setting->data.v_int);
		tmp = XtMalloc(len + 1);
		len = snprintf(tmp, len + 1, "%i", setting->data.v_int);
		detailCount = 1;
		detail = (XmStringTable)XtMalloc(detailCount * sizeof(XmString));
		detail[0] = XmStringCreateLocalized(tmp);
		XtFree(tmp);
	    }
	    break;
	case XSETTINGS_TYPE_STRING:
	    detailCount = 1;
	    detail = (XmStringTable)XtMalloc(detailCount * sizeof(XmString));
	    detail[0] = XmStringCreateLocalized(XrmQuarkToString(setting->data.v_string));
	    break;
	case XSETTINGS_TYPE_COLOR:
	    {
	    String tmp = "";
	    int len;

		len = snprintf(tmp, 0, "#%02x%02x%02x %02x",
		    setting->data.v_color.red,
		    setting->data.v_color.green,
		    setting->data.v_color.blue,
		    setting->data.v_color.alpha);
		tmp = XtMalloc(len + 1);
		len = snprintf(tmp, len + 1, "#%02x%02x%02x %02x",
		    setting->data.v_color.red,
		    setting->data.v_color.green,
		    setting->data.v_color.blue,
		    setting->data.v_color.alpha);
		detailCount = 1;
		detail = (XmStringTable)XtMalloc(detailCount * sizeof(XmString));
		detail[0] = XmStringCreateLocalized(tmp);
		XtFree(tmp);
	    }
	    break;
	default:
	    printf("Unknown\n");
	    break;
	}
	XtVaSetValues(entryParent,
	    XmNdetail, detail,
	    XmNdetailCount, detailCount,
	    NULL);
	for (; detailCount > 0; detailCount--)
	{
	    XmStringFree(detail[detailCount - 1]);
	}
	XtFree((char *)detail);
    }
    XtFree(fullname);
}

static void
settingsCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
XmXsettingsCallbackStruct *cbs = (XmXsettingsCallbackStruct *)call_data;

    printSetting(&cbs->setting, client_data, (cbs->action == XSETTINGS_ACTION_DELETED));
}

int 
main(int argc, char *argv[])
{
XtAppContext        app_context;
Widget              top_level, helloworld_main;
Widget MessageBox;
Widget settings;
Widget sw;
XSettingsSetting *setting;
int num_settings;
int i;

    top_level = XtVaAppInitialize(&app_context, APP_CLASS, 
	NULL, 0,
	&argc, argv,
	fallbacks, NULL); 
#if 0
    settings = XtNameToWidget(top_level, ".XSettings");
#else
    settings = XmCreateXsettings(top_level, "XSettings", NULL, 0);
#endif

    XtGetApplicationResources(top_level, &AppResources, 
    	resources, XtNumber(resources),
    	NULL, 0);
    MessageBox = XmCreateMessageBox(top_level, "XSettingsControl", NULL, 0);
    sw = XmVaCreateScrolledWindow(MessageBox, "ContainerScrolledWindow",
    	XmNscrollingPolicy, XmAUTOMATIC,
    	NULL);
    helloworld_main = XmVaCreateContainer(sw, "xsettings_main",
    	XmNlayoutType, XmDETAIL,
    	NULL);
    XtAddCallback(MessageBox, XmNokCallback, okCallback, NULL);
    XtAddCallback(MessageBox, XmNcancelCallback, okCallback, NULL);

    if (settings)
    {
	XtVaGetValues(settings,
	    XmNnumXsettings, &num_settings,
	    XmNxsettings, &setting,
	    NULL);
	for (i = 0; i < num_settings; i++)
	{
	    printSetting(&setting[i], helloworld_main, False);
	}
	XtAddCallback(settings, XmNxsettingsCallback, settingsCallback, helloworld_main);
    }
    XtManageChild(helloworld_main);
    XtManageChild(sw);
    XtManageChild(MessageBox);

    XtRealizeWidget(top_level);

    XtAppMainLoop(app_context);

    return (0);
}
