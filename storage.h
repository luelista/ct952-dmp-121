
#ifndef __STORAGE__H__
#define __STORAGE__H__

#include "input.h"
#include "utl.h"
#include "oswrap.h"
#include "notransl.h"
#include "srcfilter.h"

#define WRITE_EEPROM_ENABLE

// Following define the data allocation in EEPROM
// 1st line means the starting address in EEPROM
// 2nd line means the ending address in EEPROM

// SETUP Address   (Total 51 Bytes)
// Currently it uses 48 bytes, the first 37 bytes is DVD only.
#define SETUP_ADDR_BASE         0x00
#define SETUP_ADDR_LAST         0x30

// STB Address     (Total 71 Bytes)    // Share 12 btyes with Setup
// System uses 12 bytes. STB uses the rest of 59 bytes, 
#define STB_ADDR_BASE         0x25
#define STB_ADDR_LAST         0x6B 

// DivX Address    (Total 54 Bytes)
// Currently it use 54 bytes.
#define SETUP_ADDR_DIVX_BASE         0x6C 
#define SETUP_ADDR_DIVX_LAST         0xA1

// RESUME Address  (Total 76 Bytes)
// Currently it use 124 bytes.
// Notice: Resume space is 24N+4
// wyc2.51-909P, storage information for file list in JPG encode.
#ifndef SUPPORT_ENCODE_JPG_PICTURE
#define SETUP_ADDR_RESUME_BASE  0xA2
#define SETUP_ADDR_RESUME_LAST  0xED

//TFT Address      (Total 3 Bytes)
// Currently it uses 3 bytes
#define TFT_ADDR_BASE           0xEE
#define TFT_ADDR_LAST           0xF0
#else
#define SETUP_ADDR_JPG_ENCODE_BASE  (0xA2)
#define SETUP_ADDR_JPG_ENCODE_LAST  (0xA2 + SRCFTR_SUPPORT_JPG_ENCODE_NUM*2+2-1)

//TFT Address      (Total 3 Bytes)
// Currently it uses 3 bytes
#define TFT_ADDR_BASE           (SETUP_ADDR_JPG_ENCODE_LAST+1)
#define TFT_ADDR_LAST           (TFT_ADDR_BASE+3-1)
#endif //


// Available Address (Total 7 Bytes)
// The availab space is from 0xF1 - 0xF7 

// Game Address (Total 8 Bytes)
// Notice: Reserve the address from 0xF8 to 0xFF for OSDGame.

// DMP_116, use new address to save image frame setup, the new address is exceed 256
// need to change ROM_BANK_SIZE as 512 (hdevice.h)

// Image Frame Setup Address   (Total 20 Bytes)
// Currently it uses 20 bytes.
#define IMAGE_FRAME_SETUP_ADDR_BASE		0x100
#define IMAGE_FRAME_SETUP_ADDR_LAST		0x113

// Radio Address   (Total 42 Bytes)
// Currently it uses 42 bytes.
#ifdef CT950_STYLE
#define RADIO_ADDR_BASE		0x114
#define RADIO_ADDR_LAST		(RADIO_ADDR_BASE+2*RADIO_FAVORITE_NUM+2-1)
#endif //

//////////////////////////////////////////////////////////////////////


//Default values
#define SETUP_PASSWORD_NO           4
#define SETUP_DEFAULT_PASSWORD      8888

#define SUPPORT_GOLDEN_PASSWORD
#define GOLDEN_PASSWORD             9922


//Other definitions
#define SETUP_VERSION   0x01        // FAE use only
//#define INTER_VERSION   0x0C        // J200 use only
#define INTER_VERSION   0x0D        // J200 use only


#define SETUP_DATA_RESET        1
#define SETUP_DATA_DEFAULT      2
#define SETUP_DATA_CURRENT      3

// This is for BACKDOOR "6666"
#define READ_TOTAL_DEFAULT      0xFF    // set all bit to 1 means set all to deafult value.


// Following define the data allocation in EEPROM
// 1st line means EEPROM address
// 2nd line means the bytes needed
// Setup moude use 48 bytes currently


// split Identify into 2 fields, ID+VERSION
#define SETUP_ADDR_IDENTIFY     (SETUP_ADDR_BASE)
#define EPROM_IDENTIFY_BYTE     2 

#define SETUP_ADDR_VERSION      (SETUP_ADDR_IDENTIFY + EPROM_IDENTIFY_BYTE)
#define EPROM_VERSION_BYTE      1
#define SETUP_ADDR_INTERVERSION (SETUP_ADDR_VERSION + EPROM_VERSION_BYTE)
#define EPROM_INTERVERSION_BYTE 1

#define SETUP_ADDR_AUDIO_LANGUAGE   (SETUP_ADDR_INTERVERSION + EPROM_INTERVERSION_BYTE)
#define EPROM_AUDIO_LANGUAGE_BYTE   1

#define SETUP_ADDR_SPST_LANGUAGE    (SETUP_ADDR_AUDIO_LANGUAGE + EPROM_AUDIO_LANGUAGE_BYTE)
#define EPROM_SPST_LANGUAGE_BYTE    1

#define SETUP_ADDR_SCREEN_SAVER (SETUP_ADDR_SPST_LANGUAGE + EPROM_SPST_LANGUAGE_BYTE)
#define EPROM_SCREEN_SAVER_BYTE 1

#define SETUP_ADDR_SCAN_MODE    (SETUP_ADDR_SCREEN_SAVER + EPROM_SCREEN_SAVER_BYTE)
#define EPROM_SCAN_MODE_BYTE    1

// Brian1.25, Chroma phase
#define SETUP_ADDR_CHROM_PHASE (SETUP_ADDR_SCAN_MODE + EPROM_SCAN_MODE_BYTE)
#define EPROM_CHROM_PHASE_BYTE 	1

//Kevin0.87b, cross out later (consider all modules that use __SetupInfo.bAudioOutput)
#define SETUP_ADDR_AUDIO_OUTPUT (SETUP_ADDR_CHROM_PHASE + EPROM_CHROM_PHASE_BYTE)
#define EPROM_AUDIO_OUTPUT_BYTE 1

#define SETUP_ADDR_AUDIO_DIGITAL_OUT (SETUP_ADDR_AUDIO_OUTPUT + EPROM_AUDIO_OUTPUT_BYTE)
#define EPROM_AUDIO_DIGITAL_OUT_BYTE 1

#define SETUP_ADDR_AUDIO_ANALOG_OUT (SETUP_ADDR_AUDIO_DIGITAL_OUT + EPROM_AUDIO_DIGITAL_OUT_BYTE)
#define EPROM_AUDIO_ANALOG_OUT_BYTE 1

#define SETUP_ADDR_AUDIO_LPCM   (SETUP_ADDR_AUDIO_ANALOG_OUT + EPROM_AUDIO_ANALOG_OUT_BYTE)
#define EPROM_AUDIO_LPCM_BYTE   1

#define SETUP_ADDR_AUDIO_DOWNMIX        (SETUP_ADDR_AUDIO_LPCM + EPROM_AUDIO_LPCM_BYTE)
#define EPROM_AUDIO_DOWNMIX_BYTE        1 

#define SETUP_ADDR_AUDIO_PROLOGIC       (SETUP_ADDR_AUDIO_DOWNMIX + EPROM_AUDIO_DOWNMIX_BYTE)
#define EPROM_AUDIO_PROLOGIC_BYTE       1  

#define SETUP_ADDR_REGION_CTL   (SETUP_ADDR_AUDIO_PROLOGIC + EPROM_AUDIO_PROLOGIC_BYTE)
#define EPROM_REGION_CTL_BYTE   1

#define SETUP_ADDR_APS_CTL      (SETUP_ADDR_REGION_CTL + EPROM_REGION_CTL_BYTE)
#define EPROM_APS_CTL_BYTE      1

#define SETUP_ADDR_PARENTAL_CTL_LEVEL   (SETUP_ADDR_APS_CTL + EPROM_APS_CTL_BYTE)
#define EPROM_PARENTAL_CTL_LEVEL_BYTE 1 

#define SETUP_ADDR_AUDIO_CENTER     (SETUP_ADDR_PARENTAL_CTL_LEVEL + EPROM_PARENTAL_CTL_LEVEL_BYTE)
#define EPROM_AUDIO_CENTER_BYTE     1
#define SETUP_ADDR_AUDIO_REAR       (SETUP_ADDR_AUDIO_CENTER + EPROM_AUDIO_CENTER_BYTE)
#define EPROM_AUDIO_REAR_BYTE       1
#define SETUP_ADDR_AUDIO_SUBWOOFER  (SETUP_ADDR_AUDIO_REAR + EPROM_AUDIO_REAR_BYTE)
#define EPROM_AUDIO_SUBWOOFER_BYTE  1
#define SETUP_ADDR_AUDIO_DNR        (SETUP_ADDR_AUDIO_SUBWOOFER + EPROM_AUDIO_SUBWOOFER_BYTE)
#define EPROM_AUDIO_DNR_BYTE        1
#define SETUP_ADDR_AUDIO_KARAOKE    (SETUP_ADDR_AUDIO_DNR + EPROM_AUDIO_DNR_BYTE)
#define EPROM_AUDIO_KARAOKE_BYTE    1
#define SETUP_ADDR_PARENTAL_CTL     (SETUP_ADDR_AUDIO_KARAOKE + EPROM_AUDIO_KARAOKE_BYTE)
#define EPROM_PARENTAL_CTL_BYTE     1
#define SETUP_ADDR_MENU_LANGUAGE    (SETUP_ADDR_PARENTAL_CTL + EPROM_PARENTAL_CTL_BYTE)
#define EPROM_MENU_LANGUAGE_BYTE    1

#define SETUP_ADDR_EX_SPST_LANGUAGE (SETUP_ADDR_MENU_LANGUAGE + EPROM_MENU_LANGUAGE_BYTE)
#define EPROM_EX_SPST_LANGUAGE_BYTE    1

#define SETUP_ADDR_MENU_LANGUAGE_CODE   (SETUP_ADDR_EX_SPST_LANGUAGE + EPROM_EX_SPST_LANGUAGE_BYTE)
#define EPROM_MENU_LANGUAGE_CODE_BYTE   2
#define SETUP_ADDR_AUDIO_LANGUAGE_CODE  (SETUP_ADDR_MENU_LANGUAGE_CODE + EPROM_MENU_LANGUAGE_CODE_BYTE)
#define EPROM_AUDIO_LANGUAGE_CODE_BYTE  2

#define SETUP_ADDR_SPST_LANGUAGE_CODE   (SETUP_ADDR_AUDIO_LANGUAGE_CODE + EPROM_AUDIO_LANGUAGE_CODE_BYTE)
#define EPROM_SPST_LANGUAGE_CODE_BYTE   2

#define SETUP_ADDR_SMART_PLAY           (SETUP_ADDR_SPST_LANGUAGE_CODE + EPROM_SPST_LANGUAGE_CODE_BYTE)
#define EPROM_SMART_PLAY_BYTE           1

#define SETUP_ADDR_ANGLE_MARK_DISPLAY   (SETUP_ADDR_SMART_PLAY + EPROM_SMART_PLAY_BYTE)
#define EPROM_ANGLE_MARK_DISPLAY_BYTE   1

#define SETUP_ADDR_VCD_CTL              (SETUP_ADDR_ANGLE_MARK_DISPLAY + EPROM_ANGLE_MARK_DISPLAY_BYTE) 
#define EPROM_VCD_CTL_BYTE              1 

#define SETUP_ADDR_DEBUG_MODE           (SETUP_ADDR_VCD_CTL + EPROM_VCD_CTL_BYTE) 
#define EPROM_DEBUG_MODE_BYTE           1 

#define SETUP_ADDR_AP_MODE              (SETUP_ADDR_DEBUG_MODE + EPROM_DEBUG_MODE_BYTE) 
#define EPROM_AP_MODE_BYTE              1 


// elmer2.78, support burn-in testing
// add 0x10 to make sure that this position is available
#ifdef SUPPORT_WATCHDOG_BURNIN
#define SETUP_ADDR_WATCHDOG             (SETUP_ADDR_AP_MODE + EPROM_AP_MODE_BYTE + 0x10)
#define EPROM_WATCHDOG_BYTE             1

#define SETUP_ADDR_NVPLAYMODE           (SETUP_ADDR_WATCHDOG + EPROM_WATCHDOG_BYTE)
#define EPROM_NVPLAYMODE_BYTE           1

#define SETUP_ADDR_REPEATMODE           (SETUP_ADDR_NVPLAYMODE + EPROM_NVPLAYMODE_BYTE)
#define EPROM_REPEATMODE_BYTE           1
#endif
//


//CoCo2.38, the image frame SETUP will occpy the same position as DVD SETUP but replace some unused DVD address
#ifdef IMAGE_FRAME_SETUP
/*
#define SETUP_ADDR_PHOTO_SETTING_INTERVAL_TIME      SETUP_ADDR_AUDIO_LANGUAGE
#define SETUP_ADDR_PHOTO_SETTING_TRANSITION_EFFECT  SETUP_ADDR_SPST_LANGUAGE
#define SETUP_ADDR_PHOTO_SETTING_BROWSE             SETUP_ADDR_MENU_LANGUAGE    
#define SETUP_ADDR_AUTO_PLAY_PHOTO                  SETUP_ADDR_AUDIO_KARAOKE
#define SETUP_ADDR_AUTO_PLAY_MUSIC                  SETUP_ADDR_AUDIO_DNR
#define SETUP_ADDR_AUTO_PLAY_PHOTO_PLUS_MUSIC       SETUP_ADDR_AUDIO_LPCM
#define SETUP_ADDR_AUTO_PLAY_MOVIE                  SETUP_ADDR_VCD_CTL
#define SETUP_ADDR_DISPLAY_TYPE                     SETUP_ADDR_REGION_CTL

#define SETUP_ADDR_ALARM_ENABLE                     SETUP_ADDR_AUDIO_CENTER
#define SETUP_ADDR_AUTO_POWER_ON_ENABLE             SETUP_ADDR_AUDIO_REAR
#define SETUP_ADDR_AUTO_POWER_OFF_ENABLE            SETUP_ADDR_AUDIO_SUBWOOFER
#define SETUP_ADDR_ALARM_TIME                       SETUP_ADDR_MENU_LANGUAGE_CODE
#define SETUP_ADDR_AUTO_POWER_ON_TIME               SETUP_ADDR_AUDIO_LANGUAGE_CODE
#define SETUP_ADDR_AUTO_POWER_OFF_TIME              SETUP_ADDR_SPST_LANGUAGE_CODE

//DMP_107, add the address to save volumn
#define SETUP_ADDR_VOLUME                           SETUP_ADDR_SMART_PLAY //&SETUP_ADDR_ANGLE_MARK_DISPLAY
*/

// DMP_116, use new address to save image frame setup
#define SETUP_ADDR_PHOTO_SETTING_INTERVAL_TIME	(IMAGE_FRAME_SETUP_ADDR_BASE)
#define EPROM_PHOTO_SETTING_INTERVAL_TIME_BYTE	1

#define SETUP_ADDR_PHOTO_SETTING_TRANSITION_EFFECT	(SETUP_ADDR_PHOTO_SETTING_INTERVAL_TIME + EPROM_PHOTO_SETTING_INTERVAL_TIME_BYTE)
#define EPROM_PHOTO_SETTING_TRANSITION_EFFECT_BYTE	1

#define SETUP_ADDR_PHOTO_SETTING_BROWSE		(SETUP_ADDR_PHOTO_SETTING_TRANSITION_EFFECT + EPROM_PHOTO_SETTING_TRANSITION_EFFECT_BYTE)	    
#define EPROM_PHOTO_SETTING_BROWSE_BYTE		1

#define SETUP_ADDR_AUTO_PLAY_PHOTO	(SETUP_ADDR_PHOTO_SETTING_BROWSE + EPROM_PHOTO_SETTING_BROWSE_BYTE)
#define EPROM_AUTO_PLAY_PHOTO_BYTE	1

#define SETUP_ADDR_AUTO_PLAY_MUSIC	(SETUP_ADDR_AUTO_PLAY_PHOTO + EPROM_AUTO_PLAY_PHOTO_BYTE)
#define EPROM_AUTO_PLAY_MUSIC_BYTE	1

#define SETUP_ADDR_AUTO_PLAY_PHOTO_PLUS_MUSIC	(SETUP_ADDR_AUTO_PLAY_MUSIC + EPROM_AUTO_PLAY_MUSIC_BYTE)
#define EPROM_AUTO_PLAY_PHOTO_PLUS_MUSIC_BYTE	1

#define SETUP_ADDR_AUTO_PLAY_MOVIE	(SETUP_ADDR_AUTO_PLAY_PHOTO_PLUS_MUSIC + EPROM_AUTO_PLAY_PHOTO_PLUS_MUSIC_BYTE)
#define EPROM_AUTO_PLAY_MOVIE_BYTE	1

#define SETUP_ADDR_DISPLAY_TYPE	(SETUP_ADDR_AUTO_PLAY_MOVIE + EPROM_AUTO_PLAY_MOVIE_BYTE)              
#define EPROM_DISPLAY_TYPE_BYTE	1

//Damon1.12, add the address to match setup info. In fact, it's not used 
#define SETUP_ADDR_REPEAT_MODE	(SETUP_ADDR_DISPLAY_TYPE + EPROM_DISPLAY_TYPE_BYTE)              
#define EPROM_REPEAT_MODE_BYTE	1

#define SETUP_ADDR_ALARM_ENABLE	(SETUP_ADDR_REPEAT_MODE + EPROM_REPEAT_MODE_BYTE)                    
#define EPROM_ALARM_ENABLE_BYTE	1

#define SETUP_ADDR_AUTO_POWER_ON_ENABLE	(SETUP_ADDR_ALARM_ENABLE + EPROM_ALARM_ENABLE_BYTE)             
#define EPROM_AUTO_POWER_ON_ENABLE_BYTE	1

#define SETUP_ADDR_AUTO_POWER_OFF_ENABLE	(SETUP_ADDR_AUTO_POWER_ON_ENABLE + EPROM_AUTO_POWER_ON_ENABLE_BYTE)          
#define EPROM_AUTO_POWER_OFF_ENABLE_BYTE	1

#define SETUP_ADDR_ALARM_TIME		(SETUP_ADDR_AUTO_POWER_OFF_ENABLE + EPROM_AUTO_POWER_OFF_ENABLE_BYTE)      
#define EPROM_ALARM_TIME_BYTE		2

#define SETUP_ADDR_AUTO_POWER_ON_TIME	(SETUP_ADDR_ALARM_TIME + EPROM_ALARM_TIME_BYTE)              
#define EPROM_AUTO_POWER_ON_TIME_BYTE	2

#define SETUP_ADDR_AUTO_POWER_OFF_TIME	(SETUP_ADDR_AUTO_POWER_ON_TIME + EPROM_AUTO_POWER_ON_TIME_BYTE)             
#define EPROM_AUTO_POWER_OFF_TIME_BYTE	2

//DMP_107, add the address to save volumn
#define SETUP_ADDR_VOLUME		(SETUP_ADDR_AUTO_POWER_OFF_TIME + EPROM_AUTO_POWER_OFF_TIME_BYTE)
#define EPROM_VOLUME_BYTE		2
#endif  //IMAGE_FRAME_SETUP

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
// The following 11 bytes are shared with STB. Please don't changed the address.

#define SETUP_ADDR_STB_MAGIC_COUNT_HEAD (SETUP_ADDR_AP_MODE + EPROM_AP_MODE_BYTE)
#define EEPROM_STB_MAGIC_COUNT_HEAD     1

#define SETUP_ADDR_UI_STYLE     (SETUP_ADDR_STB_MAGIC_COUNT_HEAD + EEPROM_STB_MAGIC_COUNT_HEAD)
#define EPROM_UI_STYLE          1 

#define SETUP_ADDR_BRIGHTNESS   (SETUP_ADDR_UI_STYLE + EPROM_UI_STYLE)
#define EPROM_BRIGHTNESS_BYTE   1

#define SETUP_ADDR_CONTRAST     (SETUP_ADDR_BRIGHTNESS + EPROM_BRIGHTNESS_BYTE)
#define EPROM_CONTRAST_BYTE     1

#define SETUP_ADDR_ASPECT       (SETUP_ADDR_CONTRAST + EPROM_CONTRAST_BYTE)
#define EPROM_ASPECT_BYTE       1

#define SETUP_ADDR_TV_SYTEM     (SETUP_ADDR_ASPECT + EPROM_ASPECT_BYTE)
#define EPROM_TV_SYSTEM_BYTE    1

#define SETUP_ADDR_VIDEO_OUTPUT (SETUP_ADDR_TV_SYTEM + EPROM_TV_SYSTEM_BYTE)
#define EPROM_VIDEO_OUTPUT_BYTE 1

#define SETUP_ADDR_OSD_LANGUAGE (SETUP_ADDR_VIDEO_OUTPUT + EPROM_VIDEO_OUTPUT_BYTE)
#define EPROM_OSD_LANGUAGE_BYTE 1

#define SETUP_ADDR_PASSSTRING   (SETUP_ADDR_OSD_LANGUAGE + EPROM_OSD_LANGUAGE_BYTE)
#define EPROM_PASSSTRING_BYTE   2

#define SETUP_ADDR_DEFAULT_TYPE (SETUP_ADDR_PASSSTRING + EPROM_PASSSTRING_BYTE)
#define EPROM_DEFAULT_TYPE_BYTE 1

#define SETUP_ADDR_LAST_MODE    (SETUP_ADDR_DEFAULT_TYPE + EPROM_DEFAULT_TYPE_BYTE)
#define EPROM_DEFAULT_LAST_MODE 1

//----------------------------------------------------------------------------------------

//*************************************************************
//* brian.277a, ADDR_LAST is in fact the next available address
//*************************************************************
#define SETUP_ADDR_USED             (SETUP_ADDR_LAST_MODE + EPROM_DEFAULT_LAST_MODE - 1) 
#define SETUP_DATA_BYTES            (SETUP_ADDR_USED - SETUP_ADDR_BASE + 1)

// Do error checking if total used space is out of the reserved space.
#if (SETUP_ADDR_USED > SETUP_ADDR_LAST)
#error "Exceed the reserved space for Setup Module in the EEPROM space !!"
#endif  // #if (SETUP_ADDR_USED > SETUP_ADDR_LAST)

//*************************************************************

// DMP1.16

#ifdef IMAGE_FRAME_SETUP
#define IMAGE_FRAME_SETUP_ADDR_USED             (SETUP_ADDR_AUTO_POWER_OFF_TIME + EPROM_AUTO_POWER_OFF_TIME_BYTE - 1) 
#define IMAGE_FRAME_SETUP_DATA_BYTES            (IMAGE_FRAME_SETUP_ADDR_USED - IMAGE_FRAME_SETUP_ADDR_BASE + 1)

// Do error checking if total used space is out of the reserved space.
#if (IMAGE_FRAME_SETUP_ADDR_USED > IMAGE_FRAME_SETUP_ADDR_LAST)
#error "Exceed the reserved space for Setup Module in the EEPROM space !!"
#endif  // #if (IMAGE_FRAME_SETUP_ADDR_USED > IMAGE_FRAME_SETUP_ADDR_LAST)
#endif //

//*************************************************************
//Alan1.06 for TFT 
#define TFT_ADDR_BIRHGTNESS           TFT_ADDR_BASE
#define TFT_BRIGHTNESS_BYTE           1
#define TFT_ADDR_CONTRAST             (TFT_ADDR_BIRHGTNESS + TFT_BRIGHTNESS_BYTE)
#define TFT_CONTRAST_BYTE             1
#define TFT_ADDR_SATURATION           (TFT_ADDR_CONTRAST + TFT_CONTRAST_BYTE)
#define TFT_SATURATION_BYTE           1

//*************************************************************
#define TFT_ADDR_USED                 (TFT_ADDR_SATURATION + TFT_SATURATION_BYTE - 1)
#define TFT_DATA_BYTES                (TFT_ADDR_USED - TFT_ADDR_BASE + 1)

// Do error checking if total used space is out of the reserved space.
#if (TFT_ADDR_USED > TFT_ADDR_LAST)
#error "Exceed the reserved space for Setup Module in the EEPROM space !!"
#endif  // #if (TFT_ADDR_USED > TFT_ADDR_LAST)

//*************************************************************
//For Radio
#ifdef CT950_STYLE
#define RADIO_ADDR_BACKUP_FREQ		(RADIO_ADDR_BASE)
#define EPROM_BACKUP_FREQ_BYTE		2

#define RADIO_ADDR_FAVORITE			(RADIO_ADDR_BASE + EPROM_BACKUP_FREQ_BYTE)
#define EPROM_FAVORITE_BYTE			(2*RADIO_FAVORITE_NUM)

#define RADIO_ADDR_USED                 (RADIO_ADDR_FAVORITE + EPROM_FAVORITE_BYTE - 1)
#define RADIO_DATA_BYTES                (RADIO_ADDR_USED - RADIO_ADDR_BASE + 1)

// Do error checking if total used space is out of the reserved space.
#if (RADIO_ADDR_USED > RADIO_ADDR_LAST)
#error "Exceed the reserved space for Radio Module in the EEPROM space !!"
#endif  // #if (RADIO_ADDR_USED > RADIO_ADDR_LAST)
#endif //

// To add one more entry in EEPROM, must allocate
// [USER] (1) EEPROM address, (2) Bytes
// [USER] (3) Add one entry to SetupInfo.
// [USER] (4) EEPROM last address
// add here


// wyc2.51-909P, JPG encode file list information in storage.
#ifndef SUPPORT_ENCODE_JPG_PICTURE
// Ming0.80, Define EEPROM address while save resume into. into EEPROM
#ifdef SUPPORT_RESUME_FROM_EEPROM
#define SETUP_ADDR_RESUME_FLAG      (SETUP_ADDR_RESUME_BASE)
#define RESUME_FLAG_LENGTH          (RESUME_MARK_SIZE)
// LLY1.00, combine the resume ID & information together
// Let it same as the resume information data structure
#define SETUP_ADDR_RESUME_DATA      (SETUP_ADDR_RESUME_FLAG + RESUME_FLAG_LENGTH)
#define RESUME_DATA_LENGTH          ((RESUME_ID_ENTRY_SIZE + RESUME_INFO_ENTRY_SIZE) * (RESUME_DISC_NUM))
#define SETUP_ADDR_RESUME_USED      (SETUP_ADDR_RESUME_DATA + RESUME_DATA_LENGTH - 1)
#endif  // SUPPORT_RESUME_FROM_EEPROM

#else
#define SETUP_ADDR_JPG_ENCODE_MASK          (SETUP_ADDR_JPG_ENCODE_BASE)
#define SETUP_ADDR_JPG_ENCODE_FILE_LIST     (SETUP_ADDR_JPG_ENCODE_MASK+1)
#define SETUP_ADDR_JPG_ENCODE_FILE_ENTRY    (SETUP_ADDR_JPG_ENCODE_FILE_LIST+SRCFTR_SUPPORT_JPG_ENCODE_NUM)
#define SETUP_ADDR_JPG_ENCODE_FILE_STATUS   (SETUP_ADDR_JPG_ENCODE_FILE_ENTRY+1)
#endif //

#define REGION_ALL      0xff  // support all region
#define REGION1         0x01  // support region 1
#define REGION2         0x02  // support region 2
#define REGION3         0x04  // support region 3
#define REGION4         0x08  // support region 4
#define REGION5         0x10  // support region 5
#define REGION6         0x20  // support region 6
#define REGION7         0x40  // support region 7, reserved?
#define REGION8         0x80  // support region 8, reserved


// the data sequence must follow the address sequence.
typedef struct tagSETUPINFO
{
    char    cSetupIdentify[EPROM_IDENTIFY_BYTE];
    BYTE    bSetupVersion;
    BYTE    bInterVersion;       // Please don't modify this field except D300

    // external reference
    BYTE    bAudioLanguage;     //Audio Language
    BYTE    bSPSTLanguage;      //Subtitle Language
    BYTE    bSCRControl;        //Screen Saver
    BYTE    bProgressiveScan;   //Progressive Video Output: On/Off,  Brian0.81, 
    BYTE    bChromPhaseOffset;	//Video hue adjustment	    
    BYTE    bAudioOutput;       //Audio output  //Kevin0.87b, cross out later (consider all modules that use __SetupInfo.bAudioOutput)
    BYTE    bAudioDigitalOut;   //Audio Digital Output
    BYTE    bAudioAnalogOut;    //Audio Analog Output
    BYTE    bAudioLPCM;         //Audio LPCM
    BYTE    bAudioDownMix;      //Audio Analog Output
    BYTE    bAudioPrologic;     // Brian2.78b-2, prologic on/off
    BYTE    bRegionControl;     //Region control
    BYTE    bAPSControl;        //APS control for back door
    BYTE    bParentalCtl;       //Parental control level

    BYTE    bAudioCenter;       //Audio Center
    BYTE    bAudioRear;         //Audio Rear
    BYTE    bAudioSubwoofer;    //Audio Subwoofer
    BYTE    bAudioDNR;          //Audio DNR compression
    BYTE    bAudioKaraoke;      //Audio karaoke
    BYTE    bParentalControl;   //Parental control for back door
    BYTE    bMenuLanguage;      //Menu Language
    BYTE    bExSPSTLanguage;    //External Subtitle Language
    WORD    wMenuLanguageCode;  //Record other menu language code
    WORD    wAudioLanguageCode; //Record other audio language code
    WORD    wSPSTLanguageCode;  //Record other SPST language code
    BYTE    bSmartPlay;         //DVD Smart Playback
    BYTE    bAngleMarkDisplay;  //Angle mark display
    BYTE    bVCDControl;        // ** TCH2.74; "1" mean can;t play VCD-relative title
    BYTE    bDebugMode;         // Record the debug mode
    BYTE    bAPMode;


///////          NOTE !!!    The order should be the same as SYSTEMINFO
    BYTE    bSTBMagiceCountHead;// STB magic count head
    BYTE    bUIStyle;           //UI Style
    BYTE    bBrightness;        //Brightness
    BYTE    bContrast;          //Contrast
    BYTE    bAspectRatio;       //TV type
    BYTE    bTVSystem;          //TV system
    BYTE    bVideoOutput;       //Video Output, no mapping UI
    BYTE    bOSDLanguage;       //OSD Language
    WORD    wDefaultPassword;
    BYTE    bDefaultType;       // DVB only. Default startup mode.
    BYTE    bLastMode;          // DVB only. Last mode. 


    // data are NOT stored in EEPROM
    // brian.170a, as Micky request another variable that actually contains the OSD dispaly time
    // for Angle message
    BYTE    bAngleMarkTime; // 0 when SETUP_CUSTOM_ANGLE_MARK_OFF; 0xFF when SETUP_CUSTOM_ANGLE_MARK_ON
    BYTE    bAudioTestTone;
}   PACK_FORMAT SETUPINFO; 

typedef struct tagIMAGE_FRAME_SETUPINFO
{
    BYTE    bPhotoIntervalTime;
    BYTE    bTransitionEffect;
    BYTE    bBrowse;
    BYTE    bAutoPlayPhoto;
    BYTE    bAutoPlayMusic;
    BYTE    bAutoPlayPhotoPlusMusic;
    BYTE    bAutoPlayMovie;
    BYTE    bDisplayType;
    BYTE    bRepeatMode;
    BYTE    bAlarmEnable;
    BYTE    bAutoPowerOnEnable;
    BYTE    bAutoPowerOffEnable;
    WORD    wAlarmTime;             //high byte==>hour, low byte==>minute
    WORD    wAutoPowerOnTime;       //high byte==>hour, low byte==>minute
    WORD    wAutoPowerOffTime;      //high byte==>hour, low byte==>minute
} PACK_FORMAT IMAGE_FRAME_SETUPINFO;

///////          NOTE !!!    The order should be the same as SETUPINFO
typedef struct tagSYSTEMINFO
{
    BYTE    bSTBMagiceCountHead;// STB magic count head
    BYTE    bUIStyle;           //UI Style
    BYTE    bBrightness;        //Brightness
    BYTE    bContrast;          //Contrast
    BYTE    bAspectRatio;       //TV type
    BYTE    bTVSystem;          //TV system
    BYTE    bVideoOutput;       //Video Output, no mapping UI
    BYTE    bOSDLanguage;       //OSD Language
    WORD    wDefaultPassword;
    BYTE    bDefaultType;       // Default startup mode. DVB only
    BYTE    bLastMode;          // Last mode. DVB only 
}   PACK_FORMAT SYSTEMINFO; 

#ifdef  BACK_DOOR_DECLARE

// Definitions for back door solution //CoCo.043
#define BACK_DOOR_SEQUENCE_NUM  4       //  this is the total sequence number
BYTE    __bBackDoorSeqIndex = 0;        // this is the index to acess the sequence array
BYTE    __bBackDoorSequence[BACK_DOOR_SEQUENCE_NUM] =
            {KEY_VOL_UP, KEY_VOL_DOWN, KEY_VOL_UP, KEY_VOL_DOWN};

#define SETUP_REGION_NUM    7

BYTE    _bDoBackDoor = FALSE;
BYTE    _bDebugSetMode = FALSE;
BYTE    _bBackDoorRegion = 0;
BYTE    _bBackDoorIndex = 0;


#ifndef CT950_STYLE
#ifndef RELEASE_SETTING
#define TOTAL_BACK_DOOR_NUM     2  // debug mode and region
#else
#define TOTAL_BACK_DOOR_NUM     1  // debug mode
#endif
#else //CT950_STYLE
#define TOTAL_BACK_DOOR_NUM     1  //only debug mode
#endif


// Debug mode information
#define MAX_DBG_MODE    12
/*
#ifdef  PIN_216            
BYTE    aDebugMode[MAX_DBG_MODE] = 
        {UTL_DBG_NONE, UTL_DBG_DSU1_EN, UTL_DBG_UART1_EN, (UTL_DBG_DSU1_EN|UTL_DBG_UART1_EN)};
#else  
BYTE    aDebugMode[MAX_DBG_MODE] =
        {UTL_DBG_NONE, UTL_DBG_DSU1_EN, UTL_DBG_UART1_EN, (UTL_DBG_DSU1_EN|UTL_DBG_UART2_EN)};
#endif 
*/
//BYTE    _bDebugIndex;

#define FORMAT_VERSION          0 
#define FORMAT_DATE             1
#define FORMAT_NUMBER           2

/* alan test
//Note: Don't translate the following strings.
//CoCo1.20, reduce code size.
const WORD aSETUPFW[] = {3, CHAR_F, CHAR_SOLIDUS, CHAR_W};
const WORD aSETUPFAE[] = {3, CHAR_F, CHAR_A, CHAR_E};
const WORD aSETUPMPEG[] = {4, CHAR_M, CHAR_P, CHAR_E, CHAR_G};
const WORD aSETUPJPG[] = {3, CHAR_J, CHAR_P, CHAR_G};
const WORD aSETUPDisp[] = {4, CHAR_D, CHAR_i, CHAR_s, CHAR_p};
const WORD aSETUPDivX[] = {4, CHAR_D, CHAR_i, CHAR_v, CHAR_X};
const WORD aSETUPNav[] = {3, CHAR_N, CHAR_a, CHAR_v};
const WORD aSETUPParser[] = {6, CHAR_P, CHAR_a, CHAR_r, CHAR_s, CHAR_e, CHAR_r};
const WORD aSETUPInfo[] = {4, CHAR_I, CHAR_n, CHAR_f, CHAR_o};
const WORD aSETUPAudio[] = {5, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
const WORD aSETUPUSB[] = {3, CHAR_U, CHAR_S, CHAR_B};
const WORD aSETUPASupport[] = {9, CHAR_A, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_u, CHAR_p, CHAR_p, CHAR_o, CHAR_r, CHAR_t};
#ifdef SUPPORT_TFT
const WORD aSETUPTFT[] = {2, CHAR_V, CHAR_D};
#endif

const WORD aSETUPServo[] = {5, CHAR_S, CHAR_e, CHAR_r, CHAR_v, CHAR_o};
const WORD aSETUPBM[] = {2, CHAR_B, CHAR_M};
const WORD aSETUPuCode[] = {5, CHAR_u, CHAR_C, CHAR_o, CHAR_d, CHAR_e};
const WORD aSETUPPCBSYSTEM[] = {10, CHAR_P, CHAR_C, CHAR_B, CHAR_SPACE, CHAR_S, CHAR_Y, CHAR_S, CHAR_T, CHAR_E, CHAR_M};
const WORD aSETUPRFTYPE[] = {7, CHAR_R, CHAR_F, CHAR_SPACE, CHAR_T, CHAR_Y, CHAR_P, CHAR_E};
const WORD aSETUPMOTORTYPE[] = {10, CHAR_M, CHAR_O, CHAR_T, CHAR_O, CHAR_R, CHAR_SPACE, CHAR_T, CHAR_Y, CHAR_P, CHAR_E};
const WORD aSETUPLOADER[] = {6, CHAR_L, CHAR_O, CHAR_A, CHAR_D, CHAR_E, CHAR_R};
*/

#endif  // #ifdef  BACK_DOOR_DECLARE


#ifdef  MAIN_SETUP_DECLARE
char _cLanguageCode[]={'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
char _cTempLanguageCode[2];

BYTE    _bRegionCode[]={REGION_ALL, REGION1, REGION2, REGION3, REGION4, REGION5, REGION6};

//For OSD usage
BYTE    __bCurrentMsg = 0;//MSG_SETUP_MAIN_BASE;
BYTE    __bOSD2FirstShowMsg = TRUE;
BYTE    __bDisableMsg = 0;

SETUPINFO  __SetupInfo;

BYTE    __bItemOrValue; //0==>item, 1==>value
BYTE    __bDefaultValueOffset;
BYTE    __bValuePositionIndex;
BYTE    __bCursorPosIndex;


// This variable keeps the original OSD language value before the user change the setting
// so we can check this to decide if we need to switch the OSD to a different language.
BYTE    __bOSDLanguage; 


#else
extern  SETUPINFO  __SetupInfo;
extern  BYTE    SETUP_MENUINFO_NUM;
extern  BYTE    __bMenuItemEnable[];
extern  BYTE    __bSetupID;
extern  BIT     __btInputPassword;
extern  BYTE    __bPasswordIndex;
extern  BYTE    __bPassword[];
extern  BYTE    __btInputMenuLanguageCode;
extern  BYTE    __bMenuLanguageCodeIndex;
extern  BYTE    __bMenuLanguageCode[4];
extern  BYTE    __btInputAudioLanguageCode;
extern  BYTE    __bAudioLanguageCodeIndex;
extern  BYTE    __bAudioLanguageCode[4];
extern  BYTE    __btInputSPSTLanguageCode;
extern  BYTE    __bSPSTLanguageCodeIndex;
extern  BYTE    __bSPSTLanguageCode[4];
extern  BYTE    __bItemOrValue;
extern  BYTE    __bDefaultValueOffset;
extern  BYTE    __bValuePositionIndex;
extern  BYTE    __bCursorPosIndex;
extern  BYTE    __bInputPassword;
extern  BYTE    __bInputPasswordIndex;
extern  BYTE	__bLanguageCode[4];
extern  BYTE    __bSetupMode;
//For OSD usage
extern  BYTE    __bCurrentMsg;
extern  BYTE    __bOSD2FirstShowMsg;
extern  BYTE    __bDisableMsg;
#endif

//return status for input password (SETUP_InputPassword)
// brian1.08
// In my implementation, it is no need to follow Menu Item/Value ID sequence
// **** We don't expect to change the value of these definitions, for NVCMD and DVDInfo will reference them
#define SETUP_INPUT_PASSWORD_BASE               (0)
#define SETUP_INPUT_PASSWORD_NORMAL             (SETUP_INPUT_PASSWORD_BASE)
#define SETUP_INPUT_PASSWORD_CORRECT            (SETUP_INPUT_PASSWORD_BASE+1)
#define SETUP_INPUT_PASSWORD_WRONG              (SETUP_INPUT_PASSWORD_BASE+2)
#define SETUP_INPUT_PASSWORD_CLEAR              (SETUP_INPUT_PASSWORD_BASE+3)
#define SETUP_INPUT_PASSWORD_INVALID_KEY        (SETUP_INPUT_PASSWORD_BASE+4)
#define SETUP_INPUT_PASSWORD_VERIFIED           (SETUP_INPUT_PASSWORD_BASE+5) // brian, CT908




#endif

