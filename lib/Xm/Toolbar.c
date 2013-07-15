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

/************************************************************
*	INCLUDE FILES
*************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <Xm/ToolbarP.h>
#include <Xm/XmP.h>
#include <Xm/Xm.h>
#include "XmI.h"
#include <X11/Shell.h>
#include <Xm/Label.h>
#include <Xm/ExtP.h>

#include <stdlib.h> /* It only for removing warning at line: 402*/

/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

#define SUPERCLASS ((WidgetClass) &xmManagerClassRec)

#define INFINITY 32767		/* a very large short. */

/************************************************************
*	MACROS
*************************************************************/

#define IsVert(w) (XmToolbar_orientation(w) == XmVERTICAL)

/************************************************************
*	GLOBAL DECLARATIONS
*************************************************************/

/************************************************************
*	STATIC FUNCTION DECLARATIONS
*************************************************************/
static void ClassInitialize();
static void LeaveToolbarChild(Widget, XtPointer, XEvent *, Boolean *);
static void Realize(Widget, Mask *, XSetWindowAttributes *);
static void Resize(Widget), ChangeManaged(Widget), InsertChild(Widget);
static void DeleteChild(Widget);
static void Initialize(Widget, Widget, ArgList, Cardinal *);

static Boolean SetValues(Widget, Widget, Widget, ArgList, Cardinal *);
static Boolean ToolbarSetValues(Widget, Widget, Widget, ArgList, Cardinal *);

static XtGeometryResult GeometryManager(Widget, XtWidgetGeometry *,
					XtWidgetGeometry *);
static XtGeometryResult QueryGeometry(Widget,
				      XtWidgetGeometry *, XtWidgetGeometry *);
static void ToolbarDestroy(Widget);
static void ConstraintDestroy(Widget);
static void ConstraintInitialize(Widget, Widget, ArgList, Cardinal *);

/************************
 * Actions and callbacks.
 ************************/
static void PopupLabelTimerEvent(XtPointer, XtIntervalId *);
static void AddPopupTimer(Widget, XtPointer, XEvent *, Boolean *);
static void RemovePopupTimer(Widget, XtPointer, XEvent *, Boolean *);
static void PopdownLabelShell(Widget, XtPointer, XEvent *, Boolean *);

/*********************
 * Internal Routines.
 *********************/

static XtGeometryResult SetPrefSize(Widget, Boolean, 
				    Boolean, Dimension *, Dimension *);
static void GetPrefSize(Widget, XtWidgetGeometry *, XtWidgetGeometry *);
static void PlaceChildren(Widget, Widget, Dimension, Boolean, Dimension *);
static void InsertInGroupList(XmToolbarWidget, Widget);
static int  CompareGroupIds(void *, void *);
static int  CompareGroupPositions(void *, void *);
static void ResetGroups(XmToolbarWidget, Widget);
static void AddPopupEventHandlers(Widget);
static void PopupLabel(Widget, Widget);
static void PopdownLabel(Widget);
static void PopupCheckSetRenderTable(Widget, int, XrmValue*);

/************************************************************
*	STATIC DECLARATIONS
*************************************************************/

static XtResource resources[] = {
  {XmNenterChildCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffsetOf(XmToolbarRec, toolbar.enter_child_callback),
     XmRPointer, (XtPointer)NULL},
  {XmNgroupSpacing, XmCGroupSpacing, XmRHorizontalDimension,sizeof(Dimension),
     XtOffsetOf(XmToolbarRec, toolbar.group_spacing),
     XmRImmediate, (XtPointer) 0},
  {XmNhorizontalMargin, XmCMargin, XmRHorizontalDimension, sizeof(Dimension),
     XtOffsetOf(XmToolbarRec, toolbar.h_margin),
     XmRImmediate, (XtPointer) 0},
  {XmNleaveChildCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     XtOffsetOf(XmToolbarRec, toolbar.leave_child_callback),
     XmRPointer, (XtPointer)NULL},
  {XmNorientation, XmCOrientation, XmROrientation, sizeof(unsigned char),
     XtOffsetOf(XmToolbarRec, toolbar.orientation),
     XmRImmediate, (XtPointer) XmHORIZONTAL},
  {XmNpopupDelay, XmCPopupDelay, XmRCardinal, sizeof(Cardinal),
     XtOffsetOf(XmToolbarRec, toolbar.popup_delay),
     XmRImmediate, (XtPointer)500},
  {XmNpopupLabelEnabled, XmCPopupLabelEnabled, XmRBoolean, sizeof(Boolean),
     XtOffsetOf(XmToolbarRec, toolbar.popup_enabled),
     XmRImmediate, (XtPointer)False},
  {XmNpopupForeground, XmCForeground, XmRPixel, sizeof(Pixel),
     XtOffsetOf(XmToolbarRec, toolbar.popup_foreground),
     XmRCallProc, (XtPointer)_XmForegroundColorDefault},
  {XmNpopupBackground, XmCBackground, XmRPixel, sizeof(Pixel),
     XtOffsetOf(XmToolbarRec, toolbar.popup_background),
     XmRCallProc, (XtPointer)_XmBackgroundColorDefault},
  {XmNpopupFontList, XmCFontList, XmRFontList, sizeof(XmFontList),
     XtOffsetOf(XmToolbarRec, toolbar.popup_fontlist),
     XmRCallProc, (XtPointer)PopupCheckSetRenderTable},
  {"pri.vate","Pri.vate", XmRBoolean, sizeof(Boolean),
      XtOffsetOf(XmToolbarRec, toolbar.popup_check_set_render_table),
      XmRImmediate, (XtPointer) False},
  {XmNpopupRenderTable, XmCRenderTable, XmRRenderTable, sizeof(XmRenderTable),
    XtOffsetOf(XmToolbarRec, toolbar.popup_fontlist),
    XmRCallProc, (XtPointer)PopupCheckSetRenderTable},
  {XmNverticalMargin, XmCMargin, XmRVerticalDimension, sizeof(Dimension),
     XtOffsetOf(XmToolbarRec, toolbar.v_margin),
     XmRImmediate, (XtPointer) 0},
};

static XmSyntheticResource get_resources[] =
{
  {XmNhorizontalMargin, sizeof(Dimension),
      XtOffsetOf(XmToolbarRec, toolbar.h_margin),
      XmeFromHorizontalPixels, (XmImportProc) XmeToHorizontalPixels },
  {XmNverticalMargin, sizeof(Dimension),
      XtOffsetOf(XmToolbarRec, toolbar.v_margin),
      XmeFromVerticalPixels, (XmImportProc) XmeToVerticalPixels },
  {XmNgroupSpacing, sizeof(Dimension),
      XtOffsetOf(XmToolbarRec, toolbar.group_spacing), 
      XmeFromHorizontalPixels, (XmImportProc) XmeToHorizontalPixels },
};

static XtResource subresources[] = {
  {XmNentryLabelString, XmCEntryLabelString, XmRXmString, sizeof(XmString),
     XtOffsetOf(XmToolbarConstraintsRec, toolbar.entry_label),
     XmRImmediate, (XtPointer) NULL},
  {XmNgroupPosition, XmCGroupPosition,XmRUnsignedChar,sizeof(unsigned char),
     XtOffsetOf(XmToolbarConstraintsRec, toolbar.position),
     XmRImmediate, (XtPointer) 0},
  {XmNtoolbarGroup, XmCToolbarGroup, XmRUnsignedChar, sizeof(unsigned char),
     XtOffsetOf(XmToolbarConstraintsRec, toolbar.group),
     XmRImmediate, (XtPointer) 0},
  {XmNtoolbarEntryData, XmCToolbarEntryData, XmRPointer, sizeof(XtPointer),
     XtOffsetOf(XmToolbarConstraintsRec, toolbar.entry_data),
     XmRPointer, (XtPointer) NULL},
};

    
XmToolbarClassRec xmToolbarClassRec = {
  { /* core fields */
    /* superclass		*/	SUPERCLASS,
    /* class_name		*/	"XmToolbar",
    /* widget_size		*/	sizeof(XmToolbarRec),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
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
    /* destroy			*/	ToolbarDestroy,
    /* resize			*/	Resize,
    /* expose			*/	NULL,
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
   {		/* composite_class fields */
    /* geometry_manager   */      GeometryManager,
    /* change_managed     */      ChangeManaged,
    /* insert_child       */      InsertChild,			
    /* delete_child       */      DeleteChild,
    /* extension          */      NULL,                                     
   },
   {		/* constraint_class fields */
    /* resource list        */         (XtResource*)subresources,
    /* num resources        */         XtNumber(subresources),
    /* constraint size      */         sizeof(XmToolbarConstraintsRec),
    /* init proc            */         ConstraintInitialize,
    /* destroy proc         */         ConstraintDestroy,
    /* set values proc      */         ToolbarSetValues,
    /* extension            */         NULL, 
   },
   {		/* manager_class fields */
    /* default translations   */      XtInheritTranslations,	
    /* syn_resources          */      get_resources,
    /* num_syn_resources      */      XtNumber(get_resources),
    /* syn_cont_resources     */      NULL,
    /* num_syn_cont_resources */      0,
    /* parent_process         */      XmInheritParentProcess,
    /* extension	      */      NULL,	
   },
   {		/* Toolbar fields */
    /* extension              */      NULL,
   }
};

WidgetClass xmToolbarWidgetClass = (WidgetClass)&xmToolbarClassRec;

/************************************************************
*	STATIC CODE
*************************************************************/

/*	Function Name: ClassInitialize
 *	Description:   Called to initialize the class
 *	Arguments:     none
 *	Returns:       nothing
 */
static void
ClassInitialize()
{
    XmToolbarClassRec* wc = &xmToolbarClassRec;
}

/*	Function Name: Initialize
 *	Description:   Called to initialize information specific
 *                     to this widget.
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
    XmToolbarWidget tbw = (XmToolbarWidget) set;

#ifdef DEMO
    _XmInitialIzeConverters(req);
#endif

    XmToolbar_minor_col_space(tbw) = 0;
    XmToolbar_group_lists(tbw) = NULL;
    XmToolbar_num_groups(tbw) = 0;
    XmToolbar_popped_up(tbw) = False;
    XmToolbar_popup_timer(tbw) = (XtIntervalId)NULL;
    XmToolbar_popup_shell(tbw) = NULL;
    XmToolbar_popup_label(tbw) = NULL;
    XmToolbar_popup_delay_restore(tbw) = XmToolbar_popup_delay(tbw);
    XmToolbar_do_place(tbw) = False;
    
    if (XmToolbar_popup_fontlist(tbw) == NULL)
    {
        XmToolbar_popup_fontlist(tbw) = XmeGetDefaultRenderTable((Widget)tbw,
							    XmLABEL_FONTLIST);
    }
    /* Make a local copy of the font list */
    XmToolbar_popup_fontlist(tbw) = XmFontListCopy(XmToolbar_popup_fontlist(tbw));

    if( XtWidth(req) == 0 )
    {
	set->core.width = 10;
    }
    if( XtHeight(req) == 0 )
    {
	set->core.height = 10;
    }
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
Realize(Widget w, Mask *valueMask, XSetWindowAttributes *attributes)
{
    XtCreateWindow (w, InputOutput, CopyFromParent, *valueMask, attributes);
}

/*	Function Name: Resize
 *	Description:   Called when this widget has been resized.
 *	Arguments:     w - the widget to resize. 
 *	Returns:       none.
 */

static void 
Resize(Widget w)
{
    XmToolbarWidget tbw = (XmToolbarWidget) w;
    Dimension major_space;

    if (IsVert(w))
	major_space = tbw->core.height;
    else
	major_space = tbw->core.width;

    PlaceChildren(w, NULL, major_space, True, NULL);
}



static Boolean 
XmToolbarPosOccupied( XmToolbarWidget tbw, int group_index, Widget set,
		     int *ret_pos )
{
  int i=0,pos;
  Boolean full = False;

  pos = XmToolbarC_position(set);

  while (i < (int)XmToolbar_GroupMemCount(tbw, group_index)){

    if ((pos == XmToolbarC_position(XmToolbar_GroupMembers(tbw, group_index)[i]))
	&& (XmToolbar_GroupMembers(tbw, group_index)[i] != set)){
      full = True;
      pos++;
    }
    i++;
  }

  *ret_pos = pos;

  return(full);

}


/*	Function Name: ToolbarSetValues
 *	Description:   Called when some constraint data needs to be modified 
 *                     on-the-fly.
 *	Arguments:     old - the current (old) widget values.
 *                     request - before superclassed have changed things.
 *                     set - what will acutally be the new values. 
 *                     args, num_args - The arguments passed to 
 *                                      the child's setvalues call.
 *	Returns:       none
 */

/* ARGSUSED */
static Boolean 
ToolbarSetValues(Widget old, Widget request, Widget set,
		 ArgList args, Cardinal * num_args)
{
    XmToolbarWidget	set_tbw = (XmToolbarWidget)XtParent(set);
    int group = 0; /* Initial value changed to zero. It'll disable warning at compile time.*/
    Boolean found = False, redisplay=False;
    int new_pos;
    Boolean retval = False;
    
    if ((XmToolbarC_group(old) != XmToolbarC_group(set) )||
	(XmToolbarC_position(old) != XmToolbarC_position(set))){
      for ( group = 0; group < XmToolbar_num_groups(set_tbw); group++ ) {
	if (XmToolbarC_group(set) == (XmToolbar_group_lists(set_tbw))[group]->group_id){
	  found = True;
	  break;
	}
      }
    }

    if (XmToolbarC_group(old) != XmToolbarC_group(set) )
    {
      /*
       * get new group member count for new position of widget since we
       * haven't specified where it should really go. We assume that if
       * nothing is specified then the user doesn't care if it keeps the
       * same position or not, so we put it at the end
       */

      if (found) {
	if (XmToolbarPosOccupied(set_tbw,group,set,&new_pos))
	  XmToolbarC_position(set) = new_pos;
      }

      ResetGroups(set_tbw, NULL);
      if (XtIsRealized((Widget)set_tbw)) redisplay = True;
    }

    else if ((XmToolbarC_position(old) != XmToolbarC_position(set))&&
	     (XmToolbarC_group(old) == XmToolbarC_group(set) )){

      if (XmToolbarPosOccupied(set_tbw,group,set,&new_pos))
	XmToolbarC_position(set) = new_pos;
      
      qsort(XmToolbar_GroupMembers(set_tbw, group), 
	    XmToolbar_GroupMemCount(set_tbw, group),
	    sizeof(Widget), (__compar_fn_t)CompareGroupPositions);
      
      if (XtIsRealized((Widget)set_tbw)) redisplay = True;

    }

    /* 
     *Force the parent's redisplay 
     */

    if (redisplay){
      /*
       * the following ugly little hack forces a geometry request so that a
       * redraw of the Toolbar parent will happen. These values are
       * returned to their proper values in GeometryManager
       */
	
	XmToolbar_do_place(set_tbw) = True; 
	XmToolbar_save_width(set_tbw) = set->core.width; 
	set->core.width = set->core.width + 10; 
	
	retval = True;
      /*
       * end of ugly little hack 
       */

    }

    if ( XmToolbarC_entry_label(old) != XmToolbarC_entry_label(set) )
    {
	if ( XmToolbarC_entry_label(old) != NULL )
	    XmStringFree(XmToolbarC_entry_label(old));
	
	if ( XmeStringIsValid( (XmString)XmToolbarC_entry_label(set)) )
	{
	    XmToolbarC_entry_label(set) =
		XmStringCopy(XmToolbarC_entry_label(set));
	}
	else
	{
	    XmToolbarC_entry_label(set) = NULL;
	}
    }
    return(retval);
}




/*    Function Name: ToolbarDestroy
 *    Description:   Destroys all data allocated by the widget
 *    Arguments:     w - the widget.
 *    Returns:       none.
 */

static void
ToolbarDestroy( Widget w )
{
    XmToolbarWidget  tbw = (XmToolbarWidget)w;
    register int i; 

    if (XmToolbar_group_lists(tbw) != NULL) {
	for (i = 0; i < XmToolbar_num_groups(tbw); i++) {
	    if (XmToolbar_group_lists(tbw)[i] != NULL) {
		XtFree((XtPointer) XmToolbar_group_lists(tbw)[i]->members);
		XtFree((XtPointer) XmToolbar_group_lists(tbw)[i]);
	    }
	}
	XtFree((XtPointer) XmToolbar_group_lists(tbw));
    }

    if (XmToolbar_popup_fontlist(tbw) != NULL)
	XmFontListFree (XmToolbar_popup_fontlist(tbw));
}


/*	Function Name: SetValues
 *	Description:   Called when some widget data needs to be modified on-
 *                     the-fly.
 *	Arguments:     current - the current (old) widget values.
 *                     request - before superclassed have changed things.
 *                     set - what will acutally be the new values. 
 *                     args, num_args - the arguments in the list.
 *	Returns:       none
 */

/*ARGSUSED*/
static Boolean 
SetValues(Widget current, Widget request, Widget set,
	  ArgList args, Cardinal * num_args)
{
    XmToolbarWidget 	old_tbw = (XmToolbarWidget) current;
    XmToolbarWidget 	set_tbw = (XmToolbarWidget) set;
    Boolean		redraw = False;
    
    if ((XmToolbar_group_spacing(old_tbw) != XmToolbar_group_spacing(set_tbw)) &&
	XtIsRealized(set))
    {
	redraw = True;
    }

    if (XmToolbar_orientation(old_tbw) != XmToolbar_orientation(set_tbw))
    {
	XtWidgetGeometry	pref, req;

	req.request_mode = 0;
	GetPrefSize(set, &req, &pref);

	XtHeight(set) = pref.height;
	XtWidth(set) = pref.width;
    }
    
    if (redraw)
    {
	return (True);
    }

    if (XmToolbar_popup_delay(old_tbw) != XmToolbar_popup_delay(set_tbw))
	XmToolbar_popup_delay_restore(set_tbw) = XmToolbar_popup_delay(set_tbw);
    
    return (False);
}

/*	Function Name: QueryGeometry
 *	Description:   Called when my parent wants to know what size
 *                     I would like to be.
 *	Arguments:     w - the widget to check.
 *                     indended - constriants imposed by the parent.
 *                     preferred - what I would like.
 *	Returns:       See Xt Manual.
 */
    
static XtGeometryResult 
QueryGeometry(Widget w,XtWidgetGeometry *intended, XtWidgetGeometry *preferred)
{
    GetPrefSize(w, intended, preferred);
    return(_XmHWQuery(w, intended, preferred));
}

/************************************************************
 *
 * Composite Information.
 *
 ************************************************************/

/*	Function Name: GeometryManager
 *	Description:   handles requests from children for a size change.
 *	Arguments:     child - the child to change.
 *                     request - the geometry that the child wants.
 *                     return - what we will allow if this is an almost.
 *	Returns:       status.
 */

/*ARGSUSED*/
static XtGeometryResult
GeometryManager(Widget w, XtWidgetGeometry * request, 
		XtWidgetGeometry * result)
{
    XtGeometryResult ret_val = XtGeometryNo;
    XmToolbarWidget tbw = (XmToolbarWidget) XtParent(w);
    Dimension major_space, minor_space, minor_used, r_major, r_minor;
    Dimension old_width, old_height, old_border_width;

    if (request->request_mode & 
	~(CWWidth | CWHeight | CWBorderWidth | XtCWQueryOnly)) 
    {
	return(XtGeometryNo);
    }

    if ( XmToolbar_do_place(tbw) && (request->request_mode & CWWidth) )
    {
      /*
       * this replaces bogus values set in ToolbarSetValues
       * to force a redisplay 
       */

	XmToolbar_do_place(tbw) = False;
	request->width = XmToolbar_save_width(tbw);
	w->core.width = XmToolbar_save_width(tbw);
    }
    
    old_width = w->core.width;
    if (request->request_mode & CWWidth) 
	w->core.width = request->width;

    old_height = w->core.height;
    if (request->request_mode & CWHeight) 
	w->core.height = request->height;

    old_border_width = w->core.border_width;
    if (request->request_mode & CWBorderWidth)
	w->core.border_width = request->border_width;

    if (SetPrefSize((Widget) tbw, 
		    True, True, &r_major, &r_minor) == XtGeometryYes) 
    {
	/* We got the size we wanted, allow child to resize. */
	ret_val = XtGeometryYes;
    }
    else {
	/*
	 * We did not get the size we wanted, allow child to resize only
	 * if it will fit in the current toolbar space. 
	 */
	if (IsVert(tbw)) {
	    major_space = tbw->core.height;
	    minor_space = tbw->core.width;
	}
	else {
	    major_space = tbw->core.width;
	    minor_space = tbw->core.height;
	}
	
	PlaceChildren((Widget) tbw, w, major_space, False, &minor_used);
	if (minor_used <= minor_space) {
	    ret_val = XtGeometryYes;
	}
    }

    if ((ret_val == XtGeometryYes) &&
	!(request->request_mode & XtCWQueryOnly))
    {
	SetPrefSize((Widget) tbw, False, True, &r_major, &r_minor);
	PlaceChildren((Widget) tbw, w, r_major, True, NULL);
	*result = *request;
    }
    else {
	w->core.width = old_width;
	w->core.height = old_height;
	w->core.border_width = old_border_width;
    }

    return(ret_val);
}

/*	Function Name: InsertChild
 *	Description: called when a new child is to be added.
 *	Arguments: w - the new child.
 *	Returns: none.
 *
 * This routine simply makes sure that no gadgets are added.
 */

static void 
InsertChild(Widget w)
{
    XtWidgetProc insert_child;
    if (_XmGadgetWarning(w)) return;

    AddPopupEventHandlers(w);

    _XmProcessLock();
    insert_child = *((CompositeWidgetClass) SUPERCLASS)->composite_class.insert_child;
    _XmProcessUnlock();
    (*insert_child)(w);

    /* Check for non-widgets - ButtonBox doesn't support gadgets. */
    if (_XmGadgetWarning(w))
    {
	return;
    }


    /*
     * Now make sure the groups lists are updated.
     */
    InsertInGroupList((XmToolbarWidget)XtParent(w), w);
}

/*	Function Name: DeleteChild
 *	Description: called when a child is being deleted.
 *	Arguments: w - the child.
 *	Returns: none.
 *
 */

static void 
DeleteChild(Widget w)
{
    XtWidgetProc delete_child;
    XmToolbarWidget	tbw = (XmToolbarWidget)XtParent(w);

    if ( XmToolbar_popped_up(tbw) )
    {
	PopdownLabel(XtParent(w));
    }

    if ( XmToolbar_popup_timer(tbw) != (XtIntervalId)NULL )
    {
	XtRemoveTimeOut(XmToolbar_popup_timer(tbw));
    }
    XmToolbar_popup_timer(tbw) = (XtIntervalId)NULL;

    _XmProcessLock();
    delete_child = *((CompositeWidgetClass) SUPERCLASS)->composite_class.delete_child;
    _XmProcessUnlock();
    (*delete_child)(w);

    ResetGroups( tbw, NULL);
}

/*	Function Name: ConstraintDestroy
 *	Description: Called to clean up after child with constraints is
 *                   destroyed.
 *	Arguments: w - the child that has been destroyed.
 *	Returns: none.
 */

static void 
ConstraintDestroy(Widget w)
{
    XmToolbarWidget tbw = (XmToolbarWidget)XtParent(w);
    Dimension major_space;
    
    /* Remove the subwidget info and destroy the pixmap */

    if (XmToolbarC_entry_label(w) != NULL)
    {
	XmStringFree(XmToolbarC_entry_label(w));
    }
    
    if (!tbw->core.being_destroyed) {

      ResetGroups(tbw, w);
    
      if (IsVert(tbw))
	major_space = tbw->core.height;
      else
	major_space = tbw->core.width;
      
      PlaceChildren((Widget)tbw, NULL, major_space, True, NULL);

    }

} /* DeleteChild */




/*
 * Function: ConstraintInitialize
 * Description: Initializes constraints on children
 * Arguments: req - the widget
 *            set - copy of the widget
 *            args - arguments passed
 *            num_args - number of arguments passed
 * Returns: none.
 */
/* ARGSUSED */
static void
ConstraintInitialize(Widget req, Widget set, ArgList args, Cardinal *num_args)
{
    if ( XmToolbarC_entry_label(set) != NULL )
    {
	XmToolbarC_entry_label(set) =
	    XmStringCopy(XmToolbarC_entry_label(set));
    }
}

/*	Function Name: ChangeManaged
 *	Description:   When a management change has occured...
 *	Arguments:     w - the icon box widget.
 *	Returns:       none.
 */

static void
ChangeManaged(Widget w)
{
    XmToolbarWidget tbw = (XmToolbarWidget) w;
    Dimension r_major, r_minor;
    Dimension major_space;
    
    if ( XmToolbar_popped_up(tbw) )
    {
	PopdownLabel(w);
    }
    SetPrefSize(w, False, True, &r_major, &r_minor);
    if (IsVert(w))
	major_space = tbw->core.height;
    else
	major_space = tbw->core.width;
    PlaceChildren(w, NULL, major_space, True, NULL);

    XmeNavigChangeManaged(w);	/* For Motif navigation */
}

/************************************************************
 *
 * Internal routines.
 *
 ************************************************************/

/*	Function Name: SetPrefSize
 *	Description: Sets the preferred size of the toolbar.
 *	Arguments: w - the toolbar widget.
 *                 query_only - query only?
 *                 request - make a request?
 * RETURNED        r_major, r_minor - the returned major and minor space used.
 *	Returns: the result of the query.
 */

static XtGeometryResult
SetPrefSize(Widget w,Boolean query_only, Boolean request_in,
	    Dimension *r_major, Dimension *r_minor)
{
    Widget *childP;
    XmToolbarWidget tbw = (XmToolbarWidget) w;
    Dimension total_major, max_minor, t_minor;

    total_major = max_minor = 0;

    if (IsVert(w)) {
	ForAllChildren(tbw, childP) {
	    total_major += ((*childP)->core.height + 
			    2 * (*childP)->core.border_width);
	    total_major += XmToolbar_v_margin(tbw);
	    t_minor =(*childP)->core.width + 2 * (*childP)->core.border_width;
	    if (t_minor > max_minor )
		max_minor = t_minor;
	}

	total_major += XmToolbar_v_margin(tbw);	/* add one extra margin. */
    }
    else {
	ForAllChildren(tbw, childP) {
	    total_major += ((*childP)->core.width + 
			    2 * (*childP)->core.border_width);
	    total_major += XmToolbar_h_margin(tbw);
	    t_minor =(*childP)->core.height + 2 * (*childP)->core.border_width;
	    if (t_minor > max_minor )
		max_minor = t_minor;
	}

	total_major += XmToolbar_h_margin(tbw);	/* add one extra margin. */
    }

    if ( XmToolbar_num_groups(tbw) > 0 )
    {
	total_major += ((XmToolbar_num_groups(tbw) - 1) * XmToolbar_group_spacing(tbw));
    }
    XmToolbar_minor_col_space(tbw) = max_minor;

    if (request_in) {
	XtGeometryResult ret_val;
	XtWidgetGeometry request, reply;

	if (query_only) 
	    request.request_mode = XtCWQueryOnly;
	else
	    request.request_mode = 0;
	request.request_mode |= CWWidth | CWHeight;

	if (IsVert(w)) {
	    max_minor += 2 * XmToolbar_h_margin(tbw);
	    request.width = max_minor;
	    request.height = total_major;
	}
	else {
	    max_minor += 2 * XmToolbar_v_margin(tbw);
	    request.height = max_minor;
	    request.width = total_major;
	}

	ret_val = XtMakeGeometryRequest(w, &request, &reply);
	if (ret_val == XtGeometryAlmost) {
	    if (reply.request_mode & (CWWidth | CWHeight)) {
		if (reply.request_mode & CWHeight &&
		    reply.height == request.height) {
		    reply.request_mode &= ~CWHeight;
		    GetPrefSize(w, &reply, &request);		    
		}
		else if (reply.request_mode & CWWidth &&
			 reply.width == request.width) {
		    reply.request_mode &= ~CWWidth;
		    GetPrefSize(w, &reply, &request);
		}
		else
		    request = reply;
	    }
	    else {
		GetPrefSize(w, &reply, &request);
	    }
	    request.request_mode = CWWidth | CWHeight;

	    ret_val = XtMakeGeometryRequest(w, &request, &reply);
	    if (ret_val == XtGeometryAlmost)
		ret_val = XtMakeGeometryRequest(w, &reply, NULL);
	    else
		reply = request;
	}
	else
	    reply = request;

	if (IsVert(w)) {
	    *r_minor = reply.width;
	    *r_major = reply.height;
	}
	else {
	    *r_minor = reply.height;
	    *r_major = reply.width;
	}

	return(ret_val);
    }

    *r_minor = max_minor;
    *r_major = total_major;

    return(XtGeometryYes);
}

/*	Function Name: GetPrefSize
 *	Description: Gets the preferred size of the toolbar.
 *	Arguments: w - the toolbar widget.
 *                 intended - list of constraints.
 *                 preferred - resulting preferred geom.
 *	Returns: none
 */

static void
GetPrefSize(Widget w, XtWidgetGeometry *intended, XtWidgetGeometry *preferred)
{
    XmToolbarWidget tbw = (XmToolbarWidget) w;
    Dimension major_space, minor_space, min_minor_space;

    if ((intended->request_mode & CWWidth) && 
	(intended->request_mode & CWHeight))
    {
	*preferred = *intended;
	return;
    }

    if (intended->request_mode & CWWidth) {
	if (IsVert(w)) {
	    /*
	     * This is a nasty loop, but I see no way to get this right
	     * w/o iterating, could be slow.
	     */

	    PlaceChildren(w, NULL, INFINITY, False, &min_minor_space);
	    for (major_space = 0; ; major_space++)
	    {
		PlaceChildren(w, NULL, major_space, False, &minor_space);
		if ((minor_space <= intended->width) || 
		    (minor_space <= min_minor_space))
		{
		    break;
		}
	    }
	}
	else {
	    major_space = intended->width;
	    PlaceChildren(w, NULL, major_space, False, &minor_space);
	}
	if ( XmToolbar_num_groups(tbw) > 0 )
	{
	    major_space +=
		((XmToolbar_num_groups(tbw) - 1) * XmToolbar_group_spacing(tbw));
	}
    }
    else if (intended->request_mode & CWHeight) {
	if (IsVert(w)) {
	    major_space = intended->height;
	    PlaceChildren(w, NULL, major_space, False, &minor_space);
	}
	else {
	    /*
	     * This is a nasty loop, but I see no way to get this right
	     * w/o iterating, could be slo tho.
	     */

	    PlaceChildren(w, NULL, INFINITY, False, &min_minor_space);
	    for (major_space = 0; minor_space > min_minor_space; major_space++)
	    {
		PlaceChildren(w, NULL, major_space, False, &minor_space);
		if (minor_space <= intended->height)
		    break;
	    }
	}
	if ( XmToolbar_num_groups(tbw) > 0 )
	{
	    major_space +=
		((XmToolbar_num_groups(tbw) - 1) * XmToolbar_group_spacing(tbw));
	}
    }    
    else {
	SetPrefSize(w, False, False, &major_space, &minor_space);
    }

    if (IsVert(w)) {
	preferred->width = minor_space;
	preferred->height = major_space;
    }
    else {
	preferred->width = major_space;
	preferred->height = minor_space;
    }
}


/*	Function Name: PlaceChildren
 *	Description: places all the children.
 *	Arguments: w - the toolbar widget.
 *                 ignore_child - the child to not call XtMoveWidget on.
 *                 place_kids - place the kids.
 *                 major_space - The amount of major space avaliable.
 *                 minor_used - the amount of spaced used in the minor
 *                              direction
 *	Returns: none
 */

static void
PlaceChildren(Widget w, Widget ignore_child, Dimension major_space,
	      Boolean place_kids, Dimension *minor_used)
{
    Widget child;
    XmToolbarWidget tbw = (XmToolbarWidget) w;
    int i, j, placed = 0;
    Dimension	sep, needed;
    Position minor_offset;
    Dimension major_used = 0, child_space = 0;
    Dimension minor_margin, major_margin;

    if (IsVert(w)) {
	minor_margin = XmToolbar_h_margin(tbw);
	major_margin = XmToolbar_v_margin(tbw);
    }
    else {
	minor_margin = XmToolbar_v_margin(tbw);
	major_margin = XmToolbar_h_margin(tbw);
    }
    
    minor_offset = minor_margin;
    sep = XmToolbar_group_spacing(tbw);
    
    /*
     * Iterate through each group and the widgets in that group
     */
    for ( i = 0; i < XmToolbar_num_groups(tbw); i++ )
    {
	j = 0;
	while ( j < (int)XmToolbar_GroupMemCount(tbw, i) )
	{
	    child = XmToolbar_GroupMembers(tbw, i)[j];
	    
	    if (!XtIsManaged(child) && !child->core.being_destroyed) {
	      j++;
	      continue;
	    }
	    
	    if (IsVert(w)) 
		child_space = child->core.height;
	    else
		child_space = child->core.width;
	    child_space += 2 * child->core.border_width;

	    needed = major_used + child_space + major_margin;
	    if ((needed <= major_space) || (placed == 0))
	    {
		Dimension child_minor_size;
		Position minor_loc;

		if (IsVert(w))
		    child_minor_size = child->core.width;	
		else 
		    child_minor_size = child->core.height;
		child_minor_size += 2 * child->core.border_width;
		
		minor_loc = (Position) (XmToolbar_minor_col_space(tbw) - 
					child_minor_size) / 2 + minor_offset;

		if (place_kids) {
		    if (child != ignore_child) {
			if (IsVert(w)) {
			    _XmMoveWidget(child, (Position) minor_loc,
				  (Position) (major_used + major_margin));
			}
			else {
			    _XmMoveWidget(child,
				  (Position) (major_used + major_margin),
				  (Position) minor_loc);
			}
		    }
		    else {
			if (IsVert(w)) {
			    child->core.x = (Position) minor_loc;
			    child->core.y = (Position) (major_used +
							major_margin);
			}
			else {
			    child->core.x = (Position) (major_used + 
							major_margin);
			    child->core.y = (Position) minor_loc;
			}
		    }
		}
		
		major_used += child_space + major_margin;
		placed++;
		j++;
	    }
	    else {
		minor_offset += XmToolbar_minor_col_space(tbw) + minor_margin;
		major_used = 0;
		placed = 0;
	    }
	}
	major_used += sep;
    }
    
    if (minor_used == NULL)
	return;

    *minor_used = minor_offset;

    if (placed != 0)
	*minor_used += XmToolbar_minor_col_space(tbw) + minor_margin;

    *minor_used +=  minor_margin;
}

/*
 * Function:
 *      InsertInGroupList(tbw, w)
 * Description:
 *      Places the child in the correct group list for placement operations
 * Input:
 * 	tbw	- XmToolbarWidget: the toolbar
 *      w	- Widget	: the child to add
 * Output:
 *      None.
 */
static void
InsertInGroupList(XmToolbarWidget tbw, Widget w)
{
    int		i, groupNdx, new_pos;
    Boolean	found = False;
    
    /*
     * See if this is a new group.
     */
    for ( i = 0; i < (int)XmToolbar_num_groups(tbw); i++ )
    {
	if ( XmToolbarC_group(w) == XmToolbar_group_lists(tbw)[i]->group_id )
	{
	    groupNdx = i;
	    found = True;
	    break;
	}
	found = False;
    }

    if ( !found )
    {
	/*
	 * We need to allocate a new group list for this group.
	 */
      XmToolbar_num_groups(tbw)++;

      XmToolbar_group_lists(tbw) = 
	(ToolbarGroup **)XtRealloc((char *)XmToolbar_group_lists(tbw),
				   XmToolbar_num_groups(tbw) * sizeof(ToolbarGroup *));
      
      XmToolbar_group_lists(tbw)[XmToolbar_num_groups(tbw) - 1] = 
	(ToolbarGroup *)XtMalloc( sizeof(ToolbarGroup));
      
      XmToolbar_group_lists(tbw)[XmToolbar_num_groups(tbw) - 1]->group_id = XmToolbarC_group(w);
      XmToolbar_group_lists(tbw)[XmToolbar_num_groups(tbw) - 1]->members = NULL;
      XmToolbar_group_lists(tbw)[XmToolbar_num_groups(tbw) - 1]->num_members = 0;
      
      qsort(XmToolbar_group_lists(tbw), XmToolbar_num_groups(tbw), sizeof(ToolbarGroup *),
	    (__compar_fn_t)CompareGroupIds);
      
      groupNdx = 0;
      for ( i = 0; i < XmToolbar_num_groups(tbw); i++ )
	{
	  if ( XmToolbarC_group(w) == XmToolbar_group_lists(tbw)[i]->group_id )
	    {
	      groupNdx = i;
	      break;
	    }
	}
    }
    
    XmToolbar_group_lists(tbw)[groupNdx]->members =
	(WidgetList)XtRealloc((char *)XmToolbar_GroupMembers(tbw, groupNdx),
			(XmToolbar_GroupMemCount(tbw, groupNdx) + 1) * sizeof(Widget));
    XmToolbar_GroupMembers(tbw, groupNdx)[XmToolbar_GroupMemCount(tbw, groupNdx)] = w;
    XmToolbar_GroupMemCount(tbw, groupNdx)++;

   /* First, make sure the position is unoccupied. If it is, give 
    * the child the  available empty position nearest to what was
    * requested
    */

    if (XmToolbarPosOccupied(tbw, groupNdx, w, &new_pos))
      XmToolbarC_position(w) = new_pos;

    qsort(XmToolbar_GroupMembers(tbw, groupNdx), XmToolbar_GroupMemCount(tbw, groupNdx),
	  sizeof(Widget), (__compar_fn_t)CompareGroupPositions);
}

/*
 * Function:
 *      CompareGroupIds(e1, e2)
 * Description:
 *      Compare group ids.
 * Input:
 *      e1, e2	- ToolbarGroup **	: the groups to compare
 * Output:
 *      > 0	- e1 > e2
 *	= 0	- e1 == e2
 *	< 0 	- e1 < e2
 */
static int
CompareGroupIds(void *e1, void *e2)
{
    ToolbarGroup	**g1 = (ToolbarGroup **)e1;
    ToolbarGroup	**g2 = (ToolbarGroup **)e2;

    if ( (*g1)->group_id < (*g2)->group_id ) return(-1);
    if ( (*g1)->group_id > (*g2)->group_id ) return(1);

    return(-1);
}

/*
 * Function:
 *      CompareGroupPositions(e1, e2)
 * Description:
 *      Compare group positions.
 * Input:
 *      e1, e2	- ToolbarGroup *	: the groups to compare
 * Output:
 *      > 0	- e1 > e2
 *	< 0 	- e1 < e2
 */
static int
CompareGroupPositions(void *e1, void *e2)
{
    Widget	*w1 = (Widget *)e1;
    Widget	*w2 = (Widget *)e2;

    if ( XmToolbarC_position(*w1) < XmToolbarC_position(*w2) )
	return(-1);

    if ( XmToolbarC_position(*w1) > XmToolbarC_position(*w2) ) return(1);

    return(1);
}


/*
 * Function:
 *      ResetGroups(tbw)
 * Description:
 *      Reset the groups lists.
 * Input:
 *      tbw	- XmToolbarWidget	: the toolbar.
 *	ignore	- Widget		: a child not to insert.
 * Output:
 *      None.
 */
static void
ResetGroups(XmToolbarWidget tbw, Widget ignore)
{
    int		i,j,child_num;

    /*
     * First remove all the old group lists.
     */
    for ( i = 0; i < XmToolbar_num_groups(tbw); i++ )
    {
	if ( XmToolbar_GroupMemCount(tbw, i) > 0 )
	{
	    XtFree((char *)XmToolbar_GroupMembers(tbw, i));
	}
	XtFree((char *)XmToolbar_group_lists(tbw)[i]);
    }
    XtFree((char *)XmToolbar_group_lists(tbw));

    XmToolbar_num_groups(tbw) = 0;
    XmToolbar_group_lists(tbw) = NULL;
    
    /*
     * Now Re-insert each child
     */
    for ( i = 0; i < tbw->composite.num_children; i++ )
    {
	if ( (tbw->composite.children)[i] == ignore ) continue;
	InsertInGroupList(tbw, (tbw->composite.children)[i]);
    }
    /* reorder the child list so it matches the actual visual order */

    child_num = 0;
    for ( i = 0; i < XmToolbar_num_groups(tbw); i++ ) {
      for (j = 0; j < (int)XmToolbar_GroupMemCount(tbw, i); j++)
	{
	  tbw->composite.children[child_num] = XmToolbar_GroupMembers(tbw, i)[j];
	  child_num++;
	}
    }

}

/*
 * Function:
 *      AddPopupEventHandlers(tbw, w)
 * Description:
 *      Add event handlers for EnterNotify LeaveNotify, and MotionNotify
 *	events on a child of the toolbar.
 * Input:
 *	w	- Widget		: the widget to which to add the
 *					  handlers.
 * Output:
 *      None.
 */
static void
AddPopupEventHandlers(Widget w)
{
    XtAddEventHandler(w, EnterWindowMask, False, AddPopupTimer, NULL);
    XtAddEventHandler(w, LeaveWindowMask, False, LeaveToolbarChild, NULL);
    XtAddEventHandler(w, KeyPressMask, False, PopdownLabelShell, NULL);
    XtAddEventHandler(w, ButtonPressMask, False, PopdownLabelShell, NULL);
}

/*
 * Function:
 *      PopupLabelTimerEvent(client, timer_id)
 * Description:
 *      Called when the timer has expired for popping up the label string
 *	of a toolbar child.
 * Input:
 *      client	- XtPointer	: the widget in question.
 *	timer_id- XtIntervalId *: the timer id.
 * Output:
 *      None.
 */
static void
PopupLabelTimerEvent(XtPointer client, XtIntervalId *timer_id)
{
    Widget		w = (Widget)client;
    XmToolbarWidget	tbw = (XmToolbarWidget)XtParent(w);

    if ( w->core.being_destroyed ) return;
    
    if ( XmToolbar_popup_enabled(tbw) && XmToolbarC_entry_label(w) != NULL )
    {
	XmToolbar_popup_delay(tbw) = 0;
	if ( XmToolbar_popped_up(tbw) )
	{
	    PopdownLabel(XtParent(w));
	    XmToolbar_popped_up(tbw) = False;
	}
	PopupLabel(XtParent(w), w);
	XmToolbar_popped_up(tbw) = True;
    }
    XtRemoveTimeOut(*timer_id);
    XmToolbar_popup_timer(tbw) = (XtIntervalId)NULL;
}

/*
 * Function:
 *       LeaveToolbarChild
 * Description:
 *      Called when the pointer leaves a Toolbar child completely. Restores
 *      popup_delay value to its original value. 
 * Input:
 *      Widget w - toolbar child
 *      XtPointer client - passed client data, don't care
 *      XEvent *event - this event
 *      Boolean *go_on 
 * Output:
 *      None.
 */
static void
LeaveToolbarChild(Widget w, XtPointer client,
		  XEvent *event, Boolean *go_on)
{
  XmToolbarWidget tbw = (XmToolbarWidget) XtParent(w);
  XLeaveWindowEvent *leave_event = (XLeaveWindowEvent *) event;

  /* No need to popup child label shell now... */
  RemovePopupTimer(w, client, event, go_on);

  if (leave_event->detail != NotifyInferior) {
    if (XmToolbar_popped_up(tbw)) {
	PopdownLabel(XtParent(w));
	XmToolbar_popped_up(tbw) = False;
    }
    XmToolbar_popup_delay(tbw) = XmToolbar_popup_delay_restore(tbw);
  }
}

/*
 * Function:
 *       AddPopupTimer
 * Description:
 *      Adds a timer with popup_delay, and timer event PopupLabelTimerEvent
 * Input:
 *      Widget w - toolbar
 *      XtPointer client - passed client data, don't care since it's NULL 
 *      XEvent *event - this event
 *      Boolean *go_on 
 * Output:
 *      None.
 */
/* ARGSUSED */
static void
AddPopupTimer(Widget w, XtPointer client, XEvent *event, Boolean *go_on)
{
    XmToolbarWidget	tbw = (XmToolbarWidget)XtParent(w);
    XmToolbarCallbackStruct enter_info;

    /*
     * PWC: Defend against possibly getting called twice in a row...
     * This check for a NULL popup_timer fixes the problem of an
     * erroneous second popup timer being added...
     */
    if (XmToolbar_popup_timer(tbw) != (XtIntervalId) NULL)
	return;
 
    XmToolbar_popup_timer(tbw) = 
      XtAppAddTimeOut(XtWidgetToApplicationContext(w),
		      XmToolbar_popup_delay(tbw),
		      PopupLabelTimerEvent,
		      (XtPointer)w);

    if (XmToolbar_enter_child_callback(tbw)){
      enter_info.reason = XmCR_ENTER_CHILD;
      enter_info.event = event;
      enter_info.toolbar_item = w;
      enter_info.position = XmToolbarC_position(w);
      enter_info.group = XmToolbarC_group(w);
      XtCallCallbackList(XtParent(w), XmToolbar_enter_child_callback(tbw),
			 (XtPointer)&enter_info);
    }
}

/*
 * Function:
 *      RemovePopupTimer
 * Description:
 *      Removes the timer from the toolbar item
 * Input:
 *      Widget w - toolbar
 *      XtPointer client - passed client data, don't care since it's NULL 
 *      XEvent *event - this event
 *      Boolean *go_on 
 * Output:
 *      None.
 */
/* ARGSUSED */
static void
RemovePopupTimer(Widget w, XtPointer client, XEvent *event, Boolean *go_on)
{
    XmToolbarWidget	tbw = (XmToolbarWidget)XtParent(w);
    XmToolbarCallbackStruct enter_info;

    if ( XmToolbar_popup_timer(tbw) != (XtIntervalId)NULL )
    {
	XtRemoveTimeOut(XmToolbar_popup_timer(tbw));
    }
    XmToolbar_popup_timer(tbw) = (XtIntervalId)NULL;

    if (XmToolbar_leave_child_callback(tbw)){
      enter_info.reason = XmCR_LEAVE_CHILD;
      enter_info.event = event;
      enter_info.toolbar_item = w;
      enter_info.position = XmToolbarC_position(w);
      enter_info.group = XmToolbarC_group(w);
      XtCallCallbackList(XtParent(w),XmToolbar_leave_child_callback(tbw), 
			 (XtPointer)&enter_info);
    }
}

/*
 * XmRCallProc routine for checking toolbar.font before setting it to NULL
 * If "popup_check_set_render_table" is True, then function has 
 * been called twice on same widget, thus resource needs to be set NULL, 
 * otherwise leave it alone.
 */

/*ARGSUSED*/
static void 
PopupCheckSetRenderTable(Widget wid,
		    int offset,
		    XrmValue *value)
{
  XmToolbarWidget lw = (XmToolbarWidget)wid;

  /* Check if been here before */
  if (lw->toolbar.popup_check_set_render_table)
      value->addr = NULL;
  else {
      lw->toolbar.popup_check_set_render_table = True;
      value->addr = (char*)&(lw->toolbar.popup_fontlist);
  }

}

/*
 * Function:
 *      PopdownLabelShell
 * Description:
 *      Pops down the label shell under the toolbar item if there is one
 *      popped up
 * Input:
 *      Widget w - toolbar
 *      XtPointer client - passed client data, don't care since it's NULL 
 *      XEvent *event - this event
 *      Boolean *go_on 
 * Output:
 *      None.
 */
/* ARGSUSED */
static void
PopdownLabelShell(Widget w, XtPointer client, XEvent *event, Boolean *go_on)
{
    XmToolbarWidget	tbw = (XmToolbarWidget)XtParent(w);

    /* PWC - adding this to remove chance delayed popup of label shell... */
    if ( XmToolbar_popup_timer(tbw) != (XtIntervalId)NULL )
    {
	XtRemoveTimeOut(XmToolbar_popup_timer(tbw));
    }
    XmToolbar_popup_timer(tbw) = (XtIntervalId)NULL;

    if ( XmToolbar_popup_shell(tbw) != NULL )
    {
	PopdownLabel(XtParent(w));
    }
    
}

/*
 * Function: PopupLabel(tbw, w)
 * Description: Pops up the label under widget w
 * Input: tbw - toolbar widget parent
 *        w - toolbar item under which the label will appear
 * Returns: none
 *      
 */
static void
PopupLabel(Widget toolbar_w, Widget w)
{
    Arg			args[5];
    Cardinal		ac;
    Position		rootX, rootY, x, y;
    Dimension		height, width;
    int                 scr_width, scr_height, shell_width, shell_height;
    XmToolbarWidget tbw = (XmToolbarWidget)toolbar_w;

    if ( !(XmToolbar_popup_enabled(tbw)) ) return;

    if ( XmToolbar_popup_shell(tbw) == NULL )
    {
	/*
	 * Need to create the shell and label.
	 */
	ac = 0;
	XtSetArg(args[ac], XmNallowShellResize, True); ac++;
	XtSetArg(args[ac], XmNancestorSensitive, True); ac++;
	XmToolbar_popup_shell(tbw) =
	    XtCreatePopupShell("toolbarPopupShell", overrideShellWidgetClass,
			       (Widget)tbw, args, ac);

	ac = 0;
	XtSetArg(args[ac], XmNrecomputeSize, True); ac++;
	XmToolbar_popup_label(tbw) =
	    XtCreateManagedWidget("toolbarPopupLabel", xmLabelWidgetClass,
				  XmToolbar_popup_shell(tbw), args, ac);

    }

    /*
     * Determine some basic offsets
     */
    XtTranslateCoords(w, 0, 0, &rootX, &rootY);
    
    ac = 0;
    XtSetArg(args[ac], XmNbackground, XmToolbar_popup_background(tbw)); ac++;
    XtSetArg(args[ac], XmNforeground, XmToolbar_popup_foreground(tbw)); ac++;
    XtSetArg(args[ac], XmNfontList, XmToolbar_popup_fontlist(tbw)); ac++;
    XtSetArg(args[ac], XmNlabelString, XmToolbarC_entry_label(w)); ac++;
    XtSetValues(XmToolbar_popup_label(tbw), args, ac);

    ac = 0;
    XtSetArg(args[ac], XmNwidth, &width); ac++;
    XtSetArg(args[ac], XmNheight, &height); ac++;
    XtGetValues(w, args, ac);

    x = rootX + (width/2);
    y = rootY + height;

    /*
     * Now we do a little work here to make sure that the popup is always
     * visible. (i.e. so we don't place it so it is hangin off the visible
     * area of the screen.	
     */
    scr_width = WidthOfScreen(XtScreen(w));
    scr_height = HeightOfScreen(XtScreen(w));

    shell_width = XtWidth(XmToolbar_popup_shell(tbw));
    shell_height = XtHeight(XmToolbar_popup_shell(tbw));
    
    if( y + shell_height > scr_height )
    {
	y = rootY - shell_height;
    }
    if( y < 0 ) y = 0;
    if( x + shell_width > scr_width )
    {
	x = scr_width - ((int) shell_width);
    }
    if( x < 0 ) x = 0;

    ac = 0;
    XtSetArg(args[ac], XmNx, x); ac++;
    XtSetArg(args[ac], XmNy, y); ac++;
    XtSetValues(XmToolbar_popup_shell(tbw), args, ac);
    
    XtPopup(XmToolbar_popup_shell(tbw), XtGrabNone);
    XmToolbar_popped_up(tbw) = True;

}

/*
 * Function: PopdownLabel(w)
 * Description: Pops down the popup shell
 * Input: w - toolbar widget
 * Output: none
 *      
 */
static void
PopdownLabel( Widget w )
{
  XmToolbarWidget tbw = (XmToolbarWidget)w;

  if ( XmToolbar_popup_enabled(tbw) && XmToolbar_popup_shell(tbw) != NULL )
    {
      XtPopdown(XmToolbar_popup_shell(tbw));
      
    }
  XmToolbar_popped_up(tbw) = False;
 
}

static int
ValidGroup( XmToolbarWidget tbw, unsigned char test_group )
{
  int group;

  for ( group = 0; group < XmToolbar_num_groups(tbw); group++ ) {
    if (test_group == XmToolbar_group_lists(tbw)[group]->group_id){
      return(group);
      break;
    }
  }
  return(-1);
}


/************************************************************
 *
 * Public Functions.
 *
 ************************************************************/

/*	Function Name: XmCreateToolbar
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The created widget.
 */

Widget
XmCreateToolbar(Widget parent, String name,
		ArgList args, Cardinal num_args)
{
    return(XtCreateWidget(name, xmToolbarWidgetClass, parent, args, num_args));
}


/*	Function Name: XmToolbarMapGroup
 *	Description: Maps all the widgets of a particular group
 *	Arguments: tbar - the parent toolbar widget
 *                 group_num - number of the group to map
 *	Returns: none
 */
void
XmToolbarMapGroup( Widget tbar, unsigned char group_num )
{
  register int j,group;
  XmToolbarWidget tbw = (XmToolbarWidget)tbar;

  if ((group = ValidGroup(tbw,group_num)) < 0)
    return;

  for (j = 0; j < (int)XmToolbar_GroupMemCount(tbw, group); j++)
    {
	XtMapWidget(XmToolbar_GroupMembers(tbw, group)[j]);
    }
}

/*	Function Name: XmToolbarUnmapGroup
 *	Description: Unmaps all the widgets of a particular group
 *	Arguments: tbar - the parent toolbar widget
 *                 group_num - number of the group to unmap
 *	Returns: none
 */
void
XmToolbarUnmapGroup( Widget tbar, unsigned char group_num )
{
  register int j,group;
  XmToolbarWidget tbw = (XmToolbarWidget)tbar;

  if ((group = ValidGroup(tbw,group_num)) < 0)
    return;

  for (j = 0; j < (int)XmToolbar_GroupMemCount(tbw, group); j++)
    {
      XtUnmapWidget(XmToolbar_GroupMembers(tbw, group)[j]);
    }
  
}

/*	Function Name: XmToolbarManageGroup
 *	Description: Manages all the widgets of a particular group
 *	Arguments: tbar - the parent toolbar widget
 *                 group_num - number of the group to manage
 *	Returns: none
 */
void
XmToolbarManageGroup( Widget tbar, unsigned char group_num )
{
  register int j,group;
  XmToolbarWidget tbw = (XmToolbarWidget)tbar;

  if ((group = ValidGroup(tbw,group_num)) < 0)
    return;

  for (j = 0; j < (int)XmToolbar_GroupMemCount(tbw, group); j++)
    {
      if (!XtIsManaged(XmToolbar_GroupMembers(tbw, group)[j]))
	XtManageChild(XmToolbar_GroupMembers(tbw, group)[j]);
    }
}

/*	Function Name: XmToolbarUnmanageGroup
 *	Description: Unmanages all the widgets of a particular group
 *	Arguments: tbar - the parent toolbar widget
 *                 group_num - number of the group to unmanage
 *	Returns: none
 */
void
XmToolbarUnmanageGroup( Widget tbar, unsigned char group_num )
{
  register int j,group;
  XmToolbarWidget tbw = (XmToolbarWidget)tbar;

  if ((group = ValidGroup(tbw,group_num)) < 0)
    return;
  for (j = 0; j < (int)XmToolbar_GroupMemCount(tbw, group); j++)
    {
      if (XtIsManaged(XmToolbar_GroupMembers(tbw, group)[j]))
	XtUnmanageChild(XmToolbar_GroupMembers(tbw, group)[j]);
    }
}

/*	Function Name: XmToolbarDestroyGroup
 *	Description: Destroys all the widgets of a particular group
 *	Arguments: tbar - the parent toolbar widget
 *                 group_num - number of the group to destroy
 *	Returns: none
 */
void
XmToolbarDestroyGroup( Widget tbar, unsigned char group_num )
{
  register int j,group;
  XmToolbarWidget tbw = (XmToolbarWidget)tbar;
  
  if ((group = ValidGroup(tbw,group_num)) < 0)
    return;

  for (j = 0; j < (int)XmToolbar_GroupMemCount(tbw, group); j++)
    XtDestroyWidget(XmToolbar_GroupMembers(tbw, group)[j]);

}

/*
 *	Function Name: XmToolbarInsertBefore
 *	Description:   Repositions a widget to be immediately before
 *                     (and in the same group as) another.
 *	Arguments:     insert        - Widget to move
 *                     insert_before - target widget
 *	Returns:       none
 */
void
XmToolbarInsertBefore( Widget insert, Widget insert_before )
{
    Arg args[32];
    int ac, i;
    XmToolbarWidget tb = (XmToolbarWidget) XtParent(insert);
    int curr_num;
    unsigned char group;
    WidgetList newgroup;

    /*
     * Sanity checks
     */
    if(XtParent(insert) != XtParent(insert_before)) return;
    if(XtClass(XtParent(insert)) != xmToolbarWidgetClass) return;

    /*
     * First put it in the same group
     */
    ac = 0;
    XtSetArg(args[ac], XmNtoolbarGroup, &group); ac++;
    XtGetValues(insert_before, args, ac);

    ac = 0;
    XtSetArg(args[ac], XmNtoolbarGroup, group); ac++;
    XtSetValues(insert, args, ac);

    newgroup = (WidgetList) XtMalloc(sizeof(Widget)
			  * (XmToolbar_group_lists(tb)[group])->num_members);

    /*
     * Now step through all the members of the group and renumber
     */
    curr_num = 0;
    for(i=0; i < (int) XmToolbar_group_lists(tb)[group]->num_members; i++)
    {
	Widget curr = (XmToolbar_group_lists(tb)[group]->members)[i];

	if(curr == insert)
	{
	    continue;
	}
	else if(curr == insert_before)
	{
	    newgroup[curr_num] = insert;
	    XmToolbarC_position(insert) = curr_num;

	    curr_num++;
	    newgroup[curr_num] = insert_before;
	    XmToolbarC_position(insert_before) = curr_num;
	}
	else
	{
	    newgroup[curr_num] = curr;
	    XmToolbarC_position(curr) = curr_num;
	}
	curr_num++;
    }
    
    XtFree((char*)XmToolbar_group_lists(tb)[group]->members);
    XmToolbar_group_lists(tb)[group]->members = newgroup;

    /*
     * Finally, place the children
     */
    Resize((Widget)tb);
}
