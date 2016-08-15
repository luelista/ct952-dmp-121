//**************************************************************************
//         Copyright (c) 2004, Cheertek Inc . All rights reserved.
//         D300, all right reserved.

//	Product	: WinDVD Firmware

//	Date	: 2004.8.20
//	Author	: Cheertek (D300 CoCo Chiang)
//	Purpose	: This is the UI for Digest.
//	Sources	: OSDDG.c/OSDDG.h
//***************************************************************************

#ifndef __OSDDG_H__
#define __OSDDG_H__

//Define the mix ratio of the background.
#define OSDDG_DLG_MIX_RATIO_BACKGROUND  28

//Define the number of buttons, refer to the Digest Module?
#define OSDDG_DLG_BUTTON_NUM            2

//Define the number of buttons in Digest main menu
#define OSDDG_BUTTON_NUM                2

//Define the font's height
#define OSDDG_FONT_HEIGHT               26

//Define the time (secc.) in the frame          
#define OSDDG_TIME_INTERVAL             5

//Define the number of the columns
#define OSDDG_COLUMN_NUM                3

//Define the value of the arc for background
#define OSDDG_DLG_ARC_VALUE                     15 //Arc can be 13, 15, 17, 19, 21, 23, 25.

//Define the value of the arc for background
#define OSDDG_DLG_OUTER_ARC_VALUE               21 //Arc can be 13, 15, 17, 19, 21, 23, 25.

//define the pixels between the outer and inner arc of the background
#define OSDDG_DLG_OUTER_INNER_ARC_PIXEL         (OSDDG_DLG_OUTER_ARC_VALUE-OSDDG_DLG_ARC_VALUE)

//Define the value of the arc for background
#define OSDDG_BUTTON_ARC_VALUE                  13 //Arc can be 13, 15, 17, 19, 21, 23, 25.

//Define the value of the arc for background
#define OSDDG_BUTTON_OUTER_ARC_VALUE            15 //Arc can be 13, 15, 17, 19, 21, 23, 25.

//define the pixels between the outer and inner arc of the background
#define OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL      (OSDDG_BUTTON_OUTER_ARC_VALUE-OSDDG_BUTTON_ARC_VALUE)

//Define the value of the arc for background
#define OSDDG_INFO_REGION_ARC_VALUE             13 //Arc can be 13, 15, 17, 19, 21, 23, 25.

//Define the value of the arc for background
#define OSDDG_INFO_REGION_OUTER_ARC_VALUE       15 //Arc can be 13, 15, 17, 19, 21, 23, 25.

//define the pixels between the outer and inner arc of the background
#define OSDDG_INFO_REGION_OUTER_INNER_ARC_PIXEL (OSDDG_INFO_REGION_OUTER_ARC_VALUE-OSDDG_INFO_REGION_ARC_VALUE)
     
//***************************************************************************
//The message what will be used in OSDDG_Update()
//***************************************************************************
#define OSDDG_MSG_DLG_INITIAL           0
#define OSDDG_MSG_CLEAR_HIGHLIGHT       1
#define OSDDG_MSG_UPDATE_HIGHLIGHT      2
#define OSDDG_MSG_DLG_EXIT              3
#define OSDDG_MSG_INITIAL               4
#define OSDDG_MSG_DRAW_NORMAL_FRAME     5
#define OSDDG_MSG_DRAW_SELECT_FRAME     6
#define OSDDG_MSG_UPDATE_FRAME_TIME     7
#define OSDDG_MSG_CLEAR_ALL_FRAME       8
#define OSDDG_MSG_UPDATE_UI_INFO        9
#define OSDDG_MSG_CHANGE_TV_MODE        10
#define OSDDG_MSG_EXIT                  11
#define OSDDG_MSG_CHANGE_PAGE           12

//***************************************************************************
//The parameters
//***************************************************************************
//The parameter for draw/clear the background, highlight.
#define OSDDG_DRAW                          0
#define OSDDG_CLEAR                         1

//The parameter for updating highlight
#define OSDDG_UPDATE_HIGHLIGHT_DRAW         0
#define OSDDG_UPDATE_HIGHLIGHT_CLEAR        1

//The parameter for drawing the frame
#define OSDDG_FRAME_NORMAL                  0
#define OSDDG_FRAME_SELECT                  1

//define the type of tunning position for PAL
#define OSDDG_TUNE_POS_TYPE_TITLE           0
#define OSDDG_TUNE_POS_TYPE_BUTTON          1
#define OSDDG_TUNE_POS_TYPE_HELP            2

//***************************************************************************
//The Digest UI's position
//***************************************************************************
//define the button of the dialog box.
#define OSDDG_DLG_BUTTON_TRACK_DIGEST           0
#define OSDDG_DLG_BUTTON_BUTTON_DISC_INTERVAL   1

//Define the attributes of the dialog box.
#define OSDDG_DLG_BACKGROUND_BUTTON_DISTANCE_H    16
#define OSDDG_DLG_BACKGROUND_BUTTON_DISTANCE_V    32
#define OSDDG_DLG_BUTTON_BUTTON_DISTANCE_V        16  
#define OSDDG_DLG_BUTTON_TEXT_DISTANCE_H          8
#define OSDDG_DLG_BUTTON_HEIGHT                   30
  
//The position of the title
#define OSDDG_TITLE_START_H                 16
#define OSDDG_TITLE_START_V                 0
#define OSDDG_TITLE_WIDTH                   320        

//The position of the page info.
#define OSDDG_PAGE_INFO_END_H               600
#define OSDDG_PAGE_INFO_START_V             OSDDG_TITLE_START_V

//The position of the info. region
#define OSDDG_INFO_REGION_START_H           OSDDG_TITLE_START_H
#define OSDDG_INFO_REGION_START_V           360
#define OSDDG_INFO_REGION_WIDTH             300
#define OSDDG_INFO_REGION_HEIGHT            26
#define OSDDG_INFO_REGION_STRING_START_H    (OSDDG_INFO_REGION_START_H+16)          
      
//The distance between the info. region and the button
#define OSDDG_INFO_REGION_BUTTON_DISTANCE   24

//The position of the button
#define OSDDG_BUTTON_START_H                (OSDDG_INFO_REGION_START_H+OSDDG_INFO_REGION_WIDTH+OSDDG_INFO_REGION_BUTTON_DISTANCE)
#define OSDDG_BUTTON_START_V                OSDDG_INFO_REGION_START_V
#define OSDDG_BUTTON_WIDTH                  100
#define OSDDG_BUTTON_HEIGHT                 26
#define OSDDG_BUTTON_BUTTON_DISTANCE        24

//Define the attributes for drawing the frame
#define OSDDG_FRAME_THICKNESS               4

//Define the attributes for drawing the info. in the frame.
#define OSDDG_FRAME_INFO_DISTANCE_H         8
#define OSDDG_FRAME_INFO_DISTANCE_V         8
#define OSDDG_FRAME_INFO_NUM_TIME_DISTANCE  8

//Tune the OSD to meet the video.
#define OSDDG_TUNE_WIDTH_NTSC               2
#define OSDDG_TUNE_HEIGHT_NTSC              2
#define OSDDG_TUNE_WIDTH_PAL                2
#define OSDDG_TUNE_HEIGHT_PAL               16
#define OSDDG_TUNE_DISTANCE_HEIGHT_PAL      16

//Adjust the position for VDS and HDS is changed to center the video
#define OSDDG_TUNE_FRAME_H_NTSC             66
#define OSDDG_TUNE_FRAME_V_NTSC             18
#define OSDDG_TUNE_FRAME_H_PAL              72
#define OSDDG_TUNE_FRAME_V_PAL              28 

//PSCAN is different from Interlace
#define OSDDG_TUNE_FRAME_H_NTSC_PSCAN       68
#define OSDDG_TUNE_FRAME_V_NTSC_PSCAN       18
#define OSDDG_TUNE_FRAME_H_PAL_PSCAN        68
#define OSDDG_TUNE_FRAME_V_PAL_PSCAN        30

//define the position of tunning title in PAL mode.
#define OSDDG_TUNE_TITLE_START_V_PAL        0
#define OSDDG_TUNE_BUTTON_START_V_PAL       50
#define OSDDG_TUNE_HELP_START_V_PAL         50

//***************************************************************************
//Palette definition
//***************************************************************************
#define OSDDG_PALETTE_ENTRY_START                           GDI_GENERAL_UI_PALETTE_INDEX_START
#define OSDDG_PALETTE_ENTRY_DLG_BACKGROUND                  OSDDG_PALETTE_ENTRY_START
#define OSDDG_PALETTE_ENTRY_TITLE_STRING                    (OSDDG_PALETTE_ENTRY_START+1)
#define OSDDG_PALETTE_ENTRY_HELP_STRING                     (OSDDG_PALETTE_ENTRY_START+2)
#define OSDDG_PALETTE_ENTRY_ENABLE_BUTTON_STRING            (OSDDG_PALETTE_ENTRY_START+3)
#define OSDDG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_STRING         (OSDDG_PALETTE_ENTRY_START+4)
#define OSDDG_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT             (OSDDG_PALETTE_ENTRY_START+5)
#define OSDDG_PALETTE_ENTRY_NORMAL_FRAME_DARK               (OSDDG_PALETTE_ENTRY_START+6)
#define OSDDG_PALETTE_ENTRY_SELECT_FRAME_BRIGHT             (OSDDG_PALETTE_ENTRY_START+7)
#define OSDDG_PALETTE_ENTRY_SELECT_FRAME_DARK               (OSDDG_PALETTE_ENTRY_START+8)
#define OSDDG_PALETTE_ENTRY_DLG_ENABLE_BUTTON_BACKGROUND    (OSDDG_PALETTE_ENTRY_START+9)
#define OSDDG_PALETTE_ENTRY_DLG_DISABLE_BUTTON_BACKGROUND   (OSDDG_PALETTE_ENTRY_START+10)
#define OSDDG_PALETTE_ENTRY_DLG_HIGHLIGHT_BUTTON_BACKGROUND (OSDDG_PALETTE_ENTRY_START+11)
#define OSDDG_PALETTE_ENTRY_DLG_ENABLE_BUTTON_STRING        (OSDDG_PALETTE_ENTRY_START+12)
#define OSDDG_PALETTE_ENTRY_DLG_DISABLE_BUTTON_STRING       (OSDDG_PALETTE_ENTRY_START+13)
#define OSDDG_PALETTE_ENTRY_DLG_HIGHLIGHT_BUTTON_STRING     (OSDDG_PALETTE_ENTRY_START+14)          
#define OSDDG_PALETTE_ENTRY_FRAME_INFO_BACKGROUND           (OSDDG_PALETTE_ENTRY_START+15)
#define OSDDG_PALETTE_ENTRY_FRAME_INFO_STRING               (OSDDG_PALETTE_ENTRY_START+16)
#define OSDDG_PALETTE_ENTRY_INFO_REGION_BACKGROUND          (OSDDG_PALETTE_ENTRY_START+17)
#define OSDDG_PALETTE_ENTRY_ENABLE_BUTTON_BACKGROUND        (OSDDG_PALETTE_ENTRY_START+18) //Will be removed if use Bitmap button
#define OSDDG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BACKGROUND     (OSDDG_PALETTE_ENTRY_START+19) //Will be removed if use Bitmap button
#define OSDDG_PALETTE_ENTRY_BUTTON_FRAME                    (OSDDG_PALETTE_ENTRY_START+20)
#define OSDDG_PALETTE_ENTRY_DLG_FRAME                       (OSDDG_PALETTE_ENTRY_START+21)
#define OSDDG_PALETTE_ENTRY_INFO_REGION_FRAME               (OSDDG_PALETTE_ENTRY_START+22)
      
#define OSDDG_PALETTE_COLOR_DLG_BACKGROUND_1                    0x0096E1
#define OSDDG_PALETTE_COLOR_DLG_BACKGROUND_2                    0x8CD9D7
#define OSDDG_PALETTE_COLOR_DLG_BACKGROUND_3                    0xA0B2AA
#define OSDDG_PALETTE_COLOR_DLG_BACKGROUND_4                    0xDECC38
#define OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_BACKGROUND_1      0xA1E5FF
#define OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_BACKGROUND_2      0x78E8A8
#define OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_BACKGROUND_3      0x91B3C0
#define OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_BACKGROUND_4      0x9F975C
#define OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_BACKGROUND_1   0xFAD578    
#define OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_BACKGROUND_2   0xFFFE00    
#define OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_BACKGROUND_3   0x40C42F    
#define OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_BACKGROUND_4   0x5E5BDC    
#define OSDDG_PALETTE_COLOR_DLG_FRAME_1                         0x0060E1
#define OSDDG_PALETTE_COLOR_DLG_FRAME_2                         0x8CF5D7
#define OSDDG_PALETTE_COLOR_DLG_FRAME_3                         0x8C9696
#define OSDDG_PALETTE_COLOR_DLG_FRAME_4                         0x0060E1
#define OSDDG_PALETTE_COLOR_INFO_REGION_BACKGROUND_1            0xC1C1E9
#define OSDDG_PALETTE_COLOR_INFO_REGION_BACKGROUND_2            0x8AF3AC
#define OSDDG_PALETTE_COLOR_INFO_REGION_BACKGROUND_3            0x759EEA
#define OSDDG_PALETTE_COLOR_INFO_REGION_BACKGROUND_4            0x6AD3E4
#define OSDDG_PALETTE_COLOR_ENABLE_BUTTON_BACKGROUND_1          0xD0D0F7
#define OSDDG_PALETTE_COLOR_ENABLE_BUTTON_BACKGROUND_2          0x31B95B
#define OSDDG_PALETTE_COLOR_ENABLE_BUTTON_BACKGROUND_3          0x6E73E5
#define OSDDG_PALETTE_COLOR_ENABLE_BUTTON_BACKGROUND_4          0x43DCB2
#define OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_BACKGROUND_1       0xFFDD66
#define OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_BACKGROUND_2       0x8985DF
#define OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_BACKGROUND_3       0xF3E328
#define OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_BACKGROUND_4       0x359C4D


#define OSDDG_PALETTE_COLOR_TITLE_STRING                    0xC8C8C8
#define OSDDG_PALETTE_COLOR_HELP_STRING                     0x010101
#define OSDDG_PALETTE_COLOR_ENABLE_BUTTON_STRING            0x010101
#define OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_STRING         0x010101
#define OSDDG_PALETTE_COLOR_NORMAL_FRAME_BRIGHT             0xE1E1E1
#define OSDDG_PALETTE_COLOR_NORMAL_FRAME_DARK               0xA3A3A3
#define OSDDG_PALETTE_COLOR_SELECT_FRAME_BRIGHT             0xFAC14E
#define OSDDG_PALETTE_COLOR_SELECT_FRAME_DARK               0x836527
#define OSDDG_PALETTE_COLOR_DLG_DISABLE_BUTTON_BACKGROUND   0xB0B0B0
#define OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_STRING        0x010101
#define OSDDG_PALETTE_COLOR_DLG_DISABLE_BUTTON_STRING       0xC8C8C8
#define OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_STRING     0x010101
#define OSDDG_PALETTE_COLOR_FRAME_INFO_BACKGROUND           0x202020
#define OSDDG_PALETTE_COLOR_FRAME_INFO_STRING               0xC8C8C8
#define OSDDG_PALETTE_COLOR_INFO_REGION_BACKGROUND          0x9D9CBA
#define OSDDG_PALETTE_COLOR_BUTTON_FRAME                    0xFFFFFF
#define OSDDG_PALETTE_COLOR_INFO_REGION_FRAME               0xFFFFFF

//***************************************************************************
//Export functions
//***************************************************************************
void OSDDG_Update(BYTE bMessage);
BYTE OSDDG_Recover(BYTE bRecoverRegion);
void OSDDG_ClearRegion(BYTE bClearRegion);

//***************************************************************************
//Internal functions
//***************************************************************************
void _OSDDG_InitializeGDI(void);
void _OSDDG_DLG_Initial(BYTE bRecover);
void _OSDDG_DLG_UpdateBackground(BYTE bUpdate);
void _OSDDG_DLG_DrawButton(BYTE bIndex, BYTE bHightlight);
WORD _OSDDG_DLG_GetDlgHeight(void);
void _OSDDG_DLG_FillButtonString(BYTE bIndex);
WORD _OSDDG_DLG_GetMaxButtonWidth(void);
void _OSDDG_Initial(void);
void _OSDDG_DisplayTitle(void);
void _OSDDG_DisplayPageInfo(void);
void _OSDDG_DrawInfoRegion(void);
void _OSDDG_DrawButton(BYTE bIndex, BYTE bHighlight);
void _OSDDG_InitializeStringTransparentAttr(void);
void _OSDDG_DrawFrame(BYTE bIndex, BYTE bUpdate);
void _OSDDG_ClearFrameRegion(void);
void _OSDDG_Exit(void);
void _OSDDG_ClearPageInfoRegion(void);
void _OSDDG_UpdateInfoRegion(void);
void _OSDDG_RecoverAll(void);
void _OSDDG_UpdteFrameInfo(BYTE bIndex);
void _OSDDG_UpdateHighlight(BYTE bUpdate);
void _OSDDG_ChangeTVMode(void);
void _OSDDG_TuneFramePosition(void);
void _OSDDG_DrawAllFrames(void);
BYTE _OSDDG_TuneForTVType(BYTE bType);
void _OSDDG_InitializeNDAttribute(void);

#endif //__OSDDG_H__
