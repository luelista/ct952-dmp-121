#ifndef __HADAC_H__
#define __HADAC_H__

#ifdef __cplusplus
   extern "C"{
#endif
void	HAL_ADAC_Set(BYTE bSampleFreq, BYTE bBitResolution);
#define HAL_DAC_PCM1723         0
#define HAL_DAC_W6630           1
#define HAL_DAC_PCM1716         2
#define HAL_DAC_PCM1720         3
#define HAL_DAC_AV2188          4
#define HAL_DAC_PCM1602         5
#define HAL_DAC_WM8746          6
#define HAL_DAC_PCM1742         7
#define HAL_DAC_W571C161        8

///// Audio Frequency /////
#define AUDIO_FREQ_NONE         0
#define AUDIO_FREQ_BASE         1
#define AUDIO_FREQ_8K           (AUDIO_FREQ_BASE+ 0)
#define AUDIO_FREQ_16K          (AUDIO_FREQ_BASE+ 1)
#define AUDIO_FREQ_32K          (AUDIO_FREQ_BASE+ 2)
#define AUDIO_FREQ_64K          (AUDIO_FREQ_BASE+ 3)
#define AUDIO_FREQ_128K         (AUDIO_FREQ_BASE+ 4)
#define AUDIO_FREQ_11K          (AUDIO_FREQ_BASE+ 5)
#define AUDIO_FREQ_22K          (AUDIO_FREQ_BASE+ 6)
#define AUDIO_FREQ_44K          (AUDIO_FREQ_BASE+ 7)
#define AUDIO_FREQ_88K          (AUDIO_FREQ_BASE+ 8)
#define AUDIO_FREQ_176K         (AUDIO_FREQ_BASE+ 9)
#define AUDIO_FREQ_12K          (AUDIO_FREQ_BASE+ 10)
#define AUDIO_FREQ_24K          (AUDIO_FREQ_BASE+ 11)
#define AUDIO_FREQ_48K          (AUDIO_FREQ_BASE+ 12)
#define AUDIO_FREQ_96K          (AUDIO_FREQ_BASE+ 13)
#define AUDIO_FREQ_192K         (AUDIO_FREQ_BASE+ 14)
#define AUDIO_FREQ_END          AUDIO_FREQ_192K
#define AUDIO_FREQ_NO           (AUDIO_FREQ_END-AUDIO_FREQ_BASE+1)

extern BYTE     __bHALDeEmphasis; //rename __bHAL1_Deemphasis as __bHALDeEmphasis

// LLY2.20, remove it since it was put @ HADAC.C
#if 0
// LLY2.15, add macro to control Audio related pin action.
// [1] ADAC_CS6CH pin High/ Low action
#define ADAC_CS6CH_HI()     {HAL_WriteGPIO(ADAC_CS6CH_GPIO_GRP, PIN_ADAC_CS6CH, 1);}
#define ADAC_CS6CH_LO()     {HAL_WriteGPIO(ADAC_CS6CH_GPIO_GRP, PIN_ADAC_CS6CH, 0);}

// [2] ADAC_CS2CH pin High/ Low action
#define ADAC_CS2CH_HI()     {HAL_WriteGPIO(GPIO_A, PIN_ADAC_CS2CH, 1);}
#define ADAC_CS2CH_LO()     {HAL_WriteGPIO(GPIO_A, PIN_ADAC_CS2CH, 0);}

// [3] ADAC Clock pin High/ Low action
#define ADAC_SCK_HI()       {HAL_WriteGPIO(GPIO_A, PIN_ADAC_SCK, 1);}
#define ADAC_SCK_LO()       {HAL_WriteGPIO(GPIO_A, PIN_ADAC_SCK, 0);}

// [4] ADAC Data pin High/ Low action
#define ADAC_SDA_HI()       {HAL_WriteGPIO(GPIO_A, PIN_ADAC_SDA, 1);}
#define ADAC_SDA_LO()       {HAL_WriteGPIO(GPIO_A, PIN_ADAC_SDA, 0);}
#endif  // #if 0

#ifdef __cplusplus
   }
#endif

#endif  // __HADAC_H__

