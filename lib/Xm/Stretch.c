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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "XmI.h"
#include <Xm/StretchP.h>
#include <X11/Shell.h>
#include <Xm/Label.h>


#define NOT_USED	NULL

static String cursor_names[] = {
    NOT_USED,
    "top_side",
    "bottom_side",
    NOT_USED,
    "left_side",
    "top_left_corner",
    "bottom_left_corner",
    NOT_USED,
    "right_side",
    "top_right_corner",
    "bottom_right_corner",
};

/*
 *	5	1	9
 *	4		8
 *	6	2	10
 *
 * Where the corners are equal width and the top/bottom and 
 * left/right are sized by the child.
 */

#define INSIDE          0
#define TOP		(1 << 0)
#define BOTTOM		(1 << 1)
#define LEFT	        (1 << 2)
#define RIGHT	        (1 << 3)
#define TOP_LEFT	(TOP | LEFT)
#define BOTTOM_LEFT	(BOTTOM | LEFT)
#define TOP_RIGHT	(TOP | RIGHT) 
#define BOTTOM_RIGHT	(BOTTOM | RIGHT)

#ifndef ABS
#define ABS(a)          (((a) >= 0) ? (a) : -(a))
#endif

#define IsCorner(i) \
(IsTopRight(i) || IsTopLeft(i) || IsBottomRight(i) || IsBottomLeft(i))

#define IsAlongTop(i)		((i) & TOP)
#define IsAlongBottom(i)	((i) & BOTTOM)
#define IsAlongLeft(i)		((i) & LEFT)
#define IsAlongRight(i)		((i) & RIGHT)
#define IsTop(i)		(TOP == (i))
#define IsBottom(i)		(BOTTOM == (i))
#define IsLeft(i)		(LEFT == (i))
#define IsRight(i)		(RIGHT == (i))

#define IsTopLeft(i)		(TOP_LEFT == (i))
#define IsTopRight(i)		(TOP_RIGHT == (i))
#define IsBottomLeft(i)		(BOTTOM_LEFT == (i))
#define IsBottomRight(i)	(BOTTOM_RIGHT == (i))

static XtResource resources[] =
{
    {
      XmNshadowThickness, XmCShadowThickness, XmRShort, sizeof(short), 
      XtOffsetOf(XmManagerRec, manager.shadow_thickness),
      XmRImmediate, (XtPointer) 5
    },
    {
      XmNresizeCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
      XtOffsetOf(XmStretchRec, stretch.resize_callback),
      XmRImmediate, (XtPointer) NULL
    },
    {
      XmNwidthInc, XmCIncrement, XmRHorizontalDimension, sizeof(Dimension),
      XtOffsetOf(XmStretchRec, stretch.width_inc),
      XmRImmediate, (XtPointer) 1
    },
    {
      XmNheightInc, XmCIncrement, XmRVerticalDimension, sizeof(Dimension),
      XtOffsetOf(XmStretchRec, stretch.height_inc),
      XmRImmediate, (XtPointer) 1
    },
    {
      XmNmaxWidth, XmCMaxSize, XmRInt, sizeof(int),
      XtOffsetOf(XmStretchRec, stretch.max_width),
      XmRImmediate, (XtPointer) 32000
    },
    {
      XmNmaxHeight, XmCMaxSize, XmRInt, sizeof(int),
      XtOffsetOf(XmStretchRec, stretch.max_height),
      XmRImmediate, (XtPointer) 32000
    },
    {
      XmNminWidth, XmCMinSize, XmRInt, sizeof(int),
      XtOffsetOf(XmStretchRec, stretch.min_width),
      XmRImmediate, (XtPointer) 1
    },
    {
      XmNminHeight, XmCMinSize, XmRInt, sizeof(int),
      XtOffsetOf(XmStretchRec, stretch.min_height),
      XmRImmediate, (XtPointer) 1
    }
};

static XmSyntheticResource get_resources[] =
{
    {
      XmNwidthInc, sizeof(Dimension),
      XtOffsetOf(XmStretchRec, stretch.width_inc),
      XmeFromHorizontalPixels, (XmImportProc) XmeToHorizontalPixels
    },
    {
      XmNheightInc, sizeof(Dimension),
      XtOffsetOf(XmStretchRec, stretch.height_inc),
      XmeFromVerticalPixels, (XmImportProc) XmeToVerticalPixels
    }
};

/* Methods */

static void ClassInitialize();
static void ChangeManaged(Widget), InsertChild(Widget);
static void Initialize(Widget, Widget, ArgList, Cardinal *);
static void Resize (Widget), Destroy(Widget);
static void Realize(Widget, Mask *, XSetWindowAttributes *);

static Boolean SetValues (Widget, Widget, Widget, ArgList, Cardinal *);

static XtGeometryResult GeometryManager(Widget, XtWidgetGeometry *, 
					XtWidgetGeometry *);

static XtGeometryResult QueryGeometry(Widget, XtWidgetGeometry *, 
				      XtWidgetGeometry *);

/* actions */

static void Start(Widget, XEvent *, String *, Cardinal *);
static void Movement(Widget, XEvent *, String *, Cardinal *);
static void End(Widget, XEvent *, String *, Cardinal *);

/* private */

static Widget FindChild (XmStretchWidget);
static int GetLocation(XmStretchWidget, XEvent *, 
		       Dimension, Dimension, Position *, Position *);
static void ProcessLocation(XmStretchWidget, int, 
			    Dimension, Dimension, Position, Position);

static void BuildLabel(XmStretchWidget), PopupLabel(XmStretchWidget, Boolean);
static void PrintSize(XmStretchWidget, int, int);
static void SetMotifLabel(Widget, String), Swap(Position *, Position *);
static void RemakeWindowCursors(XmStretchWidget);
static void CheckBounds(XmStretchWidget, Dimension *, Dimension *);
static void RefigureChildren(XmStretchWidget);
static void RationalizeXandY(XmStretchWidget);

static int RubberBand (XmStretchWidget, int, int, int, int);

static Cursor GetCursor(Widget, String);

/*
 * DANGER!!!
 *
 * XmStart() does a server Grab that is removed in XmEnd(), do not call 
 * XmStart() without calling XmEnd() or you will lose horribly.
 */

static char translations[] =
    "<Btn1Down>:	XmStart()\n\
     <Btn1Up>:		XmEnd()\n\
     <Btn1Motion>:	XmMovement()";

static XtActionsRec actions[] =
{
    {"XmStart", 	Start},
    {"XmEnd", 	End},
    {"XmMovement", Movement},
};

XmStretchClassRec xmStretchClassRec = {
  { /* core fields */
#define SUPERCLASS	(&xmFrameClassRec)
    /* superclass		*/	(WidgetClass) SUPERCLASS,
    /* class_name		*/	"XmStretch",
    /* widget_size		*/	sizeof(XmStretchRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	actions,
    /* num_actions		*/	XtNumber(actions),
    /* resources		*/	(XtResource*)resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	Destroy,
    /* resize			*/	Resize,
    /* expose			*/	XtInheritExpose,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersionDontCheck,
    /* callback_private		*/	NULL,
    /* tm_table			*/	translations,
    /* query_geometry		*/	(XtGeometryHandler) QueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  { /* composite fields */
    /* geometry_manager   */    	GeometryManager,
    /* change_managed     */    	ChangeManaged,
    /* insert_child       */    	InsertChild,
    /* delete_child       */    	XtInheritDeleteChild,
    /* extension          */    	NULL
  },
   {            /* constraint_class fields */
    /* resource list      */		NULL,
    /* num resources      */		0,
    /* constraint size    */            sizeof(XmStretchConstraintsRec),
    /* init proc          */ 	    	NULL,                           
    /* destroy proc       */      	NULL,                         
    /* set values proc    */      	NULL,                           
    /* extension          */      	NULL,                         
   },
   {             /* manager class     */
    /* default translations   */      XtInheritTranslations,	
    /* syn_resources          */      get_resources,
    /* num_syn_resources      */      XtNumber(get_resources),
    /* syn_cont_resources     */      NULL,
    /* num_syn_cont_resources */      0,
    /* parent_process         */      XmInheritParentProcess,
    /* extension	      */      NULL,	
   },
  { /* frame fields */
    /* extension         */     	NULL,                 
  },
  { /* stretch fields */
    /* extension         */     	NULL,                 
  }
};

WidgetClass xmStretchWidgetClass = (WidgetClass) &xmStretchClassRec;
		
/****************************************************************
 *
 * Widget Methods
 *
 ****************************************************************/

/*	Function Name: ClassInitialize
 *	Description:   Initializes this widget class.
 *	Arguments:     none
 *	Returns:       nothing
 */
static void
ClassInitialize()
{

    XmStretchClassRec* wc = &xmStretchClassRec;
}


/*	Function Name: Initialize
 *	Description:   Initializes this widget.
 *	Arguments:     req - what was originally requested.
 *                     set - what will be created (our superclassed have
 *                           already mucked with this)
 *                     args, num_args - The arguments passed to 
 *                                      the creation call.
 *	Returns:       none.
 *
 * NOTE: frame makes sure we're not 0x0
 */

/*ARGSUSED*/
static void
Initialize(Widget req, Widget set, ArgList args, Cardinal *num_args)
{
    XmStretchWidget sw = (XmStretchWidget) set;
    XGCValues gcVal;
    XtGCMask mask;
    int i;
    
#ifdef DEMO
    _XmInitialIzeConverters(req);
#endif

    XmStretch_sides_broken(sw) = INSIDE;
    
    gcVal.function = GXxor;
    gcVal.foreground = (sw->core.background_pixel ^ sw->manager.foreground);
    gcVal.subwindow_mode = IncludeInferiors;
    
    mask = GCFunction | GCForeground | GCSubwindowMode;
    XmStretch_rubber_GC(sw) = XtGetGC(set, mask, &gcVal);
    
    for (i = 0; i < N_CURSORS; i++) {
	*(XmStretch_cursors(sw) + i) = None;
	*(XmStretch_windows(sw) + i) = None;
    }
    
    BuildLabel(sw);

    if( XtWidth(req) == 0 )
    {
	set->core.width = 50;
    }
    if( XtHeight(req) == 0 )
    {
	set->core.height = 50;
    }
}

/*	Function Name: Resize
 *	Description:   Called when this widget has been resized.
 *	Arguments:     w - the widget to resize. 
 *	Returns:       none.
 */

static void 
Resize(Widget w)
{
    XmStretchWidget sw = (XmStretchWidget) w;
    XtWidgetProc resize;

    /* first call superclass's routine */    
    _XmProcessLock();
    resize = *SUPERCLASS->core_class.resize;
    _XmProcessUnlock();
    (* resize) (w);

    RemakeWindowCursors(sw);
}

/*	Function Name: Realize
 *	Description:   Called to realize this widget.
 *	Arguments:     w - the widget to realize.
 *                     valueMask, attributes - attributes to use when creating
 *                     this widget's window.
 *	Returns:       none.
 */

static void 
Realize(Widget w, Mask *valuemask, XSetWindowAttributes *attributes)
{
    XmStretchWidget sw = (XmStretchWidget) w;

    /*
     * Get rid of XmManager's frobbing with the BitGravity Field.
     */

    XtCreateWindow (w, InputOutput, CopyFromParent, *valuemask, attributes);

    RemakeWindowCursors(sw);
}

/*	Function Name: Destroy
 *	Description:   Frees all data created by this widget.
 *	Arguments:     w - the widget that is being destroyed.
 *	Returns:       none.
 */

static void 
Destroy(Widget w)
{
    XmStretchWidget sw = (XmStretchWidget) w;

    XtReleaseGC(w, XmStretch_rubber_GC(sw));
}

/*	Function Name: SetValues
 *	Description:   Called when some widget data needs to be modified on-
 *                     the-fly.
 *	Arguments:     current - the current (old) widget values.
 *                     req - before superclassed have changed things.
 *                     set - what will acutally be the new values. 
 *                     args_in, num_args - the arguments in the list.
 *	Returns:       none
 */

/*ARGSUSED*/
static Boolean
SetValues (Widget current, Widget req, Widget set, 
	   ArgList args_in, Cardinal * num_args)
{
    XmStretchWidget sw = (XmStretchWidget) set;
    XmStretchWidget osw = (XmStretchWidget) current;
    Boolean resize = False;

    if (sw->manager.shadow_thickness != osw->manager.shadow_thickness) {
	resize = True;
    }

    if ((XmStretch_width_inc(sw) != XmStretch_width_inc(osw)) ||
	(XmStretch_height_inc(sw) != XmStretch_height_inc(osw)) ||
	(XmStretch_max_width(sw) != XmStretch_max_width(osw)) ||
	(XmStretch_max_height(sw) != XmStretch_max_height(osw)) ||
	(XmStretch_min_width(sw) != XmStretch_min_width(osw)) ||
	(XmStretch_min_height(sw) != XmStretch_min_height(osw)))
    {
	Dimension width, height;
	Widget child = FindChild(sw);
	
	if (child != NULL) {
	    width = child->core.width;
	    height = child->core.height;
	    CheckBounds(sw, &width, &height);
	 
	    if ((width != child->core.width) || (height != child->core.height))
		resize = True;

	    /*
	     * Our new preferred size.
	     */

	    sw->core.width = width + 2 * sw->manager.shadow_thickness;
	    sw->core.height = height + 2 * sw->manager.shadow_thickness;
	}
    }
    
    if (resize) 
	RemakeWindowCursors(sw);

    return (False);
} /* SetValues */

/*      Function Name: QueryGeometry
 *      Description:   Called when my parent wants to know my preferred size.
 *      Arguments:     w - the widget to check.
 *                     intended - parent imposed geometry.
 *                     preferred - what I would like.
 *      Returns:       status.
 */

static XtGeometryResult
QueryGeometry(Widget w, XtWidgetGeometry *request,
              XtWidgetGeometry *preferred)
{
    XmStretchWidget sw = (XmStretchWidget) w;
    XtWidgetGeometry c_reply;
    Dimension shadow_thickness;
    Widget child = FindChild(sw);

    shadow_thickness = 2 * sw->manager.shadow_thickness;
    
    if (child == NULL) {
	c_reply.height = c_reply.width = 1;
    }
    else {
	/*
	 * If there are constraints placed on us by our parent
	 * Subtract off the space needed for our shadows.
	 */
	
	if (request->request_mode & CWWidth) {
	    if (request->width > shadow_thickness)
		request->width -= shadow_thickness;
	}
	
	if (request->request_mode & CWHeight) {
	    if (request->height > shadow_thickness)
		request->height -= shadow_thickness;
	}
	
	/*
	 * Ask child, reminding it that we will not allow borders.
	 */
	
	request->request_mode |= CWBorderWidth;
	request->border_width = 0;
	
	(void) XtQueryGeometry(child, request, &c_reply);
    }

    /*
     * Use child's reply to formulate our desired geometry, just take the
     * child's size and add space for our shadows.
     */

    if (c_reply.width > shadow_thickness)
	preferred->width = c_reply.width + shadow_thickness;
    else
	preferred->width = 1;

    if (c_reply.height > shadow_thickness)
	preferred->height = c_reply.height + shadow_thickness;
    else
	preferred->height = 1;

    return(_XmHWQuery(w, request, preferred));
}


/*      Function Name: InsertChild
 *      Description:   when a child widget is added.
 *      Arguments:     w - the child widget
 *      Returns:       none.
 */

/* ARGSUSED */
static void
InsertChild(register Widget w)
{
    XtWidgetProc insert_child;
    /* Check for non-widgets - ButtonBox doesn't support gadgets. */

    if (_XmGadgetWarning(w))
    {
	return;
    }

    _XmProcessLock();
    insert_child = *SUPERCLASS->composite_class.insert_child;
    _XmProcessUnlock();
    (*insert_child)(w);
}

/*	Function Name: ChangeManaged
 *	Description:   Called whenever our children change managed state.
 *	Arguments:     w - the widget whos children are changing..
 *	Returns:       none.
 */

static void 
ChangeManaged(Widget w)
{
    XmStretchWidget sw = (XmStretchWidget) w;
    Dimension width, height;
    Widget child;
    XtWidgetProc ch_managed;

#if 0		/* works, but then other parts of XmFrame interfere if there is more than one child */
    {
	    /* hack */
	    register Widget *childP;
	    Boolean foundManaged = False;
	    ForAllChildren(sw, childP)
		{
		if (foundManaged) XtUnmanageChild(*childP);
		else
		{
		if (XtIsManaged(*childP)) foundManaged = True;
		}
		}
    }
#endif

    child = FindChild(sw);

    if (child != NULL) {
	width = child->core.width;
	height = child->core.height;
	CheckBounds(sw, &width, &height);
	
	_XmResizeWidget(child, width, height, 0);
    }

    _XmProcessLock();
    ch_managed = SUPERCLASS->composite_class.change_managed;
    _XmProcessUnlock();
    (*ch_managed)(w);

    if (child != NULL) {
	RemakeWindowCursors(sw);
	RefigureChildren(sw);
    }
}

static XtGeometryResult 
GeometryManager(Widget w, XtWidgetGeometry * request, XtWidgetGeometry * reply)
{
    XmStretchWidget sw = (XmStretchWidget) XtParent(w);
    XtWidgetGeometry lreq;
    Boolean rwidth, rheight;
    XtGeometryHandler geo_manager;

    rwidth = request->request_mode & CWWidth;
    rheight = request->request_mode & CWHeight;

    lreq.request_mode = 0;

    if (rwidth) {
	lreq.width = request->width;
	lreq.request_mode |= CWWidth;
    }
    else
	lreq.width = w->core.width;

    if (rheight) {
	lreq.height = request->height;
	lreq.request_mode |= CWHeight;
    }
    else
	lreq.height = w->core.height;

    if (lreq.request_mode == 0)
	return(XtGeometryNo);	/* No interesting changes. */

    CheckBounds(sw, &(lreq.width), &(lreq.height));

    /* first call superclass's routine */    
    _XmProcessLock();
    geo_manager = *SUPERCLASS->composite_class.geometry_manager;
    _XmProcessUnlock();

    switch((*geo_manager)(w, &lreq, reply)) {
    case XtGeometryNo:
	return(XtGeometryNo);
    case XtGeometryAlmost:
	lreq.width = reply->width;
	lreq.height = reply->height; 
	/*
	 * Will fall through to the XtGeometryAlmost code below.
	 */
	break;
    default:		/* XtGeometryYes */
	if ((!rwidth || (lreq.width == request->width)) &&
	   (!rheight || (lreq.height == request->height)))
	{
	    if (!(request->request_mode & XtCWQueryOnly))
		RemakeWindowCursors(sw);

	    return(XtGeometryYes);
	}
	/*
	 * CheckBounds changed the width, we need to reply almost.
	 * Will fall through to the XtGeometryAlmost code below.
	 */
	break;
    }

    /*
     * Prepare our "Almost" reply.
     */

    reply->request_mode = 0;

    if (rwidth) {
	reply->width = lreq.width;
	reply->request_mode |= CWWidth;
    }

    if (rheight) {
	reply->height = lreq.height;
	reply->request_mode |= CWHeight;
    }

    return(XtGeometryAlmost);
}

/************************************************************
 *
 *  Action Procedures.
 *
 ************************************************************/

/*	Function Name: Start
 *	Description:   Called to start the resize operation.
 *	Arguments:     w - the stretch widget.
 *                     event - the event that caused this action.
 *                     params, num_params - the parameter list.
 *	Returns:       none.
 */

/*ARGSUSED*/
static void 
Start(Widget w, XEvent *event, String * params, Cardinal * num_params)
{
    XmStretchWidget sw = (XmStretchWidget)w;
    Widget child = FindChild(sw);
    Dimension c_width, c_height;
    Position x, y;
    int location;

    if (child == NULL)
	return;

    c_width = child->core.width;
    c_height = child->core.height;

    XmStretch_start_realx(sw) = sw->manager.shadow_thickness;
    XmStretch_start_realy(sw) = sw->manager.shadow_thickness;
    XmStretch_last_realx(sw) = XmStretch_start_realx(sw) + c_width;
    XmStretch_last_realy(sw) = XmStretch_start_realy(sw) + c_height;
    
    XmStretch_old_width(sw) = c_width;
    XmStretch_old_height(sw) = c_height;

    PrintSize(sw, c_width, c_height);

    PopupLabel(sw, True);	/* Pop up the label. */

    /* initial box is size of child */

    /*
     * Since we are drawing on the root window with includeInferiors
     * we must grab the server, or we can mess up someone elses window.
     */

#ifndef DEBUG
    XGrabServer(XtDisplay(w));
#endif

    RubberBand(sw, XmStretch_start_realx(sw), XmStretch_start_realy(sw),
	       XmStretch_last_realx(sw), XmStretch_last_realy(sw));

    location = GetLocation(sw, event, c_width, c_height, &x, &y);

    if (location > 0) {
	XChangeActivePointerGrab(XtDisplay(w), 
				 (ButtonPressMask | ButtonReleaseMask |
				  ButtonMotionMask),
				 XmStretch_cursors(sw)[location],
				 XtLastTimestampProcessed(XtDisplay(w)));
    }
}

/*	Function Name: Movement
 *	Description:   Called when the cursor moves during a resize operation.
 *	Arguments:     w - the stretch widget.
 *                     event - the event that caused this action.
 *                     params, num_params - the parameter list.
 *	Returns:       none.
 */

/*ARGSUSED*/
static void 
Movement(Widget w, XEvent *event, String * params, Cardinal * num_params)
{
    XmStretchWidget sw = (XmStretchWidget)w;
    Widget child = FindChild(sw);
    int location;
    Position x, y;
    Dimension c_width, c_height;
    unsigned char old_broken;
    
    if (child == NULL)
	return;

    c_width = child->core.width;
    c_height = child->core.height;

    /*
     * Erase old box.
     */
    
    RubberBand(sw, XmStretch_start_realx(sw), XmStretch_start_realy(sw),
                XmStretch_last_realx(sw), XmStretch_last_realy(sw));

    location = GetLocation(sw, event, c_width, c_height, &x, &y);

    /*
     * We now have location in set 1,2,4,8 5,6,9,10. 0 is inside.
     * set drawn box depending on it and constraints, if implemented 
     */
    
    if ((location == INSIDE) && (XmStretch_sides_broken(sw) == INSIDE)) {
	/*
	 * Never been outside -- just vibrate 
	 */

	RubberBand(sw, XmStretch_start_realx(sw), XmStretch_start_realy(sw),
		   XmStretch_last_realx(sw), XmStretch_last_realy(sw));
	return;
    }

    old_broken = XmStretch_sides_broken(sw);
    ProcessLocation(sw, location, c_width, c_height, x, y);

    if (old_broken != XmStretch_sides_broken(sw)) {
	XChangeActivePointerGrab(XtDisplay(w), 
				 (ButtonPressMask | ButtonReleaseMask |
				  ButtonMotionMask),
				 XmStretch_cursors(sw)[XmStretch_sides_broken(sw)],
				 XtLastTimestampProcessed(XtDisplay(w)));
    }

    /* 
     * We should now have a box with (start_realx, start_realy) in UpperLeft
     * and (last_realx, last_realy) in LowerRight 
     */

    RubberBand(sw, XmStretch_start_realx(sw), XmStretch_start_realy(sw),
	       XmStretch_last_realx(sw), XmStretch_last_realy(sw));
}

/*	Function Name: End
 *	Description:   Called to end the resize operation.
 *	Arguments:     w - the stretch widget.
 *                     event - the event that caused this action.
 *                     params, num_params - the parameter list.
 *	Returns:       none.
 */

/*ARGSUSED*/
static void 
End(Widget w, XEvent *event, String * params, Cardinal * num_params)
{
    XmStretchWidget sw = (XmStretchWidget)w;
    Widget child = FindChild(sw);
    XmStretchWidgetInfo info;
    XtGeometryResult result;
    Dimension c_width, c_height;
    Position x, y;
    XtWidgetGeometry request, reply;
    int location;

    if (child == NULL)
	return;

    /*
     * Erase old box.
     */

    RubberBand(sw, XmStretch_start_realx(sw), XmStretch_start_realy(sw),
	       XmStretch_last_realx(sw), XmStretch_last_realy(sw));

#ifndef DEBUG
    XUngrabServer(XtDisplay(w));
#endif

    PopupLabel(sw, False);	/* Pop down the label. */

    c_width = child->core.width;
    c_height = child->core.height;

    location = GetLocation(sw, event, c_width, c_height, &x, &y);
    if ((location == INSIDE) && (XmStretch_sides_broken(sw) == INSIDE)) {
	return;			/* no change... */
    }
    ProcessLocation(sw, location, c_width, c_height, x, y);

    c_width = ABS(XmStretch_last_realx(sw) - XmStretch_start_realx(sw));
    c_height = ABS(XmStretch_last_realy(sw) - XmStretch_start_realy(sw));

    request.width = c_width + 2 * sw->manager.shadow_thickness;
    request.height = c_height + 2 * sw->manager.shadow_thickness;
    request.request_mode = CWWidth | CWHeight;

    result = XtMakeGeometryRequest(w, &request, &reply);

    info.x = sw->core.x + XmStretch_start_realx(sw);
    info.y = sw->core.y + XmStretch_start_realy(sw);
    info.width = c_width / XmStretch_width_inc(sw);
    info.height = c_height / XmStretch_height_inc(sw);

    switch (result) {
    case XtGeometryYes:
	info.success = True;
	RefigureChildren(sw);	/* We can grow or shrink */
	(*(XtClass(w))->core_class.resize)(w);
	break;
    case XtGeometryNo:
    default:
	info.success = False;
	break;
    }

    XtCallCallbackList (w, XmStretch_resize_callback(w), (XtPointer) &info);
    XmStretch_sides_broken(sw) = INSIDE;
}

/************************************************************
 *
 *  Private functions.
 *
 ************************************************************/

/*	Function Name: FindChild
 *	Description: Finds the managed child of this widget.
 *	Arguments: pw - the stretch widget.
 *	Returns: the child, or NULL if there is no managed child.
 */

static Widget
FindChild (XmStretchWidget sw)
{
    register Widget *childP;

    /*
     * Find the managed child on which we should operate.  Ignore multiple
     * managed children.
     */

    ForAllChildren(sw, childP)
	if (XtIsManaged(*childP) && !((*childP)->core.being_destroyed))
	    return *childP;

    return (Widget) NULL;
}

/*	Function Name: GetLocation
 *	Description: Gets the location value from an event.
 *	Arguments: sw - the stretch widget.
 *                 event - the event that caused this action.
 *                 c_width, c_height - the child's width and height.
 *	Returns: the location varable, or -1 on failure.
 */

static int
GetLocation(XmStretchWidget sw, XEvent *event, 
	    Dimension c_width, Dimension c_height, Position *x, Position *y)
{
    Position currentx, currenty, s_thick;
    int location;

    switch(event->xany.type) {
    case ButtonPress:
    case ButtonRelease:
	currentx = event->xbutton.x;
	currenty = event->xbutton.y;
	break;
    case MotionNotify:
	currentx = event->xmotion.x;
	currenty = event->xmotion.y;
	break;
    default:
	*y = *x = 0;
	return(-1);
    }

    /*
     * This algorithm takes advantage of fact that: we know location, 
     * we know where we are relative to the box, we can keep track of 
     * which sides have been passed through, and that the 4 values 
     * describing the box can be set independently from the 5 points 
     * including the box corners and the current cursor location 
     */

    s_thick = sw->manager.shadow_thickness;

    location = (TOP *	 (int) (currenty < s_thick) + 
		BOTTOM * ((int)currenty > (int) (c_height - s_thick)) +
		LEFT *	 (int) (currentx < s_thick) +
		RIGHT *	 ((int)currentx > (int) (c_width - s_thick)) );

    *x = currentx;
    *y = currenty;
    return(location);
}

/*	Function Name: ProcessLocation
 *	Description: Processes the location information and computes
 *                   the new start/last realx/realy values.
 *	Arguments: sw - the stretch widget.
 *                 location - the location value.
 *                 c_width, c_height - the child size.
 *                 x, y - the current x and y location from the event.
 *	Returns: none.
 */

static void
ProcessLocation(XmStretchWidget sw, int location, 
		Dimension c_width, Dimension c_height, Position x, Position y)
{
    if (location < 0)		/* error, punt out */
	return;

    if ( IsAlongTop(location) || IsAlongTop(XmStretch_sides_broken(sw))) {
	XmStretch_start_realy(sw) = y;
	XmStretch_last_realy(sw) = sw->manager.shadow_thickness + c_height;
	if (IsAlongBottom(XmStretch_sides_broken(sw)))
	    XmStretch_sides_broken(sw) &= ~BOTTOM;
	XmStretch_sides_broken(sw) |= TOP;
    }
    
    if ( IsAlongBottom(location) || IsAlongBottom(XmStretch_sides_broken(sw))) {
	XmStretch_last_realy(sw) = y;
	XmStretch_start_realy(sw) = sw->manager.shadow_thickness;
	if (IsAlongTop(XmStretch_sides_broken(sw)))
	    XmStretch_sides_broken(sw) &= ~TOP;
	XmStretch_sides_broken(sw) |= BOTTOM;
	}

    if (IsAlongLeft(location) || IsAlongLeft(XmStretch_sides_broken(sw))) {
	XmStretch_start_realx(sw) = x;
	XmStretch_last_realx(sw) = sw->manager.shadow_thickness + c_width;
	if (IsAlongRight(XmStretch_sides_broken(sw)))
	    XmStretch_sides_broken(sw) &= ~RIGHT;
	XmStretch_sides_broken(sw) |= LEFT;
    }

    if (IsAlongRight(location) || IsAlongRight(XmStretch_sides_broken(sw))) {
	XmStretch_last_realx(sw) = x;
	XmStretch_start_realx(sw) = sw->manager.shadow_thickness;
	if (IsAlongLeft(XmStretch_sides_broken(sw)))
	    XmStretch_sides_broken(sw) &= ~LEFT;
	XmStretch_sides_broken(sw) |= RIGHT;
    }

    RationalizeXandY(sw);

    /*
     * Convert to a multiple of width or height increment.
     * 
     * Isn't integer arithmetic fun ?
     */

    {
	Dimension width, height;

	width = XmStretch_last_realx(sw) - XmStretch_start_realx(sw);
	height = XmStretch_last_realy(sw) - XmStretch_start_realy(sw);

	CheckBounds(sw, &width, &height);

	if (XmStretch_last_realy(sw) == sw->manager.shadow_thickness + c_height)
	    XmStretch_start_realy(sw) = XmStretch_last_realy(sw) - height;
	else
	    XmStretch_last_realy(sw) = XmStretch_start_realy(sw) + height;

	if (XmStretch_last_realx(sw) == sw->manager.shadow_thickness + c_width)
	    XmStretch_start_realx(sw) = XmStretch_last_realx(sw) - width;
	else 
	    XmStretch_last_realx(sw) = XmStretch_start_realx(sw) + width;

	if ((XmStretch_old_width(sw) != width) || 
	    (XmStretch_old_height(sw) != height))
	{
	    XmStretch_old_width(sw) = width;
	    XmStretch_old_height(sw) = height;
  
	    PrintSize(sw, width, height);
	}
    }
}

/*	Function Name: CheckBounds
 *	Description:   Make sure this stays within the maximum allowable
 *                     height and width.
 *	Arguments:     sw - the stretch widget.
 *                     width, height - sizes to bounds check *** IN_OUT ***
 *	Returns:       none.
 */

static void
CheckBounds(XmStretchWidget sw, Dimension *width, Dimension *height)
{
    Dimension c_width, c_height; 	/* height and width in cells. */

    /*
     * Keep the width and height within the max and min bounds.
     */
    
    c_width = (int)((int)(*width) + (int)XmStretch_width_inc(sw) / 2) /
	(int)XmStretch_width_inc(sw);
    c_height = (int)((int)(*height) + (int)XmStretch_height_inc(sw) / 2)/
	(int)XmStretch_height_inc(sw);

    if ((int)c_width > (int)XmStretch_max_width(sw))
	c_width = XmStretch_max_width(sw);
    else if ((int)c_width < (int)XmStretch_min_width(sw))
	c_width = XmStretch_min_width(sw);
    
    if ((int)c_height > (int)XmStretch_max_height(sw))
	c_height = XmStretch_max_height(sw);
    else if ((int)c_height < (int)XmStretch_min_height(sw))
	c_height = XmStretch_min_height(sw);

    *width = c_width * XmStretch_width_inc(sw);
    *height = c_height * XmStretch_height_inc(sw);
}

/*	Function Name: BuildLabel
 *	Description:   Builds the size display widget.
 *	Arguments:     sw - the stretch widget.
 *	Returns:       none
 */

static void
BuildLabel(XmStretchWidget sw)
{
    Arg args[10];
    Cardinal num_args;
    Widget shell;
    
    num_args = 0;
    XtSetArg (args[num_args], XmNallowShellResize, True); num_args++;
    XtSetArg (args[num_args], XmNoverrideRedirect, True); num_args++;
    XtSetArg (args[num_args], XmNancestorSensitive, True); num_args++;
    shell = XtCreatePopupShell("labelShell", transientShellWidgetClass,
			       (Widget) sw, args, num_args);
    XmStretch_label_shell(sw) = shell;

    num_args = 0;
    XtSetArg (args[num_args], XmNrecomputeSize, True); num_args++;
    XmStretch_label(sw) = XtCreateManagedWidget("label", xmLabelWidgetClass,
					      shell, args, num_args);
}

/*	Function Name: PopupLabel
 *	Description:   Pops the label up or down.
 *	Arguments:     sw - the stretch widget.
 *                     up - if true the pop it up, otherwise pop it down.
 *	Returns:       none.
 */

static void
PopupLabel(XmStretchWidget sw, Boolean up)
{
    Arg args[10];
    Cardinal num_args;
    Position x, y;
    Widget child, shell = XmStretch_label_shell(sw);

    if ((child = FindChild(sw)) == NULL)
	return;

    if (!up) {
	XtPopdown(shell);
	return;
    }

    XtRealizeWidget(shell);

    XtTranslateCoords(child, 1, 1, &x, &y);

    num_args = 0;
    XtSetArg(args[num_args], XmNx, x); num_args++;
    XtSetArg(args[num_args], XmNy, y); num_args++;
    XtSetValues(shell, args, num_args);

    XtPopup(shell, XtGrabNone);
}

/*	Function Name: PrintSize
 *	Description:   Prints the current size.
 *	Arguments:     sw - the stretch widget.
 *                     width, height - the size in pixels.
 *	Returns:       none.
 */

static void
PrintSize(XmStretchWidget sw, int width, int height)
{
    char buf[BUFSIZ];

    sprintf(buf, "%d x %d", 
	    (int)width / (int)XmStretch_width_inc(sw),
	    (int)height / (int)XmStretch_height_inc(sw));

    SetMotifLabel(XmStretch_label(sw), buf);
}

/*	Function Name: SetMotifLabel
 *	Description:   Sets the label of a motif widget give an char *.
 *	Arguments:     w - the widget who's label to set.
 *                     val - the string to set it to.
 *	Returns:       none
 */

static void
SetMotifLabel(Widget w, String val)
{
    Arg args[1];
    XmString temp = XmStringCreate(val, XmFONTLIST_DEFAULT_TAG);
    
    XtSetArg(args[0], XmNlabelString, temp);
    XtSetValues(w, args, (Cardinal) 1);
    XmStringFree(temp);
}    

/*	Function Name: RefigureChildren
 *	Description:   Figures out what size a child is allowed to become.
 *	Arguments:     sw - the stretch widget.
 *	Returns:       none.
 */

static void
RefigureChildren(XmStretchWidget sw)
{
    Widget child = FindChild(sw);
    Dimension c_width, c_height, s_thick;

    if (child == NULL)
	return;

    s_thick = sw->manager.shadow_thickness;
    c_width = sw->core.width - 2 * s_thick;
    c_height = sw->core.height - 2 * s_thick;
    
    CheckBounds(sw, &c_width, &c_height);

    _XmResizeWidget(child, c_width, c_height, (Dimension) 0);

    RemakeWindowCursors(sw);
}

/*	Function Name: Swap
 *	Description:   swaps two integers.
 *	Arguments:     a, b - pointers to a and b. ** MODIFIED IN PLACE **.
 *	Returns:       none.
 */

static void
Swap(Position * a, Position *b)
{
   Position temp = *a;      
   *a = *b;             
   *b = temp;          
}

/*	Function Name: RationalizeXandY
 *	Description:   Makes sure that start_x is less than last_x
 *	Arguments:     sw - the stretch widget.
 *	Returns:       none.
 */

static void
RationalizeXandY(XmStretchWidget sw)
{
    if (XmStretch_start_realx(sw) > XmStretch_last_realx(sw)) 
	Swap(&(XmStretch_start_realx(sw)), &(XmStretch_last_realx(sw)));
    
    if (XmStretch_start_realy(sw) > XmStretch_last_realy(sw)) 
	Swap(&(XmStretch_start_realy(sw)), &(XmStretch_last_realy(sw)));
}

/*	Function Name: RubberBand
 *	Description:   Draws a rubber banded area.
 *	Arguments:     sw - the stretch widget.
 *                     x1, y1 - a corner.
 *                     x2, y2 - the opposite corner.
 *	Returns:       none.
 */

/* rectangles only */
static int 
RubberBand (XmStretchWidget sw, int x1, int y1, int x2, int y2)
{
    int retval = (x2 > x1) + 2 * (y2 > y1);
    Position 	rootx1, rooty1, rootx2, rooty2;

    /*
     * Translate to root coordinates.
     */

    XtTranslateCoords((Widget) sw,
		      (Position) x1, (Position) y1, &rootx1, &rooty1);
    XtTranslateCoords((Widget) sw,
		      (Position) x2, (Position) y2, &rootx2, &rooty2);
    
    XDrawRectangle(XtDisplay((Widget) sw), 
		   RootWindowOfScreen(XtScreen((Widget) sw)),
		   XmStretch_rubber_GC(sw), (int) rootx1, (int) rooty1,
		   (unsigned int) (rootx2 - rootx1 - 1),
		   (unsigned int) (rooty2 - rooty1 - 1));

    return(retval);
}

/*	Function Name: RemakeWindowCursors
 *	Description:   Makes windows if it should, destroys others, destroys
 *                     cursors as it goes. Also called by ChangeManaged to
 *                     locate children.
 *	Arguments:     sw - the stretch widget.
 *	Returns:       none
 */

static void 
RemakeWindowCursors(XmStretchWidget sw)
{
    Display *dpy = XtDisplay(sw);
    int i;
    Dimension c_width, c_height;
    Widget child = FindChild(sw);
    
    if (child == NULL || !XtIsRealized((Widget) sw))
	return;
    
    c_width = child->core.width;
    c_height = child->core.height;

    for (i = 0; i < N_CURSORS; i++) {
	unsigned long valuemask = 0;
	XSetWindowAttributes attr;
	int x, y, width=0, height=0;

	/* is this a special unused case? skip if so */
	if (cursor_names[i] == NOT_USED)
	    continue;

	/*
	 * Check if this falls into prohibited catagory.
	 */
	
	if (XmStretch_cursors(sw)[i] == None)
	    XmStretch_cursors(sw)[i] = GetCursor((Widget) sw, 
					       cursor_names[i]);
	
	/*
	 * We calculate where window is based on
	 * which direction we're dealing with 
	 */
	
	if (IsAlongRight(i) || IsAlongLeft(i))
	    width = sw->manager.shadow_thickness;
	
	if (IsAlongTop(i) || IsAlongBottom(i))
	    height = sw->manager.shadow_thickness;
	
	/* corners are set by now; override for sides */
	
	if (IsRight(i) || IsLeft(i))
	    height = c_height;
	
	if (IsTop(i) || IsBottom(i))
	    width = c_width;
	
	/* all width/height are set by now */
	
	if (IsTop(i) || IsBottom(i))
	    x = sw->manager.shadow_thickness;
	else if (IsAlongRight(i))
	    x = sw->core.width - width;
	else
	    x = 0;
	
	if (IsLeft(i) || IsRight(i))
	    y = sw->manager.shadow_thickness;
	else if (IsAlongBottom(i))
	    y = sw->core.height - height;
	else
	    y = 0;
	
	attr.cursor = XmStretch_cursors(sw)[i];
	valuemask |= CWCursor;
	
	if (XmStretch_windows(sw)[i] == None) {
	    XmStretch_windows(sw)[i] =XCreateWindow(dpy, XtWindow(sw),
						  x,y,width,height, 0, 0,
						  InputOnly, CopyFromParent,
						  valuemask, &attr);
	}
	else {
	    XWindowChanges xwc;
	    xwc.x = x; xwc.y = y;
	    xwc.width = width; xwc.height = height;
	    XConfigureWindow (dpy,  XmStretch_windows(sw)[i],
			      CWX | CWY | CWWidth | CWHeight, &xwc);
	}
	
	/*
	 * This map is only necessary if you are running in something
	 * like BX where the children are added after realize time.
	 */
	
	XMapWindow(dpy, XmStretch_windows(sw)[i]);
    }
}

/*	Function Name: GetCursor
 *	Description:   Calls XtConvertAndStore to get a cursor.
 *	Arguments:     w - a widget.
 *                     name - the name of the cursor to get.
 *	Returns:       none.
 */

static Cursor
GetCursor(Widget w, String name)
{
    Cursor cursor;
    XrmValue from, to;

    from.size = strlen(name) + 1; /* Room for '\0'. */
    from.addr = name;
    
    to.size = sizeof(Cursor);
    to.addr = (XtPointer) &cursor;

    if (!XtConvertAndStore(w, XmRString, &from, XmRCursor, &to)) {
	static String params[] = { "XmStretchWidget", "cursor" };
	Cardinal num = 2;

	XtAppWarningMsg(XtWidgetToApplicationContext(w),
			XmNconversionFailure, XmNconversionFailure, 
			XmCWidgetSetError, XmNconversionFailureMsg,
			params, &num);
    }

    return(cursor);
}

/****************************************************************
 *
 * Public Routines.
 *
 ****************************************************************/

/*	Function Name: XmCreateStretch
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The created widget.
 */

Widget
XmCreateStretch(Widget parent, String name,
		ArgList args, Cardinal num_args)
{
    return(XtCreateWidget(name, xmStretchWidgetClass, parent, args, num_args));
}

