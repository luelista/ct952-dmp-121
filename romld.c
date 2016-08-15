#include "winav.h"
#include "ctkav.h"
#ifndef AP_FRAMEWORK
#include "comdec.h"
#endif
#include "romld.h"
#include "utl.h"


//***************************************************************************
//  Function    :   ROMLD_GetSectionByName
//  Abstract    :   Get address of section structure in section table
//  Arguments   :   pSecTbl: Point to section table. ==0: Use default section table.
//                  pcName : The name of section, a string, must be 4 characters.
//  Return      :   !=0: Address of section structure
//                  ==0: The name not found in section table
//  Side Effect :
//  Notes       :
//***************************************************************************
PSECTION_ENTRY ROMLD_GetSectionByName(PSECTION_TABLE pSecTbl, const PCHAR pcName)
{
    if(pcName != 0 && pcName[4] == 0)    // string must be 4 characters
    {
        DWORD i, dwName;

        for(i = dwName = 0; i < 4; ++i)
        {
            char c = pcName[i];
            if(c >= 'a' && c <= 'z')
            {
                c -= 'a' - 'A'; // convert to uppercase
            }
            dwName = (dwName << 8) | c;
        }

        pSecTbl = (PSECTION_ENTRY)((DWORD)pSecTbl + (DWORD)ROMLD_SECTION_TABLE_ADDR);

        for(i = 0; i < ROMLD_SECTION_TABLE_SIZE && pSecTbl[i].dwName != 0; ++i)
        {
            if(pSecTbl[i].dwName == dwName)
            {
                return pSecTbl + i;
            }
        }
    }

    return 0;   // not found
}
// Micky2.17 begin
#ifndef AP_FRAMEWORK
//***************************************************************************
//  Function    :   ROMLD_LoadSectionTo
//  Abstract    :   Load section data
//  Arguments   :   pSec:   Address of section structure
//                  dwDest: !=0: Address of destination
//                          ==0: Load to pSec->dwLMA.
//                  dwZIP_WM: !=0: Address of Unzip Working Memory
//  Return      :   !=0: Address of loaded data
//                  ==0: Load data fail
//  Side Effect :
//  Notes       :
//***************************************************************************
DWORD ROMLD_LoadSectionTo(const PSECTION_ENTRY pSec, DWORD dwDest, DWORD dwZIP_WM)
{
    DWORD dwSrc, dwSize, dwFlag, dwCheckSum;

    if(pSec == 0)
    {
        return 0;   // load section fail
    }

    dwSrc = pSec->dwRMA;
    dwSize = pSec->dwRSize;

    if(dwSrc == 0 || dwSize == 0)
    {
        return 0;   // load section fail
    }

    if(dwDest == 0)
    {
        dwDest = pSec->dwLMA;
    }

    dwFlag = pSec->dwCheckSumFlag;

    if(dwFlag & ROMLD_SECTION_FLAG_ZIP)
    {
        if(dwZIP_WM == 0)
        {
            dwZIP_WM = (DWORD)DS_UNZIP_BUF_ST;
        }
        if(dwDest < 0x40000000 || UTL_Decompress((PBYTE)dwSrc, (PBYTE)dwDest, (PBYTE)dwZIP_WM) == -1)
        {
            return 0;
        }
    }
    else if(dwSrc != dwDest)
    {
        DWORD dwTemp = dwDest;
        dwSize >>= 2;   // byte size to DWORD size
        while(dwSize) // move memory
        {
            *((DWORD*)dwTemp) = *((DWORD*)dwSrc);
            dwTemp += 4;
            dwSrc += 4;
            --dwSize;
        }
    }

    dwSrc = dwDest;
    dwSize = (pSec->dwLSize + 3) >> 2;
    dwCheckSum = 0;
    while(dwSize)
    {
        DWORD dwTemp = *((DWORD*)dwSrc);
        dwCheckSum += dwTemp & 0xff;
        dwTemp >>= 8;
        dwCheckSum += dwTemp & 0xff;
        dwTemp >>= 8;
        dwCheckSum += dwTemp & 0xff;
        dwTemp >>= 8;
        dwCheckSum += dwTemp & 0xff;
        dwSrc += 4;
        --dwSize;
    }

    if((pSec->dwCheckSumFlag >> 16) != (dwCheckSum & 0xffff))
    {
        return 0;
    }

    return dwDest;  // ok! load section to dwDest.
}



//***************************************************************************
//  Function    :   ROMLD_MoveSectionTable
//  Abstract    :   Move section table
//  Arguments   :   pSrcSecTbl: Address of source section table. ==0: Use default section table.
//                  pDestSecTbl: Address of destination section table.
//  Return      :   !=0: Address of destination section table
//                  ==0: Move fail
//
//  Side Effect :
//  Notes       :
//***************************************************************************
DWORD ROMLD_MoveSectionTable(const PSECTION_TABLE pSrcSecTbl, PSECTION_TABLE pDestSecTbl)
{
    DWORD dwOffset, i;

    if((DWORD)pDestSecTbl < 0x40000000)
    {
        return 0;
    }

    if(pSrcSecTbl == 0)
    {
        pSrcSecTbl = ROMLD_SECTION_TABLE_ADDR;
    }

    dwOffset = (DWORD)pSrcSecTbl - (DWORD)pDestSecTbl;

    for(i = 0; i < ROMLD_SECTION_TABLE_SIZE; ++i)
    {
        memcpy(&pDestSecTbl[i], &pSrcSecTbl[i], sizeof(SECTION_ENTRY));
        pDestSecTbl[i].dwRMA += dwOffset;
    }

    return (DWORD)pDestSecTbl;
}
#endif  //#ifndef AP_FRAMEWORK
// Micky2.17 end
