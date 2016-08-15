#include "winav.h"
#include "utl.h"
#include "gdi.h"
#include "osddsply.h"
#include "osdnd.h"
#include "input.h"
#include "cc.h"
#include "osd.h"
#include "disp.h"
#include "osdss.h"
#include "hal.h"
#include "initial.h"
#include "infofilter.h"
#include "mmanager.h"
#include "poweronmenu.h"
#include "storage.h"
#include "dvdsetup.h"
#include "mainmenu.h"
#include "notransl.h"
#include "OSDString\\strextrn.h"
#include "fontindex.h"
#ifdef SUPPORT_RADIO_TUNER
#include "radio.h"
#endif //SUPPORT_RADIO_TUNER
#include "clock.h"
#include "calenui.h"
#include "alarm.h"
#include "autopower.h"
#include "media.h"
#include "srcfilter.h"
#include "jpegdec.h"
#include "edit.h"

#ifdef SUPPORT_PHOTO_ICON
const BYTE _aPOWERPNMENU_Photo[]=
{
#include "bmp/Menu_Photo.txt"
};

const BYTE _aPOWERPNMENU_Photo_H[]=
{
#include "bmp/Menu_Photo_H.txt"
};

#endif //SUPPORT_PHOTO_ICON

#ifdef SUPPORT_MUSIC_ICON
const BYTE _aPOWERPNMENU_Music[]=
{
#include "bmp/Menu_Music.txt"
};

const BYTE _aPOWERPNMENU_Music_H[]=
{
#include "bmp/Menu_Music_H.txt"
};

#endif //SUPPORT_MUSIC_ICON

#ifdef SUPPORT_PHOTO_MUSIC_ICON
const BYTE _aPOWERPNMENU_Photo_Audio[]=
{
#include "bmp/Menu_Photo_Audio.txt"
};

const BYTE _aPOWERPNMENU_Photo_Audio_H[]=
{
#include "bmp/Menu_Photo_Audio_H.txt"
};

#endif //SUPPORT_PHOTO_MUSIC_ICON

#ifdef SUPPORT_MOVIE_ICON
const BYTE _aPOWERPNMENU_Movie[]=
{
#include "bmp/Menu_Movie.txt"
};

const BYTE _aPOWERPNMENU_Movie_H[]=
{
#include "bmp/Menu_Movie_H.txt"
};

#endif //SUPPORT_MOVIE_ICON

#ifdef SUPPORT_SETUP_ICON
const BYTE _aPOWERPNMENU_Setup[]=
{
#include "bmp/Menu_Setup.txt"
};

const BYTE _aPOWERPNMENU_Setup_H[]=
{
#include "bmp/Menu_Setup_H.txt"
};

#endif //SUPPORT_SETUP_ICON

#ifdef SUPPORT_CALENDAR_ICON
const BYTE _aPOWERPNMENU_Calender[]=
{
#include "bmp/Menu_Calender.txt"
};

const BYTE _aPOWERPNMENU_Calender_H[]=
{
#include "bmp/Menu_Calender_H.txt"
};
#endif //SUPPORT_CALENDAR_ICON

#ifdef SUPPORT_CLOCK_ICON
const BYTE _aPOWERPNMENU_Clock[]=
{
#include "bmp/Menu_Clock.txt"
};

const BYTE _aPOWERPNMENU_Clock_H[]=
{
#include "bmp/Menu_Clock_H.txt"
};

#endif //SUPPORT_CLOCK_ICON

#ifdef SUPPORT_ALARM_ICON
const BYTE _aPOWERPNMENU_Alarm[]=
{
#include "bmp/Menu_Alarm.txt"
};

const BYTE _aPOWERPNMENU_Alarm_H[]=
{
#include "bmp/Menu_Alarm_H.txt"
};

#endif //SUPPORT_ALARM_ICON

#ifdef SUPPORT_AUTO_POWER_ICON
const BYTE _aPOWERPNMENU_AutoPower[]=
{
#include "bmp/Menu_AutoPower.txt"
};

const BYTE _aPOWERPNMENU_AutoPower_H[]=
{
#include "bmp/Menu_AutoPower_H.txt"
};

#endif //SUPPORT_AUTO_POWER_ICON

#ifdef SUPPORT_EDIT_ICON
const BYTE _aPOWERPNMENU_Edit[]=
{
#include "bmp/Menu_Edit.txt"
};

const BYTE _aPOWERPNMENU_Edit_H[]=
{
#include "bmp/Menu_Edit_H.txt"
};

#endif //SUPPORT_EDIT_ICON

#ifdef SUPPORT_FAVORITE_ICON
const BYTE _aPOWERPNMENU_Favorite[]=
{
#include "bmp/Menu_Favor.txt"
};

const BYTE _aPOWERPNMENU_Favorite_H[]=
{
#include "bmp/Menu_Favor_H.txt"
};

#endif //SUPPORT_FAVORITE_ICON

#ifdef SUPPORT_RADIO_TUNER_ICON
const BYTE _aPOWERPNMENU_Radio[]=
{
#include "bmp/Menu_Radio.txt"
};

const BYTE _aPOWERPNMENU_Radio_H[]=
{
#include "bmp/Menu_Radio_H.txt"
};

#endif //SUPPORT_RADIO_TUNER_ICON

#ifdef SUPPORT_STB_ICON
const BYTE _aPOWERPNMENU_STB[]=
{
#include "bmp/Menu_STB.txt"
};

const BYTE _aPOWERPNMENU_STB_H[]=
{
#include "bmp/Menu_STB_H.txt"
};
#endif //SUPPORT_STB_ICON

#ifdef SUPPORT_GAME_ICON
const BYTE _aPOWERPNMENU_Game[]=
{
#include "bmp/Menu_Game.txt"
};

const BYTE _aPOWERPNMENU_Game_H[]=
{
#include "bmp/Menu_Game_H.txt"
};
#endif //SUPPORT_GAME_ICON

const BYTE *_aPOWERONMENUBMPArray[POWERONMENU_TOTAL_ICON*2] = {
#ifdef SUPPORT_PHOTO_ICON
    _aPOWERPNMENU_Photo,                // 0
#endif //SUPPORT_PHOTO_ICON
#ifdef SUPPORT_MUSIC_ICON
    _aPOWERPNMENU_Music,                // 1
#endif //SUPPORT_MUSIC_ICON
#ifdef SUPPORT_PHOTO_MUSIC_ICON
    _aPOWERPNMENU_Photo_Audio,          // 2
#endif //SUPPORT_PHOTO_MUSIC_ICON
#ifdef SUPPORT_MOVIE_ICON
    _aPOWERPNMENU_Movie,                // 3
#endif //SUPPORT_MOVIE_ICON
#ifdef SUPPORT_SETUP_ICON
    _aPOWERPNMENU_Setup,                // 4
#endif //SUPPORT_SETUP_ICON
#ifdef SUPPORT_CALENDAR_ICON
    _aPOWERPNMENU_Calender,             // 5
#endif //SUPPORT_CALENDAR_ICON
#ifdef SUPPORT_CLOCK_ICON
    _aPOWERPNMENU_Clock,                // 6
#endif //SUPPORT_CLOCK_ICON
#ifdef SUPPORT_ALARM_ICON
    _aPOWERPNMENU_Alarm,                // 7
#endif //SUPPORT_ALARM_ICON
#ifdef SUPPORT_AUTO_POWER_ICON
    _aPOWERPNMENU_AutoPower,            // 8
#endif //SUPPORT_AUTO_POWER_ICON
#ifdef SUPPORT_STB_ICON
    _aPOWERPNMENU_STB,                  // 9
#endif //SUPPORT_STB_ICON
#ifdef SUPPORT_EDIT_ICON
    _aPOWERPNMENU_Edit,                 // 10
#endif //SUPPORT_EDIT_ICON
#ifdef SUPPORT_FAVORITE_ICON
    _aPOWERPNMENU_Favorite,             // 11
#endif //SUPPORT_FAVORITE_ICON
#ifdef SUPPORT_GAME_ICON
    _aPOWERPNMENU_Game,                 //12
#endif //SUPPORT_GAME_ICON
#ifdef SUPPORT_RADIO_TUNER_ICON
    _aPOWERPNMENU_Radio,                //13
#endif //SUPPORT_RADIO_TUNER_ICON
#ifdef SUPPORT_PHOTO_ICON
    _aPOWERPNMENU_Photo_H,              // 0
#endif //SUPPORT_PHOTO_ICON
#ifdef SUPPORT_MUSIC_ICON
    _aPOWERPNMENU_Music_H,              // 1
#endif //SUPPORT_MUSIC_ICON
#ifdef SUPPORT_PHOTO_MUSIC_ICON
    _aPOWERPNMENU_Photo_Audio_H,        // 2
#endif //SUPPORT_PHOTO_MUSIC_ICON
#ifdef SUPPORT_MOVIE_ICON
    _aPOWERPNMENU_Movie_H,              // 3
#endif //SUPPORT_MOVIE_ICON
#ifdef SUPPORT_SETUP_ICON
    _aPOWERPNMENU_Setup_H,              // 4
#endif //SUPPORT_SETUP_ICON
#ifdef SUPPORT_CALENDAR_ICON
    _aPOWERPNMENU_Calender_H,           // 5
#endif //SUPPORT_CALENDAR_ICON
#ifdef SUPPORT_CLOCK_ICON
    _aPOWERPNMENU_Clock_H,              // 6
#endif //SUPPORT_CLOCK_ICON
#ifdef SUPPORT_ALARM_ICON
    _aPOWERPNMENU_Alarm_H,              // 7
#endif //SUPPORT_ALARM_ICON
#ifdef SUPPORT_AUTO_POWER_ICON
    _aPOWERPNMENU_AutoPower_H,          // 8
#endif //SUPPORT_AUTO_POWER_ICON
#ifdef SUPPORT_STB_ICON
    _aPOWERPNMENU_STB_H,                // 9
#endif //SUPPORT_STB_ICON
#ifdef SUPPORT_EDIT_ICON
    _aPOWERPNMENU_Edit_H,               // 10 
#endif //SUPPORT_EDIT_ICON
#ifdef SUPPORT_FAVORITE_ICON
    _aPOWERPNMENU_Favorite_H,           // 11
#endif //SUPPORT_FAVORITE_ICON
#ifdef SUPPORT_GAME_ICON
    _aPOWERPNMENU_Game_H,               // 12
#endif //SUPPORT_GAME_ICON
#ifdef SUPPORT_RADIO_TUNER_ICON
    _aPOWERPNMENU_Radio_H,              //13
#endif //SUPPORT_RADIO_TUNER_ICON
};

const DWORD _aPOWERONMENU_Palette[]=        // Palette 2 data, for button bitmap
{
#include "bmp/palPowerOnMenu.txt"       // bit 8/24 indicate Mix_Enable
};

const POWER_ON_MENU_ICON PowerOnMenuIcon[POWERONMENU_TOTAL_ICON] = {
#ifdef SUPPORT_PHOTO_ICON
    {POWERONMENU_ICON_ID_PHOTO, POWERONMENU_ICON_PALETTE_ENTRY_PHOTO_ICON, POWERONMENU_ICON_PALETTE_ENTRY_PHOTO_H_ICON, POWERONMENU_ICON_PHOTO_POS_X, POWERONMENU_ICON_PHOTO_POS_Y},
#endif //SUPPORT_PHOTO_ICON
#ifdef SUPPORT_MUSIC_ICON
    {POWERONMENU_ICON_ID_MUSIC, POWERONMENU_ICON_PALETTE_ENTRY_MUSIC_ICON, POWERONMENU_ICON_PALETTE_ENTRY_MUSIC_H_ICON, POWERONMENU_ICON_MUSIC_POS_X, POWERONMENU_ICON_MUSIC_POS_Y},
#endif //SUPPORT_MUSIC_ICON
#ifdef SUPPORT_PHOTO_MUSIC_ICON
    {POWERONMENU_ICON_ID_PHOTO_PLUS_MUSIC, POWERONMENU_ICON_PALETTE_ENTRY_PHOTO_MUSIC_ICON, POWERONMENU_ICON_PALETTE_ENTRY_PHOTO_MUSIC_H_ICON, POWERONMENU_ICON_PHOTO_MUSIC_POS_X, POWERONMENU_ICON_PHOTO_MUSIC_POS_Y},
#endif //SUPPORT_PHOTO_MUSIC_ICON
#ifdef SUPPORT_MOVIE_ICON
    {POWERONMENU_ICON_ID_MOVIE, POWERONMENU_ICON_PALETTE_ENTRY_MOVIE_ICON, POWERONMENU_ICON_PALETTE_ENTRY_MOVIE_H_ICON, POWERONMENU_ICON_MOVIE_POS_X, POWERONMENU_ICON_MOVIE_POS_Y},
#endif //SUPPORT_MOVIE_ICON
#ifdef SUPPORT_SETUP_ICON
    {POWERONMENU_ICON_ID_SETUP, POWERONMENU_ICON_PALETTE_ENTRY_SETUP_ICON, POWERONMENU_ICON_PALETTE_ENTRY_SETUP_H_ICON, POWERONMENU_ICON_SETUP_POS_X, POWERONMENU_ICON_SETUP_POS_Y},
#endif //SUPPORT_SETUP_ICON
#ifdef SUPPORT_CALENDAR_ICON
    {POWERONMENU_ICON_ID_CALENDAR, POWERONMENU_ICON_PALETTE_ENTRY_CALENDAR_ICON, POWERONMENU_ICON_PALETTE_ENTRY_CALENDAR_H_ICON, POWERONMENU_ICON_CALENDAR_POS_X, POWERONMENU_ICON_CALENDAR_POS_Y},
#endif //SUPPORT_CALENDAR_ICON
#ifdef SUPPORT_CLOCK_ICON
    {POWERONMENU_ICON_ID_CLOCK, POWERONMENU_ICON_PALETTE_ENTRY_CLOCK_ICON, POWERONMENU_ICON_PALETTE_ENTRY_CLOCK_H_ICON, POWERONMENU_ICON_CLOCK_POS_X, POWERONMENU_ICON_CLOCK_POS_Y},
#endif //SUPPORT_CLOCK_ICON
#ifdef SUPPORT_ALARM_ICON
    {POWERONMENU_ICON_ID_ALARM, POWERONMENU_ICON_PALETTE_ENTRY_ALARM_ICON, POWERONMENU_ICON_PALETTE_ENTRY_ALARM_H_ICON, POWERONMENU_ICON_ALARM_POS_X, POWERONMENU_ICON_ALARM_POS_Y},
#endif //SUPPORT_ALARM_ICON
#ifdef SUPPORT_AUTO_POWER_ICON
    {POWERONMENU_ICON_ID_AUTO_POWER, POWERONMENU_ICON_PALETTE_ENTRY_AUTO_POWER_ICON, POWERONMENU_ICON_PALETTE_ENTRY_AUTO_POWER_H_ICON, POWERONMENU_ICON_AUTO_POWER_POS_X, POWERONMENU_ICON_AUTO_POWER_POS_Y},
#endif //SUPPORT_AUTO_POWER_ICON
#ifdef SUPPORT_STB_ICON
    {POWERONMENU_ICON_ID_STB, POWERONMENU_ICON_PALETTE_ENTRY_STB_ICON, POWERONMENU_ICON_PALETTE_ENTRY_STB_H_ICON, POWERONMENU_ICON_STB_POS_X, POWERONMENU_ICON_STB_POS_Y},
#endif //SUPPORT_STB_ICON
#ifdef SUPPORT_EDIT_ICON
    {POWERONMENU_ICON_ID_EDIT, POWERONMENU_ICON_PALETTE_ENTRY_EDIT_ICON, POWERONMENU_ICON_PALETTE_ENTRY_EDIT_H_ICON, POWERONMENU_ICON_EDIT_POS_X, POWERONMENU_ICON_EDIT_POS_Y},
#endif //SUPPORT_EDIT_ICON
#ifdef SUPPORT_FAVORITE_ICON
    {POWERONMENU_ICON_ID_FAVORITE, POWERONMENU_ICON_PALETTE_ENTRY_FAVORITE_ICON, POWERONMENU_ICON_PALETTE_ENTRY_FAVORITE_H_ICON, POWERONMENU_ICON_FAVORITE_POS_X, POWERONMENU_ICON_FAVORITE_POS_Y},
#endif //SUPPORT_FAVORITE_ICON
#ifdef SUPPORT_GAME_ICON
    {POWERONMENU_ICON_ID_GAME, POWERONMENU_ICON_PALETTE_ENTRY_GAME_ICON, POWERONMENU_ICON_PALETTE_ENTRY_GAME_H_ICON, POWERONMENU_ICON_GAME_POS_X, POWERONMENU_ICON_GAME_POS_Y},
#endif //SUPPORT_GAME_ICON
#ifdef SUPPORT_RADIO_TUNER_ICON
    {POWERONMENU_ICON_ID_RADIO_TUNER, POWERONMENU_ICON_PALETTE_ENTRY_RADIO_TUNER_ICON, POWERONMENU_ICON_PALETTE_ENTRY_RADIO_TUNER_H_ICON, POWERONMENU_ICON_RADIO_POS_X, POWERONMENU_ICON_RADIO_POS_Y},
#endif //SUPPORT_RADIO_TUNER_ICON
};

BYTE _bPOWERONMENUCursorIndex;
BYTE __bPOWERONMENUInitial = FALSE;
BYTE _bPOWERONMENUSubUIMode;
BYTE _bPOWERONMENUShowPlayableFile = FALSE; //CoCo1.11

extern void MEDIA_USB(void);
extern BYTE __bMMExtFilterMode;
extern BYTE __bMMAutoPlay;
extern IMAGE_FRAME_SETUPINFO __ImageFrameSetupInfo;
extern BYTE __bLOGO;

extern void MEDIA_ClearUSBSwitchState(void);

//***************************************************************************
//  Function    :   _POWERONMENU_InitializeGDI
//  Abstract    :   Initialize the palette for the power on menu.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _POWERONMENU_InitializeGDI(void)
{
    //Initialize the text palettes
    GDI_ChangePALEntry(POWERONMENU_PALETTE_ENTRY_ICON_NORMAL_TEXT, POWERONMENU_PALETTE_COLOR_ICON_NORMAL_TEXT, FALSE);
    GDI_ChangePALEntry(POWERONMENU_PALETTE_ENTRY_ICON_HIGHLIGHT_TEXT, POWERONMENU_PALETTE_COLOR_ICON_HIGHLIGHT_TEXT, FALSE);

    GDI_WaitPaletteComplete();
}

void POWERONMENU_Initial(void)
{
    //DWORD dwSaveInt;

    if (__bPOWERONMENUInitial)
        return;

    /* CoCo, set "__bLOGO = 0" will enable display after displaying LOGO.
    //Enable the display always.
    HAL_DISABLE_INTERRUPTS( dwSaveInt );
    __dwVideo_en |= DISP_VIDEO_READY;
    HAL_RESTORE_INTERRUPTS( dwSaveInt );

    DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
    */

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    if (__bMMJPEGAudioMode)
    {
        MM_ExitJPEGAudioMode();
    }
#endif //SUPPORT_JPEG_AUDIO_PLAYING

 // wyc2.38-950, stop and reset variables when back to menu.
    CC_KeyCommand(KEY_STOP);
        //MEDIA_Removed(SOURCE_DISC);
    // wyc1.05-909, reset state to open/close and show LOGO when media removed.
    CC_ResetState( KEY_OPEN_CLOSE );
    
    OSD_ChangeUI(OSD_UI_POWERON_MENU, OSD_ENTER_UI);
   
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM    
    CC_ForceStop(CC_FORCE_STOP_FROM_POWERON_MENU);
#endif

#ifdef SUPPORT_POWER_SWITCH
    if (!__bPowerSw_StandBy)
		UTL_ShowLogo();
#else
    UTL_ShowLogo();
#endif //SUPPORT_POWER_SWITCH
	
    MEDIA_ClearUSBSwitchState();
	
#ifdef SUPPORT_POWER_SWITCH
    if (!__bPowerSw_StandBy)
        DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);   
#endif //SUPPORT_POWER_SWITCH
	
#ifdef POWERON_STANDBY
    DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
#endif //POWERON_STANDBY
	
    _bPOWERONMENUCursorIndex = 0;
    __bPOWERONMENUInitial = TRUE;
    _bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_MODE_NONE;

    //CoCo2.51
    //DISP_DisplayCtrl(DISP_TFT_MODE, FALSE);

    _POWERONMENU_DrawAllUI();

    OSDSS_ResetTime();
}

void _POWERONMENU_DrawAllUI(void)
{
    if (__bLOGO == LOGO_NONE)
    {
        DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
        UTL_ShowLogo();
    }
	
    POWERONMENU_ConfigOSDRegion();

    _POWERONMENU_InitializeGDI();
     #ifdef SUPPORT_SYSTEM_SHORTCUT_KEY
     if(Disable_Init_Menu())
   	return;
    #endif	

    _POWERONMENU_ShowIcon();

    _POWERONMENU_UpdateCursor(_bPOWERONMENUCursorIndex, FALSE);
}

//*********************************************************************************************************
//  Function    :   POWERONMENU_ProcessKey
//  Abstract    :   Call this function to process the keys.
//  Arguments   :   none.
//  Return		:   KEY_NO_KEY: The input key is processed.
//              :   INVALID_KEY: The input key is not allowed.
//              :   KEY_BYPASS: Will not process this key and pass it to main flow to process it.
//              :   KEY_XXX: The main program need do more process for the key.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
#ifdef SUPPORT_SYSTEM_SHORTCUT_KEY
BYTE    DisInitMenuFlag=FALSE;
BYTE Shortcut_Key_Entry(BYTE  bkey)
{
      switch(bkey)
      {
		case KEY_PHOTO:
			_POWERONMENU_EnterPhotoMode();     
			bkey=KEY_NO_KEY;
			break;
		case KEY_MUSIC:
			_POWERONMENU_EnterMusicMode();
			bkey=KEY_NO_KEY;
			break;
		case KEY_PHOTO_MUSIC:
			_POWERONMENU_EnterPhotoMusicMode();
			bkey=KEY_NO_KEY;
			break;
		case KEY_MOVIE:
			_POWERONMENU_EnterMovieMode();
			bkey=KEY_NO_KEY;
			break;
		case KEY_DATE:
			_bPOWERONMENUCursorIndex=POWERONMENU_ICON_ID_CALENDAR;
			_bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_CALENDAR;
			CALENDAR_Entry(CALENDAR_MODE_DISPLAY_CLOCK_AND_TIME);
			bkey=KEY_NO_KEY;
			break;
		case KEY_TIME:
			_bPOWERONMENUCursorIndex=POWERONMENU_ICON_ID_CLOCK;
			_bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_CLOCK;
		 	DisInitMenuFlag=TRUE;
			CLOCK_Entry();
			DisInitMenuFlag=FALSE;
			bkey=KEY_NO_KEY;
			break;
		default:
			break;
      }
	return bkey;
}

BYTE POWERONMENU_CheckKey(BYTE  Prekey)
{
      if(__bPOWERONMENUInitial)
     {
		if((_bPOWERONMENUSubUIMode>=POWERONMENU_SUB_UI_CALENDAR)&&(_bPOWERONMENUSubUIMode<=POWERONMENU_SUB_UI_EDIT))
		{     extern BYTE _bCALENDARMode;
		       extern BYTE _bCLOCKMode;;
			 switch(Prekey)
      			{
				case KEY_PHOTO:
				case KEY_MUSIC:
				case KEY_PHOTO_MUSIC:
				case KEY_MOVIE:
					POWERONMENU_Exit(OSD_UI_EXIT_ALL);
					break;
				case KEY_DATE:
					if((_bPOWERONMENUCursorIndex==POWERONMENU_ICON_ID_CALENDAR)&&(_bPOWERONMENUSubUIMode == POWERONMENU_SUB_UI_CALENDAR)&&(_bCALENDARMode==CALENDAR_MODE_DISPLAY_CLOCK_AND_TIME))
					{
						Prekey=KEY_INVALID;
					}
					else
					{
						POWERONMENU_Exit(OSD_UI_EXIT_ALL);
					}
					break;
				case KEY_TIME:
					if((_bPOWERONMENUCursorIndex==POWERONMENU_ICON_ID_CLOCK)&&(_bPOWERONMENUSubUIMode == POWERONMENU_SUB_UI_CLOCK)&&(_bCLOCKMode == CLOCK_MODE_SHOW_CLOCK))
					{
						Prekey=KEY_INVALID;
					}
					else
					{
						POWERONMENU_Exit(OSD_UI_EXIT_ALL);
					}
					break;
				default:
					break;
     		      }
		}
     }
     else
     {
     		switch(Prekey)
     		{
			case KEY_PHOTO:
				if(__bMMExtFilterMode==EXT_BIT_JPG)
				{
					Prekey=KEY_INVALID;
				}
				break;
			case KEY_MUSIC:
				if(__bMMExtFilterMode==(EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA))
				{
					Prekey=KEY_INVALID;
				}
				break;
			case KEY_PHOTO_MUSIC:
				if(__bMMExtFilterMode==( EXT_BIT_JPG|EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA))
				{
					Prekey=KEY_INVALID;
				}
				break;
			case KEY_MOVIE:
				if(__bMMExtFilterMode==(  EXT_BIT_DAT|EXT_BIT_MPG|EXT_BIT_VOB|EXT_BIT_AVI))
				{
					Prekey=KEY_INVALID;
				}
				break;
			case KEY_THUMBNAIL:
				if (__bMMJPEGAudioMode)
				{
					Prekey=KEY_INVALID;
				}
			default:
				break;
    		 }

	}
	 return Prekey;
}
BYTE POWERONMENU_PrevKey(BYTE  Prekey)
{
   	Prekey=POWERONMENU_CheckKey(Prekey);	
  	 if ((Prekey != KEY_PHOTO) && (Prekey != KEY_MUSIC) && (Prekey != KEY_PHOTO_MUSIC) && (Prekey != KEY_MOVIE)&& (Prekey != KEY_DATE)&& (Prekey != KEY_TIME))
    	{
        	return Prekey;
    	}
     	if (__btPowerDown)
    	{
     		return KEY_NO_KEY;
    	}	
     	if(__bPOWERONMENUInitial)
     	{
		Prekey=Shortcut_Key_Entry(Prekey);
     	}
     	else
     	{
        	if (__btPowerDown)
            	{
                	return KEY_NO_KEY;
            	}
		#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            	if (__bMMJPEGAudioMode)
            	{
		  	MM_ExitJPEGAudioMode();
		 	 DisInitMenuFlag=TRUE;
		  	POWERONMENU_Initial();
		 	 DisInitMenuFlag=FALSE;
            	}
           	else
		#endif 				
            	{               
			#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
                    	MM_CopyDeleteMode_Exit();
			#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
                    	MEDIA_ExitUSBSRC();
                    	DisInitMenuFlag=TRUE;
                    	POWERONMENU_Initial();
		     	DisInitMenuFlag=FALSE;		 
            	}
	  	Prekey=Shortcut_Key_Entry(Prekey);
     	}
      	return Prekey;
}
BOOL Disable_Init_Menu(void)
{
	return DisInitMenuFlag;
}
#endif

BYTE POWERONMENU_ProcessKey(void)
{
    BYTE bMediaStatus;
#ifdef SUPPORT_SYSTEM_SHORTCUT_KEY	
    __bKey=POWERONMENU_PrevKey(__bKey);
#endif
    if (!__bPOWERONMENUInitial)
    {
        if (__bKey == KEY_FUNCTION)
        {
            //Fix the problem about system dead when power off==>KEY_FUNCTION==>power on.
            if (__btPowerDown)
            {
                return KEY_NO_KEY;
            }

            //elmer2.38a, exit USB for MM UI.
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {                
                if ((__bKey != KEY_PHOTO) && (__bKey != KEY_MUSIC) && (__bKey != KEY_PHOTO_MUSIC) && (__bKey != KEY_MOVIE))
                {
                    MM_ExitJPEGAudioMode();
                    __bFWKey = KEY_FUNCTION;
                    return KEY_NO_KEY;
                }
                else
                {
                    POWERONMENU_CheckModeKey();
                }
            }
            else
            {
#endif //SUPPORT_JPEG_AUDIO_PLAYING
                
                MEDIA_ExitUSBSRC();
                
                bMediaStatus = MediaInfo[_MapSourceID2Index(__SF_SourceGBL[__dwSFStreamID].bSourceIndex)].bState;
                // wyc1.50-909, when doing pre-scan action, don't allow entering media select UI, otherwise system will auto play after exit
                // media select UI but system should pause when entering media select UI.
                
                /*
                if ((bMediaStatus == MEDIA_INSERT) || (bMediaStatus == MEDIA_PARSING) || (bMediaStatus == MEDIA_RECOGNIZE) || ((__wDiscType & CDROM_M1) && (__bMotionState == MOTION_STATE_PRESCAN)))
                {
                    return INVALID_KEY;
                }
                else
                */
                {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
                    if (__bMMJPEGAudioMode)
                    {
                        __bMMJPEGAudioMode = FALSE;
                    }
#endif //SUPPORT_JPEG_AUDIO_PLAYING

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
                    MM_CopyDeleteMode_Exit();
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
                    
                    POWERONMENU_Initial();
                    return KEY_NO_KEY;
                }
            }
        }
        else
        {            
            return KEY_BYPASS;
        }
    }

    if (__bKey == KEY_POWER)
        return KEY_BYPASS;

    if (__bKey == KEY_PLAY_PAUSE)
    {
        __bKey = KEY_ENTER;
    }

    POWERONMENU_CheckModeKey();

    //CoCo1.11, clear the "Show Playable message".
    if (_bPOWERONMENUShowPlayableFile)
    {
        OSDND_ClearRegion(OSD_ND_UPPER_RIGHT_REGION);
        _bPOWERONMENUShowPlayableFile = FALSE;
    }

    switch (_bPOWERONMENUSubUIMode)
    {
    case POWERONMENU_SUB_UI_CALENDAR:
        CALENDAR_ProcessKey();
        break;
    case POWERONMENU_SUB_UI_CLOCK:
        CLOCK_ProcessKey();
        break;
    case POWERONMENU_SUB_UI_ALARM:
        ALARM_ProcessKey();
        break;
    case POWERONMENU_SUB_UI_AUTO_POWER:
        AUTOPWR_ProcessKey();
        break;
    case POWERONMENU_SUB_UI_EDIT:
        EDIT_ProcessKey();
        break;
#ifdef SUPPORT_GAME_ICON
    case POWERONMENU_SUB_UI_GAME:
        MAINMENU_Game_Menu_ProcessKey(__bKey);
        break;
#endif //#ifdef SUPPORT_GAME_ICON
    case POWERONMENU_SUB_UI_MODE_NONE:
    default:
        {
            switch (__bKey)
            {
            case KEY_UP:
                _POWERONMENU_GotoUpIcon();
                break;
            case KEY_DOWN:
                _POWERONMENU_GotoDownIcon();
                break;
            case KEY_LEFT:
                _POWERONMENU_GotoLeftIcon();
                break;
            case KEY_RIGHT:
                _POWERONMENU_GotoRightIcon();
                break;
            case KEY_ENTER:
            case KEY_PLAY:
                return _POWERONMENU_ProcessIcon();
                break;
            default:
                break;
            }
        }
        break;
    }

    return KEY_NO_KEY;
}

void _POWERONMENU_ShowIcon(void)
{
    BYTE bIconIndex;
    PARM_DRAW_BITMAP DrawBitmap;

    //Load palette for Navigaor's icons.
    GDI_PALETTE_INFO    PaletteInfo_PowerOnMenu = {POWERONMENU_BITMAP_PALETTE_INDEX_START,  PAL_ADDR, _aPOWERONMENU_Palette};

	GDI_LoadPalette(&PaletteInfo_PowerOnMenu);
    //GDI_LoadBMP(_aPOWERONMENUBMPArray, POWERONMENU_TOTAL_ICON*2);

    for (bIconIndex = 0; bIconIndex < POWERONMENU_TOTAL_ICON; bIconIndex++)
    {
        GDI_ChangePALEntry(PowerOnMenuIcon[bIconIndex].bNormalBGPaletteIndex, 0x5a000000, FALSE);
    }

    GDI_WaitPaletteComplete();
    
    for (bIconIndex = 0; bIconIndex < POWERONMENU_TOTAL_ICON; bIconIndex++)
    {
        //Draw icon
        DrawBitmap.bBitmapID = bIconIndex;
        
        DrawBitmap.wDestX = PowerOnMenuIcon[bIconIndex].wStartX;
        DrawBitmap.wDestY = PowerOnMenuIcon[bIconIndex].wStartY;
        
        DrawBitmap.bDestRegion = 0;
        //GDI_DrawBitmapBySW(&DrawBitmap, _aPOWERONMENUBMPArray); 
        GDI_DrawZipBitmap(&DrawBitmap, _aPOWERONMENUBMPArray, (PBYTE)POWERPNMENU_ICON_UNZIP_ADDR, POWERPNMENU_UNZIP_WINDOW_ADDR);

        _POWERONMENU_DrawIconText(bIconIndex, FALSE);
    }
}

void _POWERONMENU_UpdateCursor(BYTE bCursorIndex, BYTE bClear)
{
    PARM_DRAW_BITMAP DrawBitmap;

    //Change the icon to highlight icon
    if (bClear)
    {
        DrawBitmap.bBitmapID = bCursorIndex;
    }
    else
    {
        DrawBitmap.bBitmapID = bCursorIndex+POWERONMENU_TOTAL_ICON;
    }
    
    DrawBitmap.wDestX = PowerOnMenuIcon[bCursorIndex].wStartX;
    DrawBitmap.wDestY = PowerOnMenuIcon[bCursorIndex].wStartY;
    
    DrawBitmap.bDestRegion = 0;
    //GDI_DrawBitmapBySW(&DrawBitmap, _aPOWERONMENUBMPArray);  
    GDI_DrawZipBitmap(&DrawBitmap, _aPOWERONMENUBMPArray, (PBYTE)POWERPNMENU_ICON_UNZIP_ADDR, POWERPNMENU_UNZIP_WINDOW_ADDR);

    //Update the background of the icon
    if (bClear)
    {
        GDI_ChangePALEntry(PowerOnMenuIcon[bCursorIndex].bNormalBGPaletteIndex, 0x5a000000, FALSE);
        _POWERONMENU_DrawIconText(bCursorIndex, FALSE);
    }
    else
    {
        GDI_ChangePALEntry(PowerOnMenuIcon[bCursorIndex].bHighlightBGPaletteIndex, POWERONMENU_PALETTE_COLOR_ICON_HIGHLIGHT_BACKGROUND, FALSE);
        _POWERONMENU_DrawIconText(bCursorIndex, TRUE);
    }
}

//  ***************************************************************************
//  Function    :   POWERONMENU_ConfigOSDRegion
//  Description :   
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void POWERONMENU_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;
#ifdef  BORROW_FRAME_BUFFER
    DWORD dwBufferAddr;
#endif //

    GDI_ClearRegion(0);

#ifdef  BORROW_FRAME_BUFFER

    if ((POWERPNMENU_WIDTH == __RegionList[0].wWidth) && (POWERPNMENU_HEIGHT <= __RegionList[0].wHeight) &&
        (GDI_OSD_8B_MODE == __RegionList[0].bColorMode)) 
    {
        return;
    }
    else if (__RegionList[0].bColorMode != GDI_OSD_8B_MODE)   // Thumbnail mode & preview mode
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

void _POWERONMENU_GotoDownIcon(void)
{
    BYTE bIconIndex;
    DWORD dwDistance = 0;
    DWORD dwShortestDistance = 0xFFFFFFFF;
    WORD wX, wY;
    BYTE bLatestIndex = _bPOWERONMENUCursorIndex;

    for (bIconIndex = 0; bIconIndex < POWERONMENU_TOTAL_ICON; bIconIndex++)
    {
        if (PowerOnMenuIcon[bIconIndex].wStartY > PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartY)
        {
            if (PowerOnMenuIcon[bIconIndex].wStartX >= PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartX)
            {
                wX = PowerOnMenuIcon[bIconIndex].wStartX-PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartX;
            }
            else
            {
                wX = PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartX-PowerOnMenuIcon[bIconIndex].wStartX;
            }
            
            wY = PowerOnMenuIcon[bIconIndex].wStartY-PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartY;
            dwDistance = wX*wX+wY*wY;

            if (dwDistance < dwShortestDistance)
            {
                dwShortestDistance = dwDistance;
                bLatestIndex = bIconIndex;
            }
        }
    }

    _POWERONMENU_MoveCursor(bLatestIndex);
}

void _POWERONMENU_GotoUpIcon(void)
{
    BYTE bIconIndex;
    DWORD dwDistance = 0;
    DWORD dwShortestDistance = 0xFFFFFFFF;
    WORD wX, wY;
    BYTE bLatestIndex = _bPOWERONMENUCursorIndex;

    for (bIconIndex = 0; bIconIndex < POWERONMENU_TOTAL_ICON; bIconIndex++)
    {
        if (PowerOnMenuIcon[bIconIndex].wStartY < PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartY)
        {
            if (PowerOnMenuIcon[bIconIndex].wStartX >= PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartX)
            {
                wX = PowerOnMenuIcon[bIconIndex].wStartX-PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartX;
            }
            else
            {
                wX = PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartX-PowerOnMenuIcon[bIconIndex].wStartX;
            }
            
            wY = PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartY-PowerOnMenuIcon[bIconIndex].wStartY;
            dwDistance = wX*wX+wY*wY;

            if (dwDistance < dwShortestDistance)
            {
                dwShortestDistance = dwDistance;
                bLatestIndex = bIconIndex;
            }
        }
    }

    _POWERONMENU_MoveCursor(bLatestIndex);
}

void _POWERONMENU_MoveCursor(BYTE bNewCursorIndex)
{
    if (bNewCursorIndex != _bPOWERONMENUCursorIndex)
    {
        _POWERONMENU_UpdateCursor(_bPOWERONMENUCursorIndex, TRUE);
        _bPOWERONMENUCursorIndex = bNewCursorIndex;
        _POWERONMENU_UpdateCursor(_bPOWERONMENUCursorIndex, FALSE);
    }        
}

void _POWERONMENU_GotoRightIcon(void)
{
    BYTE bIconIndex;
    DWORD dwDistance = 0;
    DWORD dwShortestDistance = 0xFFFFFFFF;
    WORD wX, wY;
    BYTE bLatestIndex = _bPOWERONMENUCursorIndex;

    for (bIconIndex = 0; bIconIndex < POWERONMENU_TOTAL_ICON; bIconIndex++)
    {
        if (PowerOnMenuIcon[bIconIndex].wStartX > PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartX)
        {
            if (PowerOnMenuIcon[bIconIndex].wStartY >= PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartY)
            {
                wY = PowerOnMenuIcon[bIconIndex].wStartY-PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartY;
            }
            else
            {
                wY = PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartY-PowerOnMenuIcon[bIconIndex].wStartY;
            }
            
            wX = PowerOnMenuIcon[bIconIndex].wStartX-PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartX;
            dwDistance = wX*wX+wY*wY;

            if (dwDistance < dwShortestDistance)
            {
                dwShortestDistance = dwDistance;
                bLatestIndex = bIconIndex;
            }
        }
    }

    _POWERONMENU_MoveCursor(bLatestIndex);
}

void _POWERONMENU_GotoLeftIcon(void)
{
    BYTE bIconIndex;
    DWORD dwDistance = 0;
    DWORD dwShortestDistance = 0xFFFFFFFF;
    WORD wX, wY;
    BYTE bLatestIndex = _bPOWERONMENUCursorIndex;

    for (bIconIndex = 0; bIconIndex < POWERONMENU_TOTAL_ICON; bIconIndex++)
    {
        if (PowerOnMenuIcon[bIconIndex].wStartX < PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartX)
        {
            if (PowerOnMenuIcon[bIconIndex].wStartY >= PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartY)
            {
                wY = PowerOnMenuIcon[bIconIndex].wStartY-PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartY;
            }
            else
            {
                wY = PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartY-PowerOnMenuIcon[bIconIndex].wStartY;
            }
            
            wX = PowerOnMenuIcon[_bPOWERONMENUCursorIndex].wStartX - PowerOnMenuIcon[bIconIndex].wStartX;
            dwDistance = wX*wX+wY*wY;

            if (dwDistance < dwShortestDistance)
            {
                dwShortestDistance = dwDistance;
                bLatestIndex = bIconIndex;
            }
        }
    }

    _POWERONMENU_MoveCursor(bLatestIndex);
}

//*********************************************************************************************************
//  Function    :   POWERONMENU_Recover
//  Abstract    :   Call this function to do recovery.
//  Arguments   :   bRecoverRegion: OSD_RECOVER_UPPER_REGION/OSD_RECOVER_BOTTOM_REGION.
//  Return		:   TRUE: The region is recovered.
//              :   FALSE: The region doesn't need to be recovered in THUMB UI.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE POWERONMENU_Recover(BYTE bRecoverRegion)
{
    if (bRecoverRegion == OSD_RECOVER_ALL)
    {
        switch (_bPOWERONMENUSubUIMode)
        {
        case POWERONMENU_SUB_UI_CALENDAR:
            CALENDAR_Recover();
            break;
        case POWERONMENU_SUB_UI_CLOCK:
            CLOCK_Recover(bRecoverRegion);
            break;
        case POWERONMENU_SUB_UI_ALARM:
            ALARM_Recover(bRecoverRegion);
            break;
        case POWERONMENU_SUB_UI_AUTO_POWER:
            AUTOPWR_Recover(bRecoverRegion);
            break;
        case POWERONMENU_SUB_UI_EDIT:
            EDIT_Recover();
            break;
#ifdef SUPPORT_GAME_ICON
        case POWERONMENU_SUB_UI_GAME:
            MAINMENU_Game_Menu_Recover();
            break;
#endif //#ifdef SUPPORT_GAME_ICON
        case POWERONMENU_SUB_UI_MODE_NONE:  
        default:
            _POWERONMENU_DrawAllUI();
            break;
        }
    }

    return TRUE;
}

//*********************************************************************************************************
//  Function    :   POWERONMENU_BackToMainMenu
//  Abstract    :   Call this function to return to the main menu.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void POWERONMENU_BackToMainMenu(void)
{
    _bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_MODE_NONE;
#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_16)
    OSDND_ConfigOSDRegion();
    //GDI_ClearRegion(0);
#endif		
    _POWERONMENU_DrawAllUI();
}


//*********************************************************************************************************
//  Function    :   POWERONMENU_Exit
//  Abstract    :   This function will prepare all things to exit power on menu.
//  Arguments   :   bExitMode: OSD_UI_EXIT_ALL/OSD_UI_EXIT_CONTROL_ONLY.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void POWERONMENU_Exit(BYTE bExitMode)
{
    __bPOWERONMENUInitial = FALSE;

    switch (_bPOWERONMENUSubUIMode)
    {
    case POWERONMENU_SUB_UI_CALENDAR:
        CALENDAR_Exit(FALSE);
        break;
    case POWERONMENU_SUB_UI_CLOCK:
        CLOCK_Exit(FALSE);
        break;
    case POWERONMENU_SUB_UI_ALARM:
        ALARM_Exit(FALSE);
        break;
    case POWERONMENU_SUB_UI_AUTO_POWER:
        AUTOPWR_Exit(FALSE);
        break;
    case POWERONMENU_SUB_UI_EDIT:
        EDIT_Exit(FALSE);
        break;
#ifdef SUPPORT_GAME_ICON
    case POWERONMENU_SUB_UI_GAME:
        MAINMENU_Game_Menu_Exit(FALSE);
        break;
#endif //#ifdef SUPPORT_GAME_ICON
    case POWERONMENU_SUB_UI_MODE_NONE:  
    default:
        break;
    }

    if (bExitMode == OSD_UI_EXIT_ALL)
    {
        GDI_ClearRegion(0);
    }
}

void POWERONMENU_InternalExit(void)
{
    POWERONMENU_Exit(OSD_UI_EXIT_ALL);

    OSD_ChangeUI(OSD_UI_POWERON_MENU, OSD_EXIT_UI);
}

void POWERONMENU_ClearScreen(void)
{  
    // Clear the display buffer refer to current state (playing mode, power on menu/submenu, screen saver mode).
    if (__bAttrPlay & TYPE_CDROM_AV)
    {
        HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
    }
    else if (__bAttrPlay == ATTR_JPG) 
    {       
        HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
    }
    else if (__bLOGO != LOGO_NONE) 
    {
        HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
        __bLOGO = LOGO_NONE;
    }
#ifndef NO_SCREEN_SAVER
#ifdef OSDSS_JPEG_SCREEN_SAVER
    else if (_bOSDSSScreenSaverMode) 
    {
        HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
    }	
#endif
#endif
}

void _POWERONMENU_DrawIconText(BYTE bIconIndex, BYTE bHighlight)
{
    PARM_DRAW_STRING IconStringAttr;
    
    MAINMENU_UseColorKey(&IconStringAttr);

    if (bHighlight)
    {
        IconStringAttr.bTextColor = POWERONMENU_PALETTE_ENTRY_ICON_HIGHLIGHT_TEXT;
    }
    else //normal
    {
        IconStringAttr.bTextColor = POWERONMENU_PALETTE_ENTRY_ICON_NORMAL_TEXT;
    }

    switch (PowerOnMenuIcon[bIconIndex].bIconID)
    {
#ifdef SUPPORT_PHOTO_ICON
    case POWERONMENU_ICON_ID_PHOTO:
        __pOSDUTLOutputString = aPOWERONIconPhoto;
        break;        
#endif //SUPPORT_PHOTO_ICON
#ifdef SUPPORT_MUSIC_ICON
    case POWERONMENU_ICON_ID_MUSIC:
        __pOSDUTLOutputString = aPOWERONIconMusic;
        break;
#endif //SUPPORT_MUSIC_ICON
#ifdef SUPPORT_PHOTO_MUSIC_ICON
    case POWERONMENU_ICON_ID_PHOTO_PLUS_MUSIC:
        __pOSDUTLOutputString = aPOWERONIconPhotoMusic;
        break;
#endif //SUPPORT_PHOTO_MUSIC_ICON
#ifdef SUPPORT_MOVIE_ICON
    case POWERONMENU_ICON_ID_MOVIE:
        __pOSDUTLOutputString = aPOWERONIconMovie;
        break;
#endif //SUPPORT_MOVIE_ICON
#ifdef SUPPORT_SETUP_ICON
    case POWERONMENU_ICON_ID_SETUP:
        __pOSDUTLOutputString = aPOWERONIconSetup;
        break;
#endif //SUPPORT_SETUP_ICON
#ifdef SUPPORT_CALENDAR_ICON
    case POWERONMENU_ICON_ID_CALENDAR:
        __pOSDUTLOutputString = aPOWERONIconCalendar;
        break;
#endif //SUPPORT_CALENDAR_ICON
#ifdef SUPPORT_CLOCK_ICON
    case POWERONMENU_ICON_ID_CLOCK:
        __pOSDUTLOutputString = aPOWERONIconClock;
        break;
#endif //SUPPORT_CLOCK_ICON
#ifdef SUPPORT_ALARM_ICON
    case POWERONMENU_ICON_ID_ALARM:
        __pOSDUTLOutputString = aPOWERONIconAlarm;
        break;
#endif //SUPPORT_ALARM_ICON
#ifdef SUPPORT_AUTO_POWER_ICON
    case POWERONMENU_ICON_ID_AUTO_POWER:
        __pOSDUTLOutputString = aPOWERONIconAutoPower;
        break;
#endif //SUPPORT_AUTO_POWER_ICON
#ifdef SUPPORT_STB_ICON
    case POWERONMENU_ICON_ID_STB:
        __pOSDUTLOutputString = aMENUIconSTB;
        break;
#endif //SUPPORT_STB_ICON
#ifdef SUPPORT_EDIT_ICON
    case POWERONMENU_ICON_ID_EDIT:
        __pOSDUTLOutputString = aPOWERONIconEdit;
        break;
#endif //SUPPORT_EDIT_ICON
#ifdef SUPPORT_FAVORITE_ICON
    case POWERONMENU_ICON_ID_FAVORITE:
        __pOSDUTLOutputString = aPOWERONIconFavorite;
        break;
#endif //SUPPORT_FAVORITE_ICON
#ifdef SUPPORT_GAME_ICON
    case POWERONMENU_ICON_ID_GAME:
        __pOSDUTLOutputString = aPOWERONIconGame;
        break;
#endif //SUPPORT_GAME_ICON
#ifdef SUPPORT_RADIO_TUNER_ICON
    case POWERONMENU_ICON_ID_RADIO_TUNER:
        __pOSDUTLOutputString = aPOWERONIconRadio;
        break;
#endif //SUPPORT_RADIO_TUNER_ICON
    default:
        return;
    }              
    
    IconStringAttr.wX = PowerOnMenuIcon[bIconIndex].wStartX+((POWERONMENU_ICON_WIDTH-GDI_GetStringWidth(__pOSDUTLOutputString))>>1);
    IconStringAttr.wY = PowerOnMenuIcon[bIconIndex].wStartY+POWERONMENU_ICON_HEIGHT+POWERONMENU_ICON_TEXT_DISTANCE_V;        
    GDI_DrawString_909(0, &IconStringAttr, __pOSDUTLOutputString);   
}

BYTE _POWERONMENU_ProcessIcon(void)
{
    switch (PowerOnMenuIcon[_bPOWERONMENUCursorIndex].bIconID)
    {
#ifdef SUPPORT_PHOTO_ICON
    case POWERONMENU_ICON_ID_PHOTO:
        _POWERONMENU_EnterPhotoMode();                
        break;        
#endif //SUPPORT_PHOTO_ICON
#ifdef SUPPORT_MUSIC_ICON
    case POWERONMENU_ICON_ID_MUSIC:
        _POWERONMENU_EnterMusicMode();
        break;
#endif //SUPPORT_MUSIC_ICON
#ifdef SUPPORT_PHOTO_MUSIC_ICON
    case POWERONMENU_ICON_ID_PHOTO_PLUS_MUSIC:
        _POWERONMENU_EnterPhotoMusicMode();
        break;
#endif //SUPPORT_PHOTO_MUSIC_ICON
#ifdef SUPPORT_MOVIE_ICON
    case POWERONMENU_ICON_ID_MOVIE:
        _POWERONMENU_EnterMovieMode();
        break;
#endif //SUPPORT_MOVIE_ICON
#ifdef SUPPORT_SETUP_ICON
    case POWERONMENU_ICON_ID_SETUP:
        GDI_ClearRegion(0);
        __bFWKey = KEY_SETUP;
        break;
#endif //SUPPORT_SETUP_ICON
#ifdef SUPPORT_CALENDAR_ICON
    case POWERONMENU_ICON_ID_CALENDAR:
        _bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_CALENDAR;

#ifdef SUPPORT_SET_DATE_BY_MENU
        CALENDAR_Entry(CALENDAR_MODE_MENU);
#else
        CALENDAR_Entry(CALENDAR_MODE_SETTING);
#endif //SUPPORT_SET_DATE_BY_MENU
        break;
#endif //SUPPORT_CALENDAR_ICON
#ifdef SUPPORT_CLOCK_ICON
    case POWERONMENU_ICON_ID_CLOCK:
        _bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_CLOCK;
        CLOCK_Entry();
        break;
#endif //SUPPORT_CLOCK_ICON
#ifdef SUPPORT_ALARM_ICON
    case POWERONMENU_ICON_ID_ALARM:
        _bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_ALARM;
        ALARM_Entry();
        break;
#endif //SUPPORT_ALARM_ICON
#ifdef SUPPORT_AUTO_POWER_ICON
    case POWERONMENU_ICON_ID_AUTO_POWER:
        _bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_AUTO_POWER;
        AUTOPWR_Entry();
        break;
#endif //SUPPORT_AUTO_POWER_ICON
#ifdef SUPPORT_STB_ICON
    case POWERONMENU_ICON_ID_STB:
        MEDIA_STB();
        break;
#endif //SUPPORT_STB_ICON
#ifdef SUPPORT_EDIT_ICON
    case POWERONMENU_ICON_ID_EDIT:
        DVDSETUP_SetSlideShowIntervalTime();
        DVDSETUP_SetSlideShowEffect();

        _bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_EDIT;
        EDIT_Entry();
        break;
#endif //SUPPORT_EDIT_ICON
#ifdef SUPPORT_FAVORITE_ICON
    case POWERONMENU_ICON_ID_FAVORITE:
        _POWERONMENU_EnterFavoriteMode();
        break;
#endif //SUPPORT_FAVORITE_ICON
#ifdef SUPPORT_GAME_ICON
    case POWERONMENU_ICON_ID_GAME:
        GDI_ClearRegion(0);
        _bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_GAME;
        MAINMENU_Game_Menu_Initial();
        break;
#endif //SUPPORT_GAME_ICON
#ifdef SUPPORT_RADIO_TUNER_ICON
    case POWERONMENU_ICON_ID_RADIO_TUNER:
        POWERONMENU_InternalExit();
        RADIO_Entry(RADIO_MODE_NORMAL);
        break;
#endif //SUPPORT_RADIO_TUNER_ICON
    default:
        break;
    }

    return KEY_NO_KEY;
}        

void POWERONMENU_Trigger(BYTE bType, WORD wTime)
{
    switch (_bPOWERONMENUSubUIMode)
    {
    case POWERONMENU_SUB_UI_CALENDAR:
        CALENDAR_Trigger();
        break;
    case POWERONMENU_SUB_UI_CLOCK:
        CLOCK_Trigger(bType, wTime);
        break;
    case POWERONMENU_SUB_UI_ALARM:
        break;
    case POWERONMENU_SUB_UI_AUTO_POWER:
        break;
    case POWERONMENU_SUB_UI_EDIT:
        break;
#ifdef SUPPORT_GAME_ICON
    case POWERONMENU_SUB_UI_GAME:
        MAINMENU_Game_Menu_Trigger();
        break;
#endif //SUPPORT_GAME_ICON
    case POWERONMENU_SUB_UI_MODE_NONE:  
    default:
        break;
    }    
}

BYTE POWERONMENU_CheckModeKey(void)
{
    BYTE bMediaStatus;

    if ((__bKey != KEY_PHOTO) && (__bKey != KEY_MUSIC) && (__bKey != KEY_PHOTO_MUSIC) && (__bKey != KEY_MOVIE))
    {
        return KEY_BYPASS;
    }

	if (__bSetupMode == TRUE)
	{
		OSD_RemoveUI(OSD_UI_SETUP);
	}
    
    if (!__bPOWERONMENUInitial)
    {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__bMMJPEGAudioMode)
        {
            MM_ExitJPEGAudioMode();
        }
#endif //SUPPORT_JPEG_AUDIO_PLAYING
                
        MEDIA_ExitUSBSRC();
        
        bMediaStatus = MediaInfo[_MapSourceID2Index(__SF_SourceGBL[__dwSFStreamID].bSourceIndex)].bState;
        // wyc1.50-909, when doing pre-scan action, don't allow entering media select UI, otherwise system will auto play after exit
        // media select UI but system should pause when entering media select UI.
		/*--CoCo1.20, remove it becuase Elmer and YC confirmed what we don't do it for USB.
        if ((bMediaStatus == MEDIA_INSERT) || (bMediaStatus == MEDIA_PARSING) || ((__wDiscType & CDROM_M1) && (__bMotionState == MOTION_STATE_PRESCAN)))
        {
            return INVALID_KEY;
        }
        else
		*/
        {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {
                __bMMJPEGAudioMode = FALSE;
            }
#endif //SUPPORT_JPEG_AUDIO_PLAYING

            CC_KeyCommand(KEY_STOP);
            //MEDIA_Removed(SOURCE_DISC);
            // wyc1.05-909, reset state to open/close and show LOGO when media removed.
            CC_ResetState( KEY_OPEN_CLOSE );
                        
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM    
            CC_ForceStop(CC_FORCE_STOP_FROM_POWERON_MENU);
#endif
        }
    }    
    else
    {
        POWERONMENU_Exit(OSD_UI_EXIT_ALL);
    }

    switch (__bKey)
    {
    case KEY_PHOTO:
        _POWERONMENU_EnterPhotoMode();
        break;
    case KEY_MUSIC:
        _POWERONMENU_EnterMusicMode();
        break;
    case KEY_PHOTO_MUSIC:
        _POWERONMENU_EnterPhotoMusicMode();
        break;
    case KEY_MOVIE:
        _POWERONMENU_EnterMovieMode();
        break;
    default:
        break;
    }

    return KEY_NO_KEY;
}

void _POWERONMENU_EnterPhotoMode(void)
{
    __bMMExtFilterMode = EXT_BIT_JPG;
    POWERONMENU_InternalExit();
    //Set JPEG relative setting always.
    DVDSETUP_SetSlideShowIntervalTime();
    DVDSETUP_SetSlideShowEffect();
    if (__ImageFrameSetupInfo.bAutoPlayPhoto == SETUP_AUTO_PLAY_PHOTO_ON)
    {
        __bMMAutoPlay = TRUE;
    }
    else
    {
        __bMMAutoPlay = FALSE;
    }
    
    MEDIA_USB();
}

void _POWERONMENU_EnterMusicMode(void)
{
#ifdef NL_SOLUTION
    __bMMExtFilterMode = EXT_BIT_MP3|EXT_BIT_MP2;
#else
    __bMMExtFilterMode = EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA;
#endif //NL_SOLUTION
    POWERONMENU_InternalExit();
    
    if (__ImageFrameSetupInfo.bAutoPlayMusic == SETUP_AUTO_PLAY_MUSIC_ON)
    {
        __bMMAutoPlay = TRUE;
    }
    else
    {
        __bMMAutoPlay = FALSE;
    }
    
    
    MEDIA_USB();
}

void _POWERONMENU_EnterPhotoMusicMode(void)
{
    __bMMExtFilterMode = EXT_BIT_JPG|EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA;

#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
    if (__bMMPlayMediaDirectlyMode == FALSE)
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
    {
        POWERONMENU_InternalExit();
    }
    
    //Set JPEG relative setting always.
    DVDSETUP_SetSlideShowIntervalTime();
    DVDSETUP_SetSlideShowEffect();
    
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    __bMMJPEGAudioMode = TRUE;
    __bMMAutoPlay = TRUE;
    __bRepeat = REPEAT_DISC;
#ifndef CT909P_IC_SYSTEM

    if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_JPEG_DONE) )
    {
        INITIAL_ThreadInit(THREAD_JPEG_DECODER);
        
        OS_YieldThread();
        
        OS_TimedWaitFlag( &__fThreadInit, INIT_DEC_THREAD_JPEG_DONE, FLAG_WAITMODE_AND,
            (COUNT_100_MSEC) );

        if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_JPEG_DONE) )
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "JPEG thread not initial done, value: %lx\n", OS_PeekFlag(&__fThreadInit));
        }
    }

    JPEGDEC_ConfigProcessor(1); //Use 1 processor only
#endif //#ifndef CT909P_IC_SYSTEM
#else
    
    if (__ImageFrameSetupInfo.bAutoPlayPhotoPlusMusic == SETUP_AUTO_PLAY_PHOTO_PLUS_MUSIC_ON)
    {
        __bMMAutoPlay = TRUE;
    }
    else
    {
        __bMMAutoPlay = FALSE;
    }
#endif //SUPPORT_JPEG_AUDIO_PLAYING
    
    MEDIA_USB();
}

void _POWERONMENU_EnterMovieMode(void)
{
    __bMMExtFilterMode = EXT_BIT_DAT|EXT_BIT_MPG|EXT_BIT_VOB|EXT_BIT_AVI;
    POWERONMENU_InternalExit();
    if (__ImageFrameSetupInfo.bAutoPlayMovie == SETUP_AUTO_PLAY_MOVIE_ON)
    {
        __bMMAutoPlay = TRUE;
    }
    else
    {
        __bMMAutoPlay = FALSE;
    }
    MEDIA_USB();
}

#ifdef SUPPORT_FAVORITE_FUNCTION
void _POWERONMENU_EnterFavoriteMode(void)
{
    if (__bMMJPGEncodeNum > 0)
    {
        POWERONMENU_InternalExit();
        //Set JPEG relative setting always.
        DVDSETUP_SetSlideShowIntervalTime();
        DVDSETUP_SetSlideShowEffect();
        
        // Switch source to SPI
        __wDiscType = BOOK_M1;
        __wPlayItem = 1;
        __SF_SourceGBL[0].bSourceIndex = SOURCE_SPI;
        __bModeCmd=KEY_PLAY;  //Winnie 1.03, fix first time key press key_next then show key play
        UTL_PlayItem(__wPlayItem, 0);
    }
    else
    {
        strcpy(__bMsgString, "No playable file");
        _bPOWERONMENUShowPlayableFile = TRUE; //CoCo1.11
        OSD_Output(MSG_NO_PLAYABLE_FILE, 0, 3);                     
    }
}
#endif //SUPPORT_FAVORITE_FUNCTION

#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
void POWERONMENU_PowerOnPlayMediaDirectly(void)
{
    __bMMPlayMediaDirectlyMode = TRUE;

    _POWERONMENU_EnterPhotoMusicMode();
}
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON