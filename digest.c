
#define     VIEWTIME                    225L    // The default starting point from Track

// The file "winav.h" must be included in first line
// LLY.045-1, porting for new define depending on 22QF055.DOC
// all original include file will be put in "winav.h"
#include  "winav.h"
#include "ctkav.h"
// LLY.045-1 end ...

#include "parser.h" 

#include "digest.h"
#include "initial.h"
//LJY1.20, support 4M flash, remove digest
#ifndef NO_DIGEST

#include "cc.h"

#include "infofilter.h"
#include "utl.h"
#include "hal.h"
#include "SrcFilter.h"

#include "input.h"
#include "chips.h"
#include "osd.h"
#include "linear.h"
#include "osddg.h" 
#include "disp.h" 
#include "gdi.h"
#include "jpegdec.h"

DIGESTUI __DigestUIAttr;

DWORD   __dwIFrames; //the I Frame decteced number
// wyc.100 dvd digest
BYTE    __bDVDDigest;

DWORD   _dwDigestTime;    // the sector time to show in one track
extern  DWORD   __dwGoToTime;
extern  BYTE    __bFrames;
extern  BIT     __btDIGEST;  // means Digest Mode
extern  DWORD   __dwTimeDiscEnd; // The end of the disc time

BYTE     _bDigestFirstPlay;   // record the repeat time in one track

BIT      __btDigestEnableAudio;    // control Audio O/P when ] ()
                                                // enter track mode


// indicate the DIGEST mode will start from a CD-DA track
BIT    __btCDDADigest;

extern DWORD    __dwTimeTorelence;

WORD _aDigestFrmPosX[3];
WORD _aDigestFrmPosY[2];


#ifdef NO_PIP
#define FRAME_THICKNESS         2

JPU_OPZONE _Digest_Source;
JPU_OPZONE _Digest_Destination;

URECT       _DIGEST_Rect;
void _DIGEST_ShowFrame(BOOL bShow);
void _DIGEST_EnterDisplayMode(PBYTE pbKeyNo);

extern void _InitRect(PURECT pRect, WORD wLeft, WORD wTop, WORD wRight, WORD wBottom);

#else   // ifdef NO_PIP

VOID _DigestFrameCopy(DWORD dwX, DWORD dwY)
{
    DWORD   dwHStep;
    DWORD   dwVStep;
    DWORD   dwCurrentFrame;
    DWORD   dwSrcAddress;
    DWORD   dwSrcWidth, dwSrcHeight;
    DWORD   dwDigestFrameSize;

    HAL_ReadInfo(HAL_INFO_HSIZE, &dwSrcWidth);
    HAL_ReadInfo(HAL_INFO_VSIZE, &dwSrcHeight);

    if(MainVideoInfo.wVWidth == 480)
    {
        dwDigestFrameSize = DIGEST_FRAME_SIZE_V_480;
    }
    else
    {
        dwDigestFrameSize = DIGEST_FRAME_SIZE_V_576;
    }

    OS_LockMutex(&__mutexGDIIdle); //LJY0.75, prevent confliting with GDI when uses JPU.

    REG_JPU_CTRL        = (((dwY & 0x0F)!=FALSE)<< 14) | (0 << 12) | (1 << 4) | (0 << 3);
    REG_JPU_HEIWID_SRC  = (dwSrcWidth << 12) | dwSrcHeight;
    REG_JPU_HEIWID_DST  = (DIGEST_FRAME_SIZE_H << 12) | dwDigestFrameSize;
    dwHStep = ((dwSrcWidth-1)*0x2000)/(DIGEST_FRAME_SIZE_H-1);
    dwVStep = ((dwSrcHeight-1)*0x2000)/(dwDigestFrameSize-1);
    REG_JPU_HVSC_FACTOR = (dwVStep << 16) | dwHStep;
    REG_JPU_STRIPE_RW   = (((REG_DISP_PIP_STRIP>>24)*16) << 16) | ((REG_DISP_MAIN1_CR&0xFF)*32);
    
    // FORWARD_REFERENCE_FRAME or BACKWARD_REFERENCE_FRAME
    HAL_ReadInfo(HAL_INFO_PIPFRAME, &dwCurrentFrame);

    if (dwCurrentFrame == 1)
    {
        REG_JPU_ADDR_R_ST   = FORWARD_REFERENCE_FRAME*8;
    }
    else
    {
        REG_JPU_ADDR_R_ST   = BACKWARD_REFERENCE_FRAME*8;
    }

    dwSrcAddress = DS_FRAMEBUF_ST + (((dwY&0xFFF0)>>4)*((REG_DISP_MAIN1_CR&0xFF)*32))*8 + (dwX<<4);
    REG_JPU_ADDR_W_ST   = ((dwY % 16)<< 27) | (dwSrcAddress&0x07FFFFFF);
    REG_JPU_CTRL |= JPU_GO;
    while (REG_JPU_CTRL & 1)
    {
        OS_YieldThread();
    }

    REG_JPU_CTRL        = (((dwY & 0x1F)!=FALSE)<< 14) | (0 << 12) | (1 << 4) | (1 << 3);
    REG_JPU_ADDR_R_ST += Y_MAXSIZE_CB00*8;
    dwSrcAddress = DS_FRAMEBUF_ST + Y_MAXSIZE_CB00*8 + (((dwY&0xFFE0)>>5)*((REG_DISP_MAIN2_CR&0xFF)*32))*8 + (dwX<<4);
    REG_JPU_ADDR_W_ST = (((dwY % 32)/2)<< 27) | (dwSrcAddress&0x07FFFFFF);
    REG_JPU_CTRL |= JPU_GO;
    while (REG_JPU_CTRL & 1)
    {
        OS_YieldThread();
    }

    OS_UnlockMutex(&__mutexGDIIdle); //LJY0.75, prevent confliting with GDI when uses JPU.   
}
#endif

//  *********************************************************************
//  Function    :   _DigestPlayInit
//  Description :   Enter the DIGEST mode and reorcd the Page NO.
//  Arguments   :   None
//  Return      :
//  Side Effect :   when it's called, it will always enter digest mode
//  *********************************************************************
void  _DigestPlayInit (void)
{
#ifndef NO_PIP
    WIN_COLOR PIPColor;    
#endif

    HAL_PlayCommand(COMMAND_STOP, 0);
    // wyc2.53-909P,
    SrcFilter_Stop(__dwSFStreamID);

    HAL_ControlParser(HAL_PARSER_STOP, 0);
    HAL_Reset(HAL_RESET_AUDIO);

//Alan1.05 for nosound bug   HAL_ControlParser(HAL_PARSER_AUDIO_ID, HAL_THROW_AUDIO);

    UTL_ModeInitial ( MODE_LINEAR );
    //LJY0.68, disable frame 0 video output.
    // DISP_Display(0, DISP_MAINVIDEO);
    DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
    DISP_ForceDisplay(0, DISP_MAINVIDEO);

    HAL_ClearScreen(HAL_COLOR_DIGEST, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
    DISP_MainVideoSet(&MainVideoInfo, MainVideoInfo.wHWidth, MainVideoInfo.wVWidth, 0);

#ifdef NO_PIP
    _Digest_Source.wPosCoordH = 0;
    _Digest_Source.wPosCoordV = 0;
    _Digest_Destination.wWidth = DIGEST_FRAME_SIZE_H;
    if(MainVideoInfo.wVWidth == 480)
    {
        _Digest_Destination.wHeight = DIGEST_FRAME_SIZE_V_480;
    }
    else
    {
        _Digest_Destination.wHeight = DIGEST_FRAME_SIZE_V_576;
    }

    GDI_ChangePALEntry(DIGEST_ENTRY_COLOR_BRIGHT_FRAME, DIGEST_VALUE_COLOR_BRIGHT_FRAME, TRUE);
    GDI_ChangePALEntry(DIGEST_ENTRY_COLOR_DARK_FRAME, DIGEST_VALUE_COLOR_DARK_FRAME, TRUE);

    HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, 0);
#else
    //set PIP border
    PIPColor.Color.bWeight = 0x3f;
    PIPColor.Color.bY = 0x97;
    PIPColor.Color.bU = 0x28;
    PIPColor.Color.bV = 0xCB;
    DISP_BGColorSet(PIPColor, DISP_PIP_BORDER);
    DISP_PIPBorderSet(5, 5, TRUE);
    __DISPFrameInfo[0].bSrcFrame = 1;;
    __DISPFrameInfo[0].bDispFrame = 1;
    PIPVideoInfo.wHWidth = DIGEST_FRAME_SIZE_H;
    if(MainVideoInfo.wVWidth == 480)
    {
        PIPVideoInfo.wVWidth = DIGEST_FRAME_SIZE_V_480;
    }
    else
    {
        PIPVideoInfo.wVWidth = DIGEST_FRAME_SIZE_V_576;
    }
    PIPVideoInfo.bEnable = TRUE;
#endif // #ifdef NO_PIP

    __bModePlay= MODE_PLAYUNKNOW;

#ifndef NO_CDDADIGEST
    if (__wDiscType & BOOK_CDDA)
        __btCDDADigest=TRUE;
#endif

    UTL_SetPlayMode ( MODE_PLAYMOTION );
    // Restore normal audio
    /* --CoCo2.33?
    if (__btMute)
    {
        OSD_OUTPUT_MACRO ( MSG_MUTE, FALSE, 1 );
    }
    */

    __btMute= FALSE;  // digest default not mute
// f/w 0.45, cancel 3D in DIGEST mode
    if (__bModeChannel==AUDIO_3D)
    {
        __bModeChannel=AUDIO_STEREO;
        OSD_OUTPUT_MACRO(MSG_AUDIO, __bModeChannel, 1);
    }

//Alan1.05 for nosound bug  CHIPS_OpenAudio( FALSE );

    // First time setting
    _bDigestFirstPlay = TRUE;
    
    // must put after DigestInitial
//    HAL_AdjustTVMode(DISP_NEXTVSYNC);

    __btPlaying= FALSE;
    __btPause= FALSE;
    __bModeCmd= KEY_STOP;

    _DigestFrmPosInit();

    //LJY0.68, enable frame 0 video output.
    DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);

    PARSER_ActionFilter( 0, PARSER_ENABLE_SP1, FALSE );
    CHIPS_OpenAudio( TRUE );
}

    
void DIGEST_Trigger(void)
{   
    if(__DigestUIAttr.bDigestMode <= DIGEST_MODE_DIALOG)
        return;
    _DIGEST_DisplayFrame();
}

void   _DIGEST_DisplayFrame (void)
{
    static DWORD dwDigestPlayTime;
    DWORD dwTmp1, dwTmp2;
#ifdef NO_PIP
    DWORD dwDecFrame;
    DWORD dwSrcAddress = 0;
    DWORD dwDesAddress;
#endif
    
    if(_bDigestFirstPlay)
    {
        if(__DigestUIAttr.bDigestMode == DIGEST_MODE_TRACK)
        {
            if (__DigestUIAttr.bCurTrackNo >= (__wTrackEnd - __wTrackBegin + 1))
                __DigestUIAttr.bCurFrameNo = 0;        
            if(__DigestUIAttr.bCurFrameNo == 0)
            {
                __DigestUIAttr.bCurTrackNo = (__DigestUIAttr.bCurPageNo)*(DIGEST_FRAME_COUNT); 
            } 
            __wPlayItem = __DigestUIAttr.bCurTrackNo + __wTrackBegin;
        }
        else
        {
            if((_dwDigestTime + DIGEST_DISCVIEW_TIMEINTERVAL) < __dwTimeDiscEnd)
            {
                _dwDigestTime = __dwTimeDiscBegin + (__DigestUIAttr.bCurPageNo*(DIGEST_FRAME_COUNT) 
                                + __DigestUIAttr.bCurFrameNo)*(DIGEST_DISCVIEW_TIMEINTERVAL);                
            }
            else
            {
                __DigestUIAttr.bCurFrameNo = 0;
                __DigestUIAttr.bCurTrackNo = 0;
            }
        }

        OSDDG_Update(OSDDG_MSG_UPDATE_UI_INFO);

        UTL_GetPlayRange ( __wPlayItem );

        UTL_SetPlayItemMode ( __wPlayItem );

        if(__DigestUIAttr.bDigestMode == DIGEST_MODE_DISCINTERVAL)
        {
            _dwDigestTime = __dwTimeDiscBegin + (__DigestUIAttr.bCurPageNo*(DIGEST_FRAME_COUNT) 
                            + __DigestUIAttr.bCurFrameNo)*(DIGEST_DISCVIEW_TIMEINTERVAL);
        }
        else if(__DigestUIAttr.bDigestMode == DIGEST_MODE_TRACK)
        {
            _dwDigestTime = __dwTimeBegin; // for track view
        }
        
        __dwTimeEnd = _dwDigestTime + 750;

        HAL_ReadInfo(HAL_INFO_HSIZE, &dwTmp1);
        HAL_ReadInfo(HAL_INFO_VSIZE, &dwTmp2);

#ifdef NO_PIP

        _DIGEST_ShowFrame(FALSE);

        _Digest_Source.wWidth = dwTmp1;
        _Digest_Source.wHeight = dwTmp2;
        _Digest_Source.wFrameStripe = _Digest_Source.wWidth;

/*
        _Digest_Destination.wPosCoordH = 0;
        _Digest_Destination.wPosCoordV = 0;
*/
        _Digest_Destination.wPosCoordH = _aDigestFrmPosX[(__DigestUIAttr.bCurFrameNo%DIGEST_FRAME_COUNT_PER_ROW)];
        _Digest_Destination.wPosCoordV = _aDigestFrmPosY[(__DigestUIAttr.bCurFrameNo/DIGEST_FRAME_COUNT_PER_ROW)];

/*
        _Digest_Destination.wWidth = DIGEST_FRAME_SIZE_H;
        _Digest_Destination.wHeight = DIGEST_FRAME_SIZE_V_576;
*/
        _Digest_Destination.wFrameStripe = 720;

/*
        _Digest_Destination.wWidth = dwTmp1;
        _Digest_Destination.wHeight = dwTmp2;
        _Digest_Destination.wFrameStripe = 720;
*/
        _DIGEST_ShowFrame(TRUE);

#else
        PIPVideoInfo.sHStart = _aDigestFrmPosX[(__DigestUIAttr.bCurFrameNo%DIGEST_FRAME_COUNT_PER_ROW)];
        PIPVideoInfo.sVStart = _aDigestFrmPosY[(__DigestUIAttr.bCurFrameNo/DIGEST_FRAME_COUNT_PER_ROW)];
        DISP_PIPSet(&PIPVideoInfo, dwTmp1, dwTmp2, DISP_NEXTVSYNC);
#endif // #ifdef NO_PIP

        HAL_PlayCommand(COMMAND_DIGEST, 0);

        // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
        UTL_PlayFromTime ( _dwDigestTime, __dwTimeEnd );

        _bDigestFirstPlay = FALSE;
        dwDigestPlayTime = OS_GetSysTimer();  
    }
    else
    {
        if((OS_GetSysTimer() - dwDigestPlayTime) > COUNT_5_SEC)
        {
            //HAL_PlayCommand(COMMAND_STOP, 0);
            SrcFilter_Stop(__dwSFStreamID);
            HAL_ControlParser(HAL_PARSER_STOP, 0);
            HAL_Reset(HAL_RESET_AUDIO);
//Alan1.05 for nosound bug            HAL_ControlParser(HAL_PARSER_AUDIO_ID, HAL_THROW_AUDIO);

            // Scale down and Copy the PIP data to Frame 0.
#ifndef NO_PIP
            _DigestFrameCopy(PIPVideoInfo.sHStart, PIPVideoInfo.sVStart);
#endif // #ifdef NO_PIP
            
            _bDigestFirstPlay = TRUE;

            if(__DigestUIAttr.bDisplayedFrameNo< DIGEST_FRAME_COUNT)
            {
                __DigestUIAttr.bDisplayedFrameNo++;
            }

            __DigestUIAttr.bCurFrameNo = (++__DigestUIAttr.bCurFrameNo) % (DIGEST_FRAME_COUNT);
            __DigestUIAttr.bCurTrackNo++;

#ifdef NO_PIP
#else
            DISP_DisplayCtrl(DISP_PIP, FALSE);
#endif // #ifdef NO_PIP

            HAL_Reset(HAL_RESET_VIDEO);
        }

#ifdef NO_PIP
        HAL_ReadInfo(HAL_INFO_DECCMD_ACK, &__dwTemp);
        if (MODE_SCAN_DONE != __dwTemp)
            return;
        if (FALSE == HAL_ReadInfo(HAL_INFO_DECFRAME, &dwDecFrame))
            return;

        MACRO_RESET_JPU();

        // Scale Y
        if (1 == dwDecFrame)
        {
            dwSrcAddress = DS_FRAMEBUF1_ADDR_ST;
        }
        else if (2 == dwDecFrame)
        {
            dwSrcAddress = DS_FRAMEBUF2_ADDR_ST;
        }

        dwDesAddress = DS_FRAMEBUF0_ADDR_ST +
            _JPUUTIL_GetDispFrameOffset_SCFR(((PJPU_OPZONE)&_Digest_Destination), BLOCK_Y, 0);

        _JPUUTIL_Scale(dwSrcAddress, dwDesAddress, 
            ((PJPU_OPZONE)&_Digest_Source), ((PJPU_OPZONE)&_Digest_Destination), BLOCK_Y);


        // Scale U
        dwSrcAddress += 720 * 576;
        dwDesAddress = DS_FRAMEBUF0_ADDR_ST + 720 * 576 +
            _JPUUTIL_GetDispFrameOffset_SCFR(((PJPU_OPZONE)&_Digest_Destination), BLOCK_UV, 0);

        _JPUUTIL_Scale(dwSrcAddress, dwDesAddress, 
            ((PJPU_OPZONE)&_Digest_Source), ((PJPU_OPZONE)&_Digest_Destination), BLOCK_UV);

        // Scale V
        dwSrcAddress += 128;
        dwDesAddress += 128;

        _JPUUTIL_Scale(dwSrcAddress, dwDesAddress, 
            ((PJPU_OPZONE)&_Digest_Source), ((PJPU_OPZONE)&_Digest_Destination), BLOCK_UV);

        HAL_PlayCommand(COMMAND_DIGEST, 0);
#endif

        // LJY0.76, let it resume from current playback time when digest->NPF.
        // LLY2.05, call API to get current decoded time 
        __dwResumeTime = UTL_GetDecodedTime();
    }
}


//  *********************************************************************
//  Function    :   DIGEST_Digest2Linear
//  Description :   Exit the DIGEST mode, and enter linear playing mode
//  Return      :   NONE
//  Side Effect :
//  *********************************************************************
void DIGEST_Digest2Linear(void)
{   
    CC_KeyCommand(KEY_STOP);

    if(__DigestUIAttr.bDigestMode == DIGEST_MODE_DISCINTERVAL)        
    {
        __dwGoToTime = __dwTimeDiscBegin + 
                        (__DigestUIAttr.bCurPageNo*(DIGEST_FRAME_COUNT) + __DigestUIAttr.bCurFrameNo)*(DIGEST_FRAME_PLAYTIME);
        __dwTemp = __dwGoToTime;
        // Get track number for this entry point
        CDINFO_ParseInfo ( VCD_ENTRIES_TRACK, & __dwTemp, NULL_0 );
        __wPlayItem = __dwTemp;
    }
    else
    {                
        __dwTemp = __wPlayItem;  // play track
        if ( __dwTemp > __wTrackEnd )
            __dwTemp = __wTrackEnd;
    }

    __bTemp= UTL_CheckPlayItemMode ( (WORD)__dwTemp );
    // the entry point is a CD-DA track
    //LJY279, marked to show logo while exit digest to play CDDA track     
    __bModePlay=MODE_PLAYUNKNOW;
    __btDIGEST=FALSE;

}

//  *********************************************************************
//  Function    :   DIGEST_Number
//  Description :   Parser the input number and keep the current time in
//                  that position. Then enter linear play with specified
//                  time
//  Arguments   :   None
//
//  Return      :   TRUE, valid key
//              :   FASE, invalid key
//  Change      :
//  Side Effect :
//  *********************************************************************
BYTE DIGEST_Number(DWORD dwNumber)
{
    extern  DWORD   __dwGoToTime;       // Time that use GOTO position
    // show the input number

//LJY2.81, set as FASLE or __btPlayFromBegin won't be set to be TRUE in LINEAR_Action() by pressing number key.
//        It needs to call UTL_ForceChangeTrack() for DISCVIEW -> NPF or the NPF will be abnormal???
    __btPlaying = FALSE; //TRUE; wyc.170a, make follow codes DVD digest only and change the limit from 9 to __bGrid*__bGrid

#ifdef NO_PIP
#else
    DISP_DisplayCtrl(DISP_PIP, FALSE); //LJY0.68, disable PIP.
#endif // #ifdef NO_PIP

    if (__bKey != KEY_PLAY)
    {   
        if(__DigestUIAttr.bDigestMode == DIGEST_MODE_DISCINTERVAL)
        {
            if ( dwNumber > DIGEST_FRAME_COUNT )
            {
                return FALSE;
            }

            __dwGoToTime = __dwTimeDiscBegin + 
                    (((DWORD)(dwNumber-1)*(DIGEST_FRAME_COUNT) + __DigestUIAttr.bCurFrameNo)*(DIGEST_FRAME_PLAYTIME));
        
            DIGEST_Digest2Linear();
            
            __bFWKey = KEY_GOTOTIME_PLAY;
        }
        else
        {     
            // for VCD, the __wPlayItem=TrackNO+1;
            if (__wDiscType & BOOK_VCD)
            {
                DIGEST_Digest2Linear();
            }
            else
            {
                DIGEST_Digest2Linear();
            }
            
            LINEAR_ChangeTrack ( KEY_NUMBER, (WORD)dwNumber );
        } //if ( __bStyle == DIGEST_STYLE_DISCVIEW )
    } 
    return TRUE;
}

//  ***************************************************************
//  Function    :   DIGEST_CheckHang
//  Description :   Called by MONITOR module to check the hang
//                  phenomenon and recover to the normal state
//  Arguments   :   none
//  Return      :   none
//  Side Effect :
//  ***************************************************************
void DIGEST_CheckHang()
{
}

//LJY1.10, reduce extern global reference for INITIAL module.
void DIGEST_VarInit(void)
{
    BYTE bTmp;
    __btDIGEST = DIGEST_MODE_NULL;

    __DigestUIAttr.bButtonIndex = 0;
    __DigestUIAttr.bCurPageNo = 0;
    __DigestUIAttr.bCurFrameNo = 0;
    __DigestUIAttr.bCurTrackNo = 0;
    __DigestUIAttr.bDisplayedFrameNo = 0;
    //LJY0.80, fix digest last track won't be shown issue.
    bTmp = (BYTE)(__wTrackEnd - __wTrackBegin + 1);
    __DigestUIAttr.bTotalPageNo = bTmp/(DIGEST_FRAME_COUNT);
    if(bTmp % DIGEST_FRAME_COUNT)
    {
        __DigestUIAttr.bTotalPageNo++;
    }
}
 

BYTE DIGEST_ProcessKey(void)
{
    static BYTE bKeyNo;

    if(__DigestUIAttr.bDigestMode == DIGEST_MODE_NULL)
    {
//Alan102, #157. Temporarily prevent to enter the digest mode for CDDA.
        if ((__bKey != KEY_DIGEST) || (__wDiscType & CDROM_M1) ||
            (__wDiscType & BOOK_DVD) || (BOOK_CDDA == __wDiscType))
            return KEY_BYPASS;//bKey; //LJY0.80, for Micky's new key prcessing rule.
    }
    if((__bKey == KEY_POWER) || (__bKey == KEY_OPEN_CLOSE))
    {
        if(__DigestUIAttr.bDigestMode == DIGEST_MODE_DIALOG)
        {
            //OSDDG_Update(OSDDG_MSG_DLG_EXIT);    
            OSDDG_Update(OSDDG_MSG_EXIT); //CoCo0.80a, use this message to exit Digest mode.
            __DigestUIAttr.bDigestMode = DIGEST_MODE_NULL;			
            __btDIGEST = DIGEST_MODE_NULL;
        }
        else
        {
#ifdef NO_PIP
#else
            DISP_DisplayCtrl(DISP_PIP, FALSE);
#endif // #ifdef NO_PIP
            DIGEST_DigestExit();            
        }        
        return KEY_BYPASS;//bKey; //LJY0.80, for Micky's new key prcessing rule.
    }
    
    if(__bKey == KEY_DIGEST)
    {
        if(__btPBC)
        {
            __bKey = KEY_INVALID;
        }
        else if(__DigestUIAttr.bDigestMode > DIGEST_MODE_DIALOG)
        {
            __bKey = KEY_PLAY;  
        }
    }
    else if(__bKey == KEY_PLAY)
    {
        if(__DigestUIAttr.bDigestMode == DIGEST_MODE_DIALOG)
        {
            __bKey = KEY_ENTER;            
        }
        else if(__DigestUIAttr.bDigestMode > DIGEST_MODE_DIALOG)
        {            
            __bKey = KEY_PLAY;
            /*Alan2.17
            if(__DigestUIAttr.bButtonIndex == 0)
                __bFWKey = KEY_DIGEST;
            */
        }
    }
    else
    {
        if ( ((__bKey >= NONNPF_PLAYABLEKEY_BEGIN ) && ( __bKey <= NONNPF_PLAYABLEKEY_END)))
            __bKey = KEY_PLAY;        
    }
    
    switch(__bKey)
    {
        case    KEY_DIGEST:
            if(__DigestUIAttr.bDigestMode == DIGEST_MODE_DIALOG)
            {
                OSDDG_Update(OSDDG_MSG_EXIT); //CoCo0.80a, use this message to exit Digest mode.
                __DigestUIAttr.bDigestMode = DIGEST_MODE_NULL;			
                __btDIGEST = DIGEST_MODE_NULL;
                break;
            }
            else
            {
                _DIGEST_EnterDisplayMode(&bKeyNo);
            }
            break;

        case KEY_UP:
        case KEY_DOWN:            
        case KEY_LEFT:
        case KEY_RIGHT:
            OSDDG_Update(OSDDG_MSG_CLEAR_HIGHLIGHT);
            if((__bKey == KEY_LEFT) || (__bKey == KEY_UP))
            {
                __DigestUIAttr.bButtonIndex = (__DigestUIAttr.bButtonIndex + bKeyNo - 1) % (bKeyNo);                        
            }
            else
            {
                __DigestUIAttr.bButtonIndex = (++__DigestUIAttr.bButtonIndex ) % (bKeyNo);            
            }
            OSDDG_Update(OSDDG_MSG_UPDATE_HIGHLIGHT);               
            break;
        case KEY_ENTER:
            if (DIGEST_MODE_DIALOG == __DigestUIAttr.bDigestMode)
            {
                CC_DoKeyAction(KEY_STOP);
                bKeyNo = DIGEST_MENU2_BUTTON_NO;            
                OSDDG_Update(OSDDG_MSG_DLG_EXIT);     
                __DigestUIAttr.bDigestMode =  __DigestUIAttr.bButtonIndex + DIGEST_MODE_TRACK;
                __btDIGEST = __DigestUIAttr.bDigestMode;
                OSDDG_Update(OSDDG_MSG_INITIAL);                                   
                OSDDG_Update(OSDDG_MSG_UPDATE_UI_INFO);
                OSDDG_Update(OSDDG_MSG_UPDATE_HIGHLIGHT);                      
                _DigestPlayInit ();    
            }
            else if (DIGEST_MODE_TRACK == __DigestUIAttr.bDigestMode || 
                     DIGEST_MODE_DISCINTERVAL == __DigestUIAttr.bDigestMode)
            {
#ifdef SUPPORT_NUMBER_CONFIRM_KEY
                if (0 < __dwNumberPrevious)
                {
                    if (__dwNumberPrevious < __wTrackEnd)
                    {
                        DIGEST_DigestExit();
                        return KEY_BYPASS;
                    }
                    else
                    {
                        return KEY_INVALID;
                    }
                }
                else
#endif
                if ((DIGEST_MENU2_BUTTON_NO - 1 == __DigestUIAttr.bButtonIndex) && 
                    (DIGEST_MODE_DIALOG < __DigestUIAttr.bDigestMode ))
                {
                    DIGEST_DigestExit();
                    __bKey = KEY_PLAY; //LJY0.80, for Micky's new key prcessing rule.
                } 
                else if ((0 == __DigestUIAttr.bButtonIndex) && 
                         (DIGEST_MODE_DIALOG < __DigestUIAttr.bDigestMode ))
                {
                    _DIGEST_EnterDisplayMode(&bKeyNo);
                }
            }
            break;
        case    KEY_PLAY:
            if(__DigestUIAttr.bDigestMode > DIGEST_MODE_DIALOG)
            {
#ifdef NO_PIP
#else
                DISP_DisplayCtrl(DISP_PIP, FALSE);
#endif // #ifdef NO_PIP
#ifdef SUPPORT_NUMBER_CONFIRM_KEY
                if (0 < __dwNumberPrevious)
                {
                    if (__dwNumberPrevious < __wTrackEnd)
                    {
                        DIGEST_DigestExit();
                        return KEY_BYPASS;
                    }
                    else
                    {
                        return KEY_INVALID;
                    }
                }
#endif
            }
            DIGEST_DigestExit();
            __bKey = KEY_PLAY; //LJY0.80, for Micky's new key prcessing rule.
            break;
        case    KEY_PREV:
        case    KEY_NEXT:
            if ( __bKey == KEY_NEXT ) // next digest page
            {
            //LJY277a, to show page mode cyclic                        
                __DigestUIAttr.bCurPageNo= (++__DigestUIAttr.bCurPageNo)%(__DigestUIAttr.bTotalPageNo);
            }
            else    // prev digest page
            {
                if ( __DigestUIAttr.bCurPageNo )
                {
                    __DigestUIAttr.bCurPageNo--;
                }
                else
                {
                    //LJY277a, to show page mode cyclic
                    __DigestUIAttr.bCurPageNo = __DigestUIAttr.bTotalPageNo - 1;
                }                    
            }
            __DigestUIAttr.bCurFrameNo = 0;
            __DigestUIAttr.bDisplayedFrameNo = 0;
                
            _bDigestFirstPlay = TRUE;
            
            HAL_PlayCommand(COMMAND_STOP, 0);
            HAL_Reset(HAL_RESET_VIDEO);
            SrcFilter_Stop(__dwSFStreamID);
            HAL_ControlParser(HAL_PARSER_STOP, 0);
            HAL_Reset(HAL_RESET_AUDIO);
#ifdef NO_PIP
#else
            DISP_DisplayCtrl(DISP_PIP, FALSE); //LJY0.68, disable PIP.
#endif // #ifdef NO_PIP
            HAL_ClearScreen(HAL_COLOR_DIGEST, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
            OSDDG_Update(OSDDG_MSG_CHANGE_PAGE); //LJY0.80, update page info.
            OSDDG_Update(OSDDG_MSG_UPDATE_UI_INFO);
            break;

        case    KEY_N0:
        case    KEY_N1:
        case    KEY_N2:
        case    KEY_N3:
        case    KEY_N4:
        case    KEY_N5:
        case    KEY_N6:
        case    KEY_N7:
        case    KEY_N8:
        case    KEY_N9:
        case    KEY_N10:
        case    KEY_N11:
        case    KEY_N12:
        case    KEY_N13:
        case    KEY_N14:
        case    KEY_N15:
        case    KEY_N16:
        case    KEY_N17:
        case    KEY_N18:
        case    KEY_N19:
        case    KEY_N20:
        case    KEY_N10PLUS:
        case    KEY_N20PLUS:
#ifndef SUPPORT_NUMBER_CONFIRM_KEY
            DIGEST_DigestExit();
#endif
            return KEY_BYPASS;
            break;

        case    KEY_NUMBER:
/*Alan1.02a, bug #287
            //LJY0.80, add number key range protection.
            if(__dwNumberPrevious < __wTrackEnd)
            {
                DIGEST_DigestExit();
                DIGEST_Number(__dwNumberPrevious);
            }
            else
            {
                __bKey = INVALID_KEY;                
            }
*/
            DIGEST_DigestExit();
            return KEY_BYPASS;
            break;            

        default:
            __bKey = INVALID_KEY;//KEY_INVALID;
            break;
    }
//LJY0.80, for Micky's new key prcessing rule.
    if(__bKey == KEY_PLAY)
        return KEY_TODO;
    else if(__bKey == INVALID_KEY)
        return INVALID_KEY;
    else
        return KEY_NO_KEY;
}

void _DIGEST_EnterDisplayMode(PBYTE pbKeyNo)
{
                DIGEST_VarInit();           
                __DigestUIAttr.bDigestMode = DIGEST_MODE_DIALOG;	
                __btDIGEST = DIGEST_MODE_DIALOG;
                *pbKeyNo = DIGEST_MENU1_BUTTON_NO;
                __DigestUIAttr.bButtonEnable[0]=1; //enable track-digest button
                if(__wTrackEnd <= 3)
                	__DigestUIAttr.bButtonEnable[1]=1; //enable discinterval button
                else
                {
                    __DigestUIAttr.bButtonEnable[1]=0; //disable discinterval button
                    (*pbKeyNo)--;
                }
                __DigestUIAttr.bButtonIndex = 0; //high-lighten track-digest button
                OSDDG_Update(OSDDG_MSG_DLG_INITIAL);
                OSDDG_Update(OSDDG_MSG_UPDATE_HIGHLIGHT);
}

void _DIGEST_ShowFrame(BOOL bShow)
{
    DWORD dwRGBColor;
    if (bShow)
    {
        dwRGBColor = 0xEEEE00;
    }
    else
    {
        dwRGBColor = 0x000000;
    }

    _InitRect(&(_DIGEST_Rect), _Digest_Destination.wPosCoordH - FRAME_THICKNESS,
              _Digest_Destination.wPosCoordV - FRAME_THICKNESS - 1,
              _Digest_Destination.wPosCoordH + _Digest_Destination.wWidth + FRAME_THICKNESS, 
              _Digest_Destination.wPosCoordV + _Digest_Destination.wHeight + FRAME_THICKNESS);
    GDI_FBDrawRect(0, &_DIGEST_Rect, FRAME_THICKNESS, dwRGBColor);
}

void _DigestFrmPosInit(void)
{
    BYTE i;
       
    for(i=0;i<3;i++)
    {
        _aDigestFrmPosX[i] = DIGEST_FIRST_FRAME_POS_H+ i*(DIGEST_FRAME_SIZE_H+DIGEST_FRAME_SPACING_H);
    }
    
    for(i=0;i<2;i++)
    {
        if(MainVideoInfo.wVWidth == 480)
        {
            _aDigestFrmPosY[i] = DIGEST_FIRST_FRAME_POS_V_480+ i*(DIGEST_FRAME_SIZE_V_480+DIGEST_FRAME_SPACING_V_480);
        }
        else
        {
            _aDigestFrmPosY[i] = DIGEST_FIRST_FRAME_POS_V_576+ i*(DIGEST_FRAME_SIZE_V_576+DIGEST_FRAME_SPACING_V_576);
        }
    }
}

void DIGEST_DigestExit(void)
{
    HAL_PlayCommand(COMMAND_STOP, 0);
    SrcFilter_Stop(__dwSFStreamID);
    HAL_Reset(HAL_RESET_VIDEO);
    HAL_Reset(HAL_RESET_AUDIO);
#ifdef NO_PIP
    DISP_ForceDisplay(NULL_FRAME, DISP_MAINVIDEO);
#else
    DISP_DisplayCtrl(DISP_PIP, FALSE); //LJY0.68, disable PIP.
#endif // #ifdef NO_PIP
    HAL_ClearScreen(HAL_COLOR_DIGEST, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
    OSDDG_Update(OSDDG_MSG_EXIT);                       
    __DigestUIAttr.bDigestMode = DIGEST_MODE_NULL;			
    __btDIGEST = DIGEST_MODE_NULL;                

    //Alan1.05 for nosound bug
    if ((__bSP_Disp_OFF == 0) || (__bSP_Disp_OFF&0x80))
    {
        DISP_DisplayCtrl(DISP_SP1, TRUE);
        PARSER_ActionFilter( 0, PARSER_ENABLE_SP1, TRUE );
    }

    //CoCo2.33, Digest module will diable "mute" automatically. Therefore, we need to clear MSG_MUTE to prevent MSG_MUTE still exists in OSD stack.
    OSD_OUTPUT_MACRO ( MSG_MUTE, FALSE, 0);
}

#endif //NO_DIGEST

