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
/* (c) Copyright 1989, Massachusetts Institute of Technology */
/*   Author:  Jim Fulton, MIT X Consortium */
/* (c) Copyright 1992, Integrated Computer Solutions, Inc. */
/*   Author:  Chris D. Peterson */
 
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>			/* for BUFSIZ. */
#include <ctype.h>			/* for isascii() etc. */
#include <stdlib.h>			/* for atof(), depends on system */
#include <math.h>			/* for atof(), depends on system */

#include <Xm/XmP.h>
#include <Xm/DrawP.h>

#include <Xm/ManagerP.h>
#include <Xm/PannerP.h>
#include <Xm/XmP.h>
#include <Xm/XmI.h>

#include "ReportsI.h"

#define SUPERCLASS ((WidgetClass) &xmPrimitiveClassRec)

#define XmPannerDefaultScale 8                /* percent */

static char defaultTranslations[] = 
  "<Btn1Down>:		XmGetFocus() XmStart()\n\
   <Btn1Motion>:	XmMove()\n\
   <Btn1Up>:		XmNotify() XmStop()\n\
   <Btn2Down>:		XmAbort()\n\
   <Btn3Down>,<Btn3Up>:	XmWarpTo()\n\
   <Key>osfSelect:	XmSet(rubberband,toggle)\n\
   <Key>osfActivate:	XmPage(+1p,+1p)\n\
   None<Key>Return:	XmPage(+1p,+1p)\n\
   None<Key>space:	XmPage(+1p,+1p)\n\
   <Key>osfDelete:	XmPage(-1p,-1p)\n\
   <Key>osfBackSpace:	XmPage(-1p,-1p)\n\
   <Key>osfLeft:	XmPage(-.5p,+0)\n\
   <Key>osfRight:	XmPage(+.5p,+0)\n\
   <Key>osfUp:		XmPage(+0,-.5p)\n\
   <Key>osfDown:	XmPage(+0,+.5p)";

/*
 * These are the primitive translations that we need, this removes 
 * the ones for up, down, left, and right, which we are setting above.
 */

static char traverseTranslations[] = 
  "<Key>osfHelp:	PrimitiveHelp()\n\
   Shift ~Meta ~Alt <Key>Tab: PrimitivePrevTabGroup()\n\
   ~Meta ~Alt <Key>Tab: PrimitiveNextTabGroup()\n\
   <Enter>:		PrimitiveEnter()\n\
   <Leave>:		PrimitiveLeave()\n\
   <FocusIn>:		PrimitiveFocusIn()\n\
   <FocusOut>:		PrimitiveFocusOut()\n\
   <Unmap>:		PrimitiveUnmap()";

static void ActionStart(Widget, XEvent *, String *, Cardinal *);
static void ActionStop(Widget, XEvent *, String *, Cardinal *);
static void ActionAbort(Widget, XEvent *, String *, Cardinal *);
static void ActionMove(Widget, XEvent *, String *, Cardinal *);
static void ActionPage(Widget, XEvent *, String *, Cardinal *);
static void ActionNotify(Widget, XEvent *, String *, Cardinal *);
static void ActionSet(Widget, XEvent *, String *, Cardinal *);
static void ActionWarpTo(Widget, XEvent *, String *, Cardinal *);

static XtActionsRec actions[] = {
    { "XmWarpTo", 	ActionWarpTo},	
    { "XmStart", 	ActionStart },	/* start tmp graphics */
    { "XmStop", 	ActionStop },	/* stop tmp graphics */
    { "XmAbort", 	ActionAbort },	/* punt */
    { "XmMove", 	ActionMove },	/* move tmp graphics on Motion event */
    { "XmPage",         ActionPage },	/* page around usually from keyboard */
    { "XmNotify", 	ActionNotify },	/* callback new position */
    { "XmSet", 		ActionSet },    /* set various parameters */
    { "XmGetFocus",	_XmGetFocus},   /* Send focus to me. */
    /*
     *  Why aren't these in primitive? 
     */
    { "PrimitiveEnter",  (XtActionProc) _XmPrimitiveEnter },
    { "PrimitiveLeave",  (XtActionProc) _XmPrimitiveLeave },
};

/*
 * resources for the panner
 */
static XmPartResource resources[] = {
    { XmNresize, XmCResize, XmRBoolean, sizeof(Boolean),
	XtOffsetOf(XmPannerRec, panner.resize_to_pref),
	XmRImmediate, (XtPointer) False },
    { XmNreportCallback, XmCReportCallback, XmRCallback, sizeof(XtPointer),
	XtOffsetOf(XmPannerRec, panner.report_callbacks),
	XmRCallback, (XtPointer) NULL },
    { XmNdefaultScale, XmCDefaultScale, XmRDimension, sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.default_scale),
	XmRImmediate, (XtPointer) XmPannerDefaultScale},
    { XmNrubberBand, XmCRubberBand, XmRBoolean, sizeof(Boolean),
      XtOffsetOf(XmPannerRec, panner.rubber_band),
      XmRImmediate, (XtPointer) FALSE },
    { XmNinternalSpace, XmCInternalSpace, XmRHorizontalDimension, sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.internal_border),
	XmRImmediate, (XtPointer) 2 },

    /*
     * These are in the coordinates of the object the panner is
     * scrolling and are normalized to fit the current size of the panner.
     */

    { XmNcanvasWidth, XmCCanvasWidth, XmRHorizontalDimension,sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.canvas_width), 
	XmRImmediate, (XtPointer) 0 },
    { XmNcanvasHeight, XmCCanvasHeight, XmRVerticalDimension,sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.canvas_height), 
	XmRImmediate, (XtPointer) 0 },
    { XmNsliderX, XmCSliderX, XmRHorizontalPosition, sizeof(Position),
	XtOffsetOf(XmPannerRec, panner.slider_x), 
	XmRImmediate, (XtPointer) 0 },
    { XmNsliderY, XmCSliderY, XmRVerticalPosition, sizeof(Position),
	XtOffsetOf(XmPannerRec, panner.slider_y), 
	XmRImmediate, (XtPointer) 0 },
    { XmNsliderWidth, XmCSliderWidth, XmRHorizontalDimension,sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.slider_width), 
	XmRImmediate, (XtPointer) 0 },
    { XmNsliderHeight, XmCSliderHeight, XmRVerticalDimension,sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.slider_height),
	XmRImmediate, (XtPointer) 0 },

    /*
     * Put the panner in its own tab group.
     */

    { XmNnavigationType, XmCNavigationType, XmRNavigationType, sizeof(unsigned char),
	XtOffsetOf(XmManagerRec, manager.navigation_type),
	XmRImmediate, (XtPointer) XmTAB_GROUP }
};

static XmSyntheticResource get_resources[] =
{
    { XmNcanvasWidth, sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.canvas_width),
	XmeFromHorizontalPixels, (XmImportProc) XmeToHorizontalPixels
    },
    { XmNcanvasHeight, sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.canvas_height),
	XmeFromVerticalPixels, (XmImportProc) XmeToVerticalPixels
    },
    { XmNsliderWidth, sizeof(Dimension), 
	XtOffsetOf(XmPannerRec, panner.slider_width),
	XmeFromHorizontalPixels, (XmImportProc) XmeToHorizontalPixels
    },
    { XmNsliderHeight, sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.slider_height),
	XmeFromVerticalPixels, (XmImportProc) XmeToVerticalPixels
    },
    { XmNsliderX, sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.slider_x),
	XmeFromHorizontalPixels, (XmImportProc) XmeToHorizontalPixels
    },
    { XmNsliderHeight, sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.slider_y),
	XmeFromVerticalPixels, (XmImportProc) XmeToVerticalPixels
    },
    { XmNinternalSpace, sizeof(Dimension),
	XtOffsetOf(XmPannerRec, panner.internal_border),
	XmeFromHorizontalPixels, (XmImportProc) XmeToHorizontalPixels
    },
};

typedef struct _XmPannerPrivateRec
{
    GC slider_gc;		/* background of slider */
    GC xor_gc;			/* for doing XOR tmp graphics */
    tmpstruct tmp;
    Position knob_x;		/* real upper left of knob in canvas */
    Position knob_y;
    Dimension knob_width;	/* real size of knob in canvas */
    Dimension knob_height;
    Position last_x;		/* previous location of knob */
    Position last_y;
    double haspect;		/* aspect ratio of core to canvas */
    double vaspect;
} XmPannerPrivateData;

static void ResetSliderGC(XmPannerWidget);
static void ResetXorGC(XmPannerWidget);
static void CheckKnob(register XmPannerWidget, Boolean);
static void ScaleKnob(XmPannerWidget, Boolean, Boolean);
static void Rescale(XmPannerWidget);
static void GetDefaultSize(XmPannerWidget, Dimension *, Dimension *);

static Boolean GetEventXY(XmPannerWidget, XEvent *, int *, int *);

static int ParsePageString(register char *, int, int, Boolean *);

/*
 * widget class methods used below
 */

/* create gc's */
static void Initialize(Widget, Widget, ArgList, Cardinal *);

/* setup offsets */
static void ClassInitialize(void);

/* clean up widget */
static void Destroy(Widget);

/* need to rescale ourselves */
static void Resize(Widget);

/* draw ourselves */
static void
Redisplay(Widget, XEvent *, Region);

/* set all of the resources */
static Boolean SetValues(Widget, Widget, Widget, ArgList, Cardinal *);

/* deal with failed setval geom req */
static void SetValuesAlmost(Widget, Widget,
			    XtWidgetGeometry *, XtWidgetGeometry *);

/* say how big we would like to be */
static XtGeometryResult QueryGeometry(Widget,
				      XtWidgetGeometry *, XtWidgetGeometry *);

XmPannerClassRec xmPannerClassRec = {
  { /* core fields */
    /* superclass		*/	SUPERCLASS,
    /* class_name		*/	"XmPanner",
    /* widget_size		*/	sizeof(XmPannerRec),
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
    /* expose			*/	Redisplay,
    /* set_values		*/	SetValues,
    /* set_values_hook		*/	NULL,
    /* set_values_almost	*/	SetValuesAlmost,
    /* get_values_hook		*/	NULL,
    /* accept_focus		*/	NULL,
    /* version			*/	XtVersionDontCheck,
    /* callback_private		*/	NULL,
    /* tm_table			*/	defaultTranslations,
    /* query_geometry		*/	(XtGeometryHandler) QueryGeometry,
    /* display_accelerator	*/	XtInheritDisplayAccelerator,
    /* extension		*/	NULL
  },
  { /* Xmprimitive fields */
      (XtWidgetProc)_XtInherit, /* border_highlight   */
      (XtWidgetProc)_XtInherit, /* border_unhighlight */
      traverseTranslations,	/* translations       */
      NULL,                     /* arm_and_activate   */
      get_resources,   	    	/* syn resources      */
      XtNumber(get_resources),	/* num syn_resources  */
      NULL                      /* extension          */
  },
  { /* panner fields */
    /* extension */    NULL
  }
};

WidgetClass xmPannerWidgetClass = (WidgetClass) &xmPannerClassRec;

/*****************************************************************************
 *                                                                           *
 *			    panner utility routines                          *
 *                                                                           *
 *****************************************************************************/

/*	Function Name: ResetSliderGC
 *	Description: Resets the graphics context of the slider
 *	Arguments: pw - the panner widget.
 *	Returns: none
 */

static void 
ResetSliderGC (XmPannerWidget pw)	
{
    XtGCMask valuemask = GCForeground;
    XGCValues values;

    if (XmPanner_slider_gc(pw) != None) 
	XtReleaseGC ((Widget) pw, XmPanner_slider_gc(pw));

    values.foreground = pw->primitive.foreground;
    XmPanner_slider_gc(pw) = XtGetGC ((Widget) pw, valuemask, &values);
}

/*	Function Name: ResetXorGC
 *	Description: Resets the Xor Graphics Context.
 *	Arguments: pw - the Panner widget.
 *	Returns: none.
 */

static void 
ResetXorGC(XmPannerWidget pw)		/* used when resources change */
{
    if (XmPanner_xor_gc(pw)) 
	XtReleaseGC ((Widget) pw, XmPanner_xor_gc(pw));

    if (XmPanner_rubber_band(pw)) {
	XtGCMask valuemask = (GCForeground | GCFunction);
	XGCValues values;

	values.foreground = (pw->primitive.foreground ^
			     pw->core.background_pixel);
	values.function = GXxor;
	XmPanner_xor_gc(pw) = XtGetGC ((Widget) pw, valuemask, &values);
    } else {
	XmPanner_xor_gc(pw) = NULL;
    }
}

/*	Function Name: CheckKnob
 *	Description: Makes sure the knob is within the allowed canvas space.
 *	Arguments: pw - the panner widget.
 *                 knob - If true we are working on the knob, else we
 *                        are working on the temp outline.
 *	Returns: none.
 */

static void 
CheckKnob(register XmPannerWidget pw, Boolean knob)
{
    Position pad = 2 * (XmPanner_internal_border(pw) +
			pw->primitive.highlight_thickness +
			pw->primitive.shadow_thickness);
    Position maxx = (((Position) pw->core.width) - pad -
		     ((Position) XmPanner_knob_width(pw)));
    Position maxy = (((Position) pw->core.height) - pad -
		     ((Position) XmPanner_knob_height(pw)));
    Position *x = (knob ? &XmPanner_knob_x(pw) : &XmPanner_tmp(pw).x);
    Position *y = (knob ? &XmPanner_knob_y(pw) : &XmPanner_tmp(pw).y);

    /*
     * note that positions are already normalized (i.e. internal_border
     * has been subtracted out)
     */

    if (*x < 0)
	*x = 0;
    if (*x > maxx) 
	*x = maxx;

    if (*y < 0) 
	*y = 0;
    if (*y > maxy)
	*y = maxy;

    if (knob) {
	
	XmPanner_slider_x(pw) = (Position)(((double)XmPanner_knob_x(pw)) /
					   XmPanner_haspect(pw) + 0.5);
	XmPanner_slider_y(pw) = (Position)(((double)XmPanner_knob_y(pw)) /
					   XmPanner_vaspect(pw) + 0.5);
	XmPanner_last_x(pw) = XmPanner_last_y(pw) = PANNER_OUTOFRANGE;
    }
}

/*	Function Name: ScaleKnob
 *	Description: Scales the knob to a new size as specified in the
 *                   resources, based on the panner's current geometry.
 *	Arguments: pw - the panner.
 *                 location - rescale the current location?
 *                 size - rescale the knob size?
 *	Returns: none.
 */

static void 
ScaleKnob(XmPannerWidget pw, Boolean location, Boolean size)
{
    if (location) {
	XmPanner_knob_x(pw) = (Position) PANNER_HSCALE (pw, XmPanner_slider_x(pw));
	XmPanner_knob_y(pw) = (Position) PANNER_VSCALE (pw, XmPanner_slider_y(pw));
    }

    if (size) {
	Dimension width, height;

	if (XmPanner_slider_width(pw) < 1) {
	    XmPanner_slider_width(pw) = XmPanner_canvas_width(pw);
	}
	if (XmPanner_slider_height(pw) < 1) {
	    XmPanner_slider_height(pw) = XmPanner_canvas_height(pw);
	}
	width = MIN (XmPanner_slider_width(pw), XmPanner_canvas_width(pw));
	height = MIN (XmPanner_slider_height(pw), XmPanner_canvas_height(pw));

	XmPanner_knob_width(pw) = (Dimension) PANNER_HSCALE (pw, width);
	XmPanner_knob_height(pw) = (Dimension) PANNER_VSCALE (pw, height);

 	XmPanner_knob_width(pw) = MAX (XmPanner_knob_width(pw), 2);
 	XmPanner_knob_height(pw) = MAX (XmPanner_knob_height(pw), 2);
    }

    CheckKnob (pw, TRUE);
}

/*	Function Name: Rescale
 *	Description: Resets the current aspect ratio based on the panner
 *                   size and the desired canvas size.
 *	Arguments: pw - the panner widget.
 *	Returns: none.
 */

static void 
Rescale(XmPannerWidget pw)
{
    int hpad = 2 * (XmPanner_internal_border(pw) +
		    pw->primitive.highlight_thickness +
		    pw->primitive.shadow_thickness);
    int vpad = hpad;

    if (XmPanner_canvas_width(pw) < 1)
      XmPanner_canvas_width(pw) = pw->core.width;

    if (XmPanner_canvas_height(pw) < 1)
      XmPanner_canvas_height(pw) = pw->core.height;

    if ((int)pw->core.width <= hpad) 
	hpad = 0;

    if ((int)pw->core.height <= vpad) 
	vpad = 0;

    XmPanner_haspect(pw) = 
	(double)(pw->core.width - hpad) / (double)XmPanner_canvas_width(pw);

    XmPanner_vaspect(pw) = 
	(double)(pw->core.height - vpad) / (double)XmPanner_canvas_height(pw);

    ScaleKnob (pw, TRUE, TRUE);
}

/*	Function Name: GetDefaultSize
 *	Description: Gets the desired size of the panner.
 *	Arguments: pw - the panner widget.
 * RETURNED        wp, hp - the desired height and width.
 *	Returns: none.
 */

static void 
GetDefaultSize (XmPannerWidget pw, Dimension *wp, Dimension *hp)
{
    Dimension pad = 2 * (XmPanner_internal_border(pw) +
			 pw->primitive.highlight_thickness +
			 pw->primitive.shadow_thickness);

    *wp = PANNER_DSCALE (pw, XmPanner_canvas_width(pw)) + pad;
    *hp = PANNER_DSCALE (pw, XmPanner_canvas_height(pw)) + pad;
}

/*	Function Name: GetEventXY
 *	Description: Gets the x and y location from an event, automatically
 *                   subtracting off the internal borders.
 *	Arguments: pw - the panner widget
 *                 event - the event to rip the x and y out of
 * RETURNED        x, y - the x and y from the event.                
 *	Returns: True if successful
 */

static Boolean 
GetEventXY (XmPannerWidget pw, XEvent *event, int *x, int *y)
{
    int pad = (XmPanner_internal_border(pw) +
	       pw->primitive.highlight_thickness +
	       pw->primitive.shadow_thickness);

    switch (event->type) {
      case ButtonPress:
      case ButtonRelease:
	*x = event->xbutton.x - pad;
	*y = event->xbutton.y - pad;
	return TRUE;

      case KeyPress:
      case KeyRelease:
	*x = event->xkey.x - pad;
	*y = event->xkey.y - pad;
	return TRUE;

      case EnterNotify:
      case LeaveNotify:
	*x = event->xcrossing.x - pad;
	*y = event->xcrossing.y - pad;
	return TRUE;

      case MotionNotify:
	*x = event->xmotion.x - pad;
	*y = event->xmotion.y - pad;
	return TRUE;
    }

    return FALSE;
}

/*	Function Name: ParsePageString
 *	Description: Parses the page string from an action routine.
 *	Arguments: s - the string to parse
 *                 pagesize - the size of a page.
 *                 canvassize - the size of the current canvass
 * RETURNED        relative - is the position relative to the current loc?
 *	Returns: the location or offset.
 */

static int 
ParsePageString(register char *s, 
		int pagesize, int canvassize, Boolean *relative)
{
    char *cp;
    double val = 1.0;
    Boolean rel = FALSE;

    /*
     * syntax:    spaces [+-] number spaces [pc\0] spaces
     */

    while (isascii(*s) && isspace(*s)) 	/* skip white space */
	s++;

    if (*s == '+' || *s == '-') {	/* deal with signs */
	rel = TRUE;
	if (*s == '-') val = -1.0;
	s++;
    }

    if (*s == '\0') {			/* if null then return nothing */
	*relative = TRUE;
	return 0;
    }

    /* skip over numbers */

    for (cp = s; isascii(*s) && (isdigit(*s) || *s == '.'); ) 
	s++;

    val *= atof (cp);

    /* skip blanks */

    for (; isascii(*s) && isspace(*s); ) 
	s++;

    if (*s) {				/* if units */
	switch (s[0]) {
	  case 'p': case 'P':
	    val *= (double) pagesize;
	    break;

	  case 'c': case 'C':
	    val *= (double) canvassize;
	    break;
	}
    }

    *relative = rel;

    return ((int) val);
}

#define DRAW_TMP(pw) \
{ \
    XDrawRectangle (XtDisplay(pw), XtWindow(pw), \
		    XmPanner_xor_gc(pw), \
		    (int) (XmPanner_tmp(pw).x + XmPanner_internal_border(pw) + \
			   pw->primitive.highlight_thickness + \
			   pw->primitive.shadow_thickness), \
		    (int) (XmPanner_tmp(pw).y + XmPanner_internal_border(pw) + \
			   pw->primitive.highlight_thickness + \
			   pw->primitive.shadow_thickness), \
		    (unsigned int) (XmPanner_knob_width(pw) - 1), \
		    (unsigned int) (XmPanner_knob_height(pw) - 1)); \
    XmPanner_tmp(pw).showing = !XmPanner_tmp(pw).showing; \
}

#define UNDRAW_TMP(pw) \
{ \
    if (XmPanner_tmp(pw).showing) DRAW_TMP(pw); \
}

/*****************************************************************************
 *                                                                           *
 * 			     panner class methods                            *
 *                                                                           *
 *****************************************************************************/

/*      Function Name: ClassInitialize
 *      Description:   Initializes class-specific data (offsets)
 *      Arguments:     none
 *      Returns:       nothing
 */
static void
ClassInitialize()
{
    XmPannerClassRec *wc = &xmPannerClassRec;
}


/*	Function Name: Initialize
 *	Description: Called when the panner is initially created.
 *	Arguments: greq - the requested widget
 *                 gset - the new widget.
 *                 args, num_args - the argument list.
 *	Returns: none.
 */

/* ARGSUSED */
static void 
Initialize (Widget greq, Widget gset, ArgList args, Cardinal *num_args)
{
    XmPannerWidget req = (XmPannerWidget) greq, set = (XmPannerWidget) gset;
    Dimension defwidth, defheight;

#ifdef DEMO
    _XmInitialIzeConverters(greq);
#endif

    XmPanner_private(set) =
	(XmPannerPrivateData *)XtCalloc(1, sizeof(XmPannerPrivateData));
    
    ASSIGN_MAX(XmPanner_canvas_width(req), 1);
    ASSIGN_MAX(XmPanner_canvas_height(req), 1);

    if (XmPanner_default_scale(req) < 1)
      XmPanner_default_scale(set) = XmPannerDefaultScale;

    GetDefaultSize (req, &defwidth, &defheight);

    if (req->core.width < 1) 
	set->core.width = defwidth;

    if (req->core.height < 1) 
	set->core.height = defheight;

    XmPanner_slider_gc(set) = XmPanner_xor_gc(set) = NULL;

    ResetSliderGC (set);		/* foreground */
    ResetXorGC (set);			/* foreground ^ background */

    Rescale(set);			/* does a position check */

    XmPanner_tmp(set).doing = FALSE;
    XmPanner_tmp(set).showing = FALSE;
}

/*	Function Name: Destroy 
 *	Description: Called when the widget is destroyed to clean up.
 *	Arguments: w - the panner widget.
 *	Returns: none.
 */

static void 
Destroy (Widget w)
{
    XmPannerWidget pw = (XmPannerWidget) w;

    XtReleaseGC (w, XmPanner_slider_gc(pw));
    XtReleaseGC (w, XmPanner_xor_gc(pw));
    XtFree((char *)XmPanner_private(pw));
}

/*	Function Name: Resize
 *	Description: Called when the size of the panner has changed.
 *	Arguments: w - the widget.
 *	Returns: none.
 */

static void
Resize (Widget w)
{
    Rescale ((XmPannerWidget) w);
}

/*	Function Name: Redisplay
 *	Description: Redraws the contents of the panner.
 *	Arguments: gw - the panner widget.
 *                 event - the expose event.
 *                 region - *** UNUSED ***.
 *	Returns: none.
 */

/* ARGSUSED */
static void 
Redisplay(Widget gw, XEvent * event, Region region)
{
    XmPannerWidget pw = (XmPannerWidget) gw;
    Display *dpy = XtDisplay(gw);
    Window w = XtWindow(gw);
    int pad = (XmPanner_internal_border(pw) +
	       pw->primitive.highlight_thickness +
	       pw->primitive.shadow_thickness);
    int dx, dy, width, height;
    int kx = XmPanner_knob_x(pw) + pad, ky = XmPanner_knob_y(pw) + pad;

    XmPanner_tmp(pw).showing = FALSE;

    if (event != NULL) {
	dx = dy = pw->primitive.highlight_thickness;
	width = pw->core.width - (2 * dx);
	height = pw->core.height - (2 * dy);
	
	_XmDrawShadows(XtDisplay(gw), XtWindow(gw), 
		       pw->primitive.bottom_shadow_GC, 
		       pw->primitive.top_shadow_GC, dx, dy, width, height,
		       pw->primitive.shadow_thickness, XmSHADOW_OUT);

	if (pw->primitive.highlighted) 
	    (*(xmPrimitiveClassRec.primitive_class.border_highlight))(gw);
	else
	    (*(xmPrimitiveClassRec.primitive_class.border_unhighlight))(gw);
    }

    if ((XmPanner_tmp(pw).doing && XmPanner_rubber_band(pw)) || (event == NULL))
    {
	XClearArea (XtDisplay(pw), XtWindow(pw), 
		    (int) XmPanner_last_x(pw) + pad, 
		    (int) XmPanner_last_y(pw) + pad, 
		    (unsigned int) (XmPanner_knob_width(pw)),
		    (unsigned int) (XmPanner_knob_height(pw)),
		    False);
    }

    XmPanner_last_x(pw) = XmPanner_knob_x(pw);
    XmPanner_last_y(pw) = XmPanner_knob_y(pw);

    XmeDrawShadows(XtDisplay(gw), XtWindow(gw), 
		   pw->primitive.top_shadow_GC,
		   pw->primitive.bottom_shadow_GC,
		   kx, ky, XmPanner_knob_width(pw), XmPanner_knob_height(pw),
		   pw->primitive.shadow_thickness, XmSHADOW_OUT);

    dx = kx + pw->primitive.shadow_thickness;
    dy = ky + pw->primitive.shadow_thickness;

    if (((int)XmPanner_knob_width(pw) >
	 (int)(2 * pw->primitive.shadow_thickness)) &&
	((int)XmPanner_knob_height(pw) >
	 (int)(2 * pw->primitive.shadow_thickness)))
    {
	width = XmPanner_knob_width(pw) - (2 * pw->primitive.shadow_thickness);
	height = XmPanner_knob_height(pw) - (2 * pw->primitive.shadow_thickness);
	
	XFillRectangle (dpy, w, XmPanner_slider_gc(pw), dx, dy, width, height);
    }

    if (XmPanner_tmp(pw).doing && XmPanner_rubber_band(pw)) 
	DRAW_TMP (pw);
}

/*	Function Name: SetValues
 *	Description: Called when a resource value has changed.
 *	Arguments: gcur - the current state of the widget
 *                 greq - the requested state of the widget
 *                 gset - the new state of the widget.
 *                 args, num_args - the argument list.
 *	Returns: 
 */

/* ARGSUSED */
static Boolean 
SetValues (Widget gcur, Widget greq, 
	   Widget gset, ArgList args, Cardinal *num_args)
{
    XmPannerWidget cur = (XmPannerWidget) gcur;
    XmPannerWidget set = (XmPannerWidget) gset;
    Boolean redisplay = FALSE;

    if (cur->primitive.foreground != set->primitive.foreground) {
	ResetSliderGC (set);
	
	if (cur->primitive.foreground != cur->core.background_pixel)
	    ResetXorGC (set);
	
	redisplay = TRUE;
    }
    else if (cur->core.background_pixel != set->core.background_pixel) 
    {
	ResetXorGC (set);
	redisplay = TRUE;
    }

    if (XmPanner_rubber_band(cur) != XmPanner_rubber_band(set)) {
	ResetXorGC (set);
	if (XmPanner_tmp(set).doing) redisplay = TRUE;
    }

    if (XmPanner_resize_to_pref(set) &&
	(XmPanner_canvas_width(cur) != XmPanner_canvas_width(set) ||
	 XmPanner_canvas_height(cur) != XmPanner_canvas_height(set) ||
	 XmPanner_resize_to_pref(cur) != XmPanner_resize_to_pref(set))) 
    {
	GetDefaultSize (set, &set->core.width, &set->core.height);
	redisplay = TRUE;
    }
    else if (XmPanner_canvas_width(cur) != XmPanner_canvas_width(set) ||
	     XmPanner_canvas_height(cur) != XmPanner_canvas_height(set) ||
	     XmPanner_internal_border(cur) != XmPanner_internal_border(set)) 
    {
	Rescale (set);			/* does a scale_knob as well */
	redisplay = TRUE;

    }
    else {
	Boolean loc = (XmPanner_slider_x(cur) != XmPanner_slider_x(set) ||
		       XmPanner_slider_y(cur) != XmPanner_slider_y(set));
	Boolean siz = (XmPanner_slider_width(cur) != XmPanner_slider_width(set) ||
		       XmPanner_slider_height(cur) != XmPanner_slider_height(set));
	if (loc || siz) 
	    {
	    /* Fake it as though user has made a page movement; works as long
	    ** as we don't get a callback now with different X and Y values
	    */
	    /* note! siz does change from the porthole, but if the app changes
	    ** it programmatically, then the value is off
	    */
	    XmPanner_tmp(set).doing = TRUE;
	    XmPanner_tmp(set).x = XmPanner_knob_x(set) + PANNER_HSCALE (set, XmPanner_slider_x(set) - XmPanner_slider_x(cur));
	    XmPanner_tmp(set).y = XmPanner_knob_y(set) + PANNER_VSCALE (set, XmPanner_slider_y(set) - XmPanner_slider_y(cur));
	    ActionNotify((Widget)set, NULL, NULL, 0);
	    XmPanner_tmp(set).doing = FALSE;
	    ScaleKnob(set,loc,siz);
	    redisplay = TRUE;
	    }
	}

    return redisplay;
}

/*	Function Name: SetValuesAlmost
 *	Description: Called when a geometry request has failed from set values.
 *	Arguments: gold - the old widget
 *                 gset - the new widget.
 *                 req, reply - What we requested and received from our
 *                              parent's geometry manager.
 *	Returns: none.
 */

static void 
SetValuesAlmost (Widget gold, Widget gset, 
		 XtWidgetGeometry *req, XtWidgetGeometry *reply)
{
    XtAlmostProc set_val_al;

    if (reply->request_mode == 0) {	/* got turned down, so cope */
	Resize (gset);
    }

    _XmProcessLock();
    set_val_al = *SUPERCLASS->core_class.set_values_almost;
    _XmProcessUnlock();
    (*set_val_al)(gold, gset, req, reply);

}

/*	Function Name: QueryGeometry
 *	Description: Called when our parent is asking what size we would
 *                   like.
 *	Arguments: gw - the panner widget.
 *                 intended - info from our parent.
 *                 pref - what we would like to become.
 *	Returns: XtGeometry{Yes,No,Almost} (see Xt Spec for details).
 */

static XtGeometryResult 
QueryGeometry (Widget gw, XtWidgetGeometry *intended, XtWidgetGeometry *pref)
{
    XmPannerWidget pw = (XmPannerWidget) gw;

    GetDefaultSize (pw, &pref->width, &pref->height);

    return(_XmHWQuery(gw, intended, pref));
}

/*****************************************************************************
 *                                                                           *
 * 			      panner action procs                            *
 *                                                                           *
 *****************************************************************************/

/*	Function Name: ActionStart
 *	Description: called when a panning action starts.
 *	Arguments: gw - the panner widget.
 *                 event - the event that caused this action. 
 *                 params, num_params - action routine args.
 *	Returns: none
 */

/* ARGSUSED */
static void 
ActionStart(Widget gw, XEvent *event, String *params, Cardinal *num_params)
{
    XmPannerWidget pw = (XmPannerWidget) gw;
    int x, y;

    if (!GetEventXY (pw, event, &x, &y)) {
	XBell (XtDisplay(gw), 0);	/* should do error message */
	return;
    }

    XmPanner_tmp(pw).doing = TRUE;
    XmPanner_tmp(pw).startx = XmPanner_knob_x(pw);
    XmPanner_tmp(pw).starty = XmPanner_knob_y(pw);
    XmPanner_tmp(pw).dx = (((Position) x) - XmPanner_knob_x(pw));
    XmPanner_tmp(pw).dy = (((Position) y) - XmPanner_knob_y(pw));
    XmPanner_tmp(pw).x = XmPanner_knob_x(pw);
    XmPanner_tmp(pw).y = XmPanner_knob_y(pw);

    if (XmPanner_rubber_band(pw)) 
	DRAW_TMP (pw);
}

/*	Function Name: ActionStop
 *	Description: called when a panning action ends.
 *	Arguments: gw - the panner widget.
 *                 event - the event that caused this action. 
 *                 params, num_params - action routine args.
 *	Returns: none
 */

/* ARGSUSED */
static void 
ActionStop(Widget gw, XEvent *event, String *params, Cardinal *num_params)
{
    XmPannerWidget pw = (XmPannerWidget) gw;
    int x, y;

    if (GetEventXY (pw, event, &x, &y)) {
	XmPanner_tmp(pw).x = ((Position) x) - XmPanner_tmp(pw).dx;
	XmPanner_tmp(pw).y = ((Position) y) - XmPanner_tmp(pw).dy;
	CheckKnob (pw, FALSE);
    }

    if (XmPanner_rubber_band(pw))
	UNDRAW_TMP (pw);

    XmPanner_tmp(pw).doing = FALSE;
}

/*	Function Name: ActionAbort
 *	Description: called when a panning action is aborted.
 *	Arguments: gw - the panner widget.
 *                 event - the event that caused this action. 
 *                 params, num_params - action routine args.
 *	Returns: none
 */

/* ARGSUSED */
static void 
ActionAbort(Widget gw, XEvent *event, String *params, Cardinal *num_params)
{
    XmPannerWidget pw = (XmPannerWidget) gw;

    if (!XmPanner_tmp(pw).doing)
	return;

    if (XmPanner_rubber_band(pw)) 
	UNDRAW_TMP (pw);

    if (!XmPanner_rubber_band(pw)) {		/* restore old position */
	XmPanner_tmp(pw).x = XmPanner_tmp(pw).startx;
	XmPanner_tmp(pw).y = XmPanner_tmp(pw).starty;
	ActionNotify (gw, event, params, num_params);
    }

    XmPanner_tmp(pw).doing = FALSE;
}

/* ARGSUSED */
static void 
ActionWarpTo(Widget gw, XEvent *event, String *params, Cardinal *num_params)
{
    XmPannerWidget pw = (XmPannerWidget) gw;
    int x, y;

    if (!GetEventXY (pw, event, &x, &y)) {
	XBell (XtDisplay(gw), 0);	/* should do error message */
	return;
    }

    XmPanner_tmp(pw).doing = TRUE;
    XmPanner_tmp(pw).x = x;
    XmPanner_tmp(pw).y = y;
    ActionNotify((Widget)pw, NULL, NULL, 0);
    XmPanner_tmp(pw).doing = FALSE;
}

/*	Function Name: ActionMove
 *	Description: Called when a panning action is in progress.
 *	Arguments: gw - the panner widget.
 *                 event - the event that caused this action. 
 *                 params, num_params - action routine args.
 *	Returns: none
 */

/* ARGSUSED */
static void 
ActionMove(Widget gw, XEvent *event, String *params, Cardinal *num_params)
{
    XmPannerWidget pw = (XmPannerWidget) gw;
    int x, y;

    if (!XmPanner_tmp(pw).doing)
	return;

    if (!GetEventXY (pw, event, &x, &y)) {
	XBell (XtDisplay(gw), 0);	/* should do error message */
	return;
    }

    if (XmPanner_rubber_band(pw)) 
	UNDRAW_TMP (pw);

    XmPanner_tmp(pw).x = ((Position) x) - XmPanner_tmp(pw).dx;
    XmPanner_tmp(pw).y = ((Position) y) - XmPanner_tmp(pw).dy;

    if (!XmPanner_rubber_band(pw)) {
	ActionNotify (gw, event, params, num_params);  /* does a check */
    } else {
	CheckKnob (pw, FALSE);
	DRAW_TMP (pw);
    }
}

/*	Function Name: ActionPage
 *	Description: called to move the pane by pages.
 *	Arguments: gw - the panner widget.
 *                 event - the event that caused this action. 
 *                 params, num_params - action routine args.
 *	Returns: none
 */

/* ARGSUSED */
static void 
ActionPage(Widget gw, XEvent *event, String *params, Cardinal *num_params)
{
    XmPannerWidget pw = (XmPannerWidget) gw;
    Cardinal zero = 0;
    Boolean isin = XmPanner_tmp(pw).doing;
    int x, y;
    Boolean relx, rely;
    int pad = 2 * (XmPanner_internal_border(pw) + 
		   pw->primitive.highlight_thickness +
		   pw->primitive.shadow_thickness);

    if (*num_params != 2) {
	XBell (XtDisplay(gw), 0);
	return;
    }

    x = ParsePageString(params[0], (int) XmPanner_knob_width(pw),
			((int) pw->core.width) - pad, &relx);
    y = ParsePageString(params[1], (int) XmPanner_knob_height(pw),
			((int) pw->core.height) - pad, &rely);

    if (relx) 
	x += XmPanner_knob_x(pw);
    if (rely) 
	y += XmPanner_knob_y(pw);

    if (isin) {				/* if in, then use move */
	XEvent ev;
	ev.xbutton.type = ButtonPress;
	ev.xbutton.x = x;
	ev.xbutton.y = y;
	ActionMove (gw, &ev, (String *) NULL, &zero);
    } else {				/* else just do it */
	XmPanner_tmp(pw).doing = TRUE;
	XmPanner_tmp(pw).x = x;
	XmPanner_tmp(pw).y = y;
	ActionNotify (gw, event, (String *) NULL, &zero);
	XmPanner_tmp(pw).doing = FALSE;
    }
}

/*	Function Name: ActionNotify
 *	Description: Called to send a report to notify that something
 *                   has happened.
 *	Arguments: gw - the panner widget.
 *                 event - the event that caused this action. 
 *                 params, num_params - action routine args.
 *	Returns: none
 */

/* ARGSUSED */
static void 
ActionNotify(Widget gw, XEvent *event /*NOT_USED*/, String *params /*NOT_USED*/, Cardinal *num_params /*NOT_USED*/)
{
    XmPannerWidget pw = (XmPannerWidget) gw;
    Position tmp;

    if (!XmPanner_tmp(pw).doing) return;

    CheckKnob (pw, FALSE);

    XmPanner_knob_x(pw) = XmPanner_tmp(pw).x;
    XmPanner_knob_y(pw) = XmPanner_tmp(pw).y;

    XmPanner_slider_x(pw) = (Position)(((double)XmPanner_knob_x(pw)) /
				       XmPanner_haspect(pw) + 0.5);
    XmPanner_slider_y(pw) = (Position)(((double)XmPanner_knob_y(pw)) /
				       XmPanner_vaspect(pw) + 0.5);

    if (XmPanner_slider_x(pw) >
	(tmp = (((Position) XmPanner_canvas_width(pw)) - 
		((Position) XmPanner_slider_width(pw)))))
	XmPanner_slider_x(pw) = tmp;
    if (XmPanner_slider_x(pw) < 0) XmPanner_slider_x(pw) = 0;
    if (XmPanner_slider_y(pw) >
	(tmp = (((Position) XmPanner_canvas_height(pw)) - 
		((Position) XmPanner_slider_height(pw)))))
	XmPanner_slider_y(pw) = tmp;
    if (XmPanner_slider_y(pw) < 0) XmPanner_slider_y(pw) = 0;

    if (XmPanner_last_x(pw) != XmPanner_knob_x(pw) ||
	XmPanner_last_y(pw) != XmPanner_knob_y(pw)) 
    {
	XmScrollReport rep;

	if (XtIsRealized(gw))
		(*(gw->core.widget_class->core_class.expose))(gw, (XEvent*) NULL, (Region) NULL);
	rep.changed = (XmPRSliderX | XmPRSliderY);
	rep.slider_x = XmPanner_slider_x(pw);
	rep.slider_y = XmPanner_slider_y(pw);
	rep.slider_width = XmPanner_slider_width(pw);
	rep.slider_height = XmPanner_slider_height(pw);
	rep.canvas_width = XmPanner_canvas_width(pw);
	rep.canvas_height = XmPanner_canvas_height(pw);
	XtCallCallbackList (gw, XmPanner_report_callbacks(pw), (XtPointer) &rep);
    }
}

/*	Function Name: ActionSet
 *	Description: Called to set an option of the panner widget.
 *	Arguments: gw - the panner widget.
 *                 event - the event that caused this action. 
 *                 params, num_params - action routine args.
 *	Returns: none
 */

/* ARGSUSED */
static void 
ActionSet(Widget gw, XEvent *event, String *params, Cardinal *num_params)
{
    XmPannerWidget pw = (XmPannerWidget) gw;
    Boolean rb;
    char buf[BUFSIZ];

    if (*num_params < 2) {
	XBell (XtDisplay(gw), 0);
	return;
    }

    XmCopyISOLatin1Lowered (buf, params[0]);

    if (!streq("rubberband", buf)) {
	XBell (XtDisplay(gw), 0);
	return;
    }

    XmCopyISOLatin1Lowered (buf, params[1]);

    if (streq(buf, "on")) {
	rb = TRUE;
    } else if (streq(buf, "off")) {
	rb = FALSE;
    } else if (streq(buf, "toggle")) {
	rb = !XmPanner_rubber_band(pw);
    } else {
	XBell (XtDisplay(gw), 0);
	return;
    }

    if (rb != XmPanner_rubber_band(pw)) {
	Arg args[1];
	XtSetArg (args[0], XmNrubberBand, rb);
	XtSetValues (gw, args, (Cardinal) 1);
    }
}

/************************************************************
 *
 *  Public functions.
 *
 ************************************************************/

/*	Function Name: XmCreatePanner
 *	Description: Creation Routine for UIL and ADA.
 *	Arguments: parent - the parent widget.
 *                 name - the name of the widget.
 *                 args, num_args - the number and list of args.
 *	Returns: The created widget.
 */

Widget
XmCreatePanner(Widget parent, String name,
	       ArgList args, Cardinal num_args)
{
    return(XtCreateWidget(name, xmPannerWidgetClass,
			  parent, args, num_args));
}
