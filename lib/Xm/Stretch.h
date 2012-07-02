/*
 * Motif
 *
 * Copyright (c) 1987-2012, The Open Group. All rights reserved.
 *
 * These libraries and programs are free software; you can
 * redistribute them and/or modify them under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * These libraries and programs are distributed in the hope that
 * they will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with these librararies and programs; if not, write
 * to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301 USA
*/
/* 
 * HISTORY
*/ 
/* (c) Copyright 1990, Integrated Computer Solutions, Inc. */
/*   Author:  Chris D. Peterson */

#ifndef _XmStretch_h
#define _XmStretch_h

#include <Xm/Xm.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct _XmStretchWidgetInfo {
    Boolean success;		/* Was the size change sucessful? */
    Dimension width, height;	/* new width and height (in increments). */
    Position x, y;		/* only valid if success == XmStretchYes */
} XmStretchWidgetInfo;

extern WidgetClass xmStretchWidgetClass;

typedef struct _XmStretchClassRec*	XmStretchWidgetClass;
typedef struct _XmStretchRec*		XmStretchWidget;

/*	Function Name: XmCreateStretch
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The Widget created.
 */

Widget XmCreateStretch(
		Widget,
		String,
		ArgList,
		Cardinal
);



#if defined(__cplusplus)
}
#endif

#endif /* _XmStretch_h - DON'T ADD STUFF AFTER THIS #endif */
