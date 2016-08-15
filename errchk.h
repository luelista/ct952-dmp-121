#ifndef __ERRCHK_H__
#define __ERRCHK_H__


//  *********************************************************************  //
//      Add error protection code area for compiler time                   //
//  *********************************************************************  //
// [1] Only can choose one UART for serial debug
// SYuan2.77 : for settting UART1 & UART2 can choose different port while non-CT909R IC.
#ifdef CT909R_IC_SYSTEM
#if defined(SD_FROM_UART1) && defined(SD_FROM_UART2)
#error "SD can't work for two port together !"
#endif  // #if defined(SD_FROM_UART1) && defined(SD_FROM_UART2)
#endif  // #ifdef CT909R_IC_SYSTEM

// [2] DSU1/ UART1 can't exist together while same port
#ifdef DSU1_UART1_SAME_PORT
#if defined(ENABLE_DSU1) && defined(SD_FROM_UART1)
#error "DSU1/ UART1 can't exist together for 128pin system !"
#endif  // #if defined(SD_FROM_UART1) && defined(SD_FROM_UART2)
#endif  // #ifdef DSU1_UART1_SAME_PORT

// [3] Can't use DSU1 or UART1, while GPA[3:2] as GPIO
#ifdef  GPA_2_3_AS_GPIO
#if defined(ENABLE_DSU1) || defined(SD_FROM_UART1)
#error "Can't use DSU1/ UART1 since GPA[3:2] as GPIO"
#endif  // #if defined(ENABLE_DSU1) || defined(SD_FROM_UART1)
#endif  // #ifdef GPA_2_3_AS_GPIO

// [4] Can't use UART2, while GPA[13:12] as GPIO
#ifdef  GPA_12_13_AS_GPIO
#if defined(SD_FROM_UART2)
#error "Can't use UART2 since GPA[13:12] as GPIO"
#endif  // #if defined(ENABLE_DSU1) || defined(SD_FROM_UART1)
#endif  // #ifdef GPA_2_3_AS_GPIO

// [5] GPB[10], Key Detect 0 can't exist together
#if defined(GPB_10_AS_GPIO) && !defined(NO_KEY_DETECT_0)
#error "Can't enable GPB[10] and key detect 0 together !"
#endif  // #if defined(GPB_10_AS_GPIO) && !defined(NO_KEY_DETECT_0)

// [6] GPB[11], Key Detect 1 can'w exist together
#if defined(GPB_11_AS_GPIO) && !defined(NO_KEY_DETECT_1)
#error "Can't enable GPB[11] and key detect 1 together !"
#endif  // #if defined(GPB_11_AS_GPIO) && !defined(NO_KEY_DETECT_1)

// [7] GPB[1], Latch D0 can't exist together
#if defined(GPB_1_AS_GPIO) && !defined(NO_LATCH_D0)
#error "Can't enable GPB[1] and latch D0 together !"
#endif  // #if defined(GPB_1_AS_GPIO) && !defined(NO_LATCH_D0)

// [8] GPB[14], Latch D1 can't exist together
#if defined(GPB_14_AS_GPIO) && !defined(NO_LATCH_D1)
#error "Can't enable GPB[14] and latch D1 together !"
#endif  // #if defined(GPB_14_AS_GPIO) && !defined(NO_LATCH_D1)

// [9] H/W or S/W expand GPIO only can choose one -- LLY2.16
#if defined(SW_EXPAND_GPIO) && defined(ENABLE_EXPAND_GPIO)
#error "Can't enable GPB[11] and key detect 1 together !"
#endif  // #if defined(SW_EXPAND_GPIO) && defined(ENABLE_EXPAND_GPIO)

// [10] CCIR656 can't exist w/ Expand GPIO  -- LLY2.16
#if defined(SUPPORT_CCIR656) && defined(ENABLE_EXPAND_GPIO)
#error "CCIR656 Can't exist w/ expand GPIO together !"
#endif  // #if defined(SUPPORT_CCIR656) && defined(ENABLE_EXPAND_GPIO)

// [11] GPA 28/29/30/31 can't work w/ 5.1 channel configure
#if defined(GPA_28_29_30_31_AS_GPIO) && !defined(SUPPORT_2_CHANNEL_ONLY)
#error "PCMSD pin can't work as GPA together !"
#endif  // #if defined(GPA_28_29_30_31_AS_GPIO) && !defined(SUPPORT_2_CHANNEL_ONLY)

// [12] Can't choose any external ADAC if enable "use internal ADAC only" -- LLY2.17
#ifdef  USE_INTERNAL_ADAC_ONLY
#if (AUDIO_DAC != DAC_INTERNAL)
#error "Can't choose external ADAC if use internal ADAC only"
#endif  // #if AUDIO_DAC != DAC_INTERNAL
#endif  // #ifdef USE_INTERNAL_ADAC_ONLY

// [13] Can't enable watch dog while disable error conceal -- LLY2.51
#if !defined(ERROR_CONCEAL) && defined(SUPPORT_WATCH_DOG)
#error "Can't turn-on watch dog while disable error conceal !"
#endif  // #ifndef ERROC_CONCEAL

// [14] SCART feature can't exist w/ external audio DAC for CT909G_IC -- LLY2.61
#ifdef  CT909G_IC_SYSTEM
#if (AUDIO_DAC != DAC_INTERNAL)
#ifdef  SUPPORT_SCART_IF
#error "SCART and external ADAC can't exist together for CT909G !"
#endif  // #ifdef SUPPORT_SCART_IF
#endif  // #if (AUDIO_DAC != DAC_INTERNAL)
#endif  // #ifdef CT909G_IC_SYSTEM

#endif  // #ifndef __ERRCHK_H__
