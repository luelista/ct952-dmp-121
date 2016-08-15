//=========================================================
//  The module is used to drive RTC. 
//=========================================================
#include "winav.h"
#include "ctkav.h"
#include "comdef.h"
#include "hal.h"
#include "comutl.h"
#include "RTCdrv.h"
/**************************************************************************
*    CONFIGURATION
**************************************************************************/
//#define EMU_RTC              //Enable RTC Emulator, DO NOT continue going after turning off system power
#define TIMER2
#define ONEDIODE            //Set One Diode for RTC Charging
#define DEFAULT_SEC    0
#define DEFAULT_MIN    0
#define DEFAULT_HR      0
#define DEFAULT_DATE  1 
#define DEFAULT_MON    1 //Jan
#define DEFAULT_DAY    1 //Monday
#define DEFAULT_YEAR  6 //2006

/*
  * RELOAD_VALUE_PON = 990671
  * 1/(132750000/2) *67 * RELOAD_VALUE_PON = 1 sec
  *
  */
#if defined(CT909P_IC_SYSTEM)||defined(CT909G_IC_SYSTEM)
#if (CPU_SPEED == CPU_133M) //132.75MHz
#define RELOAD_VALUE_PON  990671
#define RELOAD_VALUE_POFF    964285
#define CNT_RATIO_OFF_ON 1027
#define CNT_RATIO_ON_OFF 973
#endif
#if (CPU_SPEED == CPU_146M) //146NHz
#define RELOAD_VALUE_PON  999999
#define RELOAD_VALUE_POFF 964285
#define CNT_RATIO_OFF_ON 1037
#define CNT_RATIO_ON_OFF 964
#endif
#endif

#if defined(CT909R_IC_SYSTEM)
#if (CPU_SPEED == CPU_133M) //133NHz
#define RELOAD_VALUE_PON 992536
#define RELOAD_VALUE_POFF   964285
#define CNT_RATIO_OFF_ON 1029
#define CNT_RATIO_ON_OFF 971
#endif
#if (CPU_SPEED == CPU_146M) //146NHz
#define RELOAD_VALUE_PON 999999
#define RELOAD_VALUE_POFF 964285
#define CNT_RATIO_OFF_ON 1037
#define CNT_RATIO_ON_OFF 964
#endif
#endif

/**************************************************************************/

#ifdef EMU_RTC
ULONGLONG  __u64LastTick; //Recorded the elapsed time in ticks;
RTC_TM        __RTCEmu_TM;
BYTE _bChkSleepLeakTimeStart;
ULONGLONG __u64SystemTick2 = 0;
#endif

#ifndef EMU_RTC
/**************************************************************************
Function       :   RTC_InputByte
Abstract       :   Input one byte to RTC
Arguments   :   Input value
Return         :   none.
Side Effect    :   none.
Notes           :
***************************************************************************/
void RTC_InputByte ( BYTE bValue )
{
    BYTE bIndex;

    for ( bIndex = 0; bIndex < 8; bIndex++ )
    {
        HAL_WriteGPIO(GRP_RTCSDA, PIN_RTCSDA, 0);
        
        if ( bValue & 0x01 )
        {
            HAL_WriteGPIO(GRP_RTCSDA, PIN_RTCSDA, 1);
        }
        HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 1);
        HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 0);
        bValue = bValue >> 1;
    }
}

/**************************************************************************
Function       :   RTC_OutputByte
Abstract       :   Get one byte from RTC
Arguments   :   none.
Return         :   one byte from RTC.
Side Effect    :   none.
Notes           :
***************************************************************************/
BYTE RTC_OutputByte ( void )
{
    BYTE bIndex;
    BYTE bResult;

    bResult = 0;

    for ( bIndex = 0; bIndex < 8; bIndex++ )
    {
        bResult >>= 1;
        if ( HAL_ReadGPIO(GRP_RTCSDA, PIN_RTCSDA) )
        {
            bResult |= 0x80;
        }
        HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 1);
        HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 0);        
    }

    return bResult;
}

/**************************************************************************
Function       :   RTC_W1302
Abstract       :   write one byte to the addr of 1302 RTC
Arguments   :   bAddr : write register, bData : write value
Return         :   none.
Side Effect    :   none.
Notes           :
***************************************************************************/
void RTC_W1302 ( BYTE bAddr, BYTE bData )
{
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 0);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 1);
    RTC_InputByte ( bAddr );
    RTC_InputByte ( bData );
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 1);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);    
}

/**************************************************************************
Function       :   RTC_W1302
Abstract       :   Read one byte from the addr of 1302 RTC
Arguments   :   bAddr : write register
Return         :   The read result.
Side Effect    :   none.
Notes           :
***************************************************************************/
BYTE RTC_R1302 ( BYTE bAddr )
{
    BYTE bResult;
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 0);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 1);
    RTC_InputByte ( bAddr );
    bResult = RTC_OutputByte ( );
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 1);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);    
    return bResult;
}

/**************************************************************************
Function       :   RTC_BurstW1302T
Abstract       :   Consecutively write first eight clock/calendar registers
Arguments   :   BYTE *bWClock : 7Byte clock data + 1Byte control
Return         :   none.
Side Effect    :   none.
Notes           :
***************************************************************************/
void RTC_BurstW1302T(BYTE *bWClock)
{
    BYTE bIndex;
    
    //Disable write protection
    RTC_W1302 ( RTC1302_CTRL_REG, 0x00 ); 

    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 0);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 1);

    //Write clock burst address
    RTC_InputByte ( RTC1302_CLOCKBURST_REG );

    //8Byte = 7Byte clock data + 1Byte control
    for ( bIndex = 8; bIndex > 0; bIndex--)
    {
        RTC_InputByte( *bWClock );
        bWClock++;
    }
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 1);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);   
} 

/**************************************************************************
Function       :   RTC_BurstR1302T
Abstract       :   Consecutively read first eight clock/calendar registers
Arguments   :   
Return         :   BYTE *bWClock : 7Byte clock data + 1Byte control
Side Effect    :   none.
Notes           :
***************************************************************************/
void RTC_BurstR1302T(BYTE *bWClock)
{
    BYTE bIndex;
    
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 0);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 1);

    //Write clock burst address
    RTC_InputByte ( (RTC1302_CLOCKBURST_REG | RTC_READ_MODE) );

    //8Byte = 7Byte clock data + 1Byte control
    for ( bIndex = 8; bIndex > 0; bIndex--)
    {
        *bWClock = RTC_OutputByte( );
        bWClock++;
    }
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 1);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);   
} 


void RTC_BurstW1302R(BYTE *bWReg)
{
    BYTE bIndex;
    //Disable write protection
    RTC_W1302 ( RTC1302_CTRL_REG, 0x00 );     

    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 0);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 1);
    
    RTC_InputByte ( RTC1302_RAMBURST_REG );

    for ( bIndex = 31; bIndex > 0; bIndex--)       
    {
        RTC_InputByte(*bWReg);  
        bWReg++;
    }
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 1);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);  
} 

void RTC_BurstR1302R ( BYTE *bRReg )
{
    BYTE bIndex;
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 0);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 1);
    RTC_InputByte( RTC1302_RAMBURST_REG | RTC_READ_MODE );
    for ( bIndex = 31; bIndex > 0; bIndex--)
    {
        *bRReg = RTC_OutputByte ( );
        bRReg++;
    }
    HAL_WriteGPIO(GRP_RTCCLK, PIN_RTCCLK, 1);
    HAL_WriteGPIO(GRP_RTCRST, PIN_RTCRST, 0);  
}

void RTC_SetCharge ( void )
{
    RTC_W1302 ( RTC1302_CTRL_REG, 0x00 ); 
#ifdef ONEDIODE    
    RTC_W1302 ( RTC1302_TC_REG, 0xA5 );    // one diode
#else
    RTC_W1302 ( RTC1302_TC_REG, 0xA9 ); //Two diode only charge to 1.9V
#endif
    RTC_W1302 ( RTC1302_CTRL_REG, 0x80 );   
}
#endif

/**************************************************************************
Function       :   RTC_SetTime
Abstract       :   Set system time as 20YY/MM/DD/HH/MM/SS
Arguments   :   pointer of Time struct
Return         :   
Side Effect    :   none.
Notes           :
***************************************************************************/
void RTC_SetTime ( PRTC_TM pRTC_TM) 
{
#ifdef EMU_RTC

    DWORD dwSaveInt;
    __RTCEmu_TM.bYear   = pRTC_TM->bYear;
    __RTCEmu_TM.bMon    = pRTC_TM->bMon;
    __RTCEmu_TM.bDate   = pRTC_TM->bDate; 
    __RTCEmu_TM.bHr     = pRTC_TM->bHr;     
    __RTCEmu_TM.bMin    = pRTC_TM->bMin;         
    __RTCEmu_TM.bSec    = pRTC_TM->bSec;        

    HAL_DISABLE_INTERRUPTS(dwSaveInt);  
    __u64SystemTick2 = 0;
    HAL_RESTORE_INTERRUPTS(dwSaveInt);    
    
    REG_PLAT_TIMER2_CONTROL = 0x0;
    REG_PLAT_TIMER2_COUNTER = RELOAD_VALUE_PON;
    REG_PLAT_TIMER2_RELOAD = RELOAD_VALUE_PON;
    REG_PLAT_TIMER2_CONTROL |= 0x3;    //(RL|EN)    

    __u64LastTick = __u64SystemTick2;

#else
    //Disable write protection
    RTC_W1302 ( RTC1302_CTRL_REG, 0x00 );        
    RTC_W1302 ( RTC1302_SEC_REG,   COMUTL_DecimaltoBCD(pRTC_TM->bSec)  );  
    RTC_W1302 ( RTC1302_MIN_REG,   COMUTL_DecimaltoBCD(pRTC_TM->bMin)   );  
    RTC_W1302 ( RTC1302_HR_REG,     COMUTL_DecimaltoBCD(pRTC_TM->bHr)    );  
    RTC_W1302 ( RTC1302_DATE_REG, COMUTL_DecimaltoBCD(pRTC_TM->bDate) );  
    RTC_W1302 ( RTC1302_MON_REG,  COMUTL_DecimaltoBCD(pRTC_TM->bMon)  );  
    RTC_W1302 ( RTC1302_DAY_REG,   COMUTL_DecimaltoBCD(pRTC_TM->bDay)  );  
    RTC_W1302 ( RTC1302_YEAR_REG, COMUTL_DecimaltoBCD(pRTC_TM->bYear)  );      
    //Enable write protection
    RTC_W1302 ( RTC1302_CTRL_REG, 0x80 );       
#endif    
}

/**************************************************************************
Function       :   RTC_GetTime
Abstract       :   Get system time as 20YY/MM/DD/HH/MM/SS
Arguments   :   pointer of return Time struct
Return         :   
Side Effect    :   none.
Notes           :
***************************************************************************/
void RTC_GetTime ( PRTC_TM pRTC_TM ) 
{
#ifdef EMU_RTC    
    _RTC_Emu_Gmtime( pRTC_TM );
#else
    pRTC_TM->bSec    = COMUTL_BCDtoDecimal( RTC_R1302( RTC1302_SEC_REG  | RTC_READ_MODE ) );
    pRTC_TM->bMin    = COMUTL_BCDtoDecimal( RTC_R1302( RTC1302_MIN_REG   | RTC_READ_MODE ) );
    pRTC_TM->bHr      = COMUTL_BCDtoDecimal( RTC_R1302( RTC1302_HR_REG    | RTC_READ_MODE ) );
    pRTC_TM->bDate  = COMUTL_BCDtoDecimal( RTC_R1302( RTC1302_DATE_REG | RTC_READ_MODE ) );
    pRTC_TM->bMon   = COMUTL_BCDtoDecimal( RTC_R1302( RTC1302_MON_REG  | RTC_READ_MODE ) );
    pRTC_TM->bDay   = COMUTL_BCDtoDecimal( RTC_R1302( RTC1302_DAY_REG   | RTC_READ_MODE ) );
    pRTC_TM->bYear  = COMUTL_BCDtoDecimal( RTC_R1302( RTC1302_YEAR_REG  | RTC_READ_MODE ) );
#endif    
}

void RTC_Test_Set(void)
{
    RTC_TM bTest  = {0, 0, 0, 1, 1, 1, 6}; //sec, min, hr, date, month, day, year.
    printf("sec = 0x%hx", bTest.bSec);
    printf("min = 0x%hx", bTest.bMin);
    printf("hr = 0x%hx", bTest.bHr);
    printf("date = 0x%hx", bTest.bDate);
    printf("mon = 0x%hx", bTest.bMon);
    printf("day = 0x%hx", bTest.bDay);
    printf("year = 0x%hx", bTest.bYear);        
    RTC_SetTime (&bTest );
}

void RTC_Test_Get(void)
{
    RTC_TM bTest;
    RTC_GetTime( &bTest);
    printf("sec = 0x%hx", bTest.bSec);
    printf("min = 0x%hx", bTest.bMin);
    printf("hr = 0x%hx", bTest.bHr);
    printf("date = 0x%hx", bTest.bDate);
    printf("mon = 0x%hx", bTest.bMon);
    printf("day = 0x%hx", bTest.bDay);
    printf("year = 0x%hx", bTest.bYear);   
}

/**************************************************************************
Function       :   RTC_Initial
Abstract       :   System RTC initialization
Arguments   :   none
Return         :   
Side Effect    :   none.
Notes           :
***************************************************************************/
BYTE RTC_Initial (void)
{
#ifdef EMU_RTC
    _RTC_Emu_Initial();
#else
    BYTE bUninited;

    RTC_TM DefaultTM  = { DEFAULT_SEC, DEFAULT_MIN, DEFAULT_HR, 
                                         DEFAULT_DATE, DEFAULT_MON, DEFAULT_DAY, DEFAULT_YEAR};
    
    bUninited = RTC_R1302 ( RTC1302_SEC_REG | RTC_READ_MODE ) & 0x80;
    if ( bUninited )
    {
        RTC_SetTime ( &DefaultTM );
    }

    //Set charge
     RTC_SetCharge();
#endif        
    return TRUE;
}


#ifdef EMU_RTC
//--------------------------------------------------------------------
void RTC_Emu_ChkTimerReload(BYTE bMode)
{
    /* When system enters sleep:
      * Convert power-on timer counts into power-down timer counts
      * When system wakes up:
      * Convert power-down timer counts into power-on timer counts
      */
    DWORD dwTimer2Cnt;

    switch(bMode)
    {
        case    RTC_EMU_SLEEP:
                    dwTimer2Cnt = (RELOAD_VALUE_PON - REG_PLAT_TIMER2_COUNTER);
                    REG_PLAT_TIMER2_COUNTER = (RELOAD_VALUE_POFF - (dwTimer2Cnt*CNT_RATIO_ON_OFF)/1000);
                    REG_PLAT_TIMER2_RELOAD = RELOAD_VALUE_POFF;
                    _bChkSleepLeakTimeStart = TRUE;
                    break;
                    
        case    RTC_EMU_WAKEUP:
                    if(_bChkSleepLeakTimeStart == TRUE)
                    {
                        dwTimer2Cnt = RELOAD_VALUE_POFF - REG_PLAT_TIMER2_COUNTER;
                        REG_PLAT_TIMER2_COUNTER = (RELOAD_VALUE_PON - (dwTimer2Cnt*CNT_RATIO_OFF_ON)/1000 );
                        REG_PLAT_TIMER2_RELOAD = RELOAD_VALUE_PON;
                    }
                    break;
                    
        default:
                    return;
    }

}
//--------------------------------------------------------------------
VOID _RTC_Emu_Initial(void)
{
    
    REG_PLAT_TIMER2_COUNTER = RELOAD_VALUE_PON;
    REG_PLAT_TIMER2_RELOAD = RELOAD_VALUE_PON;
    REG_PLAT_TIMER2_CONTROL |= 0x3;    //(RL|EN)
    
    __RTCEmu_TM.bYear   = DEFAULT_YEAR;
    __RTCEmu_TM.bMon    = DEFAULT_MON;
    __RTCEmu_TM.bDate   = DEFAULT_DATE;
    __RTCEmu_TM.bHr      = DEFAULT_HR;
    __RTCEmu_TM.bMin    = DEFAULT_MIN;
    __RTCEmu_TM.bSec    = DEFAULT_SEC;
    
    __u64LastTick = __u64SystemTick2;

    _bChkSleepLeakTimeStart = FALSE;
}
//--------------------------------------------------------------------
BYTE _RTC_Emu_GetDaysofMonth(WORD wSolarYear, BYTE bSolarMonth)
{
    BYTE bSolarCal[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    if ( _RTC_Emu_GetLeap(wSolarYear) != 0 && bSolarMonth == 2 )
    {
        return bSolarCal[bSolarMonth-1]+1;
    }
    if ( bSolarMonth>12 )
    {
        return 0;
    }

    return bSolarCal[bSolarMonth-1];
}
//--------------------------------------------------------------------
BYTE _RTC_Emu_GetLeap( WORD wyear )
{
    if ( wyear % 400 == 0 )
    {
        return 1;
    }
    else if ( wyear % 100 == 0 )
    {
        return 0;
    }
    else if ( wyear % 4 == 0 )
    {
        return 1;
    }
    return 0;
}
//--------------------------------------------------------------------
void _RTC_Emu_Gmtime(PRTC_TM pRTCEmu_TM)
{
    DWORD dwBaseSecs;
    ULONGLONG u64CurTick;
    ULONGLONG u64Temp;
    
    pRTCEmu_TM->bYear   = __RTCEmu_TM.bYear;
    pRTCEmu_TM->bMon    = __RTCEmu_TM.bMon;   
    pRTCEmu_TM->bDate   = __RTCEmu_TM.bDate;    
    pRTCEmu_TM->bHr      = __RTCEmu_TM.bHr;    
    pRTCEmu_TM->bMin    = __RTCEmu_TM.bMin;    
    pRTCEmu_TM->bSec    = __RTCEmu_TM.bSec;    
    
    u64CurTick = __u64SystemTick2;
    u64Temp = u64CurTick - __u64LastTick;
    dwBaseSecs = (DWORD)(u64Temp);
    
    _RTC_Emu_TimeDateCalculate(pRTCEmu_TM, dwBaseSecs);        
}
//--------------------------------------------------------------------
void _RTC_Emu_TimeDateCalculate(PRTC_TM pRTCEmu_TM, DWORD dwBaseSec)
{
    WORD wSec, wMin, wHour, wDay;
    BYTE bLastDayOfMonth;

    //following converts seconds to dates, ex: 286855s => 3days, 7h:40m::55s
    //sec
    wSec = dwBaseSec % 60;
    dwBaseSec -= wSec;

    //min
    wMin = (dwBaseSec/60) % 60;
    dwBaseSec -= wMin * 60;

    //hour
    wHour = (dwBaseSec/3600) % 24;
    dwBaseSec -= wHour*3600;

    //day
    wDay = dwBaseSec/86400;

    pRTCEmu_TM->bSec +=  wSec;
    
    if(pRTCEmu_TM->bSec  > 59)
    {
        pRTCEmu_TM->bSec %= 60;
        pRTCEmu_TM->bMin++;
    }

    pRTCEmu_TM->bMin += wMin;
    
    if(pRTCEmu_TM->bMin > 59)
    {
        pRTCEmu_TM->bMin %= 60;
        pRTCEmu_TM->bHr++;
    }

    pRTCEmu_TM->bHr += wHour;
    
    if(pRTCEmu_TM->bHr > 23)
    {
        pRTCEmu_TM->bHr %= 24;
        pRTCEmu_TM->bDate++;
    }

    pRTCEmu_TM->bDate += wDay;

    bLastDayOfMonth = _RTC_Emu_GetDaysofMonth(pRTCEmu_TM->bYear+2000, pRTCEmu_TM->bMon);
    while(pRTCEmu_TM->bDate > bLastDayOfMonth)
    {
        pRTCEmu_TM->bDate -= bLastDayOfMonth;
        pRTCEmu_TM->bMon++;
        
        if(pRTCEmu_TM->bMon > 12)
        {
            pRTCEmu_TM->bMon = 1;
            pRTCEmu_TM->bYear++;
        }
        
        bLastDayOfMonth = _RTC_Emu_GetDaysofMonth(pRTCEmu_TM->bYear+2000, pRTCEmu_TM->bMon);
    }
}

#endif //#ifdef EMU_RTC

