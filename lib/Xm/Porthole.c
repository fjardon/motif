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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <Xm/XmP.h>
#include <Xm/DrawP.h>
#include <Xm/PortholeP.h>
#include <Xm/Panner.h>

#include <Xm/XmP.h>
#include <Xm/XmI.h>

#include "ReportsI.h"

#define SIZEONLY (CWWidth | CWHeight)

/*
 * resources for the porthole
 */

static XmPartResource resources[] = {
    {XmNreportCallback, XmCReportCallback, XmRCallback, sizeof(XtPointer),
	XtOffsetOf(XmPortholeRec, porthole.report_callbacks),
	XmRImmediate, (XtPointer) NULL},
    { XmNforceChildToFill, XmCBoolean, XmRBoolean, sizeof(Boolean),
	XtOffsetOf(XmPortholeRec, porthole.force_child_to_fill),
	XmRImmediate, (XtPointer) True },
    {XmNshadowThickness, XmCShadowThickness, XmRHorizontalDimension, sizeof(Dimension),
	XtOffsetOf(XmManagerRec, manager.shadow_thickness),
	XmRImmediate, (XtPointer) 2},
    {XmNmarginWidth, XmCMargin, XmRHorizontalDimension, sizeof(Dimension),
	 XtOffsetOf(XmPortholeRec, porthole.margin_width),
	 XmRImmediate, (XtPointer) 0},
    {XmNmarginHeight, XmCMargin, XmRVerticalDimension, sizeof(Dimension),
	 XtOffsetOf(XmPortholeRec, porthole.margin_height),
	 XmRImmediate, (XtPointer) 0}
};

static unsigned int UsableWidth(Widget), UsableHeight(Widget);
static Widget FindChild(Widget);
static void SendReport(XmPortholeWidget, unsigned int);
static void LayoutChildren(Widget, Widget, XtWidgetGeometry *,
			   Position *, Position *, Dimension *, Dimension *);
static void PlaceChildren(Widget, Boolean,
			  Position, Position, Dimension, Dimension);

/*
 * widget class methods used below
 */
/* set gravity and upcall */
static void
Realize(Widget, Mask *, XSetWindowAttributes *);

/* report new size */
static void
Resize(Widget);

/* set up initial values. */
static void
Initialize(Widget, Widget, ArgList, Cardinal *);

static void
ClassInitialize(void);

/* deal with child requests */
static XtGeometryResult
GeometryManager(Widget, XtWidgetGeometry *, XtWidgetGeometry *);

/* somebody added a new widget */
static void
ChangeManaged(Widget);

/* say how big would like to be */
static XtGeometryResult
QueryGeometry(Widget, XtWidgetGeometry *, XtWidgetGeometry *);

static Boolean
SetValues(Widget, Widget, Widget, ArgList, Cardinal*);

static void
Redisplay(Widget, XEvent *, Region);

static void
ClassPartInitialize(WidgetClass);

#define SUPERCLASS ((WidgetClass) &xmManagerClassRec)

XmPortholeClassRec xmPortholeClassRec = {
  { /* core fields */
    /* superclass		*/	SUPERCLASS,
    /* class_name		*/	"XmPorthole",
    /* widget_size		*/	sizeof(XmPortholeRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	ClassPartInitialize,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	Realize,
    /* actions			*/	NULL,
    /* num_actions		*/	0,
    /* resources		*/	(XtResource*)resources,
    /* num_resources		*/	XtNumber(resources),
    /* xrm_class		*/	NULLQUARK,
    /* compress_motion		*/	TRUE,
    /* compress_exposure	*/	TRUE,
    /* compress_enterleave	*/	TRUE,
    /* visible_interest		*/	FALSE,
    /* destroy			*/	NULL,
    /* resize			*/	Resize,
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersionDontCheck,
    /* callback_private		*/	NULL,
    /* tm_table			*/	XtInheritTranslations,
    /* query_geometry		*/	(XtGeometryHandler) QueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  { /* composite fields */
    /* geometry_manager		*/	GeometryManager,
    /* change_managed		*/	ChangeManaged,
    /* insert_child		*/	XtInheritInsertChild,
    /* delete_child		*/	XtInheritDeleteChild,
    /* extension		*/	NULL
  },
  {		/* constraint_class fields */
    /* resource list        */         NULL,
    /* num resources        */         0,	
    /* constraint size      */         0,	
    /* init proc            */         NULL, 
    /* destroy proc         */         NULL,
    /* set values proc      */         NULL, 
    /* extension            */         NULL, 
  },
  {		/* manager_class fields */
    /* default translations   */      XtInheritTranslations,	
    /* get_cont_resources     */      NULL, 
    /* num_syn_resources      */      0,
    /* syn_cont_resources     */      NULL,
    /* num_syn_cont_resources */      0,
    /* parent_process         */      XmInheritParentProcess,
    /* extension	      */      NULL,	
  },
  { /* porthole fields */
    /* Find Child Routine */		FindChild,
    /* Usable Width */			UsableWidth,
    /* Usable Width */			UsableHeight,
    /* Layout Children */		LayoutChildren,
    /* Place Children */		PlaceChildren,
    /* extension    */			0
  }
};

WidgetClass xmPortholeWidgetClass = (WidgetClass) &xmPortholeClassRec;

/*****************************************************************************
 *                                                                           *
 *			       utility routines                              *
 *                                                                           *
 *****************************************************************************/

/*	Function Name: SendReport
 *	Description: Sends an XmScrollReport to all the callbacks on the
 *                   report callback list.
 *	Arguments: pw - the porthole widget.
 *                 changed - which elements have changed.
 *	Returns: none.
 */

static void 
SendReport (XmPortholeWidget pw, unsigned int changed)
{
    XmPortholeWidgetClass wc = (XmPortholeWidgetClass) XtClass((Widget) pw);
    Widget child = XmPorthole_current_child(pw);

    if (XmPorthole_report_callbacks(pw) && child) {
	XmScrollReport prep;

	prep.changed = changed;
	prep.slider_x = -child->core.x;	/* porthole is "inner" */
	prep.slider_y = -child->core.y;	/* child is outer since it is larger */
	prep.slider_width = (*wc->porthole_class.usable_width) ((Widget) pw);
	prep.slider_height = (*wc->porthole_class.usable_height) ((Widget) pw);
	prep.canvas_width = child->core.width;
	prep.canvas_height = child->core.height;

	XtCallCallbackList ((Widget)pw, XmPorthole_report_callbacks(pw),
			    (XtPointer) &prep);
    }
}

/*	Function Name: PannerCallback
 *	Description: called when the panner has moved.
 *	Arguments: panner - the panner widget.
 *                 porthole_ptr - pointer to the porthole widget.
 *                 report_ptr - the panner record.
 *	Returns: none.
 */

/* ARGSUSED */
static void 
PannerCallback(Widget w, XtPointer porthole_ptr, XtPointer report_ptr)
{
    Arg args[3];
    Cardinal n;
    XmPortholeWidget pw = (XmPortholeWidget) porthole_ptr;
    Widget child = XmPorthole_current_child(pw);
    XmScrollReport *report = (XmScrollReport *) report_ptr;
    unsigned char units;
    
    if ((child == NULL) || !(report->changed & (XmPRSliderX | XmPRSliderY)))
	return;

    /*
     * Reports always come in in pixels, we have to make sure
     * that the set values goes through as pixels or bad things will happen...
     */

    XtSetArg (args[0], XmNunitType, &units);
    XtGetValues(child, args, (Cardinal) 1);

    n = 0;
    XtSetArg (args[n], XmNunitType, XmPIXELS); n++;
    if (report->changed & XmPRSliderX) {
	XtSetArg (args[n], XmNx, -report->slider_x); n++;
    }
    if (report->changed & XmPRSliderY) {
	XtSetArg (args[n], XmNy, -report->slider_y); n++;
    }
    XtSetValues(child, args, n);

    /*
     * Reset the unit type.
     */

    XtSetArg (args[0], XmNunitType, units);
    XtSetValues(child, args, (Cardinal) 1);
}

/*	Function Name: PortholeCallback
 *	Description: called when the porthole or its child has
 *                   changed 
 *	Arguments: porthole - the porthole widget.
 *                 panner_ptr - the panner widget.
 *                 report_ptr - the porthole record.
 *	Returns: none.
 */

/* ARGSUSED */
static void 
PortholeCallback(Widget w, XtPointer panner_ptr, XtPointer report_ptr)
{
    Arg args[10];
    Cardinal n;
    Widget panner = (Widget) panner_ptr;
    XmScrollReport *report = (XmScrollReport *) report_ptr;
    unsigned char units;

    /*
     * Reports always come in in pixels, we have to make sure
     * that the set values goes through as pixels or bad things will happen...
     */
    
    n = 0;
    XtSetArg (args[n], XmNunitType, &units); n++;
    XtGetValues(panner, args, n);

    n = 0;
    XtSetArg (args[n], XmNunitType, XmPIXELS); n++;
    /*
     * The Panner cannot be allowed to resize since report callbacks
     * are sent to the panner for the resize proc of the porthole.  This
     * could cause a change that requires a change in the porthole which
     * quickly becomes an infinite loop.
     */
    XtSetArg (args[n], XmNresize, False); n++;
    if (report->changed & XmPRSliderX) {
	XtSetArg (args[n], XmNsliderX, report->slider_x); n++;
    }
    if (report->changed & XmPRSliderY) {
	XtSetArg (args[n], XmNsliderY, report->slider_y); n++;
    }
    if (report->changed & XmPRSliderWidth) {
	XtSetArg (args[n], XmNsliderWidth, report->slider_width); n++;
    }
    if (report->changed & XmPRSliderHeight) {
	XtSetArg (args[n], XmNsliderHeight, report->slider_height); n++;
    }
    if (report->changed & XmPRCanvasWidth) {
	XtSetArg (args[n], XmNcanvasWidth, report->canvas_width); n++;
    }
    if (report->changed & XmPRCanvasHeight) {
	XtSetArg (args[n], XmNcanvasHeight, report->canvas_height); n++;
    }
    XtSetValues (panner, args, n);

    /*
     * Reset the unit type.
     */

    n = 0;
    XtSetArg (args[n], XmNunitType, units); n++;
    XtSetValues(panner, args, n);
}

/*	Function Name: ReparentChild
 *	Description: Reparents the managed child of the porthole.
 *	Arguments: w - the porthole widget.
 *	Returns: none.
 */

static void
ReparentChild(Widget w)
{
    XmPortholeWidget pw = (XmPortholeWidget) w;
    Widget child = XmPorthole_current_child(pw);

    if (!XtIsRealized(w))
	return;

    if (child == NULL) {
	XUnmapWindow(XtDisplay(w), XmPorthole_clip_win(pw));
	return;
    }
    /* 
     * We know that the child is managed 
     */
    else if (child->core.mapped_when_managed) {
	XMapWindow(XtDisplay(w), XmPorthole_clip_win(pw));
	XtRealizeWidget(child);
	XtMapWidget(child);
    }
    else
	XtRealizeWidget(child);

    XReparentWindow(XtDisplay(child), XtWindow(child), XmPorthole_clip_win(pw),
		    child->core.x, child->core.y);
}

/*****************************************************************************
 *                                                                           *
 *			     Core Class Methods                              *
 *                                                                           *
 *****************************************************************************/

/*	Function Name: ClassInitialize
 *	Description: Initialize class-specific data
 *	Arguments: none.
 *	Returns: none.
 */
static void
ClassInitialize()
{
    XmPortholeClassRec* wc = &xmPortholeClassRec;
}

/*	Function Name: ClassPartInitialize
 *	Description: handles inheritance of class functions.
 *	Arguments: class - the widget class of this widget.
 *	Returns: none.
 */

static void
ClassPartInitialize(WidgetClass class)
{
    XmPortholeWidgetClass superC, wc = (XmPortholeWidgetClass) class;
    
    superC = (XmPortholeWidgetClass)wc->core_class.superclass;

    /* 
     * We don't need to check for NULL super since we'll get to The functions
     * defined by the Porthole class eventually.
     */

    if (wc->porthole_class.find_child == XtInheritFindChild) 
	wc->porthole_class.find_child = superC->porthole_class.find_child;

    if (wc->porthole_class.usable_width == XtInheritUsable) 
	wc->porthole_class.usable_width = superC->porthole_class.usable_width;

    if (wc->porthole_class.usable_height == XtInheritUsable) 
	wc->porthole_class.usable_height= superC->porthole_class.usable_height;

    if (wc->porthole_class.layout == XtInheritLayout) 
	wc->porthole_class.layout = superC->porthole_class.layout;

    if (wc->porthole_class.place_children == XtInheritPlaceChildren) {
	wc->porthole_class.place_children = 
	    superC->porthole_class.place_children;
    }
}

/*	Function Name: Initialize
 *	Description:   Initializes this widget.
 *	Arguments:     req - what was originally requested.
 *                     set - what will be created (our superclassed have
 *                           already mucked with this)
 *                     args, num_args - The arguments passed to 
 *                                      the creation call.
 *	Returns:       none.
 */

/*ARGSUSED*/
static void 
Initialize(Widget req, Widget set, ArgList args, Cardinal * num_args)
{
    XmPortholeWidget pw = (XmPortholeWidget) set;
    XRectangle *clip = &(XmPorthole_clip(pw));
    Dimension st = pw->manager.shadow_thickness;
    XmPortholeWidgetClass wc = (XmPortholeWidgetClass) XtClass(set);

#ifdef DEMO
    _XmInitialIzeConverters(req);
#endif

    if (pw->core.width == 0)
	pw->core.width = 100; /* Set a Default Size. */
    
    if (pw->core.height == 0)
	pw->core.height = 100;

    clip->x = st + XmPorthole_margin_width(pw);
    clip->y = st + XmPorthole_margin_height(pw);
    clip->width = (*wc->porthole_class.usable_width) (set);
    clip->height = (*wc->porthole_class.usable_height) (set);

    XmPorthole_current_child(pw) = NULL;
}

/*	Function Name: Realize
 *	Description:   Called to realize this widget.
 *	Arguments:     w - Widget to realize.
 *                     valueMask, attributes - attributes to use when creating
 *                     this widget's window.
 *	Returns:       none.
 *
 * This overrides the Manager's frobbing with various values.
 */

static void 
Realize(register Widget w, Mask *valueMask, XSetWindowAttributes *attributes)
{
    XSetWindowAttributes clip_attrs;
    Mask clip_mask;
    XmPortholeWidget pw = (XmPortholeWidget) w;
    XRectangle *clip = &(XmPorthole_clip(pw));

    XtCreateWindow (w, InputOutput, CopyFromParent, *valueMask, attributes);
    
    if (XmPorthole_force_child_to_fill(pw)) {
	clip_mask = 0;
    }
    else {
	clip_mask = CWBackPixel;
	clip_attrs.background_pixel = w->core.background_pixel;
    }
    
    XmPorthole_clip_win(pw) = XCreateWindow(XtDisplay(w), XtWindow(w), 
					  (int) clip->x, (int) clip->y,
					  (unsigned int) clip->width,
					  (unsigned int) clip->height,
					  0, CopyFromParent, CopyFromParent,
					  CopyFromParent, 
					  clip_mask, &clip_attrs);
    
    if (pw->core.background_pixmap != XtUnspecifiedPixmap) {
	XSetWindowBackgroundPixmap(XtDisplay(w), XmPorthole_clip_win(pw), 
				   pw->core.background_pixmap);
    }

    ReparentChild(w);
}

/*	Function Name: Redisplay
 *	Description:   This function redraws the porthole shadows.
 *	Arguments:     w - the Porthole widget.
 *                     event - event that caused the exposure.
 *                     region - the region containing all the exposures.
 *	Returns:       none
 */

/* ARGSUSED */
static void
Redisplay(Widget w, XEvent * event, Region region)
{
    XmPortholeWidget pw = (XmPortholeWidget) w;
    Position x, y;
    Dimension width, height, st = pw->manager.shadow_thickness;
    
    x = XmPorthole_clip(pw).x - st;
    y = XmPorthole_clip(pw).y - st;

    st *= 2;
    width = XmPorthole_clip(pw).width + st;
    height = XmPorthole_clip(pw).height + st;
    XmeDrawShadows(XtDisplay(w), XtWindow(w), pw->manager.top_shadow_GC,
		   pw->manager.bottom_shadow_GC, x, y, width, height, 
		   pw->manager.shadow_thickness, XmSHADOW_IN);
}

/*	Function Name: Resize
 *	Description:   Called when this widget has been resized.
 *	Arguments:     w - Extended List Widget to resize.
 *	Returns:       none.
 */

static void 
Resize (Widget w)
{
    XmPortholeWidget pw = (XmPortholeWidget) w;
    XmPortholeWidgetClass wc = (XmPortholeWidgetClass) XtClass(w);
    Widget child = XmPorthole_current_child(pw);

    XmPorthole_clip(pw).width = (*wc->porthole_class.usable_width) (w);
    XmPorthole_clip(pw).height = (*wc->porthole_class.usable_height) (w);

    if (XtIsRealized(w)) {
#if (XmVersion > 1001)
	XmDropSiteStartUpdate(w);
#endif

	XResizeWindow(XtDisplay(w), XmPorthole_clip_win(pw), 
		      (unsigned int) XmPorthole_clip(pw).width,
		      (unsigned int) XmPorthole_clip(pw).height);

#if (XmVersion > 1001)
	XmDropSiteEndUpdate(w);
#endif
    }

    /*
     * If we have a child, we need to make sure that it is at least as big
     * as we are and in the right place.
     */

    if (child) {
	Position x, y;
	Dimension width, height;

	(*wc->porthole_class.layout) (w, child, NULL, 
				      &x, &y, &width, &height);
	(*wc->porthole_class.place_children)(w, True, x, y, width, height);

	SendReport(pw, (unsigned int) (XmPRSliderWidth | XmPRSliderHeight |
				       XmPRCanvasWidth | XmPRCanvasHeight));
    }
}

/*	Function Name: QueryGeometry
 *	Description:   Called when my parent wants to know what size
 *                     I would like to be.
 *	Arguments:     w - the drt widget.
 *                     indended - constriants imposed by the parent.
 *                     preferred - what I would like.
 *	Returns:       See Xt Manual.
 */

static XtGeometryResult 
QueryGeometry (Widget w, 
	       XtWidgetGeometry *intended, XtWidgetGeometry *preferred)
{
    register XmPortholeWidget pw = (XmPortholeWidget) w;
    XmPortholeWidgetClass wc = (XmPortholeWidgetClass) XtClass(w);
    Widget child = XmPorthole_current_child(pw);

    if (child == NULL) {
    	preferred->width = w->core.width;
	preferred->height = w->core.height;
    }
    else {
	unsigned int usable_width, usable_height;
	int vedge, hedge;

	usable_width = (*wc->porthole_class.usable_width) (w);
	usable_height = (*wc->porthole_class.usable_height) (w);

	hedge = (w->core.width - usable_width);
	vedge = (w->core.height - usable_height);

	preferred->width = XmPorthole_p_child_width(pw) + hedge;
	preferred->height = XmPorthole_p_child_height(pw) + vedge;
    }

    return(_XmHWQuery(w, intended, preferred));
}

/*	Function Name: SetValues
 *	Description:   Called a resources is changed.
 *	Arguments:     current - the current (old) widget values.
 *                     request - before superclassed have changed things.
 *                     set - what will acutally be the new values. 
 *                     args, num_args - the arguments in the list.
 *	Returns:       none
 */

/* ARGSUSED */
static Boolean 
SetValues(Widget current, Widget request, Widget set,
	  ArgList args, Cardinal * num_args)
{
    XmPortholeWidget c_pw = (XmPortholeWidget) current;
    XmPortholeWidget pw = (XmPortholeWidget) set;
    XmPortholeWidgetClass wc = (XmPortholeWidgetClass) XtClass(set);
    Boolean redisplay = False;
    Boolean layout = False;

    /*
     * Motif gets in the way, override.
     */
    
    if (request->core.width != set->core.width)
	set->core.width = request->core.width;

    if (request->core.height != set->core.height)
	set->core.height = request->core.height;

    /*
     * If Force child to fill is True the force the clip background to none,
     * otherwise set the clip background pixmap and pixel to track the 
     * porthole.
     */

    if (XtIsRealized(set)) {
	if (XmPorthole_force_child_to_fill(c_pw) != 
	    XmPorthole_force_child_to_fill(pw))
	{
	    Widget child;

	    if (XmPorthole_force_child_to_fill(pw)) {
		XSetWindowBackgroundPixmap(XtDisplay(set),
					   XmPorthole_clip_win(pw), None);
	    }

	if ((child = XmPorthole_current_child(pw)) != NULL) {
		Position x, y;
		Dimension width, height;
		
		(*wc->porthole_class.layout)(set, child, NULL,
					     &x, &y, &width, &height);
		(*wc->porthole_class.place_children)(set, True,
						     x, y, width, height);
		
		SendReport(pw, 
			   (unsigned int)(XmPRSliderWidth | XmPRSliderHeight |
					  XmPRCanvasWidth | XmPRCanvasHeight));
	    }
	
	    if (c_pw->core.background_pixmap == XtUnspecifiedPixmap) {
		XSetWindowBackground(XtDisplay(set), XmPorthole_clip_win(pw), 
				     pw->core.background_pixel);
	    }
	    else {
		XSetWindowBackgroundPixmap(XtDisplay(set),
					   XmPorthole_clip_win(pw), 
					   pw->core.background_pixmap);
	    }

	    XClearWindow(XtDisplay(set), XmPorthole_clip_win(pw));
	}

	if (!XmPorthole_force_child_to_fill(pw)) {
	    if (c_pw->core.background_pixmap !=c_pw->core.background_pixmap) {
		XSetWindowBackgroundPixmap(XtDisplay(set),
					   XmPorthole_clip_win(pw), 
					   pw->core.background_pixmap);
	    }
	    
	    if (c_pw->core.background_pixel != c_pw->core.background_pixel) {
		XSetWindowBackground(XtDisplay(set), XmPorthole_clip_win(pw), 
					 pw->core.background_pixel);
	    }
	}
    }

    if ((c_pw->manager.shadow_thickness != pw->manager.shadow_thickness) ||
        (XmPorthole_margin_height(c_pw) != XmPorthole_margin_height(pw)) ||
        (XmPorthole_margin_width(c_pw) != XmPorthole_margin_width(pw)))
    {
	if (XtIsRealized(set)) {
	    XRectangle *clip = &(XmPorthole_clip(pw));
	    Dimension st = pw->manager.shadow_thickness;

	    clip->x = st + XmPorthole_margin_width(pw);
	    clip->y = st + XmPorthole_margin_height(pw);
	    clip->width = (*wc->porthole_class.usable_width) (set);
	    clip->height = (*wc->porthole_class.usable_height) (set);

#if (XmVersion > 1001)
	    XmDropSiteStartUpdate(set);
#endif

	    XMoveWindow(XtDisplay(set), XmPorthole_clip_win(pw), 
			(int) clip->x, (int) clip->y);

	    XResizeWindow(XtDisplay(set), XmPorthole_clip_win(pw), 
			  (unsigned int) clip->width, 
			  (unsigned int) clip->height);

#if (XmVersion > 1001)
	    XmDropSiteEndUpdate(set);
#endif

	    layout = redisplay = True;
	}
    }


    if (layout) {
	Position x, y;
	Dimension width, height;
	Widget child = XmPorthole_current_child(pw);

	if (child != NULL) {
	    (*wc->porthole_class.layout)(set, child, NULL,
					 &x, &y, &width, &height);
	    (*wc->porthole_class.place_children)(set, True,
						 x, y, width, height);
	    SendReport (pw, 
			(unsigned int) (XmPRSliderWidth | XmPRSliderHeight));
	}
    }

    return(redisplay);
}

/************************************************************
 *
 *  Composite Class Methods.
 *
 ************************************************************/

/*	Function Name: GeometryManager
 *	Description:   handles requests from children for a size change.
 *	Arguments:     child - the child to change.
 *                     request - the geometry that the child wants.
 *                     return - what we will allow if this is an almost.
 *	Returns:       status.
 */

static XtGeometryResult 
GeometryManager (Widget w, XtWidgetGeometry *req, XtWidgetGeometry *reply)
{
    XmPortholeWidget pw = (XmPortholeWidget) w->core.parent;
    XmPortholeWidgetClass wc = (XmPortholeWidgetClass) XtClass((Widget) pw);
    Widget child = XmPorthole_current_child(pw);
    Boolean okay = TRUE;

    if (child != w) 
	return XtGeometryNo;  /* unknown child */

    *reply = *req;			/* assume we'll grant everything */

    if ((req->request_mode & CWBorderWidth) && (req->border_width != 0)) {
	reply->border_width = 0;	/* require border width of 0 */
	okay = FALSE;
    }

    (*wc->porthole_class.layout) ((Widget)pw, child, req, &reply->x, &reply->y,
				  &reply->width, &reply->height);

    if (((req->request_mode & CWX) && req->x != reply->x) ||
	((req->request_mode & CWY) && req->x != reply->x) ||
	((req->request_mode & CWWidth) && req->width != reply->width) ||
	((req->request_mode & CWHeight) && req->height != reply->height))
    {
	okay = FALSE;
    }

    /*
     * If we failed on anything, simply return without touching widget
     */

    if (!okay)
	return XtGeometryAlmost;

    /*
     * If not just doing a query, update widget and send report.  Note that
     * we will often set fields that weren't requested because we want to keep
     * the child visible.
     */

    if (!(req->request_mode & XtCWQueryOnly)) {
	Position x, y;
	Dimension width, height;
	unsigned int changed = 0;

	if (child->core.x != reply->x) {
	    changed |= XmPRSliderX;
	    x = reply->x;
	}
	else
	    x = child->core.x;

	if (child->core.y != reply->y) {
	    changed |= XmPRSliderY;
	    y = reply->y;
	}
	else
	    y = child->core.y;

	if (child->core.width != reply->width) {
	    changed |= XmPRCanvasWidth;
	    width = reply->width;
	}
	else
	    width = child->core.width;

	if (child->core.height != reply->height) {
	    changed |= XmPRCanvasHeight;
	    height = reply->height;
	}
	else
	    height = child->core.height;

	if (changed) {
	    (*wc->porthole_class.place_children)((Widget) pw, 
						 False, x, y, width, height);
	    SendReport (pw, changed);
	}
    }

    return XtGeometryYes;		/* success! */
}

/*	Function Name: ChangeManaged
 *	Description:   When a management change has occured...
 *	Arguments:     w - the icon box widget.
 *	Returns:       none.
 */

static void 
ChangeManaged(Widget w)
{
    XmPortholeWidget pw = (XmPortholeWidget) w;
    XmPortholeWidgetClass wc = (XmPortholeWidgetClass) XtClass(w);
    Widget child = (*wc->porthole_class.find_child)(w);
    unsigned int all;

    /*
     * If the managed child changes then reset the preferred child size.
     */

    if (XmPorthole_current_child(pw) != child) {
	/*
	 * This allows the widget to have the proper preferred size if 
	 * it is remanaged later on and becomes the "child" if the porthole
	 * once again.
	 */

	if ((XmPorthole_current_child(pw) != NULL) && 
	    (!XmPorthole_current_child(pw)->core.being_destroyed))
	{
	    _XmResizeWidget(XmPorthole_current_child(pw), 
			    XmPorthole_p_child_width(pw),
			    XmPorthole_p_child_height(pw),
			    XmPorthole_current_child(pw)->core.border_width);
	}

	XmPorthole_p_child_height(pw) = 0;
	XmPorthole_p_child_width(pw) = 0;
	XmPorthole_current_child(pw) = child;
    }

    if (child != NULL) {
	XtWidgetGeometry geom;
	Dimension rwidth, rheight;

	if ((XmPorthole_p_child_height(pw) == 0) ||
	    (XmPorthole_p_child_width(pw) == 0))
	{
	    /*
	     * Set the child's preferred sizes.
	     */
	    XmPorthole_p_child_height(pw) = child->core.height;
	    XmPorthole_p_child_width(pw) = child->core.width;
	}

	/*
	 * ||| Should only do this sometimes...
	 */

	XtQueryGeometry(w, NULL, &geom); /* Querying my own desired geometry */
	
	(void) _XmRequestNewSize(w, False, geom.width, geom.height,
				 &rwidth, &rheight);

	(*wc->core_class.resize)(w); /*Need to make sure resize proc called. */
	
        {
	    Position x, y;
	    Dimension width, height;

	    (*wc->porthole_class.layout)(w, child, NULL, 
					 &x, &y, &width, &height);

	    (*wc->porthole_class.place_children)(w, True, x, y, width, height);
	}

	all = (XmPRSliderX | XmPRSliderY | XmPRSliderWidth | XmPRSliderHeight |
	       XmPRCanvasWidth | XmPRCanvasHeight);

	SendReport (pw, all);
    }

    ReparentChild(w);

   XmeNavigChangeManaged(w);	/* for Motif navigation */
}

/************************************************************
 *
 *  Porthole Widget Class Methods.
 *
 ************************************************************/

/*	Function Name: FindChild
 *	Description: Finds the managed child of the porthole.
 *	Arguments: pw - the porthole widget.
 *	Returns: the child, or NULL if there is no managed child.
 */

static Widget
FindChild (Widget w)
{
    register XmPortholeWidget pw = (XmPortholeWidget) w;
    register Widget *childP;

    /*
     * Find the managed child on which we should operate.  Ignore multiple
     * managed children.
     */

    ForAllChildren(pw, childP)
	if (XtIsManaged(*childP) && !((*childP)->core.being_destroyed))
	    return *childP;

    return (Widget) NULL;
}

/*	Function Name: UsableHeight 
 *	Description: The amount of height avaliable to the dynamic child of 
 *                   the Porthole.
 *	Arguments: w - the porthole widget.
 *	Returns: The amount of usable height.
 */

static unsigned int 
UsableHeight(Widget w)
{
    XmPortholeWidget pw = (XmPortholeWidget) w;
    Dimension used;

    used = 2 * (pw->manager.shadow_thickness + XmPorthole_margin_height(pw));
    return((pw->core.height > used) ? (pw->core.height - used) : 1);
}

/*	Function Name: UsableWidth 
 *	Description: The amount of width avaliable to the dynamic child of 
 *                   the Porthole.
 *	Arguments: w - the porthole widget.
 *	Returns: The amount of usable width.
 */

static unsigned int
UsableWidth(Widget w)
{
    XmPortholeWidget pw = (XmPortholeWidget) w;
    Dimension used;

    used = 2 * (pw->manager.shadow_thickness + XmPorthole_margin_width(pw));
    return((pw->core.width > used) ? (pw->core.width - used) : 1);
}

/*	Function Name: LayoutChildren
 *	Description: Finds the new size and location for the child.
 *                   Mostly enforces the constraint that the child must
 *                   cover the entire clip window.
 *	Arguments: pw - the porthole widget.
 *                 child - the child.
 *                 geomp - if values are specified here they override
 *                         the child's current values.
 * RETURNED        xp, yp - new location.
 * RETURNED        widthp, heightp - new size.
 *	Returns: none.
 */

static void 
LayoutChildren(Widget w, Widget child, XtWidgetGeometry *geomp,
	    Position *xp, Position *yp, 
	    Dimension *widthp, Dimension *heightp)
{
    Position minx, miny;
    XmPortholeWidget pw = (XmPortholeWidget) w;
    XmPortholeWidgetClass wc = (XmPortholeWidgetClass) XtClass(w);
    unsigned int usable_width, usable_height;

    *xp = child->core.x;		/* default to current values */
    *yp = child->core.y;
    *widthp = child->core.width;
    *heightp = child->core.height;

    if (geomp != NULL) {		/* mix in any requested changes */
	if (geomp->request_mode & CWX) 
	    *xp = geomp->x;

	if (geomp->request_mode & CWY) 
	    *yp = geomp->y;

	if (geomp->request_mode & CWWidth) 
	    XmPorthole_p_child_width(pw) = geomp->width;
	*widthp = XmPorthole_p_child_width(pw);

	if (geomp->request_mode & CWHeight) 
	    XmPorthole_p_child_height(pw) = geomp->height;
	*heightp = XmPorthole_p_child_height(pw);
    }
    else {
	*widthp = XmPorthole_p_child_width(pw);
	*heightp = XmPorthole_p_child_height(pw);
    }

    usable_width = (*wc->porthole_class.usable_width) (w);
    usable_height = (*wc->porthole_class.usable_height) (w);

    /*
     * Make sure that the child is at least as large as the porthole; there
     * is no maximum size.
     */

    if (XmPorthole_force_child_to_fill(pw)) {
	ASSIGN_MAX(*widthp, usable_width); 
	ASSIGN_MAX(*heightp, usable_height);
    }

    /*
     * Make sure that the child is still on the screen.  Note that this must
     * be done *after* the size computation so that we know where to put it.
     */

    minx = ((Position) usable_width) - ((Position) *widthp);
    miny = ((Position) usable_height) - ((Position) *heightp);

    /*
     * Keep at lower right corner 
     */

    ASSIGN_MAX(*xp, minx);
    ASSIGN_MAX(*yp, miny);

    /*
     * Keep at upper left corner 
     */

    ASSIGN_MIN(*xp, 0);
    ASSIGN_MIN(*yp, 0);
}

/*	Function Name: PlaceChildren
 *	Description: Places all children of the porthole
 *	Arguments: w - the porthole widget.
 *                 resize_child - Actually resize the child, or just set
 *                                the values, used in GeometryManager.
 *                 x, y, width, height - new size of the child.
 *	Returns: none
 */

static void
PlaceChildren(Widget w, Boolean resize_child, Position x, Position y,
	      Dimension width, Dimension height)
{
    XmPortholeWidget pw = (XmPortholeWidget) w;
    Widget child = XmPorthole_current_child(pw);

    if (resize_child) {
	_XmConfigureWidget (child, x, y, width, height, (Dimension) 0);
    }
    else {
	child->core.x = x;
	child->core.y = y;
	child->core.width = width;
	child->core.height = height;
    }
}

/************************************************************
 *
 *  Public routines.
 *
 ************************************************************/

/*	Function Name: XmPortholeConnectPanner
 *	Description: Allows a porthole and panner to be easily connected.
 *	Arguments: porthole - the porthole widget.
 *                 panner - the panner widget.
 *	Returns: none.
 */

void
XmPortholeConnectPanner(Widget porthole, Widget panner)
{
    unsigned int all;

    XtAddCallback(porthole, 
		  XmNreportCallback, PortholeCallback, (XtPointer) panner);
    XtAddCallback(panner, 
		  XmNreportCallback, PannerCallback, (XtPointer) porthole);

    all = (XmPRSliderX | XmPRSliderY | XmPRSliderWidth | XmPRSliderHeight |
	   XmPRCanvasWidth | XmPRCanvasHeight);
    
    SendReport ((XmPortholeWidget) porthole, all);
}

/************************************************************
 *
 *  Public functions.
 *
 ************************************************************/

/*	Function Name: XmCreatePorthole
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The created widget.
 */

Widget
XmCreatePorthole(Widget parent, String name,
		 ArgList args, Cardinal num_args)
{
    return(XtCreateWidget(name, xmPortholeWidgetClass,
			  parent, args, num_args));
}

void XmPortholeVisible(Widget w, Widget descendant)
{
	XmPortholeWidget pw = (XmPortholeWidget) w;
	Widget child = descendant;

	while (child && (XtParent(child) != w))
		child = XtParent(child);
	if (NULL == child)
		return;		/* not a descendant */
	
    	if (XmPorthole_current_child(pw) != child) 
	{
	    	XtUnmanageChild(XmPorthole_current_child(pw));
		XtManageChild(child);
	}

	/* at this point, child == XmPorthole_current_child() and is managed */

	{
	Position src[2], dest[2];	/* x is 0, y is 1 */
	Position d[2];
	Dimension pw_dim[2];		/* width is 0, height is 1 */
	Position child_pos[2];
	int i;

	XtTranslateCoords(descendant, ((int)XtWidth(descendant))/2, ((int)XtHeight(descendant))/2, &src[0], &src[1]);
	XtTranslateCoords(child, 0, 0, &dest[0], &dest[1]);

	pw_dim[0] = XtWidth(pw);
	pw_dim[1] = XtHeight(pw);
	child_pos[0] = XtX(child);
	child_pos[1] = XtY(child);

	for (i=0; i<=1; i++)
	{
		d[i] = src[i] - dest[i] + child_pos[i] - ((int)pw_dim[i])/2;
	}

	XtX(child) -= d[0];
	XtY(child) -= d[1];
	_XmConfigureWidget (child, child->core.x, child->core.y, child->core.width, child->core.height, child->core.border_width);
	}

	SendReport (pw, XmPRSliderX | XmPRSliderY);
}

