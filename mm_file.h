/**************************************************************************

        (C)Copyright Cheertek Inc. 2007,
           J200, all right reserved

        Product : DVD/DMP Firmware

        Date    : 2007.4.10
        Author  : Cheertek (J200 WYC/CoCo)
        Purpose : The file structure and preparation of media manager.
        Sources : mm_file.h

****************************************************************************/
#ifndef __MM_FILE_H__
#define __MM_FILE_H__


//////////////////////////////////////////////////////////////
// If define it, it will convert the unknown name into "MP3XXX" or "JPGXXX"
// Otherwise, it will display "_" to present the unknown char
//#define CONVERT_UNKNOWN_NAME

// If define it, F/W will discard the empty directory -- LLY2.80
// Otherwise, the empty directory will be always kept
//#define DISCARD_EMPTY_DIRECTORY

////////////////////////////////////////////////////////////////
#define FILE_HANDLE_DEPTH_OF_FILE_PATH      256
#define SUPPORT_DIR_LENGTH                  8
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// Max directory number which F/W can support
// Modify this value to support more or less directory in a disc
#define MM_MAX_DIR_NO   150  // max. number of directory

// Max file number which F/W can support
// Modify this value to support more or less files in a disc.
// but if exceed 256, need do more extra chage on some variables
#define MM_MAX_FILE_NO_ONE_TITLE     ((((DS_FW_BUFFER_END_MM - DS_FW_BUFFER_ST_MM) - (MM_MAX_DIR_NO*sizeof(DIRNAMEINFO)))/sizeof(FILE_RECORD)) - 100)
#define  MM_MAX_PARSE_DIR_SECTORS       (MM_MAX_FILE_NO_ONE_TITLE / 30)

// The max file number for once time CDROM parsing(SRAM buffer issue)
// LLY2.81, reduce max file number of once time, 330 --> 200
// Because, we must reserve some SRAM space to store directory tree information
// Notice: will support multi-parsering of one directory
#define MM_MAX_FILE_NO_ONCE_TIME     150 

// wyc1.10-909, the define of support number of DivX SP files.
#define MM_MAX_SRT_NUM              50
////////////////////////////////////////////////////////////////
// ***** Some define for function parameter usage
// Use for FM_PreparePage() 1st parameter -- bPageID
#define MM_PREPARE_FILEPAGE             0
#define MM_PREPARE_PROGLIST             100

////////////////////////////////////////////////////////////////
// Use for _QueryInfo() 1st parameter -- bQueryID
#define MM_QUERY_DIR_INDEX                  0
#define MM_QUERY_UNKNOWN_CHAR               1

// LLY0.83b-2, Splite into two groups for CT9928AF & CT908 UI
#define MM_QUERY2_TOTAL_ITEM_OF_DIRPAGE         101
#define MM_QUERY2_TOTAL_SORT_FILTER_ITEM        102

////////////////////////////////////////////////////////////////
// Use for _MM_ConvertName() 1st parameter -- bType
#define MM_CONVERT_DIRNAME          1
#define MM_CONVERT_FILENAME         2

////////////////////////////////////////////////////////////////
// Use for MM_AccessJPGStatus() parameter  -- bMode
#define MM_GET_JPG_STATUS           1
#define MM_STORE_JPG_STATUS         2

////////////////////////////////////////////////////////////////
// Use for _FM_FileIndexConvert() 1st parameter: bMode -- LLY0.83b-2
// directory <--> disc coordinate index convert
#define MM_INDEX_CONVERT_GROUP0             0
#define MM_INDEX_CONVERT_DIR2DISC           (MM_INDEX_CONVERT_GROUP0 + 0)
#define MM_INDEX_CONVERT_DISC2DIR           (MM_INDEX_CONVERT_GROUP0 + 1)
// wyc2.38a-909s, add one case to report value bu sort/filter mode.
#define MM_INDEX_CONVERT_DIR2DISC_BY_FILTER (MM_INDEX_CONVERT_GROUP0 + 2)
// absolute <--> relative coordinate index convert
#define MM_INDEX_CONVERT_GROUP1             10
#define MM_INDEX_CONVERT_ABS2REL_DIR_LIST   (MM_INDEX_CONVERT_GROUP1 + 0)
#define MM_INDEX_CONVERT_REL2ABS_DIR_LIST   (MM_INDEX_CONVERT_GROUP1 + 1)
#define MM_INDEX_CONVERT_ABS2REL_FILE_LIST  (MM_INDEX_CONVERT_GROUP1 + 2)
#define MM_INDEX_CONVERT_REL2ABS_FILE_LIST  (MM_INDEX_CONVERT_GROUP1 + 3)
#define MM_INDEX_CONVERT_GROUP2             20

////////////////////////////////////////////////////////////////
#define UP_DIR							0xFE   // up directory
#define UNEXIST_DIR						0xFF   // unexist directory

////////////////////////////////////////////////////////////////
#define MM_FILE_RECORD_DUMP_FAILED      0xFFFF

////////////////////////////////////////////////////////////////
#define MM_VOLUME_SERVO					0
#define MM_VOLUME_USB_UDISC_A			1
#define MM_VOLUME_USB_UDISC_B			2
#define MM_VOLUME_CARD_SD_A				3
#define MM_VOLUME_CARD_CF_A				4
#define MM_VOLUME_CARD_SM_A				5
#define MM_VOLUME_CARD_MMC_A			6
#define MM_VOLUME_CARD_MS_A				7
#define MM_VOLUME_CARD_MICRO_A			8
#define MM_VOLUME_CARD_SD_B				9
#define MM_VOLUME_CARD_CF_B				10
#define MM_VOLUME_CARD_SM_B				11
#define MM_VOLUME_CARD_MMC_B			12    
#define MM_VOLUME_CARD_MS_B				13
#define MM_VOLUME_CARD_MICRO_B			14

////////////////////////////////////////////////////////////////
#define MM_SORT_BY_TYPE					0x1
#define MM_SORT_BY_NAME					0x2
#define MM_SORT_BY_TIME					0x4    
#define MM_SORT_BY_ASCENDING			0x10
#define MM_SORT_BY_DESCENDING			0x20

#define MM_SORT_CONN_PREV_EXT			(0xFF66)
#define MM_SORT_CONN_NEXT_EXT			(0xFF67)
#define MM_SORT_INIT_VALUE				(0xFF68)

// Follow define is used for _MM_GetSortRecord
#define MM_SORT_NEXT_RECORD				0
#define MM_SORT_PREV_RECORD				1

// wyc2.60-909P, modify for unicode case
#define MM_SORT_FILTER_LINK_READY       0x1
#define MM_SORT_FILTER_LINK_NOT_READY   0

// define the supported sorting types, now support sort by name, by type and time.
#define MM_SORT_TYPE					3

////////////////////////////////////////////////////////////////
#define MM_GET_FILE_YEAR				1
#define MM_GET_FILE_MONTH				2
#define MM_GET_FILE_DATE				3
#define MM_GET_FILE_HOUR				4
#define MM_GET_FILE_MIN					5
#define MM_GET_FILE_SEC					6
#define MM_GET_FILE_LENGTH				7

////////////////////////////////////////////////////////////////
// *****  Structure Define Area  *****
// [0] Menu Page info. structure
// LLY2.80, re-adjust the element structure
// (1) Reduce some variables unit from WORD to BYTE:
//     ex. wNOofItems/ wTotalPages/ wPageIndex
// (2) Replace some variables with global variables:
//     ex. wDirIndex
// (3) Remove __wHighLight/ __wSelection
typedef struct tagMENUPAGEINFO
{
    // Index always from "0", ie. 0 means 1st directory/ file, 1 means 2nd directory/ file, ...
    WORD    w1stID;     // 1st item index of current file list page
    BYTE    bNOofItems;     // number of items to be display for current file list page
    BYTE    bTotalPages;    // total page number of current directory/file menu page
    BYTE    bPageIndex;     // the current page index of current directory/ file menu page
}   MENUPAGEINFO, * PMENUPAGEINFO;

// [3] DRAM location for File Relative Buffer -- LLY2.80
typedef struct tagDRAM_LOC
{
    DWORD   dwFileName;     // starting DRAM address of file name buffer
    DWORD   dwFileTOC;      // starting DRAM address of file TOC buffer
    DWORD   dwJPGStatus;    // starting DRAM address of JPG status buffer
}   DRAM_LOC, * PDRAM_LOC;

////////////////////////////////////////////////////////////////
//Extern variables
extern DWORD _dwMMTemp; //Kevin1.08
extern DWORD _dwMMTemp1;
extern DWORD _dwMMTemp2;
extern DWORD _dwMMTemp3;
extern DWORD _dwMMTemp4;
extern DWORD _dwMMTemp5;
extern DWORD _dwMMTemp6;
extern DWORD _dwMMTemp7;
extern DWORD _dwMMTemp8;
extern DWORD _dwMMTemp9;
extern WORD _wMMTemp;
extern BYTE _bMMIdx;
extern PDIRNAMEINFO __pMMDirNameList;
extern WORD *__pMMFilesOfDir;
extern WORD *__pMMFilterFilesOfDir;
extern BYTE __bMMTotalDirs;
extern WORD __wMMTotalFiles;
extern WORD __wMMTotalFilterFiles;
extern PTREE_INFO __pMMVolumeDirTree;
extern BYTE _bMMParentID;
extern PFILENAMEINFO __pMMProgContent;
extern BYTE __bMMVolumeNum;
extern BYTE __bMMExtFilterMode;
extern BYTE __bMMExtFilterModeInMMMode;
extern BYTE __bMMSortMethod;
extern BYTE __bMMSortSequence;
extern BYTE    __bMMVolumeSet [1];
extern PFILE_RECORD __pMMFileRecord;
extern PFILE_RECORD __pMMFileRecordStart;
extern BYTE *__pFMComFile;
extern BYTE __bMaxExtNO;
extern BYTE __bTotalSRTFiles;
extern BYTE _bMMExistUpDir;
extern DWORD __dwMMFWBuffEnd;

////////////////////////////////////////////////////////////////
//Internal functions
WORD _MM_GetDir1stID(BYTE bDirIdx);
WORD _MM_QueryInfo1(BYTE bQueryID, WORD wParam);
WORD _MM_QueryInfo2(BYTE bQueryID, WORD wDirID);
BYTE _MM_PrepareInfo(void);
BYTE _MM_SortFilterFiles(WORD wDirID, BYTE bFilterMode, BYTE bSortMode);
DWORD _MM_CheckExtFilteredIn(BYTE bExtID, BYTE bFilterMode);
WORD _MM_GetSortRecord(WORD wSortIndex, BYTE bType);
void _MM_ConvertName(BYTE bType, WORD wIndex);
WORD _MM_GetNoneJPEGItems(void);
void _MM_PrepareFilterFile(void);
#ifdef SUPPORT_CHAR_SUBPICTURE
void _MM_GetMP4SP(PFILE_LOG pFileLog);
#endif //SUPPORT_CHAR_SUBPICTURE

////////////////////////////////////////////////////////////////
//External functions
BYTE MM_CheckPlayableCDROM(void);
void MM_GetRange(WORD wDirID, WORD wIndex, PDWORD pdwStart, PDWORD pdwEnd);
BYTE MM_GetFileType(WORD wDirID, WORD wIndex);
WORD MM_IndexConvert(BYTE bMode, BYTE bDir, WORD wIndex);
void MM_PreparePage(BYTE bPageID, WORD wDirID, BYTE bMMExtFilterMode, BYTE bMMSortMethod);
BYTE MM_ParseDirFiles(WORD wDirID, BYTE bFileType, WORD wFileIndex);
void MM_AccessJPGStatus(BYTE bMode, WORD wIdx);
WORD MM_ID_AbsoluteToRelative(DWORD bAbsID);
WORD MM_ID_RelativeToAbsolute(BYTE bDirID, WORD wRelID);
void MM_TriggerDumping(WORD wAbsID, WORD wNum);
WORD MM_GetDumpedFileRecord(void);
DWORD MM_GetFileInfo(DWORD dwType);

#ifdef SUPPORT_CHAR_SUBPICTURE
BYTE MM_GetSRTRecord(PDWORD pdwSecLBA, PDWORD pdwSecNO, PBYTE bMP4SPType);
#endif //SUPPORT_CHAR_SUBPICTURE

#ifdef SUPPORT_FILE_HANDLE
FILE_RANGE MM_GetFileRange(WORD wID);
BYTE    MM_SearchFileByFileRecord(char* pFilePathName, WORD* wFileOffset);
#endif //SUPPORT_FILE_HANDLE

#ifdef DISCARD_EMPTY_DIRECTORY
BYTE _MM_ExistEmptyFolder(void)
#endif //DISCARD_EMPTY_DIRECTORY

void MM_File_Initial(void);
////////////////////////////////////////////////////////////////


#endif //__MM_FILE_H__
