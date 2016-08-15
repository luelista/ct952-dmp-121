//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2004,
//          D300, all right reserved.
//
//      Product : CT909
//
//      Date    : 2004.05.11
//      Author  : Cheertek (D100 BigFox)
//      Purpose : Provide the register definition of AIU
//      Sources : 
//  **************************************************************************
#ifndef __CTKAV_AIU_H__
#define __CTKAV_AIU_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

// REG_<Module Name>_BASE : Specify the base address of this module.
// REG_<Module Name>_<Register Name> : Specify the register name.

#define     REG_AIU_BASE                (CT909_IO_START+0x400L)
#define     REG_AIU_AIUEN               (*(volatile DWORD *)(REG_AIU_BASE+0x0L))
// UPK (1~71))
#define     REG_AIU_B14_INDEX           (*(volatile DWORD *)(REG_AIU_BASE+0x04L))    // B14_INDEXin
#define     REG_AIU_BYTE_ALIGN          (*(volatile DWORD *)(REG_AIU_BASE+0x08L))    // byte_align[0]
#define     REG_AIU_DBYTE_ALIGN         (*(volatile DWORD *)(REG_AIU_BASE+0x0cL))    // byte_align[1]
#define     REG_AIU_BITWCNT             (*(volatile DWORD *)(REG_AIU_BASE+0x10L))    // bitwcnt
#define     REG_AIU_BITNO               (*(volatile DWORD *)(REG_AIU_BASE+0x14L))    // read bitno
#define     REG_AIU_EXTRACT_BITS(n)     (*(volatile DWORD *)(REG_AIU_BASE+0x18L+(n))) // 418~498
#define     REG_AIU_SHOW_BITS(n)        (*(volatile DWORD *)(REG_AIU_BASE+0x9cL+(n))) // 49c~51c

// UPK2 ports (72~142))
#define     REG_AIU_B14_INDEX2          (*(volatile DWORD *)(REG_AIU_BASE+0x120L))  // B14_INDEXin
#define     REG_AIU_BYTE_ALIGN2         (*(volatile DWORD *)(REG_AIU_BASE+0x124L))  // byte_align[0]
#define     REG_AIU_DBYTE_ALIGN2        (*(volatile DWORD *)(REG_AIU_BASE+0x128L))  // byte_align[1]
#define     REG_AIU_BITWCNT2            (*(volatile DWORD *)(REG_AIU_BASE+0x12cL))  // bitwcnt2
#define     REG_AIU_BITNO2              (*(volatile DWORD *)(REG_AIU_BASE+0x130L))  // read bitno2
#define     REG_AIU_EXTRACT_BITS2(n)    (*(volatile DWORD *)(REG_AIU_BASE+0x134L+(n))) // 534~5b4
#define     REG_AIU_SHOW_BITS2(n)       (*(volatile DWORD *)(REG_AIU_BASE+0x1b8L+(n))) // 5b8~638

// PCMRD ports
#define     REG_AIU_SPORT_CONFG0        (*(volatile DWORD *)(REG_AIU_BASE+0x23cL)) // sel_acfg0
#define     REG_AIU_TXL                 (*(volatile DWORD *)(REG_AIU_BASE+0x240L)) // sel_txl
#define     REG_AIU_TXR                 (*(volatile DWORD *)(REG_AIU_BASE+0x244L)) // sel_txr
#define     REG_AIU_TXLS                (*(volatile DWORD *)(REG_AIU_BASE+0x248L)) // sel_txls
#define     REG_AIU_TXRS                (*(volatile DWORD *)(REG_AIU_BASE+0x24cL)) // sel_txrs
#define     REG_AIU_TXC                 (*(volatile DWORD *)(REG_AIU_BASE+0x250L)) // sel_txc
#define     REG_AIU_TXLFE               (*(volatile DWORD *)(REG_AIU_BASE+0x254L)) // sel_txlfe
#define     REG_AIU_TXLMIX              (*(volatile DWORD *)(REG_AIU_BASE+0x258L)) // sel_txlmix
#define     REG_AIU_TXRMIX              (*(volatile DWORD *)(REG_AIU_BASE+0x25cL)) // sel_txrmix

#if defined(CT909R_IC_SYSTEM) || defined(CT909P_IC_SYSTEM)
#define     REG_AIU_TXLSTEREO           (*(volatile DWORD *)(REG_AIU_BASE+0x260L)) // sel_txlstereo
#define     REG_AIU_TXRSTEREO           (*(volatile DWORD *)(REG_AIU_BASE+0x264L)) // sel_txrstereo
#define     REG_AIU_CLRTIR              (*(volatile DWORD *)(REG_AIU_BASE+0x268L)) // clear timer interrupt routine bit
#define     REG_AIU_SPORT_CONFG1        (*(volatile DWORD *)(REG_AIU_BASE+0x26cL)) // sel_acfg1 
// AIUWR ports
#define     REG_AIU_AIUWR               (*(volatile DWORD *)(REG_AIU_BASE+0x270L)) // aiuwr
// AIURD ports
#define     REG_AIU_AIURD               (*(volatile DWORD *)(REG_AIU_BASE+0x274L)) // regrd
// MICWR ports
#define     REG_AIU_MIC_CONFG           (*(volatile DWORD *)(REG_AIU_BASE+0x278L)) // sel_mic_acfg0
#define     REG_AIU_MICRD               (*(volatile DWORD *)(REG_AIU_BASE+0x27cL)) // sel_rd
#define     REG_AIU_MICMIX              (*(volatile DWORD *)(REG_AIU_BASE+0x280L)) // micmixdata_sel
// SPDIFOUT ports
#define     REG_AIU_SPDIFO_CONFG        (*(volatile DWORD *)(REG_AIU_BASE+0x284L)) // sel_spdifout_acfg0
#define     REG_AIU_SPDIFO_FRAMECNT     (*(volatile DWORD *)(REG_AIU_BASE+0x288L)) // blkcnt_rd and pcmrd bitcnt
#define     REG_AIU_TXLSPDIF            (*(volatile DWORD *)(REG_AIU_BASE+0x28cL)) // sel_txl
#define     REG_AIU_TXRSPDIF            (*(volatile DWORD *)(REG_AIU_BASE+0x290L)) // sel_txr
// SPDIFIN ports
#define     REG_AIU_SPDIFI_CONFG0       (*(volatile DWORD *)(REG_AIU_BASE+0x294L)) // sel_spdi_cfg0
#define     REG_AIU_SPDIFI_CONFG1       (*(volatile DWORD *)(REG_AIU_BASE+0x298L)) // sel_spdi_cfg1
#define     REG_AIU_SPDIFI_CONFG2       (*(volatile DWORD *)(REG_AIU_BASE+0x29cL)) // sel_spdi_cfg2
#define     REG_AIU_SPDIFI_CONFG3       (*(volatile DWORD *)(REG_AIU_BASE+0x2a0L)) // sel_spdi_cfg3
#define     REG_AIU_SPDIFI_CONFG4       (*(volatile DWORD *)(REG_AIU_BASE+0x2a4L)) // sel_spdi_cfg3
#define     REG_AIU_SPDIFI_PCPD         (*(volatile DWORD *)(REG_AIU_BASE+0x2a8L)) // sel_rd_PcPd
#define     REG_AIU_SPDIFI_STATUS0      (*(volatile DWORD *)(REG_AIU_BASE+0x2acL)) // sel_status0
#define     REG_AIU_SPDIFI_STATUS1      (*(volatile DWORD *)(REG_AIU_BASE+0x2b0L)) // rd_channel_sts
#define     REG_AIU_SPDIFI_MACRO        (*(volatile DWORD *)(REG_AIU_BASE+0x2b4L)) // rd_macro
#define     REG_AIU_SPDIFI_MRADDR       (*(volatile DWORD *)(REG_AIU_BASE+0x2b8L)) // set_mraddr
#define     REG_AIU_SPDIFI_CHANNELSTS0  (*(volatile DWORD *)(REG_AIU_BASE+0x2bcL))
#define     REG_AIU_SPDIFI_CHANNELSTS1  (*(volatile DWORD *)(REG_AIU_BASE+0x2c0L))
#define     REG_AIU_SPDIFI_CHANNELSTS2  (*(volatile DWORD *)(REG_AIU_BASE+0x2c4L))
#define     REG_AIU_SPDIFI_CHANNELSTS3  (*(volatile DWORD *)(REG_AIU_BASE+0x2c8L))
#define     REG_AIU_SPDIFI_CHANNELSTS4  (*(volatile DWORD *)(REG_AIU_BASE+0x2ccL))
#define     REG_AIU_SPDIFI_CHANNELSTS5  (*(volatile DWORD *)(REG_AIU_BASE+0x2d0L))
#define     REG_AIU_SPDIFI_DECL         (*(volatile DWORD *)(REG_AIU_BASE+0x2d4L))
#define     REG_AIU_SPDIFI_DECR         (*(volatile DWORD *)(REG_AIU_BASE+0x2d8L))
#define     REG_AIU_SPDIFI_RAWSUBFRAME  (*(volatile DWORD *)(REG_AIU_BASE+0x2dcL))
#define     REG_AIU_WRFMT               (*(volatile DWORD *)(REG_AIU_BASE+0x2e0L)) //sel_wrfmt
#define     REG_AIU_DAC_CONFG0          (*(volatile DWORD *)(REG_AIU_BASE+0x2e8L)) //sel_dac_confg0 
#define     REG_AIU_CG_CLKENA           (*(volatile DWORD *)(REG_AIU_BASE+0x2ecL)) //sel_cg clk enable
#define     REG_AIU_CG_CLKDIS           (*(volatile DWORD *)(REG_AIU_BASE+0x2f0L)) //sel_cg clk disable
#endif

#ifdef CT909G_IC_SYSTEM
#define     REG_AIU_TXLFADE             (*(volatile DWORD *)(REG_AIU_BASE+0x260L)) // sel_txlfade
#define     REG_AIU_TXRFADE             (*(volatile DWORD *)(REG_AIU_BASE+0x264L)) // sel_txrfade
#define     REG_AIU_TXLSFADE            (*(volatile DWORD *)(REG_AIU_BASE+0x268L)) // sel_txlsfade
#define     REG_AIU_TXRSFADE            (*(volatile DWORD *)(REG_AIU_BASE+0x26CL)) // sel_txrsfade
#define     REG_AIU_TXCFADE             (*(volatile DWORD *)(REG_AIU_BASE+0x270L)) // sel_txcfade
#define     REG_AIU_TXLFEFADE           (*(volatile DWORD *)(REG_AIU_BASE+0x274L)) // sel_txlfefade
#define     REG_AIU_TXLMIXFADE          (*(volatile DWORD *)(REG_AIU_BASE+0x278L)) // sel_txlmixfade
#define     REG_AIU_TXRMIXFADE          (*(volatile DWORD *)(REG_AIU_BASE+0x27CL)) // sel_txrmixfade
#define     REG_AIU_CLRTIR              (*(volatile DWORD *)(REG_AIU_BASE+0x280L)) // clear timer interrupt routine bit
#define     REG_AIU_SPORT_CONFG1        (*(volatile DWORD *)(REG_AIU_BASE+0x284L)) // sel_acfg1 
#define     REG_AIU_PCMSCALELR          (*(volatile DWORD *)(REG_AIU_BASE+0x288L)) // pcmrd scale lr
#define     REG_AIU_PCMSCALELRS         (*(volatile DWORD *)(REG_AIU_BASE+0x28CL)) // pcmrd scale lrs
#define     REG_AIU_PCMSCALECLFE        (*(volatile DWORD *)(REG_AIU_BASE+0x290L)) // pcmrd scale clfe
#define     REG_AIU_PCMSCALELRMIX       (*(volatile DWORD *)(REG_AIU_BASE+0x294L)) // pcmrd scale lrmix
#define     REG_AIU_PCMFADEOUTIN        (*(volatile DWORD *)(REG_AIU_BASE+0x298L)) // pcmrd fade out/fade in 
// AIUWR ports
#define     REG_AIU_AIUWR               (*(volatile DWORD *)(REG_AIU_BASE+0x29CL)) // aiuwr
// AIURD ports
#define     REG_AIU_AIURD               (*(volatile DWORD *)(REG_AIU_BASE+0x2A0L)) // regrd
// SPDIFOUT ports
#define     REG_AIU_SPDIFO_CONFG        (*(volatile DWORD *)(REG_AIU_BASE+0x2A4L)) // sel_spdifout_acfg0
#define     REG_AIU_SPDIFO_FRAMECNT     (*(volatile DWORD *)(REG_AIU_BASE+0x2A8L)) // blkcnt_rd and pcmrd bitcnt
#define     REG_AIU_TXLSPDIF            (*(volatile DWORD *)(REG_AIU_BASE+0x2ACL)) // sel_txl
#define     REG_AIU_TXRSPDIF            (*(volatile DWORD *)(REG_AIU_BASE+0x2B0L)) // sel_txr 
#define     REG_AIU_CHSTS               (*(volatile DWORD *)(REG_AIU_BASE+0x2B4L)) //  sel_chsts (channel status)
//reg174 reserved
// #define     REG_AIU_REG174_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2B8L)) //reserved 

//DAC ports (175~176)
#define     REG_AIU_DAC_CONFG0          (*(volatile DWORD *)(REG_AIU_BASE+0x2BCL)) //sel_dac_confg0 
#define     REG_AIU_DAC_APBEN           (*(volatile DWORD *)(REG_AIU_BASE+0x2C0L)) //sel_dac_apben 
//reg177 reserved 
// #define     REG_AIU_REG177_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2C4L)) //reserved

//ICG ports (178~179)
#define     REG_AIU_CG_CLKENA           (*(volatile DWORD *)(REG_AIU_BASE+0x2c8L)) //sel_cg clk enable
#define     REG_AIU_CG_CLKDIS           (*(volatile DWORD *)(REG_AIU_BASE+0x2ccL)) //sel_cg clk disable
//wrfmt port (180)
#define     REG_AIU_WRFMT               (*(volatile DWORD *)(REG_AIU_BASE+0x2d0L)) //sel_wrfmt
// reserved ports (181~222)
/*
#define     REG_AIU_REG181_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2d4L))
#define     REG_AIU_REG182_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2d8L))
#define     REG_AIU_REG183_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2dcL))
*/
#endif

// reserved ports
/*
#define     REG_AIU_REG184_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2e0L))
#define     REG_AIU_REG185_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2e4L))
#define     REG_AIU_REG186_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2e8L))
#define     REG_AIU_REG187_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2ecL))
#define     REG_AIU_REG188_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2f0L))
#define     REG_AIU_REG189_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2fcL))
#define     REG_AIU_REG190_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2f8L))
#define     REG_AIU_REG191_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x2fcL))
#define     REG_AIU_REG192_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x300L))
#define     REG_AIU_REG193_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x304L))
#define     REG_AIU_REG194_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x308L))
#define     REG_AIU_REG195_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x30cL))
#define     REG_AIU_REG196_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x310L))
#define     REG_AIU_REG197_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x314L))
#define     REG_AIU_REG198_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x318L))
#define     REG_AIU_REG199_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x31cL))
#define     REG_AIU_REG200_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x320L))
#define     REG_AIU_REG201_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x324L))
#define     REG_AIU_REG202_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x328L))
#define     REG_AIU_REG203_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x32cL))
#define     REG_AIU_REG204_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x330L))
#define     REG_AIU_REG205_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x334L))
#define     REG_AIU_REG206_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x338L))
#define     REG_AIU_REG207_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x33cL))
#define     REG_AIU_REG208_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x340L))
#define     REG_AIU_REG209_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x344L))
#define     REG_AIU_REG210_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x348L))
#define     REG_AIU_REG211_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x34cL))
#define     REG_AIU_REG212_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x350L))
#define     REG_AIU_REG213_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x354L))
#define     REG_AIU_REG214_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x358L))
#define     REG_AIU_REG215_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x35cL))
#define     REG_AIU_REG216_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x360L))
#define     REG_AIU_REG217_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x364L))
#define     REG_AIU_REG218_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x368L))
#define     REG_AIU_REG219_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x36cL))
#define     REG_AIU_REG220_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x370L))
#define     REG_AIU_REG221_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x374L))
#define     REG_AIU_REG222_ADDR         (*(volatile DWORD *)(REG_AIU_BASE+0x378L))
*/
#define     REG_AIU_DMA_STATUS          (*(volatile DWORD *)(REG_AIU_BASE+0x37cL))
// GR ports (224~255))
#define     REG_AIU_GR(addr)            (*(volatile DWORD *)(REG_AIU_BASE+0x380+((BYTE)(addr)<<2)))

// define enable bit for REG_AIU_AIUEN
#define AIU_DMA_ENABLE_UNPACKER0        (0x00000001)
#define AIU_DMA_ENABLE_PCM_OUT          (0x00000002)
#define AIU_DMA_ENABLE_SPDIF_OUT        (0x00000004)
#define AIU_DMA_ENABLE_AIUWR            (0x00000008)
#define AIU_DMA_ENABLE_MIC_INPUT        (0x00000010)
#define AIU_DMA_ENABLE_SPDIF_INPUT      (0x00000020)
#define AIU_DMA_ENABLE_UNPACKER1        (0x00000040)
#define AIU_DMA_ENABLE_AIURD            (0x00000080)

#ifdef __cplusplus
        }
#endif

#endif ///
