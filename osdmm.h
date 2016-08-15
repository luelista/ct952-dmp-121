//**************************************************************************
//         Copyright (c) 2003, Cheertek Inc . All rights reserved.
//         D300, all right reserved.

//	Product	: WinDVD Firmware

//	Date	: 2004.6.11
//	Author	: Cheertek (D300 CoCo Chiang)
//	Purpose	: Provide the UI for Media Manager.
//	Sources	: OSDMM.c/OSDMM.h
//***************************************************************************
#ifndef __OSDMM_H__
#define __OSDMM_H__

#define OSDMM_REGION_WIDTH                          616

#define OSDMM_MIX_RATIO_BACKGROUND                  28

//define the maximum items in the file window.
#define OSDMM_MAX_ITEM_IN_FILE_WINDOW               8

//define the maximum DIR in the left file window
#define OSDMM_MAX_DIR_IN_FILE_WINDOW                5

// record the row numbers of filter dialog pop up menu.
#define OSDMM_FILTER_DLG_ROW_NUM                    4
#define OSDMM_FILTER_DLG_CLOUME_NUM                 2

//define the max number of charachters for file string
#define OSDMM_MAX_FILENAME_LENGTH					20 

//define the max number of charachters for file string
#define OSDMM_MAX_DIRNAME_LENGTH					16

//define the time to flash an audio file.
#define OSDMM_FLASH_AUDIO_FILE_TIME                 COUNT_300_MSEC

//define the time to update audio spectrum
#define OSDMM_UPDATE_AUDIO_SPECTRUM_TIME            COUNT_50_MSEC

//define the pixels for tuning the OSD region in PAL mode.
#define OSDMM_TUNE_PAL_REGION_PIXELS                40

//define the shift bits of amplitude for audio spectrum
#define OSDMM_AMPLITUDE_SHIFT_BITS                  12

//define the value of the arc of the background
#define OSDMM_BACKGROUND_ARC_BALUE                  19 //can be 13, 15, 17, 19, 21, 23, 25.

//define the value of the arc of the background
#define OSDMM_BACKGROUND_OUTER_ARC_BALUE            25 //can be 13, 15, 17, 19, 21, 23, 25.

//define the pixels between the outer and inner arc of the background
#define OSDMM_BACKGROUND_OUTER_INNER_ARC_PIXEL      (OSDMM_BACKGROUND_OUTER_ARC_BALUE-OSDMM_BACKGROUND_ARC_BALUE+2)

//define the value of the arc of the info region
#define OSDMM_INFO_REGION_ARC_VALUE                 13

//define the value of the outer arc of the info region
#define OSDMM_INFO_REGION_OUTER_ARC_VALUE           15

//define the pixels between the outer and inner arc of the info region
#define OSDMM_INFO_REGION_OUTER_INNER_ARC_PIXEL     (OSDMM_INFO_REGION_OUTER_ARC_VALUE-OSDMM_INFO_REGION_ARC_VALUE)

//define the value of the arc of the button
#define OSDMM_BUTTON_ARC_VALUE                      13

//define the value of the outer arc of the button
#define OSDMM_BUTTON_OUTER_ARC_VALUE                15

//define the pixels between the outer and inner arc of the button
#define OSDMM_BUTTON_REGION_OUTER_INNER_ARC_PIXEL   (OSDMM_BUTTON_OUTER_ARC_VALUE-OSDMM_BUTTON_ARC_VALUE)

//define the value of the arc of dialog box
#define OSDMM_DIALOG_BOX_ARC_VALUE                  19

//define the value of the outer arc of the dialog box
#define OSDMM_DIALOG_BOX_OUTER_ARC_VALUE            23

//define the pixels between the outer and inner arc of the dialog box
#define OSDMM_DIALOG_BOX_OUTER_INNER_ARC_PIXEL      (OSDMM_DIALOG_BOX_OUTER_ARC_VALUE-OSDMM_DIALOG_BOX_ARC_VALUE)

//define the height for displaying each dir or file
#define OSDMM_ITEM_HEIGHT                           36//40

//define the icon width
#define OSDMM_ICON_WIDTH                            32

//define the icon width for dialog box. (Check box, radio box, etc.)
#define OSDMM_DLG_ICON_WIDTH                        28

//define the height of the font in file window
#define OSDMM_FILE_WINDOW_FONT_HEIGHT               26
#define OSDMM_TITLE_FONT_HEIGHT                     26
#define OSDMM_HELP_FONT_HEIGHT                      26

//define the distance between the first item and the frame
#define OSDMM_START_ITEM_DISTANCE_V                 10

//define the mode for initialize File Manager UI
#define OSDMM_MODE_INITIAL_UI                       0
#define OSDMM_MODE_RECOVER_UI                       1

//Parameters for the type of separate frame
#define OSDMM_HORIZONTAL_SEPARATE_FRAME             0
#define OSDMM_VERTICAL_SEPARATE_FRAME               1

//define the state we want to update highlight
#define OSDMM_UPDATE_DRAW                           0
#define OSDMM_UPDATE_CLEAR                          1

//define the state to change the page
#define OSDMM_CHANGE_PAGE_NEXT                      0
#define OSDMM_CHANGE_PAGE_PREVIOUS                  1
 
//define the file window
#define OSDMM_LEFT_FILE_WINDOW                      0
#define OSDMM_RIGHT_FILE_WINDOW                     1

//define the button's state
#define OSDMM_BUTTON_ENABLE                         0
#define OSDMM_BUTTON_DISABLE                        1
#define OSDMM_BUTTON_HIGHLIGHT                      2


//Define the type of audio spectrum
#define OSDMM_SPECTRUM_LOW                          0
#define OSDMM_SPECTRUM_HIGH                         1

//define the region to be cleared.
#define OSDMM_TITLE_REGION                          0
#define OSDMM_LEFT_FILE_WINDOW_REGION               1
#define OSDMM_RIGHT_FILE_WINDOW_REGION              2
#define OSDMM_RIGHT_SCROLL_BAR_REGION               3
#define OSDMM_LEFT_SCROLL_BAR_REGION                4
#define OSDMM_RECOVER_UPPER_REGION                  5
#define OSDMM_RECOVER_BOTTOM_REGION                 6 
#define OSDMM_TITLE_SUB_REGION                      7
#define OSDMM_LEFT_FILE_WINDOW_CONTENT_REGION       8
#define OSDMM_RIGHT_FILE_WINDOW_CONTENT_REGION      9
#define OSDMM_BUTTON_REGION                         10
#define OSDMM_HELP_REGION                           11
#define OSDMM_PREVIEW_REGION                        12
#define OSDMM_TIME_REGION                           13
#define OSDMM_ALL_REGION                            14
#define OSDMM_DLG_REGION                            15
#define OSDMM_DLG_TITLE_REGION                      16

//define the OSD DRAM configuration
#define OSDMM_OSD_REGION_HEIGHT                     440

//define the vertical starting position of the title
#define OSDMM_TITLE_START_V                         16

//define the width of the title
#define OSDMM_TITLE_PLAYING_DIR_WIDTH               124
#define OSDMM_TITLE_PLAYING_FILE_WIDTH              120 //6 digits+ 1 slash
#define OSDMM_TITLE_PLAYING_DIR_FILE_DISTANCE       12    
#define OSDMM_TITLE_WIDTH                           (OSDMM_ICON_WIDTH*2+OSDMM_TITLE_PLAYING_DIR_WIDTH+OSDMM_TITLE_PLAYING_FILE_WIDTH+OSDMM_TITLE_PLAYING_DIR_FILE_DISTANCE) //344

//define the position and the size of the big frame 
#define OSDMM_BIG_FRAME_THICKNESS                   8
#define OSDMM_BIG_FRAME_H                           16
#define OSDMM_TITLE_BIG_FRAME_DISTANCE_V            4
#define OSDMM_BIG_FRAME_V                           (OSDMM_TITLE_START_V+OSDMM_TITLE_FONT_HEIGHT+OSDMM_TITLE_BIG_FRAME_DISTANCE_V)
#define OSDMM_BIG_FRAME_WIDTH                       584
#define OSDMM_BIG_FRAME_HEIGHT                      (OSDMM_MAX_ITEM_IN_FILE_WINDOW*OSDMM_ITEM_HEIGHT+OSDMM_START_ITEM_DISTANCE_V+OSDMM_BIG_FRAME_THICKNESS*2)   

//define the distance between the normal display and the big frame
#define OSDMM_BIG_FRAME_ND_DISTANCE                 24

//define the horizontal starting position of the title
#define OSDMM_TITLE_START_H                         OSDMM_BIG_FRAME_H

//define the time region to show current audio playing time.
#define OSDMM_TIME_START_H                          (OSDMM_TITLE_START_H+OSDMM_TITLE_WIDTH)
#define OSDMM_TIME_START_V                          OSDMM_TITLE_START_V
#define OSDMM_TIME_WIDTH                            96//112

//define the position of the vertical separate frame
#define OSDMM_VERTICAL_SEPARATE_FRAME_H              264
#define OSDMM_VERTICAL_SEPARATE_FRAME_V              (OSDMM_BIG_FRAME_V + OSDMM_BIG_FRAME_THICKNESS)
#define OSDMM_VERTICAL_SEPARATE_FRAME_WIDTH          4
#define OSDMM_VERTICAL_SEPARATE_FRAME_HEIGHT         (OSDMM_BIG_FRAME_HEIGHT - OSDMM_BIG_FRAME_THICKNESS*2)
#define OSDMM_VERTICAL_SEPARATE_FRAME_THICKNESS      4

//define the position of the horizontal separate frame
#define OSDMM_HORIZONTAL_SEPARATE_FRAME_H           (OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_THICKNESS)
#define OSDMM_HORIZONTAL_SEPARATE_FRAME_V           (OSDMM_ICON_LEFT_WINDOW_START_V+OSDMM_MAX_DIR_IN_FILE_WINDOW*OSDMM_ITEM_HEIGHT+1)
#define OSDMM_HORIZONTAL_SEPARATE_FRAME_WIDTH       (OSDMM_VERTICAL_SEPARATE_FRAME_H-OSDMM_BIG_FRAME_H-OSDMM_BIG_FRAME_THICKNESS)
#define OSDMM_HORIZONTAL_SEPARATE_FRAME_HEIGHT      4
#define OSDMM_HORIZONTAL_SEPARATE_FRAME_THICKNESS   4

//define the distance between the icon and the big frame
#define OSDMM_FRAME_ICON_DISTANCE_H                 24

//define the starting position of the icon in the left window
#define OSDMM_ICON_LEFT_WINDOW_START_H              (OSDMM_BIG_FRAME_H+OSDMM_FRAME_ICON_DISTANCE_H)
#define OSDMM_ICON_LEFT_WINDOW_START_V              (OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_THICKNESS+OSDMM_START_ITEM_DISTANCE_V)

//define the distance between the icon and the item
#define OSDMM_ICON_ITEM_DISTANCE                    8

//define the starting position of the DIR in the left window.
#define OSDMM_ITEM_LEFT_WINDOW_START_H              (OSDMM_ICON_LEFT_WINDOW_START_H+OSDMM_ICON_WIDTH+OSDMM_ICON_ITEM_DISTANCE)
#define OSDMM_ITEM_LEFT_WINDOW_START_V              OSDMM_ICON_LEFT_WINDOW_START_V

//define the preview region
#define OSDMM_PREVIEW_WINDOW_START_H                (OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_THICKNESS)
#define OSDMM_PREVIEW_WINDOW_START_V                (OSDMM_HORIZONTAL_SEPARATE_FRAME_V+OSDMM_HORIZONTAL_SEPARATE_FRAME_THICKNESS)
#define OSDMM_PREVIEW_WINDOW_WIDTH                  OSDMM_HORIZONTAL_SEPARATE_FRAME_WIDTH
#define OSDMM_PREVIEW_WINDOW_HEIGHT                 (OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_HEIGHT-OSDMM_HORIZONTAL_SEPARATE_FRAME_V-OSDMM_HORIZONTAL_SEPARATE_FRAME_THICKNESS-OSDMM_BIG_FRAME_THICKNESS)

//define the starting position of the icon in the right window
#define OSDMM_ICON_RIGHT_WINDOW_START_H             280
#define OSDMM_ICON_RIGHT_WINDOW_START_V             OSDMM_ICON_LEFT_WINDOW_START_V

//define the starting position of the item in the right window.
#define OSDMM_ITEM_RIGHT_WINDOW_START_H             (OSDMM_ICON_RIGHT_WINDOW_START_H+OSDMM_ICON_WIDTH+OSDMM_ICON_ITEM_DISTANCE)
#define OSDMM_ITEM_RIGHT_WINDOW_START_V             OSDMM_ICON_LEFT_WINDOW_START_V

//define the common attribute for the scroll bar
#define OSDMM_SCROLL_BAR_WIDTH                      16
#define OSDMM_SCROLL_BAR_V                          (OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_THICKNESS)

//define the position for the right scroll bar
#define OSDMM_RIGHT_SCROLL_BAR_H                    (OSDMM_BIG_FRAME_H + OSDMM_BIG_FRAME_WIDTH-OSDMM_BIG_FRAME_THICKNESS-OSDMM_SCROLL_BAR_WIDTH)
#define OSDMM_RIGHT_SCROLL_BAR_HEIGHT               (OSDMM_BIG_FRAME_HEIGHT-OSDMM_BIG_FRAME_THICKNESS*2)

//define the position for the left scroll bar
#define OSDMM_LEFT_SCROLL_BAR_H                     (OSDMM_VERTICAL_SEPARATE_FRAME_H-OSDMM_SCROLL_BAR_WIDTH)
#define OSDMM_LEFT_SCROLL_BAR_HEIGHT                (OSDMM_HORIZONTAL_SEPARATE_FRAME_V - OSDMM_BIG_FRAME_V - OSDMM_BIG_FRAME_THICKNESS - OSDMM_HORIZONTAL_SEPARATE_FRAME_THICKNESS)

//define the position of the help string. 
#define OSDMM_BACKGROUND_HELP_DISTANCE_H            4          
#define OSDMM_HELP_START_H                          (OSDMM_BIG_FRAME_H+OSDMM_BACKGROUND_HELP_DISTANCE_H)
#ifndef NO_MM_BUTTON //CoCo2.38
#define OSDMM_HELP_WIDTH                            320
#else
#define OSDMM_HELP_WIDTH                            (OSDMM_BIG_FRAME_WIDTH-OSDMM_BACKGROUND_HELP_DISTANCE_H*2)
#endif //NO_MM_BUTTON //CoCo2.38
#define OSDMM_COPY_HELP_WIDTH                       320
#define OSDMM_HELP_HEIGHT                           26
#define OSDMM_FRAME_HELP_DISTANCE                   8
#define OSDMM_HELP_START_V                          (OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_HEIGHT+OSDMM_FRAME_HELP_DISTANCE)
#define OSDMM_HELP_REGION_STRING_DISTANCE           16
#define OSDMM_HELP_STRING_START_H                   (OSDMM_HELP_START_H+OSDMM_HELP_REGION_STRING_DISTANCE)
#define OSDMM_HELP_STRING_START_V                   OSDMM_HELP_START_V
#define OSDMM_HELP_AUDIO_DISTANCE                   16

//define the distance of the background and the content
#define OSDMM_BACKGROUND_CONTENT_DISTANCE           8

//define the region of the background
#define OSDMM_BACKGROUND_H                          (OSDMM_BIG_FRAME_H-OSDMM_BACKGROUND_CONTENT_DISTANCE)
#define OSDMM_BACKGROUND_V                          (OSDMM_TITLE_START_V-OSDMM_BACKGROUND_CONTENT_DISTANCE)
#define OSDMM_BACKGROUND_RIGHT                      (OSDMM_BIG_FRAME_H+OSDMM_BIG_FRAME_WIDTH+OSDMM_BACKGROUND_CONTENT_DISTANCE)
#define OSDMM_BACKGROUND_BOTTOM                     (OSDMM_HELP_START_V+OSDMM_HELP_HEIGHT+OSDMM_BACKGROUND_CONTENT_DISTANCE)

//define the distance of the background and the content
#define OSDMM_HELP_BUTTON_DISTANCE                  12
#define OSDMM_FRAME_BUTTON_DISTANCE                 8
#define OSDMM_BUTTON_DISTANCE_H                     12

//define the button's position.
#define OSDMM_BUTTON_START_H                        (OSDMM_HELP_START_H+OSDMM_HELP_WIDTH+OSDMM_HELP_BUTTON_DISTANCE)
#define OSDMM_BUTTON_WIDTH                          116 //Need to reserve 16 pixel width for round rectangle. Therefore, Excel set 100
#define OSDMM_BUTTON_START_V                        (OSDMM_BIG_FRAME_V+OSDMM_BIG_FRAME_HEIGHT+OSDMM_FRAME_BUTTON_DISTANCE)
#define OSDMM_BUTTON_HEIGHT                         26
#define OSDMM_DLG_BUTTON_WIDTH                      104 //92 //80 //Need to reserve 16 pixel width for round rectangle. Therefore, Excel set 88
#define OSDMM_DLG_BUTTON_HEIGHT                     26

#define OSDMM_COPY_BUTTON_START_H                   (OSDMM_HELP_START_H+OSDMM_COPY_HELP_WIDTH+OSDMM_HELP_BUTTON_DISTANCE)

//define the distance between the right scroll bar and the serial num
#define OSDMM_SCROLLBAR_SERIALNUM_DISTANCE          8

//define the distance between the serial num and file name string.
#define OSDMM_SERIALNUM_FILENAME_DISTANCE           16	

//define the position of the serial num
#define OSDMM_SERIAL_NUM_START_H                    (OSDMM_RIGHT_SCROLL_BAR_H - OSDMM_SCROLLBAR_SERIALNUM_DISTANCE)     

//define the thickness of the selected frame for program
#define OSDMM_PROGRAM_FRAME_THICKNESS               2

#define OSDMM_HIGHLIGHT_DISTANCE_H                  2
#define OSDMM_HIGHLIGHT_DISTANCE_V                  0 //1

//define the distance of the selected frame
#define OSDMM_SELECT_FRAME_DISTANCE_H               6
#define OSDMM_SELECT_FRAME_DISTANCE_V			    3 //4

//define the item width of the dialog box
#define OSDMM_DLG_FILE_TYPE_ITEM_WIDTH              96

//define the item height of the dialog box
#define OSDMM_DLG_ITEM_HEIGHT                       26

//define the distance between the dialog and the farme
#define OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_H       16

//define the distance between the dialog and the title
#define OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_V       8

//define the distance between the frame and the item
#define OSDMM_DLG_FRAME_ITEM_DISTANCE_H             8
#define OSDMM_DLG_FRAME_ITEM_DISTANCE_V             8

//define distance between the frame and the button
#define OSDMM_DLG_FRAME_BUTTON_DISTANCE_V           8

//define the distance betwwen the button and background
#define OSDMM_DLG_BUTTON_BACKGROUND_DISTANCE_V      16

//define the horizontal distance between each items
#define OSDMM_DLG_FILE_TYPE_ITEM_DISTANCE_H         16

//define the vertical distance between each items
#define OSDMM_DLG_ITEM_DISTANCE_V                   8

//define the horizontal distance between icon and item of the dialog.
#define OSDMM_DLG_ICON_ITEM_DISTANCE_H              4

//define the distance between the tlte and farme
#define OSDMM_DLG_FILE_TYPE_TITLE_FRAME_DISTANCE_V  12

//Define the width, height and thickness of the "file type" dialog box
#define OSDMM_DLG_FILE_TYPE_FRAME_THICKNESS         4
#define OSDMM_DLG_FILE_TYPE_FRAME_WITDH             (OSDMM_DLG_FILE_TYPE_FRAME_THICKNESS*2+OSDMM_DLG_FILE_TYPE_ITEM_WIDTH*OSDMM_FILTER_DLG_CLOUME_NUM+OSDMM_DLG_FRAME_ITEM_DISTANCE_H*2+OSDMM_DLG_FILE_TYPE_ITEM_DISTANCE_H)
#define OSDMM_DLG_FILE_TYPE_FRAME_HEIGHT            (OSDMM_DLG_FILE_TYPE_FRAME_THICKNESS*2+OSDMM_DLG_ITEM_HEIGHT*OSDMM_FILTER_DLG_ROW_NUM+OSDMM_DLG_ITEM_DISTANCE_V*(OSDMM_FILTER_DLG_ROW_NUM-1)+OSDMM_DLG_FRAME_ITEM_DISTANCE_V*2)

//define the size of the "file type" dialog box.
#define OSDMM_DLG_FILE_TYPE_WIDTH                   (OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_H*2+OSDMM_DLG_FILE_TYPE_FRAME_WITDH)
#define OSDMM_DLG_FILE_TYPE_HEIGHT                  (OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_V+OSDMM_TITLE_FONT_HEIGHT+OSDMM_DLG_FILE_TYPE_TITLE_FRAME_DISTANCE_V+OSDMM_DLG_FILE_TYPE_FRAME_HEIGHT+OSDMM_DLG_FRAME_BUTTON_DISTANCE_V+OSDMM_BUTTON_HEIGHT+OSDMM_DLG_BUTTON_BACKGROUND_DISTANCE_V)

//define the starting position of the "file type" dialog box
#define OSDMM_DLG_FILE_TYPE_START_H                 (OSDMM_BIG_FRAME_H+((OSDMM_BIG_FRAME_WIDTH-OSDMM_DLG_FILE_TYPE_WIDTH)>>1))
#define OSDMM_DLG_FILE_TYPE_START_V                 (OSDMM_BIG_FRAME_V+((OSDMM_BIG_FRAME_HEIGHT-OSDMM_DLG_FILE_TYPE_HEIGHT)>>1))

//the title of the "file type" dialog box.
#define OSDMM_DLG_FILE_TYPE_TITLE_START_H           (OSDMM_DLG_FILE_TYPE_START_H+OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_H)
#define OSDMM_DLG_FILE_TYPE_TITLE_START_V           (OSDMM_DLG_FILE_TYPE_START_V+OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_V)

//the frame of the "file type" dialog box.
#define OSDMM_DLG_FILE_TYPE_FRAME_START_H           OSDMM_DLG_FILE_TYPE_TITLE_START_H
#define OSDMM_DLG_FILE_TYPE_FRAME_START_V           (OSDMM_DLG_FILE_TYPE_TITLE_START_V+OSDMM_TITLE_FONT_HEIGHT+OSDMM_DLG_FILE_TYPE_TITLE_FRAME_DISTANCE_V)

//Define the content region of the "file type" dialog box.
#define OSDMM_DLG_FILE_TYPE_CONTENT_START_H         OSDMM_DLG_FILE_TYPE_START_H
#define OSDMM_DLG_FILE_TYPE_CONTENT_START_V         (OSDMM_DLG_FILE_TYPE_TITLE_START_V+OSDMM_TITLE_FONT_HEIGHT+OSDMM_DLG_FILE_TYPE_TITLE_FRAME_DISTANCE_V)
#define OSDMM_DLG_FILE_TYPE_CONTENT_WIDTH           OSDMM_DLG_FILE_TYPE_WIDTH
#define OSDMM_DLG_FILE_TYPE_CONTENT_HEIGHT          

//define the starting position of the button
#define OSDMM_DLG_FILE_TYPE_BUTTON_START_H          (OSDMM_DLG_FILE_TYPE_FRAME_START_H+OSDMM_DLG_FILE_TYPE_FRAME_WITDH-OSDMM_DLG_BUTTON_WIDTH)
#define OSDMM_DLG_FILE_TYPE_BUTTON_START_V          (OSDMM_DLG_FILE_TYPE_FRAME_START_V+OSDMM_DLG_FILE_TYPE_FRAME_HEIGHT+OSDMM_DLG_FRAME_BUTTON_DISTANCE_V)

//Define the position of each items
#define OSDMM_DLG_FILE_TYPE_ITEM_START_H            (OSDMM_DLG_FILE_TYPE_FRAME_START_H+OSDMM_DLG_FILE_TYPE_FRAME_THICKNESS+OSDMM_DLG_FRAME_ITEM_DISTANCE_H)
#define OSDMM_DLG_FILE_TYPE_ITEM_START_V            (OSDMM_DLG_FILE_TYPE_FRAME_START_V+OSDMM_DLG_FILE_TYPE_FRAME_THICKNESS+OSDMM_DLG_FRAME_ITEM_DISTANCE_V)

//define the title bar of the "file type" dialog box
#define OSDMM_DLG_FILE_TYPE_TITLE_BAR_START_H       OSDMM_DLG_FILE_TYPE_START_H
#define OSDMM_DLG_FILE_TYPE_TITLE_BAR_START_V       OSDMM_DLG_FILE_TYPE_START_V
#define OSDMM_DLG_FILE_TYPE_TITLE_BAR_WIDTH         OSDMM_DLG_FILE_TYPE_WIDTH
#define OSDMM_DLG_FILE_TYPE_TITLE_BAR_HEIGHT        (OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_V+OSDMM_TITLE_FONT_HEIGHT)

//define the item width of the dialog box
#define OSDMM_DLG_SORT_FILE_ITEM_WIDTH                  120//112

//define the horizontal separate frame' height and thickness
#define OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_HEIGHT       4
#define OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_THICKNESS    4

//define the distance between the separate frame and title
#define OSDMM_DLG_SORT_FILE_FRAME_TITLE_DISTANCE_V      4

//define the distance between the "method" frame and "sequence" frame
#define OSDMM_DLG_SORT_FILE_METHOD_SEQUENCE_DISTANCE    16

//define the attributes of method frame.
#define OSDMM_DLG_SORT_FILE_METHOD_FRAME_THICKNESS      4
#define OSDMM_DLG_SORT_FILE_METHOD_FRAME_WIDTH          (OSDMM_DLG_FRAME_ITEM_DISTANCE_H*2+OSDMM_DLG_ICON_WIDTH+OSDMM_DLG_ICON_ITEM_DISTANCE_H+OSDMM_DLG_SORT_FILE_ITEM_WIDTH+OSDMM_DLG_SORT_FILE_METHOD_FRAME_THICKNESS*2)
#define OSDMM_DLG_SORT_FILE_METHOD_FRAME_HEIGHT         (OSDMM_DLG_ITEM_HEIGHT*3+OSDMM_DLG_FRAME_ITEM_DISTANCE_V*2+OSDMM_DLG_ITEM_DISTANCE_V*2+OSDMM_DLG_SORT_FILE_METHOD_FRAME_THICKNESS*2)

//define the size of the "sort file" dialog box
#define OSDMM_DLG_SORT_FILE_WIDTH                       (OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_H*2+OSDMM_DLG_SORT_FILE_METHOD_FRAME_WIDTH+OSDMM_DLG_SORT_FILE_METHOD_SEQUENCE_DISTANCE+OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_WIDTH)
#define OSDMM_DLG_SORT_FILE_HEIGHT                      (OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_V+OSDMM_TITLE_FONT_HEIGHT+OSDMM_DLG_SORT_FILE_FRAME_TITLE_DISTANCE_V*2+OSDMM_TITLE_FONT_HEIGHT+OSDMM_DLG_SORT_FILE_FRAME_TITLE_DISTANCE_V+OSDMM_DLG_SORT_FILE_METHOD_FRAME_HEIGHT+OSDMM_DLG_FRAME_BUTTON_DISTANCE_V+OSDMM_DLG_BUTTON_HEIGHT+OSDMM_DLG_BUTTON_BACKGROUND_DISTANCE_V)

//define the starting position of the "file type" dialog box
#define OSDMM_DLG_SORT_FILE_START_H                     (OSDMM_BIG_FRAME_H+((OSDMM_BIG_FRAME_WIDTH-OSDMM_DLG_SORT_FILE_WIDTH)>>1))
#define OSDMM_DLG_SORT_FILE_START_V                     (OSDMM_BIG_FRAME_V+((OSDMM_BIG_FRAME_HEIGHT-OSDMM_DLG_SORT_FILE_HEIGHT)>>1))

//the title of the "sort file" dialog box.
#define OSDMM_DLG_SORT_FILE_TITLE_START_H               (OSDMM_DLG_SORT_FILE_START_H+OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_H)
#define OSDMM_DLG_SORT_FILE_TITLE_START_V               (OSDMM_DLG_SORT_FILE_START_V+OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_V)

//define the horizontal separate frame
#define OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_H            OSDMM_DLG_SORT_FILE_START_H
#define OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_V            (OSDMM_DLG_SORT_FILE_TITLE_START_V+OSDMM_TITLE_FONT_HEIGHT+OSDMM_DLG_SORT_FILE_FRAME_TITLE_DISTANCE_V)

//define the sub-title of the "method" frame
#define OSDMM_DLG_SORT_FILE_SUB_TITLE_METHOD_START_H    OSDMM_DLG_SORT_FILE_TITLE_START_H
#define OSDMM_DLG_SORT_FILE_SUB_TITLE_METHOD_START_V    (OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_V+OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_HEIGHT+OSDMM_DLG_SORT_FILE_FRAME_TITLE_DISTANCE_V)

//define the "method" frame in "sort file" dialog box
#define OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_H        OSDMM_DLG_SORT_FILE_TITLE_START_H
#define OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_V        (OSDMM_DLG_SORT_FILE_SUB_TITLE_METHOD_START_V+OSDMM_TITLE_FONT_HEIGHT+OSDMM_DLG_SORT_FILE_FRAME_TITLE_DISTANCE_V)

//define the sub-title of the "method" frame
#define OSDMM_DLG_SORT_FILE_SUB_TITLE_SEQUENCE_START_H  (OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_H+OSDMM_DLG_SORT_FILE_METHOD_FRAME_WIDTH+OSDMM_DLG_SORT_FILE_METHOD_SEQUENCE_DISTANCE)
#define OSDMM_DLG_SORT_FILE_SUB_TITLE_SEQUENCE_START_V  OSDMM_DLG_SORT_FILE_SUB_TITLE_METHOD_START_V

//define the "sequence" frame in "sort file" dialog box
#define OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_START_H      OSDMM_DLG_SORT_FILE_SUB_TITLE_SEQUENCE_START_H
#define OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_START_V      OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_V
#define OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_THICKNESS    OSDMM_DLG_SORT_FILE_METHOD_FRAME_THICKNESS
#define OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_WIDTH        OSDMM_DLG_SORT_FILE_METHOD_FRAME_WIDTH
#define OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_HEIGHT       OSDMM_DLG_SORT_FILE_METHOD_FRAME_HEIGHT         

//define the width of the sepqrate frame
#define OSDMM_DLG_SORT_FILE_SEPARATE_FRAME_WIDTH        OSDMM_DLG_SORT_FILE_WIDTH

//define the starting position of the button
#define OSDMM_DLG_SORT_FILE_BUTTON_START_H              (OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_START_H+OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_WIDTH-OSDMM_DLG_BUTTON_WIDTH)
#define OSDMM_DLG_SORT_FILE_BUTTON_START_V              (OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_V+OSDMM_DLG_SORT_FILE_METHOD_FRAME_HEIGHT+OSDMM_DLG_FRAME_BUTTON_DISTANCE_V)

//Define the position of each items
#define OSDMM_DLG_SORT_FILE_METHOD_TYPE_ITEM_START_H    (OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_H+OSDMM_DLG_SORT_FILE_METHOD_FRAME_THICKNESS+OSDMM_DLG_FRAME_ITEM_DISTANCE_H+OSDMM_DLG_ICON_WIDTH+OSDMM_DLG_ICON_ITEM_DISTANCE_H)
#define OSDMM_DLG_SORT_FILE_METHOD_TYPE_ITEM_START_V    (OSDMM_DLG_SORT_FILE_METHOD_FRAME_START_V+OSDMM_DLG_SORT_FILE_METHOD_FRAME_THICKNESS+OSDMM_DLG_FRAME_ITEM_DISTANCE_V)

#define OSDMM_DLG_SORT_FILE_METHOD_NAME_ITEM_START_H    OSDMM_DLG_SORT_FILE_METHOD_TYPE_ITEM_START_H
#define OSDMM_DLG_SORT_FILE_METHOD_NAME_ITEM_START_V    (OSDMM_DLG_SORT_FILE_METHOD_TYPE_ITEM_START_V+OSDMM_DLG_ITEM_HEIGHT+OSDMM_DLG_ITEM_DISTANCE_V)

#define OSDMM_DLG_SORT_FILE_METHOD_TIME_ITEM_START_H    OSDMM_DLG_SORT_FILE_METHOD_TYPE_ITEM_START_H
#define OSDMM_DLG_SORT_FILE_METHOD_TIME_ITEM_START_V    (OSDMM_DLG_SORT_FILE_METHOD_NAME_ITEM_START_V+OSDMM_DLG_ITEM_HEIGHT+OSDMM_DLG_ITEM_DISTANCE_V)

#define OSDMM_DLG_SORT_FILE_SEQUENCE_ASCENDING_ITEM_START_H (OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_START_H+OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_THICKNESS+OSDMM_DLG_FRAME_ITEM_DISTANCE_H+OSDMM_DLG_ICON_WIDTH+OSDMM_DLG_ICON_ITEM_DISTANCE_H)
#define OSDMM_DLG_SORT_FILE_SEQUENCE_ASCENDING_ITEM_START_V (OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_START_V+OSDMM_DLG_SORT_FILE_SEQUENCE_FRAME_THICKNESS+OSDMM_DLG_FRAME_ITEM_DISTANCE_V)

#define OSDMM_DLG_SORT_FILE_SEQUENCE_DESCENDING_ITEM_START_H    OSDMM_DLG_SORT_FILE_SEQUENCE_ASCENDING_ITEM_START_H
#define OSDMM_DLG_SORT_FILE_SEQUENCE_DESCENDING_ITEM_START_V    (OSDMM_DLG_SORT_FILE_SEQUENCE_ASCENDING_ITEM_START_V+OSDMM_DLG_ITEM_HEIGHT+OSDMM_DLG_ITEM_DISTANCE_V)
       
//define the title bar of the "sort file" dialog box
#define OSDMM_DLG_SORT_FILE_TITLE_BAR_START_H                   OSDMM_DLG_SORT_FILE_START_H          
#define OSDMM_DLG_SORT_FILE_TITLE_BAR_START_V                   OSDMM_DLG_SORT_FILE_START_V
#define OSDMM_DLG_SORT_FILE_TITLE_BAR_WIDTH                     OSDMM_DLG_SORT_FILE_WIDTH
#define OSDMM_DLG_SORT_FILE_TITLE_BAR_HEIGHT                    (OSDMM_DLG_BACKGROUND_TITLE_DISTANCE_V+OSDMM_TITLE_FONT_HEIGHT+OSDMM_DLG_SORT_FILE_FRAME_TITLE_DISTANCE_V)

//define the attributes for the info. in the Program List
#define OSDMM_PROGRAM_LIST_INFO_DATE_START_H                OSDMM_ICON_LEFT_WINDOW_START_H
#define OSDMM_PROGRAM_LIST_INFO_DATE_START_V                OSDMM_ICON_LEFT_WINDOW_START_V
#define OSDMM_PROGRAM_LIST_INFO_FILE_SIZE_START_H           OSDMM_PROGRAM_LIST_INFO_DATE_START_H
#define OSDMM_PROGRAM_LIST_INFO_FILE_SIZE_START_V           (OSDMM_ICON_LEFT_WINDOW_START_V+OSDMM_ITEM_HEIGHT)

//Define the attributes of audio spectrum line.
#define OSDMM_AUDIO_SPECTRUM_LINE_NUM                   16
#define OSDMM_AUDIO_SPECTRUM_WIDTH                      8
#define OSDMM_AUDIO_SPECTRUM_DISTANCE                   4
#define OSDMM_AUDIO_SPECTRUM_PER_RANGE_HEIGHT           4
#define OSDMM_AUDIO_SPECTRUM_START_H                    (OSDMM_PREVIEW_WINDOW_START_H+(OSDMM_PREVIEW_WINDOW_WIDTH-OSDMM_AUDIO_SPECTRUM_WIDTH*OSDMM_AUDIO_SPECTRUM_LINE_NUM-OSDMM_AUDIO_SPECTRUM_DISTANCE*(OSDMM_AUDIO_SPECTRUM_LINE_NUM-1))/2)
#define OSDMM_AUDIO_SPECTRUM_START_V                    (OSDMM_PREVIEW_WINDOW_START_V+OSDMM_PREVIEW_WINDOW_HEIGHT-8)

//Define the attributes of the "difference"
#define OSDMM_AUDIO_SPECTRUM_DIFFERENCE_HEIGHT          2
#define OSDMM_AUDIO_SPECTRUM_DIFFERENCE_COLOR           OSDMM_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT

//Define the colors of one spectrum line.
#define OSDMM_AUDIO_SPECTRUM_LINE_COLOR_NUM             5

//Define the color and the number of range for the spectrum
#define OSDMM_AUDIO_SPECTRUM_LINE_0_RANGE               4
#define OSDMM_AUDIO_SPECTRUM_LINE_0_COLOR               OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_0
#define OSDMM_AUDIO_SPECTRUM_LINE_1_RANGE               3
#define OSDMM_AUDIO_SPECTRUM_LINE_1_COLOR               OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_1
#define OSDMM_AUDIO_SPECTRUM_LINE_2_RANGE               3
#define OSDMM_AUDIO_SPECTRUM_LINE_2_COLOR               OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_2
#define OSDMM_AUDIO_SPECTRUM_LINE_3_RANGE               3
#define OSDMM_AUDIO_SPECTRUM_LINE_3_COLOR               OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_3
#define OSDMM_AUDIO_SPECTRUM_LINE_4_RANGE               2
#define OSDMM_AUDIO_SPECTRUM_LINE_4_COLOR               OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_4

//define the type of icons
#define OSDMM_ICON_TYPE_VOLUME_SERVO                    0
#define OSDMM_ICON_TYPE_USB                             1
#define OSDMM_ICON_TYPE_CARD_READER                     2
#define OSDMM_ICON_TYPE_UPFOLDER                        3
#define OSDMM_ICON_TYPE_FOLDER                          4
#define OSDMM_ICON_TYPE_OPENFOLDER                      5
#define OSDMM_ICON_TYPE_JPEG                            6
#define OSDMM_ICON_TYPE_MP3                             7
#define OSDMM_ICON_TYPE_WMA                             8
#define OSDMM_ICON_TYPE_DIVX                            9
#define OSDMM_ICON_TYPE_VOB                             10
#define OSDMM_ICON_TYPE_MP2                             11
#define OSDMM_ICON_TYPE_DAT                             12
#define OSDMM_ICON_TYPE_AC3                             13
#define OSDMM_ICON_TYPE_MPG                             14
#define OSDMM_ICON_TYPE_MP4                             15
#define OSDMM_ICON_TYPE_NONE                            0xFF

//define the type of the title
#define OSDMM_TITLE_TYPE_NONE                           0
#define OSDMM_TITLE_TYPE_DIR                            1
#define OSDMM_TITLE_TYPE_FILE                           2
#define OSDMM_TITLE_TYPE_FILE_FOLDER                    3
#define OSDMM_TITLE_TYPE_UP_FOLDER                      4
#define OSDMM_TITLE_TYPE_PROGRAM                        5
#define OSDMM_TITLE_TYPE_PROGRAM_LIST                   6
#define OSDMM_TITLE_TYPE_PLAYING                        7

//define the type of help
#define OSDMM_HELP_TYPE_NONE                            0
#define OSDMM_HELP_TYPE_PROGRAM                         1
#define OSDMM_HELP_TYPE_PROGRAM_LIST                    2
#define OSDMM_HELP_TYPE_JPEG                            3
#define OSDMM_HELP_TYPE_MP3                             4
#define OSDMM_HELP_TYPE_WMA                             5
#define OSDMM_HELP_TYPE_DIVX                            6
#define OSDMM_HELP_TYPE_VOB                             7
#define OSDMM_HELP_TYPE_MP2                             8
#define OSDMM_HELP_TYPE_DAT                             9
#define OSDMM_HELP_TYPE_MPG                             10

//define the parameter of changing UI mode.
#define OSDMM_CHANGE_MODE_NORMAL_TO_PROGRAM             0
#define OSDMM_CHANGE_MODE_PROGRAM_TO_PROGRAM_LIST       1
#define OSDMM_CHANGE_MODE_PROGRAMLIST_TO_PROGRAM        2
#define OSDMM_CHANGE_MODE_PROGRAM_TO_NORMAL             3

//define the dialog ID
#define OSDMM_DLG_FILE_TYPE                             0
#define OSDMM_DLG_SORT_FILE                             1

//define the entry for the color palette
#define OSDMM_PALETTE_ENTRY_START                                   GDI_GENERAL_UI_PALETTE_INDEX_START
#define OSDMM_PALETTE_ENTRY_GENERAL_BACKGROUND                      OSDMM_PALETTE_ENTRY_START
#define OSDMM_PALETTE_ENTRY_TITLE_STRING                            (OSDMM_PALETTE_ENTRY_START+1)                        
#define OSDMM_PALETTE_ENTRY_HELP_STRING                             (OSDMM_PALETTE_ENTRY_START+2)
#define OSDMM_PALETTE_ENTRY_GENERAL_STRING_IN_FILE_WINDOW           (OSDMM_PALETTE_ENTRY_START+3)      
#define OSDMM_PALETTE_ENTRY_HIGHLIGHT_BACKGROUND                    (OSDMM_PALETTE_ENTRY_START+4)                
#define OSDMM_PALETTE_ENTRY_ENABLE_BUTTON_STRING                    (OSDMM_PALETTE_ENTRY_START+5)                
#define OSDMM_PALETTE_ENTRY_HIGHLIGHT_BUTTON_STRING                 (OSDMM_PALETTE_ENTRY_START+6)             
#define OSDMM_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT                     (OSDMM_PALETTE_ENTRY_START+7)                 
#define OSDMM_PALETTE_ENTRY_NORMAL_FRAME_DARK                       (OSDMM_PALETTE_ENTRY_START+8)                   
#define OSDMM_PALETTE_ENTRY_SELECT_FRAME_BRIGHT                     (OSDMM_PALETTE_ENTRY_START+9)                 
#define OSDMM_PALETTE_ENTRY_SELECT_FRAME_DARK                       (OSDMM_PALETTE_ENTRY_START+10)                 
#define OSDMM_PALETTE_ENTRY_SEPARATE_FRAME_CENTER                   (OSDMM_PALETTE_ENTRY_START+11)               
#define OSDMM_PALETTE_ENTRY_UPFOLDER_ICON_TEXT_FOREGROUND           (OSDMM_PALETTE_ENTRY_START+12)                  
#define OSDMM_PALETTE_ENTRY_UPFOLDER_ICON_SHADOW_1                  (OSDMM_PALETTE_ENTRY_START+13)              
#define OSDMM_PALETTE_ENTRY_UPFOLDER_ICON_SHADOW_2                  (OSDMM_PALETTE_ENTRY_START+14)              
#define OSDMM_PALETTE_ENTRY_FOLDER_ICON_TEXT_FOREGROUND             (OSDMM_PALETTE_ENTRY_START+15)         
#define OSDMM_PALETTE_ENTRY_FOLDER_ICON_SHADOW_1                    (OSDMM_PALETTE_ENTRY_START+16)                
#define OSDMM_PALETTE_ENTRY_FOLDER_ICON_SHADOW_2                    (OSDMM_PALETTE_ENTRY_START+17)                
#define OSDMM_PALETTE_ENTRY_JPEG_ICON_TEXT_FOREGROUND               (OSDMM_PALETTE_ENTRY_START+18)           
#define OSDMM_PALETTE_ENTRY_JPEG_ICON_SHADOW_1                      (OSDMM_PALETTE_ENTRY_START+19)                  
#define OSDMM_PALETTE_ENTRY_JPEG_ICON_SHADOW_2                      (OSDMM_PALETTE_ENTRY_START+20)                  
#define OSDMM_PALETTE_ENTRY_MP3_ICON_TEXT_FOREGROUND                (OSDMM_PALETTE_ENTRY_START+21)            
#define OSDMM_PALETTE_ENTRY_MP3_ICON_SHADOW_1                       (OSDMM_PALETTE_ENTRY_START+22)                   
#define OSDMM_PALETTE_ENTRY_MP3_ICON_SHADOW_2                       (OSDMM_PALETTE_ENTRY_START+23)                   
#define OSDMM_PALETTE_ENTRY_WMA_ICON_TEXT_FOREGROUND                (OSDMM_PALETTE_ENTRY_START+24)
#define OSDMM_PALETTE_ENTRY_WMA_ICON_SHADOW_1                       (OSDMM_PALETTE_ENTRY_START+25) 
#define OSDMM_PALETTE_ENTRY_WMA_ICON_SHADOW_2                       (OSDMM_PALETTE_ENTRY_START+26)
#define OSDMM_PALETTE_ENTRY_OPENFOLDER_ICON_TEXT_FOREGROUND         (OSDMM_PALETTE_ENTRY_START+27)
#define OSDMM_PALETTE_ENTRY_OPENFOLDER_ICON_SHADOW_1                (OSDMM_PALETTE_ENTRY_START+28)
#define OSDMM_PALETTE_ENTRY_OPENFOLDER_ICON_SHADOW_2                (OSDMM_PALETTE_ENTRY_START+29)
#define OSDMM_PALETTE_ENTRY_TIME_STRING                             (OSDMM_PALETTE_ENTRY_START+30)
#define OSDMM_PALETTE_ENTRY_DIVX_ICON_TEXT_FOREGROUND               (OSDMM_PALETTE_ENTRY_START+31)
#define OSDMM_PALETTE_ENTRY_DIVX_ICON_SHADOW_1                      (OSDMM_PALETTE_ENTRY_START+32)
#define OSDMM_PALETTE_ENTRY_DIVX_ICON_SHADOW_2                      (OSDMM_PALETTE_ENTRY_START+33)
#define OSDMM_PALETTE_ENTRY_MP2_ICON_TEXT_FOREGROUND                (OSDMM_PALETTE_ENTRY_START+34)
#define OSDMM_PALETTE_ENTRY_MP2_ICON_SHADOW_1                       (OSDMM_PALETTE_ENTRY_START+35)
#define OSDMM_PALETTE_ENTRY_MP2_ICON_SHADOW_2                       (OSDMM_PALETTE_ENTRY_START+36)
#define OSDMM_PALETTE_ENTRY_DAT_ICON_TEXT_FOREGROUND                (OSDMM_PALETTE_ENTRY_START+37)
#define OSDMM_PALETTE_ENTRY_DAT_ICON_SHADOW_1                       (OSDMM_PALETTE_ENTRY_START+38)
#define OSDMM_PALETTE_ENTRY_DAT_ICON_SHADOW_2                       (OSDMM_PALETTE_ENTRY_START+39)
#define OSDMM_PALETTE_ENTRY_VOB_ICON_TEXT_FOREGROUND                (OSDMM_PALETTE_ENTRY_START+40)
#define OSDMM_PALETTE_ENTRY_VOB_ICON_SHADOW_1                       (OSDMM_PALETTE_ENTRY_START+41)
#define OSDMM_PALETTE_ENTRY_VOB_ICON_SHADOW_2                       (OSDMM_PALETTE_ENTRY_START+42)
#define OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_TEXT_FOREGROUND       (OSDMM_PALETTE_ENTRY_START+43)
#define OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_SHADOW_1              (OSDMM_PALETTE_ENTRY_START+44)
#define OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_SHADOW_2              (OSDMM_PALETTE_ENTRY_START+45)
#define OSDMM_PALETTE_ENTRY_VOLUME_USB_ICON_TEXT_FOREGROUND         (OSDMM_PALETTE_ENTRY_START+46)
#define OSDMM_PALETTE_ENTRY_VOLUME_USB_ICON_SHADOW_1                (OSDMM_PALETTE_ENTRY_START+47)
#define OSDMM_PALETTE_ENTRY_VOLUME_USB_ICON_SHADOW_2                (OSDMM_PALETTE_ENTRY_START+48)
#define OSDMM_PALETTE_ENTRY_VOLUME_CARD_READER_ICON_TEXT_FOREGROUND (OSDMM_PALETTE_ENTRY_START+49)
#define OSDMM_PALETTE_ENTRY_VOLUME_CARD_READER_ICON_SHADOW_1        (OSDMM_PALETTE_ENTRY_START+50)    
#define OSDMM_PALETTE_ENTRY_VOLUME_CARD_READER_ICON_SHADOW_2        (OSDMM_PALETTE_ENTRY_START+51)
#define OSDMM_PALETTE_ENTRY_DLG_BACKGROUND                          (OSDMM_PALETTE_ENTRY_START+52)
#define OSDMM_PALETTE_ENTRY_BUTTON_ENABLE_BACKGROUND                (OSDMM_PALETTE_ENTRY_START+53)
#define OSDMM_PALETTE_ENTRY_BUTTON_HIGHLIGHT_BACKGROUND             (OSDMM_PALETTE_ENTRY_START+54)
#define OSDMM_PALETTE_ENTRY_HELP_REGION_BACKGROUND                  (OSDMM_PALETTE_ENTRY_START+55)
#define OSDMM_PALETTE_ENTRY_HIGHLIGHT_STRING_IN_FILE_WINDOW         (OSDMM_PALETTE_ENTRY_START+56)
#define OSDMM_PALETTE_ENTRY_DLG_STRING                              (OSDMM_PALETTE_ENTRY_START+57)
#define OSDMM_PALETTE_ENTRY_CHECKED_ICON_TEXT_FOREGROUND            (OSDMM_PALETTE_ENTRY_START+58)
#define OSDMM_PALETTE_ENTRY_CHECKED_ICON_SHADOW_1                   (OSDMM_PALETTE_ENTRY_START+59)
#define OSDMM_PALETTE_ENTRY_CHECKED_ICON_SHADOW_2                   (OSDMM_PALETTE_ENTRY_START+60)
#define OSDMM_PALETTE_ENTRY_UNCHECKED_ICON_TEXT_FOREGROUND          (OSDMM_PALETTE_ENTRY_START+61)
#define OSDMM_PALETTE_ENTRY_RADIO_ICON_TEXT_FOREGROUND              (OSDMM_PALETTE_ENTRY_START+62)
#define OSDMM_PALETTE_ENTRY_RADIO_ICON_SHADOW_1                     (OSDMM_PALETTE_ENTRY_START+63)
#define OSDMM_PALETTE_ENTRY_UNRADIO_ICON_TEXT_FOREGROUND            (OSDMM_PALETTE_ENTRY_START+64)
#define OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_0                   (OSDMM_PALETTE_ENTRY_START+65)
#define OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_1                   (OSDMM_PALETTE_ENTRY_START+66)
#define OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_2                   (OSDMM_PALETTE_ENTRY_START+67)
#define OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_3                   (OSDMM_PALETTE_ENTRY_START+68)
#define OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_LINE_4                   (OSDMM_PALETTE_ENTRY_START+69)
#define OSDMM_PALETTE_ENTRY_AUDIO_SPECTRUM_DROP                     (OSDMM_PALETTE_ENTRY_START+70)
#define OSDMM_PALETTE_ENTRY_DLG_TITLE_BAR                           (OSDMM_PALETTE_ENTRY_START+71)
#define OSDMM_PALETTE_ENTRY_BACKGROUND_FRAME                        (OSDMM_PALETTE_ENTRY_START+72)
#define OSDMM_PALETTE_ENTRY_INFO_REGION_FRAME                       (OSDMM_PALETTE_ENTRY_START+73)
#define OSDMM_PALETTE_ENTRY_BUTTON_FRAME                            (OSDMM_PALETTE_ENTRY_START+74)
#define OSDMM_PALETTE_ENTRY_DLG_OUTER_FRAME                         (OSDMM_PALETTE_ENTRY_START+75)
#define OSDMM_PALETTE_ENTRY_BUTTON_DISABLE_BACKGROUND               (OSDMM_PALETTE_ENTRY_START+76)
#define OSDMM_PALETTE_ENTRY_DISABLE_BUTTON_STRING                   (OSDMM_PALETTE_ENTRY_START+77)
#define OSDMM_PALETTE_ENTRY_AC3_ICON_TEXT_FOREGROUND                (OSDMM_PALETTE_ENTRY_START+78)        
#define OSDMM_PALETTE_ENTRY_MPG_ICON_TEXT_FOREGROUND                (OSDMM_PALETTE_ENTRY_START+79)
#define OSDMM_PALETTE_ENTRY_MPG_ICON_SHADOW_1                       OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_SHADOW_2
#define OSDMM_PALETTE_ENTRY_MPG_ICON_SHADOW_2                       OSDMM_PALETTE_ENTRY_VOLUME_SERVO_ICON_TEXT_FOREGROUND
#define OSDMM_PALETTE_ENTRY_MP4_ICON_TEXT_FOREGROUND                (OSDMM_PALETTE_ENTRY_START+80)
#define OSDMM_PALETTE_ENTRY_MP4_ICON_SHADOW_1                       (OSDMM_PALETTE_ENTRY_START+81)
#define OSDMM_PALETTE_ENTRY_MP4_ICON_SHADOW_2                       (OSDMM_PALETTE_ENTRY_START+82)

//define the palette color
#define OSDMM_PALETTE_COLOR_GENERAL_BACKGROUND_1                        0x8C8CAA
#define OSDMM_PALETTE_COLOR_GENERAL_BACKGROUND_2                        0xC0C0C0//0xCFCFCF
#define OSDMM_PALETTE_COLOR_GENERAL_BACKGROUND_3                        0xDCE5F8
#define OSDMM_PALETTE_COLOR_GENERAL_BACKGROUND_4                        0xCADABC//0xF0F0D0
#define OSDMM_PALETTE_COLOR_BACKGROUND_FRAME_1                          0x686994
#define OSDMM_PALETTE_COLOR_BACKGROUND_FRAME_2                          0xAAAAAA
#define OSDMM_PALETTE_COLOR_BACKGROUND_FRAME_3                          0x686994//0xC9D9F6
#define OSDMM_PALETTE_COLOR_BACKGROUND_FRAME_4                          0xD2D2AA
#define OSDMM_PALETTE_COLOR_TITLE_STRING_1                              0xFFFFFF
#define OSDMM_PALETTE_COLOR_TITLE_STRING_2                              0x2A3A9A
#define OSDMM_PALETTE_COLOR_TITLE_STRING_3                              0x9A5EE4//0xFFFFFF
#define OSDMM_PALETTE_COLOR_TITLE_STRING_4                              0x7D5A24
#define OSDMM_PALETTE_COLOR_TIME_STRING_1                               OSDMM_PALETTE_COLOR_TITLE_STRING_1                         
#define OSDMM_PALETTE_COLOR_TIME_STRING_2                               OSDMM_PALETTE_COLOR_TITLE_STRING_2                         
#define OSDMM_PALETTE_COLOR_TIME_STRING_3                               OSDMM_PALETTE_COLOR_TITLE_STRING_3                         
#define OSDMM_PALETTE_COLOR_TIME_STRING_4                               OSDMM_PALETTE_COLOR_TITLE_STRING_4                         
#define OSDMM_PALETTE_COLOR_HELP_REGION_BACKGROUND_1                    0x5050A0
#define OSDMM_PALETTE_COLOR_HELP_REGION_BACKGROUND_2                    0x54B55B
#define OSDMM_PALETTE_COLOR_HELP_REGION_BACKGROUND_3                    0xFDE85D//0xB9BDFD
#define OSDMM_PALETTE_COLOR_HELP_REGION_BACKGROUND_4                    0xE4BC44
#define OSDMM_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_1                  0x5078FF
#define OSDMM_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_2                  0x92B1C7
#define OSDMM_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_3                  0xC693F0//0xDADCFE
#define OSDMM_PALETTE_COLOR_BUTTON_ENABLE_BACKGROUND_4                  0xD5CAB2
#define OSDMM_PALETTE_COLOR_ENABLE_BUTTON_STRING_1                      0xFFFFFF      
#define OSDMM_PALETTE_COLOR_ENABLE_BUTTON_STRING_2                      0xFFFFFF      
#define OSDMM_PALETTE_COLOR_ENABLE_BUTTON_STRING_3                      0xFFFFFF      
#define OSDMM_PALETTE_COLOR_ENABLE_BUTTON_STRING_4                      0xFFFFFF
#define OSDMM_PALETTE_COLOR_DLG_TITLE_BAR_1                             0x666666
#define OSDMM_PALETTE_COLOR_DLG_TITLE_BAR_2                             0x737FC5
#define OSDMM_PALETTE_COLOR_DLG_TITLE_BAR_3                             0xFDE85D
#define OSDMM_PALETTE_COLOR_DLG_TITLE_BAR_4                             0x62BD6A
#define OSDMM_PALETTE_COLOR_DLG_OUTER_FRAME_1                           0x555555
#define OSDMM_PALETTE_COLOR_DLG_OUTER_FRAME_2                           0x5E7070
#define OSDMM_PALETTE_COLOR_DLG_OUTER_FRAME_3                           0x4A5FA9
#define OSDMM_PALETTE_COLOR_DLG_OUTER_FRAME_4                           0xFBA925
#define OSDMM_PALETTE_COLOR_DLG_BACKGROUND_1                            0x949494
#define OSDMM_PALETTE_COLOR_DLG_BACKGROUND_2                            0xB0DBD6
#define OSDMM_PALETTE_COLOR_DLG_BACKGROUND_3                            0xABBFCA //0xAFB3FF//0x757BFF
#define OSDMM_PALETTE_COLOR_DLG_BACKGROUND_4                            0xFCFE96
     

#define OSDMM_PALETTE_COLOR_HELP_STRING                                 0x010101
#define OSDMM_PALETTE_COLOR_GENERAL_STRING_IN_FILE_WINDOW               0x010101
#define OSDMM_PALETTE_COLOR_HIGHLIGHT_BACKGROUND                        0xFFD900//0xFFA928               
#define OSDMM_PALETTE_COLOR_HIGHLIGHT_BUTTON_STRING                     0x010101           
#define OSDMM_PALETTE_COLOR_NORMAL_FRAME_BRIGHT                         0xAAAAAA//0xFFFFFF                
#define OSDMM_PALETTE_COLOR_NORMAL_FRAME_DARK                           0x202020                  
#define OSDMM_PALETTE_COLOR_SELECT_FRAME_BRIGHT                         0xA52100//0xFAC14E
#define OSDMM_PALETTE_COLOR_SELECT_FRAME_DARK                           0xA52100//0x836527
#define OSDMM_PALETTE_COLOR_SEPARATE_FRAME_CENTER                       0xC0C0C0
#define OSDMM_PALETTE_COLOR_UPFOLDER_ICON_TEXT_FOREGROUND               0xE7C22D
#define OSDMM_PALETTE_COLOR_UPFOLDER_ICON_SHADOW_1                      0x010101
#define OSDMM_PALETTE_COLOR_UPFOLDER_ICON_SHADOW_2                      0xD92F0B
#define OSDMM_PALETTE_COLOR_FOLDER_ICON_TEXT_FOREGROUND                 0xE7C22D         
#define OSDMM_PALETTE_COLOR_FOLDER_ICON_SHADOW_1                        0x010101
#define OSDMM_PALETTE_COLOR_FOLDER_ICON_SHADOW_2                        0x010101
#define OSDMM_PALETTE_COLOR_JPEG_ICON_TEXT_FOREGROUND                   0x00FFFF
#define OSDMM_PALETTE_COLOR_JPEG_ICON_SHADOW_1                          0x000000                  
#define OSDMM_PALETTE_COLOR_JPEG_ICON_SHADOW_2                          0xFFFFFF
#define OSDMM_PALETTE_COLOR_MP3_ICON_TEXT_FOREGROUND                    0xEA60A1 //0xFFFF00
#define OSDMM_PALETTE_COLOR_MP3_ICON_SHADOW_1                           0xAC3A70 //0xFFFFFF                  
#define OSDMM_PALETTE_COLOR_MP3_ICON_SHADOW_2                           0xFED9EB //0x000000
#define OSDMM_PALETTE_COLOR_WMA_ICON_TEXT_FOREGROUND                    0x3CFF1E
#define OSDMM_PALETTE_COLOR_WMA_ICON_SHADOW_1                           0xFFFFFF
#define OSDMM_PALETTE_COLOR_WMA_ICON_SHADOW_2                           0x000000
#define OSDMM_PALETTE_COLOR_OPENFOLDER_ICON_TEXT_FOREGROUND             0x000000
#define OSDMM_PALETTE_COLOR_OPENFOLDER_ICON_SHADOW_1                    0xEBC427
#define OSDMM_PALETTE_COLOR_OPENFOLDER_ICON_SHADOW_2                    0xB3982A
//#define OSDMM_PALETTE_COLOR_TIME_STRING                                 OSDMM_PALETTE_COLOR_TITLE_STRING                         
#define OSDMM_PALETTE_COLOR_DIVX_ICON_TEXT_FOREGROUND                   0x000000
#define OSDMM_PALETTE_COLOR_DIVX_ICON_SHADOW_1                          0xFFFFFF
#define OSDMM_PALETTE_COLOR_DIVX_ICON_SHADOW_2                          0xFFFF00
#define OSDMM_PALETTE_COLOR_MP2_ICON_TEXT_FOREGROUND                    0xE60000           
#define OSDMM_PALETTE_COLOR_MP2_ICON_SHADOW_1                           0xFFFFFF
#define OSDMM_PALETTE_COLOR_MP2_ICON_SHADOW_2                           0x000000
#define OSDMM_PALETTE_COLOR_DAT_ICON_TEXT_FOREGROUND                    0x000000
#define OSDMM_PALETTE_COLOR_DAT_ICON_SHADOW_1                           0xFFFFFF
#define OSDMM_PALETTE_COLOR_DAT_ICON_SHADOW_2                           0x00FF00
#define OSDMM_PALETTE_COLOR_VOB_ICON_TEXT_FOREGROUND                    0x000000
#define OSDMM_PALETTE_COLOR_VOB_ICON_SHADOW_1                           0xFFFFEA    
#define OSDMM_PALETTE_COLOR_VOB_ICON_SHADOW_2                           0xFF0000
#define OSDMM_PALETTE_COLOR_VOLUME_SERVO_ICON_TEXT_FOREGROUND           0xEC0000
#define OSDMM_PALETTE_COLOR_VOLUME_SERVO_ICON_SHADOW_1                  0xFFFFFC
#define OSDMM_PALETTE_COLOR_VOLUME_SERVO_ICON_SHADOW_2                  0x000000
#define OSDMM_PALETTE_COLOR_VOLUME_USB_ICON_TEXT_FOREGROUND             0xFF0000
#define OSDMM_PALETTE_COLOR_VOLUME_USB_ICON_SHADOW_1                    0xFFFFFF
#define OSDMM_PALETTE_COLOR_VOLUME_USB_ICON_SHADOW_2                    0x000000
#define OSDMM_PALETTE_COLOR_VOLUME_CARD_READER_ICON_TEXT_FOREGROUND     0xFFFFFF
#define OSDMM_PALETTE_COLOR_VOLUME_CARD_READER_ICON_SHADOW_1            0xFF0000
#define OSDMM_PALETTE_COLOR_VOLUME_CARD_READER_ICON_SHADOW_2            0x000000
#define OSDMM_PALETTE_COLOR_BUTTON_HIGHLIGHT_BACKGROUND                 0xFFDD66
#define OSDMM_PALETTE_COLOR_HIGHLIGHT_STRING_IN_FILE_WINDOW             0x010101
#define OSDMM_PALETTE_COLOR_DLG_STRING                                  0x010101
#define OSDMM_PALETTE_COLOR_CHECKED_ICON_TEXT_FOREGROUND                0xFFE600//0xFFFF00
#define OSDMM_PALETTE_COLOR_CHECKED_ICON_SHADOW_1                       0xFFE600 //0xFFFF00
#define OSDMM_PALETTE_COLOR_CHECKED_ICON_SHADOW_2                       0x2668F8 //0x1C1CFF
#define OSDMM_PALETTE_COLOR_UNCHECKED_ICON_TEXT_FOREGROUND              0x1C1CFF
#define OSDMM_PALETTE_COLOR_RADIO_ICON_TEXT_FOREGROUND                  0xFFFF00//0xFA910A
#define OSDMM_PALETTE_COLOR_RADIO_ICON_SHADOW_1                         0x2668F8//0x0000FF
#define OSDMM_PALETTE_COLOR_UNRADIO_ICON_TEXT_FOREGROUND                0x2668F8//0x0000FF
#define OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_LINE_0                       0x299501                 
#define OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_LINE_1                       0x2AC910                
#define OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_LINE_2                       0xF5FF3C
#define OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_LINE_3                       0xFFA928
#define OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_LINE_4                       0xCF330C
#define OSDMM_PALETTE_COLOR_AUDIO_SPECTRUM_DROP                         0x666666
#define OSDMM_PALETTE_COLOR_INFO_REGION_FRAME                           0x555555//0xFFFFFF
#define OSDMM_PALETTE_COLOR_BUTTON_FRAME                                0x555555//0xFFFFFF
#define OSDMM_PALETTE_COLOR_BUTTON_DISABLE_BACKGROUND                   0xACACAC
#define OSDMM_PALETTE_COLOR_DISABLE_BUTTON_STRING                       0xFFFFFF
#define OSDMM_PALETTE_COLOR_AC3_ICON_TEXT_FOREGROUND                    0xFFFFFF        
#define OSDMM_PALETTE_COLOR_MPG_ICON_TEXT_FOREGROUND                    0xFFFFFF
#define OSDMM_PALETTE_COLOR_MP4_ICON_TEXT_FOREGROUND                    0xFFFF00
#define OSDMM_PALETTE_COLOR_MP4_ICON_SHADOW_1                           0xFFFFFF
#define OSDMM_PALETTE_COLOR_MP4_ICON_SHADOW_2                           0x000000


//define the fixed palette entry for the button
#define OSDMM_PALETTE_ENTRY_BUTTON                                      0x05

//define the messages for OSDMM_Update
#define OSDMM_MSG_INITIAL                       0
#define OSDMM_MSG_EXIT                          1
#define OSDMM_MSG_UPDATE_HIGHLIGHT              2
#define OSDMM_MSG_CLEAR_HIGHLIGHT               3
#define OSDMM_MSG_UPDATE_CHANGE_DIR             4
#define OSDMM_MSG_UPDATE_PROGRAM_ITEM           5
#define OSDMM_MSG_UPDATE_FILE_WINDOW_NEXT       6
#define OSDMM_MSG_UPDATE_FILE_WINDOW_PREVIOUS   7
#define OSDMM_MSG_UPDATE_CHANGE_DIR_LEVEL       8
#define OSDMM_MSG_CHANGE_MODE                   9
#define OSDMM_MSG_PLAYING                       10
#define OSDMM_MSG_STOP                          11
#define OSDMM_MSG_SHOW_DLG                      12
#define OSDMM_MSG_CLEAR_DLG                     13
#define OSDMM_MSG_UPDATE_DLG_ITEM               14
#define OSDMM_MSG_REMOVE_ACTIVE_VOLUME          15
#define OSDMM_MSG_REMOVE_NOT_ACTIVE_VOLUME      16
#define OSDMM_MSG_UPDATE_HELP                   17
#define OSDMM_MSG_PROGRAM_DELETE_ALL            18
#define OSDMM_MSG_CLEAR_ACTIVE_PLAYING_FILE     19
#define OSDMM_MSG_PROGRAM_DELETE                20
#define OSDMM_MSG_JPEG_PREVIEW                  21
#define OSDMM_MSG_ENABLE_BUTTON                 22
#define OSDMM_MSG_DISABLE_BUTTON                23
#define OSDMM_MSG_SAVE                          24

//define the parameter for OSDMM_MSG_INITIAL
#define OSDMM_INITIAL_MODE_GENERAL              0
#define OSDMM_INITIAL_MODE_COPY                 1


#define MM_ROOT                                 0
//********************************************************************************************************
//Export APIs
//********************************************************************************************************
void OSDMM_Update(BYTE bMessage, BYTE bParam);
// LLY2.53, expand time value unit from WORD to DWORD
//void OSDMM_Trigger(BYTE bType, WORD wTime);
void OSDMM_Trigger(BYTE bType, DWORD dwTime);
BYTE OSDMM_Recover(BYTE bRecoverRegion);
void OSDMM_ClearRegion(BYTE bRegion);
void OSDMM_ResetControl(void);
void OSDMM_ConfigOSDRegion(void);
void OSDMM_TuneRegionForPALMode(void);

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW
void OSDMM_ShowSaveDlg(void);
void OSDMM_DeleteSaveDlg(void);
BYTE OSDMM_SaveDlgProcessKey(BYTE bKey);
void OSDMM_ShowDeleteDlg(void);
void OSDMM_DeleteDeleteDlg(void);
BYTE OSDMM_DeleteDlgProcessKey(BYTE bKey);
void OSDMM_SaveDlgConfigOSDRegion(void);
void OSDMM_CopyDeleteDlg_Exit(BYTE bExitMode);
BYTE OSDMM_CopyDeleteDlg_Recover(BYTE bRecoverRegion);
#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW

#ifdef SUPPORT_AUDIO_EFFECT
void OSDMM_ShowAudioEQ(BYTE bEQType);
#endif

//********************************************************************************************************
//Internal APIs
//********************************************************************************************************
void _OSDMM_InitializeGDI(void);
void _OSDMM_Exit(void);
void _OSDMM_Initial(BYTE bMode);
void _OSDMM_SetFrameAttribute(BYTE bStyle);
void _OSDMM_DrawBigFrame(void);
void _OSDMM_DrawSeparateFrame(BYTE bKind);
void _OSDMM_DrawLeftFileWindow(void);
void _OSDMM_InitializeStringPalette(BYTE bRegion);
void _OSDMM_DrawRightFileWindow(void);
void _OSDMM_UpdateHighlight(BYTE bUpdate);
void _OSDMM_ClearRegion(BYTE bRegion);
void _OSDMM_CopyFileString(WORD wRelativeIndex);
void _OSDMM_DrawSelectedFrame(BYTE bUpdate, BYTE bIndex);
void _OSDMM_CopyDirString(BYTE bID, WORD wWidth);
void _OSDMM_UpdateRightScrollBar(void);
void _OSDMM_DrawVolume(BYTE bIndex, BYTE bVolumeID, BYTE bHighlight);
void _OSDMM_SetVolumeString(BYTE bVolumeID);
void _OSDMM_DrawAllVolumes(void);
void _OSDMM_DrawDir(BYTE bIndex);
void _OSDMM_DrawLeftFileWindowIcon(BYTE bIndex, BYTE bIconType, BYTE bHighlight);
BYTE _OSDMM_HaveUpDIR(void);
void _OSDMM_DrawUpDIR(BYTE bIndex, BYTE bHighlight);
void _OSDMM_UpdateLeftScrollBar(void);
void _OSDMM_DrawSerialNumber(BYTE bIndex, WORD wNum);
void _OSDMM_ChangeDir(void);
void _OSDMM_ShowActiveDir(void);
void _OSDMM_UpdateStartDrawItemByNewCursor(void);
void _OSDMM_ClearItemRegionInFileWindow(BYTE bFileWindow, BYTE bIndex);
void _OSDMM_ChangeDirLevel(void);
void _OSDMM_ChangePage(BYTE bChange);
void _OSDMM_UpdateProgramItem(void);
BYTE _OSDMM_GetIconType(BYTE bExtID);
void _OSDMM_SetIconAttribute(BYTE bIconType, BYTE bHighlight);
void _OSDMM_DrawRightFileWindowIcon(BYTE bIndex, BYTE bIconType, BYTE bHighlight);
void _OSDMM_PL_CopyFileString(BYTE bProgramIndex);
void _OSDMM_PL_OutputItemString(BYTE bProgramIndex, BYTE bPosIndex, BYTE bHighlight);
void _OSDMM_PL_DrawRightFileWindow(void);
void _OSDMM_PrepareUpdateTitle(void);
void _OSDMM_UpdateTitle(void);
void _OSDMM_UpdateFolderTitle(BYTE bDirID);
void _OSDMM_UpdateFileTitle(void);
void _OSDMM_ClearTitleSubRegion(WORD wPos);
void _OSDMM_DrawButton(BYTE bIndex, BYTE bHighlight);
void _OSDMM_DrawAllButtons(void);
void _OSDMM_DrawRightFileWindowHighlight(BYTE bUpdate, BYTE bFileLength);
void _OSDMM_UpdateHelp(BYTE bHelpType);
void _OSDMM_ChangProramModeToNormal(void);
void _OSDMM_UpdatePlayingTitle(void);
void _OSDMM_ChangMode(BYTE bChangeMode);
void _OSDMM_DLG_DrawFileTypeItem(BYTE bFileType, BYTE bHighlight);
void _OSDMM_DLG_DrawSortMethodItem(BYTE bSet, BYTE bMethod, BYTE bHighlight);
void _OSDMM_DLG_DrawButton(BYTE bHighlight);
void _OSDMM_DLG_DrawSortMethodItem(BYTE bSet, BYTE bMethod, BYTE bHighlight);
void _OSDMM_DLG_DrawSortSequenceItem(BYTE bSet, BYTE bSequence, BYTE bHighlight);
void _OSDMM_ShowFileTypeDialogBox(void);
void _OSDMM_ShowSortFileDialogBox(void);
void _OSDMM_DLG_DrawDlgBackground(BYTE bDlg);
void _OSDMM_DLG_DrawItemRegionInFileType(BYTE bDraw, BYTE bIndex);
void _OSDMM_DLG_DrawItemRegionInSortFile(BYTE bDraw, BYTE bIndex);
void _OSDMM_DLG_DrawAllMethodItemsInSortFile(BYTE bHighlight);
void _OSDMM_DLG_DrawAllSequenceItemsInSortFile(BYTE bHighlight);
void _OSDMM_PL_DrawLeftFileWindow(void);
void _OSDMM_FlashPlayingFile(void);
void _OSDMM_DeleteProgramItem(void);
void _OSDMM_ShowAudioSpectrum(void);
void _OSDMM_DrawSpectrumLine(BYTE bOldValue, BYTE bNewValue, BYTE bIndex);
void _OSDMM_DrawSpectrumRange(BYTE bRange, BYTE bIndex, BYTE bUpdate);
void _OSDMM_DrawSpectrumDrop(BYTE bRange, BYTE bIndex, BYTE bUpdate);
void _OSDMM_SetSpectrumDrop(BYTE bType, BYTE bOldRange, BYTE bNewRange, BYTE bIndex, BYTE bShift, DWORD dwDrop);
void _OSDMM_ChangeSpectrumDrop(BYTE bType, BYTE bRange, BYTE bIndex, BYTE bShift, DWORD dwDrop);
void _OSDMM_UpdateSpectrumLowLines(BYTE bIndex, DWORD dwExtractBits, DWORD dwKeepBits);
void _OSDMM_UpdateSpectrumHighLines(BYTE bIndex, DWORD dwExtractBits, DWORD dwKeepBits);
void _OSDMM_ResetAudioSpectrum(void);
void _OSDMM_DrawBackground(void);
void _OSDMM_DrawInfoRegion(void);
void _OSDMM_ClearJPEGPreviewRegion(BYTE bTransparent);
void _OSDMM_StopAudioPlayingFile(void);
WORD _OSDMM_ShowAudioInfo(WORD wStartH);
void _OSDMM_ShowAudioTotalTime(void);
void _OSDMM_DrawString(void);
#ifdef SUPPORT_UNICODE_CODING
void _OSDMM_DrawStringByUnicode(void);
#endif

#ifdef SUPPORT_COPY_FILE_IN_MM_UI
void _OSDMM_ShowSaveFileDlg(void);
void _OSDMM_DeleteSaveFileDlg(void);
void _OSDMM_UpdateSaveFileProgress(void);
#endif //SUPPORT_COPY_FILE_IN_MM_UI

//********************************************************************************************************
//Extern global variables
//********************************************************************************************************
extern BYTE __bActiveDirChange;
extern WORD __wOSDMMStartDrawItem;

//********************************************************************************************************
//data structure
//********************************************************************************************************
typedef struct tagSPECTRUM
{
    BYTE bRangeNum;
    BYTE bColor;
} SPECTRUM, *PSPECTRUM;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define OSDMM_DIALOG_BUTTON_ID_YES                  0
#define OSDMM_DIALOG_BUTTON_ID_NO                   1

#ifdef OSDMM_MAIN_FILE_DECLARATION
#ifdef SUPPORT_COPY_FILE_IN_MM_UI
#include "dialog.h"
#include "OSDString\\strextrn.h"

//The dialog is for notifying user that we are saving the files to flash
#define OSDMM_SAVEFILE_DIALOG_WIDTH						360
#define OSDMM_SAVEFILE_DIALOG_HEIGHT                    200
#define OSDMM_SAVEFILE_DIALOG_BUTTON_NUM                0
#define OSDMM_SAVEFILE_DIALOG_CONTENT_DISTANCE_V        40

//Define the position of the progress bar.
#define OSDMM_DISTANCE_FONT_PROGRESS_BAR                8
#define OSDMM_PROGRESS_BAR_WIDTH                        200
#define OSDMM_PROGRESS_BAR_HEIGHT                       12
#define OSDMM_SAVEFILE_PROGRESS_POS_H                   40
#define OSDMM_SAVEFILE_PROGRESS_POS_V                   80

DIALOG _OSDMMSaveFileDlg = {
    ((OSDMM_REGION_WIDTH-OSDMM_SAVEFILE_DIALOG_WIDTH)>>1),
    ((OSDMM_OSD_REGION_HEIGHT-OSDMM_SAVEFILE_DIALOG_HEIGHT)>>1),
    OSDMM_SAVEFILE_DIALOG_WIDTH,
    OSDMM_SAVEFILE_DIALOG_HEIGHT,
    aMMSaveFileDlgTitle,
    DIALOG_TITLE_STRING_ALIGNMENT_CENTER,
    OSDMM_SAVEFILE_DIALOG_BUTTON_NUM,
    0,
    0,
    0,
    NULL
};

#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW

//Button action
#define OSDMM_SAVE_DIALOG_BUTTON_ACTION_NONE        0xFF             

#define OSDMM_SAVE_DIALOG_WIDTH				        300
#define OSDMM_SAVE_DIALOG_HEIGHT                    200
#define OSDMM_SAVE_DIALOG_BUTTON_NUM                2
#define OSDMM_SAVE_DIALOG_CONTENT_DISTANCE_V        40

#define OSDMM_SAVE_DIALOG_REGION_WIDTH              600
#define OSDMM_SAVE_DIALOG_REGION_HEIGHT             400


DLG_BUTTON _SaveDialogButton[OSDMM_SAVE_DIALOG_BUTTON_NUM] = {
    {aBUTTONYes, OSDMM_DIALOG_BUTTON_ID_YES, DIALOG_BUTTON_START_X_OPTIMIZED},
    {aBUTTONNo, OSDMM_DIALOG_BUTTON_ID_NO, DIALOG_BUTTON_START_X_OPTIMIZED},
};

DIALOG _OSDMMSaveDlg = {
    ((OSDMM_SAVE_DIALOG_REGION_WIDTH-OSDMM_SAVE_DIALOG_WIDTH)>>1),
    ((OSDMM_SAVE_DIALOG_REGION_HEIGHT-OSDMM_SAVE_DIALOG_HEIGHT)>>1),
    OSDMM_SAVE_DIALOG_WIDTH,
    OSDMM_SAVE_DIALOG_HEIGHT,
    aMMSaveDlgTitle,
    DIALOG_TITLE_STRING_ALIGNMENT_CENTER,
    OSDMM_SAVE_DIALOG_BUTTON_NUM,
    OSDMM_DIALOG_BUTTON_ID_YES,
    DIALOG_BUTTON_WIDTH_OPTIMIZED,
    DIALOG_BUTTON_START_Y_OPTIMIZED,
    _SaveDialogButton,
};

DIALOG _OSDMMDeleteDlg = {
    ((OSDMM_SAVE_DIALOG_REGION_WIDTH-OSDMM_SAVE_DIALOG_WIDTH)>>1),
    ((OSDMM_SAVE_DIALOG_REGION_HEIGHT-OSDMM_SAVE_DIALOG_HEIGHT)>>1),
    OSDMM_SAVE_DIALOG_WIDTH,
    OSDMM_SAVE_DIALOG_HEIGHT,
    aMMDeleteDlgTitle,
    DIALOG_TITLE_STRING_ALIGNMENT_CENTER,
    OSDMM_SAVE_DIALOG_BUTTON_NUM,
    OSDMM_DIALOG_BUTTON_ID_YES,
    DIALOG_BUTTON_WIDTH_OPTIMIZED,
    DIALOG_BUTTON_START_Y_OPTIMIZED,
    _SaveDialogButton,
};

#endif //SUPPORT_COPY_FILE_IN_SLIDE_SHOW
#endif //SUPPORT_COPY_FILE_IN_MM_UI
#endif //#ifdef OSDMM_MAIN_FILE_DECLARATION
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__OSDMM_H__
