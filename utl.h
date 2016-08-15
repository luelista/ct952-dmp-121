
/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           L300, all right reserved.

        Product : VCD25 Firmware

        Date    : 1997.1.9
        Author  : Cheertek (D300 TCH)
        Purpose : Utility module
                  The moudle will support some utility function
        Sources : utl.h/ utl.c

****************************************************************************/

#ifndef __UTL_H__
#define __UTL_H__
// Micky1.07_909
extern BYTE        _bJumpTime;

/////////////////////////////////////////////////////////////////
// **** Extern global variable Area ****
extern  BYTE    __bModePlay;    // in utl.c
// wyc1.25, extern this value.
//LLY0.72, extern  BYTE    __bLastModePlay;
extern  BYTE    __bAttrPlay;  // playing mode attribute -- LLY.274p
extern  BYTE    __bAttrPlayNew; // new desired playing mode attribute --LLY.274p
extern  BIT     __btNTSCBitStream;      // current playing bitstream mode
//DVD_051Micky
// it records the stable time value
// to prevent the time value is undated by interrupt
//extern  DWORD   __dwSysTime;
// Micky2.80, review global variables init.
extern  BYTE    __bCheckAudioType;
extern  BIT     __btCheckVideoType;
extern  BYTE    __bModePlayPreCDROM;
extern  DWORD   _dwTriggerShowTime;
extern  DWORD   _dwGap_TriggerShowTime;

// LJY2.81, for scratched VCD time control
//#define SCRATCHED_VCD_ENHANCE
extern DWORD   __dwUTLPrevTime ;

// Keep previous scan time for next scan time-out control
extern  DWORD   __dwScanTime;

// LLY0.75, keep total time of current playback file.
extern DWORD   __dwFileTime;

// LLY0.76-1, keep sector number of one second for CDROM motion file
extern DWORD   __dwSectorPerSec;

// LLY0.80, keep desired Subpicture/ OGT type for decoder to set run length table
extern BYTE    __dwSPType;

// LLY0.85, keep desired video ID
extern BYTE    __bVideoID;

// LLY0.95, keep DRM play state information
extern BYTE    __bDRMPlayState;

// LLY0.95, keep DivX DRM remain view count
extern BYTE    __bDRMRemainView;

// LLY0.96, keep if the DivX index table exist
extern BYTE    __bNoDivXIndexTbl;

// LLY1.02, keep previous play item index
extern WORD   __wPrevPlayItem;

// LLY1.02, keep previous play item directory
extern WORD    __wPrevPlayDir;

// LLY1.02, the flag used check if the previous and new play item are same
extern  BYTE    __bSameItem;

// wyc2.14-909s, extern variable.
extern BYTE     _bUTLSrcBuffMod;

// LLY2.15, add a variable to keep desired debug mode
// Notice: must set the default value during _INITIAL_Variables_PowerON()
// SYuan2.77, Because add _dwDBGMode,  Be deleted.
//extern  BYTE    __bDebugMode;

// LLY2.55, add a variable to keep video content format of current AVI container
extern  DWORD   __dwAVIVideoFMT;

// Grievous2.60, add a variable to keep bits for each sample
extern BYTE __bBitsPerSample;



#ifdef NO_DVD //CoCo2.37p
extern WORD __wChapter;
extern WORD __wChapter_Ns_Title;
extern BYTE __bACHN;
extern WORD __wSP_LCD;
extern WORD __wA_LCD;
extern DWORD   __dw1stSCR;
#endif //#ifdef NO_DVD //CoCo2.37p

////////////////////////////////////////////////////////////////
// **** Global Define Area ****
#define     TRACK_GAP                   150L
#define     CDROM_RETEST_COUNT          2

#define     MODE1                       0x10
#define     MODE2                       0x20
#define     MODE2FORM1                  0x21
#define     MODE2FORM2                  0x22

// LLY1.01, define find next sequence header counter for pre-decode command
#define     FNSQ_CNT_FOR_PREDECODE      1


// LLY.101, define the audio checking type for __bCheckAudioType usage
// only allow 0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
#define     CHECK_NONE                  0x00
#define     CHECK_AUDIO_FREQ            0x01
// LLY.161-2, it's unnecessary by 20010928 DSP code
// because detecting CDDA method has modified
//#define     CHECK_CDDA_FORMAT           0x02
// wyc.170-2
#define     CHECK_LPCM_DEEMPHASIS       0x04

// LLY.161-2, define the stage for getting CDDA format
#define     UTL_GETCDDAFORMAT_INITIAL   0
#define     UTL_GETCDDAFORMAT_RUNTIME   1


// definitions

// Disc relative
#define     VTOC_SECTOR             16    //00:02:16
#define     BIH_SECTOR              75    //00:03:00
#define     INFO_VCD_SECTOR         150   //00:04:00
#define     ENTRIES_VCD_SECTOR      151   //00:04:01
#define     LOT_VCD_SECTOR          152   //00:04:02
#define     VCD_PSD_SECTOR          184   //00:04:34

//////////////////////////////////////
#define     CD_DATASECTOR           2352L

#define     DVD_DATASECTOR          2048L
#define     M2F1_DATASECTOR         2048L
#define     M2F1_SECTOR             2060L

#define     M2F2_DATASECTOR         2324L
#define     M2F2_SECTOR             2336L

#define     CDXA_HEADER             (12)
#define     SYNC                    (12)
#define     CDXA_START              (24)
//DVD017
#define     CDDA_DATASECTOR         2352L
#define     CDROM_START             (16)

// LLY.274p, re-define the playing mode relative info.
// [1] PlayMode definition; __bModePlay
#define     MODE_PLAYUNKNOW         0x00
#define     MODE_PLAYMOTION         0x01 // A/V mode
#define     MODE_PLAYVIDEO          0x02 // video only, ex JPG
#define     MODE_PLAYAUDIO          0x04 // audio only, ex CDDA/ MP3
#define     MODE_PLAYCDROM          0x08

#define     MODE_PLAYSTILLPAL       0x10
#define     MODE_PLAYSTILLNTSC      0x20
#define     MODE_PLAYSTILL          0x30
#define     MODE_PLAYSTILLHPAL      0x40
#define     MODE_PLAYSTILLHNTSC     0x80
#define     MODE_PLAYSTILLH         0xC0
#define     MODE_STILL              0xF0

// wyc1.21, redefine the ATTR of each mode and remind that each mode can't across 0xF, for example CDROM audio mode only can use from
// 0x10 ~ 0x1F because of 0x1F+1 = 0x20 and it will use ATTR_CDROM_VIDEO attribute. So totally can use 16 attribute types.
#define     ATTR_NONE               0x00

// the types
#define     TYPE_CDROM_AUDIO        0x10
#define     TYPE_CDROM_VIDEO        0x20
#define     TYPE_CDROM_AV           0x40
#define     TYPE_CDDA_DISC          0x80
// CDROM Audio relative attribute
#define     ATTR_AUDIO_BEGIN        TYPE_CDROM_AUDIO
#define     ATTR_MP3                (ATTR_AUDIO_BEGIN+0)
#define     ATTR_MP2                (ATTR_AUDIO_BEGIN+1)
#define     ATTR_WMA                (ATTR_AUDIO_BEGIN+2)
#define     ATTR_AC3                (ATTR_AUDIO_BEGIN+3)
#define     ATTR_DTS                (ATTR_AUDIO_BEGIN+4)
#define     ATTR_AUDIO_END          (ATTR_DTS)

// Video relative attribute
#define     ATTR_VIDEO_BEGIN        (TYPE_CDROM_VIDEO)
#define     ATTR_JPG                (ATTR_VIDEO_BEGIN+0)
#define     ATTR_MPEG               (ATTR_VIDEO_BEGIN+1) // LLY0.64-1
#define     ATTR_VIDEO_END          (ATTR_JPG)

// A/V both relative attribute
#define     ATTR_AV_BEGIN       (TYPE_CDROM_AV)
#define     ATTR_DAT            (ATTR_AV_BEGIN + 0)
#define     ATTR_MPG            (ATTR_AV_BEGIN + 1)
#define     ATTR_VOB            (ATTR_AV_BEGIN + 2)
#define     ATTR_AVI            (ATTR_AV_BEGIN + 3)
#define     ATTR_AV_END         (ATTR_AVI)

#define     ATTR_CDDA           (TYPE_CDDA_DISC)


// Micky1.21, CodeReview --- remove still mode relative attribute.


// F/W Mode definition; UTL_ModeInitial()
//#define     MODE_NONE               0x00
#define     MODE_LINEAR             0x01
#define     MODE_PBC                0x02
#define     MODE_PROGRAM            0x04
#define     MODE_RANDOM             0x08
#define     MODE_OPENCLOSE          0x10


// LLY2.60, re-adjust the ID value for UTL_QueryCapability()
// ie. the ID can be continue since bit-address is no used.
#define     QUERY_STRICTUOP                         0x0001  // Strict UOP
#define     QUERY_VIRTUAL_DTS                       0x0002  // Virtual DTS
#define     QUERY_SUPPORT_PASSWORD_QUERY            0x0003  // Parental control password query -- Micky0.96
// LLY1.10, add new query for checking valid AVI file, ie. A/V all valid
// Otherwise, only playback Audio or Video only; or skip the file if A/V all non-valid
#define     QUERY_VALID_MOTION                      0x0004  // valid motion file
#define     QUERY_KEYSET_STOP                       0x0005
#define     QUERY_SUPPORT_ANTISHOCK                 0x0006  // support anti-shock
// DVD_172Micky, add two more querys for checking abort.
// for the consistency between main flow and other functions.
#define     QUERY_ABORT_SERVO                       0x0007
#define     QUERY_ABORT_MAINFLOW                    0x0008
//Aron2.31-909R, add for abort nav control
#define     QUERY_ABORT_NAVCTRL                     0x0009
//Aron2.38-909R,support still resuming
#define     QUERY_SUPPORT_STILLRSM                  0x000A

// Let ID from 0xXX00 for query current solution setting case
#define     QUERY_ASPECT_RATIO_SETTING              0x0100      // Aspect Ratio
#define     QUERY_DIGITAL_AUDIO_OUTPUT_SETTING      0x0200      // Digital audio output
#define     QUERY_PARENTAL_CONTROL_LEVEL_SETTING    0x0300      // Parental Control Level
#define     QUERY_PSCAN_SETTING                     0x0400      // Progressive scan




// LLY0.62, define the ID for __bLOGO, only use bit[3:0]
#define LOGO_NONE               0
#define LOGO_DEFAULT            1
#define LOGO_CAPTURE            2
#define LOGO_JPEG               3
#define LOGO_CDG                4
// define the LOGO attribute, only use bit[7:4]
#define LOGO_CHANGE_TV_TYPE     0x80

// LLY0.80, define two macro to get LOGO type and LOGO attribute
#define LOGO_TYPE()             (__bLOGO&0x0F)
#define LOGO_ATTR()             (__bLOGO&0xF0)


// LLY0.95, Define the DRM playback state for __bDRMPlayState usage
#define DRM_PLAY_STATE_CONTINUE     0   // continue the playback.
#define DRM_PLAY_STATE_WAIT         1   // wait user operation.
#define DRM_PLAY_STATE_ABORT        2   // about the playback, and return to menu
#define DRM_PLAY_STATE_SKIP         3   // skip the item, and play next item


// LLY1.01, define the ID for UTL_PreScanAction() 1st parameter usage
// bit[7:4] means prescan mode; bit[3:0] means prescan sub-mode
#define PRESCAN_MOTION_INFO         0x10
#define PRESCAN_SCR_VALUE           0x20
    #define PRESCAN_1ST_SCR             0x21
    #define PRESCAN_LAST_SCR            0x22
#define PRESCAN_MOTION_IDXTBL       0x40



// LLY1.01, define the ID for UTL_SetPreScanError() usage
#define ERR_1ST_PRESCAN_FAIL        0
#define ERR_NOT_FIND_LAST_SCR       1
#define ERR_NOT_FIND_AVI_IDXTBL     2

// LLY1.06, define the ID for UTL_PreStartServo() 2nd parameter
#define ACTION_CONTINUE     0
#define ACTION_SKIP         1

// wyc2.14-909s, move define from infofilter.h to here.
#define     DUMP_BUFF_NORMAL_MODE               1
// LLY2.21a, split MM UI and MM title detection mode since OSD is different.
#define     DUMP_BUFF_MM_TITLE_DETECT_MODE      2   // For general Media Manager mode, only used for title detection.
#define     DUMP_BUFF_MM_UI_MODE                3   // For MM UI display only mode
#define     DUMP_BUFF_MM_AVI_MODE               4
#define     DUMP_BUFF_MM_MOTION_MODE            5
// LLY2.21, add Media Manager Mode's audio only and video only two cases.
#define     DUMP_BUFF_MM_AUDIO_ONLY_MODE        6
#define     DUMP_BUFF_MM_VIDEO_ONLY_MODE        7   // JPEG slide show/ video only file
// LLY2.22, split JPEG preview and thumbnail from MM video only mode
// Since, the buffer config are different.
#define     DUMP_BUFF_MM_JPEG_PREVIEW_MODE      8
#define     DUMP_BUFF_MM_JPEG_THUMBNAIL_MODE    9
#define     DUMP_BUFF_AP_MODE                   10
#define     DUMP_BUFF_DVB_MODE                  11
// LLY2.22, add new case for CDG title detection
// Since, it will config servo dump in buffer and parser data dump buffer @ different mode.
#define     DUMP_BUFF_CDG_DETECT_MODE           12
#define     DUMP_BUFF_MM_MJPG_MODE		        13  // CMH2.55, add motion jpeg buffer mode


// LLY2.15, define the ID for __bDebugMode
// Notice: must use bit addressing, DSU: bit[2], UART1: bit[1], UART2: bit[0]
#define UTL_DBG_DSU1_EN     0x4
#define UTL_DBG_UART1_EN    0x2
#define UTL_DBG_UART2_EN    0x1
#define UTL_DBG_NONE        0x0

// LLY2.15, define the ID for UTL_Config_DebugMode() 1st parameter
#define UTL_DBG_INIT        1   // Do initial and setting
#define UTL_DBG_SET         2   // Do setting only


// LLY2.17, define the ID for UTL_Config_FrameBuffer() 1st parameter
// LLY2.22, split them into four groups since different mode maybe same group
// Since, JPEG slide show and MM UI/ JPEG thumbnail, JPEG preview, MM audio only are different mode.
#define UTL_FRAMEBUF_NORMAL         0x10
#define UTL_FRAMEBUF_MM_SLIDE_SHOW  0x20
#define UTL_FRAMEBUF_MJPEG          0x30   // Motion JPEG Frame Buffer
#define UTL_FRAMEBUF_MM             0x40
#define UTL_FRAMEBUF_CDG            0x80

#define UTL_FRAMEBUF_MOTION         (UTL_FRAMEBUF_NORMAL+1)   // DVD/ VCD/ CVD/ SVCD motion
#define UTL_FRAMEBUF_MM_MOTION      (UTL_FRAMEBUF_NORMAL+2)   // Data Disc motion
#define UTL_FRAMEBUF_MP4_MOTION     (UTL_FRAMEBUF_NORMAL+3)   // MP4 motion
#define UTL_FRAMEBUF_DIGEST         (UTL_FRAMEBUF_NORMAL+4)   // Digest mode

#define UTL_FRAMEBUF_JPEG_SLIDE     (UTL_FRAMEBUF_MM_SLIDE_SHOW+1)   // JPEG Slide Show
// LLY2.22, split JPEG preview and thumbnail into two modes
#define UTL_FRAMEBUF_JPEG_PREVIEW   (UTL_FRAMEBUF_MM+1)   // JPEG preview
#define UTL_FRAMEBUF_JPEG_THUMBNAIL (UTL_FRAMEBUF_MM+2)   // JPEG thumbnail
#define UTL_FRAMEBUF_MM_UI          (UTL_FRAMEBUF_MM+3)   // MM UI mode
#define UTL_FRAMEBUF_MM_AUDIO_ONLY  (UTL_FRAMEBUF_MM+4)   // CDROM/DVDROM Audio only file

#define UTL_FRAMEBUF_UNKNOWN        0xFF


// LLY2.21, define the ID for _UTL_GetBitsContent() 1st parameter.
#define INFO_AUDIO      0
#define INFO_SPST       1
#define INFO_AVI_VIDEO  2
#define INFO_FILETIME   3


// LLY2.32, add for UTL_CheckNVErrorCode() return value
#define NV_FAIL_BY_READ     0   // servo read error
#define NV_FAIL_BY_FORMAT   1   // NV sector format is wrong

//Aron2.78-909P created
#define DVD_LOAD_VTS_FAIL   1
#define DVD_LOAD_TTPGC_FAIL 2
#define DVD_NEXT_LINK_ERR   4

// LLY2.60 begin ...
// The section is uesed to define the value for UTL_QueryCapability() to query current solution value.
// Notice: the value can't be zero since '0' means not support case
// The value for aspect ratio mode setting (SETTING_ASPECT_RATIO)
#define ASPECT_RATIO_MODE_LB                1   // 4:3 LetterBox
#define ASPECT_RATIO_MODE_PS                2   // 4:3 PanScan
#define ASPECT_RATIO_MODE_WIDE              3   // 16:9 WIDE

// The value for digital audio output mode setting (SETTING_DIGITAL_AUDIO_OUTPUT)
#define DIGITAL_AUDIO_OUTPUT_SPDIF_RAW      1   // SPDIF-RAW
#define DIGITAL_AUDIO_OUTPUT_SPDIF_PCM      2   // SPDIF-PCM
#define DIGITAL_AUDIO_OUTPUT_OFF            3   // Off

// The value for progressive scan mode (SETTING_PSCAN)
#define PSCAN_MODE_ON                       1   // PScan mode on
#define PSCAN_MODE_OFF                      2   // PScan mode off
// LLY2.60 end ...

//------------------------------------------------
//For transfer DSU/UART SOURCE to DWORD.
//SYuan2.77-909P
//-------------------------------------------------
#define     SET_DSU1_TX_VALUE(x)            (x)
#define     SET_DSU1_RX_VALUE(x)            (x) << 4
#define     SET_UART1_TX_VALUE(x)           (x) << 8
#define     SET_UART1_RX_VALUE(x)           (x) << 12
#define     SET_UART2_TX_VALUE(x)           (x) << 16
#define     SET_UART2_RX_VALUE(x)           (x) << 20

//------------------------------------------------
//For get DSU/UART SOURCE from DWORD.
//SYuan2.77-909P
//-------------------------------------------------
#define     GET_DSU1_TX_VALUE(x)     (x) & 0xf
#define     GET_DSU1_RX_VALUE(x)    ((x) & 0xf0) >> 4
#define     GET_UART1_TX_VALUE(x)   ((x) & 0xf00) >> 8
#define     GET_UART1_RX_VALUE(x)   ((x) & 0xf000) >> 12
#define     GET_UART2_TX_VALUE(x)   ((x) & 0xf0000) >> 16
#define     GET_UART2_RX_VALUE(x)   ((x) & 0xf00000) >> 20



////////////////////////////////////////////////////////////
// structure definition
#ifndef     DEF_UNIONWORD
#define     DEF_UNIONWORD
typedef union tagUNIONWORD
{
// ** TCH0.99-1;     char    cS[2];
    BYTE    cS[2];
    WORD    nVal;
}   UWORD;
#endif

#ifndef     DEF_UNIONDWORD
#define     DEF_UNIONDWORD
typedef union tagUNIONDWORD
{
// ** TCH0.99-1; ///    char    cS[4];
    BYTE    cS[4];
    DWORD   dwVal;
}   UDWORD;
#endif

//////////////////////////////////////////////////////
// Function Prototype
BYTE    UTL_ShowJPEG_Slide(BYTE bType, BYTE bFrameIdx); //CoCo2.78, add "bType" parameter
BYTE    UTL_ShowLogo(void);
// LLY2.35, give 2nd parameter for end sector address since FAT system need it.
//BIT     UTL_PlayFromTime ( DWORD dwTime );
BIT     UTL_PlayFromTime(DWORD dwStart, DWORD dwEnd);
//void    UTL_PreScanInfo(BYTE bAttr); // LLY0.72
// LLY1.01, split issue pre-scan & get information procedure.
BYTE    UTL_PreScanAction(WORD wMode, DWORD dwStart, DWORD dwEnd);
void    UTL_Read_PreScan_Info(WORD wMode);
void    UTL_SetPreScanError(BYTE bErr);
void    UTL_MovieASTSetting(BYTE bID); // LLY0.80
BYTE    UTL_SetPlayMode(BYTE bMode);
void    _UTL_ProgAudioOnly(void);
void    _UTL_ProgVideoOnly(void);
void    _UTL_ProgMotion(void);
void    _UTL_Update_Movie_AudioInfo(BYTE bMode, BYTE bID);

#ifndef SIMP_UTL
// LLY2.15, add for config debug mode
// SYuan2.77, add 2nd parameter for UTL_Config_DebugMode()
void    UTL_Config_DebugMode(BYTE bAction, DWORD dwPath);
DWORD UTL_Get_Path_DebugMode(void);
//SYuan2.77-909P, add for getting debug mode now.
DWORD   UTL_Get_DebugMode(void);
// LLY2.17, add for frame buffer config usage
void    UTL_Config_FrameBuffer(BYTE bMode);
// LLY2.20, raname it as UTL_Config_BufferMode()
void    UTL_Config_BufferMode(BYTE bBuffMode);
void    UTL_BFRModeSet(BYTE bFlag); // LLY2.22
void    UTL_Init_PlayItem(WORD nItem);
void    UTL_Start_PlayItem(DWORD dwTime);
WORD    UTL_PlayItem ( WORD nItem, DWORD dwTime );
BYTE    UTL_CheckPlayItemMode ( WORD nITEM );
BYTE UTL_CheckBGPlayItemMode ( WORD nItem );
BYTE    UTL_SetPlayItemMode ( WORD nItem );
BIT     UTL_GetPlayRange ( WORD nItem );
DWORD   UTL_GetCurrentTime (void);
// LLY2.04 create ...
DWORD   UTL_GetDecodedTime(void);
DWORD   UTL_FrameCntToSectors(void);
DWORD   UTL_SectorsToFrameCnt(DWORD dwSector);
// LLY2.04 end.

// LLY2.35, add new API to get desired sector ID base on WMA time
DWORD   UTL_WMATimeToSectors(DWORD dwTime);
DWORD   UTL_MStoSectors ( BYTE bHour, BYTE bMin, BYTE bSec );
DWORD   UTL_SectorstoMSF ( DWORD hsgTime );
void    UTL_ModeInitial ( BYTE bMode );
// LLY0.72, don't return value since nobody reference it.
//BIT     UTL_ForceChangeTrack ( WORD wTrack );
void    UTL_ForceChangeTrack(WORD wTrack);
BYTE    UTL_TriggerShowTime (void);     // ** TCH0.99-1; Call this function for ShowTime
BYTE    UTL_OutputShowTime (void);      // ** TCH0.99-1; Modify return value
WORD    UTL_EstimateSectors ( DWORD dwTime );
void    UTL_ShowTrack(BYTE bMessage, WORD wPlayItem, BYTE bSec);
//BYTE    UTL_TransferCommandID(BYTE bKey);
void    UTL_TransferCommandID(BYTE bKey);
DWORD   UTL_QueryCapability(WORD wKind );
void    UTL_GetCDDAFormat(BYTE bStage);  // LLY.161-2
BYTE    UTL_SetCDDADeEmphasis(void);
void    UTL_PreStartServo(DWORD dwPosition, BYTE bAction);
//LJY1.24a, add an API for OSD time or resume compensation
DWORD   UTL_EstimateBufferTime(void);
DWORD   UTL_JumpGroove(BYTE bMode, DWORD dwTime);
// LLY2.32, add for check NV fail status
BYTE	UTL_CheckNVErrorCode(void);

// LLY0.95, add new APIs for DivX DRM playback
BYTE    UTL_DRM_PlaybackCapability(void);
BYTE    UTL_DRM_StartPlayback(void);


void    MONITOR_VideoStream(void);
void    MONITOR_AudioStream(void);
BIT     MONITOR_CheckBuffer(void);

//Aron2.36a created, check the events in the while loop
void    UTL_EventTrigger(void);

//Aron2.78-909P created
void    UTL_ProcNavError(BYTE bErrType);

// LLY2.53, porting Chuan's code to do unzip operation.
int     UTL_Decompress(PBYTE pSrc, PBYTE pDest, PBYTE pWorkMem);

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
// LLY2.53, porting YC's code to support stream sending pause/ continue
BYTE UTL_StreamPause(BYTE bStream_id);
BYTE UTL_StreamContinue(BYTE bStream_id);

// LLY2.53, add new API to get background playitem range
BYTE UTL_GetPlayRange_BGItem(WORD nItem, PDWORD pdwStart, PDWORD pdwEnd);

// LLY2.53, add new API to support audio only/ video only playback.
void    UTL_PlayAudio(BYTE bMode);
void    UTL_StopAudio(void);
void    UTL_PlayImage(BYTE bMode);
void    UTL_StopImage(BYTE bMode);
// LLY2.53, add new API to support background item playback
BIT     UTL_PlayFromTime_BGItem(DWORD dwStart, DWORD dwEnd);
#endif  // #ifdef SUPPORT_JPEG_AUDIO_PLAYING


/////////////////////////////////////////////////
// protected functions
DWORD   _UTL_CalTime_FromFrameCnt(void);
DWORD   _UTL_CalTime_FromSector(void);
#ifndef NO_DVD //CoCo2.37p
DWORD   _UTL_CalTime_FromSTC(void);
#endif //#ifndef NO_DVD //CoCo2.37p

void    _Play_TimeA(void);


#endif  // #ifndef SIMP_UTL
// Grievous2.78, this function should be declare
void    _UTL_PreConfig_AVIBuffer(void);
// Grievous2.78, add a variable to keep internal/external subpicture mode
extern BYTE    __bUTLSPTextMode;
#endif  // __UTL_H_
