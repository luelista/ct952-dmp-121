//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2004,
//          J000, all right reserved.
//
//      Product : CT909S
//
//      Date    : 2006.03.9
//      Author  : Cheertek
//      Purpose : 
//      Sources : 
//  **************************************************************************
#ifndef  __DVB_DRAM_32M_H__
#define  __DVB_DRAM_32M_H__


////////////////////////////////////////////////////////////////////////////////////////
//
//  DRAM Allocation Table
//
////////////////////////////////////////////////////////////////////////////////////////
#if (DRAM_CONFIGURATION_TYPE_DVB == DRAM_SIZE_32)
////////////////////////////////////////////////////////////////////////////////////////

#define DS_OSD_PAL_EXTRA_REGION_ST          (DS_DVD_DRAM_END+0x000000L) 
#define DS_OSD_PAL_EXTRA_REGION_END         (DS_DVD_DRAM_END+0x00F000L) // 616*100 BYTES
#define DS_OSD_BACKUP_REGION_ST             (DS_DVD_DRAM_END+0x00F000L)
#define DS_OSD_BACKUP_REGION_END            (DS_DVD_DRAM_END+0x01F000L) // 320*200 bytes.

#define DS_MEMALLOC_REGION_ST               (DS_DVD_DRAM_END+0x32C000L)
#define DS_MEMALLOC_REGION_END              (DS_DVD_DRAM_END+0x390000L) // 100K DW

////////////////////////////////////////////////////////////////////////////////////////

#define DS_FRAME_DIGEST_ADDR_ST             (DS_DVD_DRAM_END+0x02C800L)
#define DS_FRAME_DIGEST_ADDR_END            (DS_DVD_DRAM_END+0x0C4800L)     // 720x576*1.5 -> 152K DW
#define DS_FRAME_DIGEST_DEFLICKER_ADDR_ST   (DS_DVD_DRAM_END+0x024000L)
#define DS_FRAME_DIGEST_DEFLICKER_ADDR_END  (DS_DVD_DRAM_END+0x02C800L)     // 8.5K DW

////////////////////////////////////////////////////////////////////////////////////////

#define DS_INPUT_LARGINSREP_ST          	(DS_FRAME_DIGEST_DEFLICKER_ADDR_ST)

#define DS_INPUT_INSREP_ST                  (DS_DVD_DRAM_END+0x0C4800L)
#if (DECODER_SYSTEM == DVD909S_EVAL) 
#define DS_INPUT_INSREP_END                 (DS_DVD_DRAM_END+0x21F000L)
#define DS_DATABASE_ST                      (DS_DVD_DRAM_END+0x21F000L)
#define DS_DATABASE_END                     (DS_DVD_DRAM_END+0x25F000L)
#else
#define DS_INPUT_INSREP_END                 (DS_DVD_DRAM_END+0x25F000L)
#endif

////////////////////////////////////////////////////////////////////////////////////////
#define DS_DVB_TTX_ST                       (DS_DVD_DRAM_END+0x25F000L)
#define DS_DVB_TTX_END                      (DS_DVD_DRAM_END+0x31F000L)     // 192K DW
#define DS_DVB_SP_ST                        (DS_DVD_DRAM_END+0x25F000L)
#define DS_DVB_SP_END                       (DS_DVD_DRAM_END+0x31F000L)     // 192K DW

////////////////////////////////////////////////////////////////////////////////////////
#endif // (DRAM_CONFIGURATION_TYPE_DVB == DRAM_SIZE_32)


#endif  //__DVB_DRAM_32M_H__
