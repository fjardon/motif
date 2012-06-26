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

#ifndef _XmExt18ListP_h
#define _XmExt18ListP_h

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

/************************************************************
*	INCLUDE FILES
*************************************************************/
#include <Xm/ManagerP.h>
#include <Xm/PrimitiveP.h>

#include <Xm/Ext18List.h>

/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * IList Stuff.
 */

#define XmExt18List_DEFAULT_VISIBLE_COUNT	5	/* XmNvisibleItemCount */

/************************************************************
*	MACROS
*************************************************************/

#define XmExt18ListIndex (XmManagerIndex + 1)
#define XmI18ListIndex (XmPrimitiveIndex + 1)

#define XmI18List_selection_policy(w) (((XmI18ListWidget)(w))->ilist.selection_policy)
#define XmI18List_num_columns(w) (((XmI18ListWidget)(w))->ilist.num_columns)
#define XmI18List_column_titles(w) (((XmI18ListWidget)(w))->ilist.column_titles)
#define XmI18List_num_rows(w) (((XmI18ListWidget)(w))->ilist.num_rows)
#define XmI18List_row_data(w) (((XmI18ListWidget)(w))->ilist.row_data)
#define XmI18List_first_col_pixmaps(w) (((XmI18ListWidget)(w))->ilist.first_col_pixmaps)
#define XmI18List_font_list(w) (((XmI18ListWidget)(w))->ilist.font_list)
#define XmI18List_v_bar(w) (((XmI18ListWidget)(w))->ilist.v_bar)
#define XmI18List_h_bar(w) (((XmI18ListWidget)(w))->ilist.h_bar)
#define XmI18List_first_row(w) (((XmI18ListWidget)(w))->ilist.first_row)
#define XmI18List_first_col(w) (((XmI18ListWidget)(w))->ilist.first_col)
#define XmI18List_double_click(w) (((XmI18ListWidget)(w))->ilist.double_click)
#define XmI18List_single_select(w) (((XmI18ListWidget)(w))->ilist.single_select)
#define XmI18List_selected_header(w) (((XmI18ListWidget)(w))->ilist.selected_header)
#define XmI18List_sort_functions(w) (((XmI18ListWidget)(w))->ilist.sort_functions)
#define XmI18List_string_direction(w) (((XmI18ListWidget)(w))->ilist.string_direction)
#define XmI18List_alignment(w) (((XmI18ListWidget)(w))->ilist.alignment)
#define XmI18List_column_widths(w) (((XmI18ListWidget)(w))->ilist.column_widths)
#define XmI18List_end(w) (((XmI18ListWidget)(w))->ilist.end)
#define XmI18List_anchor(w) (((XmI18ListWidget)(w))->ilist.anchor)
#define XmI18List_sep_y(w) (((XmI18ListWidget)(w))->ilist.sep_y)
#define XmI18List_title_row_height(w) (((XmI18ListWidget)(w))->ilist.title_row_height)
#define XmI18List_row_height(w) (((XmI18ListWidget)(w))->ilist.row_height)
#define XmI18List_gc(w) (((XmI18ListWidget)(w))->ilist.gc)
#define XmI18List_rev_gc(w) (((XmI18ListWidget)(w))->ilist.rev_gc)
#define XmI18List_stippled_gc(w) (((XmI18ListWidget)(w))->ilist.stippled_gc)
#define XmI18List_stippled_rev_gc(w) (((XmI18ListWidget)(w))->ilist.stippled_rev_gc)
#define XmI18List_inv_gc(w) (((XmI18ListWidget)(w))->ilist.inv_gc)
#define XmI18List_state(w) (((XmI18ListWidget)(w))->ilist.state)
#define XmI18List_timeout(w) (((XmI18ListWidget)(w))->ilist.timeout)
#define XmI18List_working_row(w) (((XmI18ListWidget)(w))->ilist.working_row)
#define XmI18List_working_col(w) (((XmI18ListWidget)(w))->ilist.working_col)
#define XmI18List_time(w) (((XmI18ListWidget)(w))->ilist.time)
#define XmI18List_left_loc(w) (((XmI18ListWidget)(w))->ilist.left_loc)
#define XmI18List_search_column(w) (((XmI18ListWidget)(w))->ilist.search_column)
#define XmI18List_visible_rows(w) (((XmI18ListWidget)(w))->ilist.visible_rows)
#define XmI18List_new_visual_style(w) (((XmI18ListWidget)(w))->ilist.new_visual_style)
#define XmI18List_entry_background_pixel(w) (((XmI18ListWidget)(w))->ilist.entry_background_pixel)
#define XmI18List_entry_background_use(w) (((XmI18ListWidget)(w))->ilist.entry_background_use)
#define XmI18List_entry_background_gc(w) (((XmI18ListWidget)(w))->ilist.entry_background_gc)
#define XmI18List_entry_background_fill_gc(w) (((XmI18ListWidget)(w))->ilist.entry_background_fill_gc)
#define XmI18List_entry_background_rev_gc(w) (((XmI18ListWidget)(w))->ilist.entry_background_rev_gc)
#define XmI18List_entry_background_stippled_gc(w) (((XmI18ListWidget)(w))->ilist.entry_background_stippled_gc)
#define XmI18List_entry_background_stippled_rev_gc(w) (((XmI18ListWidget)(w))->ilist.entry_background_stippled_rev_gc)
#define XmI18List_entry_background_inv_gc(w) (((XmI18ListWidget)(w))->ilist.entry_background_inv_gc)

#define XmExt18List_title(w) (((XmExt18ListWidget)(w))->ext_list.title)
#define XmExt18List_find_label(w) (((XmExt18ListWidget)(w))->ext_list.find_label)
#define XmExt18List_double_click(w) (((XmExt18ListWidget)(w))->ext_list.double_click)
#define XmExt18List_single_select(w) (((XmExt18ListWidget)(w))->ext_list.single_select)
#define XmExt18List_show_find(w) (((XmExt18ListWidget)(w))->ext_list.show_find)
#define XmExt18List_title_wid(w) (((XmExt18ListWidget)(w))->ext_list.title_wid)
#define XmExt18List_frame(w) (((XmExt18ListWidget)(w))->ext_list.frame)
#define XmExt18List_ilist(w) (((XmExt18ListWidget)(w))->ext_list.ilist)
#define XmExt18List_v_bar(w) (((XmExt18ListWidget)(w))->ext_list.v_bar)
#define XmExt18List_h_bar(w) (((XmExt18ListWidget)(w))->ext_list.h_bar)
#define XmExt18List_find(w) (((XmExt18ListWidget)(w))->ext_list.find)
#define XmExt18List_find_text(w) (((XmExt18ListWidget)(w))->ext_list.find_text)
#define XmExt18List_last_search(w) (((XmExt18ListWidget)(w))->ext_list.last_search)
#define XmExt18List_item_found(w) (((XmExt18ListWidget)(w))->ext_list.item_found)
#define XmExt18List_not_found(w) (((XmExt18ListWidget)(w))->ext_list.not_found)
#define XmExt18List_visible_rows(w) (((XmExt18ListWidget)(w))->ext_list.visible_rows)
#define XmExt18List_title_string(w) (((XmExt18ListWidget)(w))->ext_list.title_string)

/************************************************************
*	GLOBAL DECLARATIONS
*************************************************************/

/*
 * IList widget definitions.
 */

/* I18List struct passed to Convert proc for drag and drop */
typedef struct _XmI18ListDragConvertStruct
{
    Widget    w;
    XmString *strings;
    int       num_items;
    Pixmap    pixmap;
} XmI18ListDragConvertStruct;

typedef struct _I18ListClassPart {
    XtPointer extension;	/* Just in case we need it later. */
} I18ListClassPart;

typedef struct _XmI18ListClassRec {
    CoreClassPart		core_class;
    XmPrimitiveClassPart	primitive;
    I18ListClassPart		ilist_class;
} XmI18ListClassRec;
externalref XmI18ListClassRec xmI18ListClassRec;

typedef struct _XmI18ListPart {
    /*
     * Resources
     */

    unsigned char selection_policy; /* selection mode - kat 12-28-90 */
    short num_columns;		/* number of columns in the list. */
    XmString * column_titles;	/* title for each column. */
    short num_rows;		/* number of rows in the list. */
    Xm18RowInfo * row_data;	/* Data to put into each column. */
    Boolean first_col_pixmaps;	/* Should we put mini_icons in the first
				   column of each entry? */		
    XmFontList font_list;	/* This widget's font list. */

    Widget v_bar, h_bar;	/* Scrollbars that may be used
				   to scroll this widget. */

    short first_row;		/* which row is at the top of the display. */
    short first_col;		/* which column is at the far left. */

    XtCallbackList double_click; /* The double click callback list. */
    XtCallbackList single_select; /*The single click callback list. */

    short selected_header;	/* The currently selected header. */

    Xm18SortFunction *sort_functions; /* The client supplied sort functions */

    unsigned char string_direction;
    unsigned char alignment;

    /* 
     * Private State
     */

    short * column_widths;	/* Width of each column. */
    short end;			/* The non-anchor end point. */ 
    short anchor;		/* The anchor point for the extended 
				   selection. */

    int sep_y;			/*location of the top of the separator line.*/
    
    short title_row_height;	/* height of title row */
    short row_height;	/* height of all other data rows */

    GC gc;			/* The graphics context for normal text. */
    GC rev_gc;			/* The graphics context for inverted text. */
    GC stippled_gc;		/* The graphics context for normal text. */
    GC stippled_rev_gc;		/* The graphics context for inverted text. */
    GC inv_gc;			/* The graphics context for inverting areas. */

    unsigned short state;	/* The state of this widget. */
    XtIntervalId timeout;	/* The mulit - click timout. */

    short working_row, working_col; /* A Working row and column. */
    Time time;			/*The server time of the last button click. */

    int left_loc;		/* left margin in pixels. */

    short search_column;	/* added for I18List Find support */

    int visible_rows;		/* Visible item (row) count */

    Boolean new_visual_style;

    Pixel entry_background_pixel;
    Boolean entry_background_use;
    GC entry_background_gc;			
    GC entry_background_fill_gc;			
    GC entry_background_stippled_gc;		
    GC entry_background_stippled_rev_gc;	
    GC entry_background_inv_gc;			
    GC entry_background_rev_gc;			

    Boolean check_set_render_table; /* used in CheckSetRenderTable */

    XmI18ListDragConvertStruct * drag_conv;
} XmI18ListPart;

typedef struct _XmI18ListRec {
    CorePart		core;
    XmPrimitivePart	primitive;
    XmI18ListPart	ilist;
} XmI18ListRec;

/*
 * Extended List.
 */

typedef struct {
    XtPointer extension;	/* Just in case we need it later. */
} XmExt18ListClassPart;

typedef struct _XmExt18ListClassRec {
    CoreClassPart		core_class;
    CompositeClassPart		composite_class;
    ConstraintClassPart		constraint_class;
    XmManagerClassPart		manager_class;
    XmExt18ListClassPart		ext_list_class;
} XmExt18ListClassRec;

typedef struct {
    /* resources */

    XmString title;		/* Title for the list (backwards compatible) */
    XmString find_label;	/* label for Find button */

    XtCallbackList double_click; /* The double click callbacks. */
    XtCallbackList single_select; /* The single click callbacks. -kat */

	Boolean show_find;    /* whether to display the Find button and textF */

    /* private state */

    Widget title_wid;		/* The list title widget. */
    Widget frame;		/* Frame to display list into. */
    Widget ilist;		/* The internal list widget. */
    Widget v_bar, h_bar;	/* The scrollbars. */
    Widget find, find_text;	/* Widgets used for a find. */

    String last_search;

    XtCallbackList item_found;	/* Called when find succeeds */
    XtCallbackList not_found;	/* Called when find doesn't succeed */

    int visible_rows;		/* visible items (mirrored in XmI18ListPart) */
    XmString title_string;	/* (preferred use) Title for the list */

    Boolean check_set_select_callback;
} XmExt18ListPart;

typedef struct _XmExt18ListRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    XmManagerPart	manager;
    XmExt18ListPart	ext_list;
} XmExt18ListRec;

/************************************************************
*	EXTERNAL DECLARATIONS
*************************************************************/

Xm18RowInfo ** XmI18ListGetSelectedRows(
#ifndef _NO_PROTO
Widget 
#endif
);

void
Xm18IListUnselectItem( 
#ifndef _NO_PROTO
Widget , Xm18RowInfo *
#endif
);

void
Xm18IListUnselectAllItems( 
#ifndef _NO_PROTO
Widget
#endif
);

void
XmI18ListToggleRow(
#ifndef _NO_PROTO
Widget , short
#endif
);

typedef struct _XmI18ListClassRec	*XmI18ListWidgetClass;
typedef struct _XmI18ListRec	*XmI18ListWidget;

extern XmExt18ListClassRec xmExt18ListClassRec;

extern XmI18ListClassRec xiI18ListClassRec;
extern WidgetClass xmI18ListWidgetClass; 

#if defined(__cplusplus)
}
#endif

/************************************************************
*	STATIC DECLARATIONS
*************************************************************/

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* _XmExt18ListP_h */
