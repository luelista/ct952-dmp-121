#include "winav.h"
#include "comdef.h"
#include "osd.h"
#include "utl.h"
#include "cc.h"
#include "linear.h"
#include "gdi.h"
#include "osdss.h"
#include "disp.h"
#include "setup.h"
#include "hal.h"
#include "srcfilter.h"
#include "osdnd.h"
// LLY2.51, include "romld.h" temporally since current JPEG address is get from romld
// And, can remove it after real saving JPEG within SPI flash feature is ok
#include "romld.h"
#ifdef CT950_STYLE//CoCo2.38
#include "clock.h"
#include "alarm.h"
#endif //CT950_STYLE

#ifdef SUPPORT_RADIO_TUNER
#include "radio.h"
#endif

#ifdef SUPPORT_CHAR_SUBPICTURE
#ifdef EX_SP_USE_SPU_BUFFER
#include "parser.h"
#include "subpict.h"
#endif
#endif

#include "thumb.h"
#include "char_subpict.h"
#include "jpegdec.h"

//WORD aSSDVDLOGOTest[] = {8, CHAR_C, CHAR_h, CHAR_e,  CHAR_e, CHAR_r, CHAR_t, CHAR_e, CHAR_k};
#ifndef NO_SCREEN_SAVER
DWORD aOSDSSDVDLOGOPalette[] = {OSDSS_PALETTE_COLOR_DVD_LOGO_1, OSDSS_PALETTE_COLOR_DVD_LOGO_2, OSDSS_PALETTE_COLOR_DVD_LOGO_3, OSDSS_PALETTE_COLOR_DVD_LOGO_4,
                                OSDSS_PALETTE_COLOR_DVD_LOGO_5, OSDSS_PALETTE_COLOR_DVD_LOGO_6, OSDSS_PALETTE_COLOR_DVD_LOGO_7, OSDSS_PALETTE_COLOR_DVD_LOGO_8};
//CoCo1.06, define the bitmap and palette for screen saver.
DWORD _aSCRSaver[]=
{
#ifndef NO_SCREEN_SAVER

#ifndef OSDSS_JPEG_SCREEN_SAVER
#include "ScrSaver.txt"
#else
0x0, 0x00000002, 0x0, 0x0, // dummy, for no screen saver case. Set Size  to 0, so no data will be download to DRAM
#endif //OSDSS_JPEG_SCREEN_SAVER

#else

0x0, 0x00000002, 0x0, 0x0, // dummy, for no screen saver case. Set Size  to 0, so no data will be download to DRAM

#endif
};

DWORD * _aScrrenSaverBMPArray[1] = {_aSCRSaver};

DWORD Palette_ScreenSaver[]=
{
#ifndef OSDSS_JPEG_SCREEN_SAVER
#include "PalScr.txt"
#endif //OSDSS_JPEG_SCREEN_SAVER
};

GDI_PALETTE_INFO    __PaletteInfo_ScreenSaver = {95,  PAL_ADDR, Palette_ScreenSaver};
#endif

PARM_DRAW_STRING _OSDSSStringAttr;
DWORD __dwOSDSSCheckTime = 0xFFFFFFFF;
DWORD __dwOSDServoCheckTime = 0xFFFFFFFF;
PARM_DRAW_BITMAP _OSDSSDrawBitmap;
POSITION _bOSDSSCurrentPos;
DWORD _dwOSDSSMoveTime;
DWORD _dwOSDSSColorChangeTime;
DWORD _dwOSDSSMixRatioChangeTime;
DWORD __dwOSDSSCheckNOData = 0xFFFFFFFF;
BYTE _bOSDSSScreenSaverMode = FALSE;
DWORD __dwOSDServoCheckNOData = 0xFFFFFFFF;
BYTE _bOSDSSInvertH;
BYTE _bOSDSSInvertV;
BYTE _bOSDSSMixRatioInvert;
BYTE _bOSDSSMixRatio;
BYTE _bSDSSColorChange;

extern BYTE __dwMMJPEGPreview;
extern BYTE _bOSDCurrentUI;

#ifdef SUPPORT_ENCODE_JPG_PICTURE
extern BYTE __bMMJPGEncodeNum;
// LLY2.55, add a variable to keep previous source index before switch to SPI path
// for screen saver w/ JPEG in SPI Flash
BYTE    __bOldSrcIdx;
#endif //#ifdef SUPPORT_ENCODE_JPG_PICTURE

// LLY2.51, add a new variable to keep desired JPEG picture index, index from '1'
BYTE __bOSDSSPicIdx = 0;
BYTE _bOSDSSBackupCurrentUI;

#ifndef SUPPORT_ENCODE_JPG_PICTURE
#define OSDSS_MAX_PICTURE_NUM       2
#endif //SUPPORT_ENCODE_JPG_PICTURE

//CoCo2.38
// LLY2.51, remove __bLOGOIndex, since nobody reference it.
//extern BYTE __bLOGOIndex;
#ifdef CT950_STYLE
extern BYTE __bPOWERONMENUInitial;
#endif //CT950_STYLE


// LLY2.51, add a API to trigger each JPEG picture update.
//  Notice: need do modification if JPEG saving in flash feature ok
//  ***********************************************************************************
//  Function    :   _OSDSS_PictureUpdate
//  Description :   Update JPEG picture base on desired index.
//  Arguments   :   bIdx, specify the desired JPEG index
//  Return      :   None
//  **********************************************************************************
void _OSDSS_PictureUpdate(BYTE bIdx)
{
//    extern BYTE __bMMEffectIdx;

#ifndef SUPPORT_ENCODE_JPG_PICTURE

    PSECTION_ENTRY pLogoTbl;
    volatile PDWORD pdwPtr;
    DWORD   dwSize;

    switch(bIdx)
    {
    default:
    case 0:
        pLogoTbl = ROMLD_GetSectionByName(0, "LOG1");
        break;
    case 1:
        pLogoTbl = ROMLD_GetSectionByName(0, "LOG2");
        break;
    }

    pdwPtr = (PDWORD) pLogoTbl->dwRMA;

    // Read-back total size from flash
    dwSize = pdwPtr[0] & 0x0000FFFF;

    // Set the pointer to logo data start address
    // Notice: must /4, since pdwPtr is DWORD unit
    pdwPtr += 8/4;

    // wyc2.53-909P,
    SrcFilter_Stop(__dwSFStreamID);
    HAL_ControlParser(HAL_PARSER_STOP, 0);

    // Set playback mode and attribute as JPEG first
    __bModePlay = MODE_PLAYUNKNOW;
    __bAttrPlayNew = ATTR_JPG;

    // Disable thumbnail and JPEG preview mode first
    // Since buffer mode configuration will reference it within
    // UTL_SetPlayMode() --> UTL_Config_BufferMode(DUMP_BUFF_MM_VIDEO_ONLY_MODE)
    __bThumbnailMode=0;
    __dwMMJPEGPreview=0;

    // Config playback mode as JPEG
    UTL_SetPlayMode(MODE_PLAYVIDEO);

    // Fill desired date to video buffer directly
    HAL_FillVideoBuffer(HAL_VIDEOBUF_MM, pdwPtr, dwSize);

    // Reset video decoder
    HAL_ResetVideoDecoder(HAL_VIDEO_DECODER1|HAL_VIDEO_DECODER2 );

#else

    // LLY2.55, must keep original source index before switch to SPI path
    __bOldSrcIdx =  __SF_SourceGBL[0].bSourceIndex;

    // Switch source to SPI
    __SF_SourceGBL[0].bSourceIndex = SOURCE_SPI;

    // Start to playabck a Item configuration.
    UTL_PlayItem(bIdx+1, 0);

#endif //SUPPORT_ENCODE_JPG_PICTURE

    // LLY2.51, keep desired JPEG Effect mode for JPEG slide slow as screen save mode
    // LLY2.52, if JPEG need effect feature it can't go single JPEG Slide patch
    // ie. it must go _MM_ImageTrigger() since need two buffer.
    //__bHALJPEGEffect= __bMMEffectIdx;
    __bHALJPEGEffect= HALJPEG_EFFECT_NONE;

#ifdef JPG_SINGLE_FRAME_BUFFER
    __HALJPEGSetting.bFrameBuffer = 0;
#else
	//CoCo2.77
	if (__wHALJPEGCurrentDisplayBuf)
	{
		__HALJPEGSetting.bFrameBuffer = 0;
	}
	else
	{
		__HALJPEGSetting.bFrameBuffer = 1;
	}

    //__HALJPEGSetting.bFrameBuffer = !__HALJPEGSetting.bFrameBuffer;
#endif //JPG_SINGLE_FRAME_BUFFER

    // Do single JPEG Slide Show procedure.
    if(UTL_ShowJPEG_Slide(JPEG_PARSE_TYPE_NORMAL, __HALJPEGSetting.bFrameBuffer))
    {
        // Clear __bLOGO as none since it has exist default LOGO mode.
        __bLOGO=LOGO_NONE;
    }

}


//********************************************************************************************************
//  Function    :   OSDSS_Entry
//  Abstract    :   This function is the entry for entering screen saver.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void OSDSS_Entry(void)
{
#ifndef NO_SCREEN_SAVER

#ifdef OSDSS_JPEG_SCREEN_SAVER

#ifdef SUPPORT_ENCODE_JPG_PICTURE
    if (__bMMJPGEncodeNum == 0)
        return;
#else
    __bOSDSSPicIdx = 0;
#endif //SUPPORT_ENCODE_JPG_PICTURE

#endif //OSDSS_JPEG_SCREEN_SAVER

    _bOSDSSScreenSaverMode = TRUE;

    _bOSDSSBackupCurrentUI = _bOSDCurrentUI;

    //Tell OSD that screen saver enters.
    OSD_ChangeUI(OSD_UI_SCREEN_SAVER, OSD_ENTER_UI);

#ifdef SUPPORT_CHAR_SUBPICTURE
#ifdef EX_SP_USE_SPU_BUFFER
    // Damon2.56, clear SP display region, or SP will display in srceen saver.
    if(__bAttrPlay == ATTR_AVI && __SP1.ESFormat == PARSER_ESFMT_TEXT_SP_EXT)
    {
        CHAR_SP_ClearRegion();    
    }
#endif
#endif

#ifdef OSDSS_JPEG_SCREEN_SAVER

    OSDND_ConfigOSDRegion();
    _dwOSDSSMoveTime = OS_GetSysTimer();
    GDI_ClearRegion(0); //clear region 0

    // LLY2.51, call _OSDSS_PictureUpdate() to change desired JPEG file
    //__bLOGO = 0;
    //__bLOGOIndex++;
    //UTL_ShowLogo();
    _OSDSS_PictureUpdate(__bOSDSSPicIdx);
#else
    //Disable the main video.
    DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
    //DISP_DisplayCtrl(DISP_PIP, FALSE); //CoCo1.10, don't disable PIP. If disable PIP then enable again, the PIP can't be displayed well.
    HAL_Display(HAL_DISPLAY_SPST_DEC, FALSE);

    GDI_ClearRegion(0); //clear region 0

    GDI_DeactivateRegion();
    GDI_InitialRegion(1); //Use region 1

    _OSDSS_DVDLOGO_Initial();

#endif //OSDSS_JPEG_SCREEN_SAVER

#endif
}


//********************************************************************************************************
//  Function    :   OSDSS_Monitor
//  Abstract    :   This function monitors enter scrren saver or not.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void OSDSS_Monitor(void)
{
    if (__btPowerDown)
        return;

    //CoCo1.10, separate the servo stop from screen saver.
    _OSDSS_Servo_Monitor();

#ifndef NO_SCREEN_SAVER
    if (__dwOSDSSCheckTime == 0xFFFFFFFF)
    {
        __dwOSDSSCheckTime=OS_GetSysTimer();
        __dwOSDSSCheckNOData = __dwTimeNow;

        return;
    }
    else
    {
        if (!_bOSDSSScreenSaverMode)
        {
            if (__dwOSDSSCheckNOData == __dwTimeNow)
            {
                if ((OS_GetSysTimer()-__dwOSDSSCheckTime) > OSDSS_ENTER_TIME)
                {
#ifdef OSDSS_JPEG_SCREEN_SAVER //CoCo2.38
#ifdef CT950_STYLE
                    if (__bPOWERONMENUInitial && (__bCLOCKShowClock == FALSE) && (__bAlarmState == ALARM_NONE))
                    {
                        //OSDND_ConfigOSDRegion();
                        OSDSS_Entry();
                    }
#ifdef SUPPORT_RADIO_TUNER
#ifndef SUPPORT_RADIO_RDS
                    else if (__bPlayRadio == TRUE)
                    {
                        OSDSS_Entry();
                    }
#endif
#endif //SUPPORT_RADIO_RDS
                    else
#endif //CT950_STYLE
                    {
                        OSDSS_ResetTime();
                    }
#else
                    if (__SetupInfo.bSCRControl == SETUP_SCREEN_SAVER_ON)
                    {
                        OSDSS_Entry();
                    }
#endif //
                }
            }
            else
            {
                OSDSS_ResetTime();
            }
        }
    }
#endif //NO_SCREEN_SAVER
}

//********************************************************************************************************
//  Function    :   _OSDSS_Servo_Monitor
//  Abstract    :   This function monitors when to stop the servo.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _OSDSS_Servo_Monitor(void)
{
    if (__dwOSDServoCheckTime == 0xFFFFFFFF)
    {
        __dwOSDServoCheckTime=OS_GetSysTimer();
        __dwOSDServoCheckNOData = __dwTimeNow;
        return;
    }
    else
    {
        if (__dwOSDServoCheckNOData == __dwTimeNow)
        {
#ifdef RELEASE_SETTING
            if ((OS_GetSysTimer()-__dwOSDServoCheckTime) > OSDSS_SERVO_STOP_TIME)
            {
                SrcFilter_PowerMode (SERVO_POWER_STOP);
            }
#endif //
        }
        else
        {
            __dwOSDServoCheckNOData = __dwTimeNow;
            __dwOSDServoCheckTime=OS_GetSysTimer();
        }
    }
}


//********************************************************************************************************
//  Function    :   OSDSS_Trigger
//  Abstract    :   This function will be called by OSD_Trigger().
//  Arguments   :   bType: OSD_TRIGGER_CONTROL/OSD_TRIGGER_TIME
//              :   wTime: It's valid only when OSD_TRIGGER_TIME
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
// LLY2.53, expand time value unit from WORD to DWORD
// Since, we will use [23:16]:hour, [15:8]: min, [7:0]: sec
//void OSDSS_Trigger(BYTE bType, WORD wTime)
void OSDSS_Trigger(BYTE bType, DWORD dwTime)
{
#ifndef NO_SCREEN_SAVER

    _OSDSS_Move();

#endif
}

//CoCo2.60
void OSDSS_InternalExit(void)
{
#ifndef NO_SCREEN_SAVER

    _bOSDSSScreenSaverMode = FALSE;

    GDI_DeactivateRegion();
    GDI_InitialRegion(0); //Use region 0

    GDI_ClearRegion(0); //clear region 0

    GDI_ActivateRegion(0);

    OSD_AdjustOSDPos();

    //Enable the main video.
    DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
    HAL_Display(HAL_DISPLAY_SPST_DEC, TRUE);

    //Tell OSD that Media Manager exits.
    OSD_ChangeUI(OSD_UI_SCREEN_SAVER, OSD_EXIT_UI);

#ifdef  SUPPORT_ENCODE_JPG_PICTURE
    // LLY2.55, Switch source to previous source index
    __SF_SourceGBL[0].bSourceIndex = __bOldSrcIdx;
#endif  // #ifdef  SUPPORT_ENCODE_JPG_PICTURE


#endif
}

//********************************************************************************************************
//  Function    :   OSDSS_Exit
//  Abstract    :   Call this function to exit screen saver.
//  Arguments   :   bExitMode: OSD_UI_EXIT_ALL/OSD_UI_EXIT_CONTROL_ONLY.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void OSDSS_Exit(BYTE bExitMode) //CoCo2.60
{
#ifndef NO_SCREEN_SAVER

    _bOSDSSScreenSaverMode = FALSE;

    if (bExitMode == OSD_UI_EXIT_ALL)
    {
    GDI_DeactivateRegion();
    GDI_InitialRegion(0); //Use region 0

    GDI_ClearRegion(0); //clear region 0

    GDI_ActivateRegion(0);

    OSD_AdjustOSDPos();

    //Enable the main video.
    DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
    HAL_Display(HAL_DISPLAY_SPST_DEC, TRUE);
    }

#ifdef  SUPPORT_ENCODE_JPG_PICTURE
    // LLY2.55, Switch source to previous source index
    __SF_SourceGBL[0].bSourceIndex = __bOldSrcIdx;
#endif  // #ifdef  SUPPORT_ENCODE_JPG_PICTURE


#endif
}


//********************************************************************************************************
//  Function    :   OSDSS_Reset
//  Abstract    :   Reset the time for wakeing up screen saver. This function will be called when receiving
//              :   IR key.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void OSDSS_Reset(void)
{
#ifndef NO_SCREEN_SAVER
    __dwOSDSSCheckTime = OS_GetSysTimer();
#endif

    __dwOSDServoCheckTime = OS_GetSysTimer();

#ifndef NO_SCREEN_SAVER
    if (_bOSDSSScreenSaverMode)
    {
#ifdef OSDSS_JPEG_SCREEN_SAVER //CoCo2.38
        CC_KeyCommand(KEY_STOP);
        // LLY2.51, don't clear __bLOGO as nono by yourself, since it will be clear while non-show LOGO
        // And, remove __bLOGOIndex since nobody reference it.
        //__bLOGO = 0;

        if (__bKey != KEY_FUNCTION)
        {
            if (_bOSDSSBackupCurrentUI == OSD_UI_NONE)
            {
                __bKey = KEY_FUNCTION;
            }
            else
            {
                __bKey = KEY_NO_KEY;
            }
        }

        //__bLOGOIndex = 0;
        UTL_ShowLogo();
        OSDSS_InternalExit(); //CoCo2.60
#else
        if (__SetupInfo.bSCRControl == SETUP_SCREEN_SAVER_ON)
        {
            OSDSS_InternalExit(); //CoCo2.60
        }
#endif //OSDSS_JPEG_SCREEN_SAVER

        // wyc1.06-909, don't spin SERVO when exit screen saver
        //UTL_PreStartServo( __dwTimeNow);
    }
#endif //NO_SCREEN_SAVER
}

//********************************************************************************************************
//  Function    :   _OSDSS_Move
//  Abstract    :   This function will move DVD LOGO.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _OSDSS_Move(void)
{
#ifndef NO_SCREEN_SAVER

#ifndef OSDSS_JPEG_SCREEN_SAVER
    WORD wDownBoundary;
#endif //OSDSS_JPEG_SCREEN_SAVER

#ifdef OSDSS_JPEG_SCREEN_SAVER

    if ((OS_GetSysTimer() - _dwOSDSSMoveTime) < COUNT_5_SEC)
        return;

    _dwOSDSSMoveTime = OS_GetSysTimer();

#ifndef SUPPORT_ENCODE_JPG_PICTURE
    if (__bOSDSSPicIdx == (OSDSS_MAX_PICTURE_NUM-1))
#else
    if (__bOSDSSPicIdx == (__bMMJPGEncodeNum-1))
#endif //SUPPORT_ENCODE_JPG_PICTURE
    {
        __bOSDSSPicIdx = 0;
    }
    else
    {
        __bOSDSSPicIdx++;
    }

    //To prevent the screen saver will flash for the same picture.
#ifdef SUPPORT_ENCODE_JPG_PICTURE
    if (__bMMJPGEncodeNum > 1)
#endif //SUPPORT_ENCODE_JPG_PICTURE
    {
        _OSDSS_PictureUpdate(__bOSDSSPicIdx);
    }

#else

    if ((OS_GetSysTimer() - _dwOSDSSMoveTime) < OSDSS_DVDLOGO_MOVE_SPEED)
        return;

    _dwOSDSSMoveTime = OS_GetSysTimer();

    if ((OS_GetSysTimer() - _dwOSDSSColorChangeTime) > OSDSS_DVDLOGO_COLOR_CHANGE_SPEED)
    {
        _OSDSS_ColorChange();

        _dwOSDSSColorChangeTime = OS_GetSysTimer();
    }

    if ((OS_GetSysTimer() - _dwOSDSSMixRatioChangeTime) > OSDSS_DVDLOGO_MIX_RATIO_CHANGE_SPEED)
    {
        _OSDSS_MixRatioChange();

        _dwOSDSSMixRatioChangeTime = OS_GetSysTimer();
    }


    if (_bOSDSSInvertH)
    {
        if (_bOSDSSCurrentPos.nHPos < OSDSS_DVDLOGO_BOUNDARY_LEFT)
        {
            _bOSDSSCurrentPos.nHPos = OSDSS_DVDLOGO_BOUNDARY_LEFT;
            _bOSDSSInvertH = FALSE;
        }
        else
            _bOSDSSCurrentPos.nHPos -= OSDSS_DVDLOGO_POS_UPDATE_H;
    }
    else
    {
        if ((_bOSDSSCurrentPos.nHPos + OSDSS_DVDLOGO_POS_UPDATE_H+OSDSS_DVDLOGO_BITMAP_WIDTH) >= OSDSS_DVDLOGO_BOUNDARY_RIGHT)
        {
            _bOSDSSCurrentPos.nHPos = OSDSS_DVDLOGO_BOUNDARY_RIGHT-OSDSS_DVDLOGO_BITMAP_WIDTH;
            _bOSDSSInvertH = TRUE;
        }
        else
            _bOSDSSCurrentPos.nHPos += OSDSS_DVDLOGO_POS_UPDATE_H;
    }

    if (_bOSDSSInvertV)
    {
        if (_bOSDSSCurrentPos.nVPos < OSDSS_DVDLOGO_BOUNDARY_UP)
        {
            _bOSDSSCurrentPos.nVPos = OSDSS_DVDLOGO_BOUNDARY_UP;
            _bOSDSSInvertV = FALSE;
        }
        else
            _bOSDSSCurrentPos.nVPos -= OSDSS_DVDLOGO_POS_UPDATE_V;
    }
    else
    {
        if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
            wDownBoundary = OSDSS_DVDLOGO_BOUNDARY_DOWN_NTSC;
        else
            wDownBoundary = OSDSS_DVDLOGO_BOUNDARY_DOWN_PAL;

        if ((_bOSDSSCurrentPos.nVPos + OSDSS_DVDLOGO_POS_UPDATE_V+OSDSS_DVDLOGO_BITMAP_HEIGHT) >= wDownBoundary)
        {
            _bOSDSSCurrentPos.nVPos = wDownBoundary-OSDSS_DVDLOGO_BITMAP_HEIGHT;
            _bOSDSSInvertV = TRUE;
        }
        else
            _bOSDSSCurrentPos.nVPos += OSDSS_DVDLOGO_POS_UPDATE_V;
    }

    if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
        GDI_MoveRegion(1, REGION_X_NTSC+_bOSDSSCurrentPos.nHPos, REGION_Y_NTSC+_bOSDSSCurrentPos.nVPos);
    else
        GDI_MoveRegion(1, REGION_X_PAL+_bOSDSSCurrentPos.nHPos, REGION_Y_PAL+_bOSDSSCurrentPos.nVPos);

#endif //#ifdef OSDSS_JPEG_SCREEN_SAVER

#endif
}

//********************************************************************************************************
//  Function    :   _OSDSS_DVDLOGO_Initial
//  Abstract    :   This function will initialize the action of DVD LOGO.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _OSDSS_DVDLOGO_Initial(void)
{
#ifndef NO_SCREEN_SAVER
    _bOSDSSCurrentPos.nHPos = OSDSS_DVDLOGO_BITMAP_START_H;
    _bOSDSSCurrentPos.nVPos = OSDSS_DVDLOGO_BITMAP_START_V;

    //Initialize the time to move the DVD LOGO
    _dwOSDSSMoveTime = OS_GetSysTimer();
    _dwOSDSSColorChangeTime = OS_GetSysTimer();
    _dwOSDSSMixRatioChangeTime = OS_GetSysTimer();

    _bOSDSSInvertH = FALSE;
    _bOSDSSInvertV = FALSE;
    _bOSDSSMixRatioInvert = FALSE;
    _bSDSSColorChange = 0;

    _bOSDSSMixRatio = OSDSS_DVDLOGO_DEFAULT_MIX_RATIO;

    GDI_ClearRegion(1); //clear region 1
    GDI_ChangePALEntry(OSDSS_PALETTE_ENTRY_DVD_LOGO, aOSDSSDVDLOGOPalette[_bSDSSColorChange], TRUE);
    GDI_SetMixRatio(_bOSDSSMixRatio); //The mix ration just effects the background
    if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
        GDI_MoveRegion(1, REGION_X_NTSC, REGION_Y_NTSC);
    else
        GDI_MoveRegion(1, REGION_X_PAL, REGION_Y_PAL);

    GDI_ActivateRegion(1); //open GDI

    /*
    _OSDSSStringAttr.bBackground = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDSSStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDSSStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDSSStringAttr.bTextColor = OSDSS_PALETTE_ENTRY_DVD_LOGO;
    _OSDSSStringAttr.bColorKey = 0;
    _OSDSSStringAttr.wX = _bOSDSSCurrentPos.nHPos;
    _OSDSSStringAttr.wY = _bOSDSSCurrentPos.nVPos;

    GDI_DrawString_909(1, &_OSDSSStringAttr, aSSDVDLOGOTest);
    */

    _OSDSSDrawBitmap.bBitmapID = OSDSS_BMP_SCREEN_SAVER;
    _OSDSSDrawBitmap.wDestX = _bOSDSSCurrentPos.nHPos;
    _OSDSSDrawBitmap.wDestY = _bOSDSSCurrentPos.nVPos;
    _OSDSSDrawBitmap.bDestRegion = 1;
	/*
    GDI_LoadPalette(GDI_PAL_SCREEN_SAVER);
    GDI_LoadBMP(GDI_OSD_BMP); // load other BMP used by Display mode (this including screen saver BMP)
    GDI_DrawBitmap(&_OSDSSDrawBitmap);
	*/
	//CoCo1.06, for new GDI libray
	GDI_LoadPalette(&__PaletteInfo_ScreenSaver);
	GDI_LoadBMP(_aScrrenSaverBMPArray, 1); // load other BMP used by Display mode (this including screen saver BMP)
	GDI_DrawBitmap(&_OSDSSDrawBitmap, _aScrrenSaverBMPArray);
#endif
}

//********************************************************************************************************
//  Function    :   _OSDSS_ColorChange
//  Abstract    :   This function will change the color and mix ratio of the DVD LOGO
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _OSDSS_ColorChange(void)
{
#ifndef NO_SCREEN_SAVER
    _bSDSSColorChange++;
    if (_bSDSSColorChange >= (sizeof(aOSDSSDVDLOGOPalette)/4))
        _bSDSSColorChange = 0;

    GDI_ChangePALEntry(OSDSS_PALETTE_ENTRY_DVD_LOGO, aOSDSSDVDLOGOPalette[_bSDSSColorChange], TRUE);
    GDI_ActivateRegion(1); //open GDI
#endif
}

//********************************************************************************************************
//  Function    :   _OSDSS_MixRatioChange
//  Abstract    :   This function will change the mix ratio of the DVD LOGO
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _OSDSS_MixRatioChange(void)
{
#ifndef NO_SCREEN_SAVER

    if (_bOSDSSMixRatioInvert)
    {
        if ((_bOSDSSMixRatio-OSDSS_DVDLOGO_MIX_RATIO_UPDATE) < OSDSS_DVDLOGO_MIN_MIX_RATIO)
        {
            _bOSDSSMixRatioInvert = FALSE;
            _bOSDSSMixRatio = OSDSS_DVDLOGO_MIN_MIX_RATIO;
        }
        else
        {
            _bOSDSSMixRatio -= OSDSS_DVDLOGO_MIX_RATIO_UPDATE;
        }
    }
    else
    {
        _bOSDSSMixRatio += OSDSS_DVDLOGO_MIX_RATIO_UPDATE;
        if ((_bOSDSSMixRatio+OSDSS_DVDLOGO_MIX_RATIO_UPDATE) > OSDSS_DVDLOGO_MAX_MIX_RATIO)
        {
            _bOSDSSMixRatioInvert = TRUE;
            _bOSDSSMixRatio = OSDSS_DVDLOGO_MAX_MIX_RATIO;
        }
    }

    GDI_SetMixRatio(_bOSDSSMixRatio);
    GDI_ActivateRegion(1); //open GDI

#endif
}

void OSDSS_ResetTime(void)
{
    __dwOSDSSCheckNOData = __dwTimeNow;
    __dwOSDSSCheckTime=OS_GetSysTimer();
}