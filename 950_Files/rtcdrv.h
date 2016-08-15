#ifndef __RTCDRV_H__
#define __RTCDRV_H__
#ifdef __cplusplus
        extern  "C"     {
#endif

#define RTC_WRITE_MODE 0x0
#define RTC_READ_MODE   0x1

enum RTC1302_REGADDR
{
    RTC1302_SEC_REG                 = 0x80,     //Second
    RTC1302_MIN_REG                 = 0x82,     //Minute
    RTC1302_HR_REG                   = 0x84,     //Hour
    RTC1302_DATE_REG               = 0x86,     //DATE
    RTC1302_MON_REG                 = 0x88,     //Month
    RTC1302_DAY_REG                 = 0x8A,     //DAY
    RTC1302_YEAR_REG                = 0x8C,    //YEAR
    RTC1302_CTRL_REG                = 0x8E,     //Control register
    RTC1302_TC_REG                    = 0x90,     //Trickle Charger
    RTC1302_CLOCKBURST_REG    = 0xBE,    //Clock Burst
    RTC1302_BRAM_REG                = 0xC0,    //B. Ram
    RTC1302_RAMBURST_REG        = 0xFE     //Ram Burst
};

enum RTC_BCD2BIN_COMMAND
{
    RTC_SEC,
    RTC_MIN,
    RTC_HR,
    RTC_DATE,
    RTC_MON,
    RTC_DAY,
    RTC_YEAR
};

enum RTC_CHKSLEEPLEAKTIME_MODE
{
    RTC_EMU_SLEEP,
    RTC_EMU_WAKEUP
};

typedef struct tagRTC_TM
{
    BYTE bSec;
    BYTE bMin;
    BYTE bHr;
    BYTE bDate;
    BYTE bMon;
    BYTE bDay;
    BYTE bYear;
} RTC_TM, *PRTC_TM;


BYTE RTC_Initial (void);
void  RTC_SetTime ( PRTC_TM pRTC_TM) ;
void  RTC_GetTime ( PRTC_TM pRTC_TM ) ;
void  RTC_SetCharge ( void );
void  RTC_Emu_ChkTimerReload( BYTE bMode );
void RTC_Test_Set(void);

VOID _RTC_Emu_Initial(void);
VOID _RTC_Emu_SetTime ( PRTC_TM pRTCEmu_TM);
VOID _RTC_Emu_Gmtime(PRTC_TM pRTCEmu_TM);
VOID _RTC_Emu_TimeDateCalculate(PRTC_TM pRTCEmu_TM, DWORD dwBaseSec);
BYTE _RTC_Emu_GetDaysofMonth(WORD wSolarYear, BYTE bSolarMonth);
BYTE _RTC_Emu_GetLeap( WORD wyear );

#ifdef __cplusplus
        }
#endif
#endif  // __RTCDRV_H__

