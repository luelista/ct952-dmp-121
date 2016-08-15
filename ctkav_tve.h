//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2004-2007,
//          J000, all right reserved.
//
//      Product : CT909/ CT909S/ CT909P
//
//      Date    : 2006.11.03
//      Author  : Cheertek (J500 Chuan)
//      Purpose : Provide the register definition of TVE
//      Sources : 
//  **************************************************************************

#ifndef __CTKAV_TVE_H__
#define __CTKAV_TVE_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

#define     REG_TVE_BASE                (CT909_IO_START+0x1880L)

#define     REG_TVE_R0                  (*(volatile DWORD *)(REG_TVE_BASE+0x0L))    // 0x1880
#define     REG_TVE_R1                  (*(volatile DWORD *)(REG_TVE_BASE+0x4L))    // 0x1884
#define     REG_TVE_CCTX_LN_O           (*(volatile DWORD *)(REG_TVE_BASE+0x8L))    // 0x1888
#define     REG_TVE_CCTX_LN_E           (*(volatile DWORD *)(REG_TVE_BASE+0xCL))    // 0x188C
#define     REG_TVE_CC_D                (*(volatile DWORD *)(REG_TVE_BASE+0x10L))   // 0x1890
#define     REG_TVE_WSS_D_O             (*(volatile DWORD *)(REG_TVE_BASE+0x14L))   // 0x1894
#define     REG_TVE_WSS_D_E             (*(volatile DWORD *)(REG_TVE_BASE+0x18L))   // 0x1898
#define     REG_TVE_GAIN_YC             (*(volatile DWORD *)(REG_TVE_BASE+0x1CL))   // 0x189C
#define     REG_TVE_GAIN_CAV            (*(volatile DWORD *)(REG_TVE_BASE+0x20L))   // 0x18A0
#define     REG_TVE_GAIN_G              (*(volatile DWORD *)(REG_TVE_BASE+0x24L))   // 0x18A4
#define     REG_TVE_MV0                 (*(volatile DWORD *)(REG_TVE_BASE+0x28L))   // 0x18A8
#define     REG_TVE_MV1                 (*(volatile DWORD *)(REG_TVE_BASE+0x2CL))   // 0x18AC
#define     REG_TVE_MV2                 (*(volatile DWORD *)(REG_TVE_BASE+0x30L))   // 0x18B0
#define     REG_TVE_MV3                 (*(volatile DWORD *)(REG_TVE_BASE+0x34L))   // 0x18B4
#define     REG_TVE_MV4                 (*(volatile DWORD *)(REG_TVE_BASE+0x38L))   // 0x18B8
#define     REG_TVE_MV5                 (*(volatile DWORD *)(REG_TVE_BASE+0x3CL))   // 0x18BC
#define     REG_TVE_R2                  (*(volatile DWORD *)(REG_TVE_BASE+0x40L))   // 0x18C0

#ifdef __cplusplus
        }
#endif

#endif
