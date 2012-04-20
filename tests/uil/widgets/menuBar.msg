! 
! Motif
!
! Copyright (c) 1987-2012, The Open Group. All rights reserved.
!
! These libraries and programs are free software; you can
! redistribute them and/or modify them under the terms of the GNU
! Lesser General Public License as published by the Free Software
! Foundation; either version 2 of the License, or (at your option)
! any later version.
!
! These libraries and programs are distributed in the hope that
! they will be useful, but WITHOUT ANY WARRANTY; without even the
! implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
! PURPOSE. See the GNU Lesser General Public License for more
! details.
!
! You should have received a copy of the GNU Lesser General Public
! License along with these librararies and programs; if not, write
! to the Free Software Foundation, Inc., 51 Franklin Street, Fifth
! Floor, Boston, MA 02110-1301 USA
! 
! 
! HISTORY
! 
!   $XConsortium: menuBar.msg /main/7 1995/07/13 20:48:19 drk $
!OSF/Motif: @(#)menuBar.msg	3.2  92/01/31
!
!  
!  	THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED
!  AND COPIED ONLY IN ACCORDANCE WITH THE TERMS OF SUCH LICENSE AND
!  WITH THE INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR
!  ANY OTHER COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE
!  AVAILABLE TO ANY OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF THE
!  SOFTWARE IS HEREBY TRANSFERRED.
!  
!  	THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT
!  NOTICE AND SHOULD NOT BE CONSTRUED AS A COMMITMENT BY OPEN SOFTWARE
!  
!  
!  
!  
!  
!  
!  
!  
!  
! DEC is a registered trademark of Digital Equipment Corporation
! DIGITAL is a registered trademark of Digital Equipment Corporation
!
 value
        message : "This is a menuBar with the following\ncharacteristics:\n\n   - background is green\n" &
	"   - borderWidth is 50\n   - borderColor is mediumBlue\n" &
	"   - width is 200\n   - height is 100\n" &
	"   - shadowThickness is 5\n   - bottomshadowColor is navy\n" &
	"   - entryAlignment is\n     XmALIGNMENT_CENTER\n" &
	"   - packing is XmPACK_COLUMN\n   - marginWidth, marginHeight are 10\n" &
	"   - valueChangedcallback is ON\n     for toggleButtons\n" &
	"   - armCallback is ON for\n     pushbuttons\n" &
!	"   - background of TearOffButtons in\n" &
!	"     both Pulldowns should be wheat\n" &
	"   - tearOffModel is XmTEAR_OFF_ENABLED\n" &
	"     for both Pulldown Menus\n" &
	"   - tearOffMenuActivateCallback and\n" &
	"     tearOffMenuDeactivateCallback are\n" &
	"     ON for both Pulldown Menus\n\n" &
	"Automatically created widget resources:\n\n" &
	"   - background of PulldownMenu 1's tearOff\n" &
	"     control is wheat\n" &
	"   - background of PulldownMenu 2's tearOff\n" &
	"     control is red\n" &
	"   - tearOff control has 2 pixel shadow\n" &
	"   - tearOff control has separatorType of\n" &
	"     XmDOUBLE_DASHED_LINE\n\n" &
	"All other resources are set by default.\n";
