#include "winav.h"

#ifndef IMAGE_FRAME_SETUP //CoCo2.38
#ifndef SUPPORT_STB
#define MAIN_SETUP_DECLARE
#include <string.h>
#include "ctkav.h"

#include    "input.h"
#include    "osd.h"
#include    "hal.h"
#include    "cc.h"
#include    "hal.h"
#include    "chips.h"

#include    "navigate.h"
#include    "infofilter.h"
#include    "utl.h"
#include    "SrcFilter.h"

#include    "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include    "osd3.h"

#include    "comutl.h"
#include    "cell.h"

#define     BACK_DOOR_DECLARE
#include    "setup.h"

#include    "osddsply.h"
#include    "osdnd.h"
#include    "osddlg.h"
#include    "osddivx.h"
#include    "mmanager.h"
#include "romld.h"
#include "media.h"  //xuli2.53

#define  SETUP_MAIN_FILE
//#include    "setupstr.lan"
#include "OSDString\\strextrn.h"

#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
BYTE __bPreCountStop; //Xuli1.26, Save the previous stop mode before entering SETUP to decide if need resume after exit.
#endif


#define IDENTIFY_STRING         "C"
#define FRAME_THICKNESS         2

#define SETUP_WIDTH             616
#define SETUP_HEIGHT            410 

#ifdef SUPPORT_CHAR_SUBPICTURE
#include "char_subpict.h"
#endif

// SETUP_FRAME_Y0 should be lower than the upper osd region, otherwise
// When input disc, it shows "Loading...".
// The strings will cover partial of Setup UI
#define SETUP_FRAME_X0          SETUP_GLOBAL_OFFSET_X
#define SETUP_FRAME_Y0          SETUP_GLOBAL_OFFSET_Y
#define SETUP_FRAME_X1          (SETUP_WIDTH + SETUP_FRAME_X0)
#define SETUP_FRAME_Y1          (SETUP_HEIGHT + SETUP_FRAME_Y0) 

#define CENTER_FRAME_X0          SETUP_FRAME_X0 
#define CENTER_FRAME_Y0          (SETUP_FRAME_Y0 + 41) 
#define CENTER_FRAME_X1          SETUP_FRAME_X1
#define CENTER_FRAME_Y1          (SETUP_FRAME_Y1 - 41) 

#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
#define ICON_FRAME_X0       (22+SETUP_FRAME_X0)
//#define ICON_FRAME_Y0       (CENTER_FRAME_Y0 + 5) 
#define ICON_FRAME_Y0       (CENTER_FRAME_Y0 + 0) 
#define ICON_FRAME_X1       (ICON_FRAME_X0 + 570)
#define ICON_FRAME_Y1       (ICON_FRAME_Y0 + 57) 
#else
#define ICON_FRAME_X0       (5+SETUP_FRAME_X0)
#define ICON_FRAME_Y0       (SETUP_FRAME_Y0 + 67) 
#define ICON_FRAME_X1       (ICON_FRAME_X0 + 76)
#define ICON_FRAME_Y1       (ICON_FRAME_Y0 + 274) 
#endif

#define BACKDOOR_DISPLAY_VERSION_START_V	45 
#define BACKDOOR_DISPLAY_VERSION_START_H        ICON_FRAME_X0 
#define VERSION_OFFSET                          120

#define BACKDOOR_DISPLAY_VERSION_START_H1       (BACKDOOR_DISPLAY_VERSION_START_H + 320) 

// Brian0.87
// We can enter back door when selection bar is on a specified position
// BACKDOOR_ENTRY_POINT defines this position
#define BACKDOOR_ENTRY_POINT    SETUP_MAIN_LANGUAGE

#define WRITE_EEPROM_ENABLE

// brian.277a-3rd, definition for _SetTestTone function
#define MUTE_CHANNEL    0
#define L_CHANNEL       1
#define R_CHANNEL       2
#define C_CHANNEL       3
#define LS_CHANNEL      4
#define RS_CHANNEL      5
#define SW_CHANNEL      6

#define SPEAKER_ALL             0x3F   // 00111111
#define SPEAKER_L               0x01   // 00000001
#define SPEAKER_R               0x02   // 00000010
#define SPEAKER_C               0x04   // 00000100
#define SPEAKER_LS              0x08   // 00001000
#define SPEAKER_RS              0x10   // 00010000
#define SPEAKER_SW              0x20   // 00100000
#define SPEAKER_HIGHLIGHT       0x40   // 01000000
#define SPEAKER_CLEAR           0x80   // 10000000


#define FORMAT_VERSION          0 
#define FORMAT_DATE             1
#define FORMAT_NUMBER           2


#define DEBUG_MODE_0		0x00000000
#define DEBUG_MODE_1		0x00000011
#define DEBUG_MODE_2		0x00000022
#define DEBUG_MODE_3		0x00001100
#define DEBUG_MODE_4		0x00001111
#define DEBUG_MODE_5		0x00001122
#define DEBUG_MODE_6		0x00002200
#define DEBUG_MODE_7		0x00002211
#define DEBUG_MODE_8		0x00002222
#define DEBUG_MODE_9		0x00400222
#define DEBUG_MODE_10		0x00400211
#define DEBUG_MODE_11		0x00400200

BYTE _bCARDPath, _bNIMPath;

#ifndef REMOVE_SETUP_SPEAKER_TEST
BYTE    _bTestToneStage;
DWORD   _dwStageCount;
BYTE    _bDSPTestTone;  // record whether we are using DSP test tone micro code or not
#endif

#define BACKDOOR_DISPLAY_X    (460+SETUP_GLOBAL_OFFSET_X) // Brian1.24
#define BACKDOOR_DISPLAY_Y    (SETUP_FRAME_Y0 + 5) 

#define BACKDOOR_SERVO_X      (300+SETUP_GLOBAL_OFFSET_X) // Brian1.24
#define BACKDOOR_SERVO_Y      36

#include "disp.h"

//***************************************************************************

// save to EEPROM info.
BYTE    __bPassword[SETUP_PASSWORD_NO+1];
BYTE    __bSetupMode; // = FALSE; put in _INITIAL_Variables_PowerON

BYTE	__bLanguageCode[4];

BYTE    _bAudioTypeChange; // = FALSE;
BYTE    _bVideoSettingChange; // = FALSE;
BYTE    _bParentalControlChange; // = FALSE;


// for SETUP_InputPassword
BYTE    __bInputPassword; //=FALSE;
BYTE    __bInputPasswordIndex; // = 0;
BYTE    __bSETUPUIStyleChanged = FALSE;


BYTE *pbSetupPtr;

BYTE i;
BYTE _bSetupTemp;

#ifdef _DEBUG_INFO
BYTE _bEEPROMError = 0;
#endif

// update Setup info to SPRM

extern  WORD aPlayIcon[];
extern  WORD aDirIcon[];
extern  WORD aColon[];
#ifndef NO_DISC_MODE //CoCo2.37p
extern  WORD __wSPRMs [];
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
extern  BYTE __bNowBTNNumber;
extern  BYTE __bInvalidRegion;
extern  BYTE _bShowCEMessage;
extern  BYTE __bServoRevision[];
// wyc2.00-909S
extern  BYTE __dwMMJPEGPreview;
extern DWORD    __dwVersionCard;
extern  void _SetAudType(BYTE bType);  // in CHIPS module

#ifndef SUPPORT_ATAPI_SOURCE
extern BYTE SRVDBG_PCBNumber(void);
extern BYTE SRVDBG_MotorDriver(void);
extern BYTE SRVDBG_RFAmpChip(void);
extern BYTE SRVDBG_MotorPower(void);
#endif


void    _SetAngleMarkTime(void);
void    _WriteSetupData(BYTE bSetup, BYTE bDefaultPassword);
void    _SetTestTone(BYTE bChannel);
void    _SoftVolumeMax(void); // brian.277a-3rd
void OSDSETUP_Display(void);
void OSDSETUP_ProcessSpeaker(void);

//CoCo1.11a, for backdoor
void _OutputAllSWVersion(void);
void _OutputVersion(DWORD dwVersion, BIT btFormat);
void _TranslateString(BYTE* pbAsciiString);
void _ClearVersionRegion(void);
//BYTE _GetDebugIndex(void);
void _OutputDebugInfo(void);
WORD _TranslateNum(BYTE bNum);
WORD* _TranslateDebugMode(BYTE bMode); 
BYTE _GetDebugIndex(DWORD dwMode);
void _SetDebugMode(BYTE bIndex);
void _GetUsefulDebugMode(BYTE bDIR);


DWORD _dwDebugMode;
DWORD _dwBackupDebugMode;
BYTE _bDebugIndex;

BYTE	_bPasswordOK=FALSE;


//extern WORD    aDigit[];
extern BYTE    _bGDIInit;
extern BYTE    __bThumbnailRepaint;
extern BYTE    __bThumbnailMode;
extern BYTE    DATA    _bOSDLang;

//CoCo1.11a, for backdoor
extern BYTE    _bUDEData[6];
extern COPYINFO    cpinfo ; // Brian1.25


//Alan, for drawing
WORD             _wSetupTemp;
WORD             _wSetupString[GDI_STRING_LENGTH];
PARM_FRAME       _SETUP_FrameAttr;
PARM_DRAW_STRING _SETUP_StrAttr;
PARM_RECT        _SETUP_RectAttr;


void _SETUP_ShowUI(void);
void _SETUP_ClearUI(void);
void _SETUP_Exit(void);
void _SETUP_NumberToString(WORD wNum);
void _SETUP_OutputNumber(WORD wX, WORD wY, WORD wNum, BYTE bTextColor, BYTE bBackgroundColor);
void _SETUP_OutputString(WORD wX, WORD wY, BYTE bTextColor, BYTE bBackgroundColor);
void _SETUP_InitPalette(void);
void _SETUP_DrawSelectString(BYTE bOSDLanguage);
void _SETUP_ShowSpeaker(BYTE bLoc);

void _SaveDataToSetupInfo(void);
void _ProcessBackDoorKey(void);
void _SpeakerTest(void);
void _SetDownMix(void);
void _SetAnalogEnable(void);
void _SETUP_FillRect(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom); //CoCo1.20, reduce code size.
void _SETUP_DrawFrame(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom); //CoCo1.20, reduce code size.
void _SETUP_DrawString(void); //CoCo1.20, reduce code size.
void _SETUP_DrawDynamicString(WORD *pwString); //CoCo1.20, reduce code size.

#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
void _SETUP_EnterByPrestop(void);
void _SETUP_ExitByResume(void);
#endif

extern void _InitRect(PURECT pRect, WORD wLeft, WORD wTop, WORD wRight, WORD wBottom); //CoCo1.20, reduce code size.


//Alex1.07a, 20031013 SHOW PSCAN PROMPT
#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
extern BYTE __bOSDPromptPreKey;
extern BYTE __bOSDPromptMode;
extern BYTE OSDSETUP_InputPscan(BYTE bKey);
extern void OSDSETUP_ScreenVideoOutputPscanDlg(void);

extern WORD OSDUTL_TransformChar(char chCode);

#endif //#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)




// Brian, support GDI library
/*--CoCo1.20, reduce code size.
DWORD code _aSpeaker[]=
{
#ifndef REMOVE_SETUP_SPEAKER_TEST

#include "bmp/Speaker.txt"

#else

0x0, 0x00000002, 0x0, 0x0, // dummy, for no speaker case. Set Size  to 0, so no data will be download to DRAM

#endif
};

#define GDI_BMP_SPEAKER    0

#define GDI_MAX_SETUP_BMP_NUM     1


DWORD * _aSetupBMPArray[GDI_MAX_SETUP_BMP_NUM] = {
    _aSpeaker    // 32
};
*/

//CoCo1.20, reduce code size.
#define GDI_BMP_SPEAKER    0
#define GDI_MAX_SETUP_BMP_NUM     1

//CoCo1.20, reduce code size.
#ifndef REMOVE_SETUP_SPEAKER_TEST
DWORD code _aSpeaker[]=
{
#include "bmp/Speaker.txt"
};

#define GDI_BMP_SPEAKER    0

#define GDI_MAX_SETUP_BMP_NUM     1


DWORD * _aSetupBMPArray[GDI_MAX_SETUP_BMP_NUM] = {
    _aSpeaker    // 32
};
#endif //REMOVE_SETUP_SPEAKER_TEST




#if (!defined(REMOVE_SETUP_ICON)) || (!defined(REMOVE_SETUP_SPEAKER_TEST))
DWORD Palette_Setup[]=    
{
#include "bmp/PalSetup.txt"    
};
#else
DWORD Palette_Setup[]=    
{
0x0     // dummy
};
#endif

GDI_PALETTE_INFO    __PaletteInfo_Setup = {95, PAL_ADDR, Palette_Setup};





BYTE SETUP_ProcessKey(void)
{
    extern BYTE __bMediaSelectUI;
    BYTE bKeyStatus;


    // wyc2.17-909S, support KEY_DISPLAY as SETUP key.
#ifdef SUPPORT_DISPLAY_KEY_DO_SETUP
    if ((__btPlaying == FALSE) && (__btPause == FALSE) && (__bModeCmd == KEY_STOP))
    {
        if (__bKey == KEY_DISPLAY)
            __bKey = KEY_SETUP;
    }
#endif //

    if ((__bSetupMode == FALSE) && (__bKey!=KEY_SETUP))
    {
        // If not in SETUP mode now, and Key is not SETUP
        // just return, process as normal keys
        return KEY_BYPASS; //FALSE;
    }

    bKeyStatus = INVALID_KEY;

#ifdef SUPPORT_USB_SOURCE
    if(__bChooseMedia == MEDIA_SELECT_USB)  //xuli2.53
    {
        if ((__bKey == KEY_OPEN_CLOSE) || (__bKey == KEY_FUNCTION))
	    return KEY_NO_KEY;  //xuli2.53
    }
#endif //

    if ((__bKey == KEY_POWER) || (__bKey == KEY_OPEN_CLOSE))
    {
        __bSetupMode = FALSE;
        //Brian1.05
        //_bDoBackDoor = FALSE;
        if (_bDoBackDoor)
            _ExitBackDoor();
        __bInputPassword = FALSE;

#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
        // Brian1.07a, cancel PScan prompt
        __bOSDPromptMode = FALSE;
#endif

        //Brian0.86-2, clear the region and inform OSD
        _SETUP_ClearUI();
        return KEY_BYPASS; //TRUE;
    }

    // Brian2.81, we treate compound KEY_PLAY_PAUSE/ KEY_PAUSE_STEP as KEY_PAUSE in backdoor
    if (_bDoBackDoor)
    {
        if (__bKey == KEY_PLAY_PAUSE || __bKey == KEY_PAUSE_STEP)
            __bKey = KEY_PAUSE;

        _ProcessBackDoorKey();
        bKeyStatus = KEY_NO_KEY;
        return bKeyStatus;
    }
    else
    {
        if (__bKey == KEY_PLAY || __bKey == KEY_PLAY_PAUSE)
            __bKey = KEY_ENTER;
    }


    if (__bInputPassword)
    {
	extern BYTE _bMENUTemp;

	if (__wMENUCurrentItemId == SETUP_CUSTOM_CHANGE_PASSWORD)
	{
            _bMENUTemp = OSDSETUP_ChangePassword(__bKey);
            if (_bMENUTemp == SETUP_INPUT_PASSWORD_VERIFIED)
            {
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
                HAL_WriteStorage(SETUP_ADDR_PASSSTRING, (BYTE *)
                    &(__SetupInfo.wDefaultPassword), EPROM_PASSSTRING_BYTE);
#endif
            }

            bKeyStatus = KEY_NO_KEY;
            return bKeyStatus;

	}
	else if (__wMENUCurrentItemId == SETUP_CUSTOM_PARENTAL)
	{
            _bMENUTemp = SETUP_InputPassword(__bKey);
            if (_bMENUTemp == SETUP_INPUT_PASSWORD_CORRECT)
            {
                __bKey = KEY_RIGHT;
                _bPasswordOK = TRUE;
            }
            else
            {
                __bKey = KEY_NO_KEY;
            }
        }

//Alex1.07a,20031013 SETUP_SHOW_PSCAN_PROMP
#ifdef SUPPORT_PSCAN_PROMPT_SETUP   
        //Alex1.07a 20031009 if current menu is one of pscan, 
        //  choose support pscan or not
        
        else if (__wMENUCurrentItemId == SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN ||
                 __wMENUCurrentItemId == SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN ||
		 __wMENUCurrentItemId == SETUP_SCREEN_VIDEO_OUTPUT_VGA) 
	{   
            _bMENUTemp = OSDSETUP_InputPscan(__bKey);
	}        
#endif //#ifdef SUPPORT_PSCAN_PROMPT_SETUP
	else 
	{
            _bMENUTemp = OSDSETUP_InputLanguageCode(__bKey);
            if (_bMENUTemp == SETUP_INPUT_PASSWORD_CORRECT)
            {
                OSDMENU_MenuOperation(KEY_ENTER);

		switch (__wMENUPrevItemId)
		{
		case SETUP_LANGUAGE_MENU_OTHERS:
                    //Save the menu language code to __SetupInfo
                    __SetupInfo.wMenuLanguageCode = _GetLanguageNumber();
                    //Save the data to EEPROM
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
                    HAL_WriteStorage(SETUP_ADDR_MENU_LANGUAGE_CODE,
                        (BYTE *)&(__SetupInfo.wMenuLanguageCode), 2);
#endif
                    break;
                case SETUP_LANGUAGE_AUDIO_OTHERS:
                    //Save the menu language code to __SetupInfo
                    __SetupInfo.wAudioLanguageCode = _GetLanguageNumber();
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
                    HAL_WriteStorage(SETUP_ADDR_AUDIO_LANGUAGE_CODE,
                        (BYTE *)&(__SetupInfo.wAudioLanguageCode), 2);
#endif
                    break;
                case SETUP_LANGUAGE_SP_OTHERS:
                    //Save the menu language code to __SetupInfo
                    __SetupInfo.wSPSTLanguageCode = _GetLanguageNumber();
#ifdef WRITE_EEPROM_ENABLE//SYSTEM_8051
                    HAL_WriteStorage(SETUP_ADDR_SPST_LANGUAGE_CODE,
                        (BYTE *)&(__SetupInfo.wSPSTLanguageCode), 2);
#endif
                    break;
                }
                // has been called in the previous _SaveDataToSetupInfo()

                _SaveDataToSetupInfo(); // Brian1.07, put it here so when it call _TransferLanguageToSPRM, the __SetupInfo.xxxLanguageCode is correct

            } // end if (_bMENUTemp == SETUP_INPUT_PASSWORD_CORRECT)
            bKeyStatus = KEY_NO_KEY;
	}
    }

    switch (__bKey)
    {
    case KEY_SETUP:
        if (TRUE == __bMediaSelectUI )
        {
            bKeyStatus = KEY_INVALID; 
        }
        else
        {
            if (!__bSetupMode)
            {
#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
                // Brian1.08, when none-setup mode PSCAN prompt dialog is displayed,
                // don't enter Setup mode.
                if (__bOSDPromptMode)
                {
                    bKeyStatus = KEY_BYPASS;
                    return bKeyStatus;
                }
#endif
//LJY0.76, not support setup in digest play mode.
#ifndef NO_DIGEST
                if(__btDIGEST) return KEY_BYPASS;
#endif
                MM_CompleteJPGEffect(TRUE);
                _SETUP_ShowUI();
            }
            else
            {
                SETUP_Exit(OSD_UI_EXIT_ALL);
            }
            bKeyStatus = KEY_NO_KEY; 
        }
        break;
    case KEY_UP:

        OSDMENU_MenuOperation(__bKey);
#ifndef REMOVE_SETUP_SPEAKER_TEST
		OSDSETUP_ProcessSpeaker();
#endif

        bKeyStatus = KEY_NO_KEY;
        break;
    case KEY_DOWN:

		OSDMENU_MenuOperation(__bKey);
#ifndef REMOVE_SETUP_SPEAKER_TEST
		OSDSETUP_ProcessSpeaker();
#endif

        bKeyStatus = KEY_NO_KEY;
        break;

#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
    // Brian1.08a, horizontal layout could use STOP key to go to paranet menu
    case KEY_STOP:
#endif
    case KEY_LEFT:

#ifndef REMOVE_SETUP_SPEAKER_TEST
        if (__wMENUCurrentItemId == SETUP_AUDIO_SPEAKER_TEST)
	{
            // moving from SPEAKER TEST to AUDIO SETUP
            _SETUP_ShowSpeaker(SPEAKER_CLEAR);
	}
#endif

        OSDMENU_MenuOperation(__bKey);

#ifndef REMOVE_SETUP_SPEAKER_TEST
        if (__wMENUCurrentItemId == SETUP_AUDIO_SPEAKER_TEST)
	{
            // moving from Auto/Left/Right/.../Subwoofer to SPEAKER TEST
            // clear speaker highlight by displaying the picture again
            _SETUP_ShowSpeaker(SPEAKER_ALL);

            if (_bDSPTestTone)
            {
            // Mute Test tone
            CHIPS_SoftMute();
//alan090            UTL_DelayTime(COUNT_50_MSEC, FALSE); // Brian1.23 fix "bo" noise when TestTone --> Exit Setup
            _bAudioTypeChange = TRUE;
            _bTestToneStage = 0; // set to 0, will not trigger Auto mode stage
            }

		}
#endif
        bKeyStatus = KEY_NO_KEY;
        break;

    case KEY_RIGHT:
    case KEY_ENTER:
    case KEY_PLAY:          // Brian2.81, CC module doesn't transfer key anymore
    case KEY_PLAY_PAUSE:    // Brian2.81, CC module doesn't handle compound keys

	if (__wMENUCurrentItemId == SETUP_CUSTOM_PARENTAL && !_bPasswordOK)
	{
            OSDSETUP_PasswordDlg();
	}
	else if (__wMENUCurrentItemId == SETUP_CUSTOM_CHANGE_PASSWORD)
	{
            OSDSETUP_ChangePasswordDlg();
	}
#ifndef NO_DRM
	else if (__wMENUCurrentItemId == SETUP_CUSTOM_DIVX_VOD)
	{
            OSDDIVX_DrawDialog(DLG_REG_CODE);
        }
#endif
	else if (__wMENUCurrentItemId == SETUP_LANGUAGE_MENU_OTHERS ||
                __wMENUCurrentItemId == SETUP_LANGUAGE_AUDIO_OTHERS ||
                __wMENUCurrentItemId == SETUP_LANGUAGE_SP_OTHERS)
	{
            OSDSETUP_LanguageCodeDlg();
	}

//Alex 1.08,20031013 if CurrentItemId is YUU_PSCAN, RGB_PSCAN , or VGA , show OSDSETUP_PscanDlg
#ifdef SUPPORT_PSCAN_PROMPT_SETUP
        else if ((__wMENUCurrentItemId == SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN ||
                 __wMENUCurrentItemId == SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN ||
                 __wMENUCurrentItemId == SETUP_SCREEN_VIDEO_OUTPUT_VGA)     && 
                 (__bInputPassword==FALSE  )&&
                 (__SetupInfo.bVideoOutput != SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN &&
                 __SetupInfo.bVideoOutput != SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN &&
                 __SetupInfo.bVideoOutput != SETUP_SCREEN_VIDEO_OUTPUT_VGA)  )
	{
            __bOSDPromptPreKey=KEY_ACTIVE;
            OSDSETUP_ScreenVideoOutputPscanDlg();
	}
#endif// #ifdef SUPPORT_PSCAN_PROMPT_SETUP

	else
	{
            OSDMENU_MenuOperation(__bKey);

#ifndef REMOVE_SETUP_SPEAKER_TEST
	// speacial case: when from Root to Item, and the item is "Speaker Test"
            if (__wMENUCurrentItemId == SETUP_AUDIO_SPEAKER_TEST)
            {
                _SETUP_ShowSpeaker(SPEAKER_ALL);
                if (_bDSPTestTone)
                {
                    // Mute Test tone
                    CHIPS_SoftMute();
                    // fix "bo" noise when TestTone --> Exit Setup
//alan090                    UTL_DelayTime(COUNT_50_MSEC, FALSE); 
                    _bAudioTypeChange = TRUE;
                    // set to 0, will not trigger Auto mode stage
                    _bTestToneStage = 0;
                }
            }
            else
            {
                // check if it is one of the speaker test item,
                // if it is, do the Test tone
                _SpeakerTest();
            }
#endif	//#ifndef REMOVE_SETUP_SPEAKER_TEST

	}

	if (__bKey == KEY_ENTER)
	{
            // save changed value to serial EEPROM
            if (!__bInputPassword) 
            {
                // because in Input or Change Password mode,
                // the current Item is still the Item not on the Value
		// so we don't want and don't need to save the value to EEPROM,
                // but there is no protection
		// for this in _SaveDataToSetupInfo, so we do the check here.
		_SaveDataToSetupInfo();
                _SETUP_DrawSelectString(__SetupInfo.bOSDLanguage - SETUP_LANGUAGE_OSD_BASE);
            }

            if (__wMENUCurrentItemId == SETUP_MAIN_EXIT)
            {
                SETUP_Exit(OSD_UI_EXIT_ALL);
            }
	}

        bKeyStatus = KEY_NO_KEY;
        break;

    case KEY_STOP_RETURN:
        bKeyStatus =KEY_NO_KEY;
        break;

    case KEY_TEST1:
    case KEY_TEST2:
    case KEY_TEST3:
    case KEY_TEST4:
    case KEY_TEST5:
    case KEY_TEST6:
    case KEY_TEST7:
    case KEY_TEST8:
    case KEY_TEST9:
    case KEY_I2CTEST:
        // these KEYs are for Emulator Test Tool
        // we need to pass them out
        break;


    case KEY_N0:
    case KEY_N1:
    case KEY_N2:
    case KEY_N3:
    case KEY_N4:
    case KEY_N5:
    case KEY_N6:
    case KEY_N7:
    case KEY_N8:
    case KEY_N9: 
    default:
        if (__btPause)
        {
            bKeyStatus = KEY_NO_KEY;
            return bKeyStatus ; //TRUE;
        }

#ifndef DO_NOT_SUPPORT_BACK_DOOR
        if ((_bDoBackDoor == FALSE) && (__wMENUCurrentItemId == BACKDOOR_ENTRY_POINT)) //SETUP_MAIN_LANGUAGE)) 
        {
            if  (__bBackDoorSeqIndex == 0)
            {
                if (__bKey == __bBackDoorSequence[__bBackDoorSeqIndex])
                    __bBackDoorSeqIndex++;
            }
            else if (__bBackDoorSeqIndex < BACK_DOOR_SEQUENCE_NUM)
            {
                if (__bKey == __bBackDoorSequence[__bBackDoorSeqIndex])
                    __bBackDoorSeqIndex++;
                else
                    __bBackDoorSeqIndex = 0;

                if (__bBackDoorSeqIndex == BACK_DOOR_SEQUENCE_NUM)
                {
                    _bDoBackDoor = TRUE;

                    // Set text color for back door case
                    _SETUP_StrAttr.bTextColor = OSDMENU_ENTRY_DESC_TEXT_COLOR;
                    _SETUP_StrAttr.bBackground = OSDSETUP_ENTRY_CENTER_COLOR_BG;

                    if (SETUP_BACK_DOOR_REGION == _bBackDoor[_bBackDoorIndex]) 
                    {
                    _bSetupTemp = _ConvertBackDoorRegionNum(__SetupInfo.bRegionControl);
                    _SETUP_OutputNumber(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y,
                                        _bSetupTemp,
                                        OSDSETUP_ENTRY_COLOR_TOP_BANNER_TEXT,
                                        OSDSETUP_ENTRY_COLOR_TOP_BANNER);
                    }
                    else // SETUP_BACK_DOOR_DEBUG_MODE
                    {
                        _bDebugSetMode = FALSE;
                        _dwDebugMode = UTL_Get_DebugMode();	
                        _bDebugIndex = _GetDebugIndex(_dwDebugMode);
                        _dwBackupDebugMode = _dwDebugMode;                
                        _ClearVersionRegion();
                        _OutputDebugInfo();
						
                        _SETUP_OutputNumber(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y,
                            6000 + _bDebugIndex,
                            OSDSETUP_ENTRY_COLOR_TOP_BANNER_TEXT, 
                            OSDSETUP_ENTRY_COLOR_TOP_BANNER);                        						
                    }
                }
            }
        }
#endif //#ifndef DO_NOT_SUPPORT_BACK_DOOR

        bKeyStatus = KEY_NO_KEY;
        break;
    }

    return bKeyStatus;
}


void SETUP_PartialRedraw(DWORD X0, DWORD Y0, DWORD X1, DWORD Y1)
{
    PARM_RECT RectAttr;
    RectAttr.rect.wLeft = X0; 
    RectAttr.rect.wTop = Y0; 
    RectAttr.rect.wRight = X1; 
    RectAttr.rect.wBottom = Y1; 
    RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
    GDI_FillRect_909(0, &RectAttr);
    OSDSETUP_RedrawMenu();
}

//  *************************************************************************
//  Function    :   SETUP_Recover
//  Description :   The function is called by OSD to do the recovery of Program UI
//  Arguments   :   bRecoverRegion, the recover region
//              :   OSD_RECOVER_ALL
//              :   OSD_RECOVER_UPPER_REGION, invalid
//              :   OSD_RECOVER_BOTTOM_REGION, invalid
//  Return      :   TRUE, The region has been recovered.
//              :   FALSE, The region doesn't need to be recovered in this UI.
//  Side Effect :
//  *************************************************************************
BYTE SETUP_Recover(BYTE bRecoverRegion)
{
    // The whole setup need to be redraw when recover from screen saver mode
    // Only partial setup need to be redraw when recover from p-scan
    if (OSD_RECOVER_ALL != bRecoverRegion) return FALSE;

//    _SETUP_ShowUI();

    // Clear region
/* Alan2.20
    _SETUP_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _InitRect(&(_SETUP_RectAttr.rect), 0, 0, 
                      GDI_REGION_WIDTH, GDI_REGION_HEIGHT);
    _SETUP_FillRect(0, 0, GDI_REGION_WIDTH, GDI_REGION_HEIGHT); //CoCo1.20, reduce code size.
*/

    OSDSETUP_Display();

    OSDMENU_SetMenuPalEntry();

    OSDMENU_RedrawAll();

//  The following code can replace the above code to 
//  recover the setup to the original status
/*
    extern BYTE __bMENUSelect[MENU_LEVEL];
    BYTE i, bLevel0, bLevel1, bLevel2;

    if (OSD_RECOVER_ALL != bRecoverRegion) return FALSE;

    bLevel0 = __bMENUSelect[0];
    bLevel1 = __bMENUSelect[1];
    bLevel2 = __bMENUSelect[2];


    _SETUP_ShowUI();

    for (i = 0; i < bLevel0; i++)
    {
        OSDMENU_MenuOperation(KEY_DOWN);
    }
    OSDMENU_MenuOperation(KEY_RIGHT);
    for (i = 0; i < bLevel1; i++)
    {
        OSDMENU_MenuOperation(KEY_DOWN);
    }
    OSDSETUP_RedrawMenu();
*/

    return TRUE;
}



//  ****************************************************************************
//  Function    :   SETUP_Exit 
//  Description :   The OSD kernel will call this function to exit normally.
//              :   This function can NOT send OSD_ChangeUI(xxx, OSD_EXIT_UI) to
//              :   notify OSD kernel it exits.
//  Arguments   :   bExitMode
//                  OSD_UI_EXIT_ALL             : Exit control and clear the UI.
//                  OSD_UI_EXIT_CONTROL_ONLY    : Exit control only.
//  Return      :
//  Side Effect :
//  ****************************************************************************

void SETUP_Exit(BYTE bExitMode)
{
    switch (bExitMode)
    {
    case OSD_UI_EXIT_ALL:
        if (_bDoBackDoor)
            _ExitBackDoor();
        _SETUP_ClearUI();
        _SETUP_Exit();
        break;
    case OSD_UI_EXIT_CONTROL_ONLY:
        _SETUP_Exit();
        break;
    default:
        break;
    }
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
    BYTE bTmp=0xff;

    _bVideoSettingChange = TRUE;
    //int i = 0;
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
    // always clear current EEPROM data
    if (bSetup==SETUP_DATA_RESET)
    {
        for (i=0; i<EPROM_IDENTIFY_BYTE;i++)
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

        __SetupInfo.bAspectRatio=SETUP_DEFAULT_ASPECT;
#ifndef NO_SCREEN_SAVER // Brian1.20
        __SetupInfo.bSCRControl = SETUP_DEFAULT_SCREEN_SAVER;
#endif
        __SetupInfo.bTVSystem = SETUP_DEFAULT_TV_SYSTEM;
        __SetupInfo.bBrightness = SETUP_DEFAULT_BRIGHTNESS;
        __SetupInfo.bContrast = SETUP_DEFAULT_CONTRAST;
#ifdef SUPPORT_ADJUST_CHROM_PHASE // Brian1.25, Chroma phase
        __SetupInfo.bChromPhaseOffset = SETUP_DEFAULT_CHROM_PHASE;
#endif		
        __SetupInfo.bVideoOutput = SETUP_DEFAULT_VIDEO_OUTPUT;
        __SetupInfo.bAudioDigitalOut = SETUP_DEFAULT_AUDIO_DIGITAL_OUT;
	__SetupInfo.bAudioAnalogOut = SETUP_DEFAULT_AUDIO_ANALOG_OUT;
	__SetupInfo.bAudioDownMix = SETUP_DEFAULT_AUDIO_DOWNMIX;
        __SetupInfo.bAudioPrologic = SETUP_DEFAULT_AUDIO_PROLOGIC; // Brian1.08
        __SetupInfo.bAudioCenter = SETUP_DEFAULT_AUDIO_CENTER;
        __SetupInfo.bAudioRear = SETUP_DEFAULT_AUDIO_REAR;
        __SetupInfo.bAudioSubwoofer = SETUP_DEFAULT_AUDIO_SUBWOOFER;
        __SetupInfo.bAudioLPCM = SETUP_DEFAULT_AUDIO_LPCM;
        __SetupInfo.bAudioDNR = SETUP_DEFAULT_AUDIO_DNR;
        __SetupInfo.bAudioKaraoke = SETUP_DEFAULT_AUDIO_KARAOKE;

        // Brian1.24, check if need to set to default
        if (bDefaultPassword & USE_DEFAULT_REGIONCODE) 
            __SetupInfo.bRegionControl = SETUP_DEFAULT_REGION;
        if (bDefaultPassword & USE_DEFAULT_PARENTAL_CTRL) 
            __SetupInfo.bParentalControl = SETUP_DEFAULT_PARENTAL_CTL;
        if (bDefaultPassword & USE_DEFAULT_APS_CTRL) 
            __SetupInfo.bAPSControl = SETUP_DEFAULT_APS_CTL;

        __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE;
        __SetupInfo.bMenuLanguage = SETUP_DEFAULT_MENU_LANGUAGE;
        __SetupInfo.bAudioLanguage = SETUP_DEFAULT_AUDIO_LANGUAGE;
        __SetupInfo.bSPSTLanguage = SETUP_DEFAULT_SPST_LANGUAGE;
        //__SetupInfo.bExSPSTLanguage = SETUP_DEFAULT_EX_SPST_LANGUAGE;
        __SetupInfo.bExSPSTLanguage = SETUP_DEFAULT_ENCODING_LANGUAGE;
        __SetupInfo.bUIStyle = SETUP_DEFAULT_UI_STYLE;
        __SetupInfo.bSmartPlay = SETUP_DEFAULT_SMART_PLAY;
        __SetupInfo.bAngleMarkDisplay = SETUP_DEFAULT_ANGLE_MARK;
        __SetupInfo.bProgressiveScan = SETUP_DEFAULT_PROGRESSIVE_SCAN; // Brian0.81
        __SetupInfo.bAudioDigitalOut = SETUP_DEFAULT_AUDIO_DIGITAL_OUT; // Brian0.82
        __SetupInfo.bAudioAnalogOut = SETUP_DEFAULT_AUDIO_ANALOG_OUT; // Brian0.82

#ifndef NO_DISC_MODE //CoCo2.37p
        // Brian1.05, fix the problem that check region code still error after it is restored as default 
        // just call NV_Setting, it will set __wSPRMs[20] always
        NV_Setting(SET_REGION_CODE, 0);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

        // TCC276, let 0x5A be TRUE.
        if (SETUP_DEFAULT_VCD_CTL == FALSE)
            __SetupInfo.bVCDControl = 0 ;
        else
            __SetupInfo.bVCDControl = 0x5A ;

        _SetAngleMarkTime(); //brian.170a
    }

#ifndef NO_DISC_MODE //CoCo2.37p
    _TransferLanguageToSPRM();
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

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

    pbSetupPtr=(BYTE *)&__SetupInfo;
    HAL_ReadStorage(SETUP_ADDR_AUDIO_LANGUAGE, pbSetupPtr + EPROM_IDENTIFY_BYTE +
                   EPROM_VERSION_BYTE + EPROM_INTERVERSION_BYTE,
                   SETUP_ADDR_USED - SETUP_ADDR_INTERVERSION);

        //error protection
        if ((__SetupInfo.bAspectRatio < SETUP_SCREEN_TV_TYPE_BEGIN) ||
            (__SetupInfo.bAspectRatio > SETUP_SCREEN_TV_TYPE_END))
        {
            __SetupInfo.bAspectRatio = SETUP_DEFAULT_ASPECT;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_ASPECT;
#endif
        }
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

        if ((__SetupInfo.bTVSystem < SETUP_SCREEN_TV_SYSTEM_BEGIN) ||
            (__SetupInfo.bTVSystem > SETUP_SCREEN_TV_SYSTEM_END))
        {
            __SetupInfo.bTVSystem = SETUP_DEFAULT_TV_SYSTEM;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_TV_SYTEM;
#endif
        }

        if ((__SetupInfo.bProgressiveScan < SETUP_PROGRESSIVE_SCAN_BEGIN) ||
            (__SetupInfo.bProgressiveScan > SETUP_PROGRESSIVE_SCAN_END))
        {
            __SetupInfo.bProgressiveScan = SETUP_DEFAULT_PROGRESSIVE_SCAN;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_TV_SYTEM;
#endif
        }

        if ((__SetupInfo.bBrightness < SETUP_SCREEN_BRIGHTNESS_BEGIN) ||
            (__SetupInfo.bBrightness > SETUP_SCREEN_BRIGHTNESS_END))
        {
            __SetupInfo.bBrightness = SETUP_DEFAULT_BRIGHTNESS;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_BRIGHTNESS;
#endif
        }

        if ((__SetupInfo.bContrast < SETUP_SCREEN_CONTRAST_BEGIN) ||
            (__SetupInfo.bContrast > SETUP_SCREEN_CONTRAST_END))
        {
            __SetupInfo.bContrast = SETUP_DEFAULT_CONTRAST;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_CONTRAST;
#endif
        }

#ifdef SUPPORT_ADJUST_CHROM_PHASE // Brian1.25, Chroma phase
        if ((__SetupInfo.bChromPhaseOffset< SETUP_SCREEN_CHROM_PHASE_BEGIN) ||
            (__SetupInfo.bChromPhaseOffset> SETUP_SCREEN_CHROM_PHASE_END))
        {
            __SetupInfo.bChromPhaseOffset= SETUP_DEFAULT_CHROM_PHASE;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_CHROM_PHASE;
#endif
        }
#endif

        if ((__SetupInfo.bVideoOutput < SETUP_SCREEN_VIDEO_OUTPUT_BEGIN) ||
            (__SetupInfo.bVideoOutput > SETUP_SCREEN_VIDEO_OUTPUT_END))
        {
            __SetupInfo.bVideoOutput = SETUP_DEFAULT_VIDEO_OUTPUT;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_VIDEO_OUTPUT;
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


        if ((__SetupInfo.bAudioLPCM < SETUP_AUDIO_LPCM_BEGIN) ||
            (__SetupInfo.bAudioLPCM > SETUP_AUDIO_LPCM_END))
        {
            __SetupInfo.bAudioLPCM = SETUP_DEFAULT_AUDIO_LPCM;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_AUDIO_LPCM;
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

        // Brian1.08
        if ((__SetupInfo.bAudioPrologic < SETUP_AUDIO_PROLOGIC_BEGIN) ||
            (__SetupInfo.bAudioPrologic > SETUP_AUDIO_PROLOGIC_END))
        {
            __SetupInfo.bAudioPrologic = SETUP_DEFAULT_AUDIO_PROLOGIC;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_AUDIO_PROLOGIC;
#endif
        }

        // Brian1.10, check region code range
        if ((__SetupInfo.bRegionControl != REGION_ALL) && (__SetupInfo.bRegionControl != REGION1) &&
            (__SetupInfo.bRegionControl != REGION2) && (__SetupInfo.bRegionControl != REGION3) &&
            (__SetupInfo.bRegionControl != REGION4) && (__SetupInfo.bRegionControl != REGION5) &&
            (__SetupInfo.bRegionControl != REGION6) )
        {
            __SetupInfo.bRegionControl = SETUP_DEFAULT_REGION;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_REGION_CTL;
#endif
        }

        if ((__SetupInfo.bParentalCtl < SETUP_CUSTOM_PC_LEVEL_BEGIN) ||
            (__SetupInfo.bParentalCtl > SETUP_CUSTOM_PC_LEVEL_END))
        {
            __SetupInfo.bParentalCtl = SETUP_DEFAULT_PARENTAL;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_PARENTAL_CTL_LEVEL;
#endif
        }

        if ((__SetupInfo.bOSDLanguage < SETUP_LANGUAGE_OSD_BEGIN) ||
            (__SetupInfo.bOSDLanguage > SETUP_LANGUAGE_OSD_END))
        {
            __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_OSD_LANGUAGE;
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

        if ((__SetupInfo.bAudioDNR < SETUP_AUDIO_DNR_BEGIN) ||
            (__SetupInfo.bAudioDNR > SETUP_AUDIO_DNR_END))
        {
            __SetupInfo.bAudioDNR = SETUP_DEFAULT_AUDIO_DNR;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_AUDIO_DNR;
#endif
        }

        if ((__SetupInfo.bAudioKaraoke < SETUP_AUDIO_KARAOKE_BEGIN) ||
            (__SetupInfo.bAudioKaraoke > SETUP_AUDIO_KARAOKE_END))
        {
            __SetupInfo.bAudioKaraoke = SETUP_DEFAULT_AUDIO_KARAOKE;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_AUDIO_KARAOKE;
#endif
        }

        if ((__SetupInfo.bMenuLanguage < SETUP_LANGUAGE_MENU_BEGIN) ||
            (__SetupInfo.bMenuLanguage > SETUP_LANGUAGE_MENU_END))
        {
            __SetupInfo.bMenuLanguage = SETUP_DEFAULT_MENU_LANGUAGE;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_MENU_LANGUAGE;
#endif
        }

        if ((__SetupInfo.bAudioLanguage < SETUP_LANGUAGE_AUDIO_BEGIN) ||
            (__SetupInfo.bAudioLanguage > SETUP_LANGUAGE_AUDIO_END))
        {
            __SetupInfo.bAudioLanguage = SETUP_DEFAULT_AUDIO_LANGUAGE;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_AUDIO_LANGUAGE;
#endif
        }

        if ((__SetupInfo.bSPSTLanguage < SETUP_LANGUAGE_SP_BEGIN) ||
            (__SetupInfo.bSPSTLanguage > SETUP_LANGUAGE_SP_END))
        {
            __SetupInfo.bSPSTLanguage = SETUP_DEFAULT_SPST_LANGUAGE;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_SPST_LANGUAGE;
#endif
        }

        if ((__SetupInfo.bExSPSTLanguage < SETUP_LANGUAGE_ENCODING_BEGIN) ||
            (__SetupInfo.bExSPSTLanguage > SETUP_LANGUAGE_ENCODING_END-1))
        {
            __SetupInfo.bExSPSTLanguage = SETUP_DEFAULT_ENCODING_LANGUAGE;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_EX_SPST_LANGUAGE;
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

        if ((__SetupInfo.bAngleMarkDisplay < SETUP_CUSTOM_ANGLE_MARK_BEGIN) ||
            (__SetupInfo.bAngleMarkDisplay > SETUP_CUSTOM_ANGLE_MARK_END))
        {
            __SetupInfo.bAngleMarkDisplay = SETUP_DEFAULT_ANGLE_MARK;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_ANGLE_MARK_DISPLAY;
#endif
        }

        if ((__SetupInfo.bUIStyle < SETUP_CUSTOM_UI_STYLE_BEGIN) ||
            (__SetupInfo.bUIStyle > SETUP_CUSTOM_UI_STYLE_END))
        {
            __SetupInfo.bUIStyle = SETUP_DEFAULT_UI_STYLE;

#ifdef _DEBUG_INFO
            _bEEPROMError = SETUP_ADDR_ANGLE_MARK_DISPLAY;
#endif
        }

        // TCC276, add protection for __SetupInfo.bVCDControl
        if (__SetupInfo.bVCDControl != 0 && __SetupInfo.bVCDControl != 0x5A)
        {
                        if (SETUP_DEFAULT_VCD_CTL == FALSE)
                                __SetupInfo.bVCDControl = 0 ;
                        else
                                __SetupInfo.bVCDControl = 0x5A ;
        }

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
#endif

    _SetAngleMarkTime(); //brian.172, should always set angle mark time
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
    __bInputPassword=FALSE;
    __bInputPasswordIndex=0;
    __bSETUPUIStyleChanged = FALSE;

#ifndef WRITE_EEPROM_ENABLE

    _WriteSetupData(SETUP_DATA_DEFAULT, READ_TOTAL_DEFAULT);
    // Module should add their Init function here to set EPROM to default value

#else

    pbSetupPtr=(BYTE *)&__SetupInfo;
#ifdef WRITE_EEPROM_ENABLE //Kevin0.91, add define
    HAL_ReadStorage(SETUP_ADDR_IDENTIFY, pbSetupPtr, EPROM_IDENTIFY_BYTE + 
                   EPROM_VERSION_BYTE + EPROM_INTERVERSION_BYTE);
#endif
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

        _SetVideoOutputToIC();

#ifndef NO_DISC_MODE //CoCo2.37p
        //Brian1.05, just call NV_Setting, it will set __wSPRMs[20] always
        NV_Setting(SET_REGION_CODE, 0); 
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

        //Kevin1.25, move here
        // Brian1.25, set brightness and contrast when initialization
        _SetBrightnessToIC();
        _SetContrastToIC();

        _SetDownMix(); 

#ifndef NO_DISC_MODE //CoCo2.37p
        _TransferLanguageToSPRM();
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

        _SetDNRToIC();

#ifdef  SUPPORT_KARAOKE_SET
        if ((__SetupInfo.bAudioKaraoke - SETUP_AUDIO_KARAOKE_BASE) == 0) //on
            CHIPS_MICControl(CHIPS_ON);
        else if ((__SetupInfo.bAudioKaraoke - SETUP_AUDIO_KARAOKE_BASE) == 1) //off
            CHIPS_MICControl(CHIPS_OFF);
#endif

#ifndef NO_DISC_MODE //CoCo2.37p
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
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

#ifndef REMOVE_SETUP_SPEAKER_TEST
    // brian.277a Test Tone
    __SetupInfo.bAudioTestTone = SETUP_AUDIO_SPEAKER_TEST_AUTO;
#endif

    SETUP_LoadLanguage();
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
    OSD_ReplaceFont();
//#endif

    return TRUE;
}


//  ***************************************************************************
//  Function    :   _GetPasswordNumber
//  Description :   This function will transfer the char password to "number" password.
//  Arguments   :   None.
//  Return      :   The password number.
//  Side Effect :
//  ***************************************************************************
WORD    _GetPasswordNumber(void)
{
    WORD    wPassword = 0;
#ifndef NO_SETUP
    wPassword =  (WORD)__bPassword[0]*(WORD)1000;
    wPassword += (WORD)__bPassword[1]*(WORD)100;
    wPassword += (WORD)__bPassword[2]*(WORD)10;
    wPassword += (WORD)__bPassword[3];
#endif
    return wPassword;
}

//  ***************************************************************************
//  Function    :   _SetMenuEnable
//  Description :   This function will set __bMenuItemEnable array according to
//                  bMode.
//  Arguments   :   VCD mode: bMode == 1, Run-time mode: bMode == 2
//  Return      :   none
//  Side Effect :
//  ***************************************************************************
void    _SetMenuEnable(BYTE bMode)
{
#ifndef NO_SETUP
    switch (bMode)
    {
        case 0:
	case 1:
	case 2:
		// enable all items
		// Note: take care of the size of _bEnableXXX arry, may not be just 1 
		_bEnableLanguage[0] = 0xFF;
		_bEnableScreen[0] = 0xFF;
		_bEnableAudio[0] = 0xFF;
		_bEnableCustom[0] = 0xFF;
		_bEnableMain[0] = 0xFF;
        break;
    }

    switch (bMode)
    {
	case 1:	//VCD mode
	case 2:	//Run-time mode
            OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_LANGUAGE, SETUP_LANGUAGE_MENU, FALSE);
            OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_LANGUAGE, SETUP_LANGUAGE_AUDIO, FALSE);
            OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_LANGUAGE, SETUP_LANGUAGE_SUBTITLE, FALSE);
#ifndef REMOVE_SETUP_SPEAKER_TEST
            OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_AUDIO, SETUP_AUDIO_SPEAKER_TEST, FALSE);
#endif
            // Brian2.79, use __bCountStop to determine the status
            // Allow change TV Type when pre-stop, i.e., __bCountStop == 1
            // Enable TV Type setting when play AVI format files 
            if ((__bCountStop != 1) && (__bAttrPlay != ATTR_AVI))
            {
                //disable "TV TYPE"
                OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_SCREEN, SETUP_SCREEN_TV_TYPE, FALSE);
            }
            break;
    }
	
    switch (bMode)
    {

#ifndef ENABLE_CUSTOM_SETUP
    case 1: //VCD mode
#endif
    case 2: //Run-time mode
        // Brian2.79, use __bCountStop to determine the status
        // Allow change CUSTOM SETUP when pre-stop, i.e., __bCountStop == 1
        if (__bCountStop != 1)
	{
           //disable "CUSTOM SETUP"
           OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_CUSTOM, SETUP_CUSTOM_PARENTAL, FALSE);
           OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_CUSTOM, SETUP_CUSTOM_ANGLE_MARK, FALSE);
           OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_CUSTOM, SETUP_CUSTOM_DEFAULT_VALUE, FALSE);
        }
        else
        {  
            // is in pre-stop and custom setup is enabled,
            // so we also enable Change Password and Angle Mark
           //disable Parental Control
           OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_CUSTOM, SETUP_CUSTOM_PARENTAL, FALSE);
           //disable Read Default Value
           OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_CUSTOM, SETUP_CUSTOM_DEFAULT_VALUE, FALSE);
        }

        //disabe "AUDIO LPCM"
        if (bMode == 2) // VCD mode doesn't disable LPCM
            OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_AUDIO, SETUP_AUDIO_LPCM, FALSE);

        break;
    case 3: //not 5.1 channel
        //disabe "AUDIO CENTER"
		OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_AUDIO, SETUP_AUDIO_CENTER, FALSE);
        //disabe "AUDIO REAR"
		OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_AUDIO, SETUP_AUDIO_REAR, FALSE);
        //disabe "AUDIO SUBWOOFER"
		OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_AUDIO, SETUP_AUDIO_SUBWOOFER, FALSE);

        // Brian1.23, only do that when Prologic, otherwise the item
        // has the same ID as SETUP_MENU_AUDIO_WITH_PROLOGIC will be diabled
        if (__dwSupportFeature & SUPPORT_FEATURE_PROLOGIC)
        {
            // Brian1.08, disable the Pro Logic item directly
            OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_AUDIO_WITH_PROLOGIC,
                                    SETUP_AUDIO_PROLOGIC, FALSE);
        }

        break;
    case 4: //5.1 channel
        //Enable "AUDIO CENTER"
        OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_AUDIO, SETUP_AUDIO_CENTER, TRUE);
        //Enable "AUDIO REAR"
        OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_AUDIO, SETUP_AUDIO_REAR, TRUE);
        //Enable "AUDIO SUBWOOFER"
        OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_AUDIO, SETUP_AUDIO_SUBWOOFER, TRUE);

        // Brian1.23, only do that when Prologic, otherwise the item 
        // has the same ID as SETUP_MENU_AUDIO_WITH_PROLOGIC will be enabled
        if (__dwSupportFeature & SUPPORT_FEATURE_PROLOGIC)
        {
            // Brian1.08, enable the Pro Logic item
            OSDMENU_SET_ITEM_ENABLE(SETUP_MENU_AUDIO_WITH_PROLOGIC,
                                    SETUP_AUDIO_PROLOGIC, TRUE);
        }
        break;
    default:
        break;
    }
#endif // #ifndef NO_SETUP
}


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
    // Brian1.25
    HAL_AdjustBrightness(__SetupInfo.bBrightness-SETUP_SCREEN_BRIGHTNESS_BASE);
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
    // Brian1.25
    HAL_AdjustContrast(__SetupInfo.bContrast-SETUP_SCREEN_CONTRAST_BASE);
#endif // #ifndef NO_SETUP
}

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
    HAL_WriteAM(HAL_AM_DRC_LEVEL, __SetupInfo.bAudioDNR - SETUP_AUDIO_DNR_BASE);
#endif 
}

//  ***************************************************************************
//  Function    :   _SetupTVSystemToIC
//  Description :   This function will set contrast value to IC.
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _SetTVSystemToIC(void)
{
#ifndef NO_SETUP

    HAL_AdjustTVMode(DISP_NEXTVSYNC);

#endif // #ifndef NO_SETUP
}

//  ***************************************************************************
//  Function    :   _SetVideoAspectToIC
//  Description :   This function will set tv aspect type to IC.
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
// LLY.278, remove this function, because there is only one statement for this  function
// So, it's unnecessary any more !!
#ifdef SET_SCART_FROM_SETUP
void _SetVideoAspectToIC(void)
{
#ifndef NO_SETUP
    // LLY.278, merge scart setting while refer setup info. or not into HAL_SetScart()
    // ie, this function will handle the setting value
    //HAL_SetSCART(HAL_SCART_POWER_ON);

    if (__SetupInfo.bAspectRatio==SETUP_SCREEN_TV_TYPE_WIDE)
    {
        HAL_SetSCART(HAL_SCART_16_9);
    }
    else
    {
        HAL_SetSCART(HAL_SCART_POWER_ON);
    }
#endif // #ifndef NO_SETUP
}
#endif //#ifdef SUPPORT_SCART_IF


//  ***************************************************************************
//  Function    :   _SetVideoOutputToIC
//  Description :   This function will set contrast value to IC.
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _SetVideoOutputToIC(void)
{
#ifndef NO_SETUP

#ifdef SUPPORT_SCART_IF
    switch (__SetupInfo.bVideoOutput)
    {
    case SETUP_SCREEN_VIDEO_OUTPUT_RGB:
        HAL_SetSCART(HAL_SCART_RGB);
        break;
    case SETUP_SCREEN_VIDEO_OUTPUT_NORMAL:
        HAL_SetSCART(HAL_SCART_CVBS);
        break;
    default:
        HAL_SetSCART(HAL_SCART_CVBS);
        break;
    }

#endif //#ifdef SUPPORT_SCART_IF
#endif // #ifndef NO_SETUP
}


//  ***************************************************************************
//  Function    :   _TransferLanguageToSPRM
//  Description :   This function will transfer the language value in SetupInfo
//              :   to wSPRMs[ ].
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
#ifndef NO_DISC_MODE //CoCo2.37p
void    _TransferLanguageToSPRM(void)
{
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
#ifndef NO_DISC_MODE //CoCo2.37p
        _TransferOthersMenuLanguageCode(__SetupInfo.wMenuLanguageCode);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
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
#ifndef NO_DISC_MODE //CoCo2.37p
        _TransferOthersAudioLanguageCode(__SetupInfo.wAudioLanguageCode);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
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
#ifndef NO_DISC_MODE //CoCo2.37p
        _TransferOthersSPSTLanguageCode(__SetupInfo.wSPSTLanguageCode);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        break;
    default:
        break;
    }
}
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

//  ***************************************************************************
//  Function    :   _TransferOthersMenuLanguageCode
//  Description :   This function will transfer the menu language value that user
//              :   inputs to the standard language code.
//  Arguments   :   wLanguageCode, the menu language code.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
#ifndef NO_DISC_MODE //CoCo2.37p
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
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

//  ***************************************************************************
//  Function    :   _TransferOthersAudioLanguageCode
//  Description :   This function will transfer the menu language value that user
//              :   inputs to the standard language code.
//  Arguments   :   wLanguageCode, the audio language code.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
#ifndef NO_DISC_MODE //CoCo2.37p
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
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

//  ***************************************************************************
//  Function    :   _TransferOthersSPSTLanguageCode
//  Description :   This function will transfer the menu language value that user
//              :   inputs to the standard language code.
//  Arguments   :   wLanguageCode, the SPST language code.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
#ifndef NO_DISC_MODE //CoCo2.37p
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

#endif //#ifndef NO_DISC_MODE //CoCo2.37p

//  ***************************************************************************
//  Function    :   _GetLanguageNumber
//  Description :   This function will transfer the char password to "number" password.
//  Arguments   :   bID, the ID.
//  Return      :   The password number.
//  Side Effect :
//  ***************************************************************************
WORD    _GetLanguageNumber(void) //(BYTE bID)
{
    WORD    wLanguageNum = 0;

#ifndef NO_SETUP
    wLanguageNum =  (WORD)__bLanguageCode[0]*(WORD)10;
    wLanguageNum += (WORD)__bLanguageCode[1];
    wLanguageNum <<= 8;
    wLanguageNum += (WORD)__bLanguageCode[2]*(WORD)10;
    wLanguageNum += (WORD)__bLanguageCode[3];
#endif
    return wLanguageNum;
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
#endif
}



//  ***************************************************************************
//  Function    :   SETUP_AdjustSetting
//  Description :   This function will be called when the setting is changed.
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
    case SETUP_SCREEN_VIDEO_OUTPUT:
        HAL_AdjustTVMode(DISP_NEXTVSYNC);
        _SetVideoOutputToIC();
        break;
        case SETUP_AUDIO_OUTPUT:
#if 0
        //CoCo.172q, fix the problem about MP3/LPCM/CD-DA audio is not continuous.
        //if (__bAudioType == HAL_AUDIO_AC3)
        if (__bAudioType != HAL_AUDIO_PCM && __bAudioType != HAL_AUDIO_LPCM &&
            __bAudioType != HAL_AUDIO_MPG && __bAudioType != HAL_AUDIO_MP3)
            HAL_SetAudioType(__bAudioType);
        CHIPS_ControlSPDIFOutput();
#else
#ifndef NO_DISC_MODE //CoCo2.37p
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
#endif //#ifndef NO_DISC_MODE //CoCo2.37p    
        {
            // TCC275, direct call CHIPS module, will re-set ACFGR and SPDIF.
            if(!((__wDiscType & CDROM_M1) && (MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem)==EXTNAME_JPG))) //xuli2.53
            _SetAudType (__bAudioType);
        }
        

#endif
        break;
    case SETUP_AUDIO_KARAOKE:

        break;
    default:
        break;
    }
#endif
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
    if (SETUP_WIDTH <= __RegionList[0].wWidth  && SETUP_HEIGHT <= __RegionList[0].wHeight &&
        GDI_OSD_8B_MODE == __RegionList[0].bColorMode)   // Thumbnail mode & preview mode
    {
        // Use OSD buffer and Frame buffer 0
        RegionInfo.dwTAddr = DS_OSDFRAME_ST;   // Use OSD buffer and Frame buffer 0 
    }
    else
    {
        // Use Frame buffer 1 or 2
        dwBufferAddr = DISP_QueryAvailableBuffer();
        if (0 == dwBufferAddr)
        {
            RegionInfo.dwTAddr = DS_OSDFRAME_ST;
            printf("DISP can't find an available buffer for OSD region\n");
        }
        else
        {
            RegionInfo.dwTAddr = dwBufferAddr;
        }
    }
#else
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
#endif // #ifdef  BORROW_FRAME_BUFFER

    RegionInfo.wWidth = SETUP_FRAME_X1;
    RegionInfo.wHeight = SETUP_FRAME_Y1;
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;

#ifdef  BORROW_FRAME_BUFFER
    OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    OSD_SetRegion(0, FALSE, &RegionInfo);
#endif

    /*
    GDI_ConfigRegionInfo(0, &RegionInfo);
    GDI_InitialRegion(0);

    // Clear region
    GDI_ClearRegion(0);

    GDI_ActivateRegion(0);
    */
}


#ifndef REMOVE_SETUP_SPEAKER_TEST
//  ***************************************************************************
//  Function    :   _SetTestTone
//  Description :   This function will set test tone according to bChannel
//  Arguments   :   bChannel
//  Return      :   None.
//  Side Effect :   PCM_SCALE will be set to MAX; the setting of DSP will be changed.
//  ***************************************************************************
//kevin1.05, integrate w/ power up music
void _SetTestTone(BYTE bChannel)
{
    // Brian1.22, wait until DPS fade in/out complete
    CHIPS_SoftMute();
//alan090    UTL_DelayTime(COUNT_50_MSEC, FALSE);

    _bDSPTestTone = 1;
    _bAudioTypeChange = TRUE;

    // audio_cfg1 bit0~2
    // 0:L, 1:SW, 2:R, 3:LS, 4:RS, 5:C

    switch (bChannel)
    {
    case MUTE_CHANNEL: // mute
        HAL_WriteAM(HAL_AM_PCM_SCALE, VOLUME_MIN);
        return;        //break;        //alan090
    case L_CHANNEL:
        //HAL_WriteAM(HAL_AM_CHANNEL_MODE, 0); 
        HAL_PlayTone(HAL_TONE_CH_L); 
        break;
    case R_CHANNEL:
        HAL_PlayTone(HAL_TONE_CH_R);   
        break;
     case LS_CHANNEL:
        HAL_PlayTone(HAL_TONE_CH_LS);   
        break;
    case RS_CHANNEL:
        HAL_PlayTone(HAL_TONE_CH_RS);   
        break;
    case C_CHANNEL:
        HAL_PlayTone(HAL_TONE_CH_C);   
        break;
    case SW_CHANNEL:
        HAL_PlayTone(HAL_TONE_CH_LFE);   
        break;
    }
}

#endif // #ifndef REMOVE_SETUP_SPEAKER_TEST

//  ***************************************************************************
//  Function    :   _SoftVolumeMax
//  Description :   This function tune the volume from 0 to max smoothly
//  Arguments   :   none
//  Return      :   None.
//  Side Effect :   none
//  ***************************************************************************
void _SoftVolumeMax(void)
{
    //Kevin1.10, DSP refers to DSP internal DM[pcmscale_fade_inout] (current pcm scale) and PCMSCALE register (target ocm scale) to do fade in/out 
    //Adv: DSP is able to do fade in when sound starts; F/W doesn't have to take ~7 msec to do fade in/out bt SAVCR macro command
    //Kevin1.10, write PCMSCALE register instead of macro command (RISC IRAM code jumper table is contaminated by somebody unknown) 
    HAL_WriteAM(HAL_AM_PCM_SCALE, VOLUME_MAX); 
}


//  ***************************************************************************
//  Function    :   SETUP_SetDefaultValue
//  Description :   This function will set the settings to default value.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :
//  ***************************************************************************
//void    SETUP_SetDefaultValue(void)
void    SETUP_SetDefaultValue(BYTE bType) // Brian1.24
{
#ifndef NO_OSD
#ifndef NO_SETUP

    // Brian1.10, fix bug, general backdoor need to change ODS language variables
    __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE;
    __bOSDLanguage = __SetupInfo.bOSDLanguage;
    _bOSDLang = __bOSDLanguage - SETUP_LANGUAGE_OSD_BASE;

    _WriteSetupData(SETUP_DATA_DEFAULT, bType); // Brian1.24
    // Brian1.24, always adjust setting when Set default
    SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);
    SETUP_AdjustSetting(SETUP_AUDIO_OUTPUT); 
    SETUP_AdjustSetting(SETUP_AUDIO_KARAOKE);

#endif
#endif
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

    if (__SetupInfo.bAngleMarkDisplay == SETUP_CUSTOM_ANGLE_MARK_ON)
    {
        __SetupInfo.bAngleMarkTime = 0xFF;
    }
    else
    {
        __SetupInfo.bAngleMarkTime = 0;
    }

#endif
#endif
}

//  ***************************************************************************
//  Function    :   _SaveDataToSetupInfo
//  Description :   This function will save the value to __SetupInfo. It will
//                  refer bLeftOrRight to decide which value should be assign to
//                  __SetupInfo.
//  Arguments   :   bID ==> the current ID.
//                  bLeftOrRight, bLeftOrRight == 0 ==>shift left.
//                                bLeftOrRight == 1 ==>shift right.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void _SaveDataToSetupInfo(void) //(BYTE bID, BYTE bLeftOrRight)
{
#ifndef NO_SETUP

// Brian1.08a
#ifdef SUPPORT_MENU_CURSOR_STAY_AT_VALUE_AFTER_ENTER
    switch (__wMENUItemIdAfterEnter)
#else
    switch (__wMENUCurrentItemId)
#endif
    {


    case SETUP_LANGUAGE_OSD:

        if (__SetupInfo.bOSDLanguage != __bOSDLanguage)
        {
            __bOSDLanguage = __SetupInfo.bOSDLanguage;
			_bOSDLang = __bOSDLanguage - SETUP_LANGUAGE_OSD_BASE;

            SETUP_LoadLanguage();
            // re-draw menu
			OSDSETUP_RedrawMenu();			
        }
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_OSD_LANGUAGE, (BYTE *)&(__SetupInfo.bOSDLanguage), 1);
#endif
        break;

    case SETUP_LANGUAGE_MENU:
        //__SetupInfo.bMenuLanguage = __bMenuLanguage;
#ifndef NO_DISC_MODE //CoCo2.37p
        _TransferLanguageToSPRM();

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_MENU_LANGUAGE, (BYTE *)&(__SetupInfo.bMenuLanguage), 1);
#endif
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        break;
    case SETUP_LANGUAGE_AUDIO:
        //__SetupInfo.bAudioLanguage = __bAudioLanguage;
#ifndef NO_DISC_MODE //CoCo2.37p
        _TransferLanguageToSPRM();

        // tell NV to re-get audio stream number
        __wSPRMs[1]=0xf;
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUDIO_LANGUAGE, (BYTE *)&(__SetupInfo.bAudioLanguage), 1);
#endif
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        break;
    case SETUP_LANGUAGE_SUBTITLE:
        //__SetupInfo.bSPSTLanguage = __bSPSTLanguage;
#ifndef NO_DISC_MODE //CoCo2.37p
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
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        break;
/*
#ifdef SUPPORT_CHAR_SUBPICTURE
    case SETUP_LANGUAGE_EX_SUBTITLE:
       CHAR_SP_ReplaceFont();

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_EX_SPST_LANGUAGE, (BYTE *)&(__SetupInfo.bExSPSTLanguage), 1);
#endif
        break;
#endif
*/
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
    case SETUP_LANGUAGE_ENCODING:
       OSD_ReplaceFont();

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_EX_SPST_LANGUAGE, (BYTE *)&(__SetupInfo.bExSPSTLanguage), 1);
#endif
        break;
//#endif

    case SETUP_SCREEN_TV_TYPE:
        //__SetupInfo.bAspectRatio = __bScreenTVType;
        // LLY.278, set SCART relative info. base on setup setting while enable the setting
        // Otherwise, it will be done while bitstream info. is decoded ok !!
#ifdef SET_SCART_FROM_SETUP
        // LLY.278, merge scart setting will refer setup info. or not into HAL_SetScart()
        // ie, this function will handle the setting value
        //HAL_SetSCART(HAL_SCART_POWER_ON);
        // Remove this function by call HAL_SetSCART directly !!
        _SetVideoAspectToIC();// TCC276
#endif  // #ifdef SET_SCART_FROM_SETUP

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_ASPECT, (BYTE *)&(__SetupInfo.bAspectRatio), 1);
#endif
        _bVideoSettingChange = TRUE;
        break;

#ifndef NO_SCREEN_SAVER // Brian1.20
    case SETUP_SCREEN_SAVER:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_SCREEN_SAVER, (BYTE*)&(__SetupInfo.bSCRControl), 1);
#endif
        _bVideoSettingChange = TRUE;
        break;
#endif

    case SETUP_SCREEN_TV_SYSTEM:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_TV_SYTEM, (BYTE *)&(__SetupInfo.bTVSystem), 1);
#endif
        _bVideoSettingChange = TRUE;
	__bLOGO |= LOGO_CHANGE_TV_TYPE; //xuli.0110

        break;

#ifndef SUPPORT_TFT
    case SETUP_SCREEN_BRIGHTNESS:
        _SetBrightnessToIC();
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_BRIGHTNESS, (BYTE *)&(__SetupInfo.bBrightness), 1);
#endif
        _bVideoSettingChange = TRUE;
        break;

    case SETUP_SCREEN_CONTRAST:
        _SetContrastToIC();
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_CONTRAST, (BYTE *)&(__SetupInfo.bContrast), 1);
#endif
        _bVideoSettingChange = TRUE;
        break;

    case SETUP_SCREEN_VIDEO_OUTPUT:
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_VIDEO_OUTPUT, (BYTE *)&(__SetupInfo.bVideoOutput), 1);
#endif
        // Brian1.06, P-SCAN is integrated into Video Output,
        // SETUP_ChangeValue will set __SetupInfo.bProgressiveScan accroding to __SetupInfo.bVideoOutput
        SETUP_ChangeValue(SETUP_SCREEN_PROGRESSIVE_SCAN);
        _bVideoSettingChange = TRUE;
        break;

#endif

#ifdef SUPPORT_ADJUST_CHROM_PHASE // Brian1.25, Chroma phase
    case SETUP_SCREEN_CHROM_PHASE:
        HAL_AdjustTVMode(DISP_NEXTVSYNC); 
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_CHROM_PHASE, (BYTE *)&(__SetupInfo.bChromPhaseOffset), 1);
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
        _SetAnalogEnable();

        // re-draw menu
        OSDSETUP_RedrawMenu();
        break;


    case SETUP_AUDIO_DOWNMIX: // Alan, CT909
#ifdef WRITE_EEPROM_ENABLE
        HAL_WriteStorage(SETUP_ADDR_AUDIO_DOWNMIX, (BYTE *)&(__SetupInfo.bAudioDownMix), 1);
#endif
        _SetDownMix();

        // re-draw menu
        OSDSETUP_RedrawMenu();
        break;



    case SETUP_AUDIO_PROLOGIC: // Brian1.08
        _bAudioTypeChange = TRUE;

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUDIO_PROLOGIC, (BYTE *)&(__SetupInfo.bAudioPrologic), 1);
#endif
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

    case SETUP_AUDIO_LPCM:
        _bAudioTypeChange = TRUE;

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUDIO_LPCM, (BYTE *)&(__SetupInfo.bAudioLPCM), 1);
#endif
        break;
    case SETUP_AUDIO_DNR:
        _bAudioTypeChange = TRUE;

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

#ifndef REMOVE_UI_SETTING
    case SETUP_CUSTOM_UI_STYLE:
        SETUP_Recover(OSD_RECOVER_ALL);
        __bSETUPUIStyleChanged = TRUE;
#ifdef WRITE_EEPROM_ENABLE
        HAL_WriteStorage(SETUP_ADDR_UI_STYLE, (BYTE *)&(__SetupInfo.bUIStyle), 1);
#endif
        break;
#endif

#if defined(SUPPORT_DVD_SMRTPB) && defined(SUPPORT_DVD_SMRTPB_SETUP)
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

    case SETUP_CUSTOM_DEFAULT_VALUE:
        // Brian1.06
        if (__SetupInfo.bOSDLanguage != SETUP_DEFAULT_OSD_LANGUAGE)
        {
            __SetupInfo.bOSDLanguage = SETUP_DEFAULT_OSD_LANGUAGE;
            __bOSDLanguage = __SetupInfo.bOSDLanguage;
			_bOSDLang = __bOSDLanguage - SETUP_LANGUAGE_OSD_BASE;
            SETUP_LoadLanguage();
            // re-draw menu
			OSDSETUP_RedrawMenu();			
        }

        // Brian1.24, set default according to the flag in SETUP_CUSTOM_READ_DEFAULT
        _WriteSetupData(SETUP_DATA_DEFAULT, SETUP_CUSTOM_READ_DEFAULT);

        _SetContrastToIC();                   

        _SetBrightnessToIC();

        //Brian1.06
        _SetAnalogEnable();
        // Brian1.11b
        _bAudioTypeChange = TRUE;

#ifdef SETTING_CHANGE_IMMEDIATELY
    if (TRUE == _bVideoSettingChange)
        SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);

    // TCC276
    SETUP_AdjustSetting(SETUP_SCREEN_TV_TYPE);
        
    // Brian2.78b-2, need do this check anyway
    if (_bAudioTypeChange)
    {
        SETUP_AdjustSetting(SETUP_AUDIO_OUTPUT);
    }
#endif // #ifndef SETTING_CHANGE_IMMEDIATELY

        break;

    case SETUP_CUSTOM_PARENTAL:
#ifndef NO_DISC_MODE //CoCo2.37p
/*Alan1.05
        if (__SetupInfo.bParentalControl)
            __wSPRMs[13] = __SetupInfo.bParentalCtl - (WORD)SETUP_CUSTOM_PC_LEVEL_BASE + 1;
        else
            __wSPRMs[13] = SETUP_CUSTOM_PC_LEVEL_OFF - SETUP_CUSTOM_PC_LEVEL_1 + 1; //__wSPRMs[13] = 8;
*/

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
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        break;

    case SETUP_BACK_DOOR_REGION:
        __SetupInfo.bRegionControl = _bRegionCode[_bBackDoorRegion];

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_REGION_CTL, (BYTE*)&(__SetupInfo.bRegionControl), 1);
#endif
        break;

    default:
        break;
    }

#endif
}

//  ***************************************************************************
//  Function    :   SETUP_Trigger
//  Description :   Blinking the current selection bar
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :
//  ***************************************************************************

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

    }


#endif
#endif
}


void OSDSETUP_Display(void)
{
    GDI_ClearRegion(0);

    SETUP_GDIInit();

    if (SETUP_CUSTOM_UI_STYLE_1 == __SetupInfo.bUIStyle)
    {
        _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_SETUP_RectAttr.rect), SETUP_FRAME_X0, SETUP_FRAME_Y0, //CoCo1.20, reduce code size.
                        SETUP_FRAME_X1, SETUP_FRAME_Y1);
        GDI_FillRoundRect(0, 21, &_SETUP_RectAttr);
    }
    else if (SETUP_CUSTOM_UI_STYLE_2 == __SetupInfo.bUIStyle)
    {
        _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_SETUP_RectAttr.rect), //CoCo1.20, reduce code size.
                        SETUP_FRAME_X0, SETUP_FRAME_Y0,
                        SETUP_FRAME_X1 - 50, CENTER_FRAME_Y0-1);
        GDI_FillRoundRectTop(0, 25, &_SETUP_RectAttr);
        _SETUP_FillRect(SETUP_FRAME_X1 - 100, SETUP_FRAME_Y0,
                        SETUP_FRAME_X1, CENTER_FRAME_Y0-1); //CoCo1.20, reduce code size.

        _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER;
        _SETUP_FillRect(SETUP_FRAME_X0, CENTER_FRAME_Y1 + 1,
                        SETUP_FRAME_X1 - 50, SETUP_FRAME_Y1); //CoCo1.20, reduce code size.

        _InitRect(&(_SETUP_RectAttr.rect), //CoCo1.20, reduce code size.
                        SETUP_FRAME_X1 - 100, CENTER_FRAME_Y1 + 1,
                        SETUP_FRAME_X1, SETUP_FRAME_Y1);
        GDI_FillRoundRectBottom(0, 25, &_SETUP_RectAttr);
    }
    else if (SETUP_CUSTOM_UI_STYLE_3 == __SetupInfo.bUIStyle)
    {
        _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_SETUP_RectAttr.rect), //CoCo1.20, reduce code size.
                        SETUP_FRAME_X0, SETUP_FRAME_Y0,
                        SETUP_FRAME_X1, CENTER_FRAME_Y0-1);
        GDI_FillRoundRectTop(0, 21, &_SETUP_RectAttr);
        _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER;
        _SETUP_FillRect(SETUP_FRAME_X0, CENTER_FRAME_Y0 + 1,
                        SETUP_FRAME_X1, SETUP_FRAME_Y1);
    }
    else if (SETUP_CUSTOM_UI_STYLE_4 == __SetupInfo.bUIStyle)
    {
        _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_SETUP_RectAttr.rect), //CoCo1.20, reduce code size.
                        SETUP_FRAME_X0, SETUP_FRAME_Y0,
                        SETUP_FRAME_X1, CENTER_FRAME_Y0-1);
        GDI_FillRoundRectTop(0, 21, &_SETUP_RectAttr);
        _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER;
        _InitRect(&(_SETUP_RectAttr.rect), //CoCo1.20, reduce code size.
                        SETUP_FRAME_X0, CENTER_FRAME_Y0 + 1,
                        SETUP_FRAME_X1, SETUP_FRAME_Y1);
        GDI_FillRoundRectBottom(0, 21, &_SETUP_RectAttr);
    }

    _SETUP_FrameAttr.bStyle = FRAME_POP_SOLID;
    _SETUP_FrameAttr.bThickness = FRAME_THICKNESS;
    _SETUP_FrameAttr.bColorBright = OSDSETUP_ENTRY_FRAME_BRIGHT;
    _SETUP_FrameAttr.bColorDark   = OSDSETUP_ENTRY_FRAME_DARK;
    _SETUP_FrameAttr.bColorCenter = OSDSETUP_ENTRY_FRAME_CENTER;

    _SETUP_DrawFrame(SETUP_FRAME_X0,
           SETUP_FRAME_Y0 + 37, SETUP_FRAME_X1, SETUP_FRAME_Y0 + 40); //CoCo1.20, reduce code size.

    _SETUP_DrawFrame( SETUP_FRAME_X0,
           SETUP_FRAME_Y1 - 40, SETUP_FRAME_X1, SETUP_FRAME_Y1 - 37);  //CoCo1.20, reduce code size.

    // Draw the Setup main background
    _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
    _SETUP_FillRect(CENTER_FRAME_X0, CENTER_FRAME_Y0, //CoCo1.20, reduce code size.
                    CENTER_FRAME_X1, CENTER_FRAME_Y1);

    // Draw the string in bottom banner
    _SETUP_StrAttr.bBackground = OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER;
    _SETUP_StrAttr.bTextColor = OSDSETUP_ENTRY_COLOR_WHITE;
    _SETUP_StrAttr.wX = SETUP_FRAME_X0+25;
    _SETUP_StrAttr.wY = SETUP_FRAME_Y1-30;
    _SETUP_DrawDynamicString(aDirIcon); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += 175;
    _SETUP_DrawDynamicString(aPlayIcon); //CoCo1.20, reduce code size.

    _SETUP_StrAttr.wX += 40;
    _SETUP_DrawDynamicString(aColon); //CoCo1.20, reduce code size.

    _SETUP_DrawSelectString(_bOSDLang);

    // Draw the Setup icon list background
    _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_ICON_RECT;
    _InitRect(&(_SETUP_RectAttr.rect), ICON_FRAME_X0, ICON_FRAME_Y0,
                                       ICON_FRAME_X1, ICON_FRAME_Y1);
    _SETUP_FillRect(ICON_FRAME_X0, ICON_FRAME_Y0, ICON_FRAME_X1, ICON_FRAME_Y1);

/*
    // Brian1.24a, bmp/font icon use the same layout
    // Draw the Setup icon list background
//    _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_TOP_BANNER;
    _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_ICON_RECT;
    _SETUP_FillRect(ICON_FRAME_X0, ICON_FRAME_Y0, ICON_FRAME_X1, ICON_FRAME_Y1); //CoCo1.20, reduce code size.
*/

#ifndef REMOVE_SETUP_HELP_MESSAGE
    // Draw the Setup help text background
    _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_HELP_COLOR_BG;
    _SETUP_FillRect((24+SETUP_GLOBAL_OFFSET_X), 364,
                    (591+SETUP_GLOBAL_OFFSET_X), 396); //CoCo1.20, reduce code size.
#endif



#ifndef REMOVE_SETUP_HELP_MESSAGE
    OSDSETUP_ShowHelpText(__wMENUCurrentItemId);
#endif

}

#ifndef REMOVE_SETUP_SPEAKER_TEST
// This function is supposed to use only in KEY_UP/DOWN operation.
void OSDSETUP_ProcessSpeaker(void)
{
    if (__wMENUCurrentItemId == SETUP_AUDIO_SPEAKER_TEST &&
        __wMENUPrevItemId != SETUP_AUDIO_SPEAKER_TEST)
    {
        _SETUP_ShowSpeaker(SPEAKER_ALL);
    }
    else if (__wMENUPrevItemId == SETUP_AUDIO_SPEAKER_TEST 
             && __bMENUCurrentLevel == 1) // only when it is in AUDIO SETUP MENU
    {
        _SETUP_ShowSpeaker(SPEAKER_CLEAR);
    }
    else
    {
        // Clear speaker highlight just by showing the whole BMP 
        switch (__wMENUPrevItemId)
        {
        case SETUP_AUDIO_SPEAKER_TEST_AUTO:
        case SETUP_AUDIO_SPEAKER_TEST_LEFT:
        case SETUP_AUDIO_SPEAKER_TEST_RIGHT:
        case SETUP_AUDIO_SPEAKER_TEST_LS:
        case SETUP_AUDIO_SPEAKER_TEST_RS:
        case SETUP_AUDIO_SPEAKER_TEST_CENTER:
        case SETUP_AUDIO_SPEAKER_TEST_SW:
            _SETUP_ShowSpeaker(SPEAKER_ALL);
            break;
	}
        // check if it is one of the speaker test item,
        // if it is, do the Test tone
        _SpeakerTest();
    }
}

#endif // #ifndef REMOVE_SETUP_SPEAKER_TEST

BYTE    _ConvertBackDoorRegionNum(BYTE bOriginalNum)
{
#ifndef NO_SETUP
    switch (bOriginalNum)
    {
    case REGION_ALL:
        return 255;
    case REGION1:
        return 1;
    case REGION2:
        return 2;
    case REGION3:
        return 3;
    case REGION4:
        return 4;
    case REGION5:
        return 5;
    case REGION6:
        return 6;
    case REGION7:
        return 7;
    case REGION8:
        return 8;
    default:
        return 255;
    }
#else
    return 255;
#endif
}

//  ***************************************************************************
//  Function    :   _ExitBackDoor
//  Description :   Clean the flags and OSD
//  Arguments   :   none.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _ExitBackDoor(void)
{
#ifndef NO_OSD
#ifndef NO_SETUP

    if (_bDoBackDoor)
    {
        _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_TOP_BANNER;
        // Need to clear the version-display region
        _SETUP_FillRect(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y,
                        BACKDOOR_DISPLAY_X+120, BACKDOOR_DISPLAY_Y + 26); //CoCo1.20, reduce code size.

        OSDMENU_Initial();
        OSDMENU_SetMenuPalEntry();
	  _ClearVersionRegion();
        OSDMENU_DisplayTheMenu();
    }
    //For back door
    __bBackDoorSeqIndex = 0;
    _bDoBackDoor = FALSE;
    _bBackDoorIndex = 0;
    //_bDebugSetMode = FALSE;

    // Back door will change __wMENUCurrentItemId to the corresponding back door ID 
    // We need to set current ID back to the original vaule when we leave it
    __wMENUCurrentItemId = BACKDOOR_ENTRY_POINT; //SETUP_MAIN_LANGUAGE;

    //_IdentifyRegionCode();  // brian.161 check region code (show or clear the error message)

#endif
#endif
}

void _ProcessBackDoorKey(void)
{
    switch (__bKey)
    {
    case KEY_SETUP:
        _ExitBackDoor();
        return;
        break;
    case KEY_UP:
        _bBackDoorIndex--;
        if (_bBackDoorIndex == 0xff)
            _bBackDoorIndex = TOTAL_BACK_DOOR_NUM - 1;
#ifdef  CT909P_IC_SYSTEM
        if (SETUP_BACK_DOOR_DEBUG_MODE == _bBackDoor[_bBackDoorIndex]) 
        {
            _bDebugSetMode = FALSE;
            _dwDebugMode = UTL_Get_DebugMode();	
            _bDebugIndex = _GetDebugIndex(_dwDebugMode);
            _dwBackupDebugMode = _dwDebugMode;                
            _ClearVersionRegion();
            _OutputDebugInfo();
        }
        else
        {
            OSDMENU_Initial();
            OSDMENU_SetMenuPalEntry();
            _ClearVersionRegion();
            OSDMENU_DisplayTheMenu();
        }			
#endif		
        break;
    case KEY_DOWN:
        _bBackDoorIndex++;
        if (_bBackDoorIndex >= TOTAL_BACK_DOOR_NUM)
            _bBackDoorIndex = 0;
#ifdef  CT909P_IC_SYSTEM
        if (SETUP_BACK_DOOR_DEBUG_MODE == _bBackDoor[_bBackDoorIndex]) 
        {
            _bDebugSetMode = FALSE;
            _dwDebugMode = UTL_Get_DebugMode();	
            _bDebugIndex = _GetDebugIndex(_dwDebugMode);			
            _dwBackupDebugMode = _dwDebugMode;                
            _ClearVersionRegion();
            _OutputDebugInfo();
        }
        else
        {
            OSDMENU_Initial();
            OSDMENU_SetMenuPalEntry();
            _ClearVersionRegion();
            OSDMENU_DisplayTheMenu();
        }			
#endif				
        break;
    case KEY_LEFT:        
#ifdef SUPPORT_MENU_CURSOR_STAY_AT_VALUE_AFTER_ENTER
        // Brian1.08a, use another variable, and _SaveDataToSetupInfo also check this variable
        __wMENUItemIdAfterEnter = _bBackDoor[_bBackDoorIndex];
#else
        __wMENUCurrentItemId = _bBackDoor[_bBackDoorIndex];
#endif
        if (SETUP_BACK_DOOR_REGION == _bBackDoor[_bBackDoorIndex])
        {
            _bBackDoorRegion--;
            if (_bBackDoorRegion == 0xFF)
                _bBackDoorRegion = SETUP_REGION_NUM - 1;
        } 
        else if (SETUP_BACK_DOOR_DEBUG_MODE == _bBackDoor[_bBackDoorIndex]) 
        {
/*        
            _bDebugIndex = _GetDebugIndex();
	    if (0 == _bDebugIndex)
            {
                _bDebugIndex = MAX_DBG_MODE - 1;
            } 
            else
            {
                _bDebugIndex--;
            }
            __bDebugMode = aDebugMode[_bDebugIndex];
            UTL_Config_DebugMode(UTL_DBG_SET);
*/
#ifdef  CT909P_IC_SYSTEM   
            _bDebugSetMode = TRUE;
            _dwDebugMode = UTL_Get_DebugMode();	
            _bDebugIndex = _GetDebugIndex(_dwDebugMode);
            _GetUsefulDebugMode(FALSE);
            _SetDebugMode(_bDebugIndex);
            UTL_Config_DebugMode(UTL_DBG_SET, _dwDebugMode);   
            _ClearVersionRegion();
            _OutputDebugInfo();               
#endif
        }
        _SaveDataToSetupInfo();
        break;
    case KEY_RIGHT:
#ifdef SUPPORT_MENU_CURSOR_STAY_AT_VALUE_AFTER_ENTER
        // Brian1.08a, use another variable, and _SaveDataToSetupInfo also check this variable
        __wMENUItemIdAfterEnter = _bBackDoor[_bBackDoorIndex];
#else
        __wMENUCurrentItemId = _bBackDoor[_bBackDoorIndex];
#endif
        if (SETUP_BACK_DOOR_REGION == _bBackDoor[_bBackDoorIndex])
        {
            _bBackDoorRegion++;
            if (_bBackDoorRegion == SETUP_REGION_NUM)
                _bBackDoorRegion = 0;
        }
        else if (SETUP_BACK_DOOR_DEBUG_MODE == _bBackDoor[_bBackDoorIndex])
        {
/*        
            _bDebugIndex = _GetDebugIndex();
            _bDebugIndex++;
	    if (MAX_DBG_MODE == _bDebugIndex)
            {
                _bDebugIndex = 0;
            } 
            __bDebugMode = aDebugMode[_bDebugIndex];
            UTL_Config_DebugMode(UTL_DBG_SET);
*/
#ifdef  CT909P_IC_SYSTEM   
            _bDebugSetMode = TRUE;   
            _dwDebugMode = UTL_Get_DebugMode();	
            _bDebugIndex = _GetDebugIndex(_dwDebugMode);
            _GetUsefulDebugMode(TRUE);
            _SetDebugMode(_bDebugIndex);	
            UTL_Config_DebugMode(UTL_DBG_SET, _dwDebugMode);   
            _ClearVersionRegion();
            _OutputDebugInfo();               
#endif			
        }
        _SaveDataToSetupInfo();
        break;

        // LLY2.78b-3, merge all action to display desired version within backdoor
    // [1] KEY_MUTE: Customer S/W version (maybe different to our F/W version)
    // [2] KEY_PAUSE: ATAPI servo code version
    // [3] KEY_ZOOM: Real F/W version
    case KEY_MUTE:
    case KEY_ZOOM: // Brian2.79, use KEY_ZOOM instead //KEY_ZOOM
        if (_bDoBackDoor)
        {
	    _ClearVersionRegion();
            _OutputAllSWVersion();
            OS_DelayTime(COUNT_5_SEC); //CoCo1.20, reduce code size

            // Brian1.11b, move to here
            //CoCo1.11a, for backdoor
            OSDMENU_Initial();
            OSDMENU_SetMenuPalEntry();
	    _ClearVersionRegion();
            OSDMENU_DisplayTheMenu();
/*
            OSDMENU_DisplaySubMenu(__bMENUSelect[__bMENUCurrentLevel],
                                   __bMENUCurrentLevel);
*/
        }
        break;
    default:
        break;
     }


    switch (_bBackDoor[_bBackDoorIndex])
    {
    case SETUP_BACK_DOOR_REGION:
#ifndef NO_DISC_MODE //CoCo2.37p
        __bInvalidRegion = TRUE;
        _bSetupTemp = _ConvertBackDoorRegionNum(__SetupInfo.bRegionControl);

        //Brian1.05, just call NV_Setting, it will set __wSPRMs[20] always
        NV_Setting(SET_REGION_CODE, 0);

        _SETUP_OutputNumber(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y, _bSetupTemp,
                            OSDSETUP_ENTRY_COLOR_TOP_BANNER_TEXT, 
                            OSDSETUP_ENTRY_COLOR_TOP_BANNER);

#endif //#ifndef NO_DISC_MODE //CoCo2.37p
        break;

    case SETUP_BACK_DOOR_DEBUG_MODE:
        _SETUP_OutputNumber(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y,
                            6000 + _bDebugIndex,
                            OSDSETUP_ENTRY_COLOR_TOP_BANNER_TEXT, 
                            OSDSETUP_ENTRY_COLOR_TOP_BANNER);
        break;
    default:
        break;
    }
}


#ifndef REMOVE_SETUP_SPEAKER_TEST
void _SpeakerTest(void)
{
    switch (__wMENUCurrentItemId)
    {
        case SETUP_AUDIO_SPEAKER_TEST_LEFT:
            _SETUP_ShowSpeaker(SPEAKER_L|SPEAKER_HIGHLIGHT);
            _bTestToneStage = 0; // set to 0, will not trigger Auto mode stage
            _SetTestTone(L_CHANNEL);
			break;
        case SETUP_AUDIO_SPEAKER_TEST_RIGHT:
            _SETUP_ShowSpeaker(SPEAKER_R|SPEAKER_HIGHLIGHT);
            _bTestToneStage = 0; // set to 0, will not trigger Auto mode stage
            _SetTestTone(R_CHANNEL);
			break;
        case SETUP_AUDIO_SPEAKER_TEST_LS:
            _SETUP_ShowSpeaker(SPEAKER_LS|SPEAKER_HIGHLIGHT);
            _bTestToneStage = 0; // set to 0, will not trigger Auto mode stage
            _SetTestTone(LS_CHANNEL);
			break;
        case SETUP_AUDIO_SPEAKER_TEST_RS:
            _SETUP_ShowSpeaker(SPEAKER_RS|SPEAKER_HIGHLIGHT);
            _bTestToneStage = 0; // set to 0, will not trigger Auto mode stage
            _SetTestTone(RS_CHANNEL);
			break;
        case SETUP_AUDIO_SPEAKER_TEST_CENTER:
            _SETUP_ShowSpeaker(SPEAKER_C|SPEAKER_HIGHLIGHT);
            _bTestToneStage = 0; // set to 0, will not trigger Auto mode stage
            _SetTestTone(C_CHANNEL);
			break;
        case SETUP_AUDIO_SPEAKER_TEST_SW:
            _SETUP_ShowSpeaker(SPEAKER_SW|SPEAKER_HIGHLIGHT);
            _bTestToneStage = 0; // set to 0, will not trigger Auto mode stage
            _SetTestTone(SW_CHANNEL);
			break;
        case SETUP_AUDIO_SPEAKER_TEST_AUTO:
            // it start from L channel, so highlight the speaker
            _SETUP_ShowSpeaker(SPEAKER_L|SPEAKER_HIGHLIGHT);
            _SetTestTone(L_CHANNEL);
            _dwStageCount = __dwCountSystem;
            _bTestToneStage = 1; // start from left channel
            break;
    }
}
#endif // #ifndef REMOVE_SETUP_SPEAKER_TEST

//Brian1.06
void _SetAnalogEnable(void)
{
    //Brian1.05
    if (__SetupInfo.bAudioAnalogOut == SETUP_AUDIO_ANALOG_OUT_2CH)
    {
        // 2 channel, disable Center, Rear and SW items
        _SetMenuEnable(3);
    }
    else
    {
        // 5.1 channel, enable Center, Rear and SW items
        _SetMenuEnable(4);
    }
}

void _SetDownMix(void)
{
    if (SETUP_AUDIO_DOWNMIX_LORO == __SetupInfo.bAudioDownMix)
    {
        CHIPS_DownMixMode(AC3_DOWNMIX_LORO);
    }
    else
    {
        CHIPS_DownMixMode(AC3_DOWNMIX_LTRT);
    }
}

//CoCo1.11a, for backdoor
//Output string by the following sequence. F/W version, Customer version, SERVO version, FAE version
void _OutputAllSWVersion(void)
{
    //=================================================
    // Output version info in the 1st line
    //=================================================

    // output the description text

    _SETUP_StrAttr.bBackground = OSDSETUP_ENTRY_CENTER_COLOR_BG;
    _SETUP_StrAttr.bTextColor = OSDSETUP_ENTRY_COLOR_WHITE;
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_StrAttr.wY = BACKDOOR_DISPLAY_VERSION_START_V;

    //Show Customer version.
    if (0 != __csVersionCustomer[0])
    {
        _TranslateString((BYTE*)__csVersionCustomer); 
        _SETUP_DrawString(); //CoCo1.20, reduce code size.
        _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);
    }


    //Show F/W version.
    _SETUP_DrawDynamicString(aSETUPFW); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionRelease, FORMAT_VERSION);

    //Show FAE      version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    //GDI_DrawString_909(0, &_SETUP_StrAttr, _wSetupString); //--CoCo1.20, reduce code size.
    _SETUP_DrawDynamicString(aSETUPFAE); //CoCo1.20, reduce code size.

    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionFAE, FORMAT_VERSION);
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);
    
    //Show MPEG version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_DrawDynamicString(aSETUPMPEG); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionMPEG, FORMAT_DATE);

    //Show JPEG version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    _SETUP_DrawDynamicString(aSETUPJPG); //--CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionJPEG, FORMAT_DATE);
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);

    //Show Display version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_DrawDynamicString(aSETUPDisp); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionDisplay, FORMAT_DATE);

    //Show DivX version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    _SETUP_DrawDynamicString(aSETUPDivX); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionDivX, FORMAT_DATE);
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);

    //Show Navigate version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_DrawDynamicString(aSETUPNav); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionNavigate, FORMAT_DATE);

    //Show USB version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    _SETUP_DrawDynamicString(aSETUPUSB);
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionUSB, FORMAT_DATE);
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);

    //Show Parser version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_DrawDynamicString(aSETUPParser); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionParser, FORMAT_DATE);

    //Show Info version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    _SETUP_DrawDynamicString(aSETUPInfo);
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionINFO, FORMAT_DATE);
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);

    // Proc2 / Audio Version
    HAL_ReadAM(HAL_AM_PROC2_VERSION, &__dwTemp);
    __dwTemp = (((__dwTemp & 0x1E0) << 3) | (__dwTemp & 0x1F)) | 0x00060000;
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_DrawDynamicString(aSETUPAudio); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwTemp, FORMAT_DATE);

    // Proc2 / Audio Support Feature 
    HAL_ReadAM(HAL_AM_PROC2_SUPPORT, &__dwTemp);
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    _SETUP_DrawDynamicString(aSETUPASupport); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwTemp, FORMAT_NUMBER);
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);

    //Show Card version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_DrawDynamicString(aSETUPCard);
    _SETUP_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionCard, FORMAT_DATE);

#ifdef SUPPORT_TFT
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    _SETUP_DrawDynamicString(aSETUPTFT); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET + 75;
    _OutputVersion(__dwVersionTFT, FORMAT_VERSION);
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);
#else
    // Give one more line spaceing
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);
#endif

#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
    //Show Servo module version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_DrawDynamicString(aSETUPServo); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET + 75;
    _OutputVersion(__dwVersionServo, FORMAT_VERSION);

    //Show BM version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    _SETUP_DrawDynamicString(aSETUPBM); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET + 75;
    _OutputVersion(__dwVersionServoBM, FORMAT_VERSION);
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);

    //Show Servo uCode version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_DrawDynamicString(aSETUPuCode); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET + 75;
    _OutputVersion(__dwVersionServoUCode, FORMAT_VERSION);
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p

    //Show PCB Sytem version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_DrawDynamicString(aSETUPPCBSYSTEM); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET + 75;
#ifndef SUPPORT_ATAPI_SOURCE
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
    _OutputVersion(SRVDBG_PCBNumber(), FORMAT_NUMBER);
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
#endif

    //Show RF Type version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    _SETUP_DrawDynamicString(aSETUPRFTYPE); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET + 75;
#ifndef SUPPORT_ATAPI_SOURCE
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
    _OutputVersion(SRVDBG_RFAmpChip(), FORMAT_NUMBER);
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
#endif
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);

    //Show Motor Type version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    _SETUP_DrawDynamicString(aSETUPMOTORTYPE); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET + 75;
#ifndef SUPPORT_ATAPI_SOURCE
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
    _OutputVersion(SRVDBG_MotorDriver(), FORMAT_NUMBER);
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
#endif

    //Show Motor Power version.
    _SETUP_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    _SETUP_DrawDynamicString(aSETUPLOADER); //CoCo1.20, reduce code size.
    _SETUP_StrAttr.wX += VERSION_OFFSET + 75;
#ifndef SUPPORT_ATAPI_SOURCE
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
    _OutputVersion(SRVDBG_MotorPower(), FORMAT_NUMBER);
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
#endif
    _SETUP_StrAttr.wY += GDI_GetStringHeight(_wSetupString);
}

//CoCo1.11a, for backdoor
void _OutputVersion(DWORD dwVersion, BIT btFormat)
{
    BYTE bTemp = 1;

    switch (btFormat)
    {
        case FORMAT_VERSION:
            _wSetupTemp = dwVersion >> 16; 

            if (0 != _wSetupTemp / 1000)
            {
                _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp / 1000);
            }
            _wSetupTemp = _wSetupTemp % 1000;

            _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp / 100);

            _wSetupString[bTemp++] = CHAR_FULL_STOP;

            _wSetupTemp = _wSetupTemp % 100;
            _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp / 10);

            _wSetupTemp = _wSetupTemp % 10;
            _wSetupString[bTemp++] = CHAR_0 + _wSetupTemp;

            _wSetupTemp = (dwVersion & 0x0000FF00) >> 8; 
            if (_wSetupTemp)
            {
                _wSetupString[bTemp++] = CHAR_a + (_wSetupTemp - 1);
            }
            else // Brian1.10,  for displaying correct FAE version
            {
                _wSetupString[bTemp++] = CHAR_LOW_LINE;
            }
            
            _wSetupTemp = dwVersion & 0x000000FF; 
            if (_wSetupTemp)
            {
                _wSetupString[bTemp++] = CHAR_a + (_wSetupTemp - 1);
            }
            else // Brian1.10,  for displaying correct FAE version
            if (_wSetupString[bTemp-1] == CHAR_LOW_LINE)
            {
                bTemp--;
            }
            
            _wSetupString[0] = bTemp - 1;
            _SETUP_DrawString(); //CoCo1.20, reduce code size.
            break;
        case FORMAT_DATE:
            _wSetupTemp = dwVersion >> 16; 
            _wSetupTemp %= 100;
            _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp / 10);
            _wSetupTemp = _wSetupTemp % 10;
            _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp % 10);
            _wSetupString[bTemp++] = CHAR_HYPHEN_MINUS;   // Year

            _wSetupTemp = (dwVersion & 0x0000FF00) >> 8; 
            _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp / 10);
            _wSetupTemp = _wSetupTemp % 10;
            _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp % 10);
            _wSetupString[bTemp++] = CHAR_HYPHEN_MINUS;   // Month

            _wSetupTemp = dwVersion & 0x000000FF; 
            _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp / 10);
            _wSetupTemp = _wSetupTemp % 10;
            _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp % 10);
            _wSetupString[bTemp++] = CHAR_HYPHEN_MINUS;   // Day

            _wSetupTemp = dwVersion >> 16;
            _wSetupTemp /= 100;
            OSDUTL_PrepareOutputString();
            OSDUTL_OutputNumFromIndex(_wSetupTemp);
            memcpy(&(_wSetupString[bTemp]), &(__wOSDUTLStringBuf[1]),
                   __wOSDUTLStringBuf[0]*2);
            bTemp += __wOSDUTLStringBuf[0];    // Version

            _wSetupString[0] = bTemp - 1;
            _SETUP_DrawString(); //CoCo1.20, reduce code size.
            break;
        case FORMAT_NUMBER:
            _wSetupTemp = dwVersion; 
            if (0 != _wSetupTemp / 1000)
            {
                _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp / 1000);
            }
            _wSetupTemp = _wSetupTemp % 1000;

            _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp / 100);

            _wSetupTemp = _wSetupTemp % 100;
            _wSetupString[bTemp++] = CHAR_0 + (_wSetupTemp / 10);

            _wSetupTemp = _wSetupTemp % 10;
            _wSetupString[bTemp++] = CHAR_0 + _wSetupTemp;

            _wSetupString[0] = bTemp - 1;
            _SETUP_DrawString(); //CoCo1.20, reduce code size.
/*
            // --CoCo1.20, reduce code size.
            OSDUTL_PrepareOutputString();
            OSDUTL_OutputNumFromIndex((WORD)dwVersion);
            GDI_DrawString_909(0, &_SETUP_StrAttr, __wOSDUTLStringBuf);
*/
            break;
        default:
            break;
    }
}

void _OutputDebugInfo(void)
{
    PARM_DRAW_STRING DebugStrAttr;  
    WORD wStrTmp[9];

    _bCARDPath = FALSE;
    _bNIMPath = FALSE;
	
    DebugStrAttr.bBackground = OSDSETUP_ENTRY_CENTER_COLOR_BG;
    DebugStrAttr.bTextColor = OSDSETUP_ENTRY_COLOR_WHITE;
    DebugStrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H+10;
    DebugStrAttr.wY = BACKDOOR_DISPLAY_VERSION_START_V+15;
    // Show titile: Debug Mode Info
    GDI_DrawString_909(0, &DebugStrAttr, aSETUPDebugModeInfo);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPDebugModeInfo);
	
    // Show default debug mode value	    
    OSDUTL_PrepareOutputString();    
    OSDUTL_CopyStringFromIndex(aSETUPDefault);     
    wStrTmp[0] = 8;	
    wStrTmp[1] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF0000000) >> 28));
    wStrTmp[2] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF000000) >> 24));	
    wStrTmp[3] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF00000) >> 20));    
    wStrTmp[4] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF0000) >> 16));
    wStrTmp[5] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF000) >> 12));
    wStrTmp[6] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF00) >> 8));
    wStrTmp[7] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF0) >> 4));
    wStrTmp[8] = _TranslateNum((BYTE)(_dwBackupDebugMode & 0xF));
    OSDUTL_CopyStringFromIndex(wStrTmp);      
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);   
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPDefault);

    // Show new debug mode value
    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aSETUPNew);    
    if (_bDebugSetMode)
    {
        wStrTmp[0] = 8;	
        wStrTmp[1] = _TranslateNum((BYTE)((_dwDebugMode & 0xF0000000) >> 28));
        wStrTmp[2] = _TranslateNum((BYTE)((_dwDebugMode & 0xF000000) >> 24));	
        wStrTmp[3] = _TranslateNum((BYTE)((_dwDebugMode & 0xF00000) >> 20));    
        wStrTmp[4] = _TranslateNum((BYTE)((_dwDebugMode & 0xF0000) >> 16));
        wStrTmp[5] = _TranslateNum((BYTE)((_dwDebugMode & 0xF000) >> 12));
        wStrTmp[6] = _TranslateNum((BYTE)((_dwDebugMode & 0xF00) >> 8));
        wStrTmp[7] = _TranslateNum((BYTE)((_dwDebugMode & 0xF0) >> 4));
        wStrTmp[8] = _TranslateNum((BYTE)(_dwDebugMode & 0xF));
        OSDUTL_CopyStringFromIndex(wStrTmp);      
    }
    else
    {
        OSDUTL_CopyStringFromIndex(aSETUPNotSet);
    }
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPNew);

    // Show DSU_TX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPDSU_TX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)(_dwDebugMode & 0xF)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPDSU_TX);

    // Show DSU_RX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPDSU_RX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)((_dwDebugMode & 0xF0) >> 4)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPDSU_RX);

    // Show UR1_TX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPUR1_TX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)((_dwDebugMode & 0xF00) >> 8)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPUR1_TX);

    // Show UR1_RX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPUR1_RX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)((_dwDebugMode & 0xF000) >> 12)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPUR1_RX);

    // Show UR2_TX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPUR2_TX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)((_dwDebugMode & 0xF0000) >> 16)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPUR2_TX);

     // Show UR2_RX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPUR2_RX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)((_dwDebugMode & 0xF00000) >> 20)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPUR2_RX) + 10;

    if (_bCARDPath)
    {
        OSDUTL_PrepareOutputString();   
        OSDUTL_CopyStringFromIndex(aSETUPDBGInfo1);        
        GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
        DebugStrAttr.wY += GDI_GetStringHeight(__pOSDUTLOutputString);
    }

    if (_bNIMPath)
    {
        OSDUTL_PrepareOutputString();   
        OSDUTL_CopyStringFromIndex(aSETUPDBGInfo2);        
        GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
        DebugStrAttr.wY += GDI_GetStringHeight(__pOSDUTLOutputString);
    }	
}

WORD _TranslateNum(BYTE bNum)
{
    if (bNum < 10)
    {
        return aNDNumbers[bNum];
    }
    else if (bNum <= 15)
    {
        return aNDUppercase[bNum-10];
    }
    
    return aNDNumbers[0]; // Non matched.
    
}

WORD* _TranslateDebugMode(BYTE bMode)
{
    WORD * pwDebugModStr;

    switch (bMode)
    {
    case 0:
        pwDebugModStr = aSETUPDisable;
        break;    
    case 1: 
        _bNIMPath = TRUE;
        pwDebugModStr = aSETUPEnable;
        break;
    case 2: 
        _bCARDPath = TRUE;
        pwDebugModStr = aSETUPEnable;
        break;
    default:
        pwDebugModStr = aSETUPEnable;
        break;
    }    

    return pwDebugModStr;
}

BYTE _GetDebugIndex(DWORD dwMode)
{
    switch (dwMode)
    {
    case DEBUG_MODE_1:        		
        return 1;
    case DEBUG_MODE_2:
        return 2;
    case DEBUG_MODE_3:
        return 3;
    case DEBUG_MODE_4:
        return 4;	
    case DEBUG_MODE_5:
        return 5;
    case DEBUG_MODE_6:		
        return 6;
    case DEBUG_MODE_7:		
        return 7;
    case DEBUG_MODE_8:		
        return 8;
    case DEBUG_MODE_9:		
        return 9;
    case DEBUG_MODE_10:		
        return 10;
    case DEBUG_MODE_11:		
        return 11;		
    case DEBUG_MODE_0:	
    default:
        return 0;
    }
}

void _SetDebugMode(BYTE bIndex)
{
    _dwDebugMode = 0;
	
    switch (bIndex)
    {                              
    case 1: 
        _dwDebugMode|=SET_DSU1_TX_VALUE(NIM_PATH);    //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NIM_PATH);    //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NOT_USED);  //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);  //UART1_RX_SOURCE << 12;                     
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);  //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);   //UART2_RX_SOURCE << 20;
        break;
    case 2: 
        _dwDebugMode|=SET_DSU1_TX_VALUE(CARD_READER_PATH);      //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(CARD_READER_PATH);    //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NOT_USED);               //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);               //UART1_RX_SOURCE << 12;                    
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);               //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);               //UART2_RX_SOURCE << 20;
        break;
    case 3:
        _dwDebugMode|=SET_DSU1_TX_VALUE(NOT_USED);       //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NOT_USED);      //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NIM_PATH);      //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NIM_PATH);      //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);     //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);     //UART2_RX_SOURCE << 20;
        break;
    case 4: 
        _dwDebugMode|=SET_DSU1_TX_VALUE(NIM_PATH);       //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NIM_PATH);       //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NIM_PATH);     //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NIM_PATH);     //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);    //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);    //UART2_RX_SOURCE << 20;
        break;
    case 5:
        _dwDebugMode|=SET_DSU1_TX_VALUE(CARD_READER_PATH);       //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(CARD_READER_PATH);      //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NIM_PATH);                  //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NIM_PATH);                  //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                 //UART2_RX_SOURCE << 20;
        break;
    case 6: 
        _dwDebugMode|=SET_DSU1_TX_VALUE(NOT_USED);                    //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NOT_USED);                   //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);     //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(CARD_READER_PATH);     //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                  //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                  //UART2_RX_SOURCE << 20;
        break;
    case 7:       
        _dwDebugMode|=SET_DSU1_TX_VALUE(NIM_PATH);                      //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NIM_PATH);                      //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);      //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(CARD_READER_PATH);      //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                   //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                   //UART2_RX_SOURCE << 20;
        break;    
    case 8:     
        _dwDebugMode|=SET_DSU1_TX_VALUE(CARD_READER_PATH);       //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(CARD_READER_PATH);      //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);     //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(CARD_READER_PATH);     //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                 //UART2_RX_SOURCE << 20;
        break; 
    case 9:    
        _dwDebugMode|=SET_DSU1_TX_VALUE(CARD_READER_PATH);          //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(CARD_READER_PATH);          //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);         //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);                     //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                      //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(EXPAND_GPIO_PATH);         //UART2_RX_SOURCE << 20;
        break; 
    case 10:         
        _dwDebugMode|=SET_DSU1_TX_VALUE(NIM_PATH);                      //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NIM_PATH);                      //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);       //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);                   //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                    //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(EXPAND_GPIO_PATH);       //UART2_RX_SOURCE << 20;
        break;
    case 11:    
        _dwDebugMode|=SET_DSU1_TX_VALUE(NOT_USED);                       //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NOT_USED);                      //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);        //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);                     //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                     //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(EXPAND_GPIO_PATH);        //UART2_RX_SOURCE << 20;
        break; 
    case 0: 
    default: 
        _dwDebugMode|=SET_DSU1_TX_VALUE(NOT_USED);          //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NOT_USED);          //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NOT_USED);        //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);        //UART1_RX_SOURCE << 12;    
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);        //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);        //UART2_RX_SOURCE << 20;        
        break;
    }
}

void _GetUsefulDebugMode(BYTE bDIR)
{
    DWORD dwDBGMode, dwCompare;
	
    dwDBGMode = UTL_Get_Path_DebugMode();
	
    if (bDIR) // Increase Index
    {
        do
        {
            _bDebugIndex++;
            if (_bDebugIndex >= MAX_DBG_MODE)
            {
                _bDebugIndex = 0;
            }   
            
            dwCompare = 1L << _bDebugIndex;			
        } while (!(dwCompare & dwDBGMode));            
    }
    else // Decrease Index
    {
        do
        {
            if (_bDebugIndex == 0)
            {
                _bDebugIndex = MAX_DBG_MODE - 1;
            }
            else
            {
                _bDebugIndex--;
            } 
            
            dwCompare = 1L << _bDebugIndex;			
        } while (!(dwCompare & dwDBGMode)); 
    }
}

void _ClearVersionRegion(void)
{
    _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
    _SETUP_FillRect(SETUP_FRAME_X0, CENTER_FRAME_Y0,
            SETUP_FRAME_X1, CENTER_FRAME_Y1); //CoCo1.20, reduce code size.
}

void SETUP_GDIInit(void)
{
//Alan1.00    OSD_LoadFontTable(FONTABLE_SETUP);
    //GDI_LoadPalette(GDI_PAL_SETUP); // Load Setup BMP palette
    //GDI_LoadBMP(GDI_SETUP_BMP); // load Setup BMP
    //Brian,  support GDI library
    GDI_LoadPalette(&__PaletteInfo_Setup);

    //--CoCo1.20, reduce code size.
#ifndef REMOVE_SETUP_SPEAKER_TEST
    GDI_LoadBMP(_aSetupBMPArray, GDI_MAX_SETUP_BMP_NUM); // load Setup BMP
#endif

    // Brian1.25
    // load entry 255 for transparency color in case the palette is destroyed.
    // The other entries for Setup usage is loaded by OSDMENU_SetMenuPalEntry, so don't worry being destroyed.
    //GDI_LoadPalette(0);
    //Brian,  support GDI library
    GDI_LoadPalette(&__PaletteInfo_Transparent);
    
#ifdef USE_ONE_OSD_REGION
    GDI_InitialRegion (0); // initialize region to set it to 8-bit mode
    // brian1.22, fix PAL mode, enter Setup, OSD V position shift
    // (it was set to NTSC by Init region)
    // OSD_FillHeadtoDRAM(OSD_FILLHEADER_ENTER_SETUP);
    //OSD_AdjustRegion(); //CoCo.CT909
    GDI_ActivateRegion(0);
#else
    // All 8-bit OSD should use Region 1
    GDI_ClearRegion(1); // Clear region first
    GDI_SET_CURRENT_REGION(1);
    GDI_ActivateRegion();
#endif

    // Brian1.20
    /*--CoCo1.20, reduce code size, don't need to do this in SETUP.
#if defined(REMOVE_SETUP_ICON) && defined(REMOVE_SETUP_SPEAKER_TEST)
	GDI_SetGDIPalEntry();
#else
    // Brian, no use
    //GDI_LoadPalette(1); // load color table for Setup UI BMP	
#endif
    */
    
    GDI_SetMixRatio(GDI_GENERAL_MIX_RATIO);

    _SETUP_InitPalette();

    OSDMENU_SetMenuPalEntry();
}


//***************************************************************************
//  Function    :   _SETUP_ShowUI 
//  Abstract    :   Draw the Setup UI.
//  Arguments   :
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void _SETUP_ShowUI(void)
{
#ifndef REMOVE_SETUP_SPEAKER_TEST
    // test tone always auto
    __SetupInfo.bAudioTestTone = SETUP_AUDIO_SPEAKER_TEST_AUTO;
#endif

#ifndef REMOVE_SETUP_SPEAKER_TEST
    _bTestToneStage = 0; // initial
    _bDSPTestTone = 0;
#endif

    __bSetupMode = TRUE;

    _bAudioTypeChange = FALSE;
    _bVideoSettingChange = FALSE; 
    _bParentalControlChange = FALSE;

    // put Menu Initial here before _SetMenuEnable(), because we want the pointer _pMenuIndex
    // points to the correct menu at the beginning 
    OSDMENU_Initial();

    if (!(__wDiscType & BOOK_DVD))
    {
        if (__wDiscType==NO_DISC_IN_DRIVE)
            _SetMenuEnable(0);
        else
        {
            // Brian2.79, use __bCountStop to determine the status
            if (__bCountStop < 2) // means playing or pre-stop
                _SetMenuEnable(1);
            else // stop
                _SetMenuEnable(0);
        }

        if (__btPlaying) // Brian2.81, thumbnail support KEY_PAUSE
        {
// brian.171a, change to KEY_PLAY then KEY_PAUSE
// This is a workaround to make SCF/SCB, FF look like paused
// (before the real KEY_PAUSE issue is solved)
// However add this would also make it change to PLAY mode when exit Setup.
// This is a side effect. When KEY_PAUSE issue is sovled for SCF/SCB, FF,
// we could remove this part
#ifdef  PLAY_AFTER_EXIT_SETUP
            // this for none-DVD
            if (__bAttrPlay&TYPE_CDROM_AUDIO) // Brian1.25, audio only could directly enter pause mode. Otherwise ==> // Kevin, MP3 SCF/SCB->Setup->Exit Setup=> jump to next file (below (KEY_PLAY) will reset audio s.t. A2-Rem < 720) 
                __bModeCmd = KEY_PLAY;

#ifndef PRESTOP_BEFORE_SETUP_AND_PROGRAM
            //xuli2.34, Because MP3 still toggle KEY_PAUSE when enter/exit SETUP, need set __bModeCmd to KEY_PLAY before issue KEY_PAUSE, or else KEY_PAUSE will be INVALID.                  
            // wyc2.00-909S, don't issue play command again when exit setup mode from JPG preview active, because preview mode also is play
            // mode and don't need to issue play again. And issue play again also will call reset video and it will abort JPG decode and make
            // JPG decoding progress aborted and won't continue to decode after exit setup mode.
            if (( __bModeCmd != KEY_PLAY) && (__dwMMJPEGPreview == FALSE) && (!__bThumbnailMode))  // if it is already in play mode, don't need to issue KEY_PLAY again
                CC_DoKeyAction (KEY_PLAY);
#endif  //#ifndef PRESTOP_BEFORE_SETUP_AND_PROGRAM
#endif  //#ifdef  PLAY_AFTER_EXIT_SETUP

#ifndef PRESTOP_BEFORE_SETUP_AND_PROGRAM //XuLi.230
             CC_DoKeyAction (KEY_PAUSE);
#endif
        }

        // Brian1.10, add kevin's support for MP3 Fast Forward.
#ifdef  PLAY_AFTER_EXIT_SETUP
        //MP3 Fast Forward case, SCF->enter SETUP->exit SETUP => Normal Play
        // wyc2.38-909s, need to add the WMA case.
        if ((__bAttrPlay==ATTR_MP3) || (__bAttrPlay==ATTR_WMA))
        {   
            // always set to normal play
            __bScanLevel=0;
            HAL_WriteAM(HAL_AM_FF_SPEED, __bScanLevel); 
        }
#endif

#ifndef NO_DIGEST //LJY1.20, support 4M flash, remove digest
        //LJY1.00, fix the noise problem while it entered setup in digest mode.
        if(__btDIGEST)
        {
            CHIPS_OpenAudio(FALSE);
        }
#endif

    }
    else if (__btPlaying)
    {
// brian.171a, chnage to KEY_PLAY then KEY_PAUSE
#ifdef  PLAY_AFTER_EXIT_SETUP
#ifndef PRESTOP_BEFORE_SETUP_AND_PROGRAM //XuLi.230
        // this is for DVD
        if (__bModeCmd != KEY_PLAY) // if it is already in play mode, don't need to issue KEY_PLAY again
            CC_DoKeyAction (KEY_PLAY);
#endif  //#ifndef PRESTOP_BEFORE_SETUP_AND_PROGRAM
#endif  //#ifdef  PLAY_AFTER_EXIT_SETUP

#ifndef PRESTOP_BEFORE_SETUP_AND_PROGRAM //XuLi.230
        CC_DoKeyAction (KEY_PAUSE);
#endif
        _SetMenuEnable(2);  //run-time mode
    }
    else
    {
        // Brian2.79, use __bCountStop to determine the status
        if (__bCountStop < 2) // means playing or pre-stop
            _SetMenuEnable(2);
        else // stop
            _SetMenuEnable(0);
    }


#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
    // wyc2.22
    //_SETUP_EnterByPrestop();
    CC_ForceStop(CC_FORCE_STOP_FROM_SETUP);
#endif

    _SETUP_StrAttr.bShadow1 = 1;
    _SETUP_StrAttr.bShadow2 = 1;

    __bOSDLanguage = __SetupInfo.bOSDLanguage;

    _bPasswordOK = FALSE;

    // inform OSD that we are entering Setup
    // OSD module should update _bCurrentUI and _bPreviousUI
    //Tell OSD that the Display mode UI enters.
    // ** OSD_ChangeUI must be called AFTER prestop
    OSD_ChangeUI(OSD_UI_SETUP, OSD_ENTER_UI);

    OSDMENU_SetMenuPalEntry();

    _SetAnalogEnable();
            
    // Brian0.87, set one field display when enter Setup
    // Micky1.07, RISC support another feature, DVD still won't do 
    // one-field control must write the value as 0x20000.
            
    DISP_ForceOneField(TRUE); // ???
    OSDSETUP_Display();

    OSDMENU_DisplayTheMenu();
}



//***************************************************************************
//  Function    :   _SETUP_ClearUI 
//  Abstract    :   Draw the Setup UI.
//  Arguments   :
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void _SETUP_ClearUI(void)
{
//Alan1.00    OSD_LoadFontTable(FONTABLE_OSD);

    // Brian0.90 always do these when exit Setup

    _SETUP_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _SETUP_FillRect(0, 0, SETUP_FRAME_X1, SETUP_FRAME_Y1); //CoCo1.20, reduce code size.

#ifdef GDI_4_BIT_OSD
    // reload palette for 4-bit region as early as possible
    // this colud save some trouble: the pink-screen-flash problem
    GDI_SetGDIPalEntry();
#endif

    OSD_ChangeUI(OSD_UI_SETUP, OSD_EXIT_UI);
}



//  ***************************************************************************
//  Function    :   _ExitSetup
//  Description :   Exit SETUP and recover some flags.
//  Arguments   :   none.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void    _SETUP_Exit(void)
{
//#ifndef NO_OSD
#ifndef NO_SETUP

#ifndef REMOVE_SETUP_SPEAKER_TEST
    if (_bDSPTestTone) // Brian1.23 //Brian1.11d-2, fix "bo" noise when TestTone --> Exit Setup
    {
        CHIPS_SoftMute();
        OS_DelayTime(COUNT_50_MSEC); //CoCo1.20, reduce code size
    }
#endif

    // Brian1.07a, clear it, for this is also used by PScan Prompt
    __bInputPassword = FALSE;
#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
            // Brian1.07a, cancel PScan prompt
            __bOSDPromptMode = FALSE;
#endif

    // Brian0.87, disable one field display when exit Setup
    DISP_ForceOneField(FALSE);

    // brian.277a Test Tone
    // PCM_SCALE may be set by Test Tone, so we turn it off as it was when we enter Setup
    // if we don't turn PCM_SCALE off, there may be noise (on AMP) when it change Audio Output
    CHIPS_OpenAudio(FALSE);

    //clear the password status if user press KEY_SETUP to exist password page
    //_RecoverVariables();

#ifndef NO_DISC_MODE //CoCo2.37p
    //turn on highlight
    HAL_ControlHighlight(HAL_HLI_ACTION_SELECT, __bNowBTNNumber+__bGroupOffset);
    //HAL_Display(HAL_DISPLAY_SPST, TRUE);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

    __bKey=KEY_NO_KEY;
    //__bSetupMode = FALSE;

    // to prevent from doing a long time when the the level is changed.
#ifndef NO_DISC_MODE //CoCo2.37p
    if (_bParentalControlChange)
        NV_Setting(SET_PTL_ID_FLD, 0);
#endif //#ifndef NO_DISC_MODE //CoCo2.37p

#ifndef SETTING_CHANGE_IMMEDIATELY
    if (TRUE == _bVideoSettingChange)
        SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);

    // TCC276
    SETUP_AdjustSetting(SETUP_SCREEN_TV_TYPE);
        
    // Brian2.78b-2, need do this check anyway
    if (_bAudioTypeChange)
    {
        SETUP_AdjustSetting(SETUP_AUDIO_OUTPUT);
    }
#endif // #ifndef SETTING_CHANGE_IMMEDIATELY

#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
    // Alan2.30, Fix the mantis bug #1892.
    // The following code is duplicated from the case KEY_PLAY in MEDIA_MediaSelection_ProcessKey() of media.c
    // the purpose of enable main video in MM mode here is because main video will disable after enter meida select UI.
    // So we need to restore it here.
    if (__wDiscType & BOOK_M1)
    {
        // wyc2.00-909S, only restore main video in MM UI mode because JPG slide show or motion mode we don't disable main video when entering media select UI.
        // Also will happen video scramble when open main video during JPG decoding here, so must only open it when MM UI mode.
        if ((__bMMMenuID != MM_MENU_NONE) || (__bThumbnailMode))
        {
            DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
        }
    }

    //_SETUP_ExitByResume();
    // wyc2.22,
    CC_ForceResumeRePlay(CC_FORCE_STOP_FROM_SETUP);
#else // #ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
    if (__btPause)
    {
        // wyc2.00-909S, because the __bModeCmd is KEY_STOP when JPG preview decoding mode and it will make CC_DoKeyAction(KEY_PAUSE) to issue
        // CC_KeyCommand(KEY_STOP) and then JPG preview won't continue when exit setup mode. So we set __bModeCmd to KEY_PLAY to 
        // cheat CC_DoKeyAction(KEY_PAUSE) to issue CC_KeyCommand(KEY_PLAY) to continue to decode preview after exiting setup mode.
        if (__dwMMJPEGPreview == TRUE || __bThumbnailMode)
            __bModeCmd = KEY_PLAY;
        // Alan2.37. update the following code
        // Use CC_DoKeyAction(KEY_PLAY) to replace CC_DoKeyAction(KEY_PAUSE) to fix the bug.
        // When play MP3, pressing SCF -> PAUSE and then enter / exit setup, the MP3 can't playback 
        CC_DoKeyAction(KEY_PLAY);
//        CC_DoKeyAction(KEY_PAUSE);
    }
#endif

    __bSetupMode = FALSE;

    //Tell OSD that the Display mode UI enters.
//alan, comment    OSD_ChangeUI(OSD_UI_SETUP, OSD_EXIT_UI);

#ifndef NO_DISC_MODE //CoCo2.37p
    _IdentifyRegionCode();  // brian.161 check region code (show or clear the error message)
#endif //#ifndef NO_DISC_MODE //CoCo2.37p
   
#ifndef NO_DIGEST //LJY1.20, support 4M flash, remove digest
    //LJY0.87, enable audio for digest mode
    if(__btDIGEST)
        CHIPS_OpenAudio( TRUE );
#endif

#endif
}

void _SETUP_NumberToString(WORD wNum)
{
    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex(wNum);
    for(_bSetupTemp = 0; _bSetupTemp <= __wOSDUTLStringBuf[0]; _bSetupTemp++)
    { 
        _wSetupString[_bSetupTemp] = __wOSDUTLStringBuf[_bSetupTemp];
    }
/*
    _wSetupTemp = 10000;
    for (_bSetupTemp=5; _bSetupTemp>0; _bSetupTemp--)
    {
        if (wNum / _wSetupTemp) break;
        _wSetupTemp = _wSetupTemp/10;
    }

    if (_bSetupTemp == 0) _bSetupTemp = 1;
    if (_wSetupTemp == 0) _wSetupTemp = 1;

    _wSetupString[0] = _bSetupTemp;

    for (_bSetupTemp=1; _bSetupTemp<=_wSetupString[0]; _bSetupTemp++)
    {
        _wSetupString[_bSetupTemp] = aDigit[wNum/_wSetupTemp];
        wNum = wNum % _wSetupTemp;
        _wSetupTemp = _wSetupTemp/10;
    }
*/
}

void _SETUP_OutputNumber(WORD wX, WORD wY, WORD wNum, BYTE bTextColor, BYTE bBackgroundColor)
{
        _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_TOP_BANNER;
        _SETUP_FillRect(wX, wY, wX+95, wY+28); //CoCo1.20, reduce code size.

        _SETUP_NumberToString(wNum);
        _SETUP_StrAttr.wX = wX; _SETUP_StrAttr.wY = wY;
        _SETUP_StrAttr.bBackground = bBackgroundColor;
        _SETUP_StrAttr.bTextColor = bTextColor;
        _SETUP_DrawString(); //CoCo1.20, reduce code size.
}


void _SETUP_OutputString(WORD wX, WORD wY, BYTE bTextColor, BYTE bBackgroundColor)
{
    _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
    _SETUP_FillRect(BACKDOOR_SERVO_X, wY, //CoCo1.20, reduce code size.
                    BACKDOOR_SERVO_X+225, wY+31);

    _bSetupTemp = 0;
    while (__bMsgString[_bSetupTemp] != 0)
    {
        _bSetupTemp++;
    }
    _wSetupString[0] = _bSetupTemp;
    if(_wSetupString[0] > GDI_STRING_LENGTH)
        _wSetupString[0]=GDI_STRING_LENGTH;
    for(_bSetupTemp=0; _bSetupTemp<_wSetupString[0]; _bSetupTemp++)
    {
        _wSetupString[_bSetupTemp+1]= OSDUTL_TransformChar(__bMsgString[_bSetupTemp]);
    }
    
    _SETUP_StrAttr.wX = wX; _SETUP_StrAttr.wY = wY;
    _SETUP_StrAttr.bBackground = bBackgroundColor;
    _SETUP_StrAttr.bTextColor = bTextColor;
    _SETUP_DrawString(); //CoCo1.20, reduce code size.
}


void _SETUP_DrawSelectString(BYTE bOSDLanguage)
{

    _SETUP_StrAttr.wX = SETUP_FRAME_X0+255;
    _SETUP_StrAttr.wY = SETUP_FRAME_Y1-30;
    
    _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER;
 //CoCo1.20, reduce code size.
    _InitRect(&(_SETUP_RectAttr.rect), _SETUP_StrAttr.wX,
              SETUP_FRAME_Y1 - 30, SETUP_FRAME_X1 - 1, SETUP_FRAME_Y1 - 5);
//    GDI_FillRoundRect(0, 21, &_SETUP_RectAttr);   //It causes UI Style 2, draw error
    GDI_FillRoundRect(0, 15, &_SETUP_RectAttr);

    _SETUP_StrAttr.bBackground = OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER;
    _SETUP_StrAttr.bTextColor = OSDSETUP_ENTRY_COLOR_WHITE;
    _SETUP_StrAttr.bShadow1 = OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER;
//    GDI_DrawString_909(0, &_SETUP_StrAttr, aSetupSelect[bOSDLanguage]);
    _SETUP_DrawDynamicString(aSetupSelect);
}

#ifndef REMOVE_SETUP_SPEAKER_TEST
void _SETUP_ShowSpeaker(BYTE bLoc)
{
#define SPK_ORG_X       (116L+SETUP_GLOBAL_OFFSET_X)
#define SPK_ORG_Y       212L

#define SPK_L_X         (SPK_ORG_X + 35)   
#define SPK_L_Y         (SPK_ORG_Y + 10)   
#define SPK_L_W         25
#define SPK_L_H         70 
#define SPK_R_X         (SPK_L_X+ 120)   
#define SPK_R_Y         (SPK_L_Y)   

#define SPK_C_X         (SPK_ORG_X + 75)   
#define SPK_C_Y         (SPK_ORG_Y + 15)   
#define SPK_C_W         53 
#define SPK_C_H         20 

#define SPK_LS_X        (SPK_ORG_X + 15)   
#define SPK_LS_Y        (SPK_ORG_Y + 90)   
#define SPK_LS_W        25
#define SPK_LS_H        40 
#define SPK_RS_X        (SPK_LS_X + 164)   
#define SPK_RS_Y        (SPK_LS_Y)   

#define SPK_SW_X        (SPK_ORG_X + 58)   
#define SPK_SW_Y        (SPK_ORG_Y + 55)   
#define SPK_SW_W        18
#define SPK_SW_H        28 

    PARM_DRAW_BITMAP _SETUP_BitmapAttr;

    if (SPEAKER_CLEAR == bLoc) 
    {
        _SETUP_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
        _SETUP_FillRect(SPK_ORG_X, SPK_ORG_Y, SPK_ORG_X + 220, SPK_ORG_Y + 140); //CoCo1.20, reduce code size.
        return;
    }

    _SETUP_BitmapAttr.bBitmapID = GDI_BMP_SPEAKER;
    _SETUP_BitmapAttr.bDestRegion = 0;
    _SETUP_BitmapAttr.wDestX = SPK_ORG_X;
    _SETUP_BitmapAttr.wDestY = SPK_ORG_Y;
    //GDI_DrawBitmap(&_SETUP_BitmapAttr);
    // Brian, support GDI library
    GDI_DrawBitmap(&_SETUP_BitmapAttr, _aSetupBMPArray);

    _SETUP_FrameAttr.bStyle = FRAME_POP;
    _SETUP_FrameAttr.bThickness = FRAME_THICKNESS;
    _SETUP_FrameAttr.bColorBright = OSDSETUP_ENTRY_SPEAKER_HIGHLIGHT;
    _SETUP_FrameAttr.bColorDark   = OSDSETUP_ENTRY_SPEAKER_HIGHLIGHT;
    _SETUP_FrameAttr.bColorCenter = OSDSETUP_ENTRY_SPEAKER_HIGHLIGHT;

    if ((SPEAKER_L & bLoc) && (SPEAKER_HIGHLIGHT & bLoc))
    {
        _SETUP_DrawFrame(SPK_L_X, SPK_L_Y,
               (SPK_L_X + SPK_L_W), (SPK_L_Y + SPK_L_H)); //CoCo1.20, reduce code size.
    }

    if ((SPEAKER_R & bLoc) && (SPEAKER_HIGHLIGHT & bLoc))
    {
        _SETUP_DrawFrame(SPK_R_X, SPK_R_Y,
               (SPK_R_X + SPK_L_W), (SPK_R_Y + SPK_L_H)); //CoCo1.20, reduce code size.
    }

    if ((SPEAKER_C & bLoc) && (SPEAKER_HIGHLIGHT & bLoc))
    {
        _SETUP_DrawFrame(SPK_C_X, SPK_C_Y,  //CoCo1.20, reduce code size.
               (SPK_C_X + SPK_C_W), (SPK_C_Y + SPK_C_H));
    }

    if ((SPEAKER_LS & bLoc) && (SPEAKER_HIGHLIGHT & bLoc))
    {
        _SETUP_DrawFrame(SPK_LS_X, SPK_LS_Y,
               (SPK_LS_X + SPK_LS_W), (SPK_LS_Y + SPK_LS_H)); //CoCo1.20, reduce code size.
    }

    if ((SPEAKER_RS & bLoc) && (SPEAKER_HIGHLIGHT & bLoc))
    {
        _SETUP_DrawFrame(SPK_RS_X, SPK_RS_Y,  //CoCo1.20, reduce code size.
               (SPK_RS_X + SPK_LS_W), (SPK_RS_Y + SPK_LS_H));
    }

    if ((SPEAKER_SW & bLoc) && (SPEAKER_HIGHLIGHT & bLoc))
    {
        _SETUP_DrawFrame(SPK_SW_X, SPK_SW_Y,  //CoCo1.20, reduce code size.
               (SPK_SW_X + SPK_SW_W), (SPK_SW_Y + SPK_SW_H));
    }
}
#endif //--CoCo1.20, reduce code size.

void _TranslateString(BYTE* pbAsciiString)
{
    BYTE bTemp = 0;

    while ('\0' != pbAsciiString[bTemp])
    {
        _wSetupString[bTemp+1] = OSDUTL_TransformChar(pbAsciiString[bTemp]); //CoCo1.20, reduce code size.
        bTemp++;
    }
    _wSetupString[0] = bTemp;
}

/*
BYTE _GetDebugIndex(void)
{
    for (_bSetupTemp = 0; _bSetupTemp < MAX_DBG_MODE; _bSetupTemp++)
    {
        if (aDebugMode[_bSetupTemp] == __bDebugMode)
            break;
    }
    return _bSetupTemp;
}
*/

void _SETUP_InitPalette(void)
{
    if (SETUP_CUSTOM_UI_STYLE_1 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_TOP_BANNER,
            OSDSETUP_VALUE_COLOR_TOP_BANNER1, TRUE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_TOP_BANNER_TEXT,
            OSDSETUP_VALUE_COLOR_TOP_BANNER_TEXT1, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER,
            OSDSETUP_VALUE_COLOR_BOTTOM_BANNER1, TRUE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER_TEXT,
            OSDSETUP_VALUE_COLOR_BOTTOM_BANNER_TEXT1, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_CENTER_COLOR_BG,
            OSDSETUP_VALUE_CENTER_COLOR_BG1, TRUE);
    }
    else if (SETUP_CUSTOM_UI_STYLE_2 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_TOP_BANNER,
            OSDSETUP_VALUE_COLOR_TOP_BANNER2, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_TOP_BANNER_TEXT,
            OSDSETUP_VALUE_COLOR_TOP_BANNER_TEXT2, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER,
            OSDSETUP_VALUE_COLOR_BOTTOM_BANNER2, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER_TEXT,
            OSDSETUP_VALUE_COLOR_BOTTOM_BANNER_TEXT2, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_CENTER_COLOR_BG,
            OSDSETUP_VALUE_CENTER_COLOR_BG2, TRUE);
    }
    else if (SETUP_CUSTOM_UI_STYLE_3 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_TOP_BANNER,
            OSDSETUP_VALUE_COLOR_TOP_BANNER3, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_TOP_BANNER_TEXT,
            OSDSETUP_VALUE_COLOR_TOP_BANNER_TEXT3, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER,
            OSDSETUP_VALUE_COLOR_BOTTOM_BANNER3, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER_TEXT,
            OSDSETUP_VALUE_COLOR_BOTTOM_BANNER_TEXT3, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_CENTER_COLOR_BG,
            OSDSETUP_VALUE_CENTER_COLOR_BG3, TRUE);
    }
    else if (SETUP_CUSTOM_UI_STYLE_4 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_TOP_BANNER,
            OSDSETUP_VALUE_COLOR_TOP_BANNER4, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_TOP_BANNER_TEXT,
            OSDSETUP_VALUE_COLOR_TOP_BANNER_TEXT4, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER,
            OSDSETUP_VALUE_COLOR_BOTTOM_BANNER4, FALSE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_BOTTOM_BANNER_TEXT,
            OSDSETUP_VALUE_COLOR_BOTTOM_BANNER_TEXT4, TRUE);
        GDI_ChangePALEntry(OSDSETUP_ENTRY_CENTER_COLOR_BG,
            OSDSETUP_VALUE_CENTER_COLOR_BG4, TRUE);
    }

    GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_WHITE, OSDSETUP_VALUE_COLOR_WHITE, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_COLOR_ORANGE, OSDSETUP_VALUE_COLOR_ORANGE, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_FRAME_BRIGHT, OSDSETUP_VALUE_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_FRAME_DARK, OSDSETUP_VALUE_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_FRAME_CENTER, OSDSETUP_VALUE_FRAME_CENTER, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_HELP_COLOR_BG, OSDSETUP_VALUE_HELP_COLOR_BG, TRUE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_COLOR_BG, OSDSETUP_VALUE_ICON_COLOR_BG, TRUE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_SPEAKER_HIGHLIGHT, OSDSETUP_VALUE_SPEAKER_HIGHLIGHT, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_RECT, OSDSETUP_VALUE_ICON_RECT, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_NORMAL, OSDSETUP_VALUE_ICON_NORMAL, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_OVER, OSDSETUP_VALUE_ICON_OVER, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_DOWN, OSDSETUP_VALUE_ICON_DOWN, FALSE);
#ifdef REMOVE_SETUP_ICON
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_COLOR_ORANGE, OSDSETUP_VALUE_ICON_ORANGE, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_COLOR_BLUE, OSDSETUP_VALUE_ICON_BLUE, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_COLOR_RED, OSDSETUP_VALUE_ICON_RED, FALSE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_COLOR_GREEN, OSDSETUP_VALUE_ICON_GREEN, FALSE);
#endif

    GDI_WaitPaletteComplete();
}

//CoCo1.20, reduce code size.
void _SETUP_FillRect(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom)
{
    _InitRect(&(_SETUP_RectAttr.rect), wLeft, wTop, wRight, wBottom);    
    GDI_FillRect_909(0, &_SETUP_RectAttr);
}

//CoCo1.20, reduce code size.
void _SETUP_DrawFrame(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom)
{
    _InitRect(&(_SETUP_FrameAttr.rect), wLeft, wTop, wRight, wBottom);    
    GDI_DrawFrame_909(0, &_SETUP_FrameAttr);
}

//CoCo1.20, reduce code size.
void _SETUP_DrawString(void)
{
    _SETUP_StrAttr.bColorKey = 0; //For CT909, color key should be 0 for IC's bug
    GDI_DrawString_909(0, &_SETUP_StrAttr, _wSetupString);
}

//CoCo1.20, reduce code size.
void _SETUP_DrawDynamicString(WORD *pwString)
{
    GDI_DrawString_909(0, &_SETUP_StrAttr, pwString);
}

/* Alan2.22, remove it and use CC_ForceStop(ALL_CASE_FORCE_STOP) instead
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
void _SETUP_EnterByPrestop(void)
{
    __bPreCountStop = __bCountStop;

    if ( (!(__wDiscType & CDROM_M1)
//LJY2.39a, don't support stop/resume for CDG
#ifndef NO_CDG
        &&(!(__bCDDA_DTS & CDDA_CDG))
#endif
         ) || (__bAttrPlay & TYPE_CDROM_AV)
       )
       {
           if (
#ifndef NO_DIGEST
               __btDIGEST||
#endif
               __btPlaying || __btPause ||(__btPBC && (__wPlayItem>99)))
           {
               if ((__bModeCmd!=KEY_PLAY) || (__btPause))
               {
                   OSD_OUTPUT_MACRO(MSG_PLAYING, 0, 0); //xuli1.26, clear right-upper corner OSD, e.x. PAUSE/SCF/SCB/FF, or else it will be recover when exit SETUP
               }
               CC_DoKeyAction(KEY_STOP);
#ifndef NO_MP3
               if ( !(__wDiscType & CDROM_M1) )
#endif
               {
                   UTL_ShowLogo();
               }
           }
       }
       else if(__btPlaying)
           CC_DoKeyAction (KEY_PAUSE);
}
*/

/* Alan2.22, remove it and use CC_ForceResumeRePlay() instead
void _SETUP_ExitByResume(void)
{
    extern BIT __btInputProgram;

    if (__bPreCountStop==0) //Need resume
    {
//LJY2.39a, don't support stop/resume for CDG
        if ( (!(__wDiscType & CDROM_M1)
#ifndef NO_CDG
            &&(!(__bCDDA_DTS & CDDA_CDG))
#endif
             ) || (__bAttrPlay & TYPE_CDROM_AV))
        {
            UTL_PreStartServo( __dwTimeNow, ACTION_CONTINUE);
            // CDROM_AV : Playing -> Program Mode -> Setup Mode -> Exit -> Should not do resume
            if (!((__bAttrPlay & TYPE_CDROM_AV) && (__bMMMenuID == MM_MENU_EDITPROG)))
            CC_DoKeyAction(KEY_RESUME);
        }
        else if (__btPause)
        {
            if (!__btInputProgram)
                CC_DoKeyAction(KEY_PAUSE);
        }
    }
}
#endif // #ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
*/

void SETUP_LoadLanguage(void)
{
    PSECTION_ENTRY pSection;
    pSection = ROMLD_GetSectionByName(0, "Engl");

    switch (__SetupInfo.bOSDLanguage)
    {
#if (OSD_LANGUAGE_GROUP == CHINA_AREA_OSD)
 #if (CHINESE_OSD_MODE == SIMPLE_CHINESE)
    case SETUP_LANGUAGE_OSD_SIMP_CHINESE: // Simplified Chinese
        ROMLD_LoadSectionToByName(0, "Chin", pSection->dwLMA, 0);
        break;
 #elif (CHINESE_OSD_MODE == TRADITIONAL_CHINESE)
    case SETUP_LANGUAGE_OSD_TRAD_CHINESE: // Traditional Chinese
        ROMLD_LoadSectionToByName(0, "Chin", pSection->dwLMA, 0);
        break;
 #endif
#elif (OSD_LANGUAGE_GROUP == EUROPE_AREA_OSD)
    case SETUP_LANGUAGE_OSD_FRENCH:
        ROMLD_LoadSectionToByName(0, "Fren", pSection->dwLMA, 0);
        break;
    case SETUP_LANGUAGE_OSD_GERMAN:
        ROMLD_LoadSectionToByName(0, "Germ", pSection->dwLMA, 0);
        break;
    case SETUP_LANGUAGE_OSD_ITALIAN:
        ROMLD_LoadSectionToByName(0, "Ital", pSection->dwLMA, 0);
        break;
    case SETUP_LANGUAGE_OSD_SPANISH:
        ROMLD_LoadSectionToByName(0, "Span", pSection->dwLMA, 0);
        break;
    case SETUP_LANGUAGE_OSD_PORTUGUESE:
        ROMLD_LoadSectionToByName(0, "Port", pSection->dwLMA, 0);
        break;
    case SETUP_LANGUAGE_OSD_DUTCH:
        ROMLD_LoadSectionToByName(0, "Dutc", pSection->dwLMA, 0);
        break;
    case SETUP_LANGUAGE_OSD_POLISH:
        ROMLD_LoadSectionToByName(0, "Poli", pSection->dwLMA, 0);
        break;
#elif (OSD_LANGUAGE_GROUP == JAPAN_AREA_OSD) 
    case SETUP_LANGUAGE_OSD_JAPANESE:
        ROMLD_LoadSectionToByName(0, "Japa", pSection->dwLMA, 0);
        break;
#elif (OSD_LANGUAGE_GROUP == KOREAN_AREA_OSD) 
    case SETUP_LANGUAGE_OSD_KOREAN:
        ROMLD_LoadSectionToByName(0, "Kore", pSection->dwLMA, 0);
        break;
#endif
    case SETUP_LANGUAGE_OSD_ENGLISH:
    default:
        ROMLD_LoadSectionToByName(0, "Engl", pSection->dwLMA, 0);
        break;
    }
}

#endif // #ifdef SUPPORT_STB
#endif //#ifndef IMAGE_FRAME_SETUP //CoCo2.38
