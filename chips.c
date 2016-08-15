#include "winav.h"
#include "digest.h"

#include "cc.h"
#include "utl.h"
#include "ctkav.h"
#include "input.h"
#include "chips.h"
#include "utl.h"
#include "osd.h"
#include "hal.h"
#include "setup.h"
#include "infofilter.h"
#include "SrcFilter.h"
#include "navigate.h"
#include "initial.h"
#include "media.h"
#ifdef CT950_STYLE
#include "dvdsetup_op.h"
#endif //CT950_STYLE

#undef printf
#ifndef WIN32
#define printf(x, y...)     DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, x, ## y)
#endif // WIN32

char    __bEchoLevel;
BYTE    __bSurround;     // Record Surround kind
BYTE    __bVocal;
DWORD   __dwVolAudio;
DWORD   __dwPreVolAudio; //Kevin1.00, add

#ifndef NO_MIC_INPUT
    DWORD   __wMICVol;
#endif // NO_MIC_INPUT

char __cKeyShift;            // Key adjustment; Up/Down
char _bDualMode;
char _bAudType;
BYTE _bSpDifMode; // 1:output compressed bitstream, 0:output stereo lpcm  (Kevin0.84a add)2:disable spdif output
BYTE _bChannelMode;
char _bLFE;

// TCC051, for reduce sram
WORD _wCHIPSTmp;

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
extern BYTE __bMMJPEGAudioMode;
#endif //SUPPORT_JPEG_AUDIO_PLAYING

// wyc2.17-909S, for language study.
#if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)
    extern BYTE __bLangStudyFlag;
#endif // (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)

// Brian.170, SW MIC Detect
#ifdef ENABLE_AUTOVOCAL
    #define MIC_INPUT_THRESHOLD_HIGH 1450L
    #define MIC_INPUT_THRESHOLD_LOW 1280L
    #ifdef SW_MIC_DETECT
        extern bit __sbMICDetect;
    #endif // SW_MIC_DETECT
#endif // ENABLE_AUTOVOCAL

//  *********************************************************************
//  Function    :   _SetAudio
//  Description :   It will set audio variable _bAudType according to __bAudioType
//  Arguments   :   bType:      The type of command (Reference HAL_SetAudioType)
//  Return      :   None
//  Change      :
//  Side Effect :
//  *********************************************************************
void _SetAudio(BYTE bAudio)
{
#ifdef SUPPORT_STB
    if(__bChooseMedia == MEDIA_SELECT_STB)
    {
        switch(bAudio)
        {
        case HAL_AUDIO_MP2:
        case HAL_AUDIO_MPG:
        case HAL_AUDIO_MP3:
            _bSpDifMode = 0; //pcm
            break;

        case HAL_AUDIO_AC3:
            _bSpDifMode = 1;  //always set raw as default when DVB AC3
            break;

        default:
            break;
        }
    }
    else
#endif

    switch(bAudio)
    {
    // TCC025, use same definition as HAL
    case HAL_AUDIO_PCM:
    case HAL_AUDIO_LPCM:
        _bAudType = AUDIO_PCM;
        // TCC043, must force spdif off for pcm data so that it also send through SPDIF
        if(__SetupInfo.bAudioDigitalOut == SETUP_AUDIO_DIGITAL_OUT_OFF)
        {
            _bSpDifMode = 2; //off
        }
        else
        {
            _bSpDifMode = 0;  //pcm
        }
        break; // PCM

    case HAL_AUDIO_MP2:  //Alex1.10, System will support MP2 format audio
    case HAL_AUDIO_MPG:
    case HAL_AUDIO_MP3:
        // TCC043, must force spdif off for pcm data so that it also send through SPDIF
        _bAudType = AUDIO_MPG;
        if(__SetupInfo.bAudioDigitalOut == SETUP_AUDIO_DIGITAL_OUT_OFF)
        {
            _bSpDifMode = 2; //off
        }
        else
        {
            _bSpDifMode = 0; //pcm
        }
        break; // MPEG

#ifdef SUPPORT_WMA
    case HAL_AUDIO_WMA:  //Kevin1.08
        // TCC043, must force spdif off for pcm data so that it also send through SPDIF
        _bAudType = AUDIO_WMA;
        if(__SetupInfo.bAudioDigitalOut == SETUP_AUDIO_DIGITAL_OUT_OFF)
        {
            _bSpDifMode = 2; //off
        }
        else
        {
            _bSpDifMode = 0; //pcm
        }
        break; // HAL_AUDIO_WMA
#endif //#ifdef SUPPORT_WMA

    case HAL_AUDIO_AC3:
        _bAudType = AUDIO_AC3;
        // TCC274, set spdif mode according to setupinfo.
        if(__SetupInfo.bAudioDigitalOut == SETUP_AUDIO_DIGITAL_OUT_SPDIF_PCM)
        {
            _bSpDifMode = 0;  //pcm
        }
        else if(__SetupInfo.bAudioDigitalOut == SETUP_AUDIO_DIGITAL_OUT_SPDIF_RAW)
        {
            _bSpDifMode = 1;  //raw
        }
        else
        {
            _bSpDifMode = 2;  //off
        }
        break; // HAL_AUDIO_AC3

    // TCC039, add setting DTS and SDDS type to force spdif mode
    case HAL_AUDIO_DTS:
        _bAudType = AUDIO_DTS;     // ** TCH0.412; DTS should be 11b, Don't know SDDS.
        if(__SetupInfo.bAudioDigitalOut == SETUP_AUDIO_DIGITAL_OUT_SPDIF_PCM)
        {
            //Kevin1.05-3, modify
            //_bSpDifMode = 0;  //pcm
            if(__dwSupportFeature & SUPPORT_FEATURE_DTS)
            {
                _bSpDifMode = 0;  //pcm
            }
            else
            {
                //Kevin1.10, still show SPDIF pcm in SETUP Digital Output menu
                //__SetupInfo.bAudioDigitalOut = SETUP_AUDIO_DIGITAL_OUT_SPDIF_RAW;
                _bSpDifMode = 1;  //raw
            }
        }
        else if(__SetupInfo.bAudioDigitalOut == SETUP_AUDIO_DIGITAL_OUT_SPDIF_RAW)
        {
            _bSpDifMode = 1;  //raw
        }
        else
        {
            _bSpDifMode = 2;  //off
        }
        break; // HAL_AUDIO_DTS

    default:
        break;
    }
}

//  *********************************************************************
//  Function    :   _SetDualMode
//  Description :   It will set variable _bDualMode according to __bModeChannel
//  Arguments   :   None
//  Return      :   None
//  Change      :
//  Side Effect :
//  *********************************************************************
//#pragma NOAREGS //Kevin1.23, may be called by ISR
void _SetDualMode(void)
{
    // TCC275, check if MP3 is mono, must not set it to RR or no audio output. Force set it to LR (same as LL)
    if((__bAttrPlay == ATTR_MP3) || (__bAttrPlay == ATTR_MP2)) //Alex1.10, support MP2 format audio
    {
        HAL_ReadAM(HAL_AM_MP3MP2_CH_NO, &__dwTemp);

        if(__dwTemp == 1)      // Mono
        {
            _bDualMode = DUAL_STEREO;
            return;
        }
    }

    // TCC022, Dual mode is LR/LL/RR/Mixed
    if((__bModeChannel == AUDIO_LR) || (__bModeChannel == AUDIO_RL))
    {
        _bDualMode = DUAL_STEREO;
    }
    else if(__bModeChannel == AUDIO_LL)
    {
        _bDualMode = DUAL_LEFTMONO;
    }
    else if(__bModeChannel == AUDIO_RR)
    {
        _bDualMode = DUAL_RIGHTMONO;
    }
#ifdef DOLBY_CERTIFICATION //Kevin0.76a
    else if(__bModeChannel == AUDIO_MIXED)
    {
        _bDualMode = DUAL_MIXEDMONO;
    }
#endif
}

// Kevin2.81, create
//  *******************************************************************************
//  Function    :   _CHIPS_FilterFunSupport
//  Description :   Filter if the desired function is supported or not
//  Arguments   :   bType, the desired function
//  Return      :   TRUE: supported
//                  FALSE: not supported
//  Side Effect :   none
//  *******************************************************************************
//#pragma NOAREGS // called in ISR  //Kevin1.24
BYTE _CHIPS_FilterFunSupport(BYTE bType)
{
    //Kevin0.87, modify to fixed "It displays "invalid" in File Manager when KEY_VOL_UP/KEY_VOL_DOWN even the cursor is at the MP3 file.
    //if (__bAttrPlay == ATTR_JPG)
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    if (!__bMMJPEGAudioMode)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
    {
        if((__bAttrPlay == ATTR_JPG) && (__btPlaying || __btPause) )
            return FALSE;
    }

    switch(bType)
    {
    case CHIPS_VOLUMECONTROL:
    case CHIPS_SOUNDEFFECT:
    //Kevin1.23, comment
    case CHIPS_OPENAUDIO:    //Kevin0.87, add
        return TRUE;
        break;

    //Kevin0.90, modify
    case CHIPS_ECHO:         //except DTS
        if(_bAudType != AUDIO_DTS)
            return TRUE;
        break;

    //Kevin1.24, let Prologic & Surround mutual exclusive, otherwise there may be noise
    case CHIPS_SURROUND:
        //kevin0.69, support surround for all audio types
        //if ( _bAudType!=AUDIO_DTS &&
        if((__SetupInfo.bAudioPrologic == SETUP_AUDIO_PROLOGIC_OFF) || (__SetupInfo.bAudioAnalogOut == SETUP_AUDIO_ANALOG_OUT_2CH))
            return TRUE;
        break;

    case CHIPS_KEYCONTROL:
        //kevin0.69, support keyshift for all audio types
        /*
        if ((_bAudType==AUDIO_AC3) ||    //dvd/vcd karaoke
        //((_bAudType==AUDIO_MPG) && (__bAttrPlay != ATTR_MP3)) )
        //Alex1.10, support MP2 format audio
        ((_bAudType==AUDIO_MPG) && (__bAttrPlay != ATTR_MP3) && (__bAttrPlay != ATTR_MP2) && (__bAttrPlay != ATTR_JPG)))
        */
        //kevin1.10, not allow keyshift when doing language study
        // wyc2.17-909S, for language study.
#if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)
        if(__bLangStudyFlag == TRUE)
            return FALSE;
#endif
        return  TRUE;
        break;

    default:
        break;
    }

	return FALSE;
}

//  *********************************************************************
//  Function    :   CHIPS_Initial
//  Description :   It will initial the global variables set to w9922qf
//              :   to it's default value.
//              :   Will write the variables to chip only when issue CHIPS_OpenAudio
//  Arguments   :   wInit:      The type to initial (Can use || to combine settings)
//                      CHIPS_INIT_KEY : key control
//                      CHIPS_INIT_SURROUND : surround type
//                      CHIPS_INIT_ECHO : echo level
//                      CHIPS_INIT_3DLEVEL : 3d level
//                      CHIPS_INIT_VOL : volume level
//                      CHIPS_INIT_CHANNELMODE : channel mode
//                      CHIPS_INIT_AC3KOKMODE : ac3 karaoke mode
//                      CHIPS_INIT_DUALMODE : ac3 dual mode
//                      CHIPS_INIT_AUDTYPE : audio type
//                      CHIPS_INIT_ALL : all above settings
//                      CHIPS_INIT_BYPASS : bypass all setting
//  Return      :   None
//  Change      :
//  Side Effect :
//  *********************************************************************
void CHIPS_Initial(WORD wInit)
{
    if(wInit != CHIPS_INIT_BYPASS)
    {
        if(wInit & CHIPS_INIT_KEY)     // Initial KEY value
        {
            __cKeyShift = CHIPS_DEFAULT_KEY;
            HAL_WriteAM(HAL_AM_KEYSHIFT_EN, 0); //kevin1.07
        }

        if(wInit & CHIPS_INIT_SURROUND)        // Initial Surround Type
        {
            __bSurround = CHIPS_DEFAULT_SURROUND;

#ifdef SUPPORT_USER_DEFINED_EQ
            for(__bBandSelect=BAND_SELECT_BEGIN; __bBandSelect<=BAND_SELECT_END; __bBandSelect++)
            {
                __bPerAmp[__bBandSelect] = DEFAULT_PER_BAND_AMP;
            }
            __bBandSelect=BAND_SELECT_BEGIN;
#endif
        }

        if(wInit & CHIPS_INIT_ECHO)    // Initial Echo level
        {
            __bEchoLevel = CHIPS_DEFAULT_ECHO;
        }

         if(wInit & CHIPS_INIT_VOL)     // Initial Voice Level
        {
#ifdef CT950_STYLE
            __dwVolAudio = (DWORD)DVDSETUP_GetVolume();
#else
            __dwVolAudio = CHIPS_DEFAULT_VOL;
#endif //CT950_STYLE
			__dwPreVolAudio = 0;
#ifndef NO_MIC_INPUT
            __wMICVol = MIC_VOL_DEFAULT; //kevin1.01
#endif
        }

        if(wInit & CHIPS_INIT_CHANNELMODE)     // Initial Channel Mode
        {
            switch(__SetupInfo.bAudioAnalogOut)
            {
            case SETUP_AUDIO_ANALOG_OUT_2CH:
                _bLFE = 0;
                // J500KWShih_220, set down mix mode for SD0
                if(__SetupInfo.bAudioDownMix == SETUP_AUDIO_DOWNMIX_LORO)
                {
                    _bChannelMode = CHIPS_LR;
                }
                else
                {
                    _bChannelMode = CHIPS_LRD;
                }
                break;

            case SETUP_AUDIO_ANALOG_OUT_51CH:
                if(__SetupInfo.bAudioCenter == SETUP_AUDIO_CENTER_ON)
                {
                    if(__SetupInfo.bAudioRear == SETUP_AUDIO_REAR_ON)
                    {
                        _bChannelMode = CHIPS_LCRLSRS;
                    }
                    else
                    {
                        _bChannelMode = CHIPS_LCR;
                    }
                }
                else
                {
                    if(__SetupInfo.bAudioRear == SETUP_AUDIO_REAR_ON)
                    {
                        _bChannelMode = CHIPS_LRLSRS;
                    }
                    else
                    {
                        _bChannelMode = CHIPS_LRD;
                    }
                }
                if(__SetupInfo.bAudioSubwoofer == SETUP_AUDIO_SUBWOOFER_ON)
                {
                    _bLFE = 1;
                }
                else
                {
                    _bLFE = 0;
                }
                break ;

            default:
                break;
            }

            switch(__SetupInfo.bAudioDigitalOut)
            {
            case SETUP_AUDIO_DIGITAL_OUT_SPDIF_PCM:
                _bSpDifMode = 0;	//pcm
                break;

            case SETUP_AUDIO_DIGITAL_OUT_SPDIF_RAW:
                _bSpDifMode = 1;	//raw
                break;

            case SETUP_AUDIO_DIGITAL_OUT_OFF:
                _bSpDifMode = 2;	//off
                break;

            default:
                break;
            }
        }

        if(wInit & CHIPS_INIT_AC3KOKMODE)      // Initial AC3 Karaoke Mode
        {
            __bVocal = CHIPS_DEFAULT_AC3KOKMODE;
        }
        if(wInit & CHIPS_INIT_DUALMODE)        // Initial Dual Mode
        {
            _SetDualMode();
        }
        if(wInit & CHIPS_INIT_AUDTYPE) // Initial Audio Type
        {
            _SetAudio(__bAudioType);
        }
    }
}

//  *********************************************************************
//  Function    :   CHIPS_KeyControl
//  Description :   It will do key control according to the parameter
//  Arguments   :   bKey:      The type of command
//                      CHIPS_UP : control up 1 level
//                      CHIPS_DOWN : control down 1 level
//                      CHIPS_NORMAL : set to default level
//                      CHIPS_TOGGLE : toggle between levels in range
//                      CHIPS_LEVEL : return current level
//                      CHIPS_SUPPORT : return if this function supported
//  Return      :   TRUE : Supported/Succeeded
//              :   FALSE : Not supported
//  Change      :   Will turn off surround.
//                  Will open audio if mute
//  Side Effect :   Only MPEG1 and AC3 support key control
//  *********************************************************************
BYTE CHIPS_KeyControl(BYTE bKey)
{
    BOOL fKey = TRUE;

    //Kevin2.81, Filter if the desired function is supported or not in one file
    if(!_CHIPS_FilterFunSupport(CHIPS_KEYCONTROL))
    {
        return FALSE;
    }

    // key-up/down will open volume
    if(__btMute)
    {
        // J500KWShih_218, fixed mantis bug 1400 and 1563,
        HAL_IOMute(FALSE);
       __btMute = FALSE;
       OSD_OUTPUT_MACRO(MSG_MUTE, __btMute, 0);
    }

    switch(bKey)
    {
    case CHIPS_UP:
        __cKeyShift++;
        if(__cKeyShift > NORMALKEY_OFFSET)
        {
            fKey = FALSE;
            __cKeyShift = NORMALKEY_OFFSET;
        }
        break;

    case CHIPS_DOWN:
        __cKeyShift--;
        if(__cKeyShift < (-1*(char)NORMALKEY_OFFSET))
        {
            fKey = FALSE;
            __cKeyShift = -1 * (char)NORMALKEY_OFFSET;
        }
        break;

    case CHIPS_NORMAL:
        if(__cKeyShift == 0)
        {
            fKey = FALSE;
        }
        else
        {
            __cKeyShift = 0;
        }
        break ;

    default:
        break;
    }

    // J500KWShih_221a, if KEY does not need to change, we will not close/open audio.
    if(fKey)
    {
        //Kevin1.01, avoid the bo noise when pressing key +/-
        CHIPS_OpenAudio(FALSE);

        //kevin1.07, move form CHIPS_OpenAudio
        if(__cKeyShift < 0) // from -1 to -7
        {
            _wCHIPSTmp = -(__cKeyShift) | 0x8;
        }
        else // from 0 to 7
        {
            _wCHIPSTmp = __cKeyShift;
        }
        HAL_WriteAM(HAL_AM_KEYSHIFT, _wCHIPSTmp);

        //kevin0.72, PCM / DTS also supports keyshift
        if(__cKeyShift != 0)
        {
            HAL_WriteAM(HAL_AM_KEYSHIFT_EN, 1);
        }
        else
        {
            HAL_WriteAM(HAL_AM_KEYSHIFT_EN, 0);
        }

        CHIPS_OpenAudio(TRUE);
    }

	OSD_OUTPUT_MACRO(MSG_KEY, __cKeyShift+NORMALKEY_OFFSET, 1);

    return TRUE;
}

//  *********************************************************************
//  Function    :   CHIPS_ProLogicControl
//  Description :   set ProLogic|| DM
//  Arguments   :   None
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************
//Kevin1.08, create
void CHIPS_ProLogicControl(void)
{
    //#define         W99AV_DM_PROLOGIC_ENABLE        0x0175c //1:enable  0:disable (If src > 2 ch, enable will be of no use)
    //#define         W99AV_DM_PROLOGIC_OCFG          0x00de2
    //bit[2:0] chancfg
    //    011: L,R,C
    //    110: L,R,Ls,Rs
    //    111: L,R,Ls,Rs,C
    //bit[5:3] decmode
    //    000: ProLogic I
    //    001: Virtual
    //    010: Music
    //    011: Movie
    //    100: Matrix
    //bit[7:6] samprate
    //    00 : 48K
    //    01 : 44.1K
    //    10 : 32K
    //#define         W99AV_DM_PROLOGIC_OCTRL         0x00de4 //default : 0x1b0

    DWORD dwTemp;
    BYTE bTemp;

    //Kevin1.24, add protection
    if(!(__dwSupportFeature & SUPPORT_FEATURE_PROLOGIC))
    {
        __SetupInfo.bAudioPrologic = SETUP_AUDIO_PROLOGIC_OFF;
    }

    switch(__SetupInfo.bAudioPrologic)
    {
    case SETUP_AUDIO_PROLOGIC_PROLOGIC:
        dwTemp = 0 << 3;
        bTemp = 1;
        break;

    case SETUP_AUDIO_PROLOGIC_VIRTUAL:
        dwTemp = 1 << 3;
        bTemp = 0;
        break;

    case SETUP_AUDIO_PROLOGIC_MUSIC:
        dwTemp = 2 << 3;
        bTemp = 0;
        break;

    case SETUP_AUDIO_PROLOGIC_MOVIE:
        dwTemp = 3 << 3;
        bTemp = 1;
        break;

    case SETUP_AUDIO_PROLOGIC_MATRIX:
        dwTemp = 4 << 3;
        bTemp = 0;
        break;

    case SETUP_AUDIO_PROLOGIC_OFF:
    default:
        HAL_WriteAM(HAL_AM_PROLOGIC_ENABLE, 0);
        return;
    }

    switch(__SetupInfo.bAudioAnalogOut)
    {
    case SETUP_AUDIO_ANALOG_OUT_2CH: //Kevin1.10-2, disable Prologic for 2CH output
        HAL_WriteAM(HAL_AM_PROLOGIC_ENABLE, 0);
        return;

    case SETUP_AUDIO_ANALOG_OUT_51CH:
        if(__SetupInfo.bAudioCenter==SETUP_AUDIO_CENTER_ON && __SetupInfo.bAudioRear==SETUP_AUDIO_REAR_ON)
        {
            dwTemp |= 7;
        }
        else if(__SetupInfo.bAudioCenter==SETUP_AUDIO_CENTER_ON && __SetupInfo.bAudioRear==SETUP_AUDIO_REAR_OFF)
        {
            dwTemp |= 3;
        }
        else if(__SetupInfo.bAudioCenter==SETUP_AUDIO_CENTER_OFF && __SetupInfo.bAudioRear==SETUP_AUDIO_REAR_ON)
        {
            dwTemp |= 6;
        }
        else
        {
            HAL_WriteAM(HAL_AM_PROLOGIC_ENABLE, 0);
            return;
        }

    default:
        break;
    }

    HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &__dwTemp);

    //Kevin1.20, use the most close freq settings
    if(__dwTemp < AUDIO_FREQ_44K)
    {
        dwTemp |= (2 << 6); //32K
    }
    else if(__dwTemp < AUDIO_FREQ_48K)
    {
        dwTemp |= (1 << 6); //44K
    }
    //else
        //dwTemp |= (0<<6); //48K

    /* //kevin909, add HAL_AM_PROLOGIC_SUR_DELAY later
    //Kevin2.37, conflicts w/ AC3
    //HAL_WriteDM(HAL_AM_PROLOGIC_SUR_DELAY, (DWORD)bTemp);
    HAL_WriteAM(HAL_AM_PROLOGIC_SUR_DELAY, 0);
    */
    HAL_WriteAM (HAL_AM_PROLOGIC_CTRL, 0x1b0);
    HAL_WriteAM (HAL_AM_PROLOGIC_CFG, dwTemp);  //kevin0.69
    HAL_WriteAM(HAL_AM_PROLOGIC_ENABLE, 1);

    //Kevin1.24, let Prologic & Surround mutual exclusive
    if(__bSurround)
    {
       __bSurround = 0;
       OSD_Output(MSG_SURROUND, __bSurround, 0x0);
    }
}

//  *********************************************************************
//  Description :   set HDCD DM
//  Arguments   :   None
//  Return      :   None
//  *********************************************************************
void CHIPS_ControlHDCD(void)
{
	if( (__bAttrPlay==ATTR_CDDA) && ( __bCDDA_DTS==CDDA_PURE || __bCDDA_DTS==CDDA_HDCD || __bCDDA_DTS==CDDA_CDG )) //kevin0.80-2, add ATTR_CDDA protection
    {
        HAL_WriteAM(HAL_AM_HDCD_CTRL, 0x613c);

        HAL_WriteAM(HAL_AM_PCM_MODE, 1);
    }

    //HDCD + Prologic => performance issue
    if(__SetupInfo.bAudioPrologic != SETUP_AUDIO_PROLOGIC_OFF)
    {
        HAL_WriteAM(HAL_AM_PCM_MODE, 1);
    }
}

//  *********************************************************************
//  Function    :   CHIPS_Surround
//  Description :   It will do surround according to the parameter
//  Arguments   :   bKey:      The type of command
//                      CHIPS_UP : control up 1 level
//                      CHIPS_DOWN : control down 1 level
//                      CHIPS_NORMAL : set to default level
//                      CHIPS_TOGGLE : toggle between levels in range
//                      CHIPS_LEVEL : return current level
//                      CHIPS_SUPPORT : return if this function supported
//  Return      :   TRUE : Supported/Succeeded
//              :   FALSE : Not supported
//  Change      :   Will turn off key control
//  Side Effect :   Only MP3 doesn't support surround
//  *********************************************************************
BYTE CHIPS_Surround(BYTE bSurd)
{
    //Kevin2.81, Filter if the desired function is supported or not in one place
    if(!_CHIPS_FilterFunSupport(CHIPS_SURROUND))
    {
        return FALSE;
    }

    HAL_WriteAM(HAL_AM_PROLOGIC_ENABLE, 0);

    //Kevin0.86, add to fix the "KEY_SURROUND but mute is not canceled" bug
    // surround will open volume
    if(__btMute)
    {
        // J500KWShih_218, fixed mantis bug 1400 and 1563,
        HAL_IOMute(FALSE);
       __btMute = FALSE;
       OSD_OUTPUT_MACRO(MSG_MUTE, __btMute, 0);
    }

    switch(bSurd)
    {
    case CHIPS_UP:
        if(__bSurround < CHIPS_SURROUND_TYPE)
        {
            __bSurround++;
        }
        break;

    case CHIPS_DOWN:
        if(__bSurround > CHIPS_SURROUND_NONE)
        {
            __bSurround--;
        }
        break;

    case CHIPS_NORMAL:
        __bSurround = CHIPS_SURROUND_NONE;
        break ;

    case CHIPS_TOGGLE:
        __bSurround++;
        __bSurround %= (CHIPS_SURROUND_TYPE + 1);
        break;

    case CHIPS_LEVEL:
        return __bSurround;

    case CHIPS_SUPPORT:
        return TRUE;

    default:
        break;
    }

    CHIPS_OpenAudio(TRUE);

    OSD_OUTPUT_MACRO(MSG_SURROUND, __bSurround, 0x3);

    return TRUE ;
}

//  *********************************************************************
//  Function    :   CHIPS_Echo
//  Description :   It will do echo according to the parameter
//                  w9922qf doesn't support echo
//  Arguments   :   bEcho:      The type of command
//                      CHIPS_UP : control up 1 level
//                      CHIPS_DOWN : control down 1 level
//                      CHIPS_NORMAL : set to default level
//                      CHIPS_TOGGLE : toggle between levels in range
//                      CHIPS_LEVEL : return current level
//                      CHIPS_SUPPORT : return if this function supported
//  Return      :   TRUE : Supported/Succeeded
//              :   FALSE : Not supported
//  Change      :
//  Side Effect :
//  *********************************************************************
BYTE CHIPS_Echo(BYTE bEcho)
{
    //Kevin2.81, Filter if the desired function is supported or not in one place
    if(!_CHIPS_FilterFunSupport(CHIPS_ECHO))
    {
        return FALSE;
    }

    switch(bEcho)
    {
    case CHIPS_UP:
        if(++__bEchoLevel > ECHO_LEVEL)
        {
            __bEchoLevel = ECHO_LEVEL;
        }
        break;

    case CHIPS_DOWN:
        if(__bEchoLevel > 0)
        {
            __bEchoLevel--;
        }
        break;

    case CHIPS_NORMAL:
        __bEchoLevel = CHIPS_DEFAULT_ECHO;
        break;

    case CHIPS_TOGGLE:
        break;

    case CHIPS_LEVEL:
        return __bEchoLevel;

    case CHIPS_SUPPORT:
        return TRUE;

    default:
        break;
    }

    //Kevin1.05, modify
	//W99AV_WriteDM(W99AV_DM_ECHO_CR0, (((WORD)0x5B*__bEchoLevel/ECHO_LEVEL)<<4) | 0x4 );  //scaling factor = 4/16
	if(!__bEchoLevel)
    {
        HAL_WriteAM(HAL_AM_ECHO_CR, 0x808);
    }
	else
    {
        HAL_WriteAM(HAL_AM_ECHO_CR, (((WORD)0x5B*__bEchoLevel/ECHO_LEVEL)<<4) | 0x8 );  //scaling factor = 8/16
    }

    OSD_OUTPUT_MACRO(MSG_ECHO, __bEchoLevel, 0x3);

    return TRUE;
}

//  *********************************************************************
//  Function    :   CHIPS_DownMixMode
//  Description :   set AC3 downmix mode at run time
//  Arguments   :   bMode:
//                      DOWNMIX_LORO : LoRo
//                      DOWNMIX_LTRT : LtRt
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************
//Kevin1.07, create
void CHIPS_DownMixMode(BYTE bMode)
{
    switch(bMode)
    {
    case AC3_DOWNMIX_LORO:
        HAL_WriteAM(HAL_AM_AC3_DOWNMIX_MODE, 0);
        // J500KWShih_220, set down mix mode for SD0
        if(__SetupInfo.bAudioAnalogOut == SETUP_AUDIO_ANALOG_OUT_2CH)
        {
            //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Lo/Ro");
            HAL_WriteAM(HAL_AM_CHANNEL_MODE, CHIPS_LR);
        }
        break;

    case AC3_DOWNMIX_AUTO:
        HAL_WriteAM(HAL_AM_AC3_DOWNMIX_MODE, 2);
        break ;

    case AC3_DOWNMIX_LTRT:
    default :
        HAL_WriteAM(HAL_AM_AC3_DOWNMIX_MODE, 1);
        // J500KWShih_220, set down mix mode for SD0
        if(__SetupInfo.bAudioAnalogOut == SETUP_AUDIO_ANALOG_OUT_2CH)
        {
            //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Lt/Rt");
            HAL_WriteAM(HAL_AM_CHANNEL_MODE, CHIPS_LRD);
        }
        break;
    }
}

//  *********************************************************************
//  Function    :   CHIPS_MICControl
//  Description :   Toggle between mic effect levels
//                  w9922qf doesn't support mic control
//  Arguments   :   bON :
//                      CHIPS_TOGGLE, toggle between mic level
//  Return      :   TRUE : supported and carried out
//                  FALSE : Not supported
//  Change      :
//  Side Effect :
//  *********************************************************************
BYTE CHIPS_MICControl(BYTE bON)
{
// DVD_102Micky, support MIC on/off
#ifdef SUPPORT_MIC_CONTROL
    if(bON == CHIPS_ON)
        __sbMuteMic = 1;
    else if(bON == CHIPS_OFF)
        __sbMuteMic = 0;
#else //#ifdef SUPPORT_MIC_CONTROL
#ifdef SUPPORT_MIC_CONTROL_INV
    if(bON == CHIPS_ON)
        __sbMuteMic = 0;
    else if(bON == CHIPS_OFF)
        __sbMuteMic = 1;
#else //#ifdef SUPPORT_MIC_CONTROL_INV
    return FALSE;
#endif //#ifdef SUPPORT_MIC_CONTROL_INV
#endif //#ifdef SUPPORT_MIC_CONTROL

    return TRUE;
}

//  *********************************************************************
//  Function    :   CHIPS_VolumeControl
//  Description :   It will do volume level control according to the parameter
//  Arguments   :   bFunction:      The type of command
//                      CHIPS_UP : control up 1 level
//                      CHIPS_DOWN : control down 1 level
//                      CHIPS_NORMAL : set to default level
//                      CHIPS_TOGGLE : toggle between levels in range
//                      CHIPS_LEVEL : return current level
//                      CHIPS_SUPPORT : return if this function supported
//  Return      :   TRUE : Supported/Succeeded
//              :   FALSE : Not supported
//  Change      :   Will open audio if mute
//  Side Effect :
//  *********************************************************************
DWORD CHIPS_VolumeControl(BYTE bFunction)
{
    //Kevin2.81, Filter if the desired function is supported or not in one place
    if (!_CHIPS_FilterFunSupport(CHIPS_VOLUMECONTROL))
    {
        return FALSE;
    }

    switch(bFunction)
    {
    case CHIPS_UP:
        //_TuneAudioVolume(TRUE);
        //Kevin2.81 delete _TuneAudioVolume()
        __dwVolAudio += VOLUME_GAP;
        if(__dwVolAudio > VOLUME_MAX)
        {
            __dwVolAudio= VOLUME_MAX;
        }
//#ifdef CT950_STYLE
 //       DVDSETUP_SaveVolume();
//#endif //CT950_STYLE
        break;

    case CHIPS_DOWN:
        //_TuneAudioVolume(FALSE);
        //Kevin2.81 delete _TuneAudioVolume()
        if( __dwVolAudio < VOLUME_MIN + VOLUME_GAP)
        {
            __dwVolAudio = VOLUME_MIN;
        }
        else
        {
            __dwVolAudio -= VOLUME_GAP;
        }
//#ifdef CT950_STYLE
//        DVDSETUP_SaveVolume();
//#endif //CT950_STYLE
        break;

    case CHIPS_LEVEL:
        return __dwVolAudio;

    case CHIPS_NORMAL:
        __dwVolAudio = VOLUME_DEFAULT;
#if 0// def 0//CT950_STYLE
        DVDSETUP_SaveVolume();
#endif //CT950_STYLE
        break;

    case CHIPS_TOGGLE:
        break;

    case CHIPS_SUPPORT:
        return TRUE;

    default:
        return FALSE;
    } // switch(bFunction)

//DVD018
    if(__btMute)
    {
        //DVD018
        // J500KWShih_218, fixed mantis bug 1400 and 1563,
        HAL_IOMute(FALSE);
       __btMute = FALSE ;
       OSD_OUTPUT_MACRO(MSG_MUTE, __btMute, 0);
    }

    CHIPS_OpenAudio(TRUE);
    OSD_OUTPUT_MACRO(MSG_VOLUME, (__dwVolAudio / VOLUME_GAP), 3);

    return TRUE;
}

#ifndef NO_MIC_INPUT
//  *********************************************************************
//  Function    :   CHIPS_MICVolControl
//  Description :   Do MIC volume level control
//  Arguments   :   bFunction:      The type of command
//                      CHIPS_UP : control up 1 level
//                      CHIPS_DOWN : control down 1 level
//                      CHIPS_NORMAL : set to default level
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************
void    CHIPS_MICVolControl( BYTE bFunction)
{
    switch(bFunction)
    {
        case    CHIPS_UP:
                __wMICVol += MIC_VOL_GAP ;
                if ( __wMICVol > MIC_VOL_MAX )
                    __wMICVol= MIC_VOL_MAX;
                break;

        case    CHIPS_DOWN:
                if ( __wMICVol < MIC_VOL_MIN + VOLUME_GAP)
                    __wMICVol = MIC_VOL_MIN;
                else
                    __wMICVol -= MIC_VOL_GAP;
                break;

        case    CHIPS_NORMAL :
                __wMICVol = MIC_VOL_DEFAULT ;
                break;
    }

    HAL_WriteAM(HAL_AM_MIC_PCM_SCALE, __wMICVol);

    OSD_OUTPUT_MACRO(MSG_VOLUME, (__wMICVol / MIC_VOL_GAP), 3 ); //kevin909, remove _bVolOrMIC & add MSG_MICVOLUME later
}
#endif

//  *********************************************************************
//  Function    :   CHIPS_ChannelSwitch
//  Description :   It will set the desired channel mode
//  Arguments   :   bCh:      The type of channel
//                      CHIPS_LRD : Audio is Stereo (LR) Dolby Surround Compatible
//                      CHIPS_C : Audio is Center
//                      CHIPS_LR : Audio is LR
//                      CHIPS_LCR : Audio is LCR
//                      CHIPS_LRLS : Audio is LRLs
//                      CHIPS_LCRLS : Audio is LCRLs
//                      CHIPS_LRLSRS : Audio is LRLsRs
//                      CHIPS_LCRLSRS : Audio is LCRLsRs
//  Return      :   TRUE : Succeeded
//              :   FALSE : Out of range
//  Change      :
//  Side Effect :
//  *********************************************************************
#if 0   // TCC274, _bChannelMode should reference setupinfo.
BYTE    CHIPS_ChannelSwitch(BYTE bCh)
{
    if (bCh > CHIPS_LCRLSRS)
        return FALSE ;

    _bChannelMode = bCh ;

    CHIPS_OpenAudio(TRUE);
    //OSD_OUTPUT_MACRO(MSG_CHANNEL, bCh, 3 );
    return TRUE;
}
#endif
//  *********************************************************************
//  Function    :   CHIPS_LFEEnable
//  Description :   It will turn LFE on/off
//  Arguments   :   bParam:      The type of channel
//                      TRUE : Turn LFE on
//                      FALSE : Turn LFE off
//  Return      :   None
//  Change      :
//  Side Effect :
//  *********************************************************************
#if 0   // TCC274, _bLFE should reference setupinfo.
void    CHIPS_LFEEnable(BYTE bParam)
{
    if (bParam)
        _bLFE = 1 ;
    else
        _bLFE = 0 ;
    CHIPS_OpenAudio(TRUE);
    //OSD_OUTPUT_MACRO(MSG_LFE, _bLFE, 3 );
}
#endif
//  *********************************************************************
//  Function    :   CHIPS_AC3KokMode
//  Description :   It will do ac3 karaoke control according to the parameter
//                  Modes:
//                      VOCAL_NOVOCAL
//                      VOCAL_LEFT
//                      VOCAL_RIGHT
//                      VOCAL_STEREO
//  Arguments   :   bFunction:      The type of command
//                      CHIPS_UP : control up 1 level
//                      CHIPS_DOWN : control down 1 level
//                      CHIPS_NORMAL : set to default level
//                      CHIPS_TOGGLE : toggle between levels in range
//                      CHIPS_LEVEL : return current level
//                      CHIPS_SUPPORT : return if this function supported
//  Return      :   TRUE : Supported/Succeeded
//              :   FALSE : Not supported
//  Change      :
//  Side Effect :
//  *********************************************************************
BYTE CHIPS_AC3KokMode(BYTE bMode)
{
    // TCC050a, only AC3 support vocal.
    if(_bAudType != AUDIO_AC3)
    {
        return FALSE;
    }

    switch(bMode)
    {
    case CHIPS_UP:
        if(__bVocal < VOCAL_ON)
        {
            __bVocal++;
        }
        break;

    case CHIPS_DOWN:
        if(__bVocal > VOCAL_OFF)
        {
            __bVocal--;
        }
        break ;

    case CHIPS_NORMAL:
        __bVocal = 0;
        break ;

    case CHIPS_TOGGLE:
// TCC104, if defined ENABLE_AUTOVOCAL, only has 2 modes (On/Off)
#ifdef ENABLE_AUTOVOCAL
        if(__bVocal == VOCAL_OFF)
        {
            __bVocal = VOCAL_ON;
        }
        else
        {
            __bVocal = VOCAL_OFF;
        }
#else
        __bVocal++;
        __bVocal %= 4;
#endif
        break;

    case CHIPS_LEVEL:
        return __bVocal;

    case CHIPS_SUPPORT:
        return TRUE;

    default:
        break;
    }

    CHIPS_OpenAudio(TRUE);

#ifdef SUPPORT_PRINTF
    switch(__bVocal)
    {
    case VOCAL_OFF:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AC3 No Vocal\n");
        break;
    case VOCAL_LL:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AC3 Left Vocal\n");
        break;
    case VOCAL_RR:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AC3 Right Vocal\n");
        break;
    case VOCAL_ON:
    default:
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AC3 Stereo Vocal\n");
        break;
    }
#endif

    // Current no suitable OSD to show
    OSD_OUTPUT_MACRO(MSG_VOCAL, __bVocal, 3);

    return TRUE;
}

//  *********************************************************************
//  Function    :   CHIPS_DualMode
//  Description :   It will do ac3 dual mode control according to the parameter
//                  Modes:
//                  DUAL_STEREO
//                  DUAL_LEFTMONO
//                  DUAL_RIGHTMONO
//                  DUAL_MIXEDMONO
//  Arguments   :   bFunction:      The type of command
//                      CHIPS_UP : control up 1 level
//                      CHIPS_DOWN : control down 1 level
//                      CHIPS_NORMAL : set to default level
//                      CHIPS_TOGGLE : toggle between levels in range
//                      CHIPS_LEVEL : return current level
//                      CHIPS_SUPPORT : return if this function supported
//  Return      :   TRUE : Supported/Succeeded
//              :   FALSE : Not supported
//  Change      :
//  Side Effect :
//  *********************************************************************
#if 0   // TCC274, _bDualMode should reference __bModeChannel.
BYTE    CHIPS_DualMode(BYTE bMode)
{
    switch(bMode)
    {
        case CHIPS_UP :
            if (_bDualMode < DUAL_MIXEDMONO)
                _bDualMode ++ ;
            break ;

        case CHIPS_DOWN :
            if (_bDualMode > DUAL_STEREO)
                _bDualMode -- ;
            break ;

        case CHIPS_NORMAL :
            _bDualMode = 0 ;
            break ;

        case CHIPS_TOGGLE :
            _bDualMode ++ ;
            _bDualMode %= 4 ;
            break ;

        case CHIPS_LEVEL :
            return _bDualMode ;

        case CHIPS_SUPPORT :
            return TRUE ;

        default :
            break ;
    }

    CHIPS_OpenAudio(TRUE);
    //OSD_OUTPUT_MACRO(MSG_DUAL, _bDualMode, 3 );
    return TRUE;
}
#endif

//  *********************************************************************
//  Function    :   _ChangeDSPCode
//  Description :   It will load correct dsp code according to the desired audio type
//                  This function is mainly called by _SetAudType function
//                  Only BA5580 need to change dsp code
//  Arguments   :   bType:      The type of command (Reference HAL_SetAudioType)
//  Return      :   None
//  Change      :
//  Side Effect :
//  *********************************************************************
void _ChangeDSPCode(BYTE bType)
{
//Kevin909, runtime unzip separate Audio codes
#ifdef RUNTIME_UNZIP_AUDIO_CODE
    DWORD dwTempReg;

    // LLY2.55, remove it since current using PROC2 code will be kept in __bCurProc2Code
    //static BYTE _bCurAudioType = HAL_AUDIO_NONE;

    switch(bType)
    {
    case HAL_AUDIO_MPG:
    case HAL_AUDIO_MP3:
    case HAL_AUDIO_MP2:
        bType = HAL_AUDIO_MPG;
        break;

    default:
        break;
    }

    // LLY2.55, don't compare desired PROC2 code w/ _bCurAudioType
    // Since, current F/W will call HAL_ReloadAudioDecoder() directly w/o keeping current using code
    // So, the comparsion statement maybe worng.
    // ex. DMP solution: LOGO (JPEG code) --> play MP3 + JPEG (load MP3 code by call HAL_ReloadAudioDecoder() directly)
    //     --> KEY_FUNCTION to return main (it's JPEG logo) ==> will be think as same PROC2 code.
	/*
	if((bType == _bCurAudioType) || (bType == HAL_AUDIO_NONE))
        return;
	*/
	if( (bType == __bCurProc2Code) || (bType == HAL_AUDIO_NONE) )
	{
        return;
	}

#if 0
    switch(bType)
    {
    case HAL_AUDIO_MPG:
    case HAL_AUDIO_MP3:
    case HAL_AUDIO_MP2:
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 2);
        break; // MPEG

    case HAL_AUDIO_PCM:
    case HAL_AUDIO_LPCM:
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 3);
        break; // PCM

    case HAL_AUDIO_DTS:
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 4);
        if(__wDiscType & BOOK_DVD)
        {
            HAL_WriteAM(HAL_AM_DTS_MODE, 0);
        }
        else if(__wDiscType & BOOK_CDDA)
        {
            HAL_WriteAM(HAL_AM_DTS_MODE, 1);
        }
        break; // DTS

    case HAL_AUDIO_WMA:
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 6);
        break; // WMA

        // LLY2.17, add for AAC audio stream
    case HAL_AUDIO_AAC:
        break; // AAC

    case HAL_AUDIO_AC3:
    default:
        HAL_WriteAM(HAL_AM_AUDIO_TYPE, 1);
        break; // default audio code is AC3
    }
#endif

    // LLY2.55, remove it since current using PROC2 code will be kept in __bCurProc2Code
    // within HAL_ReloadAudioDecoder()
	//_bCurAudioType = bType;

    //for stop: may be removed if audio decoder doesn't check audio type
	HAL_Reset(HAL_RESET_AUDIO);

    //senshong2.38a: JPEG decoder use REG_AIU_GR(2) ~ REG_AIU_GR(17) to connect width processor 2 from processor 1
    //So before playback audio, we need to reset REG_AIU_GR(2) ~ REG_AIU_GR(17).
#ifdef SUPPORT_JPEGDEC_ON_PROC2
    {
        BYTE bIndx;
        for ( bIndx=2; bIndx<17; bIndx++)
        {
            REG_AIU_GR(bIndx)= 0;
        }
    }
#endif

    //reload audio decoder
    // Micky2.11a
    HAL_ReloadAudioDecoder(bType);

    //restart
	HAL_Reset(HAL_RESET_AUDIO);

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

    if(REG_AIU_SPORT_CONFG0 != dwTempReg)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "\nREG_AIU_SPORT_CONFG0 Setting Wrong");
        // LLY2.56, do key lock first since AIU_SPORT_CONFG0 will be accessed by PROC2 together
        MACRO_PLAT_KEY_LOCK( );
        REG_AIU_SPORT_CONFG0 = dwTempReg;
        MACRO_PLAT_KEY_UNLOCK( );
    }

#endif // RUNTIME_UNZIP_AUDIO_CODE
}

//  Kevin1.10, create
//  *********************************************************************
//  Function    :   CHIPS_ControlAnalogOutput
//  Description :   Control Analog Output on/off
//  Arguments   :   None
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************
//#pragma NOAREGS // may be called in ISR (CHIPS_ControlAnalogOutput() -> CHIPS_OpenAudio()-> ISR
void CHIPS_ControlAnalogOutput(void)
{
    //kevin0.91, fix DTS still has analog output for the part No. that doesn't support DTS
    if((__SetupInfo.bAudioAnalogOut == SETUP_AUDIO_ANALOG_OUT_OFF) || ((__bAudioType == HAL_AUDIO_DTS) && !(__dwSupportFeature&SUPPORT_FEATURE_DTS)))
        HAL_WriteAM(HAL_AM_PCM_MUTE, 1);
    else
        HAL_WriteAM(HAL_AM_PCM_MUTE, 0);
}

//  *********************************************************************
//  Function    :   _SetAudType
//  Description :   It will set audio type to register according to the parameter
//                  This function is mainly called by HAL_SetAudioType function
//  Arguments   :   bType:      The type of command (Reference HAL_SetAudioType)
//  Return      :   None
//  Change      :
//  Side Effect :
//  *********************************************************************
void _SetAudType(BYTE bType)
{
    //WORD wTemp;
    BYTE bOldAudType, bOldSpdifMode;

    // TCC171, Backup old audio type and spdif mode. Only these 2 value change need reset dsp.
    bOldAudType = _bAudType;
    bOldSpdifMode = _bSpDifMode;

	//kevin0.67, change Proc2 code will take effect since the following HAL_RESET_AUDIO
	//Kevin2.31, move from below here. Run-time unzip DSP ucode will destroy audio config regs/DMs
    _ChangeDSPCode(bType);

#ifndef DOLBY_CERTIFICATION //Kevin0.76a, keep acmod settings
    // TCC031, reference setup module for current setting
    CHIPS_Initial(CHIPS_INIT_CHANNELMODE);
#endif

    // TCC274, move to a function
    _SetAudio(bType);

    // TCC171, only these 2 types change need reset dsp.
    // Brian.276p, JPEG mode (HAL_AUDIO_NONE) doesn't need reset
    //Kevin1.07, fix "Setup->Audio->Digital out->SPDIF PCM/RAW->exit setup, audio will be unsmooth"
    //if ((bOldAudType != _bAudType || bOldSpdifMode != _bSpDifMode) && bType != HAL_AUDIO_NONE)
    if((bOldAudType != _bAudType) && (bType != HAL_AUDIO_NONE))
    {
        // TCC046, need to reset dsp before set register & dm
        HAL_Reset (HAL_RESET_AUDIO);
    }

    //Kevin1.24a, add
    //Kevin1.10, Control Analog Output on/off
    CHIPS_ControlAnalogOutput();

    //Kevin1.24a, uncomment to fix SPDIF: mute->prestop->resume => auto mute off _dwACR2[14]
    //Kevin1.10, comment
    CHIPS_ControlSPDIFOutput();

    //Kevin1.08, set ProLogic|| DM
    CHIPS_ProLogicControl();

    //Kevin0.80
    CHIPS_ControlHDCD();
}

//  *********************************************************************
//  Function    :   CHIPS_SPDIF_OnOff
//  Description :   Turn on/off SPDIF via AND gate hardware since BE chip
//  Arguments   :   bOn: 1: Turn on SPDIF (SPDIF carrier signal)
//                       0: Turn off SPDIF (grounding)
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************
//#pragma NOAREGS         // may be called in ISR
void CHIPS_SPDIF_OnOff(BYTE bOn)
{
    // Joey_238: SPDIF rule. [D30:D29], 01: off, 00: on, 02: on and mute
    if(bOn)
    {
        REG_PLAT_SYSTEM_CONFIGURATION1 &= ~(3 << 29);   // set D30-D29 = 0
        REG_AIU_AIUEN |= AIU_DMA_ENABLE_SPDIF_OUT;
    }
    else
    {
        REG_PLAT_SYSTEM_CONFIGURATION1 = (REG_PLAT_SYSTEM_CONFIGURATION1 & ~(3 << 29)) | (1 << 29); // set D30-D29 = 1
    }
}

//  Kevin2.80, create
//  *********************************************************************
//  Function    :   CHIPS_SPDIF_Mute
//  Description :   enable/disable SPDIF
//  Arguments   :   bEnable: 1: enable SPDIF
//                           0: disable SPDIF
//  Return      :   None
//  Side Effect :   None
//  *********************************************************************
//#pragma NOAREGS         // may be called in ISR
void CHIPS_SPDIF_Mute(BYTE bEnable)
{
    // Joey_238: SPDIF rule. [D30:D29], 01: off, 00: on, 02: on and mute
    if(((REG_PLAT_SYSTEM_CONFIGURATION1 >> 29) & 3) != 1)
    {
        if(bEnable)
        {
            REG_PLAT_SYSTEM_CONFIGURATION1 &= ~(3 << 29);   // set D30-D29 = 0
            REG_AIU_AIUEN |= AIU_DMA_ENABLE_SPDIF_OUT;
        }
        else
        {
            REG_PLAT_SYSTEM_CONFIGURATION1 = (REG_PLAT_SYSTEM_CONFIGURATION1 & ~(3 << 29)) | (2 << 29); // set D30-D29 = 2
        }
    }
}

//  *********************************************************************
//  Function    :   CHIPS_ControlSPDIFOutput
//  Description :   Control SPDIF enable/disable
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  *********************************************************************
// because CHIP_ControlSPDIFOutput() --> CHIPS_OpenAudio() --> ISR
//#pragma NOAREGS         // may be called in ISR
void CHIPS_ControlSPDIFOutput(void)
{
//kevin1.10, avoid PROC2 performance is not enough when doing language study
// wyc2.17-909S, for language study

    // Joey2.60: SPDIF PCM without volume control.
    HAL_WriteAM(HAL_AM_DO_SPDIF_PCM_SCALE, 1);

#if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)
    if((__bLangStudyFlag == TRUE) && (_bSpDifMode ==1))
    {
	    CHIPS_SPDIF_Mute(0); //disable spdif
        CHIPS_SPDIF_OnOff(0); //turn off SPDIF via AND gate
    }
    else
#endif

    if(_bSpDifMode == 2)
    {
		CHIPS_SPDIF_Mute(0); //disable spdif
        CHIPS_SPDIF_OnOff(0); //turn off SPDIF via AND gate
    }
	else
    {
        if(__btMute)   // In mute state
            CHIPS_SPDIF_Mute(0);
        else
    		CHIPS_SPDIF_Mute(1); //enable spdif

        CHIPS_SPDIF_OnOff(1); //turn on SPDIF via AND gate
    }


}

//  *********************************************************************
//  Function    :   CHIPS_OpenAudio
//  Description :   It will set dsp related registers according to the
//                  global variables set by almost all above functions
//  Arguments   :   bParam:      The type of command
//                      TRUE : Turn on audio
//                      FALSE : Turn off audio
//  Return      :   None
//  Change      :
//  Side Effect :
//  *********************************************************************
// when not using karaoke chip
// function is called automatically when playing each item
// so it must open the volume by channel, mute, key control status
//#pragma NOAREGS         // may be called in ISR
//Kevin0.87, modify
//void CHIPS_OpenAudio(BYTE bParam)
BYTE CHIPS_OpenAudio(BYTE bParam)
{
    if(bParam == FALSE) // Turn on audio
    {
        // Brian.173, Mute the MIC input first
        CHIPS_MICEnable(FALSE);

        // LLY.273, use "soft-mute" to replace original method
        CHIPS_SoftMute();

        CHIPS_SPDIF_Mute(0);

        // LLY2.16, do H/W mute while mute to pass °ê¼Ð mute testing (909R bug??)
        //HAL_IOMute(TRUE);
    }
    else
    {
        if(__btMute) // watch out for this break, 'casue the following function will not be called.
            return TRUE;

        if(__bSurround != CHIPS_SURROUND_NONE)
        {
            HAL_WriteAM(HAL_AM_SURR_EN, 1);

            if(__bSurround == CHIPS_SURROUND_LIVE) // live
            {
                HAL_WriteAM(HAL_AM_SURR_0, SURROUND_TYPE_LIVE);
            }
            else if(__bSurround == CHIPS_SURROUND_THEATER)
            {
                HAL_WriteAM(HAL_AM_SURR_0, SURROUND_TYPE_THEATER);
            }
            // TCC028, add surround type for future usage
            else if(__bSurround == CHIPS_SURROUND_STEREO)
            {
                HAL_WriteAM(HAL_AM_SURR_0, SURROUND_TYPE_STEREO);
            }
            else if(__bSurround == CHIPS_SURROUND_MOVIE)
            {
                HAL_WriteAM(HAL_AM_SURR_0, SURROUND_TYPE_MOVIE);
            }
            else if(__bSurround == CHIPS_SURROUND_KARAOKE)
            {
                HAL_WriteAM(HAL_AM_SURR_0, SURROUND_TYPE_KARAOKE);
            }
        }
        else
        {
            HAL_WriteAM(HAL_AM_SURR_EN, 0);
        }

        // TCC274, move to a function to set variable
        _SetDualMode();

        HAL_WriteAM(HAL_AM_CHANNEL_MODE, _bChannelMode); //_bChannelMode

        HAL_WriteAM(HAL_AM_LFE_OUTPUT, _bLFE); //_bLFE

        HAL_WriteAM(HAL_AM_AC3_KOK_MODE, __bVocal); //__bVocal
        HAL_WriteAM(HAL_AM_DUAL_MODE, _bDualMode); //_bDualMode

        if(_bSpDifMode != 2)
        {
            HAL_WriteAM(HAL_AM_SPDIF_MODE, (_bSpDifMode&0x01)); //0:pcm 1:raw
        }
        else
        {   // Joey2.51: if _bSpDifMode == 2 (spdif off), switch to spdif raw, because spdif PCM not supported by DTS
            HAL_WriteAM(HAL_AM_SPDIF_MODE, 1);  // spdif raw (avoid DTS hang)
        }

        HAL_WriteAM(HAL_AM_PCM_SCALE, __dwVolAudio); //16 bits
        __dwPreVolAudio = __dwVolAudio;

        // Joey2.75: Add AC3 downmix scale control. The scale value can be 0~15.
        HAL_WriteAM(HAL_AM_AC3_DOWN_MIX_SCALE, 15);

        //Kevin1.26, SPDIF enable -> fade in
        //Kevin1.10, Control Analog Output on/off
        CHIPS_ControlAnalogOutput();

        // LLY.103-1, replace with function API
        CHIPS_ControlSPDIFOutput();

        // Brian.173, enable the MIC input after mute
        CHIPS_MICEnable(TRUE);
    }

    return TRUE;
}

//  *********************************************************************
//  Function    :  CHIPS_MICEnable
//  Description :  Enable/Disable MIC Input.
//  Arguments   :  bEnable : enable or disable MIC input
//              :  TRUE: enable
//              :  FASLE: disable
//  Return      :  none.
//  Change      :
//  Side Effect :
//  *********************************************************************
// Brian.173
// LLY.273-1, must add "NOAGRES" pragma
// because CHIPS_MICEnable() --> CHIPS_OpenAudio() --> ISR
//#pragma NOAREGS         // may be called in ISR
void CHIPS_MICEnable(BYTE bEnable)
{
#ifdef NO_MIC_INPUT
    //kevin0.66, fix pause->normal play => noise (HW bug: Proc1 access AIU => unpack error)
	HAL_WriteAM(HAL_AM_MIC_PCM_SCALE, 0);
#else //#ifdef NO_MIC_INPUT
    if(bEnable)
    {
        //kevin0.66, fix pause->normal play => noise (HW bug: Proc1 access AIU => unpack error)
        HAL_WriteAM(HAL_AM_MIC_PCM_SCALE, __wMICVol);
    }
    else
    {
        //kevin0.66, fix pause->normal play => noise (HW bug: Proc1 access AIU => unpack error)
        HAL_WriteAM(HAL_AM_MIC_PCM_SCALE, 0);
    }
#endif //#ifdef NO_MIC_INPUT
}

//  *********************************************************************
//  Function    :  CHIPS_SoftMute
//  Description :  Do soft mute.
//  Arguments   :  none.
//  Return      :  none.
//  Change      :
//  Side Effect :
//  *********************************************************************
// LLY.273, remodify the procedure
// LLY.273-1, must add "NOAGRES" pragma
// because CHIPS_SoftMute() --> CHIPS_OpenAudio() --> ISR
//#pragma NOAREGS         // may be called in ISR
void CHIPS_SoftMute(void)
{
    DWORD dwTimerCount_FadeOut; //kevin1.07
    DWORD dwTemp;

    HAL_ReadAM(HAL_AM_REAL_PCM_SCALE, &dwTemp);

    if(dwTemp == 0)
    {
        HAL_WriteAM(HAL_AM_PCM_SCALE, 0);
        return;
    }

    HAL_WriteAM(HAL_AM_PCM_SCALE, 0);

    //kevin1.07, wait for fade out to be done
    dwTimerCount_FadeOut = OS_GetSysTimer();

    while((OS_GetSysTimer()-dwTimerCount_FadeOut) < COUNT_250_MSEC) //kevin1.11, 50M->250M
    {
        //kevin1.11, fix fade out may not be completed
        HAL_ReadAM(HAL_AM_REAL_PCM_SCALE, &dwTemp);

        if(dwTemp == 0)
        {
            OS_DelayTime(COUNT_50_MSEC);
            // LLY2.61, porting Chuan's code to pass Dolby Certification -- mute is 0dB
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
            REG_PLAT_RESET_CONTROL_ENABLE = PLAT_RESET_AIU_DAC_ENABLE;
            REG_PLAT_RESET_CONTROL_DISABLE = PLAT_RESET_AIU_DAC_DISABLE;
#endif // #if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
            break;
        }

        OS_YieldThread();
    }

    /*
    if(dwTemp != 0)
    {
        DBG_Printf(DBG_THREAD_PARSER, DBG_INFO_PRINTF, "PCM sca=%lx\n", dwTemp);
    }
    */
}

// Brian.170
//  *********************************************************************
//  Function    :   CHIPS_SWMICDetect()
//  Description :   It read the MIC input and compare it with threshlod,
//                  and determine the __sbMICDetect value
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  *********************************************************************
void CHIPS_MICDetect(void)
{
}

// J500KWShih_231, mpeg error concealment
//  *********************************************************************
//  Function    :   CHIPS_ControlMPEGEErrorConcealment()
//  Description :   It read the MIC input and compare it with threshlod,
//                  and determine the __sbMICDetect value
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  *********************************************************************
void CHIPS_ControlMPEGEErrorConcealment(BYTE bEnable)
{
    HAL_WriteAM(HAL_AM_WMA_MPEG_ERROR_CONCEALMENT_MASK, bEnable);
}

#ifdef DOLBY_CERTIFICATION //kevin0.80
BYTE CHIPS_Dolby_ProcessKey(void)
{
    if (__bKey==KEY_N1)
    {
            static BYTE    _bCenterDelay=0; //bit[2:0]: 0~5

            switch (_bCenterDelay)
            {
            case 0:
                strcpy(__bMsgString, "c delay0");
                break;
            case 1:
                strcpy(__bMsgString, "c delay1");
                break;
            case 2:
                strcpy(__bMsgString, "c delay2");
                break;
            case 3:
                strcpy(__bMsgString, "c delay3");
                break;
            case 4:
                strcpy(__bMsgString, "c delay4");
                break;
            case 5:
                strcpy(__bMsgString, "c delay5");
                break;
            }

            OSD_Output(MSG_STRING, 0, 0x3);

            HAL_WriteAM( HAL_AM_CENTER_DELAY, _bCenterDelay);

            _bCenterDelay++;
            if (_bCenterDelay>5)
                _bCenterDelay = 0;

            return KEY_NO_KEY;
    }

    else if (__bKey==KEY_N2)
    {
        static BYTE    _bSurrDelay=0; //bit[7:3]: 0~31

        switch (_bSurrDelay)
        {
        case 0:
            strcpy(__bMsgString, "s delay0");
            break;
        case 1:
            strcpy(__bMsgString, "s delay1");
            break;
        case 2:
            strcpy(__bMsgString, "s delay2");
            break;
        case 3:
            strcpy(__bMsgString, "s delay3");
            break;
        case 4:
            strcpy(__bMsgString, "s delay4");
            break;
        case 5:
            strcpy(__bMsgString, "s delay5");
            break;
        case 6:
            strcpy(__bMsgString, "s delay6");
            break;
        case 7:
            strcpy(__bMsgString, "s delay7");
            break;
        case 8:
            strcpy(__bMsgString, "s delay8");
            break;
        case 9:
            strcpy(__bMsgString, "s delay9");
            break;
        case 10:
            strcpy(__bMsgString, "s delay10");
            break;
        case 11:
            strcpy(__bMsgString, "s delay11");
            break;
        case 12:
            strcpy(__bMsgString, "s delay12");
            break;
        case 13:
            strcpy(__bMsgString, "s delay13");
            break;
        case 14:
            strcpy(__bMsgString, "s delay14");
            break;
        case 15:
            strcpy(__bMsgString, "s delay15");
            break;
        }

        OSD_Output(MSG_STRING, 0, 0x3);

        HAL_WriteAM( HAL_AM_SURROUND_DELAY, _bSurrDelay);

        _bSurrDelay++;
        if (_bSurrDelay>15)
            _bSurrDelay = 0;

        return KEY_NO_KEY;
    }
    else if (__bKey == KEY_N3)
    {
        static BYTE    bDownMixMode=0; //0~2

        extern BYTE    _bChannelMode ;
        extern char    _bLFE ;

        switch (bDownMixMode)
        {
        case 0:
            _bChannelMode=CHIPS_LCRLSRS;
            _bLFE=1;
            strcpy(__bMsgString, "5.1 ch");
            break;
        case 1:
            _bChannelMode=CHIPS_LRD;
            _bLFE=0;
            CHIPS_DownMixMode(AC3_DOWNMIX_LTRT);
            strcpy(__bMsgString, "2ch LtRt");
            break;
        case 2:
            _bChannelMode=CHIPS_LR;
            _bLFE=0;
            CHIPS_DownMixMode(AC3_DOWNMIX_LORO);
            strcpy(__bMsgString, "2ch LoRo");
            break;
        case 3:
            _bChannelMode=CHIPS_LR;
            _bLFE=0;
            CHIPS_DownMixMode(AC3_DOWNMIX_AUTO);
            strcpy(__bMsgString, "2ch Auto");
            break;
        }
        CHIPS_OpenAudio(TRUE);
        OSD_Output(MSG_STRING, 0, 0x5);
        bDownMixMode++;
        if (bDownMixMode>3)
            bDownMixMode = 0;

        return KEY_NO_KEY;
    }
    else if (__bKey == KEY_N4)
    {
        static BYTE    bACMOD=0;    //0~6

        extern BYTE    _bChannelMode ;
        extern char    _bLFE ;

        //CHIPS_Initial (CHIPS_INIT_CHANNELMODE) ;

        switch(bACMOD)
        {
        case 0:
            _bChannelMode=CHIPS_C;
            _bLFE=0;
            strcpy(__bMsgString, "acmod1_0");
            break;
        case 1:
            _bChannelMode=CHIPS_LR;
            _bLFE=0;
            strcpy(__bMsgString, "acmod2_0");
            break;
        case 2:
            _bChannelMode=CHIPS_LRLS;
            _bLFE=0;
            strcpy(__bMsgString, "acmod2_1");
            break;
        case 3:
            _bChannelMode=CHIPS_LRLSRS;
            _bLFE=0;
            strcpy(__bMsgString, "acmod2_2");
            break;
        case 4:
            _bChannelMode=CHIPS_LCR;
            _bLFE=0;
            strcpy(__bMsgString, "acmod3_0");
            break;
        case 5:
            _bChannelMode=CHIPS_LCRLS;
            _bLFE=0;
            strcpy(__bMsgString, "acmod3_1");
            break;
        case 6:
            _bChannelMode=CHIPS_LCRLSRS;
            _bLFE=1;
            strcpy(__bMsgString, "acmod3_2");
            break;
        }
        CHIPS_OpenAudio(TRUE);
        OSD_Output(MSG_STRING, 0, 0x5);
        bACMOD++;
        if (bACMOD>6)
            bACMOD = 0;

        return KEY_NO_KEY;
    }
    else if (__bKey == KEY_N5)
    {
        static BYTE    bBassMode=0;    //0~3

        switch (bBassMode)
        {
        case 0:
            strcpy(__bMsgString, "no bass");
            break;
        case 1:
            strcpy(__bMsgString, "conf 1");
            break;
        case 2:
            strcpy(__bMsgString, "conf 3+sub");
            break;
        case 3:
            strcpy(__bMsgString, "conf3 no sub");
            break;
        }
        HAL_WriteAM(HAL_AM_BASS_MANAGE_CONFIG, bBassMode);
        OSD_Output(MSG_STRING, 0, 0x5);

        bBassMode++;
        if (bBassMode>3)
            bBassMode = 0;

        return KEY_NO_KEY;
    }
    //kevin1.05, center/surr delay waveform inverse: add test key before doing/leaving delay testing
    else if (__bKey == KEY_N6)
    {
    	strcpy(__bMsgString, "test delay");
        HAL_WriteAM(HAL_AM_TEST_CENTERSURR_DELAY, 1);
        OSD_Output(MSG_STRING, 0, 0x5);

        return KEY_NO_KEY;
    }
    else if (__bKey == KEY_N7)
    {
    	strcpy(__bMsgString, "not test delay");
        HAL_WriteAM(HAL_AM_TEST_CENTERSURR_DELAY, 0);
        OSD_Output(MSG_STRING, 0, 0x5);

        return KEY_NO_KEY;
    }

    return KEY_BYPASS;
}
#endif
