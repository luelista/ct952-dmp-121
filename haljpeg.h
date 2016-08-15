//**************************************************************************
//         Copyright (c) 2004, Cheertek Inc . All rights reserved.
//         D300, all right reserved.

//      Product : WinDVD Firmware

//      Date    : 2004.5.25
//      Author  : Cheertek (D300 CoCo Chiang)
//      Purpose : JPEG-relative control
//      Sources : HALJPEG.c/HALJPEG.h
//***************************************************************************
#ifndef __HALJPEG_H__
#define __HALJPEG_H__

//define the parameter to tune the TV attribute
#define HALJPEG_TUNE_DISPLAY_RATIO_FOR_TV   110

//define the number for JPEG effect.
#define HALJPEG_EFFECT_LINE_H               6
#define HALJPEG_EFFECT_LINE_V               4

//define the panning pixels 
#define HALJPEG_PAN_PIXELS                  32
//#define JPEG_SINGLE_BUFFER_FULL_SCREEN
//parameter for displaying JPEG
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16) 
#ifdef JPEG_SINGLE_BUFFER_FULL_SCREEN
#define HALJPEG_FIRST_SCALING_WIDTH         720
#define HALJPEG_FIRST_SCALING_HEIGHT        448
#define HALJPEG_DISPLAY_BUF_WIDTH           720 
#define HALJPEG_DISPLAY_BUF_HEIGHT          448
#else
#define HALJPEG_FIRST_SCALING_WIDTH         640
#define HALJPEG_FIRST_SCALING_HEIGHT        448
#define HALJPEG_DISPLAY_BUF_WIDTH           640 
#define HALJPEG_DISPLAY_BUF_HEIGHT          448
#endif //#ifdef JPEG_SINGLE_BUFFER_FULL_SCREEN

#else
#ifdef CT950_STYLE
#define HALJPEG_FIRST_SCALING_WIDTH         720
#define HALJPEG_FIRST_SCALING_HEIGHT        480
#else
#define HALJPEG_FIRST_SCALING_WIDTH         640
#define HALJPEG_FIRST_SCALING_HEIGHT        432
#endif //CT950_STYLE
#define HALJPEG_DISPLAY_BUF_WIDTH           720 
#define HALJPEG_DISPLAY_BUF_HEIGHT          480
#endif //DRAM_CONFIGURATION_TYPE_DVD

//parameter for "fit widht" or "fit height"
#define HALJPEG_DISPLAY_FIT_WIDTH           0
#define HALJPEG_DISPLAY_FIT_HEIGHT          1

//For panel usage
#define HALJPEG_PANEL_WIDTH_RATIO           16
#define HALJPEG_PANEL_HEIGHT_RATIO          9

#ifdef SUPPORT_JPEG_FULL_SCREEN
#if(HALJPEG_PANEL_WIDTH_RATIO == 16 && HALJPEG_PANEL_HEIGHT_RATIO == 9)
#define HALJPEG_CLIP_FIT_DISP_HEIGHT
#else
#define HALJPEG_CLIP_FIT_DISP_WIDTH
#endif//#if(HALJPEG_PANEL_WIDTH_RATIO == 16 && HALJPEG_PANEL_HEIGHT_RATIO == 9)
#endif//#ifdef SUPPORT_JPEG_FULL_SCREEN


// define the effect of slide transition
#define HALJPEG_EFFECT_NONE                 0
#define HALJPEG_EFFECT_COVER_UP             1
#define HALJPEG_EFFECT_COVER_DOWN           2
#define HALJPEG_EFFECT_COVER_LEFT           3
#define HALJPEG_EFFECT_COVER_RIGHT          4
#define HALJPEG_EFFECT_UNCOVER_UP           5
#define HALJPEG_EFFECT_UNCOVER_DOWN         6
#define HALJPEG_EFFECT_UNCOVER_LEFT         7
#define HALJPEG_EFFECT_UNCOVER_RIGHT        8    
#define HALJPEG_EFFECT_WIPE_UP              9
#define HALJPEG_EFFECT_WIPE_DOWN            10
#define HALJPEG_EFFECT_WIPE_LEFT            11
#define HALJPEG_EFFECT_WIPE_RIGHT           12
#define HALJPEG_EFFECT_PUSH_UP              13
#define HALJPEG_EFFECT_PUSH_DOWN            14
#define HALJPEG_EFFECT_PUSH_LEFT            15
#define HALJPEG_EFFECT_PUSH_RIGHT           16
#define HALJPEG_EFFECT_BLIND_VER            17
#define HALJPEG_EFFECT_BLIND_HOR            18
#define HALJPEG_EFFECT_COLORATION_VER       19
#define HALJPEG_EFFECT_COLORATION_HOR       20
#define HALJPEG_EFFECT_BLOCK_1              21
#define HALJPEG_EFFECT_BLOCK_2              22
#define HALJPEG_EFFECT_BLOCK_3              23
#define HALJPEG_EFFECT_BLOCK_4              24
#define HALJPEG_EFFECT_BLOCK_5              25
#define HALJPEG_EFFECT_BLOCK_6              26
#define HALJPEG_EFFECT_BLOCK_7              27
#define HALJPEG_EFFECT_BLOCK_8              28
#define HALJPEG_EFFECT_BLOCK_9              29
#define HALJPEG_EFFECT_RANDOM               30
#define HALJPEG_EFFECT_LAST                 HALJPEG_EFFECT_RANDOM

#define HALJPEG_RANDOM_EFFECT_BASE          29

// define the combination of "rotate" and "flip"
#define HALJPEG_OPERATION_I                 0
#define HALJPEG_OPERATION_RC                1
#define HALJPEG_OPERATION_FHFV              2
#define HALJPEG_OPERATION_RCC               3
#define HALJPEG_OPERATION_FH                4
#define HALJPEG_OPERATION_RCFV              5
#define HALJPEG_OPERATION_FV                6 
#define HALJPEG_OPERATION_RCFH              7

//define the background's color of JPEG
#define HALJPEG_SLIDESHOW_BACKGROUND_COLOR  0x008080

//The parameter for HALJPEG_Status()
#define HALJPEG_PARSE_HEADER                0   //Get "parse header" status.
#define HALJPEG_DECODE                      1   //Get "Decode" status.
#define HALJPEG_PSFB                        2   //tommy0.67
#define HALJPEG_RESET                       3
#define HALJPEG_OPERATION                   4

//The parameter for getting JPEG status from JPEG Decoder
#define JPEG_PARSE_HEADER                   0   //Get "parse header" status.
#define JPEG_DECODE                         1   //Get "Decode" status.
#define JPEG_PSFB                           2   //Get "Preset frame buffer" status.
#define JPEG_RESET                          3   //Get "Reset" status.
#define JPEG_OPERATION                      4   //Get "Operation" status.

#define JPEG_STATUS_FAIL                    0   //The action is failed.
#define JPEG_STATUS_OK                      1   //The action is OK.
#define JPEG_STATUS_UNFINISH                2   //The action is unfinished. 
#define JPEG_STATUS_UNSUPPORT               3   //This return value is for "parse header" to represent that the JPEG file can't be decoded in our solution.
#define Y_MAXSIZE_8C00  (0x8c00L)

#define HALJPEG_STATUS_FAIL                 JPEG_STATUS_FAIL   //The action is failed.
#define HALJPEG_STATUS_OK                   JPEG_STATUS_OK   //The action is OK.
#define HALJPEG_STATUS_UNFINISH             JPEG_STATUS_UNFINISH   //The action is unfinished. 
#define HALJPEG_STATUS_UNSUPPORT            JPEG_STATUS_UNSUPPORT   //This return value is for "parse header" to represent that the JPEG file can't be decoded in our solution.

// Chuan-eCos, add some definition.
#define HAL_ZOOM_FAIL                       0
#define HAL_ZOOM_OK                         1
#define HAL_ZOOM_REDO                       2

// LLY2.22, define for HALJPEG_ConfigFrame()
#define HALJPEG_FRAME_BUFFER_SLIDE_SHOW     0
#define HALJPEG_FRAME_BUFFER_PREVIEW        1
#define HALJPEG_FRAME_BUFFER_THUMBNAIL      2

//senshong2.78: block effect reference delay time
#define  HALJPEG_TRANSITION_TIME COUNT_3_SEC

typedef struct tagJPEGSETTING
{
    BYTE    bFrameBuffer;           //0:buffer 0, 1:buffer 1.
    BYTE 	bOperation;             //0: I, 1: Fh, 2: Fv, 3: FhFv, 4: Rc, 5: Rcc, 6: RcFh, 7: RcFv
    WORD	wDisplayStartH;         //The starting horizontal position of the display rectangle.
    WORD 	wDisplayStartV;         //The starting vertical position of the display rectangle.
    WORD 	wDisplayWidth;          //The width of the display rectangle.
    WORD 	wDisplayHeight;         //The height of the display rectangle.
    DWORD	dwBackgroundColor;      //The background color (YUV).
} JPEGSETTING, *PJPEGSETTING;


//This structre is for scaling.
typedef struct tagJPEGSCALING
{
    WORD    wSrcExtractStartH;      //The starting horizontal position to extract picture from Forg.
    WORD    wSrcExtractStartV;      //The starting horizontal position to extract picture from Forg.
    WORD    wSrcExtractWidth;       //The width of the picture we want to extract form the Forg.
    WORD    wSrcExtractHeight;      //The height of the picture we want to extract form the Forg.
    WORD    wScalingWidth;          //The width of the picture after scaling.
    WORD    wScalingHeight;         //The height of the picture after scaling.
    PJPEGSETTING pJPEGSetting;     //The JPEG setting.
} JPEGSCALING, *PJPEGSCALING;

//This structure is for "decode" function
typedef struct tagJPEGDECODE
{
    WORD    wFirstScalingWidth;     //The width after first scaling.
    WORD	wFirstScalingHeight;    //The height after first scaling.
    PJPEGSCALING pJPEGScaling;     //The setting of JPEG scaling.
} JPEGDECODE, *PJPEGDECODE;


//Export functions
void HALJPEG_Initial(void);
void HALJPEG_Reset(void);
void HALJPEG_ParseHeader(BYTE bParseType, BYTE bDecodeBuf);
void HALJPEG_Decode(void);
BYTE HALJPEG_GetZoomFactor(void);
BYTE HALJPEG_Zoom(BYTE bAction);
BYTE HALJPEG_TransformOperation(BYTE bPreviousResult, BYTE bCurrentOperation);
void HALJPEG_Operation(void);
BYTE HALJPEG_Status(BYTE bStatusType);
void HALJPEG_Abort(void);
BYTE HALJPEG_Display(BYTE bFrameBuf);
void HALJPEG_ChangeTVMode(void);
void HALJPEG_CompleteEffectImmediately(BYTE bFrameBuf);
void HALJPEG_ConfigFrame(BYTE bMode); // LLY2.22, add to config JPEG frame buffer.
void HALJPEG_SetDisplay(void);

//Internal functions
void _HALJPEG_GetFitWidthOrHeight(void);
BYTE _HALJPEG_GetRotateStatus(void);
void _HALJPEG_ResetClipRegionToFullBuf(void);
void _HALJPEG_CalculateScaling(BYTE bZoomMode);
void _HALJPEG_FirstScaling(void);
BYTE _HALJPEG_TransformPanAction(BYTE bAction);
void _HALJPEG_DoZoomPanReScaling(void);
WORD _HALJPEG_GetPanBoundary (BYTE bAction);
void _HALJPEG_ForceTo8Multiple(WORD *wValue);
void _HALJPEG_Clip(void);
void _HALJPEG_ReScaling(void);
void _HALJPEG_ProtectBufWidth(WORD *wValue);
void _HALJPEG_ProtectBufHeight(WORD *wValue);
void _HALJPEG_CalculateRandomEffect(void);
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
void HALJPEG_EffectConfigure(BYTE wEffectConfigure);
void HALJPEG_CopyPictureToDisplayBuffer(BYTE bBufferSet);
#endif
void _HALJPEG_TuneForTVAttribute(void);

#ifdef SUPPORT_JPEG_FULL_SCREEN
VOID _HALJPEG_CalcSrcPicOffsetAndTransPicSize(PWORD pwSrcPicOffsetH, PWORD pwSrcPicOffsetV);
#endif //SUPPORT_JPEG_FULL_SCREEN

//Effect function
WORD HALJPEG_EffectFillBlock(DWORD dwBlockType,BYTE bFrameBuffIndex);
WORD HALJPEG_EffectColoration(DWORD dwBlockType,BYTE bFrameBuffIndex);
WORD HALJPEG_EffectBlind(DWORD dwBlockType,BYTE bFrameBuffIndex);
DWORD HALJPEG_ShowEffect(DWORD dwEffectType,DWORD dwStep,DWORD dwDispBufIndex);
VOID HALJPEG_SwitchDisplay(BYTE bFrameBuf);


//Extern variables
extern BYTE __bHALJPEGEffect;          //The effect of slide transition
extern JPEGSETTING __HALJPEGSetting;   //The JPEG setting before decoding/operating a JPEG. Note: The caller should fill the attributes.
extern WORD _wHALJPEGRealWidth;        //JPEG real width            
extern WORD _wHALJPEGRealHeight;       //JPEG real height    
extern WORD _wHALJPEGClipHStart;       //The horizontal starting position to clip the picture.
extern WORD _wHALJPEGClipVStart;       //The vertical starting position to clip the picture.
extern WORD _wHALJPEGClipWidth;        //The width to clip the picture.
extern WORD _wHALJPEGClipHeight;       //The height to clip the picture.
extern WORD _wHALJPEGScalingWidth;     //The width after scaling.
extern WORD _wHALJPEGScalingHeight;    //The height after scaling.

extern BYTE __wHALJPEGCurrentDisplayBuf;		//Keep current display buffer //CoCo2.77

#endif __HALJPEG_H__
