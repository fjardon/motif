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
/* (c) Copyright 1990, Massachusetts Institute of Technology */
/*   Author:  Jim Fulton, MIT X Consortium */
/* (c) Copyright 1992, Integrated Computer Solutions, Inc. */
/*   Author:  Chris D. Peterson */

#ifndef _XmReportsI_h
#define _XmReportsI_h

#ifdef __cplusplus
extern "C" {
#endif

/*
 * XmScrollReport - this structure is used by the reportCallback of the
 * various widgets to report its position.
 * All fields must be filled in, although the changed field may be used as
 * a hint as to which fields have been altered since the last report.
 */

typedef struct _XmScrollReport {
    unsigned int changed;		/* mask, see below */
    Position slider_x, slider_y;	/* location of slider within outer */
    Dimension slider_width, slider_height;  /* size of slider */
    Dimension canvas_width, canvas_height;  /* size of canvas */
} XmScrollReport;

#define XmPRSliderX		(1 << 0)
#define XmPRSliderY		(1 << 1)
#define XmPRSliderWidth		(1 << 2)
#define XmPRSliderHeight	(1 << 3)
#define XmPRCanvasWidth		(1 << 4)
#define XmPRCanvasHeight	(1 << 5)

#ifdef __cplusplus
}	/* Closes scope of 'extern "C"' declaration */
#endif

#endif /* _XmReportsI_h */
