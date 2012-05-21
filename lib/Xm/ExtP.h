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

#ifndef __ExtP_h__
#define __ExtP_h__

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

#include <Xm/Ext.h>

/************************************************************
*	INCLUDE FILES
*************************************************************/

#if defined(hpux) && OS_MAJOR_VERSION < 10
#include <nl_ctype.h>
#endif

/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (*XmVoidFunc)(void);
typedef int (*XmIntFunc)(void);
typedef unsigned int (*XmUnsignedIntFunc)(void);

extern String xm_std_filter[], xm_std_constraint_filter[];

/************************************************************
*	MACROS
*************************************************************/

#define streq(a, b) (((a) != NULL) && ((b) != NULL) && (strcmp((a), (b)) == 0))

#define ForAllChildren(w, childP)                                          \
  for ( (childP) = (w)->composite.children ;                               \
        (childP) < (w)->composite.children + (w)->composite.num_children ; \
        (childP)++ )

/*
 * Math Stuff 
 *
 * Some Systems define MIN and MAX so I have to undef them before I make
 * my own definitions.
 */

#undef MIN			
#undef MAX			
#undef ABS

#define MIN(a,b)        (((int)(a) < (int)(b)) ? (a) : (b))
#define MAX(a,b)        (((int)(a) > (int)(b)) ? (a) : (b))
#define ABS(a)          (((int)(a) >= 0) ? (a) : -(a))

#define ASSIGN_MAX(a, b) ((a) = ((int)(a) > (int)(b) ? (a) : (b)))
#define ASSIGN_MIN(a, b) ((a) = ((int)(a) < (int)(b) ? (a) : (b)))

#define XM_ICON_BUTTON_CLASS_NAME ("XmIconButton")
#define XM_EXT_LIST_CLASS_NAME ("XmExtendedList")
#define XM_ILIST_CLASS_NAME ("XmIList")
#define XM_EXT_18_LIST_CLASS_NAME ("XmExtended18List")
#define XM_I18LIST_CLASS_NAME ("XmI18List")


/************************************************************
*	GLOBAL DECLARATIONS
*************************************************************/

    /*
     * Be sure to add new definitions for BX when adding new functions
     * so that we do no conflict with the versions of the widgets built
     * for BX.
     */

#ifdef _NO_PROTO
    extern void XmResolveAllPartOffsets64();
    extern XtGeometryResult _XmRequestNewSize(), _XmHWQuery();
    extern void _XiGetFocus(), _XmFilterArgs(), _XmSetValuesOnChildren(); 
    extern void _XmMoveWidget(), _XmResizeWidget(), _XiConfigureObject();
    extern Boolean _XmGadgetWarning();
    extern String _XmGetMBStringFromXmString();

    /*
     * Context Managment Routines.
     */
    
    extern Boolean _XmGetContextData(), _XmUtilIsSubclassByNameQ();
    extern void _XmSetContextData(), _XmDeleteContextData();
    extern void _XmInitialIzeConverters();

    extern void _XmExtHighlightBorder(Widget);
    extern void _XmExtUnhighlightBorder(Widget);
#else
    extern void XmResolveAllPartOffsets64(WidgetClass,
					  XmOffsetPtr*, XmOffsetPtr*);
    extern void _XmMoveWidget(Widget, Position, Position);
    extern void _XmResizeWidget(Widget, Dimension, Dimension, Dimension);
    extern void _XmConfigureWidget(Widget, Position, Position, 
			    Dimension, Dimension, Dimension);

    extern XtGeometryResult _XmRequestNewSize(Widget, Boolean, Dimension,
					      Dimension,
					      Dimension *, Dimension *);
    
    extern XtGeometryResult _XmHWQuery(Widget, XtWidgetGeometry*,
				       XtWidgetGeometry *);
    
    extern void _XiGetFocus(Widget, XEvent *, String *, Cardinal *);
    
    extern void _XmFilterArgs(ArgList, Cardinal, String *,
			      ArgList *, Cardinal *);

    extern void _XmSetValuesOnChildren(Widget, ArgList, Cardinal);
    
    extern Boolean _XmGadgetWarning(Widget);

    extern String _XmGetMBStringFromXmString(XmString);

    /*
     * Context Managment Routines.
     */
    
    extern void _XmSetContextData(Widget, XContext, XtPointer);
    extern void _XmDeleteContextData(Widget, XContext);
    extern Boolean _XmGetContextData(Widget, XContext, XtPointer *);
    extern Boolean _XmUtilIsSubclassByNameQ(Widget, XrmQuark);
    extern void _XmInitialIzeConverters(Widget);

    extern void _XmExtHighlightBorder(Widget);
    extern void _XmExtUnhighlightBorder(Widget);
#endif /* _NO_PROTO */

#define _XmInitialiIzeConverters(w) _XmInitializeConverters((w), "ICS EnhancementPak 3.0")

/************************************************************
*	EXTERNAL DECLARATIONS
*************************************************************/

#if defined(__cplusplus)
}
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* __ExtP_h__ */
