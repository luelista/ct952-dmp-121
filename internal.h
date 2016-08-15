
#ifndef __INTERNAL_H__
#define __INTERNAL_H__
#ifdef __cplusplus
    extern  "C" {
#endif

// wyc2.17-909S, move to here
// set different peripherials
// [DECODER_SYTEM]
#define		DVD909_EVAL		        1
// wyc1.07-909, re-name define to DVD909_HFY
#define     DVD909_HFY              2
// LLY1.50, define a name for 909S evaluation board
#define     DVD909S_EVAL            3
// LLY2.11, define a name for 909R evaluation board
#define     DVD909R_EVAL            4
// LLY2.15, define a name for CT908R-HFY system board
#define     DVD909R_HFY             5
// LLY2.16, define a name for CT219_HCN system board
#define     STB219_HCN              6
// LLY2.16, define a name for CT219_YHI
#define     STB219_YHI              7
#define     DVD909P_A_EVAL          8    // for CT909P-A 128 pin platform.
#define     DVD909P_B_EVAL          9    // for CT909P-B 128 pin platform.
// LLY2.75, rename DVD909P_EVAL_P3 as DVD956A_DEV
//#define     DVD909P_EVAL_P3         10   // for CT909P 216 pin platform.
#define     DVD956A_DEV             10
// LLY2.55, define a name for CT952A evaluation board
#define     DMP952A_EVAL            11
// LLY2.56, define a name for CT909G evaluation board
#define     DVD909G_EVAL            12
// LLY2.60, define a name for CT956A demo board
#define     DMP956A_EVAL            13
// LLY2.60, define a name for CT955A demo board
#define     DMP955A_EVAL            14
// Joey2.78, define a name for CT952B evaluation board
#define     DMP952B_EVAL            15

#define     DRAM_SIZE_UNUSED    0
#define     DRAM_SIZE_16        1
#define     DRAM_SIZE_32        2
#define     DRAM_SIZE_64        3

//CoCo2.76
#define     FLASH_SIZE_4        0
#define     FLASH_SIZE_8        1
#define     FLASH_SIZE_16       2
#define     FLASH_SIZE_32       3

// define the sell destination for OSD language selection
// [SELL_DESTINATION]
#define CHINA                   1
#define TAIWAN                  2
#define HONGKONG                3
#define FRANCE                  4
#define GERMANY                 5
#define ITALY                   6
#define SPAIN                   7
#define PORTUGAL                8
#define EUROPE					9
#define BRITISH                 10
#define JAPAN                   11  // LLY.276-5
#define KOREA                   12  // Brian1.07, Jeff 20030917 Add for Korean OSD
//////////////////////////////////////////////////////////////////////////////////////////

//DRAM size
#define     DRAM_CONFIGURATION_TYPE             DRAM_SIZE_16

//CoCo2.76, platform configuration.
//Need to modify the platform configuration before you making code.

// 1. DECODER_SYSTEM
// Must choose from [DECODER_SYSTEM] for system board
#ifndef DECODER_SYSTEM
#define     DECODER_SYSTEM      DMP952A_EVAL
#endif

// 2. Support features.
// #define SUPPORT_STB
#define SUPPORT_DMP

// 3. Modify platform. The define is in platform.h (#define CT909R_IC_SYSTEM/#define CT909P_IC_SYSTEM)

// 4. Flash size.
#define FLASH_SIZE                              FLASH_SIZE_16

//5. Sell Destination==>for OSD language
#define SELL_DESTINATION                        EUROPE
//////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//
//  DRAM Allocation Table
//
////////////////////////////////////////////////////////////////////////////////////////

#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_16)

#define     DRAM_CONFIGURATION_TYPE_DVD       DRAM_SIZE_16
#define     DRAM_CONFIGURATION_TYPE_DVB       DRAM_SIZE_UNUSED

// wyc2.17-909S, JPG frame buffer only keep one when 16M solution.
#define JPG_SINGLE_FRAME_BUFFER
#define JPG_FRAME_640_480_MODE

// Alan, prestop before enter Setup and Program UI
// wyc2.22-909s, remove one same define here.
#define PRESTOP_BEFORE_SETUP_AND_PROGRAM
#define BORROW_FRAME_BUFFER
// Alan, simple style for search UI in 16M solution
#define SIMPLE_SEARCH_UI

// External subtitle use SPU buffer instead of OSD buffer
#define EX_SP_USE_SPU_BUFFER
#define EX_SP_POS_INDEPEND_OSDND
#define EX_SP_SHARE_WITH_FIRMWARE_BUFFER

#define FLASH_INT_OFF

#define FONT_TABLE_METHOD    CREATE_DYNAMICALLY
#define USE_MINI_OSD_REGION

#define SUPPORT_GDI_FILL_RECT_BY_SW

#define SUPPORT_SIMPLE_DISPLAY_MODE //Support simple display mode for 16M solution.
#define USE_4B_MODE_OSD
#define SUPPORT_FBR_MODE
#define SUPPORT_BFR_MODE
// #define NO_DRM
// #define NO_SCREEN_SAVER     // Don't load screen-saver

#ifdef SUPPORT_GRAPH_ZOOM
#undef SUPPORT_GRAPH_ZOOM
#endif

//#ifdef SUPPORT_WMA
//#undef SUPPORT_WMA
//#endif
#define SUPPORT_WMA

#ifdef SUPPORT_OSDGAME
#undef SUPPORT_OSDGAME
#endif

// wyc2.35a,
//#ifdef SUPPORT_UDF_FILE_SYSTEM
#define SUPPORT_UDF_FILE_SYSTEM
//#endif

#ifdef SUPPORT_NAVIGATOR
#undef SUPPORT_NAVIGATOR
#endif

#define SUPPORT_SIMPLE_DISPLAY_MODE
#define REMOVE_SETUP_SPEAKER_TEST
#define NO_DIGEST
#define REMOVE_SETUP_ICON
#define REMOVE_SETUP_HELP_MESSAGE
#define LOGO_FROM_FLASH
// wyc2.76-909P,
#ifdef CT909P_IC_SYSTEM
#define SMALL_SOURCE_BUFFER
#endif //

// LLY2.31, don't change MPLL clock during power down mode while 16M solution.
// Since, SPI data can't be read-back while MPLL 146 --> 54MHz
// LLY2.36, remove this limitation since F/W will do necessary MPLL setting for SPI Flash
//#define NOT_POWERDOWN_MPLL

// LLY2.36, if define it, it will config AVI index buffer, SP buffer, servo buffer by F/W itself
#define SET_AVI_IDX_SP_SERVO_BUFF_RUNTIME

// LLY2.53, porting Chuan's code to always enable debug and unzip share same buffer @ 16M solution.
#define UNZIP_SHARE_WITH_DBG_BUFFER

#elif (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_32)
// Chuan2.11, Force to support FBR Mode to save DRAM space.
// #define  SUPPORT_FBR_MODE

#define     DRAM_CONFIGURATION_TYPE_DVD       DRAM_SIZE_32
#define     DRAM_CONFIGURATION_TYPE_DVB       DRAM_SIZE_UNUSED

//Only 32M/64M support graphics zoom.
#define SUPPORT_GRAPH_ZOOM
#define FONT_TABLE_METHOD    SPLIT_2_1_BIT
#define SUPPORT_WMA

#define SUPPORT_UDF_FILE_SYSTEM

// External subtitle use SPU buffer instead of OSD buffer
#define EX_SP_USE_SPU_BUFFER
#define NO_SCREEN_SAVER     // Don't load screen-saver

// wyc2.21-909s, disable OSDGAME.
//#define SUPPORT_OSDGAME
#ifdef SUPPORT_OSDGAME
//#define SHOW_MENU_ICON
#define SUPPORT_GAME_TETRIS
//#define SUPPORT_GAME_GO5
//#define SUPPORT_GAME_SOKOBAN
//#define SUPPORT_GAME_RUSHHOUR
//#define SUPPORT_GAME_MINER
#endif //SUPPORT_OSDGAME

#elif (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_64)

#define FLASH_INT_OFF

#ifdef SUPPORT_STB
#define     DRAM_CONFIGURATION_TYPE_DVD       DRAM_SIZE_32
#define     DRAM_CONFIGURATION_TYPE_DVB       DRAM_SIZE_32

#define FONT_TABLE_METHOD    SPLIT_2_1_BIT
#undef SUPPORT_OSDGAME

// External subtitle use SPU buffer instead of OSD buffer
#define EX_SP_USE_SPU_BUFFER

#define SUPPORT_219_OSDGAME
#define NO_SCREEN_SAVER     // Don't load screen-saver
#ifdef SUPPORT_219_OSDGAME
//#define SHOW_MENU_ICON
#define SUPPORT_GAME_TETRIS
//#define SUPPORT_GAME_GOMOKU
//#define SUPPORT_GAME_SOKOBAN
#define SUPPORT_GAME_RUSHHOUR
#define SUPPORT_GAME_MINER
#define SUPPORT_CALENDAR
#endif //SUPPORT_219_OSDGAME

#else
#define     DRAM_CONFIGURATION_TYPE_DVD       DRAM_SIZE_64
#define     DRAM_CONFIGURATION_TYPE_DVB       DRAM_SIZE_UNUSED

#define FONT_TABLE_METHOD    TRADITION
#define ENLARGE_OSD_FOR_PAL_MODE

// wyc2.77-909p, when DMP 64M mode, also use small buffer mode to avoid MP3 hang problem.
#ifdef CT909P_IC_SYSTEM
#ifdef SUPPORT_DMP
#define SMALL_SOURCE_BUFFER
#endif //
#endif //

//#define SUPPORT_OSDGAME
#ifdef SUPPORT_OSDGAME
//#define SHOW_MENU_ICON
#define SUPPORT_GAME_TETRIS
//#define SUPPORT_GAME_GO5
//#define SUPPORT_GAME_SOKOBAN
//#define SUPPORT_GAME_RUSHHOUR
//#define SUPPORT_GAME_MINER
#endif //SUPPORT_OSDGAME

#endif
#ifdef CT909P_IC_SYSTEM
#ifdef SUPPORT_NAVIGATOR
#undef SUPPORT_NAVIGATOR
#endif
#ifdef SUPPORT_UDF_FILE_SYSTEM
#undef SUPPORT_UDF_FILE_SYSTEM
#endif//
#ifdef SUPPORT_GRAPH_ZOOM
#undef SUPPORT_GRAPH_ZOOM
#endif
#else
#define SUPPORT_NAVIGATOR
#define SUPPORT_UDF_FILE_SYSTEM
#define SUPPORT_GRAPH_ZOOM
#endif ///

#define SUPPORT_WMA

//Only 32M/64M support graphics zoom.
// #define SUPPORT_GRAPH_ZOOM


#else
#define     DRAM_CONFIGURATION_TYPE_DVD       DRAM_SIZE_UNUSED
#define     DRAM_CONFIGURATION_TYPE_DVB       DRAM_SIZE_UNUSED
#endif

#define     CT909_DRAM_START            (0x40000000L)
#define     DATA_WIDTH                  (4)     // 4 bytes width

#ifdef  APPLICATION_WINDOWS
#include    "buffer.h"
#else
#include "dvd_dram_16m.h"
#include "dvd_dram_32m.h"
#include "dvd_dram_64m.h"
#include "dvb_dram_32m.h"
#endif
////////////////////////////////////////
// Please define the U-P working Speed
#define     CPU_27M          27000000   // 27 MHz
#define     CPU_54M          54000000   // 54 MHz
#define     CPU_100M        100000000   // 100 MHz
#define     CPU_120M        120000000   // 120 MHz
#define     CPU_133M        133000000   // 133 MHz
#define     CPU_142M        142000000   // 142 MHz
#define     CPU_146M        146000000   // 146 MHz
#define     CPU_162M        162000000   // 162 MHz

#define     CPU_SPEED       CPU_133M

// LLY2.38, must choose desired CPU working speed when power down mode
#define     POWERDOWN_CPU_SPEED     CPU_27M

// J500CSC_2.05, for supporting runtime decompressed audio code
#define     DS_AUDIO_TABLE_ST           (0x00000008L)

#ifdef __cplusplus
    }
#endif
#endif  // __INTERNAL_H__
