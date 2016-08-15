#ifndef __OSDFONT_H__
#define __OSDFONT_H__
#ifdef __cplusplus
   extern "C"{
#endif

#include "gdi.h"

typedef enum tagFONTABLE
{
    FONTABLE_OSD = 0,   // Alan.STB, for STB OSD. It include ISO 8859-15, 8859-9
#ifdef SUPPORT_STB      // Alan2.00
    FONTABLE_TELETEXT_LATIN,  // Alan.STB, STB teletext for latin G0, G2 
    FONTABLE_TELETEXT_MOSAICS,   // Alan.STB, STB teletext for G1 and G3 block mosaics
    FONTTABLE_6937,     // Alan.STB , STB ISO 6937. It will append after 2-bit and 1-bit OSD font
#endif 
    FONTABLE_UNICODE,
    FONTABLE_BIG5,
    FONTABLE_SHIFT_JIS,
    FONTTABLE_8859_2,
    FONTTABLE_8859_7,
    // Damon2.51, for Hebrew
    FONTTABLE_8859_8,
    FONTTABLE_8859_9,
    FONTTABLE_8859_15,
    FONTTABLE_CP_1250,
    FONTTABLE_CP_1251,
    FONTTABLE_END = FONTTABLE_CP_1251,
} FONTABLE_ID;


#ifdef OSD_1BIT_FONT_NUM
#define OSD_FONT_NUM OSD_1BIT_FONT_NUM
#else
#define OSD_FONT_NUM OSD_2BIT_FONT_NUM
#endif


// Chuan0.85, Use the definition of CTKAV.H
#define  ADDR_UNZIPBUFFER    (DS_UNZIP_BUF_ST)    // (OSD_FONT_TABLE_BASE_ADDRESS + 0x00010000) 

#ifdef SUPPORT_STB
#define  TELETEXT_MOSAICS_FONT_TABLE_BASE_ADDRESS  (DS_FW_BUFFER_ST)
#endif

#define CHAR_ENCODE_ISO_CP		0
#define CHAR_ENCODE_UNICODE		1
#define CHAR_ENCODE_OTHER			2


void    OSD_LoadFontTable(FONTABLE_ID bFontableID);
WORD    OSD_FontableChecksum(void);
void    OSD_ReplaceFont(void);
void    OSD_AddFontToTable(FONTABLE_ID bFontableID);
void    OSD_SetCurFontTable(FONTABLE_ID bFontableID);

extern const BYTE *__pCurFontTable;
extern const BYTE _bFontable_OSD_2bit[];
#if (FONT_TABLE_METHOD != TRADITION)
extern const BYTE _bFontable_OSD_1bit[];
#endif

extern WORD __aAsciiMappingTable[][2];

extern WORD * __wCurLocMapTab;
extern WORD * __wCurUniMapTab;
extern BYTE __bEnCodeType;
extern BYTE __bCurFontHeight;

#ifdef SUPPORT_UNICODE_CODING
extern WORD * __wUnicodeTable;
extern WORD * __wFontSizeTable;
#endif

#ifdef __cplusplus
   }
#endif  // #ifdef __cplusplus
#endif  //__OSDFONT_H__
