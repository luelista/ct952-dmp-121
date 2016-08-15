//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2004,
//          D300, all right reserved.
//
//      Product : CT909S
//
//      Date    : 2004.05.03
//      Author  : Cheertek (J500 Chuan)
//      Purpose : 
//      Sources : 
//  **************************************************************************

#ifndef  __CTKAV_H__
#define  __CTKAV_H__

//#define PROC2_AUDIO
// #if defined(PROC2_AUDIO) || defined(TEST909S)
#ifndef     _PLATFORM_H
#include "platform.h"
#include "oswrap.h"
#endif // PROC2_AUDIO

#ifdef __cplusplus
        extern  "C"     {
#endif

// wyc2.16-909S, move DRAM config define to internal.h

// Platform Type
#define     SIMULATION  1
#define     TSIM_LEON   2
#define     LEON_FPGA   3
#define     ECOS_OS     4

// Chuan, specify platform
//#define   PLATFORM    SIMULATION
//#define   PLATFORM    TSIM_LEON
//#define   PLATFORM    LEON_FPGA
#define     PLATFORM    ECOS_OS

#define     CT909_IO_START              (0x80000000L)
#define     DATA_WIDTH                  (4)     // 4 bytes width

#include "ctkav_aiu.h"
#include "ctkav_atapi.h"
#include "ctkav_biu.h"
#include "ctkav_disp.h"
#include "ctkav_gpu.h"
#include "ctkav_jpu.h"
#include "ctkav_mcu.h"
#include "ctkav_platform.h"
#include "ctkav_spu.h"
#include "ctkav_tve.h"
#include "ctkav_vdec.h"
#include "ctkav_ts.h"
#include "ctkav_jpg.h"
#include "ctkav_sdc.h"
#include "ctkav_msc.h"
#include "ctkav_nfc.h"

#ifdef PROC2_AUDIO
#include "ecos.h"
#include "debug.h"  // Chuan, Include debug module
#endif // PROC2_AUDIO

#ifdef __cplusplus
        }
#endif

#endif
