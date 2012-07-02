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

#ifndef _XmPannerP_h
#define _XmPannerP_h

#include <Xm/PrimitiveP.h>
#include <Xm/Panner.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _XmPannerClassPart { /* new fields in widget class */
    XtPointer extension;
} XmPannerClassPart;

typedef struct _XmPannerClassRec {	/* Panner widget class */
    CoreClassPart core_class;
    XmPrimitiveClassPart primitive_class;
    XmPannerClassPart panner_class;
} XmPannerClassRec;

typedef struct {
  Boolean doing;	       	/* tmp graphics in progress */
  Boolean showing;		/* true if tmp graphics displayed */
  Position startx, starty;	/* initial position of slider */
  Position dx, dy;		/* offset loc for tmp graphics */
  Position x, y;	       	/* location for tmp graphics */
} tmpstruct;


typedef struct XmPannerPart {	/* new fields in widget */
    /* resources... */
    XtCallbackList report_callbacks;	/* callback/Callback */
    Boolean resize_to_pref;		/* resizeToPreferred/Boolean */
    Pixel foreground;			/* foreground/Foreground */
    Dimension default_scale;		/* defaultScale/DefaultScale */
    Dimension canvas_width;		/* canvasWidth/CanvasWidth */
    Dimension canvas_height;		/* canvasHeight/CanvasHeight */
    Position slider_x;			/* sliderX/SliderX */
    Position slider_y;			/* sliderY/SliderY */
    Dimension slider_width;		/* sliderWidth/SliderWidth */
    Dimension slider_height;		/* sliderHeight/SliderHeight */
    Dimension internal_border;		/* internalBorderWidth/BorderWidth */
    Boolean rubber_band;		/* true = rubber band, false = move */
    /* private data... */
    struct _XmPannerPrivateRec *private;
} XmPannerPart;

typedef struct _XmPannerRec {
    CorePart core;
    XmPrimitivePart primitive;
    XmPannerPart panner;
} XmPannerRec;

#define XmPannerIndex (XmPrimitiveIndex + 1)

#define XmPanner_report_callbacks(w) 	(((XmPannerWidget)(w))->panner.report_callbacks)
#define XmPanner_resize_to_pref(w) 	(((XmPannerWidget)(w))->panner.resize_to_pref)
#define XmPanner_foreground(w) 		(((XmPannerWidget)(w))->panner.foreground)
#define XmPanner_default_scale(w) 	(((XmPannerWidget)(w))->panner.default_scale)
#define XmPanner_canvas_width(w) 	(((XmPannerWidget)(w))->panner.canvas_width)
#define XmPanner_canvas_height(w) 	(((XmPannerWidget)(w))->panner.canvas_height)
#define XmPanner_slider_x(w) 		(((XmPannerWidget)(w))->panner.slider_x)
#define XmPanner_slider_y(w)		(((XmPannerWidget)(w))->panner.slider_y)
#define XmPanner_slider_width(w) 	(((XmPannerWidget)(w))->panner.slider_width)
#define XmPanner_slider_height(w) 	(((XmPannerWidget)(w))->panner.slider_height)
#define XmPanner_internal_border(w) 	(((XmPannerWidget)(w))->panner.internal_border)
#define XmPanner_rubber_band(w) 	(((XmPannerWidget)(w))->panner.rubber_band)

#define XmPanner_private(w) (((XmPannerWidget)(w))->panner.private)

#define XmPanner_slider_gc(w) 	(XmPanner_private(w)->slider_gc)
#define XmPanner_xor_gc(w) 	(XmPanner_private(w)->xor_gc)
#define XmPanner_haspect(w) 	(XmPanner_private(w)->haspect)
#define XmPanner_vaspect(w) 	(XmPanner_private(w)->vaspect)
#define XmPanner_tmp(w) 	(XmPanner_private(w)->tmp)
#define XmPanner_knob_x(w) 	(XmPanner_private(w)->knob_x)
#define XmPanner_knob_y(w) 	(XmPanner_private(w)->knob_y)
#define XmPanner_knob_width(w) 	(XmPanner_private(w)->knob_width)
#define XmPanner_knob_height(w) (XmPanner_private(w)->knob_height)
#define XmPanner_last_x(w) 	(XmPanner_private(w)->last_x)
#define XmPanner_last_y(w) 	(XmPanner_private(w)->last_y)

#define PANNER_HSCALE(pw,val) (XmPanner_haspect((pw)) * ((double) (val)))
#define PANNER_VSCALE(pw,val) (XmPanner_vaspect((pw)) * ((double) (val)))

#define PANNER_DSCALE(pw,val) (Dimension)  \
  (((unsigned long) (val)) * (unsigned long) XmPanner_default_scale((pw)) / 100L)

#define PANNER_OUTOFRANGE -30000

/*
 * external declarations
 */

extern XmPannerClassRec xmPannerClassRec;

#ifdef __cplusplus
}	/* Closes scope of 'extern "C"' declaration */
#endif

#ifdef _NO_PROTO

extern void _XmPrimitiveEnter() ;
extern void _XmPrimitiveLeave() ;

#else

extern void _XmPrimitiveEnter(
                        Widget wid,
                        XEvent *event,
                        String *params,
                        Cardinal *num_params) ;
extern void _XmPrimitiveLeave(
                        Widget wid,
                        XEvent *event,
                        String *params,
                        Cardinal *num_params) ;

#endif
#endif /* _XmPannerP_h */

