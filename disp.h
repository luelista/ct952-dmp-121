#ifndef __DISP_H__
#define __DISP_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

#ifndef AP_FRAMEWORK
#define AVSYNC_ENABLE
#endif

#ifndef     __OSWRAP_H__
#include "oswrap.h"
#endif // __OSWRAP_H__


typedef struct  tagDISP_CTRL
{
    DWORD   dwFY_Addr;
    DWORD   dwFC_Addr;
    DWORD   dwPTS;
    volatile DWORD   dwTR;
    SHORT   sFrame_center_horizontal_offset[3];
    // SHORT   sframe_center_vertical_offset[3];
    WORD    wNumber_of_frame_center_offsets;
    BYTE    bGroup;
    BYTE    bFieldNum;
    BYTE    bTopField;
    BYTE    bSrcFrame;
    BYTE    bDispFrame;
    BYTE    bPrevFrame;
    BYTE    bBFR_Mode;
#ifdef SUPPORT_N_STRIPE
    BYTE    bNStripe_Mode;
#endif
} DISP_CTRL, *PDISP_CTRL;

typedef struct  tagDIGESTVIDEO_INFO
{
    WIN_PARAMETER   DigestVideo;
    DWORD           dwDestAddrY;
    DWORD           dwDestAddrUV;
    DWORD           dwDeFlicker_Addr;
} DIGESTVIDEO_INFO, *PDIGESTVIDEO_INFO;

// copying is permitted with restriction, NO APS
typedef struct  tagCGMSA_DATA
{
    DWORD           dwStatus;    // 0:Unused 1: Ready 2: In Used
    ULONG_UNION     ulunCGMSData[3];
} CGMSA_DATA, *PCGMSA_DATA;
#define     DISP_CGMSA_UNUSED           (0)
#define     DISP_CGMSA_READY            (1)
#define     DISP_CGMSA_INUSED           (2)

typedef struct  tagAPTS_INFO
{
    DWORD   dwPTS;
    DWORD   dwReserved;
    ULONGLONG_UNION    ullAddress;
} APTS_INFO, *PAPTS_INFO;


#define     APTS_NUM                    (16)
#define     APTS_MASK                   (APTS_NUM-1)

#define     DISP_UNKNOWN_NEWSTC         (0x00000)       // F/W issue VideoMaster or AudioMaster to do initial value.

// Wait Previous bitstream Ending
#define     DISP_WAIT_SPU_DONE          (0x00010)       // Wait last SPU Done

// Wait Next bitstream Playing
#define     DISP_WAIT_STC_UPDATE        (0x01000)       // Wait Video/ or Audio Update STC
#define     DISP_VIDEO_STC_UPDATED      (0x01100)       // Update STC by Video
#define     DISP_AUDIO_STC_UPDATED      (0x01200)       // Update STC by Audio
#define     DISP_STC_UPDATED            (0x01300)       // Update STC by Video/ or Audio

// Cancel NewSTC control
#define     DISP_CANCEL_NEWSTC          (0x10000)       // F/W force to cancel NewSTC (dis-continue -> continue)
#define     DISP_STC_UNINITIALED        (0x20000)       // A/V don't update yet!

#define     PTS_OVER                    (6L)
#define     PTS_OVERFLOW                (0x60000L)
#define     PTS_OVERFLOW_MAX            (0x150000L)

#define     NULL_PTS                    (0xFFFFFFFFL)
#define     NULL_TR                     (0xFFFFFFFFL)
#define     NULL_FRAME                  (5)
#define     DISP_FRAMEHALT              (0x10)
                                        
#define     DISP_MAINVIDEO              (0x1)
#define     DISP_PIP                    (0x2)
#define     DISP_OSD                    (0x4)
#define     DISP_SP1                    (0x8)
#define     DISP_SP2                    (0x10)
#define     DISP_SPDEC                  (0x20)
                                        
#define     DISP_MISC                   (0x1)
#define     DISP_TVMODE                 (0x2)
#define     DISP_MAINVIDEOSET           (0x4)
#define     DISP_PIPSET                 (0x8)
#define     DISP_SLIDESHOW              (0x10)
#define     DISP_SETPALETTE             (0x20)
                                        
#define     MODE_COLORBAR               (0x08)
#define     MODE_PAL                    (0x10)
#define     MODE_PAL_RGB                (0x11)
#define     MODE_PAL_YUV                (0x12)
#define     MODE_PAL_VGA                (0x13)
#define     MODE_PAL_NORMAL             (0x14)
#define     MODE_NTSC                   (0x20)
#define     MODE_NTSC_RGB               (0x21)
#define     MODE_NTSC_YUV               (0x22)
#define     MODE_NTSC_VGA               (0x23)
#define     MODE_NTSC_NORMAL            (0x24)
/*
#define     MODE_HDTV                   (0x40)
#define     MODE_HDTV_RGB               (0x41)
#define     MODE_HDTV_YUV               (0x42)
#define     MODE_HDTV_VGA               (0x43)
*/
#define     MODE_TFT                    (0x40)
#define     MODE_TFT_ANALOG             (0x45)
#define     MODE_TFT_DIGITAL            (0x46)
#define     MODE_PSCAN                  (0x80)
#define     MODE_RGB                    (0x1)
#define     MODE_YUV                    (0x2)
#define     MODE_VGA                    (0x3)
#define     MODE_NORMAL                 (0x4)

#define     SLIDESHOW_RESET             (0)
#define     SLIDESHOW_COVERUP           (1)
#define     SLIDESHOW_COVERDOWN         (2)
#define     SLIDESHOW_COVERLEFT         (3|0x80)
#define     SLIDESHOW_COVERRIGHT        (4|0x80)
#define     SLIDESHOW_UNCOVERUP         (5)
#define     SLIDESHOW_UNCOVERDOWN       (6)
#define     SLIDESHOW_UNCOVERLEFT       (7|0x80)
#define     SLIDESHOW_UNCOVERRIGHT      (8|0x80)
#define     SLIDESHOW_WIPEUP            (9)
#define     SLIDESHOW_WIPEDOWN          (10)
#define     SLIDESHOW_WIPELEFT          (11|0x80)
#define     SLIDESHOW_WIPERIGHT         (12|0x80)
#define     SLIDESHOW_PUSHUP            (13)
#define     SLIDESHOW_PUSHDOWN          (14)
#define     SLIDESHOW_PUSHLEFT          (15|0x80)
#define     SLIDESHOW_PUSHRIGHT         (16|0x80)
#define     SLIDESHOW_BOXIN             (20)
#define     SLIDESHOW_BOXOUT            (21)
#define     SLIDESHOW_FADE              (22)
#define     SLIDESHOW_QUERY             (0xFF)
                                        
#define     SLIDESHOW_RET_PREPARE       (0)
#define     SLIDESHOW_RET_UNFINISH      (1)
#define     SLIDESHOW_RET_OK            (2)
#define     SLIDESHOW_RET_UNSUPPORT     (3)
                                        
#define     DISP_ADDCONTRAST            (0)
#define     DISP_SUBCONTRAST            (1)
                                        
#define     DISP_BRIGHT                 (0)
#define     DISP_DARK                   (1)
                                        
#define     DISP_MAIN_BG                (0)
#define     DISP_PIP_BG                 (1)
#define     DISP_PIP_BORDER             (2)
                                        
// OSD EFFECT                           
#define     DISP_OSD1X                  (0)
#define     DISP_OSD2X                  (1)
#define     DISP_OSD8BIT                (0)
#define     DISP_OSD4BIT                (1)
                                        
#define     DISP_FRAME0                 (0)
#define     DISP_FRAME1                 (1)
#define     DISP_FRAME2                 (2)
#define     DISP_FRAME3                 (3)
#define     DISP_NEXTVSYNC              (0xF)
                                        
// The size of NTSC/PAL                 
#define     HSIZE_NTSC_PAL              (720)
#define     HSIZE_NTSC                  (720)
#define     HSIZE_PAL                   (720)
#define     VSIZE_NTSC                  (480)
#define     VSIZE_PAL                   (576)
                                        
#define     HSTART_NTSC                 (132)
#define     HSTART_PAL                  (141)
#define     VSTART_NTSC                 (43)
#define     VSTART_PAL                  (45)

#define     OHT_NTSC                    (858)
#define     OHT_PAL                     (864)
#define     OVT_NTSC                    (525)
#define     OVT_PAL                     (625)
                                        
#define     FRAMERATE_NTSC              (30 * 2)
#define     FRAMERATE_PAL               (25 * 2)
#define     FRAMERATE_TFT_DIGITAL       (37)
                                        
#define     TVSYNC_WIDTH                (20)
#define     TVSYNC_HEIGHT               (10)
#define     TFTSYNC_WIDTH               (1)
#define     TFTSYNC_HEIGHT              (1)
                                        
#define     DISP_VIDEO_ON               (0x1)
#define     DISP_VIDEO_READY            (0x2)

#define     DISP_FORCE_ONEFIELD_FW      (0x1)
#define     DISP_ONEFIELD               (0x2)

#define     DISP_INFO_SETPALETTE_DONE   (0x1)

#define     DISP_UNFREEZE_STC           (0x0)
#define     DISP_FORCE_FREEZE_STC       (0x1)
#define     DISP_WAIT_NEXT_STC          (0x2)

#define     CHECK_CNT                   (2)
#define     INT_LINE16_NO               (2)

#define     DISP_CCO_EN                 (0x0200)
#define     DISP_CCE_EN                 (0x0400)
#define     DISP_WSSO_EN                (0x0800)
#define     DISP_WSSE_EN                (0x1000)

VOID    DISP_EnableDigestCopy(DWORD dwEnable);
VOID    DISP_DelayVSYNC(DWORD dwVSYNC);
VOID    DISP_FrameCopy(PDIGESTVIDEO_INFO DigestVideo_Info, DWORD dwFrameIdx);
VOID    DISP_STCFreeze(DWORD dwSTC, DWORD dwCondition);
VOID    DISP_UpdateSTC( DWORD dwPTS, DWORD dwNotify );
VOID    DISP_UpdateNewSTC(DWORD dwSTC);
VOID    DISP_VBIDataSet(DWORD dwWSS_D_E, DWORD dwWSS_D_O, DWORD dwWSS_CGMSEn);
VOID    DISP_MonitorVBICtrl(VOID);
VOID    DISP_PSMode(DWORD dwEnable);
VOID    DISP_VideoMaster(DWORD dwEnable, DWORD dwOverRunPTS);
VOID    DISP_ForceOneField(DWORD dwEnable);
VOID    DISP_NewSTC(DWORD dwSTC, DWORD dwFlag);
VOID    DISP_MainVideoSet(PWIN_PARAMETER MainVideo, DWORD dwSrcW, DWORD dwSrcH, DWORD dwFrameIdx);
VOID    DISP_TVModeSet(DWORD dwMode, WORD wHStart, WORD wVStart);
VOID    DISP_PIPSet(PWIN_PARAMETER PIP, DWORD dwSrcW, DWORD dwSrcH, DWORD dwFrameIdx);
VOID    DISP_PIPBorderSet(DWORD dwWidth, DWORD dwHeight, DWORD dwEnable);
VOID    DISP_DigestSet(PDIGESTVIDEO_INFO DigestVideo_Info);
VOID    DISP_AdjustContrast (SHORT sValue);
VOID    DISP_AdjustBright (SHORT sValue);
VOID    DISP_SetPalette(DWORD *pdwPalette);
VOID    DISP_OSDHighLight(PWIN_PARAMETER OSDBRK, SHORT sBRKAdjust);
VOID    DISP_FrameBufferMode(DWORD dwFBRMode, DWORD dwBFRMode);
VOID    DISP_OSDSet(PWIN_PARAMETER OSD, DWORD dwOSDWidth, DWORD dwOSDHeight);
// VOID    DISP_OSDEffect(DWORD dwBlend, DWORD dwMode);
VOID    DISP_OSDSetMixRatio(DWORD dwBlend);
VOID    DISP_OSDSetColorMode(DWORD dwMode);
VOID    DISP_FrameBufferSet(DWORD dwF0Adr, DWORD dwF1Adr, DWORD dwF2Adr, DWORD dwF3Adr, DWORD dwYMax);
VOID    DISP_BGColorSet(WIN_COLOR Color, DWORD dwWindow);
VOID    DISP_Clip(PWIN_PARAMETER ClipArea, DWORD dwFrameIdx, DWORD dwWindow);
VOID    DISP_Display(DWORD dwFrameIdx, DWORD dwWindow);
VOID    DISP_ForceDisplay(DWORD dwFrameIdx, DWORD dwWindow);
VOID    DISP_DisplayCtrl(DWORD dwWindow, DWORD dwEnable);
VOID    DISP_OneFieldCtrl(DWORD dwWindow, DWORD dwEnable);
DWORD   DISP_ShowEffect(DWORD dwEffectCase, DWORD dwStep, DWORD dwFrame);
VOID    DISP_Vsync(VOID);
VOID    DISP_ScreenEnd(DWORD dwSTC);
DWORD   DISP_QueryAvailableBuffer(VOID);
VOID    ISR_DISPSaveClearStatus( VOID );
VOID    ISR_VSYNC(VOID);

/////////////////////////////////////////////////////////////////////
// Global Variable Area
/////////////////////////////////////////////////////////////////////
// extern FLAG_T  __fDISPInfo;
extern DISP_CTRL  __DISPFrameInfo[4];
extern CGMSA_DATA  __sCGMSA_DATA[3];
extern APTS_INFO __APTS_Info[APTS_NUM]; // Record Audio PTS & its address
extern DWORD   __dwDISPTR;
extern volatile DWORD  __dwVSYNC;
extern DWORD   __dwOSD_Region_Base;
extern DWORD   __dwM_sel;
extern DWORD   __dwDISPSlowCnt;
extern DWORD   __dwDISPSlowLevel;
extern DWORD   __dwDISPMode;
extern DWORD   __dwVideo_en;
extern DWORD   __dwFrame_Rate;
extern DWORD   __dwOutputRate;
extern DWORD   __dwVideoMaster;
extern DWORD   __dwSTCUpdated;
extern DWORD   __dwNewSTCStatus;
extern DWORD   __dwDISP_PTS;
extern DWORD   __dwFBRMode;
extern DWORD   __dwBFRMode;
extern DWORD   __dwAPTS_Rp;
extern DWORD   __dwAPTS_Wp;
#ifdef __cplusplus
        }
#endif

#endif
