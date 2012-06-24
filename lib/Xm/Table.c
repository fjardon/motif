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

/*
 * XmResolveAllPartOffset warning
 *
 * Note that this file contains two widget classes, the XmTable and
 * the XmTableClip widgets.  Both share the _same_ constraint part, but
 * not their superclass (the clip widget is a subclass of DrawingArea,
 * not manager).  Since neither superclass has any constraint
 * definitions, the widgets were originally written to share the same
 * XiC() macro to get the constraint part.  This _may_ be a problem in
 * the future, if someone ever adds constraints to the drawing area.  It
 * hasn't happened as of CDE 2.1 though.
 */
#include <stdlib.h>
#include <stdio.h>

#include <Xm/XmP.h>
#include <Xm/DrawP.h>
#include <Xm/ExtP.h>
#include <Xm/TableP.h>
#include <X11/cursorfont.h>
#include <Xm/Label.h>
#include <Xm/DrawingAP.h>
#include <Xm/Text.h>
#include <Xm/TextFP.h>
#include <Xm/ScrollBar.h>

#include "XmStrDefsI.h"

#ifdef __cplusplus
extern "C" {
#endif

#if NeedWidePrototypes
#define WIDE_PROTO_TYPE	unsigned int
#else
#define WIDE_PROTO_TYPE	unsigned char
#endif

Pixel 		_XmAccessColorData(XmColorData *, WIDE_PROTO_TYPE);
XmColorData 	*_XmGetColors(Screen *, Colormap, Pixel);
void 		_XmForegroundColorDefault(Widget, int, XrmValue *);



#ifndef Min
#define Min(a,b) (((int)(a)<(int)(b))?a:b)
#endif

#ifndef Max
#define Max(a,b) (((int)(a)>(int)(b))?a:b)
#endif

#if NeedFunctionPrototypes

static void ClassInitialize(void);
static void ClipClassInitialize(void);
static void ClassPartInitialize(WidgetClass);
static void Initialize(Widget, Widget, ArgList, Cardinal*);
static void Destroy(Widget);
static void Resize(Widget);
static void Redisplay(Widget, XEvent*, Region);
static Boolean SetValues(Widget, Widget, Widget, ArgList, Cardinal*);
static XtGeometryResult QueryGeometry(Widget, XtWidgetGeometry*,
				      XtWidgetGeometry*);
static XtGeometryResult GeometryManager(Widget, XtWidgetGeometry*,
					XtWidgetGeometry*);
static void ClipChangeManaged(Widget);
static void ChangeManaged(Widget);
static void InsertChild(Widget);

static void ConstraintInitialize(Widget, Widget, ArgList, Cardinal*);
static void ConstraintDestroy(Widget);
static Boolean ConstraintSetValues(Widget, Widget, Widget, ArgList,
				   Cardinal*);

static void Layout(XmTableWidget);
static void LayoutTitles(XmTableWidget);
static void LayoutCells(XmTableWidget);
static void LayoutActive(XmTableWidget);
static void CalcTableInfo(XmTableWidget);
static void XiSetActive(XmTableWidget, Widget, int, int, Boolean);
#ifdef DYNAMIC_COLUMN_SIZING
static Widget GetMoveWidget(XmTableWidget, int);
#endif
static Boolean IsHorizVisible(XmTableWidget, Widget);
static Boolean IsVertVisible(XmTableWidget, Widget, int);

static void TableExposeCB(Widget, XtPointer, XtPointer);
static void TableResizeCB(Widget, XtPointer, XtPointer);
static void TableInputCB(Widget, XtPointer, XtPointer);
static void TitleLabelExposeEH(Widget, XtPointer, XEvent*, Boolean*);
#ifdef DYNAMIC_COLUMN_SIZING
static void TitleMotionEH(Widget, XtPointer, XEvent*, Boolean*);
static void TitleButtonPressEH(Widget, XtPointer, XEvent*, Boolean*);
#endif
static void TableVScrollCB(Widget, XtPointer, XtPointer);
static void TableHScrollCB(Widget, XtPointer, XtPointer);

static void XiCallRender(XmTableWidget, Widget, XRectangle*, String);

static void XmTableNextTabGroup(Widget, XEvent*, String*, Cardinal*);
static void XmTablePrevTabGroup(Widget, XEvent*, String*, Cardinal*);
static void XmTableNextCell(Widget, XEvent*, String*, Cardinal*);
static void XmTablePreviousCell(Widget, XEvent*, String*, Cardinal*);
static void XmTableUpCell(Widget, XEvent*, String*, Cardinal*);
static void XmTableDownCell(Widget, XEvent*, String*, Cardinal*);
static void XmTableRightCell(Widget, XEvent*, String*, Cardinal*);
static void XmTableLeftCell(Widget, XEvent*, String*, Cardinal*);
static void XmTablePageDown(Widget, XEvent*, String*, Cardinal*);
static void XmTablePageUp(Widget, XEvent*, String*, Cardinal*);
static void XmTableBeginData(Widget, XEvent*, String*, Cardinal*);
static void XmTableEndData(Widget, XEvent*, String*, Cardinal*);
static void XmTableRestoreAndSend(Widget, XEvent*, String*, Cardinal*);
static void XmTableRestore(Widget, XEvent*, String*, Cardinal*);
static void XmTableReinstateTraversal(Widget, XEvent*, String*, Cardinal*);

static void XiConfigureClip(Widget, Position, Position, Dimension,
			    Dimension);

static void _XiDefaultTitleForeground(Widget, int, XrmValue*);
static void _XiDefaultTitleTopShadowColor(Widget, int, XrmValue*);
static void _XiDefaultTitleTopShadowPixmap(Widget, int, XrmValue*);
static void _XiDefaultTitleBottomShadowColor(Widget, int, XrmValue*);
static void _XiDefaultTitleShadeColor(Widget, int, XrmValue*);
static void _XiDefaultTableColor(Widget, int, XrmValue*);
static void _XiDefaultLineColor(Widget, int, XrmValue*);
static void _XiDefaultShadeColor(Widget, int, XrmValue*);

static Widget _XmTextFocusWidget(Widget);
static void _XiTextSelectText(Widget);
static void _XiTextUnselectText(Widget);
static void _XiTextSetValue(Widget, String);
static String _XiTextGetValue(Widget);
static void _XiTextRender(Widget, Widget, XRectangle*, GC, String);

static Widget _XiComboBoxFocusWidget(Widget);
static void _XiComboBoxSelectText(Widget);
static void _XiComboBoxUnselectText(Widget);
static void _XiComboBoxSetValue(Widget, String);
static String _XiComboBoxGetValue(Widget);
static void _XiComboBoxRender(Widget, Widget, XRectangle*, GC, String);

static GC CreateGC(Widget, Pixel, Pixmap, Boolean);
static void SetUpGCs(XmTableWidget);

#ifdef DYNAMIC_COLUMN_SIZING
static void DrawMoveLine(XmTableWidget, int, Boolean);
#endif
static void PassDownResources(XmTableWidget, ArgList, Cardinal, ArgList,
			      Cardinal);

static SmScrollMgr SmScrollMgrCreate(void);
static void SmScrollMgrDestroy(SmScrollMgr);
static void SmScrollAdd(SmScrollMgr, int, int);
static void SmScrollRemove(SmScrollMgr);
static void SmScrollEvent(SmScrollMgr, XEvent*);

static void MoveScrollRegion(XmTableWidget, int, int, int, int);

static Boolean XiValidInputChild(Widget);

#else

static void ClassInitialize();
static void ClipClassInitialize();
static void ClassPartInitialize();
static void Initialize();
static void Destroy();
static void Resize();
static void Redisplay();
static Boolean SetValues();

static XtGeometryResult QueryGeometry();
static XtGeometryResult GeometryManager();
static void ClipChangeManaged();
static void ChangeManaged();
static void InsertChild();

static void ConstraintInitialize();
static void ConstraintDestroy();
static Boolean ConstraintSetValues();

static void Layout();
static void LayoutTitles();
static void LayoutCells();
static void LayoutActive();
static void CalcTableInfo();
static void XiSetActive();
#ifdef DYNAMIC_COLUMN_SIZING
static Widget GetMoveWidget();
#endif
static Boolean IsHorizVisible();
static Boolean IsVertVisible();

static void TableExposeCB();
static void TableResizeCB();
static void TableInputCB();
static void TitleLabelExposeEH();
#ifdef DYNAMIC_COLUMN_SIZING
static void TitleMotionEH();
static void TitleButtonPressEH();
#endif
static void TableVScrollCB();
static void TableHScrollCB();

static void XiCallRender();

static void XmTableNextTabGroup();
static void XmTablePrevTabGroup();
static void XmTableNextCell();
static void XmTablePreviousCell();
static void XmTableUpCell();
static void XmTableDownCell();
static void XmTableRightCell();
static void XmTableLeftCell();
static void XmTablePageDown();
static void XmTablePageUp();
static void XmTableBeginData();
static void XmTableEndData();
static void XmTableRestoreAndSend();
static void XmTableRestore();
static void XmTableReinstateTraversal();

static void XiConfigureClip();

static void _XiDefaultTitleForeground();
static void _XiDefaultTitleTopShadowColor();
static void _XiDefaultTitleTopShadowPixmap();
static void _XiDefaultTitleBottomShadowColor();
static void _XiDefaultTitleShadeColor();
static void _XiDefaultTableColor();
static void _XiDefaultLineColor();
static void _XiDefaultShadeColor();

static Widget _XmTextFocusWidget();
static void _XiTextSelectText();
static void _XiTextUnselectText();
static void _XiTextSetValue();
static String _XiTextGetValue();
static void _XiTextRender();

static Widget _XiComboBoxFocusWidget();
static void _XiComboBoxSelectText();
static void _XiComboBoxUnselectText();
static void _XiComboBoxSetValue();
static String _XiComboBoxGetValue();
static void _XiComboBoxRender();

static GC CreateGC();
static void SetUpGCs();

#ifdef DYNAMIC_COLUMN_SIZING
static void DrawMoveLine();
#endif
static void PassDownResources();

static SmScrollMgr SmScrollMgrCreate();
static void SmScrollMgrDestroy();
static void SmScrollAdd();
static void SmScrollRemove();
static void SmScrollEvent();

static void MoveScrollRegion();

static Boolean XiValidInputChild();

#endif

#define INITIAL_DIMENSION	 100
#define DEFAULT_MARGIN           10
#define DEFAULT_SPACING          3
#define DEFAULT_SHADOW_THICKNESS 2
#define DEFAULT_BUFFER_SIZE      5
#define MIN_CELL_SIZE		 10

#define MOVE_THRESHOLD 5
#define XmNO_MOVE      0
#define XiSTART_MOVE   1
#define XiDOING_MOVE   2

#define XiVisNone         0
#define XiVisUpper	  (1L<<0)
#define XiVisLower	  (1L<<1)
#define XiVisLeft	  (1L<<2)
#define XiVisRight	  (1L<<3)
#define XiVisUpperLeft    (XiVisUpper | XiVisLeft)
#define XiVisUpperRight   (XiVisUpper | XiVisRight)
#define XiVisLowerLeft    (XiVisLower | XiVisLeft)
#define XiVisLowerRight   (XiVisLower | XiVisRight)

#define XiValidPixmap(p) ((p) != (Pixmap) NULL && (p) != XmUNSPECIFIED_PIXMAP)
#define XiResizeWidth(t) XmTable_resize_width(t)
#define XiResizeHeight(t) XmTable_resize_height(t)
#define SetActive(t,a,y,v) XiSetActive(t,a,y,v,False)
#define XiReleaseGC(w, g) if( (g) != NULL ) {\
			      XtReleaseGC((Widget) (w), (g)); \
			      (g) = NULL; \
			  } else
#define XiValidChild(c) ((c) != NULL && XtIsManaged(c) && \
			 !(c)->core.being_destroyed)

#define XiC(w) (*((XmTableConstraintPart*) \
		  (((char*)((w)->core.constraints)) + \
		   XmTableC_offsets[XmTableIndex])))

#define XiEditable(w) (XiC(w).editable)
#define XiResizable(w) (XiC(w).resizable)
#define XiXOffset(w)      (XiC(w).offset)
#define XiIndex(w) (XiC(w).position_index)
#define XiCallSelectText(c) if( XiC(c).select_text_proc != NULL ) \
                            { \
			        (*(XiC(c).select_text_proc))(c); \
			    } else
#define XiCallUnselectText(c) if( XiC(c).unselect_text_proc != NULL ) \
                              { \
			          (*(XiC(c).unselect_text_proc))(c); \
			      } else
#define XiCallFocusWidget(c) ( (XiC(c).focus_widget_func != NULL) \
			       ? (*(XiC(c).focus_widget_func))(c) \
			       : (c) )
#define XiCallSetValue(c,s) if( XiC(c).set_value_proc != NULL ) \
                            { \
			        (*(XiC(c).set_value_proc))(c,s); \
			    } else
#define XiCallGetValue(c) ( (XiC(c).get_value_func != NULL) \
			    ? (*(XiC(c).get_value_func))(c) \
			    : NULL )
#define XiCallFree(c,v) if( XiC(c).free_proc != NULL ) \
                        { \
			    (*(XiC(c).free_proc))((XtPointer)(v)); \
			}
#define XiFetchCellValue(t,c,y) \
    ( (XmTable_fetch_cell_value_func((t)) != NULL) \
      ? (*(XmTable_fetch_cell_value_func((t))))((Widget) (t),c,y) \
      : NULL )
#define XiStoreCellValue(t,c,y,s) \
    if( XmTable_store_cell_value_proc((t)) != NULL ) \
    { \
        (*(XmTable_store_cell_value_proc((t))))((Widget) (t),c,y,s); \
    } else
#define XiFreeCellValue(t,s) \
    if( XmTable_free_cell_value_proc((t)) != NULL ) \
    { \
        (*(XmTable_free_cell_value_proc((t))))((Widget) (t),s); \
    } else

/*
 * Actions -
 *     Below are the definitions of the actions used by the children
 *     of the Table.  We add these actions to the Table so that its
 * children can find them without the actions having to be global.  */
static XtActionsRec actions[] = {
    { "XmTableNextTabGroup",	(XtActionProc) XmTableNextTabGroup	},
    { "XmTablePrevTabGroup",	(XtActionProc) XmTablePrevTabGroup	},
    { "XmTableNextCell",	(XtActionProc) XmTableNextCell     	},
    { "XmTablePreviousCell",	(XtActionProc) XmTablePreviousCell 	},
    { "XmTableUpCell",		(XtActionProc) XmTableUpCell       	},
    { "XmTableDownCell",	(XtActionProc) XmTableDownCell	   	},
    { "XmTableRightCell",	(XtActionProc) XmTableRightCell    	},
    { "XmTableLeftCell",	(XtActionProc) XmTableLeftCell	    	},
    { "XmTablePageDown",	(XtActionProc) XmTablePageDown	    	},
    { "XmTablePageUp",		(XtActionProc) XmTablePageUp 		},
    { "XmTableBeginData",	(XtActionProc) XmTableBeginData      	},
    { "XmTableEndData",		(XtActionProc) XmTableEndData        	},
    { "XmTableRestoreAndSend",	(XtActionProc) XmTableRestoreAndSend	},
    { "XmTableRestore",		(XtActionProc) XmTableRestore		},
    { "XmTableReinstateTraversal", (XtActionProc)XmTableReinstateTraversal },
};

/*
 * Default Edit Translations -
 *     Below are the translations that are added to the children of
 *     the Table.  This is so that when one of these events are
 *     triggered the Table goes into traversal mode.
 */
static char default_edit_translations[] =
    "Shift Ctrl<Key>Tab:	XmTableReinstateTraversal()\n\
     Ctrl<Key>Tab:		XmTableReinstateTraversal()\n\
     Shift<Key>Tab:		XmTableReinstateTraversal()\n\
     <Key>Tab:			XmTableReinstateTraversal()\n\
     <Key>osfNextField:		XmTableReinstateTraversal()\n\
     <Key>osfPrevField:		XmTableReinstateTraversal()\n\
     <Key>osfUp:		XmTableReinstateTraversal()\n\
     <Key>osfDown:		XmTableReinstateTraversal()\n\
     <Key>osfPageDown:		XmTableReinstateTraversal()\n\
     <Key>osfPageUp:		XmTableReinstateTraversal()\n\
     <Key>osfBeginLine:		XmTableReinstateTraversal()\n\
     <Key>osfEndLine:		XmTableReinstateTraversal()";

/*
 * Default Traversal Translations -
 *     Below are the translations that are added to the children of
 *     the Table so that the Table can enforce a universal traversal
 *     model.
 */
static char default_traversal_translations[] =
    "Shift Ctrl<Key>Tab:	XmTablePrevTabGroup()\n\
     Ctrl<Key>Tab:		XmTableNextTabGroup()\n\
     Shift<Key>Tab:		XmTablePreviousCell()\n\
     <Key>Tab:			XmTableNextCell()\n\
     <Key>osfNextField:		XmTableNextCell()\n\
     <Key>osfPrevField:		XmTablePreviousCell()\n\
     <Key>osfUp:		XmTableUpCell()\n\
     <Key>osfDown:		XmTableDownCell()\n\
     <Key>osfRight:		XmTableRightCell()\n\
     <Key>osfLeft:		XmTableLeftCell()\n\
     <Key>osfPageDown:		XmTablePageDown()\n\
     <Key>osfPageUp:		XmTablePageUp()\n\
     <Key>osfBeginLine:		XmTableBeginData()\n\
     <Key>osfEndLine:		XmTableEndData()\n\
     <Key>osfDelete:		XmTableRestoreAndSend()\n\
     <Key>osfBackSpace:		XmTableRestoreAndSend()\n\
     <ButtonPress>:		XmTableRestoreAndSend()\n\
     <Key>osfActivate:		XmTableRestore()\n\
     <Key>Return:		XmTableRestore()\n\
     <Key>:			XmTableRestoreAndSend()";


/*
 * Resources -
 *     Below is the list of resources that the Table widget supports.
 */
#ifdef offset
#undef offset
#endif
#define offset(field) XmPartOffset(XmTable, field)
static XtResource resources[] = {
   { XmNshadowThickness, XmCShadowThickness,
	 XmRHorizontalDimension, sizeof(Dimension),
	 XtOffsetOf(XmTableRec, manager.shadow_thickness),
	 XmRImmediate, (XtPointer) DEFAULT_SHADOW_THICKNESS },
   { XmNshowTitles, XmCShowTitles,
	 XmRBoolean, sizeof(Boolean), offset(show_titles),
	 XmRImmediate, (XtPointer) True },
   { XmNmarginHeight, XmCMarginHeight,
	 XmRVerticalDimension, sizeof(Dimension), offset(margin_height),
	 XmRImmediate, (XtPointer) DEFAULT_MARGIN },
   { XmNmarginWidth, XmCMarginWidth,
	 XmRHorizontalDimension, sizeof(Dimension), offset(margin_width),
	 XmRImmediate, (XtPointer) DEFAULT_MARGIN },
   { XmNspacing, XmCSpacing,
	 XmRHorizontalDimension, sizeof(Dimension), offset(spacing),
	 XmRImmediate, (XtPointer) DEFAULT_SPACING },
   { XmNnumRows, XmCNumRows,
	 XmRCardinal, sizeof(Cardinal), offset(num_rows),
	 XmRImmediate, (XtPointer) 0 },
   { XmNresizeHeight, XmCResizeHeight,
	 XmRBoolean, sizeof(Boolean), offset(resize_height),
	 XmRImmediate, (XtPointer) False },
   { XmNresizeWidth, XmCResizeWidth,
	 XmRBoolean, sizeof(Boolean), offset(resize_width),
	 XmRImmediate, (XtPointer) True },
   { XmNlineThickness, XmCLineThickness,
	 XmRHorizontalDimension, sizeof(Dimension), offset(line_width),
	 XmRImmediate, (XtPointer) 1 },
   { XmNsetChildColor, XmCBoolean,
	 XmRBoolean, sizeof(Boolean), offset(set_child_color),
	 XmRImmediate, (XtPointer) True },
   { XmNtitleAlignment, XmCAlignment,
	 XmRAlignment, sizeof(unsigned char), offset(title_alignment),
	 XmRImmediate, (XtPointer) XmALIGNMENT_BEGINNING },
   { XmNtitleBackground, XmCBackground,
	 XmRPixel, sizeof(Pixel), offset(title_background),
	 XmRCallProc, (XtPointer) _XmTopShadowColorDefault },
   { XmNtitleForeground, XmCForeground,
	 XmRPixel, sizeof(Pixel), offset(title_foreground),
	 XmRCallProc, (XtPointer) _XiDefaultTitleForeground },
   { XmNtitleBackgroundPixmap, XmCBackgroundPixmap,
	 XmRXmBackgroundPixmap, sizeof(Pixmap), offset(title_background_pixmap),
	 XmRCallProc, (XtPointer) _XmManagerTopShadowPixmapDefault },
   { XmNtitleTopShadowColor, XmCTopShadowColor,
	 XmRPixel, sizeof(Pixel), offset(title_top_shadow_color),
	 XmRCallProc, (XtPointer) _XiDefaultTitleTopShadowColor },
   { XmNtitleTopShadowPixmap, XmCTopShadowPixmap,
	 XmRPrimTopShadowPixmap, sizeof(Pixmap), offset(title_top_shadow_pixmap),
	 XmRCallProc, (XtPointer) _XiDefaultTitleTopShadowPixmap },
   { XmNtitleBottomShadowColor, XmCBottomShadowColor,
	 XmRPixel, sizeof(Pixel), offset(title_bottom_shadow_color),
	 XmRCallProc, (XtPointer) _XiDefaultTitleBottomShadowColor },
   { XmNtitleBottomShadowPixmap, XmCBottomShadowPixmap,
	 XmRPrimBottomShadowPixmap, sizeof(Pixmap), offset(title_bottom_shadow_pixmap),
	 XmRImmediate, (XtPointer) XmUNSPECIFIED_PIXMAP },
   { XmNtitleShadeColor, XmCTitleShadeColor,
	 XmRPixel, sizeof(Pixel), offset(title_shade_color),
	 XmRCallProc, (XtPointer) _XiDefaultTitleShadeColor },
   { XmNtitleShadePixmap, XmCTitleShadePixmap,
	 XmRPrimBottomShadowPixmap, sizeof(Pixmap), offset(title_shade_pixmap),
	 XmRImmediate, (XtPointer) XmUNSPECIFIED_PIXMAP },
   { XmNfontList, XmCFontList,
	 XmRFontList, sizeof(XmFontList), offset(font_list),
	 XmRImmediate, (XtPointer) NULL },
   { XmNtableColor, XmCTableColor,
	 XmRPixel, sizeof(Pixel), offset(table_color),
	 XmRCallProc, (XtPointer) _XiDefaultTableColor },
   { XmNtablePixmap, XmCTablePixmap,
	 XmRPrimForegroundPixmap, sizeof(Pixmap), offset(table_pixmap),
	 XmRImmediate, (XtPointer) XmUNSPECIFIED_PIXMAP },
   { XmNlineForeground, XmCLineForeground,
	 XmRPixel, sizeof(Pixel), offset(line_color),
	 XmRCallProc, (XtPointer) _XiDefaultLineColor },
   { XmNlinePixmap, XmCLinePixmap,
	 XmRPrimForegroundPixmap, sizeof(Pixmap), offset(line_pixmap),
	 XmRImmediate, (XtPointer) XmUNSPECIFIED_PIXMAP },
   { XmNshadeColor, XmCShadeColor,
	 XmRPixel, sizeof(Pixel), offset(shade_color),
	 XmRCallProc, (XtPointer) _XiDefaultShadeColor },
   { XmNshadePixmap, XmCShadePixmap,
	 XmRPrimBottomShadowPixmap, sizeof(Pixmap), offset(shade_pixmap),
	 XmRImmediate, (XtPointer) XmUNSPECIFIED_PIXMAP },
   { XmNfetchCellValueFunc, XmCFetchCellValueFunc,
	 XmRProc, sizeof(XmFetchCellValueFunc), offset(fetch_cell_value_func),
	 XmRImmediate, (XtPointer) NULL },
   { XmNstoreCellValueProc, XmCStoreCellValueProc,
	 XmRProc, sizeof(XmStoreCellValueProc), offset(store_cell_value_proc),
	 XmRImmediate, (XtPointer) NULL },
   { XmNfreeCellValueProc, XmCFreeCellValueProc,
	 XmRProc, sizeof(XmFreeCellValueProc), offset(free_cell_value_proc),
	 XmRImmediate, (XtPointer) NULL },
   { XmNcellTraverseVerifyCallback, XmCCellTraverseVerifyCallback,
	 XmRCallback, sizeof(XtCallbackList),
	 offset(cell_traverse_verify_callback),
	 XmRImmediate, (XtPointer) NULL },
   { XmNcellTraverseCallback, XmCCellTraverseCallback,
	 XmRCallback, sizeof(XtCallbackList), offset(cell_traverse_callback),
	 XmRImmediate, (XtPointer) NULL },
};
#undef offset

/*
 * ConstraintResources -
 *     Below are the constraint resources.  These resources are
 *     duplicated because there are two widgets in this file that both
 *     use the same constraint resources.
 */
#ifdef offset
#undef offset
#endif
#define offset(field) XmConstraintPartOffset(XmTable, field)
static XtResource constraint_resources[] = {
    { XmNcolumnTitle, XmCColumnTitle,
	  XmRXmString, sizeof(XmString), offset(column_title),
	  XmRImmediate, (XtPointer) NULL },
    { XmNresizable, XmCBoolean,
	  XmRBoolean, sizeof(Boolean), offset(resizable),
	  XmRImmediate, (XtPointer) True },
    { XmNeditable, XmCEditable,
	  XmRBoolean, sizeof(Boolean), offset(editable),
	  XmRImmediate, (XtPointer) True },
    { XmNfocusWidgetFunc, XmCTableFunc,
	  XmRProc, sizeof(XmWidgetFunc), offset(focus_widget_func),
	  XmRImmediate, (XtPointer) NULL },
    { XmNselectTextProc, XmCTableProc,
	  XmRProc, sizeof(XtWidgetProc), offset(select_text_proc),
	  XmRImmediate, (XtPointer) NULL },
    { XmNunselectTextProc, XmCTableProc,
	  XmRProc, sizeof(XtWidgetProc), offset(unselect_text_proc),
	  XmRImmediate, (XtPointer) NULL },
    { XmNsetValueProc, XmCTableProc,
	  XmRProc, sizeof(XtStringProc), offset(set_value_proc),
	  XmRImmediate, (XtPointer) NULL },
    { XmNgetValueFunc, XmCTableFunc,
	  XmRProc, sizeof(XmStringFunc), offset(get_value_func),
	  XmRImmediate, (XtPointer) NULL },
    { XmNfreeProc, XmCTableProc,
	  XmRProc, sizeof(XmFreeProc), offset(free_proc),
	  XmRImmediate, (XtPointer) NULL },
    { XmNrenderProc, XmCTableProc,
	  XmRProc, sizeof(XmRenderProc), offset(render_proc),
	  XmRImmediate, (XtPointer) NULL },
};

    /*
     * Note: these use the same offset macros as above, so that we can
     * use the same access macros for both widgets.  This is OK _only_
     * because there are no constraints defined in the superclasses.
     */
static XtResource clip_constraint_resources[] = {
    { XmNcolumnTitle, XmCColumnTitle,
	  XmRString, sizeof(String), offset(column_title),
	  XmRImmediate, (XtPointer) NULL },
    { XmNresizable, XmCBoolean,
	  XmRBoolean, sizeof(Boolean), offset(resizable),
	  XmRImmediate, (XtPointer) True },
    { XmNeditable, XmCEditable,
	  XmRBoolean, sizeof(Boolean), offset(editable),
	  XmRImmediate, (XtPointer) True },
    { XmNfocusWidgetFunc, XmCTableFunc,
	  XmRProc, sizeof(XmWidgetFunc), offset(focus_widget_func),
	  XmRImmediate, (XtPointer) NULL },
    { XmNselectTextProc, XmCTableProc,
	  XmRProc, sizeof(XtWidgetProc), offset(select_text_proc),
	  XmRImmediate, (XtPointer) NULL },
    { XmNunselectTextProc, XmCTableProc,
	  XmRProc, sizeof(XtWidgetProc), offset(unselect_text_proc),
	  XmRImmediate, (XtPointer) NULL },
    { XmNsetValueProc, XmCTableProc,
	  XmRProc, sizeof(XtStringProc), offset(set_value_proc),
	  XmRImmediate, (XtPointer) NULL },
    { XmNgetValueFunc, XmCTableFunc,
	  XmRProc, sizeof(XmStringFunc), offset(get_value_func),
	  XmRImmediate, (XtPointer) NULL },
    { XmNfreeProc, XmCTableProc,
	  XmRProc, sizeof(XmFreeProc), offset(free_proc),
	  XmRImmediate, (XtPointer) NULL },
    { XmNrenderProc, XmCTableProc,
	  XmRProc, sizeof(XmRenderProc), offset(render_proc),
	  XmRImmediate, (XtPointer) NULL },
};
#undef offset

/*
 * Default Procedures -
 *     Below is the default list of interface procedures for a small
 *     class of known children.  These procedures are simply the
 *     methods that Table uses to talk to its kids.
 */
static XmTableDefaultProcRec default_procs[] = {
    { "XmTextField", NULL,
	  _XmTextFocusWidget,
	  _XiTextSelectText,
	  _XiTextUnselectText,
	  _XiTextSetValue,
	  _XiTextGetValue,
	  (XmFreeProc) XtFree,
          _XiTextRender },
    { "XmText", NULL,
	  _XmTextFocusWidget,
	  _XiTextSelectText,
	  _XiTextUnselectText,
	  _XiTextSetValue,
	  _XiTextGetValue,
	  (XmFreeProc) XtFree,
          _XiTextRender },
    { "XiCombinationBox", NULL,
	  _XiComboBoxFocusWidget,
	  _XiComboBoxSelectText,
	  _XiComboBoxUnselectText,
	  _XiComboBoxSetValue,
	  _XiComboBoxGetValue,
	  (XmFreeProc) XtFree,
          _XiComboBoxRender },
};

#ifdef XmRENAME_WIDGETS
#define xmTableClipWidgetClass  xmXXTableClipWidgetClass
#define xmTableClipClassRec xmXXTableClipClassRec
#endif

/*
 * XiClipWidget -
 *     Below is the definition of the private widget class
 *     xiClipWidgetClass.  This class is used exclusively by the
 *     XmTableWidget and is not intended to be used anywhere else.
 *
 *     The XiClipWidget is the widget that XmTable uses as a clip
 *     area for the table.  This widget is meant to pass constraint
 *     resources and other information between the table children and
 *     the Table.
 *
 *     Offset note:  this widget has not been converted to the
 *     XmResolvePartOffset idiom and does not have to be.  While it
 *     inherits from drawing area, it adds no data below the manager
 *     record.
 */
typedef struct _XmTableClipClassPart {
    XtPointer extension;
} XmTableClipClassPart;

typedef struct _XmTableClipClassRec {
    CoreClassPart          core_class;
    CompositeClassPart     composite_class;
    ConstraintClassPart    constraint_class;
    XmManagerClassPart     manager_class;
    XmDrawingAreaClassPart drawing_area_class;
    XmTableClipClassPart        clip_class;
} XmTableClipClassRec, * XmTableClipWidgetClass;

typedef struct _XmTableClipPart {
    XtPointer extension;
} XmTableClipPart;

typedef struct _XmTableClipRec {
    CorePart          core;
    CompositePart     composite;
    ConstraintPart    constraint;
    XmManagerPart     manager;
    XmDrawingAreaPart drawing_area;
    XmTableClipPart        clip;
} XmTableClipRec, * XmTableClipWidget;

XmTableClipClassRec xmTableClipClassRec = {
    { /* Core Class Part */
	(WidgetClass)&xmDrawingAreaClassRec, /* superclass		    */
	"XmTableClip",			/* class_name			    */
	sizeof(XmTableClipPart),        /* size				    */
	ClipClassInitialize,            /* class_initialize		    */
	NULL,				/* class_part_initialize	    */
	False,				/* inited			    */
	NULL,				/* initialize			    */
	(XtArgsProc) NULL,		/* initialize_hook		    */
	XtInheritRealize,		/* realize			    */
	NULL,				/* actions			    */
	0,				/* num_actions			    */
	NULL,				/* resources			    */
	0,				/* num_resources		    */
	NULLQUARK,			/* xrm_class			    */
	True,				/* compress_motion		    */
	XtExposeCompressSeries |
	    XtExposeGraphicsExpose |
	    XtExposeNoExpose,		/* compress_exposure		    */
	True,				/* compress_enterleave		    */
	False,				/* visible_interest		    */
	NULL,				/* destroy			    */
	XtInheritResize,		/* resize			    */
	XtInheritExpose,		/* expose			    */
	NULL,				/* set_values			    */
	NULL,				/* set_values_hook		    */
	XtInheritSetValuesAlmost,	/* set_values_almost		    */
	(XtArgsProc) NULL,		/* get_values_hook		    */
	NULL,				/* accept_focus			    */
	XtVersion,             /* version			    */
	(XtPointer) NULL,		/* callback_private		    */
	XtInheritTranslations,		/* tm_table			    */
	XtInheritQueryGeometry,		/* query_geometry		    */
	(XtStringProc) NULL,		/* display_accelerator		    */
	(XtPointer) NULL },		/* extension			    */
    { /* Composite Class Part */
	GeometryManager,		/* geometry_manager		    */
	ClipChangeManaged,		/* change_managed		    */
	XtInheritInsertChild,		/* insert_child			    */
	XtInheritDeleteChild,		/* delete_child			    */
	(XtPointer) NULL },		/* extension			    */
    { /* Constraint Class Part */
	clip_constraint_resources,	/* resources			    */
	XtNumber(clip_constraint_resources),/* num_resources		    */
	sizeof(XmTableConstraintPart),	/* constraint_size		    */
	NULL,				/* initialize			    */
	ConstraintDestroy,		/* destroy			    */
	ConstraintSetValues,		/* set_values			    */
	(XtPointer) NULL },		/* extension			    */
    { /* Xm Manager Class Part */
	XtInheritTranslations,		/* translations			    */
	(XmSyntheticResource*) NULL,	/* syn_resources		    */
	0,				/* num_syn_resources		    */
	(XmSyntheticResource*) NULL,	/* syn_constraint_resources	    */
	0,				/* num_syn_constraint_resources	    */
	NULL,				/* parent_process		    */
	(XtPointer) NULL },		/* extension			    */
    { /* Xm Drawing Area Class Part */
	0 },				/* mumble			    */
    { /* Xi Clip Class Part */
	(XtPointer) NULL },
};

/*
 * XmTableWidget -
 */
XmTableClassRec xmTableClassRec = {
    { /* Core Class Part */
	(WidgetClass)&xmManagerClassRec,/* superclass		    	    */
	"XmTable",			/* class_name			    */
	sizeof(XmTablePart),		/* size				    */
	ClassInitialize,		/* class_initialize		    */
	ClassPartInitialize,		/* class_part_initialize	    */
	False,				/* inited			    */
	Initialize,			/* initialize			    */
	(XtArgsProc) NULL,		/* initialize_hook		    */
	XtInheritRealize,		/* realize			    */
	actions,			/* actions			    */
	XtNumber(actions),		/* num_actions			    */
	resources,			/* resources			    */
	XtNumber(resources),		/* num_resources		    */
	NULLQUARK,			/* xrm_class			    */
	True,				/* compress_motion		    */
	True,				/* compress_exposure		    */
	True,				/* compress_enterleave		    */
	False,				/* visible_interest		    */
	Destroy,			/* destroy			    */
	Resize,				/* resize			    */
	Redisplay,			/* expose			    */
	SetValues,			/* set_values			    */
	NULL,				/* set_values_hook		    */
	XtInheritSetValuesAlmost,	/* set_values_almost		    */
	(XtArgsProc) NULL,		/* get_values_hook		    */
	NULL,				/* accept_focus			    */
	XtVersion,		/* version			    */
	(XtPointer) NULL,		/* callback_private		    */
	"",				/* tm_table			    */
	QueryGeometry,			/* query_geometry		    */
	(XtStringProc) NULL,		/* display_accelerator		    */
	(XtPointer) NULL },		/* extension			    */
    { /* Composite Class Part */
	GeometryManager,		/* geometry_manager		    */
	ChangeManaged,			/* change_managed		    */
	InsertChild,			/* insert_child			    */
	XtInheritDeleteChild,		/* delete_child			    */
	(XtPointer) NULL },		/* extension			    */
    { /* Constraint Class Part */
	constraint_resources,		/* resources			    */
	XtNumber(constraint_resources),	/* num_resources		    */
	sizeof(XmTableConstraintPart),	/* constraint_size		    */
	ConstraintInitialize,		/* initialize			    */
	NULL,				/* destroy			    */
	NULL,				/* set_values			    */
	(XtPointer) NULL },		/* extension			    */
    { /* Xm Manager Class Part */
	XtInheritTranslations,		/* translations			    */
	(XmSyntheticResource*) NULL,	/* syn_resources		    */
	0,				/* num_syn_resources		    */
	(XmSyntheticResource*) NULL,	/* syn_constraint_resources	    */
	0,				/* num_syn_constraint_resources	    */
	NULL,				/* parent_process		    */
	(XtPointer) NULL },		/* extension			    */
    { /* Xi Table Class Part */
	default_traversal_translations, /* translations    		    */
	default_edit_translations, 	/* edit_translations    	    */
	default_procs, 			/* default_procs 		    */
	XtNumber(default_procs),	/* num_default_procs		    */
	(XtPointer) NULL },		/* extension			    */
};

WidgetClass xiTableClipWidgetClass = (WidgetClass) &xmTableClipClassRec;
WidgetClass xmTableWidgetClass = (WidgetClass) &xmTableClassRec;

XmOffsetPtr XmTable_offsets;
XmOffsetPtr XmTableC_offsets;

/*
 * These are dummies to pass to XmResolveAllPartOffsets.  The actual
 * access to the TableClip's constraints is done through the XmTable's
 * offset table.  This works because the superclass defines no
 * constriants.
 */
XmOffsetPtr XmTableClip_offsets;
XmOffsetPtr XmTableClipC_offsets;

/*
 * Function:
 *     ClassInitialize()
 * Description:
 *      Handles initialization of the offset records
 * Input:
 *      none
 * Output:
 *      none
 */
#if NeedFunctionPrototypes
static void
ClassInitialize(void)
#else
static void
ClassInitialize()
#endif
{
    XmResolveAllPartOffsets(xmTableWidgetClass,
			    &XmTable_offsets,
			    &XmTableC_offsets);
}

/*
 * Function:
 *     ClipClassInitialize()
 * Description:
 *      Handles initialization of the XmTableClip class's resources
 * Input:
 *      none
 * Output:
 *      none
 */
#if NeedFunctionPrototypes
static void
ClipClassInitialize(void)
#else
static void
ClipClassInitialize()
#endif
{
    XmResolveAllPartOffsets(xiTableClipWidgetClass,
			    &XmTableClip_offsets,
			    &XmTableClipC_offsets);
}

/*
 * Function:
 *     ClassPartInitialize(widget_class)
 * Description:
 *	This function initializes the XmTableWidgetClass by parsing the
 *	class translations tables and setting of the call procedures.
 * Input:
 *	widget_class : WidgetClass - the widget class to initialize
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static void
ClassPartInitialize(WidgetClass widget_class)
#else
static void
ClassPartInitialize(widget_class)
    WidgetClass widget_class;
#endif
{
    XmTableWidgetClass twc = (XmTableWidgetClass) widget_class;

    _XmFastSubclassInit (widget_class, XmTABLE_BIT );

    /*
     * Since the translations were entered as strings we need to
     * convert them to XtTranslationTables.
     */
    if( twc->table_class.traversal_translations != NULL )
    {
	if( (String)twc->table_class.traversal_translations ==
	    XtInheritTranslations )
	{
	    twc->table_class.traversal_translations = (String)
		((XmTableWidgetClass) twc->core_class.superclass)->
		    table_class.traversal_translations;
	}
	else
	{
	    twc->table_class.traversal_translations = (String)
		XtParseTranslationTable((String) twc->table_class.
					traversal_translations);
	}
    }

    if( twc->table_class.edit_translations != NULL )
    {
	if( (String) twc->table_class.edit_translations ==
	    XtInheritTranslations )
	{
	    twc->table_class.edit_translations = (String)
		((XmTableWidgetClass) twc->core_class.superclass)->
		    table_class.edit_translations;
	}
	else
	{
	    twc->table_class.edit_translations = (String)
		XtParseTranslationTable((String) twc->table_class.
					edit_translations);
	}
    }

    if( widget_class != xmTableWidgetClass &&
        twc->table_class.num_default_procs > 0 )
    {
	if( twc->table_class.default_procs[0].widget_class_name != NULL &&
	    strcmp(twc->table_class.default_procs[0].widget_class_name,
		   "__APPEND__") == 0 )
	{
	    XmTableDefaultProcs procs, list1, list2;
	    int                 cnt1, cnt2;

	    list1 = ((XmTableWidgetClass) twc->core_class.superclass)->
		table_class.default_procs;
	    cnt1 = ((XmTableWidgetClass) twc->core_class.superclass)->
		table_class.num_default_procs;
	    list2 = twc->table_class.default_procs;
	    cnt2 = twc->table_class.num_default_procs;

	    procs = (XmTableDefaultProcs)
		XtMalloc(sizeof(XmTableDefaultProcRec) * (cnt1 + cnt2));

	    (void) memcpy((XtPointer) procs, (XtPointer) list1,
			  cnt1 * sizeof(XmTableDefaultProcRec));
	    (void) memcpy((XtPointer) (procs+cnt1), (XtPointer) list2,
			  cnt2 * sizeof(XmTableDefaultProcRec));

	    twc->table_class.num_default_procs = cnt1 + cnt2;
	    twc->table_class.default_procs = procs;
	}
    }
}

/*
 * Function:
 *	Initialize(request, widget, arg_list, arg_cnt)
 * Description:
 *	This is the initialize procedure for the XmTableWidget.  This
 *	function simply initializes a Table instance.
 * Input:
 *	request  : Widget   - holds the user requests
 *	widget   : Widget   - holds the new values
 *	arg_list : ArgList  - argument list to modify by
 *	arg_cnt  : Cardinal - number of arguments
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
Initialize(Widget request, Widget widget, ArgList arg_list, Cardinal *arg_cnt)
#else
static void
Initialize(request, widget, arg_list, arg_cnt)
    Widget   request, widget;
    ArgList  arg_list;
    Cardinal *arg_cnt;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;

    /*
     * Lets start by initializing all the members of this new instance
     * that are not initialized by resources.
     */
    XmTable_inited(tw) = False;

    XmTable_y_origin(tw) = 0;
    XmTable_x_origin(tw) = 0;
    XmTable_offset(tw) = NULL;
    XmTable_num_offsets(tw) = 0;

    XmTable_cell_x(tw) = -1;
    XmTable_cell_y(tw) = -1;
    XmTable_active_cell(tw) = NULL;

    XmTable_move_status(tw) = XmNO_MOVE;
    XmTable_move_widget(tw) = NULL;
    XmTable_move_start_x(tw) = 0;
    XmTable_move_cur_x(tw) = 0;
    XmTable_half_line_width(tw) = XmTable_line_width(tw)/2;
#ifdef DYNAMIC_COLUMN_SIZING
    XmTable_move_cursor(tw) = XCreateFontCursor(XtDisplay(widget),
					       XC_sb_h_double_arrow);
#else
    XmTable_move_cursor(tw) = None;
#endif /* DYNAMIC_COLUMN_SIZING */

    XmTable_save_size(tw).x = 0;
    XmTable_save_size(tw).y = 0;
    XmTable_save_size(tw).width = 0;
    XmTable_save_size(tw).height = 0;
    XmTable_save_shadow_thickness(tw) = 0;

    XmTable_title_height(tw) = 1;
    XmTable_table_height(tw) = 0;
    XmTable_table_width(tw) = 0;
    XmTable_row_height(tw) = 0;
    XmTable_valid_cnt(tw) = 0;

    XmTable_clip(tw) = NULL;
    XmTable_title_clip(tw) = NULL;
    XmTable_title_backing(tw) = NULL;
    XmTable_vscroll(tw) = NULL;
    XmTable_hscroll(tw) = NULL;

    XmTable_foreground_GC(tw) = NULL;
    XmTable_table_GC(tw) = NULL;
    XmTable_move_GC(tw) = NULL;
    XmTable_title_top_shadow_GC(tw) = NULL;
    XmTable_title_bottom_shadow_GC(tw) = NULL;
    XmTable_line_GC(tw) = NULL;
    XmTable_shade_GC(tw) = NULL;

    /*
     * Now that we are all setup lets create our children that we will be
     * using to build the table widget.  We will also attach the callbacks
     * to the widgets so that we can get notified about certain things.
     */
    XmTable_clip(tw) =
	XtVaCreateManagedWidget("Clip", xiTableClipWidgetClass, widget,
				XmNbackground, XmTable_table_color(tw),
				XmNbackgroundPixmap, XmTable_table_pixmap(tw),
				XmNmarginWidth, 0,
				XmNmarginHeight, 0,
				XmNshadowThickness, 0,
				XmNborderWidth, 0,
				NULL);
    XtAddCallback(XmTable_clip(tw), XmNexposeCallback,
		  TableExposeCB, (XtPointer) tw);
    XtAddCallback(XmTable_clip(tw), XmNresizeCallback,
		  TableResizeCB, (XtPointer) tw);
    XtAddCallback(XmTable_clip(tw), XmNinputCallback,
		  TableInputCB, (XtPointer) tw);

    XmTable_title_clip(tw) =
	XtVaCreateWidget("TitleClip", xmDrawingAreaWidgetClass, widget,
			 XmNbackground, XmTable_title_shade_color(tw),
			 XmNbackgroundPixmap, XmTable_title_shade_pixmap(tw),
			 XmNmarginWidth, 0,
			 XmNmarginHeight, 0,
			 XmNshadowThickness, 0,
			 XmNborderWidth, 0,
			 XmNtraversalOn, False,
			 NULL);
    XtVaSetValues(XmTable_title_clip(tw),
		  XmNtranslations, NULL,
		  NULL);

    XmTable_title_backing(tw) =
	XtVaCreateManagedWidget("TitleBacking", xmDrawingAreaWidgetClass,
				XmTable_title_clip(tw),
				XmNbackground, XmTable_title_background(tw),
				XmNbackgroundPixmap,
				XmTable_title_background_pixmap(tw),
				XmNmarginWidth, 0,
				XmNmarginHeight, 0,
				XmNborderWidth, 0,
				XmNtraversalOn, False,
				NULL);
    XtVaSetValues(XmTable_title_backing(tw),
		  XmNtranslations, NULL,
		  NULL);
#ifdef DYNAMIC_COLUMN_SIZING
    XtAddEventHandler(XmTable_title_backing(tw), PointerMotionMask, False,
		      TitleMotionEH, (XtPointer) tw);
    XtAddEventHandler(XmTable_title_backing(tw),
		      ButtonPressMask|ButtonReleaseMask, False,
		      TitleButtonPressEH, (XtPointer) tw);
#endif /* DYNAMIC_COLUMN_SIZING */

    XmTable_vscroll(tw) =
	XtVaCreateWidget("VScroll", xmScrollBarWidgetClass, widget,
			 XmNbackground, tw->core.background_pixel,
			 XmNorientation, XmVERTICAL,
			 XmNincrement, 1,
			 XmNpageIncrement, 1,
			 NULL);
    XtAddCallback(XmTable_vscroll(tw), XmNincrementCallback,
		  TableVScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_vscroll(tw), XmNpageIncrementCallback,
		  TableVScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_vscroll(tw), XmNdecrementCallback,
		  TableVScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_vscroll(tw), XmNpageDecrementCallback,
		  TableVScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_vscroll(tw), XmNdragCallback,
		  TableVScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_vscroll(tw), XmNtoTopCallback,
		  TableVScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_vscroll(tw), XmNtoBottomCallback,
		  TableVScrollCB, (XtPointer) NULL);

    XmTable_hscroll(tw) =
	XtVaCreateWidget("HScroll", xmScrollBarWidgetClass, widget,
			 XmNbackground, tw->core.background_pixel,
			 XmNorientation, XmHORIZONTAL,
			 XmNincrement, 1,
			 XmNpageIncrement, 1,
			 NULL);
    XtAddCallback(XmTable_hscroll(tw), XmNincrementCallback,
		  TableHScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_hscroll(tw), XmNpageIncrementCallback,
		  TableHScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_hscroll(tw), XmNdecrementCallback,
		  TableHScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_hscroll(tw), XmNpageDecrementCallback,
		  TableHScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_hscroll(tw), XmNdragCallback,
		  TableHScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_hscroll(tw), XmNtoTopCallback,
		  TableHScrollCB, (XtPointer) NULL);
    XtAddCallback(XmTable_hscroll(tw), XmNtoBottomCallback,
		  TableHScrollCB, (XtPointer) NULL);

    /*
     * Now that we are ready, lets create the GCs that we will use to
     * draw the various things we need to draw.
     */
    SetUpGCs(tw);

    if( XmTable_font_list(tw) == NULL )
    {
	XmTable_font_list(tw) =
	    XmFontListCopy(XmeGetDefaultRenderTable(widget, XmLABEL_FONTLIST));
    }
    else
    {
	XmTable_font_list(tw) =
	    XmFontListCopy(XmTable_font_list(tw));
    }

    /*
     * Finally lets pick an initial size if we were not given one.  Since
     * at this point we have no children our initialize size it a matter
     * of personal preference so don't look for any logic as to how the
     * initial width and height were gotten.
     */
    if( request->core.width == 0 )
    {
	widget->core.width = INITIAL_DIMENSION;
    }
    if( request->core.height == 0 )
    {
	widget->core.height = INITIAL_DIMENSION;
    }

    /*
     * Now that we are done initializing this instance, lets mark it as
     * such. Then we will send if off to do a resize so that all our
     * kids will be in the correct places.
     */
    XmTable_scroll_mgr(tw) = SmScrollMgrCreate();

    XmTable_inited(tw) = True;

    Resize(widget);
}

/*
 * Function:
 *	Destroy(widget)
 * Description:
 *	This function is called when an instance is destroyed.  This routine
 *	needs to clean up any memory that an instance has alloced.
 * Input:
 *	widget : Widget - the instance being destroyed.
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static void
Destroy(Widget widget)
#else
static void
Destroy(widget)
    Widget widget;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;

    /*
     * Lets start by releasing the GCs that we use throughout this
     * widget.
     */
    XiReleaseGC(widget, XmTable_foreground_GC(tw));
    XiReleaseGC(widget, XmTable_move_GC(tw));
    XiReleaseGC(widget, XmTable_title_top_shadow_GC(tw));
    XiReleaseGC(widget, XmTable_title_bottom_shadow_GC(tw));
    XiReleaseGC(widget, XmTable_table_GC(tw));
    XiReleaseGC(widget, XmTable_line_GC(tw));
    XiReleaseGC(widget, XmTable_shade_GC(tw));

    XtFree((XtPointer) XmTable_offset(tw));

    if( XmTable_font_list(tw) != NULL )
    {
	XmFontListFree(XmTable_font_list(tw));
    }

    SmScrollMgrDestroy(XmTable_scroll_mgr(tw));
}

/*
 * Function:
 *	Resize(widget)
 * Description:
 *	This function is called when the Table changes size.  This
 *	function just needs to change the shadow around the cliping
 *	area and relayout the widgets.
 * Input:
 *	widget : Widget - the widget that is changing size.
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static void
Resize(Widget widget)
#else
static void
Resize(widget)
    Widget widget;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;

    Layout(tw);
}

/*
 * Function:
 *	Redisplay(widget, event, region)
 * Description:
 *	This function is called when the Table gets and expose event.
 *	This function needs to redraw any visual that the table
 *	supports.
 * Input:
 *	widget  : Widget - the widget getting the expose event
 *	XEvent* : event  - the expose event
 *	Region  : region - the Region that is exposed
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
Redisplay(Widget widget, XEvent *event, Region region)
#else
static void
Redisplay(widget, event, region)
    Widget widget;
    XEvent *event;
    Region region;
#endif
{
    XmTableWidget  tw = (XmTableWidget) widget;
    Position       shadow = tw->manager.shadow_thickness;
    Widget         clip = XmTable_clip(tw);

    /*
     * Since we are about to draw a shadow, and when we resize we need
     * to erase this shadow, so lets save the size and thickness of the
     * shadow, so that we know what to erase.
     */
    XmTable_save_size(tw).x = XtX(clip) - shadow;
    XmTable_save_size(tw).y = XtY(clip) - shadow;
    XmTable_save_size(tw).width = XtWidth(clip) + (2 * shadow);
    XmTable_save_size(tw).height = XtHeight(clip) + (2 *  shadow);
    XmTable_save_shadow_thickness(tw) = shadow;

    /*
     * Now lets draw the actual shadow around our clip window.
     */
    XmeDrawShadows(XtDisplay(widget), XtWindow(widget),
		   tw->manager.top_shadow_GC,
		   tw->manager.bottom_shadow_GC,
		   XmTable_save_size(tw).x, XmTable_save_size(tw).y,
		   XmTable_save_size(tw).width, XmTable_save_size(tw).height,
		   shadow, XmSHADOW_IN);
}

/*
 * Function:
 *	SetValues(request, current, widget, arg_list, arg_cnt)
 * Description:
 *	This function is called when a resource is changed for the
 *	table.
 * Input:
 *	request  : Widget   - the users request
 *	current  : Widget   - the current value for the widget
 *	widget   : Widget   - the new values for the widget
 *	arg_list : ArgList  - the arguments that were set
 *	arg_cnt  : Cardinal - the number of arguments
 * Output:
 *	Boolean - should the widget be redisplayed?
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static Boolean
SetValues(Widget current, Widget request, Widget widget, ArgList arg_list,
	  Cardinal *arg_cnt)
#else
static Boolean
SetValues(current, request, widget, arg_list, arg_cnt)
    Widget   current, request, widget;
    ArgList  arg_list;
    Cardinal *arg_cnt;
#endif
{
    XmTableWidget  cur = (XmTableWidget) current,
                   tw = (XmTableWidget) widget;
    Boolean        need_resize = False,
                   need_layout = False,
                   need_clip_expose = False,
                   need_expose = False,
                   need_recalc = False;
    Arg            label_list[20], child_list[20];
    Cardinal       label_cnt = 0, child_cnt = 0;
    static Boolean in = False;

    if( in ) return( False );
    in = True;

    if( cur->manager.shadow_thickness != tw->manager.shadow_thickness )
    {
	need_layout = True;
	need_resize = True;
	XtSetArg(label_list[label_cnt], XmNshadowThickness,
		 tw->manager.shadow_thickness); ++label_cnt;
    }

    if( XmTable_margin_height(cur) != XmTable_margin_height(tw) ||
        XmTable_margin_width(cur) != XmTable_margin_width(tw) ||
        XmTable_spacing(cur) != XmTable_spacing(tw) ||
        XmTable_line_width(cur) != XmTable_line_width(tw) )
    {
	need_layout = True;
	need_resize = True;
    }

    if( XmTable_show_titles(cur) != XmTable_show_titles(tw) )
    {
	if( XmTable_show_titles(tw) )
	{
	    XtManageChild(XmTable_title_clip(tw));
	}
	else
	{
	    XtUnmanageChild(XmTable_title_clip(tw));
	}
	need_resize = True;
    }

    if( XmTable_num_rows(cur) != XmTable_num_rows(tw) )
    {
	need_clip_expose = True;
	need_layout = True;
	need_recalc = True;

	if( XiValidChild(XmTable_active_cell(tw)) )
	{
	    if( XmTable_num_rows(tw) == 0 )
	    {
		XiSetActive(tw, NULL, -1, XiVisNone, True);
		XmTable_cell_y(tw) = -1;
		XmTable_active_cell(tw) = NULL;

		if( (XmTable_x_origin(tw) != 0) || 
                    ((XmTable_y_origin(tw) != 0) &&
		    (XmTable_title_backing(tw) != NULL) &&
		    (XmTable_hscroll(tw) != NULL) && 
                    (XmTable_vscroll(tw) != NULL)))
		{
		    int tmp, slider, inc, page_inc;

		    XmTable_x_origin(tw) = 0;
		    XmTable_y_origin(tw) = 0;

		    XtMoveWidget(XmTable_title_backing(tw), 0, 0);

		    XmScrollBarGetValues(XmTable_hscroll(tw), &tmp, &slider,
					 &inc, &page_inc);
		    XmScrollBarSetValues(XmTable_hscroll(tw), 0, slider,
					 inc, page_inc, False);
		    XmScrollBarGetValues(XmTable_vscroll(tw), &tmp, &slider,
					 &inc, &page_inc);
		    XmScrollBarSetValues(XmTable_vscroll(tw), 0, slider,
					 inc, page_inc, False);
		}
	    }
	    else if( XmTable_cell_y(tw) >= XmTable_num_rows(tw) )
	    {
		XiSetActive(tw, XmTable_active_cell(tw), XmTable_num_rows(tw) - 1,
			    XiVisNone, True);
	    }
	}
	else if( XmTable_num_rows(tw) > 0 )
	{
	    WidgetList kid;
	    Cardinal   kidCnt, i;

	    XtVaGetValues(XmTable_clip(tw),
			  XmNchildren, &kid,
			  XmNnumChildren, &kidCnt,
			  NULL);

	    for( i = 0; i < kidCnt; ++i, ++kid )
	    {
		if( !XiValidChild(*kid) ) continue;

		XiSetActive(tw, *kid, 0, XiVisUpperLeft, True);
		break;
	    }
	}
    }

    if( XmTable_set_child_color(cur) != XmTable_set_child_color(tw) )
    {
    }

    if( XmTable_title_alignment(cur) != XmTable_title_alignment(tw) )
    {
	XtSetArg(label_list[label_cnt], XmNalignment,
		 XmTable_title_alignment(tw)); label_cnt++;
    }

    if( XmTable_font_list(cur) != XmTable_font_list(tw) )
    {
	need_layout = True;
	need_resize = True;

	if( XmTable_font_list(cur) != NULL )
	{
	    XmFontListFree(XmTable_font_list(cur));
	}
	if( XmTable_font_list(tw) == NULL )
	{
	    XmTable_font_list(tw) =
		XmFontListCopy(XmeGetDefaultRenderTable(widget,
						     XmLABEL_FONTLIST));
	}
	else
	{
	    XmTable_font_list(tw) =
		XmFontListCopy(XmTable_font_list(tw));
	}
	XtSetArg(label_list[label_cnt], XmNfontList,
		 XmTable_font_list(tw)); label_cnt++;
    }

    if( cur->core.background_pixel != tw->core.background_pixel ||
        cur->core.background_pixmap != tw->core.background_pixmap )
    {
	XtVaSetValues(XmTable_vscroll(tw),
		      XmNbackground, tw->core.background_pixel,
		      XmNbackgroundPixmap, tw->core.background_pixmap,
		      NULL);
	XtVaSetValues(XmTable_hscroll(tw),
		      XmNbackground, tw->core.background_pixel,
		      XmNbackgroundPixmap, tw->core.background_pixmap,
		      NULL);
    }

    if( cur->manager.top_shadow_color != tw->manager.top_shadow_color )
    {
	XtVaSetValues(XmTable_vscroll(tw),
		      XmNtopShadowColor, tw->manager.top_shadow_color,
		      XmNtopShadowPixmap, tw->manager.top_shadow_pixmap,
		      NULL);
	XtVaSetValues(XmTable_hscroll(tw),
		      XmNtopShadowColor, tw->manager.top_shadow_color,
		      XmNtopShadowPixmap, tw->manager.top_shadow_pixmap,
		      NULL);
	XtVaSetValues((Widget) tw,
		      XmNtopShadowColor, tw->manager.top_shadow_color,
		      XmNtopShadowPixmap, tw->manager.top_shadow_pixmap,
		      NULL);
	need_expose = True;
    }

    if( cur->manager.bottom_shadow_color != tw->manager.bottom_shadow_color )
    {
	XtVaSetValues(XmTable_vscroll(tw),
		      XmNbottomShadowColor, tw->manager.bottom_shadow_color,
		      XmNbottomShadowPixmap,tw->manager.bottom_shadow_pixmap,
		      NULL);
	XtVaSetValues(XmTable_hscroll(tw),
		      XmNbottomShadowColor, tw->manager.bottom_shadow_color,
		      XmNbottomShadowPixmap,tw->manager.bottom_shadow_pixmap,
		      NULL);
	XtVaSetValues((Widget) tw,
		      XmNbottomShadowColor,tw->manager.bottom_shadow_color,
		      XmNbottomShadowPixmap,tw->manager.bottom_shadow_pixmap,
		      NULL);
	need_expose = True;
    }

    if( cur->manager.foreground != tw->manager.foreground &&
        XmTable_set_child_color(tw) )
    {
	XtSetArg(child_list[child_cnt], XmNforeground,
		 tw->manager.foreground); child_cnt++;
    }

    if( XmTable_table_color(cur) != XmTable_table_color(tw) ||
        XmTable_table_pixmap(cur) != XmTable_table_pixmap(tw) )
    {
	XtVaSetValues(XmTable_clip(tw),
		      XmNbackground, XmTable_table_color(tw),
		      XmNbackgroundPixmap, XmTable_table_pixmap(tw),
		      NULL);
	if( XmTable_set_child_color(tw) )
	{
	    XtSetArg(child_list[child_cnt], XmNbackground,
		     XmTable_table_color(tw)); child_cnt++;
	    XtSetArg(child_list[child_cnt], XmNbackgroundPixmap,
		     XmTable_table_pixmap(tw)); child_cnt++;
	}
    }

    if( XmTable_title_background(cur) != XmTable_title_background(tw) ||
        XmTable_title_background_pixmap(cur) !=
        XmTable_title_background_pixmap(tw) )
    {
	XtSetArg(label_list[label_cnt], XmNbackground,
		 XmTable_title_background(tw)); label_cnt++;
	XtSetArg(label_list[label_cnt], XmNbackgroundPixmap,
		 XmTable_title_background_pixmap(tw)); label_cnt++;
	XtVaSetValues(XmTable_title_backing(tw),
		      XmNbackground, XmTable_title_background(tw),
		      XmNbackgroundPixmap,XmTable_title_background_pixmap(tw),
		      NULL);
    }

    if( XmTable_title_foreground(cur) != XmTable_title_foreground(tw) )
    {
	XtSetArg(label_list[label_cnt], XmNforeground,
		 XmTable_title_foreground(tw)); label_cnt++;
    }

    if( (XmTable_title_top_shadow_color(cur) !=
	 XmTable_title_top_shadow_color(tw)) ||
        (XmTable_title_top_shadow_pixmap(cur) !=
	 XmTable_title_top_shadow_pixmap(tw)) )
    {
	XiReleaseGC(tw, XmTable_title_top_shadow_GC(tw));
    }

    if( (XmTable_title_bottom_shadow_color(cur) !=
	 XmTable_title_bottom_shadow_color(tw)) ||
        (XmTable_title_bottom_shadow_pixmap(cur) !=
	 XmTable_title_bottom_shadow_pixmap(tw)) )
    {
	XiReleaseGC(tw, XmTable_title_bottom_shadow_GC(tw));
    }

    if( XmTable_title_shade_color(cur) != XmTable_title_shade_color(tw) ||
        XmTable_title_shade_pixmap(cur) != XmTable_title_shade_pixmap(tw) )
    {
	XtVaSetValues(XmTable_title_clip(tw),
		      XmNbackground, XmTable_title_shade_color(tw),
		      XmNbackgroundPixmap, XmTable_title_shade_pixmap(tw),
		      NULL);
    }

    if( XmTable_shade_color(cur) != XmTable_shade_color(tw) ||
        XmTable_shade_pixmap(cur) != XmTable_shade_pixmap(tw) )
    {
	XiReleaseGC(tw, XmTable_shade_GC(tw));
	need_clip_expose = True;
    }

    if( XmTable_line_color(cur) != XmTable_line_color(tw) )
    {
	XiReleaseGC(tw, XmTable_line_GC(tw));
	need_clip_expose = True;
    }

    PassDownResources(tw, child_list, child_cnt, label_list, label_cnt);

    SetUpGCs(tw);

    /*
     * Now that all the work has been done lets check if we need to
     * get a new size for us, and if so lets be sure to set it.
     */
    if( need_recalc || need_resize )
    {
	CalcTableInfo(tw);
    }

    if( need_recalc || need_resize || need_layout ) {
	LayoutCells(tw);
	Layout(tw);
    }

    if( need_resize )
    {
	XtWidgetGeometry request, wanted;

	request.request_mode = 0;
	QueryGeometry((Widget) tw, &request, &wanted);

	if( XiResizeWidth(tw) )	tw->core.width = wanted.width;
	if( XiResizeHeight(tw) ) tw->core.height = wanted.height;
    }

    if( need_clip_expose && XtIsRealized(XmTable_clip(tw)) )
    {
	XClearArea(XtDisplay(tw), XtWindow(XmTable_clip(tw)),
		   0, 0, 0, 0, True);
    }

    in = False;

    return( need_expose );
}

/*
 * Function:
 * 	QueryGeometry(widget, request, wanted)
 * Description:
 *	This function is called when someone is interested in what size
 *	the Table would like, or if it would accept a specific size.
 * Input:
 *	widget  : Widget            - the widget getting the request
 *	request : XtWidgetGeometry* - the requested geometry
 *	wanted  : XtWidgetGeometry* - the returned geometry changes
 * Output:
 *	XtGeometryResult - the result of the geometry query
 */
static XtGeometryResult
#if NeedFunctionPrototypes
QueryGeometry(Widget widget, XtWidgetGeometry *request,
	      XtWidgetGeometry *wanted)
#else
QueryGeometry(widget, request, wanted)
    Widget           widget;
    XtWidgetGeometry *request, *wanted;
#endif
{
    XmTableWidget   tw = (XmTableWidget) widget;
    Dimension        width, height;
    XtGeometryResult result = XtGeometryYes;
    
    /*
     * First lets do the easy check, if the caller did not specify a
     * request_mode then they are just curious about which size we
     * want to be so lets calculate the size we WANT to be and pass
     * that back.
     */
    if( request->request_mode == 0 )
    {
	wanted->request_mode = CWWidth | CWHeight;
	wanted->width = XmTable_table_width(tw) + 2 * (XmTable_margin_width(tw) +
						tw->manager.shadow_thickness);
	wanted->height = XmTable_table_height(tw) +
	    ((XmTable_show_titles(tw) ?
	      (XtHeight(XmTable_title_clip(tw)) + XmTable_spacing(tw)) : 0) +
	     (2 * (XmTable_margin_height(tw) + tw->manager.shadow_thickness)));

	return( XtGeometryAlmost );
    }

    /*
     * Now the hard case.  It appears that the caller wants to know what
     * we think about a given geometry.  Well first of all if the
     * caller does not care about our width or height then we could care
     * less, because we will take any width, height, and border width
     * handed to us.
     */
    if( !(request->request_mode & (CWWidth | CWHeight)) )
    {
	/*
	 * Well if appears that the caller is interested in something
	 * else other than our dimensions, so lets accept what he gave
	 * us.
	 */
	return( XtGeometryYes );
    }

    /*
     * Now the really hard case here the caller want to know what we
     * think about a certain width and or height.  We really can accept
     * any width and height, but what we will do is calculate a minimum
     * and if the caller goes below that we will ask for a bit more.
     */
    memcpy((XtPointer) wanted, (XtPointer) request, sizeof(XtWidgetGeometry));
    if( request->request_mode & CWWidth )
    {
	/*
	 * Lets first calculate our minimum width.  This minimum is more
	 * of a rough estimate then an absoulte minimum but here goes.
	 */
	width = XmTable_spacing(tw) + XtWidth(XmTable_vscroll(tw)) +
	    (XiValidChild(XmTable_active_cell(tw)) ? XtWidth(XmTable_active_cell(tw)) : 1) +
	    2 * (XmTable_margin_width(tw) +
		 tw->manager.shadow_thickness +
		 XmTable_line_width(tw));

	if( request->width < width )
	{
	    wanted->width = width;
	    result = XtGeometryAlmost;
	}
    }

    if( request->request_mode & CWHeight )
    {
	/*
	 * Lets first calculate our minimum width.  This minimum is more
	 * of a ruff estimate then an absoulte minimum but here goes.
	 */
	height = (XmTable_spacing(tw) + XtHeight(XmTable_hscroll(tw)) +
		  XmTable_row_height(tw) +
		  (XmTable_show_titles(tw)
		   ? (XtHeight(XmTable_title_clip(tw)) + XmTable_spacing(tw)) : 0) +
		  2 * (XmTable_margin_height(tw) +
		       tw->manager.shadow_thickness +
		       XmTable_line_width(tw)));
	if( request->height < height )
	{
	    wanted->height = height;
	    result = XtGeometryAlmost;
	}
    }

    return( result );
}

/*
 * Function:
 *	GeometryManager(widget, request, allowed)
 * Description:
 *	This fucntion is called when one of our children is asking to
 *	change its geometry.  This function serves both the XiClipWidget
 *	and the XmTableWidget and is meany to do the right thing when
 *	a child requests a new geometry.
 * Input:
 *	widget  : Widget            - the child requesting the change
 *	request : XtWidgetGeometry* - the requested change
 *	allowed : XtWidgetGeometry* - the allowed geometry
 * Output:
 *	XtGeometryResult - the geometry result code
 */
#if NeedFunctionPrototypes
static XtGeometryResult
GeometryManager(Widget widget, XtWidgetGeometry *request,
		XtWidgetGeometry *allowed)
#else
static XtGeometryResult
GeometryManager(widget, request, allowed)
    Widget           widget;
    XtWidgetGeometry *request, *allowed;
#endif
{
    XmTableWidget    tw;
    Widget           clip;
    Dimension	     width, height;
    XtGeometryResult result = XtGeometryYes;
    
    if( XtIsSubclass(XtParent(widget), xmTableWidgetClass) )
    {
	/*
	 * If this is one of the children of the table widget requesting
	 * a new size then it is doing something that it should not do.
	 * all the geometry changes will be handled through the clip.
	 * so if one of these kids is asking for a change tell it to
	 * go away.
	 */
	return( XtGeometryNo );
    }

    clip = XtParent(widget);
    tw = (XmTableWidget) XtParent(clip);

    /*
     * Lets start by copying the request structure into our allowed
     * structure.
     */
    memcpy((XtPointer) allowed, (XtPointer) request, sizeof(XtWidgetGeometry));
    allowed->request_mode = 0;

    /*
     * Now lets see what they want if.  First we will do the quick check
     * to see if they specified something that we won't let them change
     * (i.e. x, y, borderWidth).  If they did then we will set the result
     * to XtGeometryAlmost.
     */
    if( request->request_mode & (CWX | CWY | CWBorderWidth) )
    {
	if( request->request_mode & CWBorderWidth )
	{
	    allowed->request_mode |= CWBorderWidth;
	    allowed->border_width = 0;
	}
	result = XtGeometryAlmost;
    }

    /*
     * Next lets check to see if they specified the width or height or
     * not, and if not we want to return right here.
     */
    if( !(request->request_mode & (CWWidth | CWHeight)) )
    {
	if( allowed->request_mode != 0 )
	{
	    return( result );
	}
	return( XtGeometryNo );
    }

    /*
     * Now lets check for the things that we do care about.  These are
     * the dimensions of the child.  The rule is that we will basically
     * take any dimension that they give us.  The only thing we need to
     * check is if this is a query or not.
     */
    width = XtWidth(widget);
    height = XtHeight(widget);
    if( request->request_mode & CWWidth )
    {
	allowed->request_mode |= CWWidth;
	if( request->width < MIN_CELL_SIZE )
	{
	    allowed->width = width = MIN_CELL_SIZE;
	    result = XtGeometryAlmost;
	}
	else
	{
	    width = request->width;
	}
    }
    if( request->request_mode & CWHeight )
    {
	XtWidgetGeometry wanted;
	WidgetList       kid;
	Cardinal         kidCnt, i;

	XtVaGetValues(XtParent(widget),
		      XmNchildren, &kid,
		      XmNnumChildren, &kidCnt,
		      NULL);

	allowed->request_mode |= CWHeight;

	height = 1;
	for( i = 0; i < kidCnt; ++i )
	{
	    if( !XiValidChild(*kid) ) continue;

	    if( *kid == widget )
	    {
		wanted.height = request->height;
	    }
	    else
	    {
		if( XtQueryGeometry(*kid, NULL, &wanted) == XtGeometryNo ||
		    (wanted.request_mode & CWHeight) )
		{
		    wanted.height = XtHeight(*kid);
		}
		if( (int) wanted.height > (int) height )
		{
		    height = wanted.height;
		}
	    }
	}

	if( request->height != height )
	{
	    allowed->height = height;
	    result = XtGeometryAlmost;
	}
	else
	{
	    height = request->height;
	}
    }

    if( result == XtGeometryYes && !(request->request_mode & XtCWQueryOnly) )
    {
	if( width != XtWidth(widget) || height != XtHeight(widget) )
	{
	    XtResizeWidget(widget, width, height, 0);

	    CalcTableInfo(tw);
	    Layout(tw);
	    LayoutCells(tw);
	    LayoutActive(tw);

	    XClearArea(XtDisplay(clip), XtWindow(clip), 0, 0, 0, 0, True);
	    result = XtGeometryDone;
	}
    }

    return( result );
}

/*
 * Function:
 *	ClipChangeManaged(widget)
 * Description:
 *	This function is called when a child of the XiClip changes managed
 *	state.  This function needs to tell the table and the clip how
 *	to deal with the change.
 * Input:
 *	widget : Widget - the clip widget
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static void
ClipChangeManaged(Widget widget)
#else
static void
ClipChangeManaged(widget)
    Widget widget;
#endif
{
    XmTableWidget    tw = (XmTableWidget) XtParent(widget);
    CompositeWidget  cw = (CompositeWidget) widget;
    WidgetList       kid = cw->composite.children;
    Cardinal         kidCnt = cw->composite.num_children, i;
    Widget           focus, label;
    XtWidgetGeometry request, allowed;
    Boolean          title_change = False;
    Dimension	     max_height;

   
    /*
     * Lets start by calculating the dimensions of the table iteself
     * and laying out the titles.
     */
    CalcTableInfo(tw);
    LayoutCells(tw);

    /*
     * Now lets walk through the kids and make sure that they are all
     * initialized
     */
    for( i = 0; i < kidCnt; ++i )
    {
	XtTranslations trans;

	if( (label = XiC(kid[i]).label) != NULL )
	{
	    if( XtIsManaged(kid[i]) && !XtIsManaged(label) )
	    {
		XtManageChild(label);
		title_change = True;
	    }
	    else if( !XtIsManaged(kid[i]) && XtIsManaged(label) )
	    {
		XtUnmanageChild(label);
		title_change = True;
	    }
	}

	if( XmTable_active_cell(tw) != kid[i] )
	{
	    XtSetMappedWhenManaged(kid[i], False);
	}

	if( XmTable_set_child_color(tw) )
	{
	    XtVaSetValues(kid[i],
			  XmNbackground, XmTable_table_color(tw),
			  XmNbackgroundPixmap, XmTable_table_pixmap(tw),
			  XmNforeground, tw->manager.foreground,
			  NULL);
	}

	/*
	 * If this is not a valid child, or it has already had its
	 * translations inited skip it.
	 */
	if( !XiValidChild(kid[i]) || XiC(kid[i]).translations_inited )
	{
	    continue;
	}

	/*
	 * Find the widget that receives the keyboard focus for this child
	 * and then override its default translations with the edit
	 * translations.
	 */
	focus = XiCallFocusWidget(kid[i]);
	XtOverrideTranslations(focus,(XtTranslations)
			       ((XmTableWidgetClass) XtClass(tw))->
			       table_class.edit_translations);

	/*
	 * Now grab the modified translations from the widget and store
	 * them away in the constraint record for later use.
	 */
	trans = NULL;
	XtVaGetValues(focus,
		      XmNtranslations, &trans,
		      NULL);
	XiC(kid[i]).edit_translations = trans;

	/*
	 * Again override the widgets translations, this time with the
	 * traversal translations.
	 */
	XtOverrideTranslations(focus, (XtTranslations)
			       ((XmTableWidgetClass) XtClass(tw))->
			       table_class.traversal_translations);
	/*
	 * Grab the modified translations back from the widget and store
	 * them away as well.
	 */
	trans = NULL;
	XtVaGetValues(focus,
		      XmNtranslations, &trans,
		      NULL);
	XiC(kid[i]).traversal_translations = trans;

	/*
	 * Now mark this widget as inited so that we won't try this hack
	 * again.
	 */
	XiC(kid[i]).translations_inited = True;
    }

    /*
     * If the currently active cell is not valid then we need to choose
     * a new active cell.  We will choose this by finding the first
     * valid kid and making that the active cell.
     */
    if( !XiValidInputChild(XmTable_active_cell(tw)) )
    {
	int cell_y = 0;
	if( XmTable_active_cell(tw) != NULL )
	{
	    cell_y = XmTable_cell_y(tw);
	}
	XmTable_active_cell(tw) = NULL;
	for( i = 0; i < kidCnt; ++i, ++kid )
	{
	    if( !XiValidInputChild(*kid) ) continue;

	    SetActive(tw, *kid, cell_y, XiVisUpperLeft);

	    break;
	}
    }

    if( title_change )
    {
	XtWidgetGeometry wanted;

	XtVaGetValues(XmTable_title_backing(tw),
		      XmNchildren, &kid,
		      XmNnumChildren, &kidCnt,
		      NULL);
	max_height = 1;
	for( i = 0; i < kidCnt; ++i )
	{
	    if( !XiValidChild(kid[i]) ) continue;

	    if( XtQueryGeometry(kid[i], NULL, &wanted) == XtGeometryNo ||
	        !(wanted.request_mode & CWHeight) )
	    {
		wanted.height = XtHeight(kid[i]);
	    }
	    if( wanted.height > max_height ) max_height = wanted.height;
	}

	XtResizeWidget(XmTable_title_clip(tw),
		       XtWidth(XmTable_title_clip(tw)),
		       max_height, 0);
	XtResizeWidget(XmTable_title_backing(tw),
		       XtWidth(XmTable_title_backing(tw)),
		       max_height, 0);
    }

    /*
     * Finally now that everything is set up, lets calculate the size that
     * the table wants to bx and ask if we can be that size.
     */
    if( XmTable_valid_cnt(tw) == 0 )
    {
	/*
	 * If we do not have any valid children then we will ask to become
	 * our default size.
	 */
	request.request_mode = 0;

	if( XiResizeWidth(tw) )
	{
	    request.request_mode |= CWWidth;
	    request.width = INITIAL_DIMENSION;
	}
	if( XiResizeHeight(tw) )
	{
	    request.request_mode |= CWHeight;
	    request.height = INITIAL_DIMENSION;
	}
    }
    else
    {
	/*
	 * Ok we have valid children so lets use the calculated table
	 * size and add on the need bits to get the complete size of
	 * the table.
	 */
	request.request_mode = 0;

	if( XiResizeWidth(tw) )
	{
	    request.request_mode |= CWWidth;
	    request.width = (XmTable_table_width(tw) +
			     2 * (tw->manager.shadow_thickness +
				  XmTable_margin_width(tw)));
	}
	if( XiResizeHeight(tw) )
	{
	    request.request_mode |= CWHeight;
	    request.height = (XmTable_table_height(tw) +
			      (XmTable_show_titles(tw)
			       ? (XtHeight(XmTable_title_clip(tw)) +
				  XmTable_spacing(tw)) : 0) +
			      2 * (tw->manager.shadow_thickness +
				   XmTable_margin_height(tw)));
	}
    }

    /*
     * Now that we know what size we want to be, lets ask the parent
     * of the table if we can be that size.
     */
    switch( XtMakeGeometryRequest((Widget) tw, &request, &allowed) )
    {
    case XtGeometryAlmost:
	/*
	 * We almost got what we want so lets cooperate and accept what
	 * our parent told us and ask for that instead.
	 */
	XtMakeGeometryRequest((Widget) tw, &allowed, NULL);
	break;
    case XtGeometryNo:
    case XtGeometryYes:
    default:
	break;
    }

    /*
     * Finally since things have changed a bit lets relayout our titles
     * and the redraw our table if needed.
     */
    Layout(tw);
    LayoutTitles(tw);
}

/*
 * Function:
 * 	ChangeManaged(widget)
 * Description:
 *	This routine is called when a child of the Table changes managed
 *	state.
 * Input:
 *	widget : Widget - the table widget
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static void
ChangeManaged(Widget widget)
#else
static void
ChangeManaged(widget)
    Widget widget;
#endif
{
    static Boolean in_change_managed = False;
    XmTableWidget  tw = (XmTableWidget) widget;

   
    
    /*
     * If we are a non-inited table, of this is a re-entrent call then
     * we do not want to be here, so lets leave.
     */
    if( !XmTable_inited(tw) || in_change_managed ) return;

    /*
     * Lets mark the fact that we are in changed managed so that this
     * function is not re-entrent.
     */
    in_change_managed = True;

    /*
     * Since something changed managed state, lets relayout our widgets
     * and out titles.
     */
    Layout(tw);

    /*
     * Now that we are done with change managed, lets turn off our
     * stop sign.
     */
    in_change_managed = False;
}

/*
 * Function:
 *	InsertChild(widget)
 * Description:
 *	This function is called when a child is inserted into the table,
 *	this function moves the child to the clip area where it
 *	belongs.
 * Input:
 *	widget : Widget - the widget that is being inserted.
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static void
InsertChild(Widget widget)
#else
static void
InsertChild(widget)
    Widget widget;
#endif
{
    XmTableWidget        tw = (XmTableWidget) XtParent(widget);
    CompositeWidgetClass sc = (CompositeWidgetClass)
	xmTableWidgetClass->core_class.superclass;

  
    /*
     * If the table is not inited then we want to add the child to the
     * table, because that is one of our kids.  If the table is inited
     * then we want to reparent the new child to the clip area.
     */
    if( XmTable_inited(tw) )
    {
	/*
	 * Here we do two things, one we reparent the widget to the clip
	 * area, and two we make sure that this widget is not mapped when
	 * it is managed.
	 */
	widget->core.parent = XmTable_clip(tw);
	widget->core.mapped_when_managed = False;
    }

    /*
     * Now dynamically inherit the insert child routine from our
     * superclass.
     */
    (*sc->composite_class.insert_child)(widget);
}

/*
 * Function:
 *	FindDefaultProc(widget)
 * Description:
 *	This function attempts to find the default interface functions for
 *	the given child and if found returns a pointer to the default
 *	function structure.
 * Input:
 *	widget : Widget - the child we want the procs for
 * Output:
 *	XmTableDefaultProcs - the default procs struct or NULL
 */
#if NeedFunctionPrototypes
static XmTableDefaultProcs
FindDefaultProcs(Widget widget)
#else
static XmTableDefaultProcs
FindDefaultProcs(widget)
    Widget widget;
#endif
{
    XmTableWidgetClass  wclass;
    XmTableDefaultProcs procs;
    Cardinal             num_procs, i;

    wclass = (XmTableWidgetClass) XtClass(XtParent(widget));
    procs = wclass->table_class.default_procs;
    num_procs = wclass->table_class.num_default_procs;

    /*
     * Now lets walk through the list of default procs looking for a
     * class match.
     */
    for( i = 0; i < num_procs; ++i, ++procs )
    {
	if( procs->widget_class == XtClass(widget) )
	{
	    /*
	     * Ok we found a class match so lets return the
	     * default proc struct.
	     */
	    return( procs );
	}
	else if( procs->widget_class == NULL &&
		 strcmp(XtClass(widget)->core_class.class_name,
			procs->widget_class_name) == 0 )
	{
	    /*
	     * Here we found a class name match so we want to do a couple
	     * of things here.  First we want to cache the class pointer
	     * so that we can do faster lookups on this class next time,
	     * and two we want to return the default procs structure.
	     */
	    procs->widget_class = XtClass(widget);
	    return( procs );
	}
    }

    /*
     * If we got here then we did not find a match so we will return
     * NULL to let the world know that we did not find anything.
     */
    return( NULL );
}

/*
 * Function:
 *	AssignDefaultFunctions(widget)
 * Description:
 *	This function is called to default any of the interface
 * 	functions that the user did not set.
 * Input:
 *	widget : Widget - the widget to set the functions for.
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static void
AssignDefaultFunctions(Widget widget)
#else
static void
AssignDefaultFunctions(widget)
    Widget widget;
#endif
{
    XmTableConstraint   wc = &XiC(widget);
    XmTableDefaultProcs procs = FindDefaultProcs(widget);

    /*
     * First lets check if we found some default procs.  And if not lets
     * just return because we can't use nothing to set defaults.
     */
    if( procs == NULL ) return;

    /*
     * Now fill in any interface procs that were not set with the default.
     */
    if( wc->focus_widget_func == NULL )
    {
	wc->focus_widget_func = procs->focus_widget_func;
    }

    if( wc->select_text_proc == NULL )
    {
	wc->select_text_proc = procs->select_text_proc;
    }

    if( wc->unselect_text_proc == NULL )
    {
	wc->unselect_text_proc = procs->unselect_text_proc;
    }

    if( wc->set_value_proc == NULL )
    {
	wc->set_value_proc = procs->set_value_proc;
    }

    if( wc->get_value_func == NULL )
    {
	wc->get_value_func = procs->get_value_func;
    }

    if( wc->free_proc == NULL )
    {
	wc->free_proc = procs->free_proc;
    }

    if( wc->render_proc == NULL )
    {
	wc->render_proc = procs->render_proc;
    }
}

/*
 * Function:
 * 	ConstraintInitialize(request, widget, arg_list, arg_cnt)
 * Description:
 *	This function is called to initialize the constraints for the
 *	Table widget.
 * Input:
 *	request  : Widget   - the requested settings
 *	widget   : Widget   - the new widget
 *	arg_list : ArgList  - the arg list containing the settings
 *	arg_cnt  : Cardinal - the number of args in the list
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
ConstraintInitialize(Widget request, Widget widget, ArgList arg_list,
		     Cardinal *arg_cnt)
#else
static void
ConstraintInitialize(request, widget, arg_list, arg_cnt)
    Widget   request, widget;
    ArgList  arg_list;
    Cardinal *arg_cnt;
#endif
{
    XmTableWidget     tw = (XmTableWidget) XtParent(widget);
    XmTableConstraint wc = &XiC(widget);
   
    /*
     * First lets initialize all the constraint members that are not
     * initialized by resources.
     */
    wc->translations_inited = False;
    wc->edit_translations = NULL;
    wc->traversal_translations = NULL;

    wc->_column_title = NULL;
    wc->offset = 0;

    wc->label = NULL;

    /*
     * If the table is not initialized then this must be one of our kids
     * and not one of the children of the table.  If this is the case
     * we don't want to set this kid up, so lets skip it.
     */
    if( !XmTable_inited(tw) )
    {
	return;
    }

    /*
     * If we get here then we are dealing with a child that is going to
     * be reparented as a child of the clip area and as such is a
     * element in the table so lets grab the values that we care about
     * and stuff them in the correct places.
     */

    /*
     * Lets make a copy of the title that they passed to us, so that
     * we do not lose if they free the string they gave us.
     */
    if( wc->column_title != NULL )
    {
	wc->_column_title = XmStringCopy(wc->column_title);
    }

    /*
     * Assign the default interface functions to the widget if they
     * did not specify defaults.
     *
     * NOTE:
     *     by doing this we prevent the user from ever setting an
     *	   interface function to NULL, but that is the intention.
     */
    AssignDefaultFunctions(widget);

    /*
     * Now lets cheate a title label for this widget.  If may be the case
     * that we never need this widget, but it can't hurt to have it
     * around, just in case.  We also add event handlers to this label
     * so that we get notification when certain things happen so that
     * we can react to them.
     */
    wc->label =
	XtVaCreateWidget("Label", xmLabelWidgetClass,
			 XmTable_title_backing(tw),
			 XmNshadowThickness,
			 tw->manager.shadow_thickness,
			 XmNforeground, XmTable_title_foreground(tw),
			 XmNbackground, XmTable_title_background(tw),
			 XmNalignment, XmTable_title_alignment(tw),
			 XmNfontList, XmTable_font_list(tw),
			 XmNlabelType, XmSTRING,
			 XmNmarginBottom, 0,
			 XmNmarginTop, 0,
			 XmNmarginLeft, 0,
			 XmNmarginRight, 0,
			 XmNhighlightOnEnter, False,
			 XmNhighlightThickness, 0,
			 XmNtraversalOn, False,
			 XmNborderWidth, 0,
			 XmNwidth, XtWidth(widget), 
			 XmNlabelString, wc->_column_title,
			 XmNuserData, widget,
			 NULL);
   
    XtVaSetValues(wc->label,
		  XmNtranslations, NULL,
		  NULL);
    XtAddEventHandler(wc->label, ExposureMask, False, TitleLabelExposeEH,
		      (XtPointer) tw);
#ifdef DYNAMIC_COLUMN_SIZING
    XtAddEventHandler(wc->label, PointerMotionMask, False, TitleMotionEH,
		      (XtPointer) tw);
    XtAddEventHandler(wc->label, ButtonPressMask|ButtonReleaseMask,
		      False, TitleButtonPressEH, (XtPointer) tw);
#endif /* DYNAMIC_COLUMN_SIZING */
}

/*
 * Function:
 *	ConstraintDestroy(widget)
 * Description:
 *	This function is called when a child of the table
 *	is destroyed.  This function simply cleans up any memory that
 *	this child alloced in its constraint record.
 * Input:
 *	widget : Widget - the child being delt with.
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static void
ConstraintDestroy(Widget widget)
#else
static void
ConstraintDestroy(widget)
    Widget widget;
#endif
{
    XmTableConstraint wc = &XiC(widget);

    /*
     * First lets see if we have a column title and if so lets free the
     * puppy so that we do not dribble memory.
     */
    if( wc->_column_title != NULL )
    {
	XmStringFree(wc->_column_title);
    }

    /*
     * Finally if this column has a label then we need to destroy the
     * label widget as well.
     */
    if( wc->label ) XtDestroyWidget(wc->label);
}

/*
 * Function:
 *	ConstraintSetValues(current, request, widget, arg_list, arg_cnt)
 * Description:
 *	This function is called when the application changes a constraint
 *	resource on a child of the Table.  This function deals with the
 *	change and make the appropriate changes in the table as a whole.
 * Input:
 *	request : Widget    - the requested changes
 *	current : Widget    - the current values
 *	widget  : Widget    - the new values (request changes)
 *	arg_list : ArgList  - the args that made the changes
 *	arg_cnt  : Cardinal - the number of args
 * Output:
 *	Boolean - is a redisplay needed.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static Boolean
ConstraintSetValues(Widget current, Widget request, Widget widget,
		    ArgList arg_list, Cardinal *arg_cnt)
#else
static Boolean
ConstraintSetValues(current, request, widget, arg_list, arg_cnt)
    Widget   current, request, widget;
    ArgList  arg_list;
    Cardinal *arg_cnt;
#endif
{
    XmTableWidget     tw = (XmTableWidget) XtParent(XtParent(widget));
    XmTableConstraint cw = &(XiC(current)),
                      w = &(XiC(widget));
    XtWidgetGeometry  wanted;

    Boolean retval = False;

    if( cw->column_title != w->column_title )
    {
	if( w->_column_title != NULL )
	{
	    XmStringFree(w->_column_title);
	}
	if( w->column_title != NULL )
	{
	    w->_column_title = XmStringCopy(w->column_title);
	}
	else
	{
	    w->_column_title = NULL;
	}
	XtVaSetValues(w->label,
		      XmNrecomputeSize, False,
		      NULL);
	XtVaSetValues(w->label,
		      XmNlabelString, w->_column_title,
		      NULL);
	XtVaSetValues(w->label,
		      XmNrecomputeSize, True,
		      NULL);

	retval = False;
    }

    if( XtQueryGeometry(widget, NULL, &wanted) == XtGeometryNo ) return False;

    if( !(wanted.request_mode & CWHeight) ) wanted.height = XtHeight(widget);

    if( wanted.height != XtHeight(widget) )
    {
	CalcTableInfo(tw);
	Layout(tw);
	LayoutCells(tw);
	LayoutActive(tw);

	if( XmTable_clip(tw) != NULL && XtIsRealized(XmTable_clip(tw)) )
	{
	    XClearArea(XtDisplay(XmTable_clip(tw)), XtWindow(XmTable_clip(tw)), 0, 0, 0, 0,
		       True);
	}

	retval = True;
    }

    return retval;
}

/*
 * Function:
 *	Layout(tw)
 * Description:
 *	This function is one of the work horses of this widget.  This
 *	function places and sizes all the children of the table.
 * Input:
 *	tw : XmTableWidget - table that needs to be "layed-out"
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static void
Layout(XmTableWidget tw)
#else
static void
Layout(tw)
    XmTableWidget tw;
#endif
{
    Position   x, y, width, height,
               vmargin = XmTable_margin_height(tw),
               hmargin = XmTable_margin_width(tw),
               spacing = XmTable_spacing(tw),
               shadow  = tw->manager.shadow_thickness;
    Widget     bclip   = XmTable_title_backing(tw),
               tclip   = XmTable_title_clip(tw),
               clip    = XmTable_clip(tw),
               vscroll = XmTable_vscroll(tw),
               hscroll = XmTable_hscroll(tw);
    Cardinal   valid_cnt = XmTable_valid_cnt(tw);
    Boolean    need_vscroll, need_hscroll, change;

  
    
    /*
     * First lets check if we have any valid kids and if not we will
     * hide the title area, because there are no titles to show.
     */
    if( valid_cnt > 0 && XmTable_show_titles(tw) )
    {
       
	XtManageChild(tclip);
    }
    else
    {
       
	XtUnmanageChild(tclip);
    }

    /*
     * Next lets find calculate the amount of space that we have for the
     * table itself.  This is basically the size of the Table widget
     * minus the decorations and the title area(if the title area is
     * visible).
     */
    width = (Position) XtWidth(tw) - 2 * hmargin - 2 * shadow;
    height = (Position) XtHeight(tw) - 2 * vmargin - 2 * shadow -
	(XiValidChild(tclip) ? (XtHeight(tclip) + spacing) : 0);
  
    /*
     * Now that we know how much room we have for the table lets discover
     * if we need the scrollbars: horizontal and/or vertical.
     *
     * To find out if we need the scrollbars we use this simple loop that
     * basically says...
     *
     *	1. If we don't have enough vertical room and we are not already
     *     using the vertical scrollbar then use the vertical scrollbar and
     *     shorten the width to compensate for the vertical scrollbar.
     *
     *  2. If we don't have enough horizontal room and we are not already
     *     using the horizontal scrollbar then use the horizontal scrollbar
     * 	   and shorten the height to compensate for the horizontal scrollbar.
     *
     *  3. Repeat steps 1 & 2 until both scrollbars are being used or
     * 	   everything fits.
     *
     */
    need_vscroll = need_hscroll = False;
    do
    {
	/*
	 * Set that fact that we have not made a changed for this
	 * iteration of this loop.
	 */
	change = False;

	/*
	 * 1. If we don't have enough height and if we have not set the
	 *    fact that we want the vertical scrollbar then remember that
	 *    we want the vertical scrollbar, compensate the width for
	 *    scrollbar and mark that there was a change.
	 */
	if( !need_vscroll && XmTable_table_height(tw) > height )
	{
	    need_vscroll = True;
	    width -= (XtWidth(vscroll) + spacing);
	    change = True;
	}

	/*
	 * 2. If we don't have enough width and if we have not set the
	 *    fact that we want the horizontal scrollbar then remember that
	 *    we want the horizontal scrollbar, compensate the height for
	 *    scrollbar and mark that there was a change.
	 */
	if( !need_hscroll && XmTable_table_width(tw) > width )
	{
	    need_hscroll = True;
	    height -= (XtHeight(hscroll) + spacing);
	    change = True;
	}

	/*
	 * If we made a change that means we want to iterate the loop
	 * again since the width and/ or height have been adjusted.  If
	 * no change was made then we are done.
	 */
    } while( change );

    if( need_hscroll )
    {
	/*
	 * It seems that we want the horizontal scrollbar so lets manage and
	 * place the scrollbar in its right location at is desired size.
	 */
	XtManageChild(hscroll);

	x = hmargin;
	y = (Position) XtHeight(tw) - vmargin - (Position) XtHeight(hscroll);
	width = ((Position) XtWidth(tw) - hmargin -
		 (need_vscroll
		  ? ((Position) XtWidth(vscroll) + spacing) : 0)) - x;
	XtConfigureWidget(hscroll, x, y, width, XtHeight(hscroll), 0);
    }
    else
    {
	/*
	 * We don't have a need for this scrollbar so lets hide it
	 * away until later.
	 */
	XtUnmanageChild(hscroll);
       
    }

    if( need_vscroll )
    {
	/*
	 * It seems that we want the vertical scrollbar so lets manage and
	 * place the scrollbar in its right location at is desired size.
	 */
	XtManageChild(vscroll);

	x = (Position) XtWidth(tw) - hmargin - (Position) XtWidth(vscroll);
	y = vmargin + (XiValidChild(tclip)
		       ? (XmTable_title_height(tw) + spacing) : 0);
	height = ((Position) XtHeight(tw) - vmargin -
		  (need_hscroll ?
		   ((Position) XtHeight(hscroll) + spacing) : 0)) - y;
	XtConfigureWidget(vscroll, x, y, XtWidth(vscroll), height, 0);
    }
    else
    {
	/*
	 * We don't have a need for this scrollbar so lets hide it
	 * away until later.
	 */
	XtUnmanageChild(vscroll);
    }

    /*
     * Now that we have our scrollbar sized and placed lets find out
     * how much room we have for our other kids, i.e. the title area
     * and the table.
     */
    x = hmargin;
    width = ((Position) XtWidth(tw) - hmargin -
	     (need_vscroll ? ((Position) XtWidth(vscroll) + spacing) : 0)) - x;

    if( XtIsManaged(tclip) )
    {
	/*
	 * It seems that we want the titles show so lets place and size
	 * the title area.
	 */
	y = vmargin;
	XtConfigureWidget(tclip, x, y, width, XmTable_title_height(tw), 0);
    }

    /*
     * The below if clause is a little useless since clip should always
     * be managed, but if does serve as a nice break-up point so you can
     * tell at want point in the source this is actual happening.  In
     * other words the if is only for readability.
     */
    if( XtIsManaged(clip) )
    {
	/*
	 * Now that everything is place and sized but the clip area we
	 * know that any space left over belongs to the clip area.  So
	 * lets configure the clip area to the left overspace.
	 *
	 * Note:
	 *	We configure the clip area with "XiConfigureClip" rather
	 *	than "XtConfigureWidget" because "XiConfigureClip" does
	 *	the right thing with the shadow around the clip area.
	 */
	x += shadow;
	y = vmargin + (XtIsManaged(tclip) ? (XtHeight(tclip) + spacing) : 0) +
	    shadow;
	height = ((Position) XtHeight(tw) - vmargin - shadow -
		  (XtIsManaged(hscroll)
		   ? ((Position) XtHeight(hscroll) + spacing) : 0)) - y;
	width -= (2*shadow);
       
	if( width < 1 ) width = 1;
	if( height < 1 ) height = 1;
	XiConfigureClip(clip, x, y, width, height);
    }

    /*
     * Now that everything is set up we need to set up the scrollbar values
     * and slider sizes, so lets do that.
     */
    if( need_hscroll )
    {
	int max_size = XmTable_table_width(tw),
	    slider_size = XtWidth(clip),
	    value;

	/*
	 * First lets grab the current value of the scrollbar, because we
	 * are going to try to maintain this value if we can.
	 */
	XtVaGetValues(hscroll,
		      XmNvalue, &value,
		      NULL);

	/*
	 * Now lets massage value, slider size and max_size to fit
	 * reality.
	 */
	if( max_size == 0 )
	{
	    max_size = 1;
	    slider_size = 1;
	    value = 0;
	}
	else
	{
	    if( slider_size > max_size ) slider_size = max_size;
	    if( value + slider_size > max_size ) value = max_size -slider_size;
	}

	/*
	 * Now lets set the new values back to the scrollbar and
	 * remember the current location.
	 */
	XtVaSetValues(hscroll,
		      XmNmaximum, max_size,
		      XmNsliderSize, slider_size,
		      XmNvalue, value,
		      NULL);
	XmTable_x_origin(tw) = -value;
    }
    else
    {
	/*
	 * Here we don't have the scrollbar, so lets place the origin
	 * at 0.
	 */
	XmTable_x_origin(tw) = 0;
    }

    if( need_vscroll )
    {
	int max_size = XmTable_table_height(tw),
	    slider_size = XtHeight(clip),
	    value;

	/*
	 * First lets grab the current value of the scrollbar, because we
	 * are going to try to maintain this value if we can.
	 */
	XtVaGetValues(vscroll,
		      XmNvalue, &value,
		      NULL);

	/*
	 * Now lets massage value, slider size and max_size to fit
	 * reality.
	 */
	if( max_size == 0 )
	{
	    max_size = 1;
	    slider_size = 1;
	    value = 0;
	}
	else
	{
	    if( slider_size > max_size ) slider_size = max_size;
	    if( value + slider_size > max_size ) value = max_size -slider_size;
	}

	/*
	 * Now lets set the new values back to the scrollbar and
	 * remember the current location.
	 */
	XtVaSetValues(vscroll,
		      XmNmaximum, max_size,
		      XmNsliderSize, slider_size,
		      XmNvalue, value,
		      NULL);
	XmTable_y_origin(tw) = -value;
    }
    else
    {
	/*
	 * Here we don't have the scrollbar, so lets place the origin
	 * at 0.
	 */
	XmTable_y_origin(tw) = 0;
    }

    /*
     * Now that everything is really set up, lets move the title clipping
     * area and relayout our titles, the active widget and then
     * cause a redraw to happen in the table.
     */
    XtMoveWidget(bclip, -XmTable_x_origin(tw), 0);
    LayoutTitles(tw);
    LayoutActive(tw);
    if( XtIsRealized(XmTable_clip(tw)) )
    {
	XClearArea(XtDisplay(tw), XtWindow(XmTable_clip(tw)), 0, 0, 0, 0, True);
    }
}

/*
 * Function:
 *	FindChild(child)
 * Description:
 *	Given a widget this widget walks up the hierarchy until it finds
 *	a widget that is a child xiClipWidgetClass it returns that widget.
 *	This function is used to find a child of the table given its
 * 	focus widget.
 * Input:
 *	child : Widget - the focus widget
 * Output:
 *	Widget - the child of the table that uses that focus widget.
 */
#if NeedFunctionPrototypes
static Widget
FindChild(Widget child)
#else
static Widget
FindChild(child)
    Widget child;
#endif
{
    /*
     * Real simple here keep walking up using XtParent until we come to
     * a widget whose parent class is the clip area and then return that
     * widget.
     */
    while( XtClass(XtParent(child)) != xiTableClipWidgetClass )
    {
	child = XtParent(child);
    }

    return( child );
}

/*
 * Function:
 *	FindNextCell(cell, wrap, didWrap)
 * Description:
 *	This function finds the cell to the right of the given widget,
 *	and wraps around if wrap is true.
 * Input:
 *	cell    : Widget   - the cell you want to find "next" for
 *	wrap    : Boolean  - are we allowed to wrap around
 *	didWrap : Boolean* - (return) did we wrap?
 * Output:
 *	Widget -
 */
#if NeedFunctionPrototypes
static Widget
FindNextCell(Widget cell, Boolean wrap, Boolean *didWrap)
#else
static Widget
FindNextCell(cell, wrap, didWrap)
    Widget  cell;
    Boolean wrap, *didWrap;
#endif
{
    WidgetList kid;
    Widget     clip = XtParent(cell), first = NULL, next = NULL;
    Cardinal   kidCnt, i;
    Boolean    found = False;

    /*
     * Lets start by getting the child set that we are allowed to
     * work with
     */
    XtVaGetValues(clip,
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);

    /*
     * We are going to start by assuming that we are not going to wrap so
     * lets remember that fact.
     */
    if( didWrap != NULL ) *didWrap = False;

    /*
     * Now lets walk through the kiddies and try to find the child that
     * comes after us.
     */
    for( i = 0; i < kidCnt; ++i, ++kid )
    {
	/*
	 * Skip all illegitimate (sp?) kids
	 */
	if( !XiValidInputChild(*kid) ) continue;

	/*
	 * Next remember the first kid incase we do need to wrap.
	 */
	if( first == NULL ) first = *kid;

	/*
	 * If we have not found ourself yet then look to see if we have
	 * now, and if so remember that fact.
	 */
	if( !found )
	{
	    if( *kid == cell ) found = True;
	    continue;
	}

	/*
	 * If we get here that means that we are at the next kid, so lets
	 * save a pointer to this kid.
	 */
	next = *kid;
	break;
    }

    /*
     * If we did not find the next kid and we are allowed to wrap then
     * save the first (wrapped) kid as the next and mark the fact that
     * we did a wrap.
     */
    if( next == NULL && wrap )
    {
	if( didWrap != NULL ) *didWrap = True;
	next = first;
    }

    /*
     * Return the next kid
     */
    return( next );
}

/*
 * Function:
 *	FindPrevCell(cell, wrap, didWrap)
 * Description:
 *	This function finds the cell to the left of the given cell,
 *	and wraps around if wrap is True.
 * Input:
 *	cell    : Widget   - the cell that we want to find the prev of
 *	wrap    : Boolean  - should we attempt to wrap around
 *	didWrap : Boolean* - did we wrap?
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static Widget
FindPrevCell(Widget cell, Boolean wrap, Boolean *didWrap)
#else
static Widget
FindPrevCell(cell, wrap, didWrap)
    Widget cell;
    Boolean wrap, *didWrap;
#endif
{
    WidgetList kid;
    Widget     clip = XtParent(cell), last = NULL, prev = NULL;
    Cardinal   kidCnt;
    int        i;
    Boolean    found = False;

    /*
     * Lets start but grabbing the list of kids that we are allowed to
     * play with.
     */
    XtVaGetValues(clip,
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);

    /*
     * We will assume that we did not wrap so lets set that up.
     */
    if( didWrap != NULL ) *didWrap = False;

    /*
     * Now lets walk through the kid list in reverse searching for the
     * previous cell.
     */
    for( i = kidCnt - 1; i >= 0; --i )
    {
	/*
	 * If this is not a valid kid, then skip it.
	 */
	if( !XiValidInputChild(kid[i]) ) continue;

	/*
	 * Remember the last kid in case we need to wrap.
	 */
	if( last == NULL ) last = kid[i];

	/*
	 * If we have not found the current cell yet and this cell is
	 * the current then lets remember that we have found us.
	 */
	if( !found )
	{
	    if( kid[i] == cell ) found = True;
	    continue;
	}

	/*
	 * If we got here then this is the previous cell so lets remember
	 * it and break the loop.
	 */
	prev = kid[i];
	break;
    }

    /*
     * If we did not find the previous cell and we are supposed to wrap
     * then grab the last cell and remember that we wrapped.
     */
    if( prev == NULL && wrap )
    {
	if( didWrap != NULL ) *didWrap = True;
	prev = last;
    }

    /*
     * Return the previous widget.
     */
    return( prev );
}

/*
 * Function:
 *	XmTableNextTabGroup(widget, event, params, num_params)
 * Description:
 *	This function moves the traversal to the next tab group.
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableNextTabGroup(Widget widget, XEvent *event, String *params,
		    Cardinal *num_params)
#else
static void
XmTableNextTabGroup(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    XmProcessTraversal(widget, XmTRAVERSE_NEXT_TAB_GROUP);
}

/*
 * Function:
 *	XmTablePrevTabGroup(widget, event, params, num_params)
 * Description:
 *	This function moves the traversal to the previous tab group.
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTablePrevTabGroup(Widget widget, XEvent *event, String *params,
		    Cardinal *num_params)
#else
static void
XmTablePrevTabGroup(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    XmProcessTraversal(widget, XmTRAVERSE_PREV_TAB_GROUP);
}

/*
 * Function:
 *	XmTableNextCell(widget, event, params, num_params)
 * Description:
 *	This action proc handles the next cell (tab) traversal
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableNextCell(Widget widget, XEvent *event, String *params,
		 Cardinal *num_params)
#else
static void
XmTableNextCell(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    Widget        child = FindChild(widget);
    XmTableWidget tw = (XmTableWidget) XtParent(XtParent(child));
    Boolean       wrapped;
    Widget        next = FindNextCell(child, True, &wrapped);
    int		  cell_y = XmTable_cell_y(tw);

    if( wrapped ) cell_y++;

    if( cell_y >= XmTable_num_rows(tw) ) return;

    SetActive(tw, next, cell_y, (wrapped ? XiVisLowerLeft : XiVisLowerRight));
}

/*
 * Function:
 *	XmTablePreviousCell(widget, event, params, num_params)
 * Description:
 *	Handles traversal to the previous cell.
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTablePreviousCell(Widget widget, XEvent *event, String *params,
		    Cardinal *num_params)
#else
static void
XmTablePreviousCell(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    Widget         child = FindChild(widget);
    XmTableWidget tw = (XmTableWidget) XtParent(XtParent(child));
    Boolean        wrapped;
    Widget         prev = FindPrevCell(child, True, &wrapped);
    int		   cell_y = XmTable_cell_y(tw);

    if( wrapped ) cell_y--;

    if( cell_y < 0 ) return;

    SetActive(tw, prev, cell_y, (wrapped ? XiVisUpperRight : XiVisUpperLeft));
}

/*
 * Function:
 *	XmTableUpCell(widget, event, params, num_params)
 * Description:
 *	Handles traversal to the cell above the current
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableUpCell(Widget widget, XEvent *event, String *params,
	      Cardinal *num_params)
#else
static void
XmTableUpCell(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    Widget         child = FindChild(widget);
    XmTableWidget tw = (XmTableWidget) XtParent(XtParent(child));
    int		   cell_y = XmTable_cell_y(tw);

    if( cell_y > 0 )
    {
	SetActive(tw, XmTable_active_cell(tw), --cell_y, XiVisUpper);
    }
}

/*
 * Function:
 *	XmTableDownCell(widget, event, params, num_params)
 * Description:
 *	Handles traversal to the cell below the current
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableDownCell(Widget widget, XEvent *event, String *params,
		Cardinal *num_params)
#else
static void
XmTableDownCell(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    Widget         child = FindChild(widget);
    XmTableWidget tw = (XmTableWidget) XtParent(XtParent(child));
    int		   cell_y = XmTable_cell_y(tw);

    if( cell_y < (XmTable_num_rows(tw)-1) )
    {
	SetActive(tw, XmTable_active_cell(tw), ++cell_y, XiVisLower);
    }
}

/*
 * Function:
 *	XmTableRightCell(widget, event, params, num_params)
 * Description:
 *	Handles traversal to the cell to the right of the current
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableRightCell(Widget widget, XEvent *event, String *params,
		 Cardinal *num_params)
#else
static void
XmTableRightCell(widget, event, params,num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    Widget         child = FindChild(widget);
    XmTableWidget tw = (XmTableWidget) XtParent(XtParent(child));
    Widget         next = FindNextCell(child, False, NULL);

    if( next != NULL )
    {
	SetActive(tw, next, XmTable_cell_y(tw), XiVisRight);
    }
}

/*
 * Function:
 *	XmTableLeftCell(widget, event, params, num_params)
 * Description:
 *	Handles traversal to the cell to the left of the current
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableLeftCell(Widget widget, XEvent *event, String *params,
		Cardinal *num_params)
#else
static void
XmTableLeftCell(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    Widget         child = FindChild(widget);
    XmTableWidget tw = (XmTableWidget) XtParent(XtParent(child));
    Widget         prev = FindPrevCell(child, False, NULL);

    if( prev != NULL )
    {
	SetActive(tw, prev, XmTable_cell_y(tw), XiVisLeft);
    }
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTablePageDown(Widget widget, XEvent *event, String *params,
		Cardinal *num_params)
#else
static void
XmTablePageDown(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    /*DKB - Not Yet Implemented */
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTablePageUp(Widget widget, XEvent *event, String *params,
	      Cardinal *num_params)
#else
static void
XmTablePageUp(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    /*DKB - Not Yet Implemented */
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableBeginData(Widget widget, XEvent *event, String *params,
		 Cardinal *num_params)
#else
static void
XmTableBeginData(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    Widget         child = FindChild(widget);
    XmTableWidget tw = (XmTableWidget) XtParent(XtParent(child));
    WidgetList     kid;
    Cardinal       kidCnt, i;

    XtVaGetValues(XmTable_clip(tw),
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);

    for( i = 0; i < kidCnt; ++i )
    {
	if( !XiValidChild(kid[i]) ) continue;

	SetActive(tw, kid[i], 0, XiVisUpperLeft);
	return;
    }
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableEndData(Widget widget, XEvent *event, String *params,
	       Cardinal *num_params)
#else
static void
XmTableEndData(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    Widget         child = FindChild(widget), last = NULL;
    XmTableWidget tw = (XmTableWidget) XtParent(XtParent(child));
    WidgetList     kid;
    Cardinal       kidCnt, i;

    XtVaGetValues(XmTable_clip(tw),
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);

    for( i = 0; i < kidCnt; ++i )
    {
	if( !XiValidChild(kid[i]) ) continue;

	last = kid[i];
    }

    if( last != NULL )
    {
	SetActive(tw, last, XmTable_num_rows(tw) - 1, XiVisLowerRight);
    }
}

/*
 * Function:
 *	XmTableRestoreAndSend(widget, event, params, num_params)
 * Description:
 *	This action first replaces the edit translations on the
 *	given widget and redispatches the event.
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableRestoreAndSend(Widget widget, XEvent *event, String *params,
		      Cardinal *num_params)
#else
static void
XmTableRestoreAndSend(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    /*
     * First lets find the child of the Table widget that this
     * widget belongs to.
     */
    while( XtParent(widget) &&
	   XtClass(XtParent(widget)) != xiTableClipWidgetClass )
    {
	widget = XtParent(widget);
    }

    /*
     * If we have not yet initied the translations for this puppy
     * then skip it because bad things could happen.
     */
    if( XtParent(widget) == NULL || !XiC(widget).translations_inited )
    {
	return;
    }

    /*
     * All seems well so lets set the translations and then
     * dispatch the event again so the action that caused this will
     * happen to the widget correctly.
     */
    XtVaSetValues(XiCallFocusWidget(widget),
		  XmNtranslations, XiC(widget).edit_translations,
		  NULL);
    XtDispatchEvent(event);
}

/*
 * Function:
 *	XmTableRestore(widget, event, params, num_params)
 * Description:
 *	This action simply restores the default edit translations
 *	and unselects the text.
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableRestore(Widget widget, XEvent *event, String *params,
	       Cardinal *num_params)
#else
static void
XmTableRestore(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    /*
     * First lets find the child of the Table widget that this
     * widget belongs to.
     */
    while( XtParent(widget) &&
	   XtClass(XtParent(widget)) != xiTableClipWidgetClass )
    {
	widget = XtParent(widget);
    }

    /*
     * If we have not yet initied the translations for this puppy
     * then skip it because bad things could happen.
     */
    if( XtParent(widget) == NULL || !XiC(widget).translations_inited )
    {
	return;
    }

    /*
     * All seems well so lets set the translations and then
     * unselect the text so that the user can do whatever they want to
     * do.
     */
    XtVaSetValues(XiCallFocusWidget(widget),
		  XmNtranslations, XiC(widget).edit_translations,
		  NULL);

    XiCallUnselectText(widget);
}

/*
 * Function:
 *	XmTableReinstateTraversal(widget, event, params, num_params)
 * Description:
 *	This action replaces the edit translations with the
 *	traversal translations and then redispatches the event so
 *	the actual traversal happens.
 * Input:
 *	widget     : Widget    - the cell that catch this action
 *	event      : XEvent*   - the event that caused the action
 *	params     : String*   - action parameter list (unused)
 *	num_params : Cardinal* - number of params in list (unused)
 * Output:
 *	None.
 */
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XmTableReinstateTraversal(Widget widget, XEvent *event, String *params,
			  Cardinal *num_params)
#else
static void
XmTableReinstateTraversal(widget, event, params, num_params)
    Widget   widget;
    XEvent   *event;
    String   *params;
    Cardinal *num_params;
#endif
{
    /*
     * First lets find the child of the Table widget that this
     * widget belongs to.
     */
    while( XtParent(widget) &&
	   XtClass(XtParent(widget)) != xiTableClipWidgetClass )
    {
	widget = XtParent(widget);
    }

    /*
     * If we have not yet initied the translations for this puppy
     * then skip it because bad things could happen.
     */
    if( XtParent(widget) == NULL || !XiC(widget).translations_inited )
    {
	return;
    }

    /*
     * All seems well so lets set the translations and then
     * dispatch the event again so the action that caused this will
     * happen to the widget correctly.
     */
    XtVaSetValues(XiCallFocusWidget(widget),
		  XmNtranslations, XiC(widget).traversal_translations,
		  NULL);
    XtDispatchEvent(event);
}

#if NeedFunctionPrototypes
static void
_XiDefaultTitleForeground(Widget widget, int offset, XrmValue *value)
#else
static void
_XiDefaultTitleForeground(widget, offset, value)
    Widget   widget;
    int      offset;
    XrmValue *value;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;
    Colormap            colormap;
    Pixel               background;
    XmColorData         *color_data;
    static Pixel        pixel;

    value->size = sizeof(Pixel);
    value->addr = (XtPointer) &pixel;

    if( !XtIsSubclass(widget, xmTableWidgetClass) )
    {
	_XmForegroundColorDefault(widget, offset, value);
	return;
    }

    background = XmTable_title_background(tw);
    colormap = tw->core.colormap;

    color_data = _XmGetColors(XtScreen(widget), colormap, background);

    pixel = _XmAccessColorData(color_data, XmFOREGROUND);
}

#if NeedFunctionPrototypes
static void
_XiDefaultTitleTopShadowColor(Widget widget, int offset, XrmValue *value)
#else
static void
_XiDefaultTitleTopShadowColor(widget, offset, value)
    Widget   widget;
    int      offset;
    XrmValue *value;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;
    Colormap            colormap;
    Pixel               background;
    XmColorData         *color_data;
    static Pixel        pixel;

    value->size = sizeof(Pixel);
    value->addr = (XtPointer) &pixel;

    if( !XtIsSubclass(widget, xmTableWidgetClass) )
    {
	_XmTopShadowColorDefault(widget, offset, value);
	return;
    }

    background = XmTable_title_background(tw);
    colormap = tw->core.colormap;

    color_data = _XmGetColors(XtScreen(widget), colormap, background);

    pixel = _XmAccessColorData(color_data, XmTOP_SHADOW);
}

#if NeedFunctionPrototypes
static void
_XiDefaultTitleTopShadowPixmap(Widget widget, int offset, XrmValue *value)
#else
static void
_XiDefaultTitleTopShadowPixmap(widget, offset, value)
    Widget   widget;
    int      offset;
    XrmValue *value;
#endif
{
    XmTableWidget tw;
    static Pixmap  pixmap;

    pixmap = XmUNSPECIFIED_PIXMAP;

    value->size = sizeof(Pixel);
    value->addr = (XtPointer) &pixmap;

    if( !XtIsSubclass(widget, xmTableWidgetClass) )
    {
	_XmManagerTopShadowPixmapDefault(widget, offset, value);
	return;
    }

    tw = (XmTableWidget) widget;

    if( XmTable_title_top_shadow_color(tw) == XmTable_title_background(tw) )
    {
	pixmap = XmGetPixmapByDepth(XtScreen(tw), "50_foreground",
				    XmTable_title_top_shadow_color(tw),
				    tw->manager.foreground,
				    tw->core.depth);
    }
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
_XiDefaultTitleBottomShadowColor(Widget widget, int offset, XrmValue *value)
#else
static void
_XiDefaultTitleBottomShadowColor(widget, offset, value)
    Widget   widget;
    int      offset;
    XrmValue *value;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;
    XmColorData    *color_data;
    static Pixel   pixel;

    value->size = sizeof(Pixel);
    value->addr = (XtPointer) &pixel;

    color_data = _XmGetColors(XtScreen(widget), tw->core.colormap,
			      XmTable_title_background(tw));

    pixel = _XmAccessColorData(color_data, XmBOTTOM_SHADOW);
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
_XiDefaultTitleShadeColor(Widget widget, int offset, XrmValue *value)
#else
static void
_XiDefaultTitleShadeColor(widget, offset, value)
    Widget   widget;
    int      offset;
    XrmValue *value;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;
    XmColorData    *color_data;
    static Pixel   pixel;

    value->size = sizeof(Pixel);
    value->addr = (XtPointer) &pixel;

    color_data = _XmGetColors(XtScreen(widget), tw->core.colormap,
			      XmTable_title_background(tw));

    pixel = _XmAccessColorData(color_data, XmSELECT);
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
_XiDefaultTableColor(Widget widget, int offset, XrmValue *value)
#else
static void
_XiDefaultTableColor(widget, offset, value)
    Widget   widget;
    int      offset;
    XrmValue *value;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;
    XmColorData    *color_data;
    static Pixel   pixel;
    Pixel          tmp;

    value->size = sizeof(Pixel);
    value->addr = (XtPointer) &pixel;

    color_data = _XmGetColors(XtScreen(widget), tw->core.colormap,
			      tw->core.background_pixel);

    tmp = _XmAccessColorData(color_data, XmTOP_SHADOW);

    color_data = _XmGetColors(XtScreen(widget), tw->core.colormap, tmp);

    pixel = _XmAccessColorData(color_data, XmTOP_SHADOW);
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
_XiDefaultLineColor(Widget widget, int offset, XrmValue *value)
#else
static void
_XiDefaultLineColor(widget, offset, value)
    Widget   widget;
    int      offset;
    XrmValue *value;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;
    XmColorData    *color_data;
    static Pixel   pixel;

    value->size = sizeof(Pixel);
    value->addr = (XtPointer) &pixel;

    color_data = _XmGetColors(XtScreen(widget), tw->core.colormap,
			      XmTable_table_color(tw));

    pixel = _XmAccessColorData(color_data, XmBOTTOM_SHADOW);
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
_XiDefaultShadeColor(Widget widget, int offset, XrmValue *value)
#else
static void
_XiDefaultShadeColor(widget, offset, value)
    Widget   widget;
    int      offset;
    XrmValue *value;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;
    XmColorData    *color_data;
    static Pixel   pixel;

    value->size = sizeof(Pixel);
    value->addr = (XtPointer) &pixel;

    color_data = _XmGetColors(XtScreen(widget), tw->core.colormap,
			      XmTable_table_color(tw));

    pixel = _XmAccessColorData(color_data, XmSELECT);
}

#if NeedFunctionPrototypes
static GC
CreateGC(Widget widget, Pixel pixel, Pixmap pixmap, Boolean move_gc)
#else
static GC
CreateGC(widget, pixel, pixmap, move_gc)
    Widget  widget;
    Pixel   pixel;
    Pixmap  pixmap;
    Boolean move_gc;
#endif
{
    XtGCMask  mask;
    XGCValues values;

    mask = GCForeground;
    values.foreground = pixel;

    if( XiValidPixmap(pixmap) )
    {
	mask |= (GCTile | GCFillStyle);
	values.fill_style = FillTiled;
	values.tile = pixmap;
    }

    if( move_gc )
    {
	mask |= (GCFunction | GCSubwindowMode | GCPlaneMask);
	values.function = GXinvert;
	values.subwindow_mode = IncludeInferiors;
	values.plane_mask = pixel ^ widget->core.background_pixel;
    }

    return( XtGetGC(widget, mask, &values) );
}

/*
 * Function:
 *	SetUpGCs(tw)
 * Description:
 *	This routine gets GCs for each of the Table's GC that are not
 *	valid (i.e. NULL).
 * Input:
 *	tw : XmTableWidget - the widget who needs GCs
 * Output:
 *	None.
 */
#if NeedFunctionPrototypes
static void
SetUpGCs(XmTableWidget tw)
#else
static void
SetUpGCs(tw)
    XmTableWidget tw;
#endif
{
    if( XmTable_foreground_GC(tw) == NULL )
    {
	XmTable_foreground_GC(tw) =
	    CreateGC((Widget) tw, tw->manager.foreground,
		     XmUNSPECIFIED_PIXMAP, False);
    }

    if( XmTable_move_GC(tw) == NULL )
    {
	XmTable_move_GC(tw) =
	    CreateGC((Widget) tw, tw->manager.foreground,
		     XmUNSPECIFIED_PIXMAP, True);
    }

    if( XmTable_title_top_shadow_GC(tw) == NULL )
    {
	XmTable_title_top_shadow_GC(tw) =
	    CreateGC((Widget) tw, XmTable_title_top_shadow_color(tw),
		     XmTable_title_top_shadow_pixmap(tw), False);
    }

    if( XmTable_title_bottom_shadow_GC(tw) == NULL )
    {
	XmTable_title_bottom_shadow_GC(tw) =
	    CreateGC((Widget) tw, XmTable_title_bottom_shadow_color(tw),
		     XmTable_title_bottom_shadow_pixmap(tw), False);
    }

    if( XmTable_table_GC(tw) == NULL )
    {
	XmTable_table_GC(tw) =
	    CreateGC((Widget) tw, XmTable_table_color(tw),
		     XmTable_table_pixmap(tw), False);
    }

    if( XmTable_line_GC(tw) == NULL )
    {
	XmTable_line_GC(tw) =
	    CreateGC((Widget) tw, XmTable_line_color(tw),
		     XmTable_line_pixmap(tw), False);
    }

    if( XmTable_shade_GC(tw) == NULL )
    {
	XmTable_shade_GC(tw) =
	    CreateGC((Widget) tw, XmTable_shade_color(tw),
		     XmTable_shade_pixmap(tw), False);
    }
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
TitleLabelExposeEH(Widget label, XtPointer client, XEvent *event,
		   Boolean *continue_dispatch)
#else
static void
TitleLabelExposeEH(label, client, event, continue_dispatch)
    Widget    label;
    XtPointer client;
    XEvent    *event;
    Boolean   *continue_dispatch;
#endif
{
    XmTableWidget tw = (XmTableWidget) client;

    XmeDrawShadows(XtDisplay(label), XtWindow(label),
		   XmTable_title_top_shadow_GC(tw),
		   XmTable_title_bottom_shadow_GC(tw),
		   0, 0, XtWidth(label), XtHeight(label),
		   tw->manager.shadow_thickness, XmSHADOW_OUT);
}

#ifdef DYNAMIC_COLUMN_SIZING
/* ARGSUSED */
#if NeedFunctionPrototypes
static void
TitleButtonPressEH(Widget widget, XtPointer client, XEvent *event,
		   Boolean *continue_dispatch)
#else
static void
TitleButtonPressEH(widget, client, event, continue_dispatch)
    Widget    widget;
    XtPointer client;
    XEvent    *event;
    Boolean   *continue_dispatch;
#endif
{
    XmTableWidget tw = (XmTableWidget) client;
    Widget         label;
    Position       width = XtWidth(widget), x, dx;

    if( event->xany.type != ButtonPress &&
        event->xany.type != ButtonRelease ||
        event->xbutton.button != Button1 ) return;

    x = event->xmotion.x;

    if( XtClass(widget) == xmLabelWidgetClass )
    {
	if( x > DEFAULT_BUFFER_SIZE && x < (width - DEFAULT_BUFFER_SIZE) )
	{
	    return;
	}
    }

    /*
     * If we get here that means that we really want to initiate a move
     */
    if( event->xbutton.type == ButtonPress )
    {
	if( XtClass(widget) == xmLabelWidgetClass )
	{
	    x += XtX(widget);
	}
	if( (XmTable_move_widget(tw) = GetMoveWidget(tw, x)) == NULL )
	{
	    return;
	}

	label = XiC(XmTable_move_widget(tw)).label;
	if( x < XtX(label) + XmTable_half_line_width(tw) + MIN_CELL_SIZE )
	{
	    x = XtX(label) + XmTable_half_line_width(tw) + MIN_CELL_SIZE;
	}

	XmTable_move_start_x(tw) = XmTable_move_cur_x(tw) = x;
	XGrabPointer(XtDisplay(widget), XtWindow(XmTable_title_backing(tw)),
		     False, ButtonReleaseMask | PointerMotionMask,
		     GrabModeAsync, GrabModeAsync,
		     None, XmTable_move_cursor(tw),
		     event->xbutton.time);
	XtAddGrab(XmTable_title_backing(tw), True, False);
	XmTable_move_status(tw) = XiSTART_MOVE;
    }
    else if( XmTable_move_widget(tw) != NULL )
    {
	label = XiC(XmTable_move_widget(tw)).label;
	if( XmTable_move_status(tw) == XiDOING_MOVE )
	{
	    if( !XiC(XmTable_move_widget(tw)).resizable )
	    {

		XmTable_move_cur_x(tw) =
		    XtX(label) + XtWidth(label) + XmTable_half_line_width(tw);
	    }
	    DrawMoveLine(tw, -1, True);
	}
	XUngrabPointer(XtDisplay(widget), event->xbutton.time);
	XtRemoveGrab(XmTable_title_backing(tw));

	dx = (XmTable_move_cur_x(tw) - XmTable_move_start_x(tw));
	if( XiC(XmTable_move_widget(tw)).resizable && dx != 0 )
	{
	    XtResizeWidget(XmTable_move_widget(tw),
			   XtWidth(XmTable_move_widget(tw)) + dx,
			   XtHeight(XmTable_move_widget(tw)), 0);
	    XtResizeWidget(label,
			   XtWidth(XmTable_move_widget(tw)),
			   XtHeight(label), 0);
	    CalcTableInfo(tw);
	    LayoutCells(tw);
	    Layout(tw);
	}

	XmTable_move_status(tw) = XmNO_MOVE;
	XmTable_move_widget(tw) = NULL;
    }
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
TitleMotionEH(Widget widget, XtPointer client, XEvent *event,
	      Boolean *continue_dispatch)
#else
static void
TitleMotionEH(widget, client, event, continue_dispatch)
    Widget    widget;
    XtPointer client;
    XEvent    *event;
    Boolean   *continue_dispatch;
#endif
{
    XmTableWidget tw = (XmTableWidget) client;
    Position       width = XtWidth(widget), x;
    Widget         label;
    int            dx;

    if( event->xany.type != MotionNotify ||
        (XmTable_move_status(tw) != XmNO_MOVE &&
	 XmTable_move_widget(tw) == NULL) )
    {
	return;
    }

    x = event->xmotion.x;

    switch( XmTable_move_status(tw) )
    {
    case XiSTART_MOVE:
	if( XtClass(widget) == xmLabelWidgetClass )
	{
	    x += XtX(widget);
	}
	label = XiC(XmTable_move_widget(tw)).label;
	if( x < XtX(label) + XmTable_half_line_width(tw) + MIN_CELL_SIZE )
	{
	    x = XtX(label) + XmTable_half_line_width(tw) + MIN_CELL_SIZE;
	}
	dx = XmTable_move_start_x(tw) - x;
	if( dx < 0 ) dx = -dx;
	if( dx <= MOVE_THRESHOLD )
	{
	    XmTable_move_status(tw) = XiDOING_MOVE;
	}
	if( XmTable_move_status(tw) == XiDOING_MOVE )
	{
	    if( !XiC(XmTable_move_widget(tw)).resizable )
	    {
		label = XiC(XmTable_move_widget(tw)).label;

		DrawMoveLine(tw, XtX(label) + XtWidth(label) +
			     XmTable_half_line_width(tw), False);
	    }
	    else
	    {
		DrawMoveLine(tw, x, False);
	    }
	}
	XmTable_move_cur_x(tw) = x;
	break;
    case XiDOING_MOVE:
	label = XiC(XmTable_move_widget(tw)).label;
	if( x < XtX(label) + XmTable_half_line_width(tw) + MIN_CELL_SIZE )
	{
	    x = XtX(label) + XmTable_half_line_width(tw) + MIN_CELL_SIZE;
	}
	if( x > (int) XtWidth(XmTable_clip(tw)) ) x = XtWidth(XmTable_clip(tw));
	if( x != XmTable_move_cur_x(tw) &&
	    XiC(XmTable_move_widget(tw)).resizable )
	{
	    DrawMoveLine(tw, x, True);
	}
	XmTable_move_cur_x(tw) = x;
	break;
    case XmNO_MOVE:
    default:
	if( XtClass(widget) == xmLabelWidgetClass )
	{
	    if( x > DEFAULT_BUFFER_SIZE && x < (width - DEFAULT_BUFFER_SIZE) )
	    {
		XUndefineCursor(XtDisplay(widget),
				XtWindow(XmTable_title_backing(tw)));
	    }
	    else
	    {
		XDefineCursor(XtDisplay(widget),
			      XtWindow(XmTable_title_backing(tw)),
			      XmTable_move_cursor(tw));
	    }
	}
	else
	{
	    XDefineCursor(XtDisplay(widget),
			  XtWindow(XmTable_title_backing(tw)),
			  XmTable_move_cursor(tw));
	}
	break;
    }
}
#endif /* DYNAMIC_COLUMN_SIZING */


#if NeedFunctionPrototypes
static void
LayoutTitles(XmTableWidget tw)
#else
static void
LayoutTitles(tw)
    XmTableWidget tw;
#endif
{
    Widget     clip = XmTable_clip(tw);
    WidgetList kid;
    Cardinal   kidCnt, i;
    Dimension  line_width = XmTable_line_width(tw);
    Position   x = line_width + tw->manager.shadow_thickness;

    XtVaGetValues(clip,
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);
   
    for( i = 0; i < kidCnt; ++i )
    {
	if( !XiValidChild(kid[i]) )
	{
	    XiC(kid[i]).column_index = -1;
	    continue;
	}
        
	XiC(kid[i]).column_index = i;
	XtMoveWidget(XiC(kid[i]).label, x, 0);
	XtConfigureWidget(XiC(kid[i]).label, x, 0,
			  XtWidth(kid[i]), XmTable_title_height(tw), 0);
	x += (XtWidth(XiC(kid[i]).label) + line_width);
    }
    XtConfigureWidget(XmTable_title_backing(tw), XmTable_x_origin(tw), 0, x,
		      XmTable_title_height(tw), 0);
}

#if NeedFunctionPrototypes
static void
LayoutActive(XmTableWidget tw)
#else
static void
LayoutActive(tw)
    XmTableWidget tw;
#endif
{
    int x, y;

    if( XmTable_active_cell(tw) == NULL ) return;

    x = XiXOffset(XmTable_active_cell(tw));
    y = XmYOffset(tw, XmTable_cell_y(tw));

    XtConfigureWidget(XmTable_active_cell(tw), x + XmTable_x_origin(tw), y + XmTable_y_origin(tw),
		      XtWidth(XmTable_active_cell(tw)), XmTable_row_height(tw), 0);
}

#if NeedFunctionPrototypes
static void
LayoutCells(XmTableWidget tw)
#else
static void
LayoutCells(tw)
    XmTableWidget tw;
#endif
{
    Widget     clip = XmTable_clip(tw);
    WidgetList kid;
    Cardinal   kidCnt, i;
    Dimension  lineWidth = XmTable_line_width(tw);
    Position   x = lineWidth;

    XtVaGetValues(clip,
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);
    
    for( i = 0; i < kidCnt; ++i, ++kid )
    {
	if( !XiValidChild(*kid) ) continue;

	XiXOffset(*kid) = x;
	XtConfigureWidget(*kid, x,XmYOffset(tw, XmTable_cell_y(tw)),
			  XtWidth(*kid), XmTable_row_height(tw), 0);
	x += (XtWidth(*kid) + lineWidth);
       
    }
}

#if NeedFunctionPrototypes
static void
TableExposeCB(Widget clip, XtPointer client, XtPointer cbdata)
#else
static void
TableExposeCB(clip, client, cbdata)
    Widget    clip;
    XtPointer client, cbdata;
#endif
{
    XmTableWidget               tw = (XmTableWidget) client;
    XmDrawingAreaCallbackStruct *draw = (XmDrawingAreaCallbackStruct*) cbdata;
    XEvent                      *event = draw->event;
    WidgetList                  kid;
    Cardinal                    kidCnt;
    int                         i, rowHeight, numCols, lineWidth, lineIdx,
                                y, y2, x, x2, numLines, tableWidth,
                                tableHeight, x_origin, y_origin, grayIdx,
                                top, bottom, left, right, numRows;
    XRectangle                  *lines, gray[2], rect, xrect;

    if( event )
    {
	switch( event->type )
	{
	default:
	    return;
	case NoExpose:
	    SmScrollEvent(XmTable_scroll_mgr(tw), event);
	    return;
	case GraphicsExpose:
	case Expose:
	    break;
	}
    }

    SmScrollEvent(XmTable_scroll_mgr(tw), event);

    xrect.x = event->xexpose.x;
    xrect.y = event->xexpose.y;

    xrect.x = Max(xrect.x, 0);
    xrect.y = Max(xrect.y, 0);
    xrect.width = xrect.x + event->xexpose.width;
    xrect.height = xrect.y + event->xexpose.height;

    xrect.width = Min(xrect.width, XtWidth(XmTable_clip(tw))-1);
    xrect.height = Min(xrect.height, XtHeight(XmTable_clip(tw))-1);

    XtVaGetValues(clip,
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);

    /*
     * First thing that we want to do is find out the height of each of
     * our rows.
     */
    numCols = XmTable_valid_cnt(tw);
    tableWidth = XmTable_table_width(tw);
    rowHeight = XmTable_row_height(tw);

   
    x_origin = XmTable_x_origin(tw);
    y_origin = XmTable_y_origin(tw);

    /*
     * Now lets calculate the block of cells that need to be expose.
     */
    top = bottom = -1;
    numRows = 0;
  
    for( i = 0; i < XmTable_num_rows(tw); ++i )
    {
	y = y_origin + XmYOffset(tw, i);
	y2 = y + rowHeight;

	if( y2 < xrect.y || y > (int) xrect.height )
        {   
             continue;
         }
	if( top == -1 )
	{
	    top = i;
	}
	bottom = i;
	numRows++;
    }

    left = right = -1;
    numCols = 0;
    for( i = 0; i < kidCnt; ++i )
    {
	if( !XiValidChild(kid[i]) ) 
        {   
         
            continue;
        }

	x = x_origin + XiXOffset(kid[i]);
	x2 = x + XtWidth(kid[i]);

	if( x2 < xrect.x || x > (int) xrect.width ) continue;

	if( left == -1 )
	{
	    left = i;
	}
	right = i;
	numCols++;
    }
   
     
    /*
     * If we get here without any columns, that means that we have no valid
     * children, so lets just clear our play area and return.
     */
    if( numCols == 0 || numRows == 0 )
    {
      
	XFillRectangle(XtDisplay(clip), XtWindow(clip), XmTable_shade_GC(tw),
		       xrect.x, xrect.y, event->xexpose.width,
		       event->xexpose.height);
	return;
    }

    if( top < 0 || bottom < 0 || right < 0 || left < 0 )
    {
	fprintf(stderr, "Got here with a negative cell position\n");
	fprintf(stderr,
		"  top = %d\n  bottom = %d\n  right = %d\n  left = %d\n",
		top, bottom, right, left);
	exit(1);
    }

    /*
     * Now that we know both the number of columns that we are dealing with
     * and the number of rows that we want lets calculate the number of
     * lines that we are going to need to draw.  Then lets allocate some
     * memory so that we can draw all the lines in one blast.
     */
    numLines = (numCols + 1) + (numRows + 1);
    lines = (XRectangle*) XtMalloc(numLines * sizeof(XRectangle));
    lineWidth = XmTable_line_width(tw);
    y = XmYOffset(tw, top) - lineWidth;
    x = XiXOffset(kid[left]) - lineWidth;
    tableHeight = XmYOffset(tw, bottom) + rowHeight + lineWidth - y;
    tableWidth = XiXOffset(kid[right]) + XtWidth(kid[right]) +
	lineWidth - x;

    /*
     * Now lets walk through store away all the values for the horizontal
     * lines.
     */
    for( i = 0, lineIdx = 0; i <= numRows; ++i, lineIdx++ )
    {
	lines[lineIdx].x = x + x_origin;
	lines[lineIdx].y = y + y_origin;
	lines[lineIdx].width = tableWidth;

	lines[lineIdx].height = lineWidth;
	y += (lineWidth + rowHeight);
    }

    /*
     * Now lets walk through our kids one more time this time getting
     * and storing information for the vertical lines.
     */
    y = XmYOffset(tw, top) - lineWidth;
    for( i = 0, x2 = left; i < numCols; ++x2 )
    {
	if( !XiValidChild(kid[x2]) ) continue;

	lines[lineIdx].x = x + x_origin;
	lines[lineIdx].y = y + y_origin;
	lines[lineIdx].width = lineWidth;
	lines[lineIdx].height = tableHeight;
	x += (XtWidth(kid[x2]) + lineWidth);
	lineIdx++;
	i++;
    }
    lines[lineIdx].x = x + x_origin;
    lines[lineIdx].y = y + y_origin;
    lines[lineIdx].width = lineWidth;
    lines[lineIdx].height = tableHeight;
    lineIdx++;

    grayIdx = 0;
    tableWidth = XmTable_table_width(tw);
    if( (tableWidth + x_origin) < (int) XtWidth(clip) )
    {
	gray[grayIdx].x = tableWidth + x_origin;
	gray[grayIdx].y = 0;
	gray[grayIdx].width = XtWidth(clip) - gray[grayIdx].x;
	gray[grayIdx].height = XtHeight(clip);
	grayIdx++;
    }

    tableHeight = XmTable_table_height(tw);
    if( (tableHeight + y_origin) < (int) XtHeight(clip) )
    {
	gray[grayIdx].x = 0;
	gray[grayIdx].y = tableHeight + y_origin;
	gray[grayIdx].width = XtWidth(clip);
	gray[grayIdx].height = XtHeight(clip) - gray[grayIdx].y;
	grayIdx++;
    }

    XFillRectangles(XtDisplay(clip), XtWindow(clip), XmTable_line_GC(tw),
		    lines, lineIdx);

    if( grayIdx > 0 )
    {
	XFillRectangles(XtDisplay(clip), XtWindow(clip),
			XmTable_shade_GC(tw), gray, grayIdx);
    }

    XtFree((XtPointer) lines);

    /*
     * Now lets fill in the blanks
     */
    for( i = left; i <= right; ++i )
    {
	String tmp;

	if( !XiValidChild(kid[i]) ) continue;

	for( y = top; y <= bottom; y++ )
	{
	    if( kid[i] == XmTable_active_cell(tw) &&
	        XmTable_cell_y(tw) == y ) continue;

	    rect.x = XiXOffset(kid[i]) + x_origin;
	    rect.y = XmYOffset(tw, y) + y_origin;
	    rect.width = XtWidth(kid[i]);
	    rect.height = XmTable_row_height(tw);
	    tmp = XiFetchCellValue(tw, kid[i], y);
	    XiCallRender(tw, kid[i], &rect, tmp);
	    XiFreeCellValue(tw, tmp);
	}
    }
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
TableResizeCB(Widget clip, XtPointer client, XtPointer cbdata)
#else
static void
TableResizeCB(clip, client, cbdata)
    Widget    clip;
    XtPointer client, cbdata;
#endif
{
    /* EMPTY */
}

#if NeedFunctionPrototypes
static void
TableInputCB(Widget clip, XtPointer client, XtPointer cbdata)
#else
static void
TableInputCB(clip, client, cbdata)
    Widget    clip;
    XtPointer client, cbdata;
#endif
{
    XmDrawingAreaCallbackStruct *draw;
    XmTableWidget              tw = (XmTableWidget) client;
    Widget                      active;
    WidgetList                  kid;
    Cardinal                    kidCnt, i;
    Position			x, y;
    int                         cell_y, x_origin, y_origin;

    draw = (XmDrawingAreaCallbackStruct*) cbdata;

    if( draw->event->xany.type != ButtonPress ) return;

    x_origin = XmTable_x_origin(tw);
    y_origin = XmTable_y_origin(tw);

    x = draw->event->xbutton.x;
    y = draw->event->xbutton.y;

    if( x > (x_origin + (int) XmTable_table_width(tw)) ||
        y > (y_origin + (int) XmTable_table_height(tw)) ) return;

    XtVaGetValues(clip,
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);

    for( i = 0, active = NULL; i < kidCnt; ++i, ++kid )
    {
	if( !XiValidChild(*kid) ) continue;

	if( x < (x_origin + XiXOffset(*kid) + (int) XtWidth(*kid)) )
	{
	    active = *kid;
	    break;
	}
    }

    if( active == NULL || !XiValidInputChild(active) ) return;

    cell_y = -1;
    for( i = 0; i < XmTable_num_rows(tw); ++i )
    {
	if( y < (y_origin + XmYOffset(tw, i) + XmTable_row_height(tw)) )
	{
	    cell_y = i;
	    break;
	}
    }

    if( cell_y == -1 ) return;

    SetActive(tw, active, cell_y, XiVisNone);
}

#if NeedFunctionPrototypes
static void
CalcTableInfo(XmTableWidget tw)
#else
static void
CalcTableInfo(tw)
    XmTableWidget tw;
#endif
{
    Widget           clip = XmTable_clip(tw);
    WidgetList       kid;
    Cardinal         kidCnt, i, validCnt;
    int              rowHeight, tableHeight, tableWidth, titleHeight, y;
    XtWidgetGeometry wanted;

    /*
     * Lets first calculate the height of the title cliping area.
     */
    XtVaGetValues(clip,
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);

    titleHeight = 1;
    rowHeight = tableHeight = tableWidth = 0;
    validCnt = 0;
    for( i = 0; i < kidCnt; ++i, ++kid )
    {
	if( !XiValidChild(*kid) ) continue;

	validCnt++;
	if( XtQueryGeometry(*kid, NULL, &wanted) == XtGeometryNo ||
	    !(wanted.request_mode & CWHeight) )
	{
	    wanted.height = XtHeight(*kid);
	}
	if( (int) wanted.height > rowHeight ) rowHeight = wanted.height;
	tableWidth += XtWidth(*kid);

	if( XtQueryGeometry(XiC(*kid).label, NULL, &wanted) == XtGeometryNo ||
	    !(wanted.request_mode & CWHeight) )
	{
	    wanted.height = XtHeight(XiC(*kid).label);
	}
	if( (int) wanted.height > titleHeight ) titleHeight = wanted.height;
    }
    XmTable_title_height(tw) = titleHeight;

    /*
     * Now that we know the height of each row, lets go and cache away
     * the offsets for the beginning of each row.  Lets start by making
     * that we have enough memory in the cache.
     */
    if( XmTable_num_rows(tw) > XmTable_num_offsets(tw) )
    {
	XmTable_num_offsets(tw) = XmTable_num_rows(tw);
	XmTable_offset(tw) = (int*)
	    XtRealloc((XtPointer) XmTable_offset(tw),
		      sizeof(int) * XmTable_num_offsets(tw));
    }

    y = XmTable_line_width(tw);
    for( i = 0; i < XmTable_num_rows(tw); ++i )
    {
	XmTable_offset(tw)[i] = y;
	y += (rowHeight + XmTable_line_width(tw));
    }

    tableHeight = (rowHeight * XmTable_num_rows(tw)) +
	(XmTable_line_width(tw) * (XmTable_num_rows(tw)+1));
    tableWidth += ((validCnt+1) * XmTable_line_width(tw));

    XmTable_table_height(tw) = tableHeight;
    XmTable_table_width(tw) = tableWidth;
    XmTable_row_height(tw) = rowHeight;
    XmTable_valid_cnt(tw) = validCnt;
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
XiSetActive(XmTableWidget tw, Widget active, int cell_y, int vis,
	    Boolean set_values)
#else
static void
XiSetActive(tw, active, cell_y, vis, set_values)
    XmTableWidget tw;
    Widget        active;
    int           cell_y, vis;
    Boolean       set_values;
#endif
{
    Position               y, height, old_y, old_x_origin, old_y_origin;
    Widget                 old_active;
    XmTableCallbackStruct  cbdata;
    XEvent                 ev;

    old_y_origin = XmTable_y_origin(tw);
    old_x_origin = XmTable_x_origin(tw);

    old_active = XmTable_active_cell(tw);
    old_y = XmTable_cell_y(tw);

    if( old_active != active || old_y != cell_y )
    {
	cbdata.reason = XmCR_CELL_VERIFY_TRAVERSE;
	cbdata.event = NULL;
	cbdata.from_cell = old_active;
	cbdata.from_row = old_y;
	cbdata.to_cell = active;
	cbdata.to_row = cell_y;
	cbdata.confirm = True;
	XtCallCallbackList((Widget) tw, XmTable_cell_traverse_verify_callback(tw),
			(XtPointer) &cbdata);

	if( old_active != NULL && !cbdata.confirm ) return;
    }

    if( old_active != NULL && (old_active != active || old_y != cell_y) )
    {
	String tmp = NULL;

	XtSetMappedWhenManaged(old_active, False);
	if( XtIsRealized(old_active) ) XtUnmapWidget(old_active);
	tmp = XiCallGetValue(old_active);
	XiStoreCellValue(tw, old_active, old_y, tmp);
 	XiCallFree(old_active, tmp);
    }

    if( XmTable_num_rows(tw) == 0 || active == NULL )
    {
	XmTable_active_cell(tw) = NULL;
	XmTable_cell_y(tw) = -1;
	return;
    }

    if( vis != XiVisNone )
    {
	int     x, y, width, height;
	Boolean change = False;

	x = -XmTable_x_origin(tw);
	y = -XmTable_y_origin(tw);
	width = XtWidth(XmTable_clip(tw));
	height = XtHeight(XmTable_clip(tw));

	if( !IsHorizVisible(tw, active) )
	{
	    if( width >= XmTable_table_width(tw) )
	    {
		x = 0;
	    }
	    else
	    {
		Boolean left = False, right = False;

		if( vis & XiVisRight )
		{
		    if( width < (int) XtWidth(active) )
		    {
			left = True;
		    }
		    else
		    {
			right = True;
		    }
		}
		else if( vis & XiVisLeft )
		{
		    left = True;
		}

		if( right )
		{
		    x = width - XtWidth(active) - XmTable_line_width(tw);
		    x = XiXOffset(active) - x;
		}
		else if( left )
		{
		    x = XiXOffset(active) - XmTable_line_width(tw);
		}
	    }
	}

	if( !IsVertVisible(tw, active, cell_y) )
	{
	    if( height >= XmTable_table_height(tw) )
	    {
		y = 0;
	    }
	    else
	    {
		Boolean upper = False, lower = False;

		if( vis & XiVisLower )
		{
		    if( height < (int) XtHeight(active) )
		    {
			upper = True;
		    }
		    else
		    {
			lower = True;
		    }
		}
		else if( vis & XiVisUpper )
		{
		    upper = True;
		}

		if( lower )
		{
		    y = height - XtHeight(active) - XmTable_line_width(tw);
		    y = XmYOffset(tw, cell_y) - y;
		}
		else if( upper )
		{
		    y = XmYOffset(tw, cell_y) - XmTable_line_width(tw);
		}
	    }
	}

	if( x <= 0 )
	{
	    x = 0;
	}
	else if( (x + width) > XmTable_table_width(tw) )
	{
	    x = XmTable_table_width(tw) - width;
	}

	if( y <= 0 )
	{
	    y = 0;
	}
	else if( (y + height) > XmTable_table_height(tw) )
	{
	    y = XmTable_table_height(tw) - height;
	}

	if( XmTable_x_origin(tw) != -x )
	{
	    change = True;
	    XmTable_x_origin(tw) = -x;
	}
	if( XmTable_y_origin(tw) != -y )
	{
	    change = True;
	    XmTable_y_origin(tw) = -y;
	}

	if( change )
	{
	    int tmp, slider, inc, page_inc, max;

	    XtMoveWidget(XmTable_title_backing(tw), XmTable_x_origin(tw), 0);

	    XmScrollBarGetValues(XmTable_hscroll(tw), &tmp, &slider,
				 &inc, &page_inc);
	    XtVaGetValues(XmTable_hscroll(tw),
			  XmNmaximum, &max,
			  NULL);
	    if( (x + slider) > max )
	    {
		x = max - slider;
	    }
	    XmScrollBarSetValues(XmTable_hscroll(tw), x, slider,
				 inc, page_inc, False);

	    XmScrollBarGetValues(XmTable_vscroll(tw), &tmp, &slider,
				 &inc, &page_inc);
	    XtVaGetValues(XmTable_vscroll(tw),
			  XmNmaximum, &max,
			  NULL);
	    if( (y + slider) > max )
	    {
		y = max - slider;
	    }
	    XmScrollBarSetValues(XmTable_vscroll(tw), y, slider,
				 inc, page_inc, False);

	    if( XtIsRealized((Widget)tw) )
	    {
		MoveScrollRegion(tw, old_x_origin, old_y_origin, XmTable_x_origin(tw),
				 XmTable_y_origin(tw));
	    }
	}
    }

    XmTable_active_cell(tw) = active;
    XmTable_cell_y(tw) = cell_y;

    if( active != NULL )
    {
	String tmp;

	height = XmTable_row_height(tw);
	y = XmYOffset(tw, cell_y);

	XtConfigureWidget(active,
			  XiXOffset(active) + XmTable_x_origin(tw),
			  y + XmTable_y_origin(tw), XtWidth(active),
			  height, 0);
	XtSetMappedWhenManaged(active, True);

	/*
	 * We are traversing down the same column.  In this case, any
	 * focus callbacks won't be called, because the widget never
	 * loses focus.  So, we send a FocusOut event followed by a
	 * FocusIn event to fake out the widget.
	 */
	if ((active == old_active) && (old_y != cell_y))
	{
	    ev.xfocus.type = FocusOut;
	    ev.xfocus.display = XtDisplay(active);
	    ev.xfocus.window = XtWindow(active);
	    ev.xfocus.mode = NotifyNormal;
	    ev.xfocus.detail = NotifyDetailNone;
	    XSendEvent(XtDisplay(active), XtWindow(active), False,
		       FocusChangeMask, &ev);


	    ev.xfocus.type = FocusIn;
	    XSendEvent(XtDisplay(active), XtWindow(active), False,
		       FocusChangeMask, &ev);
	}

	XmProcessTraversal(XiCallFocusWidget(active), XmTRAVERSE_CURRENT);

	tmp = XiFetchCellValue(tw, active, cell_y);
	XiCallSetValue(active, tmp);
	XiCallSelectText(active);
	XiFreeCellValue(tw, tmp);

	cbdata.reason = XmCR_CELL_TRAVERSE;
	cbdata.event = NULL;
	cbdata.from_cell = old_active;
	cbdata.from_row = old_y;
	cbdata.to_cell = active;
	cbdata.to_row = cell_y;
	cbdata.confirm = True;
	XtCallCallbackList((Widget) tw, XmTable_cell_traverse_callback(tw),
			(XtPointer) &cbdata);

    }
}

#if NeedFunctionPrototypes
static void
XiCallRender(XmTableWidget tw, Widget child, XRectangle *rect, String value)
#else
static void
XiCallRender(tw, child, rect, value)
    XmTableWidget tw;
    Widget        child;
    XRectangle    *rect;
    String        value;
#endif
{
    Widget clip = XmTable_clip(tw);

    if( !XtIsRealized(clip) || XiC(child).render_proc == NULL ) return;

    XSetClipRectangles(XtDisplay(clip), XmTable_foreground_GC(tw),
		       0, 0, rect, 1, YXBanded);
    (*(XiC(child).render_proc))(clip, child, rect, XmTable_foreground_GC(tw),
				value);
    XSetClipMask(XtDisplay(clip), XmTable_foreground_GC(tw), None);
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
TableVScrollCB(Widget widget, XtPointer client, XtPointer cbdata)
#else
static void
TableVScrollCB(widget, client, cbdata)
    Widget    widget;
    XtPointer client, cbdata;
#endif
{
    XmTableWidget            tw = (XmTableWidget) XtParent(widget);
    XmScrollBarCallbackStruct *scroll = (XmScrollBarCallbackStruct*) cbdata;
    int			      i, y, row, slider_size, max_value, min =0, height,
                              old_x, old_y;

    old_x = XmTable_x_origin(tw);
    old_y = XmTable_y_origin(tw);
    switch( scroll->reason )
    {
    case XmCR_INCREMENT:
	row = -1;
	for( i = 0; i < XmTable_num_rows(tw); ++i )
	{
	    y = XmYOffset(tw, i) + XmTable_y_origin(tw);
	    if( y > (int) XmTable_line_width(tw) && (row == -1 || y < min) )
	    {
		row = i;
		min = y;
	    }
	}

	XtVaGetValues(widget,
		      XmNsliderSize, &slider_size,
		      XmNmaximum, &max_value,
		      NULL);

	if( row == -1 )
	{
	    y = max_value - slider_size;
	    XmTable_y_origin(tw) = -y;
	}
	else
	{
	    y = XmYOffset(tw, row) - XmTable_line_width(tw);
	    if( (y + slider_size) > max_value ) y = max_value - slider_size;
	    XmTable_y_origin(tw) = -y;
	}
	XtVaSetValues(widget,
		      XmNvalue, y,
		      NULL);
	break;
    case XmCR_PAGE_INCREMENT:
	row = -1;
	for( i = 0; row == -1 && i < XmTable_num_rows(tw); ++i )
	{
	    y = XmYOffset(tw, i) + XmTable_y_origin(tw);
	    if( (y + XmTable_row_height(tw)) > (int) XtHeight(XmTable_clip(tw))&&
	        y != XmTable_line_width(tw) )
	    {
		row = i;
	    }
	}

	XtVaGetValues(widget,
		      XmNsliderSize, &slider_size,
		      XmNmaximum, &max_value,
		      NULL);

	if( row == -1 )
	{
	    y = max_value - slider_size;
	    XmTable_y_origin(tw) = -y;
	}
	else
	{
	    y = XmYOffset(tw, row) - XmTable_line_width(tw);
	    if( (y + slider_size) > max_value ) y = max_value - slider_size;
	    XmTable_y_origin(tw) = -y;
	}
	XtVaSetValues(widget,
		      XmNvalue, y,
		      NULL);
	break;
    case XmCR_DECREMENT:
	row = -1;
	for( i = 0; i < XmTable_num_rows(tw); ++i )
	{
	    y = XmYOffset(tw, i) + XmTable_y_origin(tw);
	    if( y < 0 && (row == -1 || y > min) )
	    {
		row = i;
		min = y;
	    }
	}
	if( row == -1 ) return;

	XtVaGetValues(widget,
		      XmNsliderSize, &slider_size,
		      XmNmaximum, &max_value,
		      NULL);
	y = XmYOffset(tw, row) - (int) XmTable_line_width(tw);
	if( (y + slider_size) > max_value ) y = max_value - slider_size;
	XmTable_y_origin(tw) = -y;
	XtVaSetValues(widget,
		      XmNvalue, y,
		      NULL);
	break;
    case XmCR_PAGE_DECREMENT:
	row = -1;
	for( i = XmTable_num_rows(tw)-1; i >= 0; --i )
	{
	    y = XmYOffset(tw, i) + XmTable_y_origin(tw);
	    if( y < 0 )
	    {
		row = i;
		break;
	    }
	}
	if( row == -1 ) return;

	height = 0;
	for( ; i >= 0; --i )
	{
	    if( (int) (height + XmTable_row_height(tw) + XmTable_line_width(tw)) >
	        (int) XtHeight(XmTable_clip(tw)) )
	    {
		break;
	    }
	    height += XmTable_row_height(tw) + XmTable_line_width(tw);
	    row = i;
	}

	XtVaGetValues(widget,
		      XmNsliderSize, &slider_size,
		      XmNmaximum, &max_value,
		      NULL);
	if( row == -1 )
	{
	    y = 0;
	    XmTable_y_origin(tw) = 0;
	}
	else
	{
	    y = XmYOffset(tw, row) - XmTable_line_width(tw);
	    if( (y + slider_size) > max_value ) y = max_value - slider_size;
	    if( y < 0 ) y = 0;
	    XmTable_y_origin(tw) = -y;
	}
	XtVaSetValues(widget,
		      XmNvalue, y,
		      NULL);
	break;
    case XmCR_TO_BOTTOM:
    case XmCR_TO_TOP:
    case XmCR_DRAG:
    default:
	XmTable_y_origin(tw) = -scroll->value;
	break;
    }
    LayoutActive(tw);
    MoveScrollRegion(tw, old_x, old_y, XmTable_x_origin(tw), XmTable_y_origin(tw));
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static void
TableHScrollCB(Widget widget, XtPointer client, XtPointer cbdata)
#else
static void
TableHScrollCB(widget, client, cbdata)
    Widget    widget;
    XtPointer client, cbdata;
#endif
{
    XmTableWidget            tw = (XmTableWidget) XtParent(widget);
    XmScrollBarCallbackStruct *scroll = (XmScrollBarCallbackStruct*) cbdata;
    WidgetList                kid;
    Cardinal                  kidCnt, min = 0;
    Widget		      child;
    int			      i, x, slider_size, max_value, width,
                              old_x, old_y;

    old_x = XmTable_x_origin(tw);
    old_y = XmTable_y_origin(tw);
    XtVaGetValues(XmTable_clip(tw),
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);

    switch( scroll->reason )
    {
    case XmCR_INCREMENT:
	/*
	 * First thing we need to do is see what is the next column
	 * we should align to.  We do this by finding the child closest
	 * to the left side of the clip area.
	 */
	child = NULL;
	for( i = 0; i < kidCnt; ++i )
	{
	    if( !XiValidChild(kid[i]) ) continue;
	    x = XiXOffset(kid[i]) + XmTable_x_origin(tw);
	    if( x > (int) XmTable_line_width(tw) && (child == NULL || x < min) )
	    {
		child = kid[i];
		min = x;
	    }
	}

	/*
	 * Ok we now that we know the child that we are interested in,
	 * lets grab some information from the scrollbar so that we can
	 * calculate where we want to be.
	 */
	XtVaGetValues(widget,
		      XmNsliderSize, &slider_size,
		      XmNmaximum, &max_value,
		      NULL);

	if( child == NULL )
	{
	    /*
	     * If we are not able to find a child then we want to
	     * scroll to the end.
	     */
	    x = max_value - slider_size;
	    XmTable_x_origin(tw) = -x;
	}
	else
	{
	    /*
	     * We have a child so now lets calcuate the x origin to place
	     * this kid at the left side.
	     */
	    x = XiXOffset(child) - (int) XmTable_line_width(tw);
	    if( (x + slider_size) > max_value ) x = max_value - slider_size;
	    XmTable_x_origin(tw) = -x;
	}

	/*
	 * Now that we know the new value for the scrollbar lets set it back
	 * to the scrollbar.
	 */
	XtVaSetValues(widget,
		      XmNvalue, x,
		      NULL);
	break;
    case XmCR_PAGE_INCREMENT:

	/*
	 * Here we want to find the first child that is not entirely on the
	 * page and make that child the left most child.
	 */
	child = NULL;
	for( i = 0; i < kidCnt; ++i )
	{
	    if( !XiValidChild(kid[i]) ) continue;
	    x = XiXOffset(kid[i]) + XmTable_x_origin(tw);
	    if( (x + (int) XtWidth(kid[i])) > (int) XtWidth(XmTable_clip(tw)) &&
	        (x != XmTable_line_width(tw)) &&
	        (child == NULL || x < min) )
	    {
		child = kid[i];
		min = x;
	    }
	}

	XtVaGetValues(widget,
		      XmNsliderSize, &slider_size,
		      XmNmaximum, &max_value,
		      NULL);
	if( child == NULL )
	{
	    x = max_value - slider_size;
	    XmTable_x_origin(tw) = -x;
	}
	else
	{
	    x = XiXOffset(child) - (int) XmTable_line_width(tw);
	    if( (x + slider_size) > max_value ) x = max_value - slider_size;
	    XmTable_x_origin(tw) = -x;
	}
	XtVaSetValues(widget,
		      XmNvalue, x,
		      NULL);
	break;
    case XmCR_DECREMENT:
	child = NULL;
	for( i = 0; i < kidCnt; ++i )
	{
	    if( !XiValidChild(kid[i]) ) continue;
	    x = XiXOffset(kid[i]) + XmTable_x_origin(tw);
	    if( x < 0 && (child == NULL || x > min) )
	    {
		child = kid[i];
		min = x;
	    }
	}
	if( child == NULL ) return;

	XtVaGetValues(widget,
		      XmNsliderSize, &slider_size,
		      XmNmaximum, &max_value,
		      NULL);
	if( child == NULL )
	{
	    x = 0;
	    XmTable_x_origin(tw) = 0;
	}
	else
	{
	    x = XiXOffset(child) - (int) XmTable_line_width(tw);
	    if( (x + slider_size) > max_value ) x = max_value - slider_size;
	    XmTable_x_origin(tw) = -x;
	}
	XtVaSetValues(widget,
		      XmNvalue, x,
		      NULL);
	break;
    case XmCR_PAGE_DECREMENT:

	/*
	 * First thing we want to do is start at the last child and find the
	 * first (last) child with it x off the visible area.
	 */
	child = NULL;
	for( i = kidCnt-1; i >= 0; --i )
	{
	    if( !XiValidChild(kid[i]) ) continue;
	    x = XiXOffset(kid[i]) + XmTable_x_origin(tw);
	    if( x < 0 && child == NULL )
	    {
		child = kid[i];
		break;
	    }
	}
	if( child == NULL ) return;

	/*
	 * Now that we have one child that is going to be on the next page
	 * we want to keep adding children until we have no more room.
	 */
	width = 0;
	for( ; i >= 0; --i )
	{
	    if( !XiValidChild(kid[i]) ) continue;
	    if( (int) (width + XtWidth(kid[i]) + XmTable_line_width(tw)) >
	        (int) XtWidth(XmTable_clip(tw)) )
	    {
		break;
	    }
	    width += XtWidth(kid[i]) + XmTable_line_width(tw);
	    child = kid[i];
	}

	XtVaGetValues(widget,
		      XmNsliderSize, &slider_size,
		      XmNmaximum, &max_value,
		      NULL);
	if( child == NULL )
	{
	    x = 0;
	    XmTable_x_origin(tw) = 0;
	}
	else
	{
	    x = XiXOffset(child) - (int) XmTable_line_width(tw);
	    if( (x + slider_size) > max_value ) x = max_value - slider_size;
	    if( x < 0 ) x = 0;
	    XmTable_x_origin(tw) = -x;
	}
	XtVaSetValues(widget,
		      XmNvalue, x,
		      NULL);
	break;
    case XmCR_TO_BOTTOM:
    case XmCR_TO_TOP:
    case XmCR_DRAG:
    default:
	XmTable_x_origin(tw) = -scroll->value;
	break;
    }
    XtMoveWidget(XmTable_title_backing(tw), XmTable_x_origin(tw), 0);
    LayoutActive(tw);
    MoveScrollRegion(tw, old_x, old_y, XmTable_x_origin(tw), XmTable_y_origin(tw));
}

#ifdef DYNAMIC_COLUMN_SIZING
#if NeedFunctionPrototypes
static void
DrawMoveLine(XmTableWidget tw, int x, Boolean erase)
#else
static void
DrawMoveLine(tw, x, erase)
    XmTableWidget tw;
    int           x;
    Boolean       erase;
#endif
{
    int y, width, height, offset;

    offset = XmTable_margin_width(tw) - XmTable_half_line_width(tw);
    y = XmTable_margin_height(tw);
    height = XtY(XmTable_clip(tw)) + (int) XtHeight(XmTable_clip(tw)) - y;
    width = (XmTable_line_width(tw) == 0 ? 1 : XmTable_line_width(tw));

    if( erase )
    {
	XFillRectangle(XtDisplay(tw), XtWindow(tw), XmTable_move_GC(tw),
		       XmTable_move_cur_x(tw) + offset, y, width, height);
    }

    if( x >= 0 )
    {
	XFillRectangle(XtDisplay(tw), XtWindow(tw), XmTable_move_GC(tw),
		       x + offset, y, width, height);
    }
}
#endif

#if NeedFunctionPrototypes
static void
XiConfigureClip(Widget clip, Position x, Position y, Dimension width,
		Dimension height)
#else
static void
XiConfigureClip(clip, x, y, width, height)
    Widget    clip;
    Position  x, y;
    Dimension width, height;
#endif
{
    XmTableWidget tw = (XmTableWidget) XtParent(clip);

    if( x != XtX(clip) || y != XtY(clip) || width != XtWidth(clip) ||
        height != XtHeight(clip) )
    {
	if( XtIsRealized((Widget)tw) && XmTable_save_size(tw).width != 0 )
	{
	    _XmClearBorder(XtDisplay(tw), XtWindow(tw),
			   XmTable_save_size(tw).x, XmTable_save_size(tw).y,
			   XmTable_save_size(tw).width,
			   XmTable_save_size(tw).height,
			   XmTable_save_shadow_thickness(tw));
	}

	XtConfigureWidget(clip, x, y, width, height, 0);

	if( XtIsRealized((Widget)tw) )
	{
	    Redisplay((Widget) tw, NULL, False);
	}
    }
}


/*
 * Default Interface Procedures
 *
 *     	Below are the default interface procedures that the Table will
 *     	use to talk to known widgets.  Currently there are defaults for
 *	the following widget classes.
 *
 *	o XmTextField
 *	o XmText
 *	o XiComboBox (and no you don't need the library to compile Table)
 */
#if NeedFunctionPrototypes
static Widget
_XmTextFocusWidget(Widget widget)
#else
static Widget
_XmTextFocusWidget(widget)
    Widget widget;
#endif
{
    return( widget );
}

#if NeedFunctionPrototypes
static void
_XiTextSelectText(Widget widget)
#else
static void
_XiTextSelectText(widget)
    Widget widget;
#endif
{
    XmTextPosition last = XmTextGetLastPosition(widget);
    if( last == 0 ) return;

    XmTextSetSelection(widget, 0, last,
		       XtLastTimestampProcessed(XtDisplay(widget)));
}

#if NeedFunctionPrototypes
static void
_XiTextUnselectText(Widget widget)
#else
static void
_XiTextUnselectText(widget)
    Widget widget;
#endif
{
    XmTextClearSelection(widget,
			 XtLastTimestampProcessed(XtDisplay(widget)));
}

#if NeedFunctionPrototypes
static void
_XiTextSetValue(Widget widget, String value)
#else
static void
_XiTextSetValue(widget, value)
    Widget widget;
    String value;
#endif
{
    XmTextSetString(widget, value);
}

#if NeedFunctionPrototypes
static String
_XiTextGetValue(Widget widget)
#else
static String
_XiTextGetValue(widget)
    Widget widget;
#endif
{
    return( XmTextGetString(widget) );
}

#if NeedFunctionPrototypes
static void
_XiTextRender(Widget parent, Widget child, XRectangle *rect, GC gc,
	      String value)
#else
static void
_XiTextRender(parent, child, rect, gc, value)
    Widget     parent, child;
    XRectangle *rect;
    GC         gc;
    String     value;
#endif
{
    XmTextFieldWidget tf = (XmTextFieldWidget) child;
    Dimension         highlight, shadow, marginWidth, marginHeight;
    XmFontList        fontList;
    XmFontContext     context;
    XmFontListEntry   entry;
    XmFontType        typeReturn;
    XFontStruct       *font;
    Boolean	      sensitive, aSensitive;
    int		      length, tmp, marginTop, marginBottom;
    Pixel	      foreground, background;
    Cardinal	      depth;
    XGCValues	      gcvalues;

    if( value == NULL || strlen(value) == 0 ) return;

    XtVaGetValues(child,
		  XmNhighlightThickness, &highlight,
		  XmNshadowThickness, &shadow,
		  XmNmarginWidth, &marginWidth,
		  XmNmarginHeight, &marginHeight,
		  XmNfontList, &fontList,
		  XmNsensitive, &sensitive,
		  XmNancestorSensitive, &aSensitive,
		  XmNforeground, &foreground,
		  XmNbackground, &background,
		  XmNdepth, &depth,
		  NULL);

    sensitive = sensitive && aSensitive;

    marginTop = tf->text.margin_top + shadow + highlight;
    marginBottom = tf->text.margin_bottom + shadow + highlight;

    if( fontList == NULL || !XmFontListInitFontContext(&context, fontList) )
    {
	return;
    }

    rect->x += (highlight + shadow + marginWidth);
    rect->y += marginTop;
    if( (tmp = (int) rect->width - (2*(highlight+shadow+marginWidth))) < 0 )
    {
	return;
    }
    rect->width = tmp;
    if( (tmp = (int) rect->height - (marginTop + marginBottom)) < 0 )
    {
	return;
    }
    rect->height = tmp;

    entry = XmFontListNextEntry(context);
    font = (XFontStruct*) XmFontListEntryGetFont(entry, &typeReturn);
    if( font == NULL ) return;

    if( typeReturn == XmFONT_IS_FONT )
    {
	XSetFont(XtDisplay(child), gc, font->fid);
	length = XTextWidth(font, value, strlen(value));
    }
    else
    {
	XRectangle  ink, all;
	XFontStruct **fonts;
	char        **font_names;
	int         cnt;

	XmbTextExtents((XFontSet)font, value, strlen(value), &ink, &all);
	length = ink.width;

	cnt = XFontsOfFontSet((XFontSet)font, &fonts, &font_names);
	if( cnt == 0 ) return;
	XSetFont(XtDisplay(child), gc, fonts[0]->fid);
    }

    XSetClipRectangles(XtDisplay(child), gc, 0, 0, rect, 1, YXBanded);
    rect->y += TextF_FontAscent(tf);

    if( (length = XTextWidth(font, value, strlen(value))) > (int) rect->width )
    {
	rect->x = (rect->x + rect->width) - length;
    }

    if( !sensitive )
    {
	gcvalues.fill_style = FillTiled;
	gcvalues.tile = XmGetPixmapByDepth(XtScreen(parent),
					   "50_foreground",
					   foreground,
					   background,
					   depth);
	XChangeGC(XtDisplay(parent), gc,
		  GCTile | GCFillStyle,
		  &gcvalues);
    }

    if( typeReturn == XmFONT_IS_FONT )
    {
	XDrawString(XtDisplay(parent), XtWindow(parent), gc,
		    rect->x, rect->y, value, strlen(value));
    }
    else
    {
	XmbDrawString(XtDisplay(parent), XtWindow(parent),
		      (XFontSet)font, gc, rect->x, rect->y, value,
		      strlen(value));
    }

    if( !sensitive )
    {
	gcvalues.fill_style = FillSolid;
	XChangeGC(XtDisplay(parent), gc,
		  GCFillStyle,
		  &gcvalues);
	XmDestroyPixmap(XtScreen(parent), gcvalues.tile);
    }

    XmFontListFreeFontContext(context);
}

#if NeedFunctionPrototypes
static Widget
_XiComboBoxFocusWidget(Widget widget)
#else
static Widget
_XiComboBoxFocusWidget(widget)
    Widget widget;
#endif
{
    return( XtNameToWidget(widget, "*text") );
}

#if NeedFunctionPrototypes
static void
_XiComboBoxSelectText(Widget widget)
#else
static void
_XiComboBoxSelectText(widget)
    Widget widget;
#endif
{
    XmTextPosition last;

    widget = XtNameToWidget(widget, "*text");

    if( (last = XmTextGetLastPosition(widget)) == 0 ) return;

    if( widget == NULL || last == 0 ) return;

    XmTextFieldSetSelection(widget, 0, last,
			    XtLastTimestampProcessed(XtDisplay(widget)));
}

#if NeedFunctionPrototypes
static void
_XiComboBoxUnselectText(Widget widget)
#else
static void
_XiComboBoxUnselectText(widget)
    Widget widget;
#endif
{
    if( (widget = XtNameToWidget(widget, "*text")) == NULL ) return;

    XmTextClearSelection(widget, XtLastTimestampProcessed(XtDisplay(widget)));
}

#if NeedFunctionPrototypes
static void
_XiComboBoxSetValue(Widget widget, String value)
#else
static void
_XiComboBoxSetValue(widget, value)
    Widget widget;
    String value;
#endif
{
    XmTextFieldSetString(XtNameToWidget(widget, "*text"), value);
}

#if NeedFunctionPrototypes
static String
_XiComboBoxGetValue(Widget widget)
#else
static String
_XiComboBoxGetValue(widget)
    Widget widget;
#endif
{
    return( XmTextFieldGetString(XtNameToWidget(widget, "*text")) );
}

#if NeedFunctionPrototypes
static void
_XiComboBoxRender(Widget parent, Widget child, XRectangle *rect, GC gc,
		  String value)
#else
static void
_XiComboBoxRender(parent, child, rect, gc, value)
    Widget     parent, child;
    XRectangle *rect;
    GC         gc;
    String     value;
#endif
{
    Widget   text = XtNameToWidget(child, "*text");
    Position  x, y, saveX, saveY;
    Dimension width, height;

    if( text == NULL || value == NULL || strlen(value) == 0 ) return;

    XtVaGetValues(text,
		  XmNx, &x,
		  XmNy, &y,
		  XmNwidth, &width,
		  XmNheight, &height,
		  NULL);

    saveX = rect->x;
    saveY = rect->y;

    rect->x += x;
    rect->y += y;
    rect->width = (saveX + (x + width)) - rect->x;
    rect->height = (saveY + (y + height)) - rect->y;
    _XiTextRender(parent, text, rect, gc, value);
}

#ifdef DYNAMIC_COLUMN_SIZING
#if NeedFunctionPrototypes
static Widget
GetMoveWidget(XmTableWidget tw, int x)
#else
static Widget
GetMoveWidget(tw, x)
    XmTableWidget tw;
    int           x;
#endif
{
    Widget     clip = XmTable_clip(tw), label, last = NULL;
    WidgetList kid;
    Cardinal   kidCnt, i;

    XtVaGetValues(clip,
		  XmNchildren, &kid,
		  XmNnumChildren, &kidCnt,
		  NULL);

    for( i = 0; i < kidCnt; ++i )
    {
	if( !XiValidChild(kid[i]) ) continue;

	label = XiC(kid[i]).label;

	if( x <= (XtX(label) + DEFAULT_BUFFER_SIZE) )
	{
	    return( last );
	}
	last = kid[i];
    }

    if( last != NULL &&
        x < (int) (XtX(label) + XtWidth(label) + XmTable_line_width(tw)) )
    {
	return( last );
    }


    return( NULL );
}
#endif

#if NeedFunctionPrototypes
static Boolean
IsHorizVisible(XmTableWidget tw, Widget active)
#else
static Boolean
IsHorizVisible(tw, active)
    XmTableWidget tw;
    Widget        active;
#endif
{
    int x, width;

    x = XiXOffset(active) + XmTable_x_origin(tw);
    width = XtWidth(active);


    if( x < 0 || (x + width) > (int) XtWidth(XmTable_clip(tw)) )
    {
	return( False );
    }

    return( True );
}

/* ARGSUSED */
#if NeedFunctionPrototypes
static Boolean
IsVertVisible(XmTableWidget tw, Widget active, int cell_y)
#else
static Boolean
IsVertVisible(tw, active, cell_y)
    XmTableWidget tw;
    Widget        active;
    int           cell_y;
#endif
{
    int y, height;

    y = cell_y * XmTable_row_height(tw) + (cell_y + 1) * XmTable_line_width(tw) +
	XmTable_y_origin(tw);
    height = XmTable_row_height(tw);

    if( y < 0 || (y + height) > (int) XtHeight(XmTable_clip(tw)) )
    {
	return( False );
    }

    return( True );
}

#if NeedFunctionPrototypes
static void
PassDownResources(XmTableWidget tw, ArgList child_list, Cardinal child_cnt,
		  ArgList label_list, Cardinal label_cnt)
#else
static void
PassDownResources(tw, child_list, child_cnt, label_list, label_cnt)
    XmTableWidget tw;
    ArgList  child_list;
    Cardinal child_cnt;
    ArgList  label_list;
    Cardinal label_cnt;
#endif
{
    Cardinal   i, kidCnt;
    WidgetList kid;

    if( (label_cnt + child_cnt) > 0 )
    {
	XtVaGetValues(XmTable_clip(tw),
		      XmNchildren, &kid,
		      XmNnumChildren, &kidCnt,
		      NULL);

	for( i = 0; i < kidCnt; ++i )
	{
	    if( kid[i]->core.being_destroyed ) continue;
	    XtSetValues(kid[i], child_list, child_cnt);
	    XtSetValues(XiC(kid[i]).label, label_list, label_cnt);
	}
    }
}

#if NeedFunctionPrototypes
void
XmTableUpdate(Widget widget)
#else
void
XmTableUpdate(widget)
    Widget widget;
#endif
{
    XmTableWidget tw = (XmTableWidget) widget;

    /*
     * First lets check to make sure that we have a widget class that
     * we can actually deal with and if not lets give a warning and
     * leave.
     */
    if( !XtIsRealized(widget) || !XtIsSubclass(widget, xmTableWidgetClass) )
    {
	return;
    }

    /*
     * Updating the table turns out to be real easy, all we have to do
     * is expose the area that is the table.
     */
    XClearArea(XtDisplay(widget), XtWindow(XmTable_clip(tw)),
	       0, 0, 0, 0, True);
}

#if NeedFunctionPrototypes
void
XmTableGotoCell(Widget table, Widget child, int y)
#else
void
XmTableGotoCell(table, child, y)
    Widget table, child;
    int    y;
#endif
{
    XmTableWidget tw = (XmTableWidget) table;
    int           vis = XiVisNone;

    if( child == NULL ) child = XmTable_active_cell(tw);

    if( y < 0 || y >= XmTable_num_rows(tw) || child == NULL ) return;

    if( y < XmTable_cell_y(tw) )
    {
	vis |= XiVisUpper;
    }
    else if( y > XmTable_cell_y(tw) )
    {
	vis |= XiVisLower;
    }

    SetActive(tw, child, y, vis);
}

#if NeedFunctionPrototypes
static void
MoveScrollRegion(XmTableWidget tw, int x1, int y1, int x2, int y2)
#else
static void
MoveScrollRegion(tw, x1, y1, x2, y2)
    XmTableWidget tw;
    int           x1, y1, x2, y2;
#endif
{
    XRectangle copy, clear;
    XPoint     dest;
    int        tmp, dx, dy;

    /*
     * Here what we want to do is copy as much as the area as we can and
     * then clear the areas that need to be redraw.
     */

    if( XmTable_clip(tw) == NULL || !XtIsRealized(XmTable_clip(tw)) ) return;

    if( x1 != x2 )
    {
	copy.width = copy.height = 0;
    }
    if( x1 < x2 )
    {
	dx = x2 - x1;
	copy.x = 0;
	copy.y = 0;
	copy.width = ((tmp = ((int) XtWidth(XmTable_clip(tw)) - dx)) < 0 ? 0 : tmp);
	copy.height = XtHeight(XmTable_clip(tw));
	dest.x = dx;
	dest.y = 0;

	clear.x = 0;
	clear.y = 0;
	clear.width = dx;
	clear.height = XtHeight(XmTable_clip(tw));

	SmScrollAdd(XmTable_scroll_mgr(tw), dest.x - copy.x, 0);
	XCopyArea(XtDisplay(tw), XtWindow(XmTable_clip(tw)), XtWindow(XmTable_clip(tw)),
		  XmTable_table_GC(tw),
		  copy.x, copy.y, copy.width, copy.height, dest.x, dest.y);
	XClearArea(XtDisplay(tw), XtWindow(XmTable_clip(tw)),
		   clear.x, clear.y, clear.width, clear.height, True);
    }
    else
    {
	dx = x1 - x2;
	copy.x = dx;
	copy.y = 0;
	copy.width = ((tmp = ((int) XtWidth(XmTable_clip(tw)) - dx)) < 0 ? 0 : tmp);
	copy.height = XtHeight(XmTable_clip(tw));
	dest.x = 0;
	dest.y = 0;

	clear.x = (int) XtWidth(XmTable_clip(tw)) - dx;
	clear.y = 0;
	clear.width = dx;
	clear.height = XtHeight(XmTable_clip(tw));

	SmScrollAdd(XmTable_scroll_mgr(tw), dest.x - copy.x, 0);
	XCopyArea(XtDisplay(tw), XtWindow(XmTable_clip(tw)), XtWindow(XmTable_clip(tw)),
		  XmTable_table_GC(tw),
		  copy.x, copy.y, copy.width, copy.height, dest.x, dest.y);
	XClearArea(XtDisplay(tw), XtWindow(XmTable_clip(tw)),
		   clear.x, clear.y, clear.width, clear.height, True);
    }

    if( y1 != y2 )
    {
	if( y1 < y2 )
	{
	    dy = y2 - y1;
	    copy.x = 0;
	    copy.y = 0;
	    copy.width = XtWidth(XmTable_clip(tw));
	    copy.height = ((tmp = ((int) XtHeight(XmTable_clip(tw)) - dy)) < 0
			   ? 0 : tmp);
	    dest.x = 0;
	    dest.y = dy;

	    clear.x = 0;
	    clear.y = 0;
	    clear.width = XtWidth(XmTable_clip(tw));
	    clear.height = dy;

	    SmScrollAdd(XmTable_scroll_mgr(tw), 0, dest.y - copy.y);
	    XCopyArea(XtDisplay(tw), XtWindow(XmTable_clip(tw)),
		      XtWindow(XmTable_clip(tw)), XmTable_table_GC(tw),
		      copy.x, copy.y, copy.width, copy.height, dest.x, dest.y);
	    XClearArea(XtDisplay(tw), XtWindow(XmTable_clip(tw)),
		       clear.x, clear.y, clear.width, clear.height, True);
	}
	else
	{
	    dy = y1 - y2;
	    copy.x = 0;
	    copy.y = dy;
	    copy.width = XtWidth(XmTable_clip(tw));
	    copy.height = ((tmp = ((int) XtHeight(XmTable_clip(tw)) - dy)) < 0
			   ? 0 : tmp);
	    dest.x = 0;
	    dest.y = 0;

	    clear.x = 0;
	    clear.y = (int) XtHeight(XmTable_clip(tw)) - dy;
	    clear.width = XtWidth(XmTable_clip(tw));
	    clear.height = dy;

	    SmScrollAdd(XmTable_scroll_mgr(tw), 0, dest.y - copy.y);
	    XCopyArea(XtDisplay(tw), XtWindow(XmTable_clip(tw)),
		      XtWindow(XmTable_clip(tw)), XmTable_table_GC(tw),
		      copy.x, copy.y, copy.width, copy.height, dest.x, dest.y);
	    XClearArea(XtDisplay(tw), XtWindow(XmTable_clip(tw)),
		       clear.x, clear.y, clear.width, clear.height, True);
	}
    }
}

#if NeedFunctionPrototypes
static SmScrollMgr
SmScrollMgrCreate(void)
#else
static SmScrollMgr
SmScrollMgrCreate()
#endif
{
    SmScrollMgr mgr = XtNew(SmScrollMgrRec);

    mgr->offset_x = 0;
    mgr->offset_y = 0;
    mgr->scroll_count = 0;
    mgr->scroll_queue = NULL;
    mgr->scrolling = False;

    return( mgr );
}

#if NeedFunctionPrototypes
static void
SmScrollMgrDestroy(SmScrollMgr mgr)
#else
static void
SmScrollMgrDestroy(mgr)
    SmScrollMgr mgr;
#endif
{
    if( mgr->scroll_queue != NULL )
    {
	SmScrollNode tmp, node = mgr->scroll_queue->next;

	while( node != mgr->scroll_queue )
	{
	    tmp = node;
	    node = node->next;
	    XtFree((XtPointer) tmp);
	}
	XtFree((XtPointer) node);
    }
    XtFree((XtPointer) mgr);
}

#if NeedFunctionPrototypes
static void
SmScrollAdd(SmScrollMgr mgr, int dx, int dy)
#else
static void
SmScrollAdd(mgr, dx, dy)
    SmScrollMgr mgr;
    int         dx, dy;
#endif
{
    SmScrollNode node = XtNew(SmScrollNodeRec);

    node->x = dx;
    node->y = dy;

    mgr->offset_x += dx;
    mgr->offset_y += dy;
    mgr->scroll_count++;

    if( mgr->scroll_queue == NULL )
    {
	mgr->scroll_queue = node;
	node->next = node;
	node->prev = node;
    }
    else
    {
	SmScrollNode last = mgr->scroll_queue->prev;

	last->next = node;
	node->next = mgr->scroll_queue;
	node->prev = last;
	mgr->scroll_queue->prev = node;
    }
}

#if NeedFunctionPrototypes
static void
SmScrollRemove(SmScrollMgr mgr)
#else
static void
SmScrollRemove(mgr)
    SmScrollMgr mgr;
#endif
{
    if( mgr->scroll_count > 0 )
    {
	SmScrollNode node = mgr->scroll_queue;

	mgr->offset_x -= node->x;
	mgr->offset_y -= node->y;

	if( node->next == node )
	{
	    mgr->scroll_queue = NULL;
	}
	else
	{
	    mgr->scroll_queue = node->next;
	    node->next->prev = node->prev;
	    node->prev->next = node->next;
	}
	XtFree((XtPointer) node);

	mgr->scroll_count--;
    }
}

#if NeedFunctionPrototypes
static void
SmScrollEvent(SmScrollMgr mgr, XEvent *event)
#else
static void
SmScrollEvent(mgr, event)
    SmScrollMgr mgr;
    XEvent      *event;
#endif
{
    switch( event->type )
    {
    case Expose:
	event->xexpose.x += mgr->offset_x;
	event->xexpose.y += mgr->offset_y;
	break;
    case GraphicsExpose:
	if (mgr->scrolling == False)
	{
	    SmScrollRemove(mgr);
	    if( event->xgraphicsexpose.count != 0 ) mgr->scrolling = True;
	}
	else if (event->xgraphicsexpose.count == 0)
	{
	    mgr->scrolling = False;
	}
	event->xgraphicsexpose.x += mgr->offset_x;
	event->xgraphicsexpose.y += mgr->offset_y;
	break;
    case NoExpose:
	SmScrollRemove(mgr);
	mgr->scrolling = False;
	break;

    default:
	break;
    }
}

/*
 * Function:
 *	XmCreateTable(parent, name, arg_list, arg_cnt);
 * Description:
 *	Creates an unmanaged instance of an XiDBTable and returns its
 *	widget id.
 * Input:
 *	parent   : Widget   - the parent of the new instance.
 *	name     : String   - the name of the new instance.
 *	arg_list : ArgList  - the arguments to create the instance with.
 *	arg_cnt  : Cardinal - the number of arguments in the list
 *
 * Output:
 *	Widget - the widget id of the new instance.
 */
Widget
XmCreateTable(Widget parent, String name, ArgList arg_list,
	      Cardinal arg_cnt)
{
    return( XtCreateWidget(name, xmTableWidgetClass, parent, arg_list,
			   arg_cnt) );
}

/*
 * Function:
 *	XiValidInputChild(widget)
 * Description:
 *	This function determines if a child is valid to traverse to, and
 *	returns a Boolean indicating the result.
 * Input:
 *	widget : Widget - the widget to check
 * Output:
 *	Boolean - True if you can traverse to this child, else False.
 */
static Boolean
XiValidInputChild(Widget widget)
{
    Boolean traversalOn,
            sensitive = True,
            aSensitive = True;

    /*
     * First lets do the quick check.  Because if this widget is not
     * a valid child then it certainly cannot be a valid input
     * child.
     */
    if( !XiValidChild(widget) ) return( False );

    if( (traversalOn = XiEditable(widget)) )
    {
	XtVaGetValues(widget,
		      XmNtraversalOn, &traversalOn,
		      XmNsensitive, &sensitive,
		      XmNancestorSensitive, &aSensitive,
		      NULL);

	traversalOn = (traversalOn && sensitive && aSensitive);
    }

    return( traversalOn );
}

#ifdef __cplusplus
}
#endif

