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

#ifndef _XmTableLayout_h
#define _XmiTableLayout_h

extern WidgetClass       xmTableLayoutWidgetClass;

#ifndef XmIsTableLayout
#define XmIsTableLayout(w)	XtIsSubclass(w, xmTableLayoutWidgetClass)
#endif /* XmcIsTableLayout */

typedef struct _XmTableLayoutClassRec *XmTableLayoutWidgetClass;
typedef struct _XmTableLayoutRec *XmTableLayoutWidget;

#define	XmTableLayoutIgnoreWidth	1
#define	XmTableLayoutIgnoreHeight	2
#define	XmTableLayoutIgnoreBoth	3

#define	XmTableLayoutFillWidth	1
#define	XmTableLayoutFillHeight	2
#define	XmTableLayoutFillBoth		3

/* The return value.  This is the result of an Ok or Apply callback,
 * (which are usually labeled done and save, respectively)
 *
 */

#define	XmTableLayoutClick		1
#define	XmTableLayoutDoubleClick	2
#define	XmTableLayoutHELP		3
#define	XmTableLayoutDrag		4
#define	XmTableLayoutRelease		5
#define	XmTableLayoutDrawCell	6

typedef struct {
	int		reason;		/* why */
	XEvent		*event;		/* the button press event that did it */
	Widget		eventWidget;	/* the widget that got the event */
	int		count;		/* how many times button was pressed */
	int		row, column;	/* nearest row and column */
} XmTableLayoutClickCallbackStruct;

typedef struct {
	int		reason;		/* why */
	XEvent		*event;		/* the button press event that did it */
	Widget		eventWidget;	/* the widget that got the event */
	int		count;		/* how many times button was pressed */
	int		row, column;	/* nearest row and column */
	int		baseRow;	/* where the drag began */
	int		baseColumn;	/* where the drag began */
} XmTableLayoutDragCallbackStruct;

typedef struct {
	int		reason;		/* why */
	int		row, column;	/* row and column to draw */
	int		x, y;		/* Where to draw on TableLayout Widget */
	Dimension	width, height;
} XmTableLayoutDrawCellCallbackStruct;


#if defined(__cplusplus)
extern "C" {
#endif


int	XmTableLayoutDoLayout(
#if NeedFunctionPrototypes
	Widget	w,
	int	doit
#endif
);

#define	AdjustWrap	1

void	XmTableLayoutRowAdjust(
#if NeedFunctionPrototypes
	Widget	w,
	int	from,
	int	to,
	int	adjust,
#if defined(c_plusplus) || defined(__cplusplus)
	int	wrap = 0
#else
	int	wrap
#endif
#endif
);

extern void	XmTableLayoutInsertColumns(
#if NeedFunctionPrototypes
	Widget	w,
	int	insertPoint,
	int	howMany
#endif
);

void	XmTableLayoutInsertRows(
#if NeedFunctionPrototypes
	Widget	w,
	int	insertPoint,
	int	howMany
#endif
);

void	XmTableLayoutDeleteColumns(
#if NeedFunctionPrototypes
	Widget	w,
	int	deletePoint,
	int	howMany
#endif
);

void	XmTableLayoutDeleteRows(
#if NeedFunctionPrototypes
	Widget	w,
	int	deletePoint,
	int	howMany
#endif
);

int	XmTableLayoutXYToRowColumn(
#if NeedFunctionPrototypes
	Widget	w,
	int	x, 
	int	y,
	int	*row,
	int	*column
#endif
);

Widget	XmTableLayoutRowColumnToWidget(
#if NeedFunctionPrototypes
	Widget	table,
	int	row,
	int	column
#endif
);

#if defined(__cplusplus)
}
#endif

/* DON'T ADD STUFF AFTER THIS #endif */
#endif /* _XmTableLayout_h */
