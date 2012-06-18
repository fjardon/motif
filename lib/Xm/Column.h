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

#ifndef __column_h__
#define __column_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <Xm/Xm.h>
#include <Xm/Ext.h>
#include <Xm/XmExtDefs.h>

extern WidgetClass xmColumnWidgetClass;

typedef struct _XmColumnClassRec * XmColumnWidgetClass;
typedef struct _XmColumnRec      * XmColumnWidget;

enum { XmFILL_UNSPECIFIED, XmFILL_FLUSH, XmFILL_RAGGED };

enum { XmDISTRIBUTE_TIGHT, XmDISTRIBUTE_SPREAD };

#ifndef XmIsColumn
#define XmIsColumn(w) (XtIsSubclass(w, xmColumnWidgetClass))
#endif

#if NeedFunctionPrototypes
extern Widget XmCreateColumn(Widget, String, ArgList, Cardinal);
extern Widget XmColumnGetChildLabel(Widget);
#else
extern Widget XmCreateColumn();
extern Widget XmColumnGetChildLabel();
#endif

#ifdef __cplusplus
}
#endif

#endif /* __column_h__ */
