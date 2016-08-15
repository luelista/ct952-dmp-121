//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2004,
//          D300, all right reserved.
//
//      Product : CT909
//
//      Date    : 2004.05.03
//      Author  : Cheertek (D300 Chuan)
//      Purpose : Provide the register definition of SPU
//      Sources :
//  **************************************************************************
// =================================================================================================
#ifndef __CTKAV_SPU_H__
#define __CTKAV_SPU_H__
// =================================================================================================
#ifdef __cplusplus
        extern  "C"     {
#endif // __cplusplus
// =================================================================================================
#define     REG_SPU_BASE                (CT909_IO_START+0x1900L)
// -------------------------------------------------------------------------------------------------
// define SPU Registers
#define     REG_SPU_SPCTL               (*(volatile DWORD *)(REG_SPU_BASE+0x00L))
#define     REG_SPU_SP_VHDS             (*(volatile DWORD *)(REG_SPU_BASE+0x04L))
#define     REG_SPU_HL1VA1              (*(volatile DWORD *)(REG_SPU_BASE+0x08L))
#define     REG_SPU_HL1HA1              (*(volatile DWORD *)(REG_SPU_BASE+0x0cL))
#define     REG_SPU_HL1VA2              (*(volatile DWORD *)(REG_SPU_BASE+0x10L))
#define     REG_SPU_HL1HA2              (*(volatile DWORD *)(REG_SPU_BASE+0x14L))
#define     REG_SPU_HL2VA1              (*(volatile DWORD *)(REG_SPU_BASE+0x18L))
#define     REG_SPU_HL2HA1              (*(volatile DWORD *)(REG_SPU_BASE+0x1cL))
#define     REG_SPU_HL2VA2              (*(volatile DWORD *)(REG_SPU_BASE+0x20L))
#define     REG_SPU_HL2HA2              (*(volatile DWORD *)(REG_SPU_BASE+0x24L))
#define     REG_SPU_SP_VA(n)            (*(volatile DWORD *)(REG_SPU_BASE+0x28L+((n)<<3)))
#define     REG_SPU_SP_VA1              (*(volatile DWORD *)(REG_SPU_BASE+0x28L))
#define     REG_SPU_SP_HA(n)            (*(volatile DWORD *)(REG_SPU_BASE+0x2cL+((n)<<3)))
#define     REG_SPU_SP_HA1              (*(volatile DWORD *)(REG_SPU_BASE+0x2cL))
#define     REG_SPU_SP_VA2              (*(volatile DWORD *)(REG_SPU_BASE+0x30L))
#define     REG_SPU_SP_HA2              (*(volatile DWORD *)(REG_SPU_BASE+0x34L))
#define     REG_SPU_SP1_VCSH            (*(volatile DWORD *)(REG_SPU_BASE+0x38L))
#define     REG_SPU_SP2_VCSH            (*(volatile DWORD *)(REG_SPU_BASE+0x3cL))
#define     REG_SPU_SP1_HCSW            (*(volatile DWORD *)(REG_SPU_BASE+0x40L))
#define     REG_SPU_SP2_HCSW            (*(volatile DWORD *)(REG_SPU_BASE+0x44L))
#define     REG_SPU_SP_VHSTEP           (*(volatile DWORD *)(REG_SPU_BASE+0x48L))
#define     REG_SPU_SP12_OFFSET         (*(volatile DWORD *)(REG_SPU_BASE+0x4cL))
#define     REG_SPU_SP12_HOFFSET        (*(volatile DWORD *)(REG_SPU_BASE+0x50L))
#define     REG_SPU_HCOLOR(n)           (*(volatile DWORD *)(REG_SPU_BASE+0x54L+((n)<<3)))
#define     REG_SPU_HCOLOR1             (*(volatile DWORD *)(REG_SPU_BASE+0x54L))
#define     REG_SPU_HCONTRAST(n)        (*(volatile DWORD *)(REG_SPU_BASE+0x58L+((n)<<3)))
#define     REG_SPU_HCONTRAST1          (*(volatile DWORD *)(REG_SPU_BASE+0x58L))
#define     REG_SPU_HCOLOR2             (*(volatile DWORD *)(REG_SPU_BASE+0x5cL))
#define     REG_SPU_HCONTRAST2          (*(volatile DWORD *)(REG_SPU_BASE+0x60L))
#define     REG_SPU_SCOLOR              (*(volatile DWORD *)(REG_SPU_BASE+0x64L))
#define     REG_SPU_SCONTRAST           (*(volatile DWORD *)(REG_SPU_BASE+0x68L))
#define     REG_SPU_SB_PXD              (*(volatile DWORD *)(REG_SPU_BASE+0x6cL))
#define     REG_SPU_SB_CC               (*(volatile DWORD *)(REG_SPU_BASE+0x70L))
#define     REG_SPU_VCD30_VA            (*(volatile DWORD *)(REG_SPU_BASE+0x74L))
#define     REG_SPU_VCD30_HA            (*(volatile DWORD *)(REG_SPU_BASE+0x78L))
#define     REG_SPU_VCD30_CTRL          (*(volatile DWORD *)(REG_SPU_BASE+0x7cL))
#define     REG_SPU_VCD30_YUV0          (*(volatile DWORD *)(REG_SPU_BASE+0x80L))
#define     REG_SPU_VCD30_YUV1          (*(volatile DWORD *)(REG_SPU_BASE+0x84L))
#define     REG_SPU_CPR_YUV(n)          (*(volatile DWORD *)(REG_SPU_BASE+0x88L+((n)<<2)))
#define     REG_SPU_CPR_YUV0            (*(volatile DWORD *)(REG_SPU_BASE+0x88L))
#define     REG_SPU_CPR_YUV1            (*(volatile DWORD *)(REG_SPU_BASE+0x8cL))
#define     REG_SPU_CPR_YUV2            (*(volatile DWORD *)(REG_SPU_BASE+0x90L))
#define     REG_SPU_CPR_YUV3            (*(volatile DWORD *)(REG_SPU_BASE+0x94L))
#define     REG_SPU_CPR_YUV4            (*(volatile DWORD *)(REG_SPU_BASE+0x98L))
#define     REG_SPU_CPR_YUV5            (*(volatile DWORD *)(REG_SPU_BASE+0x9cL))
#define     REG_SPU_CPR_YUV6            (*(volatile DWORD *)(REG_SPU_BASE+0xa0L))
#define     REG_SPU_CPR_YUV7            (*(volatile DWORD *)(REG_SPU_BASE+0xa4L))
#define     REG_SPU_CPR_YUV8            (*(volatile DWORD *)(REG_SPU_BASE+0xa8L))
#define     REG_SPU_CPR_YUV9            (*(volatile DWORD *)(REG_SPU_BASE+0xacL))
#define     REG_SPU_CPR_YUV10           (*(volatile DWORD *)(REG_SPU_BASE+0xb0L))
#define     REG_SPU_CPR_YUV11           (*(volatile DWORD *)(REG_SPU_BASE+0xb4L))
#define     REG_SPU_CPR_YUV12           (*(volatile DWORD *)(REG_SPU_BASE+0xb8L))
#define     REG_SPU_CPR_YUV13           (*(volatile DWORD *)(REG_SPU_BASE+0xbcL))
#define     REG_SPU_CPR_YUV14           (*(volatile DWORD *)(REG_SPU_BASE+0xc0L))
#define     REG_SPU_CPR_YUV15           (*(volatile DWORD *)(REG_SPU_BASE+0xc4L))
#define     REG_SPU_MASK0               (*(volatile DWORD *)(REG_SPU_BASE+0xc8L))
#define     REG_SPU_MASK1               (*(volatile DWORD *)(REG_SPU_BASE+0xccL))
#define     REG_SPU_MASK2               (*(volatile DWORD *)(REG_SPU_BASE+0xd0L))
#define     REG_SPU_MASK3               (*(volatile DWORD *)(REG_SPU_BASE+0xd4L))
#define     REG_SPU_MASK4               (*(volatile DWORD *)(REG_SPU_BASE+0xd8L))
#define     REG_SPU_LCUT_IDX0           (*(volatile DWORD *)(REG_SPU_BASE+0xdcL))
#define     REG_SPU_LCUT_IDX1           (*(volatile DWORD *)(REG_SPU_BASE+0xe0L))
#define     REG_SPU_LCUT_IDX2           (*(volatile DWORD *)(REG_SPU_BASE+0xe4L))
#define     REG_SPU_LCUT_IDX3           (*(volatile DWORD *)(REG_SPU_BASE+0xe8L))
#define     REG_SPU_LCUT_IDX4           (*(volatile DWORD *)(REG_SPU_BASE+0xecL))
#define     REG_SPU_RISC_ACS            (*(volatile DWORD *)(REG_SPU_BASE+0xf0L))
#define     REG_SPU_SP_ERR              (*(volatile DWORD *)(REG_SPU_BASE+0xf4L))
#define     REG_SPU_GC                  (*(volatile DWORD *)(REG_SPU_BASE+0xf8L))
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
    #define     REG_SPU_BMP_CFG             (*(volatile DWORD *)(REG_SPU_BASE+0xfcL))
#else
    #define     REG_SPU_GC_CLR              (*(volatile DWORD *)(REG_SPU_BASE+0xfcL))
#endif
// =================================================================================================
// 909S: SPCTL
#define     SPU_SP1_EN                  (0x00000001)
#define     SPU_SP2_EN                  (0x00000002)
#define     SPU_SP12_IDX                (0x00000004)
#define     SPU_PRI_IDX                 (0x00000008)
#define     SPU_HL1R1_EN                (0x00000010)
#define     SPU_HL1R2_EN                (0x00000020)
#define     SPU_HL1_EN                  (SPU_HL1R1_EN | SPU_HL1R2_EN)
#define     SPU_HL2R1_EN                (0x00000040)
#define     SPU_HL2R2_EN                (0x00000080)
#define     SPU_HL2_EN                  (SPU_HL2R1_EN | SPU_HL2R2_EN)
#define     SPU_VCD30_HL_EN             (0x00000100)
#define     SPU_FRAME_DECODE            (0x00000200)
#define     SPU_SPHS_EN                 (0x00000400)
#define     SPU_SPVS_EN                 (0x00000800)
#define     SPU_SPV_SHIFT_EN            (0x00001000)
#define     SPU_SPH_SHIFT_EN            (0x00002000)
#define     SPU_SPV_CLIP_TYPE           (0x0000c000)
#define     SPU_SPV_CLIP_EN             (0x00010000)
#define     SPU_SPH_CLIP_EN             (0x00020000)
#define     SPU_CHCOLEN_1               (0x00040000)
#define     SPU_CHCOLEN_2               (0x00080000)
#define     SPU_SPREG_RDY               (0x00100000)
#define     SPU_SPREG1_RST              (0x00200000)
#define     SPU_SPREG2_RST              (0x00400000)
#define     SPU_CFF_REQ                 (0x00800000)
#define     SPU_B_FIRST_1               (0x01000000)
#define     SPU_B_FIRST_2               (0x02000000)
#define     SPU_FIELD_INVERSE           (0x04000000)
#define     SPU_BMP_EN                  (0x10000000)
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
    #define     SPU_FREE_RUN                (0x20000000)
#endif
#define     SPU_TEST_END                (0x80000000)
// -------------------------------------------------------------------------------------------------
// 909S: SB_PXD
#define     SPU_SP1_BOTTOMFIELD         (0x00000003)
#define     SPU_SP1_TOPFIELD            (0x00000300)
#define     SPU_SP2_BOTTOMFIELD         (0x00030000)
#define     SPU_SP2_TOPFIELD            (0x03000000)
// -------------------------------------------------------------------------------------------------
// 909S: SB_CC
#define     SPU_SP1_CHG_COLCON          (0x00000003)
#define     SPU_SP2_CHG_COLCON          (0x00000300)
// -------------------------------------------------------------------------------------------------
// 909S: SP_ERR
#define     SPU_ERR_MASK0               (0x00000001)
#define     SPU_ERR_MASK1               (0x00000002)
#define     SPU_ERR_MASK2               (0x00000004)
#define     SPU_ERR_MASK3               (0x00000008)
#define     SPU_ERR_MASK4               (0x00000010)
#define     SPU_ERR_MASK5               (0x00000020)
#define     SPU_CE_CLR                  (0x00010000)
#define     SPU_F_DFF0                  (0x00020000)
#define     SPU_F_DFF1                  (0x00040000)
#define     SPU_F_CFF                   (0x00080000)
#define     SPU_RLC_E                   (0x00100000)
#define     SPU_F_CLDET                 (0x00200000)
#define     SPU_RF_SLOW                 (0x00400000)
// =================================================================================================
#ifdef __cplusplus
        }
#endif // __cplusplus
// =================================================================================================
#endif // __CTKAV_SPU_H__
// =================================================================================================
