#ifndef __HSYS_H__
#define __HSYS_H__

#ifdef __cplusplus
   extern "C"{
#endif

// LLY2.15, add varible to keep system clock
extern  DWORD   __dwSysClk;
extern  WIN_PARAMETER   MainVideoInfo;

// Micky2.11a
void    HAL_StartSystem(void);
BYTE    HAL_InitPlatForm(void);
void    HAL_PowerControl(BYTE bDevice, BYTE bAction);
void    HAL_IOMute(BYTE bMute); //
// LLY2.04, Get DRAM size
DWORD   HAL_GetDRAMSize(void);
WORD    HAL_CheckSum(DWORD dwStart, DWORD dwEnd);

// LLY2.15, add two APIs for watch dog reated.
void    HAL_WatchDog_Enable(BYTE bEnable);
BYTE    HAL_WatchDog_Status(void);
void    HAL_WatchDog_Reset(void);

// LLY2.61, new add for SCART PWM control
#ifdef  SUPPORT_SCART_IF
#ifdef  SUPPORT_SCART_PWM_CTRL
BYTE    HAL_SCART_PWM_Init(BYTE bPort);
BYTE    HAL_SCART_PWM_Ctrl(BYTE bPort, BYTE bVal);
#endif  // #ifdef SUPPORT_SCART_PWM_CTRL
#endif  // #ifdef  SUPPORT_SCART_IF




// Define the ID for HAL_GetDRAMSize() return value
#define     HAL_DRAM_UNKNOWN    0x50000000
#define     HAL_DRAM_16M        0x40200000
#define     HAL_DRAM_32M        0x40400000
#define     HAL_DRAM_64M        0x40800000
#define     HAL_DRAM_128M       0x41000000
#define     HAL_DRAM_256M       0x42000000
#define     HAL_DRAM_512M       0x44000000

// Define the ID for HAL_PowerControl() 1st parameter
#define HAL_POWER_VDEC      1
#define HAL_POWER_USB       2
#define HAL_POWER_PROM      3
#define HAL_POWER_PLL       4   // LLY2.13, add new case for power down all PLL part
#define HAL_POWER_PROC2     5   // LLY2.36, add new case for power down PROC2
#define HAL_POWER_SERVO     6   // Chuan2.77, add new case for power down Servo part if servo code isn't exist

// Define the ID for HAL_PowerControl() 2nd parameter
#define HAL_POWER_NORMAL    1
#define HAL_POWER_SAVE      0


// LLY2.16, define the ID value for slow down system clock method
#define TUNE_MPLL_VALUE     0
#define USE_EXTERNAL_OSCI   1

// Choose the desired method to slow down system clock
#define SLOW_DOWN_SYSCLK_METHOD     TUNE_MPLL_VALUE


// LLY2.36, re-modify this API since need return value to know the action ok or fail.
//void HAL_ClockSet(BYTE bMode, DWORD dwType);
DWORD   HAL_ClockSet(BYTE bMode, DWORD dwType);
#define     MODE_MPLL                   (0)
#define     MODE_APLL                   (1)
#define     MODE_UPLL                   (2)
#define     MODE_CLKCTL_VIDEO           (3)
#define     MODE_CLKCTL_SERVO           (5)
// LLY2.22, remove following define
// Since, HAL_ClockSet() 2nd parameter will use CPU_XXX directly
#if 0
#define     TYPE_27M                    (0x01)
#define     TYPE_54M                    (0x02)
#define     TYPE_100M                   (0x03)
#define     TYPE_120M                   (0x04)
#define     TYPE_133M                   (0x05)
#define     TYPE_146M                   (0x06)
#define     TYPE_162M                   (0x07)
#endif  // #if 0

#define     TYPE_HDTV                   (0x20)
#define     TYPE_PSCAN                  (0x40)


#define     TYPE_OCLK108M               (0x1)
#define     TYPE_OCLK54M                (0x2)
#define     TYPE_OCLK27M                (0x3)
#ifdef __cplusplus
   }
#endif

#endif  // __HSYS_H__
