#ifndef __HAL_H__
#define __HAL_H__

#ifdef __cplusplus
   extern "C"{
#endif

#include "haljpeg.h"

#ifdef SUPPORT_TFT
#include "TFT.h"
#endif

///// Global Vars /////
//extern BYTE     __bAVBufferMode;
extern WORD     __wHALAudioBitrate;
extern WORD     __wHALSamplesPerFrame;
extern DWORD    __dwHALAudioFreq;
extern BYTE     __bCDDA_DTS;
extern BYTE     __bMPEGVideoMode;
extern BYTE     __bTVType;
//extern WORD     __wPicVsize; // keep picture vertical size
//extern WORD     __wPicHsize; // keep picture horizontal size
//extern WORD     __wPicHStart;
//extern WORD     __wPicHEnd;
//extern WORD     __wHCW, __wVCW;  // keep picture hor. & ver. size while clip command
extern SHORT     __wXStart,__wYStart;
extern WORD     __wCDROMReadingOffset;
//extern DWORD    __dwHALABUFFSIZE, __dwHALVBUFFSIZE;
//extern DWORD    __dwHALVDRR;
extern DWORD    __dwHALABUFFSIZE; //, __dwHALVBUFFSIZE;
//extern WORD     __wHALPCRLow;
//extern WORD     __wHALPCRHigh;
extern WORD     __wHALVCRLow, __wHALVCRHigh;
//extern WORD     __wHALCAR;
//extern WORD     __wHALHDSR, __wHALHDWR;
//extern WORD     __wHALSPVDS, __wHALSPHDS;
//extern WORD     __wHALPARCTLRLow, __wHALPARCTLRHigh;
//extern WORD     __wHALPARFIFOCTLRLow, __wHALPARFIFOCTLRHigh;
extern WORD     __wHALAVIDRLow, __wHALAVIDRHigh;
//extern WORD     __wHALNVSUBIDRLow, __wHALNVSUBIDRHigh;
//extern WORD     __wHALBSFORMATR;
//extern BYTE     __bHALSpOgtType;

extern  BYTE   __bZoomPos;  // LLY2.20, keep last zoom position status


// LLY2.04, only define _REG[26] array while enable "#define SUPPORT_TFT"
// Since, TFT related code will shared this array, and the code is same between 908 and 909
#ifdef  SUPPORT_TFT
extern  BYTE   _REG[26];
#endif  // #ifdef SUPPORT_TFT

extern BYTE    __bTVE_JPEG; //kevin1.00
extern DWORD _dwHALUserDefinedColor; //CoCo.CDG

// LLY2.16, remove to pio.h
//#define PIN_USB_POWER 31  //Jeff 20050602 Add for USB power

//remove CDDA_BITSTREAM_* & CDDA_DISC_*
//Usage: __bCDDA_DTS==CDDA_PURE; if (__bCDDA_DTS==CDDA_DTS); ...
#define CDDA_UNKNOWN    0
#define CDDA_PURE       1
#define CDDA_HDCD       2
#define CDDA_DTS        3
#define CDDA_CDG        4


// LLY2.20, define the ID for __bZoomPos reference
#define POS_LAST_RIGHT  0x1
#define POS_LAST_LEFT   0x2
#define POS_LAST_UP     0x4
#define POS_LAST_DOWN   0x8



// ***** Function prototype definition Area *****
//[0] Test Tone
void    HAL_PlayTone(BYTE bCh);
#define HAL_TONE_CH_L   0
#define HAL_TONE_CH_R   1
#define HAL_TONE_CH_LS  2
#define HAL_TONE_CH_RS  3
#define HAL_TONE_CH_C   4
#define HAL_TONE_CH_LFE 5
#define HAL_TONE_CH_ALL 7


//# [1] Video relative function
void    HAL_Display(BYTE bDispType, BYTE bDispAction);
#define HAL_DISPLAY_SPST_DEC        1
#define HAL_DISPLAY_VIDEO           2   // LLY2.22
#define HAL_DISPLAY_SPST1           3
#define HAL_DISPLAY_SPST2           4

void    HAL_AdjustTVMode(DWORD dwFrameIdx);
void    HAL_TVSetAPS(BYTE bType); //
void    HAL_TVEncoderInitial(void); //
void    HAL_SetAspectRatio(BYTE bMode);

void    HAL_AdjustBrightness(BYTE bMode); //
#define HAL_BRIGHTNESS_NORMAL       0
#define HAL_BRIGHTNESS_BRIGHT       1
#define HAL_BRIGHTNESS_SOFT         2

void    HAL_AdjustContrast(BYTE bLevel); //
#define HAL_CONTRAST_LEVEL0         0
#define HAL_CONTRAST_LEVEL1         1
#define HAL_CONTRAST_LEVEL2         2
#define HAL_CONTRAST_LEVEL3         3
#define HAL_CONTRAST_LEVEL4         4
#define HAL_CONTRAST_LEVEL5         5
#define HAL_CONTRAST_LEVEL6         6
#define HAL_CONTRAST_LEVEL7         7
#define HAL_CONTRAST_LEVEL8         8

void    HAL_ControlHighlight(BYTE bAction, BYTE bButton);
#define HAL_HLI_ACTION_SELECT       1
#define HAL_HLI_ACTION_ACTIVE       2
#define HAL_HLI_ACTION_ENTER        3
#define HAL_HLI_ACTION_TURNOFF      4
#define HAL_HLI_BUTTON_UPPER        64
#define HAL_HLI_BUTTON_DOWN         65
#define HAL_HLI_BUTTON_LEFT         66
#define HAL_HLI_BUTTON_RIGHT        67

void    HAL_SetSCART(BYTE bScartType); //
#define HAL_SCART_CVBS              0
#define HAL_SCART_RGB               1
#define HAL_SCART_POWER_STANDBY     2
#define HAL_SCART_POWER_ON          3
#define HAL_SCART_16_9              4

//# [2] Audio relative function
void    HAL_SetAudioType(BYTE bType); //
#define HAL_AUDIO_NONE      0
#define HAL_AUDIO_BEGIN		1
#define HAL_AUDIO_PCM       (HAL_AUDIO_BEGIN+0)
#define HAL_AUDIO_MPG       (HAL_AUDIO_BEGIN+1)
#define HAL_AUDIO_AC3       (HAL_AUDIO_BEGIN+2)
#define HAL_AUDIO_MP3       (HAL_AUDIO_BEGIN+3)
#define HAL_AUDIO_MP2       (HAL_AUDIO_BEGIN+4)
#define HAL_AUDIO_LPCM      (HAL_AUDIO_BEGIN+5)
#define HAL_AUDIO_DTS       (HAL_AUDIO_BEGIN+6)
#define HAL_AUDIO_WMA       (HAL_AUDIO_BEGIN+7)
#define HAL_AUDIO_AAC       (HAL_AUDIO_BEGIN+8)
#define HAL_AUDIO_END		(HAL_AUDIO_AAC) // LLY2.10, new added AAC audio format for Nero Digital

// LLY2.38a, porting Chuan's code to support load JPEG decoder @ PROC2
#define HAL_VIDEO_BEGIN		200
#define HAL_VIDEO_JPG       (HAL_VIDEO_BEGIN+0)
#define HAL_VIDEO_END		(HAL_VIDEO_JPG)

void    HAL_SetAudioDAC(BYTE bSampleFreq); //
BYTE    HAL_LPCM_DownSample(BYTE bOn); // LLY2.30, used to control LPCM down sample


//# [3] Parser relative function
// LLY2.36, add return value to tell caller the desired action is ok or fail
//void    HAL_ControlParser(BYTE bPacketType, DWORD bID);
BYTE    HAL_ControlParser(BYTE bPacketType, DWORD bID);
#define HAL_PARSER_VIDEO_ID         1
#define HAL_PARSER_AUDIO_ID         2
#define HAL_PARSER_VIDEO_ONLY       3
#define HAL_PARSER_AUDIO_ONLY       4
#define HAL_PARSER_CDROM            5
#define HAL_PARSER_SP_ID            6
#define HAL_PARSER_NV_ID            7
#define HAL_PARSER_ENABLE           8
// LLY2.10, remove parser disable/ reset/ jpeg decode case,
// Since, nobody call it and implement it.
//#define HAL_PARSER_DISABLE          9
//#define HAL_PARSER_RESET            10
//#define HAL_PARSER_JPEG_DECODE      11
#define HAL_PARSER_STOP             12
#define HAL_PARSER_SP2_ID           13

// use for second parameter: HAL_ControlParser()
#define HAL_THROW_VIDEO             0xFF
#define HAL_THROW_AUDIO             0xFF
#define HAL_THROW_SP                0xFF


//# [6] CDROM decoding relative function //???
void    HAL_PrepareCDROMDecode(DWORD dwStartSector, BYTE bSectors);
void    HAL_CDROMDecode(void);
void    HAL_CDROMDecodeDone(void);
BOOL    HAL_IsNeedCDROMDecoding( DWORD hsgTime, BYTE NOofSector);


//# [9] Get bitstream info. relative function
BYTE     HAL_GetStreamInfo(void);

//# [10]Set play mode relative function
void    HAL_SetPlayMode(BYTE bMode); //
#define HAL_PLAYMODE_CDROM          0
#define HAL_PLAYMODE_MPEG           1
#define HAL_PLAYMODE_CDDA           2
#define HAL_PLAYMODE_JPEG           3

//# [11]Power on/off relative function
void    HAL_PowerDown(BYTE bPowerDown); //

//# [12]Zoom relative function
BYTE HAL_Zoom(BYTE bAction);
//see CC.H -- "VIEW_ZOOM_XXX"

//# [13]A/V buffer control relative function
#define HAL_FILL_VIDEO_BUFFER           0
#define HAL_FILL_AUDIO_BUFFER           1
#define HAL_FILL_AV_BUFFER              3

//# [14]Disc change relative fucntion
void    HAL_SetDiscType(WORD wDiscType);

//# [15]Reset relative function
void    HAL_Reset(BYTE bType); //
#define HAL_RESET_AUDIO         1
#define HAL_RESET_VIDEO         2
#define HAL_RESET_SPST          3 //?
#define HAL_RESET_TOTAL         4
// LLY2.10, remove reset NV case since nobody to implement it.
//#define HAL_RESET_NV            5 //?
#define HAL_RESET_AVSPST        6

// LLY2.01. remove this API since nobody to implement the code
#if 0
void    HAL_ResetPeripheral(BYTE bType, BYTE bAction); //
#define HAL_RESET_TVENCODER             0
#define HAL_RESET_AUDIODAC              1
#endif  // #if 0

//# [16]A/V sync control relative function
void    HAL_ControlAVSync(BYTE bType, DWORD dwParam);
#define HAL_AVSYNC_RESYNC       1 //dwParam=STC/SCR
#define HAL_AVSYNC_RESETSYNC    2 //dwParam=__dw1stSCR
#define HAL_AVSYNC_VIDEOMASTER  3 //dwParam=TRUE video master; FALSE audio master
#define HAL_AVSYNC_2SEC_LIMIT   4 // disable mechanism: audio STC - STC > 2sec, audio buffer will be reset


//# [18]A/V buffer setting relative function
// LLY2.01, remodify the parameter define for HAL_SetBuffer().
void    HAL_SetBuffer(BYTE bType, BYTE bMode);
// (1) for bType
#define HAL_SETBUFFER_AUDIO     0
#define HAL_SETBUFFER_VIDEO     1
// (2) for bMode
// (2-1) Audio buffer usage
#define HAL_AUDIOBUF_BEGIN          1
#define HAL_AUDIOBUF_MPG            (HAL_AUDIOBUF_BEGIN + 0)
#define HAL_AUDIOBUF_AC3            (HAL_AUDIOBUF_BEGIN + 1)
#define HAL_AUDIOBUF_DTS            (HAL_AUDIOBUF_BEGIN + 2)
#define HAL_AUDIOBUF_MP3            (HAL_AUDIOBUF_BEGIN + 3)
#define HAL_AUDIOBUF_LPCM           (HAL_AUDIOBUF_BEGIN + 4)
#define HAL_AUDIOBUF_PCM            (HAL_AUDIOBUF_BEGIN + 5)
#define HAL_AUDIOBUF_AAC            (HAL_AUDIOBUF_BEGIN + 6)    // LLY2.17, add for AAC
#define HAL_AUDIOBUF_WMA            (HAL_AUDIOBUF_BEGIN + 7)    // LLY2.35, add for WMA
#define HAL_AUDIOBUF_END            (HAL_AUDIOBUF_WMA)
// (2-2) Video buffer usage
#define HAL_VIDEOBUF_BEGIN          21
#define HAL_VIDEOBUF_NORMAL         (HAL_VIDEOBUF_BEGIN + 0)
#define HAL_VIDEOBUF_AVI            (HAL_VIDEOBUF_BEGIN + 1)
#define HAL_VIDEOBUF_MM_MOTION      (HAL_VIDEOBUF_BEGIN + 2)
// LLY2.20, integrate MM_AUDIO_ONLY & MM_VIDEO_ONLY into MM mode
#define HAL_VIDEOBUF_MM             (HAL_VIDEOBUF_BEGIN + 3)
#define HAL_VIDEOBUF_END            (HAL_VIDEOBUF_MM)


//# [22] TVE
void    HAL_SetTVE(void); //

//Initialization
void HAL_InitVariables(void);
void HAL_PowerOnInit(void);

void HAL_SetBufferThreshold(void);

#define     OVERSAMPLE_1X               (0x0)
#define     OVERSAMPLE_2X               (0x2)
#define     OVERSAMPLE_4X               (0x3)


//void HAL_SetPIP(RECT rectPIP, BYTE bBorderWidth, BYTE bBorderColor, BYTE bBackgroundColor);



void    HAL_PlayCommand(BYTE bCmdID, BYTE bParam); //bParam: slow factor; otherwise, NULL
#define         COMMAND_PLAY            0x01
#define         COMMAND_STEP            0x02
#define         COMMAND_FAST            0x03
#define         COMMAND_FREEZE          0x04
#define         COMMAND_SLOW            0x05
#define         COMMAND_SCAN            0x06
#define         COMMAND_DIGEST          0x08
#define         COMMAND_PAUSE           0x09
#define         COMMAND_STOP            0x10
//Micky2.04, add more interface
#define         COMMAND_V_UPDATE_SEQ    0x20
#define         COMMAND_V_PREDECODE     0x21
#define         COMMAND_V_CLEAR_STILL   0x22
#define         COMMAND_V_RELEASE_MODE  0x23
#define         COMMAND_V_ERROR_RATIO   0x24


//THUMBNAIL.h
#define THUMBNAIL_BACKGROUND_COLOR              0x22d472

// R/W Storage related function.
// LLY2.53, modify the parameter unit to support > 256 byte setup info
//void    HAL_ReadStorage(BYTE bAddr, BYTE* pbVal, BYTE bCnt);
//void    HAL_WriteStorage(BYTE bAddr, BYTE* pbVal, BYTE bCnt);
void    HAL_ReadStorage(WORD wAddr, BYTE* pbVal, WORD wCnt);
void    HAL_WriteStorage(WORD wAddr, BYTE* pbVal, WORD wCnt);

void    HAL_AdjustTFT_DigitalMode(BYTE bOn);

//kevin0.80
#ifdef MACROVISION_CERTIFICATION
extern WORD TVE_ReadMVRegN0N22W(BYTE bReg);
extern void TVE_WriteMVRegN0N22W(BYTE bReg, WORD wValue);
extern void TVE_WrieN0N22Default(void);
#define N0  0
#define N1  1
#define N2  2
#define N3  3
#define N4  4
#define N5  5
#define N6  6
#define N7  7
#define N8  8
#define N9  9
#define N10 10
#define N11 11
#define N12 12
#define N13 13
#define N14 14
#define N15 15
#define N16 16
#define N17 17
#define N18 18
#define N19 19
#define N20 20
#define N21 21
#define N22 22
#endif //#ifdef MACROVISION_CERTIFICATION

#include    "hio.h"
#include    "hsys.h"
#include    "hdevice.h"
#include    "hdecoder.h"
#include    "hadac.h"
#ifdef __cplusplus
   }
#endif

#endif  // __HAL_H__

