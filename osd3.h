/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved.

        Product : WinAV Firmware

        Date    : 2003.01.14
        Author  : Cheertek (D300 BrianCheng)
        Purpose : OSD module 3
                  The moudle provides (Setup) menu UI functions
        Sources : osd3.h/ osd3.c

------------------------------------------------------------
        Update

        Date    : 
        Name    : Cheertek (D300 BrianCheng)
        Description     :

                        Item                                    Source
        ----------------------------------------        -------------------
        . 
****************************************************************************/

#ifndef __OSD3_H__
#define __OSD3_H__

#include "chips.h"

// *** BE AWARE OF THIS
// *** String Tool doesn't modify this. If the OSD language number is different ==>
// *** Please modify it manually
#if (SELL_DESTINATION == CHINA)
    #define OSD_LANGUAGE_NUMBER 2
#elif (SELL_DESTINATION == TAIWAN)
    #define OSD_LANGUAGE_NUMBER 2
#elif (SELL_DESTINATION == EUROPE)
#ifdef SUPPORT_STB
    #define OSD_LANGUAGE_NUMBER 8
#else
    #define OSD_LANGUAGE_NUMBER 8
#endif //SUPPORT_STB
#endif //(SELL_DESTINATION == EUROPE)

// Brian1.24, this value will offset the entire Menu UI
// *** Must not < -16, or some X-position will overflow, ex, DIALOG_INPUT_PASSWORD_OSD_X1
#define MENU_GLOBAL_OFFSET_X     SETUP_GLOBAL_OFFSET_X
#define MENU_GLOBAL_OFFSET_Y     SETUP_GLOBAL_OFFSET_Y


#define OSDMENU_SET_ITEM_ENABLE(MENUID, ITEMID, ENABLE)\
	{\
		_bMENUTemp = MENUID;\
		_wMENUTemp = ITEMID;\
		_bMENUTemp2 = ENABLE;\
		OSDMENU_SetItemEnable();\
	}

#define MENU_LEVEL 3        // How many level of menu.


// *** the sequence of MENU_ID must be exactly the same as _SetupMenuIndex[] array
typedef enum tagMENU_ID
{
    SETUP_MENU_MAIN=0,
    SETUP_MENU_MAIN_WITH_PROLOGIC, // Brian1.08, support runtime change menu for Audio Prologic
    SETUP_MENU_LANGUAGE,
    SETUP_MENU_SCREEN,
    SETUP_MENU_AUDIO,
    SETUP_MENU_AUDIO_WITH_PROLOGIC, // Brian1.08, support runtime change menu for Audio Prologic
    SETUP_MENU_CUSTOM,
    SETUP_MENU_LANGUAGE_OSD,
    SETUP_MENU_LANGUAGE_MENU,
    SETUP_MENU_LANGUAGE_AUDIO,
    SETUP_MENU_LANGUAGE_SP,
/*
#ifdef SUPPORT_CHAR_SUBPICTURE
#ifndef NO_CHAR_SUBPICTURE_MENU
    SETUP_MENU_LANGUAGE_EX_SP,
#endif
#endif
*/
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
    SETUP_MENU_LANGUAGE_ENCODING,
//#endif
    SETUP_MENU_SCREEN_TV_TYPE,
#ifndef NO_SCREEN_SAVER // Brian1.20
    SETUP_MENU_SCREEN_SAVER,
#endif
    SETUP_MENU_SCREENTV_SYSTEM,
#ifndef SUPPORT_TFT
    SETUP_MENU_SCREEN_VIDEO_OUTPUT,
#endif
    SETUP_MENU_SCREEN_PROGRESSIVE_SCAN,
#ifndef SUPPORT_TFT
    SETUP_MENU_SCREEN_BRIGHTNESS,
    SETUP_MENU_SCREEN_CONTRAST,
#endif
#ifdef SUPPORT_ADJUST_CHROM_PHASE // Brian1.25
	SETUP_MENU_SCREEN_CHROM_PHASE,
#endif
    SETUP_MENU_AUDIO_DIGITAL_OUT,
    SETUP_MENU_AUDIO_ANALOG_OUT,
    SETUP_MENU_AUDIO_DOWNMIX,
    SETUP_MENU_AUDIO_PROLOGIC,  // Brian1.08, support runtime change menu for Audio Prologic
    SETUP_MENU_AUDIO_CENTER,
    SETUP_MENU_AUDIO_REAR,
    SETUP_MENU_AUDIO_SUBWOOFER,
#ifndef REMOVE_SETUP_SPEAKER_TEST
    SETUP_MENU_AUDIO_SPEAKER_TEST,
#endif
    SETUP_MENU_AUDIO_DNR,
    SETUP_MENU_CUSTOM_PARENTAL_CONTROL,
#ifndef REMOVE_UI_SETTING 
    SETUP_MENU_CUSTOM_UI_STYLE,
#endif
#if defined(SUPPORT_DVD_SMRTPB) && defined(SUPPORT_DVD_SMRTPB_SETUP)
    SETUP_MENU_CUSTOM_SMART_PLAY,
#endif
    SETUP_MENU_CUSTOM_ANGLE_MARK,
    SETUP_MENU_CUSTOM_DEFAULT_VALUE,
	// AV system
	AVSYS_MENU_EQ,
    AVSYS_MENU_CHANNEL

} MENU_ID;

#define SETUP_OLD_PASSWORD 1
#define SETUP_NEW_PASSWORD 2
#define SETUP_VERIFY_PASSWORD 3

//#include "multistr.h"

/*Alan2.13, move from osd3.h to multistr.h 
// The multiple language string structure for menu item string
typedef struct tagMULTI_STRING
{
    WORD * pbStr[OSD_LANGUAGE_NUMBER];
} MULTI_STRING, *PMULTI_STRING;
*/

// The menu structure
typedef struct tagMENU_STRUCT
{
    WORD *pbItemText;
    struct tagMENU_STRUCT *pSubMenu;
    BYTE    bAttribute;      // bit0~2: 0:item, 1:value, 2:..., 3: --,   bit4: 0:string, 1:icon
    BYTE    Id;
} MENU_STRUCT, *PMENU_STRUCT;


#ifdef MENU_DECLARATION 
#include "OSDString\\strextrn.h"

// Brian1.08a
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU

// UI layout for horizontal icon menu

// Level 0 menu location and width 
#define SETUP_MENU_ROOT_X   (36+MENU_GLOBAL_OFFSET_X) 
//#define SETUP_MENU_ROOT_Y   (MENU_GLOBAL_OFFSET_Y + 53) 
#define SETUP_MENU_ROOT_Y   (MENU_GLOBAL_OFFSET_Y + 46) 
#define SETUP_MENU_ROOT_W   (60+4)      // each icon is 60 pixel wide plus 2*2 pixel border
#define SETUP_MENU_ROOT_R   5       // can display x icons in the list

// Level 1 menu location (x, y), width and height (how may row of text)
#define SETUP_MENU_ITEM_X   (22+MENU_GLOBAL_OFFSET_X) // Brian1.24
#define SETUP_MENU_ITEM_Y   120//135//90
#define SETUP_MENU_ITEM_W   255
#define SETUP_MENU_ITEM_R   4

// Level 2 menu location (x, y), width and height (how may row of text)
#define SETUP_MENU_VALUE_X   (328+MENU_GLOBAL_OFFSET_X) // Brian1.24
#define SETUP_MENU_VALUE_Y   120//135
#define SETUP_MENU_VALUE_W   240
#define SETUP_MENU_VALUE_R   5

#else

// UI layout for vertical icon menu

// Level 0 menu location and width 
#define SETUP_MENU_ROOT_X   (12+MENU_GLOBAL_OFFSET_X) 
#define SETUP_MENU_ROOT_Y   (MENU_GLOBAL_OFFSET_Y + 72) 

#ifdef REMOVE_SETUP_ICON	//xuli1118
#define SETUP_MENU_ROOT_W   (60+8)      // let frame a little bit wider than text
#else
#define SETUP_MENU_ROOT_W   (60+4)      // each icon is 60 pixel wide plus 2*2 pixel border
#endif

#define SETUP_MENU_ROOT_R   5       // can display x icons in the list

// Level 1 menu location (x, y), width and height (how may row of text)
#define SETUP_MENU_ITEM_X   (95+MENU_GLOBAL_OFFSET_X) // Brian1.24
#define SETUP_MENU_ITEM_Y   (MENU_GLOBAL_OFFSET_Y + 72) 
#define SETUP_MENU_ITEM_W   255
#define SETUP_MENU_ITEM_R   4 

// Level 2 menu location (x, y), width and height (how may row of text)
#define SETUP_MENU_VALUE_X   (365+MENU_GLOBAL_OFFSET_X) // Brian1.24
#define SETUP_MENU_VALUE_Y   (MENU_GLOBAL_OFFSET_Y + 72) 
#define SETUP_MENU_VALUE_W   240
#define SETUP_MENU_VALUE_R   5

#endif



// bAttribute: bit 0~1
#define IS_ITEM     0   // the item is an "item", 
#define IS_VALUE    1   // the item is a "value"
#define IS_DIALOG   2   // the item is a "..."
#define IS_LINE     3   // the item is a seperate line

// Declare the menu hierachy

MENU_STRUCT _MenuLanguageOSD[]={
#if (OSD_LANGUAGE_GROUP == CHINA_AREA_OSD)
#if (CHINESE_OSD_MODE == SIMPLE_CHINESE)
    {(WORD *)(&(__SetupInfo.bOSDLanguage)),  (struct tagMENU_STRUCT *)SETUP_MENU_LANGUAGE_OSD, 2, SETUP_LANGUAGE_OSD_BASE},
    {(WORD *)SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
#ifdef OSD_ITEM_TEXT_SAME_AS_LANGUAGE
    {aMsgOSDEnglish,   NULL,   0x01,   SETUP_LANGUAGE_OSD_ENGLISH},
    {aMsgOSDChinese,   NULL,   0x01,   SETUP_LANGUAGE_OSD_SIMP_CHINESE}
#else
    {aMsgEnglish,   NULL,   0x01,   SETUP_LANGUAGE_OSD_ENGLISH},
    {aMsgChinese,   NULL,   0x01,   SETUP_LANGUAGE_OSD_SIMP_CHINESE}
#endif	//#ifdef OSD_ITEM_TEXT_SAME_AS_LANGUAGE
    };
#elif (CHINESE_OSD_MODE == TRADITIONAL_CHINESE)
    {(WORD *)(&(__SetupInfo.bOSDLanguage)),  (struct tagMENU_STRUCT *)SETUP_MENU_LANGUAGE_OSD, 2, SETUP_LANGUAGE_OSD_BASE},
    {(WORD *)SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
#ifdef OSD_ITEM_TEXT_SAME_AS_LANGUAGE
    {aMsgOSDEnglish,   NULL,   0x01,   SETUP_LANGUAGE_OSD_ENGLISH},
    {aMsgOSDChinese,   NULL,   0x01,   SETUP_LANGUAGE_OSD_TRAD_CHINESE}
#else
    {aMsgEnglish,   NULL,   0x01,   SETUP_LANGUAGE_OSD_ENGLISH},
    {aMsgChinese,   NULL,   0x01,   SETUP_LANGUAGE_OSD_TRAD_CHINESE}
#endif	//#ifdef OSD_ITEM_TEXT_SAME_AS_LANGUAGE
    };
#endif	//#elif (CHINESE_OSD_MODE == TRADITIONAL_CHINESE)
#elif (OSD_LANGUAGE_GROUP == EUROPE_AREA_OSD)
    {(WORD *)(&(__SetupInfo.bOSDLanguage)),  (struct tagMENU_STRUCT *)SETUP_MENU_LANGUAGE_OSD, OSD_LANGUAGE_NUMBER, SETUP_LANGUAGE_OSD_BASE},
    {(WORD *)SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
#ifdef OSD_ITEM_TEXT_SAME_AS_LANGUAGE
    {aMsgOSDEnglish,   NULL,   0x01,   SETUP_LANGUAGE_OSD_ENGLISH},
    {aMsgOSDFrench,    NULL,   0x01,   SETUP_LANGUAGE_OSD_FRENCH},
    {aMsgOSDGerman,    NULL,   0x01,   SETUP_LANGUAGE_OSD_GERMAN},
    {aMsgOSDItalian,   NULL,   0x01,   SETUP_LANGUAGE_OSD_ITALIAN},
    {aMsgOSDSpanish,   NULL,   0x01,   SETUP_LANGUAGE_OSD_SPANISH},
    {aMsgOSDPortuguese,NULL,   0x01,   SETUP_LANGUAGE_OSD_PORTUGUESE},
    {aMsgOSDDutch,     NULL,   0x01,   SETUP_LANGUAGE_OSD_DUTCH},
    {aMsgOSDPolish,    NULL,   0x01,   SETUP_LANGUAGE_OSD_POLISH},
    /*
    {aMsgCzech,     NULL,   0x01,   SETUP_LANGUAGE_OSD_CZECH},
    {aMsgRussian,    NULL,   0x01,   SETUP_LANGUAGE_OSD_RUSSIAN},
    {aMsgGreece,    NULL,   0x01,   SETUP_LANGUAGE_OSD_GREECE},
    {aMsgTurkey,    NULL,   0x01,   SETUP_LANGUAGE_OSD_TURKEY}
    */
#else	//#ifdef OSD_ITEM_TEXT_SAME_AS_LANGUAGE
    {aMsgEnglish,   NULL,   0x01,   SETUP_LANGUAGE_OSD_ENGLISH},
    {aMsgFrench,    NULL,   0x01,   SETUP_LANGUAGE_OSD_FRENCH},
    {aMsgGerman,    NULL,   0x01,   SETUP_LANGUAGE_OSD_GERMAN},
    {aMsgItalian,   NULL,   0x01,   SETUP_LANGUAGE_OSD_ITALIAN},
    {aMsgSpanish,   NULL,   0x01,   SETUP_LANGUAGE_OSD_SPANISH},
    {aMsgPortuguese,NULL,   0x01,   SETUP_LANGUAGE_OSD_PORTUGUESE},
    {aMsgDutch,  NULL,   0x01,   SETUP_LANGUAGE_OSD_DUTCH},
    {aMsgPolish, NULL,   0x01,   SETUP_LANGUAGE_OSD_POLISH},
    /*
    {aMsgCzech,     NULL,   0x01,   SETUP_LANGUAGE_OSD_CZECH},
    {aMsgRussian,    NULL,   0x01,   SETUP_LANGUAGE_OSD_RUSSIAN},
    {aMsgGreece,    NULL,   0x01,   SETUP_LANGUAGE_OSD_GREECE},
    {aMsgTurkey,    NULL,   0x01,   SETUP_LANGUAGE_OSD_TURKEY}
    */
#endif	//#ifdef OSD_ITEM_TEXT_SAME_AS_LANGUAGE
    };
#elif (OSD_LANGUAGE_GROUP == KOREAN_AREA_OSD) //Brian1.07, Jeff 20030917 Add for Korean OSD
    {(WORD *)(&(__SetupInfo.bOSDLanguage)),  (struct tagMENU_STRUCT *)SETUP_MENU_LANGUAGE_OSD, 2, SETUP_LANGUAGE_OSD_BASE},
    {(WORD *)SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgEnglish,   NULL,   0x01,   SETUP_LANGUAGE_OSD_ENGLISH},
    {aMsgKorean,   NULL,   0x01,   SETUP_LANGUAGE_OSD_KOREAN}
    };
#elif (OSD_LANGUAGE_GROUP == JAPAN_AREA_OSD)
    {(WORD *)(&(__SetupInfo.bOSDLanguage)),  (struct tagMENU_STRUCT *)SETUP_MENU_LANGUAGE_OSD, 2, SETUP_LANGUAGE_OSD_BASE},
    {(WORD *)SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgEnglish,   NULL,   0x01,   SETUP_LANGUAGE_OSD_ENGLISH},
    {aMsgJapanese,   NULL,   0x01,   SETUP_LANGUAGE_OSD_JAPAN}
    };
#endif

/*
#else // #ifndef SUPPORT_STRING_GENERATOR
#include "langmenu.h" // the language menu is generated by string tool
#endif // #ifndef SUPPORT_STRING_GENERATOR
*/

MENU_STRUCT _MenuLanguageMenu[]={
    {(WORD * )&(__SetupInfo.bMenuLanguage),  (struct tagMENU_STRUCT *)SETUP_MENU_LANGUAGE_MENU, 13, SETUP_LANGUAGE_MENU_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgEnglish,   NULL,   0x01,   SETUP_LANGUAGE_MENU_ENGLISH},
    {aMsgChinese,   NULL,   0x01,   SETUP_LANGUAGE_MENU_CHINESE},
    {aMsgJapanese,  NULL,   0x01,   SETUP_LANGUAGE_MENU_JAPANESE},
    {aMsgFrench,    NULL,   0x01,   SETUP_LANGUAGE_MENU_FRENCH},
    {aMsgGerman,    NULL,   0x01,   SETUP_LANGUAGE_MENU_GERMAN},
    {aMsgItalian,   NULL,   0x01,   SETUP_LANGUAGE_MENU_ITALIAN},
    {aMsgSpanish,   NULL,   0x01,   SETUP_LANGUAGE_MENU_SPANISH},
    {aMsgPortuguese,NULL,   0x01,   SETUP_LANGUAGE_MENU_PORTUGUESE},
    {aMsgHindi,      NULL,   0x01,   SETUP_LANGUAGE_MENU_HINDI},
    {aMsgThai,      NULL,   0x01,   SETUP_LANGUAGE_MENU_THAI},
    {aMsgRussian,   NULL,   0x01,   SETUP_LANGUAGE_MENU_RUSSIAN},
    {aMsgKorean,    NULL,   0x01,   SETUP_LANGUAGE_MENU_KOREAN},
    {aMsgOthers,    NULL,   0x01,   SETUP_LANGUAGE_MENU_OTHERS},
    };

MENU_STRUCT   _MenuLanguageAudio[]={
    {(WORD *   )&(__SetupInfo.bAudioLanguage),  (struct tagMENU_STRUCT   *)SETUP_MENU_LANGUAGE_AUDIO, 13, SETUP_LANGUAGE_AUDIO_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgEnglish,   NULL,   0x01,   SETUP_LANGUAGE_AUDIO_ENGLISH},
    {aMsgChinese,   NULL,   0x01,   SETUP_LANGUAGE_AUDIO_CHINESE},
    {aMsgJapanese,  NULL,   0x01,   SETUP_LANGUAGE_AUDIO_JAPANESE},
    {aMsgFrench,    NULL,   0x01,   SETUP_LANGUAGE_AUDIO_FRENCH},
    {aMsgGerman,    NULL,   0x01,   SETUP_LANGUAGE_AUDIO_GERMAN},
    {aMsgItalian,   NULL,   0x01,   SETUP_LANGUAGE_AUDIO_ITALIAN},
    {aMsgSpanish,   NULL,   0x01,   SETUP_LANGUAGE_AUDIO_SPANISH},
    {aMsgPortuguese,NULL,   0x01,   SETUP_LANGUAGE_AUDIO_PORTUGUESE},
    {aMsgHindi,      NULL,   0x01,   SETUP_LANGUAGE_AUDIO_HINDI},
    {aMsgThai,      NULL,   0x01,   SETUP_LANGUAGE_AUDIO_THAI},
    {aMsgRussian,   NULL,   0x01,   SETUP_LANGUAGE_AUDIO_RUSSIAN},
    {aMsgKorean,    NULL,   0x01,   SETUP_LANGUAGE_AUDIO_KOREAN},
    {aMsgOthers,    NULL,   0x01,   SETUP_LANGUAGE_AUDIO_OTHERS},
    };

MENU_STRUCT   _MenuLanguageSP[]={
    {(WORD *   )&(__SetupInfo.bSPSTLanguage),  (struct tagMENU_STRUCT   *)SETUP_MENU_LANGUAGE_SP, 14, SETUP_LANGUAGE_SP_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgEnglish,       NULL,   0x01,   SETUP_LANGUAGE_SP_ENGLISH},
    {aMsgChinese,       NULL,   0x01,   SETUP_LANGUAGE_SP_CHINESE},
    {aMsgJapanese,      NULL,   0x01,   SETUP_LANGUAGE_SP_JAPANESE},
    {aMsgFrench,        NULL,   0x01,   SETUP_LANGUAGE_SP_FRENCH},
    {aMsgGerman,        NULL,   0x01,   SETUP_LANGUAGE_SP_GERMAN},
    {aMsgItalian,       NULL,   0x01,   SETUP_LANGUAGE_SP_ITALIAN},
    {aMsgSpanish,       NULL,   0x01,   SETUP_LANGUAGE_SP_SPANISH},
    {aMsgPortuguese,    NULL,   0x01,   SETUP_LANGUAGE_SP_PORTUGUESE},
    {aMsgHindi,          NULL,   0x01,   SETUP_LANGUAGE_SP_HINDI},
    {aMsgThai,          NULL,   0x01,   SETUP_LANGUAGE_SP_THAI},
    {aMsgRussian,       NULL,   0x01,   SETUP_LANGUAGE_SP_RUSSIAN},
    {aMsgKorean,        NULL,   0x01,   SETUP_LANGUAGE_SP_KOREAN},
    {aMsgOthers,        NULL,   0x01,   SETUP_LANGUAGE_SP_OTHERS},
    {aMsgOff,        NULL,   0x01,   SETUP_LANGUAGE_SP_OFF},
    };

/*
#ifdef SUPPORT_CHAR_SUBPICTURE
#ifndef NO_CHAR_SUBPICTURE_MENU
MENU_STRUCT   _MenuLanguageEx_SP[]={
    {(WORD *   )&(__SetupInfo.bExSPSTLanguage),  (struct tagMENU_STRUCT   *)SETUP_MENU_LANGUAGE_EX_SP, 2, SETUP_LANGUAGE_EX_SP_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
#ifdef ISO_8859_15_FONT
    {aMsgWesternEuropean,  NULL,  0x01,  SETUP_LANGUAGE_EX_SP_WESTERN_EUROPEAN},
#endif
#ifdef ISO_8859_2_FONT
    {aMsgCentralEuropean,  NULL,  0x01,  SETUP_LANGUAGE_EX_SP_CENTRAL_EUROPEAN},
#endif
#ifdef ISO_8859_7_FONT
    {aMsgGreek,  NULL,  0x01,  SETUP_LANGUAGE_EX_SP_GREEK},
#endif
// Damon2.51, for Hebrew
#ifdef ISO_8859_8_FONT
    {aMsgHebrew,  NULL,  0x01,  SETUP_LANGUAGE_EX_SP_HEBREW},
#endif
#ifdef ISO_8859_9_FONT
    {aMsgTurkish,    NULL,   0x01,   SETUP_LANGUAGE_EX_SP_TURKISH},
#endif
#ifdef CP_1251_FONT
    {aMsgCyrillic,  NULL,  0x01,  SETUP_LANGUAGE_EX_SP_CYRILLIC},
#endif
#ifdef CP_1250_FONT
    {aMsgCP1250,  NULL,  0x01,  SETUP_LANGUAGE_EX_SP_CP1250},
#endif
    };
#endif   // NO_CHAR_SUBPICTURE_MENU
#endif   // SUPPORT_CHAR_SUBPICTURE
*/

//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
MENU_STRUCT   _MenuLanguageEncoding[]={
    {(WORD *   )&(__SetupInfo.bExSPSTLanguage),  (struct tagMENU_STRUCT   *)SETUP_MENU_LANGUAGE_ENCODING, SETUP_LANGUAGE_ENCODING_END-SETUP_LANGUAGE_ENCODING_BEGIN-1, SETUP_LANGUAGE_ENCODING_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN
    {aCODEWesternEuropean,  NULL,  0x01,  SETUP_LANGUAGE_ENCODING_WESTERN_EUROPEAN},
#endif
#ifdef SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN
    {aCODECentralEuropean,  NULL,  0x01,  SETUP_LANGUAGE_ENCODING_CENTRAL_EUROPEAN},
#endif
#ifdef SUPPORT_CHAR_ENCODING_GREEK
    {aCODEGreek,  NULL,  0x01,  SETUP_LANGUAGE_ENCODING_GREEK},
#endif
// Damon2.51, for Hebrew
#ifdef SUPPORT_CHAR_ENCODING_HEBREW
    {aCODEHebrew,  NULL,  0x01,  SETUP_LANGUAGE_ENCODING_HEBREW},
#endif
#ifdef SUPPORT_CHAR_ENCODING_TURKISH
    {aCODETurkish,    NULL,   0x01,   SETUP_LANGUAGE_ENCODING_TURKISH},
#endif
#ifdef SUPPORT_CHAR_ENCODING_CP1250
    {aCODECP1250,  NULL,  0x01,  SETUP_LANGUAGE_ENCODING_CP1250},
#endif
#ifdef SUPPORT_CHAR_ENCODING_CYRILLIC
    {aCODECyrillic,  NULL,  0x01,  SETUP_LANGUAGE_ENCODING_CYRILLIC},
#endif
#ifdef SUPPORT_CHAR_ENCODING_CHINESE
    {aCODETWNUni,  NULL,  0x01,  SETUP_LANGUAGE_ENCODING_TWN_UNICODE},
#endif
#ifdef SUPPORT_CHAR_ENCODING_BIG5
    {aCODETWNBig5,  NULL,  0x01,  SETUP_LANGUAGE_ENCODING_TWN_BIG5},
#endif
#ifdef SUPPORT_CHAR_ENCODING_JAPANESE
    {aCODEJPNUni,  NULL,  0x01,  SETUP_LANGUAGE_ENCODING_JPN_UNICODE},
#endif
#ifdef SUPPORT_CHAR_ENCODING_SHIFT_JIS
    {aCODEJPNJIS,  NULL,  0x01,  SETUP_LANGUAGE_ENCODING_JPN_SHIFT_JIS},
#endif
    };
//#endif   // SUPPORT_CHAR_ENCODING_SWITCH

BYTE _bEnableLanguage[1];

MENU_STRUCT   _MenuLanguage[]={
/*
#ifdef SUPPORT_CHAR_SUBPICTURE
#ifndef NO_CHAR_SUBPICTURE_MENU
    {(WORD *   )_bEnableLanguage,  (struct tagMENU_STRUCT   *)SETUP_MENU_LANGUAGE, 5, SETUP_LANGUAGE_BASE},
#else
    {(WORD *   )_bEnableLanguage,  (struct tagMENU_STRUCT   *)SETUP_MENU_LANGUAGE, 4, SETUP_LANGUAGE_BASE},
#endif
#else
    {(WORD *   )_bEnableLanguage,  (struct tagMENU_STRUCT   *)SETUP_MENU_LANGUAGE, 4, SETUP_LANGUAGE_BASE},
#endif
*/
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
    {(WORD *   )_bEnableLanguage,  (struct tagMENU_STRUCT   *)SETUP_MENU_LANGUAGE, 5, SETUP_LANGUAGE_BASE},
/*#else
    {(WORD *   )_bEnableLanguage,  (struct tagMENU_STRUCT   *)SETUP_MENU_LANGUAGE, 4, SETUP_LANGUAGE_BASE},
#endif*/
    {(WORD *   )SETUP_MENU_ITEM_X, (struct tagMENU_STRUCT   *)SETUP_MENU_ITEM_Y, SETUP_MENU_ITEM_R, SETUP_MENU_ITEM_W},
    {aMsgOSD,       (PMENU_STRUCT)&_MenuLanguageOSD,   0x0,   SETUP_LANGUAGE_OSD},
    {aMsgMenu,      (PMENU_STRUCT)&_MenuLanguageMenu,   0x0,   SETUP_LANGUAGE_MENU},
    {aMsgAudio,     (PMENU_STRUCT)&_MenuLanguageAudio,   0x0,   SETUP_LANGUAGE_AUDIO},
    {aMsgSubtitle,  (PMENU_STRUCT)&_MenuLanguageSP,   0x0,   SETUP_LANGUAGE_SUBTITLE},
/*
#ifdef SUPPORT_CHAR_SUBPICTURE
#ifndef NO_CHAR_SUBPICTURE_MENU
    {aMsgEx_Subtitle,  (PMENU_STRUCT)&_MenuLanguageEx_SP,   0x0,   SETUP_LANGUAGE_EX_SUBTITLE}
#endif
#endif
*/
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
    {aMENUEncoding,  (PMENU_STRUCT)&_MenuLanguageEncoding,   0x0,   SETUP_LANGUAGE_ENCODING}
//#endif
    };

MENU_STRUCT   _MenuScreenTVType[]={
    {(WORD *   )&(__SetupInfo.bAspectRatio),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREEN_TV_TYPE, 3, SETUP_SCREEN_TV_TYPE_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgPS,    NULL,   0x01,   SETUP_SCREEN_TV_TYPE_PS},
    {aMsgLB,    NULL,   0x01,   SETUP_SCREEN_TV_TYPE_LB},
    {aMsgWide,  NULL,   0x01,   SETUP_SCREEN_TV_TYPE_WIDE}
    };

#ifndef NO_SCREEN_SAVER // Brian1.20
MENU_STRUCT   _MenuScreenSaver[]={
    {(WORD *   )&(__SetupInfo.bSCRControl),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREEN_SAVER, 2, SETUP_SCREEN_SAVER_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgOn,   NULL,   0x01,   SETUP_SCREEN_SAVER_ON},
    {aMsgOff,  NULL,   0x01,   SETUP_SCREEN_SAVER_OFF}
    };
#endif

MENU_STRUCT   _MenuScreenTVSystem[]={
#ifdef SUPPORT_EXTRA_PAL_MODE
    {(WORD *   )&(__SetupInfo.bTVSystem),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREENTV_SYSTEM, 5, SETUP_SCREEN_TV_SYSTEM_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgNTSC,  NULL,   0x01,   SETUP_SCREEN_TV_SYSTEM_NTSC},
    {aMsgPAL,   NULL,   0x01,   SETUP_SCREEN_TV_SYSTEM_PAL},
    {aMsgAuto,  NULL,   0x01,   SETUP_SCREEN_TV_SYSTEM_AUTO},
    {aMsgPAL_M,   NULL,   0x01,   SETUP_SCREEN_TV_SYSTEM_PAL_M},
    {aMsgPAL_N,   NULL,   0x01,   SETUP_SCREEN_TV_SYSTEM_PAL_N}

#else
    {(WORD *   )&(__SetupInfo.bTVSystem),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREENTV_SYSTEM, 3, SETUP_SCREEN_TV_SYSTEM_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgNTSC,  NULL,   0x01,   SETUP_SCREEN_TV_SYSTEM_NTSC},
    {aMsgPAL,   NULL,   0x01,   SETUP_SCREEN_TV_SYSTEM_PAL},
    {aMsgAuto,  NULL,   0x01,   SETUP_SCREEN_TV_SYSTEM_AUTO}
#endif

    };


// Brian1.07, support SCART interface
#ifdef SUPPORT_SCART_IF

#define VIDEO_OUTPUT_NO	3 //5 //xuli.0728

#ifndef SUPPORT_TFT 
MENU_STRUCT   _MenuScreenVideoOutput[]={
    {(WORD *   )&(__SetupInfo.bVideoOutput),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREEN_VIDEO_OUTPUT, VIDEO_OUTPUT_NO, SETUP_SCREEN_VIDEO_OUTPUT_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgNormal,  NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_NORMAL},
    {aMsgRGB,   NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_RGB},
    {aMsgRGBPScan,   NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN},
//    {aMsgYUV,  NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_YUV},
//    {aMsgYUVPScan,   NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN},
    };
#endif

#else

// For the most case, we use this.

//Brian1.00-4, RELEASE_SETTING no VGA item
#ifdef NO_PSCAN_OUTPUT
#define VIDEO_OUTPUT_NO	3
#elif defined(NO_VGA_OUTPUT) // RELEASE_SETTING
#define VIDEO_OUTPUT_NO	5
#else
#define VIDEO_OUTPUT_NO	6
#endif

#ifndef SUPPORT_TFT 
#ifdef NO_PSCAN_OUTPUT
MENU_STRUCT   _MenuScreenVideoOutput[]={
    {(WORD *   )&(__SetupInfo.bVideoOutput),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREEN_VIDEO_OUTPUT, VIDEO_OUTPUT_NO, SETUP_SCREEN_VIDEO_OUTPUT_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgVideoNormal,  NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_NORMAL},
    {aMsgRGB,   NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_RGB},    
    {aMsgYUV,   NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_YUV},    
    };
#else
MENU_STRUCT   _MenuScreenVideoOutput[]={
    {(WORD *   )&(__SetupInfo.bVideoOutput),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREEN_VIDEO_OUTPUT, VIDEO_OUTPUT_NO, SETUP_SCREEN_VIDEO_OUTPUT_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgVideoNormal,  NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_NORMAL},
    {aMsgRGB,   NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_RGB},
    {aMsgRGBPScan,   NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN},
    {aMsgYUV,   NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_YUV},
    {aMsgYUVPScan,   NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN}
#ifndef NO_VGA_OUTPUT // RELEASE_SETTING
    ,{aMsgVGAPScan,   NULL,   0x01,   SETUP_SCREEN_VIDEO_OUTPUT_VGA}
#endif
    };
#endif // #ifndef SUPPORT_TFT
#endif // #ifdef NO_PSCAN_OUTPUT

#endif // #ifdef SUPPORT_SCART_IF


MENU_STRUCT   _MenuProgressiveScan[]={
    {(WORD *   )&(__SetupInfo.bProgressiveScan),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREEN_PROGRESSIVE_SCAN, 2, SETUP_PROGRESSIVE_SCAN_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgOn,   NULL,   0x01,   SETUP_PROGRESSIVE_SCAN_ON},
    {aMsgOff,  NULL,   0x01,   SETUP_PROGRESSIVE_SCAN_OFF}
    };

#ifndef SUPPORT_TFT
MENU_STRUCT   _MenuScreenBrightness[]={
    {(WORD *   )&(__SetupInfo.bBrightness),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREEN_BRIGHTNESS, 3, SETUP_SCREEN_BRIGHTNESS_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgBriNormal,    NULL,   0x01,   SETUP_SCREEN_BRIGHTNESS_NORMAL},
    {aMsgBright,    NULL,   0x01,   SETUP_SCREEN_BRIGHTNESS_BRIGHT},
    {aMsgSoft,      NULL,   0x01,   SETUP_SCREEN_BRIGHTNESS_SOFT}
    };

MENU_STRUCT   _MenuScreenContrast[]={
    {(WORD *   )&(__SetupInfo.bContrast),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREEN_CONTRAST, 9, SETUP_SCREEN_CONTRAST_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, 7, 180}, //SETUP_MENU_VALUE_W},
    {aMsgMinus4,    NULL,   0x01,   SETUP_SCREEN_CONTRAST_MINUS4},
    {aMsgMinus3,    NULL,   0x01,   SETUP_SCREEN_CONTRAST_MINUS3},
    {aMsgMinus2,    NULL,   0x01,   SETUP_SCREEN_CONTRAST_MINUS2},
    {aMsgMinus1,    NULL,   0x01,   SETUP_SCREEN_CONTRAST_MINUS1},
    {aMsg0,         NULL,   0x01,   SETUP_SCREEN_CONTRAST_0},
    {aMsgPlus1,     NULL,   0x01,   SETUP_SCREEN_CONTRAST_PLUS1},
    {aMsgPlus2,     NULL,   0x01,   SETUP_SCREEN_CONTRAST_PLUS2},
    {aMsgPlus3,     NULL,   0x01,   SETUP_SCREEN_CONTRAST_PLUS3},
    {aMsgPlus4,     NULL,   0x01,   SETUP_SCREEN_CONTRAST_PLUS4}
    };
#endif // #ifndef SUPPORT_TFT

#ifdef SUPPORT_ADJUST_CHROM_PHASE // Brian1.25
MENU_STRUCT   _MenuScreenChromPhase[]={
    {(WORD *   )&(__SetupInfo.bChromPhaseOffset),  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREEN_CHROM_PHASE, 13, SETUP_SCREEN_CHROM_PHASE_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, 7, 180}, //SETUP_MENU_VALUE_W},
    {aMsgMinus6,    NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_MINUS6},
    {aMsgMinus5,    NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_MINUS5},    
    {aMsgMinus4,    NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_MINUS4},
    {aMsgMinus3,    NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_MINUS3},
    {aMsgMinus2,    NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_MINUS2},
    {aMsgMinus1,    NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_MINUS1},       
    {aMsg0,         NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_0},
    {aMsgPlus1,     NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_PLUS1},
    {aMsgPlus2,     NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_PLUS2},
    {aMsgPlus3,     NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_PLUS3},
    {aMsgPlus4,     NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_PLUS4},
    {aMsgPlus5,     NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_PLUS5},
    {aMsgPlus6,     NULL,   0x01,   SETUP_SCREEN_CHROM_PHASE_PLUS6},    
    };
#endif

BYTE _bEnableScreen[1];

#ifndef NO_SCREEN_SAVER // Brian1.20
#ifndef SUPPORT_ADJUST_CHROM_PHASE // Brian1.25
#ifndef SUPPORT_TFT
#define SCREEN_ITEM_NO     6 //7 // Brian1.06, P-SCAN is integrated into Video Output
#else
#define SCREEN_ITEM_NO     3
#endif
#else
#ifndef SUPPORT_TFT
#define SCREEN_ITEM_NO     7
#else
#define SCREEN_ITEM_NO     4
#endif
#endif
#else
#ifndef SUPPORT_ADJUST_CHROM_PHASE
#ifndef SUPPORT_TFT
#define SCREEN_ITEM_NO     5 //7 // Brian1.06, P-SCAN is integrated into Video Output
#else
#define SCREEN_ITEM_NO     2
#endif
#else
#ifndef SUPPORT_TFT
#define SCREEN_ITEM_NO		6
#else
#define SCREEN_ITEM_NO          3
#endif
#endif
#endif

MENU_STRUCT   _MenuScreen[]={
    {(WORD *   )_bEnableScreen,  (struct tagMENU_STRUCT   *)SETUP_MENU_SCREEN, SCREEN_ITEM_NO, SETUP_SCREEN_BASE},
    {(WORD *   )SETUP_MENU_ITEM_X, (struct tagMENU_STRUCT   *)SETUP_MENU_ITEM_Y, 6, SETUP_MENU_ITEM_W},
    {aMsgTVType,        (PMENU_STRUCT)&_MenuScreenTVType,   0x0,   SETUP_SCREEN_TV_TYPE},
#ifndef NO_SCREEN_SAVER // Brian1.20
    {aMsgScreenSaver,   (PMENU_STRUCT)&_MenuScreenSaver,   0x0,   SETUP_SCREEN_SAVER},
#endif
    {aMsgTVSystem,      (PMENU_STRUCT)&_MenuScreenTVSystem,   0x0,   SETUP_SCREEN_TV_SYSTEM},
#ifndef SUPPORT_TFT
    {aMsgVideoOutput,   (PMENU_STRUCT)&_MenuScreenVideoOutput,   0x0,   SETUP_SCREEN_VIDEO_OUTPUT},
#endif
    //{aMsgProgressiveScan,   (PMENU_STRUCT)&_MenuProgressiveScan,   0x0,   SETUP_SCREEN_PROGRESSIVE_SCAN},
#ifndef SUPPORT_TFT
    {aMsgBrightness,    (PMENU_STRUCT)&_MenuScreenBrightness,   0x0,   SETUP_SCREEN_BRIGHTNESS},
    {aMsgContrast,      (PMENU_STRUCT)&_MenuScreenContrast,   0x0,   SETUP_SCREEN_CONTRAST},
#endif
#ifdef SUPPORT_ADJUST_CHROM_PHASE // Brian1.25
    {aMsgChromPhase,      (PMENU_STRUCT)&_MenuScreenChromPhase,   0x0,   SETUP_SCREEN_CHROM_PHASE}
#endif
    };


MENU_STRUCT   _MenuAudioDigitalOut[]={
    {(WORD *   )&(__SetupInfo.bAudioDigitalOut),  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_DIGITAL_OUT, 3, SETUP_AUDIO_DIGITAL_OUT_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgSPDIF_PCM, NULL,   0x01,   SETUP_AUDIO_DIGITAL_OUT_SPDIF_PCM},
    {aMsgSPDIF_RAW, NULL,   0x01,   SETUP_AUDIO_DIGITAL_OUT_SPDIF_RAW},
    {aMsgOff,       NULL,   0x01,   SETUP_AUDIO_DIGITAL_OUT_OFF}
    };

#ifdef SUPPORT_ANALOG_OUTPUT_OFF // Brian1.10
MENU_STRUCT   _MenuAudioAnalogOut[]={
    {(WORD *   *)&(__SetupInfo.bAudioAnalogOut),  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_ANALOG_OUT, 3, SETUP_AUDIO_ANALOG_OUT_BASE},
    {(WORD *   *)SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsg2CH,   NULL,   0x01,   SETUP_AUDIO_ANALOG_OUT_2CH},
    {aMsg51CH,  NULL,   0x01,   SETUP_AUDIO_ANALOG_OUT_51CH},
    {aMsgOff,   NULL,   0x01,   SETUP_AUDIO_ANALOG_OUT_OFF}
    };
#else
MENU_STRUCT   _MenuAudioAnalogOut[]={
    {(WORD *   )&(__SetupInfo.bAudioAnalogOut),  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_ANALOG_OUT, 2, SETUP_AUDIO_ANALOG_OUT_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsg2CH,   NULL,   0x01,   SETUP_AUDIO_ANALOG_OUT_2CH},
    {aMsg51CH,  NULL,   0x01,   SETUP_AUDIO_ANALOG_OUT_51CH}
};
#endif


MENU_STRUCT   _MenuAudioDownMix[]={
    {(WORD *   )&(__SetupInfo.bAudioDownMix),  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_DOWNMIX, 2, SETUP_AUDIO_DIGITAL_OUT_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgLoRo, NULL,   0x01,   SETUP_AUDIO_DOWNMIX_LORO},
    {aMsgLtRt, NULL,   0x01,   SETUP_AUDIO_DOWNMIX_LTRT}
    };


// Brian1.08, Pro Logic
MENU_STRUCT   _MenuAudioPrologic[]={
    {(WORD *   )&(__SetupInfo.bAudioPrologic),  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_PROLOGIC, 6, SETUP_AUDIO_PROLOGIC_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgPrologic,   NULL,   0x01,   SETUP_AUDIO_PROLOGIC_PROLOGIC},
    {aMsgVirtual,    NULL,   0x01,   SETUP_AUDIO_PROLOGIC_VIRTUAL},
    {aMsgMusic,      NULL,   0x01,   SETUP_AUDIO_PROLOGIC_MUSIC},
    {aMsgMovie,      NULL,   0x01,   SETUP_AUDIO_PROLOGIC_MOVIE},
    {aMsgMatrix,     NULL,   0x01,   SETUP_AUDIO_PROLOGIC_MATRIX},
    {aMsgOff,        NULL,   0x01,   SETUP_AUDIO_PROLOGIC_OFF}
    };

MENU_STRUCT   _MenuAudioCenter[]={
    {(WORD *   )&(__SetupInfo.bAudioCenter),  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_CENTER, 2, SETUP_AUDIO_CENTER_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgOn,   NULL,   0x01,   SETUP_AUDIO_CENTER_ON},
    {aMsgOff,  NULL,   0x01,   SETUP_AUDIO_CENTER_OFF}
    };

MENU_STRUCT   _MenuAudioRear[]={
    {(WORD *   )&(__SetupInfo.bAudioRear),  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_REAR, 2, SETUP_AUDIO_REAR_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgOn,   NULL,   0x01,   SETUP_AUDIO_REAR_ON},
    {aMsgOff,  NULL,   0x01,   SETUP_AUDIO_REAR_OFF}
    };

MENU_STRUCT   _MenuAudioSubwoofer[]={
    {(WORD *   )&(__SetupInfo.bAudioSubwoofer),  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_SUBWOOFER, 2, SETUP_AUDIO_SUBWOOFER_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgOn,   NULL,   0x01,   SETUP_AUDIO_SUBWOOFER_ON},
    {aMsgOff,  NULL,   0x01,   SETUP_AUDIO_SUBWOOFER_OFF}
    };

#ifndef REMOVE_SETUP_SPEAKER_TEST
MENU_STRUCT   _MenuAudioSpeakerTest[]={
    {(WORD *   )NULL,  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_SPEAKER_TEST, 7, SETUP_AUDIO_SPEAKER_TEST_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, 7, SETUP_MENU_VALUE_W},
    //{aMsgOff,   NULL,   0x01,   SETUP_AUDIO_SPEAKER_TEST_OFF},
    //{NULL,      NULL,   0x03,   SETUP_AUDIO_SPEAKER_TEST_SEPARATE_LINE}, // the separate line
    {aMsgAutoTest,  NULL,   0x01,   SETUP_AUDIO_SPEAKER_TEST_AUTO},
    {aMsgLeft,  NULL,   0x01,   SETUP_AUDIO_SPEAKER_TEST_LEFT},
    {aMsgRight, NULL,   0x01,   SETUP_AUDIO_SPEAKER_TEST_RIGHT},
    {aMsgCenter,NULL,   0x01,   SETUP_AUDIO_SPEAKER_TEST_CENTER},
    {aMsgLS,    NULL,   0x01,   SETUP_AUDIO_SPEAKER_TEST_LS},
    {aMsgRS,    NULL,   0x01,   SETUP_AUDIO_SPEAKER_TEST_RS},
    {aMsgSW,    NULL,   0x01,   SETUP_AUDIO_SPEAKER_TEST_SW}
    };
#endif

#ifdef SUPPORT_DNR_ON_OFF // Brian1.25
MENU_STRUCT   _MenuAudioDNR[]={
    {(WORD *   )&(__SetupInfo.bAudioDNR),  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_DNR, 2, SETUP_AUDIO_DNR_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, 7, SETUP_MENU_VALUE_W},
    {aMsgOn,   NULL,   0x01,   SETUP_AUDIO_DNR_8},
    {aMsgOff,  NULL,   0x01,   SETUP_AUDIO_DNR_0}
    };
#else

// Brian0.91
MENU_STRUCT   _MenuAudioDNR[]={
    {(WORD *   )&(__SetupInfo.bAudioDNR),  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_DNR, 9, SETUP_AUDIO_DNR_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, 7, SETUP_MENU_VALUE_W},
    {aMsgDNR_0,   NULL,   0x01,   SETUP_AUDIO_DNR_0},
    {aMsgDNR_1,  NULL,   0x01,    SETUP_AUDIO_DNR_1},
    {aMsgDNR_2,   NULL,   0x01,   SETUP_AUDIO_DNR_2},
    {aMsgDNR_3,  NULL,   0x01,    SETUP_AUDIO_DNR_3},
    {aMsgDNR_4,   NULL,   0x01,   SETUP_AUDIO_DNR_4},
    {aMsgDNR_5,  NULL,   0x01,    SETUP_AUDIO_DNR_5},
    {aMsgDNR_6,   NULL,   0x01,   SETUP_AUDIO_DNR_6},
    {aMsgDNR_7,  NULL,   0x01,    SETUP_AUDIO_DNR_7},
    {aMsgDNR_8,   NULL,   0x01,   SETUP_AUDIO_DNR_8}
    };
#endif


BYTE _bEnableAudio[1];

#ifdef NO_DNR_ITEM
    #ifdef SUPPORT_2_CHANNEL_ONLY // No DNR and 2 channel only
        #define MENU_AUDIO_ITEMS    2
        #define MENU_AUDIO_ITEMS_WITH_PROLOGIC    2 // Brian1.08, 2 channel has no Pro Logic
    #else // No DNR
        #ifdef REMOVE_SETUP_SPEAKER_TEST
            #define MENU_AUDIO_ITEMS    6
            #define MENU_AUDIO_ITEMS_WITH_PROLOGIC    7 // Brian1.08
        #else
            #define MENU_AUDIO_ITEMS    7
            #define MENU_AUDIO_ITEMS_WITH_PROLOGIC    8 // Brian1.08
        #endif
    #endif // #ifdef SUPPORT_2_CHANNEL_ONLY
#else // NO_DNR_ITEM
    #ifdef SUPPORT_2_CHANNEL_ONLY // has DNR and 2 channel only
        #define MENU_AUDIO_ITEMS    3
        #define MENU_AUDIO_ITEMS_WITH_PROLOGIC    3 // Brian1.08, 2 channel has no Pro Logic
    #else // has DNR and 5.1 channel
        #ifdef REMOVE_SETUP_SPEAKER_TEST
            #define MENU_AUDIO_ITEMS    7
            #define MENU_AUDIO_ITEMS_WITH_PROLOGIC    8 // Brian1.08
        #else
            #define MENU_AUDIO_ITEMS    8
            #define MENU_AUDIO_ITEMS_WITH_PROLOGIC    9 // Brian1.08
        #endif
    #endif // #ifdef SUPPORT_2_CHANNEL_ONLY
#endif // NO_DNR_ITEM

MENU_STRUCT   _MenuAudio[]={
    {(WORD *   )_bEnableAudio,  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO, MENU_AUDIO_ITEMS, SETUP_AUDIO_BASE},
    {(WORD *   )SETUP_MENU_ITEM_X, (struct tagMENU_STRUCT   *)SETUP_MENU_ITEM_Y, 3, SETUP_MENU_ITEM_W},
    {aMsgDigitalOut,    (PMENU_STRUCT)&_MenuAudioDigitalOut,   0x0,   SETUP_AUDIO_DIGITAL_OUT},
#ifndef SUPPORT_2_CHANNEL_ONLY // Brian1.05
    {aMsgAnalogOut,  (PMENU_STRUCT)&_MenuAudioAnalogOut, 0x0, SETUP_AUDIO_ANALOG_OUT},
#endif
    {aMsgDownMix,    (PMENU_STRUCT)&_MenuAudioDownMix,   0x0, SETUP_AUDIO_DOWNMIX},
#ifndef SUPPORT_2_CHANNEL_ONLY // Brian1.05
    {aMsgCenter,        (PMENU_STRUCT)&_MenuAudioCenter,   0x0,   SETUP_AUDIO_CENTER},
    {aMsgRear,          (PMENU_STRUCT)&_MenuAudioRear,   0x0,   SETUP_AUDIO_REAR},
    {aMsgSubwoofer,     (PMENU_STRUCT)&_MenuAudioSubwoofer,   0x0,   SETUP_AUDIO_SUBWOOFER},
#ifndef REMOVE_SETUP_SPEAKER_TEST
    {aMsgSpeakerTest,   (PMENU_STRUCT)&_MenuAudioSpeakerTest,   0x0,   SETUP_AUDIO_SPEAKER_TEST} ,
#endif

#endif
#ifndef NO_DNR_ITEM
    {aMsgDNR,     (PMENU_STRUCT)&_MenuAudioDNR,   0x0,   SETUP_AUDIO_DNR}
#endif

    };

// Brian1.08, Audio menu with Pro Logic
MENU_STRUCT   _MenuAudio_With_Prologic[]={
    {(WORD *   )_bEnableAudio,  (struct tagMENU_STRUCT   *)SETUP_MENU_AUDIO_WITH_PROLOGIC, MENU_AUDIO_ITEMS_WITH_PROLOGIC, SETUP_AUDIO_BASE},
    {(WORD *   )SETUP_MENU_ITEM_X, (struct tagMENU_STRUCT   *)SETUP_MENU_ITEM_Y, 3, SETUP_MENU_ITEM_W},
    {aMsgDigitalOut,    (PMENU_STRUCT)&_MenuAudioDigitalOut,   0x0,   SETUP_AUDIO_DIGITAL_OUT},
#ifndef SUPPORT_2_CHANNEL_ONLY // Brian1.05
    {aMsgAnalogOut,  (PMENU_STRUCT)&_MenuAudioAnalogOut,   0x0,   SETUP_AUDIO_ANALOG_OUT},
#endif
    {aMsgDownMix,    (PMENU_STRUCT)&_MenuAudioDownMix,   0x0,   SETUP_AUDIO_DOWNMIX},
#ifndef SUPPORT_2_CHANNEL_ONLY // Brian1.05
    {aMsgPrologic ,     (PMENU_STRUCT)&_MenuAudioPrologic,   0x0,   SETUP_AUDIO_PROLOGIC},
    {aMsgCenter,        (PMENU_STRUCT)&_MenuAudioCenter,   0x0,   SETUP_AUDIO_CENTER},
    {aMsgRear,          (PMENU_STRUCT)&_MenuAudioRear,   0x0,   SETUP_AUDIO_REAR},
    {aMsgSubwoofer,     (PMENU_STRUCT)&_MenuAudioSubwoofer,   0x0,   SETUP_AUDIO_SUBWOOFER},
#ifndef REMOVE_SETUP_SPEAKER_TEST
    {aMsgSpeakerTest,   (PMENU_STRUCT)&_MenuAudioSpeakerTest,   0x0,   SETUP_AUDIO_SPEAKER_TEST},
#endif

#endif
#ifndef NO_DNR_ITEM
    {aMsgDNR,     (PMENU_STRUCT)&_MenuAudioDNR,   0x0,   SETUP_AUDIO_DNR}
#endif

    };

MENU_STRUCT   _MenuCustomParentalControl[]={
    {(WORD *   )&(__SetupInfo.bParentalCtl),  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM_PARENTAL_CONTROL, 9, SETUP_CUSTOM_PC_LEVEL_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgLevel1,    NULL,   0x01,   SETUP_CUSTOM_PC_LEVEL_1},
    {aMsgLevel2,    NULL,   0x01,   SETUP_CUSTOM_PC_LEVEL_2},
    {aMsgLevel3,    NULL,   0x01,   SETUP_CUSTOM_PC_LEVEL_3},
    {aMsgLevel4,    NULL,   0x01,   SETUP_CUSTOM_PC_LEVEL_4},
    {aMsgLevel5,    NULL,   0x01,   SETUP_CUSTOM_PC_LEVEL_5},
    {aMsgLevel6,    NULL,   0x01,   SETUP_CUSTOM_PC_LEVEL_6},
    {aMsgLevel7,    NULL,   0x01,   SETUP_CUSTOM_PC_LEVEL_7},
    {aMsgLevel8,    NULL,   0x01,   SETUP_CUSTOM_PC_LEVEL_8},
    {aMsgOff,       NULL,   0x01,   SETUP_CUSTOM_PC_LEVEL_OFF}
    };

#ifndef REMOVE_UI_SETTING
MENU_STRUCT   _MenuCustomUIStyle[]={
    {(WORD *   )&(__SetupInfo.bUIStyle),  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM_UI_STYLE, 4, SETUP_CUSTOM_UI_STYLE_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgUIStyle1,   NULL,   0x01,   SETUP_CUSTOM_UI_STYLE_1},
    {aMsgUIStyle2,   NULL,   0x01,   SETUP_CUSTOM_UI_STYLE_2},
    {aMsgUIStyle3,   NULL,   0x01,   SETUP_CUSTOM_UI_STYLE_3},
    {aMsgUIStyle4,   NULL,   0x01,   SETUP_CUSTOM_UI_STYLE_4},
    };
#endif

#if defined(SUPPORT_DVD_SMRTPB) && defined(SUPPORT_DVD_SMRTPB_SETUP)
MENU_STRUCT   _MenuCustomSmartPlay[]={
    {(WORD *   )&(__SetupInfo.bSmartPlay),  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM_SMART_PLAY, 2, SETUP_CUSTOM_SMART_PLAY_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgOn,   NULL,   0x01,   SETUP_CUSTOM_SMART_PLAY_ON},
    {aMsgOff,  NULL,   0x01,   SETUP_CUSTOM_SMART_PLAY_OFF}
    };
#endif

MENU_STRUCT   _MenuCustomAngleMark[]={
    {(WORD *   )&(__SetupInfo.bAngleMarkDisplay),  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM_ANGLE_MARK, 2, SETUP_CUSTOM_ANGLE_MARK_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgOn,   NULL,   0x01,   SETUP_CUSTOM_ANGLE_MARK_ON},
    {aMsgOff,  NULL,   0x01,   SETUP_CUSTOM_ANGLE_MARK_OFF}
    };

MENU_STRUCT   _MenuCustomDefaultValue[]={
    {(WORD *   )NULL,  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM_DEFAULT_VALUE, 1, SETUP_CUSTOM_DEFAULT_VALUE_BASE},
    {(WORD *   )SETUP_MENU_VALUE_X, (struct tagMENU_STRUCT   *)SETUP_MENU_VALUE_Y, SETUP_MENU_VALUE_R, SETUP_MENU_VALUE_W},
    {aMsgRead,   NULL,   0x01,   SETUP_CUSTOM_DEFAULT_VALUE_READ}
    };

BYTE _bEnableCustom[1];

MENU_STRUCT   _MenuCustom[]={

#ifndef NO_DRM
    #ifdef SUPPORT_DVD_SMRTPB 
        #ifndef REMOVE_UI_SETTING
            {(WORD *   )_bEnableCustom,  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM, 7, SETUP_CUSTOM_BASE},
        #else
            {(WORD *   )_bEnableCustom,  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM, 6, SETUP_CUSTOM_BASE},
        #endif
    #else
        #ifndef REMOVE_UI_SETTING
            {(WORD *   )_bEnableCustom,  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM, 6, SETUP_CUSTOM_BASE},
        #else
            {(WORD *   )_bEnableCustom,  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM, 5, SETUP_CUSTOM_BASE},
        #endif
    #endif
#else
    #if defined(SUPPORT_DVD_SMRTPB) && defined(SUPPORT_DVD_SMRTPB_SETUP)
        #ifndef REMOVE_UI_SETTING
            {(WORD *   )_bEnableCustom,  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM, 6, SETUP_CUSTOM_BASE},
        #else
            {(WORD *   )_bEnableCustom,  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM, 5, SETUP_CUSTOM_BASE},
        #endif
    #else
        #ifndef REMOVE_UI_SETTING
            {(WORD *   )_bEnableCustom,  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM, 5, SETUP_CUSTOM_BASE},
        #else
            {(WORD *   )_bEnableCustom,  (struct tagMENU_STRUCT   *)SETUP_MENU_CUSTOM, 4, SETUP_CUSTOM_BASE},
        #endif
    #endif
#endif // #ifndef NO_DRM

    {(WORD *   )SETUP_MENU_ITEM_X, (struct tagMENU_STRUCT   *)SETUP_MENU_ITEM_Y, SETUP_MENU_ITEM_R, SETUP_MENU_ITEM_W},
    {aMsgParental,          (PMENU_STRUCT)&_MenuCustomParentalControl,   0x00,   SETUP_CUSTOM_PARENTAL}, // need to verify password before pop-up submenu
    {aMsgChangePassword,    NULL,   0x02,   SETUP_CUSTOM_CHANGE_PASSWORD},
#ifndef REMOVE_UI_SETTING
    {aMsgUIStyle,           (PMENU_STRUCT)&_MenuCustomUIStyle,   0x0,   SETUP_CUSTOM_UI_STYLE},
#endif

#ifndef NO_DRM
    {aMsgDivXVOD,    NULL,   0x02,   SETUP_CUSTOM_DIVX_VOD},
#endif

#if defined(SUPPORT_DVD_SMRTPB) && defined(SUPPORT_DVD_SMRTPB_SETUP)
    {aMsgSmartPlay,         (PMENU_STRUCT)&_MenuCustomSmartPlay,   0x0,   SETUP_CUSTOM_SMART_PLAY},
#endif

    {aMsgAngleMark,         (PMENU_STRUCT)&_MenuCustomAngleMark,   0x0,   SETUP_CUSTOM_ANGLE_MARK},
    {aMsgDefaultValue,      (PMENU_STRUCT)&_MenuCustomDefaultValue,   0x0,   SETUP_CUSTOM_DEFAULT_VALUE}
    };

BYTE _bEnableMain[1];

MENU_STRUCT   _MenuMain[]={ // the main menu are all icons, the item text means 
    {(WORD *   )_bEnableMain,  (struct tagMENU_STRUCT   *)SETUP_MENU_MAIN, 5, SETUP_MAIN_BASE},
    {(WORD *   )SETUP_MENU_ROOT_X, (struct tagMENU_STRUCT   *)SETUP_MENU_ROOT_Y, SETUP_MENU_ROOT_R, SETUP_MENU_ROOT_W},
    {aMsgLanguageSetup,     (PMENU_STRUCT)&_MenuLanguage,   0x10,   SETUP_MAIN_LANGUAGE},
    {aMsgScreenSetup,       (PMENU_STRUCT)&_MenuScreen,     0x10,   SETUP_MAIN_SCREEN},
    {aMsgAudioSetup,        (PMENU_STRUCT)&_MenuAudio,      0x10,   SETUP_MAIN_AUDIO},
    {aMsgCustomSetup,       (PMENU_STRUCT)&_MenuCustom,     0x10,   SETUP_MAIN_CUSTOM},
    {aMsgExitSetup,         NULL,                           0x10,   SETUP_MAIN_EXIT}
    };

// Brian1.08, support runtime change menu for Audio Prologic
MENU_STRUCT   _MenuMain_With_Prologic[]={ // the main menu are all icons, the item text means 
    {(WORD *   )_bEnableMain,  (struct tagMENU_STRUCT   *)SETUP_MENU_MAIN_WITH_PROLOGIC, 5, SETUP_MAIN_BASE},
    {(WORD *   )SETUP_MENU_ROOT_X, (struct tagMENU_STRUCT   *)SETUP_MENU_ROOT_Y, SETUP_MENU_ROOT_R, SETUP_MENU_ROOT_W},
    {aMsgLanguageSetup,     (PMENU_STRUCT)&_MenuLanguage,   0x10,   SETUP_MAIN_LANGUAGE},
    {aMsgScreenSetup,       (PMENU_STRUCT)&_MenuScreen,     0x10,   SETUP_MAIN_SCREEN},
    {aMsgAudioSetup,        (PMENU_STRUCT)&_MenuAudio_With_Prologic,      0x10,   SETUP_MAIN_AUDIO},
    {aMsgCustomSetup,       (PMENU_STRUCT)&_MenuCustom,     0x10,   SETUP_MAIN_CUSTOM},
    {aMsgExitSetup,         NULL,                           0x10,   SETUP_MAIN_EXIT}
    };





// *** the sequence of _SetupMenuIndex[] must be exactly the same as MENU_ID
PMENU_STRUCT   _SetupMenuIndex[]=
{
            (PMENU_STRUCT)&_MenuMain,             //SETUP_MENU_MAIN
            (PMENU_STRUCT)&_MenuMain_With_Prologic,    //SETUP_MENU_MAIN_WITH_PROLOGIC  // Brian1.08
            (PMENU_STRUCT)&_MenuLanguage,         //SETUP_MENU_LANGUAGE
            (PMENU_STRUCT)&_MenuScreen,           //SETUP_MENU_SCREEN
            (PMENU_STRUCT)&_MenuAudio,            //SETUP_MENU_AUDIO
            (PMENU_STRUCT)&_MenuAudio_With_Prologic,            //SETUP_MENU_AUDIO_WITH_PROLOGIC    // Brian1.08
            (PMENU_STRUCT)&_MenuCustom,           //SETUP_MENU_CUSTOM
            (PMENU_STRUCT)&_MenuLanguageOSD,      //SETUP_MENU_LANGUAGE_OSD
            (PMENU_STRUCT)&_MenuLanguageMenu,     //SETUP_MENU_LANGUAGE_MENU
            (PMENU_STRUCT)&_MenuLanguageAudio,    //SETUP_MENU_LANGUAGE_AUDIO
            (PMENU_STRUCT)&_MenuLanguageSP,       //SETUP_MENU_LANGUAGE_SP
/*
#ifdef SUPPORT_CHAR_SUBPICTURE
#ifndef NO_CHAR_SUBPICTURE_MENU
            (PMENU_STRUCT)&_MenuLanguageEx_SP,    //SETUP_MENU_LANGUAGE_EX_SP
#endif
#endif
*/
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
            (PMENU_STRUCT)&_MenuLanguageEncoding,    //SETUP_MENU_LANGUAGE_ENCODING
//#endif
            (PMENU_STRUCT)&_MenuScreenTVType,     //SETUP_MENU_SCREEN_TV_TYPE
#ifndef NO_SCREEN_SAVER // Brian1.20
            (PMENU_STRUCT)&_MenuScreenSaver,      //SETUP_MENU_SCREEN_SAVER
#endif
            (PMENU_STRUCT)&_MenuScreenTVSystem,   //SETUP_MENU_SCREENTV_SYSTEM
#ifndef SUPPORT_TFT
            (PMENU_STRUCT)&_MenuScreenVideoOutput,//SETUP_MENU_SCREEN_VIDEO_OUTPUT
#endif
            (PMENU_STRUCT)&_MenuProgressiveScan,    //SETUP_MENU_SCREEN_PROGRESSIVE_SCAN
#ifndef SUPPORT_TFT
            (PMENU_STRUCT)&_MenuScreenBrightness, //SETUP_MENU_SCREEN_BRIGHTNESS
            (PMENU_STRUCT)&_MenuScreenContrast,   //SETUP_MENU_SCREEN_CONTRAST
#endif
#ifdef SUPPORT_ADJUST_CHROM_PHASE // Brian1.25
            (PMENU_STRUCT)&_MenuScreenChromPhase,   //SETUP_MENU_SCREEN_CONTRAST
#endif
            (PMENU_STRUCT)&_MenuAudioDigitalOut,  //SETUP_MENU_AUDIO_DIGITAL_OUT
            (PMENU_STRUCT)&_MenuAudioAnalogOut,   //SETUP_MENU_AUDIO_ANALOG_OUT
            (PMENU_STRUCT)&_MenuAudioDownMix,   //SETUP_MENU_AUDIO_DOWNMIX
            (PMENU_STRUCT)&_MenuAudioPrologic,    //SETUP_MENU_AUDIO_PROLOGIC     // Brian1.08
            (PMENU_STRUCT)&_MenuAudioCenter,      //SETUP_MENU_AUDIO_CENTER
            (PMENU_STRUCT)&_MenuAudioRear,        //SETUP_MENU_AUDIO_REAR
            (PMENU_STRUCT)&_MenuAudioSubwoofer,   //SETUP_MENU_AUDIO_SUBWOOFER
#ifndef REMOVE_SETUP_SPEAKER_TEST
            (PMENU_STRUCT)&_MenuAudioSpeakerTest, //SETUP_MENU_AUDIO_SPEAKER_TEST
#endif
            (PMENU_STRUCT)&_MenuAudioDNR,         //SETUP_MENU_AUDIO_DNR
            (PMENU_STRUCT)&_MenuCustomParentalControl, //SETUP_MENU_CUSTOM_PARENTAL_CONTROL
#ifndef REMOVE_UI_SETTING
            (PMENU_STRUCT)&_MenuCustomUIStyle,    //SETUP_MENU_CUSTOM_UI_STYLE
#endif
#if defined(SUPPORT_DVD_SMRTPB) && defined(SUPPORT_DVD_SMRTPB_SETUP)
            (PMENU_STRUCT)&_MenuCustomSmartPlay,  //SETUP_MENU_CUSTOM_SMART_PLAY
#endif
            (PMENU_STRUCT)&_MenuCustomAngleMark,  //SETUP_MENU_CUSTOM_ANGLE_MARK
            (PMENU_STRUCT)&_MenuCustomDefaultValue, //SETUP_MENU_CUSTOM_DEFAULT_VALUE
};


#else

extern BYTE    __bCurrentMenuId; // replace _pCurrentMenu
extern WORD    __wMENUCurrentItemId, __wMENUPrevItemId;
extern BYTE    __bMENUCurrentLevel;

#ifdef SUPPORT_MENU_CURSOR_STAY_AT_VALUE_AFTER_ENTER
extern WORD    __wMENUItemIdAfterEnter; // Brian1.08a, the Item ID after key Enter
#endif


extern WORD    _wMENUTemp; // this extern is just for Enable/Disable Menu Item
extern BYTE    _bMENUTemp, _bMENUTemp2; // this extern is just for Enable/Disable Menu Item



extern BYTE _bEnableLanguage[];
extern BYTE _bEnableScreen[];
extern BYTE _bEnableAudio[];
extern BYTE _bEnableCustom[];
extern BYTE _bEnableMain[];

#endif //#ifdef MENU_DECLARATION 



extern void OSDMENU_Initial(void);
extern void OSDMENU_DisplayMenu(BYTE bMenuId, BYTE bItem, BYTE bLevel);
extern void OSDMENU_DrawCheckMark(BYTE bLocation, BYTE bColor, BYTE bCheck);

extern void OSDMENU_MenuOperation(BYTE bKey);
extern void OSDMENU_SetMenuPalEntry(void);
extern void OSDSETUP_PasswordDlg(void);
extern void OSDSETUP_ChangePasswordDlg(void);


extern BYTE OSDSETUP_ChangePassword(BYTE bKey);
extern void OSDSETUP_ShowHelpText(BYTE bID);
extern void OSDMENU_SetItemEnable(void);
extern void OSDSETUP_LanguageCodeDlg(void);
extern BYTE    OSDSETUP_InputLanguageCode(BYTE bKey);
extern void OSDSETUP_RedrawMenu(void);

extern void OSDMENU_DisplayTheMenu(void);
extern void OSDMENU_RedrawAll(void);

extern void OSDPASSWORD_Exit(BYTE bExitMode);
extern BYTE OSDPASSWORD_Recover(BYTE bRecoverRegion);
extern void OSDPASSWORD_ConfigOSDRegion(void);



#endif  // __OSD3_H__

