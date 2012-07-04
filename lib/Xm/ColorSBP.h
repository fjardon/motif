/*
 *    Copyright 2008, Integrated Computer Solutions, Inc.
 *
 *		       All Rights Reserved.
 *
 *    AUTHOR:  Andriy Konoval
 *
 */

#ifndef	_XmColorSelectionBoxP_h
#define	_XmColorSelectionBoxP_h

#if defined(VMS) || defined(__VMS)
#include <X11/apienvset.h>
#endif

/************************************************************
 *      INCLUDE FILES
 *************************************************************/

#include <Xm/BulletinBP.h>
#include <Xm/ColorSB.h>

/************************************************************
 *      TYPEDEFS AND DEFINES
 *************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define XmColorSelectionBox_COLOR_NAME_SIZE 100
#define XmColorSelectionBox_NUM_TOGGLES 2
#define S_len 256
#define H_len 241
#define widgetN 15
#define COUNT_PALETTE_COLORS 48
#define COUNT_CUSTOM_COLORS 16
#define MATRIX_WIDTH 280
#define DRAW_VOLUME_WIDTH 16
#define MIN_CORE_WIDTH 540
#define COLOR_FRAME_WIDTH 24
#define COLOR_FRAME_HEIGHT 23
#define CROSS_SIZE			    10
#define CROSS_CENTER_SIZE    1
#define CROSS_LINE_WIDTH     2

typedef struct _ColorSelectionBoxClassPart
{
	XtPointer extension;
} ColorSelectionBoxClassPart;

typedef struct _XmColorSelectionBoxClassRec
{
	CoreClassPart core_class;
	CompositeClassPart composite_class;
	ConstraintClassPart constraint_class;
	XmManagerClassPart manager_class;
	XmBulletinBoardClassPart bulletin_board_class;
	ColorSelectionBoxClassPart color_selector_class;
} XmColorSelectionBoxClassRec;

typedef struct
{
	int r;
	int g;
	int b;
} col;

typedef struct _XmColorSelectionBoxPart
{
	/* resources */

	Dimension margin_width; /* for geom management     */
	Dimension margin_height; /* for geom management	   */

	XmString add_label_string;
	XmString ok_label_string;
	XmString cancel_label_string;
	XmString help_label_string;
	int helpOn;

	/* private state */
	unsigned long color_value;
	int hue_value;
	int sat_value;
	int red_value;
	int green_value;
	int blue_value;

	Widget work_area; //other widgets

	Widget frameMatrix;
	Widget drawingAreaMatrix;
	Widget frameVolume;
	Widget drawingAreaVolume;
	Widget separator;
	Widget ManagerForm;
	Widget ColorBoard;
	Widget MatrixBoard;
	Widget ButtonBox;
	Widget scrollBar;
	Widget labelHue;
	Widget labelSat;
	Widget labelVal;
	Widget labelR;
	Widget labelG;
	Widget labelB;
	Widget redF;
	Widget greenF;
	Widget blueF;
	Widget brightnessF;
	Widget satF;
	Widget hueF;
	Widget color;
	Widget pButtonAddColor;
	Widget pButtonOk;
	Widget pButtonHelp;
	Widget labelPalette;
	Widget labelCustomColors;
	Widget frame_color;
	Widget ColoredFieldWidget[48];
	Widget CustomFieldWidget[48];
	Widget labelSelectedColor;
	Widget selectedColorF;
	Widget dropper;

	XtCallbackList ok_callback; /*  callbacks  */
	XtCallbackList cancel_callback;

	XImage *im1;
	XImage *im2;
	int widget_N;
	int depth;
	int brightness_volume;
	int x;
	int y;
	GC gc;
	GC gc2;
	col arr_col[S_len][H_len];
	Boolean first_expose;
	Widget highlighted_button;
	Pixel highlighting_background_color;	

  Boolean allow_callbacks;
} XmColorSelectionBoxPart;

typedef struct _XmColorSelectionBoxRec
{
	CorePart core;
	CompositePart composite;
	ConstraintPart constraint;
	XmManagerPart manager;
	XmBulletinBoardPart bulletin_board;
	XmColorSelectionBoxPart cs;
} XmColorSelectionBoxRec;

typedef struct _XmColorSelectionBoxConstraintPart
{
	XtPointer extension;
} XmColorSelectionBoxConstraintPart;

typedef struct _XmColorSelectionBoxConstraintRec
{
	XmBulletinBoardPart bulletin_board;
	XmColorSelectionBoxConstraintPart cs;
} XmColorSelectionBoxConstraintRec;

externalref XmBulletinBoardClassRec xmBulletinBoardClassRec;
/************************************************************
 *      MACROS
 *************************************************************/

#define XmColorSelectionBoxIndex (XmManagerIndex + 1)
#define XmColorSB_WorkArea(w) (((XmColorSelectionBoxWidget)(w))->cs.work_area)
#define XmColorSB_ColorBoard(w) (((XmColorSelectionBoxWidget)(w))->cs.ColorBoard)
#define XmColorSB_MatrixBoard(w) (((XmColorSelectionBoxWidget)(w))->cs.MatrixBoard)
#define XmColorSB_frameMatrix(w) (((XmColorSelectionBoxWidget)(w))->cs.frameMatrix)
#define XmColorSB_frameVolume(w) (((XmColorSelectionBoxWidget)(w))->cs.frameVolume)
#define XmColorSB_ManagerForm(w) (((XmColorSelectionBoxWidget)(w))->cs.ManagerForm)
#define XmColorSB_separator(w) (((XmColorSelectionBoxWidget)(w))->cs.separator)
#define XmColorSB_drawingAreaMatrix(w) (((XmColorSelectionBoxWidget)(w))->cs.drawingAreaMatrix)
#define XmColorSB_drawingAreaVolume(w) (((XmColorSelectionBoxWidget)(w))->cs.drawingAreaVolume)
#define XmColorSB_scrollBar(w) (((XmColorSelectionBoxWidget)(w))->cs.scrollBar)
#define XmColorSB_im1(w) (((XmColorSelectionBoxWidget)(w))->cs.im1)
#define XmColorSB_im2(w) (((XmColorSelectionBoxWidget)(w))->cs.im2)
#define XmColorSB_gc(w) (((XmColorSelectionBoxWidget)(w))->cs.gc)
#define XmColorSB_gc2(w) (((XmColorSelectionBoxWidget)(w))->cs.gc2)
#define XmColorSB_redF(w) (((XmColorSelectionBoxWidget)(w))->cs.redF)
#define XmColorSB_greenF(w) (((XmColorSelectionBoxWidget)(w))->cs.greenF)
#define XmColorSB_blueF(w) (((XmColorSelectionBoxWidget)(w))->cs.blueF)
#define XmColorSB_brightnessF(w) (((XmColorSelectionBoxWidget)(w))->cs.brightnessF)
#define XmColorSB_satF(w) (((XmColorSelectionBoxWidget)(w))->cs.satF)
#define XmColorSB_hueF(w) (((XmColorSelectionBoxWidget)(w))->cs.hueF)
#define XmColorSB_labelR(w) (((XmColorSelectionBoxWidget)(w))->cs.labelR)
#define XmColorSB_labelG(w) (((XmColorSelectionBoxWidget)(w))->cs.labelG)
#define XmColorSB_labelB(w) (((XmColorSelectionBoxWidget)(w))->cs.labelB)
#define XmColorSB_labelHue(w) (((XmColorSelectionBoxWidget)(w))->cs.labelHue)
#define XmColorSB_labelSat(w) (((XmColorSelectionBoxWidget)(w))->cs.labelSat)
#define XmColorSB_labelVal(w) (((XmColorSelectionBoxWidget)(w))->cs.labelVal)
#define XmColorSB_pButtonAddColor(w) (((XmColorSelectionBoxWidget)(w))->cs.pButtonAddColor)
#define XmColorSB_pButtonOk(w) (((XmColorSelectionBoxWidget)(w))->cs.pButtonOk)
#define XmColorSB_pButtonCancel(w) (((XmColorSelectionBoxWidget)(w))->bulletin_board.cancel_button)
#define XmColorSB_pButtonHelp(w) (((XmColorSelectionBoxWidget)(w))->cs.pButtonHelp)
#define XmColorSB_color(w) (((XmColorSelectionBoxWidget)(w))->cs.color)
#define XmColorSB_brightness_volume(w) (((XmColorSelectionBoxWidget)(w))->cs.brightness_volume)
#define XmColorSB_ColoredFieldWidget(w) (((XmColorSelectionBoxWidget)(w))->cs.ColoredFieldWidget)
#define XmColorSB_CustomFieldWidget(w) (((XmColorSelectionBoxWidget)(w))->cs.CustomFieldWidget)
#define XmColorSB_labelPalette(w) (((XmColorSelectionBoxWidget)(w))->cs.labelPalette)
#define XmColorSB_labelCustomColors(w) (((XmColorSelectionBoxWidget)(w))->cs.labelCustomColors)
#define XmColorSB_labelSelectedColor(w) (((XmColorSelectionBoxWidget)(w))->cs.labelSelectedColor) 
#define XmColorSB_selectedColorF(w) (((XmColorSelectionBoxWidget)(w))->cs.selectedColorF) 
#define XmColorSB_dropper(w) (((XmColorSelectionBoxWidget)(w))->cs.dropper) 

#define XmColorSB_color_value(w) (((XmColorSelectionBoxWidget)(w))->cs.color_value)
#define XmColorSB_hue_value(w) (((XmColorSelectionBoxWidget)(w))->cs.hue_value)
#define XmColorSB_sat_value(w) (((XmColorSelectionBoxWidget)(w))->cs.sat_value)
#define XmColorSB_red_value(w) (((XmColorSelectionBoxWidget)(w))->cs.red_value)
#define XmColorSB_green_value(w) (((XmColorSelectionBoxWidget)(w))->cs.green_value)
#define XmColorSB_blue_value(w) (((XmColorSelectionBoxWidget)(w))->cs.blue_value)
#define XmColorSB_helpOn(w) (((XmColorSelectionBoxWidget)(w))->cs.helpOn)

#define XmColorSB_widget_N(w) (((XmColorSelectionBoxWidget)(w))->cs.widget_N)
#define XmColorSB_depth(w) (((XmColorSelectionBoxWidget)(w))->cs.depth)
#define XmColorSB_brightness_volume(w) (((XmColorSelectionBoxWidget)(w))->cs.brightness_volume)
#define XmColorSB_x(w) (((XmColorSelectionBoxWidget)(w))->cs.x)
#define XmColorSB_y(w) (((XmColorSelectionBoxWidget)(w))->cs.y)
#define XmColorSB_arr_col(w) (((XmColorSelectionBoxWidget)(w))->cs.arr_col)
#define XmColorSB_first_expose(w) (((XmColorSelectionBoxWidget)(w))->cs.first_expose)

#define XmColorSB_margin_width(w) (((XmColorSelectionBoxWidget)(w))->cs.margin_width)
#define XmColorSB_margin_height(w) (((XmColorSelectionBoxWidget)(w))->cs.margin_height)
#define XmColorSB_frameColor(w) (((XmColorSelectionBoxWidget)(w))->cs.frame_color)

#define XmColorSB_allow_callbacks(w) (((XmColorSelectionBoxWidget)(w))->cs.allow_callbacks)
 

#define CS_WIDTH 570
#define CS_HEIGHT 480
/************************************************************
 *      GLOBAL DECLARATIONS
 *************************************************************/

/************************************************************
 *       EXTERNAL DECLARATIONS
 ************************************************************/

extern XmColorSelectionBoxClassRec xmColorSelectionBoxClassRec;

/************************************************************
 *       STATIC DECLARATIONS
 ************************************************************/

#ifdef __cplusplus
} /* Closes scope of 'extern "C"' declaration */
#endif

#if defined(VMS) || defined(__VMS)
#include <X11/apienvrst.h>
#endif

#endif /* _XmColorSelectionBoxP_h */
/* DON'T ADD STUFF AFTER THIS #endif */
