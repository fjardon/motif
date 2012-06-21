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

#ifndef ButtonBoxP_h
#define ButtonBoxP_h

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

/************************************************************
 *      INCLUDE FILES
 ************************************************************/

#include <Xm/ManagerP.h>
#include <Xm/ButtonBox.h>

/************************************************************
 *      TYPEDEFS AND DEFINES
 ************************************************************/

/************************************************************
 *      MACROS
 ************************************************************/

#define XmButtonBoxIndex (XmManagerIndex + 1)

extern XmOffsetPtr XmButtonBox_offsets;
extern XmOffsetPtr XmButtonBoxC_offsets;

#define BBoxField(w,f,t) XmField(w, XmButtonBox_offsets, XmButtonBox, f, t)
#define XmButtonBox_equal_size(w) BBoxField(w, equal_size, Boolean)
#define XmButtonBox_fill_option(w) BBoxField(w, fill_option, XmFillOption)
#define XmButtonBox_margin_width(w) BBoxField(w, margin_width, Dimension)
#define XmButtonBox_margin_height(w) BBoxField(w, margin_height, Dimension)
#define XmButtonBox_spacing(w) BBoxField(w, spacing, Dimension)
#define XmButtonBox_orientation(w) BBoxField(w, orientation, unsigned char)

#define BBoxCField(w,f,t) XmConstraintField(w, XmButtonBoxC_offsets, XmButtonBox, f, t)
#define XmButtonBoxC_pref_width(w) BBoxCField(w, pref_width, Dimension)
#define XmButtonBoxC_pref_height(w) BBoxCField(w, pref_height, Dimension)

/************************************************************
 *      GLOBAL DECLARATIONS
 ************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _XmButtonBoxClassPart
{
    XtPointer extension;		/* In case its needed later */
} XmButtonBoxClassPart;

typedef struct _XmButtonBoxClassRec
{
    CoreClassPart       	core_class;
    CompositeClassPart  	composite_class;
    ConstraintClassPart         constraint_class;
    XmManagerClassPart          manager_class;
    XmButtonBoxClassPart      	buttonbox_class;

} XmButtonBoxClassRec;

typedef struct _XmBBoxConstraintsPart {
    Dimension pref_width, pref_height;
} XmBBoxConstraintsPart;
    
typedef struct _XmBBoxConstraintsRec {
    XmManagerConstraintPart	manager;
    XmBBoxConstraintsPart	bbox;
} XmBBoxConstraintsRec, *XmBBoxConstraints;

/*
 * Match XmOffset nomenclature
 */
typedef XmBBoxConstraintsPart XmButtonBoxConstraintPart;

typedef struct
{
    /* resources */

    Boolean		equal_size;
    XmFillOption	fill_option;
    Dimension		margin_width, margin_height;
    Dimension		spacing;
    unsigned char	orientation;

} XmButtonBoxPart;

typedef struct _XmButtonBoxRec
{
    CorePart        	core;
    CompositePart   	composite;
    ConstraintPart      constraint;
    XmManagerPart       manager;
    XmButtonBoxPart     button_box;

}  XmButtonBoxRec;

/************************************************************
 *       EXTERNAL DECLARATIONS
 ************************************************************/

extern XmButtonBoxClassRec xmButtonBoxClassRec;

/************************************************************
 *       STATIC DECLARATIONS
 ************************************************************/

#ifdef __cplusplus
}	/* Closes scope of 'extern "C"' declaration */
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* _XmButtonBoxP_h */
