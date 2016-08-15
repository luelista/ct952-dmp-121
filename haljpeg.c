#include "winav.h"
#include "ctkav.h"
#include "jpegdec.h"
#include "jpegfmt.h"
#include "haljpeg.h"
#include "cc.h"
#include "hal.h"
#include "disp.h"
#include "utl.h"
#include "infofilter.h"
#include "mmanager.h"

#ifdef ECOS909
#if (PLATFORM == ECOS_OS)
#include "ecoscc.h"
#endif
#endif

//Temp variables
BYTE _bHALJPEGTemp, _bHALJPEGTemp1;
WORD _wHALJPEGTemp, _wHALJPEGTemp1;

//Global variables for HALJPEG module usage only
BYTE _bHALJPEGFit;              //The JPEG fit width or fit height?
BYTE _bHALJPEGScalingAll;       //Extract "all" image from original buffer or not.
BYTE _bHALJPEGZoomRate;         //Real zoom rate * 100
BYTE _bJPEGZoomMethod;          //We can use "clip" command to zoom JPEG or just scaling it.
BYTE _bJPEGIsEncodeMode;        //Chekc current whether is JPEG encoded mode
WORD _wHALJPEGScalingRate;      //Real scaling rate * 1000
WORD _wHALJPEGRealWidth;        //JPEG real width
WORD _wHALJPEGRealHeight;       //JPEG real height
WORD _wHALJPEGExtractHStart;    //The horizontal starting position to extract the JPEG from original buffer.
WORD _wHALJPEGExtractVStart;    //The vertical starting position to extract the JPEG from original buffer.
WORD _wHALJPEGExtractWidth;     //The width to extract the JPEG from original buffer.
WORD _wHALJPEGExtractHeight;    //The height to extract the JPEG from original buffer.
WORD _wHALJPEGClipHStart;       //The horizontal starting position to clip the picture.
WORD _wHALJPEGClipVStart;       //The vertical starting position to clip the picture.
WORD _wHALJPEGClipWidth;        //The width to clip the picture.
WORD _wHALJPEGClipHeight;       //The height to clip the picture.
WORD _wHALJPEGScalingWidth;     //The width after scaling.
WORD _wHALJPEGScalingHeight;    //The height after scaling.
JPEGSCALING _HALJPEGScaling;    //The Scaling structure.
JPEGDECODE _HALJPEGDecode;      //The Decode structure.
DWORD _dwHALJPEGCheckTime;
WORD _wHALJPEGBackupRealWidth;        //JPEG real width
WORD _wHALJPEGBackupRealHeight;        //JPEG real width


//Global variables for other module usage.
BYTE __bHALJPEGEffect;          //The effect of slide transition
BYTE _bHALJPEGRandomEffectType = 0; //Keep the real effect for random effect. Thus, we can complete the effect if need.
JPEGSETTING __HALJPEGSetting;   //The JPEG setting before decoding/operating a JPEG. Note: The caller should fill the attributes.
WIN_PARAMETER _JPEGClipWinParameter;
BYTE __wHALJPEGCurrentDisplayBuf;		//Keep current display buffer //CoCo2.77
//Senshong2.78: add effect block counter
DWORD _wHALJPEGBlockCounter;

extern DWORD __dwCoded_Picture_Width;
extern DWORD __dwCoded_Picture_Height;
extern  WIN_PARAMETER   MainVideoInfo;
extern BYTE        __bThumbnailMode;
extern  BYTE __dwMMJPEGPreview;

#ifdef SUPPORT_JPEGDEC_ON_PROC2
extern void _ChangeDSPCode(BYTE bType);
#endif //SUPPORT_JPEGDEC_ON_PROC2


// LLY2.21a create ...
//  ****************************************************************************************
//  Function    :   HALJPEG_ConfigFrame()
//  Description :   Used to config JPEG slide show and preview mode frame buffer address
//  Arguments   :   None
//  Return      :   None
//  ****************************************************************************************
void HALJPEG_ConfigFrame(BYTE bMode)
{
#ifndef JPG_SINGLE_FRAME_BUFFER
    DWORD dwDisplyBuff1;
#endif  // #ifndef JPG_SINGLE_FRAME_BUFFER

    extern WORD  __wFrameWidth;
    
    
#ifdef JPG_SINGLE_FRAME_BUFFER
    // LLY2.22, the display frame buffer setting are different between slide show and preview/ thumbnail
#ifdef JPEG_SINGLE_BUFFER_FULL_SCREEN
    JPEG_DisplayFullScreen(1);// 0: Not full screen,1: full screen
#endif

    JPEG_SetFrameBufferAddress(DS_FRAMEBUF_ST_SLIDESHOW, DS_FRAMEBUF_END_SLIDESHOW, DS_FRAMEBUF_ST_THUMBNAIL, DS_FRAMEBUF_END_THUMBNAIL, JPEG_DRAM_16M);
    if(bMode==HALJPEG_FRAME_BUFFER_SLIDE_SHOW)
    {
#ifdef JPEG_SINGLE_BUFFER_FULL_SCREEN
        DISP_FrameBufferSet(DS_FRAMEBUF_ST_SLIDESHOW, DS_FRAMEBUF_ST_SLIDESHOW, (DS_FRAMEBUF_END_SLIDESHOW - (HALJPEG_DISPLAY_BUF_WIDTH*HALJPEG_DISPLAY_BUF_HEIGHT*3/2)),DS_FRAMEBUF_ST_SLIDESHOW, 0x9D80);
#else
        DISP_FrameBufferSet(DS_FRAMEBUF_ST_SLIDESHOW, DS_FRAMEBUF_ST_SLIDESHOW, (DS_FRAMEBUF_END_SLIDESHOW - JPEG_DISP_SIZE_FOR_16M),DS_FRAMEBUF_ST_SLIDESHOW, Y_MAXSIZE_8C00);
        
#endif
    }
    else
    {
#ifdef JPEG_SINGLE_BUFFER_FULL_SCREEN
        DISP_FrameBufferSet(DS_FRAMEBUF_ST_THUMBNAIL, DS_FRAMEBUF_ST_THUMBNAIL, DS_FRAMEBUF_ST_THUMBNAIL, DS_FRAMEBUF_ST_THUMBNAIL, 0x9D80);
#else
        DISP_FrameBufferSet(DS_FRAMEBUF_ST_THUMBNAIL, DS_FRAMEBUF_ST_THUMBNAIL, DS_FRAMEBUF_ST_THUMBNAIL, DS_FRAMEBUF_ST_THUMBNAIL, Y_MAXSIZE_8C00);
#endif
        
    }
#else
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_64)
    dwDisplyBuff1 = JPEG_SetFrameBufferAddress(DS_FRAMEBUF_ST, (DS_FRAMEBUF_ST+DS_FRAMEBUF_SIZE-1), DS_JPG_ORG_BUFF_ST, DS_JPG_ORG_BUFF_END, JPEG_DRAM_64M);
#elif (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_32)
    dwDisplyBuff1 = JPEG_SetFrameBufferAddress(DS_FRAMEBUF_ST, (DS_FRAMEBUF_ST+DS_FRAMEBUF_SIZE-1), 0, 0, JPEG_DRAM_32M);
#endif
    DISP_FrameBufferSet(DS_FRAMEBUF_ST, dwDisplyBuff1, DS_FRAMEBUF_ST, DS_FRAMEBUF_ST, Y_MAXSIZE_A8C0);
#endif //JPG_SINGLE_FRAME_BUFFER
    DISP_FrameBufferMode(FALSE, FALSE);
    //Senshong2.78:reset block counter
    _wHALJPEGBlockCounter = 0;
    // LLY2.30, give frame buffer width base on global define
    __wFrameWidth = HALJPEG_DISPLAY_BUF_WIDTH;
}


//***************************************************************************
//  Function    :   HALJPEG_Initial
//  Abstract    :   Call this funciton when decoding the JPEG at first time.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void HALJPEG_Initial(void)
{
//senshong2.38a: add code to load jpeg decoder to processor 2,when playback jpeg
#ifdef SUPPORT_JPEGDEC_ON_PROC2
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    if (!__bMMJPEGAudioMode)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
    {
#ifndef CT909P_IC_SYSTEM
        JPEGDEC_ConfigProcessor(0);
        _ChangeDSPCode(HAL_VIDEO_JPG);
#endif //#ifndef CT909P_IC_SYSTEM
    }
#endif //SUPPORT_JPEGDEC_ON_PROC2

    //Stop video decoder, reset buffer and reset parser
    HAL_Reset(HAL_RESET_VIDEO);

    // wyc0.66, need to reset it and must put these code after HAL_Reset(HAL_RESET_VIDEO), otherwise JPG decode error after first picture.
    //__bLOGO = LOGO_JPEG;

    //__bAudioType = HAL_AUDIO_NONE; //Can't set the audio type because we may slide show with audio playing

    //Set A/V buffer
    //HAL_SetBuffer(HAL_SETBUFFER_AV, HAL_BUFFERMODE_JPEG);

    //Disable video.
    //Set the source (display buffer) for main video.
    //__dwCoded_Picture_Width = HALJPEG_DISPLAY_BUF_WIDTH;
    //__dwCoded_Picture_Height = HALJPEG_DISPLAY_BUF_HEIGHT;

    //DISP_MainVideoSet(&MainVideoInfo, __dwCoded_Picture_Width, __dwCoded_Picture_Height, DISP_NEXTVSYNC);

    // LLY2.21, remove turn-off main video procedure into UTL_Config_FrameBuffer()
    //DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE); //The Display should be closed after the main vedio has set.

    // LLY2.21, call UTL_Config_FrameBuffer() set desired frame buffer mode
    // LLY2.22, remove it since _UTL_ProgVideoOnly() will set it.
    //UTL_Config_FrameBuffer(UTL_FRAMEBUF_JPEG_SLIDE);

    // LLY2.21a, remove following procedure
    // Since, UTL_Config_FrameBuffer() will config the frame buffer mode.
#if 0
//    DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
    //UTL_DelayTime(COUNT_50_MSEC,1);
    OS_DelayTime(COUNT_50_MSEC); //CoCo1.20, reduce code size

    /*
    //Set display frame buffer.
    __dwDISPf0_addr = JPEGFMT_BUFF_DISP0;
    __dwDISPf1_addr = JPEGFMT_BUFF_DISP1;
    // Chuan0.66, __wDISPY_MaxSize declare as DWORD
    __dwDISPY_MaxSize = Y_MAXSIZE_A8C0;
    DISP_FrameBufferSet(DISP_MAINVIDEO);

    //CoCo0.90, set the address of the frame buffer for PIP
    //Set display frame buffer.
    __dwDISPpf0_addr = JPEGFMT_BUFF_DISP0;
    __dwDISPpf1_addr = JPEGFMT_BUFF_DISP1;
    DISP_FrameBufferSet(DISP_PIP);
    */

    // Chuan0.90, Set MainVideo & PIP at the same time.
    // wyc2.17-909S, make frame buff0 as slide show buffer and buff1 as thumbnail buffer when single buffer mode.
#ifdef JPG_SINGLE_FRAME_BUFFER
    dwDisplyBuff1 = JPEG_SetFrameBufferAddress(DS_FRAMEBUF_ST_SLIDESHOW, DS_FRAMEBUF_END_SLIDESHOW, DS_FRAMEBUF_ST_THUMBNAIL, DS_FRAMEBUF_END_THUMBNAIL, JPEG_DRAM_16M);
    //DISP_FrameBufferSet(DS_FRAMEBUF_ST_SLIDESHOW, DS_FRAMEBUF_ST_THUMBNAIL, DS_FRAMEBUF_ST_SLIDESHOW, DS_FRAMEBUF_ST_SLIDESHOW, Y_MAXSIZE_8C00);
    //DISP_FrameBufferSet(DS_FRAMEBUF_ST_SLIDESHOW,(DS_FRAMEBUF_ST_SLIDESHOW + JPEG_DISP_SIZE_FOR_16M), (DS_FRAMEBUF_ST_SLIDESHOW + (JPEG_DISP_SIZE_FOR_16M*2)),DS_FRAMEBUF_END_THUMBNAIL, Y_MAXSIZE_8C00);
    DISP_FrameBufferSet(DS_FRAMEBUF_ST_SLIDESHOW, DS_FRAMEBUF_ST_THUMBNAIL, (DS_FRAMEBUF_ST_SLIDESHOW + (JPEG_DISP_SIZE_FOR_16M*2)),DS_FRAMEBUF_ST_THUMBNAIL, Y_MAXSIZE_8C00);
#else
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_64)
    dwDisplyBuff1 = JPEG_SetFrameBufferAddress(DS_FRAMEBUF_ST, (DS_FRAMEBUF_ST+DS_FRAMEBUF_SIZE-1), DS_JPG_ORG_BUFF_ST, DS_JPG_ORG_BUFF_END, JPEG_DRAM_64M);
#elif (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_32)
    dwDisplyBuff1 = JPEG_SetFrameBufferAddress(DS_FRAMEBUF_ST, (DS_FRAMEBUF_ST+DS_FRAMEBUF_SIZE-1), 0, 0, JPEG_DRAM_32M);
#endif
    DISP_FrameBufferSet(DS_FRAMEBUF_ST, dwDisplyBuff1, DS_FRAMEBUF_ST, DS_FRAMEBUF_ST, Y_MAXSIZE_A8C0);
#endif //JPG_SINGLE_FRAME_BUFFER
    DISP_FrameBufferMode(FALSE, FALSE);
#endif  // #if 0

    //__DISPFrameInfo[0].bSrcFrame = 1;
    //__DISPFrameInfo[0].bDispFrame = 1;
    //__DISPFrameInfo[1].bSrcFrame = 1;
    //__DISPFrameInfo[1].bDispFrame = 1;

    /*
    __DISPFrameInfo[0].bSrcFrame = 1;
    __DISPFrameInfo[0].bDispFrame = 1;
    __DISPFrameInfo[0].bBFR_Mode = 0;
    __DISPFrameInfo[1].bSrcFrame = 1;
    __DISPFrameInfo[1].bDispFrame = 1;
    __DISPFrameInfo[1].bBFR_Mode = 0;
    __DISPFrameInfo[2].bSrcFrame = 1;
    __DISPFrameInfo[2].bDispFrame = 1;
    __DISPFrameInfo[2].bBFR_Mode = 0;
    */

    //Set the source (display buffer) for main video.
    /*
    __dwCoded_Picture_Width = HALJPEG_DISPLAY_BUF_WIDTH;
    __dwCoded_Picture_Height = HALJPEG_DISPLAY_BUF_HEIGHT;
    DISP_MainVideoSet(&MainVideoInfo, __dwCoded_Picture_Width, __dwCoded_Picture_Height, DISP_NEXTVSYNC);
    */
    HALJPEG_SetDisplay();


    //Do jpeg reset
    // Chuan0.67, Don't do jpeg_reset when no any bits in.
    //HALJPEG_Reset();
}

//***************************************************************************
//  Function    :   HALJPEG_Reset
//  Abstract    :   Call this function to do JPEG reset when decoding each JPEG file.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void HALJPEG_Reset(void)
{
    //Abort current JPEG decoder action first when JPEG reset.
    HALJPEG_Abort();
    OS_YieldThread();

    //Call JPEG Decoder's reset function.
    JPEG_Reset();

    _dwHALJPEGCheckTime=OS_GetSysTimer();
    while((OS_GetSysTimer()-_dwHALJPEGCheckTime) < COUNT_5_SEC)
    {
        if (HALJPEG_Status(HALJPEG_RESET) == JPEG_STATUS_OK)
        {
            break;
        }

        OS_YieldThread();
    }
}

//***************************************************************************
//  Function    :   HALJPEG_ParseHeader
//  Abstract    :   CCall this function to parse JPEG's header.
//  Arguments   :   bParseType: HALJPEG_PARSE_TYPE_NORMAL==>Parse the normal JPEG.
//              :               HALJPEG_PARSE_TYPE_THUMBNAIL==>Parse the thumbnail.
//              :               If there is no thumbnail in the file, parse the normal JPEG automatically.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void HALJPEG_ParseHeader(BYTE bParseType, BYTE bDecodeBuf)
{
	//CoCo2.76, put LOGO flag here to mean that the JPEG is starting to decode and background will be JPEG.
	//Don't put it in HALJPEG_Initial() because it may let LOGO re-decoded (screen will flash.) in some cases.
	__bLOGO = LOGO_JPEG;

    JPEG_SetSharpRatio(0,0);

	//senshong:get the mode that want to check current whether is encoded mode
    _bJPEGIsEncodeMode = 0;
    JPEG_ParseHeader(bParseType, bDecodeBuf);
    _bJPEGIsEncodeMode = bParseType;

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
    HALJPEG_SetDisplay();
#endif
}

//***************************************************************************
//  Function    :   HALJPEG_Decode
//  Abstract    :   Call this function to decode the JPEG file.
//  Arguments   :   bFrameBuf: Decode the JPEG to the frame buffer 0 or 1.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
#ifdef SUPPORT_HAL_JPEG_AUTO_RATIO
extern BYTE Get_Setup_Display_Type_16_9(void);
WORD Get_HalJpeg_Panel_Ratio(WORD  TempJPEGRealWidth,WORD TempJPEGRealHeight)
{    
	WORD WidthHeightRatio;
#ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
	extern BYTE DisplayLogoFlag;
	if(DisplayLogoFlag)
	{
		 WidthHeightRatio=TempJPEGRealWidth*10/TempJPEGRealHeight;
		 WidthHeightRatio=WidthHeightRatio*256+10;
		 return WidthHeightRatio;
	}
#endif
	if(Get_Setup_Display_Type_16_9())
	{
		WidthHeightRatio=16;
		WidthHeightRatio=WidthHeightRatio*256+9;
	}
	else
	{	
#ifdef SUPPORT_JPEG_AUTO_AND_FULL_SCREEN
		WidthHeightRatio=TempJPEGRealWidth*10/TempJPEGRealHeight;
		if((WidthHeightRatio>=13)&&(WidthHeightRatio<=19))
		{
			 WidthHeightRatio=WidthHeightRatio*256+10;
		}	
		else
		{
			WidthHeightRatio=16;
			WidthHeightRatio=WidthHeightRatio*256+9;
		}
#else
	WidthHeightRatio=4;
	WidthHeightRatio=WidthHeightRatio*256+3;
#endif
	}
	return WidthHeightRatio;
}
#endif
void HALJPEG_Decode(void)
{
#ifdef SUPPORT_JPEG_FULL_SCREEN
    WORD wSrcOffseH = 0;
    WORD wSrcOffseV = 0;
#endif //SUPPORT_JPEG_FULL_SCREEN

#ifdef CT950_STYLE
    _wHALJPEGBackupRealWidth = _wHALJPEGRealWidth;
    _wHALJPEGBackupRealHeight = _wHALJPEGRealHeight;
    if (_bJPEGIsEncodeMode != JPEG_PARSE_TYPE_THUMBNAIL)
    {
#ifdef SUPPORT_HAL_JPEG_AUTO_RATIO	
   		BYTE   HEIGHT_RATIO,WIDTH_RATIO;
		HEIGHT_RATIO=Get_HalJpeg_Panel_Ratio(_wHALJPEGRealWidth,_wHALJPEGRealHeight)&0x00ff;
		WIDTH_RATIO=(Get_HalJpeg_Panel_Ratio(_wHALJPEGRealWidth,_wHALJPEGRealHeight)>>8)&0x00ff;
		if (!_HALJPEG_GetRotateStatus())
        	{  
           	 	_wHALJPEGRealWidth = _wHALJPEGRealWidth*HEIGHT_RATIO*720/WIDTH_RATIO/480;
        	}
        	else
        	{
            		_wHALJPEGRealHeight = _wHALJPEGRealHeight*HEIGHT_RATIO*720/WIDTH_RATIO/480;
       	 }
#else	
#ifdef SUPPORT_JPEG_FULL_SCREEN
        _HALJPEG_CalcSrcPicOffsetAndTransPicSize(&wSrcOffseH, &wSrcOffseV);
#endif //SUPPORT_JPEG_FULL_SCREEN
        if (!_HALJPEG_GetRotateStatus())
        {  
            _wHALJPEGRealWidth = _wHALJPEGRealWidth*HALJPEG_PANEL_HEIGHT_RATIO*720/HALJPEG_PANEL_WIDTH_RATIO/480;
        }
        else
        {
            _wHALJPEGRealHeight = _wHALJPEGRealHeight*HALJPEG_PANEL_HEIGHT_RATIO*720/HALJPEG_PANEL_WIDTH_RATIO/480;
        }
#endif
    }

#endif
    //Do first scaling for 3 buffers to 1 buffer(the final original buffer).
    _HALJPEG_FirstScaling();

    _HALJPEG_CalculateScaling(VIEW_ZOOM100);

    //Set Scaling structure before calling JPEG
#ifdef SUPPORT_JPEG_FULL_SCREEN
    _wHALJPEGExtractHStart = wSrcOffseH;
    _wHALJPEGExtractVStart = wSrcOffseV;
#endif //SUPPORT_JPEG_FULL_SCREEN
    _HALJPEGScaling.wSrcExtractStartH = _wHALJPEGExtractHStart;
    _HALJPEGScaling.wSrcExtractStartV = _wHALJPEGExtractVStart;
    _HALJPEGScaling.wSrcExtractWidth = _wHALJPEGExtractWidth;
    _HALJPEGScaling.wSrcExtractHeight = _wHALJPEGExtractHeight;
    _HALJPEGScaling.wScalingWidth = _wHALJPEGScalingWidth;
    _HALJPEGScaling.wScalingHeight = _wHALJPEGScalingHeight;
    _HALJPEGScaling.pJPEGSetting = &__HALJPEGSetting;

    _HALJPEGDecode.wFirstScalingWidth = _wHALJPEGRealWidth;
    _HALJPEGDecode.wFirstScalingHeight = _wHALJPEGRealHeight;
    _HALJPEGDecode.pJPEGScaling = &_HALJPEGScaling;
    
#ifdef SUPPORT_COLOR_NORMALOZATOPN
	JPEGDEC_EnableColorTransform(TRUE);
#endif

    JPEG_Decode(&_HALJPEGDecode);

    //Calculate the random effect.
    _HALJPEG_CalculateRandomEffect();
    //Senshong2.78:  before display effect need reset block counter
    _wHALJPEGBlockCounter = 0;
}

//***************************************************************************
//  Function    :   HALJPEG_GetZoomFactor
//  Abstract    :   Call this function to get next zoom factor.
//  Arguments   :   none.
//  Return      :   return the zoom factor.
//  Side Effect :   none.
//  Notes       :   use __bZoomFactor as current zoom factor.
//***************************************************************************
BYTE HALJPEG_GetZoomFactor(void)
{
    if ((__bZoomFactor+1) > VIEW_ZOOM200)
        return VIEW_ZOOM25;
    else
        return (__bZoomFactor+1);
}

//***************************************************************************
//  Function    :   HALJPEG_Zoom
//  Abstract    :   Call this function to zoom in/zoom out or pan the JPEG.
//  Arguments   :   VIEW_NORAML==>fit to TV display.
//              :   VIEW_ZOOM25==>zoom out to 25% of the original image.
//              :   VIEW_ZOOM50==>zoom out to 50% of the original image.
//              :   VIEW_ZOOM100==>The original image size.
//              :   VIEW_ZOOM150==>zoom in to 150% of the original image.
//              :   VIEW_ZOOM200==>zoom in to 200% of the original image.
//              :   VIEW_ZOOM_LEFT==>pan left.
//              :   VIEW_ZOOM_RIGHT==>pan right.
//              :   VIEW_ZOOM_UP==>pan up.
//              :   VIEW_ZOOM_DOWN==>pan down.
//  Return      :   TRUE: The action is done.
//              :   FALSE: The action is invalid.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
BYTE HALJPEG_Zoom(BYTE bAction)
{
    if ((bAction == VIEW_ZOOM_LEFT) ||
        (bAction == VIEW_ZOOM_RIGHT) ||
        (bAction == VIEW_ZOOM_UP) ||
        (bAction == VIEW_ZOOM_DOWN))
    {
        if (__bZoomFactor <= VIEW_ZOOM100) //Doesn't need to pan
            return FALSE;

        if (!_bHALJPEGScalingAll) //Extract part of the image from the FOrg.
        {
            bAction = _HALJPEG_TransformPanAction(bAction);

            //Calculate the pan pixels
            _wHALJPEGTemp1 = (WORD)((DWORD)HALJPEG_PAN_PIXELS * (DWORD)_wHALJPEGExtractWidth / (DWORD)__HALJPEGSetting.wDisplayWidth);
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGTemp1);

            switch (bAction)
            {
            case VIEW_ZOOM_LEFT:
                if (_wHALJPEGExtractHStart == 0)
                    return HAL_ZOOM_FAIL;
                else if (_wHALJPEGExtractHStart <= _wHALJPEGTemp1)
                    _wHALJPEGExtractHStart = 0;
                else
                    _wHALJPEGExtractHStart -= _wHALJPEGTemp1;
                break;
            case VIEW_ZOOM_RIGHT:
                if (_wHALJPEGExtractHStart == (_wHALJPEGRealWidth - _wHALJPEGExtractWidth))
                    return HAL_ZOOM_FAIL;
                else if ((_wHALJPEGExtractHStart + _wHALJPEGExtractWidth + _wHALJPEGTemp1) >= _wHALJPEGRealWidth)
                    _wHALJPEGExtractHStart = _wHALJPEGRealWidth - _wHALJPEGExtractWidth;
                else
                    _wHALJPEGExtractHStart += _wHALJPEGTemp1;
                break;
            case VIEW_ZOOM_UP:
                if (_wHALJPEGExtractVStart == 0)
                    return FALSE;
                else if (_wHALJPEGExtractVStart <= _wHALJPEGTemp1)
                    _wHALJPEGExtractVStart = 0;
                else
                    _wHALJPEGExtractVStart -= _wHALJPEGTemp1;
                break;
            case VIEW_ZOOM_DOWN:
                if (_wHALJPEGExtractVStart == (_wHALJPEGRealHeight - _wHALJPEGExtractHeight))
                    return HAL_ZOOM_FAIL;
                else if ((_wHALJPEGExtractVStart + _wHALJPEGExtractHeight + _wHALJPEGTemp1) >= _wHALJPEGRealHeight)
                    _wHALJPEGExtractVStart = _wHALJPEGRealHeight - _wHALJPEGExtractHeight;
                else
                    _wHALJPEGExtractVStart += _wHALJPEGTemp1;
                break;
            default:
                break;
            }

            //Re-caling the JPEG in the original buffer to display buffer and show it.
            _HALJPEG_DoZoomPanReScaling();
        }
        else //Extract all image from original buffer. (zoom mode > _fScalingRate)
        {
            if (_wHALJPEGScalingRate < 1000)
            {
                //Calculate the pan pixels
                _wHALJPEGTemp1 = (WORD)((DWORD)HALJPEG_PAN_PIXELS*(DWORD)__HALJPEGSetting.wDisplayWidth/(DWORD)HALJPEG_DISPLAY_BUF_WIDTH*100L/(DWORD)_bHALJPEGZoomRate);
                //The "pan pixel" have to be 8's multipe?

                //Don't pan the black region
                //To get the rotating status. Rotating 90 degree==>TRUE. Else==>FALSE.
                _bHALJPEGTemp = _HALJPEG_GetRotateStatus();
                switch (bAction)
                {
                case VIEW_ZOOM_LEFT:
                case VIEW_ZOOM_RIGHT:
                    if (_bHALJPEGTemp)
                    {
                        if (_wHALJPEGClipWidth >= _wHALJPEGScalingHeight)
                            return FALSE;
                    }
                    else
                    {
                        if (_wHALJPEGClipWidth >= _wHALJPEGScalingWidth)
                            return FALSE;
                    }
                case VIEW_ZOOM_UP:
                case VIEW_ZOOM_DOWN:
                    if (_bHALJPEGTemp)
                    {
                        if (_wHALJPEGClipHeight >= _wHALJPEGScalingWidth)
                            return FALSE;
                    }
                    else
                    {
                        if (_wHALJPEGClipHeight >= _wHALJPEGScalingHeight)
                            return FALSE;
                    }
                default:
                    break;
                }
            }
            else
                _wHALJPEGTemp1 = HALJPEG_PAN_PIXELS * _wHALJPEGClipWidth / HALJPEG_DISPLAY_BUF_WIDTH;

            _wHALJPEGTemp = _HALJPEG_GetPanBoundary(bAction);
            switch (bAction)
            {
            case VIEW_ZOOM_LEFT:
                if (_wHALJPEGClipHStart == _wHALJPEGTemp)
                    return FALSE;
                else if (_wHALJPEGClipHStart < (_wHALJPEGTemp1 + _wHALJPEGTemp))
                    _wHALJPEGClipHStart = _wHALJPEGTemp;
                else
                    _wHALJPEGClipHStart -= _wHALJPEGTemp1;
                break;
            case VIEW_ZOOM_RIGHT:
                if (_wHALJPEGClipHStart == _wHALJPEGTemp)
                    return FALSE;
                else if ((_wHALJPEGClipHStart + _wHALJPEGTemp1) > _wHALJPEGTemp)
                    _wHALJPEGClipHStart = _wHALJPEGTemp;
                else
                    _wHALJPEGClipHStart += _wHALJPEGTemp1;
                break;
            case VIEW_ZOOM_UP:
                if (_wHALJPEGClipVStart == _wHALJPEGTemp)
                    return FALSE;
                else if (_wHALJPEGClipVStart < (_wHALJPEGTemp1 + _wHALJPEGTemp))
                    _wHALJPEGClipVStart = _wHALJPEGTemp;
                else
                    _wHALJPEGClipVStart -= _wHALJPEGTemp1;
                break;
            case VIEW_ZOOM_DOWN:
                if (_wHALJPEGClipVStart == _wHALJPEGTemp)
                    return FALSE;
                else if ((_wHALJPEGClipVStart + _wHALJPEGTemp1) > _wHALJPEGTemp)
                    _wHALJPEGClipVStart = _wHALJPEGTemp;
                else
                    _wHALJPEGClipVStart += _wHALJPEGTemp1;
                break;
            default:
                break;
            }

            //Set clip function here.
            _HALJPEG_Clip();

            /*
            //Re-caling the JPEG in the original buffer to display buffer and show it.
            _HALJPEG_DoZoomPanReScaling();
            */
        }
    }
    else
    {
        //_HALJPEG_CalculateScaling(bAction);

        //Re-caling the JPEG in the original buffer to display buffer and show it.
        _HALJPEG_DoZoomPanReScaling();
    }

    return TRUE;
}

//***************************************************************************
//  Function    :   HAL_JPEGTransformOperation
//  Abstract    :   Call this function to transform the result of "rotate" and "flip".
//  Arguments   :   bPreviousResult:  HALJPEG_OPERATION_I,
//              :                     HALJPEG_OPERATION_FH,
//              :                     HALJPEG_OPERATION_FV,
//              :                     HALJPEG_OPERATION_FHFV,
//              :                     HALJPEG_OPERATION_RC,
//              :                     HALJPEG_OPERATION_RCC,
//              :                     HALJPEG_OPERATION_RCFH,
//              :                     HALJPEG_OPERATION_RCFV
//              :   bCurrentOperation:  HALJPEG_OPERATION_FH,
//              :                       HALJPEG_OPERATION_FV,
//              :                       HALJPEG_OPERATION_RC,
//              :                       HALJPEG_OPERATION_RCC
//  Return      :   The operation.
//  Side Effect :   none.
//  Notes       :   Call this function to get operation before calling HALJPEG_Operation().
//***************************************************************************
BYTE HALJPEG_TransformOperation(BYTE bPreviousResult, BYTE bCurrentOperation)
{
    switch (bPreviousResult)
    {
    case HALJPEG_OPERATION_I:
        _bHALJPEGTemp = bCurrentOperation;
        break;
    case HALJPEG_OPERATION_RC:
        {
            switch (bCurrentOperation)
            {
            case HALJPEG_OPERATION_RC:
                _bHALJPEGTemp = HALJPEG_OPERATION_FHFV;
                break;
            case HALJPEG_OPERATION_RCC:
                _bHALJPEGTemp = HALJPEG_OPERATION_I;
                break;
            case HALJPEG_OPERATION_FH:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCFH;
                break;
            case HALJPEG_OPERATION_FV:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCFV;
                break;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_RCC:
        {
            switch (bCurrentOperation)
            {
            case HALJPEG_OPERATION_RC:
                _bHALJPEGTemp = HALJPEG_OPERATION_I;
                break;
            case HALJPEG_OPERATION_RCC:
                _bHALJPEGTemp = HALJPEG_OPERATION_FHFV;
                break;
            case HALJPEG_OPERATION_FH:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCFV;
                break;
            case HALJPEG_OPERATION_FV:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCFH;
                break;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_FH:
        {
            switch (bCurrentOperation)
            {
            case HALJPEG_OPERATION_RC:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCFV;
                break;
            case HALJPEG_OPERATION_RCC:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCFH;
                break;
            case HALJPEG_OPERATION_FH:
                _bHALJPEGTemp = HALJPEG_OPERATION_I;
                break;
            case HALJPEG_OPERATION_FV:
                _bHALJPEGTemp = HALJPEG_OPERATION_FHFV;
                break;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_FV:
        {
            switch (bCurrentOperation)
            {
            case HALJPEG_OPERATION_RC:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCFH;
                break;
            case HALJPEG_OPERATION_RCC:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCFV;
                break;
            case HALJPEG_OPERATION_FH:
                _bHALJPEGTemp = HALJPEG_OPERATION_FHFV;
                break;
            case HALJPEG_OPERATION_FV:
                _bHALJPEGTemp = HALJPEG_OPERATION_I;
                break;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_FHFV:
        {
            switch (bCurrentOperation)
            {
            case HALJPEG_OPERATION_RC:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCC;
                break;
            case HALJPEG_OPERATION_RCC:
                _bHALJPEGTemp = HALJPEG_OPERATION_RC;
                break;
            case HALJPEG_OPERATION_FH:
                _bHALJPEGTemp = HALJPEG_OPERATION_FV;
                break;
            case HALJPEG_OPERATION_FV:
                _bHALJPEGTemp = HALJPEG_OPERATION_FH;
                break;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_RCFH:
        {
            switch (bCurrentOperation)
            {
            case HALJPEG_OPERATION_RC:
                _bHALJPEGTemp = HALJPEG_OPERATION_FH;
                break;
            case HALJPEG_OPERATION_RCC:
                _bHALJPEGTemp = HALJPEG_OPERATION_FV;
                break;
            case HALJPEG_OPERATION_FH:
                _bHALJPEGTemp = HALJPEG_OPERATION_RC;
                break;
            case HALJPEG_OPERATION_FV:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCC;
                break;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_RCFV:
        {
            switch (bCurrentOperation)
            {
            case HALJPEG_OPERATION_RC:
                _bHALJPEGTemp = HALJPEG_OPERATION_FV;
                break;
            case HALJPEG_OPERATION_RCC:
                _bHALJPEGTemp = HALJPEG_OPERATION_FH;
                break;
            case HALJPEG_OPERATION_FH:
                _bHALJPEGTemp = HALJPEG_OPERATION_RCC;
                break;
            case HALJPEG_OPERATION_FV:
                _bHALJPEGTemp = HALJPEG_OPERATION_RC;
                break;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }

    return _bHALJPEGTemp;
}


//***************************************************************************
//  Function    :   HALJPEG_Operation
//  Abstract    :   Call this function for rotate and flip. This function can be
//              :   called only when JPEG has been decoded OK and just do the "rotate"
//              :   or "flip" operation. Note: should call HALJPEG_TransformOperation()
//              :   before calling this function.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void HALJPEG_Operation(void)
{
    _HALJPEG_ReScaling();

    DISP_Display(__HALJPEGSetting.bFrameBuffer, DISP_MAINVIDEO);
}

//***************************************************************************
//  Function    :   HALJPEG_Status
//  Abstract    :   Call this function to get the status of JPEG Decoder.
//  Arguments   :   bStatusType: HALJPEG_PARSE_HEADER/HALJPEG_DECODE/HALJPEG_PSFB
//  Return      :   HALJPEG_STATUS_UNFINISH/HALJPEG_STATUS_UNSUPPORT/
//              :   HALJPEG_STATUS_OK/HALJPEG_STATUS_FAIL
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
BYTE HALJPEG_Status(BYTE bStatusType)
{
    switch (bStatusType)
    {
    case HALJPEG_PARSE_HEADER:
        return JPEG_Status(JPEG_PARSE_HEADER);
    case HALJPEG_DECODE:
        return JPEG_Status(JPEG_DECODE);
    case HALJPEG_PSFB:                          //tommy0.67
        return JPEG_Status(JPEG_PSFB);
    case HALJPEG_RESET:
        return JPEG_Status(JPEG_RESET);
    case HALJPEG_OPERATION:
        return JPEG_Status(JPEG_OPERATION);
    default:
        break;
    }

    return HALJPEG_STATUS_FAIL;
}

//***************************************************************************
//  Function    :   HALJPEG_Abort
//  Abstract    :   Call this function to abort JPEG decoding.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void HALJPEG_Abort(void)
{
    JPEG_Abort();
}

//***************************************************************************
//  Function    :   HALJPEG_Display
//  Abstract    :   Call this function to display JPEG.
//  Arguments   :   bFrameBuf: Frame buffer 0 or 1.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
BYTE HALJPEG_Display(BYTE bFrameBuf)
{
    BYTE bJPEGEffect;

    //Call "slide show", "blending", "PIP" functions for effect.
    if (__bHALJPEGEffect != HALJPEG_EFFECT_NONE)
    {
        bJPEGEffect = __bHALJPEGEffect;
        if (bJPEGEffect == HALJPEG_EFFECT_RANDOM)
        {
            bJPEGEffect = _bHALJPEGRandomEffectType;
        }

        switch (bJPEGEffect)
        {
        case HALJPEG_EFFECT_COVER_UP:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_COVERUP, HALJPEG_EFFECT_LINE_V, bFrameBuf);
            break;
        case HALJPEG_EFFECT_COVER_DOWN:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_COVERDOWN, HALJPEG_EFFECT_LINE_V, bFrameBuf);
            break;
        case HALJPEG_EFFECT_COVER_LEFT:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_COVERLEFT, HALJPEG_EFFECT_LINE_H, bFrameBuf);
            break;
        case HALJPEG_EFFECT_COVER_RIGHT:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_COVERRIGHT, HALJPEG_EFFECT_LINE_H, bFrameBuf);
            break;
        case HALJPEG_EFFECT_UNCOVER_UP:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_UNCOVERUP, HALJPEG_EFFECT_LINE_V, bFrameBuf);
            break;
        case HALJPEG_EFFECT_UNCOVER_DOWN:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_UNCOVERDOWN, HALJPEG_EFFECT_LINE_V, bFrameBuf);
            break;
        case HALJPEG_EFFECT_UNCOVER_LEFT:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_UNCOVERLEFT, HALJPEG_EFFECT_LINE_H, bFrameBuf);
            break;
        case HALJPEG_EFFECT_UNCOVER_RIGHT:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_UNCOVERRIGHT, HALJPEG_EFFECT_LINE_H, bFrameBuf);
            break;
        case HALJPEG_EFFECT_WIPE_UP:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_WIPEUP, HALJPEG_EFFECT_LINE_V, bFrameBuf);
            break;
        case HALJPEG_EFFECT_WIPE_DOWN:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_WIPEDOWN, HALJPEG_EFFECT_LINE_V, bFrameBuf);
            break;
        case HALJPEG_EFFECT_WIPE_LEFT:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_WIPELEFT, HALJPEG_EFFECT_LINE_H, bFrameBuf);
            break;
        case HALJPEG_EFFECT_WIPE_RIGHT:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_WIPERIGHT, HALJPEG_EFFECT_LINE_H, bFrameBuf);
            break;
        case HALJPEG_EFFECT_PUSH_UP:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_PUSHUP, HALJPEG_EFFECT_LINE_V, bFrameBuf);
            break;
        case HALJPEG_EFFECT_PUSH_DOWN:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_PUSHDOWN, HALJPEG_EFFECT_LINE_V, bFrameBuf);
            break;
        case HALJPEG_EFFECT_PUSH_LEFT:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_PUSHLEFT, HALJPEG_EFFECT_LINE_H, bFrameBuf);
            break;
        case HALJPEG_EFFECT_PUSH_RIGHT:
            _bHALJPEGTemp = DISP_ShowEffect(SLIDESHOW_PUSHRIGHT, HALJPEG_EFFECT_LINE_H, bFrameBuf);
            break;
        case HALJPEG_EFFECT_BLIND_HOR:
        case HALJPEG_EFFECT_BLIND_VER:
        case HALJPEG_EFFECT_COLORATION_HOR:
        case HALJPEG_EFFECT_COLORATION_VER:
        case HALJPEG_EFFECT_BLOCK_1:
        case HALJPEG_EFFECT_BLOCK_2:
        case HALJPEG_EFFECT_BLOCK_3:
        case HALJPEG_EFFECT_BLOCK_4:
        case HALJPEG_EFFECT_BLOCK_5:
        case HALJPEG_EFFECT_BLOCK_6:
        case HALJPEG_EFFECT_BLOCK_7:
        case HALJPEG_EFFECT_BLOCK_8:
        case HALJPEG_EFFECT_BLOCK_9:
            _bHALJPEGTemp = HALJPEG_ShowEffect(bJPEGEffect,0,bFrameBuf);
            break;
        default:
            break;
        }
    }
    else
    {
        DISP_Display(bFrameBuf, DISP_MAINVIDEO);

        //CoCo0.95, Fix the bug about JPEG PIP is wrong.
        DISP_Display(bFrameBuf, DISP_PIP);

        DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);

        _bHALJPEGTemp = SLIDESHOW_RET_OK;
        _HALJPEG_Clip();
    }

	__wHALJPEGCurrentDisplayBuf = bFrameBuf; //CoCo2.77

    return _bHALJPEGTemp;
}

//***************************************************************************
//  Function    :   _HALJPEG_GetRotateStatus
//  Abstract    :   Get the rotating status of JPEG.
//  Arguments   :   bOperation: the operation.
//  Return          TRUE ==> The image is rotated 90 degree. (Rc or Rcc)
//              :   FALSE ==> The image is not rotated 90 degree.
//  Side Effect :   none.
//  Notes       :   none.
//***************************************************************************
BYTE _HALJPEG_GetRotateStatus(void)
{
    switch (__HALJPEGSetting.bOperation)
    {
    case HALJPEG_OPERATION_RC:
    case HALJPEG_OPERATION_RCC:
    case HALJPEG_OPERATION_RCFH:
    case HALJPEG_OPERATION_RCFV:
        return TRUE;
    default:
        break;
    }

    return FALSE;
}


//***************************************************************************
//  Function    :   _HALJPEG_GetFitWidthOrHeight
//  Abstract    :   Calculate the picture should fit width or height.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   none.
//***************************************************************************
void _HALJPEG_GetFitWidthOrHeight(void)
{
    _bHALJPEGTemp = _HALJPEG_GetRotateStatus();
    if (_bHALJPEGTemp) //rotate 90 degree
    {
        // if (_wHALJPEGRealWidth/_wHALJPEGRealHeight > __HALJPEGSetting.wDisplayHeight/__HALJPEGSetting.wDisplayWidth)==>fit height
        //fit width
        if ((DWORD)((DWORD)_wHALJPEGRealWidth * (DWORD)__HALJPEGSetting.wDisplayWidth) >
            (DWORD)((DWORD)_wHALJPEGRealHeight * (DWORD)__HALJPEGSetting.wDisplayHeight))
        {
            _bHALJPEGFit =  HALJPEG_DISPLAY_FIT_HEIGHT;
        }
        else //fit width
        {
            _bHALJPEGFit =  HALJPEG_DISPLAY_FIT_WIDTH;
        }
    }
    else
    {
        // if (_wHALJPEGRealWidth/_wHALJPEGRealHeight < __HALJPEGSetting.wDisplayWidth/__HALJPEGSetting.wDisplayHeight)==>fit height
        //fit height
        if ((DWORD)((DWORD)_wHALJPEGRealWidth * (DWORD)__HALJPEGSetting.wDisplayHeight) <
            (DWORD)((DWORD)_wHALJPEGRealHeight * (DWORD)__HALJPEGSetting.wDisplayWidth))
        {
            _bHALJPEGFit = HALJPEG_DISPLAY_FIT_HEIGHT;

        }
        else //fit width
        {
            _bHALJPEGFit = HALJPEG_DISPLAY_FIT_WIDTH;
        }
    }
}

//***************************************************************************
//  Function    :   _HALJPEG_ResetClipRegionToFullBuf
//  Abstract    :   Call this function to clip full region
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   none.
//***************************************************************************
void _HALJPEG_ResetClipRegionToFullBuf(void)
{
    _wHALJPEGClipHStart = 0;
    _wHALJPEGClipVStart = 0;
    _wHALJPEGClipWidth = HALJPEG_DISPLAY_BUF_WIDTH;
    _wHALJPEGClipHeight = HALJPEG_DISPLAY_BUF_HEIGHT;
}

//***************************************************************************
//  Function    :   _HALJPEG_CalculateScaling
//  Abstract    :   Calculate the scaling according to the zoom mode.
//  Arguments   :   bZoomMode ==> The paramter of zoom mode.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void _HALJPEG_CalculateScaling(BYTE bZoomMode)
{
    _bHALJPEGScalingAll = TRUE; //Extract all image from FOrg.

    _HALJPEG_GetFitWidthOrHeight();

    //To get the rotating status. Rotating 90 degree==>TRUE. Else==>FALSE.
    _bHALJPEGTemp = _HALJPEG_GetRotateStatus();

    //Calculate the scaling rate that we have to use the clip command
	if (_bHALJPEGTemp) //rotate
	{
		if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
			_wHALJPEGScalingRate = (WORD)((DWORD)_wHALJPEGRealHeight * 1000L / (DWORD)__HALJPEGSetting.wDisplayWidth);
		else //fit height
			_wHALJPEGScalingRate = (WORD)((DWORD)_wHALJPEGRealWidth * 1000L / (DWORD)__HALJPEGSetting.wDisplayHeight);
	}
	else
	{
		if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
			_wHALJPEGScalingRate = (WORD)((DWORD)_wHALJPEGRealWidth*1000L/(DWORD)__HALJPEGSetting.wDisplayWidth);
		else //fit height
			_wHALJPEGScalingRate = (WORD)((DWORD)_wHALJPEGRealHeight*1000L/(DWORD)__HALJPEGSetting.wDisplayHeight);
	}

    _wHALJPEGExtractWidth = _wHALJPEGRealWidth;   //Extract the width in the original buffer.
    _wHALJPEGExtractHeight = _wHALJPEGRealHeight; //Extract the Height in the original buffer.

    //Reset the clip region
    _HALJPEG_ResetClipRegionToFullBuf();

    switch (bZoomMode)
    {
    case VIEW_ZOOM25:
        _bHALJPEGTemp1 = 4;
        _bHALJPEGZoomRate = 25;
        break;
    case VIEW_ZOOM50:
        _bHALJPEGTemp1 = 2;
        _bHALJPEGZoomRate = 50;
        break;
    case VIEW_ZOOM100:
        _bHALJPEGTemp1 = 1;
        _bHALJPEGZoomRate = 100;
        break;
    case VIEW_ZOOM150:
        _bHALJPEGZoomRate = 150;
        break;
    case VIEW_ZOOM200:
        _bHALJPEGZoomRate = 200;
        break;
    default:
        break;
    }

    if (bZoomMode <= VIEW_ZOOM100)
    {
        if (_bHALJPEGTemp) //rotate
        {
            if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
                _wHALJPEGScalingHeight = __HALJPEGSetting.wDisplayWidth / _bHALJPEGTemp1;
            else
                _wHALJPEGScalingWidth = __HALJPEGSetting.wDisplayHeight / _bHALJPEGTemp1;
        }
        else
        {
            if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
                _wHALJPEGScalingWidth = __HALJPEGSetting.wDisplayWidth / _bHALJPEGTemp1;
            else
                _wHALJPEGScalingHeight = __HALJPEGSetting.wDisplayHeight / _bHALJPEGTemp1;
        }
    }
    else
    {
        //Need to re-decode when zoom in mode.
        if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
        {
            if (_bHALJPEGTemp) //rotate
                _wHALJPEGScalingHeight = HALJPEG_DISPLAY_BUF_WIDTH;
            else
                _wHALJPEGScalingWidth = HALJPEG_DISPLAY_BUF_WIDTH;
        }
        else //fit height
        {
            if (_bHALJPEGTemp) //rotate
                _wHALJPEGScalingWidth = HALJPEG_DISPLAY_BUF_HEIGHT;
            else
                _wHALJPEGScalingHeight = HALJPEG_DISPLAY_BUF_HEIGHT;
        }
    }

    //CoCo, Does CT909 JPEG Decoder need this?
    _HALJPEG_ForceTo8Multiple(&_wHALJPEGScalingWidth);
    _HALJPEG_ForceTo8Multiple(&_wHALJPEGScalingHeight);

    if (_bHALJPEGTemp) //Rotate 90
    {
        if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
        {
            _wHALJPEGScalingWidth = (WORD)((DWORD)_wHALJPEGScalingHeight * (DWORD)_wHALJPEGRealWidth / (DWORD)_wHALJPEGRealHeight);
            _HALJPEG_ProtectBufHeight(&_wHALJPEGScalingWidth);
        }
        else //fit height
        {
            _wHALJPEGScalingHeight = (WORD)((DWORD)_wHALJPEGScalingWidth * (DWORD)_wHALJPEGRealHeight / (DWORD)_wHALJPEGRealWidth);
            _HALJPEG_ProtectBufWidth(&_wHALJPEGScalingHeight);
        }
    }
    else
    {
        if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
        {
            _wHALJPEGScalingHeight = (WORD)((DWORD)_wHALJPEGScalingWidth * (DWORD)_wHALJPEGRealHeight / (DWORD)_wHALJPEGRealWidth);
            _HALJPEG_ProtectBufHeight(&_wHALJPEGScalingHeight);
        }
        else //fit height
        {
            _wHALJPEGScalingWidth = (WORD)((DWORD)_wHALJPEGScalingHeight * (DWORD)_wHALJPEGRealWidth / (DWORD)_wHALJPEGRealHeight);
            _HALJPEG_ProtectBufWidth(&_wHALJPEGScalingWidth);
        }
    }

    //CoCo, Does CT909 JPEG Decoder need this?
    _HALJPEG_ForceTo8Multiple(&_wHALJPEGScalingWidth);
    _HALJPEG_ForceTo8Multiple(&_wHALJPEGScalingHeight);

    _wHALJPEGExtractHStart = 0; //Extract the image from the starting position of the FOrg.
    _wHALJPEGExtractVStart = 0; //Extract the image from the starting position of the FOrg.

    if (bZoomMode > VIEW_ZOOM100)
    {
        //Calculate the width and height of the clip region.
        //if (_wHALJPEGScalingRate < (WORD)_bHALJPEGZoomRate*10)
        {
            //First, extract image by decode command. Second, clip it to fit the zoom rate.
            if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
            {
                if (_bHALJPEGTemp) //Rotate 90
                    _wHALJPEGClipWidth = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_WIDTH*(DWORD)HALJPEG_DISPLAY_BUF_WIDTH*100L/(DWORD)__HALJPEGSetting.wDisplayWidth/(DWORD)_bHALJPEGZoomRate);
                else
                    _wHALJPEGClipWidth = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_WIDTH*(DWORD)HALJPEG_DISPLAY_BUF_WIDTH*100L/(DWORD)__HALJPEGSetting.wDisplayWidth/(DWORD)_bHALJPEGZoomRate);

                _wHALJPEGClipHeight = (WORD)((DWORD)_wHALJPEGClipWidth * (DWORD)HALJPEG_DISPLAY_BUF_HEIGHT / (DWORD)HALJPEG_DISPLAY_BUF_WIDTH);
            }
            else
            {
                if (_bHALJPEGTemp) //Rotate 90
                    _wHALJPEGClipHeight = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_HEIGHT*(DWORD)HALJPEG_DISPLAY_BUF_HEIGHT*100L/(DWORD)__HALJPEGSetting.wDisplayHeight/(DWORD)_bHALJPEGZoomRate);
                else
                    _wHALJPEGClipHeight = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_HEIGHT*(DWORD)HALJPEG_DISPLAY_BUF_HEIGHT*100L/(DWORD)__HALJPEGSetting.wDisplayHeight/(DWORD)_bHALJPEGZoomRate);

                _wHALJPEGClipWidth = (WORD)((DWORD)_wHALJPEGClipHeight * (DWORD)HALJPEG_DISPLAY_BUF_WIDTH / (DWORD)HALJPEG_DISPLAY_BUF_HEIGHT);
            }

            _wHALJPEGClipHStart = (HALJPEG_DISPLAY_BUF_WIDTH - _wHALJPEGClipWidth) >> 1;
            _wHALJPEGClipVStart = (HALJPEG_DISPLAY_BUF_HEIGHT - _wHALJPEGClipHeight) >> 1;
        }
        /*
        else
        {
            _bHALJPEGScalingAll = FALSE;

            //Calculate the width and height of the picture that we want to extract from FOrg.
            if (_bHALJPEGTemp) //Rotate 90
            {
                if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
                {
                    if (((DWORD)__HALJPEGSetting.wDisplayWidth*(DWORD)_wHALJPEGRealWidth*(DWORD)_bHALJPEGZoomRate/100L/(DWORD)_wHALJPEGRealHeight) > (DWORD)HALJPEG_DISPLAY_BUF_HEIGHT)
                    {
                        //fill the whole screen to prevent the black region
                        _wHALJPEGScalingWidth = HALJPEG_DISPLAY_BUF_HEIGHT;
                        _wHALJPEGExtractWidth = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_HEIGHT*(DWORD)_wHALJPEGRealHeight/(DWORD)__HALJPEGSetting.wDisplayWidth*100L/(DWORD)_bHALJPEGZoomRate);
                    }
                    else
                        _wHALJPEGScalingWidth = (WORD)((DWORD)__HALJPEGSetting.wDisplayWidth*(DWORD)_wHALJPEGRealWidth / (DWORD)_wHALJPEGRealHeight*(DWORD)_bHALJPEGZoomRate/100L);

                    _wHALJPEGExtractHeight = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_WIDTH*(DWORD)_wHALJPEGRealHeight*100L/(DWORD)__HALJPEGSetting.wDisplayWidth/(DWORD)_bHALJPEGZoomRate);
                }
                else //fit height
                {
                    if (((DWORD)__HALJPEGSetting.wDisplayHeight*(DWORD)_wHALJPEGRealHeight*(DWORD)_bHALJPEGZoomRate/100L/(DWORD)_wHALJPEGRealWidth) > (DWORD)HALJPEG_DISPLAY_BUF_WIDTH)
                    {
                        //fill the whole screen to prevent the black region
                        _wHALJPEGExtractHeight = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_WIDTH*(DWORD)_wHALJPEGRealWidth*100L/(DWORD)__HALJPEGSetting.wDisplayHeight/(DWORD)_bHALJPEGZoomRate);
                        _wHALJPEGScalingHeight = HALJPEG_DISPLAY_BUF_WIDTH;
                    }
                    else
                        _wHALJPEGScalingHeight = (WORD)((DWORD)__HALJPEGSetting.wDisplayHeight*(DWORD)_wHALJPEGRealHeight/(DWORD)_wHALJPEGRealWidth*(DWORD)_bHALJPEGZoomRate/100L);

                    _wHALJPEGExtractWidth = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_HEIGHT*(DWORD)_wHALJPEGRealWidth*100L/(DWORD)__HALJPEGSetting.wDisplayHeight/(DWORD)_bHALJPEGZoomRate);
                }
            }
            else
            {
                if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
                {
                    _wHALJPEGExtractWidth = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_WIDTH*(DWORD)_wHALJPEGRealWidth*100L/(DWORD)__HALJPEGSetting.wDisplayWidth/(DWORD)_bHALJPEGZoomRate);
                    if (((DWORD)__HALJPEGSetting.wDisplayWidth*(DWORD)_wHALJPEGRealHeight*(DWORD)_bHALJPEGZoomRate/100L/(DWORD)_wHALJPEGRealWidth) > (DWORD)HALJPEG_DISPLAY_BUF_HEIGHT)
                    {
                        //fill the whole screen to prevent the black region
                        _wHALJPEGExtractHeight = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_HEIGHT*(DWORD)_wHALJPEGRealWidth*100L/(DWORD)__HALJPEGSetting.wDisplayWidth/(DWORD)_bHALJPEGZoomRate);
                        _wHALJPEGScalingHeight = HALJPEG_DISPLAY_BUF_HEIGHT;
                    }
                    else
                        _wHALJPEGScalingHeight = (WORD)((DWORD)__HALJPEGSetting.wDisplayWidth*(DWORD)_wHALJPEGRealHeight/(DWORD)_wHALJPEGRealWidth*(DWORD)_bHALJPEGZoomRate/100L);
                }
                else //fit height
                {
                    if (((DWORD)__HALJPEGSetting.wDisplayHeight*(DWORD)_wHALJPEGRealWidth*(DWORD)_bHALJPEGZoomRate/100L/(DWORD)_wHALJPEGRealHeight) > (DWORD)HALJPEG_DISPLAY_BUF_WIDTH)
                    {
                        //fill the whole screen to prevent the black region
                        _wHALJPEGExtractWidth = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_WIDTH*(DWORD)_wHALJPEGRealHeight*100L/(DWORD)__HALJPEGSetting.wDisplayHeight/(DWORD)_bHALJPEGZoomRate);
                        _wHALJPEGScalingWidth = HALJPEG_DISPLAY_BUF_WIDTH;
                    }
                    else
                        _wHALJPEGScalingWidth = (WORD)((DWORD)__HALJPEGSetting.wDisplayHeight*(DWORD)_wHALJPEGRealWidth/(DWORD)_wHALJPEGRealHeight*(DWORD)_bHALJPEGZoomRate/100L);

                    _wHALJPEGExtractHeight = (WORD)((DWORD)HALJPEG_DISPLAY_BUF_HEIGHT*(DWORD)_wHALJPEGRealHeight*100L/(DWORD)__HALJPEGSetting.wDisplayHeight/(DWORD)_bHALJPEGZoomRate);
                }
            }

            //CoCo, Does CT909 need it?
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGExtractWidth);
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGExtractHeight);
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGScalingWidth);
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGScalingHeight);

            //Set the starting position to extract the image from FOrg.
            _wHALJPEGExtractHStart = (_wHALJPEGRealWidth - _wHALJPEGExtractWidth) >> 1;
            _wHALJPEGExtractVStart = (_wHALJPEGRealHeight - _wHALJPEGExtractHeight) >> 1;

            //CoCo, Does CT909 need it?
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGExtractHStart);
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGExtractVStart);
        }
        */
    }

#ifndef CT950_STYLE
    _HALJPEG_TuneForTVAttribute();
#elif (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_16)
    _HALJPEG_TuneForTVAttribute();
#endif //#ifndef CT950_STYLE

    if (_wHALJPEGScalingWidth < HALJPEG_DISPLAY_BUF_WIDTH)
    {
        if ((HALJPEG_DISPLAY_BUF_WIDTH-_wHALJPEGScalingWidth) <= HALJPEG_ENLARGE_WIDTH_PIXELS)
        {
            _wHALJPEGScalingWidth = HALJPEG_DISPLAY_BUF_WIDTH;
        }
    }

    if (_wHALJPEGScalingHeight < HALJPEG_DISPLAY_BUF_HEIGHT)
    {
        if ((HALJPEG_DISPLAY_BUF_HEIGHT-_wHALJPEGScalingHeight) <= HALJPEG_ENLARGE_HEIGHT_PIXELS)
        {
            _wHALJPEGScalingHeight = HALJPEG_DISPLAY_BUF_HEIGHT;
        }
    }
}

//***************************************************************************
//  Function    :   _HALJPEG_FirstScaling
//  Abstract    :   Do the first scaling to tell RISC scaling 3 frame buffer(big original buffer)
//              :   to one frame buffer. (720x480)
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   Modify _wHALJPEGRealWidth and _wHALJPEGRealHeight.
//***************************************************************************
void _HALJPEG_FirstScaling(void)
{
    //fit height
    if ((DWORD)((DWORD)_wHALJPEGRealWidth * (DWORD)HALJPEG_FIRST_SCALING_HEIGHT) <
        (DWORD)((DWORD)_wHALJPEGRealHeight * (DWORD)HALJPEG_FIRST_SCALING_WIDTH))
    {
        if (_wHALJPEGRealHeight > HALJPEG_FIRST_SCALING_HEIGHT)
        {
            //fit height
            //Can't exchange the sequence of calculating the width and height
            //Real width must be 8's multiple.
            _wHALJPEGRealWidth = (WORD)((DWORD)_wHALJPEGRealWidth * (DWORD)HALJPEG_FIRST_SCALING_HEIGHT / (DWORD)_wHALJPEGRealHeight);
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGRealWidth);
            _wHALJPEGRealHeight =  HALJPEG_FIRST_SCALING_HEIGHT;
        }
    }
    else //fit width
    {
        if (_wHALJPEGRealWidth > HALJPEG_FIRST_SCALING_WIDTH)
        {
            //Can't exchange the sequence of calculating the width and height
            //Real height must be 2's multiple.
            _wHALJPEGRealHeight = (WORD)((DWORD)_wHALJPEGRealHeight * (DWORD)HALJPEG_FIRST_SCALING_WIDTH / (DWORD)_wHALJPEGRealWidth);
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGRealHeight);

            _wHALJPEGRealWidth =  HALJPEG_FIRST_SCALING_WIDTH;
        }
    }
}

//***************************************************************************
//  Function    :   _HALJPEG_TransformPanAction
//  Abstract    :   This function will transform the action of the pan according to
//              :   current operation.
//  Arguments   :   bAction, the action of pan.
//  Return      :   The action of pan.
//  Side Effect :   none.
//  Notes       :   use __HALJPEGSetting.bOperation to judge which pan action should be transformed.
//***************************************************************************
BYTE _HALJPEG_TransformPanAction(BYTE bAction)
{
    switch (__HALJPEGSetting.bOperation)
    {
    case HALJPEG_OPERATION_RC:
        {
            switch (bAction)
            {
            case VIEW_ZOOM_LEFT:
                return VIEW_ZOOM_DOWN;
            case VIEW_ZOOM_RIGHT:
                return VIEW_ZOOM_UP;
            case VIEW_ZOOM_UP:
                return VIEW_ZOOM_LEFT;
            case VIEW_ZOOM_DOWN:
                return VIEW_ZOOM_RIGHT;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_FHFV:
        {
            switch (bAction)
            {
            case VIEW_ZOOM_LEFT:
                return VIEW_ZOOM_RIGHT;
            case VIEW_ZOOM_RIGHT:
                return VIEW_ZOOM_LEFT;
            case VIEW_ZOOM_UP:
                return VIEW_ZOOM_DOWN;
            case VIEW_ZOOM_DOWN:
                return VIEW_ZOOM_UP;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_RCC:
        {
            switch (bAction)
            {
            case VIEW_ZOOM_LEFT:
                return VIEW_ZOOM_UP;
            case VIEW_ZOOM_RIGHT:
                return VIEW_ZOOM_DOWN;
            case VIEW_ZOOM_UP:
                return VIEW_ZOOM_RIGHT;
            case VIEW_ZOOM_DOWN:
                return VIEW_ZOOM_LEFT;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_RCFH:
        {
            switch (bAction)
            {
            case VIEW_ZOOM_LEFT:
                return VIEW_ZOOM_UP;
            case VIEW_ZOOM_RIGHT:
                return VIEW_ZOOM_DOWN;
            case VIEW_ZOOM_UP:
                return VIEW_ZOOM_LEFT;
            case VIEW_ZOOM_DOWN:
                return VIEW_ZOOM_RIGHT;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_FH:
        {
            switch (bAction)
            {
            case VIEW_ZOOM_LEFT:
                return VIEW_ZOOM_RIGHT;
            case VIEW_ZOOM_RIGHT:
                return VIEW_ZOOM_LEFT;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_FV:
        {
            switch (bAction)
            {
            case VIEW_ZOOM_UP:
                return VIEW_ZOOM_DOWN;
            case VIEW_ZOOM_DOWN:
                return VIEW_ZOOM_UP;
            default:
                break;
            }
        }
        break;
    case HALJPEG_OPERATION_RCFV:
        {
            switch (bAction)
            {
            case VIEW_ZOOM_LEFT:
                return VIEW_ZOOM_DOWN;
            case VIEW_ZOOM_RIGHT:
                return VIEW_ZOOM_UP;
            case VIEW_ZOOM_UP:
                return VIEW_ZOOM_RIGHT;
            case VIEW_ZOOM_DOWN:
                return VIEW_ZOOM_LEFT;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }

    return bAction;
}


//***************************************************************************
//  Function    :   _HALJPEG_DoZoomPanReScaling
//  Abstract    :   This function will do zoom/pan action by re-scaling.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void _HALJPEG_DoZoomPanReScaling(void)
{
    _HALJPEG_ReScaling();

    _HALJPEG_Clip();

    DISP_Display(__HALJPEGSetting.bFrameBuffer, DISP_MAINVIDEO);

    //CoCo0.95, Fix the bug about JPEG PIP is wrong
    DISP_Display(__HALJPEGSetting.bFrameBuffer, DISP_PIP);
}

//***************************************************************************
//  Function    :   _HALJPEG_GetPanBoundary
//  Abstract    :   call this function to get left, right, up and down boundary.
//  Arguments   :   bAction,
//              :   VIEW_ZOOM_RIGHT ==> get right boundary
//              :   VIEW_ZOOM_LEFT ==> get left boundary
//              :   VIEW_ZOOM_UP ==> get up boundary
//              :   VIEW_ZOOM_DOWN ==> get down boundary
//  Return      :   Horizontal/Vertical position.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
WORD _HALJPEG_GetPanBoundary (BYTE bAction)
{
    //To get the rotating status. Rotating 90 degree==>TRUE. Else==>FALSE.
    _bHALJPEGTemp = _HALJPEG_GetRotateStatus();

    switch (bAction)
    {
    case VIEW_ZOOM_LEFT:
        if (!_bHALJPEGScalingAll || (_bHALJPEGScalingAll &&!_bHALJPEGTemp)) //Extract part of the image from the FOrg || not rotate
        {
            if (_bHALJPEGScalingAll && (_wHALJPEGClipWidth >= _wHALJPEGScalingWidth))
            {
                _wHALJPEGTemp = _wHALJPEGClipHStart;
            }
            else
            {
                _wHALJPEGTemp = HALJPEG_DISPLAY_BUF_WIDTH -_wHALJPEGScalingWidth;
                _wHALJPEGTemp >>= 1;
            }
        }
        else
		{
			if (_wHALJPEGClipWidth >= _wHALJPEGScalingHeight)
				_wHALJPEGTemp = _wHALJPEGClipHStart;
			else
			{
				_wHALJPEGTemp = HALJPEG_DISPLAY_BUF_WIDTH - _wHALJPEGScalingHeight;
				_wHALJPEGTemp >>= 1;
			}
		}
        break;
    case VIEW_ZOOM_RIGHT:
        if (!_bHALJPEGScalingAll || (_bHALJPEGScalingAll &&!_bHALJPEGTemp)) //Extract part of the image from the FOrg.
        {
            if (_bHALJPEGScalingAll && (_wHALJPEGClipWidth >= _wHALJPEGScalingWidth))
            {
                _wHALJPEGTemp = _wHALJPEGClipHStart;
            }
            else
            {
                _wHALJPEGTemp = HALJPEG_DISPLAY_BUF_WIDTH - _wHALJPEGScalingWidth;
                _wHALJPEGTemp >>= 1;
                _wHALJPEGTemp = _wHALJPEGTemp + _wHALJPEGScalingWidth - _wHALJPEGClipWidth;
            }
        }
        else
        {
            if (_wHALJPEGClipWidth >= _wHALJPEGScalingHeight)
                _wHALJPEGTemp = _wHALJPEGClipHStart;
            else
            {
                _wHALJPEGTemp = HALJPEG_DISPLAY_BUF_WIDTH - _wHALJPEGScalingHeight;
                _wHALJPEGTemp >>= 1;
                _wHALJPEGTemp = _wHALJPEGTemp + _wHALJPEGScalingHeight - _wHALJPEGClipWidth;
            }
        }
        break;
    case VIEW_ZOOM_UP:
        if (!_bHALJPEGScalingAll || (_bHALJPEGScalingAll &&!_bHALJPEGTemp)) //Extract part of the image from the FOrg.
        {
            if (_bHALJPEGScalingAll && (_wHALJPEGClipHeight >= _wHALJPEGScalingHeight))
            {
                _wHALJPEGTemp = _wHALJPEGClipVStart;
            }
            else
            {
                _wHALJPEGTemp = HALJPEG_DISPLAY_BUF_HEIGHT - _wHALJPEGScalingHeight;
                _wHALJPEGTemp >>= 1;
            }
        }
        else
		{
			if (_wHALJPEGClipHeight >= _wHALJPEGScalingWidth)
				_wHALJPEGTemp = _wHALJPEGClipVStart;
			else
			{
				_wHALJPEGTemp = HALJPEG_DISPLAY_BUF_HEIGHT - _wHALJPEGScalingWidth;
				_wHALJPEGTemp >>= 1;
			}
		}
        break;
    case VIEW_ZOOM_DOWN:
        if (!_bHALJPEGScalingAll || (_bHALJPEGScalingAll &&!_bHALJPEGTemp)) //Extract part of the image from the FOrg.
        {
            if (_bHALJPEGScalingAll && (_wHALJPEGClipHeight >= _HALJPEGScaling.wScalingHeight))
            {
                _wHALJPEGTemp = _wHALJPEGClipVStart;
            }
            else
            {
                _wHALJPEGTemp = HALJPEG_DISPLAY_BUF_HEIGHT - _wHALJPEGScalingHeight;
                _wHALJPEGTemp >>= 1;
                _wHALJPEGTemp = _wHALJPEGTemp + _wHALJPEGScalingHeight - _wHALJPEGClipHeight;
            }
        }
        else
		{
			if (_wHALJPEGClipHeight >= _wHALJPEGScalingWidth)
				_wHALJPEGTemp = _wHALJPEGClipVStart;
			else
			{
				_wHALJPEGTemp = HALJPEG_DISPLAY_BUF_HEIGHT - _wHALJPEGScalingWidth;
				_wHALJPEGTemp >>= 1;
				_wHALJPEGTemp = _wHALJPEGTemp + _wHALJPEGScalingWidth - _wHALJPEGClipHeight;
			}
		}
        break;
    }

    return _wHALJPEGTemp;
}


//***************************************************************************
//  Function    :   _HALJPEG_ForceTo8Multiple
//  Abstract    :   Transfer the value to 8's multiple.
//  Arguments   :   wValue: The value you want to transfer to.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   none.
//***************************************************************************
void _HALJPEG_ForceTo8Multiple(WORD *wValue)
{
    *wValue = (*wValue + 7) >> 3;
    *wValue <<= 3;
}

//***************************************************************************
//  Function    :   _HALJPEG_Clip
//  Abstract    :   Call DISP's clip function to do clip.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   none.
//***************************************************************************
void _HALJPEG_Clip(void)
{
    _JPEGClipWinParameter.sHStart = _wHALJPEGClipHStart;
    _JPEGClipWinParameter.wHWidth = _wHALJPEGClipWidth;
    _JPEGClipWinParameter.sVStart = _wHALJPEGClipVStart;
    _JPEGClipWinParameter.wVWidth = _wHALJPEGClipHeight;
    _JPEGClipWinParameter.bEnable = TRUE;

    //Call "clip" function
    // Chuan0.66, remove two parameters.
    DISP_Clip(&_JPEGClipWinParameter, /*__HALJPEGSetting.bFrameBuffer*/DISP_NEXTVSYNC, DISP_MAINVIDEO);
}

//***************************************************************************
//  Function    :   HALJPEG_ChangeTVMode
//  Abstract    :   Call this function when TV mode is changed.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   none.
//***************************************************************************
void HALJPEG_ChangeTVMode(void)
{
    /*
    __dwCoded_Picture_Width = HALJPEG_DISPLAY_BUF_WIDTH;
    __dwCoded_Picture_Height = HALJPEG_DISPLAY_BUF_HEIGHT;

    DISP_MainVideoSet(&MainVideoInfo, __dwCoded_Picture_Width, __dwCoded_Picture_Height, DISP_NEXTVSYNC);
    */

    HALJPEG_SetDisplay();

    _HALJPEG_Clip();
}

//***************************************************************************
//  Function    :   _HALJPEG_ReScaling
//  Abstract    :   This function will do zoom/pan action by re-scaling.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void _HALJPEG_ReScaling(void)
{
#ifdef SUPPORT_JPEG_FULL_SCREEN
    WORD wSrcOffsetH = 0;
    WORD wSrcOffsetV = 0;
#endif //SUPPORT_JPEG_FULL_SCREEN

#ifndef JPG_SINGLE_FRAME_BUFFER	
    //__HALJPEGSetting.bFrameBuffer = !__HALJPEGSetting.bFrameBuffer;
	__wHALJPEGCurrentDisplayBuf = !__wHALJPEGCurrentDisplayBuf;
	__HALJPEGSetting.bFrameBuffer = __wHALJPEGCurrentDisplayBuf;
#endif

#ifdef CT950_STYLE
    _wHALJPEGRealWidth = _wHALJPEGBackupRealWidth;
    _wHALJPEGRealHeight = _wHALJPEGBackupRealHeight;
#ifdef SUPPORT_HAL_JPEG_AUTO_RATIO	
{
   	BYTE   HEIGHT_RATIO,WIDTH_RATIO;
	HEIGHT_RATIO=Get_HalJpeg_Panel_Ratio(_wHALJPEGRealWidth,_wHALJPEGRealHeight)&0x00ff;
	WIDTH_RATIO=(Get_HalJpeg_Panel_Ratio(_wHALJPEGRealWidth,_wHALJPEGRealHeight)>>8)&0x00ff;
	if (!_HALJPEG_GetRotateStatus())
        {  
            _wHALJPEGRealWidth = _wHALJPEGRealWidth*HEIGHT_RATIO*720/WIDTH_RATIO/480;
        }
        else
        {
            _wHALJPEGRealHeight = _wHALJPEGRealHeight*HEIGHT_RATIO*720/WIDTH_RATIO/480;
        }
}
#else	
#ifdef SUPPORT_JPEG_FULL_SCREEN
    _HALJPEG_CalcSrcPicOffsetAndTransPicSize(&wSrcOffsetH, &wSrcOffsetV);
#endif //#ifdef SUPPORT_JPEG_FULL_SCREEN
    if (!_HALJPEG_GetRotateStatus())
    {  
        _wHALJPEGRealWidth = _wHALJPEGRealWidth*HALJPEG_PANEL_HEIGHT_RATIO*720/HALJPEG_PANEL_WIDTH_RATIO/480;
    }
    else
    {
        _wHALJPEGRealHeight = _wHALJPEGRealHeight*HALJPEG_PANEL_HEIGHT_RATIO*720/HALJPEG_PANEL_WIDTH_RATIO/480;
    }
#endif
    _HALJPEG_FirstScaling();
#endif //#ifdef CT950_STYLE

    _HALJPEG_CalculateScaling(__bZoomFactor);
#ifdef SUPPORT_JPEG_FULL_SCREEN
    _wHALJPEGExtractHStart = wSrcOffsetH;
    _wHALJPEGExtractVStart = wSrcOffsetV;
#endif //SUPPORT_JPEG_FULL_SCREEN
    //Set Scaling structure before calling JPEG
    _HALJPEGScaling.wSrcExtractStartH = _wHALJPEGExtractHStart;
    _HALJPEGScaling.wSrcExtractStartV = _wHALJPEGExtractVStart;
    _HALJPEGScaling.wSrcExtractWidth = _wHALJPEGExtractWidth;
    _HALJPEGScaling.wSrcExtractHeight = _wHALJPEGExtractHeight;
    _HALJPEGScaling.wScalingWidth = _wHALJPEGScalingWidth;
    _HALJPEGScaling.wScalingHeight = _wHALJPEGScalingHeight;
    _HALJPEGScaling.pJPEGSetting = &__HALJPEGSetting;

    //Call JPEG Decoder's "Operate" function to do it.
    JPEG_Operate(&_HALJPEGScaling);

    _dwHALJPEGCheckTime=OS_GetSysTimer();

    while((OS_GetSysTimer()-_dwHALJPEGCheckTime) < COUNT_500_MSEC)
    {
        if (HALJPEG_Status(JPEG_OPERATION) == JPEG_STATUS_OK)
        {
            break;
        }

        OS_YieldThread();
    }
}


//***************************************************************************
//  Function    :   _HALJPEG_ProtectBufWidth
//  Abstract    :   This function will protect the width of JPEG.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void _HALJPEG_ProtectBufWidth(WORD *wValue)
{
    if (*wValue > HALJPEG_DISPLAY_BUF_WIDTH)
        *wValue = HALJPEG_DISPLAY_BUF_WIDTH;
}

//***************************************************************************
//  Function    :   _HALJPEG_ProtectBufHeight
//  Abstract    :   This function will protect the height of JPEG.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void _HALJPEG_ProtectBufHeight(WORD *wValue)
{
    if (*wValue > HALJPEG_DISPLAY_BUF_HEIGHT)
        *wValue = HALJPEG_DISPLAY_BUF_HEIGHT;
}


//***************************************************************************
//  Function    :   HALJPEG_CompleteEffectImmediately
//  Abstract    :   Call this function to display JPEG.
//  Arguments   :   bFrameBuf: Frame buffer 0 or 1.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void HALJPEG_CompleteEffectImmediately(BYTE bFrameBuf)
{
    BYTE bJPEGEffect;

    //Call "slide show", "blending", "PIP" functions for effect.
    if (__bHALJPEGEffect != HALJPEG_EFFECT_NONE)
    {
        bJPEGEffect = __bHALJPEGEffect;
        if (__bHALJPEGEffect == HALJPEG_EFFECT_RANDOM)
        {
            bJPEGEffect = _bHALJPEGRandomEffectType;
        }

        switch (bJPEGEffect)
        {
        case HALJPEG_EFFECT_COVER_UP:
            DISP_ShowEffect(SLIDESHOW_COVERUP, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_COVER_DOWN:
            DISP_ShowEffect(SLIDESHOW_COVERDOWN, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_COVER_LEFT:
            DISP_ShowEffect(SLIDESHOW_COVERLEFT, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_COVER_RIGHT:
            DISP_ShowEffect(SLIDESHOW_COVERRIGHT, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_UNCOVER_UP:
            DISP_ShowEffect(SLIDESHOW_UNCOVERUP, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_UNCOVER_DOWN:
            DISP_ShowEffect(SLIDESHOW_UNCOVERDOWN, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_UNCOVER_LEFT:
            DISP_ShowEffect(SLIDESHOW_UNCOVERLEFT, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_UNCOVER_RIGHT:
            DISP_ShowEffect(SLIDESHOW_UNCOVERRIGHT, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_WIPE_UP:
            DISP_ShowEffect(SLIDESHOW_WIPEUP, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_WIPE_DOWN:
            DISP_ShowEffect(SLIDESHOW_WIPEDOWN, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_WIPE_LEFT:
            DISP_ShowEffect(SLIDESHOW_WIPELEFT, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_WIPE_RIGHT:
            DISP_ShowEffect(SLIDESHOW_WIPERIGHT, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_PUSH_UP:
            DISP_ShowEffect(SLIDESHOW_PUSHUP, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_PUSH_DOWN:
            DISP_ShowEffect(SLIDESHOW_PUSHDOWN, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_PUSH_LEFT:
            DISP_ShowEffect(SLIDESHOW_PUSHLEFT, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_PUSH_RIGHT:
            DISP_ShowEffect(SLIDESHOW_PUSHRIGHT, 0xFFFFFFFF, bFrameBuf);
            break;
        case HALJPEG_EFFECT_BLIND_HOR:
        case HALJPEG_EFFECT_BLIND_VER:
        case HALJPEG_EFFECT_COLORATION_HOR:
        case HALJPEG_EFFECT_COLORATION_VER:
        case HALJPEG_EFFECT_BLOCK_1:
        case HALJPEG_EFFECT_BLOCK_2:
        case HALJPEG_EFFECT_BLOCK_3:
        case HALJPEG_EFFECT_BLOCK_4:
        case HALJPEG_EFFECT_BLOCK_5:
        case HALJPEG_EFFECT_BLOCK_6:
        case HALJPEG_EFFECT_BLOCK_7:
        case HALJPEG_EFFECT_BLOCK_8:
        case HALJPEG_EFFECT_BLOCK_9:
            HALJPEG_ShowEffect(bJPEGEffect,0xFFFFFFFF,bFrameBuf);
            break;
        
        default:
            break;
        }
    }
}

#ifdef JPG_SINGLE_FRAME_BUFFER
//***************************************************************************
//  Function    :   HALJPEG_EffectConfigure
//  Abstract    :   Call this function to Configure JPEG Effect.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void HALJPEG_EffectConfigure(BYTE wEffectConfigure)
{
		JPEGDEC_EffectConfigure(wEffectConfigure);
}
//***************************************************************************
//  Function    :   HALJPEG_CopyPictureToDisplayBuffer
//  Abstract    :   Call this function to let JPEG copy picture to display frame.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void HALJPEG_CopyPictureToDisplayBuffer(BYTE bBufferSet)
{
    JPEGDEC_CopyPictureToDisplayBuffer(bBufferSet);
    OS_DelayTime(COUNT_50_MSEC);
}
#endif

//***************************************************************************
//  Function    :   HALJPEG_SetDisplay
//  Abstract    :   Set main video for Display.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void HALJPEG_SetDisplay(void)
{
    WIN_PARAMETER   JPEGMainVideoInfo = MainVideoInfo;
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
#ifndef JPEG_SINGLE_BUFFER_FULL_SCREEN
    if ((__wDiscType & CDROM_M1) && (__bAttrPlay == ATTR_JPG) && (!__bThumbnailMode) && (__dwMMJPEGPreview == FALSE))
    {
        if (MainVideoInfo.wVWidth == 480)
        {
            JPEGMainVideoInfo.wHWidth = MainVideoInfo.wHWidth -80;
            JPEGMainVideoInfo.wVWidth = MainVideoInfo.wVWidth - 32;
            JPEGMainVideoInfo.sHStart = 147;
            JPEGMainVideoInfo.sVStart  = 65;
            JPEGMainVideoInfo.bEnable = MainVideoInfo.bEnable;
        }
        else
        {
            JPEGMainVideoInfo.wHWidth = MainVideoInfo.wHWidth -80;
            JPEGMainVideoInfo.wVWidth = MainVideoInfo.wVWidth;
            JPEGMainVideoInfo.sHStart = 156;
            JPEGMainVideoInfo.sVStart  = MainVideoInfo.sVStart;
            JPEGMainVideoInfo.bEnable = MainVideoInfo.bEnable;
        }
    }
#endif //JPEG_SINGLE_BUFFER_FULL_SCREEN
#endif

    __dwCoded_Picture_Width = HALJPEG_DISPLAY_BUF_WIDTH;
    __dwCoded_Picture_Height = HALJPEG_DISPLAY_BUF_HEIGHT;
    
    //senshong:when encoding mode, we need to disable display
    if(_bJPEGIsEncodeMode == JPEG_PARSE_TYPE_THUMB_ENCODE)
    {
        JPEGMainVideoInfo.bEnable = FALSE;
    }

    DISP_MainVideoSet(&JPEGMainVideoInfo, __dwCoded_Picture_Width, __dwCoded_Picture_Height, DISP_NEXTVSYNC);
    ///Need to wait 1 VSYNC
    DISP_DelayVSYNC(1);
}

void _HALJPEG_CalculateRandomEffect(void)
{
    BYTE bJPEGEffect;

    //Note: Before calling DISP_ShowEffect() function, we need to calculate the effect time.
    //For example, if we set the "effect num" for

    //Call "slide show", "blending", "PIP" functions for effect.
    if (__bHALJPEGEffect == HALJPEG_EFFECT_RANDOM)
    {
        bJPEGEffect = (__wPlayItem+_bHALJPEGRandomEffectType) % HALJPEG_RANDOM_EFFECT_BASE;
        if (bJPEGEffect == HALJPEG_EFFECT_NONE)
        {
            bJPEGEffect++;
        }
        
        _bHALJPEGRandomEffectType = bJPEGEffect;
    }
}

void _HALJPEG_TuneForTVAttribute(void)
{
    //CoCo0.90, tune the width/height ratio for TV attribute.
    if (_HALJPEG_GetRotateStatus())
    {
        if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
        {
            _wHALJPEGScalingWidth = _wHALJPEGScalingWidth*100/HALJPEG_TUNE_DISPLAY_RATIO_FOR_TV;
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGScalingWidth);
        }
        else //fit height
        {
            _wHALJPEGScalingHeight = _wHALJPEGScalingHeight*HALJPEG_TUNE_DISPLAY_RATIO_FOR_TV/100;
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGScalingHeight);

            if (_wHALJPEGScalingHeight > __HALJPEGSetting.wDisplayWidth)
                _wHALJPEGScalingHeight = __HALJPEGSetting.wDisplayWidth;
        }
    }
    else
    {
        if (_bHALJPEGFit == HALJPEG_DISPLAY_FIT_WIDTH) // fit width
        {
            _wHALJPEGScalingHeight = _wHALJPEGScalingHeight*100/HALJPEG_TUNE_DISPLAY_RATIO_FOR_TV;
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGScalingHeight);
        }
        else //fit height
        {
            _wHALJPEGScalingWidth = _wHALJPEGScalingWidth*HALJPEG_TUNE_DISPLAY_RATIO_FOR_TV/100;
            _HALJPEG_ForceTo8Multiple(&_wHALJPEGScalingWidth);

            if (_wHALJPEGScalingWidth > __HALJPEGSetting.wDisplayWidth)
                _wHALJPEGScalingWidth = __HALJPEGSetting.wDisplayWidth;
        }
    }
}

VOID HALJPEG_SwitchDisplay(BYTE bFrameBuf)
{
        DISP_Display(bFrameBuf, DISP_MAINVIDEO);
        DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
        DISP_DelayVSYNC(1);
        
}

DWORD HALJPEG_ShowEffect(DWORD dwEffectType,DWORD dwStep,DWORD dwDispBufIndex)
{
    WORD wEffectStatus;

    if(_wHALJPEGBlockCounter == 0 && dwStep == 0xffffffff)
    {
        return SLIDESHOW_RET_PREPARE;
    }

    if(_wHALJPEGBlockCounter!= 0 && dwStep == 0xffffffff)
    {
        HALJPEG_SwitchDisplay(dwDispBufIndex);
        return SLIDESHOW_RET_PREPARE;
    }

    
    switch(dwEffectType)
    {
        case HALJPEG_EFFECT_BLIND_HOR:
            wEffectStatus = HALJPEG_EffectBlind(0,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_BLIND_VER:
            wEffectStatus = HALJPEG_EffectBlind(1,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_COLORATION_HOR:
            wEffectStatus = HALJPEG_EffectColoration(0,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_COLORATION_VER:
            wEffectStatus = HALJPEG_EffectColoration(1,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_BLOCK_1:
            wEffectStatus = HALJPEG_EffectFillBlock(HALJPEG_EFFECT_BLOCK_1,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_BLOCK_2:
            wEffectStatus = HALJPEG_EffectFillBlock(HALJPEG_EFFECT_BLOCK_2,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_BLOCK_3:
            wEffectStatus = HALJPEG_EffectFillBlock(HALJPEG_EFFECT_BLOCK_3,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_BLOCK_4:
            wEffectStatus = HALJPEG_EffectFillBlock(HALJPEG_EFFECT_BLOCK_4,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_BLOCK_5:
            wEffectStatus = HALJPEG_EffectFillBlock(HALJPEG_EFFECT_BLOCK_5,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_BLOCK_6:
            wEffectStatus = HALJPEG_EffectFillBlock(HALJPEG_EFFECT_BLOCK_6,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_BLOCK_7:
            wEffectStatus = HALJPEG_EffectFillBlock(HALJPEG_EFFECT_BLOCK_7,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_BLOCK_8:
            wEffectStatus = HALJPEG_EffectFillBlock(HALJPEG_EFFECT_BLOCK_8,dwDispBufIndex);
            break;
        case HALJPEG_EFFECT_BLOCK_9:
            wEffectStatus = HALJPEG_EffectFillBlock(HALJPEG_EFFECT_BLOCK_9,dwDispBufIndex);
            break;
        default:
            wEffectStatus = SLIDESHOW_RET_PREPARE;
            break;
    }

    return wEffectStatus;
}



WORD HALJPEG_EffectFillBlock(DWORD dwBlockType,BYTE bFrameBuffIndex)
{
    static DWORD a, b, n, z;
    static WORD  wMaxW,wMaxH,wMinW,wMinH,wBoundaryW,wBoundaryH;
    static WORD  maxx, maxy;
    static DWORD dwBeginTime;
    static BOOL  bHAlternativeFlip;
    static DWORD dwHAlternativeFlip_Step;
    
    if(_wHALJPEGBlockCounter == 0)
    {
        WORD wEffectTable[9][5] =
        {
            {0x04,0xf0,0x6b,0x01,0x01},{0xb4,0x78,0x07,0x01,0x01},{0x08,0x08,0xb51,0x01,0x0b},{0x10,0x10,0x51d,0x01,0x0f},
            {0x04,0x4,0x477b,0x01,0x09},{0x10,0x78,0x85,0x01,0x02},{0x08,0x08,0x7b1,0x00,0x15},{0x04,0x04,0x513d,0x00,0x33},
            {0x08,0x04,0x2935,0x00,0x21}
        };
        WORD wEffectType = ( 9 - (HALJPEG_EFFECT_BLOCK_9 - dwBlockType + 1));
        
        wMaxW = wEffectTable[wEffectType][0];
        wMaxH = wEffectTable[wEffectType][1];
        a = wEffectTable[wEffectType][2];
        bHAlternativeFlip = wEffectTable[wEffectType][3];
        dwHAlternativeFlip_Step = wEffectTable[wEffectType][4];
        maxx = 720 / wMaxW;
        maxy = 480 / wMaxH;
        n = maxx * maxy;
        wMinW = (HALJPEG_DISPLAY_BUF_WIDTH % wMaxW);
        wMinH = (HALJPEG_DISPLAY_BUF_HEIGHT % wMaxH);
        wBoundaryW = HALJPEG_DISPLAY_BUF_WIDTH - wMinW;
        wBoundaryH = HALJPEG_DISPLAY_BUF_HEIGHT - wMinH;
        b = (wEffectType)*128;
        dwBeginTime = OS_GetSysTimer();
    }
    if(_wHALJPEGBlockCounter < n)
    {
        WORD wWidthSize,wHeightSize;
        DWORD x, y,dwCoordX,dwCoordY;
        DWORD dwCurrTime = OS_GetSysTimer();

        z = (a * _wHALJPEGBlockCounter + b) % n;
        x = z % maxx;
        y = z / maxx;

        if(bHAlternativeFlip)
        {   // alternative horizontal flip
            if((y / dwHAlternativeFlip_Step) & 1)
            {
                x = maxx - 1 - x;
            }
        }
        dwCoordX = x * wMaxW;
        dwCoordY = y * wMaxH;
        wWidthSize = wMaxW;
        wHeightSize = wMaxH;

        if(dwCoordX >= wBoundaryW || dwCoordY >= wBoundaryH)
        {
            if(dwCoordX == wBoundaryW && wMinW !=0 )
            {
                wWidthSize = wMinW;
            }
            if(dwCoordY == wBoundaryH && wMinH !=0)
            {
                wHeightSize = wMinH;
            }
            if(dwCoordX > wBoundaryW || dwCoordY > wBoundaryH)
            {
                ++_wHALJPEGBlockCounter;
                return SLIDESHOW_RET_UNFINISH;
            }
        }
        
        JPUUTIL_FillBlock((WORD)bFrameBuffIndex, dwCoordX, dwCoordY, wWidthSize, wHeightSize , BLOCK_ALL);

        if(dwCurrTime - dwBeginTime <  HALJPEG_TRANSITION_TIME)
        {
            DWORD dwDelay = ( HALJPEG_TRANSITION_TIME - (dwCurrTime - dwBeginTime)) / (n - _wHALJPEGBlockCounter);
            OS_DelayTime(dwDelay);
        }

        _wHALJPEGBlockCounter++;
        return SLIDESHOW_RET_UNFINISH;
    }
    else
    {
        HALJPEG_SwitchDisplay(bFrameBuffIndex);
        return SLIDESHOW_RET_OK;
    }

    
}


WORD HALJPEG_EffectColoration(DWORD dwBlockType,BYTE bFrameBuffIndex)
{
    static WORD wBlckNum;
    

    if(_wHALJPEGBlockCounter == 0)
    {
        if(dwBlockType == 0)
        {
            wBlckNum = HALJPEG_DISPLAY_BUF_WIDTH / 4;
        }
        else
        {
            wBlckNum = HALJPEG_DISPLAY_BUF_HEIGHT / 4;
        }
    }
    
    if(_wHALJPEGBlockCounter < (wBlckNum << 1))
    {
        WORD wCoordV,wCoordH;
        WORD wBlockSizeV,wBlockSizeH;

        if(dwBlockType == 0)
        {
            wCoordH = _wHALJPEGBlockCounter % wBlckNum;
            wCoordV = 0;
            wBlockSizeH = 4;
            wBlockSizeV = HALJPEG_DISPLAY_BUF_HEIGHT;
            if(_wHALJPEGBlockCounter >= wBlckNum)
            {
                wCoordH = wBlckNum - 1 - wCoordH;
            }

        }
        else
        {   
            wCoordV = _wHALJPEGBlockCounter % wBlckNum;
            wCoordH = 0;
            wBlockSizeH = HALJPEG_DISPLAY_BUF_WIDTH;
            wBlockSizeV = 4;
            if(_wHALJPEGBlockCounter >= wBlckNum)
            {
                wCoordV = wBlckNum -1 - wCoordV;
            }
        }

        if(_wHALJPEGBlockCounter < wBlckNum )
        {
            JPUUTIL_FillBlockColor((WORD)bFrameBuffIndex, wCoordH *4, wCoordV *4, wBlockSizeH, wBlockSizeV, 0x8080);
            JPUUTIL_FillBlock((WORD)bFrameBuffIndex, wCoordH *4, wCoordV *4, wBlockSizeH, wBlockSizeV, BLOCK_Y);
        }
        else
        {
            JPUUTIL_FillBlock((WORD)bFrameBuffIndex,wCoordH *4,wCoordV *4,wBlockSizeH,wBlockSizeV,BLOCK_UV);
        }

        _wHALJPEGBlockCounter++;
        OS_DelayTime(COUNT_10_MSEC);
        return SLIDESHOW_RET_UNFINISH;
    }
    else
    {
        HALJPEG_SwitchDisplay(bFrameBuffIndex);
        _wHALJPEGBlockCounter = 0;
    }
    return SLIDESHOW_RET_OK;
    
    
}


WORD HALJPEG_EffectBlind(DWORD dwBlockType,BYTE bFrameBuffIndex)
{
    static WORD wBlckNr;
    if(_wHALJPEGBlockCounter == 0)
    {
        if(dwBlockType == 0)
        {
            wBlckNr = HALJPEG_DISPLAY_BUF_WIDTH / 8;
        }
        else
        {
            wBlckNr = HALJPEG_DISPLAY_BUF_HEIGHT / 8;
        }
    }
    
    if(_wHALJPEGBlockCounter < wBlckNr)
    {
        WORD wCoordH,wCoordV;
        WORD wBlockSizeV,wBlockSizeH;

        if(dwBlockType == 0)
        {
            wCoordH = (41*_wHALJPEGBlockCounter + 10) % wBlckNr;
            wCoordV = 0;
            wBlockSizeH = 8;
            wBlockSizeV = HALJPEG_DISPLAY_BUF_HEIGHT;

        }
        else
        {   
            wCoordV = (41*_wHALJPEGBlockCounter + 10) % wBlckNr;
            wCoordH = 0;
            wBlockSizeH = HALJPEG_DISPLAY_BUF_WIDTH;
            wBlockSizeV = 8;
        }
            
        
        JPUUTIL_FillBlock((WORD)bFrameBuffIndex,wCoordH *8,wCoordV *8,wBlockSizeH,wBlockSizeV,BLOCK_ALL);
        _wHALJPEGBlockCounter++;
        OS_DelayTime(COUNT_10_MSEC);
        return SLIDESHOW_RET_UNFINISH;
        
    }
    else
    {
        HALJPEG_SwitchDisplay(bFrameBuffIndex);
        _wHALJPEGBlockCounter = 0;
    }
    return SLIDESHOW_RET_OK;
    
    
}


#ifdef SUPPORT_JPEG_FULL_SCREEN
VOID _HALJPEG_CalcSrcPicOffsetAndTransPicSize(PWORD pwSrcPicOffsetH,PWORD pwSrcPicOffsetV)
{
    //senshong:Calculate the real picture's ratio
    DWORD dwPicRatio = ((DWORD)_wHALJPEGRealWidth *1000 / (DWORD)_wHALJPEGRealHeight);
    volatile WORD wTransDispWidth = (WORD)((HALJPEG_DISPLAY_BUF_HEIGHT / HALJPEG_PANEL_HEIGHT_RATIO) * HALJPEG_PANEL_WIDTH_RATIO);
    *pwSrcPicOffsetH = 0;
    *pwSrcPicOffsetV = 0;
    //senshong:we need to transform real picture size ratio for 16:9 or 4:3 panel
    //when picture's width is larger than picture's height, we check the ratio whether neet to transform picture's size
    if(dwPicRatio > 1000)
    {
        WORD wSrcWidth = 0;
        WORD wSrcHeight = 0;
        WORD wTransRealWidth =_wHALJPEGRealWidth*HALJPEG_PANEL_HEIGHT_RATIO*720/HALJPEG_PANEL_WIDTH_RATIO/480;
        BYTE bDispFit;
        //fit height
        if ((DWORD)((DWORD)wTransRealWidth * (DWORD)__HALJPEGSetting.wDisplayHeight) <=
            (DWORD)((DWORD)_wHALJPEGRealHeight * (DWORD)__HALJPEGSetting.wDisplayWidth))
        {
            bDispFit = HALJPEG_DISPLAY_FIT_HEIGHT;

        }
        else //fit width
        {
            bDispFit = HALJPEG_DISPLAY_FIT_WIDTH;
        }
        //Query the original buffer picture's width and height
        JPEGDEC_QuerySrcPicSize(&wSrcWidth,&wSrcHeight);
        if(bDispFit == HALJPEG_DISPLAY_FIT_WIDTH )
        {
#ifdef HALJPEG_CLIP_FIT_DISP_WIDTH
            WORD wTransWidth =  (WORD)((HALJPEG_DISPLAY_BUF_HEIGHT * dwPicRatio)/1000);
            WORD wSrcWidthoffset = 0;
            if(wTransWidth >= HALJPEG_DISPLAY_BUF_WIDTH && dwPicRatio <= 2000)
            {
                
                //Query the original buffer picture's width and height
                JPEGDEC_QuerySrcPicSize(&wSrcWidth,&wSrcHeight);
                wSrcWidthoffset = (wTransWidth - HALJPEG_DISPLAY_BUF_WIDTH) /2;
                wSrcWidthoffset = ((WORD)(((DWORD)(wSrcWidthoffset * wSrcWidth))/ wTransWidth))&0xfffc;
                //senshong:according to panel ratio, we transform the real picture's size to 16:9 or 4:3 
                _wHALJPEGRealHeight = (WORD)((DWORD)((DWORD)_wHALJPEGRealWidth *HALJPEG_PANEL_HEIGHT_RATIO)/HALJPEG_PANEL_WIDTH_RATIO);
            }
            JPEGDEC_SrcPicOffset(wSrcWidthoffset,0);
            *pwSrcPicOffsetH = wSrcWidthoffset;
#endif
        }
        else
        {
#ifdef HALJPEG_CLIP_FIT_DISP_HEIGHT
            WORD wTransHeight,wSrcHeightoffset;
            wSrcHeightoffset = 0;
            //senshong:If display width need full screen, we need to calculate the best ratio for height according to display width.
            wTransHeight = (WORD)((wTransDispWidth * 1000)/ dwPicRatio);
            //senshong:Check the height whether cut the size about source picture's height
            //When the height is larger than display height, we need to cut the source picture's height.
            if(wTransHeight >= HALJPEG_DISPLAY_BUF_HEIGHT)
            {
                //In order to let display picture is at the middle of display frame, we need to calculate the height offset of original buffer picture.
                wSrcHeightoffset = (wTransHeight - HALJPEG_DISPLAY_BUF_HEIGHT)/2;
                wSrcHeightoffset = ((WORD)(((DWORD)(wSrcHeightoffset * wSrcHeight))/wTransHeight))&0xfffc;
                //senshong:according to panel ratio, we transform the real picture's size to 16:9 or 4:3 
                _wHALJPEGRealHeight = (WORD)((DWORD)((DWORD)_wHALJPEGRealWidth *HALJPEG_PANEL_HEIGHT_RATIO)/HALJPEG_PANEL_WIDTH_RATIO);
            }
            JPEGDEC_SrcPicOffset(0,wSrcHeightoffset);
            *pwSrcPicOffsetV = wSrcHeightoffset;
#endif
        }
    }
}

#endif //SUPPORT_JPEG_FULL_SCREEN
