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
/* (c) Copyright 1989, Massachusetts Institute of Technology */
/*   Author:  Jim Fulton, MIT X Consortium */
/* (c) Copyright 1992, Integrated Computer Solutions, Inc. */
/*   Author:  Chris D. Peterson */

#ifndef _XmPanner_h
#define _XmPanner_h

#include <Xm/Xm.h>
#include <Xm/ExtP.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* external declarations */

/*	Function Name: XmCreatePanner
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The Widget created.
 */

Widget XmCreatePanner(
#ifndef _NO_PROTO
Widget, String, ArgList, Cardinal
#endif
);

extern WidgetClass xmPannerWidgetClass;

typedef struct _XmPannerClassRec *XmPannerWidgetClass;
typedef struct _XmPannerRec      *XmPannerWidget;

#if defined(__cplusplus)
}
#endif

#endif /* _XmPanner_h */
