#ifndef __ROMLD__
#define __ROMLD__

#ifdef __cplusplus
   extern "C"{
#endif


#define ROMLD_SECTION_TABLE_ADDR  ((PSECTION_ENTRY)0x10)
#define ROMLD_SECTION_TABLE_SIZE  32  // number of section entries


typedef struct tagSECTION_ENTRY
{
    DWORD dwName;           // Name (4 characters)
    DWORD dwLMA;            // Logical Memory Address
    DWORD dwRMA;            // ROM Memory Address
    DWORD dwLSize;          // Size of Section on LMA
    DWORD dwRSize;          // Size of Section on RMA
    DWORD dwCheckSumFlag;   // Bit0: 1=Load
                            // Bit1: 1=Program Entry
                            // Bit2: 1=Zipped
                            // Bit3-15: Reserved
                            // Bit16-31: CheckSum
} SECTION_ENTRY, *PSECTION_ENTRY;

typedef SECTION_ENTRY PSECTION_TABLE[ROMLD_SECTION_TABLE_SIZE];

#define ROMLD_SECTION_FLAG_LOAD         1
#define ROMLD_SECTION_FLAG_PROGENTRY    2
#define ROMLD_SECTION_FLAG_ZIP          4

// ROM Loader API
PSECTION_ENTRY  ROMLD_GetSectionByName(PSECTION_TABLE pSecTbl, const PCHAR pcName);
#define         ROMLD_GetSectionByIndex(pSecTbl, Index) ((Index) >= 0 && (Index) < ROMLD_SECTION_TABLE_SIZE ? ((pSecTbl) == 0 ? ROMLD_SECTION_TABLE_ADDR + (Index)) : 0)

DWORD           ROMLD_LoadSectionTo(const PSECTION_ENTRY pSec, DWORD dwDest, DWORD dwZIP_WM);
#define         ROMLD_LoadSectionToByName(pSecTbl, pcName, dwDest, dwZIP_WM) (ROMLD_LoadSectionTo(ROMLD_GetSectionByName((pSecTbl), (pcName)), (dwDest), (dwZIP_WM)))
#define         ROMLD_LoadSection(pSec, dwZIP_WM) (ROMLD_LoadSectionTo(pSec, 0, dwZIP_WM))
#define         ROMLD_LoadSectionByName(pSecTbl, pcName, dwZIP_WM) (ROMLD_LoadSectionTo(GetSectionByName((pSecTbl), (pcName)), 0, (dwZIP_WM)))

DWORD           ROMLD_MoveSectionTable(const PSECTION_TABLE pSrcSecTbl, PSECTION_TABLE pDestSecTbl);

typedef void    (**PBOOT_LoadSectionAndRun)(PSECTION_TABLE pSecTbl, DWORD pUnzipWorkMem, DWORD dwSP);
#define         ROMLD_BOOT_LoadSectionAndRun   (**((PBOOT_LoadSectionAndRun) (0xC)))

#ifdef __cplusplus
   }
#endif

#endif  // __ROMLD__
