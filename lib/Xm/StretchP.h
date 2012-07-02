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

#ifndef _XmStretchP_h
#define _XmStretchP_h

#include <Xm/ExtP.h>
#include <Xm/FrameP.h>
#include <Xm/Stretch.h>


#ifdef __cplusplus
extern "C" {
#endif

#define N_CURSORS	11

typedef struct _XmStretchClassPart {
    XtPointer extension;
} XmStretchClassPart;

typedef struct _XmStretchClassRec {
    CoreClassPart	core_class;
    CompositeClassPart  composite_class;
    ConstraintClassPart constraint_class;
    XmManagerClassPart  manager_class;
    XmFrameClassPart	frame_class;
    XmStretchClassPart	stretch_class;
} XmStretchClassRec;

externalref XmStretchClassRec xmStretchClassRec;

typedef struct _XmStretchPart {
    /* Resources */

    XtCallbackList	resize_callback;
    
    Dimension width_inc, height_inc;	/* increments in x and y directions. */
    int max_width, max_height;	/* maximum sizes. */
    int min_width, min_height;	/* minimum sizes. */

    /* Private State */

    GC		rubber_GC;
    Widget      label_shell;	/* For displaying the current size. */
    Widget      label;
    Window 	windows[N_CURSORS];
    Cursor 	cursors[N_CURSORS];		/* for resizing */

    Position 	start_realx, start_realy;       /* for RubberBand */
    Position 	last_realx, last_realy; 	/* for RubberBand */

    Dimension 	old_width, old_height;          /* the last width and height.*/
    unsigned char sides_broken;
    
} XmStretchPart;

typedef struct _XmStretchConstraintsPart {
    XtPointer dummy;
} XmStretchConstraintsPart;

typedef struct _XmStretchConstraintsRec {
    XmManagerConstraintPart  manager;
    XmFrameConstraintPart    frame;
    XmStretchConstraintsPart stretch;
} XmStretchConstraintsRec, *XmStretchConstraints;

typedef struct _XmStretchRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    XmManagerPart	manager;
    XmFramePart		frame;
    XmStretchPart	stretch;
} XmStretchRec;

#define XmStretchIndex (XmFrameIndex + 1)

#define XmStretch_resize_callback(w)	(((XmStretchWidget)(w))->stretch.resize_callback)
#define XmStretch_width_inc(w) 		(((XmStretchWidget)(w))->stretch.width_inc)
#define XmStretch_height_inc(w) 	(((XmStretchWidget)(w))->stretch.height_inc)
#define XmStretch_max_width(w) 		(((XmStretchWidget)(w))->stretch.max_width)
#define XmStretch_max_height(w) 	(((XmStretchWidget)(w))->stretch.max_height)
#define XmStretch_min_width(w) 		(((XmStretchWidget)(w))->stretch.min_width)
#define XmStretch_min_height(w)		(((XmStretchWidget)(w))->stretch.min_height)
#define XmStretch_rubber_GC(w) 		(((XmStretchWidget)(w))->stretch.rubber_GC)
#define XmStretch_label_shell(w) 	(((XmStretchWidget)(w))->stretch.label_shell)
#define XmStretch_label(w) 		(((XmStretchWidget)(w))->stretch.label)
#define XmStretch_windows(w) ((Window*)&((XmStretchWidget)((w)))->stretch.windows)
#define XmStretch_cursors(w) ((Cursor*)&((XmStretchWidget)((w)))->stretch.cursors)
#define XmStretch_start_realx(w) 	(((XmStretchWidget)(w))->stretch.start_realx)
#define XmStretch_start_realy(w) 	(((XmStretchWidget)(w))->stretch.start_realy)
#define XmStretch_last_realx(w) 	(((XmStretchWidget)(w))->stretch.last_realx)
#define XmStretch_last_realy(w) 	(((XmStretchWidget)(w))->stretch.last_realy)
#define XmStretch_old_width(w) 		(((XmStretchWidget)(w))->stretch.old_width)
#define XmStretch_old_height(w) 	(((XmStretchWidget)(w))->stretch.old_height)
#define XmStretch_sides_broken(w) 	(((XmStretchWidget)(w))->stretch.sides_broken)

#ifdef __cplusplus
}	/* Closes scope of 'extern "C"' declaration */
#endif

#endif /* _XmStretchP_h - DON'T ADD STUFF AFTER THIS #endif */
