/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved.

        Product : WinAV Firmware

        Date    : 1998.10.10.
        Author  : Cheertek (D300 TCH)
        Purpose : CD information module
                  The moudle will support the information of any CD Disc
        Sources : infofilter.h/ cdinfo.c/ cdinfo2.c

------------------------------------------------------------
        Update

        Date    : 1999.8.18.
        Name    : Cheertek (D300 TCH)
        Description     : ???

                        Item                                    Source
        ----------------------------------------        -------------------
        . Support to check CVD Encryption
        . Redefine ALL DiscType
        . Check DVD title.
****************************************************************************/

#ifndef __INFOFILTER_H__
#define __INFOFILTER_H__

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// Definition declare part
#define     MAX_TRACKNO                 (99+ 1)

#define     OFFSET_ENTRIES_VCD          12
#define     OFFSET_INFO_VCD             56

#define     ENTRY_MAX_LEN               32

// #define DEBUG_FUNCTION

#define     GAP_INFOVCD                 668
#define     GAP_ENTRIES                 40
// Added by Chern Chyh-Chain, 08-30-2001, BEGIN

// Fix problem: Some special DVD titles have a title set encrypted with not only one title key.
// Current what we found is one title set encrypted with two title keys and three titles have this phenomenon.
#define     MAX_TITLE_KEY_NUM       2       // How many title keys per title set
#define     MAX_MULTI_KEYS_TITLES     5 //4  //3     // How many titles have multiple keys phenomenon we found
#define     INVALID_MULTI_KEYS_TITLE    0xFF    // When __bMultiKeysTitle equals to this value, it means no multiple keys title inserted.

#define     _cSectorDVD     __cSector3
//////////////////////////////////////
// Define for navigate type and disc / media mode.
#define     UNKNOW_NAVIGATE_MODE        0
#define     DISC_NAVIGATE_MODE      1
#define     MEDIA_NAVIGATE_MODE     2
#define     NAVIGATE_NONE           3

// wyc.check, temp comment it because need to convert old define.
/*#define     UNKNOW_DISC_MEDIA       0
#define     BOOK_CDDA       0x0001
#define     BOOK_CDI        0x0002
#define     BOOK_VCD        0x0004
#define     BOOK_DVD        0x0008
#define     BOOK_CVD        0x0010
#define     BOOK_SVCD       0x0020
#define     VCD_1_1     0x0100
#define     VCD_2_0     0x0200
#define     VCD_KARAOKE     0x0400
#define     BOOK_ENCRYPT        0x0800
#define     CDROM_M1        0x0001
#define     CDROM_M2        0x0002
#define     DVDROM      0x0004
#define     PICTURE_CD      0x0008
#define     FDISK       0x0010
#define     NO_DISC_MEDIA_IN_DRIVE      0x8000*/


#define     BOOK_UNKNOW                 0x0000

#define     BOOK_CDDA                   0x0001
#define     BOOK_CDROM                  0x0002          // Mode1 only
#define     BOOK_CDI                    0x0004
#define     BOOK_VCD                    0x0008
//#define     BOOK_PHOTOCD                0x0010        // ** DVD0.16; remove it
#define     BOOK_DVD                    0x0010
#define     BOOK_CVD                    0x0020
#define     BOOK_SVCD                   0x0040
#define     BOOK_2X                     0x0060  // BOOK_CVD | BOOK_SVCD

// attribute of DiscType
#define     VCD_1_0                     BOOK_VCD
#define     VCD_1_1                     0x1000
#define     VCD_2_0                     0x2000
#define     VCD_KARAOKE                 0x4000
#define     BOOK_ENCRYPT                0x8000  // CVD-Encryption

#define     CDROM_M1                   0x0100
#define     CDROM_XA                    0x0200
#define     CDROM_PICTURECD             0x0400  

#define     BOOK_M1                      0x0102  // BOOK_CDROM | CDROM_M1
#define     BOOK_XA                      0x0302  // BOOK_CDROM | CDROM_M1 | CDROM_XA
#define     BOOK_PICTURECD              0x0702  // BOOK_CDROM | CDROM_M1 | CDROM_XA | CDROM_PICTURECD. 
#define     DOOR_OPEN                   0x0001
#define     NO_DISC_IN_DRIVE            0x0800

#define     DISCINFO_NONE                           0x00
#define     DISCINFO_ENCRYPT                    0x01
#define     DISCINFO_REGION_ERROR               0x02   
#define     DISCINFO_DAMAGED                0x04  
#define     DISCINFO_UNRECOGNIZE                    0x08   

#define     DISCINFO_UNPLAYABLE             0x0F   

// Track Info Byte
#define     PREEMPHASIS_2_CHANNELS          0x00010000
#define     PREEMPHASIS_4_CHANNELS          0x10010000

// CDINFO_ParseInfo definition
#define     DISC_VOL_SPACESIZE                  1
#define     DISC_LOC_MPATHTABLE                 2

// VCD definition
#define     VCD_BASIC                           5       // max larger than DISC_XXX
#define     VCD_ENTRIES_ADDRESS                 VCD_BASIC+ 1    //
#define     VCD_ENTRIES                         VCD_BASIC+ 2    // total entries
#define     VCD_ENTRIES_LOCATE                  VCD_BASIC+ 3

#define     VCD_SEQUENCES                       VCD_BASIC+ 4
#define     VCD_SEQUENCE_ADDRESS                VCD_BASIC+ 5
#define     VCD_SEQUENCE_LOCATE                 VCD_BASIC+ 6

#define     VCD_ENTRIES_TRACK                   VCD_BASIC+ 7

// InfoVCD definition
#define     INFOVCD_PSDSIZE                     VCD_BASIC+ 20
#define     INFOVCD_FIRSTSEG_ADDRESS            VCD_BASIC+ 21
#define     INFOVCD_OFFSET_MUL                  VCD_BASIC+ 22
#define     INFOVCD_MAXLIST_ID                  VCD_BASIC+ 23
#define     INFOVCD_MAXSEG_NO                   VCD_BASIC+ 24
#define     INFOVCD_SEGPLAY_ITEM                VCD_BASIC+ 25

// CD Information String
#define     VCD_CDTITLE                         10
#define     VCD_CDID                            11
#define     VCD_CDVER                           12

#define     KARAOKE_SEQUENCE_TITLE              20
#define     KARAOKE_SONG_WRITER                 KARAOKE_SEQUENCE_TITLE+ 1
#define     KARAOKE_COMPOSER                    KARAOKE_SEQUENCE_TITLE+ 2

// Play Segment Item
#define     SEGPLAY_ITEM_BASE                   1
#define     SEGPLAY_ITEM_FIRST                  SEGPLAY_ITEM_BASE+ 1
#define     SEGPLAY_ITEM_HASAUDIO               SEGPLAY_ITEM_BASE+ 2
#define     SEGPLAY_ITEM_HASVIDEO               SEGPLAY_ITEM_BASE+ 3
#define     SEGPLAY_ITEM_AUD_SINGLE             SEGPLAY_ITEM_BASE+ 4
#define     SEGPLAY_ITEM_AUD_STEREO             SEGPLAY_ITEM_BASE+ 5
#define     SEGPLAY_ITEM_AUD_DUAL               SEGPLAY_ITEM_BASE+ 6
#define     SEGPLAY_ITEM_VID_NTSC_STILL         SEGPLAY_ITEM_BASE+ 7
#define     SEGPLAY_ITEM_VID_NTSC_STILLH        SEGPLAY_ITEM_BASE+ 8
#define     SEGPLAY_ITEM_VID_NTSC_MOTION        SEGPLAY_ITEM_BASE+ 9
#define     SEGPLAY_ITEM_VID_PAL_STILL          SEGPLAY_ITEM_BASE+ 10
#define     SEGPLAY_ITEM_VID_PAL_STILLH         SEGPLAY_ITEM_BASE+ 11
#define     SEGPLAY_ITEM_VID_PAL_MOTION         SEGPLAY_ITEM_BASE+ 12

#define     SEGPLAY_ITEM_HIGH                   SEGPLAY_ITEM_BASE+ 20
#define     SEGPLAY_ITEM_NORMAL                 SEGPLAY_ITEM_BASE+ 21
#define     SEGPLAY_ITEM_MOTION                 SEGPLAY_ITEM_BASE+ 22
#define     SEGPLAY_ITEM_VALUE                  SEGPLAY_ITEM_BASE+ 23

// Selection/ Play list
#define     ID_PLAYLIST                         0x01
#define     ID_SELECTIONLIST                    0x02
#define     ID_ENDLIST                          0x04

#define     LIST_FIRST                          300
#define     LIST_CHOOSE                         LIST_FIRST+ 1
#define     LIST_NEXT                           LIST_FIRST+ 2
#define     LIST_PREVIOUS                       LIST_FIRST+ 3
#define     LIST_RETURN                         LIST_FIRST+ 4
#define     LIST_DEFAULT                        LIST_FIRST+ 5
#define     LIST_TIMEOUT                        LIST_FIRST+ 6
#define     LIST_MENUSELECT                     LIST_FIRST+ 7
#define     LIST_PLAY                           LIST_FIRST+ 8
#define     LIST_WORK                           LIST_FIRST+ 9

#define     LIST_SEL_START                      1
#define     LIST_SEL_NOS                        LIST_SEL_START+ 2
#define     LIST_SEL_BSN                        LIST_SEL_START+ 3
#define     LIST_SEL_LISTID                     LIST_SEL_START+ 4
#define     LIST_SEL_WAITTIME                   LIST_SEL_START+ 5
#define     LIST_SEL_LOOPCOUNT                  LIST_SEL_START+ 6
#define     LIST_SEL_JUMPTIME                   LIST_SEL_START+ 7
#define     LIST_SEL_PLAYITEMNO                 LIST_SEL_START+ 8
#define     LIST_SEL_SELECTION                  LIST_SEL_START+ 9
#define     LIST_SEL_DEFAULT                    LIST_SEL_START+10
#define     LIST_SEL_NEXT                       LIST_SEL_START+11

#define     LIST_PLAY_START                     100
#define     LIST_PLAY_NOI                       LIST_PLAY_START+ 1
#define     LIST_PLAY_LISTID                    LIST_PLAY_START+ 2
#define     LIST_PLAY_PLAYTIME                  LIST_PLAY_START+ 3
#define     LIST_PLAY_PLAYITEM_WAITTIME         LIST_PLAY_START+ 4
#define     LIST_PLAY_AUTOPAUSE_WAITTIME        LIST_PLAY_START+ 5
#define     LIST_PLAY_PLAYITEMNO                LIST_PLAY_START+ 6

// The usage of Directory Record
#define         FILE_FLAG_NONE                           0
#define         FILE_FLAG_FILE                           0x0001
#define         FILE_FLAG_DIR                            0x0002
#define         FILE_FLAG_ERROR_SERVO                    0x0000 // ** TCH0.32;
#define         FILE_FLAG_PARENT                         0x0010
#define         FILE_FLAG_CURRENT                        0x0020
#define         FILE_FLAG_ROOT                           0x0040

// GetEntryInfo Usage [ISO9660]
#define         ENTRY_INFO_LOCATION                 1
#define         ENTRY_INFO_DATALENGTH               2
#define         ENTRY_INFO_NAME                     3
#define         ENTRY_INFO_ATTRIBUTE                4
#define         ENTRY_INFO_PARENT_ID                5
// wyc1.10-909, define for DivX SP.
#define         ENTRY_INFO_DATALENGTH_IN_BYTE       6

// wyc1.07-909, because MAX_EXTENSION_NO will be changed by outside define and structure DIRNAMEINFO will be used in non-protected module and protected module.
// So DIRNAMEINFO can't use MAX_EXTENSION_NO as reference and need to use DEFAULT_MAX_EXTENSION_NO and DEFAULT_MAX_EXTENSION_NO can't be changed by outside define.
// Otherwise FAT file system will be wrong because MM & FAT will reference to wrong extension number.
#define         DEFAULT_MAX_EXTENSION_NO        8
#ifdef SUPPORT_WMA
#define         MAX_EXTENSION_NO                8
#else
#define         MAX_EXTENSION_NO                7
#endif
// For record the file extension name type
// Because OSDMM also use these define to draw icon, so these define must be sequence number mode for number increase / decrease.
// wyc1.02-909, redefine it.
#ifdef DOLBY_CERTIFICATION
#define         EXTNAME_NONE                    0xFF
#define         EXTNAME_MP3                     0
#define         EXTNAME_MP2                     1
#define         EXTNAME_AC3                     2
#define         EXTNAME_JPG                     3
#define         EXTNAME_DAT                     4
#define         EXTNAME_MPG                     5
#define         EXTNAME_VOB                     6
#define         EXTNAME_AVI                     7
///// Follow are not supported now.
#define         EXTNAME_WMA                     9
#define         EXTNAME_DTS                     10
// wyc2.20-909S, add this define here to fix compile wrong when define DOLBY_CERTIFICATION
#define         EXTNAME_MP4                     11   // LLY1.50

// wyc0.80, add new define for ext name in bit resolution.
#define         EXT_BIT_MP3                     0x1
#define         EXT_BIT_MP2                     0x2
#define         EXT_BIT_AC3                     0x4
#define         EXT_BIT_JPG                     0x8
#define         EXT_BIT_DAT                     0x10
#define         EXT_BIT_MPG                     0x20
#define         EXT_BIT_VOB                     0x40
#define         EXT_BIT_AVI                     0x80
#define         EXT_BIT_WMA                     0
#define         EXT_BIT_DTS                     0
#define         EXT_BIT_ALL                     0xFF
#else
#ifdef SUPPORT_WMA
#define         EXTNAME_NONE                    0xFF
#define         EXTNAME_MP3                     0
#define         EXTNAME_MP2                     1
#define         EXTNAME_WMA                     2
#define         EXTNAME_JPG                     3
#define         EXTNAME_DAT                     4
#define         EXTNAME_MPG                     5
#define         EXTNAME_VOB                     6
#define         EXTNAME_AVI                     7
///// Follow are not supported now.
#define         EXTNAME_AC3                     8
#define         EXTNAME_DTS                     9
#define         EXTNAME_MP4                     10

#define         EXT_BIT_MP3                     0x1
#define         EXT_BIT_MP2                     0x2
#define         EXT_BIT_WMA                     0x4
#define         EXT_BIT_JPG                     0x8
#define         EXT_BIT_DAT                     0x10
#define         EXT_BIT_MPG                     0x20
#define         EXT_BIT_VOB                     0x40
#define         EXT_BIT_AVI                     0x80
#else
#define         EXTNAME_NONE                    0xFF
#define         EXTNAME_MP3                     0
#define         EXTNAME_MP2                     1
#define         EXTNAME_JPG                     2
#define         EXTNAME_DAT                     3
#define         EXTNAME_MPG                     4
#define         EXTNAME_VOB                     5
#define         EXTNAME_AVI                     6
///// Follow are not supported now.
#define         EXTNAME_AC3                     9
#define         EXTNAME_DTS                     10
#define         EXTNAME_MP4                     11   // LLY1.50

#define         EXT_BIT_MP3                     0x1
#define         EXT_BIT_MP2                     0x2
#define         EXT_BIT_JPG                     0x4
#define         EXT_BIT_DAT                     0x8
#define         EXT_BIT_MPG                     0x10
#define         EXT_BIT_VOB                     0x20
#define         EXT_BIT_AVI                     0x40
#define         EXT_BIT_WMA                     0x0
#endif //
// wyc2.38-909s,
#define         EXT_BIT_ALL                     (0xFF)
#endif //

// wyc1.11a, add MP4 char based SP define here.
#define         MP4SP_EXT_NONE                  (0)
#define         MP4SP_EXT_START                 (1)
#define         MP4SP_EXT_SRT                   (MP4SP_EXT_START)
#define         MP4SP_EXT_SSA                   (MP4SP_EXT_START + 1)
#define         MP4SP_EXT_ASS                   (MP4SP_EXT_START + 2)
#define         MP4SP_EXT_SUB                   (MP4SP_EXT_START + 3)
#define         MP4SP_EXT_SMI                   (MP4SP_EXT_START + 4)
#define         MP4SP_EXT_PSB                   (MP4SP_EXT_START + 5)
// wyc2.16-909S, add sub-title extension type PSB.
#define         MP4SP_EXT_END                   (MP4SP_EXT_PSB)

// wyc.278. set the reading range of Fmanager function.
#define         SECTOR_FM_READ_RANGE          15

#define         FIRST_SESSION                   1
#define         LAST_SESSION                   0

// wyc2.60-909P, the unicode tag.
#define         IF_UNICODE_FILE_TAG             0x80

// wyc1.01-909, add one define FILE_SYSTEM_NONE
#define         FILE_SYSTEM_NONE            0
#define         FILE_SYSTEM_ISO9660         1
#define         FILE_SYSTEM_ISOJOLIET       2
#define         FILE_SYSTEM_UDF             3
#define         FILE_SYSTEM_FAT16           4
#define         FILE_SYSTEM_FAT32           5
#define 	FILE_SYSTEM_FAT12           6

// scping, add define for UDF
#define AVDP_LOCATION   256
#define AVDP_TAGID          2    // Anchor Volume Descriptor Pointer
#define PD_TAGID            5    // Partition Descriptor
// File define
#define FSD_TAGID           256  // File Set Descriptor
#define FID_TAGID           257  // File Identifier Descriptor
#define FE_TAGID            261  // File Entry
#define ICB_DIR             4    // Dir
#define ICB_FILE            5    // File
#define M_SESSION           10   // MultiSession for DVD-R DL

#define     DIRRECORD_DATALEN1          10
#define     DIRRECORD_DATALEN2          14

#define IF_FLAG_TRIGGER_PARSE_INFO  0x00000001
#define IF_FLAG_PARSE_INFO_OK  0x00000002
#define IF_FLAG_PARSE_INFO_FAILED  0x00000004
#define IF_FLAG_PARSE_INFO_PARSING  0x00000008
#define IF_FLAG_TRIGGER_DELETE_INFO  0x00000010
#define IF_FLAG_DELETE_INFO_COMLETED  0x00000020

#define     INFOFILTER_PARSING      0x1
#define     INFOFILTER_PARSE_OK     0x2
#define     INFOFILTER_PARSE_FAILED     0x3

#define FILE_RECORD_NONE       (0xFFFC) // mean no record in this file link list
#define FILE_RECORD_BEGIN       (0xFFFD)
#define FILE_RECORD_END             (0xFFFE)
// wyc0.68, mean this file record is not link by sort/filter yet
#define FILE_RECORD_CLEAN       (0xFFFF)

#define MM_MAX_FILENAME_LENGTH  (12) // total 12 chars in file name.
#define MM_MAX_DIRNAME_LENGTH   (10) // total 10 chars in dir name.
#define MM_ISO_SUPPORT_FILE_LENGTH  (64)

// define for parsing time information from ISO disc
#define     IF_TIME_YEAR_RECORD_OFFSET     (1980 - 1900)   // 1980 is our recording begin, 1900 is ISO9660 recorded from 1900.
#define     IF_YEAR_SHIFT       26
#define     IF_MONTH_SHIFT      22
#define     IF_DATE_SHIFT       17
#define     IF_HOUR_SHIFT       12
#define     IF_MIN_SHIFT        6
#define     IF_SEC_SHIFT        0

// wyc0.85, add define
#define     IF_YEAR_MASK_BIT    0x3F
#define     IF_MONTH_MASK_BIT   0xF
#define     IF_DATE_MASK_BIT    0x1F
#define     IF_HOUR_MASK_BIT    0x1F
#define     IF_MIN_MASK_BIT     0x3F
#define     IF_SEC_MASK_BIT     0x3F

#define     IF_DUMP_IN_SECTOR_SIZE      2048

// wyc0.63, add two define for DVD / VCD / CDROM dumped sector number.
// wyc0.65, set from 150 to 15 to fix some DVD-ROM can't detect CDROM disc successfully problem.
#define     IF_DUMP_SECTOR_NUM_CDROM        15
#define     IF_DUMP_SECTOR_NUM_OTHER        5
#define     IF_DUMP_SECTOR_NOTCARE_CDROM    0xFE
#define     IF_DUMP_SECTOR_NOTCARE_OTHER    0xFF


// wyc2.36a-909s,
#define     IF_MODE_CONFIG_SUPPORT          0x1
#define     IF_MODE_CHECK_MPEG_CD           0x2
#define     IF_MODE_CLEAR_MODE              0x3

#define     IF_SUOOPRT_FUSS_MPEGCD          0x1
#define     IF_MPEG_CD_TRUE                 0x2

// wyc2.38a-909s, two mode define 
#define     IF_DISC_MODE       1
#define     IF_DISK_MODE       2

// wyc2.55-909P-AP,
#define     IF_PARSE_FILE_MODE  1
#define     IF_PARSE_AP_MODE    2

// wyc2.14-909s, move define to utl.h.

// Follow define for used for FAT
#define FAT_MAX_SHORT_FILE_NAME     8
#define FAT_START_CLUS_NUM          2

#define PARSER_SECTOR_SIZE          (2048)
#define USB_DEFAULT_SECTOR_SIZE     (512)

// wyc1.05-909, add one define to record the cached cluster number in DRAM. Beause FAT16 will use 2 bytes per cluster, 
// so cached cluster mumber is 1024. Total used DRAM is 2048 bytes.
#define FAT12_TABLE_CACHE_CLUS_NUM    (1364)
#define FAT16_TABLE_CACHE_CLUS_NUM    (1024)
// wyc1.07-909, because FAT32 need 4 bytes to record one cluster in FAT table, so we just use 512 as the cache memory size.
#define FAT32_TABLE_CACHE_CLUS_NUM    (512)
// maybe different in FAT32
#define FAT16_TABLE_OCCUPY_SIZE_PER_CLUS      (2)
#define FAT32_TABLE_OCCUPY_SIZE_PER_CLUS      (4)

#define FAT_INFO_BYTE_PER_USBSEC            (1)
#define FAT_INFO_DATA_START_SEC             (2)
#define FAT_INFO_SECTOR_PER_CLUSTER         (3)

#define FAT12_TABLE_CLUS_END                  (0xFFF)
#define FAT16_TABLE_CLUS_END                  (0xFFFF)
#define FAT32_TABLE_CLUS_END                  (0x0FFFFFFF)  // FAT is DWORD reverse. So actural value is 0x0FFFFFFF

///////////////////////////////////////////////////////
// Structure define part.
// Define the structure to keep the title's information for comparing and reading correct title key.
typedef struct  tagMULTI_KEYS_TITLE_LOG
{
    char     cTitleName [15];     // Point to a title name string
    BYTE    bRegionInfo;        // Title's region number
    BYTE    bTitleSetNo;    // Title set number which has this abnormal phenomenon
    DWORD   dwTitleKeyAddress[MAX_TITLE_KEY_NUM];   // Keep first cell start address of each title key
} MULTI_KEYS_TITLE_LOG, *PMULTI_KEYS_TITLE_LOG;

// follow structure define are for UDF
typedef struct  tagTIMESTAMP
{
    WORD    wTypeAndTimezone;
	  WORD    wYear;
	  BYTE    bMonth;
	  BYTE    bDay;
	  BYTE    bHour;
	  BYTE    bMinute;
	  BYTE    bSecond;
	  BYTE    bCentiseconds;
	  BYTE    bHundredsofMicroseconds;
	  BYTE    bMicroseconds;
}   PACK_FORMAT TIMESTAMP, * PTIMESTAMP; 

typedef struct  tagEXTENT_AD
{
    DWORD   dwExtentLength;
    DWORD   dwExtentLocation;
}   PACK_FORMAT EXTENT_AD, * PEXTENT_AD;

typedef struct  tagLB_ADDR
{
    DWORD   dwLogicBlockNum;
    WORD   wParRefeNum;
}   PACK_FORMAT LB_ADDR, * PLB_ADDR;

typedef struct  tagLONG_AD
{
    DWORD   dwExtentLength;
    LB_ADDR   ExtentLocation;
    BYTE    bImpUse[6];
}   PACK_FORMAT LONG_AD, * PLONG_AD;

typedef struct  tagDESCRIPTOR_TAG
{
    WORD    wTagIdentifier;
    WORD    wDescriptorVersion;
    BYTE    bTagCheckSum;
    BYTE    bReserved;
    WORD    wTagSerialNum;
    WORD    wDescriptorCRC;
    WORD    wDescriptorCRCLength;
    DWORD   dwTagLocation;
}   PACK_FORMAT DESCRIPTOR_TAG, * PDESCRIPTOR_TAG;

typedef struct  tagICB_TAG
{
    DWORD    dwPRNDE;
    WORD    wStrategyType;
    WORD    wStrateryParameter;
    WORD    wMaxNumOfEntry;
    BYTE    bReserve;
    BYTE    bFileType;
    BYTE    bParentICBLoc[6];
    WORD   wFlag;
}   PACK_FORMAT ICB_TAG, * PICB_TAG;

typedef struct  tagAVDP_STRUCTURE
{
    DESCRIPTOR_TAG  DesTag;
    EXTENT_AD   MVDST;
    EXTENT_AD   RVDST;
}   PACK_FORMAT AVDP_STRUCTURE, * PAVDP_STRUCTURE;

typedef struct  tagPARTITION_DESCRIPTOR
{
    DESCRIPTOR_TAG  DesTag;
    DWORD   dwVDSNum;
    WORD    wPartitionFlags;
    WORD    wPartitionNumber;
    BYTE    bPartitionContents[32];
    BYTE    bPartitionContentUse[128];
    DWORD   dwAccessType;
    DWORD   dwPartitionStartLocation;
    DWORD   dwPartitionLength;
}   PACK_FORMAT PARTITION_DESCRIPTOR, * PPARTITION_DESCRIPTOR;

typedef struct  tagFILE_SET_DESCRIPTOR
{
    DESCRIPTOR_TAG  DesTag;
    BYTE    bUnUsedByte[384];
    LONG_AD     RootICB;
}   PACK_FORMAT FILE_SET_DESCRIPTOR, * PFILE_SET_DESCRIPTOR;

typedef struct  tagFILE_ID_DESCRIPTOR
{
    DESCRIPTOR_TAG  DesTag;
    WORD    wFileVerNum;
    BYTE    bFileCharacteristics;
    BYTE    bFILength;
    LONG_AD     ICB;
    WORD    wIULength;       
}   PACK_FORMAT FILE_ID_DESCRIPTOR, * PFILE_ID_DESCRIPTOR;

typedef struct  tagFILE_ENTRY_DESCRIPTOR
{
    DESCRIPTOR_TAG  DesTag;
    ICB_TAG ICBTag;
    BYTE    bUnUsedByte1[20];
    BYTE    bInfoLength[8];
    BYTE    bLogicBlock[8];
	  TIMESTAMP AccessTime; 
    BYTE    bUnUsedByte2[84];
    DWORD   dwEALength;
    DWORD   dwADLength;
    LONG_AD    AD1;
    LONG_AD    AD2;   
}   PACK_FORMAT FILE_ENTRY_DESCRIPTOR, * PFILE_ENTRY_DESCRIPTOR;

// follow structure define are for FAT
typedef struct  tagFAT_FS_FAT16_BOOT_SECTOR
{
    WORD wBytesPerSector;
    BYTE bSectorsPerCluster;
    WORD ReservedSectors;
    BYTE bNumberOfFATs;
    WORD wRootEntries;
    WORD wTotalSectors;   
    BYTE bMedia;
    WORD wSectorsPerFAT;
    WORD wSectorsPerTrack;   
    WORD wHeadsPerCylinder;
    DWORD dwHiddenSectors;
    DWORD dwTotalSectorsBig;   
}   PACK_FORMAT FAT_FS_FAT16_BOOT_SECTOR, * PFAT_FS_FAT16_BOOT_SECTOR;

typedef struct  tagFAT_FS_FAT16_BPB
{
    BYTE bJmp[3];
    BYTE bOemName[8];
    FAT_FS_FAT16_BOOT_SECTOR bBootSector;
    BYTE bDriveNumber;
    BYTE bUnused;
    BYTE bExtBootSignature;   
    DWORD dwSerialNumber;
    BYTE bVolumeLabel[11];
    BYTE bFileSystem[8];   
}   PACK_FORMAT FAT_FS_FAT16_BPB, * PFAT_FS_FAT16_BPB;

// wyc1.07-909, structure define for FAT32.
typedef struct  tagFAT_FS_FAT32_BOOT_SECTOR
{
    WORD wBytesPerSector;
    BYTE bSectorsPerCluster;
    WORD ReservedSectors;
    BYTE bNumberOfFATs;
    WORD wRootEntries;
    WORD wTotalSectors;   
    BYTE bMedia;
    WORD wSectorsPerFAT;
    WORD wSectorsPerTrack;   
    WORD wHeadsPerCylinder;
    DWORD dwHiddenSectors;
    DWORD dwTotalSectorsBig;   
    DWORD dw32SectorsPerFAT;
    WORD w32Flags;
    WORD w32Version;
    DWORD dw32RootCluster;
    WORD w32InfoSector;
    WORD w32BootBackupStart;
    BYTE b32Reserved[12];
}   PACK_FORMAT FAT_FS_FAT32_BOOT_SECTOR, * PFAT_FS_FAT32_BOOT_SECTOR;

typedef struct  tagFAT_FS_FAT32_BPB
{
    BYTE bJmp[3];
    BYTE bOemName[8];
    FAT_FS_FAT32_BOOT_SECTOR bBootSector;
    BYTE b32DriveNumber;
    BYTE b32Unused;
    BYTE b32ExtBootSignature;   
    DWORD dw32SerialNumber;
    BYTE b32VolumeLabel[11];
    BYTE b32FileSystem[8];   
}   PACK_FORMAT FAT_FS_FAT32_BPB, * PFAT_FS_FAT32_BPB;

// wyc1.02, new structure to parse physical sector.
typedef struct  tagMBR
{
    BYTE bDummy[454];
    DWORD dwLBABegin;
}   PACK_FORMAT MBR, * PMBR;

typedef struct  tagFAT_FS_FAT_SFN_ENTRY
{
    BYTE bDirName[8];
    BYTE bDirExtName[3];
    BYTE bDirAttr;
    BYTE bReserved;
    BYTE bDirCrtTimeTenth;
    WORD wDirCtrTime;
    WORD wDirCrtDate;
    WORD wDirLstAccDate;
    WORD wDirFstClusHI;
    WORD wDirWrtTime;
    WORD wDirWrtDate;
    WORD wDirFstClusLO;
    DWORD dwDirFileSize;
}   PACK_FORMAT FAT_FS_FAT_SFN_ENTRY, * PFAT_FS_FAT_SFN_ENTRY;

typedef struct  tagFAT_FS_FAT_LFN_ENTRY
{
    BYTE bLDirOrd;
    BYTE bLDirName1[10];
    BYTE bLDirAttr;
    BYTE bLDirType;
    BYTE bLDirChkSum;
    BYTE bLDirName2[12];
    WORD bLDirFstClusLO;
    BYTE bLDirName3[4];
}   PACK_FORMAT FAT_FS_FAT_LFN_ENTRY, * PFAT_FS_FAT_LFN_ENTRY;

#define ENTRY_RANGE_LENGTH      8   // wyc1.10, need to increase the length to 7
typedef struct  tagENTRY_RANGE
{
    BYTE cS[4];
    DWORD    dwLen;       // ** TCH0.50;
}   ENTRY_RANGE, * PENTRY_RANGE;

// LLY1.20 create, declare a file range structure.
typedef struct tagFILE_RANGE
{
    DWORD   dwStart;    // Start sector
    DWORD   dwLen;      // File length in sector unit
} FILE_RANGE, *PFILE_RANGE;

// File name information structure
// NOTICE!! When update structure of FILENAMEINFO, need also to update FILENAMEINFO_LENGTH also.
typedef struct tagFILENAMEINFO
{
    char    cName[MM_MAX_FILENAME_LENGTH+1];
    BYTE    bExtID;
}   FILENAMEINFO, * PFILENAMEINFO;

// dir name information structure
// NOTICE !! When update DIRNAMEINFO structure, need also to modify DIRNAMEINFO_LENGTH.
typedef struct tagDIRNAMEINFO
{
    char    cName[MM_MAX_DIRNAME_LENGTH+1]; // store directory name
    BYTE    bMMLinkStatus;
    WORD    wMMFileRecordEntry;
    WORD    wMMFileRecordEnd;
    // wyc1.07-909, can't use define MAX_EXTENSION_NO because it will be changed by SUPPORT_4M_FLASH define, so use another to replace it.
    // wyc2.60-909P, seemed don't use in code now.
    // WORD    wMMExtFileNum[DEFAULT_MAX_EXTENSION_NO];    // for recording the file number in each extension name in each dirs.
}   DIRNAMEINFO, * PDIRNAMEINFO;

// wyc1.05-909, move these two define from mmanager.h tp infofilter.h
// The structures of TREE record.
typedef struct tagTREE_INFO
{
    BYTE    bParentID;          // parent ID
    BYTE    b1stSubDirID;       // 1st sub-directory ID
    BYTE    bNoOfSubDir;        // total number of sub-directory
}   TREE_INFO, * PTREE_INFO;

// wyc1.07-909, move define to here.
// Max length of DIR_LOC structure
#define DIR_LOC_LENGTH          4

typedef struct tagDIR_LOC
{
    BYTE    bSector[DIR_LOC_LENGTH]; // keep location (3 BYTE unit)
}   DIR_LOC, * PDIR_LOC;


// Max length of TREE_INFO structure
#define TREE_INFO_LENGTH        3

// wyc1.05-909, add one define to parse dir information.
typedef struct  tagFAT_FS_FAT_DIRLOG
{
    BYTE bMaxStringLen;
    WORD wMaxDirNO;
    BYTE bActualDirNO;
    PDIRNAMEINFO pDirNameList;
    PTREE_INFO pVolumeDirTree;
    WORD *pFilesOfDir;
    PDIR_LOC pDirLoc;
} FAT_FS_FAT_DIRLOG, * PFAT_FS_FAT_DIRLOG;

// File record structure
// NOTICE!! When update structure of FILE_RECORD, need also to update FILE_RECORD_LENGTH also.
typedef struct  tagFILE_RECORD
{
    FILENAMEINFO        FileName;       // 14 bytes
    WORD        wNextFileRecord;        // 2 bytes
    ENTRY_RANGE        FileRange;       // 8 bytes
    BYTE        bFileAttr;      // 1 bytes
    WORD        wPrevFileRecord;        // 2 bytes
    DWORD       dwFileTime;     // 4 bytes
}FILE_RECORD, *PFILE_RECORD;

// wyc1.10-909, add structure for DivX SRC file parsing.
// wyc1.11a, rename define and re-design the config og structure.
typedef struct  tagMP4SP_RECORD
{
    FILENAMEINFO    MP4SPName;       // 14 bytes
    DWORD   dwBeginTime;
    DWORD   dwLen;
    //DWORD   dwMemOffset;
}MP4SP_RECORD, *PMP4SP_RECORD;


// elmer,
typedef struct	tagMP4SP_INDEX
{
    WORD    wMMSPRecordEntry;
    WORD    wMMSPRecordEnd;
}MP4SP_INDEX, *PMP4SP_INDEX;

typedef struct  tagFILE_LOG
{
    BYTE            bMaxStringLen;      // include end of strng "\0"
    BYTE            bGapbyString;
    WORD            wMaxNO;
    WORD            wActualNO;
    WORD            wTotalNO;
    WORD            wStartNO;           // "0" is start, new from WinDVD 0.50
    WORD            wAttrFilter;        // ** TCH1.01b; only support which FILE_FLAG_XXXX
    BYTE            bDirIndex;
    // wyc1.00, record the ending sector position of current parsing dir.
    DWORD           bDirSectorNum;
    // wyc1.25, use pFileRecord to replace these two pointer.
    FILE_RECORD     *pFileRecord;
    void            * ptrVoid;
    char            * pName;
    WORD            * pAttr;            // FILE_FLAG_XXXX
    // wyc2.81, need to add TREE record into FILE_LOG for FManager to get the TREE record from CDINFO.
    BYTE           * pTree;
    // wyc1.10-909, pointer for DivX SP.
    MP4SP_RECORD      *pMP4SPRecord;
    BYTE            bSrcNum;
}   FILE_LOG, * PFILE_LOG;


//////////////////////////////////////////////////
// structure definition
typedef struct tagPATHTABLE
{
    BYTE  LEN_DI;
    BYTE  ExtendedAttribRecordLen;
    BYTE  LocationOfExtent0;
    BYTE  LocationOfExtent1;
    BYTE  LocationOfExtent2;
    BYTE  LocationOfExtent3;
///    WORD  ParentDirNum;
    BYTE  ParentDirNum[2];
    char  DirectoryID[128];
} PACK_FORMAT PATHTABLE;


typedef struct tagDIRRECORD
{
    BYTE  LEN_DR;
    BYTE  ExtendAttribute;
    BYTE    LocationExtent1 [4];        // ** TCH0.452;
    BYTE  LocationExtent2_0;
    BYTE  LocationExtent2_1;
    BYTE  LocationExtent2_2;
    BYTE  LocationExtent2_3;
    BYTE    DataLen1[4];    // ** TCH0.452;
    BYTE    DataLen2[4];    // ** TCH0.452;
    char Time[7];         // Recording Date adn Time
    BYTE flags;
    BYTE FileUnitSize;
    BYTE InterleaveGap;
    BYTE VolumeSequence [4];    // ** TCH0.452;
    BYTE FileIDLen;
} PACK_FORMAT DIRRECORD;

typedef struct tagNAVIGATE_MODE
{
    BYTE        bNavigateMode;
    WORD        wDiscMediaType;
} NAVIGATE_MODE, *PNAVIGATE_MODE;

typedef struct tagVTOC
{
    BYTE  DescriptorType;
    char  StandardID[5];
    BYTE  DescriptorVer;
    char  Unused1;
    char SystemID[32];
    char VolumeID[32];
    char  Unused2[8];
    // ** TCH0.50; 80 bytes
///    DWORD VolumeSpace1;
    BYTE    cVolumeSpace1[4];
    BYTE  VolumeSpace2_0;
    BYTE  VolumeSpace2_1;
    BYTE  VolumeSpace2_2;
    BYTE  VolumeSpace2_3;
    char Unused3[32];
    // ** TCH0.50; 120 bytes
    DWORD VolumeSet;
    DWORD VolumeSequence;
    DWORD LogicBlockSize;
    DWORD PathSize1;
    DWORD PathSize2;
    DWORD LPath;
    DWORD LOPath;
    // ** TCH0.50; 148 bytes
    BYTE  MPath0;
    BYTE  MPath1;
    BYTE  MPath2;
    BYTE  MPath3;
    DWORD MOPath;
    // ** TCH0.50; 156 bytes
    char Root[34];
    char VolumeSetID[128];
    char IDs[256];
    char AppID[128];
} VTOC;

typedef struct  tagINFOVCD
{
    char             SystemID[8];
    unsigned char    VerID0;
    unsigned char    VerID1;
    char             DiscID[16];
    unsigned char    AlbumVolumes0;
    unsigned char    AlbumVolumes1;
    unsigned char    VolumesNo0;
    unsigned char    VolumesNo1;

//   below will used   by VIDEO_CD 2.00
    unsigned char    SizeMapTrackMotionPicture[13];
    unsigned char    StatusFlag;

    char             cPSDSize[4];
    unsigned char    cSegmentAddress_BCD[3];
    unsigned char    cMultiplier;
    char             cMaxListID[2];
    char             cMaxSegmentNO[2];
    unsigned char    cSegmentPlayItem_Table [1980];
    unsigned char    cReserved[12];
} INFOVCD;

typedef struct tagENTRY
{
    char            FileID[8];
    int             SpecID;
    unsigned char   EntryUsed0;
    unsigned char   EntryUsed1;
    unsigned char   Track1;
    unsigned char   Track2;
    unsigned char   Entry1;
} ENTRY;

typedef struct tagTRACKINFO
{
    BYTE        bTrackNO;
    BYTE        bTrackInfo;
    DWORD       dwSectors;
} TRACKINFO;

typedef struct  tagPlayList
{
    unsigned char   cListHeader;
    unsigned char   cNOI;
    unsigned char   cListID[2];
    unsigned char   cPreviousOffset[2];
    unsigned char   cNextOffset[2];
    unsigned char   cReturnOffset[2];
    unsigned char   cPlayTime [2];
    unsigned char   cPlayItemWaitTime;
    unsigned char   cAutoPauseWaitTime;
    unsigned char   cItemNO [1000][2];    // suppose not exceed 1999
                                        // need to change to dynamic
}   PLAYLIST;

typedef struct  tagSelectionList
{
    unsigned char   cListHeader;
    unsigned char   cReserved;
    unsigned char   cNOS;
    unsigned char   cBSN;
    unsigned char   cListID[2];
    unsigned char   cPreviousOffset[2];
    unsigned char   cNextOffset[2];
    unsigned char   cReturnOffset[2];
    unsigned char   cDefaultOffset[2];
    unsigned char   cTimeOutOffset[2];
    unsigned char   cWaitTime;
    unsigned char   cLoopCount;
    unsigned char   cPlayItemNO[2];
    unsigned char   cSelectionNO [100][2];    // range from 0 - 99
}   SELECTIONLIST;

// elmer,
typedef struct tagFAT12ENTRY
{
	BYTE bEntry[3];
} PACK_FORMAT FAT12ENTRY, *PFAT12ENTRY;


///////////////////////////////////////////////////////
// Extern variables to other to reference part
extern  char            * __cSector1, * __cSector2, * __cSector3, * __cSector4;       // ** TCH0.50;

extern  WORD                __wCDROMReadingRange;

extern  char                _cS [];     // ** TCH0.31;

extern  BYTE            __bEXTAttr;
extern  DWORD   __dwSectorDir;
extern  BYTE    __bDVDROM;
extern  BYTE    __bFileSystem;

extern      DWORD       __dwSectorLastSession;
extern  BYTE    _bCDROMDecodingMode;
extern  DWORD       __dwSectorPSD;      //It'll record PSD sector NO. as saving read time
// wyc2.60-909P,
extern  BYTE    __dwLPath;
extern  DWORD   _dwSectorMPath;
extern  DWORD               _dwMPathSize;       // record MPath Size
extern  WORD        __wOffsetPSD;   // share with cdinfo2.c
extern  DWORD       __dwSectorLOT;      //It'll record PSD sector NO. as saving read time
extern  DWORD           __dwCD2Sector;
extern  WORD            __wStart_INFOVCD, __wStart_ENTRIES;
extern  BYTE    __bVTSN_MultiKeys;
extern  DWORD   __dwSector_MultiKeys [];
// wyc2.55-909P-AP,
extern BYTE    __bIFParseMode;

// FAT extern variables.
#ifdef SUPPORT_FAT_FILE_SYSTEM
extern  BYTE __bSecsPerClu;
extern WORD __wDataStartSec;
extern WORD __wBYTEsPerSec;
#endif //
// UDF extern variables.
#ifndef NO_CDDISC //++CoCo2.37p
extern char *__cSectorFE;
extern DWORD _dwPDLoc, _dwFSDLoc, _dwFELoc, _dwFIDLoc;
extern WORD _wFIDNumber, _wFIDPoint;
extern BYTE _bDirNO;
extern WORD _wFileNO;
#endif //#ifndef NO_CDDISC //++CoCo2.37p

// elmer
#ifdef SUPPORT_CHAR_SUBPICTURE
extern BYTE __bAVIFileCnt, __bMP4SPFileCnt;
#endif

// wyc2.21-909S, extern variable.
extern DWORD _dwIFTempBuffAddr;
////////////////////////////////////////////////
// export Functions
// Export functions that located in INFOFILTER module.
BOOL    INFOFILTER_GetFileSystem (void);
BOOL    INFOFILTER_GetDiscTypeFromSource (void);
WORD    INFOFILTER_RecognizeMedia (void);
#ifndef NO_CDDISC //++CoCo2.37p
void        INFOFILTER_PrepareVCD ( WORD wDiscType );
#endif //#ifndef NO_CDDISC //++CoCo2.37p
BYTE    INFOFILTER_AbortParsing(void);
BYTE    INFOFILTER_TriggerParsingInfo(BYTE	bSourceID);
BYTE    INFOFILTER_ParseInformation(BYTE	bSourceID);
BYTE    INFOFILTER_GetParsingStatus(BYTE	bSourceID);
BYTE    INFOFILTER_TriggerDeletingInfo(BYTE	bSourceID);
BYTE    INFOFILTER_DumpSectors (DWORD   dwSecLBA, WORD  wSecNO, BYTE* pDumpBuff, DWORD dwDumpBytes, WORD wByteOffset);

void    INFOFILTER_Thread( DWORD index );
void    INFOFILTER_ThreadInit(void);
void    INFOFILTER_ThreadDelete(void);
BYTE INFOFILTER_Detect_HDCD(void);
BYTE INFOFILTER_Detect_CDDTS(void);
BYTE INFOFILTER_Detect_CDG(void);
void HAL_DetectChipVersion(void); //kevin0.80a
void INFOFILTER_ResetDumpSectors(void);
// wyc2.55-909P-AP,
void INFOFILTER_SetParseMode(BYTE bParseMode);

// wyc2.17b-909S,
void    INFOFILTER_CFGUserData(DWORD    dwUserDataAddr);
// wyc2.21-909S,
void    INFOFILTER_SetTempBuff(DWORD    dwBuffAddr);
void INFOFILTER_CFGCDGDetectBuff(DWORD dwBuffST, DWORD dwBuffEND, DWORD dwBuffENDNOCDG);
// wyc2.36a-909s,
BYTE INFOFILTER_FUSSMPEGCD (BYTE bMode, BYTE bParam);
//winnie 2.52, this nwe function to check special vcd disc using mpeg sector format 0x000001BA
// wyc2.53-909p,
BOOL _INFOFILTER_CheckMPEGVCD(void);
// wwyc2.01-909S, prototype.
// wyc2.14-909s, move function to utl.h.
//void INFOFILTER_ChgDumpMode(BYTE bDumpMode);

// Export functions that located in CDINFO module.
BOOL    CDINFO_GetTrackNO ( BYTE * StartTrack, BYTE * EndTrack, DWORD * LeadOut );
BOOL    CDINFO_GetTrackInfo ( BYTE TrackNO, DWORD * Startpoint, BYTE * TrackInfo );
BOOL    CDINFO_ParseInfo ( WORD nMode, DWORD * pdwVal, WORD nSubMode );
BYTE    CDINFO_CheckSegPlayItem ( WORD nMode, WORD nItem );
WORD    CDINFO_GetList ( WORD nMode, WORD nSubMode );
WORD    CDINFO_ParseList ( WORD nMode, WORD * pWord );
WORD        CDINFO_GetListIDOffset ( WORD nItem );
DWORD   CDINFO_SearchStartSector ( DWORD dwStartSector );
#ifdef    TEST
BOOL        CDINFO_ParseInfoString ( WORD nWord, WORD nTrack, char * pChar, int iLen );
#endif    // TEST

//  Parse ISO9660
#ifndef NO_CDDISC //++CoCo2.37p
BOOL    CD_DetectCVDEncryption (void);
BOOL    CD_GetEntryInfo ( BYTE bType, DWORD * pVal );
BYTE    CD_GetEntryByFileName(char *pFileName, char *pDirName, DWORD *dwFileLoc, DWORD *dwFileLength);
WORD    CD_GetFirstEntry ( char * pEntry );
WORD    CD_SetDirectoryLocation ( DWORD dwSector );
WORD    CD_GetEntrybyIndex ( BYTE bIndex, char * pEntry );
WORD    CD_GetTotalEntriesbyDir ( PFILE_LOG pFILELog );       // ** TCH0.38;
WORD    CD_GetTotalEntriesbyPath ( PFILE_LOG pFILELog );      // ** TCH0.38;
DWORD   CD_ParserDirectory ( char * pDir, char * pExt );
#endif //#ifndef NO_CDDISC //++CoCo2.37p

// wyc1.01-909, move prototype to header file to let FAT module can use this function.
BYTE    CD_CheckExtName(char   *pExtName  );
// wyc2.55-909P-AP,
BYTE    CD_CheckAPExtName(char   *pExtName  );
// wyc1.11a, new function to parse MP4 char SP file.
BYTE    CD_CheckMP4SPExtName(char   *pExtName  );

#ifndef NO_CDDISC //++CoCo2.37p
// Export functions that located in UDF module. add by scping
extern BYTE UDF_CheckFileSystem(void);
extern void UDF_BuildFileSystem(PFAT_FS_FAT_DIRLOG pUDFDir_Log, PFILE_LOG pFile_Log);
extern DWORD _UDF_ReadAVDP(void);
extern DWORD _UDF_GetFSD(DWORD dwPDLoc);
extern DWORD _UDF_GetRoot(DWORD dwFSDLoc);
extern DWORD _UDF_ReadRootFE(DWORD dwFELoc);
extern BYTE _UDF_ReadFID(DWORD dwFIDLoc, PFAT_FS_FAT_DIRLOG pUDFDir_Log, PFILE_LOG pFile_Log);
extern BYTE _UDF_ReadFE(DWORD dwFELoc, PFAT_FS_FAT_DIRLOG pUDFDir_Log, PFILE_LOG pFile_Log);
extern WORD UDF_SwapWORD(WORD wData);
extern DWORD UDF_SwapDWORD(DWORD dwData);
extern BYTE UDF_CheckLayerJump(DWORD dwID, DWORD dwLoc, DWORD dwLen);
//BYTE    UDF_ReadInfoData(WORD    wLBASector, BYTE* pDumpPointer);
//BYTE    UDF_GetFileSetLoc(void);
//BYTE    UDF_BuildFileSystem(void);
#endif //#ifndef NO_CDDISC //++CoCo2.37p

#ifdef SUPPORT_FAT_FILE_SYSTEM
extern BYTE FAT_BuildFileSystem(BYTE bSourceID, PFAT_FS_FAT_DIRLOG pFATDir_Log, PFILE_LOG pFile_Log, PMP4SP_INDEX pMP4SPIndex);
extern BYTE FAT_CheckUSBSecNum(DWORD *dwTimeNow, DWORD *dwTimeEnd, DWORD dwTimeBegin, DWORD *dwOffset);
extern BYTE FAT_GetBPBInfo(BYTE bSourceID);
// wyc2.15-909S, extern function.
extern DWORD FAT_GetNextClus(BYTE bCallByUSBSRC, DWORD dwClusNum);
extern BYTE FAT_AssignFATTableMem(DWORD* pFATTable);
extern WORD FAT_GetFATInfo(BYTE bFATInfoType);
extern DWORD FAT_ClusToSector(DWORD dwClusNum);
extern DWORD FAT_SectorToClus(DWORD dwSecNum);
extern BYTE FAT_SetWorkMem(BYTE* pFATBuff, WORD wMemSize);
extern void FAT_InitVars(void);
// wyc1.06-909, new function
extern BYTE FAT_GetEntryByFileName(char *pFileName, char *pDirName, DWORD *dwFileLoc, DWORD *dwFileLength);
#endif //

#endif  // __INFOFILTER_H__

