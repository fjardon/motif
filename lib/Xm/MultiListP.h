#ifndef _XmMultiListP_h
#define _XmMultiListP_h

#include <Xm/Ext18ListP.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define XmMultiList_DEFAULT_VISIBLE_COUNT	XmExt18List_DEFAULT_VISIBLE_COUNT

#define XmMultiListIndex	XmExt18ListIndex

#define XmMultiList_offsets		XmExt18List_offsets
#define XmMultiListC_offsets		XmExt18ListC_offsets

#define XmMultiList_title(w)		XmExt18List_title(w)
#define XmMultiList_find_label(w)	XmExt18List_find_label(w)
#define XmMultiList_double_click(w)	XmExt18List_double_click(w)
#define XmMultiList_single_select(w)	XmExt18List_single_select(w)
#define XmMultiList_show_find(w)	XmExt18List_show_find(w)
#define XmMultiList_title_wid(w)	XmExt18List_title_wid(w)
#define XmMultiList_frame(w)		XmExt18List_frame(w)
#define XmMultiList_ilist(w)		XmExt18List_ilist(w)
#define XmMultiList_v_bar(w)		XmExt18List_v_bar(w)
#define XmMultiList_h_bar(w)		XmExt18List_h_bar(w)
#define XmMultiList_find(w)		XmExt18List_find(w)
#define XmMultiList_find_text(w)	XmExt18List_find_text(w)
#define XmMultiList_last_search(w)	XmExt18List_last_search(w)
#define XmMultiList_item_found(w)	XmExt18List_item_found(w)
#define XmMultiList_not_found(w)	XmExt18List_not_found(w)
#define XmMultiList_visible_rows(w)	XmExt18List_visible_rows(w)
#define XmMultiList_title_string(w)	XmExt18List_title_string(w)

typedef XmExt18ListClassPart	XmMultiListClassPart;
typedef XmExt18ListClassRec	XmMultiListClassRec;
typedef XmExt18ListPart		XmMultiListPart;
typedef XmExt18ListRec		XmMultiListRec;

#define xmMultiListClassRec	xmExt18ListClassRec

#if defined(__cplusplus)
}
#endif

#endif /* _XmExt18ListP_h */
