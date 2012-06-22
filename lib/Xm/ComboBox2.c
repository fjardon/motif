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

/************************************************************
*	INCLUDE FILES
*************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#if defined(VMS) && !defined(__alpha)
#define mbstowcs(a,b,n) XiMbstowcs(a,b,n)
#endif

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/DrawP.h>

#include <Xm/ComboBox2P.h>

#include <X11/Shell.h>
#include <Xm/ArrowB.h>
#include <Xm/Label.h>
#include <Xm/TextF.h>
#include <Xm/Text.h>
#include <Xm/List.h>

#include <Xm/ExtP.h>

/*
 * NOTE:  I use the same syntax for popup and popdown that Xt Uses. 
 *        so something is "up" if it is on the screen.  With combo
 *        boxes this can often be backwards of how the user thinks,
 *        but it is all internal stuff anyway.
 * Note: most uses are now unposted (up) (doesn't appear) and posted (down)
 *	 (does appear)
 */

/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

#define SUPERCLASS ((WidgetClass) &xmManagerClassRec)

#ifndef XmNautoTraversal
#define XmNautoTraversal "autoTraversal"
#define XmCAutoTraversal "AutoTraversal"
#endif
#ifndef XmNactivateOnFill
#define XmNactivateOnFill "activateOnFill"
#define XmCActivateOnFill "ActivateOnFill"
#endif 

/************************************************************
*	MACROS
*************************************************************/

#define XmTextFieldGetString(t) \
    ( XmIsTextField(t) ? XmTextFieldGetString(t) : XmTextGetString(t) )

#define XmTextFieldSetString(t,s) \
    ( XmIsTextField(t) ? XmTextFieldSetString(t,s) : XmTextSetString(t,s) )

#define XmTextFieldInsert(t,l,p) \
    ( XmIsTextField(t) ? XmTextFieldInsert(t,l,p) : XmTextInsert(t,l,p) )

#define XmTextFieldSetInsertionPosition(t,p) \
    ( XmIsTextField(t) ? XmTextFieldSetInsertionPosition(t,p) \
                       : XmTextSetInsertionPosition(t,p) )
    

/************************************************************
*	GLOBAL DECLARATIONS
*************************************************************/

/************************************************************
*	STATIC FUNCTION DECLARATIONS
*************************************************************/
static void TextButtonPress(Widget, XtPointer, XEvent *, Boolean *);

static char combo_translations[] =
"#override \n\
Ctrl <Key>osfDown:	XiComboListPost()\n\
Ctrl <Key>osfUp:	XiComboListUnpost()\n\
Any <Key>osfCancel:	XiComboListCancel()";

static void Resize(Widget), ChangeManaged(Widget), Destroy(Widget);
static void Initialize(Widget, Widget, ArgList, Cardinal *);
static void GetValuesHook(Widget, ArgList, Cardinal *);
static Boolean SetValues(Widget, Widget, Widget, ArgList, Cardinal *);

static XtGeometryResult GeometryManager(Widget,
					XtWidgetGeometry*, XtWidgetGeometry*);
static XtGeometryResult QueryGeometry(Widget,
				      XtWidgetGeometry *, XtWidgetGeometry *);

static void ClassInitialize();
static void ExposeMethod(Widget, XEvent*, Region);

/************************
 * Actions and callbacks.
 ************************/

static void VerifyTextField(Widget, XtPointer, XtPointer);
static void ModifyVerifyTextField(Widget, XtPointer, XtPointer);
static void ValueChangedTextField(Widget, XtPointer, XtPointer);
static void ListSelected(Widget, XtPointer, XtPointer);

static void ComboUnpost(Widget, XEvent *, String *, Cardinal *);
static void ComboPost(Widget, XEvent *, String *, Cardinal *);
static void ComboCancel(Widget, XEvent *, String *, Cardinal *);

/*********************
 * Internal Routines.
 *********************/

static XmCombinationBox2Widget FindComboBox(Widget);

static void PopdownList(Widget);
static void LoseFocusHandler(Widget, XtPointer, XEvent *, Boolean *);
static void RegisterShellHandler(Widget);
static void CreateChildren(Widget, ArgList, Cardinal);
static void CreatePopup(Widget, ArgList, Cardinal);
static void PlaceChildren(Widget, Boolean, Widget);
static void FindDesiredSize(Widget, Widget, Dimension *, Dimension *,
		   XtWidgetGeometry *, XtWidgetGeometry *, XtWidgetGeometry *);
static void GetTextAndLabelWidth(Widget, Dimension, Dimension, Dimension, 
				 Dimension, Dimension *, Dimension *);

static Boolean SetListFromText(Widget, Boolean), SetTextFromList(Widget);
static Boolean PopupList(Widget);

/************************************************************
*	STATIC DECLARATIONS
*************************************************************/

static XtActionsRec actions[] =
{
    { "XiComboListDown", ComboPost },
    { "XiComboListPost", ComboPost },
    { "XiComboListUp", ComboUnpost },
    { "XiComboListUnpost", ComboUnpost },
    { "XiComboListCancel", ComboCancel },
};

static XmPartResource resources[] = {
#define offset(field) XmPartOffset(XmCombinationBox2, field)
  {XmNhorizontalMargin, XmCMargin, XmRHorizontalDimension, sizeof(Dimension),
     offset(h_space), XmRImmediate, (XtPointer) 2},
  {XmNverticalMargin, XmCMargin, XmRVerticalDimension, sizeof(Dimension),
     offset(v_space), XmRImmediate, (XtPointer) 2},
  {XmNverify, XmCVerify, XmRBoolean, sizeof(Boolean),
     offset(verify), XmRImmediate, (XtPointer) True},
  {XmNeditable, XmCEditable, XmRBoolean, sizeof(Boolean),
     offset(editable), XmRImmediate, (XtPointer) True},
  {XmNcustomizedCombinationBox, XmCBoolean, XmRBoolean, sizeof(Boolean),
     offset(customized_combo_box), XmRImmediate, (XtPointer) False},
  {XmNverifyTextCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     offset(verify_text_callback), XmRImmediate, (XtPointer) NULL},
  {XmNupdateTextCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     offset(update_text_callback), XmRImmediate, (XtPointer) NULL},
  {XmNupdateShellCallback, XmCCallback, XmRCallback, sizeof(XtCallbackList),
     offset(update_shell_callback), XmRImmediate, (XtPointer) NULL},
  {XmNpopupShellWidget, XmCWidget, XmRWidget, sizeof(Widget),
     offset(popup_shell), XmRImmediate, (XtPointer) NULL},
  {XmNshowLabel, XmCBoolean, XmRBoolean, sizeof(Boolean),
     offset(show_label), XmRImmediate, (XtPointer) True},
  {XmNpopupOffset, XmCPopupOffset, XmRInt, sizeof(int),
     offset(popup_offset), XmRImmediate, (XtPointer) 15},
  {XmNpopupCursor, XmCCursor, XmRCursor, sizeof(Cursor),
     offset(popup_cursor), XmRString, (XtPointer) "left_ptr"},
  {XmNuseTextField, XmCUseTextField, XmRBoolean, sizeof(Boolean),
     offset(use_text_field), XmRImmediate, (XtPointer) True },
  {XmNcomboTranslations, XmCTranslations, XmRTranslationTable,
     sizeof(XtTranslations), offset(translations),
     XmRString, (XtPointer) combo_translations},
  {XmNvisibleItemCount, XmCVisibleItemCount, XmRInt, sizeof(int),
     offset(visible_items), XmRImmediate, (XtPointer) 5},
  {XmNnewVisualStyle, XmCNewVisualStyle, XmRBoolean, sizeof(Boolean),
     offset(new_visual_style), XmRImmediate, (XtPointer)True},
  {XmNshadowThickness, XmCShadowThickness, XmRHorizontalDimension,
     sizeof (Dimension), XtOffsetOf(XmManagerRec, manager.shadow_thickness),
     XmRImmediate, (XtPointer) 2},

  /* intentionally undocumented feature used by other ICS widgets */
  {XmNverifyTextFailedCallback, XmCCallback, XmRCallback,
     sizeof(XtCallbackList), offset(verify_text_failed_callback),
     XmRImmediate, (XtPointer) NULL},
#ifdef XmNautoTraversal
  {XmNautoTraversal, XmCAutoTraversal, XmRBoolean, sizeof(Boolean),
	offset(autoTraversal), XmRImmediate, (XtPointer) True},
#endif
#ifdef XmNactivateOnFill
  {XmNactivateOnFill, XmCActivateOnFill, XmRInt, sizeof(int),
	offset(activateOnFill), XmRImmediate, (XtPointer) 0},
#endif

};

static XmSyntheticResource get_resources[] =
{
    { XmNhorizontalMargin, sizeof(Dimension), offset(h_space),
	  _XmFromHorizontalPixels, (XmImportProc) _XmToHorizontalPixels
    },
    { XmNverticalMargin, sizeof(Dimension), offset(v_space),
	  _XmFromVerticalPixels, (XmImportProc) _XmToVerticalPixels
    },
};
#undef offset
 
XmCombinationBox2ClassRec xmCombinationBox2ClassRec = {
  { /* core fields */
    /* superclass		*/	SUPERCLASS,
    /* class_name		*/	"XmCombinationBox2",
    /* widget_size		*/	sizeof(XmCombinationBox2Part),
    /* class_initialize		*/	ClassInitialize,
    /* class_part_initialize	*/	NULL,
    /* class_inited		*/	FALSE,
    /* initialize		*/	Initialize,
    /* initialize_hook		*/	NULL,
    /* realize			*/	XtInheritRealize,
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
    /* expose			*/	ExposeMethod,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	XtInheritSetValuesAlmost,
    /* get_values_hook		*/	GetValuesHook,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersion,
    /* callback_private		*/	NULL,
    /* tm_table			*/	XtInheritTranslations,
    /* query_geometry		*/	(XtGeometryHandler) QueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
   {		/* composite_class fields */
    /* geometry_manager   */      GeometryManager,
    /* change_managed     */      ChangeManaged,
    /* insert_child       */      XtInheritInsertChild,			
    /* delete_child       */      XtInheritDeleteChild,			
    /* extension          */      NULL,                                     
   },
   {		/* constraint_class fields */
    /* resource list        */         NULL,
    /* num resources        */         0,	
    /* constraint size      */         0,	
    /* destroy proc         */         NULL,
    /* init proc            */         NULL,				    
    /* set values proc      */         NULL,				       
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
  { /* Combination Box fields */
      NULL                      /* extension          */
  }
};

WidgetClass xmCombinationBox2WidgetClass = 
                                    (WidgetClass)&xmCombinationBox2ClassRec;

XmOffsetPtr XmCombinationBox2_offsets;
XmOffsetPtr XmCombinationBox2C_offsets;

/************************************************************
*	STATIC CODE
*************************************************************/

#if defined(VAX) && defined(VMS)
static char * OsiStrncpy( char *s1, char *s2, int len );
static size_t
XiMbstowcs(wchar_t* wcs, char* mbs, size_t n)
{
    if (wcs)
    {
	memset((char*)wcs, 0, n);
	OsiStrncpy((char*)wcs, mbs, n);
	return((size_t) strlen((char*) wcs));
    }
    else
    {
	return (0);
    }
}

/* Function:
 *     OsiStrncpy(s1, s2, len)
 * Description:
 *     strncpy() with check for NULL strings
 * Input:
 *     s1     - char * : copy to here
 *     s2     - char * : copy from here
 *     len    - int    : bytes to copy
 * 
 * Output:
 *    s1
 */

static char *
OsiStrncpy( char *s1, char *s2, int len )
{
  if (s2 == NULL) s2 = "";
  if (s1 == NULL) return(NULL);

  return(strncpy(s1,s2,len));

}

#endif

/*	Function Name: ClassInitialize
 *	Description:   class_initialize method for XmCombinationBox2
 *	Arguments:     none
 *	Returns:       nothing
 */
static void
ClassInitialize()
{
    XmCombinationBox2ClassRec* wc = &xmCombinationBox2ClassRec;
    int i;

    XmResolveAllPartOffsets(xmCombinationBox2WidgetClass,
			    &XmCombinationBox2_offsets,
			    &XmCombinationBox2C_offsets);

    for(i=0; i<wc->manager_class.num_syn_resources; i++) {
	(wc->manager_class.syn_resources)[i].resource_offset =
	    XmGetPartOffset(wc->manager_class.syn_resources + i,
			    &XmCombinationBox2_offsets);
    }
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
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) set;
    ArgList f_args;
    Cardinal f_num_args;
    Dimension desired_height, desired_width;

#ifdef DEMO
    _XmInitialIzeConverters( req );
#endif

    XmComboBox2_list_state(cbw) = XmCombinationBox2_POSTED;
    XmComboBox2_old_text(cbw) = NULL;
    XmComboBox2_doActivate(cbw) = False;
    XmComboBox2_inValueChanged(cbw) = False;

    _XmFilterArgs(args, *num_args, xm_std_filter, &f_args, &f_num_args);

    CreateChildren(set, f_args, f_num_args);

    if (!XmComboBox2_customized_combo_box(cbw))
	CreatePopup(set, f_args, f_num_args);

    RegisterShellHandler(set);

    if (!XmComboBox2_editable(cbw)) {
      VerifyTextField(NULL, (XtPointer) cbw, (XtPointer) NULL);
      XtAddEventHandler(XmComboBox2_text(cbw), ButtonPressMask,
			False, TextButtonPress, NULL);
    }

    /*
     * Override the bad things that XmManager is doing to me.
     */
    if ((req->core.height == 0) || (req->core.width == 0))
      {
        XtWidgetGeometry arrow_geom, text_geom, label_geom;
	
        FindDesiredSize(set, NULL, &desired_width, &desired_height,
			&label_geom, &text_geom, &arrow_geom);
	
      }
    
    if (req->core.height == 0)
      set->core.height = desired_height;
    else set->core.height = req->core.height;
    
    if (req->core.width == 0)
      set->core.width =  desired_width;
    else set->core.width = req->core.width;

    XtFree((char *) f_args);
}


/*	Function Name: Resize
 *	Description:   Called when this widget has been resized.
 *	Arguments:     w - Extended List Widget to realize.
 *	Returns:       none.
 */

static void 
Resize(Widget w)
{
    if (XtIsRealized(w)) XClearWindow(XtDisplay(w), XtWindow(w));
    PlaceChildren(w, False, NULL);
}

/*ARGSUSED*/
static void
GetValuesHook(Widget w, ArgList args, Cardinal *num_args)
{
    XmCombinationBox2Widget 	cbw = (XmCombinationBox2Widget) w;
    XrmQuark			visible_quark;
    int 			i;

    visible_quark = XrmStringToQuark(XmNvisibleItemCount);

    for (i = 0; i < ((int) *num_args); i++)
    {
	if (visible_quark == XrmStringToQuark(args[i].name))
	{
	    XtVaGetValues(XmComboBox2_list(cbw), args[i].name, args[i].value, NULL);

	    break;
	}
    }
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
    Boolean                place_children = False;
    XmCombinationBox2Widget old_cbw = (XmCombinationBox2Widget) current,
                           set_cbw = (XmCombinationBox2Widget) set;
    ArgList                f_args;
    Cardinal               f_num_args;

    Boolean		   retval = False;

    /*
     * Pass filtered setvalues requests down to children.
     */
    _XmFilterArgs(args, *num_args, xm_std_filter, &f_args, &f_num_args);
    if( !XmComboBox2_customized_combo_box(set_cbw) )
    {
	/*
	 * It is possible that the new combo box does not have a list
	 * associated with it because if the old value for the
	 * resource "XmNcustomizedCombinationBox" is True.  If this is
	 * the case then we need to build a popup list for the new
	 * combo box.
	 */

	if( XmComboBox2_customized_combo_box(old_cbw) &&
	    !XmComboBox2_popup_shell(set_cbw) )
	{
	    /*
	     * It turns out that we need to create a new popup list
	     * for this widget so lets go ahead and do it.
	     */

	    CreatePopup (set, f_args, f_num_args);
	}
	XtSetValues(XmComboBox2_list(set_cbw), f_args, f_num_args);
    }
    
    /* make it Create/Get only */
    if( XmComboBox2_use_text_field(set_cbw) != XmComboBox2_use_text_field(old_cbw) )
	XmComboBox2_use_text_field(set_cbw) = XmComboBox2_use_text_field(old_cbw);

    _XmSetValuesOnChildren(set, f_args, f_num_args);

    if (XmComboBox2_editable(set_cbw) != XmComboBox2_editable(old_cbw)){
      if (!XmComboBox2_editable(set_cbw)) 
	XtAddEventHandler(XmComboBox2_text(set_cbw), ButtonPressMask, False,
			  TextButtonPress, NULL);
      else
	XtRemoveEventHandler(XmComboBox2_text(set_cbw), ButtonPressMask, False,
			     TextButtonPress, NULL);
    }

    if ((XmComboBox2_h_space(set_cbw) != XmComboBox2_h_space(old_cbw)) ||
	(XmComboBox2_v_space(set_cbw) != XmComboBox2_v_space(old_cbw))) 
    {
	place_children = True;
    }
    
    if (XmComboBox2_popup_shell(set_cbw) != XmComboBox2_popup_shell(old_cbw)) {
	if (!XmComboBox2_customized_combo_box(old_cbw)) {
	    /*
	     * Non custom box, I created it, so I should destroy it.
	     */
	    XtDestroyWidget(XmComboBox2_popup_shell(old_cbw));
	}

	RegisterShellHandler(set);
    }

    /* note! the following causes a size change in the XmCombinationBox2. It
    ** could be possible to redefine the visuals so that this isn't the case,
    ** though. As is, the effect is that the widget changes size when the 
    ** resource changes. By itself, this isn't a problem -- it changes back 
    ** when the resource is flipped the other way. But when used with a
    ** manager widget whose geometry management is the equivalent of 
    ** XmRESIZE_GROW, the XmCombinationBox2 will not shrink back down in size.
    */
    if (XmComboBox2_new_visual_style(set_cbw) != XmComboBox2_new_visual_style(old_cbw))
    {
	Arg targs[4]; 
	int tn = 0;
	if(XmComboBox2_new_visual_style(set_cbw)) 
		{
		/* ArrowB doesn't track this information itself; it draws
		** itself minus its highlightThickness
		*/
		Dimension width, height;
		tn = 0;
		XtSetArg(targs[tn], XmNwidth, &width); tn++;
		XtSetArg(targs[tn], XmNheight, &height); tn++;
		XtGetValues(XmComboBox2_arrow(set_cbw),targs,tn);
		tn = 0;
		XtSetArg(targs[tn], XmNwidth, width - 4); tn++;
		XtSetArg(targs[tn], XmNheight, height - 4); tn++;
		XtSetArg(targs[tn], XmNhighlightThickness, 0); tn++;
		XtSetValues(XmComboBox2_arrow(set_cbw),targs,tn);
		tn = 0;
		XtSetArg(targs[tn], XmNhighlightThickness, 0); tn++;
		XtSetArg(targs[tn], XmNshadowThickness, 0); tn++;
		XtSetValues(XmComboBox2_text(set_cbw),targs,tn);
		}
	else
		{
		Dimension width, height;
		tn = 0;
		XtSetArg(targs[tn], XmNwidth, &width); tn++;
		XtSetArg(targs[tn], XmNheight, &height); tn++;
		XtGetValues(XmComboBox2_arrow(set_cbw),targs,tn);
		tn = 0;
		XtSetArg(targs[tn], XmNwidth, width + 4); tn++;
		XtSetArg(targs[tn], XmNheight, height + 4); tn++;
		XtSetArg(targs[tn], XmNhighlightThickness, 2); tn++;
		XtSetValues(XmComboBox2_arrow(set_cbw),targs,tn);

		tn = 0;
		XtSetArg(targs[tn], XmNhighlightThickness, 2); tn++;
		XtSetArg(targs[tn], XmNshadowThickness, 2); tn++;
		XtSetValues(XmComboBox2_text(set_cbw),targs,tn);
		}
	retval = True;	/* for shadows */
    }

    if (XmComboBox2_show_label(set_cbw) != XmComboBox2_show_label(old_cbw))
    {
	retval = True;
	if (XmComboBox2_show_label(old_cbw))
	{
	    XtUnmanageChild(XmComboBox2_label(old_cbw));
	}
	else
	{
	    XtManageChild(XmComboBox2_label(set_cbw));
	}
    }
    XtFree((char *) f_args);

    if (place_children)
	{
	/* figure out the new geometry needed */
    	XtWidgetGeometry arrow_geom, text_geom, label_geom;
    
    	FindDesiredSize(set, NULL, &(set->core.width), &(set->core.height), 
		    &label_geom, &text_geom, &arrow_geom);
	}

    /* Fix for CR03893, not resizing once the */
    /* label width is modified                */
    Resize((Widget)set_cbw);
    
    return(retval);
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
    XtWidgetGeometry arrow_geom, text_geom, label_geom;
    
    FindDesiredSize(w, NULL, &(preferred->width), &(preferred->height), 
		    &label_geom, &text_geom, &arrow_geom);

    return(_XmHWQuery(w, intended, preferred));
}

/*	Function Name: Destroy
 *	Description: Called when the widget dies.
 *	Arguments: w - the widget.
 *	Returns: none.
 */

static void
Destroy(Widget w)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;
 
    XtFree((char *) XmComboBox2_old_text(cbw));
}

/************************************************************
 *
 * Semi - Public Routines for child management.
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
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) XtParent(w);
    Dimension c_width, c_height, old_width, old_height, r_width, r_height;
    XtGeometryResult ret_val;
    XtWidgetGeometry arrow_geom, text_geom, label_geom;
    Boolean w_req = request->request_mode & CWWidth;
    Boolean h_req = request->request_mode & CWHeight;

    if (!(w_req || h_req)) return(XtGeometryNo);
    
    old_width = w->core.width;
    old_height = w->core.height;

    if (w_req)
	w->core.width = request->width;

    if (h_req)
	w->core.height = request->height;

    FindDesiredSize((Widget) cbw, w, &c_width, &c_height, 
		    &label_geom, &text_geom, &arrow_geom);

    _XmRequestNewSize((Widget) cbw, True, 
       c_width, c_height, &r_width, &r_height);

    if (c_height == r_height) {
	Dimension arrow_width, text_width, label_width, tbw, lbw;

	result->height = w->core.height;

	if (XmComboBox2_show_label(cbw)) {
	    label_width = label_geom.width;
	    lbw = label_geom.border_width;
	}
	else {
	    label_width = 0;
	    lbw = 0;
	}	    

	text_width = text_geom.width;
	tbw = text_geom.border_width;

	arrow_width = arrow_geom.width + 2 * arrow_geom.border_width;

	GetTextAndLabelWidth((Widget) cbw, r_width, arrow_width, lbw, tbw,
			     &text_width, &label_width);

	if ((text_width != 1) && 
	    ((label_width != 1) || XmComboBox2_show_label(cbw))) 
	{
	    if (w == XmComboBox2_text(cbw)) 
		result->width = text_width;
	    else if (w == XmComboBox2_label(cbw)) 
		result->width = label_width;
	    else
		result->width = w->core.width;
	}
	else {
	    result->width = old_width;
	}
	result->request_mode = CWWidth | CWHeight;
    }

    if ((!h_req || (result->height == request->height)) &&
	(!w_req || (result->width == request->width))) 
    {
	if (request->request_mode & XtCWQueryOnly)
	    ret_val = XtGeometryYes;
	else {
	    /* PlaceChildren((Widget) cbw, True, w); */
	    _XmRequestNewSize((Widget) cbw, False, 
		      c_width, c_height, &r_width, &r_height);
	    return(XtGeometryYes);
	}
    }
    else if ((!h_req || (result->height == request->height)) ||
	     (!w_req || (result->width == request->width))) 
    {	
	ret_val = XtGeometryAlmost;
    }
    else
        ret_val = XtGeometryNo;

    w->core.height = old_height;
    w->core.width = old_width;

    return(ret_val);
}

/*	Function Name: ChangeManaged
 *	Description:   When a management change has occured...
 *	Arguments:     w - the combination box widget.
 *	Returns:       none.
 */

static void
ChangeManaged(Widget w)
{
    if(XtIsRealized(w))
	PlaceChildren(w, True, NULL);
    else
	PlaceChildren(w, False, NULL);
	
    _XmNavigChangeManaged(w);	/* for Motif navigation */
}

/*
 *      Function Name: ExposeMethod
 *      Description:   Handles redisplay
 *      Arguments:     w - the combobox
 *                     event - The expose event in question
 *                     r - The region to be exposed
 */
/*ARGSUSED*/
static void
ExposeMethod(Widget wid, XEvent *event, Region r)
{
    Dimension x, y, w, h;
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget)wid;
    Widget text  = XmComboBox2_text(cbw);
    Widget arrow = XmComboBox2_arrow(cbw);

    if(!XmComboBox2_new_visual_style(cbw))
	return;

    x = text->core.x - text->core.border_width -
	cbw->manager.shadow_thickness;

    y = text->core.y - text->core.border_width -
	cbw->manager.shadow_thickness;

    w = text->core.width + arrow->core.width +
	2*(text->core.border_width + arrow->core.border_width) +
	2*cbw->manager.shadow_thickness;

    h = text->core.height + 2*text->core.border_width +
	2*cbw->manager.shadow_thickness;

    _XmDrawShadows(XtDisplay(cbw), XtWindow(cbw),
		   cbw->manager.top_shadow_GC,
		   cbw->manager.bottom_shadow_GC,
		   x, y, w, h,
		   cbw->manager.shadow_thickness,
		   XmSHADOW_IN);
		   
}

/************************************************************
 *
 * Actions and Callbacks.
 *
 ************************************************************/

/*	Function Name: ArrowClicked
 *	Description:   This is called when the arrow button is selected.
 *	Arguments:     w - the arrow button widget.
 *                     combo_ptr - the combination box pointer.
 *                     info_ptr - a pointer to the arrow button info.
 *	Returns:       none.
 */

/* ARGSUSED */
static void
ArrowClicked(Widget w, XtPointer combo_ptr, XtPointer info_ptr)
{
    XmCombinationBox2Widget      cbw = (XmCombinationBox2Widget) combo_ptr;
    Arg                         args[10];
    Cardinal                    num_args;
    Boolean                     is_unposted, success = True;
    XmAnyCallbackStruct         cbdata;
    XmArrowButtonCallbackStruct *arrow = (XmArrowButtonCallbackStruct*)
	info_ptr;

    /*
     * Do Nothing... 
     */

    if (XmComboBox2_list_state(cbw) == XmCombinationBox2_IN_PROGRESS)
	return;

    /*
     * DANGER:  Do not return early from this function w/o setting
     *          XmComboBox2_list_state(cbw) back to either XmCombinationBox2_UNPOSTED or XmCombinationBox2_POSTED or
     *          the combo box will never be able to pop up or down its
     *          list.
     * 
     *          You have been warned!             CP 9/8/93.
     */

    if (XmComboBox2_list_state(cbw) == XmCombinationBox2_UNPOSTED)
	is_unposted = True;
    else /*  (XmComboBox2_list_state(cbw) == XmCombinationBox2_POSTED) */
	is_unposted = False;

    XmComboBox2_list_state(cbw) = XmCombinationBox2_IN_PROGRESS;

    if (is_unposted) {
	PopdownList((Widget) cbw);

	if (!XmComboBox2_customized_combo_box(cbw))	
	    (void) SetTextFromList((Widget) cbw);

	cbdata.reason = XiCR_UPDATE_TEXT;
	cbdata.event = (arrow == NULL ? NULL : arrow->event);
	XtCallCallbackList((Widget) cbw, XmComboBox2_update_text_callback(cbw),
			   (XtPointer) &cbdata);
    }
    else {
	if (success = PopupList((Widget) cbw))
	{
	    cbdata.reason = XiCR_UPDATE_SHELL;
	    cbdata.event = (arrow == NULL ? NULL : arrow->event);
	    XtCallCallbackList((Widget) cbw, XmComboBox2_update_shell_callback(cbw),
			       (XtPointer) &cbdata);
	    
	    /*
	     * Save the old text in case the user cancels.
	     */
	    
	    XtFree((char *) XmComboBox2_old_text(cbw));
	    XmComboBox2_old_text(cbw) = XmTextFieldGetString(XmComboBox2_text(cbw));
	    
	    if (!XmComboBox2_customized_combo_box(cbw) &&
		!SetListFromText((Widget) cbw, False) &&
		XmComboBox2_verify(cbw)) 
	    {
		XmTextFieldSetString(XmComboBox2_text(cbw), "");
	    }
	}
    }

    /*
     * 'success' will only be False if Popuplist failed.
     */

    if (success)
    {
	num_args = 0;
	XtSetArg(args[num_args], XmNarrowDirection, 
		 is_unposted ? XmARROW_DOWN : XmARROW_UP); num_args++;
	XtSetValues(w, args, num_args);
    }

    /*
     * If we were up, we are now down, and visa-versa.
     */

    if (is_unposted)
	XmComboBox2_list_state(cbw) = XmCombinationBox2_POSTED;
    else
	XmComboBox2_list_state(cbw) = XmCombinationBox2_UNPOSTED;
}

/*	Function Name: CheckExtensions
 *	Description:   Verifies that the extension is of the correct
 *                     format, with correct version number and 
 *                     record type
 *	Arguments:     combo - XmCombinationBox2WidgetClass
 *	Returns:       returns a valid extension as a 
 *                     XmCombinationBox2ClassPartExtension pointer
 */

static XmCombinationBox2ClassPartExtension *
CheckExtensions( XmCombinationBox2WidgetClass combo )
{
  XmCombinationBox2ClassPartExtension *ret_extension=NULL, *extension;

  extension=
    (XmCombinationBox2ClassPartExtension *)(combo->combo_class.extension);

  while ((ret_extension == NULL) && (extension != NULL)){
    if ((extension->record_type == NULLQUARK) && 
	(extension->version == XmCombinationBox2ExtensionVersion)) {
      ret_extension = (XmCombinationBox2ClassPartExtension *)extension;
    }
    extension=(XmCombinationBox2ClassPartExtension *)extension->next_extension;
  }

  return( ret_extension );

}

  
/*	Function Name: IsTextOK
 *	Description:   This is called to verify the text field.
 *	Arguments:     w - the text field widget.
 *                     combo_ptr - the combination box pointer.
 * UNUSED              info_ptr - a pointer to the arrow button info.
 *	Returns:       none.
 */

/*ARGSUSED*/
static Boolean
IsTextOK(XmCombinationBox2Widget cbw)
{
	Boolean isOK = True;
	XmCombinationBox2ClassPartExtension *addition;

	addition = CheckExtensions( (XmCombinationBox2WidgetClass)XtClass(cbw) );
	if (addition && addition->verify) {
		char *text = XmTextFieldGetString(XmComboBox2_text(cbw));
		/* let the subclass verify the string with the application */
		isOK = (*addition->verify)((Widget)cbw, text);
		XtFree(text);
	} else {
		isOK = SetListFromText((Widget) cbw, True);
	}
	return isOK;
}

/*	Function Name: VerifyTextField
 *	Description:   This is called to verify the text field.
 *	Arguments:     w - the text field widget.
 *                     combo_ptr - the combination box pointer.
 * UNUSED              info_ptr - a pointer to the arrow button info.
 *	Returns:       none.
 */

/*ARGSUSED*/
static void
VerifyTextField(Widget w, XtPointer combo_ptr, XtPointer info_ptr)
{
  XmCombinationBox2Widget     cbw = (XmCombinationBox2Widget) combo_ptr;
  XmTextVerifyCallbackStruct *field = (XmTextVerifyCallbackStruct*) info_ptr;
  XmAnyCallbackStruct        cbdata;
  Boolean		     allowTraverse = True;

  if (!XmComboBox2_customized_combo_box(cbw))
  {
      if (XmComboBox2_verify(cbw) && !IsTextOK(cbw)) 
      {
	  /*
	   * Check to see if the extension is there
	   */
	  XmCombinationBox2ClassPartExtension *addition;
	  
	  addition =
	      CheckExtensions((XmCombinationBox2WidgetClass)XtClass(cbw));
	  
	  cbdata.reason = XiCR_VERIFY_TEXT_FAILED;
	  cbdata.event = (field == NULL ? NULL : field->event);
	  
	  XtCallCallbackList((Widget)cbw,
			     XmComboBox2_verify_text_failed_callback(cbw),
			     (XtPointer) &cbdata);
	  
	  /* otherwise we handle the error; again must let the subclass 
	  ** know the string has been updated -- in response to the user's
	  ** input although not to that value -- to some other value
	  */
	  if (!XmComboBox2_editable(cbw)) {
	      /* it is not clear how this case can be reached but fill it out
	      ** for completeness
	      */
	      XmListSelectPos(XmComboBox2_list(cbw), 1, False);
	      SetTextFromList((Widget) cbw);
	      if (addition && addition->update) 
	      {
		  char *text = XmTextFieldGetString(XmComboBox2_text(cbw));
		  (void)(*addition->update)((Widget)cbw,text);
		  XtFree(text);
	      }
	  }
	  else
	  {
	      XmTextFieldSetString(XmComboBox2_text(cbw), "");
	      if (addition && addition->update)
		  (void)(*addition->update)((Widget)cbw,"");
	  }
      }
      /* else blindly accept the value */
      else
	  {
	      XmCombinationBox2ClassPartExtension *addition =
		  CheckExtensions( (XmCombinationBox2WidgetClass)XtClass(cbw) );
	      if (addition && addition->update) 
		  {
		      char *text = XmTextFieldGetString(XmComboBox2_text(cbw));
		      (void)(*addition->update)((Widget)cbw,text);
		      XtFree(text);
		  }
	  }
  }
  
  if (w != NULL)
  {
      cbdata.reason = XiCR_VERIFY_TEXT;
      cbdata.event = (field == NULL ? NULL : field->event);
      XtCallCallbackList((Widget) cbw,
			 XmComboBox2_verify_text_callback(cbw),
			 (XtPointer) &cbdata);
  }
  
  if(!XmComboBox2_doActivate(cbw)) {
      /* here if we were NOT called from the autofill code */
      if(field == NULL || field->reason != XmCR_ACTIVATE) {
	  allowTraverse = False;
      }
  }
  if(allowTraverse && XmComboBox2_autoTraversal(cbw)) {
      (void) XmProcessTraversal((Widget)cbw, XmTRAVERSE_NEXT_TAB_GROUP);
  }
}


/*	Function Name: ModifyVerifyTextField
 *	Description:   This is called to check if text field is filled so
 *			we can autotraverse
 *	Arguments:     w - the text field widget.
 *                     combo_ptr - the combination box pointer.
 * UNUSED              info_ptr - a pointer to the arrow button info.
 *	Returns:       none.
 */

/*ARGSUSED*/
static void
ModifyVerifyTextField(Widget w, XtPointer combo_ptr, XtPointer info_ptr)
{
    XmCombinationBox2Widget     cbw = (XmCombinationBox2Widget) combo_ptr;
    XmTextVerifyCallbackStruct *field = (XmTextVerifyCallbackStruct*) info_ptr;

    if (XmComboBox2_activateOnFill(cbw) <= 0) return;
    
    if(field == NULL || field->event == NULL ||
       field->event->type != KeyPress) return;
    
    /* printf("Text m/v callback, cur/new/start/end = %d %d %d %d\n",
     * field->currInsert, field->newInsert, field->startPos, field->endPos);
     */

    /* No make sure we are inserting a single character at the end */
    if(field->currInsert != field->endPos) return;
    if(field->currInsert != field->startPos) return;
    if(field->text->length != 1) return;
    
    if(field->currInsert + 1 == XmComboBox2_activateOnFill(cbw)) {
	XmComboBox2_doActivate(cbw) = True;
    }
}


/*	Function Name: ValueChangedTextField
 *	Description:   Called after value is changed, checks doActivate flag
 * 			to see if we should try traversal
 *	Arguments:     w - the text field widget.
 *                     combo_ptr - the combination box pointer.
 * UNUSED              info_ptr - a pointer to the arrow button info.
 *	Returns:       none.
 */

/*ARGSUSED*/
static void
ValueChangedTextField(Widget w, XtPointer combo_ptr, XtPointer info_ptr)
{
    XmCombinationBox2Widget     cbw = (XmCombinationBox2Widget) combo_ptr;
    
    if(XmComboBox2_inValueChanged(cbw)) return;
    
    XmComboBox2_inValueChanged(cbw) = True;
    if(XmComboBox2_doActivate(cbw)) {
	VerifyTextField(NULL, (XtPointer) cbw, (XtPointer) info_ptr);
	/* We clear this AFTER the call, because it is used to
	 * check inside VerifyText if we should do an autoTraversal
	 * or not
	 */
	XmComboBox2_doActivate(cbw) = False;
    }
    XmComboBox2_inValueChanged(cbw) = False;
}

/*	Function Name: ListSelected
 *	Description:   Called when the popdown list is selected.
 *	Arguments:     w - the list widget.
 *                     cbw_ptr - pointer to the combination box.
 *                     list_data_ptr - data from the list widget.
 *	Returns:       none.
 */

/* ARGSUSED */
static void
ListSelected(Widget w, XtPointer cbw_ptr, XtPointer list_data_ptr)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) cbw_ptr;
    XmListCallbackStruct *list_data;

    list_data = (XmListCallbackStruct *) list_data_ptr;

    if ((list_data->reason == XmCR_BROWSE_SELECT) &&
	((list_data->event == NULL) ||
	 (list_data->event->xany.type != ButtonPress) &&
	 (list_data->event->xany.type != ButtonRelease)))
    {
	/* 
	 * Do not popup list is browse select mode.
	 */
	if (!XmComboBox2_customized_combo_box(cbw))	
	    (void) SetTextFromList((Widget) cbw);

	return;			
    }

    /*
     * Same thing happens as when the arrow is clicked.
     */

    ArrowClicked(XmComboBox2_arrow(cbw), (XtPointer) cbw, NULL);
}

/*	Function Name: ShellButtonEvent
 *	Description: Called when a button press is passed to the shell
 *                   from the grab that was activated.
 *	Arguments: w - the shell widget.
 *                 cbw_ptr - the combo box widget pointer.
 *                 event - the event that caused this action.
 *                 junk - *** UNUSED.
 *	Returns: none.
 *
 * NOTE: Because this is popped up spring loaded we get all events
 * 	 that are delivered, so we need to not pop this down if we clicked
 *       on the arrow (the arrow click is handled by the arrow button)
 *       or if we click in the popped up combo box widget.
 */

/* ARGSUSED */
static void
ShellButtonEvent(Widget w, XtPointer cbw_ptr, XEvent *event, Boolean *junk)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) cbw_ptr;
    Widget event_widget;

    if (event->xany.type != ButtonPress) {
	if ((event->xany.type == ButtonRelease) &&
	    !XmComboBox2_customized_combo_box(cbw))
	    {
		XtCallActionProc(XmComboBox2_list(cbw), "ListEndSelect",
				 event, NULL, 0);
	    }
	return;
    }

    event_widget = XtWindowToWidget(event->xany.display, event->xany.window);

    if (event_widget == XmComboBox2_arrow(cbw)) 
	return;
    else if ((event_widget == XmComboBox2_text(cbw)) && !XmComboBox2_editable(cbw))
    {
	TextButtonPress(event_widget, NULL, event, junk);
	return;
    }
    else {
	Widget event_shell = event_widget; 

	while (!XtIsShell(event_shell))
	    event_shell = XtParent(event_shell);

	if ( (event_shell == (Widget) XmComboBox2_popup_shell(cbw)) &&
	    (event_widget != (Widget) XmComboBox2_popup_shell(cbw))) 
	{
	    return;
	}
    }
    
   if (XmComboBox2_list_state(cbw) != XmCombinationBox2_POSTED)	/* in case this popup shell is used for more than one combobox */
    	ArrowClicked(XmComboBox2_arrow(cbw), cbw_ptr, NULL);
}

/*	Function Name: LoseFocusHandler
 *	Description: This function is called whenever the shell loses focus
 *                   in this case we should bring down the list.
 *	Arguments: w - the shell widget.
 *                 cbw_ptr - the combo box widget pointer.
 *                 event - the event that caused this action.
 *                 junk - *** UNUSED ***.
 *	Returns: none.
 */

/* ARGSUSED */
static void
LoseFocusHandler(Widget w, XtPointer cbw_ptr, XEvent *event, Boolean *junk)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) cbw_ptr;
    XFocusChangeEvent *fevent = &(event->xfocus);

    if ((event->xany.type != FocusOut) || (XmComboBox2_list_state(cbw) != XmCombinationBox2_UNPOSTED) ||
	(fevent->detail == NotifyInferior))
    {
	return;
    }

    ArrowClicked(XmComboBox2_arrow(cbw), cbw_ptr, NULL);
}

/*	Function Name: ComboUnpost
 *	Description:   Called when the user wants to remove the list from the
 *                     screen.
 *	Arguments:     w - a child widget of the combo box.
 *                     event - the event that caused this action.
 *                     params, num_params - action routine parameters.
 *	Returns:       none.
 */

/*ARGSUSED*/
static void
ComboUnpost(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
    XmCombinationBox2Widget cbw = FindComboBox(w);

    if (cbw == NULL)
	return;

    if (XmComboBox2_list_state(cbw) == XmCombinationBox2_UNPOSTED) 
	ArrowClicked(XmComboBox2_arrow(cbw), (XtPointer) cbw, NULL);
}

/*	Function Name: ComboPost
 *	Description:   Called when the user wants to show the list on the
 *                     screen.
 *	Arguments:     w - a child widget of the combo box.
 *                     event - the event that caused this action.
 *                     params, num_params - action routine parameters.
 *	Returns:       none.
 */

/*ARGSUSED*/
static void
ComboPost(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
    XmCombinationBox2Widget cbw = FindComboBox(w);

    if (cbw == NULL)
	return;

    if (XmComboBox2_list_state(cbw) != XmCombinationBox2_UNPOSTED) 
	ArrowClicked(XmComboBox2_arrow(cbw), (XtPointer) cbw, NULL);
}

/*	Function Name: ComboCancel
 *	Description:   Called when the user wants to remove the list from the
 *                     screen, but not update the text.
 *	Arguments:     w - a child widget of the combo box.
 *                     event - the event that caused this action.
 *                     params, num_params - action routine parameters.
 *	Returns:       none.
 */

/*ARGSUSED*/
static void
ComboCancel(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
    Arg args[10];
    Cardinal num_args;
    XmCombinationBox2Widget cbw = FindComboBox(w);

    if ((cbw == NULL) || XmComboBox2_list_state(cbw) != XmCombinationBox2_UNPOSTED)
	return;

    PopdownList((Widget) cbw);
    XmComboBox2_list_state(cbw) = XmCombinationBox2_POSTED; /* List is now down (not visible). */

    num_args = 0;
    XtSetArg(args[num_args], XmNarrowDirection,	XmARROW_DOWN); num_args++;
    XtSetValues(XmComboBox2_arrow(cbw), args, num_args);

    if (XmComboBox2_old_text(cbw) != NULL) {
	XmTextFieldSetString(XmComboBox2_text(cbw), XmComboBox2_old_text(cbw));
	XtFree((char *) XmComboBox2_old_text(cbw));
	XmComboBox2_old_text(cbw) = NULL;
    }
}

/************************************************************
 *
 * Internal routines.
 *
 ************************************************************/

/*	Function Name: FindComboBox
 *	Description: Finds the combo box given any one of its descendants.
 *	Arguments: w - any descendant of the combo box.
 *	Returns: the combo box widget or NULL.
 */

static XmCombinationBox2Widget
FindComboBox(Widget w)
{
    while (!XtIsSubclass(w, xmCombinationBox2WidgetClass)) {
	if ((w = XtParent(w)) == NULL)
	    break;
    }
    return((XmCombinationBox2Widget) w);
}
 
/*	Function Name: RegisterShellHandler
 *	Description: Registers an event handler on the shell.
 *	Arguments: w - the combo box widget.
 *	Returns: none.
 */

static void
RegisterShellHandler(Widget w)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;

    if (XmComboBox2_popup_shell(cbw) == NULL)
	return;

    /*
     * Don't register the translations if it's a customized shell; the
     * actions referenced won't be there!
     * This, of course, means that any actions specified in this
     * translation table won't ever get called for a customized shell...
     */
    if(!XmComboBox2_customized_combo_box(cbw)) {
	XtOverrideTranslations(XmComboBox2_popup_shell(cbw),
			       XmComboBox2_translations(cbw));
    }

    XtAddEventHandler(XmComboBox2_popup_shell(cbw), 
		      ButtonPressMask | ButtonReleaseMask, False,
		      ShellButtonEvent, (XtPointer) w);
    
    XtAddEventHandler(XmComboBox2_popup_shell(cbw), FocusChangeMask, False,
		      LoseFocusHandler, (XtPointer) cbw);
}

/*	Function Name: PlaceChildren
 *	Description:   Places and sizes the children.
 *	Arguments:     w - the combo box widget.
 *                     allow_resize - allow the combo box to attempt a resize.
 *                     child - Do not query this child for a width, just
 *                             use its default size.
 *	Returns:       none
 */
 
static void
PlaceChildren(Widget w, Boolean allow_resize, Widget child)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;
    Dimension width, height, rwidth, rheight, child_height;
    Dimension text_width, label_width, label_bw;
    Dimension shadow;
    Position arrow_x, text_x;
    XtWidgetGeometry arrow_geom, label_geom, text_geom;
    Widget label = NULL;
    Widget text = XmComboBox2_text(cbw);
    Widget arrow = XmComboBox2_arrow(cbw);

    FindDesiredSize(w, child, &width, &height, 
		    &label_geom, &text_geom, &arrow_geom);

    if (XmComboBox2_show_label(cbw)) 
    {
	label = XmComboBox2_label(cbw);
	label_width = label_geom.width;
	label_bw = label_geom.border_width;
    }
    else 
    {
	label_width = 0;
	label_bw = 0;
    }

    text_width = text_geom.width;

    if (allow_resize) 
    {
	_XmRequestNewSize(w, False, width, height, &rwidth, &rheight);
    }
    else 
    {
	rwidth = w->core.width;
	rheight = w->core.height;
    }	

    if (width != rwidth) 
    {
	Dimension arrow_width = arrow_geom.width + 2 * arrow_geom.border_width;
	GetTextAndLabelWidth(w, rwidth, arrow_width, 
			     label_bw, text_geom.border_width,
			     &text_width, &label_width);
    }

    child_height = rheight - 2 * XmComboBox2_v_space(cbw);
    arrow_x = (rwidth - (arrow_geom.width + 2 * arrow_geom.border_width) \
	       - XmComboBox2_h_space(cbw));

    text_x = label_width + 2*label_geom.border_width + \
	XmComboBox2_h_space(cbw);

    if (XmComboBox2_show_label(cbw)) 
    {
	text_x += XmComboBox2_h_space(cbw);
    }
    else
    {
	text_geom.border_width = 0;
    }

    XmComboBox2_text_x(cbw) = text_x;
    if ( XmComboBox2_new_visual_style(cbw) ) 
    {
	shadow        = cbw->manager.shadow_thickness;
	child_height -= 2 * shadow;
	arrow_x      -= shadow;
	text_x       += shadow;
    }
    else 
    {
	shadow = 0;
    }

    /*
     * Now resize the widgets
     */
    if (XmComboBox2_show_label(cbw)) 
    {
	_XmConfigureWidget(label, XmComboBox2_h_space(cbw),
			   XmComboBox2_v_space(cbw), 
			   label_width, 
			   child_height - 2 * label_geom.border_width,
			   label_geom.border_width);
    }	

    _XmConfigureWidget(arrow, arrow_x,
		       XmComboBox2_v_space(cbw) + shadow,
		       arrow_geom.width, 
		       child_height - 2 * arrow_geom.border_width,
		       arrow_geom.border_width);

    _XmConfigureWidget(text, text_x,
		       XmComboBox2_v_space(cbw) + shadow, 
		       text_width, child_height - 2 * text_geom.border_width, 
		       text_geom.border_width);

}

/*	Function Name: GetTextAndLabelWidth
 *	Description:   Finds the size for both the text and label widgets.
 *	Arguments:     w - the combo box.
 *                     combo_width - the new size of the combo box.
 *                     arrow_width - the new width of the arrow button.
 *                     lbw, tbw - the border widths of the label
 *                                and text widgets.
 *                     text_width, label_width - The desired size of the
 *      RETURNED                                 text and label is passed in,
 *                                               and the new sizes are returned
 *	Returns:       none
 */

static void
GetTextAndLabelWidth(Widget w, Dimension combo_width, Dimension arrow_width, 
		     Dimension lbw, Dimension tbw, 
		     Dimension *text_width, Dimension *label_width)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;
    int text_and_label;

    text_and_label = combo_width - (arrow_width + 2 * (lbw + tbw));
    text_and_label -= 3 * XmComboBox2_h_space(cbw);

    if (XmComboBox2_show_label(cbw)) 
    {
	text_and_label -= XmComboBox2_h_space(cbw);
    }

    if (XmComboBox2_new_visual_style(cbw)) 
    {
	text_and_label -= 2 * cbw->manager.shadow_thickness;
	text_and_label += XmComboBox2_h_space(cbw);
    }

    if (text_and_label >= (int) (*text_width + *label_width)) 
    {
	*text_width = text_and_label - *label_width;
    }
    /*
     * We need to shrink each a bit. 
     */
    else 
    {	
	if ( text_and_label < 2 ) 
	{
	    *text_width = *label_width = 1;
	    return;
	}

	*text_width = ((int)(*text_width * text_and_label)/
		       (int)(*text_width + *label_width));

	*label_width = text_and_label - *text_width;
    }

    if (*text_width < 1)
    {
	*text_width = 1;
    }

    if ((*label_width < 1) && (XmComboBox2_show_label(cbw))) 
    {
	*label_width = 1;
    }
}

/*	Function Name: FindDesiredSize
 *	Description:   Finds the desired size of the combination box.
 *	Arguments:     w - the combo box.
 *      RETURNED       width_ret, height_ret - The desired size.
 *	Returns:       none
 */
 
static void
FindDesiredSize(Widget w, Widget child, 
		Dimension * width_ret, Dimension * height_ret,
		XtWidgetGeometry * label, XtWidgetGeometry * text,
		XtWidgetGeometry * arrow)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;
    Dimension shadow;
    int label_width, text_width, arrow_width, h_space;
    
    if ( !XmComboBox2_show_label(cbw) ) 
    {
	label->width = 0;
	label->height = 0;
	label->border_width = 0;
    }	
    else if ( child != XmComboBox2_label(cbw) ) {
	(void) XtQueryGeometry(XmComboBox2_label(cbw), NULL, label);
    } 
    else
    {
	label->width = child->core.width;
	label->height = child->core.height;
	label->border_width = child->core.border_width; 
    }	
    
    if ( child != XmComboBox2_text(cbw) ) 
    {	
	(void) XtQueryGeometry(XmComboBox2_text(cbw), NULL, text);
    } 
    else 
    {
	text->width = child->core.width;
	text->height = child->core.height;
	text->border_width = child->core.border_width;
    }

    if ( child != XmComboBox2_arrow(cbw) ) 
    {
	(void) XtQueryGeometry(XmComboBox2_arrow(cbw), NULL, arrow);
    }
    else 
    {
	arrow->width = child->core.width;
	arrow->height = child->core.height;
	arrow->border_width = child->core.border_width;
    }

    label_width = label->width + 2 * label->border_width;
    text_width = text->width + 2 * text->border_width;
    arrow_width = arrow->width + 2 * arrow->border_width;
    h_space = 3 * XmComboBox2_h_space(cbw);
    *width_ret = (Dimension )(label_width + text_width \
			      + arrow_width + h_space);


    if ( XmComboBox2_show_label(cbw) )
    {
	*width_ret += XmComboBox2_h_space(cbw);
    }

    if ( XmComboBox2_new_visual_style(cbw) ) 
    {
	shadow = cbw->manager.shadow_thickness;
	*width_ret += 2 * shadow;
	*width_ret -= XmComboBox2_h_space(cbw);
    }	
    else	
    {
	shadow = 0;
    }

    *height_ret = 0;
    ASSIGN_MAX(*height_ret, label->height + 2 * label->border_width);
    ASSIGN_MAX(*height_ret, text->height + \
	       2 * text->border_width + 2 * shadow);

    /* 
     * can't use arrow sizes in the specification; 
     * it has no query_geometry procedure
     */
    /*
     * ASSIGN_MAX(*height_ret, arrow->height + 2 * arrow->border_width
     * + 2 * shadow);
     */

    *height_ret += 2 * XmComboBox2_v_space(cbw);

}

/*	Function Name: CreateChildren
 *	Description:   Creates all normal children for the Combo Box.
 *	Arguments:     w - the combo box.
 *                     args, num_args - the args to create this widget with.
 *	Returns:       none.
 *
 * NOTE(DKB:03/07/96): 
 *	This function used to only created the needed children, i.e. if
 *      show label was False the label was not created, and the popup list
 *	was never created. This was changed due to the fact that this widget
 *	uses a resources pass-through method thus if the widgets are not
 *	created at initialization all resources that should be pass-through
 *	to those widgets are lost.
 */
static void
CreateChildren(Widget w, ArgList args, Cardinal num_args)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;
    Arg largs[10];
    Cardinal num_largs;
    Arg      targs[10], *merge;
    Cardinal tn = 0;


    XmComboBox2_label(cbw) = XtCreateWidget("label", xmLabelWidgetClass, 
				      w, args, num_args);
    XtVaSetValues(XmComboBox2_label(cbw), XmNtraversalOn, False, NULL);

    /*
     * If we are supposed to show this label lets manage the widget.
     */
    if( XmComboBox2_show_label(cbw) ) 
    {
	XtManageChild(XmComboBox2_label(cbw));
    }

    if( XmComboBox2_use_text_field(cbw) )
    {
	tn = 0;
	if(XmComboBox2_new_visual_style(cbw)) 
	{
	    XtSetArg(targs[tn], XmNshadowThickness, 0); tn++;
	    XtSetArg(targs[tn], XmNhighlightThickness, 0); tn++;
	}
	else
	{
	    XtSetArg(targs[tn], XmNshadowThickness, 2); tn++;
	    XtSetArg(targs[tn], XmNhighlightThickness, 2); tn++;
	}
	merge = XtMergeArgLists(args, num_args, targs, tn);
	XmComboBox2_text(cbw) =
	    XtCreateManagedWidget("text", xmTextFieldWidgetClass,
				  w, merge, num_args+tn);
	XtFree((XtPointer)merge);
    }
    else
    {
	tn = 0;
	if(XmComboBox2_new_visual_style(cbw)) 
	{
	    XtSetArg(targs[tn], XmNshadowThickness, 0); tn++;
	    XtSetArg(targs[tn], XmNhighlightThickness, 0); tn++;
	}
	else
	{
	    XtSetArg(targs[tn], XmNshadowThickness, 2); tn++;
	    XtSetArg(targs[tn], XmNhighlightThickness, 2); tn++;
	}
	XtSetArg(targs[tn], XmNeditMode, XmSINGLE_LINE_EDIT); tn++;
	XtSetArg(targs[tn], XmNrows, 1); tn++;
	XtSetArg(targs[tn], XmNwordWrap, False); tn++;
	XtSetArg(targs[tn], XmNscrollHorizontal, False); tn++;
	XtSetArg(targs[tn], XmNscrollVertical, False); tn++;

	merge = XtMergeArgLists(args, num_args, targs, tn);

	XmComboBox2_text(cbw) = XtCreateManagedWidget("text", xmTextWidgetClass,
						w, merge, num_args + tn);
	XtFree((XtPointer)merge);
    }
    
    XtAddCallback(XmComboBox2_text(cbw), XmNlosingFocusCallback, 
		  VerifyTextField, (XtPointer) cbw);
    XtAddCallback(XmComboBox2_text(cbw), XmNactivateCallback,
		  VerifyTextField, (XtPointer) cbw);
    XtAddCallback(XmComboBox2_text(cbw), XmNmodifyVerifyCallback,
		  ModifyVerifyTextField, (XtPointer) cbw);
    XtAddCallback(XmComboBox2_text(cbw), XmNvalueChangedCallback,
		  ValueChangedTextField, (XtPointer) cbw);


    XtOverrideTranslations(XmComboBox2_text(cbw),
			   XmComboBox2_translations(cbw));

    if(XmComboBox2_new_visual_style(cbw)) 
    {
	tn = 0;
        XtSetArg(targs[tn], XmNhighlightThickness, 0); tn++;
        XtSetArg(targs[tn], XmNshadowThickness, 2); tn++;
	merge = XtMergeArgLists(args, num_args, targs, tn);
	XmComboBox2_arrow(cbw) =
	    XtCreateManagedWidget("arrow", xmArrowButtonWidgetClass,
				  w, merge, num_args+tn);
	XtFree((XtPointer)merge);
    }
    else
    {
	    tn = 0;
	    XtSetArg(targs[tn], XmNshadowThickness, 2); tn++;
	    XtSetArg(targs[tn], XmNhighlightThickness, 2); tn++;
	    XmComboBox2_arrow(cbw) =
		    XtCreateManagedWidget("arrow", xmArrowButtonWidgetClass,
				  w, args, num_args);
    }
    num_largs = 0;
    XtSetArg(largs[num_largs], XmNarrowDirection, XmARROW_DOWN); num_largs++;
    XtSetValues(XmComboBox2_arrow(cbw), largs, num_largs);

    XtOverrideTranslations(XmComboBox2_arrow(cbw),
			   XmComboBox2_translations(cbw));

    XtAddCallback(XmComboBox2_arrow(cbw), XmNactivateCallback, 
		  ArrowClicked, (XtPointer) w);
}

/*	Function Name: CreatePopup
 *	Description:   Create the popup shell that contains the list.
 *	Arguments:     w - the combo box.
 *                     args, num_args - the number of arguments in the list.
 *	Returns:       none.
 */

static void
CreatePopup(Widget w, ArgList args, Cardinal num_args)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;
    Arg *new_list, largs[10];
    Cardinal num_largs;

    num_largs = 0;
    XtSetArg(largs[num_largs], XmNoverrideRedirect, True); num_largs++;
    XtSetArg(largs[num_largs], XmNsaveUnder, True); num_largs++;
    XtSetArg(largs[num_largs], XmNallowShellResize, True); num_largs++;
    XtSetArg(largs[num_largs], XmNancestorSensitive, True); num_largs++;
    new_list = XtMergeArgLists(args, num_args, largs, num_largs);
    XmComboBox2_popup_shell(cbw) = XtCreatePopupShell("popupShell", 
						topLevelShellWidgetClass, w,
						new_list,
						num_largs + num_args);
    XtFree((char *) new_list);

    /*
     * Set the visible item count of the list child widget
     */
    num_largs = 0;
    XtSetArg(largs[num_largs], XmNvisibleItemCount,
	     XmComboBox2_visible_items(cbw)); num_largs++;
    new_list = XtMergeArgLists(args, num_args, largs, num_largs);
    XmComboBox2_list(cbw) = XmCreateScrolledList(XmComboBox2_popup_shell(cbw), "list",
					   new_list, num_args + num_largs);

    XtFree((char *) new_list);

    XtAddCallback(XmComboBox2_list(cbw), XmNdefaultActionCallback,
		  ListSelected, (XtPointer) cbw);

    XtAddCallback(XmComboBox2_list(cbw), XmNsingleSelectionCallback,
		  ListSelected, (XtPointer) cbw);

    XtAddCallback(XmComboBox2_list(cbw), XmNbrowseSelectionCallback,
		  ListSelected, (XtPointer) cbw);

    XtManageChild(XmComboBox2_list(cbw));
}

/*	Function Name: PopdownList
 *	Description:   Pops down the list of choices.
 *	Arguments:     w - the combo box.
 *	Returns:       none
 */

static void
PopdownList(Widget w)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;

    if (XmComboBox2_popup_shell(cbw) != NULL) {
	XSetInputFocus(XtDisplay(w), XmComboBox2_focus_owner(cbw),
		       XmComboBox2_focus_state(cbw), 
		       XtLastTimestampProcessed(XtDisplay(w)));
	
	/*
	 * The order is important here to keep from generating Xt Errors.
	 */
	
	XtRemoveGrab(XmComboBox2_arrow(cbw));
	XtUngrabKeyboard(w, XtLastTimestampProcessed(XtDisplay(w)));
	XtPopdown(XmComboBox2_popup_shell(cbw));
    }
    else {
	XtAppWarningMsg(XtWidgetToApplicationContext(w),
			XiNnoComboShell, XiNnoComboShell,
			XmCICSWidgetSetError,
			XiNnoComboShellMsg, NULL, NULL);
    }
}

/* ARGSUSED */
static void
TextButtonPress(Widget w , XtPointer client, XEvent *event, Boolean *go_on)
{
    XmCombinationBox2Widget cbw = FindComboBox(w);

    if (cbw == NULL)
	return;

    /*
     * It is debatable whether or not we should eat this event.  For now,
     * we'll let it continue.
     */
#ifdef notdef
    *go_on = False;
#endif

    if (event->xany.type == ButtonPress)  {
	if (XmComboBox2_list_state(cbw) == XmCombinationBox2_POSTED)  {
	    (void)ComboPost( w, NULL, NULL, NULL );
	    XmComboBox2_list_state(cbw) = XmCombinationBox2_BEGIN_POPUP_FROM_TEXT;
	    return;
	}
	else if (XmComboBox2_list_state(cbw) == XmCombinationBox2_UNPOSTED) {
	    (void)ComboUnpost( w, NULL, NULL, NULL );
	}
	else if (XmComboBox2_list_state(cbw) == XmCombinationBox2_BEGIN_POPUP_FROM_TEXT)
	    XmComboBox2_list_state(cbw) = XmCombinationBox2_UNPOSTED;
    }
}


/*	Function Name: PopupList
 *	Description:   Pops up the list of choices.
 *	Arguments:     w - the combo box.
 *	Returns:       True if sucessful.
 */

static Boolean
PopupList(Widget w)
{	
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;
    Widget shell = XmComboBox2_popup_shell(cbw);
    Position x, y, temp;
    Dimension width;
    int ret, scr_width, scr_height;
    Arg args[10];
    Cardinal num_args;

    if (shell == NULL) {
	XtAppWarningMsg(XtWidgetToApplicationContext(w),
			XiNnoComboShell, XiNnoComboShell, XmCICSWidgetSetError,
			XiNnoComboShellMsg, NULL, NULL);
	return(False);
    }

    XtTranslateCoords(w, 0, XtHeight(w), &x, &y);
    XtRealizeWidget(shell);

    num_args = 0;

    temp = XmComboBox2_text_x(cbw) + XmComboBox2_popup_offset(cbw);
    x += temp;

    if (!XmComboBox2_customized_combo_box(cbw)) {
	width = (cbw->core.width - temp -
		 (shell)->core.border_width);
	XtSetArg(args[num_args], XmNwidth, width); num_args++;
    }
    else
    {
	width = XtWidth(shell);
    }

    /*
     * Now that we know where on the screen we want to display the popup
     * we have to make sure that it will fit on the screen. What we
     * do here is check if y + height and x + width are still on the
     * screen. If now we decrease x and/or until the popup would be on
     * the screen. As a last check we make sure that x and y are >= 0.
     *
     * Lets start by getting the width and height of the screen.
     */
    scr_width = WidthOfScreen(XtScreen(shell));
    scr_height = HeightOfScreen(XtScreen(shell));

    if( (int)(y + XtHeight(shell)) > scr_height )
    {
	Position tmp;
	XtTranslateCoords(w, 0, 0, &tmp, &y);
	y -= ((int)XtHeight(shell));
    }
    if( y < 0 ) y = 0;
    if( (int)(x + width) > scr_width )
    {
	x = scr_width - ((int)width);
    }
    if( x < 0 ) x = 0;

    XtSetArg(args[num_args], XmNx, x); num_args++;
    XtSetArg(args[num_args], XmNy, y); num_args++;
    XtSetValues(shell, args, num_args);
    
    /*
     * Because of an Xt bug, we need to cal the shell widget's resize
     * proc ourselves.
     */

    (*(XtClass(shell)->core_class.resize))(shell);

    XGetInputFocus(XtDisplay(shell), 
		   &(XmComboBox2_focus_owner(cbw)), &(XmComboBox2_focus_state(cbw)));

    /*
     * If we are in pointer root mode it is possible that the pointer is
     * over our popup so that when it is popped up we have the focus.
     * if we later do a XSetInputFocus a lose focus will be delivered
     * to us and pop us down.  I have solved this problem by temporarily
     * setting the keyboard focus to the combo box's window and then
     * popping up the window and setting the focus to us.  This keeps
     * us from losing the focus.
     * 
     * Keyboard events could be lost, but until the window comes up we
     * don't know where the events will be delivered anyway so this is
     * no worse than just settting the input focus to our window.
     *
     * A Sync. popup may solve this problem, but it is not too big a deal.
     *
     * Chris D. Peterson
     */

    XSetInputFocus(XtDisplay(shell), XtWindow((Widget) cbw), RevertToParent, 
		   XtLastTimestampProcessed(XtDisplay(w)) - 1);

    XtPopupSpringLoaded(shell);

    ret = XtGrabPointer(shell, True, 
			ButtonPressMask | ButtonReleaseMask,
			GrabModeAsync, GrabModeAsync, None, 
			XmComboBox2_popup_cursor(cbw),
			XtLastTimestampProcessed(XtDisplay(w)));

    if (ret != GrabSuccess) {
	XtPopdown(shell);

	/*
	 * We could not popup the list, this is because somehow things
	 * got so slow that we were not allowed to grab the server.
	 * In this case I will fail silently and the user can try again.
	 */

	return(False);
    }
	
    XtAddGrab(XmComboBox2_arrow(cbw), False, False);

    /*
     * Since this is an override redirect window we know that no one
     * will get in the way of thisX SetInputFocus Event.
     */

    XSetInputFocus(XtDisplay(shell), 
		   XtWindow(shell), RevertToParent, CurrentTime);

    return(True);
}

/*	Function Name: SetListFromText
 *	Description:   Makes the list highlights match the text widget.
 *	Arguments:     w - the combo box widget.
 *                     no_action - just check to see if this would succeed.
 *	Returns:       True if each item was found, or the text widget is
 *                     empty.
 */

static Boolean
SetListFromText(Widget w, Boolean no_action)
{
    Arg args[10];
    Cardinal num_args;
    int count = 0, vcount, tcount;
    XrmValue to, from;
    XmStringTable table, tptr;
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;
    String ptr = XmTextFieldGetString(XmComboBox2_text(cbw));
    XmStringTable sel_table;
    Boolean error = False;
    unsigned char policy;
    XmCombinationBox2ClassPartExtension *addition;

    if (ptr != NULL) {
	int num_items;

	addition = CheckExtensions((XmCombinationBox2WidgetClass)XtClass(cbw));
	if (addition && addition->setListFromText)
	{
	    XtFree(ptr);
	    return (*addition->setListFromText)(w, XmComboBox2_text(cbw),
						XmComboBox2_list(cbw));
	}

	num_args = 0;
	XtSetArg(args[num_args], XmNitemCount, &num_items); num_args++;
	XtGetValues(XmComboBox2_list(cbw), args, num_args);
	
	/*
	 * Strlen can be used here because we are attempting to find the
	 * number of bytes in the string not the number of i18n characters.
	 */

	from.size = sizeof(char) * (strlen(ptr) + 1);
	from.addr = ptr;
	
	to.size = sizeof(XmStringTable);
	to.addr = (XtPointer) &table;
	
	XtConvertAndStore(XmComboBox2_list(cbw), XmRString, &from, 
			  XmRXmStringTable, &to);
	

	/*
	 * If the text field contains "", the table will be NULL
	 */
	if (table != NULL)  {
	    for(tptr = table, count = 0; *tptr != NULL ; tptr++) count++; 
	    sel_table = (XmStringTable) XtMalloc(sizeof(XmString) * count);
	    for(tptr = table, count = 0; *tptr != NULL ; tptr++) {
		if (XmListItemExists(XmComboBox2_list(cbw), *tptr))
		    sel_table[count++] = *tptr;
		else {
		    error = TRUE;
		    break;
		}
	    }
	}
	else
		sel_table = NULL;

	XtFree((char *) ptr);
    }
    else {
	count = 0;
	sel_table = NULL;
    }

    if (!no_action)
	XmListDeselectAllItems(XmComboBox2_list(cbw));

    /*
     * If single select and there is more than one element in 
     * the list, then we have an error.
     */

    num_args = 0;
    XtSetArg(args[num_args], XmNselectionPolicy, &policy); num_args++;
    XtSetArg(args[num_args], XmNvisibleItemCount, &vcount); num_args++;
    XtSetArg(args[num_args], XmNitemCount, &tcount); num_args++;
    XtGetValues(XmComboBox2_list(cbw), args, num_args);

    if ((((policy == XmSINGLE_SELECT) || 
	  (policy == XmBROWSE_SELECT)) && (count > 1)) || error) 
    {
    	XtFree((char *) sel_table);
	return(FALSE);
    }

    num_args = 0;
    XtSetArg(args[num_args], XmNselectedItems, sel_table); num_args++;
    XtSetArg(args[num_args], XmNselectedItemCount, count); num_args++;
    XtSetValues(XmComboBox2_list(cbw), args, num_args);
	
    /*
     * Makes the first selected item the first item in the list.
     */
    
    if (count > 0) {
	int *pos_list, num, pos = 0;

	XmListGetMatchPos(XmComboBox2_list(cbw), sel_table[0], &pos_list, &num);

	if (num > 0) {
	    pos = pos_list[0] - vcount/2;
	    if (pos < 1)
		pos = 1;
	    else if (pos > (tcount - vcount + 1))
		pos = tcount - vcount + 1;

	    XtFree((char *) pos_list);
	}
	
	XmListSetPos(XmComboBox2_list(cbw), pos);
    }

    XtFree((char *) sel_table);

    return(TRUE);
}

/*	Function Name: SetTextFromList
 *	Description:   Makes the text strings matched the elements 
 *                     highlighted in the list.
 *	Arguments:     w - the combo box widget.
 *	Returns:       True if each item was found, or the text widget is
 *                     empty.
 */

static Boolean
SetTextFromList(Widget w)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;
    Arg args[10];
    Cardinal num_args;    
    XmStringTable items;
    int count;
    unsigned char policy;
    register int i, text_loc;
    XmCombinationBox2ClassPartExtension *addition;

    addition = CheckExtensions( (XmCombinationBox2WidgetClass)XtClass(cbw) );
    if (addition && addition->setTextFromList)
    {
	return (*addition->setTextFromList)(w,XmComboBox2_text(cbw),
					    XmComboBox2_list(cbw));
    }

    num_args = 0;
    XtSetArg(args[num_args], XmNselectedItems, &items); num_args++;
    XtSetArg(args[num_args], XmNselectedItemCount, &count); num_args++;
    XtSetArg(args[num_args], XmNselectionPolicy, &policy); num_args++;
    XtGetValues(XmComboBox2_list(cbw), args, num_args);

    if ((policy == XmMULTIPLE_SELECT) || (policy == XmEXTENDED_SELECT) ||
	(count > 0))
    {
	XmTextFieldSetString(XmComboBox2_text(cbw), "");
    }

    text_loc = 0;
    i = 0;
    while (i < count) {
	register int len;
	String ptr;
	wchar_t temp[BUFSIZ];

	ptr = _XmGetMBStringFromXmString(items[i]);

	if (ptr == NULL) {
	    static String strs[] = {"Combination Box", NULL};
	    Cardinal num = 1;
	    XtAppWarningMsg(XtWidgetToApplicationContext((Widget) cbw),
			    XiNstringGetFailed, XiNstringGetFailed,
			    XmCICSWidgetSetError,
			    XiNstringGetFailedMsg, strs, &num);

	    i++;
	    continue;
	}

	/*
	 * There must be a better way to do a strlen for I18N text.
	 */

	len = (int) mbstowcs(temp, ptr, BUFSIZ);
	
	XmTextFieldInsert(XmComboBox2_text(cbw), text_loc, ptr);
	XtFree((char *) ptr);
	text_loc += len;
	
	if (++i >= count)
	    break;
	
	XmTextFieldInsert(XmComboBox2_text(cbw), text_loc, ",");
	text_loc++;
    }	

    XmTextFieldSetInsertionPosition(XmComboBox2_text(cbw), 0);
    return(FALSE);
}

/************************************************************
 *
 * Public Routines.
 *
 ************************************************************/

/*	Function Name: XmCombinationBox2GetValue
 *	Description:   Retreives the value from the combo box.
 *	Arguments:     w - the combination box.
 *	Returns:       The value in the text widget.
 */

String
XmCombinationBox2GetValue(Widget w)
{
    XmCombinationBox2Widget cbw = (XmCombinationBox2Widget) w;

    return(XmTextFieldGetString(XmComboBox2_text(cbw)));
}

/*	Function Name: XmCreateCombinationBox2
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The created widget.
 */

Widget
XmCreateCombinationBox2(Widget parent, String name,
		      ArgList args, Cardinal num_args)
{
    return(XtCreateWidget(name, xmCombinationBox2WidgetClass,
			  parent, args, num_args));
}

/*      Function Name:  XmCombinationBox2GetLabel
 *      Description:    Returns the "label" child of the XmCombinationBox2
 *      Arguments:      w - The XmCombinationBox2 Widget
 *      Returns:        The specified child of the XmCombinationBox2
 */

Widget XmCombinationBox2GetLabel(Widget w)
{
    if(!XtIsSubclass(w, xmCombinationBox2WidgetClass))
	return NULL;
    return XmComboBox2_label(w);
}



/*      Function Name:  XmCombinationBox2GetArrow
 *      Description:    Returns the "arrow" child of the XmCombinationBox2
 *      Arguments:      w - The XmCombinationBox2 Widget
 *      Returns:        The specified child of the XmCombinationBox2
 */

Widget XmCombinationBox2GetArrow(Widget w)
{
    if(!XtIsSubclass(w, xmCombinationBox2WidgetClass))
	return NULL;
    return XmComboBox2_arrow(w);
}



/*      Function Name:  XmCombinationBox2GetText
 *      Description:    Returns the "text" child of the XmCombinationBox2
 *      Arguments:      w - The XmCombinationBox2 Widget
 *      Returns:        The specified child of the XmCombinationBox2
 */

Widget XmCombinationBox2GetText(Widget w)
{
    if(!XtIsSubclass(w, xmCombinationBox2WidgetClass))
	return NULL;
    return XmComboBox2_text(w);
}



/*      Function Name:  XmCombinationBox2GetList
 *      Description:    Returns the "list" child of the XmCombinationBox2
 *      Arguments:      w - The XmCombinationBox2 Widget
 *      Returns:        The specified child of the XmCombinationBox2
 */

Widget XmCombinationBox2GetList(Widget w)
{
    if(!XtIsSubclass(w, xmCombinationBox2WidgetClass))
	return NULL;
    return XtNameToWidget(w, "*list");
}


