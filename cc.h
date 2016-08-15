
/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved.

        Product : WinAV Firmware

        Date    : 1998.10.10.
        Author  : Cheertek (D300 TCH)
        Purpose : Control Center module
                  The moudle will control all operations
        Sources : cc.h/ cc.c

****************************************************************************/

#ifndef __CC_H__
#define __CC_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

// Micky1.07, reduce SRAM--_bVMG_PTL_LVLI 200 bytes.
#define REDUCE_SRAM_VMG_PTL_LVLI

//#define SUPPORT_CC_DEBUG_INFO

////////////////////////////////////////////
// argument parameters

#define     FAST_FACTOR         0       // 0:I,P; 1:I, 2:Other I
// wyc.162, moves these define from cc.c to cc.h
// DVD_099cMicky, add a flag for ISR Key
#define KEY_FROM_ISR 1      // the key is from ISR
#define KEY_FROM_FW 2       // the key is generate by F/W
// DVD_101bMicky, add one more key source for distinguish
// CC_DoKeyAction(KEY_STOP) is called directly or through __bISRKey
#define KEY_FROM_NONE   3

//DVD_039-1SCF
// don't need
//#define     SCAN_GAP            750     // SCAN Jump time: 10 seconds
//Kevin1.24, let the display jump time of CDDA SCF & SCB be more consistent
#define     SCB_EXTRA           75 //300    // SCAN Backward usage; More jump than Forward Jump
#define     PRACTICE_BACKGAP    225     // PRATICE back sectors
//DVD_039-1SCF
// the max. value can be 7 levels for scan
#define MAX_SCAN_LEVEL          5
//DVD_039-2SLOW
// the max. value can be 7 levels for slow
// wyc1.10-909, Kevin's update code.
// wyc2.17-909S, add different level for language study function.
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
#define MAX_LAN_STUDY_SLOW_LEVEL    2
#define MAX_SLOW_LEVEL              5
#elif (SLOW_MODE_CONFIG == NORMAL_SLOW_MODE)
#define MAX_SLOW_LEVEL              5
#elif (SLOW_MODE_CONFIG == LANGUAGE_STUDY_MODE)
#define MAX_SLOW_LEVEL              2
#endif //


// LLY.170, re-define some variable for program mode usage
// Notice:
// [1] the max of entry no for each row is 3
// [2] the max of row no for the program mode is 6
#define ENTRYNO_ROW         3  // the entry number of each row in program mode
#define ROWNO_PROGRAM       3  // the total row number in program mode
#define PROGRAM_TRACK   ENTRYNO_ROW*ROWNO_PROGRAM


#define     NORMALKEY_OFFSET    6       // the total key levels is 1+ 2*(NORMALKEY_OFFSET)

// SCAN usage
#define     SCAN_NONE           0       // NO SCAN mode
#define     SCAN_GO             1       // SCAN had done
#define     SCAN_END            2       // SCAN had got IFSC flag and wait for next SCAN command
// LLY1.02, add new state for SCAN mode
// SCAN Initial action, only used for motion SCAN mode
// Let 1st SCAN mode go the same path for following SCAN procedure.
// ie. don't issue scan first and let it go MONITOR_CheckBuffer() directly.
// since we may not find any I-frame during DivX file end.
// So, we can't get SCAN_DONE ack until file playend.
#define     SCAN_INIT           3       // SCAN initial action,

// The REPEAT function is disable when PBC is ON
#define     REPEAT_NONE         0               // Play the disc and stop
#define     REPEAT_TRACK        1               // Repeat play the track
// ** TCH0.25; begin...
#define     REPEAT_DISC         5               // Repeat play the disc
#define     REPEAT_ALLDISCS     6               // Repeat all discs (Multi-Disc)
#define     REPEAT_CHAPTER      2               // Repeat One Chapter
#define     REPEAT_TITLE        3               // Repeat One Title
// ** TCH0.25; end...
#define     REPEAT_FOLDER       4       // ** TCH2.81; Repeat one folder

#define     AUDIO_LR            0               // Audio is Stereo (LR)
#define     AUDIO_RL            1               // Audio is Stereo (RL)
#define     AUDIO_LL            2               // Audio is LL
#define     AUDIO_RR            3               // Audio is right
#define     AUDIO_MIXED         4               //kevin0.80
#define     AUDIO_3D            5               // 3D-Audio
#define     AUDIO_VOICEOFF      6
#define     AUDIO_STEREO        AUDIO_RL        // Audio is Stereo (LR)

// Micky0.68-909, enlarge bookmark from 6 to 20.
#define     BOOKMARK_NO         20       // BookMark capability

// Micky0.85, re-define the ID for CT908
// must by the sequence 25->50->100->150-> 200.
// as haljpeg will use the method > , < for different action
#define     VIEW_ZOOM25         1 	// JPEG, 25%
#define     VIEW_ZOOM50         2 	// JPEG, 50%
#define     VIEW_ZOOM100        3	// JPEG, 100%
#define     VIEW_ZOOM150        4	// JPEG, 150%
#define     VIEW_ZOOM200        5	// JPEG, 200%

// VIEW_NORMAL still define the "fit to screen" mode
#define     VIEW_NORMAL         VIEW_ZOOM100
#define     VIEW_ZOOMX2         10	// zoom x2
#define     VIEW_ZOOMX4         11	// zoom x4
// VIEW_ZOOMX1 is a pseudo ID, only for OSD reference.
#define     VIEW_ZOOMX1         12   // VCD/DVD fit to screen

// Micky2.80, integrate the ZOOM ID control.
// main/ HAL/ OSD all reference the set defined in CC.h
/*
#define     VIEW_NORMAL         0	// video fit to screen
#define     VIEW_ZOOMX2         1	// zoom x2
#define     VIEW_ZOOMX4         2	// zoom x4
#define     VIEW_ZOOM25         3 	// JPEG, 25%
#define     VIEW_ZOOM50         4 	// JPEG, 50%
#define     VIEW_ZOOM100        5	// JPEG, 100%
#define     VIEW_ZOOM150        6	// JPEG, 150%
#define     VIEW_ZOOM200        7	// JPEG, 200%
*/
// Reserve some IDs for ZOOMxxx. Max. is 19.
#define     VIEW_ZOOM_SUPPORT	20  // query if ZOOM support
#define     VIEW_ZOOM_LEFT	    21	// view zoom left
#define     VIEW_ZOOM_RIGHT	    22	// view zoom right
#define     VIEW_ZOOM_UP	    23	// view zoom up
#define     VIEW_ZOOM_DOWN      24	// view zoom down

// LLY.036, define for cursor position in search mode
#define     SEARCH_CURSOR_BEGIN     1
#define     SEARCH_CURSOR_TITLE     (SEARCH_CURSOR_BEGIN+0)
#define     SEARCH_CURSOR_CHAPTER   (SEARCH_CURSOR_BEGIN+1)
#define     SEARCH_CURSOR_TIME      (SEARCH_CURSOR_BEGIN+2)  // LLY.039
#define     SEARCH_CURSOR1_END      SEARCH_CURSOR_CHAPTER
#define     SEARCH_CURSOR2_END      SEARCH_CURSOR_TIME  // LLY.039

// LLY.036, define for search mode switch
#define     SEARCH_MODE_BEGIN           0
#define     SEARCH_MODE_OFF             (SEARCH_MODE_BEGIN+0)
#define     SEARCH_MODE_TITLE_CHAPTER   (SEARCH_MODE_BEGIN+1)
#define     SEARCH_MODE_TIME            (SEARCH_MODE_BEGIN+2)
//alex1.23,20040218 combine gototime , search
//combine search mode
/**********************************************************************/
#define     SEARCH_MODE_END             SEARCH_MODE_TIME
//#define     SEARCH_MODE_END             SEARCH_MODE_TITLE_CHAPTER

// wyc.277a-2-AVSys, new define follow mode for AUX channel. Update the sequence.
#define     AUX_CHANNEL_BEGIN           0
#define     AUX_CHANNEL_DVD             AUX_CHANNEL_BEGIN
#define     AUX_CHANNEL_TUNER           (AUX_CHANNEL_BEGIN + 1)
#define     AUX_CHANNEL_AV1             (AUX_CHANNEL_BEGIN + 2)
#define     AUX_CHANNEL_AV2             (AUX_CHANNEL_BEGIN + 3)
#define     AUX_CHANNEL_END             AUX_CHANNEL_AV2

// wyc2.22
#define     CC_FORCE_STOP_FROM_SETUP            0x1
#define     CC_FORCE_STOP_FROM_MEDIA_SELECT_UI  0x2
#define     CC_FORCE_STOP_FROM_PROGRAM          0x4
#define     CC_FORCE_STOP_FROM_BOOKMARK         0x8
#define     CC_FORCE_STOP_FROM_POWERON_MENU     0x10
#define     CC_FORCE_STOP_FROM_ALARM_MENU     0x20

// LLY.039, define the no of digit for goto time mode support
#define DISPTIME_NO             7

///////////////////////////////////////////
// Micky2.79, tune the usage of __bCountStop, don't reference DOUBLE_STOP
extern BYTE    __bCountStop;

// ** TCH0.50; begin...

// DVD_172r2, enlarge share buffer size.
// DVDINFO will use it as temp. buffer for DRAM moving.
// 192/4= 48 double words. 3 times read DRAM burst. call 4 times write DRAM burst.
//#define     LENGTH_SHAREBUFFER          160
#define     LENGTH_SHAREBUFFER          192
///#define     LENGTH_SHAREPOOL            8192
///#define     LENGTH_SHAREPOOL            9192    // ** TCH1.00b; size of (_cPGCI+_cVNG_PGCI_UT)
///#define     LENGTH_SHAREPOOL            14860   // ** TCH1.01b; add 720 VMG_PGCI_SRP (720* 8= 5760 bytes)
// DVD_172r2, reduce pool size.
// MP3 : 6838(6388+450)
// VCD : 6104(4992+600+500)
// DVD : 7926(5884 + 348 + 200 +  984 + 510)
///#define     LENGTH_SHAREPOOL            15900     // ** TCH1.71;
// DVD_276Micky, adjust the pool length
// to resere SRAM space for serial debug or 28C Combo
// the actual size DVD will use is 7926.

// Micky1.03, fix Matrix Region 2-> select play will re-loading.
// the total cells is 116. enlarge to 150 cells.(DVDINFO.c MAX_XELLS_WITHIN_PGC= 150)
// Micky1.07, reduce SRAM--_bVMG_PTL_LVLI 200 bytes.
// can set max. cells
// ** TCH0.60; begin...
/*
#ifdef  _RESERVE_SRAM
// Micky1.07, reduce SRAM--_bVMG_PTL_LVLI 200 bytes.
#ifdef  REDUCE_SRAM_VMG_PTL_LVLI
#define MAX_CELLS_WITHIN_PGC    255
#else
#define MAX_CELLS_WITHIN_PGC    150
#endif
#else
#define MAX_CELLS_WITHIN_PGC    255
#endif
*/

#define MAX_CELLS_WITHIN_PGC    255
// ** TCH0.60; end...
#define RESERVE_SRAM_BYTES      ((255-MAX_CELLS_WITHIN_PGC)*2)

/*
// Micky2.78+one_cpu
// reserve SRAM for serial debug and one_cpu
#ifdef  _RESERVE_SRAM
// DVDINFO.
// It may sacrifice some worst case titles(cells over 100).
// Micky0.91, release the reserve SRAM in PGCI part(200 bytes).
// MAX_CELLS_WITHIN_PGC(255-100), SIZE_PGCI(5884-200)
//#define RESERVE_SRAM_BYTES  (310+200)
// MAX_CELLS_WITHIN_PGC(255-100)
#define RESERVE_SRAM_BYTES  310
#else
#define RESERVE_SRAM_BYTES      0
#endif
*/

// Micky2.80, reduce VTSI from 984 to 540.
// DVD : 7482(5884 + 348 + 200 +  540 + 510)
// just cut 500 bytes.(444+76)
//#define     LENGTH_SHAREPOOL                (8000-RESERVE_SRAM_BYTES)
// Micky1.07, reduce SRAM--_bVMG_PTL_LVLI 200 bytes.
// DVD :7282(5884+348+540+510)
// VCD : 4992(CDDA: 4992)
// MP3 : 6008(JPEG: 6008)
#ifdef  REDUCE_SRAM_VMG_PTL_LVLI
// wyc2.16-909S, enlarge the pool when support UDF because UDF need more 120 bytes.
#ifdef SUPPORT_UDF_FILE_SYSTEM
#define     LENGTH_SHAREPOOL                (7950-RESERVE_SRAM_BYTES-200+120) // udf will use additional memory 120 bytes for PE.
#else
#define     LENGTH_SHAREPOOL                (7950-RESERVE_SRAM_BYTES-200)
#endif //
#else
#define     LENGTH_SHAREPOOL                (7950-RESERVE_SRAM_BYTES)
#endif

// TCC171, return value for auto-upgrade check
// Chuan 172 2002/02/21: Add some value to check.
#define FLASH_FAIL_NO_ERROR           0
#define FLASH_FAIL_OTHER_DISC         1
#define FLASH_FAIL_INVALID_DISC       2
#define FLASH_FAIL_USER_ABORT         3
#define FLASH_FAIL_CDROMDECODE        4    // Chuan 277 2002/09/12: Add CDROM DECODE FAIL STATUS
#define FLASH_AUTO_UPGRADE            5
#define FLASH_UPDATE_LOGO             6
#define FLASH_FAIL_CHECKSUM           7    // Chuan0.87-2th, Add CheckSUM Info.

extern  char        __cBuffer [];  // Max for SPPalette
extern  BYTE        * __pbBuffer; // Pointer for __cBuffer.   // ** TCH1.60a-2;
extern  BYTE        __bPool [];  // Common-Pool for DVD/ VCD    // ** TCH1.61;
extern  WORD        __wPoolOffset;  // record the current usage of Pool
extern  DWORD   __dwFWBufUsedSize;
// ** TCH0.50; end...
// **** TCH0.99E-XQ, Begin...
extern  BYTE        __bStillTime;   // record the Still Time (0: NONE, 0xFF: Infinite; Other: seconds.)
extern  BYTE            __bVOBUStill;    // flag indicate the VOBU still is on/off
// DVD_100Micky
extern BYTE    __bKeySource;
// wyc0.60, move to here from OSD
extern  BYTE    __bLOGO;

// f/w 0.34
extern BYTE *__bpDataPointer;
extern  BYTE           __bISRKey;              // The key value comes from Key-Scan or IR
extern BYTE                 __bFWKey;               // LLY0.62, the key valuef comes from F/W
extern  WORD                __wSCAN_GAP;
extern  DWORD               __dwDSPArg;
// global variables
// DVD_274pMicky, remove the __bSurround declaration.
//extern  BYTE                __bSurround;        // Record Surround kind
extern  BYTE                __bZoomFactor;

extern  WORD                __wMaxHStart;
extern  WORD                __wMaxVStart;
extern  DWORD               __dwCheckTime;  // record start time for time out
//DVD_051Micky, add byte size timer count
//extern  BYTE            __bSystemTimer;
extern  DWORD           __dwSystemTimer;

extern  DWORD           __dwCountSystem;
extern  WORD            __wDiscType;
extern  BYTE                __bDiscInfo;        // ** TCH1.70; Record Disc
// wyc1.25, add one variable to record flow when detected as wrong disc for debug usage,
extern  BYTE    __bTestFlow;

extern  DWORD               __dwTimeBegin;
extern  DWORD               __dwTimeEnd;
extern  DWORD               __dwPreGetTime;     // ** TCH0.31;
extern  DWORD               __dwTimeNow;
extern  BYTE                __bKey;
extern  BYTE                __bScan;

//extern  DWORD           __dwTimeRef;            // It's a time for reference
extern  DWORD           __dwTimeDiscBegin;
extern  DWORD           __dwTimeDiscEnd;
extern  DWORD           __dwTimeTorelence;
extern  DWORD           __dwVUBuffer;
extern  DWORD           __dwVOBuffer;
// Micky0.86, add audio buffer threshold value
extern  DWORD           __dwAUBuffer;
extern  DWORD           __dwAOBuffer;
extern  DWORD           __dwSTCValue;  // ** TCH0.99-1; Record Current STC Value
extern  DWORD           __dwTimeA, __dwTimeB;          // ** TCH1.61a; record the times for repeatAB

// LLY2.61, add new variable to keep A point frame count value for audio only file's repeat A-B usage
extern  DWORD           __dwTimeA_FrameCnt;

// wyc.simu909, move to cc.h
extern  BYTE    __bInvalidRegion;   // in cc.c
// wyc1.06-909,
extern BYTE __bActiveMessage;

extern  WORD            __wPlayItem;            //
extern WORD            __wBGPlayItem;

// DVD_274pMicky, remove the __bSurround declaration.
//extern  WORD            __wVolAudio;         // HIBYTE is right channel, LOBYTE is left channel
extern  WORD            __wStatus;                // record VCD 2.0 List type
// wyc2.22-909s,
extern DWORD   _dwForceStopMode;

//Alex1.21,20031210 support bookmark
#ifdef SUPPORT_BOOKMARK
extern BYTE             __bBookMark_Title[BOOKMARK_NO+ 1];        // record BookMark Title
#endif //#ifdef SUPPORT_BOOKMARK
extern  WORD            __wBookMark [BOOKMARK_NO+ 1];             // record BookMark Time
extern  BIT             __btBookMark;             // BookMark ON/OFF (PBC OFF)
void        _CC_ProcessBookMark (void);

//Kevin0.86, comment
///extern  WORD            __wProgramData [PROGRAM_TRACK];       // Max program is

extern  BYTE            __bModeCmd;         // Current macro command mode
extern  WORD            __wTrackBegin;
extern  WORD            __wTrackEnd;
extern  BYTE            __bRepeat;             // record repeat mode
extern  BYTE            __bError;
extern  BYTE            __bModeChannel;         // record channel information
extern  BYTE            __bAudioType;         // record Audio Type information    // ** TCH0.25;
// DVD_275b+, SUPPORT_VIRTUAL_DTS
#if 1//def  SUPPORT_VIRTUAL_DTS
extern  BYTE            __bSelectAudioType;
#endif  //#ifdef  SUPPORT_VIRTUAL_DTS

// DVD_274pMicky, remove the __cKeyShift declaration.
//extern  char            __cKeyShift;            // Key adjustment; Up/Down
// DVD_102pMicky, integrate __bTVMode with SetupInfo.bTVSystem
// remove __bTVMode
// extern  BYTE            __bTVMode;              // record TV mode MTSC/PAL/AUTO
extern  BYTE            __bRepeatAB;            // REPEAT_NONE/ REPEAT_A/ REPEAT_AB (PBC OFF)
extern  BYTE            __bShowTimeType;        // Output Module use. ref to osd.h

extern  BIT             __btPlaying;    // means playing (NPF/FF/...)
extern  BIT             __btPlayEnd;    // Check if it's into Play-End mode
extern  BIT             __btPause;      // if it's into Pause mode
extern  BIT             __btPBC;        // means VDC2.0 PBC
extern  BIT             __btDriveOpen;  // means drive is in close mode when Power-ON
extern  BYTE            __bFirstPlay;   // Does it play first time ?

extern  BIT             __btMute;               // Mute information
extern  BIT             __btCheckBitStream;     // Disable/Enable to check bitstream information
extern  BIT             __btJumping;            // Disable/Enable Jumping Now
extern  BIT             __btRandom;              // in random mode (PBC off)
extern  BIT             __btProgram;             // Program Mode ON/OFF (PBC OFF)

//alex1.23,20040218 combine gototime , search
//remove __btGotoTime declare
/*
extern  BIT             __btGoToTime;             // GoToTime ON/OFF (PBC OFF)
*/

// LLY.036, Search mode usage
extern  BYTE    __bSearchMode;    // Search mode on/off
extern  BYTE    __bSearchCursor;  // indicate the search cursor positon
extern  BYTE    __bSearchTitle;   // the title number for search mode
extern  WORD    __wSearchChapter;  // the chapter number for search mode
extern  WORD    __wSearchChapter_Ns_Title;  // the total chapter number for search

//extern  BIT             __btHighColorTrunc;
// DVD_104Micky, extend the vocal mode define.
// DVD_274pMicky, remove the __bSurround declaration.
//extern  BYTE            __bVocal;              // In Vocal Cancel mode
extern  BYTE            __bAutoVocal;          // Enable Auto-Sing
extern  BIT             __btAutoStop;

extern  BIT             __btOpenPlay;            // The CLOSE command is caused by "PLAY" key or NOT
extern  BIT             __btPowerDown;           // PowerDown Mode ON/OFF

// temporarily usage, Please free these variables when you leave one API
// Don't use it in parameter of functions
extern  DWORD      __dwTemp;
extern  WORD       __wTemp;
extern  BYTE       __bTemp;
extern  BIT             __btTemp;

extern  BIT             __btDIGEST;             // means Digest Mode

extern  BYTE     __bMMAtiveFrame; // LLY.274p-1

#ifdef  TEST
// Testing usage
extern  BYTE            __bTestMethod;
extern  BYTE            __bTest;
extern  BYTE            __bTestBlock;
extern  BYTE            __bBegin;
extern  BYTE            __bEnd;
extern  WORD            __wTest;
extern  WORD            __wTest1;
#endif  // TEST

// ** TCH0.26; begin...
extern  BYTE            __bASTN;   // Audio Stream Number
extern  BYTE            __bASTNO;    // Index of Audio Stream Number, from "0"-"__bASTNs-1".     // ** TCH0.451;
extern  BYTE            __bASTNs;    // Max. Audio Stream Number, from "0".
extern  BYTE            __bASTID;       // The Real sub_stream_id. DVD may be different than __bASTN.
// Micky0.90a
extern  BYTE            __bSP2_Disp_OFF;  // ** TCH0.402;  SP Display is disabled?
extern  BYTE            __bSP_Disp_OFF;  // ** TCH0.402;  SP Display is disabled?
extern  BYTE            __bSPSTN;
extern  BYTE            __bSPST2N;   // Aron1.01 SP Stream Number for SP2, from "0".
extern  BYTE            __bSPSTNO;    // Index of Audio Stream Number, from "0"-"__bSPSTNs-1".   // ** TCH0.451;
extern  BYTE            __bSPSTNs;   // Max. SP/OGT Stream Number, from "0".
extern  BYTE            __bSPSTID;      // The Real sub_stream_id. DVD may be different than __bSPSTN.

extern  BYTE            __bSPST2NO;    // Chuan0.75, Index of Subpicture Stream Number 2, from "0"-"__bSPSTNs-1"
extern  BYTE            __bSPST2ID;      // Chuan0.75, The Real sub_stream_id 2. DVD may be different than __bSPSTN.
// ** TCH0.26; end...

// ** TCH0.27; begin...
extern BYTE            __bAGLN;    // Angle Number. from "1".
extern BYTE            __bAGL_Ns;  // Number of Angles.
// ** TCH0.27; end...
//DVD_039-1SCF
extern  BYTE    __bScanLevel;

extern  DWORD      __dwTimeKey;
// Micky0.72, change number to DWORD to avoid overflow.
extern  DWORD            __dwNumberPrevious;
extern  DWORD           __dwResumeTime;
extern  BIT             __btNewTime;
extern  DWORD           _dwTimeCheckTray;
// wyc1.02-909, add extern variable.
extern DWORD __dwGoToTime;
// wyc2.05-909S, porting fish's code for MP3 resume.
// LLY2.05, porting Wing/ YC code to support MP3 disc resume
extern DWORD            __dwResumeItem;
extern DWORD            __dwGoTimeFrameCnt;
// LLY2.36, create a new variable to keep the desired STC value for GoTime action
// To avoid the inaccuracy from STC/90K = sec unit, then * 90K again to get PTS
extern  DWORD           __dwGoTimeSTC;
// LLY2.61, create a new variable to keep time A STC value for Repeat A-B
extern  DWORD           __dwTimeA_STC;


// wyc2.37-909s
extern BYTE    __bACHN;                // Record current Audio Channel Number. From "1"..
extern WORD    __wChapter_Ns_Title;    // Record the Total Chapters/ current Title (PTT)
extern WORD    __wV_ATR;               // Record Video Attribute (current)
extern WORD    __wSP_LCD;              // SP Language
extern WORD    __wA_LCD;               // Audio Language
extern DWORD   __dw1stSCR;             // Keep 1st SCR value

extern BOOL   __bSmartPB_ABLE;
extern BYTE    __bMaxPBT_TT;   // the Title with max playback time in current DVD disc

// DVD_099cMicky, support show OSD only when IR key is pressed
extern  BYTE    __bKeyMessage;

// Micky1.07, adding DEBUG info variable declaration
extern  BIT     __btDumpFlag;
extern  BYTE    __bDumpType;
#define INFO_BASIC          0x1L
#define INFO_TITLE_REL      0x2L
#define INFO_SERVO_REL      0x4L
#define INFO_AVSYNC_REL     0x8L
#define INFO_ERR_CONCEAL    0x10L
#define INFO_MISC           0x20L

#define     KEY_DISALLOW    0//REJECT  0
#define     KEY_ACCEPT  1
#define     KEY_IGNORE      2//CANCEL  2

// LLY.040-1 begin, for screen saver usage
#ifndef NO_SCREEN_SAVER
extern      BYTE    __bScreenSaver;
#define     NONE_SCREEN_SAVER               0
#define     PREPARE_SCREEN_SAVER            1
#define     ENTER_SCREEN_SAVER              2
#define     TRIGGER_SCREEN_SAVER            3
#define     DISABLE_SCREEN_SAVER            4
#endif  // #ifndef NO_SCREEN_SAVER

// DVD_161Micky, support Loading-> Setup-> Exit, will continue loading action
extern BYTE __bFlowState;
#define FLOW_NORMAL                     0x00
// DVD_172Micky, FLOW_CONTROL begin
/*
#define FLOW_SUSPEND_STABLE             0x80    // wait stable is interrupt
#define FLOW_SUSPEND_DETECTION          0x40    // title detection is interrupt
#define FLOW_SUSPEND_PLAY               0x20    // play is interrupt
// FLOW_SETUP will be removed.
#define FLOW_SETUP                      0x10    // in setup mode
*/
// DVD_172Micky, FLOW_CONTROL end

#define FLOW_RELEASE_STABLE             0x08    // allow continue wait stable
#define FLOW_RELEASE_DETECTION          0x04    // allow continue title detect
#define FLOW_RELEASE_PLAY               0x02    // allow continue to play
// DVD_172Micky, FLOW_CONTROL begin
#define FLOW_OPEN                       0x10
#define FLOW_KEEP_RESUME                0x01
#define FLOW_TRIGGER    (FLOW_RELEASE_STABLE| FLOW_RELEASE_DETECTION| FLOW_RELEASE_PLAY)
// DVD_172Micky, FLOW_CONTROL end

///////////////////////////////////////////
BYTE        CC_DoKeyAction ( BYTE bKey );
BYTE        CC_KeyCommand ( BYTE bKey );        // it'll be used by extra module

//////////////////////////////////////////////

void        _Get_PlayRange (void);
// Micky0.62-909, removed
// [CT909 Removed]
//void        _CC_Process_KeyNumber ( WORD wValue );

BYTE        _CC_PreProcessKey (void);

void        _CC_QChannelTorelence(void);

BYTE        _CC_ImmediateKey(void);
// wyc1.05-909, add extern function.
VOID    CC_ResetState(BYTE bState);
// wyc2.22-909s, extern variables.
BYTE CC_ForceStop(DWORD dwMode);
void CC_ForceResumeRePlay(DWORD dwMode);

BIT     _InvalidAccess(DWORD dwMode);
extern BIT  __btIntroMode;
extern DWORD    __dwIntroPlayTime;

#define CC_MODE_PBC     0x00000001L
#define CC_MODE_PLAY    0x00000002L
#define CC_MODE_STOP    0x00000004L
#define CC_MODE_INTRO   0x00000008L
#define CC_MODE_LINEAR  0x00000010L
#define CC_MODE_DIGEST  0x00000020L
#define  CC_MODE_PROGRAM 0x00000040L
#define CC_MODE_GOTIME  0x00000080L


BYTE    _KeyGroupTest(BYTE bKey);
BYTE    _KeyGroupAudio(BYTE bKey);
BYTE    _KeyGroupPlay(BYTE bKey);
BYTE    _KeyGroupMode(BYTE bKey);
// ** TCH0.60; BYTE    _KeyGroupMultiDisc(BYTE bKey);
BYTE    _KeyGroupVideo(BYTE bKey);
BYTE    _KeyGroupNumber(BYTE bKey);
BYTE    _KeyGroupSetUP (BYTE bKey);     // ** TCH0.27;
BYTE    _KeyGroupDVD   (BYTE bKey);     // ** TCH0.27;
BYTE    _KeyGroupOthers(BYTE bKey);
BYTE    _CC_ProcessSearchAction(BYTE bKey, WORD wParam); // ** LLY.036
extern  void PANEL_Initial(void);
extern  void PANEL_Output(BYTE bMessage, WORD wParam);

//DVD_049MickyPowerDown
// support auto power down
extern DWORD __dwTimeWaitPowerDown;
// wyc1.01-909, remove it because of not use it.
//extern DWORD __dwProgressCount;  // LLY.276-5
// Micky2.79-2, support FullScreen function
// init. value 0 will be assigned within INITIAL_Variables.
// 1: means full screen video on, 0: means full screen video off.
extern  BYTE    __bFullScreen;
// Micky1.24, make sure that servo send correct dvdinfo data
extern BYTE __bDataRequest;

//DVD_043-2
// check region code match or not
// wyc2.37-909s
#ifndef NO_DISC_MODE //CoCo2.37p
BYTE    _IdentifyRegionCode(void);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

// Micky2.81, Provide an API for access __bMessageMap
void _CC_MapKeyMessage(BYTE bKey);
// Micky0.90, use new buffer control.
// Monitor audio overflow for audio buffer 1 only
// Minitor audio underflow for audio buffer1+2
// if disabled, MONITOR.c need check???
#define NEW_AUDIO_BUFFER_CONTROL

// Micky1.05, add auto test for IC sorting by CoCo
#ifdef AUTO_TEST
void _CC_AutoTest(void);
void  CC_AutoTestInitial(void);
#endif

// ** TCH1.21-1; begin...
void _CC_Variables_PowerON (void);
// Micky1.23, support DVD_T
#define CC_ACTION_NONE          0
#define CC_ACTION_DVD_OFF       1
#define CC_ACTION_SAVE_RESUME   2

// ** TCH0.60; begin...
// Internal Function protype
void    _CC_Trigger_AutoPowerDown(void);    // ** TCH0.60;
void    _CC_Trigger_NumberTimeOut(void);    // ** TCH0.60;

// Micky0.62-909, removed
// [909 Removed]
//void    _ConfirmAction(WORD wValue);
// LLY0.62, add function prototype for _CC_AssignActionKey()
void _CC_AssignActionKey(void);

// LLY0.62, add function prototype and parameter ID for _CC_GoTimeAction
BYTE _CC_GoTimeAction(BYTE bAction);
#define GOTIME_EXECUTE      0       //  Execute GoTime action
#define GOTIME_CALCULATE    1       // Calculate GoTime value

extern  DWORD   __dwW99AV_DestDRAM;
extern  DWORD   __dwW99AV_SrcDRAM;
void    Thread_CTKDVD (DWORD index);

extern  DWORD   __dwOpenTime;   // ** TCH0.60;

// ** TCH0.60; end...

// Micky0.62-909
// Micky0.80, remove parameter bKey.
BYTE    _CC_NumberProcessKey( void);//BYTE bKey );
//BYTE    _CC_DVD_PreProcessKey(void);
#ifndef NO_DISC_MODE //CoCo2.37p
BYTE    CC_DVD_ProcessKey(void);//BYTE bKey);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

// wyc2.37-909s
#ifndef NO_DISC_MODE //++CoCo2.37p
BYTE    CC_VCD_ProcessKey(void);//BYTE bKey);
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
// wyc2.38a-909s,
BYTE CC_CompareAPFile(BYTE bMode, DWORD *dwFileLoc, DWORD *dwFileLength);

// Micky0.51-909, process number key, then convert as KEY_NUMBER
#define MAX_DIGITS_NUMBER   10000   // the value can't exceet 0xFFFF(65535)
#define MAX_DIGITS          5

#define NUMBER_CLEAR        1
#define NUMBER_SHIFT        2
#define NUMBER_DIGITS       3
#define NUMBER_SET_INPUT    4
#define NUMBER_SET_TIMEOUT  5
#define NUMBER_CLEAR_TIMEOUT    6
// Micky0.72, update number queue
#define	NUMBER_QUEUE			7
BYTE    CC_NumberUtility(BYTE bAction);
BYTE    CC_MainProcessKey(void);
BYTE    _CC_CheckValidKey(void);
// Micky0.68, merge SP change into an API for navigator and key switch share use.
// parameter 1 means by KEY_SPST_CTL, 0 means by friendly navigator
BYTE	CC_SPST_Change(BYTE bSwitch);
// Micky0.76
BYTE	CC_Angle_Change(BYTE bSwitch);
// Micky0.80, add switch ID
BYTE	CC_Audio_Change(BYTE bSwitch);
#define CC_SWITCH_QUERY     0x00
// Micky0.90-2, add query for SP2
#define CC_SWITCH_QUERY_SP1 CC_SWITCH_QUERY
#define CC_SWITCH_QUERY_SP2 0x80

#define CC_TOGGLE           0x10
#define CC_TOGGLE_SP1       0x11
#define CC_TOGGLE_SP2       0x12
#define CC_TOGGLE_ANGLE    0x11
#define CC_TOGGLE_AUDIO     0x11
#define CC_SWITCH_SP1        0x01
#define CC_SWITCH_SP2        0x02
#define CC_SWITCH_ANGLE     0x01
#define CC_SWITCH_AUDIO     0x01

// wyc0.91, Brian's updated code.
// Brian0.91
extern DWORD   __dwVersionMPEG;     // MPEG decoder module version
extern DWORD   __dwVersionJPEG;     // JPEG decoder module version
extern DWORD   __dwVersionDisplay; // Display module version
extern DWORD   __dwVersionDivX;     // DivX module version
extern DWORD   __dwVersionNavigate; // Navigate module version
extern DWORD   __dwVersionParser;     // Parser module version
// wyc1.20-909
extern DWORD   __dwVersionUSB;      // USB module version.
// wyc1.50-909, new protected module group.
extern DWORD   __dwVersionINFO;       // INFO module version.

extern DWORD   __dwVersionServo;     // Servo module version
extern DWORD   __dwVersionServoBM; // Servo BM version
extern DWORD   __dwVersionCard; // Card module version
extern DWORD   __dwVersionServoUCode; // Servo uCode version
extern DWORD   __dwVersionRelease;   // F/W release version
extern DWORD   __dwVersionFAE;       // FAE version
extern DWORD   __dwVersionTFT;       // VD version
extern const BYTE __csVersionCustomer[]; // the customer version string
#ifdef SUPPORT_AV_IN
extern BIT _btAVIN;  //Jeff 20060705 Add for AV in
extern BIT _btTFT_PowerDown;
#endif

#ifdef SUPPORT_POWER_SWITCH
extern BYTE __bPowerSw;
extern BYTE __bPowerSw_StandBy;
#endif //SUPPORT_POWER_SWITCH
#ifdef __cplusplus
        }
#endif
#endif  // __CC_H__
