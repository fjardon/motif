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

#ifndef _OutlineP_h
#define _Outlinels -P_h

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

/************************************************************
*	INCLUDE FILES
*************************************************************/

#include <Xm/HierarchyP.h>
#include <Xm/Outline.h>
#include <Xm/xmlist.h>

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

/************************************************************
*	MACROS
*************************************************************/

#define XiOutlineIndex (XmHierarchyIndex + 1)

extern XmOffsetPtr XiOutline_offsets;
extern XmOffsetPtr XiOutlineC_offsets;

#define XiOutlineField(w,f,t) XmField(w, XiOutline_offsets, XiOutline, f, t)
#define XiOutline_top_node_of_display(w) XiOutlineField(w, top_node_of_display, OutlineConstraints)
#define XiOutline_max_width(w) XiOutlineField(w, max_width, Dimension)
#define XiOutline_max_widget_width(w) XiOutlineField(w, max_widget_width, Dimension)
#define XiOutline_child_op_list(w) XiOutlineField(w, child_op_list, XmList)
#define XiOutline_ul_point(w) XiOutlineField(w, ul_point, XPoint)
#define XiOutline_lr_point(w) XiOutlineField(w, lr_point, XPoint)
#define XiOutline_draw_gc(w) XiOutlineField(w, draw_gc, GC)

#define XiOutline_indent_space(w) XiOutlineField(w, indent_space, Dimension)
#define XiOutline_constrain_width(w) XiOutlineField(w, constrain_width, Boolean)
#define XiOutline_connect_nodes(w) XiOutlineField(w, connect_nodes, Boolean)



/*
 * WARNING!
 *
 * These macros don't use the standard fieldmacro(widget) form.  They take
 * _pointers to OutlineConstraintsRec structures_.  Be careful.
 */
#define XiOutlineCField(constraints, variable, type) \
        (*(type *)(((char *) constraints) + \
        XiOutlineC_offsets[XiOutlineIndex] + \
        XtOffsetOf(XiOutlineConstraintPart, variable)))

#define XiOutlineC_top_node_of_display(c) XiOutlineCField(c, top_node_of_display, HierarchyConstraintRec*)
#define XiOutlineC_widget_x(c) XiOutlineCField(c, widget_x, Position)
#define XiOutlineC_open_close_x(c) XiOutlineCField(c, open_close_x, Position)
#define XiOutlineC_height(c) XiOutlineCField(c, height, Dimension)
#define XiOutlineC_new_x(c) XiOutlineCField(c, new_x, Position)
#define XiOutlineC_new_y(c) XiOutlineCField(c, new_y, Position)
#define XiOutlineC_oc_new_x(c) XiOutlineCField(c, oc_new_x, Position)
#define XiOutlineC_oc_new_y(c) XiOutlineCField(c, oc_new_y, Position)
#define XiOutlineC_map(c) XiOutlineCField(c, map, Boolean)
#define XiOutlineC_unmap(c) XiOutlineCField(c, unmap, Boolean)
#define XiOutlineC_move(c) XiOutlineCField(c, move, Boolean)

/************************************************************
*	GLOBAL DECLARATIONS
*************************************************************/

typedef void (*XiOutlineCalcLocationProc)(Widget, Boolean);
typedef int (*XiOutlineMaxWidthProc)(Widget);
    
typedef struct {
    /*Calculates the maximum width of the outline.*/
    XiOutlineMaxWidthProc calc_max_width;
    /* Calculates the locations of the objects. */
    XiOutlineCalcLocationProc calc_locations;
    /* Just in case we need it later. */
    XtPointer extension;
} OutlineClassPart;

typedef struct _XiOutlineClassRec {
    CoreClassPart		core_class;
    CompositeClassPart		composite_class;
    ConstraintClassPart		constraint_class;
    XmManagerClassPart		manager_class;
    HierarchyClassPart          hierarchy_class;
    OutlineClassPart    	outline_class;
} XiOutlineClassRec;

typedef struct _OutlineNodeInfo {
    /*
     * Public (Resource) data.
     */

    /*
     * Private data.
     */

    HierarchyConstraintRec * top_node_of_display;

    Position widget_x, open_close_x; /*location of node and open/close button*/

    Dimension height;	/* height of this row (max of node and open button). */

    Position new_x, new_y, oc_new_x, oc_new_y;
    Boolean map, unmap, move;
} OutlineNodeInfo;

typedef OutlineNodeInfo XiOutlineConstraintPart;

typedef struct _OutlineConstraintRec {
    XmManagerConstraintPart manager;
    HierNodeInfo 	hierarchy;
    OutlineNodeInfo 	outline;
} OutlineConstraintRec, *OutlineConstraints;


typedef struct _OutlinePart {
    /* Resources */
    Dimension indent_space;	/* The number of pixels to indent each level */
 
    /* Private State */

    OutlineConstraints top_node_of_display;

    Dimension max_width;	/* Width of the widest row. */
    Dimension max_widget_width;	/* Width of the widgets in the widest row. */

    XmList child_op_list;         /* List of child operations */
    XPoint ul_point, lr_point;  /* Bounding box for exposure compression */

    /* more resources */
    Boolean constrain_width;
    Boolean connect_nodes; 

    /* more private */
    GC draw_gc;

} OutlinePart;

typedef OutlinePart XiOutlinePart;

typedef struct _XiOutlineRec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    XmManagerPart	manager;
    HierarchyPart	hierarchy;
    OutlinePart		outline;
} XiOutlineRec;

#define XtInheritCalcMaxWidth 		((XiOutlineMaxWidthProc)_XtInherit)
#define XtInheritCalcLocations 		((XiOutlineCalcLocationProc)_XtInherit)

/************************************************************
*	EXTERNAL DECLARATIONS
*************************************************************/

extern XiOutlineClassRec 	xiOutlineClassRec;

/************************************************************
*	STATIC DECLARATIONS
*************************************************************/

#ifdef __cplusplus
}	/* Closes scope of 'extern "C"' declaration */
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* _OutlineP_h */
