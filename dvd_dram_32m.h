//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2005-2007,
//          D300, all right reserved.
//
//      Product : CT909S
//
//      Date    : 2006.01.16
//      Author  : Cheertek (J500 Chuan)
//      Purpose : 
//      Sources : 
//  **************************************************************************
#ifndef  __DVD_DRAM_32M_H__
#define  __DVD_DRAM_32M_H__


////////////////////////////////////////////////////////////////////////////////////////
//
//  DRAM Allocation Table
//
////////////////////////////////////////////////////////////////////////////////////////
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_32)
#define     DS_DVD_DRAM_END             (CT909_DRAM_START+0x400000L)

////////////////////////////////////////////////////////////////////////////////////////
//  PROC1/ PROC2 Code
#define     DS_PROC2_STARTADDR          (CT909_DRAM_START+0x002000L)
#define     DS_PROC2_SP                 (CT909_DRAM_START+0x031F00L)
#define     PROC2_SP                    (*(volatile DWORD *)(0x800007D4L))  // GR(21)
#define     PROC2_STARTADR              (*(volatile DWORD *)(0x800007D8L))  // GR(22)

/////////////////////////////////////////////////////////////////////////////////
// PCM Buffer
#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_64)
#define     DS_PCMBUF_ST                (CT909_DRAM_START+0x71F000L)
#define     DS_PCMBUF_END               (CT909_DRAM_START+0x72C000L)       // 13K DW

#define     DS_LANGUAGESTUDY_BUFF_ST    (CT909_DRAM_START+0x71D000L)
#define     DS_LANGUAGESTUDY_BUFF_END   (CT909_DRAM_START+0x71F000L)       // 2K DW

#else
#define     DS_PCMBUF_ST                (CT909_DRAM_START+0x135000L)
#define     DS_PCMBUF_END               (CT909_DRAM_START+0x142000L)       // 13K DW
#endif

/////////////////////////////////////////////////////////////////////////////////
// ZIP2006 Buffer
#define     DS_UNZIP_BUF_ST             (CT909_DRAM_START+0x142000L)
#define     DS_UNZIP_BUF_END            (CT909_DRAM_START+0x145000L)        // 3K DW
#ifdef CT909R_IC_SYSTEM
#define     DS_UNZIP_STARTADDR          (0x800064b0L)
#endif
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
#define     DS_UNZIP_STARTADDR          (0x80006f00L)
#endif

/////////////////////////////////////////////////////////////////////////////////
// NV Buffer: 0.5K DW
// USB Buffer: 0.5K DW
#define     NV_SIZE                     (0x800) // 2048 bytes                    
#define     DS_NV_BUF_ADDR_ST           (CT909_DRAM_START+0x001800L)
#define     DS_NV_BUF_ADDR_END          (DS_NV_BUF_ADDR_ST + NV_SIZE)

#define     DS_USB_BUFF_ST              (CT909_DRAM_START+0x001800L)
#define     DS_USB_BUFF_END             (CT909_DRAM_START+0x002000L)    // 0.5K DW

// wyc2.78-909P,
#define     DS_USB_MEMPOOL_ST_HOST      (CT909_DRAM_START+0x39C000L)
#define     DS_USB_MEMPOOL_END_HOST     (CT909_DRAM_START+0x3AC000L)    // 16K DW
#define     DS_USB_MEMPOOL_ST_DEVICE    (CT909_DRAM_START+0x37C000L)
#define     DS_USB_MEMPOOL_END_DEVICE   (CT909_DRAM_START+0x3AC000L)    // 48K DW
#define     DS_USB_DEVICE_SCSI_POOL_ST  (CT909_DRAM_START+0x344000L)    // 16K DW
#define     DS_USB_DEVICE_SCSI_POOL_END (CT909_DRAM_START+0x354000L)

/////////////////////////////////////////////////////////////////////////////////
// FRAME Buffer: 456K DW 0x40145000 ~ 0x4030D000
// CD Detect buffer use FRAME 0.
#define     DS_FRAMEBUF_ST              (CT909_DRAM_START+0x145000L)
#define     DS_FRAMEBUF_SIZE            (0x98000L)
#define     DS_FRAMEBUF0_ADDR_ST        (DS_FRAMEBUF_ST)
#define     DS_FRAMEBUF0_ADDR_END       (DS_FRAMEBUF0_ADDR_ST+DS_FRAMEBUF_SIZE)
#define     DS_FRAMEBUF1_ADDR_ST        (DS_FRAMEBUF0_ADDR_END)
#define     DS_FRAMEBUF1_ADDR_END       (DS_FRAMEBUF1_ADDR_ST+DS_FRAMEBUF_SIZE)
#define     DS_FRAMEBUF2_ADDR_ST        (DS_FRAMEBUF1_ADDR_END)
#define     DS_FRAMEBUF2_ADDR_END       (DS_FRAMEBUF2_ADDR_ST+DS_FRAMEBUF_SIZE)

#define     DS_FRAMEBUF_SIZE_FBR        (0x72C00L)
#define     DS_FRAMEBUF0_ADDR_ST_FBR    (DS_FRAMEBUF_ST)
#ifdef SUPPORT_BFR_MODE
#define     DS_FRAMEBUF0_ADDR_END_FBR   (DS_FRAMEBUF0_ADDR_ST_FBR+(DS_FRAMEBUF_SIZE_FBR/2))
#else
#define     DS_FRAMEBUF0_ADDR_END_FBR   (DS_FRAMEBUF0_ADDR_ST_FBR+DS_FRAMEBUF_SIZE_FBR)
#endif
#define     DS_FRAMEBUF1_ADDR_ST_FBR    (DS_FRAMEBUF0_ADDR_END_FBR)
#define     DS_FRAMEBUF1_ADDR_END_FBR   (DS_FRAMEBUF1_ADDR_ST_FBR+DS_FRAMEBUF_SIZE_FBR)
#define     DS_FRAMEBUF2_ADDR_ST_FBR    (DS_FRAMEBUF1_ADDR_END_FBR)
#define     DS_FRAMEBUF2_ADDR_END_FBR   (DS_FRAMEBUF2_ADDR_ST_FBR+DS_FRAMEBUF_SIZE_FBR)

// Audio Only/ JPEG Preview
#define     DS_FRAMEBUF_ST_MM           (DS_FRAMEBUF_ST)
#define     DS_FRAMEBUF_END_MM          (DS_FRAMEBUF2_ADDR_END)	// 457.5K DW

// JPEG Slide Show Mode/ JPEG Thumbnail Mode
#define     DS_FRAMEBUF_ST_SLIDESHOW    (DS_FRAMEBUF_ST_MM)
#define     DS_FRAMEBUF_END_SLIDESHOW   (DS_FRAMEBUF_END_MM)
#define     DS_FRAMEBUF_ST_THUMBNAIL    (DS_FRAMEBUF_ST_MM)
#define     DS_FRAMEBUF_END_THUMBNAIL   (DS_FRAMEBUF_END_MM)

// CDG Frame Buffer
#define     DS_FRAMEBUF_ST_CDG          (DS_FRAMEBUF1_ADDR_ST)
#define     DS_FRAMEBUF_END_CDG         (DS_FRAMEBUF2_ADDR_END)

#define     Y_MAXSIZE_A8C0              (0xA8C0L)
#define     Y_MAXSIZE_PAL               (0x1950L)       // 720x576x8/32 >> 4
#define     Y_MAXSIZE_NTSC              (0x1520L)       // 720x480x8/32 >> 4
#define     Y_MAXSIZE_PAL_FBR           (0x1320L)       // 544x576x8/32 >> 4

// Note: protected file INFOFILTER uses the following defines
#define     DS_CD_DETECT_BUF_ST         (DS_FRAMEBUF0_ADDR_ST)
#define     DS_CD_DETECT_BUF_END        (DS_FRAMEBUF0_ADDR_END)
#define     DS_CD_DETECT_BUF_SECTORS    ((DS_CD_DETECT_BUF_END-DS_CD_DETECT_BUF_ST)/(2352+96))  
#define     DS_CD_DETECT_BUF_END_NONCDG (DS_CD_DETECT_BUF_ST+DS_CD_DETECT_BUF_SECTORS*2352)

/////////////////////////////////////////////////////////////////////////////////
// OSD Font Table: 502 Fonts 32K DW (0x403AC000L ~ 0x403BD000L) 
// OSD Region: 620x540x8 82K DW     (0x403BD000L ~ 0x40400000L)
#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_64)
#define     DS_OSDSTR_ST                (CT909_DRAM_START+0x41F000L)
#define     DS_OSDSTR_END               (CT909_DRAM_START+0x424000L)  // 5K DW
#else
#define     DS_OSDSTR_ST                (CT909_DRAM_START+0x12F000L)
#define     DS_OSDSTR_END               (CT909_DRAM_START+0x132000L)  // 3K DW
#endif
#define     DS_OSD_FONTTABLE_ST         (CT909_DRAM_START+0x3AC000L) 
#define     DS_OSD_FONTTABLE_END        (CT909_DRAM_START+0x3BD000L)
#define     DS_OSDFRAME_ST              (CT909_DRAM_START+0x3BD000L)  
#define     DS_OSDFRAME_END             (CT909_DRAM_START+0x400000L)  // 67K DW
#define     DS_OSDFRAME_ST_MM           (DS_OSDFRAME_ST)  
#define     DS_OSDFRAME_END_MM          (DS_OSDFRAME_END)  // 67K DW
#define     DS_SP_OSD_ST                (CT909_DRAM_START+0x33B000L)
#define     DS_SP_OSD_END               (CT909_DRAM_START+0x340000L)  // 5K DW
/////////////////////////////////////////////////////////////////////////////////
// FW Buffer:
#define     DS_FW_BUFFER_ST             (CT909_DRAM_START+0x30D000L)
#define     DS_FW_BUFFER_END            (CT909_DRAM_START+0x322000L)       // 21K DW
#define     DS_FW_BUFFER_ST_MM          (CT909_DRAM_START+0x30D000L)
#define     DS_FW_BUFFER_END_MM         (CT909_DRAM_START+0x32F000L)       // 21K DW
/////////////////////////////////////////////////////////////////////////////////
// SP Buffer: 13K DW
#define     SPU_SIZE                    (0xD000)        // SPU Max Size: 0xD000 (53220 bytes)
#define     DS_SP1BUF_ADDR_ST           (CT909_DRAM_START+0x32F000L)
#define     DS_SP1BUF_ADDR_END          (DS_SP1BUF_ADDR_ST + SPU_SIZE)
#define     DS_SP2BUF_ADDR_ST           (CT909_DRAM_START+0x322000L)
#define     DS_SP2BUF_ADDR_END          (DS_SP2BUF_ADDR_ST + SPU_SIZE)
#define     DS_SP1BUF_ADDR_ST_MM        (DS_SP1BUF_ADDR_ST)
#define     DS_SP1BUF_ADDR_END_MM       (DS_SP1BUF_ADDR_END)    // 13K DW.

/////////////////////////////////////////////////////////////////////////////////
// UserData Buffer: Use SP1 as UserData Buffer
#define     DS_USERDATA_BUF_ST          (DS_SP1BUF_ADDR_ST)
#define     DS_USERDATA_BUF_END         (DS_SP1BUF_ADDR_END)    // 13K DW
#define     DS_USERDATA_BUF_ST_MM       (DS_SP1BUF_ADDR_ST_MM)
#define     DS_USERDATA_BUF_END_MM      (DS_SP1BUF_ADDR_END_MM) // 13K DW

/////////////////////////////////////////////////////////////////////////////////
// Audio Buffer
#define     DS_AD0BUF_ST                (CT909_DRAM_START+0x33C000L)
#define     DS_AD0BUF_END               (CT909_DRAM_START+0x340000L)  // 4K DW
#define     DS_AD1BUF_ST                (CT909_DRAM_START+0x340000L)
#define     DS_AD1BUF_END               (CT909_DRAM_START+0x344000L)  // 4K DW

#define     DS_AD0BUF_ST_AVI            (CT909_DRAM_START+0x340000L)
#define     DS_AD0BUF_END_AVI           (CT909_DRAM_START+0x35F000L)  // 31K DW
#define     DS_AD1BUF_ST_AVI            (CT909_DRAM_START+0x35F000L)
#define     DS_AD1BUF_END_AVI           (CT909_DRAM_START+0x364000L)  // 5K DW

#define     DS_AD0BUF_ST_MM_MOTION      (CT909_DRAM_START+0x322000L)
#define     DS_AD0BUF_END_MM_MOTION     (CT909_DRAM_START+0x342000L)  // 32K DW
#define     DS_AD1BUF_ST_MM_MOTION      (CT909_DRAM_START+0x342000L)
#define     DS_AD1BUF_END_MM_MOTION     (CT909_DRAM_START+0x346000L)  // 4K DW

#define     DS_AD0BUF_ST_MM             (CT909_DRAM_START+0x322000L)
#define     DS_AD0BUF_END_MM            (CT909_DRAM_START+0x375000L)  // 83K DW
#define     DS_AD1BUF_ST_MM             (CT909_DRAM_START+0x375000L)
#define     DS_AD1BUF_END_MM            (CT909_DRAM_START+0x37A000L)  // 5K DW

/////////////////////////////////////////////////////////////////////////////////
// Video Buffer
#define     DS_VDBUF_ST                 (CT909_DRAM_START+0x344000L)
#define     DS_VDBUF_END                (CT909_DRAM_START+0x38E000L)  // 74K DW
#define     DS_VDBUF_ST_AVI             (CT909_DRAM_START+0x364000L)
#define     DS_VDBUF_END_AVI            (CT909_DRAM_START+0x388000L)  // 36K DW
#define     DS_VDBUF_ST_MM_MOTION       (CT909_DRAM_START+0x346000L)
#define     DS_VDBUF_END_MM_MOTION      (CT909_DRAM_START+0x38E000L)  // 69K DW
#define     DS_VDBUF_ST_MM              (CT909_DRAM_START+0x37A000L)
#define     DS_VDBUF_END_MM             (CT909_DRAM_START+0x38E000L)  // 17K DW

/////////////////////////////////////////////////////////////////////////////////
// SP Char-Based Input/Output Buffer/ AVI Index Buffer: 3K DW
#define     DS_CHAR_BASED_SP_OUTBUF_ST  (CT909_DRAM_START+0x322000L)
#define     DS_CHAR_BASED_SP_OUTBUF_END (CT909_DRAM_START+0x33B000L)       // 25K DW

#define     DS_AVI_INDEX_TBL_ST         (CT909_DRAM_START+0x388000L)
#define     DS_AVI_INDEX_TBL_END        (CT909_DRAM_START+0x38B000L)        // 3 DW
#define     DS_AVI_INDEX_TBL_LEN        (DS_AVI_INDEX_TBL_END - DS_AVI_INDEX_TBL_ST)
#define     DS_AVI_TMP_INDEX_TBL_ST     (DS_VDBUF_ST_AVI)
#define     DS_AVI_TMP_INDEX_TBL_END    (DS_VDBUF_END_AVI)
#define     DS_AVI_TMP_INDEX_TBL_LEN    (DS_AVI_TMP_INDEX_TBL_END - DS_AVI_TMP_INDEX_TBL_ST)

/////////////////////////////////////////////////////////////////////////////////
// Servo Buffer:
#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_64)
#define     DS_INPUT_BITBUF_ST          (CT909_DRAM_START+0x424000L)

#ifdef SUPPORT_ANTISHOCK
#if (DECODER_SYSTEM == DVD909S_EVAL)
#define     DS_INPUT_BITBUF_END         (CT909_DRAM_START+0x618000L)  // 500K DW: 50 Blocks
#else
#define     DS_INPUT_BITBUF_END         (CT909_DRAM_START+0x71C000L)  // 760K DW: 76 Blocks
#endif  // #if (DECODER_SYSTEM == DVD909S_EVAL)
#else
#define     DS_INPUT_BITBUF_END         (CT909_DRAM_START+0x47E000L)  // 90K DW: 9 Blocks
#endif  // #ifdef SUPPORT_ANTISHOCK

#define     DS_INPUT_BITBUF_ST_MM       (DS_INPUT_BITBUF_ST)
#define     DS_INPUT_BITBUF_END_MM      (DS_INPUT_BITBUF_END)
#define     DS_INPUT_BITBUF_ST_AVI      (DS_INPUT_BITBUF_ST)
#define     DS_INPUT_BITBUF_END_AVI     (DS_INPUT_BITBUF_END)
#define     DS_INPUT_BITBUF_ST_MOTION   (DS_INPUT_BITBUF_ST)
#define     DS_INPUT_BITBUF_END_MOTION  (DS_INPUT_BITBUF_END)
#else

#define     DS_INPUT_BITBUF_ST          (CT909_DRAM_START+0x38E000L)
#define     DS_INPUT_BITBUF_END         (CT909_DRAM_START+0x3AC000L)  // 30K DW: 3 Blocks
#define     DS_INPUT_BITBUF_ST_MM       (DS_INPUT_BITBUF_ST)
#define     DS_INPUT_BITBUF_END_MM      (DS_INPUT_BITBUF_END)  // 30K DW: 3 Blocks
#define     DS_INPUT_BITBUF_ST_AVI      (CT909_DRAM_START+0x398000L)
#define     DS_INPUT_BITBUF_END_AVI     (CT909_DRAM_START+0x3AC000L)  // 20K DW: 2 Blocks
#define     DS_INPUT_BITBUF_ST_MOTION   (DS_INPUT_BITBUF_ST)
#define     DS_INPUT_BITBUF_END_MOTION  (DS_INPUT_BITBUF_END)  // 30K DW: 3 Blocks

#define     DS_INPUT_BITBUF_ST_SMALL          (CT909_DRAM_START+0x38E000L)
#define     DS_INPUT_BITBUF_END_SMALL         (CT909_DRAM_START+0x39C000L)  // 14K DW
#define     DS_INPUT_BITBUF_ST_MM_SMALL       (DS_INPUT_BITBUF_ST_SMALL)
#define     DS_INPUT_BITBUF_END_MM_SMALL      (DS_INPUT_BITBUF_END_SMALL)  // 14K DW
#define     DS_INPUT_BITBUF_ST_AVI_SMALL      (CT909_DRAM_START+0x398000L)
#define     DS_INPUT_BITBUF_END_AVI_SMALL     (CT909_DRAM_START+0x39C000L)  // 4K DW
#define     DS_INPUT_BITBUF_ST_MOTION_SMALL   (DS_INPUT_BITBUF_ST_SMALL)
#define     DS_INPUT_BITBUF_END_MOTION_SMALL  (DS_INPUT_BITBUF_END_SMALL)  // 14K DW
#endif
/////////////////////////////////////////////////////////////////////////////////
// Debug Buffer:
#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_64)
#define     DS_DEBUG_BUF_ST             (CT909_DRAM_START+0x398000L)
#define     DS_DEBUG_BUF_END            (CT909_DRAM_START+0x39C000L)        // 4K DW
#else
#define     DS_DEBUG_BUF_ST             (CT909_DRAM_START+0x132000L)
#define     DS_DEBUG_BUF_END            (CT909_DRAM_START+0x135000L)        // 3K DW
// #define     DS_DEBUG_BUF_ST             (CT909_DRAM_START+0x400000L)
// #define     DS_DEBUG_BUF_END            (CT909_DRAM_START+0x480000L)        // 128K DW
#endif
/////////////////////////////////////////////////////////////////////////////////
// SideInfo Buffer:
#define     DS_SIDEINFO_BUF_ST          (CT909_DRAM_START+0x38B000L)
#define     DS_SIDEINFO_BUF_END         (CT909_DRAM_START+0x398000L)        // 13K DW

/////////////////////////////////////////////////////////////////////////////////
// For AP Usage
#define     DS_INPUT_BITBUF_ST_AP       (CT909_DRAM_START+0x168000L)
#define     DS_INPUT_BITBUF_END_AP      (CT909_DRAM_START+0x1A4000L)  // 60K DW: 6 Blocks
#define     DS_OSD_FONTTABLE_ST_AP      (CT909_DRAM_START+0x1A4000L) 
#define     DS_OSD_FONTTABLE_END_AP     (CT909_DRAM_START+0x1C4000L)  // 32K DW
#define     DS_OSDFRAME_ST_AP           (CT909_DRAM_START+0x1C4000L)  
#define     DS_OSDFRAME_END_AP          (CT909_DRAM_START+0x216000L)  // 82K DW
#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_64)
#define     DS_USERDATA_BUF_ST_AP       (CT909_DRAM_START+0x3AD000L)  // Need avoid overlap w/ DS_USB_MEMPOOL_ST/ DS_USB_MEMPOOL_END
#define     DS_USERDATA_BUF_END_AP      (CT909_DRAM_START+0x800000L)  // 1107K DW
#define     DS_AP_INFO_AREA             (CT909_DRAM_START+0x3AC000L)
#else
#define     DS_USERDATA_BUF_ST_AP       (CT909_DRAM_START+0x217000L)
#define     DS_USERDATA_BUF_END_AP      (CT909_DRAM_START+0x39C000L)  // 389K DW
#define     DS_AP_INFO_AREA             (CT909_DRAM_START+0x216000L)
#endif

#define     DS_AP_CODE_AREA             (DS_USERDATA_BUF_ST_AP)

/////////////////////////////////////////////////////////////////////////////////
// Radio Buffer:
#ifndef SUPPORT_ANTISHOCK
#define     DS_RADIO_BMPDATA_ST         (CT909_DRAM_START+0x355000L)  // 67K DW
#define     DS_RADIO_BMPDATA_END        (CT909_DRAM_START+0x398000L)
#endif

#endif // (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_32)

#ifdef __cplusplus
        }
#endif

#endif // __DVD_DRAM_32M_H__
