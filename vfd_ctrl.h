/*****************************************************************************

 vfd_ctrl.h     -       This file is for VFD controller, All varibles and functions 
                                are HW relative. 
                                1. PANEL_Initial() : Initial VFD controller 
                                2. PANEL_KeyScan() : Panel KeyScan function, include SW Key
                                3. _Hardware_CTRL(): Transfer dispalyram.
                                4. _LightVFD_LED() : Light VFD Led.
                                5. _OutByteVFD()   : Write one BYTE to VFD controller.
                                6. _InByteVFD()    : Read one BYTE key data from VFD controller
                                -------------------------------------------------------------
                                Version 2.00
                                -------------------------------------------------------------
                                Copyright (c) 2000, Cheertek Corp. All rights reserved. 

******************************************************************************/
BYTE __icVFDnow=0; //CoCo2.38

#ifndef NO_PANEL
#include "key.h"

//BYTE __icVFDnow=0;

// ************************ For EVAL_P5 ************************

#define GPIO_FPCS          GPIO_GP6
#define GPIO_RED_LED       GPIO_GP3
#define GPIO_GREEN_LED     GPIO_GP2

#define GPIO_FPCS_HI       (GPIO_FPCS | GPIO_PH)
#define GPIO_FPCS_LO       (GPIO_FPCS | GPIO_PL)
#define GPIO_RED_LED_HI    (GPIO_RED_LED | GPIO_PH)
#define GPIO_RED_LED_LO    (GPIO_RED_LED | GPIO_PL)
#define GPIO_GREEN_LED_HI  (GPIO_GREEN_LED | GPIO_PH)
#define GPIO_GREEN_LED_LO  (GPIO_GREEN_LED | GPIO_PL)
#define GPIO_LED_OUTPUT    (0x0001<<GPIO_RED_LED)

// LLY2.15, remove VFD pin define to "pio.h"
#if 0
#define GPIO_VFD_STB     25 
#define GPIO_VFD_SDA     26
#define GPIO_VFD_SCK     27  
#endif  // #if 0

// LLY2.15, using general GPIO group define for VFD_STB usage
#define _VFD_CS_HI        {HAL_WriteGPIO(VFD_STB_GPIO_GRP, GPIO_VFD_STB,1);}
#define _VFD_CS_LO        {HAL_WriteGPIO(VFD_STB_GPIO_GRP, GPIO_VFD_STB,0);}
#define _VFD_SCK_HI       {HAL_WriteGPIO(GPIO_A, GPIO_VFD_SCK,1);}
#define _VFD_SCK_LO       {HAL_WriteGPIO(GPIO_A, GPIO_VFD_SCK,0);}
#define _VFD_SDA_HI       {HAL_WriteGPIO(GPIO_A, GPIO_VFD_SDA,1);}
#define _VFD_SDA_LO       {HAL_WriteGPIO(GPIO_A, GPIO_VFD_SDA,0);}
#define _VFD_SDA_READ     (HAL_ReadGPIO(GPIO_A, GPIO_VFD_SDA)?1:0)

// *************************************************************

extern BYTE _bDisplayRam[VFD_GRID_ARRAY];

#endif   // #ifndef NO_PANEL

#if 0 // LLY2.16, remove to Hio.h
#define GPIO_KS_DATA     18 
#define GPIO_KS_CLK      16 
#define GPIO_KS_DET0     10  
#define GPIO_KS_DET1     11  

#define _KS_CLK_HI        {HAL_WriteGPIO(GPIO_A, GPIO_KS_CLK, 1);} 
#define _KS_CLK_LO        {HAL_WriteGPIO(GPIO_A, GPIO_KS_CLK, 0);}
#define _KS_DAT_HI        {HAL_WriteGPIO(GPIO_A, GPIO_KS_DATA, 1);}
#define _KS_DAT_LO        {HAL_WriteGPIO(GPIO_A, GPIO_KS_DATA, 0);}
#define _KS_DET0_READ     (HAL_ReadGPIO(GPIO_B, GPIO_KS_DET0)?1:0) 
#define _KS_DET1_READ     (HAL_ReadGPIO(GPIO_B, GPIO_KS_DET1)?1:0) 
#endif  // #if 0

