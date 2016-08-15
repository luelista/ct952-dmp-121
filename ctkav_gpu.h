//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2004-2007,
//          J000, all right reserved.
//
//      Product : CT909/ CT909S/ CT909P
//
//      Date    : 2006.11.03
//      Author  : Cheertek (J000 BrianCheng)
//      Purpose : Provide the register definition of GPU
//      Sources : 
//  **************************************************************************

#ifndef __CTKAV_GPU_H__
#define __CTKAV_GPU_H__

#ifdef __cplusplus
        extern  "C"     {
#endif


#define     REG_GPU_BASE                (CT909_IO_START+0x2880L)
#define     REG_GPU_CTL0                (*(volatile DWORD *)(REG_GPU_BASE+0x0L))    // 0x2880 
#define     REG_GPU_CTL1                (*(volatile DWORD *)(REG_GPU_BASE+0x4L))    // 0x2884  
#define     REG_GPU_COL_NDX             (*(volatile DWORD *)(REG_GPU_BASE+0x8L))    // 0x2888  
#define     REG_GPU_OP_SIZE             (*(volatile DWORD *)(REG_GPU_BASE+0xCL))    // 0x288C
#define     REG_GPU_AG_OFF              (*(volatile DWORD *)(REG_GPU_BASE+0x10L))   // 0x2890
#define     REG_GPU_SRC_ADDR            (*(volatile DWORD *)(REG_GPU_BASE+0x14L))   // 0x2894
#define     REG_GPU_DEST_ADDR           (*(volatile DWORD *)(REG_GPU_BASE+0x18L))   // 0x2898
#define     REG_GPU_FONT_ADDR           (*(volatile DWORD *)(REG_GPU_BASE+0x1CL))   // 0x289C
#define     REG_GPU_FONT_CONFIG           (*(volatile DWORD *)(REG_GPU_BASE+0x20L))   // 0x28A0
#define     REG_GPU_OP_THRE           (*(volatile DWORD *)(REG_GPU_BASE+0x24L))   // 0x28A4

// Font RAM index
//#define     REG_GPU_G10                  (*(volatile DWORD *)(REG_GPU_BASE+0x28L))   // 0x28A8
#define     REG_GPU_FONT_RAM_INDEX                  (*(volatile DWORD *)(REG_GPU_BASE+0x28L))   // 0x28A8

// Font RAM
#define     REG_GPU_G11                  (*(volatile DWORD *)(REG_GPU_BASE+0x2CL))   // 0x28AC
#define     REG_GPU_G12                  (*(volatile DWORD *)(REG_GPU_BASE+0x30L))   // 0x28B0
#define     REG_GPU_G13                 (*(volatile DWORD *)(REG_GPU_BASE+0x34L))   // 0x28B4
#define     REG_GPU_G14                  (*(volatile DWORD *)(REG_GPU_BASE+0x38L))   // 0x28B8
#define     REG_GPU_G15                  (*(volatile DWORD *)(REG_GPU_BASE+0x3CL))   // 0x28BC
#define     REG_GPU_G16                  (*(volatile DWORD *)(REG_GPU_BASE+0x40L))   // 0x28C0
#define     REG_GPU_G17                  (*(volatile DWORD *)(REG_GPU_BASE+0x44L))   // 0x28C4



// GPU Control 0 bit definition
#define     GPU_CLR_FONT                (0x80000000L)   // GPU_CTL0[31]
#define     GPU_OP                            (0x10000000L)   // GPU_CTL0[28]   


#define     GPU_REM_W_THRE              (0x00700000L)   // GPU_CTL0[22:20] 
#define     GPU_REM_R_THRE              (0x00070000L)   // GPU_CTL0[18:16]


#define     GPU_DEST_INC_V              (0x00008000L)   // GPU_CTL0[15]
#define     GPU_DEST_INC_H              (0x00004000L)   // GPU_CTL0[14]
#define     GPU_SRC_INC_V               (0x00002000L)   // GPU_CTL0[13]
#define     GPU_SRC_INC_H               (0x00001000L)   // GPU_CTL0[12]


#define     GPU_OP_JUDGE_EN         (0x00000800L)   //GPU_CTL0[11]
#define     GPU_INT_EN                      (0x00000400L)   //GPU_CTL0[10]
#define     GPU_STATUS                  (0x00000200L)   // GPU_CTL0[9]
#define     GPU_COL_KEY_EN              (0x00000100L)   // GPU_CTL0[8]

#define     GPU_MIRROR_EN                  (0x00000040L)   // GPU_CTL0[6] for op_mode 101 only
#define     GPU_FONT_1BIT              (0x00000020L)   // GPU_CTL0[5]
#define     GPU_OP_MODE                 (0x0000000CL)   // GPU_CTL0[4:2]
#define     GPU_START                   (0x00000002L)   // GPU_CTL0[1]
#define     GPU_ACCESS_FONT_EN                   (0x00000001L)   // GPU_CTL0[0]

// GPU Control 1  bit definition
#define     GPU_FILL_DATA               (0xFF000000L)   // GPU_CTL1[31:24]
#define     GPU_COL_KEY                             (0x00FF0000L)   // GPU_CTL1[23:16]
#define     GPU_TOTAL_FONT_WIDTH        (0x000001FFL)   // GPU_CTL1[8:0]


// REG_GPU_FONT_CONFIG
#define     GPU_FONT_WIDTH                (0x07000000L)   // [26:24], in DW, max value is 4
#define     GPU_FONT_NUM                (0x000F0000L)   //     [19:16], max value is 14
#define     GPU_FONT_CAPACITY                (0x00000FFFL)   //     [11:0], in DW

// REG_GPU_OP_THRE
#define GPU_OP_U_BOUND                  (0x03FF0000L)       // [25:16]
#define GPU_OP_L_BOUND                  (0x000003FFL)       // [9:0]


#ifdef __cplusplus
        }
#endif

#endif // #ifndef __REG_GPU_H_
