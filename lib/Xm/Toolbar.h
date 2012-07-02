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

#ifndef _XmToolbar_h
#define _XmToolbar_h

#include <Xm/Xm.h>

/************************************************************
*	INCLUDE FILES
*************************************************************/

/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct _XmToolbarClassRec	*XmToolbarWidgetClass;
typedef struct _XmToolbarRec		*XmToolbarWidget;

typedef struct _XmToolbarCallbackStruct
{
  int    reason;	/* the reason for the callback */
  XEvent *event;        /* the event struct */
  Widget toolbar_item;  /* toolbar item for which the event occured */
  int position;         /* position of item */
  int group;            /* group number of item */
} XmToolbarCallbackStruct, *XmToolbarCallbackPtr;

/************************************************************
*	MACROS
*************************************************************/

/************************************************************
*	GLOBAL DECLARATIONS
*************************************************************/

/************************************************************
*	EXTERNAL DECLARATIONS
*************************************************************/

extern WidgetClass xmToolbarWidgetClass; 

/************************************************************
*	EXTERNAL FUNCTIONS
*************************************************************/

extern Widget XmCreateToolbar(
#ifndef _NO_PROTO
Widget, String, ArgList, Cardinal
#endif
);

extern void XmToolbarMapGroup( 
#ifndef _NO_PROTO
Widget , unsigned char  
#endif
);

extern void XmToolbarUnmapGroup( 
#ifndef _NO_PROTO
Widget , unsigned char  
#endif
);


extern void XmToolbarManageGroup( 
#ifndef _NO_PROTO
Widget , unsigned char  
#endif
);


extern void XmToolbarUnmanageGroup( 
#ifndef _NO_PROTO
Widget , unsigned char  
#endif
);


extern void XmToolbarDestroyGroup( 
#ifndef _NO_PROTO
Widget , unsigned char  
#endif
);

extern void XmToolbarInsertBefore(
#ifndef _NO_PROTO
Widget, Widget
#endif
);

#if defined(__cplusplus)
}
#endif

#endif /* _XmToolbar_h */
