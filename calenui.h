#ifndef __CALENUI_H__
#define __CALENUI_H__

#include "rtcdrv.h"

//define the calendar mode
#define CALENDAR_MODE_SETTING                               0
#define CALENDAR_MODE_DISPLAY_CLOCK_AND_TIME                1
//define the mode of clock
#define CALENDAR_MODE_MENU                                  2
#define CALENDAR_MODE_DISPLAY_PERPETUAL_CALENDAR            3
#define CALENDAR_MODE_SET_DATE                              4

//define the distance between the date and time
#define CALENDAR_DATA_TIME_DISTANCE_H       20

#define CALENDAR_MENU_ENTRY_START		                    GDI_GENERAL_UI_PALETTE_INDEX_START
#define CALENDAR_MENU_ENTRY_TITLE_BG_COLOR			        (CALENDAR_MENU_ENTRY_START)
#define CALENDAR_MENU_ENTRY_LARGEFORM_BG_COLOR		        (CALENDAR_MENU_ENTRY_START+1)
#define CALENDAR_MENU_ENTRY_COLOR_DARK			            (CALENDAR_MENU_ENTRY_START+2)
#define CALENDAR_MENU_ENTRY_COLOR_HIGHLIGHT		            (CALENDAR_MENU_ENTRY_START+3)
#define CALENDAR_MENU_ENTRY_TETRIS_BODY_COLOR		        (CALENDAR_MENU_ENTRY_START+4)
#define CALENDAR_MENU_ENTRY_TITLE_BODY_HIGHLIGHT_COLOR		(CALENDAR_MENU_ENTRY_START+5)
#define CALENDAR_MENU_ENTRY_SUNDAY_COLOR                    (CALENDAR_MENU_ENTRY_START+6)
#define CALENDAR_MENU_ENTRY_SATURDAY_COLOR                  (CALENDAR_MENU_ENTRY_START+7)
#define CALENDAR_MENU_ENTRY_LAST						    CALENDAR_MENU_ENTRY_TITLE_BODY_HIGHLIGHT_COLOR

#define CALENDAR_MENU_VALUE_TITLE_BG_COLOR			        0x626395 //navy blue
#define CALENDAR_MENU_VALUE_LARGEFORM_BG_COLOR              0xC1C2C3  //gray  
#define CALENDAR_MENU_VALUE_COLOR_DARK			            0x202020 //black
#define CALENDAR_MENU_VALUE_COLOR_HIGHLIGHT		            0xE8CF92 //light yellow
#define CALENDAR_MENU_VALUE_TETRIS_BODY_COLOR		        0xA6A9C2 //light navy blue
#define CALENDAR_MENU_VALUE_SUNDAY_COLOR                    0xE10000    
#define CALENDAR_MENU_VALUE_SATURDAY_COLOR                  0xFFFF00
#define CALENDAR_MENU_VALUE_TITLE_BODY_HIGHLIGHT_COLOR      0xCC0000   //red fhfeng 1011


#define CALENDER_ITEM_WIDTH    60
#define CALENDER_ITEM_HEIGHT   38//60
#define CALENDER_ITEM_X_OFFSET 16
#define CALENDER_ITEM_BORDER   2

#define CALENDER_HEAD_HEIGHT_OF_YEAR	40
#define CALENDER_HEAD_HEIGHT_OF_WEEKLY	30
#define CALENDER_HEAD_HEIGHT	(CALENDER_HEAD_HEIGHT_OF_YEAR+CALENDER_HEAD_HEIGHT_OF_WEEKLY)
#define CALENDER_BODY_HEIGHT	(CALENDER_ITEM_HEIGHT*6 + 2*CALENDER_ITEM_BORDER)
#define CALENDER_TILE_HEIGHT	70
#define CALENDER_X_OFFSET		16

#define CAL_FIRSTYEAR	1900//1999
#define CAL_LASTYEAR	2100//2031

#define CAL_DEFAULT_YEAR  2006
#define CAL_DEFAULT_MONTH   10
#define CAL_DEFAULT_DATA    1
#define CALENDER_BACKGROUND_BORDER   16

#define YEAR_MONTH_CHANGE  0
#define DATA_CHANGE 1

void CALENDAR_Entry(BYTE bMode);
void CALENDAR_Recover(void);
BYTE CALENDAR_ProcessKey(void);
void CALENDAR_Exit(BYTE bClearBackground);
void CALENDAR_Trigger(void);

BYTE Calendar_SolarCalendar(WORD u16Year, BYTE u8Month,BYTE u8Day);

void _Calendar_ConfigOSDRegion(void);
void _CALENDAR_Set_YYMMDD(void);
BYTE _CALENDAR_ShowTime(BYTE bImmediate);
void _CALENDAR_ShowTimeContent(PRTC_TM pRTCTime);
void _CALENDAR_ShowCalendar(BYTE bShowTime);

#ifdef SUPPORT_SET_DATE_BY_MENU
void _CALENDAR_ShowMenu(void);
void CALENDAR_SetDate(WORD wYear, BYTE bMonth, BYTE bDate);
void CALENDAR_CancelSetDate(void);
#endif //SUPPORT_SET_DATE_BY_MENU

//////////////////////////////////////////////////////////////////////////////////////////
#ifdef SUPPORT_SET_DATE_BY_MENU
//UI part
#include "setdate.h"
#include "dialog.h"

typedef enum tagCALENDAR_ID_LIST
{
    CALENDAR_MAIN_BEGIN=0,
    CALENDAR_MAIN_DISPLAY = CALENDAR_MAIN_BEGIN,
    CALENDAR_MAIN_SET_DATE,
    CALENDAR_MAIN_EXIT,
    CALENDAR_MAIN_END = CALENDAR_MAIN_EXIT,   
} CALENDAR_ID_LIST;

#define CALENDAR_MAIN_MENU_START_X                          (MAINMENU_CONTENT_REGION_START_X+60)
#define CALENDAR_MAIN_MENU_START_Y                          (MAINMENU_CONTENT_REGION_START_Y+40)

//define the maximum visible menu items.
#define CALENDAR_MAX_VISIBLE_MENU_ITEM_NUM                  5

//////////////////////////////////////////////////////////////////////////////////////////
#ifdef CALENDAR_MAIN_FILE
#include "OSDString\\strextrn.h"

MENU_ITEM _MENU_ITEM_CALENDAR[] = {
    {aCLOCKDisplay, NULL, MENU_ITEM_STATE_ENABLE, CALENDAR_MAIN_DISPLAY},
    {aSETDATESetDate, NULL, MENU_ITEM_STATE_ENABLE, CALENDAR_MAIN_SET_DATE},
    {aCLOCKExit, NULL, MENU_ITEM_STATE_ENABLE, CALENDAR_MAIN_EXIT},
};

MENU _MENU_CALENDAR = {MENU_TYPE_GENERAL, (CALENDAR_MAIN_END-CALENDAR_MAIN_BEGIN+1), CALENDAR_MAIN_MENU_START_X, CALENDAR_MAIN_MENU_START_Y, MENU_WIDTH_OPTIMIZED, MENU_ITEM_ALIGNMENT_LEFT, _MENU_ITEM_CALENDAR, 0, 0, MENU_ITEM_NO_CHECKED_ITEM};

#ifdef NL_SOLUTION
#define CALENDAR_SETDATE_TITLE_DATE_DISTANCE_V     40
#else
#define CALENDAR_SETDATE_TITLE_DATE_DISTANCE_V     20
#endif //NL_SOLUTION

SETDATE_DATE _CALENDAR_SET_DATE = {2007, 1, 1, ((SET_DATE_DIALOG_WIDTH-SETDATE_DISPLAY_WIDTH)>>1), CALENDAR_SETDATE_TITLE_DATE_DISTANCE_V};
SETDATE_DLG_ACTION _CALENDAR_SET_DATE_Action = {CALENDAR_SetDate, CALENDAR_CancelSetDate};

#endif //CALENDAR_MAIN_FILE
#endif //SUPPORT_SET_DATE_BY_MENU
//////////////////////////////////////////////////////////////////////////////////////////


#endif //__CALENUI_H__

