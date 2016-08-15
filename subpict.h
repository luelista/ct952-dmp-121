//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2004,
//          D300, all right reserved.
//
//      Product : CT909
//
//      Date    : 2003.09.17
//      Author  : Cheertek (D300 Chuan)
//      Purpose : SUBPICTURE
//      Sources : subpict.c/ subpict.h
//  **************************************************************************
#ifndef __SUBPICT_H__
#define __SUBPICT_H__

#ifdef __cplusplus
   extern "C"{
#endif

#define SUPPORT_SPDEC

#include "parser.h"

//#define NO_PTS

// Joey2.11, Patch H/W bug for color change
#define PATCH_HWBUG_CC

#define     SP_TYPE_NONE            (0x0)
#define     SP_TYPE_DVD             (0x1)
#define     SP_TYPE_SVCD            (0x2)
#define     SP_TYPE_CVD             (0x3)
#define     SP_TYPE_DIVX            (0x4)

#define     SPPTS_OVERFLOW          (0x1000000L)

#define SHOWBYTE()          (REG_SPU_RISC_ACS)
#define SHOWWORD()          (SHOWBYTE() << 8 | SHOWBYTE())
#define SHOWDWORD()         (SHOWWORD() << 16 | SHOWWORD())

#define SPU_GETWORD(n) \
{  \
    n = SHOWBYTE(); \
    n = SHOWBYTE() | (n << 8); \
}

#define SPU_GETDWORD(n) \
{ \
    n = SHOWBYTE(); \
    n = SHOWBYTE() | (n << 8); \
    n = SHOWBYTE() | (n << 8); \
    n = SHOWBYTE() | (n << 8); \
}

#define SPU_GETWORD_LITTLE_ENDIAN(n) \
{  \
    n = SHOWBYTE(); \
    n |= (SHOWBYTE() << 8); \
}

#define SP_NULL_ID          (0xFFFFFFFF)

// SP display Control Command (SP_DCCMD)
#define DCCMD_FSTA_DSP              (0x00)
#define DCCMD_STA_DSP               (0x01)
#define DCCMD_STP_DSP               (0x02)
#define DCCMD_SET_COLOR             (0x03)
#define DCCMD_SET_CONTR             (0x04)
#define DCCMD_SET_DAREA             (0x05)
#define DCCMD_SET_DSPXA             (0x06)
#define DCCMD_CHG_COLCON            (0x07)
#define DCCMD_CMD_END               (0xFF)
#define CHG_COLCON_TERMINATION      (0x0FFFFFFF)

// OGT display Control Command (OGT_CMD)
#define OGT_TIME_ENABLE             (0x08000000)
#define OGT_TIME_DISABLE            (0x00000000)
#define OGT_REGION_TOPLEFT          (0x10000000)
#define OGT_REGION_BOTTOMRIGHT      (0x18000000)
#define OGT_COLOR                   (0x20000000)
#define OGT_COLOR_HL                (0x28000000)
#define OGT_BLEND                   (0x30000000)
#define OGT_BLEND_HL                (0x38000000)
#define OGT_TOPFIELD                (0x40000000)
#define OGT_BOTTOMFIELD             (0x48000000)
#define OGT_REGION_TOPLEFT_HL       (0x50000000)
#define OGT_REGION_BOTTOMRIGHT_HL   (0x58000000)
#define OGT_CMD_END                 (0x04080C10)

// The result code of DecodeDCSQ
// #define RST_DCSQ_WAIT_BUFFER        (1) // Need more data in buffer to decode the DCSQ. (not used now)
#define RST_DCSQ_ERROR              (2) // Some errors in the display control commands.
#define RST_DCSQ_NEXT_DCSQ          (3) // The current DCSQ has been decoded successfully, and there are still some DCSQs in the DCSQT.
#define RST_DCSQ_LAST_DCSQ          (4) // The current DCSQ has been decoded successfully, and is the last DCSQ in the DCSQT.

// Subpicture Stage
#define SPUSTAGE_PREPARE            (0)
// #define SPUSTAGE_CHECK_SIZE         (1) // (not used now)
#define SPUSTAGE_CHECK_START        (2)
#define SPUSTAGE_CHECK_END          (3)
#define SPUSTAGE_END                (4)


// Joey1.50, for 909 and 909S
#define MAKE_LCUT(RUN_POS, RUN_LEN, PXD_POS, SCALE, OFFSET, CODE_LEN)                   \
    (((RUN_POS) & 0xf) << 24) | (((RUN_LEN) & 0xf) << 20) | (((PXD_POS) & 0xf) << 16) | \
    (((SCALE) & 7) << 12) | (((OFFSET) & 3) << 8) | ((CODE_LEN) & 0x1f)

// DVD_SP
#define MASK0_DVD           (0x1FFFCL)
#define MASK1_DVD           (0x1FC00L)
#define MASK2_DVD           (0x1F000L)
#define MASK3_DVD           (0x1C000L)
#define MASK4_DVD           (0x1C000L)
#define LCUT_IDX0_DVD       MAKE_LCUT(0, 0, 14, 1, 0, 16)
#define LCUT_IDX1_DVD       MAKE_LCUT(6, 8, 14, 1, 0, 16)
#define LCUT_IDX2_DVD       MAKE_LCUT(4, 6, 10, 1, 0, 12)
#define LCUT_IDX3_DVD       MAKE_LCUT(2, 4,  6, 1, 0,  8)
#define LCUT_IDX4_DVD       MAKE_LCUT(0, 2,  2, 1, 0,  4)

// SVCD_OGT
#define MASK0_SVCD          (0x1FC00L)
#define MASK1_SVCD          (0x1C000L)
#define MASK2_SVCD          (0x1C000L)
#define LCUT_IDX0_SVCD      MAKE_LCUT(0, 0, 6, 1, 0, 8)
#define LCUT_IDX1_SVCD      MAKE_LCUT(2, 4, 6, 4, 0, 8)
#define LCUT_IDX2_SVCD      MAKE_LCUT(0, 2, 2, 1, 0, 4)

// CVD_OGT
#define MASK0_CVD           (0x1C000L)
#define MASK1_CVD           (0x1C000L)
#define LCUT_IDX0_CVD       MAKE_LCUT(2, 2, 0, 1, 1, 4)
#define LCUT_IDX1_CVD       MAKE_LCUT(0, 0, 0, 1, 0, 2)


// SP Display Mode
#define MODE_N2N            (0x0)
#define MODE_N2P            (0x1)
#define MODE_P2N            (0x2)
#define MODE_P2P            (0x3)
#define MODE_PS             (0x10)
#define MODE_LB             (0x20)
#define MODE_WS             (0x40)

#define USE_SP1             (0)
#define USE_SP2             (1)

#ifdef NO_PTS
#define EXTRA_BYTE          (0)
#else
#define EXTRA_BYTE          (8)
#endif

#define HLI_ACTION_OK       (0x0)
#define HLI_ACTION_SELECT   (0x1)
#define HLI_ACTION_ACTIVE   (0x2)
#define HLI_ACTION_ENTER    (0x3)
#define HLI_ACTION_OFF      (0x4)


// Constants for SP_CTRL::bEnable
#define SPCTRL_SP1              (0x1)
#define SPCTRL_SP2              (0x2)
#define SPCTRL_SP1_FSTA_DSP     (0x4)
// #define SPCTRL_SP2_FSTA_DSP     (0x8)
// #define SPCTRL_UPDATEREM        (0x8)
// #define SPCTRL_FREEZE           (0x10)
#define SPCTRL_REALPTS          (0x20)
#define SPCTRL_ENABLE           (0x40)
//#define SPCTRL_RESET_BUFFER (0x80)

// Constants for SP_CTRL::bStatus
#define SPCTRL_NEWSTC           (0x10)

#define MCU_SUB_SPREM_MAXSIZE   (0xffff*4)

#define MAGIC_PATTERN_CODE1     (0x000001BA)
#define MAGIC_PATTERN_CODE2     (0xFFFFFFFF)

#define     SPU_NUM         (128)
#define     SPU_MASK        (SPU_NUM-1)


typedef struct tagSP_CTRL
{
    DWORD   dwSP1ID;  // ID of SP#1, 0xFFFFFFFF indicate SP_NULL_ID
    DWORD   dwSP2ID;  // ID of SP#2, 0xFFFFFFFF indicate SP_NULL_ID
    BYTE    bEnable;
                        // bit[0] SP#1 on/off (SPCTRL_SP1) - WinDVD
                        // bit[1] SP#2 on/off (SPCTRL_SP2) - WinDVD
                        // bit[2] force display detected for SP1 (SPCTRL_SP1_FSTA_DSP) - SP
                        // bit[5] Real PTS reach (SPCTRL_REALPTS) - Decoder
                        // bit[6] enable (SPCTRL_ENABLE) - WinDVD/Parser
    BYTE    bStatus;
                        // bit[1:0] 0: force display detected, 1: display detected, 2: stop detected for SP1
                        // bit[3:2] 0: force display detected, 1: display detected, 2: stop detected for SP2
                        // bit[4] 0: Bitstream is continuous, 1: Bitstream is dis-continuous. (SPCTRL_NEWSTC)
} SP_CTRL, *PSP_CTRL;

typedef struct  tagspu
{
    DWORD   dwSPU_NextAddress[SPU_NUM];
    DWORD   dwReadPtr;
    DWORD   dwPacketPTS;
    DWORD   dwDCSQPTS;
    DWORD   dwRead_Counter;
    DWORD   dwWrite_Counter;
    DWORD   dwWrite_Counter_NewSTC; // Record Write Counter for NewSTC
    DWORD   dwSP_ID;
    DWORD   dwSP_NextID;
    DWORD   dwTopFieldAddr;
    DWORD   dwBottomFieldAddr;
    DWORD   dwTopFieldAddr_PL;
    DWORD   dwBottomFieldAddr_PL;
    DWORD   dwPattern[4];
    WORD    wDCSQ_STM;          // Execution Start Time of DCSQ (relative value)
    WORD    wSPU_SZ;            // Size of the current SPU
    WORD    wCurr_SPU_SZ;       // current wSPU_SZ
    WORD    wPrev_DCSQ_SA;      // previous wDCSQ_SA
    WORD    wDCSQ_SA;           // Starting address of the current DCSQ

// Joey2.11, Patch H/W bug for color change
#ifdef PATCH_HWBUG_CC
    WORD    wFirst_DCSQ_SA;     // Starting address of first DCSQ
#endif // PATCH_HWBUG_CC

    WORD    wVStart;
    WORD    wVEnd;
    WORD    wVStart_PL;
    WORD    wVEnd_PL;
    WORD    wPassLine;
    BYTE    bStage;
    BOOL    blUsage;
    BOOL    blReset;

// J500KWShih, For Nero Digital
//===================================================================
//  PARSER_ESFMT_DVD_SP  : for DVD sub-picture
//  PARSER_ESFMT_TEXT_SP : for UTF-8 charactor base subtitle
//===================================================================
    PARSER_ESFMT ESFormat;
} SPU, *PSPU;

typedef struct tagSTRU_HLI
{
    struct
    {
        WORD    wHLI_SS         PACK_FORMAT;   // Status of HLI
        DWORD   dwHLI_S_PTM     PACK_FORMAT;   // Start PTM of HLI
        DWORD   dwHLI_E_PTM    PACK_FORMAT;   // End PTM of HLI
        DWORD   dwBTN_SL_E_PTM  PACK_FORMAT;   // End PTM of Button Select
        WORD    wBTN_MD         PACK_FORMAT;   // Button Mode
        BYTE    bBTN_OFN        PACK_FORMAT;   // Button Offset Number
        BYTE    bBTN_Ns         PACK_FORMAT;   // Number of Buttons
        BYTE    bNSL_BTN_Ns     PACK_FORMAT;   // Number of Numerical Select Buttons
        BYTE    bReserved       PACK_FORMAT;   // Reserved
        BYTE    bFOSL_BTNN      PACK_FORMAT;   // Forcedly Selected Button Number
        BYTE    bFOAC_BTNN      PACK_FORMAT;   // Forcedly Activated Button Number
    } HL_GI; // 22 bytes

    struct
    {
        WORD    wSL_COLI_Color      PACK_FORMAT;   // Selection Color Information - Color
        WORD    wSL_COLI_Contrast   PACK_FORMAT;   // Selection Color Information - Contrast
        WORD    wAC_COLI_Color      PACK_FORMAT;   // Action Color Information - Color
        WORD    wAC_COLI_Contrast   PACK_FORMAT;   // Action Color Information - Contrast
    } BTN_COLIT[3]; // 8 bytes x 3

    struct
    {
        WORD    wBTN_POSI_1         PACK_FORMAT;
        DWORD   dwBTN_POSI_2        PACK_FORMAT;
        BYTE    bAJBTN_POSI_Upper   PACK_FORMAT;
        BYTE    bAJBTN_POSI_Lower   PACK_FORMAT;
        BYTE    bAJBTN_POSI_Left    PACK_FORMAT;
        BYTE    bAJBTN_POSI_Right   PACK_FORMAT;
        DWORD   dwBTN_CMD_1         PACK_FORMAT;
        DWORD   dwBTN_CMD_2         PACK_FORMAT;
    } BTNIT[36]; // 18 bytes x 36, maybe 1x36 or 2x18 or 3x12 BTNs
} HLI , *PHLI; // 694 bytes

extern volatile WIN_PARAMETER _SPU_MainVideo;   // backup of _MainVideo from disp
extern volatile SP_CTRL __SPUCtrl;
extern volatile SPU     __SP1;
extern volatile SPU     __SP2;
extern volatile PHLI    __pHLIBuffer;
extern volatile BYTE    __bHLI_Ready;
extern volatile BYTE    __bHLI_On;
extern volatile BYTE    __bHLI_Action;      // Inform SP to do HLI control. Possible active lists as below.
                                            // HLI_ACTION_SELECT: change the highlight to selection color.
                                            // HLI_ACTION_ACTIVE: change the highlight to active color.
                                            // HLI_ACTION_ENTER: change the highlight to active color. Same as ACTIVE?
                                            // HLI_ACTION_OFF: Turn off the HLI
                                            // HLI_ACTION_OK: do nothing.
extern BYTE    __bHLI_BTNNumber;    // Current BTN Number accord to different SPST display mode(LB/ WD/ PS)
extern BOOL    __blHLI_Status;      // TRUE: on, FALSE: off

extern DWORD   __dwSP_EndLine;
extern ULONG_UNION   __ulSPVHDS_HLI;
extern ULONG_UNION   __ulSPVHDS;
extern DWORD   __dwScaleFactor;

VOID    SPU_AdjustVHDS(VOID);
VOID    SPU_DisplayMode(BYTE bSPMode, BYTE bTVMode, CHAR cOffset);
VOID    SPU_Reset(DWORD dwType);
VOID    SPU_SetPalette(DWORD *dwPalette);
VOID    SPU_TimeStamp(DWORD dwPTS );
VOID    SPU_VCD30Set(WIN_PARAMETER VCD30, BYTE bColor, BYTE bContrast);
VOID    SPU_ScreenEnd(VOID);
VOID    SPU_Vsync(VOID);
VOID    SPU_NewSTC(DWORD dwNewSTC);
// Micky2.11a
VOID    SPU_ControlHighlight(BYTE bAction, BYTE bButton);

// Bitmap SP
VOID    SPU_BMP_Init(VOID);
VOID    SPU_BMP_SetDisplay(BYTE bSPn, BYTE bEnable);
VOID    SPU_BMP_SetColorContrast(BYTE bSPn, BYTE bColorIndex, DWORD dwColorYUV, BYTE bContrast);
VOID    SPU_BMP_SetDisplayArea(BYTE bSPn, WORD wX, WORD wY, WORD wWidth, WORD wHeight, const BYTE* pbBMP);

#ifdef __cplusplus
   }
#endif

#endif  // __SUBPICT_H__
