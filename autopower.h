#ifndef __AUTOPOWER_H__
#define __AUTOPOWER_H__
#ifdef __cplusplus
        extern  "C"     {
#endif

typedef struct tagAUTOPWR_TM
{
    //BYTE bSec;    
    BYTE bMin;
    BYTE bHr;
    //BYTE bDate;
    //BYTE bMon;
    //BYTE bYear;
} AUTOPWR_TM, *PAUTOPWR_TM;

enum AUTOPWR_STATE
{
    AUTOPWR_NONE,
    AUTOPWR_ON,
    AUTOPWR_OFF    
};

//////////////////////////////////////////////////////////////////////////////////////////
//UI part
#include "settime.h"

typedef enum tagAUTOPWR_ID_LIST
{
    AUTOPWR_MAIN_BEGIN=0,
    AUTOPWR_MAIN_POWER_ON_ENABLE = AUTOPWR_MAIN_BEGIN,
    AUTOPWR_MAIN_POWER_OFF_ENABLE,
    AUTOPWR_MAIN_SET_TIME,
    AUTOPWR_MAIN_EXIT,
    AUTOPWR_MAIN_END = AUTOPWR_MAIN_EXIT,

    //Power on Enable
    AUTOPWR_POWER_ON_ENABLE_BEGIN = 6,
    AUTOPWR_POWER_ON_ENABLE_ON = AUTOPWR_POWER_ON_ENABLE_BEGIN,
    AUTOPWR_POWER_ON_ENABLE_OFF,
    AUTOPWR_POWER_ON_ENABLE_END = AUTOPWR_POWER_ON_ENABLE_OFF,

    //Power off Enable
    AUTOPWR_POWER_OFF_ENABLE_BEGIN = 10,
    AUTOPWR_POWER_OFF_ENABLE_ON = AUTOPWR_POWER_OFF_ENABLE_BEGIN,
    AUTOPWR_POWER_OFF_ENABLE_OFF,
    AUTOPWR_POWER_OFF_ENABLE_END = AUTOPWR_POWER_OFF_ENABLE_OFF,

    //Set Time
    AUTOPWR_SET_TIME_BEGIN = 15,
    AUTOPWR_SET_TIME_POWER_ON = AUTOPWR_SET_TIME_BEGIN,
    AUTOPWR_SET_TIME_POWER_OFF,
    AUTOPWR_SET_TIME_END = AUTOPWR_SET_TIME_POWER_OFF,
   
} AUTOPWR_ID_LIST;

#define AUTOPWR_MAIN_MENU_START_X                         (MAINMENU_CONTENT_REGION_START_X+60)
#define AUTOPWR_MAIN_MENU_START_Y                         (MAINMENU_CONTENT_REGION_START_Y+40)

//define the maximum visible menu items.
#define AUTOPWR_MAX_VISIBLE_MENU_ITEM_NUM                 5

//define the mode of clock
#define AUTOPWR_MODE_MENU                                 0
#define AUTOPWR_MODE_SET_TIME_POWER_ON                    1
#define AUTOPWR_MODE_SET_TIME_POWER_OFF                   2

//////////////////////////////////////////////////////////////////////////////////////////
//Export APIs
void AUTOPWR_Entry(void);
BYTE AUTOPWR_ProcessKey(void);
void AUTOPWR_Exit(BYTE bClearBackground);
BYTE AUTOPWR_Recover(BYTE bRecoverRegion);
void AUTOPWR_SetTime(BYTE bHour, BYTE bMinute, BYTE bSecond);
void AUTOPWR_CancelSetTime(void);


//Internal APIs
void _AUTOPWR_ConfigOSDRegion(void);
void _AUTOPWR_ProcessMenuItem(BYTE bID);
void _AUTOPWR_ShowMenu(void);

//////////////////////////////////////////////////////////////////////////////////////////
#ifdef AUTOPWR_MAIN_FILE
#include "OSDString\\strextrn.h"

MENU_ITEM _MENU_ITEM_AUTOPWR_Power_On_Enable[] = {
    {aMENUOn, NULL, MENU_ITEM_STATE_ENABLE, AUTOPWR_POWER_ON_ENABLE_ON},
    {aMENUOff, NULL, MENU_ITEM_STATE_ENABLE, AUTOPWR_POWER_ON_ENABLE_OFF},
};

MENU _MENU_AUTOPWR_Power_On_Enable = {MENU_TYPE_GENERAL, (AUTOPWR_POWER_ON_ENABLE_END-AUTOPWR_POWER_ON_ENABLE_BEGIN+1), MENU_START_X_OPTIMIZED, MENU_START_Y_OPTIMIZED, MENU_WIDTH_OPTIMIZED, MENU_ITEM_ALIGNMENT_LEFT, _MENU_ITEM_AUTOPWR_Power_On_Enable, 0, 1, 1};

MENU_ITEM _MENU_ITEM_AUTOPWR_Power_Off_Enable[] = {
    {aMENUOn, NULL, MENU_ITEM_STATE_ENABLE, AUTOPWR_POWER_OFF_ENABLE_ON},
    {aMENUOff, NULL, MENU_ITEM_STATE_ENABLE, AUTOPWR_POWER_OFF_ENABLE_OFF},
};

MENU _MENU_AUTOPWR_Power_Off_Enable = {MENU_TYPE_GENERAL, (AUTOPWR_POWER_OFF_ENABLE_END-AUTOPWR_POWER_OFF_ENABLE_BEGIN+1), MENU_START_X_OPTIMIZED, MENU_START_Y_OPTIMIZED, MENU_WIDTH_OPTIMIZED, MENU_ITEM_ALIGNMENT_LEFT, _MENU_ITEM_AUTOPWR_Power_Off_Enable, 0, 1, 1};

MENU_ITEM _MENU_ITEM_AUTOPWR_Set_Time[] = {
    {aAUTOPWRPowerOn, NULL, MENU_ITEM_STATE_ENABLE, AUTOPWR_SET_TIME_POWER_ON},
    {aAUTOPWRPowerOff, NULL, MENU_ITEM_STATE_ENABLE, AUTOPWR_SET_TIME_POWER_OFF},
};

MENU _MENU_AUTOPWR_Set_Time = {MENU_TYPE_GENERAL, (AUTOPWR_SET_TIME_END-AUTOPWR_SET_TIME_BEGIN+1), MENU_START_X_OPTIMIZED, MENU_START_Y_OPTIMIZED, MENU_WIDTH_OPTIMIZED, MENU_ITEM_ALIGNMENT_LEFT, _MENU_ITEM_AUTOPWR_Set_Time, 0, 0, MENU_ITEM_NO_CHECKED_ITEM};

MENU_ITEM _MENU_ITEM_AUTOPWR[] = {
    {aAUTOPWRPowerOnEnable, (PMENU)&_MENU_AUTOPWR_Power_On_Enable, MENU_ITEM_STATE_ENABLE, AUTOPWR_MAIN_POWER_ON_ENABLE},
    {aAUTOPWRPowerOffEnable, (PMENU)&_MENU_AUTOPWR_Power_Off_Enable, MENU_ITEM_STATE_ENABLE, AUTOPWR_MAIN_POWER_OFF_ENABLE},
    {aAUTOPWRSetTime, (PMENU)&_MENU_AUTOPWR_Set_Time, MENU_ITEM_STATE_ENABLE, AUTOPWR_MAIN_SET_TIME},
    {aCLOCKExit, NULL, MENU_ITEM_STATE_ENABLE, AUTOPWR_MAIN_EXIT},
};

MENU _MENU_AUTOPWR = {MENU_TYPE_GENERAL, (AUTOPWR_MAIN_END-AUTOPWR_MAIN_BEGIN+1), AUTOPWR_MAIN_MENU_START_X, AUTOPWR_MAIN_MENU_START_Y, MENU_WIDTH_OPTIMIZED, MENU_ITEM_ALIGNMENT_LEFT, _MENU_ITEM_AUTOPWR, 0, 0, MENU_ITEM_NO_CHECKED_ITEM};

#define AUTOPWR_SETTIME_TITLE_TIME_DISTANCE_V     20

SETTIME_TIME _AUTOPWR_SET_TIME = {SETTIME_DISPLAY_MODE_HOUR_MINUTE, 0, 0, 0, ((SET_TIME_DIALOG_WIDTH-SETTIME_DISPLAY_MODE_HOUR_MINUTE_WIEDH)>>1), AUTOPWR_SETTIME_TITLE_TIME_DISTANCE_V};
SETTIME_DLG_ACTION _AUTOPWR_SET_TIME_Action = {AUTOPWR_SetTime, AUTOPWR_CancelSetTime};

#endif //ALARM_MAIN_FILE
//////////////////////////////////////////////////////////////////////////////////////////



//extern BOOL __bTriggerAlarm;
//extern BYTE __bAutoPwrState;

extern AUTOPWR_TM __AutoPWROnTM;
extern AUTOPWR_TM __AutoPWROffTM;
extern BOOL __bEnableAutoPWROn;
extern BOOL __bEnableAutoPWROff;
extern BOOL __bTriggerAlarm;
extern BYTE __bAutoPWRState;

void _AUTOPWR_Check ( void );
void _AUTOPWR_Monitor ( void );

void AUTOPWR_Initial(void);
void AUTOPWR_EnableOn(void);
void AUTOPWR_DisableOn(void);
void AUTOPWR_EnableOff(void);
void AUTOPWR_DisableOff(void);
void AUTOPWR_SetOnTM ( PAUTOPWR_TM ptm);
void AUTOPWR_GetOnTM ( PAUTOPWR_TM ptm );
void AUTOPWR_SetOffTM ( PAUTOPWR_TM ptm);
void AUTOPWR_GetOffTM ( PAUTOPWR_TM ptm );
void AUTOPWR_Trigger ( void );

#ifdef __cplusplus
        }
#endif
#endif  // __AUTOPOWER_H__