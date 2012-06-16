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

#ifndef _XmCombinationBox2P_h
#define _XmCombinationBox2P_h

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

/************************************************************
*	INCLUDE FILES
*************************************************************/

#include <Xm/XmP.h>		/* Need for XmVersion # */

#if (XmVersion == 1002 || XmVersion == 2001)
#include <Xm/ManagerP.h>
#endif

#include <Xm/ComboBox2.h>

/************************************************************
*	TYPEDEFS AND DEFINES
*************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define XmCombinationBox2_UP 		((unsigned char) 0)
#define XmCombinationBox2_UNPOSTED	XmCombinationBox2_UP
#define XmCombinationBox2_DOWN        	((unsigned char) 1)
#define XmCombinationBox2_POSTED		XmCombinationBox2_DOWN        	
#define XmCombinationBox2_IN_PROGRESS 	((unsigned char) 2)
#define XmCombinationBox2_BEGIN_POPUP_FROM_TEXT  ((unsigned char) 3)

#define XmCombinationBox2Index (XmManagerIndex + 1)

extern XmOffsetPtr XmCombinationBox2_offsets;
extern XmOffsetPtr XmCombinationBox2C_offsets;

#define ComboField(w, f, t) XmField(w, XmCombinationBox2_offsets, \
                                    XmCombinationBox2, f, t)
#define XiComboBox_h_space(w) ComboField(w, h_space, Dimension)
#define XiComboBox_v_space(w) ComboField(w, v_space, Dimension)
#define XiComboBox_popup_offset(w) ComboField(w, popup_offset, int)
#define XiComboBox_verify(w) ComboField(w, verify, Boolean)
#define XiComboBox_editable(w) ComboField(w, editable, Boolean)
#define XiComboBox_show_label(w) ComboField(w, show_label, Boolean)
#define XiComboBox_customized_combo_box(w) ComboField(w, customized_combo_box, Boolean)
#define XiComboBox_use_text_field(w) ComboField(w, use_text_field, Boolean)
#define XiComboBox_popup_shell(w) ComboField(w, popup_shell, Widget)
#define XiComboBox_popup_cursor(w) ComboField(w, popup_cursor, Cursor)
#define XiComboBox_translations(w) ComboField(w, translations, XtTranslations)
#define XiComboBox_verify_text_callback(w) ComboField(w, verify_text_callback, XtCallbackList)
#define XiComboBox_verify_text_failed_callback(w) ComboField(w, verify_text_failed_callback, XtCallbackList)
#define XiComboBox_update_text_callback(w) ComboField(w, update_text_callback, XtCallbackList)
#define XiComboBox_update_shell_callback(w) ComboField(w, update_shell_callback, XtCallbackList)
#define XiComboBox_visible_items(w) ComboField(w, visible_items, int)
#define XiComboBox_new_visual_style(w) ComboField(w, new_visual_style, Boolean)

#define XiComboBox_old_text(w) ComboField(w, old_text, String)
#define XiComboBox_focus_owner(w) ComboField(w, focus_owner, Window)
#define XiComboBox_focus_state(w) ComboField(w, focus_state, int)
#define XiComboBox_list_state(w) ComboField(w, list_state, unsigned char)
#define XiComboBox_text_x(w) ComboField(w, text_x, Position)
#define XiComboBox_list(w) ComboField(w, list, Widget)
#define XiComboBox_label(w) ComboField(w, label, Widget)
#define XiComboBox_text(w) ComboField(w, text, Widget)
#define XiComboBox_arrow(w) ComboField(w, arrow, Widget)

#define XiComboBox_autoTraversal(w) ComboField(w, autoTraversal, Boolean)
#define XiComboBox_activateOnFill(w) ComboField(w, activateOnFill, int)
#define XiComboBox_doActivate(w) ComboField(w, doActivate, Boolean)
#define XiComboBox_inValueChanged(w) ComboField(w, inValueChanged, Boolean)

/************************************************************
*	MACROS
*************************************************************/

/************************************************************
*	GLOBAL DECLARATIONS
*************************************************************/

/* Should return True to ignore invalid entry warning. Combination Box
 *  does not currently use this. Presumes do it in subclasses 
 */
typedef Boolean (*XmCombinationBox2TextProc)(
#ifndef _NO_PROTO
    Widget,	
    char *text
#endif
);

typedef Boolean (*XmCombinationBox2TextListMapProc)(
#ifndef _NO_PROTO
    Widget,		/* combo box */
    Widget,		/* text */
    Widget		/* list */
#endif
);

/* Version number for the first Revision  */
#define XmCombinationBox2ExtensionVersion 2

typedef struct {
        /* standard extension fields */

	XtPointer 			next_extension;
	XrmQuark                        record_type;
	long                            version;
	Cardinal                        record_size;

	/* extra fields */
	XmCombinationBox2TextProc	verify;
	XmCombinationBox2TextProc	update;
	XmCombinationBox2TextListMapProc	setTextFromList;
	XmCombinationBox2TextListMapProc	setListFromText;
} XmCombinationBox2ClassPartExtension;

typedef struct {
    XtPointer extension;	/* Just in case we need it later. */
} XmCombinationBox2ClassPart;

typedef struct _XmCombinationBox2ClassRec {
    CoreClassPart		core_class;
    CompositeClassPart		composite_class;
    ConstraintClassPart		constraint_class;
    XmManagerClassPart		manager_class;
    XmCombinationBox2ClassPart	combo_class;
} XmCombinationBox2ClassRec;

typedef struct {
    /* resources */

    Dimension h_space;		/* The amount of space to leave between */
    Dimension v_space;		/* widgets and the box edges. */

    int popup_offset;		/* The offset of the popup offset from the
				   left edge of the text widget. */

    Boolean verify;		/* Verify the contents of the Text widget
				   on leave or CR when this is True. */

    Boolean editable;		/* Allow the text field to be edited? */

    Boolean show_label;		/* Whether or not to show the label. */
    
    Boolean customized_combo_box; /* Is this a customized combo box. */

    Boolean use_text_field; /* Use XmTextField of XmText for textual input */

    Widget popup_shell;		/* The id of the popup shell. */
	
    Cursor popup_cursor;	/* Cursor for the Popup Window.  */

    XtTranslations translations; /* The translation table for all children. */

    /*
     * Callbacks to verify, and update the text and shell widgets.
     */

    XtCallbackList verify_text_callback;
    XtCallbackList verify_text_failed_callback;
    XtCallbackList update_text_callback;
    XtCallbackList update_shell_callback;

    /* private state */

    String old_text;		/* The old text value. */
    Window focus_owner;		/* Previous owner and state of the focus. */
    int focus_state;

    unsigned char list_state;	/* XmCombinationBox2_UP, XmCombinationBox2_DOWN or XmCombinationBox2_IN_PROGRESS. */

    Position text_x;		/* X location of the text widget. */

    Widget list;		/* List contained in the popup shell. */

    Widget label;		/* The three children of the combo box. */
    Widget text; 
    Widget arrow;

    int visible_items;		/* only to set/get XmNvisibleItemCount, which is
				** a sop for non-customized combobox users */
    
    Boolean new_visual_style;

    Boolean autoTraversal;	/* traverse next on return */
    int	activateOnFill;		/* activate when we fill this many chars */
    Boolean doActivate;		/* do activate on next value changed */
    Boolean inValueChanged;	/* recursion prevention */
} XmCombinationBox2Part;

typedef struct _XmCombinationBox2Rec {
    CorePart		core;
    CompositePart	composite;
    ConstraintPart	constraint;
    XmManagerPart	manager;
    XmCombinationBox2Part combo;
} XmCombinationBox2Rec;

/************************************************************
*	EXTERNAL DECLARATIONS
*************************************************************/

extern XmCombinationBox2ClassRec xmCombinationBox2ClassRec;

/************************************************************
*	STATIC DECLARATIONS
*************************************************************/

#ifdef __cplusplus
}	/* Closes scope of 'extern "C"' declaration */
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* _XmCombinationBox2P_h */
