//**************************************************************************
//         Copyright (c) 2004, Cheertek Inc . All rights reserved.
//         D300, all right reserved.

//	Product	: WinDVD Firmware

//	Date	: 2004.12.22
//	Author	: Cheertek (D300 CoCo Chiang)
//	Purpose	: Provide the UI for Screen Saver.
//	Sources	: osdss.c/osdss.h
//***************************************************************************
#ifndef __OSDSS_H__
#define __OSDSS_H__

//***************************************************************************
//The defines for configuration
//***************************************************************************
#ifdef CT950_STYLE
#define OSDSS_JPEG_SCREEN_SAVER
#define OSDSS_ENTER_TIME                        (COUNT_10_SEC*2) //COUNT_3_SEC //(COUNT_10_SEC*6*10) //10 minutes
#else
#define OSDSS_ENTER_TIME                        (COUNT_10_SEC*6*10) //10 minutes
#endif //CT950_STYLE
#define OSDSS_SERVO_STOP_TIME                   (COUNT_10_SEC*6*5) //5 minutes

//***************************************************************************
//Defines for DVD LOGO screen saver
//***************************************************************************
//define the speed to move scrren saver
#define OSDSS_DVDLOGO_MOVE_SPEED                COUNT_50_MSEC

//define the speed to change color
#define OSDSS_DVDLOGO_COLOR_CHANGE_SPEED        COUNT_10_SEC

//define the speed to change mix ratio
#define OSDSS_DVDLOGO_MIX_RATIO_CHANGE_SPEED    (COUNT_100_MSEC)

#define OSDSS_BMP_SCREEN_SAVER					0	

#define OSDSS_DVDLOGO_BITMAP_START_H            0
#define OSDSS_DVDLOGO_BITMAP_START_V            0
#define OSDSS_DVDLOGO_POS_UPDATE_H              1
#define OSDSS_DVDLOGO_POS_UPDATE_V              2

//define the width and height of the bitmap
#define OSDSS_DVDLOGO_BITMAP_WIDTH              120
#define OSDSS_DVDLOGO_BITMAP_HEIGHT             64

//define the boundary of moving OSD region.
#define OSDSS_DVDLOGO_BOUNDARY_LEFT             (-20)
#define OSDSS_DVDLOGO_BOUNDARY_UP               (-20)
#define OSDSS_DVDLOGO_BOUNDARY_RIGHT            660
#define OSDSS_DVDLOGO_BOUNDARY_DOWN_NTSC        430
#define OSDSS_DVDLOGO_BOUNDARY_DOWN_PAL         520

//define the range update for mix ratio
#define OSDSS_DVDLOGO_MIX_RATIO_UPDTE           2

/*
//define the max value for color change
#define OSDSS_DVDLOGO_MAX_COLOR_VALUE           0xFFFF00              

//define the min value for color change
#define OSDSS_DVDLOGO_MIN_COLOR_VALUE           0x808000              

//define the color change
#define OSDSS_DVDLOGO_COLOR_CHANGE              0x100000
*/

//define the default mix ratio
#define OSDSS_DVDLOGO_DEFAULT_MIX_RATIO         31

//define the max mix ratio
#define OSDSS_DVDLOGO_MAX_MIX_RATIO             31

//define the updating range of the mix ratio
#define OSDSS_DVDLOGO_MIX_RATIO_UPDATE          1

//define the Min mix ratio
#define OSDSS_DVDLOGO_MIN_MIX_RATIO             0 //The min mix ratio must > OSDSS_DVDLOGO_MIX_RATIO_UPDATE

//define the entry for the color palette
#define OSDSS_PALETTE_ENTRY_START               GDI_GENERAL_UI_PALETTE_INDEX_START
#define OSDSS_PALETTE_ENTRY_DVD_LOGO            OSDSS_PALETTE_ENTRY_START

//define the palette color
#define OSDSS_PALETTE_COLOR_DVD_LOGO_1          0xFFD900            
#define OSDSS_PALETTE_COLOR_DVD_LOGO_2          0xFF6400
#define OSDSS_PALETTE_COLOR_DVD_LOGO_3          0xC83200
#define OSDSS_PALETTE_COLOR_DVD_LOGO_4          0x963200
#define OSDSS_PALETTE_COLOR_DVD_LOGO_5          0x963264
#define OSDSS_PALETTE_COLOR_DVD_LOGO_6          0x006400
#define OSDSS_PALETTE_COLOR_DVD_LOGO_7          0x966400
#define OSDSS_PALETTE_COLOR_DVD_LOGO_8          0xC89600
         
//***************************************************************************
//Export APIs
//***************************************************************************
// LLY2.53, expand OSD time unit from WORD to DWORD
//void OSDSS_Trigger(BYTE bType, WORD wTime);
void OSDSS_Trigger(BYTE bType, DWORD dwTime);
void OSDSS_Exit(BYTE bExitMode);
void OSDSS_Reset(void);
void OSDSS_Monitor(void);
void OSDSS_ResetTime(void);
void OSDSS_Entry(void);
void OSDSS_InternalExit(void);

extern BYTE __bOSDSSPicIdx;
extern BYTE _bOSDSSScreenSaverMode;
//***************************************************************************
//Internal APIs
//***************************************************************************
void _OSDSS_Move(void);
void _OSDSS_DVDLOGO_Initial(void);
void _OSDSS_ColorChange(void);
void _OSDSS_MixRatioChange(void);
void _OSDSS_Servo_Monitor(void);
void _OSDSS_PictureUpdate(BYTE bIdx);

//********************************************************************************************************
//data structure
//********************************************************************************************************
typedef struct tagPOSITION
{
    int nHPos;
    int nVPos;
} POSITION, *PPOSITION;


#endif //__OSDSS_H__
