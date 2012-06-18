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
#ifndef __Xi_TabList_h__
#define __Xi_TabList_h__

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {XmPIXMAP_TOP, XmPIXMAP_BOTTOM, XmPIXMAP_RIGHT, XmPIXMAP_LEFT,
	      XmPIXMAP_NONE, XmPIXMAP_ONLY} XmPixmapPlacement;
typedef enum {XiTAB_VALUE_COPY, XiTAB_VALUE_SHARE} XiTabValueMode;

typedef enum {XiTAB_CMP_VISUAL, XiTAB_CMP_SIZE, XiTAB_CMP_EQUAL} XiTabResult;

#define XiCOLOR_DYNAMIC ((Pixel)-1)
#define XiPIXMAP_DYNAMIC ((Pixmap) 3)
#define XiTAB_LAST_POSITION -1
#define XiTAB_NOT_FOUND -1

#define XiTAB_LABEL_STRING      (1L<<0)
#define XiTAB_LABEL_PIXMAP      (1L<<1)
#define XiTAB_PIXMAP_PLACEMENT  (1L<<2)
#define XiTAB_BACKGROUND        (1L<<3)
#define XiTAB_FOREGROUND        (1L<<4)
#define XiTAB_VALUE_MODE        (1L<<5)
#define XiTAB_LABEL_ALIGNMENT   (1L<<6)
#define XiTAB_STRING_DIRECTION  (1L<<7)
#define XiTAB_BACKGROUND_PIXMAP	(1L<<8)
#define XiTAB_SENSITIVE         (1L<<9)
#define XiTAB_ALL_FLAGS         (XiTAB_LABEL_STRING|XiTAB_LABEL_PIXMAP|\
				 XiTAB_PIXMAP_PLACEMENT|XiTAB_BACKGROUND|\
				 XiTAB_FOREGROUND|XiTAB_VALUE_MODE|\
				 XiTAB_LABEL_ALIGNMENT|XiTAB_STRING_DIRECTION|\
				 XiTAB_BACKGROUND_PIXMAP|XiTAB_SENSITIVE)

typedef struct _XiTabAttributeRec {
    XmString          label_string;	/* default: NULL                     */
    XmStringDirection string_direction; /* default: XmSTRING_DIRECTION_L_TO_R*/
    Pixmap            label_pixmap;	/* default: XmUNSPECIFIED_PIXMAP     */
    int               label_alignment;  /* default: XmALIGNEMENT_CENTER      */
    XmPixmapPlacement pixmap_placement; /* default: XmPIXMAP_RIGHT           */
    Pixel             foreground;       /* default: XiCOLOR_DYNAMIC          */
    Pixel             background;       /* default: XiCOLOR_DYNAMIC          */
    Pixmap            background_pixmap;/* default: XiPIXMAP_DYNAMIC         */
    Boolean	      sensitive;        /* default: True                     */
    XiTabValueMode    value_mode;       /* default: XiTAB_VALUE_COPY         */
} XiTabAttributeRec, * XmTabAttributes;

typedef struct _XmTabbedStackListRec *XmTabbedStackList;

XmTabbedStackList XmTabbedStackListCreate(
#ifndef _NO_PROTO
void
#endif
);

XmTabbedStackList XmTabbedStackListCopy(
#ifndef _NO_PROTO
XmTabbedStackList
#endif
);

void XmTabbedStackListFree(
#ifndef _NO_PROTO
XmTabbedStackList
#endif
);

void XmTabbedStackListRemove(
#ifndef _NO_PROTO
XmTabbedStackList, int
#endif
);

int XmTabbedStackListInsert(
#ifndef _NO_PROTO
XmTabbedStackList, int, XtValueMask, XmTabAttributes
#endif
);

int XmTabbedStackListAppend(
#ifndef _NO_PROTO
XmTabbedStackList, XtValueMask, XmTabAttributes
#endif
);

void XmTabbedStackListModify(
#ifndef _NO_PROTO
XmTabbedStackList, int, XtValueMask, XmTabAttributes
#endif
);

void XmTabbedStackListQuery(
#ifndef _NO_PROTO
XmTabbedStackList, int, XmTabAttributes
#endif
);

int XmTabbedStackListFind(
#ifndef _NO_PROTO
XmTabbedStackList, XmString
#endif
);

void XmTabbedStackListSimpleRemove(
#ifndef _NO_PROTO
XmTabbedStackList, XmString
#endif
);

int XmTabbedStackListSimpleInsert(
#ifndef _NO_PROTO
XmTabbedStackList, int, XmString
#endif
);

int XmTabbedStackListSimpleAppend(
#ifndef _NO_PROTO
XmTabbedStackList, XmString
#endif
);

void XmTabbedStackListSimpleModify(
#ifndef _NO_PROTO
XmTabbedStackList, int, XmString
#endif
);

XmString XmTabbedStackListSimpleQuery(
#ifndef _NO_PROTO
XmTabbedStackList, int
#endif
);

XiTabResult XmTabbedStackListCompare(
#ifndef _NO_PROTO
XmTabbedStackList, XmTabbedStackList
#endif
);

void XiTabAttibutesFree(
#ifndef _NO_PROTO
XmTabAttributes
#endif
);

#ifdef __cplusplus
}	/* Closes scope of 'extern "C"' declaration */
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* __Xi_TabList_h__ */
