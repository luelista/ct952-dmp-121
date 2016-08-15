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
#ifdef CT950_STYLE
#include "rtcdrv.h"
#include "autopower.h"
#endif //CT950_STYLE

#ifdef SUPPORT_POWERON_MENU
#include "poweronmenu.h"
#endif //SUPPORT_POWERON_MENU

#include "gdi.h" 
#include "comutl.h"

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
#endif

//////////////////////////////////////////////////////////////
// wyc1.05, add a new variable to record the next going directory, purpose is to moving highlight between directories when issuing KEY_UP/KEY_DOWN.
// Keep the next to go directory ID, index from '0' -- WYC1.05
BYTE    _bMMNextSelectDir;

// wyc1.05, add a new variable to record the type of moving cursor across boundary. One is KEY_NEXT/KEY_PREVher is KEY_UP/KEY_DOWN.
// Record the type of moving cursor across boundary.
BYTE    _bMMAcrossDirType;
//////////////////////////////////////////////////////////////
CURSOR_INFO __MMPlayingItem ;
CURSOR_INFO __MMSelectCursor;
WORD    __wMMNewCursorIndex;
//////////////////////////////////////////////////////////////
BYTE __bMMMenuID; // =MM_MENU_NONE;
BYTE __bMMPopMenuID;
BYTE __bMMInitMenu;
//////////////////////////////////////////////////////////////
DWORD __dwMMKeyToDo;
//////////////////////////////////////////////////////////////
#ifdef SUPPORT_AUDIO_EFFECT
BYTE _bAudioEQ = 0;
#endif //SUPPORT_AUDIO_EFFECT
///////////////////////////////////////////////////////////
extern  BYTE    __bMMSaveProgress;

///////////////////////////////////////////////////////////
extern  BIT     __btInputProgram;
extern DWORD __dwMMJPGBuffEmptyTime;
#ifdef IMAGE_FRAME_SETUP //CoCo2.38
extern IMAGE_FRAME_SETUPINFO __ImageFrameSetupInfo;
#endif //IMAGE_FRAME_SETUP

extern BYTE CC_PWRCtrl(void);
///////////////////////////////////////////////////////////

//  ***************************************************************************
//  Function    :   MM_UpdateMenuInfo
//  Description :   Update the highlight/ selection info for Menu
//  Arguments   :   bKey    : The user key
//  Return      :   KEY_NO_KEY  , the input key is processed
//              :   INVALID_KEY , the input key is not allowed under this mode
//              :   KEY_xxx     , the main program need do more process for the key.
//  Side Effect :
//  Global      :
//  ***************************************************************************
BYTE MM_UpdateMenuInfo(BYTE bKey)
{
    BYTE    bMMRet;

    switch(bKey)
    {
    case KEY_THUMBNAIL:
        if ((__MMSelectCursor.wIndex < __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir)
            || (__pMMFileRecordStart[MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID, (__MMSelectCursor.wIndex-__pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir))].FileName.bExtID != EXTNAME_JPG))
		{
            return INVALID_KEY;
		}
        __bMMMenuID=MM_MENU_NONE;
        OSDMM_Update(OSDMM_MSG_EXIT,0);
        MM_EnterThumbMode();
        return KEY_NO_KEY;

#ifdef SUPPORT_COPY_FILE_IN_MM_UI
    case    KEY_MM_FILECOPY:
         MM_InitialMenu(MM_MENU_ENCODE_FILE, FALSE);
        break;
#endif //
    case    KEY_PROGRAM:
#ifdef SUPPORT_FM_PROGRAM //CoCo1.05, use "SUPPORT_FM_PROGRAM" define to open/close program function.
        if ((!__btInputProgram) && (!__btPlayingProgram))
        {
             LINEAR_Program(LINEAR_PROGRAM_MODE,(BYTE)NULL);

            // Set __bProgramCurrentIndex as __bProgramMaxEnty while enter program mode
            // Otherwise, previous program mode data will be destroyed
            // while program mode playing --> enter program mode again
            // Notice: must put this procedure after LINEAR_PROGRAM_MODE
            // because __bProgramCurrentIndex value will be set to zero wihtin it
            __bProgramCurrentIndex=__bProgramMaxEntry;
            __bRepeat = REPEAT_NONE;

            if (__wDiscType & CDROM_PICTURECD)
            {
                if (__bMMMenuID != MM_MENU_NONE) //CoCo1.26, slide show==>File Manager doesn't need to do this.
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); 

                if (__pMMFilterFilesOfDir[__MMSelectCursor.wDirID] != 0) //If the active dir doesn't have file, don't move the cursor. This will cause the unpredicted error for MM_IndexConvert error.
                {
                    __MMSelectCursor.bPosition=POS_FILE_LIST;
                    // convert cursor index from relative to absolute coordinate
                    __MMSelectCursor.wIndex=MM_IndexConvert(MM_INDEX_CONVERT_REL2ABS_FILE_LIST,
                        __MMSelectCursor.wDirID, 0x0);
                }
            }

            if (__bMMMenuID != MM_MENU_NONE)
            {
                MM_InitialMenu(MM_MENU_EDITPROG, TRUE);
            }
            else //Use OSDMM_MSG_SHOW to recover UI.
            {
                if ((__bAttrPlay & TYPE_CDROM_AV) || (__bPBReady == FALSE))
                {
                    CC_KeyCommand(KEY_STOP);
                     OSD_RemoveUI(OSD_UI_DISPLAY);
                    HAL_PlayCommand(COMMAND_STOP, 0);
                    HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
                }
                // wyc1.10-909, need to clear ZOOM mode after entering MM program edit mode.
#ifdef SUPPORT_GRAPH_ZOOM
                if (__bZoomFactor != VIEW_NORMAL)
                {
                    __bZoomFactor = VIEW_NORMAL;
                    HAL_Zoom (VIEW_NORMAL);
                    OSD_OUTPUT_MACRO ( MSG_ZOOM, (WORD)__bZoomFactor, 0x3 ); //Clear PIP and OSD
                    OSD_OUTPUT_MACRO ( MSG_ZOOM_TEXT, (WORD)__bZoomFactor, 0x3 ); //Clear PIP and OSD
                }
#endif
                __MMSelectCursor.bPosition = POS_FILE_LIST;
                __MMSelectCursor.wIndex = __wPlayItem-1;
                __MMPlayingItem.wIndex = __MMSelectCursor.wIndex;
                __bMMMenuID = MM_MENU_EDITPROG;
                MM_InitialMenu(MM_MENU_EDITPROG, FALSE);
                if (__bAttrPlay == ATTR_JPG)
                {
                    MM_JPEGPreview();
                }
            }
            // wyc0.85a, update the button status correctly.
            // wyc2.35-909S, disable program list menu when playing mode.
            // wyc2.36-909S, when preview mode, don't disable the button.
#ifndef NO_MM_BUTTON
            if ((__btPlaying) && (__dwMMJPEGPreview == FALSE))
            {
                OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_PROG_LIST);
            }
            else
            {
                OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_PROG_LIST);
            }
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_PLAY);
#endif //
            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
        }
        else
        {
            if (__bMMMenuID == MM_MENU_PROGLIST)
                return INVALID_KEY;
            if (__bMMMenuID == MM_MENU_NONE)
            {
                __MMSelectCursor.bPosition = POS_FILE_LIST;
                __MMSelectCursor.wIndex = __wPlayItem-1;
            }
            // wyc0.85a, when select cursor stay in button & playing and then press KEY_PROGRAM to enter normal mode, should move
            // cursot to other part depend on if files in dir. Because of button should be disabled and cursor can't stay in button list.
            if ((__MMSelectCursor.bPosition == POS_BUTTON_LIST) && (__bMMMenuID == MM_MENU_EDITPROG) && (__btPlaying))
            {
                if (__pMMFilterFilesOfDir[__MMSelectCursor.wDirID] == 0)
                {
                    // go to the active directory position
                    _MM_SetCursorToActiveDirPos();
                }
                else
                {
                    __MMSelectCursor.bPosition=POS_FILE_LIST;
                    __MMSelectCursor.wIndex=__MMPlayingItem.wIndex;
                }
            }
            LINEAR_Program(LINEAR_PROGRAM_CLEAR,(BYTE)NULL);

            // Reset max program entry as zero while exit program mode
            __bProgramMaxEntry=0;

            // wyc0.85a, when program playing JPG and press KEY_PROGRAM, just clear program mode and don't update button and select cursor.
            if (__bMMMenuID == MM_MENU_NONE)
                return KEY_NO_KEY;

#ifndef NO_MM_BUTTON
            if (__btPlaying)
            {
                OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_FILTER);
                OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_SORT);
            }
            else
            {
                OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_FILTER);
                OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_SORT);
            }
#endif //
            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
        }
        return KEY_NO_KEY;
#else
        return INVALID_KEY;
#endif // SUPPORT_FM_PROGRAM

    // LLY2.81, re-adjust the KEY_RESUME procedure
    // real IR key or transfering key all go here
    case    KEY_RESUME:
#ifdef  SUPPORT_MP3_RESUME
        // wyc2.05-909S, add this code to avoid doing resume function when file auto play end.
        if((__bCountStop!=0) && (__bLastPlayItem == FALSE)) // non-playing mode
        {
            {
                // always display "MSG_RESUME" (don't care the key source)
                // Because, it will return KEY_PLAY to CC module
                // So, there is no chance to sending MSG_RESUME within CC
                OSD_OUTPUT_MACRO ( MSG_RESUME, __wTemp, 0x3 );

                // clear selection cursor
                OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); //CoCo.CT909, OSDMM_Update needs two parameters.

                // wyc2.00-909S, need to update dir id to resume item, otherwise will play wrong resume item.
                // LLY2.05, need keep desired playing dir/ index for resume (old procedure is not enough)
                // And, need update it for selection cursor
                __MMPlayingItem.wDirID = HIWORD(__dwResumeItem);
                __MMPlayingItem.wIndex = MM_IndexConvert(MM_INDEX_CONVERT_REL2ABS_FILE_LIST,
                        (BYTE)(HIWORD(__dwResumeItem)), LOWORD(__dwResumeItem-1));
                __MMPlayingItem.bPosition = POS_FILE_LIST;
                __MMSelectCursor.wDirID = __MMPlayingItem.wDirID;
                __MMSelectCursor.wIndex = __MMPlayingItem.wIndex;
                __MMSelectCursor.bPosition = __MMPlayingItem.bPosition;

                // LLY2.05, need prepare the file info list and re-draw the menu
                // Since, we don't know where directory and file are for KEY_RESUME
                MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
                OSDMM_Update(OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL, 0); 

                // wyc0.71, need to update highlight again
                OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);

                // LLY0.83b-2, Update the desired playing item & playing directory
                // wyc1.02-909, need to update __wPlayItem to __dwMMResumeItem
                __wPlayItem = LOWORD(__dwResumeItem);

#ifndef NO_MM_BUTTON
                if (__bMMMenuID == MM_MENU_NORMAL)
                {
                    OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_FILTER);
                    OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_SORT);
                }
                else
                {
                    OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_PROG_LIST);
                    OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_PLAY);
                }
#endif //
                // wyc1.06-909, need to set __bMMResumetoMotion to TRUE to go resume function.
                _dwMMTemp4 = MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem);
                if (_dwMMTemp4 == EXTNAME_DAT)
                    __bAttrPlay = ATTR_DAT;
                else if (_dwMMTemp4 == EXTNAME_MPG)
                    __bAttrPlay = ATTR_MPG;
                else if (_dwMMTemp4 == EXTNAME_VOB)
                    __bAttrPlay = ATTR_VOB;
                else if (_dwMMTemp4 == EXTNAME_AVI)
                    __bAttrPlay = ATTR_AVI;
                // LLY2.05, support MP3 Disc/ Time resume feature
                else if(_dwMMTemp4 == EXTNAME_MP3)
                    __bAttrPlay = ATTR_MP3;
                else if(_dwMMTemp4 == EXTNAME_WMA)
                    __bAttrPlay = ATTR_WMA;

                if ((__bAttrPlay & TYPE_CDROM_AV))
                {
                    // make system enter play mode but __bCountStop is 1 and system will abnormal. So set __bCountStop to 0 here.
                    __bCountStop = 0;
                    __bMMMenuID = MM_MENU_NONE;
                    OSDMM_Update(OSDMM_MSG_EXIT,0);
                    // LLY2.31, call parser API to check if index table build done.
                    //if (__bNoDivXIndexTbl || (__dwTemp == PARSER_ESFMT_WMA_V1) || (__dwTemp == PARSER_ESFMT_WMA_V2))
                    if (!PARSER_QueryAVIIndexTable())// || (__dwTemp == PARSER_ESFMT_WMA_V1) || (__dwTemp == PARSER_ESFMT_WMA_V2))
                    {
                        return KEY_PLAY;
                    }
                    else
                    {
                        return bKey;
                    }
                }
                // LLY2.05, support MP3 resume
                // LLY2.35, support WMA resume
                else if((__bAttrPlay == ATTR_MP3) || (__bAttrPlay == ATTR_WMA) )
                {
                    // wyc2.36-909S, fix when resume from JPG preview to MP3, then JPG image don't clear problem.
                    if (__dwMMTriggerMode == MM_IMAGE_TRIGGER_MODE)
                    {
                        HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
                    }
                    // call this to let OSD time and audio spectrum can running.
                    OSDMM_Update(OSDMM_MSG_PLAYING, 0);

                    // Need set __bMMStage as MM_STAGE_PREPAREINFO
                    // Otherwise, the trigger mechanism can't work
                    __bMMStage = MM_STAGE_PREPAREINFO;

                    // LLY2.11, get playing item range, to keep value @ __dwTimeBegin, __dwTimeEnd
                    // Otherwise, the GoTime action maybe wrong while H/W power on --> resume
                    // LLY2.53, modify the API w/ extra parameter to get desired playback range start/ end
                    // Since, we will not update them into __dwTimeBegin/ __dwTimeEnd directly
                    MM_GetRange(__MMPlayingItem.wDirID, __wPlayItem, &__dwTimeBegin, &__dwTimeEnd);

                    __bCountStop = 0;
                    return bKey;
                }
                else
                {
                    // wyc1.20-909, need to clear JPG buffer when pre-stop -> play from MP3 file because it maybe resume from JPG preview mode.
                    if (__dwMMTriggerMode == MM_IMAGE_TRIGGER_MODE)
                    {
                        HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
                    }
                    __dwMMKeyToDo = TRUE;
                    return KEY_PLAY;
                }
            }
        }
        else
#endif  // #ifdef SUPPORT_MP3_RESUME
        {
            // nothing need to do and always return invalid key
            return INVALID_KEY;
        }
        break;

    case    KEY_PAUSE:
        // wyc0.91, don't call MSG_STOP when pause mode, because when call MSG_PLAYING when KEY_PLAY. the MP3 file info will be cleared by call MSG_PLAYING.
        if ((__bModeCmd==KEY_FF) && (__btPause) && (__bScanLevel != 0) && (__bAttrPlay & TYPE_CDROM_AUDIO))
        {
            UTL_TransferCommandID(KEY_PLAY);
            __bKeyMessage=MSG_SCANFORWARD;
            OSD_OUTPUT_MACRO ( __bKeyMessage, __bScanLevel, 0xFF );
            __btPlaying= TRUE;
            __btPause = FALSE;
            return KEY_NO_KEY;
        }
        return bKey;
    case    KEY_PLAY:
        // [1] if selection cursor @ directory list
        if(__MMSelectCursor.bPosition == POS_VOL_DIR_LIST)
        {
            // if cursor @ ROOT postion
            // update selection cursor @ 1st postion of file list
            if(__MMSelectCursor.wIndex==0)
            {
                // wyc2.05-909S, need to modify to check the files in ROOT, not check the active dir. Otherwise will happen can't enter
                // root dir when no files in active dir.
                if(_MM_QueryInfo2(MM_QUERY2_TOTAL_SORT_FILTER_ITEM, __MMSelectCursor.wIndex))
                {
                    // clear selection cursor first
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); 

                    // Update final active directory ID as "0"
                    __MMSelectCursor.wDirID=0x0;
                    _bMMNextSelectDir = __MMSelectCursor.wDirID;

                    // Prepare the file info. for file list
                    MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);

                    // Re-draw the menu
                    OSDMM_Update(OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL, 0); 

                    // Update selection cursor info.
                    __MMSelectCursor.bPosition=POS_FILE_LIST;
                    __MMSelectCursor.wIndex=0x0;

                    // Draw selection cursor on menu
                    OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 

                    if (MM_JPEGPreview())
                        return KEY_NO_KEY;

                    // all thing done, so return no key
                    return KEY_NO_KEY;
                }
                else
                {
                    return KEY_INVALID;
                }
            }
            // only valid while cursor @ UP_DIR position
            // return to up-directory, ie. parent directory !!
            else if(_bMMExistUpDir && __MMSelectCursor.wIndex==1)
            {
                // clear selection cursor first
                OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); 

                // update active directory ID first
                // because MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID) will refer it
                __MMSelectCursor.wDirID=_bMMParentID;
                _bMMNextSelectDir = __MMSelectCursor.wDirID;

                // Prepare the file information
                MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);

                // Re-draw the menu
                OSDMM_Update(OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL, 0); 

                // Update final selection cursor information
                _MM_SetCursorToActiveDirPos();

                // Draw selection cursor on menu
                OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 

                // all thing done, so return no key
                return KEY_NO_KEY;
            }
            // Otherwise, return invalid key
            else
            {
                // When highlight in dir field and press play, make highlight goto file field
                // wyc0.80, should use sort/filter item to replace old one.
                if(_MM_QueryInfo2(MM_QUERY2_TOTAL_SORT_FILTER_ITEM, __MMSelectCursor.wDirID))
                {
                    // clear the selection cursor first
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); 
                    // Update cursor @ 1st file ID
                    __MMSelectCursor.bPosition=POS_FILE_LIST;
                    __MMSelectCursor.wIndex=__wOSDMMStartDrawItem; 
                    MM_JPEGPreview();
                }
                else
                {
                    return INVALID_KEY;
                }
                // Update final cursor on menu
                OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 
                return KEY_NO_KEY;
            }
        }
        // [2] if selection cursor @ file list
        else if(__MMSelectCursor.bPosition == POS_FILE_LIST)
        {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {
                return _MM_SetJPEGAudioPlayItem();                                
            }
            else
#endif //#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            {
                // if cursor @ sub-directory
                // enter the sub-directory, and re-draw Menu
                if(__MMSelectCursor.wIndex < __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir)
                {
                    // Update selection cursor info. to active directory ID
                    __MMSelectCursor.wDirID=(BYTE)(__pMMVolumeDirTree[__MMSelectCursor.wDirID].b1stSubDirID+__MMSelectCursor.wIndex);
                    _bMMNextSelectDir = __MMSelectCursor.wDirID;
                    
                    // Prepare the file info. of the new directory
                    // Cursor index still @ 1st postion of file list
					MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
                    
                    // Re-draw the Menu
                    OSDMM_Update(OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL, 0); 
                    
                    // if exist item within file list window
                    // Update final selection cursor @ 1st position of file list
                    if(_MM_QueryInfo2(MM_QUERY2_TOTAL_SORT_FILTER_ITEM,__MMSelectCursor.wDirID))
                    {
                        __MMSelectCursor.wIndex=0x0;
                    }
                    // else, update the selection cursor @ active directory
                    else
                    {
                        _MM_SetCursorToActiveDirPos();
                    }
                    OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 
                    
                    MM_JPEGPreview();
                    
                    return KEY_NO_KEY;
                }
                else
                {
                    // wyc0.85a, when MP3 pause mode and press KEY_PLAY. Make KEY_PLAY do PLAY to exit pause mode, not to do program edit.
                    // Otherwise no key can do play when MP3 pause mode.
                    // wyc2.51-909P, add code to control the ENCODE FILE menu.
                    if(((__bMMMenuID==MM_MENU_EDITPROG) && (__btPause == FALSE))
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
                        || (__bMMMenuID == MM_MENU_ENCODE_FILE)
#endif //
                        )
                    {
                        // Update selection cursor @ original postion again
                        // Otherwise, selection cursor will disappear !!
                        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 
                        
                        // Transfer cursor index from absolute to relative coordinate
                        _dwMMTemp2=MM_IndexConvert(MM_INDEX_CONVERT_ABS2REL_FILE_LIST,
                            __MMSelectCursor.wDirID, __MMSelectCursor.wIndex);
                        
                        // Increase index by '1', because we will keep real playing item
                        // value within __ProgIdx[].wSubIndex, ie. same as __wPlayItem
                        // In another ways, cursor index from '0'; __wPlayItem index from '1'
                        _dwMMTemp2++;
                        
                        // Finding if the selection item has exist within the program list
                        // Notice: 2nd parameter is un-necessary for this case
                        __bTemp=LINEAR_ProgramEntryOperation(LINEAR_QUERY_INDEX,
                            (BYTE)NULL, __MMSelectCursor.wDirID, _dwMMTemp2);
                        
                        // if yes, delete the element
                        // if no, add the element
                        if(__bTemp != 0xFF) // exist this content in program list
                        {
                            // Delete the content and re-arrange the list
                            // Notice: 3rd & 4th parameter is un-necessary for this case
                            LINEAR_ProgramEntryOperation(LINEAR_DELETE_ENTRY_BY_INDEX_W_REARRANGE, __bTemp, (BYTE)NULL, (WORD)NULL);
                            
                            // Decrease current program index by '1' for next empty entry
                            __bProgramCurrentIndex--;
                            
                            // Decrease max program entry number by '1' while delete one element
                            __bProgramMaxEntry--;
                        }
                        else // can't find content in program list
                        {
                            // Append the content into the list
                            // wyc1.05-2, we should use __bProgramMaxEntry to veirfy if still can input program entry.
                            // Not use __bProgramCurrentIndex variable.
                            _dwMMTemp5=LINEAR_ProgramEntryOperation(LINEAR_ADD_ENTRY_BY_INDEX, __bProgramMaxEntry, __MMSelectCursor.wDirID, _dwMMTemp2);
                            
                            // if return value != 0xFF, means insert action ok
                            // else, return "invalid key"
                            if(_dwMMTemp5!=0xFF)
                            {
                                // Increase current program index by '1' for next empty entry
                                __bProgramCurrentIndex++;
                                
                                // Increase max program entry by '1' while add one element
                                __bProgramMaxEntry++;
                            }
                            else //exceed the maximum program number
                            {
                                return KEY_INVALID;
                            }
                        }

                        // Draw the program relative cursor
                        OSDMM_Update(OSDMM_MSG_UPDATE_PROGRAM_ITEM, 0); 
                    }
                    else
                    {
                        // wyc0.75, need to check if preview mode, if TRUE need to issue stop command and reset preview state.
                        if (__dwMMJPEGPreview == TRUE)
                        {
                            MM_ResetPreviewMode();
                            // wyc1.02-909, when play from pre-stop mode, need to set __dwMMKeyToDo to TRUE to go TODO mode. If not do it,
                            // otherwise _CC_PreProcessKey will transfer KEY_PLAY to KEY_RESUME.
                            if (__bCountStop == 1)
                                __dwMMKeyToDo = TRUE;
                        }
                        // wyc0.75, when key from FW, that mean this key is not processed by CC_DoKeyAction yet and will process this
                        // time. So we can't clear the __dwMMJPEGPreview to let it will do preview this time.
                        if (__bKeySource == KEY_FROM_ISR)
						{
                            __dwMMJPEGPreview = FALSE; 
						}
                        return _MM_SettingAndPlayItem();
                    }
                    // everything is done, so retun no key
                    return KEY_NO_KEY;
                }
            }
        }
        else if(__MMSelectCursor.bPosition == POS_BUTTON_LIST)
        {
            // Do corresponding action base on desired button
            return _MM_ButtonAction();
        }
        else if (__MMSelectCursor.bPosition == POS_PROG_LIST)
        {
            // Delete the content and re-arrange the list
            // Notice: 3rd & 4th parameter is un-necessary for this case
            LINEAR_ProgramEntryOperation(LINEAR_DELETE_ENTRY_BY_INDEX_W_REARRANGE, __MMSelectCursor.wIndex, (BYTE)NULL, (WORD)NULL);

            // Decrease current program index by '1' for next empty entry
            __bProgramCurrentIndex--;

            // Decrease max program entry number by '1' while delete one element
            if (__MMSelectCursor.wIndex == (__bProgramMaxEntry-1))
            {
                if (__MMSelectCursor.wIndex == 0)
                {
                    __MMSelectCursor.bPosition = POS_BUTTON_LIST;
                    __MMSelectCursor.wIndex = MM_BTN_RETURN;
#ifndef NO_MM_BUTTON
                    OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_DELETE_ALL);
#endif //
                }
                else
                    __MMSelectCursor.wIndex--;
            }
            __bProgramMaxEntry--;
            MM_PreparePage(MM_PREPARE_PROGLIST,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
            if (((__MMSelectCursor.wIndex+1) % OSDMM_MAX_ITEM_IN_FILE_WINDOW) == 0)
            {
                OSDMM_Update(OSDMM_MSG_UPDATE_FILE_WINDOW_PREVIOUS, 0);
                OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
            }
            else
                OSDMM_Update(OSDMM_MSG_PROGRAM_DELETE, 0);
            MM_JPEGPreview();
            return KEY_NO_KEY;
        }
        else if (__MMSelectCursor.bPosition == POS_DLG_ITEM)
        {
            if (__bMMPopMenuID == MM_POP_MENU_FILTER)
            {
                if (((__bMMExtFilterMode >> __MMSelectCursor.wIndex) & 0x1) == 0x1)
                {
                    __bMMExtFilterMode &= ~(0x1<<__MMSelectCursor.wIndex);
                }
                else
                {
                    __bMMExtFilterMode |= (0x1<<__MMSelectCursor.wIndex);
                }
            }
            else if (__bMMPopMenuID == MM_POP_MENU_SORT)
            {
                if (__MMSelectCursor.wIndex < MM_SORT_TYPE)
                    __bMMSortMethod = (0x1 << __MMSelectCursor.wIndex);
                else
                    __bMMSortSequence = 0x10 << (__MMSelectCursor.wIndex-MM_SORT_TYPE);
            }
            __bMMExtFilterModeInMMMode = __bMMExtFilterMode;
            
            OSDMM_Update(OSDMM_MSG_UPDATE_DLG_ITEM, 0);
            return KEY_NO_KEY;
        }
        break;

    case    KEY_PREV:
    case    KEY_NEXT:
        // wyc0.85, because current solution support JPG preview, so need to reset this value when move cursor. Otherwise the state
        // will be wrong state from JPG preview when cursor stay in other files.
        if (__dwMMTriggerMode == MM_IMAGE_TRIGGER_MODE)
        {
            __bMMStage = MM_STAGE_NONE;
            __btPlaying = FALSE;
        }
    case    KEY_SCF:
    case    KEY_SCB:
        // wyc1.07a, not allow KEY_NEXT / KEY_PREV when file playing and UI is in program list mode.
        if ((__bAttrPlay & TYPE_CDROM_AUDIO) && (__bCountStop==0))
        {
            // wyc1.20-909, when KEY_NEXT/KEY_PREV in program list mode & audio play mode. Don't call OSDMM_MSG_STOP, otherwise the help
            // message in bottom will disappear when KEY_NEXT/KEY_PREV.
            if (__bMMMenuID != MM_MENU_PROGLIST)
                OSDMM_Update(OSDMM_MSG_STOP, 0);

            return bKey;
        }
        else // playing mode
        {
            // Transfer input key as KEY_UP/ KEY_DOWN
            // wyc0.87, make KEY_NEXT / KEY_PREV in stop mode to enter next / prev page in FM UI mode.
            if ((bKey==KEY_NEXT) || (bKey == KEY_SCF))
            {
                // When highlight in program list mode, need to support the KEY_NEXT to next program list page.
                if (__MMSelectCursor.bPosition == POS_PROG_LIST)
                {
                    if((__wOSDMMStartDrawItem+OSDMM_MAX_ITEM_IN_FILE_WINDOW) >= __bProgramMaxEntry) //CoCo.CT909
                        return INVALID_KEY;
                    OSDMM_Update(OSDMM_MSG_UPDATE_FILE_WINDOW_NEXT, 0);
                    __MMSelectCursor.wIndex += OSDMM_MAX_ITEM_IN_FILE_WINDOW;
                    if (__MMSelectCursor.wIndex > (__bProgramMaxEntry-1))
                        __MMSelectCursor.wIndex = __bProgramMaxEntry-1;
                }
                else
                {
                    // wyc0.70, to support KEY_NEXT / KEY_PREV OK in sort/filter mode, so need to add new checking mechanism here.
                    _dwMMTemp7=(DWORD)(__pMMFilterFilesOfDir[__MMSelectCursor.wDirID] + __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir);
                    if((__wOSDMMStartDrawItem+OSDMM_MAX_ITEM_IN_FILE_WINDOW) >= _dwMMTemp7)
					{
                        return INVALID_KEY;
					}

                    if (__MMSelectCursor.bPosition == POS_FILE_LIST)
					{
                        __MMSelectCursor.wIndex += OSDMM_MAX_ITEM_IN_FILE_WINDOW;
					}
                    OSDMM_Update(OSDMM_MSG_UPDATE_FILE_WINDOW_NEXT, 0);
                    // wyc0.85, when cursor is bigger than existed files in directory, need to update select cursor to max. existed
                    // files. Otherwisr will see select cursor stay in wrong position.
                    if ((__MMSelectCursor.bPosition == POS_FILE_LIST) && (__MMSelectCursor.wIndex >= _dwMMTemp7))
					{
                        __MMSelectCursor.wIndex = (WORD)(_dwMMTemp7-1);
					}
                }
            }
            else  // KEY_PREV
            {
                if (__wOSDMMStartDrawItem <= 0)
				{
                    return INVALID_KEY;
				}

                if ((__MMSelectCursor.bPosition == POS_FILE_LIST) || (__MMSelectCursor.bPosition == POS_PROG_LIST))
                {
                    if (__MMSelectCursor.wIndex < OSDMM_MAX_ITEM_IN_FILE_WINDOW)
					{
                        __MMSelectCursor.wIndex = 0;
					}
                    else
					{
                        __MMSelectCursor.wIndex -= OSDMM_MAX_ITEM_IN_FILE_WINDOW;
					}
                }

                OSDMM_Update(OSDMM_MSG_UPDATE_FILE_WINDOW_PREVIOUS, 0); 
            }

            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
            MM_JPEGPreview();
            return KEY_NO_KEY;
        }
    case    KEY_UP:
    case    KEY_DOWN:
        // wyc0.85, because current solution support JPG preview, so need to reset this value when move cursor. Otherwise the state
        // will be wrong state from JPG preview when cursor stay in other files.
        if (__dwMMTriggerMode == MM_IMAGE_TRIGGER_MODE)
        {
            __bMMStage = MM_STAGE_NONE;
            __btPlaying = FALSE;
            // wyc1.11c, fix when move cursor to dir part when JPG decode not OK, then press number
            // key to re-play JPG, then only show preview picture problem.
            __dwMMJPEGPreview = FALSE;
        }

        // selection cursor @ directory list
        if (__MMSelectCursor.bPosition == POS_VOL_DIR_LIST)
        {
            // Process KEY_UP case
            if (bKey==KEY_UP)
            {
                // Calculate final desired active directory ID first
                // underflow, return invalid key
                if(__MMSelectCursor.wIndex==0)
                {
                    return INVALID_KEY;
                }
                else
                {
                    _dwMMTemp5=(BYTE)MM_IndexConvert(MM_INDEX_CONVERT_ABS2REL_DIR_LIST, __MMSelectCursor.wDirID, (WORD)(__MMSelectCursor.wIndex-1));

                    // Update __bActiveDirChange flage as TRUE
                    // while final directory & current directory ID are different
                    // Notice: __bActiveDirChange value must be ready first
                    //         becasue OSDMM_MSG_CLEAR_HIGHLIGHT will reference it
                    // If the value == TRUE, means old folder icon will be closed
                    // The flage will be cleared after OSDFM referenced done.
                    if (_dwMMTemp5 != __MMSelectCursor.wDirID)
					{
                        __bActiveDirChange=TRUE;
					}

                    // clear selection cursor first
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); 

                    // decrease cursor index by '1'
                    __MMSelectCursor.wIndex--;
                }
            }
            else // Process KEY_DOWN case
            {
                // overflow, return invalid key
                // Notice: cursor index from '0'
                if((__MMSelectCursor.wIndex+1)==_MM_QueryInfo2(MM_QUERY2_TOTAL_ITEM_OF_DIRPAGE,__MMSelectCursor.wDirID))
                {
                    return INVALID_KEY;
                }
                else
                {
                    // Calculate final desired active directory ID first
                    _dwMMTemp5=(BYTE)MM_IndexConvert(MM_INDEX_CONVERT_ABS2REL_DIR_LIST, __MMSelectCursor.wDirID, (WORD)(__MMSelectCursor.wIndex+1));

                    // Update __bActiveDirChange flage as TRUE
                    // while final directory & current directory ID are different
                    // Notice: __bActiveDirChange value must be ready first
                    //         becasue OSDMM_MSG_CLEAR_HIGHLIGHT will reference it
                    // If the value == TRUE, means folder icon will be closed
                    // The flage will be cleared after OSDFM referenced done.
                    if (_dwMMTemp5 != __MMSelectCursor.wDirID)
					{
                        __bActiveDirChange=TRUE;
					}

                    // clear selection cursor first
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); 

                    // increase cursor index by '1'
                    __MMSelectCursor.wIndex++;
                }
            }
            // Update final active directory ID
            __MMSelectCursor.wDirID=_dwMMTemp5;
            _bMMNextSelectDir = __MMSelectCursor.wDirID;

            // Prepare the file info. for file list
            MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
            // Re-draw Menu
            OSDMM_Update(OSDMM_MSG_UPDATE_CHANGE_DIR, 0); 
        }
        // selection cursor @ file list
        else if(__MMSelectCursor.bPosition == POS_FILE_LIST)
        {
            // process KEY_UP case
            if ((bKey==KEY_UP) || (bKey==KEY_DOWN))
            {
                if (((__MMSelectCursor.wIndex==0) && (bKey==KEY_UP)) || ((__MMSelectCursor.wIndex==(_MM_QueryInfo2(MM_QUERY2_TOTAL_SORT_FILTER_ITEM,__MMSelectCursor.wDirID)-1) && (bKey==KEY_DOWN))))
                {
                    // wyc1.05, supporting moving KEY_UP / KEY_DOWN to acrossing directory boundary.
                    // wyc1.10, support a define to ring the select cursor in same directory.
                    if (bKey == KEY_UP)
                    {
#ifndef  SUPPORT_FM_CURSOR_RING_SAME_DIRECTORY
                        if (__MMSelectCursor.wDirID > 0)
						{
                            _bMMNextSelectDir = __MMSelectCursor.wDirID - 1;
						}
                        else
						{
                            _bMMNextSelectDir = (__bMMTotalDirs-1);
						}
#else
                        _bMMNextSelectDir = __MMSelectCursor.wDirID;
#endif //SUPPORT_FM_CURSOR_RING_SAME_DIRECTORY
                    }
                    else //KEY_DOWN
                    {
#ifndef  SUPPORT_FM_CURSOR_RING_SAME_DIRECTORY
                        if (__MMSelectCursor.wDirID < (__bMMTotalDirs-1))
						{
                            _bMMNextSelectDir = __MMSelectCursor.wDirID + 1;
						}
                        else
						{
                            _bMMNextSelectDir = 0;
						}
#endif //SUPPORT_FM_CURSOR_RING_SAME_DIRECTORY
                    }
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); 
                    MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
#ifndef  SUPPORT_FM_CURSOR_RING_SAME_DIRECTORY
                    _bMMAcrossDirType = MM_UP_DOWN_ACROSS_DIR;
                    _MM_SimulateChangeDir();
                    __MMSelectCursor.bPosition=POS_FILE_LIST;
                    // wyc1.07, when no files or dirs in this active dir, then move the select cursor to dir list.
                    // wyc0.70, need to use queny file num after filtering.
                    if (_MM_QueryInfo2(MM_QUERY2_TOTAL_SORT_FILTER_ITEM,__MMSelectCursor.wDirID) == 0)
                    {
                        _MM_SetCursorToActiveDirPos();
                    }
                    else
#endif //SUPPORT_FM_CURSOR_RING_SAME_DIRECTORY
                    {
                        if (bKey == KEY_UP)
						{
                            __MMSelectCursor.wIndex = _MM_QueryInfo2(MM_QUERY2_TOTAL_SORT_FILTER_ITEM,__MMSelectCursor.wDirID)-1;
						}
                        else
						{
                            __MMSelectCursor.wIndex = 0;
						}
                    }

                    OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 
                }
                else
                {
                    if (bKey == KEY_UP)
                    {
                        // clear selection cursor first
                        OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0);
                        // decrease the index by '1'
                        __MMSelectCursor.wIndex--;
                    }
                    else
                    {
                        // clear selection cursor first
                        OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0);
                        // increase the index by '1'
                        __MMSelectCursor.wIndex++;
                    }
                }

                if (MM_JPEGPreview())
				{
                    return KEY_NO_KEY;
				}
            }
        }
        else if (__MMSelectCursor.bPosition == POS_PROG_LIST)
        {
            if (bKey == KEY_UP)
            {
                if (__MMSelectCursor.wIndex == 0)
				{
                    return KEY_INVALID;
				}
                OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT,0);
                __MMSelectCursor.wIndex--;
            }
            else if (bKey == KEY_DOWN)
            {
                if (__MMSelectCursor.wIndex == (__bProgramMaxEntry-1))
				{
                    return KEY_INVALID;
				}
                OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT,0);
                __MMSelectCursor.wIndex++;
            }

            MM_JPEGPreview();
        }
        else if (__MMSelectCursor.bPosition == POS_DLG_ITEM)
        {
            bMMRet = MM_RETURN_NONE;
            OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT,0);
            if (__bMMPopMenuID == MM_POP_MENU_FILTER)
            {
                if (bKey == KEY_UP)
                {
                    if ((__MMSelectCursor.wIndex % OSDMM_FILTER_DLG_ROW_NUM) == 0)
					{
                        bMMRet = MM_RETURN_INVALID;
					}
                    else
					{
                        __MMSelectCursor.wIndex--;
					}
                }
                else
                {
                    if ((__MMSelectCursor.wIndex % OSDMM_FILTER_DLG_ROW_NUM) == (OSDMM_FILTER_DLG_ROW_NUM-1))
                    {
                        __MMSelectCursor.bPosition = POS_BUTTON_LIST;
                        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
                    }
                    else
                    {
                        // wyc1.05, fix when MAX_EXTENSION_NO lenn than 8, then the un-supported file will appear after move cursor
                        // to its index.
                        if ((__MMSelectCursor.wIndex+1) < MAX_EXTENSION_NO)
						{
                            __MMSelectCursor.wIndex++;
						}
                        else
                        {
                            __MMSelectCursor.bPosition = POS_BUTTON_LIST;
                            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
                        }
                    }
                }
            }
            else if (__bMMPopMenuID == MM_POP_MENU_SORT)
            {
                if (bKey == KEY_UP)
                {
                    if (__MMSelectCursor.wIndex == 0)
					{
                        bMMRet = MM_RETURN_INVALID;
					}
                    else
					{
                        __MMSelectCursor.wIndex--;
					}
                }
                else
                {
                    if (__MMSelectCursor.wIndex == (MM_SORT_TYPE + 2 -1)) // increase 2 is adding ascending/descending.
                    {
                        __MMSelectCursor.bPosition = POS_BUTTON_LIST;
                    }
                    else if (__MMSelectCursor.wIndex == (MM_SORT_TYPE - 1))
                    {
                        __MMSelectCursor.bPosition = POS_BUTTON_LIST;
                    }
                    else
					{
                        __MMSelectCursor.wIndex++;
					}
                }
            }
            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
            if (bMMRet == MM_RETURN_INVALID)
			{
                return KEY_INVALID;
			}
        }
        else if (__MMSelectCursor.bPosition == POS_BUTTON_LIST)
        {
            if ((bKey == KEY_UP) && (__bMMPopMenuID != MM_POP_MENU_NONE))
            {
                OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0);
                __MMSelectCursor.bPosition = POS_DLG_ITEM;
                if (__bMMPopMenuID == MM_POP_MENU_SORT)
				{
                    __MMSelectCursor.wIndex = MM_SORT_TYPE+2-1; // increase 2 is because adding ascending & descending items.
				}
                else
				{
                    __MMSelectCursor.wIndex = MAX_EXTENSION_NO-1;
				}
            }
            else
            {
                return KEY_INVALID;
            }
        }

        // wyc1.05, need to update active color when move up/down.
        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 

        // return no key because all thing done
        return KEY_NO_KEY;

        break;

    // LLY2.81, re-adjust the procedure to consider if support runtime selection cursor
    case    KEY_LEFT:
    case    KEY_RIGHT:
        // wyc0.85, because current solution support JPG preview, so need to reset this value when move cursor. Otherwise the state
        // will be wrong state from JPG preview when cursor stay in other files.
        if (__dwMMTriggerMode == MM_IMAGE_TRIGGER_MODE)
        {
            __bMMStage = MM_STAGE_NONE;
            __btPlaying = FALSE;
            __dwMMJPEGPreview = FALSE;
        }

        // selection cursor @ directory list
        if (__MMSelectCursor.bPosition == POS_VOL_DIR_LIST)
        {
            if (bKey == KEY_LEFT)
            {
#ifndef NO_MM_BUTTON
                if (__bMMMenuID == MM_MENU_NORMAL)
                {
                    // wyc1.10-909, if we allow entering sirt/filter dialog in pause mode, then user can re-arrange the file and it will happen
                    // problem because playing file maybe filter out. So we don't allow entering sort/filter function in pause mode.
                    if ((__btPlaying == TRUE) || (__btPause))
					{
                        return KEY_INVALID;
					}
                    // clear the selection cursor
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT,0);
                    __MMSelectCursor.wIndex=MM_BTN_FILTER;
                }
                else if (__bMMMenuID == MM_MENU_EDITPROG)
                {
                    // wyc2.35-909S, add code to support disable program list mode when playing mode.
                    // clear the selection cursor
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT,0);
                    if (__btPlaying)
                    {
                        __MMSelectCursor.wIndex = MM_BTN_PLAY;
                    }
                    else
                    {
                        __MMSelectCursor.wIndex = MM_BTN_PROG_LIST;
                    }
                }
                else if (__bMMMenuID == MM_MENU_PROGLIST)
                {
                    if (__bProgramMaxEntry == 0)
					{
                        return INVALID_KEY;
					}
                    // clear the selection cursor
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT,0);
                    __MMSelectCursor.wIndex=MM_BTN_DELETE_ALL;
                }
                // update cursor @ control button list's 1st positon
                __MMSelectCursor.bPosition=POS_BUTTON_LIST;
#else
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
				if (__bMMMenuID == MM_MENU_ENCODE_FILE)
				{
					OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT,0);
					__MMSelectCursor.wIndex=MM_BTN_SAVE;
					__MMSelectCursor.bPosition=POS_BUTTON_LIST;
				}
				else
#endif //SUPPORT_COPY_FILE_IN_MM_UI
					
					return INVALID_KEY;
#endif //NO_MM_BUTTON
            }
            else // KEY_RIGHT
            {
                // if cursor @ UP_DIR, don't allow to move right
                if (_bMMExistUpDir && (__MMSelectCursor.wIndex==0x1))
				{
                    return INVALID_KEY;
				}

                // Othewise, can go following procedure
                // check if exist item within file list window
                if (_MM_QueryInfo2(MM_QUERY2_TOTAL_SORT_FILTER_ITEM,__MMSelectCursor.wDirID))
                {
                    // clear the selection cursor first
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); 
                    // Update cursor @ 1st file ID
                    __MMSelectCursor.bPosition = POS_FILE_LIST;
                    __MMSelectCursor.wIndex = __wOSDMMStartDrawItem;

                    if (MM_JPEGPreview())
                    {
                        return KEY_NO_KEY;
                    }
                }
                else
                {
                    return INVALID_KEY;
                }
            }
            // Update final cursor on menu
            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); //CoCo.CT909, OSDMM_Update needs two parameters.
        }
        // selection cursor @ file list
        else if(__MMSelectCursor.bPosition == POS_FILE_LIST)
        {
            // wyc1.05-2, record the file highlight position when lighlight is in file field. Record the value for press "play button" usage.
            if(bKey==KEY_LEFT)
            {
                // clear the selection cursor first
                OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); //CoCo.CT909, OSDMM_Update needs two parameters.
                // go to the active directory position
                _MM_SetCursorToActiveDirPos();
            }
            else // KEY_RIGHT
            {
                // wyc2.38-909s, support no button code.
#ifndef NO_MM_BUTTON
                if (__bMMMenuID == MM_MENU_NORMAL)
                {
                    // wyc1.10-909, if we allow entering sirt/filter dialog in pause mode, then user can re-arrange the file and it will happen
                    // problem because playing file maybe filter out. So we don't allow entering sort/filter function in pause mode.
                    if ((__btPlaying == TRUE) || (__btPause))
                        return KEY_INVALID;
                    // clear the selection cursor
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); //CoCo.CT909, OSDMM_Update needs two parameters.
                    __MMSelectCursor.wIndex=MM_BTN_SORT;
                }
                else if ((__bMMMenuID == MM_MENU_EDITPROG) 
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
                    || (__bMMMenuID == MM_MENU_ENCODE_FILE)
#endif //
                    )
                {
                    // clear the selection cursor
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); //CoCo.CT909, OSDMM_Update needs two parameters.
                    __MMSelectCursor.wIndex=MM_BTN_PLAY;
                }
                else if (__bMMMenuID == MM_MENU_PROGLIST)
                {
                    // clear the selection cursor
                    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); //CoCo.CT909, OSDMM_Update needs two parameters.
                    __MMSelectCursor.wIndex=MM_BTN_RETURN;
                }
                // update cursor @ control button list's 1st positon
                __MMSelectCursor.bPosition=POS_BUTTON_LIST;
#else
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
                    if (__bMMMenuID == MM_MENU_ENCODE_FILE)
                    {
                        OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); //CoCo.CT909, OSDMM_Update needs two parameters.
                        __MMSelectCursor.wIndex=MM_BTN_EXIT;
                        __MMSelectCursor.bPosition=POS_BUTTON_LIST;
                    }
                    else
#endif //
                return INVALID_KEY;
#endif //
            }

            // Update final cursor on menu
            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); //CoCo.CT909, OSDMM_Update needs two parameters.
        }
        // selection cursor @ button list
        else if(__MMSelectCursor.bPosition == POS_BUTTON_LIST)
        {
            if (__bMMPopMenuID != MM_POP_MENU_NONE)
                return INVALID_KEY;
            OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT,0);
            // Specify the desired finding action base on input key
            if(bKey==KEY_LEFT)
            {
                // underflow, return invalid key
                if(__MMSelectCursor.wIndex==0)
                {
                    if (__bMMMenuID == MM_MENU_PROGLIST)
                    {
                        if (__bProgramMaxEntry == 0)
                        {
                            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
                            return INVALID_KEY;
                        }
                        else
                        {
                            __MMSelectCursor.bPosition = POS_PROG_LIST;
                            __MMSelectCursor.wIndex = 0;
                        }
                    }
                    else
                    {
                        // go to the active directory position
                        _MM_SetCursorToActiveDirPos();
                    }
                }
                // else, finding previous valid control button
                else
                {
                    if ((__bProgramMaxEntry == 0) && (__bMMMenuID == MM_MENU_PROGLIST))
                    {
                        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
                        return INVALID_KEY;
                    }
                    else
                    {
                        // wyc2.35-909S, add code to support disable program list mode when playing mode.
                        if (__btPlaying)
                        {
                            _MM_SetCursorToActiveDirPos();
                        }
                        else
                        {
                            __MMSelectCursor.wIndex=0;
                        }
                    }
                }
            }
            else // KEY_RIGHT
            {
                // overflow, return invalid key
                if(__MMSelectCursor.wIndex==(MM_MAX_CTL_BTN_NO-1))
                {
                    if (__bMMMenuID == MM_MENU_PROGLIST)
                    {
                        if (__bProgramMaxEntry == 0)
                        {
                            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
                            return FALSE;
                        }
                        else
                        {
                            __MMSelectCursor.bPosition = POS_PROG_LIST;
                            __MMSelectCursor.wIndex = 0;
                        }
                    }
                    else
                    {
                        if(_MM_QueryInfo2(MM_QUERY2_TOTAL_SORT_FILTER_ITEM,__MMSelectCursor.wDirID))
                        {
                            // Update cursor @ 1st file ID
                            __MMSelectCursor.bPosition=POS_FILE_LIST;
                            __MMSelectCursor.wIndex=__wOSDMMStartDrawItem; //CoCo.CT909

                            //CoCo0.72, support JPEG Preview
                            if (MM_JPEGPreview())
                                return KEY_NO_KEY;
                        }
                        else
                        {
                            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
                            return INVALID_KEY;
                        }
                    }
                }
                // else, finding next valid control button
                else
                {
                    __MMSelectCursor.wIndex=1;
                }
            }

            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
        }
        // selection cursor @ program list
        else if(__MMSelectCursor.bPosition == POS_PROG_LIST)
        {
            OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT,0);
            // update cursor @ control button list's 1st positon
            __MMSelectCursor.bPosition=POS_BUTTON_LIST;
            if (bKey == KEY_LEFT)
            {
                __MMSelectCursor.wIndex = MM_BTN_DELETE_ALL;
            }
            else if (bKey == KEY_RIGHT)
            {
                __MMSelectCursor.wIndex=MM_BTN_RETURN;
            }
            // Update final cursor on menu
            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
        }
        else if (__MMSelectCursor.bPosition == POS_DLG_ITEM)
        {
            OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT,0);
            if (__bMMPopMenuID == MM_POP_MENU_FILTER)
            {
                if (__MMSelectCursor.wIndex < OSDMM_FILTER_DLG_ROW_NUM)
                    __MMSelectCursor.wIndex+=(OSDMM_FILTER_DLG_ROW_NUM);
                else
                    __MMSelectCursor.wIndex-=OSDMM_FILTER_DLG_ROW_NUM;
                if (__MMSelectCursor.wIndex >= MAX_EXTENSION_NO)
                {
                    __MMSelectCursor.bPosition = POS_BUTTON_LIST;
                }
            }
            else if (__bMMPopMenuID == MM_POP_MENU_SORT)
            {
                if (bKey == KEY_LEFT)
                {
                    if (__MMSelectCursor.wIndex < MM_SORT_TYPE)
                    {
                        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
                        return KEY_INVALID;
                    }
                    else
                        __MMSelectCursor.wIndex = 0;
                }
                else if (bKey == KEY_RIGHT)
                {
                    if (__MMSelectCursor.wIndex >= MM_SORT_TYPE)
                    {
                        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
                        return KEY_INVALID;
                    }
                    else
                        __MMSelectCursor.wIndex = MM_SORT_TYPE;
                }
            }
            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
        }
        // return no key because all thing done
        return KEY_NO_KEY;
        break;

    case    KEY_STOP:
        // wyc0.87, avoid second time KEY_STOP to make cuesor move up to 1st playing file.
        if (__bCountStop >= 2)
            return KEY_STOP;
    case    KEY_RETURN:
    // wyc0.80, when press KEY_MENU, let it go same path as KEY_STOP.
    case    KEY_MENU:
        // Use __MMPlayingItem.wDirID to keep final directory information
        // Because, we want to check final directory and active directory if same
        // Then, to re-draw Menu
        // [1] Pre-Stop : same as current __MMPlayingItem.wDirID
        // [2] Real-Stop : return to ROOT, so __MMPlayingItem.wDirID=0
        // [3] Return: return to current playing directory
#ifdef SUPPORT_ENCODE_JPG_PICTURE
        if ((__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI) && ((__bKey == KEY_MENU) || (__bKey == KEY_RETURN)))
        {
            return INVALID_KEY;
        }
#endif //SUPPORT_ENCODE_JPG_PICTURE

        if ((__bCountStop==1) && (bKey==KEY_STOP) && (__bMMMenuID != MM_MENU_NONE))// Pre-stop --> Real Stop
        {
            //CoCo1.25, MP3 playing ==> move cursor to the ROOT (no other DIRs in the left window)==>KEY_STOP==>STOP
            if (__MMSelectCursor.bPosition == POS_VOL_DIR_LIST)
                return KEY_STOP;

            OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0);
            // wyc0.91, when rel stop in program list mode, need to clear all program entries because real stop will clear all program entries.
            if (__bMMMenuID == MM_MENU_PROGLIST)
            {
                OSDMM_Update(OSDMM_MSG_PROGRAM_DELETE_ALL,0);
#ifndef NO_MM_BUTTON
                OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_DELETE_ALL);
#endif //
                __MMSelectCursor.bPosition = POS_BUTTON_LIST;
                __MMSelectCursor.wIndex = MM_BTN_RETURN;
                OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
                if (__btProgram)
                    LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
                // wyc0.91, the reason of setting to TRUE is because system don't exit program list mode yet and we assume program
                // list mode is also in program mode. So need to initial these two variables to TRUE. Otherwise, there are problems
                // when press BTN_RETURN to back to program edit mode because of program variables wrong.
                __btProgram= TRUE;
                __btInputProgram=TRUE;
            }
            else
            {
                __MMSelectCursor.wIndex = __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir ;
                // wyc0.80, when JPG slide show and press KEY_STOP to enter MM UI, then press KEY_STOP again. Need to handle the STOP
                // action here because if let CC to handle KEY_STOP, then preview action will be ignored.
                OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
                if (__pMMFileRecord->FileName.bExtID == EXTNAME_JPG)
				{
                    MM_JPEGPreview();
				}

                if (__btProgram) //Clear program mode in real-stop mode.
				{
                    LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
				}
            }
            __bCountStop = 2;
#ifdef SET_REPEAT_OFF_WHEN_FULL_STOP
            __bRepeat = REPEAT_NONE;
#endif //
            OSD_OUTPUT_MACRO(MSG_STOP, 0, 3);
            return KEY_NO_KEY;
        }

        if ((bKey==KEY_STOP) || (bKey == KEY_MENU))
        {
#ifdef IMAGE_FRAME_SETUP 
            if (bKey == KEY_STOP)
            {
                if (__ImageFrameSetupInfo.bBrowse == SETUP_PHOTO_SETTING_BROWSE_FILE_MANAGER)
                {
#ifdef SUPPORT_ENCODE_JPG_PICTURE
                    if ((__SF_SourceGBL[0].bSourceIndex != SOURCE_SPI) && (__bAttrPlay == ATTR_JPG))
#endif //SUPPORT_ENCODE_JPG_PICTURE
                    {
                        bKey = KEY_MENU;
                    }
                }
            }
#endif //IMAGE_FRAME_SETUP

            // wyc0.80, when KEY_STOP from MM_MENU_NONE, need to enter thumbnail mode when playing JPG.
            if (__bMMMenuID == MM_MENU_NONE)
            {
#ifdef IMAGE_FRAME_SETUP
                //CoCo2.53, fix the bug about after playing the last jpeg file,UI will auto enter filemanager ui not thumbnail ui when setup browse item setting is thumbnail 
                if ((__bAttrPlay == ATTR_JPG) && (bKey == KEY_STOP))
                {
                    if ((__ImageFrameSetupInfo.bBrowse == SETUP_PHOTO_SETTING_BROWSE_THUMBNAIL) || (__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI))
                    {
                        MM_EnterThumbMode();
                        return KEY_NO_KEY;
                    }
                }
#else
                if ((__bAttrPlay == ATTR_JPG) && (bKey == KEY_STOP) && (__bLastPlayItem == FALSE))
                {
                    MM_EnterThumbMode();
                    return KEY_NO_KEY;
                }
#endif //IMAGE_FRAME_SETUP
                else
                {
                    // wyc0.85, add condition POS_BUTTON_LIST, because program JPG entries -> press button "play" -> KEY_PROGRAM to cancel
                    // program mode -> KEY_MENU -> Then __MMSelectCursor.bPosition will be in button list and need to move to file list.
                    // Otherwise will see wrong cursor because __MMSelectCursor.wIndex will be updated by __wPlayItem.
                    if ((__MMSelectCursor.bPosition == POS_UNEXIST) || (__MMSelectCursor.bPosition == POS_BUTTON_LIST))
					{
                        __MMSelectCursor.bPosition = POS_FILE_LIST;
					}
                    __MMSelectCursor.wIndex=MM_IndexConvert(MM_INDEX_CONVERT_REL2ABS_FILE_LIST,
                        __MMPlayingItem.wDirID, (WORD)(__wPlayItem-1));
                    if (__btProgram)
					{
                        __MMSelectCursor.bPosition = POS_FILE_LIST;
					}
                    _MM_ChangePlayingCursorToSelectCursor();
                }
            }
            else
            {
                if (bKey == KEY_MENU)
                    return INVALID_KEY;
            }

            __bTemp = MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem);
            // wyc1.06-909, also need to issue MSG_STOP when WMA mode, otherwise icon will continue flash.
            if ((__bTemp == EXTNAME_MP3) || (__bTemp==EXTNAME_MP2) || (__bTemp==EXTNAME_WMA)
#ifdef DOLBY_CERTIFICATION
                || (__bTemp==EXTNAME_AC3)
#endif //
            )
                OSDMM_Update(OSDMM_MSG_STOP, 0);
            // wyc1.07-909, issue KEY_STOP again to enter real stop mode when __bMMEnterRealStop is set to TRUE by LINEAR_ChangeTrack -> _CheckRepeatMode.
            if (__bMMEnterRealStop == TRUE)
            {
                __bMMEnterRealStop = FALSE;
                if (__bAttrPlay != ATTR_JPG)
				{
                    __bFWKey = KEY_STOP;
				}
            }
        }
        else //Return
        {
            if (__bMMMenuID == MM_MENU_NONE)
            {
                __MMSelectCursor.bPosition=POS_FILE_LIST;
                __MMSelectCursor.wIndex=MM_IndexConvert(MM_INDEX_CONVERT_REL2ABS_FILE_LIST,
                    __MMPlayingItem.wDirID, (WORD)(__wPlayItem-1));
            }
            else
            {
                OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0);
                _MM_SetCursorToActiveDirPos();

                // wyc2.31-909S, fix key return when MP3 program scan mode, then still see scan OSD problem.
                if (__bScanLevel!=0)
                {
                    // wyc0.71, need to assign key to show OSD, otherwise KEY_SCF OSD will not display
                    __bScanLevel = 0;
                    __bKeyMessage = MSG_SCANFORWARD;
                    OSD_OUTPUT_MACRO ( MSG_SCANFORWARD, (WORD)__bScanLevel, 0 );
                }
                if (__btPause)
                {
                    OSD_OUTPUT_MACRO ( MSG_PLAYING, 0, 0 );
                }

                OSD_OUTPUT_MACRO(MSG_RETURN, (WORD)NULL, 3);

                if ((__bAttrPlay & TYPE_CDROM_AUDIO) && (__bCountStop==0))
				{
                    OSDMM_Update(OSDMM_MSG_STOP, 0); 
				}
            }

            // wyc0.85, when pressing KEY_RETURN, we only need to do stop and don't let other process key function to access it any more.
            __dwMMKeyToDo = TRUE;
        }
        // LLY0.84, process the action for re-draw the Menu
        // [1] Re-initial the Menu while none Menu exist
        // Move the code below the setting __MMSelectCursor related code. To fix when playing JPG and
        // press KEY_STOP, then FM UI will show file menu in file 1 first then show the correct one.
        if(__bMMMenuID==MM_MENU_NONE)
        {
#ifdef IMAGE_FRAME_SETUP 
            __bMMFirstJPEGPlaying = TRUE;
#endif //IMAGE_FRAME_SETUP
            // LLY1.11, clear previous play item & directory as un-exist for pre-stop case
            // while AVI pre-scan not done or no index table.
            // Otherwise, it will treat as the same item, and don't pre-scan AVI index table while re-playback.
            __dwTemp = PARSER_QueryContainerAudioType(__dwSFStreamID, __bASTNO);
            if( (MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem) == EXTNAME_AVI)  &&
                ((!__bPBReady) || (__bNoDivXIndexTbl) || (__dwTemp == PARSER_ESFMT_WMA_V1) || (__dwTemp == PARSER_ESFMT_WMA_V2)) )
            {
                __wPrevPlayDir=0xFFFF;
                __wPrevPlayItem=0xFFFF;
            }

            __bCountStop = 1;
            // wyc2.35-909S, clear scan level to fix stop from AVI scan mode, then MP3 can't pause problem.
            __bScanLevel = 0;
            //CoCo0.95, stop it before drawing the UI.
            //Fix the bug about "1. Normal Play->STOP=>結果"檔案類型"會覆蓋在High Light框所mark的檔名上. 2.Normal Play->STOP=>結果UI會缺角. 3. Normal Play->STOP=>結果UI的字體會錯亂."
            CC_KeyCommand(KEY_STOP);
            __MMSelectCursor.wDirID=__MMPlayingItem.wDirID;
            _bMMNextSelectDir = __MMSelectCursor.wDirID;

            //If it is a zoom mode, recover it to normal state.
#ifdef SUPPORT_GRAPH_ZOOM
            if (__bZoomFactor != VIEW_NORMAL)
            {
				__bZoomFactor = VIEW_NORMAL;
                HAL_Zoom (VIEW_NORMAL);
                OSD_OUTPUT_MACRO ( MSG_ZOOM, (WORD)__bZoomFactor, 0x3 ); //Clear PIP and OSD
                OSD_OUTPUT_MACRO ( MSG_ZOOM_TEXT, (WORD)__bZoomFactor, 0x3 ); //Clear PIP and OSD
            }
#endif //SUPPORT_GRAPH_ZOOM
            // wyc1.20a-909, clear repeat AB message when enter STOP mode from repeat AB mode. Otherwise the repeat AB message will recover.
            if (__bRepeatAB!=REPEAT_NONE)
            {
                __bRepeatAB = REPEAT_NONE;
                OSD_OUTPUT_MACRO ( MSG_REPEATAB, REPEAT_NONE, 0);
            }
            // wyc0.80, when press KEY_STOP from CDROM motion playing, need to clear screen.
            // wyc2.22-909s, move to here to fix MM UI will be destroyed by HAL_ClearScreen problem. Because 16M MM UI will share the frame
            // buffer memory.
            if (__bAttrPlay & TYPE_CDROM_AV)
            {
                // wyc0.85, need to stop decoder because of disp will continue to display and maybe query current frame ID and clear it,
                // but dis have displayed to next picture. So current frame buffer still not cleated.
                HAL_PlayCommand(COMMAND_STOP, 0);
                HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
            }

            // When press stop in program playing mode, need to enter EDIT PROG mode. If not in program mdoe, enter normal mode.
            // wyc1.10, when simple program mode, don't enter program edit mode and just enter normal mode when stop.
            if (__bLastPlayItem == TRUE)
            {
                LINEAR_Program(LINEAR_PROGRAM_CLEAR,0);
                MM_InitialMenu(MM_MENU_NORMAL|MM_CLEAR_BACKGROUND, FALSE);
            }
            else
            {
                if (__btProgram)
				{
                    MM_InitialMenu(MM_MENU_EDITPROG|MM_CLEAR_BACKGROUND, FALSE);
				}
                else
				{
                    MM_InitialMenu(MM_MENU_NORMAL|MM_CLEAR_BACKGROUND, FALSE);
				}
            }

            if (__bAttrPlay & TYPE_CDROM_VIDEO)
            {
                // wyc2.22-909s, need to abort JPG decode because JPG decoder use same memory buffer as MM UI usage.
                HALJPEG_Abort();
                MM_JPEGPreview();
            }
#ifndef NO_MM_BUTTON
            if (__btPlaying && !__btPlayingProgram)
            {
                OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_FILTER);
                OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_SORT);
            }
#endif //NO_MM_BUTTON
            // wyc0.85, when play JPG then press KEY_RETURN, need to show UI first then show "RETURN" OSD. Because if show "RETURN"
            // before show MM UI, "return" OSD will clear by MM UI.
            if (bKey == KEY_RETURN)
            {
                OSD_OUTPUT_MACRO(MSG_RETURN, (WORD)NULL, 3);
            }
            else
            {
                OSD_OUTPUT_MACRO(MSG_STOP, 0, 3);
            }
            // wyc0.75, need to force it entering stop mode when press KEY_STOP from JPG slide show.
            // wyc0.85, remove display UI when entering MM UI to aovid play JPG -> Stop to MM UI -> Play DAT, then display mode wrong problem.
            OSD_RemoveUI(OSD_UI_DISPLAY);

#if defined (SUPPORT_PSNR_TEST) || defined (SUPPORT_DIVX_CERTIFICATION)
            // LLY1.05, assign __bCountStop as 2 while F/W assign KEY_STOP or KEY_RETURN
            // Otherwise, it will go resume if following key is KEY_PLAY
            if( (bKey==KEY_STOP || bKey==KEY_RETURN) && (__bKeySource==KEY_FROM_FW) )
            {
                __bCountStop=2;
            }
#endif  // #ifdef SUPPORT_DIVX_CERTIFICATION

            // LLY2.21, always assign __bCountStop as 2 while abort DRM playback w/ KEY_RETURN
            if( (bKey==KEY_RETURN) && (__bDRMPlayState==DRM_PLAY_STATE_ABORT) )
            {
                __bCountStop = 2;
            }

            __bLastPlayItem = FALSE;

            return KEY_NO_KEY;
        }
        __bLastPlayItem = FALSE;
        // wyc0.80, need to enable button when KEY_STOP in normal playing mode.
#ifndef NO_MM_BUTTON
        if (__btPlaying && !__btPlayingProgram)
        {
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_FILTER);
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_SORT);
        }
        // wyc2.35-909S, add code to support disable program list mode when playing mode.
        if (__bMMMenuID == MM_MENU_EDITPROG)
        {
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_PROG_LIST);
        }
#endif //NO_MM_BUTTON

        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 

        // Alwasy assign __bCountStop=2,
        // [1] KEY_RETURN case only: while support pre-stop mode
        //     because nobody will set it for KEY_RETURN case
        // [2] Only support one STOP mode case
        {
        // wyc1.10, add a define to control it when need to support one stages KEY_STOP. Default is two stage KEY_STOP.
#ifdef  SIPPORT_ONE_STAGE_STOP
            __bCountStop=2;
#endif //SIPPORT_ONE_STAGE_STOP
        }
        return KEY_STOP;

        break;
#ifdef SUPPORT_AUDIO_EFFECT
    // Damon2.76, for audio EQ test
    case    KEY_EPG:
        if (__bAttrPlay & TYPE_CDROM_AUDIO)
        {
            _bAudioEQ++;
            if (_bAudioEQ > 6)
            {
               _bAudioEQ = 0;
            }
            HAL_WriteAM(HAL_AM_EQ_MODE, _bAudioEQ);
            OSDMM_ShowAudioEQ(_bAudioEQ);        
            return KEY_NO_KEY; 
        }
        break;
#endif //SUPPORT_AUDIO_EFFECT
    default:
        break;
    }

    return  bKey;
}

//  *************************************************************************
//  Function    :   MM_ProgramListToNormal
//  Description :   Recover the MM UI from program list to normal mode.
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  Global      :
//  *************************************************************************
void MM_ProgramListToNormal(void)
{
    __MMSelectCursor.bPosition = POS_FILE_LIST;
    MM_PreparePage(MM_PREPARE_FILEPAGE, __MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);

    __MMSelectCursor.wIndex = MM_BTN_PROG_LIST;
    __bMMMenuID = MM_MENU_EDITPROG; //Should identify the correct menu ID to draw the title string.
    OSDMM_Update(OSDMM_MSG_CHANGE_MODE, OSDMM_CHANGE_MODE_PROGRAMLIST_TO_PROGRAM);
    __bMMMenuID = MM_MENU_NORMAL; //Should identify the correct menu ID to draw the title string.
    OSDMM_Update(OSDMM_MSG_CHANGE_MODE, OSDMM_CHANGE_MODE_PROGRAM_TO_NORMAL);
    __MMSelectCursor.wIndex= __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir;
    OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
#ifndef NO_MM_BUTTON
    OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_FILTER);
    OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_SORT);
#endif //
}

//  *************************************************************************
//  Function    :   MM_InitialMenu
//  Description :   Initial the File Manager UI for different mode
//  Arguments   :   bMode[6:0], specify the desired mode
//                  MM_MENU_NORMAL, normal mode UI
//                  MM_MENU_EDITPROG, input program mode UI
//                  MM_MENU_PROGLIST, program list UI
//                  bMode[7], specify if it's necessary to clear background
//                  MM_CLEAR_BACKGROUND
//  Return      :   None
//  Side Effect :
//  Global      :
//  Notice      :   Must update desired information first for this API reference
//                  [1] directory ID @ __MMSelectCursor.wDirID
//                  [2] selection cursor info. @ __wSelectCursor
//  *************************************************************************
void MM_InitialMenu(BYTE bMode, BYTE bRecover)
{
    // LLY2.50, always clear un-support A-V format OSD while return to MM UI
    // Otherwise, previous OSD maybe display until time-out
    OSD_Output(MSG_STRING, 0, 0);
    
    __bMMExtFilterMode = __bMMExtFilterModeInMMMode;

    // LLY2.21a, call UTL_Config_BufferMode() to change as MM UI mode
    UTL_Config_BufferMode(DUMP_BUFF_MM_UI_MODE);

#ifndef SUPPORT_MP3_WITH_LOGO
    if(bMode & MM_CLEAR_BACKGROUND)
    {
        DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);

        // Clear this API to clear __bLOGO as LOGO_NONE
        // Otherwise, Logo --> UI --> open : no logo display
        OSD_Output(MSG_LOGO, OSD_LOGO_CLEAR, NULL);

    }
#endif  // #ifndef SUPPORT_MP3_WITH_LOGO

    HAL_AdjustTFT_DigitalMode(TRUE);

    //kevin1.00, JPEG & MPEG use diff gains
    __bTVE_JPEG = TRUE;  //set as JPEG menu 'cause JPEG will do preview on the menu
    HAL_SetTVE();

    // wyc1.10, when initial menu from JPG (or DAT / VOB) playing and slide show mode. We need to initial the UI instead of recover UI.
    // So set bRecover to 0 to re-initial FM UI.
    if ((__bMMMenuID == MM_MENU_NONE) && (__bCountStop == 0))
	{
        bRecover = 0;
	}

    // wyc1.07, need to AND 0xF is because high nibble are used to do other checking, for example, decide to clear active cursor or not.
    __bMMMenuID=bMode & 0xF;

    switch(__bMMMenuID)
    {
    case    MM_MENU_NORMAL:
        // [2] Prepare the necessary file info. of current directory
        //       Give 2nd parameter as non-zero, so it don't draw button
        MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);

        if (bRecover)
		{
            OSDMM_Update(OSDMM_MSG_CHANGE_MODE, OSDMM_CHANGE_MODE_PROGRAM_TO_NORMAL);
		}
        else
        {
            // [3] Display 1st Menu
            OSDMM_Update(OSDMM_MSG_INITIAL, 0);
        }

        // [5] Update the desired cursor
        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 
#ifndef NO_MM_BUTTON
        if (__btPlaying)
        {
            OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_FILTER);
            OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_SORT);
        }
        else
        {
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_FILTER);
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_SORT);
        }
#endif //
        break;

    case    MM_MENU_EDITPROG:
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
    case    MM_MENU_ENCODE_FILE:
#endif //SUPPORT_COPY_FILE_IN_MM_UI
        // [2] Prepare the necessary file info. of current directory
        //       Give 2nd parameter as non-zero, so it don't draw button
        MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);

        if (bRecover)
		{
            OSDMM_Update(OSDMM_MSG_CHANGE_MODE, OSDMM_CHANGE_MODE_NORMAL_TO_PROGRAM);
		}
        else
        {
            // [3] Draw the input program mode UI
            if (__bMMMenuID == MM_MENU_EDITPROG)
			{
                OSDMM_Update(OSDMM_MSG_INITIAL, OSDMM_INITIAL_MODE_GENERAL); 
			}
            else
            {
                OSDMM_Update(OSDMM_MSG_INITIAL, OSDMM_INITIAL_MODE_COPY);
            }
        }
        // wyc2.35-909S, add code to support disable program list mode when playing mode.
#ifndef NO_MM_BUTTON
        if (__bMMMenuID == MM_MENU_EDITPROG)
        {
            if (__btPlaying)
            {
                OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_PROG_LIST);
            }
            else
            {
                OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_PROG_LIST);
            }

            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_PLAY);
        }
        else
        {
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_SAVE);
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_EXIT);
        }
#endif //NO_MM_BUTTON

        // [5] Update selection cursor
        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 
        break;

    case    MM_MENU_PROGLIST:
        // Prepare the filename for progrqam list
        MM_PreparePage(MM_PREPARE_PROGLIST,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);

        // Draw the program list UI
        OSDMM_Update(OSDMM_MSG_CHANGE_MODE, OSDMM_CHANGE_MODE_PROGRAM_TO_PROGRAM_LIST);

        // Draw the final selection cursor
        OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
        break;

    default:
        break;
    }
}

//#######################################################################
//  Function    :   MM_Initial
//  Description :   Initialize some variable for File Manager's Menu
//  Parameters  :   None.
//  Return      :   Always TRUE
//  Changed     :
//  Side Effect :
//#######################################################################
void MM_Initial(BYTE bMenuMode)
{
	MM_PLAY_Initial();
	MM_File_Initial();
	MM_UI_Initial();

    // Step 2: don't enter Menu mode for Kodak Picture CD -- LLY0.84
    if(__wDiscType & CDROM_PICTURECD)
    {
        // wyc1.11-909, must prepare page when picture, necause it also have file structure.
        MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
        return; 
    }

    // Prepare the necessary information for 1st display UI
    // Turn-on selection cursor @ 1st item of file list
    __MMSelectCursor.bPosition=POS_FILE_LIST;
    __MMSelectCursor.wIndex=0x0;

    if (__bMMAutoPlay)
    {
		_MM_AutoPlay_Initial();
    }
    else
    {
        // When not auto play need to reset panel message to MSG_STOP. Otherwise
        // PANEL will continue to show MSG_LOADING but state is stop.
        PANEL_Output(MSG_STOP, (WORD)NULL);
    }

#ifdef CT950_STYLE
	_MM_PhotoFrame_Initial(bMenuMode);
#else //#ifdef IMAGE_FRAME_SETUP 
    if (__bMediaSelectUI == TRUE)
    {
        // wyc2.00-909S, clear media select mode when entering MM UI if media select UI is active. Otherwise will happen video black
        // in center problem.
        __bMediaSelectUI = FALSE;
        OSDDLG_MEDIA_SELECT_Update(OSDDLG_MSG_EXIT, OSD_UI_CLEAR_NORMAL);
    }

    while((OS_GetSysTimer() - __dwMMJPGBuffEmptyTime) < COUNT_1_SEC);

    // 1st initial the Menu
    MM_InitialMenu(MM_MENU_NORMAL|MM_CLEAR_BACKGROUND, FALSE);
	
    if (!__bMMAutoPlay)
    {
        MM_JPEGPreview();
    }
#endif //CT950_STYLE
}

//  **************************************************************************
//  Function    :   _MM_ButtonAction
//  Description :   Do corresponding action base on the selection button
//  Arguments   :   None
//  Return      :   KEY_NO_KEY, the input key has processed ok
//                  KEY_XXX, the main flow need do more action for this key
//  Side Effect :
//  **************************************************************************
BYTE _MM_ButtonAction(void)
{    
    OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0);
    if (__bMMPopMenuID != MM_POP_MENU_NONE)
    {
#ifndef NO_MM_BUTTON
		_MM_ExitPopMenuDialog();
#endif //#ifndef NO_MM_BUTTON
    }
    else
    {
        // Step 2: Do the correspond action base on desired button
        switch (__bMMMenuID)
        {
		case    MM_MENU_NORMAL:
			__MMSelectCursor.bPosition = POS_DLG_ITEM;
			if (__MMSelectCursor.wIndex == MM_BTN_FILTER)
			{
				__bMMPopMenuID = MM_POP_MENU_FILTER;
			}
			else if (__MMSelectCursor.wIndex == MM_BTN_SORT)
			{
				__bMMPopMenuID = MM_POP_MENU_SORT;
				__MMSelectCursor.wIndex = 0;
			}
			OSDMM_Update(OSDMM_MSG_SHOW_DLG,0);
			OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
			break;
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
		case    MM_MENU_ENCODE_FILE:
			if (__MMSelectCursor.wIndex == MM_BTN_SAVE)
			{
				if (__bProgramMaxEntry != 0)
				{
					_MM_ProcessPhotoSave();
				}
				else
				{
					return KEY_INVALID;
				}
			}
			else if (__MMSelectCursor.wIndex == MM_BTN_EXIT)
			{				
				MEDIA_ExitUSBSRC();
				POWERONMENU_Initial();
			}
			break;
#endif //SUPPORT_COPY_FILE_IN_MM_UI
		case    MM_MENU_EDITPROG:
#ifndef NO_MM_BUTTON
			if (__MMSelectCursor.wIndex == MM_BTN_PROG_LIST)
			{
				_MM_ProcessProgramListBtn();				
				return KEY_NO_KEY;
			}
			else if (__MMSelectCursor.wIndex == MM_BTN_PLAY)
			{
				__dwMMJPEGPreview = FALSE; 
				// wyc2.17-909S, when MP3 scan mode and pressing PLAY button in program edit mode, need to cancel scan mode.
				if ((__bAttrPlay==ATTR_MP3) && (__bScanLevel != 0))
				{
					__bScanLevel=0;
					HAL_WriteAM(HAL_AM_FF_SPEED, __bScanLevel);
				}
				if (__bCountStop == 1)
					__dwMMKeyToDo = TRUE;
				return  _MM_SettingAndPlayItem();
			}
#endif //#ifndef NO_MM_BUTTON			
			break;
		case    MM_MENU_PROGLIST:
#ifndef NO_MM_BUTTON
			if (__MMSelectCursor.wIndex == MM_BTN_DELETE_ALL)
			{
				_MM_ProcessProgramListDeleteAllBtn();
				return  KEY_NO_KEY;
			}
			else if (__MMSelectCursor.wIndex == MM_BTN_RETURN)
			{
				_MM_ReturnToEditProgramMode();				
				return KEY_NO_KEY;
			}
#endif //NO_MM_BUTTON			
			break;
		default:
			break;
        }
    }
    return KEY_NO_KEY;
}

//  *********************************************************************
//  Function    :   MM_PlayItem
//  Description :   The function will be called within LINEAR_PlayItem()
//                  And, do the necessary thing while get desired play item
//                  [1] Update the correct highlight info. in Menu mode
//                  [2] Turn on/ off OSD menu while current and next playing
//                      item format are different, ex JPG <--> MP3
//  Arguments   :   wTrackNO, the desired playing item, index from "1"
//  Return      :   None
//  Side Effect :
//  *********************************************************************
void MM_PlayItem(WORD wTrackNO)
{
    _dwMMTemp4 = MM_GetFileType(__MMPlayingItem.wDirID, wTrackNO);
    if (__bMMMenuID != MM_MENU_NONE)
    {
        // Play motion or photo files.
        if ((_dwMMTemp4 == EXTNAME_DAT) || (_dwMMTemp4 == EXTNAME_VOB)
            || (_dwMMTemp4 == EXTNAME_AVI) || (_dwMMTemp4 == EXTNAME_MPG) || (_dwMMTemp4 == EXTNAME_JPG) ||(_dwMMTemp4 == EXTNAME_MP4))  // LLY1.50, add MP4
        {
            __bMMMenuID = MM_MENU_NONE;
            OSDMM_Update(OSDMM_MSG_EXIT,0);
        }
    }
	
    if (__bMMMenuID == MM_MENU_NONE)
    {
        // wyc1.10, when these audio type without motion need to show UI when playing.
        _dwMMTemp4 = MM_GetFileType(__MMPlayingItem.wDirID, wTrackNO);
        if (( _dwMMTemp4 == EXTNAME_MP3 )
#ifdef DOLBY_CERTIFICATION
            || (_dwMMTemp4 == EXTNAME_AC3)
#endif //
            || (_dwMMTemp4 == EXTNAME_WMA)
			|| ( _dwMMTemp4==EXTNAME_MP2)
			|| (_dwMMTemp4==EXTNAME_DTS)
			)
        {
            OSD_RemoveUI(OSD_UI_DISPLAY);
            // Update active cursor information
            __MMSelectCursor.bPosition=POS_FILE_LIST;
            // convert cursor index from relative to absolute coordinate
            __MMSelectCursor.wIndex=MM_IndexConvert(MM_INDEX_CONVERT_REL2ABS_FILE_LIST, __MMPlayingItem.wDirID, (WORD)(wTrackNO-1));
			
            // Get the dir index of the desired playing item (same as playing directory ID)
            __MMSelectCursor.wDirID=__MMPlayingItem.wDirID;
            _bMMNextSelectDir = __MMSelectCursor.wDirID;
            _MM_ChangePlayingCursorToSelectCursor();
			
            if (__btProgram)
			{
                MM_InitialMenu(MM_MENU_EDITPROG|MM_CLEAR_BACKGROUND, FALSE);
			}
            else
			{
                MM_InitialMenu(MM_MENU_NORMAL|MM_CLEAR_BACKGROUND, FALSE);
			}
			
            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
			
        }
        else
        {
            // wyc1.06-909, remove UI when change to different extname file, because the display mode is diferent, so need to reset display mode to avoid error.
            switch (__bAttrPlay)
            {
			case ATTR_JPG:
				if (_dwMMTemp4!=EXTNAME_JPG)
				{
					OSD_RemoveUI(OSD_UI_DISPLAY);
				}
				break;
			case ATTR_DAT:
			case ATTR_MPG:
			case ATTR_VOB:
				if ((_dwMMTemp4!=EXTNAME_DAT) && (_dwMMTemp4!=EXTNAME_MPG) && (_dwMMTemp4!=EXTNAME_VOB))
				{
					OSD_RemoveUI(OSD_UI_DISPLAY);
				}
				break;
			case ATTR_AVI:
				if (_dwMMTemp4!=EXTNAME_AVI)
				{
					OSD_RemoveUI(OSD_UI_DISPLAY);
				}
				break;
				// wyc1.10-909, when last play item is audio files, then will go default case here. When play from audio file, then
				// we need to clear OSD before play image/motion file to avoid show wrong OSD on display mode.
			default:
				// wyc1.20-909, because fish update new mechanism in ATTR_PRESCAN_INFO, so now use follow updated code is OK.
				// wyc2.32-909S, fix quickly next/prev making display mode disappearred problem.
				if ((__bAttrPlay == ATTR_NONE) && (__bMotionState != MOTION_STATE_PLAY))
				{
					if ((__bAttrPlayNew == ATTR_AVI) && (_dwMMTemp4==EXTNAME_AVI))
						;
					else if ((__bAttrPlayNew == ATTR_DAT) && (_dwMMTemp4==EXTNAME_DAT))
						;
					else if ((__bAttrPlayNew == ATTR_VOB) && (_dwMMTemp4==EXTNAME_VOB))
						;
					else
						OSD_RemoveUI(OSD_UI_DISPLAY);
				}
				else
				{
					OSD_RemoveUI(OSD_UI_DISPLAY);
				}
				break;
            }
            if (__MMSelectCursor.wDirID != __MMPlayingItem.wDirID)
            {
                __MMSelectCursor.wDirID=__MMPlayingItem.wDirID;
                _bMMNextSelectDir = __MMSelectCursor.wDirID;
            }
        }
    }
    else
    {
        // Turn-off the OSD Menu while the desire play item is JPG
        if (( MM_GetFileType(__MMPlayingItem.wDirID, wTrackNO)==EXTNAME_JPG ) && (__dwMMJPEGPreview == FALSE))
        {
            __bMMMenuID=MM_MENU_NONE;
            __MMSelectCursor.wDirID = __MMPlayingItem.wDirID;
            OSDMM_Update(OSDMM_MSG_EXIT,0);
            return;
        }
		
        // wyc1.05, use _bMMIdx to indicate if need to move highlight or just move active color. (_bMMIdx only used in _MM_PrepareInfo and we can temp use it)
        if ((!_bMMIdx) && ((__btPlayingProgram) || (__btRandom) || (__MMPlayingItem.wIndex == 0xFFFF) || MM_SelectCursorPlayingItemEqual()))
		{
            _bMMIdx = TRUE;
		}
        __MMPlayingItem.bPosition=POS_FILE_LIST; // file list
        __MMPlayingItem.wIndex = __pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir+__wPlayItem-1;
		
        if ((_bMMIdx) && (__bMMMenuID != MM_MENU_PROGLIST))
        {
            OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0);
            _bMMAcrossDirType = MM_NEXT_PREV_ACROSS_DIR;
            _MM_SimulateChangeDir();
            __MMSelectCursor.bPosition = __MMPlayingItem.bPosition;
            __MMSelectCursor.wIndex = __MMPlayingItem.wIndex;
            OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
        }
    }
	
    // wyc0.80a, support all kinds of audio only files here, otherwise some audio files will have problem.
    if( ( _dwMMTemp4 == EXTNAME_MP3 ) || ( _dwMMTemp4 == EXTNAME_MP2 ) || (_dwMMTemp4 == EXTNAME_WMA)
#ifdef DOLBY_CERTIFICATION
        || (_dwMMTemp4 == EXTNAME_AC3)
#endif //
		)
	{
        OSDMM_Update(OSDMM_MSG_PLAYING, 0); 
	}
}

//  ***************************************************************************
//  Function    :   _MM_SimulateChangeDir
//  Description :   Simulator user operator while auto-playing crossing directory
//                  Otherwise, the final UI maybe update incorrect !!
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  Notice      :   Must call this API before final active directory & cursor
//                  information update.
//  ***************************************************************************
void _MM_SimulateChangeDir(void)
{
    // If final directory ID != current active/ open directory
    // wyc1.05-3. adjust the condition to enter follow change dir code to avoid file part flash one more times.
    if( ((__MMPlayingItem.wDirID != __MMSelectCursor.wDirID) && (_bMMAcrossDirType==MM_NEXT_PREV_ACROSS_DIR))
    || ((_bMMNextSelectDir != __MMSelectCursor.wDirID) && (_bMMAcrossDirType==MM_UP_DOWN_ACROSS_DIR)))
    {
        //Set active cursor information as active directory
        _MM_SetCursorToActiveDirPos();

        //Turn-off "folder icon" by calling OSDMM_MSG_CLEAR_HIGHLIGHT w/ __bActiveDirChange=TRUE
        __bActiveDirChange=TRUE;
        OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0);

        // check if at same level between final and active/ open directory
        // Notice: Don't use _dwMMTemp2/ _dwMMTemp1/ _dwMMTemp5
        // because they will be used within MM_PreparePage()
        if (_bMMAcrossDirType == MM_NEXT_PREV_ACROSS_DIR)
        {
            if (__pMMVolumeDirTree[__MMPlayingItem.wDirID].bParentID == __pMMVolumeDirTree[__MMSelectCursor.wDirID].bParentID)
            {
                _dwMMTemp6=TRUE; //OSDMM_MSG_UPDATE_CHANGE_DIR
            }
            else
            {
                _dwMMTemp6=FALSE; //OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL
            }

            // Update __MMSelectCursor.wDirID value first,
            // because it will be refered within MM_PreparePage()
            __MMSelectCursor.wDirID=__MMPlayingItem.wDirID;
        }
        else if (_bMMAcrossDirType == MM_UP_DOWN_ACROSS_DIR)
        {
            if ( __pMMVolumeDirTree[_bMMNextSelectDir].bParentID == __pMMVolumeDirTree[__MMSelectCursor.wDirID].bParentID)
            {
                _dwMMTemp6=TRUE; //OSDMM_MSG_UPDATE_CHANGE_DIR
            }
            else
            {
                _dwMMTemp6=FALSE; //OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL
            }
            // Update __MMSelectCursor.wDirID value first,
            // because it will be refered within MM_PreparePage()
            __MMSelectCursor.wDirID=_bMMNextSelectDir;
        }

        //Prepare the file information.
        MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);

        // Set active cursor as next active directory
        // before calling OSDMM_MSG_UPDATE_CHANGE_DIR/ CHANGE_DIR_LEVEL
        // Otherwise, the "folder icon" can't turn-on
        _MM_SetCursorToActiveDirPos();

        if ((__bKey == KEY_UP) || (__bKey == KEY_PREV))
        {
            __wTemp = _MM_QueryInfo2(MM_QUERY2_TOTAL_SORT_FILTER_ITEM,__MMSelectCursor.wDirID);
            if (__wTemp > 0)
			{
                __wMMNewCursorIndex = __wTemp - 1;
			}
            else
			{
                __wMMNewCursorIndex = 0;
			}
        }
        else
		{
            __wMMNewCursorIndex = 0;
		}

        if (_dwMMTemp6)
        {
            OSDMM_Update(OSDMM_MSG_UPDATE_CHANGE_DIR, 0); 
        }
        else
        {
            OSDMM_Update(OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL, 0);
        }
    }
    // if final directory ID == current active/ open directory
    else
    {
        // Nothing need to do
        // And, UI module will update final 1stFileID
    }
}

//  ***************************************************************************
//  Function    :   _MM_ChangePlayingCursorToSelectCursor
//  Description :   This function will clear the display mode to 0.
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  Notice      :
//  ***************************************************************************
void _MM_ChangePlayingCursorToSelectCursor(void)
{
	__MMPlayingItem.bPosition = __MMSelectCursor.bPosition;
	__MMPlayingItem.bVolID= __MMSelectCursor.bVolID;
	__MMPlayingItem.wIndex = __MMSelectCursor.wIndex;
    __MMPlayingItem.wDirID=__MMSelectCursor.wDirID;
}

BYTE _MM_SettingAndPlayItem(void)
{
    __wPlayItem = __MMSelectCursor.wIndex+1-__pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir;

    if (!__dwMMJPEGPreview)
    {
        if (__dwMMTriggerMode == MM_IMAGE_TRIGGER_MODE)
        {
            HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
        }

#ifndef NO_MM_BUTTON
        if (__bMMMenuID == MM_MENU_NORMAL)
        {
            OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_FILTER);
            OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_SORT);
        }
        // wyc2.35-909S, disable program list mode when playing mode.
        else if (__bMMMenuID == MM_MENU_EDITPROG)
        {
            OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_PROG_LIST);
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_PLAY);
        }
        else
        {
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_FILTER);
            OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_SORT);
        }
#endif //NO_MM_BUTTON
        // wyc1.05a, need to set to 0 to represent play mode here, otherwise when stop during divx prescan mode, MM UI won't appear.
        __bCountStop = 0;
    }

    // wyc0.80, when don't select any program entries and press button "play", then need to recover to normal UI and directly normal play.
    // wyc0.80a, add checking to clear program mode only non-preview mode when play item. Otherwise preview in program edit mode
    // will clear program mode, we will see program edit mode cleared after moving cursor in JPG files.
    if ((__bProgramMaxEntry == 0) && (__btProgram) && (__bMMMenuID == MM_MENU_EDITPROG) && (__dwMMJPEGPreview == FALSE))
    {
        // because only press button "play" will go this case, so the cursor is stay in button and playitem will be caulate wrong.
        // So need to re-initial __wPlayItem here.
        __wPlayItem = 1;
        LINEAR_Program(LINEAR_PROGRAM_CLEAR,(BYTE)NULL);

        //call this when play, otherwise the title message will still keep in program mode.
        OSDMM_Update(OSDMM_MSG_PLAYING,0);
    }

    // wyc1.05, when issuing KEY_PLAY in playing mode, need to check if playing cursor and select cursor is the same.
    // If not the same, need to make KEY_PLAY action, if not, skip KEY_PLAY and return to CC to control it.
    if (MM_SelectCursorPlayingItemEqual())
	{
        return KEY_PLAY;
	}

    // wyc1.11-909, when press KEY_PLAY in random mode, need to clear random mode for more reasonable.
    if (__bKeySource == KEY_FROM_ISR)
	{
        __btRandom = FALSE;
	}

    if (__btInputProgram)
    {
        __bScanLevel=0;
        HAL_WriteAM(HAL_AM_FF_SPEED, __bScanLevel);
    }

    //CoCo0.72, recover the current "flash" playing file to normal when moving cursor and press KEY_PLAY to play the new item.
    if (!__dwMMJPEGPreview)
	{
        OSDMM_Update(OSDMM_MSG_STOP, 0);
	}

    CC_KeyCommand(KEY_STOP);

    _MM_ChangePlayingCursorToSelectCursor(); 
    return KEY_PLAY;
}

//***************************************************************************
//  Function    :   MM_Exit
//  Abstract    :   This function will be called by OSD kernel to exit the UI.
//  Arguments   :   bExitMode: OSD_UI_EXIT_ALL/OSD_UI_EXIT_CONTROL_ONLY
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void MM_Exit(BYTE bExitMode)
{
    if (bExitMode == OSD_UI_EXIT_ALL)
    {
        //Clear all Media Manager OSD region.
        GDI_ClearRegion(0); //clear region 0
    }

    //Reset the button status.
    OSDMM_ResetControl();

    __bMMMenuID=MM_MENU_NONE;
}

BYTE MM_SelectCursorPlayingItemEqual(void)
{
    //The two statement is equal because __MMPlayingItem.bPosition is always at POS_FILE_LIST in our file structure.
    if ((__MMSelectCursor.wDirID == __MMPlayingItem.wDirID) && (__MMPlayingItem.wIndex == __MMSelectCursor.wIndex) && (__MMSelectCursor.bPosition == POS_FILE_LIST))
	{
        return TRUE;
	}
    else
	{
        return FALSE;
	}
}

void _MM_SetCursorToActiveDirPos(void)
{
    __MMSelectCursor.bPosition = POS_VOL_DIR_LIST;
    __MMSelectCursor.wIndex = MM_IndexConvert(MM_INDEX_CONVERT_REL2ABS_DIR_LIST, __MMSelectCursor.wDirID, __MMSelectCursor.wDirID);
}

void MM_DumpOneFileForHighlightItem(void)
{
    MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID, (WORD)(__MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir)), 1);
    MM_GetDumpedFileRecord();
}

//***************************************************************************
//  Function    :   MM_ThumbToMMEntry
//  Abstract    :   This function will be called by thumbnail to enter MM UI.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void MM_ThumbToMMEntry(void)
{
    __MMSelectCursor.bPosition = POS_FILE_LIST;
    __MMSelectCursor.wIndex=MM_IndexConvert(MM_INDEX_CONVERT_REL2ABS_FILE_LIST,
    __MMPlayingItem.wDirID, (WORD)(__wPlayItem-1));

#ifdef CT950_STYLE
    __MMPlayingItem.bPosition = __MMSelectCursor.bPosition;
#else
    _MM_ChangePlayingCursorToSelectCursor();
#endif //CT950_STYLE
    __MMSelectCursor.wDirID=__MMPlayingItem.wDirID;
    if (__btProgram)
    {
        MM_InitialMenu(MM_MENU_EDITPROG|MM_CLEAR_BACKGROUND, FALSE);
    }
    else
    {
        MM_InitialMenu(MM_MENU_NORMAL|MM_CLEAR_BACKGROUND, FALSE);
    }

    MM_JPEGPreview();
#ifndef NO_MM_BUTTON
    OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_FILTER);
    OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_SORT);
#endif //NO_MM_BUTTON
}

//  *********************************************************************
//  Function    :   MM_ProcessKey
//  Description :   It will do some key relative action:
//                  [1] Filter the un-support key, and
//                  [2] Up highlight/ Selection info of Menu
//                  [3] Do key action
//  Arguments   :   bKey    : The user key
//  Return      :   KEY_NO_KEY  , the input key is processed
//              :   INVALID_KEY , the input key is not allowed under this mode
//              :   KEY_xxx     , the main program need do more process for the key.
//  Side Effect :
//  *********************************************************************
BYTE MM_ProcessKey(void)
{
    if (!(__wDiscType & CDROM_M1) || __bThumbnailMode)
    {
        return KEY_BYPASS;
    }

    // LLY1.02, dis-allow playable key before CDROM motion playback stage ready
    // LLY2.18, must check motion state since __bAttrPlay is not set ready during prescan mode
    if( ((__bAttrPlay & TYPE_CDROM_AV) || (__bMotionState != MOTION_STATE_NONE) ) && (!__bPBReady) )

    {
        if ((__bKey>=NONNPF_PLAYABLEKEY_BEGIN) && (__bKey<=NONNPF_PLAYABLEKEY_END))
        {
            __bKey=KEY_NO_KEY;
            return KEY_INVALID;
        }
    }

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
    if (_MM_CopyDeleteMode_ProcessKey())
    {
        return KEY_NO_KEY;
    }
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW

    // Step 1: Don't processing the relative key within input program mode
    // Step 2: Transfer some keys and get the correspond message
    _dwMMTemp5=FALSE;
    switch(__bKey)
    {
    case    KEY_ENTER:
        if(__bCountStop==0) // playing mode
        {
            // wyc1.05, support moving highlight in playing mode.
            // selection exist, transfer it as KEY_PLAY
            if(__MMSelectCursor.bPosition != POS_UNEXIST)
            {
                __bKey=KEY_PLAY;
                _CC_MapKeyMessage(__bKey);
            }
            // no selection, return invalid key
            else
            {
                return INVALID_KEY;
            }
        }
        else // non-playing mode, always transfer to KEY_PLAY
        {
            __bKey=KEY_PLAY;
            _dwMMTemp5=TRUE;
            break;
        }

    case    KEY_PLAY_PAUSE:
        if (__bKey == KEY_PLAY_PAUSE)
        {
            if (MM_SelectCursorPlayingItemEqual() || (__bMMMenuID == MM_MENU_NONE))
            {
                if ( __btPlaying ) // within playing mode
                {
                    // when IR key is KEY_PLAY_PAUSE, should transfer to KEY_PLAY when scf/scb mode.
                    // Otherwise it will show invalid when audio/motion scan mode.
                    if ((__bModeCmd == KEY_SCF) || (__bModeCmd == KEY_SCB) || (__bModeCmd == KEY_FF))
                    {
                        __bKey = KEY_PLAY;
                    }
                    else
                    {
                        __bKey=KEY_PAUSE;
                        _dwMMTemp5=TRUE;
                    }
                }
                else // non-playing mode, ex pause/ stop: transfer it as KEY_PLAY
                {
                    __bKey=KEY_PLAY;
                }
            }
            else
			{
                __bKey = KEY_PLAY;
			}
        }
        // Don't break this case, because KEY_PLAY must go following procedure !!
        //break;

    case    KEY_PLAY:
        // wyc1.07-909, check input program mode first, otherwise will happen MP3 scan mode -> enter program edit mode -> KEY_PLAY ->
        // then will select program entry but also will clear scan mode. Suppose should keep in scan mode.
        // Let it go for input program mode
        if (__btInputProgram)
        {
            break;
        }
        //Kevin1.10-MP3FF, SCF(FM handle)->PLAY(FM or/and CC handle)
        // wyc2.36-909S, fix MP3 scan in program mode, then press KEY_PLAY, will cancel audio scan mode but OSD still keep in scan mode problem.
        if ((__bAttrPlay & ATTR_AUDIO_BEGIN) && (__bMMMenuID == MM_MENU_NORMAL) && __btPlaying && __bScanLevel)
        {
            // wyc1.05, when issuing KEY_PLAY in playing mode, need to check if playing cursor and select cursor is the same.
            // If not the same, need to make KEY_PLAY action, if not, skip KEY_PLAY and return to CC to control it.
            if (MM_SelectCursorPlayingItemEqual())
            {
                __bScanLevel=0;
                HAL_WriteAM(HAL_AM_FF_SPEED, __bScanLevel);
                __bModeCmd = KEY_PLAY; 
                __bKeyMessage = MSG_PLAYING; 
                OSD_OUTPUT_MACRO ( MSG_PLAYING, (WORD)NULL, 3 );
                return KEY_NO_KEY;
            }
            break;
        }

        if ((__bCountStop==1) && (__bMMMenuID == MM_MENU_NORMAL))
        {
            // wyc0.75, only key source from ISR we transfer KEY_PLAY to KEY_RESUME, when key from FW. Then mean this key have been
            // transfer once and don't transfer it again.
            if (__bKeySource == KEY_FROM_ISR)
            {
                // within file list position & index same as resume time
                // Notice: get directory ID and file ID of resume time first
                if( (__MMSelectCursor.bPosition==POS_FILE_LIST) &&
                    (__MMSelectCursor.wDirID==HIWORD(__dwResumeItem)) &&
                    (__MMSelectCursor.wIndex==MM_IndexConvert(MM_INDEX_CONVERT_REL2ABS_FILE_LIST,
                        (BYTE)(HIWORD(__dwResumeItem)), LOWORD(__dwResumeItem-1))) )
                {
                    // LLY1.11, don't do resume action while pre-scan not done & no index table
                    // Notice: YC says we can't using MM_GetFileType() w/ selection cursor before the item be playback
                    if ((__pMMFileRecordStart[MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID,
                         __MMSelectCursor.wIndex-__pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir)].FileName.bExtID)
                           == EXTNAME_AVI)
                    {
                        if((__bPBReady) && (PARSER_QueryAVIIndexTable()))
                        {
                            __bKey=KEY_RESUME;
                            _dwMMTemp5=TRUE;
                        }
                        else
                        {
                            // wyc0.75, when select cursor is not stay in resume play item, need to set __bCountStop to 2. Otherwise there
                            // are problems when playing item of select cursor.
                            __bCountStop = 2;
                        }
                    }
                    else
                    {
                        __bKey=KEY_RESUME;

                        if (MM_GetFileType(__MMPlayingItem.wDirID, __MMSelectCursor.wIndex+1) == EXTNAME_JPG)
                        {
                            HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
                        }
                        _dwMMTemp5=TRUE;
                    }
                }
                else
                {
                    // wyc0.75, when select cursor is not stay in resume play item, need to set __bCountStop to 2. Otherwise there
                    // are problems when playing item of select cursor.
                    __bCountStop = 2;
                }
            }
            else if (__bKeySource == KEY_FROM_FW)
            {
                return KEY_BYPASS;
            }
        }

        // LLY.276p-4, transfer KEY_PLAY as KEY_NEXT while
        // [1] JPEG playing
        // [2] w/ Zoom mode
        // [3] No OSD Menu mode
        // ie. don't care the time-out control
        // Notice: can't transfer key while stop w/ Zoom-In JPEG picture
		//CoCo2.78, the following code is error. 
		//Don't set __bZoomFactor to an absolute value.
        if(__bAttrPlay==ATTR_JPG && __bZoomFactor!=VIEW_NORMAL && __bMMMenuID==MM_MENU_NONE)
        {
            __bKey = KEY_ZOOM;
            __bZoomFactor = VIEW_ZOOM50;
            _dwMMTemp5=TRUE;
        }
        break;

    case    KEY_STOP_RETURN:
        if(__wDiscType & CDROM_PICTURECD)
        {
#ifdef IMAGE_FRAME_SETUP 
            if (__ImageFrameSetupInfo.bBrowse == SETUP_PHOTO_SETTING_BROWSE_THUMBNAIL)
            {
                __bKey=KEY_THUMBNAIL;
            }
            else
            {
                __bKey=KEY_MENU;
            }
#else
            __bKey=KEY_THUMBNAIL;
#endif //#ifdef IMAGE_FRAME_SETUP 
        }
        else // non-Kodak Picture CD
        {
            __bKey=KEY_STOP;
        }
        _dwMMTemp5=TRUE;
        break;

    case    KEY_PAUSE_STEP:
        if (!(__bAttrPlay & TYPE_CDROM_AV))
		{
            __bKey=KEY_PAUSE;
		}
        _dwMMTemp5=TRUE;
        break;

    case    KEY_SCF:
    case    KEY_FF:
#if (FM_KEY_SCF_KEY_SCB_MODE == MODE_SCAN_ONLY) || (FM_KEY_SCF_KEY_SCB_MODE == MODE_SCAN_PAGE) //Kevin1.26-2
        // LLY2.35, support WMA SCF mode
        //if ( (__bAttrPlay==ATTR_MP3 || __bAttrPlay==ATTR_MP2) && __btPlaying)
        if ( (__bAttrPlay==ATTR_MP3 || __bAttrPlay==ATTR_MP2 || __bAttrPlay==ATTR_WMA) && __btPlaying)
        {
            __bKey = KEY_FF;
            //clear __bScanLevel
            if (__bKey != __bModeCmd)
            {
                __bScanLevel=0;
            }
            __bModeCmd = KEY_FF;

            if (__bScanLevel == MAX_SCAN_LEVEL)
                __bScanLevel=0;
            else
                __bScanLevel++;

            HAL_WriteAM(HAL_AM_FF_SPEED, __bScanLevel); //kevin0.76, 3 bits (0~5) for FF
            // wyc1.22-AKI, avoid OSD to show "X1" when back to normal play.
            if (__bScanLevel!=0)
            {
                // wyc0.71, need to assign key to show OSD, otherwise KEY_SCF OSD will not display
                __bKeyMessage = MSG_SCANFORWARD;
                OSD_OUTPUT_MACRO ( MSG_SCANFORWARD, (WORD)__bScanLevel, 0xFF );
            }
            else
            {
                __bModeCmd = KEY_PLAY;
                _CC_MapKeyMessage(KEY_PLAY);
                OSD_OUTPUT_MACRO ( MSG_PLAYING, (WORD)NULL, 0x3 );
            }
            // end AKI ...
            __bKey=KEY_NO_KEY;
        }
        else if(__bAttrPlay==ATTR_WMA)
        {
            return KEY_BYPASS;
        }
#if (FM_KEY_SCF_KEY_SCB_MODE == MODE_SCAN_ONLY)
        //SCF is not used as page down
        else if (__bMMMenuID != MM_MENU_NONE) //not JPG slide show
            __bKey=KEY_INVALID;
#endif
#endif

        break;

    case    KEY_SCB:
#if (FM_KEY_SCF_KEY_SCB_MODE == MODE_SCAN_ONLY) || (FM_KEY_SCF_KEY_SCB_MODE == MODE_SCAN_PAGE) //Kevin1.26-2
        // LLY2.35, support WMA SCF mode
        //Kevin1.24a, support MP3/MP2 SCB
        //if ( (__bAttrPlay==ATTR_MP3 || __bAttrPlay==ATTR_MP2) && __btPlaying)
        if ( (__bAttrPlay==ATTR_MP3 || __bAttrPlay==ATTR_MP2 || __bAttrPlay==ATTR_WMA) && __btPlaying)
        {
            // LLY2.36a, initial scan action as initial
            // Other title is done @ CC.C, but there is no chance to return key to CC module.
            __bScan = SCAN_INIT;

            //clear __bScanLevel
            if (__bKey != __bModeCmd)
            {
                __bScanLevel=0;
            }
            __bModeCmd = KEY_SCB;

            if (__bScanLevel == MAX_SCAN_LEVEL)
                __bScanLevel=0;
            else
                __bScanLevel++;

            HAL_WriteAM(HAL_AM_FF_SPEED, 0);

            // wyc1.22-AKI, avoid OSD to show "X1" when back to normal play.
            if (__bScanLevel!=0)
            {
                // wyc0.70, add it to force to show scan OSD on video.
                __bKeyMessage = MSG_SCANBACKWARD;
                OSD_OUTPUT_MACRO ( MSG_SCANBACKWARD, (WORD)__bScanLevel, 0xFF );
            }
            else
            {
                 HAL_WriteAM(HAL_AM_PLAY_COMMAND, 1);

                __bModeCmd = KEY_PLAY;
                _CC_MapKeyMessage(KEY_PLAY);

                OSD_OUTPUT_MACRO ( MSG_PLAYING, (WORD)NULL, 0x3 );
            }
            // end AKI ...
            __bKey=KEY_NO_KEY;
        }
        // LLY2.35, support WMA SCAN mode
        else if(__bAttrPlay==ATTR_WMA)
        {
            return KEY_BYPASS;
        }
#if (FM_KEY_SCF_KEY_SCB_MODE == MODE_SCAN_ONLY)
        //SCB is not used as page down
        else if (__bMMMenuID != MM_MENU_NONE) //not JPG slide show
            __bKey=KEY_INVALID;
#endif
#endif

        break;

    // LLY2.81, transfer STOP/ DIGEST key as KEY_THUMBNAIL while Kodak Picture CD
    case    KEY_STOP:
        //Kevin1.10-MP3FF, SCF(FM handle)->STOP(FM&CC handle)
        if (__bAttrPlay==ATTR_MP3 && __bScanLevel)
        {
            __bScanLevel=0;
            HAL_WriteAM(HAL_AM_FF_SPEED, __bScanLevel);
            __bModeCmd = KEY_STOP;  //Kevin1.24a, add

            break;
        }
        // wyc1.06-909, just break when non-JPG mode, only JPG need to check thumbnail entry code when KEY_STOP.
        if (__bAttrPlay != ATTR_JPG)
            break;
    case    KEY_DIGEST:
        if (1)//__wDiscType & CDROM_PICTURECD)
        {
            MM_DumpOneFileForHighlightItem();

            // wyc1.10, initial the __wPlayItem before entering THUMB mode because of THUMB module will reference __wPlayItem to put highlight position.
            if ((!(__wDiscType & CDROM_PICTURECD)) && (__bMMMenuID == MM_MENU_NORMAL))
            {
                if ((__MMSelectCursor.bPosition == POS_VOL_DIR_LIST) || //Cursor is at the left DIR part.
                    (__MMSelectCursor.wIndex < __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir) || //cursor is at the folder.
                    (__pMMFileRecord->FileName.bExtID != EXTNAME_JPG)) //CoCo1.11
				{
                    __wPlayItem = _MM_GetNoneJPEGItems() + 1;
				}
                else
				{
					__wPlayItem = __MMSelectCursor.wIndex - __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir + 1;
				}
            }

            if ((__wDiscType & CDROM_PICTURECD) || (__bKey == KEY_DIGEST))
            {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
                if (__bMMJPEGAudioMode && (__bKey == KEY_DIGEST))
                {
                    return  INVALID_KEY;
                }
                else
#endif //#ifdef SUPPORT_JPEG_AUDIO_PLAYING
                    __bKey=KEY_THUMBNAIL;
            }
            _dwMMTemp5=TRUE;
        }
        break;

    case    KEY_UP:
    case    KEY_DOWN:
    case    KEY_RIGHT:
    case    KEY_LEFT:
        if( __bZoomFactor != VIEW_NORMAL ) // convert to KEY_ZOOM_UP/down/left/right
        {
            if(__bMMMenuID == MM_MENU_NONE)
            {
                if (__bAttrPlay == ATTR_JPG)
                {
                    __bKey = KEY_ZOOM_UP + (__bKey-KEY_UP);
                    _dwMMTemp5=TRUE;
                }
                // wyc0.91, support left/right/up/down image in ZOOM mode in MM-motion files.
                else
                {
                    return KEY_BYPASS;
                }
            }
        }
        break;
    default :
        break;
    }

    // Assign the corresponding value for __bKeyMessage if F/W transfering key
    if(_dwMMTemp5)
    {
        _CC_MapKeyMessage(__bKey);
    }

    //Filter un-support key base on playing mode attribute
    __bKey=_MM_FilterKey(__bKey);
    if(__bKey==KEY_INVALID)
        return INVALID_KEY;

    // Do reset and initialize while start to play
    switch(__bKey)
    {
    case    KEY_NEXT:
    case    KEY_PREV:
    case    KEY_PLAY:
    case    KEY_NUMBER:
    case    KEY_ACTIVE:
        if ((__bKey == KEY_NUMBER) && ((__btInputProgram == TRUE) || __bMMJPEGAudioMode)) //CoCo2.55, don't allow KEY_NUMBER in JPEG+Audio mode.
        {
            extern BYTE bNumberTimeOut;
            __dwNumberPrevious = 0;
            bNumberTimeOut = FALSE;
            return KEY_INVALID;
        }

        if ((__bZoomFactor != VIEW_NORMAL) && (__bKey != KEY_PLAY) && (__bKey!=KEY_NUMBER))
        {
            __bZoomFactor = VIEW_NORMAL;
            HAL_Zoom (VIEW_NORMAL);
#ifdef SUPPORT_GRAPH_ZOOM
            OSD_OUTPUT_MACRO(MSG_ZOOM, (WORD)__bZoomFactor, 0x3);
#endif
            OSD_OUTPUT_MACRO(MSG_ZOOM_TEXT, (WORD)__bZoomFactor, 0x3);
        }

        if (__bAttrPlay & TYPE_CDROM_AUDIO)
        {
            if (((__btInputProgram == FALSE) || ((__btInputProgram == TRUE) && (__bKey == KEY_NEXT || __bKey == KEY_PREV))) && (__MMSelectCursor.bPosition == POS_FILE_LIST))
            {
                // wyc2.36-909S, fix when issue play in MP3 scan mode + program edit mode, then will cancel audio scan mode but OSD keep in scan mode.
                if ((__bKey != KEY_PLAY) || (__bMMMenuID == MM_MENU_NORMAL))
                {
                    __bScanLevel=0;
                    HAL_WriteAM(HAL_AM_FF_SPEED, __bScanLevel);
                }

                if ((__bKey == KEY_PLAY) && (__bMMMenuID == MM_MENU_EDITPROG)&&(__btPause))
                {
                    __bScanLevel=0;
                    HAL_WriteAM(HAL_AM_FF_SPEED, __bScanLevel);
                }
                break;
            }
        }

        // wyc0.87, fix the MP3 pause -> play will re-start playing problem.
        if ( (__bKey == KEY_PLAY)&&(__btPause) )
            return KEY_BYPASS;
        break;
    }


    // Do key action while non-Menu Mode
    if(__bMMMenuID == MM_MENU_NONE)
    {
        __bKey = MM_ProcessKeyInPlayMode(__bKey);
    }
    else
    {
        __bKey= MM_UpdateMenuInfo(__bKey);
    }

    if (__dwMMKeyToDo == TRUE)
    {
        __dwMMKeyToDo = FALSE;
        return KEY_TODO;
    }
    else
        return KEY_BYPASS;
}

//  ************************************************************************
//  Function    :   _MM_FilterKey
//  Description :   Filter the unsupport key base on playing mode
//  Argument    :   bKey, the desired key
//  Return      :   KEY_INVALD, un-support key
//                  bKey, the input key
//  Side Effect :
//  ************************************************************************
BYTE _MM_FilterKey(BYTE bKey)
{
#ifndef NO_MM_BUTTON
    if (__bMMPopMenuID != MM_POP_MENU_NONE)
    {
		return _MM_FilterPopMenuDialogKey(bKey);
    }
#endif //NO_MM_BUTTON
	
    // Step 1: the keys are un-support for all mode
    switch(bKey)
    {
    case    KEY_UP:
    case    KEY_DOWN:
    case    KEY_LEFT:
    case    KEY_RIGHT:
    case    KEY_PROGRAM:
		return bKey;
    case    KEY_FF:
    case    KEY_SF:
    case    KEY_LAN_STUDY:
    case    KEY_STEP:
    case    KEY_FREEZE:
    case    KEY_GOTOTIME:
    case    KEY_SPST_CTL:
        // wyc0.75, JPG slide show don't support these keys.
        if ((__bMMMenuID == MM_MENU_NONE) && (__bAttrPlay != ATTR_JPG))
        {
			return bKey;
        }
        else
            return INVALID_KEY;
    case    KEY_AST_CTL:
        if ((__bMMMenuID == MM_MENU_NONE) && (__bAttrPlay & TYPE_CDROM_AV) )
            return bKey;
        else
            return INVALID_KEY;
		
		// LLY2.36, support repeat A-B action for CDROM/ DVDROM title's all file except JPEG file
    case    KEY_REPEAT_AB:
        if(__bAttrPlay == ATTR_JPG)
        {
            return INVALID_KEY;
        }
        else
        {
            return bKey;
        }
		
    case    KEY_BOOKMARK:
    case    KEY_DIGEST:
    case    KEY_TITLE:
        return INVALID_KEY;
    case    KEY_PAUSE:
        break;
    default:
        break;
    }
	
    // Step 2: Filter un-support key base on menu mode on/off -- LLY.276p-4
    if(__bMMMenuID==MM_MENU_NONE)  // menu mode off
    {
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        if (__bMMJPEGAudioMode)
        {
            switch (bKey)
            {
            case KEY_RANDOM:
                return INVALID_KEY;
            default:
                break;
            }
        }
#endif //SUPPORT_JPEG_AUDIO_PLAYING
		
        if(__bAttrPlay==ATTR_JPG)
        {
            switch(bKey)
            {
            case    KEY_AUDIO_LR:
                return KEY_INVALID;
            case    KEY_RETURN:
                if (__wDiscType & CDROM_PICTURECD)
                    return KEY_INVALID;
            default:
                break;
            }
        }
    }
    else  // menu mode on
    {
        switch(bKey)
        {
        case    KEY_DISPLAY:
        case    KEY_ZOOM:
            return  KEY_INVALID;
        case    KEY_RANDOM:
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
                return INVALID_KEY;
#endif //SUPPORT_JPEG_AUDIO_PLAYING
			
            if ((__MMSelectCursor.wIndex < __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir) || (__MMSelectCursor.bPosition != POS_FILE_LIST))
            {
                __btRandom = FALSE;
                return INVALID_KEY;
            }
			
        default:
            break;
        }

        if ((__btInputProgram) && (bKey >= KEY_N0 && bKey <= KEY_N9))
			return INVALID_KEY;
    }
	
    // Return the input key while the key can be supported
    return  bKey;
}
void MM_ConfigMMInitMenu(BYTE bMMMenu)
{
    __bMMInitMenu = bMMMenu;
}

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
BYTE MM_EnterCopyDeleteMode(void)
{
    if ((__bAttrPlay == ATTR_JPG) && (__bMMStage == MM_STAGE_POLLING_TIMEOUT) && (__bMMJPGEncodeNum < SRCFTR_SUPPORT_JPG_ENCODE_NUM))
    {
        if (__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI)
        {            
            __bMMSlideShowCopyDeleteMode = MM_SLIDE_SHOW_DELETE_MODE;
            OSD_ChangeUI(OSD_UI_COPY_DELETE_DLG, OSD_ENTER_UI);
            OSDMM_ShowDeleteDlg();             
        }
        else
        {
            //copy mode            
            __bMMSlideShowCopyDeleteMode = MM_SLIDE_SHOW_COPY_MODE;
            OSD_ChangeUI(OSD_UI_COPY_DELETE_DLG, OSD_ENTER_UI);
            OSDMM_ShowSaveDlg();
        }

        return TRUE;
    }
    else
	{
        return FALSE;
	}
}


#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
BYTE _MM_CopyDeleteMode_ProcessKey(void)
{
    BYTE bResult;

#ifdef CT950_STYLE
    RTC_TM   rtc_tm1;
    RTC_TM   rtc_tm2;
    AUTOPWR_TM poweroff_tm;
#endif //CT950_STYLE

    if (__bMMSlideShowCopyDeleteMode == MM_SLIDE_SHOW_COPY_MODE)
    {
        bResult = OSDMM_SaveDlgProcessKey(__bKey);
        if (bResult == OSDMM_DIALOG_BUTTON_ID_YES)
        {
            OSDMM_DeleteSaveDlg();

#ifdef BORROW_FRAME_BUFFER
            DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
#endif //BORROW_FRAME_BUFFER
            
            //Copy the photo to the flash.             
#ifdef CT950_STYLE
            RTC_GetTime (&rtc_tm1);
#endif //
            
            __bMMSaveProgress = 0;
            OSDMM_Update(OSDMM_MSG_SAVE,0);

            JPEGDEC_SlideShowEncodeMode(JPEG_SLIDESHOW_ENCODE);
         
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {
                UTL_StreamPause(0);
                UTL_StreamPause(BG_ITEM_STREAM_ID);
                
                UTL_StreamContinue(0);
                
                __bMMAudioStreamPause = TRUE;
                __bMMVideoStreamPause = TRUE;                
            }       
#endif //SUPPORT_JPEG_AUDIO_PLAYING
            
            OSDMM_Trigger(OSD_TRIGGER_CONTROL,3);
            MM_EncodeFile_Add(__MMPlayingItem.wDirID, __wPlayItem+__pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir-1);
            __bMMSaveProgress = 100;
            OSDMM_Trigger(OSD_TRIGGER_CONTROL,3);

            JPEGDEC_SlideShowEncodeMode(JPEG_SLIDESHOW_ENCODE_NONE);

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {               
                UTL_StreamContinue(BG_ITEM_STREAM_ID);                                
                __bMMAudioStreamPause = FALSE;
            }       
#endif //SUPPORT_JPEG_AUDIO_PLAYING

            
#ifdef CT950_STYLE
            if (__bEnableAutoPWROff)
            {
                AUTOPWR_GetOffTM ( &poweroff_tm );
                RTC_GetTime (&rtc_tm2);
                if ((60*rtc_tm1.bHr+rtc_tm1.bMin) <= (poweroff_tm.bHr*60+poweroff_tm.bMin) && 
                    (60*rtc_tm2.bHr+rtc_tm2.bMin) >= (poweroff_tm.bHr*60+poweroff_tm.bMin))
                {
                    CC_PWRCtrl();
                    __bAutoPWRState = AUTOPWR_NONE;
                }                                
            }
#endif //

            __bMMSlideShowCopyDeleteMode = MM_SLIDE_SHOW_NORMAL_MODE;
            OSD_ChangeUI(OSD_UI_COPY_DELETE_DLG, OSD_EXIT_UI);

#ifdef BORROW_FRAME_BUFFER
            DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
#endif //BORROW_FRAME_BUFFER

#ifdef SUPPORT_PHOTO_BROWSE_MODE
            if (!__bMMPhotoBrowseMode)
                __bFWKey=KEY_NEXT;
#endif //SUPPORT_PHOTO_BROWSE_MODE
        }
        else if (bResult == OSDMM_DIALOG_BUTTON_ID_NO)
        {
            //delete the save dialog and return to the normal slide show mode.
            OSDMM_DeleteSaveDlg();
            __bMMSlideShowCopyDeleteMode = MM_SLIDE_SHOW_NORMAL_MODE;
            OSD_ChangeUI(OSD_UI_COPY_DELETE_DLG, OSD_EXIT_UI);

#ifdef SUPPORT_PHOTO_BROWSE_MODE
            if (!__bMMPhotoBrowseMode)
                __bFWKey=KEY_NEXT;
#endif //SUPPORT_PHOTO_BROWSE_MODE
        }
    }
    else if (__bMMSlideShowCopyDeleteMode == MM_SLIDE_SHOW_DELETE_MODE)
    {
        bResult = OSDMM_DeleteDlgProcessKey(__bKey);
        if (bResult == OSDMM_DIALOG_BUTTON_ID_YES)
        {
            OSDMM_DeleteDeleteDlg();
            __bMMSlideShowCopyDeleteMode = MM_SLIDE_SHOW_NORMAL_MODE;

            //delete the photo from the flash.
            MM_EncodeFile_Delete(__wPlayItem-1);
            OSD_ChangeUI(OSD_UI_COPY_DELETE_DLG, OSD_EXIT_UI);

#ifdef BORROW_FRAME_BUFFER
            DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
#endif //BORROW_FRAME_BUFFER

#ifdef SUPPORT_PHOTO_BROWSE_MODE
            if (!__bMMPhotoBrowseMode)
                __bFWKey=KEY_NEXT;
#endif //SUPPORT_PHOTO_BROWSE_MODE
        }
        else if (bResult == OSDMM_DIALOG_BUTTON_ID_NO)
        {
            //delete the "delete dialog" and keep on slide show.
            OSDMM_DeleteDeleteDlg();
            __bMMSlideShowCopyDeleteMode = MM_SLIDE_SHOW_NORMAL_MODE;
            OSD_ChangeUI(OSD_UI_COPY_DELETE_DLG, OSD_EXIT_UI);

#ifdef SUPPORT_PHOTO_BROWSE_MODE
            if (!__bMMPhotoBrowseMode)
                __bFWKey=KEY_NEXT;
#endif //SUPPORT_PHOTO_BROWSE_MODE
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW

void MM_CopyDeleteMode_Exit(void)
{
    if (__bMMSlideShowCopyDeleteMode == MM_SLIDE_SHOW_COPY_MODE)
    {
        OSDMM_DeleteSaveDlg();
        OSD_ChangeUI(OSD_UI_COPY_DELETE_DLG, OSD_EXIT_UI);
    }
    else if (__bMMSlideShowCopyDeleteMode == MM_SLIDE_SHOW_DELETE_MODE)
    {
        OSDMM_DeleteDeleteDlg();
        OSD_ChangeUI(OSD_UI_COPY_DELETE_DLG, OSD_EXIT_UI);
    }
    
    __bMMSlideShowCopyDeleteMode = MM_SLIDE_SHOW_NORMAL_MODE;
}

#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW

void MM_UI_Initial(void)
{
    if (__wDiscType & CDROM_PICTURECD)
    {
        __MMSelectCursor.wDirID=1;
        __MMPlayingItem.wDirID=1;
    }
    else
    {
        // [1-1] active/ open directory @ 1st ID
        __MMSelectCursor.wDirID=0;
        // [1-2] playing directory @ 1st ID
        __MMPlayingItem.wDirID=0;
    }
	
    __MMSelectCursor.bPosition=POS_UNEXIST;
    __MMSelectCursor.wIndex=0xffff;
	
    __bProgramMaxEntry=0;
	
    _bMMNextSelectDir = 0;
    _bMMAcrossDirType = MM_ACROSS_DIR_NONE;
	
    // wyc1.20, initial this value to avoid sometimes file list don't show problem.
    __wMMNewCursorIndex = 0;
	
    __bMMMenuID=MM_MENU_NONE;
    __bMMPopMenuID = MM_POP_MENU_NONE;
	
    __dwMMKeyToDo = FALSE;
}

#ifdef CT950_STYLE
void _MM_PhotoFrame_Initial(BYTE bMenuMode)
{
    _MM_PrepareFilterFile();
	
    if ((!__bMMAutoPlay) && (__ImageFrameSetupInfo.bBrowse == SETUP_PHOTO_SETTING_BROWSE_THUMBNAIL) && (__bMMExtFilterMode == EXT_BIT_JPG) && (bMenuMode != MM_INITIAL_MENU_COPY)) 
    {
        if (MM_GetFirstPlayableFile())
        {
            if (bMenuMode == MM_INITIAL_MENU_COPY)
			{
                MM_InitialMenu(MM_MENU_ENCODE_FILE|MM_CLEAR_BACKGROUND, FALSE);
			}
            else if (bMenuMode == MM_INITIAL_MENU_NORMAL)
            {
                __bMMMenuID=MM_MENU_NONE;
                __MMPlayingItem.wDirID = 0;
                __MMPlayingItem.wIndex = 0;
                MM_EnterThumbMode();
            }
        }
        else
        {
            MEDIA_ExitUSBSRC();
            OSD_Output(MSG_NO_PLAYABLE_FILE, 0, 3);
            OS_DelayTime(COUNT_500_MSEC);
            POWERONMENU_Initial();
        }
    }
    else
    {
        // wyc2.38-909s, need to do prepare page when enter MM UI to get real filter in files & change custor to dir part when no files in this dir.
        // Otherwise will see wrong files if no files in dir & cursor put in file part.
        if (MM_GetFirstPlayableFile())
        {
            if (bMenuMode == MM_INITIAL_MENU_COPY)
            {
                MM_InitialMenu(MM_MENU_ENCODE_FILE|MM_CLEAR_BACKGROUND, FALSE);
                __bMMAutoPlay = FALSE;
            }
            else if (bMenuMode == MM_INITIAL_MENU_NORMAL)
            {
                if (!__bMMAutoPlay)
                {
                    MM_InitialMenu(MM_MENU_NORMAL|MM_CLEAR_BACKGROUND, FALSE);
                }
            }
            
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            if (__bMMJPEGAudioMode)
            {
                if (_MM_JPEGAudioPlayInitial())
                {
#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
                    __bMMPlayMediaDirectlyMode = FALSE;
                    __bMMStartJPEGAudioTrigger = FALSE;
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
                }
				else
				{
#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
					__bMMJPEGAudioMode = FALSE;
                    __bMMStartJPEGAudioTrigger = FALSE;
					__bMMPlayMediaDirectlyMode = FALSE;
                    
                    __bMMExtFilterMode = EXT_BIT_DAT|EXT_BIT_MPG|EXT_BIT_VOB|EXT_BIT_AVI;
					_MM_PrepareFilterFile();

					if (!MM_GetFirstPlayableFile())
					{
						MEDIA_ExitUSBSRC();
						OS_YieldThread();
						__bDiscInfo &= DISCINFO_UNPLAYABLE;						
						
						if (!MM_PlayPhotoInFlash())
						{
							POWERONMENU_Initial();
						} 
					}
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
				}
            }
#endif //SUPPORT_JPEG_AUDIO_PLAYING
            			
            if (!__bMMAutoPlay) 
                MM_JPEGPreview();
        }
        else
        {
#ifdef SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON			
            if (__bMMPlayMediaDirectlyMode == TRUE)
            {
				__bMMPlayMediaDirectlyMode = FALSE;
				__bMMJPEGAudioMode = FALSE;
				__bMMStartJPEGAudioTrigger = FALSE;
				
				__bMMExtFilterMode = EXT_BIT_DAT|EXT_BIT_MPG|EXT_BIT_VOB|EXT_BIT_AVI;
				_MM_PrepareFilterFile();
				
				if (!MM_GetFirstPlayableFile())
				{
					MEDIA_ExitUSBSRC();
					OS_YieldThread();
					__bDiscInfo &= DISCINFO_UNPLAYABLE;						
					
					if (!MM_PlayPhotoInFlash())
					{
						POWERONMENU_Initial();
					} 
				}				
            }
            else
#endif //SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON
            {                
                MEDIA_ExitUSBSRC();
                OSD_Output(MSG_NO_PLAYABLE_FILE, 0, 3);
                OS_DelayTime(COUNT_500_MSEC);
                POWERONMENU_Initial();
            }			
        }
    }
}
#endif //CT950_STYLE	

void _MM_AutoPlay_Initial(void)
{
#ifdef MM_AUTO_PLAY_DCIM_DIR_FIRST
	BYTE   _MMTempString[MM_MAX_DIRNAME_LENGTH+1];

	_dwMMTemp8 = FALSE;
	for (__MMSelectCursor.wDirID = 0; __MMSelectCursor.wDirID<__bMMTotalDirs; __MMSelectCursor.wDirID++)
	{
		strcpy(_MMTempString,__pMMDirNameList[__MMSelectCursor.wDirID].cName);
		COMUTL_StrUpr(_MMTempString);
		if (!strcmp(_MMTempString,"DCIM"))
		{
			for (_dwMMTemp9=__pMMVolumeDirTree[__MMSelectCursor.wDirID].b1stSubDirID; _dwMMTemp9<(__pMMVolumeDirTree[__MMSelectCursor.wDirID].b1stSubDirID+__pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir); _dwMMTemp9++) 
			{
				MM_PreparePage(MM_PREPARE_FILEPAGE,_dwMMTemp9, __bMMExtFilterMode, __bMMSortMethod);
				if (__pMMFilterFilesOfDir[_dwMMTemp9] != 0)
				{
					__MMSelectCursor.wDirID = _dwMMTemp9;
					_dwMMTemp8 = TRUE;
					break;
				}
			}
		}
		if (_dwMMTemp8 == TRUE)
			break;
	}

	if (_dwMMTemp8 == FALSE)
	{
		for (__MMSelectCursor.wDirID = 0; __MMSelectCursor.wDirID<__bMMTotalDirs; __MMSelectCursor.wDirID++)
		{
			MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
			if (__pMMFilterFilesOfDir[__MMSelectCursor.wDirID] != 0)
				break;
		}
	}
	__MMPlayingItem.wDirID = __MMSelectCursor.wDirID;
	__MMSelectCursor.wIndex = __pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir;
#else
	// wyc2.38a-909s, use __bMMTotalDirs to replace MM_MAX_DIR_NO to avoid getting wrong DRAM data when dir less than MM_MAX_DIR_NO.
	for (__MMSelectCursor.wDirID = 0; __MMSelectCursor.wDirID<__bMMTotalDirs; __MMSelectCursor.wDirID++)
	{
		MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
		if (__pMMFilterFilesOfDir[__MMSelectCursor.wDirID] != 0)
			break;
	}
	__MMPlayingItem.wDirID = __MMSelectCursor.wDirID;
	__MMSelectCursor.wIndex = __pMMVolumeDirTree[__MMPlayingItem.wDirID].bNoOfSubDir;
#endif //MM_AUTO_PLAY_DCIM_DIR_FIRST
}

void MM_NumberActionInUIMode(void)
{
	HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_CURRENTFRAME);
	
	//CoCo0.80, Recover the flashing file and reset the audio spectrum and time.
	if (__btPlaying)
	{
		OSDMM_Update(OSDMM_MSG_STOP, 0);
	}
	OSDMM_Update(OSDMM_MSG_CLEAR_HIGHLIGHT, 0); 
	
	__MMSelectCursor.wIndex = __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir +__wPlayItem-1;
	__MMSelectCursor.bPosition = POS_FILE_LIST;
	
	_MM_ChangePlayingCursorToSelectCursor(); 
	
	OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0); 
	OSDMM_Update(OSDMM_MSG_PLAYING, 0); 
	//winnie 2.70, added code to fix Button color abnormal when KEY_NUMBER to play MP3 from stop mode
#ifndef NO_MM_BUTTON
	if (__bMMMenuID == MM_MENU_NORMAL)
	{
		OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_FILTER);
		OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_SORT);
	}                 
#endif 
}

#ifndef NO_MM_BUTTON
void _MM_ProcessProgramListBtn(void)
{
	if (__bProgramMaxEntry == 0)
	{
		__MMSelectCursor.bPosition=POS_BUTTON_LIST;
		__MMSelectCursor.wIndex=MM_BTN_RETURN;
	}
	else
	{
		__MMSelectCursor.bPosition=POS_PROG_LIST;
		__MMSelectCursor.wIndex=0x0;
	}
				
	// Draw the program list menu
	MM_InitialMenu(MM_MENU_PROGLIST|MM_CLEAR_BACKGROUND, FALSE);
	if (__bProgramMaxEntry == 0)
	{
		OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_DELETE_ALL);
	}
	else
	{
		OSDMM_Update(OSDMM_MSG_ENABLE_BUTTON, MM_BTN_DELETE_ALL);
	}

	MM_JPEGPreview();
}
#endif //NO_MM_BUTTON

#ifndef NO_MM_BUTTON
void _MM_ProcessProgramListDeleteAllBtn(void)
{
	// Reset current program index as zero
	__bProgramCurrentIndex=0x0;

	// Reset max program entry as zero
	__bProgramMaxEntry=0x0;

	// Update final selection cursor @ "Return" button
	__MMSelectCursor.wIndex = MM_BTN_RETURN;

	//Clear Program data
	LINEAR_Program(LINEAR_PROGRAM_CLEAR, (BYTE)NULL);

	// Clear all still in program mode. So can't to set __btProgram to 0. __btInputProgram still can't set to 0.
	__btProgram = TRUE;
	__btInputProgram = TRUE;
	OSDMM_Update(OSDMM_MSG_PROGRAM_DELETE_ALL,0);
	__MMSelectCursor.wIndex = 1;
	OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
	OSDMM_Update(OSDMM_MSG_DISABLE_BUTTON, MM_BTN_DELETE_ALL);
}
#endif //NO_MM_BUTTON

#ifndef NO_MM_BUTTON
void _MM_ReturnToEditProgramMode(void)
{
	// Prepare the final selection cursor index @ 1st file of current active directory
	__MMSelectCursor.wIndex = MM_BTN_PROG_LIST;
				
	MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
	__bMMMenuID = MM_MENU_EDITPROG; //Should identify the correct menu ID to draw the title string.
	OSDMM_Update(OSDMM_MSG_CHANGE_MODE, OSDMM_CHANGE_MODE_PROGRAMLIST_TO_PROGRAM); //Need to use this "change mode".
	OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
}
#endif //NO_MM_BUTTON

#ifndef NO_MM_BUTTON
void _MM_ExitPopMenuDialog(void)
{
	_MM_PrepareFilterFile();
	
	OSDMM_Update(OSDMM_MSG_CLEAR_DLG, 0);
	if (__bMMPopMenuID == MM_POP_MENU_FILTER)
		__MMSelectCursor.wIndex = MM_BTN_FILTER;
	else
		__MMSelectCursor.wIndex = MM_BTN_SORT;
	__bMMPopMenuID = MM_POP_MENU_NONE;
	// wyc2.17-909S, need to re-draw menu when re-prepare the file page data. Otherwise the menu will wrong when filter out files in
	// not 1st pages.
	OSDMM_Update(OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL, 0);
	OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT, 0);
}
#endif //NO_MM_BUTTON

#ifndef NO_MM_BUTTON
BYTE _MM_FilterPopMenuDialogKey(BYTE bKey)
{
	switch(bKey)
	{
	case    KEY_UP:
	case    KEY_DOWN:
	case    KEY_LEFT:
	case    KEY_RIGHT:
	case    KEY_PLAY:
	case    KEY_POWER:
	case    KEY_OPEN_CLOSE:
		return bKey;
	default:
		return INVALID_KEY;
	}
}
#endif //NO_MM_BUTTON

#ifdef SUPPORT_COPY_FILE_IN_MM_UI
void _MM_ProcessPhotoSave(void)
{
#ifdef CT950_STYLE
    RTC_TM   rtc_tm1;
    RTC_TM   rtc_tm2;
    AUTOPWR_TM poweroff_tm;

	WORD wMMDir, wMMIndex;

	RTC_GetTime (&rtc_tm1);
	
	wMMDir = __MMSelectCursor.wDirID;
	wMMIndex = __MMSelectCursor.wIndex;
	__bMMSaveProgress = 0;
	OSDMM_Update(OSDMM_MSG_SAVE,0);
	for (_dwMMTemp9=0; _dwMMTemp9<__bProgramMaxEntry; _dwMMTemp9++) 
	{
		__bMMSaveProgress = (100/__bProgramMaxEntry)*_dwMMTemp9;
		OSDMM_Trigger(OSD_TRIGGER_CONTROL,3);
		MM_EncodeFile_Add((WORD)__ProgIdxList[_dwMMTemp9].bMainIndex,__ProgIdxList[_dwMMTemp9].wSubIndex-1);
	}

	HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_FRAME0);
	HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_JPEG, HAL_DISP_FRAME1);
	LINEAR_Program(LINEAR_PROGRAM_CLEAR,0);

	__bMMSaveProgress = 100;
	OSDMM_Trigger(OSD_TRIGGER_CONTROL,3);
	__MMSelectCursor.wDirID = wMMDir;
	__MMSelectCursor.wIndex = wMMIndex;
	OSDMM_Update(OSDMM_MSG_UPDATE_HIGHLIGHT,0);
	
	if (__bEnableAutoPWROff)
	{
		AUTOPWR_GetOffTM (&poweroff_tm);
		RTC_GetTime(&rtc_tm2);
		if ((60*rtc_tm1.bHr+rtc_tm1.bMin) <= (poweroff_tm.bHr*60+poweroff_tm.bMin) && 
			(60*rtc_tm2.bHr+rtc_tm2.bMin) >= (poweroff_tm.bHr*60+poweroff_tm.bMin))
		{
			CC_PWRCtrl();
			__bAutoPWRState = AUTOPWR_NONE;
		}                                
	}
#endif //
}
#endif //SUPPORT_COPY_FILE_IN_MM_UI

