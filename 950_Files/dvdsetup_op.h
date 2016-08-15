
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////
//Transform the ID
#define SETUP_SCREEN_TV_SYSTEM_BEGIN                    SETUP_DISPLAY_TV_STANDARD_BEGIN
#define SETUP_SCREEN_TV_SYSTEM_NTSC                     SETUP_DISPLAY_TV_STANDARD_NTSC 
#define SETUP_SCREEN_TV_SYSTEM_PAL                      SETUP_DISPLAY_TV_STANDARD_PAL 
#define SETUP_SCREEN_TV_SYSTEM_AUTO                     SETUP_DISPLAY_TV_STANDARD_AUTO 
#define SETUP_SCREEN_TV_SYSTEM_END                      SETUP_DISPLAY_TV_STANDARD_END

////////////////////////////////////////////////////////////////////////////////////////////////////////

//define deafult value
#ifndef MUSTEK_SETUP_MENU
#define SETUP_PHOTO_SETTING_DEFAULT_INTERVAL_TIME       SETUP_PHOTO_SETTING_INTERVAL_TIME_10_SEC   
#else //MUSTEK_SETUP_MENU
#define SETUP_PHOTO_SETTING_DEFAULT_INTERVAL_TIME       SETUP_PHOTO_SETTING_INTERVAL_TIME_15_SEC   
#endif //MUSTEK_SETUP_MENU
#define SETUP_PHOTO_SETTING_DEFAULT_TRANSITION_EFFECT   SETUP_PHOTO_SETTING_TRANSITION_EFFECT_OFF
#define SETUP_PHOTO_SETTING_DEFAULT_BROWSE              SETUP_PHOTO_SETTING_BROWSE_THUMBNAIL

#define SETUP_AUTO_PLAY_DEFAULT_PHOTO                   SETUP_AUTO_PLAY_PHOTO_OFF
#define SETUP_AUTO_PLAY_DEFAULT_MUSIC                   SETUP_AUTO_PLAY_MUSIC_OFF
#define SETUP_AUTO_PLAY_DEFAULT_PHOTO_PLUS_MUSIC        SETUP_AUTO_PLAY_PHOTO_PLUS_MUSIC_OFF
#define SETUP_AUTO_PLAY_DEFAULT_MOVIE                   SETUP_AUTO_PLAY_MOVIE_OFF

#ifndef MUSTEK_SETUP_MENU
#define SETUP_DISPLAY_DEFAULT_TV_STANDARD               SETUP_DISPLAY_TV_STANDARD_AUTO
#else //MUSTEK_SETUP_MENU
#define SETUP_DISPLAY_DEFAULT_TV_STANDARD               SETUP_DISPLAY_TV_STANDARD_NTSC
#endif //MUSTEK_SETUP_MENU
#define SETUP_DISPLAY_DEFAULT_DISPLAY_TYPE              SETUP_DISPLAY_TYPE_16_9
#define SETUP_DISPLAY_DEFAULT_BRIGHTNESS                SETUP_DISPLAY_BRIGHTNESS_NORMAL
#define SETUP_DISPLAY_DEFAULT_CONTRAST                  SETUP_DISPLAY_CONTRAST_0

#define SETUP_CUSTOM_DEFAULT_OSD                        SETUP_CUSTOM_OSD_ENGLISH
#define SETUP_CUSTOM_DEFAULT_UI_STYLE                   SETUP_CUSTOM_UI_STYLE_3
*/

extern void DVDSETUP_SetDefaultState(void);
extern void DVDSETUP_SaveDataToSetupInfo(BYTE bMenuID);
extern void DVDSETUP_Exit_AdjustSetting(void);
extern void DVDSETUP_InitStatus(void);

void SETUP_LoadLanguage(void);

#ifdef IMAGE_FRAME_SETUP
void _SetDisplayToIC(void);
WORD DVDSETUP_GetVolume(void);
void DVDSETUP_SetDefaultTFTAttribute(void);
void DVDSETUP_SaveVolume(void);
#endif