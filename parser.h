//**************************************************************************
//         Copyright (c) 2004, Cheertek Inc . All rights reserved.
//         D300, all right reserved.
//
//	Product	: CheerDVD Firmware
//	Date	: 2004.12.12.
//	Author	: Cheertek (D300 CSC)
//	Purpose	: Parses different kind bit streams, MPEG & AVI, into elementary streams
//            , audio/video/subtitle, ..., etc.
//            It also handles the copy protection issue.
//	Sources	: parser.h/ lparser.h/ parser.c/ parser_mpeg.h/ parser_mpeg.c/
//            parser_avi.h/ parser_avi.c/ parser_prescan.c/
//***************************************************************************

// Modification use :
//**************************************************************************
//	Update
//	Date  	: 2004.12.12
//	Name	: Cheertek (D300 CSC)
//	Description	: 
//
//			Item			Source
//	----------------------------------------		-------------------

#ifndef __PARSER_H__
#define __PARSER_H__

#ifdef WIN32_TOOL
#include "ctkav.h"
#else // WIN32_TOOL
// J500KWShih_216, Support CT909 Component AP and cross platform issue for Windows 909 Player
//#include <cyg/kernel/kapi.h>
//#include <cyg/hal/hal_intr.h>
//#include <cyg/hal/hal_arch.h>          // Architecture specific definitions
//#include "winav.h"
#include "ctkav.h"
#include "srcfilter.h"
#include "comdec.h"
#include "disp.h"
#include "initial.h"
#endif // WIN32_TOOL

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


// K200 John, added thread delay to release CPU resource for CT216U
//#define PARSER_SUPPORT_216U

//#define PARSER_CODE_DEBUG           // enable some debug code for check point
//#define PARSER_DETAIL_DEBUG_INFO    // enable detail debug information
//#define PARSER_NORMAL_DEBUG_INFO    // enable normal (necessary) debug information
// J500KWShih_235, using stream rate to calculate video PTS
#define PARSER_AVI_VIDEO_PTS_STREAM

// J500KWShih_237, support MP4 (Nero Digital)
// #define PARSER_SUPPORT_MP4

//====================================
// declare type/structure/definition
//====================================
// declare commands supported by PARSER module
#define PARSER_DVB_FILTER_MAX               (32)        // support 16 sets filter
#define PARSER_DVB_PID_MAX                  (16)        // support maximum 16 PIDs

#define PARSER_DVB_FILTER_ID_NO             (3)         // support how many DWORD ID

#define PARSER_DVB_TIMEOUT_INFINITE (0)

#define PARSER_INVALID_FILTER_ID    (0xff)
#define PARSER_DVB_DUMMY_PID        (0x1ffe)    // this PID almost won't be used, we use here as dummy PID for initialization

#define RING_BUFFER_SIZE_1K         (1024)
#define RING_BUFFER_SIZE_4K         (4096)
#define RING_BUFFER_SIZE_8K         (8192)

#define DONT_CARE_TABLE_ID          (0xff)
#define DONT_CARE_SUB_TABLE_ID      (0xffff)
#define DONT_CARE_SECTION_NUM       (0xff)

// define filter flag's bit meaning
#define PARSER_DVB_FILTER_NULL              (0x00000000)
#define PARSER_DVB_FILTER_PES_AUDIO         (0x00000001)
#define PARSER_DVB_FILTER_PES_VIDEO         (0x00000002)
#define PARSER_DVB_FILTER_PES_TELETEXT      (0x00000004)
#define PARSER_DVB_FILTER_PES_SUBTITLE      (0x00000008)
#define PARSER_DVB_FILTER_SECTION           (0x00000010)
#define PARSER_DVB_FILTER_EQUAL             (0x00000020)
#define PARSER_DVB_FILTER_NOT_EQUAL         (0x00000040)

// define section filter's status bit
#define PARSER_DVB_SECTION_OK               (0x00000001)
#define PARSER_DVB_SECTION_OVERFLOW         (0x00000002)
#define PARSER_DVB_SECTION_DISCONTINUITY    (0x00000004)
#define PARSER_DVB_SECTION_DONE             (0x00010000)
#define PARSER_DVB_SECTION_SEARCH_START     (0x00020000)
#define PARSER_DVB_SECTION_RUNNING          (0x00040000)
#define PARSER_DVB_SECTION_ENABLE           (0x00080000)

typedef enum    tagPARSER_FILTER_STATUS
{
	PARSER_FILTER_NO_ERROR,             /**< no error */
	PARSER_FILTER_SECTION_AVAILABLE,    /**< there is a section available */
	PARSER_FILTER_USER_ABORT,           /**< user abort process */
    PARSER_FILTER_TIMEOUT,              /**< filter timeout */
    PARSER_FILTER_CRC_ERROR,            /**< section crc check error */
    PARSER_FILTER_ERROR                 /**< some thing wrong */
} PARSER_FILTER_STATUS, *PPARSER_FILTER_STATUS;

typedef enum    tagPARSER_DVBCMD
{
    PARSER_DVBCMD_ALLOC,
    PARSER_DVBCMD_ASSIGN,
    PARSER_DVBCMD_CHANGE_ID,
    PARSER_DVBCMD_FREE,
    PARSER_DVBCMD_SET,
    PARSER_DVBCMD_AVAILABLE,
    PARSER_DVBCMD_ALLAVAILABLE,
    PARSER_DVBCMD_ENABLE,
    PARSER_DVBCMD_DISABLE,
    PARSER_DVBCMD_INIT,
    PARSER_DVBCMD_INITALL,
    PARSER_DVBCMD_BUF_RESET,
    PARSER_DVBCMD_START,
    PARSER_DVBCMD_RESTART,
    PARSER_DVBCMD_STOP
} PARSER_DVBCMD, *PPARSER_DVBCMD;

typedef enum    tagPARSER_DVBREPLAY_POS
{
    PARSER_DVBREPLAY_START,
    PARSER_DVBREPLAY_CUR
} PARSER_DVBREPLAY_POS, *PPARSER_DVBREPLAY_POS;

typedef void (*FPPARSER_SEC_CB)(BYTE bFID, PARSER_FILTER_STATUS FltStatus);

typedef struct  tagPARSER_DVB_SEC_HEAD
{
    union
    {
        DWORD   dwSecHead[3];
        BYTE    bSecHead[12];
        struct
        {
            BYTE    bTable_id: 8;
            BYTE    bSection_syntax_indicator: 1;
            BYTE    bPrivate_indicator: 1;
            BYTE    bReserved: 2;
            WORD    wSection_length: 12;
            WORD    wSubTableID: 16;
            BYTE    bBitReserved2: 2;
            BYTE    bVersionNumber: 5;
            BYTE    bBitCurrenNext: 1;
            BYTE    bSectionNumber: 8;
            BYTE    bLastSectionNumber: 8;
            DWORD   dwExtendIDs: 32;
        } PACK_FORMAT stSecHead;	// Micky2.17c
    } Data;
} PARSER_DVB_SEC_HEAD, *PPARSER_DVB_SEC_HEAD;

typedef struct  tagPARSER_DVB_FILTER_CFG
{
    WORD    wPID;               // indicate associate with which PID
//    WORD    wPIDIdx;            // indicate associate with which PID
    PARSER_DVB_SEC_HEAD SecHeadID;
    PARSER_DVB_SEC_HEAD SecHeadMask;
    DWORD   dwFlag;
    DWORD   dwUpperAddr;
    DWORD   dwLowerAddr;
    FPPARSER_SEC_CB fpSecCB;
} PARSER_DVB_FILTER_CFG, *PPARSER_DVB_FILTER_CFG;

typedef struct tagPARSER_AVI_AUDIO_BUFFER
{
    DWORD AudioBuffer0;
    DWORD AudioBuffer1;
} PARSER_AVI_AUDIO_BUFFER, *PPARSER_AVI_AUDIO_BUFFER;

typedef enum	tagPARSER_AVI_INFO
{
	PARSER_AVI_HEIGHT,
	PARSER_AVI_WIDTH,
	PARSER_AVI_PSUDO_RATE,
	PARSER_AVI_FRAMES,
	PARSER_AVI_CODEC_VERSION
} PARSER_AVI_INFO, *PPARSER_AVI_INFO;

typedef enum    tagPARSER_COMMAND
{
    PARSER_CMD_STOP,
    PARSER_CMD_START,
    // J500KWShih, char-base 16M solution
    PARSER_CMD_PAUSE,
    PARSER_CMD_PRESCAN,
    PARSER_CMD_RESUME,
    PARSER_CMD_PUSH_VIDEO,
    PARSER_CMD_AVI_PTS,
    PARSER_CMD_DISC_KEY,
    PARSER_CMD_IGNORE_EDC,              // J500CSC_110, enable EDC detection or not depneds on parameter's value
    PARSER_CMD_MAX
} PARSER_COMMAND, *PPARSER_COMMAND;

typedef enum    tagPARSER_PLAY_PARM
{
    PARSER_PLAY_NORMAL = 0,
    PARSER_PLAY_REPLAY
} PARSER_PLAY_PARM, *PPARSER_PLAY_PARM;

typedef enum    tagPARSER_PRESCAN
{
    // J500KWShih_120a, for container
    PARSER_PRESCAN_CONTAINER_INFO,
    PARSER_PRESCAN_CONTAINER_INDEX,
    PARSER_PRESCAN_SCR,
    PARSER_PRESCAN_1STSCR,
//    PARSER_PRESCAN_DISC_KEY,
    PARSER_PRESCAN_TITLE_KEY,
    PARSER_PRESCAN_DVB_PAT_PMT
} PARSER_PRESCAN, *PPARSER_PRESCAN;

typedef enum    tagPARSER_CDDA_TYPE
{
    PARSER_CDDA_UNKNOWN = 0,
    PARSER_CDDA_PURE,
    PARSER_CDDA_HDCD,
    PARSER_CDDA_DTS,
    PARSER_CDDA_CDG
} PARSER_CDDA_TYPE, *PPARSER_CDDA_TYPE;

typedef enum    tagPARSER_ESTYPE
{
    PARSER_ES_AUDIO = 0,
    PARSER_ES_VIDEO,
    PARSER_ES_SP1,
    PARSER_ES_TELETEXT = PARSER_ES_SP1,
    PARSER_ES_SP2,
    PARSER_ES_SUBTITLE = PARSER_ES_SP2,
    PARSER_ES_TS_PACKET = PARSER_ES_SP2,
    PARSER_ES_NV,
    PARSER_DVB_PID_AVAILABLE = PARSER_ES_NV,
    PARSER_DVB_FILTER_AVAILABLE = PARSER_DVB_PID_AVAILABLE,
    PARSER_ES_DATA,
    PARSER_ES_NULL,
    PARSER_ES_MAX = PARSER_ES_NULL
} PARSER_ESTYPE, *PPARSER_ESTYPE;

typedef enum    tagPARSER_ESFMT
{
    PARSER_ESFMT_UNKNOWN = 0,
    // define audio ES format
    PARSER_ESFMT_MP12,
    PARSER_ESFMT_MP3,
    PARSER_ESFMT_AC3,
    PARSER_ESFMT_DTS,
    PARSER_ESFMT_PCM,
    PARSER_ESFMT_MLP,
    PARSER_ESFMT_OGG,
    PARSER_ESFMT_WMA_V1,
    PARSER_ESFMT_WMA_V2,
    PARSER_ESFMT_AAC,  // LLY1.20

    PARSER_ESFMT_MS_ADPCM,
    PARSER_ESFMT_DVI_ADPCM,
    PARSER_ESFMT_PCM_MULAW,
    PARSER_ESFMT_PCM_ALAW,
    // define SP ES format
    PARSER_ESFMT_DVD_SP = 100,  // this is DVD SP format subtitle included in container (ex. AVI/ MP4/ ...)
    PARSER_ESFMT_TEXT_SP,       // this is char base subtitle included in container (ex. AVI/ MP4/ ...)
    PARSER_ESFMT_UTF8_SP,       // this is UTF-8 char base subtitle in container (ex. MP4)
    PARSER_ESFMT_TEXT_SP_EXT,   // this is char base subtitle from external file
    // define video ES format
    PARSER_ESFMT_MPEG1 = 150,
    PARSER_ESFMT_MPEG2,
    PARSER_ESFMT_MJPG,
    PARSER_ESFMT_DIVX311    = 311,
    PARSER_ESFMT_DIVX4      = 412,
    PARSER_ESFMT_DIVX5      = 500,
    PARSER_ESFMT_MPEG4      = 801,
    PARSER_ESFMT_XVID       = 802,
    PARSER_ESFMT_3IVX       = 803,
    PARSER_ESFMT_MS_MPEG4   = 804,
} PARSER_ESFMT, *PPARSER_ESFMT;

#if 0
typedef enum    tagPARSER_MPEG4_NUM
{
    PARSER_MPEG4_COMMON     = 0,
    PARSER_MPEG4_3IVX       = 200,
    PARSER_MPEG4_DIVX311    = 311,
    PARSER_MPEG4_DIVX4      = 412,
    PARSER_MPEG4_DIVX5      = 500,
    PARSER_MPEG4_XVID       = 100
} PARSER_MPEG4_NUM, *PPARSER_MPEG4_NUM;
#endif  // 0

typedef enum    tagPARSER_STATE
{
    PARSER_STATE_STOP,
    PARSER_STATE_PAUSE,
    PARSER_STATE_FNP,
    PARSER_STATE_HEADER,
    PARSER_STATE_SUBHEADER,
    PARSER_STATE_DATA,
    PARSER_STATE_SKIP,
    PARSER_STATE_BYPASS,
    PARSER_STATE_DRM
} PARSER_STATE, *PPARSER_STATE;

typedef enum    tagPARSER_STREAM_STATE
{
    PARSER_STREAM_START,
    PARSER_STREAM_PRESCAN
} PARSER_STREAM_STATE, *PPARSER_STREAM_STATE;

// define input data format
#define PARSER_DATAFMT_DVD_2064         (0x00000001)
#define PARSER_DATAFMT_DVD_2048         (0x00000002)
#define PARSER_DATAFMT_CDDA             (0x00000004)
#define PARSER_DATAFMT_CDDA_SUBCODE     (0x00000008)
#define PARSER_DATAFMT_CDROM            (0x00000010)
#define PARSER_DATAFMT_CDROM_M1         (0x00000020)
#define PARSER_DATAFMT_CDROM_M2         (0x00000040)
#define PARSER_DATAFMT_CDROM_M2F1       (0x00000080)
#define PARSER_DATAFMT_CDROM_M2F2       (0x00000100)
#define PARSER_DATAFMT_LINEAR           (0x00000200)
//#define PARSER_DATAFMT_DVD_BLOCK        (0x00000200)
//#define PARSER_DATAFMT_CD_BLOCK         (0x00000400)
#define PARSER_DATAFMT_NOCHANGE         (0x00000000)

// define input bit stream format
#define PARSER_BSFMT_MUXED              (0x00000001)    // *
#define PARSER_BSFMT_AUDIO_ONLY         (0x00000002)    // *
#define PARSER_BSFMT_VIDEO_ONLY         (0x00000004)    // *
#define PARSER_BSFMT_SP1_ONLY           (0x00000008)    // *
#define PARSER_BSFMT_SP2_ONLY           (0x00000010)    // *
#define PARSER_BSFMT_DATA_ONLY          (0x00000020)    // *

#define PARSER_BSFMT_CSS_DISC_KEY       (0x00000040)
#define PARSER_BSFMT_CSS_TITLE_KEY      (0x00000080)
#define PARSER_BSFMT_MKB_DATA           (0x00000100)
#define PARSER_BSFMT_CPPM_ALBUM_ID      (0x00000200)
#define PARSER_BSFMT_CPRM_MEDIA_KEY     (0x00000400)

#define PARSER_BSFMT_BYTE_SWAP          (0x00000800)
#define PARSER_BSFMT_AVI                (0x00001000)
#define PARSER_BSFMT_CDDA               (0x00002000)
#define PARSER_BSFMT_CDDA_SUBCODE       (0x00004000)
#define PARSER_BSFMT_MP4                (0x00008000)

#define PARSER_BSFMT_DVD_VIDEO          (0x00010000)
#define PARSER_BSFMT_DVD_AUDIO          (0x00020000)
#define PARSER_BSFMT_SVCD               (0x00040000)
#define PARSER_BSFMT_CVD                (0x00080000)

#define PARSER_BSFMT_WAV                (0x00100000)

// J500KWShih_233, 
#define PARSER_BSFMT_WMA                (0x00200000)

#define PARSER_BSFMT_DVB                (0x01000000)
#define PARSER_BSFMT_DVB_TS             (0x02000000)

#define PARSER_BSFMT_NOCHANGE           (0x00000000)

// define output filter
#define PARSER_ENABLE_AUDIO             (0x01 << PARSER_ES_AUDIO)
#define PARSER_ENABLE_VIDEO             (0x01 << PARSER_ES_VIDEO)
#define PARSER_ENABLE_SP1               (0x01 << PARSER_ES_SP1)
#define PARSER_ENABLE_DVB_TELETEXT      (0x01 << PARSER_ES_TELETEXT)
#define PARSER_ENABLE_SP2               (0x01 << PARSER_ES_SP2)
#define PARSER_ENABLE_DVB_SUBTITLE      (0x01 << PARSER_ES_SUBTITLE)
#define PARSER_ENABLE_NV                (0x01 << PARSER_ES_NV)
#define PARSER_ENABLE_DATA              (0x01 << PARSER_ES_DATA)

#define PARSER_ENABLE_END_PADDING       (0x00010000)
#define PARSER_ENABLE_NV_PAUSE          (0x00020000)

#define PARSER_ENABLE_COPY_PROTECTION   (0x00040000)

#define PARSER_ENABLE_ALL               (0xffffffff)

// define ES found
#define PARSER_FOUND_AUDIO              (PARSER_ENABLE_AUDIO)
#define PARSER_FOUND_VIDEO              (PARSER_ENABLE_VIDEO)
#define PARSER_FOUND_SP1                (PARSER_ENABLE_SP1)
#define PARSER_FOUND_DVB_TELETEXT       (PARSER_ENABLE_DVB_TELETEXT)
#define PARSER_FOUND_SP2                (PARSER_ENABLE_SP2)
#define PARSER_FOUND_DVB_SUBTITLE       (PARSER_ENABLE_DVB_SUBTITLE)
#define PARSER_FOUND_NV                 (PARSER_ENABLE_NV)
#define PARSER_FOUND_DATA               (PARSER_ENABLE_DATA)
#define PARSER_FOUND_NONE               (0x0000000)

// define event can be detected
#define PARSER_EVENT_AUDIO_PTS          (0x00000001)
#define PARSER_EVENT_VIDEO_PTS          (0x00000002)
#define PARSER_EVENT_SP_PTS             (0x00000004)
#define PARSER_EVENT_NV_PACK            (0x00000008)
#define PARSER_EVENT_TRIGGER_FLAG       (0x00000010)
#define PARSER_EVENT_EOR_FLAG           (0x00000020)
#define PARSER_EVENT_EOF_FLAG           (0x00000040)
#define PARSER_EVENT_DSM_TRICK_INFO     (0x00000080)
#define PARSER_EVENT_MOTION_DETECTED    (0x00000100)
#define PARSER_EVENT_PACK_SCR           (0x00000200)
#define PARSER_EVENT_NEW_ES_FOUND       (0x00000400)

// J500KWShih_121, for different movie container
//#define PARSER_EVENT_AVI_ERROR          (0x10000000)    // when AVI stream has some error, ID icorrect, sector crashed
#define PARSER_EVENT_MOVIE_ERROR		(0x10000000)
#define PARSER_EVENT_ERROR              (0x20000000)
#define PARSER_EVENT_RUNNING            (0x40000000)
#define PARSER_EVENT_COMPLETED          (0x80000000)    // this special event can not be disabled
#define ATAPI_EVENT_START               (0x08000000)    // this special event can not be disabled
#define ATAPI_EVENT_COMPLETED           (0x04000000)    // this special event can not be disabled
// J500KWShih_120a, for MP4 file support
#define PARSER_EVENT_NOT_READY          (0x02000000)    // tell mm that should update source filter

#define FLAG_SET_ALL                    (0xffffffff)

#define PARSER_PADDING_BYTE             (0xff)

#define STREAM_INVALID_ID               (0xffffffff)

// J500KWShih_237, for support MP4 (Nero Digital)
#ifdef PARSER_SUPPORT_MP4
#define PARSER_ES_BUF_LEN               (128)
#else
#define PARSER_ES_BUF_LEN               (64)
#endif // PARSER_SUPPORT_MP4

#define PARSER_ES_BUF_MASK              (PARSER_ES_BUF_LEN-1)

// Modified by J500CSC_20060910, this value indicates a reserved space at ESBuf for inserting extra header information
// and let AV decoder can handle data well.  MUST MAKE SURE if we add or delete any information at DumpES function.
// Here we set the value as more 8B tolerance than the largest reserved space needed.
#define PARSER_EXTRA_HEAD_SIZE          (32)    //(12)            // Extra information bytes needs to be added when each new
                                                        // packet or chunk is found.
                                                        // (1) LPCM for DVD-VIDEO: 0x55805580 + size(16-bit)
                                                        // (2) (DVD or SVCD/CVD) Subtitle/OGT: PTS (4B) + ID (4B)
                                                        // (3) AVI Video --> 12B or 16B
                                                        //     tag 0x000001ff (4B) + PTS (4B) + chunk offset (4B)
                                                        //     + DivX 311: 0x000001B6 (4B)
                                                        // (4) WMA start header for AVI search function --> 24B
                                                        //     0xfedcba9876543210 (8B) + 14B (audio header) + 2B extra header (B:4-5)
                                                        // (5) (DivX) DVD Subtitle --> 16B
                                                        //     Start PTS (4B) + ID (4B) + tag DCSQ (4B) + End PTS (4B)
#define PARSER_EXTRA_MANUAL_BYTES       (24)

#define PARSER_BS_BUF_LEN               (128)           // this value should be 2's power value
#define PARSER_BS_BUF_MASK              (PARSER_BS_BUF_LEN-1)

#define PARSER_MAX_AVI_VIDEO_TRACK      (1)
#define PARSER_MAX_AVI_AUDIO_TRACK      (8)
#define PARSER_MAX_AVI_CHAPTER_TRACK    (16)
#define PARSER_MAX_AVI_SUBTITLE_TRACK   (32)

#define PARSER_MAX_DVD_VIDEO_TRACK      (1)
#define PARSER_MAX_DVD_AUDIO_TRACK      (8)
#define PARSER_MAX_DVD_SUBTITLE_TRACK   (32)

#define PARSER_MAX_MP4_VIDEO_TRACK		(1)
#define PARSER_MAX_MP4_AUDIO_TRACK		(8)
#define PARSER_MAX_MP4_SP_TRACK			(32)

// define error code for DivX DRM
#define DRM_ERROR_NONE                      (0)

#define DRM_ERROR_INVALID_MASTER_KEY        (10)
#define DRM_ERROR_DECRYPTION_FAILED         (11)
#define DRM_ERROR_READING_MEMORY            (12)
#define DRM_ERROR_WRITING_MEMORY            (13)
#define DRM_ERROR_UNRECOGNIZED_DRM_MODE     (14)
#define DRM_ERROR_NOT_AUTH_USER             (15)
#define DRM_ERROR_RENTAL_EXPIRED            (16)
#define DRM_ERROR_NEED_MORE_RANDOM_SAMPLE   (17)
#define DRM_ERROR_NOT_INITIALIZED           (18)
#define DRM_ERROR_NOT_LIMITED_RENTAL_TYPE   (19) 
#define DRM_ERROR_NOT_COMMITTED             (20)
#define DRM_ERROR_NOT_RENTAL_QUERIED        (21)
#define DRM_ERROR_BAD_SLOT_NUMBER           (22)
#define DRM_ERROR_NULL_GUARD_SET_SIGNAL     (23)
#define DRM_ERROR_INVALID_ALIGNMENT         (24)

// define the EDC error code
#define PARSER_EDC_NOERR                (0x0000)    // no error at all
#define PARSER_EDC_ERROR                (0x8000)    // sector is dump-in with error can not recover
#define PARSER_EDC_NULL                 (0x4000)    // sector can not dump-in at all

// J500KWShih_120a, for Nero Digital chapter
#define PARSER_MP4_CHAP_NEXT			(0x01)
#define PARSER_MP4_CHAP_PREV			(0x02)
#define PARSER_MP4_CHAP_END 			(0xFFFFFFFF)

// J500CSC_221, define the audio buffer gap for protecting overwrite issue at AC3
#define PARSER_AUDIO_BUF_GAP            (MCU_CHANNEL_DUMMY_LEN + MCU_CHANNEL_DUMMY_LEN + 2352 + 0x1000)

#ifdef  PARSER_CODE_DEBUG
typedef struct  tagHW_ADDR
{
    volatile PDWORD pdwInStreamCurAddr;
    volatile PDWORD pdwInStreamType;
    volatile PDWORD pdwOutStreamCurAddr;
    volatile PDWORD pdwOutStreamType;
    volatile PDWORD pdwRemainderEnable;
    volatile PDWORD pdwVideoBufRemainder;
    volatile PDWORD pdwAudioBuf0Remainder;
    volatile PDWORD pdwAudioBuf1Remainder;
    volatile PDWORD pdwSP1BufRemainder;
    volatile PDWORD pdwSP2BufRemainder;
    volatile PDWORD pdwBIUCtrl;
    volatile PDWORD pdwBIUBypassCount;
    volatile PDWORD pdwBIURemainder;
//    volatile PDWORD pdw
} HW_ADDR, *PHW_ADDR;
#endif  //PARSER_CODE_DEBUG

// NOTE: The cache needs to be flushed if data is read by CPU directly
typedef struct  tagPARSER_OUTPUT_STREAM_BUFFER
{
    DWORD   dwBaseAddr;         // At initialization stage, this address is the same as dwLowerAddr in general except some special usage
    DWORD   dwUpperAddr;
    DWORD   dwLowerAddr;
    DWORD   dwTotalLen;         // record the total length of output buffer in byte

//    DWORD   dwReadCounter;      // this only valid for CPU read directly from memory, unit is sector (PARSER_ES_DATA)
//    DWORD   dwWriteCounter;     // this only valid for CPU read directly from memory, unit is sector (PARSER_ES_DATA)

    BYTE    bESBuf[PARSER_ES_BUF_LEN];    // a buffer stores the data can not transfer due to 64-bit alignment limintation
    DWORD   dwESBufLen;         // length of store buffer in bytes
    DWORD   dwESBufIdx;         // point to the first buffer store
    DWORD   dwSPULen;           // the lebgth for subpicture unit
} PARSER_OUTPUT_STREAM_BUFFER, *PPARSER_OUTPUT_STREAM_BUFFER;

// Added by J500CSC_20070111
// define a struct to store the data for AVI's MS MPEG-4 header
// We will overlap it with the NV's output stream buffer
#define PARSER_MS_MPEG4_HEAD_BUF_LEN        (32)
typedef struct  tagPARSER_MS_MPEG4_STREAM_HEADER
{
    BOOL    fValid;
    BOOL    fVideoRangeStart;
    DWORD   dwBufLength;
    BYTE    bBuffer[PARSER_MS_MPEG4_HEAD_BUF_LEN];
} PARSER_MS_MPEG4_STREAM_HEADER, *PPARSER_MS_MPEG4_STREAM_HEADER;

typedef struct tagPARSER_STATUS
{
    PARSER_STATE    NextState;  // specify next state when change occurred
    PARSER_STATE    CurState;   // specify the current status
    PARSER_STREAM_STATE CurStreamState; // specify current stream state
    PARSER_PRESCAN   PrescanType;
    BOOL    fFNPDone;           // specify if the FNP state is done
    BOOL    fBlockDone;         // specify if block is processed ok
    BOOL    fOutputBlocked;     // specify if output is blocked
    BOOL    fSkipDummyHead;     // specify if this is dummy sector at head of a block for DVD-2064 or AVI header file
    DWORD   dwCurID;            // specify cirrent ID (it will need 32-bit for AVI format)
    DWORD   dwESFound;          // specify how many elementary stream detected
    PARSER_ESTYPE   CurESType;  // specify current ES type
    DWORD   dwActionFilter;     // specify the output filter for elementary stream
    DWORD   dwByteScan;         // specify how many bytes are scanned

    // J500KWShih_202, byte counter for by-pass/skip mode
    DWORD   dwByteCount;        // specify how many bytes are scanned

    DWORD   dwBypassLen;        // specify the length of bypass or skipping
    DWORD   dwOverBypassLen;    // specify the length of block overflow for bypass
    DWORD   dwBSBufLen;
    DWORD   dwBSBufIdx;
    BYTE    bBSBuf[PARSER_BS_BUF_LEN];      // save the reset data which is not enough to complete the state action

    DWORD   dwBSFormat;         // specify the input data and bit stream format
    DWORD   dwBIUCLIPLEN;       // clipping information for this bit stream
    DWORD   dwBIUSECTORLEN;     // the sector length output from MCU
    DWORD   dwBIUFRMLEN;        // frmae length for this bit stream
    DWORD   dwBIUInputEnable;   // keep the settings for enabling BIU's input feature
                                // If data comes from SERVO, we need to turn on scramble feature
    DWORD   dwMCUBCR0A;
    DWORD   dwMCUBCR0D;
    DWORD   dwMCUBlockLen;      // the length of whole block (MCU viewpoint)
    DWORD   dwBIUBlockLen;
    DWORD   dwSectorLen;        // the length of sector (PARSER viewpoint)
    DWORD   dwSectorsPerBlock;  // the number of sectors per block
    DWORD   dwSearchPatternLen; // list the search pattern length, ex. 3 for MPEG & 4 for AVI
    DWORD   dwChunkSize;        // indicate current Chunk size

    DWORD   dwEDCFlag;          // use one bit to indicate each sector has error or not
                                // This value will be updated per block.
                                // If current PARSER is handling sector n, the bit 
                                // sector (n-32) (n-31) (n-30)   (n-4) (n-3) (n-2) (n-1)
                                // bit     31     30     29   ... 03    02    01    00

    // variables for DVB solution
    DWORD   dwTSLen;
    DWORD   dwCurPIDIdx;

    DWORD   dwNStripeReadAddr;  // J500KWShih_216, decoder read ES buffer address
    BOOL    fNStripe;           // J500KWShih_216, enable N-Stripe
                                // TRUE : enable
                                // FALSE: disable
    BOOL    fDataFmtDetect;     // indicate if the data format has been detected already
    BOOL    fHeadDetected;      // indicate a chunk or mpeg head was found
    BOOL    fEDCDetect;         // J500CSC_110, indicate to detect EDC flag or not
    BOOL    fRangeStartDetect;  // J500CSC_121
    BOOL    fRunAtDSRMode;      // J500CSC for DVB, indicate if run at DVB mode or not?
    BOOL    fCharSubpicture;    // J500KWShih, indicate parser is wait for char subtitle parsing
    FLAG_T  flagParserEvent;    // report events when PARSER is parsing

    DWORD   dwIPStartBiuBlk;    // indicate the start block for Instant-Replay
    DWORD   dwIPEndBiuBlk;      // indicate the end block for Instant-Replay
    DWORD   dwIPFreeBlk;        // indicate how many blocks are not filled for Instant-Replay
    DWORD   dwIPTotalBlk;       // indicate the total number of blocks can be used for Instant-Replay
    BOOL    fIPStartPlaying;    // indicate the instant replay is enabling

    BOOL    fDVBScramble;       // indicate if the DVB stream is scramble or not
    DWORD   dwDVBPktCounter;    // a DVB 188 packet counter, increase one when one 188 packet is found

    PPARSER_MS_MPEG4_STREAM_HEADER  pMSMpeg4Header;     // store the MS MPEG-4 header and need to write to video buffer at each
                                                        // stream start (include GoToTime)
    DWORD   dwReserved[9];     // reserved for future usage
} PARSER_STATUS, * PPARSER_STATUS;

typedef struct  tagPARSER_BITS_CONTENT
{
    BOOL    fValid;
    BOOL    fCopyProtection;
//    BOOL    fVideoIDConfirm;
    BOOL    fUpdatePTSValid;
    DWORD   dwVideoTrackCount;
    DWORD   dwAudioTrackCount;
    DWORD   dwSubtitleTrackCount;
    DWORD   dwWidth;                // width of the AVI file in pixels
    DWORD   dwHeight;               // height of the AVI file in pixels
    DWORD   dwFrameRate;            // video frame rate
    DWORD   dwPsudoRate;			// fram rate * 65536
    DWORD   dwTotalFrames;
    DWORD   dwMicrosecPerFrame;     // the number of microseconds between frames
    DWORD   dwDropHeadSize;         // indicate how many bytes need to be dropped at first in order to get the chunk ID position
    DWORD   dwCurAudioByteCount;    // record a byte counter from start play or audio switching for update audio PTS
    WORD    wCurAudioIdx;
    WORD    wCurVideoIdx;

    // J500KWShih, for MP4
    DWORD   dwChapterCount;
    DWORD   dwStartPTS;

    union
    {
        struct
        {
            DWORD   dwSample2Chunk;
            struct
            {
                PARSER_ESFMT    ESFormat;
                BYTE            bChannelNo;
                BYTE            bBitsPerSample; // J500CSC_20070130, report the number of bits per sample for host reference
                DWORD           dwID;
                DWORD           dwSampleRate;
                DWORD           dwSamples;      // save the audio sample counter for AVI bit stream
            } AudioTrack[PARSER_MAX_AVI_AUDIO_TRACK];
            struct
            {
                PARSER_ESFMT    ESFormat;
                DWORD           dwID;
                DWORD           dwID_db;
                // J500KWShih_235, using stream rate to calculate video PTS
#ifdef PARSER_AVI_VIDEO_PTS_STREAM
                DWORD           dwFrameCount;
#else
                ULONGLONG       ullVideoMSCount;    // save the frame's micro second for AVI bit stream
#endif // PARSER_AVI_VIDEO_PTS_STREAM
            } VideoTrack[PARSER_MAX_AVI_VIDEO_TRACK];
            struct
            {
                PARSER_ESFMT    ESFormat;
                DWORD           dwID;
            } SubtitleTrack[PARSER_MAX_AVI_SUBTITLE_TRACK];
        } AVI;

        struct
        {
            struct
            {
                PARSER_ESFMT    ESFormat;
                DWORD           dwID;
            } AudioTrack[PARSER_MAX_DVD_AUDIO_TRACK];
            DWORD   dwVideoTrackID[PARSER_MAX_DVD_VIDEO_TRACK];
            DWORD   dwSubtitleTrackID[PARSER_MAX_DVD_SUBTITLE_TRACK];
        } MPEG;
    } StreamID;

    DWORD   dwReserved[16];     // reserved for future usage
} PARSER_BITS_CONTENT, *PPARSER_BITS_CONTENT;

// define ID structure for DVB system
// Each ID contains 3x4 bytes and has bit mask operation, EQUAL or NOT EQUAL,
typedef struct  tagPARSER_DVB_FILTER
{
    WORD    wPIDIdx;            // indicate associate with which PID
    WORD    wSecLen;            // indicate how much data is not processed
    DWORD   dwID[PARSER_DVB_FILTER_ID_NO];
    DWORD   dwIDMask[PARSER_DVB_FILTER_ID_NO];
    DWORD   dwFlag;             // indicate the filter is valid or empty

    DWORD   dwWriteAddr;        // At initialization stage, this address is the same as dwLowerAddr in general except some special usage
                                // It will be increase only when one complete field is output
    DWORD   dwCurWriteAddr;     // current writting address
    DWORD   dwUpperAddr;
    DWORD   dwLowerAddr;
//    DWORD   dwTotalLen;         // record the total length of output buffer in byte
    DWORD   dwReadAddr;         // this only valid for CPU read directly from memory
    DWORD   dwStatusFlag;       // indicate if any error occurred
    DWORD   dwPESLen;           // keep how many PES length are there

    FPPARSER_SEC_CB fpSecCB;
    DWORD   dwTimeoutMilliS;    // millisecond to time out
    DWORD   dwTimerEnd;
    DWORD   dwSecReadCounter;
    DWORD   dwSecWriteCounter;

    DWORD   dwDebug;
} PARSER_DVB_FILTER, *PPARSER_DVB_FILTER;

#define PARSER_DVB_PESLEN_INFINITE      (0xffffffff)    // means there is no specified how many bytes is a PES packet
#define PARSER_DVB_END_SECTOR           (0x00ffffff)
#define PARSER_DVB_LOOPBACK_SECTOR      (PARSER_DVB_END_SECTOR >> 4)

typedef struct  tagPARSER_DVB_PID_STATE
{
    WORD    wPID;
    BYTE    bTSContinuity;
    BOOL    fAlloc;
    BOOL    fTSDiscontinuity;
    BYTE    bCount;
    WORD    wBufRemLen;             // save how many bytes remaining at buffer (< 4)
    DWORD   dwSecRemLen;            // indicate how many section length are remained
    ULONG_UNION ulunBuf;            // save remaining bytes (in DWORD alignment)
} PARSER_DVB_PID_STATE, *PPARSER_DVB_PID_STATE;

typedef struct tagPARSER_GBL
{
    PARSER_STATUS               Status[MAX_STREAM_COUNT];
    PARSER_BITS_CONTENT         BitsContent;
    DWORD   dwCurInStreamIdx;   // store current stream index working on
    //================================= Don't change above order !!!!!!!!!!!!!!
    PARSER_OUTPUT_STREAM_BUFFER ESBuffer[PARSER_ES_MAX];    // declare infortmation storage for each output stream
#ifdef  PARSER_CODE_DEBUG
    HW_ADDR     HWAddr;
#endif  //PARSER_CODE_DEBUG
#ifdef  PARSER_SUPPORT_DVB
    PARSER_DVB_FILTER           DVBFilter[PARSER_DVB_FILTER_MAX];
    PARSER_DVB_PID_STATE        DVBPIDState[PARSER_DVB_PID_MAX];
    cyg_flag_t  flagDVBFilterEvent;     // report events when PARSER is parsing
#endif  // PARSER_SUPPORT_DVB
} PARSER_GBL, *PPARSER_GBL;

#define PARSER_VIDEO_PTS_GAP        (0)
#define PARSER_AUDIO_COUNT_NULL     (0xFFFFFFFF)
#define PARSER_FAIL_SECTOR          (0xFFFFFFFF)

typedef struct  tagPARSER_AV_PTS
{
    DWORD   dwVideoPTS;
    DWORD   dwAudioPTS;
} PARSER_AV_PTS, *PPARSER_AV_PTS;

// J500KWShih_220, AVI audio timestamp formula
typedef enum tagPARSER_AVI_TIMESTAMP_FORMULA
{
    PARSER_AVI_TIMESTAMP_FORMULA_STREAM,    // PC
    PARSER_AVI_TIMESTAMP_FORMULA_BYTE       // MTK
} PARSER_AVI_TIMESTAMP_FORMULA, *PPARSER_AVI_TIMESTAMP_FORMULA;

//====================================
// declare global variables
//====================================
extern PARSER_GBL  __PARSER_gbl;       // declare the export global data structure

//====================================
// declare global functions
//====================================
extern void    PARSER_ThreadMain( DWORD index );
extern void    PARSER_ThreadExit( WORD data );
extern void    PARSER_ThreadInit( void );

extern void    PARSER_DRM_Config( DWORD dwEEPROMBaseAddr );
extern BOOL    PARSER_DRM_PowerOnInit( void );
extern void    PARSER_DRM_SetModelID( BYTE bLow, BYTE bTop );
extern BOOL    PARSER_DRM_GetRegistrationCodeString( PBYTE pbRegistrationCodeString );
extern DWORD   PARSER_DRM_Init( PBYTE pbUseLimit, PBYTE pbUseCount );
extern void    PARSER_DRM_EarlyInit( void );
extern DWORD   PARSER_DRM_CommitPlayback( void );
extern DWORD   PARSER_GetAVIInfo( PARSER_AVI_INFO AVIInfoType );
extern BOOL    PARSER_Command( DWORD dwStreamIdx, PARSER_COMMAND Command, void * pParamRet );

extern BOOL    PARSER_BufferReset( PARSER_ESTYPE ESType );
extern BOOL    PARSER_ActionFilter( DWORD dwStreamIdx, DWORD dwESFilter, BOOL fEnable );
extern BOOL    PARSER_ChangeID( DWORD dwStreamIdx, PARSER_ESTYPE ESType, DWORD dwID );
extern BOOL    PARSER_SetDataFmt( DWORD dwStreamIdx, DWORD dwDataFmt, DWORD dwBSFmt );
extern DWORD   PARSER_GetBufferRemainder( PARSER_ESTYPE ESType );
extern BOOL    PARSER_SetOutBufferAddress( PARSER_ESTYPE ESType, DWORD dwBufLowerAdr, DWORD dwBufUpperAdr );
extern BOOL    PARSER_SetBSBufferAddress( DWORD dwStreamIdx, DWORD dwBufLowerAdr, DWORD dwBufUpperAdr );
extern DWORD   PARSER_QueryEvent( DWORD dwStreamIdx, DWORD dwEvents, BOOL fClear );
extern DWORD   PARSER_GetCurSector( DWORD dwStreamIdx );

extern void    PARSER_WriteTitleKey( ULONGLONG_UNION ullunTitleKey );
extern void    PARSER_ReadTitleKey( PULONGLONG_UNION pullunTitleKey );
extern DWORD   PARSER_ReadCPRMAI( void );

// J500KWShih_120a, for container
extern PPARSER_BITS_CONTENT    PARSER_GetContainerContent( void );
extern DWORD   PARSER_GetContainerIndexPosition(DWORD dwFileStartSector);
extern DWORD   PARSER_GetContainerTimePosition( DWORD dwFileStartSector, DWORD dwTimeInSec, DWORD dwIndexInc, PPARSER_AV_PTS pAVPTS );
extern DWORD   PARSER_GetMovieEndPosition(DWORD dwFileStartSector);
extern DWORD   PARSER_GetContainerCurVideoTime( void );
extern void    PARSER_InitialContainerPrescan(DWORD dwIdxTblTempBuffer, DWORD dwIdxTblTempBufferLen, DWORD dwIdxTblBuffer, DWORD dwIdxTblBufferLen);

// J500KWShih_214, get DVD CGMS
extern DWORD   PARSER_GetDVDCGMS(DWORD dwStreamIdx);

// J500KWShih_216, enable N-Stripe
//***************************************************************************
//	Function	:	PARSER_EnableNStripe
//	Abstract	:	enable N Stripe
//
//	Arguments	:	dwStreamIdx
//                  flag        : TRUE , enable
//                                FALSE, disable
//                  dwReadAddr  : decoder read ES buffer address
//					
//	Return		:	TRUE , success
//                  FALSE, unreasonable read address
//	Side Effect	:
//	Notes		:   If disable N-stripe (flag = FALSE), the value of dwReadAddr will
//                  be ignored and will be set to 0.
//***************************************************************************
extern BOOL PARSER_EnableNStripe(DWORD dwStreamIdx, BOOL flag, DWORD dwReadAddr);

// J500KWShih_216, we will not extern __PARSER_gbl for information hiding
// UTL_Read_PreScan_Info in utl.c and _MM_MotionTrigger in mmanager.c
// should used this function to get copy protection infomation
extern BOOL PARSER_GetCopyProtection(void);

extern BOOL    PARSER_GetSCR( PDWORD pdwSCR );
extern DWORD   PARSER_GetEDCFlag( DWORD dwStreamIdx );
extern BOOL    PARSER_AddExtTextSP( DWORD dwStreamIdx );            // J500CSC_120

extern PARSER_ESFMT    PARSER_QueryContainerAudioType( DWORD dwStreamIdx, DWORD dwAudioIdx );   // J500CSC_121

#ifdef PARSER_SUPPORT_MP4
// J500KWShih_120a, for MP4 file support
extern DWORD   PARSER_GetMP4MoovBoxPosition(void);
extern WORD    PARSER_GetMP4Chapter(DWORD dwTimeInSec);
extern DWORD   PARSER_GetMP4ChapterTime(BYTE bDirection);
#endif PARSER_SUPPORT_MP4

// J500KWShih_220, set AVI audio timestamp formula
extern void PARSER_setAVITimestampFormula(PARSER_AVI_TIMESTAMP_FORMULA timestamp);

// J500KWShih_230, add index entry by decoder while AVI w/o index table
extern void PARSER_addAVIIndexEntry(DWORD dwPTS, DWORD dwOffset);

// J500KWShih_230, query AVI index table
extern BOOL PARSER_QueryAVIIndexTable(void);

// J500KWShih_230, get AVI video buffer ratio
extern BYTE PARSER_getAVIVideoRatio(void);

// J500KWShih_232, get AVI audio track index
extern BYTE PARSER_getAVIAudioMaxBitrateTrackIndex(void);

// J500KWShih_232, get AVI audio buffer size
extern DWORD PARSER_getAVIAudioBufferSize(BYTE index);

// J500KWShih_233, get WMA time position
extern DWORD PARSER_getWMATimePosition(DWORD dwFileStartSector, DWORD dwHeaderSize, DWORD dwPacketSize, DWORD dwBitRate, DWORD dwTimeInSec);

extern void    PARSER_Debug( DWORD dwStreamIdx );

#ifdef  PARSER_TEST
extern void    ATAPI_Thread( DWORD index );
extern BYTE    ATAPI_ReadSectors( DWORD dwStartPos, DWORD dwEndPos );
#endif  // PARSER_TEST

#ifdef PARSER_CHECK_FRAME_ID
extern void PARSER_CheckFrame(BOOL fType);
#endif // PARSER_CHECK_FRAME_ID

extern BOOL PARSER_CheckESFound(DWORD ESType);

extern DWORD   PARSER_CtlDVBFilter( PARSER_DVBCMD PIDCmd, DWORD dwFilterIdx, DWORD dwParam );
extern DWORD   PARSER_DVB_FilterOpen( DWORD dwBufSize, WORD wPID );
extern void    PARSER_DVB_FilterClose( DWORD dwFilterIdx );
extern PARSER_FILTER_STATUS    PARSER_DVB_SectionMonitor( WORD wPID, BYTE bTableID,
        WORD wSubTableID, BYTE bSectionNum, FPPARSER_SEC_CB sec_cb, WORD wTimeoutMilliSec,
        PBYTE pbFilterIdx );
extern PARSER_FILTER_STATUS    PARSER_DVB_SectionGet( WORD wPID, BYTE bTableID,
        WORD wSubTableID, BYTE bSectionNum, PBYTE pbSection, WORD wMaxSecSize,
        WORD wTimeoutMilliSec );
extern void    PARSER_DVB_FilterPoll( void );
extern BOOL    PARSER_DVB_FilterCheckTimeOut( DWORD dwFilterIdx );
extern BOOL    PARSER_DVB_FilterSectionReady( DWORD dwFilterIdx );
extern BOOL    PARSER_DVB_FilterGetSection( DWORD dwFilterIdx, PBYTE pbSection, WORD wMaxLength, BOOL fTSPkt );
extern DWORD   PARSER_DVBCRC32( PBYTE pbSectionField );
extern BOOL    PARSER_DVBInitPath( DWORD dwStreamIdx, DWORD dwNimCfgFlag, BOOL f909SEval );
extern void    PARSER_DVBClosePath( DWORD dwStreamIdx );
extern void    PARSER_DVBResetPath( void );
extern void    PARSER_DVBRestartPath( DWORD dwStreamIdx, DWORD dwNimCfgFlag );
extern void    PARSER_DVB_ForceStop( void );
extern BOOL    PARSER_DVBReplayReset( DWORD dwStreamIdx, PARSER_DVBREPLAY_POS ReplayPos );
extern DWORD   PARSER_DVB_IstantReplayRatio( DWORD dwStreamIdx );
extern DWORD   PARSER_DVB_IstantReplayFillRatio( DWORD dwStreamIdx );

extern DWORD   PARSER_DVB_TSFilterOpen( DWORD dwBufAddr, DWORD dwBufSize, WORD wPID );
extern void    PARSER_DVB_TSFilterClose( DWORD dwFilterIdx );
extern DWORD   PARSER_DVB_GetPktCounter( DWORD dwStreamIdx );
extern BOOL    PARSER_DVB_BSScramble( DWORD dwStreamIdx );

extern void    Upper4Chars( PULONG_UNION pulunData );
extern DWORD   ATOI(PCHAR pstr);

extern BOOL    PARSER_QueryNoneInputBufferReady( DWORD dwStreamIdx );
extern BOOL    PARSER_QueryESBufferReady( DWORD dwStreamIdx, DWORD dwSize );

// J500CSC_20070514, Support host to get current MPEG SCR
extern BOOL    PARSER_MPEG_GetCurrentSCR( PULONGLONG_UNION pullunSCR );

#ifdef __cplusplus
}
#endif //__cplusplus

#endif  // __PARSER_H__
