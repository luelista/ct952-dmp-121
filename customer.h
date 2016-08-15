/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved

        Product : Cheertek W99AV series Firmware
        Chip    : W9922QF

        Date    : 2001.04.11.
        Author  : Cheertek (D300 WWC)
        Purpose : Put customer depending setting into a single file
        Sources : customer.h

****************************************************************************/
//////////////////////////////////////////////////////////////
// ***** Big Modification History Area *****
// Micky1.00
//  - Remove W9928AF related system define DVD28A_xxx/ DVD_28C_xxx.

#ifndef __CUSTOMER_H__
#define __CUSTOMER_H__

//////////////////////////////////////
#define RELEASE_SETTING
// LLY2.13, select desired CPU working clock
// #define     CPU_SPEED       CPU_133M     // If Don't define this, will be default=133MHz

// F/W code version
// !!!! Don't allow modification !!!
//Micky1.26, put the version at custmer.h, as servo will reference it.
//alex080  support cd rom upgrade.
#define     SW_VERSION          (278L)
#define     SW_MINOR_VERSION    (0) // default is '0', '1' means 'a', '16' means 'p', and so on...

// LLY2.78b-3, add two extra define for customer S/W version
#define CUSTOMER_SW_VERSION         SW_VERSION
#define CUSTOMER_SW_MINOR_VERSION   SW_MINOR_VERSION

// Micky1.11a, add FAE version.
#define FAE_SW_VERSION  SW_VERSION 

//CoCo2.38
//#define CT951_PLATFORM

// wyc0.91, Brian's updated code.
// Brian0.91
// default is '0', '1' means 'a', '16' means 'p', and so on...
#define FAE_SW_MINOR_VERSION    0               // FAE should maintain this number
#define CS_VERSION_CUSTOMER     "WTK20070612VER06"	// Chuan1.06, define the customer version string, default is CTK909

#define MODEL_ID_TOP_BYTE      (0x30 & 0xF0)
#define MODEL_ID_LOW_BYTE      (0x31)

// wyc2.17-909S, move decoder define to internal.h
// Micky1.10-2, add configuration for support 32M SDRAM
// define it, will support 32M SRDAM configuration. need uCode cooperation
// #define USE_32M_SDRAM

//LJY1.11, support anti-shock
// #define SUPPORT_ANTISHOCK 
#ifdef SUPPORT_ANTISHOCK
//#define SUPPORT_ANTISHOCK_SOFT_RESET_VIDEO //LJY1.23, LJY1.11d, enable it for soft reset AV buffer when freeze->play or multi-audio switching
//#define SUPPORT_OPENTRAY_WHEN_CLOSETRAY_FAILED 	//LJY1.25, enable it for CZ00 car loader supporting
#endif

// Must define the time for auto power down
#define WAITTIME_POWERDOWN      (COUNT_10_SEC*48L)

#define IR_POWERDOWN_MODE       PSEUDO_SHUTDOWN    // LLY.171-1
// ******************************
// Customer operation habit
// ******************************
// Micky0.66-909, move the define into customer.h, as cc/cckey.c will reference it.
// define it, Fast will be the 1st round for SCF
// else, will use scan for all round.
#define SAME_FAST_AS_SCF2

// Micky1.24
#define GOTOTIME_SHOW_INVALID
///#define CLEAR_REPEAT_WHEN_REAL_STOP		

//#define DOLBY_CERTIFICATION       //Kevin0.80
//#define MACROVISION_CERTIFICATION //kevin0.80
//#define SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE //Joie2.35 for CT909R video circuit add inductance 1.8uh and 3.3uh with VDAC3.3V        
//#define    POWERON_STANDBY
#define    AUTO_PLAY
// wyc1.07a, a define to support FM auto play and default enable it. Also add a dfine SUPPORT_TUNE_JPG_SHOW_TIME and enable it.
#define MMANAGER_AUTO_PLAY
#define SUPPORT_TUNE_JPG_SHOW_TIME
//#define    DEFAULT_NOPBC
//#define    BORDER_BLUE
//#define AUTO_POWERDOWN
// Micky1.25, add auto powerdown for screen saver mode
// #define POWERDOWN_AFTER_SCREENSAVER
//#define SUPPORT_NPF_TRIGGER_ONLY
#define SUPPORT_NEXT_TRIGGER_FROM_STOP  // ** TCH1.72;
//#define AUTO_PBCOFF
#define DOUBLE_STOP
//#define GOTOTIME_LEFTTORIGHT
//#define GOTOTIME_NUMBER_NOROUND
//#define ACCEPT_TIMEOUT_NUMBERKEY  // ** TCH2.78a;
//#define KEY_N0_SAME_NPLUS
//#define SUPPORT_NUMBER_CONFIRM_KEY
// wyc1.10, new define for control CDROM titles number confirm key on/off and select cursor ring in same directory.
#define SUPPORT_CDROM_NUMBER_CONFIRM_KEY
//#define SUPPORT_FM_CURSOR_RING_SAME_DIRECTORY
#define SUPPORT_PRGM_NUM_INPUT //Kevin1.24, from to osdprgm.c
#define SUPPORT_GRAPH_ZOOM
// wyc1.07-909, need to define it.
#define SUPPORT_NAVIGATOR

// wyc.172-CDDA, add define to support when meet Medley CDDA title the system can play cross trakcs without mute audio.
#define     SUPPORT_PLAY_MEDLEYCDDA
#define     PROGRAM_PLAY_DIRECT
#define     ROUND_SCAN
// #define     FORCE_CLEAR_LOGO
//#define     SAME_AB_RESUME
//#define     REPEAT_CLEAR_REPEATAB
#define KEY_AUDIO_LR_SAME_KEY_AST_CTL
#define ABSOLUTE_GOTOTIME

// wyc.278a-1, define to support NTSC 4.43
// #define SUPPORT_NTSC443
#define FORCE_CHANGE_TRACK_NORMAL_PLAY
///#define FORCE_SWITCH_NONPBC
///#define FORCE_DEFAULT_AST_SPST_AFTER_REAL_STOP       // ** TCH1.01-1;
///#define FORCE_SET_REPEAT_MODE_AFTER_REAL_NEXTPREV       // ** TCH1.03;
#define SUPPORT_AUDIO_DEEMPHASYS
#define SUPPORT_GENERAL_BACKDOOR
#define SHOW_LOADING_STATUS
/// #define NUMBER_OUTRANGE_INVALID

// Can be defined only when IR KEY_ACTIVE is not exist
#define SUPPORT_KEY_PLAY_AS_KEY_ACTIVE

// DVD_161Micky, support MP3 resume can play from track begin
// if define it, when resume, can play the last played track
#define SUPPORT_MP3_RESUME
// wyc1.20-909,
/// #define SUPPORT_FILE_HANDLE

// DVD_161Micky, support Loading-> Setup-> Exit, will continue loading action
#define SUPPORT_LOADING_SETUP

#define SUPPORT_SHOW_UNRECOGNIZE_DISC    // ** TCH1.70;

#define SUPPORT_INDEPENDENT_NAVIGATOR

// #define SET_REPEAT_OFF_WHEN_FULL_STOP
// #define SUPPORT_6CH_OUTPUT_FOR_NONDVD   // *** LLY.171-1

///#define SUPPORT_VCD_CONTROL              // ** TCH2.74;

// DVD_275b+, SUPPORT_VIRTUAL_DTS
///#define SUPPORT_VIRTUAL_DTS

// define it, means System support Scart_IF only.
// #define     SUPPORT_SCART_IF      //Kevin1.07
// define it, means scart 16:9/4:3 setting only reference setup's setting. Won't reference bitstream format.
//#define     SET_SCART_FROM_SETUP    //Kevin1.07    

// Micky2.78b-2, add support for play chapter by number key.
// define it, will accept input number as chapter number and play it.
// only support the feature when 1) Title domain and 2)must have no selective buttons
// #define SUPPORT_PLAY_CHAPTER_BY_NUMBER

// Kevin1.05, support 2 CVBS outputs when Interlaced + Normal
// Differential Gain %6.2 exceed standard <%5 when 2 CVBS outputs at the same time
// define it, outputs AVO3 CVBS & AVO0 CVBS when Interlaced + Normal
// otherwise, outputs AVO3 CVBS & turn off AVO0 CVBS  
//#define SUPPORT_2_CVBS

//Kevin1.10, support analog output off in setup menu
//#define SUPPORT_ANALOG_OUTPUT_OFF

// ************************************************
// Disc Resume Feature relative define 
// ************************************************
// If define it, support disc resume feature while I/R Power on.
// ex. IR power off: save the resume info.
//     Detect disc done, ex. IR power on or close tray: playback the resume info if same disc
//#define SUPPORT_RESUME_AFTER_POWERON

// If define it, support disc resume feature while open/close.
// ex. Tray open: save the resume info.
//     Detect disc done, ex. IR power on or close tray: playback the resume info if same disc
//#define SUPPORT_RESUME_AFTER_OPENCLOSE

// If define it, the multi disc resume feature will be kept in EEPROM
//#define SUPPORT_RESUME_FROM_EEPROM

// Ming2.42-1, Clear Resume info. in fully STOP mode.
//#define CLEAR_RESUME_WHEN_STOP



////////////////////////////////////////////////
// SERVO Relative Zone
////////////////////////////////////////////////
/// #define SUPPORT_LOADER_OPEN_BY_EXTRA_FORCE  // ** TCH1.61a;
// Micky1.24, add define for correct data
// #define it, servo must send correct data for DVDINFO
#define ACCEPT_CORRECT_CDROM_DATA
//#define SUPPORT_USB_POWER_BY_IO
////////////////////////////////////////////////
// OSD relative Zone
////////////////////////////////////////////////
// <1> General OSD part ////

// <2> Menu-Like UI
// Brian1.07
// #define OSD_ITEM_TEXT_SAME_AS_LANGUAGE 

// #define SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG

//#define SUPPORT_ADJUST_CHROM_PHASE    // Brian1.25, support chroma adjust ability
//#define SUPPORT_STRING_GENERATOR  // Brian1.25, define this will use string files generated by String Generate Tool

// <3> Display "Program Play" Message while starting the program mode playing -- LLY2.78b-3
//#define SUPPORT_PROGRAM_PLAY_INFO
// Micky1.10, define it, KEY_REPEAT will toggle between repeat track/ disc when playing program.
//#define SUPPORT_ONLY_2_REPEAT_MODE_IN_PROGRAM

////////////////////////////////////////////////////////////////////////////
// default no MIC input for DBB     // brian.274
// this only works for DBB; for DAB must replace DSP code to disable MIC
#define NO_MIC_INPUT

// Micky2.80p, only SYSTEM_8051 support serial debug
// ** TCH0.60; #ifdef  SYSTEM_8051
//Open this define if you want to use serial debug tool. //CoCo.sd
#define SERIAL_DEBUG

// LLY2.80, support volume adjustment button within panel
//#define ENABLE_PANEL_VOLUME_ADJUSTMENT_BUTTON

// Micky1.05, add auto test for IC sorting by CoCo
///#define AUTO_TEST

//CoCo1.06, open/close program function in File Manager mode
#define SUPPORT_FM_PROGRAM

//Alex1.07a, Setup will show PSCAN prompt dialog
#define SUPPORT_PSCAN_PROMPT_SETUP 

// wyc2.55-909P-AP,
// #define SUPPORT_PARSE_ALL_AP_FILES
// wyc1.00-909, add define to decide support FREEZE function or not.
//#define SUPPORT_FREEZE_FUNCTION

//Alex1.21,20031208 Support BookMark
///#define SUPPORT_BOOKMARK

// Micky1.22a, if no FP_PGC, just play from TT/CC 1/1
// define it, will play from TT/CC 1/1 when FP_PGC doesn't exist.
// for DVD title only.
// #define PLAY_TT1CC1_WHEN_NO_FPPGC

// wyc2.17-909S, support define to let KEY_DISPLAY as KEY_SETUP when STOP mode, default is disabled.
// #define SUPPORT_DISPLAY_KEY_DO_SETUP

// wyc2.36a-909s,
// #define SUPPORT_FUSS_MPEG_CD

#define SUPPORT_CHAR_SUBPICTURE

// wyc1.00-909, add Micky's new define
// enable it, when parental level is not allowed, it will query password
// disable it, it will just display parental error, and don't allow the playing.
// must disable for Forum title test
///#define SUPPORT_PASSWORD_QUERY

//Aron1.05, Support DVD Smart playback feature
#define SUPPORT_DVD_SMRTPB

//Control smart play with setup setting
//enable it by setup setting will do auto smart play after loading DVD title
//it is only used for pure DVD solution
//#define SUPPORT_DVD_SMRTPB_SETUP

// Chuan1.08, Support Power Saving to power down some HW blocks.
// wyc2.03-909S, need to check 909S define here.
// wyc1.21-909, define to support build-in card reader.
//#define SUPPORT_BUILDIN_CARD_READER

//#define SUPPORT_STB
// wyc2.38a-909s, define to play files in DCIM dir first.
// #define MM_AUTO_PLAY_DCIM_DIR_FIRST

// elmer2.32
#define SUPPORT_NO_AUDIO_IO_MUTE
// wyc2.38a-909s, define to support slot in platform.
// #define SUPPORT_SLOT_IN      // KCHong 2.17, for slot-in loader

//Aron2.38, to support still resuming
// if it enables bookmark, it also supports bookmark to content with still picture
#define SUPPORT_STILLRSM

//Modify this define to tune the maximum pixels that we can enlarge for JPEG width.
#define HALJPEG_ENLARGE_WIDTH_PIXELS        40

//Modify this define to tune the maximum pixels that we can enlarge for JPEG height.
#define HALJPEG_ENLARGE_HEIGHT_PIXELS       20

// code to support TFT function code.
//#define SUPPORT_TFT
#ifdef SUPPORT_TFT
#ifndef NO_SCREEN_SAVER
#define NO_SCREEN_SAVER
#endif //NO_SCREEN_SAVER
#define LED_BackLight
#define SUPPORT_CCIR656
#endif //SUPPORT_TFT

// Damon2.56, it's for Hebrew external subtitle
#define HEBREW_IE_FORMAT
//#define HEBREW_WORD_FORMAT
//#define HEBREW_WORD_INVERSION

//Damon2.78, remove the definition
//#define SUPPORT_CHAR_ENCODING_SWITCH

#define SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN
#define SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN
//#define SUPPORT_CHAR_ENCODING_GREEK
//#define SUPPORT_CHAR_ENCODING_HEBREW
//#define SUPPORT_CHAR_ENCODING_TURKISH
//#define SUPPORT_CHAR_ENCODING_CP1250
//#define SUPPORT_CHAR_ENCODING_CYRILLIC
//#define SUPPORT_CHAR_ENCODING_CHINESE
//#define SUPPORT_CHAR_ENCODING_JAPANESE

#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN
#define ISO_8859_15_FONT           // ISO 8859-15
#endif

#ifdef SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN
#define ISO_8859_2_FONT           // ISO 8859-2
#endif

#ifdef SUPPORT_CHAR_ENCODING_GREEK
#define ISO_8859_7_FONT           // ISO 8859-7
#endif

#ifdef SUPPORT_CHAR_ENCODING_HEBREW
#define ISO_8859_8_FONT           // ISO 8859-8
#endif

#ifdef SUPPORT_CHAR_ENCODING_TURKISH
#define ISO_8859_9_FONT           // ISO 8859-9
#endif

#ifdef SUPPORT_CHAR_ENCODING_CP1250
#define CP_1250_FONT           // CP 1250
#endif

#ifdef SUPPORT_CHAR_ENCODING_CYRILLIC
#define CP_1251_FONT            // CP 1251
#endif

#if defined(SUPPORT_CHAR_ENCODING_CHINESE) || defined(SUPPORT_CHAR_ENCODING_JAPANESE)
#define SUPPORT_UNICODE_CODING
#endif

#ifdef SUPPORT_CHAR_ENCODING_CHINESE
#define SUPPORT_CHAR_ENCODING_BIG5
#endif

#ifdef SUPPORT_CHAR_ENCODING_JAPANESE
#define SUPPORT_CHAR_ENCODING_SHIFT_JIS
#endif

#define SUPPORT_CHOOSE_MEDIA
#define  SUPPORT_MENU_KEY_FUNCTION_EXIT
#define  SUPPORT_ALARM_AUTO_EXIT
#define  SUPPORT_SYSTEM_SHORTCUT_KEY
#define SUPPORT_DSPLAY_16_9_AND_AUTO
#define  SUPPORT_DEFAULT_VOLUME_IN_FLASH
#define  SUPPORT_LOGO_AUTO_FULL_SCREEN
//#define  SUPPORT_SAVE_REPEAT_IN_FLASH
#ifdef SUPPORT_DSPLAY_16_9_AND_AUTO
#define SUPPORT_JPEG_AUTO_AND_FULL_SCREEN
#define SUPPORT_HAL_JPEG_AUTO_RATIO
#endif
//#define  SUPPORT_POWER_ON_DISPLAY_LOGO
///#define SUPPORT_CONNECT_PC_CHECK

///#define  SUPPORT_TFT_43_KEY
//#define SUPPORT_REMOVE_SETUP_TFT_SET
#endif  // __CUSTOMER_H__
