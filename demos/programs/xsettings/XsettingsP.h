/**
 *
 * $Id: XsettingsP.h,v 1.2 2009/06/28 11:49:56 rwscott Exp $
 *
 * Copyright 2009 Rick Scott <rwscott@users.sourceforge.net>
 *
 **/

#ifndef _XSETTINGSP_H
#define _XSETTINGSP_H

#include "Xsettings.h"
#include <X11/IntrinsicP.h>
#include <X11/ObjectP.h>
#include <Xm/XmP.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    XtPointer extension;
} XmXsettingsClassPart;

typedef struct _XmXsettingsClassRec {
    ObjectClassPart object_class;
    XmXsettingsClassPart xsettings_class;
} XmXsettingsClassRec;

extern XmXsettingsClassRec xmXsettingsClassRec;

typedef struct _XmXsettingsPart {
    Window manager;
    Cardinal serial;
    int num_settings;
    XSettingsSetting *setting;
    Atom atom;
    char local_byte_order;
    XtCallbackList xsettings_callback;
    XtPointer user_data;
    Atom selection_atom;
} XmXsettingsPart;

typedef struct _XmXsettingsRec {
    ObjectPart object;
    XmXsettingsPart xsettings;
} XmXsettingsRec;

#ifdef __cplusplus
}
#endif

#endif
