//**************************************************************************
//         Copyright (c) 2004, Cheertek Inc . All rights reserved.
//         D300, all right reserved.

//	Product	: WinDVD Firmware

//	Date	: 2004.6.29
//	Author	: Cheertek (D300 CoCo Chiang)
//	Purpose	: Provide the UI for thumbnail.
//	Sources	: Thumb.c
//***************************************************************************
#ifndef __THUMB_H__
#define __THUMB_H__

#ifndef CT950_STYLE
#define SUPPORT_THUMB_PROGRAM
#endif //CT950_STYLE

//Open this define to use OSD to draw thumbnail's frame
//#define THUMB_DRAW_OSD_FRAME

//Open this define to support round highlight.
#define THUMB_ROUND_HIGHLIGHT       

//Define the interval of time to flash the current selection
#define THUMB_SELECT_FRAME_FLASH_INTERVAL   COUNT_300_MSEC

//define the color of the background
//#define THUMB_BACKGROUND_COLOR              0x22d472 //The color of the frame buffer. (YUV)
#define THUMB_BACKGROUND_COLOR              0x000000 //The color of the frame buffer. (RGB)

//define the number of palette for the background
#define THUMB_BACKGROUND_PALETTE_NUM        50

//Note: The following color is RGB not YUV
//define the color for thumbnail frame
#define THUMB_FRAME_NORMAL_COLOR            0xE1E1E1

#define THUMB_FRAME_SELECT_COLOR_1          0xFFFF00
#define THUMB_FRAME_SELECT_COLOR_2          0x8985DF
#define THUMB_FRAME_SELECT_COLOR_3          0xF3E328
#define THUMB_FRAME_SELECT_COLOR_4          0x359C4D

#define THUMB_FRAME_PROGRAM_COLOR           0xA52100

//define the number of rows in a page
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16) 
#define THUMB_ROW_NUM                       3
#else
#define THUMB_ROW_NUM                       4
#endif //DRAM_CONFIGURATION_TYPE_DVD

//define the number of columns in a page
#define THUMB_COLUMN_NUM                    5

//define the number of thumbnails in a page
#define THUMB_TOTAL_NUM_IN_PAGE             (THUMB_ROW_NUM*THUMB_COLUMN_NUM)

//define the height of font or any GDI object
#define THUMB_TITLE_FONT_HEIGHT             26
#define THUMB_HELP_FONT_HEIGHT              26

//define the value of the arc of the button
#define THUMB_BUTTON_ARC_VALUE              13

//define the value of the arc of the button
#define THUMB_BUTTON_OUTER_ARC_VALUE        15

//define the pixels between the outer and inner arc of the background
#define THUMB_BUTTON_OUTER_INNER_ARC_PIXEL  (THUMB_BUTTON_OUTER_ARC_VALUE-THUMB_BUTTON_ARC_VALUE)

//define the value of the arc of the help region
#define THUMB_HELP_REGION_ARC_VALUE         13

//define the value of the arc of the help region
#define THUMB_HELP_REGION_OUTER_ARC_VALUE   15

//define the pixels between the outer and inner arc of the background
#define THUMB_HELP_REGION_OUTER_INNER_ARC_PIXEL    (THUMB_HELP_REGION_OUTER_ARC_VALUE-THUMB_HELP_REGION_ARC_VALUE)

//define the type of highlight
#define THUMB_UPDATE_HIGHLIGHT_DRAW         0
#define THUMB_UPDATE_HIGHLIGHT_CLEAR        1

//define maximum decoding time
#define THUMB_MAX_DECODE_TIME               (COUNT_10_SEC*6)

//define maximum "parse header" time
#define THUMB_MAX_PARSEHEADER_TIME          COUNT_5_SEC

//define the maximum times to re-decode a thumbnail when decoding error
#define THUMB_REDECODE_TIMES                3

//define the maximum times to check hang
#define THUMB_CHECK_HANG_TIMES              20

//parameters for changing a page
#define THUMB_NEXT_PAGE                     0
#define THUMB_PREVIOUS_PAGE                 1
#define THUMB_CURRENT_PAGE                  2

//Decoding stage
#define THUMB_STAGE_START                   0
#define THUMB_STAGE_PARSEHEADER             1
#define THUMB_STAGE_DECODE                  2             

//parameters for the type of the cursor
#define THUMB_CURSOR_PICTURE                0
#define THUMB_CURSOR_BUTTON                 1

//parameters for sub-mode of thumbnail
#define THUMB_MAIN_MODE_GENERAL             0
#define THUMB_MAIN_MODE_EDIT                1

//parameters for sub-mode of thumbnail  
typedef enum tagTHUMB_MODE_ID_LIST
{
    THUMB_SUBMODE_GENERAL = 0,
    THUMB_SUBMODE_ROTATE,
    THUMB_SUBMODE_PROGRAM,
    THUMB_SUBMODE_PROGRAM_LIST,
#ifndef THUMB_NO_EDIT_MODE
    THUMB_SUBMODE_EDIT_MODE,
#endif
    THUMB_SUBMODE_DELETE_MODE,
    THUMB_SUBMODE_SORT_MODE,

    //Put the mode here because just for compile issue only
#ifdef THUMB_NO_EDIT_MODE
    THUMB_SUBMODE_EDIT_MODE,
#endif
} THUMB_MODE_ID_LIST;


//parameter for button's state
#define THUMB_BUTTON_STATE_ENABLE           0
#define THUMB_BUTTON_STATE_HIGHLIGHT        1

//define thumbnail's button ID
#define THUMB_BUTTON_ROTATE                 0
#define THUMB_BUTTON_GENERAL_SLIDESHOW      1
#define THUMB_BUTTON_RESET                  2
#define THUMB_BUTTON_ROTATE_RETURN          3
#define THUMB_BUTTON_PROGRAM_SLIDESHOW      4
#define THUMB_BUTTON_PROGRAMLIST            5
#define THUMB_BUTTON_PROGRAMLIST_SLIDESHOW  6
#define THUMB_BUTTON_PROGRAMLIST_RETURN     7
#define THUMB_BUTTON_EDIT_DELETE            8
#define THUMB_BUTTON_EDIT_SORT              9
#define THUMB_BUTTON_DELETE_DELETEALL       10    
#define THUMB_BUTTON_DELETE_RETURN          11
#define THUMB_BUTTON_SORT_RETURN            12
#define THUMB_BUTTON_SORT_PLAY              13
    
//parameters for slide show
#define THUMB_PLAY_START                    0
#define THUMB_PLAY_CURRENT                  1

//parameters for thumbnail decode mode
#define THUMB_DECODE_PAGE                   0
#define THUMB_DECODE_ONE                    1
#define THUMB_DECODE_UNDETERMINED           2

//parameters for drawing thumbnail's frame
#define THUMB_FRAME_NORMAL                  0
#define THUMB_FRAME_SELECT                  1
#define THUMB_FRAME_PROGRAM                 2
#define THUMB_FRAME_CLEAR                   3

//defint the update for the frame
#define THUMB_UPDATE_FRAME_GENERAL_TO_PROGRAM           0
#define THUMB_UPDATE_FRAME_PROGRAM_TO_GENERAL           1

//define the type of clearing
#define THUMB_CLEAR_TITLE_REGION                        0
#define THUMB_CLEAR_PAGE_INFO_REGION                    1
#define THUMB_CLEAR_HELP_REGION                         2
#define THUMB_CLEAR_BUTTON_REGION                       3
#define THUMB_CLEAR_GENERAL_MODE_FRAME_REGION           4
#define THUMB_CLEAR_RECOVER_UPPER_REGION                5
#define THUMB_CLEAR_RECOVER_BOTTOM_REGION               6

//define the type of tunning position for PAL
#define THUMB_TUNE_POS_TYPE_TITLE                       0
#define THUMB_TUNE_POS_TYPE_BUTTON                      1
#define THUMB_TUNE_POS_TYPE_HELP                        2

//***************************************************************************
//The following define is for UI-relative attributes.
//***************************************************************************
//define the OSD DRAM configuration
#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_16)
#define THUMB_OSD_REGION_WIDTH						616
#define THUMB_OSD_REGION_HEIGHT                     420
#else
#define THUMB_OSD_REGION_WIDTH						GDI_REGION_WIDTH
#define THUMB_OSD_REGION_HEIGHT                     440
#endif

//define the thickness of the thumbnail's frame
#define THUMB_FRAME_THICKNESS               4
#define THUMB_HIGHLIGHT_FRAME_THICKNESS     4//6

//Define the thumbnail's size and position.
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16) 

#define THUMB_WIDTH                         96
#define THUMB_HEIGHT_NTSC                   64 //to be modified
#define THUMB_HEIGHT_PAL                    80
#define THUMB_DISTANCE_WIDTH                16 //the distance between each thumbnail
#define THUMB_DISTANCE_HEIGHT_NTSC          16
#ifdef JPEG_SINGLE_BUFFER_FULL_SCREEN
#define THUMB_H_REGION                      88 //the region between left side of the buffer and thumbnail
#else
#define THUMB_H_REGION                      46 //the region between left side of the buffer and thumbnail
#endif //JPEG_SINGLE_BUFFER_FULL_SCREEN
#define THUMB_V_REGION_NTSC                 56 //66 //the region between top side of the buffer and thumbnail

#define THUMB_TUNE_DISPLAY_WIDTH_NTSC       2
#define THUMB_TUNE_DISPLAY_HEIGHT_NTSC      6

//define the width and height should be tunned for PAL.
#define THUMB_TUNE_DISPLAY_WIDTH_PAL        2//0
#define THUMB_TUNE_DISPLAY_HEIGHT_PAL       18//16
#define THUMB_TUNE_DISTANCE_HEIGHT_PAL      16

//adjust the position for VDS and HDS is changed to center the video
#ifdef JPEG_SINGLE_BUFFER_FULL_SCREEN
#define THUMB_TUNE_FRAME_H_NTSC             58 //66
#define THUMB_TUNE_FRAME_V_NTSC             0 //18
#define THUMB_TUNE_FRAME_H_PAL              58//40 //72
#define THUMB_TUNE_FRAME_V_PAL              0 
#else
#define THUMB_TUNE_FRAME_H_NTSC             0 //66
#define THUMB_TUNE_FRAME_V_NTSC             0 //18
#define THUMB_TUNE_FRAME_H_PAL              0//40 //72
#define THUMB_TUNE_FRAME_V_PAL              0 
#endif //JPEG_SINGLE_BUFFER_FULL_SCREEN

//PSCAN is different from Interlace
#define THUMB_TUNE_FRAME_H_NTSC_PSCAN       0
#define THUMB_TUNE_FRAME_V_NTSC_PSCAN       0
#define THUMB_TUNE_FRAME_H_PAL_PSCAN        0
#define THUMB_TUNE_FRAME_V_PAL_PSCAN        0

#define THUMB_TUNE_OSD_H_OFFSET_NTSC        32
#define THUMB_TUNE_OSD_V_OFFSET_NTSC        32
#define THUMB_TUNE_OSD_H_OFFSET_PAL         34
#define THUMB_TUNE_OSD_V_OFFSET_PAL         34

#define THUMB_TUNE_OSD_H_DISTANCE_NTSC      12
#define THUMB_TUNE_OSD_V_DISTANCE_NTSC      5
#define THUMB_TUNE_OSD_H_DISTANCE_PAL       10
#define THUMB_TUNE_OSD_V_DISTANCE_PAL       8

#define THUMB_TUNE_OSD_H_OFFSET_NTSC_PSCAN  32
#define THUMB_TUNE_OSD_V_OFFSET_NTSC_PSCAN  32
#define THUMB_TUNE_OSD_H_OFFSET_PAL_PSCAN   38
#define THUMB_TUNE_OSD_V_OFFSET_PAL_PSCAN   30

#define THUMB_TUNE_OSD_H_DISTANCE_NTSC_PSCAN    12
#define THUMB_TUNE_OSD_V_DISTANCE_NTSC_PSCAN    5
#define THUMB_TUNE_OSD_H_DISTANCE_PAL_PSCAN     10
#define THUMB_TUNE_OSD_V_DISTANCE_PAL_PSCAN     10


//define the position of tunning button in PAL mode.
#define THUMB_TUNE_BUTTON_START_V_PAL       56

//define the position of tunning title in PAL mode.
#define THUMB_TUNE_TITLE_START_V_PAL        0

//define the position of tunning help in PAL mode.
#define THUMB_TUNE_HELP_START_V_PAL         THUMB_TUNE_BUTTON_START_V_PAL

#else
#define THUMB_WIDTH                         96
#define THUMB_HEIGHT_NTSC                   64 //to be modified
#define THUMB_HEIGHT_PAL                    80
#define THUMB_DISTANCE_WIDTH                16 //the distance between each thumbnail
#define THUMB_DISTANCE_HEIGHT_NTSC          16
#define THUMB_H_REGION                      88 //the region between left side of the buffer and thumbnail
#define THUMB_V_REGION_NTSC                 56 //66 //the region between top side of the buffer and thumbnail

#define THUMB_TUNE_DISPLAY_WIDTH_NTSC       2
#define THUMB_TUNE_DISPLAY_HEIGHT_NTSC      6

//define the width and height should be tunned for PAL.
#define THUMB_TUNE_DISPLAY_WIDTH_PAL        2//0
#define THUMB_TUNE_DISPLAY_HEIGHT_PAL       18//16
#define THUMB_TUNE_DISTANCE_HEIGHT_PAL      16

//adjust the position for VDS and HDS is changed to center the video
#define THUMB_TUNE_FRAME_H_NTSC             58 //66
#define THUMB_TUNE_FRAME_V_NTSC             30 //18
#define THUMB_TUNE_FRAME_H_PAL              58//40 //72
#define THUMB_TUNE_FRAME_V_PAL              28 

//PSCAN is different from Interlace
#define THUMB_TUNE_FRAME_H_NTSC_PSCAN       58
#define THUMB_TUNE_FRAME_V_NTSC_PSCAN       28
#define THUMB_TUNE_FRAME_H_PAL_PSCAN        60
#define THUMB_TUNE_FRAME_V_PAL_PSCAN        30

//define the position of tunning button in PAL mode.
#define THUMB_TUNE_BUTTON_START_V_PAL       56

//define the position of tunning title in PAL mode.
#define THUMB_TUNE_TITLE_START_V_PAL        0

//define the position of tunning help in PAL mode.
#define THUMB_TUNE_HELP_START_V_PAL         THUMB_TUNE_BUTTON_START_V_PAL

#endif //DRAM_CONFIGURATION_TYPE_DVD


//define the display position for title
#define THUMB_TITLE_START_H         24 //8
#define THUMB_TITLE_START_V         0

//define the display position for page info
#define THUMB_PAGE_START_H          424
#define THUMB_PAGE_END_H            (THUMB_WIDTH * THUMB_COLUMN_NUM + THUMB_DISTANCE_WIDTH * (THUMB_COLUMN_NUM-1) + THUMB_H_REGION-THUMB_TUNE_FRAME_H_NTSC+6)
#define THUMB_PAGE_START_V          THUMB_TITLE_START_V

//define the distance between picture and help
#define THUMB_PICTURE_HELP_DISTANCE         24

//define the display position for help
#define THUMB_HELP_START_H          14
#define THUMB_HELP_START_V          (THUMB_V_REGION_NTSC-THUMB_TUNE_FRAME_V_NTSC+THUMB_HEIGHT_NTSC*THUMB_ROW_NUM+THUMB_DISTANCE_WIDTH*(THUMB_ROW_NUM-1)+THUMB_PICTURE_HELP_DISTANCE)
#define THUMB_HELP_WIDTH            300//316//336
#define THUMB_HELP_HEIGHT           26
#define THUMB_HELP_STRING_REGION_DISTANCE   8
#define THUMB_HELP_STRING_START_H   (THUMB_HELP_START_H+THUMB_HELP_STRING_REGION_DISTANCE)

//define the distance between the button and the help string.
#define THUMB_HELP_BUTTON_DISTANCE          12//16

//The width of the button and the distance between the buttons
#define THUMB_BUTTON_START_H                (THUMB_HELP_START_H+THUMB_HELP_WIDTH+THUMB_HELP_BUTTON_DISTANCE)
#define THUMB_BUTTON_DISTANCE               12//16
#define THUMB_BUTTON_START_V                THUMB_HELP_START_V
#define THUMB_BUTTON_WIDTH                  126 //Need to reserve 16 pixel width for round rectangle. Therefore, Excel set 110
#define THUMB_BUTTON_HEIGHT                 26        
      

//define tge position and size of the background
#define THUMB_BACKGROUND_START_H    0
#define THUMB_BACKGROUND_START_V    0
#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_16)
#define THUMB_BACKGROUND_RIGHT      620
#define THUMB_BACKGROUND_BOTTOM     420
#else
#define THUMB_BACKGROUND_RIGHT      640
#define THUMB_BACKGROUND_BOTTOM     460
#endif

//define the palette entry
#define THUMB_PALETTE_ENTRY_START                           GDI_GENERAL_UI_PALETTE_INDEX_START
#define THUMB_PALETTE_ENTRY_SELECT_FRAME_BRIGHT             THUMB_PALETTE_ENTRY_START
#define THUMB_PALETTE_ENTRY_SELECT_FRAME_DARK               (THUMB_PALETTE_ENTRY_START+1)
#define THUMB_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT             (THUMB_PALETTE_ENTRY_START+2)
#define THUMB_PALETTE_ENTRY_NORMAL_FRAME_DARK               (THUMB_PALETTE_ENTRY_START+3)
#define THUMB_PALETTE_ENTRY_PROGRAM_FRAME_BRIGHT            (THUMB_PALETTE_ENTRY_START+4)
#define THUMB_PALETTE_ENTRY_PROGRAM_FRAME_DARK              (THUMB_PALETTE_ENTRY_START+5)
#define THUMB_PALETTE_ENTRY_TITLE_STRING                    (THUMB_PALETTE_ENTRY_START+6)
#define THUMB_PALETTE_ENTRY_PAGE_STRING                     (THUMB_PALETTE_ENTRY_START+7)
#define THUMB_PALETTE_ENTRY_HELP_STRING                     (THUMB_PALETTE_ENTRY_START+8)
#define THUMB_PALETTE_ENTRY_BUTTON_STRING_ENABLE            (THUMB_PALETTE_ENTRY_START+9)
#define THUMB_PALETTE_ENTRY_BUTTON_STRING_HIGHLIGHT         (THUMB_PALETTE_ENTRY_START+10)
#define THUMB_PALETTE_ENTRY_PL_FILE_EXT                     (THUMB_PALETTE_ENTRY_START+11)
#define THUMB_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND        (THUMB_PALETTE_ENTRY_START+12)
#define THUMB_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND     (THUMB_PALETTE_ENTRY_START+13)
#define THUMB_PALETTE_ENTRY_HELP_REGION_BACKGROUND          (THUMB_PALETTE_ENTRY_START+14)
#define THUMB_PALETTE_ENTRY_MP3_ICON_TEXT_FOREGROUND        (THUMB_PALETTE_ENTRY_START+15)            
#define THUMB_PALETTE_ENTRY_MP3_ICON_SHADOW_1               (THUMB_PALETTE_ENTRY_START+16)                   
#define THUMB_PALETTE_ENTRY_MP3_ICON_SHADOW_2               (THUMB_PALETTE_ENTRY_START+17)                   
#define THUMB_PALETTE_ENTRY_WMA_ICON_TEXT_FOREGROUND        (THUMB_PALETTE_ENTRY_START+18)
#define THUMB_PALETTE_ENTRY_WMA_ICON_SHADOW_1               (THUMB_PALETTE_ENTRY_START+19) 
#define THUMB_PALETTE_ENTRY_WMA_ICON_SHADOW_2               (THUMB_PALETTE_ENTRY_START+20)
#define THUMB_PALETTE_ENTRY_MP4_ICON_TEXT_FOREGROUND        (THUMB_PALETTE_ENTRY_START+21)
#define THUMB_PALETTE_ENTRY_MP4_ICON_SHADOW_1               (THUMB_PALETTE_ENTRY_START+22)
#define THUMB_PALETTE_ENTRY_MP4_ICON_SHADOW_2               (THUMB_PALETTE_ENTRY_START+23)
#define THUMB_PALETTE_ENTRY_MP2_ICON_TEXT_FOREGROUND        (THUMB_PALETTE_ENTRY_START+24)
#define THUMB_PALETTE_ENTRY_MP2_ICON_SHADOW_1               (THUMB_PALETTE_ENTRY_START+25)
#define THUMB_PALETTE_ENTRY_MP2_ICON_SHADOW_2               (THUMB_PALETTE_ENTRY_START+26)
#define THUMB_PALETTE_ENTRY_DAT_ICON_TEXT_FOREGROUND        (THUMB_PALETTE_ENTRY_START+27)
#define THUMB_PALETTE_ENTRY_DAT_ICON_SHADOW_1               (THUMB_PALETTE_ENTRY_START+28)
#define THUMB_PALETTE_ENTRY_DAT_ICON_SHADOW_2               (THUMB_PALETTE_ENTRY_START+29)
#define THUMB_PALETTE_ENTRY_VOB_ICON_TEXT_FOREGROUND        (THUMB_PALETTE_ENTRY_START+30)
#define THUMB_PALETTE_ENTRY_VOB_ICON_SHADOW_1               (THUMB_PALETTE_ENTRY_START+31)
#define THUMB_PALETTE_ENTRY_VOB_ICON_SHADOW_2               (THUMB_PALETTE_ENTRY_START+32)
#define THUMB_PALETTE_ENTRY_BUTTON_FRAME                    (THUMB_PALETTE_ENTRY_START+33)
#define THUMB_PALETTE_ENTRY_HELP_REGION_FRAME               (THUMB_PALETTE_ENTRY_START+34)
#define THUMB_PALETTE_ENTRY_PROGRAM_SYMBOL                  (THUMB_PALETTE_ENTRY_START+35)
#define THUMB_PALETTE_ENTRY_MPG_ICON_TEXT_FOREGROUND        (THUMB_PALETTE_ENTRY_START+36)
#define THUMB_PALETTE_ENTRY_MPG_ICON_SHADOW_1               (THUMB_PALETTE_ENTRY_START+37)
#define THUMB_PALETTE_ENTRY_MPG_ICON_SHADOW_2               (THUMB_PALETTE_ENTRY_START+38)

#define THUMB_PALETTE_COLOR_SELECT_FRAME_BRIGHT             0xFAC14E
#define THUMB_PALETTE_COLOR_SELECT_FRAME_DARK               0x836527
#define THUMB_PALETTE_COLOR_NORMAL_FRAME_BRIGHT             0xE1E1E1
#define THUMB_PALETTE_COLOR_NORMAL_FRAME_DARK               0xA3A3A3
#define THUMB_PALETTE_COLOR_PROGRAM_FRAME_BRIGHT            0xA52100//0x967E2A
#define THUMB_PALETTE_COLOR_PROGRAM_FRAME_DARK              0xA52100//0x815008
#define THUMB_PALETTE_COLOR_HELP_STRING                     0x010101
#define THUMB_PALETTE_COLOR_BUTTON_STRING_ENABLE            0x010101
#define THUMB_PALETTE_COLOR_BUTTON_STRING_HIGHLIGHT         0x010101 
#define THUMB_PALETTE_COLOR_PL_FILE_EXT                     0xFFD900 //0xFFA928

//Different color
#define THUMB_PALETTE_COLOR_TITLE_STRING_1                  0xFFFFFF
#define THUMB_PALETTE_COLOR_TITLE_STRING_2                  0xFFFFFF
#define THUMB_PALETTE_COLOR_TITLE_STRING_3                  0xFFFFFF
#define THUMB_PALETTE_COLOR_TITLE_STRING_4                  0x010101
#define THUMB_PALETTE_COLOR_PAGE_STRING_1                   THUMB_PALETTE_COLOR_TITLE_STRING_1
#define THUMB_PALETTE_COLOR_PAGE_STRING_2                   THUMB_PALETTE_COLOR_TITLE_STRING_2
#define THUMB_PALETTE_COLOR_PAGE_STRING_3                   THUMB_PALETTE_COLOR_TITLE_STRING_3
#define THUMB_PALETTE_COLOR_PAGE_STRING_4                   THUMB_PALETTE_COLOR_TITLE_STRING_4
#define THUMB_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_1      0xD0D0F7
#define THUMB_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_2      0x31B95B//0x6E73E5
#define THUMB_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_3      0x6E73E5//0x31B95B
#define THUMB_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_4      0x43DCB2
#define THUMB_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND_1   0xFFDD66
#define THUMB_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND_2   0x8985DF//0xF3E328
#define THUMB_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND_3   0xF3E328//0x8985DF
#define THUMB_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND_4   0x359C4D
#define THUMB_PALETTE_COLOR_HELP_REGION_BACKGROUND_1        0xC1C1E9
#define THUMB_PALETTE_COLOR_HELP_REGION_BACKGROUND_2        0x8AF3AC//0x759EEA
#define THUMB_PALETTE_COLOR_HELP_REGION_BACKGROUND_3        0x759EEA//0x8AF3AC
#define THUMB_PALETTE_COLOR_HELP_REGION_BACKGROUND_4        0x6AD3E4


#define THUMB_PALETTE_COLOR_MP3_ICON_TEXT_FOREGROUND        0xFFFF00                                
#define THUMB_PALETTE_COLOR_MP3_ICON_SHADOW_1               0xFFFFFF                  
#define THUMB_PALETTE_COLOR_MP3_ICON_SHADOW_2               0x000000    
#define THUMB_PALETTE_COLOR_WMA_ICON_TEXT_FOREGROUND        0x3CFF1E           
#define THUMB_PALETTE_COLOR_WMA_ICON_SHADOW_1               0xFFFFFF
#define THUMB_PALETTE_COLOR_WMA_ICON_SHADOW_2               0x000000
#define THUMB_PALETTE_COLOR_MP2_ICON_TEXT_FOREGROUND        0xE60000           
#define THUMB_PALETTE_COLOR_MP2_ICON_SHADOW_1               0xFFFFFF
#define THUMB_PALETTE_COLOR_MP2_ICON_SHADOW_2               0x000000
#define THUMB_PALETTE_COLOR_DAT_ICON_TEXT_FOREGROUND        0x000000
#define THUMB_PALETTE_COLOR_DAT_ICON_SHADOW_1               0xFFFFFF
#define THUMB_PALETTE_COLOR_DAT_ICON_SHADOW_2               0x00FF00
#define THUMB_PALETTE_COLOR_VOB_ICON_TEXT_FOREGROUND        0x000000
#define THUMB_PALETTE_COLOR_VOB_ICON_SHADOW_1               0xFFFFEA    
#define THUMB_PALETTE_COLOR_VOB_ICON_SHADOW_2               0xFF0000
#define THUMB_PALETTE_COLOR_BUTTON_FRAME                    0xFFFFFF
#define THUMB_PALETTE_COLOR_HELP_REGION_FRAME               0xFFFFFF
#define THUMB_PALETTE_COLOR_PROGRAM_SYMBOL                  0xFFD900
#define THUMB_PALETTE_COLOR_MPG_ICON_TEXT_FOREGROUND        0xFFFFFF
#define THUMB_PALETTE_COLOR_MPG_ICON_SHADOW_1               0x000000
#define THUMB_PALETTE_COLOR_MPG_ICON_SHADOW_2               0xEC0000
#define THUMB_PALETTE_COLOR_MP4_ICON_TEXT_FOREGROUND        0xFFFF00
#define THUMB_PALETTE_COLOR_MP4_ICON_SHADOW_1               0xFFFFFF
#define THUMB_PALETTE_COLOR_MP4_ICON_SHADOW_2               0x000000


//Exported functions
void THUMB_Trigger(void);
BYTE THUMB_ProcessKey(void);
void THUMB_Entry(void);
void THUMB_Exit(BYTE bExitMode);
BYTE THUMB_Recover(BYTE bRecoverRegion);
void THUMB_ClearRegion(BYTE bClearRegion);
void THUMB_NormalExit(void);
void THUMB_UI_Trigger(BYTE bType, DWORD wTime);
void THUMB_ConfigOSDRegion(void);
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM    
void THUMB_ResetRetryTimes(void);
#endif //PRESTOP_BEFORE_SETUP_AND_PROGRAM

#ifdef SUPPORT_JPEG_BACKGROUND
void THUMB_SetBackgroundImage(PCHAR pcName);
#endif //SUPPORT_JPEG_BACKGROUND

//Internal functions
void _THUMB_Initial(void);
void _THUMB_Start(void);
void _THUMB_GetTotalFiles(void);
void _THUMB_InitialStage(void);
void _THUMB_InitializeGDI(void);
void _THUMB_TuneFramePosition(void);
BYTE _THUMB_TuneForTVType(BYTE bType);
BYTE _THUMB_CheckPlayEnd(void);
void _THUMB_UpdateHighlight(BYTE bUpdate);
void _THUMB_GetPosition(BYTE bIndex);
void _THUMB_DrawFrame(BYTE bIndex, BYTE bMode);
void _THUMB_ProcessError(void);
void _THUMB_ChangeToNextPic(void);
void _THUMB_CheckPlayPageBoundary(void);
BYTE _THUMB_CheckPagePlayEnd(void);
void _THUMB_DisplayPageInfo(void);
BYTE _THUMB_SearchNextValidPic(void);
void _THUMB_PlayEnd(void);
BYTE _THUMB_CheckChangePageValid(BYTE bAction);
BYTE _THUMB_ChangePageAction(BYTE bAction);
void _THUMB_ClearScreen(void);
void _THUMB_DrawDefaultBackground(void);
void _THUMB_ClearRegion(BYTE bClearType);
BYTE _THUMB_CheckLeftBoundary(void);
BYTE _THUMB_CheckRightBoundary(void);
BYTE _THUMB_CheckUpBoundary(void);
BYTE _THUMB_CheckDownBoundary(void);
WORD _THUMB_GetButtonPos(BYTE bButtonIndex);
void _THUMB_DrawButton(BYTE bButtonIndex, BYTE bButtonState);
void _THUMB_FillButtonName(BYTE bButtonID);
BYTE _THUMB_ProcessButton(void);
void _THUMB_EnterNewMode(void);
void _THUMB_UpdateTitle(void);
void _THUMB_UpdateHelp(void);
void _THUMB_SetNormalStringAttribute(void);
void _THUMB_ResetRotateState(void);
void _THUMB_UpdateThumbnailFrame(BYTE bType);
BYTE _THUMB_ProcessEnterKey(void);
void _THUMB_ProcessEmptyProgramList(void);
void _THUMB_ClearFileExtInProgramList(BYTE bIndex);
void _THUMB_Rotate(void);
void _THUMB_ToSlideShow(BYTE bType);
void _THUMB_JPEG_PSFB(BYTE bAllRegion, BYTE bIndex);
void _THUMB_ShowFileExtInProgramList(BYTE bIndex);
void _THUMB_Decode(void);
void _THUMB_RecoverAll(void);
void _THUMB_InitializeNDAttribute(void);
void _THUMB_ParseHeader(void);
void _THUMB_GetJPEGInfo(BYTE bIndex);
void _THUMB_GetJPEGFileDate(BYTE bIndex);
void _THUMB_ResetJPEGInfo(void);
void _THUMB_ShowGeneralHelp(BYTE bIndex);
void _THUMB_DrawHelpRegion(void);
void _THUMB_OutputProgramSymbol(BYTE bIndex);
void _THUMB_ClearProgramSymbol(BYTE bIndex);
void _THUMB_DrawString(void);
BYTE _THUMB_ToMMUI(void);
void _THUMB_InitializeDirFile(void);
void _THUMB_CheckDumpFileRecord(BYTE bChangePage);
WORD _THUMB_GetPlayIndex(void);
WORD _THUMB_GetTotalFilesBeforeDirID(WORD wDirID);

#ifdef SUPPORT_DELETE_FILE_IN_THUMB_UI
BYTE _THUMB_SortToPrevious(void);
BYTE _THUMB_SortToNext(void);
#endif //SUPPORT_DELETE_FILE_IN_THUMB_UI

typedef struct  tagJPEG_INFO
{
    WORD wHResolution;
    WORD wVResolution;
    CHAR cDate[11];
	DWORD dwYear;
	DWORD dwMonth;
	DWORD dwDate;
} JPEG_INFO, *PJPEG_INFO;

//Extern variables
extern BYTE __bThumbnailMode;
extern BYTE __bTHUMBMainMode;

#endif //__THUMB_H__
