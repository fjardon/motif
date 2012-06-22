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

#ifndef _XmExt_h_
#define _XmExt_h_


/************************************************************
*	INCLUDE FILES
*************************************************************/

#include <Xm/Xm.h>
#include <Xm/XmStrDefsI.h>

#if defined(__cplusplus)
extern "C" {
#endif



typedef Widget (*XmWidgetFunc)(Widget);


/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

/*
 * General name definitions.
 */


#define XmNbadActionParameters "badActionParameters"
#define XmNbadActionParametersMsg \
    "Bad parameters passed to an action routine of widget '%s'."

#define XmNbadMotionParams "badMotionParams"
#define XmNbadMotionParamsMsg \
"%s - Motion Action : must have exactly 1 parameter, either '%s' or '%s'."

#define XmNbadRowPixmap "badRowPixmap"
#define XmNbadRowPixmapMsg "Extended List: Row pixmaps must have depth of 1."

#define XmNbadXlfdFont "badXlfdFont"
#define XmNbadXlfdFontMsg \
"%s: All Xlfd fonts must contain 14 hyphens\n'%s' is not valid."

#define XmNcellNotEmpty "cellNotEmpty"
#define XmNcellNotEmptyMsg "XmIconBox: Cell %s is not empty"

#define XmNcolorNameTooLong "colorNameTooLong"
#define XmNcolorNameTooLongMsg \
  "%s: Color name '%s' is too long, truncated to '%s'."

#define XmNcontextSaveFailed "contextSaveFailed"
#define XmNcontextSaveFailedMsg "Internal Error: Could not save context data."

#define XmNconversionFailure "conversionFailure"
#define XmNconversionFailureMsg \
	"%s: Unable to perform string to %s conversion."

#define XmNcouldNotFindFamilyData "couldNotFindFamilyData"
#define XmNcouldNotFindFamilyDataMsg \
  "%s: Could not find family data for family '%s'."

#define XmNforceGreaterThanZero "forceGreaterThanZero"
#define XmNforceGreaterThanZeroMsg \
"%s : %s must be greater than zero being reset to one (1)."
    
#define XmNinsertBeforeNotSibling "insertBeforeNotSibling"
#define XmNinsertBeforeNotSiblingMsg "XmHierarchy: InsertBefore Widget\
 is not a sibling of '%s'.\nInserting child at end of list."
    
#define XmNnoComboShell "noComboShell"
#define XmNnoComboShellMsg \
    "Combination Box: When using a custom combo box a shell must be provided."

#define XmNnoEmptyCells "noEmptyCells"
#define XmNnoEmptyCellsMsg "XmIconBox: Could not find any empty cells."

#define XmNnoGadgetSupport "noGadgetSupport"
#define XmNnoGadgetSupportMsg "Widget %s does not support gadget children."
    
#define XmNpixEditBadImageCreate "pixEditBadImageCreate"
#define XmNpixEditBadImageCreateMsg "Pixmap Editor: Can't allocate image data"

#define XmNsameAsImageOrPix "sameAsImageOrPix"
#define XmNsameAsImageOrPixMsg \
"%s : The bitmapMode resource can only be changed at the same time as the pixmap or image"

#define XmNselfOrOutsideOfApplicationDrop "selfOrOutsideOfApplicationDrop"
#define XmNselfOrOutsideOfApplicationDropMsg \
    "Attempt to drop into illegal object."

#define XmNstaticResource "staticResource"
#define XmNstaticResourceMsg \
    "The resource '%s' may not be changed dynamically."

#define XmNtextVerifyFailed "textVerifyFailed"
#define XmNtextVerifyFailedMsg "Combination Box: Text item validation failed."

#define XmNunexpectedEvent "unexpectedEvent"
#define XmNunexpectedEventMsg "%s: Unexpected Event Type %s.\n"

#define XmNunparsableColor "unparsableColor"
#define XmNunparsableColorMsg \
  "%s: Could not parse the color name '%s'."

#define XmNnodeParentIsSelf "nodeParentIsSelf"
#define XmNnodeParentIsSelfMsg \
  "%s: The node parent cannot be self referential."

#define XmNstringGetFailed "stringGetFailed"
#define XmNstringGetFailedMsg "%s: XmGetStringLToR Failed."

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
