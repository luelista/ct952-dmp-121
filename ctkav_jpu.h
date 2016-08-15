//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2006,
//          J500, all right reserved.
//
//      Product : CT909, CT909S
//
//      Author  : Cheertek (J500 Tommy)
//      Purpose : Provide the register definition of JPU
//      Sources : 
//  **************************************************************************

#ifndef __CTKAV_JPU_H__
#define __CTKAV_JPU_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

#define     REG_JPU_BASE                (CT909_IO_START+0x2880L)
#define     REG_JPU_CTRL                (*(volatile DWORD *)(REG_JPU_BASE+0x0L))    // 0x2880
#define     REG_JPU_ADDR_R_ST           (*(volatile DWORD *)(REG_JPU_BASE+0x4L))    // 0x2884
#define     REG_JPU_ADDR_W_ST           (*(volatile DWORD *)(REG_JPU_BASE+0x8L))    // 0x2888
#define     REG_JPU_STRIPE_RW           (*(volatile DWORD *)(REG_JPU_BASE+0xCL))    // 0x288C
#define     REG_JPU_HEIWID_SRC          (*(volatile DWORD *)(REG_JPU_BASE+0x10L))   // 0x2890
#define     REG_JPU_HEIWID_DST          (*(volatile DWORD *)(REG_JPU_BASE+0x14L))   // 0x2894
#define     REG_JPU_HVSC_FACTOR         (*(volatile DWORD *)(REG_JPU_BASE+0x18L))   // 0x2898
#define     REG_JPU_FILL_DATA           (*(volatile DWORD *)(REG_JPU_BASE+0x1CL))   // 0x289C

#define     REG_JPU_RW_OFFSET           (*(volatile DWORD *)(REG_JPU_BASE+0x20L))   // 0x28A0
#define     REG_JPU_MPEGSC_CR           (*(volatile DWORD *)(REG_JPU_BASE+0x24L))   // 0x28A4


// JPU_CTRL

#define     JPU_BUSY                    (0x1L)          // JPU_CTRL[0]
#define     JPU_GO                      (0x2L)          // JPU_CTRL[1]
//#define   reserved                    (0x4L)          // JPU_CTRL[2] reserved
#define     JPU_UV_IDX                  (0x8L)          // JPU_CTRL[3]. 0: Y, 1: UV
#define     JPU_OP_MODE                 (0x70L)         // JPU_CTRL[6:4], JPU's operation mode
                                                        // 000 reserved, 001 scaling, 010 flip or rotate
                                                        // 011 scaling and flip or rotate, 100 color filling select Y,
                                                        // 101 color filling select U, 110 color filling select V,
                                                        // 111 de-flickering
//define direct operation values for convenience
//#define   JPU_FT_OP                   (0x00L)         //there is no UV-interleaving for CT909S
#define     JPU_SC_OP                   (0x10L)
#define     JPU_FR_OP                   (0x20L)
#define     JPU_SC_FR_OP                (0x30L)
#define     JPU_FC_Y_OP                 (0x40L)
#define     JPU_FC_U_OP                 (0x50L)
#define     JPU_FC_V_OP                 (0x60L)
#define     JPU_DE_FL                   (0x70L)

//#define   reserved                    (0x80L)         // JPU_CTRL[7] reserved
#define     JPU_FR_TYPE                 (0x700L)        // JPU_CTRL[8:10], type of flipping or rotation
                                                        // 000 original, 001 R90C, 010 R180C, 011 R270C
                                                        // 100 flip horizontally, 101 FH+R90C, 110 FH+R180C, 111 FH+R270C
//#define   reserved                    (0x800L)        // JPU_CTRL[11] reserved
#define     JPU_YUV_TYPE                (0x3000L)       // JPU_CTRL[13:12], YUV sampling type.
#define     JPU_H_NEAREST_EN            (0x4000L)       // JPU_CTRL[14], interpolation or select nearest pixel, horizontally
                                                        // 0: resampling by interpolation
                                                        // 1: resampling by selecting the nearest one
#define     JPU_V_NEAREST_EN            (0x8000L)       // JPU_CTRL[15], interpolation or select nearest pixel, vertically
//#define   reserved                    (0xFF0000L)     // JPU_CTRL[23:16] reserved
#define     JPU_TEST_FINISH             (0x1000000L)    // JPU_CTRL[24], finish the test on macros?
//#define   reserved                    (0xD000000L)    // JPU_CTRL[27:25] reserved
#define     JPU_GPU_OP                  (0x10000000L)   // JPU_CTRL[28], GPU operation
                                                        // 0: JPU operation. All registers' indexes are JPU-defined.
                                                        // 1: GPU operation. All registers' indexes are GPU-defined.
//#define   reserved                    (0x60000000L)   // JPU_CTRL[30:29] reserved
#define     JPU_INT_CLR                 (0x80000000L)   // JPU_CTRL[31], signal to reset JPU's interrupt

// ADDR_R_ST
#define     JPU_ADDR_R_ST               (0x07FFFFFCL)   // ADDR_R_ST[26:2]

// ADDR_W_ST
#define     JPU_ADDR_W_ST               (0x07FFFFFCL)   // ADDR_W_ST[26:2]

// STRIPE_RW
#define     JPU_STRIPE_R                (0x000001FFL)   // STRIPE_RW[8:0]
#define     JPU_STRIPE_W                (0x01FF0000L)   // STRIPE_RW[24:16]

// HEIWID_SRC
#define     JPU_H_SRC                   (0x00000FFFL)   // HEIWID_SRC[11:0]
#define     JPU_W_SRC                   (0x0FFF0000L)   // HEIWID_SRC[27:16]

// HEIWID_DST
#define     JPU_H_DST                   (0x00000FFFL)   // HEIWID_DST[11:0]
#define     JPU_W_DST                   (0x0FFF0000L)   // HEIWID_DST[27:16]

// HVSC_FACTOR
#define     JPU_H_STEP                  (0x0000FFFFL)   // HVSC_FACTOR[15:0]
#define     JPU_V_STEP                  (0xFFFF0000L)   // HVSC_FACTOR[31:16]

// FILL_DATA
#define     JPU_V                       (0x0000FFL)     // FILL_DATA[7:0]
#define     JPU_U                       (0x00FF00L)     // FILL_DATA[15:8]
#define     JPU_Y                       (0xFF0000L)     // FILL_DATA[23:16]

// RW_OFFSET
#define     JPU_Y_OFFSET_W              (0x1FL)         // RW_OFFSET[4:0]
#define     JPU_Y_ADJ_W_EN              (0x80L)         // RW_OFFSET[7]
#define     JPU_Y_OFFSET_R              (0x1F00L)       // RW_OFFSET[12:8]
#define     JPU_Y_ADJ_R_EN              (0x8000L)       // RW_OFFSET[15]
#define     JPU_X_OFFSET_W              (0x1F0000L)     // RW_OFFSET[19:16]
#define     JPU_X_ADJ_W_EN              (0x800000L)     // RW_OFFSET[23]
#define     JPU_X_OFFSET_R              (0x1F000000L)   // RW_OFFSET[27:24]
#define     JPU_X_ADJ_R_EN              (0x80000000L)   // RW_OFFSET[31]

#ifdef __cplusplus
        }
#endif

#endif
