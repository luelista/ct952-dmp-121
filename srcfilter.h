/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved

        Product : WinDVD Firmware

        Date    : 1999.8.26
        Purpose : Provide all SrcFilter information
        Sources :

****************************************************************************/

#ifndef __SRCFILTER_H__
#define __SRCFILTER_H__
#ifdef __cplusplus
        extern  "C"     {
#endif

//====================================
// declare type/structure/definition
//====================================
// define source type
#define     MAX_REMOVABLE_MEDIA     2

/*
enum    tagSOURCE_TYPE
{
    SOURCE_USB0,
    SOURCE_USB1,
    SOURCE_SERVO,
    SOURCE_ATAPI,
    SOURCE_NULL,    
    SOURCE_ALL = SOURCE_NULL,
    SOURCE_MAX_INDEX = SOURCE_NULL 
};
*/

// Brian0.65
//====================================
// declare type/structure/definition
//====================================
// Brian.ct909
// define Source type ID, use 8 bits: 
// [7:4]: Source Main ID
// [3:0]: Source Sub ID, mainly for USB card reader slot

#define SOURCE_NULL         0xFF // Brian0.67
#define SOURCE_ALL           SOURCE_NULL

#define SOURCE_ATAPI        0x10
#define SOURCE_SERVO        0x20

#define SOURCE_USB0_FIRST   0x30
#define SOURCE_USB0_0       SOURCE_USB0_FIRST
#define SOURCE_USB0_1       0x31
#define SOURCE_USB0_2       0x32
#define SOURCE_USB0_3       0x33
#define SOURCE_USB0_4       0x34
#define SOURCE_USB0_5       0x35
#define SOURCE_USB0_6       0x36
#define SOURCE_USB0_7       0x37
#define SOURCE_USB0_LAST    SOURCE_USB0_7

#define SOURCE_USB1_FIRST   0x40
#define SOURCE_USB1_0       SOURCE_USB1_FIRST
#define SOURCE_USB1_1       0x41
#define SOURCE_USB1_2       0x42
#define SOURCE_USB1_3       0x43
#define SOURCE_USB1_4       0x44
#define SOURCE_USB1_5       0x45
#define SOURCE_USB1_6       0x46
#define SOURCE_USB1_7       0x47
#define SOURCE_USB1_LAST    SOURCE_USB1_7

#define SOURCE_DVB          0x50
// wyc2.51-909P, add the source type SPI.
#define SOURCE_SPI          0x60

//Brian0.85, add range check define for removable source
#define SOURCE_REMOVABLE_FIRST  SOURCE_USB0_FIRST
#define SOURCE_REMOVABLE_LAST  SOURCE_USB1_LAST

// wyc1.01-909, modify to support max 2 bits.
// wyc2.53-909P, for support MP3/JPG play together, we increase one source count here. [0] mean current playing source information.
// [1] mean the backup of 1st source, [2] mean the backup of second source.
#define MAX_STREAM_COUNT        (2)     // We allow at most three input streams in the same time

typedef struct  tagSOURCE_FILTER_INPUT_STREAM_BUFFER
{
    BYTE    bSourceIndex;       // indicate this bit stream came from which source, ex. ATAPI, SERVO, USB, ...
    // PARSER module related members
    DWORD   dwDataFormat;       // specify the input data format
//    DWORD   EventReport;        // specify how many events needed to be reported

    // specify the memory buffer region for input bit stream DUMPIN
    DWORD   dwUpperAddr;
    DWORD   dwLowerAddr;
    DWORD   dwReadAddr;         // specify the read pointer, for buffer circular issue
    DWORD   dwWriteAddr;        // specify the write pointer
    DWORD   dwBuffLength;       // unit is sectors.

    // specify the total bit stream range wants to read
    DWORD   dwStartSector;
    DWORD   dwEndSector;
    DWORD   dwReadSector;       // indicate which sector that parser is processing
    DWORD   dwWriteSector;      // indicate which sector that source is reading

    // J500CSC_110, prevent some dvd titles' block start address is not alignment to 16 sectors.
    // This variable is updated by servo and not backward compatiable. Be careful if we want to
    // roll back to previous version.
    DWORD   dwBlockOffset;       // Indicate the start address's (LBA) least significant nibble of each block.
                                // This value is valid for DVD only and it is updated by servo when
                                // it received READ command.
    // wyc0.50-909
    // Other purpose variables.
    BYTE    bEnableRunning;
 } SOURCE_FILTER_INPUT_STREAM_BUFFER, *PSOURCE_FILTER_INPUT_STREAM_BUFFER;

//====================================
// declare global variables
//====================================
extern SOURCE_FILTER_INPUT_STREAM_BUFFER   __SF_SourceGBL[MAX_STREAM_COUNT];    // declare infortmation storage for each input stream


// Variables Extern Part.
// Brian0.65
extern DWORD   __dwSFStreamID;
extern DWORD    __dwSFDirRead;

// wyc1.06-909,
extern BYTE __bSFSourcePWDown;

// Global Variables
extern BYTE    __bCurrentIF; // using ATAPI_IF 
// wyc0.62, remove __bServoAutoCtrl
// wyc1.05-909, move from usbsrc.h to srcfilter.h because other module can't directly reference variables in usbsrc.
#ifdef SUPPORT_USB_SOURCE
extern WORD __wUSBSRCCluSize;
extern DWORD __dwUSBSRCPhyOffset;
extern DWORD __dwUSBStartPos;
#endif //


// wyc2.51-909P, extern variables.
#ifdef SUPPORT_ENCODE_JPG_PICTURE
extern  DWORD   __dwSFSPIStartAddr;
extern  DWORD   __dwSFSPIEndAddr;
#endif //

//LJY0.81, servo buffer config
// wyc2.76-909P,
// wyc2.77-909p, when not servo source, use another alignment format.
#ifdef SUPPORT_SERVO_SOURCE
#define SERVO_BUFSIZE_ALIGN_UNIT      (0xA000)
#else
#define SERVO_BUFSIZE_ALIGN_UNIT      (0x800)
#endif //

#define SECTOR_512  512 //2048/4=512DW
#define SECTOR_588  588 //2352/4=588DW
#define SECTOR_581  581 //2324/4=581DW, LJY1.24a, for VCD/SVCD 
#define SECTOR_607  607 //(2352+4*19)/5=607DW, For non-CDG RISC code, "55805580" will be inserted per 128bytes.
#define SECTOR_784  784 //((24B data+8B subcode)*98)/4=784DW, RISC won't insert "55805580"
//LJY1.23, share same define for CDG & pure CDDA
#ifdef SUPPORT_CDDA_WITH_SUBCODE
#define SECTOR_CDDA     SECTOR_784
#else
#define SECTOR_CDDA     SECTOR_588 //55805580+2352B for pure CDDA
#endif

//----------------------------------------------
//              SERVO definition
//----------------------------------------------
//LJY0.75, redefine it to conform with servo's definition 
#define DISC_TYPE_DVD           (0x00)
#define DISC_TYPE_CD            (0x80) 
#define DISC_TYPE_DVD_ROM       DISC_TYPE_DVD
#define DISC_TYPE_DVD_RAM       (0x01)
#define DISC_TYPE_DVD_R         (0x02)
#define DISC_TYPE_DVD_RW        (0x03)
#define DISC_TYPE_DVD_PLUS_RW   (0x09)
#define DISC_TYPE_DVD_PLUS_R    (0x0A) 
// wyc2.38a-909s, add one mode for DVD plus R dual layer disc.
#define DISC_TYPE_DVD_PLUS_R_DUAL   (0x0E)
#define DISC_TYPE_VCD           (0x14|DISC_TYPE_CD)
#define DISC_TYPE_CDI           (0x0A|DISC_TYPE_CD) 
#define DISC_TYPE_CDDA          (0x04|DISC_TYPE_CD)
#define DISC_TYPE_UNKNOWN       0xff

#define SERVO_MODE_CDROM        0x08
#ifdef CDDA_WAITSERVO
#define SERVO_MODE_AUDIO        0x10
#else
#define SERVO_MODE_AUDIO        0x00
#endif

// *** MULTI_DISC; Compatible with multiple-disc
#define SRCFILTER_NODISC                0x00
#define SRCFILTER_HAVEDISC              0x01
#define SRCFILTER_WAITSTABLE_TIMEOUT    0x02 //LJY1.05, it means servo cannot focus-on for power-on initial.
#define SRCFILTER_USER_ABORT            0x03 //LJY1.05-2
#define SRCFILTER_LOADDISC_FAILED       0x04 //LJY1.25, Support opentray when servo fails to close tray.
#define SRCFILTER_WAITING_STABLE        0x5
#define SRCFILTER_WRONGDISC                0x6
#define SRCFILTER_COMMAND_FAIL          0xff

// Error Status
#define ERR_RESPONSE            0xff
#define ERR_NOFOUND             0
#define ERR_SERVO_READ          1
#define ERR_SERVO_JUMP          2
#define ERR_SERVO_TRAY          3
#define ERR_SERVO_SET           4
#define ERR_SERVO_INFO          5

#define SERVO_END_TIME          0x7fffffffL

// TCC037, for control threshold
#define SERVO_THRESHOLD_DISABLE     0x01
#define SERVO_THRESHOLD_ENABLE      0x02
#define SERVO_THRESHOLD_VIDEO       0x03
#define SERVO_THRESHOLD_AUDIO       0x04
/////////////////////////////////
//LJY0.75, redefine tray status value for CT909
//LJY1.25, Support opentray when servo fails to close tray.
#define SRC_TRAY_OPENED         0x00
#define SRC_TRAY_OPENING        0x01
#define SRC_TRAY_CLOSED         0x02
#define SRC_TRAY_CLOSING        0x03
#define SRC_TRAY_USER_PUSH      0x04
#define SRC_TRAY_UNKNOWN        0xFF    // ** TCH1.05; 

// ** TCH1.05; begin... SrcFilter_GetStatus()
#define SRC_STATUS_NONE                         (0x00)
#define SRC_SPINDLE_STATUS_SPINNING       (0x08) 
// ** TCH1.05; end... 


////////////////////////////////////////////
// TCC172, For returning error status of the atapi command
#define SERVO_ERROR_NONE            0   // No Error
#define SERVO_ERROR_CMD             1   // Command Error
#define SERVO_ERROR_NOTREADY        2   // Device not ready(2/xx/xx)
#define SERVO_ERROR_NODISC          3   // No Disc in tray(2/3a/xx)
#define SERVO_ERROR_LOADDISC        4   // Loading disc (2/4/xx)
#define SERVO_ERROR_AUTH            5   // Authentication Error
#define SERVO_ERROR_INCOMPAT     6      // Imcompatible disc type. Usually incorrect command
#define SERVO_ERROR_TIMEOUT         7   // Timout Error. (atapi timeout)
#define SERVO_ERROR_READ            8   // Read Command Error
#define SERVO_ERROR_DMA             9   // DMA Error (DMA timeout w/o buffer full)
#define SERVO_ERROR_DEAD            10  // Dead Error (DMA timeout+buffer full)
#define SERVO_ERROR_USERABORT       11  // Means the command isn't issued/finished by IR key abort
#define SERVO_ERROR_UNKNOWN         12  // Unknow Error Type

////////////////////////////////////////////
// TCC155, default a/v threshold
#define SERVO_DEFAULT_VIDEO_THRESHOLD   0xa000
//Kevin0.90, modify
//Currently, audio buffer size=8KDW and audio buffer underflow threshold=5KDW. The 1KHz-AC3 break noise may be due to the RISC performance issue for that RISC has to frequently deal with the input bitstream when audio buffer remainder is less than the  underflow threshold.
//=> Temp. sloution: use a small audio underflow threshold, say 1.25KDW.  The setting seems OK for other DVD tiltes (AC3/DTS/LPCM). Integrate into DVD_090.
//#define SERVO_DEFAULT_AUDIO_THRESHOLD       0x1400/4  

// Brian0.90, before this issue is cleared, use the original value.
#define SERVO_DEFAULT_AUDIO_THRESHOLD   0x1400  

////////////////////////////////////////////
// TCC170, power mode for atapi
// wyc.171-3, modify the parameter for 
#define SERVO_POWER_STOP          0x0
#define SERVO_POWER_IDLE          0x20
#define SERVO_POWER_STANDBY       0x30
#define SERVO_POWER_SLEEP         0x50
#define SERVO_POWER_START         0x1
////////////////////////////////////////////
// wyc.170-2, add the define for CDDA audio de-emphasis
#define SERVO_CDDA_NONE             0
#define SERVO_CDDA_DEEMPHASIS       1
#define SERVO_CDDA_NON_DEEMPHASIS     2
////////////////////////////////////////////

//LJY0.95, wait stable mode
#define WAITSTABLE_SHOW_INFO            1
#define WAITSTABLE_NOT_SHOW_INFO        2
#define WAITSTABLE_SLEDGE                 3
#define SERVO_STATE_SLEDGE                4 //servo state of sledge-in is 4
#define WAITSTABLE_SLEDGE_SHOW_BAR      5 //LJY1.07, show loading status bar

//LJY1.10
#define AUDIO_DATA                          0x00
#define CONTINUOUS_DATA                0x01
#define INTERPOLATION_DISABLE       0x02 //bit1, for interpolation control
#define SUBCHANNEL_ENABLE           0x04  //bit2, for subcode control

#define SERVO_CGMS_NO_RESTRICTION   0
#define SERVO_CGMS_RESERVED         1
#define SERVO_CGMS_ONE_COPY         2
#define SERVO_CGMS_NO_COPY          3
#define SERVO_CGMS_NONE             4

// Define for other thread to communicate with ATAPI thread.
#define SRCFILTER_WAIT_STABLE                   0x00000001
#define SRCFILTER_WAIT_STABLE_DETECTING         0x00000002
#define SRCFILTER_WAIT_STABLE_HAVEDISC          0x00000004
#define SRCFILTER_WAIT_STABLE_NODISC            0x00000008
#define SRCFILTER_MONITOR_READDATA              0x00000010
#define SRCFILTER_MONITOR_READDATA_COMPLETED    0x00000020
// wyc1.20-909,
#define SRCFILTER_DUMPDATA                      0x00000040
#define SRCFILTER_DUMPDATA_COMPLETED            0x00000080

// elmer2.38
#define USBSRC_MSG_NO_CARD                      0x00000001
#define USBSRC_MSG_UNPLUG_CARD                  0x00000002
#ifdef SUPPORT_POWERON_MENU
#define USBSRC_MSG_RESUME			0x00000010
#endif

// ====== elmer ======
#define SRCFILTER_USBSRC_CHECK_DEVICE			0x00000001
#define SRCFILTER_USBSRC_RECOGNIZE_MEDIA		0X00000002
// wyc2.78-909P, new commands for usbsrc.
#define SRCFILTER_USBSRC_SWITCH_USB_STACK       0x00000020
#define SRCFILTER_USBSRC_DEVICE_MODE_CHK_CARD   0x00000040
#define SRCFILTER_USBSRC_EXIT_USB_STACK         0x00000080

#define SRCFILTER_USBSRC_CMD_ALL				0xFFFFFFFF


#define	SRCFILTER_FLAG_CMD						1
#define SRCFILTER_FLAG_STATUS					2
#define	SRCFILTER_FLAG_RUNNING					3
// ===================

#define SF_DUMPIN_BUFF_EMPTY    1
#define SF_DUMPIN_BUFF_FULL    2
#define SF_DUMPIN_BUFF_NORMAL    3
// ID for each source

// wyc1.07-909, add one state of connect state.
#define SRCFTR_USB_STATE_NO_MEDIA           1
#define SRCFTR_USB_STATE_CONN_MEDIA         2
#define SRCFTR_USB_STATE_READY_MEDIA        3
#define SRCFTR_USB_STATE_WRONG_MEDIA        4
// wyc1.20-909
#define SRCFTR_USB_STATE_NO_CARD            5


// wyc2.51-909P, define relative the JPG encode.
#define SRCFTR_SUPPORT_JPG_ENCODE_NUM       (15)
#define SRCFTR_SUPPORT_ENCODE_FLASH_SIZE    ((24*1024*1024)/8) // unit is bytes
#define SRCFTR_FLASH_RW_UNIT                ((64*1024)/8)
#define SRCFTR_ONE_JPG_ENCODE_SIZE          (64*1024)   // unit is bytes
// #define SRCFTR_ONE_JPG_ENCODE_SIZE          (((SRCFTR_SUPPORT_ENCODE_FLASH_SIZE/SRCFTR_SUPPORT_JPG_ENCODE_NUM)/SRCFTR_FLASH_RW_UNIT)*SRCFTR_FLASH_RW_UNIT) // unit is bytes
#define SRCFTR_ONE_JPG_ENCODE_SECTOR        (SRCFTR_ONE_JPG_ENCODE_SIZE/2048)

#define SRCFTR_SPI_ENCODE_ADDR              (0x110000)

// Structure Definition
// wyc.172r, define a new structure for scan title key usage and the purpose is to reduce one array.
typedef struct
{
 DWORD TitKey[2];  // Record the frame of the track start address
} SCAN_TITLE;

typedef struct
{
 BYTE Trk;  // Record the track number
 BYTE Ctl;  // Record the attribute of the track
 BYTE Min;  // Record the minute of the track start address
 BYTE Sec;  // Record the second of the track start address
 BYTE Frm;  // Record the frame of the track start address
} TRKINFO;

typedef struct
{
 BYTE bFirstTrack;  // Record the first track of the session
 BYTE bLastTrack;  // Record the last track of the session
 BYTE bLastSession; // Brian0.66, added by JY ??
 BYTE bDiscType; // Brian0.66, added by JY ??
 DWORD dwLeadOutPos;  // Record the leadout position of the session
} SESSIONINFO;

typedef struct {
    BYTE        bADRControl;
    BYTE        bTrackNum;
    BYTE        bPoint;
    BYTE        bRelMin;
    BYTE        bRelSec;
    BYTE        bRelFrame;
    BYTE        bReserved;
    BYTE        bAbsMin;
    BYTE        bAbsSec;
    BYTE        bAbsFrame;
} Q_CODE;

typedef struct tagCOPYINFO
{
    BYTE    bCPS_Type ;  // Store the Copy Protect Type of the dvd title
    BYTE    bRegionInfo ;  // Stores the Region Code Info of the dvd title
    BYTE    bCGMS ; // Store the Copy Generation Management System
} COPYINFO ;


//====================================
// declare global functions
//====================================
extern BYTE SrcFilter_Reset(BYTE bForceReset);
extern BYTE SrcFilter_Init(void);
extern BYTE SrcFilter_OpenTray(void);
extern BYTE SrcFilter_CloseTray(void);
extern BYTE SrcFilter_AudioDeemphasis(WORD wPlayItem);
extern BYTE SrcFilter_TriggerGettingMediaStatus(void);
extern BYTE SRCFilter_PollingMediaStatus(void);
extern BYTE SrcFilter_Stop(DWORD dwStream_id);
extern BYTE SrcFilter_ReadSectors(DWORD dwStream_id, DWORD dwStartPos, DWORD dwEndPos);

extern BYTE SrcFilter_GetDiscInfo(PBYTE pMinTrack, PBYTE pMaxTrack, PDWORD pMaxSector);
extern BYTE SrcFilter_GetTrackInfo(BYTE bTrackNum, PBYTE pTrackCtl, PDWORD pStartPos);
extern BYTE SrcFilter_ReadTOC( BYTE bTemp);
//extern BYTE SrcFilter_Monitor(void); //Brian1.15  // CoCo, reduce code size.
//extern BYTE SrcFilter_CheckDiscEnding(void); //Brian1.15  // CoCo, reduce code size.
//extern BYTE SrcFilter_ReadDiscKey(void); //Brian1.15  // CoCo, reduce code size.
extern WORD    SrcFilter_GetDiscType(void);
extern BYTE SrcFilter_CheckTrayStatus(void);
extern BOOL    SrcFilter_EnableRunning ( BYTE bGo );
//extern BYTE    SrcFilter_CSS_SentDiscKey(void); //Brian1.15  // CoCo, reduce code size.
// wyc1.06-909
extern BYTE SrcFilter_Seek (DWORD dwSeekLBA, BYTE  bResumeRead);
//extern void SrcFilter_AdjustThreshold (BYTE bType, DWORD dwLevel); //Brian1.15  // CoCo, reduce code size.
extern BYTE SrcFilter_ServoAbnormal(void);
extern BYTE SrcFilter_ReadTitleKey(DWORD dwSectorNumber, BYTE bStore);
extern BYTE SrcFilter_GetTrackEnd(BYTE bTrackNum, PDWORD pLength);
extern BYTE SrcFilter_SetTitleKey(BYTE bStore);
extern BYTE SrcFilter_GetDiscLength(PDWORD pLength);
extern BYTE SrcFilter_Abort(void);
extern BYTE SrcFilter_GetStatus(BYTE bSrcIndex);
extern void SrcFilter_ClearState(void);
extern BYTE SrcFilter_PowerMode (BYTE bMode);
extern  void    SRCFilter_ThreadDelete(void);
// LLY1.50, restore API since CELL module will use it.
BYTE SrcFilter_GetCurrentPos(PDWORD pCurrentPos); 
extern  BYTE    SRCFilter_GetMediaStatus(void);
// wyc1.05-909, remove function.
// wyc0.85, add one function to abort Get Media Status.
extern  void    SRCFilter_AbortGetMediaStatus(void);
// wyc2.78-909P,
extern BYTE SrcFilter_ConfigBuffer(BYTE bId, DWORD wBufStart, WORD bBufSize);
// wyc2.17a-909S
extern BYTE SrcFilter_CFGEDCBuff( DWORD dwStartAddr, WORD wSize );
// Brian0.69
extern void SrcFilter_InitVariables(void);
extern void SrcFilter_StartUnit(void);

extern  BYTE    SrcFilter_ResetRecover (BYTE bNeedStable);
// wyc2.53-909P,
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
extern BYTE SrcFilter_StreamPause(DWORD dwStream_id);
extern BYTE SrcFilter_StreamContinue(DWORD dwStream_id);
#endif //
void    ATAPI_Thread( DWORD index );

extern void    SRCFilter_EnableCacheFlush( void );      // F100CSC_102
// wyc1.05-909, new function for USB.
// wyc1.07-909, modify 3th parameter to DWORD* to support FAT32.
extern BYTE SRCFilter_DumpData(DWORD dwSecNO, DWORD dwNum, DWORD* pFATBuff);
// wyc1.20-909
extern BYTE SRCFilter_DumpDatabySource(DWORD dwSecNO, DWORD dwNum, DWORD* pFATBuff);
// wyc2.05-909S,
extern BYTE SRCFilter_ExitSource(void);
// wyc2.37-909s,
extern BYTE SRCFilter_USBSRCExit(void);


extern BOOL    SrcFilter_ChangeSourceID( DWORD dwStreamIdx, BYTE bSourceIdx );  // J500CSC_218

extern BOOL  SrcFilter_CheckSrcReady(VOID);// J200Aron_231


BYTE SrcFilter_TriggerUSBSRCCmd(BYTE flag, DWORD dwCmd);

BYTE SrcFilter_PeekUSBSRCCmd(BYTE flag, DWORD dwCmd);

void SrcFilter_ClearUSBSRCCmd(BYTE flag, DWORD dwCmd);
// wyc2.78-909P,
void SrcFilter_SetOTGMode(BYTE bUSBOTGMode);
BYTE SrcFilter_GetOTGMode(void);

#ifdef __cplusplus
        }
#endif
#endif  // __SRCFILTER_H__
