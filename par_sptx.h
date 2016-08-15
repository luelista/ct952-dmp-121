//**************************************************************************
//		Copyright (c) 2005, Cheertek Inc . All rights reserved.
//		F100, all right reserved.

//	Product	: CheerDVD Firmware

//	Date	: 2005.07.15.
//	Author	: Cheertek (F100 KWShih)
//	Purpose	: Parse the text base subtitle file.
//	Sources	: par_sptx.h/ par_sptx.c
//***************************************************************************

#ifndef __PAR_SPTX_H__
#define __PAR_SPTX_H__

#ifdef WIN32_TOOL
#include "winav.h"
#endif // WIN32_TOOL

// For identify the end of subtitle.
#define PARSER_SPTXT_END	(0xFFFFFFFF)

// Return code
// It means that the function is success
#define PARSER_SPTXT_SUCCESS			(0x0001)
// It means that user pass a NULL read buffer pointer
#define PARSER_SPTXT_INVALID_RP			(0x0002)
// It means that user pass a NULL out buffer pointer at the first time
#define PARSER_SPTXT_INVALID_OP			(0x0004)
// It means that the subtitle format is not support
#define PARSER_SPTXT_INVALID_FORMAT		(0x0008)
// It means that the output buffer is full
#define PARSER_SPTXT_BUFFER_OVERFLOW	(0x0010)

// ============================= Size of PARSER_SPTXT_HEADER ==================
// If PARSER_SPTXT_HEADER structure has been modify, the value should be change
#define PARSER_SPTXT_HEADER_SIZE	(0x0A)

// ============================= Structure Define =============================
// It be used for contains the start, end PTS and subtitle string length
typedef struct tagPARSER_SPTXT_HEADER
{
	DWORD	dwStart;
	DWORD	dwEnd;
    BYTE	bID;
	BYTE	bLen;
} PARSER_SPTXT_HEADER, *PPARSER_SPTXT_HEADER;

// 
typedef struct tagPARSER_SPTXT_BUF
{
	DWORD	dwAddr;
	DWORD	dwLen;
}PARSER_SPTXT_BUF, *PPARSER_SPTXT_BUF;

// ============================= Function Prototype ===========================
extern DWORD PARSER_SPTXT(PPARSER_SPTXT_BUF pRead, PPARSER_SPTXT_BUF pOut, DWORD dwPTS, DWORD spType);
#ifndef WIN32_TOOL
    extern DWORD PARSER_SPTXT_ReadSector(void);
    extern DWORD PARSER_SPTXT_SPSector(void);
#endif // WIN32_TOOL
extern void PARSER_SPTXT_SetFilter(DWORD filter);
extern void PARSER_SPTXT_initial(void);

#endif // __PARSER_SPTXT_H__
