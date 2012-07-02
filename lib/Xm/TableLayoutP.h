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
 * 
 */
/*
 * HISTORY
 */
/* (c) Copyright 1992, Integrated Computer Solutions, Inc. */


#ifndef	_XmTableLayoutP_h
#define	_XmTableLayoutP_h

#include	<Xm/XmP.h>
#include	<Xm/ManagerP.h>
#include	<Xm/PrimitiveP.h>
#include	<TableLayout.h>

/* the boiler plate for the widget itself */

typedef struct _TableLayoutClassPart {
	char	*mystuff;		/* Make the compiler happy */
} TableLayoutClassPart;


typedef struct _XmTableLayoutClassRec {
	CoreClassPart		core_class;
	CompositeClassPart	composite_class;
	ConstraintClassPart	constraint_class;
	XmManagerClassPart	manager_class;
	TableLayoutClassPart	tablelayout_class;
} XmTableLayoutClassRec;

extern XmTableLayoutClassRec	xmTableLayoutClassRec;

typedef struct _XmTableLayoutAxis {
	char	tag[8];		/* for debugging printouts */
	int	n;		/* how many things */
	int	firstScroll;	/* thing where scrolling begins (res) */
	Dimension margin;	/* space between things (res) */

	int	firstVis;	/* first one that is visable */
	int	lastVis;	/* last one visable */
	long	*width;		/* how wide each column is.  Always correct,
				 * even for columns off the scroll region
				 */
	long	*disp;		/* where it begins on the image.  May be < 0
				 * if not in scroll region.
				 */
	long	totalWidth;	/* width needed for the whole table */
	Widget	sb;		/* my scroll bar */
	long	sbX, sbY;	/* where it begins */
	short	moved;		/* have we repositioned it */
	short	mapped;		/* have we been displayed yet */
	short	size, value;	/* cache current size and value to optimize
				 * away setvalue calls to prevent flicker */
	short	oldMin, oldMax;	/* more of the same */
	Widget	inherit;	/* Inherit widths from someone else */
} XmTableLayoutAxis;

typedef struct _XmTableLayoutPart {
	int		row, col;	/* current position (RO) */
	short		fillMode;	/* pad last column or row */
	short		deferSizingChildren;	/* childrenNeedExpose */
	short		mustDisplay;
	short		mustLayout;		/* must perform layout soon */
	short		doLayout;
	short		inGeometryManager;	/* prevent recursion */
	short		inDoLayout;		/* prevent recursion */
	short		inDrawChildren;		/* prevent recursion */
	short		inInitialize;		/* prevent geometry during setup */
	XtIntervalId	timerId;	/* Timer to defer scroll bar
					 * drag processing */
	short		scrollDir;	/* last scroll direction, 0 for
					 * towards end, 1 for head */
	XmTableLayoutAxis	cols;
	XmTableLayoutAxis	rows;

	GC		gc;		/* for drawing the table */
	XtCallbackProc	clickCallback;	/* call back to client when clicked */
	XtCallbackProc	dragCallback;	/* call back to client when dragged */
	XtCallbackProc	drawCallback;	/* call back to client for imaging */
	XtCallbackProc	dropCallback;	/* call back to client when dragged */
} XmTableLayoutPart;


typedef struct _XmTableLayoutRec {
	CorePart	core;
	CompositePart	composite;
	ConstraintPart	constraint;
	XmManagerPart  manager;
	XmTableLayoutPart	tablelayout;
} XmTableLayoutRec;

typedef struct _XmTableLayoutConstraintsPart {
	int	row;		/* my position in the table */
	int	col;
	int	rowSpan;	/* how many rows/cols I span */
	int	colSpan;
	int	ignore;		/* which dimensions to ignore in layout */
	int	gravity;	/* how to align me */

	Dimension	h, w;	/* desired size from creation and explicit
				 * geometry requests from the child
				 */
	int	placed;		/* Have I been placed on the table yet */
} XmTableLayoutConstraintsPart, *XmTableLayoutConstraints;

typedef struct _XmTableLayoutConstraintsRec {
	XmManagerConstraintPart manager;
	XmTableLayoutConstraintsPart	tablelayout;
} XmTableLayoutConstraintsRec, *XmTableLayoutConstraintsPtr;

#endif	/* _XmTableLayoutP_h */
