#ifndef _XmDropDownP_h
#define _XmDropDownP_h

#include <Xm/ComboBox2P.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XmDropDown_UP		XmCombinationBox2_UP
#define XmDropDown_UNPOSTED	XmCombinationBox2_UNPOSTED
#define XmDropDown_DOWN		XmCombinationBox2_DOWN
#define XmDropDown_POSTED	XmCombinationBox2_POSTED
#define XmDropDown_IN_PROGRESS	XmCombinationBox2_IN_PROGRESS
#define XmDropDown_BEGIN_POPUP_FROM_TEXT \
	XmCombinationBox2_BEGIN_POPUP_FROM_TEXT

#define XmDropDownIndex	XmCombinationBox2Index

#define XmDropDown_offsets	XmCombinationBox2_offsets
#define XmDropDownC_offsets	XmCombinationBox2C_offsets

#define XmComboBox2_h_space(w)
#define XmComboBox2_v_space(w)
#define XmComboBox2_popup_offset(w)
#define XmComboBox2_verify(w)
#define XmComboBox2_editable(w)
#define XmComboBox2_show_label(w)
#define XmComboBox2_customized_combo_box(w)
#define XmComboBox2_use_text_field(w)
#define XmComboBox2_popup_shell(w)
#define XmComboBox2_popup_cursor(w)
#define XmComboBox2_translations(w)
#define XmComboBox2_verify_text_callback(w)
#define XmComboBox2_verify_text_failed_callback(w)
#define XmComboBox2_update_text_callback(w)
#define XmComboBox2_update_shell_callback(w)
#define XmComboBox2_visible_items(w)
#define XmComboBox2_new_visual_style(w)

#define XmComboBox2_old_text(w)
#define XmComboBox2_focus_owner(w)
#define XmComboBox2_focus_state(w)
#define XmComboBox2_list_state(w)
#define XmComboBox2_text_x(w)
#define XmComboBox2_list(w)
#define XmComboBox2_label(w)
#define XmComboBox2_text(w)
#define XmComboBox2_arrow(w)

#define XmComboBox2_autoTraversal(w)
#define XmComboBox2_activateOnFill(w)
#define XmComboBox2_doActivate(w)
#define XmComboBox2_inValueChanged(w)

typedef Boolean (*XmDropDownTextProc)(
#ifndef _NO_PROTO
    Widget,	
    char *text
#endif
);

typedef Boolean (*XmDropDownTextListMapProc)(
#ifndef _NO_PROTO
    Widget,		/* combo box */
    Widget,		/* text */
    Widget		/* list */
#endif
);

/* Version number for the first Revision  */
#define XmDropDownExtensionVersion	XmCombinationBox2ExtensionVersion

typedef XmDropDownClassPartExtension	XmCombinationBox2ClassPartExtension;
typedef	XmDropDownClassPart	XmCombinationBox2ClassPart;
typedef XmDropDownClassRec	XmCombinationBox2ClassRec;
typedef XmDropDownPart		XmCombinationBox2Part;
typedef XmDropDownRec	XmCombinationBox2Rec;

#define xmDropDownClassRec	xmCombinationBox2ClassRec

#ifdef __cplusplus
}	/* Closes scope of 'extern "C"' declaration */
#endif

#endif /* _XmDropDownP_h */
