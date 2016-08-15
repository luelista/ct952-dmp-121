#ifndef __SETDATE_H__
#define __SETDATE_H__

#include "dialog.h"

#define MAX_SUPPORT_SETDATE_HANDLE_NUM                              3
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "mainmenu.h"

#define SET_DATE_DIALOG_WIDTH                                        240
#define SET_DATE_DIALOG_HEIGHT                                       160

#ifdef NL_SOLUTION
#define SET_DATE_DIALOG_BUTTON_NUM                                   0
#else
#define SET_DATE_DIALOG_BUTTON_NUM                                   2
#endif //NL_SOLUTION

#define SET_DATE_DIALOG_BUTTON_ID_OK                                 0
#define SET_DATE_DIALOG_BUTTON_ID_CANCEL                             1

#define SET_DATE_DIALOG_EDIT_TEXT_MAX_WIDTH                          120
#define SET_DATE_DIALOG_EDIT_SET_DATE_DIGIT_NUM                      4
#define SET_DATE_DIALOG_EDIT_HEIGHT                                  26

#define SET_DATE_DIALOG_EDIT_TEXT_DLG_DISTANCE_H                     12
#define SET_DATE_DIALOG_EDIT_TEXT_DLG_TITLE_DISTANCE_V               20   

#define SET_DATE_DIALOG_EDIT_TEXT_EDIT_DISTANCE_H                    8

#define SETDATE_MAX_YEAR                                            2050
#define SETDATE_MIN_YEAR                                            2007

//define the position of the edit box
#define SET_DATE_DIALOG_EDIT_START_X                                 ((MAINMENU_CONTENT_REGION_START_X+((MAINMENU_CONTENT_REGION_WIDTH-SET_DATE_DIALOG_WIDTH)>>1))+SET_DATE_DIALOG_EDIT_TEXT_DLG_DISTANCE_H+SET_DATE_DIALOG_EDIT_TEXT_MAX_WIDTH)
#define SET_DATE_DIALOG_EDIT_START_Y                                 ((MAINMENU_CONTENT_REGION_START_Y+((MAINMENU_CONTENT_REGION_HEIGHT-SET_DATE_DIALOG_HEIGHT)>>1))+DIALOG_TITLE_HEIGHT+DIALOG_SEPARATOR_HEIGHT+SET_DATE_DIALOG_EDIT_TEXT_DLG_TITLE_DISTANCE_V)

#define SET_DATE_DIALOG_CURSOR_POS_YEAR                              0
#define SET_DATE_DIALOG_CURSOR_POS_MONTH                             1  
#define SET_DATE_DIALOG_CURSOR_POS_DATE                              2          
#define SET_DATE_DIALOG_CURSOR_POS_BUTTON                            3

//Define the return status for _SETDATE_EDIT_ProcessKey
#define SETDATE_EDIT_ACTION_DONE					                0
#define SETDATE_EDIT_ACTION_EXCEED_LEFT					            1
#define SETDATE_EDIT_ACTION_EXCEED_RIGHT					        2
#define SETDATE_EDIT_ACTION_EXCEED_ENTER                            3

//Initialize palette
#define SET_DATE_DIALOG_PALETTE_ENTRY_BEGIN                          GDI_GENERAL_UI_PALETTE_INDEX_START//70
#define SET_DATE_DIALOG_PALETTE_ENTRY_EDIT_TEXT                      SET_DATE_DIALOG_PALETTE_ENTRY_BEGIN
#define SET_DATE_DIALOG_PALETTE_ENTRY_EDIT_INFO                      (SET_DATE_DIALOG_PALETTE_ENTRY_BEGIN+1)
#define SET_DATE_DIALOG_PALETTE_ENTRY_EDIT_HIGHLIGHT                 (SET_DATE_DIALOG_PALETTE_ENTRY_BEGIN+2)

#define SET_DATE_DIALOG_PALETTE_COLOR_EDIT_INFO                      0xFFFFFF

#define SET_DATE_DIALOG_PALETTE_COLOR_EDIT_TEXT_1                    0x423789
#define SET_DATE_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_1               0xFFD900

#define SET_DATE_DIALOG_PALETTE_COLOR_EDIT_TEXT_2                    0x860706
#define SET_DATE_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_2               0xCA8838

#define SET_DATE_DIALOG_PALETTE_COLOR_EDIT_TEXT_3                    0x2A2694
#define SET_DATE_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_3               0xD38A34

#define SET_DATE_DIALOG_PALETTE_COLOR_EDIT_TEXT_4                    0x546791
#define SET_DATE_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_4               0xD18B26

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef SET_DATE_MAIN_FILE
#include "OSDString\\strextrn.h"

DLG_BUTTON _SETDATEDialogButton[SET_DATE_DIALOG_BUTTON_NUM] = {
    {aBUTTONOK, SET_DATE_DIALOG_BUTTON_ID_OK, DIALOG_BUTTON_START_X_OPTIMIZED},
    {aBUTTONCancel, SET_DATE_DIALOG_BUTTON_ID_CANCEL, DIALOG_BUTTON_START_X_OPTIMIZED},
};

DIALOG _SETDATEDlg = {
    (MAINMENU_CONTENT_REGION_START_X+((MAINMENU_CONTENT_REGION_WIDTH-SET_DATE_DIALOG_WIDTH)>>1)),
    (MAINMENU_CONTENT_REGION_START_Y+((MAINMENU_CONTENT_REGION_HEIGHT-SET_DATE_DIALOG_HEIGHT)>>1)),
    SET_DATE_DIALOG_WIDTH,
    SET_DATE_DIALOG_HEIGHT,
    aSETDATESetDate,
    DIALOG_TITLE_STRING_ALIGNMENT_CENTER,
    SET_DATE_DIALOG_BUTTON_NUM,
    SET_DATE_DIALOG_BUTTON_ID_CANCEL,
    DIALOG_BUTTON_WIDTH_OPTIMIZED,
    DIALOG_BUTTON_START_Y_OPTIMIZED,
#ifdef NL_SOLUTION
    NULL,
#else
    _SETDATEDialogButton,
#endif //NL_SOLUTION
};

#endif //#ifdef SET_DATE_MAIN_FILE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//define invlide MENU HANDLE
#define SETDATE_HANDLE_INVALID                          0xFF

#define SETDATE_HANDLE_FREE                             0xFE

//define the handle of the settime
typedef BYTE HSETDATE;

typedef struct tagSETDATE_DLG_ACTION
{
    void (*SETDATE_OK_Func_Ptr)(WORD, BYTE, BYTE);
    void (*SETDATE_CANCEL_Func_Ptr)(void);
} SETDATE_DLG_ACTION, *PSETDATE_DLG_ACTION;

//define the time mode
#define SETDATE_DISPLAY_TIME_YEAR                       0
#define SETDATE_DISPLAY_TIME_MONTH                      1
#define SETDATE_DISPLAY_TIME_DATE                       2

//define the attribute of the number for displaying time
#define SETDATE_NUM_WIDTH                               12
#define SETDATE_NUM_SEPARATE_WIDTH                      8

#define SETDATE_DISPLAY_WIDTH                           (SETDATE_NUM_WIDTH*8+SETDATE_NUM_SEPARATE_WIDTH*2)

typedef struct tagSETDATE_DATE
{
    WORD wYear;
    BYTE bMonth;
    BYTE bDate;
    WORD wStartX;
    WORD wStartY;
} SETDATE_DATE, *PSETDATE_DATE;

typedef struct tagSETDATE
{
    SETDATE_DATE DisplayDate;
    PSETDATE_DLG_ACTION pSetDateDlgAction;
} SETDATE, *PSETDATE;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Export APIs
HSETDATE SETDATE_Create(PSETDATE_DATE pDisplayDate, PSETDATE_DLG_ACTION pSetDateAction);
void SETDATE_Show(HSETDATE hSetDate, BYTE bBackgroundColor, BYTE bUIStyle);
void SETDATE_ProcessKey(HSETDATE hSetDate, BYTE bKey);
void SETDATE_Recover(HSETDATE hSetDate);
void SETDATE_Exit(HSETDATE hSetDate, BYTE bClearBackground);

//Internal APIs
HSETDATE _SETDATE_GetSetDateHandle(void);
void _SETDATE_DrawAllUI(HSETDATE hSetDate);
void _SETDATE_InitializeGDI(BYTE bUIStyle);
BYTE _SETDATE_EDIT_ProcessKey(HSETDATE hSetDate, BYTE bKey);
void _SETDATE_UpdateCursor(HSETDATE hSetDate, BYTE bClear);
void _SETDATE_ShowDate(HSETDATE hSetDate);
void _SETDATE_ShowSeparate(HSETDATE hSetDate, BYTE bPos);
void _SETDATE_ShowNum(HSETDATE hSetDate, WORD wNum, WORD wStartX, BYTE bCursor);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif //__SETDATE_H__