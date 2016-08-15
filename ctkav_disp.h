//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2004-2007,
//          J000, all right reserved.
//
//      Product : CT909/ CT909S/ CT909P
//
//      Date    : 2006.11.03
//      Author  : Cheertek (J500 Chuan)
//      Purpose : Provide the register definition of DISP
//      Sources : 
//  **************************************************************************

#ifndef __CTKAV_DISP_H__
#define __CTKAV_DISP_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

#define     REG_DISP_BASE               (CT909_IO_START+0x1A00L)
#define     REG_DISP_MAIN1_CR           (*(volatile DWORD *)(REG_DISP_BASE+0x00L))    // 0x1A00 Main frame 1 control
#define     REG_DISP_MAIN2_CR           (*(volatile DWORD *)(REG_DISP_BASE+0x04L))    // 0x1A04 Main frame 2 control
#define     REG_DISP_H_REQ              (*(volatile DWORD *)(REG_DISP_BASE+0x08L))    // 0x1A08 the times to access DRAM for each line
#define     REG_DISP_STRIPE             (*(volatile DWORD *)(REG_DISP_BASE+0x0CL))    // 0x1A0C Main frame's stripe
#define     REG_DISP_HCLIP_CR           (*(volatile DWORD *)(REG_DISP_BASE+0x10L))    // 0x1A10 Horizontal clip control
#define     REG_DISP_VCLIP_CR           (*(volatile DWORD *)(REG_DISP_BASE+0x14L))    // 0x1A14 Vertical clip control
#define     REG_DISP_REDUNDANT          (*(volatile DWORD *)(REG_DISP_BASE+0x18L))    // 0x1A18 Redundant for 1st DRAM access
#define     REG_DISP_VSCALE_CR          (*(volatile DWORD *)(REG_DISP_BASE+0x1CL))    // 0x1A1C Vertical scaling control
#define     REG_DISP_HU_SCALE_CR        (*(volatile DWORD *)(REG_DISP_BASE+0x20L))    // 0x1A20 Horizontal scaling up control
#define     REG_DISP_HD_SCALE_CR        (*(volatile DWORD *)(REG_DISP_BASE+0x24L))    // 0x1A24 Horizontal scaling down control
#define     REG_DISP_LB_CR1             (*(volatile DWORD *)(REG_DISP_BASE+0x28L))    // 0x1A28 Line buffer control 1
#define     REG_DISP_LB_CR2             (*(volatile DWORD *)(REG_DISP_BASE+0x2CL))    // 0x1A2C Line buffer control 2
#define     REG_DISP_MAIN_MODE          (*(volatile DWORD *)(REG_DISP_BASE+0x30L))    // 0x1A30 Main video mode control             
#define     REG_DISP_MAIN_BG            (*(volatile DWORD *)(REG_DISP_BASE+0x34L))    // 0x1A34 Main video background color
#define     REG_DISP_TGEN_TOTAL         (*(volatile DWORD *)(REG_DISP_BASE+0x38L))    // 0x1A38 Timing generator, H/V total width
#define     REG_DISP_SYNC_WH            (*(volatile DWORD *)(REG_DISP_BASE+0x3CL))    // 0x1A3C H/V sync pulse width
#define     REG_DISP_SCREEN_POS         (*(volatile DWORD *)(REG_DISP_BASE+0x40L))    // 0x1A40 Screen window LU position
#define     REG_DISP_SCREEN_SIZE        (*(volatile DWORD *)(REG_DISP_BASE+0x44L))    // 0x1A44 Screen window size
#define     REG_DISP_VIDEO_POS          (*(volatile DWORD *)(REG_DISP_BASE+0x48L))    // 0x1A48 Main video window LU position
#define     REG_DISP_VIDEO_SIZE         (*(volatile DWORD *)(REG_DISP_BASE+0x4CL))    // 0x1A4C Main video window size
#define     REG_DISP_OSD_POS            (*(volatile DWORD *)(REG_DISP_BASE+0x50L))    // 0x1A50 OSD window LU position
#define     REG_DISP_OSD_SIZE           (*(volatile DWORD *)(REG_DISP_BASE+0x54L))    // 0x1A54 OSD window H/V width
#define     REG_DISP_OSD_CR             (*(volatile DWORD *)(REG_DISP_BASE+0x58L))    // 0x1A58 OSD control register
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
#define     REG_DISP_OSD_CR1            (*(volatile DWORD *)(REG_DISP_BASE+0x5CL))    // 0x1A5C The definition of the transparent color for OSD
#define     REG_DISP_BRIGHT_CR          (*(volatile DWORD *)(REG_DISP_BASE+0x60L))    // 0x1A60 Brightness, contrast, and gamma
#define     REG_DISP_N_LINE_REPEAT      (*(volatile DWORD *)(REG_DISP_BASE+0x64L))    // 0x1A64 Back to 1st line after access n lines
#define     REG_DISP_MEM_LINE           (*(volatile DWORD *)(REG_DISP_BASE+0x68L))    // 0x1A68 Main display read frame buffer line position
#define     REG_DISP_N_HSYNC_INT        (*(volatile DWORD *)(REG_DISP_BASE+0x6CL))    // 0x1A6C Interrupt for each n hsyncs
#define     REG_DISP_BRK_OSD_POS        (*(volatile DWORD *)(REG_DISP_BASE+0x70L))    // 0x1A70 OSD's BRK window's LU position
#define     REG_DISP_BRK_OSD_SIZE       (*(volatile DWORD *)(REG_DISP_BASE+0x74L))    // 0x1A74 OSD's BRK window's size
#define     REG_DISP_BRADJ_OSD          (*(volatile DWORD *)(REG_DISP_BASE+0x78L))    // 0x1A78 the amount to adjust the brightness in the OSD's BRK region
#define     REG_DISP_GC0                (*(volatile DWORD *)(REG_DISP_BASE+0x7CL))    // 0x1A7C DISP's gated-clock control: status and enable bits
#define     REG_DISP_GC1                (*(volatile DWORD *)(REG_DISP_BASE+0x80L))    // 0x1A80 DISP's gated-clock control: disable bits
#define     REG_DISP_GC2                (*(volatile DWORD *)(REG_DISP_BASE+0x84L))    // 0x1A84 DISP's gated-clock control for the horizontal counter; setting for the vertical region
#define     REG_DISP_GC3                (*(volatile DWORD *)(REG_DISP_BASE+0x88L))    // 0x1A88 DISP's gated-clock control for the horizontal counter; setting for the horizonal region
#define     REG_DISP_ODD_CR0            (*(volatile DWORD *)(REG_DISP_BASE+0x8CL))    // 0x1A8C Vertical clip for auto-odd setting
#define     REG_DISP_ODD_CR1            (*(volatile DWORD *)(REG_DISP_BASE+0x90L))    // 0x1A90 Phase adjustment for auto-odd setting
#endif
#ifdef CT909R_IC_SYSTEM
#define     REG_DISP_BRIGHT_CR          (*(volatile DWORD *)(REG_DISP_BASE+0x5CL))    // 0x1A5C Brightness, contrast, and gamma
#define     REG_DISP_N_LINE_REPEAT      (*(volatile DWORD *)(REG_DISP_BASE+0x60L))    // 0x1A60 Back to 1st line after access n lines
#define     REG_DISP_MEM_LINE           (*(volatile DWORD *)(REG_DISP_BASE+0x64L))    // 0x1A64 Main display read frame buffer line position
#define     REG_DISP_N_HSYNC_INT        (*(volatile DWORD *)(REG_DISP_BASE+0x68L))    // 0x1A68 Interrupt for each n hsyncs
#define     REG_DISP_BRK_OSD_POS        (*(volatile DWORD *)(REG_DISP_BASE+0x6CL))    // 0x1A6C OSD's BRK window's LU position
#define     REG_DISP_BRK_OSD_SIZE       (*(volatile DWORD *)(REG_DISP_BASE+0x70L))    // 0x1A70 OSD's BRK window's size
#define     REG_DISP_BRADJ_OSD          (*(volatile DWORD *)(REG_DISP_BASE+0x74L))    // 0x1A74 the amount to adjust the brightness in the OSD's BRK region
#define     REG_DISP_GC0                (*(volatile DWORD *)(REG_DISP_BASE+0x78L))    // 0x1A78 DISP's gated-clock control: status and enable bits
#define     REG_DISP_GC1                (*(volatile DWORD *)(REG_DISP_BASE+0x7CL))    // 0x1A7C DISP's gated-clock control: disable bits
#endif
#define     REG_DISP_FY_ADDR(n)         (*(volatile DWORD *)(REG_DISP_BASE+0xC0L+((DWORD)(n)<<3)))
#define     REG_DISP_FC_ADDR(n)         (*(volatile DWORD *)(REG_DISP_BASE+0xC4L+((DWORD)(n)<<3)))
#define     REG_DISP_F0Y_ADDR           (*(volatile DWORD *)(REG_DISP_BASE+0xC0L))    // 0x1AC0 Main frame buffer 0 Y start address             
#define     REG_DISP_F0C_ADDR           (*(volatile DWORD *)(REG_DISP_BASE+0xC4L))    // 0x1AC4 Main frame buffer 0 C start address             
#define     REG_DISP_F1Y_ADDR           (*(volatile DWORD *)(REG_DISP_BASE+0xC8L))    // 0x1AC8 Main frame buffer 1 Y start address             
#define     REG_DISP_F1C_ADDR           (*(volatile DWORD *)(REG_DISP_BASE+0xCCL))    // 0x1ACC Main frame buffer 1 C start address             
#define     REG_DISP_F2Y_ADDR           (*(volatile DWORD *)(REG_DISP_BASE+0xD0L))    // 0x1AD0 Main frame buffer 2 Y start address             
#define     REG_DISP_F2C_ADDR           (*(volatile DWORD *)(REG_DISP_BASE+0xD4L))    // 0x1AD4 Main frame buffer 2 C start address             
#define     REG_DISP_F3Y_ADDR           (*(volatile DWORD *)(REG_DISP_BASE+0xD8L))    // 0x1AD8 Main frame buffer 3 Y start address             
#define     REG_DISP_F3C_ADDR           (*(volatile DWORD *)(REG_DISP_BASE+0xDCL))    // 0x1ADC Main frame buffer 3 C start address             
#define     REG_DISP_GAM_OSD(n)         (*(volatile DWORD *)(CT909_IO_START+0x1C00L+((DWORD)(n)<<2)))
//  GAM_OSD     1c00h~1ffch               RISC access window of Gamma RAM and OSD color palette.
//                                        Need insert about 24-idle cycle, also read data result is previous one address              

// VSCALE_CR
#define     DISP_V_NEAREST               (0x01000000L)
#define     DISP_V_ALIGN                 (0x02000000L)
#define     DISP_VPHASE_EN               (0x04000000L)
#define     DISP_VSCALE_EN               (0x08000000L)

// HU_SCALE_CR
#define     DISP_HU_NEAREST_EN           (0x00100000L)
#define     DISP_HU_ODD_FIRST            (0x00200000L)
#define     DISP_HU_REPEAT               (0x00400000L)
#define     DISP_DETV_EN                 (0x10000000L)

// HD_SCALE_CR
#define     DISP_SH_C_ODD                (0x01000000L)
#define     DISP_SH_C_SKIP               (0x02000000L)
#define     DISP_HD_NEARST_EN            (0x04000000L)
#define     DISP_HD_ODD_FIRST            (0x08000000L)
#define     DISP_HD_EN                   (0x10000000L)

// TGEN_TOTAL
#define     DISP_TGEN_EN                 (0x10000000L)

// SYNC_WH
#define     DISP_PSCAN_EN                (0x10000000L)

// VIDEO_SIZE
#define     DISP_VIDEO_EN                (0x10000000L)

// OSD_SIZE
#define     DISP_OSD_EN                  (0x10000000L)


// OSD_CR
#define     DISP_OSD_VINT_CLR            (0x00400000L)
#define     DISP_ACCESS_OSD              (0x01000000L)
#define     DISP_OSD_H2X                 (0x10000000L)
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
#define     DISP_OSD_VS_EN               (0x20000000L)
#endif
#ifdef CT909R_IC_SYSTEM
#define     DISP_OSD_VS_EN               (0x00100000L)
#endif

// OSD_CR1
#define     DISP_OSD_T_EN                (0x01000000L)

// BRIGHT_CR
#define     DISP_C_GAMMA_EN              (0x01000000L)
#define     DISP_Y_GAMMA_EN              (0x02000000L)
#define     DISP_SET_GAMMA               (0x10000000L)

// MEM_LINE
#define     DISP_TOPFIELD                (0)
#define     DISP_BOTTOMFIELD             (1)
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
#define     DISP_EVEN_FIELD              (0x10000000L)
#endif
#ifdef CT909R_IC_SYSTEM
#define     DISP_EVEN_FIELD              (0x00010000L)
#endif

#ifdef __cplusplus
        }
#endif

#endif
