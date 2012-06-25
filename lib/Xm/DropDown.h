#ifndef _XmDropDown_h
#define _XmDropDown_h

#include <Xm/ComboBox2.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct _XmCombinationBox2ClassRec	*XmDropDownWidgetClass;
typedef struct _XmCombinationBox2Rec		*XmDropDownWidget;

#define XmDropDownGetValue(w)	XmCombinationBox2GetValue(w)
#define XmCreateDropDown(w,name,args,args_count) \
	XmCreateCombinationBox2(w,name,args,args_count)
#define XmDropDownGetLabel(w)	XmCombinationBox2GetLabel(w)
#define XmDropDownGetArrow(w)	XmCombinationBox2GetArrow(w)
#define XmDropDownGetText(w)	XmCombinationBox2GetText(w)
#define XmDropDownGetList(w)	XmCombinationBox2GetList(w)

#define xmDropDownWidgetClass	xmCombinationBox2WidgetClass

#if defined(__cplusplus)
}
#endif

#endif /* _XmCombinationBox2_h */
