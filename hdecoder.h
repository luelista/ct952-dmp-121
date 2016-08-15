#ifndef __HDECODER_H__
#define __HDECODER_H__

#ifdef __cplusplus
   extern "C"{
#endif

// LLY2.35, define a structure for A/V bits buffer address
typedef struct  tagBITS_BUF_ADDR
{
    DWORD   dwA0Start;
    DWORD   dwA0End;
    DWORD   dwA1Start;
    DWORD   dwA1End;
    DWORD   dwVStart;
    DWORD   dwVEnd;
} BITS_BUF_ADDR, *PBITS_BUF_ADDR;


void HAL_DetectChipVersion(void); //kevin0.80a
// LLY2.18, remove chip version and part number related define from "hal.h" to here
// Since, it only used for "hdecoder.c"
// Define the value for chip version (__bChipVersion)
// Notice: define the ID value is same as spec. ie, can't modify the value by yourself.
// LLY2.51, porting Chuan's code to add chip version for CT909P
// LLY2.61, porting Chuan's code to add chip version for CT909P/G new IC
#define     VERSION_AA          0xA
#define     VERSION_BA          0xB000
#define     VERSION_BB          0xB001
#define     VERSION_P_AA        0x3000
#define     VERSION_P_AB        0x3001
#define     VERSION_G_AA        0xD000
#define     VERSION_G_AB        0xD001


// LLY2.75, expand as DWORD unit since we will support more feature
//extern  BYTE    __bSupportFeature;
extern  DWORD   __dwSupportFeature;
#define SUPPORT_FEATURE_DTS             0x00000001L
#define SUPPORT_FEATURE_PROLOGIC        0x00000002L
#define SUPPORT_FEATURE_WMA             0x00000004L
#define SUPPORT_FEATURE_HDCD            0x00000008L
#define SUPPORT_FEATURE_DIVX            0x00000010L
#define SUPPORT_FEATURE_NERODIGITAL     0x00000020L
#define SUPPORT_FEATURE_DVB             0x00000040L    // LLY2.18
#define SUPPORT_FEATURE_SPECIAL         0x00000080L    // LLY2.18
#define SUPPORT_FEATURE_NOMPEG4         0x00000100L
#define SUPPORT_FEATURE_NO51CH          0x00000200L
#define SUPPORT_FEATURE_MULTIMEDIA      0x00000400L
#define SUPPORT_FEATURE_DIGITAL_PANEL   0x00000800L
#define SUPPORT_FEATURE_NAND_FLASH      0x00001000L

#define SUPPORT_FEATURE_ALL         0xFF

// LLY2.55, add a global variable to keep current using PROC2
extern  BYTE    __bCurProc2Code;


void    HAL_ResetVideoDecoder(BYTE bDecoders);
// LLY2.51, give desired video buffer mode since we may in different mode.
//void    HAL_FillVideoBuffer(PDWORD  pdwPtr, DWORD dwSize);
void    HAL_FillVideoBuffer(BYTE bMode, PDWORD  pdwPtr, DWORD dwSize);
void    HAL_ClearScreen(BYTE bColor, BYTE bMode, DWORD dwFrameIdx);
DWORD   HAL_LoadAudioCode( BYTE bAudioType, PDWORD pdwAudioTbl, PBYTE pbDstAddr, PBYTE dwWorkMem );
void    HAL_ReloadAudioDecoder(BYTE bAudioType);
void    HAL_InitVideo(void);
void    HAL_InitAudio(void);
void    HAL_Cal_BitsAddr(PBITS_BUF_ADDR pBitsBuf_Addr); // LLY2.35
void    HAL_SetAudio_Buffer(BYTE bMediaAttr, BYTE bMode);
void    HAL_SetVideo_Buffer(BYTE bMode);
void    HAL_ResetAudioDecoder(void);
void    HAL_ResetAIUPCM(void);
//# [8] Clear frame buffer
//clear screen mode
#define HAL_CLEAR_SCREEN_MPEG   0
#define HAL_CLEAR_SCREEN_JPEG   1

#define HAL_COLOR_BORDER    0
#define HAL_COLOR_BLACK     1
#define HAL_COLOR_BLUE      2
#define HAL_COLOR_RED       3
#define HAL_COLOR_GREEN     4
#define HAL_COLOR_YELLOW    5
#define HAL_COLOR_PINK      6
#define HAL_COLOR_GRAY      7
#define HAL_COLOR_PURPLE    8
#define HAL_COLOR_DIGEST    10
#define HAL_COLOR_THUMB     11
#define HAL_COLOR_USER_DEFINE   12 //CoCo1.05

#define HAL_DISP_FRAME0         0
#define HAL_DISP_FRAME1         1
#define HAL_DISP_FRAME2         2
#define HAL_DISP_CURRENTFRAME   5

#define HAL_VIDEO_DECODER1  1
#define HAL_VIDEO_DECODER2  2
extern  DWORD   __dwClearBuffMode;
//# [23] R/W Shared Memory of Proc1 & Proc2
BYTE    HAL_ReadAM(BYTE bCmdId, DWORD* pdwVal);  //kevin1.00
void    HAL_WriteAM(BYTE bCmdId, DWORD dwVal);


#define     HAL_AM_BIT_RATE                 0x1
#define     HAL_AM_SAMPLE_RATE              0x2
// LLY2.77, porting CoCo's code since audio don't use "HAL_AM_WMA_DRM" now
// And, replace this position w/ new meaning.
//#define     HAL_AM_WMA_DRM                  0x3
#define		HAL_AM_WMA_L1_DECODE			0x3 
#define     HAL_AM_MP3_LSF                  0x4
#define     HAL_AM_LPCM_DEEMPHASIS          0x5
#define     HAL_AM_FRAME_SAMPLE_SEC_CNT     0x6
#define     HAL_AM_WMA_BLOCK_ALIGN          0x7
#define     HAL_AM_SAMPLES_PER_FRAME        0x8 //MP3/MP2
#define     HAL_AM_DOWNSAMPLE_EN            0x9
#define     HAL_AM_UPSAMPLE_EN              0xa //for MP3-LSF, MP2 LSF, DTS
#define     HAL_AM_AC3_DOWNMIX_MODE         0xb
#define     HAL_AM_2CH_TO_6CH               0xc //Duplicate
#define     HAL_AM_CHANNEL_MODE             0xd
#define     HAL_AM_LFE_OUTPUT               0xe
#define     HAL_AM_AC3_KOK_MODE             0xf
#define     HAL_AM_DUAL_MODE                0x10
#define     HAL_AM_AUDIO_TYPE               0x11
#define     HAL_AM_SPDIF_MODE               0x12
#define     HAL_AM_SPDIF_OUTPUT             0x13
#define     HAL_AM_KEYSHIFT_EN              0x14
#define     HAL_AM_SURR_EN                  0x15
#define     HAL_AM_KEYSHIFT                 0x16
#define     HAL_AM_PCM_MODE                 0x17
#define     HAL_AM_DTS_MODE                 0x18
#define     HAL_AM_START_DECODE             0x19 //for MP3/WMA
#define     HAL_AM_PROLOGIC_ENABLE          0x1a
#define     HAL_AM_PROLOGIC_CFG             0x1b
#define     HAL_AM_PROLOGIC_CTRL            0x1c
#define     HAL_AM_FF_SPEED                 0x1d
#define     HAL_AM_SKIP_THRESHOLD           0x1e
#define     HAL_AM_PCMBUF0_ADR              0x23
#define     HAL_AM_PCMBUF1_ADR              0x24
#define     HAL_AM_PCMBUF1_LEN              0x25
#define     HAL_AM_PCMBUF2_ADR              0x26
#define     HAL_AM_PCMBUF2_LEN              0x27
#define     HAL_AM_SKIP_ALL_FRAMES          0x28
#define     HAL_AM_PCM_MUTE                 0x29
#define     HAL_AM_ENCODE_SPDIF_RAW         0x2a
#define     HAL_AM_DECODE_DTS               0x2b
#define     HAL_AM_SURR_0                   0x2c
#define     HAL_AM_SURR_1                   0x2d
#define     HAL_AM_RESET_AUDIO              0x2e
#define     HAL_AM_LOAD_CODE                0x2f
#define     HAL_AM_WRITE_COMRAM             0x30
#define     HAL_AM_AUDIO_START              0x31
#define     HAL_AM_SKIP_FRAME_NO            0x32
#define     HAL_AM_ECHO_CR                  0x33
#define     HAL_AM_MIC_DETECT_THRESHOLD     0x34
#define     HAL_AM_PCM_SCALE                0x35 //target PCM scale
#define     HAL_AM_MIC_PCM_SCALE            0x36
#define     HAL_AM_PLAY_COMMAND             0x37
#define     HAL_AM_BASS_MANAGE_CONFIG       0x38
// Joey2.60: add new interface for controlling of SPDIF PCM with/without volume control.
#define     HAL_AM_DO_SPDIF_PCM_SCALE       0x39 // 0: PCM_SCACLE effect SPDIF PCM,  1: PCM_SCALE doesn't effect it
#define     HAL_AM_VIDEO_MASTER             0x3a
#define     HAL_AM_VIDEO_UPDATED_STC        0x3b
#define     HAL_AM_RESET_PTS                0x3c
#define     HAL_AM_MP2_BITSTREAM            0x3d
#define     HAL_AM_MP3MP2_CH_NO             0x3e
#define     HAL_AM_EQ_LOW                   0x3f //4bits/band MSB: high freq; LSB: low freq
#define     HAL_AM_EQ_HIGH                  0x40 //4bits/band MSB: high freq; LSB: low freq
#define     HAL_AM_EQ_AMPLITUDE_LEFT_SHIFT  0x41
#define     HAL_AM_PCMBUF_ADR               0x42
#define     HAL_AM_PCMBUF_LEN               0x43
#define     HAL_AM_DRC_MODE                 0x44 //Dynamic Range Control mode
#define     HAL_AM_DRC_LEVEL                0x45 //Dynamic Range Control level
#define     HAL_AM_DOLBY_SURR_FLAG          0x46 //Dolby Surround Flag
#define     HAL_AM_CENTER_DELAY             0x47 //Dolby Center Delay
#define     HAL_AM_SURROUND_DELAY           0x48 //Dolby Surround Delay
#define     HAL_AM_ACMOD                    0x49 //ACMOD
#define     HAL_AM_HDCD_CTRL                0x51
#define     HAL_AM_PROC2_WAIT_STC           0x52 //for KEY_AST
#define     HAL_AM_TOTAL_TIME               0x53
#define     HAL_AM_PROC2_VERSION            0x54
#define     HAL_AM_PROC2_SUPPORT            0x55
#define     HAL_AM_SPECIAL_KEYUP_MODE       0x56 //for language study
#define     HAL_AM_TEST_CENTERSURR_DELAY    0x57
#define     HAL_AM_PCM_CH_NUM               0x58
#define     HAL_AM_PCM_BTYE_PER_SAMPLE      0x59
#define     HAL_AM_REAL_PCM_SCALE           0x60 //kevin1.11, real PCM scale
#define     HAL_AM_DBGBUF_ADR               0x61
#define     HAL_AM_DBGBUF_IDX               0x62
#define     HAL_AM_DBG_SHOWINFO             0x63
#define     HAL_AM_LSBUF_ADR                0x64 // J500KWShih_218, buffer address for language study
#define     HAL_AM_MPEG_STB_RADIO           0x65 // LLY2.21, add new command for STB radio mode
#define     HAL_AM_WMA_HEADER_SIZE          0x66 // J500CSC2.25, add new command for getting WMA header length
#define     HAL_AM_WMA_PACKET_SIZE          0x67 // J500CSC2.25, add new command for getting WMA packet length
#define     HAL_AM_WMA_MPEG_ERROR_CONCEALMENT_MASK  0x68 // J500KWShih_230, mpeg error concealment
// LLY2.38, add new interface to info DSP not update STC
#define     HAL_AM_AUDIO_NOT_UPDATE_STC     0x69
#define     HAL_AM_EQ_MODE                  0x6a // Damon2.76, for audio EQ
// Joey2.60: add new interface for volume control of selectable channels
#define     HAL_AM_PCM_SCALE_ACTIVE_CHANNEL	0x6c // [0:7]: PCM scale write channel: [0]:L [1]:R [2]:Ls [3]:Rs [4]:C [5]:LFE [6]:Lmix [7]:Rmix.
// LLY2.61, add new AM interface to tell audio decoder current CT909G IC type (5.1ch or 2ch)
#define 	HAL_AM_CT909G_2CH				0x6d
// LLY2.75, porting Joey's code, add AC3 downmix scale control
#define     HAL_AM_AC3_DOWN_MIX_SCALE       0x6f // 4-bit. AC3 down-mix scale control. 2.2 format, Only effect while channel number >2. out=in*((AC3_DOWN_MIX_SCALE/0x4)+1)
// Grievous2.77, provide A/V sync control for audio STC is great than STC 2 second will clean audio buffer for *.avi file.
#define     HAL_AM_AV_SYNC_2SEC_CONDITION   0x71 // 0:disable skip; 1:enable skip


BYTE    HAL_ReadInfo(BYTE bInfo, DWORD *pdwData);
#define     HAL_INFO_FRAME_ID                   0
#define     HAL_INFO_FRAME_ERROR                1
#define     HAL_INFO_MACRO_BLOCK                2
#define     HAL_INFO_VIDEO_REMAIN               5
#define     HAL_INFO_AUDIO_REMAIN               6
#define     HAL_INFO_AUDIO1_REMAIN              7
#define     HAL_INFO_AUDIO2_REMAIN              8
#define     HAL_INFO_HSIZE                      9
#define     HAL_INFO_VSIZE                      10
#define     HAL_INFO_STC                        11
#define     HAL_INFO_AUDIOFREQ                  12
//#define     HAL_INFO_SCR            13
#define     HAL_INFO_SEQ_END_CODE               14
#define     HAL_INFO_STILL                      HAL_INFO_SEQ_END_CODE
#define     HAL_INFO_HLI_STATUS                 15  // Chuan0924
#define     HAL_INFO_DECCMD_ACK                 16  // Chuan0924
#define     HAL_INFO_DECFRAME                   17
#define     HAL_INFO_DISPFRAME_INDEX            18
#ifndef NO_PIP
#define     HAL_INFO_PIPFRAME                   19
#endif  // #ifndef NO_PIP
#define     HAL_INFO_CAN_RUNTIME_RESET_PROC2    20
// wyc1.11a, ad one new case
#define     HAL_INFO_DISP_MODE_SET              21
// Chuan1.21, add one new case to query STC updated or not
#define     HAL_INFO_STC_UPDATED                22
#define     HAL_INFO_ASPECT_RATIO               23
// LLY2.21a, add to read back audio channel number
#define     HAL_INFO_CHANNEL_NO                 24
// LLY2.22, add to read back FBR mode information.
#define     HAL_INFO_FBR_MODE                   25
// LLY2.22, add to read back DISP frame Y & UV Address
#define     HAL_INFO_DISPFRAME_YADDR            26
#define     HAL_INFO_DISPFRAME_UVADDR           27
// LLY2.30, add to read back LPCM down sample or not flag
#define     HAL_INFO_LPCM_DOWNSAMPLE            28
// LLY2.36, add to read back New STC value is valid or not
#define     HAL_INFO_NEW_STC_VALID              29
// LLY2.51, add to read back audio bitrate information.
#define     HAL_INFO_BITRATE                    30




// LLY1.21a, Define the time-out value for PROC2 reset ACK
#define PROC2_RESET_ACK_TIME_OUT    COUNT_1_SEC
#define VALUE_8K                8000
#define VALUE_16K               16000
#define VALUE_32K               32000
#define VALUE_64K               64000
#define VALUE_128K              128000
#define VALUE_11K               11025
#define VALUE_22K               22050
#define VALUE_44K               44100
#define VALUE_88K               88200
#define VALUE_176K              176400
#define VALUE_12K               12000
#define VALUE_24K               24000
#define VALUE_48K               48000
#define VALUE_96K               96000
#define VALUE_192K              192000
void    HAL_SetAudioFrequency(DWORD dwFreqValue);
void    HAL_SetAudioChannel(BYTE bChNo);

#ifdef __cplusplus
   }
#endif

#endif  // __HDECODER_H__

