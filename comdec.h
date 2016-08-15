//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2004,
//          D300, all right reserved.
//
//      Product : CT909
//
//      Date    : 2004.05.03
//      Author  : Cheertek (D300 Chuan)
//      Purpose : 
//      Sources : 
//  **************************************************************************

#ifndef __COMDEC_H__
#define __COMDEC_H__

#ifdef __cplusplus
        extern  "C"     {
#endif
// #define     SUPPORT_FORCE_STOP
// #define SUPPORT_N_STRIPE

#define N_STRIPE_LEN                    (288)

// Playable Command
typedef enum tagVDEC_CMD
{
    MODE_NONE = 0x0,
    
    MODE_STOP = 0x10,      // stop                              
    MODE_STOPPED,          // stopped                           
    MODE_SCAN_DONE,        // Scan cmd is done                  
    MODE_PREDEC_DONE,      // pre-decode sequence header is done
    MODE_SLOWREV,          // slow motion revise

    MODE_AVSYNC = 0x20,                
    MODE_PLAY,             // normal play
    MODE_FREEZE,           // freeze the video, but audio going
    MODE_FNPF,             // fast normal play with audio(2X speed)
    MODE_NONAVSYNC = 0x28,                                                
    MODE_SF,               // slow motion
    MODE_PAUSE,            // pause the play
    MODE_STEP,             // single step play
   
    MODE_SCAN = 0x40,
    MODE_FF,               // fast IP with audio(2X speed) 
    
    MODE_PREDECODE = 0x80,  // pre-decode sequence header               
    MODE_FORCE_IP,         // Force to decode from IP                  
    MODE_UPDATE_SEQ,       // update sequence information.             
    MODE_CLEAR_STILL,      // Clear sequence end code.                 
    MODE_FORCE_FNSEQ,      // Force to decode sequence.                
    MODE_SAFE_STOP,        // delay 1 picture to stop                  
    MODE_RELEASE_MODE,     // Release Mode to do more error concealment
    MODE_ERROR_RATIO
} EN_VDEC_CMD, *PEN_VDEC_CMD;

typedef struct  tagPTS_INFO
{
    DWORD   dwPTS;
    DWORD   dwReserved;
    ULONGLONG_UNION    ullAddress;
} PTS_INFO, *PPTS_INFO;

#define     PTS_NUM                     (64)
#define     PTS_MASK                    (PTS_NUM-1)

#define     KEEP_DEC_FRAME              (1)
#define     KEEP_DEC_BFRAME             (2)

#define     UPDATE_SEQ_CNT              (2)

#define     TOP_FIELD                   (1)
#define     BOTTOM_FIELD                (2)
#define     FRAME_PICTURE               (3)

#define     COMDEC_NOERROR              (0)
#define     COMDEC_ERROR_USERSTOP       (1)
#define     COMDEC_ERROR_TIMEOUT        (2)

VOID    COMDEC_Debug(VOID);
VOID    COMDEC_SoftReset(DWORD dwVideoAddress);
DWORD   COMDEC_Query_MBRDY( DWORD dwTimeOut );
DWORD   COMDEC_CalculateCurPos(VOID);
VOID    COMDEC_Command(EN_VDEC_CMD dwCMD, DWORD dwArg);
VOID    DSR_ResumeDec(VOID);
VOID    DSR_SuspendDec(VOID);
#ifdef SUPPORT_N_STRIPE
VOID    ISR_DecodeMB(VOID);
VOID    ISR_16L(VOID);
LONG    MPG_DecodeMB(VOID);
#endif
// LLY0.72
//#if (PLATFORM == ECOS_OS)
VOID    DivX_ThreadMain(DWORD dwSIDEINFO_ADDR);
VOID    DivX_ThreadExit(WORD data);
VOID    MPEG2_ThreadMain(DWORD data);
VOID    MPEG2_ThreadExit(WORD data);
//#endif
VOID    MPEG2_ThreadInit( VOID );
VOID    DivX_ThreadInit( VOID );
VOID    DivX_Debug(VOID);
BOOL    MPEG2_GetCCData( PULONG_UNION pullCCData );
VOID    COMDEC_Substitution(DWORD dwFrame, DWORD dwMBAStart, DWORD dwMBAEnd, DWORD dwMBWidth, DWORD dwPicture_structure);

extern PTS_INFO __PTS_Info[PTS_NUM]; // Record PTS & its address

extern DWORD    __dwPTS_Rp;
extern DWORD    __dwPTS_Wp;
extern DWORD    __dwAPTS_Rp;
extern DWORD    __dwAPTS_Wp;

extern DWORD    __dwCoded_Picture_Width;
extern DWORD    __dwCoded_Picture_Height;
extern DWORD    __dwPlayModeArg;
extern DWORD    __dwDecFrame;
extern DWORD    __dwVideo_Rp;
extern DWORD    __dwSeqEndCode;
extern DWORD    __dwFNSeq;
extern DWORD    __dw32PullDown;
extern DWORD    __dwPicErrCnt;
extern DWORD    __dwDecGroup;
extern DWORD    __dwReleaseMode;

extern DWORD    Second_Field;
extern DWORD    _dwDecMode;
extern DWORD    _dwKeepDecode;
extern DWORD    _dwDecY;
extern DWORD    _dwBFR_Mode;
extern DWORD    _dwErrRatio;
extern DWORD    _dwSkipFrame;
extern DWORD    _dwDecodeTop;
#ifdef SUPPORT_N_STRIPE
extern DWORD    _dwNStripe_Mode;
extern DWORD    _dwDispBase;
#endif
extern BYTE     _bWaitRLDone;
extern WORD     _wYMaxSize;

// For MPEG2DEC.C
extern DWORD    __dwIPCnt;
extern DWORD    __dwUpdateSEQ;
extern DWORD    __dwGoToPTS;
extern DWORD    __dwAspect_Ratio_Info;
#ifdef __cplusplus
        }
#endif

#endif //__COMDEC_H__
