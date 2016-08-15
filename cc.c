//#define CARD_TEST   // Joey2.76: for card test

#define MAP_PLAYABLE_MESSAGE_ONLY 
// ***** Big Modification History Area *****

// The file "winav.h" must be included in first line
#include "winav.h"
#include "ctkav.h"  // ** TCH0.60; 

#include    <string.h>

#include "initial.h"
#include "utl.h"
#include "panel.h"     

#include "hal.h"

#include "SrcFilter.h"
#include "input.h"
#include "chips.h"
#include "thumb.h"
#include "comutl.h"
//#include "amp.h"
#include "gdi.h" //CoCo.CT909
#include "osdmm.h" //CoCo.CT909
#include "osddsply.h"
#include "osdsrch.h"
#include  "osdprgm.h"
// Micky0.80a, must clear the key value.
#ifdef SUPPORT_BOOKMARK
#include "osdbmark.h"
#endif//#ifdef SUPPORT_BOOKMARK

// LLY0.95, include DivX OSD related header file for using OSDCOMDLG_ProcessKey()
#ifndef NO_DRM
#include "osddlg.h"
#endif  // #ifndef NO_DRM

// ** TCH0.60; #ifndef SYSTEM_8051
BYTE    EX1, EX0, EA;
// ** TCH0.60; #endif

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
//#include "sysdebug.h" //CoCo.sd

// ** TCH0.60; begin... 
#include "thumb.h"
#include "media.h" 
// ** TCH0.60; end... 

#ifdef CT950_STYLE //CoCo2.38
#include "alarm.h"
#include "autopower.h"
#endif //CT950_STYLE

#include "disp.h"
#include "parser.h"
#include "comdec.h"
//#include "mpeg2dec.h"
#include "subpict.h"
#include "osdss.h"
//alex080,support cdrom auto upgrade
#include "cdg.h"  //LJY0.95
#include "osdap.h" // LLY1.50, used to add OSDAP_ProcessKey()
#ifdef IMAGE_FRAME_SETUP //CoCo2.38
#include "poweronmenu.h"
#endif //IMAGE_FRAME_SETUP //CoCo2.38

#ifdef SUPPORT_DMP
#include "dvdsetup_op.h"
#endif //SUPPORT_DMP

#ifdef SUPPORT_RADIO_TUNER
#include "radio.h"
#endif //SUPPORT_RADIO_TUNER

#ifdef SUPPORT_CHAR_SUBPICTURE
#include "char_subpict.h"  //Alan1.10, support char-based subpicture
#endif

#if defined(SUPPORT_STB) || defined(IMAGE_FRAME_SETUP) //CoCo2.38
#include "mainmenu.h"
#endif
// wyc2.76-909P
#if (USB_SYSTEM == USB2_0_JUNGO)
#include "usbwrap.h"
#endif //
#include "usbsrc.h"


extern BYTE MAIN_MENU_ProcessKey(void);

//#define   NO_CDG
DWORD   __dwW99AV_SrcDRAM;
DWORD   __dwW99AV_DestDRAM;

#define NULL_0      0       // ** TCH0.60; 

#ifndef NO_MP3

#include "Mmanager.h"

#endif  // #ifndef NO_MP3 

//CoCo1.02, add OSD game from XuLi's code
#ifdef SUPPORT_OSDGAME
#include "GameMain.h"
extern EN_GAME_ID __enGameID;
#endif

#ifdef SUPPORT_TFT        
    #include "tft.h"
    BIT _btAVIN=0; //Jeff 20060705 Add for AV in
    BIT _btTFT_PowerDown=0; //Jeff 20060705 Add for AV in
#endif 


char        __cBuffer [LENGTH_SHAREBUFFER+ 1];  // Max for SPPalette
BYTE        * __pbBuffer; // ** TCH1.60a-2; Pointer to __cBuffer for unsigned operation

// elmer2.75, make sure to 8-byte alignment
BYTE        __bPool [LENGTH_SHAREPOOL] __attribute__ ((aligned (8)));  // Common-Pool for DVD/ VCD    // ** TCH1.61;
//BYTE        __bPool [LENGTH_SHAREPOOL];  // Common-Pool for DVD/ VCD    // ** TCH1.61;
WORD        __wPoolOffset;  // record the current usage of Pool

// wyc2.37-909s, replace to NO_DISC_MODE
#ifndef NO_DISC_MODE //++CoCo2.37p
DWORD       __dwFWBufUsedSize;
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p

// Micky2.79, tune the usage of __bCountStop, don't reference DOUBLE_STOP
// __bCountStop=0, means playing(include pause)
// __bCountStop=1, means pre-stop(Value 1 exist only when DOUBLE_STOP is defined)
// __bCountStop>=2, means real-stop
BYTE    __bCountStop;
BYTE __bLOGO; //CoCo, Need to move the variable's declaration to SWOSD.

BYTE *__bpDataPointer;

BIT     __btNTSCBitStream;
// for debugging!!!!
DWORD   __dwCheckTime;
BYTE    __bZoomFactor;

#ifndef NO_DIGEST
extern  BYTE        __bTrackNO;         // DIGEST usage, current track NO (Had played)
//LJY0.87(LJY2.81), for DVCD discview
#endif

DWORD   __dwOpenTime;

// wyc.172-2, extern this variable from servo and move to begin of program
extern BYTE __bCloseCommand ;
// wyc0.95, extern variable.
extern DWORD   __dwFileTime;
// wyc1.02-909, extern variable
extern BYTE _bCDGState;

//  global definition
//DVD_051Micky, add byte size timer count
// for avoid DWORD data move by interrupt inteference
// must clear it as 0 before using
// only alow max. 3 seconds(base on 50Mhz)
//BYTE        __bSystemTimer;
DWORD       __dwSystemTimer;

DWORD       __dwCountSystem;

DWORD           __dwTimeBegin;
DWORD           __dwTimeEnd;
DWORD   __dwTimeNow;

WORD    __wDiscType;                    // Record Disc Type
BYTE            __bDiscInfo;                    // ** TCH1.70; Record Disc
// wyc1.25, add one variable to record flow when detected as wrong disc for debug usage,
BYTE    __bTestFlow;

BYTE    __bKey;
// LLY0.62, add new variable to keep F/W assign key for IR and F/W key conflict issue
BYTE    __bFWKey;

// LLY1.05, remove __dwTimeRef to CELL module, since only it will use it.
//DWORD           __dwTimeRef;

// record a disc playable Begin Sectors (VCD is from Track 2)
DWORD           __dwTimeDiscBegin;        
// record a disc playable End Sectors
DWORD           __dwTimeDiscEnd;

//Alex1.21,20031210 support bookmark
#ifdef SUPPORT_BOOKMARK
BYTE            __bBookMark_Title[BOOKMARK_NO+ 1];        // record BookMark Title
WORD            __wBookMark [BOOKMARK_NO+ 1];             // record BookMark Time //CoCo1.20, reduce code size.
//Aron2.38, support still resuming
#ifdef SUPPORT_STILLRSM
BYTE            __bBookMarkIsStill [BOOKMARK_NO+ 1];
DWORD           __dwBookMarkVOBU [BOOKMARK_NO+ 1];
#endif
BIT             __btBookMark;             // BookMark ON/OFF (PBC OFF) //CoCo1.20, reduce code size.
#endif //#ifdef SUPPORT_BOOKMARK

// default VCD is track 2;
WORD            __wPlayItem;
// wyc2.53-909P,
WORD            __wBGPlayItem;
// record VCD 2.0 List type
WORD            __wStatus;

// Micky0.62-909, removed
// [909 Removed]
/*
//DVD_047-1. Number confirm
void _ConfirmAction(WORD wValue);
WORD            _wPreValue;//Micky2.80, review global variables init.=0xffff;

DWORD   __dwTimeNumber;
*/
BYTE            __bModeCmd;             // Record current macro command
                                        // Remove the initialization as it add Initial_Variables()

WORD            __wTrackBegin;// first valid track
WORD            __wTrackEnd;// last valid track
BYTE            __bRepeat= REPEAT_NONE;         // record repeat mode
                                // REPEAT_NONE: non-repeat, REPEAT_TRACK:repeat one track,
                                // REPEAT_DISC: repeat all tracks
BYTE            __bModeChannel;// record channel information
BYTE            __bAudioType;// record Audio Type information

BYTE            __bError;// record the previous error

BYTE            __bTimeMin, __bTimeSec;         // GOTOTime Usage

BYTE            __bRepeatAB;    // RepeatAB ON/OFF (PBC OFF)
BYTE            __bShowTimeType;// Output Module use. ref to osd.h
BYTE            __bGoToTimeNO;                  // count the I/P Number in GoToTime
DWORD __dwEntryStart;
///////////////
// DIGEST Usage
// Micky1.20, 4M flash
#ifndef NO_DIGEST
//jyliu.pc908, using BYTE instead of BIT
BYTE             __btDIGEST;             // means Digest Mode
#endif

//////////////////////////////////////////////////////
BIT             __btPlaying;// means is playing (NPF/FF/...)
BIT             __btPlayEnd= TRUE;             // Check if it's into Play-End mode
BIT             __btPause;// if it's into Pause mode
BIT             __btPBC;// means VDC2.0 PBC
BIT             __btDriveOpen;// means drive is in close mode when Power-ON
BYTE            __bFirstPlay;// Does it play first time ?

                // add the usage of recover hang
BIT             __btMute;// Disable/Enable Mute

BIT             __btRandom;// in random mode (PBC off)

BYTE            __bAutoVocal;// Enable Auto-Sing

BYTE            __bScan;                // The Status of SCAN Command (encounters IFSC)

DWORD                   _dwTimeCheckTray;

// global usage; don't remove those variables
// Please don't use in the functions that be called by Interrupt
DWORD      __dwTemp;
WORD       __wTemp;
BYTE       __bTemp;
BIT             __btTemp;

// __wSlowSpeed is actual slow speed ratio
WORD            __wSlowSpeed;// Slow factor
BYTE    __bSlowLevel;

BYTE    __bTimeHour;    // GoTo Time usage

// GOTOTIME
DWORD           __dwGoToTime;   // Time that use GOTO position 
BIT             __btNewTime;// A flag that means it's a NEW time
BIT             __btPlayFromBegin;// Told F/W it should play Track-Begin first
BIT             __btFromStopMode;// GOTO position comes from a "STOP" mode
// LLY2.36, create a new variable to keep the desired STC value for GoTime action
// To avoid the inaccuracy from STC/90K = sec unit, then * 90K again to get PTS
DWORD           __dwGoTimeSTC;

// LLY2.61, create a new variable to keep time A STC value for Repeat A-B
// ie. don't share it w/ __dwGoTimeSTC
// Since, stop/ resume case also save the STC in __dwGoTimeSTC
// So, the value maybe overwritten during repeat A-B --> enter/exit SETUP (do stop/resume)
DWORD           __dwTimeA_STC;

// wyc2.05-909S, porting fish's code for MP3 resume.
// Keep the final stop point
// DivX/ AVI/ MP4: second unit for parser
// Others (not include DVD title): sector ID (will minus buffer remain)
DWORD           __dwResumeTime;// Time that record final stop point

// LLY2.05, porting Wing/ YC code to support MP3 disc resume
// [1] __dwResumeItem
// HIWORD: playing directory index (from 0); LOWORD: __wPlayItem (from 1)
DWORD           __dwResumeItem;

// [2] __dwGoTimeFrameCnt
// Keep GoTime frame count, since it must calculate base on GoTime sector & bitstream bitrate, samplepreframe...
// And, the value is only valid after DSP start to decode the audio.
// So, the value must be set during 
// (1) Keep TimeA point for Repeat A-B case
// (2) Keep resume time point for resume case
// (3) Calculate GoTime value for GoTime case
DWORD           __dwGoTimeFrameCnt; 


BIT             __btPowerDown;// PowerDown Mode ON/OFF


// TCC155, a flag for checking tray is open or closed
BYTE            _bOpen;

BYTE            __bASTN;   // Audio Stream Number, from "0".
BYTE            __bASTNO;    // Index of Audio Stream Number, from "0"-"__bASTNs-1".     // ** TCH0.451;
BYTE            __bASTNs;    // Max. Audio Stream Number.
BYTE            __bASTID;       // The Real sub_stream_id. DVD may be different than __bASTN.

// Micky0.90a, SP2 control, add SP display status for SP2
// wyc1.00-909, add comment for __bSP_Disp_OFF
BYTE            __bSP2_Disp_OFF;    // 1, SP2 is off by user, 0x10, SP2 is off by SP1 off.
BYTE            __bSP_Disp_OFF;     // 1,  SP1 is off by user,0x10, SP1 is off by NV cmd, 1 ~ 127 means sp off 0, 128 ~ 255 means sp on
BYTE            __bSPSTN;       // SP Stream Number, from "0".
BYTE            __bSPST2N;       // Aron1.01 SP Stream Number for SP2, from "0".
BYTE            __bSPSTNO;    // Index of Audio Stream Number, from "0"-"__bSPSTNs-1".   // ** TCH0.451;
BYTE            __bSPST2NO;   // Chuan0.75, Tndex of Subpicture Stream Number2.

BYTE            __bSPSTNs;   // Max. SP/OGT Stream Number.
BYTE            __bSPSTID;      // The Real sub_stream_id. DVD may be different than __bSPSTN.
BYTE            __bSPST2ID;  // Chuan0.75, The Real sub_stream_id 2.

BYTE            __bAGLN;    // Angle Number. from "0".
BYTE            __bAGL_Ns;  // Number of Angles.

// LLY2.37, following variable is removed from NAVIAGER.C/ CELL.C
// Since, they are not used only for DVD title
BYTE    __bACHN;                // Record current Audio Channel Number. From "1"..
WORD    __wChapter_Ns_Title;    // Record the Total Chapters/ current Title (PTT)
WORD    __wV_ATR;               // Record Video Attribute (current)
WORD    __wSP_LCD;              // SP Language
WORD    __wA_LCD;               // Audio Language
DWORD   __dw1stSCR;             // Keep 1st SCR value


BOOL   __bSmartPB_ABLE;
BYTE   __bMaxPBT_TT;   // the Title with max playback time in current DVD disc


#ifndef NO_SCREEN_SAVER
BYTE    __bScreenSaver; //=NONE_SCREEN_SAVER;
#endif  // #ifndef NO_SCREEN_SAVER

DWORD        __dwSTCValue;  // ** TCH0.99-1; Record Current STC Value
DWORD        __dwTimeA, __dwTimeB;           // record the times for repeatAB

// LLY2.61, add new variable to keep A point frame count value for audio only file's repeat A-B usage
DWORD       __dwTimeA_FrameCnt;

BIT     __btIntroMode;
DWORD   __dwIntroPlayTime;

// wyc2.22-909s, new variable to check if force enter stop mode.
DWORD   _dwForceStopMode;
//Kevin1.22a, utl will use also, don't declare as static
DWORD    _dwTimerCount_MainLoop;// ** TCH0.99-1; Main-Loop Counter


BYTE    _KeyGroupTest(BYTE bKey);
BYTE    _KeyGroupAudio(BYTE bKey);
BYTE    _KeyGroupPlay(BYTE bKey);
BYTE    _KeyGroupMode(BYTE bKey);
// ** TCH0.60; BYTE    _KeyGroupMultiDisc(BYTE bKey);
BYTE    _KeyGroupVideo(BYTE bKey);
BYTE    _KeyGroupOthers(BYTE bKey);

#ifdef SUPPORT_AV_IN
BYTE    _KeyGroupTFT(BYTE bKey); //Jeff 20060705 add for TFT
#endif //#ifdef SUPPORT_AV_IN


DWORD   __dwTimerCount_Keyscan=0; //Jeff 20050530 add for panel key

// DVD_273.2, for encrpyted DVD begin
// clear the auto scan next cell's command.
BYTE    __bAutoScans;
// DVD_273.2, for encrpyted DVD end

//DVD_039-1SCF
BYTE    __bScanLevel;
//DVD_043-1
BYTE    __bInvalidRegion;
//DVD_049MickyPowerDown
#ifdef  AUTO_POWERDOWN
// support auto power down
// when the value is 0xFFFFFFFF, means don't need do auto power down check
// other value, means need do auto power down check.
DWORD __dwTimeWaitPowerDown;
#endif

// wyc1.06-909, vairable to represent current play mode is from AVI scan
BYTE _bAVIScan2Play;
// Because REPEATAB and SLOW use same OSD regin and they are both forever message, so need to add one variable to decide which is active message.
BYTE __bActiveMessage;
// wyc2.77-909p, add the usb performance test code.
#ifdef TEST_DEVICE_PERFORMANCE
extern    DWORD   __dwTestUSBSpeed;
#endif //

// DVD_099bMicky, support another backdoor method
// DVD_100-3Micky, the general backdoor is supported in drive open case
#ifdef  SUPPORT_GENERAL_BACKDOOR
// Brian1.24, add another general backdoor
#define BACKDOOR_NUMBER     3       // number of backdoors that supported
#define BACKDOOR_KEYNUM     4       // number of passwords for backdoor
// DVD_100-3Micky, support backdoor for reset default setup data
BYTE    code __bBackDoorKeys[BACKDOOR_NUMBER][BACKDOOR_KEYNUM]=
        {
            {KEY_N9, KEY_N9, KEY_N2, KEY_N2},   // #1 backdoors
            {KEY_N1, KEY_N2, KEY_N3, KEY_N4},   // #2 backdoors
            {KEY_N6, KEY_N6, KEY_N6, KEY_N6},   // #3 backdoors // Brian1.24, Total default
        };
#define BACKDOOR_VIDEO_OUTPUT   0
#define BACKDOOR_SETUP_DEFAULT  1
#define BACKDOOR_SETUP_TOTAL_DEFAULT  2 // Brian1.24, total default

BYTE    __bKeyIndex[BACKDOOR_NUMBER];           // the index for each backdoor
#endif

// DVD_099cMicky, support show OSD only when IR key is pressed
#ifdef  SHOW_IR_KEY_OSD_ONLY
// the mapping table for KEYs & MSGs
// Only list MSGs, and KEY sequence is according to input.h
// start from INPUT_KEY_GROUP2(KEY_MUTE)
// Micky2.79-2, don't need specify the array size
BYTE code   _bMessageMap[]={
// for 0.80, should keep group3 only. add next/prev extra.
#ifdef  MAP_PLAYABLE_MESSAGE_ONLY    

                                MSG_PLAYING, MSG_STOP, MSG_FAST,
                                MSG_SCANFORWARD, MSG_SCANBACKWARD,MSG_SLOW,
                                MSG_PAUSE, MSG_STEP, MSG_FREEZE,
                                MSG_NULL,   // dummy
                                MSG_PBC, MSG_NEXT, MSG_PREV,
#else
                                // Group 2, 20s
                                MSG_MUTE, MSG_VOLUME, MSG_VOLUME,
                                MSG_KEY, MSG_KEY, MSG_KEY,
                                MSG_ECHO, MSG_ECHO, MSG_AUDIO,
                                MSG_SURROUND, MSG_VOCAL, MSG_AUTOVOCAL,
                                MSG_VOICECANCEL,MSG_3D, MSG_KARAOKE,
                                MSG_AST_CTL,
    //CoCo.099d
    // support a key to switch audio output(2 ch/5.1ch/spdif)
                                // Micky1.10, map KEY_MIC_VOL_UP/DOWN to MSG_VOLUME
                                MSG_AUDIO_OUTPUT_CTL, MSG_NULL, MSG_VOLUME, MSG_VOLUME, // dummy
                                // Group 3, 10s
                                MSG_PLAYING, MSG_STOP, MSG_FAST,
                                // Micky1.07, fix the key message sequence error
                                // these message are forever display, OSD won't check message
                                // but FM will use 3s.
                                MSG_SCANFORWARD, MSG_SCANBACKWARD,MSG_SLOW,
                                MSG_PAUSE, MSG_STEP, MSG_FREEZE,
                                MSG_NULL,   // dummy
                                // Group 4, 30s
                                MSG_PBC, MSG_NEXT, MSG_PREV,
                                MSG_RETURN, MSG_PROGRAM, MSG_NULL,
                                MSG_PROGRAM, MSG_NULL, MSG_NULL,
                                MSG_TVMODE, MSG_DISPLAY, MSG_DIGEST,
                                MSG_REPEATAB, MSG_REPEAT, MSG_INTRO,
                                MSG_RANDOM, MSG_REMAIN_ABSTIME, MSG_RESUME,
                                MSG_GOTOTIME, MSG_NULL, MSG_OPEN,
                                MSG_POWER, MSG_NULL,//KEY_DEFAULT
                                // Micky2.81, add message for thumbnail.
                                MSG_NULL, MSG_NULL, MSG_PSCAN, MSG_NULL, // dummy
                                MSG_NULL, MSG_NULL, MSG_NULL, // dummy
                                // Group 5, 15s
                                MSG_NULL, MSG_NULL, MSG_NULL, MSG_NULL, // KEY_LED1~4
                                MSG_NULL, MSG_NULL,                     // KEY_DISCSKIP, SKIP_BACK
                                // change MSG_DISCPOSITION into MSG_NULL
                                MSG_NULL, MSG_NULL, MSG_NULL,
                                MSG_NULL, MSG_NULL, MSG_NULL,
                                MSG_NULL, MSG_NULL,
                                MSG_NULL,           // dummy
                                // Group 6, 25s
                                MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER,
                                MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER,
                                MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER, MSG_NUMBER,
                                MSG_KEY10, MSG_KEY10,
                                MSG_NULL, MSG_NULL, // dummy
                                // Group 7, 10s
                                // Micky2.80, accord cc.h ZOOM ID for HAL_Zoom.
                                // MS_ZOOMMOVE will be removed.
                                MSG_ZOOM, MSG_ZOOM, MSG_ZOOM,
                                MSG_ZOOM, MSG_ZOOM, MSG_SPST_CTL,
                                /*
                                MSG_ZOOM, MSG_ZOOMMOVE, MSG_ZOOMMOVE,
                                MSG_ZOOMMOVE, MSG_ZOOMMOVE, MSG_SPST_CTL,
                                */
                                // DVD_101bMicky, for KEY_VIDEO_OUTPUT_CTL
                                MSG_VIDEO_OUTPUT,
                                MSG_NULL, MSG_NULL, MSG_NULL,   // dummy
                                // Group 8, 20s
                                MSG_NULL, MSG_NULL, MSG_NULL,
                                MSG_NULL, MSG_NULL,
                                MSG_NULL, MSG_NULL, MSG_NULL, MSG_NULL,// dummy
                                MSG_NULL, MSG_NULL, MSG_NULL, MSG_NULL,// dummy
                                MSG_NULL, MSG_NULL, MSG_NULL, MSG_NULL,// dummy
                                MSG_NULL, MSG_NULL, MSG_NULL,           // dummy
                                // Group 9, 20s
                                MSG_NULL, MSG_NULL, MSG_INVALID,
                                MSG_INVALID, MSG_NULL,      // ** TCH0.60-1;  OSDGAME is not exist
                                MSG_NULL, MSG_NULL, MSG_NULL, MSG_NULL,// dummy
                                MSG_NULL, MSG_NULL, MSG_NULL, MSG_NULL,// dummy
                                MSG_NULL, MSG_NULL, MSG_NULL, MSG_NULL,// dummy
                                MSG_NULL, MSG_NULL, MSG_NULL,          // dummy
                                // Group 10
                                MSG_MENU, MSG_MENU, // LLY.099c
                                MSG_NULL,
                                MSG_NULL, MSG_NULL,
                                MSG_ANGLE_CTL,
                    // Micky2.79-2, support FullScreen function
                    // MSG_FULLSCREEN
                                MSG_NULL,   // KEY_ACTIVE
                                MSG_NULL,   // KEY_HOTKEY
                                MSG_FULLSCREEN,   // MSG_FULLSCREEN
#endif                                
                                };
#endif
BYTE    __bKeyMessage;
BYTE    __bKeySource;


// wyc0.91, Brain's updated code.
// Brian0.91
// define the version number variable for each protected modules
// these variables are initialized to 0, and the correct version value is assigned by related module.

// the version of these module is expressed in 0xYYYYMMDD format : 
// for example, 2005-02-21 will be expressed as 0x07D5 02 15

DWORD   __dwVersionMPEG=0;     // MPEG decoder module version
DWORD   __dwVersionJPEG=0;     // JPEG decoder module version
DWORD   __dwVersionDisplay=0; // Display module version
DWORD   __dwVersionDivX=0;     // DivX module version
DWORD   __dwVersionNavigate=0; // Navigate module version
DWORD   __dwVersionParser=0;     // Parser module version
// wyc1.20-909, for USB version.
DWORD   __dwVersionUSB=0;       // USB module version.
// wyc1.50-909, new protected module group.
DWORD   __dwVersionINFO=0;       // INFO module version.
DWORD   __dwVersionCard=0;
    
// the version of these module is expressed in xx.xx format: 
// high WORD is the F/W digit part
// high BYTE of low WORD is the alpha-beta part for RD release use, 0 means 0, 1 means a, 2 means b,...
// low BYTE of low WORD is the alpha-beta part for FAE release use, 0 means 0, 1 means a, 2 means b,...
// for example, for RD release F/W
//                    0.90  ==>   0x005a 0100
//                    0.90a  ==>   0x005a 0100
//                    0.90b  ==>   0x005a 0200
//                    1.00  ==>   0x0064 0000
//                    1.01a  ==>   0x0065 0100

// for FAE, according to Version Naming Rule, FAE could only change the LSB
//                    0.90_a  ==>   0x005a 0001
//                    0.90aa  ==>   0x005a 0101
//                    0.90bc  ==>   0x005a 0203
//                    1.00_a  ==>   0x0064 0001
//                    1.01aa  ==>   0x0065 0101

DWORD   __dwVersionServo=0;     // Servo module version
DWORD   __dwVersionServoBM=0; // Servo BM version
DWORD   __dwVersionServoUCode=0; // Servo uCode version
#ifdef SUPPORT_TFT      //Alan2.17, for TFT 
DWORD   __dwVersionTFT = 0; // VD version
#endif 

DWORD   __dwVersionRelease= (SW_VERSION<<16) | (SW_MINOR_VERSION<<8);   // F/W release version
DWORD   __dwVersionFAE= (SW_VERSION<<16) | (SW_MINOR_VERSION<<8) | FAE_SW_MINOR_VERSION;       // FAE version
// Customer version may be free format, so use string
const BYTE __csVersionCustomer[] = CS_VERSION_CUSTOMER; // Customer version string

// wyc1.05-909, rename _CC_ResetState to CC_ResetState
// DVD_172Micky, FLOW_CONTROL
// let __bFlowState be general use for auto play.
// DVD_161Micky, support Loading-> Setup-> Exit, will continue loading action
///#ifdef  SUPPORT_LOADING_SETUP
BYTE    __bFlowState ;
BYTE    _ProceedFlowState(void);
BYTE _CC_AVIScanToPlayProcess(void);

#ifdef  ENABLE_AUTOVOCAL

BYTE _CC_AutoVocal(BYTE bValidKey);

DWORD __dwMICDectectInterval;
bit   _btPreMICDetect;
BYTE  _bMICOffCount;

#ifdef  SW_MIC_DETECT
bit __sbMICDetect;
#endif

#endif  //#ifdef  ENABLE_AUTOVOCAL


#ifdef   _SHOW_IR_MAP
BYTE        _bIRMap [4], _bIRGet;
#endif  // _SHOW_IR_MAP

// Brian.276-5, for system hang control
// Timer0 ISR use it value to decide if the man loop is hanging
// wyc1.01-909, remove it because of not use it.
//DWORD __dwProgressCount;

// Micky2.79-2, support FullScreen function
// init. value 0 will be assigned within INITIAL_Variables.
// 1: means full screen video on, 0: means full screen video off.
BYTE    __bFullScreen;

// Micky2.80, Modify global variable init.
// move from INTIIAL.c
WORD    __wScanGap;     // the real sectors that will jump for each SCAN

// Micky2.81, Support different number of digits for different mode
// VCD/ DVD : max number is 9-
// AVSYS      : 999-
// when exceed the max. number, will reset the number as 0
//WORD    __wMaxNumber;

CHAR __csUPGAPName[] = CS_UPGAP_NAME; // Chuan2.78, Define UPGAP Name.

// Micky1.05, add auto test for IC sorting by CoCo
#ifdef AUTO_TEST //CoCo1.07, verify auto-test
//alex1.23,20040218 combine gototime , search
//in autotest function, key_search replace key_gototime 
/*
WORD aDVDRule[] = {COUNT_2_SEC, KEY_SEARCH, 0, KEY_N2, 0, KEY_PLAY, 0, KEY_GOTOTIME, 0, KEY_GOTOTIME, 0, KEY_N2,
            0, KEY_N2, 0, KEY_N2, 0, KEY_N0, COUNT_1_SEC, KEY_PLAY, 0, KEY_AST_CTL, COUNT_3_SEC*2, KEY_AST_CTL,
            0, KEY_AST_CTL, COUNT_5_SEC+COUNT_3_SEC, KEY_STOP, 0, KEY_STOP, 0, 0};
*/
// Brian2.17
WORD aDVDRule[] = {0, KEY_DISPLAY, 0, KEY_DISPLAY, 0, KEY_DISPLAY, 0, KEY_DISPLAY, 0, KEY_DISPLAY, 0, KEY_DISPLAY, 0,
                    KEY_SEARCH, 0, KEY_N3, 0, KEY_PLAY, 0, KEY_SEARCH, 0, KEY_SEARCH, 0, KEY_N1, 0, KEY_N2, 0, KEY_N7, 0, KEY_N1, 0, KEY_N5, 0, 
                    KEY_PLAY, 0, KEY_SPST_CTL, 0, KEY_AST_CTL, (COUNT_3_SEC*2+COUNT_2_SEC), KEY_STOP, 0, KEY_STOP, 0, 0};


WORD aCDROMRule[] = {COUNT_1_SEC * 7, KEY_STOP, 0, KEY_STOP, 0, 0};
DWORD __dwAutoTestTime;
BYTE __bRuleIndex;
BYTE __bAutoTest;
WORD *_wpRule;
#endif

#ifdef SUPPORT_POWER_SWITCH
BYTE __bPowerSw_StandBy;
BYTE __bPowerSw=TRUE;
extern	BYTE _bBacklight;
#endif //SUPPORT_POWER_SWITCH

//Alex1.07a SHOW PSCAN PROMPT
#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
//Alex1.07a 20031013 SUPPORT_PSCAN_PROMPT_OSD
#ifdef SUPPORT_PSCAN_PROMPT_OSD
extern BYTE OSDPROMPT_ProcessKey(BYTE bKey);
#endif //#ifdef SUPPORT_PSCAN_PROMPT_OSD
extern void OSDPROMPT_Trigger(void);
extern BYTE __bOSDPromptMode;
#endif //#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)


// Micky1.07, adding DEBUG info variable declaration
BIT     __btDumpFlag;
BYTE    __bDumpType;

// Micky1.23 support resume info from EEPROM
BYTE    _bAPPAction;

// Micky0.65-909. add a time count for OSD trigger.
DWORD   _dwTimeCount_OSDTrigger = 0;
// winnie 2.77, add to save volume function and fix play move un-continually issue while press vol +/- quickly
// The reason is that volume value be stored frequently. So, using time count to store the volume value.
#ifdef SUPPORT_SAVE_VOLUME
extern DWORD   __dwVolAudio;
DWORD   __dwTimerCount_Volume=0; 
#endif
// Micky0.80, new method for the ProcessKey
//void    (*KeyProcess)(BYTE);    // OK
typedef BYTE (*pFunction)(void);
pFunction FuncArray[]=
{
    &OSDAP_ProcessKey, // LLY1.50, add AP process key routine
#ifdef SUPPORT_TFT
    &TFT_ProcessKey,
#endif

#ifdef SUPPORT_OSDGAME //CoCo1.02, add OSD game from XuLi's code
    &GAMEMAIN_ProcessKey,
#endif

#ifdef SUPPORT_POWERON_MENU //CoCo2.56
    &POWERONMENU_CheckModeKey,
#endif //SUPPORT_POWERON_MENU

#ifdef DOLBY_CERTIFICATION //kevin0.80
    &CHIPS_Dolby_ProcessKey,
#endif    
// LLY0.95, add Common Dialog process key routine
// Notice: The priority is higher than PSCAN prompt.
#ifndef NO_DRM
    &OSDCOMDLG_ProcessKey,
#endif  // #ifndef NO_DRM
#ifdef  SUPPORT_PSCAN_PROMPT_OSD
    &OSDPROMPT_ProcessKey,
#endif  //#ifdef  SUPPORT_PSCAN_PROMPT_OSD    
#ifdef SUPPORT_STB
    &MAIN_MENU_ProcessKey, //CoCo.stb.20060320
#else
#ifndef NO_SETUP
#ifdef IMAGE_FRAME_SETUP //CoCo2.38
    &MAIN_MENU_ProcessKey, 
#else
    &SETUP_ProcessKey,
#endif //IMAGE_FRAME_SETUP
#endif //#ifndef NO_SETUP    
#endif //SUPPORT_STB

#ifdef SUPPORT_POWERON_MENU //CoCo2.38
    &POWERONMENU_ProcessKey,
#endif

#ifdef SUPPORT_RADIO_TUNER
    &RADIO_ProcessKey,
#endif //SUPPORT_RADIO_TUNER

    &MEDIA_MediaSelection_ProcessKey,

#ifndef NO_PROGRAM_UI //++CoCo2.37p
    &OSDPRGM_ProcessKey,
#endif //#ifndef NO_PROGRAM_UI //++CoCo2.37p

#ifndef NO_SEARCH_UI //++CoCo2.37p
    &OSDSRCH_ProcessKey,
#endif //#ifndef NO_SEARCH_UI //++CoCo2.37p

#ifdef SUPPORT_BOOKMARK
    &OSDBOOKMARK_ProcessKey,
#endif  //#ifdef SUPPORT_BOOKMARK
#ifndef NO_DIGEST
    &DIGEST_ProcessKey,
#endif  //#ifndef   NO_DOIGEST
    &_CC_NumberProcessKey,     //Alan2.37, move it after DIGEST_ProcessKey() for Mantis #1722
    &THUMB_ProcessKey,
    &MM_ProcessKey,
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
    &OSDDSPLY_ProcessKey,
#endif //#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p

// wyc2.37-909s, replace to NO_DISC_MODE
#ifndef NO_DISC_MODE //++CoCo2.37p
    &CC_VCD_ProcessKey,
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p

#ifndef NO_DISC_MODE //CoCo2.37p
    &CC_DVD_ProcessKey,
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

};
BYTE    bMaxFunctions = sizeof(FuncArray)/sizeof(pFunction);

extern BYTE __bMMAutoPlay; //CoCo2.38

// wyc2.16c-909S, cover in SUPPORT_STB define.
#ifdef SUPPORT_STB
extern int STB_Entry(void); //CoCo.stb
extern void AP_MainLoop(void); //CoCo.stb
extern BYTE __bMediaSelected;
extern BYTE _bPrevMedia, _bNextMedia;
#endif
// wyc2.16-909S
// Micky2.16
#ifdef  SYSTEM_ECOS
//  ***********************************************************************
//  Function    :   cyg_user_start
//  Description :   Entry point of eCos OS
//                  Now, only creat and start the CTKDVD thread
//  Argument    :   None
//  Return      :   None
//  ***********************************************************************
void cyg_user_start(void)
{
    extern  void INITIAL_RunSystem(void);

#ifndef NO_DRM
    // added by J500CSC_20060823, for initialize DivX DRM usage
    PARSER_DRM_EarlyInit( );
    
    // added by J500CSC_20060823, for initialize the correct DivX DRM Model ID for Vestel
    PARSER_DRM_SetModelID( MODEL_ID_LOW_BYTE, MODEL_ID_TOP_BYTE );
#endif //

    INITIAL_RunSystem();
}
#endif  //#ifdef  SYSTEM_ECOS

//  *********************************************************************
//  Function    :   main ()
//  Description :   The Firmware Entry point
//  Arguments   :   NONE
//  Return      :   NONE
//  Side Effect :   What ???
//  *********************************************************************
//ccc begin
// ** TCH0.60; main ()


void CC_DVD_MainLoop(void)
{
    while ( 1 )
    {
#if 0        
    // elmer2.37, move watchdog mechanism form here to alarm
#ifdef SUPPORT_WATCH_DOG
    REG_SRAM_WATCHDOG++;
#endif
#else
    MONITOR_CheckWatchDog();
#endif    
    DBG_Polling();

    // LLY2.36, do IR polling while checking mode is polling
    if(INPUT_GetIRCheckMode()==IR_CHECK_MODE_POLLING)
    {
        DSR_IR();
    }
    
// Micky1.05, add auto test for IC sorting by CoCo
#ifdef AUTO_TEST
    _CC_AutoTest(); //CoCo1.07, verify auto-test
#endif

// ==============================================================
/// Section[2]  : Show IR Key codes
///             : To display the IR Key code before the key was processed
// ==============================================================
#ifdef  _SHOW_IR_MAP
    {
        void _CC_ShowIRMap();
        unverified. // Micky0.80
        _CC_ShowIRMap();
    }
#endif  //#ifdef  _SHOW_IR_MAP
/// End Of Section[2]  : Show IR Key codes
    // wyc2.17-909S, follow will do it, so don't need to call it here.
    //PANEL_Trigger(0); //Jeff 20050530 Add for Panel Trigger

// ==============================================================
/// Section[3]  : Process the input Key
///             : Process the IR input key or F/W key.
/// 3.1) When key is from IR, will transfer the key to proper value, 
///      and do OSD message map.
// ==============================================================
    CC_MainProcessKey();
/// End Of Section[3]  : Process the input Key
// winnie 2.77, add to save volume function and fix play move un-continually issue while press vol +/- quickly
// The reason is that volume value be stored frequently. So, using time count to store the volume value.
#ifdef SUPPORT_SAVE_VOLUME
     if((OS_GetSysTimer()-__dwTimerCount_Volume)>COUNT_2_SEC)
     {      
        //Compare the stored volume with current volume. if two value is different, volume needs to save.    
        if( __dwVolAudio != (DWORD)DVDSETUP_GetVolume())
        {
            DVDSETUP_SaveVolume();        
        }      
        __dwTimerCount_Volume=OS_GetSysTimer();
     }    
#endif     
    //Jeff 20050530 
    // wyc2.17-909S, suppose 300ms is enough.
    if((__bISRKey==KEY_NO_KEY)&&((OS_GetSysTimer()- __dwTimerCount_Keyscan)>COUNT_300_MSEC))     
    {
        __bISRKey = PANEL_KeyScan();    
#ifdef SUPPORT_POWER_SWITCH    //process power Switch
        if(__bISRKey == KEY_PICTURE)
        {
            // Switch On, and power always OFF            
            if ((!__btPowerDown)) 
            {                  
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"\n------POWER OFF=%d ---------",__btPowerDown);
                CC_PWRCtrl();                  
            }           
            __bISRKey=KEY_NO_KEY;             
            __bPowerSw=TRUE;
            
        }
        if (__bISRKey==KEY_LCD)
        {
            //Switch OFF,                      
            if ((__btPowerDown) && (__bPowerSw))
            {
                extern BYTE _bPreRepeatFlag;
                _bPreRepeatFlag=1;
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"\n------POWER ON=%d ---------",__btPowerDown); 
                __bISRKey=KEY_POWER;
                CC_PWRCtrl();                
                POWERONMENU_Initial(); 
                
            } 
                __bPowerSw=FALSE;
                __bISRKey=KEY_NO_KEY;
                 
        }
#endif //SUPPORT_POWER_SWITCH
        PANEL_Trigger(0); 

#ifdef SUPPORT_TFT        
    TFT_Trigger();
#endif
        __dwTimerCount_Keyscan = OS_GetSysTimer();    
    }          
                                                         
#ifdef SUPPORT_STB
    if( __bChooseMedia != MEDIA_SELECT_DVD && __bChooseMedia != MEDIA_SELECT_USB)
        break;
#endif

#ifdef CT950_STYLE //CoCo2.38
#ifdef SUPPORT_POWER_SWITCH //process Power Switch
    if (!__bPowerSw)
        AUTOPWR_Trigger();
#else
    AUTOPWR_Trigger();
#endif //SUPPORT_POWER_SWITCH
    ALARM_Trigger();
#endif //CT950_STYLE

/// *** For PowerDown mode, Sector[4]~[9] are skipped
    // Nothing more needed to be done at PowerDown mode.
    if (__btPowerDown)
        goto MAIN_LOOP_END;                

//CoCo1.02, add OSD game from XuLi's code
#ifdef SUPPORT_OSDGAME
        if(__enGameID!=GAME_NONE)
        {
        GAMEMAIN_Trigger();
        goto MAIN_LOOP_END;  
        }
#endif

    // Micky0.80, put after Power down check.
#ifdef  SUPPORT_MEDIA_MANAGEMENT
    MEDIA_Management();
#endif


// ==============================================================
/// Section[4]  : Check the time out for input number mode
/// 4.1) Process the time out for input number
/// 4.2) Process the time out for KEY_N10PLUS
/// Comments:
///     - This section is not necessary for SETUP.
// ==============================================================
    _CC_Trigger_NumberTimeOut();
/// End Of Section[4]  : Check the time out for input number mode

// ==============================================================
/// Section[5]  : Modes Trigger
///             : For specific mode, it may need some auto run behavior
///             : while no user input
/// 5.1) SETUP Trigger
// ==============================================================
// Micky0.80, will be done within OSD_Trigger.
/*
/// 5.1) SETUP Trigger
    // brian.277a-3rd, allways call it.
    SETUP_Trigger();

//  5.4) OSDPROMPT Trigger    
//Alex1.07a, timeout for showing PSCAN PROMPT notes
#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
    OSDPROMPT_Trigger(); 
#endif // #if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
*/
    OSDSS_Monitor(); //Add the monitor to enter screen saver.

// Micky0.80, do all UI trigger
// must put the flow before checking setup mode-> leave main loop.
// for UI gerenal trigger/ time out.(But don't include display time)
    if ( (OS_GetSysTimer () > _dwTimeCount_OSDTrigger) )
    {
        // Micky0.72, modify from 100ms to 50ms.
        _dwTimeCount_OSDTrigger += COUNT_50_MSEC;
        OSD_Trigger( OSD_TRIGGER_CONTROL, 0xFFFF);  
    }

// LLY1.20, remove this procedure into _MM_MotionTrigger() case MM_STAGE_PLAYEND
#if 0
//Alan1.10, for char-based subpicture
#ifdef SUPPORT_CHAR_SUBPICTURE
    if (ATTR_AVI == __bAttrPlay && MODE_PLAYMOTION == __bModePlay)
    CHAR_SP_Trigger();
#endif
#endif // #if 0

/// End Of Section(5)  : Mode Trigger

/// ******  DVD Channel Sections Only Begin  ********************
/// Below Flow are necessary only for DVD Channel mode
/// !!!     Only can add the DVD Channel related
/// !!!     control flow within this block
/// Comments:
///     - For SETUP, it only needs to monitor screen saver.
// ==============================================================
/// Section[6]  : Event monitor
/// 6.1) Screen Saver
/// 6.2) Auto Vocal Detection
// ==============================================================

    DISP_MonitorVBICtrl();

// SETUP related codes must be put before Section 6.2.
// Screen saver's behavior was done within MONITOR_ScreenSaver.
/// End Of SETUP related codes.
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
    MEDIA_MonitorTOPOPEN();
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p

    // elmer2.36, fix usb in setup mode
    MEDIA_MonitorStatus();
/// *** For SETUP mode, Section[6.2]~[9] are skipped.
    if ( __bSetupMode )
        goto MAIN_LOOP_END;                

/// 6.2) Auto Vocal Detection
// check the input of Mic. for autovocal.
// DVD_172Micky, FLOW_CONTROL begin
// can't put betwwen __bISRKey != KEY_NO_KEY & __bKey != KEY_NO_KEY
// put it before ProceedFlowControl,
// if vocal is detected(_bISRKey=KEY_VOCAL), will do KEY_VOCAL first then ProceedFlowState
// if vocal key is not detected, will do ProceedFlowState directly.
// so, it's safe.
    // This part is for Auto-Vocal detection
#ifdef  ENABLE_AUTOVOCAL
    _CC_AutoVocal( NULL_0 ); // Brian.170, for SW MIC detection
#endif  // ENABLE_AUTOVOCAL
// DVD_172Micky, FLOW_CONTROL end
/// End Of Section[6]  : Event monitor

// ==============================================================
/// Section[7]  : The DVD Channel Flow Trigger
/// 7.1) Flow State Control
/// 7.2) Disc Flow Trigger
// ==============================================================
/// 7.1) Flow State Control
// Do flow trigger after title detction.
    // When FlowState need to be release, 1) no input keys && 2) non-input modes
    // then can do _ProceedFlowState.
    // LLY0.62, need to check F/W key input or not
    // Otherwise, __bFWKey maybe overwitten within _ProceedFlowState() for auto play or power on resume 
    if ( (__bFlowState & FLOW_TRIGGER) && !__bSetupMode && (__bISRKey == KEY_NO_KEY) 
          && (__bFWKey == KEY_NO_KEY) )
    {
        _ProceedFlowState();
    }
/// 7.2) Disc Flow Trigger

    __bTemp= LOBYTE ( __wDiscType ) & 0x0f;
#ifndef NO_DISC_MODE //CoCo2.37p    
    // ** DVD-TCH0.20;
    if ( __wDiscType & BOOK_DVD )
    {
#ifndef NO_DIGEST
#ifndef NO_DVDDIGEST
        unverified. // Micky0.80
        // wyc.100
        if (__btDIGEST)
        {
            if (__bISRKey!=KEY_PLAY)
            {
                DIGEST_Trigger();
            }
        }
        else
#endif // NO_DVDDIGEST
#endif
        {
            NV_Trigger ();      // ** TCH0.25;
            if ( __btPlaying )
            {
                NV_Trigger ();
            }
        }
#ifndef NO_DVDDIGEST
    // wyc.100 , prepare DVD digest, because when execute DVD digest from stop mode,
    // in some titles will have some problems, so DIGEST_PreDVDDigest is responsible
    // for entering playing mode first then execute digest command.
#ifdef PRE_DVDDIGEST
    DIGEST_PreDVDDigest();
#endif // PRE_DVDDIGEST
#endif // NO_DIGEST
    }
    else
    // ** DVD-TCH0.20;
#endif //#ifndef NO_DISC_MODE //CoCo2.37p    

#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
    if (__bThumbnailMode)
        THUMB_Trigger(); //CoCo.jpeg
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI

    switch ( __bTemp )
    {
#ifndef NO_MP3
        case    BOOK_CDROM:
                //alex080, support cdrom auto upgrade
                /* Chuan1.05, Remove AutoUpgrade flow. It will do at infofilter
                if (__bAUTOUPGD)
                {   
                    AUTOUPGD_Upgrade(AUTOUPGD_SOURCE_CDIF);                         
                    break;      
                }
                */
                
                // LLY.274p-1, Check if playable CDROM title
                if ( (__wDiscType & CDROM_M1) )
                    MM_Trigger();

                if (__bThumbnailMode)
                    THUMB_Trigger(); //CoCo.jpeg
                 break;
#endif
        case    BOOK_CDDA:
        case    BOOK_CDI:
        case    BOOK_VCD:
#ifndef NO_DISC_MODE //++CoCo2.37p
                if ( __btPBC )
                {
                    if ( ! (__wStatus & ID_ENDLIST ) )
                    {
                        __wStatus= VCD20_Trigger ();
                        if ( __wStatus & ID_ENDLIST )
                        {   // Enter END_LIST, Show LOGO and CDROM Stop
                            OSD_OUTPUT_MACRO ( MSG_ENDLIST, NULL_0, NULL_0 );
                            // DVD_101bMicky, fix VCD 2.0 pause/slow-> enter end list
                            // the OSD still don't update??
                            __bFWKey = KEY_STOP;
                        }
                    }
                }
                else
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
                {
#ifndef NO_DIGEST
                    // DIGEST mode only calls Trigger Function
                    DIGEST_Trigger(); //jyliu.pc908
#endif
                    LINEAR_Trigger ();
                    // wyc1.11-909
#ifndef NO_CDG
                    CDG_Trigger(); //LJY0.95, for CDG trigger
#endif //
                }
                break;

        default:
            break;
    }
/// End Of Section[7]  : The DVD Channel Flow Trigger

// ==============================================================
/// Section[8]  : Monitor the System status
/// 8.1) Monitor The System Hang
/// 8.2) Monitor The Time Update
/// 8.3) Monitor The Buffer Control
/// 8.4) Monitor The Bitstream Info.
// ==============================================================
/// 8.1) Monitor The System Hang
    // wyc0.95, rename to MONITOR_RecoverSystem.
    MONITOR_RecoverSystem();

// Micky0.90, fix OSD message won't be cleared when time out if tray is opened
// 8.2 is necessary for OSD_TRIGGER_MACRO to clear time out message
/// 8.2) Monitor The Time Update
    // LLY2.36a, don't update OSD time here while SCAN mode
    // Since, current procedure will break SCAN waiting period to let main flow can press input key
    // So, if go here, OSD time maybe updated during this period.
    // In other ways, OSD time only updated while I frame decoded done during SCAN mode.
    if(__bModeCmd != KEY_SCB && __bModeCmd != KEY_SCF)
    {
    UTL_TriggerShowTime ();  // ** TCH0.99-1;
    }

// Micky0.86, fix the problem no disc or open will show cmd_ack_err.(UTL_OutputShowTime-> SrcFilter_GetCurrentPos)
// Section 8.3/8.4 are not necessary when no disc & illegal disc. 
// 8.1 is necessary for auto power down/ screen saver.../ close tray(service_420)??
if ( (__wDiscType != NO_DISC_IN_DRIVE)  &&  !(__bDiscInfo & DISCINFO_UNPLAYABLE) )
{
/// 8.3) Monitor The Buffer Control
// Micky2.80, enable buffer control for DIGEST.
    if ( __btPlaying || __btNewTime 
// Micky1.20, 4M flash
#ifndef NO_DIGEST
    || (__btDIGEST && (!__bSetupMode)) 
#endif
    )
    {
        MONITOR_CheckBuffer ();
    }   // __btPlaying

/// 8.4) Monitor The Bitstream Info.
    // ** TCH0.99-1; begin...
    // every 1 second to on/off the message for encrypt
    if ( (OS_GetSysTimer () > _dwTimerCount_MainLoop) )
    {
        // Move from original Timer. Monitor Video or Audio
        MONITOR_VideoStream();
        MONITOR_AudioStream();
        // wyc2.22-909s,
        _dwTimerCount_MainLoop = OS_GetSysTimer() + COUNT_100_MSEC;
    }
    // ** TCH0.99-1; end...
}
/// End Of Section[8]  : Monitor the System status
// ==============================================================
/// Section[9]  : Monitor The Tray Status
/// 9.1) Check Tray Open by extra force(such as Eject Button)
/// 9.2) Check Tray was really closed(By IR Key or extra force)
// ==============================================================
    // Micky1.00_909
    //MEDIA_MonitorStatus();

/// End Of Section[9]  : Monitor The Tray Status
/// ******  DVD Channel Sections Only End  *********************

MAIN_LOOP_END:
    // Micky0.67-909. release the time.
    // if necessary, may check if nothing to do then release.
    OS_YieldThread();
    // continue;   // continue the while loop.
    }    
}

#ifdef SUPPORT_STB
void CC_STB_MainLoop (void)
{
    while ( 1 )
    {
    // elmer2.37, move watchdog mechanism form here to alarm
#ifdef SUPPORT_WATCH_DOG
        REG_SRAM_WATCHDOG++;
#endif
        //MONITOR_CheckWatchDog();
        DBG_Polling();
        AP_MainLoop();
        if( __bChooseMedia != MEDIA_SELECT_STB)
        {
            break;
        }
    }     
}
#endif

void    Thread_CTKDVD (DWORD index)
{

    // Brian.276-5, for system hang control 
    // initialize the progress count to 0
    _dwTimerCount_MainLoop=0;

// wyc1.25, add one variable to record flow when detected as wrong disc for debug usage,
    __bTestFlow = 0;

// ==============================================================
/// Power On Initialization
/// Only need do it once for System.
// ==============================================================
#ifdef SUPPORT_POWER_SWITCH    //process power Switch
   
    __bISRKey = PANEL_KeyScan(); //FETCH POWER_SWITCH CONDITION
    if (__bISRKey == KEY_PICTURE)
    {
        __bPowerSw_StandBy = TRUE; //run STANDBY
        __bPowerSw = TRUE; //Disable IR KEY_POWER
    }
    else
    {
        __bPowerSw_StandBy = FALSE;       
    }
    __bISRKey=KEY_NO_KEY;
#endif //SUPPORT_POWER_SWITCH

    INITIAL_System(INITIAL_SYSTEM | HARDWARE_SHUTDOWN);

    // merge I/R power on sequence with h/w power on.
    INITIAL_PowerONStatus(INITIAL_POWER_HARDWARE);
            
#ifdef SUPPORT_POWERON_MENU //CoCo2.38

#ifndef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
    POWERONMENU_Initial(); 
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON

    // elmer2.75, initialize USB and card-reader after POWERONMENU pops out
#ifdef CTK_CARDREADER
    {
        extern BOOL CARD_SetupSystem(DWORD);

        CARD_SetupSystem(DECODER_SYSTEM);
    }
#endif
#ifndef POWERON_STANDBY
#if (USB_SYSTEM == USB2_0_JUNGO)
    {
#ifdef SUPPORT_USB_SWITCH
        HAL_WriteGPIO(GPIO_A, 23, 0);
#endif
        // wyc2.78-909P, always resume the usbsrc thread because usbsrc won't go when no commands is issued.
        {
            extern DWORD _dwMediaUSBOTGChkTime;
#ifdef SUPPORT_USB_POWER_BY_IO
#ifdef SUPPORT_USB_HOST_DEVICE
            HAL_WriteGPIO(USB_POWER_GRP, PIN_USB_POWER, 0);
#else
            HAL_WriteGPIO(USB_POWER_GRP, PIN_USB_POWER, 1);
#endif //
#endif //
            USBSRC_ThreadResume();
            _dwMediaUSBOTGChkTime = OS_GetSysTimer();
        }
    }
#endif
#endif // POWERON_STANDBY

#endif //#ifdef SUPPORT_POWERON_MENU

#ifdef LED_BackLight //CoCo2.38          
#ifdef SUPPORT_POWER_SWITCH
    if (__bPowerSw_StandBy)
    {
         _bBacklight = 1;
        TFT_FreeRun(TRUE);
        TFT_BackLight(FALSE);
    }
    else
        TFT_BackLight(TRUE);
#else
    TFT_BackLight(TRUE);
#endif //SUPPORT_POWER_SWITCH
#endif //LED_BackLight

#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
    POWERONMENU_PowerOnPlayMediaDirectly();
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON

//------------------------STB testing code---START---------------------------------------------------
#ifdef SUPPORT_STB
#ifndef IMAGE_FRAME_SETUP //CoCo2.38
    MEDIA_DecidetMedia();
#endif //IMAGE_FRAME_SETUP
    while( 1 )
    {
        // The main monitor loop in player.
        // Program will always run in this loop except real Power-ON.

        if( __bChooseMedia == MEDIA_SELECT_STB)
        {
            STB_Entry();
            CC_STB_MainLoop();
        }
        else
        {
            CC_DVD_MainLoop();
        }
    }    
#else
        CC_DVD_MainLoop();
#endif
}   // main ()

//  *********************************************************************
//  Function    :   CC_KeyCommand
//  Description :   Process KEY_PLAY, KEY_STOP, KEY_FF, KEY_SF, KEY_SCF, KEY_SCB
//                          KEY_STEP, KEY_FREEZE
//  Arguments   :   bKey    : The key definition
//  Return      :   TRUE only. ???
//  Side Effect :   It'll record __bModeCmd and open __btPlayEnd
//  *********************************************************************
BYTE CC_KeyCommand ( BYTE bKey )
{
    BYTE        bKeyPrevious;
    BYTE        bTemp;

    // Only fast/npf mode can't toggle?
    if ( ( (bKey==KEY_FF) || (bKey==KEY_PLAY)) && (__bModeCmd==bKey) && (!__btPause) )
    {
        // Micky0.80, just return invalid key.
        /*
        OSD_Output(MSG_INVALID, OSD_ERR_KEY, 1);
        return TRUE;
        */
        return INVALID_KEY;
    }

    bKeyPrevious= __bModeCmd;

    // optimize codes for step/fast/scan/slow/freeze
    // For DVD, must query UOP
    // For other, query still/ CD-DA UOP
    if ( ( bKey != KEY_PLAY ) && ( bKey != KEY_STOP ) )
    {
#ifndef NO_DISC_MODE //CoCo2.37p
        if (__wDiscType & BOOK_DVD)
        {
            // DVD_051Micky, treat KEY_FF as KEY_SCF
            // must check if allow Scan
            if ( (bKey == KEY_SCF) ||
                 (bKey == KEY_FF) )
            {
                if ( ! NV_QueryUOP (UOP_CAN_FORWARD_SCAN) )
                    return INVALID_KEY;
            }
            else if (bKey == KEY_SCB)
            {
                if (! NV_QueryUOP(UOP_CAN_BACKWARD_SCAN))
                    return INVALID_KEY;
            }
            //kevin1.11, fix Play -> Slow %75 -> Slow %50 => unsmooth audio for a while (change sampling freq should be within the period of PCM scale=0)
            //kevin1.20, fade out on discont. points
            //wyc2.17-909S, support another key to enable language study function.
            if (bKey==KEY_FF || bKey == KEY_SCF || bKey == KEY_SCB || bKey == KEY_SF || bKey == KEY_LAN_STUDY)
            {
                CHIPS_OpenAudio( FALSE );                                
            }               
        }
        else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        // Micky0.67-909
        {   // non-DVD
        bTemp= UTL_CheckPlayItemMode ( __wPlayItem );

        // STILL mode or CDDA mode don't allow scan
        if ((bTemp & MODE_STILL ) || (__bAttrPlayNew==ATTR_CDDA))
        {
            // LLY.274, check playing mode has updated
            if( ((bKey==KEY_SCF) || (bKey==KEY_SCB)) && (__bAttrPlayNew==ATTR_CDDA) )
            {
                __bScan=SCAN_NONE;
            }
            else
            {
                OSD_Output(MSG_INVALID, OSD_ERR_KEY, 1);
                return TRUE;
           }
        }
        else
        {
// Micky1.06, KEY_FF don't have audio, must mute too.
//            if ( ( bKey != KEY_FF ) && ( bKey != KEY_FREEZE ) )   //fast has audio
            if (  bKey != KEY_FREEZE  )
                CHIPS_OpenAudio( FALSE );
        }
        }   // non-DVD
    }   // The ModeCmd except PLAY and STOP
// WinVCD
    // must release the auto pause state
#ifndef NO_DISC_MODE //++CoCo2.37p
    {
        extern BYTE __bVCD20Status;
        __bVCD20Status=VCD20_NORMAL;
    }
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p

    // Micky1.07, fix DVD/ CVD-> freeze-> subpicture still update
    // only KEY_PLAY can enter KEY_FREEZE mode.
    // But FREEZE-> can switch to SCF/SCB/SMF.
    // enter freeze, set subpicture off
    // check enter freeze mode
    if ( bKey == KEY_FREEZE)
    {
        if (__bModeCmd == KEY_FREEZE)
        {
            if (__btPlaying)    // freeze-> freeze, leave freeze
            {
                HAL_Display(HAL_DISPLAY_SPST1, TRUE) ;
                HAL_Display(HAL_DISPLAY_SPST2, TRUE) ;  // Chuan0.75, Give the same behavior
            }
            else                // freeze-> pause-> freeze, stay freeze
            {
                // do nothing
            }
        }
        else    // play-> freeze, enter freeze
        {
            HAL_Display(HAL_DISPLAY_SPST1, FALSE) ;
            HAL_Display(HAL_DISPLAY_SPST2, FALSE) ;  // Chuan0.75, Give the same behavior
            
        }

    }
    // check leave freeze mode
    else if ( __bModeCmd == KEY_FREEZE )
    {
        HAL_Display(HAL_DISPLAY_SPST1, TRUE) ;
        HAL_Display(HAL_DISPLAY_SPST2, TRUE) ;  // Chuan0.75, Give the same behavior
    }

    // Ming0.68-909, Disable sub-picture on SCF, SCB, STOP mode.
    if( (bKey == KEY_SCF) || (bKey == KEY_SCB) )
    {
        if((bKeyPrevious != KEY_SCF) &&
           (bKeyPrevious != KEY_SCB) )
        {
            
            // enter SCF or SCB mode
            HAL_Display(HAL_DISPLAY_SPST1, FALSE);
            HAL_Display(HAL_DISPLAY_SPST2, FALSE);
            PARSER_ActionFilter( 0, PARSER_ENABLE_SP1, FALSE );
            // Micky0.76, reset SP2                                    
            PARSER_ActionFilter( 0, PARSER_ENABLE_SP2, FALSE );
            // Chuan0.80, Pass __dwSPType to SPU_Reset() due the API changed.
            // wyc2.04-909S, replace follow code by HAL_Reset(HAL_RESET_SPST).
            HAL_Reset(HAL_RESET_SPST);
            /*SPU_Reset(__dwSPType);
            PARSER_BufferReset(PARSER_ES_SP1);
            // Micky0.75, reset SP2                        
            PARSER_BufferReset(PARSER_ES_SP2);          

//Alan1.10, for char-based subpicture
#ifdef SUPPORT_CHAR_SUBPICTURE
            CHAR_SP_Reset();
#endif
            */

            // Micky0.70, must reset audio before enter scf/scb
            // Micky0.72, audio only don't need action
            // LLY1.01, don't throw Audio if video stream has been threw.
            if( (__bModePlay == MODE_PLAYMOTION) && (__bVideoID!=HAL_THROW_VIDEO) )
            {
                HAL_ControlParser(HAL_PARSER_AUDIO_ID, HAL_THROW_AUDIO);
                HAL_Reset(HAL_RESET_AUDIO);
            }
        }
    }
    else if((bKeyPrevious == KEY_SCF) ||
            (bKeyPrevious == KEY_SCB) )

    {
        // leave SCF or SCB mode    
        // wyc0.95, only enable SP when SCF/SCB -> play when SP is on before entering SCF/SCB mode.
        if ((__bSP_Disp_OFF == 0) || (__bSP_Disp_OFF&0x80))
        {
            HAL_Display(HAL_DISPLAY_SPST1, TRUE);
            // Micky0.76, suppose don't have any SP data now. don't reset.
            /*
            SPU_Reset(__dwSPType);
            PARSER_BufferReset(PARSER_ES_SP1);
            // Micky0.75, enable SP2
            PARSER_BufferReset(PARSER_ES_SP2);        
            */
        }
        // wyc1.07-909, move outside the if statement to fix SP off -> enter scan mode -> normal play -> enable SP but SP will show with little delay.
        PARSER_ActionFilter( 0, PARSER_ENABLE_SP1, TRUE );
        // Micky0.76, enable SP2                                    
        if (__bSPST2ID!= 0xFF)
        {
            // Micky0.75, enable SP2
            if ((__bSP2_Disp_OFF == 0) || (__bSP2_Disp_OFF&0x80))
            {
                HAL_Display(HAL_DISPLAY_SPST2, TRUE);
            }
            PARSER_ActionFilter( 0, PARSER_ENABLE_SP2, TRUE );
        }
    }

    // Micky0.70, modify macro commnad A/V sync control, now is audio master
    // bitstream dis-continue, must reset audio
    // switch to the mode that has audio, must set audio master
    // Notice: nothing need to do for audio only playback mode -- LLY0.91
    // LLY1.01, don't Reset Audio if video stream has been threw.
    if( (__bModePlay != MODE_PLAYAUDIO) && (__bVideoID!=HAL_THROW_VIDEO) )
    {
        // wyc2.17-909S, add key for language study function.
        if ( ( bKey == KEY_PLAY) || ( bKey == KEY_LAN_STUDY) || (bKey == KEY_SF) || (bKey == KEY_STEP) || (bKey == KEY_FF) || (bKey == KEY_SCF) || (bKey == KEY_SCB) || (bKey == KEY_PAUSE))
        {
            if ( (__bModeCmd == KEY_STOP) || __btNewTime )
            {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
                if (!__bMMJPEGAudioMode)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
                {
                    HAL_Reset(HAL_RESET_AUDIO);
                }
                    // wyc1.10-909, by KC's updated code and need to reset video also, otherwise A/V will async sometimes and audio will mute when async.
                    HAL_Reset(HAL_RESET_VIDEO);
            }
            else if ((__bModeCmd == KEY_SCF) || (__bModeCmd == KEY_SCB))
            {
                if ((bKey != KEY_SCF) && (bKey != KEY_SCB))
                {
                    HAL_Reset(HAL_RESET_AUDIO);
                    // wyc1.10-909, by KC's updated code and need to reset video also, otherwise A/V will async sometimes and audio will mute when async.
                    HAL_Reset(HAL_RESET_VIDEO);
                    // HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, 0 );
                }
            }
            
            // Chuan0.80, FF/ FF -> Play/Freeze/Slow/Step/ SCAN -> Play/Freeze/Slow/Step will do VideoMaster..
            // Scan/ fast mode-> bitstream continue play mode, must set video master
            // Gotime/ repeat AB/ resume, must se video master
            // fast always do video master
            if ((__bModeCmd == KEY_PLAY) || (__bModeCmd == KEY_SF) || (__bModeCmd == KEY_STEP) || (__bModeCmd == KEY_FF) || (__bModeCmd == KEY_SCF) || (__bModeCmd == KEY_SCB) || (__btNewTime))
            {
                if ((__bModeCmd != KEY_PLAY) || (( bKey != KEY_LAN_STUDY) && (bKey != KEY_PAUSE) && ((__btPause != TRUE) || (bKey!=KEY_PLAY))))
                {
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"\nIssue Video Master");
                    // wyc2.36a-909s, add mpeg-cd code
#ifdef SUPPORT_FUSS_MPEG_CD
                    if (INFOFILTER_FUSSMPEGCD(IF_MODE_CHECK_MPEG_CD,0))
                        HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, FALSE);
                    else
                        HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, TRUE);
#else
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
                    if (!__bMMJPEGAudioMode)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
                        HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, 1 );
#endif //
                }
            }
        }
    }

    // wyc1.06-909, when pause -> play, need to issus seek command wake up SERVO if SERVO stopped. UTL_PreStartServo will issue seek ccommand
    // if SERVO stopped.
    // wyc1.08, modify __bKey checking by bKey to fix SETUP mode -> KEY_SETUP -> SERVO won't spin problem.
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
    if (((__btPause) && ((bKey == KEY_PLAY) || (bKey == KEY_SF) || (bKey == KEY_PAUSE) || (bKey == KEY_STEP) || (bKey == KEY_LAN_STUDY))) || ((__bModeCmd == KEY_STEP) && ((bKey == KEY_PLAY) || (bKey == KEY_SF))))
    {
        UTL_PreStartServo(__dwTimeNow,ACTION_CONTINUE);
    }   
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p

    //DVD_040-1
    __bModeCmd= bKey;
switch ( bKey )
{
    case    KEY_PLAY:
        // scan forward->black->play will have no video display
        // wyc2.04-909S, replace by DISP_DisplayCtrl
        if ( (bKeyPrevious == KEY_SCF ) && (__bFirstPlay ) )
            DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
        
        //kevin1.11, fix Slow %50 -> Play => unsmooth audio for a while (change sampling freq should be within the period of PCM scale=0)
        // wyc2.17-909S, for language study
        if (( bKeyPrevious == KEY_SF ) || (bKeyPrevious == KEY_LAN_STUDY))
            CHIPS_OpenAudio( FALSE );  
                        
        // Micky0.80a, fix reepat ab-> slow->play, the repeat ab OSD won't recover            
        if (__bKeyMessage == MSG_PLAYING)
        {
            OSD_OUTPUT_MACRO ( MSG_PLAYING, NULL_0, 3 );
        }
        else
        {
            // wyc1.06-909, when scan -> play in AVI mode, we will call _CC_AVIScanToPlayProcess to show "play" OSD. But __bKeyMessage
            // will be clear to 0 after _CC_MapKeyMessage. So we will see OSD "play" show about 0.5s then clear by follow code. So
            // we add one variable to not clear "play" OSD here if it flow is AVI scan to play to enhance this problem.
            if (_bAVIScan2Play == TRUE)
            {
                _bAVIScan2Play = FALSE;
            }
            else
            {
                OSD_OUTPUT_MACRO ( MSG_PLAYING, NULL_0, 0 );        
            }
        }
        /*
         //CoCo0.72, support JPEG Preview
        // wyc0.80, change BYTE declare to DWORD declare.
        if (!__dwMMJPEGPreview)
        {
            OSD_OUTPUT_MACRO ( MSG_PLAYING, NULL_0, 3 );
        }
        */
        // Micky0.67-909, step->play, just issue command
        // decoder will write vpts to stc always, must test VOBU_E_PTM case
        // Read back the recent STC value
        if (__bModeCmd != KEY_STOP )
        {
            HAL_ReadInfo(HAL_INFO_STC, &__dwSTCValue);
        }
        // clear Scan state
        __bScan=SCAN_NONE;

        // Micky0.72, if playing, only change macro command
        if (__btNewTime || (!__btPlaying && !__btPause) )
        {
            _Get_PlayRange ();
        }
        else
        {
            UTL_TransferCommandID(__bModeCmd);
            // Micky0.72, must be done first, as CHIPS_OpenAudio will reference it.
            __btPlaying = TRUE;
            __btPause=FALSE;
        }


        if ( ! __btMute )
            CHIPS_OpenAudio( TRUE );
        break;

    case    KEY_STOP:
        __btNewTime=FALSE;

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__bMMJPEGAudioMode == FALSE)
#endif //
            CHIPS_OpenAudio( FALSE );
            // wyc2.77-909p, remove it be KC's suggestion for macrovision enable/disable will make video shake problem.
            // HAL_TVSetAPS ( NULL_0 );


        // LLY0.95, support stop --> resume feature for 
        // [1] CDROM motion file
        // [2] DVD/ VCD/ CDDA/ CVD/ SVCD title
        // So, keep the resume time while pre-stop mode and this titles
        // Notice: DVD resume time calculation method is different.
        // wyc1.06-909, can't set __dwResumeTime when JPG preview mode, otherwise MM-motion can't resume from resume time if cursor stay in JPG preview file.
        // It is because preview will issue CC_KeyCommand(KEY_STOP) then play, so when stop from mm-motion file and move cursor to JPG file 
        // to preview mode, then it will re-set __dwResumeTime and it is the reason mm-motion can't resume from resume time.
        if( (__bCountStop==1) && !(__wDiscType&BOOK_DVD) && (__dwMMJPEGPreview == FALSE) )
        {
            // LLY2.10-1, Keep resume directory & item together for CDROM/ DVDROM title
            if(__wDiscType & CDROM_M1)
            {                
                __dwResumeItem = MAKELONG(__wPlayItem, __MMPlayingItem.wDirID);
            }

            
            if(__bAttrPlay == ATTR_AVI)
            {
                // LLY1.01, keep current PTS value for DivX 
                // Since, we must give time for parser to get the corresponding sector 
                // (base on index table)
                HAL_ReadInfo(HAL_INFO_STC, &__dwTemp);
                __dwResumeTime=__dwTemp/90000;
                // LLY2.36, keep corresping STC value here to avoid STC/90K= sec, then *90K=STC again
                __dwGoTimeSTC = __dwTemp;

                //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Stop STC: %lx, Time: %lx\n", __dwTemp, __dwResumeTime);

            }
            // LLY2.05, alwasy keep decoded sector as resume time
            // for CDROM & DVDROM 's AV or audo file, or SVCD/ CVD/ VCD/ CDDA title
            else // if( (__bAttrPlay&TYPE_CDROM_AV) || !(__wDiscType & CDROM_M1) )
            {
                // LLY2.35, must keep stop time (in second unit) as resume time for WMA
                // Since, we must give time for parser to get the corresponding sector
                // So, must re-calculate it again.
                if(__bAttrPlay == ATTR_WMA)
                {
                    // Must /75 since _UTL_CalTime_FromFrameCnt() return value is second*75
                    __dwResumeTime = _UTL_CalTime_FromFrameCnt()/75;
                }
                else // others will keep current decoded sector
                {
                    // Call API to get current decoded time.
                    __dwResumeTime = UTL_GetDecodedTime();
                }

                // Also calculate the decoded frame count for CDROM/ DVDROM audio only file
                if(__bAttrPlay & TYPE_CDROM_AUDIO)
                {
                    // LLY2.35, get decoded frame count from DSP directly
                    //__dwGoTimeFrameCnt = UTL_SectorsToFrameCnt(__dwResumeTime-__dwTimeBegin);
                    HAL_ReadAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, &__dwGoTimeFrameCnt);
                }

//                printf("Stop position: %lx, Frame Cnt: %lx\n", __dwResumeTime, __dwGoTimeFrameCnt);
            }
        }
        // Micky0.65
        // must stop source and parser and decoder
        HAL_PlayCommand(COMMAND_STOP, 0);

        // wyc2.53-909P,
        SrcFilter_Stop(__dwSFStreamID);
        
        // Micky0.68, must stop parser after UTL_GetDecodedTime()
        // once parse stop, the position will be reset.
        HAL_ControlParser(HAL_PARSER_STOP, 0);

        // Micky0.70, add A/V sync control
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__bMMJPEGAudioMode == FALSE)
#endif //
            HAL_Reset(HAL_RESET_AUDIO);
        //HAL_WriteAM(HAL_AM_PLAY_COMMAND, 0); //kevin1.20, redundant
        
        
        // Micky0.72, clear subpicture for all disc
        HAL_Reset(HAL_RESET_SPST);
        
        __bScan= SCAN_NONE;

//LJY1.24a, use NO_CDG to determine whether CDG is supported.
#ifndef NO_CDG 
        // must clear CDG buffer when changing track.
        //LJY1.10, clear CDG buffer 
        if(__bCDDA_DTS & CDDA_CDG)
        {
            if(__bCountStop != 1) //pre-stop don't clear CDG buffer
            {
                HAL_Reset(HAL_RESET_AUDIO);                
            }
        }
#endif

        // LLY.274p-1, must check more because it's unnecessary to
        // do re-initial for JPEG playing ???
        // LJY1.10, not to do re-initial for both JPEG & CDDA
        // so CD(DTS/G) won't get CDDA format track by track
        if( (__bAttrPlay!=ATTR_JPG) && (__bAttrPlay!=ATTR_CDDA) )
        {
            // For stop, goto time to CDDA
            __bModePlay= MODE_PLAYUNKNOW;
        }

        // !!!must clear the pause state
        // or pause->next/key_number will play current track!
        __btPause=FALSE;
        __btPlaying= FALSE;
        __dwTimeNow= __dwTimeBegin;     // reseek position to start sector
        // wyc1.21-909, need to initial it, otherwise the state is not correct
        __bPBReady= TRUE;
        __bMotionState=MOTION_STATE_NONE;

#ifndef NO_DISC_MODE //++CoCo2.37p
        if ( __btPBC )
        {
            __wStatus= ID_ENDLIST;      //force enter EndList Mode
            OSD_OUTPUT_MACRO ( MSG_SELECTIONLIST, 0xFFFF,  0 );
            // for not re-draw LOGO, and it has no need
        }
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
        break;

    case    KEY_FREEZE:
        //only support in normal play mode
        //freeze->pause->pause, btPlaying=FALSE
        //freeze->freeze, btPlaying=TRUE
        //pause->freeze,
#ifdef SUPPORT_FREEZE_FUNCTION
        __bScan= SCAN_NONE;

        if ( bKeyPrevious == KEY_FREEZE )   // as __bModeCmd is updated as new
        {
            if (__btPause)  //freeze->pause->freeze
            {
                // currentlt command state still is FAST
                OSD_OUTPUT_MACRO(MSG_FREEZE, TRUE, 0xff);
                //re-open CD-IF without sending command
                // Micky0.67-909, don't need buffer control             
                //UTL_PlayFromTime(__dwTimeNow);
                __btPause=FALSE;
                __btPlaying=TRUE;
                // Micky085, fix Freeze->pause->pause hang
                HAL_PlayCommand(COMMAND_FREEZE, 0);

            }
            else
            //freeze->unfreeze
            {
                __bModeCmd=KEY_PLAY;
                OSD_OUTPUT_MACRO(MSG_PLAYING, FALSE, 0x3);
                HAL_PlayCommand(COMMAND_PLAY, 0 );
            }
            CHIPS_OpenAudio( TRUE );
        }
        else
        {
// Micky1.21, CodeReview --- remove
// only KEY_PLAY will enter KEY_FREEZE(_KeyGroupPlay already do the check)
            //only allow freeze in KEY_PLAY mode
            OSD_OUTPUT_MACRO ( MSG_FREEZE, TRUE, 0xff );
            HAL_PlayCommand(COMMAND_FREEZE, 0);
        }
#endif //
        break;

    case    KEY_STEP:
        OSD_OUTPUT_MACRO ( MSG_STEP, NULL_0, 0xff );

        // Micky0.67-909, don't need buffer control for step
        // mpeg decoder will inform DSP to skip audio frames by picture rate.
        // only need to issue decoder command step + DSP command pause.
        __btPlaying = TRUE;
        
        UTL_TransferCommandID(__bModeCmd);
        
        __btPause = FALSE;
        __bScan= SCAN_NONE;
        break;
 
    break;

    case    KEY_SF:
    // wyc2.17-909S, for language study.
    case    KEY_LAN_STUDY:
        //smf->smf, only toggle slow speed
        if ((((bKey==KEY_SF) && ( bKeyPrevious==KEY_SF )) || ((bKey==KEY_LAN_STUDY) && ( bKeyPrevious==KEY_LAN_STUDY ) )) && !__btNewTime)
        {
            if ( ! __btPause )
            {
                __bSlowLevel++;
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
                if (bKey == KEY_LAN_STUDY)
                {
                    if (__bSlowLevel > MAX_LAN_STUDY_SLOW_LEVEL)
                        __bSlowLevel=1;
                }
                else
#endif //
                {
                    if (__bSlowLevel > MAX_SLOW_LEVEL)
                        __bSlowLevel=1;
                }
                __wSlowSpeed=2;
                for (__bTemp=1;__bTemp < __bSlowLevel;__bTemp++)
                {
                    __wSlowSpeed*=2;
                }
            }
            else
            {
                __btPlaying=TRUE;
                __btPause=FALSE;
            }

            // only change the slow speed argument
            // Micky0.66-909, use UTL_TransferCommandID
            // wyc2.17-909S, update to input bKey directly and move it to before calling MSG_SLOW because OSD need to reference __bLangStudyFlag variable.
            UTL_TransferCommandID(bKey);
            
            // wyc1.06-909, when active message is SLOW, then show SLOW message.
            if (__bActiveMessage == MSG_SLOW)
            {
                OSD_OUTPUT_MACRO ( MSG_SLOW, (WORD)__bSlowLevel, 0xff );
            }


            break;
        }

// Micky0.67-909, don't issue range within CC_KeyCommand
#ifndef NEW_PLAY_ITEM   
        // Micky0.72, if playing, only change macro command
        if (__btNewTime || (!__btPlaying && !__btPause) )
        {
            _Get_PlayRange ();
        }

#endif  //#ifndef   NEW_PLAY_ITEM   
        // wyc1.06-909, need to set to FALSE because can enter SF mode from pause mode. Otherwise UTL won't update time OSD because it
        // will reference __btPause to decide update time OSD or not.
        __btPause = FALSE;
        // wyc2.17-909S, issue command before OSD in SLOW mode because OSD will reference variable __bLangStudyFlag.
        // wyc1.06-909, when active message is SLOW, then show SLOW message.
        UTL_TransferCommandID(__bModeCmd);
        if (__bActiveMessage == MSG_SLOW)
        {
            OSD_OUTPUT_MACRO ( MSG_SLOW, __bSlowLevel, 0xff );
        }

        __bScan= SCAN_NONE;
        __btPlaying= TRUE;        
        break;

    case    KEY_FF:
// Micky0.70, audio data is continus in 909
/*      
        // Micky1.05, fix SCF->KEY_SF->play, audio has noise
        #ifndef SUPPORT_ANTISHOCK   //LJY1.23, LJY1.11b, don't reset audio for KEY_FF to fix AV async problem in fast mode.
        HAL_Reset(HAL_RESET_AUDIO);
        #endif
*/        
        // MICKY 121296 begin
        // already set SPCR don't care audio
        // can raise chip performance
        // play->fast/stop->fast
        // must send commnad before changing speed(smf/npf->2x..)
        // Micky0.67-909. always issue command within CC_KeyCommand
        //if (__btPlaying)
        {
            UTL_TransferCommandID(__bModeCmd);
        }

// Micky0.67-909, don't issue range within CC_KeyCommand
#ifndef NEW_PLAY_ITEM   

        // 2X, fast IP, performace is not enough
        // it cause fast repeat AB will have blocks in B->A
        // Micky0.72, if playing, only change macro command
        if (__btNewTime || (!__btPlaying && !__btPause) )
        {
            _Get_PlayRange ();
        }

#endif  //#ifndef   NEW_PLAY_ITEM   
        __btPlaying= TRUE;
        // wyc1.06-909, need to set to FALSE because can enter FF mode from pause mode. Otherwise UTL won't update time OSD because it
        // will reference __btPause to decide update time OSD or not.
        __btPause = FALSE;
// DVD_099bMicky, The KEY_FF action is in SCF round when defined.
// else it will be normal KEY_FF
#ifdef  SAME_FAST_AS_SCF2
        // show the OSD as SCF
        OSD_OUTPUT_MACRO ( MSG_SCANFORWARD, __bScanLevel, 0xFF );
#else
        // show the OSD as Fast
        OSD_OUTPUT_MACRO ( MSG_FAST, NULL_0, 0xff );
#endif

        __bScan= SCAN_NONE;
        break;

    case    KEY_SCF:
    case    KEY_SCB:
// Micky0.67-909, don't issue range within CC_KeyCommand
#ifndef NEW_PLAY_ITEM   
        // Micky0.72, if playing, only change macro command
        if (__btNewTime || (!__btPlaying && !__btPause) )
        {
            _Get_PlayRange ();
        }
        else
#endif  //#ifndef   NEW_PLAY_ITEM   
        {
            // LLY1.05, Don't issue SCAN CMD for all title
            // LLY1.02, Don't need to issue SCAN command while receive SCAN key for non-DVD title
            // And, let it be handle by MONITOR_CheckBuffer()
            // Since, the two I-frame distance maybe > 10 sec,
            // So, we may not get the SCAN_DONE command for DivX while issue SCAN during file end.
            // LLY1.01, don't issue SCAN command if video stream has been threw.            
#if 0            
            if( (__wDiscType & BOOK_DVD) )
            {
                if(( __bScan != SCAN_GO )&& (!(__bAttrPlay==ATTR_CDDA)) && (__bVideoID!=HAL_THROW_VIDEO))
                {
                    UTL_TransferCommandID(__bModeCmd);
                }
            }
#endif  // #if 0            
        }

        // LLY1.02, initial the Scan status as SCAN_INIT while no scan mode --> scan mode for non-DVD title only
#if 0        
        if(__wDiscType & BOOK_DVD)
        {
            __bScan=SCAN_GO;
        }
        else
#endif  // #if 0            
        {
            if(__bScan != SCAN_GO)
            {
                __bScan=SCAN_INIT;
            }
        }
        // wyc2.35-909S, add code to issue scan command from pause mode.
        if (__btPause)
        {
            if (bKey == KEY_SCF)
                __bKeyMessage=MSG_SCANFORWARD;
            else if (bKey == KEY_SCB)
                __bKeyMessage=MSG_SCANBACKWARD;
            UTL_TransferCommandID(bKey);
        }

        __btPlaying= TRUE;
        // wyc1.06-909, need to set to FALSE because can enter scan mode from pause mode. Otherwise UTL won't update time OSD because it
        // will reference __btPause to decide update time OSD or not.
        __btPause = FALSE;

        // LLY1.05, don't keep system timer here
        // Since, __dwTimeRef must keep issue SCAN CMD time
        // And, new SCAN algorithm don't issue SCAN CMD here
        //__dwTimeRef= UTL_GetSysTimer();

        __wTemp=1;
        for (__bTemp=0;__bTemp<(__bScanLevel-1);__bTemp++)
        {
            __wTemp*=2;
        }
        if (__wDiscType & BOOK_SVCD)
        {
            __wScanGap=(WORD)__wTemp*150;
        }
        // LLY0.95, Scan Gap means the index table entry gap for DivX SCAN mode
        else if(__bAttrPlay == ATTR_AVI)
        {
            __wScanGap=__wTemp;
        }
        // LLY0.77, the sectors of per-second value maybe different between different CDROM motion file.
        // So, the scan gap value calculation maybe base on it value.
        else if(__bAttrPlay & TYPE_CDROM_AV) 
        {
            __wScanGap=(WORD)__wTemp*__dwSectorPerSec;
        }
        else
        {
            __wScanGap=(WORD)__wTemp*75;
        }

        // __bKeyMessage keep related OSD message for the key
        OSD_OUTPUT_MACRO ( __bKeyMessage, __bScanLevel, 0xFF );


        // for support CD-DA w/ audio
        if ( __wDiscType & BOOK_CDDA )
        {
            CHIPS_OpenAudio( TRUE );
        }
        break;

    default:
        break;
}
    // Micky0.90a, will be done by UTL_PlayFromTime
    //__btPlayEnd= FALSE;

    return TRUE;
}

#ifdef CARD_TEST
//  *********************************************************************
//  Function    :   CC_CardTest
//  Description :   This function will perform card test operation.
//  Arguments   :
//  Return      :   None
//  Side Effect :
//  *********************************************************************
#include "card.h"
VOID CC_CardTest(VOID)
{

    DWORD ReadTest(DWORD dwSectorUnit)
    {
        DWORD dwTime = OS_GetSysTimer();
        DWORD dwSectorCount = 0;
        DWORD dwSectorAddress = 0;

        do
        {
            if(!CARD_ReadSector(DS_SP1BUF_ADDR_ST, dwSectorAddress, dwSectorUnit))
            {
                dwSectorCount = 0;
                break;
            }
            dwSectorCount += dwSectorUnit;
            dwSectorAddress += dwSectorUnit;
        }
        while(OS_GetSysTimer() - dwTime < COUNT_1_SEC);

        {
            char msg[32];
            if(dwSectorCount != 0)
            {
                sprintf(msg, "ReadTest(%02ld): %ldMbps\n", dwSectorUnit, dwSectorCount * 512 * 8 / 1000000);
            }
            else
            {
                sprintf(msg, "ReadTest(%02ld): fail\n", dwSectorUnit);
            }
            printf(msg);
        }

        return dwSectorCount;
    }


    DWORD WriteTest(DWORD dwSectorUnit)
    {
        DWORD dwTime = OS_GetSysTimer();
        DWORD dwSectorCount = 0;
        DWORD dwSectorAddress = 0;

        do
        {
            if(!CARD_WriteSector(DS_SP1BUF_ADDR_ST, dwSectorAddress, dwSectorUnit))
            {
                dwSectorCount = 0;
                break;
            }
            dwSectorCount += dwSectorUnit;
            dwSectorAddress += dwSectorUnit;
        }
        while(OS_GetSysTimer() - dwTime < COUNT_1_SEC);

        {
            char msg[32];
            if(dwSectorCount != 0)
            {
                sprintf(msg, "WriteTest(%02ld): %ldMbps\n", dwSectorUnit, dwSectorCount * 512 * 8 / 1000000);
            }
            else
            {
                sprintf(msg, "WriteTest(%02ld): fail\n", dwSectorUnit);
            }
            printf(msg);
        }

        return dwSectorCount;
    }


    static BOOL bInit = FALSE;
    CARD_CONTROLLER CardCtrl;

    printf("-=# Card Test Begin #=-\n");

    if(!bInit)
    {
        bInit = TRUE;
        CARD_SetupSystem(DECODER_SYSTEM);
        CARD_InitSystem();
        OS_DelayTime(COUNT_1_SEC);
    }

    CardCtrl = CARD_CardStatus_Inserted();
    switch(CardCtrl)
    {
        case CARD_CONTROLLER_SD:    printf("SD/MMC Card Inserted\n");   break;
        case CARD_CONTROLLER_MS:    printf("MS Card Inserted\n");       break;
        case CARD_CONTROLLER_XD:    printf("xD Card Inserted\n");       break;
        case CARD_CONTROLLER_NONE:  printf("Card Not Inserted\n");      return;
    }

    if(!CARD_InitCard())
    {
        printf("Card Init Fail!\n");
        return;
    }

    if(CardCtrl == CARD_CONTROLLER_SD || CardCtrl == CARD_CONTROLLER_MS)
    {   // SD/MMC/MS

        if(!CARD_SetBusWidth(FALSE))
        {
            printf("Set to 1-bit mode fail\n");
            return;
        }
        printf("Test under 1 bit mode\n");
        ReadTest(4);
        ReadTest(32);
        WriteTest(4);
        WriteTest(32);

        if(!CARD_SetBusWidth(TRUE))
        {
            printf("Set to 4-bit mode fail\n");
            return;
        }
        printf("Test under 4 bit mode\n");
        ReadTest(4);
        ReadTest(32);
        WriteTest(4);
        WriteTest(32);
    }
    else
    {   // xD
        ReadTest(4);
        ReadTest(32);
    }

    printf("-=# Card Test End #=-\n");
}
#endif  // CARD_TEST

//  *********************************************************************
//  Function    :   CC_DoKeyAction
//  Description :   This function will process all keys
//  Arguments   :
//  Return      :   TRUE[Do was processed]/ FALSE [Not process this key]
//  Side Effect :
//  *********************************************************************
BYTE CC_DoKeyAction ( BYTE bKey )
{
//    BYTE        bTemp;

#ifdef CARD_TEST
    if(bKey == KEY_DISPLAY)
    {
        CC_CardTest();
        return TRUE;
    }
#endif  // CARD_TEST

    if (bKey < INPUT_KEY_GROUP2)
        // 1st group
        return _KeyGroupTest(bKey);
    else
    if (bKey < INPUT_KEY_GROUP3)
        // 2nd group
        return _KeyGroupAudio(bKey);
    else
    if (bKey < INPUT_KEY_GROUP4)
        // 3rd group
        return _KeyGroupPlay(bKey);
    else
    if (bKey < INPUT_KEY_GROUP5)
        // 4th group
        return _KeyGroupMode(bKey);
    else
// Micky0.80, Group5 is not processed, Group 6 is processed by _CC_NumberProcessKey earlier
/*    
    if (bKey < INPUT_KEY_GROUP6)
        // 5th group
        return TRUE;    // ** TCH0.60;  _KeyGroupMultiDisc(bKey);
    else
*/    
    if (bKey < INPUT_KEY_GROUP7)
        // 6th group
        // Micky0.80, should be processed by _CC_NumberProcessKey
        return INVALID_KEY;
    else
    if (bKey < INPUT_KEY_GROUP8)
        // 7th group
        return _KeyGroupVideo(bKey);
// ** TCH0.27; begin...
    else
    if (bKey < INPUT_KEY_GROUP9)
        // Micky0.80, should be processed by SETUP_ProcessKey
        return INVALID_KEY;//_KeyGroupSetUP (bKey);
    else
    if (bKey < INPUT_KEY_GROUP10)
        return _KeyGroupOthers(bKey);
#ifdef SUPPORT_AV_IN
    else
    if (bKey < INPUT_KEY_END )  //Jeff 20060705 add for AV in
    {
        return _KeyGroupTFT(bKey);
    }
#endif //#ifdef SUPPORT_AV_IN
    else
        return  INVALID_KEY;
// ** TCH0.27; end...

}

// Micky0.67-909, don't issue range within CC_KeyCommand
// remove _Get_PlayRange
#ifndef NEW_PLAY_ITEM   
//  *********************************************************************
//  Function    :   _Get_PlayRange
//  Description :   Control the playing range of CDROM
//                  It will call a important function UTL_PlayItem ()
//  Arguments   :   NONE
//  Return      :   NONE
//  Side Effect :
//  *********************************************************************
void _Get_PlayRange ()
{
#ifndef NO_DISC_MODE //CoCo2.37p
    if ( __wDiscType & BOOK_DVD )
    {
        LINEAR_PlayItem();
    }
    else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
    {
        __bTemp= LOBYTE ( __wDiscType ) & 0x0f;
        switch ( __bTemp )
        {
#ifndef NO_MP3
            // MP3DISC
            // go the same path with VCD
        case    BOOK_CDROM:
            if ( !(__wDiscType & CDROM_M1) )
                break;
#endif
            
        case    BOOK_VCD:
        case    BOOK_CDDA:
        case    BOOK_CDI:
#ifndef NO_DISC_MODE //++CoCo2.37p
            if ( __btPBC )  // BOOK_CDI and BOOK_CDDA should be FALSE [__btPBC]
            {
                if ( __wPlayItem <= 1 )     // play nothing
                    return;
                if ( __btPause && (__dwTimeBegin != __dwTimeEnd ) )
                    UTL_PlayItem (__wPlayItem, __dwTimeNow );
                else
                {
                    
                    if (__wPlayItem > 999) //segment play item
                    {
                    }
                    else if (__wPlayItem > 99)  //entries
                    {
                        
                        // Micky1.24b, fix VCD select entry play will always play entry 1.
                        UTL_PlayItem(__wPlayItem, NULL_0);
                        // __dwTimeBegin/ End is for the entry
                        // get track number for the entry
                        __dwTemp = __dwTimeBegin;
                        CDINFO_ParseInfo ( VCD_ENTRIES_TRACK, & __dwTemp, NULL_0 );
                        UTL_ShowTrack(MSG_TRACK, (WORD)__dwTemp, 0);
                        // Get track start sector. assign to __dwEntryStart.
                        CDINFO_GetTrackInfo ( (BYTE)__dwTemp, &__dwEntryStart, & __bTemp );
                        return;
                    }
                    else    //track
                    {
                        // f/w 0.32
                        // some 2.0 play item value will be larger than last track??
                        if (__wPlayItem > __wTrackEnd)
                            __wPlayItem=__wTrackEnd;
                    }
                    
                    UTL_PlayItem (__wPlayItem, NULL_0 );
                    UTL_ShowTrack(MSG_TRACK, __wPlayItem, 0);
                }
            }
            else
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
            {
                LINEAR_PlayItem();
            }
            break;
        default:
            break;
        }   // __bTemp ( LOBYTE of (__wDiscType ) )
    }   // ** DVD-TCH0.19;
}
#endif  //#ifndef   NEW_PLAY_ITEM   
//  *********************************************************************
//  Function    :   _CC_ImmediateKey
//  Description :
//
//  Arguments   :
//  Return      :
//  Side Effect :
//  *********************************************************************
// DVD_172Micky, FLOW_CONTROL begin
BYTE    _CC_ImmediateKey()
{
    // these keys have high priority
    // LLY0.62, using  general abort mainflow API instead of checking POWER/ OPEN_CLOSE key
    //if ( ( __bISRKey == KEY_OPEN_CLOSE ) || (__bISRKey==KEY_POWER) )
    if(UTL_QueryCapability(QUERY_ABORT_MAINFLOW))
    {
        // must clear the state that
        // the KEY_OPEN_CLOSE is triggered by KEY_PLAY
// DVD_276Micky, bit reduce
///        __btOpenPlay=FALSE;
        // must action right once
        return TRUE;
    }

    if (__wDiscType == NO_DISC_IN_DRIVE)
    {
        // do not perform any key action
        return TRUE;
    }

#ifdef  AUTO_PLAY
    // LLY0.83b-2, don't do auto-play while CDROM title except Kodak Pictuce CD
    // Notice: it's necessary for CT908 File Manager Like UI
    if(__wDiscType & CDROM_M1)
    {
        if(!(__wDiscType & CDROM_PICTURECD))
        {
// Micky1.05, add auto test for IC sorting by CoCo
#ifdef AUTO_TEST
            __bAutoTest = TRUE;
            __dwAutoTestTime = OS_GetSysTimer();
#endif
            // wyc1.05-2, support FM auto play
            if (!__bMMAutoPlay) //CoCo2.38
                return TRUE;
/* --CoCo2.38
#ifndef MMANAGER_AUTO_PLAY
            return TRUE;
#endif  //#ifndef MMANAGER_AUTO_PLAY
*/
        }
    }

// DVD_172Micky, FLOW_CONTROL begin
    // unplayable disc, don't care flow control
    if ( !(__bDiscInfo & DISCINFO_UNPLAYABLE) )
        __bFlowState |= FLOW_RELEASE_PLAY;  // let the flow to trigger auto play.

#else   //#endif  //#ifdef  AUTO_PLAY
    // Micky1.10, support resume after power on for VCD/CD-DA, even non-auto play.
    // must enable the flag so that ProceedFlowState will process resume
    if (__bFlowState & FLOW_KEEP_RESUME)
        __bFlowState |= FLOW_RELEASE_PLAY;
#endif  //#ifdef  AUTO_PLAY
    return TRUE;
}
// DVD_172Micky, FLOW_CONTROL end

BIT _InvalidAccess(DWORD dwMode)
{
    if (dwMode & CC_MODE_PBC)
    {
        if (__btPBC)
            return TRUE;
    }
    if (dwMode & CC_MODE_PLAY)
    {
        if (__btPlaying || __btPause 
// Micky1.20, 4M flash
#ifndef NO_DIGEST
        || __btDIGEST
#endif
        )
            return TRUE;
    }
    if (dwMode & CC_MODE_GOTIME)
    {
//alex1.23,20040218 combine gototime , search
//        if (__btGoToTime)
        if (__bSearchMode == SEARCH_MODE_TIME)
            return TRUE;
    }
// Micky1.20, 4M flash
#ifndef NO_DIGEST
    if (dwMode & CC_MODE_DIGEST)
    {
        if (__btDIGEST)
            return TRUE;
    }
#endif
    // Micky0.65-909.
    if (dwMode & CC_MODE_PROGRAM)
    {
        if (__btProgram)
            return TRUE;
    }
    return FALSE;
}

BYTE    _KeyGroupAudio(BYTE bKey)
{
switch(bKey)
{
// KEY_GROUP2, audio keys
// Audio
    case    KEY_AUTOVOCAL:
        __bAutoVocal++;
// hwtan 6.10 begin
#ifdef    ENABLE_3_AUTOVOCAL         //ping add
        if ( __bAutoVocal > 3 )
#endif  //#else
#ifdef    ENABLE_2_AUTOVOCAL
        if ( __bAutoVocal > 2 )
#else
        if ( __bAutoVocal > 1 )
#endif
            __bAutoVocal= NULL_0;

        // only allow CVD/SVCD has 3 auto vocal mode
        if (  ( __wDiscType & BOOK_SVCD ) || (__wDiscType & BOOK_CVD) )   //ping add
        {
            // ** TCH0.60; 909 & 908 are same 

            // force to play audio ID 0
            HAL_ControlParser ( HAL_PARSER_AUDIO_ID, 0 );
            HAL_ControlParser ( HAL_PARSER_ENABLE, NULL_0 );
            __bModeChannel=AUDIO_STEREO;    //change to stereo channel
        }
        else if (__bAutoVocal==3)
        {
            __bAutoVocal=NULL_0;
        }

        OSD_OUTPUT_MACRO ( MSG_AUTOVOCAL, __bAutoVocal, 0x3 );    // clear Digest

        // DVD_104Micky, enhance the vocal mode
//        if ( (!__btVocalON) && __bAutoVocal )    // in Non-Vocal Status AND AutoVocal ON
        if ( (__bVocal==VOCAL_OFF) && __bAutoVocal )    // in Non-Vocal Status AND AutoVocal ON
        {
            if (__bAutoVocal ==3 )
            {
                // ** TCH0.60; 909 & 908 are same 

                // force to play audio ID 1
                HAL_ControlParser ( HAL_PARSER_AUDIO_ID, 1 );
                HAL_ControlParser ( HAL_PARSER_ENABLE, NULL_0 );
                __bModeChannel=AUDIO_STEREO;    //change to stereo channel
            }
            else
            if ( __bAutoVocal == 2 )
                __bModeChannel=AUDIO_RR;    //change to right channel
            else
                __bModeChannel=AUDIO_LL;    //change to left channel
            OSD_OUTPUT_MACRO ( MSG_AUDIO, __bModeChannel, 0 );
            CHIPS_OpenAudio(TRUE);
        }
// hwtan 6.10 end
        break;

    case    KEY_AST_CTL:
        // Micky0.80, move into CC_Audio_Change.            
        return CC_Audio_Change(CC_TOGGLE_AUDIO);

//Kevin1.10, add MIC volume up/down
#ifndef NO_MIC_INPUT 
    case    KEY_MIC_VOL_UP: 
            CHIPS_MICVolControl(CHIPS_UP);
            break;

    case    KEY_MIC_VOL_DOWN:
            CHIPS_MICVolControl(CHIPS_DOWN);
            break;
#endif             
    // ** SVCD-TCH;
    case    KEY_VOL_UP:
            if (!CHIPS_VolumeControl(CHIPS_UP))
                return INVALID_KEY ;
            break;

    case    KEY_VOL_DOWN:
            if (!CHIPS_VolumeControl(CHIPS_DOWN))
                return INVALID_KEY ;
            break;

        //integrate KEY_KEYUP/KEY_KEYDOWN/KEY_NORMAL
    case    KEY_KEYUP:
        // Micky1.11, KC buffer is used as audio buffer when BE DIGEST mode
        // Brian1.07f, disable key control in Digest mode, for the KC buffer doesn't exist
#ifdef USE_32M_SDRAM
#ifndef NO_DIGEST
        if (__btDIGEST)
            return INVALID_KEY;
#endif
#endif
            if ( ! CHIPS_KeyControl(CHIPS_UP) )
                return INVALID_KEY;
            break;
    case    KEY_KEYDOWN:
        // Micky1.11, KC buffer is used as audio buffer when BE DIGEST mode
        // Brian1.07f, disable key control in Digest mode, for the KC buffer doesn't exist
#ifdef USE_32M_SDRAM
#ifndef NO_DIGEST
        if (__btDIGEST)
            return INVALID_KEY;
#endif
#endif
            if ( ! CHIPS_KeyControl(CHIPS_DOWN) )
                return INVALID_KEY;
            break;
    case    KEY_KEYNORMAL:
        // Micky1.11, KC buffer is used as audio buffer when BE DIGEST mode
        // Brian1.07f, disable key control in Digest mode, for the KC buffer doesn't exist
#ifdef USE_32M_SDRAM
#ifndef NO_DIGEST
        if (__btDIGEST)
            return INVALID_KEY;
#endif
#endif
            // Don't choose this key as debug key,
            // becasue some IR don't support this key
            if ( ! CHIPS_KeyControl(CHIPS_NORMAL) )
                return INVALID_KEY;
            break;
    case    KEY_VOCAL:
        {
            // TCC2.79-2, add DTS-CD
//Alex1.10-2, 20031105 System will support MP2 format audio
            if(__bAttrPlay==ATTR_MP3 || __bAttrPlay == ATTR_MP2 ||
            __bAttrPlay==ATTR_JPG ||
            __bCDDA_DTS == CDDA_DTS)            
                return INVALID_KEY ;

#ifndef NO_DISC_MODE //CoCo2.37p
            // TCC050a, for dvd with AC3, will support vocal in chips
            if (__wDiscType & BOOK_DVD)
            {
                if (!CHIPS_AC3KokMode (CHIPS_TOGGLE))
                    return INVALID_KEY ;
                break ;
            }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

            // DVD_104Micky, enhance the vocal mode
//            __btVocalON= ! __btVocalON;

            if ( __bVocal == VOCAL_OFF )
            {
                __bVocal = VOCAL_ON;
                __bModeChannel=AUDIO_STEREO;    //change to stereo channel
                // open vocal part
// hwtan 6.10 begin
                if(__bAutoVocal == 3 )       // ping add
                {
                    // ** TCH0.60; 909 & 908 are same
                    HAL_ControlParser ( HAL_PARSER_AUDIO_ID, 0 );
                    HAL_ControlParser ( HAL_PARSER_ENABLE, NULL_0 );
                }
// hwtan 6.10 end
                if (__btMute)
                {
                    __btMute= FALSE;
                    OSD_OUTPUT_MACRO( MSG_MUTE, __btMute, 0 );
                }
            }
            else
            {
                // DVD_104Micky
                __bVocal = VOCAL_OFF;
// hwtan 6.10 begin
                // cancel vocal part
                if (__bAutoVocal ==3 )
                {
                    // for SVCD/CVD disc only
                    // force to play audio ID1
                    // ** TCH0.60; 909 & 908 are same
                    HAL_ControlParser ( HAL_PARSER_AUDIO_ID, 1 );
                    HAL_ControlParser ( HAL_PARSER_ENABLE, NULL_0 );
                    
                    __bModeChannel=AUDIO_STEREO;    //change to stereo channel
                }
                else
// hwtan 6.10 end
                if ( __bAutoVocal == 2 )
                    __bModeChannel=AUDIO_RR;    //change to right channel
                else
                    __bModeChannel=AUDIO_LL;    //change to left channel
            }
            OSD_OUTPUT_MACRO ( MSG_AUDIO, __bModeChannel, 0 );
            CHIPS_OpenAudio(TRUE);
            OSD_OUTPUT_MACRO(MSG_VOCAL, __bVocal, 0x3 );
        }
        break;

    case    KEY_AUDIO_LR:
        {

        // wyc2.37-909s, support audio L/R in all mode except DVD disc.
        /*if (__bAudioType != HAL_AUDIO_PCM && __bAudioType != HAL_AUDIO_MPG 
        && __bAudioType != HAL_AUDIO_MP3 && __bAudioType != HAL_AUDIO_MP2 )
        {
#ifdef DOLBY_CERTIFICATION //Kevin0.80, acmod = (CH1,CH2) supports AUDIO_LR            
            if (__bAudioType==HAL_AUDIO_AC3)
            {
                HAL_ReadAM(HAL_AM_ACMOD, &__dwTemp); 
                if (__dwTemp!=0)             
                    return  KEY_INVALID;
            }
            else
#endif            
            return  KEY_INVALID;                
        }*/
#ifdef DOLBY_CERTIFICATION //Kevin0.80, acmod = (CH1,CH2) supports AUDIO_LR            
        if (__bAudioType==HAL_AUDIO_AC3)
        {
            HAL_ReadAM(HAL_AM_ACMOD, &__dwTemp); 
            if (__dwTemp!=0)             
                return  KEY_INVALID;
        }
#endif

        //Kevin1.07, comment to avoid no sound for a while when audio L/R switch
        //Kevin1.05, avoid the noise when audio L/R switch
        //CHIPS_OpenAudio(FALSE);

        switch ( __bModeChannel )
        {
            case    AUDIO_STEREO:              //stereo(ch0, ch1)
                __bModeChannel= AUDIO_LL;  //left channel (ch0, ch0)
                break;

            case    AUDIO_LL:
                __bModeChannel= AUDIO_RR;  //right channel(ch1, ch1)
                break;

#ifdef DOLBY_CERTIFICATION //Kevin0.80
            case    AUDIO_RR:
                __bModeChannel= AUDIO_MIXED;  //right channel(ch1, ch1)
                break;    
            case    AUDIO_MIXED:  
#else
            case    AUDIO_RR:                                  
#endif
            default:
                __bModeChannel= AUDIO_STEREO;
                // Voice cancel is exclusive with other mode

                // If User hope to see "STEREO" words when Key-Shift
                // Please just assign __bModeChannel = AUDIO_STEREO;
                // else use below block  "// *********"
                break;
        }       // __bModeChannel

        OSD_OUTPUT_MACRO(MSG_AUDIO, __bModeChannel, 3);
        if (__btMute)
        {
            __btMute= FALSE;
            OSD_OUTPUT_MACRO(MSG_MUTE, __btMute, 0 );
        }
        CHIPS_OpenAudio(TRUE);

        }
        break;

    case    KEY_MUTE: 
        // wyc2.77-909p, start the usb performance testing code.
#ifdef TEST_DEVICE_PERFORMANCE
        __dwTestUSBSpeed = TRUE;
#endif //
        if ( __btMute )
        {
            HAL_IOMute(FALSE);
            __btMute= FALSE;
        }
        else
        {
            HAL_IOMute(TRUE);
            __btMute= TRUE;
        }

        //Kevin0.87, some application don't allow mute
        // must restore its value
        // how about AVSYS???
        //CHIPS_OpenAudio((BYTE)(!__btMute));
        if (!CHIPS_OpenAudio((BYTE)(!__btMute)))
        {
            //__btMute = ~__btMute;   //restore
            // wyc1.10-909, use follow code to restore the mute state.
            if ( __btMute )
                __btMute= FALSE;
            else
                __btMute= TRUE;
            return INVALID_KEY;
        }

        {
            BYTE    bTime;
            if (__btMute)
                bTime=0xFF;
            else
                bTime=1;
            OSD_OUTPUT_MACRO(MSG_MUTE, __btMute, bTime);
        }
        break;

    case    KEY_SURROUND:
        if ( !CHIPS_Surround(CHIPS_TOGGLE ) )
            return INVALID_KEY;
        break;

    case    KEY_ECHO_UP:
            if ( !CHIPS_Echo(CHIPS_UP) )
                return INVALID_KEY;
            break;
    case    KEY_ECHO_DOWN:
            if ( !CHIPS_Echo(CHIPS_DOWN) )
                return INVALID_KEY;
            break;

    case    KEY_VOICECANCEL:           // use Karaoke Chip capability
            // must check CHIPS's API definition
            return INVALID_KEY;

    case    KEY_KARAOKE:
            // wyc1.20-909, return KEY_INVALID when don't support MIC input.
#ifdef NO_MIC_INPUT
            return KEY_INVALID;
#else
            //Kevin1.24, modify
            // DVD_104Micky,
            if (__SetupInfo.bAudioKaraoke == SETUP_AUDIO_KARAOKE_ON)
            {
                __SetupInfo.bAudioKaraoke = SETUP_AUDIO_KARAOKE_OFF; 
                __bTemp=FALSE; //CHIPS_OFF;
            }
            else
            {
                __SetupInfo.bAudioKaraoke = SETUP_AUDIO_KARAOKE_ON; 
                __bTemp=TRUE; //CHIPS_ON;
            }

            /*            
            if ( !CHIPS_MICControl ( __bTemp ) )
                return INVALID_KEY;

            if (__bTemp == CHIPS_ON)
                __SetupInfo.bAudioKaraoke = SETUP_AUDIO_KARAOKE_ON;
            else
                __SetupInfo.bAudioKaraoke = SETUP_AUDIO_KARAOKE_OFF;
            */                            
            CHIPS_MICEnable(__bTemp);
            
            OSD_OUTPUT_MACRO ( MSG_KARAOKE, __bTemp, 0x3 );
            // Tell setup to store the value to EEPROM
            SETUP_ChangeValue (SETUP_AUDIO_KARAOKE) ;
            break;
#endif //
// f/w 0.45
// add new Keys and function
    case    KEY_3D:        
         return INVALID_KEY;
    //CoCo.099d
    // support a key to switch audio output(2 ch/5.1ch/spdif)
    case KEY_AUDIO_OUTPUT:
        // Brian1.23, support a key to switch Analog Audio Output (2 ch/5.1 ch)
         __SetupInfo.bAudioAnalogOut++;
         if (__SetupInfo.bAudioAnalogOut > SETUP_AUDIO_ANALOG_OUT_END)
         __SetupInfo.bAudioAnalogOut = SETUP_AUDIO_ANALOG_OUT_BEGIN;

         // DVD_104Micky. convert setup value ID
         SETUP_ChangeValue(SETUP_AUDIO_OUTPUT);
         SETUP_AdjustSetting(SETUP_AUDIO_OUTPUT);
         OSD_OUTPUT_MACRO(MSG_AUDIO_OUTPUT_CTL, __SetupInfo.bAudioAnalogOut, 3);
         break;
/*
        __SetupInfo.bAudioOutput++;
        if (__SetupInfo.bAudioOutput > SETUP_AUDIO_OUTPUT_END)
            __SetupInfo.bAudioOutput = SETUP_AUDIO_OUTPUT_BEGIN;

        // DVD_104Micky. convert setup value ID
        SETUP_ChangeValue(SETUP_AUDIO_OUTPUT);
        //CoCo.172q, fix the problem about MP3/LPCM/CD-DA audio is not continuous.
        //if (__bAudioType == HAL_AUDIO_AC3)  // Brian.172r call SETUP_AdjustSetting instead
        //    HAL_SetAudioType(__bAudioType);
        SETUP_AdjustSetting(SETUP_AUDIO_OUTPUT);

        OSD_OUTPUT_MACRO(MSG_AUDIO_OUTPUT_CTL, __SetupInfo.bAudioOutput, 3);
        break;
*/
    default:
    // Micky0.65-909, must return INVALID_KEY;
        return INVALID_KEY;
}
    return TRUE;
}


BYTE CC_PWRCtrl(void)
{
#ifdef SUPPORT_AV_IN
    if(_btAVIN)
    {
        if(__btPowerDown == TRUE)
        {
            if (_btTFT_PowerDown == TRUE)
            {
                _btTFT_PowerDown = FALSE;
                TFT_PowerDown(FALSE);
            }   
            else
            {   
                _btTFT_PowerDown = TRUE;    
                TFT_PowerDown(TRUE);
            }
            return TRUE;
        }
    }

#endif
        __btPowerDown=!__btPowerDown;
        if (__btPowerDown)
        {
            // wyc1.10-2, don't allow enter power down when USB recognize disc, otherwise system will hang.
            __bTemp = MediaInfo[_MapSourceID2Index(__SF_SourceGBL[__dwSFStreamID].bSourceIndex)].bState;
            if (((__bTemp == MEDIA_INSERT) || (__bTemp == MEDIA_PARSING)) && (__SF_SourceGBL[__dwSFStreamID].bSourceIndex == SOURCE_USB0_0))
            {
                __btPowerDown=!__btPowerDown;
                //break;
                return TRUE;
            }

            // wyc2.51-909P, add coco updated code.
#ifdef SUPPORT_TFT
            TFT_BackLight(FALSE);
            TFT_FreeRun(TRUE);
#endif //SUPPORT_TFT

// f/w 0.43 fix(12.17.97) begin
// Problem:
// Play CD-DA->Power down-> has short audio noise
            //extern BIT __btMPEGtoFirstCDDA;
            CHIPS_OpenAudio(FALSE);

            //Kevin1.00, H/W mute to avoid the bo noise when IR power down
            // must put before the first H/W related control
            // the root cause is still unknown.
            // Must add a delay after pulling the signal.
            // Also the delay value is just an experience value.
            HAL_IOMute(TRUE); //H/W mute
            OS_DelayTime ( COUNT_500_MSEC);

            // LLY1.00, let flow same as KEY_STOP first before enter power down mode
            // Thus, __dwResumeTime can be calculated done within it
            // Otherwise, disc resume feature can't work for IR power off/ on case
            CC_DoKeyAction(KEY_STOP);            

// DVD_172Micky, FLOW_CONTROL(*** RESUME_AFTER_POWERON ***)
// after I/R power on, keep resume.
#ifdef  SUPPORT_RESUME_AFTER_POWERON
            // LLY1.00, compiler error since no declare of "_wDiscTypeBackup"
            // after open, the discard resume flag will be set.
            //if ( (_wDiscTypeBackup!=NO_DISC_IN_DRIVE) && !(__bFlowState & FLOW_RELEASE_PLAY) ) //open / reading disc / about to play
            if ( (__wDiscType!=NO_DISC_IN_DRIVE) && !(__bFlowState & FLOW_RELEASE_PLAY) ) //open / reading disc / about to play
            {
// Micky1.06, remove unnecessary codes, as will be checked within _Save_Resume_Information            
/*            
                // must be done before _CC_ResetState/ INITIAL_Variables.
                // as __wDiscType will be cleared
                // Micky1.10, support resume after power on for DVD/CDDA/VCD
                // how about the critical case-> Title detect ok-> soon Key =power_resume
                // it should have nothing to resume?
                // if Power on stand by, the disc type is NO_DISC_IN_DRIVE
                //Kevin1.26, CDG doesn't support resume
                if ( (__wDiscType & BOOK_DVD) ||
                     ( (__wDiscType & BOOK_VCD) && (__wPlayItem >=2) && (__wPlayItem <=99) && (__dwResumeTime !=0) ) || 
                     ( (__wDiscType & BOOK_CDDA ) && !(__bCDDA_DTS & CDDA_CDG) && (__dwResumeTime !=0) )  )
                {
                     __bFlowState |= FLOW_KEEP_RESUME;
                }
*/                

                // Ming2.36, support multi-disc resume feature.
                _bAPPAction |= CC_ACTION_SAVE_RESUME;
            }
#endif  //#ifdef  SUPPORT_RESUME_AFTER_POWERON

            // DVD_101bMicky, merge the same action for Power down/Open
            // DVD_172Micky, FLOW_CONTROL begin
            // send KEY parameter for open.
            CC_ResetState( KEY_POWER );
            // DVD_172Micky, FLOW_CONTROL end

            // Micky1.07, fix open->power down-> power on-> will see "open" OSD
            // temporary clear the open message(OSD can consider clear it at MSG_POWER)
            OSD_OUTPUT_MACRO ( MSG_OPEN, FALSE, 0 );


            // LLY.170-2, merge SERVO relative control into HAL_PowerDown()

            // must send message first, need do some OSD control first
            // then can really enter power down mode
            OSD_OUTPUT_MACRO (MSG_POWER, __btPowerDown, 1);         // *** 11.5;
            // wyc2.38-909s, need to suspend USB thread when entering power down mode because CT950 must suspend USB thread when
            // not in USB mode.
#ifdef SUPPORT_POWERON_MENU //CoCo2.38
            SRCFilter_USBSRCExit();
            MEDIA_ExitUSBSRC();
#endif //SUPPORT_POWERON_MENU //CoCo2.38

            //CoCo2.60
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {
                MM_ExitJPEGAudioMode();
            }
#endif //SUPPORT_JPEG_AUDIO_PLAYING

            ///  POWERDOWN_AFTER_SCREENSAVER        // ** TCH1.25;
            // avoid the GXA-FillRect failure when POWER_ON
#ifndef NO_SCREEN_SAVER
            __bScreenSaver= NONE_SCREEN_SAVER;
#endif

            HAL_PowerDown(ENTER_POWERDOWN | IR_POWERDOWN_MODE);
            //Kevin1.26, move from above 
            // Micky1.10, fix Reading->setup->Powerdown->Power on, will reading twice.
            // must clear all trigger status
            __bFlowState &= ~(FLOW_OPEN | FLOW_RELEASE_STABLE| FLOW_RELEASE_DETECTION| FLOW_RELEASE_PLAY);

        }
        else
        {
            // LLY.171-1, Merge I/R and H/W power on sequence
            // DVD_160Micky, before leave power down, can't do any action
            //HAL_PowerDown(FALSE);
            INITIAL_System(INITIAL_SYSTEM | IR_POWERDOWN_MODE);

            // DVD_160Micky, before leave power down, can't do any action
            // move it after leave power down mode
            OSD_OUTPUT_MACRO(MSG_POWER, __btPowerDown, 1);  // *** 11.5;

// DVD_100aMicky, in standy mode, KEY_POWER can only do normal action
// Doing PCR total reset, video output is reset, may cause TV encode abnormal???
// notic sometimes NV will auto generate KEY_POWER for error data.
#ifndef POWERON_STANDBY
            // ** TCH0.512; begin... If it's nearby H/W Power-ON. Do total-reset..
            // Prevent the DVD--> ERR_DATA
            // One possible Problem: Default is Power-Down mode and push KEY_POWER within 20 seconds.
            if ( OS_GetSysTimer() < (COUNT_10_SEC* 2) )
            {
                // LLY.101, issue a protection action while logo display fail
                // [1] Issue PCR[8]: total reset, and set "black" border color
                // [2] Reset/Initialize TV Encoder/ Audio DAC if necessary
                // Micky1.24a, from 1.24a, YC move LOGO display early to INITIAL_System.
                // If IR KEY_POWER off->on, LOGO already display, must keep it. Can't do reset.
                // only the case that H/W power on-> DVD detect fail-> navigate will issue F/W KEY_POWER
                // need do total reset
                if (__bKeySource==KEY_FROM_FW)
                    HAL_Reset(HAL_RESET_TOTAL );    // ** TCH0.60; seems 909HAL is same
            }
#endif

            // ** TCH0.512; end...
// DVD_100Micky, merge I/R power on sequence with h/w power on.
            INITIAL_PowerONStatus(INITIAL_POWER_SOFTWARE);
            // wyc2.51-909P, add coco updated code.
#ifdef SUPPORT_TFT            
            TFT_FreeRun(FALSE);
            TFT_BackLight(TRUE);
#endif //SUPPORT_TFT

#ifdef SUPPORT_POWERON_MENU //CoCo2.38
            POWERONMENU_Initial(); 
#endif //#ifdef SUPPORT_POWERON_MENU
#ifdef POWERON_STANDBY
#if (USB_SYSTEM == USB2_0_JUNGO)
            {
#ifdef SUPPORT_USB_SWITCH
                HAL_WriteGPIO(GPIO_A, 23, 0);
#endif
                // wyc2.78-909P, always resume the usbsrc thread because usbsrc won't go when no commands is issued.
                {
                    extern DWORD _dwMediaUSBOTGChkTime;
#ifdef SUPPORT_USB_POWER_BY_IO
#ifdef SUPPORT_USB_HOST_DEVICE
                    HAL_WriteGPIO(USB_POWER_GRP, PIN_USB_POWER, 0);
#else
                    HAL_WriteGPIO(USB_POWER_GRP, PIN_USB_POWER, 1);
#endif //
#endif //
                    USBSRC_ThreadResume();
                    _dwMediaUSBOTGChkTime = OS_GetSysTimer();
                }
            }
#endif
#endif // POWERON_STANDBY
        }

    return TRUE;   
}

BYTE    _KeyGroupMode(BYTE bKey)
{
switch(bKey)
{
// KEY_GROUP4, Mode Control Keys
// System status relative keys
// The process will not be disc/mode depenednt
    case    KEY_POWER:      // as Reset all Functions, it's for test
            return CC_PWRCtrl();
        break;

    case    KEY_TVMODE:
/*      
            {
WIN_PARAMETER ClipArea;             
            ClipArea.sHStart = 0; 
            ClipArea.sVStart = -80;
            ClipArea.wHWidth = 720;
            ClipArea.wVWidth = 640;
            DISP_Clip(&ClipArea, DISP_NEXTVSYNC, DISP_MAINVIDEO);
            SPU_DisplayMode(MODE_LB, MODE_LB|MODE_N2N, 0);
                    
                    break;      
        }
*/      
        {
            MM_CompleteJPGEffect(FALSE);

            // DVD_102pMicky, integrate
            __SetupInfo.bTVSystem ++;
            if ( __SetupInfo.bTVSystem > SETUP_SCREEN_TV_SYSTEM_END )
                __SetupInfo.bTVSystem = SETUP_SCREEN_TV_SYSTEM_BEGIN;

            // DVD_104Micky
            SETUP_ChangeValue(SETUP_SCREEN_TV_SYSTEM);
            // Micky0.65-909
            // Must inform adjust TV mode when key from user, 
            __bLOGO |= LOGO_CHANGE_TV_TYPE;
            //HAL_AdjustTVMode (); // Brian.172r call SETUP_AdjustSetting instead
            SETUP_AdjustSetting(SETUP_SCREEN_TV_SYSTEM);

            OSD_OUTPUT_MACRO ( MSG_TVMODE, __SetupInfo.bTVSystem, 3 );

//            SETUP_ChangeTVSystem ( __SetupInfo.bTVSystem );
        }
        break;
    case    KEY_DIGEST:
// Micky1.20, 4M flash
#ifndef NO_DIGEST
            // LLY.037, disallow enter digest mode for CDDA title
            // and CDDA & MPEG within same disc title
            //if ( __wDiscType & BOOK_CDDA )  // ** 0.21;
            // LLY.037, fix CDDA & MPEG within same title
            // WYC.050a for fix CDDA digest playing from stop mode
            // LLY.274, check playing mode has updated
            //if (( __bModePlay & MODE_PLAYCDDA ) ||( __wDiscType & BOOK_CDDA ))
            if( (__bAttrPlay==ATTR_CDDA) ||( __wDiscType & BOOK_CDDA ))
                return INVALID_KEY;

#ifndef NO_DISC_MODE //CoCo2.37p
            // wyc.100 , Build DVD digest, so in DVD title can allow KEY_DIGEST
#ifdef NO_DVDDIGEST
            if ( __wDiscType & BOOK_DVD )  // ** DVD-TCH0.25;
                return INVALID_KEY;
#endif
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

//jyliu.pc908            if ( !DIGEST_Action(bKey) )
//                return INVALID_KEY;

            // Micky1.11, KC buffer is used as audio buffer when BE DIGEST mode
            // Set KEY control to normal, because KC buffer is used by frame buffer
            // Side effect: the KEY is set to normal and not recovered after leaving Digest mode.
            // DIGEST_Action will set btMute=FALSE;
            // must call OpenAudio(TRUE, then the setting will be done.
#ifdef USE_32M_SDRAM
            __cKeyShift=NULL_0;
            CHIPS_OpenAudio (TRUE);
#endif  //#ifdef USE_32M_SDRAM
#else
            return INVALID_KEY;
#endif
            break;

    case    KEY_OPEN_CLOSE:
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
        // wyc1.05-909, if system is selected in media mode, don't allow open/close tray function because of USB don't have tray.
        // wyc1.21-909, modify the value of __bChooseMedia.
        if (__bChooseMedia != MEDIA_SELECT_DVD)
            return KEY_INVALID;
        // Micky1.00_909_media
        {
            if ( __btDriveOpen )    // open to close
            {
                // wyc2.38a-909s, add slot in code.
#ifdef SUPPORT_SLOT_IN      // KCHong 2.17, for slot-in loader
                {
                extern DWORD __dwCloseTrayTimer;
                __dwCloseTrayTimer = OS_GetSysTimer();
                }
#endif
            }
            else    // close->open
            {
                // Micky0.65-909. use SOURCE_DISC instead of SOURCE_SERVO.
                // as currently use ATAPI.
                // wait stable, parsing, playing
                MEDIA_Removed(SOURCE_DISC);
            }
        }        
        {

// Micky1.06, remove unnecessary codes
// will be cleared within INITIAL_MediaSetting
/*
            // Ming0.80, support multi-disc resume.
#ifndef  SUPPORT_RESUME_AFTER_OPENCLOSE
            // Do not reset the __dwResumeTime now. It will save to 
            // EEPROM later (in _Save_Resume_Information()).
            __dwResumeTime=NULL_0;
#endif  // #ifndef  SUPPORT_RESUME_AFTER_OPENCLOSE
*/

            __btNewTime=FALSE;
            // Must clear selection message
            OSD_OUTPUT_MACRO ( MSG_SELECTIONLIST, 0xFFFF,  0 );

            // TCC041, this variable is set in SER_ATA module
            if (!__bCloseCommand)
            {   // in open mode
// Micky1.06, always set normal mode
// for resume, will be set within INITIAL_MediaSetting
                __bFlowState = FLOW_NORMAL;
/*          
                // DVD_172Micky, FLOW_CONTROL begin
                // set the flow state as normal once close disc.
                // Ming0.80, set the flow state as resume while support disc resume case
#ifdef  SUPPORT_RESUME_AFTER_OPENCLOSE
                // LLY1.00, it's un-necessary to clear the resume time here
                // Since, resume time will be set if detect the disc resume information
                //__dwResumeTime=NULL_0;
                // Will re-check at the _Check_CDROMStatus function
                // after calling _Detect_Resume_Information().
                __bFlowState = FLOW_KEEP_RESUME;
#else   // #ifdef  SUPPORT_RESUME_AFTER_OPENCLOSE
                __bFlowState = FLOW_NORMAL;
#endif // #ifdef  SUPPORT_RESUME_AFTER_OPENCLOSE
*/
                // DVD_172Micky, FLOW_CONTROL end
                // Micky0.76, clear setup UI
                OSD_RemoveUI(OSD_UI_SETUP);
                // wyc1.06-909, CoCo's updated code.
                OSD_RemoveUI(OSD_UI_DISPLAY); //CoCo1.06
                // wyc1.11-909, need to clear media select UI when close tray. Otherwise will see black dialog block after entering MM UI.
                CC_ResetState( KEY_OPEN_CLOSE );
                OSD_NormalMode();

#ifndef SUPPORT_TOP_OPEN
                OSD_OUTPUT_MACRO ( MSG_OPEN, FALSE, 0x10 );
#endif //
                SrcFilter_CloseTray();
                __dwOpenTime = OS_GetSysTimer();
                // DVD_160Micky, fix quick open/close won't read disc
                // must set as _bOpen from 1.55
                _bOpen = 0 ;        // otis
                        _dwTimeCheckTray = OS_GetSysTimer() ;  // TCC172, let it restart so that it won't check tray right after close tray command
    // Micky1.00_909_media
#ifdef  SUPPORT_MEDIA_MANAGEMENT

                __bMediaServoAction = MEDIA_SERVO_ACTION_CLOSE;
                __bMediaServoState = MEDIA_SERVO_NONE;
#endif  //#ifdef  SUPPORT_MEDIA_MANAGEMENT
                break;
            }
            else
            {
            
                // Micky0.65-909. reset servo status.
#ifdef  SUPPORT_MEDIA_MANAGEMENT
                __bMediaServoAction = MEDIA_SERVO_ACTION_NONE;
                __bMediaServoState = MEDIA_SERVO_NONE;
#endif  //#ifdef  SUPPORT_MEDIA_MANAGEMENT            

                // Ming0.80, support multi-disc resume.
#ifdef  SUPPORT_RESUME_AFTER_OPENCLOSE
                if ( (__wDiscType!=NO_DISC_IN_DRIVE) && !(__bFlowState & FLOW_RELEASE_PLAY) ) //open / reading disc / about to play                     
                {
                    // Micky1.06, remove unnexessary codes, as will be checked within _Save_Resume_Information
/*                
                    if ( (__wDiscType & BOOK_DVD) ||
                         ( (__wDiscType & BOOK_VCD) && (__wPlayItem >=2) && (__wPlayItem <=99) && (__dwResumeTime !=0) ) || 
                         ( (__wDiscType & BOOK_CDDA ) && !(__bCDDA_DTS & CDDA_CDG) && (__dwResumeTime !=0) )  )
                    {
                         __bFlowState |= FLOW_KEEP_RESUME;
                    }
*/                    
                    _bAPPAction |= CC_ACTION_SAVE_RESUME;
                }
#endif  // #ifdef  SUPPORT_RESUME_AFTER_OPENCLOSE

                // wyc0.85a, remove it because will clear it by CC_ResetState and it will call MM_Exit function.
                // ready to open drive
                // LLY1.00, remove it since it will be done within MEDIA_Remove(SOURCE_DISC)
                //CC_KeyCommand ( KEY_STOP );

                // DVD_101bMicky, merge the same action for Power down/Open
                // DVD_172Micky, FLOW_CONTROL begin
                // send KEY parameter for open.
                CC_ResetState( KEY_OPEN_CLOSE );
                // set the flow state as FLOW_OPEN.
                __bFlowState = FLOW_OPEN;
                // DVD_172Micky, FLOW_CONTROL end

                OSD_OUTPUT_MACRO ( MSG_OPEN, TRUE, 0xFF );
                // Micky2.80, Let LOGO keep as last played video setting(N or PAL)
                // must inform HAL_AdjustTVMode don't do any action.
                UTL_ShowLogo();
                _bOpen = 1 ;

                // ** TCH1.60-1; begin...  Enhance the recovery if Loader is down
                if ( ! SrcFilter_OpenTray () )
                {
                    if ( ! SrcFilter_OpenTray () )
                    {
#ifdef  _DEBUG_INFO
                        OSD_OUTPUT_MACRO ( MSG_ERRCODE, 100, 3 )
#endif  //_DEBUG_INFO
                        // LLY.103e-XQ & LLY.160a-1 begin,
                        // re-modify the procedure while open command
                        //SrcFilter_ResetRecover (FALSE);
                        if( ! SrcFilter_ResetRecover(FALSE) )
                        {
#ifdef  _DEBUG_INFO
                            OSD_OUTPUT_MACRO ( MSG_ERRCODE, 200, 0x3 );
#endif  // #ifdef _DEBUG_INF
                            // if fail, must do W9922QF real total reset
                            // And re-initialize W9922QF chip
                            HAL_PowerOnInit(); 
                            // Re-draw LOGO becase logo will appear in OPEN/CLOSE case
                            UTL_ShowLogo();
                        }
                        // LLY.103e-XQ & LLY.160a-1 end ...

                        SrcFilter_OpenTray ();
                    }
                }
                // ** TCH1.60-1; end...
                // wyc.172-2, record the time of open command for check if open command operation success..
                __dwOpenTime = OS_GetSysTimer();

                __btDriveOpen= TRUE;
                HAL_TVSetAPS ( NULL_0 );  // ** TCH0.31; clear APS flags
                // ** TCH1.03; begin... Only open Drive to set the value. Means that we allow 3 time failures max.
                // ** TCH1.03; end...
            }
        }   // KEY_OPEN_CLOSE
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
        break;

    case    KEY_NEXT:
    case    KEY_PREV:
            if ( __wDiscType == NO_DISC_IN_DRIVE )
                break;

#ifndef NO_DISC_MODE //++CoCo2.37p
            if ( __btPBC )
            {
                if ( __wStatus != ID_ENDLIST )
                {
                    if ( bKey == KEY_NEXT )
                        VCD20_Action ( LIST_NEXT );
                    else
                        VCD20_Action ( LIST_PREVIOUS );
                }
                else
                    return  INVALID_KEY;
            }
            else
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
            {
// DVD_099cMicky, support show OSD only when IR key is pressed
#ifdef  SHOW_IR_KEY_OSD_ONLY
                if (bKey==KEY_NEXT)
                    __bTemp=MSG_NEXT;
                else
                    __bTemp=MSG_PREV;
                //OSD_OUTPUT_MACRO(__bTemp, NULL_0, 0x3);
                OSD_OUTPUT_MACRO(__bTemp, OSD_NEXT_PREV_TRACK, MSG_SHOWTIME_NORMAL );
#endif
                LINEAR_ChangeTrack ( bKey, NULL_0 );
            }

            //Fix the bug about the video will flash
            //HAL_Display (HAL_DISPLAY_ONE_FIELD, 0);

            break;

    case    KEY_REPEAT:
        // Micky1.10, define the KEY_REPEAT spec.
        // Don't support it for non-TT_DOM.
        // auto clear as repeat none when passing through non-TT_DOM.
#ifndef NO_DISC_MODE //CoCo2.37p
        if ( __wDiscType & BOOK_DVD )
        {
            if ( __bDomain != TT_DOM )
                return INVALID_KEY;
        }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

#ifndef NO_DISC_MODE //++CoCo2.37p
        if ( __btPBC )  // ** 0.16;
            return INVALID_KEY;
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p

// DVD_101-2Micky, force clear random(random/repeat are exclusive)
        if (__btRandom)
        {
            __btRandom=FALSE;
            OSD_OUTPUT_MACRO(MSG_RANDOM, __btRandom, 0);
        }

// TCC051, for mustek, repeat will clear repeat a-b.
#ifdef REPEAT_CLEAR_REPEATAB
        if (__bRepeatAB != REPEAT_NONE)
        {
            __bRepeatAB = REPEAT_AB ;
            LINEAR_Action(KEY_REPEAT_AB, NULL_0) ;
        }
#endif

        // Brian1.10, support only "Repeat All" and "Repeat One" in Program Mode
#ifdef SUPPORT_ONLY_2_REPEAT_MODE_IN_PROGRAM
        // ** TCH0.25; begin...
        if ( __bRepeat >= REPEAT_DISC && !__btPlayingProgram)
            __bRepeat= REPEAT_NONE;     // ** TCH0.25;
        else
        {
            if (__btPlayingProgram)
            {
                if ( __bRepeat == REPEAT_DISC )
                {
                    __bRepeat = REPEAT_TRACK;

                }
                else
                    __bRepeat = REPEAT_DISC;
            }
            else
            {
                 __bRepeat++;
            }
            
#else
        // ** TCH0.25; begin...
        if ( __bRepeat >= REPEAT_DISC )
            __bRepeat= REPEAT_NONE;     // ** TCH0.25;
        else
        {
            __bRepeat++;

            // DVD_160Micky, in Program playing mode, don't allow repea title
            // it has no meaning.
            if (__btPlayingProgram)
            if (__bRepeat == REPEAT_TITLE)
            {
                __bRepeat ++;
            }
#endif  //#ifndef SUPPORT_ONLY_2_REPEAT_MODE_IN_PROGRAM

#ifndef NO_DISC_MODE //CoCo2.37p
            if ( __wDiscType & BOOK_DVD )
            {
                if ( __bRepeat == (REPEAT_TRACK) )
                    __bRepeat= REPEAT_CHAPTER;
                if ( __bRepeat == (REPEAT_TITLE+ 1) )
// **** TCH0.99C-XQ, Begin...
                {
                    //Aron1.11-909, constrained repeat, don't allow repeat all when PBC ON
                    if(__bNVPlayMode)
                    {
                        __bRepeat= REPEAT_NONE;
                    }
                    else
                    {
                        __bRepeat= REPEAT_DISC;
                    }                        
#ifdef    FORCE_SWITCH_NONPBC
                        if ( __bNVPlayMode )
                                {

                                        __bNVPlayMode= ! __bNVPlayMode;
                                OSD_OUTPUT_MACRO ( MSG_PBC, __bNVPlayMode, 3 );
                                if ( ! __bNVPlayMode )
                                {
                                        if ( __bDomain != TT_DOM )
                                        NV_TitlePlay ( 1, 1 );
                                        }
                                }
#endif    // FORCE_SWITCH_NONPBC
                }
// **** TCH0.99C-XQ, end...
            }
            else
#endif //CoCo2.37p
            {
                if ( __bRepeat == (REPEAT_TRACK+ 1) )
                {
                    // ** TCH2.81; begin... 
                    if ( __wDiscType & BOOK_CDROM )
                    {
                        // wyc1.10, program playing mode don't support REPEAT FOLDER.
                        if (__btPlayingProgram)
                            __bRepeat= REPEAT_DISC;
                        else
                            __bRepeat= REPEAT_FOLDER;
                    }
                    else
                    {
                        __bRepeat= REPEAT_DISC;
                    }
                    // ** TCH2.81; end... 
                }
            }
        }
        // ** TCH0.25; end...

        OSD_OUTPUT_MACRO(MSG_REPEAT, __bRepeat, 3 );
        break;

    case    KEY_RANDOM:
#ifndef NO_DISC_MODE //CoCo2.37p

// **** TCH 0.99C-XQ, Begin...
#ifdef    FORCE_SWITCH_NONPBC
        if ( __wDiscType & BOOK_DVD )
        {
                if ( __bNVPlayMode )
                {

                        __bNVPlayMode= ! __bNVPlayMode;
                OSD_OUTPUT_MACRO ( MSG_PBC, __bNVPlayMode, 3 );
                if ( ! __bNVPlayMode )
                {
                        if ( __bDomain != TT_DOM )
                        NV_TitlePlay ( 1, 1 );
                        }
                }
                __btPBC= FALSE;
        }
#endif    // FORCE_SWITCH_NONPBC
// **** TCH 0.99C-XQ, end...

//Aron1.11-909, Enhance Random play function. Only can do random in Title Domain
        if ( __wDiscType & BOOK_DVD )
        {
            if ( __bDomain != TT_DOM )
                return INVALID_KEY;
        }

#endif //CoCo2.37p
        if ( ! __btPBC )
        {
        __btRandom= ! __btRandom;
        if ( __btRandom )
        {
// DVD_101-2Micky, force repeat mode(random/repeat are exclusive)
            if (__bRepeat != REPEAT_NONE)
            {
                __bRepeat=REPEAT_NONE;
                OSD_OUTPUT_MACRO(MSG_REPEAT, __bRepeat, 0);
            }

            UTL_ModeInitial ( MODE_RANDOM );
            if ( ! __btPlaying )
                if ( ! __btPause )
                    __bFWKey= KEY_PLAY;   // *TCH** 97.7.30;
        }
//MICKY041 begin
        OSD_OUTPUT_MACRO(MSG_RANDOM, __btRandom, 3);
//MICKY041 end
        }
        else
            return INVALID_KEY;
        break;

    case    KEY_PROGRAM:
        // wyc.pro Enable the DVD program
        //if ( __wDiscType & BOOK_DVD )   // ** TCH0.29+;
        //    return KEY_INVALID;
        // Micky2.81, move the codes from _CC_PreProcessKey
        if (__btInputProgram)
        {
            LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
        }
        else
        {
        if ( !LINEAR_Program (LINEAR_PROGRAM_MODE,NULL_0) )
            return INVALID_KEY;
        }
        break;

    case    KEY_ENTER:
            // wyc.pro1 to support enter key to input program entry
            if (__btInputProgram)
            {
                _KeyGroupPlay(KEY_PLAY);   //Alan1.07, fix #532 & #553
            }
            else if ( !LINEAR_Program(LINEAR_LEFT_RIGHT_OPERATION, bKey) )
                return INVALID_KEY;
            break;

    case    KEY_CLEAR:
            // DVD_099Micky, add KEY_CLEAR will clear one GOTOTIME entry.
            // but still keep GOTOTIME input
//alex1.23,20040218 combine gototime , search
//            if (__btGoToTime)
            if(__bSearchMode==SEARCH_MODE_TIME)
            {
                if (__bGoToTimeNO)
                {
                    __bGoToTimeNO--;
#ifdef    GOTOTIME_LEFTTORIGHT
                    if (__bGoToTimeNO==1)   // at hour position
                    {
                        __bTimeMin=0;
                    }
                    else
                    if (__bGoToTimeNO==2)   // at Tens of min position
                    {
                        // clear Ones value of Min
                        __bTimeMin-=(__bTimeMin%10);
                    }
                    else
                    if (__bGoToTimeNO==3)   // at Ones of min position
                    {
                        // cleat seconds value
                        __bTimeSec = 0;
                    }
                    else
                    if (__bGoToTimeNO==4)   // at Tens of sec position
                    {
                        // clear Ones value of Sec
                        __bTimeSec-=(__bTimeSec%10);
                    }
#else
                    __bTimeSec = __bTimeSec/10;
                    __bTimeSec +=(__bTimeMin%10)*10;
                    __bTimeMin=__bTimeMin/10;
                    __bTimeMin+=(__bTimeHour%10)*10;
                    __bTimeHour=__bTimeHour/10;

#endif
                    // must also re-calculate __bTimeMin/Hr...
                    OSD_OUTPUT_MACRO ( MSG_GOTOTIME, __bGoToTimeNO, 0xFF ); //always show goto time
                }
                break;

            }
#ifdef  ENABLE_CLEAR_CURRENTPROGRAMITEM
            if ( !LINEAR_Program(LINEAR_PROGRAM_CLEAR_ENTRY,NULL_0) )
                return INVALID_KEY;
#else
            // wyc.101b, to make KEY_CLEAR to clear all entry but not exit program mode
            if ( !LINEAR_Program(LINEAR_PROGRAM_CLEAR_ALL_ENTRY,NULL_0) )
                return INVALID_KEY;
#endif
            break;

    case    KEY_RESUME:
        // Enable PBC mode RESUME
        //what about end list???
        // wyc1.20-909, don't allow KEY_RESUME when play to last item and auto stop after play end.
        if (__btPlaying || (!__btPlaying && __btPause) || __bLastPlayItem
// Micky1.20, 4M flash
#ifndef NO_DIGEST
            || __btDIGEST 
#endif
#ifndef NO_CDG 
            || ( (__bCDDA_DTS & CDDA_CDG) && ( __bCountStop >=2 ) )
#endif  //#ifndef NO_CDG 
        )
        {
            return  INVALID_KEY;
        }

        {
        extern  DWORD   _dwTimePrev;    // [UTL]
        _dwTimePrev= NULL_0; // ** TCH1.60-1; Let Time Display can show current time.
        }


#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
        // DVD_171Micky, when pause-> play, do servo pre-seek
        // fix pre-stop-> servo stop running-> resume, A/V play unsmooth
        // wyc1.06-909, adding resume parameter.
        UTL_PreStartServo( __dwTimeNow,ACTION_SKIP);
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p

#ifndef NO_DISC_MODE //CoCo2.37p
        // ** TCH0.40; begin...
        if ( __wDiscType & BOOK_DVD )
        {
            /*
            // DVD_099Micky, add GOTOTIME UOP check
            // For TimeSearch must check UOP0/UOP5
            // must check Time Search
            if ( ! NV_QueryUOP (UOP_CAN_TIME_PLAY) )
            {
                return INVALID_KEY;
            }
            if ( ! NV_QueryUOP (UOP_CAN_UOP5) ) // check Time_Search
            {
                return INVALID_KEY;
            }
            */

            if ( ! NV_Action ( ACTION_RESUME, NULL_0 ) )
                return INVALID_KEY;
            // ** TCH0.42; begin...
//alex1.23,20040218 combine gototime , search
/*
            if ( __btGoToTime )
            {
                OSD_OUTPUT_MACRO ( MSG_SETGOTOTIME, OSD_CANCEL_TIME, 0 );
                __btGoToTime= FALSE;
            }
*/

            if ( __bSearchMode > SEARCH_MODE_BEGIN )
            {
                __bSearchMode=SEARCH_MODE_OFF;
                // Micky0.60-909, modify for CT909 OSD
                OSD_ChangeUI(OSD_UI_SEARCH, OSD_EXIT_UI); 
            }
            // ** TCH0.42; end...
            break;
        }
        // ** TCH0.40; end...
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

        //need check is in segment play item or not??
        //Enable PBC mode RESUME
#ifndef NO_DISC_MODE //++CoCo2.37p
        if (__btPBC)
        {
            // Stop RESUME command if User choose ENDLIST.
            if ( ! VCD20_IsValid_Command ( LIST_WORK ) )
            {
                return  INVALID_KEY;
            }

            __wStatus= NULL_0;
            if ( __wPlayItem >= 1000 )
            {
                BYTE    bTemp;

                bTemp= UTL_CheckPlayItemMode ( __wPlayItem );
                if (bTemp & MODE_STILL)
                {
// Micky0.67-909, use new play item control                
#ifdef  NEW_PLAY_ITEM                
    _CC_PlayNewItem(0);
#endif
                    // Micky0.80
                    OSD_OUTPUT_MACRO ( MSG_RESUME, __wTemp, 0x3 );

                    CC_KeyCommand(KEY_PLAY);
                    VCD20_ShowListType();
                    break;
                }
            }   // __wPlayItem >= 1000
        }   // __btPBC
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p

        // LLY0.95, it's un-necessary to build the resume time
        // Since, OSD don't show the resume time now.
#if 0        
        // build the time for OSD/PANEL display the resume time
        __dwTemp= UTL_SectorstoMSF ( __dwResumeTime- __dwTimeDiscBegin );
        __bTimeMin= MSF_MINUTE ( __dwTemp );
        __bTimeSec= MSF_SECOND ( __dwTemp );
        __wTemp= MAKEWORD ( __bTimeSec, __bTimeMin );
        OSD_OUTPUT_MACRO ( MSG_RESUME, __wTemp, 0x3 );        
#endif  // #if 0        
        OSD_OUTPUT_MACRO(MSG_RESUME, 0, 0x3);

        __dwGoToTime=__dwResumeTime;
        // LLY0.62, call _CC_GoTimeAction(EXECUTE) directly 
        // To avoid IR and F/W key conflict issue 
        // Notice: assign __bFWKey is ok but it will be execute while next processing key
        //__bISRKey=KEY_GOTOTIME_PLAY;
        _CC_GoTimeAction(GOTIME_EXECUTE);
        break;

#ifndef NO_DISC_MODE //CoCo2.37p
    case    KEY_SEARCH_PLAY:

        //Aron1.11-909, constrained repeat, disable repeat mode when doing search
        __bRepeat= REPEAT_NONE;            
        OSD_OUTPUT_MACRO(MSG_REPEAT, __bRepeat, 0 );

        //Aron1.11, Fixed 遊走3, 在播放女王頭時,按數字鍵跳到章節之後, 此時使用navagiator功能跳章節會無法跳章節
        //change "search Chapters in the same Title" as LinkPTTN which will not execute PRE-CMD. 
        //And do "search Chapters between different Title" as Jump_VTS_PTT  
        if ( __bSearchTitle == __bTitle )
        {
            NV_TitlePlay ( __bSearchTitle, __wSearchChapter );
            SMAN_PushStateInfo ( STATE_PG, STATUS_READY );
        }
        else
        {
            // Micky2.79-2, support FullScreen function
            // always clear Full screen when goto different title.
            // About auto playing cross different title, or link title
            // many places need take care
            // temporarily don't clear it.
            __bFullScreen = 0;

            NV_InitialParameters ( 0x11 );  // ** TCH1.61a; User-Operatior-> Same as JumpVTS__PTT == PTT_Play (Title number, PTT number)        
            
            NV_TitlePlay(__bSearchTitle, __wSearchChapter);
        }
        // ** TCH1.03; begin...
        // DVD_161aMicky, fix nf200 10/15-> chapter search 10/17 audio has noise
        // must to A/V subpicture reset.
        // Ming0.75, If reset audio, video and SPST simultaneously,
        //           call HAL_Reset(HAL_RESET_AVSPST).
        //HAL_Reset(HAL_RESET_AUDIO);
        //HAL_Reset(HAL_RESET_SPST);
        //HAL_Reset(HAL_RESET_VIDEO);
        HAL_Reset(HAL_RESET_AVSPST);

        // Micky1.03-2, fix GoTo TT/CC video flicker.(PBC off/ w/o total reset)
        HAL_ControlAVSync (HAL_AVSYNC_VIDEOMASTER, 1); // ** TCH1.00-908;      Enable Video Master Mode.

        // DVD_103Micky, fix GoToTime will hang issue
        // need restore the pla command
        UTL_TransferCommandID(__bModeCmd);
///        __bModePlay= MODE_PLAYUNKNOW;
        // ** TCH1.03; end...

        //Fix the bug about the video will flash
        //HAL_Display (HAL_DISPLAY_ONE_FIELD, 0);
        break;

#endif //CoCo2.37p
    case    KEY_REPEAT_AB:
    
            // Micky1.07a, fix RepeatAB-> GoTo TT/CC, won't clear repeat AB.
            // Let GoTo TT/CC, Time <-> RepeatAB be exclusive
            if ( __bSearchMode != SEARCH_MODE_OFF )
            {
                return INVALID_KEY;
            }

#ifndef NO_DISC_MODE //CoCo2.37p
            // **** TCH0.99E-XQ, Begin...
            if (__wDiscType & BOOK_DVD)
            {
                // DVD_155Micky, check cell still and vobu still
                if (CELL_QueryStatus(CELL_IS_STILL) ||
                    CELL_QueryStatus (CELL_VOBU_STILL_ON)
                    )// DVD_100Micky, query cell still status
                {
                    return INVALID_KEY;
                }
            }
            // **** TCH0.99E-XQ, end...
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
            
            if ( !LINEAR_Action(KEY_REPEAT_AB, NULL_0) )
                return INVALID_KEY;
            break;
            
    /////////////////
    //  Psedo Key
    // DVD_277Micky,Fix DVD(GOTOTIME_LEFTTORIGHT) input 5 number-> always show invalid
    // update cc/linear/input.h
    // original codes in LINEAR_Number don't calculate correct __dwGoToTime for DVD.
    // calculate the GOTOTIME value at one place
    // after execute KEY_GOTOTIME_VALUE, must do KEY_GOTOTIME_PLAY immediately
    case    KEY_GOTOTIME_VALUE:
            // LLY0.62, collect processing KEY_GOTIME_VALUE procedure into _CC_GoTimeAction()
            _CC_GoTimeAction(GOTIME_CALCULATE);
    // Notice: after execute KEY_GOTOTIME_VALUE, must do KEY_GOTOTIME_PLAY immediately
    case    KEY_GOTOTIME_PLAY:
            // LLY0.62, collect processing KEY_GOTIME_PLAY procedure into _CC_GoTimeAction()
            return (_CC_GoTimeAction(GOTIME_EXECUTE));
            break;

    case    KEY_DISPLAY:
              // Brian1.00, not allow to switch to Display mode when Digest
// Micky1.20, 4M flash
#ifndef NO_DIGEST
      if (__btDIGEST)
            return INVALID_KEY;
#endif

      //CoCo2.51, Don't allow KEY_DISPLAY when playing JPEG in flash.
      if ((__wDiscType & CDROM_M1) && (__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI))
      {
          return INVALID_KEY;
      }
      
        // wyc0.85a, CoCo suggested to not call OSD display function when disc is parsing & waiting stable. Otherwise some bugs happened.
        __bTemp = MediaInfo[_MapSourceID2Index(__SF_SourceGBL[__dwSFStreamID].bSourceIndex)].bState;
        // wyc1.06-909, CoCo's updated code.
        if ((__bTemp != MEDIA_INSERT) && (__bTemp != MEDIA_PARSING) && (__wDiscType != NO_DISC_IN_DRIVE)) //CoCo1.06
        {
           OSD_Output ( MSG_DISPLAY, 0, 0xFF );
        }
        else
            return INVALID_KEY;
        break;

    case    KEY_REMAIN:
        //DVD_040-1
        // MP3 don't support remain time
        if (__wDiscType & CDROM_M1)
            return INVALID_KEY;

        if ((__bModeCmd==KEY_STOP) )
            return INVALID_KEY;
#ifndef NO_DISC_MODE //++CoCo2.37p
        if ( __btPBC )  // ** 0.16; STILL mode don't has the time concept.
        {
            if ( __bModePlay & MODE_STILL)
                return INVALID_KEY;
            else
            {
                // ** 0.16; in PBC mode don't has ABS time concept
                if ( __bShowTimeType == MSG_RELTIME )
                {
                    __bShowTimeType= MSG_REMAIN_RELTIME;
                }
                else
                {
                    __bShowTimeType= MSG_RELTIME;
                }
            }
        }
        else
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
        {
//DVD_039-1
// support DVD chapter/title time switching
#ifndef NO_DISC_MODE //CoCo2.37p
            if (__wDiscType & BOOK_DVD)
            {
                if ( __bShowTimeType == MSG_TITLE_TIME )
                {
                    __bShowTimeType= MSG_REMAIN_TITLE_TIME;
                }
                else
                if ( __bShowTimeType == MSG_REMAIN_TITLE_TIME )
                {
                    __bShowTimeType= MSG_CHAPTER_TIME;
                }
                else
                if ( __bShowTimeType == MSG_CHAPTER_TIME )
                {
                    __bShowTimeType= MSG_REMAIN_CHAPTER_TIME;
                }
                else
                {
                    __bShowTimeType= MSG_TITLE_TIME;
                }
            }
            else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
            {
                if ( __bShowTimeType == MSG_RELTIME )
                {
                    __bShowTimeType= MSG_REMAIN_RELTIME;
                }
                else
                if ( __bShowTimeType == MSG_REMAIN_RELTIME )
                {
                    __bShowTimeType= MSG_REMAIN_ABSTIME;
                }
                else
                {
                    __bShowTimeType= MSG_RELTIME;
                }
            }
        }

        UTL_OutputShowTime ();
        OSD_OUTPUT_MACRO( __bShowTimeType, NULL_0, 1 );
        break;

    case    KEY_INTRO:
            if ( _InvalidAccess(CC_MODE_PBC|CC_MODE_PLAY |CC_MODE_PROGRAM|CC_MODE_GOTIME) )
                return INVALID_KEY;
            __btIntroMode=TRUE;
            __dwIntroPlayTime= UTL_EstimateSectors((COUNT_10_SEC));
            __bFWKey=KEY_PLAY;
            // add OSD message
            OSD_OUTPUT_MACRO(MSG_INTRO, NULL_0, 0xff);
            break;
            
//Alex1.07a, 20031013 Support OSD SHOW PSCAN PROMPT, OSDPROMPT_ProcessKey will handle this KEY
#ifndef SUPPORT_PSCAN_PROMPT_OSD
    case    KEY_PSCAN:
        // Brian1.07, Support PSCAN key:
        // YUV <==> YUV PSCAN
        // RGB <==> RGB PSCAN
        // Other cases invalid
        if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_YUV)
        {
            __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN;
        }
        else 
        if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN)
        {
            __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_YUV;
        }
        else 
        if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN)
        {
            __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_RGB;
        }
        else
        if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_RGB)
        {
            __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN;
        }
        else 
            return INVALID_KEY;

         SETUP_ChangeValue(SETUP_SCREEN_VIDEO_OUTPUT);
         // P-SCAN is integrated into Video Output
         // SETUP_ChangeValue will set __SetupInfo.bProgressiveScan according to __bSetupInfo.VideoOutput
         SETUP_ChangeValue(SETUP_SCREEN_PROGRESSIVE_SCAN);
         OSD_OUTPUT_MACRO(MSG_PSCAN, NULL_0, 2);
         SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);

        break;
#endif //#ifndef SUPPORT_PSCAN_PROMPT_OSD

    default:
    // Micky0.65-909, must return INVALID_KEY;
        return INVALID_KEY;
}
    return TRUE;
}

// can return follwing key status
//#define KEY_DISALLOW  0
//#define KEY_ACCEPT  1
//#define KEY_IGNORE  2
// !!! when the KEY is decided, must return immediately, or it may be transferred to another key
// Micky2.81, Don't process composite keys for AVSYS/ Setup/ Thumbnail/ FM
BYTE    _CC_PreProcessKey (void )
{
// Below keys are active only under a valid disc
// Convert the __bKey for proper value accord to title and mode
// 1. Auto PBC off
// 2. KEY_NEXT trigger play
// 2. Composite keys
//      - KEY_PLAY_PAUSE/ KEY_PAUSE_STEP/ KEY_STOP_RETURN
// 3. non-NPF macro commands turn GoTo Time/ TT/CC off.
// 4. Macro command toggle
//      - KEY_PLAY---> KEY_FREEZE
//(1 bug, Freeze-> GoToTime-> freeze flag was cleared, but mode unchanged??)
//      - KEY_SF/ KEY_SCF/ KEY_SCB--->KEY_PLAY
// 5. Trigger Key
//      - Digest Trigger play(KEY_PLAY)
//      - KEY_DIGEST--->KEY_PLAY
//KEY_PLAY/ KEY_DIGEST
//      - For Program input / GoTo TT/CC/ GoTo Time/ Digest
// DVD still release
// Shared Keys
//      - KEY_MENU---> KEY_PBC
//      - KEY_DIGEST---> KEY_TITLE
//      - KEY_AUDIO_LR--->KEY_AST_CTL

//-------------------------------------------------------------------
// :: KEY_NEXT trigger play
//      - When STOP mode(execpt DIGEST)
    // ** 0.21; NO DIGEST Mode
// DVD_173Micky, don't allow KEY_NEXT to trigger play.
// now when define SUPPORT_NEXT_TRIGGER_FROM_STOP, will support PBC or non-PBC both

    if ( __bKey == KEY_NEXT )   // CDROM_M1 won't do the flow
    {
        if ( __bModeCmd == KEY_STOP )
        {
#ifdef   SUPPORT_NEXT_TRIGGER_FROM_STOP                   // ** TCH1.72;
            // Suppose it play from first Track
            __bKey=KEY_PLAY;
            return KEY_ACCEPT;
#else
            return KEY_DISALLOW;
#endif
        }
    }

//-------------------------------------------------------------------
// Micky0.80, move into VCD?DVD ProcessKye
/*
// Micky2.37, move VCD part into _CC_VCD_PreProcessKey    
    if ( _CC_VCD_PreProcessKey() )
       return KEY_ACCEPT;

    // Micky2.37, move DVD part into _CC_DVD_PreProcessKey    
    if ( _CC_DVD_PreProcessKey() )
       return KEY_ACCEPT;
*/       
    if ( __bKey == KEY_PLAY_PAUSE )
    {
        __bKey = KEY_PLAY;

        // Means FF/SCAN/SLOW don't has PAUSE in this situation
        // Means FF/SCAN/SLOW don't has DEFAULT in PBC.
        // if support playing & !pause, it can't achive (slow/fast)->play switching
        if ( __btPlaying && (__bModeCmd == KEY_PLAY ) )
        {
            __bKey= KEY_PAUSE;
        }
    }

    // LLY1.00, transfer KEY_PLAY or KEY_PLAY_PAUSE as KEY_RESUME 
    // if flow state is keep resume
    // Fix bug: disc resume feature can't work if user pressing KEY_PLAY
    // before F/W go to the _ProceedFlowState()
    // So, the KEY_PLAY will be done first and resume can't work
    if(__bKey==KEY_PLAY || __bKey==KEY_PLAY_PAUSE)
    {
// Micky1.06, reference SUPPORT_MULTI_DISC_RESUME        
//#if defined(SUPPORT_RESUME_AFTER_POWERON) || defined(SUPPORT_RESUME_AFTER_OPENCLOSE)
#if defined(SUPPORT_MULTI_DISC_RESUME)
        if (!__btPlaying)
        {
            if (__bFlowState & FLOW_KEEP_RESUME)
            {
                __bFlowState &= ~FLOW_KEEP_RESUME;

                __bKey = KEY_RESUME;

                // Assign MSG_RESUME, otherwise the desired message can't be displayed on screen
                __bKeyMessage=MSG_RESUME;

                // Also clear the release play state
                // Otherwise, it will send the play key again
                if (__bFlowState & FLOW_RELEASE_PLAY)
                {
                    __bFlowState &= ~FLOW_RELEASE_PLAY;
                }
                
                return KEY_ACCEPT;
            }
        }                
#endif  // #if defined(SUPPORT_MULTI_DISC_RESUME)
    }
    


//-------------------------------------------------------------------
// :: Composite KEY_PAUSE_STEP, KEY_PAUSE/ KEY_STEP
// :: Act as KEY_PAUSE
//      - CD-DA/ MP3 only has PAUSE
// :: Act as KEY_STEP
//      - STEP mode or Pause mode
    if ( __bKey == KEY_PAUSE_STEP )
    {
        __bKey = KEY_PAUSE;
        if ( ( __btPause && ( __bAttrPlay != ATTR_CDDA ) ) ||
             ( __bModeCmd == KEY_STEP ) )
            __bKey = KEY_STEP;  // let it go the flow non-NPF...
        // don't let the key return
        // still need check release still later
    }

//-------------------------------------------------------------------
// :: Repeat AB share use KEY_RESUME key
// :: Act as KEY_REPEAT_AB
//      - Only when playing mode
#ifdef  SAME_AB_RESUME
    if ( __bKey == KEY_RESUME )
    {
        if (__btPlaying)
        {
            __bKey=KEY_REPEAT_AB;
        }
        return KEY_ACCEPT;
    }
#endif  //#ifdef  SAME_AB_RESUME


    // LLY1.02, enable SCAN mode for DivX w/ skip video stream case
    // LLY1.01, don't support resume/ SCF/ SCB/ Repeat AB/ GoTime action
    // while DivX w/ no index table or audio only mode (video resolution > 720*576)            
    // LLY2.31, re-adjust the procedure for AVI not supported feature.
#if 0
    __dwTemp = PARSER_QueryContainerAudioType(__dwSFStreamID, __bASTNO);
    if( (__bAttrPlay == ATTR_AVI) && ( (__bNoDivXIndexTbl)||(__dwTemp == PARSER_ESFMT_WMA_V1)||(__dwTemp == PARSER_ESFMT_WMA_V2)) )// || __bVideoID==HAL_THROW_VIDEO) )
    {
        // wyc2.21a-909s, add in code to disable AVI+WMA because WMA can't support dis-continue bits.
        if( (__bKey==KEY_RESUME) || (__bKey==KEY_SCF) || (__bKey==KEY_SCB) || (__bKey == KEY_SF))
        {
            return (KEY_DISALLOW);
        }
    }
#endif  // #if 0
    if(__bAttrPlay == ATTR_AVI)
    {
        // [1] AVI + WMA: don't allow SCAN/ Slow/ Resume case, ie. bitstream dis-continue
        __dwTemp = PARSER_QueryContainerAudioType(__dwSFStreamID, __bASTNO);
        /*if((__dwTemp == PARSER_ESFMT_WMA_V1)||(__dwTemp == PARSER_ESFMT_WMA_V2))
        {
            if( (__bKey==KEY_RESUME) || (__bKey==KEY_SCF) || (__bKey==KEY_SCB) || (__bKey == KEY_SF))
            {
                return (KEY_DISALLOW);
            }
        }*/

        // [2] Disable KEY_RESUME while index table is build ready
        if( ! PARSER_QueryAVIIndexTable())
        {
             if( __bKey==KEY_RESUME )
            {
                return (KEY_DISALLOW);
            }
        }

        // [3] Always disable SCAN mode while no index table (don't care if run-time building) -- LLY2.32
        if(__bNoDivXIndexTbl)
        {
            // LLY2.38a, allow KEY_FAST while no index table case.
#ifdef  SAME_FAST_AS_SCF2
            if( (__bKey==KEY_SCF) && (__bModeCmd!=KEY_FF) )
            {
                // Continue process key.
            }
            else
#endif  // #ifdef SAME_FAST_AS_SCF2
            {
                if( (__bKey==KEY_SCF) || (__bKey==KEY_SCB) )
                {
                    return (KEY_DISALLOW);
                }
            }
        }
    }
        


// The "non-NPF macro commands will turn off GoToTime mode"
// has higher priority then macro commands toggle
//-------------------------------------------------------------------
// :: Macro Commands Toggle
// :: KEY_PLAY act as KEY_FREEZE
//      - When freeze mode
// freeze must use its single process(only do command, don't issue play range)
// as KEY_PLAY will have chance to do audio reset
    if ( __bKey == KEY_PLAY )
    {
// Micky1.21, CodeReview --- modify
//  - remove __btVideoFreeze, reference __bModeCmd/ bKeyPrevious=KEY_FREEZE directly
//        if ( __btVideoFreeze &&  !__btPause )
        if ( ( __bModeCmd == KEY_FREEZE ) && __btPlaying )
        {
            __bKey=KEY_FREEZE;
            return KEY_ACCEPT;
        }
    }
//-------------------------------------------------------------------
// :: Macro Commands Toggle
// :: KEY_SF to KEY_PLAY
//      - When playing at max. slow level, will just toggle back to normal play
    if ( __bKey==KEY_SF )
    {
        // Micky0.91, fix Pause-> KEY_SF can't play. begin
        // other macro command to KEY_SF, will reset to 1st level.
        if (__bModeCmd != KEY_SF)
        {
            __bSlowLevel = 1;
            __wSlowSpeed = 2;
        }
        else
        {
            // Micky0.91, fix Pause-> KEY_SF can't play. end
            // KEY_SF-> playing-> KEY_SF, only need to check max. level.
            // otherwise, KEY_SF will toggle  the level.(done by KEY_SF)
            // pause mode, don't need change the slow level.
            if (__btPlaying)
            {
        // Micky0.91, fix Pause-> KEY_SF can't play. begin
//            if (__bModeCmd==KEY_SF)
        // Micky0.91, fix Pause-> KEY_SF can't play. end
                if ( __bSlowLevel==MAX_SLOW_LEVEL )
                {
                    __bSlowLevel=1; //next time slow/2
                    __wSlowSpeed=2; // Slow speed
                    __bKey=KEY_PLAY;
                }
            }
        }   // __btPlaying
        return KEY_ACCEPT;
    }   // __bKey == KEY_SF
    // wyc2.17-909S, for language study
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
    if ( __bKey==KEY_LAN_STUDY)
    {
        // Micky0.91, fix Pause-> KEY_SF can't play. begin
        // other macro command to KEY_SF, will reset to 1st level.
        if (__bModeCmd != KEY_LAN_STUDY)
        {
            __bSlowLevel = 1;
            __wSlowSpeed = 2;
        }
        else
        {
            // Micky0.91, fix Pause-> KEY_SF can't play. end
            // KEY_SF-> playing-> KEY_SF, only need to check max. level.
            // otherwise, KEY_SF will toggle  the level.(done by KEY_SF)
            // pause mode, don't need change the slow level.
            if (__btPlaying)
            {
        // Micky0.91, fix Pause-> KEY_SF can't play. begin
//            if (__bModeCmd==KEY_SF)
        // Micky0.91, fix Pause-> KEY_SF can't play. end
                if ( __bSlowLevel==MAX_LAN_STUDY_SLOW_LEVEL )
                {
                    __bSlowLevel=1; //next time slow/2
                    __wSlowSpeed=2; // Slow speed
                    __bKey=KEY_PLAY;
                }
            }
        }   // __btPlaying
        return KEY_ACCEPT;
    }   // __bKey == KEY_SF
#endif //
//-------------------------------------------------------------------
//LJY1.24, use NO_CDG to determine whether CDG is supported.
#ifndef NO_CDG 
//LJY1.10, not support SCF, SCB, REPEAT_AB, GOTOTIME for CDG
        if(__bCDDA_DTS & CDDA_CDG)
        {
            // Micky0.66, remoev stop/ resume checking.
            if((__bKey == KEY_SCF) || (__bKey == KEY_SCB) || (__bKey == KEY_REPEAT_AB) || (__bKey == KEY_SEARCH) )
                return KEY_DISALLOW;
        }
#endif
//-------------------------------------------------------------------
// :: Macro Commands Toggle
// :: KEY_SCF or KEY_SCB as KEY_FF
//      - When non-Scan mode -> KEY_SCF, the key will act as KEY_FF
//      - KEY_SCBx2-> KEY_SCF, the key will act as KEY_FF
// but for JVC-> CD-DA track, the SCF can't enter
// because we can't distinguish the track attribute at this stage?
// don't need since 2.81, the flow won't include CDRROM_M1
//    if ( !(__wDiscType & BOOK_M1) )
    {
        if ( (__bKey==KEY_SCF) || (__bKey==KEY_SCB) )
        {
            // DVD_2.74Micky, support switching different scan mode, level will be reset
            if (__bKey != __bModeCmd)
            {
                __bScanLevel = 1;
#ifdef  SAME_FAST_AS_SCF2
// DVD_275Micky, fix KEY_SCF won't enter KEY_FF.
// Also support CD-DA track of mixeed VCD+CD-DA can enter KEY_SCF statge.
// Convert KEY_SCF to KEY_FF only when
// non-CDDA disc and non-CDDA playing mode.
//                if ( !(__wDiscType & BOOK_CDDA) && (__bKey == KEY_SCF) )
                // LLY1.01, don't transfer SCF to FF key if video stream has been threw.
                if ( (__bKey == KEY_SCF) && (!(__wDiscType & BOOK_CDDA) && (__bAttrPlay != ATTR_CDDA)) 
                    && (__bVideoID!=HAL_THROW_VIDEO) )
                {
                    if (__bModeCmd != KEY_FF)
                    {
#ifdef SUPPORT_FUSS_MPEG_CD
                    // LLY2.37, don't support FAST mode for Fuss MPEG CD since no video
                    // So, DSP no reference date to skip stream
                    // Thus, data stop sending and OSD is not updated.
                    if( !INFOFILTER_FUSSMPEGCD(IF_MODE_CHECK_MPEG_CD,0))
#endif // #ifdef SUPPORT_FUSS_MPEG_CD
						if (__bAttrPlay != ATTR_JPG) //CoCo2.77							
							__bKey = KEY_FF;
					}
                }
#endif  // #ifdef  SAME_FAST_AS_SCF2
            }
        }
    }
//-------------------------------------------------------------------
// :: Macro Commands Toggle
// :: KEY_SCF/ KEY_SCB to KEY_PLAY
//      - When playing at max. SCF/SCB level, will just toggle back to normal play
// Toggle by the seq. SCF-1 -> SCF-2 -> SCF-N -> normal play
// Toggle by the seq. SCB-1 -> SCB-2 -> SCB-N -> normal play
    if (__btPlaying)
    {
        // KEY_SCF mode/KEY_FF mode-> KEY_SCF
        if (
        ( (__bKey==KEY_SCF) && ((__bModeCmd==KEY_SCF)
#ifdef  SAME_FAST_AS_SCF2
        || (__bModeCmd==KEY_FF)
#endif
        )) || ((__bKey==KEY_SCB) && (__bModeCmd==KEY_SCB))
           )
        {
            //SCFn->KEY_SCF->KEY_PLAY
            //SCBn->KEY_SCB->KEY_PLAY
            if (__bScanLevel == MAX_SCAN_LEVEL)
            {
                __bScanLevel= 1;
#ifdef    ROUND_SCAN
                __bKey=KEY_PLAY;
#else
#ifdef  SAME_FAST_AS_SCF2
#ifdef SUPPORT_FUSS_MPEG_CD
                // LLY2.37, don't support FAST mode for Fuss MPEG CD since no video
                // So, DSP no reference date to skip stream
                // Thus, data stop sending and OSD is not updated.
                if( !INFOFILTER_FUSSMPEGCD(IF_MODE_CHECK_MPEG_CD,0))
#endif // #ifdef SUPPORT_FUSS_MPEG_CD
                __bKey=KEY_FF;
#endif
#endif
                // wyc1.02-909, need to call this function when SCFx32 -> play or SCBx32 -> play.
                if (__bAttrPlay == ATTR_AVI)
                {
                    return _CC_AVIScanToPlayProcess();
                }
            }
            else
            {
                __bScanLevel++;
            }
            return KEY_ACCEPT;
        }
    }   // __btPlaying

    // wyc1.02-909, use function to replace code.
    // LLY1.01, let DivX SCAN --> Play go the same flow of GoTime
    // Since, we must send the data range ahead the desired point
    // Otherwise, the audio data maybe lost a little.
    if(__bAttrPlay == ATTR_AVI)   
    {
        // LLY1.05, do GoTime action while SCAN --> FF mode 
        // Otherwise, audio data maybe lost a little for above operation.
        if( (__bKey==KEY_PLAY || __bKey==KEY_FF) && ((__bModeCmd==KEY_SCF) || (__bModeCmd==KEY_SCB)) )            
        {
            return _CC_AVIScanToPlayProcess();
        }
    }


// Micky0.69, fix KEY_PLAY won't act as KEY_RESUME when pre-stop.
    if ( __bKey == KEY_PLAY )
    {
        // first stop->play, will treat it as RESUME
        if (__bCountStop==1)
        {
            __bKey=KEY_RESUME;
        }
    }
//-------------------------------------------------------------------
// :: Direction keys re-define
    // the key defined sequence must be KEY_UP/DOWN/LEFT/RIGHT
    // the key defined sequence must be KEY_ZOOM_UP/DOWN/LEFT/RIGHT
    // up/ down/ left/ right
    if ( ( __bKey >= KEY_UP ) && ( __bKey <= KEY_RIGHT ) )
    {
        if ( __bZoomFactor != VIEW_NORMAL ) // convert to KEY_ZOOM_UP/down/left/right
        {
            __bKey = KEY_ZOOM_UP + (__bKey-KEY_UP);
            return KEY_ACCEPT;
        }
    }

    // wyc2.21-909S, transfer KEY_AST_CTL to KEY_AUDIO_LR when VCD disc.
    // wyc2.31-909S, when SVCD or CVD disc, don't do transfer key here.
    if ((__bKey == KEY_AST_CTL) && (__wDiscType & BOOK_VCD) && !(__wDiscType & BOOK_2X))
    {
        __bKey = KEY_AUDIO_LR;
    }

#ifndef SUPPORT_FREEZE_FUNCTION
    if (__bKey == KEY_FREEZE)
        return KEY_DISALLOW;
#endif //
    
    return KEY_ACCEPT;
}

#ifndef NO_DISC_MODE
BYTE    _IdentifyRegionCode()
{
    // DVD_170Micky, only DVD need checking region code.
    if ( !(__wDiscType & BOOK_DVD) )
    {
        __bInvalidRegion=FALSE;
        return TRUE;
    }

    if (__bInvalidRegion)
    {
    extern COPYINFO    cpinfo ;

    // DVD_170Micky, unplayable disc...
    // clear the invalid region bit from __bDiscInfo.
    __bDiscInfo &=~DISCINFO_REGION_ERROR;

    // cpinfo.bRegionInfo=0xfd;    // means support region 2 play
// !!!SERVO_ReadCopyInfo must clear invalid bits.
    // bit 0=0 means support Region 1
    // bit 1=0 means support Region 2, and so on.
    //DVD_043-1
    if (__SetupInfo.bRegionControl!=REGION_ALL)
    {
        if ( !((cpinfo.bRegionInfo ^ __wSPRMs[20]) & __wSPRMs[20]) )
        // expression result bit = 0 means don't support region
        {
            // LLY.161-4, modify the some control for this message
            // [1] Using new define for this message
            //     -- can't combined with MSG_INVALID,
            //        because the message will be cleard
            //        by any invaild message w/ non-forever display time
            // [2] Forever display "Error Region DVD" message
            // [3] Using macro to let PANEL know this info.
            //OSD_Output(MSG_INVALID,  OSD_ERR_REGION, 3 );

            // DVD_170Micky, unplayable disc...
            // set the invalid region bit to __bDiscInfo.
//            OSD_OUTPUT_MACRO(MSG_ABNORMAL_DISC, OSD_ERROR_REGION_DVD, 0xFF);
            __bDiscInfo |=DISCINFO_REGION_ERROR;
            OSD_OUTPUT_MACRO(MSG_ABNORMAL_DISC, __bDiscInfo, 0xFF);

            NV_Action ( ACTION_STOP, NULL_0 );    // ** TCH0.452; Need to close NV operation if have...
            return FALSE;
        }
        // LLY.161-4, clear the message while region code is right
        else
            // DVD_170Micky, unplayable disc...
//            OSD_OUTPUT_MACRO(MSG_ABNORMAL_DISC, OSD_ERROR_REGION_DVD, 0x0);
        {
            OSD_OUTPUT_MACRO(MSG_ABNORMAL_DISC, __bDiscInfo, 0x0);
        }
    }
    // LLY.161-4, clear the message while region code is right
    else
        // DVD_170Micky, unplayable disc...
//        OSD_OUTPUT_MACRO(MSG_ABNORMAL_DISC, OSD_ERROR_REGION_DVD, 0x0);
    {
        OSD_OUTPUT_MACRO(MSG_ABNORMAL_DISC, __bDiscInfo, 0x0);
    }
    __bInvalidRegion=FALSE;
    }
    return TRUE;
}
#endif

// DVD_101bMicky, integrate a general back door function
#ifdef  SUPPORT_GENERAL_BACKDOOR
void _GeneralBackDoor(BYTE bBackKey)
{
    // support another backdoor method, in case that
    // video is unvisible in some mode(such as RGB, but TV doesn't have the capability)
    for (__bTemp=0;__bTemp<BACKDOOR_NUMBER;__bTemp++)
    {
        if (bBackKey==__bBackDoorKeys[__bTemp][__bKeyIndex[__bTemp]])
        {
            __bKeyIndex[__bTemp]++;
            // check the last backdoor key
            if (__bKeyIndex[__bTemp] >= BACKDOOR_KEYNUM)
            {
#ifdef SUPPORT_PRINTF
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nSecond backdoor %d", __bTemp);
#endif
                // map backdoor passwords
                __bKeyIndex[__bTemp]=0;     // reset backdoor ID
                // backdoor for reset video output
                if (__bTemp==BACKDOOR_VIDEO_OUTPUT)
                {
                    if (__SetupInfo.bVideoOutput != SETUP_SCREEN_VIDEO_OUTPUT_NORMAL)
                    {
                        // force to change vidoe output as normal
                        __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_NORMAL;
                        // DVD_104Micky. convert setup value ID
                        SETUP_ChangeValue(SETUP_SCREEN_VIDEO_OUTPUT);

                       //Brian0.90, reset to interlace mode
                        __SetupInfo.bProgressiveScan = SETUP_PROGRESSIVE_SCAN_OFF;
                        SETUP_ChangeValue(SETUP_SCREEN_PROGRESSIVE_SCAN);

                        SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);
                    }
                }
                // backdoor for reset setup data as default
                else if (__bTemp==BACKDOOR_SETUP_DEFAULT)
                {
                    /*
                    SETUP_SetDefaultValue();
                    // Brian1.10
                    SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);
                    */
                    //Brian1.24, read optional default
                    SETUP_SetDefaultValue(READ_OPTIONAL_DEFAULT);
                }
                // Brian1.24, begin ***
                // Add new general backdoor
                // Brian1.24, add new general backdoor
                else if (__bTemp==BACKDOOR_SETUP_TOTAL_DEFAULT)
                {
                    SETUP_SetDefaultValue(READ_TOTAL_DEFAULT); //read total default
                }
                // wyc2.21-909S, remove backdoor code.

            }
        }
        else
            // do not map this backdoor password
            __bKeyIndex[__bTemp]=0;
    }// end of BACKDOOR_NUMBER
}
#endif

// DVD_101bMicky, integrate Open/Power action
// called when 1) Open tray, 2) I/R Power off
// bState is the input key.
void    CC_ResetState(BYTE bState)
{
    // LLY2.43, clear the disc resume information while fully STOP
#if defined(SUPPORT_MULTI_DISC_RESUME) && defined(CLEAR_RESUME_WHEN_STOP)
    BYTE bResumeInfoState=SAVE_INFO;

    // Set the flag to clear disc resume info. while fully stop.
    // Notice: can't put this procedure after __bCountStop=2 statement.
    // Otherwise, we can't identify pre-stop or fully stop case.
    if(__bCountStop >= 2)
    {
        bResumeInfoState = CLEAR_INFO;
    }
#endif  // #if defined(SUPPORT_MULTI_DISC_RESUME) && defined(CLEAR_RESUME_WHEN_STOP)

    OSD_RemoveAllUI(); //CoCo1.05

    // Micky0.72, must put after OSD_Remove UI, as OSD need to reference some info.
    // clear all UI.
    OSD_NormalMode();
    // wyc1.02a-909, need to clear PIP when open/close or power down and move here after remove UI.
    DISP_DisplayCtrl(DISP_PIP, FALSE);

    // wyc1.11a, need to force disp to complete the effect here because we want to show LOGO later. Otherwise we will see LOGO
    // abnormal after call UTL_ShowLOGO.
    // wyc1.50-909, replace by function.
    MM_CompleteJPGEffect(TRUE);

    // DVD_273.2, for encrpyted DVD begin
    // clear the auto scan next cell's command.
    __bAutoScans = 0;
    // DVD_273.2, for encrpyted DVD end

    //Kevin1.26, compensate A/V buffer remainder for power off-> power on resume or DVD->STB->DVD resume
    // LLY1.00, don't to get resume time here since the parser has already stopped now
    // So, the reported sector is track end.
    // Notice: must add the corresponding flow within IR power off if we remove these flow.
#if 0    
    if (!(__wDiscType & CDROM_M1))
    {
        UTL_GetCurrentTime(); 
        __dwTemp = UTL_EstimateBufferTime();
        __dwResumeTime = __dwTimeNow - __dwTemp; 
#ifdef SUPPORT_PRINTF        
        printf("__dwResumeTime=%lx, __dwTimeNow=%lx, buffer rem=%lx\n", __dwResumeTime, __dwTimeNow, __dwTemp); 
#endif
    }
#endif  // #if 0    

// Micky1.06, remove unnecessary codes, as will be cleared within INITIAL_MediaSetting
/*
    // Micky1.10, support resume after power on for DVD/CDDA/VCD
    // must keep the __dwResumeTime value
    // Ming0.80, support multi-disc resume.
#if defined(SUPPORT_RESUME_AFTER_POWERON) || defined(SUPPORT_RESUME_AFTER_OPENCLOSE)
    if ( !( __bFlowState & FLOW_KEEP_RESUME ) )
#endif  // #if defined(SUPPORT_RESUME_AFTER_POWERON) || defined(SUPPORT_RESUME_AFTER_OPENCLOSE)
        __dwResumeTime=NULL_0;
*/        

    __btNewTime=FALSE;
    // Must clear selection message
    OSD_OUTPUT_MACRO ( MSG_SELECTIONLIST, 0xFFFF,  0 );

// clear MP3 menu
#ifndef NO_MP3
    if (__wDiscType & CDROM_M1)
    {
        __bMMMenuID=MM_MENU_NONE;
        // wyc1.07, ckear this variable when reset state
        // wyc0.80, remove define & variable __bMMHide
        // Micky0.60-909, modify for CT909 OSD
        //OSDMM_Update(OSDMM_MSG_EXIT,0);
    }
#endif  //#ifndef NO_MP3

// must clear it before ACTION_STOP.
// Can't directly move INITIAL_Variables here, as it wil clear the variables(DiscType)
// that ACTION_STOP might use.
// Reset as real stop
// Micky2.79, tune the usage of __bCountStop, don't reference DOUBLE_STOP
    __bCountStop=2;

// let DVD enter STOP state when power down tray
// for restore parental level
#ifndef    NO_DISC_MODE
            NV_Action ( ACTION_STOP, NULL_0 );  // ** TCH0.37; Let NV stay on STOP mode
#endif    // NO_DISC_MODE

// LLY0.95, base on the flag to clear the disc resume info while fully stop mode
// Notice: Can't put this procedure before NV_Action(ACTION_STOP, NULL)
//         Because, the resume information is not ready.
#if defined(SUPPORT_MULTI_DISC_RESUME) && defined(CLEAR_RESUME_WHEN_STOP)
    if(bResumeInfoState==CLEAR_INFO)
    {
        _Save_Resume_Information(CLEAR_INFO);
        _bAPPAction &= ~CC_ACTION_SAVE_RESUME;
    }
#endif  // #if defined(SUPPORT_MULTI_DISC_RESUME) && defined(CLEAR_RESUME_WHEN_STOP)



// Ming0.80, support multi-disc resume.
#ifdef SUPPORT_MULTI_DISC_RESUME
    if( _bAPPAction & CC_ACTION_SAVE_RESUME )
    {
        _bAPPAction &= ~CC_ACTION_SAVE_RESUME ;
        _Save_Resume_Information(SAVE_INFO);
    }
#endif // #ifdef SUPPORT_MULTI_DISC_RESUME

// Reset all variables
    INITIAL_Variables ();

#ifdef  SUPPORT_GENERAL_BACKDOOR
    // initialize backdoor key index whenever open/close
    // initial each backdoor index
    for (__bTemp=0;__bTemp<BACKDOOR_NUMBER;__bTemp++)
    {
        __bKeyIndex[__bTemp]=0;
    }
#endif
    // DVD_2.74Micky, Reference V_4-197,
    // restore SPRMs[13] when initial access and when the play transfer to stop state
    // initial access(after loading disc or power up)
    // so, at open or power down, restore SPRM13.
    // to support query password, title "Cruel Intention"
#ifndef NO_DISC_MODE //CoCo2.37p
    if ( __wStoreSPRM13 )
    {
        // high 8 bits is the original parental value
        __wSPRMs [13] = __wStoreSPRM13 >> 8;
        __wStoreSPRM13=0x0000;
    }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

    // Brian1.25, Check font table, if incorrect, reload it.
#ifdef SUPPORT_FONT_TABLE_CHECKSUM
{
    extern WORD __wFontableChecksum;
    
    // check if need re-load font atble
    __wTemp = OSD_FontableChecksum();
    if (__wTemp != __wFontableChecksum)
    {
        OSD_LoadFontTable(FONTABLE_OSD);

    }
}
#endif

    // Micky1.00_909_media
#ifdef  SUPPORT_MEDIA_MANAGEMENT
    if (bState == KEY_POWER)
    {
        MEDIA_StateChange( MEDIA_CONTROL_CLEAR_ALL );
        MEDIA_InitialState ();
    }
#endif  //#ifdef  SUPPORT_MEDIA_MANAGEMENT
    // Micky0.90a
    __bSP_Disp_OFF = 0;
    // wyc1.02-909, need to initial _bCDGState to CDG_STATE_DISC_DETECTED, otherwise CDG will detect failed after open/close.
    // because  _bCDGState value will keep in CDG_STATE_AV_SYNC after open/close if no initial it and will make CDG_Trigger 
    // actived in next CDG detection and hang in wait VD FIFO function.
#ifndef NO_CDG 
    _bCDGState = CDG_STATE_DISC_DETECTED;
#endif //
}
// DVD_172Micky, FLOW_CONTROL begin
// let __bFlowState be general use for auto play.
// DVD_161Micky, support Loading-> Setup-> Exit, will continue loading action
//  *********************************************************************
//  Function    :   _ProceedFlowState()
//  Description :   It will check where the flow is interupted, then continue to go
//  Arguments   :   __bFlowState, record the state
//  Return      :   TRUE, always successful
//  Side Effect :
//  *********************************************************************
///#ifdef  SUPPORT_LOADING_SETUP
BYTE    _ProceedFlowState()
{
// Micky0.65-909, must show disc type first then exit.
/*
    // Micky1.25-2 fix OSD show DVD-> Setup change region code-> OSD show invalid region, but flow still going
    if (__bDiscInfo & DISCINFO_UNPLAYABLE)
    {
        __bFlowState = FLOW_NORMAL;
        return FALSE;
    }
*/    

    // Micky1.00_909_media
    if (__bFlowState & FLOW_RELEASE_DETECTION)
    {
        INITIAL_CheckServo();

         __bFlowState &= ~FLOW_RELEASE_DETECTION;
        // Micky0.65-909, must show disc type first then exit.
        // Micky1.25-2 fix OSD show DVD-> Setup change region code-> OSD show invalid region, but flow still going
        if (__bDiscInfo & DISCINFO_UNPLAYABLE)
        {
            __bFlowState = FLOW_NORMAL;
            return FALSE;
        }
         
       // issue auto play key
        _CC_ImmediateKey();
    }

    // check if need do auto play
    if (__bFlowState & FLOW_RELEASE_PLAY)
    {
        __bFlowState &= ~FLOW_RELEASE_PLAY;

        // DVD_172Micky, FLOW_CONTROL begin
///        _CC_ImmediateKey();
        // just issue __bISRKey = KEY_PLAY.
        if (!__btPlaying)
        {
// DVD_172Micky, FLOW_CONTROL(*** RESUME_AFTER_POWERON ***)
// after I/R power on, keep resume.
// Ming0.80, support multi-disc resume.
// Micky1.06, reference SUPPORT_MULTI_DISC_RESUME        
//#if defined(SUPPORT_RESUME_AFTER_POWERON) || defined(SUPPORT_RESUME_AFTER_OPENCLOSE)
#if defined(SUPPORT_MULTI_DISC_RESUME) //||  defined(SUPPORT_STB)
            if (__bFlowState & FLOW_KEEP_RESUME)
            {
                __bFlowState &= ~FLOW_KEEP_RESUME;
                __bFWKey = KEY_RESUME;
                __bKeyMessage=MSG_RESUME;
                return TRUE;
            }
#endif  // #if defined(SUPPORT_MULTI_DISC_RESUME)
            __bFWKey = KEY_PLAY;
            __bModeCmd = KEY_STOP;

// Micky1.05, add auto test for IC sorting by CoCo
#ifdef AUTO_TEST
            __bAutoTest = TRUE;
            __dwAutoTestTime = OS_GetSysTimer();
#endif

            // DVD_172Micky, FLOW_CONTROL begin
            // move it from initial.c _Check_CDROMStatus.
            // should do it till start play.
#ifndef NO_DISC_MODE //CoCo2.37p
            if (__wDiscType & BOOK_DVD)
            {
// Micky2.79, support DVD default PBC off.
#ifdef  DEFAULT_NOPBC
                if ( !NV_TitlePlay (1, 1) )
                    __bFWKey = KEY_STOP;
#else
//Aron1.06-909, Support Smart Play feature
//Aron1.11-909, when setup enables smart play, it will play the smart play title
//on startup
#if defined(SUPPORT_DVD_SMRTPB) && defined(SUPPORT_DVD_SMRTPB_SETUP)
                if ( (__SetupInfo.bSmartPlay != SETUP_CUSTOM_SMART_PLAY_OFF) && __bSmartPB_ABLE )
                {
                    if ( NV_TitlePlay(__bMaxPBT_TT, 1))
                    {
                        SMAN_PushStateInfo ( STATE_PG, STATUS_READY );
                    }
                    else
                    {
                         __bFWKey = KEY_STOP;
                    }
                }
                else
#endif //#ifdef SUPPORT_DVD_SMRTPB                   
                {
                if ( ! NV_TitlePlay ( NULL_0, NULL_0 ) )
                {
// Micky1.22a, if no FP_PGC, just play from TT/CC 1/1
#ifdef  PLAY_TT1CC1_WHEN_NO_FPPGC
                    if ( ! NV_TitlePlay ( 1, 1 ) )
                        __bFWKey= KEY_STOP;        // Force enter STOP Mode
#else
                    // ** TCH0.40; begin... DVD-Forum Testing Title has such status
                    if ( ! NV_MenuCall ( MENU_TITLE ) )
                        if ( ! NV_MenuCall ( MENU_ROOT ) )
                            __bFWKey= KEY_STOP;        // Force enter STOP Mode
#endif
#ifdef    _DUMP_CDINFO
                    PRINTF ( "**** No FP_PGC, Just Play TT/CC 1/1 ..." );
                    getch ();
#endif    // _DUMP_CDINFO
                    // ** TCH0.40; end...
                }   // if (!NV_TitlePlay()
                }                
#endif  //#ifdef  DEFAULT_NOPBC
            }   // if (__wDiscType & BOOK_DVD)
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
            // DVD_172Micky, FLOW_CONTROL end
        }
        __bFlowState = FLOW_NORMAL;
        // DVD_172Micky, FLOW_CONTROL end
    }

    return TRUE;
}
///#endif
// DVD_172Micky, FLOW_CONTROL end
// ** TCH1.21-1; begin... 
void _CC_Variables_PowerON (void)   
{
    {   // [CC] Module
        // DVD_273.2, for encrpyted DVD begin
        // clear the auto scan next cell's command.
        extern  BYTE    __bAutoScans;
        __bAutoScans = 0;

        // DVD_172Micky, FLOW_CONTROL begin
        __bFlowState = FLOW_NORMAL;

        // LLY.276-1, moved from CC.C
#ifndef NO_SCREEN_SAVER
        __bScreenSaver=NONE_SCREEN_SAVER;
#endif  // #ifndef NO_SCREEN_SAVER

        // LLY.172q, point type casting (ARM error)
        __pbBuffer= (BYTE *)__cBuffer;    // ** TCH1.60a-2;

        __dwCountSystem = 0;
        __bKey = KEY_NO_KEY;
        __dwTimeDiscBegin = NULL_0;
        __dwTimeDiscEnd = NULL_0;
        __btDriveOpen = FALSE;
        __btNewTime = FALSE;
        __btPowerDown = FALSE;
        __bKeyMessage= MSG_NULL;
        __bKeySource = KEY_FROM_NONE;
        _dwTimeCheckTray = 0;
        __btPlayEnd = TRUE;
// Micky1.23 support resume info from EEPROM
        _bAPPAction = CC_ACTION_NONE;

        // Micky1.24, initial angle number as none.
        // Fix angle marke may flash when H/W power on 
        __bAGLN = 0;    // Angle Number. from "0".
        _bAVIScan2Play = FALSE;
        _dwForceStopMode = FALSE;
        // Micky1.00_909
        MEDIA_InitialState();

    }
}

// Micky2.81, Provide an API for access __bMessageMap
void _CC_MapKeyMessage(BYTE bKey)
{
    // Micky0.80, limit to Group2~10 
    // for 0.80, should keep group3 only. add next/prev extra.
#ifdef  MAP_PLAYABLE_MESSAGE_ONLY    
//    if ( (bKey >= INPUT_KEY_GROUP3) &&  (bKey < INPUT_KEY_GROUP4)  )
    // Micky0.80a, next/prev must do message map.
    if ( (bKey >= INPUT_KEY_GROUP3) &&  (bKey <= KEY_PREV)  )
    
    {
        __bKeyMessage=_bMessageMap[bKey-INPUT_KEY_GROUP3];
    }       
#else
    if ( (bKey >= INPUT_KEY_GROUP2) &&  (bKey < INPUT_KEY_GROUP11)  )
    {
        __bKeyMessage=_bMessageMap[bKey-INPUT_KEY_GROUP2];
    }                
#endif
    else
    {
        __bKeyMessage = MSG_NULL;
    }
}

BYTE    CC_MainProcessKey(void)
{
#ifndef NO_DISC_MODE //CoCo2.37p
    extern BYTE __bPTLMsg;
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

    BYTE    bValidKey=KEY_ACCEPT;

// Top priority keys no matter what modes
// _CC_CheckValidKey
//      - KEY_POWER/ KEY_OPEN_CLOSE, only allow keys...
//      - If no valid disc, do not allow keys...

// XXX_ProcessKey
//  - If not at its mode, only process the KEY_XXX that can enter the mode.
//  - If at its mode, only process the necessary keys.
// CC_NumberProcessKey
    //kevin0.85, workaround for IR hang
    // wyc1.05-909, remove IR reset mechanism to check AC IR status.
    /*#define RESET_IR_PERIOD     3 //reset IR every x sec    
    static DWORD _dwTimerCount_ResetIR;
    if ( (UTL_GetSysTimer () > _dwTimerCount_ResetIR) )
    {
        extern void _ResetIR(void);          
        //printf("reset IR");        
        _ResetIR();                                    
        _dwTimerCount_ResetIR = UTL_GetSysTimer() + COUNT_3_SEC;    
    } */
#ifdef SUPPORT_POWER_SWITCH
    if ((__bISRKey == KEY_POWER)&&(__bPowerSw))  
    {
        __bISRKey = KEY_NO_KEY;        
    }
#endif //SUPPORT_POWER_SWITCH
    // LLY0.62, need to process the input key while IR key or F/W key exist
    if( __bISRKey != KEY_NO_KEY || __bFWKey != KEY_NO_KEY)
    {
        // LLY0.62, collect all procedure for decide the final processing key, key source ... into the API
        // And, clear the un-reference key variable
        // Notice: F/W key is high priority than ISR key

        _CC_AssignActionKey();

        bValidKey=KEY_ACCEPT;

    }   // if ( __bISRKey != KEY_NO_KEY )


    // DVD_172Micky, just process the key, no matter it's from ISR of F/W.
    if ( __bKey != KEY_NO_KEY )
    {
// Only key from IR need do key transfer
        if (__bKeySource==KEY_FROM_ISR)
        {
            OSDSS_Reset(); //CoCo0.80, reset screen saver

#ifdef CT950_STYLE //CoCo2.38
            ALARM_Stop();
#endif //CT950_STYLE

        // wyc0.95, show input key by Andy's request.
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINT_KEY,"Input Key=%hx!",__bKey);
/// 3.1) When key is from IR, will transfer the key to proper value, 
///      and do OSD message map.
            // Can transfer the KEY to the actual operation KEY
            bValidKey = _CC_CheckValidKey();
            // Micky0.76, only do key message mapping from INPUT_KEY_GROUP2
            // composite keys are not included.
            // Micky 0.80, call function to do the mapping.
            _CC_MapKeyMessage(__bKey);

        }       // if (__bKeySource==KEY_FROM_ISR)

        // DVD_101bMicky, reference KEY_ACCEPT/REJECT/IGNORE
        if ( bValidKey == KEY_DISALLOW)
        {
            // DVD_172Micky, let it show invalid.Same flow as after CC_DoKeyAction
            __bTemp = INVALID_KEY;
        }
        else if ( bValidKey==KEY_ACCEPT)
        {
            // Micky0.80, use function array for ProcessKey
#if 1
            BYTE bFuncLoop;
            for (bFuncLoop = 0; bFuncLoop < bMaxFunctions; bFuncLoop++)
            {
                __bTemp = FuncArray[bFuncLoop]();//__bKey);
                if ( __bTemp != KEY_BYPASS )
                {
                    // Micky0.80a, must clear the key value.
                    if ( __bTemp == KEY_NO_KEY)
                    {
                        __bKey = KEY_NO_KEY;
                    }
                    break;
                }
            }                
            if ( (__bTemp == INVALID_KEY) || (__bTemp == KEY_NO_KEY) )
            {
                goto LABEL_KEY_COMPLETE;
            }
            else if (__bTemp == KEY_TODO)
            {
                goto LABEL_KEY_TODO;
            }

            if ( __bKeySource == KEY_FROM_ISR )
            {
                if ( _CC_PreProcessKey() == KEY_DISALLOW )      // KEY_ACCEPT/ KEY_DISALLOW
                {
                    __bTemp = INVALID_KEY;
                    goto LABEL_KEY_COMPLETE;
                }
                else
                {
                    // Micky 0.80, do the final key message map.
                    _CC_MapKeyMessage(__bKey);
                }
            }                
            
LABEL_KEY_TODO:
            __bTemp = CC_DoKeyAction ( __bKey );
            goto LABEL_KEY_COMPLETE; 
#else       
#ifdef  SUPPORT_PSCAN_PROMPT_OSD
//     use OSDPROMPT_ProcessKey function to get the key
            __bKey = OSDPROMPT_ProcessKey(__bKey);

#endif // SUPPORT_PSCAN_PROMPT_OSD
            // Micky2.81, re-arrange the sequence of XXX_ProcessKey and parameter
#ifndef NO_SETUP
            __bKey = SETUP_ProcessKey(__bKey);

#endif  // ifndef NO_SETUP

            // Micky1.00_909
            __bKey = MEDIA_MediaSelection_ProcessKey(__bKey);
// Micky0.65-909

#ifndef NO_PROGRAM_UI //++CoCo2.37p
            if ( (__bKey == KEY_PROGRAM) || (__btInputProgram) )
            {
                __bKey=OSDPRGM_ProcessKey(__bKey);
            }
#endif //#ifndef NO_PROGRAM_UI //++CoCo2.37p
            
#ifndef NO_SEARCH_UI //++CoCo2.37p
            // Micky0.62-909
            // if ProcessKey has done the check, will remove the check.
            if ( (__bKey == KEY_SEARCH) ||( __bSearchMode != SEARCH_MODE_OFF) )
            {
                __bKey = OSDSRCH_ProcessKey(__bKey); 
            }
#endif //#ifndef NO_SEARCH_UI //++CoCo2.37p

#ifdef SUPPORT_BOOKMARK
            if ( (__bKey == KEY_BOOKMARK) || (__btBookMark))
            {
                __bKey = OSDBOOKMARK_ProcessKey(__bKey);
            }
#endif  //#ifdef SUPPORT_BOOKMARK

            // Micky0.62-909, add number process key.
            // It only processed the normal mode number key
            // PG/Search/ Bookmark ProcessKey must before the codes.
            __bKey = _CC_NumberProcessKey( __bKey );

#ifndef NO_DIGEST
            // Micky0.68, support DIGETS key.
            __bKey = DIGEST_ProcessKey(__bKey);
#endif  //#ifndef   NO_DOIGEST
            if (__bThumbnailMode)
                __bKey = THUMB_ProcessKey(__bKey); 

#ifndef NO_MP3
            if (__wDiscType & CDROM_M1)
            {
                __bKey =  MM_ProcessKey(__bKey);
            }
#endif
            // Micky0.80, only DVD disc will have navigation display
            // put by the OSD layer architecture
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
            __bKey = OSDDSPLY_ProcessKey(__bKey); //CoCo.CT909
#endif //SUPPORT_NAVIGATOR

#ifdef  SHOW_IR_KEY_OSD_ONLY
        if ( __bKeySource == KEY_FROM_ISR )
        {
            BYTE _CC_PreProcessKey(void);
            // what about key_n0->key_number->kEY_Play, don't need show the play message.
            // Micky0.76, for Media manager or any UI mode, don't do the convert.
            if  ( (__bNavigateMode == DISC_NAVIGATE_MODE ) && 
            !__btInputProgram && (__bSearchMode == SEARCH_MODE_OFF) )
            {
                bValidKey = _CC_PreProcessKey();
                // Micky0.76, mapping message again.
            if ((__bKey >= INPUT_KEY_GROUP2) )
            {
                __bKeyMessage=_bMessageMap[__bKey-INPUT_KEY_GROUP2];
#ifdef  SAME_FAST_AS_SCF2   // must use as KEY_SCF
            if (__bKey==KEY_FF)
                __bKeyMessage=_bMessageMap[KEY_SCF-INPUT_KEY_GROUP2];
#endif  //#ifdef  SAME_FAST_AS_SCF2   // must use as KEY_SCF
            }
            }
            // Micky0.76, only do key message earlier
/*            
            // Micky2.81, only do key message mapping from INPUT_KEY_GROUP2
            // composite keys are not included.
            if ((__bKey >= INPUT_KEY_GROUP2) )
            {
                __bKeyMessage=_bMessageMap[__bKey-INPUT_KEY_GROUP2];
#ifdef  SAME_FAST_AS_SCF2   // must use as KEY_SCF
            if (__bKey==KEY_FF)
                __bKeyMessage=_bMessageMap[KEY_SCF-INPUT_KEY_GROUP2];
#endif  //#ifdef  SAME_FAST_AS_SCF2   // must use as KEY_SCF
            }
*/            
        }
#else   //#ifndef  SHOW_IR_KEY_OSD_ONLY
            __bKeyMessage=MSG_ALL;
#endif  //#ifndef  SHOW_IR_KEY_OSD_ONLY
            
        bValidKey = KEY_ACCEPT;        

            if (bValidKey == KEY_ACCEPT)
            {
#ifndef NO_DISC_MODE //CoCo2.37p
            // Micky0.62-909, add VCD/DVD Process Key
            __bKey =  CC_DVD_ProcessKey(__bKey);

#endif //#ifndef NO_DISC_MODE //CoCo2.37p

#ifndef NO_DISC_MODE //++CoCo2.37p
            __bKey =  CC_VCD_ProcessKey(__bKey);
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p

            __bTemp = CC_DoKeyAction ( __bKey );
            }
            else if (bValidKey == KEY_DISALLOW)
            {
                __bTemp = INVALID_KEY;
            }
            else
            {
                __bTemp = KEY_NO_KEY;
            }
#endif
        }   // if ( bValidKey==KEY_ACCEPT)
// Micky0.80, KEY status process
LABEL_KEY_COMPLETE:
        // ** TCH1.00; begin... Must put here, let other module can use __bKeySource to decide ISR or FW.
        // clear the Key source
        __bKeySource = KEY_FROM_NONE;
        // ** TCH1.00; end...

        // DVD_172Micky, must be done right after CC_DoKeyAction as reference __bTemp
        // the return value
        // false, key is not processed, we can just clear the key.
        // true, key is processed
        // INVALID_KEY, KEY_NO_KEY
        // KEY_STOP is processed.
        if ( __bTemp == INVALID_KEY )
        {
            //Aron2.36,It means the INVALID_KEY is caused bu parental error
            //Add it to prevent showing invaild key when it is caused by parental error
#ifndef NO_DISC_MODE //CoCo2.37p
            if(__bPTLMsg) 
            {
                __bPTLMsg = FALSE;
            }
            else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
            {
                OSD_Output(MSG_INVALID, OSD_ERR_KEY, 1 );
            }
        }
        else        // ** TCH0.452; Add for "Gloden Eye". Need Micky confirm
        // Micky0.87, fix SCB to track begin will show LOGO first.
        // as it will cal CC_DoKeyAction(KEY_STOP)
        // move the show LOGO action to outside of CC_DoKeyAction
        // as the function may be called, but don't need show LOGO.
        if (__bKey==KEY_STOP)
        {
        // DVD_172Micky, FLOW_CONTROL begin
        // when STOP key is processed, cancel any auto play.
        __bFlowState &= ~FLOW_RELEASE_PLAY;
        // DVD_172Micky, FLOW_CONTROL end

        // Now play the CD-DA track of VCD2.0
        // still hard to control not re-draw LOGO
        //show LOGO here
//DVD_031, MP3
//MP3, don't show LOGO
#ifndef NO_MP3
        if ( !(__wDiscType & CDROM_M1) )
#endif
        {
            UTL_ShowLogo();
        }

        OSD_OUTPUT_MACRO ( MSG_STOP, NULL_0, 3 );
        }

        // Micky0.87, move to after MSG_STOP.
        __bKeyMessage = MSG_NULL;

// this flow has problem when DVD-> auto play end(__bISRKey=KEY_STOP from F/W),
// but soon KEY_STOP is issued from IR. It will become pre-stop.(DOUBLE_STOP case)
        if (__btPlaying 
// Micky1.20, 4M flash
#ifndef NO_DIGEST
            || __btDIGEST
#endif
            )
        {
            __bCountStop=0;
        }

        // Micky1.00_909
#ifdef  AUTO_POWERDOWN
        _CC_Trigger_AutoPowerDown();
#endif
        __bKey= KEY_NO_KEY;

    }       // if ( __bKey != KEY_NO_KEY )
    return TRUE;
}
// LLY0.62 create,
//  **************************************************************************
//  Function    :   _CC_AssignActionKey
//  Description :   Assign the action key while receive any IR or F/W key
//  Arguments   :   None
//  Return      :   None
//  Side Effect :   __bKey, __bISRKey, __bFWKey value will be modified
//  **************************************************************************
void _CC_AssignActionKey(void)
{   
// LLY1.00, it's un-necessary to keep if restore the disc resume information flag
// Otherwise, the disc resume can't work while enable "SUPPORT_RESUME_AFTER_POWERON"
// --> NPF --> IR power off/ on --> resume previous track --> open/close 
// Ming0.80, support multi-disc resume.
#if 0// def SUPPORT_MULTI_DISC_RESUME
    extern BYTE __fRestoreResumeInfo;

    __fRestoreResumeInfo = TRUE;
#endif
    // KEY_POWER or KEY_OPEN_CLOSE is 1st priority key
    // So, we can assume the key source is from ISR
    // And, direct to triggle it, cancel any key    
    if(__bISRKey==KEY_POWER || __bFWKey==KEY_POWER)
    {
        __bKeySource=KEY_FROM_ISR;
        __bKey=KEY_POWER;                
        __bISRKey=KEY_NO_KEY;
        __bFWKey=KEY_NO_KEY;
// Ming0.80, support multi-disc resume.
#if 0// def SUPPORT_MULTI_DISC_RESUME
#if !defined(SUPPORT_RESUME_AFTER_POWERON)
        __fRestoreResumeInfo = FALSE;
#endif  // #if !defined(SUPPORT_RESUME_AFTER_POWERON)
#endif  // #ifdef SUPPORT_MULTI_DISC_RESUME

    }
    else if(__bISRKey==KEY_OPEN_CLOSE || __bFWKey==KEY_OPEN_CLOSE)
    {
        __bKeySource=KEY_FROM_ISR;        
        __bKey=KEY_OPEN_CLOSE;                
        __bISRKey=KEY_NO_KEY;
        __bFWKey=KEY_NO_KEY;
// Ming0.80, support multi-disc resume.
#if 0//def SUPPORT_MULTI_DISC_RESUME
#if !defined(SUPPORT_RESUME_AFTER_OPENCLOSE)
        __fRestoreResumeInfo = FALSE;
#endif  // #if !defined(SUPPORT_RESUME_AFTER_OPENCLOSE)
#endif  // #ifdef SUPPORT_MULTI_DISC_RESUME
    }
    // Process F/W key first, then IR key
    else if(__bFWKey!=KEY_NO_KEY)
    {
        __bKeySource = KEY_FROM_FW;
        __bKey=__bFWKey;        
        __bFWKey=KEY_NO_KEY;
    }
    else if(__bISRKey!=KEY_NO_KEY)
    {
        __bKeySource=KEY_FROM_ISR;
        __bKey=__bISRKey;        
        __bISRKey=KEY_NO_KEY;
    }    
}

// LLY0.62 create for integate GoTime action ...
//  ********************************************************************************
//  Function    :   _CC_GoTimeAction
//  Description :   Issue GoTime related action: execute, calculate value
//  Argument    :   bAction, GOTIME_EXECUTE, GOTTIME_CALCULATE
//  Return      :   INVALID_KEY, the key is invalid
//                  :   TRUE, the key is valid
//  ********************************************************************************
BYTE _CC_GoTimeAction(BYTE bAction)
{    
    switch(bAction)
    {
    case    GOTIME_CALCULATE:
        // __dwGoToTime, for DVD, it's hh:mm:ss format
        // for other, it's sector format.
        // calculate __dwGoToTime value for DVD title
        if(__wDiscType & BOOK_DVD)
        {
#ifndef NO_DISC_MODE //CoCo2.37p
            __dwGoToTime=MAKE_TMSF(__bTimeHour, __bTimeMin, __bTimeSec, 0x0);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        }

        // LLY0.76, calculate the desired sector for CDROM A/V
        else if(__bAttrPlay & TYPE_CDROM_AV)
        {
            // LLY0.95, keep GoTime value in second unit for AVI file 
            if(__bAttrPlay == ATTR_AVI)
            {
                __dwGoToTime=__bTimeHour*3600+__bTimeMin*60+__bTimeSec;   
                // LLY2.36, calculate the corresponding STC value too.
                __dwGoTimeSTC = __dwGoToTime*90000;
            }
            else
            {
                __dwGoToTime = UTL_MStoSectors(__bTimeHour, __bTimeMin, __bTimeSec) + __dwTimeBegin;
            }
            //printf("GoTime: %lx\n", __dwGoToTime);
            
        }
        // LLY2.05, support CDROM/DVDROM audio only file GoTime action
        else if(__bAttrPlay & TYPE_CDROM_AUDIO)
        {
            DWORD   dwDecodedSector;
            
            // Assume the decoded sector base on desired GoTime value
            dwDecodedSector = UTL_MStoSectors(__bTimeHour, __bTimeMin, __bTimeSec);

            // LLY2.35, keep GoTime value in second unit for WMA file
            // Since, we must ask parser to get corresponding sector ID
            if(__bAttrPlay == ATTR_WMA)
            {
                __dwGoToTime=__bTimeHour*3600+__bTimeMin*60+__bTimeSec;
            }
            else // Others, plus track begin to get sector ID
            {
                __dwGoToTime = dwDecodedSector + __dwTimeBegin;
            }

            // Assume the decoded frame count base on decoded sector
            __dwGoTimeFrameCnt = UTL_SectorsToFrameCnt(dwDecodedSector);
        }
        else
        {
            __dwGoToTime= UTL_MStoSectors (__bTimeHour, __bTimeMin, __bTimeSec );
            // For playing mode, the time must relative to the track begin
// DVD_099bMicky, always use absolute time for GOTOTIME  when defined.
#ifndef ABSOLUTE_GOTOTIME
            if ( ( __bShowTimeType == MSG_RELTIME ) && (__btPlaying || __btPause) )
            {
                __dwGoToTime+=__dwTimeBegin;
                if (__dwGoToTime > __dwTimeEnd)     // over current track range
                    __dwGoToTime=__dwTimeDiscEnd;
            }
            else
#endif  // #ifndef ABSOLUTE_GOTITUME
            __dwGoToTime+=__dwTimeDiscBegin;
        }        
        break;

    case    GOTIME_EXECUTE:
        // wyc1.10-909, need to clear 0 here, otherwise press number key to select track will keep __dwNumberPrevious's value and make 
        // strack wrong selection.
        __dwNumberPrevious = 0;
        // Process DVD's goto time play
#ifndef NO_DISC_MODE //CoCo2.37p
        if(__wDiscType & BOOK_DVD)
        {
            // Micky0.95, move into navigate.c
            
            //DVD_049Micky
            // remove it, as now don't disable IR
            // EX0= 1;  // ***LLY.039, forget to enable IR interrupt

            //alex1.23,20040218 combine gototime , search, so remove __btGoToTime variable
            //__btGoToTime= FALSE; // clear input mode flag for goto time
            __bSearchMode=SEARCH_MODE_OFF; //xuli0106
            
            // DVD.039-1, check if out of range
            if( NV_TimePlay(__bTitle, __dwGoToTime) )
            {
                //Ming0.72, Move reset from _CC_GoTimeAction to cell.c.
                //          When found the destination VOBU.
                //          Will do the reset actions.
                //Aron1.20-909, constrained repeat, disable repeat mode when doing search
                __bRepeat= REPEAT_NONE;            
                OSD_OUTPUT_MACRO(MSG_REPEAT, __bRepeat, 0 );                
            }
            else
            {
                return INVALID_KEY;
            }
            // DVD.039-1 end..
        }
        else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        {
            // LLY0.95, CDROM A/V GoTime range is within current file
            // [1] AVI/ DivX/ WMA GoTime value is in sec unit -- LLY2.35
            // [2] Other AV motion GoTime value is in sector unit
            if(__bAttrPlay == ATTR_AVI || __bAttrPlay==ATTR_WMA)
            {
                if(__dwGoToTime >= __dwFileTime)
                {
                    return (INVALID_KEY);
                }
            }
            // LLY2.05, support CDROM/ DVDROM motion and audio only file GoTime action.
            else if( (__bAttrPlay & TYPE_CDROM_AV) || (__bAttrPlay & TYPE_CDROM_AUDIO) )
            {
                if(__dwGoToTime >= __dwTimeEnd)
                {
                    return (INVALID_KEY);
                }                
            }
            else
            // Micky1.25-2, fix VCD-> gototime out of range will hang
            // as SERVO_Seek will break current read command
            // check the range first, only withn range will do pre-start servo
            if ( __dwGoToTime >= (__dwTimeDiscEnd) )
            {
                return INVALID_KEY;
            }
            
            // LLY0.91, give the seek time near the desired position.
            //UTL_PreStartServo( __dwTimeNow);
            // LLY1.00, don't issue the pre-seek servo action here
            // Since the seek time maybe too long (ms ~ 3 sec)
            // And, F/W will issue stop decoder command @ _Play_TimeA()
            // So, we can see A/V will playback a moment after comfirm search action.
            //UTL_PreStartServo(__dwGoToTime);
            if ( ! LINEAR_Action(KEY_GOTOTIME_PLAY, __dwGoToTime) )
                return INVALID_KEY;
        }
        break;

    default:
        return INVALID_KEY;
    }

    return TRUE;
}
// Check high priority keys
// do 1st staget convert
BYTE    _CC_CheckValidKey(void)
{
// 1. KEY_POWER
    // Power key is valid in any time

    if ((__bKey==KEY_POWER)
#ifdef SUPPORT_AV_IN
        || (__bKey == KEY_SELECTMEDIA)
        || (__bKey==KEY_AV_IN)
#endif //
        ) //Jeff 20060705 Add for AV in
        return KEY_ACCEPT;

// 1.1 At Power dwon, only allow following keys:
    // KEY_POWER or
    // KEY_PLAY/KEY_OPEN_CLOSE(if defined)
// [POWERDOWN MODE], Only allow following keys to release
    // wyc2.78-909P, when usb is device mode and connected to PC, don't allow any key pressed.
    if ((__btPowerDown)
#ifdef SUPPORT_USB_HOST_DEVICE
            // wyc2.78-909P,
         || ((SrcFilter_GetOTGMode() == USB_MODE_DEVICE))
#endif //
)
    {
        // only DVD channel have the functions to release power down by key.
// DVD_101Micky, can select to use KEY_POWER or KEY_PLAY/OPEN_CLOSE release power down
#ifdef  RELEASE_POWERDOWN_BY_PLAY
        // wakeup by KEY_PLAY
        if ( (__bKey == KEY_PLAY) || (__bKey == KEY_PLAY_PAUSE) )
        {
            __bKey=KEY_POWER;
            return KEY_ACCEPT;
        }
#endif
#ifdef  RELEASE_POWERDOWN_BY_OPEN
        if ( __bKey==KEY_OPEN_CLOSE )
        {
            __bKey=KEY_POWER;
            __bFWKey=KEY_OPEN_CLOSE;
            return KEY_ACCEPT;
        }
#endif

#ifdef SUPPORT_TFT //kevin1.21, fix TFT key will not be processed on ext AV In 
        switch (__bKey)
        {
            case    KEY_PICTURE:                           
            case    KEY_LCD:
            case    KEY_UP:
            case    KEY_DOWN:
            case    KEY_LEFT:
            case    KEY_RIGHT:
            case    KEY_ENTER:
            case    KEY_PLAY:
            case    KEY_PLAY_PAUSE:  
            case    KEY_TEST1:
            case    KEY_TEST2:                               
                return  KEY_ACCEPT;                   
        }        
#endif 
        // All keys are ignored(don't process) when power down mode.
        __bKey=KEY_NO_KEY;
        return KEY_IGNORE;
    }
    // Micky1.20, enable TEST keys even no disc.(Group1)
    if ( __bKey >= INPUT_KEY_GROUP1 && __bKey < INPUT_KEY_GROUP2 )
        return KEY_ACCEPT;

//CoCo1.02, add OSD game from XuLi's code
#ifdef SUPPORT_OSDGAME
    if((__bKey == KEY_OSDGAME) || (__enGameID!=GAME_NONE))
    {
        return KEY_ACCEPT;
    }
#endif


    // Allow all keys when SETUP mode.
    // wyc1.05-909, brian's updated code.
    if ( __bSetupMode || __bMediaSelectUI) // Brian test
        return KEY_ACCEPT;       

// No disc, dis-allow most of keys.
//-------------------------------------------------------------------
// 2. NO_DISC_IN_DRIVE/ Unplayable disc will alow a few keys
// __wDiscType record NO_DISC_IN_DRIVE in the following 2 cases:
//      - when no disc in drive
//      - drive open
// [No Disc or Drive Open], Only allow following keys action

#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
    if ( (__wDiscType == NO_DISC_IN_DRIVE) ||
         (__bDiscInfo & DISCINFO_UNPLAYABLE) )
    {
        switch(__bKey)
        {
#ifdef SUPPORT_TFT //kevin1.20, fix TFT keys are disabled when no disc in drive or reading disc   
            case    KEY_PICTURE:                           
            case    KEY_LCD:                
            case    KEY_UP:
            case    KEY_DOWN:
            case    KEY_LEFT:
            case    KEY_RIGHT:
            case    KEY_ENTER:
            case    KEY_PLAY:
            case    KEY_PLAY_PAUSE:         
            case    KEY_TEST1: 
            case    KEY_TEST2:                    
                return  KEY_ACCEPT;                                       
#else 
            // the following keys are allowed when no disc in drive
            case    KEY_PLAY:
            case    KEY_PLAY_PAUSE:
#endif

//Alex1.07a,20031013 OSD_SHOW PSCAN PROMPT
#ifdef SUPPORT_PSCAN_PROMPT_OSD
                if ( __bOSDPromptMode)
                    return KEY_ACCEPT;
#endif //#ifdef SUPPORT_PSCAN_PROMPT_OSD

// Micky1.00, don't convert KEY_PLAY as KEY_OPEN_CLOSE for open tray by extra force
#ifndef SUPPORT_LOADER_OPEN_BY_EXTRA_FORCE
                {
                    extern BYTE __bCloseCommand ;
                    if ( !__bCloseCommand )
                    {
                        __bKey= KEY_OPEN_CLOSE;
                        return KEY_ACCEPT;
                    }
                }
#endif //SUPPORT_LOADER_OPEN_BY_EXTRA_FORCE

                if ( __btDriveOpen )
                {
                    __bKey=KEY_NO_KEY;
                    return KEY_IGNORE;
                }
                else
                    return KEY_DISALLOW;

            case    KEY_FUNCTION:
            case    KEY_VOL_UP:
            case    KEY_VOL_DOWN:
            case    KEY_DISPLAY:
            case    KEY_AUDIO_LR:
            case    KEY_MUTE:
            case    KEY_TVMODE:
            case    KEY_OPEN_CLOSE:
            // Below KEYs will trigger into special mode
            // must allow to be active always.
            case    KEY_SETUP:
            // Chuan0.84, accept KEY_PSCAN and KEY_VIDEO_OUTPUT_CTL
            case    KEY_PSCAN:
            case    KEY_VIDEO_OUTPUT_CTL:
            case    KEY_AP: // LLY1.50, allow KEY_AP to switch to AP mode

//Alex1.07a,20031013 OSD_SHOW PSCAN PROMPT 
#ifdef SUPPORT_PSCAN_PROMPT_OSD
            case    KEY_STOP:
                if ( __bOSDPromptMode)
                    return KEY_ACCEPT;
#endif //#ifdef SUPPORT_PSCAN_PROMPT_OSD

                    return  KEY_ACCEPT;
            default:
                    // DVD_101bMicky, integrate a general back door function
#ifdef  SUPPORT_GENERAL_BACKDOOR
                    if (__btDriveOpen)
                    {
                        void _GeneralBackDoor(BYTE bBackKey);
                        _GeneralBackDoor(__bKey);
                    }
#endif  //#ifdef  SUPPORT_GENERAL_BACKDOOR
// DVD_101bMicky, don't need check __bSetupMode.
                    return KEY_DISALLOW;
        }
    }
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p

// :: KEY_GOTOTIME/ KEY_DIGET/ KEY_NUMBER will auto turn PBC off
// :: KEY_NUMBER(number will process in _Process_KeyNumber)
//      - Only when PBC stop mode
#ifdef  AUTO_PBCOFF
#ifndef NO_DISC_MODE //++CoCo2.37p
    // when following keys are pressed, need turn PBC off
    if ( ( __bKey == KEY_SEARCH) || ( __bKey == KEY_DIGEST ) )
    {
        if ( __btPBC && ( __bModeCmd == KEY_STOP ) )
        {
            // just turn PBC off
            __btPBC=FALSE;
            OSD_OUTPUT_MACRO(MSG_PBC, FALSE, 0);
            return KEY_ACCEPT;
        }

    }
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
#endif  //#ifdef  AUTO_PBCOFF

    // LLY1.02, don't allow FF/ Slow/ SCF/ SCB/ Step/ Freeze key while DVD still mode
#ifndef NO_DISC_MODE //CoCo2.37p
    if(__wDiscType & BOOK_DVD)
    {
        HAL_ReadInfo(HAL_INFO_STILL, &__dwTemp);
//        printf("STILL info: %lx\n", __dwTemp);
        if(__dwTemp)
        {
            if((__bKey>=NONNPF_PLAYABLEKEY_BEGIN) && (__bKey<=NONNPF_PLAYABLEKEY_END) && (__bKey!=KEY_PAUSE) )
            {
                __bKey=KEY_NO_KEY;
                return (KEY_DISALLOW);
            }
        }
    }
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

    return KEY_ACCEPT;
}

// wyc1.02-909, add new function to process the action from scan to paly.
BYTE _CC_AVIScanToPlayProcess(void)
{
    // LLY1.21, keep gotime value for SCAN to PLAY mode here.
    HAL_ReadInfo(HAL_INFO_STC, &__dwSTCValue);

    // Notice: if previous SCAN not complete, cancel it directly
    // Otherwise, SCAN_DONE maybe complete before GoTime real start.
    // So, OSD maybe update a new time, and GoTime a previous position.
    if(__bScan == SCAN_GO)
    {
        // Must freeze the STC first before issue decoder stop
        DISP_STCFreeze(__dwSTCValue, DISP_FORCE_FREEZE_STC);

        // Stop decoder.
        HAL_PlayCommand(COMMAND_STOP, 0);

//        printf("Stop decoder\n");
    }
        
    // LLY2.36, it's un-necessary to plus 1 while SCAN back to NPF mode
    // Since, just go from current position.
    __dwGoToTime=(__dwSTCValue/90000); // +1;
    // LLY2.36, keep corresping STC value here to avoid STC/90K= sec, then *90K=STC again
    __dwGoTimeSTC = (__dwSTCValue);

    __btNewTime=TRUE;

    // LLY1.05, set the desired playback command for _Play_TimeA() reference
    __bModeCmd=__bKey;
    
    if (__bKey == KEY_PLAY)
    {
        __bKeyMessage=MSG_PLAYING;
        OSD_OUTPUT_MACRO(MSG_PLAYING, NULL_0, 0x3);
        __bKeyMessage=MSG_PLAYING;
        _bAVIScan2Play = TRUE;
        // wyc1.02a-909, when scan -> play, need to open SP when SP control is ON. Otherwise SP will be OFF after scan -> play.
        if ((__bSP_Disp_OFF == 0) || (__bSP_Disp_OFF&0x80))
        {
            DISP_DisplayCtrl(DISP_SP1, TRUE);
        }
        // wyc1.07-909, always parse SP bits when play mode.
        PARSER_ActionFilter( 0, PARSER_ENABLE_SP1, TRUE );
    }
    __bKey=KEY_NO_KEY;
    return KEY_IGNORE;
}

// wyc2.22-909s, new function to enter pre-stop mode.
//  *********************************************************************
//  Function    :   CC_ForceStop
//  Description :   This function will force system entering stop mode by different discs condition.
//  Arguments   :   none
//  Return      :   none
//  Side Effect :
//  *********************************************************************
BYTE CC_ForceStop(DWORD dwMode)
{
    // wyc2.32-909S, add define to reduce code size.
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
    __bTemp= UTL_CheckPlayItemMode ( __wPlayItem );
    /*if ((__bModeCmd == KEY_SCF) || (__bModeCmd == KEY_SCB) || (__bModeCmd == KEY_SF) || (__bModeCmd == KEY_STEP))
    return FALSE;
    else */
    if (__btPlaying || __btPause || __bThumbnailMode || ((__wDiscType & VCD_2_0 || __wDiscType & BOOK_2X)&&(__bTemp & MODE_STILL)&&(__bModeCmd != KEY_STOP))
#ifndef NO_DIGEST
        || __btDIGEST
#endif //
        )
    {
        _dwForceStopMode = dwMode;
        if (__bThumbnailMode)
        {
            __btPause = TRUE;
            UTL_TransferCommandID(KEY_PAUSE);
        }
        else
        {
#ifndef NO_DISC_MODE //CoCo2.37p
            if (__wDiscType & BOOK_DVD)
                NV_Action ( ACTION_STOP, NULL_0 );
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
            __bCountStop++;
            CC_KeyCommand(KEY_STOP);

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {                
                SrcFilter_StreamContinue(BG_ITEM_STREAM_ID);
            }
#endif //SUPPORT_JPEG_AUDIO_PLAYING
        }
        if (
#ifndef NO_DISC_MODE //CoCo2.37p
            (__wDiscType & BOOK_DVD) || 
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
            (__wDiscType & BOOK_VCD) || (__wDiscType & BOOK_2X))
            UTL_ShowLogo();
        if (__bAttrPlay == ATTR_JPG)
        {
            HALJPEG_Abort();
            DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
            OS_DelayTime(COUNT_50_MSEC);

			//CoCo2.77
			if (__bThumbnailMode == TRUE)
			{
				THUMB_ResetRetryTimes();
			}
        }
        return TRUE;
    }
#endif //
    return TRUE;
}

//  *********************************************************************
//  Function    :   CC_ForceResumeRePlay
//  Description :   This function will force system resume or re-play item depend on discs.
//  Arguments   :   none
//  Return      :   none
//  Side Effect :
//  *********************************************************************
void CC_ForceResumeRePlay(DWORD dwMode)
{
    // wyc2.32-909S, add define to reduce code size.
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
    // wyc2.22-909s, new function to resume system.
    if (_dwForceStopMode == dwMode)
    {
        if (__bThumbnailMode)
        {
            //MM_EnterThumbMode();
            CC_KeyCommand(KEY_PLAY);
        }
        // wyc2.36-909S, remove WMA checking code here to avoid system can't do WMA resume when enter/exit media select UI.
        else if ((__bAttrPlay == ATTR_JPG)// || (__bAttrPlay == ATTR_WMA)
#ifndef NO_CDG
            || (__bCDDA_DTS & CDDA_CDG)
#endif
            )
        {
            // wyc2.32-909S, when JPG decoding completed and then enter setup/media select mode, then should go next item when exit these UI.
            if ((__bAttrPlay == ATTR_JPG) && (__dwMMJPGDispOK == TRUE))// && (__wPlayItem < __pMMFilterFilesOfDir[__MMPlayingItem.wDirID]))
            {
                __bFWKey = KEY_NEXT;
            }
            else
            {
                // wyc2.30-909s, CDG need to call CC_KeyCommand(KEY_PLAY) to re-play, call UTL_PlayItem(__wPlayItem,0) in not enough.
                if (__bCDDA_DTS & CDDA_CDG)
                {
                    CC_KeyCommand(KEY_PLAY);
                }
                else
                {
                    UTL_PlayItem(__wPlayItem,0);
                }
                // wyc2.30-909s, set to play mode, otherwise JPG can't next after exiting media select UI.
                __bModeCmd = KEY_PLAY;
            }
        }
        else
        {
#ifndef NO_DISC_MODE //CoCo2.37p
            if (__wDiscType & BOOK_DVD)
            {
                NV_Action ( ACTION_RESUME, NULL_0 );
            }
            else
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
            {
#ifndef NO_DISC_MODE //++CoCo2.37p
                if (__btPBC)
                {
                    // Stop RESUME command if User choose ENDLIST.
                    if ( ! VCD20_IsValid_Command ( LIST_WORK ) )
                    {
                        return  ;
                    }

                    __wStatus= NULL_0;
                    if ( __wPlayItem >= 1000 )
                    {
                        BYTE    bTemp;

                        bTemp= UTL_CheckPlayItemMode ( __wPlayItem );
                        //Alan2.36, add the MODE_PLAYMOTION condition for Mantis bug #2307
                        if (bTemp & MODE_STILL || bTemp & MODE_PLAYMOTION)
                        {
#ifdef  NEW_PLAY_ITEM                
            _CC_PlayNewItem(0);
#endif
                            CC_KeyCommand(KEY_PLAY);
                            VCD20_ShowListType();
                            return;
                        }
                    }   // __wPlayItem >= 1000
                }
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
                __wPlayItem = LOWORD(__dwResumeItem);
                __dwGoToTime=__dwResumeTime;
                _CC_GoTimeAction(GOTIME_EXECUTE);
            }
        }
        _dwForceStopMode = FALSE;
    }
#endif //
}

// wyc2.32-909S, remove PRINTF_GENE function.

// wyc2.38a-909s, add function to parse *.AP file.
//  *********************************************************************
//  Function    :   CC_CompareAPFile
//  Description :   This function compare the *.AP file.
//  Arguments   :   none
//  Return      :   none
//  Side Effect :
//  *********************************************************************
BYTE CC_CompareAPFile(BYTE bMode, DWORD *dwFileLoc, DWORD *dwFileLength)
{
    // wyc2.55-909P-AP,
#ifdef SUPPORT_PARSE_ALL_AP_FILES    
    BYTE bRet;
    INFOFILTER_SetParseMode(IF_PARSE_AP_MODE);
    if (MM_CheckPlayableCDROM())
        bRet = TRUE;
    else
        bRet = FALSE;
    INFOFILTER_SetParseMode(IF_PARSE_FILE_MODE);
    return bRet;
#else
    if (bMode == IF_DISC_MODE)
    {
#ifndef NO_DISC_MODE
        if (CD_GetEntryByFileName(__csUPGAPName, "", dwFileLoc, dwFileLength))
            return TRUE;
        else
            return FALSE;
#endif //NO_DISC_MODE
    }
#ifdef SUPPORT_FAT_FILE_SYSTEM
    else if (bMode == IF_DISK_MODE)
    {
        if (FAT_GetEntryByFileName(__csUPGAPName, "", dwFileLoc, dwFileLength))
            return TRUE;
        else
            return FALSE;
    }
#endif //
    return FALSE;
#endif //
}

