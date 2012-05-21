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

#ifndef __xm_table_h__
#define __xm_table_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <Xm/Ext.h>

#ifndef XmIsTable
#define XmIsTable(w) XtIsSubclass((w), xmTableWidgetClass)
#endif

extern WidgetClass xmTableWidgetClass;

typedef struct _XmTableClassRec *XmTableWidgetClass;
typedef struct _XmTableRec      *XmTableWidget;

#if NeedFunctionPrototypes
typedef String (*XmStringFunc)(Widget);
typedef void   (*XmFreeProc)(XtPointer);
typedef void   (*XmRenderProc)(Widget, Widget, XRectangle*, GC, String);
typedef String (*XmFetchCellValueFunc)(Widget, Widget, int);
typedef void   (*XmStoreCellValueProc)(Widget, Widget, int, String);
typedef void   (*XmFreeCellValueProc)(Widget, String);

extern Widget XmCreateTable(Widget, String, ArgList, Cardinal);
extern void XmTableGotoCell(Widget, Widget, int);
extern void XmTableUpdate(Widget);
#else
typedef String (*XmStringFunc)();
typedef void   (*XmFreeProc)();
typedef void   (*XmRenderProc)();
typedef String (*XmFetchCellValueFunc)();
typedef void   (*XmStoreCellValueProc)();
typedef void   (*XmFreeCellValueProc)();

extern Widget XmCreateTable();
extern void XmTableGotoCell();
extern void XmTableUpdate();
#endif

#define XmCR_CELL_VERIFY_TRAVERSE 0
#define XmCR_CELL_TRAVERSE        1
typedef struct _XmTableCallbackStruct {
    int     reason;
    XEvent  *event;
    Widget  from_cell;
    int     from_row;
    Widget  to_cell;
    int     to_row;
    Boolean confirm;
} XmTableCallbackStruct;

#ifdef __cplusplus
}
#endif

#endif /* __xm_table_h__ */

