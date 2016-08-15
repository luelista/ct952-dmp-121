/**************************************************************************

        (C)Copyright Cheertek Inc. 2005 - 2007
           J000, all right reserved.

        Product : CheerDVD Firmware

        Date    : 2006.02.20
        Author  : Cheertek (J200 LLY)
        Purpose : GPIO pin declaration
        Sources : pio.h
        Usage   : Will be inclued @ winav.h
****************************************************************************/

#ifndef __PIO_H__
#define __PIO_H__

// LLY2.13, put GPIO group define here (remove form hal.h), since following code will reference it
#define GPIO_NONE   0
#define GPIO_A      1
#define GPIO_B      2
#define GPIO_C      3
#define GPIO_D      4
#define GPIO_E      5
#define GPIO_F      6
#define GPIO_EXP    7
// LLY2.50, add extra GPIO group for CT909P
#ifdef  CT909P_IC_SYSTEM
#define GPIO_G      8
#define GPIO_H      9
#endif  // #ifdef  CT909P_IC_SYSTEM


// LLY2.15, if define it, expand GPIO input mode will think low level is active.
#define ACTIVE_LOW


// LLY2.61, define the ID for SCART PWM ID
// Notice: must put within "pio.h" since following code will reference it
#define SCART_PWM0      (0) // Using GPA[6]
#define SCART_PWM1      (1) // Using GPA[8]


//////////////////////////////////////////////////////////////////////////////////////
// [1] H/W Mute pin
// LLY2.13, modify HWMUTE pin related.
#if (DECODER_SYSTEM == DVD909S_EVAL)
#define HWMUTE_GPIO_GRP     GPIO_A
#define PIN_HWMUTE          31    // GPA 31
#endif  // #if (DECODER_SYSTEM == DVD909S_EVAL)

#if (DECODER_SYSTEM == DVD909R_EVAL)
#ifndef CT951_PLATFORM
#define HWMUTE_GPIO_GRP     GPIO_EXP
#define PIN_HWMUTE          13    // EXP 13
#else  // #ifndef CT951_PLATFORM
#define USB_POWER_GRP       GPIO_A              //CoCo2.38
#define PIN_USB_POWER       29     // GPA 29    //CoCo2.38
#define HWMUTE_GPIO_GRP     GPIO_A
#define PIN_HWMUTE          8
#endif // #ifndef CT951_PLATFORM
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)

// LLY2.15, add HWMUTE pin for 909R-HFY platform.
#if (DECODER_SYSTEM == DVD909R_HFY)
#define HWMUTE_GPIO_GRP     GPIO_A
#define PIN_HWMUTE          13    // GPA 13
#endif  // #if (DECODER_SYSTEM == DVD909R_HFY)

// LLY2.16, add HWMUTE pin for CT219-HCN
#if (DECODER_SYSTEM == STB219_HCN)
#define HWMUTE_GPIO_GRP     GPIO_A
#define PIN_HWMUTE          13    // GPA 13
#endif  // #if (DECODER_SYSTEM == STB219_HCN)

// LLY2.16, add HWMUTE pin for CT219-YHI
#if (DECODER_SYSTEM == STB219_YHI)
#define HWMUTE_GPIO_GRP     GPIO_A
#define PIN_HWMUTE          13    // GPA 13
#endif  // #if (DECODER_SYSTEM == STB219_YHI)

// LLY2.50, add for CT909P 216 pin system
#if (DECODER_SYSTEM == DVD956A_DEV)
// LLY2.51, IO mute pin is change from GPC 0 to GPC 3,
// and ACTIVE lever is low + not add pull high 5V
#define HWMUTE_GPIO_GRP     GPIO_C
#define PIN_HWMUTE          3    // GPC 3
#define IOMUTE_ACTIVE_LOW
//#define IOMUTE_NOT_ADD_PULL_HIGH_5V
#endif  // #if (DECODER_SYSTEM == DVD956A_DEV)

// LLY2.55, add for CT952A system
#if (DECODER_SYSTEM == DMP952A_EVAL)
#define HWMUTE_GPIO_GRP     GPIO_G
#define PIN_HWMUTE          1   // GPC5
#define IOMUTE_ACTIVE_LOW
//#define IOMUTE_NOT_ADD_PULL_HIGH_5V
#endif  // #if (DECODER_SYSTEM == DMP952A_EVAL)

// SYuan 288, add for CT952B system 
// IO mute pin is change from GPC 5 to GPE 0
#if (DECODER_SYSTEM == DMP952B_EVAL)
#define HWMUTE_GPIO_GRP     GPIO_E
#define PIN_HWMUTE           0   // GPE0
#define IOMUTE_ACTIVE_LOW
#define IOMUTE_NOT_ADD_PULL_HIGH_5V
#endif  // #if (DECODER_SYSTEM == DMP952B_EVAL)

// LLY2.56, add for CT909G system
#if (DECODER_SYSTEM == DVD909G_EVAL)
#define HWMUTE_GPIO_GRP     GPIO_A
#define PIN_HWMUTE          4   // GPA4
#define IOMUTE_ACTIVE_LOW
#define IOMUTE_NOT_ADD_PULL_HIGH_5V
#endif  // #if (DECODER_SYSTEM == DVD909G_EVAL)

// LLY2.60, add for CT909P-A 128 pin system
#if (DECODER_SYSTEM == DVD909P_A_EVAL)
#define HWMUTE_GPIO_GRP     GPIO_EXP
#define PIN_HWMUTE          3    // Expand GPIO 3
#define IOMUTE_ACTIVE_LOW
#define IOMUTE_NOT_ADD_PULL_HIGH_5V
#endif  // #if (DECODER_SYSTEM == DVD909P_A_EVAL)

// LLY2.60, add for CT909P-B 128 pin system
#if (DECODER_SYSTEM == DVD909P_B_EVAL)
#define HWMUTE_GPIO_GRP     GPIO_EXP
#define PIN_HWMUTE          3    // Expand GPIO 3
#define IOMUTE_ACTIVE_LOW
#define IOMUTE_NOT_ADD_PULL_HIGH_5V
#endif  // #if (DECODER_SYSTEM == DVD909P_B_EVAL)

// LLY2.60, add for CT956A system
#if (DECODER_SYSTEM == DMP956A_EVAL)
#define HWMUTE_GPIO_GRP     GPIO_C
#define PIN_HWMUTE          3   // GPC3
#define IOMUTE_ACTIVE_LOW
#define IOMUTE_NOT_ADD_PULL_HIGH_5V
#endif  // #if (DECODER_SYSTEM == DMP956A_EVAL)

// LLY2.60, add for CT955A system
#if (DECODER_SYSTEM == DMP955A_EVAL)
#define HWMUTE_GPIO_GRP     GPIO_C
#define PIN_HWMUTE          3   // GPC3
#define IOMUTE_ACTIVE_LOW
#define IOMUTE_NOT_ADD_PULL_HIGH_5V
#endif  // #if (DECODER_SYSTEM == DMP955A_EVAL)


////////////////////////////////////////////////////////////////////////////////////
// [2] EEPROM pin
// Define I2C data/ clock pin for EEPROM R/W access (remove from TVE.C) -- LLY2.01
// LLY2.04, redefine EEPROM R/W data/ clock pin as GPA1, GPA4 for CT909S
#define EEPROM_GPIO_GRP     GPIO_A
#define PIN_EEPROM_SDAT     1
#define PIN_EEPROM_SCLK     4


///////////////////////////////////////////////////////////////////////////////////
// [3] SCART pin
#if (DECODER_SYSTEM == DVD909S_EVAL)
#define SCART_GPIO_GRP  GPIO_A
#define PIN_SCART_SW    20
#define PIN_SCART_TV    19
#define PIN_SCART_RGB   21
#endif  // #if (DECODER_SYSTEM == DVD909S_EVAL)

#if (DECODER_SYSTEM == DVD909R_EVAL)
#define SCART_GPIO_GRP  GPIO_A
#define PIN_SCART_SW    10
#define PIN_SCART_TV    11
#define PIN_SCART_RGB   14
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)

// LLY2.15, add for 909R-HFY system platform
#if (DECODER_SYSTEM == DVD909R_HFY)
#define SCART_GPIO_GRP  GPIO_A
#define PIN_SCART_SW    10
#define PIN_SCART_TV    11
#define PIN_SCART_RGB   14
#endif  // #if (DECODER_SYSTEM == DVD909R_HFY)

// LLY2.61, add for CT909G system
#if (DECODER_SYSTEM == DVD909G_EVAL)
#define SUPPORT_SCART_PWM_CTRL
#define PIN_SCART_PWM   SCART_PWM1
#define SCART_GPIO_GRP  GPIO_A
#define PIN_SCART_RGB   7
#endif  // #if (DECODER_SYSTEM == DVD909G_EVAL)



//////////////////////////////////////////////////////////////////////////////////
// [4] VFD control pin
#if (DECODER_SYSTEM == DVD909S_EVAL)
#define VFD_STB_GPIO_GRP    GPIO_A
// wyc2.17-909S, update to correct pin assignment in 909S
#define GPIO_VFD_STB        0     // GPA 0
#define GPIO_VFD_SDA        1     // GPA 1
#define GPIO_VFD_SCK        4     // GPA 4
#endif  // #if (DECODER_SYSTEM == DVD909S_EVAL)

#if (DECODER_SYSTEM == DVD909R_EVAL)
#define VFD_STB_GPIO_GRP    GPIO_B
#define GPIO_VFD_STB        12      // GPB 12
#define GPIO_VFD_SDA        18      // GPA 18
#define GPIO_VFD_SCK        7       // GPA 7
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)

// LLY2.15, add for 909R-HFY system platform
#if (DECODER_SYSTEM == DVD909R_HFY)
#define VFD_STB_GPIO_GRP    GPIO_A
#define GPIO_VFD_STB        16     // GPA 16
#define GPIO_VFD_SDA        12     // GPA 12
#define GPIO_VFD_SCK        7      // GPA 7
#endif  // #if (DECODER_SYSTEM == DVD909R_HFY)


//////////////////////////////////////////////////////////////////////////////////
// [5] Audio DAC control pin
#if (DECODER_SYSTEM == DVD909S_EVAL)
// LLY2.20, correct ADAC control pin to meet original design
#define PIN_ADAC_SCK            11
#define PIN_ADAC_SDA            14
#define ADAC_CS6CH_GPIO_GRP     GPIO_A
#define PIN_ADAC_CS6CH          10
//#define PIN_ADAC_CS2CH          14
#endif  // #if (DECODER_SYSTEM == DVD909S_EVAL)

#if (DECODER_SYSTEM == DVD909R_EVAL)
// Not used it.
#define PIN_ADAC_SCK            10
#define PIN_ADAC_SDA            13
#define ADAC_CS6CH_GPIO_GRP     GPIO_A
#define PIN_ADAC_CS6CH          11
#define PIN_ADAC_CS2CH          14
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)

#if (DECODER_SYSTEM == DVD909R_HFY)
#define PIN_ADAC_SCK            7   // GPA 7
#define PIN_ADAC_SDA            12  // GPA 12
#define ADAC_CS6CH_GPIO_GRP     GPIO_B
#define PIN_ADAC_CS6CH          12  // GPB 12
#define PIN_ADAC_CS2CH          14  // Not used
#endif  // #if (DECODER_SYSTEM == DVD909R_HFY)



//////////////////////////////////////////////////////////////////////////////////
// [6] DVD/ DVB mode switch control pin
#if (DECODER_SYSTEM == DVD909R_EVAL)
#define DVD_DVB_SWITCH_GRP      GPIO_EXP
#define PIN_DVD_DVB_SWITCH      8   //  Expand GPIO 8
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)

// Notice: will be removed later
#if (DECODER_SYSTEM == DVD909R_HFY)
#define DVD_DVB_SWITCH_GRP      GPIO_EXP
#define PIN_DVD_DVB_SWITCH      8   //  Expand GPIO 8
#endif  // #if (DECODER_SYSTEM == DVD909R_HFY)

// LLY2.16, add for HCN system board
#if (DECODER_SYSTEM == STB219_HCN)
#define DVD_DVB_SWITCH_GRP      GPIO_EXP
#define PIN_DVD_DVB_SWITCH      8   //  Expand GPIO 8
#endif  // #if (DECODER_SYSTEM == STB219_HCN)

// LLY2.16, add for YHI system board
#if (DECODER_SYSTEM == STB219_YHI)
#define DVD_DVB_SWITCH_GRP      GPIO_EXP
#define PIN_DVD_DVB_SWITCH      8   //  Expand GPIO 8
#endif  // #if (DECODER_SYSTEM == STB219_YHI)

// LLY2.50, add for CT909P 216 pin system
#if (DECODER_SYSTEM == DVD956A_DEV)
#define DVD_DVB_SWITCH_GRP      GPIO_A
#define PIN_DVD_DVB_SWITCH      20      // GPA 20
#endif  // #if (DECODER_SYSTEM == DVD956A_DEV)

// LLY2.60, add for CT909P-A 128 pin system
#if (DECODER_SYSTEM == DVD909P_A_EVAL)
#define DVD_DVB_SWITCH_GRP      GPIO_EXP
#define PIN_DVD_DVB_SWITCH      0      // Expand GPIO 0
#endif  // #if (DECODER_SYSTEM == DVD909P_A_EVAL)

// LLY2.60, add for CT909P-B 128 pin system
#if (DECODER_SYSTEM == DVD909P_B_EVAL)
#define DVD_DVB_SWITCH_GRP      GPIO_EXP
#define PIN_DVD_DVB_SWITCH      0      // Expand GPIO 0
#endif  // #if (DECODER_SYSTEM == DVD909P_A_EVAL)

#if (DECODER_SYSTEM == DVD909S_EVAL)
#define DVD_DVB_SWITCH_GRP      GPIO_E
#define PIN_DVD_DVB_SWITCH      8
#endif  // #if (DECODER_SYSTEM == DVD909S_EVAL)

// LLY2.60 need add "#define NO_DVD_DVB_SWITCHING while don't support DVD/ DVB mode switching pin
// ex. CT952A, CT909G
#ifndef PIN_DVD_DVB_SWITCH
#define NO_DVD_DVB_SWITCHING
#endif  // #ifndef PIN_DVD_DVB_SWITCH


//////////////////////////////////////////////////////////////////////////////////////
// [7] NIM reset control pin
#if (DECODER_SYSTEM == DVD909S_EVAL)
#define NIM_RESET_GRP           GPIO_B
#define NIM_RESET_PIN           14  // GPB 14
#endif  // #if (DECODER_SYSTEM == DVD909S_EVAL)

#if (DECODER_SYSTEM == DVD909R_EVAL)
// LLY2.56, porting CoCo's code to add NIM reset pin for CT951 platform.
#ifndef CT951_PLATFORM
#define NIM_RESET_GRP           GPIO_EXP
#define NIM_RESET_PIN           9  // Expand GPIO 9
#else
#define NIM_RESET_GRP           GPIO_A
#define NIM_RESET_PIN           18
#endif //CT951_PLATFORM
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)

// LLY2.16, add for HCN system board
#if (DECODER_SYSTEM == STB219_HCN)
#define NIM_RESET_GRP           GPIO_EXP
#define NIM_RESET_PIN           9  // Expand GPIO 9
#endif  // #if (DECODER_SYSTEM == STB219_HCN)

// LLY2.16, add for YHI system board
#if (DECODER_SYSTEM == STB219_YHI)
#define NIM_RESET_GRP           GPIO_EXP
#define NIM_RESET_PIN           9  // Expand GPIO 9
#endif  // #if (DECODER_SYSTEM == STB219_YHI)

// LLY2.50, add for CT909P 216 pin system
#if (DECODER_SYSTEM == DVD956A_DEV)
#define NIM_RESET_GRP           GPIO_C
#define NIM_RESET_PIN           1  // GPC 1
#endif  // #if (DECODER_SYSTEM == DVD956A_DEV)

// LLY2.55, add for CT952A system
#if (DECODER_SYSTEM == DMP952A_EVAL)
#define NIM_RESET_GRP           GPIO_C
#define NIM_RESET_PIN           0   // GPC 0
#endif  // #if (DECODER_SYSTEM == DMP952A_EVAL)

// SYuan 2.88, add for CT952B system
#if (DECODER_SYSTEM == DMP952B_EVAL)
#define NIM_RESET_GRP           GPIO_C
#define NIM_RESET_PIN           0   // GPC 0
#endif  // #if (DECODER_SYSTEM == DMP952B_EVAL)

// LLY2.60, add for CT909P-A 128 pin
#if (DECODER_SYSTEM == DVD909P_A_EVAL)
#define NIM_RESET_GRP           GPIO_EXP
#define NIM_RESET_PIN           9   // Expand GPIO 9
#endif  // #if (DECODER_SYSTEM == DVD909P_A_EVAL)

// LLY2.60, add for CT909P-B 128 pin
#if (DECODER_SYSTEM == DVD909P_B_EVAL)
#define NIM_RESET_GRP           GPIO_EXP
#define NIM_RESET_PIN           9   // Expand GPIO 9
#endif  // #if (DECODER_SYSTEM == DVD909P_B_EVAL)

// LLY2.60, add for CT956A system
#if (DECODER_SYSTEM == DMP956A_EVAL)
#define NIM_RESET_GRP           GPIO_C
#define NIM_RESET_PIN           1      // GPC 1
#endif  // #if (DECODER_SYSTEM == DVD956A_EVAL)


///////////////////////////////////////////////////////////////////////////////
// [8] S/W expand GPIO control pin -- LLY2.16
#if (DECODER_SYSTEM == DVD909S_EVAL)
#define KS_DATA_GRP         GPIO_A
#define GPIO_KS_DATA        18     // GPA 18
#define KS_CLK_GRP          GPIO_A
#define GPIO_KS_CLK         16     // GPA 16
#define KS_DET0_GRP         GPIO_B
#define GPIO_KS_DET0        10     // GPB 10
#define KS_DET1_GRP         GPIO_B
#define GPIO_KS_DET1        11     // GPB 11
#define KS_LATCH0_GRP       GPIO_B
#define GPIO_KS_LATCH0      1      // GPB 1
#define KS_LATCH1_GRP       GPIO_B
#define GPIO_KS_LATCH1      14     // GPB 14
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)

#if (DECODER_SYSTEM == DVD909R_EVAL)
#ifndef CT951_PLATFORM
#define KS_DATA_GRP         GPIO_A
#define GPIO_KS_DATA        18     // GPA 18
#define KS_CLK_GRP          GPIO_A
#define GPIO_KS_CLK         16     // GPA 16
// LLY2.18, using original GPIO pin for key detect 0/1 while not support CCIR656
#ifdef  SUPPORT_CCIR656
#define KS_DET0_GRP         GPIO_A
#define GPIO_KS_DET0        28     // GPA 28
#define KS_DET1_GRP         GPIO_A
#define GPIO_KS_DET1        29     // GPA 29
#else   // #ifdef SUPPORT_CCIR656
#define KS_DET0_GRP         GPIO_B
#define GPIO_KS_DET0        10     // GPB 10
#define KS_DET1_GRP         GPIO_B
#define GPIO_KS_DET1        11     // GPB 11
#endif  // #ifdef SUPPORT_CCIR656
#define KS_LATCH0_GRP       GPIO_B
#define GPIO_KS_LATCH0      1      // GPB 1
#define KS_LATCH1_GRP       GPIO_B
#define GPIO_KS_LATCH1      14     // GPB 14
#else  // #ifndef CT951_PLATFORM

#define KS_DATA_GRP         GPIO_A
#define GPIO_KS_DATA        18        // Not used
#define KS_CLK_GRP          GPIO_A
#define GPIO_KS_CLK         16        // Not used
// LLY2.18, using original GPIO pin for key detect 0/1 while not support CCIR656
#ifdef  SUPPORT_CCIR656
#define KS_DET0_GRP         GPIO_B
#define GPIO_KS_DET0        15        // Not used
#define KS_DET1_GRP         GPIO_B
#define GPIO_KS_DET1        16        // Not used
#else   // #ifdef SUPPORT_CCIR656
#define KS_DET0_GRP         GPIO_B
#define GPIO_KS_DET0        15        // Not used
#define KS_DET1_GRP         GPIO_B
#define GPIO_KS_DET1        16        // Not used
#endif  // #ifdef SUPPORT_CCIR656
#define KS_LATCH0_GRP       GPIO_B
#define GPIO_KS_LATCH0      17        // Not used
#define KS_LATCH1_GRP       GPIO_B
#define GPIO_KS_LATCH1      18        // Not used

#define USB_POWER_GRP       GPIO_A
#define PIN_USB_POWER       29     // GPA 12
#define POWER_LED           GPIO_A
#define GPIO_PLED           28
#endif // #ifndef CT951_PLATFORM
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)


// LLY2.16, add for HCN system board
#if (DECODER_SYSTEM == STB219_HCN)
#define KS_DATA_GRP         GPIO_A
#define GPIO_KS_DATA        18     // GPA 18
#define KS_CLK_GRP          GPIO_A
#define GPIO_KS_CLK         16     // GPA 16
#define KS_DET0_GRP         GPIO_A
#define GPIO_KS_DET0        28     // GPA 28
#define KS_DET1_GRP         GPIO_A
#define GPIO_KS_DET1        29     // GPA 29
#define KS_LATCH0_GRP       GPIO_B
#define GPIO_KS_LATCH0      1      // GPB 1
#define KS_LATCH1_GRP       GPIO_B
#define GPIO_KS_LATCH1      14     // GPB 14
#endif  // #if (DECODER_SYSTEM == STB219_HCN)


// LLY2.16, add for YHI system board
#if (DECODER_SYSTEM == STB219_YHI)
#define KS_DATA_GRP         GPIO_A
#define GPIO_KS_DATA        18     // GPA 18
#define KS_CLK_GRP          GPIO_A
#define GPIO_KS_CLK         16     // GPA 16
#define KS_DET0_GRP         GPIO_A
#define GPIO_KS_DET0        28     // GPA 28
#define KS_DET1_GRP         GPIO_A
#define GPIO_KS_DET1        29     // GPA 29
#define KS_LATCH0_GRP       GPIO_B
#define GPIO_KS_LATCH0      1      // GPB 1
#define KS_LATCH1_GRP       GPIO_B
#define GPIO_KS_LATCH1      14     // GPB 14
#endif  // #if (DECODER_SYSTEM == STB219_YHI)

// LLY2.60, add for CT909P-A 128 pin system board
#if (DECODER_SYSTEM == DVD909P_A_EVAL)
#define KS_DATA_GRP         GPIO_C
#define GPIO_KS_DATA        1      // GPC 1
#define KS_CLK_GRP          GPIO_C
#define GPIO_KS_CLK         2      // GPC 2
#define KS_DET0_GRP         GPIO_C
#define GPIO_KS_DET0        0      // GPC 0
#define KS_DET1_GRP         GPIO_C
#define GPIO_KS_DET1        5      // GPC 5
#define KS_LATCH0_GRP       GPIO_C
#define GPIO_KS_LATCH0      3      // GPC 3
#define KS_LATCH1_GRP       GPIO_C
#define GPIO_KS_LATCH1      4      // GPC 4
#endif  // #if (DECODER_SYSTEM == DVD909P_A_EVAL)

// LLY2.60, add for CT909P-B 128 pin system board
#if (DECODER_SYSTEM == DVD909P_B_EVAL)
#define KS_DATA_GRP         GPIO_C
#define GPIO_KS_DATA        1      // GPC 1
#define KS_CLK_GRP          GPIO_C
#define GPIO_KS_CLK         2      // GPC 2
#define KS_DET0_GRP         GPIO_C
#define GPIO_KS_DET0        0      // GPC 0
#define KS_DET1_GRP         GPIO_C
#define GPIO_KS_DET1        5      // GPC 5
#define KS_LATCH0_GRP       GPIO_C
#define GPIO_KS_LATCH0      3      // GPC 3
#define KS_LATCH1_GRP       GPIO_C
#define GPIO_KS_LATCH1      4      // GPC 4
#endif  // #if (DECODER_SYSTEM == DVD909P_B_EVAL)


////////////////////////////////////////////////////////////////////////////
// [9] USB power control pin -- LLY2.16
#if (DECODER_SYSTEM == STB219_YHI)
#define USB_POWER_GRP       GPIO_A
#define PIN_USB_POWER       12     // GPA 12
#endif  // #if (DECODER_SYSTEM == STB219_YHI)

// LLY2.55, porting CoCo's code to define USB power control pin for 952A (GPA 29)
// LLY2.76, modify the pin as GPG 4 base on J100-HCC suggestion.
#if (DECODER_SYSTEM == DMP952A_EVAL)
#define USB_POWER_GRP       GPIO_G
#define PIN_USB_POWER       4       // GPG 4
#endif  // #if (DECODER_SYSTEM == DMP952A_EVAL)

//SYuan 2.88: USB POWER pin is change from GPG 4 to GPE 1
#if (DECODER_SYSTEM == DMP952B_EVAL)
#define USB_POWER_GRP       GPIO_E
#define PIN_USB_POWER       1       // GPE 1
#endif  // #if (DECODER_SYSTEM == DMP952B_EVAL)

// LLY2.60, add for CT909P-A 128 pin system board
#if (DECODER_SYSTEM == DVD909P_A_EVAL)
#define USB_POWER_GRP       GPIO_EXP
#define PIN_USB_POWER       4       // Expand GPIO 4
#endif  // #if (DECODER_SYSTEM == DVD909P_A_EVAL)

// LLY2.60, add for CT909P-B 128 pin system board
#if (DECODER_SYSTEM == DVD909P_B_EVAL)
#define USB_POWER_GRP       GPIO_EXP
#define PIN_USB_POWER       4       // Expand GPIO 4
#endif  // #if (DECODER_SYSTEM == DVD909P_B_EVAL)

// LLY2.60, add for CT956A system
#if (DECODER_SYSTEM == DMP956A_EVAL)
// 5-pin USB : GPE[1]; 4-pin USB: GPE[0]
//wyc2.78-909P,
#define USB_POWER_GRP       GPIO_E
//#define PIN_USB_POWER       1       // GPE 1
#define PIN_USB_POWER       0       // GPE 0
#endif  // #if (DECODER_SYSTEM == DMP956A_EVAL)

// LLY2.60, add for CT955A system
#if (DECODER_SYSTEM == DMP955A_EVAL)
#define USB_POWER_GRP       GPIO_E
//#define PIN_USB_POWER       1       // GPE 1
#define PIN_USB_POWER       0       // GPE 0
#endif  // #if (DECODER_SYSTEM == DMP955A_EVAL)

#if (DECODER_SYSTEM == DVD956A_DEV)
#define USB_POWER_GRP       GPIO_G
#define PIN_USB_POWER       4       // GPE 4
#endif  // #if (DECODER_SYSTEM == DMP955A_EVAL)

// LLY2.76, always enable USB power control by IO feature if define these pin.
#if defined(PIN_USB_POWER) && defined(USB_POWER_GRP)
#define SUPPORT_USB_POWER_BY_IO
#endif  // #if defined(PIN_USB_POWER) && defined(USB_POWER_GRP)


////////////////////////////////////////////////////////////////////////
// [10] NIM I2C data/sclk pin -- LLY2.16
#if (DECODER_SYSTEM == DVD909S_EVAL)
#define NIM_I2C_GRP         GPIO_B
#define NIM_I2C_SCLKPIN     15      // GPB 15
#define NIM_I2C_DATPIN      16      // GPB 16
#endif  // #if (DECODER_SYSTEM == DVD909S_EVAL)

#if (DECODER_SYSTEM == DVD909R_EVAL)
#define NIM_I2C_GRP         GPIO_B
#define NIM_I2C_SCLKPIN     15      // GPB 15
#define NIM_I2C_DATPIN      16      // GPB 16
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)

#if (DECODER_SYSTEM == STB219_HCN)
#define NIM_I2C_GRP         GPIO_B
#define NIM_I2C_SCLKPIN     15      // GPB 15
#define NIM_I2C_DATPIN      16      // GPB 16
#endif  // #if (DECODER_SYSTEM == STB219_HCN)

#if (DECODER_SYSTEM == STB219_YHI)
#define NIM_I2C_GRP         GPIO_B
#define NIM_I2C_SCLKPIN     15      // GPB 15
#define NIM_I2C_DATPIN      16      // GPB 16
#endif  // #if (DECODER_SYSTEM == STB219_YHI)

#if (DECODER_SYSTEM == DVD956A_DEV) // Aron2.52
#define NIM_I2C_GRP         GPIO_B
#define NIM_I2C_SCLKPIN     1      // GPB 1
#define NIM_I2C_DATPIN      3      // GPB 3
#endif  // #if (DECODER_SYSTEM == DVD956A_DEV)

#if (DECODER_SYSTEM == DMP952A_EVAL)  // LLY2.55
#define NIM_I2C_GRP         GPIO_B
#define NIM_I2C_SCLKPIN     1      // GPB 1
#define NIM_I2C_DATPIN      3      // GPB 3
#endif  // #if (DECODER_SYSTEM == DMP952A_EVAL)

#if (DECODER_SYSTEM == DMP952B_EVAL)  // SYuan 2.88
#define NIM_I2C_GRP         GPIO_B
#define NIM_I2C_SCLKPIN     1      // GPB 1
#define NIM_I2C_DATPIN      3      // GPB 3
#endif  // #if (DECODER_SYSTEM == DMP952B_EVAL)

// LLY2.60, add for DVD909P-A 128 pin system board
#if (DECODER_SYSTEM == DVD909P_A_EVAL)
#define NIM_I2C_GRP         GPIO_B
#define NIM_I2C_SCLKPIN     1      // GPB 1
#define NIM_I2C_DATPIN      3      // GPB 3
#endif  // #if (DECODER_SYSTEM == DVD909P_A_EVAL)

// LLY2.60, add for DVD909P-B 128 pin system board
#if (DECODER_SYSTEM == DVD909P_B_EVAL)
#define NIM_I2C_GRP         GPIO_B
#define NIM_I2C_SCLKPIN     1      // GPB 1
#define NIM_I2C_DATPIN      3      // GPB 3
#endif  // #if (DECODER_SYSTEM == DVD909P_B_EVAL)

// LLY2.60, add for CT956A system
#if (DECODER_SYSTEM == DMP956A_EVAL)
#define NIM_I2C_GRP         GPIO_B
#define NIM_I2C_SCLKPIN     1      // GPB 1
#define NIM_I2C_DATPIN      3      // GPB 3
#endif  // #if (DECODER_SYSTEM == DMP956A_EVAL)

///////////////////////////////////////////////////////////////////////////////
// [11] NIM power control pin
#if (DECODER_SYSTEM == DMP956A_EVAL)
#define NIM_POWER_GRP           GPIO_E
#define NIM_POWER_PIN           5 // GPE 5
#endif

#ifdef NIM_POWER_PIN
#define SUPPORT_CTRL_NIM_POWER
#endif

////////////////////////////////////////////////////////////////
// [12] Top/ OpenClose control pin -- LLY2.16
#define TOP_OPENCLOSE_GRP   GPIO_A
#define PIN_TOP_OPENCLOSE   16      // GPA 16


///////////////////////////////////////////////////////////////
// [13] Slot-in control pin -- LLY2.16
#define SLOT_IN_GRP         GPIO_EXP    // Expand GPIO 14
#define PIN_SLOT_IN         14


//////////////////////////////////////////////////////////////
// [14] A/V control pin -- LLY2.32
#define AV_IN_GRP           GPIO_EXP
#define AV_IN_PIN           12


//Aron20060110
//  *********************************************************************  //
//          RTC Pin Usage Define Area
//  *********************************************************************  //
#if (DECODER_SYSTEM == DVD909R_EVAL)
#define GRP_RTCRST      GPIO_A
#define PIN_RTCRST      30

#define GRP_RTCCLK      GPIO_A
#define PIN_RTCCLK      21

#define GRP_RTCSDA      GPIO_A
#define PIN_RTCSDA      20
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)

#if (DECODER_SYSTEM == DMP952A_EVAL)
#define GRP_RTCRST      GPIO_E
#define PIN_RTCRST      0

#define GRP_RTCCLK      GPIO_B
#define PIN_RTCCLK      1

#define GRP_RTCSDA      GPIO_B
#define PIN_RTCSDA      3
#endif  // #if (DECODER_SYSTEM == DMP952A_EVAL)

#if (DECODER_SYSTEM == DMP952B_EVAL) //SYuan 2.88
#define GRP_RTCRST      GPIO_E
#define PIN_RTCRST      0

#define GRP_RTCCLK      GPIO_B
#define PIN_RTCCLK      1

#define GRP_RTCSDA      GPIO_B
#define PIN_RTCSDA      3
#endif  // #if (DECODER_SYSTEM == DMP952B_EVAL)

// LLY2.60, add for CT956A system
#if (DECODER_SYSTEM == DMP956A_EVAL)
#define GRP_RTCRST      GPIO_A      // GPA 20
#define PIN_RTCRST      20

#define GRP_RTCCLK      GPIO_B      // GPB 1
#define PIN_RTCCLK      1

#define GRP_RTCSDA      GPIO_B      // GPB 3
#define PIN_RTCSDA      3
#endif  // #if (DECODER_SYSTEM == DMP956A_EVAL)


// LLY2.60, add for CT955A system
#if (DECODER_SYSTEM == DMP955A_EVAL)
#define GRP_RTCRST      GPIO_A      // GPA 20
#define PIN_RTCRST      20

#define GRP_RTCCLK      GPIO_B      // GPB 1
#define PIN_RTCCLK      1

#define GRP_RTCSDA      GPIO_B      // GPB 3
#define PIN_RTCSDA      3
#endif  // #if (DECODER_SYSTEM == DMP955A_EVAL)


// LLY2.71, porting CoCo's code to add photo Rotate detection pin
//  *********************************************************************
//          Photo Rotate Detection Pin
//  *********************************************************************
#if (DECODER_SYSTEM == DMP952A_EVAL)
//++CoCo2.71, for Photo Rotate detection
#define GRP_AUTOROTATE  GPIO_E
#define PIN_AUTOROTATE  0
#endif  // #if (DECODER_SYSTEM == DMP952A_EVAL)

#if (DECODER_SYSTEM == DMP952B_EVAL) //Syuan 2.88
//++CoCo2.71, for Photo Rotate detection
#define GRP_AUTOROTATE  GPIO_E
#define PIN_AUTOROTATE  0
#endif  // #if (DECODER_SYSTEM == DMP952B_EVAL)

#if (DECODER_SYSTEM == DMP956A_EVAL)
//++CoCo2.71, for Photo Rotate detection
#define GRP_AUTOROTATE  GPIO_E
#define PIN_AUTOROTATE  3
#endif  // #if (DECODER_SYSTEM == DMP956A_EVAL)

#if (DECODER_SYSTEM == DMP955A_EVAL)
//++CoCo2.71, for Photo Rotate detection
#define GRP_AUTOROTATE  GPIO_E
#define PIN_AUTOROTATE  3
#endif  // #if (DECODER_SYSTEM == DMP955A_EVAL)

//  *********************************************************************  //
//          Radio Radio tuner Usage Define Area
//  *********************************************************************  //
//winnie 2.71, add for radio tuner
#ifdef SUPPORT_RADIO_TUNER
#if(DECODER_SYSTEM==DMP952A_EVAL)
#define GRP_RADIOSCK GPIO_B   //radio IIC sck
#define PIN_RADIOSCK 1
#define GRP_RADIOSDA GPIO_B   //radio IIC sda
#define PIN_RADIOSDA 3
#define GRP_RADIORST GPIO_C   // radio reset pin
#define PIN_RADIORST 0
#define GRP_RADIOSEN GPIO_G   //radio  SEN pin
#define PIN_RADIOSEN 1
#define GRP_RADIOGPIO2 GPIO_G //radio gpio2
#define PIN_RADIOGPIO2 0
#define GRP_RADIOGPIO3 GPIO_G //radio gpio3
#define PIN_RADIOGPIO3 2
#endif

//SYuan 288 , add new IC paltflorm for CT952B_EVAL
#if(DECODER_SYSTEM==DMP952B_EVAL)
#define GRP_RADIOSCK GPIO_B   //radio IIC sck
#define PIN_RADIOSCK 1
#define GRP_RADIOSDA GPIO_B   //radio IIC sda
#define PIN_RADIOSDA 3
#define GRP_RADIORST GPIO_C   // radio reset pin
#define PIN_RADIORST 0
#define GRP_RADIOSEN GPIO_G   //radio  SEN pin
#define PIN_RADIOSEN 1
#define GRP_RADIOGPIO2 GPIO_G //radio gpio2
#define PIN_RADIOGPIO2 0
#define GRP_RADIOGPIO3 GPIO_G //radio gpio3
#define PIN_RADIOGPIO3 2
#endif

#if(DECODER_SYSTEM== DMP956A_EVAL)
#define GRP_RADIOSCK GPIO_B     //radio IIC sck
#define PIN_RADIOSCK 1
#define GRP_RADIOSDA GPIO_B     //radio IIC sda
#define PIN_RADIOSDA 3
#define GRP_RADIORST GPIO_A     // radio reset pin
#define PIN_RADIORST 22
#define GRP_RADIOSEN GPIO_B     //radio  SEN pin
#define PIN_RADIOSEN 2
#define GRP_RADIOGPIO2 GPIO_B   //radio gpio2
#define PIN_RADIOGPIO2 5
#define GRP_RADIOGPIO3 GPIO_B   //radio gpio3
#define PIN_RADIOGPIO3 4
#endif

#endif

#if(DECODER_SYSTEM==DMP952A_EVAL)
#define TFT_BACKLIGHT_GRP   GPIO_G
#define PIN_TFT_BACKLIGHT   2    // GPA 16
#endif
//  *********************************************************************  //
//           Shared Pin Usage Define Area                                  //
//  *********************************************************************  //
// If define it, GPA[3:2] will be used for GPIO port -- 128 pin
// Otherwise, it will be used for DSU1/ UART1
//#define GPA_2_3_AS_GPIO

// If define it, GPA[13:12] will be used for GPIO port -- 128 pin
// Otherwise, it will be used for UART2
//#define GPA_12_13_AS_GPIO

// If define it, GPB[11] will be used for GPIO port -- 128 pin
// Otherwise, it will be used for expand GPIO KeyDetect1
//#define GPB_11_AS_GPIO

// If define it, GPB[10] will be used for GPIO port -- 128 pin
// Otherwise, it will be used fro expand GPIO KeyDetect0
//#define GPB_10_AS_GPIO

// If define it, GPB[1] will be used for GPIO port -- 128 pin
// Otherwise, it will be used for expand GPIO Latch D0
//#define GPB_1_AS_GPIO

// If define it, GPB[14] will be used for GPIO port -- 128 pin
// Otherwise, it will be used for expand GPIO Latch D1
//#define GPB_14_AS_GPIO

// If define it, GPA[28, 29, 30, 31]
// Otherwise, it will be used for PCMSD0/1/2 (5.1 channel audio output)
//#define GPA_28_29_30_31_AS_GPIO


//  *********************************************************************  //
//           DSU1/ UART1/ UART2 Configure Area                             //
//  *********************************************************************  //
// If define it, it will enable DSU1
//#define ENABLE_DSU1

// If define it, serial debug will use UART1
//#define SD_FROM_UART1

// If define it, serial debug will use UART2
//#define SD_FROM_UART2

// LLY2.50, add the choice for DSU1/ UART1/ UART2 source for CT909P IC
// Since, there are three path for it, NIM/ Card Reader/ SPI Flash
// [0] Not selection any path for using
#define     NOT_USED            0x0

// [1] NIM path
//     DSU1_RX,TX/ GPG[2,3]/ NIMCLK,NIMD/ UART2_RX,TX
//     UART1_RX,TX/ GPG[0,1]/ NIMSYN,NIMVLD/ TRAYPWM,SMBPWM
#define     NIM_PATH            0x1

// [2] Card Reader Path
//     DSU1_TX/ GPC[6]/ SDCLK/ VPSCL
//     DSU1_RX/ GPC[8]/ DAT0/ UART1_RX/ VPIICADR
//     UAR1_TX/ GPC[7]/ SDCMD/ VPSDA
#define     CARD_READER_PATH    0x2

// [3] SPI Flash path
//     DSU1_RX,TX/ GPA[2,3]/ SF_DIO,SF_DO/ UART2_RX,TX
//     UART1_RX,TX/ GPA[0,1]/ SF_CSN,SF_CLK/ DSU2_RX,TX
#define     SPI_PATH            0x3

// [4] 2nd Expand GPIO path
//     GPC[5]/ KEY_DET1/ UART2_RX
//     GPC[4]/ D_LATCH1/ PWE/ UART2_TX
#define     EXPAND_GPIO_PATH    0x4

// SYuan2.77-909P: add three define for 909G.
// [5] IO/MUTE path
//    GPA[4]/ DSU1_TX, UART1_TX, UART2_TX, DSU2_TX
#define     IO_MUTE_PATH              0x5

// [6] VFD path
//    GPA[9]/VFD_SCK/ DSU1_TX, UART1_TX
//    GPA[11]/VFD_DAT/ DSU1_RX, UART1_RX
#define     VFD_PATH                   0x6

// [7] VFD_CS path
// GPA[10] /VFD_CS/ UART1_TX
#define     VFD_CS_PATH                0x7

// [8] SCART path
//    GPA[6]/ DSU1_RX
#define     SCART_PATH                 0x8

//  *********************************************************************  //
//           Expand GPIO Usage Configure Area                              //
//  Notice: detect0/1, latch0/1 feature will use negative -- LLY2.60       //
//  *********************************************************************  //
// If define it, expand GPIO function can be work
//#define ENABLE_EXPAND_GPIO

// If define it, expand GPIO key detect 0 will be disabled
//#define NO_KEY_DETECT_0

// If define it, expand GPIO key detect 1 will be disabled
//#define NO_KEY_DETECT_1

// If define it, expand GPIO latch D0 will be disabled
//#define NO_LATCH_D0

// If define it, expand GPIO latch D1 will be disabled
//#define NO_LATCH_D1

// If define it, expand GPIO flow will go S/W path
//#define SW_EXPAND_GPIO

// ********************************************************************** //
// ******   Base on platform to choose the default configuration    ***** //
// ********************************************************************** //
// LLY2.60, declare the necessary pin for CT909P-A system
// [1] DSU1 from CARD_READER_PATH
// [2] UART2 from KeyDetect1/ D_Latch1 path
// [3] Enable S/W expand GPIO w/ ACTIVE HIGH + disable KeyDetect1/ D_Latch1
#if (DECODER_SYSTEM == DVD909P_A_EVAL)
// DSU1 from Card Reader path + disable UART1 case
/*
#define ENABLE_DSU1
#undef  SD_FROM_UART1
*/

// Disable DSU1 + UART1 from Card Reader path case
/*
#undef  ENABLE_DSU1
#define SD_FROM_UART1
*/

// Always disable UART2 since key detect 1/ d_latch 1 are necessary for servo
//#undef  SD_FROM_UART2

// Enable DSU1 from card reader + UART2 from key detect1/ d_latch1 case
#define ENABLE_DSU1
#define SD_FROM_UART2
#define NO_KEY_DETECT_1
#define NO_LATCH_D1

// Enable S/W expand GPIO
#undef  ENABLE_EXPAND_GPIO
#define SW_EXPAND_GPIO
#ifdef  ACTIVE_LOW
#undef  ACTIVE_LOW
#endif  // #ifdef ACTIVE_LOW

#ifdef  ENABLE_DSU1
//#define DSU1_SOURCE     CARD_READER_PATH
#define DSU1_TX_SOURCE CARD_READER_PATH 
#define DSU1_RX_SOURCE CARD_READER_PATH
#endif  // #ifdef ENABLE_DSU1

#ifdef  SD_FROM_UART1
//#define UART1_SOURCE    CARD_READER_PATH
#define UART1_TX_SOURCE CARD_READER_PATH  
#define UART1_RX_SOURCE CARD_READER_PATH 
#endif  // #ifdef SD_FROM_UART1

#ifdef  SD_FROM_UART2
//#define UART2_SOURCE    EXPAND_GPIO_PATH
#define UART2_TX_SOURCE EXPAND_GPIO_PATH  
#define UART2_RX_SOURCE EXPAND_GPIO_PATH 
#endif  // #ifdef SD_FROM_UART2

#endif  // #if (DECODER_SYSTEM == DVD909P_A_EVAL)

// LLY2.60, declare the necessary pin for CT909P-A system
// [1] DSU1 from CARD_READER_PATH
// [2] UART2 from KeyDetect1/ D_Latch1 path
// [3] Enable S/W expand GPIO w/ ACTIVE HIGH + disable KeyDetect1/ D_Latch1
#if (DECODER_SYSTEM == DVD909P_B_EVAL)
// DSU1 from Card Reader path + disable UART1 case
/*
#define ENABLE_DSU1
#undef  SD_FROM_UART1
*/

// Disable DSU1 + UART1 from Card Reader path case
/*
#undef  ENABLE_DSU1
#define SD_FROM_UART1
*/

// Always disable UART2 since key detect 1/ d_latch 1 are necessary for servo
//#undef  SD_FROM_UART2

// Enable DSU1 from card reader + UART2 from key detect1/ d_latch1 case
#define ENABLE_DSU1
#define SD_FROM_UART2
#define NO_KEY_DETECT_1
#define NO_LATCH_D1

// Enable S/W expand GPIO
#undef  ENABLE_EXPAND_GPIO
#define SW_EXPAND_GPIO
#ifdef  ACTIVE_LOW
#undef  ACTIVE_LOW
#endif  // #ifdef ACTIVE_LOW

#ifdef  ENABLE_DSU1
//#define DSU1_SOURCE     CARD_READER_PATH
#define DSU1_TX_SOURCE CARD_READER_PATH  
#define DSU1_RX_SOURCE CARD_READER_PATH
#endif  // #ifdef ENABLE_DSU1

#ifdef  SD_FROM_UART1
//#define UART1_SOURCE    CARD_READER_PATH
#define UART1_TX_SOURCE CARD_READER_PATH  
#define UART1_RX_SOURCE CARD_READER_PATH 
#endif  // #ifdef SD_FROM_UART1

#ifdef  SD_FROM_UART2
//#define UART2_SOURCE    EXPAND_GPIO_PATH
#define UART2_TX_SOURCE EXPAND_GPIO_PATH  
#define UART2_RX_SOURCE EXPAND_GPIO_PATH 
#endif  // #ifdef SD_FROM_UART1

#endif  // #if (DECODER_SYSTEM == DVD909P_B_EVAL)

// LLY2.60, declare the necessary pin for CT956A system (need NIM and Card Reader)
// [1] Disable DSU1
// [2] Disable UART1
// [3] Disable H/W and S/W expand GPIO
#if (DECODER_SYSTEM == DMP956A_EVAL)
// Default solution must disable DSU1/ UART1 since NIM/ Card Reader all supported -- LLY2.61
#undef  ENABLE_DSU1
#undef  SD_FROM_UART1

// Following is only for debug mode
// [Case 1] Disable Card Reader, let debug go Card reader path
// Notice: DSU1/ UART1 from card reader path will shared same pin
//         So, DSU1/ UART1 only can should one case exist
#if 0
// [Case 1-1] open DSU1 from card, and disable UART1
#define ENABLE_DSU1
#undef  SD_FROM_UART1

#ifdef  ENABLE_DSU1
//#define DSU1_SOURCE     CARD_READER_PATH
#define DSU1_TX_SOURCE CARD_READER_PATH 
#define DSU1_RX_SOURCE CARD_READER_PATH
#endif  // #ifdef ENABLE_DSU1
#endif  // #if 0

#if 0
// [Case 1-2] open UART1 from card, and disable DSU1
#undef  ENABLE_DSU1
#define SD_FROM_UART1

#ifdef  SD_FROM_UART1
//#define UART1_SOURCE    CARD_READER_PATH
#define UART1_TX_SOURCE CARD_READER_PATH 
#define UART1_RX_SOURCE CARD_READER_PATH 
#endif  // #ifdef SD_FROM_UART1
#endif  // #if 0

#if 0
// [Case 2] Disable NIM, let debug go NIM path
#define ENABLE_DSU1
#define SD_FROM_UART1

#ifdef  ENABLE_DSU1
//#define DSU1_SOURCE     NIM_PATH
#define DSU1_TX_SOURCE NIM_PATH 
#define DSU1_RX_SOURCE NIM_PATH
#endif  // #ifdef ENABLE_DSU1

#ifdef  SD_FROM_UART1
//#define UART1_SOURCE    NIM_PATH
#define UART1_TX_SOURCE NIM_PATH 
#define UART1_RX_SOURCE NIM_PATH 
#endif  // #ifdef SD_FROM_UART1
#endif  // #if 0

#undef  SD_FROM_UART2
#undef  ENABLE_EXPAND_GPIO
#undef  SW_EXPAND_GPIO

#endif  // #if (DECODER_SYSTEM == DMP956A_EVAL)

// LLY2.60, declare the necessary pin for CT955A system (no NIM feature)
// [1] DSU1 from NIM path
// [2] UART1 from  NIM path
// [3] Disable H/W and S/W expand GPIO
#if (DECODER_SYSTEM == DMP955A_EVAL)
#undef  ENABLE_DSU1
#undef  SD_FROM_UART1
#if 0   //mantis #3168 bebug.
#define ENABLE_DSU1
#define SD_FROM_UART1
#undef  SD_FROM_UART2
#undef  ENABLE_EXPAND_GPIO
#undef  SW_EXPAND_GPIO

#ifdef  SD_FROM_UART1
#define UART1_TX_SOURCE NIM_PATH 
#define UART1_RX_SOURCE NIM_PATH  
#endif //#ifdef  SD_FROM_UART1

#ifdef  ENABLE_DSU1
#define DSU1_TX_SOURCE NIM_PATH 
#define DSU1_RX_SOURCE NIM_PATH 
#endif //#ifdef  ENABLE_DSU1
#endif
#endif  // #if (DECODER_SYSTEM == DMP955A_EVAL)



// LLY2.56, declare the necessary pin for CT909G system
// [1] Enable DSU1/ UART1
// [2] Disable UART2
// [3] Disable H/W and S/W expand GPIO
#if (DECODER_SYSTEM == DVD909G_EVAL)
#define ENABLE_DSU1
#define SD_FROM_UART1

#ifdef  SD_FROM_UART1
#define UART1_TX_SOURCE VFD_CS_PATH  
#define UART1_RX_SOURCE SCART_PATH  
#endif //#ifdef  SD_FROM_UART1

#ifdef  ENABLE_DSU1
#define DSU1_TX_SOURCE VFD_PATH 
#define DSU1_RX_SOURCE VFD_PATH 
#endif //#ifdef  ENABLE_DSU1
#undef  SD_FROM_UART2
#undef  ENABLE_EXPAND_GPIO
#undef  SW_EXPAND_GPIO
#endif  // #if (DECODER_SYSTEM == DVD909G_EVAL)

// LLY2.61, re-declare the necessary pin for CT952A system
// Don't support radio tuner, then DSU1/ UART1 can go NIM path.
// [1] DSU1 from NIM path
// [2] UART1 from  NIM path
// [3] Disable H/W and S/W expand GPIO
// Support radio tuner, then all DSU1/ UART1/ UART2 all can't work
#if (DECODER_SYSTEM == DMP952A_EVAL)
#ifdef  SUPPORT_RADIO_TUNER
#undef  ENABLE_DSU1
#undef  SD_FROM_UART1
#undef  SD_FROM_UART2
// Following is only for debug mode
#if 0
// [Case 1] DSU1 on from card reader + UART1 off + UART2 off
#define ENABLE_DSU1
#ifdef  ENABLE_DSU1
//#define DSU1_SOURCE     CARD_READER_PATH
#define DSU1_TX_SOURCE CARD_READER_PATH 
#define DSU1_RX_SOURCE CARD_READER_PATH 
#endif //#ifdef  ENABLE_DSU1
#undef  SD_FROM_UART1
#undef  SD_FROM_UART2
#endif
#if 0
// [Case 2] DSU1 off + UART1 on from card reader + UART2 off
#undef  ENABLE_DSU1
#define SD_FROM_UART1
#ifdef  ENABLE_DSU1
//#define DSU1_SOURCE     CARD_READER_PATH
#define DSU1_TX_SOURCE CARD_READER_PATH 
#define DSU1_RX_SOURCE CARD_READER_PATH 
#endif  // #ifdef ENABLE_DSU1
#endif
#if 0
// [Case 3] DSU1 on from card reader + UART1_TX on + UART1_RX off + UART2_TX off + UART2_RX on
#define ENABLE_DSU1
#define SD_FROM_UART1
#define SD_FROM_UART2
#ifdef  SD_FROM_UART1
#define UART1_TX_SOURCE CARD_READER_PATH  
#define UART1_RX_SOURCE NOT_USED  
#endif //#ifdef  SD_FROM_UART1

#ifdef  SD_FROM_UART2
#define UART2_TX_SOURCE NOT_USED  
#define UART2_RX_SOURCE EXPAND_GPIO_PATH  
#endif //#ifdef  SD_FROM_UART2

#ifdef  ENABLE_DSU1
#define DSU1_TX_SOURCE CARD_READER_PATH 
#define DSU1_RX_SOURCE CARD_READER_PATH 
#endif //#ifdef  ENABLE_DSU1
#endif //#if 1 [case 3]
#else   // #ifdef SUPPORT_RADIO_TUNER

#if 1   //DSU NIM_PATH , UART NIM_PATH
#undef ENABLE_DSU1
#undef SD_FROM_UART1
#undef  SD_FROM_UART2
#ifdef  ENABLE_DSU1
#define DSU1_TX_SOURCE   NIM_PATH 
#define DSU1_RX_SOURCE   NIM_PATH 
#endif  // #ifdef ENABLE_DSU1

#ifdef  SD_FROM_UART1
#define UART1_TX_SOURCE  NIM_PATH  
#define UART1_RX_SOURCE  NIM_PATH  
#endif  // #ifdef SD_FROM_UART1

#else  //DSU CARD_READER , UART1-TX CARD_READER_PATH, UART2-RX EXPAND_GPIO_PATH
#define ENABLE_DSU1
#define SD_FROM_UART1
#define SD_FROM_UART2

#ifdef  SD_FROM_UART1
#define UART1_TX_SOURCE CARD_READER_PATH  
#define UART1_RX_SOURCE NOT_USED  
#endif //#ifdef  SD_FROM_UART1
#ifdef  SD_FROM_UART2
#define UART2_TX_SOURCE NOT_USED  
#define UART2_RX_SOURCE EXPAND_GPIO_PATH  
#endif //#ifdef  SD_FROM_UART2
#ifdef  ENABLE_DSU1
#define DSU1_TX_SOURCE CARD_READER_PATH 
#define DSU1_RX_SOURCE CARD_READER_PATH 
#endif //#ifdef  ENABLE_DSU1
#endif //#if 1   //DSU NIM_PATH , UART NIM_PATH

#endif  // #ifdef SUPPORT_RADIO_TUNER

#undef  ENABLE_EXPAND_GPIO
#undef  SW_EXPAND_GPIO

#endif  // #if (DECODER_SYSTEM == DMP952A_EVAL)

// SYuan 288, re-declare the necessary pin for CT952B system
// [1] DSU1 from CARD_READER_PATH
// [2] UART1 -TX from  CARD_READER_PATH
// [3] UART2 -RX from  NIM_PATH
// 
#if (DECODER_SYSTEM == DMP952B_EVAL)
#undef  ENABLE_DSU1
#undef  SD_FROM_UART1
#undef  SD_FROM_UART2
// Following is only for debug mode
#if 0  //[Case 0] DSU1 on from card reader + UART1-TX on from card reader + UART2-RX on from nim path  
#define ENABLE_DSU1
#define SD_FROM_UART1
#define SD_FROM_UART2

#ifdef  ENABLE_DSU1
#define DSU1_TX_SOURCE   CARD_READER_PATH 
#define DSU1_RX_SOURCE   CARD_READER_PATH 
#endif  // #ifdef ENABLE_DSU1

#ifdef  SD_FROM_UART1
#define UART1_TX_SOURCE  CARD_READER_PATH  
#define UART1_RX_SOURCE  NOT_USED 
#endif  // #ifdef SD_FROM_UART1

#ifdef  SD_FROM_UART2
#define UART2_TX_SOURCE  NOT_USED  
#define UART2_RX_SOURCE  NIM_PATH 
#endif  // #ifdef SD_FROM_UART1
#endif //#if 1 
#endif  // #if (DECODER_SYSTEM == DMP952B_EVAL)

// LLY2.50, declare the necessary pin for CT909P system
// [1] DSU1 from NIM path
// [2] UART1 from NIM path
// [3] Disable H/W and S/W expand GPIO
#if (DECODER_SYSTEM == DVD956A_DEV)
#define ENABLE_DSU1
#define SD_FROM_UART1
#undef  SD_FROM_UART2
#undef  ENABLE_EXPAND_GPIO
#undef  SW_EXPAND_GPIO

// LLY2.51, porting Chuan's code let DSU/ UART1 path all from NIM
#ifdef  SD_FROM_UART1
#define UART1_TX_SOURCE NIM_PATH  
#define UART1_RX_SOURCE NIM_PATH  
#endif //#ifdef  SD_FROM_UART1

#ifdef  ENABLE_DSU1
#define DSU1_TX_SOURCE NIM_PATH 
#define DSU1_RX_SOURCE NIM_PATH 
#endif //#ifdef  ENABLE_DSU1
#endif  // #if (DECODER_SYSTEM == DVD956A_DEV)

// If define it, UART1 will shared w/ DSU1 port, ie. GPA[3:2] -- 128 pin
// Otherwise, they will use differnt port, ie. GPA[9:8] -- 216 pin
// LLY2.50, CT909P DSU1/ UART1 will not share the same port
#ifdef CT909R_IC_SYSTEM
#if (DECODER_SYSTEM != DVD909S_EVAL)
#define DSU1_UART1_SAME_PORT
#endif  // #if (DECODER_SYSTEM != DVD909S_EVAL)
#endif  // #ifdef CT909R_IC_SYSTEM

// 909S_EVAL, 216 pin, enable DSU1/ UART1, not support expand GPIO
#if (DECODER_SYSTEM == DVD909S_EVAL)
#define ENABLE_DSU1
#define SD_FROM_UART1

#undef  SD_FROM_UART2
#undef  ENABLE_EXPAND_GPIO
#endif  // #if (DECODER_SYSTEM != DVD909S_EVAL)

// 909R_EVAL, 128 pin, DSU1/ UART2 can exist since no used for GPA[13:12], GPA[3:2]
// Enable expand GPIO 3 port (latch D1, key detect 0, key detect 1)
#if (DECODER_SYSTEM == DVD909R_EVAL)
// DSU1/ UART1 chose one to exist, since no used for GPA[3:2]
#undef  GPA_2_3_AS_GPIO
#undef  SD_FROM_UART1
#define ENABLE_DSU1
// UART2 can exist, since no used for GPA[13:12]
#undef  GPA_12_13_AS_GPIO
#define SD_FROM_UART2

// Don't support latch D0, since it used as GPIO
#define NO_LATCH_D0
#define GPB_1_AS_GPIO

// Using H/W expand GPIO, include key detect 0/1, latch D1
//#define ENABLE_EXPAND_GPIO
//#define NO_LATCH_D0

// S/W expand GPIO
// LLY2.18, only enable GPA[28, 29, 30, 31] as GPIO while enable CCIR656 mode
// Since, Key detect 0/1 control pin will move to GPA[28, 29]
#ifdef  SUPPORT_CCIR656
#define SW_EXPAND_GPIO
#define GPA_28_29_30_31_AS_GPIO
#else   // #ifdef SUPPORT_CCIR656
#ifndef CT951_PLATFORM //CoCo2.38
#define SW_EXPAND_GPIO
#endif // #ifndef CT951_PLATFORM
#endif  // #ifdef SUPPORT_CCIR656
#endif  // #if (DECODER_SYSTEM == DVD909R_EVAL)

// 909R_HFY, 128 pin, DSU1/ UART1/ UART2 can't used since all used for GPA[13:12], GPA[3:2]
// Expand GPIO must be disable, since all used for GPB[14, 11:10, 1]
#if (DECODER_SYSTEM == DVD909R_HFY)
// Must diable DSU1/ UART1, since all used as GPIO
// GPA[2]: MIC_MUTE; GPA[3] reserved
#undef  ENABLE_DSU1
#undef  SD_FROM_UART1
#define GPA_2_3_AS_GPIO

// Must disable UART2, since all used as GPIO
// GPA[12]: I2C_SDAT; GPA[13]: IO_MUTE
#undef  SD_FROM_UART2
#define GPA_12_13_AS_GPIO

// Don't support expand GPIO (include H/W or S/W), since all used for GPIO
// Since, GPB[1]: TRAYOPEN; GPB[10]: TRAYIN; GPB[11]: TRAYOUT; GPB[14]: SLHOME
#undef  ENABLE_EXPAND_GPIO
#undef  SW_EXPAND_GPIO
#define GPB_14_AS_GPIO
#define GPB_1_AS_GPIO
#define GPB_10_AS_GPIO
#define GPB_11_AS_GPIO
#endif  // #if (DECODER_SYSTEM == DVD909R_HFY)

// LLY2.16, define all related pin for CT219_HCN system
// 128 pin, DSU1/ UART1 can choose one to exist
// UART2 can't used since all used for GPA[13:12]
// Must use S/W expand GPIO and disable H/W Expand GPIO mechanism,
// since it will used for video pixel data output pin
#if (DECODER_SYSTEM == STB219_HCN)
// DSU1/ UART1 can choose one to exist
#undef  ENABLE_DSU1
#define SD_FROM_UART1

// Must disable UART2
// Since, GPA[13] for IO_MUTE; GPA[12]=USB_POWER, same as UART2 port
#undef  SD_FROM_UART2
#define GPA_12_13_AS_GPIO

// GPB[1] as MTRSTBY, so can't use for H/W expand GPIO's latch D0
#define GPB_1_AS_GPIO
#define NO_LATCH_D0

// Using use S/W expand GPIO usage, since CCIR656
#undef  ENABLE_EXPAND_GPIO
#define SW_EXPAND_GPIO
#define GPA_28_29_30_31_AS_GPIO
#endif  // #if (DECODER_SYSTEM == DVD909R_HCN)


// LLY2.16, define all related pin for CT219_YHI system
// 128 pin, DSU1/ UART1 can choose one to exist
// UART2 can't used since all used for GPA[13:12]
// Must use S/W expand GPIO and disable H/W Expand GPIO mechanism,
// since it will used for video pixel data output pin (CCIR656)
#if (DECODER_SYSTEM == STB219_YHI)
// DSU1/ UART1 can choose one to exist
#undef  ENABLE_DSU1
#define SD_FROM_UART1

// Must disable UART2
// Since, GPA[13] for IO_MUTE; GPA[12]=USB_POWER, same as UART2 port
#undef  SD_FROM_UART2
#define GPA_12_13_AS_GPIO

// GPB[1] as MTRSTBY, so can't use for H/W expand GPIO's latch D0
#define GPB_1_AS_GPIO
#define NO_LATCH_D0

// Using use S/W expand GPIO usage, since CCIR656
#undef  ENABLE_EXPAND_GPIO
#define SW_EXPAND_GPIO
#define GPA_28_29_30_31_AS_GPIO
#endif  // #if (DECODER_SYSTEM == DVD909R_YHI)


// for #define DSU1_UART1_SAME_PORT 
// Enable DSU/UART, Please #define UART/DSU != NOT_USED.
#ifdef DSU1_UART1_SAME_PORT
#ifdef  SD_FROM_UART1
#define UART1_TX_SOURCE NIM_PATH 
#define UART1_RX_SOURCE NIM_PATH  
#endif //#ifdef  SD_FROM_UART1

#ifdef  SD_FROM_UART2
#define UART2_TX_SOURCE NIM_PATH 
#define UART2_RX_SOURCE NIM_PATH  
#endif //#ifdef  SD_FROM_UART2

#ifdef  ENABLE_DSU1
#define DSU1_TX_SOURCE NIM_PATH 
#define DSU1_RX_SOURCE NIM_PATH 
#endif //#ifdef  ENABLE_DSU1

#ifdef  ENABLE_DSU2
#define DSU2_TX_SOURCE NIM_PATH 
#define DSU2_RX_SOURCE NIM_PATH 
#endif //#ifdef  ENABLE_DSU2
#endif //#ifdef DSU1_UART1_SAME_PORT

// LLY2.51, define not used if desired solution don't selection the DSU/ UART1/ UART2 path
// SYuan2.77-909P: define DSU/UART TX, RX independent.
#ifndef ENABLE_DSU1
#define DSU1_TX_SOURCE NOT_USED
#define DSU1_RX_SOURCE NOT_USED
#endif  // #ifndef ENABLE_DSU1

#ifndef ENABLE_DSU2
#define DSU2_TX_SOURCE NOT_USED
#define DSU2_RX_SOURCE NOT_USED
#endif //#ifndef DSU2_SOURCE

#ifndef SD_FROM_UART1
#define UART1_TX_SOURCE NOT_USED
#define UART1_RX_SOURCE NOT_USED
#endif  // #ifndef UART2_SOURCE

#ifndef SD_FROM_UART2
#define UART2_TX_SOURCE NOT_USED
#define UART2_RX_SOURCE NOT_USED
#endif  // #ifndef UART2_SOURCE

#endif  // __PIO_H__