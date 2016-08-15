/**************************************************************************

        (C)Copyright Cheertek Inc. 2007,
           J200, all right reserved

        Product : DVD/DMP Firmware

        Date    : 2007.4.10
        Author  : Cheertek (J200 WYC/CoCo)
        Purpose : The UI of media manager.
        Sources : mm_ui.h

****************************************************************************/
#ifndef __MM_UI_H__
#define __MM_UI_H__

#ifdef SUPPORT_POWERON_MENU //CoCo2.38
#define NO_MM_BUTTON
#endif //#ifdef SUPPORT_POWERON_MENU //CoCo2.38
#ifdef NO_MM_BUTTON
#undef SUPPORT_FM_PROGRAM
#endif //

////////////////////////////////////////////////////////////////
#define MM_MSG_PROGRESS_DIV_NUM     (10)

////////////////////////////////////////////////////////////////
// The max control button number
#define MM_MAX_CTL_BTN_NO				2

////////////////////////////////////////////////////////////////
// Use for MM_Action() return value -- LLY0.83b-2
#define MM_ACTION_DONE          1
#define MM_ACTION_CONTINUE      2
#define MM_ACTION_INVALID       3

////////////////////////////////////////////////////////////////
// Use for _FM_InitialMenu() parameter -- LLY0.86
// Only use bit[7] to specify if clear background while draw File Manager UI
// Notice: bit[6:0] is same as __bMMMenuID
#define MM_CLEAR_BACKGROUND     0x80

////////////////////////////////////////////////////////////////
// wyc1.05, supporting two types of across dir boundary.
// Type of across dir boundary.
#define MM_ACROSS_DIR_NONE          0x0
#define MM_NEXT_PREV_ACROSS_DIR     0x1
#define MM_UP_DOWN_ACROSS_DIR       0x2

////////////////////////////////////////////////////////////////
#define MM_MENU_NONE            0
#define MM_MENU_NORMAL          1
#define MM_MENU_EDITPROG        2
#define MM_MENU_PROGLIST        3
#define MM_MENU_ENCODE_FILE     4

////////////////////////////////////////////////////////////////
#define MM_POP_MENU_NONE        0
#define MM_POP_MENU_SORT        1
#define MM_POP_MENU_FILTER      2

////////////////////////////////////////////////////////////////
#define POS_VOL_DIR_LIST        0
#define POS_FILE_LIST           1
#define POS_BUTTON_LIST         2
#define POS_PROG_LIST           3
#define POS_DLG_ITEM            4
#define POS_UNEXIST             5

////////////////////////////////////////////////////////////////
#define MM_BTN_FILTER			0
#define MM_BTN_SORT				1

#define MM_BTN_PROG_LIST		0
#define MM_BTN_PLAY				1

#define MM_BTN_DELETE_ALL       0
#define MM_BTN_RETURN			1

// wyc2.51-909P, button for JPG copy mode.
#define MM_BTN_SAVE				0
#define MM_BTN_EXIT				1

////////////////////////////////////////////////////////////////
#define MM_RETURN_NONE			0
#define MM_RETURN_TRUE			1	
#define MM_RETURN_FALSE			2
#define MM_RETURN_KEY			3
#define MM_RETURN_INVALID		4

////////////////////////////////////////////////////////////////
// wyc2.51-909P, the initial menu for normal and copy mode.
#define MM_INITIAL_MENU_NORMAL  0
#define MM_INITIAL_MENU_COPY    1

////////////////////////////////////////////////////////////////
//CoCo2.61, add the parameter for slide show copy/delete mode.
#define MM_SLIDE_SHOW_NORMAL_MODE       0
#define MM_SLIDE_SHOW_COPY_MODE         1
#define MM_SLIDE_SHOW_DELETE_MODE       2

////////////////////////////////////////////////////////////////
// The value of "bButton" element
#define CTL_BTN_NEXT					1      // "Next" button
#define CTL_BTN_PREV					2      // "Previous" button
#define CTL_BTN_PROG					3      // "Program" button
#define CTL_BTN_THUMB					4      // "Thumbnail" button
#define CTL_BTN_RETURN					5      // "Return" button
#define CTL_BTN_PROG_LIST				6      // "Program List" button
#define CTL_BTN_CLEAR_ALL				7      // "Clear All" button
#define CTL_BTN_PLAY					8      // Start the playing action
#define CTL_BTN_END						9      // The end of control button

// The value of "bStatus" element
#define BTN_DISABLE						0x0     // button is disabled
#define BTN_ENABLE						0x1     // button is enabled

////////////////////////////////////////////////////////////////
typedef struct tagCURSOR_INFO
{
    BYTE    bPosition; 
    BYTE    bVolID;
    WORD    wDirID;
    WORD    wIndex;
}   CURSOR_INFO, * PCURSOR_INFO;

typedef struct tagCTL_BUTTON
{
    BYTE    bButtonID;    // keep control button ID
    BYTE    bStatus;    // disable/ enable status of the control button
}   CTL_BUTTON, * PCTL_BUTTON;

////////////////////////////////////////////////////////////////
//Extern variables
extern BYTE __bMMMenuID;
extern BYTE __bMMInitMenu;
extern CURSOR_INFO __MMPlayingItem;
extern CURSOR_INFO __MMSelectCursor;
extern WORD __wMMNewCursorIndex;
extern CTL_BUTTON __MMCtlBTNList[MM_MAX_CTL_BTN_NO];
extern BYTE	__bMMPopMenuID;

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
extern BYTE __bMMSlideShowCopyDeleteMode;
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW

////////////////////////////////////////////////////////////////
//Internal functions
BYTE _MM_ButtonAction(void);
void _MM_SimulateChangeDir(void);
void _MM_ChangePlayingCursorToSelectCursor(void);
BYTE _MM_SettingAndPlayItem(void);
void _MM_SetCursorToActiveDirPos(void);
BYTE _MM_FilterKey(BYTE bKey);
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
void _MM_ProcessPhotoSave(void);
#endif //SUPPORT_COPY_FILE_IN_MM_UI
#ifndef NO_MM_BUTTON
void _MM_ProcessProgramListBtn(void);
void _MM_ProcessProgramListDeleteAllBtn(void);
void _MM_ReturnToEditProgramMode(void);
void _MM_ExitPopMenuDialog(void);
BYTE _MM_FilterPopMenuDialogKey(BYTE bKey);
#endif //NO_MM_BUTTON

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
BYTE _MM_CopyDeleteMode_ProcessKey(void);
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW

void _MM_AutoPlay_Initial(void);
#ifdef CT950_STYLE
void _MM_PhotoFrame_Initial(BYTE bMenuMode);
#endif //CT950_STYLE
////////////////////////////////////////////////////////////////
//External functions
void MM_ProgramListToNormal(void);
void MM_InitialMenu(BYTE bMode, BYTE bRecover);
void MM_Initial(BYTE bMenuMode);
void MM_PlayItem(WORD wTrackNO);
void MM_Exit(BYTE bExitMode);
BYTE MM_SelectCursorPlayingItemEqual(void);
void MM_DumpOneFileForHighlightItem(void);
void MM_ThumbToMMEntry(void);
BYTE MM_ProcessKey(void);
void MM_ConfigMMInitMenu(BYTE bMMMenu);
BYTE MM_ProcessKeyInPlayMode(BYTE bKey);
BYTE MM_UpdateMenuInfo(BYTE bKey);
void MM_NumberActionInUIMode(void);

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
BYTE MM_EnterCopyDeleteMode(void);
void MM_CopyDeleteMode_Exit(void);
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW

void MM_UI_Initial(void);
////////////////////////////////////////////////////////////////


#endif //__MM_UI_H__
