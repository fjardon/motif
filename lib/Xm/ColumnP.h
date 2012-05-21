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

#ifndef __columnp_h__
#define __columnp_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <Xm/XmP.h>
#include <Xm/BulletinBP.h>
#include <Xm/Column.h>

typedef struct _XmColumnClassPart {
    XtPointer extension;
} XmColumnClassPart;

typedef struct _XmColumnClassRec {
    CoreClassPart            core_class;
    CompositeClassPart       composite_class;
    ConstraintClassPart      constraint_class;
    XmManagerClassPart       manager_class;
    XmBulletinBoardClassPart bulletin_board_class;
    XmColumnClassPart	     column_class;
} XmColumnClassRec;

typedef struct _XmColumnConstraintPart {

    /* Public */
    unsigned char       label_alignment;
    unsigned char       label_type;
    unsigned char       fill_style;

    Boolean		show_label;
    Boolean		stretchable;
    
    Pixmap              label_pixmap;
    XmString		label_string;

    XmFontList		label_font_list;

    /* Private */

    Widget              label_widget;
    Dimension		request_width;
    Dimension		request_height;
    XRectangle		position;
} XmColumnConstraintPart;

typedef struct _XmColumnConstraintRec {
    XmManagerConstraintPart manager;
    XmBulletinBoardConstraintPart bboard;
    XmColumnConstraintPart  column;
} XmColumnConstraintRec, * XmColumnConstraintPtr;

typedef struct _XmColumnPart {
    
    /* Public */

    unsigned char default_label_alignment;
    unsigned char default_fill_style;
    unsigned char orientation;
    unsigned char distribution;
    
    Dimension	  item_spacing;
    Dimension	  label_spacing;

    /* Private */

    Boolean	  resize_done;

} XmColumnPart;

typedef struct _XmColumnRec {
    CorePart            core;
    CompositePart       composite;
    ConstraintPart      constraint;
    XmManagerPart       manager;
    XmBulletinBoardPart bulletin_board;
    XmColumnPart        column;
} XmColumnRec;

#define XmColumnIndex (XmBulletinBoardIndex + 1)

extern XmOffsetPtr XmColumn_offsets;
extern XmOffsetPtr XmColumnC_offsets;

#define XiColCField(w,f,t) XmConstraintField(w, XmColumnC_offsets, XmColumn, f, t)
#define XmColumnC_label_alignment(w) XiColCField(w, label_alignment, unsigned char)
#define XmColumnC_label_type(w) XiColCField(w, label_type, unsigned char)
#define XmColumnC_fill_style(w) XiColCField(w, fill_style, unsigned char)
#define XmColumnC_show_label(w) XiColCField(w, show_label, Boolean)
#define XmColumnC_stretchable(w) XiColCField(w, stretchable, Boolean)
#define XmColumnC_label_pixmap(w) XiColCField(w, label_pixmap, Pixmap)
#define XmColumnC_label_string(w) XiColCField(w, label_string, XmString)
#define XmColumnC_label_font_list(w) XiColCField(w, label_font_list, XmFontList)
#define XmColumnC_label_widget(w) XiColCField(w, label_widget, Widget)
#define XmColumnC_request_width(w) XiColCField(w, request_width, Dimension)
#define XmColumnC_request_height(w) XiColCField(w, request_height, Dimension)
#define XmColumnC_position(w) XiColCField(w, position, XRectangle)

#define XmColField(w,f,t) XmField(w, XmColumn_offsets, XmColumn, f, t)
#define XmColumn_default_label_alignment(w) XmColField(w, default_label_alignment, unsigned char)
#define XmColumn_default_fill_style(w) XmColField(w, default_fill_style, unsigned char)
#define XmColumn_orientation(w) XmColField(w, orientation, unsigned char)
#define XmColumn_distribution(w) XmColField(w, distribution, unsigned char)
#define XmColumn_item_spacing(w) XmColField(w, item_spacing, Dimension)
#define XmColumn_label_spacing(w) XmColField(w, label_spacing, Dimension)
#define XmColumn_resize_done(w) XmColField(w, resize_done, Boolean)

#ifdef __cplusplus
}
#endif

#endif /* __columnp_h__ */
