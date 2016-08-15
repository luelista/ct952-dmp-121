#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "settime.h"
//////////////////////////////////////////////////////////////////////////////////////////
#define CLOCK_BITMAP_PALETTE_INDEX_START                8

#define CLOCK_BITMAP_NUM                                11

//Change this palette index to change the background color of the bitmap.
#define CLOCK_BITMAP_BACKGROUND_PALETTE_INDEX           15

//define the mode of clock
#define CLOCK_MODE_MENU                                 0
#define CLOCK_MODE_SHOW_CLOCK                           1
#define CLOCK_MODE_SET_TIME                             2

//////////////////////////////////////////////////////////////////////////////////////////
//The following define is for big clock display

#define CLOCK_NUMBER_BITMAP_WIDTH                       80
#define CLOCK_NUMBER_BITMAP_HEIGHT                      124

#define CLOCK_COLON_BITMAP_WIDTH                        32
#define CLOCK_COLON_BITMAP_HEIGHT                       124

#define CLOCK_DISPLAY_WIDTH                             (CLOCK_NUMBER_BITMAP_WIDTH*6+CLOCK_COLON_BITMAP_WIDTH*2)
#define CLOCK_DISPLAY_HEIGNT                            CLOCK_NUMBER_BITMAP_HEIGHT

#define CLOCK_DISPLAY_NUMBER_POS_START_X                ((POWERPNMENU_WIDTH-CLOCK_DISPLAY_WIDTH)>>1)
#define CLOCK_DISPLAY_NUMBER_POS_START_Y                ((POWERPNMENU_HEIGHT-CLOCK_DISPLAY_HEIGNT)>>1)

#define CLOCK_DISPLAY_TIME_DATE_DISTANCE_V              40
#define CLOCK_DISPLAY_DATE_POS_START_Y                  (CLOCK_DISPLAY_NUMBER_POS_START_Y+CLOCK_NUMBER_BITMAP_HEIGHT+CLOCK_DISPLAY_TIME_DATE_DISTANCE_V)

//palette
#define CLOCK_PALETTE_ENTRY_START                       1
#define CLOCK_PALETTE_ENTRY_DATE                        CLOCK_PALETTE_ENTRY_START

#define CLOCK_PALETTE_COLOR_DATE                        0xFFFF00


//////////////////////////////////////////////////////////////////////////////////////////
typedef enum tagCLOCK_ID_LIST
{
    CLOCK_MAIN_BEGIN=0,
    CLOCK_MAIN_DISPLAY = CLOCK_MAIN_BEGIN,
    CLOCK_MAIN_SET_TIME,
    CLOCK_MAIN_EXIT,
    CLOCK_MAIN_END = CLOCK_MAIN_EXIT,

    //Display
    CLOCK_DISPLAY_BEGIN = 6,
    CLOCK_DISPLAY_BIG_CLOCK = CLOCK_DISPLAY_BEGIN,
    CLOCK_DISPLAY_CLOCK_CALENDAR,
    CLOCK_DISPLAY_END = CLOCK_DISPLAY_CLOCK_CALENDAR,
   
} CLOCK_ID_LIST;

#define CLOCK_MAIN_MENU_START_X         (MAINMENU_CONTENT_REGION_START_X+60)
#define CLOCK_MAIN_MENU_START_Y         (MAINMENU_CONTENT_REGION_START_Y+40)

//define the maximum visible menu items.
#define CLOCK_MAX_VISIBLE_MENU_ITEM_NUM          5

//////////////////////////////////////////////////////////////////////////////////////////
//Export functions
void CLOCK_Entry(void);
BYTE CLOCK_ProcessKey(void);
BYTE CLOCK_Recover(BYTE bRecoverRegion);
void CLOCK_Trigger(BYTE bType, WORD wTime);
void CLOCK_Exit(BYTE bClearBackground);
void CLOCK_ShowMenu(void);
void CLOCK_SetTime(BYTE bHour, BYTE bMinute, BYTE bSecond);
void CLOCK_CancelSetTime(void);

//Internal functions
void _CLOCK_PrepareBigClock(void);
void _CLOCK_UpdateTimeNum(BYTE bNumIndex, BYTE bNumer);
void _CLOCK_DisplayBigClock(void);
void _CLOCK_ConfigOSDRegion(BYTE bMode);
void _CLOCK_DisplayBigClockColon(BYTE bIndex);
void _CLOCK_ProcessMenuItem(BYTE bID);
void _CLOCK_DisplayDate(void);

//extern variables
extern BYTE __bCLOCKShowClock;
//////////////////////////////////////////////////////////////////////////////////////////


#ifdef CLOCK_MAIN_FILE
//#include "setupstr.h"
#include "OSDString\\strextrn.h"

MENU_ITEM _MENU_ITEM_CLOCK_Display[] = {
    {aCLOCKBigClock, NULL, MENU_ITEM_STATE_ENABLE, CLOCK_DISPLAY_BIG_CLOCK},
    {aCLOCKClockCalendar, NULL, MENU_ITEM_STATE_ENABLE, CLOCK_DISPLAY_CLOCK_CALENDAR},
};

MENU _MENU_CLOCK_Display = {MENU_TYPE_GENERAL, (CLOCK_DISPLAY_END-CLOCK_DISPLAY_BEGIN+1), MENU_START_X_OPTIMIZED, MENU_START_Y_OPTIMIZED, MENU_WIDTH_OPTIMIZED, MENU_ITEM_ALIGNMENT_LEFT, _MENU_ITEM_CLOCK_Display, 0, 0, MENU_ITEM_NO_CHECKED_ITEM};

MENU_ITEM _MENU_ITEM_CLOCK[] = {
    {aCLOCKDisplay, (PMENU)&_MENU_CLOCK_Display, MENU_ITEM_STATE_ENABLE, CLOCK_MAIN_DISPLAY},
    {aCLOCKSetTime, NULL, MENU_ITEM_STATE_ENABLE, CLOCK_MAIN_SET_TIME},
    {aCLOCKExit, NULL, MENU_ITEM_STATE_ENABLE, CLOCK_MAIN_EXIT},
};

MENU _MENU_CLOCK = {MENU_TYPE_GENERAL, (CLOCK_MAIN_END-CLOCK_MAIN_BEGIN+1), CLOCK_MAIN_MENU_START_X, CLOCK_MAIN_MENU_START_Y, MENU_WIDTH_OPTIMIZED, MENU_ITEM_ALIGNMENT_LEFT, _MENU_ITEM_CLOCK, 0, 0, MENU_ITEM_NO_CHECKED_ITEM};

#define CLOCK_SETTIME_TITLE_TIME_DISTANCE_V     20

SETTIME_TIME _CLOCK_SET_TIME = {SETTIME_DISPLAY_MODE_HOUR_MINUTE_SEC, 0, 0, 0, ((SET_TIME_DIALOG_WIDTH-SETTIME_DISPLAY_MODE_HOUR_MINUTE_SEC_WIDTH)>>1), CLOCK_SETTIME_TITLE_TIME_DISTANCE_V};
SETTIME_DLG_ACTION _CLOCK_SET_TIME_Action = {CLOCK_SetTime, CLOCK_CancelSetTime};

#endif //CLOCK_MAIN_FILE

#endif //#ifndef __CLOCK_H__