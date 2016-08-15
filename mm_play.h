/**************************************************************************

        (C)Copyright Cheertek Inc. 2007,
           J200, all right reserved

        Product : DVD/DMP Firmware

        Date    : 2007.4.10
        Author  : Cheertek (J200 WYC/CoCo)
        Purpose : The play/features of media manager.
        Sources : mm_play.h

****************************************************************************/
#ifndef __MM_PLAY_H__
#define __MM_PLAY_H__

////////////////////////////////////////////////////////////////
// LLY1.01, define the sector number gap for each pre-scan last SCR action
// LLY2.31, enlarge the sector number for prescan last SCR once time
// Since, it maybe too small of some bitstream to find any SCR value
#define JUMP_SECTOR_FOR_PRESCAN_LAST_SCR     100 // 25

// LLY1.06, define the sector number gap for each pre-scan *.vob's multi AST/SPST number information
#define JUMP_SECTOR_FOR_PRESCAN_VOB_INFO     100

// LLY2.22, define the timeout value for last SCR pre-scan action.
#define TIME_FOR_LAST_SCR_PRESCAN   COUNT_1_SEC

////////////////////////////////////////////////////////////////
// rename define name to MM_XXX.
//CoCo1.22, modify the definition about error message.
#define MM_JPEG_PARSE_HEADER_FAIL			7001
#define MM_JPEG_PARSE_HEADER_UNFINISH		7002
#define MM_JPEG_PARSE_HEADER_UNSUPPORT		7003
#define MM_JPEG_DECODE_FAIL					7004
#define MM_JPEG_DECODE_HANG					7005
#define MM_MP3_DECODE_HANG					7101
#define MM_WMA_DECODE_HANG					7201

// [7] The max time for redecoding action while decoding error
#define MM_REDECODE_COUNT_LIMIT				3


////////////////////////////////////////////////////////////////
//Use for MM_Trigger() stage -- LLY.275a
#define MM_STAGE_NONE                   0
#define MM_STAGE_PREPAREINFO            1
#define MM_STAGE_PARSE_HEADER           2
#define MM_STAGE_DECODE                 3
#define MM_STAGE_PLAYEND                4
#define MM_STAGE_CHECK_DECODE_RESULT    5
#define MM_STAGE_DISPLAY                6
#define MM_STAGE_POLLING_TIMEOUT        7
#define MM_STAGE_NEXT_ITEM              8
// LLY1.01 & LLY1.02, add some stage for _MM_MotionTrigger() usage
#define MM_STAGE_START_PRESCAN                      10
#define MM_STAGE_POLLING_1ST_PRESCAN_DONE           11
#define MM_STAGE_POLLING_IDXTBL_PRESCAN_DONE        12
#define MM_STAGE_PRESCAN_LAST_SCR                   13
#define MM_STAGE_POLLING_LAST_SCR_PRESCAN_DONE      14
#define MM_STAGE_SET_PLAYMODE                       15
#define MM_STAGE_WAIT_PLAY                          16
#define MM_STAGE_SKIP_ITEM                          17  
#define MM_STAGE_ERROR_CONCEAL                      18  // LLY1.10, add for error concealment stage
// LLY1.20, add new stage for char base SPST parsing done
#define MM_STAGE_POLLING_SP_PARSING_DONE            19
// LLY1.20, add new stage for MP4 prescan procedure
#define MM_STAGE_POLLING_MP4_INFO_PRESCAN_2ND_DONE  20
#define MM_STAGE_POLLING_MP4_IDXTBL_PRESCAN_DONE    21

//CoCo2.53, add the stage for JPEG+MP3
#define MM_STAGE_AUDIO_STAGE_INIT                   22
#define MM_STAGE_AUDIO_STAGE_PLAY                   23

////////////////////////////////////////////////////////////////
#define MM_JPEG_FRAME_START_H	((HALJPEG_DISPLAY_BUF_WIDTH - HALJPEG_FIRST_SCALING_WIDTH)/2)
#define MM_JPEG_FRAME_START_V	((HALJPEG_DISPLAY_BUF_HEIGHT-HALJPEG_FIRST_SCALING_HEIGHT)/2)
#define MM_JPEG_FRAME_WIDTH	HALJPEG_FIRST_SCALING_WIDTH
#define MM_JPEG_FRAME_HEIGHT	HALJPEG_FIRST_SCALING_HEIGHT

////////////////////////////////////////////////////////////////
//define the region for JPEG preview
// wyc2.22-909s, for 16M supporting.
#ifdef JPG_FRAME_640_480_MODE
#define MM_JPEG_PREVIEW_NTSC_START_H        150
#define MM_JPEG_PREVIEW_NTSC_START_V        280
#define MM_JPEG_PREVIEW_NTSC_WIDTH          96
#define MM_JPEG_PREVIEW_NTSC_HEIGHT         64
#define MM_JPEG_PREVIEW_PAL_START_H         144
#define MM_JPEG_PREVIEW_PAL_START_V         262
#define MM_JPEG_PREVIEW_PAL_WIDTH           96
#define MM_JPEG_PREVIEW_PAL_HEIGHT          64
#else
#define MM_JPEG_PREVIEW_NTSC_START_H        144
#define MM_JPEG_PREVIEW_NTSC_START_V        296
#define MM_JPEG_PREVIEW_NTSC_WIDTH          96
#define MM_JPEG_PREVIEW_NTSC_HEIGHT         64
#define MM_JPEG_PREVIEW_PAL_START_H         144
#define MM_JPEG_PREVIEW_PAL_START_V         (248+36) //"36" tune the MM UI to the center
#define MM_JPEG_PREVIEW_PAL_WIDTH           96
#define MM_JPEG_PREVIEW_PAL_HEIGHT          64
#endif //JPG_FRAME_640_480_MODE

////////////////////////////////////////////////////////////////
// define the trigger mode: Image, audio or motion.
#define MM_IMAGE_TRIGGER_MODE               1
#define MM_AUDIO_TRIGGER_MODE               2
#define MM_MOTION_TRIGGER_MODE              3
#define MM_TRIGGER_NONE                     4

////////////////////////////////////////////////////////////////
// LLY1.20, define motion state for __bMotionState
#define MOTION_STATE_NONE					0
#define MOTION_STATE_PRESCAN				1
#define MOTION_STATE_PLAY					2

#ifdef SUPPORT_ENCODE_JPG_PICTURE
#define MM_JPG_ENCODE_LIST_NULL				(0xFE)
#define MM_JPG_ENCODE_LIST_END				(0xFF)

#define MM_JPG_ENCODE_FULL					(0xFD)
#define MM_JPG_ENCODE_EMPTY					(0xFC)

#define MM_JPG_ENCODE_HAVE_DATA				(0xEE)
#endif //SUPPORT_ENCODE_JPG_PICTURE

////////////////////////////////////////////////////////////////
//Extern variables
extern BYTE	__bMMAudioStreamPause;
extern BYTE __bMMVideoStreamPause;

#ifdef SUPPORT_ENCODE_JPG_PICTURE
extern BYTE __bMMJPGEncodeNum;
#endif //SUPPORT_ENCODE_JPG_PICTURE

extern BYTE __bMMJPGStatus;
extern BYTE __bMMStage;
extern BYTE __bMMAtiveFrame;
extern DWORD __dwMMJPEGDisplayTime;
extern BYTE __dwMMJPEGPreview;
extern DWORD __dwMMJPGDispOK;
extern BYTE __bLastPlayItem;
extern BYTE __bMMEnterRealStop;
extern BYTE __bMMEffectIdx;
extern BYTE __bMotionState;
extern BYTE __bPBReady;
extern DWORD __dwMMTriggerMode;
extern BYTE __bMMFirstJPEGPlaying;
extern BYTE __bMMAutoPlay;
extern BYTE _bMMAVIFailCnt;
extern CHAR __cMMJPEGFMTExifDate[11];

extern BYTE __bMMJPEGAudioMode;  //CoCo2.53, for JPEG+MP3 mode.
extern CURSOR_INFO __MMBackgroundPlayingItem;
extern BYTE __bMMJPGEncodeJPGStatus[];

#ifdef SUPPORT_PHOTO_BROWSE_MODE
extern BYTE __bMMPhotoBrowseMode;
extern BYTE __bMMEnterBrowseFromJPEGAudioMode;
#endif //SUPPORT_PHOTO_BROWSE_MODE

#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
extern BYTE __bMMPlayMediaDirectlyMode;
extern BYTE __bMMStartJPEGAudioTrigger;
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON

////////////////////////////////////////////////////////////////
//Internal functions
void _MM_MotionTrigger(void);
void _MM_ImageTrigger(void);
void _MM_JPEGChangeToNext(void);
void _MM_JPEGRedecode(void);

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
BYTE _MM_JPEGAudioPlayInitial(void);
void _MM_GetNextPlayJPEGFile(void);
void _MM_GetPreviousPlayJPEGFile(void);
void _MM_GetNextPlayAudioFile(void);
BYTE _MM_FindJPEGFile(WORD wDirID);
BYTE _MM_FindAudioFile(WORD wDirID);
void _MM_SourceControl(void);
BYTE _MM_SetJPEGAudioPlayItem(void);
void MM_ExitJPEGAudioMode(void);
#endif //SUPPORT_JPEG_AUDIO_PLAYING

#ifdef SUPPORT_PHOTO_BROWSE_MODE
void _MM_EnterPhotoBrowseMode(void);
void MM_ExitPhotoBrowseMode(void);
#endif //SUPPORT_PHOTO_BROWSE_MODE

#ifdef SUPPORT_AUTO_ROTATION
void _MM_DetectAutoRotate(void);
#endif //SUPPORT_AUTO_ROTATION

void _MM_PhotoOperation(BYTE bKey);
BYTE _MM_ZoomModeOperation(BYTE bKey);
void _MM_DoPhotoEffect(void);
////////////////////////////////////////////////////////////////
//External functions
BYTE MM_ActionForChangeTrack(BYTE bAction, WORD wParam);
void MM_Trigger(void);
void MM_AudioTrigger(void);
BYTE MM_RepeatFolder(BYTE bKey);
void MM_EnterThumbMode(void);
void MM_ResetPreviewMode(void);
void MM_RecoverPreviewWindow(void);
void MM_CompleteJPGEffect(BYTE bSetToFirstPlayJPEG);
BYTE MM_JPEGPreview(void);

#ifdef CT950_STYLE
void MM_PhotoMusicErrorConcealment(void);
#endif //CT950_STYLE

#ifdef IMAGE_FRAME_SETUP
BYTE MM_GetFirstPlayableFile(void);
#endif //IMAGE_FRAME_SETUP

#ifdef SUPPORT_ENCODE_JPG_PICTURE
void MM_EncodeFile_Init(void);
BYTE MM_EncodeFile_Add(WORD wDir, WORD wFileIndex);
BYTE MM_EncodeFile_Delete(BYTE bFileIndex);
BYTE MM_EncodeFile_GetRange(BYTE bFileIndex);
BYTE MM_EncodeFile_GetFileID(BYTE bFileIndex);
BYTE MM_EncodeFile_Exchange(BYTE bFileIndex1, BYTE bFileIndex2);
BYTE MM_EncodeJPG(WORD wEncodeDirID, WORD wEncodeFileIndex, BYTE bFileID);
#endif //SUPPORT_ENCODE_JPG_PICTURE

#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
BYTE MM_PlayPhotoInFlash(void);
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON

void MM_PLAY_Initial(void);
////////////////////////////////////////////////////////////////


#endif //__MM_PLAY_H__
