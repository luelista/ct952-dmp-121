//**************************************************************************
//         Copyright (c) 2004, Cheertek Inc . All rights reserved.
//         D300, all right reserved.

//	Product	: WinDVD Firmware

//	Date	: 2004.5.7
//	Author	: Cheertek (D300 CoCo Chiang)
//	Purpose	: This is the OSD kernel.
//	Sources	: osd.h/osd.c
//***************************************************************************

#ifndef __OSD_H__
#define __OSD_H__

#include "OSDFont.h"

//Define each UI's name. Note: Each UI should user the name to notify OSD.
#define OSD_UI_NONE                 0   //Normal Display
#define OSD_UI_DISPLAY              1   //Display mode
#define OSD_UI_MEDIA_MANAGER        2   //Media Manager
#define OSD_UI_SETUP                3   //SETUP
#define OSD_UI_THUMBNAIL            4   //Thumbnail
#define OSD_UI_SEARCH               5   //Search UI
#define OSD_UI_DVD_PROGRAM          6   //VCD/DVD Program
#define OSD_UI_DIGEST               7   //Digest
#define OSD_UI_MEDIA_SELECT_DLG     8   //Media Select Dialog box
#define OSD_UI_PSCAN_PROMPT_DLG     9   //The PSCAN prompt dialog box
#define OSD_UI_BOOKMARK             10  //Bookmark
#define OSD_UI_AUTO_UPGRADE         11  //Auto-upgrade
#define OSD_UI_SCREEN_SAVER         12  //Screen Saver
#define OSD_UI_COMMON_DLG           13  //Common Dialog Box
#define OSD_UI_NAVIGATOR            14  //Independent Navigator UI
#define OSD_UI_MAIN_MENU            15  //Main menu for DVD and DVB.
#define OSD_UI_PASSWORD             16  //Password Dialog
#define OSD_UI_POWERON_MENU         17  //Power on Menu
#define OSD_UI_COPY_DELETE_DLG      18  //Copy/Delete JPEG dialog.
#define OSD_UI_RADIO_TUNER          19  // Radio Tuner

//***************************************************************************
//Parameters for OSD_ChangeUI
//***************************************************************************
#define OSD_ENTER_UI    0
#define OSD_EXIT_UI     1
#define OSD_CHANGE_UI   2

//***************************************************************************
//Parameters for XXX_Exit
//***************************************************************************
#define OSD_UI_EXIT_ALL             0
#define OSD_UI_EXIT_CONTROL_ONLY    1

//***************************************************************************
//Parameters for clearing the UI in each OSDXXX module.
//***************************************************************************
#define OSD_UI_CLEAR_NORMAL         0
#define OSD_UI_CLEAR_FORCED         1


//***************************************************************************
//Define the UI layer
//***************************************************************************
#define OSD_UI_LAYER_0      0
#define OSD_UI_LAYER_1      1
#define OSD_UI_LAYER_2      2
#define OSD_UI_LAYER_3      3
#define OSD_UI_LAYER_4      4
#define OSD_UI_LAYER_5      5

//***************************************************************************
//Parameters for recovering UI
//***************************************************************************
#define OSD_RECOVER_UPPER_REGION        0
#define OSD_RECOVER_BOTTOM_REGION       1
#define OSD_RECOVER_ALL                 2

//***************************************************************************
//Parameters for clearing UI
//***************************************************************************
#define OSD_ND_UPPER_RIGHT_REGION       0
#define OSD_ND_BOTTOM_REGION            1
#define OSD_ND_UNKNOWN_REGION           2
#define OSD_ND_RIGHT_BOTTOM_REGION      3

//***************************************************************************
//Message for OSD_Output
//***************************************************************************
#define MSG_NULL                0                   //no message
#define MSG_ALL                 0xFF

//#define MSG_OSD_BASE            0
#define MSG_OPEN                (MSG_OSD_BASE+1)    
#define MSG_WAITING             (MSG_OSD_BASE+2)
#define MSG_INVALID             (MSG_OSD_BASE+3)
#define MSG_ABSTIME             (MSG_OSD_BASE+4)
#define MSG_RELTIME             (MSG_OSD_BASE+5)
#define MSG_REMAIN_ABSTIME      (MSG_OSD_BASE+6)
#define MSG_REMAIN_RELTIME      (MSG_OSD_BASE+7)
#define MSG_NEXT                (MSG_OSD_BASE+8)
#define MSG_PREV                (MSG_OSD_BASE+9)
#define MSG_ENDLIST             (MSG_OSD_BASE+10)
#define MSG_RETURN              (MSG_OSD_BASE+11)
#define MSG_PLAYING             (MSG_OSD_BASE+12)
#define MSG_STOP                (MSG_OSD_BASE+13)
#define MSG_SCANFORWARD         (MSG_OSD_BASE+14)
#define MSG_SCANBACKWARD        (MSG_OSD_BASE+15)
#define MSG_SLOW                (MSG_OSD_BASE+16)
#define MSG_PAUSE               (MSG_OSD_BASE+17)
#define MSG_FAST                (MSG_OSD_BASE+18)
#define MSG_STEP                (MSG_OSD_BASE+19)
#define MSG_FREEZE              (MSG_OSD_BASE+20)
//#define MSG_DISCPOSITION      //Remove it?    
//#define MSG_CHANGEDISCPOS     //Remove it?
//#define MSG_CHECKLOADER       //No one send it. But Panel module will judge it. Notify Panel's owner to remove it.
#define MSG_KEY10               (MSG_OSD_BASE+21)
#define MSG_TRACK               (MSG_OSD_BASE+22)           
#define MSG_NUMBER              (MSG_OSD_BASE+23)          
#define MSG_REPEAT              (MSG_OSD_BASE+24)          
#define MSG_REPEATAB            (MSG_OSD_BASE+25)        
#define MSG_RANDOM              (MSG_OSD_BASE+26)          
#define MSG_DIGEST              (MSG_OSD_BASE+27)
#define MSG_PBC                 (MSG_OSD_BASE+28)
#define MSG_TVMODE              (MSG_OSD_BASE+29)
#define MSG_SETGOTOTIME         (MSG_OSD_BASE+30)
#define MSG_GOTOTIME            (MSG_OSD_BASE+31)
#define MSG_RESUME              (MSG_OSD_BASE+32)
#define MSG_DISCTYPE            (MSG_OSD_BASE+33)
#define MSG_MENU                (MSG_OSD_BASE+34)
#define MSG_VOCAL               (MSG_OSD_BASE+35)
#define MSG_AUTOVOCAL           (MSG_OSD_BASE+36)
#define MSG_VOICECANCEL         (MSG_OSD_BASE+37)
#define MSG_KARAOKE             (MSG_OSD_BASE+38)
#define MSG_ZOOM                (MSG_OSD_BASE+39)
#define MSG_ZOOM_TEXT           (MSG_OSD_BASE+40)
#define MSG_VERSION             (MSG_OSD_BASE+41)
//#define MSG_ERR_PICTURE         //Remove it. No one process it. Only OSD module send this message.     
#define MSG_TITLE               (MSG_OSD_BASE+42)        
#define MSG_CHAPTER             (MSG_OSD_BASE+43)
#define MSG_SELECTIONLIST       (MSG_OSD_BASE+44)
#define MSG_INTRO               (MSG_OSD_BASE+45)
#define MSG_FULLSCREEN          (MSG_OSD_BASE+46)
//#define MSG_SETSEARCH             //Force Search UI to remove this message.
//#define MSG_SEARCH                //Force Search UI to remove this message.                  
#define MSG_TITLE_TIME          (MSG_OSD_BASE+47)
#define MSG_REMAIN_TITLE_TIME   (MSG_OSD_BASE+48)
#define MSG_CHAPTER_TIME        (MSG_OSD_BASE+49)
#define MSG_REMAIN_CHAPTER_TIME (MSG_OSD_BASE+50)
#define MSG_STRING              (MSG_OSD_BASE+51)
#define MSG_VIDEO_OUTPUT        (MSG_OSD_BASE+52)
#define MSG_ERRCODE             (MSG_OSD_BASE+53)
//#define MSG_THUMBNAIL_PAGE      //Remove it.          
#define MSG_ROTATE              (MSG_OSD_BASE+54)
#define MSG_FLIP                (MSG_OSD_BASE+55)
#define MSG_JPEG_EFFECT         (MSG_OSD_BASE+56)
#define MSG_PROGRAM_PLAY        (MSG_OSD_BASE+57)
#define MSG_SPST_CTL            (MSG_OSD_BASE+58)
#define MSG_AST_CTL             (MSG_OSD_BASE+59)
#define MSG_AUDIO_OUTPUT_CTL    (MSG_OSD_BASE+60)
//#define MSG_CLEARPROGRAM        //This is Program UI relative message. Need to move it to Program UI.
#define MSG_PROGRAM             (MSG_OSD_BASE+61)
//#define MSG_SETPROGRAM          //This is Program UI relative message. Need to move it to Program UI.
//#define MSG_ENTER               //Remove it. No one process it.                   
//#define MSG_BOOKMARK            //This is Bookmark UI relative message. Need to move it to Bookmark UI.
//#define MSG_AUTOUPGRADE         //This is Auto-upgrade UI relative message. Need to move it to Auto-upgrade UI.
#define MSG_AUDIO               (MSG_OSD_BASE+62)
#define MSG_VOLUME              (MSG_OSD_BASE+63)
#define MSG_MUTE                (MSG_OSD_BASE+64)
#define MSG_KEY                 (MSG_OSD_BASE+65)
//#define MSG_KEY_DUMMY1          //Remove it.
//#define MSG_KEY_DUMMY2          //Remove it.
#define MSG_ECHO                (MSG_OSD_BASE+66)
#define MSG_SURROUND            (MSG_OSD_BASE+67)
#define MSG_3D                  (MSG_OSD_BASE+68)
#define MSG_ANGLE_CTL           (MSG_OSD_BASE+69)
#define MSG_SOUND_EFFECT        (MSG_OSD_BASE+70)
#define MSG_STOPWAIT            (MSG_OSD_BASE+71)
#define MSG_PLAYLIST            (MSG_OSD_BASE+72)    
#define MSG_LOGO                (MSG_OSD_BASE+73)
//#define MSG_DIGESTNUMBER        //Only Panel module refer to it. But no one send this message. Remove it.
#define MSG_MOTION              (MSG_OSD_BASE+74)
#define MSG_POWER               (MSG_OSD_BASE+75)
//#define MSG_OSDRECOVER          //No one send this message and no one process it. Remove it.
//#define MSG_IGNORENEXT          //VCD20 module send this message. But no one process it. Remove it.
//#define MSG_DISCSTATUS          //No one send this message and no one process it. Remove it.              
//#define MSG_CDROM_DECODE        //No one process it. Remove it.
#define MSG_TOTALTRACKS         (MSG_OSD_BASE+76)
//#define MSG_OSDAPP              //Modify it to call OSD_ChangeUI. Remove it.
//#define MSG_PALTITLE            //No one process it. Remove it.                              
//#define MSG_LED1                //No one process it. Remove it.
//#define MSG_LED2                //No one process it. Remove it.
//#define MSG_LED3                //No one process it. Remove it.
//#define MSG_LED4                //No one process it. Remove it.
//#define MSG_UPDATE              //No one process it. Remove it.
//#define MSG_GUI                 //Modify it to call OSD_ChangeUI. Remove it.
#define MSG_FOLDER              (MSG_OSD_BASE+77)//Need to review this message. For CD-ROM title. (line-display)
#define MSG_PSCAN               (MSG_OSD_BASE+78)
#define MSG_STATUS              (MSG_OSD_BASE+79)//Need to review this message. For virtual panel.
#define MSG_PWRON_BAR           (MSG_OSD_BASE+80)
#define MSG_DISPLAY             (MSG_OSD_BASE+81)
//#define SETUP_MSG_BASE          //Need to review this message. For SETUP UI only.
//#define SETUP_MSG_END           //Need to review this message. For SETUP UI only.
//#define MSG_SETUP_MAIN_BASE     //Need to review this message. For SETUP UI only.
#define MSG_FILE_NAME           (MSG_OSD_BASE+82)
#define MSG_FILE_NUM            (MSG_OSD_BASE+83)
#define MSG_JPEG_RESOLUTION     (MSG_OSD_BASE+84)
#define MSG_JPEG_DATE           (MSG_OSD_BASE+85)
#define MSG_FILE_INFO           (MSG_OSD_BASE+86)
#define MSG_ABNORMAL_DISC       (MSG_OSD_BASE+87)
#define MSG_PROGRESS            (MSG_OSD_BASE+88) //CoCo1.10, support progress bar
#define MSG_NO_PLAYABLE_FILE    (MSG_OSD_BASE+89)
#define MSG_BROWSE_MODE         (MSG_OSD_BASE+90)

#define MAX_MSGNO_FOR_CHECK     (MSG_BROWSE_MODE+1)


// The following messages are for virtual panel. Need to review it for CT909. Especially for 6 display mode.
#define MSG_INITIAL             0
#define MSG_CLEAR               100
#define MSG_INITIAL_PALETTE     101
#define MSG_DEFAULT_STATE       102
#define MSG_CLEAR_ALL           103

//OSD language group define
#define CHINA_AREA_OSD      0
#define EUROPE_AREA_OSD     1
#define JAPAN_AREA_OSD      2
#define KOREAN_AREA_OSD     3

//Chinese OSD language mode define
#define SIMPLE_CHINESE              0
#define TRADITIONAL_CHINESE         1
#define ALL_CHINESE                 2

#define  SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE

//Choose one suitable OSD language group base on sell destination
// **** China Area OSD *****
#if SELL_DESTINATION == CHINA
#define OSD_LANGUAGE_GROUP      CHINA_AREA_OSD
#ifdef  SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#define CHINESE_OSD_MODE        SIMPLE_CHINESE
#else   // #ifdef SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#define CHINESE_OSD_MODE        ALL_CHINESE
#endif  // #ifdef SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#endif  // #if SELL_DESTINATION == CHINA

#if SELL_DESTINATION == TAIWAN
#define OSD_LANGUAGE_GROUP      CHINA_AREA_OSD
#ifdef  SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#define CHINESE_OSD_MODE        TRADITIONAL_CHINESE
#else   // #ifdef SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#define CHINESE_OSD_MODE        ALL_CHINESE
#endif  // #ifdef SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#endif  // #if SELL_DESTINATION == TAIWAN

#if SELL_DESTINATION == HONGKONG
#define OSD_LANGUAGE_GROUP      CHINA_AREA_OSD
#ifdef  SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#define CHINESE_OSD_MODE        TRADITIONAL_CHINESE
#else   // #ifdef SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#define CHINESE_OSD_MODE        ALL_CHINESE
#endif  // #ifdef SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#endif  // #if SELL_DESTINATION == HONGKONG

#if SELL_DESTINATION == FRANCE
#define OSD_LANGUAGE_GROUP      EUROPE_AREA_OSD
#endif  // #if SELL_DESTINATION == FRANCH

#if SELL_DESTINATION == GERMANY
#define OSD_LANGUAGE_GROUP      EUROPE_AREA_OSD
#endif  // #if SELL_DESTINATION == GERMANY

#if SELL_DESTINATION == ITALY
#define OSD_LANGUAGE_GROUP      EUROPE_AREA_OSD
#endif  // #if SELL_DESTINATION == ITALY

#if SELL_DESTINATION == SPAIN
#define OSD_LANGUAGE_GROUP      EUROPE_AREA_OSD
#endif  // #if SELL_DESTINATION == SPAIN

#if SELL_DESTINATION == PORTUGAL
#define OSD_LANGUAGE_GROUP      EUROPE_AREA_OSD
#endif  // #if SELL_DESTINATION == PORTUGAL

#if SELL_DESTINATION == EUROPE
#define OSD_LANGUAGE_GROUP      EUROPE_AREA_OSD
#endif  // #if SELL_DESTINATION == EUROPE

#if SELL_DESTINATION == BRITISH
#define OSD_LANGUAGE_GROUP      EUROPE_AREA_OSD
#endif  // #if SELL_DESTINATION == BRITISH

#if SELL_DESTINATION == JAPAN
#define OSD_LANGUAGE_GROUP      JAPAN_AREA_OSD
#endif  // #if SELL_DESTINATION == JAPAN

#if SELL_DESTINATION == KOREA
#define OSD_LANGUAGE_GROUP      KOREAN_AREA_OSD
#endif  // #if SELL_DESTINATION == KOREA

// Define some necessary info. base on different OSD language group
// OSD message array index for _bOSDLang reference, ex OSD_LoadFontTable()
// Notice: only support max 128 OSD language,
//          So, if "ID > 0x80" means it not support
// Max OSD language number
#if OSD_LANGUAGE_GROUP == CHINA_AREA_OSD
// OSD message array index
#define OSD_FONT_ENGLISH        0x0
#define OSD_FONT_CHINESE        0x1
#define OSD_FONT_FRENCH         0x80
#define OSD_FONT_GERMAN         0x81
#define OSD_FONT_ITALIAN        0x82
#define OSD_FONT_SPANISH        0x83
#define OSD_FONT_PORTUGUESE     0x84
#define OSD_FONT_JAPANESE       0x85
#ifdef  SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
// Max OSD language number
#define MAX_OSD_LANGUAGE        2
#else   // #ifdef SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#define MAX_OSD_LANGUAGE        3
#endif  // #ifdef SUPPORT_ONLY_ONE_CHINESE_OSD_LANGUAGE
#endif  // #if OSD_LANGUAGE_GROUP == CHINA_AREA_OSD

#if OSD_LANGUAGE_GROUP == EUROPE_AREA_OSD
// OSD message array index
#define OSD_FONT_ENGLISH        0x0
#define OSD_FONT_FRENCH         0x1
#define OSD_FONT_GERMAN         0x2
#define OSD_FONT_ITALIAN        0x3
#define OSD_FONT_SPANISH        0x4
#define OSD_FONT_PORTUGUESE     0x80
#define OSD_FONT_CHINESE        0x81
#define OSD_FONT_JAPANESE       0x82
// Max OSD language number
#define MAX_OSD_LANGUAGE        5
#endif  // #if OSD_LANGUAGE_GROUP == EUROPE_AREA_OSD

#if OSD_LANGUAGE_GROUP == JAPAN_AREA_OSD
// OSD message array index
#define OSD_FONT_ENGLISH        0x0
#define OSD_FONT_JAPANESE       0x1
#define OSD_FONT_FRENCH         0x80
#define OSD_FONT_GERMAN         0x81
#define OSD_FONT_ITALIAN        0x82
#define OSD_FONT_SPANISH        0x83
#define OSD_FONT_PORTUGUESE     0x84
#define OSD_FONT_CHINESE        0x85
// Max OSD language number
#define MAX_OSD_LANGUAGE        2
#endif  // #if OSD_LANGUAGE_GROUP == JAPAN_AREA_OSD

//define LOGO ID for MSG_LOGO usage. Need to review this part. Move it to SWOSD?
#define OSD_LOGO_DEFAULT    0x0
#define OSD_LOGO_ENCRYPT    0x1
#define OSD_LOGO_FROM_OPEN  0xFE
#define OSD_LOGO_CLEAR      0xFF

//CoCo, The following messages are for screen saver only. Need to move it to "screen saver" relative module.
//define color palette group for OSD_LoadPalette()
//Need to review this part. It's for screen saver only. Move it to SWOSD?
//#define OSD_PALETTE_FONT        0
//#define OSD_PALETTE_BMP1        1
//#define OSD_PALETTE_BMP2        2
//#define OSD_PALETTE_BMP3        3
//define screen saver mode for OSD_ShowBitmap()
//#define OSD_SCR_MODE1           OSD_APPMODE_SCR1
//#define OSD_SCR_MODE2           OSD_APPMODE_SCR2
//#define OSD_SCR_MODE3           OSD_APPMODE_SCR3

//define the MSG_VERSION's base index for debug, range is 65000 ~ 65534
#define OSD_VERSION_DEBUG_BASE  65000

//define stage for fill osd header usage: OSD_FillHeadtoDRAM(). Need to review it.
#define OSD_FILLHEADER_INITIAL      0
#define OSD_FILLHEADER_RUNTIME      1
#define OSD_FILLHEADER_ENTER_SETUP  2

////////////////////////////////////////////////////////////////////
//*********CoCo, Need to review this part for CT909
//  *****  Some parameter for OSD MSG usage definition Area *****
//parameter for repeat AB
#define REPEAT_A            REPEAT_NONE+ 1
#define REPEAT_AB           REPEAT_NONE+ 2

//For MSG_INVALID.
#define OSD_ERR_NONE          0
#define OSD_ERR_KEY           OSD_ERR_NONE+1
#define OSD_ERR_NUMBER        OSD_ERR_NONE+2
#define OSD_ERR_PARENTAL      OSD_ERR_NONE+3
#define OSD_ERR_DATA          OSD_ERR_NONE+4

//CoCo, the following messages are for Search UI only. Will remove it.
//#define OSD_CANCEL_TIME                 0
//#define OSD_INPUT_TITLE_CHAPTER         OSD_CANCEL_TIME+1
//#define OSD_INPUT_TIME					OSD_CANCEL_TIME+2
//#define OSD_COMPLETE_TIME               OSD_CANCEL_TIME+3

//Parameter for MSG_UPDATE
//#define OSD_UPDATE_AUDIO        1       //Remove this parameter because of MSG_UPDATE is removed also.

//Parameter for MSG_WAITING
#define OSD_SERVO_STABLE        1

//Parameter for MSG_MENU
#define OSD_MENU_ROOT       1
#define OSD_MENU_TITLE      2

//Parameter for MSG_NEXT & MSG_PREV
#define OSD_NEXT_PREV_TRACK         0
#define OSD_NEXT_PREV_CHAPTER       1
#define OSD_NEXT_PREV_PAGE          2
#define OSD_NEXT_PREV_ALBUM         3

//Parameter for OSD_Trigger
#define OSD_TRIGGER_TIME            0
#define OSD_TRIGGER_CONTROL	        1

//Parameter for all message
// Initial message while there is no parameter for reference
//#define INITIAL_MSG             0x0         //This message is for MSG_MEMORY (AYSYS). Remove it.
// Clear message, ex PANEL module reference
//#define CLEAR_MSG               0xFFFF      //This message is for MSG_MEMORY (AYSYS). Remove it.                       

//CoCo, need to review this part. Is it necessary for CT909.
//All channel volume control bar relative info.
//The max volume control bar NO
#define MAX_VOLCTL_BAR              16
// The volume gap of each volume control bar
#define VOLCTL_BAR_GAP              1

#define MAXNO_OF_MSG_STRING         18
extern BYTE    __bMsgString[MAXNO_OF_MSG_STRING+1]; // for MSG_STRING usage

//Alan2.21, define the buffer mode
#define OSDBUFFER_MODE_NORMAL            0
#define OSDBUFFER_MODE_MM_AVI            1
#define OSDBUFFER_MODE_MM_MOTION         2
#define OSDBUFFER_MODE_MM_AUDIO_ONLY     3
#define OSDBUFFER_MODE_MM_VIDEO_ONLY     4
#define OSDBUFFER_MODE_MM_JPEG_PREVIEW   5
#define OSDBUFFER_MODE_MM_JPEG_THUMBNAIL 6
#define OSDBUFFER_MODE_MM_UI             7
#define OSDBUFFER_MODE_AP                8
#define OSDBUFFER_MODE_DVB               9

//STATUS structure
typedef struct
{
    BYTE bPBCMode;          // PBC on/off
    BYTE bPlayingMode;   	// playing mode
    BYTE bStill;         	// current play item is motion or still
    BYTE bPowerDown;     	// power down flag
    WORD wTotalTracks;   	// total tracks of disc
    // LLY2.53, expand the time value from WORD to DWORD unit
    // Since, we will use [23:16]:hour, [15:8]:min, [7:0]: sec
    //WORD wCurrentTime;   	// current time Min:Sec
    DWORD dwCurrentTime;
    WORD wCurrentTrack;  	// current play track number
} STATUS;

//OSDMESSAGE structure
typedef struct
{
    BYTE bMessage;          // The message
    WORD wParam;     	    // The parameter
    BYTE bSec;	            // The time
} OSDMESSAGE;


//OSDBuffer structure
typedef struct
{
    BYTE  bColorMode;
    BYTE  bBufferMode;
} OSDBUF_INFO, * POSDBUF_INFO;

//CoCo1.10, support OSD dynamic reigon
typedef void (*pDynamicRegionRecoverFunc) (void);

//***************************************************************************
//Export functions
//***************************************************************************
void OSD_Output(BYTE bMessage, WORD wParam, BYTE bSec);
void OSD_Initial(void);
// LLY2.53, expand time value unit from WORD to DWORD
//void OSD_Trigger(BYTE bType, WORD wTime);
void OSD_Trigger(BYTE bType, DWORD dwTime);
void OSD_ChangeUI(BYTE bUI, BYTE bAction);
void OSD_AdjustRegion(void);
void OSD_AdjustOSDPos(void); // Brian0.75
void OSD_NormalMode(void);
BYTE OSD_RemoveUI(BYTE bUI);
void OSD_PushCurrentNBForeverMsg(BYTE bMessage);
BYTE OSD_RecoverNBForeverMsg(void);
void OSD_UpdateZoomState(void);
void OSD_RemoveAllUI(void);
BYTE OSD_NewDynamicRegion(pDynamicRegionRecoverFunc pRecoveryFunc); //CoCo1.10, support OSD dynamic region.
void OSD_DeleteDynamicRegion(void); //CoCo1.10, support OSD dynamic region.
BYTE OSD_CanUpdateDynamicRegionInfo(void); //CoCo1.10, support OSD dynamic region.
void OSD_PANEL_Output(BYTE bMessage, WORD wParam, BYTE bSec);
void OSD_SetBufferModeInfo(BYTE bBuffMode); //Alan2.21, set OSD buffer mode info
void OSD_GetBufferModeInfo(POSDBUF_INFO pOSDBuffer_Info);
void OSD_SetRegion(BYTE bRegion, BYTE bClearRegion, GDI_REGION_INFO *RegionInfo);

//***************************************************************************
//extern global variables
//***************************************************************************
extern STATUS gcShowStatus;
extern BYTE _bOSDLang;     // keep current osd language
extern BYTE _bSpeedFactor; // only support 0 ~ 255
extern BYTE _bWaitingCnt; //CoCo, this variable is for "waiting..." usage. Need to move it to OSDND?
extern BYTE __bOSDClearNDURegion;
extern BYTE __bOSDClearNDBRegion;
extern BYTE __bOSDForceToClearNBRegion;
//***************************************************************************
//Internal functions
//***************************************************************************
void _OSD_UpdateStatus(BYTE bMessage, WORD wParam);
void _OSD_PushForeverMsg(BYTE bMessage);
BYTE _OSD_SearchMsg(BYTE bMessage);
void _OSD_SetCurrentMsg(BYTE bMessage, WORD wParam, BYTE bSec);
void _OSD_RemoveMsgFromStack(BYTE bMessage);
void _OSD_ExitUI(BYTE bUI, BYTE bExitMode);
BYTE _OSD_RecoverUI(BYTE bUI, BYTE bRecoverType);
// LLY2.53, expand time value unit from WORD to DWORD
//void _OSD_UITrigger(BYTE bType, WORD wTime);
void _OSD_UITrigger(BYTE bType, DWORD dwTime);
BYTE _OSD_RecoverNDMessage(BYTE bMessagePos);
void _OSD_UpdateMessage(BYTE bMessage);
BYTE _OSD_GetUILayer(BYTE bUI);
void _OSD_RecoverNURegion(void);
void _OSD_RecoverNBRegion(void);
void _OSD_ClearMsg(BYTE bMessage);
void _OSD_ResetUI(void);
void _OSD_ClearDisplayRegion(BYTE bMessage);
void _OSD_ClearNUNBRegion(BYTE bRegion);
BYTE _OSD_MessageGroup(BYTE bMessage);
void _OSD_ResetState(void);
void _OSD_RecoverSpecialGraphZoom(void);
void _OSD_RecoverDynamicRegion(void); //CoCo1.10, support OSD dynamic region.
void _OSD_ConfigRegion(BYTE bUI);

#endif //__OSD_H__

