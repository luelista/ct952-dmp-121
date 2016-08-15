#ifndef __CHIPS_H__
#define __CHIPS_H__

// ========== Global Variable ========== //
extern BYTE __bVocal;
extern char __cKeyShift;

// ========== ==================== ========== //
// J500KWShih_231, for mpeg audio error concealment
#define CHIPS_MPG_ERR_DISABLE   (0x00)  // Disable all
#define CHIPS_MPG_ERR_HEADER    (0x01)  // [0]: MPEG header error mask
#define CHIPS_MPG_ERR_FRAME     (0x02)  // [1]: MPEG frame size error mask
#define CHIPS_MPG_ERR_BLOCK     (0x04)  // [2]: MPEG block code error mask
#define CHIPS_MPG_ERR_QUANT     (0x08)  // [3]: MPEG quantizaion error mask
#define CHIPS_MPG_ERR_CRC       (0x10)  // [4]: MPEG CRC error mask
#define CHIPS_MPG_ERR_ENABLE    (0xFF)  // Enable all

#define VOLUME_MAX      (0x7FF0)
#define VOLUME_GAP      (0x7FF)
#define VOLUME_MIN      (0x0)
#define VOLUME_DEFAULT  VOLUME_MAX

#define MIC_VOL_MAX     (0x7FF0)
#define MIC_VOL_GAP     (0x7FF)
#define MIC_VOL_MIN     (0x0)
#define MIC_VOL_DEFAULT MIC_VOL_MAX

#define ECHO_LEVEL      (8) // Echo Level is from 0 to ECHO_LEVEL

//////////// following is added by LLY on 12/23
#define CHIPS_LRD       (0) // Audio is Stereo (LR) Dolby Surround Compatible, and Lt/Rt in 2ch
#define CHIPS_C         (1) // Audio is Center
#define CHIPS_LR        (2) // Audio is LR, and Lo/Ro in 2ch
#define CHIPS_LCR       (3) // Audio is LCR
#define CHIPS_LRLS      (4) // Audio is LRLs
#define CHIPS_LCRLS     (5) // Audio is LCRLs
#define CHIPS_LRLSRS    (6) // Audio is LRLsRs
#define CHIPS_LCRLSRS   (7) // Audio is LCRLsRs

// For Vocal Definition
#define VOCAL_OFF       (0)
#define VOCAL_LL        (1)
#define VOCAL_RR        (2)
#define VOCAL_ON        (3)

// define Dual mode
#define DUAL_STEREO     (0)
#define DUAL_LEFTMONO   (1)
#define DUAL_RIGHTMONO  (2)
#define DUAL_MIXEDMONO  (3)

//Kevin1.07, AC3 downmix mode
#define AC3_DOWNMIX_LORO    (0)
#define AC3_DOWNMIX_LTRT    (1)
#define AC3_DOWNMIX_AUTO    (2)
#define AC3_DOWNMIX_DEFAULT AC3_DOWNMIX_LORO

// Initial Types
#define CHIPS_INIT_KEY          (0x001)
#define CHIPS_INIT_SURROUND     (0x002)
#define CHIPS_INIT_ECHO         (0x004)
#define CHIPS_INIT_3DLEVEL      (0x008)
#define CHIPS_INIT_VOL          (0x010)
#define CHIPS_INIT_CHANNELMODE  (0x020)
#define CHIPS_INIT_AC3KOKMODE   (0x040)
#define CHIPS_INIT_DUALMODE     (0x080)
#define CHIPS_INIT_AUDTYPE      (0x100)
#define CHIPS_INIT_VOCAL        (0x200)
#define CHIPS_INIT_ALL          (0X3FF)
//Kevin2.80, add
#define CHIPS_INIT_AMPVOL       (0x400)
#define CHIPS_INIT_BYPASS       (0x000)

// Chips Parameters
#define CHIPS_UP                (1)
#define CHIPS_DOWN              (2)
#define CHIPS_NORMAL            (3)
#define CHIPS_TOGGLE            (4)
#define CHIPS_LEVEL             (5)
#define CHIPS_SUPPORT           (6)
#define CHIPS_ON                (7)
#define CHIPS_OFF               (8)

// default value for the parameters
#define CHIPS_DEFAULT_KEY           (0)
#define CHIPS_DEFAULT_SURROUND      (0)
#define CHIPS_DEFAULT_ECHO          (0)
#define CHIPS_DEFAULT_3DLEVEL       (0)
#define CHIPS_DEFAULT_VOL           VOLUME_DEFAULT
#define CHIPS_DEFAULT_CHANNELMODE   CHIPS_LRD
#define CHIPS_DEFAULT_AC3KOKMODE    VOCAL_ON
#define CHIPS_DEFAULT_DUALMODE      DUAL_STEREO
#define CHIPS_DEFAULT_AUDTYPE       (0)

// definitions for surround
#define CHIPS_SURROUND_NONE     (0)
#define CHIPS_SURROUND_LIVE     (1)
#define CHIPS_SURROUND_THEATER  (2)
// TCC028, add define to match with OSD type
#define CHIPS_SURROUND_STEREO   (3)
#define CHIPS_SURROUND_MOVIE    (4)
#define CHIPS_SURROUND_KARAOKE  (5)

#define CHIPS_SURROUND_TYPE     (2)

// definitions of Surround Value
// TCC032, change type name so that won't conflict with karaoke module
// TCC278, should be hex other than decimal here
//Kevin1.24, avoid CDDA surround noise 
#define SURROUND_TYPE_LIVE      (0x77700L) //0x00000L
#define SURROUND_TYPE_THEATER   (0x66611L) //0x55500L
#define SURROUND_TYPE_STEREO    (0x88800L)
#define SURROUND_TYPE_MOVIE     (0x66600L)
#define SURROUND_TYPE_KARAOKE   (0x33300L)

#define CHIP_BASE           (0)
#define CHIP_AUDIOMUTE      (CHIP_BASE + 2) // Control Audio Mute or NOT
#define CHIP_KEYSHIFT       (CHIP_BASE + 3) // Control KeyShift UP/DOWN/NORMAL
#define CHIP_ENABLEKARAOKE  (CHIP_BASE + 4) // Enable Karaoke in BCA

#define CHIPS_VOLUMECONTROL (0)
#define CHIPS_SOUNDEFFECT   (1)
#define CHIPS_SURROUND      (2)
#define CHIPS_ECHO          (3)
#define CHIPS_KEYCONTROL    (4)
#define CHIPS_OPENAUDIO     (5) //Kevin0.87, add

//Kevin2.81, add
#define AUDIO_AC3   (0)
#define AUDIO_MPG   (1)
#define AUDIO_PCM   (2)
#define AUDIO_DTS   (3)
#define AUDIO_WMA   (5) //Kevin1.08

BYTE CHIPS_OpenAudio(BYTE bParam);
void CHIPS_SoftMute(void);
void CHIPS_ControlSPDIFOutput(void);    
void CHIPS_MICDetect(void); //Software MIC detection
void CHIPS_MICEnable(BYTE bEnable);
void CHIPS_SPDIF_Mute(BYTE bEnable);
void CHIPS_Initial(WORD wInit);
BYTE CHIPS_KeyControl(BYTE bKey);
BYTE CHIPS_Surround(BYTE bSurd);
void CHIPS_ProLogicControl(void);
void CHIPS_ControlHDCD(void);
BYTE CHIPS_Echo(BYTE bEcho);
BYTE CHIPS_MICControl(BYTE bON);
BYTE CHIPS_AC3KokMode(BYTE bMode);
void CHIPS_DownMixMode(BYTE bMode);
// J500KWShih_230, mpeg error concealment
void CHIPS_ControlMPEGEErrorConcealment(BYTE bEnable);

DWORD CHIPS_VolumeControl(BYTE bFunction);
// J500CSC_2.05, for supporting runtime decompressed audio code
DWORD CHIPS_LoadAudioCode(BYTE bAudioType, PDWORD pdwAudioTbl, PBYTE pbDstAddr, PBYTE dwWorkMem);

#ifndef NO_MIC_INPUT 
    void CHIPS_MICVolControl(BYTE bFunction);
#endif

#ifdef DOLBY_CERTIFICATION //kevin0.80
    BYTE CHIPS_Dolby_ProcessKey(void);
#endif

#endif // __CHIPS_H__