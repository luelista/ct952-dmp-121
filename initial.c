//////////////////////////////////////////////////////////
// ******  Big modification History Area  ******
// LLY0.60, remove un-necessary item for CT909
// - MSG_THUMBNAIL_PAGE
// - SHOW_6_DISPLAY_MODE
// - SUPPORT_AV_SYSTEM
// - Replace NULL with '0'
// - MSG_AUTOUPGRADE
// - _CheckUpgradeCD()
// - SUPPORT_DVD_T
// - CT908_UI

// LLY0.62, remove the un-necessary item
// - __bISRKeyRead: since we will use __bISRKey & __bFWKey two variables now

// LLY1.15, remove un-necessary item for Code Reduce
// - __dwTimeError: since don't support bitstream error report mechanism now.
// - _bCountDVD_Initial: since don't support DVD_Initial fail recover mechanism now
// - __bISRKeyPrev: since it was rename to _bISRKeyPrev, and initial it within Interrupt.c now.
// - __bColorMode: since it will be done within GDI_Initial
// - __bDigestStyle: since current digest mode is kept @ __DigestUIAttr.bDigestMode

// LLY2.04, remove the un-necessary variables, ex.
// - temp global variables: _bINITIALTemp, _dwINITIALTemp, _wINITIALTemp
// - _dwResourceNeedProtect
// - __bInitialStatus

//////////////////////////////////////////////////////////////
// Global define for INITIAL module
// if define it, only support simple INITIAL MODULE -- LLY0.61
#ifdef  ECOS909
#define SIMP_INITIAL
#endif  // #ifdef   ECOS909


//////////////////////////////////////////////////////////////
// *****  Include header file Area  *****
// The file "winav.h" must be included in first line
#include "winav.h"
#include "hal.h"
#include "chips.h"
#include "initial.h"
#include "utl.h"
#include "SrcFilter.h"
#include "input.h"
#include "cc.h"
#include "infofilter.h"
#include <string.h>
#include "ctkav.h"
#include "jpegdec.h"
#include "comdec.h"
#include "parser.h"
//#include "mpeg2dec.h"
#include "disp.h"
#include "subpict.h"
#include "int_gbl.h"
#include "atapi.h"
#include "digest.h"

#ifndef SIMP_INITIAL
#include "navigate.h"
#include "setup.h"
#include "linear.h"
#include "dvdinfo.h"
#include "osd.h"
#include "thumb.h"
#include "gdi.h"
#include "panel.h"  // LLY0.60, since using PANEL_Output()
#include "media.h"
#include "osdfont.h"
// wyc0.95, need to include monitor.h
#include "monitor.h"
#include "aploader.h"  // LLY1.50, for using AP related information.
#include "cell.h"

#ifdef  CT950_STYLE //CoCo.38
#include "rtcdrv.h"
#include "alarm.h"
#endif //CT950_STYLE

#ifdef SUPPORT_POWERON_MENU //CoCo2.56
#include "poweronmenu.h"
#endif //SUPPORT_POWERON_MENU


//Alan1.10, support char-based subpicture
#ifdef SUPPORT_CHAR_SUBPICTURE
#include "char_subpict.h"
#endif

// wyc1.01, include usbsrc header files.
#ifdef SUPPORT_USB_SOURCE
#include "usbsrc.h"
#endif // #ifdef SUPPORT_USB_SOURCE


#ifndef NO_MP3
#include "Mmanager.h"
#endif  // #ifndef NO_MP3

#endif  // #ifndef SIMP_INITIAL

//CoCo1.02, add OSD game from XuLi's code
#ifdef SUPPORT_OSDGAME
#include "GameMain.h"
extern EN_GAME_ID  __enGameID;
#endif

// LLY0.61, define the un-support APIs as null for SIMP_INITIAL
#ifdef  SIMP_INITIAL
#include "ecoscc.h"

// HAL modlue
// LLY0.81, define it as null since it was removed to INFOFILTER module, and it's not included for eCos909.
#define     HAL_DetectChipVersion();

// SETUP module
#define     SETUP_Initial() // LLY0.81, don't give parameter since it was remove from DVD_080.
BYTE        __bSetupMode;

// OSD module
#define     OSD_LoadFontTable(a)
#define     OSD_Initial()
#define     OSD_Output(a, b, c)

// Source Filter module
#define     DISCINFO_NONE           0x00
#define     DISCINFO_ENCRYPT        0x01
#define     DISCINFO_REGION_ERROR   0x02
#define     DISCINFO_DAMAGED        0x04
#define     DISCINFO_UNRECOGNIZE    0x08
#define     DISCINFO_UNPLAYABLE     0x0F
#define     ID_ENDLIST              0x04
BYTE        __bEXTAttr;

// LINER Module
#define     LINEAR_Program(a, b)
#define     LINEAR_PROGRAM_CLEAR    0


// UTL module
#define     UTL_ModeInitial(a)
#define     UTL_GetCDDAFormat(a)
#define     UTL_GETCDDAFORMAT_INITIAL       0
#define     UTL_QueryCapability(a)          0


// THUMB module
BYTE        __bThumbnailMode;

// NAVIGATE module
BYTE        __bDomain;
WORD        __wChapter;
BYTE        __bTitle;
#define     STOP_DOM                    0


// PANEL module
#define     PANEL_Initial()
#define     PANEL_Output(a, b)          // LLY0.91

// INITIAL module
#define    _INITIAL_ShowFirstLOGO()     UTL_ShowLogo()
#define    _INITIAL_gcShowStatus(a)


// VCD20 module
WORD       __wListType;
BIT        __btJumpTiming;

#endif  // #ifdef SIMP_INITIAL


/*
cyg_handle_t    __RTC;
cyg_handle_t    __ClockHandle;
*/

// LLY0.63, define a cyg_flag to keep every thread initial action
FLAG_T      __fThreadInit;

THREAD_INFO  ThreadCTKDVD;
THREAD_INFO  ThreadDec;
THREAD_INFO  ThreadPARSER;
THREAD_INFO  ThreadInfoFilter;
// LLY0.95, only define ATAPI thread stack while support ATAPI interface
#ifdef  SUPPORT_ATAPI_SOURCE
THREAD_INFO  ThreadATAPI;
#endif  // #ifdef SUPPORT_ATAPI_SOURCE
// wyc1.01, for USB thread.
#ifdef  SUPPORT_USB_SOURCE
THREAD_INFO  ThreadUSBSRC;
#endif  // #ifdef SUPPORT_ATAPI_SOURCE

WIN_PARAMETER   PIPVideoInfo;
WIN_COLOR       MainBGColor;
WIN_COLOR       PIPBGColor;
WIN_COLOR       PIPBorderColor;

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
// wyc2.75-909P, enlarge the usbsrc stack size
#define CTKDVD_THREAD_STACK_SIZE            (4096)
#define PARSER_THREAD_STACK_SIZE            (2560)
// wyc2.22-909s, the stack size 2560 is not safe in infofilter,
// check the size and will use about 2600 in one disc.
// So set to 3072 for safer.
#define INFO_FILTER_THREAD_STACK_SIZE       (3072)//(2560)
#else  // #if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
#define CTKDVD_THREAD_STACK_SIZE            (8192)
#define PARSER_THREAD_STACK_SIZE            (4096)
#define INFO_FILTER_THREAD_STACK_SIZE       (4096)
#endif // #if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
// LLY2.31, porting Chuan's code to reduce stack size
// since we will allocation any space for AVI decoding
// Notice: must co-work w/ >= v2.31 decoder and DRAM layout table
#define DECODER_THREAD_STACK_SIZE           (8192)
#define ATAPI_THREAD_STACK_SIZE             (2048)
// wyc2.71-portusb
// wyc2.75-909P, reduce the usbsrc stack size
#if (USB_SYSTEM == USB2_0_JUNGO)
// wyc2.78-909P, enlarge the usbsrc stack because moving the usb init from cc to usbsrc thread.
//#define USBSRC_THREAD_STACK_SIZE            (3500) // when support Jungo USB
#define USBSRC_THREAD_STACK_SIZE            (5120) // when support Jungo USB
#else
#define USBSRC_THREAD_STACK_SIZE            (2048)
#endif //


CHAR    __cCTKDVDStack[CTKDVD_THREAD_STACK_SIZE];
CHAR    __cDecStack[DECODER_THREAD_STACK_SIZE];
CHAR    __cPARSERStack[PARSER_THREAD_STACK_SIZE];
CHAR    __cInfoFilterStack[INFO_FILTER_THREAD_STACK_SIZE];
// LLY0.95, only define ATAPI thread stack while support ATAPI interface
#ifdef  SUPPORT_ATAPI_SOURCE
CHAR    __cATAPIStack[ATAPI_THREAD_STACK_SIZE];
#endif  // #ifdef SUPPORT_ATAPI_SOURCE
// wyc1.01, define the stack array for USB thread
#ifdef  SUPPORT_USB_SOURCE
CHAR    __cUSBSRCStack[USBSRC_THREAD_STACK_SIZE];
#endif  // #ifdef SUPPORT_ATAPI_SOURCE


// LLY0.61, Reserve a buffer for SP palette
BYTE    __bSPPaletteBuffer[64];

// LLY2.14, porting Aron's code to support STB mode
#ifdef SUPPORT_STB
#include "usb.h"
#include "sysmenu_op.h"
extern BYTE __bMediaSelected;
extern BYTE __bPreMediaMode;
extern void _ResetSuspendMedia(void);
#endif // #ifdef SUPPORT_STB

/////////////////////////////////////////////////////////////
//  Extern other module's global variable Area
extern  BYTE __bLOGO;
// wyc1.01, extern variable.
extern BYTE _bOpen;
////////////////////////////////////////////////////////////
// *****  Define some global variable Area  *****//
// [1] Tolerance time for reading sector
//     In CDIF path: must give the tolerance value for servo seeking, ex 5 sector
//     In ATAPI path: it's unnecessary, the value alwasy "zero"
// LLY2.80, it's necessary to give the default value, because it will be set
//          within ReadTOC() & UTL_ReadSector()
// [G-4bytes]
//DWORD   __dwTimeTorelence; //= 0;

//DWORD   _dwINITIALECOSVer;

////////////////////////////////////////////////////////////
// ***** Extern other module's global variable Area ***** //

// Brian.170, for SW detect MIC & auto vocal
#ifdef  ENABLE_AUTOVOCAL
extern DWORD __dwMICDectectInterval;
extern bit   _btPreMICDetect;
extern BYTE  _bMICOffCount;
#ifdef  SW_MIC_DETECT
extern bit __sbMICDetect;
#endif  // #ifdef  SW_MIC_DETECT
#endif  // #ifdef  ENABLE_AUTOVOCAL
// wyc2.02-909S,
extern DWORD __dwMMJPGBuffEmptyTime;


// Micky1.24, make sure that servo send correct dvdinfo data
// used for DVDINFO reference
// bit 4=0, ISO_9660 must be correct
// bit 0=0, DVDINFO must be correct
BYTE __bDataRequest;

// Ming0.80, support multi-disc resume.
#if defined( SUPPORT_MULTI_DISC_RESUME)//||defined(SUPPORT_STB)
BYTE __pbDiscUID[3];    // Use __pbDiscUID to save the unique ID for every VCD and DVD.
BYTE __fSaveResumeInfo; // Do not save the RESUME info. if it is in the password input mode.
// LLY1.00, it's un-necessary to keep if restore resume information flag
//BYTE __fRestoreResumeInfo;
Multi_Resume_Info   __Resume_Info;
#endif  // #ifdef SUPPORT_MULTI_DISC_RESUME

#ifdef AUTO_TEST
void _INITIAL_ShowPNO(void);
void _INITIAL_Clear_PNO_Region(void);
#endif

// Eric20070518, print startup time
#ifdef SUPPORT_SERVO_SOURCE
extern void SRVQA_MpegCallServoPrint(BYTE bMode);
#endif //

#ifdef SUPPORT_POWER_ON_DISPLAY_LOGO
BYTE   _bPowerOnFlag=TRUE;
#endif

//  ***********************************************************************
//  Function    :   INITIAL_RunSystem
//  Description :  The first thing for to do when system running up.
//                      Now, only creat and start the CTKDVD thread
//  Argument    :   None
//  Return      :   None
//  ***********************************************************************
//Micky_PORTING_OSWRAP
void INITIAL_RunSystem(void)
{

    // Micky2.11a
    HAL_StartSystem();
    /*
    // LLY2.04, porting Chuan's code
    REG_PLAT_PRESCALER_RELOAD = 132/2;

    // LLY1.05, always enable cache power saving feature
    REG_PLAT_CACHE_CONTROL |= CACHE_DCACHE_POWER_SAVING;

    // J500CSC_111, clear the semaphore at first start
    MACRO_PLAT_KEY_UNLOCK( );
    */
#ifdef STACK_OVERFLOW_DETECTION
    _INITIAL_ClearStack();
#endif // #ifdef STACK_OVERFLOW_DETECTION

    // LLY0.60, Check with CSC if put this statement into eCos system
    // LLY1.05, remove data control burst fetch control
    // Since it will be done within eCoS system
    //REG_PLAT_CACHE_CONTROL |= CACHE_DATA_BURST_FETCH;

    ThreadCTKDVD.priority = 5;

    // WinDVD thread should be Thread_CTKDVD that declared in CC.
    ThreadCTKDVD.priority = 5;
    ThreadCTKDVD.fpThreadEntry=  Thread_CTKDVD;
    ThreadCTKDVD.dwEntryData = 0;
    ThreadCTKDVD.ThreadName = "WinDVD";
    ThreadCTKDVD.StackBase =  (void *)__cCTKDVDStack;
    ThreadCTKDVD.StackSize = CTKDVD_THREAD_STACK_SIZE;
    OS_CreateThread(&ThreadCTKDVD);
    OS_ResumeThread(ThreadCTKDVD.handle);
}



#ifdef STACK_OVERFLOW_DETECTION
//  ***********************************************************************
//  Function    :   _INITIAL_ClearStack
//  Description :   Clear each stack w/ the magic number
//  Arguments   :   None
//  Return      :   None
//  ***********************************************************************
void _INITIAL_ClearStack(void)
{
    memset(__cCTKDVDStack, STACK_MAGIC_NUMBER, CTKDVD_THREAD_STACK_SIZE);
    memset(__cDecStack, STACK_MAGIC_NUMBER, DECODER_THREAD_STACK_SIZE);
    memset(__cPARSERStack, STACK_MAGIC_NUMBER, PARSER_THREAD_STACK_SIZE);
    memset(__cInfoFilterStack, STACK_MAGIC_NUMBER, INFO_FILTER_THREAD_STACK_SIZE);
#ifdef  SUPPORT_ATAPI_SOURCE
    memset(__cATAPIStack, STACK_MAGIC_NUMBER, ATAPI_THREAD_STACK_SIZE);
#endif  // #ifdef  SUPPORT_ATAPI_SOURCE

#ifdef  SUPPORT_USB_SOURCE
    memset(__cUSBSRCStack, STACK_MAGIC_NUMBER, USBSRC_THREAD_STACK_SIZE);
#endif  // #ifdef  SUPPORT_USB_SOURCE

#ifdef SUPPORT_SERVO_SOURCE
    {
        extern char stack[2][4096];
        memset(stack[0], STACK_MAGIC_NUMBER, 4096);
        memset(stack[1], STACK_MAGIC_NUMBER, 4096);
    }
#endif  // #ifdef SUPPORT_SERVO_SOURCE

}
#endif  // #ifdef STACK_OVERFLOW_DETECTION


//  *********************************************************************
//  Function    :   INITIAL_System
//  Description :   Some steps for Player wake-up
//                  [1] uP [2] Variables [3] PANEL/OSD [4] Servo
//                  [5] MPEG Decoder [6] TV Encoder [7] Audio DAC
//  Arguments   :   NONE
//  Return      :
//  Side Effect :
//  *********************************************************************
// LLY.171-2, re-arrange the procedure for INITIAL_System()
// Give the parameter, merge some procedure into HAL_PowerDown()
void INITIAL_System(BYTE bMode)
{
    // Micky2.11a
    BYTE    bStatus;
    // uP and global variables initial action only need to do once time
    // while 1st H/W power on
    if(bMode & HARDWARE_SHUTDOWN)
    {
        // LLY1.01, remove "thread flag initial" procedure from INITIA_Variables_PowerON() to here
        // Because, servo thread will be create/ resume within _servo_source_init()
        // And, the flag will be set within it.
        OS_InitFlag (&__fThreadInit);

        // LLY2.18, initial the mutex for I2C and Expand GPIO share same pin issue.
        HAL_I2CSrc_Ctrl(MUTEX_ACTION_INIT);

#ifdef SUPPORT_SERVO_SOURCE
        {
        // Brian0.69
        // Step 0: Initialize Servo source
        // (a) dowlaod servo ucode ==> according to test, downlaod servo ucode (access DSSP reg 0xE8) may fail after Processor_2 is running
        //      so, we do it here, before INITIAL_uProcessor
        // (b) Create and resume servo threads

        extern void _servo_source_init(void);
        extern void BMST_SetServoClock(BYTE bSrvClkTick, DWORD bSrvMlk);

        // LLY2.22, set system tick to servo
        // Notice: must set it before servo thread wake up.
        // LLY2.30, give mclk value @ 2nd parameter
        BMST_SetServoClock(SYSTEM_TICK, CPU_SPEED);

        _servo_source_init();
        }
#endif // #ifdef SUPPORT_SERVO_SOURCE

        //Micky_PORTING_OSWRAP
        OS_InitialTimer();

        // LLY2.38, porting CoCo's code to initial RTC for CT951 system
#ifdef  CT950_STYLE
        RTC_Initial();
#endif  //CT950_STYLE

        // Micky2.11a, modify INITIAL_uProcessor into HAL_InitPlatForm.
        // Step 1: Initialize uP
        //DVD_051Micky, initial uP should be done first
        //INITIAL_uProcessor ();
        bStatus = HAL_InitPlatForm();
        if ( !bStatus )
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nINIT Platform error code :: %hx", bStatus);
        }
        // wyc0.95, initial the alarm interrupt for monitor and debug.
        MONITOR_AlarmInit();

        // LLY2.15, config debug mode, and remove old DBG_Init() calling
        UTL_Config_DebugMode(UTL_DBG_INIT, 0);

        // LLY2.10, disable UART2
        // LLY2.13, remove this procedure, since no reason to disable it.
        //HAL_UART_Disable(HAL_UART2);

        // LLY2.05, give NULL to set customer pre-selected IR
        INPUT_InitIR(NULL);

#ifdef SUPPORT_SERVO_SOURCE
        {
            extern void _servo_init_alarm(void);

            _servo_init_alarm();
        }
#endif  // #ifdef SUPPORT_SERVO_SOURCE

        // Step 2: Initialize all global variables
        // LLY2.80, Don't put this procedure too late !!
        // Otherwise, something maybe wrong if following API
        // must refer those variable within it !!
        _INITIAL_Variables_PowerON ();

        // LLY2.11, initialize serial flash as earily as possible
        // ie, it must be done before R/W serial flash action.
#if (STORAGE_TYPE == SERIAL_FLASH)
        {
        extern  void    HAL_InitSerialFlash(void);

        HAL_InitSerialFlash();
        }
#endif  // #if (STORAGE_TYPE == SERIAL_FLASH)


    }

    // Something need to do for 1st H/W power and total shutdown
    if( !(bMode & PSEUDO_SHUTDOWN) )
    {
        // Step 3: Initialize some necessary pio pin
        // Need check if it's necessary for CT909 ??


        // Step 4: Initial MPEG chip

        // Step 5: Power up all HW Block

        // Set MainVideo frame buffer
        // LLY2.17, collect frame buffer setting procedure into UTL_Config_FrameBuffer()
        UTL_Config_FrameBuffer(UTL_FRAMEBUF_MOTION);
        /*
#ifdef SUPPORT_FBR_MODE // Frame buffer redundancy
        DISP_FrameBufferSet(DS_FRAMEBUF0_ADDR_ST_FBR, DS_FRAMEBUF1_ADDR_ST_FBR, DS_FRAMEBUF2_ADDR_ST_FBR, DS_FRAMEBUF0_ADDR_ST_FBR, Y_MAXSIZE_PAL_FBR * 8);
#ifdef SUPPORT_BFR_MODE // B-Frame redundancy
        DISP_FrameBufferMode(TRUE, TRUE);
#else  // #ifdef SUPPORT_BFR_MODE
        DISP_FrameBufferMode(TRUE, FALSE);
#endif // #ifdef SUPPORT_BFR_MODE
#else  // #ifdef SUPPORT_FBR_MODE
        DISP_FrameBufferSet(DS_FRAMEBUF0_ADDR_ST, DS_FRAMEBUF1_ADDR_ST, DS_FRAMEBUF2_ADDR_ST, DS_FRAMEBUF0_ADDR_ST, Y_MAXSIZE_PAL * 8);
        DISP_FrameBufferMode(FALSE, FALSE);
#endif // #ifdef SUPPORT_FBR_MODE
        */

        // Set VCR and border color
        DISP_BGColorSet(MainBGColor, DISP_MAIN_BG);
        DISP_MainVideoSet(&MainVideoInfo, __dwCoded_Picture_Width, __dwCoded_Picture_Height, DISP_NEXTVSYNC);

        // LLY1.02, set default PIP background and border color
        DISP_BGColorSet(PIPBGColor, DISP_PIP_BG);
        DISP_BGColorSet(PIPBorderColor, DISP_PIP_BORDER);


        // Step 5: detect chips version
        HAL_DetectChipVersion();

        //Kevin0.85, I/O mute to avoid the H/W power-on bo noise
        // When outputing PCMWS to ADAC for the first time
        HAL_IOMute(TRUE);

#ifndef SIMP_INITIAL
#ifndef    POWERON_STANDBY
        {
        extern STATUS    gcShowStatus;
        gcShowStatus.bPowerDown= FALSE;     // let PANEL can show information
        }
#endif  // POWERON_STANDBY

        // LLY2.76, porting SY's code to support power switch feature
#ifdef SUPPORT_POWER_SWITCH
        if(!__bPowerSw_StandBy)
        {
            extern STATUS    gcShowStatus;
            gcShowStatus.bPowerDown= FALSE;     // let PANEL can show information
        }
#endif  //SUPPORT_POWER_SWITCH

#endif  // #ifndef SIMP_INITIAL

#ifndef NO_PANEL
        // Step 6: Initial Panel ...
        // Micky0.91, panel is controlled by GPIO
        // must wait till detect base port OK.
        PANEL_Initial();    // KCHong 0.90
#endif  // #ifndef NO_PANEL


        // Step 7: Turn-off TV encoder to avoid "un-stable TV screen" appear
        // Must add after MPEG chip waking-up,
        // Otherwise, the action maybe fail if it use "GPIO" to control TV encoder
        // LLY1.50, HAL_TurnOffTVE() is rename as TVE_TurnOff()
        //HAL_TurnOffTVE();
        HAL_TVE_Off();

        // Step 8: Initial TFT related setting if support TFT -- Alan1.06
#ifdef  SUPPORT_TFT
        TFT_Init();
#endif  // #ifdef SUPPORT_TFT

    }

    // Step 8: Read-back some necessary setup info. while 1st H/W power on
    if(bMode & HARDWARE_SHUTDOWN)
    {
        SETUP_Initial();

#ifdef  CT950_STYLE //CoCo.38
        ALARM_Initial();
        AUTOPWR_Initial();
#endif  //CT950_STYLE

        // LLY1.50, check if switch to AP mode
#ifdef  POWERON_SWITCH_AP
        if(__SetupInfo.bAPMode != 0)
        {
//            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AP Mode: %lx\n", __SetupInfo.bAPMode);

            if(AP_SwitchMode(__SetupInfo.bAPMode)==FALSE)
            {
                BYTE bMode=0;
                // Notice: need restore the AP mode to DVD mode while AP switch fail
                HAL_WriteStorage(SETUP_ADDR_AP_MODE, &bMode, 1);
            }
        }
#endif  // #ifdef POWERON_SWITCH_AP

        // LLY0.95, read-back all DRM related information.
#ifndef NO_DRM
        PARSER_DRM_Config((DWORD)SETUP_ADDR_DIVX_BASE);
        PARSER_DRM_PowerOnInit();
#endif  // #ifndef NO_DRM
    }

    // Step 9: Merge the necessary thing to HAL_PowerDown()
    //         Ex. MPEG chip, TV encoder, Audio DAC, Servo, SCART ...
    HAL_PowerDown(bMode);
}


//  *********************************************************************
//  Function    :   INITIAL_PowerONStatus
//  Description :
//  Arguments   :   NONE
//  Return      :
//  Side Effect :
//  *********************************************************************
// DVD_100Micky, merge I/R power on sequence with h/w power on.
void INITIAL_PowerONStatus(BYTE bPower)
{
    DWORD   dwThreadDone;

    // Create desired thread and let it to start !!
    // Notice: it only need for for 1st power on
    if (bPower==INITIAL_POWER_HARDWARE)
    {
        INITIAL_ThreadInit(THREAD_ATAPI);
        // wyc1.01, initial USB thread.
        INITIAL_ThreadInit(THREAD_USBSRC);
        INITIAL_ThreadInit(THREAD_INFO_FILTER);

        // LLY0.72, always create the MPEG thread for MPEG LOGO display
        INITIAL_ThreadInit(THREAD_MPEG_DECODER);
        INITIAL_ThreadInit(THREAD_PARSER);
        // LLY0.61, Source Filter is not defined a thread.
        //INITIAL_ThreadInit(THREAD_SRC_FILTER);
    }

    // Initial MPEG chip ...
    HAL_PowerOnInit();

    // DVD_100Micky, only hardware power on need check power on standby mode
    if (bPower==INITIAL_POWER_HARDWARE)
    {

#ifdef  POWERON_STANDBY
        // Let F/W enter POWER-DOWN mode immediately
        __bFWKey= KEY_POWER;
        return;
#else   // #ifdef  POWERON_STANDBY
        // LLY2.76, porting SY's code to support power switch feature
#ifdef SUPPORT_POWER_SWITCH
        if (__bPowerSw_StandBy)
        {
            __bFWKey= KEY_POWER;
            return;
        }
#endif // #ifdef SUPPORT_POWER_SWITCH

#endif  // #ifdef POWERON_STANDBY
    }

    if(bPower==INITIAL_POWER_HARDWARE)
    {
        // LLY1.01, calculate the desired thread done flag for different source while H/W power on
        dwThreadDone=INIT_POWER_ON_THREAD_X_SOURCE_DONE;
#ifdef  SUPPORT_ATAPI_SOURCE
        dwThreadDone |= INIT_SRC_THREAD_ATAPI_DONE;
#endif  // #ifdef SUPPORT_ATAPI_SOURCE

#ifdef  SUPPORT_SERVO_SOURCE
        dwThreadDone |= (INIT_SRC_THREAD_SERVO1_DONE | INIT_SRC_THREAD_SERVO2_DONE);
#endif  // #ifdef SUPPORT_SERVO_SOURCE

        // LLY1.10,remove code to polling USB thread done
        // Since, current F/W only create it, but don't resume it from 1.08
#if 0//def  SUPPORT_USB_SOURCE
        dwThreadDone |= (INIT_SRC_THREAD_USB_DONE);
#endif  // #ifdef SUPPORT_USB_SOURCE

        // LLY0.95, the waiting time is absolute time
        // LLY0.68, wait power-on necessary thread initial done or leave while time-out
        //cyg_flag_timed_wait( &__fThreadInit, INIT_POWER_ON_THREAD_DONE, CYG_FLAG_WAITMODE_AND, COUNT_50_MSEC);
        OS_TimedWaitFlag( &__fThreadInit, dwThreadDone, FLAG_WAITMODE_AND,
            (COUNT_50_MSEC) );

        // LLY0.70, print-out the warning message while flag initial not complete.
        if( (OS_PeekFlag(&__fThreadInit) & dwThreadDone)!=dwThreadDone )
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Some thread not initial done. Desired: %lx, Current: %lx\n", dwThreadDone, OS_PeekFlag(&__fThreadInit));
        }

        // Micky_PORTING_OSWRAP
         //MACRO_ECOS_VER(_dwINITIALECOSVer);
        //if (LOWORD(_dwINITIALECOSVer) < ECOS_PACKAGE_VERSION)
        if  (!OS_Identify_Version() )
        {
            // Micky2.11a, remove _dwINITIALECOSVer
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nECOS Version Error");//, %lx!!",_dwINITIALECOSVer);
            OSD_LoadFontTable(FONTABLE_OSD);
            OSD_Initial();
            OSD_Output(MSG_ERRCODE, 500, 0xFF);
            while(1);
        }
    }

    // Show logo earily.
    _INITIAL_ShowFirstLOGO();

    // Load font table and do OSD initial first for power-on standby mode
    OSD_LoadFontTable(FONTABLE_OSD);
    OSD_Initial();

    // Show looading message here after OSD_Initial() done ...
    // Notice: this message can't be show while tray is not close -- LLY2.00
#ifdef SUPPORT_STB
    if( __bChooseMedia == MEDIA_SELECT_DVD && __bMediaSelected == TRUE)
#endif
    if (SrcFilter_CheckTrayStatus () == SRC_TRAY_CLOSED)
    {
#ifndef SUPPORT_POWERON_MENU //CoCo2.38
        OSD_Output ( MSG_WAITING, 0, 0xFF );
#endif //SUPPORT_POWERON_MENU
    }

//LJY1.23, per KWD's request
#ifdef SET_VIDEO_OUTPUT_WHEN_POWER_ON
    SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT); //KWD-1.21c for Norcent to fix video output blue background color
#endif  // #ifdef SET_VIDEO_OUTPUT_WHEN_POWER_ON


    SrcFilter_Reset(FALSE); //not show loading status bar

    // LLY0.80, remove it since it's un-necessary for CT909
    // Since all data will be read-back during 1st SETUP_Initial()
    //SETUP_Initial(SETUP_OTHERS_INITIAL); //CoCo.051

    INITIAL_Variables ();   // ** S-TCH; move from INITIAL_System

    CHIPS_Initial(CHIPS_INIT_ALL);


#ifdef  SIMP_INITIAL

    INITIAL_InitialState();

#else   // #ifdef SIMP_INITIAL

#ifdef  NO_DISC_MODE

	// Chuan2.77, add new case for power down Servo part if servo code isn't exist
#ifndef SUPPORT_SERVO_SOURCE
    HAL_PowerControl(HAL_POWER_SERVO, HAL_POWER_SAVE);
#endif	// #ifndef SUPPORT_SERVO_SOURCE

#else   // #ifdef NO_DISC_MODE
    // the INITIAL_InitialState is removed
    // will let MEDIA_Management do it.
    // force tray closed at power on
    // Micky0.72, add for servo tray is opened when power on. begin
  	__SF_SourceGBL[0].bSourceIndex = SOURCE_DISC;

    // wyc1.01, set _bOpen = 0 because power on will close tray in H/W and IR power on.
    // If not set to 0, then _MEDIA_MonitorServoStatus won't check tray closed status
    // and won't update __btDriveOpen to FALSE.
    // Then, INFOFTR will not parse information because of __btDriveOpen == TRUE.
    // Notice: must check where to clear _bOpen is better
    //         since it maybe incorrect for Top Loader platform while power on w/o tray close
    _bOpen = 0 ;
    if (SrcFilter_CheckTrayStatus () != SRC_TRAY_CLOSED)
    {
		_dwTimeCheckTray = OS_GetSysTimer();
        __btDriveOpen = TRUE;
        SrcFilter_CloseTray();
    	__bMediaServoState = MEDIA_SERVO_NONE;//CLOSED;

    }
    else
	{
        __bMediaServoState = MEDIA_SERVO_CLOSED;
	}
    // Micky0.72, add for servo tray is opened when power on. end

    __bMediaServoAction = MEDIA_SERVO_ACTION_CLOSE;
#endif  // #ifdef NO_DISC_MODE


    // LLY0.76-1, JY says we must give a default value for servo buffer
    // Otherwise, it will out-of range while disc reading.
    // LLY2.01, set servo/ parser dump in buffer as normal while power-on
    // LLY2.20, rename it as UTL_Config_BufferMode()
    // LLY2.51, porting CoCo's code to config buffer mode as video mode @ CT95x solution
#ifdef CT950_STYLE
    UTL_Config_BufferMode(DUMP_BUFF_MM_VIDEO_ONLY_MODE);
#else
    UTL_Config_BufferMode(DUMP_BUFF_NORMAL_MODE);
#endif //CT950_STYLE

#endif  // #ifdef SIMP_INITIAL


//DVD_049MickyPowerDown
#ifdef  AUTO_POWERDOWN
    // set the initial time stamp for checking.
    __dwTimeWaitPowerDown = OS_GetSysTimer(); //__dwCountSystem;
#endif  // #ifdef AUTO_POWERDOWN

#ifdef AUTO_TEST
//Alan2.20, add the following functions for auto-test
    _INITIAL_ShowPNO();
#endif
}


// LLY0.60 create ...
//  ***************************************************************************************
//  Function    :   INITIAL_ThreadInit
//  Description :   Create the desired thread and wake-up it
//  Argument    :   bThread, specify the desired thread
//  Return      :   None
//  Notice      :
//  ***************************************************************************************
//Micky_PORTING_OSWRAP
void INITIAL_ThreadInit(BYTE bThread)
{
    // Create the desired thread
    switch(bThread)
    {
#ifdef  SUPPORT_ATAPI_SOURCE
    case    THREAD_ATAPI:
   ThreadATAPI.priority = 5;
    ThreadATAPI.fpThreadEntry=  ATAPI_Thread;
    ThreadATAPI.dwEntryData = 0;
    ThreadATAPI.ThreadName = "ATAPI";
    ThreadATAPI.StackBase =  (void *)__cATAPIStack;
    ThreadATAPI.StackSize = ATAPI_THREAD_STACK_SIZE;
    OS_CreateThread(&ThreadATAPI);

        OS_ResumeThread(ThreadATAPI.handle);
        break;
#endif  //  #ifdef  SUPPORT_ATAPI_SOURCE

    // wyc1.01, create USB source thread.
#ifdef  SUPPORT_USB_SOURCE
    case    THREAD_USBSRC:
   ThreadUSBSRC.priority = 5;
    ThreadUSBSRC.fpThreadEntry=  USBSRC_Thread;
    ThreadUSBSRC.dwEntryData = 0;
    ThreadUSBSRC.ThreadName = "USBSRC";
    ThreadUSBSRC.StackBase =  (void *)__cUSBSRCStack;
    ThreadUSBSRC.StackSize = USBSRC_THREAD_STACK_SIZE;
    OS_CreateThread(&ThreadUSBSRC);
          break;
#endif  //  #ifdef  SUPPORT_ATAPI_SOURCE

#ifdef  SUPPORT_SERVO_SOURCE
#endif  // #ifdef SUPPORT_SSERVO_SOURCE

    case    THREAD_PARSER:
        ThreadPARSER.priority = 5;
        ThreadPARSER.fpThreadEntry=  PARSER_ThreadMain;
        ThreadPARSER.dwEntryData = 0;
        ThreadPARSER.ThreadName = "Parser";
        ThreadPARSER.StackBase =  (void *)__cPARSERStack;
        ThreadPARSER.StackSize = PARSER_THREAD_STACK_SIZE;
        OS_CreateThread(&ThreadPARSER);
        OS_ResumeThread(ThreadPARSER.handle);
        break;

#ifndef SIMP_INITIAL
    case    THREAD_INFO_FILTER:
        ThreadInfoFilter.priority = 5;
        ThreadInfoFilter.fpThreadEntry=  INFOFILTER_Thread;
        ThreadInfoFilter.dwEntryData = 0;
        ThreadInfoFilter.ThreadName = "InfoFilter";
        ThreadInfoFilter.StackBase =  (void *)__cInfoFilterStack;
        ThreadInfoFilter.StackSize = INFO_FILTER_THREAD_STACK_SIZE;
        OS_CreateThread(&ThreadInfoFilter);
        OS_ResumeThread(ThreadInfoFilter.handle);
        break;
#endif  // #ifndef SIMP_INITIAL

#if 0
    case    THREAD_SRC_FILTER:
        ThreadSrcFilter.priority = 5;
        cyg_thread_create(ThreadSrcFilter.priority,
            SRCFILTER_Thread,
            (cyg_addrword_t) 0,
            "SrcFilter",
            (void *) __cSrcFilterStack,
            SRC_FILTER_THREAD_STACK_SIZE,
            &ThreadSrcFilter.handle,
            &ThreadSrcFilter,space);
        cyg_thread_resume(ThreadSrcFilter.handle);
        break;
#endif  // #if 0


    case    THREAD_MPEG_DECODER:
        // LLY0.72, delete other video decoder thread first, ex. DivX, JPEG
        // Since, only one decoder thread will exist at the same time.
        INITIAL_ThreadDelete(THREAD_DIVX_DECODER);
        INITIAL_ThreadDelete(THREAD_JPEG_DECODER);
        ThreadDec.priority = 4;
        ThreadDec.fpThreadEntry=  MPEG2_ThreadMain;
        ThreadDec.dwEntryData = 3;
        ThreadDec.ThreadName = "Dec_MPEG";
        ThreadDec.StackBase =  (void *)__cDecStack;
        ThreadDec.StackSize = DECODER_THREAD_STACK_SIZE;
        OS_CreateThread(&ThreadDec);
        OS_ResumeThread(ThreadDec.handle);
        break;

    case    THREAD_DIVX_DECODER:
        // LLY0.72, delete other video decoder thread first, ex. MPEG, JPEG
        // Since, only one decoder thread will exist at the same time.
        INITIAL_ThreadDelete(THREAD_MPEG_DECODER);
        INITIAL_ThreadDelete(THREAD_JPEG_DECODER);

        // Create thread - DivX
        ThreadDec.priority = 4;
        ThreadDec.fpThreadEntry=  DivX_ThreadMain;
        // LLY2.31, porting Chuan's code to give extra buffer address for AVI decoding
        // Notice: Must co-work w/ >= v2.31 decoder and DRAM layout table
        ThreadDec.dwEntryData = DS_SIDEINFO_BUF_ST;
        ThreadDec.ThreadName = "Dec_DivX";
        ThreadDec.StackBase =  (void *)__cDecStack;
        ThreadDec.StackSize = DECODER_THREAD_STACK_SIZE;
        OS_CreateThread(&ThreadDec);
        OS_ResumeThread(ThreadDec.handle);
        break;

    case    THREAD_JPEG_DECODER:
        // LLY0.72, delete other video decoder thread first, ex. DivX, MPEG
        // Since, only one decoder thread will exist at the same time.
        INITIAL_ThreadDelete(THREAD_MPEG_DECODER);
        INITIAL_ThreadDelete(THREAD_DIVX_DECODER);

        // Create thread - JPEG
        ThreadDec.priority = 5;
        ThreadDec.fpThreadEntry=  JPEG_ThreadMain;
        ThreadDec.dwEntryData = 3;
        ThreadDec.ThreadName = "Dec_JPEG";
        ThreadDec.StackBase =  (void *)__cDecStack;
        ThreadDec.StackSize = DECODER_THREAD_STACK_SIZE;
        OS_CreateThread(&ThreadDec);
        OS_ResumeThread(ThreadDec.handle);
        break;
    }
}
//  LLY0.72 create ...
//  ********************************************************************************
//  Function    :   INITIAL_ThreadDelete
//  Description :   Delete the desired thread if it exist.
//                  Also release the related resource before the thread deleted
//  Arguments   :   bThread, specify the desired thread
//  Return      :   TRUE, the thread is deleted ok
//                  FALSE, the thread delete action fail
//  ********************************************************************************
BYTE INITIAL_ThreadDelete(BYTE bThread)
{
    switch(bThread)
    {
    case    THREAD_DIVX_DECODER:
        // Delete the DivX thread if it exist
        if( (OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_DIVX_DONE) )
        {
            // [1] call DivX thread exit API

            // [2] suspend the thread
            OS_SuspendThread(ThreadDec.handle);

            // [3] kill the desired thread
            OS_KillThread(ThreadDec.handle);

            // [4] clear the thread initial done flag
            OS_ClearFlag( &__fThreadInit, ~INIT_DEC_THREAD_DIVX_DONE );
        }
        return (TRUE);

    case    THREAD_MPEG_DECODER:
        // Delete the MPEG thread if it exist
        if( (OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_MPEG_DONE) )
        {
            // [1] call MPEG thread exit API to release resource
            MPEG2_ThreadExit(0);

            // [2] suspend the thread
            OS_SuspendThread(ThreadDec.handle);

            // [3] kill the desired thread
            OS_KillThread(ThreadDec.handle);

            // [4] clear the thread initial done flag
            OS_ClearFlag( &__fThreadInit, ~INIT_DEC_THREAD_MPEG_DONE );
        }
        return (TRUE);

    case    THREAD_JPEG_DECODER:
        // Delete the JPEG thread if it exist
        if( (OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_JPEG_DONE) )
        {
            // [1] call JPEG thread exit API to release resource
            JPEG_ThreadExit( 0);

            // [2] suspend the thread
            OS_SuspendThread(ThreadDec.handle);

            // [3] kill the desired thread
            OS_KillThread(ThreadDec.handle);

            // [4] clear the thread initial done flag
            OS_ClearFlag( &__fThreadInit, ~INIT_DEC_THREAD_JPEG_DONE );
        }
        return (TRUE);

    default:
        return (FALSE);

    }

    return (FALSE);
}


//  *********************************************************************
//  Function    :   INITIAL_Variables
//  Description :   The initalization of all variables
//  Arguments   :   NONE
//  Return      :   NONE
//  Side Effect :   The effect will be the OPEN-CLOSE level
//  *********************************************************************
// DVD_172Micky, FLOW_CONTROL
// called when 1) H/W power on, 2) I/R power on, 3) I/R power off, 4) Open tray.
void INITIAL_Variables(void)
{
    // Kevin0.85, remove the setting for _bSampleFreq,
    // Since it's un-necessary to set the freq again when open->close or power off->on
    //_bSampleFreq = AUDIO_FREQ_NONE;

    // LLY2.36, porting elmer's code to reset File system as none
    // while open/ close and IR power.
    __bFileSystem = FILE_SYSTEM_NONE;

    // LLY2.38, base on CoCo's suggest to remove this variables since new mechanism don't reference it.
#if 0
#ifdef  PRESTOP_BEFORE_SETUP_AND_PROGRAM
    __bPreCountStop=0;   // Alan2.02
#endif  // #ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
#endif  // #if 0

    __bDiscInfo= DISCINFO_NONE; // ** TCH1.70;

//DVD_049MickyPowerDown
#ifdef  AUTO_POWERDOWN
// support auto power down
    // reset the checking time
    __dwTimeWaitPowerDown =0xFFFFFFFF;
#endif  // #ifdef AUTO_POWERDOWN

//CoCo1.02, add OSD game from XuLi's code
#ifdef SUPPORT_OSDGAME
	__enGameID =  GAME_NONE;
#endif

    __bZoomFactor=VIEW_NORMAL;
    __wXStart=0;
    __wYStart=0;

    __bModeCmd= KEY_STOP;
    __btPlaying= FALSE;
    __btPause= FALSE;

    //Micky2.80, review global variables init.
    __dwTimeKey = 0;
    __dwNumberPrevious = 0;
#ifdef  SUPPORT_BOOKMARK
    __btBookMark = FALSE;
#endif  // #ifdef SUPPORT_BOOKMARK
    // wyc1.24a-2, because this __bSearchMode will be reset in UTL_ModeInitial. So don't need to reset it here.
    //__bSearchMode = 0;

// Micky1.06, remove unnecessary codes, as will be cleared within INITIAL_MediaSetting
/*
    // Micky1.10, support resume after power on for DVD/CDDA/VCD
    // must keep the __dwResumeTime value
    // Ming0.80, support multi-disc resume.
#if defined(SUPPORT_RESUME_AFTER_POWERON) || defined(SUPPORT_RESUME_AFTER_OPENCLOSE)
    if ( !( __bFlowState & FLOW_KEEP_RESUME ) )
#endif  // #if defined(SUPPORT_RESUME_AFTER_POWERON) || defined(SUPPORT_RESUME_AFTER_OPENCLOSE)
    __dwResumeTime = 0;
*/

//LJY1.05, per CoCo's request for module testing
#ifdef AUTO_TEST
    CC_AutoTestInitial();
#endif
    // LLY.100a, reset audio L/R status to STEREO mode
    __bModeChannel= AUDIO_STEREO;

    // DVD_274pMicky, let CHIPS do initial.
    //__bVocal= VOCAL_ON; // ** TCH1.54; Re-Init the value
    CHIPS_Initial(CHIPS_INIT_AC3KOKMODE);

    //Kevin0.90, fix "KEY UP/DOWN for other tiles and then play MP3 => no audio & OSD fails"
    CHIPS_Initial(CHIPS_INIT_KEY);

    // wyc.170, initial the de-emohasis variable
    __bHALDeEmphasis = FALSE;

    __bAudioType = HAL_AUDIO_NONE;  //Kevin2.37-2

    // wyc.pro clear the program array and variables
    LINEAR_Program(LINEAR_PROGRAM_CLEAR, 0);
    UTL_ModeInitial ( MODE_OPENCLOSE );

    __bModePlay= MODE_PLAYUNKNOW;
    // wyc1.25-2, initial this value
    //LLY0.72, __bLastModePlay= MODE_PLAYUNKNOW;
    __bScan= SCAN_NONE;

    // LLY.275a, clear thumbnail mode
    __bThumbnailMode= FALSE;

#ifndef NO_DISC_MODE
    //Aron2.17-909S, config DVD information buffer address
    __dwFWBufUsedSize = DVD_InfoBufSet(DS_FW_BUFFER_ST, (DS_FW_BUFFER_END - DS_FW_BUFFER_ST));

     __bDomain= STOP_DOM;
     __wChapter= 1;
    __bTitle= 1;


    // DVD_161Micky, fix SVCD-> can't pause
    {
    extern BYTE  __bStillTime;   // record the Still Time (0: NONE, 0xFF: Infinite; Other: seconds.)
    extern  BYTE __bVOBUStill;
    __bVOBUStill=FALSE;
    __bStillTime=0;
    }

    //Aron2.31-909R, add for init navigation error concealment parmaters
    NV_SetErrorConParm(MAXLOADVTSFAILTCNT, SERVOREADYTIMEOUT);

#endif  // #ifndef NO_DISC_MODE

    // ** S-TCH; Reset Some
    __bASTNO = __bASTN = __bASTID = 0;          // ** TCH0.451;
    __bSPSTNO = __bSPSTN = __bSPSTID = 0;       // ** TCH0.451;
    // Chuan0.75, Initial SP2 ID & NO
    __bSPST2NO = 0;
    __bSPST2ID = 0xFF;

     __wDiscType= NO_DISC_IN_DRIVE; // ** LLY.043, remove from above code

    // LLY.161, clear CDDA format to unknown while open/close
    __bCDDA_DTS=CDDA_UNKNOWN;

    // DVD_099Micky, clear repeat status as repeat none
    // LLY0.80, it's un-necessary to call OSD_Output() w/ zero time-out to clear message
    // Since OSD moudle will provide OSD_NormalMode() to handle it.
    // LLY2.38, porting CoCo's code not to reset repeat mode value while support image frame setup
#ifndef REPEAT_MODE_REF_SETUP_INFO
    __bRepeat=REPEAT_NONE;
#endif  // #ifndef IMAGE_FRAME_SETUP
    //OSD_OUTPUT_MACRO(MSG_REPEAT, __bRepeat, 0);

    __btMute= FALSE;
    //OSD_OUTPUT_MACRO(MSG_MUTE, FALSE, 0);

#ifndef NO_DIGEST
    __btDIGEST= FALSE;
#endif  // #ifndef NO_DIGEST


    // DVD_101bMicky, the initial CountStop shall be real stop
    __bCountStop=2;

// DVD_103Micky, turn on __bAutoVocal when defined is on
#ifdef  ENABLE_AUTOVOCAL
    __bAutoVocal=1;

    // Brian.170, SW MIC Detect
#ifdef  SUPPORT_MIC_DETECT_INV
    __sbMICDetect=0;
    _btPreMICDetect=0;
#else   // #ifdef SUPPORT_MIC_DETECT_INV
    __sbMICDetect = 1;
    _btPreMICDetect= 1;
#endif  // #ifdef SUPPORT_MIC_DETECT_INV
    __dwMICDectectInterval = 0;
    _bMICOffCount=0;

#endif  // #ifdef ENABLE_AUTOVOCAL

    __bEXTAttr = 0;
    // wyc1.10-DVDR, default set to FALSE.
    __bDVDROM = FALSE;


    // Micky2.79-2, support FullScreen function
    // init. value 0 will be assigned within INITIAL_Variables.
    // 1: means full screen video on, 0: means full screen video off.
    __bFullScreen=0;

    __bScanLevel = 0;
    HAL_WriteAM(HAL_AM_FF_SPEED, __bScanLevel);
#ifndef SIMP_INITIAL
    // wyc1.07, rename some variables and initial their value in initial.c
    {
    extern void MONITOR_InitialValue(void);
    MONITOR_InitialValue();
    }

#endif  // #ifndef SIMP_INITIAL

    // LLY0.65, initial the source filter relative variables
    SrcFilter_Init();

    // LLY0.60, it's un-necessary to set audio threshold for pause/ skip command
    // or video master mode
    // Because, it will be removed to HAL_InitAudio() for CT909


    // ** TCH1.00-908; begin...     It's a default value for disable Video Master Mode.
    HAL_ControlAVSync (HAL_AVSYNC_VIDEOMASTER, 0);


    //kevin1.07, really disable MV when IR power off and Open
    /*
    {
        extern  BYTE    _bAPSType;
        _bAPSType=0; //Kevin1.07a, clear when open
    }
    */
    HAL_TVSetAPS(0);

//Alex1.21,20031210 support bookmark
//Clear _wBookMark Time and Title value
#ifdef SUPPORT_BOOKMARK
    __wBookMark[0]=1;
    for(__bTemp=0;__bTemp<BOOKMARK_NO;__bTemp++)
    {
        __wBookMark[__bTemp+1]=0;
        __bBookMark_Title[__bTemp+1]=0;
//        __wBookMark_Chapter[__bTemp+1]=0;
    }

#endif//#ifdef SUPPORT_BOOKMARK

    // Ming0.80, support multi-disc resume.
    // LLY2.14, porting Aron's code to support STB system
#if defined( SUPPORT_MULTI_DISC_RESUME) //||defined(SUPPORT_STB)
    {
        extern BYTE __fSaveResumeInfo;

        __fSaveResumeInfo = TRUE;
    }

#endif // #ifdef SUPPORT_MULTI_RESUME


    // LLY1.02, reset the previous play item as unknown
    __wPrevPlayItem=0xFFFF;
    __wPrevPlayDir=0xFFFF;


    // LLY1.02, clear the SPU control to fix HLI don't disappear
    // while playback DVD title "Nero" Menu --> Open/Close tray
    // Notice: must remove to do this initial during 1st H/W power on
    //         after F/W will do CC_KeyCommand(KEY_STOP) while open/close
    __SPUCtrl.bEnable = 0;

//Alan1.10, support char-based subpicture
#ifdef SUPPORT_CHAR_SUBPICTURE
    CHAR_SP_Initial();
#endif

    // wyc2.36a-909s,
#ifdef SUPPORT_FUSS_MPEG_CD
    INFOFILTER_FUSSMPEGCD(IF_MODE_CLEAR_MODE, 0);
#endif //
    // wyc2.55-909P-AP, init it to notify infofilter to parse playable files.
    INFOFILTER_SetParseMode(IF_PARSE_FILE_MODE);

#ifdef  NO_DISC_MODE
    __SF_SourceGBL[__dwSFStreamID].bSourceIndex = SOURCE_USB0_0;
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    __SF_SourceGBL[BG_ITEM_STREAM_ID].bSourceIndex = SOURCE_USB0_0;
#endif //
#else   // #ifdef NO_DISC_MODE
    //Aron2.17-909S, set bSourceIndex = SOURCE_DISC as default
    //Otherwise, it will not config servo dump-in buffer when switching from USB
    __SF_SourceGBL[__dwSFStreamID].bSourceIndex = SOURCE_DISC;
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    __SF_SourceGBL[BG_ITEM_STREAM_ID].bSourceIndex = SOURCE_DISC;
#endif //
#endif  // #ifdef NO_DISC_MODE

    // LLY2.01, set servo/ parser dump in buffer as normal while open/close
    // LLY2.20, rename it as UTL_Config_BufferMode()
    // LLY2.51, porting CoCo's code to config buffer mode as video only for CT95x solution
#ifdef CT950_STYLE
    UTL_Config_BufferMode(DUMP_BUFF_MM_VIDEO_ONLY_MODE);
#else
    UTL_Config_BufferMode(DUMP_BUFF_NORMAL_MODE);
#endif //CT950_STYLE

    // LLY2.51, porting YC's code to config MM Init Menu
    MM_ConfigMMInitMenu(MM_INITIAL_MENU_NORMAL);
#ifdef SUPPORT_ENCODE_JPG_PICTURE
    // LLY2.51, porting YC's code to initial Encoder File
    MM_EncodeFile_Init();
#endif //

    // LLY2.36, config video display error rate
    // It will skip the picture while this picture is destroyed > 1/DEC_ERROR_RATIO
    HAL_PlayCommand(COMMAND_V_ERROR_RATIO, DEC_ERROR_RATIO);
    return ;
}


//  *********************************************************************
//  Function    :   _INITIAL_PowerON_Variables
//  Description :   ....
//  Arguments   :   NULL
//  Return      :
//  Side Effect :   Only H/W Power-ON once time
//  *********************************************************************
void _INITIAL_Variables_PowerON(void)   // ** TCH1.60-1;
{
    // LLY2.79-2, assign __bISRKey as KEY_NO_KEY again after INITIAL_uProcessor()
    // to avoid unknown 132 H/W IR interrupt
    // Fix "MSG_INVALID" appear while H/W power on by 132 H/W IR
    // Notice: Can be removed if find the reason of abnormal H/R IR interrupt
    __bISRKey=KEY_NO_KEY;

    __bFWKey=KEY_NO_KEY;


    // wyc1.25, add one variable to record flow when detected as wrong disc for debug usage,
    // And, initial to 0 first.
    __bTestFlow = 0;


    ///////////////////////////////////////////////////////////////
    // [CC] Module
    //LJY1.22, Per Micky's request
    _CC_Variables_PowerON();

    ///////////////////////////////////////////////////////////////
    {   // [UTL] Module
        extern  DWORD       _dwTimePrev;
        extern  BYTE        __bCDDAAutoChTrk;
        extern  DWORD   _dwUTLNextTriggerCount;  // ** TCH1.70-2; record SysTimer for next Trigger.

        _dwTimePrev= 0;
        // wyc.172-CDDA, add a flag to represent CDDA is play across tracks automatically.
        __bCDDAAutoChTrk = FALSE;
#ifndef SIMP_INITIAL
        __bAttrPlay=ATTR_NONE;
        __bAttrPlayNew=ATTR_NONE;
#endif  // #ifndef SIMP_INITIAL
        // LLY.274p-3, initialize this value, otherwise, the OSD_Trigger() can't be called
        // within UTL_OutputShowTime() because UTL_GetSysTimer() can't larger than this value (8051 system)
        // And, OSD message always can't time-out
        _dwUTLNextTriggerCount=0;

        // Micky2.80, review global variables init.
        __bCheckAudioType=CHECK_NONE;
        __btCheckVideoType=FALSE;
        __bModePlayPreCDROM= MODE_PLAYUNKNOW;
        _dwTriggerShowTime= 0;
        _dwGap_TriggerShowTime= (COUNT_500_MSEC+ COUNT_200_MSEC);
    }

    // LLY0.60, integrate W99AV and HAL variable initial action together
    HAL_InitVariables();

    MainVideoInfo.bEnable = TRUE;
    MainVideoInfo.sHStart = 117;
    MainVideoInfo.sVStart = 46;
    MainVideoInfo.wHWidth = 720;
    MainVideoInfo.wVWidth = 480;

    MainBGColor.Color.bWeight = 0x0;
    MainBGColor.Color.bY = 0x0;
    MainBGColor.Color.bU = 0x80;
    MainBGColor.Color.bV = 0x80;

    // LLY0.76-1, give the default value for PIP frame size (independent of digest mode)
    // Just for un-expect usaga.
    PIPVideoInfo.bEnable = FALSE;
    PIPVideoInfo.sHStart = 0;
    PIPVideoInfo.sVStart = 0;
    PIPVideoInfo.wHWidth = 360;
    PIPVideoInfo.wVWidth = 240;

    // LLY1.02, give the default value for PIP background and border color
    PIPBGColor.Color.bWeight = 0x0;
    PIPBGColor.Color.bY = 0x0;
    PIPBGColor.Color.bU = 0x80;
    PIPBGColor.Color.bV = 0x80;

    PIPBorderColor.Color.bWeight = 0x0;
    PIPBorderColor.Color.bY = 0x0;
    PIPBorderColor.Color.bU = 0x80;
    PIPBorderColor.Color.bV = 0x80;


    __dwCoded_Picture_Width = 720;
    __dwCoded_Picture_Height = 480;

    // LLY0.65, initial the flag as NONE for all thread initial done status
    // LLY1.01, can't put the thread initial flag initial here since it's too late.
    // Because, Servo thread initial flag are put before INITIAL_Variables_PowerON() now.
    //cyg_flag_init (&__fThreadInit);

    // LLY0.95, initial the DISP flag base on Chuan suggestion.
    // OS_InitFlag (&__fDISPInfo);

    // Initialize parser module and global variables -- LLY0.61
    PARSER_ThreadInit( );

    // LLY1.00, initialize JPEG decoder thread global variable
    JPEG_ThreadInit();

#ifdef  SIMP_INITIAL
    // Initial default source index as ATAPI or SERVO for SIMP_INITIAL
    // Notice: must base on supported source type -- LLY0.67
    // For read DVD system, it will be done by source filter
#ifdef  SUPPORT_ATAPI_SOURCE
    __SF_SourceGBL[0].bSourceIndex = SOURCE_ATAPI;
#endif  // #ifdef  SUPPORT_ATAPI_SOURCE

#ifdef  SUPPORT_SERVO_SOURCE
    __SF_SourceGBL[0].bSourceIndex = SOURCE_SERVO;
#endif  // #ifdef  SUPPORT_SERVO_SOURCE


    // Assign the pre-define disc type
    __wDiscType = DISC_TYPE;

    // Assign playing mode attribute
    // Notice: must be put together with UTL_SetPlayMode()
    __bAttrPlayNew = ATTR_PLAY;

#endif  // #ifdef SIMP_INITIAL


    /////////////////////////////////////////////////////////////
    // wyc.171, moves global variables initial here to reduce the code size in common bank
#ifndef NO_DIGEST //LJY1.20, support 4M flash, remove digest
    //LJY1.10, reduce extern global vars for INITIAL module.
    {
        DIGEST_VarInit();
    }
#endif

#ifndef NO_DISC_MODE
    ///////////////////////////////////////////////////////////////
    // Chuan2.80 VCD20 Module
    {
        extern WORD __wListType;
        extern BIT __btJumpTiming;
        __wListType = ID_ENDLIST;
        __btJumpTiming = 0;
    }

    // LLY2.17, configure NV buffer start address
    // since the value maybe different base on different DRAM configuration
    __dwNVStart = DS_NV_BUF_ADDR_ST;

#endif  // #ifndef NO_DISC_MODE

    //////////////////////////////////////////////////////////////
    // [SETUP] Module
    {
        // Initial __bSetupMode as FALSE
        // Because, the initial action is removed from SETUP module
        // Fix "can't playing title while 1st H/W power on"
        // No bitstream can be sent into buffer within new CC procedure.
        __bSetupMode=FALSE;
    }

    /////////////////////////////////////////////////////////////
    // [THUMB] Module
    {
        // Give default value as FALSE
        // Otherwise, user will see "unknown char" first
        // while 1st H/W power --> 1st call OSD_Output(MSG_XXX, XXX)
        __bThumbnailMode= FALSE;
    }

    //[CHIPS], LJY1.10
    {
        CHIPS_Initial(CHIPS_INIT_ALL);
    }

    // Brian0.69
    SrcFilter_InitVariables();

#ifdef SUPPORT_WATCHDOG_BURNIN
    // elmer2.78, support burn-in testing
    {
        extern DWORD __dwWatchdogTime;

        __dwWatchdogTime = OS_GetSysTimer();
    }
#endif

}


//  **********************************************************************************
//  Function    :   INITIAL_CheckServo
//  Description :   Check the SERVO. Decide which kind of title that puts in Drive
//  Arguments   :   NONE
//  Return      :   ERR_NOERR
//  Side Effect :
//  **********************************************************************************
WORD INITIAL_CheckServo(void)
{
    // For SIMP_INITIAL, do servo related action and assign a pre-define disc type
    // For real DVD system, all things will be handled by media.c
#ifdef  SIMP_INITIAL

    extern   BOOL    INFOFILTER_GetDiscTypeFromSource(VOID);

    // ** S-TCH; first suppose no disc in SERVO
    __wDiscType= NO_DISC_IN_DRIVE;

    // Micky1.05-2, Fix Loading-> Setup-> exit setup-> will detect as wrong disc
    // must clear __bDiscInfo for each title detection.
    __bDiscInfo= DISCINFO_NONE;

    // check if Drive is OPEN ot CLOSE
    if ( __btDriveOpen )
    {
        // wyc.172r, remove this code for not doing close command twice to avoid the result of mode select command to make
        // DVD title detection fail.
        // wyc.273-1, make __bCloseCommand =1 when close by extra force.
        extern BYTE __bCloseCommand ;
        __bCloseCommand = TRUE;
        __btDriveOpen= FALSE;
    }

    OSD_OUTPUT_MACRO ( MSG_WAITING, 0, 0xFF );

    // Check the exist of CD-Drive
    if (SRCFilter_GetMediaStatus() != SRCFILTER_HAVEDISC)
    {
        // stop loader for wrong disc or no disc
        // wyc1.25, add one variable to record flow when detected as wrong disc for debug usage,
        SrcFilter_PowerMode (SERVO_POWER_STOP) ;
        __bTestFlow |= 0x1;
    }
    else
    {
        // Servo Initialization: Identify -> Init -> Check Disc and Wait Stable -> ReadTOC
        // Reset CDROM and Read TOC
        if ( ! INFOFILTER_GetDiscTypeFromSource () )
        {
            __wDiscType=NO_DISC_IN_DRIVE;
            // wyc2.53-909P,
            SrcFilter_Stop(__dwSFStreamID);
            return ERR_ERROR;
        }
    }

    // Assign the pre-define disc type
    __wDiscType = DISC_TYPE;

    // Assign playing mode attribute
    // Notice: must be put together with UTL_SetPlayMode()
    __bAttrPlayNew = ATTR_PLAY;

    if( (__wDiscType & BOOK_DVD) || (__wDiscType & BOOK_SVCD) )
    {
        HAL_SetAudioDAC ( AUDIO_FREQ_48K );
    }
    else
    {
        HAL_SetAudioDAC ( AUDIO_FREQ_44K );
    }

#endif  // #ifdef SIMP_INITIAL

    _INITIAL_gcShowStatus(TRUE);

    if ( __wDiscType == NO_DISC_IN_DRIVE )
    {
        // DVD_172Micky, FLOW_CONTROL begin
        // Assume the no disc is interruptted(WaitStable) by SETUP,
        // just go ahead do the SETUP, don't need show no_disc too.
        // but need re-checking disc after exit setup.
#ifdef  SUPPORT_LOADING_SETUP
        if (__bISRKey == KEY_SETUP)
        {
            __bFlowState |= FLOW_RELEASE_DETECTION;
            return ERR_NOERR;
        }
#endif  //#ifdef  SUPPORT_LOADING_SETUP
        // else really no disc.
        // DVD_172Micky, FLOW_CONTROL end

        // Micky1.10, must clear all trigger state when no disc or invalid disc
        // Ex. FLOW_KEEP_RESUME/FLOW_RELEASE_DETECTION-> power on->
        // encouner title detection error or no disc
        __bFlowState = FLOW_NORMAL;

        __bTemp= 0xFF;
    }
    else
    {
        __bTemp= 3;
    }

    // LLY.161, let DISCTYPE info display later while CDDA title
    // DVD_170Micky, must show abnormal disc for encrypt disc.
    // now CD-DA disc type will be shown later, as need distinguish CDDA or DTS CDDA
    // DVD_172Micky, FLOW_CONTROL begin
    // show disctype except for CD-DA, as need distinguish CDDA or DTS.
    if ( !(__wDiscType & BOOK_CDDA) )
    {
        // if unplayable bit is set(unrecoganize/invalid region code...)
        // don't show MSG_DISCTYPE, show ABNORMAL_DISC
        if (__bDiscInfo & DISCINFO_UNPLAYABLE)
        {
            OSD_OUTPUT_MACRO(MSG_ABNORMAL_DISC, __bDiscInfo, 0xff);

            // Micky1.10, must clear all trigger state when no disc or invalid disc
            // Ex. FLOW_KEEP_RESUME/FLOW_RELEASE_DETECTION-> power on->
            // encouner title detection error or no disc
            __bFlowState = FLOW_NORMAL;
            //LJY1.25, fix MP3 erroneous playback when it detects wrong disc.
            // Micky1.25-2, can't clear DVD when region code error
            if ( !(__wDiscType & BOOK_DVD) )
                __wDiscType = BOOK_UNKNOW;

            //CoCo2.56, return to power on menu if no files in the USB disc.
#ifdef SUPPORT_POWERON_MENU
            // Delay a while let use can see "no playback file" message
            OS_DelayTime(COUNT_500_MSEC);

            // Exit USB source mode
            MEDIA_ExitUSBSRC();

            // Re-draw power-on menu.
            POWERONMENU_Initial();
            return ERR_NOERR;
#endif //SUPPORT_POWERON_MENU
        }
        else
        {
            OSD_OUTPUT_MACRO (MSG_DISCTYPE, __wDiscType, __bTemp );

            // Eric20070518, print startup time
#ifdef SUPPORT_SERVO_SOURCE
            SRVQA_MpegCallServoPrint(0x01);
#endif //

            // wyc2.02-909S,
            // wyc0.80, need to wait 1s, otherwise user can't see the disc type OSD and will update soon by MM UI.
            if (__wDiscType & CDROM_M1)
            {
                __dwMMJPGBuffEmptyTime = OS_GetSysTimer();
                UTL_GetPlayRange(1);
                SrcFilter_Seek(__dwTimeBegin, FALSE);
            }
        }
    }
    // DVD_172Micky, FLOW_CONTROL end

    HAL_SetDiscType(__wDiscType);

    //LLY.049, set correct A/V buffer size while playing MP3 w/ LOGO, ie.
    //nobody will set it in original control
    // LLY0.72, remove it since no related thing will be done for CT909
    //HAL_InitVideo();

    // LLY.161, display DISCTYPE info after get real CDDA format: pure/DTS
    if(__wDiscType & BOOK_CDDA)
    {
        // LLY.161-2, must give the parameter
        UTL_GetCDDAFormat(UTL_GETCDDAFORMAT_INITIAL);
        OSD_OUTPUT_MACRO (MSG_DISCTYPE, __wDiscType, 0x3 );
         
        // Eric20070518, print startup time
#ifdef SUPPORT_SERVO_SOURCE
        SRVQA_MpegCallServoPrint(0x01);
#endif //

    }

    // LLY0.68, remove the control flow while fix CD/ JVC-CDD power on resume don't work
    // since it's unknown reason (CT909 maybe double check.)

    // kevin1.05, stop the power on music playing after title detection done.
#ifdef SUPPORT_POWERON_MUSIC
    HAL_WriteAM(HAL_AM_PCM_SCALE, 0);
    HAL_WriteAM(HAL_AM_AUDIO_TYPE, 0);
    HAL_Reset(HAL_RESET_AUDIO); //stop playing power on music
#endif // #ifdef SUPPORT_POWERON_MUSIC

#ifndef SIMP_INITIAL
#ifndef NO_MP3
    // LLY.274p-3, re-think the procedure to prepare 1st Menu Mode:
    // [1] Kodak Picture CD : don't enter Menu Mode
    // [2] MP3/ MP3+JPG/ JPG : enter Menu Mode
    if( !(__bDiscInfo & DISCINFO_UNPLAYABLE) && (__wDiscType & CDROM_M1) ) // playable CDROM title
    {
        // LLY0.84, do File Manager initial action for all CDROM title
        // It will handle if enter menu or not by itself.
        // LLY2.51, porting YC's code since MM_Initial() API is updated.
        MM_Initial(__bMMInitMenu);

        // LLY0.64, don't do pre-parsing of MP3 title for CT909
    }
#endif  // #ifndef NO_MP3
#endif  // #ifndef SIMP_INITIAL

    return ERR_NOERR;
}


//  *********************************************************************
//  Function    :   INITIAL_InitialState
//  Description :   Some steps for VCD Starter
//                  [1] Show LOGO [2] Check title [3] AUTOPLAY check
//  Arguments   :   NONE
//  Return      :
//  Side Effect : Now called by 1) Game Exit, 2) INITIAL_PowerONStatus(H/W or I/R)
//              : 3) 3 cases are called when multi-discs.
//  *********************************************************************
#ifdef  SIMP_INITIAL
void INITIAL_InitialState(void)
{
    __bModePlay= MODE_PLAYUNKNOW;

    _INITIAL_gcShowStatus(FALSE);   // this will clear MSG_WAITING.???

    // Don't do disc initial, directly do KEY_OPEN
    // LLY0.62, using general abort mainflow API instead of checking POWER/ OPEN_CLOSE key
    if( UTL_QueryCapability(QUERY_ABORT_MAINFLOW))
        return;

    INITIAL_CheckServo ();

    //_CC_ImmediateKey();

#ifdef  AUTO_POWERDOWN
    //DVD_099Micky, Open/Close no disc will check auto power down
    __dwTimeWaitPowerDown = OS_GetSysTimer(); //__dwCountSystem;
#endif  //#ifdef  AUTO_POWERDOWN
}
#endif  // #ifdef  SIMP_INITIAL



// Following APIs are only supported while non-simple INITIAL module
#ifndef SIMP_INITIAL
//  *********************************************************************
//  Function    :   _INITIAL_gcShowStatus
//  Description :   Prepare the ALL data for OSD/PANEL module
//  Arguments   :   bFlag   : TRUE means will sends MSG_STOPWAIT to PANEL module
//  Return      :   NONE
//  Side Effect :
//  *********************************************************************
// LLY0.60, remove un-necessary element's initial action since STATUS structure has been modified
// - bDiscNumber
// - wTotalTime
// - bSetProgram, bProgram
// - bAudioChannel
// - bMute
// - bKaraokeLevel
// - bRepeat
// - bRepeatAB
// - bRandom
// - bTVMODE
// - bOsdDisplay
// - bSetTime
// - bOpen
// - bVocal
// - bMP3Mode
// - bSetSearch, bSearch
// - bVolume, bVolumeLevel
// - bKey
// - bEcho, bEchoLevel
// - bSurround
void _INITIAL_gcShowStatus(BYTE bFlag)
{
    STATUS StatusInfo;

#ifndef NO_DISC_MODE
    if ( __wDiscType & BOOK_DVD )
        StatusInfo.bPBCMode= __bNVPlayMode;
    else
#endif  // #ifndef NO_DISC_MODE
        StatusInfo.bPBCMode= __btPBC;       //PBC on/off

    if (__wDiscType&BOOK_VCD)
        StatusInfo.wTotalTracks=__wTrackEnd-1;   //total tracks
    else
        StatusInfo.wTotalTracks=__wTrackEnd;   //total tracks


    StatusInfo.bPlayingMode=MSG_STOP;   //playing mode

    StatusInfo.bStill=FALSE;
    // LLY2.53, expand the time value from WORD to DWORD unit
    // Since, we will use [23:16]:hour, [15:8]:min, [7:0]: sec
    //StatusInfo.wCurrentTime=0;
    StatusInfo.dwCurrentTime=0;
    StatusInfo.wCurrentTrack=0;
    StatusInfo.bPowerDown=__btPowerDown;

    __bpDataPointer=(BYTE *)(&StatusInfo);

    // LLY.172q, it's only "TRUE" or "FALSE" flag to communicate with OSD/ PANEL mode (ARM error)
    //OSD_Output ( MSG_STOPWAIT, (WORD)&StatusInfo, 0 );
    OSD_Output ( MSG_STOPWAIT, TRUE, 0 );

#ifndef NO_PANEL
    if (bFlag)
    {
        PANEL_Output ( MSG_STOPWAIT, TRUE);
    }
#endif  // #ifndef NO_PANEL
}

#ifdef SUPPORT_POWER_ON_DISPLAY_LOGO
void Show_Customer_Logo(void)
{
	if(_bPowerOnFlag)
	{
     		__bLOGO |= LOGO_CHANGE_TV_TYPE;  
    		UTL_ShowLogo();
		_bPowerOnFlag=0;
		__bLOGO=LOGO_NONE;
       	#ifdef LED_BackLight //CoCo2.38          
       	TFT_BackLight(TRUE);
		#endif
		OS_DelayTime(COUNT_2_SEC);
	}
}
#endif


// wyc1.24a, one function to show first time LOGO.
void    _INITIAL_ShowFirstLOGO(void)
{
    // LLY2.77, porting SenShong's code to fix logo display maybe fail while 1st power on
    // Notice: only do this procedure for CT950 solution only.
#ifdef  CT951_PLATFORM
    extern void _ChangeDSPCode(BYTE bType);
    BYTE bIndx;
    HAL_WriteAM(HAL_AM_PLAY_COMMAND, 0);
    for ( bIndx=2; bIndx<17; bIndx++)
    {
        REG_AIU_GR(bIndx)= 0;
    }
    HAL_ReloadAudioDecoder(HAL_AUDIO_MP3);
    HAL_ResetAudioDecoder();
    HAL_WriteAM(HAL_AM_UPSAMPLE_EN, 1);
    HAL_WriteAM(HAL_AM_AUDIO_TYPE, 2);
    HAL_ResetAudioDecoder();
    _ChangeDSPCode(HAL_VIDEO_JPG);
#endif  // #ifdef CT951_PLATFORM

        #ifdef SUPPORT_POWER_ON_DISPLAY_LOGO
    Show_Customer_Logo();
    #endif

    __bLOGO |= LOGO_CHANGE_TV_TYPE;
    UTL_ShowLogo();

#ifdef SUPPORT_POWERON_MUSIC
    // kevin1.05, power on music
    HAL_PlayTone(HAL_TONE_CH_ALL);
#endif  // #ifdef SUPPORT_POWERUP_MUSIC
}



// wyc0.50-909, move from InfoFilter thread to here.
//  ***************************************************************************
//  Function    :   INITIAL_MediaSetting
//  Description :   Do media related setting
//  Argument    :   ??
//  Return      :   None
//  ***************************************************************************
BYTE INITIAL_MediaSetting(void)
{
#ifdef AUTO_TEST
//Alan2.20, add the following functions for auto-test
    _INITIAL_Clear_PNO_Region();
#endif

    // LLY1.00, enable the flow state w/ resume if detect the resume information
    // Notice: can't put the procedure within INITIAL_CheckServo()
    //         because INITIAL_MediaSetting() will be done earier than INITIAL_CheckServo()
#ifdef SUPPORT_MULTI_DISC_RESUME
    if(_Detect_Resume_Information())
    {
        __bFlowState |= FLOW_KEEP_RESUME;
    }
    else
#endif  // #ifdef SUPPORT_MULTI_DISC_RESUME
    {
        // Cannot find the RESUME info. or invalid disc media type.
        __bFlowState = FLOW_NORMAL;
        // Micky1.06, clear __dwResumeTime also
        __dwResumeTime = NULL_0;
    }



    // Initial Multi Subpicture/ Audio/ Angle related variables
    __bASTNO= __bASTN= __bASTID= 0;
    __bSPSTNO= __bSPSTN= __bSPSTID= 0;
    __bAGLN= __bAGL_Ns= 0;

#ifndef NO_DISC_MODE
    // Initial OGT color palette for CVD/ SVCD
    if ( __wDiscType & BOOK_2X )
    {
        // ** TCH0.29; Record real number, not index from "0".
        __bASTNs= 2;
        __bSPSTNs= 4;

        // LLY0.60, using __bSPPaletteBuffer[] to keep SPST palette
        // Initial color palette table for SVCD/ CVD title
        // 1
        __bSPPaletteBuffer [1]= 0xa2; __bSPPaletteBuffer [3]= 0x2c; __bSPPaletteBuffer [2]= 0x80;
        __bSPPaletteBuffer [5]= 0xeb; __bSPPaletteBuffer [7]= 0x80; __bSPPaletteBuffer [6]= 0x80;
        __bSPPaletteBuffer [9]= 0xeb; __bSPPaletteBuffer [11]= 0x80; __bSPPaletteBuffer [10]= 0x80;
        __bSPPaletteBuffer [13]= 0x70; __bSPPaletteBuffer [15]= 0x48; __bSPPaletteBuffer [14]= 0x3a;

        // 5
        __bSPPaletteBuffer [17]= 0x54; __bSPPaletteBuffer [19]= 0xb8; __bSPPaletteBuffer [18]= 0xc6;
        __bSPPaletteBuffer [21]= 0x41; __bSPPaletteBuffer [23]= 0x64; __bSPPaletteBuffer [22]= 0xd4;
        __bSPPaletteBuffer [25]= 0x23; __bSPPaletteBuffer [27]= 0xd4; __bSPPaletteBuffer [26]= 0x72;
        __bSPPaletteBuffer [29]= 0x10; __bSPPaletteBuffer [31]= 0x80; __bSPPaletteBuffer [30]= 0x80;

        // 9
        __bSPPaletteBuffer [33]= 0x6b; __bSPPaletteBuffer [35]= 0x80; __bSPPaletteBuffer [34]= 0x80;
        __bSPPaletteBuffer [37]= 0x93; __bSPPaletteBuffer [39]= 0x83; __bSPPaletteBuffer [38]= 0x2e;
        __bSPPaletteBuffer [41]= 0x22; __bSPPaletteBuffer [43]= 0x7d; __bSPPaletteBuffer [42]= 0x83;
        __bSPPaletteBuffer [45]= 0x61; __bSPPaletteBuffer [47]= 0x3a; __bSPPaletteBuffer [46]= 0x48;

        // 13
        __bSPPaletteBuffer [49]= 0x64; __bSPPaletteBuffer [51]= 0xc3; __bSPPaletteBuffer [50]= 0xbf;
        __bSPPaletteBuffer [53]= 0x41; __bSPPaletteBuffer [55]= 0xd4; __bSPPaletteBuffer [54]= 0x64;
        __bSPPaletteBuffer [57]= 0x33; __bSPPaletteBuffer [59]= 0xe4; __bSPPaletteBuffer [58]= 0x82;
        __bSPPaletteBuffer [61]= 0x46; __bSPPaletteBuffer [63]= 0x23; __bSPPaletteBuffer [62]= 0xd1;

#ifdef  USE_LITTLE_ENDIAN
        {
            int     i;
            DWORD   *pdwPtr;

            pdwPtr = (DWORD *)__bSPPaletteBuffer;

            for(i=0; i<16; i++)
            {
                COMUTL_SwapDWORD(pdwPtr[i]);
            }
        }
#endif  //#ifdef  USE_LITTLE_ENDIAN

        // LLY0.60, clear SPU set color palette API directly for CT909
        SPU_SetPalette((DWORD *)__bSPPaletteBuffer);
    }
    else
#endif  // #ifndef NO_DISC_MODE
    {
        __bASTNs= 0;
        __bSPSTNs= 0;
    }
    // ** TCH0.26; end...


    // LLY0.85, collect the audio DAC setting together after disc detection done.
    // Notice: it's the first time to enable ACLK/ PCMCLK/ PCMWS
#ifndef NO_DISC_MODE
    if ( (__wDiscType&BOOK_DVD) || (__wDiscType&BOOK_SVCD) )
    {
#if AUDIO_DAC == DAC_W571C161  //kevin1.07, avoid W571C161 44K audio (CDDA..) always noise when HW power on
        extern BYTE    _bSampleFreq;
        HAL_SetAudioDAC ( AUDIO_FREQ_48K );
       _bSampleFreq = AUDIO_FREQ_NONE;
#endif
        HAL_SetAudioDAC ( AUDIO_FREQ_48K );
    }
    else
#endif  // #ifndef NO_DISC_MODE
    {
#if AUDIO_DAC == DAC_W571C161
        extern BYTE    _bSampleFreq;
        HAL_SetAudioDAC ( AUDIO_FREQ_44K );
       _bSampleFreq = AUDIO_FREQ_NONE;
#endif
        HAL_SetAudioDAC ( AUDIO_FREQ_44K );
    }

    HAL_ResetAIUPCM();

    // LLY2.18, remove delay since ACLK is initialize as earily as possible (HAL_InitAudio()) now.
    HAL_IOMute(FALSE);

#ifndef NO_DISC_MODE
    if ( __wDiscType & BOOK_DVD )
    {
        // check region code here
        __bInvalidRegion=TRUE;
        // will actually check region code at each play command
        _IdentifyRegionCode();

        // LLY0.85, collect the audio DAC setting together.
        //DVD_051Micky, set audio DAC frequency only.
        //HAL_SetAudioDAC ( AUDIO_FREQ_48K );

        // Is it necessary now because it will be done within UTL_SetPlayMode() ?
        // ** TCH0.362; begin...
        /*
        __bAudioType= HAL_AUDIO_AC3;
        HAL_SetAudioType( __bAudioType );        // ** DVD-TCH0.25;  move from UTL_SetPlayMode()
        */
        // ** TCH0.362; end...

        __bSP_Disp_OFF= FALSE;  // ** TCH0.403;

        // LLY0.61, remove the code because it will be done within INITIAL_Variables()
        /*
        if ( (__bRepeat == REPEAT_TRACK) )      // ** TCH0.25;
            __bRepeat= REPEAT_NONE;     // ** TCH0.46; Deafult is REPEAT_NONE
        */

        // DVD support chapter/title time
        // ** TCH0.38; begin... Force PBC flag is ON when DVD title.
        // Micky2.79, support DVD default PBC off
#ifdef  DEFAULT_NOPBC
        __bNVPlayMode= 0;
#else   // #ifdef DEFAULT_NOPBC
        __bNVPlayMode= 1;
#endif  // #ifdef DEFAULT_NOPBC

        //OSD_OUTPUT_MACRO ( MSG_PBC, __bNVPlayMode, 3 );
        // ** TCH0.38; end...
        // ** TCH0.37;
        ///        NV_TitlePlay ( 1, 1 );

        // DVD_170Micky, for unplayable disc, don't call NV_TitlePlay.
        if (__bDiscInfo & DISCINFO_UNPLAYABLE)
            return ERR_NOERR;

        if (__bShowTimeType < MSG_TITLE_TIME)
        {
            __bShowTimeType = MSG_TITLE_TIME;
        }


        return ERR_NOERR;
    }
    // ** DVD-TCH0.19;
    else    // ** TCH0.25; Non-DVD
#endif  // #ifndef NO_DISC_MODE
    {
        // LLY.274p-3, default is repeat disc for Kodak Picture CD
        if(__wDiscType & CDROM_PICTURECD)
            __bRepeat=REPEAT_DISC;

        // VCD don't support chapter/title time
        if (__bShowTimeType >= MSG_TITLE_TIME)
        {
            __bShowTimeType = MSG_RELTIME;
        }
    }

    if(__wDiscType & CDROM_M1)
    {
    }
    else
    {
#ifndef NO_DISC_MODE
        if ( __wDiscType & BOOK_CDI )    // assume ONE Track
        {
            DWORD   dwTemp;

            // CDI couldn't use CDROM function
            CDINFO_ParseInfo ( DISC_VOL_SPACESIZE, & __dwTimeDiscEnd, 0 );

            CDINFO_ParseInfo ( DISC_LOC_MPATHTABLE, &dwTemp, 0 ); //Rex, 7/9/96
            __dwTimeBegin= CDINFO_SearchStartSector(dwTemp ); //Rex, 7/9/96
            if ( ( ! __dwTimeBegin ) || ( __dwTimeBegin >= 0x00003FFF ) )
                __dwTimeBegin= dwTemp;    // avoid from sector: 0  //Rex, 7/9/96

            __wTrackBegin= 1;
            __wTrackEnd= 1;

            __dwTimeDiscBegin= __dwTimeBegin;
            __dwTimeEnd= __dwTimeDiscEnd;
        }
        else
        {
            BYTE bTemp;
            INFOFILTER_PrepareVCD ( __wDiscType );      // read data from DRAM
            // 1999.6.9. Detect CVD Encryption

            CDINFO_GetTrackNO ( & __bError, & bTemp, & __dwTimeDiscEnd );
            __wTrackBegin=(WORD)__bError;
            __wTrackEnd=(WORD)bTemp;
            SrcFilter_GetDiscLength (& __dwTimeDiscEnd );

            if ( __wDiscType & BOOK_VCD )
            {
                if ( __wTrackEnd >= 2 )
                {
                if ( __wTrackBegin <= 1 )
                    __wTrackBegin= 2;
                }
            }

            CDINFO_GetTrackInfo ( (BYTE)__wTrackBegin, & __dwTimeDiscBegin, &bTemp );
        }
#endif  // #ifndef NO_DISC_MODE
    }

#ifndef NO_DISC_MODE
    if ( __wDiscType & VCD_2_0 )
    {
        WORD    wTemp;
        //  pre-load some information and confirm the exist of VCD 2.0
        wTemp = CDINFO_GetList ( LIST_FIRST, 0 );
        if ( wTemp == 0xFFFF )
        {
            wTemp = CDINFO_GetList ( LIST_FIRST, 0 );
            if ( wTemp == 0xFFFF )
            {
            // Fail to load VCD2.0 information, then turn off the flag
            // Disc CPD 75
            __wDiscType &= (~VCD_2_0);
            //set disc type as VCD 1.1
            __wDiscType |= (VCD_1_1);
            }
        }
        // Chuan 277a-2, 2002/10/01: check the disc is legal or not. VCD must have two tracks
        if ( wTemp == ID_PLAYLIST )
        {
            if (__wTrackEnd<2)
            {
                // wyc2.38a-909s, one disc only have one track but it is a valid VCD20 disc. So check this abnormal disc's lead out position.
                // The lead out position in this abnormal disc is 245852, so will recognize it as VCD20 disc when lead out position is 245852.
                // winnie 2.54, the lead out position in this abnormal disc is 399961, so will recognize it as vcd20 disc when lead out position is 399961.
                BYTE  bTemp, bTemp1;
                DWORD dwTemp;
                CDINFO_GetTrackNO ( & bTemp, & bTemp1, & dwTemp );
                if ((dwTemp != 245852)&&(dwTemp !=399961))
                {
                    __bDiscInfo|= DISCINFO_UNRECOGNIZE;
                }
            }
        }
        else if ( wTemp == ID_SELECTIONLIST )
        // Chuan 277a-2, 2002/10/01: Selection_List: Play Item is not segment item and the track only one. Illegal.
        {
            CDINFO_ParseList ( LIST_SEL_PLAYITEMNO, & wTemp );
            if( (wTemp>1) && (wTemp<1000) && (__wTrackEnd<2) )
            {
                __bDiscInfo|= DISCINFO_UNRECOGNIZE;
            }
        }
    }   // & VCD_2_0
    else if ( __wDiscType & BOOK_VCD )   // Chuan 277a-2, 2002/10/01: VCD 1.1/ VCD KARAOKE: The number of Track must have two.
    {
        if (__wTrackEnd<2)
            __bDiscInfo|= DISCINFO_UNRECOGNIZE;
    }

#ifndef     DEFAULT_NOPBC
    if ( __wDiscType & VCD_2_0 )
    {
        // simulation ENDLIST status
        __bModeCmd= KEY_STOP;
        __wStatus= ID_ENDLIST;

        // Micky1.10, support resume after power on for DVD/CDDA/VCD begin
        // Force to enter nonPBC mode when resume.
        if (( __bFlowState & FLOW_KEEP_RESUME ))
        {
            __btPBC= 0;
        }
        else
        {
            // Auto enter PBC mode if the title is VCD 2.0
            __btPBC= TRUE;

        }
        // Micky1.10, support resume after power on for DVD/CDDA/VCD end
    }
#endif  // #ifndef DEFAULT_NOPBC
#endif  // #ifndef NO_DISC_MODE

// Micky1.06, __wPlayItem will be set as correct value when resume(within LINEAR_Action(GOTO_TIME_PLAY))
    __wPlayItem = __wTrackBegin;

    __dwTimeNow= __dwTimeBegin;

    // ** TCH2.74; begin...
#ifdef   SUPPORT_VCD_CONTROL
    if ( __SetupInfo.bVCDControl == 0x5A )
    {
        if ( __wDiscType & BOOK_VCD )
        {
            __bDiscInfo= DISCINFO_UNRECOGNIZE;
            return ERR_NOERR;
        }
    }
#endif  // SUPPORT_VCD_CONTROL
    return TRUE;

}

#endif  // #ifndef SIMP_INITIAL


// Ming0.80, support multi-disc resume.
//#define DEBUG_MULTI_DISC_RESUME
#ifdef  DEBUG_MULTI_DISC_RESUME
void _INITIAL_ShowResumeInfo(void)
{
    BYTE i;

    for(i=0;i<RESUME_DISC_NUM;i++)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Entry = %lx, RU = %hx %hx %hx %hx\n",
           i,
            __Resume_Info.Resume[i].bRU_MType,
            __Resume_Info.Resume[i].bDiscUID[0],
            __Resume_Info.Resume[i].bDiscUID[1],
            __Resume_Info.Resume[i].bDiscUID[2]);

        // Resume related information.
        if( (__Resume_Info.Resume[i].bRU_MType&0xF) == 1)   // Non-DVD title
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nRSMTime: %lx", __Resume_Info.Resume[i].RSM_Info.GNL_Resume.dwRSMTime);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nRSMDir: %lx", __Resume_Info.Resume[i].RSM_Info.GNL_Resume.wRSMDir);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nRSMItem: %lx", __Resume_Info.Resume[i].RSM_Info.GNL_Resume.wRSMItem);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nRSMFrameCnt: %lx", __Resume_Info.Resume[i].RSM_Info.GNL_Resume.dwRSMFrameCnt);

        }
        else    // DVD title
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__bRSM_Title0=%hx", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__bRSM_Title);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__wRSM_Chapter0=%x", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__wRSM_Chapter);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__wRSM_PGCN=%x", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__wRSM_PGCN);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__wRSM_SPRM5=%x", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__wRSM_SPRM5);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__bRSMValid=%hx", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__bRSMValid);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__bRSM_CEN=%hx", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__bRSM_CEN);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__bRSM_byTime=%hx", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__bRSM_byTime);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__bRSM_PGN=%hx", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__bRSM_PGN);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__bRSM_VTSN=%hx", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__bRSM_VTSN);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__bRSM_Domain=%hx", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__bRSM_Domain);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__dwRSM_NV_PCK=%lx", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__dwRSM_NV_PCK);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__wRSM_Time=%x", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__wRSM_Time);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n__bPTL=%hx", __Resume_Info.Resume[i].RSM_Info.NV_Resume.__bPTL);
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "$$$$$$$$$$$__wSPRMs[13] = %x, PTL = %hx\n", __wSPRMs[13], __Resume_Info.Resume[i].RSM_Info.NV_Resume.__bPTL);
        }
    }
}
#endif  // #ifdef  DEBUG_MULTI_DISC_RESUME

// LLY2.14, porting Aron's code to support STB system
#if defined( SUPPORT_MULTI_DISC_RESUME) //||defined(SUPPORT_STB)
//  *******************************************************************************
//  Function    :   _Detect_Resume_Information
//  Description :   Check if the resume information exist of the input disc
//  Arguments   :   None.
//  Return      :   TRUE: Get a valid RESUME information.
//                  FALSE: Cannot get a valid RESUME information.
//  Side Effect :   will modify __pbDiscUID, __dwResumeTime, __ResumeInfo
//  *******************************************************************************
BYTE    _Detect_Resume_Information(void)
{
    BYTE bMediaType;
    DWORD dwTemp, dwTemp1;
    int i;

    // LLY2.05, using Disc length as disc identify for non-DVD title
    // ex. CDDA, VCD, CVD, SVCD, CDROM, DVDROM
    // Notice: & BOOK_VCD includes VCD1.0, VCD1.1. VCD2.0, SVCD, CVD
    if ( ( (__wDiscType & BOOK_CDDA) && !(__bCDDA_DTS & CDDA_CDG) ) ||  //Kevin1.26, CDG doesn't support resume
           (__wDiscType & BOOK_VCD) || (__wDiscType & CDROM_M1) )
    {
        bMediaType = 1;
        // Use the Disc length to distinguish the VCD and CDDA discs.
        // LLY1.00, though the SrcFilter to get disc length
        // Notice: only work for servo interface now.
        // Since, it's not implemented for ATAPI interface
        // #error "Must know the Disc length"
        // SERVO_GetDiscLength (&dwTemp );
        //dwTemp =1;
        SrcFilter_GetDiscLength(&dwTemp);
        __pbDiscUID[0] = (BYTE)(dwTemp & 0xff);
        __pbDiscUID[1] = (BYTE)((dwTemp >> 8) & 0xff);
        __pbDiscUID[2] = (BYTE)((dwTemp >> 16) & 0xff);
    }
    else if(__wDiscType & BOOK_DVD)
    {
        bMediaType = 2;
        // Calculate the sum of the 1st sector to distinguish the unique disc.
        dwTemp = 0;
        for(i=0;i<(4096/4);i++)
        {
            INFOFILTER_DumpSectors (_dwFirst_VMGI, 2, (BYTE*)&dwTemp1, 4, i*4);
            dwTemp += dwTemp1;
        }
        __pbDiscUID[0] = (BYTE)(dwTemp & 0xff);
        __pbDiscUID[1] = (BYTE)((dwTemp >> 8) & 0xff);
        __pbDiscUID[2] = (BYTE)((dwTemp >> 16) & 0xff);
    }
    else
    {
        // Cannot save resume info. for this media type.
#ifdef DEBUG_MULTI_DISC_RESUME
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Don't support disc resume feature for this media type \n");
#endif  // #ifdef DEBUG_MULTI_DISC_RESUME
        __fSaveResumeInfo = FALSE;
        return FALSE;
    }

    // LLY1.00, it's un-necessary to check if restore the resume information flag
    /*
    if(!__fRestoreResumeInfo)
    {
        return FALSE;
    }
    */


    // Read-back the disc resume identify flag
#ifdef SUPPORT_RESUME_FROM_EEPROM
    HAL_ReadStorage(SETUP_ADDR_RESUME_FLAG, (BYTE*)&__Resume_Info, RESUME_FLAG_LENGTH);
#endif  // #ifdef SUPPORT_RESUME_FROM_EEPROM


    // Check if the "RSMI" identify already exist.
    if ((__Resume_Info.bRSM_Mark[0] != 'R') || (__Resume_Info.bRSM_Mark[1] != 'S') ||
        (__Resume_Info.bRSM_Mark[2] != 'M') || (__Resume_Info.bRSM_Mark[3] != 'I'))
    {
#ifdef DEBUG_MULTI_DISC_RESUME
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "cannot find RSMI\n");
#endif
        return FALSE;
    }


    // Read-back the disc resume data, include ID + information.
#ifdef SUPPORT_RESUME_FROM_EEPROM
    // LLY1.00, call HAL_ReadStorage to readback all data together
    // Don't care if necessary to partition it
    HAL_ReadStorage(SETUP_ADDR_RESUME_DATA, ((BYTE *)&__Resume_Info)+RESUME_FLAG_LENGTH,
        RESUME_DATA_LENGTH);
#endif  // #ifdef SUPPORT_RESUME_FROM_EEPROM


#ifdef DEBUG_MULTI_DISC_RESUME
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Test Disc UID = %hx %hx %hx\n", __pbDiscUID[0], __pbDiscUID[1], __pbDiscUID[2]);
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nDetect Resume info\n");
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nDVD resume information begin(Restore)*********");
    _INITIAL_ShowResumeInfo();
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nDVD resume information end*********\n");
#endif // DEBUG_MULTI_DISC_RESUME

    // Try to get a match RESUME information.
    for(i=0; i<RESUME_DISC_NUM; i++)
    {
        // [7:4] 0: Invalid entry.
        //       other: The larger value represents the more recent used disc.
        if(!(__Resume_Info.Resume[i].bRU_MType & 0xf0))   // Not a valid entry.
        {
#ifdef DEBUG_MULTI_DISC_RESUME
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "RU = %hx %hx %hx %hx\n",
            __Resume_Info.Resume[i].bRU_MType,
            __Resume_Info.Resume[i].bDiscUID[0],
            __Resume_Info.Resume[i].bDiscUID[1],
            __Resume_Info.Resume[i].bDiscUID[2]);
#endif
            continue;
        }
        // [3:0]: 1-> VCD, CDDA, DVDROM, CDROM
        //        2-> DVD.
        if((__Resume_Info.Resume[i].bRU_MType & 0x0f) == bMediaType)  // valid media type.
        {
            // Use 24-bit ID to distinguish the uniqie disc.
            if((__Resume_Info.Resume[i].bDiscUID[0] != __pbDiscUID[0]) ||
               (__Resume_Info.Resume[i].bDiscUID[1] != __pbDiscUID[1]) ||
               (__Resume_Info.Resume[i].bDiscUID[2] != __pbDiscUID[2]))
            {
                // Try the next entry.
                continue;
            }
            if (bMediaType == 1)
            {
                __dwResumeTime = __Resume_Info.Resume[i].RSM_Info.GNL_Resume.dwRSMTime;

                // LLY2.05, keep extra resume information for CDROM/ DVDROM
                if(__wDiscType & CDROM_M1)
                {
                    // resume directory and item
                    __dwResumeItem = MAKELONG(__Resume_Info.Resume[i].RSM_Info.GNL_Resume.wRSMItem,
                        __Resume_Info.Resume[i].RSM_Info.GNL_Resume.wRSMDir);

                    // resume frame count
                    __dwGoTimeFrameCnt = __Resume_Info.Resume[i].RSM_Info.GNL_Resume.dwRSMFrameCnt;
                }
                return TRUE;
            }
            else if(bMediaType == 2)
            {
                // Dump RESUME information to NV_Resume.
                memcpy(&NV_Resume[0],
//                       &__Resume_Info.Resume[i].NV_Resume,
                       &__Resume_Info.Resume[i].RSM_Info.NV_Resume,
                       sizeof(NV_RSMINFO));

                // Ming2.38, must check PTL level in RESUME process.
                if(__wSPRMs[13] < (WORD)NV_Resume[0].__bPTL)
                {
                    SETUP_InputPassword (KEY_NO_KEY);
                    while (1)
                    {
                        //Aron2.36a, need to the event trigger in the while loop
                        UTL_EventTrigger();
                        if (__bISRKey != KEY_NO_KEY)
                        {
                            switch(SETUP_InputPassword(__bISRKey))
                            {
                            // password correct
                            case SETUP_INPUT_PASSWORD_CORRECT:
                                __bISRKey = KEY_NO_KEY;
                                // store the value of __wSPRMs 13, need restore at InitialAccess and
                                // when transfer to Stop state
                                // DVD_050Micky
                                // must record the player's parental level
                                __wStoreSPRM13 = __SetupInfo.bParentalCtl - SETUP_CUSTOM_PC_LEVEL_BASE + 1;
                                __wStoreSPRM13 <<=8;    //high 8 bits, indicate original SPRM[13]

                                // change the parental level
                                __wSPRMs[13]=(WORD)NV_Resume[0].__bPTL;   // bit3 8-11

                                //DVD_050Micky
                                // store the tmp parental level in low 8 bits
                                __wStoreSPRM13 |=__wSPRMs[13];    //low 8 bits, indicate new SPRM[13]
                                // Aron1.50-909, Fix "Bone collector" Power off when displaying copyright warning message
                                // It always show parental error after resuming.
                                // must issue it when parental level is changed.
                                NV_Setting(SET_PTL_ID_FLD, 0);
                                return TRUE;

                            // open/close/power, just give up the action
                            case SETUP_INPUT_PASSWORD_CLEAR:
                                if(__bISRKey == KEY_LEFT)
                                {
                                    // input the password again.
                                    SETUP_InputPassword (KEY_NO_KEY);
                                }
                                else
                                {
                                    return FALSE;
                                }
                                break;

                            case SETUP_INPUT_PASSWORD_WRONG:
                                __bISRKey = KEY_NO_KEY;
                                return FALSE;
                            }
                            __bISRKey=KEY_NO_KEY;
                        }
                    }   // while (1)
                }
                return TRUE;
            }
        }
    }

    return FALSE;
}


//  **********************************************************************************
//  Function    :   _Save_Resume_Information
//  Description :   Save or clear resume information
//  Arguments   :   bAction, specify the action while save resume information
//                  SAVE_INFO, save the resume information
//                  CLEAR_INFO, clear the resume information
//  Return      :   TRUE: Save the resume information correctly.
//                  FALSE: Fail to save resume information.
//  Side Effect :   will modify __cBuffer
//  **********************************************************************************
// Ming2.39, clear the RESUME info. in EEPROM.
// Add the parameter to indicate the clear action.
BYTE    _Save_Resume_Information(BYTE bAction)
{
    BYTE bMediaType, bRUVal;
    int i;
    BYTE bFoundEntry;

#ifdef DEBUG_MULTI_DISC_RESUME
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Test Disc UID = %hx %hx %hx\n", __pbDiscUID[0], __pbDiscUID[1], __pbDiscUID[2]);
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "bAction = %hx\n", bAction);
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "__fSaveResumeInfo = %hx\n", __fSaveResumeInfo);
#endif

    // Ming2.39, Do not save the RESUME info. if it is in the password input mode.
    if(!__fSaveResumeInfo)
    {
        return TRUE;
    }

    if ( ( (__wDiscType & BOOK_CDDA) && !(__bCDDA_DTS & CDDA_CDG) ) ||  //Kevin1.26, CDG doesn't support resume
    // for VCD, support normal track resume(segment play item resume is not supported)
    // Micky1.06, modify to 2~599, support entry track too.
         ( ( __wDiscType & BOOK_VCD ) && ( (__wPlayItem >=2) && (__wPlayItem <=599) ) ) ||
         // LLY2.05, only support MP3 disc resume for CDROM/ DVDROM title
          ((__wDiscType&CDROM_M1) && (__bEXTAttr==EXT_BIT_MP3)) )
    {
        bMediaType = 1;
    }
    else if(__wDiscType & BOOK_DVD)
    {

#ifdef DEBUG_MULTI_DISC_RESUME
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "__dwPGC_UOP_CTL = %lx\n", __dwPGC_UOP_CTL);
#endif
        // LLY1.00, don't care if can resume or not
        // ie. always save resume information even the disc says it can't resume
        // Otherwise, the behavior is mis-match while disc resume & stop -> resume
        // ex. D-189, "The Patrior"; D-334, "The Bone Collector"
#if 0
        // Ming2.39, must check PTL level in RESUME process.
        // If cannot RESUME, do not save it.
        if((__dwPGC_UOP_CTL & UOP_CAN_RESUME) && bAction==SAVE_INFO)
        {
            return TRUE;
        }
#endif  // #if 0

        bMediaType = 2;
    }
    else
    {
        return FALSE;
    }

    // Read-back the disc resume identify flag
#ifdef SUPPORT_RESUME_FROM_EEPROM
    HAL_ReadStorage(SETUP_ADDR_RESUME_FLAG, (BYTE*)&__Resume_Info, RESUME_FLAG_LENGTH);
#endif  // #ifdef SUPPORT_RESUME_FROM_EEPROM

    if ((__Resume_Info.bRSM_Mark[0] != 'R') || (__Resume_Info.bRSM_Mark[1] != 'S') ||
        (__Resume_Info.bRSM_Mark[2] != 'M') || (__Resume_Info.bRSM_Mark[3] != 'I'))
    {
        // Ming1.24-1, clear the RESUME info. in EEPROM.
        if(bAction == CLEAR_INFO)  // Cannot find the RESUME info.
        {
            return FALSE;
        }
        memset(&__Resume_Info, 0, sizeof(Multi_Resume_Info));
        __Resume_Info.bRSM_Mark[0] = 'R';
        __Resume_Info.bRSM_Mark[1] = 'S';
        __Resume_Info.bRSM_Mark[2] = 'M';
        __Resume_Info.bRSM_Mark[3] = 'I';
    }
#ifdef SUPPORT_RESUME_FROM_EEPROM
    else
    {
        // Read-back the resume data (ID + information) if the "RSMI" already exist

        // LLY1.00, call HAL_ReadStorage to readback all data together
        // Don't care if necessary to partition it
        HAL_ReadStorage(SETUP_ADDR_RESUME_DATA, ((BYTE *)&__Resume_Info)+RESUME_FLAG_LENGTH,
            RESUME_DATA_LENGTH);

    }
#endif // #ifdef SUPPORT_RESUME_FROM_EEPROM


    // Find saved entry.
    for(i=0;i<RESUME_DISC_NUM;i++)
    {
        if((__Resume_Info.Resume[i].bRU_MType & 0x0f) == bMediaType)  // valid media type.
        {
            if((__Resume_Info.Resume[i].bDiscUID[0] == __pbDiscUID[0]) &&
               (__Resume_Info.Resume[i].bDiscUID[1] == __pbDiscUID[1]) &&
               (__Resume_Info.Resume[i].bDiscUID[2] == __pbDiscUID[2]))
            {
                // Find the saved entry.
                break;
            }
        }
    }
    if(i >= RESUME_DISC_NUM)   // Cannot find saved entry.
    {
        // Ming1.24-1, clear the RESUME info. in EEPROM.
        if(bAction==CLEAR_INFO)  // Cannot find the entry.
        {
            return FALSE;
        }

        // Find the maximal RU.
        for(bRUVal=0,i=0; i<RESUME_DISC_NUM; i++)
        {
            if((__Resume_Info.Resume[i].bRU_MType & 0xf0) > bRUVal)
            {
                bRUVal = __Resume_Info.Resume[i].bRU_MType & 0xf0;
            }
        }
        // If the maximal RU value is greater or equal than RESUME_DISC_NUM.
        // Must decrease all RU value of existed entry. Then the RESUME_DISC_NUM
        // can be filled to the new entry.
        // Micky1.06, decrease all RU value by 1 when full
        if(bRUVal >= ((RESUME_DISC_NUM<<4)))
        {
            for(i=0; i<RESUME_DISC_NUM; i++)
            {
                if(__Resume_Info.Resume[i].bRU_MType & 0xf0)
                {
                    __Resume_Info.Resume[i].bRU_MType -= 0x10;
                }
            }
        }
        // Find the entry which has the smallest RU value.
        for(bRUVal=0,i=1; i<RESUME_DISC_NUM; i++)
        {
            if((__Resume_Info.Resume[i].bRU_MType & 0xf0) <
               (__Resume_Info.Resume[bRUVal].bRU_MType & 0xf0))
            {
                // Micky1.06, keep the entry that has the small number till now.
                bRUVal = (BYTE)i;
            }
        }
        bFoundEntry = bRUVal;
    }
    else    // Find the saved entry.
    {
        bFoundEntry = i;
        // Let the RU value to be continuous.
        // Micky1.06, decrease those larger RU value by 1
        for(i=0; i<RESUME_DISC_NUM; i++)
        {
            if(((__Resume_Info.Resume[bFoundEntry].bRU_MType & 0xf0) <
                 (__Resume_Info.Resume[i].bRU_MType & 0xf0)) &&
               (__Resume_Info.Resume[i].bRU_MType & 0xf0))
            {
                __Resume_Info.Resume[i].bRU_MType -= 0x10;
            }
        }
    }

    if(bAction==CLEAR_INFO)
    {
        // Clear the entry.
        __Resume_Info.Resume[bFoundEntry].bRU_MType = 0;
    }
    else
    {
        // Micky1.06, the new update one will have the largest RU value
        __Resume_Info.Resume[bFoundEntry].bRU_MType = (RESUME_DISC_NUM<<4) | bMediaType;
        __Resume_Info.Resume[bFoundEntry].bDiscUID[0] = __pbDiscUID[0];
        __Resume_Info.Resume[bFoundEntry].bDiscUID[1] = __pbDiscUID[1];
        __Resume_Info.Resume[bFoundEntry].bDiscUID[2] = __pbDiscUID[2];
        switch(bMediaType)
        {
        case 1:
            __Resume_Info.Resume[bFoundEntry].RSM_Info.GNL_Resume.dwRSMTime = __dwResumeTime;

            // LLY2.05, keep playing item & directory for CDROM/ DVDROM title
            // And, always save resume frame count value (since don't know if audio only file here)
            if(__wDiscType & CDROM_M1)
            {
                __Resume_Info.Resume[bFoundEntry].RSM_Info.GNL_Resume.wRSMDir = HIWORD(__dwResumeItem);
                __Resume_Info.Resume[bFoundEntry].RSM_Info.GNL_Resume.wRSMItem = LOWORD(__dwResumeItem);
                __Resume_Info.Resume[bFoundEntry].RSM_Info.GNL_Resume.dwRSMFrameCnt = __dwGoTimeFrameCnt;

            }
            break;
        case 2:
            memcpy(&__Resume_Info.Resume[bFoundEntry].RSM_Info.NV_Resume,
                   &NV_Resume[0],
                   sizeof(NV_RSMINFO));
            __Resume_Info.Resume[bFoundEntry].RSM_Info.NV_Resume.__bPTL = (BYTE)__wSPRMs[13];
            break;
        }
    }

    // Save the resume to EEPROM
#ifdef SUPPORT_RESUME_FROM_EEPROM
    // LLY1.00, only save the necessary data, ex flag & modified resume slot
    // To save the time while open/close or IR power on/off
    // [1] Save resume flag
    HAL_WriteStorage(SETUP_ADDR_RESUME_FLAG, __Resume_Info.bRSM_Mark,
        RESUME_FLAG_LENGTH);
    // [2] Save modified resume slot only
    HAL_WriteStorage( (SETUP_ADDR_RESUME_DATA+bFoundEntry*RESUME_DATA_ENTRY_SIZE),
        (BYTE *)&__Resume_Info.Resume[bFoundEntry], RESUME_DATA_ENTRY_SIZE);
    // [3] Save the RU type since it is re-adjusted
    for(i=0; i<RESUME_DISC_NUM; i++)
    {
        HAL_WriteStorage( (SETUP_ADDR_RESUME_DATA+i*RESUME_DATA_ENTRY_SIZE),
            (BYTE *)&__Resume_Info.Resume[i].bRU_MType, 1);
    }

#endif // #ifdef SUPPORT_RESUME_FROM_EEPROM


#ifdef DEBUG_MULTI_DISC_RESUME
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nResume info\n");
#ifdef SUPPORT_RESUME_FROM_EEPROM
    for (i=0;
         i < (RESUME_FLAG_LENGTH+RESUME_DATA_LENGTH);
         i+=4)
    {
        HAL_ReadStorage(SETUP_ADDR_RESUME_FLAG+i,
                       __cBuffer,
                       4);
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "%hx %hx %hx %hx\n", __cBuffer[0], __cBuffer[1], __cBuffer[2], __cBuffer[3]);
    }
#endif // #ifdef SUPPORT_RESUME_FROM_EEPROM

    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nDVD resume information begin(Store)*********");
    _INITIAL_ShowResumeInfo();
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nDVD resume information end*********\n");
#endif  // #ifdef DEBUG_MULTI_DISC_RESUME
    return TRUE;
}

#endif  //#ifdef  SUPPORT_MULTI_RESUME

#ifdef AUTO_TEST
#include "osdnd.h"
//Alan2.20, add the following two functions for auto-test
//  *********************************************************************
//  Function    :   INITIAL_ShowPNO
//  Description :   Show the Part number
//  Arguments   :   NONE
//  Return      :
//  Side Effect :
//  *********************************************************************
void _INITIAL_ShowPNO(void)
{
    #define CT909R_A    0x0
    #define CT219R_A    0x8
    #define CT909R_E    0x5
    #define CT219R_E    0xD
    #define CT909R_W    0x4
    #define CT219R_W    0xC
    #define CT909RZ     0x6
    #define CT909SZ     0x2

    WORD aStrCT909R_A[] = {8, CHAR_C, CHAR_T, CHAR_9, CHAR_0, CHAR_9, CHAR_R, CHAR_LOW_LINE, CHAR_A};
    WORD aStrCT219R_A[] = {8, CHAR_C, CHAR_T, CHAR_2, CHAR_1, CHAR_9, CHAR_R, CHAR_LOW_LINE, CHAR_A};
    WORD aStrCT909R_E[] = {8, CHAR_C, CHAR_T, CHAR_9, CHAR_0, CHAR_9, CHAR_R, CHAR_LOW_LINE, CHAR_E};
    WORD aStrCT219R_E[] = {8, CHAR_C, CHAR_T, CHAR_2, CHAR_1, CHAR_9, CHAR_R, CHAR_LOW_LINE, CHAR_E};
    WORD aStrCT909R_W[] = {8, CHAR_C, CHAR_T, CHAR_9, CHAR_0, CHAR_9, CHAR_R, CHAR_LOW_LINE, CHAR_W};
    WORD aStrCT219R_W[] = {8, CHAR_C, CHAR_T, CHAR_2, CHAR_1, CHAR_9, CHAR_R, CHAR_LOW_LINE, CHAR_W};
    WORD aStrCT909RZ[] = {7, CHAR_C, CHAR_T, CHAR_9, CHAR_0, CHAR_9, CHAR_R, CHAR_Z};
    WORD aStrCT909SZ[] = {7, CHAR_C, CHAR_T, CHAR_9, CHAR_0, CHAR_9, CHAR_S, CHAR_Z};
    WORD aStrWrongPNO[] = {13, CHAR_W, CHAR_R, CHAR_O, CHAR_N, CHAR_G, CHAR_SPACE, CHAR_P, CHAR_A, CHAR_R, CHAR_T, CHAR_SPACE, CHAR_N, CHAR_O};

    WORD* pwPNO_String;

    PARM_DRAW_STRING _INITIAL_StrAttr;
    _INITIAL_StrAttr.bTextColor = OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING;
    _INITIAL_StrAttr.bBackground = OSDND_PALETTE_ENTRY_BACKGROUND;
    _INITIAL_StrAttr.wX = OSDND_BOTTOM_BACKGROUND_START_H;
    _INITIAL_StrAttr.wY = OSDND_BOTTOM_BACKGROUND_START_V - OSDND_BOTTOM_BACKGROUND_HEIGHT;

    switch (MACRO_PLAT_GET_PARTNO)
    {
    case CT909R_A:
        pwPNO_String = aStrCT909R_A;
        break;
    case CT219R_A:
        pwPNO_String = aStrCT219R_A;
        break;
    case CT909R_E:
        pwPNO_String = aStrCT909R_E;
        break;
    case CT219R_E:
        pwPNO_String = aStrCT219R_E;
        break;
    case CT909R_W:
        pwPNO_String = aStrCT909R_W;
        break;
    case CT219R_W:
        pwPNO_String = aStrCT219R_W;
        break;
    case CT909RZ:
        pwPNO_String = aStrCT909RZ;
        break;
    case CT909SZ:
        pwPNO_String = aStrCT909SZ;
        break;
    default:
        pwPNO_String = aStrWrongPNO;
        break;
    }

    GDI_DrawString_909(0, &_INITIAL_StrAttr, pwPNO_String);
}

void _INITIAL_Clear_PNO_Region(void)
{
    PARM_RECT        _INITIAL_RectAttr;

    _INITIAL_RectAttr.rect.wLeft = OSDND_BOTTOM_BACKGROUND_START_H;
    _INITIAL_RectAttr.rect.wTop = OSDND_BOTTOM_BACKGROUND_START_V - OSDND_BOTTOM_BACKGROUND_HEIGHT;
    _INITIAL_RectAttr.rect.wRight = OSDND_BOTTOM_BACKGROUND_START_RIGHT;
    _INITIAL_RectAttr.rect.wBottom = OSDND_BOTTOM_BACKGROUND_START_V;
    _INITIAL_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;

    GDI_FillRect_909(0, &_INITIAL_RectAttr);
}
#endif // #ifdef AUTO_TEST
