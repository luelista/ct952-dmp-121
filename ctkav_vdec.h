//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2004-2007,
//          J000, all right reserved.
//
//      Product : CT909/ CT909S/ CT909P
//
//      Date    : 2006.11.03
//      Author  : Cheertek (J500 Chuan)
//      Purpose : Provide the register definition of VDEC
//      Sources : 
//  **************************************************************************

#ifndef __CTKAV_VDEC_H__
#define __CTKAV_VDEC_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

#define     REG_VLD_BASE                (CT909_IO_START+0x2080L)
#define     REG_VLD_SHO                 (REG_VLD_BASE-0x4L)
#define     REG_VLD_SHO1                (*(volatile DWORD *)(REG_VLD_BASE+0x0L))    // 0x2080
#define     REG_VLD_SHO2                (*(volatile DWORD *)(REG_VLD_BASE+0x4L))    // 0x2084
#define     REG_VLD_SHO3                (*(volatile DWORD *)(REG_VLD_BASE+0x8L))    // 0x2088
#define     REG_VLD_SHO4                (*(volatile DWORD *)(REG_VLD_BASE+0xCL))    // 0x208C
#define     REG_VLD_SHO5                (*(volatile DWORD *)(REG_VLD_BASE+0x10L))   // 0x2090
#define     REG_VLD_SHO6                (*(volatile DWORD *)(REG_VLD_BASE+0x14L))   // 0x2094
#define     REG_VLD_SHO7                (*(volatile DWORD *)(REG_VLD_BASE+0x18L))   // 0x2098
#define     REG_VLD_SHO8                (*(volatile DWORD *)(REG_VLD_BASE+0x1CL))   // 0x209C
#define     REG_VLD_SHO9                (*(volatile DWORD *)(REG_VLD_BASE+0x20L))   // 0x20A0
#define     REG_VLD_SHO10               (*(volatile DWORD *)(REG_VLD_BASE+0x24L))   // 0x20A4
#define     REG_VLD_SHO11               (*(volatile DWORD *)(REG_VLD_BASE+0x28L))   // 0x20A8
#define     REG_VLD_SHO12               (*(volatile DWORD *)(REG_VLD_BASE+0x2CL))   // 0x20AC
#define     REG_VLD_SHO13               (*(volatile DWORD *)(REG_VLD_BASE+0x30L))   // 0x20B0
#define     REG_VLD_SHO14               (*(volatile DWORD *)(REG_VLD_BASE+0x34L))   // 0x20B4
#define     REG_VLD_SHO15               (*(volatile DWORD *)(REG_VLD_BASE+0x38L))   // 0x20B8
#define     REG_VLD_SHO16               (*(volatile DWORD *)(REG_VLD_BASE+0x3CL))   // 0x20BC
#define     REG_VLD_SHO17               (*(volatile DWORD *)(REG_VLD_BASE+0x40L))   // 0x20C0
#define     REG_VLD_SHO18               (*(volatile DWORD *)(REG_VLD_BASE+0x44L))   // 0x20C4
#define     REG_VLD_SHO19               (*(volatile DWORD *)(REG_VLD_BASE+0x48L))   // 0x20C8
#define     REG_VLD_SHO20               (*(volatile DWORD *)(REG_VLD_BASE+0x4CL))   // 0x20CC
#define     REG_VLD_SHO21               (*(volatile DWORD *)(REG_VLD_BASE+0x50L))   // 0x20D0
#define     REG_VLD_SHO22               (*(volatile DWORD *)(REG_VLD_BASE+0x54L))   // 0x20D4
#define     REG_VLD_SHO23               (*(volatile DWORD *)(REG_VLD_BASE+0x58L))   // 0x20D8
#define     REG_VLD_SHO24               (*(volatile DWORD *)(REG_VLD_BASE+0x5CL))   // 0x20DC
#define     REG_VLD_SHO25               (*(volatile DWORD *)(REG_VLD_BASE+0x60L))   // 0x20E0
#define     REG_VLD_SHO26               (*(volatile DWORD *)(REG_VLD_BASE+0x64L))   // 0x20E4
#define     REG_VLD_SHO27               (*(volatile DWORD *)(REG_VLD_BASE+0x68L))   // 0x20E8
#define     REG_VLD_SHO28               (*(volatile DWORD *)(REG_VLD_BASE+0x6CL))   // 0x20EC
#define     REG_VLD_SHO29               (*(volatile DWORD *)(REG_VLD_BASE+0x70L))   // 0x20F0
#define     REG_VLD_SHO30               (*(volatile DWORD *)(REG_VLD_BASE+0x74L))   // 0x20F4
#define     REG_VLD_SHO31               (*(volatile DWORD *)(REG_VLD_BASE+0x78L))   // 0x20F8
#define     REG_VLD_SHO32               (*(volatile DWORD *)(REG_VLD_BASE+0x7CL))   // 0x20FC
#define     REG_VLD_MB_CTL0             (*(volatile DWORD *)(REG_VLD_BASE+0x80L))   // 0x2100
#define     REG_VLD_VALUE1              (*(volatile DWORD *)(REG_VLD_BASE+0x84L))   // 0x2104
#define     REG_VLD_VALUE2              (*(volatile DWORD *)(REG_VLD_BASE+0x88L))   // 0x2108
#define     REG_VLD_MB_CTL1             (*(volatile DWORD *)(REG_VLD_BASE+0x8CL))   // 0x210C
#define     REG_VLD_MBINT_CTL           (*(volatile DWORD *)(REG_VLD_BASE+0x140L))  // 0x21C0
#define     REG_VLD_INFO                (*(volatile DWORD *)(REG_VLD_BASE+0x180L))  // 0x2200
#define     REG_VLD_CTL                 (*(volatile DWORD *)(REG_VLD_BASE+0x184L))  // 0x2204
#define     REG_VLD_STATUS              (*(volatile DWORD *)(REG_VLD_BASE+0x188L))  // 0x2208
#define     REG_VLD_MV0                 (*(volatile DWORD *)(REG_VLD_BASE+0x18CL))  // 0x220C
#define     REG_VLD_MV1                 (*(volatile DWORD *)(REG_VLD_BASE+0x190L))  // 0x2210
#define     REG_VLD_MV2                 (*(volatile DWORD *)(REG_VLD_BASE+0x194L))  // 0x2214
#define     REG_VLD_MV3                 (*(volatile DWORD *)(REG_VLD_BASE+0x198L))  // 0x2218
#define     REG_VLD_PMV0                (*(volatile DWORD *)(REG_VLD_BASE+0x19CL))  // 0x221C
#define     REG_VLD_PMV1                (*(volatile DWORD *)(REG_VLD_BASE+0x1A0L))  // 0x2220
#define     REG_VLD_PMV2                (*(volatile DWORD *)(REG_VLD_BASE+0x1A4L))  // 0x2224
#define     REG_VLD_PMV3                (*(volatile DWORD *)(REG_VLD_BASE+0x1A8L))  // 0x2228
#define     REG_VLD_MV_CTL              (*(volatile DWORD *)(REG_VLD_BASE+0x1ACL))  // 0x222C
#define     REG_VLD_MV_CTL1             (*(volatile DWORD *)(REG_VLD_BASE+0x1B0L))  // 0x2230
#define     REG_VLD_FSC                 (*(volatile DWORD *)(REG_VLD_BASE+0x1B4L))  // 0x2234
#define     REG_VLD_BITS_CNT            (*(volatile DWORD *)(REG_VLD_BASE+0x1B8L))  // 0x2238

#define     REG_DEQ_BASE                (CT909_IO_START+0x2280L)
#define     REG_DEQ_CTL                 (*(volatile DWORD *)(REG_DEQ_BASE+0x00L))   // 0x2280
#define     REG_DEQ_ZRAM_ADR            (*(volatile DWORD *)(REG_DEQ_BASE+0x04L))   // 0x2284
#define     REG_DEQ_QMEM_I              (*(volatile DWORD *)(REG_DEQ_BASE+0x08L))   // 0x2288
#define     REG_DEQ_QMEM_NI             (*(volatile DWORD *)(REG_DEQ_BASE+0x0CL))   // 0x228C
#define     REG_DEQ_QRAM_V_ADR          (*(volatile DWORD *)(REG_DEQ_BASE+0x10L))   // 0x2290
#define     REG_DEQ_DC_DEF_311          (*(volatile DWORD *)(REG_DEQ_BASE+0x14L))   // 0x2294

#define     REG_DEQ_JPEG_BASE           (CT909_IO_START+0x2300L)
#define     REG_DEQ_JPEG_COEFF0         (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x00L))   // 0x2300
#define     REG_DEQ_JPEG_COEFF1         (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x04L))   // 0x2304
#define     REG_DEQ_JPEG_COEFF2         (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x08L))   // 0x2308
#define     REG_DEQ_JPEG_COEFF3         (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x0CL))   // 0x230C
#define     REG_DEQ_JPEG_COEFF4         (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x10L))   // 0x2310
#define     REG_DEQ_JPEG_COEFF5         (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x14L))   // 0x2314
#define     REG_DEQ_JPEG_COEFF6         (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x18L))   // 0x2318
#define     REG_DEQ_JPEG_COEFF7         (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x1CL))   // 0x231C
#define     REG_DEQ_JPEG_COEFF8         (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x20L))   // 0x2320
#define     REG_DEQ_JPEG_COEFF9         (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x24L))   // 0x2324
#define     REG_DEQ_JPEG_COEFF10        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x28L))   // 0x2328
#define     REG_DEQ_JPEG_COEFF11        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x2CL))   // 0x232C
#define     REG_DEQ_JPEG_COEFF12        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x30L))   // 0x2330
#define     REG_DEQ_JPEG_COEFF13        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x34L))   // 0x2334
#define     REG_DEQ_JPEG_COEFF14        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x38L))   // 0x2338
#define     REG_DEQ_JPEG_COEFF15        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x3CL))   // 0x233C
#define     REG_DEQ_JPEG_COEFF16        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x40L))   // 0x2340
#define     REG_DEQ_JPEG_COEFF17        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x44L))   // 0x2344
#define     REG_DEQ_JPEG_COEFF18        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x48L))   // 0x2348
#define     REG_DEQ_JPEG_COEFF19        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x4CL))   // 0x234C
#define     REG_DEQ_JPEG_COEFF20        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x50L))   // 0x2350
#define     REG_DEQ_JPEG_COEFF21        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x54L))   // 0x2354
#define     REG_DEQ_JPEG_COEFF22        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x58L))   // 0x2358
#define     REG_DEQ_JPEG_COEFF23        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x5CL))   // 0x235C
#define     REG_DEQ_JPEG_COEFF24        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x60L))   // 0x2360
#define     REG_DEQ_JPEG_COEFF25        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x64L))   // 0x2364
#define     REG_DEQ_JPEG_COEFF26        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x68L))   // 0x2368
#define     REG_DEQ_JPEG_COEFF27        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x6CL))   // 0x236C
#define     REG_DEQ_JPEG_COEFF28        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x70L))   // 0x2370
#define     REG_DEQ_JPEG_COEFF29        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x74L))   // 0x2374
#define     REG_DEQ_JPEG_COEFF30        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x78L))   // 0x2378
#define     REG_DEQ_JPEG_COEFF31        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x7CL))   // 0x237C
#define     REG_DEQ_JPEG_COEFF32        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x80L))   // 0x2380
#define     REG_DEQ_JPEG_COEFF33        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x84L))   // 0x2384
#define     REG_DEQ_JPEG_COEFF34        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x88L))   // 0x2388
#define     REG_DEQ_JPEG_COEFF35        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x8CL))   // 0x238C
#define     REG_DEQ_JPEG_COEFF36        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x90L))   // 0x2390
#define     REG_DEQ_JPEG_COEFF37        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x94L))   // 0x2394
#define     REG_DEQ_JPEG_COEFF38        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x98L))   // 0x2398
#define     REG_DEQ_JPEG_COEFF39        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0x9CL))   // 0x239C
#define     REG_DEQ_JPEG_COEFF40        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xA0L))   // 0x23A0
#define     REG_DEQ_JPEG_COEFF41        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xA4L))   // 0x23A4
#define     REG_DEQ_JPEG_COEFF42        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xA8L))   // 0x23A8
#define     REG_DEQ_JPEG_COEFF43        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xACL))   // 0x23AC
#define     REG_DEQ_JPEG_COEFF44        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xB0L))   // 0x23B0
#define     REG_DEQ_JPEG_COEFF45        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xB4L))   // 0x23B4
#define     REG_DEQ_JPEG_COEFF46        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xB8L))   // 0x23B8
#define     REG_DEQ_JPEG_COEFF47        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xBCL))   // 0x23BC
#define     REG_DEQ_JPEG_COEFF48        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xC0L))   // 0x23C0
#define     REG_DEQ_JPEG_COEFF49        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xC4L))   // 0x23C4
#define     REG_DEQ_JPEG_COEFF50        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xC8L))   // 0x23C8
#define     REG_DEQ_JPEG_COEFF51        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xCCL))   // 0x23CC
#define     REG_DEQ_JPEG_COEFF52        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xD0L))   // 0x23D0
#define     REG_DEQ_JPEG_COEFF53        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xD4L))   // 0x23D4
#define     REG_DEQ_JPEG_COEFF54        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xD8L))   // 0x23D8
#define     REG_DEQ_JPEG_COEFF55        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xDCL))   // 0x23DC
#define     REG_DEQ_JPEG_COEFF56        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xE0L))   // 0x23E0
#define     REG_DEQ_JPEG_COEFF57        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xE4L))   // 0x23E4
#define     REG_DEQ_JPEG_COEFF58        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xE8L))   // 0x23E8
#define     REG_DEQ_JPEG_COEFF59        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xECL))   // 0x23EC
#define     REG_DEQ_JPEG_COEFF60        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xF0L))   // 0x23F0
#define     REG_DEQ_JPEG_COEFF61        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xF4L))   // 0x23F4
#define     REG_DEQ_JPEG_COEFF62        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xF8L))   // 0x23F8
#define     REG_DEQ_JPEG_COEFF63        (*(volatile DWORD *)(REG_DEQ_JPEG_BASE+0xFCL))   // 0x23FC

#define     REG_MC_BASE                 (CT909_IO_START+0x2110L)
#define     DRAM_FADR_BASE              (CT909_IO_START+0x2110L)
#define     REG_MC_DRAM_FADR0           (*(volatile DWORD *)(REG_MC_BASE))          // 0x2110
#define     REG_MC_DRAM_FADR1           (*(volatile DWORD *)(REG_MC_BASE+0x04L))    // 0x2114
#define     REG_MC_DRAM_FADR2           (*(volatile DWORD *)(REG_MC_BASE+0x08L))    // 0x2118
#define     REG_MC_DRAM_FADR3           (*(volatile DWORD *)(REG_MC_BASE+0x0CL))    // 0x211C
#define     REG_MC_DRAM_FADR4           (*(volatile DWORD *)(REG_MC_BASE+0x10L))    // 0x2120
#define     REG_MC_DRAM_BADR0           (*(volatile DWORD *)(REG_MC_BASE+0x14L))    // 0x2124
#define     REG_MC_DRAM_BADR1           (*(volatile DWORD *)(REG_MC_BASE+0x18L))    // 0x2128
#define     REG_MC_DRAM_BADR2           (*(volatile DWORD *)(REG_MC_BASE+0x1CL))    // 0x212C
#define     REG_MC_DRAM_BADR3           (*(volatile DWORD *)(REG_MC_BASE+0x20L))    // 0x2130
#define     REG_MC_DRAM_BADR4           (*(volatile DWORD *)(REG_MC_BASE+0x24L))    // 0x2134
#define     REG_MC_GMC_RESL             (*(volatile DWORD *)(REG_MC_BASE+0x28L))    // 0x2138

#define     REG_MC_CTL                  (*(volatile DWORD *)(REG_MC_BASE+0x2F0L))   // 0x2400
#define     REG_MC_VHSIZE_Y             (*(volatile DWORD *)(REG_MC_BASE+0x2F4L))   // 0x2404
#ifdef CT909R_IC_SYSTEM
#define     REG_MC_PJ12W                (*(volatile DWORD *)(REG_MC_BASE+0x2F8L))   // 0x2408
#define     REG_MC_PJ12R                (*(volatile DWORD *)(REG_MC_BASE+0x2FCL))   // 0x240C
#endif
#define     REG_MC_REFPIC               (*(volatile DWORD *)(REG_MC_BASE+0x300L))   // 0x2410
#define     REG_MC_WBPIC                (*(volatile DWORD *)(REG_MC_BASE+0x304L))   // 0x2414
#define     REG_MC_G_LUM_X0             (*(volatile DWORD *)(REG_MC_BASE+0x308L))   // 0x2418
#define     REG_MC_G_LUM_XX             (*(volatile DWORD *)(REG_MC_BASE+0x30CL))   // 0x241C
#define     REG_MC_G_LUM_XY             (*(volatile DWORD *)(REG_MC_BASE+0x310L))   // 0x2420
#define     REG_MC_G_LUM_Y0             (*(volatile DWORD *)(REG_MC_BASE+0x314L))   // 0x2424
#define     REG_MC_G_LUM_YX             (*(volatile DWORD *)(REG_MC_BASE+0x318L))   // 0x2428
#define     REG_MC_G_LUM_YY             (*(volatile DWORD *)(REG_MC_BASE+0x31CL))   // 0x242C
#define     REG_MC_G_LUM_SR             (*(volatile DWORD *)(REG_MC_BASE+0x320L))   // 0x2430
#define     REG_MC_G_CHR_X0             (*(volatile DWORD *)(REG_MC_BASE+0x324L))   // 0x2434
#define     REG_MC_G_CHR_XX             (*(volatile DWORD *)(REG_MC_BASE+0x328L))   // 0x2438
#define     REG_MC_G_CHR_XY             (*(volatile DWORD *)(REG_MC_BASE+0x32CL))   // 0x243C
#define     REG_MC_G_CHR_Y0             (*(volatile DWORD *)(REG_MC_BASE+0x330L))   // 0x2440
#define     REG_MC_G_CHR_YX             (*(volatile DWORD *)(REG_MC_BASE+0x334L))   // 0x2444
#define     REG_MC_G_CHR_YY             (*(volatile DWORD *)(REG_MC_BASE+0x338L))   // 0x2448
#define     REG_MC_G_CHR_SR             (*(volatile DWORD *)(REG_MC_BASE+0x33CL))   // 0x244C
#define     REG_MC_CURPOS               (*(volatile DWORD *)(REG_MC_BASE+0x340L))   // 0x2450
#define     REG_MC_YRSIZE               (*(volatile DWORD *)(REG_MC_BASE+0x344L))   // 0x2454
#define     REG_MC_YSTRIP               (*(volatile DWORD *)(REG_MC_BASE+0x348L))   // 0x2458
#define     REG_MC_WB_YOFF              (*(volatile DWORD *)(REG_MC_BASE+0x34CL))   // 0x245C
#ifdef CT909R_IC_SYSTEM
#define     REG_MC_XY_ADR_Y             (*(volatile DWORD *)(REG_MC_BASE+0x350L))   // 0x2460
#define     REG_MC_XY_ADR_UV            (*(volatile DWORD *)(REG_MC_BASE+0x354L))   // 0x2464
#endif

#define     REG_SIDEINFO_BASE           (CT909_IO_START+0x2240L)
#define     REG_SIDEINFO_ROW_RDADR      (*(volatile DWORD *)(REG_SIDEINFO_BASE+0x0L))    // 0x2240
#define     REG_SIDEINFO_PIC_RDADR      (*(volatile DWORD *)(REG_SIDEINFO_BASE+0x4L))    // 0x2244
#define     REG_SIDEINFO_ROW_WRADR      (*(volatile DWORD *)(REG_SIDEINFO_BASE+0x8L))    // 0x2248
#define     REG_SIDEINFO_PIC_WRADR      (*(volatile DWORD *)(REG_SIDEINFO_BASE+0xCL))    // 0x224C
#define     REG_SIDEINFO_ROW_DEFADR     (*(volatile DWORD *)(REG_SIDEINFO_BASE+0x10L))   // 0x2250
#define     REG_SIDEINFO_RDMBCNT        (*(volatile DWORD *)(REG_SIDEINFO_BASE+0x14L))   // 0x2254
#define     REG_SIDEINFO_WRMBCNT        (*(volatile DWORD *)(REG_SIDEINFO_BASE+0x18L))   // 0x2258
#define     REG_SIDEINFO_CTL            (*(volatile DWORD *)(REG_SIDEINFO_BASE+0x1CL))   // 0x225C
#define     REG_SIDEINFO_VALUE          (*(volatile DWORD *)(REG_SIDEINFO_BASE+0x20L))   // 0x2260
#define     REG_VDEC_CG_EN              (*(volatile DWORD *)(REG_MC_BASE+0x360L))   // 0x2470
#define     REG_VDEC_CG_CLR             (*(volatile DWORD *)(REG_MC_BASE+0x364L))   // 0x2474
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
#define     REG_VDEC_BUG_SEL            (*(volatile DWORD *)(REG_MC_BASE+0x368L))   // 0x2478
#endif

// MB_CTL0
#define     VLD_INTRA                   (0x1L)          // MB_CTL0[0]
#define     VLD_MB_SKIP                 (0x2L)          // MB_CTL0[1]
#define     VLD_USE_INTRA_DC_VLC        (0x4L)          // MB_CTL0[2]
#define     VLD_CBP5_0                  (0x1F8L)        // MB_CTL0[8:3]
#define     VLD_FIRST_MB                (0x200L)        // MB_CTL0[9]
#define     VLD_ACPRED_EN               (0x400L)        // MB_CTL0[10]
#define     VLD_QUANT_SCALE             (0x3F800L)      // MB_CTL0[17:11]
#define     VLD_NEW_UP                  (0x10000L)      // MB_CTL0[16]
#define     VLD_NEW_PACK                (0x20000L)      // MB_CTL0[17]
#define     VLD_DCT_TYPE                (0x40000L)      // MB_CTL0[18]
#define     VLD_FIELD_PRED              (0x80000L)      // MB_CTL0[19]

// MB_CTL1
#define     VLD_FORWARD                 (0x0004L)       // MB_CTL1[2]    
#define     VLD_BACKWARD                (0x0008L)       // MB_CTL1[3]
#define     VLD_FOURMV                  (0x0010L)       // MB_CTL1[4]
#define     VLD_MCCASE0                 (0x0020L)       // MB_CTL1[5]    
#define     VLD_MCCASE1                 (0x0040L)       // MB_CTL1[6]  
#define     VLD_MCCASE2                 (0x0080L)       // MB_CTL1[7]    
#define     VLD_MCCASE3                 (0x0100L)       // MB_CTL1[8]   
#define     VLD_MCCASE4                 (0x0200L)       // MB_CTL1[9]
#define     VLD_MCCASE5                 (0x0400L)       // MB_CTL1[10]   
#define     VLD_MCCASE6                 (0x0800L)       // MB_CTL1[11]    
#define     VLD_MCCASE7                 (0x1000L)       // MB_CTL1[12]    
#define     VLD_MCCASE8                 (0x2000L)       // MB_CTL1[13]
#define     VLD_GMC_EN                  (0x4000L)       // MB_CTL1[14]
#define     VLD_DIRECT                  (0x8000L)       // MB_CTL1[15]
#define     VLD_FOR_TOP                 (0x10000L)      // MB_CTL1[16]    
#define     VLD_FOR_BOT                 (0x20000L)      // MB_CTL1[17]
#define     VLD_BACK_TOP                (0x40000L)      // MB_CTL1[18]
#define     VLD_BACK_BOT                (0x80000L)      // MB_CTL1[19]
#define     VLD_MCCASE                  (0x3FE0L)       // MB_CTL1[13:5]

// VLD_CTL
#define     VLD_DEC                     (0x1L)          // VLD_CTL[0]   
#define     VLD_EXE                     (0x2L)          // VLD_CTL[1]
#define     VLD_MBAI                    (0x4L)          // VLD_CTL[2]
#define     VLD_MBTYPE                  (0x8L)          // VLD_CTL[3]
#define     VLD_CBP                     (0x10L)         // VLD_CTL[4]
#define     VLD_MV_MP2                  (0x20L)         // VLD_CTL[5]
#define     VLD_MCBPC                   (0x40)          // VLD_CTL[6]
#define     VLD_CBPY                    (0x80)          // VLD_CTL[7]
#define     VLD_MODB                    (0x100)         // VLD_CTL[8]
#define     VLD_BMBTYPE                 (0x200)         // VLD_CTL[9]
#define     VLD_QUANT                   (0x400)         // VLD_CTL[10]
#define     VLD_MV_MP4                  (0x800)         // VLD_CTL[11]
#define     VLD_CBPI_311                (0x1000)        // VLD_CTL[12]
#define     VLD_CBPP_311                (0x2000)        // VLD_CTL[13]
#define     VLD_MV_311                  (0x4000)        // VLD_CTL[14]
#define     VLD_NUM                     (15)            // VLD_CTL[20:15]
#define     VLD_FNS                     (0x200000L)     // VLD_CTL[21]
#define     VLD_RESET_MV                (0x800000L)     // VLD_CTL[23]
#define     VLD_UPDATE_MV               (0x1000000L)    // VLD_CTL[24]
#define     VLD_SWAP_MV                 (0x2000000L)    // VLD_CTL[25]
#define     VLD_MV_GO                   (0x4000000L)    // VLD_CTL[26]
#define     VLD_GMV_GO                  (0x8000000L)    // VLD_CTL[27]
#define     VLD_FNS_SPECIFY             (0x10000000L)   // VLD_CTL[28]
#define     VLD_BP_MODE                 (0x20000000L)   // VLD_CTL[29]
#define     VLD_LOCALRESET              (0x80000000L)   // VLD_CTL[31]
#define	    VLD_INTRA_CBPCY             (22)

// VLD_INFO
#define     VLD_JPEG_MODE               (0x1L)          // VLD_INFO[0]
#define     VLD_MPEG1_MODE              (0x2L)          // VLD_INFO[1]
#define     VLD_MPEG2_MODE              (0x4L)          // VLD_INFO[2]
#define     VLD_MPEG4_MODE              (0x8L)          // VLD_INFO[3]
#define     VLD_DIVX311_MODE            (0x10L)         // VLD_INFO[4]
#define     VLD_PICTYPE                 (0x60L)         // VLD_INFO[6:5]
#define     VLD_INTERLACED              (0x80L)         // VLD_INFO[7]
#define     VLD_DATA_PART               (0x100L)        // VLD_INFO[8]
#define     VLD_REVERSIBLE              (0x200L)        // VLD_INFO[9]
#define     VLD_SHORT_HEADER            (0x400L)        // VLD_INFO[10]
#define     VLD_INTRA_VLC_FORMAT        (0x100000L)     // VLD_INFO[20]
#define     VLD_BITS_REQ_EN             (0x200000L)     // VLD_INFO[21]
#define     VLD_NFL16                   (0x400000L)     // VLD_INFO[22]
#define     VLD_SINFO_EN                (0x800000L)     // VLD_INFO[23]

// VLD_STATUS
#define     VLD_VAL                     (0x000FFFL)     // VLD_STATUS[11:0]
#define     VLD_BAR_BITNUM              (0x01F000L)     // VLD_STATUS[16:12]
#define     VLD_N_BYTEALIGN             (0x020000L)     // VLD_STATUS[17]
#define     VLD_FSTART_CODE             (0x040000L)     // VLD_STATUS[18]
#define     VLD_FMV_DONE                (0x1000000L)    // VLD_STATUS[24]
#define     VLD_BMV_DONE                (0x2000000L)    // VLD_STATUS[25]
#define     VLD_MB_RDY                  (0x4000000L)    // VLD_STATUS[26]
#define     VLD_LIMBIT16                (0x8000000L)    // VLD_STATUS[27]

#define     VLD_SEL32                   (0x020000L)     // VLD_STATUS[17]
#define     VLD_MCBPC                   (0x40)
#define     VLD_CBPY                    (0x80)
#define     VLD_MODB                    (0x100)
#define     VLD_BMBTYPE                 (0x200)
#define     VLD_DBQUANT                 (0x400)
#define     VLD_MV_MP4                  (0x800)
#define     VLD_CBPI_311                (0x1000)
#define     VLD_CBPP_311                (0x2000)
#define     VLD_MV_311                  (0x4000)
#define     VLD_INTRA_CBPCY             (22)

// MBINT_CTL
#define     VLD_HDR_DONE                (0x1L)          // MBINT_CTL[0]
#define     VLD_RL_DONE                 (0x2L)          // MBINT_CTL[1]   
#define     VLD_PLA_ERR                 (0x4L)          // MBINT_CTL[2]    
#define     VLD_MC_DONE                 (0x8L)          // MBINT_CTL[3]    
#define     VLD_REGB_EN                 (0x400L)        // MBINT_CTL[10]   
#define     VLD_JPEG_ST                 (0x800L)        // MBINT_CTL[11]

// DEQ_CTL
#define     DEQ_VOP_START               (0x1L)
#define     DEQ_ALTERNATE_SCAN          (0x2L)          // DEQ_CTL[1]
#define     DEQ_INTRA_DC_PRECISION      (0xCL)          // DEQ_CTL[3:2]
#define     DEQ_RESET_DC_PREC           (0x10L)         // DEQ_CTL[4]
#define     DEQ_RESET_QRAM_COUNTER      (0x20L)         // DEQ_CTL[5]

// SIDEINFO_CTL
#define     SINFO_RDEN                  (0x1L)
#define     SINFO_WREN_PIC              (0x2L)

// SIDEINFO_VALUE
#define     SINFO_SIRD_READY            (0x0400L)
#define     SINFO_SIWR_READY            (0x0800L)
#define     SINFO_WR_DONE               (0x1000L)

// DRAM_xADR
#define     MC_FADR0                    (REG_MC_DRAM_FADR0+0)
#define     MC_FADR1                    (REG_MC_DRAM_FADR0+4)
#define     MC_FADR2                    (REG_MC_DRAM_FADR0+8)
#define     MC_FADR3                    (REG_MC_DRAM_FADR0+12)
#define     MC_FADR4                    (REG_MC_DRAM_FADR0+16)
#define     MC_BADR0                    (REG_MC_DRAM_FADR0+20)
#define     MC_BADR1                    (REG_MC_DRAM_FADR0+24)
#define     MC_BADR2                    (REG_MC_DRAM_FADR0+28)
#define     MC_BADR3                    (REG_MC_DRAM_FADR0+32)
#define     MC_BADR4                    (REG_MC_DRAM_FADR0+36)

// MC_CTL
#define     MC_XY_CALC                  (0x10000)

///////////////////////////////////////////////////////////////////////////////
// SRAM Allocation Table: 2048 BYTE
#define     REG_SRAM_BASE               (0xb0000000)

// Overlap Area for Decoder (MP4/MPG/JPG): 0 ~ 399
// MP4 Decoder Used: 400 bytes
#define     MP4_ROM_VARS_ADDR           (REG_SRAM_BASE)             // Size: CT909P - 28; CT909S - 32
#define     MP4_STATE_ROM_ADDR          (REG_SRAM_BASE + 0x20)      // Size: CT909P - 288; CT909S - 280
#define     B_DIR_MV_ADDR               (REG_SRAM_BASE + 0x140)      // Size: 80

// MPG Decoder Used: 64 bytes
#define     MP2_INFO_VARS_ADDR          (REG_SRAM_BASE)             // Size: 52
#define     SLICE_VARS_ADDR             (REG_SRAM_BASE + 0x34)      // Size: 20

// JPG Decoder Used: 0 bytes


// Common Area: 400 ~ 1024
#define     REG_SRAM_PLAYMODE           (*(volatile BYTE *)(REG_SRAM_BASE+0x190L))
// watchdog counter
#define     REG_SRAM_WATCHDOG           (*(volatile DWORD *)(REG_SRAM_BASE+0x194L))
#define     REG_SRAM_DISPLINE           (*(volatile DWORD *)(REG_SRAM_BASE+0x198L))


#ifdef CT909G_IC_SYSTEM
#define     VLD_UPKDATA_WIDTH           (4)
#endif

#if defined(CT909R_IC_SYSTEM) || defined(CT909P_IC_SYSTEM)
#define     VLD_UPKDATA_WIDTH           (3)
#endif

///////////////////////////////////////////////////////////////////////////////
#define     SHOW_BITS(N)                (*(volatile DWORD *)(REG_VLD_BASE-4L+((BYTE)(N)<<2)))
#define     FLUSH_BITS(N)                   \
{                                           \
    REG_VLD_CTL = ((N<<VLD_NUM)|VLD_EXE);   \
}

#ifdef __cplusplus
        }
#endif

#endif
