// ************    Big Modification *************** //
// LLY2.20, porting Rock's code to remove the procedure
// that F/W will set audio buffer/ audio buffer2 start and length in AM

#include "winav.h"
#include "ctkav.h"
#include "ctkav_mcu.h"
#include "comdec.h"
#include "hal.h" //because of VERSION_S define
#include "hdecoder.h"
#include "disp.h"    // because of variable
#include "utl.h" // because of ATTR_AVI define, TYPE_CDROM_AUDIO
#include "parser.h"  // because of PARSER_ES_AUDIO define
#include "subpict.h"
#include "romld.h"
extern  BYTE    __bAttrPlay;    //because of HAL_ResetAudioDecoder will reference it, need modification
extern  BYTE            __bAudioType;//because of HAL_ResetAudioDecoder will reference it, need modification
extern  BYTE                __bACHN;
extern DWORD __dwAspect_Ratio_Info;

// LLY2.51, porting Chuan's code since there are WORD unit for chip verion (main + sub)
//BYTE    __bChipVersion;               // record CODE VERSION
WORD    __wChipVersion;               // record CODE VERSION
BYTE    __bPartNo;  //Kevin1.05-3, make sure to see definition in chips.h

// LLY2.75, expand as DWORD unit since we will support more feature
// Kevin1.11a, support features based on __wChipVersion(IC) and __bPartNo(Package)
//BYTE    __bSupportFeature;
DWORD   __dwSupportFeature;


DWORD   _dwHALUserDefinedColor;
DWORD   __dwClearBuffMode;
BYTE    _bHALABuf2Def=FALSE;
DWORD   __dwHALAudioFreq=44100;
DWORD   __dwHALABUFFSIZE;//, __dwHALVBUFFSIZE;

// LLY2.55, add a global variable to keep current using PROC2
BYTE    __bCurProc2Code=HAL_AUDIO_NONE;


// LLY2.35, if define it, it will use audio buffer 0 and 1 for MPEG audio stream
// Otherwise, it just use one audio buffer 0
//#define USE_2BUFF_FOR_MPEG_AUDIO

// LLY2.51, porting Chuan's code to use only one audio buffer from CT909P IC
// LLY2.56, CT909G Audio buffer configuration is same as CT909P
//#ifdef  CT909P_IC_SYSTEM
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
#define USE_1BUFF_ONLY_FOR_AUDIO
#endif  // #if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)



// LLY2.31, if define it, it will set A/V buffer base on parser reported ratio
#define SET_AVI_BITS_BUFFER_BY_RATIO

#ifdef  SET_AVI_BITS_BUFFER_BY_RATIO
// Define total size for pre-reserved A/V buffer
#define AVI_AV_BUF_LEN      (DS_VDBUF_END_AVI-DS_AD0BUF_ST_AVI)

// Must reserve A0 and A1 buffer at least 4K DW space.
#define MIN_LEN_FOR_A0BUF       0x4000L
// LLY2.37, give Audio Buffer 1 min size from 4K to 5K DW
// Since some WMA files one packet size is 0x22f4 bytes
// And, DSP will start decoding after send at least 2 packet WMA data to A1 buffer
#define MIN_LEN_FOR_A1BUF       0x5000L // If use audio buffer 1

// If define it, it will print out AVI A/V buffer setting issue base on parser reported ratio
//#define DEBUG_AVI_BUFFER_RATIO

// If define it, A/V buffer calculation only base on A/V buffer ratio
// Otherwise, it also reference the minimnu parser suggested audio buffer size
//#define PARTITION_JUST_ON_AV_RATIO

#endif  // #ifdef SET_AVI_BITS_BUFFER_BY_RATIO

//********************************************************************
// Description :   Detect Chip Version and Part No; Decide supported audio features
// Arguments   :   None
// Return      :   None
//********************************************************************
// LLY2.00, modify the procedure to meet CT909S requirement.
void    HAL_DetectChipVersion(void)
{
/***** Detect Chip Version *****/
    // using macro to get chip version, and it will handle 909 & 909S
    // LLY2.51, porting Chuan's code since there are WORD unit for chip verion (main + sub)
    //__bChipVersion = MACRO_PLAT_GET_CHIPVER;
    __wChipVersion = MACRO_PLAT_GET_CHIPVER;

    // Add error protection message
#ifdef  CT909R_IC_SYSTEM
    if(__wChipVersion != VERSION_BB)
#endif  // #ifdef CT909R_IC_SYSTEM
#ifdef  CT909P_IC_SYSTEM
    if(__wChipVersion != VERSION_P_AB)
#endif  // #ifdef CT909P_IC_SYSTEM
    // LLY2.56, add for CT909G IC chip version
#ifdef  CT909G_IC_SYSTEM
    if(__wChipVersion != VERSION_G_AA)
#endif  // #ifdef CT909G_IC_SYSTEM
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Chip version wrong: %lx\n", __wChipVersion);
    }

/***** Detect Part No *****/
    // using macro to get part no, and it will handle 909 & 909S
    __bPartNo = MACRO_PLAT_GET_PARTNO;

/***** Decide Supported Featurs *****/
    // LLY2.18, re-modify the procedure to detect the supported feature

    // [1] Always enable WMA
    //__bSupportFeature = 0;  // Initial as none first
    __dwSupportFeature = SUPPORT_FEATURE_WMA;

    // [2] Check if support DivX
    if(MACRO_IC_CAPABILITY & IC_CAP_DIVX)
    {
        __dwSupportFeature |= SUPPORT_FEATURE_DIVX;
    }

    // [3] Check if support special feature, ex Game
    if(MACRO_IC_CAPABILITY & IC_CAP_SPECIAL)
    {
        __dwSupportFeature |= SUPPORT_FEATURE_SPECIAL;
    }

    // LLY2.75, add IC capability checking for CT909G IC
#ifdef  CT909G_IC_SYSTEM
    // [4] Check if not support MPEG4
    if( (MACRO_IC_CAPABILITY & IC_CAP_MPEG4) != IC_CAP_MPEG4 )
    {
        __dwSupportFeature |= SUPPORT_FEATURE_NOMPEG4;
    }

    // [5] Check if not support 5.1 channel
    if( (MACRO_IC_CAPABILITY & IC_CAP_51CH) != IC_CAP_51CH )
    {
        __dwSupportFeature |= SUPPORT_FEATURE_NO51CH;
    }

    // [6] Check if support all feature
    if(MACRO_IC_CAPABILITY == IC_CAP_ALL)
    {
        __dwSupportFeature |= SUPPORT_FEATURE_SPECIAL;
        __dwSupportFeature &= ~SUPPORT_FEATURE_NOMPEG4;
        __dwSupportFeature &= ~SUPPORT_FEATURE_NO51CH;
    }

#else   // #ifdef CT909G_IC_SYSTEM
    // [4] Check if support DVB
    if(MACRO_IC_CAPABILITY & IC_CAP_DVB)
    {
        __dwSupportFeature |= SUPPORT_FEATURE_DVB;
    }

    // [5] Check if support all feature
    if(MACRO_IC_CAPABILITY == IC_CAP_ALL)
    {
        __dwSupportFeature |= (SUPPORT_FEATURE_DIVX | SUPPORT_FEATURE_DVB | SUPPORT_FEATURE_SPECIAL);
    }
    // LLY2.75, enable support multi-media feature for CT95x serial IC
    if(MACRO_IC_CAPABILITY & IC_CAP_MULTIMEDIA)
    {
        __dwSupportFeature |= SUPPORT_FEATURE_MULTIMEDIA;
    }

    // LLY2.75, enable support digital panel feature for CT956A IC
    if(MACRO_IC_CAPABILITY & IC_CAP_DIGITAL_PANEL)
    {
        __dwSupportFeature |= SUPPORT_FEATURE_DIGITAL_PANEL;
    }

    // LLY2.75, enable support NAND Flash feature for CT956A IC
    if(MACRO_IC_CAPABILITY & IC_CAP_NAND_FLASH)
    {
        __dwSupportFeature |= SUPPORT_FEATURE_NAND_FLASH;
    }
#endif  // #ifdef CT909G_IC_SYSTEM


/******* Remove the un-necessary part if user disable it *******/
    // Disable WMA if user disable it
#ifndef SUPPORT_WMA
    __dwSupportFeature &= ~(SUPPORT_FEATURE_WMA);
#endif  // #ifndef SUPPORT_WMA

    // Disable ProLogic if user disable it
#ifndef SUPPORT_PROLOGIC
    __dwSupportFeature &= ~(SUPPORT_FEATURE_PROLOGIC);
#endif  // #ifndef SUPPORT_PROLOGIC

    // Disable ProLogic if two channel only
#ifdef  SUPPORT_2_CHANNEL_ONLY
    __dwSupportFeature &= ~(SUPPORT_FEATURE_PROLOGIC);
#endif  // #ifdef SUPPORT_2_CHANNEL_ONLY

    // Disable DivX if user disable it
#ifndef SUPPORT_DIVX
    __dwSupportFeature &= ~(SUPPORT_FEATURE_DIVX);
#endif  // #ifndef SUPPORT_DIVX

#ifdef DOLBY_CERTIFICATION //kevin0.80
    __dwSupportFeature &= ~SUPPORT_FEATURE_PROLOGIC;
#endif  // #ifdef DOLBY_CERTIFICATION

    // Support HDCD
#ifdef SUPPORT_HDCD //customer may want to disable HDCD
    //__dwSupportFeature |= SUPPORT_FEATURE_HDCD;  //not support HDCD for all part No.
#endif

    // Disable DVB feature if user disable it
#ifndef SUPPORT_STB
    __dwSupportFeature &= ~(SUPPORT_FEATURE_DVB);
#endif  // #ifndef SUPPORT_STB

    // LLY2.38, force to enable DivX 4/5 even the IC don't support DivX
    // LLY2.51, porting CSC modification, ie. current F/W can playback DivX 4/5 even chip not support
    // And, parser and decoder will control it.
#if 0 //def  FORCE_ENABLE_DIVX_4_5
    __dwSupportFeature |= SUPPORT_FEATURE_DIVX;
#endif  // #ifdef  FORCE_ENABLE_DIVX_4_5

    // LLY2.50, always disable DivX if CT95x system
#ifdef  CT951_PLATFORM
    __dwSupportFeature &= ~(SUPPORT_FEATURE_DIVX);
#endif  // #ifdef CT951_PLATFORM

    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "__wChipVersion=%hx, __bPartNo=%hx, __dwSupportFeature=%hx\n", __wChipVersion, __bPartNo, __dwSupportFeature);

#ifdef  CT909G_IC_SYSTEM
    // Do error protection if using 2 channel IC & configuration as 5.1 channel
    if(__dwSupportFeature & SUPPORT_FEATURE_NO51CH)
    {
#ifndef SUPPORT_2_CHANNEL_ONLY
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: config 5.1 channel w/ not support 5.1 ch 909G IC\n");
#endif  // #ifndef SUPPORT_2_CHANNEL_ONLY
    }
#endif  // #ifdef CT909G_IC_SYSTEM

}

void HAL_ResetVideoDecoder(BYTE bDecoders)
{
    DWORD   dwSaveInt;
    OS_DISABLE_INTERRUPTS( dwSaveInt );	// Disable INT to avoid different threads accessing
    REG_PLAT_RESET_CONTROL_ENABLE = (PLAT_RESET_VDEC_ENABLE | PLAT_RESET_VDEC2_ENABLE);
    OS_RESTORE_INTERRUPTS( dwSaveInt );	// Restore INT.
}

// LLY2.51, give desired video buffer mode since we may in different mode.
//  ************************************************************************************
//  Function    :   HAL_FillVideoBuffer
//  Description :   Fill desired data to desired video buffer directly
//  Arguments   :   bMode, specify the desired video buffer mode
//                  pdwPtr, the pointer to desired data array
//                  dwSizw, the desired data count (in DWORD unit)
//  Return      :   Nonce
//  ************************************************************************************
void    HAL_FillVideoBuffer(BYTE bMode, PDWORD  pdwPtr, DWORD dwSize)
{
    DWORD   dwCnt;
    DWORD   dwAddrStart, dwAddrEnd;

    if(bMode == HAL_VIDEOBUF_MM)
    {
        dwAddrStart = DS_VDBUF_ST_MM;
        dwAddrEnd = DS_VDBUF_END_MM;
    }
    else if(bMode == HAL_VIDEOBUF_NORMAL)
    {
        dwAddrStart = DS_VDBUF_ST;
        dwAddrEnd = DS_VDBUF_END;
    }
    else
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: not specify desired video buffer mode\n");
        return;
    }

    // Fill logo data into video buffer by DWORD unit
    for(dwCnt=0; dwCnt<dwSize; dwCnt++)
    {
        (*((volatile DWORD *) (dwAddrStart) + dwCnt))= pdwPtr[dwCnt];
    }

    // LLY0.65, fill the dummp data since some data will be kept within FIFO
    // VDFIFO: 64bit*16 + DChain FIFO: 16bit*6 + VRem: 64bit*6 = 24 * 64 bit
    for(dwCnt=dwSize; dwCnt<dwSize+24*2; dwCnt++)
    {
        (*((volatile DWORD *) (dwAddrStart) + dwCnt))= 0x0L;
    }

      REG_MCU_VREM = MCU_REMAINDER_INIT | (dwSize + 48);

    // Set Video Buffer
    // LLY0.68, let the video buffer is same as normal mode
    MACRO_MCU_SET_VLD_READ_BUFFER(dwAddrStart, dwAddrEnd, dwAddrStart);
}


// LLY2.22, collect the procedure of clear screen procedure by JPG
//  *****************************************************************************
//  Function    :   _ClearScreen
//  Description :   Base on user given parameter to clear screen by JPG
//  Arguments   :
//  Return      :   None
//  Notice      :   Only used for HAL_ClearScreen()
//  *****************************************************************************
void    _ClearScreen(DWORD dwFrame_Y_Addr, DWORD dwFrame_UV_Addr,  BOOL blFBRMode, WORD wFrameWidth, DWORD dwColor)
{
// set value for the REG_JPU_CTL
#define TRANS_JPU_CTL(type, op, uv)             (((type) << 12) | ((op)) | ((uv) << 3))

// calculate value for JPU's heiwid_src and  heiwid_dst registers
#define TRANS_HEIWID_REG(width,height)      (((width) << 16) | (height))

// calculate value for JPU's stripe register
#define GET_STRIPE(src)                     (((src) + 15) >> 4)
#define GET_STRIPE_UV(src)                  (((src) + 7) >> 3)
#define TRANS_STRIPE_REG(src,dst)           ((GET_STRIPE(src) << 16) | (GET_STRIPE(dst)))
#define TRANS_STRIPE_REG_UV(src,dst)        ((GET_STRIPE_UV(src) << 16) | (GET_STRIPE_UV(dst)))


    DWORD   dwFrame_VSize;
    extern  MUTEX_T     __mutexGDIIdle;
    WORD    wSrcWidth; // Source picture width

    // LLY2.30, collect fill Y part procedure outside since it's same for FBR or non-FBR mode
    // Chuan1.06, Get the Vertical size of current display picture.
    dwFrame_VSize = ((dwFrame_UV_Addr - dwFrame_Y_Addr)/wFrameWidth)&0xFFFFFFF8;

    OS_LockMutex(&__mutexGDIIdle);

	// reset vpu
    REG_PLAT_RESET_CONTROL_ENABLE = PLAT_RESET_VPU_ENABLE;
    REG_PLAT_RESET_CONTROL_DISABLE = PLAT_RESET_VPU_DISABLE;

    // initialize JPU
    REG_JPU_CTRL = 0;
    REG_JPU_STRIPE_RW = 0;
    REG_JPU_HEIWID_SRC = 0;
    REG_JPU_HVSC_FACTOR = 0;
    REG_JPU_ADDR_R_ST = 0;
    REG_JPU_ADDR_W_ST = 0;

	// fill Y
    REG_JPU_CTRL       = TRANS_JPU_CTL(0, JPU_FC_Y_OP, 0);      //JPEG: FC_Y
    REG_JPU_FILL_DATA  = dwColor;
    REG_JPU_STRIPE_RW  = TRANS_STRIPE_REG(0, wFrameWidth);              //JPEG: TRANS_STRIPE_REG(0, 720);

    REG_JPU_HEIWID_DST = TRANS_HEIWID_REG(wFrameWidth, dwFrame_VSize);  //JPEG: TRANS_HEIWID_REG(720, 480);
    REG_JPU_ADDR_W_ST  = dwFrame_Y_Addr;
    REG_JPU_CTRL |= JPU_GO;

    while (REG_JPU_CTRL & 1)
    {
        // do nothing, just polling
        OS_YieldThread();
    }


    if (blFBRMode == TRUE)
    {
        // Unlock mutex earily since UV part filling is designed by F/W code (not JPU)
        OS_UnlockMutex(&__mutexGDIIdle);

        // Start fill UV part
        dwFrame_VSize /= 32;
        // Get source picture width
        // LLY2.31, always set source width as 720
        wSrcWidth = 720; // (WORD) REG_DISP_MAIN1_CR;
        wSrcWidth = wSrcWidth/16;

        while(dwFrame_VSize--)
        {
            BYTE bx, by;

            for(bx = 0; bx < wSrcWidth; bx++)
            {
                for(by = 0; by < 64; by+=4)
                {
                    (*(volatile DWORD *)(dwFrame_UV_Addr + by)) = 0x82108082;
                    (*(volatile DWORD *)(dwFrame_UV_Addr + by + 0x40)) = 0x08208210;
                    (*(volatile DWORD *)(dwFrame_UV_Addr + by + 0x80)) = 0x80820820;
                }
                dwFrame_UV_Addr += 192;
            }

            // LLY2.31, it's un-necessary to %4, since source width is always set as 720
            //if (wSrcWidth % 4)
            {
                dwFrame_UV_Addr += 64;
            }
        }
    }
    else
    {
        // fill U-part
        REG_JPU_CTRL       = TRANS_JPU_CTL(0, JPU_FC_U_OP, 1);
        REG_JPU_ADDR_W_ST  = dwFrame_UV_Addr;
        REG_JPU_CTRL |= JPU_GO;
        while (REG_JPU_CTRL & 1)
        {
            // do nothing, just polling
            OS_YieldThread();
        }

        // fill V-part
        REG_JPU_CTRL       = TRANS_JPU_CTL(0, JPU_FC_V_OP, 1);
        REG_JPU_ADDR_W_ST  = dwFrame_UV_Addr + 128;
        REG_JPU_CTRL |= JPU_GO;
        while (REG_JPU_CTRL & 1)
        {
            // do nothing, just polling
            OS_YieldThread();
        }

        // Notice: can unlock mutex after UV part filling is done (since is was done by JPU)
        OS_UnlockMutex(&__mutexGDIIdle);

    }

}


//************************************************************************************
// Description  :   Issue "JPU Preset Frame Buffer Command" to fill the display
//                  frame buffer with desired color
// Reference    :   JPU's JPEG_PresetFrameBuf()
// Argument     :   bColor: the desired color mode
//                  bMode: HAL_CLEAR_SCREEN_MPEG or HAL_CLEAR_SCREEN_JPEG
//                  dwFrameIdx: frame index for MPEG mode
// Return       :   None
//************************************************************************************
void    HAL_ClearScreen(BYTE bColor, BYTE bMode, DWORD dwFrameIdx) //test OK
{
    DWORD   dwColor=0;
    DWORD   dwY_Addr, dwUV_Addr;
    DWORD   dwFBRMode;

    extern WORD  __wFrameWidth;


    // wyc0.85a, set __dwClearBuffMode by the bMode for checking if need to clear frame buffer in HAL_RESET_VIDEO
    __dwClearBuffMode = bMode;
    switch(bColor)
    {
    case HAL_COLOR_USER_DEFINE: //CoCo1.05, Add this for CDG.
        dwColor = _dwHALUserDefinedColor;
        break;
    case    HAL_COLOR_BLUE:
        dwColor = 0x0023d460;
        break;
    case    HAL_COLOR_BLACK:
        dwColor = 0x00008080;
        break;
    case    HAL_COLOR_THUMB:
        dwColor = THUMBNAIL_BACKGROUND_COLOR;
        break;
    case    HAL_COLOR_BORDER:
    default:
#ifdef  BORDER_BLUE
        dwColor = 0x0023d460;
#else
        dwColor = 0x00008080;
#endif
        break;
    }

    if (dwFrameIdx == HAL_DISP_CURRENTFRAME)
    {
        HAL_ReadInfo(HAL_INFO_DISPFRAME_INDEX, &dwFrameIdx);
    }

    // Get the structure pointer of display frame related information.
    // Notice: must give the frame index at 2nd parameter first
    dwY_Addr = dwUV_Addr = dwFrameIdx;
    HAL_ReadInfo(HAL_INFO_DISPFRAME_YADDR, &dwY_Addr);
    HAL_ReadInfo(HAL_INFO_DISPFRAME_UVADDR, &dwUV_Addr);

    // Get FBR mode or not
    HAL_ReadInfo(HAL_INFO_FBR_MODE, &dwFBRMode);

    //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
    //    "ClearScreen, Idx: %lx, Y: %lx, UV: %lx, FBR: %lx\n", dwFrameIdx, dwY_Addr, dwUV_Addr, dwFBRMode);

    // LLY2.22, collect clear frame procedure into another API
    _ClearScreen(dwY_Addr, dwUV_Addr, (BOOL)dwFBRMode, __wFrameWidth, dwColor);
}


// LLY2.00, collect the procedure of waiting R/W AM command ACK
//  **************************************************************************
//  Function    :   _Wait_AccessAM_CMD_ACK
//  Description :   Wait access AM command ACK
//  Arguments   :   None
//  Return      :   TRUE, ACK ok
//                  FALSE, ACK fail
//  Notice      :   Only used for AM command ACK
//  **************************************************************************
BYTE    _Wait_AccessAM_CMD_ACK(void)
{
    DWORD   dwHALTemp;
    DWORD   dwAck=1;

    //Wait ACK
    dwHALTemp = OS_GetSysTimer();
    while( (OS_GetSysTimer() - dwHALTemp) < COUNT_200_MSEC )
    {
        dwAck = REG_PLAT_PARAMETER1>>30;
        if (! dwAck )
        {
            break;
        }

        //release CPU resource to other threads
        OS_YieldThread();
    }
    if (dwAck)
    {
        // LLY1.21a, read-back PROC2 PC by macro, don't access AIU directly
        DWORD   dwPC;
        MACRO_PLAT_GET_PROC2_PC(dwPC);

        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nWait no Proc2 ACK, PC: %lx!", dwPC);
        return FALSE;
    }
    return TRUE;

}


// J500CSC_2.05, for supporting runtime decompressed audio code
DWORD   HAL_LoadAudioCode( BYTE bAudioType, PDWORD pdwAudioTbl, PBYTE pbDstAddr, PBYTE dwWorkMem )
{
    DWORD   dwRet = 0;         // set return value as 0 (error)

#ifdef RUNTIME_UNZIP_AUDIO_CODE
    PSECTION_ENTRY pAudioTbl;

    switch (bAudioType)
    {
        // wyc2.15-909S, update audio decoder re-load mechansim.
        case HAL_AUDIO_PCM:
        case HAL_AUDIO_LPCM:
            pAudioTbl = ROMLD_GetSectionByName(0, "MPPC");
            break;
        case HAL_AUDIO_MPG:
        case HAL_AUDIO_MP3:
        case HAL_AUDIO_MP2:
            pAudioTbl = ROMLD_GetSectionByName(0, "MPPC");
            break;
        case HAL_AUDIO_AC3:
            pAudioTbl = ROMLD_GetSectionByName(0, "ACDT");
            break;
        case HAL_AUDIO_DTS:
            pAudioTbl = ROMLD_GetSectionByName(0, "ACDT");
            break;
        case HAL_AUDIO_WMA:
            pAudioTbl = ROMLD_GetSectionByName(0, "WMA ");
            break;
        case HAL_AUDIO_AAC:
            pAudioTbl = ROMLD_GetSectionByName(0, "AAC ");
            break;
// LLY2.38a, porting Chuan's code to load JPEG decoder @ PROC2
#ifdef SUPPORT_JPEGDEC_ON_PROC2
        case HAL_VIDEO_JPG:
            pAudioTbl = ROMLD_GetSectionByName(0, "JPEG");
            break;
#endif // #ifdef SUPPORT_JPEGDEC_ON_PROC2
        default:
            pAudioTbl = 0;
            break;
    }

    if (pAudioTbl == 0) // 0 means audio decoder does not exist
    {
        return dwRet;
    }

    if ((dwRet = ROMLD_LoadSectionTo(pAudioTbl, (DWORD)pbDstAddr, (DWORD)dwWorkMem)) == 0)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Load Audio Code Fail!!");
    }
    /*
    PBYTE   pbSrc;

    if( bAudioType > HAL_AUDIO_END )
    {
        return dwRet;
    }

    pbSrc = (PBYTE) (pdwAudioTbl[((DWORD) bAudioType) - 1]);
    if( (DWORD) pbSrc == 0 )        // 0 means audio decoder does not exist
    {
        return dwRet;
    }

    dwRet = UNZIP2006_DECODE( pbSrc, pbDstAddr, dwWorkMem );
    */

#endif  // RUNTIME_UNZIP_AUDIO_CODE

    return dwRet;
}

//*********************************************************************
// Description :   Reset (reboot) Proc2 and load new audio decoder code
// Arguments   :   bAudioType, specify the new audio type for loading audio code.
// Return      :   NONE
//*********************************************************************
//kevin0.95, create
void HAL_ReloadAudioDecoder(BYTE bAudioType)
{
#ifdef SUPPORT_RUNTIME_RESET_PROC2
#define FIFO_CLEAR         (*((volatile DWORD *) (REG_MCU_BASE+0x014)))
#define FIFO_INTEN         (*((volatile DWORD *) (REG_MCU_BASE+0x01c)))    // Interrupt Control Register 2

    DWORD dwAck=0;
    DWORD dwSaveInt;
    DWORD   dwTime;
    DWORD dwREG_PLAT_TIMER1_RELOAD;

#ifdef NO_PROC2
    return ;
#endif // #ifdef NO_PROC2

    // 1: can runtime reset Proc2 0: can't
    HAL_ReadInfo(HAL_INFO_CAN_RUNTIME_RESET_PROC2, &dwAck);
    if (dwAck==FALSE)
        return;

    // Chuan1.10, Force PROC2 enter debug mode on IU watchpoint, and any trap.
    MACRO_PLAT_HALT_PROC2();

    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "reset Proc2\n");

    MACRO_MCU_DISABLE_PROTECTION(MCU_WRITE_PROTECT_AREA1_ENABLE);

// Chuan1.08, Turn on PROM
#ifdef SUPPORT_PROM_DISABLE
    OS_DISABLE_INTERRUPTS( dwSaveInt );
    REG_PLAT_CLK_GENERATOR_CONTROL &= ~(PLAT_MCLK_PROM_DISABLE);
    OS_RESTORE_INTERRUPTS( dwSaveInt );
    // Notice: delay a while to wait PROM enable ok
    OS_DelayTime(COUNT_50_MSEC);
#endif  // #ifdef SUPPORT_PROM_DISABLE

    // LLY2.16, must disable PROC2 interrupt first, and clear the value
    // Since, PROC1 may R/W DM (send interrupt to PROC2) during proc2 disable --> load code
    // So, some un-expected phenomenone will happen.
    // Notice: those procedure can be removed after we remove old DM R/W procedure.
    REG_PLAT_PROC2_INT_MASK_PRIORITY = INT_CLEAR_ALL;
    REG_PLAT_PROC2_INT_CLEAR = INT_SET_ALL;

    //#define SECINTP2A_STAT (*(volatile int *)(0x800001b8))
    //SECINTP2A_STAT = -1;

    // J500KWShih_212, modify "clear audio HW" procedure for > CT909S
#if 0 // only for CT909
    //clear audio HW
	REG_MCU_A1REM=(1<<24)|2;
	FIFO_CLEAR |= (1<<2);
	REG_MCU_PCMREM=(1<<24)|2;
	FIFO_CLEAR |= (1<<3);
	REG_MCU_SPDIFREM=(1<<24)|2;
	FIFO_CLEAR |= ((1<<4)|1);

	REG_AIU_AIUEN &= 0x3;
	FIFO_CLEAR |= 0x7d;
	FIFO_INTEN = 0;

	REG_MCU_A1REM=(1<<24)|0x0;
	REG_MCU_PCMREM=(1<<24)|0x0;
	REG_MCU_SPDIFREM=(1<<24)|0x0;

    REG_MCU_A1REM=(1<<24)|0xffffff;
#endif
	REG_MCU_A1REM = (MCU_REMAINDER_INIT | 0x00);
	REG_MCU_PCMREM = (MCU_REMAINDER_INIT | 0x00);
	REG_MCU_SPDIFREM = (MCU_REMAINDER_INIT | 0x00);

	REG_MCU_MCR5 |= (MCU_RESET_AIU_MIC_WRITE_FIFO | MCU_RESET_AIU_UPK1_FIFO | MCU_RESET_AIU_PCM_READ_FIFO | MCU_RESET_AIU_SPDIF_READ_FIFO);

	REG_AIU_AIUEN &= 0x0303;
	REG_MCU_MCR5 |= 0x7d;
	REG_MCU_MCR7 = 0;


    // J500CSC_111, clear the semaphore for avoiding it is protected by PROC2
    MACRO_PLAT_KEY_UNLOCK( );

    //reset PC
    REG_PLAT_RESET_CONTROL_ENABLE = PLAT_RESET_PROC2_ENABLE; //reset PROC2

    // Chuan1.10, Reset error mode, and exit debug mode.
    MACRO_PLAT_RELEASE_PROC2();

    PROC2_SP = DS_PROC2_SP;
    PROC2_STARTADR = DS_PROC2_STARTADDR;

// J500CSC_2.05, for supporting runtime decompressed audio code
    HAL_LoadAudioCode( bAudioType, (PDWORD) DS_AUDIO_TABLE_ST, (PBYTE) DS_PROC2_STARTADDR,
            (PBYTE) DS_UNZIP_BUF_ST );

    // LLY2.21, Porting Chuan's code to keep time1 value first
    // Since, it will be updated by PROC2
    dwREG_PLAT_TIMER1_RELOAD = REG_PLAT_TIMER1_RELOAD;

    //audio start
    (*(volatile DWORD *)(0x800007e4)) = 0x10003;

    OS_REORDER_BARRIER();
    OS_DISABLE_INTERRUPTS( dwSaveInt );	// Disable INT to avoid different threads accessing

    REG_PLAT_RESET_CONTROL_DISABLE = PLAT_RESET_PROC2_DISABLE; //reset PROC2

    OS_RESTORE_INTERRUPTS( dwSaveInt );	// Restore INT.
    OS_REORDER_BARRIER();

    //wait for Proc2's booting ACK (audio start OK)
    dwTime=OS_GetSysTimer();
    while( (OS_GetSysTimer() - dwTime) < PROC2_RESET_ACK_TIME_OUT) // kevin1.05, COUNT_200_MSEC -> COUNT_250_MSEC
    {
        dwAck = (*(volatile DWORD *)(0x800007e4));
        dwAck >>= 16;

        if(dwAck==0)
        {
            // LLY2.55, keep current PROC2 code after PROC2 booting ok
            __bCurProc2Code=bAudioType;
            break;
        }
    }

    if(dwAck)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Proc 2 booting2 fail\n");
    }

    // LLY2.21, Porting Chuan's code to restore time1 value first
    // Since, it will be updated by PROC2
    REG_PLAT_TIMER1_RELOAD = dwREG_PLAT_TIMER1_RELOAD;

    // Notice: (1) only can protect audio code area while the audio code range is valid
    //         (2) only can read-back 27bit for REG_MCU_WP1BASE
    if ((REG_MCU_WP1BASE >= (DS_PROC2_STARTADDR&0x7FFFFFF)) && (REG_MCU_WP1BASE <= (DS_PROC2_SP&0x7FFFFFF)) &&
        (REG_MCU_WP1SIZE >= (DS_PROC2_STARTADDR&0x7FFFFFF)) && (REG_MCU_WP1SIZE <= (DS_PROC2_SP&0x7FFFFFF)) &&
        (REG_MCU_WP1SIZE > REG_MCU_WP1BASE))
    {
        MACRO_MCU_ENABLE_PROTECTION(MCU_WRITE_PROTECT_AREA1_ENABLE);
    }

// Chuan1.08, Turn off PROM
#ifdef SUPPORT_PROM_DISABLE
    OS_DISABLE_INTERRUPTS( dwSaveInt );
    REG_PLAT_CLK_GENERATOR_CONTROL |= PLAT_MCLK_PROM_DISABLE;
    OS_RESTORE_INTERRUPTS( dwSaveInt );
#endif  // #ifdef SUPPORT_PROM_DISABLE

#endif
}


//********************************************************************
// Description :   Initialize Audio Shared Memory
// Arguments   :   None
// Return      :   None
//********************************************************************
// LLY2.31, base on CT908R-AP Note-005.doc to set different gain value for Dolby & °ê¼Ð certification
#ifdef  DOLBY_AUDIO_STANDARD
#define PARA_GAIN   0x3CL
#else   // #ifdef DOLBY_AUDIO_STANDARD
// LLY2.51, porting Chuan's code for different gain value for CT909P IC
// LLY2.61, porting Chuan's code to let gain value are same for CT909G/R IC
//#ifdef CT909P_IC_SYSTEM
#if defined(CT909R_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
#define PARA_GAIN   0x33L
#else   // #if defined(CT909R_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
#define PARA_GAIN   0x30L
#endif  // #if defined(CT909R_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
#endif  // #ifdef DOLBY_AUDIO_STANDARD
void    HAL_InitAudio(void) //test OK
{
    DWORD dwTempReg;

#ifdef CT909G_IC_SYSTEM
    // LLY2.61, tell audio decoder current CT909G IC type (5.1ch or 2ch)
#ifdef  SUPPORT_2_CHANNEL_ONLY
    HAL_WriteAM(HAL_AM_CT909G_2CH, 1);  // 2 ch only
#else   // #ifdef SUPPORT_2_CHANNEL_ONLY
    HAL_WriteAM(HAL_AM_CT909G_2CH, 0);  // 5.1 ch
#endif  // #ifdef SUPPORT_2_CHANNEL_ONLY
    // Joey2.60: Select all channels as active channel for volume control
    HAL_WriteAM(HAL_AM_PCM_SCALE_ACTIVE_CHANNEL, 0xff);
#endif  // #ifdef CT909G_IC_SYSTEM

	HAL_WriteAM(HAL_AM_ENCODE_SPDIF_RAW, 1);

	//max audio buffer remainer during PAUSE/Video master mode
	HAL_WriteAM(HAL_AM_SKIP_THRESHOLD, 0x1000);

    // Disable down-sample/ un-sample control
	HAL_WriteAM( HAL_AM_DOWNSAMPLE_EN, 0) ;
    HAL_WriteAM( HAL_AM_UPSAMPLE_EN, 0);

    // J500KWShih_220, initial buffer address for language syudy by zero
    HAL_WriteAM(HAL_AM_LSBUF_ADR, 0);

    // initialize A/V sync control for DSP; otherwise, it maybe let video master and A/V hang
    HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, FALSE);

#ifdef SUPPORT_6CH_OUTPUT_FOR_NONDVD
    HAL_WriteAM( HAL_AM_2CH_TO_6CH, 1);
#else   // #ifdef SUPPORT_6CH_OUTPUT_FOR_NONDVD
    HAL_WriteAM( HAL_AM_2CH_TO_6CH, 0);
#endif  // #ifdef SUPPORT_6CH_OUTPUT_FOR_NONDVD


#ifndef NO_MIC_INPUT
    //MIC detect threshold
    HAL_WriteAM(HAL_AM_MIC_DETECT_THRESHOLD, 0x80);


    //MIC config
    REG_AIU_MIC_CONFG |= (AUDIO_ALIGN_FORMAT<<2) | (0xf<<4); // 16 bits

    //kevin1.02, fix IR Power off-> power on => noise when reading disc (MIC_PCM_SCALE != 0)
    //set default MIC volume in CHIPS_Initial(CHIPS_INIT_VOL); enable/disable MIC in CHIPS_OpenAudio()
    /*
    //set default MIC volume
    //CHIPS_MICVolControl(CHIPS_NORMAL);
    {
    extern DWORD   __wMICVol;
    __wMICVol = MIC_VOL_DEFAULT ;
    HAL_WriteAM(HAL_AM_MIC_PCM_SCALE, __wMICVol);
    }
    */
#endif  // #ifndef NO_MIC_INPUT

    //set default AC3 downmix mode
    // Source 1KHz -20dB 5.1CH
    // CT908/MTK     L            R
    // LT/RT        0.57V       5.95V
    // L0/RO        4.66V       4.66V
    //CHIPS_DownMixMode(AC3_DOWNMIX_LORO); //kevin1.00, saved in Setup EEPROM

    //kevin0.76
    HAL_WriteAM(HAL_AM_DRC_MODE, 2); //default: line out

    //kevin0.80
    HAL_WriteAM(HAL_AM_BASS_MANAGE_CONFIG, 0);

    //kevin1.05, move from _ProgramACLK to fix audio channel mapping error


    // Need control [11:10] and [18:19] for internal audio DAC
    // LLY2.04, modify the setting for CT909S: Timer[31]=0, APB write channel[28]=0
    // LLY2.11, enable PCM L/R channel inverse feature while I2S_ALIGN, bit[1]
    // Otherwise, the L/R channel output maybe wrong
    // LLY2.20, set bit[3:0] default value as zero, enable it by yourself if necessary
    // Since, the value is different for I2C_ALIGN
    // LLY2.20, always program bit[19:18] as 1 since internal Audio DAC only accept I2S format
#if (AUDIO_ALIGN_FORMAT == I2S_ALIGN)
    dwTempReg = (BIT_RESOL_VALUE<<7) | (BIT_RESOL_VALUE<<4) | (ALIGN_VALUE<<2) | (1<<18) | (1<<1); //[31] timer
#else
    dwTempReg = (BIT_RESOL_VALUE<<7) | (BIT_RESOL_VALUE<<4) | (ALIGN_VALUE<<2) | (1<<18); //[31] timer
#endif // #if (AUDIO_ALIGN_FORMAT == I2S_ALIGN)

    // LLY2.61, re-modify the procedure for programming REG_AIU_SPORT_CONFG0 bit[16:13]
    // 1. only enable bit[13] (downmix L/R) while using internal audio DAC
    // 2. enable bit[16:13] (5.1 channel + downmix L/R) while internal + external audio DAC.
    // 3. enable bit[13] for 2 channel external audio DAC mode
    // 4. enable bit[16:14] for 5.1 channel external audio DAC mode
    // Notice: CT909P IC only support 2 channel only external audio DAC, so only can enable bit[13]
    //         CT909R IC bit[16:13] must control together since H/W bug even internal audio DAC
    //         or 2 channel only exteranl audio DAC
    //         CT909G IC, bit[16:13] can be control indepentent. And, it's ok to enable all bit
    //         even 2 channel only mode.
#ifdef  CT909P_IC_SYSTEM
    dwTempReg |= (0x1L<<13);
#else   // #ifdef CT909P_IC_SYSTEM
    dwTempReg |= (0xfL<<13);
#endif  // #ifdef CT909P_IC_SYSTEM

    // LLY2.56, do key lock first since AIU_SPORT_CONFG0 will be accessed by PROC2 together
    MACRO_PLAT_KEY_LOCK( );
    REG_AIU_SPORT_CONFG0 = dwTempReg;
    MACRO_PLAT_KEY_UNLOCK( );

// J500KWShih_220, set threshold for PCM FIFO, Ncahns[14:12], 3 for 5.1 + 2ch, 0 for 2ch only
#ifndef SUPPORT_2_CHANNEL_ONLY
    //REG_AIU_REG184_ADDR |= 3 << 12;
    REG_AIU_WRFMT &= (~0x00007000);
    REG_AIU_WRFMT |= 0x00003000;
#endif // SUPPORT_2_CHANNEL_ONLY

// LLY2.17, used for internal ADAC programming.
#ifdef  ENABLE_INTERNAL_ADAC
    // LLY2.04, set bit[3] & [7] as zero to disable "left/right channel power donw mode" for internal audio DAC
    REG_AIU_DAC_CONFG0 &= ~0x88;
    // clear bit[19:8] for GAIN value first
    REG_AIU_DAC_CONFG0 &= (~0x000fff00);
    // Then, set the desired gain value base on pre-defined value.
    REG_AIU_DAC_CONFG0 |= (PARA_GAIN << 8);
#endif  // #ifdef  ENABLE_INTERNAL_ADAC
    HAL_SetAudioDAC( AUDIO_FREQ_48K );

    // LLY2.51, porting Chuan's code to turn on SPDIF for CT909P IC
#ifdef  CT909P_IC_SYSTEM
    // LLY2.55, only turn-on SPDIF output while solution support it.
    // ex. DMP952A don't support SPDIF output, and use the same pin for other feature.
#ifndef NO_SPDIF_OUTPUT
    REG_PLAT_GPE_MUX |= 1;  // Chuan, Turn on SPDIF
#endif  // #ifndef NO_SPDIF_OUTPUT
#endif  // #ifdef CT909P_IC_SYSTEM

}

void    HAL_ResetAIUPCM(void)
{
    REG_AIU_AIUEN |= AIU_DMA_ENABLE_PCM_OUT;
    MACRO_IDLE();
    REG_AIU_AIUEN &= ~(AIU_DMA_ENABLE_PCM_OUT|0x200);
}

void    HAL_InitVideo(void)
{
}


// LLY2.35 create ...
#ifdef  SET_AVI_BITS_BUFFER_BY_RATIO
//  ****************************************************************************************
//  Function    :   _Cal_Safe_ABuf_Size
//  Description :   Only used to calculate a reasonable and safe audio buffer size
//  Arguments   :   dwSize, the parser original suggested buffer size
//                  dwABuf1, the desired audio buffer 1 size
//  Return      :   The final audio buffer size
//  ****************************************************************************************
DWORD   _Cal_Safe_ABuf_Size(DWORD dwSize, DWORD dwABuf1)
{
    extern  BYTE __bAudioType;
    DWORD   dwMinABuf; // A0 + A1

    // Initial minimnu audio buffer size as audio 0 only
    dwMinABuf = MIN_LEN_FOR_A0BUF;

    // Must plus audio buffer 1 size if use two buffer case.
    if( (__bAudioType==HAL_AUDIO_WMA)
#ifdef  USE_2BUFF_FOR_MPEG_AUDIO
        || (__bAudioType==HAL_AUDIO_MPG) || (__bAudioType==HAL_AUDIOBUF_MP3)
#endif  // #ifdef  USE_2BUFF_FOR_MPEG_AUDIO
        )
    {
        dwSize += dwABuf1;

        dwMinABuf += dwABuf1;
    }

    // Do error checking that A0 + A1 limit <= min audio buffer size
    if(dwSize <= dwMinABuf)
    {
        dwSize = dwMinABuf;
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: min abuf must >= A0+A1 limit: %lx\n", dwMinABuf);
    }

    return dwSize;

}


//  ****************************************************************************************
//  Function    :   HAL_Cal_BitsAddr
//  Description :   Calculate A/V bits buffer address
//  Arguments   :   pBitsBuf_Addr, a struct pointer used to keep desired A/V buffer address
//  Return      :   None
//  Notice      :   Only used for AVI bits now.
//  ****************************************************************************************
void    HAL_Cal_BitsAddr(PBITS_BUF_ADDR pBitsBuf_Addr)
{
    BYTE    bVRatio;        // keep video ratio
    DWORD   dwVBufSize;     // keep desired video buffer size
    DWORD   dwMinABufSize;  // keep minimun required audio buffer size
    DWORD   dwABuf1Size;

#ifndef PARTITION_JUST_ON_AV_RATIO
    extern  BYTE __bASTNs;
    extern  BYTE __bSPSTNs;
    extern  BYTE __bASTID;
    extern  BYTE __bAudioType;
#endif  // #ifndef PARTITION_JUST_ON_AV_RATIO

    // Step 0: Give the audio buffer 1 size first
    // If need it, set it as 5K DWORD, Otherwise, set it as 0
    if( (__bAudioType==HAL_AUDIO_WMA)
#ifdef  USE_2BUFF_FOR_MPEG_AUDIO
        || (__bAudioType==HAL_AUDIO_MPG) || (__bAudioType==HAL_AUDIOBUF_MP3)
#endif  // #ifdef  USE_2BUFF_FOR_MPEG_AUDIO
        )
    {
        dwABuf1Size = MIN_LEN_FOR_A1BUF;
    }
    else
    {
        dwABuf1Size = 0x0L;
    }

    // Get Video stream ratio
    bVRatio = PARSER_getAVIVideoRatio();

    // LLY2.37, add error protection while video stream ratio is 0 or 100
    // Then, just config A/V buffer base on pre-defined address
    if( (bVRatio == 0) || (bVRatio == 100) )
    {
        pBitsBuf_Addr->dwA0Start = DS_AD0BUF_ST_AVI;
        pBitsBuf_Addr->dwA0End = DS_AD0BUF_END_AVI;
        pBitsBuf_Addr->dwA1Start = DS_AD1BUF_ST_AVI;
        pBitsBuf_Addr->dwA1End = DS_AD1BUF_END_AVI;
        pBitsBuf_Addr->dwVStart = DS_VDBUF_ST_AVI;
        pBitsBuf_Addr->dwVEnd = DS_VDBUF_END_AVI;
    }
    else
    {
#ifdef  PARTITION_JUST_ON_AV_RATIO

        // Get video buffer size base on ratio
        dwVBufSize = AVI_AV_BUF_LEN * bVRatio / 100;

        // Let video size be 0x200 alignment (512 byte)
        if(dwVBufSize%0x200)
        {
            dwVBufSize = (dwVBufSize/0x200)*0x200;
        }

        // Get minimun audio buffer size
        dwMinABufSize = AVI_AV_BUF_LEN - dwVBufSize;

#else   // #ifdef  PARTITION_JUST_ON_AV_RATIO

        // Step 2: Get minimun requirement audio buffer size
        dwMinABufSize = PARSER_getAVIAudioBufferSize(PARSER_getAVIAudioMaxBitrateTrackIndex());

#ifdef  DEBUG_AVI_BUFFER_RATIO
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Paser give max ABuf: %lx\n", dwMinABufSize);
#endif  // #ifdef  DEBUG_AVI_BUFFER_RATIO

        dwMinABufSize = _Cal_Safe_ABuf_Size(dwMinABufSize, dwABuf1Size);

        // Step 4: Calculate minimum required A/V buffer size for two case
        // [1] Only one audio track: base A:V ratio to partition whole A/V buffer
        // [2] >=2 audio tracks: same as parser suggested minimum size
        //     and same control for >=2 subpicture tracks -- LLY2.36
        if( (__bASTNs >= 2) || (__bSPSTNs >= 2) )
        {
            // Video buffer size = total size - max track one of parser suggested min audio size
            dwVBufSize = AVI_AV_BUF_LEN - dwMinABufSize;

            // Re-calculate audio buffer size base on desired audio track
            dwMinABufSize = PARSER_getAVIAudioBufferSize(__bASTID);

#ifdef  DEBUG_AVI_BUFFER_RATIO
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Paser give current ABuf: %lx\n", dwMinABufSize);
#endif  // #ifdef  DEBUG_AVI_BUFFER_RATIO

            dwMinABufSize = _Cal_Safe_ABuf_Size(dwMinABufSize, dwABuf1Size);
        }
        else
        {
            dwVBufSize = AVI_AV_BUF_LEN * bVRatio / 100;

            // Let video buffer size be 0x200 alignment
            if(dwVBufSize%0x200)
            {
                dwVBufSize = (dwVBufSize/0x200)*0x200;
            }

            // Always set as parser suggest value for minimun audio buffer size
            // while the value calculated base on A/V ratio is less than it
            if( (AVI_AV_BUF_LEN-dwVBufSize) > dwMinABufSize)
            {
                dwMinABufSize = AVI_AV_BUF_LEN - dwVBufSize;
            }
            else
            {
                dwVBufSize = AVI_AV_BUF_LEN - dwMinABufSize;
#ifdef  DEBUG_AVI_BUFFER_RATIO
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
                    "Keep suggest ABuf size: %lx, and re-get VBuf size: %lx\n",
                    dwMinABufSize, dwVBufSize);
#endif  // #ifdef  DEBUG_AVI_BUFFER_RATIO
            }
        }


        // Step 6: do error checking for total A/V buffer size > pre-reserved space.
        // Then, can calculate the desired audio buffer end address.
        if((dwVBufSize + dwMinABufSize) > AVI_AV_BUF_LEN)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Reduce A/V since video end > pre-define\n");
            dwMinABufSize = (AVI_AV_BUF_LEN - (AVI_AV_BUF_LEN * bVRatio/100));

            // Let min audio size be 0x200 alignment (512 byte)
            if(dwMinABufSize%0x200)
            {
                dwMinABufSize = ((dwMinABufSize/0x200)+1)*0x200;
            }

            dwVBufSize = AVI_AV_BUF_LEN - dwMinABufSize;
        }

#endif  // #ifdef  PARTITION_JUST_ON_AV_RATIO

        // Step 7: Calculate final A/V buffer start/end address
        pBitsBuf_Addr->dwA0Start = DS_AD0BUF_ST_AVI;
        pBitsBuf_Addr->dwA0End = DS_AD0BUF_ST_AVI + dwMinABufSize - dwABuf1Size;
        pBitsBuf_Addr->dwA1Start = DS_AD0BUF_ST_AVI + dwMinABufSize - dwABuf1Size;
        pBitsBuf_Addr->dwA1End = DS_AD0BUF_ST_AVI + dwMinABufSize;
        pBitsBuf_Addr->dwVStart = DS_VDBUF_END_AVI - dwVBufSize;
        pBitsBuf_Addr->dwVEnd = DS_VDBUF_END_AVI;
    }


#ifdef  DEBUG_AVI_BUFFER_RATIO
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "VRatio: %lx\n", bVRatio);

    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
        "A0: %lx - %lx; A1: %lx - %lx; V: %lx - %lx\n",
        pBitsBuf_Addr->dwA0Start, pBitsBuf_Addr->dwA0End,
        pBitsBuf_Addr->dwA1Start, pBitsBuf_Addr->dwA1End,
        pBitsBuf_Addr->dwVStart, pBitsBuf_Addr->dwVEnd);
#endif  // #ifdef  DEBUG_AVI_BUFFER_RATIO


}

#endif  // #ifdef  SET_AVI_BITS_BUFFER_BY_RATIO


// LLY2.01, collect audio buffer setting procedure into a API
//  ************************************************************************
//  Function    :   Set audio buffer related thing,
//                  ex. audio buffer0/ 1 address, PCM buffer ...
//  Arguments   :   bMediaAttr, specify the media format
//                         bMode, specify desired buffer mode
//  Return      :   None
//  Side Effect :
//  ************************************************************************
void    HAL_SetAudio_Buffer(BYTE bMediaAttr, BYTE bMode)
{
    DWORD dwSaveInt;

    // Add protection code to check input mode is valid
    if(bMode>HAL_AUDIOBUF_END || bMode<HAL_AUDIOBUF_BEGIN)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: audio buffer mode is invalid: %lx", bMode);
    }

    // LLY2.04, using macro to set AIU uppacker0/1 related address
    // Notice: it's un-necessary to set read address so remove it.

    // LLY2.04, remove setting for AIU uppacker0/1 read channel read address
    // ex. REG_MCU_ACR09/ REG_MCU_ACR11
    // Since, it will be handled by PROC2

    if(bMediaAttr == ATTR_AVI) //AVI/DivX
    {
        // Notice: MPEG/ MP3 audio bytpe will use two different audio buffer.
        //         ie. buffer 1 is handle by PROC1, buffer 2 is handle by PROC2
        //         Others will use same audio buffer, and buffer size = buffer 1 + buffer 2
        //         ie. buffer 1 and buffer 2 are all handle by PROC1

#ifdef  SET_AVI_BITS_BUFFER_BY_RATIO
        BITS_BUF_ADDR   sBitsBufAddr;

        // LLY2.35, call API to calculate desired AVI audio buffer
        HAL_Cal_BitsAddr(&sBitsBufAddr);

        // LLY2.35, only WMA will use two audio buffer
#ifndef USE_1BUFF_ONLY_FOR_AUDIO
        if( (bMode==HAL_AUDIOBUF_WMA)
#ifdef  USE_2BUFF_FOR_MPEG_AUDIO
            || (bMode==HAL_AUDIOBUF_MPG) || (bMode==HAL_AUDIOBUF_MP3)
#endif  // #ifdef  USE_2BUFF_FOR_MPEG_AUDIO
            )
        {
            PARSER_SetOutBufferAddress( PARSER_ES_AUDIO, sBitsBufAddr.dwA0Start, sBitsBufAddr.dwA0End);

            //set audio buffer/ audio buffer2 start and length
            MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(sBitsBufAddr.dwA0Start, sBitsBufAddr.dwA0End, sBitsBufAddr.dwA0Start);
            MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(sBitsBufAddr.dwA1Start, sBitsBufAddr.dwA1End, sBitsBufAddr.dwA1Start);

#ifdef  DEBUG_AVI_BUFFER_RATIO
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
                "A0: %lx - %lx; A1: %lx - %lx\n",
                sBitsBufAddr.dwA0Start, sBitsBufAddr.dwA0End, sBitsBufAddr.dwA1Start, sBitsBufAddr.dwA1End);
#endif  // #ifdef  DEBUG_AVI_BUFFER_RATIO
        }
        else
#endif  // #ifndef USE_1BUFF_ONLY_FOR_AUDIO
        {
            PARSER_SetOutBufferAddress( PARSER_ES_AUDIO, sBitsBufAddr.dwA0Start, sBitsBufAddr.dwA1End );

            //set audio buffer/ audio buffer2 start and length
            MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(sBitsBufAddr.dwA0Start, sBitsBufAddr.dwA1End, sBitsBufAddr.dwA0Start);
            MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(sBitsBufAddr.dwA0Start, sBitsBufAddr.dwA1End, sBitsBufAddr.dwA0Start);

#ifdef  DEBUG_AVI_BUFFER_RATIO
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
                "A0: %lx - %lx; A1: %lx - %lx\n",
                sBitsBufAddr.dwA0Start, sBitsBufAddr.dwA1End, sBitsBufAddr.dwA0Start, sBitsBufAddr.dwA1End);
#endif  // #ifdef  DEBUG_AVI_BUFFER_RATIO
        }


#else   // #ifdef SET_AVI_BITS_BUFFER_BY_RATIO

        // LLY2.35, only WMA will use two audio buffer
#ifndef USE_1BUFF_ONLY_FOR_AUDIO
        if( (bMode==HAL_AUDIOBUF_WMA)
#ifdef  USE_2BUFF_FOR_MPEG_AUDIO
            || (bMode==HAL_AUDIOBUF_MPG) || (bMode==HAL_AUDIOBUF_MP3)
#endif  // #ifdef  USE_2BUFF_FOR_MPEG_AUDIO
            )
        {
            PARSER_SetOutBufferAddress( PARSER_ES_AUDIO, DS_AD0BUF_ST_AVI, DS_AD0BUF_END_AVI );

            //set audio buffer/ audio buffer2 start and length
            MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(DS_AD0BUF_ST_AVI, DS_AD0BUF_END_AVI, DS_AD0BUF_ST_AVI);
            MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(DS_AD1BUF_ST_AVI, DS_AD1BUF_END_AVI, DS_AD1BUF_ST_AVI);
        }
        else
#endif  // #ifndef USE_1BUFF_ONLY_FOR_AUDIO
        {
            PARSER_SetOutBufferAddress( PARSER_ES_AUDIO, DS_AD0BUF_ST_AVI, DS_AD1BUF_END_AVI );

            //set audio buffer/ audio buffer2 start and length
            MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(DS_AD0BUF_ST_AVI, DS_AD1BUF_END_AVI, DS_AD0BUF_ST_AVI);
            MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(DS_AD0BUF_ST_AVI, DS_AD1BUF_END_AVI, DS_AD0BUF_ST_AVI);
        }
#endif  // #ifdef SET_AVI_BITS_BUFFER_BY_RATIO
    }
    // LLY2.21a, let CDDA go normal case.
    else if(bMediaAttr & TYPE_CDROM_AUDIO)// || bMediaAttr==ATTR_CDDA) //.MP3/.MP2/.WMA/CDDA/CD-DTS
    {
        // Notice: MPEG/ MP3 audio bytpe will use two different audio buffer.
        //         ie. buffer 1 is handle by PROC1, buffer 2 is handle by PROC2
        //         Others will use same audio buffer, and buffer size = buffer 1 + buffer 2
        //         ie. buffer 1 and buffer 2 are all handle by PROC1
        // LLY2.35, only WMA will use two audio buffer
#ifndef USE_1BUFF_ONLY_FOR_AUDIO
        if( (bMode==HAL_AUDIOBUF_WMA)
#ifdef  USE_2BUFF_FOR_MPEG_AUDIO
            || (bMode==HAL_AUDIOBUF_MPG) || (bMode==HAL_AUDIOBUF_MP3)
#endif  // #ifdef  USE_2BUFF_FOR_MPEG_AUDIO
            )
        {
            PARSER_SetOutBufferAddress( PARSER_ES_AUDIO, DS_AD0BUF_ST_MM, DS_AD0BUF_END_MM );

            //set audio buffer/ audio buffer2 start and length
            MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(DS_AD0BUF_ST_MM, DS_AD0BUF_END_MM, DS_AD0BUF_ST_MM);
            MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(DS_AD1BUF_ST_MM, DS_AD1BUF_END_MM, DS_AD1BUF_ST_MM);
        }
        else
#endif  // #ifndef USE_1BUFF_ONLY_FOR_AUDIO
        {
            PARSER_SetOutBufferAddress( PARSER_ES_AUDIO, DS_AD0BUF_ST_MM, DS_AD1BUF_END_MM );

            //set audio buffer/ audio buffer2 start and length
            MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(DS_AD0BUF_ST_MM, DS_AD1BUF_END_MM, DS_AD0BUF_ST_MM);
            MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(DS_AD0BUF_ST_MM, DS_AD1BUF_END_MM, DS_AD0BUF_ST_MM);
        }

    }
    // LLY2.01, add CDROM/ DVDROM motion case except AVI
    else if(bMediaAttr & TYPE_CDROM_AV) // LLY2.10, correct it as TYPE_CDROM_AV
    {
        // Notice: MPEG/ MP3 audio bytpe will use two different audio buffer.
        //         ie. buffer 1 is handle by PROC1, buffer 2 is handle by PROC2
        //         Others will use same audio buffer, and buffer size = buffer 1 + buffer 2
        //         ie. buffer 1 and buffer 2 are all handle by PROC1
        // LLY2.35, only WMA will use two audio buffer
#ifndef USE_1BUFF_ONLY_FOR_AUDIO
        if( (bMode==HAL_AUDIOBUF_WMA)
#ifdef  USE_2BUFF_FOR_MPEG_AUDIO
            || (bMode==HAL_AUDIOBUF_MPG) || (bMode==HAL_AUDIOBUF_MP3)
#endif  // #ifdef  USE_2BUFF_FOR_MPEG_AUDIO
            )
        {
            PARSER_SetOutBufferAddress( PARSER_ES_AUDIO, DS_AD0BUF_ST_MM_MOTION, DS_AD0BUF_END_MM_MOTION );

            //set audio buffer/ audio buffer2 start and length
            MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(DS_AD0BUF_ST_MM_MOTION, DS_AD0BUF_END_MM_MOTION, DS_AD0BUF_ST_MM_MOTION);
            MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(DS_AD1BUF_ST_MM_MOTION, DS_AD1BUF_END_MM_MOTION, DS_AD1BUF_ST_MM_MOTION);
        }
        else
#endif  // #ifndef USE_1BUFF_ONLY_FOR_AUDIO
        {
            PARSER_SetOutBufferAddress( PARSER_ES_AUDIO, DS_AD0BUF_ST_MM_MOTION, DS_AD1BUF_END_MM_MOTION );

            //set audio buffer/ audio buffer2 start and length
            MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(DS_AD0BUF_ST_MM_MOTION, DS_AD1BUF_END_MM_MOTION, DS_AD0BUF_ST_MM_MOTION);
            MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(DS_AD0BUF_ST_MM_MOTION, DS_AD1BUF_END_MM_MOTION, DS_AD0BUF_ST_MM_MOTION);
        }

    }
    else  //DVD/VCD
    {
        // LLY2.35, only WMA will use two audio buffer
#ifndef USE_1BUFF_ONLY_FOR_AUDIO
        if( (bMode==HAL_AUDIOBUF_WMA)
#ifdef  USE_2BUFF_FOR_MPEG_AUDIO
            || (bMode==HAL_AUDIOBUF_MPG) || (bMode==HAL_AUDIOBUF_MP3)
#endif  // #ifdef  USE_2BUFF_FOR_MPEG_AUDIO
            )
        {
            PARSER_SetOutBufferAddress( PARSER_ES_AUDIO, DS_AD0BUF_ST, DS_AD0BUF_END );

            //set audio buffer/ audio buffer2 start and length
            MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(DS_AD0BUF_ST, DS_AD0BUF_END, DS_AD0BUF_ST);
            MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(DS_AD1BUF_ST, DS_AD1BUF_END, DS_AD1BUF_ST);
        }
        else // AC3 or DTS or LPCM
#endif  // #ifndef USE_1BUFF_ONLY_FOR_AUDIO
        {
            PARSER_SetOutBufferAddress( PARSER_ES_AUDIO, DS_AD0BUF_ST, DS_AD1BUF_END );

            //set audio buffer/ audio buffer2 start and length
            MACRO_MCU_SET_AIU_UNPACKER0_READ_BUFFER(DS_AD0BUF_ST, DS_AD1BUF_END, DS_AD0BUF_ST);
            MACRO_MCU_SET_AIU_UNPACKER1_READ_BUFFER(DS_AD0BUF_ST, DS_AD1BUF_END, DS_AD0BUF_ST);
        }
    }

    //set PCM buffer address
    HAL_WriteAM( HAL_AM_PCMBUF_ADR, DS_PCMBUF_ST);
    HAL_WriteAM( HAL_AM_PCMBUF_LEN, (DS_PCMBUF_END-DS_PCMBUF_ST)>>11);

    //keep the Audio buffer size
    __dwHALABUFFSIZE = (REG_MCU_ACR0A-REG_MCU_ACR0B+DATA_WIDTH)>>2;

    // Chuan0.80a, Set the threshold: 5/6 buffer size. Unit: 64 bits.
    // HAL_WriteAM(HAL_AM_SKIP_THRESHOLD, (__dwHALABUFFSIZE*5/6/2));
    // Chuan2.17, 909R: Unit: 32 bits * 16
    // Notice: must use >= 20060512 audio code
    // J500KWShih_220, to adjest the threshold for parser temporary space (0x1000)
    // LLY2.21, reference parser define directly (unit is BYTE)
    // Notice: it's un-necessary to do 5/6 buffer size since parser has done for it.
    //HAL_WriteAM(HAL_AM_SKIP_THRESHOLD, (__dwHALABUFFSIZE*5/6/16));
    HAL_WriteAM(HAL_AM_SKIP_THRESHOLD, ((__dwHALABUFFSIZE-(PARSER_AUDIO_BUF_GAP/4))/16));

    OS_DISABLE_INTERRUPTS( dwSaveInt );

    MACRO_PLAT_KEY_LOCK( );
    // LLY2.35, only WMA will use two audio buffer
#ifndef USE_1BUFF_ONLY_FOR_AUDIO
    if( (bMode==HAL_AUDIOBUF_WMA)
#ifdef  USE_2BUFF_FOR_MPEG_AUDIO
        || (bMode==HAL_AUDIOBUF_MPG) || (bMode==HAL_AUDIOBUF_MP3)
#endif  // #ifdef  USE_2BUFF_FOR_MPEG_AUDIO
        )
    {
        // Need double check with HHShih ???
        REG_MCU_AUDIO_REMCTL = (REG_MCU_AUDIO_REMCTL & ~0x3000) | 0x2000; // clear bit[13:12]
        _bHALABuf2Def = TRUE;
    }
    else
#endif  // #ifndef USE_1BUFF_ONLY_FOR_AUDIO
    {
        // Non-MPEG mode PROC1 will use two audio buffer (buffer 0 for normal, buffer 1 for SPDIF)
        // So, we must set audio buffer 1 write channel select as BIU mode, ie. disable PROC2 access
        // And, enable the remain control flag.
        REG_MCU_AUDIO_REMCTL &= ~0x3000; // clear bit[13:12]
        REG_MCU_REMEN |= MCU_REMAINDER_ENABLE_AUDIO_BUF1; //hhshih
        _bHALABuf2Def = FALSE;
    }
    MACRO_PLAT_KEY_UNLOCK( );
    OS_RESTORE_INTERRUPTS( dwSaveInt );

}

//  ************************************************************************
//  Function    :   Set video buffer related thing,
//  Arguments   :   bMode, specify desired buffer mode
//  Return      :   None
//  Side Effect :
//  ************************************************************************
void    HAL_SetVideo_Buffer(BYTE bMode)
{
    // Add protection code to check input mode is valid
    if(bMode>HAL_VIDEOBUF_END || bMode<HAL_VIDEOBUF_BEGIN)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: video buffer mode is invalid: %lx", bMode);
    }

    if(bMode == HAL_VIDEOBUF_NORMAL)
    {
        MACRO_MCU_SET_VLD_READ_BUFFER(DS_VDBUF_ST, DS_VDBUF_END, DS_VDBUF_ST);
        PARSER_SetOutBufferAddress( PARSER_ES_VIDEO, DS_VDBUF_ST, DS_VDBUF_END );
    }
    else if(bMode == HAL_VIDEOBUF_AVI)
    {

#ifdef  SET_AVI_BITS_BUFFER_BY_RATIO
        BITS_BUF_ADDR   sBitsBufAddr;

        // LLY2.35, call API to calcuate video buffer size
        HAL_Cal_BitsAddr(&sBitsBufAddr);

        MACRO_MCU_SET_VLD_READ_BUFFER(sBitsBufAddr.dwVStart, sBitsBufAddr.dwVEnd, sBitsBufAddr.dwVStart);
        PARSER_SetOutBufferAddress( PARSER_ES_VIDEO, sBitsBufAddr.dwVStart, sBitsBufAddr.dwVEnd );

#ifdef  DEBUG_AVI_BUFFER_RATIO
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
        "VAddr: %lx - %lx\n", sBitsBufAddr.dwVStart, sBitsBufAddr.dwVEnd);
#endif  // #ifdef DEBUG_AVI_BUFFER_RATIO


#else   // #ifdef SET_AVI_BITS_BUFFER_BY_RATIO
        MACRO_MCU_SET_VLD_READ_BUFFER(DS_VDBUF_ST_AVI, DS_VDBUF_END_AVI, DS_VDBUF_ST_AVI);
        PARSER_SetOutBufferAddress( PARSER_ES_VIDEO, DS_VDBUF_ST_AVI, DS_VDBUF_END_AVI );
#endif  // #ifdef SET_AVI_BITS_BUFFER_BY_RATIO
    }
    else if(bMode == HAL_VIDEOBUF_MM_MOTION)
    {
        MACRO_MCU_SET_VLD_READ_BUFFER(DS_VDBUF_ST_MM_MOTION, DS_VDBUF_END_MM_MOTION, DS_VDBUF_ST_MM_MOTION);
        PARSER_SetOutBufferAddress( PARSER_ES_VIDEO, DS_VDBUF_ST_MM_MOTION, DS_VDBUF_END_MM_MOTION );
    }
    else if(bMode == HAL_VIDEOBUF_MM)
    {
        MACRO_MCU_SET_VLD_READ_BUFFER(DS_VDBUF_ST_MM, DS_VDBUF_END_MM, DS_VDBUF_ST_MM);
        PARSER_SetOutBufferAddress( PARSER_ES_VIDEO, DS_VDBUF_ST_MM, DS_VDBUF_END_MM );
    }

}


//*****************************************************************************
// Description  :   Read information corresponding to the specified item
// Arguments    :   bInfo: the desired infomation type
//                  pdwData: ptr to the read value
// Return       :   TRUE or FALSE
// Side Effect  :
// *****************************************************************************
BYTE    HAL_ReadInfo(BYTE bInfo, DWORD *pdwData)
{
    DWORD   dwTemp;
    switch(bInfo)
    {
    case    HAL_INFO_FRAME_ID:
        *pdwData = __dwDISPTR;
        break;

    case    HAL_INFO_FRAME_ERROR:
        *pdwData = __dwPicErrCnt;
        break;

    case    HAL_INFO_HLI_STATUS:
        if (__dwSPType == SP_TYPE_DVD)
        {
            *pdwData = __blHLI_Status;
        }
        else
        {
            *pdwData = 0;
        }
        break;

    case    HAL_INFO_STILL:
        *pdwData = __dwSeqEndCode;
        break;

    case    HAL_INFO_DECCMD_ACK :
         // *pdwData = __dwPlayMode;
         *pdwData = REG_SRAM_PLAYMODE;
         break;

    case    HAL_INFO_DECFRAME:
        *pdwData = __dwDecFrame;
        break;

    // Chuan0.70, Add to query current Disp frame index
    case    HAL_INFO_DISPFRAME_INDEX:
        *pdwData = __dwM_sel;
        break;

#ifndef NO_PIP
    // Chuan0.70, Add to query current Disp PIP frame
    case    HAL_INFO_PIPFRAME:
        *pdwData = __dwPIP_disp_sel;
        break;
#endif  // #ifndef NO_PIP

    //0xffffff->0; 0->1; ...
    case    HAL_INFO_VIDEO_REMAIN:
        // Notice: must *2 to let output as DWORD unit (H/W register reports value is 64 bit unit)
        // *pdwData = ( (REG_MCU_VREM + BIT_BUFF_DIFF) & 0xFFFFFF ) * (BIT_BUFF_UNIT_32);
        *pdwData = MCU_VIDEO_BS_BUF_REMAINDER / 4; // Unit: DW
        break;

    case    HAL_INFO_AUDIO_REMAIN:
        // Notice: must *2 to let output as DWORD unit (H/W register reports value is 64 bit unit)
        /*
        if (_bHALABuf2Def)
        {
            *pdwData = ( (REG_MCU_A1REM + BIT_BUFF_DIFF) & 0xFFFFFF )* (BIT_BUFF_UNIT_32);
        }
        else
        {
            *pdwData = 0;
        }
        *pdwData += ( (REG_MCU_A0REM + BIT_BUFF_DIFF) & 0xFFFFFF ) * (BIT_BUFF_UNIT_32);
        */
        if (_bHALABuf2Def)
        {
            *pdwData = MCU_A1_BS_BUF_REMAINDER;
        }
        else
        {
            *pdwData = 0;
        }
        *pdwData += MCU_A0_BS_BUF_REMAINDER;
        *pdwData /= 4;  // Unit: DW
        break;

    case    HAL_INFO_AUDIO1_REMAIN:
        // Notice: must *2 to let output as DWORD unit (H/W register reports value is 64 bit unit)
        // *pdwData = ( (REG_MCU_A0REM + BIT_BUFF_DIFF) & 0xFFFFFF ) * (BIT_BUFF_UNIT_32);
        *pdwData = MCU_A0_BS_BUF_REMAINDER / 4; // Unit: DW
        break;

    case    HAL_INFO_AUDIO2_REMAIN:
        // Notice: must *2 to let output as DWORD unit (H/W register reports value is 64 bit unit)
        //A2-Rem=SPDIF raw W-R ptr for one buffer audio type such as AC3/DTS
        // *pdwData = ( (REG_MCU_A1REM + BIT_BUFF_DIFF) & 0xFFFFFF ) * (BIT_BUFF_UNIT_32);
        *pdwData = MCU_A1_BS_BUF_REMAINDER / 4; // Unit: DW
        break;

    case    HAL_INFO_VSIZE:
        *pdwData = __dwCoded_Picture_Height;
        break;

    case    HAL_INFO_HSIZE:
        *pdwData = __dwCoded_Picture_Width;
        break;

    case    HAL_INFO_STC:
        *pdwData = REG_PLAT_TIMER3_VALUE;
        break ;

    // LLY2.51, split audio bitrate data getting procedure from audio frequency.
    // Since, we may re-correct bitrate of variable bitrate MP3
    // So, if any call HAL_INFO_AUDIOFREQ after F/W re-correct it
    // Then, the value will be modify again.
    case    HAL_INFO_BITRATE:
        HAL_ReadAM(HAL_AM_BIT_RATE, &dwTemp);
        __wHALAudioBitrate = (WORD)dwTemp;
        *pdwData = dwTemp;
        break;

    case    HAL_INFO_AUDIOFREQ:
        // LLY2.51, split it to an indepentent case
        /*
        HAL_ReadAM(HAL_AM_BIT_RATE, &dwTemp);
        __wHALAudioBitrate = (WORD)dwTemp;
        */

        HAL_ReadAM(HAL_AM_SAMPLE_RATE, &dwTemp);

        // LLY1.20, assign the audio frequency first
        // Since, __dwHALAudioFreq means the real audio frequency (don't care up sample or not)
        switch((BYTE)dwTemp)
        {
        case    0x1:    // 8 KHz
            __dwHALAudioFreq=VALUE_8K;
            *pdwData = AUDIO_FREQ_8K;
            break;
        case    0x2:    // 16 KHz
            __dwHALAudioFreq=VALUE_16K;
            *pdwData = AUDIO_FREQ_16K;
            break;
        case    0x3:    // 32 KHz
            __dwHALAudioFreq=VALUE_32K;
            *pdwData = AUDIO_FREQ_32K;
            break;
        case    0x6:    // 11.025KHz
            __dwHALAudioFreq=VALUE_11K;
            *pdwData = AUDIO_FREQ_11K;
            break;
        case    0x7:    // 22.05 KHz
            __dwHALAudioFreq=VALUE_22K;
            *pdwData = AUDIO_FREQ_22K;
            break;
        case    0x8:    // 44.1 KHz
            __dwHALAudioFreq=VALUE_44K;
            *pdwData = AUDIO_FREQ_44K;
            break;
        case    0x9:    // 88.2 KHz
            __dwHALAudioFreq=VALUE_88K;
            *pdwData = AUDIO_FREQ_88K;
            break;
        case    0xa:    // 176.4 KHz
            __dwHALAudioFreq=VALUE_176K;
            *pdwData = AUDIO_FREQ_176K;
            break;
        case    0xb:    // 12 KHz
            __dwHALAudioFreq=VALUE_12K;
            *pdwData = AUDIO_FREQ_12K;
            break;
        case    0xc:    // 24 KHz
            __dwHALAudioFreq=VALUE_24K;
            *pdwData = AUDIO_FREQ_24K;
            break;
        case    0xd:    // 48 KHz
            __dwHALAudioFreq=VALUE_48K;
            *pdwData = AUDIO_FREQ_48K;
            break;
        case    0xe:    // 96 KHz
            __dwHALAudioFreq=VALUE_96K;
            *pdwData = AUDIO_FREQ_96K;
            break;
        case    0xf:    // 192 KHz
            __dwHALAudioFreq=VALUE_192K;
            *pdwData = AUDIO_FREQ_192K;
            break;
        default:
        case    0:
            __dwHALAudioFreq=0;
            *pdwData = AUDIO_FREQ_NONE;
            return FALSE;
            break;
        }
        //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "  __dwHALAudioFreq=%lx  __wHALAudioBitrate=%lx", __dwHALAudioFreq, __wHALAudioBitrate);

        break ;

    case    HAL_INFO_CAN_RUNTIME_RESET_PROC2:
        //can't be used when Proc2 does unpack
        //GR(1)[31]- written 1 by boot Proc2 code; 0 by small boot Proc2 code
        if ( (*(volatile DWORD *)0x80000784) >> 31 )
            *pdwData = TRUE;
        else
            *pdwData = FALSE;

        break;

    // wyc1.11a, return the disp mode for upper level code to reference.
    case    HAL_INFO_DISP_MODE_SET:
    {
        extern DWORD _dwModeSet;
        *pdwData = _dwModeSet;
        break;
    }

    // Chuan1.21, Query STC is updated or not.
    case    HAL_INFO_STC_UPDATED:
        *pdwData = __dwSTCUpdated;
        break;

#ifdef SUPPORT_STB //Aron2.17-909s, for stb
    // LLY2.17, porting Aron's code to get Aspect Ratio information.
    case    HAL_INFO_ASPECT_RATIO:
        *pdwData = __dwAspect_Ratio_Info;
        break;
#endif //

    // LLY2.21a, add to read back audio channel number
    case    HAL_INFO_CHANNEL_NO:
        // bit[1:0] : L/R/C;  bit[3:2]: Ls/Rs; bit[24]: LFE
        *pdwData = (REG_AIU_GR(14)&0x3) + ((REG_AIU_GR(14)>>2)&0x3) + ((REG_AIU_GR(14)>>24)&0x1);
        //printf("%lx, %lx, %lx, %lx\n", (REG_AIU_GR(14)&0x3), ((REG_AIU_GR(14)>>2)&0x3), ((REG_AIU_GR(14)>>24)&0x1), REG_AIU_GR(14));

        // LLY2.61, porting Grievous's code again since it seems lost for DVD_260 integration.
        // Grievous2.60, must return FALSE while the value is zero
        // to tell caller the information is not ready.
        if(*pdwData==0)
            return FALSE;
        break;

    // LLY2.22, add to read back FBR mode flag
    case    HAL_INFO_FBR_MODE:
        *pdwData = __dwFBRMode;
        break;

    // LLY2.22, add to read back DISP frame Y address
    case    HAL_INFO_DISPFRAME_YADDR:
        *pdwData = __DISPFrameInfo[(BYTE)(*pdwData)].dwFY_Addr;
        break;

    // LLY2.22, add to read back DISP frame UV address
    case    HAL_INFO_DISPFRAME_UVADDR:
        *pdwData = __DISPFrameInfo[(BYTE)(*pdwData)].dwFC_Addr;
        break;

    // LLY2.30, add to read back LPCM downsample or not flag
    case    HAL_INFO_LPCM_DOWNSAMPLE:
        *pdwData = (REG_AIU_GR(10)>>24 & 0x1);
        break;

    // LLY2.36, add to read back New STC value is valid or not
    // Notice: not implement now.
    case    HAL_INFO_NEW_STC_VALID:
        *pdwData = 1; //__dwUseNewSTC;
        break;

    default:
        return FALSE;
    }
    return TRUE;
}
//*********************************************************************
// Description  :   Write audio settings to Proc2
// Arguments    :   bType: parameter type
//                  pdwVal: ptr to memory where the read value is to be stored
// Return       :   None
// Notice       :   Using PROC1 vs. PROC2 command register
//                  [Issue mode]
//                  Para1[31:30]: 1 = write AM; 2 = read AM
//                  Para1[7:0]: Command ID
//                  Para2[31:0]: value
//                  REG_PLAT_PROC2_INT_FORCE[11] = 1, to set S/W INT to PROC2
//                  [Readback mode]
//                  Para1[31:30]: command ack
//                  Para2[31:0]: value
//*********************************************************************
void    HAL_WriteAM(BYTE bCmdId, DWORD dwVal)
{
//    DWORD dwHALTemp, dwAck=1;

#ifdef NO_PROC2
    return ;
#endif // #ifdef NO_PROC2


#ifdef DEBUG_RW_AM
    if (bCmdId!=HAL_AM_RESET_AUDIO)
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nHAL_WriteAM(%hx,%lx)", bCmdId, dwVal);
#endif // #ifdef DEBUG_RW_AM

    // Clear parameter 1 & 2 as zero first before using it.
    // Seems un-necessary ??
    REG_PLAT_PARAMETER1 = 0;
    REG_PLAT_PARAMETER2 = 0;

    // Set desired value into parameter 2
    switch (bCmdId)
    {
        ///// 1 bit /////
        case HAL_AM_DOWNSAMPLE_EN:
        case HAL_AM_UPSAMPLE_EN: //for MP3-LSF
        case HAL_AM_2CH_TO_6CH: //Duplicate
        case HAL_AM_LFE_OUTPUT:
        case HAL_AM_SPDIF_MODE:
        case HAL_AM_SPDIF_OUTPUT:
        case HAL_AM_KEYSHIFT_EN:
        case HAL_AM_SURR_EN:
        case HAL_AM_DTS_MODE:
        case HAL_AM_START_DECODE: //for MP3/WMA
        case HAL_AM_PROLOGIC_ENABLE:
        case HAL_AM_SKIP_ALL_FRAMES:
        case HAL_AM_PCM_MUTE:
        case HAL_AM_ENCODE_SPDIF_RAW:
        case HAL_AM_VIDEO_MASTER:
        case HAL_AM_VIDEO_UPDATED_STC:
        case HAL_AM_RESET_PTS:
        case HAL_AM_PROC2_WAIT_STC:
        case HAL_AM_TEST_CENTERSURR_DELAY:
        case HAL_AM_DBG_SHOWINFO:
        case HAL_AM_MPEG_STB_RADIO: // LLY2.21, for STB radio command
       	case HAL_AM_DECODE_DTS: // Aron2.36, for STB AC3 decode control
       	// LLY2.38, add to info DSP not update STC
       	// [0]: update STC; [1]: not update STC
       	case HAL_AM_AUDIO_NOT_UPDATE_STC:
    	// CMH2.55, add to enable PCM
    	case HAL_AM_PCM_BTYE_PER_SAMPLE:
        // LLY2.55, porting Grievious's code to support AM interface for PCM channel setting.
        case HAL_AM_PCM_CH_NUM:
        // Joey2.60: add for controlling of SPDIF PCM with/without volume control.
        case HAL_AM_DO_SPDIF_PCM_SCALE:
        // LLY2.61, add to tell audio decoder current CT909G type (5.1ch or 2 ch)
        case HAL_AM_CT909G_2CH:
        // Grievous2.77, provide A/V sync control for audio STC is great than STC 2 second will clean audio buffer for *.avi file.
       	case HAL_AM_AV_SYNC_2SEC_CONDITION:
            REG_PLAT_PARAMETER2 = dwVal&0x1;
            break;

        ///// 2 bits /////
        case HAL_AM_AC3_DOWNMIX_MODE:
        case HAL_AM_AC3_KOK_MODE:
        case HAL_AM_DUAL_MODE:
        case HAL_AM_PCM_MODE:
        case HAL_AM_BASS_MANAGE_CONFIG:
        case HAL_AM_DRC_MODE: //kevin0.76
            REG_PLAT_PARAMETER2 = dwVal&0x3;
            break;

        ///// 3 bits /////
        case HAL_AM_CHANNEL_MODE:
        case HAL_AM_FF_SPEED:
        case HAL_AM_CENTER_DELAY:
        case HAL_AM_EQ_MODE:	// Damon2.76, support Audio EQ setting
            REG_PLAT_PARAMETER2 = dwVal&0x7;
            break;

        ///// 4 bits /////
        case HAL_AM_PLAY_COMMAND:
        case HAL_AM_KEYSHIFT:
        case HAL_AM_EQ_AMPLITUDE_LEFT_SHIFT:
        case HAL_AM_DRC_LEVEL:
        case HAL_AM_AUDIO_TYPE:
        // LLY2.75, porting Joey's code, add AC3 downmix scale control.
        case HAL_AM_AC3_DOWN_MIX_SCALE:
            REG_PLAT_PARAMETER2 = dwVal&0xF;
            break;

        ///// 5 bits /////
        case HAL_AM_WMA_MPEG_ERROR_CONCEALMENT_MASK: // J500KWShih_230, mpeg error concealment
        case HAL_AM_SURROUND_DELAY:
            REG_PLAT_PARAMETER2 = dwVal&0x1F;
            break;

        ///// 6 bits /////
        case HAL_AM_PROLOGIC_CFG:
            REG_PLAT_PARAMETER2 = dwVal&0x3F;
            break;

        ///// 8 bits /////
        case HAL_AM_SURR_1:
        case HAL_AM_MIC_DETECT_THRESHOLD:
        // Joey2.60: add for PCM scale active channel control
        case HAL_AM_PCM_SCALE_ACTIVE_CHANNEL:
            REG_PLAT_PARAMETER2 = dwVal&0xFF;
            break;

        ///// 10 bits /////
        case HAL_AM_PROLOGIC_CTRL:
            REG_PLAT_PARAMETER2 = dwVal&0x3FF;
            break;

        ///// 12 bits /////
        case HAL_AM_ECHO_CR:
        case HAL_AM_SPECIAL_KEYUP_MODE:
            REG_PLAT_PARAMETER2 = dwVal&0xFFF;
            break;

        ///// 16 bits /////
        case HAL_AM_SKIP_THRESHOLD:
        case HAL_AM_PCMBUF1_LEN:
        case HAL_AM_PCMBUF2_LEN:
        case HAL_AM_PCMBUF_LEN:  //kevin0.72
        case HAL_AM_MIC_PCM_SCALE:
        case HAL_AM_HDCD_CTRL:

        case HAL_AM_PCM_SCALE:
#ifdef CT909G_IC_SYSTEM
            // Joey2.60: 909G PCM scaler has only 8 bits
            if(bCmdId == HAL_AM_PCM_SCALE)
            {
                dwVal = (dwVal >> 7) & 0xff;
            }
#endif

            REG_PLAT_PARAMETER2 = dwVal&0xFFFF;
            break;

        ///// 17 bits /////
        //Request
        case HAL_AM_RESET_AUDIO:
            REG_PLAT_PARAMETER2 = 0x10000;
            break;
        case HAL_AM_LOAD_CODE:
            REG_PLAT_PARAMETER2 = 0x10001;
            break;
        case HAL_AM_WRITE_COMRAM:
            REG_PLAT_PARAMETER2 = 0x10002;
            break;
        case HAL_AM_AUDIO_START:
            REG_PLAT_PARAMETER2 = 0x10003;
            break;

        ///// 20 bits /////
        case HAL_AM_SURR_0:
            REG_PLAT_PARAMETER2 = dwVal&0xFFFFF;
            break;

        ///// 24 bits /////
        case HAL_AM_FRAME_SAMPLE_SEC_CNT:
            REG_PLAT_PARAMETER2 = dwVal&0xFFFFFF;
            break;

        ///// 32 bits /////
        case HAL_AM_PCMBUF1_ADR:
        case HAL_AM_PCMBUF2_ADR:
        case HAL_AM_PCMBUF0_ADR:
        case HAL_AM_PCMBUF_ADR:  //kevin0.72
        case HAL_AM_SKIP_FRAME_NO:
        case HAL_AM_DBGBUF_ADR:
        case HAL_AM_DBGBUF_IDX:
        case HAL_AM_LSBUF_ADR:  // J500KWShih_218, buffer address for language study
            REG_PLAT_PARAMETER2 = dwVal&0xFFFFFFFF;
            break;

        default:
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "switch case error!\n");
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nHAL_WriteAM(%hx,%lx)", bCmdId, dwVal);
    }


    // Set desired command, and write action in parameter 1
    REG_PLAT_PARAMETER1 = bCmdId | 0x40000000;

    // Invoke S/W INT to PROC2 after param1 & param2 is ready
    REG_PLAT_PROC2_INT_FORCE |= (1<<11); //software INT

    if(!_Wait_AccessAM_CMD_ACK())
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, " in HAL_WriteAM:%hx\n", bCmdId);
    }
}


//**********************************************************************************
// Description  :   Read audio parameters, such as sampling rate
// Arguments    :   bType: parameter type
//                  pdwVal: ptr to memory where the read value is to be stored
// Return       :   TRUE: succeed
//                  FALSE: Wait no Proc2 ACK
// Notice       :   Using PROC1 vs. PROC2 command register for R/W AM procedure
//                  [Issue mode]
//                  Para1[31:30]: 1 = write AM; 2 = read AM
//                  Para1[7:0]: Command ID
//                  Para2[31:0]: value
//                  REG_PLAT_PROC2_INT_FORCE[11] = 1, to set S/W INT to PROC2
//                  [Readback mode]
//                  Para1[31:30]: command ack
//                  Para2[31:0]: value
//*********************************************************************************
BYTE    HAL_ReadAM(BYTE bCmdId, DWORD* pdwVal)
{
#ifdef NO_PROC2
    *pdwVal = 1;
    return TRUE;
#endif // #ifdef NO_PROC2

//    DWORD dwHALTemp, dwAck=1;

    // Clear parameter 1 & 2 as zero first before using it.
    // Seems un-necessary ??
    REG_PLAT_PARAMETER1 = 0;
    REG_PLAT_PARAMETER2 = 0;

    // Set desired command, and read action in parameter 1
    REG_PLAT_PARAMETER1 = bCmdId | 0x80000000;

    // Invoke S/W INT to PROC2 after param1 & param2 is ready
    REG_PLAT_PROC2_INT_FORCE |= (1<<11); //software INT

    // LLY2.00, collect the same procedure to wait command ACK
    if(_Wait_AccessAM_CMD_ACK())
    {
        *pdwVal = REG_PLAT_PARAMETER2;
    }
    else
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, " in HAL_ReadAM:%hx\n", bCmdId );
        return FALSE;
    }


    switch (bCmdId)
    {
        ///// 1 bit /////
        // LLY2.77, porting CoCo's since audio code don't use "HAL_AM_WMA_DRM" now
        // And, replace the position w/ new meaning.
        //case HAL_AM_WMA_DRM:
        case HAL_AM_WMA_L1_DECODE:
        case HAL_AM_LPCM_DEEMPHASIS:
        //ACK
        case HAL_AM_RESET_AUDIO:
        case HAL_AM_LOAD_CODE:
        case HAL_AM_WRITE_COMRAM:
        case HAL_AM_AUDIO_START:
        case HAL_AM_MP2_BITSTREAM:
        case HAL_AM_MP3MP2_CH_NO:
        // LLY2.55, porting Grievious's code to support AM interface for PCM channel reading.
        case HAL_AM_PCM_CH_NUM:
            *pdwVal &= 0x01;
            break;

        ///// 2 bits /////
        case HAL_AM_MP3_LSF:
        case HAL_AM_DOLBY_SURR_FLAG: //kevin0.76
            *pdwVal &= 0x03;
            break;

        ///// 3 bits /////
        case HAL_AM_ACMOD:
            *pdwVal &= 0x07;
            break;

        ///// 4 bits /////
        case HAL_AM_SAMPLE_RATE:
        case HAL_AM_PROC2_SUPPORT:
            *pdwVal &= 0x0F;
            break;

        ///// 9 bits /////
        case HAL_AM_PROC2_VERSION:
            *pdwVal &= 0x1FF;
            break;

        ///// 12 bits /////
        case HAL_AM_SAMPLES_PER_FRAME: //MP3/MP2
        case HAL_AM_TOTAL_TIME:
            *pdwVal &= 0xFFF;
            break;

        ///// 14 bits /////
        case HAL_AM_BIT_RATE:
            *pdwVal &= 0x3FFF;
            break;

        ///// 16 bits /////
        case HAL_AM_WMA_BLOCK_ALIGN:
        case HAL_AM_WMA_HEADER_SIZE:   // J500CSC2.25, add new command for getting WMA header length
        case HAL_AM_WMA_PACKET_SIZE:   // J500CSC2.25, add new command for getting WMA packet length

        case HAL_AM_REAL_PCM_SCALE:
#ifdef CT909G_IC_SYSTEM
            // Joey2.60: 909G PCM scaler has only 8 bits
            if(bCmdId == HAL_AM_REAL_PCM_SCALE)
            {
                *pdwVal <<= 7;
            }
#endif

            *pdwVal &= 0xFFFF;
            break;

        ///// 24 bits /////
        case HAL_AM_FRAME_SAMPLE_SEC_CNT:
            *pdwVal &= 0xFFFFFF;
            break;

        ///// 32 bits /////
        case HAL_AM_EQ_LOW:
        case HAL_AM_EQ_HIGH:
            *pdwVal &= 0xFFFFFFFF;
            break;

        default:
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "switch case error!\n");
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nHAL_ReadAM(%hx,%lx)", bCmdId, *pdwVal);

    }

#ifdef DEBUG_RW_AM
    if (bCmdId!=HAL_AM_RESET_AUDIO)
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nHAL_ReadAM(%hx,%lx)", bCmdId, *pdwVal);
#endif // #ifdef DEBUG_RW_AM

    return TRUE;
}
void    HAL_ResetAudioDecoder(void)
{
#ifndef NO_PROC2
    BYTE  bResetProc2 = FALSE;
    DWORD dwTimer, dwTemp;
#endif  // #ifndef NO_PROC2

    // Step 2: Start the necessary action for reset case
    // [2-1] Reset Audio part
#ifndef NO_PROC2
        // notify Proc2 the "reset audio" command. Proc2 will clear read ptr, reset FIFO, HW, ...
        HAL_WriteAM(HAL_AM_RESET_AUDIO, 1);

        // wait for Proc2's ACK.
    	dwTimer = OS_GetSysTimer();
        while( (OS_GetSysTimer()-dwTimer) < COUNT_200_MSEC )
    	{
        	if (HAL_ReadAM(HAL_AM_RESET_AUDIO, &dwTemp))
        	{
        	    if(!dwTemp)
        	    {
				    break;
        	    }
            }
            else
            {
                dwTemp = 1;
            }

            // release CPU resource to other threads
            OS_YieldThread();
        }

        if(dwTemp)
        {
            // LLY1.21a, read-back PROC2 PC by macro, don't access AIU directly
            DWORD   dwPC;
            MACRO_PLAT_GET_PROC2_PC(dwPC);

            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, " reset audio fail");
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, " PC2:%lx", dwPC );

            bResetProc2 = TRUE;
        }

        // notify parser to clear write ptr.
        //Ming0.80a, Sometimes audio remainder becomes 0xFFFFEF after audio buffer reset. Must reset again in this condition.
        dwTimer = OS_GetSysTimer();
        do{
            // notify parser to clear write ptr.
            PARSER_BufferReset( PARSER_ES_AUDIO );
            HAL_ReadInfo(HAL_INFO_AUDIO1_REMAIN, &dwTemp);
        } while((dwTemp != 0) && ((OS_GetSysTimer() - dwTimer) < COUNT_50_MSEC));

        if(dwTemp != 0)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "ERR: Reset audio buffer fail! A0Rem = %lx\n", dwTemp);
        }

        if (bResetProc2) //kevin0.95, reset Proc2
        {
            HAL_ReloadAudioDecoder(__bAudioType);

            /// let Proc2 returns to reset audio state ///
            HAL_WriteAM(HAL_AM_RESET_AUDIO, 1);
        	dwTimer = OS_GetSysTimer();
    	    while( (OS_GetSysTimer()-dwTimer) < COUNT_200_MSEC )
            {
            	HAL_ReadAM(HAL_AM_RESET_AUDIO, &dwTemp);
            	if(!dwTemp)
            	{
                    break;
            	}
                OS_YieldThread();
            }
            if(dwTemp)
            {
                // LLY1.21a, read-back PROC2 PC by macro, don't access AIU directly
                DWORD   dwPC;
                MACRO_PLAT_GET_PROC2_PC(dwPC);

                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, " reset audio2 fail");
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, " PC2:%lx", dwPC );
            }
            bResetProc2 = FALSE;
        }

#endif  // #ifndef NO_PROC2

}
//**********************************************************************************
// Description  :   Set back the audio frequency information
// Arguments    :   dwFreqValue,   the frequency value
// Return       :   None
// Notice       :   !!!!!This function only can be called at audioi reset state.
//*********************************************************************************
void    HAL_SetAudioFrequency(DWORD dwFreqValue)
{
    DWORD   dwTemp;
    dwTemp = REG_AIU_GR(14);
    dwTemp &= ~0x000F0000;
	// CMH2.55, mask the tail 2 decimal number
    // since the value is the average after parser calculation
    switch(dwFreqValue/100)
    {
        case    80:
            dwTemp |= 0x1<<16;
            break;
        case    160:
            dwTemp |= 0x2<<16;
            break;
        case    320:
            dwTemp |= 0x3<<16;
            break;
        case    110:
            dwTemp |= 0x6<<16;
            break;
        case    220:
            dwTemp |= 0x7<<16;
            break;
        default:
        case    441:
            dwTemp |= 0x8<<16;
            break;
        case    882:
            dwTemp |= 0x9<<16;
            break;
        case    1764:
            dwTemp |= 0xa<<16;
            break;
        case    120:
            dwTemp |= 0xb<<16;
            break;
        case    240:
            dwTemp |= 0xc<<16;
            break;
        case    480:
            dwTemp |= 0xd<<16;
            break;
        case    960:
            dwTemp |= 0xe<<16;
            break;
        case    1920:
            dwTemp |= 0xf<<16;
            break;
    }
    // Set desired audio frequency
    REG_AIU_GR(14) = dwTemp;
}


// LLY2.55, porting Grievous's code to remvoe this API
// Since, we can set audio channel information by write AM.
#if 0
//**********************************************************************************
// Description  :   Set back the audio channel information
// Arguments    :   bChNo,   the channel number
// Return       :   None
// Notice       :   !!!!!This function only can be called at audioi reset state.
//*********************************************************************************
void    HAL_SetAudioChannel(BYTE bChNo)
{
    DWORD   dwTemp;
    // CMH2.55, add to set PCM channel number
    dwTemp = REG_AIU_GR(13);
    dwTemp &= ~0x00000400;
    dwTemp |= (DWORD)bChNo << 10; //CH no
    REG_AIU_GR(13) = dwTemp;
}
#endif  //








