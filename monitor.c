// The file "winav.h" must be included in first line
#include "winav.h"
#include "ctkav.h"

#include "utl.h"
#include "hal.h"
#include "SrcFilter.h"
#include "input.h"
#include "chips.h"

#include "infofilter.h"

#include "digest.h"

#include "navigate.h"
#include "cc.h"
#include "monitor.h"
#include "osd.h"
#include "linear.h"
#include "cell.h"
#include "comutl.h" 
#include "vcd20.h"
#include "thumb.h"
#include "setup.h"
#include "gdi.h"
#include "panel.h"
#include "Disp.h"
#include "Comdec.h"
#include "parser.h"
#include "debug.h"
// wyc1.11-909
#include "Subpict.h"
// wyc2.16-909S
#include "initial.h"
#include "debug.h"
#include "dvdinfo.h"

#ifdef SUPPORT_STB
#include "media.h"
#include "db_defs.h"
extern EN_SERVICE_TYPE ap_ch_get_current_srv_type(void);
void _MONITOR_DVBRecover(void);
#endif

#undef PRINTF
#undef printf
// Micky2.17c
#define PRINTF(x, y...)     DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR, x, ## y)
#define printf(x, y...)     DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_PRINTF, x, ## y)

void    PRINTF_MONITOR(char *fmt, ...);
void    printf_MONITOR(char *fmt, ...);

//#define PRINTF  PRINTF_MONITOR
//#define printf  printf_MONITOR

/// ======================================================================

// Define
#define RECOVER_NORMAL              0
#define RECOVER_ENTER_STOP          1
#define RECOVER_GO_MENU_TITLE       2
#define GO_LONGEST_TITLE            3
#define GO_TT_CC_1_1                4

#define ERROR_RECOVER_MODE      GO_TT_CC_1_1
// #define _DEBUG_INFO

#define MONITOR_NEXT_TIMES          3

#define MONITOR_FAIL_CHECT_TIMES        24
#define MONITOR_DVB_FAIL_CHECT_TIMES    20
#define MONITOR_OK_CHECT_TIMES          4
#define MONITOR_RETRY_COUNT             3

#define MONITOR_RESET_PROC2_CNT         5

// elmer 2.32
#ifdef SUPPORT_NO_AUDIO_IO_MUTE
#define MUTE_THRESHOLD                  10
#ifndef ENABLE_READ_AUDIO_REM
#define ENABLE_READ_AUDIO_REM
#endif //
#endif

#ifdef STACK_OVERFLOW_DETECTION
#define MONITOR_STACK_ERROR_NONE        (0x0000)
#define MONITOR_STACK_ERROR_CTKDVD      (0x0001)
#define MONITOR_STACK_ERROR_DECODER     (0x0002)
#define MONITOR_STACK_ERROR_PARSER      (0x0004)
#define MONITOR_STACK_ERROR_INFO        (0x0008)
#define MONITOR_STACK_ERROR_ATAPI       (0x0010)
#define MONITOR_STACK_ERROR_USBSRC      (0x0020)
#define MONITOR_STACK_ERROR_SERVO       (0x0040)
#define MONITOR_STACK_ERROR_SRV_BM      (0x0080)
#endif

#ifdef ERROR_CONCEAL
#ifndef ENABLE_READ_AUDIO_REM
#define ENABLE_READ_AUDIO_REM
#endif //
#ifdef SUPPORT_WATCH_DOG
#define WATCHDOG_TIME_TO_ENABLE         (COUNT_5_SEC * 6)
#define WATCHDOG_SINGAL_ENABLE          (1L<<28) //  REG_PLAT_SYSTEM_CONFIGURATION1 [28]
#define WATCHDOG_DOWN_COUNT_VALUE       0xA00000
#endif
#endif //
 
/// ======================================================================
//#define ERROR_CONCEAL
// Variables declare
BYTE    __bResetRecover=0;
DWORD   __dwMONRecoverStage;
DWORD   _dwMONTemp1,_dwMONTemp2;
DWORD   _dwMONTempRem,_dwMONTempRem1;
DWORD   _dwMONPreARemain;
DWORD   _dwMONPreAFrmCnt;
DWORD   _dwMONPreVRemain;

// elmer2.36, CVD issues
DWORD   _dwMONSPCounter;
//DWORD   _dwMONPreVFrmCnt;
DWORD   _dwMONParReadSec;
DWORD   _dwMONFailCounter, _dwMONFailCounter2;
DWORD   _dwMONOKCounter;
DWORD   _dwMONParPlayCnt;
DWORD   _dwMONParTotCnt;
DWORD   _dwMONMDecChkHang;
DWORD   _dwMONMDecHangCnt;
// This variable only used in recover stage RECOVER_STAGE_CHECK, it is because all reset flow will go stage RECOVER_STAGE_CHECK to
// check it. Then continue go next stage.
DWORD   _dwMONNextStage;
DWORD   _dwMONRecoverCnt;
DWORD   __dwMONJumpTime,__dwMONPreJumpTime;
DWORD   _dwMONGoNextStage;
DWORD   _dwMONJumpChkTime;
BYTE    _bMaxNextTimes;

// elmer 2.32
#ifdef SUPPORT_NO_AUDIO_IO_MUTE
BYTE    _bMuteCount=0;
BYTE    _bIOMuteMode=FALSE;
DWORD   _dwMONChkTime;
#endif
// Micky_PORTING_OSWRAP
/*
// wyc0.95, move these variables from debug to monitor.
cyg_handle_t    __AlarmHandle;
cyg_alarm       __AlarmObj;
*/

#ifdef SUPPORT_STB
#define MAXHANGCNT 3
BYTE __bTVHang;
BYTE __bRadioHang;
BYTE __bNoService;
BYTE __bHangCnt = 0;
DWORD   _dwDVBMONChkTime;
DWORD   _dwInterFailCnt;
extern BYTE b8InstantReplayStatus;
#endif

#ifdef SUPPORT_WATCH_DOG
DWORD _dwWatchdogPrevCounter;
DWORD _dwWatchdogFailCount = 0;
#ifdef SUPPORT_WATCHDOG_BURNIN
DWORD __dwWatchdogTime;
BYTE  _bWatchdogData;
#endif // SUPPORT_WATCHDOG_BURNIN
#endif

/// ======================================================================

// Extern Variables
extern BYTE _bHALABuf2Def;


/// ======================================================================

// Function Prototype
void _MONITOR_AudioRecover(void);
void _MONITOR_ImageRecover(void);
void _MONITOR_MotionRecover(void);
void _MONITOR_ReadAudioRem(DWORD* pdata);
WORD _MONITOR_StackStatus(VOID);
void MONITOR_InitialValue(void);

// elmer 2.32
#ifdef SUPPORT_NO_AUDIO_IO_MUTE
void    MONITOR_CheckMute(void);
BYTE    _MONITOR_CheckMuteStatus(void);
#endif

void    PRINTF_MONITOR(char *fmt, ...)
{
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, fmt);
}

void    printf_MONITOR(char *fmt, ...)
{
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, fmt);
}

//  *************************************************************************
//  Function    :   MONITOR_CheckHang
//  Description :   This function will be executed every 100 ms by ARARM.
//                  Purpose is to check the system state and trigger the 
//                  recover stage to recover system in MONITOR_RecoverSystem.
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  *************************************************************************
// Micky2.17c
#ifdef ERROR_CONCEAL
void    MONITOR_CheckHang(void)
{
    if ((__btPlaying) && (__dwMONRecoverStage == RECOVER_STAGE_IDLE))
    {
        if (__bModePlay == MODE_PLAYVIDEO)
        {
        }
        else
        {
            HAL_ReadInfo(HAL_INFO_HLI_STATUS,&_dwMONTempRem);
            // wyc1.11-909, don't call function HAL_ReadInfo because this function don't use mutex to lock it.
            _dwMONTempRem = __blHLI_Status;
#ifndef NO_DISC_MODE //CoCo2.37p
            if ((__bModePlay == MODE_PLAYMOTION) && (((__wDiscType & BOOK_DVD) && (__bStillTime || __bVOBUStill || __bDomain == VMGM_DOM || __bDomain == VTSM_DOM || _dwMONTempRem == TRUE)) || __bModeCmd == KEY_SCF || __bModeCmd == KEY_SCB || __bModeCmd == KEY_SF || __bModeCmd==KEY_STEP || __bCellAction == VOBU_TIME))
#else
            if ((__bModePlay == MODE_PLAYMOTION) && (__bModeCmd == KEY_SCF || __bModeCmd == KEY_SCB || __bModeCmd == KEY_SF || __bModeCmd==KEY_STEP))
#endif //#ifndef NO_DVD //CoCo2.37p
                return;

            _MONITOR_ReadAudioRem(&_dwMONTempRem);
            if (_dwMONPreARemain == _dwMONTempRem)
            {
                _dwMONOKCounter = 0;
                if (__bModePlay == MODE_PLAYAUDIO)
                {
                    _dwMONFailCounter+=2;
                    // wyc1.07, don't show debug message to much.
                    if ((_dwMONFailCounter % 10) == 2)
                    {
                        DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nR_S A_Rem Hold, Cnt: %lx!",_dwMONFailCounter);
                    }
                }
                else
                {
                    _dwMONFailCounter++;

                    // elmer2.36, CVD issues
                    _dwMONTempRem1 = MCU_SP1_BS_BUF_REMAINDER / 4;

                    if (_dwMONTempRem1 >= 0x100)
                        _dwMONSPCounter++;
                    else
                        _dwMONSPCounter = 0;

                    if ((_dwMONFailCounter % 5) == 1)
                    {
                        DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nR_S A_Rem Hold, Cnt: %lx!",_dwMONFailCounter);
                    }
                }
            }
            else
            {
                _dwMONOKCounter++;
                _dwMONFailCounter = 0;
                _dwMONPreARemain = _dwMONTempRem;
                // elmer2.36, CVD issues
                _dwMONSPCounter = 0;
            }
            // wyc0.95, temp disable it because need to add mutex protect in function HAL_ReadAM
            /*<<<
            HAL_ReadAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, &_dwMONTempRem); 
            if (_dwMONPreAFrmCnt == _dwMONTempRem)
            {
                _dwMONOKCounter = 0;
                _dwMONFailCounter++;
                PRINTF("\nR_S Frm Cnt Don't Update, Check Cnt: %lx!",_dwMONFailCounter);
            }
            else
            {
                _dwMONOKCounter++;
                _dwMONFailCounter = 0;
                _dwMONPreAFrmCnt = _dwMONTempRem;
            }
            >>>*/
            if (__bModePlay != MODE_PLAYAUDIO)
            {
                _dwMONTempRem = REG_VLD_MBINT_CTL;
                if ((_dwMONMDecChkHang == _dwMONTempRem) && (((_dwMONTempRem & 0x00004000)==0x00004000) || ((_dwMONTempRem & 0x00001800)==0x00001800) || ((_dwMONTempRem & 0x00007800)==0x00007800) ))
                    _dwMONMDecHangCnt++;
                else
                    _dwMONMDecHangCnt=0;
                _dwMONMDecChkHang = REG_VLD_MBINT_CTL;

                // wyc2.03-909S                
                // _dwMONTempRem = ( (REG_MCU_VREM+1) &0xffffff )*2;
                _dwMONTempRem = MCU_VIDEO_BS_BUF_REMAINDER / 4;
                
                if (_dwMONPreVRemain == _dwMONTempRem)
                {
                    _dwMONOKCounter = 0;
                    _dwMONFailCounter++;

                    // elmer2.36, CVD issues
                    _dwMONTempRem1 = MCU_SP1_BS_BUF_REMAINDER / 4;

                    if (_dwMONTempRem1 >= 0x100)
                        _dwMONSPCounter++;
                    else
                        _dwMONSPCounter = 0;

                    if ((_dwMONFailCounter % 5) == 1)
                    {
                        DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nR_S V_Rem Hold, Cnt: %lx!",_dwMONFailCounter);
                    }
                }
                else
                {
                    _dwMONOKCounter++;
                    _dwMONFailCounter = 0;
                    _dwMONPreVRemain = _dwMONTempRem;
                    // elmer2.36, CVD issues
                    _dwMONSPCounter = 0;
                }
                _dwMONTempRem = __SF_SourceGBL[__dwSFStreamID].dwReadSector;
                if (_dwMONParReadSec == _dwMONTempRem)
                {
                    _dwMONParPlayCnt = 0;
                }
                else
                {
                    _dwMONParReadSec = _dwMONTempRem;
                    _dwMONParPlayCnt++;
                    _dwMONParTotCnt++;
                }
            }
            // wyc1.08, comment it because ALARM can't read proc2 PC, otherwise proc2 PC will wrong and can't decode.
            /*<<<
            _dwMONTemp2 = REG_PLAT_PROC2_PC;
            // wyc1.07, force enter recover stage 1 to H/W reset audio, because audio PC is out of valid range.
            if ((_dwMONTemp2 > FW_DRAM_END) || (_dwMONTemp2 < FW_DRAM_ST))
            {
                printf("\nProc2 PC out of range, PC:%lx",_dwMONTemp2);
                _dwMONProc2PCWrongCnt++;
                // wyc1.08, because sometimes Proc2 PC will be wrong, so check 5 times to ensure Proc2 is really abnormal then reset it.
                if (_dwMONProc2PCWrongCnt >= MONITOR_RESET_PROC2_CNT)
                {
                    printf("\nGo H/W Reset Audio Stage by PC Worng %lx times",_dwMONTemp2,MONITOR_RESET_PROC2_CNT);
                    _dwMONFailCounter = MONITOR_FAIL_CHECT_TIMES+3;
                    _dwMONOKCounter = 0;
                    _dwMONProc2PCWrongCnt = 0;
                }
            }
            >>>*/
        }

        if (_dwMONFailCounter >= MONITOR_FAIL_CHECT_TIMES)
        {
            // wyc1.10-909, when video remainder is not empty and REG_VLD_MBINT_CTL keep in 0x00001c08 more than 10 times.
            // Then it mean MPEG dec can't exit the infinite loop and monitor need to force mpegdec break the loop.
            // _dwMONTempRem = ( (REG_MCU_VREM+1) &0xffffff )*2;
            _dwMONTempRem = MCU_VIDEO_BS_BUF_REMAINDER / 4;
            
            // wyc1.20-909, found one case video buffer will keep in video remainder 0x10, so check with KC and he said when video
            // buffer remainder is more than 6, then decoder will cont decode until less or equal 5. So choose 6 mean video buffer is not
            // empty and is safe for this mechanism to force decoder entering STOP mode.
            if ((_dwMONTempRem >= 6) && (_dwMONMDecHangCnt >= 10) && (__bModePlay == MODE_PLAYMOTION))
            {
                _dwMONMDecHangCnt = 0;
                // __dwPlayMode = MODE_STOP;
                REG_SRAM_PLAYMODE = MODE_STOP;
                DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nForce MPEGDEC Enter STOP!");
            }
            // wyc2.17-909S, make recover checking always on when debug mode for fixing video hang in state mechine problem.
            // the action stage only do in ERROR_CONCEAL defined to avoid un-expected action in 
#ifdef ERROR_CONCEAL
            _MONITOR_ReadAudioRem(&_dwMONTempRem1);
            // video buffer empty and parser just read bits about 1.8s, mean it maybe still picture and don't do error concealment.
            // wyc1.10-909, when tiem of chapter is less in title domain, mean it maybe still and don't need to do error concealment.
#ifndef NO_DISC_MODE //CoCo2.37p
            if ((_dwMONTempRem < 0x20) && (_dwMONTempRem1<=0) && (__wTimeChapter <= 5)&& (__wTimeTitle <= 10) && (__dwSeqEndCode == TRUE) && (__bModePlay != MODE_PLAYAUDIO) && (__wDiscType & BOOK_DVD)) 
            {
                // wyc1.10-909, when video almost empty and chapter time is less than 3s. It almost same as still picture and suggest don't need to do error concealment.
                {
                    DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nCancel Recover Action!");
                    _dwMONFailCounter = 0;
                }
            }
            else
#endif //#ifndef NO_DVD //CoCo2.37p
            {
                _dwMONFailCounter = 0;
                _dwMONGoNextStage = TRUE;

                // elmer2.36, CVD issues
                if ((__wDiscType & BOOK_2X) && (_dwMONSPCounter >= MONITOR_FAIL_CHECT_TIMES))
                {
                    DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR, "\nCVD in only SP mode!");
                    _dwMONNextStage = RECOVER_STAGE_NEXT_ITEM;
                    _dwMONSPCounter = 0;
                }
            }
#endif //
        }
#ifdef ERROR_CONCEAL
        if (_dwMONOKCounter >=MONITOR_OK_CHECT_TIMES)
        {
            _dwMONOKCounter = 0;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            _dwMONNextStage = RECOVER_STAGE_SW_RESET;
            _dwMONRecoverCnt = MONITOR_RETRY_COUNT;
            _bMaxNextTimes = 0;
        }
#endif //
    }
}

#ifdef CT950_STYLE
DWORD   _dwTimer;
extern  BYTE     __bUSBSRCReadError[MAX_STREAM_COUNT];
extern  DWORD    __dwMMJPGCount, __dwMMPreJPGCount;
extern  BYTE     __bMMJPEGAudioMode;
extern  DWORD    __dwFMJPGShowTime;
DWORD   __dwAudioFailCnt, __dwVideoFailCnt;
BYTE    _bAudioReset = FALSE, _bVideoReset = FALSE, _bAllReset = FALSE;
BYTE    _bAudioResetOK = TRUE, _bVideoResetOK = TRUE, _bAllResetOK = TRUE;
DWORD   _dwMONPrePCMRem, _dwMONPCMRem;

void MONITOR_CheckDMPHang(void)
{
    BYTE bTemp;

    if (!__btPlaying)
        return;

    for (bTemp = 0; bTemp < MAX_STREAM_COUNT; bTemp++)
    {
        if (__bUSBSRCReadError[bTemp] == TRUE)
        {
            __bUSBSRCReadError[bTemp] = FALSE;

            // JPEG + MUSIC mode
            if (__bMMJPEGAudioMode == TRUE)
            {
                if (bTemp == 1)
                // MUSIC
                    __dwAudioFailCnt++;
                else
                // JPEG
                    __dwVideoFailCnt++;
                PRINTF("MONITOR: Data Read Error on Stream %d\n", bTemp);
            }
        }
        else
        {
            if (__bAttrPlay & TYPE_CDROM_AUDIO)
                _bAudioReset = TRUE, _bAudioResetOK = FALSE, __dwAudioFailCnt++;

            if (__bAttrPlay & TYPE_CDROM_VIDEO)
                _bVideoReset = TRUE, _bVideoResetOK = FALSE, __dwVideoFailCnt++;

            PRINTF("MONITOR: Data Read Error\n");
        }
    }

    if (_bAllResetOK && _bAudioResetOK && (__bAttrPlay & TYPE_CDROM_AUDIO || __bMMJPEGAudioMode == TRUE))
    {
        _MONITOR_ReadAudioRem(&_dwMONTempRem);
        _dwMONPCMRem = MCU_PCM_BUF_REMAINDER;

        if (_dwMONPreARemain == _dwMONTempRem || _dwMONPrePCMRem == _dwMONPCMRem)
        {
            if (!(_dwMONFailCounter++ % 10))
                PRINTF("MONITOR: Audio Rem Hold %lx\n", _dwMONFailCounter);

            if (_dwMONFailCounter == 30)
            {
                PRINTF("MONITOR: Recover Audio Hold ...\n");
                _dwMONFailCounter = 0;
                __dwAudioFailCnt++;
                _bAudioReset = TRUE;
                _bAudioResetOK = FALSE;
            }
        }
        else
        {
            _bAudioReset = FALSE;
            _bAudioResetOK = TRUE;
            _dwMONPreARemain = _dwMONTempRem;
            _dwMONPrePCMRem = _dwMONPCMRem;
            _dwMONFailCounter = 0;
            __dwAudioFailCnt = 0;
        }
    }

    if (_bAllResetOK && _bVideoResetOK && (__bAttrPlay & TYPE_CDROM_VIDEO))
    {
        if ((OS_GetSysTimer() - _dwTimer) > __dwFMJPGShowTime * COUNT_1_SEC)
        {
            if (__dwMMJPGCount == __dwMMPreJPGCount)
            {
                if (!(_dwMONFailCounter2++ % 2))
                    PRINTF("MONITOR: Video Rem Hold %lx\n", _dwMONFailCounter2);

                if (_dwMONFailCounter2 > 10)
                {
                    PRINTF("MONITOR: Recover Video Hold ...\n");
                    _dwMONFailCounter2 = 0;
                    _bVideoReset = TRUE;
                    _bVideoResetOK = FALSE;
                }
            }
            else
            {
                _bVideoReset = FALSE;
                _bVideoResetOK = TRUE;
                __dwMMPreJPGCount = __dwMMJPGCount;
                _dwMONFailCounter2 = 0;
                __dwVideoFailCnt = 0;
            }

            _dwTimer = OS_GetSysTimer();
        }
    }

    if (_bAllResetOK && (((__dwAudioFailCnt + __dwVideoFailCnt) > 20 || __dwAudioFailCnt > 10 || __dwVideoFailCnt > 10)))
    {
        _bAudioResetOK = FALSE;
        __dwAudioFailCnt = __dwVideoFailCnt = 0;
        _bVideoReset = _bAudioReset = FALSE;
        _bVideoResetOK = _bAudioResetOK = TRUE;
        _bAllReset = TRUE;
    }


}
#endif  // CT950_STYLE
#endif  // ERROR_CONCEAL

#ifdef ERROR_CONCEAL
#ifdef SUPPORT_STB
void MONITOR_CheckDVBHang(void)
{
    if(DVB_GetAudioVideoPause() == FALSE && __btPlaying == TRUE && b8InstantReplayStatus == FALSE)
    {
        
        if( DVB_AudioGetPID() != 0)
        {
            _MONITOR_ReadAudioRem(&_dwMONTempRem);
            if (_dwMONPreARemain == _dwMONTempRem)
            {
                _dwMONOKCounter = 0;
                if (ap_ch_get_current_srv_type() == EN_SERVICE_TYPE_RADIO)
                {
                    _dwInterFailCnt++;
                    _dwMONFailCounter+=2;
                    if ((_dwMONFailCounter % 6) == 2)
                    {
                        DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nTV R_S A_Rem Hold, Cnt: %lx!",_dwMONFailCounter);
                    }
                }
                else
                {
                    _dwInterFailCnt++;                    
                    _dwMONFailCounter++;
                    if ((_dwMONFailCounter % 3) == 1)
                    {
                        DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nTV R_S A_Rem Hold, Cnt: %lx!",_dwMONFailCounter);
                    }
                }
            }
            else
            {
                __bHangCnt = 0;
                __bNoService = FALSE;
                _dwMONOKCounter++;
                _dwMONFailCounter = 0;
                _dwMONPreARemain = _dwMONTempRem;
            }

        }

        if (ap_ch_get_current_srv_type() != EN_SERVICE_TYPE_RADIO && DVB_VideoGetPID() != 0)
        {
            _dwMONTempRem = REG_VLD_MBINT_CTL;
            if ((_dwMONMDecChkHang == _dwMONTempRem) && (((_dwMONTempRem & 0x00004000)==0x00004000) || ((_dwMONTempRem & 0x00001800)==0x00001800) || ((_dwMONTempRem & 0x00007800)==0x00007800) ))
                _dwMONMDecHangCnt++;
            else
                _dwMONMDecHangCnt=0;
            _dwMONMDecChkHang = REG_VLD_MBINT_CTL;

            // wyc2.03-909S                
            // _dwMONTempRem = ( (REG_MCU_VREM+1) &0xffffff )*2;
            _dwMONTempRem = MCU_VIDEO_BS_BUF_REMAINDER / 4;
            
            if ( _dwMONTempRem == 0)
            {
                _dwInterFailCnt++;
                _dwMONFailCounter += 5;                
                if ((_dwMONFailCounter % 3) == 1)
                {
                    DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nTV R_S V_Rem Hold, Cnt: %lx!",_dwMONFailCounter);
                }                
            }              
            else if (_dwMONPreVRemain == _dwMONTempRem)
            {
                _dwMONOKCounter = 0;
                _dwInterFailCnt++;                                    
                _dwMONFailCounter++;
                if ((_dwMONFailCounter % 3) == 1)
                {
                    DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nTV R_S V_Rem Hold, Cnt: %lx!",_dwMONFailCounter);
                }
            }
            else
            {
                __bHangCnt = 0;
                __bNoService = FALSE;
                _dwMONOKCounter++;
                _dwMONFailCounter = 0;
                _dwMONPreVRemain = _dwMONTempRem;
            }
   
        }
    }        

    if (_dwMONFailCounter >= MONITOR_DVB_FAIL_CHECT_TIMES)
    {
        __bHangCnt++;
        // wyc1.10-909, when video remainder is not empty and REG_VLD_MBINT_CTL keep in 0x00001c08 more than 10 times.
        // Then it mean MPEG dec can't exit the infinite loop and monitor need to force mpegdec break the loop.
        // _dwMONTempRem = ( (REG_MCU_VREM+1) &0xffffff )*2;
        _dwMONTempRem = MCU_VIDEO_BS_BUF_REMAINDER / 4;
        
        // wyc1.20-909, found one case video buffer will keep in video remainder 0x10, so check with KC and he said when video
        // buffer remainder is more than 6, then decoder will cont decode until less or equal 5. So choose 6 mean video buffer is not
        // empty and is safe for this mechanism to force decoder entering STOP mode.
        if(ap_ch_get_current_srv_type() == EN_SERVICE_TYPE_TV)
        {
            if ((_dwMONTempRem >= 6) && (_dwMONMDecHangCnt >= 10))
            {
                _dwMONMDecHangCnt = 0;
                DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nForce MPEGDEC Enter STOP!");
            }
            __bTVHang = TRUE;            
        }
        else if( ap_ch_get_current_srv_type() == EN_SERVICE_TYPE_RADIO)
        {
            __bRadioHang = TRUE;
        }
        
        if(__bHangCnt > MAXHANGCNT)
        {
             __bNoService = TRUE;
        }
        // wyc2.17-909S, make recover checking always on when debug mode for fixing video hang in state mechine problem.
        // the action stage only do in ERROR_CONCEAL defined to avoid un-expected action in 
#ifdef ERROR_CONCEAL
        _dwMONFailCounter = 0;
#endif //
    }
    
}
#endif
#endif

void MONITOR_CheckWatchDog(void)
{
#ifdef ERROR_CONCEAL
#ifdef SUPPORT_WATCH_DOG
    REG_SRAM_WATCHDOG++;    

#ifndef NO_DISC_MODE
#ifdef SUPPORT_WATCHDOG_BURNIN
    {
        //DWORD dwSaveInt;

        // elmer2.78, support burn-in testing
        HAL_ReadStorage(SETUP_ADDR_WATCHDOG, &_bWatchdogData, EPROM_WATCHDOG_BYTE);

        //printf("_bWatchdogData = %lx\n", _bWatchdogData);

        if (_bWatchdogData == 0x78 && __dwWatchdogTime != 0 && OS_GetSysTimer() - __dwWatchdogTime > COUNT_2_SEC * 30)
        {
            _bWatchdogData = 0xFF;
            printf("Watchdog bites ...\n");
            HAL_ReadStorage(SETUP_ADDR_NVPLAYMODE, &__bNVPlayMode, EPROM_NVPLAYMODE_BYTE);
            HAL_ReadStorage(SETUP_ADDR_REPEATMODE, &__bRepeat, EPROM_REPEATMODE_BYTE);
            HAL_WriteStorage(SETUP_ADDR_WATCHDOG, &_bWatchdogData, EPROM_WATCHDOG_BYTE);
            HAL_WriteStorage(SETUP_ADDR_NVPLAYMODE, &_bWatchdogData, EPROM_NVPLAYMODE_BYTE);
            HAL_WriteStorage(SETUP_ADDR_REPEATMODE, &_bWatchdogData, EPROM_REPEATMODE_BYTE);

            if (__wDiscType & BOOK_DVD && NV_TitlePlay(1, 1))
                SMAN_PushStateInfo (STATE_PG, STATUS_READY);
        }
    }
#endif
#endif // !CT950_STYLE
#endif //
#endif //

/*<<<
    // wyc2.15-909S, support watch dog.
#ifdef ERROR_CONCEAL
#ifdef SUPPORT_WATCH_DOG
    if (HAL_WatchDog_Status())
        HAL_WatchDog_Reset();
#endif //
#endif //
>>>*/
}

#ifdef CT950_STYLE
void MONITOR_ResetAV(DWORD dwStreamId)
{
    DWORD dwTimer;

    switch (dwStreamId)
    {
    case 0:
        HALJPEG_Reset();
        break;
    case 1:
        HAL_Reset(HAL_RESET_AUDIO);
        HAL_WriteAM(HAL_AM_PLAY_COMMAND, 0);
        PARSER_BufferReset(PARSER_ES_AUDIO);
        break;
    }

    PARSER_Command(dwStreamId, PARSER_CMD_STOP, 0);

    dwTimer = OS_GetSysTimer();
    while((OS_GetSysTimer() - dwTimer) < COUNT_50_MSEC)
    {
        if (PARSER_QueryEvent(dwStreamId, PARSER_EVENT_COMPLETED, FALSE))
            break;

        OS_YieldThread();
    }

    SrcFilter_Stop(dwStreamId);

    return;
}
#endif

//  *************************************************************************
//  Function    :   MONITOR_RecoverSystem
//  Description :   This function is called by CC and purpose is to recover 
//                  system to normal state. There are several stages to recover  
//                  system in this function. 
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  *************************************************************************
void MONITOR_RecoverSystem(void)
{
#ifdef ERROR_CONCEAL

    extern void _MM_JPEGChangeToNext(void);
    extern void MM_PhotoMusicErrorConcealment(void);

    if (__btPlaying)
    {
#ifdef SUPPORT_STB
        if( __bChooseMedia == MEDIA_SELECT_STB)
        {
            _MONITOR_DVBRecover();
        }
        else
#endif
        {
#ifdef CT950_STYLE
            if (_bAudioReset)
            {
                PRINTF("Recovering Audio ...\n");
                _bAudioReset = FALSE;

                if (__bMMJPEGAudioMode)
                {
                    MONITOR_ResetAV(1);
                }
                else
                {
                    HAL_Reset(HAL_RESET_AUDIO);
                    __bFWKey = KEY_NEXT;
                }
                OS_REORDER_BARRIER();
                _bAudioResetOK = TRUE;
            }

            if (_bVideoReset)
            {
                PRINTF("Recovering Video ...\n");
                _bVideoReset = FALSE;

                if (__bMMJPEGAudioMode)
                    MONITOR_ResetAV(0);
                else
                    _MM_JPEGChangeToNext();

                OS_REORDER_BARRIER();
                _bVideoResetOK = TRUE;
            }

            if (_bAllReset)
            {
                PRINTF("Recovering System ...\n");
                _bAllReset = FALSE;
                MM_PhotoMusicErrorConcealment();
                OS_REORDER_BARRIER();
                _bAllResetOK = TRUE;
            }


#else // !CT950_STYLE
            if (__bModePlay == MODE_PLAYAUDIO)
                _MONITOR_AudioRecover();
            else if (__bModePlay == MODE_PLAYVIDEO)
                _MONITOR_ImageRecover();
            else
                _MONITOR_MotionRecover();
#endif // CT950_STYLE
        }
    }
#endif 
    // elmer2.32
#ifdef SUPPORT_NO_AUDIO_IO_MUTE
    if ((OS_GetSysTimer() - _dwMONChkTime) >= COUNT_100_MSEC)
    {
        MONITOR_CheckMute();
        _dwMONChkTime = OS_GetSysTimer();
    }
#endif
}

#ifdef SUPPORT_STB
void _MONITOR_DVBRecover(void)
{
#ifdef ERROR_CONCEAL
    DWORD dwTimeStart;
    DWORD dwVRem, dwARem;
    if( _dwInterFailCnt > 0 && ((OS_GetSysTimer() - _dwDVBMONChkTime) >= COUNT_1_SEC) )
    {
        _dwDVBMONChkTime = OS_GetSysTimer();
        if ( _dwInterFailCnt > 12 )
        {
            __bTVHang = TRUE;
        }
        _dwInterFailCnt = 0;
    }

    if(__bTVHang || __bRadioHang)
    {
        DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nTV or Radio Recover!");        
        HAL_WriteAM(HAL_AM_PLAY_COMMAND, 0);
        HAL_ResetAudioDecoder();
        if(__bTVHang)
        {
            {
            extern BYTE    _Wait_Decoder_Stop_CMD_ACK(void);
            COMDEC_Command(MODE_STOP, 0);
            _Wait_Decoder_Stop_CMD_ACK();            
            }
        }
        
        DVB_AVDemuxDisable();
        DVB_AVDemuxEnable();
        dwTimeStart = OS_GetSysTimer();
        while( (OS_GetSysTimer() - dwTimeStart) < COUNT_100_MSEC)
        {
            HAL_ReadInfo(HAL_INFO_VIDEO_REMAIN, &dwVRem);
            HAL_ReadInfo(HAL_INFO_AUDIO_REMAIN, &dwARem);
            if(dwVRem!=0 || dwARem!=0)
            {
                DVB_AudioVideoPlay();        
                break;
            }     
            OS_YieldThread();
        }

        __bTVHang = FALSE;
        __bRadioHang = FALSE;    
        _dwInterFailCnt = 0;
    }
    
#endif
}
#endif

void _MONITOR_AudioRecover(void)
{
#ifdef ERROR_CONCEAL
    switch (__dwMONRecoverStage)
    {
        case    RECOVER_STAGE_PREPARE:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nR_S Prepare!");
            _dwMONFailCounter = 0;
            _dwMONOKCounter = 0;
            _dwMONParTotCnt = 0;
            _dwMONRecoverCnt = MONITOR_RETRY_COUNT;
            _dwMONGoNextStage = FALSE;
            _MONITOR_ReadAudioRem(&_dwMONPreARemain);
            _dwMONParReadSec = __SF_SourceGBL[__dwSFStreamID].dwReadSector;
            //HAL_ReadAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, &_dwMONPreAFrmCnt);
            _dwMONNextStage = RECOVER_STAGE_SW_RESET;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
        case    RECOVER_STAGE_IDLE:
            if (_dwMONGoNextStage == TRUE)
            {
                __dwMONRecoverStage = _dwMONNextStage;
                _dwMONGoNextStage = FALSE;
                _dwMONFailCounter = 0;
                _dwMONOKCounter = 0;
            }
            break;
        case    RECOVER_STAGE_SW_RESET:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nR_S S/W Reset Audio!");
            HAL_Reset(HAL_RESET_AUDIO);
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);
            _dwMONRecoverCnt--;
            if (_dwMONRecoverCnt == 0)
            {
                _dwMONRecoverCnt = MONITOR_RETRY_COUNT;
                // When read / write sector is same and not reach end sector, mean SERVO don't read data any more and just issue JUMP sector to read another position data.
                if (1)//(__SF_SourceGBL[0].dwWriteSector == __SF_SourceGBL[0].dwReadSector) && (__SF_SourceGBL[0].dwWriteSector <= __SF_SourceGBL[0].dwEndSector))
                    _dwMONNextStage = RECOVER_STAGE_JUMP_SECTOR;
                else
                    _dwMONNextStage = RECOVER_STAGE_HW_RESET;
            }
            else
                _dwMONNextStage = RECOVER_STAGE_SW_RESET;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
        case    RECOVER_STAGE_JUMP_SECTOR:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nR_S Jump Sectors!");
            // wyc2.53-909P,
            SrcFilter_Stop(__dwSFStreamID);            
            HAL_ControlParser(HAL_PARSER_STOP, 0);
            CHIPS_SoftMute();
            
            HAL_Reset(HAL_RESET_AUDIO);              

            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);      

            CHIPS_OpenAudio(TRUE);
            if (__wDiscType & BOOK_CDDA)
            {
                // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
                UTL_PlayFromTime(__dwTimeNow+20*(MONITOR_RETRY_COUNT+1-_dwMONRecoverCnt), __dwTimeEnd);
            }
            else
            {
                HAL_ReadAM(HAL_AM_BIT_RATE, &_dwMONTemp1);
                // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
                UTL_PlayFromTime(__dwTimeNow+(((10*_dwMONTemp1*1000)/(2048*8))*(MONITOR_RETRY_COUNT+1-_dwMONRecoverCnt)), __dwTimeEnd);
            }
            _dwMONRecoverCnt--;
            if (_dwMONRecoverCnt == 0)
            {
                _dwMONRecoverCnt = MONITOR_RETRY_COUNT;
                _dwMONNextStage = RECOVER_STAGE_HW_RESET;
            }
            else
                _dwMONNextStage = RECOVER_STAGE_JUMP_SECTOR;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
        case    RECOVER_STAGE_HW_RESET:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nR_S H/W Reset!");
            _dwMONNextStage = RECOVER_STAGE_RESET_SERVO;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
        case    RECOVER_STAGE_RESET_SERVO:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nR_S Reset Servo!");
            SrcFilter_Reset(TRUE);
            HAL_ControlParser(HAL_PARSER_STOP, 0);
            PARSER_Command(0, PARSER_CMD_START, 0);
            _dwMONNextStage = RECOVER_STAGE_NEXT_ITEM;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
        case    RECOVER_STAGE_NEXT_ITEM:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nR_S Next Item!");
            __bFWKey = KEY_NEXT;
            // elmer2.36
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
    }
#endif //
}

void _MONITOR_ImageRecover(void)
{
#ifdef ERROR_CONCEAL

#endif //
}

void _MONITOR_MotionRecover(void)
{
#ifdef ERROR_CONCEAL
    switch (__dwMONRecoverStage)
    {
        case    RECOVER_STAGE_PREPARE:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_MM_INFOFTR,"\nR_S Prepare!");
            _dwMONFailCounter = 0;
            _dwMONOKCounter = 0;
            _dwMONParTotCnt= 0;
            _dwMONMDecHangCnt = 0;
            _dwMONMDecChkHang = 0;
            _dwMONRecoverCnt = MONITOR_RETRY_COUNT;
            _dwMONGoNextStage = FALSE;
            __dwMONJumpTime = 0;
            __dwMONPreJumpTime = 0xFFFFFFFF;
            _dwMONJumpChkTime = 0;
            _MONITOR_ReadAudioRem(&_dwMONPreARemain);
            // _dwMONPreVRemain = ( (REG_MCU_VREM+1) &0xffffff )*2;
            _dwMONPreVRemain = MCU_VIDEO_BS_BUF_REMAINDER / 4;
            _dwMONParReadSec = __SF_SourceGBL[__dwSFStreamID].dwReadSector;
            _dwMONNextStage = RECOVER_STAGE_SW_RESET;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
        case    RECOVER_STAGE_IDLE:
            if (_dwMONGoNextStage == TRUE)
            {
                __dwMONRecoverStage = _dwMONNextStage;
                _dwMONGoNextStage = FALSE;
                _dwMONFailCounter = 0;
                _dwMONOKCounter = 0;
            }

            break;
        case    RECOVER_STAGE_SW_RESET:
            // wyc2.60-909P, make it default printf.
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_ASSERTION,"\nR_S S/W Reset A/V!");
            HAL_Reset(HAL_RESET_AUDIO);
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);
            HAL_Reset(HAL_RESET_SPST);

            /*COMDEC_Command(MODE_STOP, 0);
            _dwMONTemp2 = UTL_GetSysTimer();    
            while( (UTL_GetSysTimer()-_dwMONTemp2) < COUNT_50_MSEC )
            {
                //COMDEC_Command query later
                HAL_ReadInfo(HAL_INFO_DECCMD_ACK, &_dwMONTemp1);
                if (_dwMONTemp1 == MODE_STOPPED)
                    break;
                //release CPU resource to other threads 
                cyg_thread_yield();            
            }    
            
            _dwMONTemp1 = REG_MCU_DCR01;
            HAL_DISABLE_INTERRUPTS(_dwMONTemp2);
            // Chuan0.66, Use PLAT_RESET_VDEC instead of REG_VLD_INFO 
            MACRO_PLAT_KEY_LOCK( );
            REG_PLAT_RESET_CONTROL &= ~(PLAT_RESET_VDEC);   // disable the VDEC
            MACRO_PLAT_KEY_UNLOCK( );
            //MACRO_PLAT_RESET_WAITSTABLE( );
            //REG_VLD_INFO = 0;
            MACRO_MCU_RESET_VIDEO_FIFO;   // Clear video Fifo
            HAL_RESTORE_INTERRUPTS(_dwMONTemp2);
            REG_MCU_DCR00 = _dwMONTemp1;
            COMDEC_Command(MODE_FORCE_IP, 0);
            COMDEC_Command(MODE_PLAY, 0);*/

            /*HAL_PlayCommand(COMMAND_STOP,0);
            HAL_Reset(HAL_RESET_VIDEO);
            HAL_PlayCommand(COMMAND_PLAY,0);*/
            _dwMONRecoverCnt--;
            if (_dwMONRecoverCnt == 0)
            {
                _dwMONRecoverCnt = MONITOR_RETRY_COUNT;
                // When read / write sector is same and not reach end sector, mean SERVO don't read data any more and just issue JUMP sector to read another position data.
                if (1)//(__SF_SourceGBL[0].dwWriteSector == __SF_SourceGBL[0].dwReadSector) && (__SF_SourceGBL[0].dwWriteSector <= __SF_SourceGBL[0].dwEndSector))
                    _dwMONNextStage = RECOVER_STAGE_JUMP_SECTOR;
                else
                    _dwMONNextStage = RECOVER_STAGE_HW_RESET;
            }
            else
                _dwMONNextStage = RECOVER_STAGE_SW_RESET;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
        case    RECOVER_STAGE_JUMP_SECTOR:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_ASSERTION,"\nR_S Jump Sectors!");
#ifndef NO_DISC_MODE //CoCo2.37p
            if (__wDiscType & BOOK_DVD)
            {
                if (__dwMONJumpTime == 0)
                {
                    _dwMONTemp1 = CELL_QueryTime();
                    __dwMONJumpTime = __wTimeChapterBegin + (WORD)_dwMONTemp1 + 20;
                    if (((__dwMONJumpTime > __dwMONPreJumpTime) && ((__dwMONJumpTime - __dwMONPreJumpTime) < 100)) || 
                        ((__dwMONPreJumpTime > __dwMONJumpTime) && ((__dwMONPreJumpTime - __dwMONJumpTime) < 100)))
                    {
                        // when this time jump position and last time is same, mean CELL return wrong position or F/W enter infinite
                        // loop, so monitor will break it here to avoid infinite loop.
                        _dwMONRecoverCnt = 1;
                        DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_ASSERTION,"\nJump Same Position!");
                    }
                    __dwMONPreJumpTime = __dwMONJumpTime;
                }
                else
                {
                    __dwMONJumpTime += (50*(MONITOR_RETRY_COUNT - _dwMONRecoverCnt + 1));
                }
                if (__dwMONJumpTime >= __wTimeTitle )
                    __dwMONJumpTime = __wTimeTitle;
                if (__dwMONJumpTime <= _dwMONJumpChkTime)
                {
                    _dwMONRecoverCnt = MONITOR_RETRY_COUNT;
                    _dwMONNextStage = RECOVER_STAGE_NEXT_ITEM;
                    __dwMONRecoverStage = RECOVER_STAGE_IDLE;
                    break;
                }
                else
                {
                    _dwMONPreARemain = (__dwMONJumpTime / 3600);
                    _dwMONPreAFrmCnt = ((__dwMONJumpTime % 3600) / 60);
                    _dwMONPreVRemain = ((__dwMONJumpTime % 3600) % 60);
                    _dwMONTemp1=MAKE_TMSF(_dwMONPreARemain, (BYTE)_dwMONPreAFrmCnt, (BYTE)_dwMONPreVRemain, 0x0);
                    DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_ASSERTION,"\nDVD Jump Time: %lx!",_dwMONTemp1);
                    // wyc1.08, CSC suggested to issue source stop and parser stop before gototime, otherwise parser easily show
                    // parser stop failed and abnormal.
                    SrcFilter_Stop(__dwSFStreamID);            
                    HAL_ControlParser(HAL_PARSER_STOP, 0);
                    NV_TimePlay(__bTitle, _dwMONTemp1);
                    HAL_Reset(HAL_RESET_VIDEO); //CoCo.050
                    // DVD_103Micky, fix GoToTime will hang issue
                    // need restore the pla command
                    UTL_TransferCommandID(__bModeCmd);
                    // Micky1.07_909 // wyc1.07-909
                    _bJumpTime = TRUE;
                }
                _dwMONJumpChkTime = __dwMONJumpTime;
            }
            else
#endif //#ifndef NO_DVD //CoCo2.37p
            {
                SrcFilter_Stop(__dwSFStreamID);            
                HAL_ControlParser(HAL_PARSER_STOP, 0);
                CHIPS_SoftMute();
                
                HAL_Reset(HAL_RESET_AUDIO);              
                HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);
                HAL_Reset(HAL_RESET_VIDEO);
                HAL_PlayCommand(COMMAND_PLAY,0);

                CHIPS_OpenAudio(TRUE);
                // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
                UTL_PlayFromTime(__dwTimeNow+(75*5*(MONITOR_RETRY_COUNT - _dwMONRecoverCnt + 1)), __dwTimeEnd);
            }
            _dwMONRecoverCnt--;
            if (_dwMONRecoverCnt == 0)
            {
                _dwMONRecoverCnt = MONITOR_RETRY_COUNT;
                _dwMONNextStage = RECOVER_STAGE_HW_RESET;
            }
            else
                _dwMONNextStage = RECOVER_STAGE_JUMP_SECTOR;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
        case    RECOVER_STAGE_HW_RESET:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_ASSERTION,"\nR_S H/W Reset!");
            _dwMONNextStage = RECOVER_STAGE_RESET_SERVO;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
        case    RECOVER_STAGE_RESET_SERVO:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_ASSERTION,"\nR_S Reset Servo!");
            SrcFilter_Reset(TRUE);
            HAL_ControlParser(HAL_PARSER_STOP, 0);
            PARSER_Command(0, PARSER_CMD_START, 0);
            _dwMONNextStage = RECOVER_STAGE_NEXT_ITEM;
            __dwMONRecoverStage = RECOVER_STAGE_IDLE;
            break;
        case    RECOVER_STAGE_NEXT_ITEM:
            DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_ASSERTION,"\nR_S Next Item!");
#ifndef NO_DISC_MODE //CoCo2.37p
            if (__wDiscType & BOOK_DVD)
            {
                if ( !NV_Action ( ACTION_NEXT, (WORD)NULL) )    // don't allow next
                {
                    // ** 2.76-TCHM; begin...
                    // DVD-Title, 1/16(PBC) KEY_NEXT inhibit. Just go MENU_ROOT. If can't, do STOP.
                    if ( __bNVPlayMode )                        
                    {
                        if ( ! NV_MenuCall ( MENU_ROOT ) )
                        {
                            // LLY2.37p, assign KEY_STOP from F/W 
                            // To avoid IR and F/W key conflict issue
                            //__bISRKey= KEY_STOP;
                            __bFWKey=KEY_STOP;
                        }
                    }
                    else    
                    {   // Non-NV mode, just jump to TT/CC= 1/1
                        NV_TitlePlay ( 1, 1 );
                    }
                    // ** 2.76-TCHM; end... 
                }
                else
                {
                    _bMaxNextTimes++;
                    if (_bMaxNextTimes >= MONITOR_NEXT_TIMES)
                    {
                        _bMaxNextTimes = 0;
#if (ERROR_RECOVER_MODE == GO_LONGEST_TITLE)
                        if ( __bSmartPB_ABLE )
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
#elif (ERROR_RECOVER_MODE == GO_TT_CC_1_1)
                        if ( NV_TitlePlay(1, 1))
                        {
                            SMAN_PushStateInfo ( STATE_PG, STATUS_READY );
                        }
                        else
                        {
                            __bFWKey = KEY_STOP;
                        }
#endif //
                    }
                }
            }
            else
#endif// #ifndef NO_DVD //CoCo2.37p
            {
                // ** TCH1.61-1; begin... If VCD 2.0 and PBC mode. Check the valid of LIST_NEXT
                // If invalid, let's to LINEAR mode
#ifndef NO_DISC_MODE //++CoCo2.37p
                if ( __wDiscType & VCD_2_0 )
                {
                    if ( __btPBC )
                    {
                        if ( ! VCD20_IsValid_Command ( LIST_NEXT ) )
                        {
                            __bFWKey=KEY_PBC;
                            break;
                        }
                    }
                }
#endif //#ifndef NO_VCD20 //++CoCo2.37p
                // wyc2.36-909S, power doen servo when next to end of fuss scratched VCD to avoid SERVO abnormal.
                if ((__bRepeat == REPEAT_NONE) && ( __wPlayItem == __wTrackEnd ))
                {
                    SrcFilter_PowerMode (SERVO_POWER_STOP);
                    __bFWKey=KEY_STOP;
                }
                else
                {
                    __bFWKey=KEY_NEXT;
                    // elmer2.36, for parser stop fail issue
                    if ( !HAL_ControlParser(HAL_PARSER_STOP, 0))
                    {
                        OS_SuspendThread(ThreadPARSER.handle);
                        OS_KillThread(ThreadPARSER.handle);
                        INITIAL_ThreadInit(THREAD_PARSER);
                        printf("===========delete parser thread============");
                    }
                }
                // wyc2.36-909S, power doen servo when next to end of fuss scratched VCD to avoid SERVO abnormal.
                __dwMONRecoverStage = RECOVER_STAGE_PREPARE;
                break;
            }
    }
#if 0//def ERROR_CONCEAL
    // Don't allow entering error concealment mode when still picture.
    HAL_ReadInfo(HAL_INFO_STILL,&_dwMONTemp1);
    if (__bStillTime || __bVOBUStill || _dwMONTemp1 == TRUE || __bModeCmd == KEY_SCF || __bModeCmd == KEY_SCB)
        return;
    switch (__dwMONRecoverStage)
    {
        case    RECOVER_STAGE_PREPARE:
            PRINTF("\nR_S Prepare!");
            _dwMONNextStage = RECOVER_STAGE_CHECK;
            __dwMONRecoverStage = _dwMONNextStage;
            _dwMONCheckTime = UTL_GetSysTimer();
            _dwMONFailCounter = 0;
            _dwMONCheckVCounter = 0;
            _dwMONRecoverCnt = 0;
            _dwMONJumpCnt = 0;
            __dwMONJumpTime = 0;
            _MONITOR_ReadAudioRem(&_dwMONPreARemain);
            // _dwMONPreVRemain = ( (REG_MCU_VREM+1) &0xffffff )*2;
            _dwMONPreVRemain = MCU_VIDEO_BS_BUF_REMAINDER / 4;
            break;
        case    RECOVER_STAGE_CHECK:
            if (_dwMONNextStage == RECOVER_STAGE_CHECK)
            {
                if ((__SF_SourceGBL[0].dwWriteSector == __SF_SourceGBL[0].dwReadSector) && (__SF_SourceGBL[0].dwWriteSector <= __SF_SourceGBL[0].dwEndSector))
                    _dwMONNextStage = RECOVER_STAGE_JUMP_SECTOR;
                else
                    _dwMONNextStage = RECOVER_STAGE_SW_RESET;
            }
            if ((UTL_GetSysTimer() - _dwMONCheckTime) >= COUNT_300_MSEC)
            {
                _MONITOR_ReadAudioRem(&_dwMONTemp1);
                if (_dwMONPreARemain == _dwMONTemp1)
                {
                    _dwMONFailCounter++;
                    PRINTF("\nR_S A_Rem Don't Update, Check Cnt: %lx!",_dwMONFailCounter);
                }
                else
                {
                    _dwMONFailCounter = 0;
                    _dwMONCheckVCounter = 0;
                    _dwMONPreARemain = _dwMONTemp1;
                }
                // _dwMONTemp1 = ( (REG_MCU_VREM+1) &0xffffff )*2;
                _dwMONTemp1 = MCU_VIDEO_BS_BUF_REMAINDER / 4;
                if (_dwMONPreVRemain == _dwMONTemp1)
                {
                    _dwMONCheckVCounter++;
                    PRINTF("\nR_S V_Rem Don't Update, Check Cnt: %lx!",_dwMONCheckVCounter);
                }
                else
                {
                    _dwMONFailCounter = 0;
                    _dwMONCheckVCounter = 0;
                    _dwMONPreVRemain = _dwMONTemp1;
                }
                _dwMONCheckTime = UTL_GetSysTimer();
            }
            if ((_dwMONFailCounter + _dwMONCheckVCounter) >= MONITOR_FAIL_CHECT_TIMES)
            {
                __dwMONRecoverStage = _dwMONNextStage;
            }
            break;
        case    RECOVER_STAGE_SW_RESET:
            PRINTF("\nR_S S/W Reset A/V!");
            HAL_Reset(HAL_RESET_AUDIO);
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);
            HAL_Reset(HAL_RESET_VIDEO);
            HAL_PlayCommand(COMMAND_PLAY,0);
            _MONITOR_CheckStageInitVars(6);
            _dwMONRecoverCnt++;
            if (_dwMONRecoverCnt > 3)
                _dwMONNextStage = RECOVER_STAGE_JUMP_SECTOR;
            else
                _dwMONNextStage = RECOVER_STAGE_SW_RESET;
            break;
        case    RECOVER_STAGE_JUMP_SECTOR:
            PRINTF("\nR_S Jump Sectors!");
#ifndef NO_DISC_MODE //CoCo2.37p
            if (__wDiscType & BOOK_DVD)
            {
                if (__dwMONJumpTime == 0)
                {
                    _dwMONTemp1 = CELL_QueryTime();
                    __dwMONJumpTime = __wTimeChapterBegin + (WORD)_dwMONTemp1 + 10;
                    if (((__dwMONJumpTime > __dwMONPreJumpTime) && ((__dwMONJumpTime - __dwMONPreJumpTime) > 100)) || 
                        ((__dwMONPreJumpTime > __dwMONJumpTime) && ((__dwMONPreJumpTime - __dwMONJumpTime) > 100)))
                    {
                        // when this time jump position and last time is same, mean CELL return wrong position or F/W enter infinite
                        // loop, so monitor will break it here to avoid infinite loop.
                        _dwMONJumpCnt = 6;
                        PRINTF("\nJump Same Position!");
                    }
                    __dwMONPreJumpTime = __dwMONJumpTime;
                }
                else
                {
                    __dwMONJumpTime += (10*(_dwMONJumpCnt+1));
                }
                if (__dwMONJumpTime >= __wTimeTitle )
                    __dwMONJumpTime = __wTimeTitle;
                _dwMONPreARemain = (__dwMONJumpTime / 3600);
                _dwMONPreAFrmCnt = ((__dwMONJumpTime % 3600) / 60);
                _dwMONPreVRemain = ((__dwMONJumpTime % 3600) % 60);
                _dwMONTemp1=MAKE_TMSF(_dwMONPreARemain, (BYTE)_dwMONPreAFrmCnt, (BYTE)_dwMONPreVRemain, 0x0);
                NV_TimePlay(__bTitle, _dwMONTemp1);
                HAL_Reset(HAL_RESET_VIDEO); //CoCo.050
                // DVD_103Micky, fix GoToTime will hang issue
                // need restore the pla command
                UTL_TransferCommandID(__bModeCmd);
            }
            else
#endif //#ifndef NO_DVD //CoCo2.37p
            {
                SrcFilter_Stop();            
                HAL_ControlParser(HAL_PARSER_STOP, 0);
                CHIPS_SoftMute();
                
                HAL_Reset(HAL_RESET_AUDIO);              
                HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);
                HAL_Reset(HAL_RESET_VIDEO);
                HAL_PlayCommand(COMMAND_PLAY,0);

                CHIPS_OpenAudio(TRUE);
                HAL_ReadAM(HAL_AM_BIT_RATE, &_dwMONTemp1);
                // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
                UTL_PlayFromTime(__dwTimeNow+(75*5*(_dwMONJumpCnt+1)), __dwTimeEnd);
            }
            _MONITOR_CheckStageInitVars(6);
            _dwMONJumpCnt++;
            if (_dwMONJumpCnt > 5)
                _dwMONNextStage = RECOVER_STAGE_HW_RESET;
            else
                _dwMONNextStage = RECOVER_STAGE_JUMP_SECTOR;
            break;
        case    RECOVER_STAGE_HW_RESET:
            PRINTF("\nR_S H/W Reset!");
            _MONITOR_CheckStageInitVars(6);
            _dwMONNextStage = RECOVER_STAGE_RESET_SERVO;
            break;
        case    RECOVER_STAGE_RESET_SERVO:
            PRINTF("\nR_S Reset Servo!");
            _MONITOR_CheckStageInitVars(6);
            SrcFilter_Reset(TRUE);
            HAL_ControlParser(HAL_PARSER_STOP, 0);
            PARSER_Command(0, PARSER_CMD_START, 0);
            _dwMONNextStage = RECOVER_STAGE_NEXT_ITEM;
            break;
        case    RECOVER_STAGE_NEXT_ITEM:
            PRINTF("\nR_S Next Item!");
            // when Next don't allow, just enter menu
#ifndef NO_DISC_MODE //CoCo2.37p
            if (__wDiscType & BOOK_DVD)
            {
                if ( !NV_Action ( ACTION_NEXT, (WORD)NULL) )    // don't allow next
                {
                    // ** 2.76-TCHM; begin...
                    // DVD-Title, 1/16(PBC) KEY_NEXT inhibit. Just go MENU_ROOT. If can't, do STOP.
                    if ( __bNVPlayMode )                        
                    {
                        if ( ! NV_MenuCall ( MENU_ROOT ) )
                        {
                            // LLY2.37p, assign KEY_STOP from F/W 
                            // To avoid IR and F/W key conflict issue
                            //__bISRKey= KEY_STOP;
                            __bFWKey=KEY_STOP;
                        }
                    }
                    else    
                    {   // Non-NV mode, just jump to TT/CC= 1/1
                        NV_TitlePlay ( 1, 1 );
                    }
                    // ** 2.76-TCHM; end... 
                }
            }
            else
#endif //#ifndef NO_DVD //CoCo2.37p
            {
                // ** TCH1.61-1; begin... If VCD 2.0 and PBC mode. Check the valid of LIST_NEXT
                // If invalid, let's to LINEAR mode
#ifndef NO_DISC_MODE //++CoCo2.37p
                if ( __wDiscType & VCD_2_0 )
                {
                    if ( __btPBC )
                    {
                        if ( ! VCD20_IsValid_Command ( LIST_NEXT ) )
                        {
                            __bFWKey=KEY_PBC;
                            break;
                        }
                    }
                }
#endif //#ifndef NO_VCD20 //++CoCo2.37p
                __bFWKey=KEY_NEXT;
            }
            break;
    }
#endif //
#endif //
}

void    MONITOR_InitialValue(void)
{
    __dwMONRecoverStage = 0;
    // wyc1.02-909, initial it here.
    __bResetRecover = 0;
    _bMaxNextTimes = 0;
}

void    MONITOR_Debug(void)
{
    // ** TCH1.07; begin... 
    DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_PRINTF, "\nCC PlayMode=%hx, PlayCmd=%hx; Playing %hx, %hx, %hx;", __bModePlay, __bModeCmd, __btPlaying, __btPause, __btPlayEnd );
#ifndef NO_DISC_MODE //CoCo2.37p
    DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_PRINTF, "\nCC PBC %hx, NV %hx,", __btPBC, __bNVPlayMode );
#endif //#ifndef NO_DVD //CoCo2.37p

    // ** TCH1.07; end... 
    // wyc1.02a-909, re-arrange the printf message.
    DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_PRINTF,"\nR_S:%lx,play mode:%hx,A/V Rem:%lx, %lx",__dwMONRecoverStage,__btPlaying,_dwMONPreARemain,_dwMONPreVRemain);
    DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_PRINTF,"\nSE_Code:%lx, PPCnt:%lx, PTCnt:%lx",__dwSeqEndCode,_dwMONParPlayCnt,_dwMONParTotCnt);
}
// Micky2.17c
#ifdef ENABLE_READ_AUDIO_REM
void    _MONITOR_ReadAudioRem(DWORD* pdata)
{
    /*
    if (_bHALABuf2Def)
    {
        *pdata = ( (REG_MCU_A1REM+1) & 0xffffff ) *2; //DW
    }            
    else
    {        
        *pdata = 0;
    }            
    *pdata += ( (REG_MCU_A0REM+1) & 0xffffff ) *2; //DW
    */
    if (_bHALABuf2Def)
    {
        *pdata = MCU_A1_BS_BUF_REMAINDER;
    }            
    else
    {        
        *pdata = 0;
    }            
    *pdata += MCU_A0_BS_BUF_REMAINDER;
    *pdata /= 4;    // Unit DW
}
#endif  //#ifdef ERROR_CONCEAL
void    MONITOR_AlarmInit(void)
{
    //Micky_PORTING_OSWRAP
        ALARM_INFO  AlarmInfo;

    AlarmInfo.fpAlarmFunction = MONITOR_INTERRUPT;
        AlarmInfo.wEntryData = 0;
    AlarmInfo.dwInterval = SLICE_TICK;      
       OS_InitialAlarm(&AlarmInfo);
/*
    cyg_alarm_create(__ClockHandle, MONITOR_INTERRUPT,
             (cyg_addrword_t) NULL,
             &__AlarmHandle, &__AlarmObj);
    cyg_alarm_initialize(__AlarmHandle, cyg_current_time()+1, 2);
*/    
}

//VOID    MONITOR_INTERRUPT(cyg_handle_t alarmH, cyg_addrword_t data)
// Micky2.17c, can't use data as parameter
VOID    MONITOR_INTERRUPT(HANDLE_T alarmH, WORD data1)
{
    DBG_INT();
// elmer2.37, move the watchdog mechanism to from CC to alarm
#ifdef  SUPPORT_WATCH_DOG
    if ((REG_PLAT_SYSTEM_CONFIGURATION1 & WATCHDOG_SINGAL_ENABLE))
    {
        REG_PLAT_WATCHDOG = WATCHDOG_DOWN_COUNT_VALUE;
    }
#endif

    if (!(OS_GetSysTimer() % COUNT_200_MSEC))
    {
#ifdef STACK_OVERFLOW_DETECTION        
        _MONITOR_StackStatus();
#endif

#ifdef ERROR_CONCEAL
#ifdef SUPPORT_STB
        if( __bChooseMedia == MEDIA_SELECT_STB)
        {
            MONITOR_CheckDVBHang();
        }
        else
#endif //SUPPORT_STB            
        {
            MONITOR_CheckHang();
        }
#endif

#ifdef SUPPORT_WATCH_DOG
        // elmer2.37, move the watchdog mechanism to from CC to alarm
        if (REG_SRAM_WATCHDOG != _dwWatchdogPrevCounter)
        {
            _dwWatchdogFailCount = 0;
        }
        else
        {
            _dwWatchdogFailCount++;

            if (_dwWatchdogFailCount >= (WATCHDOG_TIME_TO_ENABLE / COUNT_200_MSEC))
            {
#ifndef NO_DISC_MODE
#ifdef SUPPORT_WATCHDOG_BURNIN
                // elmer2.78, support burn-in testing
                _bWatchdogData = 0x78;
                HAL_WriteStorage(SETUP_ADDR_WATCHDOG, &_bWatchdogData, EPROM_WATCHDOG_BYTE);
                HAL_WriteStorage(SETUP_ADDR_NVPLAYMODE, &__bNVPlayMode, EPROM_NVPLAYMODE_BYTE);
                HAL_WriteStorage(SETUP_ADDR_REPEATMODE, &__bRepeat, EPROM_REPEATMODE_BYTE);
#endif
#endif
                HAL_REORDER_BARRIER();
                _dwWatchdogFailCount = 0;
                REG_PLAT_WATCHDOG = 0x000000;
            }
        }
        _dwWatchdogPrevCounter =  REG_SRAM_WATCHDOG;
#endif
    }
}

// elmer2.32
#ifdef SUPPORT_NO_AUDIO_IO_MUTE
void    MONITOR_CheckMute(void)
{
    DWORD dwTemp;

    if ( __btPlaying )
    {
        _MONITOR_ReadAudioRem(&dwTemp);

        if (dwTemp < 0x100)
            return;
        if ( _MONITOR_CheckMuteStatus( ))
        {
            if (_bIOMuteMode == FALSE)
            {
                _bMuteCount++;

                //printf("Count = %lx\n", _bMuteCount);

                if (_bMuteCount == MUTE_THRESHOLD)
                {
                    HAL_IOMute(TRUE);
                    _bIOMuteMode = TRUE;
                    _bMuteCount = 0;
                }
            }
        }
        else
        {
            if (_bIOMuteMode == TRUE)
            {
                HAL_IOMute(FALSE);
                _bIOMuteMode = FALSE;
                _bMuteCount = 0;
            }
        }
    }
    else
    {
        if (_bIOMuteMode == TRUE)
        {
            HAL_IOMute(FALSE);
            _bIOMuteMode = FALSE;
            _bMuteCount = 0;
        }
    }
}

BYTE    _MONITOR_CheckMuteStatus(void)
{
    int ignor_bits,left,right;
     
    ignor_bits=3+16;  //total 32 bits for serial port output
     
    left=((int)REG_AIU_TXLMIX)>>ignor_bits;
    right=((int)REG_AIU_TXRMIX)>>ignor_bits;
     
    if(left<0)
            left=~left; 
    if(right<0)
            right=~right;
     
    if( left | right )
        return FALSE;
    else
        return TRUE;
}
#endif

#ifdef STACK_OVERFLOW_DETECTION
WORD    _MONITOR_StackStatus(VOID)
{
    WORD wRet = MONITOR_STACK_ERROR_NONE;
    if (__cCTKDVDStack[0] != STACK_MAGIC_NUMBER)
    {
        wRet |= MONITOR_STACK_ERROR_CTKDVD;
    }
        
    if (__cDecStack[0] != STACK_MAGIC_NUMBER)
    {
        wRet |= MONITOR_STACK_ERROR_DECODER;
    }

    if (__cPARSERStack[0] != STACK_MAGIC_NUMBER)
    {
        wRet |= MONITOR_STACK_ERROR_PARSER;
    }

    if (__cInfoFilterStack[0] != STACK_MAGIC_NUMBER)
    {
        wRet |= MONITOR_STACK_ERROR_INFO;
    }
#ifdef  SUPPORT_ATAPI_SOURCE
    if (__cATAPIStack[0] != STACK_MAGIC_NUMBER)
    {
        wRet |= MONITOR_STACK_ERROR_ATAPI;
    }
#endif

#ifdef  SUPPORT_USB_SOURCE
    if (__cUSBSRCStack[0] != STACK_MAGIC_NUMBER)
    {
        wRet |= MONITOR_STACK_ERROR_USBSRC;
    }
#endif        

#ifdef SUPPORT_SERVO_SOURCE 
    {
        extern char stack[2][4096];
        if (stack[0][0] != STACK_MAGIC_NUMBER)
        {
            wRet |= MONITOR_STACK_ERROR_SERVO;
        }
        if (stack[1][0] != STACK_MAGIC_NUMBER)
        {
            wRet |= MONITOR_STACK_ERROR_SRV_BM;
        }
    }
#endif


    if (wRet != MONITOR_STACK_ERROR_NONE)
    {
        DBG_Printf(DBG_THREAD_MONITOR, DBG_INFO_PRINTF,"Stack Overflow: %hx", wRet);
    }

    return wRet;

}
#endif
//////////////////////////////////////////////////////////////////////
// Protected function begin ...
//////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------

