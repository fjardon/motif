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
/*   $XConsortium: DragOverS.h /main/9 1995/07/14 10:26:11 drk $ */
/*
*  (c) Copyright 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */
#ifndef _XmDragOverS_h
#define _XmDragOverS_h

#include <Xm/Xm.h>
#include <Xm/DragC.h>
#include <Xm/DragIcon.h>

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
 *
 * DragOverShell Widget
 *
 ***********************************************************************/

/* Class record constants */

typedef struct _XmDragOverShellRec 	*XmDragOverShellWidget;
typedef struct _XmDragOverShellClassRec 	*XmDragOverShellWidgetClass;

externalref WidgetClass	xmDragOverShellWidgetClass;

#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif /* _XmDragOverS_h */
/* DON'T ADD STUFF AFTER THIS #endif */

