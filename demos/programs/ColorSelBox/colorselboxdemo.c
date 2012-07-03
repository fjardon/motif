#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/XmAll.h>
#include <Xm/Paned.h>
#include <Xm/ColorSB.h>
#include <Xm/ButtonBox.h>
#include <Xm/ToggleB.h>
#include <Xm/Text.h>

Widget top,
       colorSelectionDialog = NULL,

       bigPane, 
       rightPane, 
       colorFrame,
       infoFrame,
       resourcesFrame,
       scrolledExplain,
       infoLabel,

       buttonBox,
       leftButtonBox,
       centerButtonBox,
       rightButtonBox,
       colorExplainButton,
       hueExplainButton,
       satExplainButton,
       brightnessExplainButton,
       redExplainButton,
       greenExplainButton,
       blueExplainButton,
       helpOnExplainButton,
       helpOnValueRadioBox, 

       colorValueLabel,
       hueValueLabel,
       satValueLabel,
       brightnessValueLabel,
       redValueLabel,
       greenValueLabel,
       blueValueLabel,
       helpOnValueLabel,

       colorValueTextField,
       hueValueTextField,
       satValueTextField,
       brightnessValueTextField,
       redValueTextField,
       greenValueTextField,
       blueValueTextField,

       helpOnValueTrueToggleButton,
       helpOnValueFalseToggleButton,
       colorSelectButton,
       quitButton;

static String fallbacks[] = {
	/* General fallback resources */
	"*InfoLabel.marginWidth: 10",
	"*InfoLabel.marginHeight: 10",
	"*RightButtonBox*XmPushButton.labelString: Explain...",
	"*ColorValueLabel.labelString: XmNcolorValue",
	"*HueValueLabel.labelString: XmNhueValue",
	"*SatValueLabel.labelString: XmNsatValue",
	"*BrightnessValueLabel.labelString: XmNbrightnessValue",
	"*RedValueLabel.labelString: XmNredValue",
	"*GreenValueLabel.labelString: XmNgreenValue",
	"*BlueValueLabel.labelString: XmNblueValue",
	"*HelpOnValueLabel.labelString: XmNhelpOnValue",
	"*ColorSelectButton.labelString: Select Color",
	"*QuitButton.labelString: Quit",
	"*ButtonBox*XmLabel.alignment: XmALIGNMENT_BEGINNING",
	NULL
};

XmString getDescriptionString()
{
	XmString str = XmStringCreateLtoR(
"The ColorSelectionBox widget allows users to choose a color by \n\
using ColorMatrix or specify it in digit mode such as RGB or HSV \n\
mode. A \"Basic color\" palette with standard colors is available. \n\
A user can select a custom palette which can saved. \n\
\n\
For more information about a resource, select the Explain button \n\
to its right.", XmSTRING_DEFAULT_CHARSET);

	return str;
}

void quit(Widget w, XtPointer client_data, XtPointer call_data)
{
	exit(0);
}

Widget getInformationDialog()
{
	static Widget informationDialog = NULL;
	int n;
	Arg args[5];
	
	if (informationDialog == NULL) {
		Widget button;

		n = 0;
		XtSetArg(args[n], XmNtitle, "Explanation"); n++;
		informationDialog = XmCreateInformationDialog(top, "InformationDialog", args, n);

		button = XmMessageBoxGetChild(informationDialog, XmDIALOG_CANCEL_BUTTON);
		XtUnmanageChild(button);
		button = XmMessageBoxGetChild(informationDialog, XmDIALOG_HELP_BUTTON);
		XtUnmanageChild(button);
	}
	
	return informationDialog; 
}

void colorSelectionDialogOkCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
	int value = 0;
	char str[10];

	/* get values from colorSelectionDialog */
	XtVaGetValues(w, XmNcolorValue, &value, NULL);
	sprintf(str, "%d", value);
	XmTextSetString(colorValueTextField, str);

	/* set background of colorFrame */
	XtVaSetValues(colorFrame, XmNbackground, value, NULL); 

	XtVaGetValues(w, XmNhueValue, &value, NULL);
	sprintf(str, "%d", value);
	XmTextSetString(hueValueTextField, str);

	XtVaGetValues(w, XmNsatValue, &value, NULL);
	sprintf(str, "%d", value);
	XmTextSetString(satValueTextField, str);

	XtVaGetValues(w, XmNbrightnessValue, &value, NULL);
	sprintf(str, "%d", value);
	XmTextSetString(brightnessValueTextField, str);

	XtVaGetValues(w, XmNredValue, &value, NULL);
	sprintf(str, "%d", value);
	XmTextSetString(redValueTextField, str);

	XtVaGetValues(w, XmNgreenValue, &value, NULL);
	sprintf(str, "%d", value);
	XmTextSetString(greenValueTextField, str);

	XtVaGetValues(w, XmNblueValue, &value, NULL);
	sprintf(str, "%d", value);
	XmTextSetString(blueValueTextField, str);

	XtUnmanageChild(colorSelectionDialog);
}

void colorSelectionDialogCancelCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
	XtUnmanageChild(colorSelectionDialog);
}

void colorSelectionDialogHelpCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
	XmString helpStr = NULL;
	Widget informationDialog;
	
	informationDialog = getInformationDialog(); 

	helpStr = getDescriptionString();
	
	XtVaSetValues(informationDialog, XmNmessageString, helpStr, NULL);

	XmStringFree(helpStr);

	XtManageChild(informationDialog);
}

void selectColor(Widget w, XtPointer client_data, XtPointer call_data)
{
	Arg args[10];
	int n = 0;
	unsigned char help;

	XtVaGetValues(helpOnValueTrueToggleButton, XmNset, &help, NULL);
	
	n = 0;
	
	switch (help) {
	case XmSET:
		XtSetArg(args[n], XmNhelpOn, TRUE); n++;
		break;
	case XmUNSET:
		XtSetArg(args[n], XmNhelpOn, FALSE); n++;
		break;
	}
	
	if (colorSelectionDialog == NULL) {
		XtSetArg(args[n], XmNcolorValue, 0xFFFFFFFF); n++;
		
		colorSelectionDialog = XmCreateColorSelectionDialog(top, "ColorSelectionDialog", args, n);

		XtAddCallback(colorSelectionDialog, XmNokCallback, colorSelectionDialogOkCallback, client_data);
		XtAddCallback(colorSelectionDialog, XmNcancelCallback, colorSelectionDialogCancelCallback, client_data);
		XtAddCallback(colorSelectionDialog, XmNhelpCallback, colorSelectionDialogHelpCallback, client_data);
	} else {
		XtSetValues(colorSelectionDialog, args, n);
	}

	XtManageChild(colorSelectionDialog);
}

void explainValue(Widget w, XtPointer client_data, XtPointer call_data)
{
	XmString explanationStr = NULL;
	Widget informationDialog;
	int explain = (int)client_data;

	switch (explain) {
		case 1:
			explanationStr = XmStringCreateLtoR(
"Specifies the chosen color value.\n\n",
			XmSTRING_DEFAULT_CHARSET);
			break;
		case 2:
			explanationStr = XmStringCreateLtoR(
"Specifies the chosen color hue value.\n\n",
			XmSTRING_DEFAULT_CHARSET);
			break;
		case 3:
			explanationStr = XmStringCreateLtoR(
"Specifies the chosen color saturation value.\n\n", 
			XmSTRING_DEFAULT_CHARSET);
			break;
		case 4:
			explanationStr = XmStringCreateLtoR(
"Specifies the chosen color brightness value.\n\n", 
			XmSTRING_DEFAULT_CHARSET);
			break;
		case 5:
			explanationStr = XmStringCreateLtoR(
"Specifies the chosen red color component value.\n\n", 
			XmSTRING_DEFAULT_CHARSET);
			break;
		case 6:
			explanationStr = XmStringCreateLtoR(
"Specifies the chosen green color component value.\n\n", 
			XmSTRING_DEFAULT_CHARSET);
			break;
		case 7:
			explanationStr = XmStringCreateLtoR(
"Specifies the chosen blue color component value.\n\n", 
			XmSTRING_DEFAULT_CHARSET);
			break;
		case 8:
			explanationStr = XmStringCreateLtoR(
"Set a presence Help button.  If True, the Help button\n\
will be present at the ColorSelectinBox widget.\n\n", 
			XmSTRING_DEFAULT_CHARSET);
			break;
	}

	informationDialog = getInformationDialog();
	
	XtVaSetValues(informationDialog, XmNmessageString, explanationStr, NULL);

	XmStringFree(explanationStr);

	XtManageChild(informationDialog);
}

int main(int argc, char **argv) 
{
	int n;
	Arg args[10];
	XtAppContext app;

	XtSetLanguageProc(NULL, (XtLanguageProc) NULL, NULL); 

	top = XtVaOpenApplication(&app, "ColorSelectionBoxDemo", NULL, 0, &argc, argv, fallbacks, 
				sessionShellWidgetClass,
				XmNtitle, "Color Selection Box Demo",
				XmNallowShellResize, TRUE,
				NULL);

	bigPane = XtVaCreateManagedWidget("BigPane", xmPanedWidgetClass,
				top, XmNorientation, XmHORIZONTAL,
				NULL);

	/* right pane */
	rightPane = XtVaCreateManagedWidget("RightPane", xmPanedWidgetClass,
				bigPane, XmNshowSash, FALSE,
				XmNskipAdjust, FALSE,
				XmNorientation, XmVERTICAL,
				NULL);

	infoFrame = XtVaCreateManagedWidget("InfoFrame", xmFrameWidgetClass,
				rightPane, XmNshowSash, FALSE,
				XmNresizeToPreferred, TRUE,
				XmNpreferredPaneSize, 150,
				XmNallowResize, TRUE,
				NULL);

	scrolledExplain = XtVaCreateManagedWidget("ScrollExplain", xmScrolledWindowWidgetClass, 
				infoFrame, XmNscrollingPolicy, XmAUTOMATIC,
				XmNscrollBarDisplayPolicy, XmAS_NEEDED,
				NULL);

	infoLabel = XtVaCreateManagedWidget("InfoLabel", xmLabelWidgetClass,
				scrolledExplain, XmNalignment, XmALIGNMENT_BEGINNING,
				XmNlabelString, getDescriptionString(), NULL);

	resourcesFrame = XtVaCreateManagedWidget("ResourcesFrame", xmFrameWidgetClass,
				rightPane, XmNshowSash, FALSE, XmNresizeToPreferred, 
				FALSE, NULL);

	colorFrame = XtVaCreateManagedWidget("ColorFrame", xmFrameWidgetClass,
				rightPane, XmNshowSash, FALSE, XmNresizeToPreferred, 
				FALSE, XmNorientation, XmVERTICAL, 
				XmNpreferredPaneSize, 80, NULL);

	buttonBox = XtVaCreateManagedWidget("ButtonBox", xmButtonBoxWidgetClass,
				resourcesFrame, XmNequalSize, TRUE,
				XmNfillOption, XmFillAll, XmNorientation, 
				XmHORIZONTAL, XmNshowSash, FALSE,
				XmNskipAdjust, TRUE, NULL);

	leftButtonBox = XtVaCreateManagedWidget("LeftButtonBox", xmButtonBoxWidgetClass,
				buttonBox, XmNequalSize, TRUE, XmNfillOption, 
				XmFillMinor, XmNorientation, XmVERTICAL,
				NULL);

	centerButtonBox = XtVaCreateManagedWidget("CenterButtonBox", xmButtonBoxWidgetClass,
				buttonBox, XmNequalSize, TRUE, XmNfillOption, 
				XmFillMinor, XmNorientation, XmVERTICAL,
				NULL);

	rightButtonBox = XtVaCreateManagedWidget("RightButtonBox", xmButtonBoxWidgetClass,
				buttonBox, XmNequalSize, TRUE, XmNfillOption, 
				XmFillMinor, XmNorientation, XmVERTICAL,
				NULL);
	
	/* color value */
	colorValueLabel = XtVaCreateManagedWidget("ColorValueLabel", xmLabelWidgetClass,
    				leftButtonBox, NULL);
	colorValueTextField = XtVaCreateManagedWidget("ColorValueTextField", xmTextFieldWidgetClass,
    				centerButtonBox, XmNeditable, FALSE, NULL);
	colorExplainButton = XtVaCreateManagedWidget("ColorValueExplainButton", xmPushButtonWidgetClass,
				rightButtonBox, NULL);
	XtAddCallback(colorExplainButton, XmNactivateCallback, explainValue, (XtPointer)1);

	/* hue value */
	hueValueLabel = XtVaCreateManagedWidget("HueValueLabel", xmLabelWidgetClass,
    				leftButtonBox, NULL);
	hueValueTextField = XtVaCreateManagedWidget("HueValueTextField", xmTextFieldWidgetClass,
    				centerButtonBox, XmNeditable, FALSE, NULL);
	hueExplainButton = XtVaCreateManagedWidget("HueValueExplainButton", xmPushButtonWidgetClass,
				rightButtonBox, NULL);
	XtAddCallback(hueExplainButton, XmNactivateCallback, explainValue, (XtPointer)2);

	/* sat value */
	satValueLabel = XtVaCreateManagedWidget("SatValueLabel", xmLabelWidgetClass,
    				leftButtonBox, NULL);
	satValueTextField = XtVaCreateManagedWidget("SatValueTextField", xmTextFieldWidgetClass,
    				centerButtonBox, XmNeditable, FALSE, NULL);
	satExplainButton = XtVaCreateManagedWidget("SatValueExplainButton", xmPushButtonWidgetClass,
				rightButtonBox, NULL);
	XtAddCallback(satExplainButton, XmNactivateCallback, explainValue, (XtPointer)3);

	/* brightness value */
	brightnessValueLabel = XtVaCreateManagedWidget("BrightnessValueLabel", xmLabelWidgetClass,
    				leftButtonBox, NULL);
	brightnessValueTextField = XtVaCreateManagedWidget("BrightnessValueTextField", xmTextFieldWidgetClass,
    							   centerButtonBox, XmNeditable, FALSE, NULL);
	brightnessExplainButton = XtVaCreateManagedWidget("BrightnessValueExplainButton", xmPushButtonWidgetClass,
				rightButtonBox, NULL);
	XtAddCallback(brightnessExplainButton, XmNactivateCallback, explainValue, (XtPointer)4);

	/* red value */
	redValueLabel = XtVaCreateManagedWidget("RedValueLabel", xmLabelWidgetClass,
    				leftButtonBox, NULL);
	redValueTextField = XtVaCreateManagedWidget("RedValueTextField", xmTextFieldWidgetClass,
    				centerButtonBox, XmNeditable, FALSE, NULL);
	redExplainButton = XtVaCreateManagedWidget("RedValueExplainButton", xmPushButtonWidgetClass,
				rightButtonBox, NULL);
	XtAddCallback(redExplainButton, XmNactivateCallback, explainValue, (XtPointer)5);

	/* green value */
	greenValueLabel = XtVaCreateManagedWidget("GreenValueLabel", xmLabelWidgetClass,
    				leftButtonBox, NULL);
	greenValueTextField = XtVaCreateManagedWidget("GreenValueTextField", xmTextFieldWidgetClass,
    				centerButtonBox, XmNeditable, FALSE, NULL);
	greenExplainButton = XtVaCreateManagedWidget("GreenValueExplainButton", xmPushButtonWidgetClass,
				rightButtonBox, NULL);
	XtAddCallback(greenExplainButton, XmNactivateCallback, explainValue, (XtPointer)6);

	/* blue value */
	blueValueLabel = XtVaCreateManagedWidget("BlueValueLabel", xmLabelWidgetClass,
    				leftButtonBox, NULL);
	blueValueTextField = XtVaCreateManagedWidget("BlueValueTextField", xmTextFieldWidgetClass,
    				centerButtonBox, XmNeditable, FALSE, NULL);
	blueExplainButton = XtVaCreateManagedWidget("BlueValueExplainButton", xmPushButtonWidgetClass,
						rightButtonBox, NULL);
	XtAddCallback(blueExplainButton, XmNactivateCallback, explainValue, (XtPointer)7);

	/* help on value */
	helpOnValueLabel = XtVaCreateManagedWidget("HelpOnValueLabel", xmLabelWidgetClass,
    				leftButtonBox, NULL);

	n = 0;
	XtSetArg(args[n], XmNorientation, XmHORIZONTAL); n++;
	helpOnValueRadioBox = XmCreateRadioBox(centerButtonBox, "HelpOnValueRadioBox", args, n); 

	helpOnValueTrueToggleButton = XtVaCreateManagedWidget("HelpOnValueTrueToggleButton", xmToggleButtonWidgetClass,
    				helpOnValueRadioBox, XmNindicatorType, XmONE_OF_MANY_ROUND, 
    				XmNlabelString, XmStringCreateLocalized("TRUE"),
				NULL);
	helpOnValueFalseToggleButton = XtVaCreateManagedWidget("HelpOnValueFalseToggleButton", xmToggleButtonWidgetClass,
				helpOnValueRadioBox, XmNindicatorType, XmONE_OF_MANY_ROUND,
				XmNlabelString, XmStringCreateLocalized("FALSE"),
				NULL);
	XmToggleButtonSetState(helpOnValueTrueToggleButton, TRUE, FALSE);
	XtManageChild(helpOnValueRadioBox);

	helpOnExplainButton = XtVaCreateManagedWidget("HelpOnValueExplainButton", xmPushButtonWidgetClass,
				rightButtonBox, NULL);

	XtAddCallback(helpOnExplainButton, XmNactivateCallback, explainValue, (XtPointer) 8);


	buttonBox = XtVaCreateManagedWidget("BottomButtonBox", xmButtonBoxWidgetClass,
				rightPane, XmNequalSize, TRUE, XmNfillOption, XmFillNone,
				XmNorientation, XmHORIZONTAL, XmNshowSash, FALSE, XmNskipAdjust, 
				TRUE, NULL);

	colorSelectButton = XtVaCreateManagedWidget("ColorSelectButton", xmPushButtonWidgetClass,
						    buttonBox, NULL);

	quitButton = XtVaCreateManagedWidget("QuitButton", xmPushButtonWidgetClass,
				buttonBox, NULL);

	XtAddCallback(colorSelectButton, XmNactivateCallback, selectColor, NULL);
	XtAddCallback(quitButton, XmNactivateCallback, quit, (XtPointer) 0);

	XtRealizeWidget(top);
	XtAppMainLoop(app);

	return 0;
}

