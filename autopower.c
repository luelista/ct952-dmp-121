#define AUTOPWR_MAIN_FILE

#include "winav.h"
#include "ctkav.h"
#include "comdef.h"
#include "utl.h"
#include "chips.h"
#include "romld.h"
#include "hal.h"
#include "comutl.h"
#include "cc.h"
#include "RTCdrv.h"
#include "autopower.h"
#include "input.h"
#include "gdi.h"
#include "osd.h"
#include "setup.h"
#include "poweronmenu.h"
#include "mainmenu.h"
#include "menu.h"
#include "settime.h"
#include "autopower.h"
#include "disp.h"

extern BIT __btPowerDown;

AUTOPWR_TM __AutoPWROnTM;
AUTOPWR_TM __AutoPWROffTM;
BOOL __bEnableAutoPWROn;
BOOL __bEnableAutoPWROff;
BOOL __bTriggerAlarm;
BYTE __bAutoPWRState;

//For UI parts
HMENU _hAUTOPWRMenu = MENU_HANDLE_INVALID;
BYTE _bAUTOPWRMode;
SETTIME_TIME _AUTOPWRSetTime;
HSETTIME _hAUTOPWRSetTime = SETTIME_HANDLE_INVALID;
BYTE _bAUTOPWRExitKey[] = {KEY_LEFT};

extern IMAGE_FRAME_SETUPINFO __ImageFrameSetupInfo;

extern BYTE CC_PWRCtrl(void);
/*************************************************************************
*  Function     :   AUTOPWR_InitialValue
*  Description :   Initialize the variables of auto power module   
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_Initial ( void )
{
    __bAutoPWRState = AUTOPWR_NONE;

    //Initialize the value
   if (__ImageFrameSetupInfo.bAutoPowerOnEnable == AUTOPWR_POWER_ON_ENABLE_ON)
   {
        AUTOPWR_EnableOn();    
   }
   else
   {
       AUTOPWR_DisableOn();
   }

   if (__ImageFrameSetupInfo.bAutoPowerOffEnable == AUTOPWR_POWER_OFF_ENABLE_ON)
   {
        AUTOPWR_EnableOff();    
   }
   else
   {
       AUTOPWR_DisableOff();
   }

    __AutoPWROnTM.bHr     =  HIBYTE(__ImageFrameSetupInfo.wAutoPowerOnTime);
    __AutoPWROnTM.bMin    =  LOBYTE(__ImageFrameSetupInfo.wAutoPowerOnTime);

    __AutoPWROffTM.bHr     =  HIBYTE(__ImageFrameSetupInfo.wAutoPowerOffTime);
    __AutoPWROffTM.bMin    =  LOBYTE(__ImageFrameSetupInfo.wAutoPowerOffTime);
}

/*************************************************************************
*  Function     :   AUTOPWR_EnableOn
*  Description :   Enable auto power on
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_EnableOn(void)
{
    __bEnableAutoPWROn = TRUE;
}

/*************************************************************************
*  Function     :   AUTOPWR_DisableOn
*  Description :   Disable auto power on
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_DisableOn(void)
{
    __bEnableAutoPWROn = FALSE;
}

/*************************************************************************
*  Function     :   AUTOPWR_EnableOff
*  Description :   Enable auto power off
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_EnableOff(void)
{
    __bEnableAutoPWROff = TRUE;
}

/*************************************************************************
*  Function     :   AUTOPWR_DisableOff
*  Description :   Disable auto power off
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_DisableOff(void)
{
    __bEnableAutoPWROff = FALSE;
}

/*************************************************************************
*  Function     :   AUTOPWR_SetOnTM
*  Description :   Set auto power on timer
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_SetOnTM ( PAUTOPWR_TM ptm)
{
    __AutoPWROnTM.bHr     =  ptm->bHr;
    __AutoPWROnTM.bMin    =  ptm->bMin;
    //__AutoPWROnTM.bSec    =  ptm->bSec;    
}

/*************************************************************************
*  Function     :   AUTOPWR_GetOnTM
*  Description :   Get auto power on timer
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_GetOnTM ( PAUTOPWR_TM ptm )
{
    ptm->bHr     =    __AutoPWROnTM.bHr;
    ptm->bMin   =    __AutoPWROnTM.bMin;
    //ptm->bSec   =    __AutoPWROnTM.bSec;  
}

/*************************************************************************
*  Function     :   AUTOPWR_SetOffTM
*  Description :   Set auto power off timer
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_SetOffTM ( PAUTOPWR_TM ptm)
{
    __AutoPWROffTM.bHr     =  ptm->bHr;
    __AutoPWROffTM.bMin    =  ptm->bMin;
    //__AutoPWROffTM.bSec    =  ptm->bSec;    
}

/*************************************************************************
*  Function     :   AUTOPWR_GetOffTM
*  Description :   Get auto power off timer
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_GetOffTM ( PAUTOPWR_TM ptm )
{
    ptm->bHr     =    __AutoPWROffTM.bHr;
    ptm->bMin   =    __AutoPWROffTM.bMin;
    //ptm->bSec   =    __AutoPWROffTM.bSec;  
}

/*************************************************************************
*  Function     :   AUTOPWR_Trigger
*  Description :   Check auto power status
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_Trigger(void)
{
    _AUTOPWR_Check();
    _AUTOPWR_Monitor();
}

/*************************************************************************
*  Function     :   _AUTOPWR_Check
*  Description :   Check auto power status
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void _AUTOPWR_Check ( void )
{
    AUTOPWR_TM poweron_tm;
    AUTOPWR_TM poweroff_tm;    
    RTC_TM   rtc_tm;

    RTC_GetTime ( &rtc_tm );

    if ( __bEnableAutoPWROn && __btPowerDown)
    {
        AUTOPWR_GetOnTM ( &poweron_tm );
        if ( 
              (0  == rtc_tm.bSec) &&
              (poweron_tm.bHr     == rtc_tm.bHr) &&
              (poweron_tm.bMin   == rtc_tm.bMin)
            )
        {
            //Bingo!
            __bAutoPWRState = AUTOPWR_ON;
        }
    }
    else if ( __bEnableAutoPWROff && !__btPowerDown)
    {
        AUTOPWR_GetOffTM ( &poweroff_tm );
        if ( 
              (0  == rtc_tm.bSec) &&
              (poweroff_tm.bHr     == rtc_tm.bHr) &&
              (poweroff_tm.bMin   == rtc_tm.bMin)
            )
        {
            //Bingo!            
            __bAutoPWRState = AUTOPWR_OFF;
        }
    }
    else
    {
        __bAutoPWRState = AUTOPWR_NONE;
    }
}

/*************************************************************************
*  Function     :   _AUTOPWR_Monitor
*  Description :   Core state machine of auto power module
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void _AUTOPWR_Monitor ( void )
{
    //extern BYTE __bFWKey;

    if(__bAutoPWRState == AUTOPWR_ON && __btPowerDown)
    {
        //Do power switch Off -> On
        CC_PWRCtrl();
        POWERONMENU_Initial();
        __bAutoPWRState = AUTOPWR_NONE;
    }
    else if(__bAutoPWRState == AUTOPWR_OFF && !__btPowerDown)
    {
        //Do power switch On -> Off
        CC_PWRCtrl();
        __bAutoPWRState = AUTOPWR_NONE;
    }
    else
    {
        //Do nothing
        __bAutoPWRState = AUTOPWR_NONE;
    }
}

/*************************************************************************
*  Function     :   AUTOPWR_Test
*  Description :   Unit test
*  Arguments  :   None
*  Return        :   None
*  Side Effect   :
**************************************************************************/
void AUTOPWR_Test ( void )
{
    AUTOPWR_TM poweron_tm;
    AUTOPWR_TM poweroff_tm;    
    poweroff_tm.bHr = 0;
    poweroff_tm.bMin = 1;    
    //poweroff_tm.bSec = 15;     

    poweron_tm.bHr = 0;
    poweron_tm.bMin = 2;    
    //poweron_tm.bSec = 20; 
    
    __bAutoPWRState = AUTOPWR_NONE;
    AUTOPWR_EnableOn();    
    AUTOPWR_EnableOff();
    RTC_Test_Set();
    AUTOPWR_SetOffTM(&poweroff_tm);    
    AUTOPWR_SetOnTM(&poweron_tm);
}

//*********************************************************************************************************
//  Function    :   AUTOPWR_Entry
//  Abstract    :   This is the entry function of the Auto-Power.
//  Arguments   :   none.
//  Return		:   
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
void AUTOPWR_Entry(void)
{
    _hAUTOPWRMenu = MENU_HANDLE_INVALID;
    
    //Config the OSD to 8 bit mode.
    _AUTOPWR_ConfigOSDRegion();

    //Initialize the menu setting.
    _MENU_AUTOPWR_Power_On_Enable.bDefaultCursorPos = __ImageFrameSetupInfo.bAutoPowerOnEnable - AUTOPWR_POWER_ON_ENABLE_BEGIN;
    _MENU_AUTOPWR_Power_On_Enable.bCheckMenuItemPos = _MENU_AUTOPWR_Power_On_Enable.bDefaultCursorPos;

    _MENU_AUTOPWR_Power_Off_Enable.bDefaultCursorPos = __ImageFrameSetupInfo.bAutoPowerOffEnable - AUTOPWR_POWER_OFF_ENABLE_BEGIN;
    _MENU_AUTOPWR_Power_Off_Enable.bCheckMenuItemPos = _MENU_AUTOPWR_Power_Off_Enable.bDefaultCursorPos;

    _AUTOPWR_ShowMenu();
}

//  ***************************************************************************
//  Function    :   _AUTOPWR_ConfigOSDRegion
//  Description :   
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void _AUTOPWR_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;
#ifdef  BORROW_FRAME_BUFFER
    DWORD dwBufferAddr;
#endif //

    GDI_ClearRegion(0);

#ifdef  BORROW_FRAME_BUFFER

    if ((POWERPNMENU_WIDTH == __RegionList[0].wWidth) && (POWERPNMENU_HEIGHT <= __RegionList[0].wHeight) &&
        (GDI_OSD_8B_MODE == __RegionList[0].bColorMode)) 
    {
        return;
    }
    else if (__RegionList[0].bColorMode != GDI_OSD_8B_MODE)   // Thumbnail mode & preview mode
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
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;

#ifdef  BORROW_FRAME_BUFFER
    OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    OSD_SetRegion(0, FALSE, &RegionInfo);
#endif
}

void _AUTOPWR_ShowMenu(void)
{    
    MAINMENU_InitializeGDI(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
    MAINMENU_DrawMainBackground();
    MAINMENU_DrawSelectionHelp(FALSE, FALSE);
    
    //Draw the title
    MAINMENU_DrawTitleString(aAUTOPWRAutoPowerSetting, NULL, NULL);
    
    if (_hAUTOPWRMenu == MENU_HANDLE_INVALID)
    {
        _hAUTOPWRMenu = MENU_CreateMenu(&_MENU_AUTOPWR, AUTOPWR_MAX_VISIBLE_MENU_ITEM_NUM, _bAUTOPWRExitKey);
    }
    
    MENU_ShowMenu(_hAUTOPWRMenu, TRUE, MAINMENU_PALETTE_ENTRY_CONTENT_REGION);      
    
    _bAUTOPWRMode = AUTOPWR_MODE_MENU;
}

//*********************************************************************************************************
//  Function    :   AUTOPWR_ProcessKey
//  Abstract    :   Call this function to process the keys.
//  Arguments   :   none.
//  Return		:   KEY_NO_KEY: The input key is processed.
//              :   INVALID_KEY: The input key is not allowed.
//              :   KEY_BYPASS: Will not process this key and pass it to main flow to process it.
//              :   KEY_XXX: The main program need do more process for the key.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE AUTOPWR_ProcessKey(void)
{
    WORD wMenuAction;
    DWORD dwMenuReturn;

    if (__bKey == KEY_FUNCTION)
    {
        AUTOPWR_Exit(FALSE);
        POWERONMENU_BackToMainMenu();            
    }

    switch (_bAUTOPWRMode)
    {
    case AUTOPWR_MODE_MENU:
        dwMenuReturn = MENU_ProcessKey(_hAUTOPWRMenu, __bKey);
        wMenuAction = HIWORD(dwMenuReturn);
        
        if (HIWORD(dwMenuReturn) == MENU_ACTION_EXIT)
        {
            AUTOPWR_Exit(FALSE);
            POWERONMENU_BackToMainMenu();                        
        }
        else if (wMenuAction == MENU_ACTION_PROCESS_MENU_ITEM)
        {
            _AUTOPWR_ProcessMenuItem(LOWORD(dwMenuReturn));
        }
        break;
    case AUTOPWR_MODE_SET_TIME_POWER_ON:
    case AUTOPWR_MODE_SET_TIME_POWER_OFF:
        SETTIME_ProcessKey(_hAUTOPWRSetTime, __bKey);
        break;
    default:
        break;
    }

    return KEY_NO_KEY;
}

void _AUTOPWR_ProcessMenuItem(BYTE bID)
{
    switch (bID)
    {
    case AUTOPWR_POWER_ON_ENABLE_ON:
        AUTOPWR_EnableOn();

        //Set the setting to flash.
        __ImageFrameSetupInfo.bAutoPowerOnEnable = AUTOPWR_POWER_ON_ENABLE_ON;

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUTO_POWER_ON_ENABLE, (BYTE *)&(__ImageFrameSetupInfo.bAutoPowerOnEnable), 1);
#endif

        break;

    case AUTOPWR_POWER_ON_ENABLE_OFF:
        AUTOPWR_DisableOn();

        //Set the setting to flash.
        __ImageFrameSetupInfo.bAutoPowerOnEnable = AUTOPWR_POWER_ON_ENABLE_OFF;

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUTO_POWER_ON_ENABLE, (BYTE *)&(__ImageFrameSetupInfo.bAutoPowerOnEnable), 1);
#endif

        break;
    case AUTOPWR_POWER_OFF_ENABLE_ON:
        AUTOPWR_EnableOff();

        //Set the setting to flash.
        __ImageFrameSetupInfo.bAutoPowerOffEnable = AUTOPWR_POWER_OFF_ENABLE_ON;

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUTO_POWER_OFF_ENABLE, (BYTE *)&(__ImageFrameSetupInfo.bAutoPowerOffEnable), 1);
#endif
        break;
    case AUTOPWR_POWER_OFF_ENABLE_OFF:
        AUTOPWR_DisableOff();

        //Set the setting to flash.
        __ImageFrameSetupInfo.bAutoPowerOffEnable = AUTOPWR_POWER_OFF_ENABLE_OFF;

#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUTO_POWER_OFF_ENABLE, (BYTE *)&(__ImageFrameSetupInfo.bAutoPowerOffEnable), 1);
#endif

        break;
    case AUTOPWR_MAIN_EXIT:
        AUTOPWR_Exit(FALSE);
        POWERONMENU_BackToMainMenu();                        
        break;
    case AUTOPWR_SET_TIME_POWER_ON:
        _bAUTOPWRMode = AUTOPWR_MODE_SET_TIME_POWER_ON;

        _AUTOPWR_SET_TIME.bHour = __AutoPWROnTM.bHr;
        _AUTOPWR_SET_TIME.bMinute = __AutoPWROnTM.bMin;        

        _hAUTOPWRSetTime  = SETTIME_Create(&_AUTOPWR_SET_TIME, &_AUTOPWR_SET_TIME_Action);
        if (_hAUTOPWRSetTime != SETTIME_HANDLE_INVALID)
        {
            SETTIME_Show(_hAUTOPWRSetTime, MAINMENU_PALETTE_ENTRY_CONTENT_REGION, __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
        }
        break;
    case AUTOPWR_SET_TIME_POWER_OFF:
        _bAUTOPWRMode = AUTOPWR_MODE_SET_TIME_POWER_OFF;

        _AUTOPWR_SET_TIME.bHour = __AutoPWROffTM.bHr;
        _AUTOPWR_SET_TIME.bMinute = __AutoPWROffTM.bMin;        

        _hAUTOPWRSetTime  = SETTIME_Create(&_AUTOPWR_SET_TIME, &_AUTOPWR_SET_TIME_Action);
        if (_hAUTOPWRSetTime != SETTIME_HANDLE_INVALID)
        {
            SETTIME_Show(_hAUTOPWRSetTime, MAINMENU_PALETTE_ENTRY_CONTENT_REGION, __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
        }
        break;
    default:
        break;
    }
}

void AUTOPWR_Exit(BYTE bClearBackground)
{
    if (_hAUTOPWRMenu != MENU_HANDLE_INVALID)
    {
        MENU_DeleteMenu(_hAUTOPWRMenu, bClearBackground);
        _hAUTOPWRMenu = MENU_HANDLE_INVALID;
    }

    if (_hAUTOPWRSetTime != SETTIME_HANDLE_INVALID)
    {
        SETTIME_Exit(_hAUTOPWRSetTime, TRUE);
        _hAUTOPWRSetTime = SETTIME_HANDLE_INVALID;
    }
}

//*********************************************************************************************************
//  Function    :   AUTOPWR_Recover
//  Abstract    :   Call this function to do recovery.
//  Arguments   :   bRecoverRegion: OSD_RECOVER_UPPER_REGION/OSD_RECOVER_BOTTOM_REGION.
//  Return		:   TRUE: The region is recovered.
//              :   FALSE: The region doesn't need to be recovered in THUMB UI.
//  Side Effect :   none.
//  Notes       :
//*********************************************************************************************************
BYTE AUTOPWR_Recover(BYTE bRecoverRegion)
{
    //Config the OSD to 8 bit mode.
    _AUTOPWR_ConfigOSDRegion();

    _AUTOPWR_ShowMenu();

    if ((_bAUTOPWRMode == AUTOPWR_MODE_SET_TIME_POWER_ON) || (_bAUTOPWRMode == AUTOPWR_MODE_SET_TIME_POWER_OFF))
    {
        SETTIME_Recover(_hAUTOPWRSetTime);
    }

    return TRUE;
}

void AUTOPWR_SetTime(BYTE bHour, BYTE bMinute, BYTE bSecond)
{
    if (_bAUTOPWRMode == AUTOPWR_MODE_SET_TIME_POWER_ON)
    {
        __AutoPWROnTM.bHr     =  bHour;
        __AutoPWROnTM.bMin    =  bMinute;
        
        //Save the alarm time to flash.
        __ImageFrameSetupInfo.wAutoPowerOnTime = (bHour << 8) | bMinute;
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUTO_POWER_ON_TIME, (BYTE *)&(__ImageFrameSetupInfo.wAutoPowerOnTime), 2);
#endif

    }
    else
    {
        __AutoPWROffTM.bHr     =  bHour;
        __AutoPWROffTM.bMin    =  bMinute;

        //Save the alarm time to flash.
        __ImageFrameSetupInfo.wAutoPowerOffTime = (bHour << 8) | bMinute;
        
#ifdef WRITE_EEPROM_ENABLE //SYSTEM_8051
        HAL_WriteStorage(SETUP_ADDR_AUTO_POWER_OFF_TIME, (BYTE *)&(__ImageFrameSetupInfo.wAutoPowerOffTime), 2);
#endif
    }
    
    SETTIME_Exit(_hAUTOPWRSetTime, TRUE);
    MENU_ShowMenu(_hAUTOPWRMenu, TRUE, MAINMENU_PALETTE_ENTRY_CONTENT_REGION); 

    _bAUTOPWRMode = AUTOPWR_MODE_MENU;
}

void AUTOPWR_CancelSetTime(void)
{
    SETTIME_Exit(_hAUTOPWRSetTime, TRUE);
    MENU_ShowMenu(_hAUTOPWRMenu, TRUE, MAINMENU_PALETTE_ENTRY_CONTENT_REGION); 

    _bAUTOPWRMode = AUTOPWR_MODE_MENU;
}