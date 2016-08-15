
#define MAIN_SETUP_DECLARE

#include    <string.h>
#include    "winav.h"
#include    "chips.h"
#include    "disp.h"
#include    "input.h"
#include    "cc.h"
#include    "navigate.h"
#include    "utl.h"
#include    "hal.h"
#include    "gdi.h"
#include    "osddlg.h"
#include    "menu.h"
#include    "setup.h"
#include    "dvdsetup_op.h"
#include    "dvdsetup.h"
#include    "osd.h"
#include    "char_subpict.h"
#include "romld.h"
#include    "dvdsetup.h"
#include    "storage.h"
#include "VD\\ViporFunc.h"
#ifdef SUPPORT_STB
#include "STB\usrif\\sysmenu.h"
#endif //SUPPORT_STB

#include "poweronmenu.h"

#ifdef IMAGE_FRAME_SETUP
#include "tft.h"
#endif //IMAGE_FRAME_SETUP


#define SETUP_WIDTH             616
#define SETUP_HEIGHT            410 

// SETUP_FRAME_Y0 should be lower than the upper osd region, otherwise
// When input disc, it shows "Loading...".
// The strings will cover partial of Setup UI
#define SETUP_FRAME_X0          0
#define SETUP_FRAME_Y0          0
#define SETUP_FRAME_X1          (SETUP_WIDTH + SETUP_FRAME_X0)
#define SETUP_FRAME_Y1          (SETUP_HEIGHT + SETUP_FRAME_Y0) 

#define WRITE_EEPROM_ENABLE

#define IDENTIFY_STRING         "C"
#define FRAME_THICKNESS         2

extern BYTE    __bSetupMode;
extern BYTE	 __bTVSystemChange;

// save to EEPROM info.
extern  BYTE    __bPassword[SETUP_PASSWORD_NO+1];
extern  BYTE    __bInputPassword; //=FALSE;
extern  BYTE    __bInputPasswordIndex; // = 0;
extern  BYTE    _bVideoSettingChange; // = FALSE;

//#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
BYTE __bPreCountStop; //Xuli1.26, Save the previous stop mode before entering SETUP to decide if need resume after exit.
//#endif

BYTE    __bSETUPUIStyleChanged;

#ifdef SUPPORT_STB
SYSTEMINFO* __pSystemInfo;
#endif //SUPPORT_STB

BYTE    _bAudioTypeChange; // = FALSE;
BYTE    _bParentalControlChange; // = FALSE;

IMAGE_FRAME_SETUPINFO __ImageFrameSetupInfo;


#ifdef _DEBUG_INFO
BYTE    _bEEPROMError = 0;
#endif

extern  void _InitRect(PURECT pRect, WORD wLeft, WORD wTop, WORD wRight, WORD wBottom);

extern  WORD __wSPRMs [];
extern  BYTE             _bSetupTemp;
extern  WORD             _wSetupTemp;
extern  WORD             _wSetupString[8];
extern  PARM_DRAW_STRING _DVDSETUP_StrAttr;
extern  PARM_RECT        _DVDSETUP_RectAttr;
extern  WORD             __wSPRMs [];
extern  DWORD __dwVolAudio;

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
extern BYTE __bMMJPEGAudioMode;
#endif //SUPPORT_JPEG_AUDIO_PLAYING

// Internal functions
void _ReadSetupData(void);
void _WriteSetupData(BYTE bSetup, BYTE bDefaultPassword);
void _SetVideoOutputToIC(void);
void _SetBrightnessToIC(void);
void _SetContrastToIC(void);
void _SetDNRToIC(void);
void _SetDownMix(void);
void _TransferLanguageToSPRM(void);
void _TransferOthersAudioLanguageCode(WORD wLanguageCode);
void _TransferOthersMenuLanguageCode(WORD wLanguageCode);
void _TransferOthersSPSTLanguageCode(WORD wLanguageCode);
void _SetAngleMarkTime(void);

extern  void _SetAudType(BYTE bType);  // in CHIPS module
extern void MM_AudioTrigger(void);
extern BYTE TFT_BRIGHTNESS_VALUE_LIST[];
extern BYTE TFT_CONTRAST_VALUE_LIST[];
extern BYTE TFT_SATURATION_VALUE_LIST[];

void DVDSETUP_InitStatus(void)
{
    _bAudioTypeChange = FALSE;
}


//  ***************************************************************************
//  Function    : SETUP_Initial
//  Description : This API will export to other modules use while SETUP data should be
//              : initialized and prepare for further usage.
//  Arguments   :
//  Return      : TRUE
//  Side Effect :
//  ***************************************************************************
BIT SETUP_Initial()
{
    BYTE             *pbSetupPtr;
    
#ifndef IMAGE_FRAME_SETUP
    __bInputPassword=FALSE;
    __bInputPasswordIndex=0;
#endif //IMAGE_FRAME_SETUP

#ifdef SUPPORT_STB
    __pSystemInfo = (SYSTEMINFO*) &(__SetupInfo.bSTBMagiceCountHead);
#endif

    __bSETUPUIStyleChanged = FALSE;   

#ifndef WRITE_EEPROM_ENABLE
    _WriteSetupData(SETUP_DATA_DEFAULT, READ_TOTAL_DEFAULT);
    // Module should add their Init function here to set EPROM to default value
#else
    pbSetupPtr=(BYTE *)&__SetupInfo;
    HAL_ReadStorage(SETUP_ADDR_IDENTIFY, pbSetupPtr, EPROM_IDENTIFY_BYTE + 
        EPROM_VERSION_BYTE + EPROM_INTERVERSION_BYTE);
    __SetupInfo.cSetupIdentify[EPROM_IDENTIFY_BYTE-1] = 0;
    
    // Brian2.81
    // Setup module will call other modules' Init function only
    // when EPROM data is incorrect. Setup will not call other modules'
    // Init fuction when EPROM data is correct
    if (!strcmp(__SetupInfo.cSetupIdentify, IDENTIFY_STRING) &&
        SETUP_VERSION == __SetupInfo.bSetupVersion &&
        INTER_VERSION == __SetupInfo.bInterVersion) 
    {
        // setup data is correct
        _ReadSetupData();
        // Setup should not read data from EPROM for other modules.
        // The power-on flow should do this. 
        // Kevin1.25, move below. Otherwise, 4 wait no ACK for 8M,
        // 'cause IRAM hasn't been loaded 
    }
    else
    {
        // setup data is incorrect
        _WriteSetupData(SETUP_DATA_DEFAULT, READ_TOTAL_DEFAULT); // Brian1.24
        // Module should add their Init function here to set EPROM to default value
    }
#endif
    
    // update Setup info to SPRM
    // reference VI4-, 4.6.1.2
    // *SPRM[0], Menu Description Language Code
    // SPRM[2], Sup-picture stream on/off flag
    // SPRM[3], Angle number??
    // SPRM[11], Player Audio Mixing Mode for Karaoke
    // SPRM[12], Contry Code for Parental Management
    // SPRM[13], Parental Level
    // SPRM[14], Player Configuration for Video(Aspect ratio)
    // SPRM[15], P_CFG for audio
    // *SPRM[16], Initial Language Code for AST
    // SPRM[17], Initial Language Code extension for AST
    // *SPRM[18], Initial Language Code for SPST
    // SPRM[19], Initial Language extension Code for SPST
    // SPRM[20], Player Region Code
    // bit 15-8, upper byte
    // bit 7-0, lower byte

#ifndef IMAGE_FRAME_SETUP    
    _SetVideoOutputToIC();
#endif

#ifndef NO_DISC_MODE
    //Brian1.05, just call NV_Setting, it will set __wSPRMs[20] always
    NV_Setting(SET_REGION_CODE, 0); 
#endif //#ifndef NO_DISC_MODE

    //Kevin1.25, move here
    // Brian1.25, set brightness and contrast when initialization
    _SetBrightnessToIC();
    _SetContrastToIC();
    //Damon1.05, set display type when setup initialization
    //Or it will always set to default value (16:9) in TFT_Init()
    _SetDisplayToIC();
    _SetDownMix(); 
    _TransferLanguageToSPRM();
    _SetDNRToIC();
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
    OSD_ReplaceFont();
//#endif


#ifdef IMAGE_FRAME_SETUP
	DVDSETUP_SetRepeatMode();
#endif //IMAGE_FRAME_SETUP
  
#ifndef IMAGE_FRAME_SETUP    
    _bVideoSettingChange = FALSE;
#endif //IMAGE_FRAME_SETUP
    
#ifdef  SUPPORT_KARAOKE_SET
    if ((__SetupInfo.bAudioKaraoke - SETUP_AUDIO_KARAOKE_BASE) == 0) //on
        CHIPS_MICControl(CHIPS_ON);
    else if ((__SetupInfo.bAudioKaraoke - SETUP_AUDIO_KARAOKE_BASE) == 1) //off
        CHIPS_MICControl(CHIPS_OFF);
#endif
    
#ifndef NO_DISC_MODE
    //initial the System parameter only in Setup module not in NV_InitialParameters
    // reference Anex J
    __wSPRMs [10]=0;     // TT_PGCN fot NV_TMR, 1 to 7fffh(bits 0-14)
    __wSPRMs [11]=0;     // Player Audio ixing Mode(P_AMXMD) for karaoke.
    
    // must use upper case
    strncpy((char *)&__wSPRMs[12], "US", 2);
    
    NV_Setting(SET_PARENTAL_CONTROL, NULL_0);
    
    /* Alan1.05
    // Paraental level(PTL_LVL), 1-8(bits 0-3)
    __wSPRMs[13]= __SetupInfo.bParentalCtl - SETUP_CUSTOM_PC_LEVEL_BASE + 1;
    
     // don't do parental control, set as highest level
     if (!__SetupInfo.bParentalControl )
     __wSPRMs[13]=8;
    */
    
    // RD_ONLY
    // default ???
    __wSPRMs [14]=0;     // Player configuration(P_CFG) for Video
    __wSPRMs [15]=0;     // P_CFG for Audio
    __wSPRMs [17]=0;     // Initial Language Code extension(INI_LCD_EXT)
    __wSPRMs [19]=0;     // Initial Language Code extension for SPST
#endif //#ifndef NO_DISC_MODE
    
#ifndef REMOVE_SETUP_SPEAKER_TEST
    // brian.277a Test Tone
    __SetupInfo.bAudioTestTone = SETUP_AUDIO_SPEAKER_TEST_AUTO;
#endif
    
    SETUP_LoadLanguage();
    
    return TRUE;
}


//  ***************************************************************************
//  Function    :   SETUP_AdjustSetting
//  Description :   This function will be called the setting is changed.
//  Arguments   :   bID, the ID of the setting to be changed
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    SETUP_AdjustSetting(BYTE bID)
{
#ifndef NO_SETUP
    switch (bID)
    {
    case SETUP_SCREEN_TV_SYSTEM:
        HAL_AdjustTVMode(DISP_NEXTVSYNC);
        break;
#ifndef IMAGE_FRAME_SETUP
    case SETUP_SCREEN_VIDEO_OUTPUT:
        HAL_AdjustTVMode(DISP_NEXTVSYNC);
        _SetVideoOutputToIC();
        break;
        case SETUP_AUDIO_OUTPUT:
// DVD_275b+, SUPPORT_VIRTUAL_DTS
#ifdef  SUPPORT_VIRTUAL_DTS
        // 1) when user select stream is DTS stream, and output is SPDIF_RAW
        //    must choose real DTS stream to play.
        // 2) Current play DTS stream, and output change to non-SPDIF/RAW
        //    must choose a non-DTS stream to play.
        if ( __SetupInfo.bAudioAnalogOut == SETUP_AUDIO_DIGITAL_OUT_SPDIF_RAW ) // Brian1.23, use bAudioAnalogOut
        {
            // user select is DTS stream, but play stream may not be DTS stream.
            if (__bSelectAudioType == HAL_AUDIO_DTS)
            {
                // for choose back a DTS stream. 
                NV_Setting(SET_AST, (BYTE)NULL);
            }
        }
        else
        {
            // non-SPDIF_RAW mode, don't allow DTS stream.
            if ( __bAudioType == HAL_AUDIO_DTS )
            {
                // for choose a non-DTS stream.
                NV_Setting(SET_AST, (BYTE)NULL);
            }
        }
#endif   //#ifdef  SUPPORT_VIRTUAL_DTS
    
        {
            // TCC275, direct call CHIPS module, will re-set ACFGR and SPDIF.
            _SetAudType (__bAudioType) ;
        }
        break;
    case SETUP_AUDIO_KARAOKE:
        break;
#else
#ifdef SUPPORT_DSPLAY_16_9_AND_AUTO
    case SETUP_DISPLAY_DISPLAY_TYPE:
        _SetDisplayToIC();
        break;
#endif
#endif //#ifndef IMAGE_FRAME_SETUP
    default:
        break;
    }
#endif
}


//  ***************************************************************************
//  Function    :   SETUP_ChangeValue
//  Description :   This function will be called when user the value of the items
//              :   in SETUP UI. It can save the changed data to EEPROM.
//  Arguments   :   bID, the ID that want to be saved in EEPROM.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    SETUP_ChangeValue(BYTE bID)
{

#ifndef NO_SETUP
#ifndef IMAGE_FRAME_SETUP //CoCo2.38

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
    switch (bID)
    {
    case SETUP_SCREEN_TV_SYSTEM:
        HAL_WriteStorage(SETUP_ADDR_TV_SYTEM, (BYTE*)&(__SetupInfo.bTVSystem), 1);
        break;
    //Brian0.90
    case SETUP_SCREEN_PROGRESSIVE_SCAN:
        // Brian1.06, P-SCAN is integrated into Video Output
        if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN ||
            __SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN ||
            __SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_VGA)
        {
            __SetupInfo.bProgressiveScan = SETUP_PROGRESSIVE_SCAN_ON;
        }
        else
        {
            __SetupInfo.bProgressiveScan = SETUP_PROGRESSIVE_SCAN_OFF;
        }

        HAL_WriteStorage(SETUP_ADDR_SCAN_MODE, (BYTE*)&(__SetupInfo.bProgressiveScan), 1);
        break;
    case SETUP_SCREEN_VIDEO_OUTPUT:
        HAL_WriteStorage(SETUP_ADDR_VIDEO_OUTPUT, (BYTE *)&(__SetupInfo.bVideoOutput), 1);

        break;
    case SETUP_AUDIO_OUTPUT:
        HAL_WriteStorage(SETUP_ADDR_AUDIO_OUTPUT, (BYTE *)&(__SetupInfo.bAudioOutput), 1);
        break;
    case SETUP_AUDIO_KARAOKE:
        HAL_WriteStorage(SETUP_ADDR_AUDIO_KARAOKE, (BYTE *)&(__SetupInfo.bAudioKaraoke), 1);
        break;
    default:
        break;
    }

#endif
#endif //#ifndef IMAGE_FRAME_SETUP //CoCo2.38
#endif
}


//  ***************************************************************************
//  Function    :   SETUP_SetDefaultValue
//  Description :   This function will set the settings to default value.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :
//  ***************************************************************************
void    SETUP_SetDefaultValue(BYTE bType) // Brian1.24
{
#ifndef NO_OSD
#ifndef NO_SETUP
#ifndef IMAGE_FRAME_SETUP //CoCo2.38

    // Brian1.10, fix bug, general backdoor need to change ODS language variables
    __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE;
    __bOSDLanguage = __SetupInfo.bOSDLanguage;
    //_bOSDLang = __bOSDLanguage - SETUP_LANGUAGE_OSD_BASE;

    _WriteSetupData(SETUP_DATA_DEFAULT, bType); // Brian1.24
    // Brian1.24, always adjust setting when Set default
    SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);
    SETUP_AdjustSetting(SETUP_AUDIO_OUTPUT); 
    SETUP_AdjustSetting(SETUP_AUDIO_KARAOKE);
#ifdef SUPPORT_DSPLAY_16_9_AND_AUTO
    SETUP_AdjustSetting(SETUP_DISPLAY_DISPLAY_TYPE);
  #endif
#endif //#ifndef IMAGE_FRAME_SETUP //CoCo2.38
#endif
#endif
}


void SETUP_Exit(BYTE bExitMode)
{
    switch (bExitMode)
    {
    case OSD_UI_EXIT_ALL:
        break;
    case OSD_UI_EXIT_CONTROL_ONLY:
        break;
    default:
        break;
    }
}

BYTE SETUP_Recover(BYTE bRecoverRegion)
{
    DVDSETUP_Recover();
    return TRUE;
}

void SETUP_Trigger(void)
{
#ifndef NO_OSD
#ifndef NO_SETUP

#define LAST_AUTO_STAGE 12

    if (__bSetupMode)
    {
        // brian.277a-3rd, AUTO Test Tone
        // stage  definition:
        // 0: not in AUTO mode
        // 1: Left channel (4 sec)   2: mute (2 sec)
        // 3: Right channel (4 sec)  4: mute (2 sec)
        // 5: Center channel (4 sec) 6: mute (2 sec)
        // 7: LS channel (4 sec)     8: mute (2 sec)
        // 9: LR channel (4 sec)     10: mute (2 sec)
        // 11: SW channel (4 sec)    12: mute (2 sec)

#ifndef REMOVE_SETUP_SPEAKER_TEST
        if (_bTestToneStage)
        {
            if ( ((_bTestToneStage%2) && (OS_GetSysTimer() > _dwStageCount + COUNT_4_SEC)) ||
                 ((_bTestToneStage%2 == 0) && (OS_GetSysTimer() > _dwStageCount + COUNT_2_SEC)) )
            {

                _bTestToneStage++;
                if (_bTestToneStage > LAST_AUTO_STAGE)
                    _bTestToneStage = 1;

                // Clear speaker highlight by showing the BMP
                _SETUP_ShowSpeaker(SPEAKER_ALL);

                switch(_bTestToneStage)
                {
                case 1: // Left
                    _SETUP_ShowSpeaker(SPEAKER_L|SPEAKER_HIGHLIGHT);
                    _SetTestTone(L_CHANNEL);
                    break;
                case 3: // Right
                    _SETUP_ShowSpeaker(SPEAKER_R|SPEAKER_HIGHLIGHT);
                    _SetTestTone(R_CHANNEL);
                    break;
                case 5: // Center
                    _SETUP_ShowSpeaker(SPEAKER_C|SPEAKER_HIGHLIGHT);
                    _SetTestTone(C_CHANNEL);
                    break;
                case 7: // Left Surround
                    _SETUP_ShowSpeaker(SPEAKER_LS|SPEAKER_HIGHLIGHT);
                    _SetTestTone(LS_CHANNEL);
                    break;
                case 9: // Right Surround
                    _SETUP_ShowSpeaker(SPEAKER_RS|SPEAKER_HIGHLIGHT);
                    _SetTestTone(RS_CHANNEL);
                    break;
                case 11: // Subwoofer
                    _SETUP_ShowSpeaker(SPEAKER_SW|SPEAKER_HIGHLIGHT);
                    _SetTestTone(SW_CHANNEL);
                    break;
                case 2: // Mute
                case 4:
                case 6:
                case 8:
                case 10:
                case 12:
                    _SetTestTone(MUTE_CHANNEL);
                    break;
                }
                _dwStageCount = __dwCountSystem;

            }

        } //if (_bTestToneStage)

#endif // #ifndef REMOVE_SETUP_SPEAKER_TEST

#ifdef SUPPORT_JPEG_AUDIO_PLAYING

        if (__bMMJPEGAudioMode)
        {
            MM_AudioTrigger();
        }

#endif //SUPPORT_JPEG_AUDIO_PLAYING
    }


#endif
#endif
}

#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
//  ***************************************************************************
//  Function    :   OSDPROMPT_Trigger
//  Description :   In OSD_PROMPT_MODE , every 1 second trigger to show left time
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :
//  ***************************************************************************
void OSDPROMPT_Trigger(void)
{
}
#endif


/////////////////// Private functions //////////////////////////////////////

//  ***************************************************************************
//  Function    :   _SetDNRToIC
//  Description :   This function will set DNR value to IC. The "dynamics" register
//              :   will be modified in this function.
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _SetDNRToIC(void)
{
#ifndef NO_SETUP
#ifndef IMAGE_FRAME_SETUP //CoCo2.38
    HAL_WriteAM(HAL_AM_DRC_LEVEL, __SetupInfo.bAudioDNR - SETUP_AUDIO_DNR_BASE);
#endif //#ifndef IMAGE_FRAME_SETUP //CoCo2.38
#endif 
}

void _SetDownMix(void)
{
#ifndef IMAGE_FRAME_SETUP //CoCo2.38
    if (SETUP_AUDIO_DOWNMIX_LORO == __SetupInfo.bAudioDownMix)
    {
        CHIPS_DownMixMode(AC3_DOWNMIX_LORO);
    }
    else
    {
        CHIPS_DownMixMode(AC3_DOWNMIX_LTRT);
    }
#endif //#ifndef IMAGE_FRAME_SETUP //CoCo2.38
}


//  ***************************************************************************
//  Function    :   _TransferLanguageToSPRM
//  Description :   This function will transfer the language value in SetupInfo
//              :   to wSPRMs[ ].
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _TransferLanguageToSPRM(void)
{
#ifndef IMAGE_FRAME_SETUP //CoCo2.38
    switch (__SetupInfo.bMenuLanguage)
    {
    case SETUP_LANGUAGE_MENU_ENGLISH:
        strncpy((char *)&__wSPRMs[0], "en", 2);
        break;
    case SETUP_LANGUAGE_MENU_CHINESE:
        strncpy((char *)&__wSPRMs[0], "zh", 2);
        break;
    case SETUP_LANGUAGE_MENU_JAPANESE:
        strncpy((char *)&__wSPRMs[0], "ja", 2);
        break;
    case SETUP_LANGUAGE_MENU_FRENCH:
        strncpy((char *)&__wSPRMs[0], "fr", 2);
        break;
    case SETUP_LANGUAGE_MENU_GERMAN:
        strncpy((char *)&__wSPRMs[0], "de", 2);
        break;
    case SETUP_LANGUAGE_MENU_ITALIAN:
        strncpy((char *)&__wSPRMs[0], "it", 2);
        break;
    case SETUP_LANGUAGE_MENU_SPANISH:
        strncpy((char *)&__wSPRMs[0], "es", 2);
        break;
    case SETUP_LANGUAGE_MENU_PORTUGUESE:
        strncpy((char *)&__wSPRMs[0], "pt", 2); //Brian0.87a
        break;
    case SETUP_LANGUAGE_MENU_HINDI:
        strncpy((char *)&__wSPRMs[0], "hi", 2);
        break;
    case SETUP_LANGUAGE_MENU_THAI:
        strncpy((char *)&__wSPRMs[0], "th", 2);
        break;
    case SETUP_LANGUAGE_MENU_RUSSIAN:
        strncpy((char *)&__wSPRMs[0], "ru", 2);
        break;
    case SETUP_LANGUAGE_MENU_KOREAN:
        strncpy((char *)&__wSPRMs[0], "ko", 2);
        break;
    case SETUP_LANGUAGE_MENU_OTHERS:
        _TransferOthersMenuLanguageCode(__SetupInfo.wMenuLanguageCode);
        break;
    default:
        break;
    }

    switch (__SetupInfo.bAudioLanguage)
    {
    case SETUP_LANGUAGE_AUDIO_ENGLISH:
        strncpy((char *)&__wSPRMs[16], "en", 2);
        break;
    case SETUP_LANGUAGE_AUDIO_CHINESE:
        strncpy((char *)&__wSPRMs[16], "zh", 2);
        break;
    case SETUP_LANGUAGE_AUDIO_JAPANESE:
        strncpy((char *)&__wSPRMs[16], "ja", 2);
        break;
    case SETUP_LANGUAGE_AUDIO_FRENCH:
        strncpy((char *)&__wSPRMs[16], "fr", 2);
        break;
    case SETUP_LANGUAGE_AUDIO_GERMAN:
        strncpy((char *)&__wSPRMs[16], "de", 2);
        break;
    case SETUP_LANGUAGE_AUDIO_ITALIAN:
        strncpy((char *)&__wSPRMs[16], "it", 2);    // ** 2.74-TCH; 
        break;
    case SETUP_LANGUAGE_AUDIO_SPANISH:
        strncpy((char *)&__wSPRMs[16], "es", 2);    // ** 2.74-TCH; 
        break;
    case SETUP_LANGUAGE_AUDIO_PORTUGUESE:
        strncpy((char *)&__wSPRMs[16], "pt", 2);  //Brian0.87a // ** 2.74-TCH; 
        break;
    case SETUP_LANGUAGE_AUDIO_HINDI:
        strncpy((char *)&__wSPRMs[16], "hi", 2);
        break;
    case SETUP_LANGUAGE_AUDIO_THAI:
        strncpy((char *)&__wSPRMs[16], "th", 2);
        break;
    case SETUP_LANGUAGE_AUDIO_RUSSIAN:
        strncpy((char *)&__wSPRMs[16], "ru", 2);
        break;
    case SETUP_LANGUAGE_AUDIO_KOREAN:
        strncpy((char *)&__wSPRMs[16], "ko", 2);
        break;
    case SETUP_LANGUAGE_AUDIO_OTHERS:
        _TransferOthersAudioLanguageCode(__SetupInfo.wAudioLanguageCode);
        break;
    default:
        break;
    }

    switch (__SetupInfo.bSPSTLanguage)
    {
    //CoCo2.81, add SP OFF
    case SETUP_LANGUAGE_SP_OFF:
        //CoCo, need to set subtitle off here. Currently, we set "00" as subtitle off
        strncpy((char *)&__wSPRMs[18], "00", 2);
        break;
    case SETUP_LANGUAGE_SP_ENGLISH:
        strncpy((char *)&__wSPRMs[18], "en", 2);
        break;
    case SETUP_LANGUAGE_SP_CHINESE:
        strncpy((char *)&__wSPRMs[18], "zh", 2);
        break;
    case SETUP_LANGUAGE_SP_JAPANESE:
        strncpy((char *)&__wSPRMs[18], "ja", 2);
        break;
    case SETUP_LANGUAGE_SP_FRENCH:
        strncpy((char *)&__wSPRMs[18], "fr", 2);
        break;
    case SETUP_LANGUAGE_SP_GERMAN:
        strncpy((char *)&__wSPRMs[18], "de", 2);
        break;
    case SETUP_LANGUAGE_SP_ITALIAN:
        strncpy((char *)&__wSPRMs[18], "it", 2);    // ** 2.74-TCH; 
        break;
    case SETUP_LANGUAGE_SP_SPANISH:
        strncpy((char *)&__wSPRMs[18], "es", 2);    // ** 2.74-TCH; 
        break;
    case SETUP_LANGUAGE_SP_PORTUGUESE:
        strncpy((char *)&__wSPRMs[18], "pt", 2);  //Brian0.87a // ** 2.74-TCH; 
        break;
    case SETUP_LANGUAGE_SP_HINDI:
        strncpy((char *)&__wSPRMs[18], "hi", 2);
        break;
    case SETUP_LANGUAGE_SP_THAI:
        strncpy((char *)&__wSPRMs[18], "th", 2);
        break;
    case SETUP_LANGUAGE_SP_RUSSIAN:
        strncpy((char *)&__wSPRMs[18], "ru", 2);
        break;
    case SETUP_LANGUAGE_SP_KOREAN:
        strncpy((char *)&__wSPRMs[18], "ko", 2);
        break;
    case SETUP_LANGUAGE_SP_OTHERS:
        _TransferOthersSPSTLanguageCode(__SetupInfo.wSPSTLanguageCode);
        break;
    default:
        break;
    }
#endif //IMAGE_FRAME_SETUP
}


//  ***************************************************************************
//  Function    :   _TransferOthersAudioLanguageCode
//  Description :   This function will transfer the menu language value that user
//              :   inputs to the standard language code.
//  Arguments   :   wLanguageCode, the audio language code.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _TransferOthersAudioLanguageCode(WORD wLanguageCode)
{
#ifndef NO_SETUP

    if (((HIBYTE(wLanguageCode) >= 1) && (HIBYTE(wLanguageCode) <= 26)) &&
        ((LOBYTE(wLanguageCode) >= 1) && (LOBYTE(wLanguageCode) <= 26)))
    {
        _cTempLanguageCode[0] = _cLanguageCode[HIBYTE(wLanguageCode) - 1];
        _cTempLanguageCode[1] = _cLanguageCode[LOBYTE(wLanguageCode) - 1];
        strncpy((char *)&__wSPRMs[16], _cTempLanguageCode, 2);
    }
    else
        strncpy((char *)&__wSPRMs[16], "zh", 2);

#endif // #ifndef NO_SETUP
}


//  ***************************************************************************
//  Function    :   _TransferOthersMenuLanguageCode
//  Description :   This function will transfer the menu language value that user
//              :   inputs to the standard language code.
//  Arguments   :   wLanguageCode, the menu language code.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _TransferOthersMenuLanguageCode(WORD wLanguageCode)
{
#ifndef NO_SETUP

    if (((HIBYTE(wLanguageCode) >= 1) && (HIBYTE(wLanguageCode) <= 26)) &&
        ((LOBYTE(wLanguageCode) >= 1) && (LOBYTE(wLanguageCode) <= 26)))
    {
        _cTempLanguageCode[0] = _cLanguageCode[HIBYTE(wLanguageCode) - 1];
        _cTempLanguageCode[1] = _cLanguageCode[LOBYTE(wLanguageCode) - 1];
        strncpy((char *)&__wSPRMs[0], _cTempLanguageCode, 2);
    }
    else
        strncpy((char *)&__wSPRMs[0], "zh", 2);

#endif // #ifndef NO_SETUP
}


//  ***************************************************************************
//  Function    :   _TransferOthersSPSTLanguageCode
//  Description :   This function will transfer the menu language value that user
//              :   inputs to the standard language code.
//  Arguments   :   wLanguageCode, the SPST language code.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _TransferOthersSPSTLanguageCode(WORD wLanguageCode)
{
#ifndef NO_SETUP

    if (((HIBYTE(wLanguageCode) >= 1) && (HIBYTE(wLanguageCode) <= 26)) &&
        ((LOBYTE(wLanguageCode) >= 1) && (LOBYTE(wLanguageCode) <= 26)))
    {
        _cTempLanguageCode[0] = _cLanguageCode[HIBYTE(wLanguageCode) - 1];
        _cTempLanguageCode[1] = _cLanguageCode[LOBYTE(wLanguageCode) - 1];
        strncpy((char *)&__wSPRMs[18], _cTempLanguageCode, 2);
    }
    else
        strncpy((char *)&__wSPRMs[18], "zh", 2);

#endif // #ifndef NO_SETUP
}


//  ***************************************************************************
//  Function    : _ReadSetupData
//  Description : This function will read SETUP data from EEPROM to __SetupInfo
//              : and do some protection.
//  Arguments   : 
//  Return      : none.
//  Side Effect :
//  ***************************************************************************
void    _ReadSetupData()
{
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
    BYTE             *pbSetupPtr;
    BYTE             *pbImageFrameSetupPtr;
    
    pbSetupPtr=(BYTE *)&__SetupInfo;
    HAL_ReadStorage(SETUP_ADDR_AUDIO_LANGUAGE, pbSetupPtr + EPROM_IDENTIFY_BYTE +
        EPROM_VERSION_BYTE + EPROM_INTERVERSION_BYTE,
        SETUP_ADDR_USED - SETUP_ADDR_INTERVERSION);
    
    // DMP_116, new address for Image Frame setup
    pbImageFrameSetupPtr = (BYTE *)&__ImageFrameSetupInfo;
    HAL_ReadStorage(SETUP_ADDR_PHOTO_SETTING_INTERVAL_TIME, pbImageFrameSetupPtr,
        IMAGE_FRAME_SETUP_DATA_BYTES);    
    
#ifdef SUPPORT_STB
    SYSMENU_CheckValueRange();
#else

    if ((__SetupInfo.bVideoOutput < SETUP_SCREEN_VIDEO_OUTPUT_BEGIN) ||
        (__SetupInfo.bVideoOutput > SETUP_SCREEN_VIDEO_OUTPUT_END))
    {
        __SetupInfo.bVideoOutput = SETUP_DEFAULT_VIDEO_OUTPUT;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_VIDEO_OUTPUT;
#endif
    }
    
    if ((__SetupInfo.bUIStyle < SETUP_CUSTOM_UI_STYLE_BEGIN) || (__SetupInfo.bUIStyle > SETUP_CUSTOM_UI_STYLE_END))
    {
        __SetupInfo.bUIStyle = SETUP_DEFAULT_UI_STYLE;
    }
    
    if ((__SetupInfo.bTVSystem < SETUP_DISPLAY_TV_STANDARD_BEGIN) || (__SetupInfo.bTVSystem > SETUP_DISPLAY_TV_STANDARD_END))
    {
        __SetupInfo.bTVSystem = SETUP_DEFAULT_TV_SYSTEM;
    }

    if ((__SetupInfo.bBrightness < SETUP_DISPLAY_BRIGHTNESS_BEGIN) || (__SetupInfo.bBrightness > SETUP_DISPLAY_BRIGHTNESS_END))
    {
        __SetupInfo.bBrightness = SETUP_DEFAULT_BRIGHTNESS;
    }

    if ((__SetupInfo.bContrast < SETUP_DISPLAY_CONTRAST_BEGIN) || (__SetupInfo.bContrast > SETUP_DISPLAY_CONTRAST_END))
    {
        __SetupInfo.bContrast = SETUP_DEFAULT_CONTRAST;
    }

    if ((__SetupInfo.bOSDLanguage < SETUP_CUSTOM_OSD_BEGIN) || (__SetupInfo.bOSDLanguage > SETUP_CUSTOM_OSD_END))
    {
        __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE;
    }  
#endif //SUPPORT_STB

#ifndef NO_SCREEN_SAVER // Brian1.20
    
    if ((__SetupInfo.bSCRControl < SETUP_SCREEN_SAVER_BEGIN) ||
        (__SetupInfo.bSCRControl > SETUP_SCREEN_SAVER_END))
    {
        __SetupInfo.bSCRControl = SETUP_DEFAULT_SCREEN_SAVER;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_SCREEN_SAVER;
#endif
    }
#endif //#ifndef NO_SCREEN_SAVER
    
    if ((__SetupInfo.bProgressiveScan < SETUP_PROGRESSIVE_SCAN_BEGIN) ||
        (__SetupInfo.bProgressiveScan > SETUP_PROGRESSIVE_SCAN_END))
    {
        __SetupInfo.bProgressiveScan = SETUP_DEFAULT_PROGRESSIVE_SCAN;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_TV_SYTEM;
#endif
    }
    
    //Brian1.00-4, RELEASE_SETTING no VGA item
#ifdef NO_VGA_OUTPUT // Brian1.07 use this to replace RELEASE_SETTING	
    if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_VGA)
    {
        __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_NORMAL;
        //Brian1.06, adjust P-SCAN setting
        SETUP_ChangeValue(SETUP_SCREEN_PROGRESSIVE_SCAN);
    }
#endif    
    
    if ((__SetupInfo.bAudioDigitalOut < SETUP_AUDIO_DIGITAL_OUT_BEGIN) ||
        (__SetupInfo.bAudioDigitalOut > SETUP_AUDIO_DIGITAL_OUT_END))
    {
        __SetupInfo.bAudioDigitalOut = SETUP_DEFAULT_AUDIO_DIGITAL_OUT;
    }
    
    if ((__SetupInfo.bAudioAnalogOut < SETUP_AUDIO_ANALOG_OUT_BEGIN) ||
        (__SetupInfo.bAudioAnalogOut > SETUP_AUDIO_ANALOG_OUT_END))
    {
        __SetupInfo.bAudioAnalogOut = SETUP_DEFAULT_AUDIO_ANALOG_OUT;
    }   
    
    if ((__SetupInfo.bAudioDownMix < SETUP_AUDIO_DOWNMIX_BEGIN) ||
        (__SetupInfo.bAudioDownMix > SETUP_AUDIO_DOWNMIX_END))
    {
        __SetupInfo.bAudioDownMix = SETUP_DEFAULT_AUDIO_DOWNMIX;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_AUDIO_DOWNMIX;
#endif
    }
    
    if ((__SetupInfo.bAudioPrologic < SETUP_AUDIO_PROLOGIC_BEGIN) ||
        (__SetupInfo.bAudioPrologic > SETUP_AUDIO_PROLOGIC_END))
    {
        __SetupInfo.bAudioPrologic = SETUP_DEFAULT_AUDIO_PROLOGIC;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_AUDIO_PROLOGIC;
#endif
    }

#ifndef IMAGE_FRAME_SETUP

    if ((__SetupInfo.bAudioLPCM < SETUP_AUDIO_LPCM_BEGIN) ||
        (__SetupInfo.bAudioLPCM > SETUP_AUDIO_LPCM_END))
    {
        __SetupInfo.bAudioLPCM = SETUP_DEFAULT_AUDIO_LPCM;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_AUDIO_LPCM;
#endif
    }
    
    if ((__SetupInfo.bAudioCenter < SETUP_AUDIO_CENTER_BEGIN) ||
        (__SetupInfo.bAudioCenter > SETUP_AUDIO_CENTER_END))
    {
        __SetupInfo.bAudioCenter = SETUP_DEFAULT_AUDIO_CENTER;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_AUDIO_CENTER;
#endif
    }
    
    if ((__SetupInfo.bAudioRear < SETUP_AUDIO_REAR_BEGIN) ||
        (__SetupInfo.bAudioRear > SETUP_AUDIO_REAR_END))
    {
        __SetupInfo.bAudioRear = SETUP_DEFAULT_AUDIO_REAR;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_AUDIO_REAR;
#endif
    }
    
    if ((__SetupInfo.bAudioSubwoofer < SETUP_AUDIO_SUBWOOFER_BEGIN) ||
        (__SetupInfo.bAudioSubwoofer > SETUP_AUDIO_SUBWOOFER_END))
    {
        __SetupInfo.bAudioSubwoofer = SETUP_DEFAULT_AUDIO_SUBWOOFER;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_AUDIO_SUBWOOFER;
#endif
    } 
    
    if ((__SetupInfo.bSmartPlay < SETUP_CUSTOM_SMART_PLAY_BEGIN) ||
        (__SetupInfo.bSmartPlay > SETUP_CUSTOM_SMART_PLAY_END))
    {
        __SetupInfo.bSmartPlay = SETUP_DEFAULT_SMART_PLAY;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_SMART_PLAY;
#endif
    }   

#endif //#ifndef IMAGE_FRAME_SETUP
    
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
    if ((__SetupInfo.bExSPSTLanguage < SETUP_LANGUAGE_ENCODING_BEGIN) ||
        (__SetupInfo.bExSPSTLanguage > SETUP_LANGUAGE_ENCODING_END-1))
    {
        __SetupInfo.bExSPSTLanguage = SETUP_LANGUAGE_DEFAULT_ENCODING;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_EX_SPST_LANGUAGE;
#endif
    }
//#else //SUPPORT_CHAR_ENCODING_SWITCH
/*
    if ((__SetupInfo.bExSPSTLanguage < SETUP_LANGUAGE_EX_SP_BEGIN) ||
        (__SetupInfo.bExSPSTLanguage > SETUP_LANGUAGE_EX_SP_END))
    {
        __SetupInfo.bExSPSTLanguage = SETUP_DEFAULT_EX_SPST_LANGUAGE;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_EX_SPST_LANGUAGE;
#endif
    }
*/
//#endif //SUPPORT_CHAR_ENCODING_SWITCH    
    
    // Brian1.08
    if ((__SetupInfo.bAudioPrologic < SETUP_AUDIO_PROLOGIC_BEGIN) ||
        (__SetupInfo.bAudioPrologic > SETUP_AUDIO_PROLOGIC_END))
    {
        __SetupInfo.bAudioPrologic = SETUP_DEFAULT_AUDIO_PROLOGIC;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_AUDIO_PROLOGIC;
#endif
    }
    
    // alan, CT909
    if ((__SetupInfo.bAudioDownMix < SETUP_AUDIO_DOWNMIX_BEGIN) ||
        (__SetupInfo.bAudioDownMix > SETUP_AUDIO_DOWNMIX_END))
    {
        __SetupInfo.bAudioDownMix = SETUP_DEFAULT_AUDIO_DOWNMIX;
        
#ifdef _DEBUG_INFO
        _bEEPROMError = SETUP_ADDR_AUDIO_DOWNMIX;
#endif
        
    }

#ifdef IMAGE_FRAME_SETUP
    //always set it as normal
    __SetupInfo.bVideoOutput = SETUP_DEFAULT_VIDEO_OUTPUT;

    //Interval Time
    //__ImageFrameSetupInfo.bPhotoIntervalTime = __SetupInfo.bAudioLanguage; //use the address
    if ((__ImageFrameSetupInfo.bPhotoIntervalTime < SETUP_PHOTO_SETTING_INTERVAL_TIME_BEGIN) || (__ImageFrameSetupInfo.bPhotoIntervalTime > SETUP_PHOTO_SETTING_INTERVAL_TIME_END))
    {
        __ImageFrameSetupInfo.bPhotoIntervalTime = SETUP_PHOTO_SETTING_DEFAULT_INTERVAL_TIME;
    }

    //__ImageFrameSetupInfo.bTransitionEffect = __SetupInfo.bSPSTLanguage; //use the address
    if ((__ImageFrameSetupInfo.bTransitionEffect < SETUP_PHOTO_SETTING_TRANSITION_EFFECT_BEGIN) || (__ImageFrameSetupInfo.bTransitionEffect > SETUP_PHOTO_SETTING_TRANSITION_EFFECT_END))
    {
        __ImageFrameSetupInfo.bTransitionEffect = SETUP_PHOTO_SETTING_DEFAULT_TRANSITION_EFFECT;
    }

    //__ImageFrameSetupInfo.bBrowse = __SetupInfo.bMenuLanguage; //use the address
    if ((__ImageFrameSetupInfo.bBrowse < SETUP_PHOTO_SETTING_BROWSE_BEGIN) || (__ImageFrameSetupInfo.bBrowse > SETUP_PHOTO_SETTING_BROWSE_END))
    {
        __ImageFrameSetupInfo.bBrowse = SETUP_PHOTO_SETTING_DEFAULT_BROWSE;
    }

    //__ImageFrameSetupInfo.bAutoPlayPhoto = __SetupInfo.bAudioKaraoke; //use the address
    if ((__ImageFrameSetupInfo.bAutoPlayPhoto < SETUP_AUTO_PLAY_PHOTO_BEGIN) || (__ImageFrameSetupInfo.bAutoPlayPhoto > SETUP_AUTO_PLAY_PHOTO_END))
    {
        __ImageFrameSetupInfo.bAutoPlayPhoto = SETUP_AUTO_PLAY_DEFAULT_PHOTO;
    }

    //__ImageFrameSetupInfo.bAutoPlayMusic = __SetupInfo.bAudioDNR; //use the address
    if ((__ImageFrameSetupInfo.bAutoPlayMusic < SETUP_AUTO_PLAY_MUSIC_BEGIN) || (__ImageFrameSetupInfo.bAutoPlayMusic > SETUP_AUTO_PLAY_MUSIC_END))
    {
        __ImageFrameSetupInfo.bAutoPlayMusic = SETUP_AUTO_PLAY_DEFAULT_MUSIC;
    }

    //__ImageFrameSetupInfo.bAutoPlayPhotoPlusMusic = __SetupInfo.bAudioLPCM; //use the address
    if ((__ImageFrameSetupInfo.bAutoPlayPhotoPlusMusic < SETUP_AUTO_PLAY_PHOTO_PLUS_MUSIC_BEGIN) || (__ImageFrameSetupInfo.bAutoPlayPhotoPlusMusic > SETUP_AUTO_PLAY_PHOTO_PLUS_MUSIC_END))
    {
        __ImageFrameSetupInfo.bAutoPlayPhotoPlusMusic = SETUP_AUTO_PLAY_DEFAULT_PHOTO_PLUS_MUSIC;
    }

    //__ImageFrameSetupInfo.bAutoPlayMovie = __SetupInfo.bVCDControl; //use the address
    if ((__ImageFrameSetupInfo.bAutoPlayMovie < SETUP_AUTO_PLAY_MOVIE_BEGIN) || (__ImageFrameSetupInfo.bAutoPlayMovie > SETUP_AUTO_PLAY_MOVIE_END))
    {
        __ImageFrameSetupInfo.bAutoPlayMovie = SETUP_AUTO_PLAY_DEFAULT_MOVIE;
    }

#ifdef SUPPORT_DSPLAY_16_9_AND_AUTO
   // __ImageFrameSetupInfo.bDisplayType = __SetupInfo.bRegionControl; //use the address
    if ((__ImageFrameSetupInfo.bDisplayType < SETUP_DISPLAY_TYPE_BEGIN) || (__ImageFrameSetupInfo.bDisplayType > SETUP_DISPLAY_TYPE_END))
    {
        __ImageFrameSetupInfo.bDisplayType = SETUP_DISPLAY_DEFAULT_DISPLAY_TYPE;
		 printf("\n SETUP_DISPLAY_DEFAULT_DISPLAY_TYPE");	
    }
#endif
 
#ifdef SUPPORT_SAVE_REPEAT_IN_FLASH
 if ((__ImageFrameSetupInfo.bRepeatMode < SETUP_CUSTOM_REPEAT_MODE_BEGIN) || (__ImageFrameSetupInfo.bRepeatMode > SETUP_CUSTOM_REPEAT_MODE_END))
    {
        __ImageFrameSetupInfo.bRepeatMode = SETUP_CUSTOM_REPEAT_MODE_OFF;
	
    }
    DVDSETUP_SetRepeatMode();
#endif
    //__ImageFrameSetupInfo.bAlarmEnable = __SetupInfo.bAudioCenter;
    if ((__ImageFrameSetupInfo.bAlarmEnable < ALARM_ENABLE_BEGIN) || (__ImageFrameSetupInfo.bAlarmEnable > ALARM_ENABLE_END))
    {
        __ImageFrameSetupInfo.bAlarmEnable = SETUP_ALARM_DEFAULT_MODE;
    }

    //__ImageFrameSetupInfo.bAutoPowerOnEnable = __SetupInfo.bAudioRear;
    if ((__ImageFrameSetupInfo.bAutoPowerOnEnable < AUTOPWR_POWER_ON_ENABLE_BEGIN) || (__ImageFrameSetupInfo.bAutoPowerOnEnable > AUTOPWR_POWER_ON_ENABLE_END))
    {
        __ImageFrameSetupInfo.bAutoPowerOnEnable = SETUP_AUTO_POWER_ON_DEFAULT_MODE;
    }

    //__ImageFrameSetupInfo.bAutoPowerOffEnable = __SetupInfo.bAudioSubwoofer;
    if ((__ImageFrameSetupInfo.bAutoPowerOffEnable < AUTOPWR_POWER_OFF_ENABLE_BEGIN) || (__ImageFrameSetupInfo.bAutoPowerOffEnable > AUTOPWR_POWER_OFF_ENABLE_END))
    {
        __ImageFrameSetupInfo.bAutoPowerOffEnable = SETUP_AUTO_POWER_OFF_DEFAULT_MODE;
    }

    //For TFT relative attribute
    DVDSETUP_SetDefaultTFTAttribute();

    //__ImageFrameSetupInfo.wAlarmTime = __SetupInfo.wMenuLanguageCode;
    //__ImageFrameSetupInfo.wAutoPowerOnTime = __SetupInfo.wAudioLanguageCode;
    //__ImageFrameSetupInfo.wAutoPowerOffTime = __SetupInfo.wSPSTLanguageCode;

#endif //#ifdef IMAGE_FRAME_SETUP

#endif //#ifdef WRITE_EEPROM_ENABLE 
}


//  ***************************************************************************
//  Function    : _WriteSetupData
//  Description : This function will write SETUP data to EEPROM and initialize __SetupInfo
//  Arguments   : bSetup, the status to be processed.
//  Return      : none.
//  Side Effect :
//  ***************************************************************************
void    _WriteSetupData(BYTE bSetup, BYTE bDefaultPassword)
{
    BYTE *pbSetupPtr;
    BYTE *pbImageFrameSetupPtr;
    BYTE bTmp=0xff;
    BYTE i;

#ifndef IMAGE_FRAME_SETUP
    _bVideoSettingChange = TRUE;
#endif //IMAGE_FRAME_SETUP

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
    // always clear current EEPROM data
    if (SETUP_DATA_RESET == bSetup)
    {
        for (i = 0; i < EPROM_IDENTIFY_BYTE; i++)
            HAL_WriteStorage(SETUP_ADDR_IDENTIFY+i, &bTmp, 1);
        return;
    }
    else
#endif
    // do 1st setup data initialization
    if (bSetup==SETUP_DATA_DEFAULT)
    {
        // write winbond password
        //Here will do setup value initialization.
        // initial data
        // initial data
        // update setup version

        __SetupInfo.bSetupVersion = SETUP_VERSION;
        __SetupInfo.bInterVersion = INTER_VERSION;

        //if (bDefaultPassword)
        // Brian1.24, check if set to default password
        if(bDefaultPassword & USE_DEFAULT_PASSWORD)
        {
            __SetupInfo.wDefaultPassword=SETUP_DEFAULT_PASSWORD;
            __SetupInfo.bParentalCtl = SETUP_DEFAULT_PARENTAL;
        }

#ifndef NO_SCREEN_SAVER // Brian1.20
        __SetupInfo.bSCRControl = SETUP_DEFAULT_SCREEN_SAVER;
#endif
#ifdef SUPPORT_ADJUST_CHROM_PHASE // Brian1.25, Chroma phase
        __SetupInfo.bChromPhaseOffset = SETUP_DEFAULT_CHROM_PHASE;
#endif		
        __SetupInfo.bAudioDigitalOut = SETUP_DEFAULT_AUDIO_DIGITAL_OUT;
        __SetupInfo.bAudioAnalogOut = SETUP_DEFAULT_AUDIO_ANALOG_OUT;
        __SetupInfo.bAudioDownMix = SETUP_DEFAULT_AUDIO_DOWNMIX;
        __SetupInfo.bAudioPrologic = SETUP_DEFAULT_AUDIO_PROLOGIC; // Brian1.08
//#ifndef IMAGE_FRAME_SETUP
        __SetupInfo.bAudioCenter = SETUP_DEFAULT_AUDIO_CENTER;
        __SetupInfo.bAudioRear = SETUP_DEFAULT_AUDIO_REAR;
        __SetupInfo.bAudioSubwoofer = SETUP_DEFAULT_AUDIO_SUBWOOFER;
        __SetupInfo.bAudioLPCM = SETUP_DEFAULT_AUDIO_LPCM;
        __SetupInfo.bAudioDNR = SETUP_DEFAULT_AUDIO_DNR;
        __SetupInfo.bAudioKaraoke = SETUP_DEFAULT_AUDIO_KARAOKE;
//#endif //IMAGE_FRAME_SETUP

#ifdef IMAGE_FRAME_SETUP
    //__SetupInfo.bAudioLanguage = SETUP_PHOTO_SETTING_DEFAULT_INTERVAL_TIME; //Interval Time
    //__SetupInfo.bSPSTLanguage = SETUP_PHOTO_SETTING_DEFAULT_TRANSITION_EFFECT; //Transition Effect
    //__SetupInfo.bMenuLanguage = SETUP_PHOTO_SETTING_DEFAULT_BROWSE; //Browse
    //__SetupInfo.bAudioKaraoke = SETUP_AUTO_PLAY_DEFAULT_PHOTO; //Auto-Play photo
    //__SetupInfo.bAudioDNR = SETUP_AUTO_PLAY_DEFAULT_MUSIC; //Auto-play music
    //__SetupInfo.bAudioLPCM = SETUP_AUTO_PLAY_DEFAULT_PHOTO_PLUS_MUSIC; //Auto-play Photo+Music
    //__SetupInfo.bVCDControl = SETUP_AUTO_PLAY_DEFAULT_MOVIE; //Auto-play movie
//    __SetupInfo.bRegionControl = SETUP_DISPLAY_DEFAULT_DISPLAY_TYPE; //Display type
    __SetupInfo.bTVSystem = SETUP_DEFAULT_TV_SYSTEM; //TV standard
    __SetupInfo.bBrightness = SETUP_DEFAULT_BRIGHTNESS; //Brightness
    __SetupInfo.bContrast = SETUP_DEFAULT_CONTRAST; //Contrast
    __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE; //OSD
    __SetupInfo.bUIStyle = SETUP_DEFAULT_UI_STYLE;

    //TFT part
    __TFTPicAttr.bBrightness=SETUP_DEFAULT_TFT_BRIGHTNESS;
    __TFTPicAttr.bContrast=SETUP_DEFAULT_TFT_CONTRAST;
    __TFTPicAttr.bSaturation=SETUP_DEFAULT_TFT_SATURATION;

    /*
    __SetupInfo.bAudioCenter = SETUP_ALARM_DEFAULT_MODE;
    __SetupInfo.bAudioRear = SETUP_AUTO_POWER_ON_DEFAULT_MODE;
    __SetupInfo.bAudioSubwoofer = SETUP_AUTO_POWER_OFF_DEFAULT_MODE;
    __SetupInfo.wMenuLanguageCode = 0;
    __SetupInfo.wAudioLanguageCode = 0;
    __SetupInfo.wSPSTLanguageCode = 0;
 
    __ImageFrameSetupInfo.bPhotoIntervalTime = __SetupInfo.bAudioLanguage; //use the address
    __ImageFrameSetupInfo.bTransitionEffect = __SetupInfo.bSPSTLanguage; //use the address
    __ImageFrameSetupInfo.bBrowse = __SetupInfo.bMenuLanguage; //use the address
    __ImageFrameSetupInfo.bAutoPlayPhoto = __SetupInfo.bAudioKaraoke; //use the address
    __ImageFrameSetupInfo.bAutoPlayMusic = __SetupInfo.bAudioDNR; //use the address
    __ImageFrameSetupInfo.bAutoPlayPhotoPlusMusic = __SetupInfo.bAudioLPCM; //use the address
    __ImageFrameSetupInfo.bAutoPlayMovie = __SetupInfo.bVCDControl; //use the address
    __ImageFrameSetupInfo.bDisplayType = __SetupInfo.bRegionControl; //use the address
    __ImageFrameSetupInfo.bRepeatMode = SETUP_CUSTOM_REPEAT_MODE_OFF;

    __ImageFrameSetupInfo.bAlarmEnable = __SetupInfo.bAudioCenter;
    __ImageFrameSetupInfo.bAutoPowerOnEnable = __SetupInfo.bAudioRear;
    __ImageFrameSetupInfo.bAutoPowerOffEnable = __SetupInfo.bAudioSubwoofer;
    __ImageFrameSetupInfo.wAlarmTime = __SetupInfo.wMenuLanguageCode;
    __ImageFrameSetupInfo.wAutoPowerOnTime = __SetupInfo.wAudioLanguageCode;
    __ImageFrameSetupInfo.wAutoPowerOffTime = __SetupInfo.wSPSTLanguageCode;
    */
    // DMP_116, set default value    
    __ImageFrameSetupInfo.bPhotoIntervalTime = SETUP_PHOTO_SETTING_DEFAULT_INTERVAL_TIME;
    __ImageFrameSetupInfo.bTransitionEffect = SETUP_PHOTO_SETTING_DEFAULT_TRANSITION_EFFECT;
    __ImageFrameSetupInfo.bBrowse = SETUP_PHOTO_SETTING_DEFAULT_BROWSE;
    __ImageFrameSetupInfo.bAutoPlayPhoto = SETUP_AUTO_PLAY_DEFAULT_PHOTO;
    __ImageFrameSetupInfo.bAutoPlayMusic = SETUP_AUTO_PLAY_DEFAULT_MUSIC;
    __ImageFrameSetupInfo.bAutoPlayPhotoPlusMusic = SETUP_AUTO_PLAY_DEFAULT_PHOTO_PLUS_MUSIC;
    __ImageFrameSetupInfo.bAutoPlayMovie = SETUP_AUTO_PLAY_DEFAULT_MOVIE;
    //__ImageFrameSetupInfo.bDisplayType = SETUP_DISPLAY_DEFAULT_DISPLAY_TYPE;
    __ImageFrameSetupInfo.bRepeatMode = SETUP_CUSTOM_REPEAT_MODE_OFF;

    __ImageFrameSetupInfo.bAlarmEnable = SETUP_ALARM_DEFAULT_MODE;
    __ImageFrameSetupInfo.bAutoPowerOnEnable = SETUP_AUTO_POWER_ON_DEFAULT_MODE;
    __ImageFrameSetupInfo.bAutoPowerOffEnable = SETUP_AUTO_POWER_OFF_DEFAULT_MODE;
    __ImageFrameSetupInfo.wAlarmTime = 0;
    __ImageFrameSetupInfo.wAutoPowerOnTime = 0;
    __ImageFrameSetupInfo.wAutoPowerOffTime = 0;
#ifdef SUPPORT_DSPLAY_16_9_AND_AUTO    
    __ImageFrameSetupInfo.bDisplayType = SETUP_DISPLAY_DEFAULT_DISPLAY_TYPE; //Display type
     printf("\n reset SETUP_DISPLAY_DEFAULT_DISPLAY_TYPE");	
#endif
#ifdef SUPPORT_SAVE_REPEAT_IN_FLASH    
    __ImageFrameSetupInfo.bRepeatMode = SETUP_REPEAT_DEFAULT_CUSTOM_MODE; //Display type
    DVDSETUP_SetRepeatMode();
#endif
  /*
  	 //__SetupInfo.bSmartPlay = HIBYTE(LOWORD(__dwVolAudio));
    //__SetupInfo.bAngleMarkDisplay = LOBYTE(LOWORD(__dwVolAudio));

    __SetupInfo.bSmartPlay = HIBYTE(LOWORD(VOLUME_DEFAULT));
    __SetupInfo.bAngleMarkDisplay = LOBYTE(LOWORD(VOLUME_DEFAULT));
    */

#else
    // Brian1.24, check if need to set to default
    if (bDefaultPassword & USE_DEFAULT_REGIONCODE) 
        __SetupInfo.bRegionControl = SETUP_DEFAULT_REGION;
    if (bDefaultPassword & USE_DEFAULT_PARENTAL_CTRL) 
        __SetupInfo.bParentalControl = SETUP_DEFAULT_PARENTAL_CTL;
    if (bDefaultPassword & USE_DEFAULT_APS_CTRL) 
        __SetupInfo.bAPSControl = SETUP_DEFAULT_APS_CTL;
    
    __SetupInfo.bMenuLanguage = SETUP_DEFAULT_MENU_LANGUAGE;
    __SetupInfo.bAudioLanguage = SETUP_DEFAULT_AUDIO_LANGUAGE;
    __SetupInfo.bSPSTLanguage = SETUP_DEFAULT_SPST_LANGUAGE;
    __SetupInfo.bUIStyle = SETUP_DEFAULT_UI_STYLE;
    __SetupInfo.bBrightness = SETUP_DEFAULT_BRIGHTNESS;
    __SetupInfo.bContrast = SETUP_DEFAULT_CONTRAST;
    __SetupInfo.bTVSystem = SETUP_DEFAULT_TV_SYSTEM;
    __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE;
    __SetupInfo.bSmartPlay = SETUP_DEFAULT_SMART_PLAY;
    __SetupInfo.bAngleMarkDisplay = SETUP_DEFAULT_ANGLE_MARK;
#endif //IMAGE_FRAME_SETUP

    __SetupInfo.bProgressiveScan = SETUP_DEFAULT_PROGRESSIVE_SCAN; // Brian0.81
    __SetupInfo.bAudioDigitalOut = SETUP_DEFAULT_AUDIO_DIGITAL_OUT; // Brian0.82
    __SetupInfo.bAudioAnalogOut = SETUP_DEFAULT_AUDIO_ANALOG_OUT; // Brian0.82
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
    __SetupInfo.bExSPSTLanguage = SETUP_LANGUAGE_DEFAULT_ENCODING;
//#else
    //__SetupInfo.bExSPSTLanguage = SETUP_DEFAULT_EX_SPST_LANGUAGE;
//#endif
    __SetupInfo.bAspectRatio=SETUP_DEFAULT_ASPECT;
    __SetupInfo.bVideoOutput = SETUP_DEFAULT_VIDEO_OUTPUT;
    
#ifndef NO_DISC_MODE
    // Brian1.05, fix the problem that check region code still error after it is restored as default 
    // just call NV_Setting, it will set __wSPRMs[20] always
    NV_Setting(SET_REGION_CODE, 0);
#endif //NO_DISC_MODE
    
    // TCC276, let 0x5A be TRUE.
    if (SETUP_DEFAULT_VCD_CTL == FALSE)
        __SetupInfo.bVCDControl = 0 ;
    else
        __SetupInfo.bVCDControl = 0x5A ;
    
    _SetAngleMarkTime(); //brian.170a
    }
    
    _TransferLanguageToSPRM();
    
    strncpy(__SetupInfo.cSetupIdentify, IDENTIFY_STRING, EPROM_IDENTIFY_BYTE - 1);
    
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
    pbSetupPtr=(BYTE *) &__SetupInfo;
    
    /*
    HAL_WriteStorage(SETUP_ADDR_BASE, pbSetupPtr, SETUP_DATA_BYTES);
    */
    
    for (i=0;i<SETUP_DATA_BYTES/8;i++)
    {
        HAL_WriteStorage(SETUP_ADDR_BASE+(i*8), pbSetupPtr+(i*8), 8);
    }
    
    if (SETUP_DATA_BYTES%8)
    {
        HAL_WriteStorage(SETUP_ADDR_BASE+8*(SETUP_DATA_BYTES/8),
            pbSetupPtr+8*(SETUP_DATA_BYTES/8), SETUP_DATA_BYTES%8);
    }
    
    // DMP_116
    pbImageFrameSetupPtr = (BYTE *) &__ImageFrameSetupInfo;
	
    for (i=0;i<IMAGE_FRAME_SETUP_DATA_BYTES/8;i++)
    {
        HAL_WriteStorage(IMAGE_FRAME_SETUP_ADDR_BASE+(i*8), pbImageFrameSetupPtr+(i*8), 8);
    }
    
    if (IMAGE_FRAME_SETUP_DATA_BYTES%8)
    {
        HAL_WriteStorage(IMAGE_FRAME_SETUP_ADDR_BASE+8*(IMAGE_FRAME_SETUP_DATA_BYTES/8),
            pbImageFrameSetupPtr+8*(IMAGE_FRAME_SETUP_DATA_BYTES/8), IMAGE_FRAME_SETUP_DATA_BYTES%8);
    }      

    HAL_WriteStorage(TFT_ADDR_BIRHGTNESS, &(__TFTPicAttr.bBrightness), 1);
    HAL_WriteStorage(TFT_ADDR_CONTRAST, &(__TFTPicAttr.bContrast), 1);
    HAL_WriteStorage(TFT_ADDR_SATURATION, &(__TFTPicAttr.bSaturation), 1);
    
    // LLY0.95, only need to clear the resume flag if save resume info. to EERPOM
#ifdef  SUPPORT_RESUME_FROM_EEPROM
    
    // LLY0.95, add protection code while out-of the pre-reserved space for resume
    if((SETUP_ADDR_RESUME_USED) > (SETUP_ADDR_RESUME_LAST))
    {
        printf("#error Out-of reserved EEPROM space for resume info. !!\n");
    }
    
    
    // Brian1.23, clear resume flag to "XXXXXX"
    strcpy ((char *)__bMsgString, "XXXXXX");
    HAL_WriteStorage(SETUP_ADDR_RESUME_FLAG, __bMsgString, RESUME_FLAG_LENGTH);
    
    // LLY1.00, clear the resume ID
    for(i=0; i<(RESUME_ID_ENTRY_SIZE); i++)
    {
        __cBuffer[i]=0;
    }
    for(i=0; i<RESUME_DISC_NUM; i++)
    {
        HAL_WriteStorage( (SETUP_ADDR_RESUME_DATA+i*RESUME_DATA_ENTRY_SIZE), 
            __cBuffer, (RESUME_ID_ENTRY_SIZE));
    }
    
#endif  // #ifdef SUPPORT_RESUME_FROM_EEPROM    
    
#endif  //#ifndef  SYSTE_8051
 #ifdef SUPPORT_DEFAULT_VOLUME_IN_FLASH
    DVDSETUP_DefaultVolume();
#endif

}


//  ***************************************************************************
//  Function    :   DVDSETUP_SaveDataToSetupInfo
//  Description :   This function will save the value to __SetupInfo. It will
//                  refer bLeftOrRight to decide which value should be assign to
//                  __SetupInfo.
//  Arguments   :   bID ==> the current ID.
//                  bLeftOrRight, bLeftOrRight == 0 ==>shift left.
//                                bLeftOrRight == 1 ==>shift right.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void DVDSETUP_SaveDataToSetupInfo(BYTE bMenuID)
{
#ifndef NO_SETUP
    switch (bMenuID)
    {
#ifdef IMAGE_FRAME_SETUP
    case SETUP_PHOTO_SETTING_INTERVAL_TIME:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_PHOTO_SETTING_INTERVAL_TIME, (BYTE *)&(__ImageFrameSetupInfo.bPhotoIntervalTime), 1);
#endif
        break;
    case SETUP_PHOTO_SETTING_TRANSITION_EFFECT:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_PHOTO_SETTING_TRANSITION_EFFECT, (BYTE *)&(__ImageFrameSetupInfo.bTransitionEffect), 1);
#endif
        break;
    case SETUP_PHOTO_SETTING_BROWSE:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_PHOTO_SETTING_BROWSE, (BYTE *)&(__ImageFrameSetupInfo.bBrowse), 1);
#endif
        break;
    case SETUP_AUTO_PLAY_PHOTO:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUTO_PLAY_PHOTO, (BYTE *)&(__ImageFrameSetupInfo.bAutoPlayPhoto), 1);
#endif
        break;
    case SETUP_AUTO_PLAY_MUSIC:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUTO_PLAY_MUSIC, (BYTE *)&(__ImageFrameSetupInfo.bAutoPlayMusic), 1);
#endif
        break;
    case SETUP_AUTO_PLAY_PHOTO_PLUS_MUSIC:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUTO_PLAY_PHOTO_PLUS_MUSIC, (BYTE *)&(__ImageFrameSetupInfo.bAutoPlayPhotoPlusMusic), 1);
#endif
        break;
    case SETUP_AUTO_PLAY_MOVIE:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUTO_PLAY_MOVIE, (BYTE *)&(__ImageFrameSetupInfo.bAutoPlayMovie), 1);
#endif
        break;
    case SETUP_CUSTOM_REPEAT_MODE:
#ifdef WRITE_EEPROM_ENABLE 
        HAL_WriteStorage(SETUP_ADDR_REPEAT_MODE, (BYTE *)&(__ImageFrameSetupInfo.bRepeatMode), 1);
#endif
		break;
    case SETUP_DISPLAY_TV_STANDARD:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_TV_SYTEM, (BYTE *)&(__SetupInfo.bTVSystem), 1);
#endif
        break;
 #ifdef SUPPORT_DSPLAY_16_9_AND_AUTO
    case SETUP_DISPLAY_DISPLAY_TYPE:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_DISPLAY_TYPE, (BYTE *)&(__ImageFrameSetupInfo.bDisplayType), 1);
#endif
        break;
 #endif
        
    case SETUP_DISPLAY_BRIGHTNESS:
        _SetBrightnessToIC();
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_BRIGHTNESS, (BYTE *)&(__SetupInfo.bBrightness), 1);
#endif
        break;
    case SETUP_DISPLAY_CONTRAST:
        _SetContrastToIC();
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_CONTRAST, (BYTE *)&(__SetupInfo.bContrast), 1);
#endif
        break;
    case SETUP_DISPLAY_TFT_BRIGHTNESS:
#ifdef WRITE_EEPROM_ENABLE
        Vipor_SetPicture_Brightness(TFT_BRIGHTNESS_VALUE_LIST[__TFTPicAttr.bBrightness-SETUP_DISPLAY_TFT_BRIGHTNESS_BEGIN]);
        HAL_WriteStorage(TFT_ADDR_BIRHGTNESS, &(__TFTPicAttr.bBrightness), 1);
#endif
        break;
    case SETUP_DISPLAY_TFT_CONTRAST:
#ifdef WRITE_EEPROM_ENABLE
        Vipor_SetPicture_Contrast(TFT_CONTRAST_VALUE_LIST[__TFTPicAttr.bContrast-SETUP_DISPLAY_TFT_CONTRAST_BEGIN]);
        HAL_WriteStorage(TFT_ADDR_CONTRAST, &(__TFTPicAttr.bContrast), 1);
#endif
        break;
    case SETUP_DISPLAY_TFT_SATURATION:
#ifdef WRITE_EEPROM_ENABLE
        Vipor_SetPicture_Saturation(TFT_SATURATION_VALUE_LIST[__TFTPicAttr.bSaturation-SETUP_DISPLAY_TFT_SATURATION_BEGIN]);
        HAL_WriteStorage(TFT_ADDR_SATURATION, &(__TFTPicAttr.bSaturation), 1);
#endif
        break;
    case SETUP_CUSTOM_OSD:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_OSD_LANGUAGE, (BYTE *)&(__SetupInfo.bOSDLanguage), 1);
#endif
        break;
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
    case SETUP_LANGUAGE_ENCODING:
        OSD_ReplaceFont();
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_EX_SPST_LANGUAGE, (BYTE *)&(__SetupInfo.bExSPSTLanguage), 1);
#endif
        break;
//#endif
    case SETUP_CUSTOM_UI_STYLE:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_UI_STYLE, (BYTE *)&(__SetupInfo.bUIStyle), 1);
#endif
        break;
#else //IMAGE_FRAME_SETUP
    case SETUP_LANGUAGE_MENU:
        //__SetupInfo.bMenuLanguage = __bMenuLanguage;
        _TransferLanguageToSPRM();
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_MENU_LANGUAGE, (BYTE *)&(__SetupInfo.bMenuLanguage), 1);
#endif
        break;
    case SETUP_LANGUAGE_AUDIO:
        //__SetupInfo.bAudioLanguage = __bAudioLanguage;
        _TransferLanguageToSPRM();
        // tell NV to re-get audio stream number
        __wSPRMs[1]=0xf;
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUDIO_LANGUAGE, (BYTE *)&(__SetupInfo.bAudioLanguage), 1);
#endif
        break;
    case SETUP_LANGUAGE_SUBTITLE:
        //__SetupInfo.bSPSTLanguage = __bSPSTLanguage;
        _TransferLanguageToSPRM();
        
        if (__wSPRMs[18]==0x3030)   // means SP off by Setup
            __wSPRMs[2]=0x3f;
        else
        {
            __wSPRMs[2] &= 0xFFC0;  // clear bits 0~5
            __wSPRMs[2] |= 0x003E;  // assign bits 0~5, means SP not choosed yet.
        }
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_SPST_LANGUAGE, (BYTE *)&(__SetupInfo.bSPSTLanguage), 1);
#endif
        break;
        
#ifdef SUPPORT_CHAR_SUBPICTURE
    case SETUP_LANGUAGE_EX_SUBTITLE:
        CHAR_SP_ReplaceFont();
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_EX_SPST_LANGUAGE, (BYTE *)&(__SetupInfo.bExSPSTLanguage), 1);
#endif
        break;
#endif
        
#ifndef NO_SCREEN_SAVER // Brian1.20
    case SETUP_SCREEN_SAVER:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_SCREEN_SAVER, (BYTE*)&(__SetupInfo.bSCRControl), 1);
#endif
        _bVideoSettingChange = TRUE;
        break;
#endif
        
#ifdef SUPPORT_ADJUST_CHROM_PHASE // Brian1.25, Chroma phase
    case SETUP_SCREEN_CHROM_PHASE:
        HAL_AdjustTVMode(DISP_NEXTVSYNC); 
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_CHROM_PHASE, __SetupInfo.bChromPhaseOffset, 1);
#endif
        _bVideoSettingChange = TRUE;
        break;
#endif
        
    case SETUP_SCREEN_PROGRESSIVE_SCAN:
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        //Kevin0.87b, modify
        //HAL_WriteStorage(SETUP_ADDR_VIDEO_OUTPUT, __SetupInfo.bVideoOutput);
        HAL_WriteStorage(SETUP_ADDR_SCAN_MODE, (BYTE *)&(__SetupInfo.bProgressiveScan), 1);
#endif
        _bVideoSettingChange = TRUE;
        break;
        
    case SETUP_AUDIO_OUTPUT:
        _bAudioTypeChange = TRUE;
        
        if (__SetupInfo.bAudioOutput != SETUP_AUDIO_OUTPUT_51CH)
        {
        }
        else
        {
        }
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUDIO_OUTPUT, (BYTE *)&(__SetupInfo.bAudioOutput), 1);
#endif
        break;
        
        // Brian0.84-2nd
    case SETUP_AUDIO_DIGITAL_OUT:
        _bAudioTypeChange = TRUE;
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        //Kevin0.87b, modify
        HAL_WriteStorage(SETUP_ADDR_AUDIO_DIGITAL_OUT, (BYTE *)&(__SetupInfo.bAudioDigitalOut), 1);
#endif
        break;
        
    case SETUP_AUDIO_ANALOG_OUT:
        _bAudioTypeChange = TRUE;
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        //Kevin0.87b, modify
        HAL_WriteStorage(SETUP_ADDR_AUDIO_ANALOG_OUT, (BYTE *)&(__SetupInfo.bAudioAnalogOut), 1);
#endif
        
        //Brian1.06
        _DVDSETUP_SetAnalogEnable();
        
        break;        
        
    case SETUP_AUDIO_DOWNMIX: // Alan, CT909
#ifdef WRITE_EEPROM_ENABLE
        HAL_WriteStorage(SETUP_ADDR_AUDIO_DOWNMIX, (BYTE *)&(__SetupInfo.bAudioDownMix), 1);
#endif
        _SetDownMix();
        break;
        
    case SETUP_AUDIO_CENTER:
        _bAudioTypeChange = TRUE;
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUDIO_CENTER, (BYTE *)&(__SetupInfo.bAudioCenter), 1);
#endif
        break;
    case SETUP_AUDIO_REAR:
        _bAudioTypeChange = TRUE;
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUDIO_REAR, (BYTE *)&(__SetupInfo.bAudioRear), 1);
#endif
        break;
    case SETUP_AUDIO_SUBWOOFER:
        _bAudioTypeChange = TRUE;
        
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUDIO_SUBWOOFER, (BYTE *)&(__SetupInfo.bAudioSubwoofer), 1);
#endif
        break;
        
        _SetDNRToIC();
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUDIO_DNR, (BYTE *)&(__SetupInfo.bAudioDNR), 1);
#endif
        break;
    case SETUP_AUDIO_KARAOKE:
        _bAudioTypeChange = TRUE;
        
        if ((__SetupInfo.bAudioKaraoke - SETUP_AUDIO_KARAOKE_BASE) == 0) //on
            CHIPS_MICControl(CHIPS_ON);
        else if ((__SetupInfo.bAudioKaraoke - SETUP_AUDIO_KARAOKE_BASE) == 1) //off
            CHIPS_MICControl(CHIPS_OFF);
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUDIO_KARAOKE, (BYTE *)&(__SetupInfo.bAudioKaraoke), 1);
#endif
        break;
        
#ifndef CT950_STYLE
#ifdef SUPPORT_DVD_SMRTPB
    case SETUP_CUSTOM_SMART_PLAY:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_SMART_PLAY, (BYTE *)&(__SetupInfo.bSmartPlay), 1);
#endif
        break;
#endif
        
    case SETUP_CUSTOM_ANGLE_MARK:
        _SetAngleMarkTime(); //brian.170a
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_ANGLE_MARK_DISPLAY, (BYTE *)&(__SetupInfo.bAngleMarkDisplay), 1);
#endif
        break;

#endif //CT950_STYLE
        
    case SETUP_CUSTOM_DEFAULT_VALUE:
        // Brian1.06
        if (__SetupInfo.bOSDLanguage != SETUP_DEFAULT_OSD_LANGUAGE)
        {
            __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE;
            __bOSDLanguage = __SetupInfo.bOSDLanguage;
            //_bOSDLang = __bOSDLanguage - SETUP_LANGUAGE_OSD_BASE;
        }
        
        // Brian1.24, set default according to the flag in SETUP_CUSTOM_READ_DEFAULT
        _WriteSetupData(SETUP_DATA_DEFAULT, SETUP_CUSTOM_READ_DEFAULT);
        
        _SetContrastToIC();                   
        
        _SetBrightnessToIC();
        
        //Brian1.06
        _DVDSETUP_SetAnalogEnable();
        // Brian1.11b
        _bAudioTypeChange = TRUE;
        break;
                    
    case SETUP_CUSTOM_PARENTAL:
        if (SETUP_CUSTOM_PC_LEVEL_OFF == __SetupInfo.bParentalCtl)
            __SetupInfo.bParentalControl = FALSE;
        else
            __SetupInfo.bParentalControl = TRUE;
        
        
        NV_Setting(SET_PARENTAL_CONTROL, NULL_0);
        
        _bParentalControlChange = TRUE;
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_PARENTAL_CTL_LEVEL, (BYTE*)&(__SetupInfo.bParentalCtl), 1);
        HAL_WriteStorage(SETUP_ADDR_PARENTAL_CTL, (BYTE*)&(__SetupInfo.bParentalControl), 1);
#endif
        break;
#endif //#ifndef IMAGE_FRAME_SETUP
    default:
        break;
        }
#endif
}

void SETUP_LoadLanguage(void)
{
    PSECTION_ENTRY pSection;
    pSection = ROMLD_GetSectionByName(0, "Engl");

#ifdef SUPPORT_STB
    switch (__SetupInfo.bOSDLanguage)
    {
#if (OSD_LANGUAGE_GROUP == CHINA_AREA_OSD)
    case MENU_SYS_LANGUAGE_OSD_CHINESE:
        ROMLD_LoadSectionToByName(0, "Chin", pSection->dwLMA, 0);
        break;
#elif (OSD_LANGUAGE_GROUP == EUROPE_AREA_OSD)
    case MENU_SYS_LANGUAGE_OSD_FRENCH:
        ROMLD_LoadSectionToByName(0, "Fren", pSection->dwLMA, 0);
        break;
    case MENU_SYS_LANGUAGE_OSD_GERMAN:
        ROMLD_LoadSectionToByName(0, "Germ", pSection->dwLMA, 0);
        break;
    case MENU_SYS_LANGUAGE_OSD_ITALIAN:
        ROMLD_LoadSectionToByName(0, "Ital", pSection->dwLMA, 0);
        break;
    case MENU_SYS_LANGUAGE_OSD_SPANISH:
        ROMLD_LoadSectionToByName(0, "Span", pSection->dwLMA, 0);
        break;
    case MENU_SYS_LANGUAGE_OSD_PORTUGUESE:
        ROMLD_LoadSectionToByName(0, "Port", pSection->dwLMA, 0);
        break;
    case MENU_SYS_LANGUAGE_OSD_DUTCH:
        ROMLD_LoadSectionToByName(0, "Dutc", pSection->dwLMA, 0);
        break;
    case MENU_SYS_LANGUAGE_OSD_POLISH:
        ROMLD_LoadSectionToByName(0, "Poli", pSection->dwLMA, 0);
        break;
#endif
    case MENU_SYS_LANGUAGE_OSD_ENGLISH:
    default:
        ROMLD_LoadSectionToByName(0, "Engl", pSection->dwLMA, 0);
        break;
    }
#else
    switch (__SetupInfo.bOSDLanguage)
    {
#if (OSD_LANGUAGE_GROUP == CHINA_AREA_OSD)
    case SETUP_CUSTOM_OSD_CHINESE:
        ROMLD_LoadSectionToByName(0, "Chin", pSection->dwLMA, 0);
        break;
#elif (OSD_LANGUAGE_GROUP == JAPAN_AREA_OSD)
    case SETUP_CUSTOM_OSD_JAPANESE:
        ROMLD_LoadSectionToByName(0, "Japa", pSection->dwLMA, 0);
        break;
#elif (OSD_LANGUAGE_GROUP == KOREAN_AREA_OSD)
    case SETUP_CUSTOM_OSD_KOREAN:
        ROMLD_LoadSectionToByName(0, "Kore", pSection->dwLMA, 0);
        break;
#elif (OSD_LANGUAGE_GROUP == EUROPE_AREA_OSD)
    case SETUP_CUSTOM_OSD_FRENCH:
        ROMLD_LoadSectionToByName(0, "Fren", pSection->dwLMA, 0);
        break;
    case SETUP_CUSTOM_OSD_GERMAN:
        ROMLD_LoadSectionToByName(0, "Germ", pSection->dwLMA, 0);
        break;
    case SETUP_CUSTOM_OSD_ITALIAN:
        ROMLD_LoadSectionToByName(0, "Ital", pSection->dwLMA, 0);
        break;
    case SETUP_CUSTOM_OSD_SPANISH:
        ROMLD_LoadSectionToByName(0, "Span", pSection->dwLMA, 0);
        break;
    case SETUP_CUSTOM_OSD_PORTUGUESE:
        ROMLD_LoadSectionToByName(0, "Port", pSection->dwLMA, 0);
        break;
    case SETUP_CUSTOM_OSD_DUTCH:
        ROMLD_LoadSectionToByName(0, "Dutc", pSection->dwLMA, 0);
        break;
    case SETUP_CUSTOM_OSD_POLISH:
        ROMLD_LoadSectionToByName(0, "Poli", pSection->dwLMA, 0);
        break;
#endif
    case SETUP_CUSTOM_OSD_ENGLISH:
    default:
        ROMLD_LoadSectionToByName(0, "Engl", pSection->dwLMA, 0);
        break;
    }
#endif //SUPPORT_STB
}


//  ***************************************************************************
//  Function    :   _DVDSETUP_AdjustSetting(BYTE bID)
//  Description :   This function will be called the setting is changed.
//  Arguments   :   bID, the ID of the setting to be changed
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void _DVDSETUP_AdjustSetting(BYTE bID)
{
#ifndef IMAGE_FRAME_SETUP //CoCo2.38
    switch (bID)
    {
    case SETUP_MAIN_AUDIO:
// DVD_275b+, SUPPORT_VIRTUAL_DTS
#ifdef  SUPPORT_VIRTUAL_DTS
        // 1) when user select stream is DTS stream, and output is SPDIF_RAW
        //    must choose real DTS stream to play.
        // 2) Current play DTS stream, and output change to non-SPDIF/RAW
        //    must choose a non-DTS stream to play.
        if ( __SetupInfo.bAudioAnalogOut == SETUP_AUDIO_DIGITAL_OUT_SPDIF_RAW ) // Brian1.23, use bAudioAnalogOut
        {
            // user select is DTS stream, but play stream may not be DTS stream.
            if (__bSelectAudioType == HAL_AUDIO_DTS)
            {
                // for choose back a DTS stream.
                NV_Setting(SET_AST, (BYTE)NULL);
            }
        }
        else
        {
            // non-SPDIF_RAW mode, don't allow DTS stream.
            if ( __bAudioType == HAL_AUDIO_DTS )
            {
                // for choose a non-DTS stream.
                NV_Setting(SET_AST, (BYTE)NULL);
            }
        }
#endif   //#ifdef  SUPPORT_VIRTUAL_DTS

        {
            // TCC275, direct call CHIPS module, will re-set ACFGR and SPDIF.
            _SetAudType (__bAudioType) ;
        }
        break;
    case SETUP_CUSTOM_PARENTAL:
        NV_Setting(SET_PTL_ID_FLD, 0);
        break;
    default:
        break;
    }
#endif //#ifndef IMAGE_FRAME_SETUP //CoCo2.38
}


//  ***************************************************************************
//  Function    :   DVDSETUP_Exit_AdjustSetting(void)
//  Description :   This function will be called the setting is changed.
//  Arguments   :   bID, the ID of the setting to be changed
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void DVDSETUP_Exit_AdjustSetting(void)
{
	if (__bTVSystemChange)
	{
		SETUP_AdjustSetting(SETUP_SCREEN_TV_SYSTEM);
	}

#ifdef IMAGE_FRAME_SETUP
#ifdef SUPPORT_DSPLAY_16_9_AND_AUTO
   SETUP_AdjustSetting(SETUP_DISPLAY_DISPLAY_TYPE);
  #endif
#endif //IMAGE_FRAME_SETUP
        
#ifndef IMAGE_FRAME_SETUP //CoCo2.38
    if (TRUE == _bAudioTypeChange)
        _DVDSETUP_AdjustSetting(SETUP_MAIN_AUDIO);
    if (TRUE == _bParentalControlChange)
        _DVDSETUP_AdjustSetting(SETUP_CUSTOM_PARENTAL);
#endif //#ifndef IMAGE_FRAME_SETUP //CoCo2.38
}


//brian.170a
// __SetupInfo.bAngleMarkTime will be referenced by cc.c and cell.c to set the Angle message
// We must call this function when __SetupInfo.bAngleMarkDisplay is changed
// or the __SetupInfo.bAngleMarkTime will not be consistent with __SetupInfo.bAngleMarkDisplay
//  ***************************************************************************
//  Function    :   _SetAngleMarkTime
//  Description :   Set __SetupInfo.bAngleMarkTime acording to __SetupInfo.bAngleMarkDisplay.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :
//  ***************************************************************************
void    _SetAngleMarkTime(void)
{
#ifndef NO_OSD
#ifndef NO_SETUP
#ifndef IMAGE_FRAME_SETUP //CoCo2.38

    if (__SetupInfo.bAngleMarkDisplay == SETUP_CUSTOM_ANGLE_MARK_ON)
    {
        __SetupInfo.bAngleMarkTime = 0xFF;
    }
    else
    {
        __SetupInfo.bAngleMarkTime = 0;
    }
#endif //#ifndef IMAGE_FRAME_SETUP //CoCo2.38
#endif
#endif
}

void    DVDSETUP_SetDefaultState(void)
{
    if (__SetupInfo.bOSDLanguage != SETUP_DEFAULT_OSD_LANGUAGE)
    {
#ifndef IMAGE_FRAME_SETUP //CoCo2.38
        __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE;
#else
        __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE;
        __bOSDLanguage = __SetupInfo.bOSDLanguage;
       // _bOSDLang = __bOSDLanguage - SETUP_LANGUAGE_OSD_BASE;
#endif
    }
    
    // Brian1.24, set default according to the flag in SETUP_CUSTOM_READ_DEFAULT
    _WriteSetupData(SETUP_DATA_DEFAULT, SETUP_CUSTOM_READ_DEFAULT);
       
    _SetContrastToIC();                   
    
    _SetBrightnessToIC();
   #ifdef SUPPORT_DSPLAY_16_9_AND_AUTO
    	 _SetDisplayToIC();;
  #endif
    //For TFT relative attributes
    Vipor_SetPicture_Brightness(TFT_BRIGHTNESS_VALUE_LIST[__TFTPicAttr.bBrightness-SETUP_DISPLAY_TFT_BRIGHTNESS_BEGIN]);
    Vipor_SetPicture_Contrast(TFT_CONTRAST_VALUE_LIST[__TFTPicAttr.bContrast-SETUP_DISPLAY_TFT_CONTRAST_BEGIN]);
    Vipor_SetPicture_Saturation(TFT_SATURATION_VALUE_LIST[__TFTPicAttr.bSaturation-SETUP_DISPLAY_TFT_SATURATION_BEGIN]);

    __bRepeat = REPEAT_NONE;
    
    _bAudioTypeChange = TRUE;
}

//  ***************************************************************************
//  Function    :   SETUP_ConfigOSDRegion
//  Description :   This function will be called by OSD module when entering Setup UI
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void SETUP_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;
    //OSDBUF_INFO     OSDBuffer_Info;
#ifdef  BORROW_FRAME_BUFFER
    DWORD dwBufferAddr;
#endif //

    //GDI_DeactivateRegion();
    //Alan2.30, fix mantis bug #1886
    GDI_ClearRegion(0);

#ifdef  BORROW_FRAME_BUFFER
    if ((POWERPNMENU_WIDTH == __RegionList[0].wWidth) && (POWERPNMENU_HEIGHT == __RegionList[0].wHeight) &&
        (GDI_OSD_8B_MODE == __RegionList[0].bColorMode)) 
    {
        return;
    }
    else if (__RegionList[0].bColorMode != GDI_OSD_8B_MODE)
    {
        // Use Frame buffer 1 or 2
        dwBufferAddr = DISP_QueryAvailableBuffer();
        if (0 == dwBufferAddr)
        {
            RegionInfo.dwTAddr = DS_OSDFRAME_ST;
        }
        else
        {
            RegionInfo.dwTAddr = dwBufferAddr;
        }
    } 
    else
    {
        // Use OSD buffer and Frame buffer 0
        RegionInfo.dwTAddr = DS_OSDFRAME_ST;   // Use OSD buffer and Frame buffer 0 
    }

#else
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
#endif // #ifdef  BORROW_FRAME_BUFFER

    RegionInfo.wWidth = POWERPNMENU_WIDTH;
    RegionInfo.wHeight = POWERPNMENU_HEIGHT;
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;

#ifdef  BORROW_FRAME_BUFFER
    OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    OSD_SetRegion(0, FALSE, &RegionInfo);
#endif
}

#ifndef SUPPORT_STB
//  ***************************************************************************
//  Function    :   _SetupBrightnessToIC
//  Description :   This function will set brightness value to IC.
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _SetBrightnessToIC(void)
{
#ifndef NO_SETUP
#ifdef IMAGE_FRAME_SETUP
    HAL_AdjustBrightness(__SetupInfo.bBrightness-SETUP_DISPLAY_BRIGHTNESS_BEGIN);
#else
    HAL_AdjustBrightness(__SetupInfo.bBrightness-SETUP_SCREEN_BRIGHTNESS_BASE);
#endif //IMAGE_FRAME_SETUP
#endif // #ifndef NO_SETUP
}

//  ***************************************************************************
//  Function    :   _SetupContrastToIC
//  Description :   This function will set contrast value to IC.
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _SetContrastToIC(void)
{
#ifndef NO_SETUP
#ifdef IMAGE_FRAME_SETUP
    HAL_AdjustContrast(__SetupInfo.bContrast-SETUP_DISPLAY_CONTRAST_BEGIN);
#else
    HAL_AdjustContrast(__SetupInfo.bContrast-SETUP_SCREEN_CONTRAST_BASE);
#endif //IMAGE_FRAME_SETUP
#endif // #ifndef NO_SETUP
}

#endif //#ifndef SUPPORT_STB

#ifdef IMAGE_FRAME_SETUP
void _SetDisplayToIC(void)
{
#ifdef CT950_STYLE
#ifdef SUPPORT_DSPLAY_16_9_AND_AUTO
    if (__ImageFrameSetupInfo.bDisplayType == SETUP_DISPLAY_TYPE_16_9)
  #endif
    {
        __TFTStatusInfo.bTvType=TFT_TVTYPE_STATUS_NORMAL;
        TFT_Adjust_TvType(__TFTStatusInfo.bTvType);
    }
#ifdef SUPPORT_DSPLAY_16_9_AND_AUTO
    else
    {
        __TFTStatusInfo.bTvType=TFT_TVTYPE_STATUS_43LB;
        TFT_Adjust_TvType(__TFTStatusInfo.bTvType);
    }
 #endif
#endif //CT950_STYLE
}
#ifdef SUPPORT_HAL_JPEG_AUTO_RATIO
BYTE Get_Setup_Display_Type_16_9(void)
{
	if (__ImageFrameSetupInfo.bDisplayType == SETUP_DISPLAY_TYPE_16_9)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
#endif
#ifdef SUPPORT_TFT_43_KEY
void Set_Display_Type_Key_Tft(void)
{
   if(__bSetupMode == FALSE)
   {
   	if (__ImageFrameSetupInfo.bDisplayType == SETUP_DISPLAY_TYPE_4_3)
    	{
        __TFTStatusInfo.bTvType=TFT_TVTYPE_STATUS_NORMAL;
	__ImageFrameSetupInfo.bDisplayType =SETUP_DISPLAY_TYPE_16_9;
      TFT_Adjust_TvType(__TFTStatusInfo.bTvType);
	DVDSETUP_SaveDataToSetupInfo(SETUP_DISPLAY_DISPLAY_TYPE);
    }
    else
    {
        __TFTStatusInfo.bTvType=TFT_TVTYPE_STATUS_43LB;
	__ImageFrameSetupInfo.bDisplayType =SETUP_DISPLAY_TYPE_4_3;
       TFT_Adjust_TvType(__TFTStatusInfo.bTvType);
	DVDSETUP_SaveDataToSetupInfo(SETUP_DISPLAY_DISPLAY_TYPE);
    }
   	}

}
#endif

void DVDSETUP_SetDefaultTFTAttribute(void)
{     
    //For TFT Brightness
    if ((__TFTPicAttr.bBrightness < SETUP_DISPLAY_TFT_BRIGHTNESS_BEGIN) || (__TFTPicAttr.bBrightness > SETUP_DISPLAY_TFT_BRIGHTNESS_END))
    {
        __TFTPicAttr.bBrightness = SETUP_DEFAULT_TFT_BRIGHTNESS;
    }

    //For TFT Contrast
    if ((__TFTPicAttr.bContrast < SETUP_DISPLAY_TFT_CONTRAST_BEGIN) || (__TFTPicAttr.bContrast > SETUP_DISPLAY_TFT_CONTRAST_END))
    {
        __TFTPicAttr.bContrast = SETUP_DEFAULT_TFT_CONTRAST;
    }

    //For TFT Brightness
    if ((__TFTPicAttr.bSaturation < SETUP_DISPLAY_TFT_SATURATION_BEGIN) || (__TFTPicAttr.bSaturation > SETUP_DISPLAY_TFT_SATURATION_END))
    {
        __TFTPicAttr.bSaturation = SETUP_DEFAULT_TFT_SATURATION;
    }
}

WORD DVDSETUP_GetVolume(void)
{
    BYTE bAudioHigh;
    BYTE bAudioLow;
    WORD wVolAudio;

    HAL_ReadStorage(SETUP_ADDR_VOLUME, &bAudioHigh, 1);
    HAL_ReadStorage(SETUP_ADDR_VOLUME+1, &bAudioLow, 1);

    wVolAudio =(bAudioHigh << 8) | bAudioLow;

    if (wVolAudio > VOLUME_MAX)
    {
        //wVolAudio = VOLUME_MAX;
		wVolAudio = VOLUME_DEFAULT;
    }
    else if ((wVolAudio % VOLUME_GAP) != 0)
    {
        wVolAudio = VOLUME_DEFAULT;
    }

    return wVolAudio;
}

void DVDSETUP_SaveVolume(void)
{
    BYTE bAudioHigh;
    BYTE bAudioLow;

    bAudioHigh = HIBYTE(LOWORD(__dwVolAudio));
    bAudioLow = LOBYTE(LOWORD(__dwVolAudio));

#ifdef WRITE_EEPROM_ENABLE
    HAL_WriteStorage(SETUP_ADDR_VOLUME, &bAudioHigh, 1);
    HAL_WriteStorage(SETUP_ADDR_VOLUME+1, &bAudioLow, 1);
#endif
}
#endif //IMAGE_FRAME_SETUP
#ifdef SUPPORT_DEFAULT_VOLUME_IN_FLASH
WORD DVDSETUP_DefaultVolume(void)
{
	__dwVolAudio=VOLUME_DEFAULT;
	DVDSETUP_SaveVolume();
}
#endif
