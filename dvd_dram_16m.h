//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2005-2007,
//          D300, all right reserved.
//
//      Product : CT909S/CT909R
//
//      Date    : 2006.06.27
//      Author  : Cheertek (J500 Chuan)
//      Purpose :
//      Sources :
//  **************************************************************************
#ifndef  __DVD_DRAM_16M_H__
#define  __DVD_DRAM_16M_H__


////////////////////////////////////////////////////////////////////////////////////////
//
//  DRAM Allocation Table
//
////////////////////////////////////////////////////////////////////////////////////////
#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
#define     DS_DVD_DRAM_END             (CT909_DRAM_START+0x200000L)

////////////////////////////////////////////////////////////////////////////////////////
//  PROC1/ PROC2 Code
#define     DS_PROC2_STARTADDR          (CT909_DRAM_START+0x002000L)
#define     DS_PROC2_SP                 (CT909_DRAM_START+0x01CF00L)
#define     PROC2_SP                    (*(volatile DWORD *)(0x800007D4L))  // GR(21)
#define     PROC2_STARTADR              (*(volatile DWORD *)(0x800007D8L))  // GR(22)

/////////////////////////////////////////////////////////////////////////////////
// PCM Buffer
#define     DS_PCMBUF_ST                (CT909_DRAM_START+0x04F000L)
#define     DS_PCMBUF_END               (CT909_DRAM_START+0x05C000L)       // 13K DW

/////////////////////////////////////////////////////////////////////////////////
// ZIP2006 Buffer
#define     DS_UNZIP_BUF_ST             (CT909_DRAM_START+0x04C000L)
#define     DS_UNZIP_BUF_END            (CT909_DRAM_START+0x04F000L)        // 3K DW
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
#define     DS_USB_MEMPOOL_ST_HOST      (CT909_DRAM_START+0x1F0000L)
#define     DS_USB_MEMPOOL_END_HOST     (CT909_DRAM_START+0x200000L)    // 16K DW
#define     DS_USB_MEMPOOL_ST_DEVICE    (CT909_DRAM_START+0x1D0000L)
#define     DS_USB_MEMPOOL_END_DEVICE   (CT909_DRAM_START+0x200000L)    // 48K DW
#define     DS_USB_DEVICE_SCSI_POOL_ST  (CT909_DRAM_START+0x18C800L)     // 16K DW
#define     DS_USB_DEVICE_SCSI_POOL_END (CT909_DRAM_START+0x19C800L)


/////////////////////////////////////////////////////////////////////////////////
// FRAME Buffer: 287K DW 0x40064000 ~ 0x40183800 for FBR + BFR
// #define     DS_FRAMEBUF_ST              (CT909_DRAM_START+0x300000L)
#define     DS_FRAMEBUF_ST              (CT909_DRAM_START+0x065000L)
#define     DS_FRAMEBUF_SIZE            (0x98000L)
#define     DS_FRAMEBUF0_ADDR_ST        (DS_FRAMEBUF_ST)
#define     DS_FRAMEBUF0_ADDR_END       (DS_FRAMEBUF0_ADDR_ST+DS_FRAMEBUF_SIZE)
#define     DS_FRAMEBUF1_ADDR_ST        (DS_FRAMEBUF0_ADDR_END)
#define     DS_FRAMEBUF1_ADDR_END       (DS_FRAMEBUF1_ADDR_ST+DS_FRAMEBUF_SIZE)
#define     DS_FRAMEBUF2_ADDR_ST        (DS_FRAMEBUF1_ADDR_END)
#define     DS_FRAMEBUF2_ADDR_END       (DS_FRAMEBUF2_ADDR_ST+DS_FRAMEBUF_SIZE)

#define     DS_FRAMEBUF_SIZE_FBR        (0x72C00L)
#define     DS_FRAMEBUF0_ADDR_ST_FBR    (DS_FRAMEBUF_ST)                // 0x40065000
#ifdef SUPPORT_BFR_MODE
#define     DS_FRAMEBUF0_ADDR_END_FBR   (DS_FRAMEBUF0_ADDR_ST_FBR+(DS_FRAMEBUF_SIZE_FBR/2))
#else
#define     DS_FRAMEBUF0_ADDR_END_FBR   (DS_FRAMEBUF0_ADDR_ST_FBR+DS_FRAMEBUF_SIZE_FBR)
#endif
#define     DS_FRAMEBUF1_ADDR_ST_FBR    (DS_FRAMEBUF0_ADDR_END_FBR)     // 0x4009E600
#define     DS_FRAMEBUF1_ADDR_END_FBR   (DS_FRAMEBUF1_ADDR_ST_FBR+DS_FRAMEBUF_SIZE_FBR)
#define     DS_FRAMEBUF2_ADDR_ST_FBR    (DS_FRAMEBUF1_ADDR_END_FBR)     // 0x40111200
#define     DS_FRAMEBUF2_ADDR_END_FBR   (DS_FRAMEBUF2_ADDR_ST_FBR+DS_FRAMEBUF_SIZE_FBR)

#define     DS_FRAMEBUF_SIZE_NTSC_FBR       (0x5FC00L)
#define     DS_FRAMEBUF0_ADDR_ST_NTSC_FBR   (DS_FRAMEBUF_ST)                // 0x40061000
#define     DS_FRAMEBUF0_ADDR_END_NTSC_FBR  (DS_FRAMEBUF0_ADDR_ST_NTSC_FBR+DS_FRAMEBUF_SIZE_NTSC_FBR)
#define     DS_FRAMEBUF1_ADDR_ST_NTSC_FBR   (DS_FRAMEBUF0_ADDR_END_NTSC_FBR)     // 0x400C0C00
#define     DS_FRAMEBUF1_ADDR_END_NTSC_FBR  (DS_FRAMEBUF1_ADDR_ST_NTSC_FBR+DS_FRAMEBUF_SIZE_NTSC_FBR)
#define     DS_FRAMEBUF2_ADDR_ST_NTSC_FBR   (DS_FRAMEBUF1_ADDR_END_NTSC_FBR)     // 0x40120800
#define     DS_FRAMEBUF2_ADDR_END_NTSC_FBR  (DS_FRAMEBUF2_ADDR_ST_NTSC_FBR+DS_FRAMEBUF_SIZE_NTSC_FBR)

// Audio Only/ JPEG Preview
#define     DS_FRAMEBUF_ST_MM           (CT909_DRAM_START+0x0A2000L)
#define     DS_FRAMEBUF_END_MM          (CT909_DRAM_START+0x1A2000L)    // 256K DW

// JPEG Slide Show Mode/ JPEG Thumbnail Mode
#define     DS_FRAMEBUF_ST_SLIDESHOW    (CT909_DRAM_START+0x065000L)
#define     DS_FRAMEBUF_END_SLIDESHOW   (CT909_DRAM_START+0x1AC000L)    // 327K DW
#define     DS_FRAMEBUF_ST_THUMBNAIL    (DS_FRAMEBUF_ST_MM)
#define     DS_FRAMEBUF_END_THUMBNAIL   (DS_FRAMEBUF_END_MM)            // 256K DW

// CDG Frame Buffer
#define     DS_FRAMEBUF_ST_CDG          (DS_FRAMEBUF1_ADDR_ST_FBR)
#define     DS_FRAMEBUF_END_CDG         (DS_FRAMEBUF2_ADDR_END_FBR)

#define     Y_MAXSIZE_A8C0              (0xA8C0L)
#define     Y_MAXSIZE_PAL               (0x1950L)       // 720x576x8/32 >> 4
#define     Y_MAXSIZE_NTSC              (0x1520L)       // 720x480x8/32 >> 4
#define     Y_MAXSIZE_PAL_FBR           (0x1320L)       // 544x576x8/32 >> 4
#define     Y_MAXSIZE_NTSC_FBR          (0xFF0L)        // 544x480x8/32 >> 4

// Note: protected file INFOFILTER uses the following defines
#define     DS_CD_DETECT_BUF_ST         (DS_FRAMEBUF0_ADDR_ST_FBR)
#define     DS_CD_DETECT_BUF_END        (DS_FRAMEBUF0_ADDR_END_FBR)
#define     DS_CD_DETECT_BUF_SECTORS    ((DS_CD_DETECT_BUF_END-DS_CD_DETECT_BUF_ST)/(2352+96))
#define     DS_CD_DETECT_BUF_END_NONCDG (DS_CD_DETECT_BUF_ST+DS_CD_DETECT_BUF_SECTORS*2352)

/////////////////////////////////////////////////////////////////////////////////
// OSD Font Table: 0x4005C000 ~ 0x4005F000 3K DW
// OSD Region: Normal 620x78x4  6K DW       (0x4005F000L ~ 0x40065000L)
//             MM     620x440x8 67K DW      (0x4005F000L ~ 0x400A2000L)
// OSD String: 0x40049E00 ~ 0x4004C000 2.125K DW
#define     DS_OSDSTR_ST                (CT909_DRAM_START+0x049E00L)
#define     DS_OSDSTR_END               (CT909_DRAM_START+0x04C000L)  // 2.125K DW
#define     DS_OSD_FONTTABLE_ST         (CT909_DRAM_START+0x05C000L)
#define     DS_OSD_FONTTABLE_END        (CT909_DRAM_START+0x05F000L)  // 3K DW
#define     DS_OSDFRAME_ST              (CT909_DRAM_START+0x05F000L)
#define     DS_OSDFRAME_END             (CT909_DRAM_START+0x065000L)  // 6K DW
#define     DS_OSDFRAME_ST_MM           (CT909_DRAM_START+0x05F000L)
#define     DS_OSDFRAME_END_MM          (CT909_DRAM_START+0x0A2000L)  // 67K DW
#define     DS_SP_OSD_ST                (CT909_DRAM_START+0x1E6000L)
#define     DS_SP_OSD_END               (CT909_DRAM_START+0x1E9000L)  // 3K DW
/////////////////////////////////////////////////////////////////////////////////
// FW Buffer:
#define     DS_FW_BUFFER_ST             (CT909_DRAM_START+0x1D2800L)
#define     DS_FW_BUFFER_END            (CT909_DRAM_START+0x1D5000L)       // 2.5K DW
#define     DS_FW_BUFFER_ST_MM          (CT909_DRAM_START+0x1CC000L)
#define     DS_FW_BUFFER_END_MM         (CT909_DRAM_START+0x1DC000L)       // 16K DW
/////////////////////////////////////////////////////////////////////////////////
// SP Buffer: 13K DW
#define     SPU_SIZE                    (0xD000)        // SPU Max Size: 0xD000 (53220 bytes)
#define     DS_SP1BUF_ADDR_ST           (CT909_DRAM_START+0x1D5000L)
#define     DS_SP1BUF_ADDR_END          (DS_SP1BUF_ADDR_ST + SPU_SIZE)
#define     DS_SP2BUF_ADDR_ST           (CT909_DRAM_START)
#define     DS_SP2BUF_ADDR_END          (CT909_DRAM_START)
#define     DS_SP1BUF_ADDR_ST_MM        (CT909_DRAM_START+0x1DC000L)
#define     DS_SP1BUF_ADDR_END_MM       (DS_SP1BUF_ADDR_ST_MM + SPU_SIZE)

/////////////////////////////////////////////////////////////////////////////////
// UserData Buffer: Use SP1 as UserData Buffer
#define     DS_USERDATA_BUF_ST          (DS_SP1BUF_ADDR_ST)
#define     DS_USERDATA_BUF_END         (DS_SP1BUF_ADDR_END)        // 13K DW
#define     DS_USERDATA_BUF_ST_MM       (CT909_DRAM_START+0x1DC000L)
#define     DS_USERDATA_BUF_END_MM      (CT909_DRAM_START+0x1E6000L)  // 10K DW

/////////////////////////////////////////////////////////////////////////////////
// Audio Buffer
#define     DS_AD0BUF_ST                (CT909_DRAM_START+0x184800L)
#define     DS_AD0BUF_END               (CT909_DRAM_START+0x188800L)  // 4K DW
#define     DS_AD1BUF_ST                (CT909_DRAM_START+0x188800L)
#define     DS_AD1BUF_END               (CT909_DRAM_START+0x18C800L)  // 4K DW

#define     DS_AD0BUF_ST_AVI            (CT909_DRAM_START+0x184800L)
#define     DS_AD0BUF_END_AVI           (CT909_DRAM_START+0x19A000L)  // 22.5K DW
#define     DS_AD1BUF_ST_AVI            (CT909_DRAM_START+0x19A000L)
#define     DS_AD1BUF_END_AVI           (CT909_DRAM_START+0x19F000L)  // 5K DW

#define     DS_AD0BUF_ST_MM_MOTION      (CT909_DRAM_START+0x184800L)
#define     DS_AD0BUF_END_MM_MOTION     (CT909_DRAM_START+0x188800L)  // 4K DW
#define     DS_AD1BUF_ST_MM_MOTION      (CT909_DRAM_START+0x188800L)
#define     DS_AD1BUF_END_MM_MOTION     (CT909_DRAM_START+0x18C800L)  // 4K DW

#define     DS_AD0BUF_ST_MM             (CT909_DRAM_START+0x1AC000L)
#define     DS_AD0BUF_END_MM            (CT909_DRAM_START+0x1C7000L)  // 27K DW
#define     DS_AD1BUF_ST_MM             (CT909_DRAM_START+0x1C7000L)
#define     DS_AD1BUF_END_MM            (CT909_DRAM_START+0x1CC000L)  // 5K DW

/////////////////////////////////////////////////////////////////////////////////
// Video Buffer
#define     DS_VDBUF_ST                 (CT909_DRAM_START+0x18C800L)
#define     DS_VDBUF_END                (CT909_DRAM_START+0x1D2800L)  // 70K DW
#define     DS_VDBUF_ST_AVI             (CT909_DRAM_START+0x19F000L)
#define     DS_VDBUF_END_AVI            (CT909_DRAM_START+0x1BF000L)  // 32K DW
#define     DS_VDBUF_ST_MM_MOTION       (CT909_DRAM_START+0x18C800L)
#define     DS_VDBUF_END_MM_MOTION      (CT909_DRAM_START+0x1CC000L)  // 63.5K DW
#define     DS_VDBUF_ST_MM              (CT909_DRAM_START+0x1DC000L)
#define     DS_VDBUF_END_MM             (CT909_DRAM_START+0x1EC000L)  // 16K DW

/////////////////////////////////////////////////////////////////////////////////
// SP Char-Based Input/Output Buffer/ AVI Index Buffer: 3K DW
#define     DS_CHAR_BASED_SP_OUTBUF_ST  (CT909_DRAM_START+0x1DC000L)
#define     DS_CHAR_BASED_SP_OUTBUF_END (CT909_DRAM_START+0x1E6000L)       // 10K DW

#define     DS_AVI_INDEX_TBL_ST         (CT909_DRAM_START+0x1E9000L)
#define     DS_AVI_INDEX_TBL_END        (CT909_DRAM_START+0x1EC000L)       // 3 DW
#define     DS_AVI_INDEX_TBL_LEN        (DS_AVI_INDEX_TBL_END - DS_AVI_INDEX_TBL_ST)
#define     DS_AVI_TMP_INDEX_TBL_ST     (DS_VDBUF_ST_AVI)
#define     DS_AVI_TMP_INDEX_TBL_END    (DS_VDBUF_END_AVI)
#define     DS_AVI_TMP_INDEX_TBL_LEN    (DS_AVI_TMP_INDEX_TBL_END - DS_AVI_TMP_INDEX_TBL_ST)

/////////////////////////////////////////////////////////////////////////////////
// Servo Buffer:
// wyc2.76-909P
#define     DS_INPUT_BITBUF_ST          (CT909_DRAM_START+0x1E2000L)
#define     DS_INPUT_BITBUF_END         (CT909_DRAM_START+0x200000L)  // 30K DW: 3 Blocks
#define     DS_INPUT_BITBUF_ST_MM       (CT909_DRAM_START+0x1EC000L)
#define     DS_INPUT_BITBUF_END_MM      (CT909_DRAM_START+0x200000L)  // 20K DW: 2 Blocks
#define     DS_INPUT_BITBUF_ST_AVI      (CT909_DRAM_START+0x1EC000L)
#define     DS_INPUT_BITBUF_END_AVI     (CT909_DRAM_START+0x200000L)  // 20K DW: 2 Blocks
#define     DS_INPUT_BITBUF_ST_MOTION   (CT909_DRAM_START+0x1E2000L)
#define     DS_INPUT_BITBUF_END_MOTION  (CT909_DRAM_START+0x200000L)  // 30K DW: 3 Blocks

#define     DS_INPUT_BITBUF_ST_SMALL          (CT909_DRAM_START+0x1E2000L)
#define     DS_INPUT_BITBUF_END_SMALL         (CT909_DRAM_START+0x1F0000L)  // 14K DW
#define     DS_INPUT_BITBUF_ST_MM_SMALL       (CT909_DRAM_START+0x1EC000L)
#define     DS_INPUT_BITBUF_END_MM_SMALL      (CT909_DRAM_START+0x1F0000L)  // 4K DW
#define     DS_INPUT_BITBUF_ST_AVI_SMALL      (CT909_DRAM_START+0x1EC000L)
#define     DS_INPUT_BITBUF_END_AVI_SMALL     (CT909_DRAM_START+0x1F0000L)  // 4K DW
#define     DS_INPUT_BITBUF_ST_MOTION_SMALL   (CT909_DRAM_START+0x1E2000L)
#define     DS_INPUT_BITBUF_END_MOTION_SMALL  (CT909_DRAM_START+0x1F0000L)  // 14K DW

/////////////////////////////////////////////////////////////////////////////////
// Debug Buffer:
// #define     DS_DEBUG_BUF_ST             (CT909_DRAM_START+0x208000L)
// #define     DS_DEBUG_BUF_END            (CT909_DRAM_START+0x300000L)
#define     DS_DEBUG_BUF_ST             (CT909_DRAM_START+0x04C000L)
#define     DS_DEBUG_BUF_END            (CT909_DRAM_START+0x04F000L)        // 3K DW

/////////////////////////////////////////////////////////////////////////////////
// SideInfo Buffer:
#define     DS_SIDEINFO_BUF_ST          (CT909_DRAM_START+0x1BF000L)
#define     DS_SIDEINFO_BUF_END         (CT909_DRAM_START+0x1CC000L)        // 13K DW

/////////////////////////////////////////////////////////////////////////////////
// For AP Usage
#define     DS_INPUT_BITBUF_ST_AP       (CT909_DRAM_START+0x066000L)
#define     DS_INPUT_BITBUF_END_AP      (CT909_DRAM_START+0x084000L)  // 30K DW: 3 Blocks
#define     DS_OSD_FONTTABLE_ST_AP      (DS_OSD_FONTTABLE_ST)
#define     DS_OSD_FONTTABLE_END_AP     (DS_OSD_FONTTABLE_END)       // 3K DW
#define     DS_OSDFRAME_ST_AP           (CT909_DRAM_START+0x084000L)
#define     DS_OSDFRAME_END_AP          (CT909_DRAM_START+0x08A000L)  // 6K DW
#define     DS_USERDATA_BUF_ST_AP       (CT909_DRAM_START+0x08B000L)
#define     DS_USERDATA_BUF_END_AP      (CT909_DRAM_START+0x200000L)  // 373K DW

#define     DS_AP_INFO_AREA             (CT909_DRAM_START+0x08A000L)
#define     DS_AP_CODE_AREA             (DS_USERDATA_BUF_ST_AP)

/////////////////////////////////////////////////////////////////////////////////
// Radio Buffer:
#define     DS_RADIO_BMPDATA_ST         (CT909_DRAM_START+0x1AD000L)  // 67K DW
#define     DS_RADIO_BMPDATA_END        (CT909_DRAM_START+0x1F0000L)


#endif // (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)

#ifdef __cplusplus
        }
#endif

#endif // __DVD_DRAM_16M_H__
