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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <Xm/LabelP.h>
#include <Xm/VendorSEP.h>
#include <Xm/GadgetP.h>
#include <BaseClassI.h>
#ifdef HAVE_SLIDEC_H
#include <Xm/SlideC.h>
#endif

typedef XmVendorShellExtPart XmToolTipDataStruct;

static XmToolTipDataStruct *
ToolTipGetData(Widget w)
{
Widget top = w;
XmWidgetExtData	extData;
XmVendorShellExtObject vse;

    while (XtParent(top))
    {
	top = XtParent(top);
    }
    if ((extData = _XmGetWidgetExtData( (Widget) top, XmSHELL_EXTENSION)) &&
	 (vse = (XmVendorShellExtObject) extData->widget))
    {
    	if (vse->vendor.label == NULL)
    	{
    	Widget shell;

	    shell = XtVaCreateWidget("TipShell", transientShellWidgetClass, top,
		XmNoverrideRedirect, True,
		NULL);
	    vse->vendor.label = XmCreateLabel(shell, "TipLabel", NULL, 0);
	    XtManageChild(vse->vendor.label);
    	}
    	return(&vse->vendor);
    }
    else
    {
    	return(NULL);
    }
}

static void
ToolTipUnpostFinish(Widget slide, XtPointer client_data, XtPointer call_data)
{
XmToolTipDataStruct *TipData = (XmToolTipDataStruct *)client_data;

    if ( ! XtParent(TipData->label)->core.being_destroyed)
    {
	XtPopdown(XtParent(TipData->label));
    }
}

static void
ToolTipUnpost(XtPointer client_data, XtIntervalId *id)
{
XmToolTipDataStruct *TipData = (XmToolTipDataStruct *)client_data;

    if (TipData->duration_timer)
    {
	if (! id || (id && (*id != TipData->duration_timer)))
	{
	    XtRemoveTimeOut(TipData->duration_timer);
	}
	TipData->duration_timer = (XtIntervalId)NULL;
    }
#ifdef HAVE_SLIDEC_H
    if (TipData->slider)
    {
    	XtDestroyWidget(TipData->slider);
    	TipData->slider = NULL;
    }
#else
#endif
    ToolTipUnpostFinish(NULL, TipData, NULL);
}

static void
ToolTipPostFinish(Widget slide, XtPointer client_data, XtPointer call_data)
{
XmToolTipDataStruct *TipData = (XmToolTipDataStruct *)client_data;

    TipData->slider = NULL;
    if (TipData->post_duration > 0)
    {
    	TipData->duration_timer = XtAppAddTimeOut(XtWidgetToApplicationContext(TipData->label),
    		(unsigned long)TipData->post_duration,
    		(XtTimerCallbackProc)ToolTipUnpost,
    		TipData);
    }
}

static void
ToolTipPost(XtPointer client_data, XtIntervalId *id)
{
Widget w = (Widget)client_data;
XmToolTipDataStruct *TipData = ToolTipGetData(w);
int rx, ry, x, y;
unsigned int key;
Window root, child;
XtWidgetGeometry geo;
Position destX, destY;

    /*
    printf("%s:%s(%d) - %s\n", __FILE__, __FUNCTION__, __LINE__,
    	XtName(w));
    	*/
    TipData->timer = (XtIntervalId)NULL;
    XQueryPointer(XtDisplay(w),
		    XtWindow(w),
		    &root,
		    &child,
		    &rx, &ry,
		    &x, &y,
		    &key);
    if (TipData->duration_timer != (XtIntervalId)NULL)
    {
	XtRemoveTimeOut(TipData->duration_timer);
	TipData->duration_timer = (XtIntervalId)NULL;
    }
    if (XmIsPrimitive(w))
    {
	XtVaSetValues(TipData->label,
	    XmNlabelString, ((XmPrimitiveWidget)w)->primitive.tool_tip_string,
	    NULL);
    }
    else if (XmIsGadget(w))
    {
	XtVaSetValues(TipData->label,
	    XmNlabelString, ((XmGadget)w)->gadget.tool_tip_string,
	    NULL);
    }
    else
    {
    XmString string;

	string = XmStringCreateLocalized(XtName(w));
	XtVaSetValues(TipData->label,
	    XmNlabelString, string,
	    NULL);
	XmStringFree(string);
    }
    XtQueryGeometry(TipData->label, NULL, &geo);
#if 1
    /* rws 25 Feb 2001
       Fix for Bug #1153
       Don't let the tip be off the right/bottom of the screen
     */
    destX = rx + (XmIsGadget(w) ? XtX(w) : 0) - x + XtWidth(w) / 2;
    if (destX + geo.width > WidthOfScreen(XtScreen(w)))
    {
    	destX = WidthOfScreen(XtScreen(w)) - geo.width;
    }
    destY = ry + (XmIsGadget(w) ? XtY(w) : 0) - y + XtHeight(w);
    if (destY + geo.height > HeightOfScreen(XtScreen(w)))
    {
	destY = ry + (XmIsGadget(w) ? XtY(w) : 0) - y - geo.height;
    }
#endif
#ifdef HAVE_SLIDEC_H
    XtVaSetValues(XtParent(TipData->label),
	XmNx, rx + 1,
	XmNy, ry + 1,
	XmNwidth, 1,
	XmNheight, 1,
	NULL);
    TipData->slider = XtVaCreateWidget("ToolTipSlider", xmSlideContextWidgetClass,
	XmGetXmDisplay(XtDisplay(w)),
	XmNslideWidget, XtParent(TipData->label),
	XmNslideDestX, destX,
	XmNslideDestY, destY,
	XmNslideDestWidth, geo.width,
	XmNslideDestHeight, geo.height,
	NULL);
    XtAddCallback(TipData->slider, XmNslideFinishCallback, (XtCallbackProc)ToolTipPostFinish, TipData);
#else
    XtVaSetValues(XtParent(TipData->label),
	XmNx, destX,
	XmNy, destY,
	XmNwidth, geo.width,
	XmNheight, geo.height,
	NULL);
#endif
    XtPopup(XtParent(TipData->label), XtGrabNone);
#ifdef HAVE_SLIDEC_H
#else
    ToolTipPostFinish(NULL, TipData, NULL);
#endif
}

void 
_XmToolTipEnter(Widget wid,
		  XEvent *event,
		  String *params,
		  Cardinal *num_params)
{
XmToolTipDataStruct *TipData;

    /*
    printf("%s:%s(%d) - %s %s %i\n", 
	__FILE__, __FUNCTION__, __LINE__,
	XtName(wid),
	XmIsPrimitive(wid) ? "Primitive" : (XmIsGadget(wid) ? "Gadget" : "Unknown"),
	event ? event->type : 0);
	*/
    if ((XmIsPrimitive(wid) && ((XmPrimitiveWidget)wid)->primitive.tool_tip_string) ||
        (XmIsGadget(wid) && ((XmGadget)wid)->gadget.tool_tip_string))
    {
	TipData = ToolTipGetData(wid);
	if (TipData && TipData->enable && ! TipData->timer)
	{
	unsigned long delay;

	    if (event && (event->xcrossing.time - TipData->leave_time < TipData->post_delay))
	    {
		delay = 0;
	    }
	    else
	    {
		delay = (unsigned long)TipData->post_delay;
	    }
	    if (TipData->duration_timer)
	    {
		XtRemoveTimeOut(TipData->duration_timer);
		TipData->duration_timer = (XtIntervalId)NULL;
	    }
	    TipData->timer = XtAppAddTimeOut(XtWidgetToApplicationContext(wid),
		delay,
		(XtTimerCallbackProc)ToolTipPost,
		wid);
	}
    }
}

void 
_XmToolTipLeave(Widget wid,
		  XEvent *event,
		  String *params,
		  Cardinal *num_params)
{
XmToolTipDataStruct *TipData = ToolTipGetData(wid);

    /*
    printf("%s:%s(%d) - %s\n", __FILE__, __FUNCTION__, __LINE__, XtName(wid));
    */
    if (TipData)
    {
	if (TipData->timer)
	{
	    XtRemoveTimeOut(TipData->timer);
	    TipData->timer = (XtIntervalId)NULL;
	}
	else
	{
	    if (event && (TipData->duration_timer || TipData->post_duration == 0))
	    {
		TipData->leave_time = event->xcrossing.time;
	    }
	    ToolTipUnpost(TipData, NULL);
	}
    }
}

Widget 
XmToolTipGetLabel(Widget wid)
{
XmToolTipDataStruct *TipData = ToolTipGetData(wid);

    return(TipData->label);
}
