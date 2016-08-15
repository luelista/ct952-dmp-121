//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2006,
//          J500, all right reserved.
//
//      Product : CT909, CT909P
//
//      Author  : Cheertek (J500 Senshong)
//      Purpose : Provide the register definition of JPG
//      Sources : 
//  **************************************************************************

#ifndef __CTKAV_JPG_H__
#define __CTKAV_JPG_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

#if defined(CT909P_IC_SYSTEM ) || defined(CT909G_IC_SYSTEM)

#define     REG_JPG_BASE                          (CT909_IO_START+0x2268L)
#define     REG_JPG_SADR_BASE               (CT909_IO_START+0x2128L)
#define     REG_JPG_DATA1                       (*(volatile DWORD *)(REG_JPG_BASE+0x0L))    // 0x2268 JPEG max code value(maxcode)
#define     REG_JPG_DATA2                       (*(volatile DWORD *)(REG_JPG_BASE+0x4L))    // 0x226c JPEG max code pointer(valptr)
#define     REG_JPG_CTL0                          (*(volatile DWORD *)(REG_JPG_BASE+0x8L))    // 0x2270 JPEG control register0
#define     REG_JPG_CTL1                          (*(volatile DWORD *)(REG_JPG_BASE+0xcL))    // 0x2274 JPEG control reigster1
#define     REG_JPG_HRAM                         (*(volatile DWORD *)(REG_JPG_BASE+0x10L))    // 0x2278 JPEG huffman value
#ifdef CT909G_IC_SYSTEM
#define     REG_JPG_HV		            (*(volatile DWORD *)(CT909_IO_START +0x2124L))    //0x2124 JPG Y,U,V hs and vs value
#else
#define     REG_JPG_HV		                 (*(volatile DWORD *)(CT909_IO_START +0x2400L))    //0x2400
#endif
#define     REG_JPG_SADRY                        (*(volatile DWORD *)(REG_JPG_SADR_BASE+0x0L))    // 0x2128 The start address of line mb stripe for Y (4 byte)
#define     REG_JPG_SADRU                        (*(volatile DWORD *)(REG_JPG_SADR_BASE+0x4L))    // 0x212c The start address of line mb stripe for U (4 byte)
#define     REG_JPG_SADRV                        (*(volatile DWORD *)(REG_JPG_SADR_BASE+0x8L))    // 0x2130 The start address of line mb stripe for V (4 byte)
#define     REG_DEQ_QMEM_V                     (*(volatile DWORD *)(CT909_IO_START+0x2290L))

//JPG_CTL0
#define     JPG_MODE                (0x200000L)//jpg_mode for baseline MUC
//JPG_CTL1
#define			JPG_DPCM_RESET		(0x1L)
#define     JPG_INDEX_RESET    (0x2L)//index reset
#define     JPG_VLD_GO              (0x10L)
#define     JPG_BP                      (0x20L) //skip FF00 => FF
#define     JPG_SKIP_MB             (0x40L)
 
#endif

#ifdef __cplusplus
        }
#endif

#endif

