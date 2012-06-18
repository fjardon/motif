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

typedef struct _XmTabBoxClassRec {
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
extern XmOffsetPtr XmTabBox_offsets;

#define XmTabBoxField(w,f,t) XmField(w, XmTabBox_offsets, XiTabBox, f, t)

#define XmTabBox_font_list(w) XmTabBoxField(w, font_list, XmFontList)
#define XmTabBox_tab_style(w) XmTabBoxField(w, tab_style, XmTabStyle)
#define XmTabBox_tab_mode(w) XmTabBoxField(w, tab_mode, XmTabMode)
#define XmTabBox_tab_list(w) XmTabBoxField(w, tab_list, XmTabbedStackList)
#define XmTabBox_tab_orientation(w) XmTabBoxField(w, tab_orientation, XmTabOrientation)
#define XmTabBox_tab_edge(w) XmTabBoxField(w, tab_edge, XiTabEdge)
#define XmTabBox_arrow_placement(w) XmTabBoxField(w, arrow_placement, XiTabArrowPlacement)
#define XmTabBox_orientation(w) XmTabBoxField(w, orientation, unsigned char)
#define XmTabBox_tab_margin_width(w) XmTabBoxField(w, tab_margin_width, Dimension)
#define XmTabBox_tab_margin_height(w) XmTabBoxField(w, tab_margin_height, Dimension)
#define XmTabBox_tab_label_spacing(w) XmTabBoxField(w, tab_label_spacing, Dimension)
#define XmTabBox_highlight_thickness(w) XmTabBoxField(w, highlight_thickness, Dimension)
#define XmTabBox_tab_corner_percent(w) XmTabBoxField(w, tab_corner_percent, int)
#define XmTabBox_tab_offset(w) XmTabBoxField(w, tab_offset, Dimension)
#define XmTabBox_uniform_tab_size(w) XmTabBoxField(w, uniform_tab_size, Boolean)
#define XmTabBox_tab_auto_select(w) XmTabBoxField(w, tab_auto_select, Boolean)
#define XmTabBox_use_image_cache(w) XmTabBoxField(w, use_image_cache, Boolean)
#define XmTabBox_select_color(w) XmTabBoxField(w, select_color, Pixel)
#define XmTabBox_select_pixmap(w) XmTabBoxField(w, select_pixmap, Pixmap)
#define XmTabBox_num_stacks(w) XmTabBoxField(w, num_stacks, int)
#define XmTabBox_selected_index(w) XmTabBoxField(w, selected_index, int)
#define XmTabBox_traversal_index(w) XmTabBoxField(w, traversal_index, int)
#define XmTabBox_stacked_effect(w) XmTabBoxField(w, stacked_effect, Boolean)
#define XmTabBox_select_callback(w) XmTabBoxField(w, select_callback, XtCallbackList)
#define XmTabBox_unselect_callback(w) XmTabBoxField(w, unselect_callback, XtCallbackList)
#define XmTabBox__tab_GC(w) XmTabBoxField(w, _tab_GC, GC)
#define XmTabBox__text_GC(w) XmTabBoxField(w, _text_GC, GC)
#define XmTabBox__gray_stipple(w) XmTabBoxField(w, _gray_stipple, Pixmap)
#define XmTabBox__wanted(w) XmTabBoxField(w, _wanted, XRectangle*)
#define XmTabBox__num_wanted(w) XmTabBoxField(w, _num_wanted, int)
#define XmTabBox__actual(w) XmTabBoxField(w, _actual, struct _XiTabRect*)
#define XmTabBox__num_actual(w) XmTabBoxField(w, _num_actual, int)
#define XmTabBox__selected(w) XmTabBoxField(w, _selected, int)
#define XmTabBox__keyboard(w) XmTabBoxField(w, _keyboard, int)
#define XmTabBox__armed_tab(w) XmTabBoxField(w, _armed_tab, int)
#define XmTabBox__scroll_x(w) XmTabBoxField(w, _scroll_x, int)
#define XmTabBox__scroll_rect(w) XmTabBoxField(w, _scroll_rect, XRectangle)
#define XmTabBox__corner_size(w) XmTabBoxField(w, _corner_size, int)
#define XmTabBox__num_columns(w) XmTabBoxField(w, _num_columns, int)
#define XmTabBox__num_rows(w) XmTabBoxField(w, _num_rows, int)
#define XmTabBox__bitmap_width(w) XmTabBoxField(w, _bitmap_width, int)
#define XmTabBox__bitmap_height(w) XmTabBoxField(w, _bitmap_height, int)
#define XmTabBox__bitmap(w) XmTabBoxField(w, _bitmap, Pixmap)
#define XmTabBox__zero_GC(w) XmTabBoxField(w, _zero_GC, GC)
#define XmTabBox__one_GC(w) XmTabBoxField(w, _one_GC, GC)
#define XmTabBox__canvas(w) XmTabBoxField(w, _canvas, Widget)
#define XmTabBox__left_arrow(w) XmTabBoxField(w, _left_arrow, Widget)
#define XmTabBox__right_arrow(w) XmTabBoxField(w, _right_arrow, Widget)
#define XmTabBox__inited(w) XmTabBoxField(w, _inited, Boolean)
#define XmTabBox__cache(w) XmTabBoxField(w, _cache, struct _XiCache*)
#define XmTabBox__cache_size(w) XmTabBoxField(w, _cache_size, int)

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
