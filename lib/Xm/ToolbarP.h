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

#ifndef _XmToolbarP_h
#define _XmToolbarP_h

/************************************************************
*	INCLUDE FILES
*************************************************************/

#include <Xm/ManagerP.h>
#include <Xm/Toolbar.h>

/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

#ifdef __cplusplus 
extern "C" {
#endif

typedef struct _ToolbarGroup
{
    unsigned char	group_id;
    WidgetList		members;
    unsigned char	num_members;
} ToolbarGroup;


/************************************************************
*	MACROS
*************************************************************/

#define XmToolbarIndex (XmManagerIndex + 1)

#define XmToolbar_GroupMembers(w, i)    (((XmToolbar_group_lists(w))[(i)])->members)
#define XmToolbar_GroupMemCount(w, i)   (((XmToolbar_group_lists(w))[(i)])->num_members)

#define XmToolbar_group_spacing(w) 	(((XmToolbarWidget)(w))->toolbar.group_spacing)
#define XmToolbar_v_margin(w) 		(((XmToolbarWidget)(w))->toolbar.v_margin)
#define XmToolbar_h_margin(w)		(((XmToolbarWidget)(w))->toolbar.h_margin)
#define XmToolbar_orientation(w) 	(((XmToolbarWidget)(w))->toolbar.orientation)
#define XmToolbar_popup_enabled(w) 	(((XmToolbarWidget)(w))->toolbar.popup_enabled)
#define XmToolbar_popup_delay(w) 	(((XmToolbarWidget)(w))->toolbar.popup_delay)
#define XmToolbar_enter_child_callback(w) (((XmToolbarWidget)(w))->toolbar.enter_child_callback)
#define XmToolbar_leave_child_callback(w) (((XmToolbarWidget)(w))->toolbar.leave_child_callback)
#define XmToolbar_popup_label_callback(w) (((XmToolbarWidget)(w))->toolbar.popup_label_callback)
#define XmToolbar_popdown_label_callback(w) (((XmToolbarWidget)(w))->toolbar.popdown_label_callback)
#define XmToolbar_minor_col_space(w) 	(((XmToolbarWidget)(w))->toolbar.minor_col_space)
#define XmToolbar_group_lists(w) 	(((XmToolbarWidget)(w))->toolbar.group_lists)
#define XmToolbar_num_groups(w) 	(((XmToolbarWidget)(w))->toolbar.num_groups)
#define XmToolbar_popped_up(w) 		(((XmToolbarWidget)(w))->toolbar.popped_up)
#define XmToolbar_popup_timer(w) 	(((XmToolbarWidget)(w))->toolbar.popup_timer)
#define XmToolbar_popup_shell(w) 	(((XmToolbarWidget)(w))->toolbar.popup_shell)
#define XmToolbar_popup_label(w) 	(((XmToolbarWidget)(w))->toolbar.popup_label)
#define XmToolbar_popup_background(w) 	(((XmToolbarWidget)(w))->toolbar.popup_background)
#define XmToolbar_popup_foreground(w) 	(((XmToolbarWidget)(w))->toolbar.popup_foreground)
#define XmToolbar_popup_fontlist(w) 	(((XmToolbarWidget)(w))->toolbar.popup_fontlist)
#define XmToolbar_popup_delay_restore(w) (((XmToolbarWidget)(w))->toolbar.popup_delay_restore)
#define XmToolbar_do_place(w) 		(((XmToolbarWidget)(w))->toolbar.do_place)
#define XmToolbar_save_width(w) 	(((XmToolbarWidget)(w))->toolbar.save_width)

#define XmToolbarC_group(w) (((XmToolbarConstraints)((w)->core.constraints))->toolbar.group)
#define XmToolbarC_position(w) (((XmToolbarConstraints)((w)->core.constraints))->toolbar.position)
#define XmToolbarC_entry_label(w) (((XmToolbarConstraints)((w)->core.constraints))->toolbar.entry_label)
#define XmToolbarC_entry_data(w) (((XmToolbarConstraints)((w)->core.constraints))->toolbar.entry_data)

/************************************************************
*	GLOBAL DECLARATIONS
*************************************************************/

typedef struct {
    /* Resources */
    XtPointer extension;	/* Just in case we need it later. */
} XmToolbarClassPart;

typedef struct _XmToolbarClassRec {
    CoreClassPart		core_class;
    CompositeClassPart		composite_class;
    ConstraintClassPart		constraint_class;
    XmManagerClassPart		manager_class;
    XmToolbarClassPart	        toolbar_class;
} XmToolbarClassRec;

typedef struct {
    /* resources */
    Dimension group_spacing;	/* groupSpacing			   */
    Dimension v_margin, h_margin; /* The margins.		   */
    unsigned char orientation;	/* The orientation of the toolbar. */
    Boolean popup_enabled;	/* Popup the label string help	*/
    Cardinal popup_delay;	/* msec delay between enter and popup */
    XtCallbackList enter_child_callback;	
    XtCallbackList leave_child_callback;	
    XtCallbackList popup_label_callback;	
    XtCallbackList popdown_label_callback;	

    /* private state */
    Dimension 		minor_col_space;
    ToolbarGroup	**group_lists;
    int			num_groups;
    Boolean		popped_up;
    XtIntervalId	popup_timer;
    Widget 		popup_shell;
    Widget 		popup_label;
    Pixel		popup_background;
    Pixel		popup_foreground;
    XmFontList		popup_fontlist;
    Cardinal            popup_delay_restore;
    Boolean		popup_check_set_render_table;

    Boolean		do_place;
    Dimension		save_width;
} XmToolbarPart;

typedef struct _XmToolbarRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    XmManagerPart	manager;
    XmToolbarPart	toolbar;
} XmToolbarRec;

/* Toolbar Constraint Record */
typedef struct _XmToolbarConstraintPart {
    unsigned char group;	/* Entry's grouping within toolbar	*/
    unsigned char position;	/* Entry's position within the group	*/
    XmString	entry_label;	/* Entry's label string	(if needed)	*/
    XtPointer	entry_data;	/* User-defined data			*/
} XmToolbarConstraintPart;
    
/*
 * In case we need to add to this later.
 */

typedef struct _XmToolbarConstraintsRec {
    XmManagerConstraintPart	manager;
    XmToolbarConstraintPart	toolbar;
} XmToolbarConstraintsRec, *XmToolbarConstraints;

/************************************************************
*	EXTERNAL DECLARATIONS
*************************************************************/

extern XmToolbarClassRec xmToolbarClassRec;

/************************************************************
*	STATIC DECLARATIONS
*************************************************************/

#ifdef __cplusplus 
}	/* Closes scope of 'extern "C"' declaration */
#endif

extern void _XmForegroundColorDefault(
                        Widget widget,
                        int offset,
                        XrmValue *value) ;
extern void _XmHighlightColorDefault(
                        Widget widget,
                        int offset,
                        XrmValue *value) ;
extern void _XmBackgroundColorDefault(
                        Widget widget,
                        int offset,
                        XrmValue *value) ;

#endif /* _XmToolbarP_h */
