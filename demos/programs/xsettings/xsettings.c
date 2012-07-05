/*
 *
 * Copyright 2009 Rick Scott <rwscott@users.sourceforge.net>
 *
 */
#include <stdlib.h>
#include <limits.h>

#include <Xm/XmAll.h>        

#include "Xsettings.h"

/*
 * Fallbacks: font, label and position go together in this demo 
 */
static String fallbacks[] = {
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

static void
okCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
    XtAppSetExitFlag(XtWidgetToApplicationContext(w));
}

static void
printSetting(XSettingsSetting *setting)
{
    printf("%s:%s(%d) - \"%s\" ",
    	__FILE__, __FUNCTION__, __LINE__,
    	XrmQuarkToString(setting->name));
    switch (setting->type)
    {
    case XSETTINGS_TYPE_INT:
    	printf("%i\n", setting->data.v_int);
    	break;
    case XSETTINGS_TYPE_STRING:
    	printf("\"%s\"\n", XrmQuarkToString(setting->data.v_string));
    	break;
    case XSETTINGS_TYPE_COLOR:
    	printf("%04x %04x %04x %04x\n",
	    setting->data.v_color.red,
	    setting->data.v_color.green,
	    setting->data.v_color.blue,
	    setting->data.v_color.alpha);
    	break;
    default:
    	printf("Unknown\n");
    	break;
    }
}

static void
settingsCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
XmXsettingsCallbackStruct *cbs = (XmXsettingsCallbackStruct *)call_data;

    printSetting(&cbs->setting);
}

int 
main(int argc, char *argv[])
{
XtAppContext        app_context;
Widget              top_level, helloworld_main;
Widget MessageBox;
Widget settings;
XSettingsSetting *setting;
int num_settings;
int i;

    /* 
     * Initialize Xt and create a resizable shell 
     */

    top_level = XtVaAppInitialize(&app_context, "xsettings", 
				  NULL, 0, &argc, argv, fallbacks, NULL); 

    XtGetApplicationResources(top_level, &AppResources, 
    	resources, XtNumber(resources),
    	NULL, 0);
    /*
     *  Create the widget tree: first the bb parent.
     */
    MessageBox = XmCreateMessageBox(top_level, "XSettingsControl", NULL, 0);
    helloworld_main = XmCreateForm (MessageBox, "xsettings_main", NULL, 0);
    XtAddCallback(MessageBox, XmNokCallback, okCallback, NULL);
    XtAddCallback(MessageBox, XmNcancelCallback, okCallback, NULL);

    settings = XmCreateXsettings(MessageBox, "XSettings", NULL, 0);
    XtVaGetValues(settings,
    	XmNnumXsettings, &num_settings,
    	XmNxsettings, &setting,
    	NULL);
    for (i = 0; i < num_settings; i++)
    {
	printSetting(&setting[i]);
    }
    XtAddCallback(settings, XmNxsettingsCallback, settingsCallback, NULL);
    XtManageChild(helloworld_main);
    XtManageChild(MessageBox);

    /*
     *  Realize the toplevel widget.  This will cause the entire "managed"
     *  widget hierarchy to be displayed
     */

    XtRealizeWidget(top_level);

    /*
     *  Loop and process events
     */

    printf("%i\n", XtGetMultiClickTime(XtDisplay(top_level)));
    XtAppMainLoop(app_context);

    /* UNREACHABLE */
    return (0);
}
