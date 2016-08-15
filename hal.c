#include "winav.h"
#include "ctkav.h"
#include "hal.h"
#include "srcfilter.h"
#include "parser.h"
//#include "mpeg2dec.h"
#include "comdec.h"
#include "disp.h"
#include "utl.h"
#include "ctkav_mcu.h" //kevin0.67
#include "char_subpict.h"

#ifdef ECOS909
#include "eCoscc.h"
#endif
#include "subpict.h"
#include "cc.h"
#include "chips.h"
#include "infofilter.h"
#include "setup.h"
#include "initial.h"
#include "gdi.h"
#include "jpegfmt.h"
#include "cdg.h"
#include "media.h" //CoCo2.16, add for STB
#include "mmanager.h"

//////////////////////////////////////////////////////////////////////////////////
//  Big modification history
// LLY2.01, extend the GPIO R/W API, that it can specify the group

// LLY2.04, remove the un-necessary global variables
// ex. 908 used only (register related variables)
//     temp global variables

// LLY2.05, remove the un-necessary code
// - HAL_SetSectorFormat() since nobody ues it now.

// LLY2.10, remove CT909A related code

// LLY2.11, remove pin related define to pio.h
// ex. IOMUTE/ EEPROM_SCK/ EEPROM_DAT/ SCART_SW/ SCART_TV/ SCART_RGB

////////////////////////////////////////////////////////////////////////////////
// Some define for debug code usage
// If define it, it will print-out R/W AM procdure value
//#define DEBUG_RW_AM


//#define     NONE    0 //NULL


////////////////////////////////////////////////////////////////////////////////////
///// Global Vars /////
WORD    __wHALAudioBitrate=0;
WORD    __wHALSamplesPerFrame=1150;
BYTE    __bCDDA_DTS;// =CDDA_FORMAT_UNKNOWN;
BYTE    __bMPEGVideoMode;
BYTE    __bTVType;
#ifdef SUPPORT_TFT_DIGITAL_MODE
BYTE    __bTFTMode;
BYTE    __bTFTOutput;
#endif // SUPPORT_TFT

WORD    __wPicVsize;//=240; // keep picture vertical size
WORD    __wPicHsize;//=352; // keep picture horizontal size

WORD    __wHCW, __wVCW;  // keep picture hor. & ver. size while clip command
SHORT   __wXStart=0, __wYStart=0;

WORD    __wHALAVIDRLow=0x89E0, __wHALAVIDRHigh=0x000F;

BYTE   __bZoomPos=0;  // LLY2.20, keep last zoom position status


// LLY2.04, only define _REG[26] array while enable "#define SUPPORT_TFT"
// Since, TFT related code will shared this array, and the code is same between 908 and 909
#ifdef  SUPPORT_TFT
BYTE   _REG[26];
#endif  // #ifdef SUPPORT_TFT



///// Internal Vars /////
// LLY.103, keep video display mode: WIDE/ LB/ PS. It's the real setting value of display mode
// which is relative to bitstream supporting: LB/PS/WIDE. So, it's may different from __SetupInfo.bAspectRatio value
BYTE    _bVideoDisplayMode;
BYTE    _bSampleFreq=AUDIO_FREQ_NONE;

// LLY1.50, remove it since nobody use it now.
//DWORD   _dwHALGPCDir=0x0,         _dwHALGPCData=0; //Alan0.69



///// Internal Funs /////
BYTE    _ControlDownSample(BYTE bSampleFreq);

///// Extern Vars & function prototype/////
extern  BYTE     __bNTSCBitStream;  // define in CC module
extern  WORD    __wCurrentVBuffer;
extern  BYTE    __bLOGO;
extern  BYTE    __btDriveOpen;
extern  BYTE    __bMin, __bSec, __bFrame;
extern  DWORD   __dwSTCUpdated;
extern  void    _SetAudType(BYTE bType);  // in CHIPS module

// Micky0.70, add aspect ration related begin
DWORD	__dwPicHSize;
DWORD	__dwPicVSize;
#include 	"dvdinfo.h"
#include	"navigate.h"
// Micky0.70, add aspect ration related end


// LLY2.20, porting Chuan's code to use a define for Vertical/ Horizontal start address.
// Notice: HStart + 720 must <= 858 (NTSC)
//         HStart + 720 must <= 864 (PAL)
//         VStart + 480 must <= 525 (NTSC)
//         VStart + 576 must <= 625 (PAL)
#ifdef VIDEO_LOCATE_AT_CENTER_OF_SONY_MONITOR
// Chuan, Locate at center of SONY Monitor
#define     VIDEO_HSTART_NTSC           (101)
#define     VIDEO_VSTART_NTSC           (46)
#define     VIDEO_HSTART_NTSC_PSCAN     (98)
#define     VIDEO_VSTART_NTSC_PSCAN     (43)
#define     VIDEO_HSTART_PAL            (109)
#define     VIDEO_VSTART_PAL            (48)
#define     VIDEO_HSTART_PAL_PSCAN      (109)
#define     VIDEO_VSTART_PAL_PSCAN      (45)
#else   // #ifdef VIDEO_LOCATE_AT_CENTER_OF_SONY_MONITOR
// Chuan, Locate at center of TV
#define     VIDEO_HSTART_NTSC           (113)
#define     VIDEO_VSTART_NTSC           (46)
#define     VIDEO_HSTART_NTSC_PSCAN     (110)
#define     VIDEO_VSTART_NTSC_PSCAN     (43)
#define     VIDEO_HSTART_PAL            (119)
#define     VIDEO_VSTART_PAL            (48)
#define     VIDEO_HSTART_PAL_PSCAN      (119)
#define     VIDEO_VSTART_PAL_PSCAN      (45)
#endif  // #ifdef VIDEO_LOCATE_AT_CENTER_OF_SONY_MONITOR
// Grievous2.75, Add for aspect ratio debug
// #define ASPECT_RATIO_DEBUG
// Grievous2.75, need or not to do Pan&Scan
//#define ASPECT_RATIO_PANSCAN
#define     DIVX_BORDER_PERC        (0)         //The Percent Number of Border



//********************************************************************************
// Description :   Based on the desired audio type to issue relative setting:
// Arguments   :   bType, the desired audio type
// Return      :   None
// Side Effect :   Must call HAL_ControlParser(HAL_PARSER_AUDIO_ID,XX)
//                 to set correct audio ID
//********************************************************************************
void    HAL_SetAudioType(BYTE bType)
{
    // LLY2.18, nothing need to do if audio type is unknown
    if(bType == HAL_AUDIO_NONE)
    {
        return;
    }

    __wHALAVIDRLow &= 0x1FFF; // clear AVIDR[15:13] packet stream ID

    _SetAudType(bType);

    HAL_WriteAM(HAL_AM_UPSAMPLE_EN, 0);  //kevin1.01

    // LLY2.30, always disable LPCM down-sample setting while non-LPCM audio stream
    if(bType != HAL_AUDIO_LPCM)
    {
        HAL_WriteAM(HAL_AM_DOWNSAMPLE_EN, 0);
    }

    //set audio buffer / control parameters
    switch(bType)
    {
    case    HAL_AUDIO_PCM:
        __wHALAVIDRLow |= 0xA000;
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 3);
	    HAL_WriteAM(HAL_AM_PCM_MODE, 1); //kevin0.80, set in CHIPS_ControlHDCD

	    //CMH2.55, enable PCM audio upsample
#ifdef  SUPPORT_PCM_AUDIO_UP_SAMPLE
		HAL_WriteAM(HAL_AM_UPSAMPLE_EN, 1);
#endif  // #ifdef  SUPPORT_PCM_AUDIO_UP_SAMPLE

        HAL_SetBuffer(HAL_SETBUFFER_AUDIO, HAL_AUDIOBUF_PCM);

        break;
    case    HAL_AUDIO_MPG:
        __wHALAVIDRLow |= 0xC000;
        HAL_WriteAM(HAL_AM_START_DECODE, 1);
#ifdef  SUPPORT_MPEG_AUDIO_UP_SAMPLE
        HAL_WriteAM(HAL_AM_UPSAMPLE_EN, 1);
#endif  // #ifdef  SUPPORT_MPEG_AUDIO_UP_SAMPLE
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 2);

        HAL_SetBuffer(HAL_SETBUFFER_AUDIO, HAL_AUDIOBUF_MPG);

        break;
    case    HAL_AUDIO_AC3:
         //the range for AC3 is 0x80 ~ 0x87
        __wHALAVIDRLow &= 0x07ff;
        __wHALAVIDRLow |= 0x8000;
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 1);
        HAL_WriteAM(HAL_AM_ENCODE_SPDIF_RAW, 1);

        HAL_SetBuffer(HAL_SETBUFFER_AUDIO, HAL_AUDIOBUF_AC3);

        break;
    case    HAL_AUDIO_WMA:
        // LLY2.32, set audio type as 6 for WMA
        // Since, original setting procedure is removed from _ChangeDSPCode()
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 6);

        // Notice: WMA buffer mode is same as MP3
        // LLY2.35, give audio buffer setting mode as WMA
        // Since, only WMA will use 2 audio buffer from v2.35
        //HAL_SetBuffer(HAL_SETBUFFER_AUDIO, HAL_AUDIOBUF_MP3);
        HAL_SetBuffer(HAL_SETBUFFER_AUDIO, HAL_AUDIOBUF_WMA);

        break;
    case    HAL_AUDIO_MP3:
    case    HAL_AUDIO_MP2:
        //kevin0.80, move to MONITOR_AudioStream()
        //HAL_WriteAM(HAL_AM_START_DECODE, 0);

#ifdef  SUPPORT_MPEG_AUDIO_UP_SAMPLE
        HAL_WriteAM(HAL_AM_UPSAMPLE_EN, 1);
#endif  // #ifdef  SUPPORT_MPEG_AUDIO_UP_SAMPLE
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 2);

        HAL_SetBuffer(HAL_SETBUFFER_AUDIO, HAL_AUDIOBUF_MP3);

        break;
    case    HAL_AUDIO_LPCM:
        __wHALAVIDRLow |= 0xA000;
        //_ControlDownSample();
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 3);
	    HAL_WriteAM(HAL_AM_PCM_MODE, 0); //kevin0.80, fix CDDA->LPCM => all noise

        HAL_SetBuffer(HAL_SETBUFFER_AUDIO, HAL_AUDIOBUF_LPCM);

        break;
    case    HAL_AUDIO_DTS:
        //the range for DTS is 0x88 ~ 0x8F
        __wHALAVIDRLow &= 0x07ff;
        __wHALAVIDRLow |= 0x8800;

        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 4);
        HAL_WriteAM(HAL_AM_ENCODE_SPDIF_RAW, 1);

		//distinguish DVD-DTS from CD-DTS
		if (__wDiscType & BOOK_DVD)
        {
            HAL_WriteAM(HAL_AM_DTS_MODE, 0);
        }
		else if (__wDiscType & BOOK_CDDA)
        {
			HAL_WriteAM(HAL_AM_DTS_MODE, 1);
        }

        HAL_SetBuffer(HAL_SETBUFFER_AUDIO, HAL_AUDIOBUF_DTS);

        break;

    // LLY2.10, new added audio format: AAC for NeroDigital
    // LLY2.17, set audio buffer for AAC
    case    HAL_AUDIO_AAC:
        HAL_SetBuffer(HAL_SETBUFFER_AUDIO, HAL_AUDIOBUF_AAC);
        break;

    default:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Not implement case for HAL_SetAudioType(): %lx\n", bType);
        break;
    }

    HAL_Reset(HAL_RESET_AUDIO) ;
}



//  ***********************************************************************************************
//  Function    :   HAL_SetAudioDAC
//  Description :   Set ACLK & Program audio DAC, it also handle down/ up sameple control
//  Arguments   :   bSampleFreq, sampleing freq.
//  Return      :   None
//  Side Effect :   None
//  ***********************************************************************************************
// If define it, it will print-out current audio frequency for ADAC
//#define DEBUG_ADAC_FREQ
void    HAL_SetAudioDAC(BYTE bSampleFreq) //test OK
{
    if (bSampleFreq > AUDIO_FREQ_END)
    {
#ifdef SUPPORT_PRINTF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nAudio freq/ Bit len out of range!") ;
#endif
        return;
    }

    // control downsampling
    // LLY2.30, remove LPCM down sample procedure outside
    // And, let controller to do it by itself.
    // bSampleFreq = _ControlDownSample(bSampleFreq);

#ifdef SUPPORT_MPEG_AUDIO_UP_SAMPLE //kevin1.11c, fix MP3 SampleRate display is wrong & time display is too slow
    if ( bSampleFreq==AUDIO_FREQ_8K || bSampleFreq==AUDIO_FREQ_16K || bSampleFreq==AUDIO_FREQ_11K || bSampleFreq==AUDIO_FREQ_22K || bSampleFreq==AUDIO_FREQ_12K || bSampleFreq==AUDIO_FREQ_24K)
    {
        if ( (__bAudioType == HAL_AUDIO_MP2) || (__bAudioType == HAL_AUDIO_MP3) || (__bAudioType == HAL_AUDIO_MPG) )
        {
            DWORD   dwLSFMode;
            HAL_ReadAM( HAL_AM_MP3_LSF, &dwLSFMode);

            switch (dwLSFMode)
            {
                case 1: //MP3 LSF
                    bSampleFreq++;
                    break;
                case 3: //MP3 2.5
                    bSampleFreq+=2;
                    break;
            }
        }
        /* //not support upsampling temp.
        else if (__bAudioType == HAL_AUDIO_WMA) //kevin1.01
        {
            _dwHALTemp2++;
        }
        */
    }
#endif

// CMH2.55, up sample PCM audio frequncy to 32k/44.1k/48k
#ifdef SUPPORT_PCM_AUDIO_UP_SAMPLE
	if((__bAudioType== HAL_AUDIO_PCM) && (__bAttrPlay == ATTR_AVI))
	{
		switch(bSampleFreq)
		{
		case 1:		//8k
		case 6:		//11k
		case 0xb:	//12k
			bSampleFreq+=2;
			break;
		case 2:		//16k
		case 7:		//22k
		case 0xc:	//24k
			bSampleFreq++;
			break;
		default:
			break;
		}
	}
#endif  // #ifdef SUPPORT_PCM_AUDIO_UP_SAMPLE


    //Kevin1.26, don't set ACLK frequency & program ADAC again if new sampling freq == previous sampling freq
    // Writting reg. ACCR to program ACLK may cause SPDIF bo noise or discont. sound
    if (bSampleFreq == _bSampleFreq) //new==previous
        return;

#ifdef SUPPORT_PRINTF
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "bSampleFreq=%hx\n", bSampleFreq);
#endif

    // Step 1: keep audio frequency in global variable
    _bSampleFreq = bSampleFreq;

#ifdef  DEBUG_ADAC_FREQ
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "DAC Freq: %lx\n", _bSampleFreq);
#endif  // #ifdef DEBUG_ADAC_FREQ

    // Micky2.11a
    // Program ADAC
    HAL_ADAC_Set(_bSampleFreq, SUPPORT_PCM_WORD_LENGTH);
}



//  **********************************************************************
//  Function    :   HAL_LPCM_DownSample
//  Description :   Control LPCM downsampling
//  Arguments   :   bOn, specify downsample procedure on/off
//  Return      :   TRUE, allow to do downsample
//                  FALSE, dis-allow to do downsample
//  Return      :   None
//*********************************************************************
// LLY2.30, rename it, and re-modify it as general API, not only for HAL_SetAudioDAC() to set.
//#define DEBUG_LPCM_DOWNSAMPLE
BYTE    HAL_LPCM_DownSample(BYTE bOn)
{
	if (__bAudioType==HAL_AUDIO_LPCM)
    {
        //SPDIF supports 32K, 44K, and 48KHz only
 	    if( bOn && ((__SetupInfo.bAudioLPCM == SETUP_AUDIO_LPCM_48K) ||
        	(__SetupInfo.bAudioDigitalOut != SETUP_AUDIO_DIGITAL_OUT_OFF) )	)
        {
#ifdef  DEBUG_LPCM_DOWNSAMPLE
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Do LPCM downsample\n");
#endif  // #ifdef DEBUG_LPCM_DOWNSMAPLE

            HAL_WriteAM(HAL_AM_DOWNSAMPLE_EN, 1);
            return TRUE;
        }
        else
        {
#ifdef  DEBUG_LPCM_DOWNSAMPLE
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Don't LPCM downsample\n");
#endif  // #ifdef DEBUG_LPCM_DOWNSMAPLE

            HAL_WriteAM(HAL_AM_DOWNSAMPLE_EN, 0);
            return FALSE;
        }
    }
    return FALSE;
}


//  *******************************************************************************
//  Function    :   HAL_SetTVE
//  Description :   Base on current user choice (ex. setup) to program TVE
//  Arguments   :   None
//  Return      :   None
//  Side Effect :   None
//  *******************************************************************************
void    HAL_SetTVE(void) //test OK on Non-Interlaced + NTSC + AV
{
    int nGainOffset;
    BYTE bScanMode, bTVSystem, bVideoOutput;

    // LLY2.10, remove following protection procedure
    // Since, current flow will not generate those case
#if 0
    //Kevin1.05, add protection
    if ((__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON) &&
        (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_NORMAL))
    {
        __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN;
        SETUP_ChangeValue(SETUP_SCREEN_VIDEO_OUTPUT);
    }

    if ((__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_OFF) &&
        (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_VGA))
    {
        __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_NORMAL;
        SETUP_ChangeValue(SETUP_SCREEN_VIDEO_OUTPUT);
    }
#endif  // #if 0


    //Scan mode
    if ( __SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON ) //Progressive Scan
    {
        bScanMode = PSCAN_ON;
    }
    else
    {
        bScanMode = PSCAN_OFF;
    }

    //TV system
    if ( __bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC)
    {
        bTVSystem = TVSYSTEM_NTSC;
    }
    else if ( __bTVType == SETUP_SCREEN_TV_SYSTEM_PAL)
    {
        bTVSystem = TVSYSTEM_PAL;
    }
    else if ( __bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M)
    {
        bTVSystem = TVSYSTEM_PALM;
    }
    else
    {
        bTVSystem = TVSYSTEM_PALN;
    }

    //Video output
    if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_NORMAL)
    {
        bVideoOutput = VIDEOOUTPUT_AV_SVIDEO;
    }
    else if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_YUV)
    {
        bVideoOutput = VIDEOOUTPUT_AV_YUV;
    }
    else if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_RGB)
    {
        bVideoOutput = VIDEOOUTPUT_AV_RGB;
    }
    else if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN)
    {
        bVideoOutput = VIDEOOUTPUT_YUV_PSCAN;
    }
    else if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN)
    {
        bVideoOutput = VIDEOOUTPUT_RGB_PSCAN;
    }
    else //if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_VGA)
    {
        bVideoOutput = VIDEOOUTPUT_VGA_PSCAN;
    }

    // Alan2.35, to support the adjustment of the chroma
#ifdef SUPPORT_ADJUST_CHROM_PHASE
    nGainOffset = __SetupInfo.bChromPhaseOffset - SETUP_SCREEN_CHROM_PHASE_0;
#else
    nGainOffset = 0;
#endif

    // Micky2.11a
    if (!__bTVE_JPEG) //MPEG
    {
        HAL_TVE_Set(HAL_TVE_COLOR_SPACE_MPEG, bScanMode, bTVSystem, bVideoOutput, nGainOffset);
    }
    else
    {
        HAL_TVE_Set(HAL_TVE_COLOR_SPACE_JPEG, bScanMode, bTVSystem, bVideoOutput, nGainOffset);
    }

}

//*****************************************************************************
// Description  :   Set video/audio buffer starting address and size
// Arguments    :   bType: specify Audio or Video buffer address setting
//                      - HAL_SETBUFFER_AUDIO: set audio buffer
//                      - HAL_SETBUFFER_VIDEO: set video buffer
//                  bMode: specify the desired mode for A/V buffer
//                      - HAL_AUDIOBUF_XXX: for audio buffer
//                      - HAL_VIDEOBUF_XXX: for video buffer
// Return       :   None
//*****************************************************************************
void    HAL_SetBuffer(BYTE bType, BYTE bMode)
{
    if(bType == HAL_SETBUFFER_AUDIO)
    {
        // LLY2.35, issue audio reset since F/W will change DSP unpack buffer address while AVI playback.
        HAL_Reset(HAL_RESET_AUDIO) ;

        // Micky2.11a
        HAL_SetAudio_Buffer(__bAttrPlay, bMode);
    }
    else if(bType == HAL_SETBUFFER_VIDEO)
    {
        HAL_SetVideo_Buffer(bMode);
    }
}



//***********************************************************************************************
// Description  :   Issue relative actions while entering / exiting power down mode
// Arguments    :   bPowerDown:
//                      ENTER_POWERDOWN: enter power down mode
//                      INITIAL_SYSTEM: power on
//                      | PSEUDO_SHUTDOWN: IR power off/on
// Return       :   None
// Side Effect  :
// Notice       :   Not check REG_PLAT_CLK_GENERATOR_CONTROL for CT909S @ power saving mode
//***********************************************************************************************
void    HAL_PowerDown(BYTE bPowerDown) //test OK except HAL_RESET_TOTAL, SrcFilter_*
{
#if 0
#ifndef IR_POWERDOWN_MODE
    //#define IR_POWERDOWN_MODE       PSEUDO_SHUTDOWN in customer.h
    #define     IR_POWERDOWN_MODE   1
#endif
#endif//


    if(bPowerDown & ENTER_POWERDOWN) //always IR_POWERDOWN_MODE
    {
        HAL_Reset(HAL_RESET_TOTAL);

        HAL_SetSCART(HAL_SCART_POWER_STANDBY);

        // LLY1.50, HAL_TurnOffTVE() is rename as TVE_TurnOff()
        //HAL_TurnOffTVE();
        HAL_TVE_Off();

        // LLY2.38, turn-off CCIR656 signal while power down mode to avoid noise during AV In mode
#ifdef  SUPPORT_CCIR656
        // CCIR656: Set DVI[22]=1
        // CCIR601: Set DVI[22]=1 and SyncOut[23]=1
        REG_PLAT_SYSTEM_CONFIGURATION1 &= ~(1<<22);
#endif  // #ifdef SUPPORT_CCIR656

        // LLY0.80, stop decoder/ parser
        HAL_PlayCommand(COMMAND_STOP, 0);
        HAL_ControlParser(HAL_PARSER_STOP, 0);

        //close door & stop SERVO.
        SrcFilter_CloseTray();
        // wyc2.53-909P,
        SrcFilter_Stop(__dwSFStreamID);

        SrcFilter_PowerMode (SERVO_POWER_STOP) ;

        // wyc2.05, need to call this function to exit source HW mode. Especially in USB mode.
        SRCFilter_ExitSource();

        // LLY0.80, turn-off main video display after decoder stop
        DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);  //kevin0.76, fix IR power off -> power on => previous picture appears first, then logo is shown
        // Chuan1.08, Also disable PIP/OSD/SP
        DISP_DisplayCtrl(DISP_PIP, FALSE);
        DISP_DisplayCtrl(DISP_OSD, FALSE);
        DISP_DisplayCtrl(DISP_SP1, FALSE);
        DISP_DisplayCtrl(DISP_SP2, FALSE);

        // LLY2.36, power dwon VDEC first
        HAL_PowerControl(HAL_POWER_VDEC, HAL_POWER_SAVE);

        // LLY2.31, don't change MPLL clock during power down mode while 16M solution.
        // Since, SPI data can't be read-back while MPLL 146 --> 54MHz
#ifndef  NOT_POWERDOWN_MPLL
        // LLY2.13, remove old power down procedure, now will power down all PLL
        HAL_PowerControl(HAL_POWER_PLL, HAL_POWER_SAVE);
#endif  // #ifndef  NOT_POWERDOWN_MPLL
        // elmer2.75, power on USB
        // wyc2.78-909P, enable the usbsrc 
#ifdef SUPPORT_USB
        HAL_PowerControl(HAL_POWER_USB, HAL_POWER_SAVE);
#endif //
    }
    else  //INITIAL_SYSTEM
    {
        // LLY2.31, don't change MPLL clock during power down mode while 16M solution.
        // Since, SPI data can't be read-back while MPLL 146 --> 54MHz
        if( !(bPowerDown&HARDWARE_SHUTDOWN) )
        {
            // Wake up all function blocks
#ifndef  NOT_POWERDOWN_MPLL
            // LLY2.13, remove old wake up procedure, now will restore all PLL
            HAL_PowerControl(HAL_POWER_PLL, HAL_POWER_NORMAL);
#endif  // #ifndef NOT_POWERDOWN_MPLL

            // LLY2.36, wake up VDEC after PLL ready.
            HAL_PowerControl(HAL_POWER_VDEC, HAL_POWER_NORMAL);
        }
        // elmer2.75, power on USB
        // wyc2.78-909P, enable the usbsrc 
#ifdef SUPPORT_USB
        HAL_PowerControl(HAL_POWER_USB, HAL_POWER_NORMAL);
#endif //

        //LLY0.68, set border color
        DISP_BGColorSet(MainBGColor, DISP_MAIN_BG);
        // Based on setup info to do 1st TV encoder programming
        // LLY0.63, don't initial TVE while power on standby mode
#ifdef  POWERON_STANDBY
        if(bPowerDown & IR_POWERDOWN_MODE)
#else   // #ifdef  POWERON_STANDBY
        // LLY2.76, porting SY's code to support power switch
#ifdef SUPPORT_POWER_SWITCH
        if ((__bPowerSw_StandBy) && (bPowerDown & IR_POWERDOWN_MODE))
#endif  // #ifdef SUPPORT_POWER_SWITCH
#endif  // #ifdef  POWERON_STANDBY
        {
            HAL_TVEncoderInitial();
            // LLY2.38, turn-on CCIR656 signal while wake up
            // Since, the signal is off during power down mode to avoid noise during AV In mode
#ifdef  SUPPORT_CCIR656
            // CCIR656: Set DVI[22]=1
            // CCIR601: Set DVI[22]=1 and SyncOut[23]=1
            REG_PLAT_SYSTEM_CONFIGURATION1 |= (1<<22);
#endif  // #ifdef SUPPORT_CCIR656
        }

        // reset ADAC if necessary when H/W power on
        if( !(bPowerDown & IR_POWERDOWN_MODE) )
        {
            // LLY2.01. remove audio DAC reset procedure since nobody to implement the code
#if 0
            HAL_ResetPeripheral(HAL_RESET_AUDIODAC, TRUE);
            HAL_ResetPeripheral(HAL_RESET_AUDIODAC, FALSE);
#endif  // #if 0
        }

        // Initialize SCART function as normal mode while wake-up from power down mode
        // set as early as possble; otherwise, SCART TV will flash
        HAL_SetSCART(HAL_SCART_POWER_ON);

        // delay time for system wake
        //UTL_DelayTime ( COUNT_250_MSEC, FALSE );

        //kevin0.85, otherwise IR power on bo noise
        HAL_Reset(HAL_RESET_AUDIO);

    }
}


//**************************************************************
// Function    :   HAL_SetSCART
// Description :   Set SCART IO pins
// Arguments   :   bScartType: the desired SCART mode
// Return      :   None
//**************************************************************
// LLY2.15, using general define for SCART GPIO group
void    HAL_SetSCART(BYTE bScartType) //test OK
{
#ifdef  SUPPORT_SCART_IF

    if(__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_WIDE)
    {
        if(bScartType == HAL_SCART_POWER_ON)
            bScartType = HAL_SCART_16_9;
    }
// LLY2.30, revise the procedure for new circuit (only 3 transistors @ customer board)
// Notice: old circuit will use 4 transistors (only exit @ CTK EV board)
#ifdef  SCART_USE_4_TRANSISTORS
    switch (bScartType)
    {
        case HAL_SCART_CVBS:
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_RGB, 0);
            break ;
        case HAL_SCART_RGB:
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_RGB, 1);
            break ;
        case HAL_SCART_POWER_STANDBY:
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_SW, 0);
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_TV, 1);
            break ;
        case HAL_SCART_POWER_ON:        // 4:3
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_SW, 1);
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_TV, 0);
            break ;
        case HAL_SCART_16_9:            // 16:9
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_SW, 0);
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_TV, 0);
            break ;
    }
#else   // #ifdef  SCART_USE_4_TRANSISTORS
// LLY2.61, add the procedure that SW_SCART selection using PWM pin
#ifdef SUPPORT_SCART_PWM_CTRL
    switch (bScartType)
    {
        case HAL_SCART_CVBS:
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_RGB, 1);
            //printf("CVBS");
            break ;
        case HAL_SCART_RGB:
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_RGB, 0);
            //printf("CVBS");
            break ;
        case HAL_SCART_POWER_STANDBY:   // TV mode
            HAL_SCART_PWM_Ctrl(PIN_SCART_PWM, 0xFF);
            //printf("TV Mode");
            break ;
        case HAL_SCART_POWER_ON:        // 4:3
            HAL_SCART_PWM_Ctrl(PIN_SCART_PWM, 0x0);
            //printf("4:3");
            break ;
        case HAL_SCART_16_9:            // 16:9
            HAL_SCART_PWM_Ctrl(PIN_SCART_PWM, 0x5A);
            //printf("16:9");
            break ;
    }

#else   // #ifdef SUPPORT_SCART_PWM_CTRL
    switch (bScartType)
    {
        case HAL_SCART_CVBS:
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_RGB, 1);
            break ;
        case HAL_SCART_RGB:
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_RGB, 0);
            break ;
        case HAL_SCART_POWER_STANDBY:
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_SW, 1);
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_TV, 1);
            break ;
        case HAL_SCART_POWER_ON:        // 4:3
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_SW, 0);
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_TV, 0);
            break ;
        case HAL_SCART_16_9:            // 16:9
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_SW, 0);
	        HAL_WriteGPIO( SCART_GPIO_GRP, PIN_SCART_TV, 1);
            break ;
    }
#endif  // #ifdef SUPPORT_SCART_PWM_CTRL
#endif  // #ifdef SCART_USE_4_TRANSISTORS

#endif //#ifdef  SUPPORT_SCART_IF
}



// LLY2.01. remove this API since nobody to implement the code
#if 0
//************************************************************************
// Description  :   Reset TVE, ADAC, ...
// Arguments    :   bType: peripheral type
//                  bAction: TRUE/FALSE: reset/unreset
// Return       :   None
//************************************************************************
void    HAL_ResetPeripheral(BYTE bType, BYTE bAction)
{
    switch(bType)
    {
    case    HAL_RESET_AUDIODAC:
#ifdef SUPPORT_AUDIODAC_RESET
        for(_bHALTemp2=0; _bHALTemp2<200; _bHALTemp2++)
        {
            if(bAction)
            {
                HAL_WriteGPIO(GPIO_A, ?,1); //IO pin not reserved for 909
            }
            else
            {
                HAL_WriteGPIO(GPIO_A, ?,0);
            }
        }
#endif
        break;
    }
}
#endif  // #if 0



//*************************************************************************
//  Function    :   HAL_TVEncoderInitial
//  Description :   Initialize TV Encoder base on setup default setting
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//*************************************************************************
void    HAL_TVEncoderInitial(void) //test OK
{
    BYTE bMode;

    // Step 1: Decide __bTVType value
    if(__SetupInfo.bTVSystem==SETUP_SCREEN_TV_SYSTEM_AUTO)
    {
        // LLY2.04, default set as NTSC mode while setup setting is AUTO
        // And, don't reference __wHALVCRLow, since nobody to set it.
        __bTVType = SETUP_SCREEN_TV_SYSTEM_NTSC;
#if 0
        if(__wHALVCRLow & 0x0400) // VCR[10]=1: PAL
        {
            __bTVType=SETUP_SCREEN_TV_SYSTEM_PAL;
        }
        else                        // VCR[10]=0: NTSC
        {
            __bTVType=SETUP_SCREEN_TV_SYSTEM_NTSC;
        }
#endif  // #if 0
    }
    else
    {
        __bTVType=__SetupInfo.bTVSystem;
    }

    // Step 2: Decide __bMPEGVideoMode value based on __bTVType
    switch(__bTVType)
    {
    case    SETUP_SCREEN_TV_SYSTEM_NTSC:
    case    SETUP_SCREEN_TV_SYSTEM_PAL_M:
        __bMPEGVideoMode = MPEG_VIDEO_MODE_NTSC;
        bMode = MODE_NTSC_NORMAL;
        // LLY2.22, porting Chuan's code to initial main video H/V start
        MainVideoInfo.sHStart = VIDEO_HSTART_NTSC;
        MainVideoInfo.sVStart = VIDEO_VSTART_NTSC;
        MainVideoInfo.wVWidth = 480;
        break;
    case    SETUP_SCREEN_TV_SYSTEM_PAL:
    case    SETUP_SCREEN_TV_SYSTEM_PAL_N:
    default :
        __bMPEGVideoMode = MPEG_VIDEO_MODE_PAL;
        bMode = MODE_PAL_NORMAL;
        // LLY2.22, porting Chuan's code to initial main video H/V start
        MainVideoInfo.sHStart = VIDEO_HSTART_PAL;
        MainVideoInfo.sVStart = VIDEO_VSTART_PAL;
        MainVideoInfo.wVWidth = 576;
        break;
    }

    // Step 3: Program TV encoder
    HAL_SetTVE();

#ifdef ECOS909
    bMode = TV_OUTPUT_MODE; // Chuan0.66, Check ..
#endif

    DISP_TVModeSet(bMode, (MainVideoInfo.sHStart + 22), MainVideoInfo.sVStart);

#ifdef SUPPORT_TFT_DIGITAL_MODE
    __bTFTMode = MODE_TFT_ANALOG;
    __bTVType = 0; //otherwise, hw power up if(bNewTarget!=__bTVType) will not hold in HAL_AdjustTVMode

    __bTFTOutput = TRUE;
#endif
    // wyc1.07, need to call this function to disable OSD, otherwise OSD will scramble after POWER on from power standy mode.
    DISP_DisplayCtrl(DISP_OSD, FALSE);

    //KC1.07, avoid HW/IR power on, logo flashes (it takes time for TV to sync w/ the TVE output signal)
    OS_DelayTime(COUNT_300_MSEC);
}


// LLY2.17 remove from HDEVICE module
//  ************************************************************************************
//  Function    :   HAL_ReadStorage
//  Description :   Readback information from used storage
//  Arguments   :   wAddr, specify the desired address
//                  pbVal, keep the readback data
//                  wCnt, specify the desired readback data number (BYTE unit)
//  Return      :   None
//  ************************************************************************************
// LLY2.53, modify the parameter unit to support > 256 byte setup info
//void    HAL_ReadStorage(BYTE bAddr, BYTE* pbVal, BYTE bCnt)
void    HAL_ReadStorage(WORD wAddr, BYTE* pbVal, WORD wCnt)
{
#if (STORAGE_TYPE == SERIAL_FLASH)
    HAL_ReadSerialFlash(wAddr, pbVal, wCnt);
#endif  // #if (STORAGE_TYPE == SERIAL_FLASH)
#if (STORAGE_TYPE == EEPROM)
    if(wAddr>256)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "not support > 256 byte\n");
        return;
    }
    HAL_ReadEEPROM(wAddr, pbVal, wCnt);
#endif  // #if (STORAGE_TYPE == EEPROM)
}


// LLY2.17 remove from HDEVICE module
//  ************************************************************************************
//  Function    :   HAL_WriteStorage
//  Description :   Write information into used storage
//  Arguments   :   wAddr, specify the desired address
//                  pbVal, keep the desired data for writting
//                  wCnt, specify the desired writting data number (BYTE unit)
//  Return      :   None
//  ************************************************************************************
// LLY2.53, modify the parameter unit to support > 256 byte setup info
//void    HAL_WriteStorage(BYTE bAddr, BYTE* pbVal, BYTE bCnt)
void    HAL_WriteStorage(WORD wAddr, BYTE* pbVal, WORD wCnt)
{
#if (STORAGE_TYPE == SERIAL_FLASH)
    // LLY2.51, porting Wing's code since API name is updated.
    HAL_WriteSPF_BANK(wAddr, pbVal, wCnt);
#endif  // #if (STORAGE_TYPE == SERIAL_FLASH)
#if (STORAGE_TYPE == EEPROM)
    if(wAddr>256)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "not support > 256 byte\n");
        return;
    }

    HAL_WriteEEPROM(wAddr, pbVal, wCnt);
#endif  // #if (STORAGE_TYPE == EEPROM)

}


//************************************************************************
// Description  :   Adjust the brightness of video output
// Arguments    :   bMode: brightness mode
// Return       :   None
// Side Effect  :   None
//************************************************************************
// LLY1.50, modify the procedure since DISP_AdjustBright() meaning is updated.
void    HAL_AdjustBrightness(BYTE bMode)
{
    switch(bMode)
    {
    case    HAL_BRIGHTNESS_NORMAL:
//        DISP_AdjustBright( 0, 1);
        DISP_AdjustBright(0);
        break;
    case    HAL_BRIGHTNESS_BRIGHT:
//        DISP_AdjustBright( 16, 0);
        DISP_AdjustBright(16);
        break;
    case    HAL_BRIGHTNESS_SOFT:
//        DISP_AdjustBright( 16, 1);
        DISP_AdjustBright(-16);
        break;
    }
}



//************************************************************************
// Description  :   Adjust the contrast of video output
// Argument     :   bLevel: contrast level
// Return       :   None
// Side Effect  :   None
//************************************************************************
// LLY1.50, modify the procedure since DISP_AdjustContrast() meaning is updated.
void    HAL_AdjustContrast(BYTE bLevel) //test OK
{
    if (bLevel>HAL_CONTRAST_LEVEL4)
    {
//        DISP_AdjustContrast( 1<<(bLevel-HAL_CONTRAST_LEVEL4), 0);
        DISP_AdjustContrast( 1<<(bLevel-HAL_CONTRAST_LEVEL4));
    }
    else
    {
//        DISP_AdjustContrast( 1<<(HAL_CONTRAST_LEVEL4-bLevel), 1);
        DISP_AdjustContrast( (1<<(HAL_CONTRAST_LEVEL4-bLevel)) * (-1) );
    }
}

// LLY2.00, collect the procedure of waiting decoder stop command ACK
//  **************************************************************************
//  Function    :   _Wait_Decoder_Stop_CMD_ACK
//  Description :   Wait decoder stop command ACK
//  Arguments   :   None
//  Return      :   TRUE, ACK ok
//                  FALSE, ACK fail
//  Notice      :   Only used for decoder stop command ACK
//  **************************************************************************
BYTE    _Wait_Decoder_Stop_CMD_ACK(void)
{
    DWORD   dwTime, dwTemp;

    //wait for ACK
    dwTime = OS_GetSysTimer();
    // Chuan2.17, enlarge the waiting time from 100 --> 300 msec
    while( (OS_GetSysTimer()-dwTime) < COUNT_300_MSEC )
    {
        HAL_ReadInfo(HAL_INFO_DECCMD_ACK, &dwTemp);
        if (dwTemp == MODE_STOPPED)
        {
            break;
        }
        //release CPU resource to other threads
        OS_YieldThread();
    }
    if(dwTemp != MODE_STOPPED)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, " MODE_STOP fail");
        return FALSE;
    }

    return TRUE;

}


// LLY2.00, modify the procedure to merge the same control flow.
//  **************************************************************************************************************
//  Function    :   HAL_Reset
//  Description :   Reset the desired function block and caller must issue start command by itself.
//  Arguments   :   bType, the desired function block
//  Return      :   none
//  Side Effect :   [1] Decoder will stop after reset, caller must issue desired cmd by itself
//  Notice      :   [2] It's different between HAL_RESET_AVSPST and HAL_RESET_AUDIO + VIDEO + SPST
//                      Since, A/V/SPST will be release together after all thing done for HAL_RESET_AVSPST
//                      But, A/V/SPST will be release one by one after it done for HAL_RESET_XXX individual case
//  *************************************************************************************************************
void    HAL_Reset(BYTE bType)
{
    DWORD dwCurrentFrame;

    // Step 1: prepare stage before issue reset procedure, ex. stop decoder, parser, servo, or ...
    switch(bType)
    {
#ifndef NO_PROC2
    case    HAL_RESET_AUDIO:
#ifdef  SUPPORT_PRINTF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n# HAL_RESET_AUDIO!");
#endif  // #ifdef SUPPORT_PRINTF
        // issue stop command
        HAL_WriteAM(HAL_AM_PLAY_COMMAND, 0);

        // notify parser to throw audio bitstream.
        PARSER_ActionFilter( 0, PARSER_ENABLE_AUDIO, FALSE );
	    //release CPU resource to other threads
        OS_YieldThread();
        break;
#endif  // #ifndef NO_PROC2

    case    HAL_RESET_VIDEO:
#ifdef  SUPPORT_PRINTF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n# HAL_RESET_VIDEO!");
#endif
        break;

    case    HAL_RESET_SPST:
#ifdef  SUPPORT_PRINTF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n# HAL_RESET_SPST");
#endif
        break;

    case    HAL_RESET_AVSPST:
#ifdef  SUPPORT_PRINTF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\n# HAL_RESET_AVSPST");
#endif
        // 1. Stop decoder and proc2.
        HAL_PlayCommand(COMMAND_STOP, 0);

        // 2. Stop Source filter.
        SrcFilter_Stop(__dwSFStreamID);

        // 3. Stop Parser.
        HAL_ControlParser(HAL_PARSER_STOP, 0);
        break;
    }
    // Micky2.11a
    if(bType==HAL_RESET_AUDIO || bType==HAL_RESET_AVSPST)
    {
        HAL_ResetAudioDecoder();
        // Micky2.14, below are moved out from HAL_ResetAudioDecoder
        // LLY1.00, set audio sample frequency & channel no for DivX w/ PCM audio stream
        // Notice: [1] Must read-back the value first, and only modify [3:0] & [19:16]
        //         [2] Must check where to issue the proceudure because the value must be set
        //             during DSP reset ~ issue play command.
        //         [3] The value will be clear after DSP reset. So, F/W must set it again.
        if( (__bAttrPlay==ATTR_AVI) && (__bAudioType == HAL_AUDIO_PCM) )
        {
            HAL_SetAudioFrequency(__dwHALAudioFreq);

            // LLY2.55, porting Grievous's code to set audio channel information by write AM directly
            //HAL_SetAudioChannel(__bACHN);
            HAL_WriteAM(HAL_AM_PCM_CH_NUM, __bACHN);

            // Grievous2.60, config bits per sample base on parser given data.
            if(__bBitsPerSample==8)
            {
	            HAL_WriteAM(HAL_AM_PCM_BTYE_PER_SAMPLE, 1); // 8 bits per sample
	        }
	        else
	        {
	            HAL_WriteAM(HAL_AM_PCM_BTYE_PER_SAMPLE, 0); // 16 bits per sample
	        }
        }

        // LLY2.77, disable mechanism while playback AVI: audio STC - STC > 2 sec, audio buffer will be reset
        // Notice: the value will be reset after audio decoder
        if(__bAttrPlay == ATTR_AVI)
        {
            HAL_ControlAVSync(HAL_AVSYNC_2SEC_LIMIT, FALSE);
        }

    }

    // [2-2] Reset Video part
    if(bType==HAL_RESET_VIDEO || bType==HAL_RESET_AVSPST)
    {
        //kevin1.02, avoid thread related potential pitfall (JPEG->MP3 => __bAttrPlay!= MP3, but thread is still in JPEG)
        //if(__bAttrPlay != ATTR_JPG)
        if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_JPEG_DONE) ) //currently not in JPEG thread
        {
            //fix video flash
//micky0124, HAL_Display(HAL_DISPLAY_ONE_FIELD, 0);
            // must transfer the field control back to display.(in case of someone issue force one field, but doesn't clear it)
            DISP_ForceOneField(FALSE);

            // 1. notify decoder to stop decoding
    		//takes effect at next picture
            COMDEC_Command(MODE_STOP, 0);

            // 2. wait for ACK
            // LLY2.00, collect the waiting ack procedure into a API
            _Wait_Decoder_Stop_CMD_ACK();

            // 1. notify parser to throw bitstream.
            PARSER_ActionFilter( 0, PARSER_ENABLE_VIDEO, FALSE );
            //release CPU resource to other threads
            OS_YieldThread();

            //fix the repeatAB, picture size error
            COMDEC_Command(MODE_UPDATE_SEQ, 0);

            // force to skip B-frame until 2 I/P picture are decoded
            COMDEC_Command(MODE_FORCE_IP, 0);

            // wyc0.85a, don't clear buffer when JPG -> MP3 and CDROM mode.
            if (!(__wDiscType & CDROM_M1) || (__dwClearBuffMode == HAL_CLEAR_SCREEN_MPEG))
            {
                HAL_ReadInfo(HAL_INFO_DISPFRAME_INDEX, &dwCurrentFrame);
                if (dwCurrentFrame == DISP_FRAME0)
                {
                    HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_FRAME1);
                    HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_FRAME2);
                }
                else if (dwCurrentFrame == DISP_FRAME1)
                {
                    // LLY2.21, it's un-necessary to clear frame buffer 0
                    // Since, it use for B-frame, and MPEG decoder will not reference it for next frame.
                    //HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_FRAME0);
                    HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_FRAME2);
                }
                else
                {
                    // LLY2.21, it's un-necessary to clear frame buffer 0
                    // Since, it use for B-frame, and MPEG decoder will not reference it for next frame.
                    //HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_FRAME0);
                    HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_FRAME1);
                }
            }
        }
        else
        {
            // 1. notify parser to throw bitstream.
            PARSER_ActionFilter( 0, PARSER_ENABLE_VIDEO, FALSE );
            //release CPU resource to other threads
            OS_YieldThread();

            HALJPEG_Abort();
        }

        // 3. notify parser to clear write ptr.
        PARSER_BufferReset( PARSER_ES_VIDEO );

    }

    // [2-3] Reset SPST part
    if(bType==HAL_RESET_SPST || bType==HAL_RESET_AVSPST)
    {
        DISP_DisplayCtrl(DISP_SP1, FALSE);
        DISP_DisplayCtrl(DISP_SP2, FALSE);
        SPU_Reset(__dwSPType);
        // Micky0.68, reset SP buffer
        PARSER_BufferReset( PARSER_ES_SP1 );
        PARSER_BufferReset( PARSER_ES_SP2 );

//Alan1.11, for char-based subpicture
#ifdef  SUPPORT_CHAR_SUBPICTURE
        CHAR_SP_Reset();
#endif  // #ifdef SUPPORT_CHAR_SUBPICTURE
    }


    // Step 3: Restore to normal state.
    // [3-1] Recover audio part
#ifndef NO_PROC2
    if(bType==HAL_RESET_AUDIO || bType==HAL_RESET_AVSPST)
    {
        if (__wHALAVIDRHigh & 0x0008) //kevin0.70, bit[3]
        {
            // 6. notify parser not to throw bitstream.
            PARSER_ActionFilter( 0, PARSER_ENABLE_AUDIO, TRUE );
        }
    }
#endif  // #ifndef NO_PROC2

    // [3-2] Recover video part
    if(bType==HAL_RESET_VIDEO || bType==HAL_RESET_AVSPST)
    {
        if (__wHALAVIDRHigh & 0x0004) //kevin0.70, bit[2]
        {
            // 6. notify parser not to throw bitstream.
            PARSER_ActionFilter( 0, PARSER_ENABLE_VIDEO, TRUE );
        }
    }

    // [3-3] Recover SPST part
    if(bType==HAL_RESET_SPST || bType==HAL_RESET_AVSPST)
    {
        // Only turn on sp while nobody set sp off -- LLY0.81
        // Otherwise, SP will appear after call HAL_RESET_SPST
        // __bSP_Disp_OFF: 1 ~ 127 means sp off
        //                 0, 128 ~ 255 means sp on
        if( (__bSP_Disp_OFF==0) || (__bSP_Disp_OFF&0x80) )
        {
            DISP_DisplayCtrl(DISP_SP1, TRUE);
        }
        //kevin0.91
        if( (__bSP2_Disp_OFF==0) || (__bSP2_Disp_OFF&0x80) )
        {
            DISP_DisplayCtrl(DISP_SP2, TRUE);
        }
    }
}



//************************************************************************
// Description  :   Issue Playable Command to Video/Audio decoders
// Argument     :   bCmdID: command ID
//                  bParam: slow factor 1~5 for COMMAND_SLOW; otherwise, NULL
// Return       :   None
// Side Effect  :   None
//************************************************************************
void    HAL_PlayCommand(BYTE bCmdID, BYTE bParam) //test OK
{
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    if (__bMMJPEGAudioMode)
    {
        if (bCmdID == COMMAND_STOP)
            return;
    }
    else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
    {
        // LLY0.66, don't issue MPEG decoder relative command while playback JPEG
        if(__bAttrPlay == ATTR_JPG)
            return;
    }

    switch (bCmdID)
    {
        case COMMAND_PLAY:
            COMDEC_Command(MODE_PLAY, 0);
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);
            CHIPS_OpenAudio(TRUE); //kevin1.20, fix command switch bo noise
            break;

        case COMMAND_FREEZE:
            CHIPS_OpenAudio(FALSE); //kevin1.20, fix command switch bo noise
            COMDEC_Command(MODE_FREEZE, 0);
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);
            break;

        case COMMAND_PAUSE:
            CHIPS_OpenAudio(FALSE); //kevin1.20, fix command switch bo noise
            COMDEC_Command(MODE_PAUSE, 0);
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 6);
            break;

        case COMMAND_STOP:
            CHIPS_OpenAudio(FALSE); //kevin1.20, fix command switch bo noise

            COMDEC_Command(MODE_STOP, 0);
            // LLY2.00, collect the waiting ack procedure into a API
            _Wait_Decoder_Stop_CMD_ACK();

            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 0);
            break;

        case COMMAND_STEP:
            CHIPS_OpenAudio(FALSE); //kevin1.20, fix command switch bo noise
            COMDEC_Command(MODE_STEP, 0);
            //upper layer has to issue "skip the corresponding audio frames"
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 5); //kevin0.69, Audio will throw data(m/n) by decoder.
            break;

        case COMMAND_FAST:
            CHIPS_OpenAudio(FALSE); //kevin1.20, fix command switch bo noise
            COMDEC_Command(MODE_FF, 0); //IP, switch DISP by Decoder
            //upper layer has to issue "skip the corresponding audio frames"
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 5); //kevin0.69, Audio will throw data(m/n) by decoder.
            break;

        case COMMAND_SLOW:
            CHIPS_OpenAudio(FALSE); //kevin1.20, fix command switch bo noise
            bParam = bParam * 2;
            COMDEC_Command(MODE_SF, bParam); //IP, switch DISP by Decoder
            //upper layer has to issue "skip the corresponding audio frames"
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 5); //kevin0.69, Audio will throw data(m/n) by decoder.
            break;

        case COMMAND_SCAN:
            CHIPS_OpenAudio(FALSE); //kevin1.20, fix command switch bo noise
            COMDEC_Command(MODE_SCAN, 0); //IP, switch DISP by Decoder
            //upper layer has to issue "throw audio"
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 0);
            //wait if (__dwPlayMode == MODE_SCAN_DONE) by upper layer
            break;

        case COMMAND_DIGEST: //behavior???
#ifdef NO_PIP
            COMDEC_Command(MODE_SCAN, 1); //I, switch DISP by Digest
            //upper layer has to issue "?"
            HAL_ControlParser(HAL_PARSER_AUDIO_ID, HAL_THROW_AUDIO);
            HAL_Reset(HAL_RESET_AUDIO);
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 0);   //Alan1.10 for digest audio
#else
            // Chuan0.70, Issue FF_IP CMD and stop Audio.
            COMDEC_Command(MODE_FF, 1); //IP, switch DISP by Digest
            //upper layer has to issue "?"
            HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);   //Alan1.10 for digest audio
            CHIPS_OpenAudio(TRUE); //kevin1.20, fix command switch bo noise
#endif
            break;
//Micky2.04, add more interfaces
        case    COMMAND_V_UPDATE_SEQ:
            COMDEC_Command(MODE_UPDATE_SEQ, 0);
            break;
        case    COMMAND_V_PREDECODE:
            COMDEC_Command(MODE_PREDECODE, bParam);
            break;
        case    COMMAND_V_CLEAR_STILL:
            COMDEC_Command(MODE_CLEAR_STILL, 0);
            break;
        case    COMMAND_V_RELEASE_MODE:
            COMDEC_Command(MODE_RELEASE_MODE, bParam);
            break;
        case    COMMAND_V_ERROR_RATIO:
            COMDEC_Command(MODE_ERROR_RATIO, bParam);
            break;

    }
}


// wyc1.20-909, change bID from BYTE to DWORD because PARSER_ChangeID use DWORD as parameter.
//  *****************************************************************************************
//  Function    :   HAL_ControlParser
//  Description :   Use to control parser as desired action
//  Arguments   :   bPackType, specify the desired parser action
//                  bID, specify the desired ID
//  Return      :   TRUE, the action is success
//                  FALSE, the action is fail
//  *****************************************************************************************
// LLY2.36, add return value to tell caller the desired action is ok or fail
//void    HAL_ControlParser(BYTE bPacketType, DWORD bID)
BYTE    HAL_ControlParser(BYTE bPacketType, DWORD bID)
{
    DWORD dwTimer, dwTemp=0;
    switch(bPacketType)
    {
        case    HAL_PARSER_STOP:
            PARSER_Command(0, PARSER_CMD_STOP, 0);
            dwTimer=OS_GetSysTimer();
            while( (OS_GetSysTimer() - dwTimer) < COUNT_50_MSEC)
            {
                dwTemp=PARSER_QueryEvent(0, PARSER_EVENT_COMPLETED, TRUE);//FALSE);

                // Parser stop command ok
                if(dwTemp)
                {
                    break;
                }

                // release the CPU
                OS_YieldThread();
            }

            // Parser stop command fail
            if(!dwTemp)
            {
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Parser stop fail\n");
                return FALSE;
            }
            return TRUE;

        case    HAL_PARSER_VIDEO_ID:
            if(bID == HAL_THROW_VIDEO)
            {
                __wHALAVIDRHigh &= ~0x0004; //kevin0.70, bit[2]
                PARSER_ActionFilter( 0, PARSER_ENABLE_VIDEO, FALSE );
            }
            else
            {
                // LLY0.72, don't specify packet ID for AVI/ DivX file.
                if(__bAttrPlay == ATTR_AVI)
                {
                    PARSER_ChangeID(0, PARSER_ES_VIDEO, bID);
                }
                else
                {
                    // LLY2.05, combine packet ID + index for parser directly.
                    // ie. it's un-necessary to assign value to __wHALAVIDRLow
#if 0
                	// VIDEO_ID bit(7-0)
                	__wHALAVIDRLow &= 0xff00;
                	__wHALAVIDRLow |= (0xE0 | bID);
                	PARSER_ChangeID( 0, PARSER_ES_VIDEO, (__wHALAVIDRLow&0x00FF) );
#endif  // #if 0
                    PARSER_ChangeID( 0, PARSER_ES_VIDEO, (0xE0 | bID) );
                }

                // enable video stream ID filter and video stream parse
                __wHALAVIDRHigh |= 0x0004; //kevin0.70, bit[2]

                PARSER_ActionFilter( 0, PARSER_ENABLE_VIDEO, TRUE );
            }
            break;

        case    HAL_PARSER_AUDIO_ID:
#ifdef NO_PROC2
            if(1)//bID == HAL_THROW_AUDIO)
#else
            if(bID == HAL_THROW_AUDIO)
#endif
            {
                __wHALAVIDRHigh &= ~0x0008; //kevin0.70, bit[3]
                PARSER_ActionFilter( 0, PARSER_ENABLE_AUDIO, FALSE );
            }
            else
            {
				 // LLY0.72, don't specify packet ID for AVI/ DivX file.
                // Notice: Need to check how to do DSP reset for multi-audio switching
                if(__bAttrPlay == ATTR_AVI)
                {
                    PARSER_ChangeID( 0, PARSER_ES_AUDIO, bID );
                }
                else
                {
                	//kevin0.70-2,
                	WORD wTemp;

                    // LLY.047-3, fix MPEG multi-audio switch noise
                    // and only reset audio while different audio ID
	                //Kevin1.24, fix DVD-DTS pause->play audio discont. (due to audio is reset erroneously)
    	            //if( (__wW99AVAVIDRLow&0x1F00) != (((WORD)bID)<<8) )
        	        // for AC3/ DTS, bits 12~14 is 0.
            	    if(__wHALAVIDRLow&0x7000)  // non-AC3 and non-DTS
                	    wTemp=0x1F00;  //ID field: bit[12:8]
	                else
    	                wTemp=0x0700;   //ID field: bit[10:8]

        	        if( (__wHALAVIDRLow&wTemp) != (((WORD)bID)<<8) )
        	        {
            	        HAL_Reset(HAL_RESET_AUDIO);
        	        }

	                // LLY.048-2,
    	            // if AUDIO type is AC3 or DTS, only clear bit[10:8]
        	        // otherwise, clear bit[12:8]
            	    // DVD039, AUDIO_ID bit (15-8): [15:13], specify audio type & [12:8], audio ID
	                // But AC3 and DTS share same audio stream package ID: 0x8?
    	            // AC3: 0x80 ~ 0x87; DTS: 0x88 ~ 0x8F
        	        if(__wHALAVIDRLow&0x7000)  // non-AC3 and non-DTS
            	        __wHALAVIDRLow &= 0xE0FF;  // clear bit[12:8]
	                else    // AC3 or DTS type
    	                __wHALAVIDRLow &= 0xF8FF;  // only clear bit[10:8] and keep bit[15:11]
        	        // assign desired audio ID
            	    __wHALAVIDRLow |= ((WORD)bID) << 8;

    	            PARSER_ChangeID( 0, PARSER_ES_AUDIO, (__wHALAVIDRLow>>8) );
				}

                //enable audio stream ID filter and audio stream parse
	            __wHALAVIDRHigh |= 0x0008; //kevin0.70, bit[3]

                PARSER_ActionFilter( 0, PARSER_ENABLE_AUDIO, TRUE );
            }
            break;

        case    HAL_PARSER_VIDEO_ONLY:
            __wHALAVIDRHigh &= ~0x0008; //kevin0.70, bit[3]
            __wHALAVIDRHigh |= 0x0004; //kevin0.70, bit[2]
            PARSER_ActionFilter( 0, PARSER_ENABLE_VIDEO, TRUE );
            PARSER_ActionFilter( 0, PARSER_ENABLE_AUDIO, FALSE );
            break;

        case    HAL_PARSER_AUDIO_ONLY:
            __wHALAVIDRHigh &= ~0x0004; //kevin0.70, bit[2]
            __wHALAVIDRHigh |= 0x0008; //kevin0.70, bit[3]
            PARSER_ActionFilter( 0, PARSER_ENABLE_VIDEO, FALSE );
            PARSER_ActionFilter( 0, PARSER_ENABLE_AUDIO, TRUE );
            break;

        case     HAL_PARSER_CDROM:
            PARSER_ActionFilter( 0, PARSER_ENABLE_DATA, TRUE );
            break;

        case    HAL_PARSER_SP_ID: // SP for DVD, and OGT for CVD/SVCD
            // LLY1.11, parser support CVD OGT from v1.10
			/*
            if(__wDiscType & BOOK_CVD)
            {
                PARSER_ActionFilter( 0, PARSER_ENABLE_SP1, FALSE );
                break;
            }
            */
            PARSER_ChangeID( 0, PARSER_ES_SP1, bID );
            //Parser does necessary stuff
            PARSER_ActionFilter( 0, PARSER_ENABLE_SP1, TRUE );

            // LLY2.36, need to SPU initial otherwise SP display maybe abnormal
            // while internal and external SP switching.
            SPU_Reset(__dwSPType);

            // LLY2.13, for char-based subpicture initial SPU procedure
            // Since, SPU_Reset() will always reset SP type as DVD mode
#ifdef  SUPPORT_CHAR_SUBPICTURE
            CHAR_SP_Reset();
#endif  // #ifdef SUPPORT_CHAR_SUBPICTURE
            break;

        // Chuan0.75, Add SP2 Case
        case    HAL_PARSER_SP2_ID:
            PARSER_ChangeID( 0, PARSER_ES_SP2, bID );
            //Parser does necessary stuff
            PARSER_ActionFilter( 0, PARSER_ENABLE_SP2, TRUE );
            break;

        case    HAL_PARSER_NV_ID:
            // LLY0.66, base on input argument to enable or disable NV parsing.
            PARSER_ActionFilter( 0, PARSER_ENABLE_NV, bID );
            break;

        // LLY2.10, remove HAL_PARSER_DISABLE/ HAL_PARSER_RESET since nobody use it.
#if 0
        case    HAL_PARSER_DISABLE:
            PARSER_ActionFilter( 0, PARSER_ENABLE_ALL, FALSE );
            break;
#endif  // #if 0

        case    HAL_PARSER_ENABLE:
            break;

#if 0
        case    HAL_PARSER_RESET:
            PARSER_ActionFilter( 0, PARSER_ENABLE_ALL, FALSE );
            break;

        case    HAL_PARSER_JPEG_DECODE:
            break;
#endif  // #if 0
    }

    return TRUE;
}


void    HAL_Display(BYTE bDispType, BYTE bDispAction)
{
    switch(bDispType)
    {
    case    HAL_DISPLAY_SPST_DEC: // subpicture or subtitle
        DISP_DisplayCtrl(DISP_SPDEC, bDispAction);
        break;

    case    HAL_DISPLAY_SPST1: // subpicture or subtitle
        DISP_DisplayCtrl(DISP_SP1, bDispAction);
        break;

    // Chuan0.75, Add SP2 Case
    case    HAL_DISPLAY_SPST2:
        DISP_DisplayCtrl(DISP_SP2, bDispAction);
        break;

    //case HAL_DISPLAY_JPEG:
    //    break;
    }
}



//**************************************************************************
// Description  :   Configure the necessary setting for different title
// Arguments    :   wDiscType, the specified disc type
// Return       :   None
// Notice       :   The function will be done after title detection done
//**************************************************************************
void    HAL_SetDiscType(WORD wDiscType)
{
    // LLY0.66, Enable Parser NV packet control for DVD title
    // And disable it for other title
    // Also set SP and NV buffer for DVD title
    if(__wDiscType & BOOK_DVD)
    {
        HAL_ControlParser(HAL_PARSER_NV_ID, TRUE);

        // Set NV buffer for parser
        PARSER_SetOutBufferAddress(PARSER_ES_NV, DS_NV_BUF_ADDR_ST, DS_NV_BUF_ADDR_END);
    }
    else
    {
        HAL_ControlParser(HAL_PARSER_NV_ID, FALSE);
    }

    // Set SP1/ SP2 buffer address.
    PARSER_SetOutBufferAddress(PARSER_ES_SP1, DS_SP1BUF_ADDR_ST, DS_SP1BUF_ADDR_END);
    PARSER_SetOutBufferAddress(PARSER_ES_SP2, DS_SP2BUF_ADDR_ST, DS_SP2BUF_ADDR_END);

    // LLY2.01, don't configure parser/ servo dump in buffer here.
    // Now, it will be set during power-on/ open_close/ title detection
#if 0
    // Set parser and servo dump-in buffer
    SrcFilter_ConfigBuffer( 0, DS_INPUT_BITBUF_ST,
        ((DS_INPUT_BITBUF_END-DS_INPUT_BITBUF_ST)/SERVO_BUFSIZE_ALIGN_UNIT) );
#endif  // #if 0
}


//*********************************************************************
// Description  :   Control A/V sync
// Arguments    :   bType: sync type
//                  dwParam: STC parameter or V/A master mode
// Return       :   None
//********************************************************************
void    HAL_ControlAVSync(BYTE bType, DWORD dwParam)
{
    // LLY2.05, remove HAL_AVSYNC_STEP since decoder will hold STC when pause/ step from CT909

    switch(bType)
    {
    case    HAL_AVSYNC_RESYNC:
        // DVD_278Micky, fix pause->play video will shake(N->N)[Helbon's DVD title]
        // as the STC is reset to a smaller value, cause VOBU_E_PTM control is active.
        // the caller must give correct STC value.
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_AVSYNC, "\n# HAL_AVSYNC_RESYNC !");
        // Chuan1.05, Call function to update STC.
        // REG_PLAT_TIMER3_VALUE = dwParam;
        DISP_UpdateSTC( dwParam, FALSE);

        // Resync Video PTS.
        __dwDISP_PTS = dwParam;
        break;

    case    HAL_AVSYNC_RESETSYNC: //may not need in 909
        // LLY.160, swap the sequence, ie. let "Reset STC" action in the last time
        // otherwise, last coming PTS maybe update the STC
        // during those two action period
        // LLY.105, reset STC and clear audio PTS to reset A/V sync. control
        // to fix highlight disappear while repeat DVD-Menu for "Bone Collector"
        //  [1] Clear audio PTS only while non-SPDIF mode
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_AVSYNC, "\n#HAL_AVSYNC_RESETSYNC !");

        if(__SetupInfo.bAudioDigitalOut !=SETUP_AUDIO_DIGITAL_OUT_SPDIF_RAW&&
        __SetupInfo.bAudioDigitalOut !=SETUP_AUDIO_DIGITAL_OUT_SPDIF_PCM)
        {
            if(__bAudioType==HAL_AUDIO_AC3 || __bAudioType==HAL_AUDIO_MPG || __bAudioType==HAL_AUDIO_LPCM )
            {
                HAL_WriteAM(HAL_AM_RESET_PTS, 1);
            }
        }
        // [2] Reset STC
        // Chuan1.05, Call function to update STC.
        // REG_PLAT_TIMER3_VALUE = dwParam;
        DISP_UpdateSTC( dwParam, FALSE);

        // Reset Video PTS.
        __dwDISP_PTS = dwParam;
        break;

    case    HAL_AVSYNC_VIDEOMASTER:
        //kevin0.80-2, fix CD-DTS video master => audio hangs
        // Chuan0.80a, DivX must do Audio Master.
        // LLY1.01, don't do video master while video stream has been skip.
        if ( (dwParam) &&
             (!(__bAttrPlay & TYPE_CDROM_AUDIO)) &&
             (!(__bAttrPlay==ATTR_CDDA)) &&
             ( (__bVideoID!=HAL_THROW_VIDEO)) )
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_AVSYNC, "\n#HAL_AVSYNC_VIDEOMASTER !");

            // LLY2.56, porting Chuan's code to give a offset value for DVD title
            // while control Video master or Audio master period
            // Otherwise, the audio outptu is easy break from SPDIF-RAW (ex. Panasonic title, "流言")
            if (__wDiscType & BOOK_DVD)
            {
                DISP_VideoMaster(TRUE, 9000); // 9000 means 100msec
            }
            else
            {
                DISP_VideoMaster(TRUE, 0);
            }
            HAL_WriteAM( HAL_AM_VIDEO_UPDATED_STC, 0);
            HAL_WriteAM( HAL_AM_VIDEO_MASTER, 1);

            if (__bAttrPlay == ATTR_AVI)
                HAL_WriteAM( HAL_AM_PROC2_WAIT_STC, 1); //otherwise, audio updates STC immediately, and video will skip
            else
                HAL_WriteAM( HAL_AM_PROC2_WAIT_STC, 0);
        }
        else //HAL_AVSYNC_AUDIOMASTER
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_AVSYNC, "\n#HAL_AVSYNC_AUDIOMASTER !");

            // LLY2.56, porting Chuan's code to give a offset value for DVD title
            // while control Video master or Audio master period
            // Otherwise, the audio outptu is easy break from SPDIF-RAW (ex. Panasonic title, "流言")
            if (__wDiscType & BOOK_DVD)
            {
                DISP_VideoMaster(FALSE, 9000);  // 9000 means 100msec
            }
            else
            {
                DISP_VideoMaster(FALSE, 0);
            }

            // Chuan0.70, HAL_AM_VIDEO_UPDATED_STC is no use now.
            // HAL_WriteAM( HAL_AM_VIDEO_UPDATED_STC, 1);
            HAL_WriteAM( HAL_AM_VIDEO_MASTER, 0);
        }
        // Chuan1.00, Always report Audio STC updated at first time.
        HAL_WriteAM( HAL_AM_RESET_PTS, 1);
        break;

    // LLY2.77, add new case to disable the mechanism: audio STC - STC > 2sec,
    // then audio buffer will be reset when play command
    case    HAL_AVSYNC_2SEC_LIMIT:
        // Grievous2.77, fix A/V sync control for audio STC is great than STC 2 second
        // will clean audio buffer while playing .avi file.
        HAL_WriteAM(HAL_AM_AV_SYNC_2SEC_CONDITION,0);
        break;

    default:
        break;
    }
}



//************************************************************************
// Description  :   Issue some action once at initial time
//                  1.Issue total S/W reset
//                  2.Download micro-code and initialize chip
//                  3.Initial Video
//                  4.Inital Audio
// Arguments    :   None
// Return       :   None
//************************************************************************
void     HAL_PowerOnInit(void)
{
    // Step 9 :     Total s/w reset
    // wyc1.24a, code to decide doing W99AV_RESET_TOTAL or not. Add it is because now we will move display LOGO code to the place that before
    // W99AV_PowerOnInitial, so not need to H/W reset to avoid video become black.
  	HAL_Reset(HAL_RESET_TOTAL); // ?? remvoe it since nobody implement it ??

    // Step 10 : Initial Video and Audio
    HAL_InitVideo();

    HAL_InitAudio();
    // Micky2.14, move out from HAL_InitAudio
    // Initial Echo Value, don't call CHIPS_Echo(CHIPS_ECHO) since following problem
    // kevin1.00, fix IR Power off->on => Echo OSD is shown
    // CHIPS_Echo(CHIPS_NORMAL);
    // So, initial __bEchoLevel as CHIPS_DEFAULT_ECHO
    // And, set ECHO_CR as 0x808 (same as CHIPS_Echo(CHIPS_NORMAL)
    {
    extern char    __bEchoLevel;
    __bEchoLevel = CHIPS_DEFAULT_ECHO ;
    HAL_WriteAM(HAL_AM_ECHO_CR, 0x808);
    }

}


//W99AV_Initial()+HAL_Variables_PowerON()
//all global variables are initialized when declared
void    HAL_InitVariables(void)
{

    __dwClearBuffMode = HAL_CLEAR_SCREEN_MPEG;
    // Mick2.11a, move to HAL_InitPlatForm
    /*
    OS_DISABLE_INTERRUPTS( dwSaveInt );

    MACRO_PLAT_KEY_LOCK( );
    //kevin0.91, set REG_PLAT_SYSTEM_CONFIGURATION1 in one place
    //setup GPIO->PCMSD
    REG_PLAT_SYSTEM_CONFIGURATION1 |= 0xf<<24;
    //disable PD, TVE=normal mode, disable external ADC, PCMSD3->GPA31
    REG_PLAT_SYSTEM_CONFIGURATION1 &= ~0x88470000L;

    //kevin909, test test CCIR-601, set DVI[22]=1 SyncOut[23]=1
    //REG_PLAT_SYSTEM_CONFIGURATION1 |= 0x00C00000L;

    MACRO_PLAT_KEY_UNLOCK( );
    OS_RESTORE_INTERRUPTS( dwSaveInt );
    */
}
void    HAL_ControlHighlight(BYTE bAction, BYTE bButton)
{
    if (bAction  == HAL_HLI_ACTION_TURNOFF)
    {
        // Micky2.11a
        SPU_ControlHighlight(HLI_ACTION_OFF, (BYTE)NULL);
    }
    else if (bAction  == HAL_HLI_ACTION_SELECT)
    {
        // Micky2.11a
        SPU_ControlHighlight(HLI_ACTION_SELECT, bButton);
    }
    else
    {
        // Micky2.11a
        SPU_ControlHighlight(HLI_ACTION_ACTIVE, bButton);
    }
}


//  *********************************************************************************************
//  Function    :   _Decide_TVE_MPEGVideo_Mode
//  Description :   Base on Setup TV system setting to decide new TVE/ MPEG video mode
//  Argument    :   bMode, specify current TV system setting mode
//  Return      :   New target TVE mode,
//                  New MPEG video mode is updated @ __bMPEGVideoMode directly
//  *********************************************************************************************
BYTE _Decide_TVE_MPEGVideo_Mode(BYTE bMode)
{
    BYTE bTVEMode;

#ifdef SUPPORT_TFT_DIGITAL_MODE
    if (__bTFTOutput == TRUE)
    {
        bMode = SETUP_SCREEN_TV_SYSTEM_NTSC;
    }
#endif

    switch(bMode)
    {
    case    SETUP_SCREEN_TV_SYSTEM_NTSC:
        bTVEMode = SETUP_SCREEN_TV_SYSTEM_NTSC;
        __bMPEGVideoMode = MPEG_VIDEO_MODE_NTSC;
        break;

    case    SETUP_SCREEN_TV_SYSTEM_AUTO:
        //kevin0.95, fix TV mode = AUTO, N bitstream -> OPEN -> P bitstream => still display as N
        //if( LOGO_ATTR() & LOGO_CHANGE_TV_TYPE )
        if( (LOGO_ATTR()&LOGO_CHANGE_TV_TYPE) || (LOGO_TYPE()!=LOGO_DEFAULT) )
        {
            if (__btNTSCBitStream) //JPEG
            {
                bTVEMode = SETUP_SCREEN_TV_SYSTEM_NTSC;
                __bMPEGVideoMode = MPEG_VIDEO_MODE_NTSC;
            }
            else
            {
                bTVEMode = SETUP_SCREEN_TV_SYSTEM_PAL;
                __bMPEGVideoMode=MPEG_VIDEO_MODE_PAL;
            }
        }
        else //let Logo be the same as prev TV type when KEY_OPEN_COLSE
        {
            bTVEMode = __bTVType;
            if(bTVEMode == SETUP_SCREEN_TV_SYSTEM_NTSC)
                __bMPEGVideoMode = MPEG_VIDEO_MODE_NTSC;
            else
                __bMPEGVideoMode = MPEG_VIDEO_MODE_PAL;
        }
        break;

    case    SETUP_SCREEN_TV_SYSTEM_PAL_M:
        bTVEMode = SETUP_SCREEN_TV_SYSTEM_PAL_M;
        __bMPEGVideoMode = MPEG_VIDEO_MODE_NTSC;
        break;

    case    SETUP_SCREEN_TV_SYSTEM_PAL:
    case    SETUP_SCREEN_TV_SYSTEM_PAL_N:
    default:
        bTVEMode = __SetupInfo.bTVSystem;
        __bMPEGVideoMode = MPEG_VIDEO_MODE_PAL;
        break;

    }

    return bTVEMode;
}


//  **********************************************************************************************
//  Function    :   _GetRealVideoDisplayMode
//  Description :   Calculate the real aspect ratio setting mode based on the bitstream format
//  Arguments   :   None
//  Return      :   None
//                  Final video display mode will be updated @ _bVideoDisplayMode directly
//  **********************************************************************************************
void _GetRealVideoDisplayMode(void)
{
    // support aspect ratio setting only when
    // [1] No display LOGO
    // [2] DVD title
    // [3] Bitstream is 16:9
    // [4] Bitsteeam can support PANSCAN/LETTER BOX/WIDE setting

#ifndef NO_DISC_MODE
    WORD wTemp;

    if((__wDiscType&BOOK_DVD) && (__bLOGO==LOGO_NONE) )
    {
        if ( MASK_ASPECT_RATIO(__wV_ATR) == MASK_ASPECT_RATIO_16_9 ) //16:9 Video Attribute
        {

            wTemp = (WORD)MASK_DISPLAY_MODE(__wV_ATR);
            switch(wTemp)
            {
            case    MASK_DISPLAY_MODE_PS_LB:
                if (__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_PS )
                    _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_PS;
                else if (__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_LB )
                    _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_LB;
                else
                    _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_WIDE;
                // Micky2.36, force PAL bitstream no LB mode
#ifdef  FORCE_PAL_NO_LB
                if ( _bVideoDisplayMode == SETUP_SCREEN_TV_TYPE_LB )
                {
                    if (__wV_ATR & 0x3000)  // PAL bitstream
                    {
                        _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_PS;
                    }
                }
#endif
                break;

            case    MASK_DISPLAY_MODE_PS:
                if (__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_PS ||
                    __SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_LB)
                    _bVideoDisplayMode =  SETUP_SCREEN_TV_TYPE_PS;
                else
                    _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_WIDE;
                break;

            case    MASK_DISPLAY_MODE_LB:
                if (__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_LB ||
                    __SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_PS)
                    _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_LB;
                else
                    _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_WIDE;
                // Micky2.36, force PAL bitstream no LB mode
#ifdef  FORCE_PAL_NO_LB
                if ( _bVideoDisplayMode == SETUP_SCREEN_TV_TYPE_LB )
                {
                    if (__wV_ATR & 0x3000)  // PAL bitstream
                    {
                        _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_WIDE;
                    }
                }
#endif
                break;

            default:
                _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_WIDE;
                break;
            }
        }
        else // 4:3 Video Attribute
        {
            _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_WIDE;
        }
    }
    else
#endif  // #ifndef NO_DISC_MODE
    {
#ifdef SUPPORT_STB //Aron2.17-909s, for stb
        if( (__bChooseMedia == MEDIA_SELECT_STB) && (__bLOGO==LOGO_NONE))
        {
            DWORD dwTemp;
            HAL_ReadInfo (HAL_INFO_ASPECT_RATIO, &dwTemp);
            if ( (dwTemp & 0xF) == 0x3 ) // 16:9 Video Attribute
            {

                switch(__SetupInfo.bAspectRatio)
                {
                    case    SETUP_SCREEN_TV_TYPE_PS:
                        _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_PS;
                        break;

                    case    SETUP_SCREEN_TV_TYPE_LB:
                        _bVideoDisplayMode =  SETUP_SCREEN_TV_TYPE_LB;
                        break;

                    default:
                        _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_WIDE;
                        break;
                }
            }
            else // other Video Attribute
            {
                _bVideoDisplayMode = SETUP_SCREEN_TV_TYPE_WIDE;
            }
        }
#endif // #define SUPPORT_STB
    }
    //add DivX case later

}


//  ******************************************************************************************
//  Function    :   _ZoomVideo
//  Description :   Calculate zoomed clip width/ height value.
//  Arguments   :   None
//  Return      :   None
//  ******************************************************************************************
void    _ZoomVideo(void)
{
    // No zoom for [1] Digest mode [2] JPEG picture
    if (
#ifndef NO_DIGEST
        (__btDIGEST) ||
#endif  // #ifndef NO_DIGEST
        (LOGO_TYPE()==LOGO_JPEG)) // LLY0.80
        return;

    // Recover to normal zoom mode while displaying LOGO , ex CDDA or MP3 playing w/ LOGO
    if(LOGO_TYPE()==LOGO_DEFAULT) // LLY0.80
    {
        HAL_GetStreamInfo();
        __bZoomFactor=VIEW_NORMAL;
        __wXStart=0;
        __wYStart=0;
        __wHCW = (WORD)__dwPicHSize;
        __wVCW = (WORD)__dwPicVSize;
    }

    // Calculate zoomed clip width/height
    switch(__bZoomFactor)
    {
    case    VIEW_NORMAL:
        __wHCW = __dwPicHSize;
        __wVCW = __dwPicVSize;
        break;

    case    VIEW_ZOOMX2:
        // LLY2.55, re-calculate the value since we re-define ZoomX2 is area*1/2
        // So, HCW=HSize/1.44; VCW=VSize/1.44
        //__wHCW = __dwPicHSize/2;
        //__wVCW = __dwPicVSize/2;
        // 25/36 = 1/1.44 = 100/144
        __wHCW = (WORD)(__dwPicHSize*25/36);
        __wVCW = (WORD)(__dwPicVSize*25/36);
        break;

    case    VIEW_ZOOMX4:
        // LLY2.55, re-calculate the value since we re-define ZoomX4 is area*1/4
        // So, HCW=HSize/2; VCW=VSize/2
        //__wHCW = __dwPicHSize/4;
        //__wVCW = __dwPicVSize/4;
        __wHCW = __dwPicHSize/2;
        __wVCW = __dwPicVSize/2;
        break;
    }
}


//  ************************************************************************************************
//  Function    :   _AspectRatioSetting
//  Description :   Main video (aspect ratio)/ SP(WIDE, LS, PS)/ Display Screen(N2P, P2N) setting.
//  Argument    :   dwFrameIdx, specify condition for TV mode setting time
//                  0, 1, 2: specify to do setting while the desired frame index is displayed
//                           only used for decoder
//                  DISP_NEXTSYNC: specify to do setting while next vsync
//                                 only used for user force change TV mode
//  Return      :   None
//  ************************************************************************************************
void    _AspectRatioSetting(DWORD dwFrameIdx)
{
    WIN_PARAMETER	ClipArea;
    BYTE    bMode;
    BYTE	bTVSPMode;
    BYTE    bSPMode = MODE_WS;

    if (__btNTSCBitStream)
	{
		bTVSPMode = MODE_N2N;
		if ( __bMPEGVideoMode ==MPEG_VIDEO_MODE_PAL )
		{
			bTVSPMode = MODE_N2P;
		}
	}
    else
	{
		bTVSPMode = MODE_P2P;
		if ( __bMPEGVideoMode ==MPEG_VIDEO_MODE_NTSC )
		{
			bTVSPMode = MODE_P2N;
		}
	}

#ifndef NO_DISC_MODE
    if((__wDiscType&BOOK_DVD) && __bLOGO==LOGO_NONE)
    {
        switch(_bVideoDisplayMode)
        {
        case    SETUP_SCREEN_TV_TYPE_WIDE:
            ClipArea.sHStart = __wXStart;
            ClipArea.sVStart = __wYStart;
            ClipArea.wHWidth = __wHCW;
            ClipArea.wVWidth = __wVCW;
            bTVSPMode |= MODE_WS;
            bSPMode = MODE_WS;
            // SPU_DisplayMode(MODE_WS, MODE_WS|bTVSPMode, 0);
            break;
        case    SETUP_SCREEN_TV_TYPE_LB:
            ClipArea.sHStart = __wXStart;
            ClipArea.sVStart = __wYStart - (__wVCW/6);
            ClipArea.wHWidth = __wHCW;
            ClipArea.wVWidth = (__wVCW*4)/3;
            bTVSPMode |= MODE_LB;
    	    if (__bSPST_LB)
                bSPMode = MODE_LB;
                // SPU_DisplayMode(MODE_LB, MODE_LB|bTVSPMode, 0);
    	    else
                bSPMode = MODE_WS;
                //SPU_DisplayMode(MODE_WS, MODE_LB|bTVSPMode, 0);

	        break;
        case    SETUP_SCREEN_TV_TYPE_PS:
            DISP_PSMode(TRUE);
            ClipArea.sHStart = __wHCW/8 + __wXStart;
            ClipArea.sVStart = __wYStart;
            ClipArea.wHWidth = (__wHCW*3)/4;
            ClipArea.wVWidth = __wVCW;
            bTVSPMode |= MODE_PS;
            if (__bSPST_Display_Mode==0)	// PS
                bSPMode = MODE_PS;
                // SPU_DisplayMode(MODE_PS, MODE_PS|bTVSPMode, 0);
            else
                bSPMode = MODE_WS;
                // SPU_DisplayMode(MODE_WS, MODE_PS|bTVSPMode, 0);
            break;
        }
    }
    else if (__bCDDA_DTS == CDDA_CDG) //kevin1.02, fix CDG N<->P, display abnormal
    {
        ClipArea.sHStart = CDG_CLIP_XSTART;
        ClipArea.sVStart = CDG_CLIP_YSTART;
        ClipArea.wHWidth = CDG_CLIP_WIDTH;
        ClipArea.wVWidth = CDG_CLIP_HEIGHT;
        bTVSPMode |= MODE_WS;
    }
    else
#endif  // #ifndef NO_DISC_MODE
    {
        // Chuan1.05b, Adjust to Best fit for DivX case.
#ifndef FORCE_FULLSCREEN_FOR_DIVX
        if ((__bAttrPlay == ATTR_AVI) && (__bZoomFactor == VIEW_NORMAL)) //Chuan1.11 can't calculate best fit under Zoom mode.
        {

#define     DIVX_BORDER_WIDTH       (DIVX_BORDER_PERC*65536/100)    //The Height/Width Border Percent


            DWORD dwTVHeight;                       //The height of TV resolution
            DWORD dwBorder_Width, dwBorder_Height;  //The width/height of border
            DWORD dwFAR, dwDAR;                     //Frame aspect ratio, display aspect ratio
            DWORD dwHResizeRatio,dwVResizeRatio;    //Horizontal/Vertical resize ratio
            DWORD dwHPar, dwVPar;                   //Horizontal/Vertical pixel aspect ratio
            DWORD dwHDar, dwVDar;
            DWORD dwtemp1, dwtemp2;

            //Define HDar/VDar/TV signal on which TV mode and screen setting
            if (__wVCW <= 480)
            {
                if (__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_WIDE)
                {
                    dwHDar = 40;
                    dwVDar = 33;
                }
                else
                {
                    dwHDar = 10;
                    dwVDar = 11;
                }
                dwTVHeight = 480;
                bTVSPMode = MODE_N2N;
                if ( __bMPEGVideoMode ==MPEG_VIDEO_MODE_PAL )
                {
                    bTVSPMode = MODE_N2P;
                }
            }
            else
            {
                if (__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_WIDE)
                {
                    dwHDar = 16;
                    dwVDar = 11;
                }
                else
                {
                    dwHDar = 12;
                    dwVDar = 11;
                }
                dwTVHeight = 576;
                bTVSPMode = MODE_P2P;
                if ( __bMPEGVideoMode ==MPEG_VIDEO_MODE_NTSC )
                {
                    bTVSPMode = MODE_P2N;
                }
            }

            {
#define PAR_12_11_ON_625_4_3        2
#define PAR_10_11_ON_525_4_3        3
#define PAR_16_11_ON_625_16_9       4
#define PAR_40_33_ON_525_16_9       5

            switch(__dwAspect_Ratio_Info)
            {
                case PAR_12_11_ON_625_4_3:
                    dwHPar = 12;
                    dwVPar = 11;
                    DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "PAR_12_11_ON_625_4_3");
                    break;

                case PAR_10_11_ON_525_4_3:
                    dwHPar = 10;
                    dwVPar = 11;
                    DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "PAR_10_11_ON_525_4_3");
                    break;

                case PAR_16_11_ON_625_16_9:
                    dwHPar = 16;
                    dwVPar = 11;
                    DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "PAR_16_11_ON_625_16_9");
                    break;

                case PAR_40_33_ON_525_16_9:
                    dwHPar = 40;
                    dwVPar = 33;
                    DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "PAR_40_33_ON_525_16_9");
                    break;

                default:
                    dwHPar = 1;
                    dwVPar = 1;
                    DBG_Printf(DBG_THREAD_DECODER, DBG_INFO_PRINTF, "PAR_1_1");
                    break;
            }

            // Compute FAR = PAR* video_width/ video_height
            dwFAR = (__wHCW << 16) * dwHPar / (__wVCW * dwVPar);

            if (__SetupInfo.bAspectRatio != SETUP_SCREEN_TV_TYPE_WIDE)  // TV is 4:3
            {
                //Compute DAR = 4/3 (4:3 Screen)
                dwDAR = ( 4 << 16) / 3;

                if (dwFAR > dwDAR)
                {
                    // Letter boxing
                    dwHResizeRatio = (720 << 16)/ __wHCW * 704 / 720;
                    dwVResizeRatio = (((dwTVHeight << 16) * dwVPar)/ (__wHCW * 3 * dwHPar))* 4 ;
                }
                else if(dwFAR < dwDAR)
                {
                    // Pillar boxing
                    dwHResizeRatio = (((dwHPar * dwTVHeight * dwVDar) << 14)/(dwVPar * __wVCW * dwHDar)) << 2;
                    dwVResizeRatio = (dwTVHeight << 16)/ __wVCW;
                }
                else
                {
                    //Fit screen
                    dwHResizeRatio = (1 << 16) * 704 / 720;
                    dwVResizeRatio = (1 << 16);
                }

#ifdef ASPECT_RATIO_PANSCAN
                if(  (__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_PS) && (dwFAR > dwDAR))
                {
                    //Pan & Scan for wide screen video
                    ClipArea.wVWidth = __wVCW;
                    ClipArea.sVStart = __wYStart;

                    dwBorder_Height = 0;
                    dwBorder_Width = (((__wHCW << 16) - (((__wVCW)*dwVResizeRatio)/dwTVHeight)*720) / (2*dwHResizeRatio));

                    //Confirm the border width is positive
                    if ((__wHCW/2) < dwBorder_Width)
                    {
                        dwBorder_Width = (((((__wVCW)*dwVResizeRatio)/dwTVHeight)*720) - (__wHCW << 16)) / (2*dwHResizeRatio);
                    }

                    ClipArea.wHWidth = __wHCW - (2*dwBorder_Width);
                    ClipArea.sHStart = __wXStart + (dwBorder_Width);
                }

                else //if((__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_PS) && (dwFAR > dwDAR))
#endif //#define ASPECT_RATIO_PANSCAN
                {
                    //If aspect ratio of setup menu is not Pan & Scan and video is not wide screen
                    if(dwFAR == dwDAR)
                    {
                        //Fit screen
                        dwBorder_Height = 0;
                        dwBorder_Width = 0;
                    }
                    else //if(dwFAR == dwDAR)
                    {
                        dwBorder_Height = (((dwTVHeight << 16) - (__wVCW * dwVResizeRatio)) / (2*dwVResizeRatio));
                        dwBorder_Width = (((720 << 16) - (__wHCW * dwHResizeRatio)) / (2*dwHResizeRatio));
                    }

                    ClipArea.wHWidth = __wHCW + (2 * dwBorder_Width);
                    ClipArea.wVWidth = __wVCW + (2 * dwBorder_Height);
                    ClipArea.sHStart = __wXStart - dwBorder_Width;
                    ClipArea.sVStart = __wYStart - dwBorder_Height;
                }
            }
            else if((__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_WIDE))    //TV is 16:9
            {
                //Compute DAR = 16/9 (16:9 Screen)
                dwDAR = ( 16 << 16) / 9;

                if (dwFAR >= dwDAR)
                {
                    // Letter boxing
                    dwHResizeRatio = (720 << 16)/ __wHCW;
                    dwVResizeRatio = (((dwTVHeight << 16) * dwVPar)/ (__wHCW * 9 * dwHPar))* 16;
                }
                else //if (dwFAR >= dwDAR)
                {
                    // Pillar boxing
                    dwHResizeRatio = (((dwHPar * dwTVHeight * dwVDar) << 14)/(dwVPar * __wVCW * dwHDar))<<2;
                    dwVResizeRatio = (dwTVHeight << 16)/ __wVCW;
                }

                dwtemp1 = (__wHCW * dwHResizeRatio);
                dwtemp2 = (__wVCW * dwVResizeRatio);

                if(dwtemp1 > (720 << 16))
                {
                    //If (__wHCW * dwHResizeRatio) bigger than the width of TV resolution
                    dwBorder_Width = ((dwtemp1 >> 16) - __wHCW)/2;

                    ClipArea.wHWidth = __wHCW;
                    ClipArea.sHStart = __wXStart;

                    dwBorder_Height = (((dwTVHeight << 16) - dwtemp2) / (2*dwVResizeRatio));
                    //Confirm the border height is positive
                    if(dwBorder_Height >> 8)
                    {
                        dwBorder_Height = ((dwtemp2 - (dwTVHeight<< 16)) / (2*dwVResizeRatio));
                    }
                    ClipArea.wVWidth = __wVCW + (2 * (dwBorder_Height));
                    ClipArea.sVStart = __wYStart - (dwBorder_Height);
                }
                else //if(dwtemp1 > (720 << 16))
                {
                    dwBorder_Height = (((dwTVHeight << 16) - (__wVCW * dwVResizeRatio)) / (2*dwVResizeRatio));
                    dwBorder_Width = ((((720 << 16)- (__wHCW * dwHResizeRatio)) / (2*dwHResizeRatio)) );

                    ClipArea.wHWidth = __wHCW + (2 * dwBorder_Width);
                    ClipArea.wVWidth = __wVCW + (2 * dwBorder_Height);
                    ClipArea.sHStart = __wXStart - dwBorder_Width;
                    ClipArea.sVStart = __wYStart - dwBorder_Height;
                }
                bTVSPMode |= MODE_WS;
                bSPMode = MODE_WS;
            }

            }

#ifdef ASPECT_RATIO_DEBUG
            printf("FAR = %lx, DAR = %lx\n",dwFAR, dwDAR);
            printf("HRatio = %lx, VRatio = %lx\n",dwHResizeRatio,dwVResizeRatio);
            printf("Border_Width = %lx, Border_Height = %lx\n",dwBorder_Width, dwBorder_Height);

            printf("HStart = %lx, VStart = %lx\n" , ClipArea.sHStart, ClipArea.sVStart);
            printf("HWidth = %lx, VWidth = %lx\n" , ClipArea.wHWidth, ClipArea.wVWidth);
#endif //#ifdef ASPECT_RATIO_DEBUG


            //Set the extra border width/height

            dwBorder_Height = (DIVX_BORDER_WIDTH * ClipArea.wVWidth) >> 16;
            dwBorder_Width = (DIVX_BORDER_WIDTH * ClipArea.wHWidth) >> 16;

            ClipArea.wVWidth += (2*dwBorder_Height);
            ClipArea.wHWidth += (2*dwBorder_Width);
            ClipArea.sVStart -= dwBorder_Height;
            ClipArea.sHStart -= dwBorder_Width;

#ifdef ASPECT_RATIO_DEBUG
            printf("Extra Border: Border_Width = %lx, Border_Height = %lx\n",dwBorder_Width, dwBorder_Height);
            printf("Extra Border: HStart = %lx, VStart = %lx\n" , ClipArea.sHStart, ClipArea.sVStart);
            printf("Extra Border: HWidth = %lx, VWidth = %lx\n" , ClipArea.wHWidth, ClipArea.wVWidth);
#endif //#ifdef ASPECT_RATIO_DEBUG

        }
        else
#endif  // #ifndef FORCE_FULLSCREEN_FOR_DIVX
        {
#ifdef SUPPORT_STB    //Aron2.17-909S, for stb
            if( (__bChooseMedia == MEDIA_SELECT_STB) && (__bLOGO==LOGO_NONE))
            {
                switch(_bVideoDisplayMode)
                {
                case    SETUP_SCREEN_TV_TYPE_WIDE:
                    ClipArea.sHStart = __wXStart;
                    ClipArea.sVStart = __wYStart;
                    ClipArea.wHWidth = __wHCW;
                    ClipArea.wVWidth = __wVCW;
                    bTVSPMode |= MODE_WS;
                    bSPMode = MODE_WS;
                    break;

                case    SETUP_SCREEN_TV_TYPE_LB:
                    ClipArea.sHStart = __wXStart;
                    ClipArea.sVStart = __wYStart - (__wVCW/6);
                    ClipArea.wHWidth = __wHCW;
                    ClipArea.wVWidth = (__wVCW*4)/3;
                    bTVSPMode |= MODE_LB;
                    bSPMode = MODE_LB;
                    break;

                case    SETUP_SCREEN_TV_TYPE_PS:
                    DISP_PSMode(TRUE);
                    ClipArea.sHStart = __wHCW/8 + __wXStart;
                    ClipArea.sVStart = __wYStart;
                    ClipArea.wHWidth = (__wHCW*3)/4;
                    ClipArea.wVWidth = __wVCW;
                    bTVSPMode |= MODE_PS;
                    bSPMode = MODE_PS;
                    break;
                }
            }
            else
#endif //#define SUPPORT_STB
        	//LOGO, VCD...
        	{
                ClipArea.sHStart = __wXStart;
                ClipArea.sVStart = __wYStart;
                ClipArea.wHWidth = __wHCW;
                ClipArea.wVWidth = __wVCW;
                bTVSPMode |= MODE_WS;
        	}
        }
    }

    switch(__bMPEGVideoMode)
    {
    case    MPEG_VIDEO_MODE_NTSC:
        if(__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
        {
            MainVideoInfo.sHStart = VIDEO_HSTART_NTSC_PSCAN ;
            MainVideoInfo.sVStart = VIDEO_VSTART_NTSC_PSCAN;
            bMode = MODE_NTSC_NORMAL | MODE_PSCAN;
        }
        else
        {
            MainVideoInfo.sHStart = VIDEO_HSTART_NTSC;
            MainVideoInfo.sVStart = VIDEO_VSTART_NTSC;
            bMode = MODE_NTSC_NORMAL;
        }
        MainVideoInfo.wVWidth = 480;
        break;

    default:
    case    MPEG_VIDEO_MODE_PAL:
        if(__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
        {
            MainVideoInfo.sHStart = VIDEO_HSTART_PAL_PSCAN;
            MainVideoInfo.sVStart = VIDEO_VSTART_PAL_PSCAN;
            bMode = MODE_PAL_NORMAL | MODE_PSCAN;
        }
        else
        {
            MainVideoInfo.sHStart = VIDEO_HSTART_PAL;
            MainVideoInfo.sVStart = VIDEO_VSTART_PAL;
            bMode = MODE_PAL_NORMAL;
        }
        MainVideoInfo.wVWidth = 576;
        break;
    }


#ifdef SUPPORT_TFT_DIGITAL_MODE
    if (__bTFTMode == MODE_TFT_DIGITAL)
    {
        bMode = MODE_TFT_DIGITAL;
    }
#endif // SUPPORT_TFT

    DISP_TVModeSet(bMode, (MainVideoInfo.sHStart + 22), MainVideoInfo.sVStart);

    if( LOGO_TYPE() == LOGO_JPEG)
    {
        HALJPEG_ChangeTVMode();
    }
    else
    {
        // LLY2.16, porting Aron's code for DVB's EPG or channel list mode
#ifdef SUPPORT_STB
        extern BYTE __bDVBDispNormal;
        extern WIN_PARAMETER   __DVBMainVideoInfo;
        if(__bDVBDispNormal == FALSE)
        {
            DISP_MainVideoSet(&__DVBMainVideoInfo,__dwPicHSize, __dwPicVSize, dwFrameIdx);
        }
        else
#endif  // #ifdef SUPPORT_STB
        {
            DISP_MainVideoSet(&MainVideoInfo,__dwPicHSize, __dwPicVSize, dwFrameIdx);
            DISP_Clip(&ClipArea, dwFrameIdx, DISP_MAINVIDEO);
            if (__dwSPType != SP_TYPE_NONE)
                SPU_DisplayMode(bSPMode, bTVSPMode, 0);

        }
    }
}


//  *********************************************************************************************
//  Function    :   HAL_AdjustTVMode
//  Description :   Do TV mode change setting, include MPEG chip, TVE, TFT driver
//  Argument    :   dwFrameIdx, specify condition for TV mode setting time
//                  0, 1, 2: specify to do setting while the desired frame index is displayed
//                           only used for decoder
//                  DISP_NEXTSYNC: specify to do setting while next vsync
//                                 only used for user force change TV mode
//  Return      :   None
//  *********************************************************************************************
void HAL_AdjustTVMode(DWORD dwFrameIdx)
{
    BYTE    bNewTarget;

    //kevin0.75, fix for PAL bitstream PAL->AUTO=> NTSC settings (decide __btNTSCBitStream at the start of HAL_AdjustTVMode)
    // Decide N or P bitstream
    HAL_GetStreamInfo();

    // Based on __SetupInfo.bTVSystem, decide bNewTarget / __bMPEGVideoMode
    bNewTarget=_Decide_TVE_MPEGVideo_Mode(__SetupInfo.bTVSystem);

    __bLOGO &= ~(LOGO_CHANGE_TV_TYPE);

    // Calculate the real aspect ratio setting base on bitstram attribute
	_GetRealVideoDisplayMode();

    // Calculate zoomed width/height
    _ZoomVideo();

    // Set DISP_TVModeSet, DISP_MainVideoSet, and DISP_Clip at final stage followed by TVE programming
    _AspectRatioSetting(dwFrameIdx); //kevin909, rename later

    // Program TV encoder
#ifdef SUPPORT_TFT

	// Chuan2.77, Display module support one field display for OSD/ MainVideo, and control by F/W
#ifdef FORCE_VIDEO_ONEFIELD 
    DISP_OneFieldCtrl(DISP_MAINVIDEO, TRUE);
#else
    DISP_OneFieldCtrl(DISP_MAINVIDEO, FALSE);
#endif

#ifdef FORCE_OSD_ONEFIELD
    DISP_OneFieldCtrl(DISP_OSD, TRUE);
#else
    DISP_OneFieldCtrl(DISP_OSD, FALSE);
#endif


#ifdef SUPPORT_TFT_DIGITAL_MODE
    if (__bTFTMode == MODE_TFT_DIGITAL)
    {
        TFT_AdjustTVMode(TFT_MODE_TFT_DIGITAL);
    }
    else
#endif // #ifdef SUPPORT_TFT_DIGITAL_MODE
    {
        TFT_AdjustTVMode(bNewTarget);
    }

#else

    // Chuan2.77, One field display is just for Panel output!!
    DISP_OneFieldCtrl(DISP_MAINVIDEO, FALSE);
    DISP_OneFieldCtrl(DISP_OSD, FALSE);

#endif // #ifdef SUPPORT_TFT

    __bTVType = bNewTarget;
    HAL_SetTVE();

    // Adjust OSD region
    OSD_AdjustRegion();

#ifdef SUPPORT_STB //CoCo2.16, add for STB
    if ( __bChooseMedia == MEDIA_SELECT_DVD)
    {
        OSD_UpdateZoomState();
    }
#endif // #ifdef SUPPORT_STB
}



//  ***************************************************************************************
//  Function    :   HAL_GetStrameInfo
//  Description :   Get video stream information, to get picture size, format
//  Arguments   :   None
//  Return      :   TRUE, the value is valid
//                  FALSE, the value is invalid
//  ***************************************************************************************
BYTE	HAL_GetStreamInfo(void)
{
    BYTE    bTemp=TRUE;
    WORD    wTemp;

    if(LOGO_TYPE()==LOGO_JPEG)
        return TRUE;

    HAL_ReadInfo(HAL_INFO_HSIZE, &__dwPicHSize);
    HAL_ReadInfo(HAL_INFO_VSIZE, &__dwPicVSize);

    if (__bAttrPlay == ATTR_AVI)
    {
        // Chuan1.00, DivX case: PAL if VSize > 480
        wTemp = 480;
        bTemp = TRUE;
    }
    else
    {
        if ( __dwPicVSize > 350 )
        {
            wTemp= 490;
            if ( ( __dwPicVSize > 600) || (__dwPicVSize < 400) )
            {
                // invalid value
                bTemp=FALSE;
            }
        }
        else
        {
            wTemp= 250;
            if ( ( __dwPicVSize > 300) || (__dwPicVSize < 200 ) )
                // invalid value
                bTemp=FALSE;
        }
    }

    if (bTemp)
    {
        if ( __dwPicVSize > wTemp )
        {   // it's PAL bitstream
            __btNTSCBitStream= FALSE;
        }
        else
        {   // it's NTSC bitstream
            __btNTSCBitStream= TRUE;
        }
    }

	return TRUE;
}


// **************************************************************************
// Function     :   HAL_Zoom
// Description  :   Issue zoom video or zoom_move action
// Parameters   :   bAction, the action for zoom function
// Return       :   TRUE, function is done.
// Side Effect  :
// **************************************************************************
// LLY2.20, modify the procedure while zoom moving to let it can move to boundary
// And, enable the flag while reach the boundary
BYTE    HAL_Zoom(BYTE bAction)
{
    BYTE    bTemp=TRUE;
    DWORD   dwTemp;

    //kevin1.02, avoid thread related potential pitfall (JPEG->MP3 => __bAttrPlay!= MP3, but thread is still in JPEG)
    //if(__bAttrPlay == ATTR_JPG)
    if( OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_JPEG_DONE ) //currently not in JPEG thread
    {
		// LLY2.60, porting Grievous's code to support Motion JPEG zoom mode
		// Since, the video decoder thread is JPEG, but it need go same path of MPEG motion
		if( (__bAttrPlay == ATTR_AVI) && (__dwAVIVideoFMT==PARSER_ESFMT_MJPG) )
		{
            goto LABEL_MPEG_MOTION_ZOOM_MODE;
		}

        //kevin0.91, fix motion -> ZOOM -> JPEG -> motion => clip postion error
        __wXStart=0;
        __wYStart=0;

        if (bAction != VIEW_ZOOM_SUPPORT)
        {
            ///bTemp = _JPEGZoom(bAction, HAL_JPEG_DECODE);
            return TRUE;
        }
    }

LABEL_MPEG_MOTION_ZOOM_MODE:
    switch(bAction)
    {
    case    VIEW_ZOOM_SUPPORT:
        HAL_ReadInfo(HAL_INFO_HLI_STATUS, & dwTemp);
        if(LOGO_TYPE()==LOGO_DEFAULT || dwTemp) // LLY0.80
            return FALSE;
        else
            return TRUE;

    // Calculate zoomed clip staring x/y
    case    VIEW_NORMAL:
        __wXStart=0;
        __wYStart=0;
        break;
    case    VIEW_ZOOMX2:
        // LLY2.55, re-calculate the value since we re-define ZoomX2 is area*1/2
        // And, the 1st selection area is located @ center.
        // x={(1/2)-[(1/2)*(1/1.44)]}HorSize
        // y={(1/2)-[(1/2)*(1/1.44)]}VerSize
        // 0.153 = (1/2)-((1/2)*(1/1.44))
		//__wXStart=(WORD)__dwPicHSize/4;
        //__wYStart=(WORD)__dwPicVSize/4;
        __wXStart=(WORD)(__dwPicHSize*153/1000);
        __wYStart=(WORD)(__dwPicVSize*153/1000);
        break;
    case    VIEW_ZOOMX4:
        // LLY2.55, re-calculate the value since we re-define ZoomX4 is area*1/4
        // And, the 1st selection area is located @ center
        // x=(1/2)-((1/2)*(1/2))HorSize
        // y=(1/2)-((1/2)*(1/2))VerSize
        // 0.25 = (1/2)-((1/2)*(1/2))
		//__wXStart=(WORD)__dwPicHSize*3/8;
        //__wYStart=(WORD)__dwPicVSize*3/8;
        __wXStart=(WORD)(__dwPicHSize/4);
        __wYStart=(WORD)(__dwPicVSize/4);
        break;

    case    VIEW_ZOOM_LEFT:
        if (__bZoomFactor==VIEW_NORMAL)
            return FALSE;
        if (__wXStart >= 16)
        {
            __wXStart-=16;
        }
        else if( (__wXStart > 0) && (__wXStart <16) )
        {
            __wXStart = 0;
        }
        else
            return FALSE;
        break;

    case    VIEW_ZOOM_RIGHT:
        if (__bZoomFactor==VIEW_NORMAL)
            return FALSE;
        // Calculate the remain space to right boundary first
        dwTemp = __dwPicHSize -__wXStart - __wHCW;
        // Remain space to right boundary >= 16
        if ( dwTemp >= 16 )
        {
            __wXStart+=16;
        }
        // 0 < Remain space to right boundary < 16
        else if( (0<dwTemp) && (dwTemp<16) )
        {
            __wXStart = (__dwPicHSize - __wHCW);
        }
        // Remain space to right boundary <= 0
        else
        {
            return FALSE;
        }
        break;
    case    VIEW_ZOOM_UP:
        if (__bZoomFactor==VIEW_NORMAL)
            return FALSE;
        if (__wYStart >= 16)
        {
            __wYStart-=16;
        }
        else if( (__wYStart > 0) && (__wYStart < 16) )
        {
            __wYStart = 0;
        }
        else
        {
            return FALSE;
        }
        break;
    case    VIEW_ZOOM_DOWN:
        if (__bZoomFactor==VIEW_NORMAL)
            return FALSE;

        // Calculate remain space to bottom boundary first
        dwTemp = __dwPicVSize - __wYStart - __wVCW;

        // Remain space to bottom boundary >=16
        if ( dwTemp > 16 )
        {
            __wYStart+=16;
        }
        // 0< Remain space to bottom boundary < 16
        else if( (0<dwTemp) && (dwTemp<16) )
        {
            __wYStart = (__dwPicVSize - __wVCW);
        }
        else
            return FALSE;
        break;
    default:
        return (FALSE);
    }

    if (bTemp)
    {
        _ZoomVideo();
        _AspectRatioSetting(DISP_NEXTVSYNC);

        // LLY2.20, enable the flag if last zoom moving action
        __bZoomPos = 0;  // clear it as none first

        // Return directly if Zoom normal
        if(bAction == VIEW_NORMAL)
        {
            return TRUE;
        }

        if( (__wXStart+__wHCW) >= __dwPicHSize )
        {
            __bZoomPos |= POS_LAST_RIGHT;
        }
        if( (__wYStart+__wVCW) >= __dwPicVSize )
        {
            __bZoomPos |= POS_LAST_DOWN;
        }
        if( __wYStart == 0)
        {
            __bZoomPos |= POS_LAST_UP;
        }
        if( __wXStart == 0)
        {
            __bZoomPos |= POS_LAST_LEFT;
        }

        //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "ZoomPos: %lx, XStart: %lx, YStart: %lx\n", __bZoomPos, __wXStart, __wYStart);
        //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "__wHCW: %lx, __wVCW: %lx, __dwPicHSize: %lx, __dwPicVSize: %lx \n", __wHCW, __wVCW, __dwPicHSize, __dwPicVSize);

        return TRUE;
    }
    return FALSE;
}

void    HAL_AdjustTFT_DigitalMode(BYTE bOn)
{
#ifdef SUPPORT_TFT_DIGITAL_MODE
    if (__bTFTOutput == FALSE)
    {
        TFT_BackLight(FALSE);
        TFT_FreeRun(TRUE);
    }

    HAL_Force_TVOut_Off(__bTFTOutput);

    if (bOn == TRUE)
    {
        if ((__bTFTMode == MODE_TFT_ANALOG) && (__bTFTOutput == TRUE))
        {
            __bTFTMode = MODE_TFT_DIGITAL;
            TFT_BackLight(FALSE);
            TFT_FreeRun(TRUE);

            HAL_AdjustTVMode(DISP_NEXTVSYNC);
            DISP_DelayVSYNC(1);

            TFT_FreeRun(FALSE);
            TFT_BackLight(TRUE);
        }
    }
    else
    {
        if (__bTFTMode == MODE_TFT_DIGITAL)
        {
            __bTFTMode = MODE_TFT_ANALOG;
            TFT_BackLight(FALSE);
            TFT_FreeRun(TRUE);

            HAL_AdjustTVMode(DISP_NEXTVSYNC);
            DISP_DelayVSYNC(1);

            if (__bTFTOutput == TRUE)
            {
                TFT_FreeRun(FALSE);
                TFT_BackLight(TRUE);
            }
        }
    }
#endif

}


#if defined(SUPPORT_POWERON_MUSIC) || !defined(REMOVE_SETUP_SPEAKER_TEST)
//kevin1.05, create
//*********************************************************************
// Description  :   Play Test Tone
// Arguments    :   None
// Return       :   None
//*********************************************************************
void HAL_PlayTone(BYTE bCh)
{
#include "tone.h"
    WORD *pwPtr = aTone;
    DWORD dwCnt, dwToneBitStart, dwToneBitSize;

    dwToneBitSize = sizeof(aTone)/sizeof(WORD);

    //see HAL_SetBuffer for categorization
    if(__bAttrPlay == ATTR_AVI) //AVI/DivX
    {
     dwToneBitStart = DS_AD0BUF_ST_DIVX;
    }
    else if(__bAttrPlay & TYPE_CDROM_AUDIO || __bAttrPlay==ATTR_CDDA) //.MP3/.MP2/.WMA/CDDA/CD-DTS
    {
     dwToneBitStart = DS_AD0BUF_ST_MM;
    }
    // LLY2.01, add CDROM/ DVDROM motion case except AVI
    else if(__bAttrPlay & TYPE_CDROM_AV)
    {
        dwToneBitStart = DS_AD0BUF_ST_MM_MOTION;
    }
    else  //DVD/VCD
    {
     dwToneBitStart = DS_AD0BUF_ST;
    }

    for(dwCnt=0; dwCnt<dwToneBitSize; dwCnt++)
    {
     *( (volatile WORD *)dwToneBitStart + dwCnt ) = *(pwPtr+dwCnt);
    }

    HAL_WriteAM( HAL_AM_ABUF0_ADR, dwToneBitStart);
    HAL_WriteAM(HAL_AM_ABUF0_LEN, dwToneBitSize);
    HAL_WriteAM( HAL_AM_PCMBUF_ADR, DS_PCMBUF_ST);
    HAL_WriteAM( HAL_AM_PCMBUF_LEN, (DS_PCMBUF_END-DS_PCMBUF_ST)>>11);

    HAL_WriteAM(HAL_AM_CHANNEL_MODE, bCh);

    HAL_WriteAM(HAL_AM_PCM_SCALE, VOLUME_MAX);
    HAL_WriteAM(HAL_AM_AUDIO_TYPE, 7);
    HAL_Reset(HAL_RESET_AUDIO);
    HAL_SetAudioDAC ( AUDIO_FREQ_44K );
    HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);

    //kevin1.07, add
    HAL_IOMute(FALSE); //H/W unmute

    //kevin1.05, fix HW power on music -> play AV w/o audio => power on music appears again
    HAL_WriteAM(HAL_AM_AUDIO_TYPE, 0);

}
#endif
