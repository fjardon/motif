/* simple test program */

#include <Xm/Xm.h>
#include <Xm/ComboBox2.h>
#include <Xm/XmAll.h>

static int done = 0;

static void quitCB(Widget w, XtPointer client, XtPointer call) 
{
	done = 1;
}

/* note! using names of the widgets as equivalent to the labels */
static char * values[] = {
	"Mercury", "Venus",
	"Earth", "Mars",
	"Jupiter", "Saturn",
	"Uranus", "Neptune", 
	"Pluto",
};

static Widget toggles[XtNumber(values)];

static void upSCB(Widget w, XtPointer client, XtPointer call) 
{
	/* get the text from the box; if it matches a toggle, set that
	** toggle and exit
	*/
	String text = XmCombinationBox2GetValue(w);
	int i;
	for (i=0;i<XtNumber(values); i++)
		if (0 == strcmp(text, XtName(toggles[i])))
			{
			XmToggleButtonSetState(toggles[i], True, True);
			break;
			}
	XtFree((char*)text);
}

static void upTCB(Widget w, XtPointer client, XtPointer call) 
{
	/* find out which toggle is set, and set its value into the 
	** text area
	*/
	int i;
	for (i=0;i<XtNumber(values); i++)
		{
		Boolean set = False;
		XtVaGetValues(toggles[i], XmNset, &set, NULL);
		if (set)
			{
			XtVaSetValues(w, XmNvalue, XtName(toggles[i]), NULL);
			return;
			}
		}
	XtVaSetValues(w, XmNvalue, "", NULL); /* erase it */
}

static void vCB(Widget w, XtPointer client, XtPointer call) 
{
	/* confirm the sanity of the value in the text area against the 
	** list of toggles
	*/
	int i;
	String text = XmCombinationBox2GetValue(w);
	if (!text) return;
	if (0 == strcmp(text,""))
		{
		XtFree((char*)text);
		return;
		}
	for (i=0;i<XtNumber(values); i++)
		if (0 == strcmp(text, XtName(toggles[i])))
			return;
	/* printf ("Hey! got a value (%s) that doesn't match a valid value.\n", text); */
	XtVaSetValues(w, XmNvalue, "", NULL); /* erase it */
	XtFree((char*)text);
}


static void createQuit(Widget quit_parent)
{
	Widget button = XmCreatePushButton(quit_parent, "quit", NULL, 0);
	XtManageChild(button);
	XtAddCallback(button, XmNactivateCallback, quitCB, (XtPointer)NULL);
}

static void createScreen(Widget parent)
{
	Widget top = XmCreatePanedWindow(parent, "pane", NULL,0);

	Widget tab = XmCreateTabStack(top,"tab",NULL,0);

	XtManageChild(XmCreateScrolledText(tab,"explanation",NULL,0));
	
	{
	Arg args[10];
	int n;
	Widget combo;
	Widget popup;
	Widget rc = XmCreateRowColumn(tab, "rc", NULL,0);
	XtManageChild(rc);

	n=0;
	XtSetArg(args[n], XmNverify, True); n++;
	XtSetArg(args[n], XmNcustomizedCombinationBox, True); n++;
	combo = XmCreateCombinationBox2(rc, "combo", args, n);

	n=0;
	XtSetArg(args[n], XmNoverrideRedirect, True); n++;
	XtSetArg(args[n], XmNallowShellResize, True); n++;
	XtSetArg(args[n], XmNsaveUnder, True); n++;
	popup = XtCreatePopupShell("popup", topLevelShellWidgetClass, combo,
		args, n);

		{
		Widget radio = XmCreateRadioBox(popup, "radio", NULL, 0);
		int i;
		for (i=0; i<XtNumber(values); i++)
			XtManageChild(toggles[i] = XmCreateToggleButton(radio,values[i], NULL, 0));
		XtManageChild(radio);
		}
	
	XtVaSetValues(combo, XmNpopupShellWidget, popup, NULL);

	XtAddCallback(combo, XmNupdateShellCallback, upSCB, NULL);
	XtAddCallback(combo, XmNupdateTextCallback, upTCB, NULL);
	XtAddCallback(combo, XmNverifyTextCallback, vCB, NULL);

	
	XtManageChild(combo);
	}	

	createQuit(top);

	XtManageChild(tab);
	XtManageChild(top);
}

#define CLASS "Combo"

int main (argc,argv)
int  argc;
char *argv[];
{
	XtAppContext app_context;
	Widget app_shell;
	Display *display;

	XtToolkitInitialize ();
	app_context = XtCreateApplicationContext();
	display = XtOpenDisplay (app_context, NULL, argv[0], CLASS,
			NULL, 0, &argc, argv);
	if (!display)
	{
		XtWarning ("can't open display, exiting...");
		exit (0);
	}

	app_shell = XtAppCreateShell (NULL, CLASS,
			applicationShellWidgetClass, display, NULL, 0);
	XtVaSetValues(app_shell,XmNallowShellResize, True, NULL);

	/* create application */
	createScreen(app_shell);
	
	XtRealizeWidget(app_shell);

	/*	Process events, unwrapping correctly.  */
	while (!done)
	{
		XEvent event;
		XtAppNextEvent(app_context, &event);
        	XtDispatchEvent(&event);
	}

	XtDestroyWidget(app_shell);
	XtDestroyApplicationContext(app_context);
	exit(0);
}

