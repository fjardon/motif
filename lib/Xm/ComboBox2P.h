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
#include <Xm/ManagerP.h>
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

#define XmComboBox2_h_space(w) (((XmCombinationBox2Widget)(w))->combo.h_space)
#define XmComboBox2_v_space(w) (((XmCombinationBox2Widget)(w))->combo.v_space)
#define XmComboBox2_popup_offset(w) (((XmCombinationBox2Widget)(w))->combo.popup_offset)
#define XmComboBox2_verify(w) (((XmCombinationBox2Widget)(w))->combo.verify)
#define XmComboBox2_editable(w) (((XmCombinationBox2Widget)(w))->combo.editable)
#define XmComboBox2_show_label(w) (((XmCombinationBox2Widget)(w))->combo.show_label)
#define XmComboBox2_customized_combo_box(w) (((XmCombinationBox2Widget)(w))->combo.customized_combo_box)
#define XmComboBox2_use_text_field(w) (((XmCombinationBox2Widget)(w))->combo.use_text_field)
#define XmComboBox2_popup_shell(w) (((XmCombinationBox2Widget)(w))->combo.popup_shell)
#define XmComboBox2_popup_cursor(w) (((XmCombinationBox2Widget)(w))->combo.popup_cursor)
#define XmComboBox2_translations(w) (((XmCombinationBox2Widget)(w))->combo.translations)
#define XmComboBox2_verify_text_callback(w) (((XmCombinationBox2Widget)(w))->combo.verify_text_callback)
#define XmComboBox2_verify_text_failed_callback(w) (((XmCombinationBox2Widget)(w))->combo.verify_text_failed_callback)
#define XmComboBox2_update_text_callback(w) (((XmCombinationBox2Widget)(w))->combo.update_text_callback)
#define XmComboBox2_update_shell_callback(w) (((XmCombinationBox2Widget)(w))->combo.update_shell_callback)
#define XmComboBox2_visible_items(w) (((XmCombinationBox2Widget)(w))->combo.visible_items)
#define XmComboBox2_new_visual_style(w) (((XmCombinationBox2Widget)(w))->combo.new_visual_style)

#define XmComboBox2_old_text(w) (((XmCombinationBox2Widget)(w))->combo.old_text)
#define XmComboBox2_focus_owner(w) (((XmCombinationBox2Widget)(w))->combo.focus_owner)
#define XmComboBox2_focus_state(w) (((XmCombinationBox2Widget)(w))->combo.focus_state)
#define XmComboBox2_list_state(w) (((XmCombinationBox2Widget)(w))->combo.list_state)
#define XmComboBox2_text_x(w) (((XmCombinationBox2Widget)(w))->combo.text_x)
#define XmComboBox2_list(w) (((XmCombinationBox2Widget)(w))->combo.list)
#define XmComboBox2_label(w) (((XmCombinationBox2Widget)(w))->combo.label)
#define XmComboBox2_text(w) (((XmCombinationBox2Widget)(w))->combo.text)
#define XmComboBox2_arrow(w) (((XmCombinationBox2Widget)(w))->combo.arrow)

#define XmComboBox2_autoTraversal(w) (((XmCombinationBox2Widget)(w))->combo.autoTraversal)
#define XmComboBox2_activateOnFill(w) (((XmCombinationBox2Widget)(w))->combo.activateOnFill)
#define XmComboBox2_doActivate(w) (((XmCombinationBox2Widget)(w))->combo.doActivate)
#define XmComboBox2_inValueChanged(w) (((XmCombinationBox2Widget)(w))->combo.inValueChanged)

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

    Widget		vsb;
    Widget		hsb;
    Boolean		scrolling;
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
