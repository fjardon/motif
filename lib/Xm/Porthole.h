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
/* (c) Copyright 1990, Massachusetts Institute of Technology */
/*   Author:  Jim Fulton, MIT X Consortium */
/* (c) Copyright 1992, Integrated Computer Solutions, Inc. */
/*   Author:  Chris D. Peterson */

#ifndef _XmPorthole_h
#define _XmPorthole_h

#include <Xm/Xm.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* external declarations */

typedef struct _XmPortholeClassRec *XmPortholeWidgetClass;
typedef struct _XmPortholeRec      *XmPortholeWidget;

/*
 * Allows a panner to be easily connected to a porthole.
 */

/*	Function Name: XmPortholeConnectPanner
 *	Description: Allows a porthole and panner to be easily connected.
 *	Arguments: porthole - the porthole widget.
 *                 panner - the panner widget.
 *	Returns: none.
 */

void XmPortholeConnectPanner(
#ifndef _NO_PROTO
	Widget, /* porthole */
        Widget  /* panner */
#endif			     
);

/*	Function Name: XmCreatePorthole
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The Widget created.
 */

Widget XmCreatePorthole(
#ifndef _NO_PROTO
Widget, String, ArgList, Cardinal
#endif
);

void XmPortholeVisible(
#ifndef _NO_PROTO
Widget, Widget
#endif
);

extern WidgetClass 		 xmPortholeWidgetClass;

#if defined(__cplusplus)
}
#endif

#endif /* _XmPorthole_h */
