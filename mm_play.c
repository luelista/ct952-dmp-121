//////////////////////////////////////////////////////////////
// ****** All necessary including files Area ******
#include    "winav.h"
// wyc0.60-909, include ctkav.h and <cyg/kernel/kapi.h> for thread control.
#include "ctkav.h"
#include    <string.h>
#include    "input.h"
#include    "infofilter.h"
#include    "cc.h"
#include    "osd.h"
#include    "mmanager.h"
#include    "utl.h"
#include    "initial.h" 
#include    "SrcFilter.h"
#include    "monitor.h"
#include    "setup.h"
#include    "chips.h"
#include    "hal.h"
#include    "linear.h"
#include    "thumb.h" 
#include    "osdmm.h"
#include    "panel.h"
#include    "parser.h"
#include    "disp.h"
#include    "jpegdec.h"
#include    "disp.h" 
#include    "media.h"
#include    "osddlg.h"

#ifdef SUPPORT_POWERON_MENU
#include "poweronmenu.h"
#endif //SUPPORT_POWERON_MENU

#ifdef SUPPORT_CHAR_SUBPICTURE
#include "char_subpict.h"
#endif

#include "gdi.h" 
#include    "comutl.h"
#ifdef SUPPORT_ENCODE_JPG_PICTURE
#include "jpegenc.h"
#endif //

#include "debug.h"
#undef PRINTF
#undef printf
// Micky2.17c
#ifdef	APPLICATION_WINDOWS
#define PRINTF		DBG_Printf_Simple
#define printf		DBG_Printf_Simple
#else
#define PRINTF(x, y...)     DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, x, ## y)
#define printf(x, y...)     DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, x, ## y)
#endif //APPLICATION_WINDOWS

//////////////////////////////////////////////////////////////
BYTE __dwMMJPEGPreview = FALSE; //CoCo0.72, support JPEG Preview
BYTE __bMMJPGDecEncMode;
//////////////////////////////////////////////////////////////
BYTE _bMMPlayable;
BYTE __bMMJPGStatus;
//////////////////////////////////////////////////////////////
BYTE __bMMStage;
DWORD __dwMMTriggerMode;
BYTE __bMMAtiveFrame;
DWORD __dwMMJPEGDisplayTime;
BYTE __bMMEffectIdx;
DWORD __dwFMJPGShowTime;
DWORD __dwMMJPGDispOK;
CHAR __cMMJPEGFMTExifDate[11];
//////////////////////////////////////////////////////////////
DWORD __dwBuffEmptyTime;
DWORD _dwMMTriggerEmptyTime;
DWORD __dwMMJPGBuffEmptyTime;
//////////////////////////////////////////////////////////////
BYTE __bMMCountStopBak;
DWORD __dwMMZoomMode;
BYTE __bMMEnterRealStop;
//////////////////////////////////////////////////////////////
BYTE __bLastPlayItem;
BYTE __bPBReady;
//////////////////////////////////////////////////////////////
BYTE __bMMrogressCnt;
WORD __wMMPreProgressCurrent;
//////////////////////////////////////////////////////////////
BYTE _bMMAudioDisplayInfo = FALSE; //CoCo0.77, add audio info.
//////////////////////////////////////////////////////////////

// wyc2.38a-909s, record the AVI failed count. 
BYTE _bMMAVIFailCnt;
BYTE __bMMIndexTableOK;
// LLY1.20, add a new variable to keep motion state
BYTE __bMotionState;
//////////////////////////////////////////////////////////////
// LLY2.50, add a new variable to keep current file is un-supported format
BYTE __bUnknownFormat;
//////////////////////////////////////////////////////////////
//Define the variables for JPEG+MP3 mode
DWORD __dwMMAudioPreGetTime;
BYTE __bMMJPEGAudioMode = FALSE; 
CURSOR_INFO __MMBackgroundPlayingItem;  //The background playing item.
BYTE _bMMAudioSameItem;
BYTE __bMMAudioPlayEnd;
BYTE __bMMAudioStage;                   //The stage needs to be changed to MM_STAGE_PREPAREINFO when start to play.
WORD __wMMCurrentPlayingJPEGFileIndex;
WORD __wMMCurrentPlayingAudioFileIndex;
WORD __MMFirstJPEGPlayingIndex; 
WORD __wMMTotalJPEGFileInDir;
WORD __wMMTotalAudioFileInDir;
BYTE __bMMStartJPEGAudioTrigger = FALSE;
BYTE __bMMSkipAudio = FALSE;
BYTE __bMMVideoStreamPause = FALSE;
BYTE __bMMAudioStreamPause = FALSE;
///////////////////////////////////////////////////////////
#ifdef SUPPORT_WMA
DWORD dwSample;
DWORD dwPreSample;
#endif //SUPPORT_WMA

#ifdef DOLBY_CERTIFICATION
BYTE bShowDolbySurrFlag;
#endif //DOLBY_CERTIFICATION
///////////////////////////////////////////////////////////
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
BYTE __bMMSlideShowCopyDeleteMode;
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW

#ifdef SUPPORT_AUTO_ROTATION
BYTE __bMMPhotoAutoRoate = FALSE;
BYTE __bMMPhotoDoRoate = FALSE;
DWORD _dwMMAutoRotateCount;
BYTE _bMMPhotoAutoRoateOperation;
#endif //SUPPORT_AUTO_ROTATION

#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
BYTE __bMMPlayMediaDirectlyMode = FALSE;
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
///////////////////////////////////////////////////////////
// wyc2.51-909P, some define and variables for JPG encode mode.
#ifdef SUPPORT_ENCODE_JPG_PICTURE
BYTE    __bMMJPGEncodeEntry, __bMMJPGEncodeNum;
BYTE    __bMMJPGEncodeList[SRCFTR_SUPPORT_JPG_ENCODE_NUM];
BYTE    __bMMJPGEncodeJPGStatus[SRCFTR_SUPPORT_JPG_ENCODE_NUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#endif //SUPPORT_ENCODE_JPG_PICTURE
BYTE    __bMMSaveProgress;
///////////////////////////////////////////////////////////
#ifdef IMAGE_FRAME_SETUP 
BYTE __bMMFirstJPEGPlaying = TRUE;
#endif //IMAGE_FRAME_SETUP

#ifdef IMAGE_FRAME_SETUP
BYTE __bMMAutoPlay = FALSE;
#else
#ifdef MMANAGER_AUTO_PLAY
BYTE __bMMAutoPlay = TRUE;
#else
BYTE __bMMAutoPlay = FALSE;
#endif //MMANAGER_AUTO_PLAY
#endif //IMAGE_FRAME_SETUP
///////////////////////////////////////////////////////////
#ifdef SUPPORT_PHOTO_BROWSE_MODE
BYTE __bMMPhotoBrowseMode = FALSE;
BYTE __bMMEnterBrowseFromJPEGAudioMode = FALSE;
#endif //SUPPORT_PHOTO_BROWSE_MODE
///////////////////////////////////////////////////////////
// ****** Extern Other Module's Variable Area ******
extern DWORD __dwPreGetTime;  
extern BYTE __bResetRecover; 
extern WORD __wProgressCurrent;
extern WORD __wProgressTotal;
extern CHAR __cJPEGFMTExifDate[11];
///////////////////////////////////////////////////////////

// elmer2.78, add for DMP error concealment
DWORD __dwMMJPGCount = 1, __dwMMPreJPGCount;

//  *********************************************************************
//  Function    :   MM_Trigger
//  Description :   Check PlayEnd if player is playing...
//  Arguments   :   NONE
//  Return      :   TRUE, the stage finish ok
//                  FALSE, the stage fail
//  Side Effect :
//  *********************************************************************
void MM_Trigger(void)
{
    if (__bThumbnailMode)
        return;

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    if (__bMMJPEGAudioMode && __bMMStartJPEGAudioTrigger)
    {        
        if (__btPause == FALSE)
        {
            MM_AudioTrigger();                
            _MM_ImageTrigger();
        }
    }
    else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
    {        
        // wyc1.10-909, when pause mode, need to reset __dwBuffEmptyTime to UTL_GetSysTimer to avoid timer continue going and
        // follow condition will FALSE after pause 3-5 minutes.
        if ((__btPause) && (__bMMStage == MM_STAGE_PLAYEND) && (__btPlayEnd == TRUE))
		{
            __dwBuffEmptyTime = OS_GetSysTimer();
		}
        
        // LLY1.02, let it always go _MM_MotionTrigger() for CDROM motion file
        // LLY1.20, must check motion state since __bAttrPlay is not set ready during prescan mode
        if ((__bAttrPlay & TYPE_CDROM_AV) || (__bMotionState != MOTION_STATE_NONE))
        {
            // LLY1.10, don't check __bPBReady value while STOP mode
            // Otherwise, _MM_MotionTrigger() will be called while STOP mode.
            // Bug: progress bar will always be displayed during STOP mode.
            if (__bModeCmd != KEY_STOP)
            {
                // LLY1.02, don't do motion trigger if playback ready w/ non-playing or puase mode
                // Notce: Can't using __bMMStage==STAGE_PLAYEND as checking flag, because following bugs:
                // (1) forever trap @ NEXT_ITEM stage while 1st file playend w/o repeat mode
                //     Since current solution is to assign F/W key by KEY_STOP not to issue command directly
                // (2) it maybe polling end time-out and try to playback next item (since __btPlayEnd==TRUE)
                //     while user press KEY_STOP during parser report data sending done or presee KEY_STOP three times
                // Also can't using __bMMStage==STAGE_NEXT as checking flag
                // Otherwise, the pre-scan fail next file action can't work
                if (__bPBReady && (!__btPlaying || __btPause))
				{
                    return;
				}
            }
            else
            {
                if (!__btPlaying || __btPause)
				{
                    return;
				}
            }
            
            _MM_MotionTrigger();
        }
        
        if (!(__wDiscType & CDROM_M1) || (!__btPlaying) || __btPause)
        {
            if (__btPause && (__bAttrPlay == ATTR_JPG))
                ;
            else
                return;
        }

        if (__bAttrPlay & TYPE_CDROM_VIDEO)
		{
            _MM_ImageTrigger();
		}
        else if (__bAttrPlay & TYPE_CDROM_AUDIO)
		{
            MM_AudioTrigger();
		}
    }
}

void    _MM_MotionTrigger(void)
{
    static BYTE bPreScanCnt=0;

#ifdef  SUPPORT_PSNR_TEST
    extern DWORD   __dwDecodeCnt;
#endif  // #ifdef SUPPORT_PSNR_TEST

#ifdef IMAGE_FRAME_SETUP 
    __bMMFirstJPEGPlaying = TRUE;
#endif //IMAGE_FRAME_SETUP

    // LLY2.36, process DRM Playback State first
    // Otherwise, it maybe continue go other stage during waiting user input period
    // [1] If wait user action, don't go trigger
    if(__bDRMPlayState == DRM_PLAY_STATE_WAIT)
    {
        return;
    }
    // [2] Go wait play stage directly if user abort the action
    //     and current file is not finish index table/ SP parsing.
    //     Otherwise, user will see parscan action continue then stop after abort
    else if ((__bDRMPlayState == DRM_PLAY_STATE_ABORT) && (__bMMStage==MM_STAGE_POLLING_SP_PARSING_DONE))
    {
        __bMMStage = MM_STAGE_WAIT_PLAY;
    }

    switch(__bMMStage)
    {
        // LLY1.02, abstract issue pre-scan action into a new stage
        // Since, it's un-necessary to do pre-scan action for the same play item
	case    MM_STAGE_PREPAREINFO:
		__bLOGO = LOGO_NONE;
		__dwMMTriggerMode = MM_MOTION_TRIGGER_MODE;
		
		// LLY2.50, initial unknow format as FALSE
		__bUnknownFormat = FALSE;
		
		// wyc1.50-909, need to complete JPG effect here, other motion file will be abnormal in video.
		MM_CompleteJPGEffect(TRUE);
		// wyc2.17-909S, clear JPG effect mode when changing to motion files to avoid motion file image will keep in JPG effect slide show mode.
		__bHALJPEGEffect=HALJPEG_EFFECT_NONE;
		__bMMEffectIdx = HALJPEG_EFFECT_NONE;
		// wyc1.01-909, modify the audio time out to LSF mode, because some LSF file need more time to wait audio buffer empty to play completed file.
		// 32 is using LSF 32K to compute, the computed time mul 1000 is the unit of timer is ms. So need to transfer second to ms.
		// LLY1.20, AVI buffer setting is different from other motion file
		if (__bAttrPlayNew == ATTR_AVI)
		{
			_dwMMTriggerEmptyTime = ((((DS_AD0BUF_END_AVI-DS_AD0BUF_ST_AVI)*8)/(64*1000))+5)*COUNT_1_SEC;
		}
		else
		{
			_dwMMTriggerEmptyTime = ((((DS_AD0BUF_END-DS_AD0BUF_ST)*8)/(64*1000))+5)*COUNT_1_SEC;
		}
		
		// LLY1.02, initial the playback ready flag as FALSE
		__bPBReady=FALSE;
		__bMMrogressCnt = 0;
		
		// LLY1.02, freeze the STC value to avoid OSD time keep previous file last time
		// during open/cose --> play.
		// Since STC value don't clear to zero show LOGO (STC is unknow, decoder do nothing)
		// Notice: the STC will continue to go while next right STC update by decoder or DSP
		DISP_STCFreeze(0, DISP_WAIT_NEXT_STC);
		
		// Also set the STC value as zero, since UTL_TriggerShowTime()
		// don't readback the STC value while !__btPlaying
		// Fix OSD time not reset to zero while NEXT/PREV action.
		__dwSTCValue=0;
		
		__bMMIndexTableOK = TRUE;
		
		// If same play item & not copy protection file, set the playmode directly
		// Otherwise, start the pre-scan action.
		if(__bSameItem && !__PARSER_gbl.BitsContent.fCopyProtection)
		{
			__bMMStage=MM_STAGE_SET_PLAYMODE;
		}
		else
		{
			__bMMStage=MM_STAGE_START_PRESCAN;
			
			// LLY1.06, initial the prescan VOB information cnt as 1
			bPreScanCnt=1;
		}
		break;
		
	case    MM_STAGE_START_PRESCAN:
		// LLY1.10, always "keep current playback item" flow while issue pre-scan command
		// And, don't set __bSameItem value, since it will be set during LINEAR_PlayItem()
		// Otherwise, F/W will think it as same file while current pre-scan action abort --> KEY_PREV
		// Notice: original code is put @ MM_STAGE_SET_PLAYMODE
		__wPrevPlayItem=__wPlayItem;
		__wPrevPlayDir=__MMPlayingItem.wDirID;
		
		
		// LLY1.01, Issue pre-scan command first, and enter polling pre-scan done stage
		// Notice: Must using __bAttrPlayNew instead of __bAttrPlay
		//         Because, UTL_SetPlayMode() is called within MM_STAGE_SET_PLAYMODE
		//         So, __bAttrPlay is not ready
		if(__bAttrPlayNew==ATTR_AVI)
		{
		    // Grievous2.78, must reset sp mode as unknown
		    __bUTLSPTextMode = 2;
			UTL_PreScanAction(PRESCAN_MOTION_INFO, __dwTimeBegin, __dwTimeEnd);
		}
		else if(__bAttrPlayNew & TYPE_CDROM_AV)
		{
			// LLY1.06, parseing multi-audio/ subpicture information for *.VOB/*.DAT/*.MPG together
			// [1] Calculate the desired end sector for source to send data during prescan
			__dwTemp = __dwTimeBegin + (bPreScanCnt*JUMP_SECTOR_FOR_PRESCAN_VOB_INFO);
			
			// (desired end position-gap) < file end < (desired end position)
			if( (__dwTemp>__dwTimeEnd) && ((__dwTemp-JUMP_SECTOR_FOR_PRESCAN_VOB_INFO)<__dwTimeEnd) )
			{
				// Assign range as (desired end position - gap) ~ (file end)
				// Notice: (__dwTemp-JUMP_SECTOR_FOR_PRESCAN_VOB_INFO) maybe == file begin.
				UTL_PreScanAction(PRESCAN_1ST_SCR, (__dwTemp-JUMP_SECTOR_FOR_PRESCAN_VOB_INFO), __dwTimeEnd);
			}
			// desired end position <= file end
			else if( __dwTemp<= __dwTimeEnd )
			{
				// Assign range as (desired end position - gap) ~ desired end position
				UTL_PreScanAction(PRESCAN_1ST_SCR, (__dwTemp-JUMP_SECTOR_FOR_PRESCAN_VOB_INFO), __dwTemp);
			}
			// end position is out-of the playback range
			else
			{
				// Set error while can't find the 1st SCR value
				UTL_SetPreScanError(ERR_1ST_PRESCAN_FAIL);
				
				// Keep the time for enter skip item stage for time-out control
				__dwPreGetTime=OS_GetSysTimer();
				
				// Skip current file
				__bMMStage=MM_STAGE_SKIP_ITEM;
				
				DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"1st p-scan fail\n");
				
				// LLY2.50, output message while skip file since pre-scan fail
#ifndef  NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
				strcpy(__bMsgString, "Skip File since prescan fail");
				OSD_Output(MSG_STRING, 0, 10);
#endif  // #ifdef  NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
				
				break;
			}
#ifdef MM_MOTION_DEBUG
			DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"1st PreScan: %lx\n", bPreScanCnt);
#endif //
			
		}
		
		// Set the stage to polling prescan command done
		__bMMStage = MM_STAGE_POLLING_1ST_PRESCAN_DONE;
		break;
		
	case    MM_STAGE_POLLING_1ST_PRESCAN_DONE:
		if (__bAttrPlayNew == ATTR_AVI)
		{
			__bMMrogressCnt++;
			if ((__bMMrogressCnt % MM_MSG_PROGRESS_DIV_NUM) == 1)
			{
				if (__bMMrogressCnt == 1)
					OSD_Output(MSG_PROGRESS, 1, 1);
				else
					OSD_Output(MSG_PROGRESS, 0, 1);
			}
		}
		// [1] Waiting until parser report prescan action done
		if( !PARSER_QueryEvent(0, PARSER_EVENT_COMPLETED, FALSE) )
			break;
		
		// [2] Stop servo first after pre-scan done.
		SrcFilter_Stop(__dwSFStreamID);		
		
		// [3] Check the result of 1st pre-scan command
		if( PARSER_QueryEvent(0, PARSER_EVENT_ERROR, FALSE) )
		{
			// LLY1.06, do another range pre-scan action for *.VOB/ *.DAT/ *.MPG file
			if(__bAttrPlayNew & TYPE_CDROM_AV)
			{
				if(bPreScanCnt<5)
				{
					// Increase the prescan count
					bPreScanCnt++;
					
					// Set the stage to prescan 1st SCR again
					__bMMStage = MM_STAGE_START_PRESCAN;
					
					break;
				}
			}
			
			// If pre-scan action fail
			// Do error procedure while 1st prescan fail
			UTL_SetPreScanError(ERR_1ST_PRESCAN_FAIL);
			
			// Increase the retry count by one
			__bResetRecover++;
			
			// LLY1.02, retry the same file again if 1st pre-decode fail
			// Otherwise, go to the next file.
#ifdef  REDECODE_CURRENT_ITEM
			if(__bResetRecover <= MM_REDECODE_COUNT_LIMIT)
			{
				
				__bMMStage=MM_STAGE_START_PRESCAN;
				
				SRCFilter_EnableCacheFlush();  // F100CSC_102, for flush Servo's cache, i.e. read all sector again
				
#ifdef MM_MOTION_DEBUG
				DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"Retry same file, Retry Cnt: %lx\n", __bResetRecover);
#endif //
			}
			else
#endif  // #ifdef REDECODE_CURRENT_ITEM
			{
				// Keep the time for enter skip item stage for time-out control.
				__dwPreGetTime=OS_GetSysTimer();
				__bMMStage=MM_STAGE_SKIP_ITEM;
				
				DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"G-up, 1st p-scan fail\n");
				
				// LLY2.50, output message while skip file since pre-scan fail
#ifndef  NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
				strcpy(__bMsgString, "Skip File since prescan fail");
				OSD_Output(MSG_STRING, 0, 10);
#endif  // #ifdef  NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
				
			}
			break;
		}
		
		// [4] If pre-scan action success
		// Notice: Must using __bAttrPlayNew instead of __bAttrPlay
		//         Because, UTL_SetPlayMode() is called within MM_STAGE_SET_PLAYMODE
		//         So, __bAttrPlay is not ready
		if( __bAttrPlayNew == ATTR_AVI ) // for DivX, AVI, MP4 file
		{
#ifdef  SUPPORT_MP4
			// LLY1.20, Add procedure to check MP4 header prescan action done or not first
			if(MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem) == EXTNAME_MP4)
			{
				// Check if MP4 header is ready or not
				if(PARSER_QueryEvent(0, PARSER_EVENT_NOT_READY, TRUE))
				{
					// Update start position again before issue 2nd MP4 header prescan
					__dwTemp = PARSER_GetMP4MoovBoxPosition();
					
					// Continue to 2nd MP4 header prescan action
					UTL_PreScanAction(PRESCAN_MOTION_INFO, __dwTemp, __dwTimeEnd);
					
					// And, let the stage still to polling 1st prescan action done
					// ie. don't change the stage value
					break;
				}
			}
#endif  // #ifdef SUPPORT_MP4
			// Read-back the necessary information
			UTL_Read_PreScan_Info(PRESCAN_MOTION_INFO);
			
			// LLY1.06, skip un-supported video resolution DivX file
#ifndef  DONT_SKIP_UNSUPPORT_V_FORMAT_DIVX
			if(__bVideoID == HAL_THROW_VIDEO)
			{
				__dwPreGetTime=OS_GetSysTimer();
				__bMMStage=MM_STAGE_SKIP_ITEM;
				DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"Skip un-support A/V format DivX file\n");
				
				// LLY2.50, output message while skip un-supported video format file
#ifndef NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
				strcpy(__bMsgString, "Skip File since not supported video format");
				OSD_Output(MSG_STRING, 0, 10);
                // Winnie2.77, clear the video as black screen if skip video format since it may stay @ dialog box and don't play next file immediately.
#ifdef BLACK_SCREEN_WHILE_THROW_VIDEO                 
                HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
#endif //BLACK_SCREEN_WHILE_THROW_VIDEO
#endif //NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
				break;
			}
#endif //SKIP_UNSUPPORT_VIDEO_RESOLUTION_DIVX
			
			// LLY1.02, skip DRM/ AES file if not support DRM solution
			// LLY1.50, check DRM play state directly
			if(__bDRMPlayState==DRM_PLAY_STATE_SKIP)
			{
				// Keep the time for enter skip item stage for time-out control.
				__dwPreGetTime=OS_GetSysTimer();
				__bMMStage=MM_STAGE_SKIP_ITEM;
				DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"Skip DRM/ AES\n");
				
				// LLY2.50, output message while skip DRM/ AES file
#ifndef NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
				strcpy(__bMsgString, "Skip file since not supported DRM/AES");
				OSD_Output(MSG_STRING, 0, 10);
				// Winnie2.77, clear the video as black screen if skip DRM/ AES file since it may stay @ dialog box and don't play next file immediately.
#ifdef BLACK_SCREEN_WHILE_THROW_VIDEO                
				DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Black Screen for skip DRM/ AES file\n");
				HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
#endif //BLACK_SCREEN_WHILE_THROW_VIDEO
				
#endif //NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
				
				break;
			}
			
			// LLY1.10, skip the non support motion file if the A/V all invalid
			if (!UTL_QueryCapability(QUERY_VALID_MOTION))
			{
				// Keep the time for enter skip item stage for time-out control
				__dwPreGetTime=OS_GetSysTimer();
				__bMMStage=MM_STAGE_SKIP_ITEM;
				DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"Skip, A/V non-valid\n");
				
				// LLY2.50, output message while skip un-supported video format file
#ifndef NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
				strcpy(__bMsgString, "Skip File since not supported A/V format");
				OSD_Output(MSG_STRING, 0, 10);
#endif  //NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
				break;
			}
			
			// LLY1.20, enter parsing char-base SPST stage first
			// Notice: (1) skip this action if same file
			//         (2) can't do it after index table parsing done
			//             since it will destroy index table buffer
			if(__bSameItem)
			{
				__bMMStage = MM_STAGE_SET_PLAYMODE;
			}
			else
			{
#ifdef SUPPORT_CHAR_SUBPICTURE
#ifndef SUPPORT_INTERNAL_AND_CHAR_BASE_SUBPICTURE_TOGETHER
				if(__bSPSTNs == 0)
#endif //SUPPORT_INTERNAL_AND_CHAR_BASE_SUBPICTURE_TOGETHER
				{
#ifdef MM_MOTION_DEBUG
					DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"Parser SP File\n");
#endif //MM_MOTION_DEBUG
					CHAR_SP_ParseSPFile();
				}
#endif //SUPPORT_CHAR_SUBPICTURE
				__bMMStage = MM_STAGE_POLLING_SP_PARSING_DONE;
			}			
		}
		else if(__bAttrPlayNew & TYPE_CDROM_AV) // For VOB/ DAT/ MPG file
		{
			// Read-back 1st SCR value
			UTL_Read_PreScan_Info(PRESCAN_1ST_SCR);
			
			__bMMStage = MM_STAGE_PRESCAN_LAST_SCR;
			
			bPreScanCnt=1;
		}
		break;
		
        // LLY1.20, add new stage to polling char base SPST paring done
        case    MM_STAGE_POLLING_SP_PARSING_DONE:
            // waiting char base SP parsing done
			
			__bTemp=UTL_PreScanAction(PRESCAN_MOTION_IDXTBL, __dwTimeBegin, __dwTimeEnd);
			
			// LLY1.05-1, if prescan motion index table command ok, go to polling done stage
			// and, no index table exist will return FALSE
			// wyc2.38a-909s, check if index table parsing hang, if don't update in 10s, we will abort the parsing flow.
			if(__bTemp)
			{
				__dwMMJPEGDisplayTime = OS_GetSysTimer();
				__wMMPreProgressCurrent = (__wProgressCurrent*100)/__wProgressTotal;
				__bMMStage = MM_STAGE_POLLING_IDXTBL_PRESCAN_DONE;
			}
			// Otherwise, go the set playing mode stage
			else
			{
				__bMMStage = MM_STAGE_SET_PLAYMODE;
			}
            break;
			
        case    MM_STAGE_POLLING_IDXTBL_PRESCAN_DONE:
            if (__bAttrPlayNew == ATTR_AVI)
            {
                __bMMrogressCnt++;
                if ((__bMMrogressCnt % MM_MSG_PROGRESS_DIV_NUM) == 1)
                {
                    OSD_Output(MSG_PROGRESS, 0, 1);
                }
            }
            // wyc2.38a-909s, check if index table parsing hang, if don't update in 10s, we will abort the parsing flow.
            if (((OS_GetSysTimer() - __dwMMJPEGDisplayTime) < COUNT_10_SEC*3) || ((((__wProgressCurrent*100)/__wProgressTotal) - __wMMPreProgressCurrent) > 5))
            {
                // [1] Waiting until parser report prescan action done
                if (!PARSER_QueryEvent(0, PARSER_EVENT_COMPLETED, FALSE))
                    break;
            }
            else
            {
                HAL_ControlParser(HAL_PARSER_STOP, 0);
                __bMMIndexTableOK = FALSE;
            }
			
            // [2] Stop servo first after pre-scan done.
            SrcFilter_Stop(__dwSFStreamID);
			
			// Check if MP4 header is ready or not
			if (PARSER_QueryEvent(0, PARSER_EVENT_NOT_READY, TRUE))
			{
				// Continue to 2nd MP4 header prescan action
				UTL_PreScanAction(PRESCAN_MOTION_IDXTBL, __dwTimeBegin, __dwTimeEnd);
				
				// And, let the stage still to polling 1st prescan action done
				// ie. don't change the stage value
				break;
			}
			
            // Query AVI vaild movie end position
            // And, update it to __dwTimeEnd if it reports less value than file end range
            {
                DWORD   dwTmp;
				
                dwTmp = PARSER_GetMovieEndPosition(__dwTimeBegin);
                if ( dwTmp < __dwTimeEnd)
                {
                    __dwTimeEnd = dwTmp;
                }
            }
			
            // [3] Set Error if pre-scan AVI index table command fail
            if (PARSER_QueryEvent(0, PARSER_EVENT_ERROR, FALSE))
            {
                // Do error procedure while AVI index table pre0scan fail
                UTL_SetPreScanError(ERR_NOT_FIND_AVI_IDXTBL);
            }
            else
            {
                if (__bMMIndexTableOK == TRUE)
                {
                    // If AVI index table pre-scan success.
                    UTL_Read_PreScan_Info(PRESCAN_MOTION_IDXTBL);
                }
                else
                {
					UTL_SetPreScanError(ERR_NOT_FIND_AVI_IDXTBL);
                }
            }
			
            // [4] Set the stage to set play mode.
            __bMMStage = MM_STAGE_SET_PLAYMODE;
            break;
			
        case    MM_STAGE_PRESCAN_LAST_SCR:
            // Calculate the desired range for source to send data during prescan the last SCR value
            __dwTemp = __dwTimeEnd - (bPreScanCnt*JUMP_SECTOR_FOR_PRESCAN_LAST_SCR);
			
            // LLY2.22, keep the start time while do last SCR prescan
            __dwPreGetTime = OS_GetSysTimer();
			
            // start position > file begining.
            if (__dwTimeBegin < __dwTemp)
            {
                // Issue prescan last SCR command
                UTL_PreScanAction(PRESCAN_LAST_SCR, __dwTemp, (__dwTemp+JUMP_SECTOR_FOR_PRESCAN_LAST_SCR));
				
                // Set stage to polling prescan last SCR done
                __bMMStage = MM_STAGE_POLLING_LAST_SCR_PRESCAN_DONE;
            }
            // start position <= file beginning < (start postion + jump range)
            else if( (__dwTemp<=__dwTimeBegin) && ((__dwTemp+JUMP_SECTOR_FOR_PRESCAN_LAST_SCR)>__dwTimeBegin) )
            {
                // Issue prescan last SCR command
                UTL_PreScanAction(PRESCAN_LAST_SCR, __dwTimeBegin, (__dwTemp+JUMP_SECTOR_FOR_PRESCAN_LAST_SCR));
				
                // Set stage to polling prescan last SCR done
                __bMMStage = MM_STAGE_POLLING_LAST_SCR_PRESCAN_DONE;
            }
            // start position is out-of the playback range
            else
            {
                // Set error while can't find the last SCR value
                UTL_SetPreScanError(ERR_NOT_FIND_LAST_SCR);
				
                // Set the stage to start playback
                __bMMStage = MM_STAGE_SET_PLAYMODE;
            }
            break;
						
        case    MM_STAGE_POLLING_LAST_SCR_PRESCAN_DONE:
            // [1] Waiting until parser report prescan action done
            if( !PARSER_QueryEvent(0, PARSER_EVENT_COMPLETED, FALSE) )
            {
                // LLY2.22, do time-out protection while waiting parser complete
                // Notice: if 1st prescan last SCR procedure need plus "long seek time" (about 3 sec)
                //         others only need < 1 sec (default disable long seek issue.
				if(OS_GetSysTimer()-__dwPreGetTime > TIME_FOR_LAST_SCR_PRESCAN)
				{
					DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, "SCR T-Out\n");
					
					// Force to stop parser and servo first
					HAL_ControlParser(HAL_PARSER_STOP, 0);
					SrcFilter_Stop(__dwSFStreamID);
					goto PRESCAN_LAST_SCR_FAIL;
					
				}
				break;
            }
			
            // [2] Stop servo first after pre-scan done.
            SrcFilter_Stop(__dwSFStreamID);
			
            // [3-1] If find the last SCR value
            if( !PARSER_QueryEvent(0, PARSER_EVENT_ERROR, FALSE) )
            {
                UTL_Read_PreScan_Info(PRESCAN_LAST_SCR);
                __bMMStage = MM_STAGE_SET_PLAYMODE;
            }
            // [3-2] If can't find the last SCR value, continue the pre-scan last SCR again.
            else
            {
PRESCAN_LAST_SCR_FAIL:
			if(bPreScanCnt>=5)
			{
				// Set error while can't find the last SCR value
				UTL_SetPreScanError(ERR_NOT_FIND_LAST_SCR);
				
				// Set the stage to start playback
				__bMMStage = MM_STAGE_SET_PLAYMODE;
				
			}
			else
			{
				bPreScanCnt++;
				
				// Set the stage to prescan last SCR again
				__bMMStage = MM_STAGE_PRESCAN_LAST_SCR;
			}
            }
            break;
			
        case    MM_STAGE_SET_PLAYMODE:
            // wyc1.10a-909, fix some AVI file don't show progress bar to 100% problem.
            if (__bAttrPlayNew == ATTR_AVI)
            {
                OSD_Output(MSG_PROGRESS, 0, 1);
            }
			
            UTL_PlayItem(__wPlayItem, 0);
            if(__bAttrPlay == ATTR_AVI)
            {
                __bMMStage = MM_STAGE_WAIT_PLAY;
            }
            else
            {
                __bMMStage = MM_STAGE_PLAYEND;
            }
            break;
						
			// LLY0.95, wait play command
        case    MM_STAGE_WAIT_PLAY:
            // If continue, issue playback command, and go to playend state.
            if (__bDRMPlayState==DRM_PLAY_STATE_CONTINUE)
            {
                // If no copy protection file, issue play command directly
                if (!__PARSER_gbl.BitsContent.fCopyProtection)
                {
                    UTL_Start_PlayItem(0);
                    __bMMStage = MM_STAGE_PLAYEND;
                }
#ifndef NO_DRM
                else
                {
                    __bDRMPlayState=UTL_DRM_StartPlayback();
                    if(__bDRMPlayState==DRM_PLAY_STATE_CONTINUE)
                    {
                        UTL_Start_PlayItem(0);
                        __bMMStage = MM_STAGE_PLAYEND;
                    }
                    else if(__bDRMPlayState==DRM_PLAY_STATE_ABORT)
                    {
                        __bMMStage = MM_STAGE_NONE;
                        // How to specify the stop @ current file & display "stop" ??
                        // If assign F/W key by KEY_STOP, it will stop @ 1st file of current directory.
                        // If assing F/W key by KEY_RETURN, it can stop @ current playing file
                        // but it will show "Return" on screen.
                        __bFWKey=KEY_RETURN;
                    }
                }
#endif  // #ifndef NO_DRM
            }
            // If abort, enter stop mode
            else if(__bDRMPlayState==DRM_PLAY_STATE_ABORT)
            {
                __bMMStage = MM_STAGE_NONE;
                __bFWKey=KEY_RETURN;
            }
			
			
            // LLY2.35, display message to info user that not supported audio/ video format
            // while start to playback bitstream
            // Notice: don't show this message to earily (ex. header prescan done)
            //         since it don't support A or V only,
            //         progress bar will continue go w/ this message
#ifndef  NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
            if(__bMMStage == MM_STAGE_PLAYEND)
            {
                if(__bVideoID == HAL_THROW_VIDEO)
                {
                    strcpy(__bMsgString, "Not supported video format");
                    OSD_Output(MSG_STRING, 0, 10);
					
                    // LLY2.50, keep the starting time to show "un-support format OSD"
                    __dwPreGetTime=OS_GetSysTimer();
					
                    __bUnknownFormat=TRUE;
					
                }
                // LLY2.36, must check if exist audio stream first
                if(__bASTNs>=1 && __bASTID == HAL_THROW_AUDIO)
                {
                    strcpy(__bMsgString, "Not supported audio format");
                    OSD_Output(MSG_STRING, 0, 10);
					
                    // LLY2.50, keep the starting time to show "un-support format OSD"
                    __dwPreGetTime=OS_GetSysTimer();
					
                    __bUnknownFormat=TRUE;
                }
            }
#endif  // #ifdef  NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
            break;
			
        case    MM_STAGE_PLAYEND:
            // Enable Playback ready flag
            __bPBReady=TRUE;
			
            // LLY1.20, set motion stage as playback
            __bMotionState = MOTION_STATE_PLAY;
			
            // LLY1.20, continue to process char-base SPST if necessary
#ifdef SUPPORT_CHAR_SUBPICTURE
            if ((__bAttrPlay == ATTR_AVI))
            {
                CHAR_SP_Trigger();
            }
#endif  // #ifdef SUPPORT_CHAR_SUBPICTURE
			
            if (__btPlayEnd == TRUE)
            {
#ifdef  SUPPORT_PSNR_TEST
                HAL_ReadInfo(HAL_INFO_VIDEO_REMAIN, &_dwMMTemp);
                if(__dwDecodeCnt>=0x4B0)
                {
                    __bMMStage=MM_STAGE_NEXT_ITEM;
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"Decode all frame complete !\n");
                }
#else   // #ifdef SUPPORT_PSNR_TEST
                // wyc0.75, make time-out to 15s because our audio buffer can play about 10s when buffer full. So wait 15s for
                // proc2 to decode these data when parser sent bits completed. Also wait audio remainder to 1000 because minimun
                // audio remainder is about 700 in CT909 now.
                if ((OS_GetSysTimer() - __dwBuffEmptyTime) < (_dwMMTriggerEmptyTime))
                {
                    DWORD   dwARem;
					
                    // wyc0.85, use video ramainder to judge if need to change files in CDROM-motion mode.
                    HAL_ReadInfo(HAL_INFO_VIDEO_REMAIN, &_dwMMTemp);
                    HAL_ReadInfo(HAL_INFO_AUDIO_REMAIN, &_dwMMTemp1);
					
                    // LLY1.21a, Choose differnt Audio Remain threshold for WMA
                    // Since, it depends on Block Align
                    if(__bAudioType == HAL_AUDIO_WMA)
                    {
                        HAL_ReadAM(HAL_AM_WMA_BLOCK_ALIGN, &__dwTemp); // *8 = BYTE unit
                        dwARem=__dwTemp*8/4;
                    }
                    else
                    {
                        dwARem=1024;
                    }
									
                    // wyc0.90, when play motion files, one of Audio or Video not empty, don't go next file.
                    // Because some file only have audio or video. and some files audio is longer than video.
                    // LLY2.30, only waiting audio buffer empty while normal play mode.
                    // Since, DSP will base on STC to skip audio stream during Fast mode
                    // If Fast mode to file end, video will empty, and STC stop update
                    // So, audio stop the data skip, then, we can't wait audio buffer empty.
                    if ((_dwMMTemp > 32) || ((__bModeCmd == KEY_PLAY) && (_dwMMTemp1 > dwARem)))
                    {
                        OS_YieldThread();
                    }
                    else
                    {
                        __bMMStage=MM_STAGE_NEXT_ITEM;
#ifdef MM_MOTION_DEBUG
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"A/V buffer empty\n");
#endif //MM_MOTION_DEBUG
                    }
                }
                else
                {
                    __bMMStage=MM_STAGE_NEXT_ITEM;
#ifdef MM_MOTION_DEBUG
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"Time-out\n");
#endif //MM_MOTION_DEBUG
                }
#endif //SUPPORT_PSNR_TEST
            }
            else
            {
                if(PARSER_QueryEvent(0,PARSER_EVENT_COMPLETED, TRUE))
                {
#ifdef MM_MOTION_DEBUG
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"Parser report ending\n");
#endif //MM_MOTION_DEBUG
                    __btPlayEnd = TRUE;
                    __dwBuffEmptyTime = OS_GetSysTimer();
                }
                // LLY1.10 begin ...
                // Add AVI error concealment mechanism while parser report error during playback
                else if( (__bAttrPlay==ATTR_AVI) && PARSER_QueryEvent(0, PARSER_EVENT_MOVIE_ERROR, TRUE) )
                {
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"AVI Err\n");
                    __dwBuffEmptyTime = OS_GetSysTimer();
                    __bMMStage=MM_STAGE_ERROR_CONCEAL;
                }
                OS_YieldThread();
            }
            break;
			
			// LLY1.10, add new case for Error Concealment mechanism
        case    MM_STAGE_ERROR_CONCEAL:
            {
                extern WORD __wScanGap;
                extern DWORD __dwScanBaseTime;
                WORD    wTemp;
				
                // wait buffer empty then start the error concealment mechanism
                if ((OS_GetSysTimer() - __dwBuffEmptyTime) < (_dwMMTriggerEmptyTime))
                {
                    // wyc0.85, use video ramainder to judge if need to change files in CDROM-motion mode.
                    HAL_ReadInfo(HAL_INFO_VIDEO_REMAIN, &_dwMMTemp);
                    HAL_ReadInfo(HAL_INFO_AUDIO_REMAIN, &_dwMMTemp1);
					
                    // wyc0.90, when play motion files, one of Audio or Video not empty, don't go next file. Because some file only have audio or video.
                    // some files audio is longer than video.
                    // 8 is computed by 4*2, 4 is divide to DWORD. 2 is 1/2 video remainder.
                    if ((_dwMMTemp > 32) || (_dwMMTemp1 > 1024))
                    {
                        OS_YieldThread();
                        break;
                    }
                    else
                    {
#ifdef MM_MOTION_DEBUG
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"E_C: A/V buffer empty\n");
#endif //
                    }
                }
                else
                {
#ifdef MM_MOTION_DEBUG
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"E_C: Time-out\n");
#endif //
                }
				
#ifdef MM_MOTION_DEBUG
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"E_C: Do Error Conceal for AVI\n");
#endif //
                // Freeze STC value before decoder stop
                // Otherwise, OSD will become zero.
                HAL_ReadInfo(HAL_INFO_STC, &__dwSTCValue);
                DISP_STCFreeze(__dwSTCValue, DISP_WAIT_NEXT_STC);
				
                // Stop source/ parser/ decoder
                HAL_PlayCommand(COMMAND_STOP, NULL_0);
                SrcFilter_Stop(__dwSFStreamID);
                HAL_ControlParser(HAL_PARSER_STOP, 0);
				
                // Reset A/V buffer before new data sending
                HAL_Reset(HAL_RESET_AVSPST);
				
                if(__bModeCmd==KEY_SCF || __bModeCmd==KEY_SCB)
                {
                    __dwTimeNow=UTL_JumpGroove(__bModeCmd, 0);
                }
                // Find next jump position just like scan forward mode (while non scan mode)
                // Notice: Must set value within __wScanGap & __dwScanBaseTime first
                //         since UTL_JumpGroove() will reference it.
                //         And, restore __wScanGap after UTL_JumpGroove() calling
                //         __dwScanBaseTime is un-necessary to restore since new value will be set within it
                else
                {
                    __wScanGap=2; // jump 2 sec.
                    // __dwScanBaseTime is msec format, and parser report video time in sec format
                    __dwScanBaseTime=PARSER_GetContainerCurVideoTime();
                    __dwTimeNow=UTL_JumpGroove(KEY_SCF, 0);
                }
				
                // if next jump position is out of current file range
                if ( __bModeCmd == KEY_SCB )
                {
                    if (__dwTimeNow <= __dwTimeBegin)    //SCB reach track begin
                    {
                        // LLY.101-2, must same as PBC on mode's flow while enable following define
                        // otherwise, it will issue KEY_PREV action
                        // ** TCH1.55-1; begin... FORCE_CHANGE_TRACK_NORMAL_PLAY can do it simpler.
#ifdef FORCE_CHANGE_TRACK_NORMAL_PLAY
                        wTemp= __wPlayItem;
                        // reset the play state
                        //Kevin1.07, fix "In program playing, SCB to track begin will clear program mode"
                        //          CC_DoKeyAction (KEY_STOP) will clear program mode
                        //          SCF to track end also calls CC_KeyCommand(KEY_STOP)
                        CC_KeyCommand ( KEY_STOP );
						
                        __wPlayItem= wTemp;
                        // next will normal play from the frist track
                        CC_DoKeyAction ( KEY_PLAY );
#else //FORCE_CHANGE_TRACK_NORMAL_PLAY
						
                        // SCANBACK to first track, system will become normal play
                        if ( __wPlayItem == __wTrackBegin )
                        {
                            // reset the play state
                            //Kevin1.07, fix "In program playing, SCB to track begin will clear program mode"
                            //          CC_DoKeyAction (KEY_STOP) will clear program mode
                            //          SCF to track end also calls CC_KeyCommand(KEY_STOP)
                            //CC_DoKeyAction ( KEY_STOP );
                            CC_KeyCommand ( KEY_STOP );
							
                            // next will normal play from the frist track
                            CC_DoKeyAction ( KEY_PLAY );
                        }
                        else    // will Scan back previous track
                        {
                            // must check repeat mode then decide to play next
                            // modify for use by LINEAR_ChangeTrack call
                            LINEAR_ChangeTrack(KEY_PREV, (BYTE)NULL);
                        }   // NOT BeginTrack
#endif //FORCE_CHANGE_TRACK_NORMAL_PLAY
                        return;
                    }   // SCB reach to the Begin of the range
                }   // KEY_SCB
                else    // Non SCAN backword mode
                {
                    if ( (__dwTimeNow+ __wScanGap) >= __dwTimeEnd )
                    {
#ifdef MM_MOTION_DEBUG
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"E_C: Change to next track\n");
#endif //MM_MOTION_DEBUG
                        // must check repeat mode then decide to play next
                        // modify for use by LINEAR_ChangeTrack call
                        LINEAR_ChangeTrack(KEY_NEXT, (BYTE)NULL);
                        return;
                    }
                }
				
#ifdef MM_MOTION_DEBUG
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR,"E_C: Jump to next position\n");
#endif //MM_MOTION_DEBUG
                // Start to playback the desired range
                // LLY2.35, give start and end time for UTL_PlayFromTime() since the API is updated
                UTL_PlayFromTime(__dwTimeNow, __dwTimeEnd);
				
                // Issue playback command by calling CC_KeyCommand()
                // Notice: must set some flags first before calling CC_KeyCommand()
                // 1. __btPause=TRUE, Otherwise, NPF can't work
                // 2. __bScan=SCAN_NONE, Otherwise, Scan mode can't work
                __btPause=TRUE;
                __bScan=SCAN_NONE;
                CC_KeyCommand(__bModeCmd);
				
                // Resotre the stage to polling playend after error concealment done
                __bMMStage=MM_STAGE_PLAYEND;
            }
            break;
			
			
			// LLY1.02, add new case to handle skip current item issue
			// Before time-out,
			// If no input key, continue to loop in this stage.
			// Otherwise, to the input key directly.
			// Used to fix KEY_PREV no action while pressing previous key during 3th file
			// (1st file : playable; 2nd file: not support; 3th file: playable)
			// ie. twice KEY_PREV immediate can previous from 3th file to 1st file
        case    MM_STAGE_SKIP_ITEM:
            // LLY2.50, enlarge the time-out from 500msec to 1 sec to let user can see error message
            if( (OS_GetSysTimer()-__dwPreGetTime) > COUNT_1_SEC )
            {				
                __bMMStage = MM_STAGE_NEXT_ITEM;
            }
            break;
			
        case    MM_STAGE_NEXT_ITEM:
#ifndef  NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
            // LLY2.50, change stage to skip item let it handle time-out mechanism
            if(__bUnknownFormat)
            {
                __bUnknownFormat = FALSE;
                __bMMStage = MM_STAGE_SKIP_ITEM;
                break;
            }
#endif  // #ifdef  NO_OUTPUT_INFO_FOR_UNKNOWN_AV_FORMAT
			
            __btPlaying=FALSE;
			
            // LLY1.02, reset the re-try counter as zero
            __bResetRecover=0;
			
			// LLY0.85, must stop after playback to file end for PSNR testing.
#if defined (SUPPORT_PSNR_TEST) || defined (SUPPORT_DIVX_CERTIFICATION)
            // LLY1.05, repeat mode or program playing mode, let it go next item path
            // Otherwise, auto stop current item.
            if(__bRepeat!=REPEAT_NONE || __btPlayingProgram)
            {
                __bFWKey=KEY_NEXT;
            }
            else
            {
                __bFWKey=KEY_STOP;
            }
#else   // #ifdef SUPPORT_PSNR_TEST
            __bFWKey=KEY_NEXT;
#endif  // #ifdef SUPPORT_PSNR_TEST
			
            // Transfer the stage to NONE, otherwise, it always trap into NEXT_ITEM stage
            __bMMStage = MM_STAGE_NONE;
			
#ifdef SUPPORT_MP4
            {
                // LLY1.50, reset chapter information before change to next item
                // Since, the correct value only be updated while prescan action successfully.
                // So, it maybe wrong while playback one file w/ __dwChater_Ns_Title != 0 first
                // Then, playback a un-support file ==> KEY_NEXT will let it to find next chapter (MP4 only)
                extern WORD __wChapter_Ns_Title;
                extern WORD __wChapter;
                __wChapter_Ns_Title=0;
                __wChapter=1;
            }
#endif //SUPPORT_MP4
            break;
			
        default:
            break;
    }
}

void    MM_AudioTrigger(void)
{
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    BYTE bAudioType;
    DWORD dwTemp, dwTemp1;
	WORD wBackupPlayingItemDir;
	WORD wBackupPlayingItemIndex;
#endif //SUPPORT_JPEG_AUDIO_PLAYING
    BYTE bMMStage;
    static DWORD dwTimeStamp; 

#ifdef SUPPORT_PHOTO_BROWSE_MODE
    if (__bMMPhotoBrowseMode)
    {
        return;
    }
#endif //SUPPORT_PHOTO_BROWSE_MODE

#ifdef IMAGE_FRAME_SETUP 
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    if (!__bMMJPEGAudioMode)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
    {
        __bMMFirstJPEGPlaying = TRUE;
    }
#endif //IMAGE_FRAME_SETUP

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    if (__bMMJPEGAudioMode)
    {
		if (__bMMSkipAudio)
			return;

        bMMStage = __bMMAudioStage;
    }
    else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
    {
        bMMStage = __bMMStage;
    }

    switch (bMMStage)
    {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    case    MM_STAGE_AUDIO_STAGE_INIT:
        __bMMAudioPlayEnd=FALSE;
        
        // Get desired play item range
        UTL_GetPlayRange_BGItem(__wBGPlayItem, &dwTemp, &dwTemp1);
        
        // Get desired playitem audio mode
        bAudioType = UTL_CheckBGPlayItemMode(__wBGPlayItem);
        
        // Configurate audio only playback mode, and issue command
        if(bAudioType==ATTR_WMA)
        {
            UTL_PlayAudio(HAL_AUDIO_WMA);
        }
        else if (bAudioType==ATTR_MP2)
        {
            UTL_PlayAudio(HAL_AUDIO_MP2);
        }
        else
        {
            UTL_PlayAudio(HAL_AUDIO_MP3);
        }
        
        // Call another API to send the bitstream of background item
        UTL_PlayFromTime_BGItem(dwTemp, dwTemp1);
        
        // Change stage to play
        __bMMAudioStage = MM_STAGE_AUDIO_STAGE_PLAY;
        break;
        
    case    MM_STAGE_AUDIO_STAGE_PLAY:
        if(__bCheckAudioType & CHECK_AUDIO_FREQ)
        {
            // Check if audio frequency is ready.
            if(HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &dwTemp))
            {
                // If ready, base on it to set audio DAC
                HAL_SetAudioDAC((BYTE)dwTemp);
                
                // Continue audio decoder procedure
                HAL_WriteAM(HAL_AM_START_DECODE, 1);
                
                // Clear the check audio frequency flag
                __bCheckAudioType &= ~CHECK_AUDIO_FREQ;
            }
        }
        else
        {
            // Change stage to polling playend
            __bMMAudioStage = MM_STAGE_PREPAREINFO;
        }
        break;
#endif //#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        case    MM_STAGE_PREPAREINFO:
#ifdef DOLBY_CERTIFICATION 
            if (__bAttrPlay==ATTR_AC3)
                bShowDolbySurrFlag=TRUE;
            else
                bShowDolbySurrFlag=FALSE;
#endif
            // wyc1.01-909, modify the audio time out to LSF mode, because some LSF file need more time to wait audio buffer empty to play completed file.
            // 32 is using LSF 32K to compute, the computed time mul 1000 is the unit of timer is ms. So need to transfer second to ms.
            _dwMMTriggerEmptyTime = ((((DS_AD0BUF_END_MM-DS_AD0BUF_ST_MM)*8)/(32*1000))+5)*COUNT_1_SEC;
            _bMMAudioDisplayInfo = FALSE; 

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (!__bMMJPEGAudioMode)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
            {
                __bLOGO = LOGO_NONE;
            }
            
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (!__bMMJPEGAudioMode)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
            {
                __dwMMTriggerMode = MM_AUDIO_TRIGGER_MODE;
            }

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {
                __bMMAudioStage = MM_STAGE_PLAYEND;
            }
            else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
            {
                __bMMStage = MM_STAGE_PLAYEND;
            }

            // This is used for wait 1s more after audio remainder alomst empty.
            __dwMMAudioPreGetTime = 0xFFFFFFFF;

#ifdef SUPPORT_WMA
            dwSample = 0xFFFFFFFF;
            dwPreSample = 0xFFFFFFFF;
#endif //
            // wyc2.35-909S, add code to support disable program list mode when playing mode.
#ifndef NO_MM_BUTTON
            if (__bMMMenuID == MM_MENU_EDITPROG)
            {
                OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_PROG_LIST);
            }
#endif //
            // LLY1.05-1, keep desired playing item & directory while not same item
            // Notice: can set it after playback CMD done.

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (!__bMMJPEGAudioMode)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
            {
                if(!__bSameItem)
                {
                    __wPrevPlayItem=__wPlayItem;
                    __wPrevPlayDir=__MMPlayingItem.wDirID;
                }
            }
            break;

        case    MM_STAGE_PLAYEND:
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if ((__bMMJPEGAudioMode && __bMMAudioPlayEnd) || (!__bMMJPEGAudioMode && (__btPlayEnd == TRUE)))
#else                
            if (__btPlayEnd == TRUE)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
            {
                // wyc0.75, make time-out to 15s because our audio buffer can play about 10s when buffer full. So wait 15s for
                // proc2 to decode these data when parser sent bits completed. Also wait audio remainder to 1000 because minimun
                // audio remainder is about 700 in CT909 now.
                if ((OS_GetSysTimer() - __dwBuffEmptyTime) < _dwMMTriggerEmptyTime)
                {

#ifdef SUPPORT_WMA
                    if(__bAttrPlay==ATTR_WMA) 
                    {
                        // wyc2.77-909p, patch the Joey's code.
                        HAL_ReadInfo(HAL_INFO_AUDIO1_REMAIN, &_dwMMTemp);
                        HAL_ReadAM(HAL_AM_WMA_BLOCK_ALIGN, &_dwMMTemp1);
                        HAL_ReadAM(HAL_AM_FRAME_SAMPLE_SEC_CNT, &dwSample);

                        //kevin1.07, fix WMA low bitrate play ~sec and then jump to next file
                        if ((_dwMMTemp < _dwMMTemp1+64) && (dwSample==dwPreSample) && (dwSample!=0))
                        {
                            if (dwTimeStamp == 0xffffffff)
                            {
                                __dwMMAudioPreGetTime = OS_GetSysTimer();
                                dwTimeStamp = __dwMMAudioPreGetTime;
                            }
                        }
                        else
                        {
                            dwPreSample = dwSample; 
                            OS_YieldThread();
                        }
                    }
                    else //MP3/MP2
#endif //#ifdef SUPPORT_WMA
                    {
                        HAL_ReadInfo(HAL_INFO_AUDIO_REMAIN, &_dwMMTemp);
                        // Choose 3000 is because MP3 audio remainder maybe remain 1200 DWORDs. So increase it to avoid not time out.
                        // wyc1.05-909, use __dwPreGetTime to delay 1s after audio almost empty and can let user have time to go prev file
                        // when current file is un-playable.
#ifndef DOLBY_CERTIFICATION
                        if ((_dwMMTemp < 2000) && (__dwMMAudioPreGetTime == 0xFFFFFFFF)) //kevin1.20, avoid MP3 low bit rate file from proceeding to next file too early
#else
                        if ((_dwMMTemp < 1000) && (__dwMMAudioPreGetTime == 0xFFFFFFFF)) //kevin1.20, avoid playing .AC3 file from proceeding to next file too early
#endif //DOLBY_CERTIFICATION
                        {
                            __dwMMAudioPreGetTime = OS_GetSysTimer();
                        }
                        else
                        {
                            OS_YieldThread();
                        }
                    }
                    if (((OS_GetSysTimer() - __dwMMAudioPreGetTime) >= COUNT_1_SEC) && (__dwMMAudioPreGetTime != 0xFFFFFFFF))
                    {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
                        if (__bMMJPEGAudioMode)
                        {
                            __bMMAudioStage=MM_STAGE_NEXT_ITEM;
                        }
                        else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
                        {
                            __bMMStage=MM_STAGE_NEXT_ITEM;
                        }
                    }
                }
                else
                {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
                        if (__bMMJPEGAudioMode)
                        {
                            __bMMAudioStage=MM_STAGE_NEXT_ITEM;
                        }
                        else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
                        {

                            __bMMStage=MM_STAGE_NEXT_ITEM;
                        }
                }
            }
            else
            {
#ifdef DOLBY_CERTIFICATION 
                if (bShowDolbySurrFlag)
                {
                 	if( HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &_dwMMTemp)) //kevin1.05, check already decoding then shows surround status
                 	{
	                    HAL_ReadAM(HAL_AM_DOLBY_SURR_FLAG, &_dwMMTemp);
	                    switch (_dwMMTemp)
	                    {
	                    case 0:
	                        strcpy(__bMsgString, "not ind.");
	                        bShowDolbySurrFlag = false;
	                        OSD_Output(MSG_STRING, NULL_0, 0x5);
	                        break;
	                    case 1:
	                        strcpy(__bMsgString, "not surr");
	                        bShowDolbySurrFlag = false;
	                        OSD_Output(MSG_STRING, NULL_0, 0x5);
	                        break;
	                    case 2:
	                        strcpy(__bMsgString, "surr");
	                        bShowDolbySurrFlag = false;
	                        OSD_Output(MSG_STRING, NULL_0, 0x5);
	                        break;
	                    case 3: //the default value after audio reset
	                        break;
	                    }
			        }
                }
#endif //DOLBY_CERTIFICATION

#ifdef SUPPORT_JPEG_AUDIO_PLAYING                
                if (!__bMMJPEGAudioMode)
#endif //SUPPORT_JPEG_AUDIO_PLAYING
                {
                    if (!_bMMAudioDisplayInfo)
                    {
                        if (!(__bCheckAudioType & CHECK_AUDIO_FREQ))
                        {
                            _bMMAudioDisplayInfo = TRUE;
                            if (__bAttrPlay == ATTR_MP3)
                                OSDMM_Update(OSDMM_MSG_UPDATE_HELP, OSDMM_HELP_TYPE_MP3);
                            else if (__bAttrPlay == ATTR_MP2)
                                OSDMM_Update(OSDMM_MSG_UPDATE_HELP, OSDMM_HELP_TYPE_MP2);
#ifdef SUPPORT_WMA
                            else if (__bAttrPlay == ATTR_WMA)
                                OSDMM_Update(OSDMM_MSG_UPDATE_HELP, OSDMM_HELP_TYPE_WMA);
#endif //SUPPORT_WMA
                        }
                    }
                    
                }
                
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
                if (__bMMJPEGAudioMode)
                {
#ifdef SUPPORT_WMA
					bAudioType = UTL_CheckBGPlayItemMode(__wBGPlayItem);
					
					// Configurate audio only playback mode, and issue command
					if(bAudioType==ATTR_WMA)
					{
						HAL_ReadAM(HAL_AM_WMA_L1_DECODE, &__dwTemp);
						if (__dwTemp == 1) //Can't decode
						{
							wBackupPlayingItemDir = __MMBackgroundPlayingItem.wDirID;
							wBackupPlayingItemIndex = __MMBackgroundPlayingItem.wIndex;

							_MM_GetNextPlayAudioFile();
							if ((__MMBackgroundPlayingItem.wDirID == wBackupPlayingItemDir) && (__MMBackgroundPlayingItem.wIndex == wBackupPlayingItemIndex))
							{
								//Only 1 WMA file and the file can't be decoded.
								UTL_StopAudio();
								__bMMSkipAudio = TRUE;					

								UTL_StreamContinue(0);								
								__bMMAudioStreamPause = TRUE;
								__bMMVideoStreamPause = FALSE;
								__bMMStartJPEGAudioTrigger = FALSE;									
							}
						}
					}
#endif //SUPPORT_WMA

                    if (PARSER_QueryEvent(BG_ITEM_STREAM_ID, PARSER_EVENT_COMPLETED, TRUE)) //The stream id of the background item is 1.
                    {
                        __bMMAudioPlayEnd = TRUE;
                        dwTimeStamp = 0xffffffff; 
                        __dwBuffEmptyTime = OS_GetSysTimer();
                    }                                        
                }
                else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
                {
                    if (PARSER_QueryEvent(0,PARSER_EVENT_COMPLETED, TRUE))
                    {
                        __btPlayEnd = TRUE;
                        dwTimeStamp = 0xffffffff; 
                        __dwBuffEmptyTime = OS_GetSysTimer();
                    }
                }
                OS_YieldThread();
            }
            break;
        case    MM_STAGE_NEXT_ITEM:
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {
                __bMMAudioStage = MM_STAGE_AUDIO_STAGE_INIT;
                __bMMAudioPlayEnd = FALSE;  
                
                // LLY2.53, stop audio decoder playback related procedure.
                UTL_StopAudio();

                _MM_GetNextPlayAudioFile();
            }
            else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
            {
                __btPlaying=FALSE;
                __bFWKey=KEY_NEXT;
            }
            break;
        default:
            break;
    }

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    if (__bMMJPEGAudioMode)
    {
        _MM_SourceControl();
    }
#endif //SUPPORT_JPEG_AUDIO_PLAYING
}

void    _MM_ImageTrigger(void)
{
    WORD wJPEGAbortTime;
	
#ifdef SUPPORT_AUTO_ROTATION
    _MM_DetectAutoRotate();
#endif //SUPPORT_AUTO_ROTATION
	
    switch(__bMMStage)
    {
	case    MM_STAGE_PREPAREINFO:
		__bLOGO = LOGO_JPEG;
		if (__dwMMTriggerMode != MM_IMAGE_TRIGGER_MODE)
		{
#ifdef JPG_SINGLE_FRAME_BUFFER
			__HALJPEGSetting.bFrameBuffer = 1;
#endif //JPG_SINGLE_FRAME_BUFFER
			
			__dwMMTriggerMode = MM_IMAGE_TRIGGER_MODE;
			__dwMMJPGDispOK = TRUE;
		}
		
		// Record time for trigger audio error concealment when playing abnormal.
		__dwPreGetTime=OS_GetSysTimer();
		__dwMMJPGBuffEmptyTime = 0xFFFFFFFF;
		__bMMrogressCnt = 0;
		
		if (__dwMMJPEGPreview)
		{
			__bHALJPEGEffect=HALJPEG_EFFECT_NONE;
#ifndef IMAGE_FRAME_SETUP 
			__bMMEffectIdx = HALJPEG_EFFECT_NONE;
#endif //#ifndef IMAGE_FRAME_SETUP
			
			if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
			{
				__HALJPEGSetting.wDisplayStartH = MM_JPEG_PREVIEW_NTSC_START_H;
				__HALJPEGSetting.wDisplayStartV = MM_JPEG_PREVIEW_NTSC_START_V;
				__HALJPEGSetting.wDisplayWidth = MM_JPEG_PREVIEW_NTSC_WIDTH;
				__HALJPEGSetting.wDisplayHeight = MM_JPEG_PREVIEW_NTSC_HEIGHT;
			}
			else //PAL
			{
				__HALJPEGSetting.wDisplayStartH = MM_JPEG_PREVIEW_PAL_START_H;
				__HALJPEGSetting.wDisplayStartV = MM_JPEG_PREVIEW_PAL_START_V;
				__HALJPEGSetting.wDisplayWidth = MM_JPEG_PREVIEW_PAL_WIDTH;
				__HALJPEGSetting.wDisplayHeight = MM_JPEG_PREVIEW_PAL_HEIGHT;
			}
		}
		else
		{
			__HALJPEGSetting.wDisplayStartH = MM_JPEG_FRAME_START_H;
			__HALJPEGSetting.wDisplayStartV = MM_JPEG_FRAME_START_V;
			__HALJPEGSetting.wDisplayWidth = MM_JPEG_FRAME_WIDTH;
			__HALJPEGSetting.wDisplayHeight = MM_JPEG_FRAME_HEIGHT;
			
			MM_CompleteJPGEffect(FALSE);
#ifdef JPG_SINGLE_FRAME_BUFFER
			// wyc2.30-909s, only do the copy when decode & display OK. Otherwise the frame 2 is not ready and will see garbage image.
			if ((__bHALJPEGEffect!=HALJPEG_EFFECT_NONE) && (__dwMMJPGDispOK))
			{
#ifdef IMAGE_FRAME_SETUP 
				if (!__bMMFirstJPEGPlaying)
#endif //IMAGE_FRAME_SETUP
				{
					HALJPEG_CopyPictureToDisplayBuffer(JPEG_EFFECT_DISP_BUFFER_2_TO_0);
					__HALJPEGSetting.bFrameBuffer = 0;
					__bHALJPEGEffect = HALJPEG_EFFECT_NONE;
					HALJPEG_Display(__HALJPEGSetting.bFrameBuffer);
					OS_DelayTime(COUNT_50_MSEC);
				}
			}
#endif //JPG_SINGLE_FRAME_BUFFER
#ifdef IMAGE_FRAME_SETUP 
			if (!__bMMFirstJPEGPlaying)
			{
				__bHALJPEGEffect= __bMMEffectIdx; 
			}
			else
			{
				__bHALJPEGEffect= HALJPEG_EFFECT_NONE; //The first JPEG can't do effect.
			}
#else
			__bHALJPEGEffect= __bMMEffectIdx; //change effect only when dsiplay is OK.
#endif //IMAGE_FRAME_SETUP
			DISP_ShowEffect(SLIDESHOW_RESET, 0, __HALJPEGSetting.bFrameBuffer); //Reset the Display's state for effect.
			// wyc2.38-909s, clear it when go different play item.
			if (__bZoomFactor != VIEW_NORMAL)
			{
				__bZoomFactor = VIEW_NORMAL;
			}
		}
		__HALJPEGSetting.bOperation = HALJPEG_OPERATION_I;
		__HALJPEGSetting.dwBackgroundColor = HALJPEG_SLIDESHOW_BACKGROUND_COLOR;
		
#ifdef JPG_SINGLE_FRAME_BUFFER
		if (__dwMMJPEGPreview== TRUE)
		{
			__HALJPEGSetting.bFrameBuffer = 1;
		}
		else
		{
			// wyc2.21a-909s, when play from preview to slide show mode, need to clear frame 0 and display frame 0 to avoid to avoid seeing garbage image.
			if (__HALJPEGSetting.bFrameBuffer == 1)
			{
				HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_FRAME0);
				__HALJPEGSetting.bFrameBuffer = 0;
			}
			// wyc2.21-909S, tell JPG decoder to do effect mode.
			if (__bHALJPEGEffect != HALJPEG_EFFECT_NONE)
			{
#ifdef IMAGE_FRAME_SETUP 
				if (__bMMFirstJPEGPlaying)
				{        
					__bHALJPEGEffect = HALJPEG_EFFECT_NONE;
					HALJPEG_EffectConfigure(JPEG_EFFECT_NONE);
					__HALJPEGSetting.bFrameBuffer = 0;
				}
				else
#endif //IMAGE_FRAME_SETUP
				{                    
					HALJPEG_EffectConfigure(JPEG_EFFECT_TRUE);
					__HALJPEGSetting.bFrameBuffer = 2;
				}
			}
			else
			{
				HALJPEG_EffectConfigure(JPEG_EFFECT_NONE);
				__HALJPEGSetting.bFrameBuffer = 0;
			}
		}
#else //JPG_SINGLE_FRAME_BUFFER
		if ((__dwMMJPGDispOK == TRUE) && (__dwMMJPEGPreview == FALSE))
		{
			if (__wHALJPEGCurrentDisplayBuf)	
			{
				__HALJPEGSetting.bFrameBuffer = 0;
			}
			else
			{
				__HALJPEGSetting.bFrameBuffer = 1;
			}
		}
#endif //JPG_SINGLE_FRAME_BUFFER
		// wyc1.06-909, when display is not OK in preview mode, need to init __bMMCountStopBak to 2, othwewise __bCountStop will keep in
		// 0(because _MM_ImageTrigger don't clear it in stage MM_STAGE_NEXT_ITEM) and it mean system will think it is play mode. Then KEY_RESUME
		// will have problem because system think it is play mode.
		if ((__dwMMJPGDispOK == FALSE) && (__dwMMJPEGPreview == TRUE))
		{
			__bMMCountStopBak = 2;
		}
		__dwMMJPGDispOK = FALSE;
		
		HALJPEG_Reset();
		
		// wyc1.02a-909, need to move it outside if statement when JPG decoder won't destroy the frame buffer.
		MM_AccessJPGStatus(MM_GET_JPG_STATUS, __wPlayItem);
		__HALJPEGSetting.bOperation = __bMMJPGStatus;
		
#ifdef SUPPORT_AUTO_ROTATION
		if (__bMMPhotoAutoRoate == TRUE)
		{
#if (ROTATE_DIRECTION == KEY_LEFT)
			__HALJPEGSetting.bOperation = HALJPEG_TransformOperation(__bMMJPGStatus, HALJPEG_OPERATION_RCC);
#elif (ROTATE_DIRECTION == KEY_RIGHT)
			__HALJPEGSetting.bOperation = HALJPEG_TransformOperation(__bMMJPGStatus, HALJPEG_OPERATION_RC);
#endif //ROTATE_DIRECTION
		}
		
		_bMMPhotoAutoRoateOperation = __HALJPEGSetting.bOperation;
#endif //SUPPORT_AUTO_ROTATION
		if (__dwMMJPEGPreview)
		{
			// wyc1.10-909, clear two buffer to fix JPG preview picture overlay in slide show picture when pressing KEY_MENU from slide show problem.
			// LLY2.21a, don't clear frame buffer 0 while single buffer mode
			// Since, JPEG preview only use frame buffer 1, and OSD will share frame buffer 0
#ifndef JPG_SINGLE_FRAME_BUFFER
			HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_FRAME0);
#endif //JPG_SINGLE_FRAME_BUFFER
			HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_FRAME1);
			if (__bMMJPGDecEncMode == true)
			{
				//Senshong2.75, config jpeg encoder picture size
				__HALJPEGSetting.wDisplayStartH = MM_JPEG_FRAME_START_H;
				__HALJPEGSetting.wDisplayStartV = MM_JPEG_FRAME_START_V;
				__HALJPEGSetting.wDisplayWidth = MM_JPEG_FRAME_WIDTH;
				__HALJPEGSetting.wDisplayHeight = MM_JPEG_FRAME_HEIGHT;
				HALJPEG_ParseHeader(JPEG_PARSE_TYPE_THUMB_ENCODE, __HALJPEGSetting.bFrameBuffer);
			}
			else
				HALJPEG_ParseHeader(JPEG_PARSE_TYPE_THUMBNAIL, __HALJPEGSetting.bFrameBuffer);
		}
		else
		{
			HALJPEG_ParseHeader(JPEG_PARSE_TYPE_NORMAL, __HALJPEGSetting.bFrameBuffer);
		}
		
		__bMMStage=MM_STAGE_PARSE_HEADER;
		
		// LLY1.05-1, keep desired playing item & directory while not same item
		// Notice: can set it after playback CMD done.
		if(!__bSameItem)
		{
			__wPrevPlayItem=__wPlayItem;
			__wPrevPlayDir=__MMPlayingItem.wDirID;
		}
		break;
		
        case    MM_STAGE_PARSE_HEADER:
            _bMMPlayable = HALJPEG_Status (JPEG_PARSE_HEADER);
            // wyc1.06-909, increase to 6s to avoid servo seek too much time to make JPG parse header OK after time out.
            if ((OS_GetSysTimer() - __dwPreGetTime) <= (COUNT_3_SEC*2))
            {
                if (_bMMPlayable == JPEG_STATUS_OK)
                {
                    OSD_Output(MSG_JPEG_RESOLUTION, 0, 0xff);
                    OSD_Output(MSG_JPEG_DATE, 0, 0xff);
								
                    __bMMStage=MM_STAGE_DECODE;
                }
                else if(_bMMPlayable == JPEG_STATUS_UNSUPPORT)
                {
                    __dwPreGetTime = OS_GetSysTimer();
                    __bMMStage = MM_STAGE_POLLING_TIMEOUT;
                }
                else if (_bMMPlayable == JPEG_STATUS_FAIL)
                {
                    _MM_JPEGRedecode();
                }
            }
            else
            {
                _MM_JPEGChangeToNext();
            }
            break;
			
        case    MM_STAGE_DECODE:
            HALJPEG_Decode();
            __bMMrogressCnt++;
            if (((__bMMrogressCnt % MM_MSG_PROGRESS_DIV_NUM) == 1)
#ifdef SUPPORT_ENCODE_JPG_PICTURE
                && (__bMMJPGDecEncMode!=TRUE)
#endif //SUPPORT_ENCODE_JPG_PICTURE
                )
            {
#ifdef IMAGE_FRAME_SETUP 
#ifdef DONT_SHOW_JPEG_PROGRESS_BAR
                if (__dwMMJPEGPreview) 
#else
					if (__bMMFirstJPEGPlaying || __dwMMJPEGPreview)                
#endif //DONT_SHOW_JPEG_PROGRESS_BAR
#endif //IMAGE_FRAME_SETUP
					{
						OSD_Output(MSG_PROGRESS, 1, 1); 
					}
            }
			//winnie2.76, avoid jpg hang  while JPG decoder spend long time and can't wait for parser completed.
			//record  the jpg decoder current and start time 
			_wMMTemp=__wProgressCurrent;
			__dwMMJPEGDisplayTime = OS_GetSysTimer();  
			
            __bMMStage = MM_STAGE_PLAYEND;
            break;
			
        case    MM_STAGE_PLAYEND:
            _bMMPlayable = HALJPEG_Status(HALJPEG_DECODE);
            __bMMrogressCnt++;
            if (((__bMMrogressCnt % MM_MSG_PROGRESS_DIV_NUM) == 1)
#ifdef SUPPORT_ENCODE_JPG_PICTURE
                && (__bMMJPGDecEncMode!=TRUE)
#endif //SUPPORT_ENCODE_JPG_PICTURE
                )
            {
#ifdef IMAGE_FRAME_SETUP 
#ifdef DONT_SHOW_JPEG_PROGRESS_BAR
                if (__dwMMJPEGPreview)
#else
					if (__bMMFirstJPEGPlaying || __dwMMJPEGPreview)
#endif //DONT_SHOW_JPEG_PROGRESS_BAR
#endif //IMAGE_FRAME_SETUP
						OSD_Output(MSG_PROGRESS, 0, 1); 
            }
            if(__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI)
            {
                __bMMStage=MM_STAGE_CHECK_DECODE_RESULT;
            }
            else
            {
                if ((PARSER_QueryEvent(0,PARSER_EVENT_COMPLETED, TRUE)) || (_bMMPlayable != JPEG_STATUS_UNFINISH))
                {
                    __bMMStage=MM_STAGE_CHECK_DECODE_RESULT;
                    __dwPreGetTime = OS_GetSysTimer();
                }
                else
                {
					//winnie2.76, avoid jpg hang  while JPG decoder spend long time and can't wait for parser completed.
					//if jpg decoder continue decode, the avoid hang time doesn't to count. else, needs to count jpeg decoder time
					//if jpeg decoder time >10 sec, jpg want to play next jpg
                    if(_wMMTemp!=__wProgressCurrent)    
                    {
                        __dwMMJPEGDisplayTime = OS_GetSysTimer();  
                        _wMMTemp=__wProgressCurrent;                      
                    }
                    else
                    {                        
                        if ((OS_GetSysTimer() - __dwMMJPEGDisplayTime) >COUNT_10_SEC) 
                        {
                            HALJPEG_Abort();
                            _MM_JPEGChangeToNext();                             
                        }                        
                    }
                }
                
            }
            break;
        case    MM_STAGE_CHECK_DECODE_RESULT:
            if ((OS_GetSysTimer() - __dwPreGetTime) <= COUNT_5_SEC)
            {
                _bMMPlayable = HALJPEG_Status(HALJPEG_DECODE);
				
                __bMMrogressCnt++;
                if (((__bMMrogressCnt % MM_MSG_PROGRESS_DIV_NUM) == 1)
#ifdef SUPPORT_ENCODE_JPG_PICTURE
					&& (__bMMJPGDecEncMode!=TRUE)
#endif //SUPPORT_ENCODE_JPG_PICTURE
					)
                {
#ifdef IMAGE_FRAME_SETUP 
#ifdef DONT_SHOW_JPEG_PROGRESS_BAR
					if (__dwMMJPEGPreview)
#else
						if (__bMMFirstJPEGPlaying || __dwMMJPEGPreview)
#endif //DONT_SHOW_JPEG_PROGRESS_BAR
#endif //IMAGE_FRAME_SETUP
							OSD_Output(MSG_PROGRESS, 0, 1); 
                }
				
                if (__dwMMJPGBuffEmptyTime != 0xFFFFFFFF)
                {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
                    if (__bMMJPEGAudioMode)
                    {
                        wJPEGAbortTime = COUNT_3_SEC;
                    }
                    else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
                    {
                        wJPEGAbortTime = COUNT_1_SEC;
                    }
					
                    if ((OS_GetSysTimer() - __dwMMJPGBuffEmptyTime) >= wJPEGAbortTime)
                    {
                        HALJPEG_Abort();
                        __dwMMJPGBuffEmptyTime = OS_GetSysTimer();
                        __bMMJPGDecEncMode = FALSE;
                    }
                }
                else
                {
                    HAL_ReadInfo(HAL_INFO_VIDEO_REMAIN, &_dwMMTemp);
                    if (_dwMMTemp < 0x20)
                        __dwMMJPGBuffEmptyTime = OS_GetSysTimer();
                }
                if (_bMMPlayable != JPEG_STATUS_OK)
                {
                    break;
                }
                else
                {
                    // wyc1.10a-909, move to here by CoCo's update to avoid this message to affect display.
#ifdef IMAGE_FRAME_SETUP 
#ifdef DONT_SHOW_JPEG_PROGRESS_BAR
					if (__dwMMJPEGPreview)
#else
						if (__bMMFirstJPEGPlaying || __dwMMJPEGPreview)
#endif //DONT_SHOW_JPEG_PROGRESS_BAR
#endif //IMAGE_FRAME_SETUP
#ifdef SUPPORT_ENCODE_JPG_PICTURE
							if (__bMMJPGDecEncMode!=TRUE)
#endif //SUPPORT_ENCODE_JPG_PICTURE
								OSD_Output(MSG_PROGRESS, 0, 1);
#ifdef SUPPORT_ENCODE_JPG_PICTURE
							__bMMJPGDecEncMode = FALSE;
#endif //SUPPORT_ENCODE_JPG_PICTURE
							
							__bMMStage = MM_STAGE_DISPLAY;
							
							if (__dwMMJPEGPreview)
							{
								strcpy(__cMMJPEGFMTExifDate, __cJPEGFMTExifDate);
								OSDMM_Update(OSDMM_MSG_UPDATE_HELP, OSDMM_HELP_TYPE_JPEG);
							}
                }
            }
            else
                _MM_JPEGChangeToNext();
            break;
        case    MM_STAGE_DISPLAY:
            __dwMMJPGDispOK = TRUE;
			__wProgressCurrent = __wProgressTotal; //xuli.0510

#ifndef DONT_SHOW_JPEG_PROGRESS_BAR
#ifdef SUPPORT_ENCODE_JPG_PICTURE  //xuli.0510
                if (__bMMJPGDecEncMode != TRUE)
#endif //SUPPORT_ENCODE_JPG_PICTURE
                    OSD_Output(MSG_PROGRESS, 0, 1);		
#endif //DONT_SHOW_JPEG_PROGRESS_BAR

            if (__dwMMZoomMode)
            {
                __dwMMZoomMode = (DWORD)__bHALJPEGEffect;
                __bHALJPEGEffect=HALJPEG_EFFECT_NONE;
                HALJPEG_Display(__HALJPEGSetting.bFrameBuffer);
                __bHALJPEGEffect = (BYTE)__dwMMZoomMode;
                __dwMMJPEGDisplayTime = OS_GetSysTimer();
                __bMMStage = MM_STAGE_POLLING_TIMEOUT;
                __dwMMZoomMode = FALSE;
            }
            else
            {
                if (HALJPEG_Display(__HALJPEGSetting.bFrameBuffer) != SLIDESHOW_RET_OK)
                    break;
                else
                {
                    // elmer2.78
                    __dwMMJPGCount++;

                    if (__dwMMJPEGPreview == TRUE)
                    {
                        __bMMrogressCnt = 0; //CoCo2.30, reset it to represent the decoding is OK.
                        __bMMStage = MM_STAGE_NEXT_ITEM;
                        // wyc2.00-909S, don't show preview picture when JPG decode preview OK in media select UI mode, because the background
                        // of media select UI is all black and it seemed strange to show JPG picture when preview picture decode OK in media select UI mode
                        if (__bMediaSelectUI == TRUE)
                            DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
                    }
                    else
                    {
                        // wyc2.21-909S, copy the data to correct frame buffer.
#ifdef JPG_SINGLE_FRAME_BUFFER
                        if (__bHALJPEGEffect != HALJPEG_EFFECT_NONE)
                        {
#ifdef IMAGE_FRAME_SETUP 
                            if (!__bMMFirstJPEGPlaying)
#endif //IMAGE_FRAME_SETUP
                            {
                                // wyc2.21a-909s, change the config because JPG decoder will do it.
                                HALJPEG_CopyPictureToDisplayBuffer(JPEG_EFFECT_DISP_BUFFER_2_TO_0);
                                __HALJPEGSetting.bFrameBuffer = 0;
                                __bHALJPEGEffect = HALJPEG_EFFECT_NONE;
                                HALJPEG_Display(__HALJPEGSetting.bFrameBuffer);
                            }
                        }
						
#endif //JPG_SINGLE_FRAME_BUFFER
						
#ifdef IMAGE_FRAME_SETUP
                        __bMMFirstJPEGPlaying = FALSE;
#endif //IMAGE_FRAME_SETUP
						
                        __dwMMJPEGDisplayTime = OS_GetSysTimer();
                        __bMMStage = MM_STAGE_POLLING_TIMEOUT;
                        
#ifdef SUPPORT_AUTO_ROTATION
                        if (_bMMPhotoAutoRoateOperation != __HALJPEGSetting.bOperation)
                        {
                            __HALJPEGSetting.bOperation = _bMMPhotoAutoRoateOperation;
                            HALJPEG_Operation();                            
                        }
#endif //SUPPORT_AUTO_ROTATION
                    }
                }
            }
            break;
        case    MM_STAGE_POLLING_TIMEOUT:
            if (__bZoomFactor != VIEW_NORMAL)
            {
                __dwMMJPEGDisplayTime = OS_GetSysTimer();
            }
            else if (__btPause)
            {
            }
            else if (_bMMPlayable == JPEG_STATUS_UNSUPPORT)
            {
                if ((OS_GetSysTimer() - __dwPreGetTime) < COUNT_500_MSEC)
                    break;
                else
                    _MM_JPEGChangeToNext();
            }
            else
            {
                if( (OS_GetSysTimer()-__dwMMJPEGDisplayTime) > (__dwFMJPGShowTime*COUNT_1_SEC))
                {
#ifdef SUPPORT_PHOTO_BROWSE_MODE
                    if (__bMMPhotoBrowseMode == FALSE)
#endif //SUPPORT_PHOTO_BROWSE_MODE
                        _MM_JPEGChangeToNext();
                }
            }
            break;
        case    MM_STAGE_NEXT_ITEM:
            __btPlaying=FALSE;
            __bMMStage = MM_STAGE_NONE;
            __bResetRecover = 0;
            if (!__dwMMJPEGPreview)
            {
                __bFWKey=KEY_NEXT;
            }
            else
            {
                HALJPEG_Reset();
                CC_KeyCommand(KEY_STOP);
                __bCountStop = __bMMCountStopBak;
                __bMMCountStopBak = 2;
                __dwMMJPEGPreview = FALSE;
            }
            break;
        default:
            break;
    }
	
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    if (__bMMJPEGAudioMode)
    {
        _MM_SourceControl();
    }
#endif //SUPPORT_JPEG_AUDIO_PLAYING
	
}

//  *************************************************************************
//  Function    :   MM_RepeatFolder
//  Description :   Do repeat folder checking while receiving NEXT/ PREV key
//  Arguments   :   bKey, specify the desired key action,
//                  ex.KEY_NEXT/ KEY_PREV
//  Return      :   KEY_NEXT/ KEY_PREV: same the input key while the final
//                  playing item is not over the up/ low boundary
//                  of current playing directory
//                  KEY_REPEAT: update __wPlayItem because the final playing
//                  item is over the up/ low boundary
//                  of current playing directory
//  Side Effect :
//  Global      :
//  *************************************************************************
BYTE MM_RepeatFolder(BYTE bKey)
{
    // wyc0.91, when program playing mode and repeat folder, it is Picture CD programming mode. Because CDROM don't allow repeat
    // folder when program playing mode. Then don't allow to repeat folder when program playing mode because repeat folfer will
    // go in-correct file and don't go program playing entry path.
    if (__btPlayingProgram == TRUE)
        return bKey;

    if (bKey == KEY_PREV)
    {
        // if playing item @ 1st file of current playing directory
        if(__wPlayItem == 1)
        {
            // reset to last file of current playing directory
            __wPlayItem=__pMMFilterFilesOfDir[__MMPlayingItem.wDirID];
            // wyc2.38a-909s, when un-playable AVI file number is same as total files in dir, then mean no playable files in dir 
            // and can cancel the repeat folder mode.
            if (_bMMAVIFailCnt == __pMMFilterFilesOfDir[__MMPlayingItem.wDirID])
            {
                return KEY_STOP;
            }
            else
            {
                return KEY_REPEAT;
            }
        }
    }

    if ( bKey == KEY_NEXT)
    {
        // if playing item @ last file of current playing directory
        if(__wPlayItem == __pMMFilterFilesOfDir[__MMPlayingItem.wDirID])
        {
            // reset to 1st file of current playing directory
            __wPlayItem=1;
            // wyc2.38a-909s, when un-playable AVI file number is same as total files in dir, then mean no playable files in dir 
            // and can cancel the repeat folder mode.
            if (_bMMAVIFailCnt == __pMMFilterFilesOfDir[__MMPlayingItem.wDirID])
            {
                return KEY_STOP;
            }
            else
            {
                return KEY_REPEAT;
            }
        }
    }

    return bKey;
}

//  ***************************************************************************
//  Function    :   MM_EnterThumbMode
//  Description :   This function will do the action to enter thumbnail mode.
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  Notice      :
//  ***************************************************************************
// wyc2.22-909s, rename to MM_EnterThumbMode for global function.
void MM_EnterThumbMode(void)
{
    __bMMExtFilterModeInMMMode = __bMMExtFilterMode;
	__MMSelectCursor.bPosition=POS_UNEXIST;
	__MMSelectCursor.wIndex=0xffff;

	// Stop the action first, and do correspond setting
	CC_KeyCommand (KEY_STOP);

	__bCountStop = 2;
    __dwMMJPGDispOK = TRUE;
    __dwMMJPEGPreview = FALSE;
#ifndef IMAGE_FRAME_SETUP 
    __bMMEffectIdx = HALJPEG_EFFECT_NONE;
#endif //IMAGE_FRAME_SETUP
    if (__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI)
    {
        __bTHUMBMainMode = THUMB_MAIN_MODE_EDIT;
    }
    else
    {
        __bTHUMBMainMode = THUMB_MAIN_MODE_GENERAL;
    }

    THUMB_Entry();
}

//  ***************************************************************************
//  Function    :   _MM_JPEGChangeToNext
//  Description :   This function will change JPEG to next item.
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  Notice      :
//  ***************************************************************************
void _MM_JPEGChangeToNext(void)
{
    if (!__dwMMJPEGPreview)
    {
        HALJPEG_Reset();
    }
    __bMMStage=MM_STAGE_NEXT_ITEM;
}

//  ***************************************************************************
//  Function    :   _MM_JPEGRedecode
//  Description :   This function is responsible for JPEG re-decode flow.
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  Notice      :
//  ***************************************************************************
void _MM_JPEGRedecode(void)
{
#ifdef REDECODE_CURRENT_ITEM
	if(__bResetRecover < MM_REDECODE_COUNT_LIMIT)
	{
		__bResetRecover++;
		UTL_PlayItem(__wPlayItem, 0);
	}
	else
#endif //REDECODE_CURRENT_ITEM
	{
		_MM_JPEGChangeToNext();
	}
}

//CoCo0.72, support JPEG Preview
BYTE MM_JPEGPreview(void)
{
#ifdef SUPPORT_JPG_PREVIEW
    if ((__MMSelectCursor.bPosition == POS_FILE_LIST) && (__MMSelectCursor.wIndex >= __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir))
    {
        //winnie 2.76, fix pause icon still appear when play mp3->key pause-> Pre-view jpg file .
        OSD_OUTPUT_MACRO ( MSG_PAUSE, NULL_0, 0x00);
        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
		
        if (__pMMFileRecord->FileName.bExtID == EXTNAME_JPG)
        {
#ifndef NO_MM_BUTTON
            if (__dwMMTriggerMode == MM_AUDIO_TRIGGER_MODE)
            {
                OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_FILTER);
                OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_SORT);
            }
#endif //NO_MM_BUTTON
            OSDMM_Update(OSDMM_MSG_JPEG_PREVIEW, TRUE);
            __dwMMJPEGPreview = TRUE;
            _MM_SettingAndPlayItem();
            __bMMCountStopBak = __bCountStop;
			
            //CoCo0.95, Fix the problem about "Press KEY_PLAY to play JPEG==>Press KEY_MENU before the JPEG is decoding OK==>Then we can see white noise in the top region".
            DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE); //The Display should be closed after the main vedio has set.
            OS_DelayTime(COUNT_50_MSEC);

            // wyc0.75, directly play item here and don't let it go CC_DoKeyAction to aovid some confused status.			
            UTL_PlayItem(__wPlayItem, 0);
			
            return TRUE;
        }
        else
        {
            if (__dwMMJPEGPreview)
            {
                HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
                __dwMMJPEGPreview = FALSE;
            }
        }
    }
    // wyc0.80, support JPG preview in program list mode
    if ((__MMSelectCursor.bPosition == POS_PROG_LIST) && (__pMMProgContent[__MMSelectCursor.wIndex].bExtID == EXTNAME_JPG))
    {
        OSDMM_Update(OSDMM_MSG_JPEG_PREVIEW, TRUE);
        __dwMMJPEGPreview = TRUE;
		__bMMCountStopBak = __bCountStop;
        __MMPlayingItem.wDirID = __ProgIdxList[__MMSelectCursor.wIndex].bMainIndex;
        __MMPlayingItem.wIndex= __ProgIdxList[__MMSelectCursor.wIndex].wSubIndex;
        __wPlayItem = __MMPlayingItem.wIndex;
        UTL_PlayItem(__wPlayItem, 0);
    }
#endif //SUPPORT_JPG_PREVIEW
    return FALSE;
}

void MM_ResetPreviewMode(void)
{
#ifdef SUPPORT_JPG_PREVIEW
    CC_KeyCommand(KEY_STOP);
    __bCountStop = __bMMCountStopBak;
    __bMMStage = MM_STAGE_NONE;
    __dwMMJPEGPreview = FALSE;
    __HALJPEGSetting.bFrameBuffer = 1;
#endif //SUPPORT_JPG_PREVIEW
}

void MM_RecoverPreviewWindow(void)
{
    if ((__MMSelectCursor.bPosition == POS_FILE_LIST) && (__MMSelectCursor.wIndex >= __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir))
    {
        //Dump one file for the highlighted item.
        MM_DumpOneFileForHighlightItem();
		
        HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
        MM_JPEGPreview();
    }
}

void MM_CompleteJPGEffect(BYTE bSetToFirstPlayJPEG)
{
    if ((__bAttrPlay == ATTR_JPG) && __bHALJPEGEffect != HALJPEG_EFFECT_NONE)
    {
        HALJPEG_CompleteEffectImmediately(__HALJPEGSetting.bFrameBuffer);
        DISP_DelayVSYNC(1);
		
        if (__bMMStage == MM_STAGE_DISPLAY)
        {
#ifdef JPG_SINGLE_FRAME_BUFFER
#ifdef IMAGE_FRAME_SETUP 
            if (!__bMMFirstJPEGPlaying)
#endif //IMAGE_FRAME_SETUP
            {
                // wyc2.21a-909s, change the config because JPG decoder will do it.
                HALJPEG_CopyPictureToDisplayBuffer(JPEG_EFFECT_DISP_BUFFER_2_TO_0);
                __HALJPEGSetting.bFrameBuffer = 0;
                __bHALJPEGEffect = HALJPEG_EFFECT_NONE;
                HALJPEG_Display(__HALJPEGSetting.bFrameBuffer);
            }
#endif //JPG_SINGLE_FRAME_BUFFER            
#ifdef IMAGE_FRAME_SETUP 
            if (bSetToFirstPlayJPEG)
            {
                __bMMFirstJPEGPlaying = TRUE;
            }
#endif //IMAGE_FRAME_SETUP
            
            __dwMMJPEGDisplayTime = OS_GetSysTimer();
            __bMMStage = MM_STAGE_POLLING_TIMEOUT;
			
#ifdef JPG_SINGLE_FRAME_BUFFER
			// wyc2.60-909P, move this code outside the "if (__bMMStage == MM_STAGE_DISPLAY)" to avoid some potential error.
			HALJPEG_EffectConfigure(JPEG_EFFECT_NONE);
#endif //JPG_SINGLE_FRAME_BUFFER         
        }   
    }
}

#ifdef IMAGE_FRAME_SETUP
BYTE MM_GetFirstPlayableFile(void)
{
    BYTE bPlayableFile = FALSE;

    for (__MMSelectCursor.wDirID = 0; __MMSelectCursor.wDirID < __bMMTotalDirs; __MMSelectCursor.wDirID++)
    {
        MM_PreparePage(MM_PREPARE_FILEPAGE, __MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
        if (__pMMFilterFilesOfDir[__MMSelectCursor.wDirID] != 0)
        {
            bPlayableFile = TRUE;
            break;
        }
    }
    __MMPlayingItem.wDirID = __MMSelectCursor.wDirID;
    __MMSelectCursor.wIndex = __pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir;

    return bPlayableFile;
}
#endif //IMAGE_FRAME_SETUP

#ifdef SUPPORT_ENCODE_JPG_PICTURE
void MM_EncodeFile_Init(void)
{
    BYTE bIndex, bFileID;
    HAL_ReadStorage(SETUP_ADDR_JPG_ENCODE_MASK, &bIndex ,1);
    if (bIndex != MM_JPG_ENCODE_HAVE_DATA)
    {
#ifdef SUPPORT_BUILD_IN_ENCODE_JPG
        __bMMJPGEncodeEntry = 0;
        __bMMJPGEncodeNum = BUILD_IN_JPG_ENCODE_NUM;
        for (bIndex= 0; bIndex < (BUILD_IN_JPG_ENCODE_NUM-1); bIndex++)
            __bMMJPGEncodeList[bIndex] = bIndex+1;
        __bMMJPGEncodeList[BUILD_IN_JPG_ENCODE_NUM-1] = MM_JPG_ENCODE_LIST_END;
        for (bIndex= BUILD_IN_JPG_ENCODE_NUM; bIndex < SRCFTR_SUPPORT_JPG_ENCODE_NUM; bIndex++)
            __bMMJPGEncodeList[bIndex] = MM_JPG_ENCODE_LIST_NULL;
#else
        __bMMJPGEncodeEntry = MM_JPG_ENCODE_LIST_NULL;
        __bMMJPGEncodeNum = 0;
        for (bIndex= 0; bIndex < SRCFTR_SUPPORT_JPG_ENCODE_NUM; bIndex++)
            __bMMJPGEncodeList[bIndex] = MM_JPG_ENCODE_LIST_NULL;
#endif //SUPPORT_BUILD_IN_ENCODE_JPG
        for (bIndex= 0; bIndex < SRCFTR_SUPPORT_JPG_ENCODE_NUM; bIndex++)
            __bMMJPGEncodeJPGStatus[bIndex]=0;
        bIndex=  MM_JPG_ENCODE_HAVE_DATA;
        HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_MASK, &bIndex, 1);
        HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_LIST, __bMMJPGEncodeList, SRCFTR_SUPPORT_JPG_ENCODE_NUM);
        HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_ENTRY, &__bMMJPGEncodeEntry, 1);
        HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_STATUS, __bMMJPGEncodeJPGStatus, SRCFTR_SUPPORT_JPG_ENCODE_NUM);
    }
    else
    {
        __bMMJPGEncodeNum = 0;
        HAL_ReadStorage(SETUP_ADDR_JPG_ENCODE_FILE_LIST, __bMMJPGEncodeList, SRCFTR_SUPPORT_JPG_ENCODE_NUM);
        HAL_ReadStorage(SETUP_ADDR_JPG_ENCODE_FILE_ENTRY, &__bMMJPGEncodeEntry, 1);
        HAL_ReadStorage(SETUP_ADDR_JPG_ENCODE_FILE_STATUS, __bMMJPGEncodeJPGStatus, SRCFTR_SUPPORT_JPG_ENCODE_NUM);
        if (__bMMJPGEncodeEntry != MM_JPG_ENCODE_LIST_NULL)
        {
            __bMMJPGEncodeNum = 1;
            bFileID = __bMMJPGEncodeEntry;
            for (bIndex=0; bIndex<SRCFTR_SUPPORT_JPG_ENCODE_NUM; bIndex++)
            {
                bFileID = __bMMJPGEncodeList[bFileID];
                if ((bFileID != MM_JPG_ENCODE_LIST_END) && (bFileID != MM_JPG_ENCODE_LIST_NULL))
                {
                    __bMMJPGEncodeNum++;
                }
                else
                    break;
            }
        }
    }
}


BYTE MM_EncodeFile_Add(WORD wDir, WORD wFileIndex)
{
    BYTE bIndex, bIndex1;
    if (__bMMJPGEncodeEntry == MM_JPG_ENCODE_LIST_NULL)
    {
        __bMMJPGEncodeEntry = 0;
        __bMMJPGEncodeNum = 1;
        __bMMJPGEncodeList[0] = MM_JPG_ENCODE_LIST_END;
        MM_EncodeJPG(wDir, wFileIndex, __bMMJPGEncodeEntry);
        HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_LIST, __bMMJPGEncodeList, SRCFTR_SUPPORT_JPG_ENCODE_NUM);
        HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_ENTRY, &__bMMJPGEncodeEntry, 1);
        return __bMMJPGEncodeNum;
    }
    else
    {
        if (__bMMJPGEncodeNum >= SRCFTR_SUPPORT_JPG_ENCODE_NUM)
            return MM_JPG_ENCODE_FULL;
        for (bIndex= 0; bIndex < SRCFTR_SUPPORT_JPG_ENCODE_NUM; bIndex++)
        {
            if (__bMMJPGEncodeList[bIndex] == MM_JPG_ENCODE_LIST_END)
            {
                for (bIndex1= 0; bIndex1 < SRCFTR_SUPPORT_JPG_ENCODE_NUM; bIndex1++)
                {
                    if (__bMMJPGEncodeList[bIndex1] == MM_JPG_ENCODE_LIST_NULL)
                    {
                        __bMMJPGEncodeList[bIndex] = bIndex1;
                        __bMMJPGEncodeList[bIndex1] = MM_JPG_ENCODE_LIST_END;
                        __bMMJPGEncodeNum++;
                        MM_EncodeJPG(wDir, wFileIndex, bIndex1);
                        HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_LIST, __bMMJPGEncodeList, SRCFTR_SUPPORT_JPG_ENCODE_NUM);
                        HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_ENTRY, &__bMMJPGEncodeEntry, 1);
                        return __bMMJPGEncodeNum;
                    }
                }
            }
        }
    }
    return MM_JPG_ENCODE_FULL;
}

BYTE MM_EncodeFile_Delete(BYTE bFileIndex)
{
    BYTE bIndex;
    BYTE bFileID, bPreFileID;
    if (__bMMJPGEncodeEntry == MM_JPG_ENCODE_LIST_NULL) // no entry
    {
        return MM_JPG_ENCODE_EMPTY;
    }
    else if (bFileIndex >= __bMMJPGEncodeNum)
    {
        return FALSE;
    }
    else // have entry
    {
        if (__bMMJPGEncodeNum <= 1)
        {
            __bMMJPGEncodeList[__bMMJPGEncodeEntry] = MM_JPG_ENCODE_LIST_NULL;
            __bMMJPGEncodeEntry = MM_JPG_ENCODE_LIST_NULL;
            __bMMJPGEncodeJPGStatus[__bMMJPGEncodeEntry]=0;
        }
        else
        {
            if (bFileIndex == 0)
            {
                bFileID = __bMMJPGEncodeEntry;
                __bMMJPGEncodeEntry = __bMMJPGEncodeList[__bMMJPGEncodeEntry];
                __bMMJPGEncodeList[bFileID] = MM_JPG_ENCODE_LIST_NULL;
                __bMMJPGEncodeJPGStatus[bFileID] = 0;
            }
            else
            {
                
                bPreFileID = __bMMJPGEncodeEntry;
                bFileID = __bMMJPGEncodeList[__bMMJPGEncodeEntry];
                for (bIndex= 0; bIndex < (bFileIndex-1); bIndex++)
                {
                    bPreFileID = bFileID;
                    bFileID = __bMMJPGEncodeList[bFileID];
                }
                __bMMJPGEncodeList[bPreFileID] = __bMMJPGEncodeList[bFileID];
                __bMMJPGEncodeList[bFileID] = MM_JPG_ENCODE_LIST_NULL;
                __bMMJPGEncodeJPGStatus[bFileID] = 0;
            }
        }
        __bMMJPGEncodeNum--;
    }
    HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_LIST, __bMMJPGEncodeList, SRCFTR_SUPPORT_JPG_ENCODE_NUM);
    HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_ENTRY, &__bMMJPGEncodeEntry, 1);
    HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_STATUS, __bMMJPGEncodeJPGStatus, SRCFTR_SUPPORT_JPG_ENCODE_NUM);
    return TRUE;
}

BYTE MM_EncodeFile_GetRange(BYTE bFileIndex)
{
    BYTE bIndex;
    BYTE bFileID;
    if (__bMMJPGEncodeEntry == MM_JPG_ENCODE_LIST_NULL)
    {
        return MM_JPG_ENCODE_EMPTY;
    }
    else
    {
        if (bFileIndex == 0)
        {
            __dwTimeBegin = __bMMJPGEncodeEntry*SRCFTR_ONE_JPG_ENCODE_SECTOR;
            __dwTimeEnd = __dwTimeBegin + SRCFTR_ONE_JPG_ENCODE_SECTOR - 1;
        }
        else
        {
            bFileID = __bMMJPGEncodeList[__bMMJPGEncodeEntry];
            for (bIndex= 0; bIndex < (bFileIndex-1); bIndex++)
            {
                bFileID = __bMMJPGEncodeList[bFileID];
            }
            __dwTimeBegin = bFileID*SRCFTR_ONE_JPG_ENCODE_SECTOR;
            __dwTimeEnd = __dwTimeBegin + SRCFTR_ONE_JPG_ENCODE_SECTOR-1;
        }
    }
    return TRUE;   
}

BYTE MM_EncodeFile_GetFileID(BYTE bFileIndex)
{
    BYTE bIndex;
    BYTE bFileID;
    if (__bMMJPGEncodeEntry == MM_JPG_ENCODE_LIST_NULL)
    {
        return MM_JPG_ENCODE_EMPTY;
    }
    else
    {
        if (bFileIndex == 0)
        {
            return __bMMJPGEncodeEntry;
        }
        else
        {
            bFileID = __bMMJPGEncodeList[__bMMJPGEncodeEntry];
            for (bIndex= 0; bIndex < (bFileIndex-1); bIndex++)
            {
                bFileID = __bMMJPGEncodeList[bFileID];
            }
        }
    }
    return bFileID;
}

BYTE MM_EncodeFile_Exchange(BYTE bFileIndex1, BYTE bFileIndex2)
{
    BYTE bIndex;
    BYTE bFileID1, bFileID2;
    BYTE bPreFileID1, bPreFileID2;
    BYTE bNextFileID1, bNextFileID2;
	
    bFileID1= 0;
    bFileID2= 0;
    bPreFileID1 = 0;
    bPreFileID2 = 0;
    bNextFileID1 = 0;
    bNextFileID2 = 0;
    
    if (__bMMJPGEncodeEntry == MM_JPG_ENCODE_LIST_NULL)
    {
        return MM_JPG_ENCODE_EMPTY;
    }
    else if (bFileIndex1 == bFileIndex2)
        return FALSE;
    else
    {
        // make sure bFileIndex1 is smaller than bFileIndex2.
        if (bFileIndex1 > bFileIndex2)
        {
            bIndex = bFileIndex1;
            bFileIndex1 = bFileIndex2;
            bFileIndex2 = bIndex;
        }
        // find the file ID of bFileIndex1
        if (bFileIndex1 == 0)
        {
            bFileID1 = __bMMJPGEncodeEntry;
        }
        else
        {
            bFileID1 = __bMMJPGEncodeList[__bMMJPGEncodeEntry];
            for (bIndex= 0; bIndex < (bFileIndex1-1); bIndex++)
            {
                bFileID1 = __bMMJPGEncodeList[bFileID1];
            }
        }
        bNextFileID1 = __bMMJPGEncodeList[bFileID1];
        for (bIndex= 0; bIndex < SRCFTR_SUPPORT_JPG_ENCODE_NUM; bIndex++)
        {
            if (__bMMJPGEncodeList[bIndex] == bFileID1)
                break;
        }
        if (bIndex == SRCFTR_SUPPORT_JPG_ENCODE_NUM)
            bPreFileID1 = __bMMJPGEncodeEntry;
        else
            bPreFileID1 = bIndex;
		
        // find the file ID of bFileIndex2
        bFileID2 = __bMMJPGEncodeList[__bMMJPGEncodeEntry];
        for (bIndex= 0; bIndex < (bFileIndex2-1); bIndex++)
        {
            bFileID2 = __bMMJPGEncodeList[bFileID2];
        }
        bNextFileID2 = __bMMJPGEncodeList[bFileID2];
        for (bIndex= 0; bIndex < SRCFTR_SUPPORT_JPG_ENCODE_NUM; bIndex++)
        {
            if (__bMMJPGEncodeList[bIndex] == bFileID2)
                break;
        }
        bPreFileID2 = bIndex;
        if (bFileIndex1 == 0)
        {
            bIndex = __bMMJPGEncodeEntry;
            __bMMJPGEncodeEntry = bFileID2;
            if (__bMMJPGEncodeList[bPreFileID2] != bIndex)
                __bMMJPGEncodeList[bPreFileID2] = bIndex;
			
            bIndex = __bMMJPGEncodeList[bFileID1];
            if (__bMMJPGEncodeList[bFileID1] != __bMMJPGEncodeList[bFileID2])
                __bMMJPGEncodeList[bFileID1] = __bMMJPGEncodeList[bFileID2];
            else
                __bMMJPGEncodeList[bFileID1] = bFileID2;
            if (__bMMJPGEncodeList[bFileID2] != bIndex)
                __bMMJPGEncodeList[bFileID2] = bIndex;
            else
                __bMMJPGEncodeList[bFileID2] = bFileID1;
        }
        else
        {
            bIndex = __bMMJPGEncodeList[bPreFileID1];
            if (__bMMJPGEncodeList[bPreFileID1] != __bMMJPGEncodeList[bPreFileID2])
                __bMMJPGEncodeList[bPreFileID1] = __bMMJPGEncodeList[bPreFileID2];
            if (__bMMJPGEncodeList[bPreFileID2] != bIndex)
                __bMMJPGEncodeList[bPreFileID2] = bIndex;
			
            bIndex = __bMMJPGEncodeList[bFileID1];
            if (__bMMJPGEncodeList[bFileID1] != __bMMJPGEncodeList[bFileID2])
                __bMMJPGEncodeList[bFileID1] = __bMMJPGEncodeList[bFileID2];
            else
                __bMMJPGEncodeList[bFileID1] = bFileID2;
            if (__bMMJPGEncodeList[bFileID2] != bIndex)
                __bMMJPGEncodeList[bFileID2] = bIndex;
            else
                __bMMJPGEncodeList[bFileID2] = bFileID1;
        }
    }
    HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_LIST, __bMMJPGEncodeList, SRCFTR_SUPPORT_JPG_ENCODE_NUM);
    HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_ENTRY, &__bMMJPGEncodeEntry, 1);
    return TRUE;   
}

BYTE MM_EncodeJPG(WORD wEncodeDirID, WORD wEncodeFileIndex, BYTE bFileID)
{
    extern int sprintf( char * /* str */, const char * /* format */, ... );
    DWORD dwCheckTime;
    DWORD dwPool;
    JPEG_ENCODE_INFO JPEGEncodeInfo;
	
    // first to decode JPG to frame buffer
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
    if (__bMMSlideShowCopyDeleteMode == MM_SLIDE_SHOW_COPY_MODE)
    {
		
    }
    else
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
    {
        __dwMMJPEGPreview = TRUE;
        __MMSelectCursor.wDirID = wEncodeDirID;
        __MMSelectCursor.wIndex = wEncodeFileIndex+__pMMVolumeDirTree[wEncodeDirID].bNoOfSubDir;
        _MM_SettingAndPlayItem();
        DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE); //The Display should be closed after the main vedio has set.
        OS_DelayTime(COUNT_50_MSEC);
        
        UTL_PlayItem(__wPlayItem, 0);
        dwCheckTime = OS_GetSysTimer();
        __bMMJPGDecEncMode = TRUE;
        while ((OS_GetSysTimer() - dwCheckTime) <= (COUNT_10_SEC*5))
        {
            _MM_ImageTrigger();
#ifdef SUPPORT_WATCH_DOG
            REG_SRAM_WATCHDOG++;
#endif //SUPPORT_WATCH_DOG
            if (__bMMStage == MM_STAGE_DISPLAY)
            {
                DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE); //The Display should be closed after the main vedio has set.
                OS_DelayTime(COUNT_50_MSEC);
                HALJPEG_Reset();
                CC_KeyCommand(KEY_STOP);
                break;
            }
        }
        if (__bMMStage!=MM_STAGE_DISPLAY)
        {
            HALJPEG_Reset();
            CC_KeyCommand(KEY_STOP);
            return FALSE;
        }
    }
    //senshong: because picture width and height is not always equal to HALJPEG_DISPLAY_BUF_WIDTH and HALJPEG_DISPLAY_BUF_HEIGHT
    //So we need to query the real picture width and height in JPEG original buffer 
    //get encoding info from jpeg decoder
    JPEGDEC_ReportEncodedInfo(&JPEGEncodeInfo);
    dwCheckTime = JPEGEncodeInfo.dwPoolLength ;
    dwPool = JPEGEncodeInfo.dwPoolAddrSt;
    //Get date
    sprintf(JPEGEncodeInfo.cExifDate,"%04lu/%02lu/%02lu",MM_GetFileInfo(MM_GET_FILE_YEAR),MM_GetFileInfo(MM_GET_FILE_MONTH),MM_GetFileInfo(MM_GET_FILE_DATE));
    //Encode info into App2
    JPEGENC_SetApp2InfoAddressNSize((DWORD)(&JPEGEncodeInfo.wExifWidth), 15);
    
    JPEGENC_StartCompression(JPEGEncodeInfo.dwPoolAddrSt, JPEGEncodeInfo.dwPoolLength, JPEGEncodeInfo.dwPicAddrY, JPEGEncodeInfo.dwPicAddrUV, JPEGEncodeInfo.wPicWidth, JPEGEncodeInfo.wPicHeight, JPEGEncodeInfo.wPicStrip, JPEGEncodeInfo.wPicCoordH,JPEGEncodeInfo.wPicCoordV, 90,5,SRCFTR_ONE_JPG_ENCODE_SIZE);

    // then save into SPI flash
#ifndef  NO_SERIAL_FLASH
    if (HAL_WriteSPF((DWORD)(SRCFTR_SPI_ENCODE_ADDR + bFileID*SRCFTR_ONE_JPG_ENCODE_SIZE), (BYTE *)(dwPool), (DWORD)(SRCFTR_ONE_JPG_ENCODE_SIZE)))
       return TRUE;
    else
       return FALSE;
#endif //NO_SERIAL_FLASH
    return TRUE;
}

#endif //SUPPORT_ENCODE_JPG_PICTURE

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
BYTE _MM_JPEGAudioPlayInitial(void)
{
    BYTE bFoundJPEG;
    BYTE bFoundAudio; 
    BYTE bResult = TRUE;

    bFoundJPEG = _MM_FindJPEGFile(0);

    if (!bFoundJPEG)
    {
        //Play audio only
        __bMMJPEGAudioMode = FALSE;

#ifdef DONT_SUPPORT_PLAY_AUDIO_ONLY_IN_JPEGMUSIC_MODE
        MEDIA_ExitUSBSRC();

        __bMMVideoStreamPause = FALSE;
        __bMMAudioStreamPause = FALSE;
        __bMMStartJPEGAudioTrigger = FALSE;

        strcpy(__bMsgString, "No playable file");
        OSD_Output(MSG_NO_PLAYABLE_FILE, 0, 3);       
        
        POWERONMENU_Initial();
        return FALSE;
#endif //DONT_SUPPORT_PLAY_AUDIO_ONLY_IN_JPEGMUSIC_MODE
    }

    bFoundAudio = _MM_FindAudioFile(0);
   
    if (!bFoundAudio)
    {
        //Reset audio relative flags.
        __bMMAudioPlayEnd = TRUE;
        __bMMJPEGAudioMode = FALSE;
    }
    else
    {
        __bMMAudioStage = MM_STAGE_AUDIO_STAGE_INIT;
        __bMMAudioPlayEnd = FALSE;  
    }

    if ((bFoundJPEG == FALSE) && (bFoundAudio == FALSE))
    {
#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
        if (__bMMPlayMediaDirectlyMode)
        {
            __bMMVideoStreamPause = FALSE;
            __bMMAudioStreamPause = FALSE;
            __bMMStartJPEGAudioTrigger = FALSE;

            return FALSE;
        }
        else
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
        {
            MEDIA_ExitUSBSRC();
            POWERONMENU_Initial();
            __bMMJPEGAudioMode = FALSE;       
            
            bResult = FALSE;            
        }
    }
    
    __bMMVideoStreamPause = FALSE;
    __bMMAudioStreamPause = FALSE;
    __bMMStartJPEGAudioTrigger = FALSE;
	__bMMSkipAudio = FALSE;

    HAL_ControlAVSync(HAL_AVSYNC_VIDEOMASTER, 0);

    return bResult;
}

void _MM_GetNextPlayJPEGFile(void)
{
    if (__wMMCurrentPlayingJPEGFileIndex < (__wMMTotalJPEGFileInDir-1))
    {
        __MMPlayingItem.wIndex++;
        __wPlayItem++;
        __wMMCurrentPlayingJPEGFileIndex++;
    }
    else
    {
        //Change to different folder.
        __MMPlayingItem.wDirID++;
        if (__MMPlayingItem.wDirID >= __bMMTotalDirs)
        {
            __MMPlayingItem.wDirID = 0;
        }      

        while (_MM_FindJPEGFile(__MMPlayingItem.wDirID) == FALSE)
        {
            __MMPlayingItem.wDirID++;

            if (__MMPlayingItem.wDirID == __bMMTotalDirs)
            {
                __MMPlayingItem.wDirID = 0;
            }
        }

        __wPlayItem = __MMPlayingItem.wIndex+1-__pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir;

        __wMMCurrentPlayingJPEGFileIndex = 0;
    }
}

void _MM_GetPreviousPlayJPEGFile(void)
{
    if (__wMMCurrentPlayingJPEGFileIndex == 0)
    {
        //Change to different folder.
        if (__MMPlayingItem.wDirID == 0)
        {
            __MMPlayingItem.wDirID = __bMMTotalDirs-1;
        }
        else
        {
            __MMPlayingItem.wDirID--;
        }

        //Change to the previous folder.
        while (_MM_FindJPEGFile(__MMPlayingItem.wDirID) == FALSE)
        {
            if (__MMPlayingItem.wDirID > 0)
            {
                __MMPlayingItem.wDirID--;
            }
            else
            {
                __MMPlayingItem.wDirID = __bMMTotalDirs-1;
            }
        }

        __wPlayItem = __MMPlayingItem.wIndex-__pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir+__wMMTotalJPEGFileInDir;
        __wMMCurrentPlayingJPEGFileIndex = __wMMTotalJPEGFileInDir-1;
    }
    else
    {
        __MMPlayingItem.wIndex--;
        __wPlayItem--;
        __wMMCurrentPlayingJPEGFileIndex--;
    }                       
}

void _MM_GetNextPlayAudioFile(void)
{
    if (__wMMCurrentPlayingAudioFileIndex < (__wMMTotalAudioFileInDir-1))
    {
        __MMBackgroundPlayingItem.wIndex++;
        __wBGPlayItem++;
        __wMMCurrentPlayingAudioFileIndex++;
    }
    else
    {
        //Change to different folder.
        if (__MMBackgroundPlayingItem.wDirID < __bMMTotalDirs)
        {
            __MMBackgroundPlayingItem.wDirID++;
        }
        else
        {
            __MMBackgroundPlayingItem.wDirID = 0;
        }

        while (_MM_FindAudioFile(__MMBackgroundPlayingItem.wDirID) == FALSE)
        {
            __MMBackgroundPlayingItem.wDirID++;

            if (__MMBackgroundPlayingItem.wDirID == __bMMTotalDirs)
            {
                __MMBackgroundPlayingItem.wDirID = 0;
            }
        }

        __wBGPlayItem = __MMBackgroundPlayingItem.wIndex+1-__pMMVolumeDirTree[__MMBackgroundPlayingItem.wDirID].bNoOfSubDir;
        
        __wMMCurrentPlayingAudioFileIndex = 0;
    }
}

BYTE _MM_FindJPEGFile(WORD wDirID)
{
    WORD wDirIndex;
    WORD wFileIndex = 0;
    BYTE bFoundJPEG = FALSE;

    for (wDirIndex = wDirID; wDirIndex < __bMMTotalDirs; wDirIndex++)
    {
        if (MM_ParseDirFiles(wDirIndex, EXT_BIT_JPG|EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA, 0))
        {            
            wFileIndex = 0;
            __wMMTotalJPEGFileInDir = 0;

            while (wFileIndex < __pMMFilterFilesOfDir[wDirIndex])
            {
                MM_GetDumpedFileRecord();
                if (__pMMFileRecord->FileName.bExtID == EXTNAME_JPG)
                {
                    bFoundJPEG = TRUE;
                    __MMPlayingItem.wDirID = wDirIndex;
                    __MMPlayingItem.wIndex = __pMMVolumeDirTree[wDirIndex].bNoOfSubDir+wFileIndex;
					__MMFirstJPEGPlayingIndex = __MMPlayingItem.wIndex;
                    __wMMTotalJPEGFileInDir++;
                    break;
                }
                
                wFileIndex++;
            }
        }

        if (bFoundJPEG)
        {
            break;
        }
    }

    if (bFoundJPEG)
    {
        wFileIndex++;
        while (wFileIndex < __pMMFilterFilesOfDir[__MMPlayingItem.wDirID])
        {
            MM_GetDumpedFileRecord();
            if (__pMMFileRecord->FileName.bExtID == EXTNAME_JPG)
            {
                __wMMTotalJPEGFileInDir++;
            }

            wFileIndex++;
        }
    }

    return bFoundJPEG;
}


BYTE _MM_FindAudioFile(WORD wDirID)
{
    BYTE bFoundAudio = FALSE;
    WORD wFileIndex = 0;
    WORD wDirIndex;

    //Change to different folder.
    for (wDirIndex = wDirID; wDirIndex < __bMMTotalDirs; wDirIndex++)
    {
        if (MM_ParseDirFiles(wDirIndex, EXT_BIT_JPG|EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA, 0))
        {
            __MMBackgroundPlayingItem.wDirID = wDirIndex;
            
            wFileIndex = 0;
            __wMMTotalAudioFileInDir = 0;
            
            while (wFileIndex < __pMMFilterFilesOfDir[__MMBackgroundPlayingItem.wDirID])
            {
                MM_GetDumpedFileRecord();
                if ((__pMMFileRecord->FileName.bExtID == EXTNAME_MP3) || (__pMMFileRecord->FileName.bExtID == EXTNAME_MP2) || (__pMMFileRecord->FileName.bExtID == EXTNAME_WMA))
                {
                    bFoundAudio = TRUE;
                    __MMBackgroundPlayingItem.wIndex = __pMMVolumeDirTree[__MMBackgroundPlayingItem.wDirID].bNoOfSubDir+wFileIndex;
                    __wMMTotalAudioFileInDir++;
                    break;
                }
                
                wFileIndex++;
            }
        }

        if (bFoundAudio)
        {
            break;
        }
    }

    if (bFoundAudio)
    {
        wFileIndex++;
        while (wFileIndex < __pMMFilterFilesOfDir[__MMBackgroundPlayingItem.wDirID])
        {
            MM_GetDumpedFileRecord();
            if ((__pMMFileRecord->FileName.bExtID == EXTNAME_MP3) || (__pMMFileRecord->FileName.bExtID == EXTNAME_MP2) || (__pMMFileRecord->FileName.bExtID == EXTNAME_WMA))
            {
                __wMMTotalAudioFileInDir++;
            }

            wFileIndex++;
        }
    }

   return bFoundAudio;
}

void _MM_SourceControl(void)
{
    DWORD dwVideoRemainder = 0;
    DWORD dwAudioRemainder = 0;
    DWORD dwLowThreshold;
    DWORD dwHighThreshold;

    //Foreground stream id is 0.
    //Background stream id is 1.
    HAL_ReadInfo(HAL_INFO_VIDEO_REMAIN, &dwVideoRemainder); //DWORD
    HAL_ReadInfo(HAL_INFO_AUDIO_REMAIN, &dwAudioRemainder); //DWORD

    //Play audio only. Don't do the source toggle.
    if (__bSetupMode)
        return;

#ifdef SUPPORT_PHOTO_BROWSE_MODE
    if (__bMMPhotoBrowseMode)
    {
        return;
    }
#endif //SUPPORT_PHOTO_BROWSE_MODE

	if (__bMMSkipAudio)
		return;

//#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
    dwLowThreshold = ((DS_AD0BUF_END_MM-DS_AD0BUF_ST_MM)/4)/2;
    dwHighThreshold = ((DS_AD0BUF_END_MM-DS_AD0BUF_ST_MM)/4)/4*3;
    /*
#elif (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_32)
    dwLowThreshold = ((DS_AD0BUF_END_MM-DS_AD0BUF_ST_MM)/4)/4;
    dwHighThreshold = ((DS_AD0BUF_END_MM-DS_AD0BUF_ST_MM)/4)/2;
#else
    dwLowThreshold = ((DS_AD0BUF_END_MM-DS_AD0BUF_ST_MM)/4)/8;
    dwHighThreshold = ((DS_AD0BUF_END_MM-DS_AD0BUF_ST_MM)/4)/4;
#endif
    */

    if (dwAudioRemainder < dwLowThreshold)
    {
        if (!__bMMVideoStreamPause)
        {           
            if (!__bMMVideoStreamPause && (dwVideoRemainder != 0)) //first time.
            {
                if (__bMMStage < MM_STAGE_PARSE_HEADER)
                {
                    return;
                }
            }

            if (UTL_StreamPause(0) == TRUE)
            {
                UTL_StreamContinue(BG_ITEM_STREAM_ID);
                
                __bMMVideoStreamPause = TRUE;
                __bMMAudioStreamPause = FALSE;
            }
        }
    }
    else if (dwAudioRemainder > dwHighThreshold)
    {
        if (!__bMMAudioStreamPause)
        {
            if (__bMMStage < MM_STAGE_PREPAREINFO)
            {
                //wait JPEG decode command.
                return;
            }
            else if (__btPause)
            {
                return;
            }

            if( UTL_StreamPause(BG_ITEM_STREAM_ID) == TRUE )
            {
                UTL_StreamContinue(0);
                
                __bMMAudioStreamPause = TRUE;
                __bMMVideoStreamPause = FALSE;
            }
        }
    }
}

BYTE _MM_SetJPEGAudioPlayItem(void)
{
    //JPEG
    __wPlayItem = __MMPlayingItem.wIndex+1-__pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir;
    __wMMCurrentPlayingJPEGFileIndex = __MMPlayingItem.wIndex-__MMFirstJPEGPlayingIndex; 

    //MP3
	__MMBackgroundPlayingItem.bPosition = POS_FILE_LIST;

    __wBGPlayItem = __MMBackgroundPlayingItem.wIndex+1-__pMMVolumeDirTree[__MMBackgroundPlayingItem.wDirID].bNoOfSubDir;
    __wMMCurrentPlayingAudioFileIndex = 0;

    __bMMStartJPEGAudioTrigger = TRUE;

    return KEY_PLAY;
}

void MM_ExitJPEGAudioMode(void)
{
    UTL_StopAudio();

    if (__bMMStage == MM_STAGE_DISPLAY)
    {
        MM_CompleteJPGEffect(TRUE);
    }
    
    HALJPEG_Reset();

    __bMMJPEGAudioMode = FALSE;
    __bMMStartJPEGAudioTrigger = FALSE;

    HALJPEG_Initial();

    MEDIA_ExitUSBSRC();
}

#endif //SUPPORT_JPEG_AUDIO_PLAYING

#ifdef SUPPORT_PHOTO_BROWSE_MODE
void _MM_EnterPhotoBrowseMode(void)
{
    WORD wAudioFileNum = 0; 

    if (__bMMPhotoBrowseMode)
    {
        HALJPEG_Reset();
        if (__bMMJPEGAudioMode)
        {
            MM_ParseDirFiles(__MMPlayingItem.wDirID, EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA, 0);
            wAudioFileNum = __pMMFilterFilesOfDir[__MMPlayingItem.wDirID];
            __wPlayItem = __MMPlayingItem.wIndex-wAudioFileNum-__pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir+1;        
            MM_PreparePage(MM_PREPARE_FILEPAGE, __MMPlayingItem.wDirID, EXT_BIT_JPG, __bMMSortMethod);        
        }
        MM_EnterThumbMode();
    }
    else
    {
        __bMMPhotoBrowseMode = TRUE;
        
        if (__bMMJPEGAudioMode)
        {
            __bMMEnterBrowseFromJPEGAudioMode = TRUE;

            UTL_StopAudio();
            
            UTL_StreamContinue(0);
            
            __bMMAudioStreamPause = TRUE;
            __bMMVideoStreamPause = FALSE;
        }
        else
        {
            __bMMEnterBrowseFromJPEGAudioMode = FALSE;
        }
    }
}

void MM_ExitPhotoBrowseMode(void)
{
    if (__bMMPhotoBrowseMode)
    {
        __bMMPhotoBrowseMode = FALSE;
        
        __bMMJPEGAudioMode = FALSE;
        __bMMStartJPEGAudioTrigger = FALSE;
    }
}

#endif //SUPPORT_PHOTO_BROWSE_MODE

#ifdef SUPPORT_AUTO_ROTATION
void _MM_DetectAutoRotate(void)
{ 
    BYTE bRotate = 0;

    if ((OS_GetSysTimer() - _dwMMAutoRotateCount) < COUNT_500_MSEC)
    {
        return;
    }
    else
    {
        _dwMMAutoRotateCount = OS_GetSysTimer();
    }    

    bRotate = HAL_ReadGPIO(GRP_AUTOROTATE, PIN_AUTOROTATE);

#ifdef ROTATE_ACTIVE_LOW
    bRotate = !bRotate;
#endif //ROTATE_ACTIVE_LOW
       
    if (bRotate)
    {
        if (__bMMPhotoAutoRoate == FALSE)
        {
            __bMMPhotoDoRoate = TRUE;
        }
        else
        {
            __bMMPhotoDoRoate = FALSE;
        }

        __bMMPhotoAutoRoate = TRUE;
    }
    else
    {
        if (__bMMPhotoAutoRoate == TRUE)
        {
            __bMMPhotoDoRoate = TRUE;
        }
        else
        {
            __bMMPhotoDoRoate = FALSE;
        }

        __bMMPhotoAutoRoate = FALSE;
    }

    if ((__bMMPhotoAutoRoate == TRUE) && (__bMMPhotoDoRoate == TRUE))
    {
#if (ROTATE_DIRECTION == KEY_LEFT)
        _bMMPhotoAutoRoateOperation = HALJPEG_TransformOperation(__HALJPEGSetting.bOperation, HALJPEG_OPERATION_RCC);
#elif (ROTATE_DIRECTION == KEY_RIGHT)
        _bMMPhotoAutoRoateOperation = HALJPEG_TransformOperation(__HALJPEGSetting.bOperation, HALJPEG_OPERATION_RC);
#endif
        if (__bMMStage == MM_STAGE_POLLING_TIMEOUT)
        {
            __HALJPEGSetting.bOperation = _bMMPhotoAutoRoateOperation;
            HALJPEG_Operation();
        }
    }
    else if ((__bMMPhotoAutoRoate == FALSE) && (__bMMPhotoDoRoate == TRUE))
    {
#if (ROTATE_DIRECTION == KEY_LEFT)
        _bMMPhotoAutoRoateOperation = HALJPEG_TransformOperation(__HALJPEGSetting.bOperation, HALJPEG_OPERATION_RC);
#elif (ROTATE_DIRECTION == KEY_RIGHT)
        _bMMPhotoAutoRoateOperation = HALJPEG_TransformOperation(__HALJPEGSetting.bOperation, HALJPEG_OPERATION_RCC);
#endif
        if (__bMMStage == MM_STAGE_POLLING_TIMEOUT)
        {
            __HALJPEGSetting.bOperation = _bMMPhotoAutoRoateOperation;
            HALJPEG_Operation();
        }
    }
    
}
#endif //SUPPORT_AUTO_ROTATION

#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
BYTE MM_PlayPhotoInFlash(void)
{    
    __bMMJPEGAudioMode = FALSE;
    __bMMStartJPEGAudioTrigger = FALSE;

    HALJPEG_Initial();
    
    if (__bMMPlayMediaDirectlyMode == TRUE)
    {
        __bMMPlayMediaDirectlyMode = FALSE;

        if (__bMMJPGEncodeNum > 0)
        {
            MEDIA_ExitUSBSRC();

            OSD_OUTPUT_MACRO (MSG_WAITING, 0, 0);

            //Set JPEG relative setting always.
            DVDSETUP_SetSlideShowIntervalTime();
            DVDSETUP_SetSlideShowEffect();
            
            // Switch source to SPI
            __wDiscType = BOOK_M1;
            __wPlayItem = 1;
            __SF_SourceGBL[0].bSourceIndex = SOURCE_SPI;
            __bModeCmd=KEY_PLAY;  //Winnie 1.03, fix first time key press key_next then show key play
            UTL_PlayItem(__wPlayItem, 0);
            
            return TRUE;
        }
    }
    
    return FALSE;
}
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON

void MM_PLAY_Initial(void)
{
#ifdef MM_AUTO_PLAY_DCIM_DIR_FIRST
    BYTE   _MMTempString[MM_MAX_DIRNAME_LENGTH+1];
#endif //MM_AUTO_PLAY_DCIM_DIR_FIRST

#ifndef IMAGE_FRAME_SETUP 
    __bMMEffectIdx=HALJPEG_EFFECT_NONE; 
#endif //IMAGE_FRAME_SETUP

    __bMMStage=MM_STAGE_NONE;  
    __bMMAtiveFrame=0;  // frame buffer 0

#ifndef IMAGE_FRAME_SETUP
    __dwFMJPGShowTime = 5;
#endif //IMAGE_FRAME_SETUP

    __dwMMTriggerMode = MM_TRIGGER_NONE;

    __bLastPlayItem = FALSE;
    __bMMEnterRealStop = FALSE;
    __bMotionState=MOTION_STATE_NONE;

#ifdef IMAGE_FRAME_SETUP 
    __bMMFirstJPEGPlaying = TRUE;
#endif //IMAGE_FRAME_SETUP

#ifdef SUPPORT_PHOTO_BROWSE_MODE
    __bMMPhotoBrowseMode = FALSE;
#endif //SUPPORT_PHOTO_BROWSE_MODE

	__dwClearBuffMode = HAL_CLEAR_SCREEN_MPEG;

    _dwMMTriggerEmptyTime = ((((DS_AD0BUF_END_MM-DS_AD0BUF_ST_MM)*8)/(32*1000))+5)*COUNT_1_SEC;
#ifndef IMAGE_FRAME_SETUP 
    __bHALJPEGEffect = HALJPEG_EFFECT_NONE;
#endif //IMAGE_FRAME_SETUP
    __bMMCountStopBak = 2;
    __dwMMZoomMode = FALSE;
    _bMMAVIFailCnt= 0;
#ifdef SUPPORT_ENCODE_JPG_PICTURE
    __bMMJPGDecEncMode= FALSE;
#endif //SUPPORT_ENCODE_JPG_PICTURE
    if (!__bMMAutoPlay) 
    {
        __bLastPlayItem = TRUE;
    }
    else
    {
        __bLastPlayItem = FALSE;
    }

	__dwMMJPEGPreview = FALSE;

#ifndef IMAGE_FRAME_SETUP 
    if (__bEXTAttr == EXT_BIT_JPG)
        __bRepeat = REPEAT_DISC;  // default make CD-JPG go Repeat mode.
#endif //IMAGE_FRAME_SETUP
}

void _MM_PhotoOperation(BYTE bKey)
{
	//CoCo1.22, filter the keys when decoding is not completed.
	if (__bMMStage <= MM_STAGE_DISPLAY)
		return;
	
	// Get JPG file status info.
	// LLY2.80, desired index start from "1"
	MM_AccessJPGStatus(MM_GET_JPG_STATUS, __wPlayItem); 
	
	if(bKey==KEY_RIGHT)  // rotate by right 90 degrees
	{
		__HALJPEGSetting.bOperation = HALJPEG_TransformOperation(__bMMJPGStatus, HALJPEG_OPERATION_RC);
		OSD_Output(MSG_ROTATE, HALJPEG_OPERATION_RC, 0x3);
	}
	else if(bKey==KEY_LEFT) // rotate by left 90 degrees
	{
		__HALJPEGSetting.bOperation = HALJPEG_TransformOperation(__bMMJPGStatus, HALJPEG_OPERATION_RCC);
		OSD_OUTPUT_MACRO(MSG_ROTATE, HALJPEG_OPERATION_RCC, 0x3);
	}
	else if(bKey==KEY_UP) // flip horizontal
	{
		__HALJPEGSetting.bOperation = HALJPEG_TransformOperation(__bMMJPGStatus, HALJPEG_OPERATION_FH);
		OSD_OUTPUT_MACRO(MSG_FLIP, HALJPEG_OPERATION_FH, 0x3);
	}
	else if(bKey==KEY_DOWN)  // flip vertical
	{
		__HALJPEGSetting.bOperation = HALJPEG_TransformOperation(__bMMJPGStatus, HALJPEG_OPERATION_FV);
		OSD_OUTPUT_MACRO(MSG_FLIP, HALJPEG_OPERATION_FV, 0x3);
	}
	
	// Save final flip/ rotate operation into DRAM
	__bMMJPGStatus=__HALJPEGSetting.bOperation;
	
#ifdef SUPPORT_AUTO_ROTATION
	if (__bMMPhotoAutoRoate == TRUE)
	{
#if (ROTATE_DIRECTION == KEY_LEFT)
		__HALJPEGSetting.bOperation = HALJPEG_TransformOperation(__HALJPEGSetting.bOperation, HALJPEG_OPERATION_RCC);
#elif (ROTATE_DIRECTION == KEY_RIGHT)
		__HALJPEGSetting.bOperation = HALJPEG_TransformOperation(__HALJPEGSetting.bOperation, HALJPEG_OPERATION_RC);
#endif //ROTATE_DIRECTION
	}
#endif //SUPPORT_AUTO_ROTATION
	
	__dwMMJPEGDisplayTime = OS_GetSysTimer();
	MM_AccessJPGStatus(MM_STORE_JPG_STATUS, __wPlayItem); 
#ifdef JPG_SINGLE_FRAME_BUFFER
	DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
	OS_DelayTime(COUNT_50_MSEC);
#endif //JPG_SINGLE_FRAME_BUFFER
	HALJPEG_Operation();	
}

BYTE _MM_ZoomModeOperation(BYTE bKey)
{
	if ((__bMMStage!=MM_STAGE_NONE) && (__bMMStage<=MM_STAGE_DECODE)) 
	{
#ifdef IMAGE_FRAME_SETUP 
		__bMMFirstJPEGPlaying = TRUE;
#endif // IMAGE_FRAME_SETUP           
		return  KEY_NO_KEY;
	}
	
	if ((__bHALJPEGEffect != HALJPEG_EFFECT_NONE) && (__bMMStage < MM_STAGE_POLLING_TIMEOUT))
	{
		return  KEY_NO_KEY;
	}
	
#ifdef IMAGE_FRAME_SETUP 
	__bMMFirstJPEGPlaying = TRUE;
#endif //IMAGE_FRAME_SETUP
	
	if (bKey == KEY_ZOOM_LEFT)
	{
		if (!HALJPEG_Zoom(VIEW_ZOOM_LEFT))
			return INVALID_KEY;
		
#ifdef SUPPORT_GRAPH_ZOOM
		OSD_Output(MSG_ZOOM, VIEW_ZOOM_LEFT, 0x3);
#endif //SUPPORT_GRAPH_ZOOM
		OSD_Output(MSG_ZOOM_TEXT, VIEW_ZOOM_LEFT, 0x3);
	}
	else if (bKey == KEY_ZOOM_RIGHT)
	{
		if (!HALJPEG_Zoom(VIEW_ZOOM_RIGHT))
			return INVALID_KEY;
		
#ifdef SUPPORT_GRAPH_ZOOM
		OSD_Output(MSG_ZOOM, VIEW_ZOOM_RIGHT, 0x3);
#endif //SUPPORT_GRAPH_ZOOM
		OSD_Output(MSG_ZOOM_TEXT, VIEW_ZOOM_RIGHT, 0x3);
	}
	else if (bKey == KEY_ZOOM_UP)
	{
		if (!HALJPEG_Zoom(VIEW_ZOOM_UP))
			return INVALID_KEY;
		
#ifdef SUPPORT_GRAPH_ZOOM
		OSD_Output(MSG_ZOOM, VIEW_ZOOM_UP, 0x3);
#endif //SUPPORT_GRAPH_ZOOM
		OSD_Output(MSG_ZOOM_TEXT, VIEW_ZOOM_UP, 0x3);
	}
	else if (bKey == KEY_ZOOM_DOWN)
	{
		if (!HALJPEG_Zoom(VIEW_ZOOM_DOWN))
			return INVALID_KEY;
		
#ifdef SUPPORT_GRAPH_ZOOM
		OSD_Output(MSG_ZOOM, VIEW_ZOOM_DOWN, 0x3);
#endif //SUPPORT_GRAPH_ZOOM
		OSD_Output(MSG_ZOOM_TEXT, VIEW_ZOOM_DOWN, 0x3);
	}
	else
	{
		// Get desired zoom factor, and output OSD info.
		__bZoomFactor = HALJPEG_GetZoomFactor();
		if (__bZoomFactor == VIEW_ZOOM100)
			__dwMMZoomMode = FALSE;
		else
			__dwMMZoomMode = TRUE;
		HALJPEG_Zoom(__bZoomFactor);
#ifdef SUPPORT_GRAPH_ZOOM
		OSD_OUTPUT_MACRO ( MSG_ZOOM, (WORD)__bZoomFactor, 0x3 );
#endif //SUPPORT_GRAPH_ZOOM
		OSD_OUTPUT_MACRO ( MSG_ZOOM_TEXT, (WORD)__bZoomFactor, 0x3 );
	}
	return KEY_NO_KEY;
}

void _MM_DoPhotoEffect(void)
{
	__bMMEffectIdx++;

	if (__bMMEffectIdx>HALJPEG_EFFECT_LAST)
	{
		__bMMEffectIdx = HALJPEG_EFFECT_NONE;
	}
	
	OSD_Output(MSG_JPEG_EFFECT, __bMMEffectIdx, 0x3);
}

//  *********************************************************************************
//  Function    :   MM_ProcessKeyInPlayMode
//  Description :   Do key action while non-Menu mode
//  Arguments   :   bKey    : The user key
//              :   wTrackNO: The specified track number
//  Return      :   KEY_NO_KEY  , the input key is processed
//              :   INVALID_KEY , the input key is not allowed under this mode
//              :   KEY_xxx     , the main program need do more process for the key.
//  Side Effect :
//  Glboal      :
//  ***********************************************************************
BYTE MM_ProcessKeyInPlayMode(BYTE bKey)
{
    switch(bKey)
    {
    case    KEY_THUMBNAIL:
        if (__bAttrPlay == ATTR_JPG)
        {
            MM_EnterThumbMode();
            return  KEY_NO_KEY;
        }
        else
		{
            return INVALID_KEY;
		}

    // wyc1.07, don't return KEY_PROGRAM to KEY_NO_KEY and return to another function to control it, purpose is to support KEY_PROGRAM in JPG playing mode.
    case    KEY_PROGRAM:
#ifdef SUPPORT_FM_PROGRAM
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__bMMJPEGAudioMode)
        {
            return INVALID_KEY;
        }
        else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
        {
            bKey = MM_UpdateMenuInfo(bKey);
        }
        break;
#else
        return KEY_INVALID;
#endif //SUPPORT_FM_PROGRAM
    case    KEY_MENU:
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__bMMJPEGAudioMode)
        {
            MM_ExitJPEGAudioMode();
            __bFWKey = KEY_FUNCTION;
            return  KEY_NO_KEY;
        }
        else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
        {
            bKey = MM_UpdateMenuInfo(bKey);
            __bCountStop = 2;  // force enter real stop mode
        }
        break;
    case    KEY_SCF:
#ifdef  SUPPORT_TUNE_JPG_SHOW_TIME
        if (MM_GetFileType (__MMPlayingItem.wDirID, __wPlayItem) == EXTNAME_JPG)
        {
            __bTemp = __dwFMJPGShowTime + 1;
            if (__bTemp > 5)
            {
                // wyc1.24, when set GAP level to 2s, the X2 OSD will display a while then clear, so reset the time to keep at least 2s to show the X2 level GAP.
                __dwMMJPEGDisplayTime = OS_GetSysTimer();
                __dwFMJPGShowTime = 1;
            }
            else
                __dwFMJPGShowTime = __bTemp;
            __bKeyMessage = MSG_SCANFORWARD;
            OSD_OUTPUT_MACRO(MSG_SCANFORWARD, (WORD)(__dwFMJPGShowTime*2), 3);
            return KEY_NO_KEY;
        }
        else
#endif //SUPPORT_TUNE_JPG_SHOW_TIME
        // wyc2.31-909S, add code to disable key when disable define SUPPORT_TUNE_JPG_SHOW_TIME
        {
			if ((__bMMMenuID == MM_MENU_NONE) && (__bAttrPlay == ATTR_JPG))
				return INVALID_KEY;
            else
                return bKey;
        }
    case    KEY_SCB:
        if ((__bMMMenuID == MM_MENU_NONE) && (__bAttrPlay == ATTR_JPG))
            return INVALID_KEY;
        else
            return bKey;
    case    KEY_STOP:
    case    KEY_RETURN:
        if(__wDiscType & CDROM_PICTURECD)
        {
            // Nothing need to do for Kodak Picture CD
        }
        else
        {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {
                MM_ExitJPEGAudioMode();
                __bFWKey = KEY_FUNCTION;
                return  KEY_NO_KEY;
            }
            else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
            {                
                bKey = MM_UpdateMenuInfo(bKey);
            }
        }
        break;

    case    KEY_UP:
    case    KEY_DOWN:
    case    KEY_RIGHT:
    case    KEY_LEFT:
        if (__bAttrPlay == ATTR_JPG)
        {
			_MM_PhotoOperation(bKey);
            return  KEY_NO_KEY;
        }
        else
        {
            return KEY_INVALID;
        }
        break;

    case KEY_ZOOM:
        // wyc0.90, when playing MM motion files, need to return KEY for CC to control it.
        if (__bAttrPlay != ATTR_JPG)
            break;
    case KEY_ZOOM_LEFT: 
    case KEY_ZOOM_RIGHT:
    case KEY_ZOOM_UP:
    case KEY_ZOOM_DOWN:
		return _MM_ZoomModeOperation(bKey);
    case    KEY_N10PLUS:
        __dwMMJPEGDisplayTime=OS_GetSysTimer();
        break;
    case    KEY_ANGLE_CTL:
#ifndef IMAGE_FRAME_SETUP 
        if (__bAttrPlay == ATTR_JPG)
        {
			_MM_DoPhotoEffect();
            return KEY_NO_KEY;
        }
        else
#endif //IMAGE_FRAME_SETUP 
            return INVALID_KEY;
    case KEY_PLAY:
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__bMMJPEGAudioMode)
        {
            return _MM_SetJPEGAudioPlayItem();     
        }
#endif //SUPPORT_JPEG_AUDIO_PLAYING
        break;
#ifdef SUPPORT_PHOTO_BROWSE_MODE
    case KEY_BROWSE: //CoCo2.61, add picture browse mode. only KEY_NEXT/KEY_PREV will change to next or previous picture.
        OSD_Output(MSG_BROWSE_MODE, 0, 0x3);
        _MM_EnterPhotoBrowseMode();
        return KEY_NO_KEY;
#endif //SUPPORT_PHOTO_BROWSE_MODE
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
    case KEY_COPY_DEL:
        if (MM_EnterCopyDeleteMode())
        {
            return KEY_NO_KEY;
        }
        else
            return INVALID_KEY;
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
    default:
        break;
    }
    return bKey;
}

//  *********************************************************************
//  Function    :   MM_ActionForChangeTrack
//  Description :   Do the necessary procedure for desired input key
//                  It will update the desired __wPlayItem value,
//                  and update __MMPlayingItem.wDirID if change directory
//  Arguments   :   bAction, specify the desired action (NEXT/ PREV)
//                  wParam, specify the desired parameter
//  Return      :   MM_ACTION_DONE, the actio has beend done.
//                  MM_ACTION_CONTINUE, main flow must continue the action
//                  MM_ACTION_INVALID, the action is invalid
//  Side Effect :   __wPlayItem/ __MMPlayingItem.wDirID
//  Global      :
//  *********************************************************************
BYTE MM_ActionForChangeTrack(BYTE bAction, WORD wParam)
{
    //winnie
#ifdef  SUPPORT_ENCODE_JPG_PICTURE                        
    if (__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI) //winnie
	{
        return MM_ACTION_CONTINUE;
	}
#endif //SUPPORT_ENCODE_JPG_PICTURE
    // Don't go following procedure if non-CDROM title
    if (!(__wDiscType & CDROM_M1))
	{
        return MM_ACTION_CONTINUE;
	}
	
    // wyc1.05, set _bMMIdx (used to indicate if select cursor and active cursor are the same) FALSE here, we will update it in MM_ProcessHighLight
    // when _bMMIdx is FALSE. In MM_Action will set it first time.
    wParam = wParam - 1;
    _bMMIdx = FALSE;
    switch(bAction)
    {
    case    KEY_NEXT:
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__bMMJPEGAudioMode)
        {
            _MM_GetNextPlayJPEGFile();
            return MM_ACTION_DONE;
        }
        else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
        {        
            // current file index is the last file of current playing directory
            if (wParam == (__pMMFilterFilesOfDir[__MMPlayingItem.wDirID] -1))
            {                
                // Use a temp flag to keep if do the while-loop @ 2nd run
                // To avoid infinit loop while there are no files within all directory
                __bTemp=0;
                
                // find next playable directory, ie. exist file within it
                while (__MMPlayingItem.wDirID < __bMMTotalDirs)
                {
                    // wyc1.05, when acrossing dir by KEY_NEXT / KEY_PREV and select cursor and active cursor
                    // is same position. Then set _bMMIdx to TRUE to indicate need to move highlight and active cursor together.
                    if (MM_SelectCursorPlayingItemEqual())
					{
                        _bMMIdx = TRUE;
					}
                    // increase playing directory index by '1'
                    __MMPlayingItem.wDirID++;
                    // if overflow, reset it to 1st directory
                    if (__MMPlayingItem.wDirID >= __bMMTotalDirs)
                    {
                        __MMPlayingItem.wDirID=0;
                        __bTemp++;
                    }
                    
                    // if exist fils within this directory
                    // wyc0.85, need to sort/filter this searched dir to check if any opened files in dir now. Because only sort/filter
                    // then can get correct number of opened files in dir now.
                    _MM_SortFilterFiles(__MMPlayingItem.wDirID, __bMMExtFilterMode, __bMMSortMethod);
                    // wyc0.72, need to use __pMMFilterFilesOfDir to check because maybe some files are filtering out.
                    if(__pMMFilterFilesOfDir[__MMPlayingItem.wDirID])
                    {
                        break;
                    }
                    
                    // avoid the infinit loop
                    if (__bTemp >= 2)
                    {
#ifdef SUPPORT_PRINTF
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"can't find any next playable directory !!\n");
#endif //SUPPORT_PRINTF
                        break;
                    }
                }
                __wPlayItem = 1;
                _MM_SortFilterFiles(__MMPlayingItem.wDirID, __bMMExtFilterMode, __bMMSortMethod);
                _bMMAVIFailCnt= 0;
                return MM_ACTION_DONE;
            }
        }
        break;
		
    case    KEY_PREV:
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__bMMJPEGAudioMode)
        {
            _MM_GetPreviousPlayJPEGFile();
            return MM_ACTION_DONE;
        }
        else
#endif //SUPPORT_JPEG_AUDIO_PLAYING
        {
            // current file index is the 1st file of current playing directory
            if (wParam==0)
            {
                // Use a temp flag to keep if do the while-loop @ 2nd run
                // To avoid infinit loop while there are no files within all directory
                __bTemp=0;
                
                // find the previous playable directory, ie. exist file within it
                while (1)
                {
                    // if underflow, reset to last playing directory
                    // otherwise, decrase the playing directory index by '1'
                    if(__MMPlayingItem.wDirID==0)
                    {
                        __MMPlayingItem.wDirID=__bMMTotalDirs-1;
                        __bTemp++;
                    }
                    else
                    {
                        // wyc1.05, when acrossing dir by KEY_NEXT / KEY_PREV and select cursor and active cursor
                        // is same position. Then set _bMMIdx to TRUE to indicate need to move highlight and active cursor together.
                        if (MM_SelectCursorPlayingItemEqual())
						{
                            _bMMIdx = TRUE;
						}
                        __MMPlayingItem.wDirID--;
                    }
                    
                    // wyc0.85, need to sort/filter this searched dir to check if any opened files in dir now. Because only sort/filter
                    // then can get correct number of opened files in dir now.
                    _MM_SortFilterFiles(__MMPlayingItem.wDirID, __bMMExtFilterMode, __bMMSortMethod);
                    if (__pMMFilterFilesOfDir[__MMPlayingItem.wDirID])
                        break;
                    
                    // avoid the infinit loop
                    if(__bTemp>=2)
                    {
#ifdef SUPPORT_PRINTF
                        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"can't find any pevious playable directory !!\n");
#endif //SUPPORT_PRINTF
                        break;
                    }
                }
                _bMMAVIFailCnt= 0;
                
                // Update playing item as current directory's last file
                __wPlayItem = MM_ID_AbsoluteToRelative(__pMMDirNameList[__MMPlayingItem.wDirID].wMMFileRecordEnd)+1;
                return MM_ACTION_DONE;
            }
        }
        break;
		
    case    KEY_NUMBER:
        // check if input number is out-of-range
        // Notice: reference current active directory
        if (wParam >= __pMMFilterFilesOfDir[__MMSelectCursor.wDirID])
        {
            // if don't allow the input number out-of-range
            // return invalid w/ message
#ifdef NUMBER_OUTRANGE_INVALID
            OSD_Output(MSG_INVALID, OSD_ERR_KEY, 1 );
            return MM_ACTION_INVALID;
#endif //NUMBER_OUTRANGE_INVALID
			
            // otherwise, set the playing item as last file of active directory
            __wPlayItem = __pMMFilterFilesOfDir[__MMSelectCursor.wDirID];
            // wyc1.26-2, when play item is NULL, mean no files in this directory and should return INVALID
            if (__pMMFilterFilesOfDir[__MMSelectCursor.wDirID] == 0)
			{
                return MM_ACTION_INVALID;
			}
            __MMPlayingItem.wDirID=__MMSelectCursor.wDirID;
        }
        else
        {
            __wPlayItem=wParam+1;
            __MMPlayingItem.wDirID=__MMSelectCursor.wDirID;

            if (__bThumbnailMode)
            {
                THUMB_NormalExit();
            }
        }
        // wyc1.05a, need to set to 0 to represent play mode here, otherwise when stop during divx prescan mode, MM UI won't appear.
        __bCountStop = 0;

        if (__bMMMenuID != MM_MENU_NONE)
        {
			MM_NumberActionInUIMode();
		}
        // And, the final highlight will be updated within MM_ProcessHighLight
        return MM_ACTION_DONE;
    }
    return MM_ACTION_CONTINUE;
}

#ifdef CT950_STYLE
extern void _POWERONMENU_EnterPhotoMusicMode(void);
void MM_PhotoMusicErrorConcealment(void)
{	
	if (__bMMJPEGAudioMode)
	{
		__bKey = KEY_PHOTO_MUSIC;
	}
	else if (__bAttrPlay & ATTR_JPG)
	{
		__bKey = KEY_PHOTO;
	}
	else if (__bAttrPlay & TYPE_CDROM_AUDIO)
	{
		__bKey = KEY_MUSIC;
	}
	
	POWERONMENU_CheckModeKey();
}

#endif //CT950_STYLE
