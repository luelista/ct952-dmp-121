#ifndef __HDEVICE_H__
#define __HDEVICE_H__

#ifdef __cplusplus
   extern "C"{
#endif

//Define Serial flash
#define NOT_SUPPORT_FLASH 0xffffffff
#define ROM_BANK_SIZE 512

DWORD dwROM_RANGE_START;
DWORD dwROM_RANGE_MID ;
DWORD dwROM_RANGE_END;


// Serial Flash related function.
void    HAL_InitSerialFlash(void);
// LLY2.51, porting Wing's code to support SPI Flash write feature.
// LLY2.53, porting Wing's code to support > 256 byte setup info
//void    HAL_WriteSPF_BANK(BYTE bAddr, BYTE* pbVal, BYTE bCnt);
void    HAL_WriteSPF_BANK(WORD wAddr, BYTE* pbVal, WORD wCnt);
BYTE    HAL_WriteSPF(DWORD bAddr, BYTE* pbVal, DWORD dwCnt);
// LLY2.53, porting Wing's code to support > 256 byte setup info
//void HAL_ReadSerialFlash(BYTE bAddr, BYTE* pbVal, BYTE bCnt)
void    HAL_ReadSerialFlash(WORD wAddr, BYTE* pbVal, WORD wCnt);

// EEPROM read/write function
void    HAL_ReadEEPROM(BYTE bAddr, BYTE* pbVal, BYTE bCnt);
void    HAL_WriteEEPROM(BYTE bAddr, BYTE* pbVal, BYTE bCnt);


void    HAL_TVE_Off(void);
void    HAL_TVE_Set(BYTE bColorSpace, BYTE bScanMode, BYTE bTVSystem, BYTE bVideoOutput, int nGainOffset);
DWORD   HAL_SetTVEncoder(BYTE bColorSpace, BYTE bScanMode, BYTE bTVSystem, BYTE bVideoOutput, int nGainOffset);
void    HAL_DisableSVideo(BOOL blDisable);
void    HAL_Force_TVOut_Off(BYTE bFlag);

///// TVE /////
#define PSCAN_OFF   0
#define PSCAN_ON    1

#define TVSYSTEM_NTSC   1
#define TVSYSTEM_PAL    2
#define TVSYSTEM_PALM   3
#define TVSYSTEM_PALN   4

#define VIDEOOUTPUT_AV_SVIDEO   1
#define VIDEOOUTPUT_AV_YUV      2
#define VIDEOOUTPUT_AV_RGB      3
#define VIDEOOUTPUT_YUV_PSCAN   4
#define VIDEOOUTPUT_RGB_PSCAN   5
#define VIDEOOUTPUT_VGA_PSCAN   6

#define HAL_TVE_COLOR_SPACE_MPEG    1
#define HAL_TVE_COLOR_SPACE_JPEG    2

#ifdef __cplusplus
   }
#endif

#endif  // __HDEVICE_H__

