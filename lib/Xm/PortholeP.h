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

#ifndef _XmPortholeP_h
#define _XmPortholeP_h

#include <Xm/XmP.h>
#include <Xm/ManagerP.h>
#include <Xm/Porthole.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef Widget (*XmPortholeFindChildProc)(Widget);
typedef unsigned int (*XmPortholeUsableDimensionProc)(Widget);
typedef void (*XmPortholeLayoutProc)(Widget, Widget, XtWidgetGeometry *,
				     Position *, Position *,
				     Dimension *, Dimension *);
typedef void (*XmPortholePlaceChildrenProc)(Widget, Boolean,
					    Position, Position,
					    Dimension, Dimension);

#define XtInheritFindChild 	((XmPortholeFindChildProc)_XtInherit)
#define XtInheritUsable 	((XmPortholeUsableDimensionProc)_XtInherit)
#define XtInheritLayout 	((XmPortholeLayoutProc)_XtInherit)
#define XtInheritPlaceChildren 	((XmPortholePlaceChildrenProc)_XtInherit)

typedef struct _XmPortholeClassPart { /* new fields in widget class */
     XmPortholeFindChildProc 		find_child;
    XmPortholeUsableDimensionProc 	usable_width;
    XmPortholeUsableDimensionProc 	usable_height;
    XmPortholeLayoutProc		layout;
    XmPortholePlaceChildrenProc		place_children;
    XtPointer				extension;
} XmPortholeClassPart;

typedef struct _XmPortholeClassRec {	/* Porthole widget class */
    CoreClassPart 	core_class;
    CompositeClassPart 	composite_class;   
    ConstraintClassPart constraint_class;
    XmManagerClassPart  manager_class;
    XmPortholeClassPart	porthole_class;
} XmPortholeClassRec;

typedef struct _XmPortholePart {		/* new fields in widget */
    /* resources... */
    XtCallbackList report_callbacks;	/* callback/Callback */
    Boolean force_child_to_fill; /* Force the child to fill all space. */

    Dimension margin_height, margin_width;

    /* private data... */

    Window clip_win;		/* The clip window, so we don't write over
				   the shadows. */

    Widget current_child;	/* The old child that corrosponds to the 
				   preferred width and height. */
    Dimension p_child_width, p_child_height;

    XRectangle clip;		/* The clip window size. */
} XmPortholePart;

typedef struct _XmPortholeRec {
    CorePart 		core;
    CompositePart 	composite;
    ConstraintPart      constraint;
    XmManagerPart       manager;
    XmPortholePart 	porthole;
} XmPortholeRec;

/*
 * external declarations
 */

extern XmPortholeClassRec xmPortholeClassRec;

#define XmPortholeIndex (XmManagerIndex + 1)

#define XmPorthole_report_callbacks(w) 	(((XmPortholeWidget)(w))->porthole.report_callbacks)
#define XmPorthole_force_child_to_fill(w) (((XmPortholeWidget)(w))->porthole.force_child_to_fill)
#define XmPorthole_margin_height(w) 	(((XmPortholeWidget)(w))->porthole.margin_height)
#define XmPorthole_margin_width(w) 	(((XmPortholeWidget)(w))->porthole.margin_width)
#define XmPorthole_clip_win(w) 		(((XmPortholeWidget)(w))->porthole.clip_win)
#define XmPorthole_current_child(w)	(((XmPortholeWidget)(w))->porthole.current_child)
#define XmPorthole_p_child_width(w) 	(((XmPortholeWidget)(w))->porthole.p_child_width)
#define XmPorthole_p_child_height(w)	(((XmPortholeWidget)(w))->porthole.p_child_height)
#define XmPorthole_clip(w)		(((XmPortholeWidget)(w))->porthole.clip)

#ifdef __cplusplus
}	/* Closes scope of 'extern "C"' declaration */
#endif

#endif /* _XmPortholeP_h */
