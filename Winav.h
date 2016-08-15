
/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved

        Product : Cheertek W99AV series Firmware
        Chip    : CT908

        Date    : 2001.02.26.
        Author  : Cheertek (D300 Micky)
        Purpose : types, macro and definitions
                  Version 2.00
        Sources : winav.h

****************************************************************************/
//////////////////////////////////////////////////////////////
// ***** Big Modification History Area *****
// Micky1.00
//  - Remove W9928AF VERSION_xxx
//  - Remove W9928AF related system define DVD28A_xxx/ DVD_28C_xxx.
//  - Remove the CHIP_9928 related codes

#ifndef __WINAV_H__
#define __WINAV_H__
// Micky2.17c
#ifdef __cplusplus
        extern  "C"     {
#endif
// Micky2.17
#include    "platform.h"


//Auto testing definition, open this definition to enable Auto testing 
//#define SUPPORT_OSDTIME_OUTPUT

//AutoTuning definition, open this definition to use AutoTuning, Ben, May/05/2006.
//#define AutoTuning_MPEG
// LLY2.36a, don't power donw MPLL while do servo do auto tuning job.
#ifdef  AutoTuning_MPEG
#define NOT_POWERDOWN_MPLL
#endif  // #ifdef  AutoTuning_MPEG

// wyc2.76-909P,
// IC Version
// peteryu275, add for IC check version, 2007/3/22 11:31AM
#define IC_VERSION_RESERVED         (0x0001) //reserved for old version
#define IC_VERSION_909R             (0x0010) //909S
#define IC_VERSION_219              (0x0011)
#define IC_VERSION_909P_A_W         (0x0020)
#define IC_VERSION_909P_B_X         (0x0021)
#define IC_VERSION_909G             (0x0030)
#define IC_VERSION_950A             (0x0040)
#define IC_VERSION_952A             (0x0041)
#define IC_VERSION_955A             (0x0042)
#define IC_VERSION_956A             (0x0043)
#define IC_VERSION_956A_DEV         (0x0044)
#define IC_VERSION_951              (0x0045)

#define IC_VERSION_908G             (0x00f0)

//peteryu275, add for IC check version, 2007/3/22 11:31AM end

////////////////////////////////////////////////
// ****** Programmer/Developer's Zone ******
////////////////////////////////////////////////
// Flash code version
//alex1.21,Support 4M W29C040
// F/W write the version to DRAM, Flash tool will check the version from DRAM.
//alex080. support cd rom upgrade
#define     TOOLS_SW_VERSION     6L  // Alex080, must use mergerom 003

#define     ECOS_PACKAGE_VERSION    0x200

// wyc2.03-909s, move 909S relative define to here because of decoder system define in customer.h need to reference it.
#define FLASH_SW_VERSION    1300L  // Alex080, must use flash 075

// Micky1.10-2, add define for select MCCRBASE
#define PLL_399MHZ  1       // PLL run 399 MMz
#define PLL_360MHZ  2       // PLL run 360 MHz
#define MPEG_PLL    PLL_360MHZ

#define SUPPORT_HW_DETECT_CHIPS_VERSION //Kevin1.08, detect CT908-A/D/M/Z


//////////////////////////////////////
// Can define it for release
// define it, the following definitions will be Enable
// [ENABLE_PANEL_KEYSCAN];
// define it, F/W will suppport panel volume adjustment button
// [ENABLE_PANEL_VOLUME_ADJUSTMENT_BUTTON];
// define it, the following definitions will be Disable
// [_DEBUG_INFO];
//#define RELEASE_SETTING

////////////////////////////////////////////////
// ****** System Configuration Zone ******
////////////////////////////////////////////////

// Must select only one interface and others should be disable
// define it, the stream input path is from HOST_IF
#define BITSTREAM_ATAPIIF       1
#define BITSTREAM_DVDIF         2
#define BITSTREAM_CDIF          3
#define BITSTREAM_HOSTIF        4

// add chip version number protect
// define the chip version number that F/W can support
// for BAE, the value should be VERSION_BAE
///#define SUPPORT_CHIP_VERSION    VERSION_BBA

//////////////////////////////////////////////////////////////////////////
// LLY.171-1, define all the power on mode
// bit[0]: 1, pseudo power down; 2, total shutdown;
// It will be refered by HAL_PowerDown() & INITIAL_System()
// [IR_POWERDOWN_MODE]
// Total shutdown for MPEG chip/ PANEL/ TV Encoder/ Audio DAC
// and reserve uP and SRAM active
#define TOTAL_SHUTDOWN          0x02
// Pseudo power down mode: only turn-off some MPEG chip block, and only reserve
// three block -- VBI VCLK/ DBI SDCLK/ RISC/DBI MCLK-- PWRCR[3, 5, 12]
#define PSEUDO_SHUTDOWN         0x01    // pseudo power down mode

// The combination of hareware shutdown case : all peripheral all enter power down
// it's only for F/W reference
#define HARDWARE_SHUTDOWN       0x04    // hardware power on | total shutdown

// bit[7]: 1, enter power down mode; 0, wake-up from power down mode
#define ENTER_POWERDOWN         0x20    // enter power down mode
#define INITIAL_SYSTEM          0x10    // leave power down mode

#define USB1_1_CT909R       1
#define USB2_0_CHIPIDEA     2
#define USB2_0_JUNGO        3
// wyc2.75-909P, add one case to define no USB mode.
#define USB_NONE            4

//CoCo2.37, move it before customer.h
//#define SUPPORT_ATAPI_SOURCE
#define SUPPORT_SERVO_SOURCE
// wyc1.01-909, support USB in DVD_101.
// wyc1.02-909, don't enable USB when ATAPI is enabled. Because not sure if there are bugs in this situration.
#ifndef SUPPORT_ATAPI_SOURCE
#if defined(CT909R_IC_SYSTEM) || defined(CT909P_IC_SYSTEM)
#define SUPPORT_USB
//wyc2.78-909P, the define to support usb host+device mode.
// #define SUPPORT_USB_HOST_DEVICE
#elif defined(CT909G_IC_SYSTEM)
#define USB_SYSTEM          USB_NONE
#endif //
#ifdef SUPPORT_USB
#define SUPPORT_USB_SOURCE
#define SUPPORT_FAT_FILE_SYSTEM
// wyc2.77-909p, the define to enable the usb performance code. Default is disabled.
// #define TEST_DEVICE_PERFORMANCE

// wyc1.05-909, add one define to only parse in video / audio only files.
//#define USB_DONT_SUPPORT_MOTION_FILES
// wyc2.71-portusb,

#if defined (CT909P_IC_SYSTEM)
#define USB_SYSTEM          USB2_0_JUNGO
#define CTK_CARDREADER
#elif defined (CT909R_IC_SYSTEM)
#define USB_SYSTEM          USB1_1_CT909R
#endif //

#endif //
#endif //

// DVD_100Micky, put customer setting into customer.h
#include    "customer.h"
// wyc2.16-909S, include internal.h here.
#include    "internal.h"


#ifdef SERIAL_DEBUG
#include "debug.h"
// Micky2.17c
#ifdef	APPLICATION_WINDOWS
#define PRINTF		DBG_Printf_Simple
#define printf		DBG_Printf_Simple
void    DBG_Printf_Simple(char *fmt, ...);
#else
#define PRINTF(x, y...)     DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, x, ## y)
#define printf(x, y...)     DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, x, ## y)
#endif
// wyc2.16-909S
void    PRINTF_GENE(char *fmt, ...);
void    printf_GENE(char *fmt, ...);

//#define PRINTF  PRINTF_GENE
//#define printf  printf_GENE
#endif //

#ifndef CPU_SPEED
// LLY2.13, set CPU working speed as 133MHz as default
#define     CPU_SPEED       CPU_133M
#endif  // #ifndef  CPU_SPEED

////////////////////////////////////////
// Please choose one IC.
// Must Assign the ID(TV_ENCODER_ID) as I2C ID if the IC use I2C to control.
#define     TV_GENERAL      0x00        // no-need for control
#define     TV_W9954        0x01
#define     TV_SAA7120      0x03        //
#define     TV_ADV7170      0x04        // TV encoder with YUV
#define     TV_CS4954       0x05        // ** 0.40;
#define     TV_AV3168       0x06        // ** 0.45;

#define     I2C_ID_EEPROM   0xA0

//audio DAC configuration
#define     DAC_GENERAL                 0 //If No need to program, define as DAC_GENERAL
#define     DAC_CS4334                  DAC_GENERAL
#define     DAC_WM8746                  1
#define     DAC_PCM1723                 2
#define     DAC_PCM1723_WM8746          3
#define     DAC_CE2752                  4
#define     DAC_W571C161                5
#define     DAC_INTERNAL                6   // LLY1.50

//AUDIO_ALIGN_FORMAT
#define     RIGHT_ALIGN     0
#define     I2S_ALIGN       1
#define     LEFT_ALIGN      2

//FS
#define     ACLK_384FS      0
#define     ACLK_256FS      1
#define     ACLK_192FS      2
#define     ACLK_128FS      3

//audio volume adjustment source in AV system
#define     ANALOG_WM8746       0x1
#define     DIGITAL_DDX8000     0x2

// LLY2.11, define the storage type ID
#define     SERIAL_FLASH        1
#define     EEPROM              2
#define     NO_STORAGE          0xff

// ******************************
// SUPPORT zone for the explanation of supporting variety
// ******************************

// DVD_161Micky, support Loading-> Setup-> Exit, will continue loading action
/// #define SUPPORT_LOADING_SETUP

// DVD_104Micky, add a define for mic. control
//define  it, means system support mic. on/off control pin.
/// #define SUPPORT_MIC_CONTROL
/// #define SUPPORT_MIC_CONTROL_INV // inverse control

// ** TCH1.54; Support __sbMicDetect Pin for Mic I/P.
/// #define SUPPORT_MIC_DETECT          // Active= LOW. Mic has I/P
/// #define SUPPORT_MIC_DETECT_INV      // Active= HIGH. Mic has I/P.

// define it, means System support Scart_IF only.
// Depends on Board Configuration.
// SUPPORT_SCART_IF_GPIO is controlled by GPIO,
// SUPPORT_SCART_IF is controlled by CPU_IO.
//#define     SUPPORT_SCART_IF
//#define     SUPPORT_SCART_IF_GPIO     // ** TCH1.00a;

// define it, means System support max. PCM_LENGTH.
// Depends on AudioDAC Configuration.
//#define     SUPPORT_PCM_WORD_LENGTH           xx (xx mean "16", "18", "20", 24")

// define it, means System support Audio DAC Reset Pin. Active is LOW, means reset state.
// Depends on Board Configuration.
//#define     SUPPORT_AUDIODAC_RESET

//////////////////////////////////////////////////////
// Configuration for the Decoder System
// If NO need to program ADAC, #define AUDIO_DAC DAC_GENERAL
//Note: #define SUPPORT_PCM_WORD_LENGTH 24  for Dolby Certification

#if (DECODER_SYSTEM == DVD909_EVAL)
#define     SUPPORT_PCM_WORD_LENGTH     16
#define     AUDIO_DAC                   DAC_WM8746  // default format is right align + 24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN
#endif

#if (DECODER_SYSTEM == DVD909_HFY)
#define 	SUPPORT_PCM_WORD_LENGTH 	24
#define     AUDIO_DAC                   DAC_W571C161    // default format is right align + 16 bit
#define 	AUDIO_ALIGN_FORMAT	        RIGHT_ALIGN
#endif

// LLY1.50, add for CT909S evaluation board
#if (DECODER_SYSTEM == DVD909S_EVAL)
// [Audio DAC]
// (1) : Internal ADAC + W571C161
// Notice: Internal ADAC default format : I2S + 24 bit
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_W571C161
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN

// (2) : Internal ADAC only
// LLY2.17, use internal Audio DAC only (default format is I2S + 24 bit)
// So, support two channel only: disable PCMSD0/1/2, and let it work as GPIO
// Notice: Don't choose any AUDIO_DAC for external ADAC
//#define     USE_INTERNAL_ADAC_ONLY
//#define     ENABLE_INTERNAL_ADAC
//#define     AUDIO_DAC                   DAC_INTERNAL
//#define     SUPPORT_PCM_WORD_LENGTH     24
//#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN


// [Storage Type]
#define     STORAGE_TYPE        EEPROM

// [Total pin define]
#define     PIN_216

#define     IC_VERSION_ID       IC_VERSION_909R
#define     CS_UPGAP_NAME       "UPG909S.AP"
#endif  // #if (DECODER_SYSTEM == DVD909S_EVAL)


// LLY2.11, add for CT909R evaluation board
#if (DECODER_SYSTEM == DVD909R_EVAL)
// [Audio DAC] : Internal ADAC only
// LLY2.17, use internal Audio DAC only (default format is I2S + 24 bit)
// So, support two channel only: disable PCMSD0/1/2, and let it work as GPIO
// Notice: Don't choose any AUDIO_DAC for external ADAC
#define     USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_INTERNAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN

// [Storage Type]
// Use serial flash to save setup information.
#define     STORAGE_TYPE                SERIAL_FLASH

#define     IC_VERSION_ID       IC_VERSION_909R
#define     CS_UPGAP_NAME       "UPG909R.AP"
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)


// LLY2.15, add for CT909R HFY board
#if (DECODER_SYSTEM == DVD909R_HFY)
// [Audio DAC] : Internal ADAC + WM8746
// Notice: Internal ADAC default format : I2S + 24 bit
//         WM8746 default format: Right + 24 bit
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_WM8746
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN

// [Storage Type]
// Use serial flash to save setup information.
#define     STORAGE_TYPE                SERIAL_FLASH

#define     IC_VERSION_ID       IC_VERSION_909R
#define     CS_UPGAP_NAME       "UPG909R.AP"
#endif  // #if (DECODER_SYSTEM == DVD909R_HFY)


// LLY2.16, add for STB219 HCN board
#if (DECODER_SYSTEM == STB219_HCN)
// [Audio DAC] : Internal ADAC only
// LLY2.17, use internal Audio DAC only (default format is I2S + 24 bit)
// So, support two channel only: disable PCMSD0/1/2, and let it work as GPIO
// Notice: Don't choose any AUDIO_DAC for external ADAC
#define     USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_INTERNAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN

// [Storage Type]
// Use serial flash to save setup information.
#define     STORAGE_TYPE    SERIAL_FLASH

#define     IC_VERSION_ID               IC_VERSION_219
#define     CS_UPGAP_NAME       "UPG219.AP"
#endif  // #if (DECODER_SYSTEM == DVD909R_HCN)

// LLY2.16, add for STB219 YHI board
#if (DECODER_SYSTEM == STB219_YHI)
// [Audio DAC] : Internal ADAC only
// LLY2.17, use internal Audio DAC only (default format is I2S + 24 bit)
// So, support two channel only: disable PCMSD0/1/2, and let it work as GPIO
// Notice: Don't choose any AUDIO_DAC for external ADAC
#define     USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_INTERNAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN

// [Storage Type]
// Use serial flash to save setup information.
#define     STORAGE_TYPE                SERIAL_FLASH

#define     IC_VERSION_ID               IC_VERSION_219
#define     CS_UPGAP_NAME       "UPG219.AP"
#endif  // #if (DECODER_SYSTEM == DVD909R_YHI)

#if (DECODER_SYSTEM == DVD909P_A_EVAL)
// [Audio DAC] : Internal ADAC only
// Notice: Don't choose any AUDIO_DAC for external ADAC
#define     USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_INTERNAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN
#define     REGTABLE                    "VD\Regtable_CT909P_A.H" //Set TFT include register table . Jeff VD 1.23 Add
#define     SUPPORT_TFT
#ifndef     NO_SCREEN_SAVER
#define     NO_SCREEN_SAVER
#endif //NO_SCREEN_SAVER
#define     LED_BackLight
#define     SUPPORT_CCIR656

// [Storage Type]
// Use serial flash to save setup information.
#define     STORAGE_TYPE                SERIAL_FLASH

#define     IC_VERSION_ID               IC_VERSION_909P_A_W
#define     CS_UPGAP_NAME               "UPG909PA.AP"
// LLY2.60, don't support PSCAN output for CT909P/ CT95x serial IC
#define     NO_PSCAN_OUTPUT
#endif  // #if (DECODER_SYSTEM == DVD909P_A_EVAL)

#if (DECODER_SYSTEM == DVD909P_B_EVAL)
// [Audio DAC] : Internal ADAC only
// Notice: Don't choose any AUDIO_DAC for external ADAC
#define     USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_INTERNAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN
#define     REGTABLE                    "VD\Regtable_CT909P_A.H" //Set TFT include register table . Jeff VD 1.23 Add
// [Storage Type]
// Use serial flash to save setup information.
#define     STORAGE_TYPE                SERIAL_FLASH

#define     IC_VERSION_ID               IC_VERSION_909P_B_X
#define     CS_UPGAP_NAME               "UPG909PB.AP"

// LLY2.60, don't support PSCAN output for CT909P/ CT95x serial IC
#define     NO_PSCAN_OUTPUT
#endif  // #if (DECODER_SYSTEM == DVD909P_B_EVAL)

#if (DECODER_SYSTEM == DVD956A_DEV)
// [Audio DAC] : Internal ADAC + CS4334 2 channel only DAC
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_CS4334
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN
#define     SUPPORT_2_CHANNEL_ONLY
// wyc2.77-909p, move this define to here.
// #define SUPPORT_TFT_DIGITAL_MODE
#ifdef SUPPORT_TFT_DIGITAL_MODE
#define     REGTABLE                    "VD\Regtable_CT956.H"   //Set TFT include register table . Jeff VD 1.23 Add
#else
#define     REGTABLE                    "VD\Regtable_CT955_DEV.H"   //Set TFT include register table . Jeff VD 1.23 Add
#endif
// [Storage Type]
#define     STORAGE_TYPE                SERIAL_FLASH
// #define     STORAGE_TYPE                NO_STORAGE

#define     IC_VERSION_ID               IC_VERSION_956A_DEV
#define     CS_UPGAP_NAME               "UPG956AD.AP"
// [Total pin define]
#define     PIN_216

// LLY2.60, don't support PSCAN output for CT909P/ CT95x serial IC
#define     NO_PSCAN_OUTPUT

// LLY2.75, always enable define "USE_GPA21_CTL_74244" for CT909P_P3 216 pin system board.
#define     USE_GPA21_CTL_74244
#endif  // #if (DECODER_SYSTEM == DVD956A_DEV)

#if (DECODER_SYSTEM == DMP952A_EVAL)
// [Audio DAC] : Internal ADAC + CS4334 2 channel only DAC
#define     USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_INTERNAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT  I2S_ALIGN

#define     REGTABLE                    "VD\Regtable_CT952.H"   //Set TFT include register table . Jeff VD 1.23 Add
// [Storage Type]
#define     STORAGE_TYPE                SERIAL_FLASH
// #define     STORAGE_TYPE                NO_STORAGE

#define     IC_VERSION_ID               IC_VERSION_952A
#define     CS_UPGAP_NAME               "UPG952A.AP"
// LLY2.55, don't support SPDIF output.
#define     NO_SPDIF_OUTPUT

// LLY2.60, don't support PSCAN output for CT909P/ CT95x serial IC
#define     NO_PSCAN_OUTPUT
// Elmer2.75, support ADC_KEY for CT952A/CT955A/CT956A platform
#define     ADC_KEY
//#define     KEY_8
//#define     TOUCH_KEY
#endif  // #if (DECODER_SYSTEM == DMP952A_EVAL)

//SYuan 2.88
#if (DECODER_SYSTEM == DMP952B_EVAL)
// [Audio DAC] : Internal ADAC + CS4334 2 channel only DAC
#define     USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_INTERNAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT  I2S_ALIGN

#define     REGTABLE                    "VD\Regtable_CT952.H"   //Set TFT include register table . Jeff VD 1.23 Add
// [Storage Type]
#define     STORAGE_TYPE                SERIAL_FLASH
// #define     STORAGE_TYPE                NO_STORAGE

#define     IC_VERSION_ID               IC_VERSION_952A
#define     CS_UPGAP_NAME               "UPG952B.AP"
// LLY2.55, don't support SPDIF output.
#define     NO_SPDIF_OUTPUT

// LLY2.60, don't support PSCAN output for CT909P/ CT95x serial IC
#define     NO_PSCAN_OUTPUT
// Elmer2.75, support ADC_KEY for CT952A/CT955A/CT956A platform
#define     ADC_KEY
#define     KEY_7
//#define     TOUCH_KEY
#endif  // #if (DECODER_SYSTEM == DMP952B_EVAL)

// LLY2.60, add for CT956A system
#if (DECODER_SYSTEM == DMP956A_EVAL)
// [Audio DAC] : Internal ADAC only
#define     USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_INTERNAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN

// wyc2.77-909p, move this define to here.
#define SUPPORT_TFT_DIGITAL_MODE
#define     REGTABLE                    "VD\Regtable_CT956.H"   //Set TFT include register table . Jeff VD 1.23 Add
// [Storage Type]
#define     STORAGE_TYPE                SERIAL_FLASH
// #define     STORAGE_TYPE                NO_STORAGE

#define     IC_VERSION_ID               IC_VERSION_956A
#define     CS_UPGAP_NAME               "UPG956A.AP"
// LLY2.55, don't support SPDIF output.
#define     NO_SPDIF_OUTPUT

// LLY2.60, don't support PSCAN output for CT909P/ CT95x serial IC
#define     NO_PSCAN_OUTPUT
// Elmer2.75, support ADC_KEY for CT952A/CT955A/CT956A platform
#define     ADC_KEY
//#define     KEY_8
//#define     TOUCH_KEY
#endif  // #if (DECODER_SYSTEM == DMP956A_EVAL)

// LLY2.60, add for CT955A system
#if (DECODER_SYSTEM == DMP955A_EVAL)
// [Audio DAC] : Internal ADAC only
#define     USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_INTERNAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN

#define     REGTABLE                    "VD\Regtable_CT955.H"   //Set TFT include register table . Jeff VD 1.23 Add
// [Storage Type]
#define     STORAGE_TYPE                SERIAL_FLASH
// #define     STORAGE_TYPE                NO_STORAGE

#define     IC_VERSION_ID               IC_VERSION_955A
#define     CS_UPGAP_NAME               "UPG955A.AP"
// LLY2.55, don't support SPDIF output.
#define     NO_SPDIF_OUTPUT

// LLY2.60, don't support PSCAN output for CT909P/ CT95x serial IC
#define     NO_PSCAN_OUTPUT
// Elmer2.75, support ADC_KEY for CT952A/CT955A/CT956A platform
#define     ADC_KEY
//#define     KEY_8
//#define     TOUCH_KEY
#endif  // #if (DECODER_SYSTEM == DMP955A_EVAL)


#if (DECODER_SYSTEM == DVD909G_EVAL)
// LLY2.61, modify CT909G system configuration for audio DAC
#if 1
// [Case 1] : Internal audio DAC only (can enable SCART feature)
#define     USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     AUDIO_DAC                   DAC_INTERNAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN
#define     SUPPORT_2_CHANNEL_ONLY
#endif

#if 0
// [Case 2] : External 5.1 channel Audio (Dis-allow to enable SCART feature)
// Notice: it's use WM8766 5.1ch ADAC (I2S, 24bit),
//         and un-necessary to program (ie. run H/W default mode)
//         So, suggest to assign the DAC as DAC_GENERAL since no need program it.
#define     AUDIO_DAC                   DAC_GENERAL
#define     SUPPORT_PCM_WORD_LENGTH     24
#define     AUDIO_ALIGN_FORMAT          I2S_ALIGN
#endif

// [Storage Type]
#define     STORAGE_TYPE                SERIAL_FLASH
// #define     STORAGE_TYPE                NO_STORAGE

#define     IC_VERSION_ID               IC_VERSION_909G
#define     CS_UPGAP_NAME               "UPG909G.AP"
#endif  // #if (DECODER_SYSTEM == DVD909G_EVAL)

// LLY2.11, base on storage type to decide if define NO_SERIAL_FLASH
#if (STORAGE_TYPE == SERIAL_FLASH)
#undef  NO_SERIAL_FLASH
#endif  // #if (STORAGE_TYPE == SERIAL_FLASH)

#if (STORAGE_TYPE == EEPROM)
#define NO_SERIAL_FLASH
#endif  // #if (STORAGE_TYPE == SERIAL_FLASH)


#if (AUDIO_ALIGN_FORMAT == RIGHT_ALIGN)
#define ALIGN_VALUE     0
#elif (AUDIO_ALIGN_FORMAT == I2S_ALIGN)
#define ALIGN_VALUE     1
#elif (AUDIO_ALIGN_FORMAT == LEFT_ALIGN)
#define ALIGN_VALUE     2
#endif

#if(SUPPORT_PCM_WORD_LENGTH == 32)
#define BIT_RESOL_VALUE     4
#elif(SUPPORT_PCM_WORD_LENGTH == 24)
#define BIT_RESOL_VALUE     3
#elif(SUPPORT_PCM_WORD_LENGTH == 20)
#define BIT_RESOL_VALUE     2
#elif(SUPPORT_PCM_WORD_LENGTH == 18)
#define BIT_RESOL_VALUE     1
#else // 16
#define BIT_RESOL_VALUE     0
#endif

// ******************************
// Audio Output Configuration
// ******************************
// DVD_275b+, SUPPORT_VIRTUAL_DTS
// define it, when DTS stream is selected audio audio output is not SPDIF/RAW mode
// will auto choose a non-DTS stream.
///#define SUPPORT_VIRTUAL_DTS

// define it, only support 2 channel output
// setup only need provide SPDIF/2 channel setting
// else will support 2 channel/5.1 channel
///#define SUPPORT_2_CHANNEL_ONLY

//ACLK_384FS or ACLK_256FS
#define ACLK_FSMF  ACLK_384FS


// LLY2.17, if define it, it will enable internal Audio DAC
// Otherwise, it will be turn-off
//#define     ENABLE_INTERNAL_ADAC

// LLY2.17, always enable internal audio DAC if user choose AUDIO_DAC as DAC_INTERNAL
#if  AUDIO_DAC == DAC_INTERNAL
#define USE_INTERNAL_ADAC_ONLY
#endif  // #if  AUDIO_DAC == DAC_INTERNAL

// LLY2.17, if define it, it will use internal audio DAC only (ie. only support 2 channel)
// Otherwise, it can use external audio DAC
// Notice: user can choose it base on his platform configure
//#define     USE_INTERNAL_ADAC_ONLY
#ifdef  USE_INTERNAL_ADAC_ONLY
#define     ENABLE_INTERNAL_ADAC
#define     SUPPORT_2_CHANNEL_ONLY
#endif  // #ifdef USE_INTERNAL_ADAC_ONLY




// define it, means Using I2C to control IC
// else reduce the source code
///#define     SUPPORT_I2C

// define it, will support real Letter Box Subpicture
// else, it choose WIDE mode
// Micky0.86, only navigate.c will reference it.
// Ct908AB can't enable it.
// Micky0.95, 908AC can support it.
// Micky1.00, enable it always for CT908AC.
#define SUPPORT_LETTERBOX_SUBPICTURE

////////////////////////////////////////////////
// ****** User Operation Zone ******
////////////////////////////////////////////////
// ******************************
// OSD part
// ******************************

// LLY2.78b-3, if define it, it will display "program play" info
// while start the program mode playing
// Otherwise, it only display "PLAY"
//#define SUPPORT_PROGRAM_PLAY_INFO

// Micky1.10, define it, KEY_REPEAT will toggle between repeat track/ disc when playing program.
/// #define SUPPORT_ONLY_2_REPEAT_MODE_IN_PROGRAM

// wyc1.10, add follow define in WINAV.H
// If define it, then CDROM titles will support number confirm keys
// Otherwise, after input number key in CDROM titles then it will directly go that file.
//#define SUPPORT_CDROM_NUMBER_CONFIRM_KEY

// If define it, then cursor will loop in current directory when moving select highlight up and down.
// Otherwise, move select highlight to up of current directory will go previous directory's last file.
//#define SUPPORT_FM_CURSOR_RING_SAME_DIRECTORY

// Brian1.07
// If define it, the value text of Setup OSD item will its own language
// otherwise all the value text use the same language as the current OSD language
// #define OSD_ITEM_TEXT_SAME_AS_LANGUAGE

// Brian1.07
// If define it, Setup help message will change according to the current OSD language.
// otherwise, all the help message is English
// #define SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG

//Brian1.07a
// define this will show PSCAN prompt dialog in Setup
//#define SUPPORT_PSCAN_PROMPT_SETUP

// define this will show PSCAN prompt dialog when KEY_PSCAN or KEY_VIDEO_OUTPUT_CTL
//#define SUPPORT_PSCAN_PROMPT_OSD

// default action after PSCAN prompt time out.
// KEY_STOP: cancel; KEY_PLAY: continue
#define PSCAN_PROMPT_DEFAULT_KEY  KEY_STOP  // default is cancel the action

#define SUPPORT_FONT_TABLE_CHECKSUM   // Brian1.25, support font table checksum ability
//#define SUPPORT_ADJUST_CHROM_PHASE    // Brian1.25, support chroma adjust ability
#define SUPPORT_STRING_GENERATOR  // Brian1.25, define this will use string files generated by String Generate Tool

//LJY1.11, support anti-shock
// define it, will support ant-shock
/// #define SUPPORT_ANTISHOCK

// Micky1.20, 4M flash
///#define SUPPORT_4M_FLASH

// Micky1.10-2, add configuration for support 32M SDRAM
// define it, will support 32M SRDAM configuration. need uCode cooperation
/// #define USE_32M_SDRAM

// if define this, all the Setup dialog will recover the area by redrawing,
// otherwise it use Bitmap Copy to recover the area.
#define SETUP_DIALOG_RECOVER_BY_REDRAW

// if define this, will use only one OSD region for display
#define USE_ONE_OSD_REGION


#ifdef USE_32M_SDRAM

#ifdef SUPPORT_ANTISHOCK // Brian1.11, make sure 32M case has no Anti-shock
#undef SUPPORT_ANTISHOCK
#endif
#endif

// If define it, logo data is put @ flash, and F/W will read it from flash directly.
// Otherwise, logo data is included by our F/W program. -- LLY1.06-1
// wyc2.17-909S, enable it in this version.
#define LOGO_FROM_FLASH




// ****************************************************************
// Disc Resume Feature Area
// ****************************************************************
// If define it, it will support disc resume feature.
//#define SUPPORT_MULTI_DISC_RESUME

// Always define the supported disc number for Disc Resume feature.
#define RESUME_DISC_NUM             5

// If define it, support disc resume feature while I/R Power on.
// ex. IR power off: save the resume info.
//     Detect disc done, ex. IR power on or close tray: playback the resume info if same disc
//#define SUPPORT_RESUME_AFTER_POWERON

// If define it, support disc resume feature while open/close.
// ex. Tray open: save the resume info.
//     Detect disc done, ex. IR power on or close tray: playback the resume info if same disc
//#define SUPPORT_RESUME_AFTER_OPENCLOSE

// If define it, the multi-disc resume information will be kept in the EEPROM
//#define SUPPORT_RESUME_FROM_EEPROM

// Ming2.42-1, Clear Resume info. in fully STOP mode.
//#define CLEAR_RESUME_WHEN_STOP

// Micky1.06, merge the define error condition.
#if defined(SUPPORT_RESUME_AFTER_OPENCLOSE) || defined(SUPPORT_RESUME_AFTER_POWERON)
#undef SUPPORT_MULTI_DISC_RESUME
#define SUPPORT_MULTI_DISC_RESUME
#endif  // #if defined(SUPPORT_RESUME_AFTER_OPENCLOSE) || defined(SUPPORT_RESUME_AFTER_POWERON)

#if !defined(SUPPORT_MULTI_DISC_RESUME)
#if defined(SUPPORT_RESUME_FROM_EEPROM) || defined(CLEAR_RESUME_WHEN_STOP)
#error "Must define SUPPORT_RESUME_AFTER_OPEN_CLOSE or SUPPORT_RESUME_AFTER_OPEN_CLOSE "
#endif  //#if defined(SUPPORT_RESUME_FROM_EEPROM) || defined(CLEAR_RESUME_FROM_STOP)
#endif  //#if !defined(SUPPORT_MULTI_DISC_RESUME)

// Micky1.24,
// define it, will show invalid message when gototime is not allowed
///#define GOTIME_DISALLOW_SHOW_INVALID
// define it, will clear REPEAT status when reap stop
///#define CLEAR_REPEAT_WHEN_REAL_STOP

// ******************************
// Power On Status
// ******************************
// define it, means system will enter standby mode when power is ON first time
/// #define    POWERON_STANDBY

// define it, means system will auto-play after Open/Close or KEY_PBC or Power-ON
///#define    AUTO_PLAY

// wyc1.05-2, a define to support FM auto play
// define it, means FM will auto play
// else, will stop at FM menu.
//#define MMANAGER_AUTO_PLAY

// define it, means system will enetr NO-PBC mode even title is "VIDEO CD 2.0"
/// #define    DEFAULT_NOPBC

// define it, means Border is blue when switch track or ReadTOC
// else the border is black
/// #define    BORDER_BLUE

// define it, when stay in stop mode or no disc and
// no user operation over a period of time
// will auto enter power down mode.
///#define AUTO_POWERDOWN

// Micky1.25, add auto powerdown for screen saver mode
// #define POWERDOWN_AFTER_SCREENSAVER

// the wait time to enter power down mode
///#define WAITTIME_POWERDOWN      (COUNT_10_SEC*48L)

// If define it, it will switch to AP mode base on EEPROM saved value directly -- LLY2.00
// #define  POWERON_SWITCH_AP


// **************************************
// Auto-Upgrade Relative Area
// **************************************
// If define it, F/W will support auto-upgrade feature
// Otherwise, the feature will be disable
// Micky2.80p, only SYSTEM_8051 will support auto upgrade
#define SUPPORT_AUTO_UPGRADE

// Chuan 172 2002/02/22: if define SUPPORT_AUTO_UPGRADE, support auto upgrade
#ifdef SUPPORT_AUTO_UPGRADE
// Chuan 172 2002/02/22:
// if define AUTO_UPGRADE_CONFIRM, will confirm auto upgrade procedure by user
#define AUTO_UPGRADE_CONFIRM
// Chuan0.87-2th, define it to do checksum.
#define CHECKSUM_CHECK
// Chuan 172 2002/02/28:
// if define INCLUDE_FLASH, Auto-Upgrade Procedure will use include files (FLASH.TXT)
//#define INCLUDE_FLASH
#endif  // #ifdef SUPPORT_AUTO_UPGRADE



// ******************************
// User Operation Behavior
// ******************************
// Micky1.22a, if no FP_PGC, just play from TT/CC 1/1
// define it, will play from TT/CC 1/1 when FP_PGC doesn't exist.
// for DVD title only.
// #define PLAY_TT1CC1_WHEN_NO_FPPGC

// ** TCH2.74; define it, can support VCD control. The Default value should set in [SETUP.H]
// If enable VCDControl, VCD-series can NOT play. Otherwise, same as original version.
///#define SUPPORT_VCD_CONTROL

// wyc.170a, define it when input number is bigger than end track, it will show invalid, else will go end track and play.
// #define NUMBER_OUTRANGE_INVALID

// DVD_161Micky, support MP3 resume can play from track begin
// if define it, when resume, can play the last played track
// #define SUPPORT_MP3_RESUME

// if define it, it will only support NPF key while in stop mode
// ie. it don't allow non-play playable macro command key in stop mode
// #define SUPPORT_NPF_TRIGGER_ONLY

// ** TCH1.72; if define it, it will assume NEXT key as KEY_PLAY (except MP3) while in stop mode
// #define SUPPORT_NEXT_TRIGGER_FROM_STOP

// Micky2.78b-2, add support for play chapter by number key.
// define it, will accept input number as chapter number and play it.
// only support the feature when 1) Title domain and 2)must have no selective buttons
// #define SUPPORT_PLAY_CHAPTER_BY_NUMBER

// define it,
// means system will enetr NO-PBC mode when following keys are pressed in PBC stop mode
// KEY_NUMBER, KEY_GOTOTIME, KEY_DIGEST. And will do the action
// #define    AUTO_PBCOFF

// define it, one stop -> will be treated as temporarily stop-> play will be treated as RESUME
//          , 2 stop -> will be treated as real stop.
// else stop, will always be treated as real stop.
///#define     DOUBLE_STOP

// define it, means the sequence of GoToTime is normal and "4" number force auto-gototime
// else the sequence is from left to right and don't auto-gototime
// #define    GOTOTIME_LEFTTORIGHT

// define it, means the GoToTime number can't over 5
// else the 1st number will be rounded off
//#define GOTOTIME_NUMBER_NOROUND

// define it, means N0 will be suppose as N10 or N20
// else N0 will be assumes as "0"
//#define     KEY_N0_SAME_NPLUS

// define the value of KEY_NPLUS
#define     KEY_NPLUS                   10      //***********

// define it, means after number key input, user need press "SEL" to confirm
// else the number will directly action
//#define SUPPORT_NUMBER_CONFIRM_KEY

// define it, means system will directly play [PROGRAM] Track;
// else another mode play [PROGRAM] Track after play current Track
///#define     PROGRAM_PLAY_DIRECT

// DVD_101bMicky, rename the define
// define it, means KEY_CLAER will clear current Program Item
// else will clear PROGRAM mode
// #define    ENABLE_CLEAR_CURRENTPROGRAMITEM

// define it, means KEY_AUTOVOCAL will have two meanings
// One is AUTOVOCAL (LL) mode, another is AUTOVOCAL (RR) mode
// #define    ENABLE_2_AUTOVOCAL

// hwtan 6.10 begin
// define it, means KEY_AUTOVOCAL will have three meanings
// Two are LL&RR mode, the other is channel 0 mode for some CVD&SVCD
// #define    ENABLE_3_AUTOVOCAL      //ping add for strong request
// hwtan 6.10 end

// define it, means SCF/SCB will switch to Normal Play after SCF/SCB-2
// else The keys of SCF/SCB only switch within SCAN mode
/// #define     ROUND_SCAN

// define it, means F/W will clear LOGO when push "PROGRAM" and "GOTOTIME"
// #define     FORCE_CLEAR_LOGO

// define it, means F/W will check __sbMICVocal PIN for AutoVocal
// #define     ENABLE_AUTOVOCAL         // ** SVCD-TCH;

// define it, means scart 16:9/4:3 setting only reference setup's setting. Won't reference bitstream format.
// #define     SET_SCART_FROM_SETUP

// ***** IR control
// define it, means REPEAT_AB and RESUME key is ONE key, not separate
// Please use KEY_RESUME to represent this key. (NOT KEY_REPEAT_AB)
//#define     SAME_AB_RESUME

// DVD051, define it, then repeat key will clear repeat a-b status
//#define     REPEAT_CLEAR_REPEATAB

// DVD_099bMicky, define it, KEY_AUDIO_LR and KEY_AST_CTL will be same key
// for DVD/SVCD/CVD, it's KEY_AST_CTL
// for VCD/MP3/CD-DA, it's KEY_AUDIO_LR
///#define KEY_AUDIO_LR_SAME_KEY_AST_CTL

// DVD_101bMicky, KEY_PLAY active as KEY_ACTIVE
// define it, KEY_PLAY will act as KEY_ACTIVE
// else it, KEY_PLAY will act as normal play only
// Can be defined only when IR KEY_ACTIVE is not exist
// #define SUPPORT_KEY_PLAY_AS_KEY_ACTIVE

// DVD_099bMicky, define it, for VCD/SVCD/CVD/CD-DA, the GOTOTIME value will be absolute time
// else it will depend on current show time mode(track relative mode)
///#define ABSOLUTE_GOTOTIME

// DVD_099bMicky, define it, it will auto switch to normal play when change track
// else it will keep original play mode
///#define FORCE_CHANGE_TRACK_NORMAL_PLAY

// **** TCH0.99C-XQ, define it, it will auto switch to NON-PBC mode when push Randm or Repeat Disc.
// else it will keep original play mode
///#define FORCE_SWITCH_NONPBC

// ** TCH1.01-1; define it, it will re-init AST/SPST and let's follow SETUP information.
// else it will keep original play AST/ SPST
///#define FORCE_DEFAULT_AST_SPST_AFTER_REAL_STOP

// ** TCH1.03; define it, it will set repeat mode as REPEAT_NONE. (ESS/ ZORAN)
// else it will keep original setting (Zoran)
///#define FORCE_SET_REPEAT_MODE_AFTER_REAL_NEXTPREV

// DVD_099bMicky, define it, will support a general backdoor mode
// for non-setup mdoe
///#define SUPPORT_GENERAL_BACKDOOR

// DVD_099cMicky, define it, will dynamic showing loading status
///#define SHOW_LOADING_STATUS

// DVD_101Micky, can select to use KEY_PLAY/OPEN_CLOSE release power down
/// define it, will allow KEY_PLAY to release power down mode
/// #define RELEASE_POWERDOWN_BY_PLAY
/// define it, will allow KEY_OPEN_CLOSE to release power down mode
/// #define RELEASE_POWERDOWN_BY_OPEN

///////////////////////////////////////////////
//  ******  Subpicture Relative Zone  *****
/////////////////////////////////////////////////
// Audio Output Relative Zoon
/////////////////////////////////////////////////
// wyc.170, define it to support CDDA and LPCM de-emphasis in W6630 audio DAC.
//#define SUPPORT_AUDIO_DEEMPHASYS

// LLY.171, if define it, DSP will output 6 channel audio data for non-DVD title
// while setting audio configure in 5.1 channel
// Otherwise, it's only output 2 channel audio data
// Notice: Must use 20011212 DSP code
// #define SUPPORT_6CH_OUTPUT_FOR_NONDVD

// LLY.171, if define it, DSP will do MPEG Up-Sample feature for MP3-LSF
// Ex: 16->48, 22->44, 32->48
// Otherwise, it will output original audio frequence
 #define SUPPORT_MPEG_AUDIO_UP_SAMPLE

// CMH2.55, define it to support PCM audio up-sample feature while audio code is ready.
// #define SUPPORT_PCM_AUDIO_UP_SAMPLE

// Brian.274, if define it, MIC input will be always disabled. This only works for
// DBB. For DAB, must replace DSP code to disable MIC input.
// #define NO_MIC_INPUT

// wyc1.00-909, add define to decide support FREEZE function or not.
//#define SUPPORT_FREEZE_FUNCTION

#define SUPPORT_JPG_PREVIEW

// ******************************************
// Video Output Configure Relative Zon
// ******************************************
// If define it, it will support CCIR656 video output
// Notice: must enable this define @ "internal.h" since servo code will reference it.
// #define SUPPORT_CCIR656

// If define it, it will display better quality @ Digital TFT Panel
// But it can't output on TV at the same time.
//++CoCo2.76, default enable digital panel for CT956A.

// LLY2.20, if define it, VCLK signal is not inverse
//#define VCLK_NO_INVERSE

// ******************************************
// JPEG Decoding Relative Zone
// ******************************************
#ifdef CT909R_IC_SYSTEM
	#define SUPPORT_JPEGDEC_ON_PROC2
#endif   // #ifdef CT909R_IC_SYSTEM
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
// CMH2.55, define it to support motion jpeg playback (only valie after CT909P IC)
#define SUPPORT_MOTION_JPEG
#endif  // #if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)

#ifdef SUPPORT_TFT
//#defien SUPPORT_COLOR_NORMALIZATOPN
#endif

// ******************************************
// AVI Behavior Related Zone
// ******************************************
// LLY2.20, if define it, video will be output immediately while repeat A-B
// Notice: the OSD time maybe incorrect @ position A
//#define ENABLE_VIDEO_IMMEDIATELY_WHILE_AVI_REPEAT_AB


////////////////////////////////////////////////
// SERVO Relative Zone
////////////////////////////////////////////////

// TCC161, for checking DVD tray-out by outer forces
///#define SUPPORT_LOADER_OPEN_BY_EXTRA_FORCE

//kevin1.10, slower playback speed but pitch remains
#define NORMAL_SLOW_MODE                    1
#define LANGUAGE_STUDY_MODE                 2
#define NORMAL_SLOW_PLUS_LANGUAGE_STUDY     3
#define SLOW_MODE_CONFIG                    NORMAL_SLOW_PLUS_LANGUAGE_STUDY

// define it, means CDROM driver can support how fast. Min. should be 2X.
 #define     SERVO_MAXSPEED     2
// #define     SERVO_MAXSPEED     4       // AVID

// ******************************
// DSA-IF only
// ******************************
#define DSP_I2S                 0x01
#define DSP_SONY                0x04

//#define DSP_FORMAT  DSP_I2S
#define DSP_FORMAT  DSP_SONY

// The value that can distinguish CD-DA or VCD
// For general normal (phillips and SONY) loader
#define     DATA_TRACK                      0x40

///////////////////////////////////////////////////////////
// ****** CPU speed's Zone ******
// !!!!!! Define the count for different CPU speed
///////////////////////////////////////////////////////////

// wyc1.07-909,
#if (CPU==CPU_SPARC)
// wyc2.22-909s, make all mode run same setting.
#define SYSTEM_TICK                     (2)
#define SLICE_TICK                      (1)

#define COUNT_10_SEC                    (10000L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_9_SEC                     (900L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_8_SEC                     (8000L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_7_SEC                     (7000L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_6_SEC                     (6000L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_5_SEC                     (5000L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_4_SEC                     (4000L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_3_SEC                     (3000L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_2_SEC                     (2000L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_1_SEC                     (1000L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_500_MSEC                  (500L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_400_MSEC                  (400L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_300_MSEC                  (300L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_250_MSEC                  (250L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_200_MSEC                  (200L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_150_MSEC                  (150L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_100_MSEC                  (100L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_50_MSEC                   (50L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_25_MSEC                   (25L/(SYSTEM_TICK*SLICE_TICK))
#define COUNT_10_MSEC                   (10L/(SYSTEM_TICK*SLICE_TICK))

#define COUNT_PER_SEC                   COUNT_1_SEC
#define COUNT_HALF_SEC                  COUNT_500_MSEC
#endif  //#if (CPU==CPU_X86)
#if (CPU==CPU_X86)
// Chuan0.62, 1 ms = 1 tick
#define COUNT_10_SEC                    182
#define COUNT_5_SEC                     91
#define COUNT_4_SEC                     73
#define COUNT_3_SEC                     58
#define COUNT_2_SEC                     37
#define COUNT_1_SEC                     19
#define COUNT_500_MSEC                  9
#define COUNT_400_MSEC                  8
#define COUNT_300_MSEC                  6
#define COUNT_250_MSEC                  5
#define COUNT_200_MSEC                  4
#define COUNT_150_MSEC                  3
#define COUNT_100_MSEC                  2
#define COUNT_50_MSEC                   1

#define COUNT_PER_SEC                   COUNT_1_SEC
#define COUNT_HALF_SEC                  COUNT_500_MSEC
#endif  //#if (CPU==CPU_X86)
// ########################################################

///////////////////////////////////////////////////////////
// ****** Programmer/Developer's Zone ******
// !!!!!! Can be modified by Programmer only !!!!!!
///////////////////////////////////////////////////////////
// DVD_102pMicky, add define for integrate TVMODE
#define MPEG_VIDEO_MODE_PAL     0
#define MPEG_VIDEO_MODE_NTSC    1

// ***** Message and Error Range **************************

#define MSG_INPUT_BASE      0x00
#define MSG_PANEL_BASE      0x80
#define MSG_OSD_BASE        0x00

#define ERR_NOERR           TRUE
#define ERR_ERROR           FALSE
#define ERR_KERNEL          0x10
#define ERR_UTL             0x40
#define ERR_CDINFO          0x60
#define ERR_SERVO           0x80
#define ERR_OSD             0xA0
#define ERR_PANEL           0xC0

#define INPB(dest)                  ((BYTE) dest)
#define OUTB(dest, val)             { dest = (BYTE) val ; }

///////////////////////////////////////////////////////////
// ******************************
// ***** Simple types & common helper macros **************
// ******************************
// OSD&PANEL macro  //jeffsun 20050119

#define OSD_OUTPUT_MACRO(mbMessage, mwParam, mbSec)\
{\
    OSD_PANEL_Output(mbMessage, mwParam, mbSec);\
}
#define OSD_TRIGGER_MACRO(mbType, mwTime)\
{\
    OSD_Trigger(mbType, mwTime);\
    PANEL_Trigger(mwTime);\
}
#define OSD_INITIAL_MACRO()\
{\
    OSD_Initial();\
    PANEL_Initial();\
}

// LLY.172r added, do the neceeary thing while exit application
extern  void _ExitApp(void);

//  #define     TEST
//:Enable test functions, for cdinfo.c/cdinfo2.c information only

// valid range of successive key
//loger the Key10+ display time
// 1) when input number is not confirmed after a period of time
//    the input number will be cleared
// 2) When 10+ is input a period of time, and no final digit is input
//    will clear the input value or auto input 0 as final digit(by define)
#define COUNT_NUMBER_GAP             COUNT_5_SEC

// ** TCH0.99D; define the global show time for OSD display
#define MSG_SHOWTIME_NORMAL          1
#define MSG_SHOWTIME_LONG            3

// define the count for retry reading TOC
#define RETRY_READ_TOC  1


//***********************************

// LLY.051, define it, means F/W will display LOGO while playing MP3 title
 #define     SUPPORT_MP3_WITH_LOGO

// define it, means [ISR] Module will do IFD counting and the mechanism will start All Rcovery Activility
// define RELEASE_SETTING will define ERROR_CONCEAL also.
//#define     ERROR_CONCEAL

// DVD_099cMicky, define it, OSD will display only for IR pressed key
#define SHOW_IR_KEY_OSD_ONLY

// ** TCH1.72; define it, OSD will display each IR-Key value (Hex).
// Even the Key is invalid....
//#define   _SHOW_IR_MAP

// ******************************
// STRICT zone for control spec.
// ******************************

// define it, means System will follow DVD Video Navigator control.
// Default is enable.
#define     STRICT_UOP

//////////////////////////////////////////////////////////////////

/* --CoCo2.37p
#ifdef  NO_DVD
#define SIMP_DVD
#endif  // NO_DVD
*/

// follow are char EXT file supporting selection.
#define PARSER_SPTXT_SRT    (0x00000001)
#define PARSER_SPTXT_SSA    (0x00000002)
#define PARSER_SPTXT_SUB    (0x00000004)
#define PARSER_SPTXT_SMI    (0x00000008)
#define PARSER_SPTXT_PSB    (0x00000010)

#ifdef SUPPORT_CHAR_SUBPICTURE
#if ((SELL_DESTINATION == CHINA) || (SELL_DESTINATION == TAIWAN) || (SELL_DESTINATION == HONGKONG) || (SELL_DESTINATION == JAPAN) || (SELL_DESTINATION == KOREA))
#define NO_CHAR_SUBPICTURE_MENU
#define CHAR_SUBPICTURE_ASCII_ONLY
#else
//#define NO_CHAR_SUBPICTURE_MENU
//#define ISO_8859_15_FONT    // ISO 8859-15
//#define ISO_8859_2_FONT     // ISO 8859-2
//#define ISO_8859_7_FONT         // ISO 8859-7
// Damon2.51, for Hebrew
//#define ISO_8859_8_FONT         // ISO 8859-8
//#define ISO_8859_9_FONT         // ISO 8859-9
//#define CP_1250_FONT            // CP 1250
//#define CP_1251_FONT            // CP 1251
#endif

//#define CUSTOMER_ASCII    // Use the custerm's ascii font instead of Cheertek
//#define SUPPORT_UTF8
#define SUPPORT_EX_SUBTITLE_FORMAT (PARSER_SPTXT_SRT | PARSER_SPTXT_SSA | PARSER_SPTXT_SUB | PARSER_SPTXT_SMI | PARSER_SPTXT_PSB)
// LLY1.20, If define it, it will parsing internal SP and char base SPST together
// Otherwise, it will skip char base SPST if exist internal SPST (ie. internal SPST is high priority)
//#define SUPPORT_INTERNAL_AND_CHAR_BASE_SUBPICTURE_TOGETHER

#endif

#ifdef  SUPPORT_POWER_SAVING
// LLY1.20, if define it, it will disable PROM when power saving mode.
//#define SUPPORT_PROM_DISABLE
#endif  // #ifdef SUPPORT_POWER_SAVING



// ** TCH0.391; Remove the definition "PRINTFSTR" to [DVDINFO]

#ifdef    RELEASE_SETTING

// Brian1.07, Disable VGA Video Output when Release mode
#define NO_VGA_OUTPUT

// define it, means enable PANEL_KeyScan in [ISR]
// else disable it as avoid the mistake if don't connect PANEL when testing
#if (DECODER_SYSTEM != DVD956A_DEV)
#define ENABLE_PANEL_KEYSCAN
#endif //(DECODER_SYSTEM != DVD956A_DEV)

// define it, means enable the IFD count checking
// LLY2.79-2, always enable ERROR_CONCEAL mechanism while support serial-debug or not
// Becasue, CoCo says that it's only code size issue to disable "Error Concealment" !!
#define  ERROR_CONCEAL //CoCo.sd

// wyc2.15-909S, support watch dog.
#ifdef ERROR_CONCEAL
// wyc2.51-909P, add J300 code.
#ifndef  AutoTuning_MPEG   //kenny disable watch dog for AT tool , 061123
#define SUPPORT_WATCH_DOG
#define SUPPORT_WATCHDOG_BURNIN // elmer2.78, support watchdog burn-in testing
#endif
#endif //

#ifdef  _DEBUG_INFO
#undef  _DEBUG_INFO
#endif  //_DEBUG_INFO

#else

#define _DEBUG_INFO
// Micky1.00, only debug mode turn off panel.
// Micky0.91, support panel, but only for P5 board.
//#define NO_PANEL    //jeffsun 20050103

#endif  //#ifdef    RELEASE_SETTING

#ifdef  SUPPORT_4M_FLASH
// wyc1.02-909, no DRM when define 4M flash.
#define NO_DRM

#endif // SUPPORT_4M_FLASH

// wyc0.95, remove this define for M200 to test function.
#define REMOVE_SETUP_SPEAKER_TEST
// wyc2.21-909s, enable it.
#define NO_DIGEST
#define NO_FLOAT_POINT
#define NO_DIRECT_OPERATION
#define NO_SHIFT
#define REMOVE_SETUP_ICON
#define REMOVE_SETUP_HELP_MESSAGE

// for 909S mode.
#define NO_PIP
#define RUNTIME_UNZIP_AUDIO_CODE
#define SWITCH_USB_PORT
// wyc2.17-909S, need to enable it.
#define STACK_OVERFLOW_DETECTION
#define NO_DRM
#define SUPPORT_VSYNCPOLL

// wyc2.16-909S, enable the define when 64M solution.

//kevin1.05, support power up music
//#define SUPPORT_POWERON_MUSIC

//LJY1.23, if enable "SUPPORT_CDDA_WITH_SUBCODE", the CDIF will output subcode for pure CDDA
// Micky1.24, disable the subcode, need DSP codes support
/// #define SUPPORT_CDDA_WITH_SUBCODE

//LJY1.24, if CDG was disabled, the CDIF won't output subcode.
#ifdef NO_CDG
#ifdef SUPPORT_CDDA_WITH_SUBCODE
#undef SUPPORT_CDDA_WITH_SUBCODE
#endif
#endif

// Alan2.18, to enable display or key scan
/* --CoCo2.76, enable "ENABLE_PANEL_OUTPUT" or "ENABLE_PANEL_KEYSCAN" if need. Don't use NO_PANEL to enable/disable it.
#ifndef NO_PANEL
#define ENABLE_PANEL_OUTPUT
#define ENABLE_PANEL_KEYSCAN
#endif // #ifndef NO_PANEL
*/



//Kevin1.23, the difference can't be noticable by eyes
//#define JPEG_MPEG_USE_DIFFERENT_COLOR_GAINS

//Kevin1.26-2, switch KEY_SCF & KEY_SCB function in FM
#define MODE_SCAN_ONLY  1   //used only for SCF/SCB
#define MODE_SCAN_PAGE  2   //used for SCF/SCB first, then page up/page down
#define MODE_PAGE_ONLY  3   //used only for page up/page down

#define FM_KEY_SCF_KEY_SCB_MODE     MODE_SCAN_ONLY



//Kevin1.26, customer may want to manually enable/disable PROLOGIC
//#define SUPPORT_PROLOGIC

//Kevin0.80a, customer may want to manually enable/disable DIVX
#define SUPPORT_DIVX

// wyc1.02-909, remove un-used define.
#ifndef DEBUG_PROC2
//kevin0.95, define it to reset Proc2 when audio reset fails, ...
#define SUPPORT_RUNTIME_RESET_PROC2
#endif

// wyc1.00-909, add Micky's new define
// Micky0.96, add a define for password query
// enable it, when parental level is not allowed, it will query password
// disable it, it will just display parental error, and don't allow the playing.
// must disable for Forum title test
///#define SUPPORT_PASSWORD_QUERY


// If define it, it will show black screen while playback un-supported DivX video
// Default is always on from v2.38a
#define BLACK_SCREEN_WHILE_THROW_VIDEO


// LLY0.80a, if define it, it will dump frame buffer data after each picture display done
// And, the result is used for PSNR testing.
//#define SUPPORT_PSNR_TEST

// LLY1.05, always disable ERROR_CONCEAL mechanism while do PSNR testing.
#ifdef  SUPPORT_PSNR_TEST
#undef  ERROR_CONCEAL
#endif  // #ifdef SUPPORT_PSNR_TEST

// LLY1.02, if define it, it will do some special setting for DivX Certification
// ex. only playback one file then stop it. ie. don't auto playback the next file.
//#define SUPPORT_DIVX_CERTIFICATION

#ifdef  SUPPORT_DIVX_CERTIFICATION
// LLY1.05, always disable FAST mode while DivX certification
#undef  SAME_FAST_AS_SCF2
// LLY1.11, always disable auto-playback feature while DivX certification
#undef  MMANAGER_AUTO_PLAY
#endif  // #ifdef SUPPORT_DIVX_CERTIFICATION

// LLY2.51, must disable watch dog while disable error concealment
#ifndef ERROR_CONCEAL
#undef  SUPPORT_WATCH_DOG
#endif  // #ifndef ERROR_CONCEAL


// LLY2.53, define the stream ID for background playback item
#define BG_ITEM_STREAM_ID   1

#ifdef CT909P_IC_SYSTEM
#define SUPPORT_BUILDIN_CARD_READER
#endif //


#ifdef CT951_PLATFORM //CoCo2.38
#define SUPPORT_USB_POWER_BY_IO
#define SUPPORT_BUILDIN_CARD_READER
#define IOMUTE_NOT_ADD_PULL_HIGH_5V
#define IOMUTE_ACTIVE_LOW
/*
#define REPEAT_MODE_REF_SETUP_INFO
#define SUPPORT_TFT
#define SUPPORT_CCIR656
#define SUPPORT_2_CVBS
#define NO_PANEL
#define ENABLE_PANEL_KEYSCAN
#define LED_BackLight
#ifdef NO_SCREEN_SAVER
#undef NO_SCREEN_SAVER
#endif //NO_SCREEN_SAVER
*/
#endif

//++CoCo2.37p
#define CT950_STYLE

// wyc2.37-909s,
#ifdef CT950_STYLE

#define REPEAT_MODE_REF_SETUP_INFO
#define SUPPORT_TFT
#define SUPPORT_CCIR656
#define SUPPORT_2_CVBS
#define NO_PANEL

#if (DECODER_SYSTEM != DVD956A_DEV)
#define ENABLE_PANEL_KEYSCAN
#endif //(DECODER_SYSTEM != DVD956A_DEV)

#define LED_BackLight
#ifdef NO_SCREEN_SAVER
#undef NO_SCREEN_SAVER
#endif //NO_SCREEN_SAVER

//#define SUPPORT_JPEG_FULL_SCREEN //JPEG full screen. For example, 4:3 picture will be full screen in 16:9 panel but the picture will be cut.

//#define EMU_RTC
#define SUPPORT_SAVE_VOLUME

//#define MUSTEK_SETUP_MENU

//#define NL_SOLUTION
#ifdef NL_SOLUTION
#define THUMB_NO_EDIT_MODE
#endif //NL_SOLUTION

//#define NO_TOOLBAR_SETUP_MENU

#define SUPPORT_SET_DATE_BY_MENU

#define SUPPORT_JPEG_AUDIO_PLAYING //1.05

#define DONT_SHOW_JPEG_PROGRESS_BAR

#define JPEG_SINGLE_BUFFER_FULL_SCREEN

#define SUPPORT_POWERON_MENU //CoCo2.38
#define IMAGE_FRAME_SETUP //CoCo2.38

#define SUPPORT_PHOTO_BROWSE_MODE

//#define SUPPORT_TEXT_TOOLBAR

//#define SUPPORT_ALARM_WITH_FM_TUNER

//#define SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS

//#define SUPPORT_CHOOSE_MEDIA

//#define SUPPORT_PLAY_MEDIA_DIRECTLY_POWER_ON

//#define SUPPORT_POWER_SWITCH

#ifdef CT909P_IC_SYSTEM
//#define SUPPORT_RADIO_TUNER
#ifdef SUPPORT_RADIO_TUNER
#define SUPPORT_RADIO_RDS
#endif //SUPPORT_RADIO_TUNER
#if (DECODER_SYSTEM != DVD956A_DEV)
#define SUPPORT_AUTO_ROTATION
#endif

#ifdef SUPPORT_AUTO_ROTATION
#define ROTATE_DIRECTION    KEY_LEFT
//#define ROTATE_ACTIVE_LOW
#endif SUPPORT_AUTO_ROTATION

//#define SUPPORT_USB_SWITCH

//#define SUPPORT_LIGHT_DETECT

#endif //CT909P_IC_SYSTEM

// Define favorite number for radio tuner. The maximum number can't exceed 40.
// The definition can't disabled. It's used in storage.h to set flash address.
#define RADIO_FAVORITE_NUM			20 

// wyc2.51-909P, JPG encode relative define.
#if (FLASH_SIZE != FLASH_SIZE_8)
#define SUPPORT_ENCODE_JPG_PICTURE
#define SUPPORT_BUILD_IN_ENCODE_JPG
#endif //FLASH_SIZE

#ifdef SUPPORT_BUILD_IN_ENCODE_JPG
#define BUILD_IN_JPG_ENCODE_NUM        3
#endif //

#ifdef SUPPORT_ENCODE_JPG_PICTURE
#define SUPPORT_COPY_FILE_IN_MM_UI
#define SUPPORT_DELETE_FILE_IN_THUMB_UI
#define SUPPORT_COPY_FILE_IN_SLIDE_SHOW
#define SUPPORT_FAVORITE_FUNCTION
#endif //SUPPORT_ENCODE_JPG_PICTURE

//Add the supported icons in the power on menu.
#define SUPPORT_PHOTO_ICON
#define SUPPORT_MUSIC_ICON
#define SUPPORT_PHOTO_MUSIC_ICON
#define SUPPORT_MOVIE_ICON
#define SUPPORT_SETUP_ICON
#define SUPPORT_CALENDAR_ICON
#define SUPPORT_CLOCK_ICON
#define SUPPORT_ALARM_ICON
#ifndef NL_SOLUTION
#define SUPPORT_AUTO_POWER_ICON
#endif //NL_SOLUTION
#ifdef SUPPORT_ENCODE_JPG_PICTURE
#define SUPPORT_EDIT_ICON
#ifdef SUPPORT_FAVORITE_FUNCTION
#define SUPPORT_FAVORITE_ICON
#endif //SUPPORT_FAVORITE_FUNCTION
#endif //SUPPORT_ENCODE_JPG_PICTURE
#ifdef SUPPORT_STB
#define SUPPORT_STB_ICON
#define SUPPORT_GAME_ICON
#endif //SUPPORT_STB
#ifdef SUPPORT_RADIO_TUNER
#define SUPPORT_RADIO_TUNER_ICON
#endif //SUPPORT_RADIO_TUNER

#if defined(CT909P_IC_SYSTEM ) || defined(CT909G_IC_SYSTEM)
#else
#define CT909R_JPEG_AND_MP3
#endif //

#ifdef IMAGE_FRAME_SETUP

#ifdef SUPPORT_TUNE_JPG_SHOW_TIME
#undef SUPPORT_TUNE_JPG_SHOW_TIME
#endif //SUPPORT_TUNE_JPG_SHOW_TIME

#ifdef MMANAGER_AUTO_PLAY
#undef  MMANAGER_AUTO_PLAY
#endif //MMANAGER_AUTO_PLAY

#define NO_MM_BUTTON

#endif //IMAGE_FRAME_SETUP

#define FORCE_ENABLE_DIVX_4_5

#define NO_DISC_MODE
//#define NO_DVD
#ifdef SUPPORT_SERVO_SOURCE
#undef SUPPORT_SERVO_SOURCE
#endif //SUPPORT_SERVO_SOURCE
#endif //CT950_STYLE

//#ifdef NO_CDDISC
#ifdef NO_DISC_MODE //CoCo2.38

#ifdef SUPPORT_UDF_FILE_SYSTEM
#undef SUPPORT_UDF_FILE_SYSTEM
#endif

//#define NO_VCDDISC

//#ifdef NO_VCDDISC
//#define NO_VCD20
//#endif //NO_VCDDISC

#define NO_CDG
#define NO_SEARCH_UI
#define NO_PROGRAM_UI
#ifdef OSDPRGM_MAIN_FILE
#undef OSDPRGM_MAIN_FILE
#endif

#ifdef SUPPORT_BOOKMARK
#undef SUPPORT_BOOKMARK
#endif //SUPPORT_BOOKMARK

#ifndef NO_DIGEST
#define NO_DIGEST
#ifdef OSDDG_MAIN_FILE
#undef OSDDG_MAIN_FILE
#endif
#endif //NO_DIGEST

#define NO_DVD_DISPLAY_UI
#define NO_VCD_DISPLAY_UI
#define NO_DVD_PASSWORD_DLG //CoCo2.38

#ifdef SUPPORT_NAVIGATOR
#undef SUPPORT_NAVIGATOR
#endif //SUPPORT_NAVIGATOR

#endif //NO_DISC_MODE

// For the declaration of stings used in each UI
#define OSDDSPLY_MAIN_FILE
#define OSDND_MAIN_FILE
#define OSDMM_MAIN_FILE
#define THUMB_MAIN_FILE
#ifndef CT950_STYLE
#define OSDDG_MAIN_FILE //CoCo2.38
#define OSDPRGM_MAIN_FILE //CoCo2.38
#define SETUP_MAIN_FILE //CoCo2.38
#define SUPPORT_PSCAN_PROMPT_SETUP //CoCo2.38
#else
#define IMAGE_FRAME_MENU_MAIN_FILE //CoCo2.38
#endif //CT950_STYLE
#ifndef SUPPORT_STB  //Alan2.31, for the compatibility of DVD use DVB string files
#ifndef IMAGE_FRAME_SETUP
#define MENU_MAIN_FILE //CoCo2.38
#endif //IMAGE_FRAME_SETUP
#endif
#ifdef SUPPORT_BOOKMARK
#define OSDBMARK_MAIN_FILE
#endif

#define SUPPORT_AUDIO_EFFECT

// LLY2.38, add one define to force enable DivX 4/5 to treat as MPEG4 even the IC don't support DivX feature
//#define FORCE_ENABLE_DIVX_4_5

#ifdef  FORCE_ENABLE_DIVX_4_5
#define NO_DIVX_311
#define NO_DRM
#endif  // #ifdef FORCE_ENABLE_DIVX_4_5


#include "oswrap.h"

// LLY2.18, put "pio.h" at latest stage
// Since, it may reference some define first, ex SUPPORT_CCIR656
// LLY2.11, include "pio.h" to get corresponding pio define
// Notice: Must be put after DECODER_SYSTEM is ready
#include    "pio.h"



// LLY2.16, put the error check of user configure setting at latest stage
// To make sure all setting are configure done
#include "errchk.h"

// Micky2.17c
#ifdef  APPLICATION_WINDOWS
#include	"xsupport.h"
#endif  //#ifdef  APPLICATION_WINDOWS
#ifdef __cplusplus
        }
#endif

#endif  // __WINAV_H__


