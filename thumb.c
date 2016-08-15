#include "winav.h"
#include "ctkav.h"
#include "osd.h"
#include "fontindex.h" //++CoCo2.37P
#include "cc.h"
#include "linear.h"
#include "utl.h"
#include "setup.h"
#include "hal.h"
// wyc0.50-909, make infofilter.h before Mmanager.h because of Mmanager.h need to reference some information defined in infofilter.h
#include "infofilter.h"
#include "SrcFilter.h"
#include "mmanager.h"
#include "haljpeg.h"
#include "osdnd.h"
#include "input.h"
#include "thumb.h"
#include "haljpeg.h"
#include "comutl.h"
#include "jpegdec.h"
#include "parser.h"
#include "notransl.h"
#include "osdmm.h"
#include "osdss.h"
#include "romld.h"
#ifdef SUPPORT_POWERON_MENU
#include "poweronmenu.h"
#endif //SUPPORT_POWERON_MENU

#define THUMB_MAIN_FILE
//#include "string.lan"
#include "OSDString\\strextrn.h"

BYTE THUMBButtonGeneralMode[] = {THUMB_BUTTON_ROTATE, THUMB_BUTTON_GENERAL_SLIDESHOW};
BYTE THUMBButtonRotateMode[] = {THUMB_BUTTON_RESET, THUMB_BUTTON_ROTATE_RETURN};
BYTE THUMBButtonProgramMode[] = {THUMB_BUTTON_PROGRAM_SLIDESHOW, THUMB_BUTTON_PROGRAMLIST};
BYTE THUMBButtonProgramListMode[] = {THUMB_BUTTON_PROGRAMLIST_SLIDESHOW, THUMB_BUTTON_PROGRAMLIST_RETURN};
BYTE THUMBButtonEditMode[] = {THUMB_BUTTON_EDIT_DELETE, THUMB_BUTTON_EDIT_SORT};
#ifdef THUMB_NO_EDIT_MODE
BYTE THUMBButtonDeleteMode[] = {THUMB_BUTTON_DELETE_DELETEALL, THUMB_BUTTON_EDIT_SORT};
#else
BYTE THUMBButtonDeleteMode[] = {THUMB_BUTTON_DELETE_DELETEALL, THUMB_BUTTON_DELETE_RETURN};
#endif //THUMB_NO_EDIT_MODE
BYTE THUMBButtonSortMode[] = {THUMB_BUTTON_SORT_RETURN, THUMB_BUTTON_SORT_PLAY};

#ifdef THUMB_NO_EDIT_MODE
BYTE *THUMBButton[] = {THUMBButtonGeneralMode, THUMBButtonRotateMode, THUMBButtonProgramMode, THUMBButtonProgramListMode, THUMBButtonDeleteMode, THUMBButtonSortMode};
#else
BYTE *THUMBButton[] = {THUMBButtonGeneralMode, THUMBButtonRotateMode, THUMBButtonProgramMode, THUMBButtonProgramListMode, THUMBButtonEditMode, THUMBButtonDeleteMode, THUMBButtonSortMode};
#endif //THUMB_NO_EDIT_MODE

//delcare global variables
BYTE __bThumbnailMode;
WORD _wTHUMBIndex;  //current index that user want to play
WORD _wTHUMBPlayIndex;
BYTE _bTHUMBStop;
BYTE _bTHUMBTemp;
WORD _wTHUMBTemp, _wTHUMBTemp1, _wTHUMBTemp2;
BYTE _bTHUMBStage;
BYTE _bTHUMBRetryTimes;
WORD _wTHUMBTotalFiles;
WORD _wTHUMBStartPlayItem; //Use it to record the first JPEG in this folder.
DWORD _wTHUMBStartTime;
DWORD _dwTHUMBCheckTime;
DWORD _dwTHUMBTemp;

BYTE _bTHUMBCursorType;
BYTE _bTHUMBButtonIndex;
BYTE _bTHUMBSubMode;
BYTE _bTHUMBDecodeMode;

BYTE _bTHUMBTuneFrame_H;
BYTE _bTHUMBTuneFrame_V;
WORD _wTHUMBHStartPos;
WORD _wTHUMBVStartPos;

BYTE _bTHUMBTuneOSDOffset_H;
BYTE _bTHUMBTuneOSDOffset_V;
BYTE _bTHUMBTuneOSDDistance_H;
BYTE _bTHUMBTuneOSDDistance_V;

BYTE _bTHUMBPageNO;
BYTE _bTHUMBProgramListPageNo;

PARM_FRAME _THUMBFrame;
PARM_DRAW_STRING _THUMBStringAttr;
PARM_RECT _THUMBRect;
PARM_DRAW_BITMAP _THUMBDrawBitmap;

URECT _THUMBPSFBRect;
JPEG_INFO _JPEGInfo[THUMB_TOTAL_NUM_IN_PAGE];
DWORD _dwTHUMBFrameColor;
BYTE _bTHUMBFrameThickness;
DWORD _dwTHUMBFlashTime;
BYTE _dwTHUMBFlash;

BYTE __bTHUMBMainMode = THUMB_MAIN_MODE_GENERAL;

BYTE _bTHUMBStartSort = FALSE;

#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
BYTE _bTHUMBSortFileStartIndex;
BYTE _bTHUMBSortFileEndIndex;
extern BYTE __bMMJPGEncodeNum;
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI

#ifdef CT950_STYLE
WORD _wTHUMBPlayItemInFolder;
WORD _wTHUMBTotalFiles;
#endif //CT950_STYLE

#ifdef SUPPORT_JPEG_BACKGROUND
char cBackgoundName[4] = "";
#endif //SUPPORT_JPEG_BACKGROUND

extern BYTE __bZoomFactor;
extern  BYTE __bTVType; // keep the current TV type

extern WORD __wJPEGFMTExifImageWidth;
extern WORD __wJPEGFMTExifImageHeight;
extern CHAR __cJPEGFMTExifDate[11];
extern BYTE __bSETUPUIStyleChanged;

//The palette for thumbnail's background
DWORD _dwTHUMBPalette[THUMB_BACKGROUND_PALETTE_NUM] = {0x003687,0x003A91, 0x003E9B, 0x0042A5, 0x0046AF, 0x004AB9, 0x004EC3, 0x0052CD, 0x0056D7, 0x005AE1, 0x005EEB, 0x0062F5, 0x0066FF, 0x0A6CFF, 0x1472FF, 0x1E78FF, 0x287EFF, 0x3284FF, 0x3C8AFF, 0x4690FF, 0x5096FF, 0x5A9CFF, 0x64A2FF, 0x6EA8FF, 0x78AEFF, 0x78AEFF, 0x6EA8FF, 0x64A2FF, 0x5A9CFF, 0x5096FF, 0x4690FF, 0x3C8AFF, 0x3284FF, 0x287EFF, 0x1E78FF, 0x1472FF, 0x0A6CFF, 0x0066FF, 0x0062F5, 0x005EEB, 0x005AE1, 0x0056D7, 0x0052CD, 0x004EC3, 0x004AB9, 0x0046AF, 0x0042A5, 0x003E9B, 0x003A91, 0x003687};

//Define the background color
DWORD _dwTHUMBBtnEnableColor[] = {THUMB_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_1, THUMB_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_2, THUMB_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_3, THUMB_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_4};
DWORD _dwTHUMBBtnHighlightColor[] = {THUMB_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND_1, THUMB_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND_2, THUMB_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND_3, THUMB_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND_4};
DWORD _dwTHUMBHelpRegionColor[] = {THUMB_PALETTE_COLOR_HELP_REGION_BACKGROUND_1, THUMB_PALETTE_COLOR_HELP_REGION_BACKGROUND_2, THUMB_PALETTE_COLOR_HELP_REGION_BACKGROUND_3, THUMB_PALETTE_COLOR_HELP_REGION_BACKGROUND_4};
DWORD _dwTHUMBSelectFrameColor[] = {THUMB_FRAME_SELECT_COLOR_1, THUMB_FRAME_SELECT_COLOR_2, THUMB_FRAME_SELECT_COLOR_3, THUMB_FRAME_SELECT_COLOR_4};
DWORD _dwTHUMBTitleStringColor[] = {THUMB_PALETTE_COLOR_TITLE_STRING_1, THUMB_PALETTE_COLOR_TITLE_STRING_2, THUMB_PALETTE_COLOR_TITLE_STRING_3, THUMB_PALETTE_COLOR_TITLE_STRING_4};
DWORD _dwTHUMBPageStringColor[] = {THUMB_PALETTE_COLOR_PAGE_STRING_1, THUMB_PALETTE_COLOR_PAGE_STRING_2, THUMB_PALETTE_COLOR_PAGE_STRING_3, THUMB_PALETTE_COLOR_PAGE_STRING_4};

//*********************************************************************************************************
//  Function    :   THUMB_Entry
//  Abstract    :   Call this function to enter thumbnail mode.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void THUMB_Entry(void)
{
    DWORD dwSaveInt;
    
    if( !(OS_PeekFlag(&__fThreadInit) & INIT_DEC_THREAD_JPEG_DONE) )
    {
        INITIAL_ThreadInit(THREAD_JPEG_DECODER);
        
        OS_YieldThread();
        
        OS_TimedWaitFlag( &__fThreadInit, INIT_DEC_THREAD_JPEG_DONE, FLAG_WAITMODE_AND,
            (COUNT_100_MSEC) );
    }
    
    //CoCo1.07, fix the bug about "The thumbnails will be zoom in if slide show with effect (before the effect is completed)."
    HALJPEG_CompleteEffectImmediately(__HALJPEGSetting.bFrameBuffer);
    DISP_DelayVSYNC(1);

    //If it is a zoom mode, recover it to normal state.
#ifdef SUPPORT_GRAPH_ZOOM
    __bZoomFactor = VIEW_NORMAL;
    OSD_Output ( MSG_ZOOM, (WORD)__bZoomFactor, 0x3 ); //Clear PIP and OSD
#endif

    //Tell OSD that thumbnail UI enters
    OSD_ChangeUI(OSD_UI_THUMBNAIL, OSD_ENTER_UI);

    //Need to stop JPEG decoder before configure buffer.
    HALJPEG_Reset();

    //initialize some global variables when entering thumbnail first time.
    _THUMB_Initial();

    HALJPEG_Initial(); //CoCo2.38, must put it after UTL_Config_BufferMode

    HALJPEG_Reset();

    //__bAttrPlayNew = ATTR_JPG; //--CoCo0.72

    //Thumbnail always use frame buffer 0.
// wyc2.17-909S, need to ise frame buffer 0 when single buffer mode. Because frame 1 set to thumb buffer in single buffer mode.
#ifdef JPG_SINGLE_FRAME_BUFFER
    __HALJPEGSetting.bFrameBuffer = 1;
#else
    __HALJPEGSetting.bFrameBuffer = 0;
#endif //

    _THUMB_ClearScreen();

    //CoCo2.35, force to enable the display.
    HAL_DISABLE_INTERRUPTS( dwSaveInt );
    __dwVideo_en |= DISP_VIDEO_READY;
    HAL_RESTORE_INTERRUPTS( dwSaveInt );

    //Reset the "clip region".
    HALJPEG_Display(__HALJPEGSetting.bFrameBuffer);

    //HAL_Display(HAL_DISPLAY_VIDEO, TRUE); //Should enable video always to prevent from entering thumbnail mode when first JPEG slide show is not decoded OK and the video is disabled.

    //Initialize OSD in the new mode.
    _THUMB_EnterNewMode();

    //Reset the JPEG relative info.
    _THUMB_ResetJPEGInfo();

    //Reset the time of flash the current selection
    _dwTHUMBFlashTime = OS_GetSysTimer();
    _dwTHUMBFlash = FALSE;
}

//*********************************************************************************************************
//  Function    :   _THUMB_Initial
//  Abstract    :   This function will initialize all global variables.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_Initial(void)
{
    __bThumbnailMode = TRUE;

    // LLY2.22, config frame buffer as JPEG thumbnail mode
    // Otherwise, fill frame buffer result maybe wrong.
    // Since, the frame buffer are different between JPEG slide show and JPEG preview/ thumbnail mode
    // Notice: must call this API after __bThumbnailMode setting, since it will reference it.
    UTL_Config_BufferMode(DUMP_BUFF_MM_VIDEO_ONLY_MODE);

    //calculate the number of JPEG files.
    if (__bTHUMBMainMode == THUMB_MAIN_MODE_GENERAL)
    {        
#ifdef CT950_STYLE
        //Clear the flag to prevent MM will not clear the status before entering thumbnail mode.
        for (_wTHUMBTemp = 0; _wTHUMBTemp < __bMMTotalDirs; _wTHUMBTemp++)
        {
            // wyc2.60-909P, modify for unicode because unicode use bit 7 in link status
            __pMMDirNameList[_wTHUMBTemp].bMMLinkStatus &= ~MM_SORT_FILTER_LINK_READY;
        }

        _THUMB_InitializeDirFile();
#else
        _THUMB_GetTotalFiles();
        _wTHUMBIndex = __wPlayItem - 1 - _wTHUMBStartPlayItem;
#endif //CT950_STYLE
    }
    else //Edit mode
    {
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
        // Switch source to SPI
        __SF_SourceGBL[0].bSourceIndex = SOURCE_SPI;

        _wTHUMBIndex = 0;
        _wTHUMBTotalFiles = __bMMJPGEncodeNum;
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI
    }

    _bTHUMBPageNO = _wTHUMBIndex / THUMB_TOTAL_NUM_IN_PAGE;
    _wTHUMBPlayIndex = _bTHUMBPageNO * THUMB_TOTAL_NUM_IN_PAGE; //Play from what item.

    if (__bTHUMBMainMode == THUMB_MAIN_MODE_GENERAL)
    {
#ifdef CT950_STYLE
        //Caldulate the __MMPlayingItem.wDirID after _wTHUMBPlayIndex is assigned.
        _THUMB_CheckDumpFileRecord(TRUE);
#endif //CT950_STYLE
    }

    __bZoomFactor = VIEW_NORMAL;

    _bTHUMBRetryTimes = THUMB_REDECODE_TIMES;

    _THUMB_InitialStage();   

#ifdef SUPPORT_THUMB_PROGRAM
    if (__btProgram)
    {
        _bTHUMBSubMode = THUMB_SUBMODE_PROGRAM; //Enter "Program" mode directly
    }
    else
#endif //#ifdef SUPPORT_THUMB_PROGRAM
    {
        if (__bTHUMBMainMode == THUMB_MAIN_MODE_GENERAL)
        {
            _bTHUMBSubMode = THUMB_SUBMODE_GENERAL; //Enter "General" mode
        }
        else
        {
#ifdef THUMB_NO_EDIT_MODE
            _bTHUMBSubMode = THUMB_SUBMODE_DELETE_MODE; //Enter "delete" mode
#else
            _bTHUMBSubMode = THUMB_SUBMODE_EDIT_MODE; //Enter "edit" mode
#endif //THUMB_NO_EDIT_MODE
        }
    }

    _bTHUMBDecodeMode = THUMB_DECODE_PAGE;

    _THUMB_InitializeGDI();

    //Set the color of the background, color of the text, and vertical starting position for upper right message.
    _THUMB_InitializeNDAttribute();

    __bHALJPEGEffect = HALJPEG_EFFECT_NONE;

    //set variables of frame position
    _THUMB_TuneFramePosition();

    /*
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
    HALJPEG_SetDisplay();
#endif
    */
}

//*********************************************************************************************************
//  Function    :   THUMB_UI_Trigger
//  Abstract    :   Call this function by OSD_Trigger
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
// LLY2.53, expand time value unit from WORD to DWORD
// Since, we will use [23:16]:hour, [15:8]: min, [7:0]: sec
//void THUMB_UI_Trigger(BYTE bType, WORD wTime)
void THUMB_UI_Trigger(BYTE bType, DWORD dwTime)
{
    //Flash the current selection
    if (bType == OSD_TRIGGER_CONTROL)
    {
        if (__bThumbnailMode && _bTHUMBStop && (_bTHUMBCursorType == THUMB_CURSOR_PICTURE))
        {
            if ((OS_GetSysTimer() - _dwTHUMBFlashTime) > COUNT_1_SEC)
            {
                _bTHUMBTemp = (_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE);

#ifdef CT950_STYLE
                if ((_bTHUMBSubMode == THUMB_SUBMODE_SORT_MODE) && (_bTHUMBStartSort))
                {
                    _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_PROGRAM);
                    _dwTHUMBFlashTime = OS_GetSysTimer();
                    return;
                }
#endif //CT950_STYLE

                if (_dwTHUMBFlash == FALSE)
                {
                    _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_SELECT);
                    _dwTHUMBFlash = TRUE;
                }
                else
                {
                    if ((_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM) && (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, (BYTE)__MMSelectCursor.wDirID, (WORD)(_wTHUMBIndex+1)) != 0xFF)) //This is a programmed item.
                    {
                        _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_PROGRAM);
                    }
                    else
                    {
                        _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_NORMAL);
                    }

                    _dwTHUMBFlash = FALSE;
                }

                _dwTHUMBFlashTime = OS_GetSysTimer();
            }
        }
    }
}



//*********************************************************************************************************
//  Function    :   THUMB_Trigger
//  Abstract    :   Call this function in main loop.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void THUMB_Trigger(void)
{
    if (__bThumbnailMode)
    {
        /*
        if (_bTHUMBStop || __btPause)
        {
            __btPlaying = FALSE;
            return;
        }
        */

        if (_bTHUMBStop)
        {
            __btPlaying = FALSE;
            return;
        }

        if (__btPause)
        {
            _wTHUMBStartTime = OS_GetSysTimer();
            return;
        }

        switch (_bTHUMBStage)
        {
        case THUMB_STAGE_START:
            _bTHUMBRetryTimes = THUMB_REDECODE_TIMES;

            if (_bTHUMBDecodeMode == THUMB_DECODE_ONE) //decode only one thumbnail
            {
                _THUMB_Start();
            }
            else
            {
                if (!_THUMB_CheckPlayEnd())
                    _THUMB_Start();
                else //All thumbnails has been decoded. Stop!
                {
                    _bTHUMBStop = TRUE;
                    _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);
                    _THUMB_DisplayPageInfo(); //CoCo, Show the page info. if this is the last page and there are not "THUMB_TOTAL_NUM_IN_PAGE" thumbnails in this page.
                }
            }
            break;
        case THUMB_STAGE_PARSEHEADER:
            if ((OS_GetSysTimer() - _wTHUMBStartTime) < THUMB_MAX_PARSEHEADER_TIME)
            {
                 _bTHUMBTemp = HALJPEG_Status(HALJPEG_PARSE_HEADER);
                if (_bTHUMBTemp == JPEG_STATUS_OK)
                {
                    //update the JPEG info.
                    //_THUMB_GetJPEGInfo((BYTE)(_wTHUMBPlayIndex % THUMB_TOTAL_NUM_IN_PAGE));
                    //_THUMB_ShowGeneralHelp((BYTE)(_wTHUMBPlayIndex % THUMB_TOTAL_NUM_IN_PAGE));

                    _THUMB_Decode();
                    OSD_Output(MSG_PROGRESS, 0, 1); //CoCo1.10
                    _bTHUMBStage = THUMB_STAGE_DECODE;
                    _wTHUMBStartTime = OS_GetSysTimer();
                }
                else if (_bTHUMBTemp == JPEG_STATUS_UNSUPPORT)
                {
                    _bTHUMBRetryTimes = 0;
                    _THUMB_ProcessError();
                    return;
                }
                else if (_bTHUMBTemp == JPEG_STATUS_FAIL)
                {
                    _THUMB_ProcessError();
                    return;
                }
                else //unfinish
                {
                    _THUMB_PlayEnd();
                    return;
                }
            }
            else
            {
                _THUMB_ProcessError();
                return;
            }
            _THUMB_GetPosition((BYTE)(_wTHUMBPlayIndex % THUMB_TOTAL_NUM_IN_PAGE));
            _bTHUMBStage = THUMB_STAGE_DECODE;
            break;
        case THUMB_STAGE_DECODE:
            if ((OS_GetSysTimer() - _wTHUMBStartTime) < THUMB_MAX_DECODE_TIME)
            {
                OSD_Output(MSG_PROGRESS, 0, 1); //CoCo1.10
                _bTHUMBTemp = HALJPEG_Status(HALJPEG_DECODE);
                if (_bTHUMBTemp == JPEG_STATUS_OK)
                {
                    //HAL_ControlCDIF(HAL_CDIF_CLOSE); //--CoCo1.20, reduce code size
                    __btPlayEnd=TRUE;   //data input end, but not playing end
                    _THUMB_GetJPEGInfo((BYTE)(_wTHUMBPlayIndex % THUMB_TOTAL_NUM_IN_PAGE));
					_THUMB_ShowGeneralHelp((BYTE)(_wTHUMBPlayIndex % THUMB_TOTAL_NUM_IN_PAGE));

                    HALJPEG_Display(__HALJPEGSetting.bFrameBuffer);
                    if (_bTHUMBDecodeMode == THUMB_DECODE_PAGE)
                    {
#ifdef SUPPORT_THUMB_PROGRAM
                        if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM)
                        {
                            if (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, (BYTE)__MMSelectCursor.wDirID, (WORD)(_wTHUMBStartPlayItem+_wTHUMBPlayIndex+1)) == 0xFF) //not found
                                _THUMB_DrawFrame((BYTE)(_wTHUMBPlayIndex%THUMB_TOTAL_NUM_IN_PAGE), THUMB_FRAME_NORMAL);
                            else
                                _THUMB_DrawFrame((BYTE)(_wTHUMBPlayIndex%THUMB_TOTAL_NUM_IN_PAGE), THUMB_FRAME_PROGRAM);
                        }
                        else
#endif //#ifdef SUPPORT_THUMB_PROGRAM
                            _THUMB_DrawFrame((BYTE)(_wTHUMBPlayIndex%THUMB_TOTAL_NUM_IN_PAGE), THUMB_FRAME_NORMAL);
                        _THUMB_ChangeToNextPic();
                    }
                    else if (_bTHUMBDecodeMode == THUMB_DECODE_UNDETERMINED)
                    {
                        //Search next picture to be decoded.
                        _wTHUMBPlayIndex++;
                        if (!_THUMB_SearchNextValidPic())
                            _bTHUMBStop = TRUE;
                    }
                    else //decode only one thumbnail
                    {
                        _bTHUMBStop = TRUE;
                        _bTHUMBDecodeMode = THUMB_DECODE_PAGE;
                    }
                }
                else if (_bTHUMBTemp == JPEG_STATUS_FAIL)
                {
                    _THUMB_ProcessError();
                }
                else //unfinish
                {
                    _THUMB_PlayEnd();
                }
            }
            else
            {
                _THUMB_ProcessError();
            }
            break;
        }
    }
}

//*********************************************************************************************************
//  Function    :   THUMB_ProcessKey
//  Abstract    :   Call this function to process the keys.
//  Arguments   :   none.
//  Return		:   KEY_NO_KEY: The input key is processed.
//              :   INVALID_KEY: The input key is not allowed.
//              :   KEY_xxx: The main program need do more process for the key.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE THUMB_ProcessKey(void)
{
    if (!__bThumbnailMode)
        return KEY_BYPASS;

    //audio keys
    if ((__bKey >= INPUT_KEY_GROUP2) && (__bKey < INPUT_KEY_GROUP3))
        return INVALID_KEY;

    switch (__bKey)
    {
    //invalid keys:
    case KEY_FF:
    case KEY_SF:
    // wyc2.17-909S, add language study case
    case KEY_LAN_STUDY:
    case KEY_SCF:
    case KEY_SCB:
    case KEY_STEP:
    case KEY_FREEZE:
    case KEY_BOOKMARK:
    case KEY_DIGEST:
    case KEY_REPEAT_AB:
    case KEY_RESUME:
    case KEY_GOTOTIME:
    case KEY_RETURN:
    case KEY_STOP:
    case KEY_ZOOM:
    case KEY_STOP_RETURN:
    case KEY_DISPLAY:
    case KEY_SPST_CTL:
    case KEY_INTRO:
    case KEY_REMAIN:
        return INVALID_KEY;
#ifdef IMAGE_FRAME_SETUP //CoCo2.38
    case KEY_FUNCTION:
        THUMB_NormalExit();
        break;
#endif //#ifdef IMAGE_FRAME_SETUP //CoCo2.38
    case KEY_MENU:
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
        if (__bTHUMBMainMode == THUMB_MAIN_MODE_EDIT)
            return INVALID_KEY;
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI

        if (!_THUMB_ToMMUI())
            return INVALID_KEY;
        break;
    case KEY_TVMODE:
        __OSDNDBottomMsgAttr.wY = THUMB_HELP_START_V+_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_HELP);
        return KEY_BYPASS;
    case KEY_PAUSE_STEP:
        __bKey = KEY_PAUSE;
        return KEY_BYPASS;
    case KEY_NEXT:  //change to next page
        return _THUMB_ChangePageAction(THUMB_NEXT_PAGE);
    case KEY_PREV:  //change to previous page
        return _THUMB_ChangePageAction(THUMB_PREVIOUS_PAGE);
    case KEY_LEFT:  //move the cursor to left item
        if (_bTHUMBCursorType == THUMB_CURSOR_PICTURE)
        {
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
            if (_bTHUMBStartSort)
            {
                if (!_THUMB_SortToPrevious())
                {
                    return INVALID_KEY;
                }
            }
            else
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI
            {
                if (_THUMB_CheckLeftBoundary())
                {
#ifdef THUMBNAIL_ROUND_HIGHLIGHT
                    _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_CLEAR);
                    
                    while (((_wTHUMBIndex % THUMB_COLUMN_NUM + 1) < THUMB_COLUMN_NUM) &&
                        ((_wTHUMBIndex+1) < _wTHUMBTotalFiles))
                    {
                        _wTHUMBIndex++;
                    }
                    
                    _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);
#else
                    return INVALID_KEY; //exceed the left item.
#endif //THUMBNAIL_ROUND_HIGHLIGHT
                }
                else
                {
                    _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_CLEAR);
                    _wTHUMBIndex--;
                    _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);
                    _THUMB_ShowGeneralHelp((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
                }
            }
        }
        else //the cursor is at the button.
        {
            if (_bTHUMBButtonIndex)
            {
                //Recover the original highlight button to normal state.
                _THUMB_DrawButton(_bTHUMBButtonIndex, THUMB_BUTTON_STATE_ENABLE);

                //Update new highlight button.
                _bTHUMBButtonIndex--;
                _THUMB_DrawButton(_bTHUMBButtonIndex, THUMB_BUTTON_STATE_HIGHLIGHT);
            }
            else
                return INVALID_KEY;
        }
        break;
    case KEY_RIGHT: //move the cursor to right item
        if (_bTHUMBCursorType == THUMB_CURSOR_PICTURE)
        {
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
            if (_bTHUMBStartSort)
            {
                if (!_THUMB_SortToNext())
                {
                    return INVALID_KEY;
                }
            }
            else
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI
            {
                if (_THUMB_CheckRightBoundary())
                {
#ifdef THUMBNAIL_ROUND_HIGHLIGHT
                    _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_CLEAR);
                    while (_wTHUMBIndex > ((_wTHUMBIndex / THUMB_COLUMN_NUM) * THUMB_COLUMN_NUM))
                    {
                        _wTHUMBIndex--;
                    }
                    _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);
#else
                    return INVALID_KEY; //exceed the right item.
#endif //THUMBNAIL_ROUND_HIGHLIGHT
                }
                else
                {
                    _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_CLEAR);
                    _wTHUMBIndex++;
                    _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);
                    _THUMB_ShowGeneralHelp((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
                }
            }
        }
        else //The cursor is at the button.
        {
            if (_bTHUMBButtonIndex == 0)
            {
                //Recover the original highlight button to normal state.
                _THUMB_DrawButton(_bTHUMBButtonIndex, THUMB_BUTTON_STATE_ENABLE);

                //Update new highlight button.
                _bTHUMBButtonIndex++;
                _THUMB_DrawButton(_bTHUMBButtonIndex, THUMB_BUTTON_STATE_HIGHLIGHT);
            }
            else
                return INVALID_KEY;
        }
        break;
    case KEY_UP:    //move the cursor to up item
        if (_bTHUMBCursorType == THUMB_CURSOR_PICTURE)
        {
            if (_THUMB_CheckUpBoundary())
            {
                return INVALID_KEY; //exceed the up item.
            }
            else
            {
                _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_CLEAR);
                _wTHUMBIndex -= THUMB_COLUMN_NUM;
                _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);
                _THUMB_ShowGeneralHelp((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
            }
        }
        else //move the cursor from the button to the thumbnail
        {
            //No thumbnails in this page. The cursor is not allowed in the thumbnail part.
            if (_wTHUMBTotalFiles == 0)
                return INVALID_KEY;

#ifdef SUPPORT_THUMB_PROGRAM
            //There is no programmed thumbnail or all thumbnails has been deleted in this page.
            if ((_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST) && (_wTHUMBTotalFiles <= (_bTHUMBProgramListPageNo * THUMB_TOTAL_NUM_IN_PAGE)))
                return INVALID_KEY;
#endif //#ifdef SUPPORT_THUMB_PROGRAM

            //change the type of the cursor.
            _bTHUMBCursorType = THUMB_CURSOR_PICTURE;

            _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);

            //Change the state of the button from "HIGHLIGHT" state to "ENABLE" state
            _THUMB_DrawButton(_bTHUMBButtonIndex, THUMB_BUTTON_STATE_ENABLE);
        }
        break;
    case KEY_DOWN:  //move the cursor to down item
        if (_bTHUMBCursorType == THUMB_CURSOR_PICTURE)
        {
            if (_THUMB_CheckDownBoundary()) //exceed the down item==>go to the button part.
            {
                if (!_bTHUMBStop)
                    return INVALID_KEY;
                else
                {
                    _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_CLEAR);
                    _bTHUMBCursorType = THUMB_CURSOR_BUTTON;
                    _THUMB_DrawButton(_bTHUMBButtonIndex, THUMB_BUTTON_STATE_HIGHLIGHT);
                }
            }
            else //not exceed the down item.
            {
                if (!_bTHUMBStop)
                {
                    if (_wTHUMBPlayIndex <= (_wTHUMBIndex + THUMB_COLUMN_NUM)) //CoCo1.05, don't move the highlight when the picture is not decoded.
                        return INVALID_KEY;
                }

                _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_CLEAR);
                _wTHUMBIndex += THUMB_COLUMN_NUM;
                _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);
                _THUMB_ShowGeneralHelp((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
            }
        }
        else //The cursor is at the button now.
            return INVALID_KEY;
        break;
#ifdef SUPPORT_PHOTO_BROWSE_MODE
    case KEY_BROWSE:
        if ((_bTHUMBSubMode == THUMB_SUBMODE_GENERAL) && (_bTHUMBCursorType == THUMB_CURSOR_PICTURE))
        {
            _THUMB_ToSlideShow(THUMB_PLAY_CURRENT);
        }
        else
        {
            _THUMB_ToSlideShow(THUMB_PLAY_START);
        }
        return KEY_BYPASS;
#endif //SUPPORT_PHOTO_BROWSE_MODE
    case KEY_PROGRAM:
#ifdef SUPPORT_THUMB_PROGRAM
        if (__btProgram)
        {
            if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM)
            {
                if (_bTHUMBStop)
                {
                    //Return to general mode
                    _bTHUMBSubMode = THUMB_SUBMODE_GENERAL;

                    //CoCo1.10, Fix the bug about the programmed items are not cleard when KEY_PROGRAM==>KEY_PROGRAM.
                    LINEAR_Program(LINEAR_PROGRAM_CLEAR, (BYTE)NULL);

                    //update frame
                    if (_bTHUMBStop)
                        _THUMB_UpdateThumbnailFrame(THUMB_UPDATE_FRAME_PROGRAM_TO_GENERAL);

                    _THUMB_EnterNewMode();
                }
                else
                    return INVALID_KEY;
            }
            else
                return INVALID_KEY;
        }
        else
        {
            if (_bTHUMBSubMode == THUMB_SUBMODE_GENERAL)
            {
                if (_bTHUMBStop)
                {
                    //Enter Program mode
                    _bTHUMBSubMode = THUMB_SUBMODE_PROGRAM;
                    __btProgram = TRUE;
                    __btInputProgram = TRUE;

                    //update frame
                    if (_bTHUMBStop)
                        _THUMB_UpdateThumbnailFrame(THUMB_UPDATE_FRAME_GENERAL_TO_PROGRAM);

                    _THUMB_EnterNewMode();
                }
                else
                    return INVALID_KEY;
            }
            else
                return INVALID_KEY;
        }
#else
        return INVALID_KEY;
#endif //#ifdef SUPPORT_THUMB_PROGRAM
        break;
    case KEY_PLAY_PAUSE:
        if (__btPlaying) // within playing mode
        {
            __bKey = KEY_PAUSE;
            return KEY_BYPASS;
        }
    case KEY_ENTER:
    case KEY_PLAY:
        if (__btPause)
            return KEY_BYPASS;
        else
            return _THUMB_ProcessEnterKey();
    case KEY_RANDOM:
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
        if (__bTHUMBMainMode == THUMB_MAIN_MODE_EDIT)
            return INVALID_KEY;
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI

        __btRandom = FALSE;
		__btPause = FALSE;
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
        DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
#endif
        THUMB_NormalExit();
    default:
        return KEY_BYPASS;
    }

    return KEY_NO_KEY;
}


//*********************************************************************************************************
//  Function    :   _THUMB_Start
//  Abstract    :   Start to play a thumbnail.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_Start(void)
{
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST)
    {
#ifdef SUPPORT_THUMB_PROGRAM
        //clear the OSD in the frame to prevent remenent "MP3".
        _THUMB_ClearFileExtInProgramList((BYTE)(_wTHUMBPlayIndex%THUMB_TOTAL_NUM_IN_PAGE));

        if (__pMMProgContent[_wTHUMBPlayIndex].bExtID != EXTNAME_JPG)
        {
            //draw frame
            _THUMB_DrawFrame((BYTE)(_wTHUMBPlayIndex%THUMB_TOTAL_NUM_IN_PAGE), THUMB_FRAME_NORMAL);
            _THUMB_ShowFileExtInProgramList((BYTE)(_wTHUMBPlayIndex%THUMB_TOTAL_NUM_IN_PAGE));
            _THUMB_ChangeToNextPic();
            return;
        }
        else
        {
            //get the programmed file from __ProgIdxList[];
            //update the playing dir for executing the playing dir.
            __MMPlayingItem.wDirID = __ProgIdxList[_wTHUMBPlayIndex].bMainIndex;

            //CoCo1.06, the temporal solution to fix the thumbnails can't be decoded completely.
            if (__SF_SourceGBL[__dwSFStreamID].bSourceIndex == SOURCE_USB0_0) // USB
            {
                // wyc1.07-909, delay 100 ms is better in some platform USB mode.
                // wyc1.11-909, don't need to delay 100ms when issuing CC_KeyCommand(KEY_STOP) here.
                //UTL_DelayTime(COUNT_100_MSEC, FALSE);
            }
            // wyc1.11-909, need to issue stop command, otherwise thumbnail will decode failed in some USB disk. The reason is unknown
            // and maybe relative to timing issue.
            CC_KeyCommand(KEY_STOP);
            UTL_PlayItem((WORD)(__ProgIdxList[_wTHUMBPlayIndex].wSubIndex), 0);

            _THUMB_ParseHeader();

            //Access the file status to set correct operation
            MM_AccessJPGStatus(MM_GET_JPG_STATUS, (WORD)(__ProgIdxList[_wTHUMBPlayIndex].wSubIndex));
        }
#endif //#ifdef SUPPORT_THUMB_PROGRAM
    }
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
    else if ((_bTHUMBSubMode == THUMB_SUBMODE_EDIT_MODE) || (_bTHUMBSubMode == THUMB_SUBMODE_DELETE_MODE) || (_bTHUMBSubMode == THUMB_SUBMODE_SORT_MODE))
    {
        UTL_PlayItem((WORD)(_wTHUMBStartPlayItem+_wTHUMBPlayIndex+1), 0);
        _THUMB_ParseHeader();
     
        MM_AccessJPGStatus(MM_GET_JPG_STATUS, _wTHUMBStartPlayItem+_wTHUMBPlayIndex+1);
        
        __HALJPEGSetting.bOperation = __bMMJPGStatus;
        _wTHUMBStartTime = OS_GetSysTimer();
        _bTHUMBStage = THUMB_STAGE_PARSEHEADER;
        return;
    }
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI
    else //general mode
    {
        //update the playing dir for executing the playing dir. In general case, it is equal to __bFMActiveDir.
#ifndef CT950_STYLE
        __MMPlayingItem.wDirID = __MMSelectCursor.wDirID;
#endif //CT950_STYLE

        //CoCo1.06, the temporal solution to fix the thumbnails can't be decoded completely.
        if (__SF_SourceGBL[__dwSFStreamID].bSourceIndex == SOURCE_USB0_0) // USB
        {
            // wyc1.11-909, don't need to delay 100ms when issuing CC_KeyCommand(KEY_STOP) here.
            //UTL_DelayTime(COUNT_100_MSEC, FALSE);
        }
        // wyc1.11-909, need to issue stop command, otherwise thumbnail will decode failed in some USB disk. The reason is unknown
        // and maybe relative to timing issue.
        CC_KeyCommand(KEY_STOP);

#ifdef CT950_STYLE
		if (_bTHUMBDecodeMode == THUMB_DECODE_ONE)
		{
			_THUMB_CheckDumpFileRecord(TRUE);
		}
		else
		{
			_THUMB_CheckDumpFileRecord(FALSE);
		}

		_wTHUMBPlayItemInFolder = _THUMB_GetPlayIndex();

        UTL_PlayItem(_wTHUMBPlayItemInFolder, 0);
#else
        UTL_PlayItem((WORD)(_wTHUMBStartPlayItem+_wTHUMBPlayIndex+1), 0);
#endif //CT950_STYLE        

        _THUMB_ParseHeader();

        //Access the file status to set correct operation
#ifdef CT950_STYLE
		MM_AccessJPGStatus(MM_GET_JPG_STATUS, _wTHUMBPlayItemInFolder);
#else
        MM_AccessJPGStatus(MM_GET_JPG_STATUS, (WORD)(_wTHUMBStartPlayItem+_wTHUMBPlayIndex+1));
#endif //CT950_STYLE
    }

    __HALJPEGSetting.bOperation = __bMMJPGStatus;

    _wTHUMBStartTime = OS_GetSysTimer();
    _bTHUMBStage = THUMB_STAGE_PARSEHEADER;
    //__btPlayEnd= FALSE; //--CoCo0.91, remove it by LLY's suggestion.
}

//*********************************************************************************************************
//  Function    :   _THUMB_GetTotalFiles
//  Abstract    :   Call this function to get total files aaccording to _bTHUMBSubMode.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_GetTotalFiles(void)
{
    _bTHUMBTemp = FALSE;

#ifdef SUPPORT_THUMB_PROGRAM
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST)
    {
        //We show all kinds of files in the program list not only thumbnail.
        _wTHUMBTotalFiles = __bProgramMaxEntry;
    }
    else
#endif //#ifdef SUPPORT_THUMB_PROGRAM
    {
        _wTHUMBTotalFiles = 0;

        //Picture CD only have one valid directory and it index is 1.
        if (__wDiscType & CDROM_PICTURECD)
            __MMSelectCursor.wDirID = 1;

        // wyc0.50-909, remove it because of bDirExtAttr field is removed now.
        /* --CoCo, remove it because mmanager doesn't keep the special file attribute info.
        //CDINFO will parse the file's attribute and put it in bDirExtAttr
        if (!(__pMMDirNameList[__MMSelectCursor.wDirID].bDirExtAttr & EXTNAME_JPG)) //No JPEG files
            return;
            */

        // wyc0.85a, use new pointer to support sort/filter function.
        //Dump total files from the first files of this directory.
        MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID, 0), __pMMFilterFilesOfDir[__MMSelectCursor.wDirID]);

        for (_wTHUMBTemp = 0; _wTHUMBTemp < __pMMFilterFilesOfDir[__MMSelectCursor.wDirID]; _wTHUMBTemp++)
        {
            //Dump file record.
            MM_GetDumpedFileRecord();

            if (__pMMFileRecord->FileName.bExtID == EXTNAME_JPG)
            {
                if (!_bTHUMBTemp)
                {
                    _wTHUMBStartPlayItem = _wTHUMBTemp;
                    _bTHUMBTemp = TRUE;
                }
                _wTHUMBTotalFiles++;
            }
            else
            {
                if (_bTHUMBTemp) //All JPEG has beed parsed.
                    break;
            }
         }

		/*
		_wTHUMBStartPlayItem = 0;
        if (MM_ParseDirFiles(__MMPlayingItem.wDirID, EXT_BIT_JPG, 0))
        {
            // MM_GetDumpedFileRecord will update the __pMMFileRecord. Call MM_GetDumpedFileRecord again will go next file record
            // and update the __pMMFileRecord again.
            
            _wTHUMBTotalFiles = __pMMFilterFilesOfDir[__MMPlayingItem.wDirID];
        }
		*/
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_InitialStage
//  Abstract    :   This fucntion will initialize the stage.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   _bTHUMBStop and _bTHUMBStage are initialized.
//*********************************************************************************************************
void _THUMB_InitialStage(void)
{
    _bTHUMBStop = FALSE;
    _bTHUMBStage = THUMB_STAGE_START;
}

//*********************************************************************************************************
//  Function    :   _THUMB_InitializeGDI
//  Abstract    :   Call this function to initialize the palettes.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_InitializeGDI(void)
{
    _bTHUMBTemp = __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN;

    GDI_ClearRegion(0); //clear region 0

    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_SELECT_FRAME_BRIGHT, THUMB_PALETTE_COLOR_SELECT_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_SELECT_FRAME_DARK, THUMB_PALETTE_COLOR_SELECT_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT, THUMB_PALETTE_COLOR_NORMAL_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_NORMAL_FRAME_DARK, THUMB_PALETTE_COLOR_NORMAL_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_PROGRAM_FRAME_BRIGHT, THUMB_PALETTE_COLOR_PROGRAM_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_PROGRAM_FRAME_DARK, THUMB_PALETTE_COLOR_PROGRAM_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_HELP_STRING, THUMB_PALETTE_COLOR_HELP_STRING, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_BUTTON_STRING_ENABLE, THUMB_PALETTE_COLOR_BUTTON_STRING_ENABLE, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_BUTTON_STRING_HIGHLIGHT, THUMB_PALETTE_COLOR_BUTTON_STRING_HIGHLIGHT, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_PL_FILE_EXT, THUMB_PALETTE_COLOR_PL_FILE_EXT, FALSE);

    //GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_TITLE_STRING, THUMB_PALETTE_COLOR_TITLE_STRING, FALSE);
    //GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_PAGE_STRING, THUMB_PALETTE_COLOR_PAGE_STRING, FALSE);
    //GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND, THUMB_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND, FALSE);
    //GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND, THUMB_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND, FALSE);
    //GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_HELP_REGION_BACKGROUND, THUMB_PALETTE_COLOR_HELP_REGION_BACKGROUND, FALSE);

    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND, _dwTHUMBBtnEnableColor[_bTHUMBTemp], FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND, _dwTHUMBBtnHighlightColor[_bTHUMBTemp], FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_HELP_REGION_BACKGROUND, _dwTHUMBHelpRegionColor[_bTHUMBTemp], FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_TITLE_STRING, _dwTHUMBTitleStringColor[__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN], FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_PAGE_STRING, _dwTHUMBPageStringColor[__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN], FALSE);

    //Initialize the palettes of the icons for Program list.
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MP3_ICON_TEXT_FOREGROUND, THUMB_PALETTE_COLOR_MP3_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MP3_ICON_SHADOW_1, THUMB_PALETTE_COLOR_MP3_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MP3_ICON_SHADOW_2, THUMB_PALETTE_COLOR_MP3_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_WMA_ICON_TEXT_FOREGROUND, THUMB_PALETTE_COLOR_WMA_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_WMA_ICON_SHADOW_1, THUMB_PALETTE_COLOR_WMA_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_WMA_ICON_SHADOW_2, THUMB_PALETTE_COLOR_WMA_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MP4_ICON_TEXT_FOREGROUND, THUMB_PALETTE_COLOR_MP4_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MP4_ICON_SHADOW_1, THUMB_PALETTE_COLOR_MP4_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MP4_ICON_SHADOW_2, THUMB_PALETTE_COLOR_MP4_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MP2_ICON_TEXT_FOREGROUND, THUMB_PALETTE_COLOR_MP2_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MP2_ICON_SHADOW_1, THUMB_PALETTE_COLOR_MP2_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MP2_ICON_SHADOW_2, THUMB_PALETTE_COLOR_MP2_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_DAT_ICON_TEXT_FOREGROUND, THUMB_PALETTE_COLOR_DAT_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_DAT_ICON_SHADOW_1, THUMB_PALETTE_COLOR_DAT_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_DAT_ICON_SHADOW_2, THUMB_PALETTE_COLOR_DAT_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_VOB_ICON_TEXT_FOREGROUND, THUMB_PALETTE_COLOR_VOB_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_VOB_ICON_SHADOW_1, THUMB_PALETTE_COLOR_VOB_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_VOB_ICON_SHADOW_2, THUMB_PALETTE_COLOR_VOB_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_BUTTON_FRAME, THUMB_PALETTE_COLOR_BUTTON_FRAME, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_HELP_REGION_FRAME, THUMB_PALETTE_COLOR_HELP_REGION_FRAME, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_PROGRAM_SYMBOL, THUMB_PALETTE_COLOR_PROGRAM_SYMBOL, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MPG_ICON_TEXT_FOREGROUND, THUMB_PALETTE_COLOR_MPG_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MPG_ICON_SHADOW_1, THUMB_PALETTE_COLOR_MPG_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(THUMB_PALETTE_ENTRY_MPG_ICON_SHADOW_2, THUMB_PALETTE_COLOR_MPG_ICON_SHADOW_2, FALSE);

    //CoCo0.95
    GDI_WaitPaletteComplete();

    GDI_ActivateRegion(0); //open GDI
}

//*********************************************************************************************************
//  Function    :   _THUMB_TuneFramePosition
//  Abstract    :   Tune the thumbnail's frame when according to different TV type.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_TuneFramePosition(void)
{
    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC)
    {
        if(__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
        {
            _bTHUMBTuneFrame_H = THUMB_TUNE_FRAME_H_NTSC_PSCAN;
            _bTHUMBTuneFrame_V = THUMB_TUNE_FRAME_V_NTSC_PSCAN;

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
            _bTHUMBTuneOSDOffset_H = THUMB_TUNE_OSD_H_OFFSET_NTSC_PSCAN;
            _bTHUMBTuneOSDOffset_V = THUMB_TUNE_OSD_V_OFFSET_NTSC_PSCAN;
            _bTHUMBTuneOSDDistance_H = THUMB_TUNE_OSD_H_DISTANCE_NTSC_PSCAN;
            _bTHUMBTuneOSDDistance_V = THUMB_TUNE_OSD_V_DISTANCE_PAL_PSCAN;
#endif
        }
        else
        {
            _bTHUMBTuneFrame_H = THUMB_TUNE_FRAME_H_NTSC;
            _bTHUMBTuneFrame_V = THUMB_TUNE_FRAME_V_NTSC;

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
            _bTHUMBTuneOSDOffset_H = THUMB_TUNE_OSD_H_OFFSET_NTSC;
            _bTHUMBTuneOSDOffset_V = THUMB_TUNE_OSD_V_OFFSET_NTSC;
            _bTHUMBTuneOSDDistance_H = THUMB_TUNE_OSD_H_DISTANCE_NTSC;
            _bTHUMBTuneOSDDistance_V = THUMB_TUNE_OSD_V_DISTANCE_PAL_PSCAN;
#endif
        }
    }
    else // PAL
    {
        if(__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
        {
            _bTHUMBTuneFrame_H= THUMB_TUNE_FRAME_H_PAL_PSCAN;
            _bTHUMBTuneFrame_V= THUMB_TUNE_FRAME_V_PAL_PSCAN;

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
            _bTHUMBTuneOSDOffset_H = THUMB_TUNE_OSD_H_OFFSET_PAL_PSCAN;
            _bTHUMBTuneOSDOffset_V = THUMB_TUNE_OSD_V_OFFSET_PAL_PSCAN;
            _bTHUMBTuneOSDDistance_H = THUMB_TUNE_OSD_H_DISTANCE_PAL_PSCAN;
            _bTHUMBTuneOSDDistance_V = THUMB_TUNE_OSD_V_DISTANCE_PAL;
#endif
        }
        else
        {
            _bTHUMBTuneFrame_H= THUMB_TUNE_FRAME_H_PAL;
            _bTHUMBTuneFrame_V= THUMB_TUNE_FRAME_V_PAL;

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
            _bTHUMBTuneOSDOffset_H = THUMB_TUNE_OSD_H_OFFSET_PAL;
            _bTHUMBTuneOSDOffset_V = THUMB_TUNE_OSD_V_OFFSET_PAL;
            _bTHUMBTuneOSDDistance_H = THUMB_TUNE_OSD_H_DISTANCE_PAL;
            _bTHUMBTuneOSDDistance_V = THUMB_TUNE_OSD_V_DISTANCE_PAL;
#endif
        }
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_TuneForTVType
//  Abstract    :   Call this function to tune the value for different TV type
//  Arguments   :   bType: the type of tunning the value.
//  Return		:   the value should be tune
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_TuneForTVType(BYTE bType)
{
    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC)
        return 0;

    switch (bType)
    {
    case THUMB_TUNE_POS_TYPE_TITLE:
        return THUMB_TUNE_TITLE_START_V_PAL;
    case THUMB_TUNE_POS_TYPE_BUTTON:
        return THUMB_TUNE_BUTTON_START_V_PAL;
    case THUMB_TUNE_POS_TYPE_HELP:
        return THUMB_TUNE_HELP_START_V_PAL;
    default:
        return 0;
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_CheckPlayEnd
//  Abstract    :   Call this function to check if we've played the last thumbnail.
//  Arguments   :   none.
//  Return		:   TRUE: play end.
//              :   FALSE: You can go on playing the next thumbnail.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_CheckPlayEnd(void)
{
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
    if (_bTHUMBSubMode == THUMB_SUBMODE_SORT_MODE)
    {
        if (_wTHUMBPlayIndex > _bTHUMBSortFileEndIndex)
            return TRUE;
        else
            return FALSE;
    }
    else
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI
    {
        if ((_wTHUMBPlayIndex + 1) <= _wTHUMBTotalFiles)
            return FALSE;
        
        return TRUE;
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_UpdateHighlight
//  Abstract    :   This function will Update the highlight.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_UpdateHighlight(BYTE bUpdate)
{
    _bTHUMBTemp = (BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE);

    if (bUpdate == THUMB_UPDATE_HIGHLIGHT_DRAW)
    {
        if (_bTHUMBCursorType == THUMB_CURSOR_PICTURE)
        {
            _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_SELECT);

#ifdef SUPPORT_THUMB_PROGRAM
            if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM)
            {
                if (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, (BYTE)__MMSelectCursor.wDirID, (WORD)(_wTHUMBStartPlayItem+_wTHUMBIndex+1)) != 0xFF)
                {
                    //If it is a programmed item. Need to draw the Program symbol
                    _THUMB_OutputProgramSymbol(_bTHUMBTemp);
                }
            }
#endif //#ifdef SUPPORT_THUMB_PROGRAM
        }
    }
    else //Recover the frame to its correct state when cursor leaves it.
    {
#ifndef THUMB_DRAW_OSD_FRAME
        _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_CLEAR);
#endif

        //CoCo1.10, fix the bug about the "Program" symbol is not cleared.
        //Clear the region before re-draw the frame because the "P" for program need to be cleared.
        if (_bTHUMBSubMode != THUMB_SUBMODE_PROGRAM_LIST)
        {
            _THUMB_ClearProgramSymbol((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
        }

        //if (_wTHUMBIndex > _wTHUMBPlayIndex)
        if (_wTHUMBIndex > (_wTHUMBTotalFiles - 1))
        {
            _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_CLEAR);
        }
#ifdef SUPPORT_THUMB_PROGRAM
        else if ((_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM) && (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, (BYTE)__MMSelectCursor.wDirID, (WORD)(_wTHUMBStartPlayItem+_wTHUMBIndex+1)) != 0xFF)) //This is a programmed item.
        {
            _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_PROGRAM);
        }
#endif //SUPPORT_THUMB_PROGRAM
        else
        {
            _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_NORMAL);
        }
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_GetPosition
//  Abstract    :   This function will calculate the position for the thumbnail.
//  Arguments   :   bIndex: The index in a page. Only 0 to 8 is valid.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   Update _wTHUMBHStartPos and _wTHUMBVStartPos.
//*********************************************************************************************************
void _THUMB_GetPosition(BYTE bIndex)
{
    _wTHUMBHStartPos = (THUMB_DISTANCE_WIDTH) * (bIndex % THUMB_COLUMN_NUM) + THUMB_WIDTH * (bIndex % THUMB_COLUMN_NUM) + THUMB_H_REGION;
    _wTHUMBVStartPos = (THUMB_DISTANCE_HEIGHT_NTSC) * (bIndex / THUMB_COLUMN_NUM) + THUMB_HEIGHT_NTSC * (bIndex / THUMB_COLUMN_NUM) + THUMB_V_REGION_NTSC;
}

//*********************************************************************************************************
//  Function    :   _THUMB_DrawFrame
//  Abstract    :   Draw frame for the thumbnail.
//  Arguments   :   bIndex: The index of the thumbnail.
//              :   bMode: what kind of frame you want to draw.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_DrawFrame(BYTE bIndex, BYTE bMode)
{
#ifdef THUMB_DRAW_OSD_FRAME
    _THUMBFrame.bStyle = FRAME_DOUBLE;
    _THUMBFrame.bThickness = THUMB_FRAME_THICKNESS >> 1;
    _THUMBFrame.bColorCenter = PAL_ENTRY_COLOR_TRANSPARENT;

    _THUMB_GetPosition(bIndex);

    _wTHUMBHStartPos -= _bTHUMBTuneFrame_H;
    _wTHUMBVStartPos -= _bTHUMBTuneFrame_V;

    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC)
    {
        _THUMBFrame.rect.wRight = _wTHUMBHStartPos+THUMB_WIDTH+THUMB_TUNE_DISPLAY_WIDTH_NTSC;
        _THUMBFrame.rect.wBottom = _wTHUMBVStartPos+THUMB_HEIGHT_NTSC+THUMB_TUNE_DISPLAY_HEIGHT_NTSC;
    }
    else //PAL
    {
        _wTHUMBVStartPos += (THUMB_TUNE_DISTANCE_HEIGHT_PAL) * (bIndex / THUMB_COLUMN_NUM);
        _THUMBFrame.rect.wRight = _wTHUMBHStartPos+THUMB_WIDTH+THUMB_TUNE_DISPLAY_WIDTH_PAL;
        _THUMBFrame.rect.wBottom = _wTHUMBVStartPos+THUMB_HEIGHT_NTSC+THUMB_TUNE_DISPLAY_HEIGHT_PAL;
    }

    _THUMBFrame.rect.wLeft = _wTHUMBHStartPos;
    _THUMBFrame.rect.wTop = _wTHUMBVStartPos;

    //Clear the region before re-draw the frame because the thickness of the highlight frame may be bigger than normal frame
    _THUMBRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _THUMBRect.rect.wLeft = _THUMBFrame.rect.wLeft;
    _THUMBRect.rect.wTop = _THUMBFrame.rect.wTop;
    _THUMBRect.rect.wRight = _THUMBFrame.rect.wRight;
    _THUMBRect.rect.wBottom = _THUMBFrame.rect.wBottom;
    GDI_FillRect_909(0, &_THUMBRect);

    switch (bMode)
    {
    case THUMB_FRAME_NORMAL:
        _THUMBFrame.bColorBright = THUMB_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
        _THUMBFrame.bColorDark = THUMB_PALETTE_ENTRY_NORMAL_FRAME_DARK;
        break;
    case THUMB_FRAME_SELECT:
        _THUMBFrame.bThickness = THUMB_HIGHLIGHT_FRAME_THICKNESS >> 1;
        _THUMBFrame.bColorBright = THUMB_PALETTE_ENTRY_SELECT_FRAME_BRIGHT;
        _THUMBFrame.bColorDark = THUMB_PALETTE_ENTRY_SELECT_FRAME_DARK;
        break;
    case THUMB_FRAME_PROGRAM:
        _THUMBFrame.bColorBright = THUMB_PALETTE_ENTRY_PROGRAM_FRAME_BRIGHT;
        _THUMBFrame.bColorDark = THUMB_PALETTE_ENTRY_PROGRAM_FRAME_DARK;
        break;
    case THUMB_FRAME_CLEAR:
        _THUMBFrame.bColorBright = PAL_ENTRY_COLOR_TRANSPARENT;
        _THUMBFrame.bColorDark = PAL_ENTRY_COLOR_TRANSPARENT;
        break;
    default:
        break;
    }

    GDI_DrawFrame_909(0, &_THUMBFrame);
#else

    /* --CoCo1.10, fix the bug about the "Program" symbol is not cleared.
    //Clear the region before re-draw the frame because the "P" for program need to be cleared.
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM)
        _THUMB_ClearProgramSymbol(bIndex);
        */

    _THUMB_GetPosition(bIndex);
    switch (bMode)
    {
    case THUMB_FRAME_NORMAL:
        _bTHUMBFrameThickness = THUMB_FRAME_THICKNESS;
        _dwTHUMBFrameColor = THUMB_FRAME_NORMAL_COLOR;
        break;
    case THUMB_FRAME_SELECT:
        _bTHUMBFrameThickness = THUMB_HIGHLIGHT_FRAME_THICKNESS;
        _dwTHUMBFrameColor = _dwTHUMBSelectFrameColor[__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN]; //THUMB_FRAME_SELECT_COLOR;
        break;
    case THUMB_FRAME_PROGRAM:
        _bTHUMBFrameThickness = THUMB_FRAME_THICKNESS;
        _dwTHUMBFrameColor = THUMB_FRAME_PROGRAM_COLOR;
        break;
    case THUMB_FRAME_CLEAR:
        _bTHUMBFrameThickness = THUMB_HIGHLIGHT_FRAME_THICKNESS; //clear the max. thickness
        _dwTHUMBFrameColor = THUMB_BACKGROUND_COLOR;
        break;
    default:
        break;
    }

    _THUMBPSFBRect.wLeft = _wTHUMBHStartPos-_bTHUMBFrameThickness;
    _THUMBPSFBRect.wTop = _wTHUMBVStartPos-_bTHUMBFrameThickness;
    _THUMBPSFBRect.wRight = _wTHUMBHStartPos+THUMB_WIDTH+_bTHUMBFrameThickness;
    _THUMBPSFBRect.wBottom = _wTHUMBVStartPos+THUMB_HEIGHT_NTSC+_bTHUMBFrameThickness;

    GDI_FBDrawRect(__HALJPEGSetting.bFrameBuffer, &_THUMBPSFBRect, _bTHUMBFrameThickness, _dwTHUMBFrameColor);

#endif //THUMB_DRAW_OSD_FRAME
}

//*********************************************************************************************************
//  Function    :   _THUMB_ProcessError
//  Abstract    :   Process the error about parsing header or decoding.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ProcessError(void)
{
    //HAL_ControlCDIF(HAL_CDIF_CLOSE); //--CoCo1.20, reduce code size
    HALJPEG_Reset();

    if (_bTHUMBRetryTimes)
    {
        _bTHUMBRetryTimes--;
        _THUMB_Start();
    }
    else
    {
        if (_bTHUMBDecodeMode == THUMB_DECODE_PAGE)
        {
            _THUMB_DrawFrame((BYTE)(_wTHUMBPlayIndex%THUMB_TOTAL_NUM_IN_PAGE), THUMB_FRAME_NORMAL);
            _THUMB_ChangeToNextPic();
        }
        else if (_bTHUMBDecodeMode == THUMB_DECODE_UNDETERMINED)
        {
            _wTHUMBPlayIndex++;
            _THUMB_SearchNextValidPic();
        }
        else //decode only one thumbnail
            _bTHUMBStop = TRUE;
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_ChangeToNextPic
//  Abstract    :   Prepare to change to next picture.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ChangeToNextPic(void)
{
    _bTHUMBStage = THUMB_STAGE_START;
    _THUMB_CheckPlayPageBoundary();
    _wTHUMBPlayIndex++;
}

//*********************************************************************************************************
//  Function    :   _THUMB_CheckPlayPageBoundary
//  Abstract    :   This function will check if we have decoded all thumbnails
//              :   in a page.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_CheckPlayPageBoundary(void)
{
    if (_THUMB_CheckPagePlayEnd()) //All thumbnails has been decoded in a page.
    {
        _bTHUMBStop = TRUE;
        _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);
        _THUMB_DisplayPageInfo();
        _THUMB_ShowGeneralHelp((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_CheckPagePlayEnd
//  Abstract    :   Call this function to check if we've played the last thumbnail
//              :   in this page.
//  Arguments   :   none.
//  Return		:   TRUE: play end.
//              :   FALSE: You can go on playing the next thumbnail.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_CheckPagePlayEnd(void)
{
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST) //"Program List" mode
        _bTHUMBTemp = _bTHUMBProgramListPageNo;
    else //general mode
        _bTHUMBTemp = _bTHUMBPageNO;

    if (((_wTHUMBPlayIndex + 1) / THUMB_TOTAL_NUM_IN_PAGE) > _bTHUMBTemp)
        return TRUE;

    return FALSE;
}

//*********************************************************************************************************
//  Function    :   _THUMB_DisplayPageInfo
//  Abstract    :   This function will display the page number.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_DisplayPageInfo(void)
{
    //clear the page info.
    _THUMB_ClearRegion(THUMB_CLEAR_PAGE_INFO_REGION);

    if (_wTHUMBTotalFiles != 0)
    {
        _THUMB_SetNormalStringAttribute();
        _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_PAGE_STRING;

        OSDUTL_PrepareOutputString();
        OSDUTL_CopyStringFromIndex(aTHUMBPage);

#ifdef SUPPORT_THUMB_PROGRAM
        if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST)
        {
            OSDUTL_OutputNumFromIndex((WORD)(_bTHUMBProgramListPageNo+1));
        }
        else //general mode
#endif //SUPPORT_THUMB_PROGRAM
        {
            OSDUTL_OutputNumFromIndex((WORD)(_bTHUMBPageNO+1));
        }

        OSDUTL_CopyStringFromIndex(aTHUMBSlash);

        _bTHUMBTemp = _wTHUMBTotalFiles / THUMB_TOTAL_NUM_IN_PAGE;
        if (_wTHUMBTotalFiles % THUMB_TOTAL_NUM_IN_PAGE)
            _bTHUMBTemp++;

        OSDUTL_OutputNumFromIndex(_bTHUMBTemp);

        _THUMBStringAttr.wX = THUMB_PAGE_END_H-GDI_GetStringWidth_909(__pOSDUTLOutputString);
        _THUMBStringAttr.wY = THUMB_PAGE_START_V-_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_TITLE);

        //GDI_DrawString_909(0, &_THUMBStringAttr, __pOSDUTLOutputString);
        _THUMB_DrawString();
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_SearchNextValidPic
//  Abstract    :   This function will search the valid picture that we want to decode. Currently, it's just
//              :   for "reset" in rotate mode.
//  Arguments   :   none.
//  Return		:   TRUE: Got it.
//              :   FALSE: No suitable picture.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_SearchNextValidPic(void)
{
#ifdef CT950_STYLE
        _THUMB_CheckDumpFileRecord(TRUE);
#endif //CT950_STYLE

#ifdef CT950_STYLE
        MM_AccessJPGStatus(MM_GET_JPG_STATUS, _THUMB_GetPlayIndex());
#else
        MM_AccessJPGStatus(MM_GET_JPG_STATUS, (WORD)(_wTHUMBStartPlayItem+_wTHUMBIndex+1));
#endif //CT950_STYLE

    // wyc2.60-909P, update for unicode issue.
    while ((__pMMFileRecord->bFileAttr & ~IF_UNICODE_FILE_TAG) == HALJPEG_OPERATION_I)
    {
        _wTHUMBPlayIndex++;

        //Exceed the number of files in this folder
        if (_wTHUMBPlayIndex == _wTHUMBTotalFiles)
            return FALSE;
        else if (_wTHUMBPlayIndex == ((_bTHUMBPageNO+1) * THUMB_TOTAL_NUM_IN_PAGE)) //all thumbnails in the page are unrotating.
            return FALSE;

#ifdef CT950_STYLE
        _THUMB_CheckDumpFileRecord(TRUE);
        MM_AccessJPGStatus(MM_GET_JPG_STATUS, _THUMB_GetPlayIndex());
#else
        MM_AccessJPGStatus(MM_GET_JPG_STATUS, (WORD)(_wTHUMBStartPlayItem+_wTHUMBIndex+1));
#endif //CT950_STYLE
    }

    __bMMJPGStatus = HALJPEG_OPERATION_I;

#ifdef CT950_STYLE
	MM_AccessJPGStatus(MM_STORE_JPG_STATUS, _THUMB_GetPlayIndex());
#else
    MM_AccessJPGStatus(MM_STORE_JPG_STATUS, (WORD)(_wTHUMBStartPlayItem+_wTHUMBIndex+1));
#endif //CT950_STYLE

    //Initialize the decoding stage.
    _bTHUMBStage = THUMB_STAGE_START;

    return TRUE;
}

//*********************************************************************************************************
//  Function    :   _THUMB_PlayEnd
//  Abstract    :   Call this function to push the IFIFO when playing end.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_PlayEnd(void)
{
    if (!__btPlayEnd)
    {
        if (__dwTimeNow >= __dwTimeEnd)
        {
            __btPlayEnd = TRUE;
        }
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_CheckChangePageValid
//  Abstract    :   Call this function to see if the action of changing page is valid or not.
//  Arguments   :   bAction: THUMB_NEXT_PAGE: Change to the next page.
//              :            THUMB_PREVIOUS_PAGE: Change to the previous page.
//  Return		:   TRUE: It's valid.
//              :   FALSE: It's invalid.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_CheckChangePageValid(BYTE bAction)
{
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST) //"Program List" mode
        _bTHUMBTemp = _bTHUMBProgramListPageNo;
    else //general mode
        _bTHUMBTemp = _bTHUMBPageNO;

    if (bAction == THUMB_NEXT_PAGE)
    {
        if (((_bTHUMBTemp + 1) * THUMB_TOTAL_NUM_IN_PAGE) < _wTHUMBTotalFiles) //has "Next" page
            return TRUE;
    }
    else if (bAction == THUMB_PREVIOUS_PAGE)
    {
        if (_bTHUMBTemp > 0)
            return TRUE;
    }

    return FALSE;
}

//*********************************************************************************************************
//  Function    :   _THUMB_ChangePageAction
//  Abstract    :   Call this function when changing the page.
//  Arguments   :   bAction: THUMB_NEXT_PAGE: Change to the next page.
//              :            THUMB_PREVIOUS_PAGE: Change to the previous page.
//              :            THUMB_CURRENT_PAGE: Re-decode current page.
//  Return		:   KEY_NO_KEY: The key is processced.
//              :   INVALID_KEY: It is an invalid key.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_ChangePageAction(BYTE bAction)
{
    if (bAction != THUMB_CURRENT_PAGE)
    {
        if (!_THUMB_CheckChangePageValid(bAction))
            return INVALID_KEY;
    }

    //CoCo1.10, fix the bug about the picture is still displayed when the pictures are not all decoded OK. At that time, use can see
    //the middle picture is displayed (last page) then decoding starts from the first picture.
    HALJPEG_Reset();
    _bTHUMBStop = TRUE;

    if (__btPause)
        CC_KeyCommand(KEY_PLAY);

    _THUMB_ClearRegion(THUMB_CLEAR_GENERAL_MODE_FRAME_REGION);

    //CoCo1.10, draw help region for updating the progress bar.
    _THUMB_DrawHelpRegion();

    //Reset the JPEG relative info.
    _THUMB_ResetJPEGInfo();

    //use _wTHUMBIndex to represent page No.
    //use _wTHUMBPlayIndex to represent max files.
#ifdef SUPPORT_THUMB_PROGRAM
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST) //"Program List" mode
    {
        _wTHUMBTemp = (WORD)_bTHUMBProgramListPageNo;
    }
    else
#endif //#ifdef SUPPORT_THUMB_PROGRAM
    {
        _wTHUMBTemp = (WORD)_bTHUMBPageNO;
    }

    if (bAction == THUMB_NEXT_PAGE)
    {
        _wTHUMBTemp++;

    }
    else if (bAction == THUMB_PREVIOUS_PAGE)
    {
        _wTHUMBTemp--;
    }

    _wTHUMBPlayIndex = _wTHUMBTemp * THUMB_TOTAL_NUM_IN_PAGE;

    //recover the page No.
#ifdef SUPPORT_THUMB_PROGRAM
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST)
        _bTHUMBProgramListPageNo = (BYTE)_wTHUMBTemp;
    else
#endif //#ifdef SUPPORT_THUMB_PROGRAM
        _bTHUMBPageNO = (BYTE)_wTHUMBTemp;

	//update decoding mode
	_bTHUMBDecodeMode = THUMB_DECODE_PAGE;

    //Prepare thumbnail decoding for Previous/Next page.
    _THUMB_ClearScreen();
    _THUMB_InitialStage();

    //update cursor
    _wTHUMBIndex = _wTHUMBPlayIndex;

    _THUMB_DisplayPageInfo();

#ifdef CT950_STYLE
        _THUMB_CheckDumpFileRecord(TRUE);
#endif //CT950_STYLE

    return KEY_NO_KEY;
}

//*********************************************************************************************************
//  Function    :   _THUMB_ClearScreen
//  Abstract    :   This function will clear the screen to the color of the background.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   This function will clear all data in frame buffer.
//*********************************************************************************************************
void _THUMB_ClearScreen(void)
{
#ifdef SUPPORT_JPEG_BACKGROUND

	PSECTION_ENTRY pSection;
	if (strcmp(cBackgoundName, "") == 0)
	{
		_THUMB_DrawDefaultBackground();
	}
	else
	{
		
		pSection = ROMLD_GetSectionByName(0, cBackgoundName);

		if (pSection == 0) //Load section failed
		{
			_THUMB_DrawDefaultBackground();
			return;
		}
		
		// Set playback mode and attribute as JPEG first
		__bModePlay = MODE_PLAYUNKNOW;
		__bAttrPlayNew = ATTR_JPG;
		
		SrcFilter_Stop(__dwSFStreamID);
		HAL_ControlParser(HAL_PARSER_STOP, 0);
		
		// Config playback mode as JPEG
		UTL_SetPlayMode(MODE_PLAYVIDEO);
		
		HAL_FillVideoBuffer(HAL_VIDEOBUF_MM, (DWORD *)pSection->dwRMA, (pSection->dwRSize/4));
		
		// Micky2.11a, integrate video decoder reset procedure into a API.
		HAL_ResetVideoDecoder(HAL_VIDEO_DECODER1|HAL_VIDEO_DECODER2);
		
		if (UTL_ShowJPEG_Slide(JPEG_PARSE_TYPE_THUMB_BACKGROUND, __HALJPEGSetting.bFrameBuffer))
		{
			// Clear __bLOGO as none since it has exist default LOGO mode.
			__bLOGO=LOGO_NONE;
		}
	}
	
#else

    _THUMB_DrawDefaultBackground();    

#endif //SUPPORT_JPEG_BACKGROUND
}

void _THUMB_DrawDefaultBackground(void)
{
    BYTE bLine = 0;
    DWORD dwColor = 0;

    _THUMBPSFBRect.wLeft = 0;
    _THUMBPSFBRect.wRight = HALJPEG_DISPLAY_BUF_WIDTH;

    switch (__SetupInfo.bUIStyle)
    {
    case SETUP_CUSTOM_UI_STYLE_2:
        for (_bTHUMBTemp = 0; _bTHUMBTemp < (HALJPEG_DISPLAY_BUF_HEIGHT/4); _bTHUMBTemp++)
        {
            dwColor = (((_bTHUMBTemp*0xFF)/(HALJPEG_DISPLAY_BUF_HEIGHT/4)) << 16)+0xC800+0x64;
            _THUMBPSFBRect.wTop = (_bTHUMBTemp)*4;
            _THUMBPSFBRect.wBottom = (_bTHUMBTemp+1)*4;
             GDI_FBFillRect(__HALJPEGSetting.bFrameBuffer, &_THUMBPSFBRect, dwColor);
        }
        break;
    case SETUP_CUSTOM_UI_STYLE_3:
        for (_bTHUMBTemp = 0; _bTHUMBTemp < (HALJPEG_DISPLAY_BUF_HEIGHT/2); _bTHUMBTemp++)
        {
            dwColor = (((_bTHUMBTemp*0xFF)/(HALJPEG_DISPLAY_BUF_HEIGHT/2)) << 16)+0x9600+0xE1;
            _THUMBPSFBRect.wTop = (_bTHUMBTemp)*2;
            _THUMBPSFBRect.wBottom = (_bTHUMBTemp+1)*2;
             GDI_FBFillRect(__HALJPEGSetting.bFrameBuffer, &_THUMBPSFBRect, dwColor);
        }
        break;
    case SETUP_CUSTOM_UI_STYLE_4:
        for (_bTHUMBTemp = 0; _bTHUMBTemp < (HALJPEG_DISPLAY_BUF_HEIGHT/2); _bTHUMBTemp++)
        {
            dwColor = 0xFF0000+0xFF00+(_bTHUMBTemp*0xFF)/(HALJPEG_DISPLAY_BUF_HEIGHT/2);
            _THUMBPSFBRect.wTop = (_bTHUMBTemp)*2;
            _THUMBPSFBRect.wBottom = (_bTHUMBTemp+1)*2;
             GDI_FBFillRect(__HALJPEGSetting.bFrameBuffer, &_THUMBPSFBRect, dwColor);
        }
        break;
    case SETUP_CUSTOM_UI_STYLE_1:
    default:
        bLine = HALJPEG_DISPLAY_BUF_HEIGHT/THUMB_BACKGROUND_PALETTE_NUM;

        for (_bTHUMBTemp = 0; _bTHUMBTemp < THUMB_BACKGROUND_PALETTE_NUM; _bTHUMBTemp++)
        {
            _THUMBPSFBRect.wTop = _bTHUMBTemp*bLine;
            _THUMBPSFBRect.wBottom = (_bTHUMBTemp+1)*bLine;

            GDI_FBFillRect(__HALJPEGSetting.bFrameBuffer, &_THUMBPSFBRect, _dwTHUMBPalette[_bTHUMBTemp]);
        }

        if ((bLine * THUMB_BACKGROUND_PALETTE_NUM) < HALJPEG_DISPLAY_BUF_HEIGHT)
        {
            _THUMBPSFBRect.wTop = bLine * THUMB_BACKGROUND_PALETTE_NUM;
            _THUMBPSFBRect.wBottom = HALJPEG_DISPLAY_BUF_HEIGHT-1;

            GDI_FBFillRect(__HALJPEGSetting.bFrameBuffer, &_THUMBPSFBRect, _dwTHUMBPalette[THUMB_BACKGROUND_PALETTE_NUM-1]);
        }
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_ClearRegion
//  Abstract    :   Call this funciton to clear the region.
//  Arguments   :   bClearType: THUMB_CLEAR_TITLE_REGION: Clear the "title" region.
//              :               THUMB_CLEAR_PAGE_INFO_REGION: Clear the "Page Info" region.
//              :               THUMB_CLEAR_HELP_REGION: Clear the "Help" region.
//              :               THUMB_CLEAR_BUTTON_REGION: Clear the "button" region.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ClearRegion(BYTE bClearType)
{
    _THUMBRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;

    switch (bClearType)
    {
    case THUMB_CLEAR_TITLE_REGION:
        _THUMBRect.rect.wLeft = THUMB_TITLE_START_H;
        _THUMBRect.rect.wTop = THUMB_TITLE_START_V-_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_TITLE);
        _THUMBRect.rect.wRight = THUMB_PAGE_START_H-1;
        _THUMBRect.rect.wBottom = _THUMBRect.rect.wTop+THUMB_TITLE_FONT_HEIGHT-1;
        break;
    case THUMB_CLEAR_PAGE_INFO_REGION:
    case THUMB_CLEAR_RECOVER_UPPER_REGION:
        _THUMBRect.rect.wLeft = THUMB_PAGE_START_H;
        _THUMBRect.rect.wTop = THUMB_PAGE_START_V-_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_TITLE);
        _THUMBRect.rect.wRight = THUMB_BACKGROUND_RIGHT;
        _THUMBRect.rect.wBottom = _THUMBRect.rect.wTop+THUMB_TITLE_FONT_HEIGHT-1;
        break;
    case THUMB_CLEAR_GENERAL_MODE_FRAME_REGION:
        _THUMBRect.rect.wLeft = THUMB_BACKGROUND_START_H;
        _THUMBRect.rect.wTop = THUMB_PAGE_START_V-_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_TITLE)+THUMB_TITLE_FONT_HEIGHT;
        _THUMBRect.rect.wRight = THUMB_BACKGROUND_RIGHT;
        _THUMBRect.rect.wBottom = _THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_BUTTON)+THUMB_BUTTON_START_V-THUMB_BUTTON_OUTER_INNER_ARC_PIXEL-1;
        break;
    case THUMB_CLEAR_BUTTON_REGION:
        _THUMBRect.rect.wLeft = THUMB_BUTTON_START_H;
        _THUMBRect.rect.wTop = THUMB_BUTTON_START_V+_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_BUTTON);
        _THUMBRect.rect.wRight = THUMB_BACKGROUND_RIGHT;
        _THUMBRect.rect.wBottom = _THUMBRect.rect.wTop+THUMB_BUTTON_HEIGHT-1;
        break;
    default:
        return;
    }

    GDI_FillRect_909(0, &_THUMBRect);
}

//*********************************************************************************************************
//  Function    :   _THUMB_CheckLeftBoundary
//  Abstract    :   This function will check if the highlight exceeds the left item or not.
//  Arguments   :   none.
//  Return		:   TRUE ==> exceed left item.
//              :   FALSE ==> not exceed left item.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_CheckLeftBoundary(void)
{
    _bTHUMBTemp = _wTHUMBIndex / THUMB_COLUMN_NUM;

    if (_wTHUMBIndex > (_bTHUMBTemp * THUMB_COLUMN_NUM))
        return FALSE;

    return TRUE;
}

//*********************************************************************************************************
//  Function    :   _THUMB_CheckRightBoundary
//  Abstract    :   This function will check if the highlight exceeds the right item or not.
//  Arguments   :   none.
//  Return		:   TRUE ==> exceed right item.
//              :   FALSE ==> not exceed right item.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_CheckRightBoundary(void)
{
    if (((_wTHUMBIndex % THUMB_COLUMN_NUM + 1) >= THUMB_COLUMN_NUM) ||
        ((_wTHUMBIndex+1) >= _wTHUMBTotalFiles))
        return TRUE;
    else if (!_bTHUMBStop)
    {
        if (_wTHUMBPlayIndex <= _wTHUMBIndex) //CoCo1.05, don't move the highlight when the picture is not decoded.
        return TRUE;
    }

    return FALSE;
}

//*********************************************************************************************************
//  Function    :   _THUMB_CheckUpBoundary
//  Abstract    :   This function will check if the highlight exceeds the up item or not.
//  Arguments   :   none.
//  Return		:   TRUE ==> exceed up item.
//              :   FALSE ==> not exceed up item.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_CheckUpBoundary(void)
{
#ifdef SUPPORT_THUMB_PROGRAM
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST) //"Program List" mode
    {
        _bTHUMBTemp = _bTHUMBProgramListPageNo;
    }
    else //general mode
#endif //#ifdef SUPPORT_THUMB_PROGRAM
    {
        _bTHUMBTemp = _bTHUMBPageNO;
    }

    if (_wTHUMBIndex < (THUMB_COLUMN_NUM + _bTHUMBTemp * THUMB_TOTAL_NUM_IN_PAGE))
        return TRUE;

    return FALSE;
}

//*********************************************************************************************************
//  Function    :   _THUMB_CheckDownBoundary
//  Abstract    :   This function will check if the highlight exceeds the down item or not.
//  Arguments   :   none.
//  Return		:   TRUE ==> exceed down item.
//              :   FALSE ==> not exceed down item.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_CheckDownBoundary(void)
{
#ifdef SUPPORT_THUMB_PROGRAM
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST) //"Program List" mode
    {
        _bTHUMBTemp = _bTHUMBProgramListPageNo;
    }
    else //general mode
#endif //#ifdef SUPPORT_THUMB_PROGRAM
    {
        _bTHUMBTemp = _bTHUMBPageNO;
    }

    if (((_wTHUMBIndex + THUMB_COLUMN_NUM) >= ((_bTHUMBTemp+1)*THUMB_TOTAL_NUM_IN_PAGE)) ||
        ((_wTHUMBIndex + THUMB_COLUMN_NUM) >=  _wTHUMBTotalFiles))
        return TRUE;

    return FALSE;
}

//*********************************************************************************************************
//  Function    :   _THUMB_GetButtonPos
//  Abstract    :   Call this function to get the real position of the button in OSD buffer.
//  Arguments   :   bPosition:The index of the button
//  Return		:   The button's horizontal position.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
WORD _THUMB_GetButtonPos(BYTE bButtonIndex)
{
    _wTHUMBTemp = THUMB_BUTTON_START_H;

    for (_bTHUMBTemp = 0; _bTHUMBTemp < bButtonIndex; _bTHUMBTemp++)
    {
        _wTHUMBTemp += THUMB_BUTTON_WIDTH;
        _wTHUMBTemp += THUMB_BUTTON_DISTANCE;
    }

    return _wTHUMBTemp;
}

//*********************************************************************************************************
//  Function    :   _THUMB_DrawButton
//  Abstract    :   This function will draw the specified button.
//  Arguments   :   bButtonIndex: The index of the button.
//              :   bButtonState: The state of the button.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_DrawButton(BYTE bButtonIndex, BYTE bButtonState)
{
    /*
    if (bButtonState == THUMB_BUTTON_STATE_HIGHLIGHT)
    {
        //Highlighted button.
        _THUMBDrawBitmap.bBitmapID = GDI_BMP_BT_HIGHLIGHT;
    }
    else
    {
        //Normal button.
        _THUMBDrawBitmap.bBitmapID = GDI_BMP_BT_ENABLE;
    }
    _THUMBDrawBitmap.wDestX = _THUMB_GetButtonPos(bButtonIndex);
    _THUMBDrawBitmap.wDestY = THUMB_BUTTON_START_V+_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_BUTTON);
    _THUMBDrawBitmap.bDestRegion = 0;
    GDI_DrawBitmap(&_THUMBDrawBitmap);
    */
    //Draw the rectangle to represent the button. For PC-908 test only.
    _THUMBRect.rect.wLeft = _THUMB_GetButtonPos(bButtonIndex);
    _THUMBRect.rect.wTop = THUMB_BUTTON_START_V+_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_BUTTON);
    _THUMBRect.rect.wRight = _THUMBRect.rect.wLeft+THUMB_BUTTON_WIDTH;
    _THUMBRect.rect.wBottom = _THUMBRect.rect.wTop+THUMB_BUTTON_HEIGHT;

    if (bButtonState == THUMB_BUTTON_STATE_HIGHLIGHT)
    {
        _THUMBRect.bColor = THUMB_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND;
    }
    else
    {
        _THUMBRect.bColor = THUMB_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND;
    }

    GDI_FillRoundRect(0, THUMB_BUTTON_ARC_VALUE, &_THUMBRect);

    //Fill the frame of round rectangle
    _THUMBRect.bColor = THUMB_PALETTE_ENTRY_BUTTON_FRAME;

    _THUMBRect.rect.wLeft -= THUMB_BUTTON_OUTER_INNER_ARC_PIXEL;
    _THUMBRect.rect.wTop -= THUMB_BUTTON_OUTER_INNER_ARC_PIXEL;
    _THUMBRect.rect.wRight += THUMB_BUTTON_OUTER_INNER_ARC_PIXEL;
    _THUMBRect.rect.wBottom += THUMB_BUTTON_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, THUMB_BUTTON_OUTER_ARC_VALUE, THUMB_BUTTON_ARC_VALUE, &_THUMBRect);


    _THUMB_SetNormalStringAttribute();

    if (bButtonState == THUMB_BUTTON_STATE_HIGHLIGHT)
    {
        _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_BUTTON_STRING_HIGHLIGHT;
        _THUMBStringAttr.bBackground = THUMB_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND;
    }
    else //"enabled" button
    {
        _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_BUTTON_STRING_ENABLE;
        _THUMBStringAttr.bBackground = THUMB_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND;
    }

    _THUMB_FillButtonName(THUMBButton[_bTHUMBSubMode][bButtonIndex]);

    _THUMBStringAttr.wX = _THUMB_GetButtonPos(bButtonIndex)+((THUMB_BUTTON_WIDTH-GDI_GetStringWidth_909(__pOSDUTLOutputString))>>1);
    _THUMBStringAttr.wY = _THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_BUTTON)+THUMB_BUTTON_START_V+((THUMB_BUTTON_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString))>>1);

    //GDI_DrawString_909(0, &_THUMBStringAttr, __pOSDUTLOutputString);
    _THUMB_DrawString();
}

//*********************************************************************************************************
//  Function    :   _THUMB_FillButtonName
//  Abstract    :   This function will fill the button's name.
//  Arguments   :   bButtonID: The button's ID.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_FillButtonName(BYTE bButtonID)
{
    switch (bButtonID)
    {
    case THUMB_BUTTON_ROTATE:
        __pOSDUTLOutputString = aTHUMBButtonRotate;
        break;
    case THUMB_BUTTON_GENERAL_SLIDESHOW:
    case THUMB_BUTTON_PROGRAM_SLIDESHOW:
    case THUMB_BUTTON_PROGRAMLIST_SLIDESHOW:
    case THUMB_BUTTON_SORT_PLAY:
        __pOSDUTLOutputString = aTHUMBButtonSlideShow;
        break;
    case THUMB_BUTTON_RESET:
        __pOSDUTLOutputString = aTHUMBButtonReset;
        break;
    case THUMB_BUTTON_ROTATE_RETURN:
    case THUMB_BUTTON_PROGRAMLIST_RETURN:
    case THUMB_BUTTON_DELETE_RETURN:
    case THUMB_BUTTON_SORT_RETURN:
        __pOSDUTLOutputString = aTHUMBButtonReturn;
        break;
    case THUMB_BUTTON_PROGRAMLIST:
        __pOSDUTLOutputString = aTHUMBButtonProgramList;
        break;
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
    case THUMB_BUTTON_EDIT_DELETE:
        __pOSDUTLOutputString = aBUTTONDelete;
        break;
    case THUMB_BUTTON_EDIT_SORT:
        __pOSDUTLOutputString = aTHUMBButtonSort;
        break;
    case THUMB_BUTTON_DELETE_DELETEALL:
        __pOSDUTLOutputString = aTHUMBButtonDeleteAll;
        break;
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI
    default:
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_ProcessButton
//  Abstract    :   This function will process the button when it is pressend.
//  Arguments   :   none.
//  Return		:   KEY_NO_KEY: The key is processced.
//              :   INVALID_KEY: It is an invalid key.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_ProcessButton(void)
{
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
    BYTE i;
    BYTE bTotalFile;
#endif //

    switch (THUMBButton[_bTHUMBSubMode][_bTHUMBButtonIndex])
    {
    case THUMB_BUTTON_ROTATE:
        //enter rotate mode.
        _bTHUMBSubMode = THUMB_SUBMODE_ROTATE;
        _THUMB_EnterNewMode();
        break;
    case THUMB_BUTTON_GENERAL_SLIDESHOW:
        //Leave Thumbnail mode and start to slide show.
        _THUMB_ToSlideShow(THUMB_PLAY_START);
        return KEY_BYPASS;
    case THUMB_BUTTON_RESET:
        //Check all thumbnails in the page to see whether it is rotated.
        //If "yes", recover it to unrotated mode.
        //If "no", skip it and check next thumbnail.
        _THUMB_ResetRotateState();
        break;
    case THUMB_BUTTON_ROTATE_RETURN:
        //change it to general mode.
        _bTHUMBSubMode = THUMB_SUBMODE_GENERAL;

        //Recover the decoding mode
        _bTHUMBDecodeMode = THUMB_DECODE_PAGE;
        _THUMB_EnterNewMode();
        break;
    case THUMB_BUTTON_PROGRAM_SLIDESHOW:
    case THUMB_BUTTON_PROGRAMLIST_SLIDESHOW:
#ifdef SUPPORT_THUMB_PROGRAM
        //Play the programmed items. If no programmed files, play the normal slide show.
        if (__bProgramMaxEntry == 0)
        {
            //play the normal slide show.
            //CoCo1.10, fix the bug about can't decode JPEG OK if no programmed items and press the "play" button.
            LINEAR_Program(LINEAR_PROGRAM_CLEAR, (BYTE)NULL);
        }
        _THUMB_ToSlideShow(THUMB_PLAY_START);
        return KEY_BYPASS;
#endif //#ifdef SUPPORT_THUMB_PROGRAM
    case THUMB_BUTTON_PROGRAMLIST:
#ifdef SUPPORT_THUMB_PROGRAM
        //enter "program list" UI.
        _bTHUMBSubMode = THUMB_SUBMODE_PROGRAM_LIST;

        _bTHUMBProgramListPageNo = 0;
        _wTHUMBPlayIndex = 0;
        //update the cursor
        _wTHUMBIndex = 0;

        //Clear the thumbnails in the frame buffer
        _THUMB_ClearScreen();

        //clear the thumbnail's frame
        _THUMB_ClearRegion(THUMB_CLEAR_GENERAL_MODE_FRAME_REGION);

        _THUMB_EnterNewMode();

        if (_wTHUMBTotalFiles != 0)
        {
            //Start to decode the thumbnails
            _THUMB_InitialStage();
        }

        // wyc0.51-909, rename comment.
        // wyc2.38a-repeat,
        // wyc2.53-909P,
        MM_PreparePage(MM_PREPARE_PROGLIST,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
#endif //#ifdef SUPPORT_THUMB_PROGRAM
        break;
    case THUMB_BUTTON_PROGRAMLIST_RETURN:
#ifdef SUPPORT_THUMB_PROGRAM
        //clear all thumbnails in the this page.
        _THUMB_ClearScreen();

        //clear OSD buffer for this page.
        _THUMB_ClearRegion(THUMB_CLEAR_GENERAL_MODE_FRAME_REGION);

        //return to "Porgram Mode"
        _bTHUMBSubMode = THUMB_SUBMODE_PROGRAM;

        //recover the playing index to the general mode
        _wTHUMBPlayIndex = _bTHUMBPageNO * THUMB_TOTAL_NUM_IN_PAGE;

        //update the cursor
        _wTHUMBIndex = _wTHUMBPlayIndex;

        //Start to decode the thumbnails in program mode.
        _THUMB_InitialStage();
        _THUMB_EnterNewMode();
#endif //#ifdef SUPPORT_THUMB_PROGRAM
        break;
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
    case THUMB_BUTTON_EDIT_DELETE:
        _bTHUMBSubMode = THUMB_SUBMODE_DELETE_MODE;
        _THUMB_EnterNewMode();
        break;
    case THUMB_BUTTON_EDIT_SORT:
        _bTHUMBSubMode = THUMB_SUBMODE_SORT_MODE;
        _THUMB_EnterNewMode();
        break;
    case THUMB_BUTTON_DELETE_DELETEALL:
        bTotalFile = __bMMJPGEncodeNum;

        for (i = 0; i < bTotalFile; i++)
        {
            MM_EncodeFile_Delete(0); 
        }

        _wTHUMBTotalFiles = 0;

        _THUMB_ClearScreen();
        _bTHUMBStop = TRUE;

        _bTHUMBCursorType = THUMB_CURSOR_BUTTON;
        //move to the "return" button
        //Recover the original highlight button to normal state.
        _THUMB_DrawButton(_bTHUMBButtonIndex, THUMB_BUTTON_STATE_ENABLE);
        
        //Update new highlight button.
        _bTHUMBButtonIndex++;
        _THUMB_DrawButton(_bTHUMBButtonIndex, THUMB_BUTTON_STATE_HIGHLIGHT);        
        break;
    case THUMB_BUTTON_DELETE_RETURN:
        if (__bMMJPGEncodeNum == 0)
        {
            THUMB_NormalExit();
            POWERONMENU_Initial();
            break;
        }
        else
        {
            _bTHUMBSubMode = THUMB_SUBMODE_EDIT_MODE;
            _THUMB_EnterNewMode();
        }
        break;
    case THUMB_BUTTON_SORT_RETURN:     
        _bTHUMBStartSort = FALSE;
#ifdef THUMB_NO_EDIT_MODE
        _bTHUMBSubMode = THUMB_SUBMODE_DELETE_MODE;
#else
        _bTHUMBSubMode = THUMB_SUBMODE_EDIT_MODE;
#endif //THUMB_NO_EDIT_MODE
        _THUMB_EnterNewMode();
        break;
    case THUMB_BUTTON_SORT_PLAY:
        if (__bMMJPGEncodeNum == 0)
        {
            return INVALID_KEY;
        }
        else
        {
            DISP_DisplayCtrl(DISP_OSD, FALSE);        
            DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE); 
            THUMB_NormalExit(); 
            DISP_DisplayCtrl(DISP_OSD, TRUE); 
            DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE); 
#ifdef SUPPORT_FAVORITE_FUNCTION
            __wDiscType = BOOK_M1;
            __wPlayItem = 1;
            __SF_SourceGBL[0].bSourceIndex = SOURCE_SPI;
			__bModeCmd = KEY_PLAY;
            UTL_PlayItem(1, 0);
#else
            __bOSDSSPicIdx = 0;
            OSDSS_Entry();
#endif //SUPPORT_FAVORITE_FUNCTION
        }
        break;
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI
    default:
        return INVALID_KEY;
    }

    return KEY_NO_KEY;
}

//*********************************************************************************************************
//  Function    :   _THUMB_EnterNewMode
//  Abstract    :   This function will do initialization for new mode.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_EnterNewMode(void)
{
    //CoCo1.11, to prevent thumbnail is not decoded OK when changing mode frequently. For example, Program<==>Program.
    HALJPEG_Reset();

    _THUMB_UpdateTitle();

#ifndef CT950_STYLE
    _THUMB_GetTotalFiles();
#endif //CT950_STYLE

#ifdef SUPPORT_THUMB_PROGRAM
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST)
    {
        _wTHUMBPlayIndex = _bTHUMBProgramListPageNo * THUMB_TOTAL_NUM_IN_PAGE; //re-decode
        _THUMB_DisplayPageInfo();
    }
    else
#endif //#ifdef SUPPORT_THUMB_PROGRAM
    {
        _wTHUMBPlayIndex = _bTHUMBPageNO * THUMB_TOTAL_NUM_IN_PAGE;
    }

    _bTHUMBButtonIndex = 0;

    _THUMB_ClearRegion(THUMB_CLEAR_BUTTON_REGION);

    if (_wTHUMBTotalFiles == 0)
    {
        _bTHUMBCursorType = THUMB_CURSOR_BUTTON;

        _THUMB_DrawButton(0, THUMB_BUTTON_STATE_HIGHLIGHT);
    }
    else
    {
        _bTHUMBCursorType = THUMB_CURSOR_PICTURE;

        _THUMB_DrawButton(0, THUMB_BUTTON_STATE_ENABLE);

        //just change each mode, not first enter thumbnail mode.
        if (_bTHUMBStop)
            _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);
    }

    _THUMB_DrawButton(1, THUMB_BUTTON_STATE_ENABLE);

    //CoCo1.10, draw help region for updating the progress bar when entering thumbnail mode.
    _THUMB_DrawHelpRegion();

    if ((_bTHUMBSubMode == THUMB_SUBMODE_GENERAL) || (_bTHUMBSubMode == THUMB_SUBMODE_EDIT_MODE))
    {
        //CoCo1.10, fix the bug about the "Program" symbol is not cleared.
        //Clear the region before re-draw the frame because the "P" for program need to be cleared.
        _THUMB_ClearProgramSymbol((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));

        if (_bTHUMBStop)
        {
            _THUMB_ShowGeneralHelp((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
        }
    }
    else
    {
#ifdef THUMB_NO_EDIT_MODE
        if (_bTHUMBSubMode == THUMB_SUBMODE_DELETE_MODE)
        {
            _THUMB_ClearProgramSymbol((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
        }
#endif //THUMB_NO_EDIT_MODE

        _THUMB_UpdateHelp();
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_UpdateTitle
//  Abstract    :   This funciton will update the title according to the mode.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_UpdateTitle(void)
{
    _THUMB_ClearRegion(THUMB_CLEAR_TITLE_REGION);

    _THUMB_SetNormalStringAttribute();
    _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_TITLE_STRING;
    _THUMBStringAttr.wX = THUMB_TITLE_START_H;
    _THUMBStringAttr.wY = THUMB_TITLE_START_V-_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_TITLE);

    switch (_bTHUMBSubMode)
    {
    case THUMB_SUBMODE_GENERAL:
        __pOSDUTLOutputString = aTHUMBTitleGeneralMode;
        break;
    case THUMB_SUBMODE_ROTATE:
        __pOSDUTLOutputString = aTHUMBTitleRotateMode;
        break;
#ifdef SUPPORT_THUMB_PROGRAM
    case THUMB_SUBMODE_PROGRAM:
        __pOSDUTLOutputString = aTHUMBTitleProgramMode;
        break;
    case THUMB_SUBMODE_PROGRAM_LIST:
        __pOSDUTLOutputString = aTHUMBTitleProgramList;
        break;
#endif //#ifdef SUPPORT_THUMB_PROGRAM
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
    case THUMB_SUBMODE_EDIT_MODE:
        __pOSDUTLOutputString = aTHUMBTitleEditMode;
        break;
    case THUMB_SUBMODE_DELETE_MODE:
        __pOSDUTLOutputString = aTHUMBTitleDeleteMode;
        break;
    case THUMB_SUBMODE_SORT_MODE:
        __pOSDUTLOutputString = aTHUMBTitleSortMode;
        break;
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI
    default:
        break;
    }

    //GDI_DrawString_909(0, &_THUMBStringAttr, __pOSDUTLOutputString);
    _THUMB_DrawString();
}

//*********************************************************************************************************
//  Function    :   _THUMB_UpdateHelp
//  Abstract    :   This funciton will show the help according to the position of the cursor.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_UpdateHelp(void)
{
    //clear help region
    _THUMB_DrawHelpRegion();

    _THUMBStringAttr.wX = THUMB_HELP_STRING_START_H;

    //Draw the "play" icon first except the general mode.
    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aTHUMBHelpPlayIcon);
    switch (_bTHUMBSubMode)
    {
    case THUMB_SUBMODE_ROTATE:
        OSDUTL_CopyStringFromIndex(aTHUMBHelpRotateMode);
        break;
#ifdef SUPPORT_THUMB_PROGRAM
    case THUMB_SUBMODE_PROGRAM:
        if (!_bTHUMBStop) //CoCo1.11, to prevent the help exists with the progress bar.
            return;
        OSDUTL_CopyStringFromIndex(aTHUMBHelpProgramMode);
        break;
    case THUMB_SUBMODE_PROGRAM_LIST:
        OSDUTL_CopyStringFromIndex(aTHUMBHelpProgramList);
        break;
#endif //#ifdef SUPPORT_THUMB_PROGRAM
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
    case THUMB_SUBMODE_DELETE_MODE:
        OSDUTL_CopyStringFromIndex(aTHUMBHelpDeleteMode);
        break;
    case THUMB_SUBMODE_SORT_MODE:
        OSDUTL_CopyStringFromIndex(aTHUMBHelpSortMode);
        break;
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI
    default:
        return;
    }

    _THUMB_SetNormalStringAttribute();
    _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_HELP_STRING;
    _THUMBStringAttr.bBackground = THUMB_PALETTE_ENTRY_HELP_REGION_BACKGROUND;

    _THUMBStringAttr.wY = THUMB_HELP_START_V+_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_HELP)+((THUMB_HELP_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);

    //GDI_DrawString_909(0, &_THUMBStringAttr, __pOSDUTLOutputString);
    _THUMB_DrawString();
}

//*********************************************************************************************************
//  Function    :   _THUMB_SetNormalStringAttribute
//  Abstract    :   Set the common attribute for normal string.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_SetNormalStringAttribute(void)
{
    _THUMBStringAttr.bBackground = PAL_ENTRY_COLOR_TRANSPARENT;
    _THUMBStringAttr.bColorKey = 0;
    _THUMBStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    _THUMBStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
}

//*********************************************************************************************************
//  Function    :   THUMB_Exit
//  Abstract    :   This function will prepare all things to exit thumbnail mode.
//  Arguments   :   bExitMode: OSD_UI_EXIT_ALL/OSD_UI_EXIT_CONTROL_ONLY.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void THUMB_Exit(BYTE bExitMode)
{
    HALJPEG_Reset();

    if (bExitMode == OSD_UI_EXIT_ALL)
    {
        //clear all OSD
        GDI_ClearRegion(0); //clear region 0
    }

    __bThumbnailMode = FALSE;

    //assign other frame to prevent to see the garbage.
    //RISC will use the other frame to do some operation like "rotate" or "flip".
    //Therefore, we can't keep the previous image to do the effect because it will be destroyed by RISC.
    __bMMAtiveFrame = !__bMMAtiveFrame;

    _THUMB_JPEG_PSFB(TRUE, 0); //clear all frame buffer.

    // wyc0.51-909, rename it.
    __bMMStage=MM_STAGE_NONE;
    __btPlaying = FALSE;
    __bCountStop = 2; //For FManager UI reference.

    /*
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
    HALJPEG_SetDisplay();
#endif
    */
}


//*********************************************************************************************************
//  Function    :   _THUMB_ResetRotateState
//  Abstract    :   This function will reset all rotated thumbnails to unrotated state.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ResetRotateState(void)
{
    //Reset the state for all thumbnails in this folder and save them to DRAM.
    for (_wTHUMBTemp = 0; _wTHUMBTemp < _wTHUMBTotalFiles; _wTHUMBTemp++)
    {
        //skip current page.
        if ((_wTHUMBTemp >= _bTHUMBPageNO * THUMB_TOTAL_NUM_IN_PAGE) && (_wTHUMBTemp < ((_bTHUMBPageNO+1) * THUMB_TOTAL_NUM_IN_PAGE)))
            continue;

        _wTHUMBTemp1 = _wTHUMBStartPlayItem + _wTHUMBTemp + 1;

        MM_AccessJPGStatus(MM_GET_JPG_STATUS, _wTHUMBTemp1);

        __bMMJPGStatus = HALJPEG_OPERATION_I;

        // Store the final rotate value to DRAM
        MM_AccessJPGStatus(MM_STORE_JPG_STATUS, _wTHUMBTemp1);
    }

    //Use _wTHUMBIndex as the index of the thumbnail we want to rotate
    _wTHUMBIndex = _bTHUMBPageNO * THUMB_TOTAL_NUM_IN_PAGE;
    _wTHUMBPlayIndex = _wTHUMBIndex;

    if (_THUMB_SearchNextValidPic())
    {
        //Initialize the decoding mode.
        _bTHUMBDecodeMode = THUMB_DECODE_UNDETERMINED;

        //Initialize the decoding stage and then start to decode the thumbnails.
        _THUMB_InitialStage();
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_UpdateThumbnailFrame
//  Abstract    :   Call this function to update the frame of the thumbnail.
//  Arguments   :   bType: THUMB_UPDATE_FRAME_GENERAL_TO_PROGRAM: Draw the frames
//              :          from general mode to Program mode.
//              :          THUMB_UPDATE_FRAME_PROGRAM_TO_GENERAL: Draw the frames
//              :          from Program mode to general mode.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_UpdateThumbnailFrame(BYTE bType)
{
    //calculate the total thumbnails in this page.
    _wTHUMBTemp = _bTHUMBPageNO * THUMB_TOTAL_NUM_IN_PAGE;

    if ((_wTHUMBTemp + THUMB_TOTAL_NUM_IN_PAGE) > _wTHUMBTotalFiles)
        _wTHUMBTemp = _wTHUMBTotalFiles % THUMB_TOTAL_NUM_IN_PAGE;
    else
        _wTHUMBTemp = THUMB_TOTAL_NUM_IN_PAGE;

    for (_bTHUMBTemp = 0; _bTHUMBTemp < (BYTE)_wTHUMBTemp; _bTHUMBTemp++)
    {
        if (bType == THUMB_UPDATE_FRAME_GENERAL_TO_PROGRAM)
        {
            //search if "_bTHUMBPageNO * THUMB_TOTAL_NUM_IN_PAGE+_bTHUMBTemp" is in the program list.
            if (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, (BYTE)__MMSelectCursor.wDirID, (WORD)(_wTHUMBStartPlayItem+_bTHUMBPageNO * THUMB_TOTAL_NUM_IN_PAGE+_bTHUMBTemp+1)) != 0xFF) //not found
                _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_PROGRAM);
        }
        else if (bType == THUMB_UPDATE_FRAME_PROGRAM_TO_GENERAL)
        {
            _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_NORMAL);
        }
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_ProcessEnterKey
//  Abstract    :   This function will process the "ENTER" key for all mode.
//  Arguments   :   none.
//  Return		:   KEY_NO_KEY: The key is processced.
//              :   INVALID_KEY: It is an invalid key.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_ProcessEnterKey(void)
{
    if (_bTHUMBCursorType == THUMB_CURSOR_PICTURE) //play it directly.
    {
        switch (_bTHUMBSubMode)
        {
        case THUMB_SUBMODE_GENERAL:
            //play the thumbnail directly.
            //leave the thumbnail mode.
            _THUMB_ToSlideShow(THUMB_PLAY_CURRENT);
            return KEY_BYPASS;
        case THUMB_SUBMODE_ROTATE:
            //rotate the thumbnail right 90 degree.
            if (_bTHUMBStop) //Don't allow user to press KEY_ENTER to rotate image before a page is decoded OK.
                _THUMB_Rotate();
            else
                return INVALID_KEY;
            break;
#ifdef SUPPORT_THUMB_PROGRAM
        case THUMB_SUBMODE_PROGRAM:
            //If it's not in the program list, add the thumbnail to the program list.
            if (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, (BYTE)__MMSelectCursor.wDirID, (WORD)(_wTHUMBStartPlayItem+_wTHUMBIndex+1)) == 0xFF) //not found
            {
                if (__bProgramMaxEntry == MAX_PROG_ITEM) //current program items has been the max program item.
                {
                    //show error message.
                    return INVALID_KEY;
                }

                LINEAR_ProgramEntryOperation(LINEAR_ADD_ENTRY_BY_INDEX, __bProgramMaxEntry, (BYTE)__MMSelectCursor.wDirID, (WORD)(_wTHUMBStartPlayItem+_wTHUMBIndex+1));
                __bProgramMaxEntry++;
                //_THUMB_DrawFrame((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE), THUMB_FRAME_PROGRAM);

                _THUMB_OutputProgramSymbol((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
            }
            else //it's in program list and delete it from the program list
            {
                LINEAR_ProgramEntryOperation(LINEAR_DELETE_ENTRY_BY_INDEX_W_REARRANGE,
                    LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, (BYTE)__MMSelectCursor.wDirID, (WORD)(_wTHUMBStartPlayItem+_wTHUMBIndex+1)), 0, 0);

                __bProgramMaxEntry--;
                _THUMB_ClearProgramSymbol((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
                //_THUMB_DrawFrame((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE), THUMB_FRAME_NORMAL);
            }
            break;
        case THUMB_SUBMODE_PROGRAM_LIST:
            for (_bTHUMBTemp = (_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE); _bTHUMBTemp < THUMB_TOTAL_NUM_IN_PAGE; _bTHUMBTemp++)
            {
                //clear the region in the display buffer
                _THUMB_ClearFileExtInProgramList((BYTE)(_bTHUMBTemp%THUMB_TOTAL_NUM_IN_PAGE));

                /*
                //Clear the JPEG
                _THUMB_JPEG_PSFB(FALSE, (BYTE)(_bTHUMBTemp%THUMB_TOTAL_NUM_IN_PAGE));

                //clear the frames from the cursor
                _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_CLEAR);
                */
            }

            _THUMB_ClearScreen();

            _wTHUMBPlayIndex = _bTHUMBProgramListPageNo * THUMB_TOTAL_NUM_IN_PAGE;

            //Delete the thumbnail from the program list.
            LINEAR_ProgramEntryOperation(LINEAR_DELETE_ENTRY_BY_INDEX_W_REARRANGE, (BYTE)_wTHUMBIndex, 0, 0);
            __bProgramMaxEntry--;

            _wTHUMBTotalFiles--;

            //There is no programmed thumbnail or all thumbnails has been deleted in this page.
            if ((_wTHUMBTotalFiles == 0) || (_wTHUMBPlayIndex == _wTHUMBTotalFiles))
            {
                _THUMB_ProcessEmptyProgramList();
            }
            else if (_wTHUMBPlayIndex < _wTHUMBTotalFiles)
            {
                //To prevent cursor at an empty thumbnail
                if (_wTHUMBIndex > (_wTHUMBTotalFiles - 1))
                {
                    //_THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_CLEAR);
                    _wTHUMBIndex = _wTHUMBTotalFiles - 1;
                }

                //_wTHUMBPlayIndex = _wTHUMBIndex;

                _wTHUMBPlayIndex = 0;

                //Need to prepare the content of the Program List for EXT name
                // wyc2.38a-repeat
                MM_PreparePage(MM_PREPARE_PROGLIST,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);

                //Start to decode the thumbnails
                _THUMB_InitialStage();
            }
            break;
#endif //#ifdef SUPPORT_THUMB_PROGRAM
#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
        case THUMB_SUBMODE_EDIT_MODE:
            //Enter slide show and play the picture from the index.
            DISP_DisplayCtrl(DISP_OSD, FALSE);        
            DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE); 
            THUMB_NormalExit(); 
            DISP_DisplayCtrl(DISP_OSD, TRUE); 
            DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE); 
#ifdef SUPPORT_FAVORITE_FUNCTION
            __wDiscType = BOOK_M1;
            __wPlayItem = _wTHUMBIndex+1;
            __SF_SourceGBL[0].bSourceIndex = SOURCE_SPI;
			__bModeCmd = KEY_PLAY;
            UTL_PlayItem(_wTHUMBIndex+1, 0);
#else
            __bOSDSSPicIdx = _wTHUMBIndex; 
            OSDSS_Entry();            
#endif //SUPPORT_FAVORITE_FUNCTION
            break;
        case THUMB_SUBMODE_DELETE_MODE:
            //delete current picture.
            MM_EncodeFile_Delete(_wTHUMBIndex); 
            _wTHUMBTotalFiles--;

            //Re-decode from _wTHUMBIndex to the (__bMMJPGEncodeNum-1)
            //Start to decode the thumbnails            
            if (_wTHUMBTotalFiles > 0)
            {
                //Clear the frame.
                _THUMB_ClearScreen();

                //To prevent the cursor is at the empty picture.
                if (_wTHUMBIndex == (_wTHUMBTotalFiles))
                {
                    _wTHUMBIndex--;
                }

                if (((_wTHUMBIndex+1) % THUMB_TOTAL_NUM_IN_PAGE) == 0)
                {
                    if (_bTHUMBPageNO > 0)
                    {
                        _bTHUMBPageNO--;
                    }
                }

                //re-decode from the 1st picture to prevent the frame can't be cleared.
                _wTHUMBPlayIndex = _bTHUMBPageNO * THUMB_TOTAL_NUM_IN_PAGE;
                _THUMB_InitialStage();
            }
            else
            {
                //Move the cursor to the "return" button
                _THUMB_ClearScreen();

                _bTHUMBCursorType = THUMB_CURSOR_BUTTON;
                _bTHUMBButtonIndex = 1;
                _THUMB_DrawButton(_bTHUMBButtonIndex, THUMB_BUTTON_STATE_HIGHLIGHT);
            }
            break;
        case THUMB_SUBMODE_SORT_MODE:
            _bTHUMBStartSort = !_bTHUMBStartSort;

            if (_bTHUMBStartSort)
            {
                _THUMB_DrawFrame(_wTHUMBIndex, THUMB_FRAME_PROGRAM);
            }
            else
            {
                _THUMB_DrawFrame(_wTHUMBIndex, THUMB_FRAME_NORMAL);
            }

            break;
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI
        default:
            break;
        }
    }
    else //the cursor is at the button
        return _THUMB_ProcessButton();

    return KEY_NO_KEY;
}

//*********************************************************************************************************
//  Function    :   _THUMB_ProcessEmptyProgramList
//  Abstract    :   This function will do the things when it is empty in the Program List.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ProcessEmptyProgramList(void)
{
#ifdef SUPPORT_THUMB_PROGRAM
    //Change the cursor to the button part.
    _bTHUMBCursorType = THUMB_CURSOR_BUTTON;

    //clear frames
    _THUMB_ClearRegion(THUMB_CLEAR_GENERAL_MODE_FRAME_REGION);

    if (_wTHUMBTotalFiles == 0)
    {
        //Set the cursor to the "return" button
        _bTHUMBButtonIndex = 1;
        _THUMB_DrawButton(_bTHUMBButtonIndex, THUMB_BUTTON_STATE_HIGHLIGHT);

        //Don't display the page info if no file in the program list.
        _THUMB_DisplayPageInfo();
    }
#endif //#ifdef SUPPORT_THUMB_PROGRAM
}

//*********************************************************************************************************
//  Function    :   _THUMB_ClearFileExtInProgramList
//  Abstract    :   Call this function to clear the EXT in the Program List.
//  Arguments   :   bIndex: The index of the file.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ClearFileExtInProgramList(BYTE bIndex)
{
    _THUMB_GetPosition(bIndex);

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
    _wTHUMBHStartPos -= _bTHUMBTuneFrame_H+_bTHUMBTuneOSDOffset_H;
    _wTHUMBVStartPos -= _bTHUMBTuneFrame_V+_bTHUMBTuneOSDOffset_V;

    _wTHUMBHStartPos += (bIndex%THUMB_COLUMN_NUM)*_bTHUMBTuneOSDDistance_H;
    _wTHUMBVStartPos += (bIndex/THUMB_COLUMN_NUM)*_bTHUMBTuneOSDDistance_V;
#endif

    _THUMBRect.rect.wLeft = _wTHUMBHStartPos - _bTHUMBTuneFrame_H+THUMB_FRAME_THICKNESS;
    _THUMBRect.rect.wRight = _THUMBRect.rect.wLeft + THUMB_WIDTH- (WORD)(THUMB_FRAME_THICKNESS << 1);

    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC)
    {
        _THUMBRect.rect.wTop = _wTHUMBVStartPos - _bTHUMBTuneFrame_V;
        _THUMBRect.rect.wBottom = _THUMBRect.rect.wTop + THUMB_HEIGHT_NTSC;
    }
    else //PAL
    {
        _THUMBRect.rect.wTop = _wTHUMBVStartPos - _bTHUMBTuneFrame_V+(THUMB_TUNE_DISTANCE_HEIGHT_PAL* ((bIndex % THUMB_TOTAL_NUM_IN_PAGE) / THUMB_COLUMN_NUM));
        _THUMBRect.rect.wBottom = _THUMBRect.rect.wTop + THUMB_HEIGHT_PAL;
    }

    _THUMBRect.rect.wTop += THUMB_FRAME_THICKNESS;
    _THUMBRect.rect.wBottom -= (THUMB_FRAME_THICKNESS << 1);

    _THUMBRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;

    GDI_FillRect_909(0, &_THUMBRect);
}

//*********************************************************************************************************
//  Function    :   _THUMB_Rotate
//  Abstract    :   This function will do rotation according to the argument.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_Rotate(void)
{
    _wTHUMBPlayIndex = _wTHUMBIndex;

#ifdef CT950_STYLE  
    _THUMB_CheckDumpFileRecord(TRUE);
#endif //CT950_STYLE

#ifdef CT950_STYLE
	MM_AccessJPGStatus(MM_GET_JPG_STATUS, _THUMB_GetPlayIndex());
#else
    MM_AccessJPGStatus(MM_GET_JPG_STATUS, (WORD)(_wTHUMBStartPlayItem+_wTHUMBIndex+1));
#endif //CT950_STYLE

    // Get JPG file status info.
    __bMMJPGStatus = HALJPEG_TransformOperation(__bMMJPGStatus, HALJPEG_OPERATION_RC);

    // Store the final rotate value to DRAM
#ifdef CT950_STYLE
	MM_AccessJPGStatus(MM_STORE_JPG_STATUS, _THUMB_GetPlayIndex());
#else
    MM_AccessJPGStatus(MM_STORE_JPG_STATUS, (WORD)(_wTHUMBStartPlayItem+_wTHUMBIndex+1));
#endif //CT950_STYLE

    _bTHUMBDecodeMode = THUMB_DECODE_ONE;

    _THUMB_InitialStage();
}

//*********************************************************************************************************
//  Function    :   _THUMB_ToSlideShow
//  Abstract    :   This function will leave the thumbnail mode and go to slide show mode.
//  Arguments   :   THUMB_PLAY_START: Slide show from the first file of the folder.
//              :   THUMB_PLAY_CURRENT: Slide show form the current selected file.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ToSlideShow(BYTE bType)
{
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
    BYTE bFoundAudio;
    WORD wPlayItemIndex;
    WORD wAudioFileNum = 0; 
    extern BYTE __bMMAutoPlay;
    extern BYTE __bMMAudioPlayEnd;
    extern BYTE __bMMAudioStage;
    extern BYTE __bMMStartJPEGAudioTrigger;
    extern PTREE_INFO  __pMMVolumeDirTree;
#endif //
    extern BYTE _MM_FindJPEGFile(WORD wDirID);

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
    DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
#endif
    __bMMExtFilterMode = __bMMExtFilterModeInMMMode;

    CC_KeyCommand (KEY_STOP);

    switch (bType)
    {
    case THUMB_PLAY_START:
#ifdef CT950_STYLE
		_wTHUMBPlayIndex = 0;
        _THUMB_CheckDumpFileRecord(TRUE);
#endif //CT950_STYLE
        __wPlayItem = _wTHUMBStartPlayItem+1; //from the first File.
        if (__btProgram)
        {
            __bProgramPlayingIndex = 0;
            __btPlayingProgram = TRUE;
        }
        break;
    case THUMB_PLAY_CURRENT:
#ifdef CT950_STYLE
		_wTHUMBPlayIndex = _wTHUMBIndex;
        _THUMB_CheckDumpFileRecord(TRUE);
		__wPlayItem = _THUMB_GetPlayIndex();
#else
        __wPlayItem = _wTHUMBStartPlayItem+_wTHUMBIndex + 1;
#endif //CT950_STYLE
        break;
    default:
        break;
    }    

#ifdef SUPPORT_PHOTO_BROWSE_MODE
    if (__bMMPhotoBrowseMode)
    {
        __bMMPhotoBrowseMode = FALSE;

        //Set JPEG relative setting always.
        DVDSETUP_SetSlideShowIntervalTime();
        DVDSETUP_SetSlideShowEffect();
        
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__bMMEnterBrowseFromJPEGAudioMode)
        {
            __bMMExtFilterMode = EXT_BIT_JPG|EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA;
            
            __bMMJPEGAudioMode = TRUE;
            __bMMAutoPlay = TRUE;
            __bRepeat = REPEAT_DISC;
            
            
            MM_PreparePage(MM_PREPARE_FILEPAGE, __MMPlayingItem.wDirID, EXT_BIT_JPG|EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA, __bMMSortMethod);
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
            
            _wTHUMBPlayIndex = _wTHUMBIndex;
            _THUMB_CheckDumpFileRecord(TRUE);
            wPlayItemIndex = _THUMB_GetPlayIndex()-1;
            
            MM_PreparePage(MM_PREPARE_FILEPAGE, __MMPlayingItem.wDirID, EXT_BIT_JPG|EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA, __bMMSortMethod);
            MM_ParseDirFiles(__MMPlayingItem.wDirID, EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA, 0);
            wAudioFileNum = __pMMFilterFilesOfDir[__MMPlayingItem.wDirID];
            
            _MM_FindJPEGFile(__MMPlayingItem.wDirID);
            
            __wPlayItem = wPlayItemIndex-__pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir+1;
            __MMPlayingItem.wIndex = __pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir+wPlayItemIndex+wAudioFileNum;
            
			MM_ParseDirFiles(0, EXT_BIT_JPG|EXT_BIT_MP3|EXT_BIT_MP2|EXT_BIT_WMA, 0); //prepare for audio file.

            __bMMVideoStreamPause = FALSE;
            __bMMAudioStreamPause = FALSE;
            __bMMStartJPEGAudioTrigger = FALSE;
        }
#endif
    }
#endif //SUPPORT_PHOTO_BROWSE_MODE    

    THUMB_NormalExit();

    //CoCo0.88, fix the bug about the one-line display is wrong because of KEY_ENTER
    __bKey = KEY_PLAY;
    _CC_MapKeyMessage(KEY_PLAY);
}

//*********************************************************************************************************
//  Function    :   _THUMB_JPEG_PSFB
//  Abstract    :   Call this function to do JPEG PSFB
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_JPEG_PSFB(BYTE bAllRegion, BYTE bIndex)
{
    if (bAllRegion)
    {
        _THUMBPSFBRect.wLeft = 0;
        _THUMBPSFBRect.wTop = 0;
        _THUMBPSFBRect.wRight = HALJPEG_DISPLAY_BUF_WIDTH - 1;
        _THUMBPSFBRect.wBottom = HALJPEG_DISPLAY_BUF_HEIGHT - 1;
    }
    else
    {
        _THUMB_GetPosition(bIndex);

        _THUMBPSFBRect.wLeft = _wTHUMBHStartPos;
        _THUMBPSFBRect.wTop = _wTHUMBVStartPos;
        _THUMBPSFBRect.wRight = _THUMBPSFBRect.wLeft + THUMB_WIDTH - 1;
        _THUMBPSFBRect.wBottom = _THUMBPSFBRect.wTop + THUMB_HEIGHT_NTSC - 1;
    }

    JPEG_PresetFrameBuf(__HALJPEGSetting.bFrameBuffer, &_THUMBPSFBRect, COMUTL_RGB2YUV(THUMB_BACKGROUND_COLOR));

    _dwTHUMBCheckTime=OS_GetSysTimer();
    while((OS_GetSysTimer()-_dwTHUMBCheckTime) < COUNT_3_SEC)
    {
        if (HALJPEG_Status(HALJPEG_PSFB) == HALJPEG_STATUS_OK)
        {
            break;
        }

        OS_YieldThread();
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_ShowFileExtInProgramList
//  Abstract    :   Call this function to show the file's extention in the Program List UI.
//  Arguments   :   bIndex: The index of the file.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ShowFileExtInProgramList(BYTE bIndex)
{
    //search what kind of file in the program list.
    if (__pMMProgContent[bIndex].bExtID != EXTNAME_JPG)
    {
        _THUMB_GetPosition(bIndex);

        _THUMBStringAttr.bBackground = PAL_ENTRY_COLOR_TRANSPARENT;
        _THUMBStringAttr.bColorKey = 0;

        switch (__pMMProgContent[bIndex].bExtID)
        {
        case EXTNAME_MP2:
            __pOSDUTLOutputString = aTHUMBExtMP2;
            _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_MP2_ICON_TEXT_FOREGROUND;
            _THUMBStringAttr.bShadow1 = THUMB_PALETTE_ENTRY_MP2_ICON_SHADOW_1;
            _THUMBStringAttr.bShadow2 = THUMB_PALETTE_ENTRY_MP2_ICON_SHADOW_2;
            break;
        case EXTNAME_MP3:
            __pOSDUTLOutputString = aTHUMBExtMP3;
            _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_MP3_ICON_TEXT_FOREGROUND;
            _THUMBStringAttr.bShadow1 = THUMB_PALETTE_ENTRY_MP3_ICON_SHADOW_1;
            _THUMBStringAttr.bShadow2 = THUMB_PALETTE_ENTRY_MP3_ICON_SHADOW_2;
            break;
        case EXTNAME_DAT:
            __pOSDUTLOutputString = aTHUMBExtDAT;
            _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_DAT_ICON_TEXT_FOREGROUND;
            _THUMBStringAttr.bShadow1 = THUMB_PALETTE_ENTRY_DAT_ICON_SHADOW_1;
            _THUMBStringAttr.bShadow2 = THUMB_PALETTE_ENTRY_DAT_ICON_SHADOW_2;
            break;
        case EXTNAME_VOB:
            __pOSDUTLOutputString = aTHUMBExtVOB;
            _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_VOB_ICON_TEXT_FOREGROUND;
            _THUMBStringAttr.bShadow1 = THUMB_PALETTE_ENTRY_VOB_ICON_SHADOW_1;
            _THUMBStringAttr.bShadow2 = THUMB_PALETTE_ENTRY_VOB_ICON_SHADOW_2;
            break;
        case EXTNAME_AVI:
            __pOSDUTLOutputString = aTHUMBExtMPEG4;
            _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_MP4_ICON_TEXT_FOREGROUND;
            _THUMBStringAttr.bShadow1 = THUMB_PALETTE_ENTRY_MP4_ICON_SHADOW_1;
            _THUMBStringAttr.bShadow2 = THUMB_PALETTE_ENTRY_MP4_ICON_SHADOW_2;
            break;
        case EXTNAME_WMA:
            __pOSDUTLOutputString = aTHUMBExtWMA;
            _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_WMA_ICON_TEXT_FOREGROUND;
            _THUMBStringAttr.bShadow1 = THUMB_PALETTE_ENTRY_WMA_ICON_SHADOW_1;
            _THUMBStringAttr.bShadow2 = THUMB_PALETTE_ENTRY_WMA_ICON_SHADOW_2;
            break;
        case EXTNAME_MPG:
            __pOSDUTLOutputString = aTHUMBExtMPG;
            _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_MPG_ICON_TEXT_FOREGROUND;
            _THUMBStringAttr.bShadow1 = THUMB_PALETTE_ENTRY_MPG_ICON_SHADOW_1;
            _THUMBStringAttr.bShadow2 = THUMB_PALETTE_ENTRY_MPG_ICON_SHADOW_2;
            break;
        default:
            return;
        }

        _THUMBStringAttr.wX = _wTHUMBHStartPos + ((THUMB_WIDTH - GDI_GetStringWidth_909(__pOSDUTLOutputString)) >> 1) - _bTHUMBTuneFrame_H;

        if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC)
        {
            _THUMBStringAttr.wY = _wTHUMBVStartPos+((THUMB_HEIGHT_NTSC-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1) ;
        }
        else
        {
            _THUMBStringAttr.wY = _wTHUMBVStartPos+(THUMB_TUNE_DISTANCE_HEIGHT_PAL* ((bIndex % THUMB_TOTAL_NUM_IN_PAGE) / THUMB_COLUMN_NUM))+((THUMB_HEIGHT_PAL-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);
        }

        _THUMBStringAttr.wY -= _bTHUMBTuneFrame_V;

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
        _THUMBStringAttr.wX -= _bTHUMBTuneFrame_H+_bTHUMBTuneOSDOffset_H;
        _THUMBStringAttr.wY -= _bTHUMBTuneFrame_V+_bTHUMBTuneOSDOffset_V;

        _THUMBStringAttr.wX += (bIndex%THUMB_COLUMN_NUM)*_bTHUMBTuneOSDDistance_H;
        _THUMBStringAttr.wY += (bIndex/THUMB_COLUMN_NUM)*_bTHUMBTuneOSDDistance_V;
#endif


        //GDI_DrawString_909(0, &_THUMBStringAttr, __pOSDUTLOutputString);
        _THUMB_DrawString();
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_Decode
//  Abstract    :   Decode the thumbnail.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_Decode(void)
{
    _THUMB_GetPosition((BYTE)(_wTHUMBPlayIndex % THUMB_TOTAL_NUM_IN_PAGE));

// wyc2.17-909S, need to ise frame buffer 0 when single buffer mode. Because frame 1 set to thumb buffer in single buffer mode.
#ifdef JPG_SINGLE_FRAME_BUFFER
    __HALJPEGSetting.bFrameBuffer = 1;
#else
    __HALJPEGSetting.bFrameBuffer = 0;
#endif //
    __HALJPEGSetting.wDisplayStartH = _wTHUMBHStartPos;
    __HALJPEGSetting.wDisplayStartV = _wTHUMBVStartPos;
    __HALJPEGSetting.wDisplayWidth = THUMB_WIDTH;
    __HALJPEGSetting.wDisplayHeight = THUMB_HEIGHT_NTSC;
    __HALJPEGSetting.dwBackgroundColor = COMUTL_RGB2YUV(THUMB_BACKGROUND_COLOR);

    HALJPEG_Decode();
}

//*********************************************************************************************************
//  Function    :   THUMB_Recover
//  Abstract    :   Call this function to do recovery.
//  Arguments   :   bRecoverRegion: OSD_RECOVER_UPPER_REGION/OSD_RECOVER_BOTTOM_REGION.
//  Return		:   TRUE: The region is recovered.
//              :   FALSE: The region doesn't need to be recovered in THUMB UI.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE THUMB_Recover(BYTE bRecoverRegion)
{
    //Set variables of frame position
    _THUMB_TuneFramePosition();

    if (bRecoverRegion == OSD_RECOVER_UPPER_REGION)
    {
        //show page info.
        _THUMB_DisplayPageInfo();
    }
    else if (bRecoverRegion == OSD_RECOVER_BOTTOM_REGION)
    {
        if ((_bTHUMBSubMode == THUMB_SUBMODE_GENERAL) || (_bTHUMBSubMode == THUMB_SUBMODE_EDIT_MODE))
        {
            if (_bTHUMBStop)
            {
                _THUMB_ShowGeneralHelp((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
            }
        }
        else
        {
            _THUMB_UpdateHelp();
        }
    }
    else if (bRecoverRegion == OSD_RECOVER_ALL)
    {
        _THUMB_RecoverAll();
    }

    return TRUE;
}

//*********************************************************************************************************
//  Function    :   _THUMB_RecoverAll
//  Abstract    :   Call this function to redraw all OSD.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_RecoverAll(void)
{
    BYTE bIndex;

    GDI_ClearRegion(0); //clear region 0

    //Tell OSD the position to show display mode message is changed.
    _THUMB_InitializeNDAttribute();

    //Call this function to re-initialize GDI to prevent the palette is destroyed by other UI.
    _THUMB_InitializeGDI();

    //CoCo1.0
    if (__bSETUPUIStyleChanged)
    {
        _THUMB_ChangePageAction(THUMB_CURRENT_PAGE);
        __bSETUPUIStyleChanged = FALSE;
    }

    //Draw the frames cleared by other UI.
#ifdef SUPPORT_THUMB_PROGRAM
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST)
    {
        _wTHUMBTemp = _bTHUMBProgramListPageNo * THUMB_TOTAL_NUM_IN_PAGE;
    }
    else
#endif //#ifdef SUPPORT_THUMB_PROGRAM
        _wTHUMBTemp = _bTHUMBPageNO * THUMB_TOTAL_NUM_IN_PAGE;

    //calculate the total thumbnails in this page.
    if ((_wTHUMBTemp + THUMB_TOTAL_NUM_IN_PAGE) > _wTHUMBTotalFiles)
        _wTHUMBTemp1 = _wTHUMBTotalFiles;
    else
        _wTHUMBTemp1 = _wTHUMBTemp + THUMB_TOTAL_NUM_IN_PAGE;

    if (!_bTHUMBStop)
        _wTHUMBTemp2 = _wTHUMBPlayIndex; //Enter other UI when thumbnails are not completed in a page.
    else
        _wTHUMBTemp2 = _wTHUMBTemp1;

    while (_wTHUMBTemp < _wTHUMBTemp2)
    {
        _bTHUMBTemp = (BYTE)(_wTHUMBTemp % THUMB_TOTAL_NUM_IN_PAGE);

#ifdef SUPPORT_THUMB_PROGRAM
        if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM)
        {
            if (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, (BYTE)__MMSelectCursor.wDirID, (WORD)(_wTHUMBStartPlayItem+_wTHUMBTemp+1)) == 0xFF) //not found
                _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_NORMAL);
            else
                _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_PROGRAM);
        }
        else
#endif //#ifdef SUPPORT_THUMB_PROGRAM
        {
#ifdef SUPPORT_THUMB_PROGRAM
            if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST)
            {
                //show EXT in the frame.
                if (__pMMProgContent[_wTHUMBTemp].bExtID != EXTNAME_JPG)
                    _THUMB_ShowFileExtInProgramList((BYTE)(_wTHUMBTemp%THUMB_TOTAL_NUM_IN_PAGE));
            }
#endif //#ifdef SUPPORT_THUMB_PROGRAM

            _THUMB_DrawFrame(_bTHUMBTemp, THUMB_FRAME_NORMAL);
        }

        _wTHUMBTemp++;
    }

    //Update title, page info, button and help
    _THUMB_UpdateTitle();

    for (bIndex = 0; bIndex < 2; bIndex++)
    {
        if (_bTHUMBCursorType == THUMB_CURSOR_BUTTON)
        {
            if (_bTHUMBButtonIndex == bIndex)
                _THUMB_DrawButton(bIndex, THUMB_BUTTON_STATE_HIGHLIGHT);
            else
                _THUMB_DrawButton(bIndex, THUMB_BUTTON_STATE_ENABLE);
        }
        else
            _THUMB_DrawButton(bIndex, THUMB_BUTTON_STATE_ENABLE);
    }

    if (_bTHUMBCursorType == THUMB_CURSOR_PICTURE)
        _THUMB_UpdateHighlight(THUMB_UPDATE_HIGHLIGHT_DRAW);

    _THUMB_DisplayPageInfo();

    //Need to re-draw the help region bitmap
    _THUMB_DrawHelpRegion();

    if ((_bTHUMBSubMode == THUMB_SUBMODE_GENERAL) || (_bTHUMBSubMode == THUMB_SUBMODE_EDIT_MODE))
    {
        if (_bTHUMBStop)
        {
            _THUMB_ShowGeneralHelp((BYTE)(_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE));
        }
    }
    else
    {
        _THUMB_UpdateHelp();
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_InitializeNDAttribute
//  Abstract    :   This function will initialize the normal upper/bottom string attribute.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_InitializeNDAttribute(void)
{
    //Set the color of the background, color of the text, and vertical starting position for upper right message.
    __OSDNDUpMsgAttr.wX = THUMB_PAGE_END_H;
    __OSDNDUpMsgAttr.wY = THUMB_PAGE_START_V-_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_TITLE);
    __OSDNDUpMsgAttr.bBackground = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDUpMsgAttr.bTextColor = THUMB_PALETTE_ENTRY_TITLE_STRING;
	__OSDNDUpMsgAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
	__OSDNDUpMsgAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
	__OSDNDUpMsgAttr.bColorKey = 0;

    __OSDNDBottomMsgAttr.wX = THUMB_HELP_START_H+THUMB_HELP_STRING_REGION_DISTANCE;
    __OSDNDBottomMsgAttr.wY = THUMB_HELP_START_V+_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_HELP);
    __OSDNDBottomMsgAttr.bBackground = THUMB_PALETTE_ENTRY_HELP_REGION_BACKGROUND;
    __OSDNDBottomMsgAttr.bTextColor = THUMB_PALETTE_ENTRY_HELP_STRING;
    __OSDNDBottomMsgAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDBottomMsgAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDBottomMsgAttr.bColorKey = 0;
}


//*********************************************************************************************************
//  Function    :   THUMB_ClearRegion
//  Abstract    :   This function will be called by OSD to clear the normal upper region or normal bottom region
//              :   if the UI accepts the normal display message is displayed in its UI.
//  Arguments   :   bClearRegion: OSD_ND_UPPER_RIGHT_REGION/OSD_ND_BOTTOM_REGION.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void THUMB_ClearRegion(BYTE bClearRegion)
{
    if (bClearRegion == OSD_ND_UPPER_RIGHT_REGION)
    {
        _THUMB_ClearRegion(THUMB_CLEAR_RECOVER_UPPER_REGION);
    }
    else if (bClearRegion == OSD_ND_BOTTOM_REGION)
    {
        _THUMB_DrawHelpRegion();
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_ParseHeader
//  Abstract    :
//  Arguments   :
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ParseHeader(void)
{
    //Stop video decoder, reset buffer and reset parser
    //HAL_Reset(HAL_RESET_VIDEO);

    //REG_PLAT_RESET_CONTROL &= ~(PLAT_RESET_VDEC);   // disable the VDEC
    //REG_PLAT_RESET_CONTROL |= PLAT_RESET_VDEC;      //enable the VDEC

    HALJPEG_Reset();

    HALJPEG_ParseHeader(JPEG_PARSE_TYPE_THUMBNAIL, __HALJPEGSetting.bFrameBuffer);
    //PARSER_ActionFilter(0,PARSER_ENABLE_END_PADDING,TRUE);
}

//*********************************************************************************************************
//  Function    :   THUMB_NormalExit
//  Abstract    :   Exit the Thumbnail UI normally.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void THUMB_NormalExit(void)
{
#ifdef SUPPORT_PHOTO_BROWSE_MODE
    MM_ExitPhotoBrowseMode();
#endif //SUPPORT_PHOTO_BROWSE_MODE

    THUMB_Exit(OSD_UI_EXIT_ALL);

    //tell OSD exit thumbnail mode
    OSD_ChangeUI(OSD_UI_THUMBNAIL, OSD_EXIT_UI);

    if (!_bTHUMBStop)
        OSD_Output(MSG_PROGRESS, 0, 0); //CoCo1.10
}

//*********************************************************************************************************
//  Function    :   _THUMB_GetJPEGInfo
//  Abstract    :   This function will Get JPEG relative info. from JPEG Decoder
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_GetJPEGInfo(BYTE bIndex)
{
    _JPEGInfo[bIndex].wHResolution = __wJPEGFMTExifImageWidth;
    _JPEGInfo[bIndex].wVResolution = __wJPEGFMTExifImageHeight;

	if (__cJPEGFMTExifDate[0] == '\0')
	{
		_THUMB_GetJPEGFileDate(bIndex);
	}
	else
	{
		for (_bTHUMBTemp = 0; _bTHUMBTemp < 11; _bTHUMBTemp++)
		{
			_JPEGInfo[bIndex].cDate[_bTHUMBTemp] = __cJPEGFMTExifDate[_bTHUMBTemp];
		}

		if ((__cJPEGFMTExifDate[0] == '0') && (__cJPEGFMTExifDate[1] == '0') && (__cJPEGFMTExifDate[2] == '0') && (__cJPEGFMTExifDate[3] == '0'))
		{
			_JPEGInfo[bIndex].cDate[0] = '\0';

			_THUMB_GetJPEGFileDate(bIndex);
		}
	}
}

void _THUMB_GetJPEGFileDate(BYTE bIndex)
{
#ifdef CT950_STYLE
	_dwTHUMBTemp = MM_ID_RelativeToAbsolute(__MMPlayingItem.wDirID, _wTHUMBPlayItemInFolder-1);
#else
	_dwTHUMBTemp = MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID, _wTHUMBStartPlayItem+_wTHUMBPlayIndex);
#endif //CT950_STYLE
	
	__pMMFileRecord = __pMMFileRecordStart;
	__pMMFileRecord += _dwTHUMBTemp;
	
	_JPEGInfo[bIndex].dwYear = MM_GetFileInfo(MM_GET_FILE_YEAR);
	_JPEGInfo[bIndex].dwMonth = MM_GetFileInfo(MM_GET_FILE_MONTH);
	_JPEGInfo[bIndex].dwDate = MM_GetFileInfo(MM_GET_FILE_DATE);
}

//*********************************************************************************************************
//  Function    :   _THUMB_ResetJPEGInfo
//  Abstract    :   This function will reset JPEG info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ResetJPEGInfo(void)
{
    for (_bTHUMBTemp = 0; _bTHUMBTemp < THUMB_TOTAL_NUM_IN_PAGE; _bTHUMBTemp++)
    {
        _JPEGInfo[_bTHUMBTemp].wHResolution = 0;
        _JPEGInfo[_bTHUMBTemp].wVResolution = 0;

        _JPEGInfo[_bTHUMBTemp].cDate[0] = '\0';
    }
}


//*********************************************************************************************************
//  Function    :   _THUMB_ShowGeneralHelp
//  Abstract    :   This function will show JPEG's resolution and date in the info. region.
//  Arguments   :   bIndex: the index of the thumbnail.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ShowGeneralHelp(BYTE bIndex)
{
    if ((_bTHUMBSubMode == THUMB_SUBMODE_GENERAL) || (_bTHUMBSubMode == THUMB_SUBMODE_EDIT_MODE))
    {
        //CoCo1.10, if thumbnails are not decoded OK, we will show progress bar. Don't show the resolution info. at this time.
        if (!_bTHUMBStop)
            return;

        //clear help region
        _THUMB_DrawHelpRegion();

        _THUMB_SetNormalStringAttribute();
        _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_HELP_STRING;
        _THUMBStringAttr.bBackground = THUMB_PALETTE_ENTRY_HELP_REGION_BACKGROUND;
        _THUMBStringAttr.wY = THUMB_HELP_START_V+_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_HELP)+((THUMB_HELP_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);

        //Show JPEG resolution and file name.
        if ((_JPEGInfo[bIndex].wHResolution != 0) && (_JPEGInfo[bIndex].wVResolution != 0))
        {
            OSDUTL_PrepareOutputString();

            OSDUTL_OutputNumFromIndex(_JPEGInfo[bIndex].wHResolution);
            OSDUTL_CopyStringFromIndex(aTHUMBX);
            OSDUTL_OutputNumFromIndex(_JPEGInfo[bIndex].wVResolution);

            _THUMBStringAttr.wX = THUMB_HELP_STRING_START_H;
            //GDI_DrawString_909(0, &_THUMBStringAttr, __pOSDUTLOutputString);
            _THUMB_DrawString();
        }

        _bTHUMBTemp = 0;
		OSDUTL_PrepareOutputString();

        if (_JPEGInfo[bIndex].cDate[_bTHUMBTemp] != '\0')
        {
            while (_JPEGInfo[bIndex].cDate[_bTHUMBTemp] != '\0')
            {
                __wOSDUTLStringBuf[_bTHUMBTemp+1] = OSDUTL_TransformChar(_JPEGInfo[bIndex].cDate[_bTHUMBTemp]);
                if (__wOSDUTLStringBuf[_bTHUMBTemp+1] == CHAR_COLON)
                {
                    __wOSDUTLStringBuf[_bTHUMBTemp+1] = CHAR_SOLIDUS;
                }
                _bTHUMBTemp++;
            }

            __wOSDUTLStringBuf[0] = _bTHUMBTemp;
        }
		else
		{
			OSDUTL_OutputNumFromIndex(_JPEGInfo[bIndex].dwYear);
			OSDUTL_CopyStringFromIndex(aTHUMBSlash);
			OSDUTL_OutputNumFromIndex(_JPEGInfo[bIndex].dwMonth);
			OSDUTL_CopyStringFromIndex(aTHUMBSlash);
			OSDUTL_OutputNumFromIndex(_JPEGInfo[bIndex].dwDate);
		}

		_THUMBStringAttr.wX = THUMB_HELP_START_H+THUMB_HELP_WIDTH-THUMB_HELP_STRING_REGION_DISTANCE-GDI_GetStringWidth_909(__wOSDUTLStringBuf);
		GDI_DrawString_909(0, &_THUMBStringAttr, __wOSDUTLStringBuf);
    }
}

//*********************************************************************************************************
//  Function    :   _THUMB_DrawHelpRegion
//  Abstract    :   This function will draw the help region.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_DrawHelpRegion(void)
{
    _THUMBRect.rect.wLeft = THUMB_HELP_START_H;
    _THUMBRect.rect.wTop = THUMB_HELP_START_V+_THUMB_TuneForTVType(THUMB_TUNE_POS_TYPE_HELP);
    _THUMBRect.rect.wRight = THUMB_HELP_START_H+THUMB_HELP_WIDTH;
    _THUMBRect.rect.wBottom = _THUMBRect.rect.wTop+THUMB_HELP_HEIGHT-1;
    _THUMBRect.bColor = THUMB_PALETTE_ENTRY_HELP_REGION_BACKGROUND;

    GDI_FillRoundRect(0, THUMB_HELP_REGION_ARC_VALUE, &_THUMBRect);

    //Fill the frame of round rectangle
    _THUMBRect.bColor = THUMB_PALETTE_ENTRY_HELP_REGION_FRAME;

    _THUMBRect.rect.wLeft -= THUMB_HELP_REGION_OUTER_INNER_ARC_PIXEL;
    _THUMBRect.rect.wTop -= THUMB_HELP_REGION_OUTER_INNER_ARC_PIXEL;
    _THUMBRect.rect.wRight += THUMB_HELP_REGION_OUTER_INNER_ARC_PIXEL;
    _THUMBRect.rect.wBottom += THUMB_HELP_REGION_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, THUMB_HELP_REGION_OUTER_ARC_VALUE, THUMB_HELP_REGION_ARC_VALUE, &_THUMBRect);
}

//*********************************************************************************************************
//  Function    :   _THUMB_OutputProgramSymbol
//  Abstract    :   This function will output program symbol
//  Arguments   :   bIndex: the index of the thumbnail.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_OutputProgramSymbol(BYTE bIndex)
{
#ifdef SUPPORT_THUMB_PROGRAM

    return;

    _THUMB_GetPosition(bIndex);

    _wTHUMBHStartPos -= _bTHUMBTuneFrame_H;
    _wTHUMBVStartPos -= _bTHUMBTuneFrame_V;

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
    _wTHUMBHStartPos -= _bTHUMBTuneFrame_H+_bTHUMBTuneOSDOffset_H;
    _wTHUMBVStartPos -= _bTHUMBTuneFrame_V+_bTHUMBTuneOSDOffset_V;

    _wTHUMBHStartPos += (bIndex%THUMB_COLUMN_NUM)*_bTHUMBTuneOSDDistance_H;
    _wTHUMBVStartPos += (bIndex/THUMB_COLUMN_NUM)*_bTHUMBTuneOSDDistance_V;
#endif


    if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
    {
        _THUMBStringAttr.wX = _wTHUMBHStartPos+((THUMB_WIDTH+THUMB_TUNE_DISPLAY_WIDTH_NTSC-GDI_GetStringWidth_909(aTHUMBProgramSymbol))>>1);
        _THUMBStringAttr.wY = _wTHUMBVStartPos+((THUMB_HEIGHT_NTSC+THUMB_TUNE_DISPLAY_HEIGHT_NTSC-GDI_GetStringHeight_909(aTHUMBProgramSymbol))>>1);
    }
    else //PAL
    {
        _wTHUMBVStartPos += (THUMB_TUNE_DISTANCE_HEIGHT_PAL) * (bIndex / THUMB_COLUMN_NUM);
        _THUMBStringAttr.wX = _wTHUMBHStartPos+((THUMB_WIDTH+THUMB_TUNE_DISPLAY_WIDTH_PAL-GDI_GetStringWidth_909(aTHUMBProgramSymbol))>>1);
        _THUMBStringAttr.wY = _wTHUMBVStartPos+((THUMB_HEIGHT_NTSC+THUMB_TUNE_DISPLAY_HEIGHT_PAL-GDI_GetStringHeight_909(aTHUMBProgramSymbol))>>1);
    }

    _THUMB_SetNormalStringAttribute();
    _THUMBStringAttr.bTextColor = THUMB_PALETTE_ENTRY_PROGRAM_SYMBOL;
    GDI_DrawString_909(0, &_THUMBStringAttr, aTHUMBProgramSymbol);

#endif //#ifdef SUPPORT_THUMB_PROGRAM
}


//*********************************************************************************************************
//  Function    :   _THUMB_ClearProgramSymbol
//  Abstract    :   This function will clear program symbol
//  Arguments   :   bIndex: the index of the thumbnail.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void _THUMB_ClearProgramSymbol(BYTE bIndex)
{
#ifdef SUPPORT_THUMB_PROGRAM
    _THUMB_GetPosition(bIndex);
    _wTHUMBHStartPos -= _bTHUMBTuneFrame_H;
    _wTHUMBVStartPos -= _bTHUMBTuneFrame_V;

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
    _wTHUMBHStartPos -= _bTHUMBTuneFrame_H+_bTHUMBTuneOSDOffset_H;
    _wTHUMBVStartPos -= _bTHUMBTuneFrame_V+_bTHUMBTuneOSDOffset_V;

    _wTHUMBHStartPos += (bIndex%THUMB_COLUMN_NUM)*_bTHUMBTuneOSDDistance_H;
    _wTHUMBVStartPos += (bIndex/THUMB_COLUMN_NUM)*_bTHUMBTuneOSDDistance_V;
#endif

    _THUMBRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _THUMBRect.rect.wLeft = _wTHUMBHStartPos;
    _THUMBRect.rect.wTop = _wTHUMBVStartPos;

    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC)
    {
        _THUMBRect.rect.wRight = _wTHUMBHStartPos+THUMB_WIDTH+THUMB_TUNE_DISPLAY_WIDTH_NTSC;
        _THUMBRect.rect.wBottom = _wTHUMBVStartPos+THUMB_HEIGHT_NTSC+THUMB_TUNE_DISPLAY_HEIGHT_NTSC;
    }
    else //PAL
    {
        _wTHUMBVStartPos += (THUMB_TUNE_DISTANCE_HEIGHT_PAL) * (bIndex / THUMB_COLUMN_NUM);
        _THUMBRect.rect.wRight = _wTHUMBHStartPos+THUMB_WIDTH+THUMB_TUNE_DISPLAY_WIDTH_PAL;
        _THUMBRect.rect.wBottom = _wTHUMBVStartPos+THUMB_HEIGHT_NTSC+THUMB_TUNE_DISPLAY_HEIGHT_PAL;
    }

    GDI_FillRect_909(0, &_THUMBRect);

#endif //#ifdef SUPPORT_THUMB_PROGRAM
}

void _THUMB_DrawString(void)
{
    GDI_DrawString_909(0, &_THUMBStringAttr, __pOSDUTLOutputString);
}

//*********************************************************************************************************
//  Function    :   THUMB_ConfigOSDRegion
//  Abstract    :   This function will config the OSD region. And it is exported for OSD kernel usage.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void THUMB_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;

    RegionInfo.bColorMode = GDI_OSD_8B_MODE;

    RegionInfo.wWidth = THUMB_OSD_REGION_WIDTH;
    RegionInfo.wHeight = THUMB_OSD_REGION_HEIGHT;
    RegionInfo.dwTAddr = DS_OSDFRAME_ST_MM;

    /*
    GDI_DeactivateRegion();
    GDI_ConfigRegionInfo(0, &RegionInfo);

    GDI_InitialRegion(0);

    // Clear region
    GDI_ClearRegion(0);

    GDI_ActivateRegion(0);
    */

    OSD_SetRegion(0, TRUE, &RegionInfo);
}

//*********************************************************************************************************
//  Function    :   _THUMB_ToMMUI
//  Abstract    :   Enter MM UI.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE _THUMB_ToMMUI(void)
{
#ifdef SUPPORT_THUMB_PROGRAM
    if (_bTHUMBSubMode == THUMB_SUBMODE_PROGRAM_LIST)
        return FALSE;
#endif //#ifdef SUPPORT_THUMB_PROGRAM
    
    HALJPEG_Abort();
    
#ifdef CT950_STYLE
    _wTHUMBPlayIndex = _wTHUMBIndex;
    _THUMB_CheckDumpFileRecord(TRUE);
    __wPlayItem = _THUMB_GetPlayIndex();
#else
    __wPlayItem = _wTHUMBStartPlayItem+_wTHUMBIndex + 1;
#endif //CT950_STYLE
    
    THUMB_NormalExit();
    
    MM_ThumbToMMEntry();
    
    return TRUE;
}

void _THUMB_InitializeDirFile(void)
{
	_wTHUMBStartPlayItem = 0;

	_wTHUMBTotalFiles = _THUMB_GetTotalFilesBeforeDirID(__bMMTotalDirs);
 
    if ((__MMPlayingItem.wDirID) == 0 && (__MMPlayingItem.wIndex == 0))
    {
        _wTHUMBIndex = 0;
        for (_wTHUMBTemp = 0; _wTHUMBTemp < __bMMTotalDirs; _wTHUMBTemp++)
        {
            if (MM_ParseDirFiles(_wTHUMBTemp, EXT_BIT_JPG, 0))
            {
                MM_GetDumpedFileRecord();
                __MMPlayingItem.wDirID = _wTHUMBTemp;
                break;
            }
        }
    }
    else
    {
	    _wTHUMBIndex = _THUMB_GetTotalFilesBeforeDirID(__MMPlayingItem.wDirID) + __wPlayItem - 1;
    }
}

void _THUMB_CheckDumpFileRecord(BYTE bChangePage)
{
    BYTE bDumpRecord = FALSE;
    WORD wFileIndex;
    WORD wFiles = 0;
	WORD wFiles1, wFiles2;

    wFileIndex = _wTHUMBStartPlayItem+_wTHUMBPlayIndex;

	if (bChangePage)
	{
		for (_wTHUMBTemp = 0; _wTHUMBTemp < __bMMTotalDirs; _wTHUMBTemp++)
		{
			if (_wTHUMBTemp == (__bMMTotalDirs-1))
			{
				__MMPlayingItem.wDirID = _wTHUMBTemp;
				break;
			}
			else
			{
				wFiles1 = _THUMB_GetTotalFilesBeforeDirID(_wTHUMBTemp+1);	//0~_wTHUMBTemp		
				wFiles2 = _THUMB_GetTotalFilesBeforeDirID(_wTHUMBTemp+2);
			}

			if ((wFileIndex >= wFiles1) && (wFileIndex < wFiles2))
			{
				__MMPlayingItem.wDirID = _wTHUMBTemp+1;
				break;
			}
			else if (wFileIndex < wFiles1)
			{
				__MMPlayingItem.wDirID = _wTHUMBTemp;
				break;
			}
		}

		if (MM_ParseDirFiles(__MMPlayingItem.wDirID, EXT_BIT_JPG, 0))
		{
			MM_GetDumpedFileRecord();
		}
	}
	else
	{
		for (_wTHUMBTemp = 0; _wTHUMBTemp < __bMMTotalDirs; _wTHUMBTemp++)
		{
			if (MM_ParseDirFiles(_wTHUMBTemp, EXT_BIT_JPG, 0))
			{
				// MM_GetDumpedFileRecord will update the __pMMFileRecord. Call MM_GetDumpedFileRecord again will go next file record
				// and update the __pMMFileRecord again.

				if (bDumpRecord)
				{
					MM_GetDumpedFileRecord();
					__MMPlayingItem.wDirID = _wTHUMBTemp;
					return;
				}

				wFiles += __pMMFilterFilesOfDir[_wTHUMBTemp];

				if (wFileIndex == wFiles)
				{
					bDumpRecord = TRUE;
				}
			}
		}
	}       
}

WORD _THUMB_GetPlayIndex(void)
{
    return (_wTHUMBStartPlayItem+_wTHUMBPlayIndex-_THUMB_GetTotalFilesBeforeDirID(__MMPlayingItem.wDirID)+1);
}

WORD _THUMB_GetTotalFilesBeforeDirID(WORD wDirID)
{
	WORD wDirIndex;
	WORD wFiles = 0;

	if (wDirID == 0)
	{
		return 0;
	}

	for (wDirIndex = 0; wDirIndex < wDirID; wDirIndex++)
	{
		if (MM_ParseDirFiles(wDirIndex, EXT_BIT_JPG, 0))
		{                
			wFiles += __pMMFilterFilesOfDir[wDirIndex];            
		}
	}

	return wFiles;
}

#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
BYTE _THUMB_SortToPrevious(void)
{
    if (!_bTHUMBStop)
        return FALSE;

    if (_wTHUMBIndex == 0)
    {
        return FALSE;
    }
    else if ((_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE) == 0)
    {
        //Check change to previous page?
        _bTHUMBPageNO--;
        _wTHUMBPlayIndex = _bTHUMBPageNO*THUMB_TOTAL_NUM_IN_PAGE;
        _bTHUMBSortFileStartIndex = _wTHUMBPlayIndex;
        _bTHUMBSortFileEndIndex = _wTHUMBPlayIndex+THUMB_TOTAL_NUM_IN_PAGE-1;
        _wTHUMBIndex = _bTHUMBSortFileEndIndex;
    }
    else //change to the previous index
    {
        MM_EncodeFile_Exchange(_wTHUMBIndex, _wTHUMBIndex-1);

        _wTHUMBIndex--;
        _wTHUMBPlayIndex = _wTHUMBIndex;
        _bTHUMBSortFileStartIndex = _wTHUMBIndex;
        _bTHUMBSortFileEndIndex = _wTHUMBIndex+1;
    }

    _THUMB_InitialStage();

    return TRUE;
}

BYTE _THUMB_SortToNext(void)
{
    if (!_bTHUMBStop)
        return FALSE;

    if (_wTHUMBIndex == (__bMMJPGEncodeNum-1))
    {
        return FALSE;
    }
    else if ((_wTHUMBIndex % THUMB_TOTAL_NUM_IN_PAGE) == (THUMB_TOTAL_NUM_IN_PAGE-1))
    {
        //Check change to next page?
        if (__bMMJPGEncodeNum > ((_bTHUMBPageNO+1)*THUMB_TOTAL_NUM_IN_PAGE))
        {
            _bTHUMBPageNO++;
            _wTHUMBPlayIndex = _bTHUMBPageNO*THUMB_TOTAL_NUM_IN_PAGE;
            _bTHUMBSortFileStartIndex = _wTHUMBPlayIndex;
            _bTHUMBSortFileEndIndex = _wTHUMBPlayIndex+THUMB_TOTAL_NUM_IN_PAGE-1;
            if (_bTHUMBSortFileEndIndex > (__bMMJPGEncodeNum-1))
            {
                _bTHUMBSortFileEndIndex = __bMMJPGEncodeNum-1;
            }
        }
        else
        {
            return FALSE;
        }
    }
    else //change to the previous index
    {
        MM_EncodeFile_Exchange(_wTHUMBIndex, _wTHUMBIndex+1);
        _wTHUMBIndex++;
        _wTHUMBPlayIndex = _wTHUMBIndex-1;
        _bTHUMBSortFileStartIndex = _wTHUMBIndex-1;
        _bTHUMBSortFileEndIndex = _wTHUMBIndex;
    }
    
    _THUMB_InitialStage();
    
    return TRUE;
}

#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI

//CoCo2.77
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM    
void THUMB_ResetRetryTimes(void)
{
	_bTHUMBRetryTimes = THUMB_REDECODE_TIMES;
}
#endif //PRESTOP_BEFORE_SETUP_AND_PROGRAM

#ifdef SUPPORT_JPEG_BACKGROUND
void THUMB_SetBackgroundImage(PCHAR pcName)
{
	strcpy(cBackgoundName, pcName);
}
#endif //SUPPORT_JPEG_BACKGROUND