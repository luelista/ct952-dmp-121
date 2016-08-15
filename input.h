
#ifndef __INPUT_H__
#define __INPUT_H__

typedef struct tagIR_INFO {
    BYTE  bIR_Type;
    BYTE  bCustomerCode;
    BYTE  bCustomerCode1;
    const BYTE* aIRMap;
    BYTE  bIRMapSize;
} IR_INFO, *PIR_INFO;


/*** Function Prototypes ***/
VOID     DSR_IR(VOID);
VOID     ISR_IRSaveClearStatus( VOID );
BYTE     INPUT_RemoteScan(void);
VOID 	 INPUT_InitIR(PIR_INFO pIRInfo);
VOID     INPUT_SetIRInfo(PIR_INFO pIRInfo);
PIR_INFO INPUT_GetIRInfo(VOID);
// LLY2.36, add to set and get current IR checking mode.
void    INPUT_SetIRCheckMode(BYTE bMode);
BYTE    INPUT_GetIRCheckMode(void);


#ifdef SUPPORT_GAME_IRVOID
VOID     INPUT_SetGameIR(VOID);
BYTE     INPUT_Game_RemoteScan(VOID);
#endif

/// LLY2.36, define the mode for INPUT_SetIRCheckMode() 1st parameter
#define IR_CHECK_MODE_INT       0
#define IR_CHECK_MODE_POLLING   1



/*** defines ***/
//#define RESET_IR_AFTER_IR_INT //kevin0.75

#define CHECK_IR_CUSTOMER_CODE
//#define  SHOW_IR_MAP



/*** global key defines ***/
// ID 0     for FALSE
// ID 1     for TRUE
// reserved the 0~20 Pseudo Key ID
// pseudo keys ( 2 ~ 15 )
#define         KEY_NUMBER              2
#define         KEY_PLAY_PAUSE          3
#define         KEY_STOP_RETURN         4
#define         KEY_PAUSE_STEP          5


#define INPUT_KEY_GROUP1    20
// KEY_GROUP1, test keys
#define         KEY_TEST1               (INPUT_KEY_GROUP1+0)
#define         KEY_TEST2               (INPUT_KEY_GROUP1+1)
#define         KEY_TEST3               (INPUT_KEY_GROUP1+2)
#define         KEY_TEST4               (INPUT_KEY_GROUP1+3)
#define         KEY_TEST5               (INPUT_KEY_GROUP1+4)
#define         KEY_TEST6               (INPUT_KEY_GROUP1+5)
#define         KEY_TEST7               (INPUT_KEY_GROUP1+6)
#define         KEY_TEST8               (INPUT_KEY_GROUP1+7)
#define         KEY_TEST9               (INPUT_KEY_GROUP1+8)
//#define         KEY_TEST10              (INPUT_KEY_GROUP1+9)
#define         KEY_I2CTEST             (INPUT_KEY_GROUP1+9)
#define         KEY_CACHERW_TEST       (INPUT_KEY_GROUP1+9)


#define INPUT_KEY_GROUP2    30
// KEY_GROUP2, audio keys
#define         KEY_MUTE                (INPUT_KEY_GROUP2+0)      // mute on/off the audio
#define         KEY_VOL_UP              (INPUT_KEY_GROUP2+1)      // tune volume up
#define         KEY_VOL_DOWN            (INPUT_KEY_GROUP2+2)      // tune volume down
#define         KEY_KEYUP               (INPUT_KEY_GROUP2+3)      // key up
#define         KEY_KEYDOWN             (INPUT_KEY_GROUP2+4)      // key down
#define         KEY_KEYNORMAL           (INPUT_KEY_GROUP2+5)      // normal key
#define         KEY_ECHO_DOWN           (INPUT_KEY_GROUP2+6)      // enhance echo effect
#define         KEY_ECHO_UP             (INPUT_KEY_GROUP2+7)      // lower the echo effect

#define         KEY_AUDIO_LR            (INPUT_KEY_GROUP2+8)      // switch audio channel
#define         KEY_SURROUND            (INPUT_KEY_GROUP2+9)      // surround effect
#define         KEY_VOCAL               (INPUT_KEY_GROUP2+10)      // on/off human voice by switch L/R channel
#define         KEY_AUTOVOCAL           (INPUT_KEY_GROUP2+11)     // auto sing when mic. no input
#define         KEY_VOICECANCEL         (INPUT_KEY_GROUP2+12)     // cancel human voice by KARAOKE chip
#define         KEY_3D                  (INPUT_KEY_GROUP2+13)     // *** 3D will use a round setting
#define         KEY_KARAOKE             (INPUT_KEY_GROUP2+14)     // Control MIC ON/OFF
#define         KEY_AST_CTL             (INPUT_KEY_GROUP2+15)     // Change Audio Channel
#define         KEY_AUDIO_OUTPUT        (INPUT_KEY_GROUP2+16)
#define         KEY_MIC_VOL_UP         (INPUT_KEY_GROUP2+17)  
#define         KEY_MIC_VOL_DOWN       (INPUT_KEY_GROUP2+18)


#define INPUT_KEY_GROUP3    50
// KEY_GROUP3, Play back Command
// MPEG Special Macro Command
// KEY_FF ~ KEY_FREEZE must be the same group.
#define         KEY_PLAY                (INPUT_KEY_GROUP3+0)      // normal play
#define         KEY_STOP                (INPUT_KEY_GROUP3+1)      // stop
#define         KEY_FF                  (INPUT_KEY_GROUP3+2)      // fast IP with audio(2X speed)
#define         KEY_SCF                 (INPUT_KEY_GROUP3+3)      // scan forward
#define         KEY_SCB                 (INPUT_KEY_GROUP3+4)      // scan backward
#define         KEY_SF                  (INPUT_KEY_GROUP3+5)      // slow motion
#define         KEY_PAUSE               (INPUT_KEY_GROUP3+6)      // pause the play
#define         KEY_STEP                (INPUT_KEY_GROUP3+7)      // single step play
#define         KEY_FREEZE              (INPUT_KEY_GROUP3+8)      // freeze the video, but audio going
// wyc2.17-909S, add key for supporting language study function.
#define         KEY_LAN_STUDY           (INPUT_KEY_GROUP3+9)

#define         NONNPF_PLAYABLEKEY_BEGIN        KEY_FF
#define         NONNPF_PLAYABLEKEY_END          KEY_FREEZE
#define         KEY_THROWAUDIO_BEGIN            KEY_FF          // ** TCH1.00a;
#define         KEY_THROWAUDIO_END              KEY_SCB         // ** TCH1.00a;


#define INPUT_KEY_GROUP4    60
// KEY_GROUP4, Mode Control Keys
#define         KEY_PBC                 (INPUT_KEY_GROUP4+0)     // tuen PBC on/off
#define         KEY_NEXT                (INPUT_KEY_GROUP4+1)             // select next track or menu
#define         KEY_PREV                (INPUT_KEY_GROUP4+2)             // select prev. track or menu
#define         KEY_RETURN              (INPUT_KEY_GROUP4+3)     // return to the upper layer
#define         KEY_PROGRAM             (INPUT_KEY_GROUP4+4)     // program play order
#define         KEY_ENTER               (INPUT_KEY_GROUP4+5)     // confirm program input number
#define         KEY_CLEAR               (INPUT_KEY_GROUP4+6)     // clear all program entries
#define         KEY_BOOKMARK            (INPUT_KEY_GROUP4+7)     // set time point for directly play
#define         KEY_BOOKMARK_PLAY       (INPUT_KEY_GROUP4+8)     // play bookmark

#define         KEY_TVMODE              (INPUT_KEY_GROUP4+9)     // switch TV mode
#define         KEY_DISPLAY             (INPUT_KEY_GROUP4+10)    // OSD display mode
#define         KEY_DIGEST              (INPUT_KEY_GROUP4+11)     // digest

#define         KEY_REPEAT_AB           (INPUT_KEY_GROUP4+12)     // repeat play A to B
#define         KEY_REPEAT              (INPUT_KEY_GROUP4+13)     // set repeat mode( track / disc)
#define         KEY_INTRO               (INPUT_KEY_GROUP4+14)
#define         KEY_RANDOM              (INPUT_KEY_GROUP4+15)     // set random play

#define         KEY_REMAIN              (INPUT_KEY_GROUP4+16)     // switch the display time format
#define         KEY_RESUME              (INPUT_KEY_GROUP4+17)     // play from the last stop point

//Combine Key_Search/Key_GoTotime,Key_Search_Play/Key_GoToTime_Play
#define         KEY_GOTOTIME            (INPUT_KEY_GROUP4+18)     // goto new time
#define         KEY_SEARCH              (INPUT_KEY_GROUP4+18)  // chapter selection key
#define         KEY_SEARCH_PLAY         (INPUT_KEY_GROUP4+19) // pseudo key for "chapter selection" play

#define         KEY_OPEN_CLOSE          (INPUT_KEY_GROUP4+20)      // open or close the tray
#define         KEY_POWER               (INPUT_KEY_GROUP4+21)       // power off
#define         KEY_DEFAULT             (INPUT_KEY_GROUP4+22)
//pseudo key to calculate GOTOTIME value
#define         KEY_GOTOTIME_VALUE      (INPUT_KEY_GROUP4+23)
#define         KEY_THUMBNAIL           (INPUT_KEY_GROUP4+24)
#define         KEY_PSCAN               (INPUT_KEY_GROUP4+25)
#define         KEY_GOTOTIME_PLAY       (INPUT_KEY_GROUP4+26)
// wyc1.00-909, add one key to open DVD NV directly.
#ifdef SUPPORT_INDEPENDENT_NAVIGATOR
#define         KEY_NAVIGATOR           (KEY_CLEAR)
#endif //
//Aron1.11-909, support smart play by IR Key
#ifdef  SUPPORT_DVD_SMRTPB            
#define         KEY_SMARTPLAY           (KEY_EPG)
#endif
// LLY1.50, add new key to switch to GPS mode
#define         KEY_AP                 (INPUT_KEY_GROUP4+27)
#define         KEY_MM_FILECOPY         (INPUT_KEY_GROUP4+28)


#define INPUT_KEY_GROUP5    90
// KEY_GROUP5, Multi Disc Keys
// For Extra LED
#define         KEY_LED1                (INPUT_KEY_GROUP5+0)
#define         KEY_LED2                (INPUT_KEY_GROUP5+1)
#define         KEY_LED3                (INPUT_KEY_GROUP5+2)
#define         KEY_LED4                (INPUT_KEY_GROUP5+3)

// Multi-Disc System
#define         KEY_DISCSKIP            (INPUT_KEY_GROUP5+4)
#define         KEY_DISCSKIP_BACK       (INPUT_KEY_GROUP5+5)
#define         KEY_DISC1               (INPUT_KEY_GROUP5+6)
#define         KEY_DISC2               (INPUT_KEY_GROUP5+7)
#define         KEY_DISC3               (INPUT_KEY_GROUP5+8)
#define         KEY_DISC4               (INPUT_KEY_GROUP5+9)
#define         KEY_DISC5               (INPUT_KEY_GROUP5+10)
#define         KEY_DISC6               (INPUT_KEY_GROUP5+11)
#define         KEY_DISC7               (INPUT_KEY_GROUP5+12)
#define         KEY_DISC8               (INPUT_KEY_GROUP5+13)


#define INPUT_KEY_GROUP6    105         
// KEY_GROUP6, Number Key
// Number Key Range (the IDs must be sequential as main program will calculate it directly )
#define         KEY_N0                  (INPUT_KEY_GROUP6+0)            // key 0
#define         KEY_N1                  (INPUT_KEY_GROUP6+1)             // key 1
#define         KEY_N2                  (INPUT_KEY_GROUP6+2)             // key 2
#define         KEY_N3                  (INPUT_KEY_GROUP6+3)             // key 3
#define         KEY_N4                  (INPUT_KEY_GROUP6+4)             // key 4
#define         KEY_N5                  (INPUT_KEY_GROUP6+5)             // key 5
#define         KEY_N6                  (INPUT_KEY_GROUP6+6)             // key 6
#define         KEY_N7                  (INPUT_KEY_GROUP6+7)             // key 7
#define         KEY_N8                  (INPUT_KEY_GROUP6+8)             // key 8
#define         KEY_N9                  (INPUT_KEY_GROUP6+9)             // key 9

#define         KEY_N10                 (INPUT_KEY_GROUP6+10)             // key 10
#define         KEY_N11                 (INPUT_KEY_GROUP6+11)             // key 11
#define         KEY_N12                 (INPUT_KEY_GROUP6+12)             // key 12
#define         KEY_N13                 (INPUT_KEY_GROUP6+13)             // key 13
#define         KEY_N14                 (INPUT_KEY_GROUP6+14)             // key 14
#define         KEY_N15                 (INPUT_KEY_GROUP6+15)             // key 15
#define         KEY_N16                 (INPUT_KEY_GROUP6+16)             // key 16
#define         KEY_N17                 (INPUT_KEY_GROUP6+17)             // key 17
#define         KEY_N18                 (INPUT_KEY_GROUP6+18)             // key 18
#define         KEY_N19                 (INPUT_KEY_GROUP6+19)             // key 19
#define         KEY_N20                 (INPUT_KEY_GROUP6+20)             // key 20

#define         KEY_N10PLUS             (INPUT_KEY_GROUP6+21)             // key 10+
#define         KEY_N20PLUS             (INPUT_KEY_GROUP6+22)             // key 20+


#define INPUT_KEY_GROUP7    130
// KEY_GROUP7, Video Keys
#define         KEY_ZOOM                (INPUT_KEY_GROUP7+0)
//key sequence must be up/down/left/right
#define         KEY_ZOOM_UP             (INPUT_KEY_GROUP7+1)
#define         KEY_ZOOM_DOWN           (INPUT_KEY_GROUP7+2)
#define         KEY_ZOOM_LEFT           (INPUT_KEY_GROUP7+3)
#define         KEY_ZOOM_RIGHT          (INPUT_KEY_GROUP7+4)

//SVCD, DVD usage
#define         KEY_SPST_CTL            (INPUT_KEY_GROUP7+5)     // Change SubPicture Stream
#define         KEY_OGT                 KEY_SPST_CTL    

#define         KEY_VIDEO_OUTPUT_CTL    (INPUT_KEY_GROUP7+6)
// Micky0.80, add SP2 control key
#define         KEY_SPST2_CTL           (INPUT_KEY_GROUP7+7)


#define INPUT_KEY_GROUP8    140
// KEY_GROUP8, Setup Keys
// add setup keys
#define         KEY_SETUP               (INPUT_KEY_GROUP8+0)
//Re-arrnage the sequence to be the same as DVD Spec.
#define         KEY_UP                  (INPUT_KEY_GROUP8+1)
#define         KEY_DOWN                (INPUT_KEY_GROUP8+2)
#define         KEY_LEFT                (INPUT_KEY_GROUP8+3)
#define         KEY_RIGHT               (INPUT_KEY_GROUP8+4)


#define INPUT_KEY_GROUP9    160
// KEY_GROUP9, Other Keys
#define         KEY_NO_KEY              (INPUT_KEY_GROUP9+0)
#define         KEY_QUIT                (INPUT_KEY_GROUP9+1)
#define         INVALID_KEY             (INPUT_KEY_GROUP9+2)
#define         KEY_INVALID             INVALID_KEY             // ** TCH0.29;
#define         KEY_OSDGAME             (INPUT_KEY_GROUP9+3)    // 0.46,hwtan osd_game
// Micky0.80, add return value for XXX_ProcessKey.
#define         KEY_BYPASS              (INPUT_KEY_GROUP9+4)
#define         KEY_TODO                (INPUT_KEY_GROUP9+5)

#define INPUT_KEY_GROUP10   180         
#define INPUT_KEY_GROUPDVD  INPUT_KEY_GROUP10
// these are pseudo keys used in SETUP control
#define         KEY_TITLE               (INPUT_KEY_GROUPDVD+0)   // ** TCH0.26;
#define         KEY_MENU                (INPUT_KEY_GROUPDVD+1)   // ** TCH0.26;
#define         KEY_FP_PGC              (INPUT_KEY_GROUPDVD+2)   // ** TCH0.36; First Program

#define         KEY_ANGLE_CTL           (INPUT_KEY_GROUPDVD+5)
#define         KEY_ACTIVE              (INPUT_KEY_GROUPDVD+6) // ** TCH0.29; Active the button
//a hot key for user special use.
#define         KEY_HOTKEY              (INPUT_KEY_GROUPDVD+7)
//switch FullScreen on/off
#define         KEY_FULLSCREEN           (INPUT_KEY_GROUPDVD+8)


#define INPUT_KEY_GROUP11   200
// ++Aron.stb, The following keys are used for STB
#define         KEY_EXIT               	(INPUT_KEY_GROUP11+ 0)
#define         KEY_EVENT_KEY_FORCE_EXIT (INPUT_KEY_GROUP11+1)
#define         KEY_SERVICE_SWAP        (INPUT_KEY_GROUP11+ 2)
#define         KEY_SERVICE_LIST        (INPUT_KEY_GROUP11+ 3)
#define         KEY_TV_RADIO           	(INPUT_KEY_GROUP11+ 4)
#define         KEY_P_UP            	(INPUT_KEY_GROUP11+ 5)
#define         KEY_P_DOWN              (INPUT_KEY_GROUP11+ 6)
#define         KEY_TV_DISPLAY         	(INPUT_KEY_GROUP11+ 7)
#define         KEY_FAVORITY        	(INPUT_KEY_GROUP11+ 8)
#define         KEY_EPG        			(INPUT_KEY_GROUP11+ 9)
#define         KEY_SLEEP              	(INPUT_KEY_GROUP11+ 10)
#define         KEY_TTX           		(INPUT_KEY_GROUP11+ 11)
#define         KEY_RED         		(INPUT_KEY_GROUP11+ 12)
#define         KEY_GREEN        		(INPUT_KEY_GROUP11+ 13)
#define         KEY_YELLOW         		(INPUT_KEY_GROUP11+ 14)
#define         KEY_BLUE        		(INPUT_KEY_GROUP11+ 15)
#define         KEY_FAV_UP           	(INPUT_KEY_GROUP11+ 16)
#define         KEY_FAV_DOWN         	(INPUT_KEY_GROUP11+ 17)

#define         EVENT_KEY_FORCE_EXIT    KEY_EVENT_KEY_FORCE_EXIT
#define         KEY_GAME                KEY_OSDGAME
#define         KEY_INFO                KEY_DISPLAY
#define         KEY_SELECT              KEY_PLAY
#define         KEY_P_SCAN              KEY_PSCAN
#define         KEY_TV_SYSTEM           KEY_TVMODE
#define         KEY_AUDIO_MODE          KEY_AST_CTL
#define         KEY_SUBTITLE            KEY_SPST_CTL

//++CoCo2.60, for CT950 special mode keys
#define INPUT_KEY_GROUP12   220
#define         KEY_PHOTO               (INPUT_KEY_GROUP12+0)
#define         KEY_MUSIC               (INPUT_KEY_GROUP12+1)
#define         KEY_PHOTO_MUSIC         (INPUT_KEY_GROUP12+2)
#define         KEY_MOVIE               (INPUT_KEY_GROUP12+3)
#define         KEY_BROWSE              (INPUT_KEY_GROUP12+4)
#define         KEY_COPY_DEL            (INPUT_KEY_GROUP12+5)
#define         KEY_TIME            		(INPUT_KEY_GROUP12+6)
#define         KEY_DATE            		(INPUT_KEY_GROUP12+7)

// The following keys don't need to show OSD message 
// No need to add the correspond message into _bMessageMap[] array => save constant code size
#define INPUT_KEY_GROUP15   240
// KEY_GROUP15, AV system relative keys
#define         KEY_FUNCTION            (INPUT_KEY_GROUP15+ 0)  // switch AUX channel, ex DVD player/ Tuner/ AV1/ AV2
#define         KEY_AMP                 (INPUT_KEY_GROUP15+ 1)  // exter/ exit AMP mode
#define         KEY_BAND                (INPUT_KEY_GROUP15+ 2)  // switch FM/AM band
#define         KEY_TUNING_PLUS         (INPUT_KEY_GROUP15+ 3)  // up auto-scan
#define         KEY_TUNING_MINUS        (INPUT_KEY_GROUP15+ 4)  // down auto-scan
#define         KEY_PRESET_PLUS         (INPUT_KEY_GROUP15+ 5)  // jump to next memory station
#define         KEY_PRESET_MINUS        (INPUT_KEY_GROUP15+ 6)  // jump to previous memory station
#define         KEY_MEMORY              (INPUT_KEY_GROUP15+ 7)  // enter memory mode
#define         KEY_EQUALIZER           (INPUT_KEY_GROUP15+ 8)  // toggle Equalizer mode

#ifdef SUPPORT_TFT
#define INPUT_KEY_GROUP16   250
#define         KEY_PICTURE             (INPUT_KEY_GROUP16+ 0)  // Show TFT setup menu
#define         KEY_LCD                 (INPUT_KEY_GROUP16+ 1)  // LCD ON/OFF
#define         KEY_AV_IN               (INPUT_KEY_GROUP16+ 2)  //Switch AV IN Function //Jeff 20060705 Add
#define         KEY_BRIGHT_UP           (INPUT_KEY_GROUP16+ 3)  //Switch AV IN Function //Jeff 20060705 Add
#define         KEY_BRIGHT_DOWN         (INPUT_KEY_GROUP16+ 4)  //Switch AV IN Function //Jeff 20060705 Add
#endif
#define INPUT_KEY_END       255
// wyc1.05-909, add key for media selecting.
#define         KEY_SELECTMEDIA         (KEY_FUNCTION)


#endif  // __INPUT_H__

