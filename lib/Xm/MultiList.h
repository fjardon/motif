#ifndef _XmMultiList_h
#define _XmMultiList_h

#include <Xm/Ext18List.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define XmMULTILIST_FOUND	XmEXT18LIST_FOUND
#define XmMULTILIST_NOT_FOUND	XmEXT18LIST_NOT_FOUND
typedef struct _XmExt18ListClassRec	*XmMultiListWidgetClass;
typedef struct _XmExt18ListRec		*XmMultiListWidget;

typedef struct _Xm18RowInfo	XmMultiListRowInfo;

typedef struct _Xm18ExtListCallbackStruct	XmMultiListCallbackStruct;

typedef int (*XmMultiListSortFunction)(
#ifndef _NO_PROTO
short, const XmMultiListRowInfo *, const XmMultiListRowInfo *
#endif
);

#define XmMultiListGetSelectedRows(w)	XmExt18ListGetSelectedRows(w)
#define XmCreateMultiList(w,s,a,c)	XmCreateExt18List(w,s,a,c)
#define XmMultiListUnselectAllItems(w)	XmExt18ListUnselectAllItems(w)
#define XmMultiListDeselectAllItems(w)	XmExt18ListUnselectAllItems(w)
#define XmMultiListUnselectItem(w,r)	XmExt18ListUnselectItem(w,r)
#define XmMultiListDeselectItem(w,r)	XmExt18ListUnselectItem(w,r)
#define XmMultiListToggleRow(w,n)	XmExt18ListToggleRow(w,n)
#define XmMultiListSelectItems(w,i,c,n)	XmExt18ListSelectItems(w,i,c,n)
#define XmMultiListDeselectItems(w,i,c)	XmExt18ListDeselectItems(w,i,c)
#define XmMultiListSelectAllItems(w,n)	XmExt18ListSelectAllItems(w,n)

#define XmMultiListSelectRow(w,r,n)	XmExt18ListSelectRow(w,r,n)
#define XmMultiListDeselectRow(w,r)	XmExt18ListDeselectRow(w,r)
#define XmMultiListGetSelectedRowArray(w,n)	XmExt18ListGetSelectedRowArray(w,n)
#define	XmMultiListMakeRowVisible(w,r)	XmExt18ListMakeRowVisible(w,r)
#define xmMultiListWidgetClass	xmExt18ListWidgetClass

#if defined(__cplusplus)
}
#endif

#endif /* _XmMultiList_h */
