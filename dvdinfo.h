
/****************************************************************************
    [ mscddef.h, mscd.h, mscd.cpp, mscd1.cpp ]
    Copywrite 1995 by TCH/L300, Cheertek, all right reserved

    Author  :   Chung-Hao   Tseng
    Date    :   1995. 1. 4
    Purpose :
    Notes   :
    Update  :
     Date        Name        Description
    ======    ==========    =================================================

******************************************************************************/

#ifndef     __DVDINFO_H__
#define     __DVDINFO_H__

#ifdef __cplusplus
   extern "C"{
#endif

// Micky07312006 begin, for TMAP
//#define	SUPPORT_TMAP_SEARCH

// DVD_172r2
#define USE_SHARE_SRAM

// Micky modifiation for WinDVD 1.73. begin
#define REDUCE_VMGM_cSECTOR
#define MOVE_TT_SRPT_DRAM
#define MOVE_PTT_SRPT_DRAM
#define MOVE_VTS_PGCIT_DRAM

// Micky1.11, 32M/64M DRAM reserve address are different
// reference __dwReserveStart
/// #define DRAM_TT_SRPT_START  W99AV_DRAM_ADDR_RESERVE_START
#define DRAM_TT_SRPT_START DS_FW_BUFFER_ST

// max. 99 titles/ per VTS.
#define SIZE_TT_SRPT_FULL   (( 8 + 12*99L ))  // 1196 bytes.
// Micky0.64-909, must align at DWORD boundary.
#define DRAM_PTT_SRPT_START ( DRAM_TT_SRPT_START + ((SIZE_TT_SRPT_FULL+3)/4)*4 )
// max. 99 ??
// Micky1.00, fix "The Perfect Storm"(Region 1) can't play over 6/99. 
// in dvdinfo.c VTSI_PTT_SRPs will limit max. 99.
// and the reserved VTSI_PTT_SRPT size(808) are not enough
// the There 2 TTUs within this VTS, max. has 274 PTT_SRPs.
// the reserved 808 will be OK till 6/198.
// must reserve max. PTT_SRPT.
// 8 + 99(TTUs)*4 + 999(PTT_SRPs) *4 = 4400
// HAL_DumpUserData must modify for over 1 sector.
#define SIZE_PTT_SRPT       4400    //808
#define DRAM_VTS_PGCIT_START    ( DRAM_PTT_SRPT_START + ((SIZE_PTT_SRPT+3)/4)*4 )
// Micky0.87a, fix AVIA-> menu-> select 1st item play will play 1/1 or stop.
// The wPGCN=198. but F/W only store 180. It will use unknow DRAM data.
//#define MAX_VTS_PGC_SRP     180   // 1448 bytes
// Micky1.25, run time read VTS_PGCIT
// always keep 1 PGCI sector
/*
#define MAX_VTS_PGC_SRP     440 //2008 bytes
#define SIZE_VTS_PGCIT      (8+ 8* (MAX_VTS_PGC_SRP) )
*/
///*
#define MAX_VTS_PGC_SRP     65535L
#define SIZE_VTS_PGCIT      2048L
//*/

// Micky1.07, reduce SRAM--_bVMG_PTL_LVLI 200 bytes.
#define DRAM_VMG_PTL_LVLI_START (DRAM_VTS_PGCIT_START + ((SIZE_VTS_PGCIT+3)/4)*4 )
#define SIZE_VMG_PTL_LVLI   200

// Micky07312006 begin, for TMAP
#ifdef	SUPPORT_TMAP_SEARCH
BYTE DVD_Load_VTSTMAP(WORD wPGCN);

#define	 DRAM_VTS_TMAP_START	(DRAM_VMG_PTL_LVLI_START + SIZE_VMG_PTL_LVLI/4)
#define	SIZE_VTS_TMAP	4096
#endif	//#ifdef	SUPPORT_TMAP_SEARCH


BYTE _DVD_LoadFromDRAM ( WORD nMode, WORD nIndex  );
extern DWORD   _dwDRAMDest;
extern  char    *_cCache ;//[];

/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////
////////    Begin of Testing Region     //////////////////////
//////////////////////////////////////////////////////////
// Below should be disable in normal state except "USE_SHARE_SRAM"
// TEST another(older) PGC_C_PBI ....
//#define     TEST_PGC_C_PBI

// TEST PGCI data that compare w/ < 0.43
//#define     TEST_COMPARE_PGCI

// Test VTSM_PGCI_UT < 0.49 (0.49 will use newer method)
//#define   TEST_COMPARE_VTSM_PGCI_UT

// Test VMG_PGCI_UT. (> 1.01a will use newer method)
//#define   TEST_COMPARE_VMGM_PGCI_UT

// TEST older _cVTS_PTT_SRPT_FULL method
// #define  TEST_COMPARE_VTS_PTT_SRPT

#ifdef    TEST_COMPARE_VTSM_PGCI_UT
#define     TEST_COMPARE_MODE
#endif  // TEST_COMPARE_VTSM_PGCI_UT

#ifdef    TEST_COMPARE_VMGM_PGCI_UT
#define     TEST_COMPARE_MODE
#endif  // TEST_COMPARE_VMGM_PGCI_UT

//////////////////////////////////////////////////////
////////    End of Testing Region     //////////////////////
//////////////////////////////////////////////////////////

// ** TCH1.71-1; Define it mean use new method to process
#define     ENHANCE_PGCI_MEM

//////////////////////////////////////////////////////////////
// *** Compatible with SYSTEM

// ** TCH0.29; begin...
extern  WORD                __wSPRMs [24];      // System Parameters
extern  WORD                __wGPRMs [16];      // General Parameters
// ** TCH0.29; end...


extern  BYTE                _bTotalPGN;                 // Record the totle Program # in PGC
extern  BYTE                _bTotalCell;                // Record the totle Cell # in PGC
extern  BYTE                _bVTSN;                // current VTSN

// DVD_172r2
#ifdef  USE_SHARE_SRAM
extern  WORD                *__wCELL_PBTM;      // ** TCH0.49; Don't declare the size, the size will depends on "MAX_CELLS_WITHIN_PGC".
#else
extern  WORD                __wCELL_PBTM [];      // ** TCH0.49; Don't declare the size, the size will depends on "MAX_CELLS_WITHIN_PGC".
#endif

extern  DWORD               _dwFirst_VOBS;        // Record the LBN of first VOBS (VMGM, VTSM, VTT ) // ** TCH0.39;

extern  DWORD               __dwPGC_UOP_CTL;    // ** TCH0.37; record UOP for PGC

// ** TCH1.01b; extern  char                _cVTS_PGCIT [2048];    // ** TCH0.39;
extern  char                _cVTS_PGCIT [];

// ** TCH0.42; begin...
extern  DWORD               __dwDVD_ATRT1;      // current ATRT1 (0-3 bytes)
extern  DWORD               __dwDVD_ATRT2;      // current ATRT2 (4-7 bytes)
// ** TCH0.42; end...

extern  BYTE                __bStatus_PTL;      // ** TCH0.431; record "is the final of PTL acceptable?"


#if  OS == OS_DOS    // ** TCH0.45;

#ifdef  _DEBUG
#ifndef     _TEST
#define     _TEST
#endif
#endif      // _DEBUG

// ** TCH1.72q; 

// Brian.276p, support _printf
/*
#ifndef	PRINTF 	// ** TCH1.72q; for ADS

#ifdef  _DEBUG
#define     PRINTF      printf
#else


#ifdef  _TEST
#define     PRINTF      printf
#else
#define     PRINTF      ; //printf
#endif      // _TEST

#endif  // _DEBUG

#endif	// PRINTF
*/


#endif

//////////////////////////////////////////////////////////////
//  definition
// DVD Information Part

// in VMGI_TT_SRPT
#define     OFFSET_TT_SRP               8
#define     BLOCK_TT_SRP                12

// in VTS_PTT_SRPT
//  #define     OFFSET_PTT_SRP              408

// Video Manager Information (VMGI)
// Micky1.21, reference VMGI VMG_CAT for RMA.
// VMG_CAT start from 34, byte 35 is RMA.
#define     VMGI_VMG_CAT                35
#define     VMGI_VTS_Ns                 63      // ** TCH0.40; Only one Byte valid
#define     VMGI_FP_PGCI_SA             132
#define     VMGI_VMGM_VOBS_SA           192
#define     VMGI_TT_SRPT_SA             196
#define     VMGI_VMGM_PGCI_UT_SA        200
#define     VMGI_PTL_MAIT_SA            204
#define     VMGI_VMGM_C_ADT_SA          216
#define     VMGI_VMGM_V_ATR             256     // ** TCH0.361;
// ** TCH0.42; begin...
#define     VMGI_VMGM_AST_Ns            258
#define     VMGI_VMGM_AST_ATR           260
#define     VMGI_VMGM_SPST_Ns           340
#define     VMGI_VMGM_SPST_ATR          342
// ** TCH0.42; end...

// Video Title Set Information (VTSI)
#define     VTSI_VTSM_VOBS_SA           192
#define     VTSI_VTSTT_VOBS_SA          196

#define     VTSI_VTS_PTT_SRPT_SA        200
#define     VTSI_VTS_PGCIT_SA           204
#define     VTSI_VTSM_PGCI_UT_SA        208
#define     VTSI_VTS_TMAPT_SA           212

// ** TCH0.361; begin...
#define     VTSI_VTSM_V_ATR             256
#define     VTSI_VTSM_AST_Ns            258
#define     VTSI_VTSM_AST_ATR           260
#define     VTSI_VTSM_SPST_Ns           340
#define     VTSI_VTSM_SPST_ATR          342
// ** TCH0.361; end...

// Micky2.80, reduce VTSI.
// move VTS_V_ATR~VTS_AST_ATRT(68 bytes) to 268.
// move VTS_SPST_Ns~VTS_SPST_ATRT 194 bytes to 348.
/*
#define     VTSI_VTS_V_ATR              512
#define     VTSI_VTS_AST_Ns             514
#define     VTSI_VTS_AST_ATRT           516
#define     VTSI_VTS_SPST_Ns            596
#define     VTSI_VTS_SPST_ATRT          598
*/
#define     VTSI_VTS_V_ATR              268
#define     VTSI_VTS_AST_Ns             270
#define     VTSI_VTS_AST_ATRT           272
#define     VTSI_VTS_SPST_Ns            348 // 348+192=540
#define     VTSI_VTS_SPST_ATRT          350

// ** TCH0.29; begin...
// Mask AND/OR for V_ATR
// 00b means 4:3, 11b means 16:9, 0x0C00.
#define     MASK_ASPECT_RATIO(a)        (((a)>>10 ) & 0x3)
#define     MASK_ASPECT_RATIO_4_3       0x00
#define     MASK_ASPECT_RATIO_16_9      0x03

// 00b means LB&PS, 01b PS, 10b LB, 11b Reserved, 0x0300
#define     MASK_DISPLAY_MODE(a)        (((a)>>8 ) & 0x3)
#define     MASK_DISPLAY_MODE_PS_LB     0x00
#define     MASK_DISPLAY_MODE_PS        0x01
#define     MASK_DISPLAY_MODE_LB        0x02

// ** TCH0.99e; begin...
// 000b means 720x480, 720x576.
// 001b means 704x480, 704x576.
// 010b means 352x480, 352x576.
// 011b means 352x240, 352x288.
// Others: reserved..
#define     MASK_SOURCE_RESOLUTION(a)   (((a)>>3 ) & 0x7)
#define     MASK_SOURCE_RESOLUTION_720_480      0x000
#define     MASK_SOURCE_RESOLUTION_704_480      0x001
#define     MASK_SOURCE_RESOLUTION_352_480      0x010
#define     MASK_SOURCE_RESOLUTION_352_240      0x011
// ** TCH0.99e; end...

// Program Chain Information (PGCI)
#define     PGCI_PGC_CNT                0
#define     PGCI_PGC_PB_TM              4
#define     PGCI_PGC_UOP_CTL            8
#define     PGCI_PGC_AST_CTLT           12
#define     PGCI_PGC_SPST_CTLT          28
#define     PGCI_PGC_NV_CTL             156
#define     PGCI_PGC_SP_PLT             164
#define     PGCI_PGC_CMDT_SA            228
#define     PGCI_PGC_PGMAP_SA           230
#define     PGCI_C_PBIT_SA              232
#define     PGCI_C_POSIT_SA             234

///////////////////////////////////////////////////////////
// Follow is definition for ParserXXXX APIs.
// ParseInfo definitions (Parse VMGI)
#define     MSG_DVD_BASE                        1
#define     PARSE_INDEX_BY_DWORD        0xF000
#define     PARSE_INDEX_BY_WORD         0xF100
#define     PARSE_INDEX_BY_BYTE         0xF200
#define     PARSE_INDEX_BY_PTR          0xF400   // ** TCH0.27;

#define     DVD_TOTAL_TITLES            MSG_DVD_BASE+ 1         // Total Titles
#define     DVD_TOTAL_CHAPTERS          MSG_DVD_BASE+ 2         // Total chapter in one title

#define     DVD_TT_PB_TY                MSG_DVD_BASE+  9        // ** TCH0.99;
#define     DVD_VTSN_LOCATE             MSG_DVD_BASE+ 10        //
#define     DVD_VTS_TTN_LOCATE          MSG_DVD_BASE+ 11        //
#define     DVD_VTS_AST_ATRT            MSG_DVD_BASE+ 12        // ** TCH0.26;
#define     DVD_VTS_SPST_ATRT           MSG_DVD_BASE+ 13        // ** TCH0.38;
#define     DVD_SRP_NO                  MSG_DVD_BASE+ 15        // ** TCH0.31; Don't use it from 0.39. (WRONG!!)
#define     DVD_SEARCH_TTN              MSG_DVD_BASE+ 16        // ** TCH0.39;
#define     DVD_PTL_ID_FLD              MSG_DVD_BASE+ 17        // ** TCH0.39;
#define     DVD_TT_SRP_AGL_NS           MSG_DVD_BASE+ 18    // ** TCH1.60a-1;  

#define     DVD_TTM_EXIST               MSG_DVD_BASE+ 20        //

#define     DVD_TT_PTL_ID               MSG_DVD_BASE+ 21        // ** TCH0.40; Read TT_PLT_ID

// ParseVTSI definitions
#define     VTSI_PGCN_LOCATE            MSG_DVD_BASE+ 30        //
#define     VTSI_PGN_LOCATE             MSG_DVD_BASE+ 31        //
#define     VTSI_TTU_Ns                 MSG_DVD_BASE+ 32        //
#define     VTSI_PTT_SRPs               MSG_DVD_BASE+ 33        //
#define     VTSI_PGC_CAT                MSG_DVD_BASE+ 34        // ** TCH0.40;
#define     VTSI_PGCI_SRPs              MSG_DVD_BASE+ 35        // number of VTS_PGCI_SRPs.  

// ParsePGCI definitions
#define     PGCI_TOTAL_PGN              MSG_DVD_BASE+ 60        //
#define     PGCI_TOTAL_CEN              MSG_DVD_BASE+ 61        //
#define     PGCI_PGMAP_PGN_LOCATE       MSG_DVD_BASE+ 62        //

#define     PGCI_PGMAP_CEN              MSG_DVD_BASE+ 65        //

#define     PGCI_CMDT_PRE_CMD           MSG_DVD_BASE+ 66        //
#define     PGCI_CMDT_POST_CMD          MSG_DVD_BASE+ 67        //
#define     PGCI_CMDT_C_CMD             MSG_DVD_BASE+ 68        //

#define     PGCI_NV_CTL                 MSG_DVD_BASE+ 69        //
// It's Submode of PGCI_NV_CTL
#define     PGCI_NV_NEXT_PGCN           0
#define     PGCI_NV_PREV_PGCN           2
#define     PGCI_NV_GOUP_PGCN           4
#define     PGCI_NV_PB_MODE             6
#define     PGCI_NV_STILLTIME           7

#define     PGCI_C_PBI                  MSG_DVD_BASE+ 78        //
///#define     PGCI_C_CAT                  MSG_DVD_BASE+ 79        //
#define     PGCI_C_PBTM                 MSG_DVD_BASE+ 80        //
#define     PGCI_C_FVOBU_SA             MSG_DVD_BASE+ 81        //
#define     PGCI_C_LVOBU_SA             MSG_DVD_BASE+ 82        //
#define     PGCI_C_LVOBU_EA             MSG_DVD_BASE+ 83        //

//////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Follow is definition for Load2Cache APIs.

#define     VTSM_PGCI_UTI               MSG_DVD_BASE+ 1
#define     VTSM_LU_SRP                 MSG_DVD_BASE+ 2
#define     VTSM_LUI                    MSG_DVD_BASE+ 3
#define     VTSM_PGCI_SRP               MSG_DVD_BASE+ 4

// DVD_172r_REDUCE_SRAM, add comments
#define     VMGM_PGCI_UTI               MSG_DVD_BASE+ 10
#define     VMGM_LU_SRP                 MSG_DVD_BASE+ 11
#define     VMGM_LUI                    MSG_DVD_BASE+ 12
#define     VMGM_PGCI_SRP               MSG_DVD_BASE+ 13
#define     VMGM_PTL_SRP                MSG_DVD_BASE+ 14
#define     VMGM_TT_SRPTI               MSG_DVD_BASE+ 20    
#define     VMGM_TT_SRP                 MSG_DVD_BASE+ 21
#define     VTS_PTT_SRPTI               MSG_DVD_BASE+ 22    
#define     VTS_TTU_SA                  MSG_DVD_BASE+ 23    
#define     VTS_PTT_SRP                 MSG_DVD_BASE+ 24    
#define     VTS_PGCITI                  MSG_DVD_BASE+ 25
#define     VTS_PGCI_SRP                MSG_DVD_BASE+ 26
// Micky1.07, reduce SRAM--_bVMG_PTL_LVLI 200 bytes.
#define     VMG_PTL_ID_FLD              MSG_DVD_BASE+ 27

// Micky07312006 begin, for TMAP
#ifdef	SUPPORT_TMAP_SEARCH
#define		VTS_TMAP_SRP				MSG_DVD_BASE+ 28
#define		VTS_TMAP					MSG_DVD_BASE+ 29
#define	MAP_ENT							MSG_DVD_BASE+ 30
#endif	//#ifdef	SUPPORT_TMAP_SEARCH

//////////////////////////////////////////////////////////////

//      Structure Definition
// Structure for PGC_CMDT
typedef struct tagPGC_CMDT
{
    BYTE    PRE_CMD_Ns [2];
    BYTE    POST_CMD_Ns [2];
    BYTE    C_CMD_Ns [2];
    BYTE    PGC_CMDT_EA [2];
} PGC_CMDT, * PPGC_CMDT;

typedef struct tagPGC_GI
{
    BYTE    PGC_CNT [4];
    BYTE    PGC_PB_TM [4];
    BYTE    PGC_UOP_CTL [4];
    BYTE    PGC_AST_CTLT [16];
    BYTE    PGC_SPST_CTLT [128];
    BYTE    PGC_NV_CTL [8];
    BYTE    PGC_SP_PLT [64];
    BYTE    PGC_CMDA_SA [2];
    BYTE    PGC_PGMAP_SA [2];
    BYTE    PGC_PBIT_SA [2];
    BYTE    PGC_POSIT_SA [2];
} PGC_GI, * PPGC_GI;

typedef enum
{
    SUBNAME_IFO,
    SUBNAME_BUP
} EN_INFO_SUBNAME;

//////////////////////////////////////////////////////////////
//      dvd.cpp [OS issue]

////////////////////////////////////////////////////////////////////
// DVD part
// DVD Information Part
BOOL    DVD_Initial (void);
BOOL    DVD_CheckFirstSector (EN_INFO_SUBNAME enSubName);
BOOL    DVD_CheckPTL_MAIT (void);

BOOL    DVD_ParseInfo ( WORD nMode, DWORD *pdwVal, WORD nSubMode );
BOOL    DVD_ParseVTSI ( WORD nMode, DWORD *pdwVal, WORD nSubMode );
BOOL    DVD_ParsePGCI ( WORD nMode, DWORD *pdwVal, WORD nSubMode );

BOOL    DVD_LoadVTS ( BYTE bVTS );
BOOL    DVD_LoadVTSI_PTT_SRPT ( BYTE bTitle, WORD wPTT );

BOOL    DVD_LoadFPPGC (void);	// ** TCH1.72q; 
BOOL    DVD_LoadVMGMPGC ( WORD wPGCN ); // ** TCH0.31;

#define VTSM_PGC_JUMP                   0x8000

BOOL    DVD_LoadVTSMPGC ( WORD wType ); //
BOOL    DVD_LoadTTPGC ( WORD wPGCN );

BOOL    DVD_GetATRT ( BYTE bKind, BYTE bNO );       // ** TCH0.42;

WORD DVD_GetLangCD (BYTE bSetItem, BYTE bNO);  // Aron1.02-909, added

extern DWORD DVD_InfoBufSet(DWORD dwStartAddr, DWORD dwBufSize); //Aron2.17-909R,added

//Aron1.06-909, Support Smart Play feature
WORD _DVD_GetTT_PBTime(BYTE bTitle); // Get Title play back time
BOOL  _DVD_CheckSMRTPB ( void ); 

/////////////////////////////////////////
// Protected function

int     _CD_ReadSector ( DWORD dwSector, WORD nOffset, void * pBuffer );

// Read/Write Data
DWORD   _ReadBufferDWORD ( char * pStr, int wOffset );
WORD    _ReadBufferWORD ( char * pStr, int wOffset );
BYTE    _ReadBufferBYTE ( char * pStr, int wOffset );


// ** TCH1.55; BOOL    _WriteBufferDWORD ( char * pStr, int wOffset, DWORD dwVal );
// ** TCH1.55; BOOL    _WriteBufferWORD ( char * pStr, int wOffset, WORD wVal );

// DEBUG Usage
#ifdef SHOW_VMGI_INFO
void    _ShowVMGIInformation (void);	// ** TCH1.72q; 
#endif
#ifdef SHOW_VTSI_INFO
void    _ShowVTSIInformation (void);
#endif
#ifdef SHOW_PGCI_INFO
void    _ShowPGCIInformation (void);
#endif
void    _ShowCELLInformation (void);
#ifdef SHOW_NAV_INFO
void    _ShowInformation (BYTE bChoice);
#endif
///////////////////

void    _Backup_GPRM_SPRM ( BYTE bDirection );

//Aron1.15-909, not be used, comment it!
//void    _GetHMS ( char * pS );
BYTE    _TimeToDecimal ( BYTE bHex );

void    _PreparePGCIData (void);		// ** TCH1.72q; 

//////// Process PTL issues ///////////////////////////
BYTE    _DVD_PassPTL_ID_FLD ( BYTE bDomain, WORD wPTL_ID );
BYTE    _DVD_ConfirmPTL (void); // ** TCH2.74; 

//////////////////////////////////////////////////////
BYTE    _DVD_StorePGCI ( DWORD dwSector, WORD wOffset );
BYTE    _DVD_StoreBuffer ( DWORD dwSector, WORD wOffset, WORD wReadSize );  // ** TCH1.01b;

BYTE    _DVD_Load2Cache ( WORD nMode, WORD nIndex ); // ** TCH0.49;


///////////////////////////////////////
// ** TCH0.40; begin...  Need to declare after PPGC_GI declaration
extern  char    *           __pPGC_CMDT;
extern  PPGC_GI             __pPGC_GI;
// ** TCH0.40; end...

#ifdef __cplusplus
   }
#endif

#endif      // __DVDINFO_H__
