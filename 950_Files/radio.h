//**************************************************************************
// Copyright (c) 2007, Cheertek Inc . All rights reserved.
// J000, all right reserved.
// Product :  Firmware
// Date : 2007.02.28
// Author : Cheertek (J000 Damon)
// Purpose : Provide UI and flow for radio
// Sources : radio.h/ radio.c
//***************************************************************************

#ifndef __RADIO_H__
#define __RADIO_H__

#ifdef SUPPORT_RADIO_TUNER

//////////////////////////////////////////////////////////////////////////////////////////

#define RADIO_MODE_NUM			4
#define RADIO_BAND_DRAW_NUM		5

//Define the mode of radio.
#define RADIO_MODE_NORMAL			0
#define RADIO_MODE_RECORD			1
#define RADIO_MODE_FAVORITE		2
#define RADIO_MODE_SCAN			3
#define RADIO_MODE_SELECTION		4

//Define freq. range
#define RADIO_FREQ_RANGE_US_EUROPE			0
#define RADIO_FREQ_RANGE_JAPAN_WIDE			1
#define RADIO_FREQ_RANGE_JAPAN_NORMAL		2

//Define freq. space
#define RADIO_FREQ_SPACE_200K			0
#define RADIO_FREQ_SPACE_100K			1
#define RADIO_FREQ_SPACE_50K			2

//Define arrow type
#define RADIO_COLOR_NORMAL		0
#define RADIO_COLOR_HIGHLIGHT		1

//Define the region to be cleared & updated.
#define RADIO_MODE_NAME_REGION		0
#define RADIO_FREQ_REGION				1
#define RADIO_INFO_REGION				2
#define RADIO_ALL_REGION				3

#define RADIO_DEFAULT_FREQ			8750

#define RDS_TYPE_NUMBER				13
#define SUPPORT_RDS_TYPE_NUMBER		4	

#define RADIO_RDS_MOVE_STEP		2

//Define the time to change status (highlight or normal) of freq. arrow, record list, and mode button.
#define RADIO_CURSOR_FLASH_TIME		COUNT_500_MSEC

// Define the time to update RDS information.
#define RADIO_RDS_FLASH_TIME		COUNT_25_MSEC

#define RADIO_RDS_WAIT_TIME		COUNT_1_SEC

#define RADIO_UNZIP_ADDR     (DS_RADIO_BMPDATA_ST+0xEA60)
#define RADIO_UNZIP_WINDOW_ADDR   (RADIO_UNZIP_ADDR+0x2D000)   

#define RADIO_RDS_REGION_ADDR		DS_RADIO_BMPDATA_ST // 15K DW

//////////////////////////////////////////////////////////////////////////////////////////
//The following define is for radio frame display.

#define RADIO_BG_PHOTO_START_H		24
#define RADIO_BG_PHOTO_START_V		60

#define RADIO_TITLE_START_H			(RADIO_BG_PHOTO_START_H+68)
#define RADIO_TITLE_START_V			(RADIO_BG_PHOTO_START_V+20)

#define RADIO_FREQ_STRING_WIDTH		70
#define RADIO_FREQ_STRING_START_H		(RADIO_BG_PHOTO_START_H+5)
#define RADIO_FREQ_STRING_START_V		(RADIO_BG_PHOTO_START_V+126)

#define RADIO_BAND_FRAME_WIDTH		310
#define RADIO_BAND_FRAME_HEIGHT		70
#define RADIO_BAND_FRAME_START_H		(RADIO_BG_PHOTO_START_H+92)			
#define RADIO_BAND_FRAME_START_V		(RADIO_BG_PHOTO_START_V+76)			
#define RADIO_BAND_FRAME_ARC_VALUE				13 //can be 13, 15, 17, 19, 21, 23, 25.
#define RADIO_BAND_FRAME_OUTER_ARC_VALUE		15

#define RADIO_FREQ_ARROW_START_V		(RADIO_BAND_FRAME_START_V+4)

#define RADIO_FREQ_BAR_WIDTH		(RADIO_BAND_FRAME_WIDTH-2*RADIO_DISTANCE_OFFSET30)
#define RADIO_FREQ_BAR_HEIGHT		6
#define RADIO_FREQ_BAR_START_H		(RADIO_BAND_FRAME_START_H+RADIO_DISTANCE_OFFSET30)
#define RADIO_FREQ_BAR_START_V		(RADIO_BAND_FRAME_START_V+RADIO_DISTANCE_OFFSET30-6)

#define RADIO_INFO_WIDTH				RADIO_BAND_FRAME_WIDTH
#define RADIO_INFO_HEIGHT				40
#define RADIO_INFO_START_H				(RADIO_BG_PHOTO_START_H+100)	
#define RADIO_INFO_START_V				(RADIO_BG_PHOTO_START_V+174)

#define RADIO_RDS_WIDTH					278
#define RADIO_RDS_HEIGHT					RADIO_INFO_HEIGHT
#define RADIO_RDS_START_H					(RADIO_BAND_FRAME_START_H+((RADIO_BAND_FRAME_WIDTH-RADIO_RDS_WIDTH)>>1))
#define RADIO_RDS_START_V					(RADIO_INFO_START_V+2) 


// For RDS bitmap copy region
//#define RADIO_RDS_REGION_WIDTH			616
//#define RADIO_RDS_REGION_HEIGHT			70
#define RADIO_RDS_BOUNDARY_BUFFER_SIZE	30
#define RADIO_RDS_INFO_DISPLAY_LEFT		50
#define RADIO_RDS_INFO_DISPLAY_RIGHT		(RADIO_RDS_INFO_DISPLAY_LEFT+RADIO_RDS_WIDTH-30-1)
#define RADIO_RDS_INFO_DISPLAY_TOP		0
#define RADIO_RDS_INFO_DISPLAY_BOTTOM	39
#define RADIO_RDS_INFO_BOUNDARY_LEFT		(RADIO_RDS_INFO_DISPLAY_LEFT-RADIO_RDS_BOUNDARY_BUFFER_SIZE)
#define RADIO_RDS_INFO_BOUNDARY_RIGHT	(RADIO_RDS_INFO_DISPLAY_RIGHT+RADIO_RDS_BOUNDARY_BUFFER_SIZE)
#define RADIO_RDS_INFO_START_H			RADIO_RDS_INFO_DISPLAY_RIGHT

/*
#define RADIO_RDS_INFO_BOUNDARY_LEFT		(RADIO_RDS_START_H)
#define RADIO_RDS_INFO_BOUNDARY_RIGHT	(RADIO_RDS_START_H+RADIO_RDS_WIDTH-1)
#define RADIO_RDS_INFO_START_H			(RADIO_RDS_START_H+RADIO_RDS_WIDTH-1-20)
*/
#define RADIO_RECORD_WIDTH			180
#define RADIO_RECORD_HEIGHT			RADIO_INFO_HEIGHT
#define RADIO_RECORD_START_H			(RADIO_BAND_FRAME_START_H+((RADIO_BAND_FRAME_WIDTH-RADIO_RECORD_WIDTH)>>1))
#define RADIO_RECORD_START_V			(RADIO_INFO_START_V+2)
#define RADIO_RECORD_LIST_WIDTH		70
#define RADIO_RECORD_FREQ_WIDTH		108
#define RADIO_RECORD_FREQ_START_H	(RADIO_RECORD_START_H+RADIO_RECORD_LIST_WIDTH+2)

#define RADIO_SCAN_WIDTH			260
#define RADIO_SCAN_HEIGHT			RADIO_INFO_HEIGHT
#define RADIO_SCAN_START_H			(RADIO_BAND_FRAME_START_H+((RADIO_BAND_FRAME_WIDTH-RADIO_SCAN_WIDTH)>>1))
#define RADIO_SCAN_START_V			(RADIO_INFO_START_V+2)

#define RADIO_BUTTON_WIDTH		116
#define RADIO_BUTTON_HEIGHT		38
#define RADIO_BUTTON_ARC_VALUE				19 
#define RADIO_BUTTON_OUTER_ARC_VALUE			21

#define RADIO_BUTTON_NORMAL_START_H		(RADIO_BAND_FRAME_START_H+RADIO_BAND_FRAME_WIDTH+4)
#define RADIO_BUTTON_NORMAL_START_V		(RADIO_BG_PHOTO_START_V+38)

#define RADIO_BUTTON_RECORD_START_H		(RADIO_BUTTON_NORMAL_START_H+14)
#define RADIO_BUTTON_RECORD_START_V		(RADIO_BG_PHOTO_START_V+94)

#define RADIO_BUTTON_FAVORITE_START_H		RADIO_BUTTON_RECORD_START_H
#define RADIO_BUTTON_FAVORITE_START_V		(RADIO_BG_PHOTO_START_V+150)

#define RADIO_BUTTON_SCAN_START_H		RADIO_BUTTON_NORMAL_START_H
#define RADIO_BUTTON_SCAN_START_V			(RADIO_BG_PHOTO_START_V+206)

#define RADIO_VOLUME_START_H		RADIO_BG_PHOTO_START_H
#define RADIO_VOLUME_START_V		(RADIO_BG_PHOTO_START_V+280+RADIO_DISTANCE_OFFSET20)

#define RADIO_ARC_PIXEL			2

#define RADIO_DISTANCE_OFFSET10			10
#define RADIO_DISTANCE_OFFSET20			20
#define RADIO_DISTANCE_OFFSET30			30
#define RADIO_DISTANCE_OFFSET50			50

//////////////////////////////////////////////////////////////////////////////////////////
//Palette & Color

#define RADIO_BITMAP_PALETTE_INDEX_START			85 //The radio background uses 160 palettes.
#define RADIO_BITMAP_PALETTE_BG					(RADIO_BITMAP_PALETTE_INDEX_START+158)

//#define RADIO_HBUTTON_PALETTE_INDEX_START		40 //The highlight button uses 16 palettes.
//#define RADIO_HBUTTON_PALETTE_BG					RADIO_HBUTTON_PALETTE_INDEX_START

//#define RADIO_NBUTTON_PALETTE_INDEX_START		60 //The normal button uses 16 palettes.
//#define RADIO_NBUTTON_PALETTE_BG					(RADIO_NBUTTON_PALETTE_INDEX_START+15)

#define RADIO_PALETTE_ENTRY_NORMAL_BUTTON_BEGIN					50
#define RADIO_PALETTE_ENTRY_NORMAL_BUTTON_RESERVE_NUM			15
#define RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BEGIN				(RADIO_PALETTE_ENTRY_NORMAL_BUTTON_BEGIN+RADIO_PALETTE_ENTRY_NORMAL_BUTTON_RESERVE_NUM)
#define RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_RESERVE_NUM		15

#define RADIO_FRAME_ENTRY_START					GDI_GENERAL_UI_PALETTE_INDEX_START 

#define RADIO_PALETTE_ENTRY_HIGHLIGHT_BG					(RADIO_FRAME_ENTRY_START)
#define RADIO_PALETTE_ENTRY_TEXT_BLACK					(RADIO_PALETTE_ENTRY_HIGHLIGHT_BG+1)
#define RADIO_PALETTE_ENTRY_TEXT_WHITE					(RADIO_PALETTE_ENTRY_TEXT_BLACK+1)
#define RADIO_PALETTE_ENTRY_TEXT_GRAY						(RADIO_PALETTE_ENTRY_TEXT_WHITE+1)
#define RADIO_PALETTE_ENTRY_TEXT_NAVYBLUE				(RADIO_PALETTE_ENTRY_TEXT_GRAY+1)	
#define RADIO_PALETTE_ENTRY_FREQ_BAR_HIGHLIGHT			(RADIO_PALETTE_ENTRY_TEXT_NAVYBLUE+1)
#define RADIO_PALETTE_ENTRY_FREQ_BAR_NORMAL				(RADIO_PALETTE_ENTRY_FREQ_BAR_HIGHLIGHT+1)
#define RADIO_PALETTE_ENTRY_FREQ_FRAME_BG				(RADIO_PALETTE_ENTRY_FREQ_BAR_NORMAL+1)
#define RADIO_PALETTE_ENTRY_FREQ_FRAME_BG_OUTLINE		(RADIO_PALETTE_ENTRY_FREQ_FRAME_BG+1)
#define RADIO_PALETTE_ENTRY_NBUTTON_OUTLINE				(RADIO_PALETTE_ENTRY_FREQ_FRAME_BG_OUTLINE+1)
#define RADIO_PALETTE_ENTRY_HBUTTON_OUTLINE				(RADIO_PALETTE_ENTRY_NBUTTON_OUTLINE+1)

#define RADIO_PALETTE_COLOR_HIGHLIGHT_BG					0xFFD900
#define RADIO_PALETTE_COLOR_TEXT_BLACK					0x000000
#define RADIO_PALETTE_COLOR_TEXT_WHITE					0xFFFFFF
#define RADIO_PALETTE_COLOR_TEXT_GRAY					0x696969
#define RADIO_PALETTE_COLOR_TEXT_NAVYBLUE				0x15217B	
#define RADIO_PALETTE_COLOR_FREQ_BAR_HIGHLIGHT			0xD90000
#define RADIO_PALETTE_COLOR_FREQ_BAR_NORMAL				0xDAD9F2
#define RADIO_PALETTE_COLOR_FREQ_FRAME_BG				0xB6B6B6
#define RADIO_PALETTE_COLOR_FREQ_FRAME_BG_OUTLINE		0x7E7E7E
#define RADIO_PALETTE_COLOR_NBUTTON_OUTLINE				0x1FA1D6
#define RADIO_PALETTE_COLOR_HBUTTON_OUTLINE				0xE38813

typedef enum tagRADIO_NORMAL_BUTTON_PALETTE
{
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_0 = RADIO_PALETTE_ENTRY_NORMAL_BUTTON_BEGIN,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_1,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_2,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_3,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_4,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_5,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_6,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_7,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_8,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_9,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_10,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_11,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_12,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_13,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_14
} RADIO_NORMAL_BUTTON_PALETTE;

typedef enum tagRADIO_HIGHLIGHT_BUTTON_PALETTE
{
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_0 = RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BEGIN,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_1,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_2,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_3,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_4,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_5,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_6,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_7,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_8,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_9,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_10,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_11,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_12,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_13,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_14
} RADIO_HIGHLIGHT_BUTTON_PALETTE;

//////////////////////////////////////////////////////////////////////////////////////////
//Export functions
void RADIO_Entry(BYTE bRadioMode);
BYTE RADIO_ProcessKey(void);
BYTE RADIO_Recover(BYTE bRecoverRegion);
void RADIO_Trigger(void);
void RADIO_Exit(BYTE bExitMode);
BYTE RADIO_Initial(BYTE bPowerUp);
void RADIO_ShowFrame(void);
void RADIO_Scan(void);
void RADIO_ClearRegion(BYTE bRegion);
void RADIO_ConfigOSDRegion(void);

//Internal functions
void _RADIO_NormalModeProcess(BYTE bKey);
void _RADIO_FavoriteModeProcess(BYTE bKey);
void _RADIO_RecordModeProcess(BYTE bKey);
void _RADIO_ModeSelectionProcess(BYTE bKey);
void _RADIO_InitPalette(void);
void _RADIO_ReadData(void);
void _RADIO_DrawFrameBackground(void);
void _RADIO_DrawFreqFrame(void);
void _RADIO_ShowModeButton(void);
void _RADIO_UpdateContent(BYTE bRegion, BYTE bClear, BYTE bArrowType);
void _RADIO_UpdateTitle(BYTE bClear);
void _RADIO_UpdateFreqString(BYTE bClear);
void _RADIO_UpdateFrequencyBar(BYTE bClear, BYTE bArrowType);
void _RADIO_UpdateInfoRegion(BYTE bClear, BYTE bDrawBG, BYTE bListBGType);
#ifdef SUPPORT_RADIO_RDS
void _RADIO_UpdateRDSInfo(BYTE bDrawBG);
void _RADIO_ResetRDS(BYTE bClear);
#endif
void _RADIO_UpdateRecordInfo(BYTE bDrawBG, BYTE bListBGType, BYTE bTextType);
void _RADIO_UpdateScanInfo(BYTE bDrawBG);
void _RADIO_ChangeMode(void);
void _RADIO_GotoUpButton(void);
void _RADIO_GotoDownButton(void);
//void _RADIO_GotoRightButton(void);
//void _RADIO_GotoLeftButton(void);
void _RADIO_MoveButtonCursor(BYTE bNewCursorIndex);
void _RADIO_UpdateButtonCursor(BYTE bCursorIndex, BYTE bClear, BYTE bTextType);
void _RADIO_VolumeControl(BYTE bKey);
BYTE _RADIO_LoadBmpData(void);

//////////////////////////////////////////////////////////////////////////////////////////
// The radio mode structure
typedef struct tagRADIO_MODE_BUTTON
{
    WORD *pwText;       
    WORD wStartH;
    WORD wStartV;
    BYTE bMode;
} RADIO_MODE_BUTTON, *PRADIO_MODE_BUTTON;

extern BYTE __bPlayRadio;

#ifdef RADIO_MAIN_FILE

BYTE _bRADIONormalButtonPalette[RADIO_BUTTON_HEIGHT] = 
{
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_8, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_8, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_8, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_8,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_14, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_14,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_12,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_11,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_9,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_6,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_4,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_3,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_2, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_2,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_1, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_1, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_1,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_2, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_2,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_3, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_3,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_5,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_6,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_9,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_10, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_10,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_11,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_12, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_12,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_13, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_13,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_14, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_14, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_14, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_14, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_14, RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_14,
    RADIO_PALETTE_ENTRY_NORMAL_BUTTON_PALETTE_12    
};
    
BYTE _bRADIOHighlightButtonPalette[RADIO_BUTTON_HEIGHT] = 
{
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_9, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_9, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_9,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_5, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_5,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_4,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_3,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_2,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_1,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_0,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_13,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_12, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_12,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_13, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_13, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_13, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_13, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_13, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_13,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_0,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_1, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_1,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_2,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_3,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_4, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_4, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_4,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_5, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_5,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_6, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_6,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_7, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_7, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_7, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_7, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_7, RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_7,
    RADIO_PALETTE_ENTRY_HIGHLIGHT_BUTTON_PALETTE_6
};

DWORD _dwRADIONormalButtonPalette[] =
{
    15,
    0x3CA0CC,
    0x0792CD,
    0x0996CF,
    0x0B9AD3,
    0x139ED6,
    0x1A9FD5,
    0x1FA1D6,
    0x279CCC,
    0xA5C8D6,
    0x27A5D6,
    0x35A9D6,
    0x42AED6,
    0x53B3D6,
    0x67B8D6,
    0x7ABDD6
};

DWORD _dwRADIOHighlightButtonPalette[] =
{
    15,
    0xE1850E,
    0xE68F1C,
    0xE69328,
    0xE69933,
    0xE69D3E,
    0xE6A551,
    0xE6B06A,
    0xE6B87C,
    0xE6BE88,
    0xE6C394,
    0xE6CFAF,
    0xE6DACA,
    0xDE820C,
    0xE38813,
    0xD3741A
};

#endif //#ifdef RADIO_MAIN_FILE
#endif //#ifdef SUPPORT_RADIO_TUNER
#endif //#ifndef __FMRADIO_H__
