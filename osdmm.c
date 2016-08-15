#define OSDMM_MAIN_FILE_DECLARATION

#include "winav.h"
#include "comdef.h"
#include "OSDMM.h"
#include "osdnd.h"
#include "osd.h"
#include "utl.h"
#include "cc.h"
#include "linear.h"
// wyc0.50-909, make infofilter.h before Mmanager.h because of Mmanager.h need to reference some information defined in infofilter.h
#include "infofilter.h"
#include "mmanager.h"
#include "InfoFilter.h"
#include "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include "osdfont.h"
#include "hal.h"
#include "setup.h"
#include "notransl.h"
#include "disp.h"
#include "jpegdec.h"
#include "convchar.h"


#define OSDMM_MAIN_FILE
//#include "string.lan"
#include "OSDString\\strextrn.h"

//********************************************************************************************************
//Declare Skin setting
//********************************************************************************************************
DWORD _dwOSDMMGeneralBackgroundColor[] = {OSDMM_PALETTE_COLOR_GENERAL_BACKGROUND_1, OSDMM_PALETTE_COLOR_GENERAL_BACKGROUND_2, OSDMM_PALETTE_COLOR_GENERAL_BACKGROUND_3, OSDMM_PALETTE_COLOR_GENERAL_BACKGROUND_4};
DWORD _dwOSDMMTitleStringColor[] = {OSDMM_PALETTE_COLOR_TITLE_STRING_1, OSDMM_PALETTE_COLOR_TITLE_STRING_2, OSDMM_PALETTE_COLOR_TITLE_STRING_3, OSDMM_PALETTE_COLOR_TITLE_STRING_4};
DWORD _dwOSDMMTimeStringColor[] = {OSDMM_PALETTE_COLOR_TIME_STRING_1, OSDMM_PALETTE_COLOR_TIME_STRING_2, OSDMM_PALETTE_COLOR_TIME_STRING_3, OSDMM_PALETTE_COLOR_TIME_STRING_4};
DWORD _dwOSDMMBackgroundFrameColor[] = {OSDMM_PALETTE_COLOR_BACKGROUND_FRAME_1, OSDMM_PALETTE_COLOR_BACKGROUND_FRAME_2, OSDMM_PALETTE_COLOR_BACKGROUND_FRAME_3, OSDMM_PALETTE_COLOR_BACKGROUND_FRAME_4};
DWORD _dwOSDMMHelpRegionBackgroundColor[] = {OSDMM_PALETTE_COLOR_HELP_REGION_BACKGROUND_1, OSDMM_PALETTE_COLOR_HELP_REGION_BACKGROUND_2, OSDMM_PALETTE_COLOR_HELP_REGION_BACKGROUND_3, OSDMM_PALETTE_COLOR_HELP_REGION_BACKGROUND_4};
DWORD _dwOSDMMHButtonEnableColor[] = {OSDMM_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_1, OSDMM_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_2, OSDMM_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_3, OSDMM_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_4};
DWORD _dwOSDMMHButtonEnableStringColor[] = {OSDMM_PALETTE_COLOR_ENABLE_BUTTON_STRING_1, OSDMM_PALETTE_COLOR_ENABLE_BUTTON_STRING_2, OSDMM_PALETTE_COLOR_ENABLE_BUTTON_STRING_3, OSDMM_PALETTE_COLOR_ENABLE_BUTTON_STRING_4};
DWORD _dwOSDMMDlgTitleBarColor[] = {OSDMM_PALETTE_COLOR_DLG_TITLE_BAR_1, OSDMM_PALETTE_COLOR_DLG_TITLE_BAR_2, OSDMM_PALETTE_COLOR_DLG_TITLE_BAR_3, OSDMM_PALETTE_COLOR_DLG_TITLE_BAR_4};
DWORD _dwOSDMMDlgOuterFrameColor[] = {OSDMM_PALETTE_COLOR_DLG_OUTER_FRAME_1, OSDMM_PALETTE_COLOR_DLG_OUTER_FRAME_2, OSDMM_PALETTE_COLOR_DLG_OUTER_FRAME_3, OSDMM_PALETTE_COLOR_DLG_OUTER_FRAME_4};
DWORD _dwOSDMMDlgBackgroundColor[] = {OSDMM_PALETTE_COLOR_DLG_BACKGROUND_1, OSDMM_PALETTE_COLOR_DLG_BACKGROUND_2, OSDMM_PALETTE_COLOR_DLG_BACKGROUND_3, OSDMM_PALETTE_COLOR_DLG_BACKGROUND_4};

//********************************************************************************************************
//Declare global variables
//********************************************************************************************************
BYTE __bActiveDirChange;
BYTE _bOSDMMStartDrawDir; //The starting item in the left file window.
WORD __wOSDMMStartDrawItem;
PARM_DRAW_STRING _OSDMMStringAttr;
PARM_FRAME _OSDMMFrame;
PARM_SCROLL_BAR _OSDMMScrollBar;
PARM_RECT _OSDMMRect;
PARM_BITMAP_COPY _OSDMMBitmapCopy;
//PARM_DRAW_BITMAP _OSDMMDrawBitmap;
WORD _wOSDMMTemp;
DWORD _dwOSDMMTemp;
BYTE _bOSDMMFileLength;
BYTE _bOSDMMHaveUpDIR;
BYTE _bOSDMMTemp, _bOSDMMTemp1, _bOSDMMTemp2;
BYTE _bOSDMMIconType;
BYTE _bOSDMMTitleType;
BYTE _bOSDMMUpdate;
BYTE _bOSDMMPlayingTitle;
BYTE _bOSDMMHelpType;
BYTE _bOSDMMShowTime;
DWORD _dwOSDMMFlashTime, _dwOSDMMSpectrumTime;
BYTE _bOSDMMFlash;
WORD *__pOSDMMOutputString;
BYTE _bOSDMMSortFile[] = {MM_SORT_BY_TYPE, MM_SORT_BY_NAME, MM_SORT_BY_TIME, MM_SORT_BY_ASCENDING, MM_SORT_BY_DESCENDING};
SPECTRUM _bOSDMMSpectrum[] = {{OSDMM_AUDIO_SPECTRUM_LINE_0_RANGE, OSDMM_AUDIO_SPECTRUM_LINE_0_COLOR},
                              {OSDMM_AUDIO_SPECTRUM_LINE_1_RANGE, OSDMM_AUDIO_SPECTRUM_LINE_1_COLOR},
                              {OSDMM_AUDIO_SPECTRUM_LINE_2_RANGE, OSDMM_AUDIO_SPECTRUM_LINE_2_COLOR},
                              {OSDMM_AUDIO_SPECTRUM_LINE_3_RANGE, OSDMM_AUDIO_SPECTRUM_LINE_3_COLOR},
                              {OSDMM_AUDIO_SPECTRUM_LINE_4_RANGE, OSDMM_AUDIO_SPECTRUM_LINE_4_COLOR}};

DWORD _dwOSDMMEQLow, _dwOSDMMEQHigh;
DWORD _dwOSDMMNewEQLow, _dwOSDMMNewEQHigh;
DWORD _dwOSDMMEQDropLow, _dwOSDMMEQDropHigh;
BYTE _bOSDMMButtonState[][2] = {{OSDMM_BUTTON_ENABLE, OSDMM_BUTTON_ENABLE},
                                {OSDMM_BUTTON_ENABLE, OSDMM_BUTTON_ENABLE},
                                {OSDMM_BUTTON_ENABLE, OSDMM_BUTTON_ENABLE},
                                {OSDMM_BUTTON_ENABLE, OSDMM_BUTTON_ENABLE}};

DWORD _dwOSDMMAudioFreq, _dwOSDMMAudioBitRate, _dwOSDMMAudioTotalTime;

BYTE _bOSDMMUIMode;
BYTE _bOSDMMSaveFile = FALSE;

extern BYTE __bMMSaveProgress;

#ifdef SUPPORT_COPY_FILE_IN_MM_UI
HDIALOG _hOSDMMSaveFileDlg = DIALOG_HANDLE_INVALID;
WORD aOSDMMPercent[] = {1, CHAR_N_PERCENT};
#endif //SUPPORT_COPY_FILE_IN_MM_UI

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
HDIALOG _hOSDMMSaveDlg = DIALOG_HANDLE_INVALID;
HDIALOG _hOSDMMDeleteDlg = DIALOG_HANDLE_INVALID;
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW

//The following declaraction will refer to MManager. Will remove it.
extern BYTE	__bMMPopMenuID;
extern PDIRNAMEINFO __pMMDirNameList;
extern BYTE    __bMMVolumeSet [1];
extern DWORD   __dwFileTime;
// wyc1.05-909, can't put in mmanager.h, so put here.
extern PTREE_INFO  __pMMVolumeDirTree;

extern BYTE __bMMrogressCnt;

//********************************************************************************************************
//  Function    :   

//  Abstract    :   Update the Media Manager UI according to the message.
//  Arguments   :   OSDMM_MSG_INITIAL: Initialize the Media Manager UI. When Media Manager UI is invoked at 
//                                     first time, Fmanager need to send this message to draw the UI.
//              :   OSDMM_MSG_EXIT: Clear the Media Manager UI.
//              :   OSDMM_MSG_UPDATE_HIGHLIGHT: This message is for updating the cursor. Fmanager can send 
//              :                               this message when the cursor is moving. OSDMM will draw the 
//              :                               cursor according to the cursor's information given by Fmanager.
//              :   OSDMM_MSG_CLEAR_HIGHLIGHT: This message is for clearing the cursor.
//              :   OSDMM_MSG_UPDATE_CHANGE_DIR: Change the directory when the cursor is at the DIR window.
//              :   OSDMM_MSG_UPDATE_PROGRAM_ITEM: Draw the selected frame when the user presses KEY_ENTER to program the item.
//              :   OSDMM_MSG_UPDATE_FILE_WINDOW_NEXT: Change to the next page when user presses KEY_NEXT to show the next page.
//              :   OSDMM_MSG_UPDATE_FILE_WINDOW_PREVIOUS: Change to the previous page when user presses KEY_PREV to show the previous page.
//              :   OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL: Change the DIR level.
//              :   OSDMM_MSG_CHANGE_MODE: Send this message to change normal mode, Program mode and Program List.
//              :   OSDMM_MSG_PLAYING: Update the title to the playing mode. It will show current playing file not the file of cursor's position. 
//              :   OSDMM_MSG_UPDATE_ACTIVE_PLAYING_FILE: Update the active playing file.
//              :   OSDMM_MSG_CLEAR_ACTIVE_PLAYING_FILE: Clear the active playing file to normal state.
//              :   OSDMM_MSG_STOP: Send this message to notify OSDMM when user presses KEY_STOP. 
//              :                   Then OSDMM will update the title and stop flashing the active playing file.
//              :   OSDMM_MSG_SHOW_DLG: Send this message to show dialog.
//              :   OSDMM_MSG_CLEAR_DLG: Send this message to clear dialog.
//              :   OSDMM_MSG_UPDATE_DLG_ITEM: Send this message to update the item of the dialog.
//              :   OSDMM_MSG_REMOVE_ACTIVE_VOLUME: Send this message when user removes the active volume. 
//              :                                   Then UI will enter File Manager's starting menu.  
//              :   OSDMM_MSG_REMOVE_NOT_ACTIVE_VOLUME: Send this message when user removes the non-active 
//              :                                       volume and the non-active volume can be seen at the screen. 
//              :                                       Then only the left file window will be updated.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void OSDMM_Update(BYTE bMessage, BYTE bParam)
{
    switch (bMessage)
    {
    case OSDMM_MSG_INITIAL:
        _bOSDMMUIMode = bParam;
        _OSDMM_Initial(OSDMM_MODE_INITIAL_UI);
        break;
    case OSDMM_MSG_EXIT:
        _OSDMM_Exit();
        break;
    case OSDMM_MSG_UPDATE_HIGHLIGHT:
        _OSDMM_UpdateHighlight(OSDMM_UPDATE_DRAW);
        break;
    case OSDMM_MSG_CLEAR_HIGHLIGHT:
        _OSDMM_UpdateHighlight(OSDMM_UPDATE_CLEAR);
        break;
    case OSDMM_MSG_UPDATE_CHANGE_DIR:
        _OSDMM_ChangeDir();
        break;
    case OSDMM_MSG_UPDATE_PROGRAM_ITEM:
        _OSDMM_UpdateProgramItem();
        break;
    case OSDMM_MSG_UPDATE_FILE_WINDOW_NEXT:
        _OSDMM_ChangePage(OSDMM_CHANGE_PAGE_NEXT);
        break;
    case OSDMM_MSG_UPDATE_FILE_WINDOW_PREVIOUS:
        _OSDMM_ChangePage(OSDMM_CHANGE_PAGE_PREVIOUS);
        break;
    case OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL:
        _OSDMM_ChangeDirLevel();
        break;
    case OSDMM_MSG_CHANGE_MODE:
        _OSDMM_ChangMode(bParam);
        break;
    case OSDMM_MSG_PLAYING: //This message is for audio only
        _OSDMM_UpdatePlayingTitle();

        _OSDMM_ResetAudioSpectrum();

        HAL_WriteAM(HAL_AM_EQ_AMPLITUDE_LEFT_SHIFT, OSDMM_AMPLITUDE_SHIFT_BITS);
        
        //Let OSDMM_Trigger function to flash the playing icon, update time and draw the audio bar.
        _bOSDMMShowTime = TRUE;
        _dwOSDMMFlashTime = OS_GetSysTimer();
        _bOSDMMFlash = FALSE;
        _dwOSDMMSpectrumTime = OS_GetSysTimer();
        if (__bMMMenuID == MM_MENU_NORMAL)
        {
            //_OSDMM_DrawInfoRegion(); //clear help region //--CoCo1.05
        }
        break;
    case OSDMM_MSG_STOP:
        _OSDMM_StopAudioPlayingFile();
        break;
    case OSDMM_MSG_SHOW_DLG:
        if (__bMMPopMenuID == MM_POP_MENU_FILTER)
        {
            _OSDMM_ShowFileTypeDialogBox();
        }
        else if (__bMMPopMenuID == MM_POP_MENU_SORT)
        {
            _OSDMM_ShowSortFileDialogBox();
        }
        break;
    case OSDMM_MSG_CLEAR_DLG:
        //recover the left file window.
        _OSDMM_ClearRegion(OSDMM_LEFT_FILE_WINDOW_REGION);
        _OSDMM_DrawLeftFileWindow();

        //clear the preview window
        _OSDMM_ClearRegion(OSDMM_PREVIEW_REGION);

        //draw vertical separate frame
        _OSDMM_DrawSeparateFrame(OSDMM_VERTICAL_SEPARATE_FRAME);

        //draw horizontal separate frame
        _OSDMM_DrawSeparateFrame(OSDMM_HORIZONTAL_SEPARATE_FRAME);

        //recover the right file window.
        _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_REGION);        
        _OSDMM_DrawRightFileWindow();   
        break;
    case OSDMM_MSG_UPDATE_DLG_ITEM:
        //For "file type" dialog, the position of check/uncheck the item must be the same as the cursor
        if (__bMMPopMenuID == MM_POP_MENU_FILTER)
        {
            _OSDMM_DLG_DrawFileTypeItem((BYTE)__MMSelectCursor.wIndex, TRUE);
        }
        else if (__bMMPopMenuID == MM_POP_MENU_SORT)
        {
            if (__MMSelectCursor.wIndex < 3) //Method
            {
                _OSDMM_DLG_DrawAllMethodItemsInSortFile(TRUE);                
            }
            else //Sequence
            {
                _OSDMM_DLG_DrawAllSequenceItemsInSortFile(TRUE);                                
            }
        }
        break;
    case OSDMM_MSG_REMOVE_ACTIVE_VOLUME:
        //Return to the Media Manager's starting menu.
        _bOSDMMStartDrawDir = 0;
        __wOSDMMStartDrawItem = 0;
        _OSDMM_UpdateTitle();
        _OSDMM_ClearRegion(OSDMM_LEFT_FILE_WINDOW_CONTENT_REGION);
        _OSDMM_DrawLeftFileWindow();

        _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_CONTENT_REGION);
        _OSDMM_DrawRightFileWindow();

        _OSDMM_UpdateHelp(OSDMM_HELP_TYPE_NONE);
        _OSDMM_DrawAllButtons();
        break;
    case OSDMM_MSG_REMOVE_NOT_ACTIVE_VOLUME:
        //Draw the left file window.
        _OSDMM_ClearRegion(OSDMM_LEFT_FILE_WINDOW_CONTENT_REGION);
        _OSDMM_DrawLeftFileWindow();
        break;
    case OSDMM_MSG_UPDATE_HELP:
        if (__bMMMenuID != MM_MENU_PROGLIST)
        {
            if (__bAttrPlay & TYPE_CDROM_AUDIO)
            {
                // wyc2.38-909s, fix bitrate show incorrect problem.
                _dwOSDMMAudioBitRate = __wHALAudioBitrate;
                // LLY2.53, don't call function to get Audio Frequency again
                // Since, the value is ready after DSP docoding audio stream done.
                // And, we don't do audio pre-decode while stop -> resume
                // So, if only call this API once time, the value maybe not ready.
                // Suggest to reference __dwHALAudioFreq directly.
                // Notice: the value of __dwHALAudioFreq is VALUE_XXX not AUDIO_FREQ_XXX
                // So, must modify _OSDMM_ShowAudioInfo() too.
                //HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &_dwOSDMMAudioFreq);
                _dwOSDMMAudioFreq = __dwHALAudioFreq;
                
                if(__bAttrPlay==ATTR_WMA)
                {
                    //winnie2.76, fix play wma audio total time incorrect problem while stop-->resume.
                    //The audio total time is 00:00 while stop-->resume. 
                    //Since, the value is ready after DSP docoding audio stream done.
                    //And, we don't do audio pre-decode while stop -> resume
                    //HAL_ReadAM(HAL_AM_TOTAL_TIME, &_dwOSDMMAudioTotalTime);  
                    _dwOSDMMAudioTotalTime=__dwFileTime;
                }         
            }

            _OSDMM_UpdateHelp(bParam); //Because Mmanager knows which file is decoded and show the help, identify the help type directly by Fmanager
        }
        break;
    case OSDMM_MSG_PROGRAM_DELETE_ALL:
        //clear left file window
        _OSDMM_ClearRegion(OSDMM_LEFT_FILE_WINDOW_CONTENT_REGION);

        //clear right file window
        _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_CONTENT_REGION);
        break;
    case OSDMM_MSG_CLEAR_ACTIVE_PLAYING_FILE:
        if (!_bOSDMMFlash) //Current is flashing state. Next state is non-flashing state.
            _OSDMM_UpdateHighlight(OSDMM_UPDATE_CLEAR);
        break;
    case OSDMM_MSG_PROGRAM_DELETE:
        _OSDMM_DeleteProgramItem();
        break;
    case OSDMM_MSG_JPEG_PREVIEW:
        //Clear the OSD of the "Preview" window.
        _OSDMM_ClearRegion(OSDMM_PREVIEW_REGION);

        //Clear time
        _OSDMM_ClearRegion(OSDMM_TIME_REGION);

        if (_bOSDMMShowTime)
        {
            _OSDMM_StopAudioPlayingFile();
        }

        break;
    case OSDMM_MSG_ENABLE_BUTTON:
        _OSDMM_DrawButton(bParam, OSDMM_BUTTON_ENABLE);
        _bOSDMMButtonState[__bMMMenuID-1][bParam] = OSDMM_BUTTON_ENABLE;
        break;
    case OSDMM_MSG_DISABLE_BUTTON:
        _OSDMM_DrawButton(bParam, OSDMM_BUTTON_DISABLE);
        _bOSDMMButtonState[__bMMMenuID-1][bParam] = OSDMM_BUTTON_DISABLE;
        break;
    case OSDMM_MSG_SAVE:
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
        _OSDMM_ShowSaveFileDlg();
#endif //SUPPORT_COPY_FILE_IN_MM_UI
        break;
    default:
        break;
    }
}


//********************************************************************************************************
//  Function    :   OSDMM_Trigger
//  Abstract    :   This function will be called by OSD_Trigger(). This function will update time, audio bar 
//              :   and flash the active playing file.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
// LLY2.53, expand time value unit from WORD to DWORD
// Since, we will use [23:16]:hour, [15:8]: min, [7:0]: sec
//void OSDMM_Trigger(BYTE bType, WORD wTime)
void OSDMM_Trigger(BYTE bType, DWORD dwTime)
{
    if (_bOSDMMShowTime)
    {
        if (__bMMMenuID == MM_MENU_NORMAL)
        {
            if (bType == OSD_TRIGGER_TIME)
            {
                //update audio time?
                OSDUTL_PrepareOutputString();
                OSDUTL_ShowHourMinSec();
                
                _OSDMM_InitializeStringPalette(OSDMM_TIME_REGION);        
                _OSDMMStringAttr.wX = OSDMM_TIME_START_H;
                _OSDMMStringAttr.wY = OSDMM_TIME_START_V;    
                
                //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
                _OSDMM_DrawString();
            }
            else if (bType == OSD_TRIGGER_CONTROL)
            {
                //update the audio bar?
                if ((OS_GetSysTimer()-_dwOSDMMSpectrumTime) > OSDMM_UPDATE_AUDIO_SPECTRUM_TIME)
                {
                    _OSDMM_ShowAudioSpectrum();
                    _dwOSDMMSpectrumTime = OS_GetSysTimer();
                }
                
                //flash the active playing file?            
                if ((OS_GetSysTimer()-_dwOSDMMFlashTime) > OSDMM_FLASH_AUDIO_FILE_TIME)
                {
                    _OSDMM_FlashPlayingFile();
                    _dwOSDMMFlashTime = OS_GetSysTimer();
                }
            }
        }
    }

#ifdef SUPPORT_COPY_FILE_IN_MM_UI
    if (bType == OSD_TRIGGER_CONTROL)
    {
        if ((_bOSDMMUIMode == OSDMM_INITIAL_MODE_COPY)
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
            || (__bMMSlideShowCopyDeleteMode == MM_SLIDE_SHOW_COPY_MODE)
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
            )
        {
            if (_bOSDMMSaveFile)
            {
                //Update the progress
                _OSDMM_UpdateSaveFileProgress();
            }
        }
    }
#endif //SUPPORT_COPY_FILE_IN_MM_UI
}

//********************************************************************************************************
//  Function    :   OSDMM_Recover
//  Abstract    :   This function will be called by OSD when File Manager UI is overwritten by other UI and the UI exits.
//  Arguments   :   bRecoverRegion: OSD_RECOVER_UPPER_REGION/OSD_RECOVER_BOTTOM_REGION/OSD_RECOVER_ALL
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
BYTE OSDMM_Recover(BYTE bRecoverRegion)
{
    switch (bRecoverRegion)
    {
    case OSD_RECOVER_UPPER_REGION:
        _OSDMM_ClearRegion(OSDMM_RECOVER_UPPER_REGION);
        return FALSE; //CoCo1.10, return FALSE to let OSD to recover normal display message. For example, pasue==>zoom (show invalid)==>recover to pause.
    case OSD_RECOVER_BOTTOM_REGION:
        _OSDMM_DrawInfoRegion();
        _OSDMM_UpdateHelp(_bOSDMMHelpType);
        break;
    case OSD_RECOVER_ALL:
        _OSDMM_Initial(OSDMM_MODE_RECOVER_UI);
        return FALSE;
    default:
        break;
    }

    return TRUE;
}

//********************************************************************************************************
//  Function    :   OSDMM_ClearRegion
//  Abstract    :   This function will be called by OSD to clear the normal upper region or normal bottom 
//              :   region if the UI accepts the normal display message is displayed in its UI.
//  Arguments   :   bRegion: OSD_ND_UPPER_RIGHT_REGION/OSD_ND_BOTTOM_REGION.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void OSDMM_ClearRegion(BYTE bRegion)
{
    if (bRegion == OSD_ND_UPPER_RIGHT_REGION)
    {
        _OSDMM_ClearRegion(OSDMM_RECOVER_UPPER_REGION);
    }
    else if (bRegion == OSD_ND_BOTTOM_REGION)
    {
        //_OSDMM_ClearRegion(OSDMM_RECOVER_BOTTOM_REGION);
        _OSDMM_DrawInfoRegion();
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_InitializeGDI
//  Abstract    :   This function will initialize the palette and draw the background of the Media Manager UI.
//  Arguments   :   none.    
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_InitializeGDI(void)
{
    _bOSDMMTemp = __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN;

    GDI_ClearRegion(0); //clear region 0

    //GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND, OSDMM_PALETTE_COLOR_GENERAL_BACKGROUND, TRUE);
    //GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_TITLE_STRING, OSDMM_PALETTE_COLOR_TITLE_STRING, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_HELP_STRING, OSDMM_PALETTE_COLOR_HELP_STRING, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_GENERAL_STRING_IN_FILE_WINDOW, OSDMM_PALETTE_COLOR_GENERAL_STRING_IN_FILE_WINDOW, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND, OSDMM_PALETTE_COLOR_HIGHLIGHT_BACKGROUND, FALSE);
    //GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_ENABLE_BUTTON_STRING, OSDMM_PALETTE_COLOR_ENABLE_BUTTON_STRING, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_HIGHLIGHT_BUTTON_STRING, OSDMM_PALETTE_COLOR_HIGHLIGHT_BUTTON_STRING, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT, OSDMM_PALETTE_COLOR_NORMAL_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_NORMAL_FRAME_DARK, OSDMM_PALETTE_COLOR_NORMAL_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_SELECT_FRAME_BRIGHT, OSDMM_PALETTE_COLOR_SELECT_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_SELECT_FRAME_DARK, OSDMM_PALETTE_COLOR_SELECT_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_SEPARATE_FRAME_CENTER, OSDMM_PALETTE_COLOR_SEPARATE_FRAME_CENTER, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_UPFOLDER_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_UPFOLDER_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_UPFOLDER_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_UPFOLDER_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_UPFOLDER_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_UPFOLDER_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_FOLDER_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_FOLDER_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_FOLDER_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_FOLDER_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_FOLDER_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_FOLDER_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_JPEG_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_JPEG_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_JPEG_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_JPEG_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_JPEG_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_JPEG_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_MP3_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_MP3_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_MP3_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_MP3_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_MP3_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_MP3_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_WMA_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_WMA_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_WMA_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_WMA_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_WMA_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_WMA_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_OPENFOLDER_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_OPENFOLDER_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_OPENFOLDER_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_OPENFOLDER_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_OPENFOLDER_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_OPENFOLDER_ICON_SHADOW_2, FALSE);
    //GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_TIME_STRING, OSDMM_PALETTE_COLOR_TIME_STRING, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DIVX_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_DIVX_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DIVX_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_DIVX_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DIVX_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_DIVX_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_MP4_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_MP4_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_MP4_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_MP4_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_MP4_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_MP4_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_MP2_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_MP2_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_MP2_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_MP2_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_MP2_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_MP2_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DAT_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_DAT_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DAT_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_DAT_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DAT_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_DAT_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOB_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_VOB_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOB_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_VOB_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOB_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_VOB_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_VOLUME_SERVO_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_VOLUME_SERVO_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_VOLUME_SERVO_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOLUME_USB_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_VOLUME_USB_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOLUME_USB_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_VOLUME_USB_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOLUME_USB_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_VOLUME_USB_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOLUME_CARD_READER_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_VOLUME_CARD_READER_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOLUME_CARD_READER_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_VOLUME_CARD_READER_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_VOLUME_CARD_READER_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_VOLUME_CARD_READER_ICON_SHADOW_2, FALSE);
    //GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DLG_BACKGROUND, OSDMM_PALETTE_COLOR_DLG_BACKGROUND, FALSE);
    //GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND, OSDMM_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND, OSDMM_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND, FALSE);
    //GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_HELP_REGION_BACKGROUND, OSDMM_PALETTE_COLOR_HELP_REGION_BACKGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_HIGHLIGHT_STRING_IN_FILE_WINDOW, OSDMM_PALETTE_COLOR_HIGHLIGHT_STRING_IN_FILE_WINDOW, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DLG_STRING, OSDMM_PALETTE_COLOR_DLG_STRING, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_CHECKED_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_CHECKED_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_CHECKED_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_CHECKED_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_CHECKED_ICON_SHADOW_2, OSDMM_PALETTE_COLOR_CHECKED_ICON_SHADOW_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_UNCHECKED_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_UNCHECKED_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_RADIO_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_RADIO_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_RADIO_ICON_SHADOW_1, OSDMM_PALETTE_COLOR_RADIO_ICON_SHADOW_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_UNRADIO_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_UNRADIO_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_0, OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_LINE_0, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_1, OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_LINE_1, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_2, OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_LINE_2, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_3, OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_LINE_3, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_4, OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_LINE_4, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_DROP, OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_DROP, FALSE);
    //GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DLG_TITLE_BAR, OSDMM_PALETTE_COLOR_DLG_TITLE_BAR, FALSE);
    //GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_BACKGROUND_FRAME, OSDMM_PALETTE_COLOR_BACKGROUND_FRAME, TRUE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_INFO_REGION_FRAME, OSDMM_PALETTE_COLOR_INFO_REGION_FRAME, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_BUTTON_FRAME, OSDMM_PALETTE_COLOR_BUTTON_FRAME, FALSE);
    //GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DLG_OUTER_FRAME, OSDMM_PALETTE_COLOR_DLG_OUTER_FRAME, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_BUTTON_DISABLE_BACKGROUND, OSDMM_PALETTE_COLOR_BUTTON_DISABLE_BACKGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DISABLE_BUTTON_STRING, OSDMM_PALETTE_COLOR_DISABLE_BUTTON_STRING, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_AC3_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_AC3_ICON_TEXT_FOREGROUND, FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_MPG_ICON_TEXT_FOREGROUND, OSDMM_PALETTE_COLOR_MPG_ICON_TEXT_FOREGROUND, FALSE);

    //Change skin
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND, _dwOSDMMGeneralBackgroundColor[_bOSDMMTemp], FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_TITLE_STRING, _dwOSDMMTitleStringColor[_bOSDMMTemp], FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_BACKGROUND_FRAME, _dwOSDMMBackgroundFrameColor[_bOSDMMTemp], FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_TIME_STRING, _dwOSDMMTimeStringColor[_bOSDMMTemp], FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_HELP_REGION_BACKGROUND, _dwOSDMMHelpRegionBackgroundColor[_bOSDMMTemp], FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND, _dwOSDMMHButtonEnableColor[_bOSDMMTemp], FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_ENABLE_BUTTON_STRING, _dwOSDMMHButtonEnableStringColor[_bOSDMMTemp], FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DLG_TITLE_BAR, _dwOSDMMDlgTitleBarColor[_bOSDMMTemp], FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DLG_OUTER_FRAME, _dwOSDMMDlgOuterFrameColor[_bOSDMMTemp], FALSE);
    GDI_ChangePALEntry(OSDMM_PALETTE_ENTRY_DLG_BACKGROUND, _dwOSDMMDlgBackgroundColor[_bOSDMMTemp], FALSE);

    GDI_SetMixRatio(OSDMM_MIX_RATIO_BACKGROUND); //The mix ration just effects the background

    //CoCo0.95
    GDI_WaitPaletteComplete();

    GDI_ActivateRegion(0); //open GDI
}

//*********************************************************************************************************
//  Function    :   _OSDMM_Exit
//  Abstract    :   Call this function to exit the Media Manager UI.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_Exit(void)
{
    //Reset the button status.
    OSDMM_ResetControl();

    //Clear the video background.

    //Clear all Media Manager OSD region.
    GDI_ClearRegion(0); //clear region 0

    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL)
    {
        GDI_MoveRegion(0, REGION_X_PAL, REGION_Y_PAL);
    }

    //Tell OSD that Media Manager exits.
    OSD_ChangeUI(OSD_UI_MEDIA_MANAGER, OSD_EXIT_UI);

    OSD_RecoverNBForeverMsg();
}

//*********************************************************************************************************
//  Function    :   _OSDMM_Initial
//  Abstract    :   Call this function to initialize the UI.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_Initial(BYTE bMode)
{
    if (bMode == OSDMM_MODE_INITIAL_UI)
    {
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
        if (_bOSDMMUIMode == OSDMM_INITIAL_MODE_COPY)
        {
            HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
        }
#endif //SUPPORT_COPY_FILE_IN_MM_UI

        if (__bAttrPlay == ATTR_JPG)
        {
            // wyc1.20-909, when change to MM UI mode from JPG effect mode, need to clear buffer 0/1 to avoid show remain image on screen.
            // only clear current buffer will see remain image.
            if (__bHALJPEGEffect == HALJPEG_EFFECT_NONE)
                HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
            else
            {
                //HALJPEG_CompleteEffectImmediately(__HALJPEGSetting.bFrameBuffer);
                //DISP_DelayVSYNC(1);
				MM_CompleteJPGEffect(FALSE);
#ifndef IMAGE_FRAME_SETUP //CoCo2.38
                __bMMEffectIdx = HALJPEG_EFFECT_NONE;
#endif //#ifndef IMAGE_FRAME_SETUP //CoCo2.38

				HALJPEG_Reset();				

                HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_FRAME0);
                HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_FRAME1);
				HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_FRAME2);
            }
        }
        else
        {
            HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
        }

		OS_DelayTime(COUNT_200_MSEC);

        OSD_ChangeUI(OSD_UI_MEDIA_MANAGER, OSD_ENTER_UI); //Tell OSD that the Media Manage UI enters.
    }
    __bLOGO=LOGO_NONE;

    _OSDMM_InitializeGDI();

    //Draw the background of Media Manager
    _OSDMM_DrawBackground();   

    __bActiveDirChange = FALSE;

    _OSDMM_DrawBigFrame();

    //draw vertical separate frame
    _OSDMM_DrawSeparateFrame(OSDMM_VERTICAL_SEPARATE_FRAME);

    //draw horizontal separate frame
    _OSDMM_DrawSeparateFrame(OSDMM_HORIZONTAL_SEPARATE_FRAME);
    
    /*
    switch (__bMMMenuID)
    {
    case MM_MENU_EDITPROG:
        _bOSDMMTitleType = OSDMM_TITLE_TYPE_PROGRAM;
        break;
    case MM_MENU_PROGLIST:
        _bOSDMMTitleType = OSDMM_TITLE_TYPE_PROGRAM_LIST;
        break;
    default:
    */
        _bOSDMMTitleType = OSDMM_TITLE_TYPE_NONE; //reset the title type for re-drawing it.
        /*
        break;
    }
    */

    _OSDMM_ClearRegion(OSDMM_PREVIEW_REGION);

    if (bMode == OSDMM_MODE_INITIAL_UI)
    {
        _bOSDMMHelpType = OSDMM_HELP_TYPE_NONE;
        _bOSDMMShowTime = FALSE;
        _bOSDMMPlayingTitle = FALSE;

        //OSD_ChangeUI(OSD_UI_MEDIA_MANAGER, OSD_ENTER_UI); //Tell OSD that the Media Manage UI enters.

        if (__MMSelectCursor.bPosition == POS_FILE_LIST)
        {
            __wOSDMMStartDrawItem = (__MMSelectCursor.wIndex / OSDMM_MAX_ITEM_IN_FILE_WINDOW)*OSDMM_MAX_ITEM_IN_FILE_WINDOW;
        }
        else
            __wOSDMMStartDrawItem = 0;

        _OSDMM_DrawLeftFileWindow();
        _OSDMM_DrawRightFileWindow();
    }
    else //Recover mode
    {
        if (_bOSDMMPlayingTitle && (!__btInputProgram))
        {
            _OSDMM_UpdatePlayingTitle();
        }

        _OSDMM_ResetAudioSpectrum();

        if (__bMMMenuID == MM_MENU_PROGLIST)
        {
            _OSDMM_PL_DrawRightFileWindow();
        }
        else
        {
            _OSDMM_DrawLeftFileWindow();
            _OSDMM_DrawRightFileWindow();

            if (__bMMPopMenuID == MM_POP_MENU_FILTER)
            {
                _OSDMM_ShowFileTypeDialogBox();
            }
            else if (__bMMPopMenuID == MM_POP_MENU_SORT)
            {
                _OSDMM_ShowSortFileDialogBox();
            }
        }
    }

    //Set the attributes for normal upper message and normal bottom message.
    __OSDNDUpMsgAttr.wX = OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_WIDTH-OSDMM_BIG_FRAME_ND_DISTANCE;
    __OSDNDUpMsgAttr.wY = OSDMM_TITLE_START_V;
    __OSDNDUpMsgAttr.bBackground = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
    __OSDNDUpMsgAttr.bTextColor = OSDMM_PALETTE_ENTRY_TITLE_STRING;
    __OSDNDUpMsgAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDUpMsgAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDUpMsgAttr.bColorKey = 0;
    __OSDNDBottomMsgAttr.wX = OSDMM_HELP_STRING_START_H;
    __OSDNDBottomMsgAttr.wY = OSDMM_HELP_STRING_START_V;
    __OSDNDBottomMsgAttr.bBackground = OSDMM_PALETTE_ENTRY_HELP_REGION_BACKGROUND;
    __OSDNDBottomMsgAttr.bTextColor = OSDMM_PALETTE_ENTRY_HELP_STRING;
    __OSDNDBottomMsgAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDBottomMsgAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDBottomMsgAttr.bColorKey = 0;

    //Don't draw the highlight because the cursor information is incorrect.
    if (_bOSDMMUIMode != OSDMM_INITIAL_MODE_COPY)
    {
        _OSDMM_UpdateHighlight(OSDMM_UPDATE_DRAW);
    }

    //Draw all buttons
    _OSDMM_DrawAllButtons();

    //Draw info. region.
    _OSDMM_DrawInfoRegion();

    if ((__bAttrPlay==ATTR_JPG) && (__bMMrogressCnt != 0)) //CoCo1.11, to prevent the help exists with the progress bar.
        return;

    _OSDMM_UpdateHelp(_bOSDMMHelpType);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_SetFrameAttribute
//  Abstract    :   This function will set the attribute of the frame.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_SetFrameAttribute(BYTE bStyle)
{
    _OSDMMFrame.bColorBright = OSDMM_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
    _OSDMMFrame.bColorDark = OSDMM_PALETTE_ENTRY_NORMAL_FRAME_DARK;

    if (bStyle == FRAME_DOUBLE)
    {
        _OSDMMFrame.bColorCenter = PAL_ENTRY_COLOR_TRANSPARENT; //"FRAME_DOUBLE" doesn't need to assign the center color?
    }
    else if (FRAME_DOUBLE_SOLID)
    {
        _OSDMMFrame.bColorCenter = OSDMM_PALETTE_ENTRY_SEPARATE_FRAME_CENTER;        
    }

    _OSDMMFrame.bStyle = bStyle;
}


//*********************************************************************************************************
//  Function    :   _OSDMM_DrawBigFrame
//  Abstract    :   This function will draw the big frame for the file window.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawBigFrame(void)
{
    _OSDMM_SetFrameAttribute(FRAME_DOUBLE);

    _OSDMMFrame.bThickness = OSDMM_BIG_FRAME_THICKNESS >> 1;
    _OSDMMFrame.rect.wLeft = OSDMM_BIG_FRAME_H;
    _OSDMMFrame.rect.wTop = OSDMM_BIG_FRAME_V;
    _OSDMMFrame.rect.wRight = OSDMM_BIG_FRAME_H + OSDMM_BIG_FRAME_WIDTH;
    _OSDMMFrame.rect.wBottom = OSDMM_BIG_FRAME_V + OSDMM_BIG_FRAME_HEIGHT;

    GDI_DrawFrame_909(0, &_OSDMMFrame);        
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawSeparateFrame
//  Abstract    :   This function will draw horizontal or vertical separate frame for the file window.
//  Arguments   :   bKind: OSDMM_VERTICAL_SEPARATE_FRAME: To draw the vertical separate frame.
//              :          OSDMM_HORIZONTAL_SEPARATE_FRAME: To draw the horizontal separate frame.      
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawSeparateFrame(BYTE bKind)
{
    _OSDMM_SetFrameAttribute(FRAME_DOUBLE_SOLID);

    if (bKind == OSDMM_VERTICAL_SEPARATE_FRAME)
    {
        _OSDMMFrame.bThickness = OSDMM_VERTICAL_SEPARATE_FRAME_THICKNESS >> 1;
    
        _OSDMMFrame.rect.wLeft = OSDMM_VERTICAL_SEPARATE_FRAME_H;
        _OSDMMFrame.rect.wTop = OSDMM_VERTICAL_SEPARATE_FRAME_V;
        _OSDMMFrame.rect.wRight = OSDMM_VERTICAL_SEPARATE_FRAME_H + OSDMM_VERTICAL_SEPARATE_FRAME_WIDTH;
        _OSDMMFrame.rect.wBottom = OSDMM_VERTICAL_SEPARATE_FRAME_V + OSDMM_VERTICAL_SEPARATE_FRAME_HEIGHT;
    }
    else //horizontal separate frame
    {
        _OSDMMFrame.bThickness = OSDMM_HORIZONTAL_SEPARATE_FRAME_THICKNESS >> 1;

        _OSDMMFrame.rect.wLeft = OSDMM_HORIZONTAL_SEPARATE_FRAME_H;
        _OSDMMFrame.rect.wTop = OSDMM_HORIZONTAL_SEPARATE_FRAME_V;
        _OSDMMFrame.rect.wRight = OSDMM_HORIZONTAL_SEPARATE_FRAME_H + OSDMM_HORIZONTAL_SEPARATE_FRAME_WIDTH-1;
        _OSDMMFrame.rect.wBottom = OSDMM_HORIZONTAL_SEPARATE_FRAME_V + OSDMM_HORIZONTAL_SEPARATE_FRAME_HEIGHT;
    }

    GDI_DrawFrame_909(0, &_OSDMMFrame);        
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawLeftFileWindow
//  Abstract    :   This funciton will draw the left file window.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawLeftFileWindow(void)
{    
    if (__MMSelectCursor.wDirID == MM_ROOT)
    {
        //at the first scrren of Media Manager UI.
        _OSDMM_DrawAllVolumes();
    }
    else
    {
        //To judge if the "Up_DIR" exists or not.
        if (_OSDMM_HaveUpDIR()) //with "Up_DIR"
            _bOSDMMHaveUpDIR = TRUE;
        else
            _bOSDMMHaveUpDIR = FALSE;
    
        _bOSDMMTemp = __MMSelectCursor.wDirID - __pMMVolumeDirTree[__pMMVolumeDirTree[__MMSelectCursor.wDirID].bParentID].b1stSubDirID;
        
        _bOSDMMStartDrawDir = 0;

        if (_bOSDMMHaveUpDIR)
        {
            if (_bOSDMMTemp >= (OSDMM_MAX_DIR_IN_FILE_WINDOW-2))
                _bOSDMMStartDrawDir = (_bOSDMMTemp+1) + 2 - OSDMM_MAX_DIR_IN_FILE_WINDOW; //2 = "ROOT"+"Up_DIR"
        }
        else //without "Up_DIR"
        {
            if (_bOSDMMTemp >= (OSDMM_MAX_DIR_IN_FILE_WINDOW-1))
                _bOSDMMStartDrawDir = (_bOSDMMTemp+1) + 1 - OSDMM_MAX_DIR_IN_FILE_WINDOW; //1 = "ROOT"
        }

        if (_bOSDMMStartDrawDir == 0) //Draw from "ROOT"
        {
            _OSDMM_DrawVolume(0, __MMSelectCursor.bVolID, FALSE);
            
            if (_bOSDMMHaveUpDIR) //with "Up_DIR"
            {
                _OSDMM_DrawUpDIR(1, FALSE);                
                _OSDMM_DrawDir(2);
            }
            else
                _OSDMM_DrawDir(1);
        }
        else if (_bOSDMMStartDrawDir == 1)
        {
            if (_bOSDMMHaveUpDIR) //with "Up_DIR"
            {
                _OSDMM_DrawUpDIR(0, FALSE);
                _OSDMM_DrawDir(1);
            }
            else
                _OSDMM_DrawDir(0);
        }
        else
            _OSDMM_DrawDir(0);
    }

    _OSDMM_UpdateLeftScrollBar();
}

//*********************************************************************************************************
//  Function    :   _OSDMM_InitializeStringPalette
//  Abstract    :   This function will set the palette for the file window.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_InitializeStringPalette(BYTE bRegion)
{
    _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
    _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
    _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
    _OSDMMStringAttr.bColorKey = 0;

    switch (bRegion)
    {
    case OSDMM_TITLE_REGION:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_TITLE_STRING;
        break;
    case OSDMM_LEFT_FILE_WINDOW_REGION:
    case OSDMM_RIGHT_FILE_WINDOW_REGION:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_GENERAL_STRING_IN_FILE_WINDOW;
        break;
    case OSDMM_BUTTON_REGION:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_ENABLE_BUTTON_STRING;
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND;
        break;
    case OSDMM_HELP_REGION:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_HELP_STRING;
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HELP_REGION_BACKGROUND;
        break;
    case OSDMM_TIME_REGION:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_TIME_STRING;
        break;
    case OSDMM_DLG_REGION:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_DLG_STRING;
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_DLG_BACKGROUND;
        break;
    case OSDMM_DLG_TITLE_REGION:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_DLG_STRING;
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_DLG_TITLE_BAR;
        break;
    default:
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawRightFileWindow
//  Abstract    :   This function will draw the right file window.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawRightFileWindow(void)
{
    BYTE bOSDMMTemp;
    
    if ((__pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir == 0) && (__pMMFilterFilesOfDir[__MMSelectCursor.wDirID] == 0))
        return; //no folders and files
    
    if (__wOSDMMStartDrawItem >= __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir)
    {
        //draw file directly.
        _wOSDMMTemp = __wOSDMMStartDrawItem - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir; //for sequential number.
        
        //Transfrom relative ID to absolute ID        
        MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID, _wOSDMMTemp), OSDMM_MAX_ITEM_IN_FILE_WINDOW);
        
        for (bOSDMMTemp = 0; (bOSDMMTemp < OSDMM_MAX_ITEM_IN_FILE_WINDOW)&&(_wOSDMMTemp < __pMMFilterFilesOfDir[__MMSelectCursor.wDirID]); bOSDMMTemp++)
        {
            //Dump file record.
            if (MM_GetDumpedFileRecord() == MM_FILE_RECORD_DUMP_FAILED)
                return;
            
            //Draw icon for the files            
            _OSDMM_DrawRightFileWindowIcon(bOSDMMTemp, _OSDMM_GetIconType(__pMMFileRecord->FileName.bExtID), FALSE);
            
            _OSDMM_InitializeStringPalette(OSDMM_RIGHT_FILE_WINDOW_REGION);
                        
            //Draw string
            _OSDMMStringAttr.wX = OSDMM_ITEM_RIGHT_WINDOW_START_H;
            _OSDMMStringAttr.wY = OSDMM_ITEM_RIGHT_WINDOW_START_V + bOSDMMTemp*OSDMM_ITEM_HEIGHT;
            _OSDMM_CopyFileString(_wOSDMMTemp);
            //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
#ifdef SUPPORT_UNICODE_CODING
            if ((__pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
            {
                _OSDMM_DrawStringByUnicode();
                _bOSDMMFileLength = GDI_GetUniStringWidth(__pOSDUTLOutputString); //for draw the selected frame
            }
            else
#endif
            {
            _OSDMM_DrawString();
            _bOSDMMFileLength = GDI_GetStringWidth_909(__pOSDUTLOutputString); //for draw the selected frame
            }    
            
            //draw serial number
            _OSDMM_DrawSerialNumber(bOSDMMTemp, _wOSDMMTemp+1);
            
            //draw the selected frame for program mode
            if ((__btProgram) || (_bOSDMMUIMode == OSDMM_INITIAL_MODE_COPY))
            {
                if (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, __MMSelectCursor.wDirID, (WORD)(_wOSDMMTemp+1)) != 0xFF)
                {
                    //draw the selected frame
                    _OSDMM_DrawSelectedFrame(OSDMM_UPDATE_DRAW, bOSDMMTemp);
                }
            }
            
            _wOSDMMTemp++; //next file
        }
    }
    else //draw folder and then file
    {
        //draw folder
        _wOSDMMTemp = __wOSDMMStartDrawItem + __pMMVolumeDirTree[__MMSelectCursor.wDirID].b1stSubDirID;
        
        bOSDMMTemp = 0;
        
        while ((bOSDMMTemp < OSDMM_MAX_ITEM_IN_FILE_WINDOW) && ((bOSDMMTemp+__wOSDMMStartDrawItem) < __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir))
        {
            //draw icon for the folder.
            _OSDMM_DrawRightFileWindowIcon(bOSDMMTemp, OSDMM_ICON_TYPE_FOLDER, FALSE);
            
            //draw string.
            _OSDMM_InitializeStringPalette(OSDMM_RIGHT_FILE_WINDOW_REGION);
            _OSDMM_CopyDirString((BYTE)_wOSDMMTemp, (OSDMM_RIGHT_SCROLL_BAR_H - OSDMM_ITEM_RIGHT_WINDOW_START_H));
            _OSDMMStringAttr.wX = OSDMM_ITEM_RIGHT_WINDOW_START_H;
            _OSDMMStringAttr.wY = OSDMM_ITEM_RIGHT_WINDOW_START_V+bOSDMMTemp*OSDMM_ITEM_HEIGHT;
            //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
#ifdef SUPPORT_UNICODE_CODING
            if ((__pMMDirNameList[_wOSDMMTemp].bMMLinkStatus & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
            {
                _OSDMM_DrawStringByUnicode();                
            }
            else
#endif
            {
            _OSDMM_DrawString();
            }     
            
            bOSDMMTemp++;
            _wOSDMMTemp++;
        }
        
        //draw file
        if (__pMMFilterFilesOfDir[__MMSelectCursor.wDirID] != 0)
        {
            _wOSDMMTemp = 0;

            //Transfrom relative ID to absolute ID            
            MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID, _wOSDMMTemp), OSDMM_MAX_ITEM_IN_FILE_WINDOW);
            
            while (bOSDMMTemp < OSDMM_MAX_ITEM_IN_FILE_WINDOW)
            {
                if (_wOSDMMTemp >= __pMMFilterFilesOfDir[__MMSelectCursor.wDirID])
                    break;

                //Dump file record.
                if (MM_GetDumpedFileRecord() == MM_FILE_RECORD_DUMP_FAILED)
                    return;
                
                //Draw icon for the files
                _OSDMM_DrawRightFileWindowIcon(bOSDMMTemp, _OSDMM_GetIconType(__pMMFileRecord->FileName.bExtID), FALSE);
                
                //draw string.
                _OSDMM_InitializeStringPalette(OSDMM_RIGHT_FILE_WINDOW_REGION);
                                
                _OSDMM_CopyFileString(_wOSDMMTemp);
                _OSDMMStringAttr.wX = OSDMM_ITEM_RIGHT_WINDOW_START_H;
                _OSDMMStringAttr.wY = OSDMM_ITEM_RIGHT_WINDOW_START_V+bOSDMMTemp*OSDMM_ITEM_HEIGHT;
                //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
#ifdef SUPPORT_UNICODE_CODING
                if ((__pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
                {
                    _OSDMM_DrawStringByUnicode();
                    _bOSDMMFileLength = GDI_GetUniStringWidth(__pOSDUTLOutputString); //for draw the selected frame
                }
                else
#endif
                {
                _OSDMM_DrawString();
                _bOSDMMFileLength = GDI_GetStringWidth_909(__pOSDUTLOutputString); //for draw the selected frame
                }               
                
                //draw serial number
                _OSDMM_DrawSerialNumber(bOSDMMTemp, _wOSDMMTemp+1);
                
                //draw the selected frame for program mode
                if (__btProgram || (_bOSDMMUIMode == OSDMM_INITIAL_MODE_COPY))
                {
                    if (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, __MMSelectCursor.wDirID, (WORD)(_wOSDMMTemp+1)) != 0xFF)
                    {
                        //draw the selected frame
                        _OSDMM_DrawSelectedFrame(OSDMM_UPDATE_DRAW, bOSDMMTemp);
                    }
                }
                
                bOSDMMTemp++;
                _wOSDMMTemp++;
            }
        }
    }
    
    _OSDMM_UpdateRightScrollBar();
}


//*********************************************************************************************************
//  Function    :   _OSDMM_UpdateHighlight
//  Abstract    :   This function will update the highlight.
//  Arguments   :   bUpdate: OSDMM_UPDATE_DRAW: draw the highlight.
//              :            OSDMM_UPDATE_CLEAR: clear the highlight.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_UpdateHighlight(BYTE bUpdate)
{
    WORD wStringLength = 0;

    if ((__bMMPopMenuID == MM_POP_MENU_NONE) && (bUpdate == OSDMM_UPDATE_DRAW))
    {
        _OSDMM_UpdateTitle();
        //_OSDFM_UpdateHelp();
    }
      
    switch (__MMSelectCursor.bPosition)
    {
    case POS_VOL_DIR_LIST:
        if (!((__MMSelectCursor.wIndex < __bMMVolumeNum) || ((__MMSelectCursor.wIndex == __bMMVolumeNum) && _OSDMM_HaveUpDIR())))
        {
            _OSDMM_CopyDirString((BYTE)__MMSelectCursor.wDirID, (OSDMM_LEFT_SCROLL_BAR_H - OSDMM_ITEM_LEFT_WINDOW_START_H));
#ifdef SUPPORT_UNICODE_CODING
            if ((__pMMDirNameList[__MMSelectCursor.wDirID].bMMLinkStatus & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
            {
                wStringLength = GDI_GetUniStringWidth(__pOSDUTLOutputString);
            }
            else
#endif
            {
                wStringLength = GDI_GetStringWidth_909(__pOSDUTLOutputString);
            }    

            if (bUpdate == OSDMM_UPDATE_DRAW)				
                _bOSDMMIconType = OSDMM_ICON_TYPE_OPENFOLDER; 
            else
			{
				if (__bActiveDirChange) //change dir really
                {
                    _bOSDMMIconType = OSDMM_ICON_TYPE_FOLDER; 
                    __bActiveDirChange = FALSE;
                }
                else //Only KEY_RIGHT/KEY_LEFT to change the highlight.                
                    _bOSDMMIconType = OSDMM_ICON_TYPE_OPENFOLDER;                     
			}
        }
        else if (__MMSelectCursor.wIndex < __bMMVolumeNum) //Volume
        {
            _OSDMM_SetVolumeString(__MMSelectCursor.bVolID);
            wStringLength = GDI_GetStringWidth_909(__pOSDUTLOutputString);
        }
        else if ((__MMSelectCursor.wIndex == __bMMVolumeNum) && _OSDMM_HaveUpDIR())//up DIR
        {
            __pOSDUTLOutputString = aMMUpDIR;
            wStringLength = GDI_GetStringWidth_909(__pOSDUTLOutputString);
        }

        //Draw/clear highlight
        _OSDMMRect.rect.wLeft = OSDMM_ICON_LEFT_WINDOW_START_H - OSDMM_HIGHLIGHT_DISTANCE_H;
        _OSDMMRect.rect.wTop = OSDMM_ITEM_LEFT_WINDOW_START_V+((__MMSelectCursor.wIndex-_bOSDMMStartDrawDir)%OSDMM_MAX_DIR_IN_FILE_WINDOW)*OSDMM_ITEM_HEIGHT-OSDMM_HIGHLIGHT_DISTANCE_V;            
        _OSDMMRect.rect.wRight = OSDMM_ITEM_LEFT_WINDOW_START_H+wStringLength+OSDMM_HIGHLIGHT_DISTANCE_H;
        _OSDMMRect.rect.wBottom = OSDMM_ITEM_LEFT_WINDOW_START_V+((__MMSelectCursor.wIndex-_bOSDMMStartDrawDir)%OSDMM_MAX_DIR_IN_FILE_WINDOW)*OSDMM_ITEM_HEIGHT+OSDMM_FILE_WINDOW_FONT_HEIGHT+OSDMM_HIGHLIGHT_DISTANCE_V;

        if (bUpdate == OSDMM_UPDATE_DRAW)
        {
            _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
        }
        else
        {
            _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
        }

        GDI_FillRect_909(0, &_OSDMMRect);

        if (__MMSelectCursor.wIndex == 0)
        {
            if (bUpdate == OSDMM_UPDATE_DRAW)
                _OSDMM_DrawVolume(0, __MMSelectCursor.bVolID, TRUE);
            else
                _OSDMM_DrawVolume(0, __MMSelectCursor.bVolID, FALSE);

        }
        else if ((__MMSelectCursor.wIndex == 1) && _OSDMM_HaveUpDIR())
        {
            if (bUpdate == OSDMM_UPDATE_DRAW)
                _OSDMM_DrawUpDIR((BYTE)((__MMSelectCursor.wIndex-_bOSDMMStartDrawDir)%OSDMM_MAX_DIR_IN_FILE_WINDOW), TRUE);
            else
                _OSDMM_DrawUpDIR((BYTE)((__MMSelectCursor.wIndex-_bOSDMMStartDrawDir)%OSDMM_MAX_DIR_IN_FILE_WINDOW), FALSE);
        }
        else
        {
            if (bUpdate == OSDMM_UPDATE_DRAW)                
                _OSDMM_DrawLeftFileWindowIcon((__MMSelectCursor.wIndex-_bOSDMMStartDrawDir)%OSDMM_MAX_DIR_IN_FILE_WINDOW, _bOSDMMIconType, TRUE);
            else
                _OSDMM_DrawLeftFileWindowIcon((__MMSelectCursor.wIndex-_bOSDMMStartDrawDir)%OSDMM_MAX_DIR_IN_FILE_WINDOW, _bOSDMMIconType, FALSE);

            _OSDMM_InitializeStringPalette(OSDMM_LEFT_FILE_WINDOW_REGION);
            if (bUpdate == OSDMM_UPDATE_DRAW)
                _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
            else
                _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
            _OSDMMStringAttr.wX = OSDMM_ITEM_LEFT_WINDOW_START_H;
            _OSDMMStringAttr.wY = OSDMM_ITEM_LEFT_WINDOW_START_V+((__MMSelectCursor.wIndex-_bOSDMMStartDrawDir)%OSDMM_MAX_DIR_IN_FILE_WINDOW)*OSDMM_ITEM_HEIGHT;
            //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
#ifdef SUPPORT_UNICODE_CODING
            if ((__pMMDirNameList[__MMSelectCursor.wDirID].bMMLinkStatus & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
            {
                 _OSDMM_DrawStringByUnicode();           
            }
            else
#endif
            {
            _OSDMM_DrawString();
        }
        }

        break;
    case POS_FILE_LIST:
    case POS_PROG_LIST:
        if (__MMSelectCursor.wIndex > (__wOSDMMStartDrawItem+OSDMM_MAX_ITEM_IN_FILE_WINDOW-1))
        {
            if ((__MMSelectCursor.wIndex - (__wOSDMMStartDrawItem+OSDMM_MAX_ITEM_IN_FILE_WINDOW-1)) > 1) //jump > 1 file.
            {
                //update the __wStartDrawItem
                __wOSDMMStartDrawItem = __MMSelectCursor.wIndex - OSDMM_MAX_ITEM_IN_FILE_WINDOW + 1;

                //clear right right file window
                _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_CONTENT_REGION); //Don't clear the right scroll bar because they are in the same folder.

                //re-draw all                
                if (__MMSelectCursor.bPosition == POS_FILE_LIST)
                {
                    _OSDMM_DrawRightFileWindow();
                }
                else if (__MMSelectCursor.bPosition == POS_PROG_LIST)
                {
                    _OSDMM_PL_DrawRightFileWindow();  
                }
            }
            else
            {
                __wOSDMMStartDrawItem++;

                _OSDMMBitmapCopy.bDestRegion = 0;
                _OSDMMBitmapCopy.bSrcRegion = 0;
                _OSDMMBitmapCopy.SrcRect.wLeft = OSDMM_VERTICAL_SEPARATE_FRAME_H+OSDMM_VERTICAL_SEPARATE_FRAME_WIDTH+1;
                _OSDMMBitmapCopy.SrcRect.wTop = OSDMM_ICON_RIGHT_WINDOW_START_V+OSDMM_ITEM_HEIGHT-OSDMM_SELECT_FRAME_DISTANCE_V;
                _OSDMMBitmapCopy.SrcRect.wRight = OSDMM_RIGHT_SCROLL_BAR_H-1;
                _OSDMMBitmapCopy.SrcRect.wBottom = OSDMM_ICON_RIGHT_WINDOW_START_V+(OSDMM_MAX_ITEM_IN_FILE_WINDOW-1)*OSDMM_ITEM_HEIGHT+OSDMM_FILE_WINDOW_FONT_HEIGHT+OSDMM_SELECT_FRAME_DISTANCE_V;
                _OSDMMBitmapCopy.wDestX = OSDMM_VERTICAL_SEPARATE_FRAME_H+OSDMM_VERTICAL_SEPARATE_FRAME_WIDTH+1;
                _OSDMMBitmapCopy.wDestY = OSDMM_ICON_RIGHT_WINDOW_START_V-OSDMM_SELECT_FRAME_DISTANCE_V;

                GDI_BitmapCopy(&_OSDMMBitmapCopy);
                                        
                //Then clear the last dir in this page  and re-draw it.
                _OSDMM_ClearItemRegionInFileWindow(OSDMM_RIGHT_FILE_WINDOW, (OSDMM_MAX_ITEM_IN_FILE_WINDOW-1));

                _OSDMM_UpdateRightScrollBar();
            }
        }
        else if (__MMSelectCursor.wIndex < __wOSDMMStartDrawItem)
        {
            if ((__wOSDMMStartDrawItem - __MMSelectCursor.wIndex) > 1)
            {
                //update the __wStartDrawItem
                __wOSDMMStartDrawItem = __MMSelectCursor.wIndex;
                
                //clear right right file window
                _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_CONTENT_REGION); //Don't clear the right scroll bar because they are in the same folder.

                //re-draw all                
                if (__MMSelectCursor.bPosition == POS_FILE_LIST)
                {
                    _OSDMM_DrawRightFileWindow();
                }
                else if (__MMSelectCursor.bPosition == POS_PROG_LIST)
                {
                    _OSDMM_PL_DrawRightFileWindow();  
                }
            }
            else
            {
                __wOSDMMStartDrawItem--;
                
                //scroll down. Bitmap copy.
                _OSDMMBitmapCopy.bDestRegion = 0;
                _OSDMMBitmapCopy.bSrcRegion = 0;
                _OSDMMBitmapCopy.SrcRect.wLeft = OSDMM_VERTICAL_SEPARATE_FRAME_H+OSDMM_VERTICAL_SEPARATE_FRAME_WIDTH+1;
                _OSDMMBitmapCopy.SrcRect.wTop = OSDMM_ICON_RIGHT_WINDOW_START_V-OSDMM_SELECT_FRAME_DISTANCE_V;
                _OSDMMBitmapCopy.SrcRect.wRight = OSDMM_RIGHT_SCROLL_BAR_H-1;
                _OSDMMBitmapCopy.SrcRect.wBottom = OSDMM_ICON_RIGHT_WINDOW_START_V+(OSDMM_MAX_ITEM_IN_FILE_WINDOW-2)*OSDMM_ITEM_HEIGHT+OSDMM_FILE_WINDOW_FONT_HEIGHT+OSDMM_SELECT_FRAME_DISTANCE_V;
                _OSDMMBitmapCopy.wDestX = OSDMM_VERTICAL_SEPARATE_FRAME_H+OSDMM_VERTICAL_SEPARATE_FRAME_WIDTH+1;
                _OSDMMBitmapCopy.wDestY = OSDMM_ICON_RIGHT_WINDOW_START_V+OSDMM_ITEM_HEIGHT-OSDMM_SELECT_FRAME_DISTANCE_V;

                GDI_BitmapCopy(&_OSDMMBitmapCopy);
                
                //Then clear the last dir in this page and re-draw it.
                _OSDMM_ClearItemRegionInFileWindow(OSDMM_RIGHT_FILE_WINDOW, 0);

                _OSDMM_UpdateRightScrollBar();
            }
        }
        
        if (__MMSelectCursor.bPosition == POS_PROG_LIST)
        {
            _OSDMM_PL_CopyFileString((BYTE)__MMSelectCursor.wIndex);
            wStringLength = GDI_GetStringWidth_909(__pOSDUTLOutputString);
        }
        else
        {
            if (__MMSelectCursor.wIndex < __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir) //the cursor is at the folder.
            {
                _OSDMM_CopyDirString((BYTE)(__pMMVolumeDirTree[__MMSelectCursor.wDirID].b1stSubDirID+(BYTE)__MMSelectCursor.wIndex), (OSDMM_RIGHT_SCROLL_BAR_H - OSDMM_ITEM_RIGHT_WINDOW_START_H));
#ifdef SUPPORT_UNICODE_CODING
                if ((__pMMDirNameList[(__pMMVolumeDirTree[__MMSelectCursor.wDirID].b1stSubDirID+(BYTE)__MMSelectCursor.wIndex)].bMMLinkStatus & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
                {
                    wStringLength = GDI_GetUniStringWidth(__pOSDUTLOutputString);
                }
                else
#endif
                {
                    wStringLength = GDI_GetStringWidth_909(__pOSDUTLOutputString);
                }  			

                _bOSDMMIconType = OSDMM_ICON_TYPE_FOLDER; 
            }
            else //the cursor is at the file.
            {
                //Dump one file for the highlighted item.
                //MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID, (WORD)(__MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir)), 1);
                MM_DumpOneFileForHighlightItem(); //CoCo1.20, reduce code size
                //Dump file record.
                //MM_GetDumpedFileRecord(); //--CoCo1.20, reduce code size
                
                _OSDMM_CopyFileString((WORD)(__MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir));
#ifdef SUPPORT_UNICODE_CODING
                if ((__pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
                {                 
                    _bOSDMMFileLength = GDI_GetUniStringWidth(__pOSDUTLOutputString); //for draw the selected frame
                    wStringLength = GDI_GetUniStringWidth(__pOSDUTLOutputString);
                }
                else
#endif
                {                   
                _bOSDMMFileLength = GDI_GetStringWidth_909(__pOSDUTLOutputString); //for draw the selected frame
                    wStringLength = GDI_GetStringWidth_909(__pOSDUTLOutputString);
                }    
                
                if (__btProgram || (_bOSDMMUIMode == OSDMM_INITIAL_MODE_COPY))
                {
                    if (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, __MMSelectCursor.wDirID, (WORD)(__MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir+1)) != 0xFF)
                    {					
                        //draw the selected frame
                        _OSDMM_DrawSelectedFrame(OSDMM_UPDATE_DRAW, (BYTE)((__MMSelectCursor.wIndex - __wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW));
                    }
                }
                
                _bOSDMMIconType = _OSDMM_GetIconType(__pMMFileRecord->FileName.bExtID);
            }            
        }

        _OSDMM_DrawRightFileWindowHighlight(bUpdate, wStringLength);
        
        if (__MMSelectCursor.bPosition == POS_PROG_LIST)
        {
            if (bUpdate == OSDMM_UPDATE_DRAW)
                _OSDMM_PL_OutputItemString((BYTE)__MMSelectCursor.wIndex, (BYTE)((__MMSelectCursor.wIndex - __wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW), TRUE);
            else
                _OSDMM_PL_OutputItemString((BYTE)__MMSelectCursor.wIndex, (BYTE)((__MMSelectCursor.wIndex - __wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW), FALSE);

            //Show info.
            _OSDMM_PL_DrawLeftFileWindow();
        }
        else
        {
            //Draw icon
            if (bUpdate == OSDMM_UPDATE_DRAW)
                _OSDMM_DrawRightFileWindowIcon((BYTE)((__MMSelectCursor.wIndex - __wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW), _bOSDMMIconType, TRUE);
            else
                _OSDMM_DrawRightFileWindowIcon((BYTE)((__MMSelectCursor.wIndex - __wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW), _bOSDMMIconType, FALSE);
            
            //Draw string
            _OSDMM_InitializeStringPalette(OSDMM_RIGHT_FILE_WINDOW_REGION);

            if (bUpdate == OSDMM_UPDATE_DRAW)
                _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
            else
                _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;

            _OSDMMStringAttr.wX = OSDMM_ITEM_RIGHT_WINDOW_START_H;
            _OSDMMStringAttr.wY = OSDMM_ITEM_RIGHT_WINDOW_START_V+((__MMSelectCursor.wIndex -__wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW)*OSDMM_ITEM_HEIGHT;
            //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
            if (__MMSelectCursor.wIndex < __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir) //the cursor is at the folder.
            {
#ifdef SUPPORT_UNICODE_CODING
                if ((__pMMDirNameList[(__pMMVolumeDirTree[__MMSelectCursor.wDirID].b1stSubDirID+(BYTE)__MMSelectCursor.wIndex)].bMMLinkStatus & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
                {
                     _OSDMM_DrawStringByUnicode();           
                }
                else
#endif
                {
            _OSDMM_DrawString();
                }                
            }
            else //the cursor is at the file.
            {
#ifdef SUPPORT_UNICODE_CODING
                if ((__pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
                {
                     _OSDMM_DrawStringByUnicode();           
                }
                else
#endif
                {
                    _OSDMM_DrawString();  
                }              
            }
            
            if (__MMSelectCursor.wIndex >= __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir) //the cursor is at the file.
            {
                //draw serial number
                _OSDMM_DrawSerialNumber((__MMSelectCursor.wIndex -__wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW, (WORD)(__MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir+1));
            }
        }
        break;
    case POS_BUTTON_LIST:
        if (__bMMPopMenuID == MM_POP_MENU_NONE)
        {
            //When the title is "playing title", don't remove it because we can recover it from other UI.
            if (!_bOSDMMShowTime)
            {
                if (!__btProgram) //The title of Program mode and Program List is fixed. Don't clear it.
                {
                    _OSDMM_ClearRegion(OSDMM_TITLE_REGION);
                    _bOSDMMTitleType = OSDMM_TITLE_TYPE_NONE; //reset the title type for re-drawing it.
                }
            }

            if (bUpdate == OSDMM_UPDATE_DRAW)
                _OSDMM_DrawButton((BYTE)__MMSelectCursor.wIndex, OSDMM_BUTTON_HIGHLIGHT);
            else if (bUpdate == OSDMM_UPDATE_CLEAR)
            {
                _OSDMM_DrawButton((BYTE)__MMSelectCursor.wIndex, _bOSDMMButtonState[__bMMMenuID-1][__MMSelectCursor.wIndex]);
            }
        }
        else
        {
            if (bUpdate == OSDMM_UPDATE_DRAW)
                _OSDMM_DLG_DrawButton(TRUE);
            else
                _OSDMM_DLG_DrawButton(FALSE);
        }
        break;
     case POS_DLG_ITEM:
         if (__bMMPopMenuID == MM_POP_MENU_FILTER)
         {
             _OSDMM_DLG_DrawItemRegionInFileType(bUpdate, (BYTE)__MMSelectCursor.wIndex);
            
             //update the item
             if (bUpdate == OSDMM_UPDATE_DRAW)
                 _OSDMM_DLG_DrawFileTypeItem((BYTE)__MMSelectCursor.wIndex, TRUE);
             else
                 _OSDMM_DLG_DrawFileTypeItem((BYTE)__MMSelectCursor.wIndex, FALSE);             
         }
         else if (__bMMPopMenuID == MM_POP_MENU_SORT)
         {
             _OSDMM_DLG_DrawItemRegionInSortFile(bUpdate, (BYTE)__MMSelectCursor.wIndex);

             //update the item.
             if (__MMSelectCursor.wIndex < 3) //Method
             {
                 if (bUpdate == OSDMM_UPDATE_DRAW)
                 {
                     if (_bOSDMMSortFile[__MMSelectCursor.wIndex] == __bMMSortMethod)
                         _OSDMM_DLG_DrawSortMethodItem(TRUE, _bOSDMMSortFile[__MMSelectCursor.wIndex], TRUE);
                     else
                         _OSDMM_DLG_DrawSortMethodItem(FALSE, _bOSDMMSortFile[__MMSelectCursor.wIndex], TRUE);
                 }
                 else
                 {
                     if (_bOSDMMSortFile[__MMSelectCursor.wIndex] == __bMMSortMethod)
                         _OSDMM_DLG_DrawSortMethodItem(TRUE, _bOSDMMSortFile[__MMSelectCursor.wIndex], FALSE);
                     else
                         _OSDMM_DLG_DrawSortMethodItem(FALSE, _bOSDMMSortFile[__MMSelectCursor.wIndex], FALSE);
                 }
             }
             else //sequence
             {
                 if (bUpdate == OSDMM_UPDATE_DRAW)
                 {
                     if (_bOSDMMSortFile[__MMSelectCursor.wIndex] == __bMMSortSequence)
                     {
                         _OSDMM_DLG_DrawSortSequenceItem(TRUE, _bOSDMMSortFile[__MMSelectCursor.wIndex], TRUE);
                     }
                     else
                     {
                         _OSDMM_DLG_DrawSortSequenceItem(FALSE, _bOSDMMSortFile[__MMSelectCursor.wIndex], TRUE);
                     }
                 }
                 else
                 {
                     if (_bOSDMMSortFile[__MMSelectCursor.wIndex] == __bMMSortSequence)
                     {
                         _OSDMM_DLG_DrawSortSequenceItem(TRUE, _bOSDMMSortFile[__MMSelectCursor.wIndex], FALSE);
                     }
                     else
                     {
                         _OSDMM_DLG_DrawSortSequenceItem(FALSE, _bOSDMMSortFile[__MMSelectCursor.wIndex], FALSE);
                     }
                 }
             }
         }
        break;
    default:
        break;
    }    
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ClearRegion
//  Abstract    :   This function will clear the specified region.
//  Arguments   :   bRegion: The region that you want to clear. 
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ClearRegion(BYTE bRegion)
{
    _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;

    switch (bRegion)
    {
    case OSDMM_TITLE_REGION:
        _OSDMMRect.rect.wLeft = OSDMM_TITLE_START_H;
        _OSDMMRect.rect.wTop = OSDMM_TITLE_START_V;
        _OSDMMRect.rect.wRight = OSDMM_TITLE_START_H+OSDMM_TITLE_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_TITLE_START_V+OSDMM_TITLE_FONT_HEIGHT;
        break;
    case OSDMM_TIME_REGION:
        _OSDMMRect.rect.wLeft = OSDMM_TIME_START_H;
        _OSDMMRect.rect.wTop = OSDMM_TIME_START_V;
        _OSDMMRect.rect.wRight = OSDMM_TIME_START_H+OSDMM_TIME_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_TIME_START_V+OSDMM_TITLE_FONT_HEIGHT;
        break;
    case OSDMM_LEFT_FILE_WINDOW_REGION:
        _OSDMMRect.rect.wLeft = OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_THICKNESS;
        _OSDMMRect.rect.wTop = OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_THICKNESS;
        _OSDMMRect.rect.wRight = OSDMM_VERTICAL_SEPARATE_FRAME_H-1;
        _OSDMMRect.rect.wBottom = OSDMM_HORIZONTAL_SEPARATE_FRAME_V-1;
        break;
    case OSDMM_LEFT_FILE_WINDOW_CONTENT_REGION:
        _OSDMMRect.rect.wLeft = OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_THICKNESS;
        _OSDMMRect.rect.wTop = OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_THICKNESS;
        _OSDMMRect.rect.wRight = OSDMM_LEFT_SCROLL_BAR_H-1;
        _OSDMMRect.rect.wBottom = OSDMM_HORIZONTAL_SEPARATE_FRAME_V-1;
        break;
    case OSDMM_RIGHT_FILE_WINDOW_REGION:
        _OSDMMRect.rect.wLeft = OSDMM_VERTICAL_SEPARATE_FRAME_H+OSDMM_VERTICAL_SEPARATE_FRAME_WIDTH+1;
        _OSDMMRect.rect.wTop = OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_THICKNESS;
        _OSDMMRect.rect.wRight = OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_WIDTH-OSDMM_BIG_FRAME_THICKNESS;
        _OSDMMRect.rect.wBottom = OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_HEIGHT-OSDMM_BIG_FRAME_THICKNESS;
       break;
    case OSDMM_RIGHT_FILE_WINDOW_CONTENT_REGION:
        _OSDMMRect.rect.wLeft = OSDMM_VERTICAL_SEPARATE_FRAME_H+OSDMM_VERTICAL_SEPARATE_FRAME_WIDTH+1;
        _OSDMMRect.rect.wTop = OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_THICKNESS;
        _OSDMMRect.rect.wRight = OSDMM_RIGHT_SCROLL_BAR_H-1;
        _OSDMMRect.rect.wBottom = OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_HEIGHT-OSDMM_BIG_FRAME_THICKNESS;
        break;
    case OSDMM_RIGHT_SCROLL_BAR_REGION:
        _OSDMMRect.rect.wLeft = OSDMM_RIGHT_SCROLL_BAR_H;
        _OSDMMRect.rect.wTop = OSDMM_SCROLL_BAR_V;
        _OSDMMRect.rect.wRight = OSDMM_RIGHT_SCROLL_BAR_H+OSDMM_SCROLL_BAR_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_SCROLL_BAR_V+OSDMM_RIGHT_SCROLL_BAR_HEIGHT;
        break;
    case OSDMM_LEFT_SCROLL_BAR_REGION:
        _OSDMMRect.rect.wLeft = OSDMM_LEFT_SCROLL_BAR_H;
        _OSDMMRect.rect.wTop = OSDMM_SCROLL_BAR_V;
        _OSDMMRect.rect.wRight = OSDMM_LEFT_SCROLL_BAR_H+OSDMM_SCROLL_BAR_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_SCROLL_BAR_V+OSDMM_LEFT_SCROLL_BAR_HEIGHT;
        break;
    case OSDMM_RECOVER_UPPER_REGION:
        _OSDMMRect.rect.wLeft = OSDMM_TIME_START_H+OSDMM_TIME_WIDTH;
        _OSDMMRect.rect.wTop = OSDMM_BACKGROUND_V;
        _OSDMMRect.rect.wRight = OSDMM_BACKGROUND_RIGHT-OSDMM_BIG_FRAME_ND_DISTANCE;
        _OSDMMRect.rect.wBottom = OSDMM_BIG_FRAME_V-1;
        break;
    case OSDMM_HELP_REGION:
    case OSDMM_RECOVER_BOTTOM_REGION:
        _OSDMMRect.rect.wLeft = OSDMM_HELP_START_H;
        _OSDMMRect.rect.wTop = OSDMM_HELP_START_V;
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
        if (_bOSDMMUIMode == OSDMM_INITIAL_MODE_COPY)
            _OSDMMRect.rect.wRight = OSDMM_COPY_HELP_WIDTH;
        else
#endif //SUPPORT_COPY_FILE_IN_MM_UI
        _OSDMMRect.rect.wRight = OSDMM_HELP_START_H+OSDMM_HELP_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_HELP_START_V+OSDMM_HELP_HEIGHT;
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_HELP_REGION_BACKGROUND;
        break;
    case OSDMM_PREVIEW_REGION:
        _OSDMMRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
        _OSDMMRect.rect.wLeft = OSDMM_PREVIEW_WINDOW_START_H;
        _OSDMMRect.rect.wTop = OSDMM_PREVIEW_WINDOW_START_V;
        _OSDMMRect.rect.wRight = OSDMM_PREVIEW_WINDOW_START_H+OSDMM_PREVIEW_WINDOW_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_PREVIEW_WINDOW_START_V+OSDMM_PREVIEW_WINDOW_HEIGHT;
        break;
    default:
        break;
    }
    
    GDI_FillRect_909(0, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_CopyFileString
//  Abstract    :   This function will copy the name of the file to __wOSDUTLStringBuf[].
//  Arguments   :   wRelativeIndex: The index of the file.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   Before calling this API, need to dump file and fill _OSDMMFileRecord first.
//*********************************************************************************************************
void _OSDMM_CopyFileString(WORD wRelativeIndex)
{    
    WORD wOSDMMLength = 0;
    BYTE bIndex = 0;
    BYTE bCharWidth = 0;
    WORD wOutputCode = 0;

	//Entend the number of characters for file string automatically.
	//Calculate the max width for file string.
    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex(wRelativeIndex+1);

	wOSDMMLength = OSDMM_SERIAL_NUM_START_H - OSDMM_ITEM_RIGHT_WINDOW_START_H - OSDMM_SERIALNUM_FILENAME_DISTANCE - GDI_GetStringWidth_909(__pOSDUTLOutputString);

    bIndex = 0;

    if (__pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG)
    {
        // Unicode
        for (bIndex = 0; bIndex < MM_MAX_FILENAME_LENGTH; bIndex+=2)
        {
            // Each character use two bytes.
            wOutputCode = (WORD)(BYTE)(__pMMFileRecord->FileName.cName[bIndex]);
            wOutputCode |= ((WORD)(BYTE)(__pMMFileRecord->FileName.cName[bIndex+1])) << 8; 
			
            if (wOutputCode == 0 || wOutputCode == 0x2E) // End of string or extension.
            {
                break;
            }         
#ifdef SUPPORT_UNICODE_CODING
            if (__bEnCodeType != CHAR_ENCODE_ISO_CP)
            {
                __wOSDUTLStringBuf[(bIndex/2)+1]= wOutputCode;
                bCharWidth = GDI_GetUniFontWidth(__wOSDUTLStringBuf[(bIndex/2)+1]);			
            }
            else
#endif
            {
                __wOSDUTLStringBuf[(bIndex/2)+1] = CONVCHAR_Uni_To_CTK(wOutputCode);
                bCharWidth = GDI_GetCharWidth(__wOSDUTLStringBuf[(bIndex/2)+1]);
            }            
			
            if (wOSDMMLength >= bCharWidth)
            {
                wOSDMMLength -= bCharWidth;                
            }
            else
            {
                // The length of string exceed the default maximum length.
                break;
            }
        }
		
        __wOSDUTLStringBuf[0] = bIndex / 2;
    }
    else
    {
        //ASCII 
    while (__pMMFileRecord->FileName.cName[bIndex] != '\0')
    {
            __wOSDUTLStringBuf[bIndex+1] = CONVCHAR_ASCII_To_CTK(__pMMFileRecord->FileName.cName[bIndex]);

        bCharWidth = GDI_GetCharWidth(__wOSDUTLStringBuf[bIndex+1]);
		if (wOSDMMLength >= bCharWidth)
		{
			wOSDMMLength -= bCharWidth;
			bIndex++;
		}
		else
		{
			//Dont't increase bIndex because this charachter should not be added to the array.
			break;
		}

        if (bIndex > OSDMM_MAX_FILENAME_LENGTH)
        {
            bIndex = OSDMM_MAX_FILENAME_LENGTH;
            break;
        }
    }

    __wOSDUTLStringBuf[0] = bIndex;
}
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawSelectedFrame
//  Abstract    :   This function will draw or clear the frame for program.
//  Arguments   :   bUpdate: OSDMM_UPDATE_DRAW: Draw the frame.
//              :            OSDMM_UPDATE_CLEAR: Clear the frame.
//              :   bIndex: The index of the item in the right file window.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawSelectedFrame(BYTE bUpdate, BYTE bIndex)
{
    _OSDMMFrame.bStyle = FRAME_PUSH;
    _OSDMMFrame.bThickness = OSDMM_PROGRAM_FRAME_THICKNESS;
    _OSDMMFrame.bColorCenter = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDMMFrame.rect.wLeft = OSDMM_ICON_RIGHT_WINDOW_START_H - OSDMM_HIGHLIGHT_DISTANCE_H - OSDMM_SELECT_FRAME_DISTANCE_H;
    _OSDMMFrame.rect.wTop = OSDMM_ICON_RIGHT_WINDOW_START_V+bIndex*OSDMM_ITEM_HEIGHT - OSDMM_SELECT_FRAME_DISTANCE_V;
    _OSDMMFrame.rect.wRight = OSDMM_ICON_RIGHT_WINDOW_START_H+OSDMM_ICON_WIDTH+OSDMM_ICON_ITEM_DISTANCE+_bOSDMMFileLength+OSDMM_SELECT_FRAME_DISTANCE_H;
    _OSDMMFrame.rect.wBottom = OSDMM_ICON_RIGHT_WINDOW_START_V+bIndex*OSDMM_ITEM_HEIGHT+OSDMM_FILE_WINDOW_FONT_HEIGHT+OSDMM_SELECT_FRAME_DISTANCE_V;
	
    if (bUpdate == OSDMM_UPDATE_DRAW)
    {
        _OSDMMFrame.bColorBright = OSDMM_PALETTE_ENTRY_SELECT_FRAME_BRIGHT;
        _OSDMMFrame.bColorDark = OSDMM_PALETTE_ENTRY_SELECT_FRAME_DARK;
    }
    else if (bUpdate == OSDMM_UPDATE_CLEAR)
    {
        _OSDMMFrame.bColorBright = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
        _OSDMMFrame.bColorDark = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
    }

    GDI_DrawFrame_909(0, &_OSDMMFrame);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_CopyDirString
//  Abstract    :   Copy the name of the directory to the __wOSDUTLStringBuf[] array.
//  Arguments   :   bID: The ID of the directory.
//              :   bPos: POS_FILE_LIST==>the dir is in the right file window.
//              :         POS_VOL_DIR_LIST==> the dir is in the left file window.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_CopyDirString(BYTE bID, WORD wWidth)
{
    WORD wOSDMMLength = 0;
    BYTE bIndex = 0;
    BYTE bCharWidth = 0;
    WORD wOutputCode = 0;

    __pOSDUTLOutputString = __wOSDUTLStringBuf;

    wOSDMMLength = wWidth;

    bIndex = 0;

    if (__pMMDirNameList[bID].bMMLinkStatus & IF_UNICODE_FILE_TAG)
    {
        // Unicode
        for (bIndex = 0; bIndex < MM_MAX_DIRNAME_LENGTH; bIndex+=2)
        {
            // Each character use two bytes.
            wOutputCode = (WORD)(BYTE)(__pMMDirNameList[bID].cName[bIndex]);
            wOutputCode |= ((WORD)(BYTE)(__pMMDirNameList[bID].cName[bIndex+1])) << 8; 
			
            if (wOutputCode == 0) // End of string.
            {
                break;
            }
#ifdef SUPPORT_UNICODE_CODING
            if (__bEnCodeType != CHAR_ENCODE_ISO_CP)
            {
                __wOSDUTLStringBuf[(bIndex/2)+1]= wOutputCode;
                bCharWidth = GDI_GetUniFontWidth(__wOSDUTLStringBuf[(bIndex/2)+1]);
            }
            else
#endif
            {
                __wOSDUTLStringBuf[(bIndex/2)+1] = CONVCHAR_Uni_To_CTK(wOutputCode);
                bCharWidth = GDI_GetCharWidth(__wOSDUTLStringBuf[(bIndex/2)+1]);
            }      
			
            if (wOSDMMLength >= bCharWidth)
            {
                wOSDMMLength -= bCharWidth;                
            }
            else
            {
                // The length of string exceed the default maximum length.
                break;
            }
        }
		
        __wOSDUTLStringBuf[0] = bIndex / 2;
    }
    else
    {
        //ASCII 
    while (__pMMDirNameList[bID].cName[bIndex] != '\0')
    {
            __wOSDUTLStringBuf[bIndex+1] = CONVCHAR_ASCII_To_CTK(__pMMDirNameList[bID].cName[bIndex]);

        bCharWidth = GDI_GetCharWidth(__wOSDUTLStringBuf[bIndex+1]);
		if (wOSDMMLength >= bCharWidth)
		{
			wOSDMMLength -= bCharWidth;
			bIndex++;
		}
		else
		{
			//Dont't increase bIndex because this charachter should not be added to the array.
			break;
		}

        if (bIndex > OSDMM_MAX_DIRNAME_LENGTH)
        {
            bIndex = OSDMM_MAX_DIRNAME_LENGTH;
            break;
        }
    }

    __wOSDUTLStringBuf[0] = bIndex;
}
}


//*********************************************************************************************************
//  Function    :   _OSDMM_UpdateRightScrollBar
//  Abstract    :   This function will update the right scroll bar.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_UpdateRightScrollBar(void)
{    
    //total items in the right file window
    if (__bMMMenuID == MM_MENU_PROGLIST)
    {
        _wOSDMMTemp = __bProgramMaxEntry;
    }
    else //normal mode or Program mode.
    {
        _wOSDMMTemp = __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir+__pMMFilterFilesOfDir[__MMSelectCursor.wDirID];
    }

    if (_wOSDMMTemp <= OSDMM_MAX_ITEM_IN_FILE_WINDOW)
        return; //Doesn't need to draw the scroll bar.

    if ((_wOSDMMTemp - __wOSDMMStartDrawItem) < OSDMM_MAX_ITEM_IN_FILE_WINDOW) //The remenent items can't fill total page.
    {
        _bOSDMMTemp = (BYTE)(_wOSDMMTemp - __wOSDMMStartDrawItem); //display
    }
    else
    {   
        _bOSDMMTemp = OSDMM_MAX_ITEM_IN_FILE_WINDOW;
    }

    _OSDMMScrollBar.rect.wLeft = OSDMM_RIGHT_SCROLL_BAR_H;
    _OSDMMScrollBar.rect.wTop = OSDMM_SCROLL_BAR_V;
    _OSDMMScrollBar.rect.wRight = OSDMM_RIGHT_SCROLL_BAR_H+OSDMM_SCROLL_BAR_WIDTH;
    _OSDMMScrollBar.rect.wBottom = OSDMM_SCROLL_BAR_V+OSDMM_RIGHT_SCROLL_BAR_HEIGHT;
    _OSDMMScrollBar.wDisplay = ((WORD)OSDMM_RIGHT_SCROLL_BAR_HEIGHT*(WORD)_bOSDMMTemp)/_wOSDMMTemp;
    _OSDMMScrollBar.wStartPoint = (WORD)(((DWORD)OSDMM_RIGHT_SCROLL_BAR_HEIGHT*(DWORD)__wOSDMMStartDrawItem)/(DWORD)_wOSDMMTemp);

    GDI_DrawScrollBar_909(0, &_OSDMMScrollBar);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawVolume
//  Abstract    :   This function will draw the volume.
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawVolume(BYTE bIndex, BYTE bVolumeID, BYTE bHighlight)
{
    //Draw icon
    switch (__bMMVolumeSet[bVolumeID])
    {
    case MM_VOLUME_SERVO:
        _OSDMM_DrawLeftFileWindowIcon(bIndex, OSDMM_ICON_TYPE_VOLUME_SERVO, bHighlight);
        break;
    case MM_VOLUME_USB_UDISC_A:
    case MM_VOLUME_USB_UDISC_B:
        _OSDMM_DrawLeftFileWindowIcon(bIndex, OSDMM_ICON_TYPE_USB, bHighlight);
        break;
    case MM_VOLUME_CARD_SD_A:
    case MM_VOLUME_CARD_CF_A:
    case MM_VOLUME_CARD_SM_A:
    case MM_VOLUME_CARD_MMC_A:
    case MM_VOLUME_CARD_MS_A:
    case MM_VOLUME_CARD_MICRO_A:
    case MM_VOLUME_CARD_SD_B:
    case MM_VOLUME_CARD_CF_B:
    case MM_VOLUME_CARD_SM_B:
    case MM_VOLUME_CARD_MMC_B:
    case MM_VOLUME_CARD_MS_B:
    case MM_VOLUME_CARD_MICRO_B:
        _OSDMM_DrawLeftFileWindowIcon(bIndex, OSDMM_ICON_TYPE_CARD_READER, bHighlight);
        break;
    default:
        return;
    }

    _OSDMM_InitializeStringPalette(OSDMM_LEFT_FILE_WINDOW_REGION);

    //Draw the name of the volume.
    _OSDMM_SetVolumeString(bVolumeID);    

    if (bHighlight)
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;

    _OSDMMStringAttr.wX = OSDMM_ITEM_LEFT_WINDOW_START_H;
    _OSDMMStringAttr.wY = OSDMM_ITEM_LEFT_WINDOW_START_V + bIndex*OSDMM_ITEM_HEIGHT;
    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
}


//*********************************************************************************************************
//  Function    :   _OSDMM_SetVolumeString
//  Abstract    :   This function will set __pOSDUTLOutputString to the volume's string according to the ID.
//  Arguments   :   bVolumeID: the ID of the volume.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_SetVolumeString(BYTE bVolumeID)
{
    switch (__bMMVolumeSet[bVolumeID])
    {
    case MM_VOLUME_SERVO:
        __pOSDUTLOutputString = aMMDVDROMDrive;
        break;
    case MM_VOLUME_USB_UDISC_A:
        __pOSDUTLOutputString = aMMUSBFlashDiskA;
        break;
    case MM_VOLUME_USB_UDISC_B:
        __pOSDUTLOutputString = aMMUSBFlashDiskB;
        break;
    case MM_VOLUME_CARD_SD_A:
        __pOSDUTLOutputString = aMMCardSDA;
        break;
    case MM_VOLUME_CARD_CF_A:
        __pOSDUTLOutputString = aMMCardCFA;
        break;
    case MM_VOLUME_CARD_SM_A:
        __pOSDUTLOutputString = aMMCardSMA;
        break;
    case MM_VOLUME_CARD_MMC_A:
        __pOSDUTLOutputString = aMMCardMMCA;
        break;
    case MM_VOLUME_CARD_MS_A:
        __pOSDUTLOutputString = aMMCardMSA;
        break;
    case MM_VOLUME_CARD_MICRO_A:
        __pOSDUTLOutputString = aMMCardMICROA;
        break;
    case MM_VOLUME_CARD_SD_B:
        __pOSDUTLOutputString = aMMCardSDB;
        break;
    case MM_VOLUME_CARD_CF_B:
        __pOSDUTLOutputString = aMMCardCFB;
        break;
    case MM_VOLUME_CARD_SM_B:
        __pOSDUTLOutputString = aMMCardSMB;
        break;
    case MM_VOLUME_CARD_MMC_B:
        __pOSDUTLOutputString = aMMCardMMCB;
        break;
    case MM_VOLUME_CARD_MS_B:
        __pOSDUTLOutputString = aMMCardMSB;
        break;
    case MM_VOLUME_CARD_MICRO_B:
        __pOSDUTLOutputString = aMMCardMICROB;
        break;
    default:
        __pOSDUTLOutputString = aMMDVDROMDrive;
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawAllVolumes
//  Abstract    :   This function will draw all volumes in the left file window.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawAllVolumes(void)
{  
    _bOSDMMStartDrawDir = (__MMSelectCursor.bVolID / OSDMM_MAX_DIR_IN_FILE_WINDOW)*OSDMM_MAX_DIR_IN_FILE_WINDOW; //This is the volume ID

    for (_bOSDMMTemp = 0; (_bOSDMMTemp < OSDMM_MAX_DIR_IN_FILE_WINDOW) && (_bOSDMMTemp < __bMMVolumeNum); _bOSDMMTemp++)
    {
        _OSDMM_DrawVolume(_bOSDMMTemp, (_bOSDMMStartDrawDir+_bOSDMMTemp), FALSE);
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawDir
//  Abstract    :   This function will draw the directories.
//  Arguments   :   bIndex: The index of the position that the funciton starts to draw.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawDir(BYTE bIndex)
{
    _bOSDMMHaveUpDIR = _OSDMM_HaveUpDIR();

    if ((_bOSDMMStartDrawDir == 0) || ((_bOSDMMStartDrawDir == 1)&&_bOSDMMHaveUpDIR)) //ROOT
        _bOSDMMTemp = __pMMVolumeDirTree[__pMMVolumeDirTree[__MMSelectCursor.wDirID].bParentID].b1stSubDirID;
    else
    {
        if (_bOSDMMHaveUpDIR)
            _bOSDMMTemp = __pMMVolumeDirTree[__pMMVolumeDirTree[__MMSelectCursor.wDirID].bParentID].b1stSubDirID + _bOSDMMStartDrawDir - 2; //2 = ROOT+Up_Folder
        else
            _bOSDMMTemp = __pMMVolumeDirTree[__pMMVolumeDirTree[__MMSelectCursor.wDirID].bParentID].b1stSubDirID + _bOSDMMStartDrawDir - 1; //1 = ROOT
    }

    for (_bOSDMMTemp1 = 0; (_bOSDMMTemp1 < (OSDMM_MAX_DIR_IN_FILE_WINDOW - bIndex))&&(_bOSDMMTemp1 < __pMMVolumeDirTree[__pMMVolumeDirTree[__MMSelectCursor.wDirID].bParentID].bNoOfSubDir); _bOSDMMTemp1++)
    {
        //draw "Foder" icon
        if (_bOSDMMTemp == __MMSelectCursor.wDirID)
        {
            _OSDMM_DrawLeftFileWindowIcon((BYTE)(bIndex+_bOSDMMTemp1), OSDMM_ICON_TYPE_OPENFOLDER, FALSE);
        }
        else
        {
            _OSDMM_DrawLeftFileWindowIcon((BYTE)(bIndex+_bOSDMMTemp1), OSDMM_ICON_TYPE_FOLDER, FALSE);
        }

        //draw the name of the folder
        _OSDMM_InitializeStringPalette(OSDMM_LEFT_FILE_WINDOW_REGION);
        _OSDMM_CopyDirString(_bOSDMMTemp, (OSDMM_LEFT_SCROLL_BAR_H - OSDMM_ITEM_LEFT_WINDOW_START_H));
        _OSDMMStringAttr.wX = OSDMM_ITEM_LEFT_WINDOW_START_H;
        _OSDMMStringAttr.wY = OSDMM_ITEM_LEFT_WINDOW_START_V+(bIndex+_bOSDMMTemp1)*OSDMM_ITEM_HEIGHT;
        //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
#ifdef SUPPORT_UNICODE_CODING
        if ((__pMMDirNameList[_bOSDMMTemp].bMMLinkStatus & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
        {
            _OSDMM_DrawStringByUnicode();                
        }
        else
#endif
        {
        _OSDMM_DrawString();
        }     

        _bOSDMMTemp++;
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawLeftFileWindowIcon
//  Abstract    :   Call this function to draw the icon in the left file window.
//  Arguments   :   bIndex: The index of the position.
//              :   bIconType: The type of the icon.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawLeftFileWindowIcon(BYTE bIndex, BYTE bIconType, BYTE bHighlight)
{
    if (bHighlight)
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
    else
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;

    _OSDMMStringAttr.bColorKey = 0; //The color key is the same as background. Then the font's background won't be output.

    switch (bIconType)
    {
    case OSDMM_ICON_TYPE_VOLUME_SERVO:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMVolumeServoIcon;
        break;
    case OSDMM_ICON_TYPE_USB:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_VOLUME_USB_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_VOLUME_USB_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_VOLUME_USB_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMVolumeUSBIcon;
        break;
    case OSDMM_ICON_TYPE_CARD_READER:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_VOLUME_CARD_READER_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_VOLUME_CARD_READER_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_VOLUME_CARD_READER_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMVolumeCardReaderIcon;
        break;
    case OSDMM_ICON_TYPE_UPFOLDER:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_UPFOLDER_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_UPFOLDER_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_UPFOLDER_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMUpFolderIcon;
        break;
    case OSDMM_ICON_TYPE_FOLDER:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_FOLDER_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_FOLDER_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_FOLDER_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMFolderIcon;
        break;
    case OSDMM_ICON_TYPE_OPENFOLDER:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_OPENFOLDER_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_OPENFOLDER_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_OPENFOLDER_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMOpenFolderIcon;
        break;
    default:
        return;
    }    

    _OSDMMStringAttr.wX = OSDMM_ICON_LEFT_WINDOW_START_H;
    _OSDMMStringAttr.wY = OSDMM_ICON_LEFT_WINDOW_START_V+bIndex*OSDMM_ITEM_HEIGHT;
    GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDMMOutputString);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_HaveUpDIR
//  Abstract    :   Call this function to see if there is "Up_Folder" in the left file window.
//  Arguments   :   none.
//  Return		:   TRUE: There is "Up_Folder" in the left file window.
//              :   FALSE: There is not "Up_Folder" in the left file window.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
BYTE _OSDMM_HaveUpDIR(void)
{
    if (__pMMVolumeDirTree[__MMSelectCursor.wDirID].bParentID != MM_ROOT) //with "Up_DIR"
        return TRUE;
    else
        return FALSE;
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawUpDIR
//  Abstract    :   This function will draw "UP_DIR" icon and string.
//  Arguments   :   bIndex: the index.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawUpDIR(BYTE bIndex, BYTE bHighlight)
{
    //Draw "Up_DIR" icon
    _OSDMM_DrawLeftFileWindowIcon(bIndex, OSDMM_ICON_TYPE_UPFOLDER, bHighlight);
    
    _OSDMM_InitializeStringPalette(OSDMM_LEFT_FILE_WINDOW_REGION);
    if (bHighlight)
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;

    //Draw string
    __pOSDUTLOutputString = aMMUpDIR;    
    _OSDMMStringAttr.wX = OSDMM_ITEM_LEFT_WINDOW_START_H;
    _OSDMMStringAttr.wY = OSDMM_ITEM_LEFT_WINDOW_START_V+bIndex*OSDMM_ITEM_HEIGHT;
    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
}


//*********************************************************************************************************
//  Function    :   _OSDMM_UpdateLeftScrollBar
//  Abstract    :   This function will update the left scroll bar.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_UpdateLeftScrollBar(void)
{
    if (__MMSelectCursor.wDirID == MM_ROOT)
    {
        //update the scroll bar for first screen that Media Manager UI enters
        _wOSDMMTemp = __bMMVolumeNum;
    }
    else
    {
        _wOSDMMTemp = __pMMVolumeDirTree[__pMMVolumeDirTree[__MMSelectCursor.wDirID].bParentID].bNoOfSubDir + 1; //+"ROOT"
        
        if (_OSDMM_HaveUpDIR())
            _wOSDMMTemp++; //+"Up_DIR"
    }

    if (_wOSDMMTemp <= OSDMM_MAX_DIR_IN_FILE_WINDOW)
        return;

    _OSDMMScrollBar.rect.wLeft = OSDMM_LEFT_SCROLL_BAR_H;
    _OSDMMScrollBar.rect.wTop = OSDMM_SCROLL_BAR_V;
    _OSDMMScrollBar.rect.wRight = OSDMM_LEFT_SCROLL_BAR_H+OSDMM_SCROLL_BAR_WIDTH;
    _OSDMMScrollBar.rect.wBottom = OSDMM_SCROLL_BAR_V+OSDMM_LEFT_SCROLL_BAR_HEIGHT;
    _OSDMMScrollBar.wDisplay = (WORD)OSDMM_LEFT_SCROLL_BAR_HEIGHT*(WORD)OSDMM_MAX_DIR_IN_FILE_WINDOW/_wOSDMMTemp;
    _OSDMMScrollBar.wStartPoint = (WORD)(((DWORD)OSDMM_LEFT_SCROLL_BAR_HEIGHT*(DWORD)_bOSDMMStartDrawDir)/(DWORD)_wOSDMMTemp);

    GDI_DrawScrollBar_909(0, &_OSDMMScrollBar);
}


//*********************************************************************************************************
//  Function    :   _OSDMM_DrawSerialNumber
//  Abstract    :   This function will draw serial number for the file.
//  Arguments   :   bIndex: the index.
//              :   wNum: The serial number.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawSerialNumber(BYTE bIndex, WORD wNum)
{
    _OSDMM_InitializeStringPalette(OSDMM_RIGHT_FILE_WINDOW_REGION);  

    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex(wNum);
    _OSDMMStringAttr.wX = OSDMM_SERIAL_NUM_START_H - GDI_GetStringWidth_909(__pOSDUTLOutputString);
    _OSDMMStringAttr.wY = OSDMM_ITEM_RIGHT_WINDOW_START_V+bIndex*OSDMM_ITEM_HEIGHT;
    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ChangeDir
//  Abstract    :   This function will change the directory and update the content in the right file window.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ChangeDir(void)
{
    if (__MMSelectCursor.wIndex > (_bOSDMMStartDrawDir+OSDMM_MAX_DIR_IN_FILE_WINDOW-1))
    {
        if ((__MMSelectCursor.wIndex - (_bOSDMMStartDrawDir+OSDMM_MAX_DIR_IN_FILE_WINDOW-1)) > 1) //unnaturally jump
        {
            //clear left file window
            _OSDMM_ClearRegion(OSDMM_LEFT_FILE_WINDOW_CONTENT_REGION); //Don't clear the scroll bar because of the same level
            
            //re-calculate the starting item to do scroll up.
            _bOSDMMStartDrawDir = __MMSelectCursor.wIndex - OSDMM_MAX_DIR_IN_FILE_WINDOW + 1;
            
            //re-draw all content in the left file window.
            _OSDMM_DrawLeftFileWindow();
        }
        else
        {
            _bOSDMMStartDrawDir++;

            _OSDMMBitmapCopy.bDestRegion = 0;
            _OSDMMBitmapCopy.bSrcRegion = 0;
            _OSDMMBitmapCopy.SrcRect.wLeft = OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_THICKNESS+1;
            _OSDMMBitmapCopy.SrcRect.wTop = OSDMM_ICON_LEFT_WINDOW_START_V+OSDMM_ITEM_HEIGHT;
            _OSDMMBitmapCopy.SrcRect.wRight = OSDMM_LEFT_SCROLL_BAR_H-1;
            _OSDMMBitmapCopy.SrcRect.wBottom = OSDMM_ICON_LEFT_WINDOW_START_V+OSDMM_MAX_DIR_IN_FILE_WINDOW*OSDMM_ITEM_HEIGHT;
            _OSDMMBitmapCopy.wDestX = OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_THICKNESS+1;
            _OSDMMBitmapCopy.wDestY = OSDMM_ICON_LEFT_WINDOW_START_V;
            
            GDI_BitmapCopy(&_OSDMMBitmapCopy);
                        
            //Then clear the last dir in this page and re-draw it.
            _OSDMM_ClearItemRegionInFileWindow(OSDMM_LEFT_FILE_WINDOW, (OSDMM_MAX_DIR_IN_FILE_WINDOW-1));

            _OSDMM_ShowActiveDir();

            //update left scroll bar
            _OSDMM_UpdateLeftScrollBar();
        }
    }
    else if (__MMSelectCursor.wIndex < _bOSDMMStartDrawDir)
    {
        if ((_bOSDMMStartDrawDir - __MMSelectCursor.wIndex) > 1) //unnaturally jump
        {
            //clear left file window
            _OSDMM_ClearRegion(OSDMM_LEFT_FILE_WINDOW_CONTENT_REGION); //Don't clear the scroll bar because of the same level
            
            //re-calculate the starting item to do scroll down.
            _bOSDMMStartDrawDir = (BYTE)__MMSelectCursor.wIndex;
            
            //re-draw all content in the left file window.
            _OSDMM_DrawLeftFileWindow();
        }
        else
        {
            _bOSDMMStartDrawDir--;
            
            //scroll down. Bitmap copy.
            _OSDMMBitmapCopy.bDestRegion = 0;
            _OSDMMBitmapCopy.bSrcRegion = 0;
            _OSDMMBitmapCopy.SrcRect.wLeft = OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_THICKNESS+1;
            _OSDMMBitmapCopy.SrcRect.wTop = OSDMM_ICON_LEFT_WINDOW_START_V;
            _OSDMMBitmapCopy.SrcRect.wRight = OSDMM_LEFT_SCROLL_BAR_H-1;
            _OSDMMBitmapCopy.SrcRect.wBottom = OSDMM_ICON_LEFT_WINDOW_START_V+(OSDMM_MAX_DIR_IN_FILE_WINDOW-1)*OSDMM_ITEM_HEIGHT;
            _OSDMMBitmapCopy.wDestX = OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_THICKNESS+1;
            _OSDMMBitmapCopy.wDestY = OSDMM_ICON_LEFT_WINDOW_START_V+OSDMM_ITEM_HEIGHT;
            
            GDI_BitmapCopy(&_OSDMMBitmapCopy);
                
            //Then clear the last dir in this page and re-draw it.
            _OSDMM_ClearItemRegionInFileWindow(OSDMM_LEFT_FILE_WINDOW, 0);

            _OSDMM_ShowActiveDir();

            //update left scroll bar
            _OSDMM_UpdateLeftScrollBar();
        }
    }
    else
    {
        _OSDMM_ShowActiveDir();
    }

    if ((__MMSelectCursor.wIndex != 0) && (!((__MMSelectCursor.wIndex == 1) && _OSDMM_HaveUpDIR())))
    {
        //Calculate __wOSDMMStartDrawItem according to __wMMNewCursorIndex
        _OSDMM_UpdateStartDrawItemByNewCursor();
        
        _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_REGION);
        
        _OSDMM_DrawRightFileWindow();
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ShowActiveDir
//  Abstract    :   Call this function to show active item but don't highlight it.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ShowActiveDir(void)
{
    if (__MMSelectCursor.wIndex == 0)
    {
        _OSDMM_DrawVolume(0, __MMSelectCursor.bVolID, FALSE);
    }
    else if ((__MMSelectCursor.wIndex == 1) && _OSDMM_HaveUpDIR())
    {
        _OSDMM_DrawUpDIR(((__MMSelectCursor.wIndex-_bOSDMMStartDrawDir)%OSDMM_MAX_DIR_IN_FILE_WINDOW), FALSE);
    }
    else
    {
        _OSDMM_DrawLeftFileWindowIcon((__MMSelectCursor.wIndex-_bOSDMMStartDrawDir)%OSDMM_MAX_DIR_IN_FILE_WINDOW, OSDMM_ICON_TYPE_OPENFOLDER, FALSE);
        _OSDMM_CopyDirString((BYTE)__MMSelectCursor.wDirID, (OSDMM_LEFT_SCROLL_BAR_H - OSDMM_ITEM_LEFT_WINDOW_START_H));
        _OSDMM_InitializeStringPalette(OSDMM_LEFT_FILE_WINDOW_REGION);
        _OSDMMStringAttr.wX = OSDMM_ITEM_LEFT_WINDOW_START_H;
        _OSDMMStringAttr.wY = OSDMM_ITEM_LEFT_WINDOW_START_V+((__MMSelectCursor.wIndex-_bOSDMMStartDrawDir)%OSDMM_MAX_DIR_IN_FILE_WINDOW)*OSDMM_ITEM_HEIGHT;
        //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
#ifdef SUPPORT_UNICODE_CODING
        if ((__pMMDirNameList[__MMSelectCursor.wDirID].bMMLinkStatus & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
        {
            _OSDMM_DrawStringByUnicode();                
        }
        else
#endif
        {
        _OSDMM_DrawString();
    }    
}
}

//*********************************************************************************************************
//  Function    :   _OSDMM_UpdateStartDrawItemByNewCursor
//  Abstract    :   This function will update __wOSDMMStartDrawItem according to __wMMNewCursorIndex.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   For changing DIR or DIR level.
//*********************************************************************************************************
void _OSDMM_UpdateStartDrawItemByNewCursor(void)
{
    //to fix the problem about the UI will flash when user press KEY_PREV/KEY_UP to move the cursor to the last item of the previous folder.
    if (__wMMNewCursorIndex != 0)
    {
        if (__wMMNewCursorIndex >= OSDMM_MAX_ITEM_IN_FILE_WINDOW) //For KEY_PREV or KEY_UP, the cursor will be changed to the last item of the previos folder.
            __wOSDMMStartDrawItem = __wMMNewCursorIndex + 1 - OSDMM_MAX_ITEM_IN_FILE_WINDOW;
        else
            __wOSDMMStartDrawItem = 0; //total files in the DIR <= OSDMM_MAX_ITEM_IN_FILE_WINDOW
        
        //Clear the __wMMNewCursorIndex to prevent the problem about user press KEY_UP/KEY_DOWN in the left window.
        __wMMNewCursorIndex = 0;
    }
    else
        __wOSDMMStartDrawItem = 0;
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ClearItemRegionInFileWindow
//  Abstract    :   This function wil clear the region of the item in the file window.
//  Arguments   :   bFileWindow: OSDMM_LEFT_FILE_WINDOW: To clear the left file window.
//              :                OSDMM_RIGHT_FILE_WINDOW: To clear the right file window.
//              :   bIndex: The index of the item.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ClearItemRegionInFileWindow(BYTE bFileWindow, BYTE bIndex)
{
    if (bFileWindow == OSDMM_LEFT_FILE_WINDOW)
    {
        _OSDMMRect.rect.wLeft = OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_THICKNESS+1;
        _OSDMMRect.rect.wTop = OSDMM_ICON_LEFT_WINDOW_START_V+bIndex*OSDMM_ITEM_HEIGHT-OSDMM_HIGHLIGHT_DISTANCE_V;
        _OSDMMRect.rect.wRight = OSDMM_LEFT_SCROLL_BAR_H-1;
        _OSDMMRect.rect.wBottom = OSDMM_ICON_LEFT_WINDOW_START_V+(bIndex+1)*OSDMM_ITEM_HEIGHT-OSDMM_HIGHLIGHT_DISTANCE_V-1;
    }
    else //right file window
    {
        _OSDMMRect.rect.wLeft = OSDMM_VERTICAL_SEPARATE_FRAME_H+OSDMM_VERTICAL_SEPARATE_FRAME_WIDTH+1;
        _OSDMMRect.rect.wTop = OSDMM_ICON_RIGHT_WINDOW_START_V+bIndex*OSDMM_ITEM_HEIGHT-OSDMM_HIGHLIGHT_DISTANCE_V-OSDMM_SELECT_FRAME_DISTANCE_V;
        _OSDMMRect.rect.wRight = OSDMM_RIGHT_SCROLL_BAR_H-1;
        _OSDMMRect.rect.wBottom = OSDMM_ICON_RIGHT_WINDOW_START_V+bIndex*OSDMM_ITEM_HEIGHT+OSDMM_FILE_WINDOW_FONT_HEIGHT+OSDMM_HIGHLIGHT_DISTANCE_V+OSDMM_SELECT_FRAME_DISTANCE_V;
    }
    
    _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
    
    GDI_FillRect_909(0, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ChangeDirLevel
//  Abstract    :   This function will change the level of the directory.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   It's for changing the "level" of the directory not changeing directory.
//*********************************************************************************************************
void _OSDMM_ChangeDirLevel(void)
{
    _OSDMM_ClearRegion(OSDMM_LEFT_FILE_WINDOW_REGION);

    _OSDMM_DrawLeftFileWindow();

    //Calculate __wStartDrawItem according to __wMMNewCursorIndex
    _OSDMM_UpdateStartDrawItemByNewCursor();
        
    _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_REGION);

    _OSDMM_DrawRightFileWindow();
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ChangePage
//  Abstract    :   This function will change the page in the right file window.
//  Arguments   :   bChange: OSDMM_CHANGE_PAGE_NEXT: Change to nex page.
//              :            OSDMM_CHANGE_PAGE_PREVIOUS: Change to previous page.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ChangePage(BYTE bChange)
{
    if (bChange == OSDMM_CHANGE_PAGE_NEXT)
    {
        if (__bMMMenuID == MM_MENU_PROGLIST)
        {
            if ((__wOSDMMStartDrawItem+OSDMM_MAX_ITEM_IN_FILE_WINDOW) >= __bProgramMaxEntry)
                return;
        }
        else
        {
            if ((__wOSDMMStartDrawItem+OSDMM_MAX_ITEM_IN_FILE_WINDOW) >= (__pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir+__pMMFilterFilesOfDir[__MMSelectCursor.wDirID]))
                return;
        }

        __wOSDMMStartDrawItem += OSDMM_MAX_ITEM_IN_FILE_WINDOW;        
    }
    else if (bChange == OSDMM_CHANGE_PAGE_PREVIOUS)
    {
        if (__wOSDMMStartDrawItem == 0)
            return;

        if (__wOSDMMStartDrawItem < OSDMM_MAX_ITEM_IN_FILE_WINDOW)
            __wOSDMMStartDrawItem = 0;
        else
            __wOSDMMStartDrawItem -= OSDMM_MAX_ITEM_IN_FILE_WINDOW;
    }

    _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_CONTENT_REGION);

    if (__bMMMenuID == MM_MENU_PROGLIST)
    {
        _OSDMM_PL_DrawRightFileWindow();
    }
    else
    {
        _OSDMM_DrawRightFileWindow();
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_UpdateProgramItem
//  Abstract    :   This function will draw/clear the selected frame for the programmed item.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_UpdateProgramItem(void)
{
    //Dump one file for the highlighted item.
    //MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID, (WORD)(__MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir)), 1);
    MM_DumpOneFileForHighlightItem(); //CoCo1.20, reduce code size
    //Dump file record.
    //MM_GetDumpedFileRecord(); //--CoCo1.20, reduce code size

    _OSDMM_CopyFileString(__MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir);
#ifdef SUPPORT_UNICODE_CODING
    if ((__pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
    {                 
        _bOSDMMFileLength = GDI_GetUniStringWidth(__pOSDUTLOutputString); //for draw the selected frame
    }
    else
#endif
    {                   
    _bOSDMMFileLength = GDI_GetStringWidth_909(__pOSDUTLOutputString); //for draw the selected frame
    }   
    
    if (LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX, 0, __MMSelectCursor.wDirID, (WORD)(__MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir+1)) != 0xFF)
    {
        //draw the selected frame
        _OSDMM_DrawSelectedFrame(OSDMM_UPDATE_DRAW, (BYTE)((__MMSelectCursor.wIndex - __wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW));
    }
    else
        _OSDMM_DrawSelectedFrame(OSDMM_UPDATE_CLEAR, (BYTE)((__MMSelectCursor.wIndex - __wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW));
}

//*********************************************************************************************************
//  Function    :   _OSDMM_GetIconType
//  Abstract    :   This function will transform the type of the icon according to the file's extension name.
//  Arguments   :   bExtID, the extension ID.
//  Return		:   The type of the icon.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
BYTE _OSDMM_GetIconType(BYTE bExtID)
{
    switch (bExtID)
    {
    case EXTNAME_JPG:                
        _bOSDMMIconType = OSDMM_ICON_TYPE_JPEG; 
        break;
    case EXTNAME_MP2:
        _bOSDMMIconType = OSDMM_ICON_TYPE_MP2; 
        break;
    case EXTNAME_MP3:                
        _bOSDMMIconType = OSDMM_ICON_TYPE_MP3; 
        break;
    case EXTNAME_VOB: 
        _bOSDMMIconType = OSDMM_ICON_TYPE_VOB;
        break;
    case EXTNAME_AVI:
        //_bOSDMMIconType = OSDMM_ICON_TYPE_DIVX;
        _bOSDMMIconType = OSDMM_ICON_TYPE_MP4;
        break;
    case EXTNAME_DAT: 
        _bOSDMMIconType = OSDMM_ICON_TYPE_DAT;
        break;
    case EXTNAME_WMA: 
        _bOSDMMIconType = OSDMM_ICON_TYPE_WMA;
        break;
    case EXTNAME_AC3:
        _bOSDMMIconType = OSDMM_ICON_TYPE_AC3;
        break;
    case EXTNAME_MPG:
        _bOSDMMIconType = OSDMM_ICON_TYPE_MPG;
        break;
    default:
        _bOSDMMIconType = OSDMM_ICON_TYPE_NONE;
        break;    
    }

    return _bOSDMMIconType;
}

//*********************************************************************************************************
//  Function    :   _OSDMM_SetIconAttribute
//  Abstract    :   Call this function to draw the icon in the right file window.
//  Arguments   :   bIconType: the type of the icon.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_SetIconAttribute(BYTE bIconType, BYTE bHighlight)
{
    if (bHighlight)
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
    else
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;

    _OSDMMStringAttr.bColorKey = 0;

    switch (bIconType)
    {
    case OSDMM_ICON_TYPE_FOLDER:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_FOLDER_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_FOLDER_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_FOLDER_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMFolderIcon;
        break;
    case OSDMM_ICON_TYPE_JPEG:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_JPEG_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_JPEG_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_JPEG_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMJPEGIcon;
        break;
    case OSDMM_ICON_TYPE_MP3:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_MP3_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_MP3_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_MP3_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMMP3Icon;
        break;
    case OSDMM_ICON_TYPE_WMA:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_WMA_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_WMA_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_WMA_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMWMAIcon;
        break;
    case OSDMM_ICON_TYPE_DIVX:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_DIVX_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_DIVX_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_DIVX_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMDIVXIcon;
        break;
    case OSDMM_ICON_TYPE_MP4:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_MP4_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_MP4_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_MP4_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMMP4Icon;
        break;
    case OSDMM_ICON_TYPE_VOB:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_VOB_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_VOB_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_VOB_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMVOBIcon;
        break;
    case OSDMM_ICON_TYPE_MP2:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_MP2_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_MP2_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_MP2_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMMP2Icon;
        break;
    case OSDMM_ICON_TYPE_DAT:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_DAT_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_DAT_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_DAT_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMDATIcon;
        break;
    case OSDMM_ICON_TYPE_AC3:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_AC3_ICON_TEXT_FOREGROUND;
        __pOSDMMOutputString = aMMAC3Icon;
        break;
    case OSDMM_ICON_TYPE_MPG:
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_MPG_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_MPG_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_MPG_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMMPGIcon;
        break;
    default:
        return;
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawRightFileWindowIcon
//  Abstract    :   Call this function to draw the icon in the right file window.
//  Arguments   :   bIndex: the index.
//              :   bIconType: the type of the icon.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawRightFileWindowIcon(BYTE bIndex, BYTE bIconType, BYTE bHighlight)
{
    _OSDMM_SetIconAttribute(bIconType, bHighlight);

    if (bHighlight)
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
    else
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;

    _OSDMMStringAttr.wX = OSDMM_ICON_RIGHT_WINDOW_START_H;
    _OSDMMStringAttr.wY = OSDMM_ICON_RIGHT_WINDOW_START_V+bIndex*OSDMM_ITEM_HEIGHT;
    GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDMMOutputString);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ShowFileTypeDialogBox
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ShowFileTypeDialogBox(void)
{
    BYTE bIndex;

    //Draw the background of the dialog box.
    _OSDMM_DLG_DrawDlgBackground(OSDMM_DLG_FILE_TYPE);

    //Output the title string
    _OSDMM_InitializeStringPalette(OSDMM_DLG_TITLE_REGION);
    _OSDMMStringAttr.wX = OSDMM_DLG_FILE_TYPE_TITLE_START_H;
    _OSDMMStringAttr.wY = OSDMM_DLG_FILE_TYPE_TITLE_START_V;
    GDI_DrawString_909(0, &_OSDMMStringAttr, aMMDlgFileTypeTitle);
    
    //Draw the frame
    _OSDMM_SetFrameAttribute(FRAME_DOUBLE);
    _OSDMMFrame.bThickness = OSDMM_DLG_FILE_TYPE_FRAME_THICKNESS >> 1;
    _OSDMMFrame.rect.wLeft = OSDMM_DLG_FILE_TYPE_FRAME_START_H;
    _OSDMMFrame.rect.wTop = OSDMM_DLG_FILE_TYPE_FRAME_START_V;
    _OSDMMFrame.rect.wRight = OSDMM_DLG_FILE_TYPE_FRAME_START_H + OSDMM_DLG_FILE_TYPE_FRAME_WITDH;
    _OSDMMFrame.rect.wBottom = OSDMM_DLG_FILE_TYPE_FRAME_START_V + OSDMM_DLG_FILE_TYPE_FRAME_HEIGHT;
    GDI_DrawFrame_909(0, &_OSDMMFrame);        

    //Draw all items
    for (bIndex = 0; bIndex < MAX_EXTENSION_NO; bIndex++)
    {
        _OSDMM_DLG_DrawFileTypeItem((BYTE)bIndex, FALSE);
    }
    
    //Draw the button
    _OSDMM_DLG_DrawButton(FALSE);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DLG_DrawFileTypeItem
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DLG_DrawFileTypeItem(BYTE bFileType, BYTE bHighlight)
{
    _OSDMM_InitializeStringPalette(OSDMM_DLG_REGION);

    if (bHighlight)
    {
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
    }

    switch (bFileType)
    {
    case EXTNAME_MP3:
        __pOSDUTLOutputString = aMMMP3;
        _bOSDMMTemp = __bMMExtFilterMode & EXT_BIT_MP3;
        break;
    case EXTNAME_MP2:
        __pOSDUTLOutputString = aMMMP2;
        _bOSDMMTemp = __bMMExtFilterMode & EXT_BIT_MP2;
         break;
    case EXTNAME_JPG:
        __pOSDUTLOutputString = aMMJPEG;
        _bOSDMMTemp = __bMMExtFilterMode & EXT_BIT_JPG;
        break;
    case EXTNAME_WMA:
        __pOSDUTLOutputString = aMMWMA;
        _bOSDMMTemp = __bMMExtFilterMode & EXT_BIT_WMA;
        break;
    case EXTNAME_VOB:
        __pOSDUTLOutputString = aMMVOB;
        _bOSDMMTemp = __bMMExtFilterMode & EXT_BIT_VOB;
        break;
    case EXTNAME_DAT:
        __pOSDUTLOutputString = aMMDAT;
        _bOSDMMTemp = __bMMExtFilterMode & EXT_BIT_DAT;
        break;
    case EXTNAME_AVI:
        __pOSDUTLOutputString = aMMAVI;
        _bOSDMMTemp = __bMMExtFilterMode & EXT_BIT_AVI;
        break;
    case EXTNAME_MPG:
        __pOSDUTLOutputString = aMMMPG;
        _bOSDMMTemp = __bMMExtFilterMode & EXT_BIT_MPG;
        break;
    default:
        return;
    }

    _OSDMMStringAttr.wX = OSDMM_DLG_FILE_TYPE_ITEM_START_H+(bFileType/OSDMM_FILTER_DLG_ROW_NUM)*(OSDMM_DLG_FILE_TYPE_ITEM_DISTANCE_H+OSDMM_DLG_FILE_TYPE_ITEM_WIDTH);
    _OSDMMStringAttr.wY = OSDMM_DLG_FILE_TYPE_ITEM_START_V+(bFileType%OSDMM_FILTER_DLG_ROW_NUM)*(OSDMM_DLG_ITEM_DISTANCE_V+OSDMM_DLG_ITEM_HEIGHT)+((OSDMM_DLG_ITEM_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);

    _OSDMMStringAttr.wX += (OSDMM_DLG_ICON_ITEM_DISTANCE_H+OSDMM_DLG_ICON_WIDTH);

    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();

    if (_bOSDMMTemp) //Checked
    {
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_CHECKED_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_CHECKED_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_CHECKED_ICON_SHADOW_2;
        __pOSDMMOutputString = aMMDlgCheckedIcon;
    }
    else
    {
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_UNCHECKED_ICON_TEXT_FOREGROUND;
        __pOSDMMOutputString = aMMDlgUnCheckedIcon;
    }

    _OSDMMStringAttr.wX -= (OSDMM_DLG_ICON_ITEM_DISTANCE_H+OSDMM_DLG_ICON_WIDTH);
    GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDMMOutputString);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DLG_DrawRadioIcon
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DLG_DrawRadioIcon(BYTE bSet, BYTE bHighlight)
{    
    if (bSet)
    {
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_RADIO_ICON_TEXT_FOREGROUND;
        _OSDMMStringAttr.bShadow1 = OSDMM_PALETTE_ENTRY_RADIO_ICON_SHADOW_1;
        _OSDMMStringAttr.bShadow2 = OSDMM_PALETTE_ENTRY_RADIO_ICON_SHADOW_1;
        __pOSDMMOutputString = aMMDlgRadioSetIcon;
    }
    else
    {
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_UNRADIO_ICON_TEXT_FOREGROUND;
        __pOSDMMOutputString = aMMDlgRadioNotSetIcon;
    }

    _OSDMMStringAttr.wX -= (OSDMM_DLG_ICON_WIDTH+OSDMM_DLG_ICON_ITEM_DISTANCE_H);

    //Clear the region of the icon (readio/unradio box)
    if (bHighlight)
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
    else
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_DLG_BACKGROUND;

    _OSDMMRect.rect.wLeft = _OSDMMStringAttr.wX;
    _OSDMMRect.rect.wTop = _OSDMMStringAttr.wY;
    _OSDMMRect.rect.wRight = _OSDMMRect.rect.wLeft+OSDMM_DLG_ICON_WIDTH+OSDMM_DLG_ICON_ITEM_DISTANCE_H;
    _OSDMMRect.rect.wBottom = _OSDMMRect.rect.wTop+OSDMM_DLG_ITEM_HEIGHT-1; //CoCo2.38
    GDI_FillRect_909(0, &_OSDMMRect);

    GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDMMOutputString);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DLG_DrawSortMethodItem
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DLG_DrawSortMethodItem(BYTE bSet, BYTE bMethod, BYTE bHighlight)
{
    OSDUTL_PrepareOutputString();

    _OSDMM_InitializeStringPalette(OSDMM_DLG_REGION);

    if (bHighlight)
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;

    switch (bMethod)
    {
    case MM_SORT_BY_TYPE:
        OSDUTL_CopyStringFromIndex(aMMDlgMethodTypeItem);
        _OSDMMStringAttr.wX = OSDMM_DLG_SORT_FILE_METHOD_TYPE_ITEM_START_H;
        _OSDMMStringAttr.wY = OSDMM_DLG_SORT_FILE_METHOD_TYPE_ITEM_START_V;
        break;
    case MM_SORT_BY_NAME:
        OSDUTL_CopyStringFromIndex(aMMDlgMethodNameItem);
        _OSDMMStringAttr.wX = OSDMM_DLG_SORT_FILE_METHOD_NAME_ITEM_START_H;
        _OSDMMStringAttr.wY = OSDMM_DLG_SORT_FILE_METHOD_NAME_ITEM_START_V;
        break;
    case MM_SORT_BY_TIME :
        OSDUTL_CopyStringFromIndex(aMMDlgMethodTimeItem);
        _OSDMMStringAttr.wX = OSDMM_DLG_SORT_FILE_METHOD_TIME_ITEM_START_H;
        _OSDMMStringAttr.wY = OSDMM_DLG_SORT_FILE_METHOD_TIME_ITEM_START_V;
        break;
    default:
        return;
    }

    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();

    _OSDMM_DLG_DrawRadioIcon(bSet, bHighlight);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DLG_DrawSortSequenceItem
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DLG_DrawSortSequenceItem(BYTE bSet, BYTE bSequence, BYTE bHighlight)
{
    OSDUTL_PrepareOutputString();

    _OSDMM_InitializeStringPalette(OSDMM_DLG_REGION);

    if (bHighlight)
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;

    if (bSequence == MM_SORT_BY_ASCENDING)
    {
        OSDUTL_CopyStringFromIndex(aMMDlgSequenceAscendingItem);
        _OSDMMStringAttr.wX = OSDMM_DLG_SORT_FILE_SEQUENCE_ASCENDING_ITEM_START_H;
        _OSDMMStringAttr.wY = OSDMM_DLG_SORT_FILE_SEQUENCE_ASCENDING_ITEM_START_V;
    }
    else if (bSequence == MM_SORT_BY_DESCENDING)
    {
        OSDUTL_CopyStringFromIndex(aMMDlgSequenceDescendingItem);
        _OSDMMStringAttr.wX = OSDMM_DLG_SORT_FILE_SEQUENCE_DESCENDING_ITEM_START_H;
        _OSDMMStringAttr.wY = OSDMM_DLG_SORT_FILE_SEQUENCE_DESCENDING_ITEM_START_V;
    }

    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();

    _OSDMM_DLG_DrawRadioIcon(bSet, bHighlight);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ShowSortFileDialogBox
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ShowSortFileDialogBox(void)
{
    //Draw the background of the dialog box.
    _OSDMM_DLG_DrawDlgBackground(OSDMM_DLG_SORT_FILE);

    //Output the title string
    _OSDMM_InitializeStringPalette(OSDMM_DLG_TITLE_REGION);
    _OSDMMStringAttr.wX = OSDMM_DLG_SORT_FILE_TITLE_START_H;
    _OSDMMStringAttr.wY = OSDMM_DLG_SORT_FILE_TITLE_START_V;
    GDI_DrawString_909(0, &_OSDMMStringAttr, aMMDlgSortFileTitle);

    /*
    //Draw the deparate frame
    _OSDMM_SetFrameAttribute(FRAME_DOUBLE_SOLID);
    _OSDMMFrame.bThickness = OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_THICKNESS >> 1;
    _OSDMMFrame.rect.wLeft = OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_H;
    _OSDMMFrame.rect.wTop = OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_V;
    _OSDMMFrame.rect.wRight = OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_H+OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_WIDTH;
    _OSDMMFrame.rect.wBottom = OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_V+OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_HEIGHT;
    GDI_DrawFrame_909(0, &_OSDMMFrame);
    */

    //draw the sub-title, "Method:"
    _OSDMM_InitializeStringPalette(OSDMM_DLG_REGION);
    _OSDMMStringAttr.wX = OSDMM_DLG_SORT_FILE_SUB_TITLE_METHOD_START_H;
    _OSDMMStringAttr.wY = OSDMM_DLG_SORT_FILE_SUB_TITLE_METHOD_START_V;
    GDI_DrawString_909(0, &_OSDMMStringAttr, aMMDlgSortFileMethodTitle);

    //Draw the "Method" frame
    _OSDMM_SetFrameAttribute(FRAME_DOUBLE);
    _OSDMMFrame.bThickness = OSDMM_DLG_SORT_FILE_METHOD_FRAME_THICKNESS >> 1;
    _OSDMMFrame.rect.wLeft = OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_H;
    _OSDMMFrame.rect.wTop = OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_V;
    _OSDMMFrame.rect.wRight = OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_H + OSDMM_DLG_SORT_FILE_METHOD_FRAME_WIDTH;
    _OSDMMFrame.rect.wBottom = OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_V + OSDMM_DLG_SORT_FILE_METHOD_FRAME_HEIGHT;
    GDI_DrawFrame_909(0, &_OSDMMFrame);      

    //draw the sub-title, "Sequence:"
    _OSDMMStringAttr.wX = OSDMM_DLG_SORT_FILE_SUB_TITLE_SEQUENCE_START_H;
    _OSDMMStringAttr.wY = OSDMM_DLG_SORT_FILE_SUB_TITLE_SEQUENCE_START_V;
    GDI_DrawString_909(0, &_OSDMMStringAttr, aMMDlgSortFileSequenceTitle);

    //Draw the "Sequence" frame
    _OSDMM_SetFrameAttribute(FRAME_DOUBLE);
    _OSDMMFrame.bThickness = OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_THICKNESS >> 1;
    _OSDMMFrame.rect.wLeft = OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_START_H;
    _OSDMMFrame.rect.wTop = OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_START_V;
    _OSDMMFrame.rect.wRight = OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_START_H + OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_WIDTH;
    _OSDMMFrame.rect.wBottom = OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_START_V + OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_HEIGHT;
    GDI_DrawFrame_909(0, &_OSDMMFrame);      

    //Draw Method items.
    _OSDMM_DLG_DrawAllMethodItemsInSortFile(FALSE);

    //Draw Sequence items.
    _OSDMM_DLG_DrawAllSequenceItemsInSortFile(FALSE);

    //Draw the button
    _OSDMM_DLG_DrawButton(FALSE);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_PL_CopyFileString
//  Abstract    :   This function will copy the name of the file to __wOSDUTLStringBuf[].
//  Arguments   :   wRelativeIndex: The index of the file.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_PL_CopyFileString(BYTE bProgramIndex)
{    
    BYTE bOSDMMTemp = 0;

    WORD wOSDMMLength = 0;

	wOSDMMLength = OSDMM_RIGHT_SCROLL_BAR_H-OSDMM_SCROLLBAR_SERIALNUM_DISTANCE;

    while (__pMMProgContent[bProgramIndex].cName[bOSDMMTemp] != '\0')
    {
        __wOSDUTLStringBuf[bOSDMMTemp+1] = OSDUTL_TransformChar(__pMMProgContent[bProgramIndex].cName[bOSDMMTemp]);

		if (wOSDMMLength >= ((__wOSDUTLStringBuf[bOSDMMTemp+1] & 0xF000) >> 10))
		{
			wOSDMMLength -= ((__wOSDUTLStringBuf[bOSDMMTemp+1] & 0xF000) >> 10);
			bOSDMMTemp++;
		}
		else
		{
			//Dont't increase bOSDMMTemp because this charachter should not be added to the array.
			break;
		}

        if (bOSDMMTemp > OSDMM_MAX_FILENAME_LENGTH)
        {
            bOSDMMTemp = OSDMM_MAX_FILENAME_LENGTH;
            break;
        }
    }
    
    __wOSDUTLStringBuf[0] = bOSDMMTemp;

    __pOSDUTLOutputString = __wOSDUTLStringBuf;
}


//*********************************************************************************************************
//  Function    :   _OSDMM_PL_OutputItemString
//  Abstract    :   This function will draw the icon and string for one programmed item.
//  Arguments   :   bProgramIndex: the index of the program item.
//              :   bPosIndex: the index of the position in the file window.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_PL_OutputItemString(BYTE bProgramIndex, BYTE bPosIndex, BYTE bHighlight)
{
    //Draw icon
    if (bHighlight)
        _OSDMM_SetIconAttribute(_OSDMM_GetIconType(__pMMProgContent[bProgramIndex].bExtID), TRUE);
    else
        _OSDMM_SetIconAttribute(_OSDMM_GetIconType(__pMMProgContent[bProgramIndex].bExtID), FALSE);

    _OSDMMStringAttr.wX = OSDMM_ICON_RIGHT_WINDOW_START_H;
    _OSDMMStringAttr.wY = OSDMM_ICON_RIGHT_WINDOW_START_V+bPosIndex*OSDMM_ITEM_HEIGHT;
    GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDMMOutputString);

    //Draw the string.
    _OSDMM_InitializeStringPalette(OSDMM_RIGHT_FILE_WINDOW_REGION);

    if (bHighlight)
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
        
    _OSDMM_PL_CopyFileString(bProgramIndex);

    _OSDMMStringAttr.wX = OSDMM_ITEM_RIGHT_WINDOW_START_H;
    _OSDMMStringAttr.wY = OSDMM_ITEM_RIGHT_WINDOW_START_V+bPosIndex*OSDMM_ITEM_HEIGHT;

    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
}


//*********************************************************************************************************
//  Function    :   _OSDMM_PL_DrawRightFileWindow
//  Abstract    :   This function will draw the right file window for Program List UI.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_PL_DrawRightFileWindow(void)
{
    for (_bOSDMMTemp = 0; _bOSDMMTemp < OSDMM_MAX_ITEM_IN_FILE_WINDOW; _bOSDMMTemp++)    
    {
        if ((__wOSDMMStartDrawItem+_bOSDMMTemp) >= __bProgramMaxEntry) //Exceed the total programmed items?
            break;

        _OSDMM_PL_OutputItemString(__wOSDMMStartDrawItem+_bOSDMMTemp, _bOSDMMTemp, FALSE);
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_PL_DrawLeftFileWindow
//  Abstract    :   This function will draw the left file window to update the file info. for Program List UI.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_PL_DrawLeftFileWindow(void)
{
    _OSDMM_ClearRegion(OSDMM_LEFT_FILE_WINDOW_REGION);

    _dwOSDMMTemp = MM_ID_RelativeToAbsolute(__ProgIdxList[__MMSelectCursor.wIndex].bMainIndex,(__ProgIdxList[__MMSelectCursor.wIndex].wSubIndex-1));
    __pMMFileRecord = __pMMFileRecordStart;
    __pMMFileRecord += _dwOSDMMTemp;

    //Draw the string.
    _OSDMM_InitializeStringPalette(OSDMM_LEFT_FILE_WINDOW_REGION);
    _OSDMMStringAttr.wX = OSDMM_PROGRAM_LIST_INFO_DATE_START_H;
    _OSDMMStringAttr.wY = OSDMM_PROGRAM_LIST_INFO_DATE_START_V;

    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aMMPLHelpDate);
    OSDUTL_OutputNumFromIndex(MM_GetFileInfo(MM_GET_FILE_YEAR));
    OSDUTL_CopyStringFromIndex(aMMSlash);		
    OSDUTL_OutputNumFromIndex(MM_GetFileInfo(MM_GET_FILE_MONTH));
    OSDUTL_CopyStringFromIndex(aMMSlash);		
    OSDUTL_OutputNumFromIndex(MM_GetFileInfo(MM_GET_FILE_DATE));

    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
    /*
    printf("\nHour: %lx! ",MM_GetFileInfo(MM_GET_FILE_HOUR));
    printf("\nMin: %lx! ",MM_GetFileInfo(MM_GET_FILE_MIN));
    printf("\nSec: %lx! ",MM_GetFileInfo(MM_GET_FILE_SEC));
    */

    _OSDMMStringAttr.wX = OSDMM_PROGRAM_LIST_INFO_FILE_SIZE_START_H;
    _OSDMMStringAttr.wY = OSDMM_PROGRAM_LIST_INFO_FILE_SIZE_START_V;

    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aMMPLHelpSize);
    OSDUTL_OutputNumFromIndex(MM_GetFileInfo(MM_GET_FILE_LENGTH)*2); //unit:KB
    OSDUTL_CopyStringFromIndex(aMMPLHelpSizeKB);

    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
}

//*********************************************************************************************************
//  Function    :   _OSDMM_PrepareUpdateTitle
//  Abstract    :   Call this function to prepare to update the title string.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_PrepareUpdateTitle(void)
{
    _bOSDMMTemp = _bOSDMMTitleType;
    _bOSDMMUpdate = FALSE;
    
    if (__bMMMenuID == MM_MENU_PROGLIST)
    {
        _bOSDMMTitleType = OSDMM_TITLE_TYPE_PROGRAM_LIST;
    }
    else if (__bMMMenuID == MM_MENU_EDITPROG)
    {
        _bOSDMMTitleType = OSDMM_TITLE_TYPE_PROGRAM;
    }
    else //general mode
    {
        switch (__MMSelectCursor.bPosition)
        {
        case POS_VOL_DIR_LIST:
            if ((__MMSelectCursor.wIndex == 1) && _OSDMM_HaveUpDIR())
                _bOSDMMTitleType = OSDMM_TITLE_TYPE_UP_FOLDER;
            else
                _bOSDMMTitleType = OSDMM_TITLE_TYPE_DIR;
            break;
        case POS_FILE_LIST:
            if (__MMSelectCursor.wIndex < __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir) //the cusor is at the folder
                _bOSDMMTitleType = OSDMM_TITLE_TYPE_FILE_FOLDER;
            else
                _bOSDMMTitleType = OSDMM_TITLE_TYPE_FILE;
            break;
        default:
            break;
        }
    }

    if (_bOSDMMTemp != _bOSDMMTitleType)
        _bOSDMMUpdate = TRUE;
}

//*********************************************************************************************************
//  Function    :   _OSDMM_UpdateTitle
//  Abstract    :   Call this function to update the title string.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_UpdateTitle(void)
{
	//show playing file in the title.
	if (_bOSDMMPlayingTitle && (!__btInputProgram))
		return;

    _OSDMM_PrepareUpdateTitle();

    if (_bOSDMMUpdate)
    {
        //clear title region
        _OSDMM_ClearRegion(OSDMM_TITLE_REGION);
    }
    else
    {
        if (__btProgram)
            return;
    }

    _OSDMM_InitializeStringPalette(OSDMM_TITLE_REGION);
    _OSDMMStringAttr.wX = OSDMM_TITLE_START_H;    
    _OSDMMStringAttr.wY = OSDMM_TITLE_START_V;

    switch (_bOSDMMTitleType)
    {
    case OSDMM_TITLE_TYPE_PROGRAM:
        GDI_DrawString_909(0, &_OSDMMStringAttr, aMMProgramModeTitle);
        break;
    case OSDMM_TITLE_TYPE_PROGRAM_LIST:
        GDI_DrawString_909(0, &_OSDMMStringAttr, aMMProgramListModeTitle);
        break;
    case OSDMM_TITLE_TYPE_DIR: //The folder is at the left file window.
        _OSDMM_UpdateFolderTitle((BYTE)__MMSelectCursor.wDirID);
        break;
    case OSDMM_TITLE_TYPE_FILE_FOLDER: //The folder is at the right file window.
        _OSDMM_UpdateFolderTitle((BYTE)(__pMMVolumeDirTree[__MMSelectCursor.wDirID].b1stSubDirID+__MMSelectCursor.wIndex));
        break;
    case OSDMM_TITLE_TYPE_FILE:
        _OSDMM_UpdateFileTitle();
        break;
    case OSDMM_TITLE_TYPE_UP_FOLDER:
    default:
        break;
    }               
}

//*********************************************************************************************************
//  Function    :   _OSDMM_UpdateFolderTitle
//  Abstract    :   This function will update the title for the folder.
//  Arguments   :   bDirID: The ID of the directory.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_UpdateFolderTitle(BYTE bDirID)
{    
    _OSDMM_InitializeStringPalette(OSDMM_TITLE_REGION);
  
    _OSDMMStringAttr.wY = OSDMM_TITLE_START_V;

    __pOSDUTLOutputString = aMMTotalFiles;

    if (_bOSDMMUpdate)
    {
        _OSDMMStringAttr.wX = OSDMM_TITLE_START_H;
        //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
        _OSDMM_DrawString();
    }

    _OSDMMStringAttr.wX = OSDMM_TITLE_START_H + GDI_GetStringWidth_909(__pOSDUTLOutputString);
    
    if (!_bOSDMMUpdate)
        _OSDMM_ClearTitleSubRegion(_OSDMMStringAttr.wX);

    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex(__pMMFilterFilesOfDir[bDirID]);    
    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
}

//***************************************************************************
//  Function    :   _OSDMM_UpdateFileTitle
//  Abstract    :   This function will update the title for the file.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDMM_UpdateFileTitle(void)
{
    _OSDMM_InitializeStringPalette(OSDMM_TITLE_REGION);
    
    _OSDMMStringAttr.wY = OSDMM_TITLE_START_V;    
    __pOSDUTLOutputString = aMMFileNumber;

    if (_bOSDMMUpdate)
    {
        _OSDMMStringAttr.wX = OSDMM_TITLE_START_H;
        //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
        _OSDMM_DrawString();
    }

    _OSDMMStringAttr.wX = OSDMM_TITLE_START_H + GDI_GetStringWidth_909(__pOSDUTLOutputString);
    if (!_bOSDMMUpdate)
        _OSDMM_ClearTitleSubRegion(_OSDMMStringAttr.wX);

    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex((WORD)(__MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir + 1));
    OSDUTL_CopyStringFromIndex(aMMSlash);
    OSDUTL_OutputNumFromIndex((WORD)(__pMMFilterFilesOfDir[__MMSelectCursor.wDirID]));

    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ClearTitleSubRegion
//  Abstract    :   This function will clear the information part of the title.
//  Arguments   :   wPos: the starting position of the information part.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ClearTitleSubRegion(WORD wPos)
{
    _OSDMMRect.rect.wLeft = wPos;
    _OSDMMRect.rect.wTop = OSDMM_TITLE_START_V;
    _OSDMMRect.rect.wRight = OSDMM_TITLE_START_H+OSDMM_TITLE_WIDTH;
    _OSDMMRect.rect.wBottom = OSDMM_TITLE_START_V+OSDMM_TITLE_FONT_HEIGHT;
    
    _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
    
    GDI_FillRect_909(0, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawButton
//  Abstract    :   This function will draw the button.
//  Arguments   :   bIndex: index of the button.
//              :   bHighlight: the button is enabled or selected.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawButton(BYTE bIndex, BYTE bButtonState)
{
#ifdef NO_MM_BUTTON //CoCo2.38
    if (_bOSDMMUIMode != OSDMM_INITIAL_MODE_COPY)
    {
        return;
    }
#endif //NO_MM_BUTTON

    //Draw the rectangle to represent the button. For PC-908 test only.

    if (_bOSDMMUIMode == OSDMM_INITIAL_MODE_COPY)
    {
        _OSDMMRect.rect.wLeft = OSDMM_COPY_BUTTON_START_H+bIndex*OSDMM_BUTTON_DISTANCE_H+bIndex*OSDMM_BUTTON_WIDTH;
        _OSDMMRect.rect.wRight = OSDMM_COPY_BUTTON_START_H+bIndex*OSDMM_BUTTON_DISTANCE_H+(bIndex+1)*OSDMM_BUTTON_WIDTH;
    }
    else
    {
        _OSDMMRect.rect.wLeft = OSDMM_BUTTON_START_H+bIndex*OSDMM_BUTTON_DISTANCE_H+bIndex*OSDMM_BUTTON_WIDTH;
        _OSDMMRect.rect.wRight = OSDMM_BUTTON_START_H+bIndex*OSDMM_BUTTON_DISTANCE_H+(bIndex+1)*OSDMM_BUTTON_WIDTH;
    }

    _OSDMMRect.rect.wTop = OSDMM_BUTTON_START_V;      
    _OSDMMRect.rect.wBottom = OSDMM_BUTTON_START_V+OSDMM_BUTTON_HEIGHT;
    
    switch (bButtonState)
    {
    case OSDMM_BUTTON_HIGHLIGHT:
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND;
        break;
    case OSDMM_BUTTON_DISABLE:
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_BUTTON_DISABLE_BACKGROUND;
        break;
    case OSDMM_BUTTON_ENABLE:
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND;
    default:
        break;
    }

    GDI_FillRoundRect(0, OSDMM_BUTTON_ARC_VALUE, &_OSDMMRect);

    //Fill the frame of round rectangle
    _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_BUTTON_FRAME;

    _OSDMMRect.rect.wLeft -= OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wTop -= OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wRight += OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wBottom += OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, OSDMM_BUTTON_OUTER_ARC_VALUE, OSDMM_BUTTON_ARC_VALUE, &_OSDMMRect);

    _OSDMM_InitializeStringPalette(OSDMM_BUTTON_REGION);

    if (bButtonState == OSDMM_BUTTON_HIGHLIGHT)
    {
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BUTTON_STRING;
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND;
    }
    else if (bButtonState == OSDMM_BUTTON_DISABLE)
    {
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_DISABLE_BUTTON_STRING;
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_BUTTON_DISABLE_BACKGROUND;
    }

    if (__bMMMenuID == MM_MENU_NORMAL)
    {
        if (bIndex == 0)
        {
            __pOSDUTLOutputString = aMMBtnFileType;
        }
        else
        {
            __pOSDUTLOutputString = aMMBtnSortFile;
        }        
    }
    else if (__bMMMenuID == MM_MENU_EDITPROG)
    {
        if (bIndex == 0)
        {
            __pOSDUTLOutputString = aMMBtnList;
        }
        else
        {
            __pOSDUTLOutputString = aMMBtnPlay;
        }            
    }
    else if (__bMMMenuID == MM_MENU_PROGLIST)
    {
        if (bIndex == 0)
        {
            __pOSDUTLOutputString = aMMBtnDeleteAll;
        }
        else
        {
            __pOSDUTLOutputString = aMMBtnReturn;
        }            
    }
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
    else if (__bMMMenuID == MM_MENU_ENCODE_FILE)
    {
        if (bIndex == 0)
        {
            __pOSDUTLOutputString = aMMBtnSave;
        }
        else
        {
            __pOSDUTLOutputString = aMMBtnExit;
        }            
    }
#endif //SUPPORT_COPY_FILE_IN_MM_UI

    _OSDMMStringAttr.wX = _OSDMMRect.rect.wLeft+OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL+((OSDMM_BUTTON_WIDTH-GDI_GetStringWidth_909(__pOSDUTLOutputString)) >> 1);
    _OSDMMStringAttr.wY = _OSDMMRect.rect.wTop+OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL+((OSDMM_BUTTON_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);

    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawAllButtons
//  Abstract    :   This function will draw all buttons for the UI mode.
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawAllButtons(void)
{
#ifdef NO_MM_BUTTON
    if (_bOSDMMUIMode != OSDMM_INITIAL_MODE_COPY)
        return;
#endif //NO_MM_BUTTON

    //Draw the buttons.
    _OSDMM_DrawButton(0, _bOSDMMButtonState[__bMMMenuID-1][0]);
    _OSDMM_DrawButton(1, _bOSDMMButtonState[__bMMMenuID-1][1]);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawRightFileWindowHighlight
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawRightFileWindowHighlight(BYTE bUpdate, BYTE bFileLength)
{
    //Draw/clear highlight
    _OSDMMRect.rect.wLeft = OSDMM_ICON_RIGHT_WINDOW_START_H - OSDMM_HIGHLIGHT_DISTANCE_H;
    _OSDMMRect.rect.wTop = OSDMM_ITEM_RIGHT_WINDOW_START_V+((__MMSelectCursor.wIndex -__wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW)*OSDMM_ITEM_HEIGHT-OSDMM_HIGHLIGHT_DISTANCE_V;            
    _OSDMMRect.rect.wRight = OSDMM_ITEM_RIGHT_WINDOW_START_H+bFileLength+OSDMM_HIGHLIGHT_DISTANCE_H;
    _OSDMMRect.rect.wBottom = OSDMM_ITEM_RIGHT_WINDOW_START_V+((__MMSelectCursor.wIndex -__wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW)*OSDMM_ITEM_HEIGHT+OSDMM_FILE_WINDOW_FONT_HEIGHT+OSDMM_HIGHLIGHT_DISTANCE_V;
    
    if (bUpdate == OSDMM_UPDATE_DRAW)
    {
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
    }
    else
    {
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
    }
    
    GDI_FillRect_909(0, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_UpdateHelp
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_UpdateHelp(BYTE bHelpType)
{
    WORD wStartH = OSDMM_HELP_STRING_START_H;

    if (__dwMMJPEGPreview) //CoCo1.11, to prevent the help exists with the progress bar.    
        return;

    //clear help region
    _OSDMM_DrawInfoRegion();

    _OSDMM_InitializeStringPalette(OSDMM_HELP_REGION);

    _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HELP_REGION_BACKGROUND;

    _bOSDMMHelpType = bHelpType; //Save the help type.

    //NB forever message has the high priority.
    if (OSD_RecoverNBForeverMsg())
        return;
    else if (bHelpType == OSDMM_HELP_TYPE_NONE)
    {
        if ((__MMSelectCursor.bPosition == POS_FILE_LIST) && (__MMSelectCursor.wIndex >= __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir)) //Need to show the file. info like JPEG, MP3, etc.
        {
            //Dump one file for the highlighted item.
            //MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID, (WORD)(__MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir)), 1);
            MM_DumpOneFileForHighlightItem(); //CoCo1.20, reduce code size
        }
        else if (__btPlaying) //CoCo1.10, recover audio help when the cursor is at the left file window.             
        {
            //Dump one file for the highlighted item.
            MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMPlayingItem.wDirID, (WORD)(__MMPlayingItem.wIndex - __pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir)), 1);
            MM_GetDumpedFileRecord(); //--CoCo1.20, reduce code size
        }
        else 
            return;
        
        //Dump file record.
        //MM_GetDumpedFileRecord(); //--CoCo1.20, reduce code size
        
        switch (__pMMFileRecord->FileName.bExtID)
        {
        case EXTNAME_MP3:
            bHelpType = OSDMM_HELP_TYPE_MP3;
            break;
        case EXTNAME_MP2:
            bHelpType = OSDMM_HELP_TYPE_MP2;
            break;
        case EXTNAME_JPG:
            bHelpType = OSDMM_HELP_TYPE_JPEG;
            break;
        case EXTNAME_WMA:
            bHelpType = OSDMM_HELP_TYPE_WMA;
            break;
        case EXTNAME_VOB:
            bHelpType = OSDMM_HELP_TYPE_VOB;
            break;
        case EXTNAME_DAT:
            bHelpType = OSDMM_HELP_TYPE_DAT;
            break;
        case EXTNAME_MPG:
            bHelpType = OSDMM_HELP_TYPE_MPG;
            break;
        default:
            return;
        }
     }

    OSDUTL_PrepareOutputString();

    //Draw string from the starting position of help region.
    switch (bHelpType)
    {
    case OSDMM_HELP_TYPE_PROGRAM:
        //Load play icon
        OSDUTL_CopyStringFromIndex(aMMHelpPlayIcon);
        OSDUTL_CopyStringFromIndex(aMMHelpProgram);
        break;
    case OSDMM_HELP_TYPE_PROGRAM_LIST:
        OSDUTL_CopyStringFromIndex(aMMHelpPlayIcon);
        OSDUTL_CopyStringFromIndex(aMMHelpProgramList);
        break;
    case OSDMM_HELP_TYPE_JPEG:
        OSDUTL_ShowJPEGResolution();
        break;
    case OSDMM_HELP_TYPE_MP3:
    case OSDMM_HELP_TYPE_MP2:
    case OSDMM_HELP_TYPE_WMA:
        wStartH = _OSDMM_ShowAudioInfo(wStartH);
        if (wStartH == OSDMM_HELP_STRING_START_H) //No audio information
        {
            return;
        }
        break;    
    case OSDMM_HELP_TYPE_DIVX:
        return; //Return directly because we don't show any info. for it currently
    case OSDMM_HELP_TYPE_VOB:
        return; //Return directly because we don't show any info. for it currently
    case OSDMM_HELP_TYPE_DAT:
    case OSDMM_HELP_TYPE_MPG:
        return; //Return directly because we don't show any info. for it currently
    default:
        return;
    }

    _OSDMMStringAttr.wX = wStartH;
    _OSDMMStringAttr.wY = OSDMM_HELP_START_V+((OSDMM_HELP_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);
    
    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();

    //Draw string from the end position of help region.
    switch (bHelpType)
    {
    case OSDMM_HELP_TYPE_JPEG:
        OSDUTL_ShowJPEGDate(__MMSelectCursor.wDirID, __MMSelectCursor.wIndex);
        break;
    case OSDMM_HELP_TYPE_MP3:
    case OSDMM_HELP_TYPE_MP2:
    case OSDMM_HELP_TYPE_WMA:
        _OSDMM_ShowAudioTotalTime();
        __bOSDForceToClearNBRegion = TRUE; //CoCo1.20, fix the bug about 
        break;
    case OSDMM_HELP_TYPE_DIVX:
        return; //Return directly because we don't show any info. for it currently
    case OSDMM_HELP_TYPE_VOB:
        return; //Return directly because we don't show any info. for it currently
    case OSDMM_HELP_TYPE_DAT:
    case OSDMM_HELP_TYPE_MPG:
        return; //Return directly because we don't show any info. for it currently
    default:
        return;
    }

#ifdef SUPPORT_COPY_FILE_IN_MM_UI
    if (_bOSDMMUIMode == OSDMM_INITIAL_MODE_COPY)
        _OSDMMStringAttr.wX = OSDMM_HELP_START_H+OSDMM_COPY_HELP_WIDTH-GDI_GetStringWidth_909(__pOSDUTLOutputString)-OSDMM_HELP_REGION_STRING_DISTANCE;
    else
#endif //SUPPORT_COPY_FILE_IN_MM_UI
    _OSDMMStringAttr.wX = OSDMM_HELP_START_H+OSDMM_HELP_WIDTH-GDI_GetStringWidth_909(__pOSDUTLOutputString)-OSDMM_HELP_REGION_STRING_DISTANCE;
    _OSDMMStringAttr.wY = OSDMM_HELP_START_V+((OSDMM_HELP_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);

    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ChangProramModeToNormal
//  Abstract    :   
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ChangProramModeToNormal(void)
{
    //CoCo1.10, fix the bug about the "Program" title is not cleared when Play==>Pause==>PRG==>PRG.
    if ((__btPlaying || __btPause) && (__bAttrPlay != ATTR_JPG)) //CoCo1.02, JPEG preview may set __btPlaying to TRUE
    {
        _OSDMM_UpdatePlayingTitle();
    }
    else
    {
        _OSDMM_UpdateTitle();
    }
   
    //Draw right file window.
    _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_CONTENT_REGION); //Don't clear the right scroll bar because they are in the same folder.
    _OSDMM_DrawRightFileWindow();
        
    _OSDMM_UpdateHelp(OSDMM_HELP_TYPE_NONE);
}


//*********************************************************************************************************
//  Function    :   _OSDMM_ChangMode
//  Abstract    :   This function will change the UI mode.
//  Arguments   :   bChangeMode: change the UI mode.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ChangMode(BYTE bChangeMode)
{
    //General mode to program mode.
    switch (bChangeMode)
    {
    case OSDMM_CHANGE_MODE_NORMAL_TO_PROGRAM:
        _OSDMM_UpdateTitle();
        _OSDMM_UpdateHelp(OSDMM_HELP_TYPE_PROGRAM);
        break;
    case OSDMM_CHANGE_MODE_PROGRAM_TO_PROGRAM_LIST:
        _OSDMM_UpdateTitle();

        //Clear left file window.
        _OSDMM_ClearRegion(OSDMM_LEFT_FILE_WINDOW_REGION);

        _OSDMM_ResetAudioSpectrum();

        //Draw right file window.
        _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_REGION); //Don't clear the right scroll bar because they are in the same folder.
        __wOSDMMStartDrawItem = 0;
        _OSDMM_PL_DrawRightFileWindow();
        _OSDMM_UpdateRightScrollBar();

        _OSDMM_UpdateHelp(OSDMM_HELP_TYPE_PROGRAM_LIST);
        break;
    case OSDMM_CHANGE_MODE_PROGRAMLIST_TO_PROGRAM:
        _OSDMM_UpdateTitle();

        //Clear left file window.
        _OSDMM_ClearRegion(OSDMM_LEFT_FILE_WINDOW_CONTENT_REGION);
        _OSDMM_DrawLeftFileWindow();

        //Clear the preview window.
        _OSDMM_ClearRegion(OSDMM_PREVIEW_REGION);

        _OSDMM_ResetAudioSpectrum();

        //Draw right file window.
        _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_CONTENT_REGION); //Don't clear the right scroll bar because they are in the same folder.
        __wOSDMMStartDrawItem = (__MMSelectCursor.wIndex / OSDMM_MAX_ITEM_IN_FILE_WINDOW)*OSDMM_MAX_ITEM_IN_FILE_WINDOW;
        _OSDMM_DrawRightFileWindow();
        
        _OSDMM_UpdateHelp(OSDMM_HELP_TYPE_PROGRAM);
        break;
    case OSDMM_CHANGE_MODE_PROGRAM_TO_NORMAL:
        _OSDMM_ChangProramModeToNormal();
        break;
    default:
        return;
    }    
    
    _OSDMM_DrawAllButtons();
}


//*********************************************************************************************************
//  Function    :   _OSDMM_UpdatePlayingTitle
//  Abstract    :   Call this function to update the title string.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   This function will draw the title off-screen and then copy to the exact plosition.
//*********************************************************************************************************
void _OSDMM_UpdatePlayingTitle(void)
{
    if (__bMMMenuID == MM_MENU_NORMAL)
    {
        _bOSDMMPlayingTitle = TRUE;
        _bOSDMMTitleType = OSDMM_TITLE_TYPE_PLAYING;
        
        _OSDMM_ClearRegion(OSDMM_TITLE_REGION);
        
        _OSDMM_InitializeStringPalette(OSDMM_TITLE_REGION);
        _OSDMMStringAttr.wX = OSDMM_TITLE_START_H;    
        _OSDMMStringAttr.wY = OSDMM_TITLE_START_V;
        
        OSDUTL_PrepareOutputString();
        OSDUTL_CopyStringFromIndex(aMMPlayingFolderTitle);
        //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
        _OSDMM_DrawString();
        
        _OSDMMStringAttr.wX = OSDMM_TITLE_START_H + GDI_GetStringWidth_909(__pOSDUTLOutputString);
        
        _OSDMM_CopyDirString((BYTE)__MMPlayingItem.wDirID, OSDMM_TITLE_PLAYING_DIR_WIDTH);
        //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
#ifdef SUPPORT_UNICODE_CODING
        if ((__pMMDirNameList[__MMPlayingItem.wDirID].bMMLinkStatus & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
        {
            _OSDMM_DrawStringByUnicode();                
        }
        else
#endif
        {
        _OSDMM_DrawString();
        } 
        
        _OSDMMStringAttr.wX += GDI_GetStringWidth_909(__pOSDUTLOutputString);
        _OSDMMStringAttr.wX += OSDMM_TITLE_PLAYING_DIR_FILE_DISTANCE;
        
        OSDUTL_PrepareOutputString();
        OSDUTL_CopyStringFromIndex(aMMPlayingFileTitle);
        OSDUTL_OutputNumFromIndex((WORD)(__MMPlayingItem.wIndex - __pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir + 1));
        OSDUTL_CopyStringFromIndex(aMMSlash);
        OSDUTL_OutputNumFromIndex(__pMMFilterFilesOfDir[__MMPlayingItem.wDirID]);
        //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
        _OSDMM_DrawString();
    }
}


//*********************************************************************************************************
//  Function    :   _OSDMM_DLG_DrawButton
//  Abstract    :   This function will draw the button for the dialog box.
//  Arguments   :   bHighlight: the button is enabled or selected.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DLG_DrawButton(BYTE bHighlight)
{
    if (bHighlight)
    {
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND;
    }
    else
    {
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND;
    }

    if (__bMMPopMenuID == MM_POP_MENU_FILTER)
    {
        _OSDMMRect.rect.wLeft = OSDMM_DLG_FILE_TYPE_BUTTON_START_H;
        _OSDMMRect.rect.wTop = OSDMM_DLG_FILE_TYPE_BUTTON_START_V;
        _OSDMMRect.rect.wRight = OSDMM_DLG_FILE_TYPE_BUTTON_START_H+OSDMM_DLG_BUTTON_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_DLG_FILE_TYPE_BUTTON_START_V+OSDMM_DLG_BUTTON_HEIGHT;
    }
    else if (__bMMPopMenuID == MM_POP_MENU_SORT)
    {
        _OSDMMRect.rect.wLeft = OSDMM_DLG_SORT_FILE_BUTTON_START_H;
        _OSDMMRect.rect.wTop = OSDMM_DLG_SORT_FILE_BUTTON_START_V;
        _OSDMMRect.rect.wRight = OSDMM_DLG_SORT_FILE_BUTTON_START_H+OSDMM_DLG_BUTTON_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_DLG_SORT_FILE_BUTTON_START_V+OSDMM_DLG_BUTTON_HEIGHT;
    }
    else
        return;
        
    GDI_FillRoundRect(0, OSDMM_BUTTON_ARC_VALUE, &_OSDMMRect);

    //Fill the frame of round rectangle
    _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_BUTTON_FRAME;

    _OSDMMRect.rect.wLeft -= OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wTop -= OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wRight += OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wBottom += OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, OSDMM_BUTTON_OUTER_ARC_VALUE, OSDMM_BUTTON_ARC_VALUE, &_OSDMMRect);

    _OSDMM_InitializeStringPalette(OSDMM_BUTTON_REGION);

    if (bHighlight)
    {
        _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BUTTON_STRING;
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND;
    }

    __pOSDUTLOutputString = aMMBtnExit;    

    _OSDMMStringAttr.wX = _OSDMMRect.rect.wLeft+OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL+((OSDMM_DLG_BUTTON_WIDTH-GDI_GetStringWidth_909(__pOSDUTLOutputString)) >> 1);
    _OSDMMStringAttr.wY = _OSDMMRect.rect.wTop+OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL+((OSDMM_DLG_BUTTON_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);

    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DLG_DrawDlgBackground
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DLG_DrawDlgBackground(BYTE bDlg)
{
    if (bDlg == OSDMM_DLG_FILE_TYPE)
    {
        _OSDMMRect.rect.wLeft = OSDMM_DLG_FILE_TYPE_START_H;
        _OSDMMRect.rect.wTop = OSDMM_DLG_FILE_TYPE_START_V;
        _OSDMMRect.rect.wRight = OSDMM_DLG_FILE_TYPE_START_H+OSDMM_DLG_FILE_TYPE_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_DLG_FILE_TYPE_START_V+OSDMM_DLG_FILE_TYPE_HEIGHT;

        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_DLG_BACKGROUND;
        GDI_FillRoundRect(0, OSDMM_DIALOG_BOX_ARC_VALUE, &_OSDMMRect);

        //Fill the frame of round rectangle
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_DLG_OUTER_FRAME;
        
        _OSDMMRect.rect.wLeft -= OSDMM_DIALOG_BOX_OUTER_INNER_ARC_PIXEL;
        _OSDMMRect.rect.wTop -= OSDMM_DIALOG_BOX_OUTER_INNER_ARC_PIXEL;
        _OSDMMRect.rect.wRight += OSDMM_DIALOG_BOX_OUTER_INNER_ARC_PIXEL;
        _OSDMMRect.rect.wBottom += OSDMM_DIALOG_BOX_OUTER_INNER_ARC_PIXEL;
        
        GDI_DrawRoundRect(0, OSDMM_DIALOG_BOX_OUTER_ARC_VALUE, OSDMM_DIALOG_BOX_ARC_VALUE, &_OSDMMRect);

        //Draw the title bar
        _OSDMMRect.rect.wLeft = OSDMM_DLG_FILE_TYPE_TITLE_BAR_START_H;
        _OSDMMRect.rect.wTop = OSDMM_DLG_FILE_TYPE_TITLE_BAR_START_V;
        _OSDMMRect.rect.wRight = OSDMM_DLG_FILE_TYPE_TITLE_BAR_START_H+OSDMM_DLG_FILE_TYPE_TITLE_BAR_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_DLG_FILE_TYPE_TITLE_BAR_START_V+OSDMM_DLG_FILE_TYPE_TITLE_BAR_HEIGHT;
    }
    else if (bDlg == OSDMM_DLG_SORT_FILE)
    {
        _OSDMMRect.rect.wLeft = OSDMM_DLG_SORT_FILE_START_H;
        _OSDMMRect.rect.wTop = OSDMM_DLG_SORT_FILE_START_V;
        _OSDMMRect.rect.wRight = OSDMM_DLG_SORT_FILE_START_H+OSDMM_DLG_SORT_FILE_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_DLG_SORT_FILE_START_V+OSDMM_DLG_SORT_FILE_HEIGHT;
        
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_DLG_BACKGROUND;
        
        GDI_FillRoundRect(0, OSDMM_DIALOG_BOX_ARC_VALUE, &_OSDMMRect);
        
        //Fill the frame of round rectangle
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_DLG_OUTER_FRAME;
        
        _OSDMMRect.rect.wLeft -= OSDMM_DIALOG_BOX_OUTER_INNER_ARC_PIXEL;
        _OSDMMRect.rect.wTop -= OSDMM_DIALOG_BOX_OUTER_INNER_ARC_PIXEL;
        _OSDMMRect.rect.wRight += OSDMM_DIALOG_BOX_OUTER_INNER_ARC_PIXEL;
        _OSDMMRect.rect.wBottom += OSDMM_DIALOG_BOX_OUTER_INNER_ARC_PIXEL;
        
        GDI_DrawRoundRect(0, OSDMM_DIALOG_BOX_OUTER_ARC_VALUE, OSDMM_DIALOG_BOX_ARC_VALUE, &_OSDMMRect);

        //Draw the title bar
        _OSDMMRect.rect.wLeft = OSDMM_DLG_SORT_FILE_TITLE_BAR_START_H;
        _OSDMMRect.rect.wTop = OSDMM_DLG_SORT_FILE_TITLE_BAR_START_V;
        _OSDMMRect.rect.wRight = OSDMM_DLG_SORT_FILE_TITLE_BAR_START_H+OSDMM_DLG_SORT_FILE_TITLE_BAR_WIDTH;
        _OSDMMRect.rect.wBottom = OSDMM_DLG_SORT_FILE_TITLE_BAR_START_V+OSDMM_DLG_SORT_FILE_TITLE_BAR_HEIGHT;
    }
    
    _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_DLG_TITLE_BAR;
    GDI_FillRoundRectTop(0, OSDMM_DIALOG_BOX_ARC_VALUE, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DLG_DrawItemRegionInFileType
//  Abstract    :   
//  Arguments   :  
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DLG_DrawItemRegionInFileType(BYTE bDraw, BYTE bIndex)
{
    _OSDMMRect.rect.wLeft = OSDMM_DLG_FILE_TYPE_ITEM_START_H+(bIndex/OSDMM_FILTER_DLG_ROW_NUM)*(OSDMM_DLG_FILE_TYPE_ITEM_DISTANCE_H+OSDMM_DLG_FILE_TYPE_ITEM_WIDTH)-OSDMM_HIGHLIGHT_DISTANCE_H;
    _OSDMMRect.rect.wTop = OSDMM_DLG_FILE_TYPE_ITEM_START_V+(bIndex%OSDMM_FILTER_DLG_ROW_NUM)*(OSDMM_DLG_ITEM_DISTANCE_V+OSDMM_DLG_ITEM_HEIGHT)-OSDMM_HIGHLIGHT_DISTANCE_V;
    _OSDMMRect.rect.wRight = _OSDMMRect.rect.wLeft+OSDMM_DLG_FILE_TYPE_ITEM_WIDTH+OSDMM_HIGHLIGHT_DISTANCE_H*2;
    _OSDMMRect.rect.wBottom = _OSDMMRect.rect.wTop+OSDMM_DLG_ITEM_HEIGHT+OSDMM_HIGHLIGHT_DISTANCE_V*2;
    
    if (bDraw == OSDMM_UPDATE_DRAW)
    {
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
    }
    else
    {
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_DLG_BACKGROUND;
    }
    
    GDI_FillRect_909(0, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DLG_DrawItemRegionInSortFile
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DLG_DrawItemRegionInSortFile(BYTE bDraw, BYTE bIndex)
{    
    //index 0~2==>Method, index 3~4==>Sequence
    if (bIndex < 3)
    {
        _OSDMMRect.rect.wLeft = OSDMM_DLG_SORT_FILE_METHOD_TYPE_ITEM_START_H;
        _OSDMMRect.rect.wTop = OSDMM_DLG_SORT_FILE_METHOD_TYPE_ITEM_START_V+bIndex*OSDMM_DLG_ITEM_HEIGHT+bIndex*OSDMM_DLG_ITEM_DISTANCE_V;
    }
    else
    {
        _OSDMMRect.rect.wLeft = OSDMM_DLG_SORT_FILE_SEQUENCE_ASCENDING_ITEM_START_H;
        _OSDMMRect.rect.wTop = OSDMM_DLG_SORT_FILE_SEQUENCE_ASCENDING_ITEM_START_V+(bIndex-3)*OSDMM_DLG_ITEM_HEIGHT+(bIndex-3)*OSDMM_DLG_ITEM_DISTANCE_V;
    }

    _OSDMMRect.rect.wLeft = _OSDMMRect.rect.wLeft-OSDMM_HIGHLIGHT_DISTANCE_H;
    _OSDMMRect.rect.wTop = _OSDMMRect.rect.wTop-OSDMM_HIGHLIGHT_DISTANCE_V;
    _OSDMMRect.rect.wRight = _OSDMMRect.rect.wLeft+OSDMM_DLG_SORT_FILE_ITEM_WIDTH+OSDMM_HIGHLIGHT_DISTANCE_H*2;
    _OSDMMRect.rect.wBottom = _OSDMMRect.rect.wTop+OSDMM_DLG_ITEM_HEIGHT+OSDMM_HIGHLIGHT_DISTANCE_V*2;

    if (bDraw == OSDMM_UPDATE_DRAW)
    {
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
    }
    else
    {
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_DLG_BACKGROUND;
    }

    GDI_FillRect_909(0, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DLG_DrawAllMethodItemsInSortFile
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DLG_DrawAllMethodItemsInSortFile(BYTE bHighlight)
{    
    //Draw Method items.
    switch (__bMMSortMethod)
    {
    case MM_SORT_BY_TYPE:
        if (bHighlight)
            _OSDMM_DLG_DrawSortMethodItem(TRUE, MM_SORT_BY_TYPE, TRUE);
        else
            _OSDMM_DLG_DrawSortMethodItem(TRUE, MM_SORT_BY_TYPE, FALSE);
        _OSDMM_DLG_DrawSortMethodItem(FALSE, MM_SORT_BY_NAME, FALSE);
        _OSDMM_DLG_DrawSortMethodItem(FALSE, MM_SORT_BY_TIME, FALSE);
        break;
    case MM_SORT_BY_NAME:
        _OSDMM_DLG_DrawSortMethodItem(FALSE, MM_SORT_BY_TYPE, FALSE);
        if (bHighlight)
            _OSDMM_DLG_DrawSortMethodItem(TRUE, MM_SORT_BY_NAME, TRUE);
        else
            _OSDMM_DLG_DrawSortMethodItem(TRUE, MM_SORT_BY_NAME, FALSE);
        _OSDMM_DLG_DrawSortMethodItem(FALSE, MM_SORT_BY_TIME, FALSE);
        break;
    case MM_SORT_BY_TIME:
        _OSDMM_DLG_DrawSortMethodItem(FALSE, MM_SORT_BY_TYPE, FALSE);
        _OSDMM_DLG_DrawSortMethodItem(FALSE, MM_SORT_BY_NAME, FALSE);
        if (bHighlight)
            _OSDMM_DLG_DrawSortMethodItem(TRUE, MM_SORT_BY_TIME, TRUE);
        else
            _OSDMM_DLG_DrawSortMethodItem(TRUE, MM_SORT_BY_TIME, FALSE);
        break;
    default:
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DLG_DrawAllSequenceItemsInSortFile
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DLG_DrawAllSequenceItemsInSortFile(BYTE bHighlight)
{    
    if (__bMMSortSequence == MM_SORT_BY_ASCENDING)
    {
        if (bHighlight)            
            _OSDMM_DLG_DrawSortSequenceItem(TRUE, MM_SORT_BY_ASCENDING, TRUE);
        else
            _OSDMM_DLG_DrawSortSequenceItem(TRUE, MM_SORT_BY_ASCENDING, FALSE);

        _OSDMM_DLG_DrawSortSequenceItem(FALSE, MM_SORT_BY_DESCENDING, FALSE);
    } 
    else if (__bMMSortSequence == MM_SORT_BY_DESCENDING)
    {
        _OSDMM_DLG_DrawSortSequenceItem(FALSE, MM_SORT_BY_ASCENDING, FALSE);
        if (bHighlight)
            _OSDMM_DLG_DrawSortSequenceItem(TRUE, MM_SORT_BY_DESCENDING, TRUE);
        else
            _OSDMM_DLG_DrawSortSequenceItem(TRUE, MM_SORT_BY_DESCENDING, FALSE);
    }
}


//*********************************************************************************************************
//  Function    :   _OSDMM_FlashPlayingFile
//  Abstract    :   Flash the icon of the active playing file.
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_FlashPlayingFile(void)
{
    if (__bMMMenuID == MM_MENU_PROGLIST)
        return;

    //The active playing cursor is not in the current page.
    if ((__MMPlayingItem.wIndex > (__wOSDMMStartDrawItem+OSDMM_MAX_ITEM_IN_FILE_WINDOW-1)) || (__MMPlayingItem.wIndex < __wOSDMMStartDrawItem))
    {
        return;
    }

    if (__MMSelectCursor.wDirID != __MMPlayingItem.wDirID) //The active playing file is not equal to the cursor's position
        return;
  
    //Dump one file for the highlighted item.
    MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMPlayingItem.wDirID, (WORD)(__MMPlayingItem.wIndex - __pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir)), 1);
    //Dump file record.
    MM_GetDumpedFileRecord();
    
    //filter the file that will not playing with Media Manager UI.
    if ((__pMMFileRecord->FileName.bExtID != EXTNAME_MP3) && 
        (__pMMFileRecord->FileName.bExtID != EXTNAME_MP2) &&
        (__pMMFileRecord->FileName.bExtID != EXTNAME_WMA) &&
        (__pMMFileRecord->FileName.bExtID != EXTNAME_AC3))
        return;

    //Initialize the normal palette.
    //if ((__MMSelectCursor.bPosition == POS_FILE_LIST) && (__MMSelectCursor.wDirID == __MMPlayingItem.wDirID) && (__MMSelectCursor.wIndex == __MMPlayingItem.wIndex))
    if (MM_SelectCursorPlayingItemEqual()) //CoCo1.20, reduce code size.
        _OSDMM_SetIconAttribute(_OSDMM_GetIconType(__pMMFileRecord->FileName.bExtID), TRUE);
    else
        _OSDMM_SetIconAttribute(_OSDMM_GetIconType(__pMMFileRecord->FileName.bExtID), FALSE);

    if (_bOSDMMFlash)
    {
        //flash the icon
        //if ((__MMSelectCursor.wDirID == __MMPlayingItem.wDirID) && (__MMSelectCursor.wIndex == __MMPlayingItem.wIndex) && (__MMSelectCursor.bPosition == POS_FILE_LIST))
        if (MM_SelectCursorPlayingItemEqual()) //CoCo1.20, reduce code size.
        {
            _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
            _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
        }
        else
        {
            _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
        }
    }

    //if ((__MMSelectCursor.bPosition == POS_FILE_LIST) && (__MMSelectCursor.wDirID == __MMPlayingItem.wDirID) && (__MMSelectCursor.wIndex == __MMPlayingItem.wIndex))
    if (MM_SelectCursorPlayingItemEqual()) //CoCo1.20, reduce code size.
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;

    _OSDMMStringAttr.wX = OSDMM_ICON_RIGHT_WINDOW_START_H;
    _OSDMMStringAttr.wY = OSDMM_ICON_RIGHT_WINDOW_START_V+((__MMPlayingItem.wIndex - __wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW)*OSDMM_ITEM_HEIGHT;
    GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDMMOutputString);

    _OSDMM_InitializeStringPalette(OSDMM_RIGHT_FILE_WINDOW_REGION);

    if (_bOSDMMFlash)
    {
        //flash the file name
        //if ((__MMSelectCursor.wDirID == __MMPlayingItem.wDirID) && (__MMSelectCursor.wIndex == __MMPlayingItem.wIndex) && (__MMSelectCursor.bPosition == POS_FILE_LIST))
        if (MM_SelectCursorPlayingItemEqual()) //CoCo1.20, reduce code size.
        {
            _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;
        }
        else
        {
            _OSDMMStringAttr.bTextColor = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;
        }
    }
    
    //if ((__MMSelectCursor.bPosition == POS_FILE_LIST) && (__MMSelectCursor.wDirID == __MMPlayingItem.wDirID) && (__MMSelectCursor.wIndex == __MMPlayingItem.wIndex))
    if (MM_SelectCursorPlayingItemEqual()) //CoCo1.20, reduce code size.
        _OSDMMStringAttr.bBackground = OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND;

    //Draw string
    _OSDMMStringAttr.wX = OSDMM_ITEM_RIGHT_WINDOW_START_H;
    _OSDMMStringAttr.wY = OSDMM_ITEM_RIGHT_WINDOW_START_V+((__MMPlayingItem.wIndex - __wOSDMMStartDrawItem)%OSDMM_MAX_ITEM_IN_FILE_WINDOW)*OSDMM_ITEM_HEIGHT;
    _OSDMM_CopyFileString((WORD)(__MMPlayingItem.wIndex - __pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir));
    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
#ifdef SUPPORT_UNICODE_CODING
            if ((__pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
            {
                _OSDMM_DrawStringByUnicode();
            }
            else
#endif
            {
    _OSDMM_DrawString();
            } 

    _bOSDMMFlash = !_bOSDMMFlash;
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DeleteProgramItem
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DeleteProgramItem(void)
{
    _OSDMM_ClearRegion(OSDMM_RIGHT_FILE_WINDOW_CONTENT_REGION);    
    _OSDMM_PL_DrawRightFileWindow();
    _OSDMM_UpdateRightScrollBar();

    _OSDMM_UpdateHighlight(OSDMM_UPDATE_DRAW);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_UpdateSpectrumLowLines
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_UpdateSpectrumLowLines(BYTE bIndex, DWORD dwExtractBits, DWORD dwKeepBits)
{
    /*
    _bOSDMMTemp = (BYTE)((_dwOSDMMEQLow & dwExtractBits) >> (bIndex*4)); //old value
    _bOSDMMTemp1 = (BYTE)((_dwOSDMMNewEQLow & dwExtractBits) >> (bIndex*4)); //new value
    _bOSDMMTemp2 = (BYTE)((_dwOSDMMEQDropLow & dwExtractBits) >> (bIndex*4));
    _OSDMM_DrawSpectrumLine(_bOSDMMTemp, _bOSDMMTemp1, bIndex);
    if (_bOSDMMTemp1 >= _bOSDMMTemp)
    {
        _OSDMM_SetSpectrumDrop(OSDMM_SPECTRUM_LOW, _bOSDMMTemp2, _bOSDMMTemp1, bIndex, (bIndex*4), (_dwOSDMMEQDropLow & dwKeepBits));
    }
    else
    {
        _OSDMM_ChangeSpectrumDrop(OSDMM_SPECTRUM_LOW, _bOSDMMTemp2, bIndex, (bIndex*4), (_dwOSDMMEQDropLow & dwKeepBits));          
    }
    */

    _bOSDMMTemp = (BYTE)((_dwOSDMMEQLow & dwExtractBits) >> (((15-bIndex)%8)*4)); //old value
    _bOSDMMTemp1 = (BYTE)((_dwOSDMMNewEQLow & dwExtractBits) >> (((15-bIndex)%8)*4)); //new value
    _bOSDMMTemp2 = (BYTE)((_dwOSDMMEQDropLow & dwExtractBits) >> (((15-bIndex)%8)*4));
    _OSDMM_DrawSpectrumLine(_bOSDMMTemp, _bOSDMMTemp1, bIndex);
    if (_bOSDMMTemp1 >= _bOSDMMTemp)
    {
        _OSDMM_SetSpectrumDrop(OSDMM_SPECTRUM_LOW, _bOSDMMTemp2, _bOSDMMTemp1, bIndex, (((15-bIndex)%8)*4), (_dwOSDMMEQDropLow & dwKeepBits));
    }
    else
    {
        _OSDMM_ChangeSpectrumDrop(OSDMM_SPECTRUM_LOW, _bOSDMMTemp2, bIndex, (((15-bIndex)%8)*4), (_dwOSDMMEQDropLow & dwKeepBits));          
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_UpdateSpectrumHighLines
//  Abstract    :   
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_UpdateSpectrumHighLines(BYTE bIndex, DWORD dwExtractBits, DWORD dwKeepBits)
{
    /*
    _bOSDMMTemp = (BYTE)((_dwOSDMMEQHigh & dwExtractBits) >> ((bIndex%8)*4)); //old value
    _bOSDMMTemp1 = (BYTE)((_dwOSDMMNewEQHigh & dwExtractBits) >> ((bIndex%8)*4)); //new value
    _bOSDMMTemp2 = (BYTE)((_dwOSDMMEQDropHigh & dwExtractBits) >> ((bIndex%8)*4));
    _OSDMM_DrawSpectrumLine(_bOSDMMTemp, _bOSDMMTemp1, bIndex);
    if (_bOSDMMTemp1 >= _bOSDMMTemp)
    {
        _OSDMM_SetSpectrumDrop(OSDMM_SPECTRUM_HIGH, _bOSDMMTemp2, _bOSDMMTemp1, bIndex, ((bIndex%8)*4), (_dwOSDMMEQDropHigh & dwKeepBits));
    }
    else
    {
        _OSDMM_ChangeSpectrumDrop(OSDMM_SPECTRUM_HIGH, _bOSDMMTemp2, bIndex, ((bIndex%8)*4), (_dwOSDMMEQDropHigh & dwKeepBits));
    }
    */

    _bOSDMMTemp = (BYTE)((_dwOSDMMEQHigh & dwExtractBits) >> (((7-bIndex)%8)*4)); //old value
    _bOSDMMTemp1 = (BYTE)((_dwOSDMMNewEQHigh & dwExtractBits) >> (((7-bIndex)%8)*4)); //new value
    _bOSDMMTemp2 = (BYTE)((_dwOSDMMEQDropHigh & dwExtractBits) >> (((7-bIndex)%8)*4));
    _OSDMM_DrawSpectrumLine(_bOSDMMTemp, _bOSDMMTemp1, bIndex);
    if (_bOSDMMTemp1 >= _bOSDMMTemp)
    {
        _OSDMM_SetSpectrumDrop(OSDMM_SPECTRUM_HIGH, _bOSDMMTemp2, _bOSDMMTemp1, bIndex, (((7-bIndex)%8)*4), (_dwOSDMMEQDropHigh & dwKeepBits));
    }
    else
    {
        _OSDMM_ChangeSpectrumDrop(OSDMM_SPECTRUM_HIGH, _bOSDMMTemp2, bIndex, (((7-bIndex)%8)*4), (_dwOSDMMEQDropHigh & dwKeepBits));
    }
}


//*********************************************************************************************************
//  Function    :   _OSDMM_ShowAudioSpectrum
//  Abstract    :   This function will show audio spectrum line.
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ShowAudioSpectrum(void)
{
    HAL_ReadAM(HAL_AM_EQ_HIGH, &_dwOSDMMNewEQHigh);

    //Update Line0
    _OSDMM_UpdateSpectrumHighLines(0, 0xF0000000, 0x0FFFFFFF);

    //Update Line1
    _OSDMM_UpdateSpectrumHighLines(1, 0x0F000000, 0xF0FFFFFF);

    //Update Line2
    _OSDMM_UpdateSpectrumHighLines(2, 0x00F00000, 0xFF0FFFFF);

    //Update Line3
    _OSDMM_UpdateSpectrumHighLines(3, 0x000F0000, 0xFFF0FFFF);

    //Update Line4
    _OSDMM_UpdateSpectrumHighLines(4, 0x0000F000, 0xFFFF0FFF);

    //Update Line5
    _OSDMM_UpdateSpectrumHighLines(5, 0x00000F00, 0xFFFFF0FF);

    //Update Line6
    _OSDMM_UpdateSpectrumHighLines(6, 0x000000F0, 0xFFFFFF0F);

    //Update Line7
    _OSDMM_UpdateSpectrumHighLines(7, 0x0000000F, 0xFFFFFFF0);

    _dwOSDMMEQHigh = _dwOSDMMNewEQHigh; //Backup the value.

    HAL_ReadAM(HAL_AM_EQ_LOW, &_dwOSDMMNewEQLow);

    //Update Line8
    _OSDMM_UpdateSpectrumLowLines(8, 0xF0000000, 0x0FFFFFFF);

    //Update Line9
    _OSDMM_UpdateSpectrumLowLines(9, 0x0F000000, 0xF0FFFFFF);

    //Update Line10
    _OSDMM_UpdateSpectrumLowLines(10, 0x00F00000, 0xFF0FFFFF);

    //Update Line11
    _OSDMM_UpdateSpectrumLowLines(11, 0x000F0000, 0xFFF0FFFF);

    //Update Line12
    _OSDMM_UpdateSpectrumLowLines(12, 0x0000F000, 0xFFFF0FFF);

    //Update Line13
    _OSDMM_UpdateSpectrumLowLines(13, 0x00000F00, 0xFFFFF0FF);

    //Update Line14
    _OSDMM_UpdateSpectrumLowLines(14, 0x000000F0, 0xFFFFFF0F);

    //Update Line15
    _OSDMM_UpdateSpectrumLowLines(15, 0x0000000F, 0xFFFFFFF0);
        
    _dwOSDMMEQLow = _dwOSDMMNewEQLow; //Backup the value.
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawSpectrumLine
//  Abstract    :   This function will draw the spectrum line.
//  Arguments   :   
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawSpectrumLine(BYTE bOldValue, BYTE bNewValue, BYTE bIndex)
{
    BYTE bLineHeight;

    if (bOldValue == bNewValue)
    {        
        return; //Don't update it.
    }
    else if (bOldValue < bNewValue) //Draw the difference
    {
        bLineHeight = bNewValue;

        while (bLineHeight - bOldValue)
        {
            //Draw the top line
            _OSDMM_DrawSpectrumRange(bLineHeight, bIndex, OSDMM_UPDATE_DRAW);
            bLineHeight--;
        }        
    }
    else if (bOldValue > bNewValue) //Clear the difference
    {
        bLineHeight = bOldValue;

        while (bLineHeight - bNewValue)
        {
            //Clear the top line
            _OSDMM_DrawSpectrumRange(bLineHeight, bIndex, OSDMM_UPDATE_CLEAR);
            bLineHeight--;
        }
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawSpectrumRange
//  Abstract    :   This function will draw one range of the spectrum.
//  Arguments   :   bRange: the range num. bIndex: The index of the spectrum line. 
//              :   bUpdate: OSDMM_UPDATE_DRAW/OSDMM_UPDATE_CLEAR.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawSpectrumRange(BYTE bRange, BYTE bIndex, BYTE bUpdate)
{
    BYTE bTemp;
    BYTE bRangeNum = 0;

    if (bUpdate == OSDMM_UPDATE_DRAW)
    {        
        for (bTemp = 0; bTemp < OSDMM_AUDIO_SPECTRUM_LINE_COLOR_NUM; bTemp++)
        {
            bRangeNum += _bOSDMMSpectrum[bTemp].bRangeNum;
            if (bRange <= bRangeNum)
            {
                _OSDMMRect.bColor = _bOSDMMSpectrum[bTemp].bColor;
                break;
            }
        }
    }
    else
    {
        _OSDMMRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    }

    _OSDMMRect.rect.wLeft = OSDMM_AUDIO_SPECTRUM_START_H+bIndex*(OSDMM_AUDIO_SPECTRUM_WIDTH+OSDMM_AUDIO_SPECTRUM_DISTANCE);
    _OSDMMRect.rect.wRight = _OSDMMRect.rect.wLeft+OSDMM_AUDIO_SPECTRUM_WIDTH;
    _OSDMMRect.rect.wTop = OSDMM_AUDIO_SPECTRUM_START_V-OSDMM_AUDIO_SPECTRUM_PER_RANGE_HEIGHT*bRange;
    _OSDMMRect.rect.wBottom = OSDMM_AUDIO_SPECTRUM_START_V-OSDMM_AUDIO_SPECTRUM_PER_RANGE_HEIGHT*(bRange-1)-1;

    GDI_FillRect_909(0, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawSpectrumDrop
//  Abstract    :   This function will draw the difference of the spectrum.
//  Arguments   :   bRange: the range num. bIndex: The index of the spectrum line.
//              :   bUpdate: OSDMM_UPDATE_DRAW/OSDMM_UPDATE_CLEAR.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawSpectrumDrop(BYTE bRange, BYTE bIndex, BYTE bUpdate)
{
    if (bUpdate == OSDMM_UPDATE_DRAW)
        _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_DROP;
    else
        _OSDMMRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;

    _OSDMMRect.rect.wLeft = OSDMM_AUDIO_SPECTRUM_START_H+bIndex*(OSDMM_AUDIO_SPECTRUM_WIDTH+OSDMM_AUDIO_SPECTRUM_DISTANCE);
    _OSDMMRect.rect.wRight = _OSDMMRect.rect.wLeft+OSDMM_AUDIO_SPECTRUM_WIDTH;
    _OSDMMRect.rect.wTop = OSDMM_AUDIO_SPECTRUM_START_V-OSDMM_AUDIO_SPECTRUM_PER_RANGE_HEIGHT*bRange-OSDMM_AUDIO_SPECTRUM_DIFFERENCE_HEIGHT;
    _OSDMMRect.rect.wBottom = _OSDMMRect.rect.wTop+OSDMM_AUDIO_SPECTRUM_DIFFERENCE_HEIGHT-1;

    GDI_FillRect_909(0, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_SetSpectrumDrop
//  Abstract    :   
//  Arguments   :   bType: OSDMM_SPECTRUM_LOW/OSDMM_SPECTRUM_HIGH. bValue: The value. 
//              :   bIndex: The index of the spectrum line.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_SetSpectrumDrop(BYTE bType, BYTE bOldRange, BYTE bNewRange, BYTE bIndex, BYTE bShift, DWORD dwDrop)
{
    if (bNewRange < bOldRange)
        _OSDMM_DrawSpectrumDrop(bOldRange, bIndex, OSDMM_UPDATE_CLEAR);

    //Draw the difference
    _OSDMM_DrawSpectrumDrop(bNewRange, bIndex, OSDMM_UPDATE_DRAW);
    
    //Set the new value to the difference
    if (bType == OSDMM_SPECTRUM_LOW)
    {
        _dwOSDMMEQDropLow = (((DWORD)bNewRange) << bShift) | dwDrop;
    }
    else if (bType == OSDMM_SPECTRUM_HIGH)
    {
        _dwOSDMMEQDropHigh = (((DWORD)bNewRange) << bShift) | dwDrop;
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ChangeSpectrumDrop
//  Abstract    :   This function will change the position of the drop when the new EQ value < last EQ value.
//  Arguments   :   bType: OSDMM_SPECTRUM_LOW/OSDMM_SPECTRUM_HIGH. bValue: The value. 
//              :   bIndex: The index of the spectrum line.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ChangeSpectrumDrop(BYTE bType, BYTE bRange, BYTE bIndex, BYTE bShift, DWORD dwDrop)
{
    if (bRange != 0)
    {
        //Clear the original difference
        _OSDMM_DrawSpectrumDrop(bRange, bIndex, OSDMM_UPDATE_CLEAR);
        _OSDMM_DrawSpectrumDrop(bRange-1, bIndex, OSDMM_UPDATE_DRAW);

        //Set the new value to the difference
        if (bType == OSDMM_SPECTRUM_LOW)
        {
            _dwOSDMMEQDropLow = (((DWORD)(bRange-1)) << bShift) | dwDrop;
        }
        else if (bType == OSDMM_SPECTRUM_HIGH)
        {
            _dwOSDMMEQDropHigh = (((DWORD)(bRange-1)) << bShift) | dwDrop;
        }
    }
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ResetAudioSpectrum
//  Abstract    :   
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ResetAudioSpectrum(void)
{
    //Initialize the value of equalizer
    _dwOSDMMEQLow = 0;
    _dwOSDMMEQHigh = 0;
    _dwOSDMMEQDropLow = 0;
    _dwOSDMMEQDropHigh = 0;

    //Clear the preview window.
    _OSDMM_ClearRegion(OSDMM_PREVIEW_REGION);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawBackground
//  Abstract    :   This function will draw the background.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawBackground(void)
{
    _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND;

    _OSDMMRect.rect.wLeft = OSDMM_BACKGROUND_H;
    _OSDMMRect.rect.wTop = OSDMM_BACKGROUND_V;
    _OSDMMRect.rect.wRight = OSDMM_BACKGROUND_RIGHT;
    _OSDMMRect.rect.wBottom = OSDMM_BACKGROUND_BOTTOM;
    
    GDI_FillRoundRect(0, OSDMM_BACKGROUND_ARC_BALUE, &_OSDMMRect);

    //Fill the frame of round rectangle
    _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_BACKGROUND_FRAME;

    _OSDMMRect.rect.wLeft = OSDMM_BACKGROUND_H-OSDMM_BACKGROUND_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wTop = OSDMM_BACKGROUND_V-OSDMM_BACKGROUND_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wRight = OSDMM_BACKGROUND_RIGHT+OSDMM_BACKGROUND_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wBottom = OSDMM_BACKGROUND_BOTTOM+OSDMM_BACKGROUND_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, OSDMM_BACKGROUND_OUTER_ARC_BALUE, OSDMM_BACKGROUND_ARC_BALUE, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_DrawInfoRegion
//  Abstract    :   This function will draw the info region.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_DrawInfoRegion(void)
{
    _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_HELP_REGION_BACKGROUND;

    _OSDMMRect.rect.wLeft = OSDMM_HELP_START_H;
    _OSDMMRect.rect.wTop = OSDMM_HELP_START_V;
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
    if (_bOSDMMUIMode == OSDMM_INITIAL_MODE_COPY)
        _OSDMMRect.rect.wRight = OSDMM_HELP_START_H+OSDMM_COPY_HELP_WIDTH;
    else
#endif //SUPPORT_COPY_FILE_IN_MM_UI
    _OSDMMRect.rect.wRight = OSDMM_HELP_START_H+OSDMM_HELP_WIDTH;
    _OSDMMRect.rect.wBottom = OSDMM_HELP_START_V+OSDMM_HELP_HEIGHT;
    
    GDI_FillRoundRect(0, OSDMM_INFO_REGION_ARC_VALUE, &_OSDMMRect);

    //Fill the frame of round rectangle
    _OSDMMRect.bColor = OSDMM_PALETTE_ENTRY_INFO_REGION_FRAME;

    _OSDMMRect.rect.wLeft = OSDMM_HELP_START_H-OSDMM_INFO_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wTop = OSDMM_HELP_START_V-OSDMM_INFO_REGION_OUTER_INNER_ARC_PIXEL;
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
    if (_bOSDMMUIMode == OSDMM_INITIAL_MODE_COPY)
        _OSDMMRect.rect.wRight = OSDMM_HELP_START_H+OSDMM_COPY_HELP_WIDTH+OSDMM_INFO_REGION_OUTER_INNER_ARC_PIXEL;
    else
#endif //SUPPORT_COPY_FILE_IN_MM_UI
    _OSDMMRect.rect.wRight = OSDMM_HELP_START_H+OSDMM_HELP_WIDTH+OSDMM_INFO_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDMMRect.rect.wBottom = OSDMM_HELP_START_V+OSDMM_HELP_HEIGHT+OSDMM_INFO_REGION_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, OSDMM_INFO_REGION_OUTER_ARC_VALUE, OSDMM_INFO_REGION_ARC_VALUE, &_OSDMMRect);
}

//*********************************************************************************************************
//  Function    :   _OSDMM_StopAudioPlayingFile
//  Abstract    :   
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_StopAudioPlayingFile(void)
{
    _bOSDMMTitleType = OSDMM_TITLE_TYPE_NONE; //reset the title type for re-drawing it.
    _bOSDMMPlayingTitle = FALSE;
    _OSDMM_UpdateTitle();
    
    //clear the time if the last audio file is playting to the end.
    _OSDMM_ClearRegion(OSDMM_TIME_REGION);
    
    //Keep the "stop" state. Don't let OSDMM_Trigger to flash the playing icon, update time and draw the audio bar.
    _bOSDMMShowTime = FALSE;
    //Recover the flash file to normal state.
    _bOSDMMFlash = FALSE;
    _OSDMM_FlashPlayingFile();
    
    //clear the audio spectrum
    _OSDMM_ResetAudioSpectrum();

	//CoCo1.06, fix the problem about audio information can't be recovered when cursor is at the folder in stop mode.
	//clear help region
	_OSDMM_DrawInfoRegion();
	_bOSDMMHelpType = OSDMM_HELP_TYPE_NONE;
}

//*********************************************************************************************************
//  Function    :   _OSDMM_ShowAudioInfo
//  Abstract    :   This function will show audio relative info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
WORD _OSDMM_ShowAudioInfo(WORD wStartH)
{
    _OSDMM_InitializeStringPalette(OSDMM_HELP_REGION);

    //HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &_dwOSDMMTemp);

    OSDUTL_PrepareOutputString();

    // LLY2.53, modify the case value from AUDIO_FREQ_XXX to VALUE_XXX
    // Since, the value is same as __dwHALAudioFreq
    switch(_dwOSDMMAudioFreq)
    {      
    //case AUDIO_FREQ_8K:
    case VALUE_8K:
        OSDUTL_OutputNumFromIndex(8);
        break;

    //case AUDIO_FREQ_16K:
    case VALUE_16K:
        OSDUTL_OutputNumFromIndex(16);
        break;

    //case AUDIO_FREQ_32K:
    case VALUE_32K:
        OSDUTL_OutputNumFromIndex(32);
        break;

    //case AUDIO_FREQ_11K:
    case VALUE_11K:
        OSDUTL_OutputNumFromIndex(11);
        OSDUTL_CopyStringFromIndex(aMMMDot);
        OSDUTL_OutputNumFromIndex(0);
        OSDUTL_OutputNumFromIndex(25);
        break;

    //case AUDIO_FREQ_22K:
    case VALUE_22K:
        OSDUTL_OutputNumFromIndex(22);
        OSDUTL_CopyStringFromIndex(aMMMDot);
        OSDUTL_OutputNumFromIndex(0);
        OSDUTL_OutputNumFromIndex(5);
        break;

    //case AUDIO_FREQ_44K:
    case VALUE_44K:
        OSDUTL_OutputNumFromIndex(44);
        OSDUTL_CopyStringFromIndex(aMMMDot);
        OSDUTL_OutputNumFromIndex(1);
        break;

    //case AUDIO_FREQ_88K:
    case VALUE_88K:
        OSDUTL_OutputNumFromIndex(88);
        OSDUTL_CopyStringFromIndex(aMMMDot);
        OSDUTL_OutputNumFromIndex(2);
        break;

    //case AUDIO_FREQ_176K:
    case VALUE_176K:
        OSDUTL_OutputNumFromIndex(176);
        OSDUTL_CopyStringFromIndex(aMMMDot);
        OSDUTL_OutputNumFromIndex(4);
        break;

    //case AUDIO_FREQ_12K:
    case VALUE_12K:
        OSDUTL_OutputNumFromIndex(12);
        break;

    //case AUDIO_FREQ_24K:
    case VALUE_24K:
        OSDUTL_OutputNumFromIndex(24);
        break;

    //case AUDIO_FREQ_48K:
    case VALUE_48K:
        OSDUTL_OutputNumFromIndex(48);
        break;

    //case AUDIO_FREQ_96K:
    case VALUE_96K:
        OSDUTL_OutputNumFromIndex(96);
        break;  

    //case AUDIO_FREQ_192K:
    case VALUE_192K:
        OSDUTL_OutputNumFromIndex(192);
        break;  

    default:
        __wOSDUTLStringBuf[0] = 0;
        return wStartH;
    }

    OSDUTL_CopyStringFromIndex(aMMMAudioSP);
    _OSDMMStringAttr.wX = wStartH;
    _OSDMMStringAttr.wY = OSDMM_HELP_START_V+((OSDMM_HELP_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);    
    //GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
    _OSDMM_DrawString();

    _wOSDMMTemp = wStartH+GDI_GetStringWidth_909(__pOSDUTLOutputString)+OSDMM_HELP_AUDIO_DISTANCE;

    //if (__wHALAudioBitrate)
    if (_dwOSDMMAudioBitRate)
    {
        OSDUTL_PrepareOutputString();
        OSDUTL_OutputNumFromIndex(_dwOSDMMAudioBitRate);
        OSDUTL_CopyStringFromIndex(aMMMAudioBitRate);
    }
    else
    {
        __wOSDUTLStringBuf[0] = 0;
    }

    return _wOSDMMTemp;
}


//*********************************************************************************************************
//  Function    :   _OSDMM_ShowAudioTotalTime
//  Abstract    :   This function will show audio total time.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDMM_ShowAudioTotalTime(void)
{
    _OSDMM_InitializeStringPalette(OSDMM_HELP_REGION);

    //HAL_ReadInfo(HAL_INFO_AUDIOFREQ, &_dwOSDMMTemp);

    OSDUTL_PrepareOutputString();

    //if (__wHALAudioBitrate)
    if (_dwOSDMMAudioBitRate)
    {
        // time = total sector * 2048 (byte/sector) * 8(bit/byte) / (bitrate*1000)
        // since DSP report bitrate unit is x KBits/ sec
        //_dwOSDMMTemp = ((__dwTimeEnd-__dwTimeBegin+1)*2048*8)/(__wHALAudioBitrate*1000); //kevin0.90, K-1000, fix total time display is smaller  
        _dwOSDMMTemp = ((__dwTimeEnd-__dwTimeBegin+1)*2048*8)/(_dwOSDMMAudioBitRate*1000); //kevin0.90, K-1000, fix total time display is smaller         

        //kevin1.05, fix WMA total time is larger than real total time
        if(__bAttrPlay==ATTR_WMA)
        {
            //HAL_ReadAM(HAL_AM_TOTAL_TIME, &_dwOSDMMTemp);  
            _dwOSDMMTemp = _dwOSDMMAudioTotalTime;
        } 
    }
    else
    {
        _dwOSDMMTemp=180; // assume 3 min
    }

    //show "Minute"
    _OSDUTL_ShowTimeNum((WORD)_dwOSDMMTemp/60);

    OSDUTL_CopyStringFromIndex(aMMMouhow);

    //show "second"
    _OSDUTL_ShowTimeNum((WORD)_dwOSDMMTemp%60);
}

#ifdef SUPPORT_AUDIO_EFFECT
void OSDMM_ShowAudioEQ(BYTE bEQType)
{
    _OSDMM_DrawInfoRegion();
    _OSDMM_InitializeStringPalette(OSDMM_HELP_REGION);
    OSDUTL_PrepareOutputString();
    switch (bEQType)
    {
    case 0:
        OSDUTL_CopyStringFromIndex(aAudioNormal);
        break;
    case 1:
        OSDUTL_CopyStringFromIndex(aAudioBassBooster);
        break;
    case 2:
        OSDUTL_CopyStringFromIndex(aAudioFullTreble);
        break;
    case 3:
        OSDUTL_CopyStringFromIndex(aAudioJazz);
        break;
    case 4:
        OSDUTL_CopyStringFromIndex(aAudioRock);
        break;
    case 5:
        OSDUTL_CopyStringFromIndex(aAudioClassical);
        break;
    case 6:
    default:
        OSDUTL_CopyStringFromIndex(aAudioPop);
        break;
    }
		
    _OSDMMStringAttr.wX = OSDMM_HELP_START_H + 10;
    _OSDMMStringAttr.wY = OSDMM_HELP_START_V+((OSDMM_HELP_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);    
    GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
}
#endif

//*********************************************************************************************************
//  Function    :   OSDMM_ResetControl
//  Abstract    :   This function will reset the UI's state.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void OSDMM_ResetControl(void)
{
    //Reset the button status.
    for (_bOSDMMTemp = 0; _bOSDMMTemp < 3; _bOSDMMTemp++)
    {
        _bOSDMMButtonState[_bOSDMMTemp][0] = OSDMM_BUTTON_ENABLE;
        _bOSDMMButtonState[_bOSDMMTemp][1] = OSDMM_BUTTON_ENABLE;
    }
}

void _OSDMM_DrawString(void)
{
    GDI_DrawString_909(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
}

#ifdef SUPPORT_UNICODE_CODING
void _OSDMM_DrawStringByUnicode(void)
{
    GDI_DrawStringByUnicode(0, &_OSDMMStringAttr, __pOSDUTLOutputString);
}
#endif

//*********************************************************************************************************
//  Function    :   OSDMM_ConfigOSDRegion
//  Abstract    :   This function will config the OSD region. And it is exported for OSD kernel usage.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void OSDMM_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;

    RegionInfo.wWidth = 616; //GDI_REGION_WIDTH;
    RegionInfo.wHeight = OSDMM_OSD_REGION_HEIGHT;
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;
    RegionInfo.dwTAddr = DS_OSDFRAME_ST_MM;

    OSD_SetRegion(0, FALSE, &RegionInfo);

    OSDMM_TuneRegionForPALMode();
}

void OSDMM_TuneRegionForPALMode(void)
{
    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL)
    {
        GDI_MoveRegion(0, REGION_X_PAL, REGION_Y_PAL+OSDMM_TUNE_PAL_REGION_PIXELS);
    }
}

#ifdef SUPPORT_COPY_FILE_IN_MM_UI
void _OSDMM_ShowSaveFileDlg(void)
{
	if (_hOSDMMSaveFileDlg == DIALOG_HANDLE_INVALID)
	{
		_hOSDMMSaveFileDlg = DIALOG_CreateDialog(&_OSDMMSaveFileDlg, __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
	}

	if (_hOSDMMSaveFileDlg != DIALOG_HANDLE_INVALID)
	{
		DIALOG_InitializeGDI(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);

		DIALOG_ShowDialog(_hOSDMMSaveFileDlg, OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND, FALSE);                    
		DIALOG_ShowMessage(_hOSDMMSaveFileDlg, (OSDMM_SAVEFILE_DIALOG_WIDTH-GDI_GetStringWidth(aMMSaveFileDlgContent))/2, OSDMM_SAVEFILE_DIALOG_CONTENT_DISTANCE_V, aMMSaveFileDlgContent);
	}

    _bOSDMMSaveFile = TRUE;
}

void _OSDMM_DeleteSaveFileDlg(void)
{
    if (_hOSDMMSaveFileDlg != DIALOG_HANDLE_INVALID)
    {
        DIALOG_DeleteDialog(_hOSDMMSaveFileDlg, TRUE);
        _hOSDMMSaveFileDlg = DIALOG_HANDLE_INVALID;
    }

    _bOSDMMSaveFile = FALSE;
}

void _OSDMM_UpdateSaveFileProgress(void)
{
    PARM_DRAW_STRING StringAttr;
    PARM_VOLUME_BAR OSDMMProgressBar;

    OSDMMProgressBar.bBgColor = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    OSDMMProgressBar.bBarColor = OSDND_PALETTE_ENTRY_PROGRESS_COLOR;
    OSDMMProgressBar.bFrameColor = OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING;
    OSDMMProgressBar.bOriention = ORIENTION_HORIZONTAL;
    OSDMMProgressBar.rect.wLeft = _OSDMMSaveFileDlg.wStartX+OSDMM_SAVEFILE_PROGRESS_POS_H;
    OSDMMProgressBar.rect.wRight = OSDMMProgressBar.rect.wLeft + OSDMM_PROGRESS_BAR_WIDTH;
    OSDMMProgressBar.rect.wTop = _OSDMMSaveFileDlg.wStartY+DIALOG_TITLE_HEIGHT+OSDMM_SAVEFILE_PROGRESS_POS_V;
    OSDMMProgressBar.rect.wBottom = OSDMMProgressBar.rect.wTop + OSDMM_PROGRESS_BAR_HEIGHT;
    OSDMMProgressBar.wVolume = (WORD)__bMMSaveProgress; // percent

    GDI_DrawVolumeBar_909(0, &OSDMMProgressBar);

    OSDUTL_PrepareOutputString();

    if (__bMMSaveProgress < 10)
    {
        OSDUTL_OutputNumFromIndex(0);
        OSDUTL_OutputNumFromIndex(0);
    }
    else if (__bMMSaveProgress < 100)
    {
        OSDUTL_OutputNumFromIndex(0);
    }

    OSDUTL_OutputNumFromIndex(__bMMSaveProgress);
    OSDUTL_CopyStringFromIndex(aOSDMMPercent);

	StringAttr.bBackground = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    StringAttr.bColorKey = 0;
    StringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    StringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    StringAttr.bTextColor = OSDMM_PALETTE_ENTRY_GENERAL_STRING_IN_FILE_WINDOW;
    StringAttr.wX = OSDMMProgressBar.rect.wRight+OSDMM_DISTANCE_FONT_PROGRESS_BAR;
    StringAttr.wY = OSDMMProgressBar.rect.wTop;        
    GDI_DrawString(0, &StringAttr, __pOSDUTLOutputString);   
    
    if (__bMMSaveProgress == 100)
    {
        _OSDMM_DeleteSaveFileDlg();

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
        if (__bMMSlideShowCopyDeleteMode == MM_SLIDE_SHOW_NORMAL_MODE)
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
        {
            //Recover to the original menu.
            _OSDMM_Initial(OSDMM_MODE_RECOVER_UI);
        }
    }
}

#endif //#ifdef SUPPORT_COPY_FILE_IN_MM_UI 

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW

void OSDMM_ShowSaveDlg(void)
{
    OSDMM_SaveDlgConfigOSDRegion();

	if (_hOSDMMSaveDlg == DIALOG_HANDLE_INVALID)
	{        
		_hOSDMMSaveDlg = DIALOG_CreateDialog(&_OSDMMSaveDlg, __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
	}

	if (_hOSDMMSaveDlg != DIALOG_HANDLE_INVALID)
	{
		DIALOG_InitializeGDI(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);

		DIALOG_ShowDialog(_hOSDMMSaveDlg, PAL_ENTRY_COLOR_TRANSPARENT, TRUE);                    
		DIALOG_ShowMessage(_hOSDMMSaveDlg, (OSDMM_SAVE_DIALOG_WIDTH-GDI_GetStringWidth(aMMSaveDlgContent))/2, OSDMM_SAVE_DIALOG_CONTENT_DISTANCE_V, aMMSaveDlgContent);
	}
}

void OSDMM_DeleteSaveDlg(void)
{
    if (_hOSDMMSaveDlg != DIALOG_HANDLE_INVALID)
    {
        DIALOG_DeleteDialog(_hOSDMMSaveDlg, TRUE);
        _hOSDMMSaveDlg = DIALOG_HANDLE_INVALID;
    }
}

BYTE OSDMM_SaveDlgProcessKey(BYTE bKey)
{
    WORD wResult;

    wResult = DIALOG_Processkey(_hOSDMMSaveDlg, bKey);

    if ((wResult != DIALOG_ACTION_NONE) && (wResult != DIALOG_ACTION_DONE))
    {
        if ((HIBYTE(wResult) << 8) == DIALOG_ACTION_BUTTON_PRESSED)
        {
            return (LOBYTE(wResult));
        }
    }

    return OSDMM_SAVE_DIALOG_BUTTON_ACTION_NONE;
}

void OSDMM_ShowDeleteDlg(void)
{
    OSDMM_SaveDlgConfigOSDRegion();

	if (_hOSDMMDeleteDlg == DIALOG_HANDLE_INVALID)
	{
		_hOSDMMDeleteDlg = DIALOG_CreateDialog(&_OSDMMDeleteDlg, __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
	}

	if (_hOSDMMDeleteDlg != DIALOG_HANDLE_INVALID)
	{
		DIALOG_InitializeGDI(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);

		DIALOG_ShowDialog(_hOSDMMDeleteDlg, PAL_ENTRY_COLOR_TRANSPARENT, TRUE);                    
		DIALOG_ShowMessage(_hOSDMMDeleteDlg, (OSDMM_SAVE_DIALOG_WIDTH-GDI_GetStringWidth(aMMDeleteDlgContent))/2, OSDMM_SAVE_DIALOG_CONTENT_DISTANCE_V, aMMDeleteDlgContent);
	}
}

void OSDMM_DeleteDeleteDlg(void)
{
    if (_hOSDMMDeleteDlg != DIALOG_HANDLE_INVALID)
    {
        DIALOG_DeleteDialog(_hOSDMMDeleteDlg, TRUE);
        _hOSDMMDeleteDlg = DIALOG_HANDLE_INVALID;
    }
}

BYTE OSDMM_DeleteDlgProcessKey(BYTE bKey)
{
    WORD wResult;

    wResult = DIALOG_Processkey(_hOSDMMDeleteDlg, bKey);

    if ((wResult != DIALOG_ACTION_NONE) && (wResult != DIALOG_ACTION_DONE))
    {
        if ((HIBYTE(wResult) << 8) == DIALOG_ACTION_BUTTON_PRESSED)
        {
            return (LOBYTE(wResult));
        }
    }

    return OSDMM_SAVE_DIALOG_BUTTON_ACTION_NONE;
}

void OSDMM_SaveDlgConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;

    GDI_ClearRegion(0);

#ifdef  BORROW_FRAME_BUFFER
    //Borrow JPEG display buffer always.
    RegionInfo.dwTAddr = JPEGDEC_QueryEmptyBuffer();    
#else
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
#endif // #ifdef  BORROW_FRAME_BUFFER

    RegionInfo.wWidth = OSDMM_SAVE_DIALOG_REGION_WIDTH;
    RegionInfo.wHeight = OSDMM_SAVE_DIALOG_REGION_HEIGHT;
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;

#ifdef  BORROW_FRAME_BUFFER
    OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    OSD_SetRegion(0, FALSE, &RegionInfo);
#endif
}

void OSDMM_CopyDeleteDlg_Exit(BYTE bExitMode)
{
    OSDMM_DeleteSaveDlg();
    OSDMM_DeleteDeleteDlg();
}

BYTE OSDMM_CopyDeleteDlg_Recover(BYTE bRecoverRegion)
{
	if (_hOSDMMSaveDlg != DIALOG_HANDLE_INVALID)
	{
		DIALOG_InitializeGDI(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);

		DIALOG_ShowDialog(_hOSDMMSaveDlg, PAL_ENTRY_COLOR_TRANSPARENT, TRUE);                    
		DIALOG_ShowMessage(_hOSDMMSaveDlg, (OSDMM_SAVE_DIALOG_WIDTH-GDI_GetStringWidth(aMMSaveDlgContent))/2, OSDMM_SAVE_DIALOG_CONTENT_DISTANCE_V, aMMSaveDlgContent);
	}
	else if (_hOSDMMDeleteDlg != DIALOG_HANDLE_INVALID)
	{
		DIALOG_InitializeGDI(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);

		DIALOG_ShowDialog(_hOSDMMDeleteDlg, PAL_ENTRY_COLOR_TRANSPARENT, TRUE);                    
		DIALOG_ShowMessage(_hOSDMMDeleteDlg, (OSDMM_SAVE_DIALOG_WIDTH-GDI_GetStringWidth(aMMDeleteDlgContent))/2, OSDMM_SAVE_DIALOG_CONTENT_DISTANCE_V, aMMDeleteDlgContent);
	}

	return TRUE;
}

#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW