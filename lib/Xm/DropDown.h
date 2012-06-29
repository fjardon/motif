#ifndef _XmDropDown_h
#define _XmDropDown_h

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif
    
externalref WidgetClass xmDropDownWidgetClass;

typedef struct _XmDropDownClassRec	*XmDropDownWidgetClass;
typedef struct _XmDropDownRec		*XmDropDownWidget;

/* XmIsDropDown may already be defined for Fast Subclassing */
#ifndef XmIsDropDown
#define XmIsDropDown(w) XtIsSubclass(w, xmDropDownWidgetClass)
#endif /* XmIsDropDown */

/***** Public Function Declarations *****/

/*	Function Name: XmCreateCombinationBox2
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The Widget created.
 */

extern Widget XmCreateCombinationBox2(Widget parent,
                                      char *name,
                                      ArgList args,
                                      Cardinal argCount);

/*	Function Name: XmCombinationBox2GetValue
 *	Description:   Retreives the value from the combo box.
 *	Arguments:     w - the combination box.
 *	Returns:       The value in the text widget.
 */

extern String XmCombinationBox2GetValue(Widget w);

/*
 * Variable argument list functions
 */

extern Widget XmVaCreateCombinationBox2(
                        Widget parent,
                        char *name,
                        ...);
extern Widget XmVaCreateManagedCombinationBox2(
                        Widget parent,
                        char *name,
                        ...);

/*      Function Name:  XmCombinationBox2GetLabel
 *      Description:    Returns the "label" child of the XmCombinationBox2
 *      Arguments:      w - The XmCombinationBox2 Widget
 *      Returns:        The specified child of the XmCombinationBox2
 */

extern Widget XmCombinationBox2GetLabel(Widget w);

/*      Function Name:  XmCombinationBox2GetArrow
 *      Description:    Returns the "arrow" child of the XmCombinationBox2
 *      Arguments:      w - The XmCombinationBox2 Widget
 *      Returns:        The specified child of the XmCombinationBox2
 */

extern Widget XmCombinationBox2GetArrow(Widget w);

/*      Function Name:  XmCombinationBox2GetText
 *      Description:    Returns the "text" child of the XmCombinationBox2
 *      Arguments:      w - The XmCombinationBox2 Widget
 *      Returns:        The specified child of the XmCombinationBox2
 */

extern Widget XmCombinationBox2GetText(Widget w);

/*      Function Name:  XmCombinationBox2GetList
 *      Description:    Returns the "list" child of the XmCombinationBox2
 *      Arguments:      w - The XmCombinationBox2 Widget
 *      Returns:        The specified child of the XmCombinationBox2
 */

extern Widget XmCombinationBox2GetList(Widget w);

/*      Function Name:  XmCombinationBox2GetChild
 *      Description:    Returns the child widget id of the XmCombinationBox2
 *      Arguments:      w - The XmDropDown widget
                        child - Teh component within the DropDown
 *      Returns:        The specified child of the XmCombinationBox2
 */
extern Widget XmCombinationBox2GetChild(Widget w, int child);

#if defined(__cplusplus)
}
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* _XmDropDown_h_ */
