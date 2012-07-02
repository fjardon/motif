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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <X11/IntrinsicP.h>
#include <Xm/XmP.h>
#include <Xm/ScrollBar.h>
#include <X11/StringDefs.h>
#include <Xm/TableLayoutP.h>
#include <Xm/TransltnsP.h>

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/cursorfont.h>
#include <X11/Shell.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "MessagesI.h"

/* defines for label string resources coming from message catalog */
#define MESSAGE1         _XmMMsgTableLayout_0001
#define MESSAGE2         _XmMMsgTableLayout_0002
#define MESSAGE3         _XmMMsgTableLayout_0003
#define MESSAGE4         _XmMMsgTableLayout_0004
#define MESSAGE5         _XmMMsgTableLayout_0005
#define MESSAGE6         _XmMMsgTableLayout_0006
#define MESSAGE7         _XmMMsgTableLayout_0007
#define MESSAGE8         _XmMMsgTableLayout_0008
#define MESSAGE9         _XmMMsgTableLayout_0009
#define MESSAGE10        _XmMMsgTableLayout_0010


extern void _XmTraversePrevTabGroup( 
                        Widget w,
                        XEvent *event,
                        String *params,
                        Cardinal *num_params) ;
extern void _XmTraverseNextTabGroup( 
                        Widget w,
                        XEvent *event,
                        String *params,
                        Cardinal *num_params) ;

static void PositionThings(XmTableLayoutWidget tw, XmTableLayoutAxis *a, XmTableLayoutAxis *other);
static void MenuTraverseRight(Widget tw, XEvent *ev, String *params, Cardinal *nparams);
static void Click(Widget tw, XEvent *ev, String *params, Cardinal *nparams);
static void Release(Widget tw, XEvent *ev, String *params, Cardinal *nparams);
static void Drag(Widget tw, XEvent *ev, String *params, Cardinal *nparams);
static void Move(Widget tw, XEvent *ev, String *params, Cardinal *nparams);

static void Initialize(
        Widget request,
        Widget new,
        ArgList nargs,
        Cardinal *num_args);

/* Resources */

static	Dimension	defWidth = 300;
static	Dimension	defHeight = 300;
static	int		defnrows = 1;
static	int		defncols = 1;
static	Dimension	defMargin = 5;

static	int		Zero = 0;


#define	offset(field) XtOffset(XmTableLayoutConstraintsPtr, tablelayout.field)
static XtResource	tableConstraintResources[] = {
	{XmNrow, XtCIndex, XtRInt, sizeof(int),
		offset(row), XtRInt, (XtPointer) &Zero},
	{XmNcolumn, XtCIndex, XtRInt, sizeof(int),
		offset(col), XtRInt, (XtPointer) &Zero},

	{XmNrowSpan, XtCReadOnly, XtRInt, sizeof(int),
		offset(rowSpan), XtRInt, (XtPointer) NULL},
	{XmNcolumnSpan, XtCReadOnly, XtRInt, sizeof(int),
		offset(colSpan), XtRInt, (XtPointer) NULL},

	{XmNgravity, XtCReadOnly, XtRInt, sizeof(int),
		offset(gravity), XtRInt, (XtPointer) NULL},
	{XmNignore, XtCReadOnly, XtRInt, sizeof(int),
		offset(ignore), XtRInt, (XtPointer) NULL}
};
#undef offset

#define	offset(field) XtOffset(XmTableLayoutWidget, field)
static XtResource	resources[] = {
	{XmNrow, XtCIndex, XtRInt, sizeof(int),
		offset(tablelayout.row), XtRInt, (caddr_t) &defnrows },
	{XmNcolumn, XtCIndex, XtRInt, sizeof(int),
		offset(tablelayout.col), XtRInt, (caddr_t) &defncols },

	{XmNrows, XtCReadOnly, XtRInt, sizeof(int),
		offset(tablelayout.rows.n), XtRInt, (caddr_t) &defnrows },
	{XmNcolumns, XtCReadOnly, XtRInt, sizeof(int),
		offset(tablelayout.cols.n), XtRInt, (caddr_t) &defncols },

	{XmNcolumnMargin, XtCMargin, XtRDimension, sizeof(Dimension),
		offset(tablelayout.cols.margin), XtRDimension, (caddr_t) &defMargin},
	{XmNrowMargin, XtCMargin, XtRDimension, sizeof(Dimension),
		offset(tablelayout.rows.margin), XtRDimension, (caddr_t) &defMargin},

	{XmNfirstScrollColumn, XtCReadOnly, XtRInt, sizeof(int),
		offset(tablelayout.cols.firstScroll), XtRInt, (caddr_t) &Zero},
	{XmNfirstScrollRow, XtCReadOnly, XtRInt, sizeof(int),
		offset(tablelayout.rows.firstScroll), XtRInt, (caddr_t) &Zero},

	{XmNclickCallback, XtCCallback, XtRCallback, sizeof(XtCallbackList),
		offset(tablelayout.clickCallback), XtRCallback, NULL },
	{XmNdragCallback, XtCCallback, XtRCallback, sizeof(XtCallbackList),
		offset(tablelayout.dragCallback), XtRCallback, NULL },
	{XmNdrawCellCallback, XtCCallback, XtRCallback, sizeof(XtCallbackList),
		offset(tablelayout.drawCallback), XtRCallback, NULL },
	{XmNfillSpace, XtCSpace, XtRShort, sizeof(short),
		offset(tablelayout.fillMode), XtRShort, (XtPointer) NULL},

	{XmNinheritWidths, XtCWindow, XtRWidget, sizeof(Widget),
		offset(tablelayout.cols.inherit), XtRWidget, (XtPointer) NULL},
	{XmNinheritHeights, XtCWindow, XtRWidget, sizeof(Widget),
		offset(tablelayout.rows.inherit), XtRWidget, (XtPointer) NULL},

	{XmNchildrenNeedExpose, XmCChildrenNeedExpose, XtRShort, sizeof(short),
		offset(tablelayout.deferSizingChildren), XtRShort, (XtPointer) NULL },
};

#undef offset


/* Actions */
static void EndDrag(Widget, XEvent *, String *, Cardinal *);

static XtActionsRec actionsList[] = {
	{"MenuTraverseRight",   MenuTraverseRight},
	{"XmTableLayoutClick",  Click},
	{"XmTableLayoutDrag",   Drag},
	{"XmTableLayoutRelease",Release},
	{"XmTableLayoutMove",   Move},
	{"XmEndDrag",          EndDrag},
	{"PrevTabGroup",        _XmTraversePrevTabGroup},
	{"NextTabGroup",        _XmTraverseNextTabGroup}
};


/* internal utility functions */
static int DoLayout();
static void DrawChildren();


/* Ascend up the hierarchy until we find the table that this child is
 * on.  It should normally be only one level, but we allow a few.  If
 * if fails we have some really serious problem.
 */

static XmTableLayoutWidget findroot(w)
	Widget	w;
{
	int	limit;

	limit = 10;

	while(w->core.widget_class != (WidgetClass) &xmTableLayoutClassRec) {
		w = w->core.parent;
		limit--;
#ifdef DEBUG
		if(w == NULL || limit <= 0) {
			XtError("xmTableLayoutWidget: findroot failure");
		}
#endif
	}
	return (XmTableLayoutWidget) (w);
}


/* We defer drawing children while scrolling so that we don't get far
 * behind the user.  The scroll bar handler simply records the current
 * value, does a clip on the axis and queues up an event to do the actual
 * display of the children a little later.  That way if more position
 * changes come in we only get the last one.
 */

/* This does the display */
static void TimerEvent(tw, id)
	XmTableLayoutWidget	tw;
	XtIntervalId	*id;
{
	if(tw->core.being_destroyed) return;
	tw->tablelayout.timerId = 0;
	DrawChildren(tw);
}

/* Position changed on scroll bars */
static void ScrollPos(wid, which, cs)
	Widget	wid;
	int	which;			/* 0 for column scroller, 1 for rows */
	XmScrollBarCallbackStruct	*cs;
{
	XmTableLayoutWidget	tw = findroot(wid);
	XmTableLayoutAxis *a, *other;

	if(tw->core.being_destroyed) return;

	if(wid == tw->tablelayout.cols.sb) {
		a = &tw->tablelayout.cols;
		other = &tw->tablelayout.rows;
	} else {
		a = &tw->tablelayout.rows;
		other = &tw->tablelayout.cols;
	}

	if(a->firstVis == cs->value) return;	/* No change */

	/* if we are scrolling back towards the beginning then the
	 * new value will be less than the current.  Reverse the
	 * direction of redraw so that we will move the things
	 * on the bottom down first to avoid unneccessary oculsions
	 * of each widget
	 */
	tw->tablelayout.scrollDir = 0;
	if(cs->value < a->firstVis) tw->tablelayout.scrollDir = 1;
	a->firstVis = cs->value;
	PositionThings(tw, a, other);

	if(cs->reason == XmCR_DRAG) {
		if(tw->tablelayout.timerId == 0) {
			tw->tablelayout.timerId = XtAppAddTimeOut(
				XtWidgetToApplicationContext(wid),
				2*XtGetMultiClickTime(XtDisplay(tw)),
				TimerEvent, (XtPointer) tw);
		}
		return;
	}
	DrawChildren(tw);
	return;
}



/* Resize and redraw the scroll bars to match the new dimensions (if needed) */

static void DrawBars(tw)
	XmTableLayoutWidget	tw;
{
	int	width, height;
	Widget	child;

	if ((child = tw->tablelayout.cols.sb)) {
		if(XtIsRealized(child) && tw->tablelayout.cols.mapped == 0) {
			tw->tablelayout.cols.mapped = 1;
			XtMapWidget(child);
		}
		width = tw->core.width - tw->tablelayout.cols.sbX;
		if(XtIsRealized(child) &&
		   (tw->tablelayout.cols.moved || width != child->core.width)) {
			XtConfigureWidget(child,
				tw->tablelayout.cols.sbX, tw->tablelayout.cols.sbY,
				width, child->core.height,
				child->core.border_width);
			tw->tablelayout.cols.moved = 0;
		}
	}
	if ((child = tw->tablelayout.rows.sb)) {
		if(XtIsRealized(child) && tw->tablelayout.rows.mapped == 0) {
			tw->tablelayout.rows.mapped = 1;
			XtMapWidget(child);
		}
		height = tw->core.height - tw->tablelayout.rows.sbY;
		if(XtIsRealized(child) &&
		   (tw->tablelayout.rows.moved || height != child->core.height)) {
			XtConfigureWidget(child,
				tw->tablelayout.rows.sbX, tw->tablelayout.rows.sbY,
				child->core.width, height,
				child->core.border_width);
			tw->tablelayout.rows.moved = 0;
		}
	}
}

/* routines to deal with axis width vectors */

static void AllocateBlankAxis(a)
	XmTableLayoutAxis *a;
{
	int	i;

	if(a->inherit) return;

	if(a->width == NULL) {
		a->width = (long *) XtCalloc(a->n+1, sizeof(long));
	}
	if(a->disp == NULL) {
		a->disp = (long *) XtCalloc(a->n+1, sizeof(long));
	}
	for(i = 0; i <= a->n; i++) {
		a->width[i] = 0;
		a->disp[i] = 0;
	}
}


static void ResizeAxis(Widget w, XmTableLayoutAxis *a, int newSize)
{
	if(a->inherit) return;

	if(newSize < 1) {
		XmeWarning(w, MESSAGE1);
		return;
	}
	if(newSize > 50) {
		XmeWarning(w, MESSAGE2);
		return;
	}
	
	if(a->width) XtFree((char *) a->width);
	a->width = NULL;
	if(a->disp) XtFree((char *) a->disp);
	a->disp = NULL;

	a->n = newSize;
	AllocateBlankAxis(a);
}


/* Is this col or row in the visiable area */

static int Visible(a, thingy)
	XmTableLayoutAxis	*a;
	int		thingy;
{
	if(a->sb == NULL) return 1;

	if(thingy < a->firstScroll) return 1;
	if(thingy >= a->firstVis && thingy <= a->lastVis) return 1;
	return 0;
}

/* Map a coordinate along an axis back to the row/column index */

static int FindPositionAxis(XmTableLayoutWidget tw, XmTableLayoutAxis *a, int pos)
{
	int	thing;
	int	lastThing;	/* track the last VISIBLE item */

	lastThing = 1;
	for(thing = 1; thing <= a->n; thing++) {
		if(a->disp[thing] < 0) continue;
		if(pos < a->disp[thing]) {
			return thing - 1;
		}
		lastThing = thing;
	}
	/* we use last thing rather than a->n in case a->n is not visible */
	if(pos < a->disp[lastThing] + a->width[lastThing]) {
		return lastThing;
	}
	return a->n + 1;		/* off the end */
}


/* Figure out the last Visible row or column */

static void ClipAxis(tw, a, max)
	XmTableLayoutWidget	tw;
	XmTableLayoutAxis *a;
	int	max;
{
	int	thing;
	Arg	args[10];
	int	ac;
	int	left;

	if(a->inherit) return;

	for(thing = 1; thing <= a->n; thing++) {
		if(a->disp[thing] > max) break;
		a->lastVis = thing;
	}
	if(a->lastVis == a->n) {
		/* The last guy is visible, see if there is space left to
		 * pop in another column at the begining.  This scenario
		 * happens when the user has scrolled to the end and then
		 * resizes the window larger, leaving more space than before
		 */
		int	i;
		left = max - (a->disp[a->n] + a->width[a->n]);
		i = a->firstVis;

		/* The cast to int is neccessary because margin is unsigned.
		 * Otherwise left < 0 appears as a big unsigned and it succeds
		 * when it should not.
		 */
		while(i>a->firstScroll && left>(int)(a->width[i-1]+a->margin)) {
			i--;
			left -= (a->width[i] + a->margin);
		}
		if(a->firstVis != i) {	/* We found space, so reposition all */
			a->firstVis = i;
			DoLayout(tw);
		}
	}

	if(a->sb) {
		int	newSize, newValue;

		ac = 0;
		newSize = a->size;
		newValue = a->value;
		if(a->lastVis >= a->firstVis) {
			/* Normal case */
			newSize = a->lastVis-a->firstVis+1;
			/* If we are at then end and it is partially obscurred
			 * then leave room to scroll further
			 */
			if(a->lastVis == a->n &&
			   ((a->disp[a->n] + a->width[a->n]) > max) &&
			   newSize > 1) {
				newSize -= 1;
			}
			newValue = a->firstVis;
			XtSetArg(args[ac], XmNminimum, a->firstScroll); ac++;
		} else {
			/* Not enough space to show things or simply nothing to show */
			newSize = 1;
			newValue = 0;
			XtSetArg(args[ac], XmNminimum, 0); ac++;
		}

		/* Only notify the scroll bar if something really has changed */
		if(a->size != newSize) {
			a->size = newSize;
			XtSetArg(args[ac], XmNsliderSize, a->size); ac++;
		}
		if(a->value != newValue) {
			a->value = newValue;
			XtSetArg(args[ac], XmNvalue, a->value); ac++;
		}
		if(a->oldMax != a->n + 1) {
			a->oldMax = a->n + 1;
			XtSetArg(args[ac], XmNmaximum, a->oldMax); ac++;
		}
		if(ac > 0) XtSetValues(a->sb, args, ac);
	}

}


/* PositionThings -  compute displacements of each col/row from
 *	the width tables and the sroll bar values.  The displacements
 * 	of columns occluded because of scrolling will be < 0.
 *	The DrawChildren routine uses that to suppress imaging.
 */

static void PositionThings(XmTableLayoutWidget tw, XmTableLayoutAxis *a, XmTableLayoutAxis *other)
{
	int	thing;
	long	lastDisp, lastWidth;
	long	adjust;
	int	oldX, oldY;		/* old scroll bar positions to */
	int	otherOldX, otherOldY;	/* call to move widget */

	if(a->inherit) return;

	oldX = a->sbX;
	oldY = a->sbY;
	otherOldX = other->sbX;
	otherOldY = other->sbY;
	a->disp[0] = 0;
	a->width[0] = 0;
	a->totalWidth = 0;
	lastWidth = 0;
	lastDisp = 0;
	for(thing = 1; thing <= a->n; thing++) {
		a->totalWidth += a->width[thing] + a->margin;

		if(a->width[thing] == 0) a->width[thing] = a->margin;

		if(a->sb && thing == a->firstScroll) {
			/* if the row scroller (vertical) set the y pos */
			if(a->sb == tw->tablelayout.rows.sb) {
				a->sbY = lastDisp + lastWidth;
			} else {
				a->sbX = lastDisp + lastWidth;
			}
		}

		/* if the other axis has a scroll bar, place it between
		 * my last fixed and first visible
 		 */
		if(other->sb && thing == a->firstScroll) {

			/* if other is vertical pick the width, else height */
			if(other->sb == tw->tablelayout.rows.sb) {
				other->sbX = lastDisp + lastWidth;
				adjust = other->sb->core.width;
				a->sbX += adjust;
			} else {
				other->sbY = lastDisp + lastWidth;
				adjust = other->sb->core.height;
				a->sbY += adjust;
			}
			lastDisp += adjust + 2 * other->sb->core.border_width;
		}

		if(a->sb) {
			if(a->firstScroll <= thing && thing < a->firstVis) {
				a->disp[thing] = -1;
				continue;
			}
		}

		a->disp[thing] = lastDisp + lastWidth + a->margin;
		lastDisp = a->disp[thing];
		lastWidth = a->width[thing];
	}

	/* there are not enough items to begin scrolling */
	if(a->n < a->firstScroll) {
		if(a->sb) {
			/* if the row scroller (vertical) set the y pos */
			if(a->sb == tw->tablelayout.rows.sb) {
				a->sbY = lastDisp + lastWidth;
			} else {
				a->sbX = lastDisp + lastWidth;
			}
		}
		/* if the other axis has a scroll bar, place it between
		 * my last fixed and first visible
 		 */
		if(other->sb) {
			/* if other is vertical pick the width, else height */
			if(other->sb == tw->tablelayout.rows.sb) {
				other->sbX = lastDisp + lastWidth;
				adjust = other->sb->core.width;
				a->sbX += adjust;
			} else {
				other->sbY = lastDisp + lastWidth;
				adjust = other->sb->core.height;
				a->sbY += adjust;
			}
			lastDisp += adjust + 2 * other->sb->core.border_width;
		}
	}

	/* If we moved the scroll bars mark that fact for use during next
	 * draw cycle.
	 */
	if(a->sbX != oldX || a->sbY != oldY) a->moved = 1;
	if(other->sbX != otherOldX || other->sbY != otherOldY) other->moved = 1;
}

/* Compute the start positions along each axis.
 * Assumes that the width of each cell along each axis was calculated by
 * DoLayout (or some other means)
 */

static void ComputeRowColPositions(tw)
	XmTableLayoutWidget	tw;
{
	int	spaceLeft, row, col;

	/* Now that we have the widths for the columns, compute x,y offsets
	 * for each column/row and the scroll bars
	 */
	tw->tablelayout.cols.moved = tw->tablelayout.rows.moved = 0;
	PositionThings(tw, &tw->tablelayout.cols, &tw->tablelayout.rows);
	PositionThings(tw, &tw->tablelayout.rows, &tw->tablelayout.cols);

	/* Should we stretch the last cell to fill any room left? */

	if((tw->tablelayout.fillMode & XmTableLayoutFillWidth) &&
	   tw->tablelayout.cols.inherit == NULL) {
		col = tw->tablelayout.cols.n;
		spaceLeft = tw->core.width - tw->tablelayout.cols.disp[col];
		if(spaceLeft > 0) tw->tablelayout.cols.width[col] = spaceLeft;
	}

	if((tw->tablelayout.fillMode & XmTableLayoutFillHeight) &&
	   tw->tablelayout.cols.inherit == NULL) {
		row = tw->tablelayout.rows.n;
		spaceLeft = tw->core.height - tw->tablelayout.rows.disp[row];
		if(spaceLeft > 0) tw->tablelayout.rows.width[row] = spaceLeft;
	}
	return;
}


/* Lay out positions of all children.
 * Returns: 0 for OK, otherwise no layout was really done.
 *
 */

static int DoLayout(tw)
	XmTableLayoutWidget	tw;
{
	int	i, row, col;
	Widget	child, parent;
	XmTableLayoutConstraintsPtr	tc;
	XtWidgetGeometry	intend, pref;

	if(tw->tablelayout.inDoLayout) {
		return 1;
	}

	if(tw->tablelayout.doLayout == 0) return 1;

	if(tw->tablelayout.cols.n < 1) return 1;
	if(tw->tablelayout.rows.n < 1) return 1;

	parent = XtParent(tw);
	tw->tablelayout.inDoLayout = 1;

	AllocateBlankAxis(&tw->tablelayout.cols);
	AllocateBlankAxis(&tw->tablelayout.rows);

	if(tw->tablelayout.cols.inherit) {
		XmTableLayoutWidget control = (XmTableLayoutWidget) tw->tablelayout.cols.inherit;
		tw->tablelayout.cols.width = control->tablelayout.cols.width;
		tw->tablelayout.cols.disp = control->tablelayout.cols.disp;
	}

	for(i = 0; i < tw->composite.num_children; i++) {
		child = tw->composite.children[i];
		if(!XtIsManaged(child)) continue;

		if(child == tw->tablelayout.cols.sb) continue;
		if(child == tw->tablelayout.rows.sb) continue;

		tc = (XmTableLayoutConstraintsPtr) child->core.constraints;
		if(tc == NULL) {
			XmeWarning((Widget)tw, MESSAGE3);
			continue;
		}
		col = tc->tablelayout.col;
		row = tc->tablelayout.row;
		if(col <= 0 || col > tw->tablelayout.cols.n ||
		   row <= 0 || row > tw->tablelayout.rows.n) {
			fprintf(stderr, MESSAGE4,
				tc->tablelayout.row, tc->tablelayout.col);
			continue;
		}

		if(tc->tablelayout.placed < 0) {
			/* If not placed on screen yet, try to find out
			 * the desired dimensions
			 */
			if(tw->tablelayout.deferSizingChildren) {
				/* Go ask the widget */
				intend.request_mode = CWWidth | CWHeight;
				intend.width = intend.height = 1;
				XtQueryGeometry(child, &intend, &pref);
			} else {
				/* use our cache */
				pref.width = tc->tablelayout.w;
				pref.height = tc->tablelayout.h;
			}
		} else {
		/* Take the preferred dimensions from the our cache, rather
		 * than than querying the child.  Too many widgets just return
		 * the current value, rather than recomputing minimum size.
		 * This makes rows and columns get bigger and bigger as we add
		 * new things if we have spanning in effect.
		 */
			pref.width = tc->tablelayout.w;
			pref.height = tc->tablelayout.h;
		}

		/* account for the border */
		pref.width += 2 * child->core.border_width;
		pref.height += 2 * child->core.border_width;

		if(!(tc->tablelayout.ignore & XmTableLayoutIgnoreWidth)) {
			if(tc->tablelayout.colSpan <= 1) {
				if(tw->tablelayout.cols.width[col] < pref.width) {
				    tw->tablelayout.cols.width[col] = pref.width;
				}
			}
		}
		if(!(tc->tablelayout.ignore & XmTableLayoutIgnoreHeight)) {
			if(tc->tablelayout.rowSpan <= 1) {
				if(tw->tablelayout.rows.width[row] < pref.height) {
				    tw->tablelayout.rows.width[row] = pref.height;
				}
			}
		}
	}

	ComputeRowColPositions(tw);

	tw->tablelayout.inDoLayout = 0;
	tw->tablelayout.mustLayout = 0;
	return 0;
}

/* Try to become the right size to fit our children perfectly */
static int RequestSize(XmTableLayoutWidget tw)
{
	XtGeometryResult ret;
	Dimension	w, h;
	Dimension	grantedW, grantedH;
	Widget		sbar;

	if (!XtIsRealized((Widget)tw))
	        return XtGeometryNo;	/* We are not layed out */

	w = tw->tablelayout.cols.totalWidth;
	if ((sbar = tw->tablelayout.rows.sb)) {
		w += sbar->core.width + sbar->core.border_width;
	}
	if(tw->tablelayout.cols.inherit) w = tw->core.width;

	h = tw->tablelayout.rows.totalWidth;
	if ((sbar = tw->tablelayout.cols.sb)) {
		h += sbar->core.height + sbar->core.border_width;
	}
	if(tw->tablelayout.rows.inherit) w = tw->core.height;

	/* Ask parent for permisson */
	ret = XtMakeResizeRequest((Widget)tw, w, h, &grantedW, &grantedH);
	if(ret == XtGeometryAlmost) {
		/* They suggested a compromise, so ask for that */
		ret = XtMakeResizeRequest((Widget)tw, grantedW, grantedH, NULL, NULL);
	}
	return ret;
}


static void DrawAChild(tw, child)
	XmTableLayoutWidget	tw;
	Widget		child;
{
	int	row, col, span;
	XmTableLayoutConstraintsPtr	tc;
	int	width, height, border, x, y, disp;
	char	mesg[1000];

	tc = (XmTableLayoutConstraintsPtr) child->core.constraints;
	if(tc == NULL) {
		XtError(MESSAGE5);
		return;
	}
	col = tc->tablelayout.col;
	row = tc->tablelayout.row;
	if((col <= 0 || col > tw->tablelayout.cols.n) ||
	   (row <= 0 || row > tw->tablelayout.rows.n)) {
		snprintf(mesg, sizeof(mesg), MESSAGE4, row, col);
		XmeWarning((Widget)tw, mesg);
		return;
	}

	x =tw->tablelayout.cols.disp[col];
	y =tw->tablelayout.rows.disp[row];
	width = tw->tablelayout.cols.width[col];
	height = tw->tablelayout.rows.width[row];
	border = child->core.border_width;

	if((span = tc->tablelayout.colSpan) > 1) {
		if(span > tw->tablelayout.cols.n - col + 1) {
			span = tw->tablelayout.cols.n - col + 1;
		}
		width = tw->tablelayout.cols.disp[col+span-1] -
				tw->tablelayout.cols.disp[col] +
				tw->tablelayout.cols.width[col+span-1];
	}
	if((span = tc->tablelayout.rowSpan) > 1) {
		if(span > tw->tablelayout.rows.n - row + 1) {
			span = tw->tablelayout.rows.n - row + 1;
		}
		height = tw->tablelayout.rows.disp[row+tc->tablelayout.rowSpan-1] -
				tw->tablelayout.rows.disp[row] +
				tw->tablelayout.rows.width[row+tc->tablelayout.rowSpan-1];
	}
	width -= 2 * border;
	height -= 2 * border;

	/* Unmap children that are not visible */
	if((width <= 0 || height <= 0) ||
	   (col > tw->tablelayout.cols.lastVis) ||
	   (row > tw->tablelayout.rows.lastVis) ||
	   (tw->tablelayout.cols.disp[col] < 0) ||
	   (tw->tablelayout.rows.disp[row] < 0)) {
		if(XtIsRealized(child) && child->core.visible) {
			XtUnmapWidget(child);
			tc->tablelayout.placed = 0;
		}
		return;
	}

	/* place me where I belong */

	if(!tc->tablelayout.placed) {
		XtMapWidget(child);
	}
	tc->tablelayout.placed = 1;

	/* Flush bottom */
	if(tc->tablelayout.gravity == SouthWestGravity ||
	   tc->tablelayout.gravity == SouthGravity ||
	   tc->tablelayout.gravity == SouthEastGravity ) {
		disp = height - tc->tablelayout.h;
		if(disp > 0) {
			y += disp;
			height -= disp;
		}
	}
	/* Flush left */
	if(tc->tablelayout.gravity == NorthWestGravity ||
	   tc->tablelayout.gravity == WestGravity ||
	   tc->tablelayout.gravity == SouthWestGravity ) {
		disp = width - tc->tablelayout.w;
		if(disp > 0) {
			width -= disp;
		}
	}
	/* Flush right */
	if(tc->tablelayout.gravity == NorthEastGravity ||
	   tc->tablelayout.gravity == EastGravity ||
	   tc->tablelayout.gravity == SouthEastGravity ) {
		disp = width - tc->tablelayout.w;
		if(disp > 0) {
			x += disp;
			width -= disp;
		}
	}

	/* Horizontal centering */
	if(!(tc->tablelayout.ignore & XmTableLayoutIgnoreWidth) &&
	   (tc->tablelayout.gravity == NorthGravity ||
	    tc->tablelayout.gravity == CenterGravity ||
	    tc->tablelayout.gravity == SouthGravity ) ) {
		disp = width - tc->tablelayout.w;
		if(disp > 0) {
			x += disp / 2;
			width -= disp;
		}
	}
	/* Vertical centering */
	if(!(tc->tablelayout.ignore & XmTableLayoutIgnoreHeight) &&
	   (tc->tablelayout.gravity == WestGravity ||
	    tc->tablelayout.gravity == CenterGravity ||
	    tc->tablelayout.gravity == EastGravity ) ) {
		disp = height - tc->tablelayout.h;
		if(disp > 0) {
			y += disp / 2;
			height -= disp;
		}
	}


	XmeConfigureObject(child, x, y,
		width, height, child->core.border_width);
}


static void UserDraw(tw)
	XmTableLayoutWidget	tw;
{
	int	row, col;
	XmTableLayoutDrawCellCallbackStruct	cs;

	/* Now image the other things */
	cs.reason = XmTableLayoutDrawCell;
	for(col = tw->tablelayout.cols.firstVis;
	    col <= tw->tablelayout.cols.lastVis; col++) {
		cs.column = col;
		cs.x = tw->tablelayout.cols.disp[col];
		cs.width = tw->tablelayout.cols.width[col];

		for(row = tw->tablelayout.rows.firstVis;
		    row <= tw->tablelayout.rows.lastVis; row++) {
			cs.row = row;
			cs.y = tw->tablelayout.rows.disp[col];
			cs.height = tw->tablelayout.rows.width[col];
		}
	}
}

/* Place all the children on the screen.
 * This is only called after DoLayout has figured out the column positions
 */

static void DrawChildren(tw)
	XmTableLayoutWidget	tw;
{
	Widget	child;
	int	i;

	if(!XtIsRealized((Widget)tw)) return;

	ClipAxis(tw, &tw->tablelayout.rows, tw->core.height);
	ClipAxis(tw, &tw->tablelayout.cols, tw->core.width);

	DrawBars(tw);

	/* move all the children to the right spot */
	for(i = 0; i < tw->composite.num_children; i++) {
		if(tw->tablelayout.scrollDir == 0) {
			child = tw->composite.children[i];
		} else {
			child = tw->composite.children[
					tw->composite.num_children-1-i];
		}

		/* Skip the losers */
		if(!XtIsManaged(child)) continue;
		if(child == tw->tablelayout.cols.sb) continue;
		if(child == tw->tablelayout.rows.sb) continue;

		DrawAChild(tw, child);
	}

	UserDraw(tw);				/* Do the user imaging */

	tw->tablelayout.mustDisplay = 0;		/* compress away exposures */
	tw->tablelayout.scrollDir = 0;
	tw->tablelayout.inDrawChildren--;		/* recusion counters */

	return;
}

/* Handle Button Presses */

/* Current pos so we only trigger the drag callback when we change boxes
 * This should be a member of the widget struct, but you really can't have
 * two drags going at once (in X11 at least), so it won't matter
 */
static	int	DragRow, DragCol, BaseRow, BaseCol;

static void MenuTraverseRight(Widget tw, XEvent *ev, String *params, Cardinal *nparams)
{
}

static void Click(eWidget, ev, params, nparams)
	Widget	eWidget;
	XEvent	*ev;
	String	*params;
	Cardinal *nparams;
{
        XButtonEvent	*be = &ev->xbutton;
	XmTableLayoutClickCallbackStruct	cs;
	XmTableLayoutConstraintsPtr tc;
	static int	lastTime = 0;
	static Widget	lastWidget = NULL;
	static int	clickCount = 0;
	XmTableLayoutWidget	tw = findroot(eWidget);

	cs.reason = XmTableLayoutClick;
	cs.eventWidget = eWidget;
	cs.event = ev;

	if(be->type == ButtonPress &&
	   lastWidget == eWidget &&
	   be->time - lastTime < 200) {
		/* Double click */
		clickCount += 1;
	} else {
		clickCount = 1;
	}
	lastTime = be->time;
	lastWidget = eWidget;
	cs.count = clickCount;

	if((Widget) tw == eWidget) {
		for(cs.column = 0; cs.column < tw->tablelayout.cols.n; cs.column++) {
			if(be->x < tw->tablelayout.cols.disp[cs.column+1]) break;
		}
		for(cs.row = 0; cs.row < tw->tablelayout.rows.n; cs.row++) {
			if(be->y < tw->tablelayout.rows.disp[cs.row+1]) break;
		}
	} else {
            if(eWidget->core.parent == (Widget) tw) {
                tc = (XmTableLayoutConstraintsPtr) eWidget->core.constraints;
                /* Right on the child */
                cs.row = tc->tablelayout.row;
                cs.column = tc->tablelayout.col;
		} else {
			return;
                }
        }
        BaseCol = DragCol = cs.column;
        BaseRow = DragRow = cs.row;
        XtCallCallbacks((Widget)tw, XmNclickCallback, &cs);
}


static void Drag(eWidget, ev, params, nparams)
	Widget	eWidget;
	XEvent	*ev;
	String	*params;
	Cardinal *nparams;
{
	XButtonEvent	*be = &ev->xbutton;
	XmTableLayoutDragCallbackStruct	cs;
	XmTableLayoutWidget	tw = findroot(eWidget);
	int		x, y;

	cs.reason = XmTableLayoutDrag;
	cs.eventWidget = eWidget;
	cs.event = ev;
	cs.count = 0;
	cs.row = DragRow;
	cs.column = DragCol;
	cs.baseRow = BaseRow;
	cs.baseColumn = BaseCol;

	if((Widget) tw == eWidget) {
		x = be->x;
		y = be->y;
	} else if(eWidget->core.parent == (Widget) tw) {
		x = be->x + eWidget->core.x;
		y = be->y + eWidget->core.y;
	} else {
		return;
	}

	/* This might be a debatable policy in the future.  For now we
	 * do not want to report drags when the cursor is off the edge
	 */
	if(x < 0 || y < 0) return;
	if(x > tw->core.x + tw->core.width) return;
	if(y > tw->core.y + tw->core.height) return;

	for(cs.column = 0; cs.column < tw->tablelayout.cols.n; cs.column++) {
		if(x < tw->tablelayout.cols.disp[cs.column+1]) break;
	}
	for(cs.row = 0; cs.row < tw->tablelayout.rows.n; cs.row++) {
		if(y < tw->tablelayout.rows.disp[cs.row+1]) break;
	}

	if(DragCol != cs.column || DragRow != cs.row) {
		DragRow = cs.row;
		DragCol = cs.column;
		XtCallCallbacks(eWidget, XmNdragCallback, &cs);
	}
}


static void Release(eWidget, ev, params, nparams)
	Widget	eWidget;
	XEvent	*ev;
	String	*params;
	Cardinal *nparams;
{
	XmTableLayoutClickCallbackStruct	cs;
	XmTableLayoutWidget	tw = findroot(eWidget);

	cs.reason = XmTableLayoutRelease;
	cs.eventWidget = eWidget;
	cs.event = ev;
	cs.count = 0;
	cs.row = 0;
	cs.column = 0;
        
	XtCallCallbacks((Widget)tw, XmNclickCallback, &cs);
}



/*	Dunction Name: EndDrag
 *	Description:   Called when the drag starts.
 *	Arguments:     w - the icon box widget.
 *			event - the event that caused this action.
 *			params, num_params - action routine parameters.
 *	Returns:       none.
 */


/*ARGSUSED*/
static void
EndDrag(Widget w, XEvent * event, String * params, Cardinal * num_params)
{
}



/* Handle cursor contol actions */

static void Move(eWidget, ev, params, nparams)
	Widget	eWidget;
	XEvent	*ev;
	String	*params;
	Cardinal *nparams;
{
	XButtonEvent	*be = &ev->xbutton;
	XmTableLayoutWidget	tw = findroot(eWidget);
	int	row, column;
	XmTableLayoutConstraintsPtr tc;
	XmTableLayoutAxis *a = NULL, 
			  *other = NULL;
	int	inc = 0;

	if(*nparams != 1) return;

	if((Widget) tw == eWidget) {	/* Key press on empty space */
		XmTableLayoutXYToRowColumn(eWidget, be->x, be->y, &row, &column);
	} else {			/* Key Press on a widget */
		if(eWidget->core.parent == (Widget) tw) {
			tc = (XmTableLayoutConstraintsPtr) eWidget->core.constraints;
			/* Right on the child */
			row = tc->tablelayout.row;
			column = tc->tablelayout.col;
		} else {
			return;		/* Should not really happen */
		}
	}

	if(params[0][0] == 'u') {
		a = &tw->tablelayout.rows;
		other = &tw->tablelayout.cols;
		inc = -1;
	}
	if(params[0][0] == 'd') {
		a = &tw->tablelayout.rows;
		other = &tw->tablelayout.cols;
		inc = 1;
	}
	if(params[0][0] == 'l') {
		a = &tw->tablelayout.cols;
		other = &tw->tablelayout.rows;
		inc = -1;
	}
	if(params[0][0] == 'r') {
		a = &tw->tablelayout.cols;
		other = &tw->tablelayout.rows;
		inc = 1;
	}

	if(inc < 0 && (a->firstScroll >= a->firstVis)) {
		inc = 0;
	} else if(inc > 0 && (a->lastVis >= a->n)) {
		inc = 0;
	}

	if(inc) {
		tw->tablelayout.scrollDir = 0;
		if(inc < 0) tw->tablelayout.scrollDir = 1;
		a->firstVis += inc;
		PositionThings(tw, a, other);
		DrawChildren(tw);
	}
}


/* Class functions */


static void ClassInitialize()
{
}


/* Initialize a new TableLayout widget */

static void Initialize(
        Widget request,
        Widget new,
        ArgList nargs,
        Cardinal *num_args)
{
	XmTableLayoutWidget	tw = (XmTableLayoutWidget) new;
	int	ac;
	Arg	args[30];
	
	tw->tablelayout.inInitialize = 1;
	/* basic things */

	if(tw->core.width <= 0) tw->core.width = defWidth;
	if(tw->core.height <= 0) tw->core.height = defHeight;

	tw->tablelayout.col = 0;
	tw->tablelayout.cols.n = 1;
	tw->tablelayout.cols.width = NULL;
	tw->tablelayout.cols.disp = NULL;
	tw->tablelayout.cols.margin = defMargin;

	tw->tablelayout.row = 0;
	tw->tablelayout.rows.n = 1;
	tw->tablelayout.rows.width = NULL;
	tw->tablelayout.rows.disp = NULL;
	tw->tablelayout.rows.margin = defMargin;

	/* We should get our own GC from somewhere */
	tw->tablelayout.mustDisplay = 0;
	tw->tablelayout.mustLayout = 1;
	tw->tablelayout.doLayout = 1;
	tw->tablelayout.inGeometryManager = 0;
	tw->tablelayout.inDrawChildren = 0;
	tw->tablelayout.inDoLayout = 0;
	tw->tablelayout.timerId = 0;
	tw->tablelayout.scrollDir = 0;

	strcpy(tw->tablelayout.cols.tag, "horiz");
	strcpy(tw->tablelayout.rows.tag, "vert");
	/* Protect against a horribly confusing case */
	if(tw->tablelayout.cols.inherit) tw->tablelayout.cols.firstScroll = 0;
	if(tw->tablelayout.rows.inherit) tw->tablelayout.rows.firstScroll = 0;

	if(tw->tablelayout.cols.firstScroll > 0) {
		ac = 0;
		XtSetArg(args[ac], XmNorientation, XmHORIZONTAL); ac++;
		XtSetArg(args[ac], XmNmappedWhenManaged, 0); ac++;
		XtSetArg(args[ac], XmNrow, -1); ac++;
		XtSetArg(args[ac], XmNcolumn, -1); ac++;
		tw->tablelayout.cols.sb = XtCreateManagedWidget("hsb",
				xmScrollBarWidgetClass, new, args, ac);
		XtAddCallback(tw->tablelayout.cols.sb,XmNdragCallback,ScrollPos,0);
		XtAddCallback(tw->tablelayout.cols.sb,XmNvalueChangedCallback,ScrollPos,0);
		tw->tablelayout.cols.firstVis = tw->tablelayout.cols.firstScroll;
		tw->tablelayout.cols.moved = 1;
		tw->tablelayout.cols.mapped = 0;
		tw->tablelayout.cols.size = 0;
		tw->tablelayout.cols.value = 0;
		tw->tablelayout.cols.oldMax = -1;
		tw->tablelayout.cols.sbX = 0;
		tw->tablelayout.cols.sbY = 0;
	} else {
		tw->tablelayout.cols.firstVis = 1;
		tw->tablelayout.cols.firstScroll = 1;
		tw->tablelayout.cols.sb = NULL;
	}
	tw->tablelayout.cols.lastVis = tw->tablelayout.cols.firstVis;

	if(tw->tablelayout.rows.firstScroll > 0) {
		ac = 0;
		XtSetArg(args[ac], XmNorientation, XmVERTICAL); ac++;
		XtSetArg(args[ac], XmNmappedWhenManaged, 0); ac++;
		XtSetArg(args[ac], XmNrow, -1); ac++;
		XtSetArg(args[ac], XmNcolumn, -1); ac++;
		tw->tablelayout.rows.sb = XtCreateManagedWidget("vsb",
				xmScrollBarWidgetClass, new, args, ac);
		XtAddCallback(tw->tablelayout.rows.sb, XmNdragCallback, ScrollPos, (XtPointer)1);
		XtAddCallback(tw->tablelayout.rows.sb, XmNvalueChangedCallback, ScrollPos, (XtPointer)1);
		tw->tablelayout.rows.firstVis = tw->tablelayout.rows.firstScroll;
		tw->tablelayout.rows.moved = 1;
		tw->tablelayout.rows.mapped = 0;
		tw->tablelayout.rows.size = 0;
		tw->tablelayout.rows.value = 0;
		tw->tablelayout.rows.oldMax = -1;
		tw->tablelayout.rows.sbX = 0;
		tw->tablelayout.rows.sbY = 0;
	} else {
		tw->tablelayout.rows.firstVis = 1;
		tw->tablelayout.rows.firstScroll = 1;
		tw->tablelayout.rows.sb = NULL;
	}
	tw->tablelayout.rows.lastVis = tw->tablelayout.rows.firstVis;

	tw->tablelayout.inInitialize = 0;
}

static void ConstraintInitialize(request, cur)
	Widget	request;
	Widget	cur;
{
	XmTableLayoutConstraintsPtr tc = (XmTableLayoutConstraintsPtr)cur->core.constraints;
	XmTableLayoutWidget	tw = findroot(cur);

	if(tw->tablelayout.inInitialize) return;

	if(tw->tablelayout.row < 1) tw->tablelayout.row = 1;
	if(tw->tablelayout.col < 1) tw->tablelayout.col = 1;

	/* If user does not specify a position then work from last ones, or
	 * compute it from the absolute position (to work nicely with BX)
	 */
	if(tc->tablelayout.row == 0) {
		tc->tablelayout.row = tw->tablelayout.row;
		if(XtIsRealized((Widget)tw)) {
			tc->tablelayout.row = FindPositionAxis(tw, &tw->tablelayout.rows, cur->core.y);
			if(tc->tablelayout.row < 1) tc->tablelayout.row = 1;
		}
	}
	if(tc->tablelayout.col == 0) {
		tc->tablelayout.col = tw->tablelayout.col;
		if(XtIsRealized((Widget)tw)) {
			tc->tablelayout.col = FindPositionAxis(tw, &tw->tablelayout.cols, cur->core.x);
			if(tc->tablelayout.col < 1) tc->tablelayout.col = 1;
		}
	}

	if(tc->tablelayout.rowSpan < 1) tc->tablelayout.rowSpan = 1;
	if(tc->tablelayout.colSpan < 1) tc->tablelayout.colSpan = 1;

	/* if the tablelayout is not doing managment on the axis we can streamline
	 * things by ignoring widths
	 */
	if(tw->tablelayout.cols.inherit) tc->tablelayout.ignore |= XmTableLayoutIgnoreWidth;
	if(tw->tablelayout.rows.inherit) tc->tablelayout.ignore |= XmTableLayoutIgnoreHeight;
	if(tc->tablelayout.rowSpan > 1) tc->tablelayout.ignore |= XmTableLayoutIgnoreHeight;
	if(tc->tablelayout.colSpan > 1) tc->tablelayout.ignore |= XmTableLayoutIgnoreWidth;

	tw->tablelayout.row = tc->tablelayout.row;
	tw->tablelayout.col = tc->tablelayout.col;

	/* save current widget state */
	tc->tablelayout.h = cur->core.height;
	tc->tablelayout.w = cur->core.width;
	tc->tablelayout.placed = -1;		/* indicate never displayed */
	if(XtIsRealized((Widget)tw)) {
		/* If the tablelayout is up already check the new child for
		 * visibility.  If they are out of the scroll region then
		 * move them off the edge or else they would display at
		 * 0,0
		 */
		if(!Visible(&tw->tablelayout.cols, tc->tablelayout.col) ||
		   !Visible(&tw->tablelayout.rows, tc->tablelayout.row)) {
			/* Place it off the edge */
			cur->core.x = tw->core.width + 1;
			cur->core.y = tw->core.height + 1;
		}
	}

	/* Adjust any things needed in the tablelayout */
	if(tw->tablelayout.rows.n < tw->tablelayout.row) {
		ResizeAxis((Widget)tw, &tw->tablelayout.rows, tw->tablelayout.row);
	}
	if(tw->tablelayout.cols.n < tw->tablelayout.col) {
		ResizeAxis((Widget)tw, &tw->tablelayout.cols, tw->tablelayout.col);
	}

	/* XtOverrideTranslations(cur, tw->core.tm.translations); */
	XtAugmentTranslations(cur, tw->core.tm.translations);
}



static void Realize(Widget w, Mask *valueMask, XSetWindowAttributes *attributes)
{
        XmTableLayoutWidget tw = (XmTableLayoutWidget)w;

	(*xmTableLayoutClassRec.core_class.superclass->core_class.realize)
		(w, valueMask, attributes);

	tw->tablelayout.doLayout = 1;

	DoLayout(tw);
	RequestSize(tw);
	tw->tablelayout.mustDisplay = 1;
}


static void	Destroy(w)
	Widget	w;
{
	XmTableLayoutWidget	tw = (XmTableLayoutWidget) w;

	/* free any local data */
	if(tw->tablelayout.cols.inherit == 0) {
		if(tw->tablelayout.cols.width) XtFree((char *)tw->tablelayout.cols.width);
		if(tw->tablelayout.cols.disp) XtFree((char *)tw->tablelayout.cols.disp);
	}
	if(tw->tablelayout.rows.inherit == 0) {
		if(tw->tablelayout.rows.width) XtFree((XtPointer)tw->tablelayout.rows.width);
		if(tw->tablelayout.rows.disp) XtFree((XtPointer)tw->tablelayout.rows.disp);
	}
}


static void	Resize(tw)
	XmTableLayoutWidget	tw;
{
	/* DoLayout(tw); Layout does not have to change when we resize */
	DrawChildren(tw);
}


/* Handle geometry requests from children */

static XtGeometryResult GeometryManager(child, request, given)
	Widget			child;
	XtWidgetGeometry	*request, *given;
{
	XmTableLayoutWidget	tw;
	/* There is a precedence amoung layout levels.  Note assignments */
	enum { NoLayout=0, PartialLayout=1, FullLayout=2 }  mustLayout;
	int	rowW, colW;		/* Current size of each axis */
	int	newHeight, newWidth;	/* New size along each axis */
	int	query;
	XtGeometryResult ret = XtGeometryYes;
	XmTableLayoutConstraintsPtr	tc;
	int	row, col;

	tw = findroot(child);
	if(tw->tablelayout.inDrawChildren) {
		XmeWarning((Widget)tw, MESSAGE6);
	}
	if(tw->tablelayout.inGeometryManager++) return XtGeometryNo;

	tc = (XmTableLayoutConstraintsPtr) child->core.constraints;

	col = tc->tablelayout.col;
	row = tc->tablelayout.row;

	/* Do not allow children to move themselves */
	if((request->request_mode & CWX  && request->x != child->core.x) ||
	   (request->request_mode & CWY  && request->y != child->core.y)) {
		tw->tablelayout.inGeometryManager = 0;
		return XtGeometryNo;
	}
	query = request->request_mode & XtCWQueryOnly;

	mustLayout = NoLayout;
	newHeight = 0;
	newWidth = 0;
	/* Get the existing cell dimensions adjusted for the border of the
	 * child that wants to resize.
	 */
	colW = tw->tablelayout.cols.width[tc->tablelayout.col] - 2*child->core.border_width;
	rowW = tw->tablelayout.rows.width[tc->tablelayout.row] - 2*child->core.border_width;

	/* Now we compare the requested size to the current column/row
	 * size and see if the column or row would grow or potentially
	 * shrink.  (Same logic as in ConstraintSetValues)  If a change
	 * could happen we signal the layout, else we deny the request
	 * because we will only end up stretching the widget back to its
	 * current size.
	 */

	if(request->request_mode & CWHeight && request->height != tc->tablelayout.h) {
		if(!(tc->tablelayout.ignore & XmTableLayoutIgnoreHeight) &&
		   ((request->height > rowW) ||
		    (request->height < rowW && rowW == tc->tablelayout.h))) {
			if(!query) {
				child->core.height = request->height;
				if(request->height > rowW) {	/* widen */
					tw->tablelayout.scrollDir = 1;
					newHeight = request->height +
						2 * child->core.border_width;
					if(mustLayout < FullLayout)
						mustLayout = PartialLayout;
				} else {			/* shrink */
					mustLayout = FullLayout;
				}
			}
		} else {
			/* Do the change, but no relayout */
			if(!query) {
				child->core.height = request->height;
				/* Redraw me so that gravity works */
				if(tw->tablelayout.doLayout) DrawAChild(tw, child);
			}
		}
		tc->tablelayout.h = request->height;		/* keep cache */
	}

	if(request->request_mode & CWWidth && request->width != tc->tablelayout.w) {
		if(!(tc->tablelayout.ignore & XmTableLayoutIgnoreWidth) &&
		   ((request->width > colW) ||
		    (request->width < colW && colW == tc->tablelayout.w))) {
			if(!query) {
				child->core.width = request->width;
				if(request->width > colW) {	/* widen */
					tw->tablelayout.scrollDir = 1;
					newWidth = request->width +
						2 * child->core.border_width;
					if(mustLayout < FullLayout)
						mustLayout = PartialLayout;
				} else {			/* shrink */
					mustLayout = FullLayout;
				}
			}
		} else {
			/* Do the change, but no relayout */
			if(!query) {
				child->core.width = request->width;
				/* Redraw me so that gravity works */
				if(tw->tablelayout.doLayout) DrawAChild(tw, child);
			}
		}
		tc->tablelayout.w = request->width;	/* keep cache */
	}

	if(mustLayout != NoLayout) {
	    if(tw->tablelayout.doLayout) {
		int	errorsInLayout = 0;

		if(mustLayout == FullLayout) {
			errorsInLayout = DoLayout(tw);
		} else {
			/* Widen the column or row if we have to */
			if(tw->tablelayout.rows.width[row] < newHeight) {
				tw->tablelayout.rows.width[row] = newHeight;
			}
			if(tw->tablelayout.cols.width[col] < newWidth) {
				tw->tablelayout.cols.width[col] = newWidth;
			}
			ComputeRowColPositions(tw);
		}
		if(!errorsInLayout) {
			if(RequestSize(tw) == XtGeometryNo) {
				tw->tablelayout.mustDisplay = 1;
			}
			DrawChildren(tw);
		}
	    } else {
		tw->tablelayout.mustDisplay = 1;
	    }
	}

	tw->tablelayout.inGeometryManager = 0;	/* recursion interlock */

	return ret;
}

/* We have our own insert child routine that grows in bigger chunks
 * than the one provided from MIT.
 */

static void InsertChild(w)
	Widget	w;
{
	register Cardinal	position;
	register Cardinal	i;
	register CompositeWidget cw;
	register WidgetList	children;
	int		grow;
	XmTableLayoutWidget	tw;

	cw = (CompositeWidget) w->core.parent;
	tw = (XmTableLayoutWidget) cw;
	children = cw->composite.children;

    if (cw->composite.insert_position != NULL)
        position = (*(cw->composite.insert_position))(w);
    else
        position = cw->composite.num_children;

    if (cw->composite.num_children == cw->composite.num_slots) {
        /* Allocate more space
	 * Used to be += (num_slots / 2) + 2
	 */
	grow = cw->composite.num_slots;
	if(grow <= 0) grow = 100;
	if(grow > 1000) grow = 1000;
        cw->composite.num_slots +=  grow;
        cw->composite.children = children =
            (WidgetList) XtRealloc((XtPointer) children,
            (unsigned) (cw->composite.num_slots) * sizeof(Widget));
    }
    /* Ripple children up one space from "position" */
    for (i = cw->composite.num_children; i > position; i--) {
        children[i] = children[i-1];
    }
    children[position] = w;
    cw->composite.num_children++;
}



static void ChangeManaged(tw)
	XmTableLayoutWidget	tw;
{
	if(tw->core.being_destroyed) return;

	if(tw->tablelayout.inInitialize) return;

	/* Make sure we display at next sync point */
	tw->tablelayout.mustDisplay = 1;

	if(tw->tablelayout.doLayout == 0 || !XtIsRealized((Widget)tw)) {
		/* The user has turned off layout and redisplay because a lot
		 * of things are being added.  Just quit.
		 */
		return;
	}

	DoLayout(tw);
	RequestSize(tw);
	DrawChildren(tw);

	_XmNavigChangeManaged((Widget)tw);	/* Magic Motif code */
}



/* Handle Expose Events for the widget */

static void Exposure(w, event)
	Widget	w;
	XEvent	*event;
{
	XmTableLayoutWidget	tw = (XmTableLayoutWidget) w;

	if(tw->core.being_destroyed) return;
	if(tw->tablelayout.mustLayout) {
		DoLayout(tw);
		RequestSize(tw);
	}

	if(tw->tablelayout.mustDisplay) {
		DrawChildren(tw);
	}
}



static Boolean SetValues(current, request, new)
	XmTableLayoutWidget	current, request, new;
{
	Arg	args[5];
	int	ac = 0;
	/* deal with read only things */

	if(new->tablelayout.cols.firstScroll != current->tablelayout.cols.firstScroll) {
		if(new->tablelayout.cols.sb) {
			XtSetArg(args[ac], XmNminimum,
				new->tablelayout.cols.firstScroll); ac++;
			XtSetValues(new->tablelayout.cols.sb, args, ac);
		}
	}
	if(new->tablelayout.rows.firstScroll != current->tablelayout.rows.firstScroll) {
		if(new->tablelayout.rows.sb) {
			XtSetArg(args[ac], XmNminimum,
				new->tablelayout.cols.firstScroll); ac++;
			XtSetValues(new->tablelayout.rows.sb, args, ac);
		}
	}

	return TRUE;
}

static Boolean ConstraintSetValues(current, request, new)
	Widget		current, request, new;
{
	XmTableLayoutConstraintsPtr tc = (XmTableLayoutConstraintsPtr) new->core.constraints;
	XmTableLayoutWidget	tw = findroot(current);
	int	mustLayout, curAxisWidth;
	XtGeometryResult	parentsWhim;

	if(tw->tablelayout.inInitialize) return 0;

	/* We do not need any special processing for these */
	if(new == tw->tablelayout.cols.sb) return 0;
	if(new == tw->tablelayout.rows.sb) return 0;

	mustLayout = 0;
	/* If they move location we might have to grow the axes */
	if(tw->tablelayout.rows.n < tc->tablelayout.row) {
		ResizeAxis((Widget)tw, &tw->tablelayout.rows, tc->tablelayout.row);
		mustLayout |= 1;
	}
	if(tw->tablelayout.cols.n < tc->tablelayout.col) {
		ResizeAxis((Widget)tw, &tw->tablelayout.cols, tc->tablelayout.col);
		mustLayout |= 2;
	}
	/* Invariant:  tw->tablelayout.cols.width[tc->tablelayout.col] is a valid
	 * memory access at this point.  It contains either the correct
	 * old column width or a zero.
	 */

	if(current->core.width != new->core.width) {
		tc->tablelayout.w = new->core.width;
		/* Signal relayout if:
		 * 1) We are now larger than the old column width. Obvious.
		 * 2) We used to match the current column width.  In that
		 *	case we are now smaller and thus we may have to
		 *	shrink the column.
		 */
		curAxisWidth = tw->tablelayout.cols.width[tc->tablelayout.col];
		if(new->core.width > curAxisWidth ||
		   current->core.width == curAxisWidth) {
			mustLayout |= 4;
		}
	}
	if(current->core.height != new->core.height) {
		tc->tablelayout.h = new->core.height;
		/* As above */
		curAxisWidth = tw->tablelayout.rows.width[tc->tablelayout.row];
		if(new->core.height > curAxisWidth ||
		   current->core.height == curAxisWidth) {
			mustLayout |= 8;
		}
	}

	if(mustLayout) {
		DoLayout(tw);
		parentsWhim = RequestSize(tw);
			tw->tablelayout.mustDisplay = 1;
			DrawChildren(tw);
	}

	return FALSE;
}


/* figure out preferred geometry for the tablelayout */

static XtGeometryResult QueryGeometry(tw, request, answer)
	XmTableLayoutWidget		tw;
	XtWidgetGeometry	*request, *answer;
{
	XtGeometryResult ret = XtGeometryYes;
	Dimension	w, h;

	if(tw->core.being_destroyed) return XtGeometryNo;
	*answer = *request;

	if(tw->tablelayout.cols.width == NULL) {
		DoLayout(tw);
	}

	if(request->request_mode & CWWidth) {
		w = tw->tablelayout.cols.totalWidth;
		if(request->width < w) {
			answer->width = w;
			ret = XtGeometryAlmost;
		}
	}
	if(request->request_mode & CWHeight) {
		h = tw->tablelayout.rows.totalWidth;
		if(request->height < h) {
			answer->height = h;
			ret = XtGeometryAlmost;
		}
	}

	return ret;
}




/* The class definition stuff */

XmTableLayoutClassRec	xmTableLayoutClassRec = {
	{	/* core part */
/* pointer to superclass	*/	(WidgetClass) &xmManagerClassRec,
/* widget resource class name	*/	"XmTableLayout",
/* size of widget record	*/	sizeof(XmTableLayoutRec),
/* class initialization proc	*/	(XtProc)ClassInitialize,
/* class partinitialization	*/	NULL,
/* has class been initialized?	*/	FALSE,
/* initialize subclass fields	*/	Initialize,
/* notify that initialize called  */	NULL,
/* XCreateWindow for widget	*/	Realize,
/* actions			*/	actionsList,
/* number of entries in actions	*/	XtNumber(actionsList),
/* resources for subclass fields    */	resources,
/* number of entries in resources   */	XtNumber(resources),
/* resource class quarkified	*/	NULLQUARK,
/* compress MotionNotify for widget */	TRUE,
/* compress Expose events for widget*/	TRUE,
/* compress enter and leave events  */	TRUE,
/* select for VisibilityNotify	*/	FALSE,
/* free data for subclass pointers  */	Destroy,
/* geom manager changed widget size */	Resize,
/* rediplay window		*/	Exposure,
/* set subclass resource values	*/	SetValues,
/* notify that set_values called    */	NULL,
/* set_values "Almost"		*/	XtInheritSetValuesAlmost,
/* notify that get_values called    */	NULL,
/* assign input focus to widget	*/	NULL,
/* version of intrinsics used	*/	XtVersion,
/* list of callback offsets	*/	NULL,
/* translation tablelayout	*/	_XmTableLayout_defaultTranslations,
/* return preferred geometry	*/	QueryGeometry,
/* display your accelerator	*/	XtInheritDisplayAccelerator,
/* pointer to extension record	*/	NULL
	},
	{	/* Composite Part */
		GeometryManager,	/* XtInheritGeometryManager, */
		ChangeManaged,		/* XtInheritChangeManaged, */
		InsertChild,		/* XtInheritInsertChild, */
		XtInheritDeleteChild,
		NULL			/* extension */
	},
	{	/* Constraint Part */
		/* resources */		tableConstraintResources,
					XtNumber(tableConstraintResources),
					sizeof(XmTableLayoutConstraintsRec),
		/* initialize */	ConstraintInitialize,
		/* destroy */		NULL,
		/* set_values */	ConstraintSetValues,
		/* extension */		NULL
	},
	{	/* manager_class fields   */
		XtInheritTranslations,		/* translations           */
		NULL, 0, NULL, 0,
		XmInheritParentProcess,		/* parent_process         */
		NULL,				/* extension              */
	},
	{	/* TableLayout Widget part */
		(XtPointer) NULL,    		/* extension	  */
	}
};

WidgetClass xmTableLayoutWidgetClass = (WidgetClass) &xmTableLayoutClassRec;


/* Public utility functions */


/* Suspend or resume layout functions, return current status
 * for typical set and restore from caller.
 */

int XmTableLayoutDoLayout(Widget w, int doit)
{
	XmTableLayoutWidget	tw = (XmTableLayoutWidget)w;
	int	old;

	if (tw->core.being_destroyed)
	        return 1;

/*
	if(!XmIsTableLayout(w)) {
		XtWarning("Attempt to call TableLayoutDoLayout on a non-tablelayout.");
		return 1;
	}
*/

	old = tw->tablelayout.doLayout;

	tw->tablelayout.doLayout = doit;
	if(tw->tablelayout.doLayout) {
		DoLayout(tw);
		RequestSize(tw);
		DrawChildren(tw);
	}
	return old;
}

/* Adjust rows from: fromRow to: toRow */

void XmTableLayoutRowAdjust(Widget w, int fromRow, int toRow, int adjustment, int wrap)
{
	XmTableLayoutWidget	tw = (XmTableLayoutWidget) w;
	int	i, row, col, span;
	Widget	child;
	XmTableLayoutConstraintsPtr	tc;
	char	mesg[1000];

/*
	if(!XmIsTableLayout(w)) {
		XtWarning("Attempt to call TableLayoutRowAjust on a non-tablelayout.");
		return;
	}
*/
	
	if (fromRow > toRow) {
	    int tmp = toRow;
	    toRow = fromRow;
	    fromRow = tmp;
	}
	
	if(fromRow < 1) fromRow = 1;
	if(fromRow > tw->tablelayout.rows.n) fromRow = tw->tablelayout.rows.n;
	if(toRow <= 0 || toRow > tw->tablelayout.rows.n) {
		toRow = tw->tablelayout.rows.n;
	}
	if(!wrap) {
		/* If no wrap mode, must go to the end */
		toRow = tw->tablelayout.rows.n;
	}
	span = toRow - fromRow + 1;

	if(wrap) {
		if(adjustment >= span) {
			snprintf(mesg, sizeof(mesg), MESSAGE7,
			         adjustment, fromRow, toRow);
			XmeWarning(w, mesg);
			return;
		}
	}


	for(i = 0; i < tw->composite.num_children; i++) {
		child = tw->composite.children[i];

		/* Skip the obvious losers */
		if(!XtIsManaged(child)) continue;
		if(child == tw->tablelayout.cols.sb) continue;
		if(child == tw->tablelayout.rows.sb) continue;

		tc = (XmTableLayoutConstraintsPtr) child->core.constraints;
		if(tc == NULL) {
			XtError(MESSAGE5);
			continue;
		}
		row = tc->tablelayout.row;
		col = tc->tablelayout.col;
		if(row <= 0 || row > tw->tablelayout.rows.n) {
			snprintf(mesg, sizeof(mesg), MESSAGE8,
				tc->tablelayout.row, tc->tablelayout.col);
			XmeWarning((Widget)tw, mesg);
			continue;
		}

		if(fromRow <= row && row <= toRow) {
			tc->tablelayout.row += adjustment;
			if(wrap) {
				tc->tablelayout.row =
				   ((tc->tablelayout.row - fromRow + span) % span) + fromRow;
			}
		}
	}

	if(!wrap) {
		tw->tablelayout.rows.n += adjustment;
		ResizeAxis((Widget)tw, &tw->tablelayout.rows, tw->tablelayout.rows.n);
	}
}

/* Handles insert & delete of columns or rows */

static void XmTableLayoutInsertHelper(Widget w, int insertPoint, int howMany, int doRows)
/*
	int		insertPoint;
	int		howMany;	-- > 0 for insert, < 0 for delete
	int		doRows;		-- 0 for columns, else rows
*/
{
	XmTableLayoutWidget	tw = (XmTableLayoutWidget) w;
	int	i, position;
	Widget	child;
	XmTableLayoutConstraintsPtr	tc;
	char	mesg[1000];
	XmTableLayoutAxis	*ax;
	int	deleteTop = 0;		/* if deleting, last column to go */
	int	prevLayout;

/*
	if(!XmIsTableLayout(w)) {
		XtWarning("Attempt to call XmTableLayout utility on a non-tablelayout.");
		return;
	}
*/
	if(howMany == 0) {
		XmeWarning(w, MESSAGE9);
		return;
	}
	if(howMany < 0) deleteTop = insertPoint - howMany - 1;

	if(doRows) {
		ax = &tw->tablelayout.rows;
	} else {
		ax = &tw->tablelayout.cols;
	}

	if(insertPoint < 1) insertPoint = 1;
	if(insertPoint > ax->n) return;

	prevLayout = tw->tablelayout.doLayout;	/* Turn off layout because */
	tw->tablelayout.doLayout = 0;			/* changedManaged is called */
						/* for each destroy */

	for(i = 0; i < tw->composite.num_children; i++) {
		child = tw->composite.children[i];

		/* Skip the obvious losers */
		if(!XtIsManaged(child)) continue;
		if(child == tw->tablelayout.cols.sb) continue;
		if(child == tw->tablelayout.rows.sb) continue;

		tc = (XmTableLayoutConstraintsPtr) child->core.constraints;
		if(tc == NULL) {
			XtError(MESSAGE5);
			continue;
		}
		if(doRows) {
			position = tc->tablelayout.row;
		} else {
			position = tc->tablelayout.col;
		}
		if(position <= 0 || position > ax->n) {
			sprintf(mesg, MESSAGE8,
				tc->tablelayout.row, tc->tablelayout.col);
			XmeWarning(w, mesg);
			continue;
		}

		if(insertPoint <= position) {
			if(howMany < 0 && position <= deleteTop) {
				XtDestroyWidget(child);
			} else {
				if(doRows) {
					tc->tablelayout.row += howMany;
				} else {
					tc->tablelayout.col += howMany;
				}
			}
		}
	}


	if(ax->sb && insertPoint < ax->firstScroll) {
		ax->firstScroll += howMany;
	}
	ax->n += howMany;
	ResizeAxis(w, ax, ax->n);
	tw->tablelayout.doLayout = prevLayout;	/* Turn layout back on */
	if(howMany < 0) ChangeManaged(tw);	/* force recalc in delete mode*/
}

void XmTableLayoutInsertColumns(Widget tw, int insertPoint, int howMany)
{
	if(howMany < 0) return;
	XmTableLayoutInsertHelper(tw, insertPoint, howMany, 0);
}

void XmTableLayoutInsertRows(Widget tw, int insertPoint, int howMany)
{
	if(howMany < 0) return;
	XmTableLayoutInsertHelper(tw, insertPoint, howMany, 1);
}

void XmTableLayoutDeleteColumns(Widget tw, int insertPoint, int howMany)
{
	if(howMany < 0) return;
	XmTableLayoutInsertHelper(tw, insertPoint, -howMany, 0);
}

void XmTableLayoutDeleteRows(Widget tw, int insertPoint, int howMany)
{
	if(howMany < 0) return;
	XmTableLayoutInsertHelper(tw, insertPoint, -howMany, 1);
}

/* Convert XY to row column */
int XmTableLayoutXYToRowColumn(Widget w, int x, int y, int *row, int *column)
{
	XmTableLayoutWidget	tw = (XmTableLayoutWidget) w;
	if(x < 0 || y < 0) return -1;
	if(column) *column = FindPositionAxis(tw, &tw->tablelayout.cols, x);
	if(row) *row = FindPositionAxis(tw, &tw->tablelayout.rows, y);
	return 0;
}

Widget XmTableLayoutRowColumnToWidget(Widget w, int row, int column)
{
	XmTableLayoutWidget	tw = (XmTableLayoutWidget) w;
	int	i;
	Widget	child, retValue;
	XmTableLayoutConstraintsPtr	tc;

	retValue = NULL;
	for(i = 0; i < tw->composite.num_children; i++) {
		child = tw->composite.children[i];

		if(child == tw->tablelayout.cols.sb) continue;
		if(child == tw->tablelayout.rows.sb) continue;

		tc = (XmTableLayoutConstraintsPtr) child->core.constraints;
		if(tc == NULL) {
			XmeWarning(w, MESSAGE5);
			continue;
		}
		if(column == tc->tablelayout.col && row == tc->tablelayout.row) {
			retValue = child;
			break;
		}
	}
	return retValue;
}

/* EOF of TableLayout.C */
