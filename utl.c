// ******  Big modification History Area  ******
// LLY0.60, remove the un-necessary code:
// - MSG_CDROM_DECODE
// - MODE_NOT_CLEAR_PROGRAM
// - USE_DEBUG_CMD
// - SUPPORT_AV_SYSTEM
// - IDATA: __bMin, __bSec, __bFrame
// - data: __dwSysTimer
// - _WINDVD
// - SFR for control booting from SRAM or FLASH.
// - HAL_Monitor(HAL_MONITOR_AUDIO_FREQ_PREPARE);
// - HAL_Monitor(HAL_MONITOR_VIDEO_STATUS_PREPARE);
// - _TIME_CIRCLE
// - SUPPORT_CDIF
// - CDDA_WAITSERVO
// - UTL_DSPuCodeBackup
// - __dwSectorCDROMBegin, __dwSectorCDROMEnd
// - __bLastModePlay
// - HAL_ControlCDIF()
// - function body of UTL_ReadSectors()

// LLY0.85, remove the code of "reset buffer for OSD display abnormal" issue
// while SUPPORT_ANTISHOCK case.
// Since it's un-necessary for CT909 solution.

// LLY1.15, remove the un-necessary code for code reduce
// - UTL_ReadSectors(): since it's un-necessary for CT909

// LLY1.20, integrate MP4/ AVI/ DIVX/ Nero Digital related code
// - Rename pDivXBitsContent as pBitsContent
// - UTL_DivXASTSetting() as UTL_MovieASTSetting()

// LLY2.04, remove the un-necessary global variables
// ex. temp global variables

// LLY2.35, remove UTL_FATPlayFromTime() since it has integrated w/ UTL_PlayFromTime()

///////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// ***** Global Define Area *****
// If define it, it will print-out the scan mode relative message
//#define DEBUG_SCAN_MODE

// If define it, it will print-out the play item range message
//#define DEBUG_PLAY_RANGE

// If define it, it will print-out the desired total time
//#define DEBUG_TOTAL_TIME

// If define it, it will print-out the pre-scan information
//#define DEBUG_PRESCAN

// If define it, it will print-out the DRM relative information
//#define DEBUG_DRM

// If define it, it will print-out the jump position information for SCAN mode
//#define DEBUG_SCAN_JUMP

// If define it, it will print-out parser end padding enable or disable control status
//#define DEBUG_END_PADDING

// If define it, it will print-out desired audio ID for *.VOB
//#define DEBUG_AUDIO_ID

// If define it, it will print-out current buffer mode setting.
//#define DEBUG_BUFFER_MODE

// If define it, it will print-out show time related information.
//#define DEBUG_SHOWTIME

// If define it, it will print-out AVI GoTime related information.
//#define DEBUG_AVI_GOTIME

// If define it, it will print-out WMA GoTime related information.
//#define DEBUG_WMA_GOTIME

// If define it, it will print-out WMA SCAN mode related information
//#define DEBUG_WMA_SCAN

// If define it, it will print-out AVI Index/ SP/ Servo dump-in buffer address
//#define DEBUG_AVI_IDX_SP_SERVO_BUFFER

// If define it, it will print-out some information while playback JPEG from SPI Flash
//#define DEBUG_JPEG_FROM_SPF

// If define it, it will print-out some information while do single JPEG slide show
//#define DEBUG_JPEG_SHOW

// Grievous2.77, if define it, we will not need to wait for 3~4 sec to start playback when it is scan/goto/repeat mode.
// Since sometimes (previous index position < audio position < go time position = index position <= video position) is happen,
// parser will begin from previous index position.
// And this solution may loss about half second audio instead of playback directly.
// #define UTL_SCAN_QUICK

// If define it, it will force find 1st non-DTS audio stream while playback *.VOB
#define FIND_NODTS_AUDIO

// If define it, it don't support DivX 311 format.
//#define NO_DIVX_311


// if define it, only enable the APIs for simple UTL
#ifdef  ECOS909
#define SIMP_UTL
#endif  // #ifdef ECOS909

/////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////
// *****  Include header file Area  *****
// The file "winav.h" must be included in first line
#include "winav.h"
#include "cc.h"     // for variable reference
#include "utl.h"
#include "comutl.h"
#include "hal.h"
#include "infofilter.h"
#include "digest.h"
#include "input.h"  // for KEY_xxx reference
#include "chips.h"
#include "osd.h"
#include "srcfilter.h"  // LLY0.60, since SrcFilter_GetCurretPos()
#include "initial.h"
#include "navigate.h"
#include "Mmanager.h"
#include "subpict.h"  // LLY0.80, in order to usage SP/ OGT type ID
#include "cdg.h"  //LJY0.95
#include "char_subpict.h"


#ifdef  SIMP_UTL
#include "ecoscc.h"

// Define the variable which is only used for real DVD system
// [Media Manager Module]
DWORD   __dwMMJPEGDisplayTime;

#define     MM_GetFileType(a, b)           EXTNAME_NONE


// [CC Module]
//LLY0.72, BYTE    __bNextHWReset;
BYTE    __bSTCRunning;
DWORD   _dwTimerCount_MainLoop;
BYTE    __bRepeatAB=REPEAT_NONE;
BIT     __btNewTime;
BIT     __btTemp;
DWORD   __dwTimeNow;


// [UTL Module]
#define     UTL_OutputShowTime()
#define     UTL_SetCDDADeEmphasis()
#define     UTL_PreStartServo(a, b)
#define     UTL_TransferCommandID(a)

// [NV Module]
#define     NV_Action(a, b)


#else   // #ifdef SIMP_UTL
#include "cell.h"
#include "monitor.h"
#include "linear.h"     //Kevin0.86, add
#include "panel.h"	// LLY0.60, since PANEL_Output() & PANEL_Trigger() usage
// Micky2.80, include "thumb.h" for reference __bThumbnailMode.
#include "thumb.h"
#include "setup.h"  // LLY2.60, for UTL_QuerySetting()

#ifndef NO_DRM
#include "osddivx.h"        // For DivX DRM playback control
#endif  // #ifndef NO_DRM

#endif  // #ifdef SIMP_UTL

#include "comdec.h"
#include "parser.h"
#include "disp.h"  // LLY0.65, for using DISP_FrameBufferSet()
#include "jpegdec.h"    // LLY2.51, include to support UTL_ShowJPEG

#undef      SUPPORT_PRINTF





///////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// ***** Extern other module's global variable Area *****
extern  BYTE    __bSlowLevel;
extern  WORD    __wSlowSpeed;  // in "cc.c"
extern  BYTE    __bDVDDigest;
extern  BYTE    __bSTCRunning;

// CSC0.81, for reference DVD copyright information
extern COPYINFO    cpinfo ;

extern BYTE    __bTVE_JPEG; //TRUE: TVE setting for JPEG; FALSE: TVE setting for MPEG

extern BYTE _bHALJPEGRandomEffectType;
//////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////
// ***** Declare Global Variable Area *****
BYTE    __bCheckAudioType;// Micky2.80, review global variables init=CHECK_NONE;
BIT     __btCheckVideoType;// Micky2.80, review global variables init=FALSE;

BYTE    __bModePlay;//Micky2.80, review global variables init.= MODE_PLAYUNKNOW;      //
//LLY0.72, BYTE    __bLastModePlay;
// LLY.038, keep the previous play mode before enter CDROM decoding
BYTE    __bModePlayPreCDROM;// Micky2.80, review global variables init= MODE_PLAYUNKNOW;
// LLY.274p, keep the current and new desired playing mode attribute
BYTE    __bAttrPlay;
BYTE    __bAttrPlayNew;

BYTE    __bMin, __bSec, __bFrame;

BYTE    __bLOGO;

BYTE        _bJumpTime = FALSE;

// LLY2.04, remove _dwUTLTemp, _dwUTLTemp1, _dwUTLTemp2, _bUTLTemp1, wITLTemp1
//DVD_051Micky
// it records the stable time value
// to prevent the time value is undated by interrupt
//DWORD  __dwSysTime;
// support a function to read stable time

DWORD   _dwTriggerShowTime; // Next TRIGGER OSD/PANEL time

// LLY0.63, expand the unit from BYTE to DWORD
// Because, COUNT_XXX_MSEC maybe larger than BYTE unit
DWORD   _dwGap_TriggerShowTime; // Gap for trigger show time

DWORD   _dwTimePrev;        // ** TCH0.99-1; record the previous show time
DWORD   _dwUTLNextTriggerCount;   // ** TCH1.70-2; record SysTimer for next Trigger.

// DVD_170Micky, enhance MP3 display time method
// accumulate frames count.
DWORD    _dwTotalFrameCount;   // total decoded frames count for current track
DWORD    _dwPreFrameCount;     // previous frame count number
// wyc.172-CDDA, support a flag to represent playing across tracks automatically.
BYTE    __bCDDAAutoChTrk;

DWORD   _dwTimeStart;   // record the start time for while loop

// Keep previous scan time for next scan time-out control
DWORD   __dwScanTime=0;

// LLY2.71, keep previous issue SCAN CMD time for error protection to wait SCAN_DONE flag
DWORD   __dwCmdTime=0;

//BYTE    __bBrightnessDuration=0;

// LLY0.75, define a pointer to a DivX bitstream content structure
PPARSER_BITS_CONTENT    pBitsContent;

// LLY0.75, define a variable to keep last SCR value for *.vob/ *.dat/ *.mpg
DWORD   __dwLastSCR;

// LLY0.75, keep total time of current playback file.
DWORD   __dwFileTime;

// LLY0.76-1, keep sector number of one second for CDROM motion file
DWORD   __dwSectorPerSec;

// LLY0.80, keep cuffer buffer estimate time for VCD/ SVCD/ CDDA title
// Since, the buffer maybe reset by HAL_RESET_VIDEO/ HAL_RESET_AUDIO
DWORD   __dwBufferTime;

// LLY0.80, keep desired Subpicture/ OGT type for decoder to set run length table
BYTE    __dwSPType;

// LLY0.85, keep desired video ID
BYTE    __bVideoID;

// LLY0.95, Keep DRM playback state
BYTE    __bDRMPlayState;

// LLY0.95, keep DivX DRM remain view count
BYTE    __bDRMRemainView;

// LLY0.96, keep if the DivX index table exist
BYTE    __bNoDivXIndexTbl;

// LLY1.02, keep last position time (in msec format) for DivX scan mode.
DWORD   __dwScanBaseTime;

// LLY1.02, keep previous play item index
WORD   __wPrevPlayItem;

// LLY1.02, keep previous play item directory
WORD    __wPrevPlayDir;

// LLY1.02, the flag used check if the previous and new play item are same
BYTE    __bSameItem;

// wyc2.14-909s,
BYTE   _bUTLSrcBuffMod;

// LLY2.15, add a variable to keep desired debug mode
// Notice: must set the default value during _INITIAL_Variables_PowerON()
//SYuan2.77 : expand __bDebugMode  to DWORD, and for Setting HAL-TX, RX Indenpendent
//BYTE    __bDebugMode;
DWORD    _dwDBGMode;

// LLY2.21, initial frame buffer mode as unknown
BYTE    __bFrameBufMode=UTL_FRAMEBUF_UNKNOWN;

WORD    __wFrameWidth; // LLY2.22

// LLY2.55, add a variable to keep video content format of current AVI container
DWORD   __dwAVIVideoFMT;

// Grievous2.60, add a variable to keep bits for each sample
BYTE    __bBitsPerSample;

// Grievous2.78, add a variable to keep internal/external subpicture mode
BYTE    __bUTLSPTextMode;       // 1: internal mode ; 0: external mode ; 2: unknown

// Chuan2.22, Add a global variable to record BFR setting.
#ifdef SUPPORT_BFR_MODE // B-Frame redundancy
#ifdef CONFIG_BFR_BY_FW
BYTE  _bBFRMode = FALSE;
#endif  // #ifdef CONFIG_BFR_BY_FW
#endif  // #ifdef SUPPORT_BFR_MODE


//wyc2.17-909S, rename SUPPORT_LANGUAGE_STUDY as SLOW_MODE_CONFIG
#if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)
BYTE    __bLangStudyFlag = FALSE;
#endif

//#define LOGO_FROM_FLASH
// LLY2.38, porting CoCo's code to support multi-LOGO
// LLY2.51, remove this variables
//BYTE __bLOGOIndex = 0;

// LLY1.06-1, include "logoaddr.h" if define LOGO_FROM_FLASH to get logo data address within flash
// Otherwise, include the "logo.txt"
#ifdef  LOGO_FROM_FLASH
// LLY2.15, using new method to find logo address
#include "romld.h"
//#include "logoaddr.h"
// Define the start address within flash for logo data
//#define LOGO_START_ADDR   (*(volatile DWORD *)(_BOOT_LOGOAddress))

#else   // #ifdef LOGO_FROM_FLASH
BYTE    aLogoData[] =
{
#ifndef NO_LOGO
#ifdef  RELEASE_SETTING
#include "logo.txt"
#else   // #ifdef RELEASE_SETTING
#include "logo_dbg.txt"
#endif  // #ifdef RELEASE_SETTING
#else   // #ifndef NO_LOGO
0
#endif  // #ifndef NO_LOGO
} ;
#endif  // #ifdef LOGO_FROM_FLASH


// LLY2.51 create ...
//  ******************************************************************************************
//  Function    :   UTL_ShowJPEG_Slide
//  Description :   Do Single JPEG Slide playback control
//  Arguments   :   bFrameIdx, specify the desired frame buffer index for JPEG slide show
//                  And, the index from '0'
//  Return      :   TRUE, the action is ok
//                  FALSE, the action is fail
//  ******************************************************************************************
BYTE UTL_ShowJPEG_Slide(BYTE bType, BYTE bFrameIdx) //CoCo2.78, add "bType" parameter
{
    DWORD   dwTemp;
    BYTE    bResult=0xFF;

    // LLY2.51, don't set JPEG effect as none since screen save mode by JPEG Slide Show case
    // will enable JPEG effect feature.
    // LLY2.60, clear JPEG effect as none
    // otherwise we will see abnormal LOGO or JPEG screen saver while come from playback JPEG /w Effect
    __bHALJPEGEffect = HALJPEG_EFFECT_NONE;

    // Clear playback mode and attribe value
    // since it will be set as JPEG before call UTL_PlayItem() or UTL_SetPlayMode()
    __bModePlay = MODE_PLAYUNKNOW;
    __bAttrPlay = ATTR_NONE;

    HALJPEG_Reset();

    // LLY2.56, base on user specified frame index
    // Since, 32M/64M solution will use frame 0 or 1
    __HALJPEGSetting.bFrameBuffer=bFrameIdx;
    __HALJPEGSetting.bOperation = HALJPEG_OPERATION_I;
    __HALJPEGSetting.wDisplayStartH = MM_JPEG_FRAME_START_H;
    __HALJPEGSetting.wDisplayStartV = MM_JPEG_FRAME_START_V;
    __HALJPEGSetting.wDisplayWidth = MM_JPEG_FRAME_WIDTH;
    __HALJPEGSetting.wDisplayHeight = MM_JPEG_FRAME_HEIGHT;
    __HALJPEGSetting.dwBackgroundColor = HALJPEG_SLIDESHOW_BACKGROUND_COLOR;

    // Issue parsing JPEG header
    //HALJPEG_ParseHeader(JPEG_PARSE_TYPE_NORMAL, __HALJPEGSetting.bFrameBuffer);
	HALJPEG_ParseHeader(bType, __HALJPEGSetting.bFrameBuffer); //CoCo2.78, add "bType" parameter

    // Waiting parsing header done
    dwTemp=OS_GetSysTimer();
    while( (OS_GetSysTimer()-dwTemp) < COUNT_3_SEC*2 )
    {
        bResult = HALJPEG_Status(HALJPEG_PARSE_HEADER);
        if( bResult == JPEG_STATUS_OK )
        {
            break;
        }
    }

    if( bResult != JPEG_STATUS_OK )
    {
#ifdef  DEBUG_JPEG_SHOW
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "parsing header fail, status: %lx\n", bResult);
#endif  // #ifdef  DEBUG_JPEG_SHOW
        return FALSE;
    }

    // Issue JPEG decoding command
    HALJPEG_Decode();

    // Waiting JPEG decoding done
    dwTemp=OS_GetSysTimer();
    while( (OS_GetSysTimer()-dwTemp) < COUNT_5_SEC )
    {
        bResult = HALJPEG_Status(HALJPEG_DECODE);
        if( bResult == JPEG_STATUS_OK )
        {
            break;
        }
    }

    if( bResult != JPEG_STATUS_OK )
    {
#ifdef  DEBUG_JPEG_SHOW
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "JPEG decode fail, status: %lx\n", bResult);
#endif  // #ifdef  DEBUG_JPEG_SHOW
        return FALSE;
    }

    // Issue display command
    bResult = HALJPEG_Display(__HALJPEGSetting.bFrameBuffer);

    if(bResult != SLIDESHOW_RET_OK)
    {
#ifdef  DEBUG_JPEG_SHOW
        printf("JPEG display fail, status: %lx\n", bResult);
#endif  // #ifdef  DEBUG_JPEG_SHOW
        return FALSE;
    }

#ifdef  DEBUG_JPEG_SHOW
    printf("JPEG display ok\n");
#endif  // #ifdef  DEBUG_JPEG_SHOW
    return TRUE;
}


// LLY1.06, modify the procedure to support LOGO from flash directly.
//  ******************************************************************************************
//  Function    :   UTL_ShowLogo
//  Description :   Issue the necessary action to show LOGO
//  Argument    :   None
//  Return      :   TRUE, background LOGO is display ok
//                  FALSE, LOGO decoding is not complete
//  Notice      :   Must turn-on cache mode while access PROM in DWORD unit
//  ******************************************************************************************
#define LOGO_DATA_OFFSET_BYTE       8   // Define the offset bytes of logo real data (ie. not include header)
#ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
BYTE     DisplayLogoFlag=FALSE;
#endif
BYTE UTL_ShowLogo(void)
{
#ifndef  NO_LOGO

    DWORD   dwCnt;
    DWORD   dwSize;
    DWORD   dwTemp;
    DWORD   dwPreVisStill; //Aron2.51 fix repeat KEY_SETUP soon leads NV RSM wrong (go to the last vobu with video)
//Micky2.11a    DWORD   dwSaveInt;
    BYTE    bLogoType;  // Keep logo type, ex. MPEG Still, Motion, JPEG
    BYTE    bBitFormat; // Keep bitstream format, ex. NTSC or PAL


#ifdef  LOGO_FROM_FLASH
    // Must using DWORD unit for access flash data, since H/W limitation.
    // LLY2.15, using new method to find logo address
    //volatile PDWORD pdwPtr = (PDWORD)LOGO_START_ADDR;
    volatile PDWORD pdwPtr;
    ULONG_UNION ulunData;

    PSECTION_ENTRY pLogoTbl;

#else   // #ifdef LOGO_FROM_FLASH
    PDWORD  pdwPtr;

#endif  // #ifdef LOGO_FROM_FLASH
#ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
 	DisplayLogoFlag=TRUE;
#endif
    // Don't re-draw LOGO while background LOGO is same as requirement
    //if(__bLOGO == LOGO_DEFAULT)
    if(LOGO_TYPE() == LOGO_DEFAULT) // LLY0.80
    {
    	#ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
 	DisplayLogoFlag=FALSE;
	#endif
        return TRUE;
    }

    // LLY2.51, do parser & source stop earily before start LOGO show.
    // wyc2.53-909P,
    SrcFilter_Stop(__dwSFStreamID);
    HAL_ControlParser(HAL_PARSER_STOP, 0);

    // LLY0.68, base on LOGO design spec. to get logo related information, ex.
    // bit[31:24]: LOGO Type
    // bit[23:16]: Bitstream format
    // bit[15:0]: data length (DWORD unit)
#ifdef  LOGO_FROM_FLASH

    // LLY2.51, remove __bLOGOIndex reference.
    // LLY2.38, porting CoCo's code to support multi-LOGO
#if 0
    switch (__bLOGOIndex)
    {
    case 1:
        pLogoTbl = ROMLD_GetSectionByName(0, "LOG1");
        break;
    case 2:
        pLogoTbl = ROMLD_GetSectionByName(0, "LOG2");
        break;
    case 0:
    default:
        pLogoTbl = ROMLD_GetSectionByName(0, "LOGO");
        break;
    }
#endif  // #if 0

   #ifdef SUPPORT_POWER_ON_DISPLAY_LOGO
  {   extern BYTE   _bPowerOnFlag;
  	if(_bPowerOnFlag)
       {
	   	pLogoTbl = ROMLD_GetSectionByName(0, "LOG3");
  	}
       else
	{
		pLogoTbl = ROMLD_GetSectionByName(0, "LOGO"); 	
       }
}
#else
    	pLogoTbl = ROMLD_GetSectionByName(0, "LOGO");
#endif

    if(!pLogoTbl)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find LOGO data\n");
#ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
 	DisplayLogoFlag=FALSE;
#endif		
        return (FALSE);
    }

    pdwPtr = (PDWORD) pLogoTbl->dwRMA;

    // Read-back 1st DWORD data from flash
    ulunData.dw32bit=pdwPtr[0];
    bLogoType=ulunData.b8bit[0];
    bBitFormat=ulunData.b8bit[1];
    dwSize=ulunData.b8bit[2]*256 + ulunData.b8bit[3];

    // Set the pointer to logo data start address
    // Notice: must /4, since pdwPtr is DWORD unit
    pdwPtr += LOGO_DATA_OFFSET_BYTE/4;

#else   // #ifdef LOGO_FROM_FLASH
    bLogoType=aLogoData[0];
    bBitFormat=aLogoData[1];
    dwSize=aLogoData[2]*256 + aLogoData[3];

    // Set the pointer to logo data start address
    pdwPtr = (PDWORD) &aLogoData[LOGO_DATA_OFFSET_BYTE];

#endif  // #ifdef LOGO_FROM_FLASH

    // Specify the bitstream format as NTSC or PAL
    if(bBitFormat==0xFF) // PAL
    {
        __btNTSCBitStream=FALSE;
    }
    else // NTSC
    {
        __btNTSCBitStream=TRUE;
    }

    HAL_AdjustTFT_DigitalMode(TRUE);

    // LLY0.80, clear bit[3:0] as zero for __bLOGO
    // Otherwise, __bLOGO |= LOGO_DEFAULT action maybe no effect while LOGO_JPEG --> KEY_OPEN_CLOSE
    // Notice: don't change bit[7:4] value since LOGO attribute maybe clear
    __bLOGO &= 0xF0;

    // LLY0.70, enable the LOGO_DEFAULT as LOGO type first
    // Since, decoder will call HAL_AdjustTVMode() after sequence header detected.
    // So, it will go wrong path within _AspectRatio()
    __bLOGO |= LOGO_DEFAULT;

    // For MPEG still Logo
    if(bLogoType==0x4D)
    {
        // LLY0.72, create the MPEG thread if it doesn't exist
        if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_MPEG_DONE) )
        {
            INITIAL_ThreadInit(THREAD_MPEG_DECODER);

            // LLY0.95, the waiting time is absolute time
            // wait the thread initial done
            //cyg_flag_timed_wait( &__fThreadInit, INIT_DEC_THREAD_MPEG_DONE, CYG_FLAG_WAITMODE_AND, COUNT_50_MSEC);
            OS_TimedWaitFlag( &__fThreadInit, INIT_DEC_THREAD_MPEG_DONE, FLAG_WAITMODE_AND,
                (COUNT_50_MSEC) );

            // print-out the warning message if the thread initial fail.
            if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_MPEG_DONE) )
            {
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MPEG thread not initial done, value: %lx\n", OS_PeekFlag(&__fThreadInit));
		#ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
 		DisplayLogoFlag=FALSE;
		#endif
				
                return (FALSE);
            }

            // LLY2.21, remove turn-off main video procedure into UTL_Config_FrameBuffer()
            // LLY0.85a, reset the mainvideo frame buffer address as MPEG mode after create MPEG thread ok
            // Otherwise, MPEG LOGO will be displayed abnormal while JPEG --> KEY_OPEN_CLOSE
            // Notice:
            // (1) Must turn-off the display first to avoid seeing un-except screen
            // (2) Don't put these control before MPEG thread create routine
            //     Since __bLOGO type is not LOGO_JPEG while JPEG preview --> MP3 playing --> KEY_OPEN_CLOSE
            //     So, F/W don't turn-off the display first to avoid the un-except screen
            //DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);


            //kevin1.00, JPEG & MPEG use diff. gains
            __bTVE_JPEG = FALSE;
            //MPEG Display will call HAL_AdjustTVMode

#ifdef  SUPPORT_TFT
            // Alan1.06, force to set VP underlying registers; otherwise Logo when TV mode=PAL will not be shown (TFT_Init set N as default) until main loop ~sec later
            // Notice: Need to doble check why put here
            //         Since, bitstream format is ready until decoder decoding done,
            //         So call it @ HAL_AdjustTVMode() is better.
    		TFT_Trigger();
#endif  // #ifdef SUPPORT_TFT
        }

        // Chuan2.61, Set MainVideo Frame Buffer Address even though MPEG thread always exist.
        // Since, CDG will set its Frame Buffer Setting, so we need set it back now.
        // LLY2.17, collect frame buffer setting procedure into UTL_Config_FrameBuffer()
    	// Chuan2.22, LOGO always do BFR. It major avoid to set the wrong BFR info at the following flow.
    	// AVI(no BFR) -> LOGO -> DVD
        // Because nobody set the information at DVD/SVCD/VCD
        UTL_BFRModeSet(TRUE);
        UTL_Config_FrameBuffer(UTL_FRAMEBUF_MOTION);

        // LLY0.64, Issue update sequence header
        HAL_Reset(HAL_RESET_VIDEO);

#ifdef SUPPORT_POWER_SAVING
        // Micky2.11a, integrate VDEC power down into a API.
        HAL_PowerControl(HAL_POWER_VDEC, HAL_POWER_NORMAL);
#endif

        // LLY0.64-1, set playing mode and attribute
        // Must check how to do for reasonable method.
        __bModePlay=MODE_PLAYUNKNOW;
        __bAttrPlay=ATTR_NONE;

        // Micky2.11a, integrate fill data to video buffer procedure into a API
        HAL_FillVideoBuffer(HAL_VIDEOBUF_NORMAL, pdwPtr, dwSize);

        // Micky2.11a, integrate video decoder reset procedure into a API.
        HAL_ResetVideoDecoder(HAL_VIDEO_DECODER1|HAL_VIDEO_DECODER2 );

        //Aron2.51 fix repeat KEY_SETUP soon leads NV RSM wrong (go to the last vobu with video)
        //Check the last video is still or not.
        HAL_ReadInfo(HAL_INFO_STILL, &dwPreVisStill);

        // LLY0.70, clear still flag first
        // Otherwise, still ack will be get quickly while still --> LOGO
        HAL_PlayCommand(COMMAND_V_CLEAR_STILL, 0);
        HAL_PlayCommand(COMMAND_PLAY, 0);

        dwCnt=0;
        dwTemp=OS_GetSysTimer();
        while( (OS_GetSysTimer() - dwTemp) < COUNT_2_SEC)
        {
            HAL_ReadInfo(HAL_INFO_STILL, &dwCnt);

            // LOGO display OK
            if(dwCnt)
            {
                // LLY0.70, read-back the desired frame buffer index, and enable the video display
                // Fix, LOGO can't display while Still --> KEY_OPEN_CLOSE
                // Since, vsync not enable the display, and decode receive the stop command.
                HAL_ReadInfo(HAL_INFO_DECFRAME, &dwCnt);
                HAL_PlayCommand(COMMAND_STOP, 0);
                DISP_Display(dwCnt, DISP_MAINVIDEO);
                // LLY0.75, must force enable the display !!
                DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);

#ifdef SUPPORT_TFT
		        TFT_Trigger(); //force to set VP underlying registers; otherwise Logo when TV mode=PAL will not be shown (TFT_Init set N as default) until main loop ~sec later
#endif  // #ifdef SUPPORT_TFT

//Aron2.51 fix repeat KEY_SETUP soon leads NV RSM wrong (go to the last vobu with video)
                //Check the last video is still or not.
                //If previous one is not still, clear it.
                if(!dwPreVisStill)
                {
                    HAL_PlayCommand(COMMAND_V_CLEAR_STILL, 0);
                }
		#ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
 		DisplayLogoFlag=FALSE;
		#endif

                return (TRUE);
            }
            // release the CPU
            OS_YieldThread();
        }

        // LOGO display fail.
        if(dwCnt==FALSE)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Time-Out for LOGO \n");
            __bLOGO = LOGO_NONE;
        }
	#ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
 		DisplayLogoFlag=FALSE;
	#endif

        return (FALSE);


    }
    else if(bLogoType == 0x5A)  // For JPEG LOGO
    {
#ifdef  JPG_SINGLE_FRAME_BUFFER
        // LLY2.51, two solutin while JPEG w/ effect to LOGO
        // [1] Black the screen before LOGO display
        // [2] Complete the JPEG effect, and keep last picture before LOGO display
        // Otherwise, user will see garbage during next JPEG (LOGO) decoding peroid
        // Since, JPEG w/ effect is always active display @ frame 2
        // and next JPEG decoding (LOGO) will use frame 1 & 2
        // Notice: this problem only happen @ only one single display buffer.

        // LLY2.55, call API to reach the desired goal: MM_CompleteJPGEffect()
        // Since, following procedure is done within it from v2.55.
        MM_CompleteJPGEffect(TRUE);

		//++CoCo2.76, LOGO need to do it here because MM_CompleteJPGEffect() can't judge this case by MM stage.
		HALJPEG_EffectConfigure(JPEG_EFFECT_NONE);
#if 0
        if( (__bAttrPlay == ATTR_JPG) && (__bHALJPEGEffect!=HALJPEG_EFFECT_NONE))
        {
#ifdef  BLACK_SCREEN_WHILE_JPEG_EFFECT_TO_LOGO
            // [1] clear frame buffer 0 (for LOGO) first while back from JPEG w/ Effect
            HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_FRAME0);
#else   // #ifdef  BLACK_SCREEN_WHILE_JPEG_EFFECT_TO_LOGO
#ifdef IMAGE_FRAME_SETUP //CoCo2.38a
            extern BYTE __bMMFirstJPEGPlaying;
            WORD wResult;
            // Notice: 1st picture is not to do while non-image frame setup mode.
            //         since JPEG effect only can be enabled by KEY_ANGLE not setup.
            //senshong: need check jpeg decoded status is OK
            wResult = HALJPEG_Status(HALJPEG_DECODE);
            if (!__bMMFirstJPEGPlaying && wResult == JPEG_STATUS_OK)
#endif //IMAGE_FRAME_SETUP
            {
                // Do copy when decode and display ok
                HALJPEG_CopyPictureToDisplayBuffer(JPEG_EFFECT_DISP_BUFFER_2_TO_0);
            }
#endif  // #ifdef  BLACK_SCREEN_WHILE_JPEG_EFFECT_TO_LOGO

            __HALJPEGSetting.bFrameBuffer=0;
            // Notice: must clear JPEG as none first before call HALJPEG_Display()
            //         since, this API will ignore it while JPEG effect on.
            __bHALJPEGEffect = HALJPEG_EFFECT_NONE;
            HALJPEG_Display(__HALJPEGSetting.bFrameBuffer);
            // Notice: must delay 1 VSYNC after change display frame
            DISP_DelayVSYNC(1);
        }
#endif  // #if 0
#endif  // #ifdef  JPG_SINGLE_FRAME_BUFFER



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

        // Start JPEG Slide Show
        // LLY2.56, only can always specify frame index as 0 for JPEG LOGO for single frame buffer mode
        // Others, must toggle frame buffer between 0 and 1
#ifdef  JPG_SINGLE_FRAME_BUFFER
        dwTemp = UTL_ShowJPEG_Slide(JPEG_PARSE_TYPE_NORMAL, 0x0);
#else   // #ifdef JPG_SINGLE_FRAME_BUFFER
        // Notice: consider to provide a API to report current used frame buffer index
        //         ie. don't reference __HALJPEGSetting.bFrameBuffer directly.
        //CoCo2.71, fix the bug about the picture may have garbage when show LOGO in 64M solution
        if (__dwMMJPGDispOK == FALSE)
        {
            dwTemp = UTL_ShowJPEG_Slide(JPEG_PARSE_TYPE_NORMAL, __HALJPEGSetting.bFrameBuffer);
            __dwMMJPGDispOK = TRUE;
        }
        else
        {
			/*
            if (__HALJPEGSetting.bFrameBuffer == 0)
            {
                dwTemp = UTL_ShowJPEG_Slide(0x1);
            }
            else
            {
                dwTemp = UTL_ShowJPEG_Slide(0x0);
            }
			*/

			//CoCo2.77
			if (__wHALJPEGCurrentDisplayBuf)
			{
				dwTemp = UTL_ShowJPEG_Slide(JPEG_PARSE_TYPE_NORMAL, 0);
			}
			else
			{
				dwTemp = UTL_ShowJPEG_Slide(JPEG_PARSE_TYPE_NORMAL, 1);
			}
        }
#endif  // #ifdef JPG_SINGLE_FRAME_BUFFER


        // If decode and display ok, enable LOGO flag as default
        if(dwTemp)
        {
            // Need check if it's necessary to do TV mode setting after JPEG decoding ok
            HAL_AdjustTVMode(DISP_NEXTVSYNC);
            __bLOGO = LOGO_DEFAULT;
		#ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
 		DisplayLogoFlag=FALSE;
		#endif
	
            return TRUE;
        }
        else
        {
            __bLOGO = LOGO_NONE;
		#ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
 		DisplayLogoFlag=FALSE;
		#endif
            return FALSE;
        }
    }
    // For MPEG motion
    else if(bLogoType==0x2C)
    {
    }
   #ifdef SUPPORT_LOGO_AUTO_FULL_SCREEN
 		DisplayLogoFlag=FALSE;
    #endif

    return (FALSE);


#else   // #ifndef NO_LOGO

    // Always return TRUE while define NO_LOGO
    return (TRUE);

#endif  // #ifndef NO_LOGO
}

// Micky_PORTING_OSWRAP
/*

// LLY0.60, Get eCos timer.
//  **************************************************************************
//  Function    :   UTL_GetSysTimer
//  Description :   Get System timer
//  Arguments   :   None
//  Return      :   Current system timer (DWORD unit)
//  **************************************************************************
DWORD    UTL_GetSysTimer(void)
{
#ifdef  SYSTEM_ECOS
    __dwSysTime = (DWORD) cyg_counter_current_value(__ClockHandle);

    // Chuan1.07, System tick is a multiple of 2. We need divide by 2 to obtain increase number.
    __dwCountSystem = __dwSysTime / 2;
#endif
    return __dwCountSystem;
}


//  *********************************************************************
//  Function    :   UTL_DelayTime
//  Description :   Delay until time out
//  Arguments   :   dwCount  : the delay time
//  Return      :   None
//  Side Effect :
//  *********************************************************************
// LLY0.63, expand delay time unit from WORD to DWORD
// Because, COUNT_XXX_SEC is WORD unit, and the add operation maybe larger than 65535
// LLY1.15, modify the API define: remove 2nd parameter and don't return value
//BIT   UTL_DelayTime( DWORD dwCount, BIT btBreak )
void UTL_DelayTime(DWORD dwCount)
{
    _dwTimeStart = UTL_GetSysTimer();

    while ( ( UTL_GetSysTimer()- _dwTimeStart ) < dwCount )
    {
        // LLY0.63, it's un-necessary to do servo monitor control
        // Because, servo is a thread now from CT909.
        OS_YieldThread();
    }
//    return  TRUE;
}
*/


//  *********************************************************************
//  Function    :   UTL_PlayFromTime
//  Description :   Specify the desired time for input bitstream
//  Arguments   :   dwStart, the desired of starting point
//                  dwEnd, the desired time of end point
//  Return      :   TRUE is Successful
//  *********************************************************************
// LLY2.35, give 2nd parameter for end sector address since FAT system need it.
//BIT  UTL_PlayFromTime ( DWORD dwTime )
BIT UTL_PlayFromTime(DWORD dwStart, DWORD dwEnd)
{
    // LLY0.91, clear the __btPlayEnd flag
    // Since, we will issue the new range for parser
    // So, previous parser complete flag is invalid.
    // Otherwise, we can see the following bug:
    // MP3 SCF near the track end --> parser report data sending end --> KEY_SCB
    // --> HAL_RESET_AUDIO --> MM_AudioTrigger(): __btPlayEnd=TRUE & ARem is low
    // So, issue KEY_NEXT to playback next track.
    __btPlayEnd=FALSE;

#ifdef  SUPPORT_ENCODE_JPG_PICTURE
    // LLY2.51, support JPEG playback from SPI path
    if(__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI)
    {
        // LLY2.51, call SrcFile to re-calculate desired rnage from sector to byte unit
        // And, update the start/end value again.
        SrcFilter_ReadSectors(__dwSFStreamID, dwStart, dwEnd);
        dwStart = __dwSFSPIStartAddr;
        dwEnd = __dwSFSPIEndAddr;

#ifdef  DEBUG_JPEG_FROM_SPF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Start: %lx, End: %lx for UTL_PlayFromTime()\n", dwStart, dwEnd);
#endif  // #ifdef  DEBUG_JPEG_FROM_SPF

        // Notice: must /4 since the unit is DWORD for HLA_FillVideoBuffer()'s dwSize
        //         and must plus 1 of calculation size since start and end all included.
        HAL_FillVideoBuffer(HAL_VIDEOBUF_MM, (DWORD *)dwStart, (dwEnd-dwStart+1)/4);

        // Micky2.11a, integrate video decoder reset procedure into a API.
        HAL_ResetVideoDecoder(HAL_VIDEO_DECODER1|HAL_VIDEO_DECODER2 );

        return TRUE;
    }
#endif  // #ifdef  SUPPORT_ENCODE_JPG_PICTURE

#ifdef  SUPPORT_FAT_FILE_SYSTEM
    // LLY2.35, need do extra operation for FAT system.
    if ((__bFileSystem == FILE_SYSTEM_FAT12) || (__bFileSystem == FILE_SYSTEM_FAT16)
        || (__bFileSystem == FILE_SYSTEM_FAT32))
    {
        DWORD   dwOffset;
        FAT_CheckUSBSecNum(&dwStart, &dwEnd, __dwTimeBegin, &dwOffset);
        __SF_SourceGBL[__dwSFStreamID].dwBlockOffset = dwOffset;
    }
#endif  // #ifdef  SUPPORT_FAT_FILE_SYSTEM


    // LLY1.05, do protection while parser start cmd fail
    // ie. don't issue servo command continue.
    __bTemp=PARSER_Command( 0, PARSER_CMD_START, 0 );

    if(__bTemp)
    {
        // Tell source filter the desired range and get it
        __bTemp = SrcFilter_ReadSectors(__dwSFStreamID, dwStart, dwEnd);
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__dwSFDirRead == TRUE)
        {
            __bMMAudioStreamPause = TRUE;
            __bMMVideoStreamPause = FALSE;
        }
        else
        {
            __bMMAudioStreamPause = FALSE;
            __bMMVideoStreamPause = TRUE;
        }
#endif
        return ( __bTemp );
    }
    else
    {
        return __bTemp;
    }
}


// LLY2.53 create
//  *************************************************************************
//  Function    :   UTL_PlayFromTime_BGItem
//  Description :   Specify the desired time for background item
//  Arguments   :   dwStart, the desired of starting point
//                  dwEnd, the desired time of end point
//  Return      :   TRUE is Successful
//  Notice      :   This API only can be used for background playback item
//  *************************************************************************
BIT UTL_PlayFromTime_BGItem(DWORD dwStart, DWORD dwEnd)
{
#ifdef  SUPPORT_FAT_FILE_SYSTEM
    // LLY2.35, need do extra operation for FAT system.
    if ((__bFileSystem == FILE_SYSTEM_FAT12) || (__bFileSystem == FILE_SYSTEM_FAT16)
        || (__bFileSystem == FILE_SYSTEM_FAT32))
    {
        DWORD   dwOffset;
        DWORD   dwBegin;

        // Get background playback item range first
        MM_GetRange(__MMBackgroundPlayingItem.wDirID, __wBGPlayItem, &dwBegin, &dwOffset);

        // Don't reference __dwTimeBegin directly
        // Since, it only for forground item.
        //FAT_CheckUSBSecNum(&dwStart, &dwEnd, __dwTimeBegin, &dwOffset);
        FAT_CheckUSBSecNum(&dwStart, &dwEnd, dwBegin, &dwOffset);

        __SF_SourceGBL[BG_ITEM_STREAM_ID].dwBlockOffset = dwOffset;
    }
#endif  // #ifdef  SUPPORT_FAT_FILE_SYSTEM


    // LLY1.05, do protection while parser start cmd fail
    // ie. don't issue servo command continue.
    __bTemp=PARSER_Command( BG_ITEM_STREAM_ID, PARSER_CMD_START, 0 );

    if(__bTemp)
    {
        // Tell source filter the desired range and get it
        __bTemp = SrcFilter_ReadSectors(BG_ITEM_STREAM_ID, dwStart, dwEnd);
        if (__dwSFDirRead == TRUE)
        {
            __bMMAudioStreamPause = FALSE;
            __bMMVideoStreamPause = TRUE;
        }
        else
        {
            __bMMAudioStreamPause = TRUE;
            __bMMVideoStreamPause = FALSE;
        }
        return ( __bTemp );
    }
    else
    {
        return __bTemp;
    }
}


// LLY0.80 create ...
//  ********************************************************************************
//  Function    :   UTL_MovieASTSetting
//  Description :   Do DivX/ Nero Digital/ VOB audio stream relative setting
//  Argument    :   bID, specify the desired audio stream index
//  Return      :   None
//  ********************************************************************************
void UTL_MovieASTSetting(BYTE bID)
{
    PARSER_ESFMT    ESAudio;

    // Nothing need to do if don't exist any audio stream.
    if(!__bASTNs)
    {
        return;
    }

    // LLY1.20, update audio related information while change audio stream
    _UTL_Update_Movie_AudioInfo(__bAttrPlay, bID);

    // LLY2.21, collect AVI/ NeroDigital/ VOB audio stream format together
    if(__bAttrPlay == ATTR_AVI)
    {
        ESAudio = pBitsContent->StreamID.AVI.AudioTrack[bID].ESFormat;
    }
    else
    {
        ESAudio = pBitsContent->StreamID.MPEG.AudioTrack[bID].ESFormat;
    }

    // Get corresponding audio stream type base on desired audio stream ID
    //switch(pBitsContent->StreamID.AVI.AudioTrack[bID].ESFormat)
    switch(ESAudio)
    {
    // LLY0.90, split MP12 & MP3 since OSD will reference it.
    case    PARSER_ESFMT_MP3:
        __bAudioType=HAL_AUDIO_MP3;
        break;
    case    PARSER_ESFMT_MP12:
        __bAudioType=HAL_AUDIO_MPG;
        break;
    case    PARSER_ESFMT_AC3:
        __bAudioType=HAL_AUDIO_AC3;
        break;
    case    PARSER_ESFMT_DTS:
        __bAudioType=HAL_AUDIO_DTS;
        break;
    case    PARSER_ESFMT_PCM:
        // LLY2.21, need set as LPCM for non-AVI file's audio stream
        // Since, parser all think it as PCM
        if(__bAttrPlay == ATTR_AVI)
            __bAudioType=HAL_AUDIO_PCM;
        else
            __bAudioType=HAL_AUDIO_LPCM;
        break;
    case    PARSER_ESFMT_OGG: //???
        break;
    // LLY0.91, Support audio stream is WMA format
    // Notice: set audio ID as non-exist temporally since DSP code don't support it.
    case    PARSER_ESFMT_WMA_V1:
    case    PARSER_ESFMT_WMA_V2:
        __bAudioType=HAL_AUDIO_WMA;
        break;
    // LLY2.10, new added AAC format for Nero Digital
    case    PARSER_ESFMT_AAC:
        __bAudioType=HAL_AUDIO_AAC;
        break;
    default:
        // LLY2.55, always set as unknown audio type for un-implement audio type.
        __bAudioType=HAL_AUDIO_NONE;
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "un-implement audio type: %lx \n", ESAudio);
        break;
    }

    // LLY2.10, don't to do audio setting continue while throw audio stream
    // Ntoce: __bASTID is ready after _UTL_Update_Movie_AudioInfo()
    //        And, can return this API after __bAudioType is setting (since OSD will reference it) -- LLY2.30
    if(__bASTID == HAL_THROW_AUDIO)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Not do audio setting since skip audio stream\n");
        return;
    }

    // Do audio type setting for DSP
    HAL_SetAudioType(__bAudioType);

    // LLY1.02, enable audio frequency setting flag
    // Since the sample rate maybe different between DivX title multi-audio channel
    // Otherwise, the audio output is abnormal for DivX Certification title file 'e08" AST=5
    __bCheckAudioType |= CHECK_AUDIO_FREQ;

    // LLY1.06, must tell DSP to stop after audio frequency detection done
    // And, F/W will issue start command continue.
    // Otherwise, we may hear audio frequency wrong while beginning.
    HAL_WriteAM(HAL_AM_START_DECODE, 0);

}


// LLY1.01 create ...
//  ********************************************************************************
//  Function    :   UTL_PreScanAction
//  Description :   Issue the pre-scan command for CDROM motion file to get the
//                  necessary information, ex.
//                  [1] DivX/ AVI necessary information, build index table
//                  [2] 1st/ last PTS/ SCR value for VOB/ DAT/ MPG
//  Arguments   :   wMode, specfy the desired prescan mode
//                  dwStart, specify the start sector of the desired range
//                  dwEnd, specify the end sector of the desired range
//  Return      :   TRUE, issue the command successfully
//                  FALSE, issue the command failure
//  ********************************************************************************
// Need to remove it while parser define this bitstream format
// LLY1.20, add MP4 prescan relative procedure, and extend prescan mode unit from BYTE to WORD
// LLY1.50. re-adjust the procedure since integrate MP4 prescan action.
BYTE UTL_PreScanAction(WORD wMode, DWORD dwStart, DWORD dwEnd)
{
    DWORD dwContent;
    BYTE  bRet;

    if(wMode==PRESCAN_MOTION_INFO || wMode==PRESCAN_1ST_SCR)
    {
#ifdef  DEBUG_PRESCAN
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Do pre-scan\n");
#endif  // #ifdef  DEBUG_PRESCAN
    }

    // LLY1.21, reset the playback mode and attribute since pre-scan action maybe affect parser setting
    // So, following playback setting must be redo. Otherwise, playback maybe fail.
    // Ex, play JPG first --> try to playback a un-support motion --> playback JPG again. (fail)
    // Since, playback mode an attribute are same and nothing will be done within UTL_SetPlayMode()
    __bModePlay=MODE_PLAYUNKNOW;
    __bAttrPlay=ATTR_NONE;

    // Step 0: Always stop parser & source before each pre-scan action start -- LLY1.05
    //         Otherwise, it may cause procedure abnormal while receive any key before pre-scan action done
    HAL_ControlParser(HAL_PARSER_STOP, FALSE);
    SrcFilter_Stop(__dwSFStreamID);

    // Step 1: enable parsing user data
    PARSER_ActionFilter( 0, PARSER_ENABLE_DATA, TRUE );

    // Step 2: Specify the bitstream format for parser base on differnt source
    if(__bAttrPlayNew==ATTR_AVI) // AVI/ MP4
    {

    // Grievous2.78, preconfig avi buffer earlier to prevent the time 
    // that config sp internal/external buffer mode is later than call parser char sp
    if(__bUTLSPTextMode == 2)
    {
        __bUTLSPTextMode = 1;
    _UTL_PreConfig_AVIBuffer();
    }
        
#ifdef  SUPPORT_MP4
        if(MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem)==EXTNAME_MP4)
        {
            PARSER_SetDataFmt(0, PARSER_DATAFMT_NOCHANGE,PARSER_BSFMT_MP4);
        }
        else
#endif  // #ifdef SUPPORT_MP4
        {
            PARSER_SetDataFmt(0, PARSER_DATAFMT_NOCHANGE,PARSER_BSFMT_AVI);
        }
    }
    else // Other motion file
    {
        // set data format as MUXED for MPG/ VOB/ DAT file
        PARSER_SetDataFmt(0, PARSER_DATAFMT_NOCHANGE, PARSER_BSFMT_MUXED);
    }

    // Step 3: specify the desired parsing type
    if( wMode == PRESCAN_MOTION_INFO ) // prescan motion file header information
    {
#ifdef  DEBUG_PRESCAN
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Pre-scan motion info. \n");
#endif  // #ifdef  DEBUG_PRESCAN

        // Must initial related information if 1st pre-scan action
        // LLY2.36, remove container buffer address initial action to UTL_PreConfig_AVIBuffer()
        // Just the same time while other necessary buffer initial action.
        // And, it only be done once before pre-scan.
        if(dwStart == __dwTimeBegin)
        {
#ifdef  DEBUG_PRESCAN
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Initial Container prescan \n");
#endif  // #ifdef  DEBUG_PRESCAN

#ifdef  SUPPORT_MP4
            if(MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem)==EXTNAME_MP4)
            {
                PARSER_InitialContainerPrescan(DS_MP4_TMP_INDEX_TBL_ST, DS_MP4_TMP_INDEX_TBL_LEN, DS_MP4_INDEX_TBL_ST, DS_MP4_INDEX_TBL_LEN);
            }
            else
#endif  // #ifdef SUPPORT_MP4
            {
                // LLY2.36, base on current used F/W buffer end address to config AVI index buffer
#ifdef  SET_AVI_IDX_SP_SERVO_BUFF_RUNTIME
                PARSER_InitialContainerPrescan(DS_AVI_TMP_INDEX_TBL_ST, DS_AVI_TMP_INDEX_TBL_LEN, __dwMMFWBuffEnd, 0x3000L);
#else   // #ifdef  SET_AVI_IDX_SP_SERVO_BUFF_RUNTIME
                PARSER_InitialContainerPrescan(DS_AVI_TMP_INDEX_TBL_ST, DS_AVI_TMP_INDEX_TBL_LEN, DS_AVI_INDEX_TBL_ST, DS_AVI_INDEX_TBL_LEN);
#endif  // #ifdef  SET_AVI_IDX_SP_SERVO_BUFF_RUNTIME
            }
        }

        // specify the prescan mode as parsing motion information.
        dwContent=PARSER_PRESCAN_CONTAINER_INFO;

    }
    else if(wMode == PRESCAN_MOTION_IDXTBL) // prescan motion index table
    {
#ifndef SIMP_UTL

#ifdef  DEBUG_PRESCAN
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Pre-scan motion index table. \n");
#endif  // #ifdef  DEBUG_PRESCAN

        // [1] Get index table starting sector first
        __dwTemp = PARSER_GetContainerIndexPosition(dwStart);

        // LLY0.96, add protect while the index table sector is out-of range
        // ie. the index table maybe no-exist
        __bNoDivXIndexTbl=FALSE;
        if(__dwTemp<dwStart || __dwTemp>=dwEnd)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Index Table position is out-of range \n");
            __bNoDivXIndexTbl=TRUE;

            // LLY1.05-1, return FALSE while index table un-exist
            // Then, MM module will go to set playing mode stage directly.
            return (FALSE);
        }
        else
        {
            // [2] Give the desired range for source to send data
            dwStart=__dwTemp;

            // [3] Specify the pre-scan mode as prescan motion index table
            dwContent = PARSER_PRESCAN_CONTAINER_INDEX;
        }
#endif  // #ifndef SIMP_UTL
    }
    else if(wMode & PRESCAN_SCR_VALUE) // prescan VOB/ DAT/ MPG 1st & last SCR value
    {
#ifdef  DEBUG_PRESCAN
        if(wMode == PRESCAN_1ST_SCR)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Pre-scan 1st SCR value \n");
        }
        else if(wMode == PRESCAN_LAST_SCR)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Pre-scan last SCR value \n");
        }
#endif  // #ifdef DEBUG_PRESCAN


        // specify the prescan mode as find SCR
        dwContent=PARSER_PRESCAN_SCR;

        // F100CSC_108, for reset the track counter at 1st time
        if( (dwStart == __dwTimeBegin) && (wMode == PRESCAN_1ST_SCR) )
        {
            dwContent = PARSER_PRESCAN_1STSCR;
        }
    }

    /// LLY2.30, porting Elmer's code for FAT system play frome time
#ifdef SUPPORT_FAT_FILE_SYSTEM
    if ((__bFileSystem == FILE_SYSTEM_FAT12) || (__bFileSystem == FILE_SYSTEM_FAT16)
	    || (__bFileSystem == FILE_SYSTEM_FAT32))
    {
        DWORD dwOffset;
	    FAT_CheckUSBSecNum(&dwStart, &dwEnd, __dwTimeBegin, &dwOffset);
    	__SF_SourceGBL[__dwSFStreamID].dwBlockOffset = dwOffset;
    }
#endif //

    // Step 5: give the desired range for source to send data
    SrcFilter_ReadSectors(__dwSFStreamID, dwStart, dwEnd);

    // Step 6: Issue the pre-scan command
    bRet=PARSER_Command(0, PARSER_CMD_PRESCAN, &dwContent);

    if(bRet)
    {
        return (TRUE);
    }
    else
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "PreScan Action fail: %lx\n", wMode);
        return (FALSE);
    }

}


// LLY2.36 create
//  ***************************************************************************************
//  Function    :   UTL_PreConfig_AVIBuffer
//  Description :   Used to pre-config some AVI buffer, ex AVI index, SP, servo buffer
//                  Since, those buffer must be ready before do pre-scan.
//  Arguments   :   None
//  Return      :   None
//  Notice      :   Only be called before do pre-scan, ex UTL_Init_PlayItem()
//  ***************************************************************************************
void _UTL_PreConfig_AVIBuffer(void)
{
    DWORD   dwSPStart, dwSPSize;
    DWORD   dwServoSize;
    DWORD   dwSPOutStart=DS_SP_OSD_ST;

#ifdef  SET_AVI_IDX_SP_SERVO_BUFF_RUNTIME
#if 0
    // Step 0: Initial container for pre-scan action.
#ifdef  DEBUG_PRESCAN
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Initial Container address\n");
#endif  // #ifdef  DEBUG_PRESCAN

#ifdef  SUPPORT_MP4
    if(MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem)==EXTNAME_MP4)
    {
        PARSER_InitialContainerPrescan(DS_MP4_TMP_INDEX_TBL_ST, DS_MP4_TMP_INDEX_TBL_LEN, DS_MP4_INDEX_TBL_ST, DS_MP4_INDEX_TBL_LEN);
    }
    else
#endif  // #ifdef SUPPORT_MP4
    {
        // LLY2.36, base on current used F/W buffer end address to config AVI index buffer
#ifdef  SET_AVI_IDX_SP_SERVO_BUFF_RUNTIME
        PARSER_InitialContainerPrescan(DS_AVI_TMP_INDEX_TBL_ST, DS_AVI_TMP_INDEX_TBL_LEN, __dwMMFWBuffEnd, 0x3000L);
#else   // #ifdef  SET_AVI_IDX_SP_SERVO_BUFF_RUNTIME
        PARSER_InitialContainerPrescan(DS_AVI_TMP_INDEX_TBL_ST, DS_AVI_TMP_INDEX_TBL_LEN, DS_AVI_INDEX_TBL_ST, DS_AVI_INDEX_TBL_LEN);
#endif  // #ifdef  SET_AVI_IDX_SP_SERVO_BUFF_RUNTIME
    }
#endif  // #if 0

    // [1] If exist internal SP,
    // Grievous2.78, change an variable to determine internal SP mode
    // Since __bSPSTNs is not for the usage.
    // if(__bSPSTNs)
    if(__bUTLSPTextMode)
    {
        // [1-1] always set SP buffer as 13K DW
        // Notice: index table is near F/W buffer, and size is 3K DW
        dwSPStart = __dwMMFWBuffEnd + 0x3000L;
        dwSPSize = 0xD000L;
    // Grievous2.78, add SMALL_SOURCE_BUFFER case for different buffer address
#ifdef SMALL_SOURCE_BUFFER
        dwServoSize = DS_INPUT_BITBUF_END_AVI_SMALL - DS_INPUT_BITBUF_ST_AVI_SMALL;
#else
        // [1-2] Let servo buffer = total reserved space - F/W used buffer - 3K index table - 13K SP buffer
        // So, calculate the availabe servo buffer first
        dwServoSize = DS_INPUT_BITBUF_END_AVI - __dwMMFWBuffEnd - 0x3000L - dwSPSize;
        dwServoSize = (dwServoSize/SERVO_BUFSIZE_ALIGN_UNIT)*SERVO_BUFSIZE_ALIGN_UNIT; // 1 servo blok = 10 KDW
#endif
    }
    // [2] If don't exist internal SP,
    else
    {
        // [2-1] Always let servo buffer as 3 block
        // wyc2.76-909P
#ifdef SMALL_SOURCE_BUFFER
        dwServoSize = DS_INPUT_BITBUF_END_AVI_SMALL - DS_INPUT_BITBUF_ST_AVI_SMALL;
        // [2-2] Configure SP output buffer as 3K DW (near servo buffer)


        // [2-3] Let SP buffer - total reserved space - F/W used buffer - 3K index table - 3K SP output area - Servo buffer
        dwSPSize = DS_INPUT_BITBUF_END_AVI_SMALL - __dwMMFWBuffEnd - 0x3000L - 0x3000L - dwServoSize;
#else
        dwServoSize = SERVO_BUFSIZE_ALIGN_UNIT*3;
        // [2-2] Configure SP output buffer as 3K DW (near servo buffer)


        // [2-3] Let SP buffer - total reserved space - F/W used buffer - 3K index table - 3K SP output area - Servo buffer
        dwSPSize = DS_INPUT_BITBUF_END_AVI - __dwMMFWBuffEnd - 0x3000L - 0x3000L - dwServoSize;
#endif //
        dwSPStart = __dwMMFWBuffEnd + 0x3000L;

        if(dwSPSize < 0xA000)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Note: SP buffer < 10K DW !!\n");
        }

        // [2-4] Config SP output buffer as 3K (near servo buffer)
#ifdef SMALL_SOURCE_BUFFER
        dwSPOutStart = DS_INPUT_BITBUF_END_AVI_SMALL - dwServoSize - 0x3000L;
#else
        dwSPOutStart = DS_INPUT_BITBUF_END_AVI - dwServoSize - 0x3000L;
#endif
    }



#else   // #ifdef  SET_AVI_IDX_SP_SERVO_BUFF_RUNTIME
    // Always let servo buffer as pre-define space
    dwServoSize = DS_INPUT_BITBUF_END_AVI - DS_INPUT_BITBUF_ST_AVI;

    // Always set SP buffer as 13K DW by pre-define range while exist internal SP
    if(__bSPSTNs)
    {
        dwSPStart = DS_SP1BUF_ADDR_ST_MM;
        dwSPSize = DS_SP1BUF_ADDR_END_MM - DS_SP1BUF_ADDR_ST_MM;
    }
    else
    {
        // Configure char-based SP buffer
#ifdef  EX_SP_SHARE_WITH_FIRMWARE_BUFFER
        dwSPStart = __dwMMFWBuffEnd;
        dwSPSize = DS_CHAR_BASED_SP_OUTBUF_END - dwSPStart;
#else   // #ifdef  EX_SP_SHARE_WITH_FIRMWARE_BUFFER
        dwSPStart = DS_CHAR_BASED_SP_OUTBUF_ST;
        dwSPSize = (DS_CHAR_BASED_SP_OUTBUF_END - DS_CHAR_BASED_SP_OUTBUF_ST);
#endif  // #ifdef  EX_SP_SHARE_WITH_FIRMWARE_BUFFER

        // Configure SP output buffer as 3K DW base on pre-define area
        dwSPOutStart = DS_SP_OSD_ST;
    }

#endif  // #ifdef  SET_AVI_IDX_SP_SERVO_BUFF_RUNTIME


//    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "##### %lx, %lx, %lx, %lx, %lx\n", dwSPStart, dwSPSize,
//    (dwSPStart+dwSPSize), DS_SP1BUF_ADDR_ST_MM, DS_SP1BUF_ADDR_END_MM);


    // Config SP buffer address.
    PARSER_SetOutBufferAddress(PARSER_ES_SP1, dwSPStart, (dwSPStart+dwSPSize));


    // Config Servo dump-in buffer address
#ifdef SMALL_SOURCE_BUFFER
    SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_END_AVI_SMALL-dwServoSize,
#else
    SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_END_AVI-dwServoSize,
#endif //
        (dwServoSize/SERVO_BUFSIZE_ALIGN_UNIT) );

    // Parser data dump-out buffer, let it same as SP buffer
    PARSER_SetOutBufferAddress((DWORD)PARSER_ES_DATA, dwSPStart, (dwSPStart+dwSPSize));

    // Tell infofilter the user data mode as media manager mode.
    INFOFILTER_CFGUserData(dwSPStart);

    // Config char-based SP buffer address & output address
    if(!__bSPSTNs)
    {
        CHAR_SP_SetOutputBuffer(dwSPStart, dwSPSize);

        GDI_ConfigRegionAddress(GDI_SP_REGION_ID, dwSPOutStart);
    }


#ifdef  DEBUG_AVI_IDX_SP_SERVO_BUFFER
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "SP: %lx - %lx; Servo: %lx - %lx (%lx)\n",
        dwSPStart, (dwSPStart+dwSPSize), (DS_INPUT_BITBUF_END_AVI-dwServoSize),
        (DS_INPUT_BITBUF_END_AVI), (dwServoSize/SERVO_BUFSIZE_ALIGN_UNIT));
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "UserData: %lx - %lx; SP Display: %lx\n",
        dwSPStart, (dwSPStart+dwSPSize), dwSPOutStart);

#endif  // #ifdef  DEBUG_AVI_IDX_SP_SERVO_BUFFER

}




// LLY1.20 create, collect procedure together while updata AVI audio information
// LLY2.21, add one new paramter to specify desired mode since integrate VOB audio information together.
//  ******************************************************************************
//  Function    :   _UTL_Update_Movie_AudioInfo
//  Description :   Update AVI/ VOB audio information while change audio stream
//  Argument    :   bMode, specify desired file format, AVI or others
//                  bID, specify the desired AST index, from 0
//  Return      :   None
//  *****************************************************************************
void _UTL_Update_Movie_AudioInfo(BYTE bMode, BYTE bID)
{
    if(bMode == ATTR_AVI)
    {
        // LLY2.21, update audio stream ID as index for AVI file
        __bASTID = bID;

        // [1] Audio channel information -- LLY1.00
        __bACHN=pBitsContent->StreamID.AVI.AudioTrack[bID].bChannelNo;

        // Grievous2.60, record bit per sample value
        __bBitsPerSample=(pBitsContent->StreamID.AVI.AudioTrack[bID].bBitsPerSample);

        // [2] Sample frequency -- LLY1.00
        __dwHALAudioFreq=pBitsContent->StreamID.AVI.AudioTrack[bID].dwSampleRate;

        // [3] Filter the un-supported audio stream
        switch(pBitsContent->StreamID.AVI.AudioTrack[bID].ESFormat)
        {
        // LLY2.17, support AAC audio format
#if 0
        case    PARSER_ESFMT_AAC: //???
            // LLY1.02, set audio ID as HAL_THROW_AUDIO for un-support format
            __bASTID=HAL_THROW_AUDIO;
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AAC audio\n");
            break;
#endif  // #if 0

        case    PARSER_ESFMT_OGG: //???
            // LLY1.02, set audio ID as HAL_THROW_AUDIO for un-support format
            __bASTID=HAL_THROW_AUDIO;
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "OGG audio\n");
            break;
        // LLY0.91, Support audio stream is WMA format
        // Notice: set audio ID as non-exist temporally since DSP code don't support it.
        case    PARSER_ESFMT_WMA_V1:
        case    PARSER_ESFMT_WMA_V2:
#ifdef  SUPPORT_WMA
            // LLY1.05, assign AST ID as HAL_THROW_AUDIO while don't support WMA chip version
            if( (__dwSupportFeature & SUPPORT_FEATURE_WMA)!= SUPPORT_FEATURE_WMA)
            {
                __bASTID=HAL_THROW_AUDIO;
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "WMA audio\n");
            }
#else   // #ifdef SUPPORT_WMA
            // LLY0.96, assign AST ID as HAL_THROW_AUDIO while don't support WMA
            // To tell parser to skip the audio stream
            __bASTID=HAL_THROW_AUDIO;
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "WMA audio\n");
#endif  // #ifdef SUPPORT_WMA
            break;

        // LLY1.20, skip the audio stream if PCM w/ mono
        // Notice: must remove following preocedure after audio code is ready to support mono PCM
        // LLY2.60, remove this procedure since audio code is ready to support PCM mono
#if 0
        case    PARSER_ESFMT_PCM:
            if(pBitsContent->StreamID.AVI.AudioTrack[bID].bChannelNo==1)
            {
                __bASTID=HAL_THROW_AUDIO;
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "PCM audio w/ mono \n");
            }
            break;
#endif  // #if 0
        // Grievous2.77, show message and discard bitstream while the ADPCM audio format is not supported.
        case PARSER_ESFMT_MS_ADPCM:
        case PARSER_ESFMT_DVI_ADPCM:
        // Grievous2.78, add ALaw, MuLaw, AAC audio type when we have not supported yet.
        case PARSER_ESFMT_PCM_ALAW:
        case PARSER_ESFMT_PCM_MULAW:
            __bASTID=HAL_THROW_AUDIO;
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "ADPCM audio\n");
            break;

        case PARSER_ESFMT_AAC:
            __bASTID=HAL_THROW_AUDIO;
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AAC audio\n");
            break;

        default:
            break;
        }
    }
    else
    {
        // LLY2.21, update audio stream ID base on parser reported ID
        __bASTID=(pBitsContent->StreamID.MPEG.AudioTrack[bID].dwID)&0xF;
#ifdef  DEBUG_AUDIO_ID
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "__bASTID: %lx, parser ID: %lx, Idx: %lx\n", __bASTID,
            pBitsContent->StreamID.MPEG.AudioTrack[bID].dwID, bID);
#endif  // #ifdef DEBUG_AUDIO_ID
    }
}


// LLY2.21 create ...
//  ********************************************************************************
//  Function    :   _UTL_GetBitsContent
//  Description :   Get bitstream content and keep at global variables
//  Arguments   :   bMode, specify the desired bits content mode
//  Return      :   None
//  ********************************************************************************
void _UTL_GetBitsContent(BYTE bMode)
{
    switch(bMode)
    {
    case    INFO_AUDIO:
        // [1-1] Total AST number
        // Notice: add protection while no audio stream exist -- LLY0.80
        __bASTNs=(BYTE)pBitsContent->dwAudioTrackCount;
        if(__bASTNs)
        {
            // [1-2] Assume to choose 1st audio stream ie. 0
            __bASTNO=0;

            // LLY2.21, remoe audio stream ID assignment procedure to _UTL_Update_Movie_AudioInfo()
            // Since, the ID maybe different the index.

            // Find 1st non-DTS audio stream first
            // Notice: this procedure can't put @ _UTL_Update_Movie_AudioInfo(),
            //         since it will be called while multi-audio switching.
            //         And, find 1st non-DTS procedure is only need for normal playback.
#ifdef  FIND_NODTS_AUDIO
            while(__bASTNO < __bASTNs)
            {
                if(pBitsContent->StreamID.MPEG.AudioTrack[__bASTNO].ESFormat != PARSER_ESFMT_DTS)
                {
                    break;
                }
                else
                {
                    __bASTNO++;
#ifdef  DEBUG_AUDIO_ID
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Force change to next audio stream since DTS audio\n");
#endif  // #ifdef DEBUG_AUDIO_ID

                }
            }

            if(__bASTNO == __bASTNs)
            {
                __bASTNO=0;
#ifdef  DEBUG_AUDIO_ID
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Force restore to 1st audio since all DTS\n");
#endif  // #ifdef DEBUG_AUDIO_ID
            }
#endif  // #ifdef FIND_NODTS_AUDIO


            // LLY1.20, collect procedure toghther while update MP4/ AVI/ DivX/ Nero Digital AST information
            _UTL_Update_Movie_AudioInfo(__bAttrPlayNew, __bASTNO);

        }
        else
        {
            // [1-2] Assume to set as un-exist audio stream ID
            __bASTNO=0xFF;

            // [1-3] Assume audio stream ID as skip
            __bASTID=HAL_THROW_AUDIO;
        }

        // [1-4] Audio stream language code information
        // Notice: assume it un-exist, and set is as 0xFFFF -- LLY0.90
        __wA_LCD=0xFFFF;

        break;

    case    INFO_SPST:
        // [2-1] Total SPST number
        // Notice: add protection while no subpicture stream exist -- LLY0.80
        __bSPSTNs=(BYTE)pBitsContent->dwSubtitleTrackCount;
       
        // Grievous2.78, read internal subpicture stream total number
        __bUTLSPTextMode = (BYTE)pBitsContent->dwSubtitleTrackCount;

        if(__bSPSTNs)
        {
            // [2-2] Assume to choose 1st SPST stream, ie. 0
            __bSPSTNO=0;
        }
        else
        {
            // [2-2] Assume to set as un-exist SPST stream ID
            __bSPSTNO=0xFF;
        }

        // [2-3] SPST stream ID
        // Let subpicture ID is same as subpicture stream index
        // Then, parser will convert to corresponding subpicture ID automatically
        __bSPSTID=__bSPSTNO;


        // [2-4] SPST stream language code information
        // Notice: assume it un-exist, and set is as 0xFFFF -- LLY0.90
        __wSP_LCD=0xFFFF;

        break;

    case    INFO_AVI_VIDEO:
        // Set video ID for DivX file
        // [1] Throw the video case:
        //     - video track count is zero -- LLY1.10
        //     - not support DivX format chip -- LLY1.10
        //     - resolution > 720*576 -- LLY0.85
        // [2] Others: set the video ID as zero
        __bVideoID=0x0;
        if(pBitsContent->dwVideoTrackCount == 0)
        {
            __bVideoID=HAL_THROW_VIDEO;
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Skip video since no video track\n");
        }
        else
        {
            // LLY2.55, keep video content format
            __dwAVIVideoFMT = pBitsContent->StreamID.AVI.VideoTrack[__bVideoID].ESFormat;

            // LLY2.38, add error protection while unknown AVI video format
            if(__dwAVIVideoFMT == PARSER_ESFMT_UNKNOWN)
            {
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Skip the unknown video format\n");
                __bVideoID=HAL_THROW_VIDEO;
            }
            else if( (__dwSupportFeature & SUPPORT_FEATURE_DIVX)!= SUPPORT_FEATURE_DIVX )
            {
                switch(__dwAVIVideoFMT)
                {
                case    PARSER_ESFMT_DIVX311:
                // Marked by J500CSC, 238a
                // Treat DivX 4/5 as normal MPEG-4 streams
                //case    PARSER_ESFMT_DIVX4:
                //case    PARSER_ESFMT_DIVX5:
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Skip the DivX 311 format file since don't support DivX feature\n");
                    __bVideoID=HAL_THROW_VIDEO;
                    break;

                default:
                    break;
                }
            }
            else
            {
                // LLY2.18, using negative define for DivX 311 supporting
#ifdef  NO_DIVX_311
                if(__dwAVIVideoFMT== PARSER_ESFMT_DIVX311)
                {
                    __bVideoID=HAL_THROW_VIDEO;
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Skip video since don't support DivX311 for this model\n");
                }
                else
#endif  // #ifdef NO_DIVX_311
                {
                    if( (pBitsContent->dwWidth > 720) || (pBitsContent->dwHeight > 576) )
                    {
                        __bVideoID=HAL_THROW_VIDEO;
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Skip video since resolution is (%lx, %lx) > (720, 576) !!\n",
                            pBitsContent->dwWidth, pBitsContent->dwHeight);
                    }

    				// Chuan2.22, Judge to do BFR or not when AVI mode.
#ifdef SUPPORT_BFR_MODE
#ifdef CONFIG_BFR_BY_FW
                    if (pBitsContent->dwHeight <= 480)
                    {
                        UTL_BFRModeSet(FALSE);
                    }
                    else
                    {
                        UTL_BFRModeSet(TRUE);
                    }
#endif  // #ifdef CONFIG_BFR_BY_FW
#endif  // #ifdef SUPPORT_BFR_MODE
                }
            }
        }
        break;

    case    INFO_FILETIME:
        __dwFileTime = ((ULONGLONG)(pBitsContent->dwTotalFrames)
            * (ULONGLONG)(pBitsContent->dwMicrosecPerFrame) / (ULONGLONG)(1000000));

#ifdef  DEBUG_TOTAL_TIME
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Total Frame: %lx, MsecPerFrame: %lx, Total File Time: %lx\n",
            pBitsContent->dwTotalFrames, pBitsContent->dwMicrosecPerFrame, __dwFileTime);
#endif  // #ifdef DEBUG_TOTAL_TIME

        // LLY0.95, calculate the sector per-second value
        // Notice: assign it as 150 if total file time is zero
        if(__dwFileTime)
        {
            __dwSectorPerSec = (__dwTimeEnd - __dwTimeBegin) / __dwFileTime;
        }
        else
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: AVI total file time is zero !!\n");
            // LLY0.80, assume the sector per second for AVI as 150
            // Otherwise, SCAN mode can't do corresponding jump action.
            __dwSectorPerSec = 150;
        }
        break;
    }
}



// LLY1.01 create ...
//  ********************************************************************************
//  Function    :   UTL_Read_PreScan_Info
//  Description :   Read back the necessary information after pre-scan action done
//  Argument    :   bMode, specfy the desired prescan mode
//  Return      :   None
//  ********************************************************************************
// LLY2.21, collect readback bitstream content information procedure into a subroutine.
void UTL_Read_PreScan_Info(WORD wMode)
{
    // Step 1: read back the necessary information
    if(wMode & PRESCAN_MOTION_INFO)
    {
        // LLY0.95, do DivX DRM playback state if find Copy Protection information
        // Otherwise, continue the playback.
        // Notice: nothing need to do if not support DRM application.
        if(__PARSER_gbl.BitsContent.fCopyProtection)
        {
            // LLY1.50, Skip the file if MP4 w/ copy protection
#ifdef  SUPPORT_MP4
            if(MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem)==EXTNAME_MP4)
            {
                __bDRMPlayState=DRM_PLAY_STATE_SKIP;
                return;
            }
            else
#endif  // #ifdef SUPPORT_MP4
            {
#ifdef  NO_DRM
                __bDRMPlayState=DRM_PLAY_STATE_SKIP;
                return;
#else   // #ifdef  NO_DRM
                __bDRMPlayState=UTL_DRM_PlaybackCapability();
#endif  // #ifdef  NO_DRM
            }
        }
        else
        {
            __bDRMPlayState=DRM_PLAY_STATE_CONTINUE;
        }


        // [0] keep the pointer address to the global variable to keep DivX bits content structure
        pBitsContent = PARSER_GetContainerContent();

        // [1] call API to get audio stream related information -- LLY2.21
        _UTL_GetBitsContent(INFO_AUDIO);

        // [2] call API to get subpicture related information -- LLY2.21
        _UTL_GetBitsContent(INFO_SPST);

        // LLY0.80, reset the 1st SCR value as zero
        __dw1stSCR=0;

        // Call API to get AVI video stream related information -- LLY2.21
        _UTL_GetBitsContent(INFO_AVI_VIDEO);

        // Call API to get total file time -- LLY2.21
        _UTL_GetBitsContent(INFO_FILETIME);

        // LLY2.36, pre-config AVI related buffer, ex SP buffer, servo buffer
        // Grievous2.78, preconfig avi buffer again since the SP is external mode 
        if(__bAttrPlayNew == ATTR_AVI)
        {
            if(__bUTLSPTextMode == 0)
            {
            _UTL_PreConfig_AVIBuffer();
        }
        }

    }
    else if(wMode & PRESCAN_MOTION_IDXTBL) // read-back informatin for AVI index table
    {
        // LLY2.16, calculate the total file time after index table parsing done
        // Since, the value maybe updated if can't playback the whole file, ex Nero Digital
        // Time (sec) = (total frame) * (msec/frame) / (1000000)
#ifdef  SUPPORT_MP4
        if( MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem) == EXTNAME_MP4 )
        {
            // Call API to get file total time -- LLY2.21
            _UTL_GetBitsContent(INFO_FILETIME);

            // LLY1.50, keep current chapter and total chapter number
            // Notice: read it after index table parsing done
            //         since the value maybe updated if can't playback the whole file
            __wChapter_Ns_Title = pBitsContent->dwChapterCount;

            // And, reset current chapter as 1
            __wChapter = 1;
        }
#endif  // #ifdef SUPPORT_MP4

    }
    else if(wMode & PRESCAN_SCR_VALUE) // get 1st/ last SCR value for VOB/ DAT/ MPG
    {
        if(wMode == PRESCAN_1ST_SCR)
        {
            // get 1st SCR value
            PARSER_GetSCR(&__dw1stSCR);

            // LLY1.11, Assume multi-audio/ subpicture number is 1, and ID is 0 for *.Dat/ *.MPG/ *.VOB
            // Notice: we must assign the correct value after parser code is ready.
            __bASTNs=1;
            __bASTID=0;
            __bASTNO=__bASTID;

            __bSPSTNs=0;
            __bSPSTID=0xFF;
            __bSPSTNO=__bSPSTID;

            // LLY1.11, also set SPST/ AST stream language code as 0xFFFF
            // Notice: assume it un-exist, and set is as 0xFFFF
            __wSP_LCD=0xFFFF;
            __wA_LCD=0xFFFF;

        }
        else if(wMode == PRESCAN_LAST_SCR)
        {
            // get last SCR value
            PARSER_GetSCR(&__dwLastSCR);

            // Calculate the total file time
            // LLY2.31, add error protection that 1stSCR must < LastSCR
            if(__dwLastSCR > __dw1stSCR)
            {
                __dwFileTime = (__dwLastSCR-__dw1stSCR)/90000;
            }
            else
            {
                __dwFileTime = 0;
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: LastSCR < 1stSCR\n");
            }

#ifdef  DEBUG_TOTAL_TIME
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "1stSCR: %lx, LastSCR: %lx, Total File Time: %lx\n",
                __dw1stSCR, __dwLastSCR, __dwFileTime);
#endif  // #ifdef DEBUG_TOTAL_TIME

            // LLY0.76-1, calculate the sector number for 1 sec
            // Thus, the calculation become easy for GoTime, SCAN
            // Notice: must add protection while divided by zero
            if(__dwFileTime)
            {
                __dwSectorPerSec = (__dwTimeEnd - __dwTimeBegin) / __dwFileTime;
            }
            else
            {
                 DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: File Time is zero !!\n");
                 // LLY2.21a, assing as 75 sector/ sec for scan mode usage while total time is zero
                __dwSectorPerSec = 75; // 150
            }


            // LLY2.21, readback audio stream related information after last SCR parsing done.
            // keep the pointer address to the global variable to keep bits content structure
            pBitsContent = PARSER_GetContainerContent();

            // Get audio stream related information.
            _UTL_GetBitsContent(INFO_AUDIO);
        }
    }
}


// LLY1.01 create ...
//  ******************************************************************************************
//  Function    :   UTL_SetPreScanError
//  Description :   Do error protection while prescan error
//  Argument    :   bErr, specify the desired error type
//  Return      :   None
//  Side Effect :
//  ******************************************************************************************
void UTL_SetPreScanError(BYTE bErr)
{
    // Stop data sending of source while pre-scan fail
    SrcFilter_Stop(__dwSFStreamID);

    switch(bErr)
    {
    // Preocess parser 1st SCR and AVI table fail
    case    ERR_1ST_PRESCAN_FAIL:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "1st pre-scan fail\n");
        break;

    // Process can't find last SCR value case
    case    ERR_NOT_FIND_LAST_SCR:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find last SCR \n");

        // LLY2.21a, modify the error protection procedure while can't find last SCR case
#if 0
        // Update last SCR and file total time. (assume 3 min)
        __dwLastSCR=__dw1stSCR+90000*180;
        __dwFileTime=180;

        // set it as one for sector within one sec while can't find last SCR
        __dwSectorPerSec=75;
#endif  //
        // Assume:
        // [1] last SCR value = 1st SCR value
        __dwLastSCR = __dw1stSCR;
        // [2] total file time = 0
        __dwFileTime = 0;
        // [3] set it as 75sector/sec (only for scan mode)
        __dwSectorPerSec=75;
        break;

    // Process can't find the AVI index table
    case    ERR_NOT_FIND_AVI_IDXTBL:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err : Can't find the index table by parser\n");
        __bNoDivXIndexTbl=TRUE;
        break;

    }
}


//  **********************************************************************************************
//  Function    :   UTL_SetPlayMode
//  Description :   Set relative setting for MPEG chip for desired playing mode
//  Arguments   :   bType, the desired playing mode -- MODE_PLAYXXXX
//                  __bAttrPlayNew, the desired playing mode attribute.
//  Return      :   TRUE
//  Notice      :   [1] Must give desired new playing mode attribute into __bAttrPlayNew, first
//                  [2] Then, call UTL_SetPlayMode() and new playing mode as it's parameter
//                  [3] The desired playing mode will keep in __bModePlay
//                  [4] The desired playing mode attribute in __bAttrPlay
//  **********************************************************************************************
// LLY2.55, declare the ID for bDesiredDecoder
#define JPEG_DECODER_THREAD     0
#define MPEG_DECODER_THREAD     1
#define DIVX_DECODER_THREAD     2
#define UNINIT_DECODER_THREAD   0xff
BYTE UTL_SetPlayMode ( BYTE bType )
{
    BIT         btPseudoReset= FALSE;
    BYTE        bDesiredDecoder=UNINIT_DECODER_THREAD;

#if defined(SUPPORT_POWERON_MUSIC) || !defined(REMOVE_SETUP_SPEAKER_TEST)
    // kevin1.05, restore freq, because test tone may set another freq
    // Notice: Need remove it after F/W can handle save & restore desired audio frequency
    //         while enter/ exit test tone mode.
    // Current Problem: nobody know current audio frequency before enter test tone mode
    // LLY1.06, avoid re-program the audio DAC for un-expected value to cause "po-po" noise
    // Don't program Audio DAC for SVCD/ CDROM title here
    // Since it will be run-time detection and reset within UTL_PlayItem()
    // with __bCheckAudioType |= CHECK_AUDIO_FREQ
    if( (__wDiscType & BOOK_DVD) )
    {
        HAL_SetAudioDAC ( AUDIO_FREQ_48K );
    }
    else if( (__wDiscType & BOOK_SVCD) || (__wDiscType & BOOK_M1) )
    {
        // Nothing need to do.
    }
    else
    {
        HAL_SetAudioDAC ( AUDIO_FREQ_44K );
    }
#endif  // #if defined(SUPPORT_POWERON_MUSIC) || !defined(REMOVE_SETUP_SPEAKER_TEST)



    // Reset playing mode attribute as NONE except
    // [1] MODE_PLAYAUDIO
    // [2] MODE_PLAYVIDEO
    // [3] CDROM title, ex *.vob, *.dat, *.mpg
    if( (bType&MODE_PLAYAUDIO) || (bType&MODE_PLAYVIDEO) || (__wDiscType & CDROM_M1))
    {
    }
    else
    {
        __bAttrPlayNew=ATTR_NONE;
    }

    // Nothing need to do if playing mode and attribue are sme
    if( (bType==__bModePlay) && (__bAttrPlayNew==__bAttrPlay) )
    {
        // LLY0.75, always issue video reset before playback next JPEG file.
        // Notice: can't do it for thumbnail mode
        // And, YC says it will check why need to do this control !!
        // LLY0.80, don't care if thumbnail mode since CoCo says she will modify the flow.
        if( (__bAttrPlayNew==ATTR_JPG)) // && (!__bThumbnailMode) )
        {
            HAL_Reset(HAL_RESET_VIDEO);

            // LLY2.22, config buffer again since Slide Show and Preview are different
            // But, the playback mode are all same as MODE_PLAYVIDEO w/ ATTR_JPG
            UTL_Config_BufferMode(DUMP_BUFF_MM_VIDEO_ONLY_MODE);
	        return FALSE;
        }
	    // LLY2.55, porting Grievous's code : don't think the same mode for AVI w/ different video content.
	    else if(__bAttrPlayNew==ATTR_AVI)
	    {
            switch(__dwAVIVideoFMT)
            {
            // if want to playback MJPEG and current is not JPEG decoding mode configuration.
            case    PARSER_ESFMT_MJPG:
                if( !OS_PeekFlag(&__fThreadInit)&INIT_DEC_THREAD_JPEG_DONE)
                {
                    goto LABEL_SET_MODE;
                }
                break;

            // if want to playback MPEG1/2 and current is not MPEG decoding mode configuration
            case    PARSER_ESFMT_MPEG1:
            case    PARSER_ESFMT_MPEG2:
                if(!OS_PeekFlag(&__fThreadInit)&INIT_DEC_THREAD_MPEG_DONE)
                {
                    goto LABEL_SET_MODE;
                }
                break;

            // if want to playback MPEG4 and current is not MPEG4 decoding mode configuration
            default:
                if(!OS_PeekFlag(&__fThreadInit)&INIT_DEC_THREAD_DIVX_DONE)
                {
                    goto LABEL_SET_MODE;
                }
                break;
            }

            // Only do A/V/SP reset for the same mode, and return directly
            HAL_Reset(HAL_RESET_AVSPST);
            return FALSE;
        }
        else
        {
            // LLY1.05, issue A/V/SP reset even the same mode
            HAL_Reset(HAL_RESET_AVSPST);
            return FALSE;
        }
    }

LABEL_SET_MODE:
    // LLY0.72, record playing mode & attribute information earily.
    // Notice: In previous version, the information will be kept after all setting is ok.
    // So, other function must reference __bAttrPlayNew to know the desired playing mode attribute
    // From now, other function only need to reference __bAttrPlay to know the desired playing mode attribute
    // [1] the desired playing mode
    __bModePlay= bType;

    // [2] record last time mode for compare
    // LLY0.72, __bLastModePlay = __bModePlay;

    // [2] the desired playing mode attribute
    __bAttrPlay=__bAttrPlayNew;

    // [3] clear this value, because it has kept in __bAttrPlay
    __bAttrPlayNew=ATTR_NONE;


    // LLY0.80, reset the SP/ OGT type as none (initial value)
    __dwSPType=SP_TYPE_NONE;

    // LLY2.55, decide the desired video decoder thread base on desired playback mode attribute
    if(__bAttrPlay == ATTR_JPG)
    {
        bDesiredDecoder = JPEG_DECODER_THREAD;
    }
    else if(__bAttrPlay & TYPE_CDROM_AUDIO)
    {
        // No need to change video decoder thread while playback audio only file
        // Otherwise, JPEG thread maybe killed and MP3 playback can't exist w/ JPEG preview together.
    }
    else if(__bAttrPlay == ATTR_AVI)
    {
        switch(__dwAVIVideoFMT)
        {
        case    PARSER_ESFMT_MJPG:
            bDesiredDecoder = JPEG_DECODER_THREAD;
            break;

        case    PARSER_ESFMT_MPEG1:
        case    PARSER_ESFMT_MPEG2:
            bDesiredDecoder = MPEG_DECODER_THREAD;
            break;

        default:
            bDesiredDecoder = DIVX_DECODER_THREAD;
            break;
        }
    }
    else
    {
        bDesiredDecoder = MPEG_DECODER_THREAD;
    }


    // LLY2.01, don't configure parser/ servo dump in buffer here.
    // Now, it will be set during power-on/ open_close/ title detection
#if 0
    // LLY0.70, Set the default input bitstream buffer address always.
    // And, change it as corresponding one if necessary case.
    SrcFilter_ConfigBuffer( 0, DS_INPUT_BITBUF_ST,
        ((DS_INPUT_BITBUF_END-DS_INPUT_BITBUF_ST)/SERVO_BUFSIZE_ALIGN_UNIT) );
#endif  // #if 0

    // Kevin1.00, set TVE gain value for MPEG as default
    __bTVE_JPEG = FALSE;
    //MPEG Display will call HAL_AdjustTVMode


    // LLY0.72, create the desired video decoder thread base playing mode attribute
    // and want to playback a non-JPG item
    // LLY2.55, base on desired video decoder thread to create it.
    // Since, JPEG file and motion JPEG all use JPEG decoder.
    //if(__bAttrPlay == ATTR_JPG)
    if(bDesiredDecoder == JPEG_DECODER_THREAD)
    {
        // LLY2.76, don't set digial panel mode as TRUE here
        // Since, MJPEG/ JPEG go here both. and only still image need set as TRUE
        // Motion (MJPEG must set as FALSE);
        // So, remove it to _UTL__ProgVideoOnly()
        //HAL_AdjustTFT_DigitalMode(TRUE);

        // Create the JPEG thread if it's un-exist
        if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_JPEG_DONE) )
        {
            INITIAL_ThreadInit(THREAD_JPEG_DECODER);

            // LLY0.75, release CPU first before waiting flag
            // Otherwise, may can't get the JPEG thread initial done flag
            // Since, JPEG thread is the same priority as CheerDVD F/W
            OS_YieldThread();

            // LLY0.95, the waiting time is absolute time
            // wait the thread initial done
            //cyg_flag_timed_wait( &__fThreadInit, INIT_DEC_THREAD_JPEG_DONE, CYG_FLAG_WAITMODE_AND, COUNT_50_MSEC);
            /*OS_TimedWaitFlag( &__fThreadInit, INIT_DEC_THREAD_JPEG_DONE, FLAG_WAITMODE_AND,
                (COUNT_50_MSEC) );*/

            OS_TimedWaitFlag( &__fThreadInit, INIT_DEC_THREAD_JPEG_DONE, FLAG_WAITMODE_AND,
                (COUNT_100_MSEC) );


            // print-out the warning message if the thread initial fail.
            if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_JPEG_DONE) )
            {
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "JPEG thread not initial done, value: %lx\n", OS_PeekFlag(&__fThreadInit));
                return (FALSE);
            }

            // Notice: frame buffer address setting for JPEG mode is put within HALJPEG_Initial()

            //kevin1.00, JPEG & MPEG use diff gains
            __bTVE_JPEG = TRUE;
            HAL_SetTVE();

        }
    }
    //else if(__bAttrPlay == ATTR_AVI)
    else if(bDesiredDecoder == DIVX_DECODER_THREAD)
    {
        // LLY2.76, motion mode all set digital panel mode as FALSE
        // So, remove it to _UTL__ProgMotion()
        //HAL_AdjustTFT_DigitalMode(FALSE);

        // Create the DivX thread if it's un-exist
        if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_DIVX_DONE) )
        {
            INITIAL_ThreadInit(THREAD_DIVX_DECODER);

            // LLY0.95, the waiting time is absolute time
            // wait the thread initial done
            //cyg_flag_timed_wait( &__fThreadInit, INIT_DEC_THREAD_DIVX_DONE, CYG_FLAG_WAITMODE_AND, COUNT_50_MSEC);
            OS_TimedWaitFlag( &__fThreadInit, INIT_DEC_THREAD_DIVX_DONE, FLAG_WAITMODE_AND,
                (COUNT_100_MSEC) );

            // print-out the warning message if the thread initial fail.
            if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_DIVX_DONE) )
            {
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DivX thread not initial done, value: %lx\n", OS_PeekFlag(&__fThreadInit));
                return (FALSE);
            }

            // LLY2.21, remove turn-off main video procedure into UTL_Config_FrameBuffer()
            // LLY0.76, need to turn-off the video display first before change frame buffer address
            // Otherwise, will see the abnormal screen.
            //DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);

            // LLY2.21a, remove frame buffer config procedure since it was integrated w/ UTL_Config_BufferMode()
            // Restore Frame buffer setting as MP4 mode since previous one maybe different
            // LLY2.17, collect frame buffer setting procedure into UTL_Config_FrameBuffer()
            //UTL_Config_FrameBuffer(UTL_FRAMEBUF_MP4_MOTION);
        }
    }
    else if(bDesiredDecoder == MPEG_DECODER_THREAD)
    {
        // LLY2.76, motion mode all set digital panel mode as FALSE
        // So, remove it to _UTL__ProgMotion()
        //HAL_AdjustTFT_DigitalMode(FALSE);

        // Create MPEG decoder thread if it's un-exist
        if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_MPEG_DONE) )
        {
            INITIAL_ThreadInit(THREAD_MPEG_DECODER);

            // LLY0.95, the waiting time is absolute time
            // wait the thread initial done
            //cyg_flag_timed_wait( &__fThreadInit, INIT_DEC_THREAD_MPEG_DONE, CYG_FLAG_WAITMODE_AND, COUNT_50_MSEC);
            OS_TimedWaitFlag( &__fThreadInit, INIT_DEC_THREAD_MPEG_DONE, FLAG_WAITMODE_AND,
                (COUNT_100_MSEC) );


            // print-out the warning message if the thread initial fail.
            if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_MPEG_DONE) )
            {
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MPEG thread not initial done, value: %lx\n", OS_PeekFlag(&__fThreadInit));
                return (FALSE);
            }

            // LLY2.21, remove turn-off main video procedure into UTL_Config_FrameBuffer()
            // LLY0.76, need to turn-off the video display first before change frame buffer address
            // Otherwise, will see the abnormal screen.
            //DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);

            // LLY2.21a, remove frame buffer config procedure since it was integrated w/ UTL_Config_BufferMode()
            // Restore Frame buffer setting as MPEG motion mode since previous one maybe different
            // LLY2.17, collect frame buffer setting procedure into UTL_Config_FrameBuffer()
            //UTL_Config_FrameBuffer(UTL_FRAMEBUF_MOTION);
        }
        // Chuan2.17a, Notify MPEG2 Decoder to do more error concealment for Release Mode
        // LLY2.61, Porting YC's suggestion to disable this command temporally
        // Since, this command can't work @ CT909G IC
#ifndef CT909G_IC_SYSTEM
#ifdef  RELEASE_SETTING
        HAL_PlayCommand(COMMAND_V_RELEASE_MODE, TRUE);
#else
        HAL_PlayCommand(COMMAND_V_RELEASE_MODE, FALSE);
#endif
#endif  // #ifndef CT909G_IC_SYSTEM
    }

    // Chuan1.08, Check bType to turn on/off VDEC.
#ifdef SUPPORT_POWER_SAVING
    if ((bType == MODE_PLAYAUDIO) && (__bCDDA_DTS != CDDA_CDG))
    {
        // Micky2.11a
        HAL_PowerControl(HAL_POWER_VDEC, HAL_POWER_SAVE);
        /*
        DWORD dwSaveInt;
        OS_DISABLE_INTERRUPTS( dwSaveInt );
        REG_PLAT_CLK_GENERATOR_CONTROL |= (PLAT_MCLK_VDEC_DISABLE);
        OS_RESTORE_INTERRUPTS( dwSaveInt );
        */
    }
    else
    {
        HAL_PowerControl(HAL_POWER_VDEC, HAL_POWER_NORMAL);
    }
    /*
    else if (REG_PLAT_CLK_GENERATOR_CONTROL & PLAT_MCLK_VDEC_DISABLE)
    {
        DWORD dwSaveInt;
        OS_DISABLE_INTERRUPTS( dwSaveInt );
        REG_PLAT_CLK_GENERATOR_CONTROL &= ~(PLAT_MCLK_VDEC_DISABLE);
        OS_RESTORE_INTERRUPTS( dwSaveInt );
        // Notice: delay a while to wait VDEC enable ok
        OS_DelayTime(COUNT_50_MSEC);
    }
    */
#endif

    // Issue reset action, and initial necessary setting.
    if ( ! btPseudoReset )
    {
        // Issue A/V reset before play new item (since bitstream is dis-continue)
        // Notice: it's un-necessary to reset SPST here since it will be done within _ProgMotion()
        // (Because SPU_Reset() must reference SPST/OGT type and it will be set within _ProgMotion())
        // LLY2.53, it's un-necessary to do no effect part reset
        // ex. playback video only image, only need to reset video decoder
        //     playabck audio only file, only need to reset audio decoder
        //     playback motion, reset A/V deocder together.
        if(__bAttrPlay & TYPE_CDROM_AUDIO)
        {
            HAL_Reset(HAL_RESET_AUDIO);
        }
        else if(__bAttrPlay & TYPE_CDROM_VIDEO)
        {
            HAL_Reset(HAL_RESET_VIDEO);
        }
        else
        {
            HAL_Reset(HAL_RESET_VIDEO);
            HAL_Reset(HAL_RESET_AUDIO);
        }

#ifndef NO_DISC_MODE
        // LLY0.90, following procedure is necessary only for DVD title
        // Since, nobody maintain __bSTCRunning value
        // So, we maybe see a large OSD time while playback CDROM motion file w/ KEY_NEXT
        if(__wDiscType & BOOK_DVD)
        {
            // DVD_099dMicky, fix DVD, next will show a large tme first
            __bSTCRunning=FALSE;
            // Between menu->new cell, no UTL_OutputShowTime will be called
            // so must call it, so the time will reset to 0
            UTL_OutputShowTime();
        }
#endif  // #ifndef NO_DISC_MODE


        // Clear __bLOGO flag as NONE
        __bLOGO=LOGO_NONE;


        // Start to initial the necessary setting of MPEG chip
        // LLY0.72, remove it sinc nothing need to do initial for CT909 after HAL_RESET_VIDEO()
        //HAL_InitVideo ();

        // only cancel Zoom feature after RISC reset and initialize
        if ( __bZoomFactor != VIEW_NORMAL )
        {
            __bZoomFactor= VIEW_NORMAL;

            // LLY0.90, clear the Zoom mode PIP+OSD message together.
            // Notice: must give the time-out value as 3 sec
            //         otherwise, OSD+PIP clear action can't work. (CoCo suggestion)
#ifdef  SUPPORT_GRAPH_ZOOM
            OSD_OUTPUT_MACRO(MSG_ZOOM, __bZoomFactor, 3);
#endif  // #ifdef SUPPORT_GRAPH_ZOOM

            // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
            HAL_Zoom (VIEW_NORMAL);
#ifdef  SUPPORT_PRINTF
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n ****Cancel Zoom function !!\n");
#endif  // #ifdef SUPPORT_PRINTF
        }
    }


    // LLY1.05, tell parser to enable end padding data for following playing mode
    // To avoid the decoding un-complete
    // Notice: must turn-off it for non-JPEG playback
    // [1] JPEG
    // [2] Motion except DVD title, ex VCD/ SVCD/ CVD/ CDROM motion
    // [3] VCD Still picture -- will be done by parser automatically
    // winnie 2.54, [3] VCD still picture--needs to tell parser enable end padding by mpeg F/W
    // Some vcd20 have not sequent code result in parser can't send to video buffer completely.
    // So, need F/W to tell parser enable end padding data for following playing mode
    // [4] Motion JPEG -- Grievous2.60 (can integrated w/ JPEG file, so using decoder thread for checking)
    //if( (__bAttrPlay == ATTR_JPG)  || ( (__bModePlay==MODE_PLAYMOTION) && (__wDiscType!=BOOK_DVD) )
    if( (bDesiredDecoder == JPEG_DECODER_THREAD)  || ( (__bModePlay==MODE_PLAYMOTION) && (__wDiscType!=BOOK_DVD) )
        ||(__bModePlay&MODE_STILL) )
    {
        PARSER_ActionFilter(0, PARSER_ENABLE_END_PADDING, TRUE);

#ifdef  DEBUG_END_PADDING
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Enable parser end padding\n");
#endif  // #ifdef  DEBUG_ENABLE_PADDING
    }
    else
    {
        PARSER_ActionFilter(0,PARSER_ENABLE_END_PADDING,FALSE);

#ifdef  DEBUG_END_PADDING
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Disable parser end padding\n");
#endif  // #ifdef  DEBUG_ENABLE_PADDING
    }

    // Issue desired setting base on different mode
    switch ( bType )
    {
    // Need check how to do for CT909
    case    MODE_PLAYCDROM:
        HAL_ControlParser(HAL_PARSER_CDROM, 0);
        HAL_ControlParser(HAL_PARSER_ENABLE, 0 );
        break;

    // Include MP3/ MP2/ WMA/ CDDA/ CDG
    case    MODE_PLAYAUDIO:
        _UTL_ProgAudioOnly();
        break;

    // Set desired register for JPEG playing
    case    MODE_PLAYVIDEO:
        _UTL_ProgVideoOnly();
        break;

    case    MODE_PLAYMOTION:
        _UTL_ProgMotion();
        break;

            // high resolution
    case    MODE_PLAYSTILLHNTSC:
    case    MODE_PLAYSTILLHPAL:
    case    MODE_PLAYSTILLH:
            // normal resolution
    case    MODE_PLAYSTILLNTSC:
    case    MODE_PLAYSTILLPAL:
    case    MODE_PLAYSTILL:
        // LLY2.76, always set digital panel mode as FALSE for motion
        HAL_AdjustTFT_DigitalMode(FALSE);

        // LLY2.20, integrate dump-in/out buffer address setting together
        // And, set it as normal mode.
        UTL_Config_BufferMode(DUMP_BUFF_NORMAL_MODE);
#if 0
        // LLY0.72, Set dump-out buffer address as standard for still picture
        // LLY2.01, call HAL_SetBuffer() directly
        HAL_SetBuffer(HAL_SETBUFFER_VIDEO, HAL_VIDEOBUF_NORMAL);
#endif  // #if 0
        __bAudioType= HAL_AUDIO_MPG;
        HAL_SetAudioType( __bAudioType );

        // Set parser data format as linear + CDROM, bitstream format as muxed
        PARSER_SetDataFmt( 0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDROM), PARSER_BSFMT_MUXED );


        if (bType & MODE_PLAYSTILLH)    //high resolution
        {
            // set parser as decoding video ID 2
            __bVideoID=0x2;
        }
        else
        {
            // set parser as decoding video ID 1
            __bVideoID=0x1;
        }
        HAL_ControlParser(HAL_PARSER_VIDEO_ID, __bVideoID);

        HAL_ControlParser(HAL_PARSER_AUDIO_ID, __bASTID );
        HAL_ControlParser(HAL_PARSER_ENABLE, 0 );

        // one title playitem is encoded as motion
        // but the segment play item content is encoded as High PAL still
        // Need parser or decoder to tell F/W the case
        // Notice: the control will be do once after each reset
        //HAL_Monitor(HAL_MONITOR_VIDEO_STATUS_PREPARE);
        __btCheckVideoType=TRUE;
        break;

    default:
        break;
    }


    // LLY0.72, remove it since it always audio master mode for CT909
    // And, the sync. control will be done within CC_KeyCommand() (stop --> play)
#if 0
    // Micky1.03, fix A-Bex (motocycle) KEY_NEXT, video play-> flicker-> play.
    // must use video master.
    // 1.02 has enabled COMMAND_STC(RISC will reset STC by SCR),
    // so it can cover the problem(can't detect 1st video pts)
    // HAL_RESET_AUDIO will set back to audio master, so must set video master again.
    // only enable it for DVD now.
    if ( ! btPseudoReset  && (__wDiscType & BOOK_DVD))
    {
       HAL_ControlAVSync (HAL_AVSYNC_VIDEOMASTER, 1); // ** TCH1.00-908;      Enable Video Master Mode.
    }
#endif  // #if 0

    // f/w 0.33
    // one title (It don't rain) playitem is encoded as motion
    // but the segment play item content is encoded as High PAL still
    // now the ucode(02.26) can auto detect this, and set DRAM to let
    // F/W know this case.(This control will do once after each reset)
    // only do this checking for still picture
    if(__bModePlay & MODE_STILL)
    {
        __btCheckVideoType=TRUE;
//        HAL_Monitor(HAL_MONITOR_VIDEO_STATUS_PREPARE);
    }
    else
    {
        __btCheckVideoType=FALSE;
    }

    return TRUE;
}


//  *****************************************************************************************
//  Function    :   _UTL_ProgAudioOnly
//  Description :   Do the necessary programming of parser/ decoder while audio only mode
//                  ex. MP3/ MP2/ WMA/ AC3/ CDDA
//  Arguments   :   None
//  Return      :   None
//  Notice      :   Only be called by UTL_SetPlayMode()
//  *****************************************************************************************
void _UTL_ProgAudioOnly(void)
{
    // LLY2.21a, change CDDA playback as normal mode.
    // And, CDROM/ DVDROM audio only file as MM audio only mode.
    if(__bAttrPlay == ATTR_CDDA)
    {
        UTL_Config_BufferMode(DUMP_BUFF_NORMAL_MODE);
    }
    else
    {
        // LLY2.20, integrate dump-in/out buffer address setting together
        // And, set it as MM audio only mode.
        UTL_Config_BufferMode(DUMP_BUFF_MM_AUDIO_ONLY_MODE);
    }

#if 0
    // LLY0.67, Set dump-out buffer address for video part for CDDA and CDROM/DVDROM audio only file
    // LLY2.01, call HAL_SetBuffer() directly
    HAL_SetBuffer(HAL_SETBUFFER_VIDEO, HAL_VIDEOBUF_MM);
#endif

    // Step 1: Set audio type base on the desired playing mode attribute
    if(__bAttrPlay == ATTR_MP3)
    {
        __bAudioType= HAL_AUDIO_MP3;
    }
    else if(__bAttrPlay == ATTR_MP2)
    {
        __bAudioType= HAL_AUDIO_MP2;
    }
    else if(__bAttrPlay == ATTR_WMA)
    {
        __bAudioType= HAL_AUDIO_WMA;
    }
    else if(__bAttrPlay == ATTR_CDDA)
    {
        if (__bCDDA_DTS == CDDA_DTS)
        {
            __bAudioType= HAL_AUDIO_DTS;
        }
        else
        {
            __bAudioType= HAL_AUDIO_PCM;
        }
    }
    else if (__bAttrPlay == ATTR_AC3)
    {
        __bAudioType= HAL_AUDIO_AC3;
    }
    else if (__bAttrPlay == ATTR_DTS)
    {
        __bAudioType= HAL_AUDIO_DTS;
    }

    HAL_SetAudioType( __bAudioType );


    // Set 2: set data format, and bitstream format to parser
    if(__bAttrPlay == ATTR_CDDA) // CDDA
    {
        // Control parser data format as linear + CDDA, bitstream format as audio only
        // LLY0.80, enable parser byte-swap feature for CD-DTS only, others are not
        // Since, DSP code for PCM will do byte-swap automatically.
        if(__bCDDA_DTS == CDDA_DTS)
        {
            PARSER_SetDataFmt( 0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDDA), (PARSER_BSFMT_AUDIO_ONLY|PARSER_BSFMT_BYTE_SWAP) );
        }
        else
        {
            PARSER_SetDataFmt( 0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDDA), PARSER_BSFMT_AUDIO_ONLY);
        }

#ifndef NO_CDG
#ifndef SIMP_UTL
        if(__bCDDA_DTS == CDDA_CDG)
        {
            // LJY0.95, do CDG initial for CT909
            CDG_Initial();
        }
#endif  // #ifndef SIMP_UTL
#endif  // #ifndef NO_CDG

    }
    // LLY2.35, must enable WMA audo bits format for WMA audio type
    else if(__bAttrPlay == ATTR_WMA)
    {
        PARSER_SetDataFmt(0, PARSER_DATAFMT_NOCHANGE, (PARSER_BSFMT_AUDIO_ONLY|PARSER_BSFMT_WMA));
    }
    else // MP3, MP2, DTS, AC3 (put in CDROM or DVDROM title)
    {
        // LLY0.76-1, tell parser to do byte-swap if do Dolby_certification
#ifdef DOLBY_CERTIFICATION
        if(__bAttrPlay == ATTR_AC3)
        {
            PARSER_SetDataFmt(0, PARSER_DATAFMT_NOCHANGE, (PARSER_BSFMT_AUDIO_ONLY|PARSER_BSFMT_BYTE_SWAP));
        }
        else
#endif  // #ifdef DOLBY_CERTIFICATION
        {
            PARSER_SetDataFmt(0, PARSER_DATAFMT_NOCHANGE, PARSER_BSFMT_AUDIO_ONLY);
        }
    }

    // Step 3: enable audio only parseing
    HAL_ControlParser(HAL_PARSER_AUDIO_ONLY, NULL_0);

    // Step 4: Enable parser
    // Can remove it since it's un-necessary for CT909
    //HAL_ControlParser(HAL_PARSER_ENABLE, 0 );

}


//  *****************************************************************************************
//  Function    :   _UTL_ProgVideoOnly
//  Description :   Do the necessary programming of parser/ decoder while video only mode
//                  ex. JPEG/ Logo
//  Arguments   :   None
//  Return      :   None
//  Notice      :   Only be called by UTL_SetPlayMode()
//  *****************************************************************************************
void _UTL_ProgVideoOnly(void)
{
    // LLY2.05, collect the procedure to control video only
    if (__bDVDROM)
    {
        PARSER_SetDataFmt( 0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_DVD_2048), PARSER_BSFMT_VIDEO_ONLY );
    }
    else
    {
        PARSER_SetDataFmt( 0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDROM), PARSER_BSFMT_VIDEO_ONLY );
    }

    HAL_ControlParser(HAL_PARSER_VIDEO_ONLY, 0 );


    if(__bAttrPlay == ATTR_JPG)
    {
        // LLY2.76, always set digital panel mode as TRUE for still image
        HAL_AdjustTFT_DigitalMode(TRUE);

        // LLY2.20, integrate dump-in/out buffer address setting together
        // And, set it as MM video only only mode.
        UTL_Config_BufferMode(DUMP_BUFF_MM_VIDEO_ONLY_MODE);
#if 0
        // LLY0.67, Set dump-out buffer address for video part for CDROM/DVDROM JPEG file
        // LLY2.01, call HAL_SetBuffer() directly
        HAL_SetBuffer(HAL_SETBUFFER_VIDEO, HAL_VIDEOBUF_MM);
#endif  // #if 0

        // Do JPEG related setting.
        HALJPEG_Initial();

        // LLY.275, initialize JPEG display time as very-very small value
        // Then, it will always time-out for 1st Slide-Show
        __dwMMJPEGDisplayTime=0;
    }
    // LLY0.80, support CDROM title w/ video only raw data bitstream
    else if(__bAttrPlay & TYPE_CDROM_AV)
    {
        // LLY2.20, integrate dump-in/out buffer address setting together
        // And, set it as MM AVI mode
        UTL_Config_BufferMode(DUMP_BUFF_MM_AVI_MODE);
    }


    // Enable parser
    // Can remove it since it's un-necessary for CT909
    //HAL_ControlParser(HAL_PARSER_ENABLE, 0 );

}


//  *****************************************************************************************
//  Function    :   _UTL_ProgMotion
//  Description :   Do the necessary programming of parser/ decoder while motion
//  Arguments   :   None
//  Return      :   None
//  Notice      :   Only be called by UTL_SetPlayMode()
//                  Parser related setting is changed from CT909
//  *****************************************************************************************
void _UTL_ProgMotion(void)
{
    // LLY2.76, always set digital panel mode as FALSE for motion
    HAL_AdjustTFT_DigitalMode(FALSE);

    // LLY0.70, reset dump sector range (same as previous __dwSectorCDROMBegin & __dwSectorCDROMEnd)
    // Since the data buffer will be destroyed while start playback motion.
    // Suppose DRAM don't contain any CDROM data ...
    INFOFILTER_ResetDumpSectors();

    // DVD title
    if ( __wDiscType & BOOK_DVD )
    {
        // LLY2.20, integrate dump-in/out buffer address setting together
        // And, set it as normal mode
        UTL_Config_BufferMode(DUMP_BUFF_NORMAL_MODE);

        // LLY0.80, set SP/ OGT type as DVD mode for DVD title
        __dwSPType = SP_TYPE_DVD;
        HAL_SetAudioType( __bAudioType );

        // MiniDVD=BOOK_DVD | BOOK_CDROM
        if(__wDiscType & BOOK_CDROM) // Mini-DVD case
        {
            PARSER_SetDataFmt(0, (PARSER_DATAFMT_LINEAR | PARSER_DATAFMT_CDROM), (PARSER_BSFMT_DVD_VIDEO | PARSER_BSFMT_MUXED));
        }
        else
        {
            PARSER_SetDataFmt(0, (PARSER_DATAFMT_LINEAR | PARSER_DATAFMT_DVD_2048), (PARSER_BSFMT_DVD_VIDEO | PARSER_BSFMT_MUXED));
        }

        // LLY0.85, remove copy protection setting to servo.c

    }
    // Process the playback setting of motion file on CDROM or DVDROM -- LLY0.67
    else  if (__wDiscType & CDROM_M1)
    {
        // LLY0.80, call API to get corresponding audio stream type and call HAL_SetAudioType() AVI case
        // LLY2.21, integrate *.VOB together
        // else if(__bAttrPlay == ATTR_AVI)
        if(__bAttrPlay == ATTR_AVI || __bAttrPlay == ATTR_VOB)
        {
            UTL_MovieASTSetting(__bASTNO);
        }
        else
        {
            __bAudioType= HAL_AUDIO_MPG;
            HAL_SetAudioType(__bAudioType);
        }

        // LLY0.95, set dump-out buffer address as DivX mode only for DivX/ AVI file
        // And, let CDROM/ DVDROM other motion buffer setting as MM-Motion mode -- LLY2.01
        // Since, *.vob video bitrate is higher
        // LLY2.35, must set video buffer after __bAudioType is ready
        // Since, AVI A/V buffer setting must reference A/V ratio and current audio type
        if(__bAttrPlay == ATTR_AVI)
        {
            // LLY2.20, integrate dump-in/out buffer address setting together
            // And, set it as MM AVI mode
            // Notice: the video format maybe different within *.AVI container, ex. MJPG, MPEG1/2/4
            //         So, need to configuration different buffer mode -- CMH2.55
#ifdef SUPPORT_MOTION_JPEG
    	    if(__dwAVIVideoFMT == PARSER_ESFMT_MJPG)
    	    {
        		UTL_Config_BufferMode(DUMP_BUFF_MM_MJPG_MODE);
    	    }
    	    else
#endif  // #define SUPPORT_MOTION_JPEG
    	    {
                UTL_Config_BufferMode(DUMP_BUFF_MM_AVI_MODE);
            }
        }
        else
        {
            // LLY2.20, integrate dump-in/out buffer address setting together
            // And, set it as MM motion mode
            // Chuan2.22, Always do BFR for DAT/ VOB/ MPG/ ...
            UTL_BFRModeSet(TRUE);
            UTL_Config_BufferMode(DUMP_BUFF_MM_MOTION_MODE);
        }


        // LLY0.96, send MSG_AST_CTL/ MSG_SPST_CTL to notify OSD display mode
        // to update the AST and SPST information while change track
        OSD_Output ( MSG_AST_CTL, __bASTNO, 0x0 );
        OSD_Output ( MSG_SPST_CTL, __bSPSTNO, 0x0 );    // ** TCH0.451;


        // control the parser relative setting for CDROM ro DVDROM
        // LLY0.70, need tell parser DVD_VIDEO for VOB file
        // Otherwise, the audio information will be skipped.
        // Need check how to do for VOB file.
        if(__bAttrPlay == ATTR_VOB)
        {
            // LLY0.80, set SP/ OGT type as DVD mode for *.vob
            __dwSPType = SP_TYPE_DVD;
            if(__bDVDROM)
            {
                PARSER_SetDataFmt(0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_DVD_2048), (PARSER_BSFMT_DVD_VIDEO|PARSER_BSFMT_MUXED));
            }
            else
            {
                PARSER_SetDataFmt(0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDROM), (PARSER_BSFMT_DVD_VIDEO|PARSER_BSFMT_MUXED));
            }
        }
        // LLY0.72, configure data format as AVI for parser
        else if(__bAttrPlay == ATTR_AVI)
        {
            // LLY1.50, set different bitstream format and SP type for AVI and MP4
#ifdef  SUPPORT_MP4
            if(MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem)==EXTNAME_MP4)
            {
                __dwTemp=PARSER_BSFMT_MP4;
                __dwSPType = SP_TYPE_DVD;
            }
            else
#endif  // #ifdef SUPPORT_MP4
            {
                __dwTemp=PARSER_BSFMT_AVI;
                __dwSPType = SP_TYPE_DIVX;
            }

            if(__bDVDROM)
            {
                PARSER_SetDataFmt(0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_DVD_2048), (__dwTemp|PARSER_BSFMT_MUXED));
            }
            else
            {
                PARSER_SetDataFmt(0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDROM), (__dwTemp|PARSER_BSFMT_MUXED));
            }
        }
        else
        {
            // LLY0.80, assume SP/ OGT type as SVCD for *.mpg or *.dat file format.
            __dwSPType = SP_TYPE_SVCD;
            if(__bDVDROM)
            {
                PARSER_SetDataFmt(0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_DVD_2048), PARSER_BSFMT_MUXED);
            }
            else
            {
                PARSER_SetDataFmt(0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDROM), PARSER_BSFMT_MUXED);
            }
        }
    }
    else  // for vcd / svcd etc.
    {
        // LLY2.20, integrate dump-in/out buffer address setting together
        // And, set it as normal mode
        UTL_Config_BufferMode(DUMP_BUFF_NORMAL_MODE);

        __bAudioType= HAL_AUDIO_MPG;
        HAL_SetAudioType(__bAudioType);

        // Control parser that data format as linear + CDROM, bitstream format as muxed for VCD title.
        // LLY0.70, specify bitstream format as SVCD/ CVD for SVCD or CVD title
        // Otherwise, OGT can't be parser.
        if (__wDiscType & BOOK_SVCD)
        {
            // LLY0.80, set SP/ OGT type as SVCD mode for SVCD title
            __dwSPType = SP_TYPE_SVCD;
            PARSER_SetDataFmt( 0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDROM), PARSER_BSFMT_SVCD | PARSER_BSFMT_MUXED );
        }
        else if (__wDiscType & BOOK_CVD)
        {
            // LLY0.80, set SP/ OGT type as CVD mode for CVD title
            __dwSPType = SP_TYPE_CVD;
            PARSER_SetDataFmt( 0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDROM), PARSER_BSFMT_CVD | PARSER_BSFMT_MUXED );
        }
        else
        {
            PARSER_SetDataFmt( 0, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDROM), PARSER_BSFMT_MUXED );
        }
    }


    // LLY0.76-1, remove frame buffer address setting togher with thread creating.


    // LLY0.80, issue SPST reset after SP/OGT type setting
    // Otherwise, SPST R/W pointer, buffer is not clear while bitstream dis-continue
    HAL_Reset(HAL_RESET_SPST);

    // LLY0.85, set corresponding video ID as zero for non-DivX file
    // Since it will be set within UTL_PreScanInfo().
    if(__bAttrPlay!=ATTR_AVI)
    {
        __bVideoID=0;
    }

    // Set parser the desired A/V ID
    // Notice: reference global variable : __bVideoID for video ID -- LLY0.85
    // Since, F/W will set it as HAL_THROW_VIDEO while not support DivX format
    HAL_ControlParser(HAL_PARSER_VIDEO_ID, __bVideoID);
    HAL_ControlParser(HAL_PARSER_AUDIO_ID, __bASTID);

    // LLY.104, reset the OGT ID to same as Audio channel
    // only for SVCD and CVD, and DVD keep same
    // LLY0.80, also specify SPST ID for DivX/ AVI to support SPST
    if( (__wDiscType & BOOK_2X) || (__bAttrPlay == ATTR_AVI) )
    {
        HAL_ControlParser(HAL_PARSER_SP_ID, __bSPSTID);

        // LLY2.15, config SP2 ID, and disable SP2 for OGT and AVI
        // Otherwise, SP1 can't display since SP2 is un-known (SP mechanism)
        PARSER_ActionFilter( 0, PARSER_ENABLE_SP2, FALSE );
        __bSP2_Disp_OFF = TRUE;
        __bSPST2NO = 0xFF;
        DISP_DisplayCtrl(DISP_SP2, FALSE);
    }

    // Can remove since it un-necessary for CT909
    // cna't enable parser before data input
    //HAL_ControlParser(HAL_PARSER_ENABLE, 0 );

}


// Only support following APIs for whole DVD flow.
#ifndef SIMP_UTL
// LLY2.20 rename it as UTL_ConfigBufferMode()
//  ***************************************************************************************
//  Function    :   UTL_Config_BufferMode
//  Description :   Config corresponding buffer mode
//  Arguments   :   bBuffMode, specify the desired buffer mode
//  Return      :   None
//  Notice      :   Only config servo dump-in buffer, parser video buffer, data buffer
//                  And, must set Audio buffer during HAL_SetAudioType()
//                  Since, the motion will exist w/ differnt audio type.
//                  Also, config OSD frame buffer together -- LLY2.21
//                  Config display frame buffer together -- LLY2.21a
//  ***************************************************************************************
void UTL_Config_BufferMode(BYTE bBuffMode)
{
    // LLY2.22, must re-assign value to bBuffMode, since JPEG preview/ thumbnail/ slide show
    //          are different modes, and caller alwasy give parameter as VIDEO_ONLY mode
    if(bBuffMode == DUMP_BUFF_MM_VIDEO_ONLY_MODE)
    {
        if(__bThumbnailMode)
        {
            bBuffMode = DUMP_BUFF_MM_JPEG_THUMBNAIL_MODE;
        }
        else if(__dwMMJPEGPreview)
        {
            bBuffMode = DUMP_BUFF_MM_JPEG_PREVIEW_MODE;
        }
    }

    // LLY2.22, nothing to do while the same mode
    if(_bUTLSrcBuffMod == bBuffMode)
    {
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Same mode: %lx\n", bBuffMode);
#endif  // #ifdef DEBUG_BUFFER_MODE

#ifndef NO_DISC_MODE
        // Servo dump-in buffer
        if(bBuffMode == DUMP_BUFF_NORMAL_MODE)
        {
            DWORD   dwBufEnd;
#ifdef SMALL_SOURCE_BUFFER
            DWORD   dwBufStart=DS_INPUT_BITBUF_ST_SMALL;
            BYTE    bBufSize=((DS_INPUT_BITBUF_END_SMALL-DS_INPUT_BITBUF_ST_SMALL)/SERVO_BUFSIZE_ALIGN_UNIT);
#else
            DWORD   dwBufStart=DS_INPUT_BITBUF_ST;
            BYTE    bBufSize=((DS_INPUT_BITBUF_END-DS_INPUT_BITBUF_ST)/SERVO_BUFSIZE_ALIGN_UNIT);
#endif //
            BYTE    bId=__dwSFStreamID;
            extern  BYTE    SERVO_ConfigServoBuffer( BYTE bNum, BYTE bBufSize, ULONG_UNION ulunBufStart );


            dwBufEnd = dwBufStart + (DWORD)(bBufSize * SERVO_BUFSIZE_ALIGN_UNIT);
            SERVO_ConfigServoBuffer(bId, bBufSize, (ULONG_UNION) dwBufStart);
        }
#endif  // #ifndef NO_DISC_MODE

        // LLY2.31, need to do buffer configure again while AVI mode
        // Since, A/V buffer setting is base on different file's A/V ratio
        // And, Audio buffer re-config will be done @ UTL_MovieASTSetting()
        if(bBuffMode == DUMP_BUFF_MM_AVI_MODE)
        {
            // Parser video dump-out buffer and VLD
            HAL_SetBuffer(HAL_SETBUFFER_VIDEO, HAL_VIDEOBUF_AVI);
        }

        return;
    }


    switch(bBuffMode)
    {
    case    DUMP_BUFF_AP_MODE:  // AP mode
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AP Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE

        // Servo dump-in buffer
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_AP,
            ((DS_INPUT_BITBUF_END_AP-DS_INPUT_BITBUF_ST_AP)/SERVO_BUFSIZE_ALIGN_UNIT) );

        // Parser data dump-out buffer
        PARSER_SetOutBufferAddress((DWORD)PARSER_ES_DATA, (DWORD)DS_USERDATA_BUF_ST_AP, (DWORD)DS_USERDATA_BUF_END_AP);

        // wyc2.22-909s, need to clear the user data cache variable
        // when changing mode becuase each mode buffer address is different.
        INFOFILTER_ResetDumpSectors();

        // Tell infofilter the user data mode.
        INFOFILTER_CFGUserData(DS_USERDATA_BUF_ST_AP);

        // OSD frame buffer as AP mode
        OSD_SetBufferModeInfo(OSDBUFFER_MODE_AP);

        // It's un-necessary to config frame buffer for AP mode.
        break;

    case    DUMP_BUFF_NORMAL_MODE: // DVD/ VCD title
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Normal Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE

        // Servo dump-in buffer
#ifdef SMALL_SOURCE_BUFFER
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_SMALL,
            ((DS_INPUT_BITBUF_END_SMALL-DS_INPUT_BITBUF_ST_SMALL)/SERVO_BUFSIZE_ALIGN_UNIT) );
#else
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST,
            ((DS_INPUT_BITBUF_END-DS_INPUT_BITBUF_ST)/SERVO_BUFSIZE_ALIGN_UNIT) );
#endif //

        // Parser video dump-out buffer and VLD
        HAL_SetBuffer(HAL_SETBUFFER_VIDEO, HAL_VIDEOBUF_NORMAL);

        // Parser data dump-out buffer
        PARSER_SetOutBufferAddress((DWORD)PARSER_ES_DATA, (DWORD)DS_USERDATA_BUF_ST, (DWORD)DS_USERDATA_BUF_END);

        // wyc2.22-909s, need to clear the user data cache variable
        // when changing mode becuase each mode buffer address is different.
        INFOFILTER_ResetDumpSectors();

        // Tell infofilter the user data mode.
        INFOFILTER_CFGUserData(DS_USERDATA_BUF_ST);

        // OSD frame buffer as normal mode
        OSD_SetBufferModeInfo(OSDBUFFER_MODE_NORMAL);

        // Config frame buffer as motion mode
        UTL_Config_FrameBuffer(UTL_FRAMEBUF_MOTION);
        break;

#ifdef SUPPORT_STB
    case    DUMP_BUFF_DVB_MODE: // STB mode
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DVB Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE

        // Servo dump-in buffer
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_LARGINSREP_ST,
            ((DS_INPUT_INSREP_END-DS_INPUT_LARGINSREP_ST)/SERVO_BUFSIZE_ALIGN_UNIT) );

        // Parser data dump-out buffer
        PARSER_SetOutBufferAddress((DWORD)PARSER_ES_DATA, (DWORD)DS_USERDATA_BUF_ST, (DWORD)DS_USERDATA_BUF_END);

        // wyc2.22-909s, need to clear the user data cache variable
        // when changing mode becuase each mode buffer address is different.
        INFOFILTER_ResetDumpSectors();

        // Tell infofilter the user data mode as standard.
        INFOFILTER_CFGUserData(DS_USERDATA_BUF_ST);

        // OSD frame buffer as STB mode
        OSD_SetBufferModeInfo(OSDBUFFER_MODE_DVB);

        // Config frame buffer as motion mode
        UTL_Config_FrameBuffer(UTL_FRAMEBUF_MOTION);
        break;
#endif

    // MM title detection mode or MM UI mode
    case    DUMP_BUFF_MM_TITLE_DETECT_MODE:
    case    DUMP_BUFF_MM_UI_MODE:
        // only config OSD related buffer and frame buffer as MM UI mode while MM UI drawing
        //         otherwise, normal mode UI maybe change mode during title detection time.
        if(bBuffMode == DUMP_BUFF_MM_UI_MODE)
        {
#ifdef  DEBUG_BUFFER_MODE
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MM UI Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE

            OSD_SetBufferModeInfo(OSDBUFFER_MODE_MM_UI);

            UTL_Config_FrameBuffer(UTL_FRAMEBUF_MM_UI);
        }
        else
        {
            // wyc2.22-909s, need to clear the user data cache variable
            // when changing mode becuase each mode buffer address is different.
            INFOFILTER_ResetDumpSectors();

#ifdef  DEBUG_BUFFER_MODE
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MM title detect Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE
        }

        // Servo dump-in buffer
#ifdef SMALL_SOURCE_BUFFER
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_MM_SMALL,
            ((DS_INPUT_BITBUF_END_MM_SMALL-DS_INPUT_BITBUF_ST_MM_SMALL)/SERVO_BUFSIZE_ALIGN_UNIT) );
#else
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_MM,
            ((DS_INPUT_BITBUF_END_MM-DS_INPUT_BITBUF_ST_MM)/SERVO_BUFSIZE_ALIGN_UNIT) );
#endif //

        // Parser data dump-out buffer
        PARSER_SetOutBufferAddress((DWORD)PARSER_ES_DATA, (DWORD)DS_USERDATA_BUF_ST_MM, (DWORD)DS_USERDATA_BUF_END_MM);

        // Tell infofilter the user data mode as media manager mode.
        INFOFILTER_CFGUserData(DS_USERDATA_BUF_ST_MM);
        break;

    // LLY2.21, CDROM/ DVDROM audio only file
    case    DUMP_BUFF_MM_AUDIO_ONLY_MODE:
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MM Audio Only Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE

        // Servo dump-in buffer
#ifdef SMALL_SOURCE_BUFFER
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_MM_SMALL,
            ((DS_INPUT_BITBUF_END_MM_SMALL-DS_INPUT_BITBUF_ST_MM_SMALL)/SERVO_BUFSIZE_ALIGN_UNIT) );
#else
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_MM,
            ((DS_INPUT_BITBUF_END_MM-DS_INPUT_BITBUF_ST_MM)/SERVO_BUFSIZE_ALIGN_UNIT) );
#endif//

        // Parser video dump-out buffer and VLD
        HAL_SetBuffer(HAL_SETBUFFER_VIDEO, HAL_VIDEOBUF_MM);

        // Parser data dump-out buffer
        PARSER_SetOutBufferAddress((DWORD)PARSER_ES_DATA, (DWORD)DS_USERDATA_BUF_ST_MM, (DWORD)DS_USERDATA_BUF_END_MM);

        // Tell infofilter the user data mode as media manager mode.
        INFOFILTER_CFGUserData(DS_USERDATA_BUF_ST_MM);

        // OSD frame buffer as MM audio only mode
        OSD_SetBufferModeInfo(OSDBUFFER_MODE_MM_AUDIO_ONLY);

        // Config frame buffer as MM audio only mode
        UTL_Config_FrameBuffer(UTL_FRAMEBUF_MM_AUDIO_ONLY);
        break;

    // LLY2.21, CDROM/ DVDROM video only file, JPEG preview, thumbnail, slide show mode
    case    DUMP_BUFF_MM_VIDEO_ONLY_MODE:
    case    DUMP_BUFF_MM_JPEG_PREVIEW_MODE:
    case    DUMP_BUFF_MM_JPEG_THUMBNAIL_MODE:
        // LLY2.22, split JPEG slide show, preview, thumbnail mode.
        if(bBuffMode == DUMP_BUFF_MM_JPEG_THUMBNAIL_MODE) // thumbnail mode
        {
            bBuffMode=DUMP_BUFF_MM_JPEG_THUMBNAIL_MODE;
            OSD_SetBufferModeInfo(OSDBUFFER_MODE_MM_JPEG_THUMBNAIL);

            UTL_Config_FrameBuffer(UTL_FRAMEBUF_JPEG_THUMBNAIL);

#ifdef  DEBUG_BUFFER_MODE
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MM JPEG thumbnail Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE
        }
        else if(bBuffMode == DUMP_BUFF_MM_JPEG_PREVIEW_MODE) // JPEG preview
        {
            bBuffMode=DUMP_BUFF_MM_JPEG_PREVIEW_MODE;
            OSD_SetBufferModeInfo(OSDBUFFER_MODE_MM_JPEG_PREVIEW);

            UTL_Config_FrameBuffer(UTL_FRAMEBUF_JPEG_PREVIEW);

#ifdef  DEBUG_BUFFER_MODE
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MM JPEG preview Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE
        }
        else // video only or JPEG slide show
        {
            bBuffMode=DUMP_BUFF_MM_VIDEO_ONLY_MODE;
            OSD_SetBufferModeInfo(OSDBUFFER_MODE_MM_VIDEO_ONLY);

            // LLY2.21a, Config frame buffer as JPEG slide show mode
            UTL_Config_FrameBuffer(UTL_FRAMEBUF_JPEG_SLIDE);

#ifdef  DEBUG_BUFFER_MODE
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MM JPEG Slide Show Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE
        }

        // Servo dump-in buffer
#ifdef SMALL_SOURCE_BUFFER
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_MM_SMALL,
            ((DS_INPUT_BITBUF_END_MM_SMALL-DS_INPUT_BITBUF_ST_MM_SMALL)/SERVO_BUFSIZE_ALIGN_UNIT) );
#else
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_MM,
            ((DS_INPUT_BITBUF_END_MM-DS_INPUT_BITBUF_ST_MM)/SERVO_BUFSIZE_ALIGN_UNIT) );
#endif //

        // Parser video dump-out buffer and VLD
        HAL_SetBuffer(HAL_SETBUFFER_VIDEO, HAL_VIDEOBUF_MM);

        // Parser data dump-out buffer
        PARSER_SetOutBufferAddress((DWORD)PARSER_ES_DATA, (DWORD)DS_USERDATA_BUF_ST_MM, (DWORD)DS_USERDATA_BUF_END_MM);

        // Tell infofilter the user data mode as media manager mode.
        INFOFILTER_CFGUserData(DS_USERDATA_BUF_ST_MM);
        break;

    case    DUMP_BUFF_MM_MJPG_MODE:	// CMH2.55 add motion jpeg to be different with avi mode
    case    DUMP_BUFF_MM_AVI_MODE:  // AVI/ DivX/ Nero Digital file
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MM AVI/ MJPEG Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE

        // LLY2.36, remove servo dump-in, SP buffer, User Data buffer address setting to _UTL_PreConfig_AVIBuffer()
        // Since they are shared the pre-reserved space, and must be ready before prescan index table and char_base SP.
        // Servo dump-in buffer
        //SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_AVI,
        //    ((DS_INPUT_BITBUF_END_AVI-DS_INPUT_BITBUF_ST_AVI)/SERVO_BUFSIZE_ALIGN_UNIT) );

        // Parser video dump-out buffer and VLD
        HAL_SetBuffer(HAL_SETBUFFER_VIDEO, HAL_VIDEOBUF_AVI);

        // Parser data dump-out buffer
        //PARSER_SetOutBufferAddress((DWORD)PARSER_ES_DATA, (DWORD)DS_USERDATA_BUF_ST_MM, (DWORD)DS_USERDATA_BUF_END_MM);

        // Tell infofilter the user data mode as media manager mode.
        //INFOFILTER_CFGUserData(DS_USERDATA_BUF_ST_MM);

        // OSD frame buffer as MM AVI mode
        OSD_SetBufferModeInfo(OSDBUFFER_MODE_MM_AVI);

    	//CMH2.55, add for config motion jpeg buffer mode
#ifdef SUPPORT_MOTION_JPEG
	    if(bBuffMode==DUMP_BUFF_MM_MJPG_MODE)
    	{
            // Config frame buffer as MJPG mode
            UTL_Config_FrameBuffer(UTL_FRAMEBUF_MJPEG);
    	}
    	else
#endif  // #define SUPPORT_MOTION_JPEG
    	{
            // Config frame buffer as MM motion mode
            UTL_Config_FrameBuffer(UTL_FRAMEBUF_MP4_MOTION);
    	}

        // LLY2.31, config SP buffer as MM AVI mode
        //PARSER_SetOutBufferAddress(PARSER_ES_SP1, DS_SP1BUF_ADDR_ST_MM, DS_SP1BUF_ADDR_END_MM);

        // LLY2.53, need to call _UTL_PreConfig_AVIBuffer() here again (even it has done before UTL_Init_PlayItem())
        // Since, servo dump-in buffer maybe chnage to another address during MM_UI mode
        // So, if Play(AVI mode) --> Stop(MM UI mode) --> Resume(AVI mode): nobody re-config it.
        _UTL_PreConfig_AVIBuffer();
        break;

    case    DUMP_BUFF_MM_MOTION_MODE: // CDROM/ DVDROM motion file, ex. *.VOB/ *.DAT/ *.MPG
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MM Motion Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE

        // Servo dump-in buffer
#ifdef SMALL_SOURCE_BUFFER
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_MOTION_SMALL,
            ((DS_INPUT_BITBUF_END_MOTION_SMALL-DS_INPUT_BITBUF_ST_MOTION_SMALL)/SERVO_BUFSIZE_ALIGN_UNIT) );
#else
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_MOTION,
            ((DS_INPUT_BITBUF_END_MOTION-DS_INPUT_BITBUF_ST_MOTION)/SERVO_BUFSIZE_ALIGN_UNIT) );
#endif //

        // Parser video dump-out buffer and VLD
        HAL_SetBuffer(HAL_SETBUFFER_VIDEO, HAL_VIDEOBUF_MM_MOTION);

        // Parser data dump-out buffer
        PARSER_SetOutBufferAddress((DWORD)PARSER_ES_DATA, (DWORD)DS_USERDATA_BUF_ST_MM, (DWORD)DS_USERDATA_BUF_END_MM);

        // Tell infofilter the user data mode as media manager mode.
        INFOFILTER_CFGUserData(DS_USERDATA_BUF_ST_MM);

        // OSD frame buffer as MM motion mode
        OSD_SetBufferModeInfo(OSDBUFFER_MODE_MM_MOTION);

        // Config frame buffer as motion mode
        UTL_Config_FrameBuffer(UTL_FRAMEBUF_MM_MOTION);
        break;

    // LLY2.22, add new case for CDG title detection usage.
    case    DUMP_BUFF_CDG_DETECT_MODE:
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "CDG detect Buffer mode\n");
#endif  // #ifdef DEBUG_BUFFER_MODE

        // Servo dump-in buffer
#ifdef SMALL_SOURCE_BUFFER
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST_SMALL,
            ((DS_INPUT_BITBUF_END_SMALL-DS_INPUT_BITBUF_ST_SMALL)/SERVO_BUFSIZE_ALIGN_UNIT) );
#else
        SrcFilter_ConfigBuffer( __dwSFStreamID, DS_INPUT_BITBUF_ST,
            ((DS_INPUT_BITBUF_END-DS_INPUT_BITBUF_ST)/SERVO_BUFSIZE_ALIGN_UNIT) );
#endif //

        // LLY2.11, must set A/V dump-out buffer for parser, since it will dump sub code to video buffer
        // Otherwise, the CDG detection maybe fail
        PARSER_SetOutBufferAddress(PARSER_ES_VIDEO, DS_VDBUF_ST_MM, DS_VDBUF_END_MM);
        PARSER_SetOutBufferAddress(PARSER_ES_AUDIO, DS_AD0BUF_ST_MM, DS_AD0BUF_END_MM);

        // Parser data dump-out buffer
        PARSER_SetOutBufferAddress(PARSER_ES_DATA, DS_CD_DETECT_BUF_ST, DS_CD_DETECT_BUF_END);

        // It's un-ncessary to config infofilter the user data mode
        // And, frame buffer mode, OSD frame buffer address
        break;

    default:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Not defined buffer mode\n");
        break;
    }

    // Keep desired buffer mode.
    _bUTLSrcBuffMod = bBuffMode;

}


// LLY2.17 create
//  **************************************************************************************
//  Function    :   UTL_Config_FrameBuffer
//  Description :   Configure the deisred frame buffer mode
//  Arguments   :   bMode, specify the desired frame buffer mode
//  Return      :   None
//  **************************************************************************************
// LLY2.21, modify the procedure to keep current frame buffer mode,
// And, do main video off before change different mode
void    UTL_Config_FrameBuffer(BYTE bMode)
{
    // Nothing need to do while same group mode -- LLY2.21a
    if( (__bFrameBufMode & 0xF0) == (bMode & 0xF0) )
    {
        return;
    }
    else
    {
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Turn-off video before change buffer mode\n");
#endif  // #ifdef  DEBUG_BUFFER_MODE

        // Turn off main video first before change frame buffer mode
        // LLY2.22, remove it since it will be done @ DISP_FrameBufferSet() by itself
        DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);

        // LLY2.21a, Delay 1 VSYNC for turn off display main video.
        // LLY2.22, remove the procedure after DISP_FRAMEBufferSet()
        // Since, turn off procedure is done @ it.
        // OS_DelayTime(COUNT_50_MSEC);
    }

    switch(bMode)
    {
    case    UTL_FRAMEBUF_MOTION:        // DVD/ VCD/ CVD/ SVCD motion
    case    UTL_FRAMEBUF_MM_MOTION:     // Data Disc motion
    case    UTL_FRAMEBUF_DIGEST:        // Digest mode
    case    UTL_FRAMEBUF_MP4_MOTION:    // MP4 motion
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Normal Frame buffer mode\n");
#endif  // #ifdef  DEBUG_BUFFER_MODE

        __wFrameWidth=720;

        // Chuan2.22, Give the different frame buffer setting depend on _bBFRMode
#ifdef SUPPORT_FBR_MODE // Frame buffer redundancy
#ifdef CONFIG_BFR_BY_FW
        if (_bBFRMode != TRUE)
        {
            DISP_FrameBufferSet(DS_FRAMEBUF0_ADDR_ST_NTSC_FBR, DS_FRAMEBUF1_ADDR_ST_NTSC_FBR, DS_FRAMEBUF2_ADDR_ST_NTSC_FBR, DS_FRAMEBUF0_ADDR_ST_NTSC_FBR, Y_MAXSIZE_NTSC_FBR * 8);
        }
        else
#endif  // #ifdef CONFIG_BFR_BY_FW
        {
            DISP_FrameBufferSet(DS_FRAMEBUF0_ADDR_ST_FBR, DS_FRAMEBUF1_ADDR_ST_FBR, DS_FRAMEBUF2_ADDR_ST_FBR, DS_FRAMEBUF0_ADDR_ST_FBR, Y_MAXSIZE_PAL_FBR * 8);
        }
        __wFrameWidth=544;
#ifdef SUPPORT_BFR_MODE // B-Frame redundancy
#ifdef CONFIG_BFR_BY_FW
        DISP_FrameBufferMode(TRUE, _bBFRMode);
#else   // #ifdef CONFIG_BFR_BY_FW
        DISP_FrameBufferMode(TRUE, TRUE);
#endif  // #ifdef CONFIG_BFR_BY_FW

#else  // #ifdef SUPPORT_BFR_MODE
        DISP_FrameBufferMode(TRUE, FALSE);
#endif // #ifdef SUPPORT_BFR_MODE
#else  // #ifdef SUPPORT_FBR_MODE
        DISP_FrameBufferSet(DS_FRAMEBUF0_ADDR_ST, DS_FRAMEBUF1_ADDR_ST, DS_FRAMEBUF2_ADDR_ST, DS_FRAMEBUF0_ADDR_ST, Y_MAXSIZE_PAL * 8);
        DISP_FrameBufferMode(FALSE, FALSE);
#endif // #ifdef SUPPORT_FBR_MODE

        break;

	// CMH2.55, add for config motion jpeg frame buffer mode
#ifdef SUPPORT_MOTION_JPEG
	case	UTL_FRAMEBUF_MJPEG:
#ifdef SUPPORT_FBR_MODE
	    DISP_FrameBufferSet(DS_FRAMEBUF0_ADDR_ST_FBR, DS_FRAMEBUF0_ADDR_ST_FBR, (DS_FRAMEBUF0_ADDR_ST_FBR + (720*480*3/2)), DS_FRAMEBUF0_ADDR_ST_FBR, Y_MAXSIZE_A8C0);
	    DISP_FrameBufferMode(FALSE, FALSE);
	    JPEGDEC_MjpegFrameBufferConfigure(DS_FRAMEBUF0_ADDR_ST_FBR,DS_FRAMEBUF2_ADDR_END_FBR,1);
#else   // #ifdef SUPPORT_FBR_MODE
	    DISP_FrameBufferSet(DS_FRAMEBUF0_ADDR_ST, DS_FRAMEBUF0_ADDR_ST, (DS_FRAMEBUF0_ADDR_ST + (720*480*3/2)), DS_FRAMEBUF0_ADDR_ST, Y_MAXSIZE_A8C0);
	    DISP_FrameBufferMode(FALSE, FALSE);
	    JPEGDEC_MjpegFrameBufferConfigure(DS_FRAMEBUF0_ADDR_ST,DS_FRAMEBUF2_ADDR_END,1);
#endif  // #ifdef SUPPORT_FBR_MODE
	    break;
#endif  // #define SUPPORT_MOTION_JPEG

    // LLY2.21a, collect MM UI/ MM audio only/ MM JPEG preview/ JPEG slide together.
    //           And, preview and slide show frame buffer mode will be set by HALJPEG module itself.
    // Notice: __wFrameWidth will be assigned within HALJPEG_ConfigFrame() -- LLY2.30
    case    UTL_FRAMEBUF_MM_UI:
    case    UTL_FRAMEBUF_MM_AUDIO_ONLY:
    case    UTL_FRAMEBUF_JPEG_PREVIEW:      // JPEG preview
    case    UTL_FRAMEBUF_JPEG_THUMBNAIL:    // JPEG thumbnail
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MM UI/ MM Audio only/ JPEG preview/ thumbnail frame buffer mode\n");
#endif  // #ifdef  DEBUG_BUFFER_MODE
        HALJPEG_ConfigFrame(HALJPEG_FRAME_BUFFER_PREVIEW);
        break;

    case    UTL_FRAMEBUF_JPEG_SLIDE:        // JPEG Slide Show
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "MM JPEG Slide Frame buffer mode\n");
#endif  // #ifdef  DEBUG_BUFFER_MODE
        HALJPEG_ConfigFrame(HALJPEG_FRAME_BUFFER_SLIDE_SHOW);
        break;

    case    UTL_FRAMEBUF_CDG:           // CD-G mode
#ifdef  DEBUG_BUFFER_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "CDG frame buffer mode\n");
#endif  // #ifdef  DEBUG_BUFFER_MODE
        // LLY2.21a, config CDG frame buffer base on pre-defined setting.
        DISP_FrameBufferSet(DS_FRAMEBUF_ST_CDG, DS_FRAMEBUF_ST_CDG, DS_FRAMEBUF_ST_CDG, DS_FRAMEBUF_ST_CDG, Y_MAXSIZE_CDG/8); //"Y_MAXSIZE_CDG/8" unit is "8 BYTES"
        DISP_FrameBufferMode(FALSE, FALSE);
        // LLY2.22, porting CoCo's code to config CDG display screen buffer address
        // Since, it maybe different base on different DRAM size.
        CDG_ConfigScreenBuf(DS_FRAMEBUF_ST_CDG, DS_FRAMEBUF_ST_CDG + CDG_DISPLAY_FRAME_SIZE);
        __wFrameWidth=720;
        break;

    default:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Not implement frame buffer mode\n");
        return;
    }

    // LLY2.22, delay a VSYNC time to wait VIDEO_READY flag done.
    // Notice: must be put after DISP_FrameBufferSet(), since it was done within it.
    DISP_DelayVSYNC(1);

    // keep frame buffer mode as normal
    __bFrameBufMode = bMode;
}



// LLY2.22 create
//  **************************************************************************************
//  Function    :   UTL_BFRModeSet
//  Description :   BFR Mode on/ off setting
//  Arguments   :   bFlag
//  Return      :   None
//  Notice      :   Must clear __bFrameBuffMode together since UTL_Config_FrameBuffer()
//                  will do nothing while the same mode.
//  **************************************************************************************
void UTL_BFRModeSet(BYTE bFlag)
{
#ifdef  SUPPORT_BFR_MODE // B-Frame redundancy
#ifdef  CONFIG_BFR_BY_FW
    if(bFlag)   // Turn-on BFR mode
    {
        if (_bBFRMode == FALSE)
        {
            _bBFRMode = TRUE;
            __bFrameBufMode = UTL_FRAMEBUF_UNKNOWN;
        }
    }
    else    // Turn-off BFR mode
    {
        if (_bBFRMode == TRUE)
        {
            _bBFRMode = FALSE;
            __bFrameBufMode = UTL_FRAMEBUF_UNKNOWN;
        }
    }
#endif  // #ifdef  CONFIG_BFR_BY_FW
#endif  // #ifdef  SUPPORT_BFR_MODE
}
//************************************************************
//function :UTL_Get_Path_DebugMode
//descri   : according to IC paltform and the following case,  which of the GPIO path can Turn ON/OFF.
//          every bit spefy which of case can be used. 
//
//case 1: //DSU1 NIM_PATH, UART1 NOT_USED        
//case 2: //DSU1 CARD_READER_PATH, UART1 NOT_USED        
//case 3://DSU1 NOT_USED , UART1 NIM_PATH        
//case 4: //DSU1 NIM_PATH, UART1 NIM_PATH        
//case 5: //DSU1 CARD_READER_PATH, UART1 NIM_PATH        
//case 6: //DSU1 NOT_USED, UART1 CARD_READER_PATH        
//case 7: //DSU1 NIM_PATH, UART1 CARD_READER_PATH         
//case 8: //DSU1 CARD_READER_PATH, UART1 CARD_READER_PATH
//case 9: //DSU1 CARD_READER_PATH, UART1 TX CARD_READER , UAERT2 RX EXPAND_GPIO_PATH
//case 10: //DSU1 NIM, UART1 TX CARD_READER , UAERT2 RX EXPAND_GPIO_PATH
//case 11: //DSU1 NOT_USED, UART1 TX CARD_READER , UAERT2 RX EXPAND_GPIO_PATH        
//case 0: //DSU1 NOT_USED, UART1 NOT_USED      
//********************************************************
       
DWORD UTL_Get_Path_DebugMode(void)
{
    DWORD dwDBGPathMode;

    dwDBGPathMode=0;          //initial , all off    
#if (DECODER_SYSTEM==DMP952A_EVAL)    
    dwDBGPathMode|=1L;      //case 0 on
    dwDBGPathMode|=(1L<<1); //case 1 on
    dwDBGPathMode|=(1L<<2); //case 2 on
    dwDBGPathMode|=(1L<<3); //case 3 on  
    dwDBGPathMode|=(1L<<4); //case 7 on    
    dwDBGPathMode|=(1L<<9); //case 9 on
    dwDBGPathMode|=(1L<<10);//case 10 on
    dwDBGPathMode|=(1L<<11);//case 11 on    
#endif//#if (DECODER_SYSTEM==DMP952A_EVAL)

#if ((DECODER_SYSTEM==DMP955A_EVAL) ||(DECODER_SYSTEM==DMP956A_EVAL))    
    dwDBGPathMode|=1L;      //case 0 on
    dwDBGPathMode|=(1L<<1);//case 1 on
    dwDBGPathMode|=(1L<<2);//case 2 on
    dwDBGPathMode|=(1L<<3);//case 3 on   
    dwDBGPathMode|=(1L<<4);//case 4 on
    dwDBGPathMode|=(1L<<5);//case 5 on
    dwDBGPathMode|=(1L<<6);//case 6 on
    dwDBGPathMode|=(1L<<7);//case 7 on
    dwDBGPathMode|=(1L<<8);//case 8 on   
#endif//#if ((DECODER_SYSTEM==DMP955A_EVAL) ||(DECODER_SYSTEM==DMP956A_EVAL))   
#if (DECODER_SYSTEM==DVD956A_DEV) 
    dwDBGPathMode|=1L;      //case 0 on
    dwDBGPathMode|=(1L<<1);//case 1 on  
    dwDBGPathMode|=(1L<<2);//case 2 on  
    dwDBGPathMode|=(1L<<3);//case 3 on   
    dwDBGPathMode|=(1L<<4);//case 4 on   
    dwDBGPathMode|=(1L<<6);//case 6 on         
#endif //#if (DECODER_SYSTEM==DVD956A_DEV) 
#if (DECODER_SYSTEM==DMP952B_EVAL) 
    dwDBGPathMode|=1L;      //case 0 on         
#endif //#if (DECODER_SYSTEM==DVD956A_DEV) 
    return dwDBGPathMode;
}


//SYuan2.77 create.
//**********************************************************
// Function     :  UTL_Get_DebugMode
// Description  :  Get debug mode group value now.
// Arguments    :  None
// Return       :  DWORD dwDbugMode
//*******************************************************
DWORD UTL_Get_DebugMode(void)
{
    return (_dwDBGMode);
}

// LLY2.15 create ...
// SYuan2.77 modify to support new DSU/ UART configure method
//  **************************************************************************************
//  Function    :   UTL_Config_DebugMode
//  Description :   Config the desired debug mode
//  Arguments   :   bAction, specify debug mode configuartion action, ex initial or set
//                  dwPath, setting  DebugMode Group value.
//  Return      :   None
//  Notice      :   Will reference __bDebugMode
//  **************************************************************************************
void UTL_Config_DebugMode(BYTE bAction, DWORD dwPath)
{
    if(bAction == UTL_DBG_INIT)
    {
        // LLY2.15, base on define to enable the debug mode status
        // SYuan2.77-909P: Set HAL-TX, RX initial default.
        // expand (BYE __bDebugMode)  to (DWORD _dwDBGMode)
        //__bDebugMode=UTL_DBG_NONE;
        _dwDBGMode=UTL_DBG_NONE;
#ifdef  ENABLE_DSU1
        //__bDebugMode |= UTL_DBG_DSU1_EN;
        _dwDBGMode|=SET_DSU1_TX_VALUE(DSU1_TX_SOURCE);
        _dwDBGMode|=SET_DSU1_RX_VALUE(DSU1_RX_SOURCE);
#endif  // #ifdef ENABLE_DSU1

#ifdef  SD_FROM_UART1
        //__bDebugMode |= UTL_DBG_UART1_EN;
        _dwDBGMode|=SET_UART1_TX_VALUE(UART1_TX_SOURCE);
        _dwDBGMode|=SET_UART1_RX_VALUE(UART1_RX_SOURCE);
#endif  // #ifdef SD_FROM_UART1

#ifdef  SD_FROM_UART2
        //__bDebugMode |= UTL_DBG_UART2_EN;
        _dwDBGMode|=SET_UART2_TX_VALUE(UART2_TX_SOURCE);
        _dwDBGMode|=SET_UART2_RX_VALUE(UART2_RX_SOURCE);
#endif  // #ifdef SD_FROM_UART2
    }
    else
    {
        _dwDBGMode=dwPath;
    }
    // DSU Mode
    //SYuan2.77-909P: Add setting TX,RX
    if(!(GET_DSU1_TX_VALUE(_dwDBGMode)) || !(GET_DSU1_RX_VALUE(_dwDBGMode)))
    {
        HAL_DSU_Pin_Disable(HAL_DSU1);
    }
    else
    {
        HAL_DSU_Pin_Enable(HAL_DSU1_TX,GET_DSU1_TX_VALUE(_dwDBGMode));
        HAL_DSU_Pin_Enable(HAL_DSU1_RX,GET_DSU1_RX_VALUE(_dwDBGMode));
    }

    //SYuan2.77-909P: Add setting TX,RX for DSU/UART.
    // if Disable UART1-TX & UART-2, Disable UART
    if ( !(GET_UART1_TX_VALUE(_dwDBGMode)) && !(GET_UART2_TX_VALUE(_dwDBGMode)))
    {
        DBG_Disable();
       #ifndef SD_FROM_UART1
	 HAL_UART_Disable(HAL_UART1_RX);
	 HAL_UART_Disable(HAL_UART1_TX);
	 #endif
	 #ifndef SD_FROM_UART2
	 HAL_UART_Disable(HAL_UART2_RX);
	 HAL_UART_Disable(HAL_UART2_TX);
	 #endif
		
    }
    else
    {
        // UART1 Mode
        if(!(GET_UART1_RX_VALUE(_dwDBGMode)))
            HAL_UART_Disable(HAL_UART1_RX);
        else
            DBG_Init((__dwSysClk/2), BAUDRATE_115200, HAL_UART1_RX, _dwDBGMode);

        if(!(GET_UART1_TX_VALUE(_dwDBGMode)))
            HAL_UART_Disable(HAL_UART1_TX);
        else
            DBG_Init((__dwSysClk/2), BAUDRATE_115200, HAL_UART1_TX, _dwDBGMode);

        // UART2 Mode
        if(!(GET_UART2_RX_VALUE(_dwDBGMode)))
            HAL_UART_Disable(HAL_UART2_RX);
        else
            DBG_Init((__dwSysClk/2), BAUDRATE_115200, HAL_UART2_RX, _dwDBGMode);

        if(!(GET_UART2_TX_VALUE(_dwDBGMode)))
            HAL_UART_Disable(HAL_UART2_TX);
        else
            DBG_Init((__dwSysClk/2), BAUDRATE_115200, HAL_UART2_TX, _dwDBGMode);

    }

}
#if 0
void UTL_Config_DebugMode(BYTE bAction)
{
    if(bAction == UTL_DBG_INIT)
    {
        // LLY2.15, base on define to enable the debug mode status
        __bDebugMode=UTL_DBG_NONE;
#ifdef  ENABLE_DSU1
        __bDebugMode |= UTL_DBG_DSU1_EN;
#endif  // #ifdef ENABLE_DSU1

#ifdef  SD_FROM_UART1
        __bDebugMode |= UTL_DBG_UART1_EN;
#endif  // #ifdef SD_FROM_UART1

#ifdef  SD_FROM_UART2
        __bDebugMode |= UTL_DBG_UART2_EN;
#endif  // #ifdef SD_FROM_UART2
    }


    // DSU Mode
    if(__bDebugMode & UTL_DBG_DSU1_EN)
    {
        HAL_DSU_Pin_Enable(HAL_DSU1);
    }
    else
    {
        HAL_DSU_Pin_Disable(HAL_DSU1);
    }

    // Disable UART if not choose any UART
    if( !(__bDebugMode&UTL_DBG_UART1_EN) && !(__bDebugMode&UTL_DBG_UART2_EN) )
    {
        DBG_Disable();
    }

    // UART1 Mode
    if(__bDebugMode & UTL_DBG_UART1_EN)
    {
        DBG_Init((__dwSysClk/2), BAUDRATE_115200, HAL_UART1);
    }
    else
    {
        // LLY2.17, always disable UART1 if user want not enable UART1
        // ie. don't care if define "GPA_2_3_AS_GPIO", or "DSU1 disable"
        HAL_UART_Disable(HAL_UART1);

#if 0
        // Disable UART1 to restore GPA[3:2] as GPA while
        // [1] #define GPA_2_3_AS_GPIO, and
        // [2] DSU1 disable
#ifdef  GPA_2_3_AS_GPIO
        if( !(__bDebugMode & UTL_DBG_DSU1_EN)  )
        {
            HAL_UART_Disable(HAL_UART1);
        }
#endif  // #ifdef GPA_2_3_AS_GPIO
#endif  // #if 0
    }

    // UART2 Mode
    if(__bDebugMode & UTL_DBG_UART2_EN)
    {
        DBG_Init((__dwSysClk/2), BAUDRATE_115200, HAL_UART2);
    }
    else
    {
        // LLY2.17, always disable UART2 if user want not enable UART2
        // ie. don't care if define "GPA_12_13_AS_GPIO"
        HAL_UART_Disable(HAL_UART2);
#if 0
        // Disable UART2 to restore GPA[13:12] as GPA while #define GPA_12_13_AS_GPIO
#ifdef  GPA_12_13_AS_GPIO
        HAL_UART_Disable(HAL_UART2);
#endif  // #ifdef GPA_12_13_AS_GPIO
#endif  // #if 0
    }

}
#endif // #if 0



//  *******************************************************************************
//  Function    :   UTL_GetPlayRange
//  Description :   Get the play item's playback range
//  Arguments   :   nItem, specify the desired play item index, from '1'
//  Return      :   TRUE, the action is successful
//                  FALSE, the action is fail
//  Side Effect :
//  *******************************************************************************
BIT  UTL_GetPlayRange ( WORD nItem )
{
#ifdef  SUPPORT_ENCODE_JPG_PICTURE
    // LLY2.51, support JPEG playback from SPI directly
    if(__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI)
    {
        // call MM_EncoderFile_GetRagne() to get desired item range.
        MM_EncodeFile_GetRange(nItem-1);

        goto GET_RANGE_OK;
    }
#endif  // #ifdef  SUPPORT_ENCODE_JPG_PICTURE


    if (__wDiscType & CDROM_M1)
    {
        // LLY.274p-2, give the desired play item index
        MM_GetRange(__MMPlayingItem.wDirID, nItem, &__dwTimeBegin, &__dwTimeEnd);

        goto GET_RANGE_OK;
    }

#ifndef NO_DISC_MODE
{
    DWORD dwSector;
    WORD  nPIN, i;
    DWORD   dwTemp;

  	extern WORD __wListType;


    // ** DVD-TCH0.19; temp
    if ( __wDiscType & BOOK_DVD )
    {
#ifdef    NO_DVD
        __dwTimeBegin= 446573;
        __dwTimeEnd= 480000;
#endif    // NO_DVD

        goto GET_RANGE_OK;
    }
    // ** DVD-TCH0.19;

    if ( nItem <= 99 )  // physical track
    {
        BYTE    bTemp;
        if ( nItem > __wTrackEnd )
        {
            nItem= __wTrackEnd;
        }

        // get track starting sector,
        CDINFO_GetTrackInfo ( (BYTE)nItem, &__dwTimeBegin, & bTemp );

        // TCC161, ask SERVO to get real length
        SrcFilter_GetTrackEnd ((BYTE)nItem, &__dwTimeEnd) ;

    }
    else if ( nItem <= 599 )    // // Entry.vcd
    {
        // get the total entry points
        CDINFO_ParseInfo ( VCD_ENTRIES, &dwTemp, 0 );
        nPIN= nItem- 99;
        if ( nPIN > dwTemp )
        {
            nPIN= (BYTE)(dwTemp);
        }

        // get the starting address of the nPIN
        CDINFO_ParseInfo ( VCD_ENTRIES_ADDRESS, &dwTemp, nPIN );
        __dwTimeBegin = dwTemp;


        // Play from Entry to the end of the track
        // the __dwTimeEnd will be end of this track
        // It's final entry

        // get track NO. of this entry point
        CDINFO_ParseInfo( VCD_ENTRIES_TRACK, &dwTemp, 0 );
        // TCC161, call SERVO_GetTrackLength to get real length
        SrcFilter_GetTrackEnd( (BYTE)dwTemp, &__dwTimeEnd) ;  // ** TCH1.70;

    }
    else
    {   // Segment Play Item
        nPIN= nItem- 999;

        // get max. segment play item number
        CDINFO_ParseInfo( INFOVCD_MAXSEG_NO, &dwTemp, 0 );
        if( nPIN > (WORD)dwTemp )
        {
            return FALSE;
        }

        // get first segment play item number starting sector
        CDINFO_ParseInfo ( INFOVCD_FIRSTSEG_ADDRESS, & dwSector, 0 );
        // get start sector for nPIN
        dwSector+= (nPIN- 1 )* 150L;    // ** 0.16;VCD 2.0 definition
        __dwTimeBegin = dwSector;
        dwSector+=150L;
        for ( i= 1; i< (1980- nPIN+ 1 ); i++ )
        {
            // a first segment of a play item
            if ( CDINFO_CheckSegPlayItem ( SEGPLAY_ITEM_FIRST, (WORD)(i+ nPIN) ) )
                break;

            // each segment play item is 2 sec.(150 sector length)
            dwSector +=150L;
        }   // for ( i )

        __dwTimeEnd = dwSector-1;

    }

    // LJY0.67, move from _VCD20_GetListInfor() for fixing "JVC-Playing Time Test"
    // cannot play 5, 10, 15sec normally. Update __dwTimeEnd here for parser reference.
    // LLY0.96, only do it while PBC on mode
    if(__btPBC)
    {
        CDINFO_ParseList ( LIST_PLAY_PLAYTIME, & nPIN );
        if (__wListType == ID_PLAYLIST)
        {
            // Chuan2.80, Use _wVCD20Tmp1 instead of wPlayTime
            // if ( wPlayTime )    // time has limit
            if ( nPIN )    // time has limit
            {
                // *TCH** 1997.5.25; C-Cube title problem
                if ( ! ( (__wPlayItem == 1099) && (nPIN == 75 ))) //(wPlayTime == 75 ) ) )
                {
                    // *TCH** 1997.9; C-Cube title, but pressed by ShangHai Record Corp.
                    if ( (__wPlayItem == 1106) && (nPIN == 75) ) //(wPlayTime == 75) )
                    {
                        nPIN= 150; // wPlayTime= 150;
                    }

                    dwTemp= 5L* nPIN; // _dwVCD20Tmp= 5L* wPlayTime;

                    if ( dwTemp < ( __dwTimeEnd- __dwTimeBegin ) )
                    {
                        __dwTimeEnd = __dwTimeBegin + dwTemp;
                    }
                }
            }
        }
    }


    if ( __dwTimeEnd <= __dwTimeBegin )
    {
        // error range
        __dwTimeEnd= __dwTimeBegin+ 300L;       // give 4 seconds range
    }
}
#endif  // #ifndef  NO_DISC_MODE

GET_RANGE_OK:
#ifdef  DEBUG_PLAY_RANGE
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Idx: %lx, Range: %lx, %lx\n", nItem, *pdwStart, *pdwEnd);
#endif  // #ifdef  DEBUG_PLAY_RANGE
    return  TRUE;

}


// LLY2.53 create ...
//  *********************************************************************
//  Function    :   UTL_GetPlayRange_BGItem
//  Description :   To get playback range of background item
//  Arguments   :   nItem, specify the desired play item index, from '1'
//  Return      :   TRUE, the action is successful
//                  FALSE, the action is fail
//  Notice      :   The API is only used for background playing item
//                  And, only for CDROM/DVDROM item
//  *********************************************************************
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
BYTE UTL_GetPlayRange_BGItem(WORD nItem, PDWORD pdwStart, PDWORD pdwEnd)
{
    if (__wDiscType & CDROM_M1)
    {
        // give background play item index
        MM_GetRange(__MMBackgroundPlayingItem.wDirID, nItem, pdwStart, pdwEnd);
    }

#ifdef  DEBUG_PLAY_RANGE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Idx: %lx, Range: %lx, %lx\n", nItem, *pdwStart, *pdwEnd);
#endif  // #ifdef  DEBUG_PLAY_RANGE
    return  TRUE;
}
#endif  // #ifdef SUPPORT_JPEG_AUDIO_PLAYING


//  *********************************************************************
//  Function    :   UTL_CheckPlayItemMode
//  Description :   Check the mode of SegPlayItem;
//  Arguments   :   nItem       : follow VCD2.0 spec
//  Return      :   MODE_PLAYXXXXX
//  Side Effect :
//  *********************************************************************
BYTE UTL_CheckPlayItemMode ( WORD nItem )
{
    BYTE        bMode= MODE_PLAYUNKNOW;
    BYTE        bTemp;

#ifndef NO_DISC_MODE
    WORD        nPIN;
    DWORD   dwTemp;
#endif  // #ifndef NO_DISC_MODE


    // wyc1.21, initial its value to ATTR_NONE
    __bAttrPlayNew = ATTR_NONE;

#ifdef  SUPPORT_ENCODE_JPG_PICTURE
    // LLY2.51, add procedure for SPI JPEG file playback.
    if(__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI)
    {
        __bAttrPlayNew = ATTR_JPG;
        return MODE_PLAYVIDEO;
    }
#endif  // #ifdef  SUPPORT_ENCODE_JPG_PICTURE

    // check CDROM title playing mode
    if(__wDiscType & CDROM_M1) // playable CDROM
    {
        bTemp=MM_GetFileType(__MMPlayingItem.wDirID, nItem);
        // wyc1.24, the reset stage of re-booting when JPG hang.
        if (__bEXTAttr == EXTNAME_JPG)
        {
            if (bTemp != EXTNAME_JPG)
            {
#ifdef  _DEBUG_INFO
                //_DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nReset from UTL stage 2");
#endif ///
            }
        }
        if(bTemp==EXTNAME_MP3)
        {
            __bAttrPlayNew=ATTR_MP3;
            return MODE_PLAYAUDIO;
        }
        else if (bTemp==EXTNAME_MP2)
        {
            __bAttrPlayNew=ATTR_MP2;
            return MODE_PLAYAUDIO;
        }
        else if(bTemp==EXTNAME_WMA)
        {
            __bAttrPlayNew=ATTR_WMA;
            return MODE_PLAYAUDIO;
        }
        else if(bTemp==EXTNAME_JPG)
        {
            __bAttrPlayNew=ATTR_JPG;
            return MODE_PLAYVIDEO;
        }
        else if(bTemp==EXTNAME_AC3)
        {
            __bAttrPlayNew=ATTR_AC3;
            return MODE_PLAYAUDIO;
        }
        else if(bTemp==EXTNAME_DTS) // LLY0.70, support *.DTS file playback
        {
            __bAttrPlayNew=ATTR_DTS;
            return MODE_PLAYAUDIO;
        }
        else if(bTemp==EXTNAME_DAT)
        {
            __bAttrPlayNew=ATTR_DAT;
            return MODE_PLAYMOTION;
        }
        else if(bTemp==EXTNAME_MPG)
        {
            __bAttrPlayNew=ATTR_MPG;
            return MODE_PLAYMOTION;
        }
        else if(bTemp==EXTNAME_VOB)
        {
            __bAttrPlayNew=ATTR_VOB;
            return MODE_PLAYMOTION;
        }
        else if(bTemp==EXTNAME_AVI)// LLY1.02 || _bUTLTemp1==EXTNAME_DIVX)
        {
            __bAttrPlayNew=ATTR_AVI;
            return MODE_PLAYMOTION;
        }
        // LLY1.50, let MP4 go the same flow as AVI
#ifdef  SUPPORT_MP4
        else if(bTemp==EXTNAME_MP4)
        {
            __bAttrPlayNew=ATTR_AVI;
            return MODE_PLAYMOTION;
        }
#endif  // #ifdef SUPPORT_MP4

    }

#ifndef NO_DISC_MODE
    if ( __wDiscType & BOOK_DVD )
    {
        return  MODE_PLAYMOTION;
    }

    // check CD-DA playmode
    if (__wDiscType & BOOK_CDDA)
    {
        __bAttrPlayNew=ATTR_CDDA;
        return  MODE_PLAYAUDIO;
    }

    // LLY0.80a, clear __bCDDA_DTS as none for VCD/ SVCD/ CVD title
    // Since the previous value maybe CDDA_PURE for CDDA track of mixed VCD title
    // And, the new item maybe MPEG motion track of mixed VCD title
    __bCDDA_DTS=CDDA_UNKNOWN;

    if ( nItem <= 99 )  // physical track
    {
        CDINFO_GetTrackInfo ( (BYTE)(nItem), &dwTemp, & bTemp );

        if ( bTemp & DATA_TRACK )
        {
            // MPEG track
            bMode= ( MODE_PLAYMOTION );
        }
        else
        {
            // base one new spec. to give playing mode and attribute
            __bAttrPlayNew=ATTR_CDDA;
            bMode=( MODE_PLAYAUDIO );
            // CD-DA track
            //LJY1.10, set __bCDDA_DTS as pure CDDA for mixed title.(suppose the audio track is CDDA)
            __bCDDA_DTS = CDDA_PURE;
        }
    }
    else if ( nItem <= 599 )
    {   // Entry.vcd
        CDINFO_GetTrackInfo ( (BYTE)1, &dwTemp, & bTemp );

        bMode= ( MODE_PLAYMOTION );
    }
    else
    {   // Segment Play Item
        // TCC275a, fix cdda->segment play item __bSERVOAutoCtrl must change to TRUE
        CDINFO_GetTrackInfo ( (BYTE)1, &dwTemp, & bTemp );
        nPIN= nItem- 999;

        // segment play item content byte
        bTemp= CDINFO_CheckSegPlayItem ( SEGPLAY_ITEM_VALUE, nPIN );
        // bits 2-4=000'b, means no MPEG video data
        if ( ! (bTemp & 0x1C) )
        {   // No MPEG video data
            // setting the same mode as Still or Motion can play MPEG audio
            if ( (__bModePlay == MODE_PLAYMOTION) || (__bModePlay == MODE_STILL) )
            {
                bMode= __bModePlay;
            }
            else
                bMode= MODE_PLAYMOTION;
        }
        else if ( CDINFO_CheckSegPlayItem ( SEGPLAY_ITEM_HIGH, nPIN ) )
        {
            bTemp &= 0x1C;
            if ( bTemp == 0x08 )
                bMode= ( MODE_PLAYSTILLHNTSC );
            else
                bMode= ( MODE_PLAYSTILLHPAL );
        }
        else if ( CDINFO_CheckSegPlayItem ( SEGPLAY_ITEM_NORMAL, nPIN ) )
        {
            if ( (bTemp & 0x1C) == 0x04 )
                bMode= ( MODE_PLAYSTILLNTSC );
            else
                bMode= ( MODE_PLAYSTILLPAL );
        }
        else    //  suppose it's MOTION
        {
            // still can get more detail about NTSC/PAL motion
            bMode= ( MODE_PLAYMOTION );
        }
    }

#endif  // #ifndef NO_DISC_MODE

    return bMode;

}

BYTE UTL_CheckBGPlayItemMode ( WORD nItem )
{
    BYTE        bTemp;

    // check CDROM title playing mode
    if(__wDiscType & CDROM_M1) // playable CDROM
    {
        bTemp=MM_GetFileType(__MMBackgroundPlayingItem.wDirID, nItem);
        // wyc1.24, the reset stage of re-booting when JPG hang.

        if(bTemp==EXTNAME_MP3)
        {
            return ATTR_MP3;
        }
        else if (bTemp==EXTNAME_MP2)
        {
            return ATTR_MP2;
        }
        else if(bTemp==EXTNAME_WMA)
        {
            return ATTR_WMA;
        }
        else if(bTemp==EXTNAME_AC3)
        {
            return ATTR_AC3;
        }
        else if(bTemp==EXTNAME_DTS) // LLY0.70, support *.DTS file playback
        {
            return ATTR_DTS;
        }
    }

    return ATTR_NONE;
}





//  *********************************************************************
//  Function    :   UTL_SetPlayItemMode
//  Description :   First check the mode of the play item, then set relative
//                  command to MPEG chip
//  Arguments   :   nItem   : The play item number
//  Return      :   bTemp   : return the play mode for this play item
//  Side Effect :
//  *********************************************************************
BYTE UTL_SetPlayItemMode( WORD nItem )
{
    BYTE        bTemp;

    // Get play mode of the desired play item. ex. MODE_PLAYXXX
    // In additional, playing mode attribute will be kept in __bAttrPlayNew together
    bTemp= UTL_CheckPlayItemMode( nItem );

    // Set the relative programming for different play mode
    UTL_SetPlayMode ( bTemp );

    return bTemp;
}


// LLY1.01 create ...
//  ********************************************************************************
//  Function    :   UTL_Init_PlayItem
//  Description :   Do the initial action before playback a new item
//                  ie. it was splited from UTL_PlayItem() w/ desired time = 0
//  Arguments   :   None
//  Return      :   None
//  Notice      :   Only can be called while user make sure to re-play the new item
//                  Otherwise, some variables will be reset, ex.
//                  __dwTimeBegin, __dwTimeEnd, __bMMState, ....
//  ********************************************************************************
void UTL_Init_PlayItem(WORD nItem)
{
    extern  BYTE    __bPBReady;

    // only get __dwTimeBegin/__dwTimeEnd when starting play from beginning
    // For fast or goto time, can't get __dwTimeEnd again
    // Because in VCD 2.0, the __dwTimeEnd will be adjusted
    // slove JVC play time test will play over range in fast mode

// LLY0.85, add for DivX PSNR testing
#ifdef  SUPPORT_PSNR_TEST
    // LLY0.80a, reset the displayed frame count as zero while start to playback.
    extern DWORD __dwDecodeCnt;
    extern DWORD __dwCnt;

    __dwDecodeCnt=0;
    __dwCnt=0;
#endif  // #ifdef SUPPORT_PSNR_TEST

    // reset audio channel to ID 0
    // ** TCH0.38; begin... Only CVD/ SVCD/ DVD has Multiple Audio/
    // DVD should not re-set the Audio Channel. Need to keep same...
    // LLY.104, reset the OGT ID to same as Audio channel
    if ( __wDiscType & BOOK_2X )
    {
        // wyc1.07-909, don't reset SP & audio ID to keep same value. Because MTK also will keep same valie.
        /*__bASTNO=0;
        __bASTID= __bASTNO;
        __bSPSTNO=0;
        __bSPSTID=__bSPSTNO;*/

        // LLY1.11, parser support CVD OGT from v1.10
#if 0
        // LLY1.06-1, set CVD OGT ID as un-exist temporally
        if(__wDiscType & BOOK_CVD)
        {
            __bSPSTNO=0xFF;
            __bSPSTID=__bSPSTNO;
        }
#endif  // #if 0


        // LLY1.02, don't exist SPST/ AST stream language code information, set it as 0xFFFF
        // Notice: assume it un-exist, and set is as 0xFFFF
        __wSP_LCD=0xFFFF;
        __wA_LCD=0xFFFF;
    }


    //Kevin1.22a, avoid _dwTimerCount_MainLoop being erroneously modified by somebody s.t.
    // MONITOR_AudioStream() will never be called
    // e.g. _dwTimerCount_MainLoop=0x172e9a (wrong)  _dwCountSystem=0x12a209
    //      The below in cc.c will never hold:
    //      if ( (UTL_GetSysTimer () > _dwTimerCount_MainLoop) )
    {
        extern DWORD    _dwTimerCount_MainLoop;
        _dwTimerCount_MainLoop=0;
    }

    // Get the play range of the desired item
    UTL_GetPlayRange( nItem );

    // DVD_170Micky, enhance MP3 display time method
    // accumulate frames count.
    _dwTotalFrameCount=0;
    _dwPreFrameCount=0;
    // wyc1.20-909, init it here
    __bLastPlayItem = FALSE;

    // must clear repeat AB status when change play item or stop
    if (__bRepeatAB != REPEAT_NONE)
    {
        __bRepeatAB=REPEAT_NONE;

        // this will clear the MSG_PLAYING
        // if time is 0, the OSD won't be cleared!
        // LLY0.72, give zero time-out value to clear the repeat A-B message
        OSD_OUTPUT_MACRO(MSG_REPEATAB, REPEAT_NONE, 0);
    }

#ifndef NO_DISC_MODE
    if( __wDiscType & BOOK_DVD )   // ** TCH0.375; Modify it after release as it will cause strange status
    {
        // The player is wait-up from STOP mode. Force NV-State Machine enter a normal
        NV_Action ( ACTION_PLAY, 0 );
    }
#endif  // #ifndef NO_DISC_MODE

    // LLY0.66, stop the servo and parser here before start a new item playback
    SrcFilter_Stop(__dwSFStreamID);
    HAL_ControlParser(HAL_PARSER_STOP, 0);

    // Tell decoder to update sequence header
//Micky2.04    COMDEC_Command(MODE_UPDATE_SEQ, 0);
    HAL_PlayCommand(COMMAND_V_UPDATE_SEQ, 0);

    // LLY.275a begin ...
    // Following procedure only need to done while play new item
    // Not, pause --> play case

    // Reset recover stage while start to play new item
    // wyc0.95, rename to DWORD.
    // Micky1.07_909, only reset recover stage when play a new item begin.
    if ( (__bKeySource != KEY_FROM_ISR) && (_bJumpTime == TRUE) )
    {
		_bJumpTime = FALSE;
    }
    else
    {
        __dwMONRecoverStage=0;
		_bJumpTime = FALSE;
    }
    // Micky1.07_909, only reset recover stage when play a new item end.


    // LLY.275a, clear MM_Trigger() relative info. before playing new item
    __bMMStage=MM_STAGE_PREPAREINFO;

    // LLY1.20, reset motion state as none.
    __bMotionState=MOTION_STATE_NONE;

    // LLY1.02, reset the __bPBReady as FALSE while re-playback the item
    // Notice: can't put the assignment @ MM_STAGE_PREPAREINFO
    // Otherwise, _MM_MitionTrigger() can't be called.
    __bPBReady=FALSE;

    // LLY0.91, it's un-necessary to set __btPlayEnd=FALSE here
    // Since, it will be done within UTL_PlayFromTime()
    // LLY.276-1, disable __btPlayEnd while playing a new item
    //__btPlayEnd=FALSE;

    // LLY2.50, always clear un-support A-V format OSD before playback a new item
    // Otherwise, previous OSD maybe display until time-out if user press KEY_NEXT immediately
    OSD_Output(MSG_STRING, 0, 0);

}


// LLY1.01 create ...
//  ********************************************************************************************
//  Function    :   UTL_Start_PlayItem
//  Description :   Start the desired play item playback, ex. issue command, send data
//                  ie. it was splited from UTL_PlayItem() w/ start playback
//  Arguments   :
//  Return      :
//  ********************************************************************************************
void UTL_Start_PlayItem(DWORD dwTime)
{
    BYTE    i;
    PARSER_AV_PTS  AV_pts;

    // Assign the desired time value into __dwTimeNow.
    if ( dwTime )
    {
        // must play from the specified time
        __dwTimeNow= dwTime;
    }
    else
    {
        // f/w 0.45
        // move this to UTL_PlayItem
        // do this only dwTime==0
        // or some information might be cleared when hang reset
        // only send once for each play, but for same still, can't set again
        if (!__btPause) // KEY_SCB to the begin in PBC mode, it's in PAUSE
        {
            if (__bModePlay & MODE_STILL)
            {
                __btTemp=FALSE;
            }
            else
            {
                __btTemp=TRUE;
            }
            OSD_OUTPUT_MACRO ( MSG_MOTION, __btTemp, 0);
        }

        // LJY1.10, as don't do track by track detection, must play from begining.
        __dwTimeNow= __dwTimeBegin;

    }

    // LLY0.91, remove the pre-start servo action after desired time calculation done.
    // And, give the seek time near the desired position.
    // Notice: the seek action must be done before issue playback command.
    // LLY1.06, always do pre-seek servo action since UTL_PreStartServo() will do protection.
    // To fix "Stop --> servo stop running --> Play" A/V not smooth issue
    //if(__btPause)
    {
        // Notice: gdwNextLBA is not updated now.
        // So, give the desired position, and minus the tolence time.
//        extern DWORD gdwNextLBA;
//        UTL_PreStartServo( gdwNextLBA);
        UTL_PreStartServo(__dwTimeNow, ACTION_SKIP);
    }


    // Calculate the desired PTS value for DivX GoTime action.
    if(__bAttrPlay == ATTR_AVI)
    {
//        extern DWORD __dwGoToTime;

        // If the desired time is non-zero: must the goto case
        // So, calculate the GoTo PTS value by multiple w/ 90KHz
        if(dwTime)
        {
            // LLY2.36, use __dwGoTimeSTC directly to avoid STC/90K = sec, then *90K to STC issue.
            //__dwGoToPTS=__dwGoToTime*90000;
            __dwGoToPTS = __dwGoTimeSTC;
        }
        // If the desired time is zero: maybe the play from beginning or GoTo 0 case
        // So, always give the GoTo PTS value as zero
        else
        {
            __dwGoToPTS=0;
        }
//        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Desired Time: %lx, GoTime: %lx\n", dwTime, __dwGoToTime);
    }

    // From CT909, always issue command first
    // convert the command state(__bModeCmd) into MPEG command
    // LLY1.01, always issue the playback command here
    // Since, the API only be called while
    // [1] Non-DivX file, since UTL_PlayItem() will call this API while non-DivX file
    // [2] DivX file w/ waiting playback command (ex. re-start the playback case)
    //     ie. it will be called within _MM_MotionTrigger() w/ MM_STAGE_WATI_PLAY
    // [3] DivX file w/ dwTime!=0 case (ex. repeat AB, GoTime, Resume, SCAN->Play)
    //     since it will be handled by UTL_PlayItem()
    UTL_TransferCommandID(__bModeCmd);


    // Then, start to sending data.
    if(__bAttrPlay==ATTR_CDDA) //play CD-DA
    {
        // LLY0.64, start to sending bitstream.
        // And, SrcFilter_ReadSectors() & HAL_ControlCDIF() can be removed.
        // Since, it will be done with UTL_PlayFromTime()
        // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
        UTL_PlayFromTime(__dwTimeNow, __dwTimeEnd);
    }
    else // play MPEG data
    {
        // f/w 0.46
        // must enable interrupt first
        // or IFD may lost
        if ( !dwTime )
        {
            // wyc1.24-2, make MODE_PLAYVIDEO & MODE_PLAYAUDIO don't initial __bFirstPlay
            // because of not using it.
            if ((__bModePlay != MODE_PLAYVIDEO) && (__bModePlay != MODE_PLAYAUDIO))
            {
                __bFirstPlay= 1;
            }
        }

        // open CD-IF data, and check MSF match
        if (__wDiscType & BOOK_DVD)     // TCC278, for mini-DVD use UTL_PlayFromTime, not call it here.
            __btTemp = TRUE ;
        else
        {
            // LLY0.95, get AVI correct position by parser API
            // 1st parameter: always the file begin sector
            // 2nd parameter: desired time (in msec unit) for playback.
            // 3nd parameter: desired index entry offset current time
            // Notice:
            // [1] 0 means next & not playback index entry
            //     So, must give -1 to get previous one entry index
            //     Otherwise, we can't see the desired picture, and see the next I-frame directly
            // [2] don't care index entry value if the desired time is zero.
            if( __bAttrPlay == ATTR_AVI )
            {
// Grievous2.77, if define it, we will not need to wait for 3~4 sec to start playback when scan/goto/repeat mode.
// Since sometimes (previous index position < audio position < go time position = index position <= video position),
// parser will begin from previous index position.
// And this solution may loss about half second audio instead of playback directly.
#ifdef UTL_SCAN_QUICK
                i=0;
#else  // #ifdef UTL_SCAN_QUICK
                // LLY1.00, modify the flow while get desired time position.
                i=1;
#endif // #ifdef UTL_SCAN_QUICK
                while(1)
                {
                    __dwTimeNow = PARSER_GetContainerTimePosition(__dwTimeBegin, dwTime*1000, (-1)*(i), &AV_pts);

#ifdef  DEBUG_AVI_GOTIME
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "VPTS: %lx; APTS: %lx\n", AV_pts.dwVideoPTS, AV_pts.dwAudioPTS);
#endif  // #ifdef DEBUG_AVI_GOTIME


                    // Add protection code if can't find the desired indext entry
                    // Just playback from file beginnin.
                    if(__dwTimeNow == PARSER_FAIL_SECTOR)
                    {
                        __dwTimeNow = PARSER_GetContainerTimePosition(__dwTimeBegin, 0, 0, 0);

                        // LLY1.11, also reset GoTime PTS as zero while playback from file beginning
                        __dwGoToPTS=0;

                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Can't find desired index entry\n");
                        break;
                    }
#ifdef UTL_SCAN_QUICK
                    //Video only
                    if(__bASTID==HAL_THROW_AUDIO)
                    {
                        if(AV_pts.dwVideoPTS <= (__dwGoToPTS + 45000))
                        {
                            break;
                        }
                    }   // if(__bASTID==HAL_THROW_AUDIO)
                    //Audio only
                    else if(__bVideoID==HAL_THROW_VIDEO)
                    {
                        if(AV_pts.dwAudioPTS <= (__dwGoToPTS + 45000))
                        {
                            break;
                        }
                    }   // if(__bVideoID==HAL_THROW_AUDIO)
                    else if( AV_pts.dwVideoPTS <= (__dwGoToPTS + 45000))
                    {
                        if(i==0 && ((AV_pts.dwAudioPTS- AV_pts.dwVideoPTS) <= 45000))
                        {
                            break;
                        }   // if(i==0 && ((AV_pts.dwAudioPTS- AV_pts.dwVideoPTS) <= 45000))
                        if( AV_pts.dwAudioPTS <= (__dwGoToPTS + 45000))
                        {
                            break;
                        }   // if( AV_pts.dwAudioPTS <= (__dwGoToPTS + 45000))
                    }   // if(__bASTID==HAL_THROW_AUDIO)

#else  // #ifdef UTL_SCAN_QUICK
                    // Make sure the desired index entry audio PTS value <= GoTime PTS
                    // To avoid audio lost since DivX A/V is layout by interleaving mode
                    if(AV_pts.dwAudioPTS<=__dwGoToPTS)
                    {
                        break;
                    }
#endif // #ifdef UTL_SCAN_QUICK

                    i++;
                }

#ifdef  DEBUG_AVI_GOTIME
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AudioPTS: %lx, GoToPTS: %lx, Loop: %lx\n", AV_pts.dwAudioPTS, __dwGoToPTS, i);
#endif  // #ifdef  DEBUG_AVI_GOTIME

#ifdef  ENABLE_VIDEO_IMMEDIATELY_WHILE_AVI_REPEAT_AB
                // LLY2.20, modify the __DwGoToPTS as desired index table I-frame PTS
                // To let the motion can be playback immediately
                if(__bRepeatAB == REPEAT_AB)
                {
#ifdef  DEBUG_AVI_GOTIME
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Eenable video immediately\n");
#endif  // #ifdef  DEBUG_AVI_GOTIME
                    __dwGoToPTS = AV_pts.dwAudioPTS;
                }
#endif  // #ifdef ENABLE_VIDEO_IMMEDIATELY_WHILE_AVI_REPEAT_AB

                // Set desired PTS value to parser.
                PARSER_Command(0, PARSER_CMD_AVI_PTS, (void *)__dwGoToPTS);

//            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "GoTime: %lx, Cnt: %lx, Desired PTS: %lx\n", __dwTimeNow, i, __dwGoToPTS);
//            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "VPTS: %lx, APTS: %lx", AV_pts.dwVideoPTS, AV_pts.dwAudioPTS);
            }
            // LLY2.35, must ask parser and reference DSP information can get current sector position.
            else if(__bAttrPlay == ATTR_WMA)
            {
                __dwTimeNow=UTL_WMATimeToSectors(dwTime);

#ifdef  DEBUG_WMA_GOTIME
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Time: %lx, Sector: %lx\n", dwTime, __dwTimeNow);
#endif  // #ifdef DEBUG_WMA_GOTIME
            }

            // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
            __btTemp= UTL_PlayFromTime ( __dwTimeNow, __dwTimeEnd );
        }

        // find MSF fail
        if ( ! __btTemp )
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Find MSF fail, Position: %lx\n", __dwTimeNow);
            // LLY0.67, can't read data directly for CT909 if UTL_PlayFromTime() fail
            // Need check how to do.
        }
        else
        {
            // LLY1.06, clear the video as black screen if DivX w/ un-supported resolution video
#ifdef  BLACK_SCREEN_WHILE_THROW_VIDEO
            // LLY2.51, only black screen while playback motion and throw video ID
            // Since, if playback un-support format video AVI then playback JPEG
            // Then, nobody to set __bVideoID
            // if(__bVideoID==HAL_THROW_VIDEO)
            if( (__bModePlay & MODE_PLAYMOTION) && (__bVideoID == HAL_THROW_VIDEO) )
            {
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Black Screen for un-support video resolution\n");
                HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
            }
#endif  // #ifdef BLACK_SCREEN_WHILE_THROW_VIDEO
        }

    }   // NO-CDDA


    // must set status here
    // for play motion auto pause re-entry problem
    __btPlaying=TRUE;
    __btPause=FALSE;


    // DVD_103Micky, fix VCD play->pre-stop->play->stop->won't enter pre-stop?
    // as resume will complete through several keys conversion.
    __bCountStop=0;


}


//  ********************************************************************************
//  Function    :   UTL_PlayItem
//  Description :   Get the play item playing range, and issue command, then
//                  start input data by the specified time
//  Arguments   :   nItem       : PIN as def. in VCD2.0 if NO-DVD
//                              : NO-Use in DVD, Refer __bTitle/ __wChapter
//                  dwTime      : 0 means from start point of [nItem]
//                              : other value will play from the specified time
//  Return      :   TRUE
//  Side Effect :
//  ********************************************************************************
WORD UTL_PlayItem( WORD nItem, DWORD dwTime )
{
    extern  BIT     __btNewTime;

    //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Play Item: %d\n", nItem);


    // Micky0.85, The condition (!dwTime) means play from track begin.
    // the necessary setting before playing each track can put here
    // LLY1.05-2, It's un-necessary to do this for GoTime w/ zero time case too.
    if( ! dwTime && !__btNewTime)
    {
        // LLY1.01, don't do initial for CDROM motion file
        // Because, the procedure of CDROM motion is implemented w/ stage trigger
        // So, the initial action will be done within LINEAR_PlayItem()
        // And, the playback setting will be done at last stage.
        if( (__wDiscType&CDROM_M1) && (UTL_CheckPlayItemMode(nItem)==MODE_PLAYMOTION) )
        {
        }
        else
        {
            UTL_Init_PlayItem(nItem);
        }
    }

    // LLY0.65, remove the JPEG error recover code since it's only CT908 bug


    // Set the desired playing item mode.
    UTL_SetPlayItemMode ( nItem );

    // LLY0.72, remove old recover mechanism since it will be re-design for CT909

    // LLY.161-2, detect CDDA format : pure/DTS CDDA
    // while playing CDDA from track begin
    // Micky1.24-3, fix CD/ JVC-CDDA power on resume don't work
    // still don't know the real issue.
//    if( (__bAttrPlay==ATTR_CDDA) && !dwTime )
    if( (__bAttrPlay==ATTR_CDDA) && (!dwTime || __btNewTime))
    {
        // wyc.172-CDDA, when playing CDDA medley title we not execute CD-DTS title checking when across tracks.
#ifdef SUPPORT_PLAY_MEDLEYCDDA
        //LJY1.23, use non-DTS instead of CDDA_PURE becasue CDG bitstream is no longer CDDA_BITSTREAM_PURE.
        if ((__bCDDAAutoChTrk) && (__bCDDA_DTS!=CDDA_DTS))
        {
            // tell dsp start to decode pure CDDA,
            HAL_WriteAM(HAL_AM_PCM_MODE, 1) ;
            __bCDDAAutoChTrk = FALSE;
        }
        else

#endif  // #ifdef SUPPORT_PLAY_MEDLEYCDDA
        {

            UTL_GetCDDAFormat(UTL_GETCDDAFORMAT_RUNTIME);
        }

        // wyc.170-2, execute the read Q-channel command to get the information for de-emphasis
        UTL_SetCDDADeEmphasis();

    }


    // LLY0.91, remove the pre-start servo action after desired time calculation done.


    if (!__btPlaying && (!__btPause || __btNewTime))
    {
        // LLY0.80a, always do A/V sync. control as Video Master mode while
        // (1) playback a new item or (2) bitstream dis-continue;
        // Since, new A/V sync control code for A/V decoder.
        // wyc2.36a-909s, set audio master when mpeg-cd.
#ifdef SUPPORT_FUSS_MPEG_CD
        if (INFOFILTER_FUSSMPEGCD(IF_MODE_CHECK_MPEG_CD,0))
            HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, FALSE);
        else
            HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, TRUE);
#else
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (!__bMMJPEGAudioMode)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
        {
        HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, TRUE);
        }
#endif //

    }

    // LLY2.53, remove audio frequency detection procedure outside
    // Since, it's un-necessary to re-do while stop/resume, GoTime ...
    // ie. it's only need while 1st playback the item.
    if(!dwTime && !__btNewTime)
    {
        // for run time detect audio frequency for different frequency
        // ex. SVCD, CDROM title (since we always set it as 44.1K)
        if( ( __wDiscType & BOOK_SVCD ) || (__wDiscType & BOOK_M1) )
        {
            __bCheckAudioType |= CHECK_AUDIO_FREQ;

           // Kevin1.25, fix "SPDIF discont. while changing files" and "Freq wrong at file beginning"
           // LLY1.06, always to tell DSP to wait after audio frequency detection done.
#if 0
           if( (__bAttrPlay == ATTR_MP3)
#ifdef  SUPPORT_WMA
                || (__bAttrPlay == ATTR_WMA)
#endif  // #ifdef SUPPORT_WMA
              )
#endif  // #if 0
            {
                // host tells DSP to wait for decoding only while changing files
                HAL_WriteAM(HAL_AM_START_DECODE, 0);
            }
        }
        else
        {
            // clear "CHECK_AUDIO_FREQ" part
            __bCheckAudioType &= ~CHECK_AUDIO_FREQ;
        }
    }


    // LLY1.01, start the playback here only while
    // [1] Non DivX file.
    //     Since DivX file re-start playback need do DRM checking first
    //     So, _MM_MotionTrigger() will call UTL_Start_PlayItem() after DRM checking done.
    // [2] DivX file w/ desired time != 0, ex. GoTime, Resume, Repeat AB, SCAN->PLAY
    //     Notice: must check __btNewTime flag, since it's also necessary for GoTime w/ zero time -- LLY1.05-2
    if(__bAttrPlay == ATTR_AVI)
    {
        if(dwTime || __btNewTime)
        {
            UTL_Start_PlayItem(dwTime);
        }

        // LLY1.05, Disable audio frequency checking for AVI playback w/ video only stream
        if(!__bASTNs)
        {
            __bCheckAudioType &= ~CHECK_AUDIO_FREQ;
        }

    }
    else
    {
        UTL_Start_PlayItem(dwTime);
    }

    return TRUE;
}


// LLY2.53 create
//  ***********************************************************************************
//  Function    :   UTL_PlayImage
//  Description :   Playback a video only item
//  Arguments   :   bMode, specify the desired video only image
//  Return      :   None
//  ***********************************************************************************
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
void UTL_PlayImage(BYTE bMode)
{
    extern BYTE _Wait_Decoder_Stop_CMD_ACK(void);
    extern void JPEGDEC_ConfigProcessor(WORD wConfigProcessor);


    // Step 1: Stop video decoder reset
    // Notice: can't call HAL_RESET_VIDEO directly since it's related parser & PROC2 control
    //         So, abstract the necessary part.
    //HAL_Reset(HAL_RESET_VIDEO);
    // [1-1] stop decoder
    if(bMode == ATTR_JPG)
    {
        HALJPEG_Abort();
    }
    else
    {
        COMDEC_Command(MODE_STOP, 0);
        _Wait_Decoder_Stop_CMD_ACK();
    }

    // [1-2] notify parser not to parser video stream
    PARSER_ActionFilter( BG_ITEM_STREAM_ID, PARSER_ENABLE_VIDEO, FALSE );

    // [1-3] notify parser to clear video stream write ptr.
    PARSER_BufferReset( PARSER_ES_VIDEO );

    // Step 2: Configure video only related buffer
    // Notice: can't call UTL_Config_BufferMode() directly since some procedure is not necessary
    //UTL_Config_BufferMode(DUMP_BUFF_MM_VIDEO_ONLY_MODE);
    // [2-1] Servo dump-in buffer
#ifdef SMALL_SOURCE_BUFFER
    SrcFilter_ConfigBuffer( BG_ITEM_STREAM_ID, DS_INPUT_BITBUF_ST_MM_SMALL,
        ((DS_INPUT_BITBUF_END_MM_SMALL-DS_INPUT_BITBUF_ST_MM_SMALL)/SERVO_BUFSIZE_ALIGN_UNIT) );
#else
    SrcFilter_ConfigBuffer( BG_ITEM_STREAM_ID, DS_INPUT_BITBUF_ST_MM,
        ((DS_INPUT_BITBUF_END_MM-DS_INPUT_BITBUF_ST_MM)/SERVO_BUFSIZE_ALIGN_UNIT) );
#endif //

    // [2-2] Parser video dump-out buffer and VLD as MM mode
    HAL_SetVideo_Buffer(HAL_VIDEOBUF_MM);

    // [2-3] frame buffer for JPEG slide show.
    if(bMode==ATTR_JPG)
    {
        if(__dwMMJPEGPreview)
        {
            // for preview mode
            HALJPEG_ConfigFrame(HALJPEG_FRAME_BUFFER_PREVIEW);
        }
        else
        {
            // for slide show
            HALJPEG_ConfigFrame(HALJPEG_FRAME_BUFFER_SLIDE_SHOW);
        }
    }
    else
    {
        // for other video only image
    }


    // Step 3: Create the JPEG thread if it's un-exist
    if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_JPEG_DONE) )
{
        INITIAL_ThreadInit(THREAD_JPEG_DECODER);

        // LLY0.75, release CPU first before waiting flag
        // Otherwise, may can't get the JPEG thread initial done flag
        // Since, JPEG thread is the same priority as CheerDVD F/W
        OS_YieldThread();

        OS_TimedWaitFlag( &__fThreadInit, INIT_DEC_THREAD_JPEG_DONE, FLAG_WAITMODE_AND,
            (COUNT_100_MSEC) );

        // print-out the warning message if the thread initial fail.
        if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_JPEG_DONE) )
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "JPEG thread not initial done, value: %lx\n", OS_PeekFlag(&__fThreadInit));
        }

        // Notice: frame buffer address setting for JPEG mode is put within HALJPEG_Initial()

        //kevin1.00, JPEG & MPEG use diff gains
        __bTVE_JPEG = TRUE;
        HAL_SetTVE();
    }

    // Step 4: Enable parser end padding feature.
    PARSER_ActionFilter(BG_ITEM_STREAM_ID, PARSER_ENABLE_END_PADDING, TRUE);

    // Step 5: do video only related setting
    // Notice: can't call _UTL_ProgVideoOnly() directly since it's related many parser setting.
    //__bAttrPlay=ATTR_JPG;
    //_UTL_ProgVideoOnly();
    // [5-1] config parser data format as video only mode
    if (__bDVDROM)
    {
        PARSER_SetDataFmt( BG_ITEM_STREAM_ID, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_DVD_2048), PARSER_BSFMT_VIDEO_ONLY );
    }
    else
    {
        PARSER_SetDataFmt( BG_ITEM_STREAM_ID, (PARSER_DATAFMT_LINEAR|PARSER_DATAFMT_CDROM), PARSER_BSFMT_VIDEO_ONLY );
    }
    // [5-2] Do JPEG related setting.
    // Notice: can't call HALJPEG_Initial() directly since it related to parser control procedure.
    //HALJPEG_Initial();
    // It seems un-necessary to load JPEG code @ PROC2 since this API is only for background video only playback
    // But, must config JPEG decoder as one processor
    JPEGDEC_ConfigProcessor(0x1);

    // LLY2.61, porting Chuan's code to remove following Display frame setting procedure
    // Since, it will be done with HALJPEG_ConfigFrame() --> DISP_FrameBufferSet()
#if 0
    __DISPFrameInfo[0].bSrcFrame = 1;
    __DISPFrameInfo[0].bDispFrame = 1;
    __DISPFrameInfo[0].bBFR_Mode = 0;
    __DISPFrameInfo[1].bSrcFrame = 1;
    __DISPFrameInfo[1].bDispFrame = 1;
    __DISPFrameInfo[1].bBFR_Mode = 0;
    __DISPFrameInfo[2].bSrcFrame = 1;
    __DISPFrameInfo[2].bDispFrame = 1;
    __DISPFrameInfo[2].bBFR_Mode = 0;
#endif  // #if 0

    //Set the source (display buffer) for main video.
    HALJPEG_SetDisplay();

    // [5-3] Initialize JPEG display time as very-very small value
    //       Then, it will always time-out for 1st Slide-Show
    // Notice: must check if it's necessary to put here.
    __dwMMJPEGDisplayTime=0;

    // Step 6: restore parser to parser video stream continue.
    PARSER_ActionFilter( BG_ITEM_STREAM_ID, PARSER_ENABLE_VIDEO, TRUE );

}
#endif  // #ifdef SUPPORT_JPEG_AUDIO_PLAYING


// LLY2.53 create
//  ***********************************************************************************
//  Function    :   UTL_StopImage
//  Description :   Stop current video only image playback procedure
//  Arguments   :   bMode, specify the desired video only image mode.
//  Return      :   None
//  ***********************************************************************************
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
void UTL_StopImage(BYTE bMode)
{
    DWORD   dwTimer, dwTemp=TRUE;
    extern BYTE _Wait_Decoder_Stop_CMD_ACK(void);

    // Stop video decoder
    if(bMode == ATTR_JPG)
    {
        HALJPEG_Abort();
    }
    else
    {
        COMDEC_Command(MODE_STOP, 0);
        _Wait_Decoder_Stop_CMD_ACK();
    }

    // Stop parser
    PARSER_Command(BG_ITEM_STREAM_ID, PARSER_CMD_STOP, 0);
    dwTimer=OS_GetSysTimer();
    while( (OS_GetSysTimer() - dwTimer) < COUNT_50_MSEC)
    {
        dwTemp=PARSER_QueryEvent(BG_ITEM_STREAM_ID, PARSER_EVENT_COMPLETED, TRUE);

        // Parser stop command ok
        if(dwTemp)
        {
            break;
        }

        // release the CPU
        OS_YieldThread();
    }

    // Parser stop command fail
    if(!dwTemp)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Parser stop fail\n");
    }

    // Stop source
    SrcFilter_Stop(BG_ITEM_STREAM_ID);


}
#endif  // #ifdef SUPPORT_JPEG_AUDIO_PLAYING


// LLY2.53 create
//  ***********************************************************************************
//  Function    :   UTL_PlayAudio
//  Description :   Playback a audio only item
//  Argumenst   :   bMode, specify the desired audio mode. ex. HAL_AUDIO_XXX
//  Return      :   None
//  ***********************************************************************************
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
void UTL_PlayAudio(BYTE bMode)
{
    extern  DWORD   __dwVolAudio;
    BOOL    bReloadAudio;

    //printf( "PLAY AUDIO" );

    // Step 0: Do audio decoder reset (it will do stop too)
    // Notice: can't call HAL_RESET_AUDIO directly since it related to parser control.
    //         so, abstract the procedure step by step
    HAL_WriteAM(HAL_AM_PLAY_COMMAND, 0);
    PARSER_ActionFilter( BG_ITEM_STREAM_ID, PARSER_ENABLE_AUDIO, FALSE );

    // Notice: can't call _UTL_ProgAudioOnly() directly, since it's too many
    //         So, only abstract the necessary part (step 1 ~ 4)
    // Step 1: Config servo dump-in
#ifdef SMALL_SOURCE_BUFFER
    SrcFilter_ConfigBuffer( BG_ITEM_STREAM_ID, DS_INPUT_BITBUF_ST_MM_SMALL,
        ((DS_INPUT_BITBUF_END_MM_SMALL-DS_INPUT_BITBUF_ST_MM_SMALL)/SERVO_BUFSIZE_ALIGN_UNIT) );
#else
    SrcFilter_ConfigBuffer( BG_ITEM_STREAM_ID, DS_INPUT_BITBUF_ST_MM,
        ((DS_INPUT_BITBUF_END_MM-DS_INPUT_BITBUF_ST_MM)/SERVO_BUFSIZE_ALIGN_UNIT) );
#endif //

    // Step 2: Config audio related setting, ex AM, load code, audio dump-out buffer, ...
    // Notice: can't call HAL_SetAudioType() directly since it will call HAL_RESET_AUDIO too
    //         So, only abstract the necessary procedure
    //         ex. load desired audio code, and set related AM
    //         BTW, don't porting SPDIF mode related setting.
    // HAL_SetAudioType(__bAudioType);

    // Nothing to do if same audio mode.
    bReloadAudio = (bMode != __bAudioType);
//    if(bMode != __bAudioType)
    {
        printf("Play audio: %lx", (DWORD) bMode);
        // Keep desired audio mode to __bAudioType
        __bAudioType = bMode;

        // [2-1] reload audio decoder code
        // Notice: must change desired audio code first, then can start audio related configuration
        //         since, the setting will be work @ new audio code.

        //senshong2.38a: JPEG decoder use REG_AIU_GR(2) ~ REG_AIU_GR(17) to connect width processor 2 from processor 1
        //So before playback audio, we need to reset REG_AIU_GR(2) ~ REG_AIU_GR(17).
#ifdef SUPPORT_JPEGDEC_ON_PROC2
        {
            BYTE bIndx;
            for ( bIndx=2; bIndx<17; bIndx++)
            {
                REG_AIU_GR(bIndx)= 0;
            }
        }
#endif

        if( bReloadAudio )
        {
            HAL_ReloadAudioDecoder(bMode);
        }

        // Notice: must call HAL_ResetAudioDecoder() after change audio code to let new audio code valid
        // Otherwise, R/W AM action maybe fail.
        HAL_ResetAudioDecoder();

        // [2-2] set audio buffer / control parameters
        switch(bMode)
        {
        case    HAL_AUDIO_WMA:
            HAL_WriteAM(HAL_AM_AUDIO_TYPE, 6);
            HAL_SetAudio_Buffer(TYPE_CDROM_AUDIO, HAL_AUDIOBUF_WMA);
            break;

        case    HAL_AUDIO_MP3:
        case    HAL_AUDIO_MP2:
#ifdef  SUPPORT_MPEG_AUDIO_UP_SAMPLE
            HAL_WriteAM(HAL_AM_UPSAMPLE_EN, 1);
#else
            HAL_WriteAM(HAL_AM_UPSAMPLE_EN, 0);
#endif
            HAL_WriteAM(HAL_AM_AUDIO_TYPE, 2);
            HAL_SetAudio_Buffer(TYPE_CDROM_AUDIO, HAL_AUDIOBUF_MP3);
            break;

        default:
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Not implement case for UTL_PlayAudio(): %lx\n", bMode);
            break;
        }

        // Notice: must call HAL_ResetAudioDecoder() again to let new audio type valid (HAL_AM_AUDIO_TYPE)
        HAL_ResetAudioDecoder();


    }

    // Step 3: Set data format to parser
    // ####: need check since control parser by index 0
    if(bMode == ATTR_WMA)
    {
        // Notice: must enable WMA audo bits format for WMA audio type
        PARSER_SetDataFmt(BG_ITEM_STREAM_ID, PARSER_DATAFMT_NOCHANGE, (PARSER_BSFMT_AUDIO_ONLY|PARSER_BSFMT_WMA));
    }
    else // MP3, MP2, DTS, AC3 (put in CDROM or DVDROM title)
    {
        PARSER_SetDataFmt( BG_ITEM_STREAM_ID, PARSER_DATAFMT_NOCHANGE, PARSER_BSFMT_AUDIO_ONLY);
    }


    // Step 4: Enable audio only parseing
    // ####: need check since control parser by index 0
    PARSER_ActionFilter( BG_ITEM_STREAM_ID, PARSER_ENABLE_AUDIO, TRUE );

    // Step 5: Issue playback command
    HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);

    // Step 6: Open audio output while no mute audio mode -- LLY2.55
    // Notice: don't call CHIPS_OpenAudio(TRUE) since it's too many.
    //CHIPS_OpenAudio(TRUE);
    if(!__btMute)
    {
        HAL_WriteAM(HAL_AM_PCM_SCALE, __dwVolAudio); //16 bits
    }


    // Step 7: Enable audio frequency checking, and tell decoder to wait next start decode command.
    __bCheckAudioType |= CHECK_AUDIO_FREQ;
    HAL_WriteAM(HAL_AM_START_DECODE, 0);

}
#endif  // #ifdef SUPPORT_JPEG_AUDIO_PLAYING


// LLY2.53 create
//  ***********************************************************************************
//  Function    :   UTL_StopAudio
//  Description :   Stop current audio only playback
//  Arguments   :   None
//  Return      :   None
//  ***********************************************************************************
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
void UTL_StopAudio(void)
{
    DWORD   dwTimer, dwTemp=TRUE;

    // printf( "STOP AUDIO" );

    // Stop audio decoder
    HAL_WriteAM(HAL_AM_PLAY_COMMAND, 0);

    // Stop parser
    PARSER_Command(BG_ITEM_STREAM_ID, PARSER_CMD_STOP, 0);
    dwTimer=OS_GetSysTimer();
    while( (OS_GetSysTimer() - dwTimer) < COUNT_50_MSEC)
    {
        dwTemp=PARSER_QueryEvent(BG_ITEM_STREAM_ID, PARSER_EVENT_COMPLETED, TRUE);

        // Parser stop command ok
        if(dwTemp)
        {
            break;
        }

        // release the CPU
        OS_YieldThread();
    }

    // Parser stop command fail
    if(!dwTemp)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Parser stop fail\n");
    }

    // Stop source
    SrcFilter_Stop(BG_ITEM_STREAM_ID);


}
#endif  // #ifdef SUPPORT_JPEG_AUDIO_PLAYING


// LLY2.35 create
//  ***********************************************************************************
//  Function    :   UTL_WMATimeToSectors
//  Description :   Ask parser to get sector ID of desired WMA time (in second unit)
//  Arguments   :   Specify the desired time (in second)
//  Return      :   Correct sector ID which is packet aligment
//  Notice      :   Only valid for WMA only audio stream
//  ***********************************************************************************
DWORD UTL_WMATimeToSectors(DWORD dwTime)
{
    DWORD dwHeaderSize;
    DWORD dwPacketSize;
    DWORD dwSector;

    HAL_ReadAM(HAL_AM_WMA_HEADER_SIZE, &dwHeaderSize);
    HAL_ReadAM(HAL_AM_WMA_PACKET_SIZE, &dwPacketSize);

    dwSector=PARSER_getWMATimePosition(__dwTimeBegin, dwHeaderSize, dwPacketSize, __wHALAudioBitrate, dwTime);

    return dwSector;
}

// LLY2.04 create
//  ***********************************************************************************
//  Function    :   UTL_FrameCntToSectors
//  Description :   Convert the total decoded frame count to decoded sector number
//  Arguments   :   None
//  Return      :   Decoded sector number
//  Notice      :   Only can called this API while playback CDROM/DVDROM audio file
//  ***********************************************************************************
DWORD UTL_FrameCntToSectors(void)
{
    DWORD   dwTemp;

    if (__dwHALAudioFreq)
    {
        // read-back current audio total decoded frame count
        HAL_ReadAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, &dwTemp);

        // Calculate current decoded sample = frame count * (sample_per_frame)
        //dwTemp *=__wHALSamplesPerFrame;

        // Calculate current decoded seconds = samples/ audio freq
        //dwTemp /= __dwHALAudioFreq;

        // Calculate decoded sector = (decoded seconds * bitrate*1000)/8/2048
        // Notice: *1000 since DSP report bitrate in K=1000 unit
        //         /8 to transfer to BYTE unit
        //         /2048 to transfer to sector unit
        //dwTemp = dwTemp*__wHALAudioBitrate*125/2048;

        // So, do multiple operation first, then do division operation to avoid loss of division
        // LLY2.30, add WMA part since it only have total samples
        if (__bAttrPlay==ATTR_WMA)
        {
            // LLY2.32, readback value * 8 = real value
            dwTemp = (DWORD)(((ULONGLONG)dwTemp * 8 * __wHALAudioBitrate*125)
                        / ((ULONGLONG)2048*__dwHALAudioFreq));
        }
        else
        {
            dwTemp = (DWORD)(((ULONGLONG)dwTemp * __wHALSamplesPerFrame * __wHALAudioBitrate*125)
                        / ((ULONGLONG)2048*__dwHALAudioFreq));
        }
        return (dwTemp);
    }
    else
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: can't calculate sector number since audio bitrate is zero!\n");
        return 0;
    }


}


// LLY2.04 create
//  ***********************************************************************************
//  Function    :   UTL_SectorsToFrameCnt
//  Description :   Convert the total decoded sector number to total frame count
//  Arguments   :   dwSectorID, specify the decoded sector number
//  Return      :   Total frame count
//  Notice      :   Only can called this API while playback CDROM/DVDROM audio file
//  ***********************************************************************************
DWORD UTL_SectorsToFrameCnt(DWORD dwSector)
{
    DWORD   dwTemp;

    // Calculate the final frame count value base on desired sector number
    if (__wHALAudioBitrate)
    {
        // total second = total sector * 2048 * 8 / (bitrate*1000)
        // Notice: *2048 for sector --> byte
        //         *8 for byte --> bit
        //         /(bitrate*1000) --> for offset second
        // dwTemp = (dwSector*2048)/ (__wHALAudioBitrate*125);

        // decoded sample = total second* frequency
        // dwTemp *= __dwHALAudioFreq;

        // decoded frame = decoded sample/ sample_per_frame
        // dwTemp /= __wHALSamplesPerFrame;

        // So, do multiple all thing first, then do division to avoid division loss
        // LLY2.30, add WMA part since it only have total samples
        if (__bAttrPlay==ATTR_WMA)
        {
            dwTemp = (DWORD)(((ULONGLONG)dwSector*2048*__dwHALAudioFreq)
                    / ((ULONGLONG)__wHALAudioBitrate*125));

            // LLY2.35, must / 8 since DSP keep unit is real value/8
            dwTemp /= 8;
        }
        else
        {
            dwTemp = (DWORD)(((ULONGLONG)dwSector*2048*__dwHALAudioFreq)
                    / ((ULONGLONG)__wHALAudioBitrate*125*__wHALSamplesPerFrame));
        }

        return (dwTemp);
    }
    else
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: can't calculate frame count since audio bitrate is zero!\n");
        return 0;
    }
}


//  *********************************************************************
//  Function    :   UTL_MStoSectors
//  Description :   Convert the time format from hh:mm:ss into sectors
//  Arguments   :   bHour, the hour part
//                  bMin, the minute part
//                  bSec, the sec part
//  Return      :   the corresponding sector number
//  *********************************************************************
DWORD UTL_MStoSectors( BYTE bHour, BYTE bMin, BYTE bSec )
{
    DWORD   dwTemp;

    // Convert the input time into sec unit
    dwTemp = (bHour*3600L + bMin*60L + bSec);

    // LLY0.75, calculate desired sector for CDROM A/V file
    if(__bAttrPlay & TYPE_CDROM_AV)
    {
        // desired sector number = total sector/ total time * desired time.
        // ie. __dwSectorPerSec * desired time
        dwTemp = __dwSectorPerSec * dwTemp;
    }
    else // multiple 75 or 150 for VCD or SVCD title
    {
        dwTemp *= 75L;
        if( __wDiscType & BOOK_2X )
            dwTemp *= 2;
    }

    return dwTemp;
}


//  *********************************************************************
//  Function    :   UTL_SectorstoMSF
//  Description :   Convert the time format from HSG(sector) into mm:ss:ff
//  Arguments   :
//  Return      :   The converted mm:ss:ff value, 0f:sm
//  Side Effect :
//  *********************************************************************
DWORD UTL_SectorstoMSF( DWORD hsgTime )
{
    DWORD   dwTemp;
    if ( __wDiscType & BOOK_2X )
    {
        dwTemp = hsgTime/ 150;
    }
    else
    {
        dwTemp = hsgTime/ 75;
    }

    __bMin = (BYTE)(dwTemp/ 60);
    __bSec = (BYTE)(dwTemp% 60);     // MAX. is 59
    __bFrame = (BYTE)(hsgTime % 75);

    // convert mm:ss:ff into DWORD format xf:sm
    dwTemp= MAKE_MSF ( __bMin, __bSec, __bFrame );

    return dwTemp;
}


//  *********************************************************************
//  Function    :   UTL_GetCurrentTime
//  Description :   Get current parser sent sector ID
//  Arguments   :   None
//  Return      :   Last send sector ID
//  Notice      :   The value is invalid while servo is stopped
//  *********************************************************************
DWORD UTL_GetCurrentTime(void)
{
    DWORD   dwTemp;
    dwTemp=PARSER_GetCurSector(0);

    if(dwTemp >= __dwTimeBegin)
    {
        __dwTimeNow=dwTemp;
    }

    return (dwTemp);
}


// LLY2.04 create
//  *********************************************************************
//  Function    :   UTL_GetDecodedTime
//  Description :   Get current decoded sector ID
//  Arguments   :   None
//  Return      :   Current decoded sector ID
//  ********************************************************************
DWORD UTL_GetDecodedTime(void)
{
    DWORD   dwTemp;

    // DVDROM/ CDROM audio only file
    if( (__wDiscType&BOOK_M1) && (__bAttrPlay&TYPE_CDROM_AUDIO) )
    {
        // Calculate decoded sector from total frame count
        dwTemp=UTL_FrameCntToSectors();

        // Plus track beginning
        dwTemp += __dwTimeBegin;
    }
    else // other titles
    {
        // Parser last sent sector ID
        dwTemp = UTL_GetCurrentTime();

        // Minus buffer remain time
        dwTemp -= UTL_EstimateBufferTime();
    }

    return (dwTemp);

}



//  *********************************************************************
//  Function    :   UTL_ModeInitial
//  Description :   Clear some flags when switching to another mode
//  Arguments   :   bMode, specify the desired mode
//  Return      :   NULL
//  Side Effect :
//  *********************************************************************
void UTL_ModeInitial ( BYTE bMode )
{
    switch ( bMode )
    {
    case    MODE_OPENCLOSE:
#ifdef  SUPPORT_BOOKMARK
    {
        int         i;

        // clear book mark entries
        for ( i= 0; i< ( BOOKMARK_NO+ 1 ); i++ )
        {
            __wBookMark[i]= 0;
        }
    }
#endif  // #ifdef  SUPPORT_BOOKMARK

        // LLY0.60, it's un-necessary to force clear Display mode after open/close for CT909
        // Because, CT909 UI will close all UI while open/close

        __bShowTimeType= MSG_RELTIME; // Is it relative to display mode ??

        // DVD_104Micky, always clear stop state as real stop.
        __bCountStop=2;

    case    MODE_LINEAR:    //initial mode for linear play
        __btPBC= FALSE;
    case    MODE_PBC:               //initial mode for PBC play
        //DVD_099dMicky, don't clear PROGRAM/RANDOM when pre-stop
        // ModeInitial is called AFTER __bCountStop is set
        // LLY2.36, porting Alan's code to fix bug #2025.
        // When program UI is turn-on, don't exit it while auto-playback to disc end.
        if ( ((bMode == MODE_LINEAR) && (__bCountStop==1)) || (__btInputProgram==TRUE))
        {
            // prepare to goto pre-stop
        }
        else
        {
            // Clear program related variables
            __btProgram= FALSE;
            __btInputProgram=FALSE;

            // wyc1.07, need to set __btPlayingProgram to FALSE
            // Amd, tell OSD to clear "P" icon on display mode
            // because KEY_STOP will stop program playing.
            __btPlayingProgram=FALSE;
            OSD_Output(MSG_PROGRAM, 0, 0);

            __btNextPlayProgram=FALSE;

            // Cancel Random mode
            __btRandom= FALSE;
            OSD_OUTPUT_MACRO(MSG_RANDOM, FALSE, 0);
        }

        // Clear Repeat AB Mode
        __bRepeatAB= REPEAT_NONE;
        OSD_OUTPUT_MACRO(MSG_REPEATAB, REPEAT_NONE, 0);

        if (bMode==MODE_PBC)
        {
            __btPBC= TRUE;
            __btPlaying= FALSE;
            __btPause= FALSE;
        }
        //wyc.100 , new place here
#ifndef NO_DVDDIGEST
        if (__btDIGEST)
        {
            __btDIGEST=FALSE;
            __btPlaying = 0;
            __bDVDDigest = FALSE;
        }
#endif // NO_DVDDIGEST

        // DVD_156Micky, must clear pre-input value in number confirm mode
#ifdef  SUPPORT_NUMBER_CONFIRM_KEY
        {
            extern WORD            _wPreValue;
            OSD_OUTPUT_MACRO ( MSG_NUMBER, 0, 0 );  // clear number message
            _wPreValue=0xffff;
        }
#endif
        break;

    case    MODE_PROGRAM:   // initial mode for program input
        // wyc.pro initial some variable for program usage
        __bProgramCurrentIndex = 0;
        __btProgram= TRUE;          // this will be set in cc.c
        __btRandom= FALSE;

        break;

    case    MODE_RANDOM:    // initial mode for random play
        __btRandom= TRUE;   // this will be set in cc.c

        // Cancel program mode relative variables -- LLY0.60
        __btProgram= FALSE;
        __btPlayingProgram=FALSE;
        // wyc1.10-909, need to clear program mode after pressing KEY_RANDOM to fix when program some entries in program edit mode,
        // then press KEY_NEXT, it will go random mode but UI still keep in program edit mode problem.
        LINEAR_Program(LINEAR_PROGRAM_CLEAR,(BYTE)NULL);
        // And, tell OSD to clear "P" icon on display mode
        OSD_Output(MSG_PROGRAM, 0, 0);

        __bRepeatAB= REPEAT_NONE;
        OSD_OUTPUT_MACRO(MSG_REPEATAB, REPEAT_NONE, 0);
        break;

    default:
        break;
    }

    __btIntroMode=FALSE;
    OSD_OUTPUT_MACRO(MSG_INTRO, 0, 0);

#ifdef  SUPPORT_BOOKMARK
    __btBookMark= FALSE;
#endif  // #ifdef  SUPPORT_BOOKMARK

}


//  *********************************************************************
//  Function    :   UTL_ForceChangeTrack
//  Description :
//  Arguments   :   wTrack  : change to the new track
//  Return      :
//  Side Effect :   It'll change __dwTimeBegin/ __dwTimeNow/ __dwTimeEnd
//                  __bModePlay
//                  It'll disable video output
//  *********************************************************************
void UTL_ForceChangeTrack(WORD wTrack)
{
    BYTE    bMode;
    DWORD   dwTemp;

    __bModePlay= MODE_PLAYUNKNOW;

    // must call for CD-DA track
    // or CD-DA scan backward won't change __dwTimeBegin/__dwTimeEnd
    UTL_GetPlayRange ( wTrack );

    // Set the nItem Playing mode and attribute
    bMode= UTL_SetPlayItemMode ( wTrack );

    // LLY0.68, re-implement the pre-decode sequence header issue for CT909
    if ( bMode == MODE_PLAYMOTION )
    {
        // Issue pre-decode command
//Micky2.04        COMDEC_Command(MODE_PREDECODE, FNSQ_CNT_FOR_PREDECODE);
        HAL_PlayCommand(COMMAND_V_PREDECODE, FNSQ_CNT_FOR_PREDECODE);


        // LLY0.70, throw the audio stream while pre-decode mode, since it's un-necessary.
        // Otherwise, parser will stop since audio buffer is fullness
        HAL_ControlParser(HAL_PARSER_AUDIO_ID, HAL_THROW_AUDIO);

        // Start to sending the desired data
        // LLY1.02, need to calculate the position for AVI real A/V start
        // Since, the file beginning includes the AVI header information.
        if(__bAttrPlay == ATTR_AVI)
        {
            __dwTemp = PARSER_GetContainerTimePosition(__dwTimeBegin, 0, 0, 0);

            // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
            UTL_PlayFromTime(__dwTemp, __dwTimeEnd);
        }
        else
        {
            // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
            UTL_PlayFromTime(__dwTimeBegin, __dwTimeEnd);
        }


        // Waiting pre-decode command done
        _dwTimeStart = OS_GetSysTimer();
        // LLY0.91, enlarge the time-out value since servo max seek time is 3 sec
        // And, decode pre-decode time is 2 sec.
        while ( ( OS_GetSysTimer()-_dwTimeStart ) < COUNT_5_SEC)
        {
#ifdef SUPPORT_FUSS_MPEG_CD
            // wyc2.36a-909s, break this loop when mpeg-cd because AV sync having problem.
            if (INFOFILTER_FUSSMPEGCD(IF_MODE_CHECK_MPEG_CD,0))
                break;
#endif //
            HAL_ReadInfo(HAL_INFO_DECCMD_ACK, &dwTemp);

            if (dwTemp == MODE_PREDEC_DONE)
            {
                break;
            }
            // release the CPU
            OS_YieldThread();
        }

        if(dwTemp != MODE_PREDEC_DONE)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Pre-decode fail\n");
            //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "VRem: %lx; VDFIFO: %lx\n", MCU_VIDEO_BS_BUF_REMAINDER, REG_MCU_DCR01);
        }

        // Always stop data sending and parser after pre-decoding action.
        // Otherwise, next servo or parser action maybe fail.
        SrcFilter_Stop(__dwSFStreamID);
        HAL_ControlParser(HAL_PARSER_STOP, 0);

        if ( (__bModeCmd == KEY_SCF) || (__bModeCmd == KEY_SCB ) )
        {
            // ** TCH1.55-1; Set __bScan as SCF/SCB + KEY_NEXT/KEY_PREV will go here directly.
            // Here issue COMMAND_PLAY mode, suppose it quits from SCAN mode.
            // It may cause the 2 secs delay when call CC_KeyCommand (KEY_COMMAND) after this function
            __bScan= SCAN_NONE;
        }
    }

// LLY0.72, remove it since nobody to reference return value.
// Notice: must reference __bAttrPlay not __bAttrPlayNew
#if 0
    if( __bAttrPlayNew == ATTR_CDDA )
        return FALSE;
    else
        return TRUE;
#endif  // #if 0
}




//  *****************************************************************************************
//  Function    :   UTL_TriggerShowTime
//  Description :   Handle trigger show OSD/ PANEL timer info.
//  Arguments   :   NULL
//  Return      :   NULL
//  *****************************************************************************************
BYTE UTL_TriggerShowTime (void)
{
    if ( (OS_GetSysTimer () > _dwTriggerShowTime) )  // ** TCH0.99-1;
    {
        // LLY0.75, always record current STC time when playing mode (avoid pause mode to record), and
        // [1] NPF only for DVD-Title
        //     Because, the STC value is not updated correctly by DSP while SCAN mode.
        // [2] for CDROM A/V file playback. Otherwise, the time is not updated for Fast mode
        if( (__btPlaying) &&
            ( ((__wDiscType & BOOK_DVD) && (__bModeCmd==KEY_PLAY)) ||
                (__bAttrPlay&TYPE_CDROM_AV) ) )
//        if ( (__bModeCmd==KEY_PLAY) && (__btPlaying) )  // avoid PAUSE to record.
        {
            HAL_ReadInfo(HAL_INFO_STC, &__dwSTCValue);
//            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "STC:%lx\n", __dwSTCValue);
        }
        // Micky0.90, only query time playing mode.
        // due to UTL_TriggerShowTime will be called within main loop
        // but if no disc in drive, call SrcFilter_GetCurrentPosition will return cmd_ack_err.
        // shoule always set __bServoAutoCtrl=1 when open or power on?
        // currently, just avoid calling UTL_GetCurrentTime .
        if (__btPlaying || __btPause)
        {
            UTL_GetCurrentTime ();      // ** TCH0.99-1;
        }

        return UTL_OutputShowTime ();  // The function will call OSD/PANEL_TRIGGER
    }   // if ( > _dwTriggerShowTime )

    return  FALSE;
}



// LLY0.65 create ...
//  *****************************************************************************************
//  Function    :   _UTL_CalTime_FromFrameCnt
//  Description :   Calculate MP3/ MP2/ WMA track playback time base on frame count
//                  Must, be in sector format
//  Argument    :   None
//  Return      :   (1) Return the desired time in sector format (1sec = 75 sectors)
//              :   (2) 0xFFFFFFFF, the value is wrong. So, don't pass to OSD display
//  Rule        :   sec = [frame count * sample/frame] / Audio frequence
//  *****************************************************************************************
DWORD _UTL_CalTime_FromFrameCnt(void)
{
    DWORD   dwFrameCnt;
    DWORD   dwhsgTime=0;  // time value in sector format

    // LLY1.01, WMA time calculation method is different from other CDROM audio only mode.
    if (__bAttrPlay==ATTR_WMA)
    {
        HAL_ReadAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, &dwFrameCnt); //total samples

        // Notice: readback value * 8 = real value
        dwFrameCnt <<= 3;

        if (__dwHALAudioFreq)
        {
            dwhsgTime= dwFrameCnt/__dwHALAudioFreq;     // seconds
        }
        else
        {
            return (0xFFFFFFFF);
        }
    }
    // LLY0.75, always go the same path to calculate the timer for audio only type
    // ex. MP3, MP2, AC3, DTS
    else // if( (__bAttrPlay==ATTR_MP3) || (__bAttrPlay==ATTR_MP2) )
    {
        // read audio frame count
        HAL_ReadAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, &dwFrameCnt);

        // DVD_170Micky, enhance MP3 display time method, incase frame count is reset by audio reset
        // accumulate frames count.
        if(dwFrameCnt >= _dwPreFrameCount)
        {
            _dwTotalFrameCount+=(dwFrameCnt-_dwPreFrameCount);
        }
        else    // frame count is decrease.
        {
            _dwTotalFrameCount+=dwFrameCnt;  // frame count is reset
        }

        // Keep previous frame count
        _dwPreFrameCount=dwFrameCnt;

        // Update current frame count till now.
        dwFrameCnt=_dwTotalFrameCount;

        // DVD_171Micky, reference __dwHALAudioFreq, __wHALSamplesPerFrame
        // time(s) = [frames * (samples/frame)]/44100.
        dwhsgTime = dwFrameCnt * __wHALSamplesPerFrame; // total samples.

        if (__dwHALAudioFreq)
        {
            dwhsgTime /= __dwHALAudioFreq;     // seconds
        }
        else
        {
            // LLY0.65, always 0xFFFFFFFF if __dwHALAudioFreq is zero (audio frequency not ready)
            // Otherwise, OSD will display a large time value temporally.
            return (0xFFFFFFFF);
        }

    }

    // LLY0.75, calculate the file remain time
    if(__bShowTimeType==MSG_REMAIN_RELTIME)
        dwhsgTime = __dwFileTime - dwhsgTime;

    // Convert the time info. from sec to hsg format (sector)
    dwhsgTime *=75L;     // sector count.

    return (dwhsgTime);
}


#ifndef NO_DISC_MODE
// LLY0.65 create ...
//  *****************************************************************************************
//  Function    :   _UTL_CalTime_FromSector
//  Description :   Calculate the desired time from parser reported sector ID
//                  ex. MPEG I motion, CDDA
//  Argument    :   None
//  Return      :   (1) Return the desired time in sector format (1sec = 75 sectors)
//              :   (2) 0xFFFFFFFF, the value is wrong. So, don't pass to OSD display
//  Rule        :   sec = sector / 75
//  *****************************************************************************************
DWORD _UTL_CalTime_FromSector(void)
{
    extern DWORD   __dwEntryStart;

    DWORD   dwhsgTime=0;  // time value in sector format
    DWORD   dwTemp;

    UTL_GetCurrentTime(); //kevin1.07

    // Base on OSD show time type to calculate the desired time
    switch(__bShowTimeType)
    {
    case    MSG_REMAIN_ABSTIME:
        // calculate the absolute(disc) remain time
        dwhsgTime = (__dwTimeDiscEnd- __dwTimeNow + 1);

        // Plus av remainder to get the real time
        // Notice: only normal play need to do this
        // LLY0.85a, need to plus A/V remainder
        // since current F/W will minus A/V remainder value while scan mode
        //if (__bModeCmd == KEY_PLAY)
        {
            dwhsgTime += UTL_EstimateBufferTime();
        }
        break;

    case    MSG_REMAIN_RELTIME:
        // calculate the relative(track) remain time
        dwhsgTime = (__dwTimeEnd- __dwTimeNow + 1);

        // Plus av remainder to get the real time
        // Notice: only normal play need to do this
        // LLY0.85a, need to plus A/V remainder
        // since current F/W will minus A/V remainder value while scan mode
        //if (__bModeCmd == KEY_PLAY)
        {
            dwhsgTime += UTL_EstimateBufferTime();
        }
        break;

    case    MSG_ABSTIME:
        // calculate the absolute(disc) time
        dwhsgTime = (__dwTimeNow-__dwTimeDiscBegin + 1);

        // Minus av remainder to get the real time
        // Notice: only normal play need to do this
        // LLY0.85a, need to minus A/V remainder
        // since current F/W will minus A/V remainder value while scan mode
        //if (__bModeCmd == KEY_PLAY)
        {
            dwTemp = UTL_EstimateBufferTime();

            // DVD_101Micky, Fix CD-DA next, display time will show 1:09:10
            // the 1st UTL_OutputShowTime is called from ser_ata.c
            // at that time __dwTimeNow still not update???
            // add protection not become minus value
            if(dwhsgTime > dwTemp)
            {
                dwhsgTime -= dwTemp ;
            }

        }
        break;

    case    MSG_RELTIME:        // relative(track) time
    default:
        // for entry
        // must show time relative to the track begin
        if ( (__wPlayItem > 99) && (__wPlayItem <= 599) && !(__wDiscType & CDROM_M1) )
        {
            // __dwEntryStart is the track starting time
            dwhsgTime = (__dwTimeNow-__dwEntryStart + 1);
        }
        else
        {
            dwhsgTime = (__dwTimeNow-__dwTimeBegin + 1);
        }

        // Minus av remainder to get the real time
        // Notice, only normal play need to do this
        //LJY1.07, fix step/slow-> play, time minus 1 sec
        // for all mode, need calculate the time according to buffer remainder
        /*
        // TCC1.00, only normal play need to do this
        if (__bModeCmd == KEY_PLAY)
        */
        // LLY0.85a, need to minus A/V remainder
        // since current F/W will minus A/V remainder value while scan mode
        //if ( (__bModeCmd != KEY_SCF) && (__bModeCmd != KEY_SCB) )
        {
            dwTemp = UTL_EstimateBufferTime();

            // DVD_101Micky, Fix CD-DA next, display time will show 1:09:10
            // the 1st UTL_OutputShowTime is called from ser_ata.c
            // at that time __dwTimeNow still not update???
            // add protection not become minus value
            if( dwhsgTime > dwTemp)
            {
                dwhsgTime -= dwTemp ;
//                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Begin: %lx, Now: %lx, Buff: %lx\n", __dwTimeBegin, __dwTimeNow, dwTemp);
            }
        }
        break;

    }

    return (dwhsgTime);

}


// LLY0.67 create ...
//  *****************************************************************************************
//  Function    :   _UTL_CalTime_FromSTC
//  Description :   Calculate DVD/ VOB file playback time base on STC and cell time
//                  Must, be in sector format
//  Argument    :   None
//  Return      :   (1) Return the desired time in sector format (1sec = 75 sectors)
//              :   (2) 0xFFFFFFFF, the value is wrong. So, don't pass to OSD display
//  Notice      :   (1) 1st SCR value is encode within bitstream (cell)
//                  (2) STC value will be updated by decoder while do A/V sync.
//                  (3 )STC value will go 90K after 1sec
//  Rule        :   sec = (STC value - 1stSCR)/90K
//  *****************************************************************************************
//#define DEBUG_STC_TIME
DWORD _UTL_CalTime_FromSTC(void)
{
    extern  WORD    __wTimeCurrentDisplay;  // ** TCH1.61a;

    DWORD   dwhsgTime=0;  // time value in sector format
    DWORD   dwTemp;

    // Step 1: Process DVD title time information.
    // Micky0.90a, fix cell still display time will show 0. Only for forever still
    if (__bStillTime == 0xff)
    {
        // Micky0.87, fix DVD still menu+Display2, IR reaction is slow
        // as GxA will disable interrupt.
        // must update the update gap.
        _dwTriggerShowTime= OS_GetSysTimer ()+ _dwGap_TriggerShowTime;

        // DVD_102Micky, only CELL/PGC still display time as 0
        OSD_TRIGGER_MACRO(OSD_TRIGGER_TIME, 0);
#ifdef  DEBUG_STC_TIME
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "cell still\n");
#endif  // #ifdef  DEBUG_STC_TIME
        return (0xFFFFFFFF);
    }

    // DVD_102Micky, fix VOBU still time is displayed as 0
    if ( (__bModeCmd==KEY_PLAY) && !CELL_QueryStatus(CELL_VOBU_STILL_ON)
        // DVD_160Micky, when cell still,  the time will display as 0
        // for NF-002, cell control test.(1st cell play end will stay 5 seconds then play next cell)
        // show cell time instead.
        // but for spirit of the rainforest, play-> menu, the display time will not update as 0 immediately
        // still need think more
        /*
         && !CELL_QueryStatus(CELL_IS_STILL)
        */

        // DVD_171Micky, keep time update when cell skip VOBU
        // also need modify CELL_QueryTime
        && (__bCellState == CELL_NORMAL)
        )
    {
        // Since the STC maybe less than 1st SCR when press KEY_NEXT.
        // or, the STC maybe larger than 1st SCR when press KEY_PREV.
        // So, STC value can be reference only after __bSTCRunning=TRUE
        if( __bSTCRunning )
        {
            dwhsgTime =__dwSTCValue;
            // Micky0.90a, fix cell still display time will show 0. Only for forever still
            // if STC over VOBU_E_PTM, just use VOBU_E_PTM for display time
            if ( __bStillTime )
            {
                extern DWORD   __dwVOBU_E_PTM;
                if ( (__dwSTCValue + 30000) >= __dwVOBU_E_PTM )
                {
#ifdef  DEBUG_STC_TIME
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
                        "Use VOBU_E_PTM: %lx, Old time: \n", __dwVOBU_E_PTM-30000, dwhsgTime);
#endif  // #ifdef  DEBUG_STC_TIME
                    dwhsgTime = __dwVOBU_E_PTM-30000;
                }
            }

            // ** TCH1.03; begin...
            if ( dwhsgTime >= __dw1stSCR )
            {
                dwhsgTime -= __dw1stSCR;

                // LLY2.32, add protection while using CELL time
                // To avoid NV information is wrong while general plabyack mode
                // then OSD will become large --> small --> large
                HAL_ReadInfo(HAL_INFO_VIDEO_REMAIN, &dwTemp);
                if(dwTemp < 100)
                {
                    // DVD_160Micky, fix hang, don't update OSD time
                    // when the gap cell time and STC time over 2 seconds
                    // always show cell time
                    dwTemp=CELL_QueryTime();
                    // STC > cell time 2 seconds
                    if (dwhsgTime > ((dwTemp*90000L) + 180000))
                    {
#ifdef  DEBUG_STC_TIME
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
                            "Use cell time: %lx, Old time: %lx \n", dwTemp*90000L, dwhsgTime);
#endif  // #ifdef  DEBUG_STC_TIME
                        // use cell time as display time
                        dwhsgTime = dwTemp*90000L;
                    }
                }
            }
            else
            {
                //dwhsgTime= 0;
                // LLY2.53, give 0xFFFFFFFF since OSD time unit is expand from WORD to DWORD
                OSD_TRIGGER_MACRO(OSD_TRIGGER_TIME, 0xFFFFFFFF);
#ifdef  DEBUG_STC_TIME
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "less than 1st SCR (%lx<%lx) \n", dwhsgTime, __dw1stSCR);
#endif  // #ifdef  DEBUG_STC_TIME
                return (0xFFFFFFFF);
            }
            // ** TCH1.03; end...

            dwhsgTime /= 90000L;
        }
        else
        {
            // DVD_100Micky, fix GOTOTIME will show chapter begin time first
            // keep update time as 0 until new time position is searched
            // LLY2.53, give 0xFFFFFFFF since OSD time unit is expand from WORD to DWORD
            OSD_TRIGGER_MACRO(OSD_TRIGGER_TIME, 0xFFFFFFFF); // ** TCH1.03; Don't update time
#ifdef  DEBUG_STC_TIME
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "STC not running\n");
#endif  // #ifdef  DEBUG_STC_TIME
            return (0xFFFFFFFF);
        }
    }
    else
    {   // NON-NPF mode...
        // DVD_277Micky, fix Mustek title-> power on play-> play will show 01:10:06
        // __btPlaying=1, but __bSTCRunning=0, and __wTimeCellBegin is uninitialized.
        // __bCellState is unknown value.
        // DVDINFO will initial it for UTL.c's reference.
        if (__bCellState <=1)   // CELL_SERACH_1ST_NV or 0
        {
            // LLY2.53, give 0xFFFFFFFF since OSD time unit is expand from WORD to DWORD
            OSD_TRIGGER_MACRO(OSD_TRIGGER_TIME, 0xFFFFFFFF); // ** TCH1.03; Don't update time
#ifdef  DEBUG_STC_TIME
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Not update time since search NV\n");
#endif  // #ifdef  DEBUG_STC_TIME
            return (0xFFFFFFFF);
        }

        dwhsgTime = CELL_QueryTime();
        //DVD_051Micky
        // for SCF->play, then time will display as 0
        __dwSTCValue = (dwhsgTime*90000L)+__dw1stSCR;
    }
    __wTimeCurrentDisplay= (WORD) dwhsgTime; // ** TCH1.61a;


    switch(__bShowTimeType)
    {
    // add DVD title play time
    default:
    case    MSG_TITLE_TIME:
        // can't reference SCR directly,
        // as some chapter SCR always start with 0, must plus __wTimeChapterBegin.(Ritek)
        dwhsgTime += __wTimeCellBegin;
        break;

    // add DVD chapter play time
    case    MSG_CHAPTER_TIME:
        dwhsgTime += (__wTimeCellBegin-__wTimeChapterBegin);
        break;

    // add DVD chapter remain time
    case    MSG_REMAIN_CHAPTER_TIME:
        dwhsgTime += (__wTimeCellBegin-__wTimeChapterBegin);
        dwhsgTime = __wTimeChapter - dwhsgTime;
        break;

    // add DVD title remain time
    case    MSG_REMAIN_TITLE_TIME:
        dwhsgTime += __wTimeCellBegin;
        dwhsgTime = __wTimeTitle - dwhsgTime;
        break;

    }   // switch ( __bShowTimeType )


    // Convert the time info. from sec to hsg format (sector)
    dwhsgTime *= 75L;     // sector count.

    return (dwhsgTime);
}
#endif  // #ifndef NO_DISC_MODE


//  *****************************************************************************************
//  Function    :   UTL_OutputShowTime
//  Description :   Convert the time to absolute or relative time
//  Arguments   :   NULL
//  Return      :   FALSE   : time is not update and the state need to monitor
//  Side Effect :   will call OSD_TRIGGER_MACRO
//                  The time value will be dependent on __bShowTimeType
//                  Before call this API, make sure __dwTimeNow (Non-DVD) or
//                  __dwSTCValue (DVD) is updating.
//                  Otherwise, Call UTL_TriggerShowTime ();
//  ****************************************************************************************
BYTE UTL_OutputShowTime(void)
{

    BYTE    bTemp;  //Kevin1.25, port Wendy's code not to show time if current time > previous time for MP3 SCB

    // Notice: must check why to add volatile for wTime variable
    // Otherwise, the value will be 32 bits for it.
//    volatile WORD   wTime; // mm:ss format
    // LLY2.53, expand unit from WORD to DWORD unit to keep final OSD time
    // Since, DVD title time maybe > 4hr+15min+59sec (WORD unit max value)
    // [23:16]=Hour; [15:8]=Min; [7:0]=Sec
//    WORD    wTime;
    DWORD   dwRealTime;
    DWORD   dwTemp;
    DWORD   dwTime=0;

#ifdef  DEBUG_SHOWTIME
//    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "OutputShowTime: %lx\n", _dwTriggerShowTime);
#endif  // #ifdef DEBUG_SHOWTIME

    bTemp = TRUE; //Kevin1.25, port Wendy's code not to show time if current time > previous time for MP3 SCB

    // wyc1.23, because these two variables maybe change during error concealment
    // and OSD will become 0:0:0, so use __bModeCmd to replace it to enhance this problem.
    //if (__btPlaying || __btPause)
    if (__bModeCmd != KEY_STOP)
    {
        // TCC277, protect the time display. __dwTimeNow must be between __dwTimeBegin and __dwTimeEnd
        if (!(__wDiscType & (BOOK_CDROM|BOOK_DVD)))
        {
            if (__dwTimeNow < __dwTimeBegin || __dwTimeNow > __dwTimeEnd)
            {
#ifdef  DEBUG_SHOWTIME
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "return 1\n");
#endif  // #ifdef DEBUG_SHOWTIME

                return TRUE ;
            }
        }


        // Calculate CDROM title relative information.
        // DVD028_MP3/DVDTIME
        // wyc1.10-3, make DAT / MPG files OSD update method go VCD flow.
        if (__wDiscType & CDROM_M1)
        {
            // Audio Only bitstream, ex MP3, MP2, WMA
            // Calculate the time from frame count
            if(__bAttrPlay & TYPE_CDROM_AUDIO)
            {
                dwTime=_UTL_CalTime_FromFrameCnt();
            }

            // A/V motion bitstream, ex. VOB, MPG, DAT
            // Calculate the time from STC value
            else if(__bAttrPlay & TYPE_CDROM_AV)
            {
                //LJY1.26-2, don't update OSD time while pausing.
                if(__btPause)
                {
                    // LLY2.53, give 0xFFFFFFFF since OSD time unit is expand from WORD to DWORD
                    OSD_TRIGGER_MACRO(OSD_TRIGGER_TIME, 0xFFFFFFFF); // Don't update time
#ifdef  DEBUG_SHOWTIME
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "return 2\n");
#endif  // #ifdef DEBUG_SHOWTIME
                    return FALSE;
                }

                // LLY0.75, need to minus 1stSCR value !!
                if( __dwSTCValue >= __dw1stSCR )
                {
                    // Calculate the file elapsed time first (in sec format)
                    dwTime = (__dwSTCValue-__dw1stSCR)/90000;

                    // Don't update OSD time if file elapsed time > total file time
                    // LLY2.21a, only for total file time > 0 case
                    // ie. we will let OSD go base on STC while file time is wrong.
                    if((dwTime > __dwFileTime) && (__dwFileTime))
                    {
#ifdef  DEBUG_SHOWTIME
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "return 3\n");
#endif  // #ifdef DEBUG_SHOWTIME
                        return (FALSE);
                    }

#ifdef  SUPPORT_MP4
                    // LLY1.50, update chapter information while playback *.MP4
                    if(MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem)==EXTNAME_MP4)
                    {
                        __wChapter=PARSER_GetMP4Chapter(dwTime)+1;
                        //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "chap: %lx: time: %lx\n", __wChapter, dwTime);
                    }
#endif  // #ifdef SUPPORT_MP4

                    // LLY1.02, calculate final desired time in sector format
                    // Notice: don't using __dwLastSCR to calcalate file remain time
                    // Since, it's no meaning for DivX file.
                    // LLY2.32, only support remain time while total file time > 0
                    if( (__bShowTimeType == MSG_REMAIN_RELTIME) && (__dwFileTime > 0) ) // file remain time
                    {
                        dwTime = (__dwFileTime - dwTime)*75;
                    }
                    else    // file elapsed time
                    {
                        dwTime = dwTime*75;
                    }
                }
                else
                {
#ifdef  DEBUG_SHOWTIME
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Return 4\n");
#endif  // #ifdef  DEBUG_SHOWTIME
                    return (FALSE);
                }


                // LLY0.90, must remove this procedure
                // Otherwise, we can't see 00:00:00 while start playback.
#if 0
                // LLY.073, don't update time if the value is zero
                if(dwTime)
                {
                    return (FALSE);
                }
#endif          // #if 0
            }
            // LLY1.02, nothing need to do for other CDROM types.
            else
            {
#ifdef  DEBUG_SHOWTIME
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Return 5\n");
#endif  // #ifdef DEBUG_SHOWTIME
                return (FALSE);
            }
        }
#ifndef NO_DISC_MODE
        else if(__wDiscType & BOOK_DVD)
        {
            dwTime=_UTL_CalTime_FromSTC();
        }
        // LLY0.65, call API to calculate the MPEG I motion and CDDA playback time
        else
        {
            dwTime = _UTL_CalTime_FromSector();
        }
#endif  // #ifndef NO_DISC_MODE

        // LLY0.80a, nothing to display, if return value is 0xFFFFFFFF
        if(dwTime == 0xFFFFFFFF)
        {
#ifdef  DEBUG_SHOWTIME
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "return 6\n");
#endif  // #ifdef  DEBUG_SHOWTIME
            return (FALSE);
        }

        // Convert hsgTime format into sec format (1sec = 75 sector)
        dwTemp = dwTime/ 75;

        // LLY0.85, don't show the time while current time > previouse time for SCB mode
        // Notice: the problem only happen @ audio only case
        //         since the our F/W control it playback sometime (it may crose current time)
        // LLY1.01, don't update OSD time while video stream has been skip (same as audio only mode)
        // LLY2.75, need modify the algorithm to consider audio only SCB mode for abstime or remain time case.
        // [1] Abstime: new time must < previouse time
        // [2] remain time: new time must > previous time
        /*
        if ( ( dwTemp > _dwTimePrev ) && (__bModeCmd==KEY_SCB)
                 && ((__bModePlay == MODE_PLAYAUDIO) || (__bVideoID==HAL_THROW_VIDEO)) )
        {
#ifdef  DEBUG_SHOWTIME
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "return 7\n");
#endif  // #ifdef  DEBUG_SHOWTIME

            bTemp = FALSE;
        }
        */
        if ( (__bModeCmd==KEY_SCB) && ((__bModePlay == MODE_PLAYAUDIO) || (__bVideoID==HAL_THROW_VIDEO)) )
        {
            // [1] abstime: new time can't > previous time
            if( ( dwTemp > _dwTimePrev )  &&  ((__bShowTimeType==MSG_ABSTIME)) )
            {
#ifdef  DEBUG_SHOWTIME
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "return 7-1\n");
#endif  // #ifdef  DEBUG_SHOWTIME

                bTemp = FALSE;
            }
            // [2] remain time: new time can't < previous time
            else if( ( dwTemp < _dwTimePrev )  &&  ((__bShowTimeType==MSG_REMAIN_ABSTIME)||(__bShowTimeType==MSG_REMAIN_RELTIME)) )
            {
#ifdef  DEBUG_SHOWTIME
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "return 7-2\n");
#endif  // #ifdef  DEBUG_SHOWTIME
                if( _dwTimePrev != 0xfffffff )
                {
                    bTemp = FALSE;
                }
            }
        }

//        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "New: %ld, Old:%ld, Show: %ld \n", dwTemp, _dwTimePrev, bTemp);

#ifdef DOLBY_CERTIFICATION //kevin1.05, skip error bitstream too many seconds => shorten show time gap: virtually OK
        _dwGap_TriggerShowTime = COUNT_500_MSEC; //Joie Dolby
#endif  // #ifdef DOLBY_CERTIFICATION

        // Keep current time value if it's not equal to previous one
        if ( dwTemp != _dwTimePrev )
        {
            // LLY1.02, only keep current time while OSD want to update.
            // Otherwise, it may cause OSD time update the larger time while audio only SCB mode
            // ex. 123 --> 128 (skip not update, but keep in prev) --> 124 (< prev: 128, so update)
            if(bTemp)
            {
                _dwTimePrev= dwTemp;
            }
			_dwTriggerShowTime= OS_GetSysTimer()+ _dwGap_TriggerShowTime/2; //kevin1.07, fix SVCD display time loses some seconds e.g. no 4:48 -> 4:48 -> 4:50

#ifndef NO_DISC_MODE
            // ** TCH1.61a; Add here for acuurate DVD RepeatAB action
            if( (__wDiscType & BOOK_DVD) && (__bRepeatAB == REPEAT_AB) )
            {
                NV_Action ( ACTION_REPEATAB, 0xFF );
            }
#endif  // #ifndef NO_DISC_MODE

            _dwUTLNextTriggerCount= OS_GetSysTimer()+ COUNT_1_SEC; // ** TCH1.70-2;

        }
        else
        {
            _dwTriggerShowTime += (1+ (_dwGap_TriggerShowTime/ 10));

            if ( (OS_GetSysTimer() > _dwUTLNextTriggerCount) )
            {   // mean it's over 1 second and need to call OSD_Trigger
                _dwUTLNextTriggerCount= OS_GetSysTimer()+ COUNT_1_SEC; // ** TCH1.70-2;
            }
            else
            {
#ifdef  DEBUG_SHOWTIME
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "return 8\n");
#endif  // #ifdef DEBUG_SHOWTIME
                return  FALSE;
            }
        }


        // LLY2.53, split DVD and non-DVD time calculation into different path
        // Since, DVD title maybe > 4hr, and no frame count value
        // And, the old method only reserve BYTE unit for total min = hour*60 + min = 4hr + 15min + 59 sec
        if(__wDiscType & BOOK_DVD)
        {
            dwRealTime=MAKELONG( MAKEWORD((dwTemp%60), (dwTemp%3600)/60),
                                 MAKEWORD((dwTemp/3600), 0) );
        }
        else
        {
        // Convert hsgTime format into mm:ss:ff
        dwTemp= UTL_SectorstoMSF( dwTime );

            // Convert value into hh:mm:ss in BCD format
            dwRealTime= MAKELONG( MAKEWORD( MSF_SECOND(dwTemp), MSF_MINUTE(dwTemp)%60),
                                  MAKEWORD( MSF_MINUTE(dwTemp)/60, 0) );

//            wTime= MAKEWORD( MSF_SECOND(dwTemp), MSF_MINUTE(dwTemp));
        }


    }
    else
    {   // NON-Playing or NON-Pause
        // LLY2.53, OSD time unit is expand from WORD to DWORD
        //wTime=0;  // if not (__btPlaying || __btPause)
        dwRealTime=0;
        _dwTriggerShowTime= OS_GetSysTimer ()+ (COUNT_500_MSEC+COUNT_400_MSEC);
    }


    //
    if(bTemp) //Kevin1.25, port Wendy's code not to show time if current time > previous time for MP3 SCB
    {
//        HAL_ReadInfo(HAL_INFO_STC, &__dwTemp);
//        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "STC: %lx; 1stSCR: %lx; Time: %lx; \n", __dwTemp, __dw1stSCR, dwRealTime);
#ifdef  DEBUG_SHOWTIME
//        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Time: %d\n", wTime);
//        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Time: %lx\n", dwRealTime);
#endif  // #ifdef DEBUG_SHOWTIME

        // trigger OSD or PANEl to display time
        // LLY2.53, OSD time unit is expand from WORD to DWORD
        // OSD_TRIGGER_MACRO(OSD_TRIGGER_TIME, wTime);
        OSD_TRIGGER_MACRO(OSD_TRIGGER_TIME, dwRealTime);

    }

    return  TRUE;
}


/////////////////////////////////////////////////////
// protected functions
//  *********************************************************************
//  Function    :   UTL_ShowTrack
//  Description :       Trigger OSD or PANEL to show track number
//  Arguments   :       bMessage        : message for panel or OSD
//                      wPlayItem      : track number for display
//                      bSec            : the display time
//  Return      :       NULL
//  Side Effect :
//  *********************************************************************
void UTL_ShowTrack(BYTE bMessage, WORD wPlayTrack, BYTE bSec)
{
    // the __wPlayItem value will plus 1 for a VCD disc
    if ( (__wDiscType & BOOK_VCD) )
    {
        // Micky1.22, fix CDROM file # > 1000 will display as 1
        // Check wPlayTrack > 1000, set as 0 to let OSD display as --
        if (wPlayTrack > 999 )  // segment play item
            wPlayTrack = 0;
        else
            wPlayTrack--;
    }

    OSD_OUTPUT_MACRO ( bMessage, wPlayTrack, bSec );
}



//  *********************************************************************
//  Function    :   UTL_EstimateSectors // ** S-TCH;
//  Description :   Estimate the safe sector
//                  It depends on time and current title
//  Arguments   :   dwTime, specify the desired time value
//  Return      :   The sector NO.
//  Side Effect :
//  *********************************************************************
// LLY0.63, expand the parameter unit from WORD to DWORD
// Because, COUNT_XXX_SEC maybe larger than 65535
WORD UTL_EstimateSectors ( DWORD dwTime )
{
    BYTE    bTemp;
    WORD    wTemp;

    if( __wDiscType & BOOK_2X )
        bTemp = 150;
    else
        bTemp = 75;

    wTemp = (WORD)((bTemp * dwTime)/ COUNT_1_SEC);

    if (( __bModeCmd == KEY_SF )
        // wyc2.17-909S, add language study key checking.
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
            || ( __bModeCmd == KEY_LAN_STUDY)
#endif //
        )
    {
        // __wSlowSpeed is actual slow speed ratio
        wTemp /= (__wSlowSpeed );
    }

    return wTemp;
}


//  ******************************************************************
//  Function    :   UTL_Language_Study
//  Description :   The procedure to control language study action
//  Arguments   :   bOn, turn on/off language study
//                  bLevel, specify the desird level
//  Return      :   None
//  ******************************************************************
#define DEBUG_LANGUAGE_STUDY
#if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)
void UTL_Language_Study(BYTE bOn, BYTE bLevel)
{
    DWORD   dwTemp;
    DWORD   dwDownSampleCtl;

    // only for DVD/VCD only now
    if( !(__wDiscType & BOOK_DVD) &&  !(__wDiscType & BOOK_VCD) )
    {
        return;
    }

    // Read back LPCM down sample mode flag first
    HAL_ReadInfo(HAL_INFO_LPCM_DOWNSAMPLE, &dwDownSampleCtl);

    // Turn on language study mode
    if(bOn)
    {
#ifdef  DEBUG_LANGUAGE_STUDY
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "enable language study");
#endif  // #ifdef DEBUG_LANGUAGE_STUDY

        __bLangStudyFlag = TRUE;  //kevin1.10, fix SPDIF raw is on when language study. CHIPS_OpenAudio references this flag
        switch (bLevel)
        {
        //consider mode switches : Slow <->pause/SCF/repeatAB/repeat...
        case 1: // 75%
            // Give different key shift value for VCD and DVD
            if ( __wDiscType & BOOK_DVD)
            {
                HAL_WriteAM(HAL_AM_SPECIAL_KEYUP_MODE, 0xaaa);
            }
            else if ( __wDiscType & BOOK_VCD)
            {
                HAL_WriteAM(HAL_AM_SPECIAL_KEYUP_MODE, 0xba2);
            }

            // Give key shift parameter, and enable it.
            HAL_WriteAM(HAL_AM_KEYSHIFT, 7);
            HAL_WriteAM(HAL_AM_KEYSHIFT_EN, 1);

            // If LPCM 96KHz and not do downsample
            // just do 96K * 75% = 72K, --> set 64K
            HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &dwTemp);
            if( (__bAudioType == HAL_AUDIO_LPCM) &&
                (!dwDownSampleCtl) && (dwTemp == AUDIO_FREQ_96K) )
            {
                HAL_SetAudioDAC( AUDIO_FREQ_64K );
#ifdef  DEBUG_LANGUAGE_STUDY
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "64K");
#endif  // #ifdef DEBUG_LANGUAGE_STUDY
            }
            // Others, 48K *75% = 36K or 44.1K * 75% = 33.075K --> Set 32K
            else // 48K or 44.1K
            {
                HAL_SetAudioDAC( AUDIO_FREQ_32K );
#ifdef  DEBUG_LANGUAGE_STUDY
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "32K");
#endif  // #ifdef DEBUG_LANGUAGE_STUDY
            }

            break;

        case 2: // 50%
            // Give different key shift value for DVD and VCD
            if ( __wDiscType & BOOK_DVD)
            {
                HAL_WriteAM(HAL_AM_SPECIAL_KEYUP_MODE, 0x800);

            }
            else if ( __wDiscType & BOOK_VCD)
            {
                HAL_WriteAM(HAL_AM_SPECIAL_KEYUP_MODE, 0x8ba);
            }

            // Enable key shift control
            HAL_WriteAM(HAL_AM_KEYSHIFT, 7);
            HAL_WriteAM(HAL_AM_KEYSHIFT_EN, 1);

            // If LPCM 96KHz and not do downsample
            // just do 96K * 50% = 48K, --> set 48K
            HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &dwTemp);
            if( (__bAudioType == HAL_AUDIO_LPCM) &&
                 (!dwDownSampleCtl) && (dwTemp == AUDIO_FREQ_96K) )
            {
                HAL_SetAudioDAC( AUDIO_FREQ_48K );
#ifdef  DEBUG_LANGUAGE_STUDY
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "48K");
#endif  // #ifdef DEBUG_LANGUAGE_STUDY
            }
            // Others, 48K*50=24 or 44.1*50=22.1 --> Set 24K
            else // 48K or 44.1K
            {
                HAL_SetAudioDAC( AUDIO_FREQ_24K );
#ifdef  DEBUG_LANGUAGE_STUDY
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "24K");
#endif  // #ifdef DEBUG_LANGUAGE_STUDY
            }
            break;

        default:
            __bLangStudyFlag = FALSE;
            // J500KWShih_220, language study, set it to 0 for normal case
            HAL_WriteAM(HAL_AM_LSBUF_ADR, 0);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "switch case error %hx!", __bSlowLevel);
            break;
        }

        // J500KWShih_220, buffer address for language study
        // Notice: must set address before DSP start playback
#ifdef  DS_LANGUAGESTUDY_BUFF_ST
        if(__bLangStudyFlag)
        {
            HAL_WriteAM(HAL_AM_LSBUF_ADR, DS_LANGUAGESTUDY_BUFF_ST);
        }
        else
#endif  // #8fdef DS_LANGUAGESTUDY_BUFF_ST
        {
            HAL_WriteAM(HAL_AM_LSBUF_ADR, 0);
        }

        DISP_ForceOneField(TRUE);
        HAL_PlayCommand(COMMAND_PLAY, 0);
        CHIPS_OpenAudio(TRUE);  //otherwise slow->pause->pause => no sound
    }
    // Turn off Language study mode
    else
    {
#ifdef  DEBUG_LANGUAGE_STUDY
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "disable language study");
#endif  // #ifdef DEBUG_LANGUAGE_STUDY

        __bLangStudyFlag = FALSE;

        // J500KWShih_220, language study, set it to 0 for normal case
        HAL_WriteAM(HAL_AM_LSBUF_ADR, 0);

        if ( __wDiscType & BOOK_DVD)
        {
            HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &dwTemp);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "dwTemp2=%lx", dwTemp);
            if( (__bAudioType == HAL_AUDIO_LPCM) &&
                 (!dwDownSampleCtl) && (dwTemp == AUDIO_FREQ_96K) )
            {
                HAL_SetAudioDAC( AUDIO_FREQ_96K );
#ifdef  DEBUG_LANGUAGE_STUDY
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "96K");
#endif  // #ifdef DEBUG_LANGUAGE_STUDY
            }
            else //AUDIO_FREQ_48K
            {
                HAL_SetAudioDAC( AUDIO_FREQ_48K );
#ifdef  DEBUG_LANGUAGE_STUDY
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "48K");
#endif  // #ifdef DEBUG_LANGUAGE_STUDY
            }
        }
        else if ( __wDiscType & BOOK_VCD)
        {
            HAL_SetAudioDAC( AUDIO_FREQ_44K );

#ifdef  DEBUG_LANGUAGE_STUDY
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "48K");
#endif  // #ifdef DEBUG_LANGUAGE_STUDY
        }

        __cKeyShift = 0;
        HAL_WriteAM(HAL_AM_KEYSHIFT, 0);
        HAL_WriteAM(HAL_AM_KEYSHIFT_EN, 0);
        DISP_ForceOneField(FALSE);
    }

}
#endif  // #if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)


//  ******************************************************************
//  Function    :   UTL_TransferCommandID
//  Description :   Do the necessary while issue a playback command
//                  ex. set buffer threshold, restore A/V packet ID
//  Arguments   :   bKey, the desired command key
//  Return      :   None
//  ******************************************************************
// LLY0.64, remove the return value since nobody reference it.
//BYTE    UTL_TransferCommandID(BYTE bKey)
void UTL_TransferCommandID(BYTE bKey)
{
    BYTE    bCommand;
    WORD    wRISCArg;

    wRISCArg=0;
    _dwGap_TriggerShowTime= COUNT_500_MSEC+ COUNT_200_MSEC;       // ** TCH0.99-1;

    // LLY0.70, restore current audio/video ID while non - SCAN mode
    // since A/V sync. issue.
    if( (__bModePlay & MODE_PLAYMOTION) && ((bKey!=KEY_SCF) || (bKey!=KEY_SCB)) )
    {
        // Notice: reference global variable : __bVideoID for video ID -- LLY0.85
        // Since, F/W will set it as HAL_THROW_VIDEO while not support DivX format
        HAL_ControlParser(HAL_PARSER_VIDEO_ID, __bVideoID); // 0
        HAL_ControlParser(HAL_PARSER_AUDIO_ID, __bASTID );
    }

    switch(bKey)
    {
    case    KEY_PLAY:
        bCommand=COMMAND_PLAY;
        break;

    case    KEY_FF:
        _dwGap_TriggerShowTime= COUNT_300_MSEC;       // ** TCH0.99-2;
        bCommand=COMMAND_FAST;
        wRISCArg=FAST_FACTOR;
        break;

    case    KEY_STEP:
        bCommand=COMMAND_STEP;
        break;

    case    KEY_SF:
#if (SLOW_MODE_CONFIG == LANGUAGE_STUDY_MODE)
        // Do language study action if enable language study mode only
        UTL_Language_Study(TRUE, __bSlowLevel);
        // Notice: don't use break since it's will issue NPF command
        return;
#else   // #if (SLOW_MODE_CONFIG == LANGUAGE_STUDY_MODE)
        //  Do slow mode if enable normal slow mode only or language study + slow mode
        bCommand=COMMAND_SLOW;
        wRISCArg = __wSlowSpeed;
        break;
#endif  // #if (SLOW_MODE_CONFIG == LANGUAGE_STUDY_MODE)


    // wyc2.17-909S, add language study case.
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
    case    KEY_LAN_STUDY:
        printf("Langage Study [1]\n");
        UTL_Language_Study(TRUE, __bSlowLevel);
        // Notice: don't use break since it's will issue NPF command
        return;
#endif // #if (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)

    case    KEY_SCF:
    case    KEY_SCB:
        bCommand=COMMAND_SCAN;

        // LLY0.70, reset audio to clear audio buffer data, and throw the audio stream
        // Otherwise, audio buufer maybe fullness to let Scan mode hang
        // LLY0.96, don't issue this action for audio only bitstream
        // Otherwise, there will be no audio output while prepare to play --> SCAN immediately
        // LLY1.01, don't reset audio and skip audio while video stream has been skip (same as audio only mode)
        if( (__bModePlay != MODE_PLAYAUDIO) && (__bVideoID!=HAL_THROW_VIDEO) )
        {
            HAL_Reset(HAL_RESET_AUDIO);
            HAL_ControlParser(HAL_PARSER_AUDIO_ID, HAL_THROW_AUDIO);
        }
        break;

    case    KEY_PAUSE:
        bCommand=COMMAND_PAUSE;
        break;

    case    KEY_STOP:
        bCommand=COMMAND_STOP;
        break;

    default:
        return;
    }

    // wyc2.17-909S, update code to support language study key.
#if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)
    if( (__bLangStudyFlag==TRUE)
#if (SLOW_MODE_CONFIG == LANGUAGE_STUDY_MODE)
         && (bKey!=KEY_SF)
#elif (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
         && (bKey!=KEY_LAN_STUDY)
#endif //
      )
    {
        UTL_Language_Study(FALSE, 0);
    }
#endif

    // LLY2.77, porting CoCo's code : don't issue playback command while photobrowse mode
    // Fix bug 0002995: press key_next under BROWSE MODE, musci will play for a while
#ifdef SUPPORT_PHOTO_BROWSE_MODE
    if (__bMMPhotoBrowseMode && bCommand == KEY_PLAY)
    {
        return;
    }
#endif //SUPPORT_PHOTO_BROWSE_MODE

    // only change the slow speed argument
    HAL_PlayCommand(bCommand, (BYTE)wRISCArg);
    return ;
}


//  **********************************************************************************************
//  Function    :   UTL_QueryCapability
//  Description :   The general API for user to query some info. or current solution setting.
//  Arguments   :   bKind, specify the desired mode user want to query
//  Return      :   Non-zero, Current solution setting or just report report support the feature
//                  FALSE(zero), NOT support the feature.
//  Side Effect :
//  **********************************************************************************************
// LLY0.62, must check F/W key and IR key together
// Because, it's same important for F/W and IR key
// LLY2.38, expand parameter unit from BYTE to WORD
//BYTE    UTL_QueryCapability ( BYTE bKind )
// LLY2.60, expand this API meaning to query current solution desired mode setting.
// So, expand the return value from BYTE to DWORD
//BYTE UTL_QueryCapability(WORD wKind)
DWORD UTL_QueryCapability(WORD wKind)
{
    switch ( wKind )
    {
    // Micky0.96, add a query for parental
    case    QUERY_SUPPORT_PASSWORD_QUERY:
#ifdef	SUPPORT_PASSWORD_QUERY
        return TRUE;
#else   // #ifdef	SUPPORT_PASSWORD_QUERY
        return FALSE;
#endif  // #ifdef	SUPPORT_PASSWORD_QUERY

    // DVD_275b+, SUPPORT_VIRTUAL_DTS
    case    QUERY_VIRTUAL_DTS:
#ifdef  SUPPORT_VIRTUAL_DTS
        return TRUE;
#else   // #ifdef SUPPORT_VIRTUAL_DTS
        return FALSE;
#endif  //#ifdef  SUPPORT_VIRTUAL_DTS

    // DVD_172Micky, add two more querys for checking abort.
    // for the consistency between main flow and other functions.
    case    QUERY_ABORT_SERVO:  // query if the servo function will be aborted
        // Micky1.22, KEY_POWER can abort any..
        if (__bISRKey == KEY_POWER)
            return TRUE;
        // Micky1.22
        // if don't check recover stage, Ritek Golden man-> key next(invalid) will hang.
        // only allow to abort when recover stage >=2.
        // wyc0.95, rename to DWORD.
        if (__dwMONRecoverStage < 2)
            return FALSE;
        if (__bISRKey == KEY_OPEN_CLOSE || __bFWKey == KEY_OPEN_CLOSE)
            return TRUE;
        if (__bISRKey == KEY_NEXT || __bFWKey == KEY_NEXT)
            return TRUE;
        if (__bISRKey == KEY_PREV || __bFWKey == KEY_PREV)
            return TRUE;
        return FALSE;

    case    QUERY_ABORT_MAINFLOW:   // query if the mainflow will be aborted
        if (__bISRKey == KEY_POWER || __bFWKey == KEY_POWER)
            return TRUE;
        if (__bISRKey == KEY_OPEN_CLOSE || __bFWKey == KEY_OPEN_CLOSE)
            return TRUE;
        return FALSE;

    case    QUERY_STRICTUOP:
#ifdef  STRICT_UOP
        return TRUE;
#else   // #ifdef STRICT_UOP
        return FALSE;
#endif  // #ifdef STRICT_UOP

    case    QUERY_KEYSET_STOP:  // ** TCH1.60a;
        if ( (__bISRKey == KEY_STOP) ||  (__bISRKey == KEY_OPEN_CLOSE)
            || (__bFWKey == KEY_STOP) || (__bFWKey == KEY_OPEN_CLOSE) )
        {
#ifdef  _DEBUG_INFO
            OSD_Output ( MSG_VERSION, 101, 1 );
#endif  // #ifdef _DEBUG_INFO

            return TRUE;
        }

        return FALSE;

    // Micky1.24, add a query for support anti-shock
    case    QUERY_SUPPORT_ANTISHOCK:
#ifdef SUPPORT_ANTISHOCK
        return TRUE;
#else   // #ifdef SUPPORT_ANTISHOCK
        return FALSE;
#endif  // #ifdef SUPPORT_ANTISHOCK


    // LLY1.10, add new query for checking valid motion file, ie. A/V all valid
    // Otherwise, only playback Audio or Video only; or skip the file if A/V all non-valid
    // Notice: this API can be called after the desired file has been pre-scan done.
    case    QUERY_VALID_MOTION:
        if( (__bVideoID==HAL_THROW_VIDEO) && (__bASTID==HAL_THROW_AUDIO) )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
        break;

    //Aron2.31-909R, add for abort nav control
    case    QUERY_ABORT_NAVCTRL:
        if (__bISRKey == KEY_POWER || __bFWKey == KEY_POWER)
            return TRUE;
        if (__bISRKey == KEY_OPEN_CLOSE || __bFWKey == KEY_OPEN_CLOSE)
            return TRUE;
        if(( __bISRKey == KEY_TITLE ||__bISRKey == KEY_MENU
             ||__bISRKey == KEY_PREV||__bISRKey == KEY_NEXT
             ||__bISRKey == KEY_PBC||__bISRKey == KEY_DIGEST) )
        {
            return TRUE;
        }
        return FALSE;
        break;

    //Aron2.38, add for checking support still resuming
    case QUERY_SUPPORT_STILLRSM:
#ifdef SUPPORT_STILLRSM
        return TRUE;
#else
        return FALSE;
#endif
        break;

    // LLY2.60 begin ...
    // The section for query current solution setting.
    // [1] Aspect Ratio setting: LB/ PS/ WIDE
    case    QUERY_ASPECT_RATIO_SETTING:
        switch(__SetupInfo.bAspectRatio)
        {
        case SETUP_SCREEN_TV_TYPE_LB :
            return  (ASPECT_RATIO_MODE_LB);

        case SETUP_SCREEN_TV_TYPE_PS :
            return  (ASPECT_RATIO_MODE_PS);

        case SETUP_SCREEN_TV_TYPE_WIDE:
        default:
            return  (ASPECT_RATIO_MODE_WIDE);
        }
        break;

    // [2] Digital audio output setting: SPDIF-RAW/ SPDIF-PCM/ Off
    case    QUERY_DIGITAL_AUDIO_OUTPUT_SETTING:
        switch(__SetupInfo.bAudioDigitalOut)
        {
        case    SETUP_AUDIO_DIGITAL_OUT_SPDIF_RAW:
            return  (DIGITAL_AUDIO_OUTPUT_SPDIF_RAW);

        case    SETUP_AUDIO_DIGITAL_OUT_SPDIF_PCM:
            return  (DIGITAL_AUDIO_OUTPUT_SPDIF_PCM);

        case    SETUP_AUDIO_DIGITAL_OUT_OFF:
        default:
            return  (DIGITAL_AUDIO_OUTPUT_OFF);
        }
        break;

    // [3] Parental control level: 1~8
    case    QUERY_PARENTAL_CONTROL_LEVEL_SETTING:
        return (__SetupInfo.bParentalCtl - SETUP_CUSTOM_PC_LEVEL_BASE + 1);

    // [4] Progressive Scan setting: on/ off
    case    QUERY_PSCAN_SETTING:
        switch(__SetupInfo.bProgressiveScan)
        {
        case    SETUP_PROGRESSIVE_SCAN_ON:
            return  (PSCAN_MODE_ON);

        case    SETUP_PROGRESSIVE_SCAN_OFF:
        default:
            return  (PSCAN_MODE_OFF);
        }
        break;
    // LLY2.60 end ...

    default:
        break;
    }

    // Always return FALSE for non-defined case
    return  FALSE;
}


// wyc.170-2, the function to set the de-emphasis.
BYTE UTL_SetCDDADeEmphasis(void)
{
#ifdef SUPPORT_AUDIO_DEEMPHASYS
    BYTE    bTemp;

    // wyc.278, read the de-emphasis information from SERVO then do the related DAC setting.
    bTemp = SrcFilter_AudioDeemphasis(__wPlayItem);
    __bHALDeEmphasis = FALSE;
    if (bTemp == SERVO_CDDA_DEEMPHASIS)
    {
        __bHALDeEmphasis = TRUE;
    }
    else if (bTemp == SERVO_CDDA_NON_DEEMPHASIS)
    {
        ;
    }
    else
    {
        return FALSE;
    }
    HAL_SetAudioDAC(AUDIO_FREQ_44K); // need to check how to control the frequency.
#endif // SUPPORT_AUDIO_DEEMPHASYS
    return TRUE;
}


// LLY1.06, modify the procedure to meet current requirement
//  *************************************************************************************************
//  Function    :   UTL_PreStartServo
//  Description :   Do servo pre-spinning up servo
//  Arguments   :   dwPosition  : the position that want to spin-up the servo
//                  bAction, specify servo continue action after seek command.
//                   [1] ACTION_CONTINUE: continue the previous data reading range
//                       ie. F/W don't issue the range again, ex pause/ step --> play
//                   [2] ACTION_SKIP: skip the previous data reading range,
//                       ie. F/W will issue new range, ex GoTime, Resume action
//  Return      :   NONE
//  Side Effect :   It will take more time for servo stop-> spinup.
//  *************************************************************************************************
void UTL_PreStartServo(DWORD dwPosition, BYTE bAction)
{
    // LLY1.00, get servo status first
    // If it not @ stop status, don't issue seek action.
    // If it @ stop status, need to issue seek action.
    // wyc1.06, use power down control flag to decide if need to do seek command,
    // only do seek command to wake up source when source is power down mode.
    if (__bSFSourcePWDown == FALSE)
        return;

    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Servo in stop state\n");

    // LLY0.91, minus a tolerence range, ex 30 sector
    // Then, it can reduce servo next reading action.
    if(dwPosition >= 30)
        dwPosition -= 30;

    //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Seek: %lx\n", dwPosition);

    // LLY1.06, it's un-necessary to stop servo and parser first before issue seek command
    // Because, current flow will check if servo @ stop status first
    // Notice: in old code, don't check servo stop status. So, we need to stop before seek command
    //         To avoid servo receive the seek command during reading time.
#if 0
    // LLY0.91, must stop servo before issus servo seek command.
    // To avoid Servo Reading time to receive seek command.
    // Notice: Must stop parser together.
    SrcFilter_Stop();
    HAL_ControlParser(HAL_PARSER_STOP, 0);
#endif  // #if 0

    // LLY1.06, base on desired action to issue servo seek command
    if(bAction==ACTION_CONTINUE)
    {
        SrcFilter_Seek(dwPosition, TRUE);
    }
    else
    {
        SrcFilter_Seek(dwPosition, FALSE);
    }

}



//LJY1.24a, add an API for OSD time or resume compensation
//  ******************************************************************
//  Function    :   UTL_EstimateBufferTime
//  Description :   For OSD time or resume compensation
//  Arguments   :   none
//  Return      :   none
//  Side Effect :
//  ******************************************************************
DWORD UTL_EstimateBufferTime(void)
{
    DWORD   dwVRem, dwARem, dwTotal;

    HAL_ReadInfo (HAL_INFO_VIDEO_REMAIN, &dwVRem) ;
    HAL_ReadInfo (HAL_INFO_AUDIO_REMAIN, &dwARem) ;

    //Alan1.10, fix the bug when linear-playing track #6 to #7, the OSD time is wrong
    if (ATTR_CDDA == __bAttrPlay || TYPE_CDROM_AUDIO & __bAttrPlay)
        dwVRem = 0;

    dwTotal = dwVRem + dwARem;

    if(__bCDDA_DTS == CDDA_PURE)
    {
        dwTotal /= SECTOR_CDDA;
    }
    else if(__bCDDA_DTS == CDDA_CDG)
    {
        // LJY0.95, let it same as CDDA
        dwTotal /= SECTOR_CDDA; // with subcode
    }
    else
    {
        dwTotal /= SECTOR_581; //LJY1.24a,  for VCD/SVCD, 2324/4 will be more precisely for time calculation.
    }
    return dwTotal; //buffer time to be compensated
}




//  **************************************************************************
//  Function    :   UTL_GetCDDAFormat
//  Description :   Send bitstream to frame buffer to detect CD format
//  Arguments   :   bStage, specify the stage of getting the CDDA format
//                  UTL_GETCDDAFORMAT_INITIAL, initial stage for reading disc
//                  UTL_GETCDDAFORMAT_RUNTIME, runtime stage for playing new track
//  Return      :   None
//  **************************************************************************
void UTL_GetCDDAFormat(BYTE bStage)
{
    //kevin0.76, send ~3 seconds of sectors to buffer for CDDA detection
    if (bStage==UTL_GETCDDAFORMAT_INITIAL)
    {
        DWORD   dwTimer;
        DWORD   dwSrc;
        DWORD   dwDst;
        DWORD   dwIndx;
        ULONGLONG_UNION u64Temp0, u64Temp1;

        //reference INFOFILTER_DumpSectors()

#ifdef SUPPORT_PRINTF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DS_CD_DETECT_BUF_ST=%lx", DS_CD_DETECT_BUF_ST);
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DS_CD_DETECT_BUF_END=%lx", DS_CD_DETECT_BUF_END);
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DS_CD_DETECT_BUF_SECTORS=%lx", DS_CD_DETECT_BUF_SECTORS);
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DS_CD_DETECT_BUF_END_NONCDG=%lx", DS_CD_DETECT_BUF_END_NONCDG);
#endif

        HAL_PlayCommand(COMMAND_STOP,0);
        HAL_ControlParser(HAL_PARSER_STOP, 0);
        // wyc0.72, need to stop source when want to dump sectors.
        // Otherwise maybe source won't execute new read command because of still doing last time reading.
        SrcFilter_Stop(__dwSFStreamID);

        PARSER_SetDataFmt(0, 0,PARSER_BSFMT_DATA_ONLY | PARSER_BSFMT_CDDA_SUBCODE); //kevin0.91
        PARSER_ActionFilter( 0, PARSER_ENABLE_DATA, TRUE );

        // LLY2.22, call general API to config CDG title detection related buffer.
        UTL_Config_BufferMode(DUMP_BUFF_CDG_DETECT_MODE);
#if 0
        // LLY0.81, call SrcFilter_ConfigBuffer() to set parser and servo dump-in buffer
        // LLY2.20, must config it as MM mode, since parser A/V dump out buffer is MM mode
        SrcFilter_ConfigBuffer( 0, DS_INPUT_BITBUF_ST_MM,
            ((DS_INPUT_BITBUF_END_MM-DS_INPUT_BITBUF_ST_MM)/SERVO_BUFSIZE_ALIGN_UNIT) );
        //PARSER_SetBSBufferAddress(0,DS_INPUT_BITBUF_ST, DS_INPUT_BITBUF_END);
        PARSER_SetOutBufferAddress(PARSER_ES_DATA, DS_CD_DETECT_BUF_ST, DS_CD_DETECT_BUF_END);

        // LLY2.11, must set A/V dump-out buffer for parser, since it will dump sub code to video buffer
        // Otherwise, the CDG detection maybe fail
        PARSER_SetOutBufferAddress(PARSER_ES_VIDEO, DS_VDBUF_ST_MM, DS_VDBUF_END_MM);
        PARSER_SetOutBufferAddress(PARSER_ES_AUDIO, DS_AD0BUF_ST_MM, DS_AD0BUF_END_MM);
#endif  // #if 0
        PARSER_BufferReset(PARSER_ES_DATA);

        PARSER_Command(0, PARSER_CMD_START, 0);

        //kevin1.02, SERVO_ReadData will check __bAttrPlay & __bCDDA_DTS to send SUBCHANNEL_ENABLE command to SERVO
        __bAttrPlay = ATTR_CDDA;
        __bCDDA_DTS = CDDA_CDG;

        //Kevin1.24-3, In the beginning (0~70 sectors=40KDW=buffer size) of Track 1 & Track 2 of Hiteker 9-track CD-DTS, no DTS frame sync is found
        SrcFilter_ReadSectors(__dwSFStreamID, 35, 35+DS_CD_DETECT_BUF_SECTORS-1);
        __bCDDA_DTS = CDDA_UNKNOWN;
        __bAttrPlay = ATTR_NONE;


        dwTimer = OS_GetSysTimer();
        OS_REORDER_BARRIER();
        // LLY2.55, porting Winnie's code to enlarge the time-out value
        // Ex, one CD-G title is not close disc & has data @ outer area.
        // So servo will read inner to outer area during title detection.
        // For this case, we want to read inner area data again to identify CDG/ CD-DTS/ CDDA format
        // Thus, we need add extra time (3 sec) for servo long jump action(outer--> inner)
        //while ((OS_GetSysTimer() - dwTimer) < COUNT_2_SEC)
        while( (OS_GetSysTimer() - dwTimer) < (COUNT_2_SEC + COUNT_3_SEC) )
        {
            if (PARSER_QueryEvent(0,PARSER_EVENT_COMPLETED,TRUE))
            {
                break;
            }

            //release CPU resource to other threads
            OS_YieldThread();
        }

        PLAT_PROC1_CACHE_FLUSH;
        SrcFilter_Stop(__dwSFStreamID);

        // wyc2.17b-909S, config CDG buffer
        INFOFILTER_CFGCDGDetectBuff(DS_CD_DETECT_BUF_ST,DS_CD_DETECT_BUF_END,DS_CD_DETECT_BUF_END_NONCDG);
        // 2352B data + 96B subcode + 2352B data + 96B subcode + ...
        // wyc2.38-909s, don't check other type when CDG disc is recognize OK.
        if( INFOFILTER_Detect_CDG() )
            __bCDDA_DTS = CDDA_CDG;
        else
        {
            // => 2352B data + 2352B data + ...
            for ( dwSrc=dwDst=DS_CD_DETECT_BUF_ST;
                  dwSrc<DS_CD_DETECT_BUF_END-2448;
                  dwSrc+=2448, dwDst+=2352 ) //2352+96=2448
            {
                // LLY1.05, using macro to disable data cache power saving before issue data cache flush
                // Otherwise, it may cause A/V hang
                //REG_PLAT_CACHE_CONTROL |= CACHE_FLUSH_DCACHE; //dump-in OK => do flush immediately?
                PLAT_PROC1_DCACHE_FLUSH;

                for ( dwIndx=0; dwIndx<294; dwIndx++, dwSrc+=8, dwDst+=8) //2352/8=294
                {
                    u64Temp0 = *((volatile ULONGLONG_UNION *)dwSrc );
                    u64Temp1.b8bit[0] = u64Temp0.b8bit[1];
                    u64Temp1.b8bit[1] = u64Temp0.b8bit[0];
                    u64Temp1.b8bit[2] = u64Temp0.b8bit[3];
                    u64Temp1.b8bit[3] = u64Temp0.b8bit[2];
                    u64Temp1.b8bit[4] = u64Temp0.b8bit[5];
                    u64Temp1.b8bit[5] = u64Temp0.b8bit[4];
                    u64Temp1.b8bit[6] = u64Temp0.b8bit[7];
                    u64Temp1.b8bit[7] = u64Temp0.b8bit[6];
                    *((volatile ULONGLONG_UNION *)dwDst ) = u64Temp1;
                }
            }

            if( INFOFILTER_Detect_CDDTS() )
                __bCDDA_DTS = CDDA_DTS;
            else if( INFOFILTER_Detect_HDCD() )
                __bCDDA_DTS = CDDA_HDCD;
            else if (__bCDDA_DTS != CDDA_CDG)
                __bCDDA_DTS = CDDA_PURE;
        }

        CHIPS_ControlHDCD();
    }
    else
    {
#ifndef NO_CDG
        //LJY0.95, CDG reset for changing track.
        if(__bCDDA_DTS == CDDA_CDG)
        {
            CDG_Reset();
        }
#endif  // #ifndef NO_CDG
    }
}


//  ************************************************************************
//  Function    :   MONITOR_VideoStream
//  Description :   Monitor the video stream info. It's used to detect if
//                  IC auto perform some kinds of actions
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  ************************************************************************
void MONITOR_VideoStream(void)
{
    if (__btPlaying)
    {
        if ( __btCheckVideoType )
        {
            // one title playitem is encoded as motion -- "It don't rain"
            // but the segment play item content is encoded as High PAL still
            // Now, parser will parser motion data automatically
            // So, F/W must update the play mode as MODE_PLAYMOTION
            if(PARSER_QueryEvent(0, PARSER_EVENT_MOTION_DETECTED, TRUE))
            {
                __bModePlay=MODE_PLAYMOTION;
                __btCheckVideoType=FALSE;
            }
        }
    }

    // ** TCH2.80; begin... Fix problem: One DVCD has two format @ 1 track.
    // First is 352x288, 2nd is 352x240.
    // The DVCD title name is "¹Ü©R±µÄ²" from SZ. The reaction is a little slow.
    // If hope to accelerate, need ISR support.
    // LLY0.68, remove always check sequence header control flow
    // Since, CT909 decoder will auto motion sequence header change and adjust TV mode setting.
    // Notice: we can use above DVCD title for testing.


    if(__bModeCmd==KEY_FF || __bModeCmd==KEY_SCF || __bModeCmd==KEY_SCB)
    {
        // LLY0.68, it's un-necessary for CT909
        // Since, F/W will disable SPST parsing.
        // Reset SPST while dis-continue
        // [1] SCAN command
        // [2] FAST command
        // DVD_100aMicky, fix fast mode, subpicture buffer overflow, cause no DMA done
        // on title Titantic(48:00)
        //HAL_Reset(HAL_RESET_SPST);

        // LLY0.66, Remove following code since it maybe different for CT909
        // Chuan2.79, SCB/SCF <-> FAST, SCB <->SCF, System will hang.
        // If can't detect IFD > 1 sec, the following code will force enter Recover Stage 2 when FAST/SCF/SCB
    }

}


//  ***********************************************************************************
//  Function    :   MONITOR_AudioStream
//  Description :   Check audio bitrate for SVCD/MP3 title, and re-setting
//                  the correct value to audio DAC.
//                  Ex. one SVCD title is not 48K and it's 44.1K (It don't rain ...)
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  **********************************************************************************
void MONITOR_AudioStream(void)
{
    DWORD   dwTemp;

    // Step 1: Nothing need to do while non-playing
    if(!__btPlaying)
        return;

    // Step 2: Monitor audio frequency and set DAC to correct frequency
#ifndef DOLBY_CERTIFICATION //Kevin0.80, runtime check audio freq for Dolby sample rate sweep testing
    if(__bCheckAudioType & CHECK_AUDIO_FREQ)
#endif  // #ifndef DOLBY_CERTIFICATION
    {
        // Grievous2.60, add to determine if the setting of channel number is ready too
        // Since, audio freqency value ready not means channel number info. is ready.
        // LLY2.61, porting Grievous's code : don't wait channel no information ready for DTS audio
        // Since, DSP don't update it for DTS audio stream.
        // Notice: must use > 20070213 audio code, then channel info can updated immediately.
        //         Otherwise MP3 channel info. will be updated
        //         after 3 sec of audio frequency
        // if( HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &dwTemp) )
        // Grievous2.75, add to fix the wma audio can't play while channel number is not ready.
        if(HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &dwTemp) && ((__bAudioType==HAL_AUDIO_DTS) || (__bAudioType==HAL_AUDIO_WMA) || (HAL_ReadInfo(HAL_INFO_CHANNEL_NO, &dwTemp))))
        {
            // 0. read back audio bitrate info. since it was split from HAL_INFO_AUDIOFREQ -- LLY2.51
            HAL_ReadInfo(HAL_INFO_BITRATE, &dwTemp);

            // 1. read back audio frequency, and set audio DAC
            HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &dwTemp);

            // LLY2.30, do LPCM downsample while 96KHz
            if(__bAudioType == HAL_AUDIO_LPCM)
            {
                if(dwTemp==AUDIO_FREQ_96K)
                {
                    if(HAL_LPCM_DownSample(TRUE))
                    {
                        // Re-assign the frequency as 48KHz for ADAC programmming.
                        dwTemp=AUDIO_FREQ_48K;
                    }
                }
                else
                {
                    HAL_LPCM_DownSample(FALSE);
                }
            }
            HAL_SetAudioDAC((BYTE)dwTemp); //Step3: Host reads the frequency DM and sets correct ACLK frequency & programs ADAC.

            // LLY2.21a, read-back audio channel for CDROM/ DVDROM motion file
            // LLY2.60, don't read-back the value of AVI again since the data is prepared by parser
            //if(__bAttrPlay & TYPE_CDROM_AV)
            if( (__bAttrPlay & TYPE_CDROM_AV) && (__bAttrPlay!=ATTR_AVI) )
            {
                HAL_ReadInfo(HAL_INFO_CHANNEL_NO, &dwTemp);
                __bACHN = (BYTE)dwTemp;
                //printf("Channel NO: %lx\n", __bACHN);
            }


            // 2. read back the audio's samples/frame value and keep it.
            HAL_ReadAM(HAL_AM_SAMPLES_PER_FRAME, &dwTemp);
            __wHALSamplesPerFrame = (WORD)dwTemp;

            //Kevin1.23, Freq may change after changing track/file within 1 sec=> set prologic again
            CHIPS_ProLogicControl();

            //Kevin1.25, fix "SPDIF discont. while changing files" and "Freq wrong at file beginning" by programming ACLK & ADAC first
            //Step4: host tells DSP to start decoding
            // LLY1.06, must always issue DSP start decoding command
            // because F/W will always tell DSP to stop after audio frequency detection done.
            // ie. not only MP3/ WMA
#if 0
            if ((__bAttrPlay==ATTR_MP3)
#ifdef SUPPORT_WMA
                || (__bAttrPlay==ATTR_WMA)
#endif  // #ifdef SUPPORT_WMA
                )
#endif  // #if 0
            {
                HAL_WriteAM(HAL_AM_START_DECODE, 1);
            }

            __bCheckAudioType &= ~CHECK_AUDIO_FREQ;


            // LLY0.75, calculate the total file time for CDROM Audio only playback.
            if(__bAttrPlay & TYPE_CDROM_AUDIO)
            {
                // LLY0.76-1, avoid divide by zero bug while can't get audio bitrate.
                if(__wHALAudioBitrate)
                {
                    // kevin1.05, fix WMA total time is larger than real total time
                    if(__bAttrPlay==ATTR_WMA || __bAttrPlay==ATTR_MP3)
                    {
                        HAL_ReadAM(HAL_AM_TOTAL_TIME, &__dwFileTime);

                        // LLY2.38, get MP3 total time from DSP for variable bitrate case
                        if(__bAttrPlay == ATTR_MP3)
                        {
                            // Re-calculate average bitrate if exist file time
                            if(__dwFileTime)
                            {
                                WORD    wTemp;
                                // Calculate decoded sector = (decoded seconds * bitrate*1000)/8/2048
                                // Notice: *1000 since DSP report bitrate in K=1000 unit
                                //         /8 to transfer to BYTE unit
                                //         /2048 to transfer to sector unit
                                // average bitrate = total sectors*2048*8/total time/1000
                                //                 = total sectors*2048/total time/125
                                wTemp = (__dwTimeEnd-__dwTimeBegin+1)*2048/__dwFileTime/125;
                                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Correct bitrate: %lx --> %lx\n",__wHALAudioBitrate, wTemp);
                                __wHALAudioBitrate = wTemp;
                            }
                            // Otherwise, using original method to calculate file time
                            else
                            {
                                __dwFileTime=(__dwTimeEnd-__dwTimeBegin+1)*2048/(__wHALAudioBitrate*125);

                            }
                        }
                    }
                    else
                    {
                        // Kevin0.90, modify the calculation method, since DSP report bitrate K=1000
                        // time = total sector * 2048 (byte/sector) * 8(bit/byte) / (bitrate*1024)
                        // since DSP report bitrate unit is x KBits/ sec
                        //__dwFileTime=(__dwTimeEnd-__dwTimeBegin)*2*8/__wHALAudioBitrate;
                        __dwFileTime=(__dwTimeEnd-__dwTimeBegin+1)*2048/(__wHALAudioBitrate*125);
                    }
                }
                else
                {
                    __dwFileTime=180; // assume 3 min
                }

#ifdef  DEBUG_TOTAL_TIME
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Total File Time: %lx\n", __dwFileTime);
#endif  // #ifdef DEBUG_TOTAL_TIME

            }

        }
    }

    // Step 3: Monitor audio stream de-emphasis information and set audio DAC
#ifdef SUPPORT_AUDIO_DEEMPHASYS
    if (__bCheckAudioType & CHECK_LPCM_DEEMPHASIS)
    {
        if(HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &dwTemp))
        {
        	HAL_ReadAM(HAL_AM_LPCM_DEEMPHASIS, &dwTemp);
            if(dwTemp)
            {
                // LLY2.38, enable deemphasis action flag
                __bHALDeEmphasis = TRUE;

                // Read back audio frequency and re-set the audio DAC
                HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &dwTemp);

                // LLY2.38, do LPCM downsample while 96KHz
                if(__bAudioType == HAL_AUDIO_LPCM)
                {
                    if(dwTemp==AUDIO_FREQ_96K)
                    {
                        if(HAL_LPCM_DownSample(TRUE))
                        {
                            // Re-assign the frequency as 48KHz for ADAC programmming.
                            dwTemp=AUDIO_FREQ_48K;
                        }
                    }
                    else
                    {
                        HAL_LPCM_DownSample(FALSE);
                    }
                }

                HAL_SetAudioDAC((BYTE)dwTemp);
            }
            __bCheckAudioType &= ~CHECK_LPCM_DEEMPHASIS;
        }
    }
#endif  // #ifdef SUPPORT_AUDIO_DEEMPHASYS

 }


//  ************************************************************************
//  Function    :   _Play_TimeA
//  Description :   Control playing from point A for REPEAT AB and GOTOTIME
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  ************************************************************************
void _Play_TimeA(void)
{
    BIT     bMuteState;
    BYTE    bMode;
    extern  BIT     __btFromStopMode;
    extern  BIT     __btPlayFromBegin;
    extern  DWORD   __dwGoToTime;
    extern  BIT     __btNewTime;


    // get play playe mode for the play item, CD-DA or MOTION...
    bMode= UTL_CheckPlayItemMode ( __wPlayItem );


    // store the original flag for AudioMute
    bMuteState=__btMute;

    // temporiarily disable the AudioMute
    __btMute=TRUE;

    CHIPS_OpenAudio( FALSE ); //kevin1.20, move from below

    // LLY1.20, freeze the STC forever here,
    // Since, STC maybe un-freeze immediately while SCAN_INIT to SCAN
    // And, issue freeze STC to waiting next STC after issue stop command.
    // Otherwise, nobody to release the STC
    if(__bAttrPlay & TYPE_CDROM_AV)
    {
        HAL_ReadInfo(HAL_INFO_STC, &__dwSTCValue);
        DISP_STCFreeze(__dwSTCValue, DISP_FORCE_FREEZE_STC);
    }


    // LLY0.95, stop decoder first
    HAL_PlayCommand(COMMAND_STOP, 0);

    // LLY0.68, stop parser and servo first before jump to the requested point
    // Otherwise, the next command will be skiped.
    // Notice: it's un-necessary for CT909 to control the input data stay at a complete picture.
    SrcFilter_Stop(__dwSFStreamID);
    HAL_ControlParser(HAL_PARSER_STOP, 0);

    // LLY1.20, freeze STC to waiting next STC
    // since STC is forever freezed by F/W before issue stop command
    if(__bAttrPlay & TYPE_CDROM_AV)
    {
        DISP_STCFreeze(__dwSTCValue, DISP_WAIT_NEXT_STC);
    }


    // LLY1.00, pre-seek the servo to wake-up the servo here
    // Remove from _CC_GoTimeAction(GOTIME_EXECUTE)
    // Since servo need long time for seek command (ms ~ 3 sec)
    // So, user can see the A/V playback a moment after comfirm GoTime action.
    UTL_PreStartServo(__dwGoToTime, ACTION_SKIP);


    // Notice: need check how to do if __dwGoToTime contain CDDA data ??


    __bScan=SCAN_NONE;

    //CHIPS_OpenAudio( FALSE ); //kevin1.20, move to above

    // Step2:
    // The following action will play the beginning of play item
    // when GOTOTIME cross different track
    if (__btPlayFromBegin)
    {
        // the play item is MPEG track
        if (bMode & MODE_PLAYMOTION)
        {
            // play the beginning of this play item
            // the video output will be disabled
            //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Force Change Track\n");
            UTL_ForceChangeTrack ( __wPlayItem );
        }
    }

    // Step3:
    // Now can directly jump data to a new point
    // will take as pause->play action
    // In REPEAT AB mode, the new point will be __dwTimeA

    __btPause=TRUE;
    // In pause->play, It will reference __dwTimeNow for next start point to play
    __dwTimeNow=__dwGoToTime;

    __btPlaying=FALSE;

    // restore the AudioMute and ErrorConcealment
    __btMute=bMuteState;

    // LLY1.05, don't always reset to NPF while GoTime
    // And, the caller must set the desired playing mode first
    // Thus, DivX SCAN --> FF can go "GoTime" path and keep status as FF
#if 0
    // GOTOTIME->KEY_PLAY, return to normal play
    if(__bRepeatAB==REPEAT_NONE)
    {
        __bModeCmd=KEY_PLAY;
    }
#endif  //


    // LLY1.05, reset A/V/SP together while bit-stream dis-continue.
    // Notice: don't forget to reset SPST otherwise the SPST buffer maybe overflow.
    HAL_Reset(HAL_RESET_AVSPST);

    // perform sending command, and input data from new point
    CC_KeyCommand (__bModeCmd);

    if (__btPlayFromBegin)
    {
        // LLY0.80, give the time-out value as zero
        // Since, MSG_TRACK is used to notify OSD/ PANEL the track info. changed.
        // Not to display "TRACK XXX" on screen.
        UTL_ShowTrack(MSG_TRACK, __wPlayItem, 0); // 3);
        __btPlayFromBegin=FALSE;        // action done, clear the flag
    }

    //UTL_DelayTime(COUNT_500_MSEC, FALSE); // To make sure not to see the first picture

    // LLY0.95, don't need to clear search mode flag only while B to A point
    // because, it don't clear search mode UI together.
    // Now, search UI can exist with repeat A-B
    // And, cancel repeat A-B feature while confirm the GoTime Action.
#if 0
    if (__bSearchMode == SEARCH_MODE_TIME) // avoid showing transient OSD time during gototime search.
        __bSearchMode=SEARCH_MODE_OFF;
#endif  // #if 0

    //DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);

    // LLY2.05, restore the frame count for DSP known while playback CDROM/ DVDROM audio only file
    // Otherwise, frame count will be reset as zero by DSP reset, and OSD will start from 0
    if(__bAttrPlay & TYPE_CDROM_AUDIO)
    {
        HAL_WriteAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, __dwGoTimeFrameCnt);

        // Also reset the total & previous frame count value as desired position frame count,
        // Since, _UTL_CalTime_FromFrameCnt() will reference it.
        _dwTotalFrameCount = __dwGoTimeFrameCnt;
        _dwPreFrameCount = __dwGoTimeFrameCnt;
    }


    // *TCH** 97.7.15; __btPause=FALSE;
    __btNewTime=FALSE;
    __btFromStopMode=FALSE;
}


//  ***************************************************************************************
//  Function    :   _UTL_ScanTrigger
//  Description :   Process SCAN mode stage trigger
//  Arguments   :   None
//  Return      :   Return
//  ***************************************************************************************
void _UTL_ScanTrigger(void)
{
    WORD    wTemp;
    DWORD   dwTemp;
    BYTE    bStartJump=FALSE;

    // The sectors to jump for each SCAN complete
    // The sectors is defined in SCAN_GAP
    extern  WORD    __wScanGap;

    switch(__bScan)
    {
    case    SCAN_INIT:
    case    SCAN_GO:
        // Step 2: wait scan done action after scan the desired range finish, ex.
        //         playback a while for CDDA, MP3, MP2, or get IFSC
        // Step 2-1: CDDA scan mode control flow.
        // LLY1.01, do aduio only mode procedure while video stream has been skip.
        if( (__bAttrPlay == ATTR_CDDA) || (__bVideoID==HAL_THROW_VIDEO) )
        {
            // LLY1.02, don't issue next jump while STC is not updated for DivX w/ video is skip.
            // Since, it need more time for data sending to audio buffer
            // Then, DSP can decode the stream and update the STC
            // Otherwise, UTL_JumpGroove() will get the same next position by un-updated STC value.
            // kevin1.20, fix DivX audio only -> CDDA SCFx2 => audio is still on normal play
            //if( (__bVideoID==HAL_THROW_VIDEO) && (!__dwSTCUpdated) )
            if( (__bAttrPlay == ATTR_AVI) && (__bVideoID==HAL_THROW_VIDEO) && (!__dwSTCUpdated) )
            {
                __dwScanTime = OS_GetSysTimer();
                return;
            }

            // Playback CDDA bitstream a while (maybe 400 msec), then stop it.
            // And, prepare to jump to another position.
            // LLY0.70, enlarge the timer from 400msec to 1sec
            // Otherwise, can't here the audio.
            if ((OS_GetSysTimer() - __dwScanTime) >= COUNT_1_SEC)
            {
                __dwScanTime = OS_GetSysTimer();
                bStartJump = TRUE;
            }
        }

    	//Step 2-2: Audio only SCB mode, ex. MP3/ MP3/ DTS/ AC3 -- LLY0.75
        else if( (__bAttrPlay & TYPE_CDROM_AUDIO) && (__bModeCmd==KEY_SCB) && __bScanLevel )
        {
            if ( (OS_GetSysTimer() - __dwScanTime) >= COUNT_1_SEC )   //can't hear clear sound if < 1 sec
            {
                __dwScanTime = OS_GetSysTimer();
                bStartJump = TRUE;

                // LLY2.04, call API to transfer decoded frame count to decoded sector
                // And, plus it w/ track begin to get physical sector ID
                __dwTimeNow = __dwTimeBegin + UTL_FrameCntToSectors();
            }
        }
        // Step 2-3: VCD, SVCD, CVD and other motion scan mode
        else
        {
            if(__bScan==SCAN_GO) // Scan command has done, and porting the scan done, ex. IFSC
            {
                HAL_ReadInfo(HAL_INFO_DECCMD_ACK, &dwTemp);

#ifdef SUPPORT_FUSS_MPEG_CD
                // LLY2.37, always treat as SCAN_DONE for Fuss MPEG CD since it's no video
                // Otherwise, the scan action will not continue, and OSD not update.
                if (INFOFILTER_FUSSMPEGCD(IF_MODE_CHECK_MPEG_CD,0))
                    dwTemp=MODE_SCAN_DONE;
#endif // #ifdef SUPPORT_FUSS_MPEG_CD

                if( dwTemp == MODE_SCAN_DONE )
                {
                    bStartJump=TRUE;
                    // LLY0.76-1, keep get scan ack time for do scan mode waiting control
                    // only for CDROM motion scan mode.
                    //if(__bAttrPlay & TYPE_CDROM_AV)
                    {
                        __dwScanTime = OS_GetSysTimer();
                    }
                }
                // LLY2.71, do error protection while still can't find SCAN_DONE flag
                else
                {
                    if((OS_GetSysTimer() - __dwCmdTime) >= COUNT_5_SEC )
                    {
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: force jump since can't wait SCAN_DONE after 5sec\n");
                        bStartJump=TRUE;
                        __dwScanTime = OS_GetSysTimer();
                    }
                }
            }
            // LLY1.02, let it go scan jump directly while SCAN_INIT
            else if(__bScan==SCAN_INIT)
            {
#ifdef  DEBUG_SCAN_MODE
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Scan Init\n");
#endif  // #ifdef DEBUG_SCAN_MODE

                bStartJump=TRUE;

                // must keep current sec for SCAN mode base position (in msec format) first
                HAL_ReadInfo(HAL_INFO_STC, &__dwSTCValue);
                __dwScanBaseTime = __dwSTCValue/90;
            }
        }

        // Step 3: prepare something to start next jump, ex.
        //         [1] freeze STC to avoid OSD show zero
        //         [2] Stop decoder, parser, source
        //         [3] reset decoder
        if(bStartJump)
        {
#ifdef  DEBUG_SCAN_MODE
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Scan done\n");
#endif  // #ifdef  DEBUG_SCAN_MODE

            // Set the flag as SCAN_END, means that Complete scan an I frame
            __bScan= SCAN_END;

            // LLY0.80a, move the code ahead since show time procedure will be called early.
            // LLY0.75, must read-back STC value first for CDROM A/V file playback
            // Otherwise, the timer is not updated since nobody to read-back it.
            if(__bAttrPlay & TYPE_CDROM_AV)
            {
                // LLY0.85a, delay a while for new STC is updated done.
                // Since, STC is zero during SCAN ~ SCAN_DONE period, and will be updated by next display.
                // So, delay a sync. time. 16.6(NTSC) ~ 25 (PAL) msec.
                OS_DelayTime(COUNT_50_MSEC);//, FALSE);
                HAL_ReadInfo(HAL_INFO_STC, &__dwSTCValue);


                // LLY1.20, freeze the STC forever here,
                // Since, STC maybe un-freeze immediately while SCAN_INIT to SCAN
                // And, issue freeze STC to waiting next STC after issue stop command.
                // Otherwise, nobody to release the STC
#if 0
                // LLY1.02, freeze the STC value after SCAN done for CDROM motion file
                // To avoid OSD time going before next SCAN update.
                DISP_STCFreeze(__dwSTCValue, DISP_WAIT_NEXT_STC);
#endif  //
                DISP_STCFreeze(__dwSTCValue, DISP_FORCE_FREEZE_STC);
            }
            // LLY2.35, must keep current time for WMA
            // Since the time is calculated by frame count,
            // and it will be reset to zero after DSP reset
            else if(__bAttrPlay == ATTR_WMA)
            {
                // Notice: can shared w/ __dwSTCValue to keep WMA time
                __dwSTCValue = _UTL_CalTime_FromFrameCnt()/75;
            }

            // LLY0.80, update current OSD/ VFD time while scan done
            // Notice:
            // [1] Must call UTL_GetCurrentTime() to get __dwTimeNow before parser stop
            //     Otherwise, parser will report worng value.
            // [2] Must keep cuffer buffer estimate time first
            //     Otherwise, buffer will be clear by HAL_RESET_VIDEO
            // [3] Don't call UTL_GetCurrentTime() for CDROM audio only file
            //     Since, it will use frame count to calculate decoding sector
            if( !(__bAttrPlay & TYPE_CDROM_AUDIO) )
            {
                UTL_GetCurrentTime();
            }

            // LLY1.02, always keep buffer estimate time.
            __dwBufferTime=UTL_EstimateBufferTime();

            // Update OSD/VFD time
            UTL_OutputShowTime();


            // LLY1.02, must stop decoder first while
            // [1] 1st enter SCAN mode
            // [2] Can't get SCAN_DONE flag after time-out
            // Otherwise, it may cause Decoder hang easily
            // Since stop servo and parser procedure is put before HAL_RESET_VIDEO
            // And, the bitstream is not sent completely.
            // Notice: can't put before reading STC value
            // Since, the STC value will be reset to zero while STOP command.
            // LLY2.77, don't check __bScan != SCAN_END here
            // Since, the value is assigned earily.
            //if(__bScan != SCAN_END)
            {
                HAL_PlayCommand(COMMAND_STOP, NULL_0);
            }

            // Stop parser and servo
            // Notice: must put before buffer reset done
            // Otherwise parser will continue to sending data to buffer
            SrcFilter_Stop(__dwSFStreamID);
            HAL_ControlParser(HAL_PARSER_STOP, 0);


            // LLY1.20, freeze STC to waiting next STC
            // since STC is forever freezed by F/W before issue stop command
            // LLY2.77, remove the procedure after HAL_RESET_VIDEO to make sure video decoder is stop.
#if 0
            if(__bAttrPlay & TYPE_CDROM_AV)
            {
                DISP_STCFreeze(__dwSTCValue, DISP_WAIT_NEXT_STC);
            }
#endif  // #if 0

            // LLY1.01, do audio only scan mode control while video stream has been skip
            if( (__bModePlay == MODE_PLAYAUDIO) || (__bVideoID==HAL_THROW_VIDEO) )
            {
                //HAL_ReadInfo(HAL_INFO_AUDIO_REMAIN, &__dwTemp);
                //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "ARem: %lx\n", __dwTemp);

                // soft mute the audio output
                CHIPS_SoftMute();

                // Chuan1.00, Reset A-Buffer for SCF/SCB
                HAL_Reset(HAL_RESET_AUDIO);

                // Open the audio
                CHIPS_OpenAudio(TRUE);
            }
            else
            {
                HAL_Reset(HAL_RESET_VIDEO);
            }

            // LLY2.77, issue freeze STC to waiting next STC
            // after HAL_RESET_VIDEO to make sure video decoder is stop.
            // Old method will cause one error: Fast --> SCAN mode
            // previous stop command is not done, and F/W issue freeze STC to waiting next STC
            // So, STC continue updated, and maybe become zero by following HAL_RESET_VIDEO
            // Thus, F/W may get a zero STC value for next reference.
            if(__bAttrPlay & TYPE_CDROM_AV)
            {
                DISP_STCFreeze(__dwSTCValue, DISP_WAIT_NEXT_STC);
            }

        }


        // Step 4: Start the next jump action, ie. calculate next jump position.
        if(bStartJump)
        {
            bStartJump = FALSE;
            // Step 4-1: calculate the next jump postion for each type.
            // LLY0.75, support audio only bitstream SCB mode
            if( (__bAttrPlay&TYPE_CDROM_AUDIO) && (__bModeCmd==KEY_SCB) && __bScanLevel )
            {
                // Calculate the dump second
                dwTemp = (__bScanLevel<<2);

                // LLY2.35, give desired time (in second) to parser
                // to get corresponding sector ID for WMA next jump
                if(__bAttrPlay == ATTR_WMA)
                {
                    if(__dwSTCValue > dwTemp)
                    {
                        dwTemp = __dwSTCValue - dwTemp;
                    }
                    else
                    {
                        dwTemp=0;
                    }
                    __dwTimeNow = UTL_WMATimeToSectors(dwTemp);

#ifdef  DEBUG_WMA_SCAN
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Begin: %lx,
                        Sector: %lx, Time: %lx, Jump: %lx\n",
                        __dwTimeBegin, __dwTimeNow, __dwSTCValue, dwTemp);
#endif  // #ifdef DEBUG_WMA_SCAN
                }
                else // Others
                {
                    // Calculate the sector number base on desired jump second value
                    // sector = (second * bitrate *1000)/8/2048
                    // Notice: *1000 since DSP report bitrate in K=1000 unit
                    //         /8 to transfer to byte unit
                    //         /2048 to transfer to sector unit
                    dwTemp = (dwTemp*__wHALAudioBitrate*125)/2048; // * (bitrate*1000)/(8*2048)

                    // Calculate the final sector ID by minus the jump sector
                    if ( __dwTimeNow > __dwTimeBegin+dwTemp )
                        __dwTimeNow -= dwTemp;
                    else
                        __dwTimeNow = __dwTimeBegin;
                }

                // LLY2.04, call API to transfer desired sectors to frame count
                // Notice: must minus track begin to get sector offect
                dwTemp = UTL_SectorsToFrameCnt(__dwTimeNow-__dwTimeBegin);

                // update the frame count to DSP
                HAL_WriteAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, dwTemp);

                _dwTotalFrameCount = 0;
            }
            else // For VCD/ SVCD/ CVD/ CDDA/ CDROM motion scan mode control
            {
                __dwTimeNow=UTL_JumpGroove(__bModeCmd, __dwTimeNow);
            }
            // Step 5: control for cross track issue
            if ( __bModeCmd == KEY_SCF )
            {
                // change > into >=
                if ( (__dwTimeNow+ __wScanGap) >= __dwTimeEnd )
                {
                    // SCF is playing to the end of Playing RANGE
                    // ignore the next OSD display
                    // wyc1.10-3, don't support this message now.
                    //OSD_Output(MSG_IGNORENEXT, 1, (BYTE)NULL);

                    if ( __btPBC )
                    {
                        // let VCD20_Trigger process the next action
                        __btPlayEnd=TRUE;
                        __btPlaying=FALSE;
                        // will force to reset
                        __bModePlay=MODE_PLAYUNKNOW;
                    }
                    else
                    {
                        // LLY2.36, always stop the file playback while DivX certification.
#ifdef  SUPPORT_DIVX_CERTIFICATION
                        if(__bAttrPlay == ATTR_AVI)
                        {
                            __bFWKey = KEY_STOP;
                        }
                        else
#endif  // #ifdef  SUPPORT_DIVX_CERTIFICATION
                        {
                            // must check repeat mode then decide to play next
                            // modify for use by LINEAR_ChangeTrack call
                            LINEAR_ChangeTrack(KEY_NEXT, (BYTE)NULL);
                        }
                    }
                    return;// FALSE;
                }
            }   // if(__bModeCmd == KEY_SCF)
            else if ( __bModeCmd == KEY_SCB )
            {
                // change > into >=
                if (__dwTimeNow <= __dwTimeBegin)    //SCB reach track begin
                {
                    if ( __btPBC )
                    {
                        // When scan back to play item begin
                        // will normal play this play item
                        // Notice:
                        // [1] don't use KEY_STOP to simulate this action, since PBC state will be reset
                        //     ex. CC_DoKeyAction(KEY_STOP);
                        // [2] KEY_PAUSE can keep current PBC state, but still can't use KEY_PAUSE to
                        //     simulate the action, since KEY_PAUSE is probinit in SCB mode.
                        //     ex. CC_DoKeyAction(KAY_PAUSE);
                        // [3] Set __btPlaying=FLASE, __btPause=TRUE to simulate the pause flow
                        //     It's ok for CT908 flow.
                        //     But, it fail for CT909 flow since the checking condition is re-design
                        //     ie. F/W don't re-send playing range for pause --> play.
                        //     So, the old method is not work.
                        // Now, let __btPlaying=FALSE, __btPause=FALSE together -- LLY0.75
                        // Then, the playing range will be re-sending
                        // Since, the desired flow is same as re-start playback current track
                        // So, A/V hang bug can be fixed while PBC on --> playing item --> SCB to track beginning
                        __btPlaying=FALSE;
                        __btPause=FALSE;
                        // force to reset
                        __bModePlay= MODE_PLAYUNKNOW;
                        __dwTimeNow= 0;
                        // Remove it as it play same PlayItem
                        // __btCheckBitStream= TRUE;
                        // Will normal play this play item
                        CC_DoKeyAction ( KEY_PLAY );
                    }
                    else
                    {
    // LLY.101-2, must same as PBC on mode's flow while enable following define
    // otherwise, it will issue KEY_PREV action
    // ** TCH1.55-1; begin... FORCE_CHANGE_TRACK_NORMAL_PLAY can do it simpler.
#ifdef  FORCE_CHANGE_TRACK_NORMAL_PLAY
                        // LLY2.36, just do GoTime 0:00:00 while scanback to file begin
                        // If go same flow of other title, it will go _MM_MotionTrigger()
                        // w/ MM_STAGE_PREPAREINFO --> MM_STAGE_START_PRESCAN
                        // --> MM_STAGE_SET_PLAYMODE --> MM_STAGE_WAIT_PLAY
                        // So, it will do DRM rental count - 1 ==> It's wrong.
                        if(__bAttrPlay == ATTR_AVI)
                        {
                            __dwGoToTime=0;
                            __dwGoTimeSTC=0;
                            __btNewTime=TRUE;
                            __bModeCmd = KEY_PLAY;

                            // Notice: also need to restore SP since SP will be throw during SCAN mode
                            if(__bSPSTNs)
                            {
                                if ((__bSP_Disp_OFF == 0) || (__bSP_Disp_OFF&0x80))
                                {
                                    DISP_DisplayCtrl(DISP_SP1, TRUE);
                                }
                                PARSER_ActionFilter( 0, PARSER_ENABLE_SP1, TRUE );
                            }
                        }
                        else
                        {
                            wTemp= __wPlayItem;
                            // reset the play state
                            //Kevin1.07, fix "In program playing, SCB to track begin will clear program mode"
                            //          CC_DoKeyAction (KEY_STOP) will clear program mode
                            //          SCF to track end also calls CC_KeyCommand(KEY_STOP)
                            //CC_DoKeyAction ( KEY_STOP );
                            CC_KeyCommand ( KEY_STOP );

                            __wPlayItem= wTemp;
                            // next will normal play from the frist track
                            CC_DoKeyAction ( KEY_PLAY );
                        }
#else   // #ifdef  FORCE_CHANGE_TRACK_NORMAL_PLAY

                        // SCANBACK to first track, system will become normal play
                        if ( __wPlayItem == __wTrackBegin )
                        {
                            // reset the play state
                            //Kevin1.07, fix "In program playing, SCB to track begin will clear program mode"
                            //          CC_DoKeyAction (KEY_STOP) will clear program mode
                            //          SCF to track end also calls CC_KeyCommand(KEY_STOP)
                            //CC_DoKeyAction ( KEY_STOP );
                            CC_KeyCommand ( KEY_STOP );

                            // next will normal play from the frist track
                            CC_DoKeyAction ( KEY_PLAY );
                        }
                        else    // will Scan back previous track
                        {
                            // must check repeat mode then decide to play next
                            // modify for use by LINEAR_ChangeTrack call
                            LINEAR_ChangeTrack(KEY_PREV, (BYTE)NULL);
                        }   // NOT BeginTrack
#endif  // #ifdef  FORCE_CHANGE_TRACK_NORMAL_PLAY
    // ** TCH1.55-1; end...
                    }   // __btPBC
                    return;// TRUE;
                }   // SCB reach to the Begin of the range
            }   // KEY_SCB

            // Step 6: Start to playback the desired range
            // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
            UTL_PlayFromTime(__dwTimeNow, __dwTimeEnd);

            // LLY1.01, do audio only scan mode while video stream has been skip.
            if( (__bModePlay==MODE_PLAYAUDIO) || (__bVideoID==HAL_THROW_VIDEO) )
            {
                if(__bVideoID==HAL_THROW_VIDEO)
                {
                    // Issue audio master command again,
                    // Otherwise, nobody to clear the __dwSTCUpdated flag
                    // So, scan jump checking will be start.
                    HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, FALSE);

                    // LLY1.02, waiting audio buffer reach 1/8 fullness for DivX w/ skip video stream case
                    // Otherwise, audio break noise maybe hear since the A/V data layout is interleave
                    _dwTimeStart=OS_GetSysTimer();
                }

            }
            else // VCD/ SVCD/ CVD/ Other motion.
            {
            }

        }

        break;

    // Step 7: Start next command
//    case    SCAN_STAGE_NEXT_SCAN:
    case    SCAN_END:
        // LLY1.01, do audio only scan mode while video stream has been skip.
        if( (__bModePlay==MODE_PLAYAUDIO) || (__bVideoID==HAL_THROW_VIDEO) )
        {
            if(__bVideoID==HAL_THROW_VIDEO)
            {
                // LLY2.36a, break this case and return to main flow let input key can be processed
                //while(OS_GetSysTimer()-_dwTimeStart < COUNT_3_SEC)
                if(OS_GetSysTimer()-_dwTimeStart < COUNT_3_SEC)
                {
                    HAL_ReadInfo(HAL_INFO_AUDIO_REMAIN, &__dwTemp);
                    if( __dwTemp > 3000) //((DS_AD0BUF_END_DIVX-DS_AD0BUF_ST_DIVX+8)/8) )
                    {
                        break;
                    }
                    else
                    {
                        return;
                        //OS_YieldThread();
                    }
                }
            }
            // LLY0.68, issue DSP play command since it will not done within HAL_RESET_AUDIO
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);
        }
        else
        {
            // LLY0.76-1, wait 1 sec for CDROM motion scan mode
            // Since the scan mode speed is very fast.
            // LLY2.36a, break this case and return to main flow let input key can be processed
            //while( (OS_GetSysTimer() - __dwScanTime) < COUNT_500_MSEC )
            if( (OS_GetSysTimer() - __dwScanTime) < COUNT_500_MSEC )
            {
                //OS_YieldThread();
                return;
            }

            // send a scan command
            HAL_PlayCommand(COMMAND_SCAN, 0);
        }

        // Start scaning a new I picture
        __bScan= SCAN_GO;           // in MC [SCAN] mode

        // LLY2.71, keep issue SCAN CMD time
        // And do error protection if too long can't get SCAN_DONE flag after this time
        __dwCmdTime = OS_GetSysTimer();

#ifdef  DEBUG_SCAN_MODE
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Scan again: %lx\n", (OS_GetSysTimer()-__dwScanTime)/(COUNT_50_MSEC));
#endif  // #ifdef  DEBUG_SCAN_MODE


        break;
    }
}


//  ***************************************************************************************
//  Function    :   MONITOR_CheckBuffer
//  Description :   Monitor buffer fullness or playing range for different playing mode
//                  It also do the buffer control and check some info., ex.
//                  Repeat AB, GotoTime, NPF/ Freeze/ Slow/ SCF/ SCB
//  Arguments   :   NULL
//  Return      :   TRUE
//  Side Effect :
//  ***************************************************************************************
BIT  MONITOR_CheckBuffer(void)
{
    extern  DWORD   __dwGoToTime;

    // Step 1: Process REPEAT AB flow
    // Set A poiint-> Set B point
    // check reaching B point->Play from A point->check reaching B point...
    // LLY2.61, only do this checking during playback moment
    // Since, if non-playback mode, parser will reports last point of desried range
    // by call UTL_GetDecodertime()
    // So, it will always TRUE of dwDecodedTime > __dwTimeB
    // ex. Repat A-B --> enter/ exit SETUP (do stop/resume)
    // So, it will always think B point is reached.
    // And, playback from Time A not continue playback from enter setup point.
    // if(__bRepeatAB == REPEAT_AB)
    if(__bRepeatAB == REPEAT_AB && __btPlaying)
    {
        DWORD   dwDecodedTime;

        if (__wDiscType & BOOK_DVD) // DVD title is controled by CELL module
            return TRUE;

        // LLY2.04, call API to get current decoded sector
        dwDecodedTime = UTL_GetDecodedTime();

        if(dwDecodedTime >= __dwTimeB)
        {
            // must jump data back to A point
            // the action is the same as GOTOTIME
            __dwGoToTime=__dwTimeA;

            // LLY2.61, update the time A frame count value to __dwGoTimeFrameCnt
            // Since, it will be reference by _Play_TimeA()
            if(__bAttrPlay & TYPE_CDROM_AUDIO)
            {
                __dwGoTimeFrameCnt = __dwTimeA_FrameCnt;
            }
            // LLY2.61, update the time A STC value to __dwGoTimeSTC
            // Since, it will be reference by _Play_TimeA() --> UTL_Start_PlayItem()
            if(__bAttrPlay == ATTR_AVI)
            {
                __dwGoTimeSTC = __dwTimeA_STC;
            }

            __btNewTime=TRUE;   // flag indicate that need perform GOTOTIME
        }   // end checking B point
    }   // if ( __bRepeatAB == REPEAT_AB )

    // next play must play from __dwGoToTime
    if (__btNewTime)    // GOTOTIME or REPEAT AB B->A
    {
        //DVD_049MickyGoTime
        // the reset point shall be before starting A point
        _Play_TimeA();
    }   // End of if (__btNewTime)


    switch ( __bModeCmd )
    {
        // LLY0.66, remove buffer control case from CT909
        // Since it's only necessary for CDIF path.
    case    KEY_STOP:
    case    KEY_FF:
    case    KEY_PLAY :
    case    KEY_SF :
    case    KEY_LAN_STUDY:
    case    KEY_STEP:
    case    KEY_FREEZE :
        break;

    // Process SCAN mode flow
    case    KEY_SCF:
    case    KEY_SCB:
        // DVD scan mode is controled by CELL module
        if (__wDiscType & BOOK_DVD)
            break;

        _UTL_ScanTrigger();
        break;

    default:
        break;

    }   // switch ( __bModeCmd )
    return TRUE;
}


// LLY0.66 create ...
// *****************************************************************************
//  Function    :   UTL_JumpGroove
//  Description :   Used to get next position for scan function
//  Arguments   :   bMode, specify the SCF or SCB mode, same as __bModeCmd
//                  dwTime, give the current time value
//  Return      :   The next scan position
//  Notice      :   Only suit for CDDA/ VCD/ SVCD/ CVD
// ******************************************************************************
DWORD UTL_JumpGroove(BYTE bMode, DWORD dwTime)
{
    extern WORD    __wScanGap;
    static DWORD   dwPrevTime=PARSER_FAIL_SECTOR - 1;
    PARSER_AV_PTS  AV_pts;
    DWORD   dwDesiredScanTime;

    // LLY0.95, using different method to get position for AVI/ DivX playback
    if( (__bAttrPlay == ATTR_AVI) )
    {
#ifdef  DEBUG_SCAN_JUMP
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "[1] Scan base time: %lx\n", __dwScanBaseTime);
#endif  // #ifdef DEBUG_SCAN_JUMP

        // LLY1.05-1, keep desired GoTime (in sec format) for SCAN --> other mode usage.
        // ie. it will be reference by _CC_AVIScanToPlayProcess().
        // LLY1.21, remove GoToTime calculation to _CC_AVIScanToPlay()
        //__dwGoToTime=__dwScanBaseTime/1000+1;

        if(bMode==KEY_SCF)
        {
            // Calculate the desired scan jump time
            dwDesiredScanTime = __dwScanBaseTime+__wScanGap*1000;

#ifdef  DEBUG_SCAN_JUMP
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Desired Time: %lx\n", dwDesiredScanTime);
#endif  // #ifdef DEBUG_SCAN_JUMP

            dwTime=PARSER_GetContainerTimePosition(__dwTimeBegin, dwDesiredScanTime, 0, &AV_pts);

            if(dwTime==PARSER_FAIL_SECTOR)
                goto LABEL_STOP_SCAN;


            // Do current decoded PTS and next Scan base time compare first
            // Since, current decoded PTS value maybe larger than desired next Scan base time
            // So, we may decode the same I-frame
            HAL_ReadInfo(HAL_INFO_STC, &__dwTemp);
            if(__dwTemp >= AV_pts.dwVideoPTS)
            {
#ifdef  DEBUG_SCAN_JUMP
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Current PTS >= next scan base time: (%lx, %lx)\n", __dwTemp, AV_pts.dwVideoPTS);
#endif  // #ifdef DEBUG_SCAN_JUMP
                dwTime=PARSER_GetContainerTimePosition(__dwTimeBegin, (__dwTemp/90+__wScanGap*1000), 0, &AV_pts);

                if(dwTime==PARSER_FAIL_SECTOR)
                    goto LABEL_STOP_SCAN;

            }

            // Do previous and next desired scan position compare
            // If same, find the next index entry
            if( dwTime == dwPrevTime )
            {
#ifdef  DEBUG_SCAN_JUMP
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Same position\n");
#endif  // #ifdef DEBUG_SCAN_JUMP
                dwTime=PARSER_GetContainerTimePosition(__dwTimeBegin, dwDesiredScanTime, 1, &AV_pts);

                if(dwTime==PARSER_FAIL_SECTOR)
                    goto LABEL_STOP_SCAN;
            }


        }
        else if(bMode==KEY_SCB)
        {
            // LLY1.02, re-start the playback from file begin directly
            // if next scan jump time is <= 0 for next SCB mode
            if( __dwScanBaseTime <= (__wScanGap*1000) )
                goto LABEL_STOP_SCAN;


            // Calculate the desired scan jump time
            dwDesiredScanTime = __dwScanBaseTime-__wScanGap*1000;

#ifdef  DEBUG_SCAN_JUMP
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Desired Time: %lx\n", dwDesiredScanTime);
#endif  // #ifdef DEBUG_SCAN_JUMP

            // LLY2.36, must give -1 for SCB mode to get previous nearest position.
            dwTime=PARSER_GetContainerTimePosition(__dwTimeBegin, dwDesiredScanTime, (-1), &AV_pts);

            if(dwTime==PARSER_FAIL_SECTOR)
                goto LABEL_STOP_SCAN;


            // Do previous and next desired scan position compare
            // If same, find the next index entry
            if( dwTime == dwPrevTime )
            {
#ifdef  DEBUG_SCAN_JUMP
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Same position\n");
#endif  // #ifdef DEBUG_SCAN_JUMP
                // LLY2.36, give -1 to get 1st previous near position
                // So, if same position, must give -2 to get previous one.
                dwTime=PARSER_GetContainerTimePosition(__dwTimeBegin, dwDesiredScanTime, (-2), &AV_pts);

                if(dwTime==PARSER_FAIL_SECTOR)
                    goto LABEL_STOP_SCAN;
            }
        }

        dwPrevTime = dwTime;

        // Keep the new scan base time (msec format) by calculate from next scan jump action.
        __dwScanBaseTime = AV_pts.dwVideoPTS/90;

#ifdef  DEBUG_SCAN_JUMP
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "[2] Scan Base Time: %lx, dwTime: %lx\n", __dwScanBaseTime, dwTime);
#endif  // #ifdef DEBUG_SCAN_JUMP

        //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Scan Pos: %lx, VPTS: %lx\n", dwTime, AV_pts.dwVideoPTS);
        return dwTime;

        // If return sector ID is 0xFFFFFF: can't find the corresponding index
        // > last item within index table
        // < 1st item within index table.
        // Then, return file begin or end to stop SCAN mode
LABEL_STOP_SCAN:
        if(bMode==KEY_SCF)
            return (__dwTimeEnd);
        else if(bMode==KEY_SCB)
            return (__dwTimeBegin);


    }


#ifdef  DEBUG_SCAN_JUMP
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "[1] dwTime: %lx\n", dwTime);
#endif  // #ifdef DEBUG_SCAN_JUMP

    // LLY1.02, must minus the buffer estimate time for CDROM title
    // Since, the input parameter of this API is sector format.
    // LLY0.80, minus the buffer estimate time for non-DVD and non CDROM title.
    // ie. calculate time using sector number.
    if( !(__wDiscType&BOOK_DVD) )// && !(__wDiscType&CDROM_M1) )
    {
        if(dwTime > __dwBufferTime)
        {
            dwTime -= __dwBufferTime;
        }
    }


    if(bMode==KEY_SCF)
    {
        // the jump time range
        dwTime+= __wScanGap;
    }
    else if(bMode==KEY_SCB)
    {
        if ( dwTime > (__wScanGap+ SCB_EXTRA) )
            dwTime-= (__wScanGap+ SCB_EXTRA);
        else
            dwTime= 0;
    }

#ifdef  DEBUG_SCAN_JUMP
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "[2] dwTime: %lx\n", dwTime);
#endif  // #ifdef DEBUG_SCAN_JUMP

    // the time need jump to
    return dwTime;
}



// LLY2.32, porting Micky's code to fix some DVD tile root menu can't play end issue
// for Harry Potter & Alexander root menu
//  ********************************************************************************
//  Function    :   UTL_CheckNVErrorCode
//  Description :   Check the error code if NV reading fail
//  Return      :   NV_FAIL_BY_READ, servo reading error
//                  NV_FAIL_BY_FORMAT, the NV data format is wrong
//  Notice      :   Only used while F/W can't get NV sector
//  ********************************************************************************
BYTE	UTL_CheckNVErrorCode(void)
{
    if(PARSER_GetEDCFlag(0)!=0)
    {
        return NV_FAIL_BY_READ;        // servo read error
    }

    // Otherwise, NV format is wrong.
    return NV_FAIL_BY_FORMAT;   // parser don't think it's a NV sector
}


// LLY0.95 create ...
//  ***********************************************************************************
//  Function    :   UTL_DRM_PlaybackCapability
//  Description :   Handle DivX DRM file playback capability checking, ex.
//                  [1] Check if Rental/ Authorized Error
//                  [2] Check if Rental limit
//  Arguments   :   None
//  Return      :   DRM_PLAY_STATE_ABORT, Rental Expired or Authorized Error,
//                                        so abort the playback
//                  DRM_PLAY_STATE_WAIT, Rental file, waiting user confirm
//                  DRM_PLAY_STATE_CONTINUE, Purchase file, contine the playback
//  Notice      :   Can be called after the desired file pre-scan ok.
//                  And, only need to call while the desired file is encrypted.
//  ***********************************************************************************
BYTE UTL_DRM_PlaybackCapability(void)
{
#ifndef NO_DRM

    DWORD   dwTemp;
    BYTE    bUseLimit, bUseCount;

    // [1] Check if the user has been authorized to view the file
    //     If not, display the related screen, and stop the playback.
    //     Otherwise, continue the playback
    dwTemp=PARSER_DRM_Init(&bUseLimit, &bUseCount);
#ifdef  DEBUG_DRM
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DRM Init: %lx, Limit: %lx, Count: %lx\n", dwTemp, bUseLimit, bUseCount);
#endif  // #ifdef DEBUG_DRM

    switch(dwTemp)
    {
    // No Error
    case    DRM_ERROR_NONE:
        break;

    // Not limit rental type
    case    DRM_ERROR_NOT_LIMITED_RENTAL_TYPE:
        return (DRM_PLAY_STATE_CONTINUE);

    // Rental Expired
    case    DRM_ERROR_RENTAL_EXPIRED:
        // Show "Rental Expired" screen
        OSDDIVX_DrawDialog(DLG_RENTAL_EXPIRED);
        return (DRM_PLAY_STATE_WAIT);

    // Authorized Error
    case    DRM_ERROR_NOT_AUTH_USER:
    default:
        // Show "Authorized Error" screen
        OSDDIVX_DrawDialog(DLG_AUTHOR_ERROR);
        return (DRM_PLAY_STATE_WAIT);
    }


    // [2] Check if the file is rental or purchase
    if(bUseLimit) // rental file
    {
        __bDRMRemainView=bUseLimit-bUseCount;
        if(__bDRMRemainView)
        {
            OSDDIVX_DrawDialog(DLG_RENTAL_CONFIRM);
            return(DRM_PLAY_STATE_WAIT);
        }
        else
        {
            return (DRM_PLAY_STATE_CONTINUE);
        }
    }

#endif  // #ifndef NO_DRM

    // [3] Others, return continue playback, ex. purchase file
    return DRM_PLAY_STATE_CONTINUE;
}


// LLY0.95 create ...
//  ***********************************************************************************
//  Function    :   UTL_DRM_StartPlayback
//  Description :   Start the DivX DRM file playback
//  Arguments   :   None
//  Return      :   DRM_PLAY_STATE_CONTINUE, the playback is ok
//                  DRM_PLAY_STATE_ABORT, Rental Expired or Authorized Error happen,
//                                        So, stop the playback
//  Notice      :   Can be called after UTL_DRMPlaybackCapability()
//  ***********************************************************************************
BYTE UTL_DRM_StartPlayback(void)
{
#ifndef NO_DRM
    DWORD   dwTemp;

    dwTemp=PARSER_DRM_CommitPlayback();
#ifdef  DEBUG_DRM
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DRM Play: %lx\n", dwTemp);
#endif  // #ifdef DEBUG_DRM

    switch(dwTemp)
    {
    // No Error
    case    DRM_ERROR_NONE:
        break;

    // Rental Expired
    case    DRM_ERROR_RENTAL_EXPIRED:
        // Show "Rental Expired" screen
        OSDDIVX_DrawDialog(DLG_RENTAL_EXPIRED);
        return (DRM_PLAY_STATE_WAIT);

    // Authorized Error
    case    DRM_ERROR_NOT_AUTH_USER:
    default:
        // Show "Authorized Error" screen
        OSDDIVX_DrawDialog(DLG_AUTHOR_ERROR);
        return (DRM_PLAY_STATE_WAIT);
    }

    // Save total 48 DRM memory information.
    //HAL_WriteStorage(DRM_ADDR_DATA_BASE, (BYTE *)&__PARSER_gbl.DRMMemory, DRM_DATA_LEN);

#endif  // #ifndef NO_DRM

    return (DRM_PLAY_STATE_CONTINUE);
}

//Aron2.36a created, check the events in the while loop
//  ********************************************************************************
//  Function    :   UTL_EventTrigger
//  Description :   check the events in the while loop
//  Arguments   :   None
//  Return      :   None
// Note : Can add the something which is needed to trigger in the while loop
// ********************************************************************************
void UTL_EventTrigger(void)
{
    extern void MEDIA_MonitorTOPOPEN(void);
    extern DWORD __dwTimerCount_Keyscan;

    MONITOR_CheckWatchDog();
    // LLY2.38, porting CoCo's code to include servo souce related code
#ifdef  SUPPORT_SERVO_SOURCE
    MEDIA_MonitorTOPOPEN();
#endif  // #ifdef SUPPORT_SERVO_SOURCE
    if((__bISRKey==KEY_NO_KEY)&&((OS_GetSysTimer()- __dwTimerCount_Keyscan)>COUNT_300_MSEC))
    {
        __bISRKey = PANEL_KeyScan();
        PANEL_Trigger(0);
        __dwTimerCount_Keyscan = OS_GetSysTimer();
    }
}


// LLY2.53, porting Chuan's code if unzip and debug share the same buffer
#ifndef DECOMPRESS_METHOD_GZIP
typedef INT     (*PUNZIP2006_DECODE)(PBYTE pSrc, PBYTE pDest, PBYTE pWorkMem);
#define     UNZIP2006_DECODE   (*((PUNZIP2006_DECODE) (DS_UNZIP_STARTADDR)))
#endif
//  ************************************************************************
//  Function    :   UTL_Decompress
//  Description :   Do decompress procedure
//  Arguments   :   pSrc, the source address
//                  pDest, the destination address
//                  pWorkMem, the working memory address
//  Return      :
//  ************************************************************************
int UTL_Decompress(PBYTE pSrc, PBYTE pDest, PBYTE pWorkMem)
{
    INT iRetCode;

    // LLY2.55, add error protection while user give incorrect working memory by 0
    if(pWorkMem==NULL)
    {
        // force to set it as default un-zip working memory
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
            "Err: unzip working memory is zero, force to set as default\n");
        pWorkMem=(PBYTE)DS_UNZIP_BUF_ST;
    }

#ifdef UNZIP_SHARE_WITH_DBG_BUFFER
    // Disable Serial Debug if unzip and debug share same buffer
    if(pWorkMem == (PBYTE)DS_DEBUG_BUF_ST)
    {
        DBG_Disable();
    }
#endif  // #ifdef UNZIP_SHARE_WITH_DBG_BUFFER

    iRetCode = UNZIP2006_DECODE(pSrc, pDest, pWorkMem);

#ifdef UNZIP_SHARE_WITH_DBG_BUFFER
    // Restore the Serial Debug after unzip ok.
    if(pWorkMem == (PBYTE)DS_DEBUG_BUF_ST)
    {
        // SYuan 2.77, give 2nd parameter since new API updated.
        //UTL_Config_DebugMode(UTL_DBG_INIT);
        UTL_Config_DebugMode(UTL_DBG_INIT, 0);
    }
#endif  // #ifdef UNZIP_SHARE_WITH_DBG_BUFFER

    return iRetCode;
}


// wyc2.53-909P,
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
BYTE UTL_StreamPause(BYTE bStream_id)
{
    //DWORD dwCheckTime;
    static DWORD    dwStartCount = 0, dwStreamID;
    DWORD   dwAudioRem;

    SrcFilter_StreamPause(bStream_id);

//    dwCheckTime = OS_GetSysTimer();
//    while ( (OS_GetSysTimer() - dwCheckTime) <= (COUNT_1_SEC) )
    {
        OS_YieldThread( );
        dwAudioRem = MCU_A0_BS_BUF_REMAINDER;
        if (PARSER_QueryNoneInputBufferReady(bStream_id) == TRUE)
        {
printf( "bStream_id (%lx) read (%lx) write (%lx)", (DWORD) bStream_id,
        __SF_SourceGBL[bStream_id].dwReadSector, __SF_SourceGBL[bStream_id].dwWriteSector );
if( dwStartCount != 0 )
{
    printf( "Recover OK (%lx) ID (%lx)", dwStartCount, (DWORD) bStream_id );
}
if( bStream_id == 0x01 )
{
    asm volatile ("nop");
}
dwStartCount = 0;
            return TRUE;
        }

//        if( (OS_GetSysTimer() - dwCheckTime) >= COUNT_5_SEC )
        {
//            asm volatile ("nop");
        }
    }

if( dwStartCount == 0 )
{
    printf( "{%lx} Audio Rem (%lx) read (%lx) write (%lx)", (DWORD) bStream_id, dwAudioRem, __SF_SourceGBL[bStream_id].dwReadSector, __SF_SourceGBL[bStream_id].dwWriteSector );
    dwStreamID = (DWORD) bStream_id;
}
else if( dwStreamID != (DWORD) bStream_id )
{
    printf( "=====> Stream ID incorrect (%lx) --> (%lx)", dwStreamID, (DWORD) bStream_id );
}
dwStartCount ++;

//DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "stream pause fail: %lx %lx", __SF_SourceGBL[bStream_id].dwReadSector, __SF_SourceGBL[bStream_id].dwWriteSector);

    return FALSE;
}

BYTE UTL_StreamContinue(BYTE bStream_id)
{
    SrcFilter_StreamContinue(bStream_id);
    return TRUE;
}
#endif // #ifdef SUPPORT_JPEG_AUDIO_PLAYING

// Aron2.78-909P created
//  ************************************************************************
//  Function    :   UTL_ProcNavError
//  Description :  Let Navigation do not issue KEY_STOP directly
//                       Just report the error case. the non-protect module decide how to process it.
//  Arguments   :   bErrType, the type of error
//  Return      :
//  ************************************************************************

void UTL_ProcNavError(BYTE bErrType)
{
    switch(bErrType)
    {
        case DVD_LOAD_VTS_FAIL:
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DVD_LOAD_VTS_FAIL\n");
                break;
                
        case DVD_LOAD_TTPGC_FAIL:
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DVD_LOAD_TTPGC_FAIL\n");
                break;

        case DVD_NEXT_LINK_ERR:
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DVD_LOAD_TTPGC_FAIL\n");
                __bFWKey = KEY_NEXT;
                break;
                
        default:
                break;
    }
}

#endif  // #ifndef SIMP_UTL

