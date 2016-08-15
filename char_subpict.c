#include "winav.h"
#include "ctkav.h"
#include "utl.h"
#include "infofilter.h"
#include "cc.h"
#include "hal.h"
#include "osd.h"
#include "osdnd.h"
#include "setup.h"
#include "mmanager.h"
#include "osdfont.h"
#include "parser.h"
#include "comutl.h"

//Alan1.10, for char-based subpicture
#ifdef SUPPORT_CHAR_SUBPICTURE
#include "subpict.h"
#include "par_sptx.h"
#include "char_subpict.h"
#include "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include "convchar.h"

#ifndef CUSTOMER_ASCII
#include "OSDString/Ascii_Remapping.txt"
#endif

#define ONE_LINE_CHAR_NUMBER 64 
#define CHAR_NUMBER  (ONE_LINE_CHAR_NUMBER * CHAR_SP_MAX_LINE_NUM)

// Damon2.56, for Hebrew external subtitle
#ifdef ISO_8859_8_FONT
#ifndef HEBREW_WORD_INVERSION
#define MAX_WORD_LENGTH	20
#define MAX_WORD_NUMBER	30
#endif
#endif

DWORD                __dwParResult_CharSP;

DWORD                _dwSPType;
DWORD                _dwSPStartSector;
DWORD                _dwSPEndSector;

PPARSER_SPTXT_HEADER _pSP_HeaderPt; 
PPARSER_SPTXT_HEADER _pSP_LastHeaderPt; 
PARSER_SPTXT_BUF     _Char_SP_InputInfo;
PARSER_SPTXT_BUF     _Char_SP_OutputInfo;
PARM_DRAW_STRING     _CHAR_SP_StrAttr; 
PARM_RECT            _CHAR_SP_RectAttr;

WORD                 _aString[CHAR_NUMBER];

WORD*                _aLinePtr[CHAR_SP_MAX_LINE_NUM];
//WORD*                _pwCurrentLocaleTable;     // Current character set in used
//WORD*                _pwCurrentUnicodeTable;    // Current character set in used
//WORD*                _pwMappingTable;

//BYTE                 _bFontHeightInTable;
BOOL                 _blAlreadyDisplay = FALSE;

// Damon2.56, for Hebrew external subtitle 
#ifdef ISO_8859_8_FONT
#ifdef HEBREW_IE_FORMAT
BYTE			_bHebSentenceInfo[CHAR_SP_MAX_LINE_NUM*2];	// Save sentence start position & length
BYTE			_bHebNumberInfo[MAX_WORD_NUMBER*2]; // Save number character start position & length
BYTE			_bHebSentenceIndex;
BYTE			_bHebNumberIndex;
#elif defined(HEBREW_WORD_FORMAT)
BYTE	_bHebWordInfo[MAX_WORD_NUMBER*2];	// Save word position & length
BYTE	_bHebWordIndex;
#endif
#endif

/*
#ifndef CUSTOMER_ASCII
#include "OSDString/Ascii_Remapping.txt"
#endif

WORD _aAsciiMappingTable[][2] = 
{
    #include "OSDString/Ascii_Mapping.txt"
};

#ifdef ISO_8859_15_FONT
WORD _a8859_15_MappingTable[][2] = 
{
    #include "OSDString/ISO_8859_15_Mapping.txt"
};
#ifdef SUPPORT_UTF8 
WORD _aUni_8859_15_MappingTable[][2] = 
{
    #include "OSDString/Uni_8859-15_Mapping.txt"
};
#endif
#endif

#ifdef ISO_8859_2_FONT
WORD _a8859_2_MappingTable[][2] = 
{
    #include "OSDString/ISO_8859_2_Mapping.txt"
};
#ifdef SUPPORT_UTF8 
WORD _aUni_8859_2_MappingTable[][2] = 
{
    #include "OSDString/Uni_8859-2_Mapping.txt"
};
#endif
#endif

#ifdef ISO_8859_7_FONT
WORD _a8859_7_MappingTable[][2] = 
{
    #include "OSDString/ISO_8859_7_Mapping.txt"
};
#ifdef SUPPORT_UTF8 
WORD _aUni_8859_7_MappingTable[][2] = 
{
    #include "OSDString/Uni_8859-7_Mapping.txt"
};
#endif
#endif

// elmer2.51, add Damon's code for Hebrew
// Damon2.38a, for Hebrew
#ifdef ISO_8859_8_FONT
WORD _a8859_8_MappingTable[][2] = 
{
    #include "OSDString/ISO_8859_8_Mapping.txt"
};
#ifdef SUPPORT_UTF8 
WORD _aUni_8859_8_MappingTable[][2] = 
{
    #include "OSDString/Uni_8859-8_Mapping.txt"
};
#endif
#endif


#ifdef ISO_8859_9_FONT
WORD _a8859_9_MappingTable[][2] = 
{
    #include "OSDString/ISO_8859_9_Mapping.txt"
};
#ifdef SUPPORT_UTF8 
WORD _aUni_8859_9_MappingTable[][2] = 
{
    #include "OSDString/Uni_8859-9_Mapping.txt"
};
#endif
#endif

#ifdef CP_1250_FONT
WORD _aCP_1250_MappingTable[][2] = 
{
    #include "OSDString/CP_1250_Mapping.txt"
};
#endif

#ifdef CP_1251_FONT
WORD _aCP_1251_MappingTable[][2] = 
{
    #include "OSDString/CP_1251_Mapping.txt"
};
#ifdef SUPPORT_UTF8 
WORD _aUni_CP_1251_MappingTable[][2] = 
{
    #include "OSDString/Uni_CP-1251_Mapping.txt"
};
#endif
#endif
*/


extern  DWORD   __dwTemp;
extern  BYTE    __bTemp;
extern  BYTE    __bOSDDynamicRegion;
extern  WORD    __wFontNum[];


BOOL _CHAR_SP_CheckPTS_ExSP(void);
#ifdef SUPPORT_UTF8 
BOOL _CHAR_SP_CheckPTS_MP4(void);
#endif

void _Convert_Locale_To_CTK_Strings(void);
void _Convert_UTF8_To_CTK_Strings(void);
void _Convert_Big5_To_Unicode(void);
void _Convert_Shift_JIS_To_Unicode(void);
void _Convert_UTF8_To_Unicode(void);
void _Change_Word_Direction(void);
BYTE _SP_Autowrap(void);
void _CHAR_SP_InitPalette(void);
void _CHAR_SP_InitSPU(void);
//void CHAR_SP_ClearRegion(void);
void _CHAR_SP_DrawStrings(void);
void _CHAR_SP_RecoverFunc(void); 
void _CHAR_SP_LoadSP(void);
#if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY)
void _CHAR_SP_UseExSPFontTable(BOOL bUseExSPFontTable);
#endif


////////////////// Public functions //////////////////////////////////////

void    CHAR_SP_Initial(void)
{
    __dwParResult_CharSP = PARSER_SPTXT_INVALID_RP;

    _pSP_HeaderPt = NULL; 
    _pSP_LastHeaderPt = NULL;

    _CHAR_SP_InitPalette();

    _CHAR_SP_StrAttr.bTextColor = CHAR_SP_ENTRY_COLOR_FG;
    _CHAR_SP_StrAttr.bBackground = CHAR_SP_ENTRY_COLOR_BG;

    _CHAR_SP_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _CHAR_SP_RectAttr.rect.wLeft = CHAR_SP_REG_LEFT;
    _CHAR_SP_RectAttr.rect.wRight= CHAR_SP_REG_RIGHT;
    CHAR_SP_AdjustRegion();

/*
#if (FONT_TABLE_METHOD != CREATE_DYNAMICALLY)
    // Shift the index for each character set and only do once
    // After do it, set _aAsciiMappingTable[0][1] = 0xFFFF

    if (_aAsciiMappingTable[0][1] != 0xFFFF)
    {
#ifdef ISO_8859_15_FONT
        for (__bTemp = 1; __bTemp < _a8859_15_MappingTable[0][0]; __bTemp++)
            _a8859_15_MappingTable[__bTemp][1] += OSD_FONT_NUM; 
#endif
#ifdef ISO_8859_2_FONT
        for (__bTemp = 1; __bTemp < _a8859_2_MappingTable[0][0]; __bTemp++)
            _a8859_2_MappingTable[__bTemp][1] += OSD_FONT_NUM;
#endif
#ifdef ISO_8859_7_FONT
        for (__bTemp = 1; __bTemp < _a8859_7_MappingTable[0][0]; __bTemp++)
            _a8859_7_MappingTable[__bTemp][1] += OSD_FONT_NUM; 
#endif
#ifdef ISO_8859_9_FONT
        for (__bTemp = 1; __bTemp < _a8859_9_MappingTable[0][0]; __bTemp++)
            _a8859_9_MappingTable[__bTemp][1] += OSD_FONT_NUM;
#endif
#ifdef CP_1251_FONT
        for (__bTemp = 1; __bTemp < _aCP_1251_MappingTable[0][0]; __bTemp++)
            _aCP_1251_MappingTable[__bTemp][1] += OSD_FONT_NUM; 
#endif
        _aAsciiMappingTable[0][1] = 0xFFFF;
    }

#endif  // #if (FONT_TABLE_METHOD != CREATE_DYNAMICALLY)
*/

    //CHAR_SP_ReplaceFont();

    PARSER_SPTXT_SetFilter(SUPPORT_EX_SUBTITLE_FORMAT);

    _Char_SP_InputInfo.dwAddr = (DWORD) NULL;
    _Char_SP_InputInfo.dwLen = 0;
    _Char_SP_OutputInfo.dwAddr = (DWORD) NULL;
    _Char_SP_OutputInfo.dwLen = 0;
}

void CHAR_SP_ReplaceFont(void)
{
/*
    switch (__SetupInfo.bExSPSTLanguage)
    {
#ifdef ISO_8859_15_FONT
    case SETUP_LANGUAGE_EX_SP_WESTERN_EUROPEAN:
        _pwCurrentLocaleTable = (WORD*) (_a8859_15_MappingTable[0]);
#ifdef SUPPORT_UTF8 
        _pwCurrentUnicodeTable = (WORD*) (_aUni_8859_15_MappingTable[0]);
#endif
        OSD_AddFontToTable(FONTTABLE_8859_15);
        _bFontHeightInTable = ISO_8859_15_FONT_HEIGHT;
        break;
#endif
#ifdef ISO_8859_2_FONT
    case SETUP_LANGUAGE_EX_SP_CENTRAL_EUROPEAN:
        _pwCurrentLocaleTable = (WORD*) (_a8859_2_MappingTable[0]);
#ifdef SUPPORT_UTF8 
        _pwCurrentUnicodeTable = (WORD*) (_aUni_8859_2_MappingTable[0]);
#endif
        OSD_AddFontToTable(FONTTABLE_8859_2);
        _bFontHeightInTable = ISO_8859_2_FONT_HEIGHT;
        break;
#endif
#ifdef ISO_8859_7_FONT
    case SETUP_LANGUAGE_EX_SP_GREEK:
        _pwCurrentLocaleTable = (WORD*) (_a8859_7_MappingTable[0]);
#ifdef SUPPORT_UTF8 
        _pwCurrentUnicodeTable = (WORD*) (_aUni_8859_7_MappingTable[0]);
#endif
        OSD_AddFontToTable(FONTTABLE_8859_7);
        _bFontHeightInTable = ISO_8859_7_FONT_HEIGHT;
        break;
#endif
// elmer2.51, add Damon's code for Hebrew
#ifdef ISO_8859_8_FONT
    case SETUP_LANGUAGE_EX_SP_HEBREW:
        _pwCurrentLocaleTable = (WORD*) (_a8859_8_MappingTable[0]);
#ifdef SUPPORT_UTF8 
        _pwCurrentUnicodeTable = (WORD*) (_aUni_8859_8_MappingTable[0]);
#endif
        OSD_AddFontToTable(FONTTABLE_8859_8);
        _bFontHeightInTable = ISO_8859_8_FONT_HEIGHT;
        break;
#endif
#ifdef ISO_8859_9_FONT
    case SETUP_LANGUAGE_EX_SP_TURKISH:
        _pwCurrentLocaleTable = (WORD*) (_a8859_9_MappingTable[0]);
#ifdef SUPPORT_UTF8 
        _pwCurrentUnicodeTable = (WORD*) (_aUni_8859_9_MappingTable[0]);
#endif
        OSD_AddFontToTable(FONTTABLE_8859_9);
        _bFontHeightInTable = ISO_8859_9_FONT_HEIGHT;
        break;
#endif
#ifdef CP_1250_FONT
    case SETUP_LANGUAGE_EX_SP_CP1250:
        _pwCurrentLocaleTable = (WORD*) (_aCP_1250_MappingTable[0]);
#ifdef SUPPORT_UTF8 
        _pwCurrentUnicodeTable = (WORD*) (_aUni_8859_2_MappingTable[0]);
#endif
        OSD_AddFontToTable(FONTTABLE_8859_2);
        _bFontHeightInTable = ISO_8859_2_FONT_HEIGHT;
        break;
#endif
#ifdef CP_1251_FONT
    case SETUP_LANGUAGE_EX_SP_CYRILLIC:
        _pwCurrentLocaleTable = (WORD*) (_aCP_1251_MappingTable[0]);
#ifdef SUPPORT_UTF8 
        _pwCurrentUnicodeTable = (WORD*) (_aUni_CP_1251_MappingTable[0]);
#endif
        OSD_AddFontToTable(FONTTABLE_CP_1251);
        _bFontHeightInTable = CP_1251_FONT_HEIGHT;
        break;
#endif
    case SETUP_LANGUAGE_EX_SP_ASCII:
    default:
        _pwCurrentLocaleTable = (WORD*) (_aAsciiMappingTable[0]);
#ifdef SUPPORT_UTF8 
        _pwCurrentUnicodeTable = (WORD*) (_aAsciiMappingTable[0]);
#endif
        break;
    }*/
}


void CHAR_SP_ReloadFromTime(void)
{
    if (! (PARSER_SPTXT_SUCCESS == __dwParResult_CharSP ||
        PARSER_SPTXT_BUFFER_OVERFLOW == __dwParResult_CharSP))
    {
        return;
    }

    _pSP_HeaderPt = (PPARSER_SPTXT_HEADER)_Char_SP_OutputInfo.dwAddr;

    PARSER_Command(0, PARSER_CMD_PAUSE, NULL);
    // wyc2.53-909P,
    SrcFilter_Stop(__dwSFStreamID);

    SrcFilter_ReadSectors(__dwSFStreamID, _dwSPStartSector, _dwSPEndSector);

    PARSER_SPTXT_initial();

    __dwParResult_CharSP = PARSER_SPTXT(&_Char_SP_InputInfo,
                                        &_Char_SP_OutputInfo, __dwGoToTime * 90000, _dwSPType);

//    PARSER_Command(0, PARSER_CMD_RESUME, NULL);
}


////////////////// Private functions //////////////////////////////////////

void _CHAR_SP_RecoverFunc(void)
{
    CHAR_SP_Trigger();
}

void _CHAR_SP_LoadSP(void)
{
    DWORD dwReadAVSector, dwSPSector, dwTime1;

    if (PARSER_SPTXT_SUCCESS == __dwParResult_CharSP) return;

    PARSER_Command(0, PARSER_CMD_PAUSE, NULL);
    dwTime1 = OS_GetSysTimer();
    HAL_REORDER_BARRIER();
    while ((OS_GetSysTimer() - dwTime1) < COUNT_2_SEC)
    {
        if(PARSER_QueryEvent(0, PARSER_EVENT_COMPLETED, TRUE))
        {
            break;
        }
    }

    // save current read sector
    dwReadAVSector = PARSER_SPTXT_ReadSector();
    dwSPSector = PARSER_SPTXT_SPSector();

    SrcFilter_Stop(__dwSFStreamID);
    SrcFilter_ReadSectors(__dwSFStreamID, dwSPSector, _dwSPEndSector);

    __dwParResult_CharSP = PARSER_SPTXT(&_Char_SP_InputInfo, &_Char_SP_OutputInfo, 0, _dwSPType);

    SrcFilter_Stop(__dwSFStreamID);
    SrcFilter_ReadSectors(__dwSFStreamID, dwReadAVSector, __dwTimeEnd);

    _pSP_HeaderPt = (PPARSER_SPTXT_HEADER)_Char_SP_OutputInfo.dwAddr;

    PARSER_Command(0, PARSER_CMD_RESUME, NULL);
}

void    CHAR_SP_Reset(void)
{
#ifndef EX_SP_USE_SPU_BUFFER
    if (__bOSDDynamicRegion) 
    {
        CHAR_SP_ClearRegion();
        OSD_DeleteDynamicRegion();
    }
#endif

    if (PARSER_ESFMT_TEXT_SP_EXT == __SP1.ESFormat)
    {
        if (PARSER_SPTXT_SUCCESS == __dwParResult_CharSP ||
            PARSER_SPTXT_BUFFER_OVERFLOW == __dwParResult_CharSP)
        {
            _pSP_HeaderPt = (PPARSER_SPTXT_HEADER)_Char_SP_OutputInfo.dwAddr;
#ifdef EX_SP_USE_SPU_BUFFER
            _CHAR_SP_InitSPU();
#endif
        }
        else
        {
            _pSP_HeaderPt = NULL;
        }
    }
    else if (PARSER_ESFMT_TEXT_SP == __SP1.ESFormat)
    {
        // Need to implement
        _pSP_HeaderPt = NULL;
    }

    _pSP_LastHeaderPt = NULL;
}

void    CHAR_SP_AdjustRegion(void)
{
#ifndef EX_SP_USE_SPU_BUFFER
    if (TRUE == __bOSDDynamicRegion)
    {
        CHAR_SP_ClearRegion();
    }
#endif

    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC ||
        __bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M)
    {
        _CHAR_SP_RectAttr.rect.wBottom = CHAR_SP_REG_BOTTOM;
        _CHAR_SP_RectAttr.rect.wTop = CHAR_SP_REG_TOP;
    }
    else
    {
#ifndef EX_SP_POS_INDEPEND_OSDND
        _CHAR_SP_RectAttr.rect.wBottom = __OSDNDBottomMsgAttr.wY;
#else
        _CHAR_SP_RectAttr.rect.wBottom = CHAR_SP_REG_BOTTOM_PAL; 
#endif
        _CHAR_SP_RectAttr.rect.wTop = CHAR_SP_REG_TOP_PAL;
    }
}

void    CHAR_SP_SetOutputBuffer(DWORD dwBufferAddress, DWORD dwBufferSize)
{
    _Char_SP_OutputInfo.dwAddr = dwBufferAddress;
    _Char_SP_OutputInfo.dwLen  = dwBufferSize;
}

void    CHAR_SP_ParseSPFile(void)
{
    DWORD dwFileBytes;
    BYTE  bSecNum;
    BYTE  bFileType;

    if (!MM_GetSRTRecord(&_dwSPStartSector, &dwFileBytes, &bFileType))
    {
        CHAR_SP_Initial();
        CHAR_SP_Reset();
        return;
    }

    switch (bFileType)
    {
#ifdef PARSER_SPTXT_SRT
    case MP4SP_EXT_SRT:
        _dwSPType = PARSER_SPTXT_SRT;
        break;
#endif
#ifdef PARSER_SPTXT_SSA 
    case MP4SP_EXT_SSA:
    case MP4SP_EXT_ASS: 
        _dwSPType = PARSER_SPTXT_SSA;
        break;
#endif
#ifdef PARSER_SPTXT_SUB 
    case MP4SP_EXT_SUB:
        _dwSPType = PARSER_SPTXT_SUB;
        break;
#endif
#ifdef PARSER_SPTXT_SMI 
    case MP4SP_EXT_SMI:
        _dwSPType = PARSER_SPTXT_SMI;
        break;
#endif
#ifdef PARSER_SPTXT_PSB
    case MP4SP_EXT_PSB:
        _dwSPType = PARSER_SPTXT_PSB;
        break;
#endif
    default:
        return;
    }

/*** Alan2.36, the output buffer will be set in CHAR_SP_SetOutputBuffer().
#ifdef EX_SP_SHARE_WITH_FIRMWARE_BUFFER 
    // wyc2.21-909S, run time to update the SP output address and length by MM FW buffer usage.
    _Char_SP_OutputInfo.dwAddr = __dwMMSPBuffST;
    _Char_SP_OutputInfo.dwLen  = __dwMMSPBuffSize;
#else
    _Char_SP_OutputInfo.dwAddr = CHAR_SP_BUFFER;
    _Char_SP_OutputInfo.dwLen  = CHAR_SP_BUFFER_SIZE;
#endif
*/

    _Char_SP_InputInfo.dwAddr = (DWORD) NULL;
    _Char_SP_InputInfo.dwLen  = dwFileBytes;
    
    bSecNum = (BYTE)((_Char_SP_InputInfo.dwLen + (IF_DUMP_IN_SECTOR_SIZE- 1))
              / IF_DUMP_IN_SECTOR_SIZE); 

    _dwSPEndSector = _dwSPStartSector + bSecNum - 1;

    SrcFilter_Stop(__dwSFStreamID);
    SrcFilter_ReadSectors(__dwSFStreamID, _dwSPStartSector, _dwSPEndSector);

    PARSER_SPTXT_initial();

    __dwParResult_CharSP = PARSER_SPTXT(&_Char_SP_InputInfo,
                                        &_Char_SP_OutputInfo, 0, _dwSPType);

    if (PARSER_SPTXT_SUCCESS == __dwParResult_CharSP || 
        PARSER_SPTXT_BUFFER_OVERFLOW == __dwParResult_CharSP) 
    {
        _pSP_HeaderPt = (PPARSER_SPTXT_HEADER)_Char_SP_OutputInfo.dwAddr;

#ifdef  SUPPORT_MP4
        // LLY1.50, specify different bitstream format base on file type
        if(MM_GetFileType(__MMPlayingItem.wDirID, __wPlayItem)==EXTNAME_MP4)
        {
            PARSER_SetDataFmt(0, PARSER_DATAFMT_NOCHANGE, PARSER_BSFMT_MP4);            
        }
        else
#endif  // #ifdef SUPPORT_MP4            
        {
            PARSER_SetDataFmt(0, PARSER_DATAFMT_NOCHANGE, PARSER_BSFMT_AVI);
        }
        // LLY1.20, check if support char base subpicture
        // if yes, increase Ns by one, assign SPSTNO as 0 (1st SPST ID)
        // And, insert this SPST into parser SPST arrary
        if( PARSER_AddExtTextSP(0) )
        {
            __bSPSTNs += 1;
            __bSPSTNO = 0;
            __bSPSTID=__bSPSTNO;        
        }

#ifdef EX_SP_USE_SPU_BUFFER
        _CHAR_SP_InitSPU();      
#endif
    }
    else
    {
        _pSP_HeaderPt = NULL;
    }
}


void    CHAR_SP_Trigger(void)
{
    // Prevent to show subtitle in Setup Mode
    if (!__btPlaying) 
    {
        CHAR_SP_ClearRegion();
        return;   
    }

    if (FALSE == (__SPUCtrl.bEnable & SPCTRL_REALPTS) || 0 == (__SPUCtrl.bEnable & SPCTRL_SP1))
    {
        CHAR_SP_ClearRegion();
//Alan1.20, reomve this code
//        return;
    }

    if (PARSER_ESFMT_TEXT_SP_EXT == __SP1.ESFormat)
    {
        if (!_CHAR_SP_CheckPTS_ExSP())
            return;
    } 
#ifdef SUPPORT_UTF8 
    else if (PARSER_ESFMT_TEXT_SP == __SP1.ESFormat)
    {
        if (!_CHAR_SP_CheckPTS_MP4())
            return;
    }
#endif
    else
    {
        return;
    }
    
//    if (FALSE == (__SPUCtrl.bEnable & SPCTRL_SP1)) return;
//Alan1.20, update the above code
    if (FALSE == (__SPUCtrl.bEnable & SPCTRL_REALPTS) || 0 == (__SPUCtrl.bEnable & SPCTRL_SP1))
        return;

#ifndef EX_SP_USE_SPU_BUFFER
    if (FALSE == __bOSDDynamicRegion)
    {
        OSD_NewDynamicRegion(_CHAR_SP_RecoverFunc);
    }

    if (OSD_CanUpdateDynamicRegionInfo() == FALSE)
        return;
#endif

    if (_blAlreadyDisplay)
        return;

    _CHAR_SP_DrawStrings();
}

BOOL _CHAR_SP_CheckPTS_ExSP(void)
{
    if ( ! (PARSER_SPTXT_SUCCESS == __dwParResult_CharSP || 
        PARSER_SPTXT_BUFFER_OVERFLOW == __dwParResult_CharSP) )
        return FALSE;

    if (PARSER_SPTXT_END == _pSP_HeaderPt->dwStart)
    {
        CHAR_SP_ClearRegion();
        _CHAR_SP_LoadSP();
        return FALSE;
    }

   HAL_ReadInfo(HAL_INFO_STC_UPDATED, &__dwTemp);
   if (FALSE == __dwTemp)
        return FALSE;

    HAL_ReadInfo(HAL_INFO_STC, &__dwTemp);
   
    if (__dwTemp < _pSP_HeaderPt->dwStart)    // PTS < start time
        return FALSE; 

    if (__dwTemp > _pSP_HeaderPt->dwEnd)    // PTS > end time
    {
        CHAR_SP_ClearRegion();
        
        _pSP_HeaderPt = (PPARSER_SPTXT_HEADER) ((PBYTE)_pSP_HeaderPt +
                     (((_pSP_HeaderPt->bLen + PARSER_SPTXT_HEADER_SIZE + 3) >> 2) << 2));
        _blAlreadyDisplay = FALSE;
        return FALSE;
    }

    if (PARSER_SPTXT_END == _pSP_HeaderPt->dwStart)
    {
        return FALSE;
    }

    return TRUE;
}

#ifdef SUPPORT_UTF8 
BOOL _CHAR_SP_CheckPTS_MP4()
{
    // Check whether it is the end of all the subtitle
    if ( __SP1.dwRead_Counter ==  __SP1.dwWrite_Counter)
        return FALSE;

    _pSP_HeaderPt = (PPARSER_SPTXT_HEADER)__SP1.dwReadPtr;

    // Check SP ID
    if (__bSPSTID != _pSP_HeaderPt->bID)
        return FALSE;


    HAL_ReadInfo(HAL_INFO_STC, &__dwTemp);
   
    if (__dwTemp < _pSP_HeaderPt->dwStart)    // PTS < start time
        return FALSE; 

    if (__dwTemp > _pSP_HeaderPt->dwEnd)    // PTS > end time
    {
        CHAR_SP_ClearRegion();
        __SP1.dwReadPtr = __SP1.dwSPU_NextAddress[__SP1.dwRead_Counter & SPU_MASK];
        __SP1.dwRead_Counter++;
        return FALSE;
    }

    return TRUE;
}
#endif

void _CHAR_SP_DrawStrings(void)
{
    BYTE   bLineIndex;

    if (PARSER_ESFMT_TEXT_SP_EXT == __SP1.ESFormat)
    {
        switch (__bEnCodeType)
        {               
#ifdef SUPPORT_UNICODE_CODING       
        case CHAR_ENCODE_UNICODE:
            _Convert_UTF8_To_Unicode();
            break;
#ifdef SUPPORT_CHAR_ENCODING_BIG5
        case CHAR_ENCODE_OTHER:
            _Convert_Big5_To_Unicode();
            break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_SHIFT_JIS
        case CHAR_ENCODE_OTHER:
            _Convert_Shift_JIS_To_Unicode();
            break;
#endif
#endif
        case CHAR_ENCODE_ISO_CP:
        default:
        _Convert_Locale_To_CTK_Strings();
#ifdef ISO_8859_8_FONT
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
        // Damon2.56, Hebrew language need to change word direction
        if (__SetupInfo.bExSPSTLanguage == SETUP_LANGUAGE_ENCODING_HEBREW)
//#endif
        {        
            _Change_Word_Direction();
        }
#endif
            break;
        }
    }
#ifdef SUPPORT_UTF8 
    else if (PARSER_ESFMT_TEXT_SP == __SP1.ESFormat)
    {
        _Convert_UTF8_To_CTK_Strings();
    }
#endif

    bLineIndex = _SP_Autowrap();

#ifdef EX_SP_USE_SPU_BUFFER
    // Use the SP buffer
    SPU_BMP_SetDisplay(0, FALSE);

    SPU_BMP_SetDisplayArea(0, 51, CHAR_SP_REG_TOP, __RegionList[GDI_SP_REGION_ID].wWidth,
        __RegionList[GDI_SP_REGION_ID].wHeight, (const BYTE*)__RegionList[GDI_SP_REGION_ID].dwTAddr);

    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC || __bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M)
    {
        _CHAR_SP_StrAttr.wY = (CHAR_SP_MAX_LINE_NUM - bLineIndex) * CHAR_SP_LINE_HEIGHT;
    }
    else
    {
        _CHAR_SP_StrAttr.wY = (CHAR_SP_MAX_LINE_NUM - bLineIndex) * CHAR_SP_LINE_HEIGHT;
    }
#else  // #ifdef EX_SP_USE_SPU_BUFFER
    // Use the OSD buffer
    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC || __bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M)
    {
        _CHAR_SP_StrAttr.wY = CHAR_SP_REG_BOTTOM - (bLineIndex * CHAR_SP_LINE_HEIGHT);
    }
    else
    {
#ifndef EX_SP_POS_INDEPEND_OSDND
        _CHAR_SP_StrAttr.wY = __OSDNDBottomMsgAttr.wY - (bLineIndex * CHAR_SP_LINE_HEIGHT);
#else
        _CHAR_SP_StrAttr.wY = CHAR_SP_REG_BOTTOM_PAL - (bLineIndex * CHAR_SP_LINE_HEIGHT);
#endif
    }
#endif

    bLineIndex = 0;

//#ifndef SUPPORT_UNICODE_CODING
    //GDI_SetFontHeightInTable(_bSPFontHeight); 
//#endif


/*#if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY)
    _CHAR_SP_UseExSPFontTable(TRUE);
#endif */

    while (_aLinePtr[bLineIndex] && bLineIndex < CHAR_SP_MAX_LINE_NUM) 
    {
        if (__bEnCodeType == CHAR_ENCODE_ISO_CP)
        {
            __dwTemp = GDI_GetStringWidth_909(_aLinePtr[bLineIndex]);
        }
#ifdef SUPPORT_UNICODE_CODING
        else // CHAR_SP_CODE_UNICODE
        {
            __dwTemp = GDI_GetUniStringWidth(_aLinePtr[bLineIndex]);
        }
#endif
 
        _CHAR_SP_StrAttr.wX = CHAR_SP_REG_LEFT + (CHAR_SP_REG_WIDTH -__dwTemp) / 2;
 
        if (__bEnCodeType == CHAR_ENCODE_ISO_CP)
        {
#ifdef EX_SP_USE_SPU_BUFFER
        GDI_DrawString_909(GDI_SP_REGION_ID, &_CHAR_SP_StrAttr, _aLinePtr[bLineIndex]);
#else
        GDI_DrawString_909(0, &_CHAR_SP_StrAttr, _aLinePtr[bLineIndex]);
#endif
        }
#ifdef SUPPORT_UNICODE_CODING
        else // CHAR_SP_CODE_UNICODE
        {
#ifdef EX_SP_USE_SPU_BUFFER
        GDI_DrawStringByUnicode(GDI_SP_REGION_ID, &_CHAR_SP_StrAttr, _aLinePtr[bLineIndex]);
#else
        GDI_DrawStringByUnicode(0, &_CHAR_SP_StrAttr, _aLinePtr[bLineIndex]);
#endif
        }
#endif // #ifdef SUPPORT_UNICODE_CODING

        _CHAR_SP_StrAttr.wY += CHAR_SP_LINE_HEIGHT;
        bLineIndex++;
    }

/*
#ifdef SUPPORT_UNICODE_CODING
    OSD_SetCurFontTable(FONTABLE_OSD);
#endif
*/

    //GDI_SetFontHeightInTable(OSD_FONT_HEIGHT); 

/*#if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY)
    _CHAR_SP_UseExSPFontTable(FALSE);
#endif */

#ifdef EX_SP_USE_SPU_BUFFER
    SPU_BMP_SetDisplay(0, TRUE);
#endif
    _blAlreadyDisplay = TRUE;

}

void _Convert_Locale_To_CTK_Strings(void)
{
    WORD   wReadByteNum;
    BYTE   bCharNum, bLineIndex, bBufferIndex;
    PBYTE  pString;     // Point to the current translated character
#ifdef ISO_8859_8_FONT
#ifdef HEBREW_IE_FORMAT
    BYTE	bHebCharNum;	// save the character numbers of a word
    BYTE	bHebNewSentence;
    BYTE	bHebNumberWord;
    BYTE	bHebCharStart;
    BYTE	bSpaceIndex;

    // initial variables
    bHebCharNum = 0;
    bHebNewSentence = TRUE;
    bHebNumberWord = FALSE;
    bHebCharStart = 0;
    bSpaceIndex = 0;
    _bHebSentenceIndex = 0;
    _bHebNumberIndex = 0;
#elif defined(HEBREW_WORD_FORMAT)
    BYTE	bHebCharNum;	// save the character numbers of a word
    BYTE	bHebChar;

    // initial variables
    bHebCharNum = 0;
    bHebChar = FALSE;
    _bHebWordIndex = 0;
#endif
#endif

    /*
    if (_pSP_LastHeaderPt == _pSP_HeaderPt)
        return ;
    */

    for (bLineIndex = 0; bLineIndex < CHAR_SP_MAX_LINE_NUM; bLineIndex++)
        _aLinePtr[bLineIndex] = NULL;

    pString = ((PBYTE)_pSP_HeaderPt) + PARSER_SPTXT_HEADER_SIZE;

    wReadByteNum = 0;
    bCharNum = 0;
    bBufferIndex = 1;

    bLineIndex = 0;
    _aLinePtr[bLineIndex] = _aString;

    while (wReadByteNum < _pSP_HeaderPt->bLen && bLineIndex < CHAR_SP_MAX_LINE_NUM) 
    {
        if (*pString != 0x0D)
        {
#ifdef ISO_8859_8_FONT   
#ifdef HEBREW_IE_FORMAT  
            // Damon2.56, find each Hebrew sentence and save it's position & length
            // we will use this info to change each sentence's direction
            if ((*pString >= 0xE0 && *pString <= 0xFA) ||(*pString >= 0x30 && *pString <= 0x39))	// find Hebrew Character & number Character
            {
                 if (!bHebNewSentence)
                 {
                     bHebCharNum = bCharNum - bHebCharStart + 1; 
                     if (*pString <= 0x39) // it's number character
                     {
                         bHebNumberWord = TRUE;
                     }
                 }
                 else if (*pString >= 0xE0) // it's Hebrew character
                 {
                     bHebCharStart = bCharNum;
                     _bHebSentenceInfo[_bHebSentenceIndex++] = bBufferIndex; // the Hebrew sentence start point
                     bHebNewSentence = FALSE;
                 }                 
				 
            } 
            // find number word, it need to chang again
            else if ((*pString == 0x20) && (!bHebNewSentence)) 
            {
                if (!bHebNumberWord)
                {
                    bSpaceIndex = bBufferIndex;
                }
                else
                {
                    _bHebNumberInfo[_bHebNumberIndex++] = bSpaceIndex + 1;
                    _bHebNumberInfo[_bHebNumberIndex++] = bBufferIndex - bSpaceIndex -1;
                    bHebNumberWord = FALSE;	
                    bSpaceIndex = bBufferIndex;
                }				
            }
#elif defined(HEBREW_WORD_FORMAT)
            // Damon2.56, find each Hebrew word and save it's position & length
            // we will use this info to change each word's direction
            if (*pString >= 0xE0 && *pString <= 0xFA)	// find Hebrew Characters 
            {
                 if (!bHebChar)
                 {
		        bHebChar = TRUE;
                     // save the position of the first character of a word  
                     _bHebWordInfo[_bHebWordIndex++] = bBufferIndex;	
                 }
                                     
                 bHebCharNum++;                 		    
            }
            else
            {
                 if (bHebChar)
                 {
		        bHebChar = FALSE;
			  // save word length	
                     _bHebWordInfo[_bHebWordIndex++] = bHebCharNum;
                     bHebCharNum = 0;                     
                 }
            }
#endif
#endif	
            bCharNum++; 
            _aString[bBufferIndex] = CONVCHAR_ISO_CP_To_CTK(*pString);                        
        }
        else 
        {
#ifdef ISO_8859_8_FONT 
#ifdef HEBREW_IE_FORMAT   
            if (!bHebNewSentence)
            {
                bHebNewSentence = TRUE;
                // save word length	
                _bHebSentenceInfo[_bHebSentenceIndex++] = bHebCharNum;
                bHebCharNum = 0;  
                bHebCharStart = 0;

                if (bHebNumberWord)
                {
                    _bHebNumberInfo[_bHebNumberIndex++] = bSpaceIndex + 1;
                    _bHebNumberInfo[_bHebNumberIndex++] = bBufferIndex - bSpaceIndex -1;
                    bHebNumberWord = FALSE;	                    
                }
            }
#elif defined(HEBREW_WORD_FORMAT)
            if (bHebChar)
            {
                bHebChar = FALSE;
                // save word length	
                _bHebWordInfo[_bHebWordIndex++] = bHebCharNum;
                bHebCharNum = 0;                     
            }   
#endif           
#endif	
            if (bLineIndex < CHAR_SP_MAX_LINE_NUM)
            {
                _aLinePtr[bLineIndex] = _aString + bBufferIndex - bCharNum - 1;
                *(_aLinePtr[bLineIndex]) = bCharNum;
            }
            bLineIndex++;
            bCharNum = 0;
        }
        bBufferIndex++; 
        pString++;
        wReadByteNum++;
    }

#ifdef ISO_8859_8_FONT
#ifdef HEBREW_IE_FORMAT   
    if (!bHebNewSentence)
    {                
         // save word length	
         _bHebSentenceInfo[_bHebSentenceIndex++] = bHebCharNum;    

         if (bHebNumberWord)
         {
             _bHebNumberInfo[_bHebNumberIndex++] = bSpaceIndex + 1;
             _bHebNumberInfo[_bHebNumberIndex++] = bBufferIndex - bSpaceIndex -1;                    
         }				
     } 
#elif defined(HEBREW_WORD_FORMAT)
    if (bHebChar)
    {
        bHebChar = FALSE;
        // save word length	
        _bHebWordInfo[_bHebWordIndex++] = bHebCharNum;
        bHebCharNum = 0;                     
    }   
#endif           
#endif

    if (bLineIndex < CHAR_SP_MAX_LINE_NUM)
    {
        _aLinePtr[bLineIndex] = _aString + bBufferIndex - bCharNum - 1;
        *(_aLinePtr[bLineIndex]) = bCharNum;
    }
}

#ifdef SUPPORT_UTF8 
void _Convert_UTF8_To_CTK_Strings(void)
{
/*
    WORD   wReadByteNum;
    WORD   wOutputCode;
    BYTE   bReadBuffer[4];
    BYTE   bCharNum, bLineIndex, bBufferIndex;
    PBYTE  pString;     // Point to the start address of the string

    // The string in buffer has been translated into CTK before
    
//    if (_pSP_LastHeaderPt == _pSP_HeaderPt)
//        return;
    

    for (bLineIndex = 0; bLineIndex < CHAR_SP_MAX_LINE_NUM; bLineIndex++)
        _aLinePtr[bLineIndex] = NULL;

    pString = ((PBYTE)_pSP_HeaderPt) + PARSER_SPTXT_HEADER_SIZE;

    wReadByteNum = 0;
    bCharNum = 0;
    bBufferIndex = 1;

    bLineIndex = 0;
    _aLinePtr[bLineIndex] = _aString;

    // Translate the UTF8 -> UTF -> CTK Font Index
    while (wReadByteNum < _pSP_HeaderPt->bLen && bLineIndex < CHAR_SP_MAX_LINE_NUM) 
    {
        bReadBuffer[0] = *pString;  // Get the first byte

        if (0x80 > bReadBuffer[0])           // 0xxxxxxx
        {
            wOutputCode = (WORD) bReadBuffer[0];
            wReadByteNum++;
        }
        else if (0xE0> bReadBuffer[0])       // 110xxxxx 10xxxxxx
        {
            bReadBuffer[1] = *(pString+1);  // Get the second byte
            wOutputCode = ((WORD)(bReadBuffer[0] & 0x1F)) << 6;
            wOutputCode |= (WORD) (bReadBuffer[1] & 0x3F);
            wReadByteNum += 2;
        }
        else if (0xF0 > bReadBuffer[0])      // 1110xxxx 10xxxxxx 10xxxxxx
        {
            bReadBuffer[1] = *(pString+1);  // Get the second byte
            bReadBuffer[2] = *(pString+2);  // Get the third byte
            wOutputCode = ((WORD)(bReadBuffer[0] & 0x0F)) << 12;
            wOutputCode |= ((WORD)(bReadBuffer[1] & 0x3F)) << 6;
            wOutputCode |= (WORD)(bReadBuffer[2] & 0x3F);
            wReadByteNum += 3;
        }
        else if (0xF8 > bReadBuffer[0])      // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        {
            bReadBuffer[1] = *(pString+1);  // Get the second byte
            bReadBuffer[2] = *(pString+2);  // Get the third byte
            bReadBuffer[3] = *(pString+3);  // Get the fourth byte
            wOutputCode = ((WORD)(bReadBuffer[0] & 0x07)) << 18;
            wOutputCode |= ((WORD)(bReadBuffer[1] & 0x3F)) << 12;
            wOutputCode |= ((WORD)(bReadBuffer[2] & 0x3F)) << 6;
            wOutputCode |= (WORD)(bReadBuffer[3] & 0x3F);
            wReadByteNum += 4;
        }

        if (0x0D != wOutputCode)
        {
            _pwMappingTable = (WORD*) (_aAsciiMappingTable[0]);
            for (__bTemp = 1; __bTemp < _pwMappingTable[0]; __bTemp++)
            {
                if (_pwMappingTable[__bTemp * 2] == wOutputCode)
                {
                    break;
                }
            }

            if (__bTemp == _pwMappingTable[0] && NULL != _pwCurrentUnicodeTable &&
                ((WORD*) (_aAsciiMappingTable[0])) != _pwCurrentUnicodeTable)
            {
                _pwMappingTable = _pwCurrentUnicodeTable;
                for (__bTemp = 1; __bTemp < _pwMappingTable[0]; __bTemp++)
                {
                    if (_pwMappingTable[__bTemp * 2] == wOutputCode)
                    {
                        break;
                    }
                }
            }    // not Found

            bCharNum++; 
            if (__bTemp == _pwMappingTable[0])
                _aString[bBufferIndex] = CHAR_ASCII_SPACE; 
            else
                _aString[bBufferIndex] = _pwMappingTable[__bTemp * 2 + 1]; 
        }
        else 
        {
            if (bLineIndex < CHAR_SP_MAX_LINE_NUM)
            {
                _aLinePtr[bLineIndex] = _aString + bBufferIndex - bCharNum - 1;
                *(_aLinePtr[bLineIndex]) = bCharNum;
            }
            bLineIndex++;
            bCharNum = 0;
        }
        bBufferIndex++; 
        pString = ((PBYTE)_pSP_HeaderPt) + PARSER_SPTXT_HEADER_SIZE + wReadByteNum;
    }

    if (bLineIndex < CHAR_SP_MAX_LINE_NUM)
    {
        _aLinePtr[bLineIndex] = _aString + bBufferIndex - bCharNum - 1;
        *(_aLinePtr[bLineIndex]) = bCharNum;
    }*/
}
#endif

#ifdef SUPPORT_CHAR_ENCODING_BIG5
void _Convert_Big5_To_Unicode(void)
{
    WORD   wReadByteNum;
    WORD   wOutputCode;
    BYTE   bReadBuffer[2];
    BYTE   bCharNum, bLineIndex, bBufferIndex;
    PBYTE  pString;     // Point to the start address of the string
 
    // The string in buffer has been translated before
    /*
    if (_pSP_LastHeaderPt == _pSP_HeaderPt)
        return;
    */

    for (bLineIndex = 0; bLineIndex < CHAR_SP_MAX_LINE_NUM; bLineIndex++)
        _aLinePtr[bLineIndex] = NULL;

    pString = ((PBYTE)_pSP_HeaderPt) + PARSER_SPTXT_HEADER_SIZE;

    wReadByteNum = 0;
    bCharNum = 0;
    bBufferIndex = 1;

    bLineIndex = 0;
    _aLinePtr[bLineIndex] = _aString;

    // Translate Big5 -> Unicode
    while (wReadByteNum < _pSP_HeaderPt->bLen && bLineIndex < CHAR_SP_MAX_LINE_NUM) 
    {
        bReadBuffer[0] = *pString;  // Get the first byte

        if (0x80 > bReadBuffer[0])	// ASCII Code
        {
            wOutputCode = (WORD) bReadBuffer[0];
            wReadByteNum++;
        }
        else // Chinese character	 
        {
            if (((wReadByteNum+1) < _pSP_HeaderPt->bLen) && (*(pString+1) != 0x0D))
            {
                bReadBuffer[1] = *(pString+1);  // Get the second byte
                wOutputCode = ((WORD)(bReadBuffer[0])) << 8;
                wOutputCode |= (WORD) (bReadBuffer[1]);
                wReadByteNum += 2;		
            }
	      else // it's not two byte character, we can't read the next byte.
            {
                wOutputCode = (WORD) bReadBuffer[0];
                wReadByteNum++;
            }
        }

        if (0x0D != wOutputCode)
        {
            if (0x80 > wOutputCode)	// ASCII Code
            {
                bCharNum++;             
                _aString[bBufferIndex] = wOutputCode;            
            }            
            else	// Chinese character		 
            {
                bCharNum++;
                _aString[bBufferIndex] = CONVCHAR_To_Uni(wOutputCode);
            }
        }
        else 
        {
            if (bLineIndex < CHAR_SP_MAX_LINE_NUM)
            {
                _aLinePtr[bLineIndex] = _aString + bBufferIndex - bCharNum - 1;
                *(_aLinePtr[bLineIndex]) = bCharNum;
            }
            bLineIndex++;
            bCharNum = 0;
        }
        bBufferIndex++; 
        pString = ((PBYTE)_pSP_HeaderPt) + PARSER_SPTXT_HEADER_SIZE + wReadByteNum;
    }

    if (bLineIndex < CHAR_SP_MAX_LINE_NUM)
    {
        _aLinePtr[bLineIndex] = _aString + bBufferIndex - bCharNum - 1;
        *(_aLinePtr[bLineIndex]) = bCharNum;
    }
}
#endif

#ifdef SUPPORT_CHAR_ENCODING_SHIFT_JIS
void _Convert_Shift_JIS_To_Unicode()
{
    WORD   wReadByteNum;
    WORD   wOutputCode;
    BYTE   bReadBuffer[2];
    BYTE   bCharNum, bLineIndex, bBufferIndex;
    PBYTE  pString;     // Point to the start address of the string 

    // The string in buffer has been translated before
    /*
    if (_pSP_LastHeaderPt == _pSP_HeaderPt)
        return;
    */

    for (bLineIndex = 0; bLineIndex < CHAR_SP_MAX_LINE_NUM; bLineIndex++)
        _aLinePtr[bLineIndex] = NULL;

    pString = ((PBYTE)_pSP_HeaderPt) + PARSER_SPTXT_HEADER_SIZE;

    wReadByteNum = 0;
    bCharNum = 0;
    bBufferIndex = 1;

    bLineIndex = 0;
    _aLinePtr[bLineIndex] = _aString;

    // Translate Shift JIS -> Unicode
    while (wReadByteNum < _pSP_HeaderPt->bLen && bLineIndex < CHAR_SP_MAX_LINE_NUM) 
    {
        bReadBuffer[0] = *pString;  // Get the first byte

        if ((0x80 > bReadBuffer[0]) ||(0xA0 < bReadBuffer[0] && 0xE0 > bReadBuffer[0]))	// ASCII Code or JIS X 0201 Code
        {
            wOutputCode = (WORD) bReadBuffer[0];
            wReadByteNum++;
        }
        else
        {
            if (((wReadByteNum+1) < _pSP_HeaderPt->bLen) && (*(pString+1) != 0x0D))
            {
                bReadBuffer[1] = *(pString+1);  // Get the second byte
                wOutputCode = ((WORD)(bReadBuffer[0])) << 8;
                wOutputCode |= (WORD) (bReadBuffer[1]);
                wReadByteNum += 2;		
            }
	      else // it's not two byte character, we can't read the next byte.
            {
                wOutputCode = (WORD) bReadBuffer[0];
                wReadByteNum++;
            }
        }        

        if (0x0D != wOutputCode)
        {
            if (0x80 > wOutputCode)	// ASCII Code
            {
                bCharNum++;             
                _aString[bBufferIndex] = wOutputCode;   
                if (_aString[bBufferIndex] == 0x5C)    // Remapping the character 
                {
                    _aString[bBufferIndex] = _aString[bBufferIndex] + 0xA9;  
                }
            }
            else if (0xA0 < wOutputCode && 0xE0 > wOutputCode)	// JIS X 0201 Code
            {
                bCharNum++;             
                _aString[bBufferIndex] = 0;	
            }
            else	// JIS X 0208 Code		 
            {
                bCharNum++;
                _aString[bBufferIndex] = CONVCHAR_To_Uni(wOutputCode);
            }
        }
        else 
        {
            if (bLineIndex < CHAR_SP_MAX_LINE_NUM)
            {
                _aLinePtr[bLineIndex] = _aString + bBufferIndex - bCharNum - 1;
                *(_aLinePtr[bLineIndex]) = bCharNum;
            }
            bLineIndex++;
            bCharNum = 0;
        }
        bBufferIndex++; 
        pString = ((PBYTE)_pSP_HeaderPt) + PARSER_SPTXT_HEADER_SIZE + wReadByteNum;
    }

    if (bLineIndex < CHAR_SP_MAX_LINE_NUM)
    {
        _aLinePtr[bLineIndex] = _aString + bBufferIndex - bCharNum - 1;
        *(_aLinePtr[bLineIndex]) = bCharNum;
    }
}
#endif

#ifdef SUPPORT_UNICODE_CODING
void _Convert_UTF8_To_Unicode()
{
    WORD   wReadByteNum;
    WORD   wOutputCode = 0;
    BYTE   bReadBuffer[4];
    BYTE   bCharNum, bLineIndex, bBufferIndex;
    PBYTE  pString;     // Point to the start address of the string
    
    //The string in buffer has been translated before
    /*
    if (_pSP_LastHeaderPt == _pSP_HeaderPt)
        return;
    */

    for (bLineIndex = 0; bLineIndex < CHAR_SP_MAX_LINE_NUM; bLineIndex++)
        _aLinePtr[bLineIndex] = NULL;

    pString = ((PBYTE)_pSP_HeaderPt) + PARSER_SPTXT_HEADER_SIZE;

    wReadByteNum = 0;
    bCharNum = 0;
    bBufferIndex = 1;

    bLineIndex = 0;
    _aLinePtr[bLineIndex] = _aString;
    
    while (wReadByteNum < _pSP_HeaderPt->bLen && bLineIndex < CHAR_SP_MAX_LINE_NUM) 
    {
        bReadBuffer[0] = *pString;  // Get the first byte

        // Translate UTF8 -> Unicode
        if (0x80 > bReadBuffer[0])           // 0xxxxxxx
        {
            wOutputCode = (WORD) bReadBuffer[0];
            wReadByteNum++;
        }
        else if (0xE0> bReadBuffer[0])       // 110xxxxx 10xxxxxx
        {
            if (((wReadByteNum+1) < _pSP_HeaderPt->bLen) && (*(pString+1) != 0x0D))
            {
                bReadBuffer[1] = *(pString+1);  // Get the second byte
                wOutputCode = ((WORD)(bReadBuffer[0] & 0x1F)) << 6;
                wOutputCode |= (WORD) (bReadBuffer[1] & 0x3F);
                wReadByteNum += 2;		
            }
	      else // it's not two byte character, we can't read the next byte.
            {
                wOutputCode = (WORD) bReadBuffer[0];
                wReadByteNum++;
            }            
        }
        else if (0xF0 > bReadBuffer[0])      // 1110xxxx 10xxxxxx 10xxxxxx
        {
            if (((wReadByteNum+2) < _pSP_HeaderPt->bLen) && (*(pString+1) != 0x0D) && (*(pString+2) != 0x0D))
            {
                bReadBuffer[1] = *(pString+1);  // Get the second byte
                bReadBuffer[2] = *(pString+2);  // Get the third byte
                wOutputCode = ((WORD)(bReadBuffer[0] & 0x0F)) << 12;
                wOutputCode |= ((WORD)(bReadBuffer[1] & 0x3F)) << 6;
                wOutputCode |= (WORD)(bReadBuffer[2] & 0x3F);
                wReadByteNum += 3;	
            }
	      else // it's not three byte character, we can't read following byte.
            {
                wOutputCode = (WORD) bReadBuffer[0];
                wReadByteNum++;
            }                 
        }
        else if (0xF8 > bReadBuffer[0])      // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        {
            if (((wReadByteNum+3) < _pSP_HeaderPt->bLen) && (*(pString+1) != 0x0D) && (*(pString+2) != 0x0D) && (*(pString+3) != 0x0D))
            {
                bReadBuffer[1] = *(pString+1);  // Get the second byte
                bReadBuffer[2] = *(pString+2);  // Get the third byte
                bReadBuffer[3] = *(pString+3);  // Get the fourth byte
                wOutputCode = ((WORD)(bReadBuffer[0] & 0x07)) << 18;
                wOutputCode |= ((WORD)(bReadBuffer[1] & 0x3F)) << 12;
                wOutputCode |= ((WORD)(bReadBuffer[2] & 0x3F)) << 6;
                wOutputCode |= (WORD)(bReadBuffer[3] & 0x3F);
                wReadByteNum += 4;
            }
	      else // it's not four byte character, we can't read following byte.
            {
                wOutputCode = (WORD) bReadBuffer[0];
                wReadByteNum++;
            }                
        }
	  else // it's not UTF-8 character, we add protection here to avoid do nothing
        {
            wOutputCode = (WORD) bReadBuffer[0];
            wReadByteNum++;
        }   	
	 
        // Exchange HBTYE and LBYTE
        /*
        bTempCode = wOutputCode;        
        wOutputCode = wOutputCode >> 8;
        wOutputCode |=  bTempCode << 8;
        */

        if (0x0D != wOutputCode)
        {
            bCharNum++;             
            _aString[bBufferIndex] = wOutputCode;            
        }
        else 
        {
            if (bLineIndex < CHAR_SP_MAX_LINE_NUM)
            {
                _aLinePtr[bLineIndex] = _aString + bBufferIndex - bCharNum - 1;
                *(_aLinePtr[bLineIndex]) = bCharNum;
            }
            bLineIndex++;
            bCharNum = 0;
        }
        bBufferIndex++; 
        pString = ((PBYTE)_pSP_HeaderPt) + PARSER_SPTXT_HEADER_SIZE + wReadByteNum;
    }

    if (bLineIndex < CHAR_SP_MAX_LINE_NUM)
    {
        _aLinePtr[bLineIndex] = _aString + bBufferIndex - bCharNum - 1;
        *(_aLinePtr[bLineIndex]) = bCharNum;
    }
}
#endif //SUPPORT_UNICODE_CODING

#ifdef ISO_8859_8_FONT
void _Change_Word_Direction()
{
#ifdef HEBREW_IE_FORMAT
    BYTE CharIndex, SentenceIndex, NumberIndex;
    WORD SentenceBuffer[ONE_LINE_CHAR_NUMBER];
    WORD	WordBuffer[MAX_WORD_LENGTH];    
#elif defined(HEBREW_WORD_FORMAT)
    BYTE CharIndex, WordIndex;
    WORD WordBuffer[MAX_WORD_LENGTH];
#else
    WORD WordBuffer[ONE_LINE_CHAR_NUMBER];
    BYTE Index1, Index2;
#endif

#ifdef HEBREW_IE_FORMAT
    // change direction of number word
    for (NumberIndex = 0; NumberIndex < _bHebNumberIndex; NumberIndex += 2)
    {
        // save a number word to buffer
	  for (CharIndex = 0; CharIndex < _bHebNumberInfo[NumberIndex+1]; CharIndex++)
        {
            WordBuffer[CharIndex] = _aString[_bHebNumberInfo[NumberIndex]+CharIndex];
        }
        // change direction and return it to subtitle strings
        for (CharIndex = 0; CharIndex < _bHebNumberInfo[NumberIndex+1]; CharIndex++)
        {
            _aString[_bHebNumberInfo[NumberIndex]+CharIndex] = WordBuffer[_bHebNumberInfo[NumberIndex+1]-CharIndex-1];
        }
    }
    // change directioni of sentence
    for (SentenceIndex = 0; SentenceIndex < _bHebSentenceIndex; SentenceIndex += 2)
    {
        // save a hebrew sentence to buffer
	  for (CharIndex = 0; CharIndex < _bHebSentenceInfo[SentenceIndex+1]; CharIndex++)
        {
            SentenceBuffer[CharIndex] = _aString[_bHebSentenceInfo[SentenceIndex]+CharIndex];
        }
        // change direction and return it to subtitle strings
        for (CharIndex = 0; CharIndex < _bHebSentenceInfo[SentenceIndex+1]; CharIndex++)
        {
            _aString[_bHebSentenceInfo[SentenceIndex]+CharIndex] = SentenceBuffer[_bHebSentenceInfo[SentenceIndex+1]-CharIndex-1];
        }
    }    
#elif defined(HEBREW_WORD_FORMAT)
    for (WordIndex = 0; WordIndex < _bHebWordIndex; WordIndex += 2)
    {
        // save a hebrew word to buffer
	  for (CharIndex = 0; CharIndex < _bHebWordInfo[WordIndex+1]; CharIndex++)
        {
            WordBuffer[CharIndex] = _aString[_bHebWordInfo[WordIndex]+CharIndex];
        }
        // change direction and return it to subtitle strings
        for (CharIndex = 0; CharIndex < _bHebWordInfo[WordIndex+1]; CharIndex++)
        {
            _aString[_bHebWordInfo[WordIndex]+CharIndex] = WordBuffer[_bHebWordInfo[WordIndex+1]-CharIndex-1];
        }
    }
#else
    // inverse word
    Index1 = 0;
    while (_aLinePtr[Index1] && Index1 < CHAR_SP_MAX_LINE_NUM) 
    {
        for (Index2 = 0; Index2 < (BYTE)_aLinePtr[Index1][0]; Index2++)
        {           
            WordBuffer[Index2] = _aLinePtr[Index1][Index2+1];
        }
        // change direction and return it to subtitle strings
        for (Index2 = 0; Index2 < (BYTE)_aLinePtr[Index1][0]; Index2++)
        {
            _aLinePtr[Index1][Index2+1] = WordBuffer[_aLinePtr[Index1][0]-Index2-1];
        }       

        Index1++;
    }
#endif
}
#endif

BYTE _SP_Autowrap()
{
    BOOL   blAutowrap;
    BYTE   bCharNum = 0, bLineIndex, bCutCharNum;
#ifdef SUPPORT_UNICODE_CODING
    BYTE   bIndex, bTemp = 0;
#endif

    bLineIndex = 0;

    // The string in buffer has been translated before
    /*
    if (_pSP_LastHeaderPt == _pSP_HeaderPt)
    {
        for (__bTemp = 0; __bTemp < CHAR_SP_MAX_LINE_NUM; __bTemp++)
        {
            if (NULL != _aLinePtr[__bTemp])
                bLineIndex++;
        }
        return bLineIndex;
    }
    */

    if (__bEnCodeType == CHAR_ENCODE_ISO_CP)
    {
    // Check whether need to do the auto-wrap
    bLineIndex = 0;
    while (_aLinePtr[bLineIndex] && bLineIndex < CHAR_SP_MAX_LINE_NUM)
    {
        blAutowrap = FALSE;
        __dwTemp = GDI_GetStringWidth_909(_aLinePtr[bLineIndex]);
        bCutCharNum = 0;

        while (CHAR_SP_REG_WIDTH < __dwTemp)    // Need to cut the line
        {
            blAutowrap = TRUE;
            bCharNum = _aLinePtr[bLineIndex][0]; 
            while (CHAR_ASCII_SPACE != _aLinePtr[bLineIndex][bCharNum] && bCharNum > 0)
            {
                bCharNum--;
            }

            if (0 != bCharNum)
            {
                bCharNum--;    // Don't need the space in the end of this line
                bCutCharNum += _aLinePtr[bLineIndex][0] - bCharNum;
                _aLinePtr[bLineIndex][0] = bCharNum;
            }
            else
            {
                blAutowrap = FALSE;
                while (CHAR_SP_REG_WIDTH < GDI_GetStringWidth_909(_aLinePtr[bLineIndex]))
                {
                    _aLinePtr[bLineIndex][0]--;
                }
            }
            __dwTemp = GDI_GetStringWidth_909(_aLinePtr[bLineIndex]);
        }

        if (blAutowrap)
        {
            if (CHAR_SP_MAX_LINE_NUM - 1 > bLineIndex)
            {
                // bBufferIndex plus the char number of the next line
                if (NULL != _aLinePtr[bLineIndex+1])
                    bCutCharNum += _aLinePtr[bLineIndex+1][0];
                else
                    bCutCharNum--;  // Skip the first char, SPACE, in the line
                               
                _aLinePtr[bLineIndex+1][0] = CHAR_ASCII_SPACE;
                _aLinePtr[bLineIndex+1] = _aLinePtr[bLineIndex] + _aLinePtr[bLineIndex][0] + 1;
                _aLinePtr[bLineIndex+1][0] = bCutCharNum;
            } 
        }

        bLineIndex++;   // Next string
    }
    }
#ifdef SUPPORT_UNICODE_CODING
    else //CHAR_SP_CODE_UNICODE
    {
        // Check whether need to do the auto-wrap
    bLineIndex = 0;
        while (_aLinePtr[bLineIndex] && bLineIndex < CHAR_SP_MAX_LINE_NUM)
        {
            blAutowrap = FALSE;
            __dwTemp = GDI_GetUniStringWidth(_aLinePtr[bLineIndex]);
            bCutCharNum = 0;

            if (CHAR_SP_REG_WIDTH < __dwTemp)    // Need to cut the line
    {
                blAutowrap = TRUE;
                bCharNum = _aLinePtr[bLineIndex][0]; 
                while (CHAR_SP_REG_WIDTH < GDI_GetUniStringWidth(_aLinePtr[bLineIndex]))
                {
                    _aLinePtr[bLineIndex][0]--;
                }

                bCutCharNum = bCharNum - _aLinePtr[bLineIndex][0];
                bTemp = bCutCharNum;
                bCharNum = _aLinePtr[bLineIndex][0];                 
            }

            if (blAutowrap)
        {
                if (CHAR_SP_MAX_LINE_NUM - 1 > bLineIndex)
            {
                    // bBufferIndex plus the char number of the next line
                    if (NULL != _aLinePtr[bLineIndex+1])
            {
                        bCutCharNum += _aLinePtr[bLineIndex+1][0];
            }
				
		       // shift the exceeded character to next line 
                    for (bIndex = bTemp; bIndex > 0; bIndex--)
            {
                         _aLinePtr[bLineIndex][bCharNum+bIndex+1] = _aLinePtr[bLineIndex][bCharNum+bIndex];
            }
		   
                    _aLinePtr[bLineIndex+1] = _aLinePtr[bLineIndex] + _aLinePtr[bLineIndex][0] + 1;
                    _aLinePtr[bLineIndex+1][0] = bCutCharNum;
        }
    }

            bLineIndex++;   // Next string
        }
    }
#endif

    _pSP_LastHeaderPt = _pSP_HeaderPt;

    return bLineIndex;
}

void  _CHAR_SP_InitPalette(void)
{
    // If EX_SP_USE_SPU_BUFFER is define,
    // CHAR_SP_ENTRY_COLOR_FG index must be 2 and
    // CHAR_SP_ENTRY_COLOR_BG index must be 3.
    // Becuase in SPU, the foreground index of font is 2 and
    // the background index of font is 3
    // The colors index are shared with OSDND and dont need to init the color.
/* Alan2.30, don't need to init the colors, they are initialized by normal display.
    GDI_ChangePALEntry(CHAR_SP_ENTRY_COLOR_FG, CHAR_SP_VALUE_COLOR_FG, FALSE);
    GDI_ChangePALEntry(CHAR_SP_ENTRY_COLOR_BG, CHAR_SP_VALUE_COLOR_BG, TRUE);
*/
}

#ifdef EX_SP_USE_SPU_BUFFER
void _CHAR_SP_InitSPU(void)
{
            SPU_BMP_Init();

            SPU_BMP_SetDisplayArea(0, 51, CHAR_SP_REG_TOP, __RegionList[GDI_SP_REGION_ID].wWidth,
                __RegionList[GDI_SP_REGION_ID].wHeight, (const BYTE*)__RegionList[GDI_SP_REGION_ID].dwTAddr);

            SPU_BMP_SetColorContrast(0, 0, COMUTL_RGB2YUV(0x000000), 0);
            SPU_BMP_SetColorContrast(0, 1, COMUTL_RGB2YUV(0x000000), 0);
            SPU_BMP_SetColorContrast(0, 2, COMUTL_RGB2YUV(CHAR_SP_VALUE_COLOR_FG), 15);
            SPU_BMP_SetColorContrast(0, 3, COMUTL_RGB2YUV(CHAR_SP_VALUE_COLOR_BG), 8);

            GDI_ClearRegion(GDI_SP_REGION_ID);
}
#endif 

void CHAR_SP_ClearRegion(void)
{
#ifdef EX_SP_USE_SPU_BUFFER
    SPU_BMP_SetDisplay(0, FALSE);
    GDI_ClearRegion(GDI_SP_REGION_ID);
#else
    GDI_FillRect_909(0, &_CHAR_SP_RectAttr);
#endif
    _blAlreadyDisplay = FALSE;
}

#if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY)
void _CHAR_SP_UseExSPFontTable(BOOL bUseExSPFontTable)
{
/*
    if (bUseExSPFontTable)
    {
        switch (__SetupInfo.bExSPSTLanguage)
        {
#ifdef ISO_8859_15_FONT
        case SETUP_LANGUAGE_EX_SP_WESTERN_EUROPEAN:
            OSD_SetCurFontTable(FONTTABLE_8859_15);
            break;
#endif
#ifdef ISO_8859_2_FONT
        case SETUP_LANGUAGE_EX_SP_CENTRAL_EUROPEAN:
            OSD_SetCurFontTable(FONTTABLE_8859_2);
            break;
#endif
#ifdef ISO_8859_7_FONT
        case SETUP_LANGUAGE_EX_SP_GREEK:
            OSD_SetCurFontTable(FONTTABLE_8859_7);
            break;
#endif
// elmer2.51, add Damon's code for Hebrew
#ifdef ISO_8859_8_FONT
        case SETUP_LANGUAGE_EX_SP_HEBREW:
            OSD_SetCurFontTable(FONTTABLE_8859_8);
            break;
#endif
#ifdef ISO_8859_9_FONT
        case SETUP_LANGUAGE_EX_SP_TURKISH:
            OSD_SetCurFontTable(FONTTABLE_8859_9);
            break;
#endif
#ifdef CP_1250_FONT
        case SETUP_LANGUAGE_EX_SP_CP1250:
            OSD_SetCurFontTable(FONTTABLE_8859_2);
            break;
#endif
#ifdef CP_1251_FONT
        case SETUP_LANGUAGE_EX_SP_CYRILLIC:
            OSD_SetCurFontTable(FONTTABLE_CP_1251);
            break;
#endif
#ifdef SUPPORT_UNICODE_CODING
//        case SETUP_LANGUAGE_EX_SP_UNICODE:
//		OSD_SetCurFontTable(FONTABLE_UNICODE);
//		break;
#endif
#ifdef SUPPORT_CHINESE_SUBTITLE_BIG5
        case SETUP_LANGUAGE_EX_SP_BIG5:
		OSD_SetCurFontTable(FONTABLE_UNICODE);
		break;
#endif
#ifdef SUPPORT_JAPANESE_SUBTITLE_JIS
        case SETUP_LANGUAGE_EX_SP_SHIFT_JIS:
		OSD_SetCurFontTable(FONTABLE_UNICODE);
		break;
#endif
        case SETUP_LANGUAGE_EX_SP_ASCII:
        default:
            break;
        }
        //GDI_SetFontHeightInTable(_bFontHeightInTable); 
    }
    else
    {
        OSD_SetCurFontTable(FONTABLE_OSD);
        //GDI_SetFontHeightInTable(OSD_FONT_HEIGHT); 
    }
*/
}
#endif // #if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY)

#endif  // #ifdef SUPPORT_CHAR_SUBPICTURE




