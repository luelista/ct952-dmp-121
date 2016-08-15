//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2004,
//          D300, all right reserved.
//
//      Product : CT909
//
//      Date    : 2004.05.11
//      Author  : Cheertek (D300 WYC)
//      Purpose : Provide the register definition of ATAPI
//      Sources : 
//  **************************************************************************
#ifndef __ATAPI_H__
#define __ATAPI_H__

#ifdef __cplusplus
        extern  "C"     {
#endif
BYTE  ATAPI_Reset(BYTE bShowBar); //LJY1.07, pass parameter for status bar display
BYTE ATAPI_Init(void);       // initial servo status when change title

BYTE ATAPI_OpenTray( void );
BYTE ATAPI_CloseTray( void );
BYTE    ATAPI_Inquiry( void );
BYTE ATAPI_AudioDeemphasis(WORD wPlayItem);
// wyc0.50-909, rename it.
BYTE ATAPI_GetMediaStatus(void);
BYTE    ATAPI_PollingMediaStatus(void);
BYTE ATAPI_Stop(void);
BYTE ATAPI_ReadSectors( DWORD dwStartPos, DWORD dwEndPos );
BYTE ATAPI_GetCurrentPos( PDWORD pCurrentPos );
BYTE ATAPI_GetDiscInfo( PBYTE pMinTrack, PBYTE pMaxTrack, PDWORD pMaxPos );
BYTE ATAPI_GetTrackInfo( BYTE bTrackNum, PBYTE pTrackCtl, PDWORD pStartPos );
BYTE    ATAPI_ReadTOC( BYTE );
BYTE     ATAPI_Monitor( void );                  // WWH_CD7
BYTE ATAPI_CheckDiscEnding(void);
BYTE ATAPI_ReadDiscKey(void);
BYTE ATAPI_ReadTitleKey(DWORD dwSectorNumber, BYTE bStore);  // Replaced by Chern Shyh-Chain, 08-30-2001

BYTE ATAPI_CheckTrayStatus(void); //LJY1.25 
BOOL    ATAPI_EnableRunning ( BYTE bGo );    // ** DVD-TCH0.25;
BYTE    ATAPI_CSS_SentDiscKey(void);  // LLY.103e-XQ and LLY.160a-1, in SER_ATA.C module
BYTE ATAPI_Seek (DWORD dwSeekLBA, BYTE  bWaitBusy);
WORD    ATAPI_GetDiscType(void);       // WWH_CD7
void ATAPI_AdjustThreshold (BYTE bType, DWORD dwLevel) ;
BYTE ATAPI_ServoAbnormal( void );
BYTE ATAPI_GetTrackEnd (BYTE bTrackNum, PDWORD pLength); // ** TCH1.61a; 
BYTE ATAPI_SetTitleKey(BYTE bStore);     // Added by Chern Shyh-Chain, 08-30-2001
BYTE ATAPI_ResetRecover (BYTE bNeedStable);
BYTE ATAPI_GetDiscLength(PDWORD pLength);

BYTE ATAPI_Abort(void) ;
BYTE ATAPI_PowerMode (BYTE bMode) ;
BYTE ATAPI_GetStatus(void);
// wyc0.50-909, function to initial mutex
void    ATAPI_InitialMutexFlag(void);
void    ATAPI_ThreadDelete(void);

#ifdef __cplusplus
        }
#endif
#endif  // __ATAPI_H__
