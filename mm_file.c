//////////////////////////////////////////////////////////////
// ****** All necessary including files Area ******
#include    "winav.h"
// wyc0.60-909, include ctkav.h and <cyg/kernel/kapi.h> for thread control.
#include "ctkav.h"
#include    <string.h>
#include    "input.h"
#include    "infofilter.h"
#include    "cc.h"
#include    "osd.h"
#include    "mmanager.h"
#include    "utl.h"
#include    "initial.h" 
#include    "SrcFilter.h"
#include    "monitor.h"
#include    "setup.h"
#include    "chips.h"
#include    "hal.h"
#include    "linear.h"
#include    "panel.h"
#include    "parser.h"
#include    "disp.h"
#include    "disp.h" 
#include    "media.h"

#ifdef SUPPORT_POWERON_MENU
#include "poweronmenu.h"
#endif //SUPPORT_POWERON_MENU

#ifdef SUPPORT_CHAR_SUBPICTURE
#include "char_subpict.h"
#endif

#include    "comutl.h"

#include "debug.h"
#undef PRINTF
#undef printf
// Micky2.17c
#ifdef	APPLICATION_WINDOWS
#define PRINTF		DBG_Printf_Simple
#define printf		DBG_Printf_Simple
#else
#define PRINTF(x, y...)     DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, x, ## y)
#define printf(x, y...)     DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, x, ## y)
#endif

///////////////////////////////////////////////////////////////////////////
// File Log structure
// [G-19bytes]
FILE_LOG _MMFileLog;
// scping, for parse UDF dir record usage.
FAT_FS_FAT_DIRLOG _MMUDFDirLog;
// wyc1.05-909, for parse FAT dir record usage.
#ifdef SUPPORT_FAT_FILE_SYSTEM
FAT_FS_FAT_DIRLOG _MMFATDirLog;
DWORD *_pMMFATTable;
#endif //
///////////////////////////////////////////////////////////////////////////
PDIRNAMEINFO __pMMDirNameList;
// This variable is used to temp reference dir / file name to do some temp processing. For example, convert dir / file name.
PFILENAMEINFO  _pMMTempNameList; // NULL;
// wyc1.25, add pointer to use FILE_RECORD in SRAM
PFILE_RECORD __pMMFileRecord;
PFILE_RECORD __pMMFileRecordStart;
PFILE_RECORD __pMMFileRecordTemp;
PDIR_LOC _pMMDirLoc;
PMP4SP_RECORD     __pMMMP4SPRecord;
#ifdef SUPPORT_CHAR_SUBPICTURE
PMP4SP_RECORD     __pMMMP4SPRecordStart;
// elmer, record the index of sub-titles in each dir
PMP4SP_INDEX _pMMMP4SPIndex;
#endif //SUPPORT_CHAR_SUBPICTURE
BYTE    __bTotalSRTFiles;
//////////////////////////////////////////////////////////////
DWORD   __dwMMFWBuffEnd;  // LLY2.36, keep current used F/W buffer end address.
///////////////////////////////////////////////////////////////////////////
WORD *__pMMFilesOfDir;
WORD *__pMMFilterFilesOfDir;
BYTE *__pFMComFile;
BYTE __bMMTotalDirs;
WORD __wMMTotalFiles;
PTREE_INFO __pMMVolumeDirTree;
BYTE _bMMParentID;
// Keep the info. while exist "UP_DIR" or not -- LLY0.83b-2
BYTE _bMMExistUpDir;

// wyc2.38a-909s, varoable to record the total files after sort/filter mode.
WORD __wMMTotalFilterFiles;
DWORD _wMMGetRangeIndex;
// used for supporting parsing more files in one directory.
BYTE _bMMDirParseIdx;
///////////////////////////////////////////////////////////////////////////
WORD _wStartDumpAbsID;
WORD _wMMUnDumpFileRecord;
WORD _wNextFileRecord;
BYTE _bMMFirstDumped;
///////////////////////////////////////////////////////////////////////////
BYTE __bMMVolumeSet [1]; // Now use 1 us because only support SERVO media now, correct way need to define by max. support media and need to wait Brian's define.
BYTE __bMMVolumeNum = 1;
///////////////////////////////////////////////////////////////////////////
DWORD   _dwMMTemp; 
DWORD   _dwMMTemp1;
DWORD   _dwMMTemp2;
DWORD   _dwMMTemp3;
DWORD   _dwMMTemp4;
DWORD   _dwMMTemp5;
DWORD   _dwMMTemp6;
DWORD   _dwMMTemp7;
DWORD   _dwMMTemp8;
DWORD   _dwMMTemp9; 
WORD    _wMMTemp;
BYTE    _bMMIdx;
///////////////////////////////////////////////////////////////////////////
BYTE    __bMMUniFile;
///////////////////////////////////////////////////////////////////////////
PFILENAMEINFO    __pMMProgContent;
///////////////////////////////////////////////////////////////////////////
BYTE    __bMMExtFilterMode;
volatile BYTE    __bMMExtFilterModeInMMMode;
BYTE    __bMMSortMethod;
BYTE    __bMMPreExtFilterMode;
WORD    __wMMPreFilterDir;
BYTE    __bMMPreSortMethod;
BYTE    __bMMSortSequence;
BYTE    __bMaxExtNO;
WORD    _wMMExtFirstID[MAX_EXTENSION_NO];
WORD    _wMMExtLastID[MAX_EXTENSION_NO];
WORD    _wSortIndex;
///////////////////////////////////////////////////////////////////////////
DWORD   _dwMMLastRelID, _dwMMLastAbsID;
///////////////////////////////////////////////////////////////////////////

//#######################################################################
//  Function    :   MM_CheckPlayableCDROM
//  Description :   This function check if the disc contain playable files
//                  and will prepare all necessary information of these files
//  Parameters  :   None.
//              :
//  Return      :   TRUE,   a playable CDROM disc
//              :   FALSE,  not a playable CDROM disc
//  Changed     :
//  Side Effect :
//#######################################################################
BYTE MM_CheckPlayableCDROM(void)
{
    // wyc1.07, extern these variables for detect if CDROM detecting error.
    BYTE   *__bFMBuf;
	// the following information will be available
	// 1. __pMMDirNameList      // each directory name list
	// 3. _pFMFileTOC           // each file starting sector and length
	// 4. _bAblum1stID          // 1st song ID of each album
	// 5. __pMMFilesOfDir       // total songs of each album
	
    // wyc1.07, detect if CDROM detecting error, if error use ISO mode to re-try again.
    if ( !_MM_PrepareInfo() )
    {
        if (__SF_SourceGBL[__dwSFStreamID].bSourceIndex == SOURCE_DISC)
        {
            SrcFilter_Stop(__dwSFStreamID);
            INFOFILTER_GetFileSystem();
            __bFMBuf= (BYTE * ) __cSector3;
            _dwSectorMPath= ((DWORD)( __bFMBuf [148] )<<24);
            _dwSectorMPath+= ((DWORD)( __bFMBuf [149] )<<16);
            _dwSectorMPath+= ((DWORD)( __bFMBuf [150] )<<8);
            _dwSectorMPath+= (DWORD)( __bFMBuf [151] );
            //__bFileSystem = FILE_SYSTEM_ISO9660;
            if ( !_MM_PrepareInfo() )
                return FALSE;
        }
        else
            return FALSE;
    }
	
    return TRUE;
}

//#######################################################################
//  Function    :   MM_GetRange
//  Description :   This function will return the play range for each file
//  Parameters  :   wIndex, the file index to get play range, from '1'
//  Return      :
//  Changed     :   __dwTimeBegin,  the starting sector
//              :   __dwTimeEnd,    the ending sector
//  Side Effect :
//#######################################################################
void MM_GetRange(WORD wDirID, WORD wIndex, PDWORD pdwStart, PDWORD pdwEnd)
{
    DWORD   dwTimeBegin, dwTimeEnd;
    WORD wIndex1;
	
    // 1. Get current file's starting sector to __dwTimeBegin
    // 2. Get current file's length
    // 3. __dwTimeEnd=__dwTimeBegin + file length
    // 4. __dwTimeEnd=temp - GAP
	
    // LLY.274p-2, decrease wIndex by 1, because the array index is from '0'
    wIndex--;
	
    // LLY0.83b-2, calculate the corresponding index by "disc view"
    // Because, disc file's TOC is stored into DRAM together in "disc view"
    // wyc0.71, wIndex is the offset in MM buffer, so don't need to
    _wMMGetRangeIndex=MM_ID_RelativeToAbsolute(wDirID, wIndex);
    wIndex1=MM_ID_RelativeToAbsolute(wDirID, wIndex);
    wIndex=MM_ID_RelativeToAbsolute(wDirID, wIndex);
    printf("\nGet Range: Ori:%x, New:%x, local new:%x",wIndex,_wMMGetRangeIndex,wIndex1);
	
    // Get __dwTimeBegin and __dwTimeEnd
    // wyc1.07-909, use 4 BYTES to record file range.
    dwTimeBegin = 0;
    dwTimeBegin |= __pMMFileRecordStart[_wMMGetRangeIndex].FileRange.cS[0];
    dwTimeBegin <<=24;
    dwTimeBegin |= ((WORD)__pMMFileRecordStart[_wMMGetRangeIndex].FileRange.cS[1])<<16;
    dwTimeBegin |= ((WORD)__pMMFileRecordStart[_wMMGetRangeIndex].FileRange.cS[2])<<8;
    dwTimeBegin |= __pMMFileRecordStart[_wMMGetRangeIndex].FileRange.cS[3];
    // wyc1.10, change wLen to dwLen because of structure have change from WORD to DWORD.
    dwTimeEnd = dwTimeBegin + __pMMFileRecordStart[_wMMGetRangeIndex].FileRange.dwLen-1;
    // wyc1.01-909, when FAT file system we need to transfer cluster unit to sector unit when want to play from USB source.
    // wyc1.01-909, FAT need to do some transfer for get correct sector in FAT file system.
#ifdef SUPPORT_FAT_FILE_SYSTEM
    if ((__bFileSystem == FILE_SYSTEM_FAT12) || (__bFileSystem == FILE_SYSTEM_FAT16) || (__bFileSystem == FILE_SYSTEM_FAT32))
    {
        // wyc1.01a, need to sent more than the real file size or equal, can't less than file size. Otherwise JPG can't find 0xFFD9 and will decode failed.
        // Begin is USB sector location, size is parser sector unit, end is pesudo to compute and just for checking all data are parsed in.
        dwTimeBegin = __wDataStartSec + (dwTimeBegin - FAT_START_CLUS_NUM)*FAT_GetFATInfo(FAT_INFO_SECTOR_PER_CLUSTER);
        dwTimeEnd = dwTimeBegin + (__pMMFileRecordStart[_wMMGetRangeIndex].FileRange.dwLen/PARSER_SECTOR_SIZE);
    }
#endif //
	
    *pdwStart = dwTimeBegin;
    *pdwEnd = dwTimeEnd;
	
#ifdef  DEBUG_MESSAGE
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"Item=%lx, Start=%lx, End=%lx, Sectors=%lx\n",
        wIndex, *pdwStart, *pdwEnd, (*pdwEnd-*pdwStart+1));
#endif  // #ifdef DEBUG_MESSAGE
	
}


//  ********************************************************************
//  Function    :   MM_GetFileType
//  Description :   Get the desired item's file type, ex MP3/ JPG
//  Arguments   :   The desired playing item, index from '1'
//  Return      :   File extension ID
//  Side Effect :
//  ********************************************************************
BYTE MM_GetFileType(WORD wDirID, WORD wIndex)
{
    // wyc1.10-3, add protection to avoid non-CDROM titles get wrong information in this function.
    if (__wDiscType & CDROM_M1)
    {
        // Decrease wIndex by 1, because the array index is from '0'
        wIndex--;
        // wyc0.72, use new filter/sort link mechansim to get data
        return __pMMFileRecordStart[MM_ID_RelativeToAbsolute(wDirID, wIndex)].FileName.bExtID;
    }
    return  FALSE;
}

//#######################################################################
//  Function    :   _MM_GetDir1stID
//  Description :   Get the 1st file ID within the desired directory
//  Parameters  :   bDirIdx, specify the desired directory ID, index from '0'
//  Return      :   the 1st file ID within the desired directory
//  Changed     :
//  Side Effect :
//  Global      :   _dwMMTemp2, _dwMMTemp5
//#######################################################################
WORD _MM_GetDir1stID(BYTE bDirIdx)
{
    // LLY2.81, always return total files number (index from '0')
    // if input number is out-of total directory
    if(bDirIdx >= __bMMTotalDirs)
    {
        return __wMMTotalFiles;
    }
	
    // Otherwise, go following procedure to do calculation
    _dwMMTemp2=0;
    for(_dwMMTemp5=0; _dwMMTemp5<bDirIdx; _dwMMTemp5++)
    {
        _dwMMTemp2+=__pMMFilesOfDir[_dwMMTemp5];
    }
    return _dwMMTemp2;
}

// **************************************************************************
//  Function    :   MM_IndexConvert
//  Description :   Convert desired index between different two coordinate
//                  - directory: always '0' for each directory's 1st file
//                  - disc: index is '0' only 1st directory's 1st file
//                          others are continue
//                  - absolute: absolute cursor index within the display window
//                              ex. it includes sub-direcory NO + file NO
//                  - relative: real directory/ file index,
//                             ex. 1st item: '0'; 2nd item: '1'; ....
//  Arguments   :   bMode, specify the convert mode
//                  [1] MM_INDEX_CONVERT_DIR2DISC, convert file index from
//                      directory to disc view
//                  [2] MM_INDEX_CONVERT_DISC2DIR, convert file index from
//                      disc to directory view
//                  [3] MM_INDEX_CONVERT_ABS2REL_DIR_LIST, convert cursor index
//                      from absolute to relative coordinate (directory list)
//                  [4] MM_INDEX_CONVERT_REL2ABS_DIR_LIST, convert cursor index
//                      from relative to absolute coordinate (directory list)
//                  [5] MM_INDEX_CONVERT_ABS2REL_FILE_LIST, convert cursor index
//                      from absolute to relative coordinate (file list)
//                  [6] MM_INDEX_CONVERT_REL2ABS_FILE_LIST, convert cursor index
//                      from relative to absolute coordinate (file list)
//                  bDir, specify the desired directory index, from '0'
//                  bIndex, specify the desired file index, from '0'
//  Return      :   the file index after this conversion
//  Side Effect :
//  Global      :   _dwMMTemp2
// **************************************************************************
WORD MM_IndexConvert(BYTE bMode, BYTE bDir, WORD wIndex)
{
    if(bMode < MM_INDEX_CONVERT_GROUP1) // means GROUP0: file index convert
    {
        // Calculate the final index after this conversion
        switch(bMode)
        {
        case    MM_INDEX_CONVERT_DIR2DISC:
            // Get 1st file index (in disc view) of current directory, index from '0'
            _dwMMTemp2=_MM_GetDir1stID(bDir);
            return (_dwMMTemp2+wIndex);
			
        case    MM_INDEX_CONVERT_DISC2DIR:
            // Get 1st file index (in disc view) of current directory, index from '0'
            _dwMMTemp2=_MM_GetDir1stID(bDir);
            return (wIndex-_dwMMTemp2);
			
        case    MM_INDEX_CONVERT_DIR2DISC_BY_FILTER:
            // Otherwise, go following procedure to do calculation
            _dwMMTemp2=0;
            for(_dwMMTemp5=0; _dwMMTemp5<bDir; _dwMMTemp5++)
            {
                _dwMMTemp2+=__pMMFilterFilesOfDir[_dwMMTemp5];
            }
            return (_dwMMTemp2+wIndex);
            
        default:
            break;
        }
    }
    else if(bMode < MM_INDEX_CONVERT_GROUP2) // means GROUP1: cursor index convert
    {
        // Calculate the final cursor index after the conversion
        switch(bMode)
        {
			// convert cursor index from absolute to relative coordinate within directory list
        case    MM_INDEX_CONVERT_ABS2REL_DIR_LIST:
            if(wIndex==0)
            {
                // Notice: always return current active directory ID if @ ROOT
                // nothing need to do, becasuse it's alwasy "ROOT"
                //return wIndex;
                return bDir;
            }
            else
            {
                if(__pMMVolumeDirTree[bDir].bParentID != 0x0) // 0x0=ROOT, so UP_DIR exist
                {
                    if(wIndex==1) // @ UP_DIR position
                    {
                        // Don't modify active directory ID if @ UP_DIR
                        return bDir;
                    }
                    else  // @ non-UP_DIR position
                    {
                        // final index = input index + current level's 1st sub-directory ID - ROOT - UP_DIR
                        // Notice: current level's 1st sub-directory ID =
                        //         1st sub-directory ID of desired directory's parent
                        _dwMMTemp5=__pMMVolumeDirTree[bDir].bParentID; // get parent ID first
                        return (wIndex + __pMMVolumeDirTree[_dwMMTemp5].b1stSubDirID - 2); // 2 = ROOT + UP_DIR
                    }
                }
                else // no UP_DIR exist
                {
                    // final index = index + ROOT's 1st sub-directory ID (0x1) - ROOT
                    return (wIndex);
                }
            }
			
			// convert cursor index from relative to absolute coordinate within directory list
        case    MM_INDEX_CONVERT_REL2ABS_DIR_LIST:
            if(wIndex==0)
            {
                // nothing need to do, becasuse it's alwasy "ROOT"
                return wIndex;
            }
            else
            {
                if(__pMMVolumeDirTree[bDir].bParentID != 0x0) // 0x0=ROOT, so UP_DIR exist
                {
                    if(wIndex==1) // @ UP_DIR position
                    {
                        return 1;
                    }
                    else
                    {
                        // final index = input index - current level's 1st sub-directory ID + ROOT + UP_DIR
                        // Notice: current level's 1st sub-directory ID =
                        //         1st sub-directory ID of desired directory's parent
                        _dwMMTemp5=__pMMVolumeDirTree[bDir].bParentID; // get parent ID first
                        return (wIndex + 2 - __pMMVolumeDirTree[_dwMMTemp5].b1stSubDirID); // 2 = ROOT + UP_DIR
                    }
                }
                else // no UP_DIR exist
                {
                    // final index = index - ROOT's 1st sub-directory ID (0x1) + ROOT
                    return (wIndex);
                }
            }
            break;
			
			// convert cursor index from absolute to relative coordinate within file list window
			// Notice: only valid on file index
        case    MM_INDEX_CONVERT_ABS2REL_FILE_LIST:
            // input index - desired directory's sub-directory NO
            return  (wIndex-__pMMVolumeDirTree[bDir].bNoOfSubDir);
			
			// convert cursor index from relative to absolute coordinate within file list window
			// Notice: only valid on file index
        case    MM_INDEX_CONVERT_REL2ABS_FILE_LIST:
            // input index + desired directory's sub-directory NO
            return  (wIndex+__pMMVolumeDirTree[bDir].bNoOfSubDir);
			
        default:
            break;
        }		
    }
	
    return wIndex;
}

//#######################################################################
//  Function    :   _MM_QueryInfo1
//  Description :
//  Parameters  :   bQueryID,
//                  wParam
//  Return      :
//  Changed     :
//  Side Effect :
//  Notice      :   Collect the cases don't care directory ID
//  Global      :   _dwMMTemp2, _dwMMTemp7
//#######################################################################
WORD _MM_QueryInfo1(BYTE bQueryID, WORD wParam)
{
    WORD wTemp;
    // Step 1: process the cases which are independent of active/ playing directory ID
    switch(bQueryID)
    {
		// LLY2.81, using another method to implement this case to enhance performance
		// Because, don't call _MM_GetDir1stID() again
		// LLY0.84, search un-support char
		// TRUE: input ASCII code is a unknown char
		// FALSE: input ASCII code can be recognized
    case    MM_QUERY_UNKNOWN_CHAR:
        // Keep input ASCII code into wTemp, and using it to do comparsion
        // To reduce the code while comparsion procedure
        wTemp=(WORD)wParam;
        if( (wTemp>=48 && wTemp<=57) ||   // '0' ~ '9'
            (wTemp>=65 && wTemp<=90) ||   // 'a' ~ 'z'
            (wTemp>=97 && wTemp<=122) ||  // 'A' ~ 'Z'
            (wTemp==32) ||  // ' '
            (wTemp==43) ||  // '+'
            (wTemp==45) ||  // '-'
            (wTemp==46) ||  // '.'
            (wTemp==47) ||  // '/'
            (wTemp==58) ||  // ':'
            (wTemp==95) ||  // '_'
            (wTemp==39) )  // "'"
            return FALSE;
        else
            return TRUE;
		
    default:
        break;
    }
	
    // LLY.172q, default return FALSE if there is no match-case
    return FALSE;
}

//  *********************************************************************
//  Function    :   _MM_QueryInfo2
//  Description :   Same as _QueryInfo w/o reference 2nd parameter
//                  to reduce the SRAM
//  Argument    :   bQueryID, the desired query ID
//  Return      :   The correspond information of the desired query ID
//  Side Effect :
//  Global      :   _dwMMTemp2/ _dwMMTemp5
//  Notice      :   Collect the cases independent of directory/ file index
//  *********************************************************************
WORD _MM_QueryInfo2(BYTE bQueryID, WORD wDirID)
{
    switch(bQueryID)
    {
    case    MM_QUERY2_TOTAL_ITEM_OF_DIRPAGE:
        if(_bMMParentID == UNEXIST_DIR)
            return 1;
        return (1 + _bMMExistUpDir + __pMMVolumeDirTree[_bMMParentID].bNoOfSubDir);
		
    case    MM_QUERY2_TOTAL_SORT_FILTER_ITEM:
        return (__pMMFilterFilesOfDir[wDirID] + __pMMVolumeDirTree[wDirID].bNoOfSubDir);
    default:
        break;
    }
    return  FALSE;
}

//#######################################################################
//  Function    :   MM_PreparePage
//  Description :   Prepare the display menu page info.
//  Parameters  :   bPageID, the desired menu page style
//                  wParam
//  Return      :
//  Changed     :
//  Side Effect :
//  Global      :   _dwMMTemp2, _dwMMTemp1
//#######################################################################
// LLY2.81, implement new code to meet CT908 UI requirement
void MM_PreparePage(BYTE bPageID, WORD wDirID, BYTE bMMExtFilterMode, BYTE bMMSortMethod)
{
    // Step 1: Prepare the necessary info while draw one page
    switch(bPageID)
    {
    case    MM_PREPARE_FILEPAGE:
        // Update final parent directory ID
        // wyc2.38a-909s, add parameter wDirID in function.
        _bMMParentID=__pMMVolumeDirTree[wDirID].bParentID;
		
        // Update if exist UP-directory
        if(_bMMParentID == UNEXIST_DIR) // current directory is "ROOT"
            _bMMExistUpDir=FALSE;
        else if(_bMMParentID == 0x0) // current directory at 1st level of directory tree
            _bMMExistUpDir=FALSE;
        else // others, ie. current directory at >= 2nd level of directory tree
            _bMMExistUpDir=TRUE;
		
        _dwMMLastRelID = 0xFFFFFFFF;
        _dwMMLastAbsID = 0xFFFFFFFF;
        // wyc0.67, need to sore file gere before display file name.
        _MM_SortFilterFiles(wDirID, bMMExtFilterMode, bMMSortMethod);
        break;
		
    case    MM_PREPARE_PROGLIST:
        for(_dwMMTemp7=0; _dwMMTemp7<__bProgramMaxEntry; _dwMMTemp7++)
        {
            // Get desired item's index w/ disc view
            // Notice: __ProgIdxList[].wSubIndex index from '1'
            //         but the index from '0' within FMANAGER module
            _dwMMTemp2 = MM_ID_RelativeToAbsolute(__ProgIdxList[_dwMMTemp7].bMainIndex, (WORD)(__ProgIdxList[_dwMMTemp7].wSubIndex-1));
			
            strncpy(__pMMProgContent[_dwMMTemp7].cName, __pMMFileRecordStart[_dwMMTemp2].FileName.cName, sizeof(FILENAMEINFO)-1);
            __pMMProgContent[_dwMMTemp7].bExtID = __pMMFileRecordStart[_dwMMTemp2].FileName.bExtID;
        }
        break;
    default:
        break;
    }	
}

//#######################################################################
//  Function    :   MM_ParseDirFiles
//  Description :   This function will parse the files in dir by bFileType. For example, can use bFileType
//                  to select MP3 only. The file record will put in __pMMFileRecord.
//  Parameters  :   bDirID: The dir to parsee files. bFileType: The variable to record supported files.
//              :   wFileIndex: The specific file index to parse in dir.
//  Return      :   TRUE: Mean file parsed OK. FALSE: Mean no files or parse failed.
//  Side Effect :
//#######################################################################
BYTE MM_ParseDirFiles(WORD wDirID, BYTE bFileType, WORD wFileIndex)
{
    // wyc2.53-909P,
    if (bFileType == 0)
        return FALSE;
    //MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, bFileType, __bMMSortMethod);
    MM_PreparePage(MM_PREPARE_FILEPAGE, wDirID, bFileType, __bMMSortMethod);
    if (wFileIndex > __pMMFilterFilesOfDir[wDirID])
        return FALSE;
    if (__pMMFilterFilesOfDir[wDirID] != 0)
    {
        MM_TriggerDumping(MM_ID_RelativeToAbsolute(wDirID, wFileIndex), __pMMFilterFilesOfDir[wDirID]);
        return TRUE;
    }
    else 
        return FALSE;
}

//#######################################################################
//  Function    :   _MM_PrepareInfo
//  Description :   Prepare the all playable files relative information
//                  - the total directory name list : __pMMDirNameList
//                  - the start sector/ length of each file : _pFMFileTOC
//                  - the playing status of all JPG file : _pFMStatusList
//  Parameters  :   None
//  Return      :
//  Changed     :
//  Side Effect :   Will share __bPool[] buffer
//                  [1] _cSector3: 2048 bytes, ISO9660 parsering -- CDROM decoding
//                      Two buffer will share _cSector3[]:
//                      - __pMMDirNameList:
//                        CT9928AF: 150 directory * (6+1+1) char = 1200 bytes,
//                        CT908   : 150 directory * (8+1+1) char = 1500 bytes
//                      - _pFMStatusList:
//                        CT9928AF/ CT908: 200 file * (3 byte element) = 600 bytes
//                      CT9928AF: 200 file * (6+1+1) char = 1600 bytes;
//                      CT908   : 200 file * (8+1+1) char = 2000 bytes;
//                  [3] _pFMFileTOC: 200 file * (3+2) = 1000 bytes
//                  [4] _pMMDirLoc: 150 directory * (3 bytes) = 450 bytes
//                  [5] __pMMVolumeDirTree : 150 directory * (3 bytes) = 450 bytes
//                  So, max buffer size is: 2048 + 2000 + 1000 + 450 + 450 = 5948 bytes
//  Notice      :   _pFMStatusList[] will use temporal buffer while initialize,
//                  And all data will be always kept in DRAM.
//                  Share _cSector3[] buffer in first stage
//  Globals     :   _dwMMTemp5, _dwMMTemp6, _dwMMTemp7, _bMMIdx, _dwMMTemp2
//#######################################################################
BYTE _MM_PrepareInfo(void)
{
    __wPoolOffset = (WORD)((DWORD)__cSector3 - (DWORD)__bPool);
	
    // Increase 64 bytes because will use when parse dir record during across sector boundary.
    __wPoolOffset+=(M2F1_DATASECTOR+64);  // __cSector3 will be used for ISO9660
    // Check if the remain buffer size is enough for __cSector3 (2048 bytes)
    if( (__wPoolOffset + M2F1_DATASECTOR) > LENGTH_SHAREPOOL)
    {
#ifdef  _DEBUG_INFO
#ifdef  SUPPORT_PRINTF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\n BUFFER OVERFLOW[FMANAGER] for __cSector3[] !!!");
#endif  // #ifdef SUPPORT_PRINTF
#endif  // #ifdef _DEBUG_INFO
        return FALSE;
    }
	
    // [2] Assign buffer for _pMMDirLoc, this buffer is used to record the location of every directory
    // to parse the file information.
    _pMMDirLoc = (PDIR_LOC)(__bPool + __wPoolOffset);
    // Assign same buffer memory to __pMMFilesOfDir, __pMMFilesOfDir use same memory as _pMMDirLoc us
    // because we will set __pMMFilesOfDir value after _pMMDirLoc is used and will not use it anymore.
    __pMMFilesOfDir=(WORD *) _pMMDirLoc;
    // Update the __wPoolOffset by MM_MAX_DIR_NO*DIR_LOC_LENGTH bytes
    __wPoolOffset += (MM_MAX_DIR_NO*DIR_LOC_LENGTH);
	
    if ((__wPoolOffset + (MM_MAX_DIR_NO*DIR_LOC_LENGTH)) > LENGTH_SHAREPOOL)
    {
#ifdef  _DEBUG_INFO
#ifdef  SUPPORT_PRINTF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\n BUFFER OVERFLOW for _pMMDirLoc[] & __pMMFilesOfDir[] !!!");
#endif  // #ifdef  SUPPORT_PRINTF
#endif  // #ifdef  _DEBUG_INFO
        return FALSE;
    }
	
    // [3] Assign the buffer of __pMMVolumeDirTree
    __pMMVolumeDirTree = (PTREE_INFO)(__bPool + __wPoolOffset);
	
    // Update the __wPoolOffset by MM_MAX_DIR_NO*TREE_INFO_LENGTH  bytes
    __wPoolOffset += MM_MAX_DIR_NO*TREE_INFO_LENGTH;
	
    // Check if the remain buffer size is enough for __pMMVolumeDirTree
    if( (__wPoolOffset + MM_MAX_DIR_NO*TREE_INFO_LENGTH) > LENGTH_SHAREPOOL)
    {
#ifdef  _DEBUG_INFO
#ifdef  SUPPORT_PRINTF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\n BUFFER OVERFLOW for __pMMVolumeDirTree[] !!!");
#endif  // #ifdef  SUPPORT_PRINTF
#endif  // #ifdef  _DEBUG_INFO
        return FALSE;
    }
	
    // [4] Assign the buffer of __pMMProgContent
    __pMMProgContent = (PFILENAMEINFO)(__bPool + __wPoolOffset);
	
    // Update the __wPoolOffset by MAX_PROG_ITEM*FILENAMEINFO_LENGTH bytes
    __wPoolOffset += MAX_PROG_ITEM*sizeof(FILENAMEINFO);
	
    // check if buffer overflow
    if ((__wPoolOffset + (MAX_PROG_ITEM * sizeof(FILENAMEINFO)) ) > LENGTH_SHAREPOOL)
    {
#ifdef  _DEBUG_INFO
#ifdef  SUPPORT_PRINTF
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\n BUFFER OVERFLOW for __pMMProgContent[] !!!");
#endif  // #ifdef  SUPPORT_PRINTF
#endif  // #ifdef  _DEBUG_INFO
        return FALSE;
    }
    __pMMFilterFilesOfDir = (WORD*)(__bPool + __wPoolOffset);
#ifdef SUPPORT_FAT_FILE_SYSTEM
    // wyc1.05-909, when support FAT, FAT will borrow space of __pMMFilterFilesOfDir for save dir location, dir_loc is 3 bytes. So reserve more for FAT.
    __wPoolOffset += MM_MAX_DIR_NO*3;
#else
    __wPoolOffset += MM_MAX_DIR_NO*2;
#endif //
    if (__wPoolOffset > LENGTH_SHAREPOOL)
        return FALSE;
    // wyc1.00-909, initial the address of pointer
    __pFMComFile = (BYTE*)(__bPool + __wPoolOffset);
    __wPoolOffset += 64; // max file length in each file is 64.
	
#ifdef SUPPORT_FAT_FILE_SYSTEM
    // wyc1.05-909, allocate DRAM size for FAT cache table.
    // wyc1.07-909, to force _pMMFATTable address DWORD alignment to avoid IU error when dumping data into this pointer.
	
    __wPoolOffset += (8 - ((DWORD)__bPool + __wPoolOffset)) & 7;    // 8 bytes alignment
    _pMMFATTable = (DWORD*)((DWORD)__bPool + __wPoolOffset);
	
    // x2 is because one cluster need to use 2 bytes to record in FAT table.
    __wPoolOffset += FAT16_TABLE_CACHE_CLUS_NUM * FAT16_TABLE_OCCUPY_SIZE_PER_CLUS;
	
    if (__wPoolOffset > LENGTH_SHAREPOOL)
        return FALSE;
#endif //SUPPORT_FAT_FILE_SYSTEM
	
    // wyc1.10-909, init the DRAM for DivX SP usage.
#ifdef SUPPORT_CHAR_SUBPICTURE
    // wyc2.76-909P,
    __wPoolOffset += (4 - ((DWORD)__bPool + __wPoolOffset)) & 3;
    __pMMMP4SPRecordStart = (PMP4SP_RECORD)(__bPool + __wPoolOffset);
    __pMMMP4SPRecord = __pMMMP4SPRecordStart;
    __wPoolOffset += (sizeof(MP4SP_RECORD))*MM_MAX_SRT_NUM;
    if (__wPoolOffset > LENGTH_SHAREPOOL)
        return FALSE;
	
	
    __bTotalSRTFiles = 0;
#else
    __pMMMP4SPRecord = 0;
#endif //
	
    // scping, for UDF
    // wyc2.16-909S, add UDF file system supporting.
#ifdef SUPPORT_UDF_FILE_SYSTEM
    __cSectorFE = (char *)(__bPool + __wPoolOffset);
    __wPoolOffset += 200;
    if (__wPoolOffset > LENGTH_SHAREPOOL)
    {
        return FALSE;
    }
#endif //
	
    // wyc2.22-909s, because the audio buffer is same as user data buffer, so change to video buffer.
    INFOFILTER_SetTempBuff(DS_AD0BUF_ST);
    // elmer2.33
    _pMMMP4SPIndex = (PMP4SP_INDEX)_dwIFTempBuffAddr;
	
    // wyc0.62-909, modify code for CT909 mode.
    // Chuan0.85, Use the definition of CTKAV.H
    __pMMDirNameList = (PDIRNAMEINFO)(DS_FW_BUFFER_ST_MM);
    __pMMFileRecord = (PFILE_RECORD)(DS_FW_BUFFER_ST_MM+(MM_MAX_DIR_NO*sizeof(DIRNAMEINFO)));
    __pMMFileRecordStart = (PFILE_RECORD)(__pMMFileRecord);
    // wyc1.07-909, initial it for cdinfo to reference.
    __bMaxExtNO = MAX_EXTENSION_NO;
	
    // Step 2: Assign the necessary member value of _MMFileLog structure
    // Because, we want to call CD_GetTotalEntriesbyPath(& _MMFileLog);
    // And, get all directory relative information
	
    // LLY2.80, Specify the max string length & gap while parsing directory
    // LLY2.81-2, re-define the spec. of bMaxStringLen : don't include '\0'
    _MMFileLog. bMaxStringLen= MM_MAX_DIRNAME_LENGTH; // don't include "\0"
    _MMFileLog. bGapbyString= sizeof(DIRNAMEINFO);
    _MMFileLog. wMaxNO= MM_MAX_DIR_NO;
    _MMFileLog. wStartNO= 0;  // Who use it ??? It's unnecessary now ??
    _MMFileLog. wAttrFilter= 0xFFFF;
    // The attr of directory can temp use __pMMFileRecord to access because of __pMMFileRecord will not use now.
    _MMFileLog. pAttr= (WORD *)__pMMFileRecord;
    _MMFileLog. pTree= (BYTE *)__pMMVolumeDirTree;
	
	
    _MMFileLog. pName= (char* )__pMMDirNameList;
    _MMFileLog. ptrVoid= (void*) _pMMDirLoc;  // give a buffer point used to save directory location
    // wyc2.37-909s, replace to NO_DISC_MODE define
#ifndef NO_DISC_MODE //++CoCo2.37p
    if ((__bFileSystem == FILE_SYSTEM_ISO9660) || (__bFileSystem == FILE_SYSTEM_ISOJOLIET))
    {
		
        // Step 3: Call CD_GetTotalEntriesbyPath() to get all directory by path table
        // Notice: all necessary member of _MMFileLog must be ready !!
        CD_GetTotalEntriesbyPath(&_MMFileLog);
		
        // Step 4: Keep the final directory number after parsing ok !!
        __bMMTotalDirs= (BYTE) _MMFileLog.wActualNO;
		
        // Step 5: Find the root directory, and give the name as "ROOT"
        //           Because there is no name for root directory
        for(_bMMIdx=0; _bMMIdx<__bMMTotalDirs; _bMMIdx++)
        {
            if(_MMFileLog.pAttr[_bMMIdx] & FILE_FLAG_ROOT)
            {
                strncpy(__pMMDirNameList[_bMMIdx].cName, "ROOT", 4);
                __pMMDirNameList[_bMMIdx].cName[4]=0; // end of string
                break;
            }
        }
		
        // Step 7: Initialize total file number first
        // And, we will start to get total files of each directory
        __wMMTotalFiles=0;
        _MMFileLog.wTotalNO = __wMMTotalFiles;
		
        // Step 8: Specify the necessary element of _MMFileLog structure
        // Before calling CD_GetTotalEntriesbyDir()
        // (1) specify to search FILE only within each directory
        _MMFileLog. wAttrFilter= FILE_FLAG_FILE;
        // (2) give extension name order -- LLY.274p-2
				
        // (3) Specify the max string length & gap for parsing file
        // LLY2.81-2, re-define the spec. of bMaxStringLen : don't include '\0'
        _MMFileLog. bMaxStringLen= MM_MAX_FILENAME_LENGTH;  // don't include "\0"
        // wyc1.25, use FILE_RECORD_LENGTH to replace FILENAMEINFO_LENGTH
        _MMFileLog. bGapbyString= sizeof(FILE_RECORD);
		
        // elmer
        __bMP4SPFileCnt = 0;
        __bAVIFileCnt = 0;
		
        // Get all files within all directory
        // Notice: can't exceed 200 files !!
        for(_bMMIdx=0; _bMMIdx< __bMMTotalDirs; _bMMIdx++)
        {
            // elmer2.33
            _pMMMP4SPIndex[_bMMIdx].wMMSPRecordEntry = 0xFFFF;
            _pMMMP4SPIndex[_bMMIdx].wMMSPRecordEnd = 0xFFFF;
			
            _dwMMTemp9 = __bMP4SPFileCnt;
            // LLY0.62, using general abort mainflow API instead of checking POWER/ OPEN_CLOSE key
            if( UTL_QueryCapability(QUERY_ABORT_MAINFLOW))
                return FALSE;
						
            // Step 9: Specify directory location
            // wyc1.07-909, update to 4 bytes for FAT32 need 4 bytes to record information.
            _dwMMTemp8=((DWORD)_pMMDirLoc[_bMMIdx].bSector[0]) << 24;
            _dwMMTemp8|=((WORD)_pMMDirLoc[_bMMIdx].bSector[1]) << 16;
            _dwMMTemp8|=((WORD)_pMMDirLoc[_bMMIdx].bSector[2]) << 8;
            _dwMMTemp8|=_pMMDirLoc[_bMMIdx].bSector[3];
            CD_SetDirectoryLocation(_dwMMTemp8);
            // wyc0.87, record the current parsing dir for CDINFO to add correct index dir files attribute. Also need to parse the pointer
            // address for CDINFO.
            _MMFileLog.bDirIndex = _bMMIdx;
            // compute the end sectors of parsing file in directory. Protect to avoid over parsing another directory's files.
            _dwMMTemp8=((DWORD)_pMMDirLoc[_bMMIdx+1].bSector[0]) << 24;
            _dwMMTemp8=((DWORD)_pMMDirLoc[_bMMIdx+1].bSector[1]) << 16;
            _dwMMTemp8|=((WORD)_pMMDirLoc[_bMMIdx+1].bSector[2]) << 8;
            _dwMMTemp8|=_pMMDirLoc[_bMMIdx+1].bSector[3];
            // wyc2.36-909S, when parse to last dir, can't use next dir to check the dir sector ID because no next dir now.
            if (((_bMMIdx+1) == __bMMTotalDirs) || ((_dwMMTemp8 - __dwSectorDir) > MM_MAX_PARSE_DIR_SECTORS))
                _MMFileLog.bDirSectorNum = MM_MAX_PARSE_DIR_SECTORS;
            else
                _MMFileLog.bDirSectorNum = _dwMMTemp8 - __dwSectorDir;
			
            // Step 10: Continue to specify the member of _MMFileLog, before call CD_GetTotalEntriesbyDir()
            _MMFileLog.wMaxNO=(MM_MAX_FILE_NO_ONE_TITLE-__wMMTotalFiles);
			
            // LLY2.80, always use full buffer if F/W will save file info. into DRAM
            _MMFileLog.pFileRecord=(PFILE_RECORD) __pMMFileRecord;
			
            //can't record the attribute, or cause MP3 name/song length wrong???
            _MMFileLog.pAttr=NULL;    // don't record Attribute
			
            // Step 11: Get necessary files information within the desired directory
            // max doing 30 times to parse the data in directory when dir files more than our SRAM boundary,
            __pMMFilesOfDir[_bMMIdx] = 0;
			
#ifdef SUPPORT_CHAR_SUBPICTURE
            _MMFileLog.pMP4SPRecord = (PMP4SP_RECORD)__pMMMP4SPRecord;
#endif
			
            for (_bMMDirParseIdx = 0; _bMMDirParseIdx < 30; _bMMDirParseIdx++)
            {
                // wyc1.10-909, init the pointer for CDINFO to use.
                _MMFileLog.wStartNO=_bMMDirParseIdx;
                _dwMMTemp8 = sizeof(FILE_RECORD);
                // wyc1.07, when CDINFO return FALSE to represent can't getting any data from disc, then we should break the for loop to enter next dir parsing.
                if (!CD_GetTotalEntriesbyDir( & _MMFileLog ))
                {
                    // wyc2.60-909P, modify the mechod for link mechanism
                    __pMMDirNameList[_bMMIdx].bMMLinkStatus &= ~MM_SORT_FILTER_LINK_READY;
                    break;
                }
				
                // Step 12: Update total files within the directory
                __pMMFilesOfDir[_bMMIdx] += _MMFileLog.wActualNO;
				
                // Step 13: Convert the unknown file name into MP3XXX/ JPGXXX
#ifdef  CONVERT_UNKNOWN_NAME
                for(_dwMMTemp2=0; _dwMMTemp2<_MMFileLog.wActualNO; _dwMMTemp2++)
                {
                    // Specify the desired buffer address that stored directory name first
                    // Because, _MM_ConvertName() will use "_pMMTempNameList" for reference
                    _pMMTempNameList=&__pMMFileRecord[_dwMMTemp2].FileName;
                    __bMMUniFile = __pMMFileRecord[_dwMMTemp2].bFileAttr & IF_UNICODE_FILE_TAG;
                    _MM_ConvertName(MM_CONVERT_FILENAME, (WORD)(_dwMMTemp2+1));
                }
#endif  // #ifdef CONVERT_UNKNOWN_NAME
				
                // Step 14: Update and save information if exist files within current directory
                if( _MMFileLog.wActualNO )
                {
                    // LLY2.80, Save file relative information within this directory into DRAM
                    // Save file name information
                    __pMMFileRecord += _MMFileLog.wActualNO;
                    // wyc0.50-909, record the entry file and last file that in this directory.
                    __pMMDirNameList[_bMMIdx].wMMFileRecordEntry = __wMMTotalFiles;
                    __wMMTotalFiles+= _MMFileLog.wActualNO;
                    _MMFileLog.wTotalNO = __wMMTotalFiles;
                    __pMMDirNameList[_bMMIdx].wMMFileRecordEnd = __wMMTotalFiles - 1;
					
                    // elmer
                    if(__bMP4SPFileCnt - _dwMMTemp9 > 0)
                    {
                        _pMMMP4SPIndex[_bMMIdx].wMMSPRecordEntry = _dwMMTemp9;
                        //__pMMDirNameList[_bMMIdx].wMMSPRecordEnd = __bTotalSRTFiles - 1;
                        _pMMMP4SPIndex[_bMMIdx].wMMSPRecordEnd = __bMP4SPFileCnt - 1;
                    }
                    if (__wMMTotalFiles >= MM_MAX_FILE_NO_ONE_TITLE)
                        break;
					
                    __pMMDirNameList[_bMMIdx].bMMLinkStatus &= ~MM_SORT_FILTER_LINK_READY;
					
                }
                if (_MMFileLog.wStartNO == 0xFFEE)
                    break;
            }  // for(_bMMIdx=0 ~ __bMMTotalDirs)
        }
    }
#ifdef SUPPORT_UDF_FILE_SYSTEM
    else if (__bFileSystem == FILE_SYSTEM_UDF)
    {
        _MMUDFDirLog.wMaxDirNO = MM_MAX_DIR_NO;
        _MMUDFDirLog.bMaxStringLen = MM_MAX_DIRNAME_LENGTH;
        _MMUDFDirLog.pDirNameList = __pMMDirNameList;
        _MMUDFDirLog.pFilesOfDir = __pMMFilesOfDir;
        _MMUDFDirLog.pVolumeDirTree =(PTREE_INFO)__pMMVolumeDirTree;
        _MMUDFDirLog.pDirLoc = (PDIR_LOC)__pMMFilterFilesOfDir;
		
        //_MMFileLog.wAttrFilter= FILE_FLAG_FILE;
        _MMFileLog.bMaxStringLen= MM_MAX_FILENAME_LENGTH;
        _MMFileLog.bGapbyString= sizeof(FILE_RECORD);
        _MMFileLog.wMaxNO = MM_MAX_FILE_NO_ONE_TITLE;
        _MMFileLog.pFileRecord=(PFILE_RECORD) __pMMFileRecord;
		
		
        UDF_BuildFileSystem(&_MMUDFDirLog, &_MMFileLog);
		
        __bMMTotalDirs = _MMUDFDirLog.bActualDirNO;
        __wMMTotalFiles = _MMFileLog.wActualNO;
        // wyc2.31-909S, add UDF in convert unknown code.
#ifdef  CONVERT_UNKNOWN_NAME
        if (__bMMTotalDirs != 0)
        {
            for(_dwMMTemp2=0; _dwMMTemp2<__wMMTotalFiles; _dwMMTemp2++)
            {
                _pMMTempNameList=(PFILENAMEINFO)(&__pMMFileRecord[_dwMMTemp2].FileName);
                __bMMUniFile = __pMMFileRecord[_dwMMTemp2].bFileAttr & IF_UNICODE_FILE_TAG;
                _MM_ConvertName(MM_CONVERT_FILENAME, (WORD)(_dwMMTemp2+1));
            }
        }
#endif  // #ifdef CONVERT_UNKNOWN_NAME
	}
#endif //SUPPORT_UDF_FILE_SYSTEM
    // wyc1.01-909, file system will be detected after function INFOFILTER_GetFileSystem. So can parse the file structure here.
    // wyc1.07-909, FAT32 file system also need to enter follow code to parse files.
    else if ((__bFileSystem == FILE_SYSTEM_FAT12) || (__bFileSystem == FILE_SYSTEM_FAT16) || (__bFileSystem == FILE_SYSTEM_FAT32))
    {
#else
		{
#endif //#ifndef NO_DISC_MODE //++CoCo2.37p
#ifdef SUPPORT_FAT_FILE_SYSTEM
			
			if (!FAT_AssignFATTableMem((DWORD*)_pMMFATTable))
				return FALSE;
			// use this structure to record the pointer address of dir relative record and pass to FAT function to build value.
			_MMFATDirLog.wMaxDirNO = MM_MAX_DIR_NO;
			_MMFATDirLog.bMaxStringLen = MM_MAX_DIRNAME_LENGTH;
			_MMFATDirLog.pDirNameList = __pMMDirNameList;
			_MMFATDirLog.pFilesOfDir = __pMMFilesOfDir;
			_MMFATDirLog.pVolumeDirTree = (PTREE_INFO)__pMMVolumeDirTree;
			//  because __pMMFilesOfDir and _pMMDirLoc use same DRAM space, so let pDirLoc to use space of __pMMFilterFilesOfDir temply.
			// MM_Initial will re-assign __pMMFilesOfDir to __pMMFilterFilesOfDir, so it is OK to borrow its space here.
			_MMFATDirLog.pDirLoc = (PDIR_LOC)__pMMFilterFilesOfDir;//(PDIR_LOC)_pMMDirLoc;
			
			_MMFileLog.wAttrFilter= FILE_FLAG_FILE;
			_MMFileLog.bMaxStringLen= MM_MAX_FILENAME_LENGTH;
			_MMFileLog.bGapbyString= sizeof(FILE_RECORD);
			_MMFileLog.wMaxNO = MM_MAX_FILE_NO_ONE_TITLE;
			_MMFileLog.pFileRecord=(PFILE_RECORD) __pMMFileRecord;
			_MMFileLog.pMP4SPRecord = (PMP4SP_RECORD)__pMMMP4SPRecord;
			__wMMTotalFiles = 0;
			
			FAT_BuildFileSystem(__SF_SourceGBL[__dwSFStreamID].bSourceIndex, &_MMFATDirLog, &_MMFileLog, (PMP4SP_INDEX)_pMMMP4SPIndex);
			if (_MMFATDirLog.bActualDirNO != 0)
				__bMMTotalDirs = _MMFATDirLog.bActualDirNO;
			else
				return FALSE;
			if (_MMFileLog.wActualNO != 0)
			{
				__wMMTotalFiles = _MMFileLog.wActualNO;
#ifdef  CONVERT_UNKNOWN_NAME
				for(_dwMMTemp2=0; _dwMMTemp2<__wMMTotalFiles; _dwMMTemp2++)
				{
					_pMMTempNameList=(PFILENAMEINFO)(&__pMMFileRecord[_dwMMTemp2].FileName);
					__bMMUniFile = __pMMFileRecord[_dwMMTemp2].bFileAttr & IF_UNICODE_FILE_TAG;
					_MM_ConvertName(MM_CONVERT_FILENAME, (WORD)(_dwMMTemp2+1));
				}
#endif  // #ifdef CONVERT_UNKNOWN_NAME
			}
			else
				return FALSE;
#endif //SUPPORT_FAT_FILE_SYSTEM
		}
		
#ifdef SUPPORT_CHAR_SUBPICTURE
		//elmer
		//_MMFileLog.pFileRecord = (PFILE_RECORD)__pMMFileRecordStart;
		_MM_GetMP4SP(&_MMFileLog);
		__pMMMP4SPRecord = __pMMMP4SPRecordStart;
#endif //
		
		// Step 17: Scan & remove the empty directory
#ifdef  DISCARD_EMPTY_DIRECTORY
		while(_MM_ExistEmptyFolder())
		{
			_dwMMTemp6=__bMMTotalDirs; // used to keep real directory number
			_dwMMTemp5=FALSE; // used to keep "find next un-zero action" flag
			for(_bMMIdx=0; _bMMIdx<__bMMTotalDirs; _bMMIdx++)
			{
				if((__pMMFilesOfDir[_bMMIdx])  ||(__pMMVolumeDirTree[_bMMIdx].bNoOfSubDir))
				{
					// If enable "find next un-zero flag" is on & current directory's file is non-zero
					// ie. we find the next un-zero directory
					// There are empty directory within them, so can remove them
					if(_dwMMTemp5)
					{
						// LLY.274p-3, use "MM_MAX_DIRNAME_LENGTH" for directory name
						// Otherwise, the 7th char of directory name will be cut
						strncpy(__pMMDirNameList[_dwMMTemp7].cName, __pMMDirNameList[_bMMIdx].cName, MM_MAX_DIRNAME_LENGTH);
						__pMMDirNameList[_dwMMTemp7].cName[MM_MAX_DIRNAME_LENGTH]=NULL;
						__pMMFilesOfDir[_dwMMTemp7] = __pMMFilesOfDir[_bMMIdx];
						//     __pMMFilesOfDir[_bMMIdx] = 0;
						__pMMVolumeDirTree[_dwMMTemp7].b1stSubDirID=__pMMVolumeDirTree[_bMMIdx].b1stSubDirID;
						__pMMVolumeDirTree[_dwMMTemp7].bNoOfSubDir=__pMMVolumeDirTree[_bMMIdx].bNoOfSubDir;
						__pMMVolumeDirTree[_dwMMTemp7].bParentID=__pMMVolumeDirTree[_bMMIdx].bParentID;
						if(__pMMVolumeDirTree[_dwMMTemp7].bNoOfSubDir!=0)
						{
							for(_dwMMTemp4=0;_dwMMTemp4<__pMMVolumeDirTree[_dwMMTemp7].bNoOfSubDir;_dwMMTemp4++)
								__pMMVolumeDirTree[__pMMVolumeDirTree[_dwMMTemp7].b1stSubDirID+_dwMMTemp4].bParentID=_dwMMTemp7;
						}
						if(__pMMVolumeDirTree[__pMMVolumeDirTree[_dwMMTemp7].bParentID].b1stSubDirID==_bMMIdx)
							__pMMVolumeDirTree[__pMMVolumeDirTree[_dwMMTemp7].bParentID].b1stSubDirID=_dwMMTemp7;
						// Increase next ready index for copy next non-empty entry
						_dwMMTemp7++;
					}
					continue;
				}
				else
				{
					// If total files of current directory is zero & "finding next un-zero flag" is not on
					// ie. we meet the 1st empty directory
					// Enable this flag, and keep the ready index for copy next non-zero entry
					if(!_dwMMTemp5)
					{
						_dwMMTemp5=TRUE;
						_dwMMTemp7=_bMMIdx;
					}
					_dwMMTemp6--;
					// Modify its parent ID info.
					if( __pMMVolumeDirTree[__pMMVolumeDirTree[_bMMIdx].bParentID].bNoOfSubDir==1)
					{
						__pMMVolumeDirTree[__pMMVolumeDirTree[_bMMIdx].bParentID].b1stSubDirID= 0xFF;
					}
					else if( __pMMVolumeDirTree[__pMMVolumeDirTree[_bMMIdx].bParentID].b1stSubDirID==_bMMIdx) //xuli0609 add
					{
						__pMMVolumeDirTree[__pMMVolumeDirTree[_bMMIdx].bParentID].b1stSubDirID++;
					}
					__pMMVolumeDirTree[__pMMVolumeDirTree[_bMMIdx].bParentID].bNoOfSubDir--;
				}
			}
			__bMMTotalDirs=_dwMMTemp6;
		}
		
#endif  // #ifdef DISCARD_EMPTY_DIRECTORY
		
		// Step 18: Convert the unknown directory name into DIRXXX
#ifdef  CONVERT_UNKNOWN_NAME
		// LLY0.84, check if exist "ROOT" directory first
		// if yes, do the convert procedure from index '1'
		// if no, do the convert procedure from index '0'
		// Notice: __pMMDirNameList[] index from '0'
		// Otherwise, 1st unknown sub-directory name will be convert as "DIR002"
		if( ! strcmp(__pMMDirNameList[0].cName, "ROOT") ) // exist "ROOT" directory
			_dwMMTemp5=1;
		else
			_dwMMTemp5=0;
		
		for(_bMMIdx=_dwMMTemp5; _bMMIdx<__bMMTotalDirs; _bMMIdx++)
		{
			// Specify the desired buffer address that stored directory name first
			// Because, _MM_ConvertName() will use "__bFMNameList" for reference
			_pMMTempNameList=(PFILENAMEINFO)&__pMMDirNameList[_bMMIdx];
			__bMMUniFile = __pMMDirNameList[_bMMIdx].bMMLinkStatus & IF_UNICODE_FILE_TAG;
			
			// Specify the desired index while final convert name
			// if exist "ROOT", 1st sub-directory (index 1) is "DIR001"
			// if un-exist "ROOT", 1st directory (index 0) is "DIR001"
			_MM_ConvertName(MM_CONVERT_DIRNAME, (WORD)(_bMMIdx+1-_dwMMTemp5));
		}
#endif  // #ifdef CONVERT_UNKNOWN_NAME
		// wyc2.21-909S, update code to share FW buffer to SP when don't use so memory.
		// wyc2.35-909S, need to use DS_CHAR_BASED_SP_OUTBUF_ST.
		// LLY2.36, keep current F/W buffer used address end
		// Since, remove SP buffer start address and size setting to UTL.
		if ((DS_FW_BUFFER_ST_MM + __wMMTotalFiles*sizeof(FILE_RECORD) + __bMMTotalDirs*sizeof(DIRNAMEINFO)) >= DS_FW_BUFFER_END_MM)
		{
			//__dwMMSPBuffST = DS_CHAR_BASED_SP_OUTBUF_ST;
			__dwMMFWBuffEnd = DS_CHAR_BASED_SP_OUTBUF_ST;
		}
		else
		{
			// wyc2.32-909S, update to using MM_MAX_DIR_NO to fix SP output data putting wrong position problem.
			__dwMMFWBuffEnd = DS_FW_BUFFER_ST_MM + __wMMTotalFiles*sizeof(FILE_RECORD) + MM_MAX_DIR_NO*sizeof(DIRNAMEINFO);
		}
				
		// Step 19: Update the begin/ end track information
		__wTrackBegin=1;
		__wTrackEnd=__wMMTotalFiles;
		// wyc1.05, initial to 1 because 0 is not valid play item.
		// wyc1.05, need to initial active cursor to position -> 0 and index to 0xffff to avoid OSDFM to draw active color in file name.
		__MMPlayingItem .bPosition = 0;
		__MMPlayingItem .wIndex = 0xFFFF;
		_bMMIdx = 0;
		
		if (!__wMMTotalFiles)
			return FALSE;
		
#ifdef  DEBUG_MESSAGE
		DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,"\nTotal %d directories, %d Files", __bMMTotalDirs, __wMMTotalFiles);
#endif  //#ifdef  DEBUG_MESSAGE
		
		// LLY.274p-1, must check more ??
		__wDiscType |= CDROM_M1;
		// wyc1.10-DVDR, set disc type to BOOK_M1 when DVD-ROM title for easily to porting the code.
		if (__bDVDROM)
			__wDiscType = BOOK_M1;
		
		return TRUE;
}

//  ************************************************************************
//  Function    :   MM_AccessJPGStatus
//  Description :   Get/ store the desired JPG status from/ into DRAM
//  Arguments   :   bMode, the get/ store operation
//                  bIdx, the desired item, index from '1'
//  Return      :   None
//  Side Effect :   __bMMJPGStatus[] used to store the desired JPEG file's status
//  Global      :   _dwMMTemp2
//  ************************************************************************
void MM_AccessJPGStatus(BYTE bMode, WORD wIdx)
{
    BYTE bTemp;
    // decrease wIndex by 1, because the array index is from '0'
    wIdx--;
	
    // LLY0.83b-2, calculate the corresponding index by "disc view"
    // Because, disc JPG status is stored into DRAM together in "disc view"
#ifdef SUPPORT_ENCODE_JPG_PICTURE
    if(__SF_SourceGBL[0].bSourceIndex == SOURCE_SPI)
    {
        wIdx = MM_EncodeFile_GetFileID(wIdx);
        switch(bMode)
        {
        case    MM_GET_JPG_STATUS:
            __bMMJPGStatus = __bMMJPGEncodeJPGStatus[wIdx];
            break;
			
        case MM_STORE_JPG_STATUS:
            __bMMJPGEncodeJPGStatus[wIdx] = __bMMJPGStatus;
            HAL_WriteStorage(SETUP_ADDR_JPG_ENCODE_FILE_STATUS, __bMMJPGEncodeJPGStatus, SRCFTR_SUPPORT_JPG_ENCODE_NUM);
            break;
        }
    }
    else
#endif //
    {
        wIdx=MM_ID_RelativeToAbsolute(__MMPlayingItem.wDirID, wIdx);
        // wyc1.25, only file name access can use __pMMFileRecord, but why access bFileAttr also use __pMMFileRecord in follow code.
        // This is because only JPG will access the bFileAttr, before to display files, F/W will call MM_PreparePage to get file lists.
        // So __pMMFileRecord will get correct file lists and that's why the bFileAttr access can use __pMMFileRecord to record.
        // Chuan0.85, Use the definition of CTKAV.H
        __pMMFileRecord = __pMMFileRecordStart;
        __pMMFileRecord += wIdx;
        switch(bMode)
        {
            // wyc2.60-909P, update the code for JPG rotate status because use attr for unicode checking.
        case    MM_GET_JPG_STATUS:
            __bMMJPGStatus = (__pMMFileRecord->bFileAttr & ~IF_UNICODE_FILE_TAG);
            break;
			
        case MM_STORE_JPG_STATUS:
            bTemp = __pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG;
            __pMMFileRecord->bFileAttr = __bMMJPGStatus;
            __pMMFileRecord->bFileAttr |= bTemp;
            break;
        }
    }	
}

//  ************************************************************************
//  Function    :   MM_GetSRTRecord
//  Description :   Get the SRC file by input dir/file index.
//  Arguments   :
//
//  Return      :   TRUE: There are SRT in input file, FALSE: There are no SRC in input file.
//  Side Effect :
//  ************************************************************************
#ifdef SUPPORT_CHAR_SUBPICTURE
BYTE MM_GetSRTRecord(PDWORD pdwSecLBA, PDWORD pdwSecNO, PBYTE bMP4SPType)
{
    WORD   dwMMTemp;
    // wyc1.10-909, need to decrease __pMMVolumeDirTree[__MMSelectCursor.wDirID].bNoOfSubDir when compute the index of select cursor to get correct position.
    // wyc2.17-909S, original way use select cursor index but it won't update when next/prev and number key. So now use __wPlayItem to get correct sub-title file.
    dwMMTemp = MM_ID_RelativeToAbsolute(__MMSelectCursor.wDirID, __wPlayItem - 1);
    // wyc1.11a, update code to support new mechanism parsing code.
    if (((__pMMFileRecordStart[dwMMTemp].bFileAttr & ~IF_UNICODE_FILE_TAG) != 0x7F))// && (__SF_SourceGBL[__dwSFStreamID].bSourceIndex == SOURCE_DISC))
    {
    	*pdwSecLBA = __pMMMP4SPRecord[__pMMFileRecordStart[dwMMTemp].bFileAttr & ~IF_UNICODE_FILE_TAG].dwBeginTime;
    	*pdwSecNO =  __pMMMP4SPRecord[__pMMFileRecordStart[dwMMTemp].bFileAttr & ~IF_UNICODE_FILE_TAG].dwLen;
    	*bMP4SPType = __pMMMP4SPRecord[__pMMFileRecordStart[dwMMTemp].bFileAttr & ~IF_UNICODE_FILE_TAG].MP4SPName.bExtID;
	return TRUE;
    }
    return FALSE;
}
#endif //SUPPORT_CHAR_SUBPICTURE


#ifdef SUPPORT_FILE_HANDLE
// LLY1.20 create
//###########################################################################
//  Function    :   MM_GetFileRange
//  Description :   This function is used to get file range in sector unit
//  Parameter   :   bID, specify the index within whole file structure
//  Return      :   The file range (start + length)
//  Side Effect :
//###########################################################################
FILE_RANGE MM_GetFileRange(WORD wID)
{
    FILE_RANGE  Range;

    // Get file start sector
    Range.dwStart = 0;
    Range.dwStart |= __pMMFileRecordStart[wID].FileRange.cS[0];
    Range.dwStart <<= 24;
    Range.dwStart |= ((WORD)__pMMFileRecordStart[wID].FileRange.cS[1])<<16;
    Range.dwStart |= ((WORD)__pMMFileRecordStart[wID].FileRange.cS[2])<<8;
    Range.dwStart |= __pMMFileRecordStart[wID].FileRange.cS[3];

    // Get file lenght in sector unit
    Range.dwLen = __pMMFileRecordStart[wID].FileRange.dwLen;

    return (Range);
}


// wyc1.20-909-searchfile,
//  ************************************************************************
//  Function    :   MM_SearchFileByFileRecord
//  Description :   Find the request file by input path. Then report the offset
//                  in file record in this request file.
//  Arguments   :   pFileName: Search file name. wFileOffset: The offset in file record in this file.
//  Return      :   TRUE: File found. FALSE: File not found.
//  Side Effect :
//  ************************************************************************
BYTE    MM_SearchFileByFileRecord(char* pFilePathName, WORD* wFileOffset)
{
    char   pName[SUPPORT_DIR_LENGTH];
    // step1, to get the depth of dir.
    // use _dwMMTemp2 to represnt the dir depth in input file path.
    _dwMMTemp2= 0;
    // begin from 2 is because ROOT use "//" to represent, so we can skip ROOT string and begin from dir path.
    for (_dwMMTemp1=2; _dwMMTemp1<FILE_HANDLE_DEPTH_OF_FILE_PATH; _dwMMTemp1++)
    {
        if (pFilePathName[_dwMMTemp1] == '/')
            _dwMMTemp2++;
    }
	
    _dwMMTemp3 = 2;
    // Using to indicate the ID of match dir name in file record for searching correct dir path.
    _dwMMTemp5 = 0;
    while (_dwMMTemp2--)
    {
        // search the char '/' to find the dir name.
        for (_dwMMTemp1=_dwMMTemp3; _dwMMTemp1<FILE_HANDLE_DEPTH_OF_FILE_PATH; _dwMMTemp1++)
        {
            if (pFilePathName[_dwMMTemp1] == '/')
            {
                // _dwMMTemp4 is the length of current parsed dir name.
                _dwMMTemp4 = _dwMMTemp1 - _dwMMTemp3;
                // copy the dir name in path to temp buffer for compare usage.
                strncpy(pName,pFilePathName+_dwMMTemp3,_dwMMTemp4);
                pName[_dwMMTemp4] = 0;
                // _dwMMTemp3 mean the next search dir name in input path.
                _dwMMTemp3 = _dwMMTemp1+1;
                break;
            }
        }
        // start to compare the dir name in path & dir name in dirnamelist.
        _dwMMTemp6 = (__pMMVolumeDirTree[_dwMMTemp5].b1stSubDirID+__pMMVolumeDirTree[_dwMMTemp5].bNoOfSubDir);
        for (_dwMMTemp1=__pMMVolumeDirTree[_dwMMTemp5].b1stSubDirID; _dwMMTemp1<_dwMMTemp6; _dwMMTemp1++)
        {
            // the dir name in input path matched with the dir name file record.
            if (!strcmp(__pMMDirNameList[_dwMMTemp1].cName,pName))
            {
                _dwMMTemp5 = _dwMMTemp1;
                break;
            }
        }
        // mean input dir path don't patch the dir in file record.
        if (_dwMMTemp1 >= _dwMMTemp6)
            return FALSE;
		
    }
		
    // need to check the file name in input path and copy file name in input path to pName.
    for (_dwMMTemp1=_dwMMTemp3; _dwMMTemp1<FILE_HANDLE_DEPTH_OF_FILE_PATH; _dwMMTemp1++)
    {
        if (pFilePathName[_dwMMTemp1] == '.')
        {
            // _dwMMTemp2 used to record the end of file name.
            _dwMMTemp2 = _dwMMTemp1;
        }
        else if (pFilePathName[_dwMMTemp1] == 0)
            break;
    }
	
    // (_dwMMTemp2-_dwMMTemp3) is the length of file name in input path.
    strncpy(pName,pFilePathName+_dwMMTemp3,_dwMMTemp2-_dwMMTemp3);
    COMUTL_StrUpr((char*)(pFilePathName+_dwMMTemp2+1));
    // _dwMMTemp4 used to indicate the ext ID of input path file.
    _dwMMTemp4 = CD_CheckExtName(pFilePathName+_dwMMTemp2+1);
    MM_TriggerDumping(MM_ID_RelativeToAbsolute(_dwMMTemp5, 0), __pMMFilesOfDir[_dwMMTemp5]);
    for (_dwMMTemp1=0; _dwMMTemp1<__pMMFilesOfDir[_dwMMTemp5]; _dwMMTemp1++)
    {
        if (MM_GetDumpedFileRecord() == MM_FILE_RECORD_DUMP_FAILED)
            return FALSE;
        if ((!strncmp(__pMMFileRecord->FileName.cName, pName, _dwMMTemp2-_dwMMTemp3)) && (_dwMMTemp4 == __pMMFileRecord->FileName.bExtID))
        {
            // file in file record match as input path file name. Ext name also matched.
            *wFileOffset = MM_ID_RelativeToAbsolute(_dwMMTemp5,_dwMMTemp1);
            return TRUE;
        }
    }
    return FALSE;
}
#endif // SUPPORT_FILE_HANDLE

//  *********************************************************************
//  Function    :   MM_ID_AbsoluteToRelative
//  Description :   convert absolute ID to relative ID of file record.
//  Arguments   :   wAbsID: Absolute ID
//  Return      :   Return the relative ID
//  Remark      :   This function only work in directory boundary, can't across boundary.
//  Side Effect :
//  *********************************************************************
WORD MM_ID_AbsoluteToRelative(DWORD bAbsID)
{
    __pMMFileRecordTemp = __pMMFileRecordStart;
    _dwMMTemp3 = __pMMDirNameList[__MMPlayingItem.wDirID].wMMFileRecordEntry;
    if (_dwMMTemp3 == bAbsID)
        return (0);
    for (_dwMMTemp6=0; _dwMMTemp6< __pMMFilterFilesOfDir[__MMPlayingItem.wDirID]; _dwMMTemp6++)
    {
        _dwMMTemp3 = __pMMFileRecordTemp[_dwMMTemp3].wNextFileRecord;
        if (_dwMMTemp3 == bAbsID)
            break;
    }
    _dwMMTemp6++;
    return (_dwMMTemp6);
}

//  *********************************************************************
//  Function    :   MM_ID_RelativeToAbsolute
//  Description :   convert relative ID to absolute ID of file record.
//  Arguments   :   bDirID: Directory index. wRelID: Relative ID in this directory.
//  Return      :   Return the absolute ID
//  Side Effect :
//  *********************************************************************
WORD MM_ID_RelativeToAbsolute(BYTE bDirID, WORD wRelID)
{
    DWORD   dwMMTemp;
    // Chuan0.85, Use the definition of CTKAV.H
    __pMMFileRecord = (PFILE_RECORD)__pMMFileRecordStart;
    if (__bMMSortSequence == MM_SORT_BY_ASCENDING)
    {
        _dwMMLastAbsID = __pMMDirNameList[bDirID].wMMFileRecordEntry;
        for (dwMMTemp = 0; dwMMTemp<wRelID; dwMMTemp++)
            _dwMMLastAbsID = __pMMFileRecord[_dwMMLastAbsID].wNextFileRecord;
    }
    else if (__bMMSortSequence == MM_SORT_BY_DESCENDING)
    {
        _dwMMLastAbsID = __pMMDirNameList[bDirID].wMMFileRecordEnd;
        for (dwMMTemp = 0; dwMMTemp<wRelID; dwMMTemp++)
            _dwMMLastAbsID = __pMMFileRecord[_dwMMLastAbsID].wPrevFileRecord;
    }
    return (WORD)_dwMMLastAbsID;
}

//  *********************************************************************
//  Function    :   MM_TriggerDumping
//  Description :   Function to trigger file record dumping by absolute index.
//  Arguments   :   wAbsID: Dump file record from input absolute ID. wNum: The expected number of file record dumping.
//  Return      :   None
//  Side Effect :
//  *********************************************************************
void MM_TriggerDumping(WORD wAbsID, WORD wNum)
{
    _wStartDumpAbsID = wAbsID;
    if ((__pMMFileRecord[wAbsID].wPrevFileRecord == FILE_RECORD_NONE) && (__pMMFileRecord[wAbsID].wNextFileRecord == FILE_RECORD_NONE))
        _wMMUnDumpFileRecord = 0;
    else
        _wMMUnDumpFileRecord = wNum;
    _bMMFirstDumped = TRUE;
    _wNextFileRecord = 0;
    return;
}

//  *********************************************************************
//  Function    :   MM_GetDumpedFileRecord
//  Description :   Function to dump file record one by one after issuing function MM_TriggerDumping
//  Arguments   :   pFileRecord: Output dumped file record.
//  Return      :   Return the un-dumped file record number
//  Side Effect :
//  *********************************************************************
WORD MM_GetDumpedFileRecord(void)
{
    if ((_wMMUnDumpFileRecord == 0) || (_wNextFileRecord == FILE_RECORD_END))
        return MM_FILE_RECORD_DUMP_FAILED;
    // wyc0.65, suppose all cases need to do follow code.
    // Chuan0.85, Use the definition of CTKAV.H
    __pMMFileRecord = (PFILE_RECORD)__pMMFileRecordStart;
    if (_bMMFirstDumped)
    {
        __pMMFileRecord = __pMMFileRecord + _wStartDumpAbsID;
        _bMMFirstDumped = FALSE;
    }
    else
    {
        __pMMFileRecord = __pMMFileRecord + _wNextFileRecord;
    }
    _wMMUnDumpFileRecord--;
    if (__bMMSortSequence == MM_SORT_BY_ASCENDING)
        _wNextFileRecord = __pMMFileRecord->wNextFileRecord;
    else
        _wNextFileRecord = __pMMFileRecord->wPrevFileRecord;

    return _wMMUnDumpFileRecord;
}

BYTE _MM_SortFilterFiles(WORD wDirID, BYTE bFilterMode, BYTE bSortMode)
{
    DWORD dwMMMinVal, dwMMMinIndex, dwMMPreMinIndex;
    DWORD dwMMDirOffset, dwMMDiscMinIndex, dwMMDiscPreMinIndex;
    // don't need to sort/filter dir when no files in it.
    if (__pMMFilesOfDir[wDirID] == 0)
    {
        __pMMDirNameList[wDirID].bMMLinkStatus |= MM_SORT_FILTER_LINK_READY;
        __pMMFilterFilesOfDir[wDirID] = 0; //CoCo2.38
        return TRUE;
    }
    if ((__bMMPreExtFilterMode == bFilterMode) && (__bMMPreSortMethod == bSortMode) && (__pMMDirNameList[wDirID].bMMLinkStatus & MM_SORT_FILTER_LINK_READY) && (__wMMPreFilterDir == wDirID))
        return TRUE;
    // Chuan0.85, Use the definition of CTKAV.H
    __pMMFileRecord = (PFILE_RECORD)__pMMFileRecordStart;
	
    _dwMMTemp=(DWORD)MM_IndexConvert(MM_INDEX_CONVERT_DIR2DISC, wDirID, 0);
    // doing sorting function
    if (__bMMSortMethod == MM_SORT_BY_NAME)
    {
        __pMMFileRecord += _dwMMTemp;
        _dwMMTemp3 = _dwMMTemp;
        _dwMMTemp1 = __pMMFilesOfDir[wDirID];
        for (_dwMMTemp=0; _dwMMTemp<_dwMMTemp1; _dwMMTemp++)
        {
            if (_dwMMTemp==0)
            {
                __pMMFileRecord[_dwMMTemp].wNextFileRecord = _dwMMTemp+_dwMMTemp3+1;
                __pMMFileRecord[_dwMMTemp].wPrevFileRecord = FILE_RECORD_BEGIN;
                __pMMDirNameList[wDirID].wMMFileRecordEntry = _dwMMTemp3;
            }
            else if (_dwMMTemp==(_dwMMTemp1-1))
            {
                __pMMFileRecord[_dwMMTemp].wNextFileRecord = FILE_RECORD_END;
                __pMMFileRecord[_dwMMTemp].wPrevFileRecord = _dwMMTemp+_dwMMTemp3-1;
                __pMMDirNameList[wDirID].wMMFileRecordEnd = _dwMMTemp3 + _dwMMTemp ;
            }
            else
            {
                __pMMFileRecord[_dwMMTemp].wNextFileRecord = _dwMMTemp+_dwMMTemp3+1;
                __pMMFileRecord[_dwMMTemp].wPrevFileRecord = _dwMMTemp+_dwMMTemp3-1;
            }
        }
    }
    else if (__bMMSortMethod == MM_SORT_BY_TYPE)
    {
		
        // initial the value of these array
        for (_wSortIndex = 0; _wSortIndex < MAX_EXTENSION_NO; _wSortIndex++)
        {
            _wMMExtFirstID[_wSortIndex] = MM_SORT_INIT_VALUE;
            _wMMExtLastID[_wSortIndex] = MM_SORT_INIT_VALUE;
        }
        // To link the sorting sequence in each file records that are same extension name type.
        for (_wSortIndex=0; _wSortIndex<__pMMFilesOfDir[wDirID]; _wSortIndex++)
        {
            _dwMMTemp1 = _dwMMTemp + (DWORD)_wSortIndex;
            if (_wMMExtFirstID[__pMMFileRecord[_dwMMTemp1].FileName.bExtID] == MM_SORT_INIT_VALUE)
            {
                __pMMFileRecord[_dwMMTemp1].wPrevFileRecord = MM_SORT_CONN_PREV_EXT;
                _wMMExtFirstID[__pMMFileRecord[_dwMMTemp1].FileName.bExtID] = _dwMMTemp1;
                _wMMExtLastID[__pMMFileRecord[_dwMMTemp1].FileName.bExtID] = _dwMMTemp1;
            }
            else
            {
                __pMMFileRecord[_dwMMTemp1].wPrevFileRecord = _wMMExtLastID[__pMMFileRecord[_dwMMTemp1].FileName.bExtID];
                __pMMFileRecord[_wMMExtLastID[__pMMFileRecord[_dwMMTemp1].FileName.bExtID]].wNextFileRecord = _dwMMTemp1;
                _wMMExtLastID[__pMMFileRecord[_dwMMTemp1].FileName.bExtID] = _dwMMTemp1;
            }
        }
        // To link the sequence between each extension file type group
        for (_wSortIndex=0; _wSortIndex<MAX_EXTENSION_NO; _wSortIndex++)
        {
            if (_wMMExtLastID[_wSortIndex] != MM_SORT_INIT_VALUE)
            {
                if (_wSortIndex==0)
                {
                    __pMMDirNameList[wDirID].wMMFileRecordEntry = _wMMExtFirstID[_wSortIndex];
                    __pMMFileRecord[_wMMExtFirstID[_wSortIndex]].wPrevFileRecord = FILE_RECORD_BEGIN;
                }
                else
                {
                    // wyc0.80, enhance the sorting by type to fix when first file is not MP3, then maybe can't parse this kind of ext files.
                    __pMMFileRecord[_wMMExtFirstID[_wSortIndex]].wPrevFileRecord = _MM_GetSortRecord(_wSortIndex-1, MM_SORT_PREV_RECORD);
                    if (__pMMFileRecord[_wMMExtFirstID[_wSortIndex]].wPrevFileRecord == FILE_RECORD_BEGIN)
                        __pMMDirNameList[wDirID].wMMFileRecordEntry = _wMMExtFirstID[_wSortIndex];
                }
                if (_wSortIndex==(MAX_EXTENSION_NO-1))
                {
                    __pMMFileRecord[_wMMExtLastID[_wSortIndex]].wNextFileRecord = FILE_RECORD_END;
                    __pMMDirNameList[wDirID].wMMFileRecordEnd = _wMMExtLastID[_wSortIndex];
                }
                else
                {
                    __pMMFileRecord[_wMMExtLastID[_wSortIndex]].wNextFileRecord = _MM_GetSortRecord(_wSortIndex+1, MM_SORT_NEXT_RECORD);
                    if (__pMMFileRecord[_wMMExtLastID[_wSortIndex]].wNextFileRecord == FILE_RECORD_END)
                        __pMMDirNameList[wDirID].wMMFileRecordEnd = _wMMExtLastID[_wSortIndex];
                }
            }
        }
    }
    else if (__bMMSortMethod == MM_SORT_BY_TIME)
    {
        // need to re-sort the links to default, default is link sequentical one by one.
        __pMMFileRecord += _dwMMTemp;
        dwMMDirOffset = _dwMMTemp;
        _dwMMTemp1 = __pMMFilesOfDir[wDirID];
        // because we will reference wPrevFileRecord to check if this file record is linked by sort/filter, so need to initial first.
        for (_dwMMTemp = 0; _dwMMTemp < _dwMMTemp1; _dwMMTemp++)
        {
            __pMMFileRecord[_dwMMTemp].wNextFileRecord = FILE_RECORD_CLEAN;
            __pMMFileRecord[_dwMMTemp].wPrevFileRecord = FILE_RECORD_CLEAN;
        }
        dwMMMinVal = __pMMFileRecord[0].dwFileTime;
        dwMMMinIndex = 0;
        dwMMPreMinIndex = 0;
        for (_dwMMTemp = 0; _dwMMTemp < _dwMMTemp1; _dwMMTemp++)
        {
            dwMMPreMinIndex = dwMMMinIndex;
            for (_dwMMTemp3 = 0; _dwMMTemp3 < _dwMMTemp1; _dwMMTemp3++)
            {
                if ((__pMMFileRecord[_dwMMTemp3].dwFileTime < dwMMMinVal) && (__pMMFileRecord[_dwMMTemp3].wPrevFileRecord == FILE_RECORD_CLEAN))
                {
                    dwMMMinVal = __pMMFileRecord[_dwMMTemp3].dwFileTime;
                    dwMMMinIndex = _dwMMTemp3;
                }
            }
            dwMMDiscMinIndex = dwMMDirOffset+dwMMMinIndex;
            dwMMDiscPreMinIndex = dwMMDirOffset+dwMMPreMinIndex;
            if (_dwMMTemp == 0) // mean the min value in __pMMFileRecord
            {
                __pMMDirNameList[wDirID].wMMFileRecordEntry = dwMMDiscMinIndex;
                __pMMFileRecord[dwMMMinIndex].wPrevFileRecord = FILE_RECORD_BEGIN;
            }
            else // other value in __pMMFileRecord
            {
                __pMMFileRecord[dwMMMinIndex].wPrevFileRecord = dwMMDiscPreMinIndex;
                __pMMFileRecord[dwMMPreMinIndex].wNextFileRecord = dwMMDiscMinIndex;
                if (_dwMMTemp == (_dwMMTemp1-1)) // mean the max value in __pMMFileRecord
                {
                    __pMMDirNameList[wDirID].wMMFileRecordEnd = dwMMDiscMinIndex;
                    __pMMFileRecord[dwMMMinIndex].wNextFileRecord = FILE_RECORD_END;
                }
            }
            dwMMMinVal = 0xFFFFFFFF;
        }
    }
	
    // doing filtering function.
    // Chuan0.85, Use the definition of CTKAV.H
    __pMMFileRecord = (PFILE_RECORD)__pMMFileRecordStart;
	
    _dwMMTemp1 = __pMMFilesOfDir[wDirID];
    // use _dwMMTemp3 to represent if first linked correctly. True mean not first linked OK, Flase mean first linked OK.
    _dwMMTemp3 = TRUE;
    dwMMMinIndex = __pMMDirNameList[wDirID].wMMFileRecordEntry;
    dwMMPreMinIndex = dwMMMinIndex;
    __pMMFilterFilesOfDir[wDirID] = 0;
	
    for (_dwMMTemp=0; _dwMMTemp<_dwMMTemp1; _dwMMTemp++)
    {
        if (_MM_CheckExtFilteredIn(__pMMFileRecord[dwMMMinIndex].FileName.bExtID,bFilterMode))
        {
            __pMMFilterFilesOfDir[wDirID]++;
            // mean this kind of ext name is filtered in, so need to link it.
            // mean first entry linked OK.
            if (_dwMMTemp3 == TRUE)
            {
                __pMMFileRecord[dwMMMinIndex].wPrevFileRecord = FILE_RECORD_BEGIN;
                __pMMDirNameList[wDirID].wMMFileRecordEntry = dwMMMinIndex;
                _dwMMTemp3 = FALSE;
            }
            else
            {
                __pMMFileRecord[dwMMMinIndex].wPrevFileRecord = dwMMPreMinIndex;
                __pMMFileRecord[dwMMPreMinIndex].wNextFileRecord = dwMMMinIndex;
            }
            dwMMPreMinIndex = dwMMMinIndex;
        }
        else
        {
            // mean this kind of ext name is filtered out, so don't need to link it.
        }
        dwMMMinIndex = __pMMFileRecord[dwMMMinIndex].wNextFileRecord;
    }
    if (_dwMMTemp3 == TRUE)
    {
        __pMMFileRecord[dwMMPreMinIndex].wPrevFileRecord = FILE_RECORD_NONE;
        __pMMFileRecord[dwMMPreMinIndex].wNextFileRecord = FILE_RECORD_NONE;
    }
    else
    {
        __pMMFileRecord[dwMMPreMinIndex].wNextFileRecord = FILE_RECORD_END;
        __pMMDirNameList[wDirID].wMMFileRecordEnd = dwMMPreMinIndex;
    }
    __pMMDirNameList[wDirID].bMMLinkStatus |= MM_SORT_FILTER_LINK_READY;
    __bMMPreExtFilterMode = bFilterMode;
    __bMMPreSortMethod = bSortMode;
    __wMMPreFilterDir = wDirID;
	return TRUE;
}

DWORD _MM_CheckExtFilteredIn(BYTE bExtID, BYTE bFilterMode)
{
    BYTE   bMMTemp;
    bMMTemp = 0x1 <<bExtID;
    if (bFilterMode & bMMTemp)
        return TRUE;
    else
        return FALSE;
}

WORD _MM_GetSortRecord(WORD wSortIndex, BYTE bType)
{
    BYTE    bIndex;
    if (bType == MM_SORT_PREV_RECORD)
    {
        bIndex=(BYTE)wSortIndex;
        while (1)
        {
            if (_wMMExtLastID[bIndex] != MM_SORT_INIT_VALUE)
                return _wMMExtLastID[bIndex];
            if (bIndex == 0)
                break;
            bIndex--;
        }
        return FILE_RECORD_BEGIN;
    }
    else
    {
        for (bIndex=(BYTE)wSortIndex; bIndex<MAX_EXTENSION_NO; bIndex++)
        {
            if (_wMMExtFirstID[bIndex] != MM_SORT_INIT_VALUE)
                return _wMMExtFirstID[bIndex];
        }
        return FILE_RECORD_END;
    }
}

DWORD MM_GetFileInfo(DWORD dwType)
{
    // wyc0.91, support function to get file time in select cursor.
    switch(dwType)
    {
	case    MM_GET_FILE_YEAR:
		return (((__pMMFileRecord->dwFileTime >> IF_YEAR_SHIFT) & (IF_YEAR_MASK_BIT)) + 1980);
	case    MM_GET_FILE_MONTH:
		return ((__pMMFileRecord->dwFileTime >> IF_MONTH_SHIFT) & (IF_MONTH_MASK_BIT));
	case    MM_GET_FILE_DATE:
		return ((__pMMFileRecord->dwFileTime >> IF_DATE_SHIFT) & (IF_DATE_MASK_BIT));
		/*
        case    MM_GET_FILE_HOUR:
		return ((__pMMFileRecord->dwFileTime >> IF_HOUR_SHIFT) & (IF_HOUR_MASK_BIT));
        case    MM_GET_FILE_MIN:
		return ((__pMMFileRecord->dwFileTime >> IF_MIN_SHIFT) & (IF_MIN_MASK_BIT));
        case    MM_GET_FILE_SEC:
		return ((__pMMFileRecord->dwFileTime >> IF_SEC_SHIFT) & (IF_SEC_MASK_BIT));
		*/
	case    MM_GET_FILE_LENGTH:
		return (__pMMFileRecord->FileRange.dwLen);
	default:
		break;
    }
    return 0xFFFFFFFF;	
}

//  *********************************************************************
//  Function    :   _MM_ConvertName
//  Description :   convert the name string to MP3xxx or DIRxxx
//              :   support max. 3 digits number
//  Arguments   :   bType,  MM_CONVERT_DIRNAME, indicate album name
//              :           MM_CONVERT_FILENAME, indicate song name
//              :   bIndex, the index for the name
//  Return      :   None
//  Global      :   _dwMMTemp7, _dwMMTemp6, __bTemp
//  *********************************************************************
void _MM_ConvertName(BYTE bType, WORD wIndex)
{
    DWORD dwLength;
    dwLength = MM_MAX_FILENAME_LENGTH;
    if (bType == MM_CONVERT_FILENAME)
        dwLength = MM_MAX_FILENAME_LENGTH;
    else if (bType == MM_CONVERT_DIRNAME)
        dwLength = MM_MAX_DIRNAME_LENGTH;
    
#ifdef  CONVERT_UNKNOWN_NAME
    // LLY2.80, replace "bUnRecoganize" with "_dwMMTemp6"
    _dwMMTemp6=0; // used to keep unrecoganize char number
    // Becasue, it's ok that we don't care the last char of directory name
    for (_dwMMTemp7=0;_dwMMTemp7<dwLength;_dwMMTemp7++)
    {
        _dwMMTemp8 = _pMMTempNameList->cName[_dwMMTemp7];
        // if "0", it means "end of string"
        if (__bMMUniFile != FALSE)
        {
            if((_dwMMTemp8==0) && (_pMMTempNameList->cName[_dwMMTemp7+1] == 0))
                break;
        }
        else
        {
            if (_dwMMTemp8==0)
                break;
        }
        // LLY0.84, collect the checking procedure into FMANAGER module
        //if(_OSD_ASCIItoFont(__bTemp) ==  CHAR_UNKNOW)
        if(_MM_QueryInfo1(MM_QUERY_UNKNOWN_CHAR, _dwMMTemp8))
#ifdef  SUPPORT_CONVERT_FILE_NAME
            _dwMMTemp6++;
        // Because only one character that are unknown char, it maybe be "chinese char", so we need to convert string.
        if(_dwMMTemp6 >= 1)  // means may have chinese letter
        {
            // assign as DIR_0xx
            if (bType==MM_CONVERT_DIRNAME)
            {
                strncpy(_pMMTempNameList->cName, "DIR", 3);
                _dwMMTemp7=3;
            }
            else
            {
                // wyc1.11-2, always transfer to FILEXXX when these files are Chinese name etc because of now supporting more kinds of EXT and can't
                // name them as MP3 / JPG ...
                strncpy(_pMMTempNameList->cName, "FILE", 4);
                _dwMMTemp7=4;  // FILE are four characters.
            }
            _pMMTempNameList->cName[_dwMMTemp7++]= (char)((wIndex)/100+'0');
            _pMMTempNameList->cName[_dwMMTemp7++]= (char)(((wIndex)%100)/10+'0');
            _pMMTempNameList->cName[_dwMMTemp7++]= (char)(((wIndex)%10+'0' ));
            _pMMTempNameList->cName[_dwMMTemp7]=0;
            break;
        }
#else
        {
            _dwMMTemp8 = _dwMMTemp8 % 26;
            _pMMTempNameList->cName[_dwMMTemp7] = 0x41 + _dwMMTemp8;
        }
#endif //
    }
#endif  // #ifdef  CONVERT_UNKNOWN_NAME
}

//  ***************************************************************************
//  Function    :   _MM_GetNoneJPEGItems
//  Description :   This function will calculate total not-JPEG files
//  Arguments   :   None
//  Return      :   Total not-JPEG files.
//  Side Effect :
//  Notice      :
//  ***************************************************************************
WORD _MM_GetNoneJPEGItems(void)
{
    WORD wFMTemp;

    for (wFMTemp = 0; wFMTemp < __pMMFilterFilesOfDir[__MMSelectCursor.wDirID]; wFMTemp++)
    {
        //CoCo1.00, to support > 200 files in a folder.
        if ((wFMTemp % MM_MAX_FILE_NO_ONCE_TIME) == 0) //need to prepare the files
        {
            MM_PreparePage(MM_PREPARE_FILEPAGE,__MMSelectCursor.wDirID, __bMMExtFilterMode, __bMMSortMethod);
        }

        if (__pMMFileRecord[wFMTemp % MM_MAX_FILE_NO_ONCE_TIME].FileName.bExtID == EXTNAME_JPG)
        {
            return wFMTemp;
        }
    }

    return 0;
}

#ifdef DISCARD_EMPTY_DIRECTORY
BYTE _MM_ExistEmptyFolder(void)
{
	for(_bMMIdx=1;_bMMIdx<__bMMTotalDirs;_bMMIdx++)
	{
		if((__pMMFilterFilesOfDir[_bMMIdx]) ||(__pMMVolumeDirTree[_bMMIdx].bNoOfSubDir))
			continue;
		else
			return TRUE;
	}
	return FALSE;
}
#endif

/**
 * Elmer, 2006/06/21
 * map avi files to correct sub-title files
 *
 * @pFileLog retrieved file structure
 *
 *
 */
#ifdef SUPPORT_CHAR_SUBPICTURE
//#define MP4SP_MATCH_EXACT_NAME
void _MM_GetMP4SP(PFILE_LOG pFileLog)
{
    PMP4SP_RECORD pMP4SPRecord, pMP4SPTempBuff;
    PDIRNAMEINFO pDirNameList;
    PMP4SP_INDEX pMP4SPIndex;
    PFILE_RECORD pFileRecord;
    WORD    wIndex1, wIndex2, wIndex3;
    WORD    wMP4Idx, wFileIdx;
    WORD    wFilesOfDir, wSPOfDir;
    BYTE    bFileNamePri;
    BYTE bTemp;
    // elmer2.78, must rewrite this function
#ifndef MP4SP_MATCH_EXACT_NAME
	
    if ((__bMP4SPFileCnt == 0) || (__bAVIFileCnt == 0) || (__bFileSystem == FILE_SYSTEM_UDF))
        return;
    pMP4SPTempBuff = (PMP4SP_RECORD) (_dwIFTempBuffAddr + sizeof(MP4SP_INDEX) * MM_MAX_DIR_NO);
    pMP4SPRecord = pFileLog->pMP4SPRecord;
    pFileLog->bSrcNum = 0;
	
    pDirNameList = (PDIRNAMEINFO) pFileLog->pName;
    pMP4SPIndex = _pMMMP4SPIndex;
    pFileRecord = __pMMFileRecordStart;
	
    wMP4Idx = 0;
    wFileIdx = 0;
    bFileNamePri = 0;
    for (wIndex1 = 0; wIndex1 < __bMMTotalDirs; wIndex1++)
    {
        if (__bTotalSRTFiles >= MM_MAX_SRT_NUM)
            return;
		
        if (pMP4SPIndex[wIndex1].wMMSPRecordEntry == 0xFFFF)
        {
            wSPOfDir = 0;
        }
        else
        {
            wSPOfDir = pMP4SPIndex[wIndex1].wMMSPRecordEnd - pMP4SPIndex[wIndex1].wMMSPRecordEntry + 1;
        }
		
        wFilesOfDir = __pMMFilesOfDir[wIndex1];	
		
        for (wIndex2 = wFileIdx; wIndex2 < (wFileIdx + wFilesOfDir); wIndex2++)
        {
            if (__bTotalSRTFiles >= MM_MAX_SRT_NUM)
                return;
			
            if (pFileRecord[wIndex2].FileName.bExtID == EXTNAME_AVI)
            {
                if (wSPOfDir != 0)
                {
                    strncpy((BYTE *) pMP4SPRecord->MP4SPName.cName,
                        (BYTE *) pMP4SPTempBuff[wMP4Idx].MP4SPName.cName, pFileLog->bMaxStringLen + 1);
                    pMP4SPRecord->MP4SPName.bExtID = pMP4SPTempBuff[wMP4Idx].MP4SPName.bExtID;
                    pMP4SPRecord->dwBeginTime = pMP4SPTempBuff[wMP4Idx].dwBeginTime;
                    pMP4SPRecord->dwLen = pMP4SPTempBuff[wMP4Idx].dwLen;
                    bTemp = pFileRecord[wIndex2].bFileAttr & IF_UNICODE_FILE_TAG;
                    pFileRecord[wIndex2].bFileAttr = pFileLog->bSrcNum;
                    pFileRecord[wIndex2].bFileAttr |= bTemp;
                    pFileLog->bSrcNum++;
					
                    if (strncmp((char *) pFileRecord[wIndex2].FileName.cName,
						(BYTE *) pMP4SPRecord->MP4SPName.cName,
						strlen(pFileRecord[wIndex2].FileName.cName)) != 0)
                    {
                        bFileNamePri = 0;
                    }
                    else
                    {
                        bFileNamePri = 1;
                    }
					
                    for (wIndex3 = (wMP4Idx + 1); wIndex3 < (wMP4Idx + wSPOfDir); wIndex3++)
                    {
                        if (strncmp((char *) pFileRecord[wIndex2].FileName.cName,
							(BYTE *) pMP4SPTempBuff[wIndex3].MP4SPName.cName,
							strlen(pFileRecord[wIndex2].FileName.cName)) == 0)
                        {
                            if (bFileNamePri == 1)
                            {
                                if (pMP4SPTempBuff[wIndex3].MP4SPName.bExtID < pMP4SPRecord->MP4SPName.bExtID)
                                {
                                    strncpy((BYTE *) pMP4SPRecord->MP4SPName.cName,
                                        (BYTE *) pMP4SPTempBuff[wIndex3].MP4SPName.cName,
                                        pFileLog->bMaxStringLen + 1);
                                    pMP4SPRecord->MP4SPName.bExtID = pMP4SPTempBuff[wIndex3].MP4SPName.bExtID;
                                    pMP4SPRecord->dwBeginTime = pMP4SPTempBuff[wIndex3].dwBeginTime;
                                    pMP4SPRecord->dwLen = pMP4SPTempBuff[wIndex3].dwLen;
                                }
                            }
                            else
                            {
								
                                strncpy((BYTE *) pMP4SPRecord->MP4SPName.cName,
                                    (BYTE *) pMP4SPTempBuff[wIndex3].MP4SPName.cName,
                                    pFileLog->bMaxStringLen + 1);
                                pMP4SPRecord->MP4SPName.bExtID = pMP4SPTempBuff[wIndex3].MP4SPName.bExtID;
                                pMP4SPRecord->dwBeginTime = pMP4SPTempBuff[wIndex3].dwBeginTime;
                                pMP4SPRecord->dwLen = pMP4SPTempBuff[wIndex3].dwLen;
                            }
                        }
                    }
                    pMP4SPRecord += 1;
                    __bTotalSRTFiles++;
                }
                else
                {
                    pFileRecord[wIndex2].bFileAttr |= 0x7F;  // no sub-title file
                }
            }
        }
        wMP4Idx += wSPOfDir;
        wFileIdx += wFilesOfDir;
    }
#else
    static BYTE bMP4SPFlag;
	
    if ((__bMP4SPFileCnt == 0) || (__bAVIFileCnt == 0) || (__bFileSystem == FILE_SYSTEM_UDF))
        return;

    pMP4SPTempBuff = (PMP4SP_RECORD) (_dwIFTempBuffAddr + sizeof(MP4SP_INDEX) * MM_MAX_DIR_NO);
    pMP4SPRecord = pFileLog->pMP4SPRecord;
    pFileLog->bSrcNum = 0;
	
    pDirNameList = (PDIRNAMEINFO) pFileLog->pName;
    pMP4SPIndex = _pMMMP4SPIndex;
    pFileRecord = __pMMFileRecordStart;
	
    wMP4Idx = 0;
    wFileIdx = 0;
    bFileNamePri = 0;
    for (wIndex1 = 0; wIndex1 < __bMMTotalDirs; wIndex1++)
    {
        if (__bTotalSRTFiles >= MM_MAX_SRT_NUM)
            return;
		
        if (pMP4SPIndex[wIndex1].wMMSPRecordEntry == 0xFFFF)
        {
            wSPOfDir = 0;
        }
        else
        {
            wSPOfDir = pMP4SPIndex[wIndex1].wMMSPRecordEnd - pMP4SPIndex[wIndex1].wMMSPRecordEntry + 1;
        }
		
        wFilesOfDir = __pMMFilesOfDir[wIndex1];	
		
        for (wIndex2 = wFileIdx; wIndex2 < (wFileIdx + wFilesOfDir); wIndex2++)
        {
            if (__bTotalSRTFiles >= MM_MAX_SRT_NUM)
                return;
			
            bMP4SPFlag = FALSE;
            if (pFileRecord[wIndex2].FileName.bExtID == EXTNAME_AVI)
            {
                if (wSPOfDir != 0)
                {
                    pMP4SPRecord->MP4SPName.bExtID = 0xFF;
                    for (wIndex3 = (wMP4Idx); wIndex3 < (wMP4Idx + wSPOfDir); wIndex3++)
                    {

                        if (strncmp((char *) pFileRecord[wIndex2].FileName.cName,
							(BYTE *) pMP4SPTempBuff[wIndex3].MP4SPName.cName,
							strlen(pFileRecord[wIndex2].FileName.cName)) == 0)
                        {
                            if (pMP4SPTempBuff[wIndex3].MP4SPName.bExtID < pMP4SPRecord->MP4SPName.bExtID)
                            {

                                strncpy((BYTE *) pMP4SPRecord->MP4SPName.cName,
                                    (BYTE *) pMP4SPTempBuff[wIndex3].MP4SPName.cName, pFileLog->bMaxStringLen + 1);
                                pMP4SPRecord->MP4SPName.bExtID = pMP4SPTempBuff[wIndex3].MP4SPName.bExtID;
                                pMP4SPRecord->dwBeginTime = pMP4SPTempBuff[wIndex3].dwBeginTime;
                                pMP4SPRecord->dwLen = pMP4SPTempBuff[wIndex3].dwLen;
                                bMP4SPFlag = TRUE;
                            }
                        }
                    }

                    if (bMP4SPFlag)
                    {
                        bTemp = pFileRecord[wIndex2].bFileAttr & IF_UNICODE_FILE_TAG;
                        pFileRecord[wIndex2].bFileAttr = pFileLog->bSrcNum;
                        pFileRecord[wIndex2].bFileAttr |= bTemp;
                        pFileLog->bSrcNum++;
                        bMP4SPFlag = TRUE;
                        pMP4SPRecord += 1;
                        __bTotalSRTFiles++;
                    }
                    else
                    {
                        pFileRecord[wIndex2].bFileAttr |= 0x7F;  // no sub-title file
                    }
                }
                else
                {
                    pFileRecord[wIndex2].bFileAttr |= 0x7F;  // no sub-title file
                }
            }
        }
        wMP4Idx += wSPOfDir;
        wFileIdx += wFilesOfDir;
    }
#endif
}
#endif //SUPPORT_CHAR_SUBPICTURE

void _MM_PrepareFilterFile(void)
{
    DWORD dwMMIndex;
    
    for (dwMMIndex=0; dwMMIndex<__bMMTotalDirs; dwMMIndex++)
        __pMMDirNameList[dwMMIndex].bMMLinkStatus &= ~MM_SORT_FILTER_LINK_READY;
    
    __wMMTotalFilterFiles = 0;
    for (dwMMIndex=0; dwMMIndex<(DWORD)__bMMTotalDirs; dwMMIndex++)
    {
        MM_PreparePage(MM_PREPARE_FILEPAGE,dwMMIndex, __bMMExtFilterMode, __bMMSortMethod);
        __wMMTotalFilterFiles += __pMMFilterFilesOfDir[dwMMIndex];
    }
}

void MM_File_Initial(void)
{
    __bMMSortMethod = MM_SORT_BY_TYPE;
    __bMMSortSequence = MM_SORT_BY_ASCENDING;
	
#ifndef SUPPORT_POWERON_MENU 
    __bMMExtFilterMode = EXT_BIT_ALL;
#endif //#ifndef SUPPORT_POWERON_MENU 
	
    __bMMExtFilterModeInMMMode = __bMMExtFilterMode;
    __bMMVolumeSet [0] = MM_VOLUME_SERVO;
	
    __wMMTotalFilterFiles = __wMMTotalFiles;
	
    for (_dwMMTemp1=0; _dwMMTemp1 < (DWORD)__bMMTotalDirs; _dwMMTemp1++)
	{
        __pMMFilterFilesOfDir[_dwMMTemp1] = __pMMFilesOfDir[_dwMMTemp1];		
	}
}
