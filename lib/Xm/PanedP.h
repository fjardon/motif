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

#ifndef _XmPanedP_h
#define _XmPanedP_h

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

#include <Xm/ManagerP.h>
#include <Xm/Paned.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 *
 * Paned Widget Private Data
 *
 *********************************************************************/

/* New fields for the Paned widget class record */

typedef struct _XmPanedClassPart {
    XtPointer extension;	
} XmPanedClassPart;

/* Full Class record declaration */
typedef struct _XmPanedClassRec {
    CoreClassPart       core_class;
    CompositeClassPart  composite_class;
    ConstraintClassPart constraint_class;
    XmManagerClassPart	manager_class;
    XmPanedClassPart     paned_class;
} XmPanedClassRec;

extern XmPanedClassRec xmPanedClassRec;

/* Paned constraint record */
typedef struct _XmPanedConstraintsPart {
  /* Resources. */
    Dimension	min;		/* Minimum height */
    Dimension	max;		/* Maximum height */
    Boolean	allow_resize;	/* TRUE iff child resize requests are ok */
    Boolean     show_sash;	/* TRUE iff child will have sash below it,
				   when it is not the bottom pane. */
    Boolean	skip_adjust;	/* TRUE iff child's height should not be */
				/* changed without explicit user action. */
    int		position;	/* position location in Paned (relative to
				   other children) ** NIY ** */
    Dimension   preferred_size;	/* The Preferred size of the pane.
				   Iff this is zero then ask child for size.*/
    Boolean     resize_to_pref;	/* resize this pane to its preferred size
				   on a resize or change managed after 
				   realize. */
    Boolean     is_a_pane;	/* INTERNAL INFO */


  /* Private state. */
    Position	delta;		/* Desired Location */
    Position	olddelta;	/* The last value of delta. */
    Dimension	wp_size;	/* widget's preferred on size */ 
    Dimension	wp_off_size;	/* widget's preferred off size */ 
    int         size;		/* the size the widget will actually get. */
    Widget	sash;		/* The sash for this child */
    Widget      separator;      /* The separator for this child */

    Boolean     prefs_inited;	/* Preferences have been inited... */
} XmPanedConstraintsPart, *Pane;

typedef struct _XmPanedConstraintsRec {
    XmManagerConstraintPart	manager;
    XmPanedConstraintsPart	paned;
} XmPanedConstraintsRec, *XmPanedConstraints;

/*
 * Ugliness:  the XmOffset macros require this naming convention, yet
 * epak already depends on the pluralized name ;(
 */
typedef XmPanedConstraintsPart XmPanedConstraintPart;

/*
 * The Pane Stack Structure.
 */

typedef struct _PaneStack {
    struct _PaneStack * next;	/* The next element on the stack. */
    Pane pane;			/* The pane in this element on the stack. */
    int start_size;		/* The size of this element when it was pushed
				   onto the stack. */
} PaneStack;

#define NO_ADJUST    ((char) 0)
#define BEGAN_ADJUST ((char) 1)

/* New Fields for the XmPaned widget record */
typedef struct {
    /* resources */
    Position    sash_indent;               /* Location of sashs (per motif) */
    Boolean     refiguremode;              /* Whether to refigure changes 
					      right now */
    XtTranslations sash_translations;      /* sash translation table */
    Dimension   internal_bw;	           /* internal border width. */
    unsigned char orientation;	           /* Orientation of paned widget. */

    Cursor	cursor;		/* Cursor for paned window */

    /* Things from Motif behaviour */
    Boolean     separator_on;        /* make separator visible */
    Dimension   margin_width;        /* space between right and left edges of
                                        Paned window and it's children */
    Dimension   margin_height;       /* space between top and bottom edges of
                                        Paned window and it's children */

    /* sash modifying resources */
    Dimension   sash_width;            /* Modify sash width */
    Dimension   sash_height;           /* Modify sash height */
    Dimension   sash_shadow_thickness; /* Modify sash shadow_thickness */

    /* Private */
    Boolean	recursively_called;        /* for ChangeManaged */
    Boolean	resize_children_to_pref;   /* override constraint resources
					      and resize all children to
					      preferred size. */
    short       increment_count;           /* Sash increment count */
    char	repane_status;		   /* current adjust state. */
    Position    start_loc;	           /* mouse origin when adjusting */
    GC          flipgc;                    /* GC to use when animating
					      borders */
    short	num_panes;                 /* count of managed panes */
    short       num_slots;            	   /*number of avail. slots for kids */

    PaneStack * stack;		           /* The pane stack for this widget.*/
    WidgetList  managed_children;	   /* keep track of managed children */
 
    Boolean     allow_unused_space;      /* should the paned widget allow
                                          * a pane to be shrunk to the point
                                          * that there is unused space at
                                          * the bottom/right of the widget */
} XmPanedPart;

extern XmOffsetPtr XmPaned_offsets;
extern XmOffsetPtr XmPanedC_offsets;

/**************************************************************************
 *
 * Full instance record declaration
 *
 **************************************************************************/

typedef struct _XmPanedRec {
    CorePart       core;
    CompositePart  composite;
    ConstraintPart constraint;
    XmManagerPart  manager;
    XmPanedPart     paned;
} XmPanedRec;

/************************************************************
 *
 *  Functions
 *
 ************************************************************/
void _XmFromPanedPixels(Widget, int, XtArgVal *);
XmImportOperator _XmToPanedPixels(Widget, int, XtArgVal *);


/************************************************************
 *
 *  Macros
 *
 ************************************************************/
/*
 * XmResolvePartOffsets stuff follows
 */
#define XmPanedIndex (XmManagerIndex + 1)

#define PanedField(w, f, t) XmField(w, XmPaned_offsets, XmPaned, f, t)
#define XmPaned_sash_indent(w) PanedField(w, sash_indent, Position)
#define XmPaned_refiguremode(w) PanedField(w, refiguremode, Boolean)
#define XmPaned_sash_translations(w) PanedField(w, sash_translations, XtTranslations)
#define XmPaned_internal_bw(w) PanedField(w, internal_bw, Dimension)
#define XmPaned_orientation(w) PanedField(w, orientation, unsigned char)
#define XmPaned_cursor(w) PanedField(w, cursor, Cursor)
#define XmPaned_separator_on(w) PanedField(w, separator_on, Boolean)
#define XmPaned_margin_width(w) PanedField(w, margin_width, Dimension)
#define XmPaned_margin_height(w) PanedField(w, margin_height, Dimension)
#define XmPaned_sash_width(w) PanedField(w, sash_width, Dimension)
#define XmPaned_sash_height(w) PanedField(w, sash_height, Dimension)
#define XmPaned_sash_shadow_thickness(w) PanedField(w, sash_shadow_thickness, Dimension)
#define XmPaned_recursively_called(w) PanedField(w, recursively_called, Boolean)
#define XmPaned_resize_children_to_pref(w) PanedField(w, resize_children_to_pref, Boolean)
#define XmPaned_increment_count(w) PanedField(w, increment_count, short)
#define XmPaned_repane_status(w) PanedField(w, repane_status, char)
#define XmPaned_start_loc(w) PanedField(w, start_loc, Position)
#define XmPaned_flipgc(w) PanedField(w, flipgc, GC)
#define XmPaned_num_panes(w) PanedField(w, num_panes, short)
#define XmPaned_num_slots(w) PanedField(w, num_slots, short)
#define XmPaned_stack(w) PanedField(w, stack, PaneStack*)
#define XmPaned_managed_children(w) PanedField(w, managed_children, WidgetList)
#define XmPaned_allow_unused_space(w) PanedField(w, allow_unused_space, Boolean)

#define PanedCField(w,f,t) XmConstraintField(w, XmPanedC_offsets, \
                                             XmPaned, f, t)
#define XmPanedC_min(w) PanedCField(w, min, Dimension)
#define XmPanedC_max(w) PanedCField(w, max, Dimension)
#define XmPanedC_allow_resize(w) PanedCField(w, allow_resize, Boolean)
#define XmPanedC_show_sash(w) PanedCField(w, show_sash, Boolean)
#define XmPanedC_skip_adjust(w) PanedCField(w, skip_adjust, Boolean)
#define XmPanedC_position(w) PanedCField(w, position, int)
#define XmPanedC_preferred_size(w) PanedCField(w, preferred_size, Dimension)
#define XmPanedC_resize_to_pref(w) PanedCField(w, resize_to_pref, Boolean)
#define XmPanedC_is_a_pane(w) PanedCField(w, is_a_pane, Boolean)
#define XmPanedC_delta(w) PanedCField(w, delta, Position)
#define XmPanedC_olddelta(w) PanedCField(w, olddelta, Position)
#define XmPanedC_wp_size(w) PanedCField(w, wp_size, Dimension)
#define XmPanedC_wp_off_size(w) PanedCField(w, wp_off_size, Dimension)
#define XmPanedC_size(w) PanedCField(w, size, int)
#define XmPanedC_sash(w) PanedCField(w, sash, Widget)
#define XmPanedC_separator(w) PanedCField(w, separator, Widget)
#define XmPanedC_prefs_inited(w) PanedCField(w, prefs_inited, Boolean)

#define ForceSashOff(pane) ((pane)->min == (pane)->max)
#define PaneConsRec(w)	((XmPanedConstraints)(w)->core.constraints)
#define PaneInfo(w) ((Pane)((char*)((w)->core.constraints) \
			    + XmPanedC_offsets[XmPanedIndex]))
#define HasSash(w)	(XmPanedC_sash(w) != NULL)
#define HasSep(w)	(XmPanedC_separator(w) != NULL)

#define PaneIndex(w)	(XmPanedC_position(w))
#define IsVert(w)       (XmPaned_orientation(w) == XmVERTICAL)

#define IsLastPane(pw, childP) ((XmPaned_managed_children((pw)) + \
				 XmPaned_num_panes((pw)) - 1) == childP)

#define ForAllPaned(pw, childP) \
  for ( ((childP) = (XmPaned_managed_children((pw)))) ; \
	((childP) < ((XmPaned_managed_children((pw))) \
                    + (XmPaned_num_panes((pw))))) ; \
	(childP)++ )

#define NthPane(pw, paneIndex) (XmPaned_managed_children((pw)) + (paneIndex))

#ifdef _cplusplus
}	/* Closes scope of 'extern "C"' declaration */
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* _XmPanedP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
