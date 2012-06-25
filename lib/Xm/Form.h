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
/*   $XConsortium: Form.h /main/12 1995/07/14 10:34:46 drk $ */
/*
*  (c) Copyright 1989, DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASS. */
/*
*  (c) Copyright 1987, 1988, 1989, 1990, 1991, 1992 HEWLETT-PACKARD COMPANY */
#ifndef _XmForm_h
#define _XmForm_h


#include <Xm/BulletinB.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  Form Widget  */

externalref WidgetClass xmFormWidgetClass;

typedef struct _XmFormClassRec * XmFormWidgetClass;
typedef struct _XmFormRec      * XmFormWidget;


/* ifndef for Fast Subclassing  */

#ifndef XmIsForm
#define XmIsForm(w)	XtIsSubclass(w, xmFormWidgetClass)
#endif  /* XmIsForm */

/********    Public Function Declarations    ********/

extern Widget XmCreateForm( 
                        Widget parent,
                        char *name,
                        ArgList arglist,
                        Cardinal argcount) ;
extern Widget XmCreateFormDialog( 
                        Widget parent,
                        char *name,
                        ArgList arglist,
                        Cardinal argcount) ;
extern Widget XmVaCreateForm(
                        Widget parent,
                        char *name,
                        ...);
extern Widget XmVaCreateManagedForm(
                        Widget parent,
                        char *name,
                        ...);

/********    End Public Function Declarations    ********/

#ifdef __cplusplus
}  /* Close scope of 'extern "C"' declaration which encloses file. */
#endif

#endif /* _XmForm_h */
/* DON'T ADD ANYTHING AFTER THIS #endif */
