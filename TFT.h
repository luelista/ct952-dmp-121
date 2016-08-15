
#ifndef __TFT_H__
#define __TFT_H__

#define TFT_BATTERY_STATUS_HIGH  0X00
#define TFT_BATTERY_STATUS_LOW  0X01

#define TFT_TVTYPE_STATUS_NORMAL  0X00
#define TFT_TVTYPE_STATUS_43LB  0X01

#define TFT_REVERSE_STATUS_NORMAL  0X00
#define TFT_REVERSE_STATUS_REVERSE  0X01

#define TFT_BACKLIGHT_STATUS_ON  0X01
#define TFT_BACKLIGHT_STATUS_OFF  0X00

#define TFT_AV_STATUS_DVD  0
#define TFT_AV_STATUS_AV  1

#define TFT_SIGNAL_STATUS_LOCK 0X01
#define TFT_SIGNAL_STATUS_UNLOCK 0X00

#define TFT_SIGNAL_STATUS_NoSignal 0X00
#define TFT_SIGNAL_STATUS_GetSignal 0X01


#define TFT_OSD_BATTERY_ON 0X01
#define TFT_OSD_BATTERY_OFF 0X00

#define TFT_OSD_SetupMenu_ON 0x01
#define TFT_OSD_SetupMenu_OFF 0x00

#define TFT_OSD_AVStatus_OFF    0x00
#define TFT_OSD_AVStatus_ON    0x01

#define TFT_MODE_TFT_DIGITAL    0xFF

typedef struct tagTFTSTATUS
{
#ifdef SUPPORT_BATTERY_DETECTION
    BYTE    bBattery;        
    BYTE    bBatteryPowerDown;
#endif
    BYTE    bTvType;        
#ifdef SUPPORT_FLIP
    BYTE    bFlip;        
#endif
    BYTE    bBackLight;        
    BYTE    bAvPin;
    BYTE    bAv;     
    BYTE    bAvChaned;
    BYTE    bLockSignal;        
    BYTE    bGetSignal;
    BYTE    bReSetInputSource;
    BYTE    bDvdMonitorPowerDown;
}TFTSTATUS;

typedef struct tagTFTOSDSTATUS
{
#ifdef SUPPORT_BATTERY_DETECTION
    BYTE    bBattery; 
#endif
    BYTE    bSetupMenu;
    BYTE    bAv; 
}TFTOSDSTATUS;

typedef struct tagTFTSystemAttribute
{
    BYTE    bSource;
    BYTE    bSystem; 
    BYTE    bSubSystem;
}TFTSYSTEMATTRIBUTE;

typedef struct tagTFTPicAttribute
{
    BYTE    bBrightness; 
    BYTE    bContrast;
    BYTE    bSaturation; 
}TFTPICATTRIBUTE;


extern TFTSTATUS           __TFTStatusInfo;
extern TFTOSDSTATUS        __TFTOSDStatusInfo;

extern TFTSYSTEMATTRIBUTE  __TFTSystemAttr;

extern TFTPICATTRIBUTE     __TFTPicAttr;


extern DWORD __dwTft_BackLight_Timer;

//alan.tft #define TFT_ReadAvPIN(void) (__sbAvIn?TFT_AV_STATUS_DVD:TFT_AV_STATUS_AV)
//#define TFT_ReadAvPIN TFT_AV_STATUS_AV
//#define TFT_ReadAvPIN TFT_AV_STATUS_DVD 

#ifdef SUPPORT_TFT_PANEL_DETECT
extern BYTE  PANEL_Detect_AV_Status(void);
#endif

extern void TFT_AdjustInputSource(void);
extern void TFT_Init(void);
void TFT_AdjustTVMode(BYTE bMode);
void TFT_Trigger(void);
BYTE TFT_ProcessKey(void);
void TFT_BackLight(BYTE flag);
void TFT_Adjust_TvType(BYTE bTvType);
void TFT_PowerDown(BIT flag);
void TFT_FreeRun(BIT flag);
#endif //__TFT_H__
