#include "winav.h"
#include "ctkav.h"
#include "OSD.h"
#include "input.h"
#include "cc.h"
#include "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include "setup.h"
#include "comdec.h"
#include "utl.h"
#ifdef SUPPORT_CHAR_SUBPICTURE
#include "char_subpict.h"
#endif

/**********************************************************************
ISO 8859-2 : Bosnian, Croatian, Czech, Hungarian, Polish, Romanian,
             Serbian (in Latin transcription), Serbocroatian, Slovak,
             Slovenian, Upper Sorbian and Lower Sorbian.

ISO 8859-7 : The modern Greek language.

ISO 8859-8 : Hebrew languages.

ISO 8859-9 : Turkic languages.

ISO 8859-15 : English, French, German, Italian, Dutch, Spanish, and Portuguese.

CP 1251 : Cyrillic language.
**********************************************************************/


#ifdef SUPPORT_FONT_TABLE_CHECKSUM
WORD    __wFontableChecksum;
WORD    OSD_FontableChecksum(void);

WORD    _wOSDFontTemp;
DWORD   _dwOSDFontTemp;
PBYTE   _pbOSDFontTemp;
#endif

BYTE    _OSD_LoadTableFromFlash(FONTABLE_ID bFontableID);

LONG    lUnzipResult;

DWORD *pdwBase;
DWORD *pdwTemp;

const BYTE _bFontable_OSD_2bit[]=
{
#include "OSDString/Fontable_2bit.txt"
};

#if (FONT_TABLE_METHOD != TRADITION)
#define OSD_FONT_TABLE_2BIT_SIZE          (OSD_2BIT_FONT_NUM * OSD_2BIT_CHAR_SIZE)
#define OSD_FONT_TABLE_1BIT_BASE_ADDRESS  (OSD_FONT_TABLE_BASE_ADDRESS + OSD_FONT_TABLE_2BIT_SIZE)
#define OSD_FONT_TABLE_1BIT_SIZE          (OSD_1BIT_FONT_NUM * OSD_1BIT_CHAR_SIZE)

const BYTE _bFontable_OSD_1bit[]=
{
#include "OSDString/Fontable_1bit.txt"
};

const BYTE * __pCurFontTable = (const BYTE *) _bFontable_OSD_1bit;

# else

const BYTE * __pCurFontTable = (const BYTE *) _bFontable_OSD_2bit;

#endif  // #if (FONT_TABLE_METHOD != TRADITION)

WORD * __wCurLocMapTab;     // Current ISO/CP character mapping table in used
WORD * __wCurUniMapTab;    // Current Unicode character mapping set in used
BYTE __bEnCodeType;
BYTE __bCurFontHeight;

#ifdef SUPPORT_UNICODE_CODING
#include "romld.h"

BYTE * _bFontable_Unicode;
WORD * __wUnicodeTable;
WORD * __wFontSizeTable;

/*
#ifdef SUPPORT_CHAR_ENCODING_BIG5
WORD * _wBig5MappingTable;
#endif

#ifdef SUPPORT_CHAR_ENCODING_SHIFT_JIS
WORD * _wJISMappingTable;
#endif
*/
#endif

/////////////////////////////////////////////////////////
// font table
#ifdef SUPPORT_STB
const BYTE _bFontable_TELETEXT_LATIN[]=
{
#include "stb/usrif/fontable_TELETEXT_LATIN.txt"
};
const BYTE _bFontable_TELETEXT_MOSAICS[]=
{
#include "stb/usrif/fontable_TELETEXT_MOSAICS.txt"
};
#endif

#ifdef ISO_8859_15_FONT
const BYTE _bFontable_8859_15[]=
{
#include "OSDString/ISO_8859_15_Fontable.txt"
};
#endif

#if defined(ISO_8859_2_FONT) || defined(CP_1250_FONT)
const BYTE _bFontable_8859_2[]=
{
#include "OSDString/ISO_8859_2_Fontable.txt"
};
#endif

#ifdef ISO_8859_7_FONT
const BYTE _bFontable_8859_7[]=
{
#include "OSDString/ISO_8859_7_Fontable.txt"
};
#endif

// Damon2.51, for Hebrew
#ifdef ISO_8859_8_FONT
const BYTE _bFontable_8859_8[]=
{
#include "OSDString/ISO_8859_8_Fontable.txt"
};
#endif

#ifdef ISO_8859_9_FONT
const BYTE _bFontable_8859_9[]=
{
#include "OSDString/ISO_8859_9_Fontable.txt"
};
#endif

#ifdef CP_1251_FONT
const BYTE _bFontable_CP_1251[]=
{
#include "OSDString/CP_1251_Fontable.txt"
};
#endif

/////////////////////////////////////////////////////////
// CTK string mapping table
#ifndef CUSTOMER_ASCII
#include "OSDString/Ascii_Remapping.txt"
#endif

WORD __aAsciiMappingTable[][2] = 
{
    #include "OSDString/Ascii_Mapping.txt"
};

#ifdef ISO_8859_15_FONT
WORD _a8859_15_MappingTable[][2] = 
{
    #include "OSDString/ISO_8859_15_Mapping.txt"
};
#endif

#ifdef ISO_8859_2_FONT
WORD _a8859_2_MappingTable[][2] = 
{
    #include "OSDString/ISO_8859_2_Mapping.txt"
};
#endif

#ifdef ISO_8859_7_FONT
WORD _a8859_7_MappingTable[][2] = 
{
    #include "OSDString/ISO_8859_7_Mapping.txt"
};
#endif

#ifdef ISO_8859_8_FONT
WORD _a8859_8_MappingTable[][2] = 
{
    #include "OSDString/ISO_8859_8_Mapping.txt"
};
#endif

#ifdef ISO_8859_9_FONT
WORD _a8859_9_MappingTable[][2] = 
{
    #include "OSDString/ISO_8859_9_Mapping.txt"
};
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
#endif
/////////////////////////////////////////////////////////
// Unicode to CTK string mapping table
#ifdef CP_1251_FONT
WORD _aCP1251UniMapTab[][2] = 
{
    #include "OSDString/CP_1251_Uni_Mapping.txt"
};
#endif
/////////////////////////////////////////////////////////

#ifdef SUPPORT_STB
const BYTE _bFontable_6937[]=
{
#include "OSDString/6937_Fontable.txt"
};
#endif


#ifdef DECOMPRESS_METHOD_GZIP
volatile UNZIP_SHRVAR _OSD_Unzipvar;
volatile UNZIP_SHRMAT unzipmat; //unzip shared matrix
#endif


// Brian1.25, always call this function to load font table
// ************************************************************************
//  Function    :   OSD_LoadFontTable
//  Description :       load font table
//  Arguments   :       none
//  Return      :       none
//  Side Effect :
// ************************************************************************
void OSD_LoadFontTable(FONTABLE_ID bFontableID)
{
#ifndef UNPROTECT_FONT_TABLE 
    //Alan1.00, Disable the font table protection
    MACRO_MCU_DISABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);
#endif

    switch (bFontableID)
    {
        case FONTABLE_OSD:
#if (FONT_TABLE_METHOD != CREATE_DYNAMICALLY)
#ifdef DECOMPRESS_METHOD_GZIP
            UNZIP_PackMatrixPtrs((PUNZIP_SHRMAT)&unzipmat);
            UNZIP_InitMemPool((LONG *)ADDR_UNZIPBUFFER, (PUNZIP_SHRVAR)&_OSD_Unzipvar);
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_OSD_2bit,
                                            sizeof(_bFontable_OSD_2bit)*sizeof(BYTE),
                                            (DWORD*) OSD_FONT_TABLE_BASE_ADDRESS,
                                            (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                                            (PUNZIP_SHRMAT) &unzipmat);
            if (0 != lUnzipResult) 
                printf("Fail to decompress the font table\n");
#else
            lUnzipResult = UTL_Decompress((PBYTE)_bFontable_OSD_2bit,
                           (PBYTE)OSD_FONT_TABLE_BASE_ADDRESS, (PBYTE)ADDR_UNZIPBUFFER);
            if (0 >= lUnzipResult) 
                printf("Fail to decompress the font table\n");
#endif

#if (FONT_TABLE_METHOD != TRADITION)
#ifdef DECOMPRESS_METHOD_GZIP
            UNZIP_PackMatrixPtrs((PUNZIP_SHRMAT)&unzipmat);
            UNZIP_InitMemPool((LONG *)ADDR_UNZIPBUFFER, (PUNZIP_SHRVAR)&_OSD_Unzipvar);
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_OSD_1bit,
                                            sizeof(_bFontable_OSD_1bit)*sizeof(BYTE),
                                            (DWORD*) OSD_FONT_TABLE_1BIT_BASE_ADDRESS,
                                            (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                                            (PUNZIP_SHRMAT) &unzipmat);
            if (0 != lUnzipResult) 
                printf("Fail to decompress the font table\n");
#else
                lUnzipResult = UTL_Decompress((PBYTE)_bFontable_OSD_1bit,
                               (PBYTE)OSD_FONT_TABLE_1BIT_BASE_ADDRESS, (PBYTE)ADDR_UNZIPBUFFER);
            if (0 >= lUnzipResult) 
                printf("Fail to decompress the font table\n");
#endif
#endif // #if(FONT_TABLE_METHOD != TRADITION) 
#endif  // if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY)

            GDI_SetFontHeightInTable(OSD_FONT_HEIGHT);
            GDI_SetFontNumberInTable(OSD_2BIT_FONT_NUM 
#if (FONT_TABLE_METHOD != TRADITION)
                                     + OSD_1BIT_FONT_NUM
#endif
                                    );
            break;

#ifdef SUPPORT_STB
        case FONTABLE_TELETEXT_LATIN:
#if (FONT_TABLE_METHOD == SPLIT_2_1_BIT)
#ifdef DECOMPRESS_METHOD_GZIP
            UNZIP_PackMatrixPtrs((PUNZIP_SHRMAT)&unzipmat);
            UNZIP_InitMemPool((LONG *)ADDR_UNZIPBUFFER, (PUNZIP_SHRVAR)&_OSD_Unzipvar);
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_TELETEXT_LATIN,
                                            sizeof(_bFontable_TELETEXT_LATIN)*sizeof(BYTE),
                                            (DWORD*) OSD_FONT_TABLE_BASE_ADDRESS,
                                            (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                                            (PUNZIP_SHRMAT) &unzipmat);
            if (0 != lUnzipResult) 
                printf("Fail to decompress the font table\n");
#else
            lUnzipResult = UTL_Decompress((PBYTE)_bFontable_TELETEXT_LATIN,
                           (BYTE *)OSD_FONT_TABLE_BASE_ADDRESS, (BYTE *)ADDR_UNZIPBUFFER);
            if (0 >= lUnzipResult) 
                printf("Fail to decompress the font table\n");
#endif
#endif  // #if (FONT_TABLE_METHOD == SPLIT_2_1_BIT)
            GDI_SetFontHeightInTable(TELETEXT_LATIN_FONT_HEIGHT);
            GDI_SetFontNumberInTable(TELETEXT_LATIN_FONT_NUM); 
            break;

        case FONTABLE_TELETEXT_MOSAICS:
#if (FONT_TABLE_METHOD == SPLIT_2_1_BIT)
#ifdef DECOMPRESS_METHOD_GZIP
            UNZIP_PackMatrixPtrs((PUNZIP_SHRMAT)&unzipmat);
            UNZIP_InitMemPool((LONG *)ADDR_UNZIPBUFFER, (PUNZIP_SHRVAR)&_OSD_Unzipvar);
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_TELETEXT_MOSAICS,
                                            sizeof(_bFontable_TELETEXT_MOSAICS)*sizeof(BYTE),
                                            (DWORD*)TELETEXT_MOSAICS_FONT_TABLE_BASE_ADDRESS,
                                            (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                                            (PUNZIP_SHRMAT) &unzipmat);
            if (0 != lUnzipResult) 
                printf("Fail to decompress the font table\n");
#else
                lUnzipResult = UTL_Decompress((PBYTE)_bFontable_TELETEXT_MOSAICS,
                               (BYTE *)TELETEXT_MOSAICS_FONT_TABLE_BASE_ADDRESS, (BYTE *)ADDR_UNZIPBUFFER);
            if (0 >= lUnzipResult) 
                printf("Fail to decompress the font table\n");
#endif
#endif  // #if (FONT_TABLE_METHOD == SPLIT_2_1_BIT)
            GDI_SetFontHeightInTable(TELETEXT_MOSAICS_FONT_HEIGHT);
            GDI_SetFontNumberInTable(TELETEXT_MOSAICS_FONT_NUM); 
            break;
#endif // SUPPORT_STB
        default :
            break;
    }

/*
#ifndef SUPPORT_CHAR_ENCODING_SWITCH
#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN
    OSD_AddFontToTable(FONTTABLE_8859_15);
#elif defined(SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN)
    OSD_AddFontToTable(FONTTABLE_8859_2);
#elif defined(SUPPORT_CHAR_ENCODING_GREEK)
    OSD_AddFontToTable(FONTTABLE_8859_7);
#elif defined(SUPPORT_CHAR_ENCODING_HEBREW)
    OSD_AddFontToTable(FONTTABLE_8859_8);
#elif defined(SUPPORT_CHAR_ENCODING_TURKISH)
    OSD_AddFontToTable(FONTTABLE_8859_9);
#elif defined(SUPPORT_CHAR_ENCODING_CP1250)
    OSD_AddFontToTable(FONTTABLE_CP_1250);
#elif defined(SUPPORT_CHAR_ENCODING_CYRILLIC)
    OSD_AddFontToTable(FONTTABLE_CP_1251);
#elif defined(SUPPORT_CHAR_ENCODING_CHINESE) || defined(SUPPORT_CHAR_ENCODING_JAPANESE)
    OSD_AddFontToTable(FONTABLE_UNICODE);
#else
    __wCurLocMapTab = (WORD*) (__aAsciiMappingTable[0]);
    __wCurUniMapTab = (WORD*) (__aAsciiMappingTable[0]);
    __bEnCodeType = CHAR_ENCODE_ISO_CP;
#endif
#endif //SUPPORT_CHAR_ENCODING_SWITCH
*/

#ifdef SUPPORT_FONT_TABLE_CHECKSUM
    __wFontableChecksum = OSD_FontableChecksum();
#endif

#ifndef UNPROTECT_FONT_TABLE
    //Alan1.00, Enable the font table protection
    MACRO_MCU_ENABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);
#endif
}

// Brian1.25, always call this function to load font table
// ************************************************************************
//  Function    :   OSD_FontableChecksum
//  Description :       do fontable checksum
//  Arguments   :       none
//  Return      :       checksum value
//  Side Effect :
// ************************************************************************
#ifdef SUPPORT_FONT_TABLE_CHECKSUM
WORD OSD_FontableChecksum(void)
{
    _wOSDFontTemp = 0;

    // The font table size is 32768 WORD.  256 char * 32*32*2/8 byte / 2 = 32768
    for(_dwOSDFontTemp = 0; _dwOSDFontTemp < 65536; _dwOSDFontTemp++)
    {
        _pbOSDFontTemp = (PBYTE)(OSD_FONT_TABLE_BASE_ADDRESS + _dwOSDFontTemp);
        _wOSDFontTemp += *_pbOSDFontTemp;
    }
    return _wOSDFontTemp;
}
#endif

// *******************************************************************
//  Function    :   OSD_ReplaceFont
//  Description :   Replace character set for external usage (dir/file name, external subtitle, EPG, ...) 
//  Arguments   :       none
//  Return      :       none
//  Side Effect :
// *******************************************************************
//#ifdef SUPPORT_CHAR_ENCODING_SWITCH
void OSD_ReplaceFont(void)
{
    switch (__SetupInfo.bExSPSTLanguage)
    {
#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN
    case SETUP_LANGUAGE_ENCODING_WESTERN_EUROPEAN:        
        OSD_AddFontToTable(FONTTABLE_8859_15);  
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN
    case SETUP_LANGUAGE_ENCODING_CENTRAL_EUROPEAN:        
        OSD_AddFontToTable(FONTTABLE_8859_2);
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_GREEK
    case SETUP_LANGUAGE_ENCODING_GREEK:        
        OSD_AddFontToTable(FONTTABLE_8859_7);
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_HEBREW
    case SETUP_LANGUAGE_ENCODING_HEBREW:        
        OSD_AddFontToTable(FONTTABLE_8859_8);
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_TURKISH
    case SETUP_LANGUAGE_ENCODING_TURKISH:        
        OSD_AddFontToTable(FONTTABLE_8859_9);
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_CP1250
    case SETUP_LANGUAGE_ENCODING_CP1250:        
        OSD_AddFontToTable(FONTTABLE_CP_1250);        
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_CYRILLIC
    case SETUP_LANGUAGE_ENCODING_CYRILLIC:        
        OSD_AddFontToTable(FONTTABLE_CP_1251);    
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_CHINESE
    case SETUP_LANGUAGE_ENCODING_TWN_UNICODE:       
        OSD_AddFontToTable(FONTABLE_UNICODE);
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_BIG5
    case SETUP_LANGUAGE_ENCODING_TWN_BIG5:       
        OSD_AddFontToTable(FONTABLE_BIG5);
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_JAPANESE
    case SETUP_LANGUAGE_ENCODING_JPN_UNICODE:        
        OSD_AddFontToTable(FONTABLE_UNICODE);
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_SHIFT_JIS
    case SETUP_LANGUAGE_ENCODING_JPN_SHIFT_JIS:        
        OSD_AddFontToTable(FONTABLE_SHIFT_JIS);
        break;
#endif
    case SETUP_LANGUAGE_ENCODING_ASCII:
    default:
        __wCurLocMapTab = (WORD*) (__aAsciiMappingTable[0]);
        __wCurUniMapTab = (WORD*) (__aAsciiMappingTable[0]);
        __bEnCodeType = CHAR_ENCODE_ISO_CP;
        break;
    }
}
//#endif

// *******************************************************************
//  Function    :   OSD_AddFontsToTable
//  Description :       add some fonts to the font table
//  Arguments   :       none
//  Return      :       none
//  Side Effect :
// *******************************************************************
void OSD_AddFontToTable(FONTABLE_ID bFontableID)
{
#if (FONT_TABLE_METHOD != CREATE_DYNAMICALLY)
    DWORD* pdwUnzipAddr = NULL;

    switch (bFontableID)
    {
#ifdef ISO_8859_15_FONT
        case FONTTABLE_8859_15:
            if (__pCurFontTable == _bFontable_8859_15)
                return;
            break;
#endif

#ifdef ISO_8859_2_FONT
        case FONTTABLE_8859_2:
            if (__pCurFontTable == _bFontable_8859_2)
                return;
            break;
#endif

#ifdef ISO_8859_7_FONT
        case FONTTABLE_8859_7:
            if (__pCurFontTable == _bFontable_8859_7)
                return;
            break;
#endif

// Damon2.51, for Hebrew
#ifdef ISO_8859_8_FONT
        case FONTTABLE_8859_8:
            if (__pCurFontTable == _bFontable_8859_8)
                return;
            break;
#endif

#ifdef ISO_8859_9_FONT
        case FONTTABLE_8859_9:
            if (__pCurFontTable == _bFontable_8859_9)
                return;
            break;
#endif
#ifdef CP_1250_FONT
        case FONTTABLE_CP_1250:
            if (__pCurFontTable == _bFontable_8859_2)
                return;
            break;
#endif
#ifdef CP_1251_FONT
        case FONTTABLE_CP_1251:
            if (__pCurFontTable == _bFontable_CP_1251)
                return;
            break;
#endif

#ifdef SUPPORT_STB
        case FONTTABLE_6937:
            if (__pCurFontTable == _bFontable_6937)
                return;
            break;
#endif // #ifdef SUPPORT_STB
/*
#ifdef SUPPORT_UNICODE_CODING
        case FONTABLE_UNICODE:            
            if (__pCurFontTable == _bFontable_Unicode)
                return;
            break;
#endif
*/
        default :
            break;
    }

#ifndef UNPROTECT_FONT_TABLE
    //Alan1.00, Disable the font table protection
    MACRO_MCU_DISABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);
#endif

#ifdef DECOMPRESS_METHOD_GZIP
    UNZIP_PackMatrixPtrs((PUNZIP_SHRMAT)&unzipmat);
    UNZIP_InitMemPool((LONG *)ADDR_UNZIPBUFFER, (PUNZIP_SHRVAR)&_OSD_Unzipvar);
#endif

#if (FONT_TABLE_METHOD == TRADITION)
    pdwUnzipAddr = (DWORD*) (OSD_FONT_TABLE_BASE_ADDRESS + OSD_2BIT_CHAR_SIZE * OSD_2BIT_FONT_NUM);
#elif (FONT_TABLE_METHOD == SPLIT_2_1_BIT)
    pdwUnzipAddr = (DWORD*) (OSD_FONT_TABLE_1BIT_BASE_ADDRESS + OSD_FONT_TABLE_1BIT_SIZE);
#endif

    switch (bFontableID)
    {
#ifdef ISO_8859_15_FONT
        case FONTTABLE_8859_15:
#ifdef DECOMPRESS_METHOD_GZIP
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_8859_15,
                sizeof(_bFontable_8859_15)*sizeof(BYTE), pdwUnzipAddr,
                (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                (PUNZIP_SHRMAT) &unzipmat);
#else
            lUnzipResult = UTL_Decompress((PBYTE) _bFontable_8859_15, (PBYTE)pdwUnzipAddr, (PBYTE)ADDR_UNZIPBUFFER);
#endif
            break;
#endif

#ifdef ISO_8859_2_FONT
        case FONTTABLE_8859_2:
#ifdef DECOMPRESS_METHOD_GZIP
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_8859_2,
                               sizeof(_bFontable_8859_2)*sizeof(BYTE), (DWORD*) pdwUnzipAddr,
                               (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                               (PUNZIP_SHRMAT) &unzipmat);
#else
            lUnzipResult = UTL_Decompress((PBYTE)_bFontable_8859_2, (PBYTE)pdwUnzipAddr, (PBYTE)ADDR_UNZIPBUFFER);
#endif
            break;
#endif

#ifdef ISO_8859_7_FONT
        case FONTTABLE_8859_7:
#ifdef DECOMPRESS_METHOD_GZIP
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_8859_7,
                               sizeof(_bFontable_8859_7)*sizeof(BYTE), (DWORD*) pdwUnzipAddr,
                               (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                               (PUNZIP_SHRMAT) &unzipmat);
#else
            lUnzipResult = UTL_Decompress((PBYTE)_bFontable_8859_7, (PBYTE)pdwUnzipAddr, (PBYTE)ADDR_UNZIPBUFFER);
#endif
            break;
#endif

// Damon2.51, for Hebrew
#ifdef ISO_8859_8_FONT
        case FONTTABLE_8859_8:
#ifdef DECOMPRESS_METHOD_GZIP
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_8859_8,
                               sizeof(_bFontable_8859_8)*sizeof(BYTE), (DWORD*) pdwUnzipAddr,
                               (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                               (PUNZIP_SHRMAT) &unzipmat);
#else
            lUnzipResult = UTL_Decompress((PBYTE)_bFontable_8859_8, (PBYTE)pdwUnzipAddr, (PBYTE)ADDR_UNZIPBUFFER);
#endif
            break;
#endif

#ifdef ISO_8859_9_FONT
        case FONTTABLE_8859_9:
#ifdef DECOMPRESS_METHOD_GZIP
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_8859_9,
                               sizeof(_bFontable_8859_9)*sizeof(BYTE), pdwUnzipAddr,
                               (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                               (PUNZIP_SHRMAT) &unzipmat);
#else
            lUnzipResult = UTL_Decompress((PBYTE)_bFontable_8859_9, (PBYTE)pdwUnzipAddr, (PBYTE)ADDR_UNZIPBUFFER);
#endif
            break;
#endif

#ifdef CP_1250_FONT
        case FONTTABLE_CP_1250:
#ifdef DECOMPRESS_METHOD_GZIP
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_8859_2,
                               sizeof(_bFontable_8859_2)*sizeof(BYTE), (DWORD*) pdwUnzipAddr,
                               (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                               (PUNZIP_SHRMAT) &unzipmat);
#else
            lUnzipResult = UTL_Decompress((PBYTE)_bFontable_8859_2, (PBYTE)pdwUnzipAddr, (PBYTE)ADDR_UNZIPBUFFER);
#endif
            break;
#endif

#ifdef CP_1251_FONT
        case FONTTABLE_CP_1251:
#ifdef DECOMPRESS_METHOD_GZIP
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_CP_1251,
                               sizeof(_bFontable_CP_1251)*sizeof(BYTE), pdwUnzipAddr,
                               (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                               (PUNZIP_SHRMAT) &unzipmat);
#else
            lUnzipResult = UTL_Decompress((PBYTE)_bFontable_CP_1251, (PBYTE)pdwUnzipAddr, (PBYTE)ADDR_UNZIPBUFFER);
#endif
            break;
#endif

#ifdef SUPPORT_STB
        case FONTTABLE_6937:
#ifdef DECOMPRESS_METHOD_GZIP
            lUnzipResult = UNZIP_Decompress((DWORD*)_bFontable_6937,
                               sizeof(_bFontable_6937)*sizeof(BYTE), pdwUnzipAddr,
                               (PUNZIP_SHRVAR)&_OSD_Unzipvar,
                               (PUNZIP_SHRMAT) &unzipmat);
#else
            lUnzipResult = UTL_Decompress((PBYTE)_bFontable_6937, (PBYTE)pdwUnzipAddr, (PBYTE)ADDR_UNZIPBUFFER);
#endif
            break;
#endif // #ifdef SUPPORT_STB

#ifdef SUPPORT_UNICODE_CODING
        case FONTABLE_UNICODE:
            break;
#endif
        default :
            break;
    }

#ifdef DECOMPRESS_METHOD_GZIP
    if (0 != lUnzipResult) 
        printf("Fail to decompress the font table\n");
#else
    if (0 >= lUnzipResult)
        printf("Fail to decompress the font table\n");
#endif


#ifdef SUPPORT_FONT_TABLE_CHECKSUM
    __wFontableChecksum = OSD_FontableChecksum();
#endif

#ifndef UNPROTECT_FONT_TABLE
    //Alan1.00, Enable the font table protection
    MACRO_MCU_ENABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);
#endif
#endif // #if (FONT_TABLE_METHOD != CREATE_DYNAMICALLY)
   OSD_SetCurFontTable(bFontableID);
}

// *******************************************************************
//  Function    :  OSD_SetCurFontTable 
//  Description :       Set the current used 1-bit font table 
//  Arguments   :       none
//  Return      :       none
//  Side Effect :
// *******************************************************************
void OSD_SetCurFontTable(FONTABLE_ID bFontableID)
{
    switch (bFontableID)
    {
#ifdef ISO_8859_2_FONT
    case FONTTABLE_8859_2: 
        __pCurFontTable = (BYTE *)_bFontable_8859_2;
        __wCurLocMapTab = (WORD*) (_a8859_2_MappingTable[0]);
        __wCurUniMapTab = (WORD*) (__aAsciiMappingTable[0]);
        __bCurFontHeight = ISO_8859_2_FONT_HEIGHT;
        __bEnCodeType = CHAR_ENCODE_ISO_CP;
        GDI_SetFontHeightInTable(ISO_8859_2_FONT_HEIGHT);
        GDI_SetFontTableAddress(OSD_FONT_TABLE_BASE_ADDRESS);
        break;
#endif
#ifdef ISO_8859_7_FONT
    case FONTTABLE_8859_7: 
        __pCurFontTable =  (BYTE *) _bFontable_8859_7;
        __wCurLocMapTab = (WORD*) (_a8859_7_MappingTable[0]);
        __wCurUniMapTab = (WORD*) (__aAsciiMappingTable[0]);
        __bCurFontHeight = ISO_8859_7_FONT_HEIGHT;
        __bEnCodeType = CHAR_ENCODE_ISO_CP;
        GDI_SetFontHeightInTable(ISO_8859_7_FONT_HEIGHT);
        GDI_SetFontTableAddress(OSD_FONT_TABLE_BASE_ADDRESS);
        break;
#endif
// Damon2.51, for Hebrew
#ifdef ISO_8859_8_FONT
    case FONTTABLE_8859_8: 
        __pCurFontTable =  (BYTE *) _bFontable_8859_8;
        __wCurLocMapTab = (WORD*) (_a8859_8_MappingTable[0]);
        __wCurUniMapTab = (WORD*) (__aAsciiMappingTable[0]);
        __bCurFontHeight = ISO_8859_8_FONT_HEIGHT;
        __bEnCodeType = CHAR_ENCODE_ISO_CP;
        GDI_SetFontHeightInTable(ISO_8859_8_FONT_HEIGHT);
        GDI_SetFontTableAddress(OSD_FONT_TABLE_BASE_ADDRESS);
        break;
#endif
#ifdef ISO_8859_9_FONT
    case FONTTABLE_8859_9: 
        __pCurFontTable =  (BYTE *) _bFontable_8859_9;
        __wCurLocMapTab = (WORD*) (_a8859_9_MappingTable[0]);
        __wCurUniMapTab = (WORD*) (__aAsciiMappingTable[0]);
        __bCurFontHeight = ISO_8859_9_FONT_HEIGHT;
        __bEnCodeType = CHAR_ENCODE_ISO_CP;
        GDI_SetFontHeightInTable(ISO_8859_9_FONT_HEIGHT);
        GDI_SetFontTableAddress(OSD_FONT_TABLE_BASE_ADDRESS);
        break;
#endif
#ifdef ISO_8859_15_FONT
    case FONTTABLE_8859_15: 
        __pCurFontTable =  (BYTE *) _bFontable_8859_15;
        __wCurLocMapTab = (WORD*) (_a8859_15_MappingTable[0]);
        __wCurUniMapTab = (WORD*) (__aAsciiMappingTable[0]);
        __bCurFontHeight = ISO_8859_15_FONT_HEIGHT;
        __bEnCodeType = CHAR_ENCODE_ISO_CP;
        GDI_SetFontHeightInTable(ISO_8859_15_FONT_HEIGHT);
        GDI_SetFontTableAddress(OSD_FONT_TABLE_BASE_ADDRESS);
        break;
#endif
#ifdef CP_1250_FONT 
    case FONTTABLE_CP_1250:
        __pCurFontTable =  (BYTE *) _bFontable_8859_2;
        __wCurLocMapTab = (WORD*) (_aCP_1250_MappingTable[0]);
        __wCurUniMapTab = (WORD*) (__aAsciiMappingTable[0]);
        __bCurFontHeight = ISO_8859_2_FONT_HEIGHT;
        __bEnCodeType = CHAR_ENCODE_ISO_CP;
        GDI_SetFontHeightInTable(ISO_8859_2_FONT_HEIGHT);
        GDI_SetFontTableAddress(OSD_FONT_TABLE_BASE_ADDRESS);
        break;
#endif
#ifdef CP_1251_FONT 
    case FONTTABLE_CP_1251:
        __pCurFontTable =  (BYTE *) _bFontable_CP_1251;
        __wCurLocMapTab = (WORD*) (_aCP_1251_MappingTable[0]);
        __wCurUniMapTab = (WORD*) (_aCP1251UniMapTab[0]);
        __bCurFontHeight = CP_1251_FONT_HEIGHT;
        __bEnCodeType = CHAR_ENCODE_ISO_CP;
        GDI_SetFontHeightInTable(CP_1251_FONT_HEIGHT);
        GDI_SetFontTableAddress(OSD_FONT_TABLE_BASE_ADDRESS);
        break;
#endif
#ifdef SUPPORT_STB 
    case FONTTABLE_6937:
        __pCurFontTable =  (BYTE *) _bFontable_6937;
        GDI_SetFontHeightInTable(ISO_6937_FONT_HEIGHT);
        break;
    case FONTABLE_TELETEXT_LATIN:
        __pCurFontTable =  (BYTE *) _bFontable_TELETEXT_LATIN;
        GDI_SetFontNumberInTable(TELETEXT_LATIN_FONT_NUM); 
        GDI_SetFontHeightInTable(TELETEXT_LATIN_FONT_HEIGHT);
        GDI_SetFontTableAddress(OSD_FONT_TABLE_BASE_ADDRESS);
        break;
    case FONTABLE_TELETEXT_MOSAICS:
        __pCurFontTable =  (BYTE *) _bFontable_TELETEXT_MOSAICS;
        GDI_SetFontNumberInTable(TELETEXT_MOSAICS_FONT_NUM); 
        GDI_SetFontHeightInTable(TELETEXT_MOSAICS_FONT_HEIGHT);
        GDI_SetFontTableAddress(TELETEXT_MOSAICS_FONT_TABLE_BASE_ADDRESS);
        break;
#endif

#ifdef SUPPORT_UNICODE_CODING
    case FONTABLE_UNICODE:
        _OSD_LoadTableFromFlash(FONTABLE_UNICODE);		
        __pCurFontTable =  (BYTE *) _bFontable_Unicode;
        __bCurFontHeight = UNICODE_FONT_HEIGHT;  
        __bEnCodeType = CHAR_ENCODE_UNICODE;
        GDI_SetFontHeightInTable(UNICODE_FONT_HEIGHT);
        //GDI_SetFontWidth(UNICODE_FONT_WIDTH);
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_BIG5
    case FONTABLE_BIG5:
        _OSD_LoadTableFromFlash(FONTABLE_BIG5);		
        __pCurFontTable =  (BYTE *) _bFontable_Unicode;          
        __bCurFontHeight = UNICODE_FONT_HEIGHT; 
        __bEnCodeType = CHAR_ENCODE_OTHER;
        GDI_SetFontHeightInTable(UNICODE_FONT_HEIGHT);
        //GDI_SetFontWidth(UNICODE_FONT_WIDTH);
        break;
#endif
#ifdef SUPPORT_CHAR_ENCODING_SHIFT_JIS
    case FONTABLE_SHIFT_JIS:
        _OSD_LoadTableFromFlash(FONTABLE_SHIFT_JIS);		
        __pCurFontTable =  (BYTE *) _bFontable_Unicode;         
        __bCurFontHeight = UNICODE_FONT_HEIGHT;  
        __bEnCodeType = CHAR_ENCODE_OTHER;
        GDI_SetFontHeightInTable(UNICODE_FONT_HEIGHT);
        //GDI_SetFontWidth(UNICODE_FONT_WIDTH);
        break;
#endif
    case FONTABLE_OSD: 
    default:
/*
#if (FONT_TABLE_METHOD != TRADITION)
        __pCurFontTable =  (BYTE *) _bFontable_OSD_1bit;
#endif 
*/
        GDI_SetFontHeightInTable(OSD_FONT_HEIGHT);
        GDI_SetFontTableAddress(OSD_FONT_TABLE_BASE_ADDRESS);
        break;
    }
}

// Damon, modify the procedure to support font table from flash directly.
// *******************************************************************
//  Function    :   _OSD_LoadTableFromFlash
//  Description :   Load Font Table from flash directly
//  Arguments  :   none
//  Return       :   TRUE, point to font table address is successful.
//                      FALSE, point to font table address is failed.
//  Side Effect :
// *******************************************************************
BYTE _OSD_LoadTableFromFlash(FONTABLE_ID bFontableID)
{
#ifdef SUPPORT_UNICODE_CODING 
    PSECTION_ENTRY pFonTab, pUniTab, pFonSize; 
#endif
#if defined(SUPPORT_CHAR_ENCODING_BIG5) || defined(SUPPORT_CHAR_ENCODING_SHIFT_JIS)
    PSECTION_ENTRY pMapTab;
#endif

#ifdef SUPPORT_UNICODE_CODING   
    // load unicode font table
    pFonTab = ROMLD_GetSectionByName(0, "FONT");

    if (!pFonTab)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find unicode font table\n");
        return (FALSE);
    }

    _bFontable_Unicode = (BYTE *) pFonTab->dwRMA;	

    // load unicode table
    pUniTab = ROMLD_GetSectionByName(0, "UNIC");

    if (!pUniTab)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find unicode table\n");
        return (FALSE);
    }

    __wUnicodeTable = (WORD *) pUniTab->dwRMA;	

     // load font size mapping table
    pFonSize = ROMLD_GetSectionByName(0, "SIZE");

    if (!pFonSize)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find size table\n");
        return (FALSE);
    }

    __wFontSizeTable = (WORD *) pFonSize->dwRMA;		
#endif //SUPPORT_UNICODE_CODING

    switch (bFontableID)
    {
#ifdef SUPPORT_CHAR_ENCODING_BIG5
    case FONTABLE_BIG5:
        // load Big5 mapping table
        pMapTab = ROMLD_GetSectionByName(0, "BIGT");
    
        if (!pMapTab)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find Big5 mapping table\n");
            return (FALSE);
        }
        __wCurUniMapTab = (WORD *) pMapTab->dwRMA;
        break;
#endif		
#ifdef SUPPORT_CHAR_ENCODING_SHIFT_JIS
    case FONTABLE_SHIFT_JIS:
        // load Shift JIS mapping table
        pMapTab = ROMLD_GetSectionByName(0, "JIST");
    
        if (!pMapTab)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Can't find JIS mapping table\n");
            return (FALSE);
        }    
        __wCurUniMapTab = (WORD *) pMapTab->dwRMA;
        break;
#endif				
    default:
        break;
    }
	
    return (TRUE);    
}

