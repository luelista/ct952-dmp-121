// Micky0.65-909, enable MSG_WAITING. during parsing.
// The file "winav.h" must be included in first line
#include "winav.h"
#include "ctkav.h"  // ** TCH0.60; 

#include    <string.h>
#include "initial.h"
#include "utl.h"

#include "hal.h"

#include "SrcFilter.h"
#include "input.h"
#include "chips.h"
#include "thumb.h"
#include "comutl.h"
#include "gdi.h" //CoCo.CT909
#include "osdmm.h" //CoCo.CT909

#include "setup.h"
#include "navigate.h"
#include "cell.h"
#include "dvdinfo.h"    // ** TCH0.27;

#include "infofilter.h"
#include "vcd20.h"

#include "digest.h"

#include "cc.h"
#include "osd.h"

#include "linear.h"
#include "monitor.h"
#include "media.h"  // ** TCH0.60; 

#include "disp.h"
#include "parser.h"
//#include "mpeg2dec.h"
#include "subpict.h"

#include "osddsply.h"
// wyc0.50-909, need to include this file.
#include "infofilter.h"
#include "srcfilter.h"
#include "osddlg.h"
#include "osdss.h"
// wyc2.75-909P, include usbwrap.h if USB2.0 mode
#if (USB_SYSTEM == USB2_0_JUNGO)
#include "usbwrap.h"
#elif (USB_SYSTEM == USB1_1_CT909R)
#include "usb.h"
#endif //
#include "mmanager.h"
// elmer2.38a
#include "usbsrc.h"

#ifdef SUPPORT_POWERON_MENU //CoCo2.38
#include "poweronmenu.h"
#endif

#ifdef SUPPORT_STB
#include "sysmenu_op.h"
#endif

#ifdef CTK_CARDREADER
#include "card.h"
#endif

// wyc2.78-909P,
#ifdef SUPPORT_USB_HOST_DEVICE
#include "notedlg.h"
#include "osdss.h"
#endif //

// ============================= Definition =============================
// wyc1.01-909, re-allocate the layout.
#define NULL_0      0       // ** TCH0.60; 
// The first 2 is source servo/ source atapi
#define MAX_REMOVABLE_MEDIAS     (PER_USB_NO*SUPPORT_USB_PORT_NUM)
// 0 is for 0x10, 1 is for 0x20
// 3~10 is for 0x30,31,32,33,34,35,36,37
// 11~18 is for 0x40,41,42,43,44,45,46,47

// LLY2.17, remove top open/close pin to pio.h
//#define PIN_TOP_OPENCLOSE 16

///////////////////////////////////////////////////////////////
#define SUPPORT_REMOVABLE_MEDIA  // for real media
//#define DEBUG_MEDIA_STATUS

// ============================= Variables =============================
// Check tray force closed??
// Check tray open fail??
// Micky1.00_909_media
BYTE    __bMediaServoAction;
BYTE    __bMediaServoState;
BYTE    __bMediaUSBAction;
BYTE    __bMediaUSBState;

// Micky_909_play_item
// [Navigator Mode]
// assigned by infofilter
BYTE    __bNavigateMode = NAVIGATE_NONE;
BYTE    __bChooseMedia;
BYTE    _bPrevMedia,_bNextMedia;
BYTE    _bShowNoMedia;

// wyc2.75-909P, make default init USB one time.
// wyc2.75-909P, include usbwrap.h if USB2.0 mode
#ifdef SUPPORT_USB_SOURCE
BYTE    _bMediaInitUSB = FALSE;
#endif //

// wyc2.17a-909S
#define SERVO_SECTOR_SZIE   0xa00
// Micky2.17c
#ifdef  SUPPORT_SERVO_SOURCE
#define SERVO_EDC_BUFFER_SIZE   ((DS_INPUT_BITBUF_END-DS_INPUT_BITBUF_ST)/SERVO_SECTOR_SZIE)
WORD    _wSrvEDCBuff[SERVO_EDC_BUFFER_SIZE];
#endif  //#ifdef    SUPPORT_SERVO_SOURCE
#ifdef SUPPORT_STB
BYTE    __bPreMediaMode = MEDIA_SELECT_UNKNOW;
BYTE    __bMediaSelected = FALSE;
// wyc2.21-909s, add members' code.
extern BYTE __bSubtitlingMode;
extern VOID DVB_SbtlStop(VOID);
extern VOID DVB_TTX_Stop(VOID);
#ifndef NO_DISC_MODE
extern VOID SERVO_Enable(VOID);
extern VOID SERVO_Disable(VOID);
#endif //NO_DISC_MODE
extern VOID AP_ConfigGDIRegion(BYTE bClearRegion);
#endif

// [Media]
// Micky0.85, reference srcfilter.h define.
// wyc1.01-909, need remember to remove this variable and use __SF_SourceGBL[__dwSFStreamID].bSourceIndex to replace it.
//BYTE    __bSourceID = SOURCE_DISC;
// Micky0.85, index to MediaInfo
BYTE    __bActionMediaID = 0;

MEDIAINFO   MediaInfo[TOTAL_MEDIA];
// Micky0.85, index to MediaInfo
BYTE    _bNextServiceID = 2;

// [Media Selction dialog]
BYTE    __bMediaSelectUI = FALSE;

DWORD _dwMediaTemp1;
DWORD _dwMediaChkTime;
// wyc2.78-909P,
DWORD _dwMediaUSBOTGChkTime = 0;
BYTE _bMediaUSBConnPC = FALSE;
BYTE _bMediaExitingUSBStack = FALSE;

// wyc1.21-909, variable to represent the active port to let F/W can init the correct H/W port.
DWORD _dwUSBSRCActivePort;
#ifdef SUPPORT_SLOT_IN      // KCHong 2.17, for slot-in loader
DWORD __dwCloseTrayTimer=0;
#endif //

#ifdef SUPPORT_POWERON_MENU
#ifdef SUPPORT_USB_SWITCH
// check three times, including card-reader, USB, CF-card in order default, except specified in SETUP
BYTE    __bMediaRegCnt = 2;
#else
// check two times, including USB, card-reader default, except specified in SETUP
BYTE    __bMediaRegCnt = 1;
#endif
#endif

BYTE _bTriggerCmd1, _bTriggerCmd2;
// wyc2.78-909P,
BYTE _bTriggerCmdHD = FALSE;
BYTE _bTriggerCmdChkCARD = FALSE;
BYTE _bTriggerCmdExitUSBStack = FALSE;
BYTE _bIssueCmdExitUSBStack = FALSE;

BYTE __bMediaFirstLUN = 0xFF;
#ifdef SUPPORT_USB_SWITCH
BYTE _bMediaSource = 1; // CTK card-reader first
#else
BYTE _bMediaSource = 0; // USB first
#endif
#ifdef SUPPORT_CHOOSE_MEDIA
//BYTE __bMediaFirstLUN = 0xFF;
BYTE _bDisableCheckPort;
BYTE _bFirstCheckPort = 1;
DWORD _dwPortStatus = 0, _dwPrePortStatus;
DWORD _dwDefaultDevice = 0x00010000;    // set to 0x00010000 to detect USB first
#endif /* SUPPORT_CHOOSE_MEDIA */

// ============================= Extern Variables =============================
extern BYTE __bCloseCommand ;   // ** TCH0.60; 

// ============================= Internal function =============================
void _MEDIA_MonitorMediaStatus(void);
void _MEDIA_MonitorServoStatus(void);
void _ResetSuspendMedia(void);
BYTE _QuerySuspendMedia(void);
BYTE _MapIndex2SourceID(BYTE bIndex);
BYTE _MEDIA_Initial(void);
BOOL _MEDIA_SwitchPreAct ( BYTE bPreMedia);
BOOL _MEDIA_SwitchPostAct ( BYTE bPostMedia);
#ifdef SUPPORT_CHOOSE_MEDIA
#ifdef CTK_CARDREADER
DWORD _MEDIA_GetCardReaderStatus(DWORD dwInsert);
#endif
#endif

BYTE MEDIA_ShowSelDlg(VOID)
{
    if ( __bMediaSelectUI == FALSE )
    {
        // wyc2.22-909s,
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
        if (!CC_ForceStop(CC_FORCE_STOP_FROM_MEDIA_SELECT_UI))
            return FALSE;
#endif //
        // wyc1.05-909, update Brian's code.
        _bPrevMedia = __bChooseMedia; // Brian
        _bNextMedia = __bChooseMedia;
        //OSD_ChangeUI(OSD_UI_MEDIA_SELECT_DLG, OSD_ENTER_UI);  // enter the UI
        // wyc2.00-909S,because some OSD will be clear in non-MM mode when call this message OSDDLG_MSG_SHOW_DLG, so we need to 
        // separate two mode to avoid clear still active message in non-MM mode.
        // wyc2.32-909S, clear all region when thumbnail mode.
        if (((__wDiscType & CDROM_M1) && (__bMMMenuID != MM_MENU_NONE)) || (__bThumbnailMode))
            OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_SHOW_DLG, OSDDLG_MEDIA_SELECT_BACKGROUND_MM_MODE);
        else
            OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_SHOW_DLG, OSDDLG_MEDIA_SELECT_BACKGROUND_GENERAL_MODE);
        OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_DRAW_HIGHLIGHT, __bChooseMedia);
        __bMediaSelectUI = TRUE;
        //__bChooseMedia = FALSE;
        //__bMediaServoState = MEDIA_SERVO_PROMPT;
        if (__wDiscType & CDROM_M1)
        {
            // wyc1.10-909, keep all video/motion when JPG & all motion mode. Because if disable the video in JPG mode, when next
            // in effect transfer mode, the video won't disable.
            if (__btPlaying || __btPause)
            {
                if ((__bAttrPlay & TYPE_CDROM_AUDIO) || (__bThumbnailMode))
                {
                    DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
                    HAL_Display ( HAL_DISPLAY_SPST1, FALSE );
                }
            }
            else
            {
                DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
                HAL_Display ( HAL_DISPLAY_SPST1, FALSE );
            }
        }
        // wyc2.16-909S
#ifdef SUPPORT_STB                
        if(__bPreMediaMode != MEDIA_SELECT_STB)
#endif                
        {
            // wyc1.06-909, make system pause after enter select media UI. Because if not pause system, MP3 in CDROM mode won't go
            // next file automatically because KEY_NEXT / KEY_PREV is reject in MEDIA_MediaSelection_ProcessKey.
            // wyc2.22-909s, only no pre-stop mode do the pause command. Otherwise __btPause will error update and make resume error action.
#ifndef PRESTOP_BEFORE_SETUP_AND_PROGRAM
            __btPause= TRUE;
            __btPlaying=FALSE;
            CHIPS_OpenAudio(FALSE);
            UTL_TransferCommandID(KEY_PAUSE);
#endif //

        }
    }
    return TRUE;

}

//  *********************************************************************
//  Function    :   MEDIA_MediaSelection_ProcessKey
//  Description :   This function will process the key when media selection UI is prompt.
//  Arguments   :   bKey, the input key
//  Return      :   KEY_NO_KEY or the key to be processed.
//  Side Effect :
//  *********************************************************************
// Micky0.80, remove bKey Parameter an dreturn value
BYTE    MEDIA_MediaSelection_ProcessKey(void)
{
    if (( __bMediaSelectUI == FALSE ) && (__bKey != KEY_SELECTMEDIA))
        return KEY_BYPASS;

#ifdef SUPPORT_AV_IN
    if(__btPowerDown==TRUE&&(_btTFT_PowerDown == _btAVIN))
        return KEY_BYPASS;
#endif    
    // wyc1.10-909, need to add define to reduce code when remove USB function.
    //Aron2.21-909R, don't need to do the checking when DVB mode
    if( __SF_SourceGBL[__dwSFStreamID].bSourceIndex != SOURCE_DVB )
    {
        __bTemp = MediaInfo[_MapSourceID2Index(__SF_SourceGBL[__dwSFStreamID].bSourceIndex)].bState;
        // wyc1.50-909, when doing pre-scan action, don't allow entering media select UI, otherwise system will auto play after exit
        // media select UI but system should pause when entering media select UI.
        if ((__bTemp == MEDIA_INSERT) || (__bTemp == MEDIA_PARSING) || ((__wDiscType & CDROM_M1) && (__bMotionState == MOTION_STATE_PRESCAN)))
            return INVALID_KEY;
    }
    // wyc2.38a-909s, can't process KEY_FUNCTIOM when power down mode, otherwise LOGO will be abnormal when enter power down -> KEY_FUNCTION -> power on.
    if ((__btPowerDown) || (__bModeCmd == KEY_SCF) || (__bModeCmd == KEY_SCB) || (__bModeCmd == KEY_FF))
    {
        return INVALID_KEY;
    }
    switch(__bKey)
    {
    case    KEY_UP:
        OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_CLEAR_HIGHLIGHT, _bNextMedia); 
        if (_bNextMedia == (MEDIA_SELECT_BEGIN))
            _bNextMedia = (MEDIA_SELECT_END-1);
        else
            _bNextMedia--;
        OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_DRAW_HIGHLIGHT, _bNextMedia); 
        break;
    case    KEY_DOWN:
        OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_CLEAR_HIGHLIGHT, _bNextMedia); 
        if (_bNextMedia == (MEDIA_SELECT_END-1))
            _bNextMedia = (MEDIA_SELECT_BEGIN);
        else
            _bNextMedia++;
        OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_DRAW_HIGHLIGHT, _bNextMedia); 
        break;
    case    KEY_SELECTMEDIA: // Brian
        if ( __bMediaSelectUI == FALSE )
        {
#ifdef SUPPORT_STB
            if(__bSubtitlingMode==TRUE)
            {
                DVB_SbtlStop();
                DVB_TTX_Stop();
            }
#endif                
#ifdef USE_ONE_KEY_SELECT
            _bPrevMedia = __bChooseMedia;

#ifdef SUPPORT_AV_IN
            if(_btAVIN == TRUE)
            {
                _bPrevMedia =  MEDIA_SELECT_AV_IN;
            }
#endif                    
            _bNextMedia = _bPrevMedia + 1;
            if(_bNextMedia == MEDIA_SELECT_END)
            {
                _bNextMedia = MEDIA_SELECT_BEGIN;
            }
            __bKey = KEY_PLAY;
#else
#ifdef SUPPORT_AV_IN

            if(_btAVIN == TRUE)
            { // set exit AV in futnion
                _bPrevMedia = MEDIA_SELECT_AV_IN;
                _bNextMedia = MEDIA_SELECT_DVD;
                __bKey = KEY_PLAY;
            }
            else
#endif //
            {
                // elmer2.60, port coco's code
#ifdef SUPPORT_POWERON_MENU
                _MEDIA_SwitchPreAct(_bPrevMedia);
                __bMediaSelectUI = FALSE;
                _bShowNoMedia = FALSE;
#ifdef SUPPORT_STB
                __bMediaSelected = TRUE;
#endif //SUPPORT_STB
                MediaInfo[_MapSourceID2Index(SOURCE_USB0_0)].bState = MEDIA_NONE;
                __bChooseMedia = _bNextMedia;
                OSD_RemoveAllUI();
                CC_KeyCommand(KEY_STOP);
                CC_ResetState( KEY_OPEN_CLOSE );

                __bNavigateMode = NAVIGATE_NONE;
                MediaInfo[_MapSourceID2Index(__SF_SourceGBL[__dwSFStreamID].bSourceIndex)].bState = MEDIA_NONE;
                __bMediaUSBAction = MEDIA_ACTION_USB_NONE;
                __bMediaUSBState = MEDIA_USB_NONE;
                _dwMediaChkTime = OS_GetSysTimer();
                POWERONMENU_Initial();
#else
                MEDIA_ShowSelDlg();
#endif //SUPPORT_POWERON_MENU
                return KEY_NO_KEY;
            }
#endif    
        }
    case    KEY_PLAY:
    case    KEY_PLAY_PAUSE:
    case    KEY_ENTER: //CoCo2.16
        // micky0.65-909
        //MEDIA_MediaSelection_Exit(OSD_UI_EXIT_ALL);   // leave the UI normally
        //OSD_ChangeUI(OSD_UI_MEDIA_SELECT_DLG, OSD_EXIT_UI);   // recover UI
        __bMediaSelectUI = FALSE;
        _bShowNoMedia = FALSE;
#ifdef SUPPORT_STB
        __bMediaSelected = TRUE;
#endif
        // wyc1.20-909, re-srrange the flow to fix DVD <-> USB will play about more 500ms problem after switch mode.

        if (__bKey == KEY_SELECTMEDIA)
        {
            __bChooseMedia = _bPrevMedia;
            // the purpose of enable main video in MM mode here is because main video will disable after enter meida select UI.
            // So we need to restore it here.
            if (__wDiscType & BOOK_M1)
            {
                // wyc2.00-909S, only restore main video in MM UI mode because JPG slide show or motion mode we don't disable main video when entering media select UI.
                // Also will happen video scramble when open main video during JPG decoding here, so must only open it when MM UI mode.
                if ((__bMMMenuID != MM_MENU_NONE) || (__bThumbnailMode))
                {
                    DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
                    HAL_Display ( HAL_DISPLAY_SPST1, TRUE );
                }
            }
            OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_EXIT, OSD_UI_CLEAR_NORMAL);

#ifdef SUPPORT_STB                
            //elmer2.38, add CoCo2.38
            if(__bChooseMedia == MEDIA_SELECT_STB)
            {
                AP_ConfigGDIRegion(FALSE);
            }
#endif

#ifdef SUPPORT_STB
            if(__bPreMediaMode != MEDIA_SELECT_STB)                
#endif
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
                // wyc2.22-909s,
                CC_ForceResumeRePlay(CC_FORCE_STOP_FROM_MEDIA_SELECT_UI);
#else
            CC_KeyCommand(__bModeCmd);
#endif //
            break;
        }

        if (_bPrevMedia == _bNextMedia)
        {
            if (__wDiscType & BOOK_M1)
            {
                DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
                HAL_Display ( HAL_DISPLAY_SPST1, TRUE );
            }
            OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_EXIT, OSD_UI_CLEAR_NORMAL);
#ifdef SUPPORT_STB
            if(__bPreMediaMode != MEDIA_SELECT_STB)                
#endif
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
                // wyc2.22-909s,
                CC_ForceResumeRePlay(CC_FORCE_STOP_FROM_MEDIA_SELECT_UI);
#else
            CC_KeyCommand(__bModeCmd);
#endif //
#ifdef SUPPORT_STB
            //CoCo1.06, Fix the bug about DVB==>Media Select dialog==>DVB has OSD region problem in PAL mode.
            if(__bPreMediaMode == MEDIA_SELECT_STB)
            {
                AP_ConfigGDIRegion(FALSE);
            }
#endif //SUPPORT_STB
            return KEY_NO_KEY;
        }

        if (!_MEDIA_SwitchPreAct(_bPrevMedia))
        {
            return KEY_NO_KEY;
        }

        if(!_MEDIA_SwitchPostAct(_bNextMedia))
        {
            return KEY_NO_KEY;                
        }

        // wyc1.20-909, need to clear JPG buffer, otherwise when switch USB to auto play JPG and immediately switch to SERVO
        // before JPG decode completed, then will see screen appear green color a while.
        if (__bAttrPlay == ATTR_JPG)
        {
            HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_FRAME0);
            HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_FRAME1);
        }
        // wyc2.37-909s, don't call SrcFilter_ClearState here because will clear it in previous code.
        //SrcFilter_ClearState();
        MediaInfo[_MapSourceID2Index(SOURCE_USB0_0)].bState = MEDIA_NONE;
        __bChooseMedia = _bNextMedia;
        // wyc2.04-909S, need to remove all UI to avoid MM UI recover after exiting dialog select UI, the MM UI recover will make
        // screen flash a while then show LOGO.
        OSD_RemoveAllUI();
        OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_EXIT, OSD_UI_CLEAR_NORMAL); //CoCo1.50, "exit media select" dialog box before CC_ResetState( KEY_OPEN_CLOSE );
        CC_KeyCommand(KEY_STOP);
        //MEDIA_Removed(SOURCE_DISC);
        // wyc1.05-909, reset state to open/close and show LOGO when media removed.
        CC_ResetState( KEY_OPEN_CLOSE );
        // wyc1.05-2, need to clear OSD stack here.
        //OSD_NormalMode(); //CoCo1.50, don't call OSD_NormalMode(); again because CC_ResetState( KEY_OPEN_CLOSE ); will do it.
        UTL_ShowLogo();
        //OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_EXIT, OSD_UI_CLEAR_NORMAL); //--CoCo1.50, Can't exit any UI because all UI has been removed forcibly in CC_ResetState( KEY_OPEN_CLOSE );
        if (__wDiscType & BOOK_M1)
        {
            DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
            HAL_Display ( HAL_DISPLAY_SPST1, TRUE);
        }
        __bNavigateMode = NAVIGATE_NONE;
        MediaInfo[_MapSourceID2Index(__SF_SourceGBL[__dwSFStreamID].bSourceIndex)].bState = MEDIA_NONE;
        __bMediaUSBAction = MEDIA_ACTION_USB_NONE;
        __bMediaUSBState = MEDIA_USB_NONE;
        // elmer 2.32
        _dwMediaChkTime = OS_GetSysTimer();
        //cyg_thread_delay(150);
        // wyc2.75-909P, make USB_HCInit justed called one time.
#ifdef SUPPORT_USB_SOURCE
        if ((_bMediaInitUSB == FALSE) && (__bChooseMedia == MEDIA_SELECT_USB || __bChooseMedia == MEDIA_SELECT_CARD_READER))
        { 
            _bMediaInitUSB = TRUE;

#ifdef CTK_CARDREADER
            // elmer2.75-909P, move from card.c to here
            {
                extern BOOL CARD_SetupSystem(DWORD);

                CARD_SetupSystem(DECODER_SYSTEM);
            }
#endif
        }
#endif //
        return KEY_NO_KEY;
    case    KEY_OPEN_CLOSE:
    case    KEY_POWER:
        return KEY_BYPASS;
    default:
        return INVALID_KEY;
    }
    return KEY_NO_KEY;
}

//  *********************************************************************
//  Function    :   MEDIA_StateChange
//  Description :   This function will do the necessary action when a media is stopped/ removed.
//  Arguments   :   bMode
//  Return      :   none
//  Side Effect :
//  *********************************************************************
void MEDIA_StateChange(BYTE bMode)
{
    if (bMode & MEDIA_CONTROL_STOP)
    {
        CC_DoKeyAction(KEY_STOP);
    }
    if (bMode & MEDIA_CONTROL_KILL)
    {
#ifndef SUPPORT_REMOVABLE_MEDIA
        // Micky0.85
        if ( __SF_SourceGBL[__dwSFStreamID].bSourceIndex >= SOURCE_REMOVABLE_FIRST )
#endif  //#ifndef SUPPORT_REMOVABLE_MEDIA
            INFOFILTER_TriggerDeletingInfo(__SF_SourceGBL[__dwSFStreamID].bSourceIndex);
    }

    if (bMode & MEDIA_CONTROL_NONE)
    {
        UTL_ShowLogo();
    }
    if (bMode & MEDIA_CONTROL_CLEAR_ALL)
    {
#ifndef SUPPORT_REMOVABLE_MEDIA
        if ( __SF_SourceGBL[__dwSFStreamID].bSourceIndex >= SOURCE_REMOVABLE_FIRST)
#endif  //#ifndef SUPPORT_REMOVABLE_MEDIA
            INFOFILTER_TriggerDeletingInfo(SOURCE_ALL);
    }
}
//  *********************************************************************
//  Function    :   MEDIA_InitialState
//  Description :   This function will initialize all the media as clear state.
//  Arguments   :   none.
//  Return      :   none
//  Side Effect :
//  *********************************************************************
void MEDIA_InitialState()
{
    __SF_SourceGBL[__dwSFStreamID].bSourceIndex = SOURCE_DISC;
    _dwMediaChkTime = OS_GetSysTimer();
    __bMediaUSBAction = MEDIA_ACTION_USB_NONE;
    __bMediaUSBState = MEDIA_USB_NONE;

    __bMediaServoAction = MEDIA_SERVO_ACTION_NONE;
    __bMediaServoState = MEDIA_SERVO_NONE;
    _bMediaUSBConnPC = FALSE;
    _bTriggerCmdHD = FALSE;
    _bTriggerCmdChkCARD = FALSE;
    _bTriggerCmdExitUSBStack = FALSE;
    _bIssueCmdExitUSBStack = FALSE;
    _bMediaExitingUSBStack = FALSE;
    //_dwUSBSRCActivePort = SOURCE_USB0_0;

    _bShowNoMedia = FALSE;
    __bMediaSelectUI = FALSE;

    // elmer
    _bTriggerCmd1 = _bTriggerCmd2 = FALSE;

    for (__bTemp=0; __bTemp < TOTAL_MEDIA; __bTemp++)
    {
        MediaInfo[__bTemp].bState = MEDIA_NONE;
        MediaInfo[__bTemp].bPresent = 0;

    }
    SrcFilter_ClearState();
    //
/*
#ifdef SUPPORT_POWERON_MENU
    __bChooseMedia = MEDIA_SELECT_USB;
#else
    __bChooseMedia = MEDIA_SELECT_DVD;
#endif
*/
    __bChooseMedia = MEDIA_SELECT_DVD;
    //__bChooseMedia = MEDIA_SELECT_USB;
    __bNavigateMode = NAVIGATE_NONE;

}

//  *********************************************************************
//  Function    :   MEDIA_Management
//  Description :   This function will process the media open/close(insert/ remove)
//  Arguments   :   none.
//  Return      :   none
//  Side Effect :
//  *********************************************************************
extern BYTE _bOSDSSScreenSaverMode;

BYTE MEDIA_Management()
{
// [USB MEDIA]
#ifdef SUPPORT_USB_SOURCE    
    BYTE bTemp;

    if (__bMediaUSBAction == MEDIA_ACTION_USB_INSERT)
    {
        if (__bMediaUSBState == MEDIA_USB_INSERT)
        {
#ifdef DEBUG_MEDIA_STATUS
            //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nUSB Media Device Insert");
#endif //

            // elmer2.78, clear screensaver when parsing a device
            if (_bOSDSSScreenSaverMode)
            {
                OSD_RemoveUI(OSD_UI_SCREEN_SAVER);
                OSDSS_ResetTime();
                UTL_ShowLogo();
            }

            // wyc2.00-909S, don't show waiting message when setup mode.
            // show "loading" in the right-top corner
            if (!__bSetupMode)
                OSD_OUTPUT_MACRO(MSG_WAITING, OSD_SERVO_STABLE, 0xff);

            // Micky0.67-909, set source ID
            __SF_SourceGBL[__dwSFStreamID].bSourceIndex = SOURCE_USB0_0;        

            // [1] trigger command
            //printf("MEDIA: trigger USBSRC_CMD_RECOGNIZE_MEDIA\n");
            if (SrcFilter_TriggerUSBSRCCmd(SRCFILTER_FLAG_CMD, SRCFILTER_USBSRC_RECOGNIZE_MEDIA))
            {
#ifdef DEBUG_MEDIA_STATUS
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"Trigger USBSRC_CMD_RECOGNIZE_MEDIA\n");
#endif //

                _bTriggerCmd2 = TRUE;
            }

            if (_bTriggerCmd2 && SrcFilter_PeekUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_RECOGNIZE_MEDIA))
            {
#ifdef DEBUG_MEDIA_STATUS
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"Get USBSRC_CMD_RECOGNIZE_MEDIA OK\n");
#endif //
                _bTriggerCmd2 = FALSE;
                SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_RECOGNIZE_MEDIA);
                SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_RUNNING, SRCFILTER_USBSRC_RECOGNIZE_MEDIA);
                // [2] get status
                switch (SrcFilter_GetStatus(SOURCE_USB0_0))
                {
                case SRCFTR_USB_STATE_READY_MEDIA:
                    MediaInfo[__bActionMediaID].bState = MEDIA_RECOGNIZE;
                    __bMediaUSBAction = MEDIA_ACTION_USB_INSERT;
                    __bMediaUSBState = MEDIA_USB_RECOGNIZE;
#ifdef SUPPORT_POWERON_MENU
                    // set to 1 to indicate that it has chance to detect another USB port
#ifdef SUPPORT_USB_SWITCH
                    __bMediaRegCnt = 0;
#else
                    // elmer2.78, set 1 here stands for we have chances to detect another port when there
                    // is device in this port
                    __bMediaRegCnt = 1;
#endif
#endif /* SUPPORT_POWERON_MENU */
                    break;
                case SRCFTR_USB_STATE_NO_MEDIA:
                case SRCFTR_USB_STATE_NO_CARD:
                case SRCFTR_USB_STATE_WRONG_MEDIA:
                    MediaInfo[__bActionMediaID].bState = MEDIA_NONE;
                    __bMediaUSBAction = MEDIA_ACTION_USB_NONE;
                    __bMediaUSBState = MEDIA_USB_NONE;
                    break;
                }
            }
        }
        else if (__bMediaUSBState == MEDIA_USB_RECOGNIZE)
        {
#ifdef DEBUG_MEDIA_STATUS
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nUSB media device detected valid");
#endif
            if (__bMediaSelectUI == TRUE)
            {
                __bMediaSelectUI = FALSE;
                OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_EXIT, OSD_UI_CLEAR_NORMAL);
            }
            INFOFILTER_CFGUserData(DS_USERDATA_BUF_ST);
            INFOFILTER_TriggerParsingInfo(_MapIndex2SourceID(__bActionMediaID));
            __bMediaUSBState = MEDIA_USB_PARSING;
            //MediaInfo[_MapSourceID2Index(__SF_SourceGBL[__dwSFStreamID].bSourceIndex)].bState = MEDIA_PARSING;
            MediaInfo[__bActionMediaID].bState = MEDIA_PARSING;
        }
        else if (__bMediaUSBState == MEDIA_USB_PARSING)
        {
            bTemp = INFOFILTER_GetParsingStatus(_MapIndex2SourceID(__bActionMediaID));

            if (bTemp == INFOFILTER_PARSING)
            {
                // wyc2.00-909S, don't show waiting message when setup mode.
                if (!__bSetupMode)
                    OSD_OUTPUT_MACRO(MSG_WAITING, OSD_SERVO_STABLE, 0xff);
            }
            else
            {
                //__bMediaUSBState = MEDIA_USB_READY;
                if (bTemp == INFOFILTER_PARSE_FAILED)
                {
#ifdef  DEBUG_MEDIA_STATUS
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nUSB Media Wrong");
#endif //
                    MediaInfo[__bActionMediaID].bState = MEDIA_WRONG;
                    // if parse fail, don't process all suspend state, leave them as suspend
                }
                else
                {
#ifdef  DEBUG_MEDIA_STATUS
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nUSB Media Ready");
#endif //
                    MediaInfo[__bActionMediaID].bState = MEDIA_READY;
                    // if parse ok, clear all suspend state as none. re-checking the media status
                    _ResetSuspendMedia();
                }
                __bMediaUSBAction = MEDIA_ACTION_USB_NONE;
                __bMediaUSBState = MEDIA_USB_NONE;
                // wyc1.20a-909, need to disable IO mute when switch to USB mode. Because when H/W booting -> open tray -> switch 
                // to USB -> MP3 can hear audio. The reason is we called IO mute when booting, when tray opened, the IO still keep mute.
                // So need to disable IO mute when change mode to USB to avoid MP3 no audio problem.
                INITIAL_MediaSetting();
                // stop/ playing/ none-> insert-> stop
                // playing->insert-> stop
                // non media navigator-> insert-> play
                // infofilter will update __bNavigateMode if has playable format inside
                if (MediaInfo[__bActionMediaID].bState == MEDIA_READY)
                {
                    // current media manager has media playing
                    // wyc.check, Just handle one media now, so force to detect media and show MM UI.
                    {
                        // whenever switch to MediaManager, must auto play
                        __bFlowState |= FLOW_RELEASE_DETECTION;
                        __SF_SourceGBL[__dwSFStreamID].bSourceIndex = _MapIndex2SourceID(__bActionMediaID);

                        //MediaInfo[__bActionMediaID].bState = MEDIA_PLAY;
                    }
                    if (__wDiscType & CDROM_M1)
                    {
                        __bNavigateMode = MEDIA_NAVIGATE_MODE;
                        // wyc1.01-909, when USB detect correctly, need to set __btDriveOpen to FALSE. Otherwise SERVO will auto detect
                        // in function _MEDIA_MonitorServoStatus when __btDriveOpen is TRUE.
                        //__btDriveOpen = FALSE;
                    }
                    else
                    {
                        __bNavigateMode = DISC_NAVIGATE_MODE;
                    }
                }
                else if (__bNavigateMode == MEDIA_NAVIGATE_MODE)
                    // still doesn't contain any playable format
                {
                    __bFlowState |= FLOW_RELEASE_DETECTION;
#ifdef  DEBUG_MEDIA_STATUS
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nInvalid media");
#endif  //#ifdef  DEBUG_MEDIA_STATUS
                }
            }
        }
    }

    // suspend servo detection if under USB detection.
    if ( __bMediaUSBAction != MEDIA_ACTION_USB_NONE )
        return TRUE;
#endif // SUPPORT_USB_SOURCE

#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
    // [SERVO]
    if (__bMediaServoAction == MEDIA_SERVO_ACTION_CLOSE)
    {
        if ( __bMediaServoState == MEDIA_SERVO_CLOSED )
        {
            // wyc1.01-909, _MEDIA_MonitorServoStatus will check it and don't check it here.
            // Micky0.70, need to start unit.
#ifdef  DEBUG_MEDIA_STATUS
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nSERVO Unit Start");
#endif //
            // wyc2.17a-909S
            // Micky2.17c
            // wyc2.37-909s, must confit it here again because SERVO will clear it when open tray OK.
            UTL_Config_BufferMode(DUMP_BUFF_NORMAL_MODE);
#ifdef  SUPPORT_SERVO_SOURCE
            SrcFilter_CFGEDCBuff((DWORD)(&_wSrvEDCBuff), SERVO_EDC_BUFFER_SIZE);
#endif
            SrcFilter_StartUnit();
            if (__bMediaSelectUI == TRUE)
                CC_ResetState( KEY_OPEN_CLOSE );
            // wyc1.06-909, set source power down mode to FALSE when start unit.
            __bSFSourcePWDown = FALSE;
            // will be triggered by MEDIA_MonitorStatus
            /*
            // waiting tray closed to target
            // check if tray is closed
            __bTemp = S(); 
            if (__bTemp == SRC_TRAY_CLOSED)
            */
            {
                //__btDriveOpen = FALSE;
                __bMediaServoState = MEDIA_SERVO_STABLE;
                MediaInfo[_MapSourceID2Index(SOURCE_DISC)].bState = MEDIA_INSERT;
                // Micky0.67-909, set source ID
                __SF_SourceGBL[0].bSourceIndex = SOURCE_DISC;
                SrcFilter_TriggerGettingMediaStatus () ;
                // Micky0.76, don't issue OSD when setup
                if (!__bSetupMode)
                {
                    OSD_OUTPUT_MACRO(MSG_WAITING, OSD_SERVO_STABLE, 0xff);
                }

            }
        }
        else if (__bMediaServoState == MEDIA_SERVO_STABLE)
        {
            __bTemp = SRCFilter_PollingMediaStatus();
            if ( __bTemp == SRCFILTER_WAITING_STABLE )
            {
                // Micky0.76, don't issue OSD when setup
                if (!__bSetupMode)
                {
                    OSD_OUTPUT_MACRO(MSG_WAITING, OSD_SERVO_STABLE, 0xff);
                }
            }
            else
                if ( __bTemp == SRCFILTER_HAVEDISC )
                {
#ifdef  DEBUG_MEDIA_STATUS
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nSERVO Have Disc");
#endif //
                    __bMediaServoState = MEDIA_SERVO_PARSING;
                    // wyc2.36a-909s,
#ifdef SUPPORT_FUSS_MPEG_CD
                    INFOFILTER_FUSSMPEGCD(IF_MODE_CONFIG_SUPPORT, TRUE);
#endif //
                    INFOFILTER_CFGUserData(DS_USERDATA_BUF_ST);
                    INFOFILTER_TriggerParsingInfo(SOURCE_ATAPI);
                    MediaInfo[_MapSourceID2Index(SOURCE_DISC)].bState = MEDIA_PARSING;

                }
                else
                    if ( __bTemp == SRCFILTER_NODISC )
                    {
#ifdef  DEBUG_MEDIA_STATUS
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nSERVO No Disc");
#endif //
                        MediaInfo[_MapSourceID2Index(SOURCE_DISC)].bState = MEDIA_NONE;
                        __bMediaServoState = MEDIA_SERVO_PARSING_DONE ;
                        if ( _QuerySuspendMedia() )
                        {
                            // if parse ok, clear all suspend state as none. re-checking the media status
                            _ResetSuspendMedia();
                            __bMediaServoAction = MEDIA_SERVO_ACTION_NONE;
                            __bMediaServoState = MEDIA_SERVO_NONE;
                        }
                    }
                    else if ( __bTemp == SRCFILTER_WRONGDISC)
                    {
#ifdef  DEBUG_MEDIA_STATUS
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nSERVO Wrong Disc");
#endif //
                        MediaInfo[_MapSourceID2Index(SOURCE_DISC)].bState = MEDIA_WRONG;
                        __bMediaServoState = MEDIA_SERVO_PARSING_DONE ;
                        __wDiscType = BOOK_M1;
                        __bDiscInfo |= DISCINFO_UNRECOGNIZE;
                    }
        }
        else if ( __bMediaServoState == MEDIA_SERVO_PARSING )
        {
            __bTemp = INFOFILTER_GetParsingStatus(SOURCE_ATAPI);
            if (__bTemp == INFOFILTER_PARSING )
            {
                // Micky0.76, don't issue OSD when setup
                if (!__bSetupMode)
                {
                    OSD_OUTPUT_MACRO(MSG_WAITING, OSD_SERVO_STABLE, 0xff);
                }
            }
            else
            {
                // Micky0.76, don't go further when SETUP mode.
                // can't do this, as infofilter will clear the status(INFOFILTER_PARSE_OK) once checked.
                // the flow will be suspend by FlowState.
                //              if (__bSetupMode)
                //              {
                //                  return FALSE;
                //              }

                if (__bTemp == INFOFILTER_PARSE_OK )
                {
#ifdef  DEBUG_MEDIA_STATUS
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nSERVO Parse Disc OK");
#endif //
                    MediaInfo[_MapSourceID2Index(SOURCE_DISC)].bState = MEDIA_READY;
                }
                else  if (__bTemp == INFOFILTER_PARSE_FAILED )
                {
#ifdef  DEBUG_MEDIA_STATUS
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nSERVO Parse Disc Fail");
#endif //
                    MediaInfo[_MapSourceID2Index(SOURCE_DISC)].bState = MEDIA_WRONG;
                }
                __bMediaServoState = MEDIA_SERVO_PARSING_DONE;
                // Micky0.64-909
                if (__wDiscType & CDROM_M1)
                {
                    __bNavigateMode = MEDIA_NAVIGATE_MODE;
                }
                else
                {
                    __bNavigateMode = DISC_NAVIGATE_MODE;
                }
                // prompt UI for suspend media
                if ( __bNavigateMode == DISC_NAVIGATE_MODE )
                {
                    if ( _QuerySuspendMedia() )
                    {
                        if (__bMediaSelectUI == FALSE)
                        {
                            //OSD_ChangeUI(OSD_UI_MEDIA_SELECT_DLG, OSD_ENTER_UI);  // enter the UI //CoCo1.50, remove it because OSDDLG_MEDIA_SELECT_Update() will do it.
                            OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_SHOW_DLG, OSDDLG_MEDIA_SELECT_BACKGROUND_GENERAL_MODE);
                            OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_DRAW_HIGHLIGHT, 0);
                            __bMediaSelectUI = TRUE;
                            __bChooseMedia = MEDIA_SELECT_DVD;
                            __bMediaServoState = MEDIA_SERVO_PROMPT;
                        }
                    }
                }
                else
                {
                    // CDROM disc-> keep checking media
                    // invalid disc-> keep checking media
                    // media navigator or none
                    if ( _QuerySuspendMedia() )
                    {
                        // if parse ok, clear all suspend state as none. re-checking the media status
                        _ResetSuspendMedia();
                        __bMediaServoAction = MEDIA_SERVO_ACTION_NONE;
                        __bMediaServoState = MEDIA_SERVO_NONE;
                    }
                    // if no suspend media, just show disc type
                }
            }
        }
        else if ( __bMediaServoState == MEDIA_SERVO_PARSING_DONE )
        {
#ifdef  DEBUG_MEDIA_STATUS
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nSERVO Init Setting");
#endif //
            INITIAL_MediaSetting();
            __bMediaServoAction = MEDIA_SERVO_ACTION_NONE;
            __bMediaServoState = MEDIA_SERVO_NONE;
            __bFlowState |= FLOW_RELEASE_DETECTION;

            if (__wDiscType & CDROM_M1)
            {
                __bNavigateMode = MEDIA_NAVIGATE_MODE;
                MediaInfo[_MapSourceID2Index(SOURCE_DISC)].bState = MEDIA_PLAY;
            }
            else
            {
                __bNavigateMode = DISC_NAVIGATE_MODE;
                MediaInfo[_MapSourceID2Index(SOURCE_DISC)].bState = MEDIA_PLAY;
            }
        }
    }
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p

    return TRUE;
}

//***************************************************************************
//  Function    :   ::MEDIA_MonitorStatus
//  Abstract    :   Monitor all media Status by every 200 msec.
//  Arguments   :
//  Return      :   NONE
//  Side Effect :   
//  Notes       :   PowerDown mode will ignore it.
//***************************************************************************
// monitor the servo/media status.
void MEDIA_MonitorStatus(void)
{
    if ( __btPowerDown )
        return;
    // elmer2.38, elmer2.38a fix to 200ms
    if ((OS_GetSysTimer() - _dwMediaChkTime) >= COUNT_200_MSEC)
    {
        _dwMediaChkTime = OS_GetSysTimer();
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
        _MEDIA_MonitorServoStatus();
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
        _MEDIA_MonitorMediaStatus();
    }
}

// wyc2.17-909S, add new function to make it execute before setup processing key.
//***************************************************************************
//  Function    :   MEDIA_MonitorTOPOPEN
//  Abstract    :   Monitor all media Status by every 200 msec.
//  Arguments   :
//  Return      :   NONE
//  Side Effect :   
//  Notes       :   PowerDown mode will ignore it.
//***************************************************************************
// monitor the servo top open status.
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
void MEDIA_MonitorTOPOPEN(void)
{
#ifdef SUPPORT_TOP_OPEN
    if ( __btPowerDown )
        return;
    // wyc2.01-909S, modify the parameter by 909S mode.
    if(HAL_ReadGPIO(GPIO_A, PIN_TOP_OPENCLOSE))
    { //OPEN
        if(__bCloseCommand == 1)
            __bISRKey = KEY_OPEN_CLOSE;
    }
    else
    {//CLOSE
        if(__bCloseCommand == 0)
            __bISRKey = KEY_OPEN_CLOSE;
    }
#endif

    // wyc2.38a-909s, add slot in code.
#ifdef SUPPORT_SLOT_IN      // KCHong 2.17, for slot-in loader
    if (__bChooseMedia == MEDIA_SELECT_DVD)
        if ( ( OS_GetSysTimer()- __dwCloseTrayTimer ) > COUNT_3_SEC )
        {
            //        if(!HAL_ReadGPIO(0x6, 15))        // Old circuit
            if(HAL_ReadGPIO(0x6, 15))         // New circuit
            {
                if(__bCloseCommand == 1)
                {
                    __bFWKey = KEY_OPEN_CLOSE;
                }
            }
            __dwCloseTrayTimer = OS_GetSysTimer();
        }
#endif
}
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p

//***************************************************************************
//  Function    :   ::_MEDIA_MonitorServoStatus
//  Abstract    :   Monitor Servo Status. May issue KEY_OPEN_CLOSE if needed.
//  Arguments   :
//  Return      :   NONE
//  Side Effect :   
//  Notes       :
//***************************************************************************
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
void _MEDIA_MonitorServoStatus()
{
    extern  BYTE    _bOpen;
    // wyc1.05-909, don't need to monitor SERVO status when system selected in media mode.
    if (__bChooseMedia != MEDIA_SELECT_DVD)
        return;
    // monitor servo
    /// 9.1) Check Tray Open by extra force(such as Eject Button)
    // TCC161, for checking DVD tray-out by outer forces
#ifdef SUPPORT_LOADER_OPEN_BY_EXTRA_FORCE
    if ( !__btDriveOpen )
    {
        // TCC172, check tray open for a period of 500msec
        if (OS_GetSysTimer() - _dwTimeCheckTray >= COUNT_500_MSEC)
        {
            if (SrcFilter_CheckTrayStatus () != SRC_TRAY_CLOSED)
            {
#ifdef SUPPORT_PRINTF
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"DVDROM Tray was opened by outer forces !\n") ;
#endif
                SrcFilter_EnableRunning (FALSE) ;
                __bFWKey = KEY_OPEN_CLOSE ;
            }
            _dwTimeCheckTray = OS_GetSysTimer() ;
        }
    }
#endif      // ** TCH1.05; #ifdef SUPPORT_LOADER_OPEN_BY_EXTRA_FORCE

    /// 9.2) Check Tray was really closed(By IR Key or extra force)
    // Will monitor 2 things(every 1s):
    // **** Open tray-> but the tray was still cloesd over 6s.
    //      - Will do SrcFilter_ResetRecover, and open tray again.
    // **** Open tray-> but tray was closed by extra force
    // **** Close tray-> and the tray was closed
    //      - Leave screen saver and OSD_Recover
    //      - Will do INITIAL_InitialState to do title detection
    //      - But when SETUP mode, will just set __bFlowState(every 1s)
    // ???? Found one operation has problem.(when the flow was encountered during setup mode)
    //      Open tray-> setup-> after 1sforce close tray->case, Open tray-> _bOpen will be cleared till tray was detected as opened
    //      But if 
    // Micky1.00_909_media, remove 9.2
    // media.c will do the checking
    if ( __btDriveOpen )
    {
        // wyc1.07-909, Andy suggested to using 200ms to check SERVO tray status.
        if (OS_GetSysTimer() - _dwTimeCheckTray >= COUNT_200_MSEC)
        {
            __bTemp = SrcFilter_CheckTrayStatus (); 

            if (_bOpen)
            {
                // If tray was not closed, means tray opened successfully.
                if (__bTemp == SRC_TRAY_OPENED) 
                {
                    _bOpen = 0 ;
                }
                // wyc2.38a-909s, descrease the time for sooner reponse.
                else if (((OS_GetSysTimer() - __dwOpenTime) > (COUNT_3_SEC*5)) && !__bCloseCommand)
                {
                    //LJY1.25, Support open tray when servo fails to close tray.
#ifndef SUPPORT_OPENTRAY_WHEN_CLOSETRAY_FAILED                
                    SrcFilter_ResetRecover(FALSE);
                    SrcFilter_OpenTray();
#else
                    __bFWKey = KEY_OPEN_CLOSE;
#endif                                    
                }
            }
            else
            {
                // ** TCH1.05; begin... 
                /*
                   if (__bTemp == SRC_TRAY_CLOSED)
                   {
                   _bOpen = 1 ;
                   OSD_OUTPUT_MACRO ( MSG_OPEN, FALSE, 3 );
                   __bMediaServoAction = MEDIA_SERVO_ACTION_CLOSE;
                   __bMediaServoState = MEDIA_SERVO_CLOSED;
                   __btDriveOpen = FALSE;
                   }
                // Support open tray when servo fails to close tray
#ifdef SUPPORT_OPENTRAY_WHEN_CLOSETRAY_FAILED                
else if(__bTemp == SRC_TRAY_OPENING)
{
__bFWKey = KEY_OPEN_CLOSE;
}
#endif           
*/
switch ( __bTemp )
{
case SRC_TRAY_CLOSED:
    {                           
        _bOpen = 1 ;
#ifndef SUPPORT_TOP_OPEN
        OSD_OUTPUT_MACRO ( MSG_OPEN, FALSE, 3 );
#endif //
        __bMediaServoAction = MEDIA_SERVO_ACTION_CLOSE;
        __bMediaServoState = MEDIA_SERVO_CLOSED;
        __btDriveOpen = FALSE;
    }
    break;

    // ** TCH1.05; Do the open-close if Tray is pushed by user.
case SRC_TRAY_USER_PUSH:   
    {
        __bFWKey = KEY_OPEN_CLOSE;                        
    }
    break;

    // Support open tray when servo fails to close tray
#ifdef SUPPORT_OPENTRAY_WHEN_CLOSETRAY_FAILED                
case SRC_TRAY_OPENING:
    {
        __bFWKey = KEY_OPEN_CLOSE;
    }
    break;
#endif           

    // wyc2.51-909P, when define, will open tray when can't close OK after 3s.
#ifdef SUPPORT_OPENTRAY_WHEN_CLOSETRAY_FAILED                
case SRC_TRAY_CLOSING:
    {
        if (((OS_GetSysTimer() - __dwOpenTime) > (COUNT_3_SEC)))
            __bFWKey = KEY_OPEN_CLOSE;
    }
    break;
#endif           

}   // switch ( __bTemp)                    
// ** TCH1.05; end...                      
}
_dwTimeCheckTray = OS_GetSysTimer() ;
}
}
}
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p

//wyc2.78-909P, the function to monitor the OTG mode and will config the usb / card status by the usb mode.
#ifdef SUPPORT_USB_HOST_DEVICE
void MEDIA_MonitorAndConfigUSBOTG(void)
{
#if (USB_SYSTEM == USB2_0_JUNGO)
    BYTE bTemp;
    extern DWORD _dwOSDSSMoveTime;

    if (SrcFilter_GetOTGMode() == USB_MODE_NONE)
    {
        _dwMediaUSBOTGChkTime = OS_GetSysTimer();
        return;
    }
    
    if ((OS_GetSysTimer()-  _dwMediaUSBOTGChkTime) >= COUNT_200_MSEC)
    {
        if (_bTriggerCmdHD == FALSE)
        {
#ifdef DEBUG_MEDIA_STATUS
            printf("\nOTG ID:%hx, USB mode:%hx Power:%hx",USB_GetOTGID(),SrcFilter_GetOTGMode(), USB_ConnectToPC());
#endif //
            // the code to monitor OTG status and switch host and device.
            bTemp = USB_GetOTGID();
            if (SrcFilter_GetOTGMode() != bTemp)
            {
                if (bTemp == USB_MODE_HOST)
                {
#ifdef DEBUG_MEDIA_STATUS
                     printf("\nMedia -> Enter Host mode");
#endif //
#ifdef SUPPORT_USB_POWER_BY_IO
                    HAL_WriteGPIO(USB_POWER_GRP, PIN_USB_POWER, 1);
#endif //
                    SrcFilter_SetOTGMode(USB_MODE_HOST);
                    if (SrcFilter_TriggerUSBSRCCmd(SRCFILTER_FLAG_CMD, SRCFILTER_USBSRC_SWITCH_USB_STACK))
                    {
                        _bTriggerCmdHD = TRUE;
                    }
                }
                else if (bTemp == USB_MODE_DEVICE)
                {
#ifdef SUPPORT_USB_POWER_BY_IO
                    HAL_WriteGPIO(USB_POWER_GRP, PIN_USB_POWER, 0);
#endif //
                    if (USB_ConnectToPC())
                    {
#ifdef DEBUG_MEDIA_STATUS
                        printf("\nMedia -> Enter Device mode");
#endif //
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
                        POWERONMENU_Exit(OSD_UI_EXIT_ALL); 
                        POWERONMENU_Initial(); 
#endif //
                        SrcFilter_SetOTGMode(USB_MODE_DEVICE);
                        if (SrcFilter_TriggerUSBSRCCmd(SRCFILTER_FLAG_CMD, SRCFILTER_USBSRC_SWITCH_USB_STACK))
                        {
                            _bTriggerCmdHD = TRUE;
                        }
                    }
                    
                }
            }
        }

            
        if ((_bTriggerCmdHD == FALSE) && (SrcFilter_GetOTGMode() == USB_MODE_DEVICE) && (USB_ConnectToPC() == FALSE))
        {
#ifdef DEBUG_MEDIA_STATUS
             printf("exit device stack because no connect to PC");
#endif //
            _bIssueCmdExitUSBStack = TRUE;
        }

        if ((_bTriggerCmdChkCARD == FALSE) && ((SrcFilter_GetOTGMode() == USB_MODE_DEVICE) || (SrcFilter_GetOTGMode() == USB_MODE_EXIT_MODE)))
        {
            if (SrcFilter_TriggerUSBSRCCmd(SRCFILTER_FLAG_CMD, SRCFILTER_USBSRC_DEVICE_MODE_CHK_CARD))
            {
                _bTriggerCmdChkCARD = TRUE;
            }
        }

        if ((_bIssueCmdExitUSBStack == TRUE) && (_bTriggerCmdExitUSBStack == FALSE))
        {
            if (SrcFilter_TriggerUSBSRCCmd(SRCFILTER_FLAG_CMD, SRCFILTER_USBSRC_EXIT_USB_STACK))
            {
                _bTriggerCmdExitUSBStack = TRUE;
            }
        }
        
        // check the command status about switching host/device & CARD checking status.
        if (_bTriggerCmdHD && SrcFilter_PeekUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_SWITCH_USB_STACK))
        {
            _bTriggerCmdHD = FALSE;
            SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_SWITCH_USB_STACK);
            SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_RUNNING, SRCFILTER_USBSRC_SWITCH_USB_STACK);
        }
        if (_bTriggerCmdChkCARD && SrcFilter_PeekUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_DEVICE_MODE_CHK_CARD))
        {
            _bTriggerCmdChkCARD = FALSE;
            SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_DEVICE_MODE_CHK_CARD);
            SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_RUNNING, SRCFILTER_USBSRC_DEVICE_MODE_CHK_CARD);
        }
        if (_bIssueCmdExitUSBStack && _bTriggerCmdExitUSBStack && SrcFilter_PeekUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_EXIT_USB_STACK))
        {
            _bTriggerCmdExitUSBStack = FALSE;
            _bIssueCmdExitUSBStack = FALSE;
            SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_EXIT_USB_STACK);
            SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_RUNNING, SRCFILTER_USBSRC_EXIT_USB_STACK);
        }

        // some function need to monitor in USB device mode.
        if ((_bTriggerCmdHD == FALSE) && (_bTriggerCmdExitUSBStack == FALSE) && (SrcFilter_GetOTGMode() != USB_MODE_HOST))
        {
            if (USB_ConnectToPC() == TRUE)
            {
                OSDSS_ResetTime();
                _dwOSDSSMoveTime = OS_GetSysTimer();
            }

            if (_bMediaExitingUSBStack)
            {
                _bMediaExitingUSBStack = FALSE;
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
                POWERONMENU_Exit(OSD_UI_EXIT_ALL); 
                POWERONMENU_Initial(); 
#endif //
            }
            
            if ((_bMediaUSBConnPC == FALSE) && (USB_ConnectToPC() == TRUE))
            {
                OSD_RemoveAllUI();
                NOTEDLG_ShowUSBDevideNotifyDlg(USBDEVICE_NOTIFY_DLG_TYPE_CONNECT);
                _bMediaUSBConnPC = TRUE;
            }
            else if ((_bMediaUSBConnPC == TRUE) && (USB_ConnectToPC() == FALSE))
            {
                NOTEDLG_ExitUSBDevideNotifyDlg();
                //NOTEDLG_ShowUSBDevideNotifyDlg(USBDEVICE_NOTIFY_DLG_TYPE_POWER_DOWN);
                _bIssueCmdExitUSBStack = TRUE;
                _bMediaUSBConnPC = FALSE;
                _bMediaExitingUSBStack = TRUE;
            }
        }


        _dwMediaUSBOTGChkTime = OS_GetSysTimer();
    }
#endif //
}
#endif //

//***************************************************************************
//  Function    :   ::_MEDIA_MonitorMediaStatus()
//  Abstract    :   Monitor USB Status. 
//  Arguments   :
//  Return      :   NONE
//  Side Effect :   
//  Notes       :
//***************************************************************************
void _MEDIA_MonitorMediaStatus()
{
#ifdef SUPPORT_USB_SOURCE
    //static BYTE bTriggerCmd = FALSE;
    BYTE bTemp;

    // wyc2.78-909P, call this function frequently.
#ifdef SUPPORT_USB_HOST_DEVICE
    MEDIA_MonitorAndConfigUSBOTG();
#endif //

#ifdef SUPPORT_CHOOSE_MEDIA
    // elmer2.78, only initialize card & USB once
    if (_bMediaInitUSB == FALSE)
    { 
        _bMediaInitUSB = TRUE;

#ifdef CTK_CARDREADER
        // elmer2.75-909P, move from card.c to here
        {
            extern BOOL CARD_SetupSystem(DWORD);

            CARD_SetupSystem(DECODER_SYSTEM);

            // initialize CTK card-reader
            if (!CARD_InitSystem())
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Initializing card-reader system is failing ...\n");
        }
#endif
#if 0  //xuli.choose media from Elmer
        // elmer2.75, power on USB and only do once
        HAL_PowerControl(HAL_POWER_USB, HAL_POWER_NORMAL);
        {
#if (USB_SYSTEM == USB2_0_JUNGO)
         //   extern void USB_HCInit(BYTE, DWORD, DWORD);

#if (DECODER_SYSTEM == DVD909P_A_EVAL)
            HAL_WriteGPIO( GPIO_EXP, 4, 1);
#endif //
#ifdef SUPPORT_USB_SWITCH
            HAL_WriteGPIO(GPIO_A, 23, 0);
#endif
            // wyc2.76-909P,
           // USB_HCInit(0, DS_USB_MEMPOOL_ST, (DS_USB_MEMPOOL_END - DS_USB_MEMPOOL_ST));
           USB_HCInit(USB_MODE_HOST, 0, DS_USB_MEMPOOL_ST_HOST, (DS_USB_MEMPOOL_END_HOST - DS_USB_MEMPOOL_ST_HOST));

#elif (USB_SYSTEM == USB1_1_CT909R)
            USB_HCInit(SOURCE_USB1_FIRST, 0xFF, DS_USB_BUFF_ST);
#endif
        }
#endif  //xuli.choose media from Elmer
    }

    // elmer2.78, remove on testing
//    if (_bFirstCheckPort)
//    {
//        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF," enter check port ...\n");
//        MEDIA_USB();
//        MEDIA_ExitUSBSRC();
//        _bFirstCheckPort = 0;
//        OS_YieldThread();
//    }

    // elmer.card
    {
        static DWORD dwChkTime1 = 0;
        DWORD dwPort1Status = 0, dwPort2Status = 0;
        extern BYTE _bOSDSSScreenSaverMode;

        if (!_bDisableCheckPort && (!__btPlaying || _bOSDSSScreenSaverMode) && ((OS_GetSysTimer() - dwChkTime1) > COUNT_1_SEC) /*&& (MediaInfo[2].bState == MEDIA_NONE)*/)
        {
            //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, "... check port status:");
#if (USB_SYSTEM == USB1_1_CT909R)
            USB_ReportPortLunStatus(&dwPort1Status, &dwPort2Status);
#endif
#if (USB_SYSTEM == USB2_0_JUNGO)
            dwPort2Status =  USB_CheckConnect();
#endif
#ifdef CTK_CARDREADER
            dwPort1Status = _MEDIA_GetCardReaderStatus(CARD_CardStatus_Inserted());
#endif
            _dwPortStatus = (dwPort2Status << 16);
            _dwPortStatus |= (dwPort1Status & 0x0000FFFF);

            if (_dwPortStatus != _dwPrePortStatus)
            {
                DVDSETUP_UpdateMediaStatus();
                _dwPrePortStatus = _dwPortStatus;
            }
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, "... Port Status: %lx\n", _dwPortStatus);
            dwChkTime1 = OS_GetSysTimer();
        }
    }
#endif // SUPPORT_CHOOSE_MEDIA

    if (__bChooseMedia == MEDIA_SELECT_DVD)
        return;

    if (__bMediaServoAction != MEDIA_SERVO_ACTION_NONE)
    {
        __bMediaServoAction = MEDIA_SERVO_ACTION_NONE;
        __bMediaServoState = MEDIA_SERVO_NONE;
    }


    for (_dwMediaTemp1 = _bNextServiceID; _dwMediaTemp1 < _bNextServiceID + MAX_REMOVABLE_MEDIAS; _dwMediaTemp1++)
    {
        if (SrcFilter_TriggerUSBSRCCmd(SRCFILTER_FLAG_CMD, SRCFILTER_USBSRC_CHECK_DEVICE))
        {
            //printf("Trigger USBSRC_CMD_CHECK_DEVICE OK ... 1\n");
            _bTriggerCmd1 = TRUE;
        }

        if (_bTriggerCmd1 && SrcFilter_PeekUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_CHECK_DEVICE))
        {
            _bTriggerCmd1 = FALSE;
            SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_CHECK_DEVICE);
            SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_RUNNING, SRCFILTER_USBSRC_CHECK_DEVICE);

            if (MediaInfo[_dwMediaTemp1].bState == MEDIA_NONE)
            {
                switch (bTemp = SrcFilter_GetStatus(SOURCE_USB0_0))
                {
                case SRCFTR_USB_STATE_CONN_MEDIA:
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"[1] connect media\n");

                    if (_bOSDSSScreenSaverMode)
                    {
                        OSD_RemoveUI(OSD_UI_SCREEN_SAVER);
                        OSDSS_ResetTime();
                        UTL_ShowLogo();
                    }

                    OSD_NormalMode();
                    OSDSS_Reset();
                    _bShowNoMedia = FALSE;
                    __bNavigateMode = MEDIA_NAVIGATE_MODE;
                    __bActionMediaID = _dwMediaTemp1;
                    __SF_SourceGBL[__dwSFStreamID].bSourceIndex = _MapIndex2SourceID(__bActionMediaID);

                    MediaInfo[_dwMediaTemp1].bState = MEDIA_INSERT;
                    __bMediaUSBAction = MEDIA_ACTION_USB_INSERT;
                    __bMediaUSBState = MEDIA_USB_INSERT;
                    break;

                case SRCFTR_USB_STATE_NO_MEDIA:
                case SRCFTR_USB_STATE_NO_CARD:

                    __wDiscType = NO_DISC_IN_DRIVE;

                    if (_bShowNoMedia == FALSE)
                    {
                        switch (bTemp)
                        {
                        case SRCFTR_USB_STATE_NO_MEDIA:
                            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"[2] originally no media\n");
                            break;
                        case SRCFTR_USB_STATE_NO_CARD:
                            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"[2] originally no card\n");
                            break;
                        case SRCFTR_USB_STATE_WRONG_MEDIA:
                            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"[2] originally wrong media\n");
                            break;
                        }

                        // elmer2.78, clear screensaver
                        if (_bOSDSSScreenSaverMode)
                        {
                            OSD_RemoveUI(OSD_UI_SCREEN_SAVER);
                            OSDSS_ResetTime();
                            UTL_ShowLogo();
                        }
                        UTL_ShowLogo();
#ifdef SUPPORT_POWERON_MENU
                        if (__bMediaRegCnt)
                        {
                            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"USBSRC: Change to next USB port ...\n");
                            __bMediaRegCnt--;
#ifdef SUPPORT_USB_SWITCH
                            if (__bMediaRegCnt == 0)
                                HAL_WriteGPIO(GPIO_A, 23, 1);
#endif
                            OS_REORDER_BARRIER();
                            MEDIA_USB();
                        }
                        else
                        {
#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
                            if (!MM_PlayPhotoInFlash())
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
                            {
                                MEDIA_ExitUSBSRC();
                                OS_YieldThread();
                                POWERONMENU_Initial();
                            }
                        }
#else // !SUPPORT_POWERON_MENU
                        _bShowNoMedia = TRUE;
                        OSD_OUTPUT_MACRO(MSG_DISCTYPE, __wDiscType, 0xFF);
#endif // SUPPORT_POWERON_MENU
                    }
                    break;

                case SRCFTR_USB_STATE_READY_MEDIA:
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"[3] insert card\n");
                    // clear "USB" in the right-top corner when re-inserting card
                    OSD_NormalMode();
                    OSDSS_Reset();
                    //
                    _bShowNoMedia = FALSE;
                    __bActionMediaID = _dwMediaTemp1;
                    __SF_SourceGBL[__dwSFStreamID].bSourceIndex = _MapIndex2SourceID(__bActionMediaID);
                    __bMediaUSBAction = MEDIA_ACTION_USB_INSERT;
                    __bMediaUSBState = MEDIA_USB_RECOGNIZE;
                    break;

                case SRCFTR_USB_STATE_WRONG_MEDIA:
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"[4] originally wrong media\n");

                    // wyc2.71-portusb,
#if (USB_SYSTEM == USB1_1_CT909R)
                    USB_ClearStatus();
#endif //
                    UTL_ShowLogo();
#ifdef SUPPORT_POWERON_MENU
                    if (__bMediaRegCnt)
                    {
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"USBSRC: Change to next USB port ...\n");
                        __bMediaRegCnt--;
#ifdef SUPPORT_USB_SWITCH
                        if (__bMediaRegCnt == 0)
                            HAL_WriteGPIO(GPIO_A, 23, 1);
#endif
                        OS_REORDER_BARRIER();
                        MEDIA_USB();
                    }
                    else
                    {
#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
                        if (!MM_PlayPhotoInFlash())
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
                        {
                            MEDIA_ExitUSBSRC();
                            OS_YieldThread();
                            POWERONMENU_Initial();
                        }
                    }
#else // !SUPPORT_POWERON_MENU
                    _bShowNoMedia = TRUE;
                    OSD_OUTPUT_MACRO(MSG_DISCTYPE, __wDiscType, 0xFF);
#endif // SUPPORT_POWERON_MENU
                    break;
                }
            }
            else
            {
                switch (bTemp = SrcFilter_GetStatus(SOURCE_USB0_0))
                {
                case SRCFTR_USB_STATE_NO_MEDIA:
                case SRCFTR_USB_STATE_NO_CARD:

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
                    // if card is removed, then stop the audio
                    UTL_StopAudio();
#endif //
                    if (MediaInfo[_dwMediaTemp1].bState == MEDIA_PARSING)
                    {
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"parsing... parsing...\n");
                        INFOFILTER_AbortParsing();
                    }

                    if (bTemp == SRCFTR_USB_STATE_NO_MEDIA)
                    {
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"[5] remove media\n");
                        //MediaInfo[_dwMediaTemp1].bState = MEDIA_NONE;
                    }
                    else
                    {
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"[6] remove card\n");
                        //MediaInfo[_dwMediaTemp1].bState = MEDIA_INSERT;
                    }

                    MediaInfo[_dwMediaTemp1].bState = MEDIA_NONE;
                    __bMediaUSBAction = MEDIA_ACTION_USB_NONE;
                    __bMediaUSBState = MEDIA_USB_NONE;

                    CC_KeyCommand(KEY_STOP);
                    CC_ResetState(KEY_OPEN_CLOSE);

                    SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_CMD, SRCFILTER_USBSRC_CMD_ALL);
                    // wait for USBSRC to enter the waiting command stage
                    OS_YieldThread();
                    SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_STATUS, SRCFILTER_USBSRC_CMD_ALL);
                    SrcFilter_ClearUSBSRCCmd(SRCFILTER_FLAG_RUNNING, SRCFILTER_USBSRC_CMD_ALL);

#if (USB_SYSTEM == USB1_1_CT909R)
                    // [Note], must do this action in USB1.1, or it will not read well next time
                    // in USB2.0, this action is not necessary.
                    // wyc2.76-909P,
#ifndef CT950_STYLE
                    USB_CheckStatus();
#endif //
#endif //
                    break;
                }
            }
        }
    }
#endif // #ifdef SUPPORT_USB_SOURCE    
}
// Micky_909_play_item
/*
   Power on:
   1. Check SERVO present-> Do SERVO
   2. SERVO not present-> Scan all media presense
   */

//  *********************************************************************
//  Function    :   MEDIA_Removed
//  Description :   This function will do the must action when a media was removed
//  Arguments   :   bSource : The source ID
//  Return      :   None
//  Side Effect :
//  *********************************************************************
void    MEDIA_Removed(BYTE bSource)
{
    // servo may have these state, no matter dsic or media navigator, the action are the same.
    // none ->open, do nothing
    // parsing  -> open-> abort parsing
    // ready    -> open-> delete info
    // invalid  -> open-> delete info
    // playing  -> open-> stop-> delete info

    // MEDIA remove
    // media may have these state
    // parsing  -> remove-> abort parsing
    // ready    -> remove-> delete info
    // invalid  -> remove-> delete info
    // playing  -> remove-> stop-> delete info
    // disc navigator-> remove-> do nothing

    if ( MediaInfo[_MapSourceID2Index(bSource)].bState == MEDIA_PLAY )
    {
        MEDIA_StateChange(MEDIA_CONTROL_STOP);
    }
    else if ( MediaInfo[_MapSourceID2Index(bSource)].bState == MEDIA_PARSING )
    {
        INFOFILTER_AbortParsing();
        // Micky0.65-909, clear waiting message.        
        OSD_Output(MSG_WAITING, 0, 0);
    }
    // wyc0.85, add one case MEDIA_INSERT, also need to abort this case because it will take long time to do it.
    // wyc0.85a, Micky forget to update index of MediaInfo to _MapSourceID2Index, so need to update it. Otherwise disc recognized
    // failed after open -> close when disc is parsing.
    else if ( MediaInfo[_MapSourceID2Index(bSource)].bState == MEDIA_INSERT )
    {
        SRCFilter_AbortGetMediaStatus();
    }    
    if ( MediaInfo[_MapSourceID2Index(bSource)].bState == MEDIA_READY || MediaInfo[_MapSourceID2Index(bSource)].bState == MEDIA_PLAY)
    {
        // delete information
        MEDIA_StateChange(MEDIA_CONTROL_KILL);
    }
    // InfoFilter will update __bNavigateMode
    if ( __bNavigateMode == MEDIA_NAVIGATE_MODE )
    {
        // stay at MM menu
        _MEDIA_Initial();
    }
    else    // stay at LOGO
    {
        MEDIA_StateChange(MEDIA_CONTROL_NONE);
    }
    MediaInfo[_MapSourceID2Index(bSource)].bState = MEDIA_NONE;
    /*
    // SERVO open
    if ( bSource >= SOURCE_SERVO )
    {
    // servo may have these state, no matter dsic or media navigator, the action are the same.
    // none ->open, do nothing
    // parsing  -> open-> abort parsing
    // ready    -> open-> delete info
    // invalid  -> open-> delete info
    // playing  -> open-> stop-> delete info

    if ( __bMediaSERVOState == MEDIA_SERVO_PLAYING )   // servo is playing
    {
    MEDIA_StateChange(MEDIA_CONTROL_STOP )
    }
    else if ( __bMediaSERVOState == MEDIA_SERVO_PARSING )   // servo is parsing
    {
    MEDIA_Abort(bSource);
    }
    MEDIA_StateChange( MEDIA_CONTROL_KILL );
    }
    */
}
/*
   [MediaManager]
__bSourceID: current select ID
Set __bNavigateMode when enter MM UI
[InfoFilter]
DeletingInfo(SOURCE_ALL)
#define SOURCE_SERVO        31
#define SOURCE_ATAPI        32
*/

//  *********************************************************************
//  Function    :   _ResetSuspendMedia
//  Description :   This function will reset all suspend media state as none.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :
//  *********************************************************************
void _ResetSuspendMedia()
{
    for (__bTemp = 0; __bTemp < (MAX_REMOVABLE_MEDIAS+2);__bTemp++)
    {
        if ( MediaInfo[__bTemp].bState == MEDIA_SUSPEND )
        {
            MediaInfo[__bTemp].bState = MEDIA_NONE;
        }
    }
}
//  *********************************************************************
//  Function    :   _QuerySuspendMedia
//  Description :   This function will query if there's any suspend media
//  Arguments   :   none.
//  Return      :   TRUE, has suspend media.
//  Side Effect :
//  *********************************************************************
BYTE    _QuerySuspendMedia()
{
    for (__bTemp = 0; __bTemp < (MAX_REMOVABLE_MEDIAS+2);__bTemp++)
    {
        if ( MediaInfo[__bTemp].bState == MEDIA_SUSPEND )
        {
            return TRUE;
        }
    }
    return FALSE;
}
BYTE    _MEDIA_Initial()
{
    __bNavigateMode = MEDIA_NAVIGATE_MODE;
#ifdef  DEBUG_MEDIA_STATUS
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nDraw media manager UI");
#endif  //#ifdef  DEBUG_MEDIA_STATUS

    return TRUE;
}
// Micky0.62-909, add MediaSelection_Exit()
void    MEDIA_MediaSelection_Exit(BYTE bMode)
{
    __bMediaSelectUI = FALSE;
    if (__wDiscType & BOOK_M1)
    {
        DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
        HAL_Display ( HAL_DISPLAY_SPST1, TRUE );
    }
    // Called when it was destroyed by same layer UI
    //if ( bMode == OSD_UI_EXIT_ALL ) 
    {
        OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_EXIT, OSD_UI_CLEAR_FORCE);
    }
}
// Micky0.85
BYTE    _MapSourceID2Index(BYTE bSourceID)
{
    if (bSourceID < SOURCE_REMOVABLE_FIRST )
    {
        return ((bSourceID>>4)-1);
    }
    else
    {
        // 0x30->2, 0x31->3
        return ( ((bSourceID>>4)-2-1)*PER_USB_NO + (bSourceID&0x0F) + 2);
    }
}
BYTE    _MapIndex2SourceID(BYTE bIndex)
{
    // 2~9      =>0x30~0x38
    // 10~17   => 0x40~0x48
    if (bIndex < 2)
    {
        return ((bIndex+1)<<4);
    }
    else
    {
        return ( 0x30 + ((bIndex-2)/PER_USB_NO) *0x10+ (bIndex-((bIndex-2)/PER_USB_NO)*PER_USB_NO -2) );
    }
}

BOOL _MEDIA_SwitchPreAct ( BYTE bPreMedia)
{

    switch (bPreMedia)
    {
    case MEDIA_SELECT_DVD:
#ifdef SUPPORT_STB            
        if(_bNextMedia != MEDIA_SELECT_STB)
#endif            
        {
            if (__btDriveOpen)
            {
                __btDriveOpen = FALSE;
                SrcFilter_CloseTray();
            }
            SrcFilter_PowerMode (SERVO_POWER_STOP) ;
        }
        break;

        // wyc2.35-909s, add define.
#ifdef SUPPORT_USB_SOURCE
    case MEDIA_SELECT_USB:
#ifdef SUPPORT_BUILDIN_CARD_READER
    case MEDIA_SELECT_CARD_READER:
#endif //
        // wyc2.53-909P,
        // wyc2.37-909s, add code to exit source to make sure USBSRC don't call H/W functions when suspend it.
        //SRCFilter_USBSRCExit();
        //OS_SuspendThread(ThreadUSBSRC.handle);
        // wyc2.38-909s, need to call SrcFilter_Stop to avoid USB keep in reading data mode.
        SrcFilter_Stop(__dwSFStreamID);
        USBSRC_ThreadSuspend();
        SRCFilter_ExitSource();
        HAL_PowerControl(HAL_POWER_USB, HAL_POWER_SAVE);
        // power off card-reader in 909P
        // to be added ...
        SrcFilter_ClearState();
        MediaInfo[_MapSourceID2Index(SOURCE_USB0_0)].bState = MEDIA_NONE;
        __bNavigateMode = NAVIGATE_NONE;
        __bMediaUSBAction = MEDIA_ACTION_USB_NONE;
        __bMediaUSBState = MEDIA_USB_NONE;                       
        break;
#endif

#ifdef SUPPORT_STB
    case MEDIA_SELECT_STB:  
        OSD_RemoveAllUI();            
        __bLOGO = LOGO_NONE;
        UTL_ShowLogo();                
        MEDIA_ResetNIM();
        break;
#endif //

#ifdef SUPPORT_AV_IN
    case MEDIA_SELECT_AV_IN:
        CC_DoKeyAction (KEY_AV_IN);        
        break;        
#endif //
    }    
    return TRUE;
}

BOOL _MEDIA_SwitchPostAct ( BYTE bPostMedia)
{
    switch(bPostMedia)
    {
#ifdef SUPPORT_AV_IN
    case MEDIA_SELECT_AV_IN:
        {
            CC_DoKeyAction (KEY_AV_IN);        
        }
        break;
#endif //
#ifndef NO_DISC_MODE
    case MEDIA_SELECT_DVD: // choose disc navigator
        __bMediaServoAction = MEDIA_SERVO_ACTION_CLOSE;
        __bMediaServoState = MEDIA_SERVO_CLOSED;
        __bChooseMedia = MEDIA_SELECT_DVD;
        SrcFilter_ChangeSourceID(0, SOURCE_SERVO);        
#ifdef SUPPORT_STB
        MEDIA_SwitchMode(MEDIA_SELECT_DVD);
        SERVO_Enable();
#endif //
        break;
#endif //NO_DISC_MODE
#ifdef SUPPORT_STB
    case MEDIA_SELECT_STB:
        {
            // LLY2.18, ignore the use key if don't support DVB feature
            if(!(__dwSupportFeature & SUPPORT_FEATURE_DVB))
            {
                return FALSE;
            }
            HAL_Reset (HAL_RESET_AVSPST);
            OSD_RemoveAllUI();
            UTL_ShowLogo();
#ifndef NO_DISC_MODE
            SERVO_Disable(); 
#endif //NO_DISC_MODE
            MEDIA_SwitchMode(MEDIA_SELECT_STB);
        }
        break;
#endif //
        // wyc2.35-909s, add define.
#ifdef SUPPORT_USB_SOURCE
    case MEDIA_SELECT_USB :
#ifdef SUPPORT_BUILDIN_CARD_READER
    case MEDIA_SELECT_CARD_READER:    
#endif //
        {
            // wyc2.77-909p, enable usb clock.
#if (USB_SYSTEM != USB_NONE)
            HAL_PowerControl(HAL_POWER_USB, HAL_POWER_NORMAL);                            
#endif //
            // wyc1.11-909, need to move outside the define SUPPORT_POWER_SAVING to avoid USB mode abnormal when undef SUPPORT_POWER_SAVING.
            // wyc1.21-909, need to add parameter here when Brian provide code.
            if (_bNextMedia == MEDIA_SELECT_USB)
            {
                // wyc2.71-portusb,
#if (USB_SYSTEM == USB1_1_CT909R)
#ifdef SWITCH_USB_PORT
                _dwUSBSRCActivePort = SOURCE_USB1_FIRST;
                printf("\nSet LUN to %hx",__bMediaFirstLUN);
                USB_HCInit(SOURCE_USB1_FIRST, __bMediaFirstLUN, DS_USB_BUFF_ST);
#else
                _dwUSBSRCActivePort = SOURCE_USB0_FIRST;
                USB_HCInit(SOURCE_USB0_FIRST, 0xFF, DS_USB_BUFF_ST);
#endif //
#endif //

            }
#ifdef SUPPORT_BUILDIN_CARD_READER
            else if (_bNextMedia == MEDIA_SELECT_CARD_READER)
            {
#ifdef CTK_CARDREADER
                // for 909P
                if (!CARD_InitSystem())
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"Initializing card-reader system fails ...\n");
#endif
                // wyc2.71-portusb,
#if (USB_SYSTEM == USB1_1_CT909R)
#ifdef SWITCH_USB_PORT
                _dwUSBSRCActivePort = SOURCE_USB0_FIRST;
                USB_HCInit(SOURCE_USB0_FIRST, 0xFF, DS_USB_BUFF_ST);
#else
                _dwUSBSRCActivePort = SOURCE_USB1_FIRST;
                USB_HCInit(SOURCE_USB1_FIRST, 0xFF, DS_USB_BUFF_ST);
#endif //
#endif
            }
#endif //
#ifdef SUPPORT_STB                
            MEDIA_SwitchMode(MEDIA_SELECT_USB);
#endif

            // 0 for USB, 1 for card-reader
#ifdef CTK_CARDREADER
            if (bPostMedia == MEDIA_SELECT_USB)
                USBSRC_ConfigSource(0);
            else
                USBSRC_ConfigSource(1);
#endif
            // ??
            __dwTimeBegin = 0;
            //OS_ResumeThread(ThreadUSBSRC.handle);
            USBSRC_ThreadResume();
            // wyc2.71-portusb,
#if (USB_SYSTEM == USB1_1_CT909R)
            USB_ClearStatus();
#endif //

            __bMediaServoAction = MEDIA_SERVO_ACTION_NONE;
            __bMediaServoState = MEDIA_SERVO_NONE;

            _ResetSuspendMedia();
        }
        break;
#endif
    }
    return TRUE;
}

#ifdef SUPPORT_STB

//CoCo2.38
void MEDIA_STB(void)
{
    if ( __bChooseMedia != MEDIA_SELECT_STB )
    {
        _MEDIA_SwitchPreAct ( __bChooseMedia );        
    }
    HAL_Reset (HAL_RESET_AVSPST);
    OSD_RemoveAllUI();
    UTL_ShowLogo();    
    __bChooseMedia = MEDIA_SELECT_STB;
}

VOID MEDIA_ResetNIM(VOID)
{
    HAL_WriteGPIO(NIM_RESET_GRP, NIM_RESET_PIN, 0);
    OS_DelayTime(COUNT_50_MSEC);    
    HAL_WriteGPIO(NIM_RESET_GRP, NIM_RESET_PIN, 1);
    OS_DelayTime(COUNT_50_MSEC); 
}

VOID MEDIA_SwitchMode(BYTE bMode)
{  

    BYTE bStartupType;

    //Aron1.07-DVB, prevent po noise when NIM power is effected by DVD_DVB switch pin
    HAL_IOMute(TRUE);       
    OS_DelayTime(COUNT_50_MSEC);     

    //DVB
    if(bMode == MEDIA_SELECT_STB)
    {
        // LLY2.60, only do DVD/DVB pin switching while solution need this pin to switch mode
        // ex. it's un-necessary and no pin to do it @ DMP952A solution.
        // Notice: don't use DECODER_SYSTEM for checking
        //         since CT952A/ CT909G may have many differnt decoder system name
        //#if (DECODER_SYSTEM != DMP952A_EVAL)
#ifndef  NO_DVD_DVB_SWITCHING
        HAL_WriteGPIO(DVD_DVB_SWITCH_GRP , PIN_DVD_DVB_SWITCH, 0);
#endif
        OS_DelayTime(COUNT_50_MSEC);   
        __bChooseMedia = __bPreMediaMode = MEDIA_SELECT_STB;        
        SrcFilter_ChangeSourceID(0, SOURCE_DVB);
    }

    //DVD
    if ( bMode == MEDIA_SELECT_DVD )
    {
        PARSER_DVBClosePath(0);
        // LLY2.60, only do DVD/DVB pin switching while solution need this pin to switch mode
        // ex. it's un-necessary and no pin to do it @ DMP952A solution.
        // Notice: don't use DECODER_SYSTEM for checking
        //         since CT952A/ CT909G may have many differnt decoder system name
        //#if (DECODER_SYSTEM != DMP952A_EVAL)
#ifndef  NO_DVD_DVB_SWITCHING
        HAL_WriteGPIO(DVD_DVB_SWITCH_GRP , PIN_DVD_DVB_SWITCH, 1);
#endif
        OS_DelayTime(COUNT_50_MSEC); 
        __bChooseMedia = __bPreMediaMode = MEDIA_SELECT_DVD;                
        SrcFilter_ChangeSourceID(0, SOURCE_SERVO);
    }

    //USB
    if(bMode == MEDIA_SELECT_USB)
    {
        // LLY2.60, only do DVD/DVB pin switching while solution need this pin to switch mode
        // ex. it's un-necessary and no pin to do it @ DMP952A solution.
        // Notice: don't use DECODER_SYSTEM for checking
        //         since CT952A/ CT909G may have many differnt decoder system name
        //#if (DECODER_SYSTEM != DMP952A_EVAL)
#ifndef  NO_DVD_DVB_SWITCHING
        HAL_WriteGPIO(DVD_DVB_SWITCH_GRP , PIN_DVD_DVB_SWITCH, 1);
#endif
        OS_DelayTime(COUNT_50_MSEC);         
        __bChooseMedia =  __bPreMediaMode = MEDIA_SELECT_USB;
    }

    SYSMENU_StartupTypeInfo(READ_COMMAND, &bStartupType);

    if (bStartupType == MENU_SYS_PREFERENCE_DEFAULT_TYPE_LAST_OPERATING_TYPE)
    {
        HAL_WriteStorage(SETUP_ADDR_LAST_MODE, (BYTE *)&__bPreMediaMode, 1);
    }


}

void MEDIA_DecidetMedia(void)
{


    BYTE bStartupType, bLastStartupType;

    __bChooseMedia = MEDIA_SELECT_DVD;

    SYSMENU_StartupTypeInfo(READ_COMMAND, &bStartupType);


    switch(bStartupType)
    {
    case MENU_SYS_PREFERENCE_DEFAULT_TYPE_DVD:
        __bChooseMedia = MEDIA_SELECT_DVD;
        break;

    case MENU_SYS_PREFERENCE_DEFAULT_TYPE_DVB:
        __bChooseMedia = MEDIA_SELECT_STB;
        break;

    case MENU_SYS_PREFERENCE_DEFAULT_TYPE_USB:
        __bChooseMedia = MEDIA_SELECT_USB;
        break;

    case MENU_SYS_PREFERENCE_DEFAULT_TYPE_LAST_OPERATING_TYPE:
        SYSMENU_LastStateInfo(READ_COMMAND, &bLastStartupType);
        if(bLastStartupType==MEDIA_SELECT_DVD) __bChooseMedia = MEDIA_SELECT_DVD;
        if(bLastStartupType==MEDIA_SELECT_STB) __bChooseMedia = MEDIA_SELECT_STB;
        if(bLastStartupType==MEDIA_SELECT_USB) __bChooseMedia = MEDIA_SELECT_USB;
        break;

    default: //User select
        __bMediaSelected = FALSE;
        OSD_NormalMode();
        MEDIA_ShowSelDlg();
        do
        {
            //Aron2.37, add the checking to prevent system reboot for STB
            UTL_EventTrigger();
            DBG_Polling();
            // elmer2.38
            //Aron2.38, get ir key when IR mode is polling
            if(INPUT_GetIRCheckMode()==IR_CHECK_MODE_POLLING)
            {
                DSR_IR();
            }                

            if (__bISRKey !=KEY_NO_KEY)
            {
                __bKey = __bISRKey;
                __bISRKey = KEY_NO_KEY;
            }
            if(__bKey == KEY_POWER)
            {
                __btPowerDown=!__btPowerDown;
                if(__btPowerDown)
                {
                    CHIPS_OpenAudio(FALSE);
                    HAL_IOMute(TRUE);
                    OS_DelayTime ( COUNT_100_MSEC);
                    OSD_RemoveAllUI();
                    OSD_OUTPUT_MACRO ( MSG_OPEN, FALSE, 0 );
                    OSD_OUTPUT_MACRO (MSG_POWER, __btPowerDown, 1);        
                    HAL_PowerDown(ENTER_POWERDOWN | IR_POWERDOWN_MODE);                        
                }
                else
                {
                    INITIAL_System(INITIAL_SYSTEM | IR_POWERDOWN_MODE);
                    OSD_OUTPUT_MACRO(MSG_POWER, __btPowerDown, 1);
                    INITIAL_PowerONStatus(INITIAL_POWER_SOFTWARE);
                    MEDIA_ShowSelDlg();
                }
                __bKey = KEY_NO_KEY;
            }
            if(!__btPowerDown)
            {
                MEDIA_MediaSelection_ProcessKey();
            }
            __bKey = KEY_NO_KEY;
        }while(__bMediaSelected != TRUE);            

    }

    if( __bChooseMedia == MEDIA_SELECT_DVD)
    {
        if (SrcFilter_CheckTrayStatus () == SRC_TRAY_CLOSED)
        {        
            OSD_Output ( MSG_WAITING, 0, 0xFF );   
        }    
    }

    switch(__bChooseMedia)
    {
#ifndef NO_DISC_MODE
    case MEDIA_SELECT_DVD:
#ifdef SUPPORT_STB
        SERVO_Enable();
#endif //
        MEDIA_SwitchMode(MEDIA_SELECT_DVD);
        __bPreMediaMode = MEDIA_SELECT_DVD;
        break;
#endif //NO_DISC_MODE
    case MEDIA_SELECT_STB:
        OSD_NormalMode(); //CoCo2.16, reset the OSD mode to clear all messages.
#ifndef NO_DISC_MODE
        SERVO_Disable();
#endif //NO_DISC_MODE
        MEDIA_SwitchMode(MEDIA_SELECT_STB);
        __bPreMediaMode = MEDIA_SELECT_STB;
        break;
    case MEDIA_SELECT_USB:
        SrcFilter_PowerMode (SERVO_POWER_STOP) ;
        HAL_PowerControl(HAL_POWER_USB, HAL_POWER_NORMAL);                            
        {
            // wyc2.71-portusb,
#if (USB_SYSTEM == USB1_1_CT909R)
#ifdef SWITCH_USB_PORT
            _dwUSBSRCActivePort = SOURCE_USB1_FIRST;
            USB_HCInit(SOURCE_USB1_FIRST, 0xFF, DS_USB_BUFF_ST);
#else
            _dwUSBSRCActivePort = SOURCE_USB0_FIRST;
            USB_HCInit(SOURCE_USB0_FIRST, 0xFF, DS_USB_BUFF_ST);
#endif 
#endif //
        }

        OS_ResumeThread(ThreadUSBSRC.handle);
        // wyc2.71-portusb,
#if (USB_SYSTEM == USB1_1_CT909R)
        USB_ClearStatus();
#endif //

        __bMediaServoAction = MEDIA_SERVO_ACTION_NONE;
        __bMediaServoState = MEDIA_SERVO_NONE;

        _ResetSuspendMedia();
        __bPreMediaMode = MEDIA_SELECT_USB;
    }

}
#endif

#ifdef SUPPORT_POWERON_MENU
//  *********************************************************************
//  Function    :   MEDIA_USB
//  Description :   This function will call after poweron_menu icon selection
//  Arguments   :   none.
//  Return      :   
//  Side Effect :
//  *********************************************************************
void MEDIA_USB(void)
{
    // elmer2.78, only initialize card once
    if (_bMediaInitUSB == FALSE)
    { 
        _bMediaInitUSB = TRUE;

#ifdef CTK_CARDREADER
        // elmer2.75-909P, move from card.c to here
        {
            extern BOOL CARD_SetupSystem(DWORD);

            CARD_SetupSystem(DECODER_SYSTEM);

            // initialize CTK card-reader
            if (!CARD_InitSystem())
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Initializing card-reader system is failing ...\n");
        }
#endif
    }
#ifdef SUPPORT_CHOOSE_MEDIA
    //if (!_bFirstCheckPort)
    _bDisableCheckPort = 1;
#endif

#ifndef SUPPORT_CHOOSE_MEDIA
#ifdef SUPPORT_USB_SWITCH
    if (__bMediaRegCnt == 2)
#else
    if (__bMediaRegCnt == 1)
#endif
    {
#ifdef CT909R_IC_SYSTEM
        // USB port first
        _dwUSBSRCActivePort = SOURCE_USB1_FIRST;
#elif defined (CT909P_IC_SYSTEM)
        USBSRC_ConfigSource(_bMediaSource);
#endif
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, "1. P Source: %lx, R Source: %lx\n", _bMediaSource, _dwUSBSRCActivePort);
    }
    else
    {
#ifdef CT909R_IC_SYSTEM
        // Card-Reader port next
        _dwUSBSRCActivePort = (_dwUSBSRCActivePort == SOURCE_USB0_FIRST) ? SOURCE_USB1_FIRST : SOURCE_USB0_FIRST;
#elif defined (CT909P_IC_SYSTEM)
#ifdef SUPPORT_USB_SWITCH
        if (__bMediaRegCnt == 1)
#else
        if (__bMediaRegCnt == 0)
#endif
            _bMediaSource = (_bMediaSource == 1) ? 0 : 1;
        USBSRC_ConfigSource(_bMediaSource);
#endif //
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, "2. P Source: %lx, R Source: %lx\n", _bMediaSource, _dwUSBSRCActivePort);
    }
    // wyc2.71-portusb,
#if (USB_SYSTEM == USB1_1_CT909R)
    USB_HCInit(_dwUSBSRCActivePort, 0xFF, DS_USB_BUFF_ST);
#endif // USB_SYSTEM == USB1_1_CT909R
#else // SUPPORT_CHOOSE_MEDIA
// elmer2.78, not implement SUPPORT_USB_SWITCH in this style
    if (__bMediaRegCnt == 1)
    {
        if (_dwDefaultDevice & 0x0000FFFF)
        {
            _dwUSBSRCActivePort = SOURCE_USB0_FIRST;
            // wyc2.71-portusb,
#if (USB_SYSTEM == USB1_1_CT909R)
            USB_HCInit(_dwUSBSRCActivePort, (BYTE)_dwDefaultDevice - 1, DS_USB_BUFF_ST);
#endif //
#ifdef CTK_CARDREADER
            // elmer2.78, set CARD_READER to first detect
            _bMediaSource = 1;
            USBSRC_ConfigSource(_bMediaSource);
#endif
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, "3. P Source: %lx, R Source: %lx\n", _bMediaSource, _dwUSBSRCActivePort);
        }
        else
        {
            // wyc2.71-portusb,
#if (USB_SYSTEM == USB1_1_CT909R)
            _dwUSBSRCActivePort = SOURCE_USB1_FIRST;
            USB_HCInit(_dwUSBSRCActivePort, 0xFF, DS_USB_BUFF_ST);
#endif //
#if (USB_SYSTEM == USB2_0_JUNGO)
            // elmer2.78, set CARD_READER to first detect
            _bMediaSource = 0;
            USBSRC_ConfigSource(_bMediaSource);
#endif
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, "4. P Source: %lx, R Source: %lx\n", _bMediaSource, _dwUSBSRCActivePort);
        }
    }
    else
    {
        // wyc2.71-portusb,
#if (USB_SYSTEM == USB1_1_CT909R)
        _dwUSBSRCActivePort = (_dwUSBSRCActivePort == SOURCE_USB0_FIRST) ? SOURCE_USB1_FIRST : SOURCE_USB0_FIRST;
        USB_HCInit(_dwUSBSRCActivePort, 0xFF, DS_USB_BUFF_ST);
#endif //
        _bMediaSource = (_bMediaSource == 1) ? 0 : 1;
        USBSRC_ConfigSource(_bMediaSource);

        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, "P Source: %lx, R Source: %lx\n", _bMediaSource, _dwUSBSRCActivePort);
    }
#endif // !SUPPORT_CHOOSE_MEDIA

// elmer2.78, remove for testing
/*<<<
#ifdef CT909P_IC_SYSTEM
#ifdef SUPPORT_USB_SWITCH
    if (__bMediaRegCnt == 1)
#else
    if (__bMediaRegCnt == 0)
#endif
        _bMediaSource = (_bMediaSource == 1) ? 0 : 1;
    USBSRC_ConfigSource(_bMediaSource);
#endif // CT909P_IC_SYSTEM
>>>*/

    MEDIA_InitialState();
    USBSRC_ClearUSBSRCState();
    OS_REORDER_BARRIER();
    __bChooseMedia = MEDIA_SELECT_USB;
    USBSRC_ThreadResume();
}

//  *********************************************************************
//  Function    :   MEDIA_ExitUSBSRC
//  Description :   This function will exit USBSRC thread and then suspend it
//  Arguments   :   none.
//  Return      :   
//  Side Effect :
//  *********************************************************************
void MEDIA_ExitUSBSRC(void)
{
    // set to 1 to have chance to scan the other USB port
#ifdef SUPPORT_USB_SWITCH
    __bMediaRegCnt = 2;
#else
    __bMediaRegCnt = 1;
#endif
    // check port status
#ifdef SUPPORT_CHOOSE_MEDIA
    _bDisableCheckPort = 0;
#endif

    SrcFilter_Stop(__dwSFStreamID);
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    SrcFilter_Stop(BG_ITEM_STREAM_ID);
#endif

    SRCFilter_USBSRCExit();
    // power off USB port
#ifdef CT909P_IC_SYSTEM
    // power off card reader in 909P
    // to be added ...
#else
    // don't power off here, or SUPPORT_CHOOSE_MEDIA will fail
    //HAL_PowerControl(HAL_POWER_USB, HAL_POWER_SAVE);
#endif

    OS_REORDER_BARRIER();

    __bMediaUSBAction = MEDIA_ACTION_USB_NONE;
    __bMediaUSBState = MEDIA_USB_NONE;
    // wyc2.78-909P, don't suspend the usbsrc thread and just disable this variable.
    __bChooseMedia = MEDIA_SELECT_DVD;
    //USBSRC_ThreadSuspend();
}

#ifdef SUPPORT_CHOOSE_MEDIA
DWORD MEDIA_QueryDevice(void)
{
    return _dwPortStatus;
}

DWORD MEDIA_QueryDefaultDevice(void)
{
    if (_dwPortStatus & _dwDefaultDevice)
        return _dwDefaultDevice;
    else
        return (_dwDefaultDevice = MEDIA_DEVICE_USB);
}

VOID MEIDA_SetDefaultDevice(DWORD dwSetValue)
{
    if (dwSetValue & _dwPortStatus)
        _dwDefaultDevice = dwSetValue;
    else
        _dwDefaultDevice = MEDIA_DEVICE_USB;
}
#endif

void MEDIA_ClearUSBSwitchState(void)
{
#ifdef SUPPORT_USB_SWITCH
    HAL_WriteGPIO(GPIO_A, 23, 0);
    __bMediaRegCnt = 2;
    _bMediaSource = 1;
#else
    __bMediaRegCnt = 1;
    _bMediaSource = 0;
#endif
}
#endif

#ifdef SUPPORT_CHOOSE_MEDIA
#ifdef CTK_CARDREADER
DWORD _MEDIA_GetCardReaderStatus(DWORD dwInsert)
{
    DWORD temp = 0;

    switch(dwInsert)
    {
        case CARD_CONTROLLER_SD:
            return (temp | MEDIA_DEVICE_SD);
        case CARD_CONTROLLER_MS:
            return (temp | MEDIA_DEVICE_MS);
        case CARD_CONTROLLER_XD:
            return (temp | MEDIA_DEVICE_XD);
        default:
            return 0;
    }
}
#endif
#endif
