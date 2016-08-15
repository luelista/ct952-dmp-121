#include "internal.h"
#include "customer.h"
#include "winav.h"

#ifdef CT950_STYLE
//#include "rtcdrv.h"
//#define TFT_SHOW_TIME
#endif
#ifdef SUPPORT_TFT
//#define VIPOR_CT675
#ifdef CT909P_IC_SYSTEM
  #define VP_API_CT909P
#else
  #define VP_API_CT909
#endif

//#define TFT_SUPPORT_AVIN
// LLY3.07, porting CZ-YHTang code to use define for AV In feature.
#ifdef TFT_SUPPORT_AVIN
#define USE_CVBS_TABLE
#endif  // #ifdef TFT_SUPPORT_AVIN

#ifdef SUPPORT_CCIR656
#define USE_CCIR656_TABLE
//#define SUPPORT_BATTERY_DETECTION
#else   // #ifdef SUPPORT_CCIR656
#define USE_SVIDEO_TABLE
#endif  // #ifdef SUPPORT_CCIR656


#ifdef SUPPORT_TFT_DIGITAL_MODE
  #define Digital_PANEL 
  #define PScan_mode
#endif 
#ifndef REGTABLE
#define     REGTABLE "VD\Regtable_CT950.H" //Set TFT include register table . Jeff VD 1.23 Add
#endif
#include REGTABLE
/*
#ifdef VP_API_CT909P
#if (DECODER_SYSTEM == DMP952A_EVAL)
   #include "VD\Regtable_CT952.H"  
#elif (DECODER_SYSTEM == DVD909P_A_EVAL)
   #include "VD\Regtable_CT909P_A.H"
#elif (DECODER_SYSTEM == DVD909P_EVAL_P3)
   #include "VD\Regtable_CT909P.H"   
#elif (DECODER_SYSTEM == DMP955A_EVAL)
   #include "VD\Regtable_CT955.H"
#elif ((DECODER_SYSTEM == DMP956A_EVAL)||(DECODER_SYSTEM == DVD956A_DEV))
 //#ifdef SUPPORT_TFT_DIGITAL_MODE
  #define Digital_PANEL
  #define PScan_mode
  #include "VD\Regtable_CT956.H" 
 //#else
  //#define PScan_mode
  //#include "VD\Regtable_CT955.H"   
 //#endif
#endif 
#else
  #include "VD\Regtable_CT950.H"
#endif //VP_API_CT909P
*/
#include "CC.h"
#include "setup.h"
#include "tft.h"
#include "input.h"
#include "utl.h"
#include "HAL.h"
#include "SERVO.h"
#include "navigate.h"
//#define SetPicture_ALL
#define MUSTEK_RING_BRIGHT
#include "VD\viporfunc.c"
#include "VD\ViporOSD.C"
#include "tftsetup.c" 

//#define TFT_DEBUG
//#define Light_detect

#ifdef CT951_PLATFORM //CoCo2.38 //Jeff check 20070320
#define PIN_SDAT   20
#define PIN_SCLK   21
#endif // #ifndef CT951_PLATFORM

I2C_CONFIG  I2C_Config;
BYTE        _sbAVInSW = FALSE;
BYTE        __bTft_TVModePrev = 0;
extern     BIT _btAVIN; //Jeff 20060705 Add for AV in

DWORD __dwTft_BackLight_Timer = 0xffffffff;

#ifdef SUPPORT_BATTERY_DETECTION
DWORD  __dwTtfLowBattTime = 0xffffffff;
BYTE   __bBatterty_POWER=0;
#endif

TFTSTATUS __TFTStatusInfo = {
#ifdef SUPPORT_BATTERY_DETECTION
                             TFT_BATTERY_STATUS_HIGH,
                             TFT_BATTERY_STATUS_HIGH,
#endif
                             TFT_TVTYPE_STATUS_NORMAL,
#ifdef SUPPORT_FLIP
                             EN_PANEL_FLIP_NORMAL,
#endif
                             TFT_BACKLIGHT_STATUS_ON,
                             TFT_AV_STATUS_DVD,
                             TFT_AV_STATUS_DVD,
                             FALSE,
                             TFT_SIGNAL_STATUS_UNLOCK,
                             TFT_SIGNAL_STATUS_NoSignal,
                             0,
                             0};

TFTOSDSTATUS        __TFTOSDStatusInfo;

TFTSYSTEMATTRIBUTE  __TFTSystemAttr;

TFTPICATTRIBUTE     __TFTPicAttr;

void _TFT_Init_I2C(void);
BYTE _TFT_ReadAvPIN(void);
#ifdef SUPPORT_BATTERY_DETECTION
void TFT_BatteryAction(void);
#endif
#ifdef TFT_SUPPORT_AVIN
void TFTOSD_ShowAvStatus(BYTE bShow);
//***********************************************************************************************
BYTE code TFTOSD_AV_String[] =
{
    _VIPOR_A,
    _VIPOR_V,
    _VIPOR_EOF
};
//--------------------------------------------------------------------------
// Function    : TFTOSD_ShowAvStatus
// Description : show  osd ''AV"  or clear it
// Input       :TRUE:show osd, false: clear osd
//--------------------------------------------------------------------------
void TFTOSD_ShowAvStatus(BYTE bShow)
{
    if(bShow)
    {
        Vipor_PrintString(0,
        0,
        VIPOR_OSD_GREEN,
        (BYTE*) &TFTOSD_AV_String);
    }
    else
    {
        Vipor_OsdClearRow( 0,  0, VIPOR_OSD_BLACK);
    }
}
#endif // #ifdef TFT_SUPPORT_AVIN

#ifdef SUPPORT_BATTERY_DETECTION
BYTE code TFTOSD_Battery_String[] =
{
        _VIPOR_L,
        _VIPOR_O,
        _VIPOR_W,
        _VIPOR__,
        _VIPOR_B,
        _VIPOR_A,
        _VIPOR_T,
        _VIPOR_T,
        _VIPOR_E,
        _VIPOR_R,
        _VIPOR_Y,
        _VIPOR_EOF
};

//--------------------------------------------------------------------------
// Function    : TFTOSD_ShowBattery
// Description : show  osd ''LOW BATTERY"  or clear it
// Input       :TRUE:show osd, false: clear osd
//--------------------------------------------------------------------------
void TFTOSD_ShowBattery(BYTE bShow)
{
    BYTE i;
    BYTE code *dptr = TFTOSD_Battery_String;
    BYTE __bBatterty_POWER = Vipor_BATTERY_State();

        __bBatterty_POWER = __bBatterty_POWER * 127 /1000;

    for(i=0; i<12; i++)
      gbTrmBuf[i]=*dptr++;
    //gbTrmBuf[8]=__bBatterty_POWER/100;
    //__bBatterty_POWER -= (gbTrmBuf[8]*100);
    /*
    gbTrmBuf[8]=(__bBatterty_POWER)/10;
    __bBatterty_POWER -= (gbTrmBuf[8]*10);
    gbTrmBuf[9] = _VIPOR_V;
    gbTrmBuf[10]=__bBatterty_POWER;
    gbTrmBuf[8]++;
    //gbTrmBuf[9]++;
    gbTrmBuf[10]++;
    gbTrmBuf[11]=_VIPOR_EOF;
    bShow = 1;
    */
        if (bShow && !__bBatterty_POWER)
        {
            
                Vipor_PrintString(12,
                0,
                VIPOR_OSD_RED,
                (BYTE*) &gbTrmBuf[0]);
        }
        else
        {
                Vipor_OsdClearRow( 12,  12, VIPOR_OSD_BLACK);
        }
}
#endif // #ifdef SUPPORT_BATTERY_DETECTION

//--------------------------------------------------------------------------
// Function    : TFT_BackLight
// Description : control backlight on or off
// Input       :TRUE:turn on backlight, false: turn off backlight
//--------------------------------------------------------------------------
void TFT_IO_BackLight(BYTE flag)
{   
   // Add code to turn off back light
   #if 0
   if (flag==TRUE)
   {
   	HAL_WriteGPIO(TFT_BACKLIGHT_GRP, PIN_TFT_BACKLIGHT, 1);	
	 printf("BackLight_On\n");
   }
   else
   {
   	HAL_WriteGPIO(TFT_BACKLIGHT_GRP, PIN_TFT_BACKLIGHT, 0);	
	printf("BackLight_Off\n");
   }
   #else
   if (flag==TRUE)
   {
   	HAL_WriteGPIO(TFT_BACKLIGHT_GRP, PIN_TFT_BACKLIGHT, 0);	
	 printf("BackLight_On\n");
   }
   else
   {
   	HAL_WriteGPIO(TFT_BACKLIGHT_GRP, PIN_TFT_BACKLIGHT, 1);	
	printf("BackLight_Off\n");
   }

   #endif
}


void TFT_BackLight(BYTE flag)
{
    if (flag)
     Vipor_SetBlackLight_ON();
    else
     Vipor_SetBlackLight_OFF(); 
     TFT_IO_BackLight(flag);
   // Add code to turn off back light
}

void TFT_FreeRun(BIT flag)
{
    if (flag)
    {
//      printf("SetBlackScreen\n");
      Vipor_SetBlackScreen();
    }
    else 
    {
//      printf("SetNormalOutput\n");
      Vipor_SetNormalOutput();
    }
}    
//--------------------------------------------------------------------------
// Function    : TFT_Adjust_VideoAttr
// Description :according input signal attribute to set VP77 register
// Input       :bSource:CVBS or SVIDEO; bSystem:NTSC or PAL
//--------------------------------------------------------------------------
void TFT_Adjust_VideoAttr(BYTE bSource, BYTE bSystem)  // because  the  regtable default subsystem is 0,  so we don't need to add bSubSystem param
{
        TFT_BackLight(0);
    if (bSource == EN_LCD_VID_FORMAT_CVBS)
    {
        if(bSystem==EN_VID_MODE_NTSC)
            Vipor_SetVideo_CVBS_NTSC();
        else if(bSystem==EN_VID_MODE_PAL)
            Vipor_SetVideo_CVBS_PAL();
    }
    else if(bSource==EN_LCD_VID_FORMAT_SVIDEO)
    {
        if(bSystem==EN_VID_MODE_NTSC)
            Vipor_SetVideo_SVIDEO_NTSC();
        else if(bSystem==EN_VID_MODE_PAL)
            Vipor_SetVideo_SVIDEO_PAL();
    }
    else if(bSource==EN_LCD_VID_FORMAT_CCIR656)  //Jeff 20060517 ADD here when USE CCIR656
    {
        if(bSystem==EN_VID_MODE_NTSC)
          Vipor_CCIR656_NTSC();
        else if(bSystem==EN_VID_MODE_PAL)
            Vipor_CCIR656_PAL();  
#ifdef PScan_mode//Digital_PANEL
        else if(bSystem==EN_VID_MODE_TFT_DIGITAL)
            Vipor_CCIR656_NTSC_P();
#endif //Digital_PANEL
            /*
        else if(bSystem==EN_VID_MODE_TFT_ANALOG)    
            ;
            */
    }
    TFTSetup_SetPicAttr();

    Vipor_OsdInit();
//  TFT_BackLight(1);
}

//--------------------------------------------------------------------------
// Function    : TFT_Adjust_TvType
// Description :set vp77 register to Adjust TFT PANEL display size
// Input       :bTvType: 16:9 or 4:3
//--------------------------------------------------------------------------
void TFT_Adjust_TvType(BYTE bTvType)
{
#ifdef MUSTEK_PF_A700_OLD
 #ifndef SUPPORT_6X4
    {
    extern BYTE __bMMMenuID, __bThumbnailMode;
    extern IMAGE_FRAME_SETUPINFO __ImageFrameSetupInfo;
    if(__ImageFrameSetupInfo.bDisplayType == SETUP_DISPLAY_TYPE_4_3)
    //if(__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_LB)
    {
        if((__bAttrPlay==ATTR_JPG) && (__bMMMenuID==0) && !__bThumbnailMode)   // MM_MENU_NONE = 0
        {
            if(bTvType != TFT_TVTYPE_STATUS_43LB)
            {
               bTvType=TFT_TVTYPE_STATUS_43LB;
            }
        }
        else
        {
            if(bTvType != TFT_TVTYPE_STATUS_NORMAL)
            {
                bTvType=TFT_TVTYPE_STATUS_NORMAL;
            }
        }
    }
    }
 #endif
#endif  
    if (__TFTStatusInfo.bAv==TFT_AV_STATUS_AV)
    {
     if(bTvType==TFT_TVTYPE_STATUS_NORMAL)
        {
         Vipor_SetAspect_16_9_CVBS();
        }
     else
        {
         Vipor_SetAspect_4_3_CVBS();
        }
    }
    else
    {
    if(bTvType==TFT_TVTYPE_STATUS_NORMAL)
        {
        Vipor_SetAspect_16_9();
        }
    else
        {
        Vipor_SetAspect_4_3();
        }
}
}

//--------------------------------------------------------------------------
// Function    : TFT_AdjustTVMode
// Description :set VP77 register to match video format. It is Called by hal.c when video format is changed  
// Input       :bMode:NTSC OR PAL
//--------------------------------------------------------------------------
void TFT_AdjustTVMode(BYTE bMode)
{
    if(__bTft_TVModePrev==bMode)
        return;
    //TFT_BackLight(FALSE);
//  _btToggle_TFT=1;
//        TFT_FreeRun(1);
    switch (bMode)
    {
        case SETUP_SCREEN_TV_SYSTEM_AUTO:
        case SETUP_SCREEN_TV_SYSTEM_NTSC:
          printf("SETUP_SCREEN_TV_SYSTEM_NTSC");
            __TFTSystemAttr.bSystem=EN_VID_MODE_NTSC;
            __TFTSystemAttr.bSubSystem=COLOR_NTSC_M;
                break;
        case SETUP_SCREEN_TV_SYSTEM_PAL:
          printf("SETUP_SCREEN_TV_SYSTEM_PAL");
            __TFTSystemAttr.bSystem=EN_VID_MODE_PAL;
            __TFTSystemAttr.bSubSystem=COLOR_PAL_BGDHI;
        break;
#ifdef PScan_mode//Digital_PANEL        
        case TFT_MODE_TFT_DIGITAL:
          printf("SETUP_SCREEN_TV_SYSTEM_P_NTSC");
            __TFTSystemAttr.bSystem=EN_VID_MODE_TFT_DIGITAL;
            __TFTSystemAttr.bSubSystem=COLOR_NTSC_M;
                break;
#endif //Digital_PANEL
        default:
          printf("SETUP_SCREEN_TV_SYSTEM_NTSC");
            __TFTSystemAttr.bSystem=EN_VID_MODE_NTSC;
            __TFTSystemAttr.bSubSystem=COLOR_NTSC_M;
                break;
    }
  __TFTSystemAttr.bSource=EN_LCD_VID_FORMAT_CCIR656;

    TFT_Adjust_VideoAttr(__TFTSystemAttr.bSource,__TFTSystemAttr.bSystem);
    TFT_Adjust_TvType(__TFTStatusInfo.bTvType);
    __TFTOSDStatusInfo.bAv=TFT_OSD_AVStatus_OFF;
#ifdef SUPPORT_BATTERY_DETECTION
    __TFTOSDStatusInfo.bBattery=TFT_OSD_BATTERY_OFF;
#endif
    __TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_OFF;

    __bTft_TVModePrev=bMode;
    __dwTft_BackLight_Timer=__dwCountSystem+COUNT_1_SEC*2;

//#ifdef LED_BackLight          
#ifndef SUPPORT_TFT_DIGITAL_MODE
     TFT_BackLight(TRUE);
#endif
}

//--------------------------------------------------------------------------
// Function    : TFT_AdjustInputSource
// Description :set VP77 register to match video input source. It is Called when video input source has been changed 
// Input       :
//--------------------------------------------------------------------------
void TFT_AdjustInputSource(void)
{
    if(__TFTStatusInfo.bReSetInputSource)
    {
        //clear all osd when av switch begin
        __TFTOSDStatusInfo.bAv=TFT_OSD_AVStatus_OFF;
#ifdef SUPPORT_BATTERY_DETECTION
        __TFTOSDStatusInfo.bBattery=TFT_OSD_BATTERY_OFF;
#endif
        __TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_OFF;

        __TFTStatusInfo.bLockSignal=TFT_SIGNAL_STATUS_UNLOCK;

        __TFTStatusInfo.bReSetInputSource=0;
        if(__TFTStatusInfo.bAv==TFT_AV_STATUS_AV) //external AV In
        {
            __TFTSystemAttr.bSource=EN_LCD_VID_FORMAT_CVBS;
            Vipor_SetBlackScreen();
        }  
        else //_sbAVInSW == dvd_STATUS
        {
        #ifdef SUPPORT_CCIR656  
              __TFTSystemAttr.bSource=EN_LCD_VID_FORMAT_CCIR656;  //Jeff 20060517 ADD here when USE CCIR656
        #else
              __TFTSystemAttr.bSource=EN_LCD_VID_FORMAT_SVIDEO;
        #endif
        }    
        __TFTSystemAttr.bSubSystem=0;
        TFT_Adjust_VideoAttr(__TFTSystemAttr.bSource,__TFTSystemAttr.bSystem);
        TFT_Adjust_TvType(__TFTStatusInfo.bTvType);
        __dwTft_BackLight_Timer=__dwCountSystem+COUNT_2_SEC;
        __bTft_TVModePrev=0xAf;
    }
}


//--------------------------------------------------------------------------
// Function    : TFT_SubSystem_Trigger
// Description :trigger subsystem , not finished 
// Input       :
//--------------------------------------------------------------------------
#if 0
BIT TFT_SubSystem_Trigger(BYTE bSystem, BYTE bSubSystem)
{
    if(bSystem==EN_VID_MODE_NTSC)
    {
        Vipor_NTSC_VideoSubSystem(bSubSystem);
        OS_DelayTime(COUNT_100_MSEC, FALSE);
        return(Vipor_NTSC_GetColorBurstLocked());
    }
    else if(bSystem==EN_VID_MODE_PAL)
    {
        Vipor_PAL_VideoSubSystem(bSubSystem);
        OS_DelayTime(COUNT_100_MSEC, FALSE);
        return(Vipor_PAL_GetColorBurstLocked(bSubSystem));
    }
}
#endif

//--------------------------------------------------------------------------
// Function    : TFT_AutoLockSignal
// Description :Auto detect signal and set right VP77 register to match the input signal. It is only called as in AV STATUS 
// Input       :
//--------------------------------------------------------------------------
void TFT_AutoLockSignal(void)
{
    if(!Vipor_GetVideoLocked())  //no signal
    {
        __TFTStatusInfo.bGetSignal=TFT_SIGNAL_STATUS_NoSignal;
    }
    else
    {
        __TFTStatusInfo.bGetSignal=TFT_SIGNAL_STATUS_GetSignal;
    }
    if(__TFTStatusInfo.bGetSignal==TFT_SIGNAL_STATUS_NoSignal)
    {
        if(__TFTOSDStatusInfo.bAv==TFT_OSD_AVStatus_OFF)
        {
            __TFTOSDStatusInfo.bAv=TFT_OSD_AVStatus_ON;
#ifdef TFT_SUPPORT_AVIN
        TFTOSD_ShowAvStatus(TRUE);
#endif //TFT_SUPPORT_AVIN
            Vipor_SetBlackScreen();
        }
        __TFTStatusInfo.bLockSignal=TFT_SIGNAL_STATUS_UNLOCK;
    }
    else
    {
            if(__TFTSystemAttr.bSystem!=Vipor_GetVideoMode())
            {
                __TFTSystemAttr.bSystem=Vipor_GetVideoMode();
                __TFTSystemAttr.bSubSystem=0;
                TFT_Adjust_VideoAttr(__TFTSystemAttr.bSource,__TFTSystemAttr.bSystem);
                TFT_Adjust_TvType(__TFTStatusInfo.bTvType);

                __TFTStatusInfo.bLockSignal=TFT_SIGNAL_STATUS_UNLOCK;

            }
            else
            {
                if(__TFTStatusInfo.bLockSignal==TFT_SIGNAL_STATUS_UNLOCK)
                {
                        //Vipor_AutoBiasControl();
                        __TFTStatusInfo.bLockSignal=TFT_SIGNAL_STATUS_LOCK;
                        Vipor_SetNormalOutput();
                        
                        __TFTOSDStatusInfo.bAv=TFT_OSD_AVStatus_OFF;
#ifdef TFT_SUPPORT_AVIN
        TFTOSD_ShowAvStatus(TRUE);
#endif //TFT_SUPPORT_AVIN
                        
#if 0               
                    if(TFT_SubSystem_Trigger(__TFTSystemAttr.bSystem,__TFTSystemAttr.bSubSystem))
                    {
                        __TFTStatusInfo.bLockSignal=TFT_SIGNAL_STATUS_LOCK;
                        if(__TFTOSDStatusInfo.bAv==TFT_OSD_AVStatus_ON)
                        {
                            __TFTOSDStatusInfo.bAv=TFT_OSD_AVStatus_OFF;
    //                          COBY7300_OsdClearAV();
                        }
                        //Vipor_AutoBiasControl();  //Jeff 20060816
                        
    //                  Vipor_SetNormalOutput();
                    }
                    else
                    {
                        __TFTSystemAttr.bSubSystem++;
                        if((__TFTSystemAttr.bSystem==EN_VID_MODE_NTSC)&&(__TFTSystemAttr.bSubSystem>=COLOR_60_MAX))
                        {
                            __TFTSystemAttr.bSubSystem=COLOR_NTSC_M;
                        __TFTStatusInfo.bLockSignal=TFT_SIGNAL_STATUS_LOCK;
                    }
                        if((__TFTSystemAttr.bSystem==EN_VID_MODE_PAL)&&(__TFTSystemAttr.bSubSystem>=COLOR_50_MAX))
                        {
                            __TFTSystemAttr.bSubSystem=COLOR_PAL_BGDHI;
                        __TFTStatusInfo.bLockSignal=TFT_SIGNAL_STATUS_LOCK;
                    }
                    }
#endif                      
                }
            }
    }
}


//--------------------------------------------------------------------------
// Function    : TFT_Init_Variable
// Description :iniitial variable
// Input       :
//--------------------------------------------------------------------------
void TFT_Init_Variable(void)
{
#ifdef SUPPORT_FLIP
    __TFTStatusInfo.bFlip=EN_PANEL_FLIP_NORMAL;
#endif
    __TFTStatusInfo.bTvType=TFT_TVTYPE_STATUS_NORMAL;
#ifdef SUPPORT_BATTERY_DETECTION
    __TFTStatusInfo.bBattery=TFT_BATTERY_STATUS_HIGH;
        __TFTStatusInfo.bBatteryPowerDown=TFT_BATTERY_STATUS_HIGH;
#endif
    __TFTStatusInfo.bBackLight=TFT_BACKLIGHT_STATUS_ON;
    __TFTStatusInfo.bLockSignal=TFT_SIGNAL_STATUS_UNLOCK;

    __TFTOSDStatusInfo.bAv=TFT_OSD_AVStatus_OFF;
#ifdef SUPPORT_BATTERY_DETECTION
    __TFTOSDStatusInfo.bBattery=TFT_OSD_BATTERY_OFF;
#endif
    __TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_OFF;

//  2. Select input source
 
    __TFTStatusInfo.bAvPin= _TFT_ReadAvPIN();
        __TFTStatusInfo.bAv=__TFTStatusInfo.bAvPin;
    __TFTStatusInfo.bAvChaned=FALSE;
    if(__TFTStatusInfo.bAv==TFT_AV_STATUS_AV)
    {
        __TFTSystemAttr .bSource=EN_LCD_VID_FORMAT_CVBS;
        __TFTSystemAttr .bSystem=EN_VID_MODE_NTSC;
        __TFTSystemAttr .bSubSystem=COLOR_NTSC_M;
    }
    else
    {
        #ifdef SUPPORT_CCIR656  
        __TFTSystemAttr.bSource=EN_LCD_VID_FORMAT_CCIR656;  //Jeff 20060517 ADD here when USE CCIR656
        #else
        __TFTSystemAttr .bSource=EN_LCD_VID_FORMAT_SVIDEO;
        #endif
        __TFTSystemAttr .bSystem=EN_VID_MODE_NTSC;
        __TFTSystemAttr .bSubSystem=COLOR_NTSC_M;
    }
    __bTft_TVModePrev=0xAf;//SETUP_SCREEN_TV_SYSTEM_NTSC;
    
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFT_Init_VP77
// Description : power on initial vp77
// Input       :
//--------------------------------------------------------------------------
void TFT_Init_VP77(void)
{

    //LOAD OSD FONT    
    Vipor_LoadOsdFont();    //Move here for 952 OSD load err
    //SET REG
    TFT_Adjust_VideoAttr(__TFTSystemAttr.bSource,__TFTSystemAttr.bSystem);
    TFT_Adjust_TvType(__TFTStatusInfo.bTvType);
    Vipor_LoadGammaTable();
    Vipor_OsdInit();
}


//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFT_Init
// Description : power on initial 
// Input       :
//--------------------------------------------------------------------------

void TFT_Init(void)
{
//  1. Initial TFT controller


#ifdef VP_API_CT909P
    DWORD dwSaveInt;
    OS_DISABLE_INTERRUPTS( dwSaveInt );
    (*((volatile DWORD *)(0x80004064))) |= 0x00512224;
    (*((volatile DWORD *)(0x80004068))) |= 0x000000C0;
#ifdef SUPPORT_AV_IN 
    (*((volatile DWORD *)(0x80004078))) |= 0x00004000;  //Av in   
#endif
#ifdef Digital_PANEL
    (*((volatile DWORD *)(0x80004058))) |= 0x0505A000;  //Digital panel
#endif
    OS_RESTORE_INTERRUPTS( dwSaveInt) ;
#endif

#ifdef CT951_PLATFORM    
    _TFT_Init_I2C();
#endif
    TFT_BackLight(0);
    TFT_Init_Variable();
    TFT_Init_VP77();
    Vipor_SetFlipType(EN_PANEL_FLIP_NORMAL);
//    Vipor_SetFlipType(EN_PANEL_FLIP_H);
//    Vipor_SetFlipType(EN_PANEL_FLIP_V);
//    Vipor_SetFlipType(EN_PANEL_FLIP_H_V);
    if (__TFTStatusInfo.bAv==TFT_AV_STATUS_AV)
    {
        __bFWKey=KEY_POWER;
    }
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFT_ProcessKey
// Description : process input key. 
// Input       :bkey
//--------------------------------------------------------------------------
BYTE TFT_ProcessKey()
{
    BYTE bKey;

#ifdef TFT_DEBUG
if (KEY_MUTE == __bKey)
{
__bKey = KEY_PICTURE;
    //Vipor_OsdClearRow(0, 15, VIPOR_OSD_BLACK);    // OSD_BLACK=0
#ifdef SUPPORT_BATTERY_DETECTION
  TFT_BatteryAction();
  TFTOSD_ShowBattery(TRUE);
#endif  
}
#endif

    if((__TFTStatusInfo.bDvdMonitorPowerDown)&&(__TFTStatusInfo.bAv==TFT_AV_STATUS_DVD))
        return KEY_BYPASS;

#ifdef SUPPORT_BATTERY_DETECTION
    if (__btPowerDown && (__TFTStatusInfo.bBatteryPowerDown == TFT_BATTERY_STATUS_LOW))
    {
        return KEY_NO_KEY;
    }
#endif

    bKey = TFTSetup_ProcessKey(__bKey);

    switch(bKey)
    {
#ifdef TFT_DEBUG
            case    KEY_TEST1:
                printf("\nDVD In");
                _sbAVInSW = 0;
                __TFTStatusInfo.bReSetInputSource = TRUE;
                __TFTStatusInfo.bAv = TFT_AV_STATUS_DVD;
                bKey=KEY_NO_KEY;
                break;
            case    KEY_TEST2:
                printf("\nExt AV In");
                _sbAVInSW = 1;
                __TFTStatusInfo.bReSetInputSource = TRUE;
                __TFTStatusInfo.bAv = TFT_AV_STATUS_AV;
                bKey=KEY_NO_KEY;
                break;
#endif
#ifdef SUPPORT_FLIP
        case KEY_TFT_REVERSE:
            if(__TFTStatusInfo.bFlip==EN_PANEL_FLIP_NORMAL)
            {
                __TFTStatusInfo.bFlip=EN_PANEL_FLIP_V;
            }
            else
            {
                __TFTStatusInfo.bFlip=EN_PANEL_FLIP_NORMAL;
            }
            Vipor_SetFlipType(__TFTStatusInfo.bFlip);
            bKey=KEY_NO_KEY;
            break;
#endif
#ifdef SUPPORT_BATTERY_DETECTION
        case KEY_DISPLAY:
                    if (__bBatterty_POWER)
                    {
                        __bBatterty_POWER = 0;
                    }
                    else 
                    {
                        __bBatterty_POWER = 1;
                    }
                    bKey=KEY_NO_KEY;
                    break;  
#endif
#if 0
  case KEY_TFT43:
		#ifdef SUPPORT_TFT_43_KEY
		  {   
		  	extern void Set_Display_Type_Key_Tft(void);
			Set_Display_Type_Key_Tft();
    		  }
		#endif
		  bKey=KEY_NO_KEY;
			break;
#endif
    case KEY_TEST3:
        if(__TFTStatusInfo.bTvType==TFT_TVTYPE_STATUS_NORMAL)
        {
            __TFTStatusInfo.bTvType=TFT_TVTYPE_STATUS_43LB;
        }
        else
        {
             __TFTStatusInfo.bTvType=TFT_TVTYPE_STATUS_NORMAL;
            }
            TFT_Adjust_TvType(__TFTStatusInfo.bTvType);
          bKey=KEY_NO_KEY;
            break;
        case KEY_NO_KEY:
            bKey=KEY_NO_KEY;
            break;
        default:
                        bKey = KEY_BYPASS;
            break;
    }
    return bKey;
}

#ifdef SUPPORT_BATTERY_DETECTION

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFT_HW_Detect_LowBattery
// Description : hardware detect battery low or high
// return      :TRUE:low battery, FALSE:high battery
//--------------------------------------------------------------------------
BIT  TFT_HW_Detect_LowBattery(void)
{
   __bBatterty_POWER = Vipor_BATTERY_State();

   if (__bBatterty_POWER > 0x4E)
        return FALSE;
   else 
        return TRUE;
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFT_BatteryAction
// Description :do action when battery is low
// return      :
//--------------------------------------------------------------------------

void TFT_BatteryAction(void)
{
        if(TFT_HW_Detect_LowBattery())
                __TFTStatusInfo.bBattery        =TFT_BATTERY_STATUS_LOW;
        else
        {
                __TFTStatusInfo.bBattery=TFT_BATTERY_STATUS_HIGH;
                __TFTStatusInfo.bBatteryPowerDown=TFT_BATTERY_STATUS_HIGH;
        }


        if(__TFTStatusInfo.bBattery==TFT_BATTERY_STATUS_LOW)
        {
                if(__TFTOSDStatusInfo.bBattery==TFT_OSD_BATTERY_OFF)
                {
                        __TFTOSDStatusInfo.bBattery=TFT_OSD_BATTERY_ON;
                        TFTOSD_ShowBattery(TRUE);
                        __dwTtfLowBattTime=__dwCountSystem+COUNT_10_SEC*18;   // 3min
                }
                else
                {
                        if(__dwCountSystem>__dwTtfLowBattTime) // 3min
                        {
                                TFT_BackLight(FALSE);
                                __TFTStatusInfo.bBatteryPowerDown=TFT_BATTERY_STATUS_LOW;
                                __dwTtfLowBattTime=0xffffffff;
                                if((__TFTStatusInfo.bAv==TFT_AV_STATUS_DVD)&&(!__btPowerDown))
                                        __bFWKey=KEY_POWER;
                        }
                }
         }
         else
         {
                if(__TFTOSDStatusInfo.bBattery==TFT_OSD_BATTERY_ON)
                {
                        __TFTOSDStatusInfo.bBattery=TFT_OSD_BATTERY_OFF;
                        TFTOSD_ShowBattery(FALSE);
                        if((__TFTStatusInfo.bAv==TFT_AV_STATUS_DVD)&&__btPowerDown)
                        {
                                __bFWKey=KEY_POWER;
                        }
                        else
                                TFT_BackLight(TRUE);
                }
         }
}
#endif // #ifdef SUPPORT_BATTERY_DETECTION





//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFT_PowerDown(BIT flag) 
// Description : Turn off the power of TFT_LCD 
// return      :
//--------------------------------------------------------------------------
void TFT_PowerDown(BIT flag)
{
    //Vipor_SetBlackLight_OFF(); 
    if (flag)
    {
#ifdef TFT_SUPPORT_AVIN
        TFTOSD_ShowAvStatus(TRUE);
#endif //TFT_SUPPORT_AVIN
        Vipor_SetPowerDown();
    }
    else
    {
#ifdef TFT_SUPPORT_AVIN
        if (_btAVIN)
          TFTOSD_ShowAvStatus(TRUE);
#endif //TFT_SUPPORT_AVIN
        Vipor_SetPowerON();
    } 
}


//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFT_Trigger
// Description : trigger tft status
// return      :
//--------------------------------------------------------------------------
#ifdef Light_detect
#define ADCGLB (*(volatile int *)(0x8000407C))
#endif
void TFT_Trigger(void)
{
#ifdef Light_detect
BYTE RDATA0,RDATA1;
DWORD wADCTemp; 
#endif
#ifdef TFT_SHOW_TIME    
 TFT_Show_Time();   
#endif
#ifdef SUPPORT_BATTERY_DETECTION
        TFT_BatteryAction();
#endif
#ifdef Light_detect
ADCGLB = 0x00F40000;
for (RDATA0=0xF8;RDATA0<0xFE;RDATA0++)
;
wADCTemp = ADCGLB;
RDATA0 = (BYTE) (wADCTemp >>24);
printf("KEY_DATA0 = %hx",RDATA0);
if ((RDATA0-RDATA1)>3||(RDATA1-RDATA0)>3)
{
   RDATA1 = RDATA0;
if ( RDATA0 > 0x20)
 Vipor_SC_Write_Byte(0xDE,0x98); 
else
if ( RDATA0 > 0x1A)
 Vipor_SC_Write_Byte(0xDE,0x58);
else
if ( RDATA0 > 0x13)
 Vipor_SC_Write_Byte(0xDE,0x28);
else
if ( RDATA0 > 0x0A)
 Vipor_SC_Write_Byte(0xDE,0x08);
else
 Vipor_SC_Write_Byte(0xDE,0x00);
 // TFT_BackLight(0);
}
#endif
#ifdef SUPPORT_AV_IN   // Wing2.32, support AV-IN
    if (( _btAVIN ) && (__TFTStatusInfo.bAv != TFT_AV_STATUS_AV))
    {
            __TFTStatusInfo.bReSetInputSource = TRUE;
            __TFTStatusInfo.bAv = TFT_AV_STATUS_AV;
    }
    if (( !_btAVIN ) && (__TFTStatusInfo.bAv != TFT_AV_STATUS_DVD))
    {
            __TFTStatusInfo.bReSetInputSource = TRUE;
            __TFTStatusInfo.bAv = TFT_AV_STATUS_DVD;
    }
#endif    

    TFT_AdjustInputSource();    
    if(__TFTStatusInfo.bAv==TFT_AV_STATUS_AV)
    {
        TFT_AutoLockSignal();
    }

    TFTSetup_Trigger();
#ifdef MUSTEK_PF_A700_OLD
 #ifndef SUPPORT_6X4
    {
    extern BYTE __bMMMenuID, __bThumbnailMode;
    extern IMAGE_FRAME_SETUPINFO __ImageFrameSetupInfo;
    if(__ImageFrameSetupInfo.bDisplayType == SETUP_DISPLAY_TYPE_4_3)
    //if(__SetupInfo.bAspectRatio == SETUP_SCREEN_TV_TYPE_LB)
    {
        if((__bAttrPlay==ATTR_JPG) && (__bMMMenuID==0) && !__bThumbnailMode)   // MM_MENU_NONE = 0
        {
            if(__TFTStatusInfo.bTvType != TFT_TVTYPE_STATUS_43LB)
            {
                __TFTStatusInfo.bTvType=TFT_TVTYPE_STATUS_43LB;
                TFT_Adjust_TvType(__TFTStatusInfo.bTvType);
            }
        }
        else
        {
            if(__TFTStatusInfo.bTvType != TFT_TVTYPE_STATUS_NORMAL)
            {
                __TFTStatusInfo.bTvType=TFT_TVTYPE_STATUS_NORMAL;
                TFT_Adjust_TvType(__TFTStatusInfo.bTvType);
            }
        }
    }
    }
 #endif
#endif
}


#ifdef SUPPORT_TFT_PANEL_DETECT
//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : PANEL_Detect_AV_Status
// Description : detect av  switch. called by PANEL_KEYSCAN
// return      :
//--------------------------------------------------------------------------
BYTE __bTft_AV_Count=0;
BYTE  PANEL_Detect_AV_Status(void)
{
#ifdef SUPPORT_BATTERY_DETECTION
    if(__TFTStatusInfo.bBattery == TFT_BATTERY_STATUS_LOW)
    {
        return KEY_INVALID;
    }
#endif

    if (__TFTStatusInfo.bAvPin != _TFT_ReadAvPIN())
    {
        TFT_BackLight(0);
        __dwTft_BackLight_Timer = 0xffffffff;
        __TFTStatusInfo.bAvPin = _TFT_ReadAvPIN();
        __TFTStatusInfo.bAvChaned =TRUE;
        __bTft_AV_Count=0;
    }
    else
    {
        if(__TFTStatusInfo.bAvChaned==TRUE)
        {
            __bTft_AV_Count++;
            if(__bTft_AV_Count==20)
            {
                __TFTStatusInfo.bAvChaned=FALSE;
                if(__TFTStatusInfo.bAv!=__TFTStatusInfo.bAvPin)
                {
                    __TFTStatusInfo.bAv=__TFTStatusInfo.bAvPin;
                    __TFTStatusInfo.bReSetInputSource=1;
                    if((!__btPowerDown)&&(__TFTStatusInfo.bAv==TFT_AV_STATUS_AV))
                    {
                        return KEY_POWER;
                    }  
                    else if((__btPowerDown)&&(__TFTStatusInfo.bAv==TFT_AV_STATUS_DVD))
                    {
                        return KEY_POWER;
                    }  

                }
                else
                {
                    __TFTStatusInfo.bReSetInputSource=1;
                    
                    if(__TFTStatusInfo.bAv==TFT_AV_STATUS_AV)
                    {
                        //VPSC_WriteByte(VPSC_OUTCTR1, 0x30);
                        //VPSC_WriteByte(0xBA, 0x65);       // KWD@2005/10/19
                        //VPSCResetLB();
                    }
                }
            }
        }
    }
#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
    if(__btPowerDown)
    {
        if(!SERVO_CheckTrayStatus()&&(__bFlowState&FLOW_KEEP_RESUME)) 
        {
            __bFlowState &= ~FLOW_KEEP_RESUME;
        }
        }
#endif //#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p

    if(__dwTft_BackLight_Timer<__dwCountSystem)
    {
        __dwTft_BackLight_Timer=0xffffffff;
        TFT_BackLight(1);
    }
    return KEY_NO_KEY;
}
#endif

#ifdef CT951_PLATFORM
void _TFT_Init_I2C()
{
    I2C_Config.bIOGroup=GPIO_A;
    I2C_Config.bSDAT_Pin=PIN_SDAT;
    I2C_Config.bSCLK_Pin=PIN_SCLK;
}
#endif

BYTE _TFT_ReadAvPIN(void)
{
    return _sbAVInSW;
}
#ifdef TFT_SHOW_TIME
void TFT_Show_Time(void)
{
    RTC_TM RTCTime;
    BYTE show_string[14];
      RTC_GetTime(&RTCTime);
      //printf(" %hx / %hx   %hx:%hx \n",RTCTime.bMon,RTCTime.bDate,RTCTime.bHr,RTCTime.bMin);
    show_string [0] = ((RTCTime.bMon)/10)+1;
    show_string [1] = (RTCTime.bMon-((show_string [0]-1)*10))+1;
    show_string [2] = _VIPOR_MINUS;
    show_string [3] = ((RTCTime.bDate)/10)+1;
    show_string [4] = (RTCTime.bDate-((show_string [3]-1)*10))+1;
    show_string [5] = _VIPOR__;
    show_string [6] = ((RTCTime.bHr)/10)+1;
    show_string [7] = (RTCTime.bHr-((show_string [6]-1)*10))+1;    
    show_string [8] = _VIPOR_COLON;    
    show_string [9] = ((RTCTime.bMin)/10)+1;
    show_string [10] = (RTCTime.bMin-((show_string [9]-1)*10))+1;    
    show_string [11] = _VIPOR_COLON;    
    show_string [12] = ((RTCTime.bSec)/10)+1;
    show_string [13] = (RTCTime.bSec-((show_string [12]-1)*10))+1;      
    show_string [14] = _VIPOR_EOF;
    Vipor_PrintString_ALL(13,12,VIPOR_OSD_MAGENTA,(BYTE*) &show_string);   
}    
    
#endif    
    

#else
BYTE code aReserve;
#endif

