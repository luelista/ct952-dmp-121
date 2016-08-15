#define CLOCK_MAIN_FILE

#include "winav.h"
#include "gdi.h"
#include "cc.h"
#include "osd.h"
#include "oswrap.h"
#include "input.h"
#include "poweronmenu.h"
#include "menu.h"
#include "mainmenu.h"
#include "clock.h"
#include "rtcdrv.h"
#include "setup.h"
#include "disp.h"
#include "calenui.h"
#include "osd.h"
#include "osdnd.h"
#include "fontindex.h"

const DWORD _aCLOCK_0[]=
{
#include "bmp/0.txt"
};

const DWORD _aCLOCK_1[]=
{
#include "bmp/1.txt"
};

const DWORD _aCLOCK_2[]=
{
#include "bmp/2.txt"
};

const DWORD _aCLOCK_3[]=
{
#include "bmp/3.txt"
};

const DWORD _aCLOCK_4[]=
{
#include "bmp/4.txt"
};

const DWORD _aCLOCK_5[]=
{
#include "bmp/5.txt"
};

const DWORD _aCLOCK_6[]=
{
#include "bmp/6.txt"
};

const DWORD _aCLOCK_7[]=
{
#include "bmp/7.txt"
};

const DWORD _aCLOCK_8[]=
{
#include "bmp/8.txt"
};

const DWORD _aCLOCK_9[]=
{
#include "bmp/9.txt"
};

const DWORD _aCLOCK_Colon[]=
{
#include "bmp/colon.txt"
};

const DWORD _aCLOCK_Palette[]=
{
#include "bmp/palNumber.txt"       
};

const DWORD *_aCLOCKBMPArray[CLOCK_BITMAP_NUM] = {
    _aCLOCK_0,
    _aCLOCK_1,
    _aCLOCK_2,
    _aCLOCK_3,
    _aCLOCK_4,
    _aCLOCK_5,
    _aCLOCK_6,
    _aCLOCK_7,
    _aCLOCK_8,
    _aCLOCK_9,
    _aCLOCK_Colon,
};

WORD aCLOCKDateDistance[] = {3, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_SPACE};
WORD aCLOCKDayDistance[] = {2, CHAR_SPACE, CHAR_SPACE};

DWORD _dwCLOCKTriggerTime;
RTC_TM _CLOCKCurrnetRTCTime; 
BYTE __bCLOCKShowClock = FALSE;
HMENU _hCLOCKMenu = MENU_HANDLE_INVALID;
HSETTIME _hCLOCKSetTime = SETTIME_HANDLE_INVALID;
BYTE _bCLOCKExitKey[] = {KEY_LEFT};
BYTE _bCLOCKMode;

extern BYTE _bPOWERONMENUSubUIMode;
//*********************************************************************************************************
//  Function    :   CLOCK_Entry
//  Abstract    :   This is the entry function of the clock.
//  Arguments   :   none.
//  Return		:   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void CLOCK_Entry(void)
{
    _hCLOCKMenu = MENU_HANDLE_INVALID;
    __bCLOCKShowClock = FALSE;

    _bCLOCKMode = CLOCK_MODE_MENU;
    
    _dwCLOCKTriggerTime = OS_GetSysTimer();
    RTC_GetTime(&_CLOCKCurrnetRTCTime);

#ifdef NL_SOLUTION
    _bCLOCKMode = CLOCK_MODE_SHOW_CLOCK;
    _CLOCK_PrepareBigClock();
    _CLOCK_DisplayBigClock();
#else
    
    //Show Clock menu.
    //Config the OSD to 4 bit mode.
    _CLOCK_ConfigOSDRegion(GDI_OSD_8B_MODE);
    
    CLOCK_ShowMenu();
    
#endif //NL_SOLUTION
#ifdef SUPPORT_SYSTEM_SHORTCUT_KEY
    if(Disable_Init_Menu())
    {
	  _bCLOCKMode = CLOCK_MODE_SHOW_CLOCK;
         _CLOCK_PrepareBigClock();
         _CLOCK_DisplayBigClock();
     }
#endif

}


//*********************************************************************************************************
//  Function    :   CLOCK_ProcessKey
//  Abstract    :   Call this function to process the keys.
//  Arguments   :   none.
//  Return		:   KEY_NO_KEY: The input key is processed.
//              :   INVALID_KEY: The input key is not allowed.
//              :   KEY_BYPASS: Will not process this key and pass it to main flow to process it.
//              :   KEY_XXX: The main program need do more process for the key.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE CLOCK_ProcessKey(void)
{
    WORD wMenuAction;
    DWORD dwMenuReturn;

    if (__bKey == KEY_FUNCTION)
    {
        CLOCK_Exit(FALSE);
        POWERONMENU_BackToMainMenu();            
    }

    switch (_bCLOCKMode)
    {
    case CLOCK_MODE_MENU:
        dwMenuReturn = MENU_ProcessKey(_hCLOCKMenu, __bKey);
        wMenuAction = HIWORD(dwMenuReturn);
        
        if (HIWORD(dwMenuReturn) == MENU_ACTION_EXIT)
        {
            CLOCK_Exit(FALSE);
            POWERONMENU_BackToMainMenu();                        
        }
        else if (wMenuAction == MENU_ACTION_PROCESS_MENU_ITEM)
        {
            _CLOCK_ProcessMenuItem(LOWORD(dwMenuReturn));
        }
        break;
    case CLOCK_MODE_SHOW_CLOCK:
        break;
    case CLOCK_MODE_SET_TIME:
        SETTIME_ProcessKey(_hCLOCKSetTime, __bKey);
        break;
    default:
        break;
    }

    return KEY_NO_KEY;
}

void CLOCK_ShowMenu(void)
{    
    MAINMENU_InitializeGDI(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
    MAINMENU_DrawMainBackground();
    MAINMENU_DrawSelectionHelp(FALSE, FALSE);
    
    //Draw the title
    MAINMENU_DrawTitleString(aCLOCKClockSetting, NULL, NULL);
    
    if (_hCLOCKMenu == MENU_HANDLE_INVALID)
    {
        _hCLOCKMenu = MENU_CreateMenu(&_MENU_CLOCK, CLOCK_MAX_VISIBLE_MENU_ITEM_NUM, _bCLOCKExitKey);
    }
    
    MENU_ShowMenu(_hCLOCKMenu, TRUE, MAINMENU_PALETTE_ENTRY_CONTENT_REGION);        
}

void _CLOCK_ProcessMenuItem(BYTE bID)
{
    switch (bID)
    {
    case CLOCK_DISPLAY_BIG_CLOCK:
        _bCLOCKMode = CLOCK_MODE_SHOW_CLOCK;
        _CLOCK_PrepareBigClock();
        _CLOCK_DisplayBigClock();
        break;
    case CLOCK_DISPLAY_CLOCK_CALENDAR:
        CLOCK_Exit(FALSE);
        _bPOWERONMENUSubUIMode = POWERONMENU_SUB_UI_CALENDAR;
        __bCLOCKShowClock = TRUE;
        CALENDAR_Entry(CALENDAR_MODE_DISPLAY_CLOCK_AND_TIME);
        break;
    case CLOCK_MAIN_EXIT:
        CLOCK_Exit(FALSE);
        POWERONMENU_BackToMainMenu();                        
        break;
    case CLOCK_MAIN_SET_TIME:
        _bCLOCKMode = CLOCK_MODE_SET_TIME;
        RTC_GetTime(&_CLOCKCurrnetRTCTime);
        _CLOCK_SET_TIME.bHour = _CLOCKCurrnetRTCTime.bHr;
        _CLOCK_SET_TIME.bMinute = _CLOCKCurrnetRTCTime.bMin;
        _CLOCK_SET_TIME.bSecond = _CLOCKCurrnetRTCTime.bSec;
        _hCLOCKSetTime  = SETTIME_Create(&_CLOCK_SET_TIME, &_CLOCK_SET_TIME_Action);
        if (_hCLOCKSetTime != SETTIME_HANDLE_INVALID)
        {
            SETTIME_Show(_hCLOCKSetTime, MAINMENU_PALETTE_ENTRY_CONTENT_REGION, __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
        }
        break;
    default:
        break;
    }
}

void CLOCK_SetTime(BYTE bHour, BYTE bMinute, BYTE bSecond)
{
    _CLOCKCurrnetRTCTime.bHr = bHour;
    _CLOCKCurrnetRTCTime.bMin = bMinute;
    _CLOCKCurrnetRTCTime.bSec = bSecond;

    RTC_SetTime(&_CLOCKCurrnetRTCTime);

    SETTIME_Exit(_hCLOCKSetTime, TRUE);
    MENU_ShowMenu(_hCLOCKMenu, TRUE, MAINMENU_PALETTE_ENTRY_CONTENT_REGION); 

    _bCLOCKMode = CLOCK_MODE_MENU;
}

void CLOCK_CancelSetTime(void)
{
    SETTIME_Exit(_hCLOCKSetTime, TRUE);
    MENU_ShowMenu(_hCLOCKMenu, TRUE, MAINMENU_PALETTE_ENTRY_CONTENT_REGION); 

    _bCLOCKMode = CLOCK_MODE_MENU;
}

void _CLOCK_PrepareBigClock(void)
{
    //Load palette for Navigaor's icons.
    GDI_PALETTE_INFO    PaletteInfo_Clock = {CLOCK_BITMAP_PALETTE_INDEX_START,  PAL_ADDR, _aCLOCK_Palette};    

    //Config the OSD to 4 bit mode.
    _CLOCK_ConfigOSDRegion(GDI_OSD_4B_MODE);

    GDI_LoadPalette(&PaletteInfo_Clock);
    //GDI_LoadBMP(_aCLOCKBMPArray, CLOCK_BITMAP_NUM);
    
    GDI_ChangePALEntry(CLOCK_BITMAP_BACKGROUND_PALETTE_INDEX, 0x5a000000, FALSE);
    
    GDI_WaitPaletteComplete();
}

//***************************************************************************
//  Function    :   _CLOCK_UpdateTimeNum
//  Abstract    :   Display the big colock.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _CLOCK_UpdateTimeNum(BYTE bNumIndex, BYTE bNumer)
{
    PARM_DRAW_BITMAP DrawBitmap;
    
    DrawBitmap.bBitmapID = bNumer;
    
    DrawBitmap.wDestX = CLOCK_DISPLAY_NUMBER_POS_START_X+bNumIndex*CLOCK_NUMBER_BITMAP_WIDTH+(bNumIndex/2)*CLOCK_COLON_BITMAP_WIDTH;
    DrawBitmap.wDestY = CLOCK_DISPLAY_NUMBER_POS_START_Y;
    
    DrawBitmap.bDestRegion = 0;
    GDI_DrawBitmapBySW(&DrawBitmap, _aCLOCKBMPArray); 
}

//***************************************************************************
//  Function    :   _CLOCK_DisplayBigClockColon
//  Abstract    :   Display the colon.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _CLOCK_DisplayBigClockColon(BYTE bIndex)
{
    PARM_DRAW_BITMAP DrawBitmap;
    
    DrawBitmap.bBitmapID = 10;
    
    DrawBitmap.wDestX = CLOCK_DISPLAY_NUMBER_POS_START_X+(bIndex+1)*(CLOCK_NUMBER_BITMAP_WIDTH*2)+bIndex*CLOCK_COLON_BITMAP_WIDTH;
    DrawBitmap.wDestY = CLOCK_DISPLAY_NUMBER_POS_START_Y;
    
    DrawBitmap.bDestRegion = 0;
    GDI_DrawBitmapBySW(&DrawBitmap, _aCLOCKBMPArray); 

//#ifdef NL_SOLUTION
    _CLOCK_DisplayDate();
//#endif //NL_SOLUTION
}

void _CLOCK_DisplayDate(void)
{
    BYTE bDay;
    PARM_DRAW_STRING DateStringAttr;
    PARM_RECT DateRect;

    RTC_TM Calendar;

    //Clear the region
    DateRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    DateRect.rect.wLeft = 0;
    DateRect.rect.wRight = DateRect.rect.wLeft + POWERPNMENU_WIDTH-1;
    DateRect.rect.wTop = CLOCK_DISPLAY_DATE_POS_START_Y;
    DateRect.rect.wBottom = DateRect.rect.wTop + POWERPNMENU_HEIGHT-1;
    GDI_FillRect(0, &DateRect);

    GDI_ChangePALEntry(CLOCK_PALETTE_ENTRY_DATE, CLOCK_PALETTE_COLOR_DATE, FALSE);

    DateStringAttr.bBackground = PAL_ENTRY_COLOR_TRANSPARENT;
    DateStringAttr.bColorKey = 0;
    DateStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    DateStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    DateStringAttr.bTextColor = CLOCK_PALETTE_ENTRY_DATE;     
    DateStringAttr.wY = CLOCK_DISPLAY_DATE_POS_START_Y;

//#ifdef CT951_PLATFORM
    RTC_GetTime(&Calendar);
    /*
#else
    Calendar.bYear = 6;
    Calendar.bMon = 1;
    Calendar.bDate = 1;
#endif //CT951_PLATFORM
    */

    OSDUTL_PrepareOutputString();
    switch (Calendar.bMon)
    {
    case 1:
        OSDUTL_CopyStringFromIndex(aMonthJanuary);
        break;
    case 2:
        OSDUTL_CopyStringFromIndex(aMonthFebruary);
        break;
    case 3:
        OSDUTL_CopyStringFromIndex(aMonthMarch);
        break;
    case 4:
        OSDUTL_CopyStringFromIndex(aMonthApril);
        break;
    case 5:
        OSDUTL_CopyStringFromIndex(aMonthMay);
        break;
    case 6:
        OSDUTL_CopyStringFromIndex(aMonthJune);
        break;
    case 7:
        OSDUTL_CopyStringFromIndex(aMonthJuly);
        break;
    case 8:
        OSDUTL_CopyStringFromIndex(aMonthAugust);
        break;
    case 9:
        OSDUTL_CopyStringFromIndex(aMonthSeptember);
        break;
    case 10:
        OSDUTL_CopyStringFromIndex(aMonthOctober);
        break;
    case 11:
        OSDUTL_CopyStringFromIndex(aMonthNovember);
        break;
    case 12:
        OSDUTL_CopyStringFromIndex(aMonthDecember);
        break;
    default:
        break;
    }

    OSDUTL_CopyStringFromIndex(aCLOCKDateDistance);
    OSDUTL_OutputNumFromIndex(Calendar.bDate);
    OSDUTL_CopyStringFromIndex(aCLOCKDateDistance);
    OSDUTL_OutputNumFromIndex(Calendar.bYear+2000);

    bDay = Calendar_SolarCalendar(Calendar.bYear+2000, Calendar.bMon, Calendar.bDate);
    OSDUTL_CopyStringFromIndex(aCLOCKDayDistance);

    switch (bDay)
    {
    case 0:
        OSDUTL_CopyStringFromIndex(aWeeklySunday);
        break;
    case 1:
        OSDUTL_CopyStringFromIndex(aWeeklyMonday);
        break;
    case 2:
        OSDUTL_CopyStringFromIndex(aWeeklyTuesday);
        break;
    case 3:
        OSDUTL_CopyStringFromIndex(aWeeklyWednesday);
        break;
    case 4:
        OSDUTL_CopyStringFromIndex(aWeeklyThursday);
        break;
    case 5:
        OSDUTL_CopyStringFromIndex(aWeeklyFriday);
        break;
    case 6:
        OSDUTL_CopyStringFromIndex(aWeeklySaturday);
        break;
    default:
        break;
    }

    DateStringAttr.wX = (POWERPNMENU_WIDTH-GDI_GetStringWidth(__pOSDUTLOutputString))>>1;
    GDI_DrawString_909(0, &DateStringAttr, __pOSDUTLOutputString);
}

//***************************************************************************
//  Function    :   _CLOCK_DisplayBigClock
//  Abstract    :   Display the big colock.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _CLOCK_DisplayBigClock(void)
{
    RTC_TM RTCTime; 

//#ifdef NL_SOLUTION
    //DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
//#endif //NL_SOLUTION
    POWERONMENU_ClearScreen();

    RTC_GetTime(&RTCTime); //Be called when system want to display current time, the query result will be stored in RTC_TM struct

    //Display hour
    _CLOCK_UpdateTimeNum(0, RTCTime.bHr/10);
    _CLOCK_UpdateTimeNum(1, RTCTime.bHr%10);

    //Display colon
    _CLOCK_DisplayBigClockColon(0);

    //Display minute
    _CLOCK_UpdateTimeNum(2, RTCTime.bMin/10);
    _CLOCK_UpdateTimeNum(3, RTCTime.bMin%10);

    //Display colon
    _CLOCK_DisplayBigClockColon(1);

    //Display second
    _CLOCK_UpdateTimeNum(4, RTCTime.bSec/10);
    _CLOCK_UpdateTimeNum(5, RTCTime.bSec%10);

    __bCLOCKShowClock = TRUE;
}

//  ***************************************************************************
//  Function    :   _CLOCK_ConfigOSDRegion
//  Description :   
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void _CLOCK_ConfigOSDRegion(BYTE bMode)
{
    GDI_REGION_INFO RegionInfo;
#ifdef  BORROW_FRAME_BUFFER
    DWORD dwBufferAddr;
#endif //

    GDI_ClearRegion(0);

#ifdef  BORROW_FRAME_BUFFER

    if ((POWERPNMENU_WIDTH == __RegionList[0].wWidth) && (POWERPNMENU_HEIGHT <= __RegionList[0].wHeight) &&
        (bMode == __RegionList[0].bColorMode)) 
    {
        return;
    }
    else if (__RegionList[0].bColorMode != bMode)   // Thumbnail mode & preview mode
    {
        // Use Frame buffer 1 or 2
        dwBufferAddr = DISP_QueryAvailableBuffer();
        if (0 == dwBufferAddr)
        {
            RegionInfo.dwTAddr = DS_OSDFRAME_ST;
        }
        else
        {
            RegionInfo.dwTAddr = dwBufferAddr;
        }
    } 
    else
    {
        // Use OSD buffer and Frame buffer 0
        RegionInfo.dwTAddr = DS_OSDFRAME_ST;   // Use OSD buffer and Frame buffer 0 
    }
#else
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
#endif // #ifdef  BORROW_FRAME_BUFFER

    RegionInfo.wWidth = POWERPNMENU_WIDTH;
    RegionInfo.wHeight = POWERPNMENU_HEIGHT;
    RegionInfo.bColorMode = bMode;

#ifdef  BORROW_FRAME_BUFFER
    OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    OSD_SetRegion(0, FALSE, &RegionInfo);
#endif
}

//*********************************************************************************************************
//  Function    :   CLOCK_Recover
//  Abstract    :   Call this function to do recovery.
//  Arguments   :   bRecoverRegion: OSD_RECOVER_UPPER_REGION/OSD_RECOVER_BOTTOM_REGION.
//  Return		:   TRUE: The region is recovered.
//              :   FALSE: The region doesn't need to be recovered in THUMB UI.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE CLOCK_Recover(BYTE bRecoverRegion)
{
    //Show Clock menu.
    //Config the OSD to 4 bit mode.
    _CLOCK_ConfigOSDRegion(GDI_OSD_8B_MODE);

    switch (_bCLOCKMode)
    {
    case CLOCK_MODE_SHOW_CLOCK:
        _CLOCK_PrepareBigClock();
        _CLOCK_DisplayBigClock();
        break;
    case CLOCK_MODE_MENU:
        CLOCK_ShowMenu();
        break;
    case CLOCK_MODE_SET_TIME:
        CLOCK_ShowMenu();
        SETTIME_Recover(_hCLOCKSetTime);
        break;
    default:
        break;
    }

    return TRUE;
}

void CLOCK_Trigger(BYTE bType, WORD wTime)
{
    RTC_TM RTCTime;

    if (__bCLOCKShowClock)
    {
        if ((OS_GetSysTimer() - _dwCLOCKTriggerTime) > COUNT_500_MSEC)
        {
            RTC_GetTime(&RTCTime);
            
            if ((_CLOCKCurrnetRTCTime.bSec == RTCTime.bSec) &&
                (_CLOCKCurrnetRTCTime.bMin == RTCTime.bMin) &&
                (_CLOCKCurrnetRTCTime.bHr == RTCTime.bHr))
            {
                return;
            }
            else
            {            
                //Check the second.
                _CLOCK_UpdateTimeNum(5, RTCTime.bSec%10);
                
                if ((_CLOCKCurrnetRTCTime.bSec/10) != (RTCTime.bSec/10))
                {
                    //Display second
                    _CLOCK_UpdateTimeNum(4, RTCTime.bSec/10);
                }
                
                if ((_CLOCKCurrnetRTCTime.bMin/10) != (RTCTime.bMin/10))
                {
                    //Display minute
                    _CLOCK_UpdateTimeNum(2, RTCTime.bMin/10);
                }
                
                if ((_CLOCKCurrnetRTCTime.bMin%10) != (RTCTime.bMin%10))
                {
                    _CLOCK_UpdateTimeNum(3, RTCTime.bMin%10);
                }
                
                //Display hour
                if ((_CLOCKCurrnetRTCTime.bHr/10) != (RTCTime.bHr/10))
                {
                    //Display minute
                    _CLOCK_UpdateTimeNum(0, RTCTime.bHr/10);
                }
                
                if ((_CLOCKCurrnetRTCTime.bHr%10) != (RTCTime.bHr%10))
                {
                    _CLOCK_UpdateTimeNum(1, RTCTime.bHr%10);
                }

                //Update the date?
                if ((_CLOCKCurrnetRTCTime.bHr == 0) && (_CLOCKCurrnetRTCTime.bMin == 0) && (_CLOCKCurrnetRTCTime.bSec == 0))
                {
                    _CLOCK_DisplayDate();
                }
                
                _CLOCKCurrnetRTCTime.bSec = RTCTime.bSec;
                _CLOCKCurrnetRTCTime.bMin = RTCTime.bMin;
                _CLOCKCurrnetRTCTime.bHr = RTCTime.bHr;
                _CLOCKCurrnetRTCTime.bDate = RTCTime.bDate;
                _CLOCKCurrnetRTCTime.bMon = RTCTime.bMon;
                _CLOCKCurrnetRTCTime.bDay = RTCTime.bDay;
                _CLOCKCurrnetRTCTime.bYear = RTCTime.bYear;
            }
            
            _dwCLOCKTriggerTime = OS_GetSysTimer();
        }
    }
}

void CLOCK_Exit(BYTE bClearBackground)
{
    if (_hCLOCKMenu != MENU_HANDLE_INVALID)
    {
        MENU_DeleteMenu(_hCLOCKMenu, bClearBackground);
        _hCLOCKMenu = MENU_HANDLE_INVALID;
    }

    if (_hCLOCKSetTime != SETTIME_HANDLE_INVALID)
    {
        SETTIME_Exit(_hCLOCKSetTime, TRUE);
        _hCLOCKSetTime = MENU_HANDLE_INVALID;
    }

    __bCLOCKShowClock = FALSE;

//#ifdef NL_SOLUTION
    //DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
//#endif //NL_SOLUTION
}
