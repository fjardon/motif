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
#ifndef __TabBoxP_h__
#define __TabBoxP_h__

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

#include <Xm/XmP.h>
#include <Xm/ManagerP.h>
#include <Xm/ExtP.h>
#include <Xm/TabBox.h>
#include <Xm/TabList.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _XiTabBoxClassPart {
    XtPointer extension;
} XiTabBoxClassPart;

typedef struct _XiTabBoxClassRec {
    CoreClassPart        core_class;
    CompositeClassPart	 composite_class;
    ConstraintClassPart  constraint_class;
    XmManagerClassPart   manager_class;
    XiTabBoxClassPart    tab_box_class;
} XiTabBoxClassRec;

typedef struct _XiTabBoxPart {
    XmFontList          font_list;
    XmTabStyle          tab_style;
    XmTabMode           tab_mode;
    XmTabbedStackList           tab_list;
    XmTabOrientation    tab_orientation;
    XiTabEdge           tab_edge;
    XiTabArrowPlacement arrow_placement;
    unsigned char       orientation;
    Dimension           tab_margin_width;
    Dimension           tab_margin_height;
    Dimension           tab_label_spacing;
    Dimension           highlight_thickness;
    int                 tab_corner_percent;
    Dimension           tab_offset;
    Boolean	        uniform_tab_size;
    Boolean		tab_auto_select;
    Boolean		use_image_cache;

    Pixel		select_color;
    Pixmap		select_pixmap;

    int                 num_stacks;
    int			selected_index;
    int			traversal_index;

    Boolean		stacked_effect;
    
    XtCallbackList      select_callback;
    XtCallbackList      unselect_callback;

    /* Private */
    GC               _tab_GC;
    GC               _text_GC;
    Pixmap           _gray_stipple;
    XRectangle       *_wanted;
    int              _num_wanted;
    struct _XiTabRect *_actual;
    int              _num_actual;
    int              _selected;
    int		     _keyboard;

    int		     _armed_tab;
    
    int              _scroll_x;
    XRectangle	     _scroll_rect;

    int              _corner_size;

    int		     _num_columns;
    int		     _num_rows;

    /*
     * The following data memebers are used for the rotation of 
     * the pixmap and the text.
     */
    int              _bitmap_width;
    int		     _bitmap_height;
    Pixmap           _bitmap;

    GC		     _zero_GC;
    GC		     _one_GC;

    Widget	     _canvas;
    Widget           _left_arrow;
    Widget           _right_arrow;

    Boolean          _inited;

    struct _XiCache *_cache;
    int              _cache_size;
} XiTabBoxPart;

/*
 * Access macros for instance variables
 */
#define XiTabBoxIndex (XmManagerIndex + 1)
extern XmOffsetPtr XiTabBox_offsets;

#define XiTabBoxField(w,f,t) XmField(w, XiTabBox_offsets, XiTabBox, f, t)

#define XiTabBox_font_list(w) XiTabBoxField(w, font_list, XmFontList)
#define XiTabBox_tab_style(w) XiTabBoxField(w, tab_style, XmTabStyle)
#define XiTabBox_tab_mode(w) XiTabBoxField(w, tab_mode, XmTabMode)
#define XiTabBox_tab_list(w) XiTabBoxField(w, tab_list, XmTabbedStackList)
#define XiTabBox_tab_orientation(w) XiTabBoxField(w, tab_orientation, XmTabOrientation)
#define XiTabBox_tab_edge(w) XiTabBoxField(w, tab_edge, XiTabEdge)
#define XiTabBox_arrow_placement(w) XiTabBoxField(w, arrow_placement, XiTabArrowPlacement)
#define XiTabBox_orientation(w) XiTabBoxField(w, orientation, unsigned char)
#define XiTabBox_tab_margin_width(w) XiTabBoxField(w, tab_margin_width, Dimension)
#define XiTabBox_tab_margin_height(w) XiTabBoxField(w, tab_margin_height, Dimension)
#define XiTabBox_tab_label_spacing(w) XiTabBoxField(w, tab_label_spacing, Dimension)
#define XiTabBox_highlight_thickness(w) XiTabBoxField(w, highlight_thickness, Dimension)
#define XiTabBox_tab_corner_percent(w) XiTabBoxField(w, tab_corner_percent, int)
#define XiTabBox_tab_offset(w) XiTabBoxField(w, tab_offset, Dimension)
#define XiTabBox_uniform_tab_size(w) XiTabBoxField(w, uniform_tab_size, Boolean)
#define XiTabBox_tab_auto_select(w) XiTabBoxField(w, tab_auto_select, Boolean)
#define XiTabBox_use_image_cache(w) XiTabBoxField(w, use_image_cache, Boolean)
#define XiTabBox_select_color(w) XiTabBoxField(w, select_color, Pixel)
#define XiTabBox_select_pixmap(w) XiTabBoxField(w, select_pixmap, Pixmap)
#define XiTabBox_num_stacks(w) XiTabBoxField(w, num_stacks, int)
#define XiTabBox_selected_index(w) XiTabBoxField(w, selected_index, int)
#define XiTabBox_traversal_index(w) XiTabBoxField(w, traversal_index, int)
#define XiTabBox_stacked_effect(w) XiTabBoxField(w, stacked_effect, Boolean)
#define XiTabBox_select_callback(w) XiTabBoxField(w, select_callback, XtCallbackList)
#define XiTabBox_unselect_callback(w) XiTabBoxField(w, unselect_callback, XtCallbackList)
#define XiTabBox__tab_GC(w) XiTabBoxField(w, _tab_GC, GC)
#define XiTabBox__text_GC(w) XiTabBoxField(w, _text_GC, GC)
#define XiTabBox__gray_stipple(w) XiTabBoxField(w, _gray_stipple, Pixmap)
#define XiTabBox__wanted(w) XiTabBoxField(w, _wanted, XRectangle*)
#define XiTabBox__num_wanted(w) XiTabBoxField(w, _num_wanted, int)
#define XiTabBox__actual(w) XiTabBoxField(w, _actual, struct _XiTabRect*)
#define XiTabBox__num_actual(w) XiTabBoxField(w, _num_actual, int)
#define XiTabBox__selected(w) XiTabBoxField(w, _selected, int)
#define XiTabBox__keyboard(w) XiTabBoxField(w, _keyboard, int)
#define XiTabBox__armed_tab(w) XiTabBoxField(w, _armed_tab, int)
#define XiTabBox__scroll_x(w) XiTabBoxField(w, _scroll_x, int)
#define XiTabBox__scroll_rect(w) XiTabBoxField(w, _scroll_rect, XRectangle)
#define XiTabBox__corner_size(w) XiTabBoxField(w, _corner_size, int)
#define XiTabBox__num_columns(w) XiTabBoxField(w, _num_columns, int)
#define XiTabBox__num_rows(w) XiTabBoxField(w, _num_rows, int)
#define XiTabBox__bitmap_width(w) XiTabBoxField(w, _bitmap_width, int)
#define XiTabBox__bitmap_height(w) XiTabBoxField(w, _bitmap_height, int)
#define XiTabBox__bitmap(w) XiTabBoxField(w, _bitmap, Pixmap)
#define XiTabBox__zero_GC(w) XiTabBoxField(w, _zero_GC, GC)
#define XiTabBox__one_GC(w) XiTabBoxField(w, _one_GC, GC)
#define XiTabBox__canvas(w) XiTabBoxField(w, _canvas, Widget)
#define XiTabBox__left_arrow(w) XiTabBoxField(w, _left_arrow, Widget)
#define XiTabBox__right_arrow(w) XiTabBoxField(w, _right_arrow, Widget)
#define XiTabBox__inited(w) XiTabBoxField(w, _inited, Boolean)
#define XiTabBox__cache(w) XiTabBoxField(w, _cache, struct _XiCache*)
#define XiTabBox__cache_size(w) XiTabBoxField(w, _cache_size, int)

typedef struct _XiTabBoxRec {
    CorePart        core;
    CompositePart   composite;
    ConstraintPart  constraint;
    XmManagerPart   manager;
    XiTabBoxPart    tab_box;
} XiTabBoxRec;

#ifdef __cplusplus
} /* Close scope of 'extern "C"' declaration */
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* __TabBoxP_h__ */
