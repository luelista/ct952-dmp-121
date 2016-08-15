//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2004,
//          D300, all right reserved.
//
//      Product : CT909
//
//      Date    : 2004.05.11
//      Author  : Cheertek (D300 WYC)
//      Purpose : Provide the register definition of ATAPI
//      Sources : 
//  **************************************************************************
#ifndef __CTKAV_ATAPI_H__
#define __CTKAV_ATAPI_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

#define	REG_ATAPI_BASE         (CT909_IO_START+0x2980L)
#define REG_ATAPI_CMDCTL       (*(volatile DWORD *)(REG_ATAPI_BASE+0x0L))  
#define REG_ATAPI_TIME0        (*(volatile DWORD *)(REG_ATAPI_BASE+0x4L))  
#define REG_ATAPI_TIME1        (*(volatile DWORD *)(REG_ATAPI_BASE+0x8L))  
#define REG_ATAPI_STATUS       (*(volatile DWORD *)(REG_ATAPI_BASE+0xcL)) 
#define REG_ATAPI_TIME2        (*(volatile DWORD *)(REG_ATAPI_BASE+0x10L))  
#define REG_ATAPI_DMACTL       (*(volatile DWORD *)(REG_ATAPI_BASE+0x14L))
#define REG_ATAPI_DMALEN       (*(volatile DWORD *)(REG_ATAPI_BASE+0x18L))

#ifdef __cplusplus
        }
#endif

#endif ///
