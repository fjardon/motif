/*
 *    Copyright 2008, Integrated Computer Solutions, Inc.
 *
 *		       All Rights Reserved.
 *
 * AUTHOR:  Andriy Konoval
 *
 */

/************************************************************
 *       INCLUDE FILES
 ************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "ColorSBP.h"

#include <Xm/Xm.h>
#include <Xm/VaSimpleP.h>
#include <Xm/ButtonBox.h>
#include <Xm/Scale.h>
#include <Xm/ScrolledW.h>
#include <Xm/List.h>
#include <Xm/RowColumn.h>
#include <Xm/ToggleB.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/Container.h>
#include <Xm/BulletinB.h>
#include "GeoUtilsI.h"
#include <Xm/TraitP.h>
#include <Xm/ActivatableT.h>
#include "BulletinBI.h"
#include <Xm/DataF.h>
#include <Xm/Text.h>
#include <Xm/PushB.h>
#include <Xm/ScrollBar.h>
#include <Xm/DrawingA.h>
#include <Xm/ExtP.h>
#include <Xm/Separator.h>
#include <Xm/ButtonBox.h>
#include <Xm/SeparatoG.h>
#include "XmI.h"
#include <Xm/RowColumnP.h>
#include "MessagesI.h"

#define BASIC_COLOR_STRING	_XmMMsgColorSB_0001
#define CUSTOM_COLOR_STRING	_XmMMsgColorSB_0002
#define HUE_STRING			_XmMMsgColorSB_0003
#define SAT_STRING			_XmMMsgColorSB_0004
#define VAL_STRING			_XmMMsgColorSB_0005
#define RED_STRING			_XmMMsgColorSB_0006
#define GREEN_STRING		_XmMMsgColorSB_0007
#define BLUE_STRING			_XmMMsgColorSB_0008
#define ADD_COLOR_STRING	_XmMMsgColorSB_0009

/************************************************************
 *       TYPEDEFS AND DEFINES
 ************************************************************/

#define XmCS_MAX_WIDGETS_VERT   4

#define SUPERCLASS ( (WidgetClass) &xmBulletinBoardClassRec)

#define IsButton(w) \
	(((XtPointer) XmeTraitGet((XtPointer) XtClass((w)), XmQTactivatable) != NULL))

#define IsAutoButton(csb, w) ( \
	w == XmColorSB_pButtonOk (csb) || \
	w == XmColorSB_pButtonCancel(csb) || \
	w == XmColorSB_pButtonHelp(csb) || \
	w == XmColorSB_pButtonAddColor(csb))

#define SetupWorkArea(csb) \
if (_XmGeoSetupKid (boxPtr, XmColorSB_WorkArea(csb))) \
{ \
	layoutPtr->space_above = vspace; \
	vspace = BB_MarginHeight(csb); \
	boxPtr += 2; \
	++layoutPtr; \
}

typedef struct
{
	XmKidGeometry filter_label;
	XmKidGeometry filter_text;
	XmKidGeometry dir_list_label;
	XmKidGeometry file_list_label;
	Dimension prefer_width;
	Dimension delta_width;
} CS_GeoExtensionRec;

/************************************************************
 *       MACROS
 ************************************************************/

/************************************************************
 *       GLOBAL DECLARATIONS
 ************************************************************/

extern void XmeNavigChangeManaged(Widget);

/************************************************************
 *       STATIC FUNCTION DECLARATIONS
 ************************************************************/

static void ChangeManaged(Widget w);
static void Destroy(Widget);
static void ClassPartInitialize(WidgetClass w_class);
static void Initialize(Widget, Widget, ArgList, Cardinal *);
static void Redisplay(register Widget wid, XEvent *event, Region region);

static Boolean SetValues(Widget, Widget, Widget, ArgList, Cardinal *);
static XtGeometryResult GeometryHandler(Widget, XtWidgetGeometry *, XtWidgetGeometry *);
static XtGeometryResult QueryGeometry(Widget, XtWidgetGeometry *, XtWidgetGeometry *);

static void CalcPreferredSize(XmColorSelectionBoxWidget, Dimension *, Dimension *);
static void CreateColorPaletteWindow(XmColorSelectionBoxWidget, ArgList, Cardinal);
static void CreateColorMatrixWindow(XmColorSelectionBoxWidget, ArgList, Cardinal);
static void CreateSeparatorAndButtons(XmColorSelectionBoxWidget, ArgList, Cardinal);
static void ChangeBackgroundColor(XmColorSelectionBoxWidget csb, int _value);
static void DrawVolume(XmColorSelectionBoxWidget csb, Position y);
static void DrawMatrix(XmColorSelectionBoxWidget csb);
static void CreateColorMatrix(XmColorSelectionBoxWidget csb, Visual *visual);
static void CreateVolumeGradient(XmColorSelectionBoxWidget w, Visual *visual);
static void ReDrawCrossPixel(XmColorSelectionBoxWidget csb, Position y, Position x, Visual *visual);
static void DrawCross(XmColorSelectionBoxWidget, Position x, Position y);
static void FillColorFields(XmColorSelectionBoxWidget csb);
static int DugitCut(Widget w);
static void CalculateColor(XmColorSelectionBoxWidget csb, Position x, Position y);
static void trTable_Volume(Widget w, XEvent *event, String *args, Cardinal *num_args);
static void cb_Volume(Widget w, XtPointer csb_ptr, XtPointer call_data);
static void cb_Input(Widget w, XtPointer csb_ptr, XtPointer call_data);
static void cb_ChangeCoordinateColor(Widget w, XtPointer csb_ptr, XtPointer call_data);
static void cb_ChangeColor(Widget w, XtPointer csb_ptr, XtPointer call_data);
static void cb_AppearCursor(Widget w, XtPointer csb_ptr, XtPointer call_data);
static void cb_AddColor(Widget w, XtPointer csb_ptr, XtPointer call_data);
static void cb_BrightnessChange(Widget w, XtPointer csb_ptr, XtPointer call_data);
static void cb_PutColor(Widget w, XEvent *event, String *args, Cardinal *num_args);
static void cb_Ok(Widget w, XtPointer csb_ptr, XtPointer call_data);
static void ColorFieldCreate(Widget parent, Widget *w, char *w_name, unsigned long color_name, Position x, Position y);
static void LoadCustomColors(Widget w, XmColorSelectionBoxWidget csb);
static void LoadColorValues(XmColorSelectionBoxWidget csb);
static void _XmSelectionBoxCreateButtons(XmColorSelectionBoxWidget csb);
static void ColorSelectionBoxCallback(Widget w, XtPointer client_data, XtPointer call_data);
static void GetValues_XmNredValue(Widget w, int n, XtArgVal *value);
static void GetValues_XmNgreenValue(Widget w, int n, XtArgVal *value);
static void GetValues_XmNblueValue(Widget w, int n, XtArgVal *value);
static void GetValues_XmNhueValue(Widget w, int n, XtArgVal *value);
static void GetValues_XmNsatValue(Widget w, int n, XtArgVal *value);
static void GetValues_XmNbrightnessValue(Widget w, int n, XtArgVal *value);
static void GetValues_XmNhelpOn(Widget w, int n, XtArgVal *value);
static XmGeoMatrix ColorSelectionBoxGeoMatrixCreate(Widget wid, Widget instigator, XtWidgetGeometry *desired);

extern int _XmManagerHelp(Widget wid, XEvent *event, String *params,/* unused */Cardinal *num_params); /* unused */

/************************************************************
 *       STATIC DECLARATIONS
 ************************************************************/

static XtResource resources[] =
{
	{
		XmNcolorValue, XmRPixel, XmRPixel,
		sizeof(Pixel), XtOffsetOf(XmColorSelectionBoxRec, cs.color_value),
		XmRPixel, 0x00
	},

	{
		XmNhueValue, XmRInt , XmRInt,
		sizeof(int), XtOffsetOf(XmColorSelectionBoxRec, cs.x),
		XmRInt, 0
	},

	{
		XmNsatValue, XmRInt , XmRInt,
		sizeof(int), XtOffsetOf(XmColorSelectionBoxRec, cs.y),
		XmRInt, 0
	},

	{
		XmNbrightnessValue, XmRInt , XmRInt,
		sizeof(int), XtOffsetOf(XmColorSelectionBoxRec, cs.brightness_volume),
		XmRInt, 0
	},

	{
		XmNredValue, XmRInt , XmRInt,
		sizeof(int), XtOffsetOf(XmColorSelectionBoxRec, cs.red_value),
		XmRInt, 0
	},

	{
		XmNgreenValue, XmRInt , XmRInt,
		sizeof(int), XtOffsetOf(XmColorSelectionBoxRec, cs.green_value),
		XmRInt, 0
	},

	{
		XmNblueValue, XmRInt , XmRInt,
		sizeof(int), XtOffsetOf(XmColorSelectionBoxRec, cs.blue_value),
		XmRInt, 0
	},

	{
		XmNhelpOn, XmRInt , XmRInt,
		sizeof(int), XtOffsetOf(XmColorSelectionBoxRec, cs.helpOn),
		XmRInt, 0
	},

	{
		XmNokCallback,
		XmCCallback, XmRCallback, sizeof (XtCallbackList),
		XtOffsetOf( struct _XmColorSelectionBoxRec, cs.ok_callback)
		,
		XmRImmediate, (XtPointer) NULL
	},

	{
		XmNcancelCallback,
		XmCCallback, XmRCallback, sizeof (XtCallbackList),
		XtOffsetOf(struct _XmColorSelectionBoxRec, cs.cancel_callback),
		XmRImmediate, (XtPointer) NULL
	},
	{
		XmNautoUnmanage, XmCAutoUnmanage, XmRBoolean,
		sizeof(Boolean), XtOffsetOf(struct _XmColorSelectionBoxRec, bulletin_board.auto_unmanage),
		XmRImmediate, (XtPointer) False
	},
};

static XmSyntheticResource get_resources[] =
{
	{
		XmNredValue,  sizeof(int),
		XtOffsetOf(XmColorSelectionBoxRec, cs.red_value),
		GetValues_XmNredValue, NULL
	},

	{
		XmNgreenValue, sizeof(int),
		XtOffsetOf(XmColorSelectionBoxRec, cs.green_value),
		GetValues_XmNgreenValue, NULL
	},

	{
		XmNblueValue,  sizeof(int),
		XtOffsetOf(XmColorSelectionBoxRec, cs.blue_value),
		GetValues_XmNblueValue, NULL
	},

	{
		XmNhueValue, sizeof(int),
		XtOffsetOf(XmColorSelectionBoxRec, cs.x),
		GetValues_XmNhueValue, NULL
	},

	{
		XmNsatValue, sizeof(int),
		XtOffsetOf(XmColorSelectionBoxRec, cs.y),
		GetValues_XmNsatValue, NULL
	},

	{
		XmNbrightnessValue, sizeof(int),
		XtOffsetOf(XmColorSelectionBoxRec, cs.brightness_volume),
		GetValues_XmNbrightnessValue, NULL
	},

	{
		XmNhelpOn,  sizeof(int),
		XtOffsetOf(XmColorSelectionBoxRec, cs.helpOn),
		GetValues_XmNhelpOn, NULL
  },
};

XmColorSelectionBoxClassRec xmColorSelectionBoxClassRec = {
  { /* core fields */
    /* superclass               */      SUPERCLASS,
    /* class_name               */      "XmColorSelectionBox",
    /* widget_size              */      sizeof(XmColorSelectionBoxRec),
    /* class_initialize         */      NULL,
    /* class_part_initialize    */      ClassPartInitialize,
    /* class_inited             */      False,
    /* initialize               */      Initialize,
    /* initialize_hook          */      NULL,
    /* realize                  */      XtInheritRealize,
    /* actions                  */      NULL,
    /* num_actions              */      (Cardinal)0,
    /* resources                */      (XtResource*)resources,
    /* num_resources            */      XtNumber(resources),
    /* xrm_class                */      NULLQUARK,
    /* compress_motion          */      True,
    /* compress_exposure        */      True,
    /* compress_enterleave      */      True,
    /* visible_interest         */      False,
    /* destroy                  */      Destroy,
    /* resize                   */      XtInheritResize,
    /* expose                   */     	Redisplay,
    /* set_values               */      SetValues,
    /* set_values_hook          */      NULL,
    /* set_values_almost        */      XtInheritSetValuesAlmost,
    /* get_values_hook          */      NULL,
    /* accept_focus             */      NULL,
    /* version                  */      XtVersion,
    /* callback_private         */      NULL,
    /* tm_table                 */      XtInheritTranslations,
    /* query_geometry           */      (XtGeometryHandler) QueryGeometry,
    /* display_accelerator      */      XtInheritDisplayAccelerator,
    /* extension                */      NULL,
  },
  {            /* composite_class fields */
    /* geometry_manager   	*/      GeometryHandler,//XtInheritGeometryManager,
    /* change_managed     	*/      ChangeManaged,
    /* insert_child       	*/      XtInheritInsertChild,
    /* delete_child       	*/      XtInheritDeleteChild,
    /* extension          	*/      NULL,
  },
  {            /* constraint_class fields */
   /* resource list        	*/ 	NULL,
   /* num resources        	*/ 	0,
   /* constraint size      	*/ 	sizeof(XmColorSelectionBoxConstraintRec),
   /* destroy proc         	*/ 	NULL,
   /* init proc            	*/ 	NULL,
   /* set values proc      	*/ 	NULL,
   /* extension            	*/ 	NULL,
  },
  {            /* manager_class fields */
   /* default translations   	*/      XtInheritTranslations,
   /* syn_resources          	*/      get_resources,
   /* num_syn_resources      	*/      XtNumber(get_resources),
   /* syn_cont_resources     	*/      NULL,
   /* num_syn_cont_resources 	*/      0,
   /* parent_process         	*/      XmInheritParentProcess,//NULL,
   /* extension              	*/      NULL,
  },
  {	/* bulletin board class record */
    /*always_install_accelerators*/  	TRUE,
    /* geo_matrix_create */        		ColorSelectionBoxGeoMatrixCreate,//NULL,
    /* focus_moved_proc */       		XmInheritFocusMovedProc,
    /* extension */						NULL,
  },

  {	      /* cr_selector_class fields */
    /* mumble 		  	*/	NULL,
  }
};

WidgetClass xmColorSelectionBoxWidgetClass = (WidgetClass)&xmColorSelectionBoxClassRec;

/************************************************************
 *       STATIC CODE
 ************************************************************/

/*
 * ClassPartInitialize sets up the fast subclassing for the widget.
 */
static void
#ifdef _NO_PROTO
ClassPartInitialize(w_class)
WidgetClass w_class;
#else
ClassPartInitialize(WidgetClass w_class)
#endif /* _NO_PROTO */
{
	_XmFastSubclassInit (w_class, XmCOLORSELECTOR_BIT);
}

static void Redisplay(register Widget wid, XEvent *event, Region region)

{
	XmColorSelectionBoxWidget csb = (XmColorSelectionBoxWidget)wid;
	XtExposeProc expose;
	if (XmColorSB_first_expose(csb)) {
		DrawMatrix(csb);
		XmColorSB_first_expose(csb) = 0;
	} else {
		XPutImage(XtDisplay(wid), XtWindow(XmColorSB_drawingAreaMatrix(wid)),XmColorSB_gc(wid), XmColorSB_im1(wid), 0,0,0,0, H_len, S_len);
		XPutImage(XtDisplay(wid), XtWindow(XmColorSB_drawingAreaVolume(wid)),XmColorSB_gc2(wid), XmColorSB_im2(wid), 0,0,0,0, DRAW_VOLUME_WIDTH, S_len);
	}

	_XmProcessLock();
	expose = xmBulletinBoardClassRec.core_class.expose;
	_XmProcessUnlock();
	(*expose)((Widget)csb, event, region);
}

/*      Function Name: Initialize
 *      Description:   Called to initialize information specific
 *                     to this widget.
 *      Arguments:     request - what was originally requested.
 *                     set - what will be created (our superclasses have
 *                           already mucked with this)
 *                     args, num_args - The arguments passed to
 *                                      the creation call.
 *      Returns:       none.
 */

/* ARGSUSED */
static void Initialize(Widget request, Widget set, ArgList args, Cardinal *num_args)
{
	XmColorSelectionBoxWidget csb = (XmColorSelectionBoxWidget)set;
	Dimension width, height;
	ArgList f_args;
	Cardinal f_num_args;

	_XmFilterArgs(args, *num_args, xm_std_filter, &f_args, &f_num_args);

	/*
	 * Initialize important values.
	 */
	XmColorSB_first_expose(csb)=1;

	CreateColorPaletteWindow(csb, f_args, f_num_args);
	CreateColorMatrixWindow(csb, f_args, f_num_args);
	CreateSeparatorAndButtons(csb, f_args, f_num_args);

	CalcPreferredSize(csb, &width, &height);

	if (csb->core.width < 1)
		csb->core.width = width;

	if (csb->core.height < 1)
		csb->core.height = height;

	XtFree((XtPointer) f_args);

	if (csb->manager.initial_focus == NULL) {
		csb->manager.initial_focus = XmColorSB_pButtonOk(csb);
	}
}
/*      Function Name: Destroy
 *      Description:   Called to destroy this widget.
 *      Arguments:     w - ColorSelectorBox  Widget to destroy.
 *      Returns:       none.
 */

/* ARGSUSED */
static void Destroy(Widget w)
{
	XmColorSelectionBoxWidget csb = (XmColorSelectionBoxWidget)w;

    XFreeGC(XtDisplay(w), XmColorSB_gc(csb));
    XFreeGC(XtDisplay(w), XmColorSB_gc2(csb));
	XDestroyImage(XmColorSB_im1(csb));
	XDestroyImage(XmColorSB_im2(csb));
}

/*      Function Name: SetValues
 *      Description:   Called when some widget data needs to be modified on-
 *                     the-fly.
 *      Arguments:     current - the current (old) widget values.
 *                     request - before superclassed have changed things.
 *                     set - what will acutally be the new values.
 *                     args, num_args - the arguments in the list.
 *      Returns:       none
 */

/* ARGSUSED */
static Boolean SetValues(Widget current, Widget request, Widget set, ArgList args, Cardinal *num_args)
{
	XmColorSelectionBoxWidget csb = (XmColorSelectionBoxWidget)set;
	XmColorSelectionBoxWidget curr = (XmColorSelectionBoxWidget)current;
	char s[10];
	Boolean ColorChanged = False;
	/*
	 * Pass argument list through to all children.
	 */

	{
		ArgList f_args;
		Cardinal f_num_args;

		_XmFilterArgs(args, *num_args, xm_std_filter, &f_args, &f_num_args);
		XtFree((XtPointer) f_args);
	}

	if (XmColorSB_color_value(curr) != XmColorSB_color_value(csb)) {
		int r, g, b;
		char s[10];
		unsigned long p;
	   	p = XmColorSB_color_value(csb);

		r = p&0xff0000;
		g = p&0x00ff00;
		b = p&0x0000ff;

		r = r>>16;
		g = g>>8;

		sprintf(s, "%d", r);
		XmTextFieldSetString(XmColorSB_redF(csb), s);
		sprintf(s, "%d", g);
		XmTextFieldSetString(XmColorSB_greenF(csb), s);
		sprintf(s, "%d", b);
		XmTextFieldSetString(XmColorSB_blueF(csb), s);
		if (XmColorSB_im1(csb))
			cb_ChangeColor(NULL, csb , NULL);
	}
	else
	{
		if (XmColorSB_x(curr) != XmColorSB_x(csb)) {
			sprintf(s, "%d", XmColorSB_x(csb));
			XmTextFieldSetString(XmColorSB_hueF(csb), s);
			XmColorSB_x(csb)=DugitCut(XmColorSB_hueF(csb));
			FillColorFields(csb);
			if (XmColorSB_im1(csb))
				cb_ChangeCoordinateColor(XmColorSB_hueF(csb), csb, NULL);
		}
		if (XmColorSB_y(curr) != XmColorSB_y(csb)) {
			sprintf(s, "%d", XmColorSB_y(csb));
			XmTextFieldSetString(XmColorSB_satF(csb), s);
			XmColorSB_y(csb)=DugitCut(XmColorSB_satF(csb));
			FillColorFields(csb);
			if (XmColorSB_im1(csb))
				cb_ChangeCoordinateColor(XmColorSB_satF(csb), csb, NULL);
		}

		if (XmColorSB_red_value(curr) != XmColorSB_red_value(csb)) {
			sprintf(s, "%d", XmColorSB_red_value(csb));
			XmTextFieldSetString(XmColorSB_redF(csb), s);
			ColorChanged=True;
		}
		if (XmColorSB_green_value(curr) != XmColorSB_green_value(csb)) {
			sprintf(s, "%d", XmColorSB_green_value(csb));
			XmTextFieldSetString(XmColorSB_greenF(csb), s);
			ColorChanged=True;
		}
		if (XmColorSB_blue_value(curr) != XmColorSB_blue_value(csb)) {
			sprintf(s, "%d", XmColorSB_blue_value(csb));
			XmTextFieldSetString(XmColorSB_blueF(csb), s);
			ColorChanged=True;
		}

		if(ColorChanged && XmColorSB_im1(csb)) cb_ChangeColor(NULL, csb, NULL);

		if (XmColorSB_brightness_volume(curr) != XmColorSB_brightness_volume(csb)) {
			sprintf(s, "%d", XmColorSB_brightness_volume(csb));
			XmTextFieldSetString(XmColorSB_brightnessF(csb), s);
			if (XmColorSB_im1(csb))
				cb_BrightnessChange(XmColorSB_brightnessF(csb), csb, NULL);
		}
	}
	if ( XmColorSB_helpOn(curr)!= XmColorSB_helpOn(csb)) {
		XmColorSB_helpOn(csb)=XmColorSB_helpOn(csb);
		if (!XmColorSB_helpOn(csb))
			XtUnmanageChild(XmColorSB_pButtonHelp(csb));
		else
			XtManageChild(XmColorSB_pButtonHelp(csb));
	}

	if (((csb->bulletin_board.margin_width != curr->bulletin_board.margin_width) || (csb->bulletin_board.margin_height
					!= curr->bulletin_board.margin_height))) {
		XtWidgetProc resize;
		_XmProcessLock();
		resize = xmColorSelectionBoxClassRec.core_class.resize;
		_XmProcessUnlock();
		(*resize)(current);
	}

	return FALSE;
}

/*      Function Name: GeometryHandler
 *      Description:   handles request from children for size changes.
 *      Arguments:     child - the child to change.
 *                     request - desired geometry of child.
 *                     result - what will be allowed if almost.
 *      Returns:       status.
 */

/* ARGSUSED */
static XtGeometryResult GeometryHandler(Widget w, XtWidgetGeometry *request, XtWidgetGeometry *result)
{
	return (XtGeometryNo);
}

/*	Function Name: QueryGeometry
 *	Description:   Called when my parent wants to know what size
 *                     I would like to be.
 *	Arguments:     w - the widget to check.
 *                     indended - constriants imposed by the parent.
 *                     preferred - what I would like.
 *	Returns:       See Xt Manual.
 */

static XtGeometryResult QueryGeometry(Widget w, XtWidgetGeometry *intended, XtWidgetGeometry *preferred)
{
	CalcPreferredSize((XmColorSelectionBoxWidget) w, &(preferred->width), &(preferred->height));

	return (_XmHWQuery(w, intended, preferred));
}

/*	Function Name: ChangeManaged
 *	Description: Called when a management change happens.
 *	Arguments: w - the csb widget.
 *	Returns: none
 */

static void ChangeManaged(Widget w)
{
	XtWidgetProc resize;
	_XmProcessLock();
	resize = xmColorSelectionBoxClassRec.core_class.resize;
	_XmProcessUnlock();
	(*resize)(w);

	XmeNavigChangeManaged(w);
}

static Pixel GetCval(unsigned char c, unsigned long mask)
{
	Pixel value = c, x;
	int i;

	for (i=0, x=1; i<32; i++, x <<= 1)
		if (mask & x)
			break;
	for (; i<32; i++, x <<= 1)
		if (!(mask & x))
			break;
	if (i < 8)
		value >>= 8 - i;
	else if (i > 8)
		value <<= i - 8;
	return (value & mask);
}

static void StorePixel(Visual *visual, Screen *screen, int r, int g, int b, int x, char *cdata)
{
	static XColor *ctable= NULL;
	Pixel px = GetCval(r, visual->red_mask) | GetCval(g, visual->green_mask) | GetCval(b, visual->blue_mask);
	if (screen->root_depth == 8) {
		int bestmatch;
		double mindist;
		int i;
		if (!ctable) {
			ctable = malloc(visual->map_entries * sizeof(XColor));
			for (i = 0; i < visual->map_entries; i++)
				ctable[i].pixel = i;
			;
			XQueryColors(screen->display, screen->cmap, ctable, visual->map_entries);
		}

		bestmatch = -1;
		mindist = 0.0;
		for (i=0; i<256; i++) {
			double dr = (double) (r << 8) - (double) ctable[i].red;
			double dg = (double) (g << 8) - (double) ctable[i].green;
			double db = (double) (b << 8) - (double) ctable[i].blue;
			double dist = dr * dr + dg * dg + db * db;
			if (bestmatch < 0 || dist < mindist) {
				bestmatch = i;
				mindist = dist;
			}
		}
		cdata[x] = bestmatch;
	} else if (screen->root_depth <= 16) {
		if (ImageByteOrder(screen->display) == MSBFirst) {
			cdata[x*2] = (px >> 8);
			cdata[x*2+1] = (px & 0xff);
		} else {
			cdata[x*2] = (px & 0xff);
			cdata[x*2+1] = (px >> 8);
		}
	} else {
		if (ImageByteOrder(screen->display) == MSBFirst) {
			cdata[x*4] = (px >> 24);
			cdata[x*4+1] = (px >> 16);
			cdata[x*4+2] = (px >> 8);
			cdata[x*4+3] = (px & 0xff);
		} else {
			cdata[x*4+3] = (px >> 24);
			cdata[x*4+2] = (px >> 16);
			cdata[x*4+1] = (px >> 8);
			cdata[x*4] = (px & 0xff);
		}
	}
}

/*{{{ create color matrix*/
static void CreateColorMatrix(XmColorSelectionBoxWidget w, Visual *visual)
{
	int R, G, B, r, g, b;
	int H, S, Q;
	float k;
	unsigned long point_col;
	XColor screen_in_out;
	Colormap colormap;
	int status;
	Pixel pix;

	/*! This part fill the colors array H- Hue, S - saturation (the purity of the color) */
	/*! We will move from left corner of ColorMatrix to right, we divide Matrix by 6 and fill every part separately */
	Q=H_len/6;
	for (S=0; S<S_len; S++) {
		k=(float)S/Q;
		for (H=0; H<H_len; H++) {
			R=255;
			G=255-S;
			B=255-S;
			if (H>=0 && H<(Q*1)) {
				XmColorSB_arr_col(w)[S][H].r=R;
				XmColorSB_arr_col(w)[S][H].g=G+(int)(k*H);
				XmColorSB_arr_col(w)[S][H].b=B;
			}
			R=255;
			G=255;
			B=255-S;

			if (H>=(Q*1) && H<(Q*2)) {
				XmColorSB_arr_col(w)[S][H].r=R-(int)(k*(H-Q*1));
				XmColorSB_arr_col(w)[S][H].g=G;
				XmColorSB_arr_col(w)[S][H].b=B;
			}
			R=255-S;
			G=255;
			B=255-S;

			if (H>=(Q*2) && H<(Q*3)) {
				XmColorSB_arr_col(w)[S][H].r=R;
				XmColorSB_arr_col(w)[S][H].g=G;
				XmColorSB_arr_col(w)[S][H].b=B+(int)(k*(H-Q*2));
			}
			R=255-S;
			G=255;
			B=255;

			if (H>=(Q*3) && H<(Q*4)) {
				XmColorSB_arr_col(w)[S][H].r=R;
				XmColorSB_arr_col(w)[S][H].g=G-(int)(k*(H-Q*3));
				XmColorSB_arr_col(w)[S][H].b=B;
			}
			R=255-S;
			G=255-S;
			B=255;
			if (H>=(Q*4) && H<(Q*5)) {
				XmColorSB_arr_col(w)[S][H].r=R+(int)(k*(H-Q*4));
				XmColorSB_arr_col(w)[S][H].g=G;
				XmColorSB_arr_col(w)[S][H].b=B;
			}
			R=255;
			G=255-S;
			B=255;
			if (H>=(Q*5) && H<(Q*6)) {
				XmColorSB_arr_col(w)[S][H].r=R;
				XmColorSB_arr_col(w)[S][H].g=G;
				XmColorSB_arr_col(w)[S][H].b=B-(int)(k*(H-Q*5));
			}
		}

	}
	/*! We get pixel from array and put it to image XmColorSB_im1*/
	for (S=0; S<S_len; S++) {
		for (H=0; H<H_len; H++) {
			StorePixel(visual, XtScreen(w),
			XmColorSB_arr_col(w)[S][H].r,
			XmColorSB_arr_col(w)[S][H].g,
			XmColorSB_arr_col(w)[S][H].b, H + (255-S) * XmColorSB_im1(w)->width,
			XmColorSB_im1(w)->data);
		}
	}
}
/*}}}*/

/*!	Function Name: Start
 *	Description: Called when ColorSelectionBox is start the work.
 *	Arguments: w - the csb widget.
 *	Returns: none
 */

static void Start(XmColorSelectionBoxWidget csb)
{
	float k;
	char s[10];

	if (XmColorSB_color_value(csb)!=0){
		int r, g, b;
		char s[10];
		unsigned long p;
		p = XmColorSB_color_value(csb);

		r = p&0xff0000;
		g = p&0x00ff00;
		b = p&0x0000ff;

		r = r>>16;
		g = g>>8;

		sprintf(s, "%d", r);
		XmTextFieldSetString(XmColorSB_redF(csb), s);
		sprintf(s, "%d", g);
		XmTextFieldSetString(XmColorSB_greenF(csb), s);
		sprintf(s, "%d", b);
		XmTextFieldSetString(XmColorSB_blueF(csb), s);

		cb_ChangeColor(NULL, csb , NULL);
	}
	else if ( XmColorSB_x(csb)!=0 ||  XmColorSB_y(csb)!=0)  {
		sprintf(s, "%d", XmColorSB_x(csb));
		XmTextFieldSetString(XmColorSB_hueF(csb), s);
		XmColorSB_x(csb)=DugitCut(XmColorSB_hueF(csb));

		sprintf(s, "%d", XmColorSB_y(csb));
		XmTextFieldSetString(XmColorSB_satF(csb), s);
		XmColorSB_y(csb)=DugitCut(XmColorSB_satF(csb));

		FillColorFields(csb);
		DrawCross(csb, XmColorSB_x(csb), XmColorSB_y(csb));
		cb_ChangeCoordinateColor(XmColorSB_hueF(csb), csb, NULL);

		sprintf(s, "%d", XmColorSB_brightness_volume(csb));
		XmTextFieldSetString(XmColorSB_brightnessF(csb), s);
		cb_BrightnessChange(XmColorSB_brightnessF(csb), csb, NULL);
	}
	else {
		sprintf(s, "%d", XmColorSB_red_value(csb));
		XmTextFieldSetString(XmColorSB_redF(csb), s);

		sprintf(s, "%d", XmColorSB_green_value(csb));
		XmTextFieldSetString(XmColorSB_greenF(csb), s);

		sprintf(s, "%d", XmColorSB_blue_value(csb));
		XmTextFieldSetString(XmColorSB_blueF(csb), s);

		cb_ChangeColor(NULL, csb, NULL);

		sprintf(s, "%d", XmColorSB_x(csb));
		XmTextFieldSetString(XmColorSB_hueF(csb), s);
		XmColorSB_x(csb)=DugitCut(XmColorSB_hueF(csb));

		sprintf(s, "%d", XmColorSB_y(csb));
		XmTextFieldSetString(XmColorSB_satF(csb), s);
		XmColorSB_y(csb)=DugitCut(XmColorSB_satF(csb));
	}
}

static void CreateVolumeGradient(XmColorSelectionBoxWidget csb, Visual *visual)
{
	int R, G, B, r, g, b, V, i;
	int H, S;
	float k;
	unsigned long point_col;
	XColor screen_in_out;
	Colormap colormap;
	int status;

	if (!visual) {
		Widget parent_shell = (Widget)csb;
		while (!XtIsShell(parent_shell))
			parent_shell = XtParent(parent_shell);
		XtVaGetValues(parent_shell, XmNvisual, &visual, NULL);
		if (!visual)
			visual = DefaultVisualOfScreen(XtScreen(parent_shell));
	}

	R = XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].r;
	G = XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].g;
	B = XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].b;
	for (V=XmColorSB_im2(csb)->height-1; V>=0; V--) {
		k=(float)V/XmColorSB_im2(csb)->height;
		r=(int)(R*k);
		g=(int)(G*k);
		b=(int)(B*k);
		for (i = 0; i<XmColorSB_im2(csb)->width; i++) {
			StorePixel(visual, XtScreen(csb), r, g, b, i + (XmColorSB_im2(csb)->height-V-1)*XmColorSB_im2(csb)->width,
			XmColorSB_im2(csb)->data);
		}
	}
	XPutImage(XtDisplay(csb), XtWindow(XmColorSB_drawingAreaVolume(csb)),XmColorSB_gc2(csb), XmColorSB_im2(csb), 0,0,0,0, DRAW_VOLUME_WIDTH, S_len);
	DrawVolume(csb, XmColorSB_brightness_volume(csb));
}

/*!	Function Name: ChangeBackgroundColor
 *	Description: Called for updated XmColorSB_color widget color .
 *	Arguments: w - the csb widget, _value - color value.
 *	Returns: none
 */

static void ChangeBackgroundColor(XmColorSelectionBoxWidget csb, int _value)
{
	float k;
	int status;
	XColor screen_in_out;

	k=(float)_value/(S_len-1);

	screen_in_out.red=(int)((float)XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].r * k)<<8;
	screen_in_out.green=(int)((float)XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].g * k)<<8;
	screen_in_out.blue=(int)((float)XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].b * k)<<8;

	status = XAllocColor(XtDisplay(csb), csb->core.colormap, &screen_in_out);

	if (status) {
		Arg arg = { XmNbackground, screen_in_out.pixel };
		XtSetValues(XmColorSB_color(csb), &arg, 1);
	}
}

/*!	Function Name: DrawVolume
 *	Description: Called for drawing mark on XmColorSB_im2 which show the volume color value.
 *	Arguments: w - the csb widget, y - volume color value.
 *	Returns: none
 */

static void DrawVolume(XmColorSelectionBoxWidget csb, Position y)
{
	char s[5];
	int R, G, B;
	Position x;
	static Position y_old;
	float k, k1, k2;
	Visual *visual= NULL;
	Widget parent_shell = (Widget)csb;
	unsigned long BLACK = 0x000000;
	unsigned long WHITE = 0xFFFFFF;

	if (y < 0)
		y = 0;
	if (y > S_len-1)
		y = S_len-1;
	while (!XtIsShell(parent_shell))
		parent_shell = XtParent(parent_shell);
	XtVaGetValues(parent_shell, XmNvisual, &visual, NULL);

	if (!visual)
		visual = DefaultVisualOfScreen(XtScreen(parent_shell));

	k=(float)(y_old)/XmColorSB_im2(csb)->height;
	if (y_old != 0)
		k1 = (float)(y_old - 1)/XmColorSB_im2(csb)->height;
	else
		k1 = 0;
	if (y_old != S_len-1)
		k2 = (float)(y_old + 1)/XmColorSB_im2(csb)->height;
	else
		k2 = 1;

	R = XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].r;
	G = XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].g;
	B = XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].b;
	for (x = 0; x<XmColorSB_im2(csb)->width; x++) {
		StorePixel(visual, XtScreen(csb), (int)(R*k), (int)(G*k), (int)(B*k), (int)(x + ((S_len - 1) - (y_old)) * DRAW_VOLUME_WIDTH),
		XmColorSB_im2(csb)->data);

		if (y_old != 0)
			StorePixel(visual, XtScreen(csb), (int)(R*k1), (int)(G*k1), (int)(B*k1), (int)(x + ((S_len -1) - (y_old - 1)) * DRAW_VOLUME_WIDTH),
			XmColorSB_im2(csb)->data);
		if (y_old != S_len-1)
			StorePixel(visual, XtScreen(csb), (int)(R*k2), (int)(G*k2), (int)(B*k2), (int)(x + ((S_len-1) - (y_old + 1)) * DRAW_VOLUME_WIDTH),
			XmColorSB_im2(csb)->data);

		/*! draw three lines Black, White and Black for good looking on all color types */
		if (((S_len-1) - y) > 0)
			XPutPixel(XmColorSB_im2(csb), x,((S_len-1) - y) - 1, BLACK);
		XPutPixel(XmColorSB_im2(csb), x,((S_len-1) - y), WHITE);
		if (((S_len-1) - y) < (S_len-1))
			XPutPixel(XmColorSB_im2(csb), x,((S_len-1) - y) + 1, BLACK);
	}
	y_old = y;
	sprintf(s, "%d", y);
	XmTextFieldSetString(XmColorSB_brightnessF(csb), s);

	ChangeBackgroundColor(csb, y);
	XmColorSB_brightness_volume(csb) = y;

	XPutImage(XtDisplay(csb), XtWindow(XmColorSB_drawingAreaVolume(csb)),XmColorSB_gc2(csb), XmColorSB_im2(csb), 0,0,0,0, DRAW_VOLUME_WIDTH, S_len);
}

/*!	Function Name: DrawMatrix
 *	Description: Called for starting drawing ColorMatrix.
 *	Arguments: csb - the csb widget.
 *	Returns: none
 */

static void DrawMatrix(XmColorSelectionBoxWidget csb)
{
	Display *dp;
	int screen;
	XGCValues values= { 0 }, values2= { 0 };
	int depth;
	Visual *visual;
	char *xdata_matrix;
	char *xdata_gradient;
	Widget parent_shell;

	dp = XtDisplay(XmColorSB_drawingAreaMatrix(csb));
	if (!dp) {
		XtError("Can not open display");
		exit(1);
	}

	screen = DefaultScreen(dp);
	depth = DefaultDepth(dp,screen);

	XmColorSB_gc(csb) = XCreateGC(dp, XtWindow(csb), 0, &values);
	XmColorSB_gc2(csb) = XCreateGC(dp, XtWindow(csb), 0, &values);

	if (!XmColorSB_gc(csb) || !XmColorSB_gc2(csb)) {
		XtError("Can not create GC!!!");
		exit(1);
	}

	parent_shell = (Widget)csb;
	while (!XtIsShell(parent_shell))
		parent_shell = XtParent(parent_shell);
	XtVaGetValues(parent_shell, XmNvisual, &visual, NULL);
	if (!visual)
		visual = DefaultVisualOfScreen(XtScreen(parent_shell));
	xdata_matrix = malloc(4 * H_len * (S_len));
	XmColorSB_im1(csb) = XCreateImage(dp, visual, depth, ZPixmap, 0, xdata_matrix, H_len, S_len, 32, 0);
	xdata_gradient = malloc(4 * DRAW_VOLUME_WIDTH * S_len);
	XmColorSB_im2(csb) = XCreateImage(dp, visual, depth, ZPixmap, 0, xdata_gradient, DRAW_VOLUME_WIDTH, S_len, 32, 0);

	if (!XmColorSB_im1(csb) || !XmColorSB_im2(csb)) {
		XtError("Can not create image!!!");
		exit(1);
	}

	CreateColorMatrix(csb, visual);
	Start(csb);
	CreateVolumeGradient(csb, visual);
	XPutImage(dp, XtWindow(XmColorSB_drawingAreaMatrix(csb)), XmColorSB_gc(csb), XmColorSB_im1(csb), 0, 0, 0, 0, H_len, S_len);
	XPutImage(dp, XtWindow(XmColorSB_drawingAreaVolume(csb)), XmColorSB_gc2(csb), XmColorSB_im2(csb), 0, 0, 0, 0, DRAW_VOLUME_WIDTH, S_len);
}

/*!	Function Name: ColorFieldCreate
 *	Description: For creating color frame widget .
 *	Arguments: parent - parent widget, w - widget, color_name - color name , x,y - position.
 *	Returns: none
 */

static void ColorFieldCreate(Widget parent, Widget *w, char *w_name, unsigned long color_name, Position x, Position y)
{
	int ac;
	Arg args[10];

	XtActionsRec actions;
	String translations = "#override \n\
		<Btn1Down>: frame() ManagerGadgetArm()";

	actions.string = "frame";
	actions.proc = cb_PutColor;
	XtAppAddActions(XtDisplayToApplicationContext(XtDisplay(parent)), &actions, 1);

	ac = 0;
	XtSetArg(args[ac], XmNbackground, color_name); ac++;
	XtSetArg (args[ac], XmNtranslations, XtParseTranslationTable (translations)); ac++;
	XtSetArg(args[ac], XmNx, x); ac++;
	XtSetArg(args[ac], XmNy, y); ac++;
	XtSetArg(args[ac], XmNwidth, COLOR_FRAME_WIDTH); ac++;
	XtSetArg(args[ac], XmNheight, COLOR_FRAME_HEIGHT); ac++;
	*w = XmCreateFrame(parent, (char *)"frame", args, ac);
	XtManageChild(*w);
}

/*!      Function Name: 	CreateColorMatrixWindow
 *      Description:   	On BulletinBoard creates a ColorMatrix and other widget for serve the ColorMatrix
 *      Arguments: 	csb - the color selector widget.
 *                      cargs, cnum_args - a filtered arg list that was
 *                                       passed to create the color selector.
 *      Returns:       	none.
 */

/* ARGSUSED */
static void CreateColorMatrixWindow(XmColorSelectionBoxWidget csb, ArgList cargs, Cardinal cnum_args)
{
	Widget fr;
	Arg *margs, args[15];
	XtTranslations xlations;
	Cardinal ac;
	XtActionsRec actions;
	XtWidgetGeometry colorB_geom;
	XmString xms;

	Position x = 0;
	Position y = 5;

	Dimension height = 400;
	Dimension label_w = 40, label_h = 30;
	Dimension Text_w = 40, Text_h = 30;
	Dimension Color_w = 98, Color_h = 90;

	static char drawTranslations[] =
		"#override \n\
		<Motion>:DrawingAreaInput() ManagerGadgetButtonMotion()\n\
		<Btn1Down>:DrawingAreaInput() ManagerGadgetArm()\n\
		<Btn1Up>:DrawingAreaInput() ManagerGadgetActivate()";

	static char drawTranslations2[] =
		"#override \n\
		<Motion>:DrawingAreaInput2() ManagerGadgetButtonMotion()\n\
		<Btn1Down>:DrawingAreaInput2() ManagerGadgetArm()\n\
		<Btn1Up>:DrawingAreaInput2() ManagerGadgetActivate()";

	ac = 0;
	XtSetArg(args[ac], XmNborderWidth, 0); ac++;
	XtSetArg(args[ac], XmNx, 250); ac++;
	XtSetArg(args[ac], XmNy, 0); ac++;
	XtSetArg(args[ac], XmNwidth, MATRIX_WIDTH); ac++;
	XtSetArg(args[ac], XmNheight, height); ac++;
	XtSetArg(args[ac], XmNlayoutDirection, XmTOP_TO_BOTTOM_LEFT_TO_RIGHT); ac++;
	XtSetArg(args[ac], XmNresizePolicy, XmRESIZE_GROW); ac++;
	margs = XtMergeArgLists(args, ac, cargs, cnum_args);
	XmColorSB_MatrixBoard(csb)= XtCreateManagedWidget("MatrixBox",xmBulletinBoardWidgetClass,
			(Widget) csb, margs, cnum_args + ac);
	XtFree((XtPointer) margs);

	fr=XmColorSB_MatrixBoard(csb);

	ac = 0;
	XtSetArg(args[ac], XmNx, x); ac++;
	XtSetArg(args[ac], XmNy, y); ac++;
	XtSetArg(args[ac], XmNwidth, H_len+4); ac++;
	XtSetArg(args[ac], XmNheight, 260); ac++;
	XmColorSB_frameMatrix(csb) = XmCreateFrame(fr, (char *)"FrameMatrix", args,	ac);
	XtManageChild(XmColorSB_frameMatrix(csb));

	ac = 0;
	xlations = XtParseTranslationTable(drawTranslations);
	XtSetArg(args[ac], XmNtranslations, xlations); ac++;
	XtSetArg(args[ac], XmNtraversalOn, False); ac++;
	XtSetArg(args[ac], XmNnavigationType, XmNONE); ac++;
	XtSetArg(args[ac], XmNx, 2); ac++;
	XtSetArg(args[ac], XmNy, 2); ac++;
	XtSetArg(args[ac], XmNwidth, H_len); ac++;
	XtSetArg(args[ac], XmNheight, 256); ac++;
	XmColorSB_drawingAreaMatrix(csb) = XmCreateDrawingArea(XmColorSB_frameMatrix(csb), (char *)"ColorMatrix", args,	ac);
	XtManageChild(XmColorSB_drawingAreaMatrix(csb));

	XtAddCallback(XmColorSB_drawingAreaMatrix(csb), XmNinputCallback, cb_Input, csb);

	ac = 0;
	XtSetArg(args[ac], XmNx,x + H_len + 15); ac++;
	XtSetArg(args[ac], XmNy, y); ac++;
	XtSetArg(args[ac], XmNwidth, 20); ac++;
	XtSetArg(args[ac], XmNheight, S_len+2); ac++;
	XmColorSB_frameVolume(csb) = XmCreateFrame(fr, (char *)"FrameVolume", args, ac);
	XtManageChild(XmColorSB_frameVolume(csb));

	actions.string = "DrawingAreaInput2";
	actions.proc = trTable_Volume;
	XtAppAddActions(XtDisplayToApplicationContext(XtDisplay(XmColorSB_frameVolume(csb))), &actions, 1);

	ac = 0;
	xlations = XtParseTranslationTable(drawTranslations2);
	XtSetArg(args[ac], XmNtranslations, xlations); ac++;
	XtSetArg(args[ac], XmNx, 2); ac++;
	XtSetArg(args[ac], XmNy, 2); ac++;
	XtSetArg(args[ac], XmNtraversalOn, False); ac++;
	XtSetArg(args[ac], XmNnavigationType, XmNONE); ac++;
	XtSetArg(args[ac], XmNwidth, DRAW_VOLUME_WIDTH); ac++;
	XtSetArg(args[ac], XmNheight, S_len); ac++;
	XmColorSB_drawingAreaVolume(csb) = XmCreateDrawingArea(XmColorSB_frameVolume(csb), (char *)"Volume", args, ac);
	XtManageChild(XmColorSB_drawingAreaVolume(csb));

	XtAddCallback(XmColorSB_drawingAreaVolume(csb), XmNinputCallback, cb_Volume, csb);

	ac = 0;
	XtSetArg(args[ac], XmNx, x); ac++;
	XtSetArg(args[ac], XmNy, 15+ S_len+4); ac++;
	XtSetArg(args[ac], XmNwidth, Color_w); ac++;
	XtSetArg(args[ac], XmNheight,Color_h); ac++;
	XtSetArg(args[ac], XmNshadowType, XmSHADOW_ETCHED_OUT); ac++;
	XmColorSB_frameColor(csb) = XmCreateFrame(fr, (char *)"FrameColor", args, ac);
	XtManageChild(XmColorSB_frameColor(csb));

	ac = 0;
	XtSetArg(args[ac], XmNshadowThickness, 0); ac++;
	XmColorSB_color(csb) = XmCreateFrame(XmColorSB_frameColor(csb), (char *)"Color", args, ac);
	XtManageChild(XmColorSB_color(csb));

	ac = 0;
	xms=XmStringCreateLocalized(HUE_STRING);
	XtSetArg(args[ac], XmNlabelString, xms); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+5); ac++;
	XtSetArg(args[ac], XmNy, 20+S_len+4); ac++;
	XtSetArg(args[ac], XmNwidth, label_w); ac++;
	XmColorSB_labelHue(csb) = XmCreateLabel(fr, (char *)"LabelHue", args, ac);
	XtManageChild(XmColorSB_labelHue(csb));
	XmStringFree(xms);

	ac = 0;
	xms=XmStringCreateLocalized(SAT_STRING);
	XtSetArg(args[ac], XmNlabelString, xms); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+5); ac++;
	XtSetArg(args[ac], XmNy, 20+S_len+4+label_h); ac++;
	XtSetArg(args[ac], XmNwidth, label_w); ac++;
	XmColorSB_labelSat(csb) = XmCreateLabel(fr, (char *)"LabelSat", args, ac);
	XtManageChild(XmColorSB_labelSat(csb));
	XmStringFree(xms);

	ac = 0;
	xms=XmStringCreateLocalized(VAL_STRING);
	XtSetArg(args[ac], XmNlabelString, xms); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+5); ac++;
	XtSetArg(args[ac], XmNy, 20+S_len+4+2*label_h); ac++;
	XtSetArg(args[ac], XmNwidth, label_w); ac++;
	XmColorSB_labelVal(csb) = XmCreateLabel(fr, (char *)"LabelVal", args, ac);
	XtManageChild(XmColorSB_labelVal(csb));
	XmStringFree(xms);

	ac = 0;
	xms=XmStringCreateLocalized(RED_STRING);
	XtSetArg(args[ac], XmNlabelString, xms); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+label_w+Text_w+10); ac++;
	XtSetArg(args[ac], XmNy, 20+S_len+5); ac++;
	XtSetArg(args[ac], XmNwidth, label_w); ac++;
	XmColorSB_labelR(csb) = XmCreateLabel(fr, (char *)"LabelR", args, ac);
	XtManageChild(XmColorSB_labelR(csb));
	XmStringFree(xms);

	ac = 0;
	xms=XmStringCreateLocalized(GREEN_STRING);
	XtSetArg(args[ac], XmNlabelString, xms); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+label_w+Text_w+10); ac++;
	XtSetArg(args[ac], XmNy, 20+S_len+4+label_h); ac++;
	XtSetArg(args[ac], XmNwidth, label_w); ac++;
	XmColorSB_labelG(csb) = XmCreateLabel(fr, (char *)"LabelG", args, ac);
	XtManageChild(XmColorSB_labelG(csb));
	XmStringFree(xms);

	ac = 0;
	xms=XmStringCreateLocalized(BLUE_STRING);
	XtSetArg(args[ac], XmNlabelString, xms); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+label_w+Text_w+10); ac++;
	XtSetArg(args[ac], XmNy, 20+S_len+4+2*label_h);	ac++;
	XtSetArg(args[ac], XmNwidth, label_w); ac++;
	XmColorSB_labelB(csb) = XmCreateLabel(fr, (char *)"LabelB", args, ac);
	XtManageChild(XmColorSB_labelB(csb));
	XmStringFree(xms);

	ac = 0;
	XtSetArg(args[ac], XmNcursorPositionVisible, False); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+label_w+5); ac++;
	XtSetArg(args[ac], XmNy, 15+S_len+4); ac++;
	XtSetArg(args[ac], XmNborderWidth, 0); ac++;
	XtSetArg(args[ac], XmNwidth, Text_w); ac++;
	XmColorSB_hueF(csb) = XmCreateTextField(fr,	(char *)"HueF",	args, ac);
	XtManageChild(XmColorSB_hueF(csb));

	ac = 0;
	XtSetArg(args[ac], XmNcursorPositionVisible, False); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+label_w+5); ac++;
	XtSetArg(args[ac], XmNy, 15+S_len+4+label_h); ac++;
	XtSetArg(args[ac], XmNwidth, Text_w); ac++;
	XmColorSB_satF(csb) = XmCreateTextField(fr, (char *)"SatF",	args, ac);
	XtManageChild(XmColorSB_satF(csb));

	ac = 0;
	XtSetArg(args[ac], XmNcursorPositionVisible, False); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+label_w+5); ac++;
	XtSetArg(args[ac], XmNy, 15+S_len+4+2*label_h); ac++;
	XtSetArg(args[ac], XmNwidth, Text_w); ac++;
	XmColorSB_brightnessF(csb) = XmCreateTextField(fr, (char *)"BrightnessF", args,	ac);
	XtManageChild(XmColorSB_brightnessF(csb));

	ac = 0;
	XtSetArg(args[ac], XmNcursorPositionVisible, False); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+2*label_w+Text_w+10); ac++;
	XtSetArg(args[ac], XmNborderWidth, 0); ac++;
	XtSetArg(args[ac], XmNy, 15+S_len+4); ac++;
	XtSetArg(args[ac], XmNwidth, Text_w); ac++;
	XmColorSB_redF(csb) = XmCreateTextField(fr, (char *)"RedF", args, ac);
	XtManageChild(XmColorSB_redF(csb));

	ac = 0;
	XtSetArg(args[ac], XmNcursorPositionVisible, False); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+2*label_w+Text_w+10); ac++;
	XtSetArg(args[ac], XmNy, 15+S_len+4+label_h); ac++;
	XtSetArg(args[ac], XmNwidth, Text_w); ac++;
	XmColorSB_greenF(csb) = XmCreateTextField(fr, (char *)"GreenF",	args, ac);
	XtManageChild(XmColorSB_greenF(csb));

	ac = 0;
	XtSetArg(args[ac], XmNcursorPositionVisible, False); ac++;
	XtSetArg(args[ac], XmNx, x+Color_w+2*label_w+Text_w+10); ac++;
	XtSetArg(args[ac], XmNy, 15+S_len+4+2*label_h); ac++;
	XtSetArg(args[ac], XmNwidth, Text_w); ac++;
	XmColorSB_blueF(csb) = XmCreateTextField(fr, (char *)"BlueF", args, ac);
	XtManageChild(XmColorSB_blueF(csb));

	ac = 0;
	xms=XmStringCreateLocalized(ADD_COLOR_STRING);
	XtSetArg(args[ac], XmNlabelString, xms); ac++;
	XtSetArg(args[ac], XmNx, x); ac++;
	XtSetArg(args[ac], XmNy, 15+S_len+9+Color_h); ac++;
	XtSetArg(args[ac], XmNwidth, 258); ac++;
	XmColorSB_pButtonAddColor(csb) = XmCreatePushButton(fr, (char *)"AddColor", args, ac);
	XtManageChild(XmColorSB_pButtonAddColor(csb));
	XmStringFree(xms);

	XtAddCallback(XmColorSB_pButtonAddColor(csb), XmNactivateCallback, cb_AddColor, csb);

	XtAddCallback(XmColorSB_hueF(csb),XmNlosingFocusCallback, cb_ChangeCoordinateColor , csb);
	XtAddCallback(XmColorSB_satF(csb),XmNlosingFocusCallback, cb_ChangeCoordinateColor , csb);
	XtAddCallback(XmColorSB_brightnessF(csb),XmNlosingFocusCallback, cb_BrightnessChange , csb);
	XtAddCallback(XmColorSB_blueF(csb),XmNlosingFocusCallback, cb_ChangeColor , csb);
	XtAddCallback(XmColorSB_greenF(csb),XmNlosingFocusCallback, cb_ChangeColor , csb);
	XtAddCallback(XmColorSB_redF(csb),XmNlosingFocusCallback, cb_ChangeColor , csb);

	XtAddCallback(XmColorSB_hueF(csb),XmNfocusCallback, cb_AppearCursor, csb);
	XtAddCallback(XmColorSB_satF(csb),XmNfocusCallback, cb_AppearCursor, csb);
	XtAddCallback(XmColorSB_brightnessF(csb),XmNfocusCallback, cb_AppearCursor , csb);
	XtAddCallback(XmColorSB_blueF(csb),XmNfocusCallback, cb_AppearCursor, csb);
	XtAddCallback(XmColorSB_greenF(csb),XmNfocusCallback, cb_AppearCursor, csb);
	XtAddCallback(XmColorSB_redF(csb),XmNfocusCallback, cb_AppearCursor, csb);
}

/*      Function Name: 	CreateColorPaletteWindow
 *      Description:   	creates a palette in a BulletinBoard  to display the
 *			colors pallette.
 *      Arguments: 	csb - the color selector widget.
 *                      cargs, cnum_args - a filtered arg list that was
 *                                       passed to create the color selector.
 *      Returns:       	none.
 */

/* ARGSUSED */
static void CreateColorPaletteWindow(XmColorSelectionBoxWidget csb, ArgList cargs, Cardinal cnum_args)
{
	Widget fr, labelPalette, labelCustomColors;
	Arg *margs, args[10];
	Cardinal ac;
  XmString local_xmstring ;
	
  static unsigned long arr_color[]= { 0x000000, 0xaa0000, 0x005500, 0xaa5500, 0x00aa00, 0xaaaa00, 0x00ff00, 0xaaff00,
	        0x00007f, 0xaa007f, 0x00557f, 0xaa557f, 0x00aa7f, 0xaaaa7f, 0x00ff7f, 0xaaff7f, 0x0000ff, 0xaa00ff,
	        0x0055ff, 0xaa55ff, 0x00aaff, 0xaaaaff, 0x00ffff, 0xaaffff, 0x550000, 0xff0000, 0x555500, 0xff5500,
	        0x55aa00, 0xffaa00, 0x55ff00, 0xffff00, 0x55007f, 0xff007f, 0x55557f, 0xff557f, 0x55aa7f, 0xffaa7f,
	        0x55ff7f, 0xffff7f, 0x5500ff, 0xff00ff, 0x5555ff, 0xff55ff, 0x55aaff, 0xffaaff, 0x55ffff, 0xffffff };

	Dimension height = 400;
	Dimension width = 250;
	int j;
	char s[25];
	Position xx=10, yy=30;

	ac = 0;
	XtSetArg(args[ac], XmNx, 0); ac++;
	XtSetArg(args[ac], XmNy, 0); ac++;
	XtSetArg(args[ac], XmNwidth, width); ac++;
	XtSetArg(args[ac], XmNheight, height); ac++;
	margs = XtMergeArgLists(args, ac, cargs, cnum_args);
	XmColorSB_ColorBoard(csb)= XtCreateManagedWidget("PaletteBox",xmBulletinBoardWidgetClass, (Widget) csb,	margs, cnum_args + ac);
	XtFree((XtPointer) margs);

	fr=XmColorSB_ColorBoard(csb);

	for (j=0; j<COUNT_PALETTE_COLORS; j++) {
		snprintf(s, sizeof(s), "textFieldColored%d", j);
		ColorFieldCreate(fr, &XmColorSB_ColoredFieldWidget(csb)[j], s, arr_color[j], xx, yy);
		if (xx>=220) {
			xx=10;
			yy=yy+30;
		} else {
			xx=xx+30;
		}
	}

	xx=10, yy=230;
	for (j=0; j<COUNT_CUSTOM_COLORS; j++) {
		snprintf(s, sizeof(s), "textField%d", j);
		ColorFieldCreate(fr, &XmColorSB_CustomFieldWidget(csb)[j], s, 0xffffff, xx, yy);
		if (xx>=220) {
			xx=10;
			yy=yy+30;
		} else {
			xx=xx+30;
		}
	}

	LoadCustomColors(fr, csb);
	local_xmstring = XmStringCreate(BASIC_COLOR_STRING,
					XmFONTLIST_DEFAULT_TAG);

	ac = 0;
	XtSetArg(args[ac], XmNlabelString,local_xmstring ); ac++;
	XtSetArg(args[ac], XmNalignment, XmALIGNMENT_BEGINNING); ac++;
	XtSetArg(args[ac], XmNx, 0); ac++;
	XtSetArg(args[ac], XmNy, 0); ac++;
	XtSetArg(args[ac], XmNwidth, 100); ac++;
	XtSetArg(args[ac], XmNheight, 15); ac++;
	labelPalette = XmCreateLabel(fr, (char *)"LabelPalette", args, ac);
	XtManageChild(labelPalette);
	XmStringFree(local_xmstring);

	local_xmstring = XmStringCreate(CUSTOM_COLOR_STRING,
					XmFONTLIST_DEFAULT_TAG);

	ac = 0;
	XtSetArg(args[ac], XmNlabelString,local_xmstring ); ac++;
	XtSetArg(args[ac], XmNalignment, XmALIGNMENT_BEGINNING); ac++;
	XtSetArg(args[ac], XmNx, 0); ac++;
	XtSetArg(args[ac], XmNy, 210); ac++;
	XtSetArg(args[ac], XmNwidth, 100); ac++;
	XtSetArg(args[ac], XmNheight, 15); ac++;
	labelCustomColors = XmCreateLabel(fr, (char *)"LabelCustomColors", args, ac);
	XtManageChild(labelCustomColors);
	XmStringFree(local_xmstring);

	XmColorSB_labelPalette(csb)=labelPalette;
	XmColorSB_labelCustomColors(csb)=labelCustomColors;
}

void _XmSelectionBoxCreateButtons(XmColorSelectionBoxWidget csb)
{
	XmColorSB_pButtonOk(csb) = _XmBB_CreateButtonG( (Widget) csb, csb->cs.ok_label_string, "OK",
			XmOkStringLoc);
	XtRemoveAllCallbacks( XmColorSB_pButtonOk(csb), XmNactivateCallback) ;
	XtAddCallback(XmColorSB_pButtonOk(csb), XmNarmCallback, cb_Ok, csb);
	XtAddCallback(XmColorSB_pButtonOk(csb), XmNactivateCallback,
	ColorSelectionBoxCallback, (XtPointer) XmDIALOG_OK_BUTTON);

	XmColorSB_pButtonCancel(csb)= _XmBB_CreateButtonG( (Widget) csb, csb->cs.cancel_label_string, "Cancel",
			XmCancelStringLoc);
	
	XtAddCallback(XmColorSB_pButtonCancel(csb), XmNactivateCallback,
	ColorSelectionBoxCallback, (XtPointer) XmDIALOG_CANCEL_BUTTON);

	XmColorSB_pButtonHelp(csb) = _XmBB_CreateButtonG( (Widget) csb, csb->cs.help_label_string, "Help",
			XmHelpStringLoc);
	XtRemoveAllCallbacks( XmColorSB_pButtonHelp(csb), XmNactivateCallback) ;
	XtAddCallback(XmColorSB_pButtonHelp(csb), XmNactivateCallback,
	ColorSelectionBoxCallback, (XtPointer) XmDIALOG_HELP_BUTTON);
	Arg args[10];
	Cardinal ac=0;
	XtSetArg(args[ac],XmNx,100); ac++;
	XtSetArg(args[ac],XmNy,410); ac++;
	XtSetArg(args[ac],XmNwidth,70); ac++;
	XtSetArg(args[ac],XmNheight,40); ac++;

	XtSetValues(XmColorSB_pButtonHelp(csb), args, ac);
	XtSetValues(XmColorSB_pButtonCancel(csb), args, ac);

	XtVaSetValues(XmColorSB_pButtonOk(csb), XmNx,100, XmNy, 410, XmNshowAsDefault, 1, NULL);
	XtVaSetValues(XmColorSB_pButtonOk(csb), XmNwidth,70, XmNheight, 40, NULL);

	XtVaSetValues(XmColorSB_pButtonCancel(csb), XmNx,250, XmNy, 410, NULL);
	XtVaSetValues(XmColorSB_pButtonCancel(csb), XmNwidth,70, XmNheight, 40, NULL);

	XtVaSetValues(XmColorSB_pButtonHelp(csb), XmNx,400, XmNy, 410, NULL);
	XtVaSetValues(XmColorSB_pButtonHelp(csb), XmNwidth,70, XmNheight, 40, NULL);

	BB_DefaultButton(csb) = XmColorSB_pButtonOk(csb);
	_XmBulletinBoardSetDynDefaultButton((Widget)csb, BB_DefaultButton(csb));

	XtManageChild(XmColorSB_pButtonOk(csb));
	XtManageChild(XmColorSB_pButtonCancel(csb));
	if (XmColorSB_helpOn(csb))
			XtManageChild(XmColorSB_pButtonHelp(csb));

	return;
}

/* ARGSUSED */
static void CreateSeparatorAndButtons(XmColorSelectionBoxWidget csb, ArgList cargs, Cardinal cnum_args)
{
	Arg args[10];
	Cardinal ac;
	XtWidgetGeometry colorB_geom;

	(void) XtQueryGeometry(XmColorSB_ColorBoard(csb), NULL, &colorB_geom);

	Position y = (colorB_geom.height + 2 * colorB_geom.border_width)+10;

	ac=0;
	XtSetArg(args[ac], XmNx, 0); ac++;
	XtSetArg(args[ac], XmNy, y); ac++;
	XtSetArg(args[ac], XmNwidth, 600); ac++;
	XmColorSB_separator(csb) = XmCreateSeparatorGadget((Widget) csb, (char *)"Separator", args, ac);
	XtManageChild(XmColorSB_separator(csb));

	_XmSelectionBoxCreateButtons(csb);
}

static void LoadCustomColors(Widget parent, XmColorSelectionBoxWidget csb)
{
	unsigned long p;
	Pixel pix;
	FILE *ColorFile;
	int i;
	String env;
	char *l, *l2;
	char path[255];
	char k[3];
	XColor screen_in_out;
	int status;
	int r, g, b;

	env = getenv("HOME");
	sprintf(path, "%s/.custom_colors", env);
	ColorFile=fopen(path, "r");
	if (ColorFile!=NULL) {
		l = malloc(9);
		if (XmColorSB_widget_N(csb)<0 || XmColorSB_widget_N(csb)>=15)
			XmColorSB_widget_N(csb)=0;
		i=0;
		while (fgets(l, 8, ColorFile) && i<=widgetN) {
			if (l[0]!=0) {
				strncpy(k, l, 2);
				k[2]='\0';
				p = strtoul(k, NULL, 16);
				r=(int)p;
				strncpy(k, l+2, 2);
				k[2]='\0';
				p = strtoul(k, NULL, 16);
				g=(int)p;
				strncpy(k, l+4, 2);
				k[2]='\0';
				p = strtoul(k, NULL, 16);
				b=(int)p;

				screen_in_out.red =r<<8;
				screen_in_out.green=g<<8;
				screen_in_out.blue =b<<8;

				status = XAllocColor(XtDisplay(csb), csb->core.colormap, &screen_in_out);
				if (status) {
					Arg arg = { XmNbackground, screen_in_out.pixel };
					XtSetValues(XmColorSB_CustomFieldWidget(csb)[i], &arg, 1);
				}
				i++;
			} else
				break;
		}
		if (i<0 || i>15)
			XmColorSB_widget_N(csb)=0;
		else
			XmColorSB_widget_N(csb)=i;
		free(l);
		fclose(ColorFile);
	}
}

static void ReDrawCrossPixel(XmColorSelectionBoxWidget csb, Position y, Position x, Visual *visual)
{
	char s[10];
	unsigned long p;
	XColor screen_in_out;
	Colormap colormap;
	int status;
	int R, G, B;

	R=XmColorSB_arr_col(csb)[y][x].r;
	G=XmColorSB_arr_col(csb)[y][x].g;
	B=XmColorSB_arr_col(csb)[y][x].b;
	StorePixel(visual, XtScreen(csb), R, G, B, x + (255-y)*XmColorSB_im1(csb)->width,
	XmColorSB_im1(csb)->data);
}

static void DrawCross(XmColorSelectionBoxWidget csb, Position x, Position y)
{
	static Position x_old, y_old;
	unsigned long BLACK= 0x000000;
	Display *disp;
	Visual *visual=NULL;
	Widget parent_shell = (Widget)csb;
	int i, j;
	while (!XtIsShell(parent_shell))
		parent_shell = XtParent(parent_shell);
	XtVaGetValues(parent_shell, XmNvisual, &visual, NULL);
	if (!visual)
		visual = DefaultVisualOfScreen(XtScreen(parent_shell));

	for (j = MAX(y_old - CROSS_LINE_WIDTH/2, 0); j <= MIN(y_old + CROSS_LINE_WIDTH/2, S_len-1); j++)
		for (i = MAX(x_old - CROSS_SIZE, 0); i <= MIN(x_old + CROSS_SIZE, H_len-1); i++)
			if( x_old > i + CROSS_CENTER_SIZE || x_old < i - CROSS_CENTER_SIZE ) 
  			ReDrawCrossPixel(csb, j, i, visual);
	
	for (j = MAX(x_old - CROSS_LINE_WIDTH/2, 0); j <= MIN(x_old + CROSS_LINE_WIDTH/2, H_len-1); j++)
		for (i = MAX(y_old - CROSS_SIZE, 0); i <= MIN(y_old + CROSS_SIZE, S_len-1); i++)
			if( y_old > i + CROSS_CENTER_SIZE || y_old < i - CROSS_CENTER_SIZE ) 
  			ReDrawCrossPixel(csb, i, j, visual);	
	
	{
		static int Y;
		Y=255-y;
		
		for (j = MAX(Y - CROSS_LINE_WIDTH/2, 0); j <= MIN(Y + CROSS_LINE_WIDTH/2, S_len-1); j++)
			for (i = MAX(x - CROSS_SIZE, 0); i <= MIN(x + CROSS_SIZE, H_len-1); i++)
				if( x > i + CROSS_CENTER_SIZE || x < i - CROSS_CENTER_SIZE ) 
          XPutPixel(XmColorSB_im1(csb), i, j, BLACK);
		
		for (j = MAX(x - CROSS_LINE_WIDTH/2, 0); j <= MIN(x + CROSS_LINE_WIDTH/2, H_len-1); j++)
			for (i = MAX(Y - CROSS_SIZE, 0); i <= MIN(Y + CROSS_SIZE, S_len-1); i++)
				if( Y > i + CROSS_CENTER_SIZE || Y < i - CROSS_CENTER_SIZE ) 
          XPutPixel(XmColorSB_im1(csb), j, i, BLACK);		
	}

  x_old=x;
	y_old=y;
	
  XPutImage(XtDisplay(csb), XtWindow(XmColorSB_drawingAreaMatrix(csb)),XmColorSB_gc(csb), XmColorSB_im1(csb), 0,0,0,0, H_len, S_len);
}

static void FillColorFields(XmColorSelectionBoxWidget csb)
{
	float k;
	char s[10];
	k=(float)XmColorSB_brightness_volume(csb)/(S_len-1);
	sprintf(s, "%d", (int)((float)XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].r * k));
	XmTextFieldSetString(XmColorSB_redF(csb), s);
	sprintf(s, "%d", (int)((float)XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].g * k));
	XmTextFieldSetString(XmColorSB_greenF(csb), s);
	sprintf(s, "%d", (int)((float)XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].b * k));
	XmTextFieldSetString(XmColorSB_blueF(csb), s);
}

static int DugitCut(Widget w)
{
	char *s, *ss;
	static int zz=0, max_zz;
	ss=XtName(w);

	if (!strcmp("HueF", ss))
		max_zz=H_len-1;
	else
		max_zz=S_len-1;
	s = XmTextFieldGetString(w);
	zz=atoi(s);
	XtFree(s);

	if (zz>max_zz)
		zz=max_zz;
	if (zz<0)
		zz=0;
	s = malloc(100);
	sprintf(s, "%d", zz);

	XmTextFieldSetString(w, s);

	if (s)
		free(s);
	return (zz);
}

static void LoadColorValues(XmColorSelectionBoxWidget csb)
{
	unsigned long p;
	int i;
	char *v;

	XtVaGetValues(XmColorSB_color(csb), XmNbackground, &p, NULL);
	XmColorSB_color_value(csb)=p;

	v = XmTextFieldGetString(XmColorSB_blueF(csb));
	XmColorSB_blue_value(csb)=atoi(v);
	XtFree(v);

	v = XmTextFieldGetString(XmColorSB_greenF(csb));
	XmColorSB_green_value(csb)=atoi(v);
	XtFree(v);

	v = XmTextFieldGetString(XmColorSB_redF(csb));
	XmColorSB_red_value(csb)=atoi(v);
	XtFree(v);
}

static void CalculateColor(XmColorSelectionBoxWidget csb, Position x, Position y)
{
	float k=0;
	unsigned long p;
	char s[10];
	XColor screen_in_out;
	int status;

	if (x < 0)
		x = 0;
	if (x >= H_len)
		x = H_len-1;
	if (y < 0)
		y = 0;
	if (y > S_len-1)
		y = S_len-1;

	XmColorSB_x(csb) = x;
	XmColorSB_y(csb) = y;
	sprintf(s, "%d", x);
	XmTextFieldSetString(XmColorSB_hueF(csb), s);

	sprintf(s, "%d", y);
	XmTextFieldSetString(XmColorSB_satF(csb), s);

	FillColorFields(csb);

	sprintf(s, "%d", XmColorSB_brightness_volume(csb));
	XmTextFieldSetString(XmColorSB_brightnessF(csb), s);

	DrawCross(csb, x, y);

	CreateVolumeGradient(csb, NULL);
}

static void ColorSelectionBoxCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
	unsigned char which_button = (unsigned char)(long) client_data;
	XmColorSelectionBoxWidget csb = (XmColorSelectionBoxWidget) XtParent (w);
	XmAnyCallbackStruct *callback = (XmAnyCallbackStruct *) call_data;
	XmAnyCallbackStruct temp;
	temp.event = callback->event;

	switch (which_button) {
	case XmDIALOG_OK_BUTTON:
		temp.reason = XmCR_OK;
		XtCallCallbackList((Widget) csb, csb->cs.ok_callback, (XtPointer) &temp);
		break;

	case XmDIALOG_CANCEL_BUTTON:
		temp.reason = XmCR_CANCEL;
		XFlush(XtDisplay (csb));
		XtCallCallbackList((Widget) csb, csb->cs.cancel_callback, &temp);
		break;

	case XmDIALOG_HELP_BUTTON:
		/* Invoke the help system. */
		_XmManagerHelp((Widget)csb, callback->event, NULL, NULL) ;
		break;
	}
	return;
}

static void cb_AppearCursor(Widget w, XtPointer csb_ptr, XtPointer call_data)
{
	Arg arg = { XmNcursorPositionVisible, True };
	XtSetValues(w, &arg, 1);
}

/*      Function Name: 	cb_ChangeColor
 *      Description:   	Function which convert from RGV to HSV.
 *      Arguments: 	w - the color selector widget.
 *      Returns:       	none.
 */

/*{{{ CHANGE COLOR*/
static void cb_ChangeColor(Widget w, XtPointer csb_ptr, XtPointer call_data)
{
	XmColorSelectionBoxWidget csb;
	csb = (XmColorSelectionBoxWidget) csb_ptr;

	char *sr, *sg, *sb;
	char s[10];
	int ir, ig, ib;
	float k;
	int S = 0 , H = 0, Q, i;
	int precision, new_pecision;
	if (w) {
		Arg arg = { XmNcursorPositionVisible, False };
		XtSetValues(w, &arg, 1);
		DugitCut(w);
	}

	Q = H_len / 6;
	sr = XmTextFieldGetString(XmColorSB_redF(csb));
	ir = atoi(sr);
	XtFree(sr);
	sg = XmTextFieldGetString(XmColorSB_greenF(csb));
	ig = atoi(sg);
	XtFree(sg);
	sb = XmTextFieldGetString(XmColorSB_blueF(csb));
	ib = atoi(sb);
	XtFree(sb);
	k = (float)XmColorSB_brightness_volume(csb) / (S_len-1);
	/*! We will compare R G B colors, the heightest color = brightness_volume therefore we compare
	 three times for find brightness_volume
	 S (saturation) = 255 - smallest color value (accounting with coefficient )
	 after that We finding  more fitting H (Hue) in loop*/
	if ((int)(XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].r * k) != ir
			|| ((int)(XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].g * k) != ig)
			|| ((int)(XmColorSB_arr_col(csb)[XmColorSB_y(csb)][XmColorSB_x(csb)].b * k) != ib)) {
		if (ir == ig && ig == ib) {
			XmColorSB_brightness_volume(csb) = ir;
			S = 0;
			H = 0;
		} else {
			if (ir>=ig && ir>ib) {
				XmColorSB_brightness_volume(csb)=ir;
				k=(float)ir/255;
				ir = 255;
				ig = (int) ((float)ig/k);
				ib = (int) ((float)ib/k);
				if (ig==ib) {
					S=255-ig;
					H=0;
				}
				if (ig>ib) {
					S=255-ib;
					H=i=0;
					precision = abs(XmColorSB_arr_col(csb)[S][i].g-ig);
					for (i=1; i<Q && precision!=0; i++) {
						new_pecision= abs(XmColorSB_arr_col(csb)[S][i].g-ig);
						if (precision > new_pecision) {
							precision=new_pecision;
							H=i;
						}
					}

				} else {
					S=255-ig;
					H=i=Q*5;
					precision = abs(XmColorSB_arr_col(csb)[S][i].b-ib);
					for (i=Q*5+1; i<Q*6 && precision!=0; i++) {
						new_pecision= abs(XmColorSB_arr_col(csb)[S][i].b-ib);
						if (precision > new_pecision) {
							precision=new_pecision;
							H=i;
						}
					}
				}
			}
			if (ig>ir && ig>=ib) {
				XmColorSB_brightness_volume(csb)=ig;
				k=(float)ig/255;
				ig = 255;
				ir = (int) ((float)ir/k);
				ib = (int) ((float)ib/k);
				if (ir==ib) {
					S=255-ig;
					H=120;
				}
				if (ir>ib) {
					S=255-ib;
					H=i=Q;
					precision = abs(XmColorSB_arr_col(csb)[S][i].r-ir);
					for (i=Q+1; i<Q*2 && precision!=0; i++) {
						new_pecision= abs(XmColorSB_arr_col(csb)[S][i].r-ir);
						if (precision > new_pecision) {
							precision=new_pecision;
							H=i;
						}
					}
				} else {
					S=255-ir;
					H=i=Q*2;
					precision = abs(XmColorSB_arr_col(csb)[S][i].b-ib);
					for (i=Q*2+1; i<Q*3 && precision!=0; i++) {
						new_pecision= abs(XmColorSB_arr_col(csb)[S][i].b-ib);
						if (precision > new_pecision) {
							precision=new_pecision;
							H=i;
						}
					}
				}
			}
			if (ib>ig && ib>=ir) {
				XmColorSB_brightness_volume(csb)=ib;
				k=(float)ib/255;
				ib = 255;
				ir = (int) ((float)ir/k);
				ig = (int) ((float)ig/k);
				if (ir==ib) {
					S=255-ig;
					H=240;
				}
				if (ig>ir) {
					S=255-ir;
					H=i=Q*3;
					precision = abs(XmColorSB_arr_col(csb)[S][i].g-ig);
					for (i=Q*3+1; i<Q*4 && precision!=0; i++) {
						new_pecision= abs(XmColorSB_arr_col(csb)[S][i].g-ig);
						if (precision > new_pecision) {
							precision=new_pecision;
							H=i;
						}
					}
				} else {
					S=255-ig;
					H=i=Q*4;
					precision = abs(XmColorSB_arr_col(csb)[S][i].r-ir);
					for (i=Q*4+1; i<Q*5 && precision!=0; i++) {
						new_pecision= abs(XmColorSB_arr_col(csb)[S][i].r-ir);
						if (precision > new_pecision) {
							precision=new_pecision;
							H=i;
						}
					}
				}
			}
		}
		sprintf(s, "%d", H);
		XmTextFieldSetString(XmColorSB_hueF(csb), s);
		sprintf(s, "%d", S);
		XmTextFieldSetString(XmColorSB_satF(csb), s);
		sprintf(s, "%d", XmColorSB_brightness_volume(csb));
		XmTextFieldSetString(XmColorSB_brightnessF(csb), s);
		XmColorSB_x(csb)=H;
		XmColorSB_y(csb)=S;
		DrawCross(csb, H, S);
		CreateVolumeGradient(csb, NULL);
		LoadColorValues(csb);
	}
}
/*}}}*/

static void cb_ChangeCoordinateColor(Widget w, XtPointer csb_ptr, XtPointer call_data)
{
	XmColorSelectionBoxWidget csb;
	csb = (XmColorSelectionBoxWidget) csb_ptr;
	Arg arg = { XmNcursorPositionVisible, False };
	int dig;

	XtSetValues(w, &arg, 1);

	dig=DugitCut(w);
	if (!strcmp("HueF", XtName(w))) {
		if (XmColorSB_x(csb)!=dig) {
			XmColorSB_x(csb)=dig;
			FillColorFields(csb);
			DrawCross(csb, XmColorSB_x(csb), XmColorSB_y(csb));
		}
	}

	if (!strcmp("SatF", XtName(w))) {
		if (XmColorSB_y(csb)!=dig) {
			XmColorSB_y(csb)=dig;
			FillColorFields(csb);
			DrawCross(csb, XmColorSB_x(csb), XmColorSB_y(csb));
		}
	}

	CreateVolumeGradient(csb, NULL);
	LoadColorValues(csb);
}

static void cb_Volume(Widget w, XtPointer csb_ptr, XtPointer call_data)
{
	XmDrawingAreaCallbackStruct *inputCallback;
	inputCallback = (XmDrawingAreaCallbackStruct *) call_data;

	trTable_Volume(w, inputCallback->event, NULL, NULL);
}

static void trTable_Volume(Widget w, XEvent *event, String *args, Cardinal *num_args)
{
	char s[5];
	XmDrawingAreaCallbackStruct *inputCallback;
	XmColorSelectionBoxWidget csb;
	Position x, y;
	static int btnPress = 0;
	csb = (XmColorSelectionBoxWidget) (XtParent(XtParent(XtParent(w))));

	XButtonEvent *bevent = (XButtonEvent *) event;

	if (bevent->type == ButtonPress) {
		y=event->xbutton.y;
		DrawVolume(csb, (S_len-1) - y);
		FillColorFields(csb);
		btnPress = 1;
	}
	if (btnPress && bevent->type == MotionNotify) {
		y=event->xbutton.y;
		DrawVolume(csb, (S_len-1) - y);
		FillColorFields(csb);
	}
	if (bevent->type == ButtonRelease)
		btnPress = 0;
}

static void cb_AddColor(Widget w, XtPointer csb_ptr, XtPointer call_data)
{
	XmColorSelectionBoxWidget csb;
	csb = (XmColorSelectionBoxWidget) csb_ptr;
	Pixel p;
	Arg arg[1];

	XtSetArg(arg[0],XmNbackground, &p);
	XtGetValues(XmColorSB_color(csb), arg, 1);
	XtSetArg(arg[0], XmNbackground, p);
	XtSetValues(XmColorSB_CustomFieldWidget(csb)[XmColorSB_widget_N(csb)], arg, 1);
	if (XmColorSB_widget_N(csb)>=widgetN)
		XmColorSB_widget_N(csb)=0;
	else
		XmColorSB_widget_N(csb)++;
}

static void cb_Input(Widget w, XtPointer csb_ptr, XtPointer call_data)
{
	XmDrawingAreaCallbackStruct *inputCallback;
	XmColorSelectionBoxWidget csb;
	Position x, y;
	int current;
	static int btnPress = 0;

	csb = (XmColorSelectionBoxWidget) csb_ptr;
	inputCallback = (XmDrawingAreaCallbackStruct *) call_data;

	switch (inputCallback->reason) {
	case XmCR_INPUT:
		if (inputCallback->event->type == ButtonPress) {
			x = inputCallback->event->xbutton.x;
			y = 255- inputCallback->event->xbutton.y;
			CalculateColor(csb, x, y);
			btnPress = 1;
			LoadColorValues(csb);
		}
		if (btnPress && inputCallback->event->type == MotionNotify) {
			x = inputCallback->event->xbutton.x;
			y = 255- inputCallback->event->xbutton.y;
			CalculateColor(csb, x, y);
			LoadColorValues(csb);
		}
		if (inputCallback->event->type == ButtonRelease)
			btnPress = 0;
		break;
	}
}

static void cb_BrightnessChange(Widget w, XtPointer csb_ptr, XtPointer call_data)
{
	static int i;

	XmColorSelectionBoxWidget csb;
	csb = (XmColorSelectionBoxWidget) csb_ptr;
	if (!strcmp(XtName(w), "BrightnessF")) {
		Arg arg= { XmNcursorPositionVisible, False };
		XtSetValues(w, &arg, 1);
		i= DugitCut(w);
		XmColorSB_brightness_volume(csb) = i;

		FillColorFields(csb);
		DrawVolume(csb, XmColorSB_brightness_volume(csb));
	}
}

static void cb_PutColor(Widget w, XEvent *event, String *args, Cardinal *num_args)
{
	int r, g, b;
	char s[10];
	XmColorSelectionBoxWidget csb;
	csb = (XmColorSelectionBoxWidget) XtParent(XtParent(w));

	unsigned long p;
	XtVaGetValues(w, XtNbackground, &p, NULL);

	r = p&0xff0000;
	g = p&0x00ff00;
	b = p&0x0000ff;

	r = r>>16;
	g = g>>8;

	sprintf(s, "%d", r);
	XmTextFieldSetString(XmColorSB_redF(csb), s);
	sprintf(s, "%d", g);
	XmTextFieldSetString(XmColorSB_greenF(csb), s);
	sprintf(s, "%d", b);
	XmTextFieldSetString(XmColorSB_blueF(csb), s);

	cb_ChangeColor(NULL, csb , NULL);
}

static void cb_Ok(Widget w, XtPointer csb_ptr, XtPointer call_data)
{
	FILE *ColorFile;
	String env;
	char path[255];
	XmColorSelectionBoxWidget csb;
	int i;
	unsigned int p;
	Arg arg = { XmNbackground, (XtArgVal)&p };

	csb = (XmColorSelectionBoxWidget) csb_ptr;
	XmSelectionBoxCallbackStruct temp;

	LoadColorValues(csb);
	/*! Save custom colors to file*/
	env = getenv("HOME");
	sprintf(path, "%s/.custom_colors", env);
	ColorFile=fopen(path, "w");
	if (!ColorFile) {
		perror("fopen");
		exit(1);
	}
	for (i=0; i<=widgetN; i++) {
		XtGetValues(XmColorSB_CustomFieldWidget(csb)[i], &arg, 1);
		if (p!=0xffffff)
			fprintf(ColorFile, "%06x\n", p);
	}
	fclose(ColorFile);

}

/************************************************************
 *      LOCAL CODE
 ************************************************************/

/*	Function Name: CalcPreferredSize
 *	Description: Calculates the size this widget would prefer to be.
 *	Arguments: csb - the color selector widget.
 *  RETURNED       width, height - preferred size of the color selector.
 *	Returns: none.
 */

static void CalcPreferredSize(XmColorSelectionBoxWidget csb, Dimension *width, Dimension *height)
{
	*height = CS_HEIGHT;
	*width = CS_WIDTH;
}

/****************************************************************
 * Geometry layout fixup routine for the Matrix and Palette box.  This
 *   routine reduces the preferred width of the Matrix box widget according
 *   to the length of the Palette box.
 * This algorithm assumes that each row has at least one box.
 ****************/

/*!	Function Name: BoxFix
 *	Description: Function which create individual geometry calculating for ColorSelectionBox.
 *	Returns: none.
 */

static void BoxFix(XmGeoMatrix geoSpec, int action, XmGeoMajorLayout layoutPtr, XmKidGeometry rowPtr)
{
	XmColorSelectionBoxWidget csb = (XmColorSelectionBoxWidget)geoSpec->composite ;
	XmKidGeometry matrixBoxGeo;
	XmKidGeometry paletteBoxGeo;
	Dimension h;
	Dimension w;
	Position X;
	int W;
	int i, j = 0;
	Arg args[10];
	Cardinal ac = 0;

	if (LayoutIsRtoLM(geoSpec->composite)) {
		matrixBoxGeo = rowPtr++;
		paletteBoxGeo = rowPtr;
	} else {
		paletteBoxGeo = rowPtr++;
		matrixBoxGeo = rowPtr;
	}

	if ( !matrixBoxGeo->kid) { /* Only one widget in this row, so do nothing.
	 */
		return;
	}

	W= csb->core.width;
	matrixBoxGeo->box.width = MATRIX_WIDTH;
	matrixBoxGeo->box.x = W - MATRIX_WIDTH - 5 - csb->bulletin_board.margin_width;
	if (W<MIN_CORE_WIDTH) {
		W= csb->core.width = MIN_CORE_WIDTH;
	} else {
		/*! 3*csb->bulletin_board.margin_width because we need margin at left, right and middle */

		paletteBoxGeo->box.width = W - MATRIX_WIDTH - 5/*! for good looking :)*/- 3*csb->bulletin_board.margin_width;
	}

	h = (int)(paletteBoxGeo->box.height - COLOR_FRAME_HEIGHT)/10;
	w = (int)(paletteBoxGeo->box.width- COLOR_FRAME_WIDTH)/8;

	/*! Calculate x*/
	/*! Calculate geometry for colored frames*/
	/*! j=7 it is count rows 0..7 = 8*/
	for (i = 0; i < COUNT_PALETTE_COLORS; i++) {
		XtSetArg(args[0], XmNx, (int)(w*j)+10);
		XtSetValues(XmColorSB_ColoredFieldWidget(csb)[i], args, 1);
		XtVaGetValues(XmColorSB_ColoredFieldWidget(csb)[i], XmNx, &X, NULL);
		if (j==7)
			j=0;
		else
			j++;
	}

	/*! Calculate geometry for custom frames*/
	for (i=0; i<COUNT_CUSTOM_COLORS; i++) {
		XtSetArg(args[0], XmNx, w*j+10);
		XtSetValues(XmColorSB_CustomFieldWidget(csb)[i], args, 1);
		if (j==7)
			j=0;
		else
			j++;
	}

	j=0;
	XtSetArg(args[0],XmNy, h*j+10);
	XtSetValues(XmColorSB_labelPalette(csb), args, 1);

	/*! Calculate y*/
	div_t q;
	for (i=0; i<COUNT_PALETTE_COLORS; i++) {
		q=div(i, 8);
		if (q.rem==0)
			j++;
		XtSetArg(args[0], XmNy, h*j);
		XtSetValues(XmColorSB_ColoredFieldWidget(csb)[i], args, 1);

	}
	j++;
	XtSetArg(args[0],XmNy, h*j+10);
	XtSetValues(XmColorSB_labelCustomColors(csb), args, 1);
	for (i=0; i<COUNT_CUSTOM_COLORS; i++) {
		q=div(i, 8);
		if (q.rem==0)
			j++;
		XtSetArg(args[0], XmNy, h*j);
		XtSetValues(XmColorSB_CustomFieldWidget(csb)[i], args, 1);

	}

	return;
}

/****************************************************************/
static Boolean ColorSelectionBoxNoGeoRequest(XmGeoMatrix geoSpec)
{
	if ( BB_InSetValues( geoSpec->composite)
	&& (XtClass( geoSpec->composite) == xmColorSelectionBoxWidgetClass)) {
		return ( TRUE);
	}
	return ( FALSE);
}

/****************************************************************
 * Get Geo matrix filled with kid widgets.
 ****************/

/*!	Function Name: ColorSelectionBoxGeoMatrixCreate
 *	Description: Function which fill XmGeoRowLayout and XmKidGeometry structures
 which will uses in geometry building.
 *	Returns: none.
 */

static XmGeoMatrix ColorSelectionBoxGeoMatrixCreate(Widget wid, Widget instigator, XtWidgetGeometry *desired)
{
	XmColorSelectionBoxWidget csb = (XmColorSelectionBoxWidget) wid ;
	XmGeoMatrix geoSpec;
	register XmGeoRowLayout layoutPtr;
	register XmKidGeometry boxPtr;
	XmKidGeometry firstButtonBox;
	Boolean dirListLabelBox;
	Boolean listLabelBox;
	Boolean PaletteBox;
	Boolean MatrixBox;
	Boolean dirListBox;
	Boolean listBox;
	Boolean selLabelBox;
	Boolean filterLabelBox;
	Dimension vspace= BB_MarginHeight(csb);
	int i;

	/*
	 * Layout ColorSelectionBox XmGeoMatrix.
	 * Each row is terminated by leaving an empty XmKidGeometry and
	 * moving to the next XmGeoRowLayout.
	 */
	geoSpec = _XmGeoMatrixAlloc( XmCS_MAX_WIDGETS_VERT,
	csb->composite.num_children,
	sizeof( CS_GeoExtensionRec)) ;
	geoSpec->composite = (Widget) csb;
	geoSpec->instigator = (Widget) instigator;
	if (desired) {
		geoSpec->instig_request = *desired;
	}
	geoSpec->margin_w = BB_MarginWidth( csb) + csb->manager.shadow_thickness;
	geoSpec->margin_h = BB_MarginHeight( csb) + csb->manager.shadow_thickness;
	geoSpec->no_geo_request = ColorSelectionBoxNoGeoRequest;

	layoutPtr = &(geoSpec->layouts->row);
	boxPtr = geoSpec->boxes;

	/* menu bar */
	for (i = 0; i < csb->composite.num_children; i++) {
		Widget w = csb->composite.children[i];

		if ( XmIsRowColumn(w)
		&& ((XmRowColumnWidget)w)->row_column.type == XmMENU_BAR
		&& w != XmColorSB_WorkArea(csb)
		&& _XmGeoSetupKid( boxPtr, w)) {
			layoutPtr->fix_up = _XmMenuBarFix;
			boxPtr += 2;
			++layoutPtr;
			vspace = 0; // fixup space_above of next row.
			break;
		}
	}

	/* matrix board and palette board */

	if (LayoutIsRtoLM(csb)) {
		MatrixBox = FALSE;
		if ( XmColorSB_MatrixBoard(csb) && XtIsManaged(XmColorSB_MatrixBoard(csb))
		&& _XmGeoSetupKid( boxPtr, XmColorSB_MatrixBoard( csb))) {
			MatrixBox = TRUE;
			++boxPtr;
		}
		PaletteBox = FALSE;
		if ( XmColorSB_ColorBoard(csb) && XtIsManaged(XmColorSB_ColorBoard(csb))
		&& _XmGeoSetupKid( boxPtr, XmColorSB_ColorBoard( csb))) {
			PaletteBox = TRUE;
			++boxPtr;
		}
	} else {
		PaletteBox = FALSE;
		if ( XmColorSB_ColorBoard(csb) && XtIsManaged(XmColorSB_ColorBoard(csb))
		&& _XmGeoSetupKid( boxPtr, XmColorSB_ColorBoard( csb))) {
			PaletteBox = TRUE;
			++boxPtr;
		}
		MatrixBox = FALSE;
		if ( XmColorSB_MatrixBoard(csb) && XtIsManaged(XmColorSB_MatrixBoard(csb))
		&& _XmGeoSetupKid( boxPtr, XmColorSB_MatrixBoard( csb))) {
			MatrixBox = TRUE;
			++boxPtr;
		}
	}

	if (PaletteBox || MatrixBox) {
		layoutPtr->fix_up = BoxFix;
		layoutPtr->fit_mode = XmGEO_AVERAGING;
		layoutPtr->space_between = BB_MarginWidth(csb);
		layoutPtr->stretch_height = TRUE;
		layoutPtr->min_height = 400;
		layoutPtr->even_height = 1;
		if ( !MatrixBox && !PaletteBox) {
			layoutPtr->space_above = vspace;
			vspace = BB_MarginHeight(csb);
		}

		++boxPtr;
		++layoutPtr;
	}

	SetupWorkArea(csb)

	/* separator */

	if (_XmGeoSetupKid(boxPtr, XmColorSB_separator( csb)) ) {
		layoutPtr->fix_up = _XmSeparatorFix;
		layoutPtr->space_above = vspace;
		vspace = BB_MarginHeight(csb);
		boxPtr += 2;
		++layoutPtr;
	}

	/* button row */

	firstButtonBox = boxPtr;

	if (LayoutIsRtoLM(csb)) {
		if (_XmGeoSetupKid(boxPtr, XmColorSB_pButtonHelp( csb)) ) {
			++boxPtr;
		}
		if (_XmGeoSetupKid(boxPtr, XmColorSB_pButtonCancel( csb)) ) {
			++boxPtr;
		}
		if (_XmGeoSetupKid(boxPtr, XmColorSB_pButtonOk( csb)) ) {
			++boxPtr;
		}
	} else {
		if (_XmGeoSetupKid(boxPtr, XmColorSB_pButtonOk( csb)) ) {
			++boxPtr;
		}
		if (_XmGeoSetupKid(boxPtr, XmColorSB_pButtonCancel( csb)) ) {
			++boxPtr;
		}
		if (_XmGeoSetupKid(boxPtr, XmColorSB_pButtonHelp( csb)) ) {
			++boxPtr;
		}
	}

	if (boxPtr != firstButtonBox) {
		layoutPtr->fit_mode = XmGEO_WRAP;
		layoutPtr->even_width = 1;
		layoutPtr->space_above = vspace;
		vspace = BB_MarginHeight(csb);
		layoutPtr->even_height = 1;
		++layoutPtr;
	}

	/* the end. */

	layoutPtr->space_above = vspace;
	layoutPtr->end = TRUE;
	return (geoSpec);

}

/* ARGSUSED */
static void GetValues_XmNredValue(Widget w, int n, XtArgVal *value)
{
	(*value) = (XtArgVal) XmColorSB_red_value(w);
}

/* ARGSUSED */
static void GetValues_XmNgreenValue(Widget w, int n, XtArgVal *value)
{
	(*value) = (XtArgVal) XmColorSB_green_value(w);
}

/* ARGSUSED */
static void GetValues_XmNblueValue(Widget w, int n, XtArgVal *value)
{
	(*value) = (XtArgVal) XmColorSB_blue_value(w);
}

/* ARGSUSED */
static void GetValues_XmNhueValue(Widget w, int n, XtArgVal *value)
{
	(*value) = (XtArgVal) XmColorSB_x(w);
}

/* ARGSUSED */
static void GetValues_XmNsatValue(Widget w, int n, XtArgVal *value)
{
	(*value) = (XtArgVal) XmColorSB_y(w);
}

/* ARGSUSED */
static void GetValues_XmNbrightnessValue(Widget w, int n, XtArgVal *value)
{
	(*value) = (XtArgVal) XmColorSB_brightness_volume(w);
}

/* ARGSUSED */
static void GetValues_XmNhelpOn(Widget w, int n, XtArgVal *value)
{
	(*value) = (XtArgVal) XmColorSB_helpOn(w);
}

/************************************************************
 *
 *  Public functions.
 *
 ************************************************************/

/*	Function Name: XmCreateColorSelectionBox
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The created widget.
 */

Widget XmCreateColorSelectionBox(Widget parent, String name, ArgList args, Cardinal num_args)
{
	return (XtCreateWidget(name, xmColorSelectionBoxWidgetClass, parent, args, num_args));
}

/****************************************************************
 * This convenience function creates a DialogShell
 *   and a ColorSelectionBox child of the shell;
 *   returns the ColorSelectionBox widget.
 ****************/
Widget XmCreateColorSelectionDialog(Widget parent, char *name, ArgList arglist, Cardinal argcount)
{
	return XmeCreateClassDialog(xmColorSelectionBoxWidgetClass, parent, name, arglist, argcount) ;
}

Widget XmVaCreateColorSelectionBox(Widget parent, char *name, ...)
{
	register Widget w;
	va_list var;
	int count;

	va_start(var,name);
	count = XmeCountVaListSimple(var);
	va_end(var);

	va_start(var, name);
	w = XmeVLCreateWidget(name, xmColorSelectionBoxWidgetClass, parent, False, var, count);
	va_end(var);
	return w;
}

Widget XmVaCreateManagedColorSelectionBox(Widget parent, char *name, ...)
{
	Widget w= NULL;
	va_list var;
	int count;

	va_start(var, name);
	count = XmeCountVaListSimple(var);
	va_end(var);

	va_start(var, name);
	w = XmeVLCreateWidget(name, xmColorSelectionBoxWidgetClass, parent, True, var, count);
	va_end(var);
	return w;
}
