/*****************************************************************************

  panel.h   -   PANEL  functions, types, and definitions

                Version 2.00

                Copyright (c) 2000, Cheertek Corp. All rights reserved. *

******************************************************************************/

#ifndef __PANEL_H__
#define __PANEL_H__

//  *** Declare Function ***********************************************
BYTE PANEL_KeyScan(void);
void PANEL_Initial(void);
void PANEL_Output (BYTE bMessage, WORD wParam);
// LLY2.53, expand OSD time unit from WORD to DWORD
//void PANEL_Trigger(WORD wTime);
void PANEL_Trigger(DWORD dwTime);

#define Pannel_running
#define Col_Flash 
//  *** Mode setting for VFD panel ***************************************
//  For VFD panels, maybe there are different mode setting suitable for it
//  If the mode setting isn't correct, lighting will be wrong.

// All bytes on VFD panel
#define    VFD_GRID_ARRAY                         10    // Select a correct one!
                                                        // 14 for PT6961
                                                        // 18 for WINBOND 66880 NEC-16311's 8d/20s
                                                        // 12 for WINBOND 66881 NEC-16312's 6d/16s

// Mode setting command
#define    VFD_DISP_MODE                        0x02    // Select a correct one!
                                                        // 0x02 for PT6961 LED driver 6g/12s
                                                        // 0x03 for PT6961 LED driver 7g/11s
                                                        // 0x02 for WINBOND 66881 6g/20s
                                                        // 0x02 for WINBOND 66880 6g/16s
                                                        // 0x00 for NEC-16311's 8d/20s
                                                        // 0x02 for NEC-16312 6d/16s
                                                        // 0x05 for NEC_16312 9d/13s
                                                        // 0xFD for PNJU3421 13d/15s
                                                        // 0xF8 for PNJU3421 8d/15s

// Display control command : set pulse width
#define    VFD_CTRL_MODE                        0x8f    // Pulse width=14/16 for pulse width
                                                        // 0x8F for PT6961
                                                        // 0x8F for WINBOND 66880/66881
                                                        // 0x8F for PD16312/16311
                                                        // 0xFD for PNJU3421
                                                        // 0xF8 for PNJU3421

// Data Setting Command
#define    SETCOMMAND_WRITEDISPLAY              0x40    // Select a correct one!
                                                        // 0x40 for PT6961
#define    SETCOMMAND_READKEY                   0x42    // Select a correct one!
                                                        // 0x42 for PT6961
#define    SET_ADDRESS                          0xc0    // Select a correct one!
                                                        // 0xC0 for PT6961

// *** Definition useful for panel keyscan **************************
//"KeyScanByte" total bytes from key matrix
// WINBOND 66880/66881 Keyscan data 3 byte
// 16312 read back 3 bytes : 0~2 bytes for key Matrix & 3 for SW data;
// 16311 uses 3 ~ 6 (max)

#define    MAX_KEY_SCAN                            24    // Select a correct one!
                                                         // 24 for PT6312
                                                         // 40 for PT6961
                                                         // 48 for PT6311

// *** Definition Panel 8888 8:88:88 total LETTER can display ********
// if total LETTER <> 9, will modify panel.c
#define    VFD_LETTNUM                              10
#define    VFD_NUM                                  3

// *** VFD_LED light data ********************************************
#define     VFD_LED_ADDR                           9
#define     VFD_LED0                             0x01
#define     VFD_LED1                             0x02
//#define     VFD_LED2                             0xFB
//#define     VFD_LED3                             0xF7

// *** for read SW data **********************************************
//#define SW_DATA
/*******************************************************/

/*************** for ligth LED *************************/
//#define LIGHT_LED
/******************************************************/


#define         CIRCLE_PARTS                        3

//-----------------------------------------
#define    VFD_UNUSED_ADDR                          1
#define    VFD_UNUSED                            0x00

//chapter title hour:min:secvposition
#define    VFD_TITLE_NUM_ADDR         VFD_UNUSED_ADDR
#define    VFD_TITLE_TEN_ADDR         VFD_UNUSED_ADDR
#define    VFD_TITLE_ADDR             VFD_UNUSED_ADDR
#define    VFD_TITLE                  VFD_UNUSED

#define    VFD_CHAPTER_NUM_ADDR       VFD_UNUSED_ADDR
#define    VFD_CHAPTER_TEN_ADDR       VFD_UNUSED_ADDR
#define    VFD_CHAPTER_ADDR           VFD_UNUSED_ADDR
#define    VFD_CHAPTER                VFD_UNUSED

#define    VFD_HOUR_NUM_ADDR          VFD_UNUSED_ADDR

#define    VFD_HOUR_COL_ADDR                       0
#define    VFD_HOUR_COL                          0x80

#define    VFD_MIN_NUM_ADDR           VFD_UNUSED_ADDR
#define    VFD_MIN_TEN_ADDR           VFD_UNUSED_ADDR
#define    VFD_MIN_ADDR               VFD_UNUSED_ADDR
#define    VFD_MIN                    VFD_UNUSED

#define    VFD_MIN_COL_ADDR           VFD_UNUSED_ADDR
#define    VFD_MIN_COL                VFD_UNUSED

#define    VFD_SEC_NUM_ADDR           VFD_UNUSED_ADDR
#define    VFD_SEC_TEN_ADDR           VFD_UNUSED
#define    VFD_SEC_ADDR               VFD_UNUSED_ADDR
#define    VFD_SEC                    VFD_UNUSED

//TITLE CHAPTER TRACK CH
#define    VFD_TRACK_ADDR             VFD_UNUSED_ADDR
#define    VFD_TRACK                  VFD_UNUSED

//REPEAT one all A->B  position  ----------------------
#define    VFD_REPEAT_ADDR            VFD_UNUSED_ADDR
#define    VFD_REPEAT                 VFD_UNUSED
#define    VFD_REP_ALL_ADDR           VFD_UNUSED_ADDR
#define    VFD_REP_ALL                VFD_UNUSED
#define    VFD_REP_1_ADDR             VFD_UNUSED_ADDR
#define    VFD_REP_1                  VFD_UNUSED
#define    VFD_REP_A_ADDR             VFD_UNUSED_ADDR
#define    VFD_REP_A                  VFD_UNUSED
#define    VFD_REP_B_ADDR             VFD_UNUSED_ADDR
#define    VFD_REP_B                  VFD_UNUSED


// Audio L/R Dolby 5.1 DTS position --------------------
#define    VFD_L_ADDR                 VFD_UNUSED_ADDR
#define    VFD_L                      VFD_UNUSED
#define    VFD_R_ADDR                 VFD_UNUSED_ADDR
#define    VFD_R                      VFD_UNUSED
#define    VFD_LR_SLASH_ADDR          VFD_UNUSED_ADDR
#define    VFD_LR_SLASH               VFD_UNUSED

#define    VFD_DOLBY_ADDR                          6
#define    VFD_DOLBY                             0x20
#define    VFD_51CH_ADDR              VFD_UNUSED_ADDR
#define    VFD_51CH                   VFD_UNUSED
#define    VFD_DTS_ADDR               VFD_UNUSED_ADDR
#define    VFD_DTS                    VFD_UNUSED
#define    VFD_DIGITAL_ADDR           VFD_UNUSED_ADDR
#define    VFD_DIGITAL                VFD_UNUSED

// DVD ANGLE position-----------------------------------------
#define    VFD_ANGLE_ADDR             VFD_UNUSED_ADDR
#define    VFD_ANGLE                  VFD_UNUSED

// DVD SVCD VCD CD disc type position -------------------------
#define    VFD_DVD_ADDR                            6
#define    VFD_DVD                               0x08
#define    VFD_SVCD_ADDR              VFD_UNUSED_ADDR
#define    VFD_SVCD                   VFD_UNUSED
#define    VFD_VCD_ADDR                            6
#define    VFD_VCD                               0x02
#define    VFD_CD_ADDR                             6
#define    VFD_CD                                0x01
#define    VFD_MP3_ADDR                            6
#define    VFD_MP3                               0x04
#define    VFD_JPEG_ADDR                           6
#define    VFD_JPEG                              0x10

// PRO REMAIN PBC PLAY PAUSE play status position---------------
#define    VFD_PGM_ADDR               VFD_UNUSED_ADDR
#define    VFD_PGM                    VFD_UNUSED
#define    VFD_RAN_ADDR               VFD_UNUSED_ADDR
#define    VFD_RAN                    VFD_UNUSED
#define    VFD_REMAIN_ADDR            VFD_UNUSED_ADDR
#define    VFD_REMAIN                 VFD_UNUSED

#define    VFD_PBC_ADDR               VFD_UNUSED_ADDR
#define    VFD_PBC                    VFD_UNUSED

#define    VFD_PLAY_ADDR                           6
#define    VFD_PLAY                              0x80
#define    VFD_PAUSE_ADDR                          6
#define    VFD_PAUSE                             0x40

// KEY_NORMAL  KEY_UP  KEY_DOWN position-------------------------
#define    VFD_KEYNORMAL_ADDR         VFD_UNUSED_ADDR
#define    VFD_KEYNORMAL              VFD_UNUSED
#define    VFD_KEYDOWN_ADDR           VFD_UNUSED_ADDR
#define    VFD_KEYDOWN                VFD_UNUSED
#define    VFD_KEYUP_ADDR             VFD_UNUSED_ADDR
#define    VFD_KEYUP                  VFD_UNUSED

#define    VFD_KARAOKE_ADDR           VFD_UNUSED_ADDR
#define    VFD_KARAOKE                VFD_UNUSED

//Others position ------------------------------------------------
#define    VFD_CH_ADDR                VFD_UNUSED_ADDR
#define    VFD_CH                     VFD_UNUSED
#define    VFD_ZOOM_ADDR              VFD_UNUSED_ADDR
#define    VFD_ZOOM                   VFD_UNUSED
#define    VFD_LAST_ADDR              VFD_UNUSED_ADDR
#define    VFD_LAST                   VFD_UNUSED
#define    VFD_INDEX_ADDR             VFD_UNUSED_ADDR
#define    VFD_INDEX                  VFD_UNUSED
#define    VFD_PARENT_KEY_ADDR        VFD_UNUSED_ADDR
#define    VFD_PARENT_KEY             VFD_UNUSED

// added for DVD "CIRCLE" display (297GK, 25U48109)-----------------------
#define    VFD_CS_IN_ADDR             VFD_UNUSED_ADDR
#define    VFD_CS_IN                  VFD_UNUSED
#define    VFD_CS1_ADDR               VFD_UNUSED_ADDR
#define    VFD_CS1                    VFD_UNUSED
#define    VFD_CS2_ADDR               VFD_UNUSED_ADDR
#define    VFD_CS2                    VFD_UNUSED
#define    VFD_CS3_ADDR               VFD_UNUSED_ADDR
#define    VFD_CS3                    VFD_UNUSED

#define    VFD_CS_ALL_ADDR            VFD_UNUSED_ADDR
#define    VFD_CS_ALL                 VFD_UNUSED

//------------------------------------------------------------------------
//Letter Pattern Definition
#define    NUM_0                                 0x3f
#define    NUM_1                                 0x06
#define    NUM_2                                 0x5b
#define    NUM_3                                 0x4f
#define    NUM_4                                 0x66
#define    NUM_5                                 0x6d
#define    NUM_6                                 0x7d
#define    NUM_7                                 0x07
#define    NUM_8                                 0x7f
#define    NUM_9                                 0x6f

#define    LETT_O                               NUM_0
#define    LETT_P                                0x73
#define    LETT_E                                0x79
#define    LETT_N                                0x37
#define    LETT_L                                0x38
#define    LETT_F                                0x71

#define    LETT_n                                0x54
#define    LETT_o                                0x5c
#define    LETT_d                                0x5e
#define    LETT_I                               NUM_1
#define    LETT_S                               NUM_5
#define    LETT_C                                0x39

#define    LETT_b                                0x7c
#define    LETT_c                                0x58
#define    LETT_A                                0x77

#define    LETT_t                                0x78
#define    LETT_r                                0x50

#define    LETT_g                               NUM_9
#define    LETT_Y                                0x6e
#define    LETT_DASH                             0x40
#define    LETT_U                                0x3e
#endif  // __PANEL_H__
