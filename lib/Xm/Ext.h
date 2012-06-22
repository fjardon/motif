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

#ifndef __Ext_h__
#define __Ext_h__

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

#ifdef XmRENAME_WIDGETS
#include <Xm/BX.h>
#endif

/************************************************************
*	INCLUDE FILES
*************************************************************/

#include <Xm/Xm.h>
#include <Xm/XmStrDefsI.h>

#if defined(__cplusplus)
extern "C" {
#endif


#if NeedFunctionPrototypes
typedef Widget (*XmWidgetFunc)(Widget);
#else
typedef Widget (*XmWidgetFunc)();
#endif

#ifdef VMS
#ifndef bzero
#define bzero(b,len) memset(b, 0, (size_t)(len))
#endif
#ifndef bcopy
#define bcopy(b1,b2,len) memmove(b2, b1, (size_t)(len))
#endif
#endif

/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

/*
 * General name definitions.
 */

/*
 * Ics Widget set ERROR identifiers.
 */

#define XiNbadActionParameters "badActionParameters"
#define XiNbadActionParametersMsg \
    "Bad parameters passed to an action routine of widget '%s'."

#define XiNbadMotionParams "badMotionParams"
#define XiNbadMotionParamsMsg \
"%s - Motion Action : must have exactly 1 parameter, either '%s' or '%s'."

#define XiNbadRowPixmap "badRowPixmap"
#define XiNbadRowPixmapMsg "Extended List: Row pixmaps must have depth of 1."

#define XmNbadXlfdFont "badXlfdFont"
#define XmNbadXlfdFontMsg \
"%s: All Xlfd fonts must contain 14 hyphens\n'%s' is not valid."

#define XiNcellNotEmpty "cellNotEmpty"
#define XmNcellNotEmptyMsg "XmIconBox: Cell %s is not empty"

#define XmNcolorNameTooLong "colorNameTooLong"
#define XmNcolorNameTooLongMsg \
  "%s: Color name '%s' is too long, truncated to '%s'."

#define XiNcontextSaveFailed "contextSaveFailed"
#define XiNcontextSaveFailedMsg "Internal Error: Could not save context data."

#define XiNconversionFailure "conversionFailure"
#define XiNconversionFailureMsg \
	"%s: Unable to perform string to %s conversion."

#define XmNcouldNotFindFamilyData "couldNotFindFamilyData"
#define XmNcouldNotFindFamilyDataMsg \
  "%s: Could not find family data for family '%s'."

#define XiNforceGreaterThanZero "forceGreaterThanZero"
#define XiNforceGreaterThanZeroMsg \
"%s : %s must be greater than zero being reset to one (1)."
    
#define XiNinsertBeforeNotSibling "insertBeforeNotSibling"
#define XiNinsertBeforeNotSiblingMsg "XmHierarchy: InsertBefore Widget\
 is not a sibling of '%s'.\nInserting child at end of list."
    
#define XiNnoComboShell "noComboShell"
#define XiNnoComboShellMsg \
    "Combination Box: When using a custom combo box a shell must be provided."

#define XiNnoEmptyCells "noEmptyCells"
#define XmNnoEmptyCellsMsg "XmIconBox: Could not find any empty cells."

#define XiNnoGadgetSupport "noGadgetSupport"
#define XiNnoGadgetSupportMsg "Widget %s does not support gadget children."
    
#define XiNpixEditBadImageCreate "pixEditBadImageCreate"
#define XiNpixEditBadImageCreateMsg "Pixmap Editor: Can't allocate image data"

#define XiNsameAsImageOrPix "sameAsImageOrPix"
#define XiNsameAsImageOrPixMsg \
"%s : The bitmapMode resource can only be changed at the same time as the pixmap or image"

#define XiNselfOrOutsideOfApplicationDrop "selfOrOutsideOfApplicationDrop"
#define XiNselfOrOutsideOfApplicationDropMsg \
    "Attempt to drop into illegal object."

#define XiNstaticResource "staticResource"
#define XiNstaticResourceMsg \
    "The resource '%s' may not be changed dynamically."

#define XiNtextVerifyFailed "textVerifyFailed"
#define XiNtextVerifyFailedMsg "Combination Box: Text item validation failed."

#define XiNunexpectedEvent "unexpectedEvent"
#define XiNunexpectedEventMsg "%s: Unexpected Event Type %s.\n"

#define XmNunparsableColor "unparsableColor"
#define XmNunparsableColorMsg \
  "%s: Could not parse the color name '%s'."

#define XiNnodeParentIsSelf "nodeParentIsSelf"
#define XiNnodeParentIsSelfMsg \
  "%s: The node parent cannot be self referential."

#define XiNstringGetFailed "stringGetFailed"
#define XiNstringGetFailedMsg "%s: XmGetStringLToR Failed."

#define XmCICSWidgetSetError "ICSWidgetSetError"

/************************************************************
*	MACROS
*************************************************************/

/************************************************************
*	GLOBAL DECLARATIONS
*************************************************************/

/************************************************************
*	EXTERNAL DECLARATIONS
*************************************************************/

/************************************************************
*	CALLBACK REASONS
*************************************************************/
enum {
    XiCR_NONE = 200,
    XiCR_UPDATE_SHELL,
    XiCR_UPDATE_TEXT,
    XiCR_VERIFY_TEXT,
    XiCR_VERIFY_TEXT_FAILED,
    XiCR_ENTER_CHILD,
    XiCR_LEAVE_CHILD
};
	  

void XmCopyISOLatin1Lowered(
#ifndef _NO_PROTO
char *, char *
#endif
);

int XmCompareISOLatin1(
#ifndef _NO_PROTO
char*, char*
#endif
);

Boolean XmCompareXtWidgetGeometryToWidget(
#ifndef _NO_PROTO
XtWidgetGeometry*, Widget
#endif
);

Boolean XmCompareXtWidgetGeometry(
#ifndef _NO_PROTO
XtWidgetGeometry*, XtWidgetGeometry*
#endif
);


#if defined(__cplusplus)
}
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* __Ext_h__ */
