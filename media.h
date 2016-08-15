#ifndef __MEDIA_H__
#define __MEDIA_H__

#ifdef __cplusplus
        extern  "C"     {
#endif
// wyc1.01-909, re-allocate the layout.
// ============================= Definition =============================
#define SUPPORT_MEDIA_MANAGEMENT

// Micky1.00_909_media
#define MEDIA_SERVO_NONE            0
#define MEDIA_SERVO_CLOSING         1
#define MEDIA_SERVO_CLOSED          2
#define MEDIA_SERVO_STABLE          3
#define MEDIA_SERVO_PARSING         4
#define MEDIA_SERVO_PARSING_DONE    5
#define MEDIA_SERVO_PLAYING         6
#define MEDIA_SERVO_PROMPT          7

#define MEDIA_SERVO_ACTION_NONE     0
#define MEDIA_SERVO_ACTION_CLOSE    1
//#define MEDIA_SERVO_ACTION_OPEN     2

#define MEDIA_ACTION_USB_NONE       0
#define MEDIA_ACTION_USB_INSERT    1
//#define MEDIA_ACTION_USB_REMOVE    2

#define MEDIA_USB_NONE          0
#define MEDIA_USB_INSERT        1
#define MEDIA_USB_RECOGNIZE     2
#define MEDIA_USB_PARSING       3
#define MEDIA_USB_READY         4
#define MEDIA_USB_WRONG         5

// The value for MEDIAINFO bState
#define MEDIA_NONE      0x0   // no device
#define MEDIA_INSERT    0x1
#define MEDIA_RECOGNIZE 0x2
#define MEDIA_PARSING   0x4   // parsing
#define MEDIA_READY     0x8   // parse OK
#define MEDIA_WRONG     0x10   // wrong mass storage media
#define MEDIA_SUSPEND   0x20

#define MEDIA_READY_STOP    0x00    // the attribute in ready mode, attr is stop mode.
#define MEDIA_READY_PLAY    0x80    // the attribute in ready mode, attr is play mode.

#define MEDIA_STOP      (MEDIA_READY | MEDIA_READY_STOP)
#define MEDIA_PLAY      (MEDIA_READY | MEDIA_READY_PLAY)

// elmer.card
#define MEDIA_DEVICE_USB    0x00010000
#define MEDIA_DEVICE_SD     0x00000001
#define MEDIA_DEVICE_MS     0x00000002
#define MEDIA_DEVICE_MMC    0x00000004
#define MEDIA_DEVICE_CF     0x00000008
#define MEDIA_DEVICE_XD     0x00000010

#define PLAYING_NONE    0
#define PLAYING_NORMAL  1
#define PLAYING_PAUSE   2
#define PLAYING_END     3
#define KEY_PLAY_ITEM1  250
#define KEY_PLAY_ITEM2  251

// MAX_REMOVABLE_MEDIA is defined by srcfilter
// Micky0.85
// wyc.check, now temply only support one USB port, will add to 8 ports later code.
#define PER_USB_NO      (1)
#define SUPPORT_USB_PORT_NUM    (1)
#define TOTAL_MEDIA    ((PER_USB_NO*SUPPORT_USB_PORT_NUM)+2)    // should accord to srcfilter.h

// must define as bit 
#define MEDIA_CONTROL_STOP  1   // stop current media playback
#define MEDIA_CONTROL_KILL  2   // delete information
#define MEDIA_CONTROL_NONE  4   // stay at LOGO
#define MEDIA_CONTROL_CLEAR_ALL    8    // delete all information
#define MEDIA_CONTROL_DELETE    (MEDIA_CONTROL_STOP|MEDIA_CONTROL_KILL|MEDIA_CONTROL_NONE)

// to be removed after OSD add it.
#define OSD_UI_CLEAR_NORMAL     0
#define OSD_UI_CLEAR_FORCE      1

#if 0
// wyc1.21-909, new define in select media.
#define MEDIA_SELECT_BEGIN              (0)
#define MEDIA_SELECT_DVD                (MEDIA_SELECT_BEGIN)

// wyc2.21-909s, add members' code.
#ifdef SUPPORT_STB
#define MEDIA_SELECT_STB                (MEDIA_SELECT_BEGIN+1)
#define MEDIA_SELECT_USB                (MEDIA_SELECT_BEGIN+2)
#define MEDIA_SELECT_CARD_READER        (MEDIA_SELECT_BEGIN+3)
#define MEDIA_SELECT_UNKNOW             (MEDIA_SELECT_BEGIN+4)
#else
#define MEDIA_SELECT_USB                (MEDIA_SELECT_BEGIN+1)
#define MEDIA_SELECT_CARD_READER        (MEDIA_SELECT_BEGIN+2)
#endif
#ifdef SUPPORT_BUILDIN_CARD_READER
#define MEDIA_SELECT_END                (MEDIA_SELECT_CARD_READER)
#else
#define MEDIA_SELECT_END                (MEDIA_SELECT_USB)
#endif //
#endif

#ifdef SUPPORT_AV_IN_BY_PIN
#define MEDIA_SELECT_BEGIN  0
enum MEDIA_SELECT_MEDIA
{
    MEDIA_SELECT_DVD     = MEDIA_SELECT_BEGIN

#ifdef SUPPORT_STB
   ,MEDIA_SELECT_STB
#endif //

#ifdef SUPPORT_USB_SOURCE
   ,MEDIA_SELECT_USB
#endif //

#ifdef SUPPORT_BUILDIN_CARD_READER
    ,MEDIA_SELECT_CARD_READER
#endif //

   ,MEDIA_SELECT_END

#ifdef SUPPORT_AV_IN
   ,MEDIA_SELECT_AV_IN
#endif //


/* not use */
   ,MEDIA_SELECT_UNKNOW              

};
#else
#define MEDIA_SELECT_BEGIN  0
enum MEDIA_SELECT_MEDIA
{
    MEDIA_SELECT_DVD     = MEDIA_SELECT_BEGIN

#ifdef SUPPORT_STB
   ,MEDIA_SELECT_STB
#endif //

#ifdef SUPPORT_USB_SOURCE
   ,MEDIA_SELECT_USB
#endif //

#ifdef SUPPORT_BUILDIN_CARD_READER
    ,MEDIA_SELECT_CARD_READER
#endif //

#ifdef SUPPORT_AV_IN
   ,MEDIA_SELECT_AV_IN
#endif //

   ,MEDIA_SELECT_END

/* not use */
   ,MEDIA_SELECT_UNKNOW              

};
#endif
// Micky0.65-909.
// should define SOURCE_DISC as SOURCE_SERVO 
#ifdef SUPPORT_SERVO_SOURCE
#define SOURCE_DISC SOURCE_SERVO
#else
#define SOURCE_DISC SOURCE_ATAPI
#endif

// ============================= Structure Define =============================
typedef struct tagMEDIAINFO
{
    BYTE        bState;
    // to be removed
    BYTE        bPresent;
} MEDIAINFO;

// ============================= Extern Variables =============================
extern  BYTE    __bMediaServoState;
extern  BYTE    __bMediaServoAction;
extern  BYTE    __bMediaOtherState;
extern  BYTE    __bMediaID;
// wyc1.05-909, add extern variable
extern  BYTE    __bChooseMedia;

// Micky_909_play_item
extern  WORD    __wNewItem;
extern  WORD    __wNewCommand;
extern  BYTE    __bPlayingState;
extern  BYTE    __bMacroCmdMsg;
extern  BYTE    __bNavigateMode;
extern  BYTE    __bMediaSelectUI;
// wyc1.21-909, extern variable.
extern  DWORD _dwUSBSRCActivePort;

extern MEDIAINFO   MediaInfo[TOTAL_MEDIA];

#ifdef SUPPORT_STB
extern BYTE    __bPreMediaMode;
extern BYTE    __bMediaSelected;
#endif

// ============================= Extern Functions =============================
BYTE MEDIA_Management(void);    // ** TCH0.60; 
BYTE MEDIA_MediaSelection_ProcessKey(void);
void MEDIA_InitialState(void);  // ** TCH0.60; 
void MEDIA_Removed(BYTE bSource);
void MEDIA_StateChange(BYTE bMode);
void MEDIA_MonitorStatus(void); // ** TCH0.60; 
// wyc2.17-909S,
void MEDIA_MonitorTOPOPEN(void);
void MEDIA_MediaSelection_Exit(BYTE bMode);
// wyc2.22-909s,
BYTE MEDIA_ShowSelDlg(VOID);
#ifdef SUPPORT_STB
VOID MEDIA_SwitchMode(BYTE bMode);
void MEDIA_STB(void);
VOID MEDIA_ResetNIM(VOID);
VOID MEDIA_DecidetMedia(void);
#endif
// wyc2.78-909P,
#ifdef SUPPORT_USB_HOST_DEVICE
void MEDIA_MonitorAndConfigUSBOTG(void);
#endif //
// wyc0.85a, move to here because INFIFILTER will reference it.
BYTE _MapSourceID2Index(BYTE bSourceID);
void CC_StartPLay(BYTE bStartType, WORD wNewCommand);

#ifdef SUPPORT_POWERON_MENU
void MEDIA_USB(void);
void MEDIA_ExitUSBSRC(void);
#ifdef SUPPORT_CHOOSE_MEDIA
// elmer.card
DWORD MEDIA_QueryDevice(void);
DWORD MEDIA_QueryDefaultDevice(void);
VOID MEIDA_SetDefaultDevice(DWORD);
#endif
#endif

#ifdef __cplusplus
   }
#endif

#endif      // __MEDIA_H__
