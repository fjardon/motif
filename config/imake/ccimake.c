/* $TOG: ccimake.c /main/16 1998/02/06 11:02:20 kaleb $ */
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
 * Warning:  This file must be kept as simple as posible so that it can 
 * compile without any special flags on all systems.  Do not touch it unless
 * you *really* know what you're doing.  Make changes in imakemdep.h, not here.
 */

#define CCIMAKE			/* only get imake_ccflags definitions */
#include "imakemdep.h"		/* things to set when porting imake */

#ifndef imake_ccflags
#define imake_ccflags "-O"
#endif

main()
{
	write(1, imake_ccflags, sizeof(imake_ccflags) - 1);
	exit(0);
}

