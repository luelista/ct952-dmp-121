
/****************************************************************************
    [ mscddef.h, mscd.h, mscd.cpp, mscd1.cpp ]
    Copywrite 1995 by TCH/L300, Cheertek, all right reserved

    Author  :   Chung-Hao   Tseng
    Date    :   1995. 1. 4
    Purpose :
    Notes   :
    Update  :
     Date        Name        Description
    ======    ==========    =================================================

******************************************************************************/

#ifndef     __NAVIGATE_H__
#define     __NAVIGATE_H__

#ifdef __cplusplus
   extern "C"{
#endif

#ifdef    _TESTCD
#include "alone.h"
#endif    // _TESTCD


// ** TCH1.23; begin... 
// If enable below definition, it will be latest support. Otherwise, it's same as WinDVD 1.22a.
#define     NV_ANTIHANG
// ** TCH1.23; end... 

//////////////////////////////////////////////////////////////
//  extern

//////////////////////////////////////////////////////////////
//  definition

// *** 22QF(0.20);
extern  DWORD               _dwFirst_VMGI;


extern  BYTE                __bPreDispTitle;             //Aron,1.50-909,fix mantis #1125 Record the previous display(OSD) Logical Titles of ONE DISC
extern  WORD                __wPreDispChapter;           //Aron,1.50-909, Record the previous display(OSD) Logical Chapters of ONE Title (PTT)

extern  BYTE                __bTitle;             // Record the Logical Titles of ONE DISC
extern  WORD                __wChapter;           // Record the Logical Chapters of ONE Title (PTT)

// ** TCH0.29; begin...
extern  BYTE                __bTitle_Ns;         // Record the Total Titles NO. in ONE DISC

//Aron2.37, move to CC module
#if 0
extern  WORD                __wV_ATR;            // Record Video Attribute (current)

extern  WORD                __wChapter_Ns_Title;  // Record the Total Chapters/ current Title (PTT)
extern  BYTE                __bACHN;              // Record current Audio Channel Number.
#endif
// ** TCH0.29; end...

extern  WORD                __wPGCN;
extern  BYTE                __bPGN;
extern  BYTE                __bCEN;
extern  BYTE                __bDomain;
extern  BYTE                __bPreDomain;   // ** TCH1.61-1; 
extern  BYTE                __bNVPlayMode;      // ** TCH0.31;


// ** TCH0.39; begin...
extern  BYTE                __bModeCELLTimeSearch;
extern  DWORD               __dwTimeCELLTimeSearch;

extern BYTE                  __bStillRSM; //Aron2.38,  flag of "resume/bookmark to still content"
extern DWORD               __dwStillRSMTimeSearch;

extern  WORD                __wTimeTitle;       // Total time of Current Title
extern  WORD                __wTimeChapter;     // Total time of Current Chapter
extern  WORD                __wTimeChapterBegin;// Begin time of Current Chapter
extern  WORD                __wTimeCellBegin;// Begin time of Current Cell
// ** TCH0.39; end...

// LLY2.36, add a variable to keep previous cell begin
extern  WORD                __wPreTimeCellBegin;

extern  WORD                __wTimeCurrentDisplay; // ** TCH1.61a; 

//extern  WORD                __wVTS_StartTTN; //--CoCo1.20, reduce code size.

extern  BIT                 _btReadTitleKey;

//Aron2.37, move to CC module
#if 0
// ** TCH0.38; begin... Audio/ SP Language
extern  WORD                __wSP_LCD;          // SP Language
extern  WORD                __wA_LCD;          // Audio Language
// ** TCH0.38; end...
#endif
// ** TCH0.40; begin... Record Player_Parental_ID, "0xFFFF" mean "Not specified"
extern  WORD                __wVTS_PTL_ID;
extern  WORD                __wVMG_PTL_ID;
// ** TCH0.40; end...

extern  BYTE                __bSPST_Display_Mode;    // ** TCH0.451; 0-PS, 1-LB, 2-16:9, 3-4:3.
extern  BYTE                __bSPST_LB;          // ** TCH0.99; Use different SPSTN for LB.

// In "PGC.CPP"
extern  BYTE        __bPGCNOSequential;      // Record the PGC is Sequential/ Shuffle/ Randon
extern  BYTE        __bLoopPGCount; // Record the Loop Count.
extern  WORD        __wCELLState;   // Record the value that return from

// In "NVCMD.CPP"
//extern  unsigned char       __sNVCommand[9];
extern  BYTE                __bNVCommand[9];

// SMAN usage
extern  BYTE                __bState;
extern  BYTE                __bStatus;

extern  BYTE                __bUOP_Parameter;           // ** TCH0.99;

// ** TCH1.21-1; begin... 
extern  BYTE                __bDUMP_NV_CTL;     // ** TCH1.21-1; Dump different information for NV

#define         DUMP_NV_PTL                 0x80
#define         DUMP_NV_LOADPGC         0x02
#define         DUMP_NV_PGCSTATE         0x04
#define         DUMP_NV_CELLSTATE         0x08

#define         DUMP_NV_TTCC2PGC         0x40
#define         DUMP_NV_ENTRY               0x01
#define         DUMP_NV_PARSEINFO        0x10        // ** TCH1.23; 
#define         DUMP_NV_FLOW                0x20        // ** TCH1.25; 

// ** TCH1.21-1; end... 


//////////////////////////////////////////////////////////////////////
// move to dvdinfo.h
// in VMGI_TT_SRPT
//#define     OFFSET_TT_SRP               8
//#define     BLOCK_TT_SRP                12

#define         MAX_SPSTN               32      // ** TCH0.451;
#define         MAX_ASTN                8      // ** TCH0.451;

// MenuCall Usage, Please ref. VTSM_LU-Menu ID
#define     MENU_TITLE                  1
#define     MENU_ROOT                   3       // Same as Menu ID
#define     MENU_SP                     4       // Same as Menu ID
#define     MENU_AUDIO                  5       // Same as Menu ID
#define     MENU_ANGLE                  6       // Same as Menu ID
#define     MENU_PTT                    7       // Same as Menu ID

// Domain definitions
#define     STOP_DOM                    0
#define     VMGM_DOM                    1
#define     FP_DOM                      2
#define     VTSM_DOM                    4
#define     TT_DOM                      8

#define     SPACE_SYSTEM                0x7
#define     SPACE_MENU                  0x5
#define     SPACE_VMG                   0x3
#define     SPACE_VTS                   0xC

// NV_Action definition

#define     ACTION_UP                   5
#define     ACTION_DOWN                 6
#define     ACTION_LEFT                 7
#define     ACTION_RIGHT                8
// DVD_101-2Micky add
#define     ACTION_NUMBER_CHECK         9
#define     ACTION_NUMBER               10
// DVD_101-2Micky add
#define     ACTION_ACTIVE_CHECK         11
#define     ACTION_ACTIVE               12

#define     ACTION_REPEATAB             13

// ** TCH1.01-2; begin...
#define     ACTION_BUTTON_QUERYUOP_END          ACTION_ACTIVE

#define     ACTION_STOP_INHIBIT_BEGIN           1               // must from "1" and other ACTION_xxx should lower than ACTION_STOP_INHIBIT_END
// ** TCH1.61a; #define     ACTION_STOP_INHIBIT_END             ACTION_ACTIVE
#define     ACTION_STOP_INHIBIT_END             ACTION_REPEATAB
// ** TCH1.01-2; end...

#define     ACTION_NEXT                 15
#define     ACTION_PREV                 16

// ** TCH1.70; #define     ACTION_NEXTTITLE            20

#define     ACTION_STOP                 30
#define     ACTION_PLAY                 31      // ** TCH0.373;
#define     ACTION_RESUME               32      // ** TCH0.40;

// Flag (Return Value) of _NV_CheckNVCMD      // ** TCH0.31;
#define     FLAG_NONE                   0x00    // Nothing
#define     FLAG_FALSE                  0x00    // Error
#define     FLAG_TRUE                   0x01    // OK
#define     FLAG_GO                     0x02    // Just let flow GO through
#define     FLAG_RESTART                0x10    // Needs to restart Chips

#define     FLAG_IMMEDIATE              0x51    // NV_CMD needs to process within Command Area
#define     FLAG_DONE                   0x50    // NV_CMD is done
#define     FLAG_JUMP                   0x60    // NV_CMD will jump to another STATE
#define     FLAG_TRIGGERSTATE           0x61    // NV_CMD will jump to another STATE/STATUS and only modify PGC StateMachine

// NV_Setting definition
#define     SET_AST                     0
#define     SET_SPST                    1
#define     SET_AGL                     2       // ** TCH0.47;
#define     SET_SP2ST			3	   // Aron1.01

#define     SET_PTL_ID_FLD              10      // ** TCH0.40;

// ** TCH0.99; move to "20", if above "20", means allow setting for all states.
// Otherwise allow when DVD-Title exist
#define     SET_REGION_CODE             20
#define     SET_PARENTAL_CONTROL        21      // ** TCH1.05-908; 


//////////////////////////////////////////
// PGC_Play
#define     OP_PRECMD                   0x80
#define     OP_NO_PRECMD                0x00

//***************************************************************************
//CONTROL : Global Data type or Data Structure declare.
//***************************************************************************
// Structure for State information
typedef struct CC_tagStateInfo
{
    BYTE    bState;     // State
    BYTE    bStatus;    // Status
} CC_STATEINFO, * PCC_STATEINFO;

///typedef CC_STATEINFO *      PCC_STATEINFO;
// State_ID defineition : (1) State_ID == 0 ==> resevrved state.
//                        (2) State_ID {0- 127} ==> normal state.
//                        (3) State_ID >= 128 ==> error state.
#define STATE_UNDEF            0xFF// Unknow state error
#define STATE_RESERVED          0  // Central control will by pass this state
#define STATE_INITIAL           1  // Initialized
#define STATE_STOP              2  // Stop
#define STATE_PRECMD            3  //
#define STATE_POSTCMD           4  //
#define STATE_MARK_CMD          STATE_POSTCMD
#define STATE_PG                5  //
#define STATE_STILL             6  //
#define STATE_CELL              7  //
#define STATE_NEXTLINK          8  //
#define STATE_NEXTPG_NONV      9   // ** TCH2.37; It's for NEXTPG

// State status defineition :
#define STATUS_UNDEF            0
#define STATUS_READY            1  // Wait to execute
#define STATUS_DONE             2  // Already be executed and ended
#define STATUS_RUNNING          3  // within execute
#define STATUS_SUSPEND          4  // Suspended; after this mode will transfer to STATUS_WAITING
#define STATUS_WAITING          5  // Wainging some trigger signal
#define STATUS_REDO             6  // re-do the state back from some sleep mode
#define STATUS_NVCMD            7  // Doing NVCMD Now ( STATE_CELL only )
#define STATUS_NVCMD_EXECUTE    8  // Execute NVCMD Now (<ex> Jump/ Link issue )
#define STATUS_ERROR            20 // Error occurred

#define STATUS_SAVE             30 // SAVE status
#define STATUS_RESTORE          31 // Restore status
#define STATUS_DISCARD          32 // Discard some items
#define STATUS_EXIST_RSM        33 // Exist RSM

////////////////////////////////////////////////////////
//  _PGC_BitsDiscontinue definition

#define     DISCONTINUE_AV              0
#define     DISCONTINUE_AV_CDROM        1       // ** TCH1.03; ?? Remove it later
#define     DISCONTINUE_AV_ONLY         5       // ** TCH1.61a;    

#define     DISCONTINUE_AUDIO_SP        10

//////////////////////////////////////////////////////////////
//      dvd.cpp [OS issue]

#include "nvcmd.h"

////////////////////////////////////////////////////////////////////
// DVD part
// DVD Information Part

// Navigator Part
BYTE    NV_TitlePlay ( BYTE bTitle, WORD wPTT );
BYTE    NV_TimePlay ( BYTE bTitle, DWORD dwTime );      // ** TCH0.36;
BYTE    NV_TitleTimePlay ( WORD wTime );              // ** TCH1.23; Add it for TimePlay within one Title.


BYTE    NV_MenuCall ( BYTE bTypeMenu );
BYTE    NV_Action ( BYTE bAction, WORD wParam );
BYTE    NV_Setting ( BYTE bSetItem, BYTE bParam );      // ** TCH0.371;
BYTE    NV_QueryUOP ( DWORD dwUOP );    // ** TCH0.37;

BYTE    NV_RSMControl ( BYTE bStatus );

void    NV_Trigger (void);	// ** TCH1.72q; 
void    NV_UpdateVersion(void); //Aron1.02-909, Update NV module version
void    NV_SetErrorConParm(BYTE bMaxLoadVTSFailCnt, DWORD dwServoReadyTimeOut); //Aron2.31-909R, add to init error concealment mechanism parmater

//////////////////////////////////////////////////////
// PGC Part [PGC] Module
BYTE    PGC_Playback ( BYTE bPGN, BYTE bOperation );
BYTE    PGC_Trigger (void);	// ** TCH1.72q; 

//////////////////////////////////////////////////////
// State Control
void    SMAN_GetStateInfo ( void );
void    SMAN_SaveStateInfo ( void );

BYTE    SMAN_GetState ( void );
BYTE    SMAN_GetStatus ( void );
BYTE    SMAN_PushStateInfo ( BYTE bState, BYTE bStatus );
BYTE    SMAN_ChangeStatus ( BYTE bStatus );

/////////////////////////////////////////
// Protected function
// Navigator Usage

BOOL    _NV_Convert_TTCC2PGC ( BYTE bDirection );       // ** TCH0.31;
BOOL    _NV_Convert_STN2STNO ( BYTE bSetItem, BYTE bDirection );       // ** TCH0.451;

BOOL    _NV_BuildPGCInfor ( void );

BOOL    _NV_SetAudioParameters ( DWORD dwAudio, BYTE bParam ); // ** TCH0.371;
BYTE    _NV_SearchLanguage ( BYTE bSetItem );   // ** TCH0.38;

BYTE    _NV_ExecuteHighLight (void);                   // ** TCH0.362;
BYTE    _NV_CheckNVCMD (void);                      // ** TCH0.29;
BYTE    _NV_ExecuteNVCMD (void);                    // ** TCH0.31;
BYTE    _NV_LinearNextPrev ( BYTE bNext );      // ** TCH0.31;

BYTE    _NV_ProcessPTL_MA (void);                   // ** TCH0.431;

DWORD   _NV_GetUOP ( BYTE bMenuType );          // ** TCH0.47;
// Micky0.66-909, add a parameter for PGN
BYTE    _NV_LinkPGCN_PG ( BYTE bDomain, BYTE bPGN );      // ** TCH1.61-1; 
BYTE    _NV_CheckSP_ForcedlyDisplay (void); // ** TCH1.61-1; 


#ifdef  SIMP_DVD
BYTE    _NV_SIMPTrigger (void);      // ** TCH0.361; SIMP_DVD only
#endif    // SIMP_DVD

#ifdef    NO_PGC
BYTE    _NV_NOPGCTrigger (void);      // ** TCH0.361; NO_PGC only
BYTE    _NV_GetCELLRange (void);      // ** TCH0.361; NO_PGC only
#endif    // NO_PGC

/////////// PGC relative APIs
BYTE    _PGC_ProcessCell (void);            // ** TCH0.31;
BYTE    _PGC_WaitCELLStatus ( BYTE bCELLStatus );        // ** TCH0.39;
BYTE    _PGC_UpdateCELLInformation (void);   // ** TCH1.60a-2; 

BYTE    _PGC_BuildPGInfor ( BYTE bPGN );
BYTE    _PGC_IsAcceptCellTime ( BYTE bCEN );
BYTE    _PGC_LookAngleCEN ( BYTE bCEN );        // ** TCH1.60a-1; 
BYTE    _PGC_SkipAngleBlock ( BYTE bModeCmd );  // ** TCH1.60a-1; 

BYTE    _PGC_ChooseOneAvaiablePG ( BYTE bPGN ); // ** TCH0.41;

BYTE    _PGC_ProcessCMDT ( BYTE bTypeCMDT, BYTE bIdx ); // ** TCH1.03;
void    _PGC_BitsDiscontinue ( BYTE bMode );     // ** TCH1.03;
void    _PGC_Force2PlayMode ( BYTE bCheckType );    //TCH1.60

// Micky1.23 support resume info from EEPROM
#define USE_NEW_RESUME_INFO
typedef struct NV_tagRSMInfo
{
BYTE                __bRSM_Title;    // SPRM[4]
WORD                __wRSM_Chapter;  // SPRM[7]
WORD                __wRSM_PGCN;     // SPRM[6]
WORD                __wRSM_SPRM5;

BYTE                __bRSMValid ;

BYTE                __bRSM_CEN ;
BYTE                __bRSM_byTime ;    // TRUE mean RSM by Time, else RSM by VOBUSA
BYTE                __bRSM_PGN ; // ** TCH1.03;
BYTE                __bRSM_VTSN ;

BYTE                __bRSM_Domain ;  // ** TCH1.03;

DWORD               __dwRSM_NV_PCK ;
WORD                __wRSM_Time ;        // unit: second
BYTE                __bPTL;             // Parental Level.
BYTE                __bStillRSM;    //Aron2.38, still resuming flag
//Ming0.80, add __attribute__((packed)) for NV_RSMINFO to reduce the size.
//          Becuase Resume info. would be saved to EEPROM.
} PACK_FORMAT NV_RSMINFO, * PNV_RSMINFO;
extern  NV_RSMINFO  NV_Resume[2];

// Ming0.80, support multi-disc resume.
//           Must check PTL level in RESUME process.
#define RESUME_MARK_SIZE            4
#define RESUME_ID_ENTRY_SIZE        4
#define RESUME_INFO_ENTRY_SIZE      (sizeof(NV_RSMINFO))
// LLY1.00, resume data = resume ID + resume information
#define RESUME_DATA_ENTRY_SIZE      (RESUME_ID_ENTRY_SIZE+RESUME_INFO_ENTRY_SIZE)
#if (RESUME_DISC_NUM > 15)
#error "RESUME_DISC_NUM cannot exceed 15!"
#endif
#if (RESUME_ID_ENTRY_SIZE != 4)
#error "RESUME_ID_ENTRY_SIZE must be euqal to 4!"
#endif


// LLY2.05, define a struct for other title resume information
// Notice: the size must be same as NV_RSMINFO (20 bytes)
typedef struct tagGNL_RSMINFO
{
    // For all non-DVD title
    DWORD   dwRSMTime;      // Keep resume time
    // For CDROM/ DVDROM title
    WORD    wRSMDir;        // Keep resume directory
    WORD    wRSMItem;       // Keep resume item
    // For CDROM/ DVDROM audio only file
    DWORD   dwRSMFrameCnt;  // Keep resume frame count 
    DWORD   dwReserved[2];  // Reserved space 
} GNL_RSMINFO, *PGNL_RSMINFO;

// LLY2.05, define a union for DVD and general title resume information
typedef union tagRSMINFO_UNION
{
    NV_RSMINFO      NV_Resume;
    GNL_RSMINFO     GNL_Resume;
}RSMINFO_UNION, *PRSMINFO_UNION;

typedef struct _Multi_Resume_Entry
{
    BYTE bRU_MType;
    BYTE bDiscUID[3];
// LLY2.05, using resume info union to split DVD and other title
//    NV_RSMINFO NV_Resume;
    RSMINFO_UNION RSM_Info;
} Multi_Resume_Entry, *PMulti_Resume_Entry;

typedef struct _Multi_Resume_Info
{
    BYTE bRSM_Mark[RESUME_MARK_SIZE];
    Multi_Resume_Entry Resume[RESUME_DISC_NUM];
} Multi_Resume_Info, *PMulti_Resume_Info;

//Aron1.05-909, Record user SPST & AST setting
typedef struct NV_tagUserSetting
{
BYTE                __bUS_Vaild;
BYTE                __bUS_TTNO;
BYTE                __bUS_ASTNO;
BYTE                __bUS_SPSTNO;
WORD             __wUS_SRPM2;
// Need to keep other setting else!?
} PACK_FORMAT NV_USERSETTING, * PNV_USERSETTING;

extern NV_USERSETTING NV_Usersetting;

//Aron2.31-909R, add for error concealment mechanism parmater
//#define USER_KEY_ABORT (__bISRKey == KEY_TITLE ||__bISRKey == KEY_MENU||__bISRKey == KEY_PREV||__bISRKey == KEY_NEXT||__bISRKey == KEY_PBC||__bISRKey == KEY_DIGEST||__bISRKey == KEY_OPEN_CLOSE||__bISRKey == KEY_POWER)
#define MAXLOADVTSFAILTCNT 25
#define SERVOREADYTIMEOUT COUNT_5_SEC

typedef struct NV_tagErrorConParm
{
BYTE                __bMaxLoadVTSFailCnt;
BYTE                __bLoadVTSFailCnt;
DWORD            __dwServoReadyTimeOut;
} NV_ERRORCONPARM, *PNV_ERRORCONPARM;

extern NV_ERRORCONPARM NV_Errorconparm;

#ifdef __cplusplus
   }
#endif

#endif      // __NAVIGATE_H__


