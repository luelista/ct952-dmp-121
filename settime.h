#ifndef __SETTIME_H__
#define __SETTIME_H__

#include "dialog.h"

#define MAX_SUPPORT_SETTIME_HANDLE_NUM                              3
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "mainmenu.h"

#define SET_TIME_DIALOG_WIDTH                                        240
#define SET_TIME_DIALOG_HEIGHT                                       160
#ifdef NL_SOLUTION
#define SET_TIME_DIALOG_BUTTON_NUM                                   0
#else
#define SET_TIME_DIALOG_BUTTON_NUM                                   2
#endif //NL_SOLUTION

#define SET_TIME_DIALOG_BUTTON_ID_OK                                 0
#define SET_TIME_DIALOG_BUTTON_ID_CANCEL                             1

#define SET_TIME_DIALOG_EDIT_TEXT_MAX_WIDTH                          120
#define SET_TIME_DIALOG_EDIT_SET_TIME_DIGIT_NUM                      4
#define SET_TIME_DIALOG_EDIT_HEIGHT                                  26

#define SET_TIME_DIALOG_EDIT_TEXT_DLG_DISTANCE_H                     12

#ifdef NL_SOLUTION
#define SET_TIME_DIALOG_EDIT_TEXT_DLG_TITLE_DISTANCE_V               40   
#else
#define SET_TIME_DIALOG_EDIT_TEXT_DLG_TITLE_DISTANCE_V               20   
#endif //NL_SOLUTION

#define SET_TIME_DIALOG_EDIT_TEXT_EDIT_DISTANCE_H                    8

//define the position of the edit box
#define SET_TIME_DIALOG_EDIT_START_X                                 ((MAINMENU_CONTENT_REGION_START_X+((MAINMENU_CONTENT_REGION_WIDTH-SET_TIME_DIALOG_WIDTH)>>1))+SET_TIME_DIALOG_EDIT_TEXT_DLG_DISTANCE_H+SET_TIME_DIALOG_EDIT_TEXT_MAX_WIDTH)
#define SET_TIME_DIALOG_EDIT_START_Y                                 ((MAINMENU_CONTENT_REGION_START_Y+((MAINMENU_CONTENT_REGION_HEIGHT-SET_TIME_DIALOG_HEIGHT)>>1))+DIALOG_TITLE_HEIGHT+DIALOG_SEPARATOR_HEIGHT+SET_TIME_DIALOG_EDIT_TEXT_DLG_TITLE_DISTANCE_V)

#define SET_TIME_DIALOG_CURSOR_POS_HOUR                              0
#define SET_TIME_DIALOG_CURSOR_POS_MINUTE                            1  
#define SET_TIME_DIALOG_CURSOR_POS_SECOND                            2          
#define SET_TIME_DIALOG_CURSOR_POS_BUTTON                            3

//Define the return status for _SETTIME_EDIT_ProcessKey
#define SETTIME_EDIT_ACTION_DONE					                0
#define SETTIME_EDIT_ACTION_EXCEED_LEFT					            1
#define SETTIME_EDIT_ACTION_EXCEED_RIGHT					        2
#define SETTIME_EDIT_ACTION_EXCEED_ENTER                            3

//Initialize palette
#define SET_TIME_DIALOG_PALETTE_ENTRY_BEGIN                          GDI_GENERAL_UI_PALETTE_INDEX_START//70
#define SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_TEXT                      SET_TIME_DIALOG_PALETTE_ENTRY_BEGIN
#define SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_INFO                      (SET_TIME_DIALOG_PALETTE_ENTRY_BEGIN+1)
#define SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_HIGHLIGHT                 (SET_TIME_DIALOG_PALETTE_ENTRY_BEGIN+2)

#define SET_TIME_DIALOG_PALETTE_COLOR_EDIT_INFO                      0xFFFFFF

#define SET_TIME_DIALOG_PALETTE_COLOR_EDIT_TEXT_1                    0x423789
#define SET_TIME_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_1               0xFFD900

#define SET_TIME_DIALOG_PALETTE_COLOR_EDIT_TEXT_2                    0x860706
#define SET_TIME_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_2               0xCA8838

#define SET_TIME_DIALOG_PALETTE_COLOR_EDIT_TEXT_3                    0x2A2694
#define SET_TIME_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_3               0xD38A34

#define SET_TIME_DIALOG_PALETTE_COLOR_EDIT_TEXT_4                    0x546791
#define SET_TIME_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_4               0xD18B26

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef SET_TIME_MAIN_FILE
#include "OSDString\\strextrn.h"

DLG_BUTTON _SETTIMEDialogButton[SET_TIME_DIALOG_BUTTON_NUM] = {
    {aBUTTONOK, SET_TIME_DIALOG_BUTTON_ID_OK, DIALOG_BUTTON_START_X_OPTIMIZED},
    {aBUTTONCancel, SET_TIME_DIALOG_BUTTON_ID_CANCEL, DIALOG_BUTTON_START_X_OPTIMIZED},
};

DIALOG _SETTIMEDlg = {
    (MAINMENU_CONTENT_REGION_START_X+((MAINMENU_CONTENT_REGION_WIDTH-SET_TIME_DIALOG_WIDTH)>>1)),
    (MAINMENU_CONTENT_REGION_START_Y+((MAINMENU_CONTENT_REGION_HEIGHT-SET_TIME_DIALOG_HEIGHT)>>1)),
    SET_TIME_DIALOG_WIDTH,
    SET_TIME_DIALOG_HEIGHT,
    aSETTIMESetTime,
    DIALOG_TITLE_STRING_ALIGNMENT_CENTER,
    SET_TIME_DIALOG_BUTTON_NUM,
    SET_TIME_DIALOG_BUTTON_ID_CANCEL,
    DIALOG_BUTTON_WIDTH_OPTIMIZED,
    DIALOG_BUTTON_START_Y_OPTIMIZED,
#ifdef NL_SOLUTION
    NULL,
#else
    _SETTIMEDialogButton,
#endif //NL_SOLUTION
};

#endif //#ifdef SET_TIME_MAIN_FILE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define invlide MENU HANDLE
#define SETTIME_HANDLE_INVALID              0xFF

#define SETTIME_HANDLE_FREE                 0xFE

//define the handle of the settime
typedef BYTE HSETTIME;

typedef struct tagSETTIME_DLG_ACTION
{
    void (*SETTIME_OK_Func_Ptr)(BYTE, BYTE, BYTE);
    void (*SETTIME_CANCEL_Func_Ptr)(void);
} SETTIME_DLG_ACTION, *PSETTIME_DLG_ACTION;

//define the display mode
#define SETTIME_DISPLAY_MODE_HOUR_MINUTE            0
#define SETTIME_DISPLAY_MODE_HOUR_MINUTE_SEC        1

//define the time mode
#define SETTIME_DISPLAY_TIME_HOUR                   0
#define SETTIME_DISPLAY_TIME_MINUTE                 1
#define SETTIME_DISPLAY_TIME_SECOND                 2

//define the attribute of the number for displaying time
#define SETTIME_NUM_WIDTH                           12
#define SETTIME_NUM_COLON_WIDTH                     8

#define SETTIME_DISPLAY_MODE_HOUR_MINUTE_WIEDH      (SETTIME_NUM_WIDTH*4+SETTIME_NUM_COLON_WIDTH*1)
#define SETTIME_DISPLAY_MODE_HOUR_MINUTE_SEC_WIDTH  (SETTIME_NUM_WIDTH*6+SETTIME_NUM_COLON_WIDTH*2)

typedef struct tagSETTIME_TIME
{
    BYTE bDisplayType;
    BYTE bHour;
    BYTE bMinute;
    BYTE bSecond;
    WORD wStartX;
    WORD wStartY;
} SETTIME_TIME, *PSETTIME_TIME;

typedef struct tagSETTIME
{
    SETTIME_TIME DisplayTime;
    PSETTIME_DLG_ACTION pSetTimeDlgAction;
} SETTIME, *PSETTIME;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Export APIs
HSETTIME SETTIME_Create(PSETTIME_TIME pDisplayTime, PSETTIME_DLG_ACTION pSetTimeAction);
void SETTIME_Show(HSETTIME hSetTime, BYTE bBackgroundColor, BYTE bUIStyle);
void SETTIME_ProcessKey(HSETTIME hSetTime, BYTE bKey);
void SETTIME_Recover(HSETTIME hSetTime);
void SETTIME_Exit(HSETTIME hSetTime, BYTE bClearBackground);

//Internal APIs
HSETTIME _SETTIME_GetSetTimeHandle(void);
void _SETTIME_InitializeGDI(BYTE bUIStyle);
BYTE _SETTIME_EDIT_ProcessKey(HSETTIME hSetTime, BYTE bKey);
void _SETTIME_UpdateCursor(HSETTIME hSetTime, BYTE bClear);
void _SETTIME_ShowTime(HSETTIME hSetTime);
void _SETTIME_ShowColon(HSETTIME hSetTime, BYTE bPos);
void _SETTIME_ShowTimeNum(HSETTIME hSetTime, WORD wNum, WORD wStartX, BYTE bCursor);
void _SETTIME_DrawAllUI(HSETTIME hSetTime);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





















#endif //__SETTIME_H__