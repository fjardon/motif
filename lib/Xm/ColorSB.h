/*
 *    Copyright 2008, Integrated Computer Solutions, Inc.
 *
 *		       All Rights Reserved.
 *
 *    AUTHOR:  Andriy Konoval
 *
 */

#ifndef _XmColorSelectionBox_h
#define _XmColorSelectionBox_h

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

#include <Xm/Ext.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct _XmColorSelectionBoxClassRec *XmColorSelectionBoxWidgetClass;
typedef struct _XmColorSelectionBoxRec *XmColorSelectionBoxWidget;

/*	Function Name: XmCreateColorSelectionBox
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The Widget created.
 */

Widget XmCreateColorSelectionBox(
#ifndef _NO_PROTO
		Widget, String, ArgList, Cardinal
#endif
		);

/*
 * Variable argument list functions
 */

extern Widget XmVaCreateColorSelectionBox(Widget parent, char *name, ...);
extern Widget XmVaCreateManagedColorSelectionBox(Widget parent, char *name, ...);
extern Widget XmCreateColorSelectionDialog(Widget w, String name, ArgList csb_args, Cardinal csb_n);
// TODO: !!! implement XmVaCreateColorSelectionDialog

extern WidgetClass xmColorSelectionBoxWidgetClass;

#if defined(__cplusplus)
}
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* _XmColorSelectionBox_h */ 
/* DON'T ADD STUFF AFTER THIS #endif */
