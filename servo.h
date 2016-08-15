/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved

        Product : WinDVD Firmware

        Date    : 1999.8.26
        Purpose : Provide all SERVO information
        Sources :

****************************************************************************/

#ifndef __SERVO_H__
#define __SERVO_H__

#ifdef __cplusplus
        extern  "C"     {
#endif

#define SERVO_NEW_CMD           0x00000001
#define SERVO_ABORT_CMD         0x00000002
#define SERVO_CMD_FINISHED      0x00001000
#define SERVO_ABORT_FINISHED    0x00002000


extern FLAG_T      _fServoCmdFlag;

// **********************************************************
//LJY0.62,
#define SERVO_CMD_OPEN_TRAY             (0x01)
#define SERVO_CMD_CLOSE_TRAY            (0x02)
#define SERVO_CMD_START_UNIT            (0x03)
// wyc2.17a-909S
#define SERVO_CMD_CFG_EDC_BUFF          (0x04)
//#define SERVO_CMD_GET_TOC               (0x05) ==> merged with START_UNIT
#define SERVO_CMD_CONFIG_BUF            (0x05)
#define SERVO_CMD_READ_CD               (0x06)
#define SERVO_CMD_READ_DVD              (0x07)
#define SERVO_CMD_READ_DVD_CTRL_DATA    (0x08)
#define SERVO_CMD_READ_DVD_BCA          (0x09)
#define SERVO_CMD_SEEK                  (0x0A)
#define SERVO_CMD_GET_STATUS            (0x0B)
#define SERVO_CMD_POWER_MANAGEMENT      (0x0C)
#define SERVO_CMD_SERVO_DEBUG           (0x0D)


#define GETSTATUS                       (0x10)
#define GETCOPYRIGHTINFORMATION         (0x14)
#define GETFWVERSION                    (0x15)



// for GetStatus command
#define SERVO_STATUS_TRAY                   (0x07)
#define SERVO_STATUS_MEDIA                  (0x30)
#define SERVO_STATUS_SPINDLE                (0x08) //LJY0.91, for spindle status detection

#define SERVO_STATUS_COMMAND_ERROR          (0xff)
#define SERVO_TRAY_STATUS_OPENED            (0x00)
#define SERVO_TRAY_STATUS_OPENING           (0x01)
#define SERVO_TRAY_STATUS_CLOSED            (0x02)
#define SERVO_TRAY_STATUS_CLOSING           (0x03)
#define SERVO_TRAY_STATUS_USER_PUSH         (0x04)
#define SERVO_TRAY_STATUS_UNKNOW            (0x07)
#define SERVO_MEDIA_STATUS_ANALYZING        (0x00)
#define SERVO_MEDIA_STATUS_PRESENT          (0x10)
#define SERVO_MEDIA_STATUS_NOT_PRESENT      (0x20)
#define SERVO_MEDIA_STATUS_NOT_ANALYZED     (0x30)
#define SERVO_SPINDLE_STATUS_STOPPED        (0x00) //LJY0.91, for spindle status detection
#define SERVO_SPINDLE_STATUS_SPINNING       (0x08) //LJY0.91, for spindle status detection



//***********************************************************
#define WAITSTABLE_TIMEOUT                  (COUNT_10_SEC * 4)  
#define SERVO_NODISC                        (0x00)
#define SERVO_HAVEDISC                      (0x01)
#define SERVO_WAITSTABLE_TIMEOUT            (0x02) //LJY1.05, it means servo cannot focus-on for power-on initial.
#define SERVO_USER_ABORT                    (0x03) //LJY1.05-2
#define SERVO_LOADDISC_FAILED	            (0x04) //LJY1.25, Support opentray when servo fails to close tray.
#define SERVO_WRONGDISC                     (0x05)

//***********************************************************
#if 0       // marked by F100CSC_108
//BYTE SERVO_Reset(void); //--CoCo1.20, reduce code size.
//BYTE SERVO_Init(void); //--CoCo1.20, reduce code size.
BYTE SERVO_OpenTray(void);
BYTE SERVO_CloseTray(void);
BYTE SERVO_StartUnit(void);
//BYTE SERVO_ConfigServoBuffer(BYTE bOn, BYTE bNum, DWORD wBufPar);
extern BYTE    SERVO_ConfigServoBuffer( BYTE bNum, BYTE bBufSize, ULONG_UNION ulunBufStart );
//BYTE SERVO_Stop(void); //--CoCo1.20, reduce code size.
BYTE SERVO_PowerMode (BYTE bMode);
//BYTE SERVO_GetServoDebugInfo (void); //--CoCo1.20, reduce code size.
//BYTE SERVO_SetSectors (DWORD dwStartPos, DWORD dwEndPos); //--CoCo1.20, reduce code size.
//DWORD SERVO_JumpGroove(BYTE bForward, DWORD dwTime); //--CoCo1.20, reduce code size.
BYTE SERVO_GetDiscInfo(PBYTE pMinTrack, PBYTE pMaxTrack, PDWORD pMaxSector);
BYTE SERVO_GetDiscLength(PDWORD pLength);
BYTE SERVO_GetTrackInfo(BYTE bTrackNum, PBYTE pTrackCtl, PDWORD pStartPos);
// wyc1.06-909,
//BYTE SERVO_Seek (DWORD dwSeekLBA, BYTE  bResumeRead);
extern BYTE    SERVO_Seek( ULONG_UNION ulunSeekLBA, BYTE  bResumeRead );
extern void    SERVO_EnableCacheFlush( void );      // Added by F100CSC_101a
//BYTE SERVO_ReadData(DWORD dwStartLBA, DWORD dwSectorNum, BYTE bBuf);
extern BYTE    SERVO_ReadData( ULONG_UNION ulunStartLBA, DWORD dwSectorNum, BYTE bBuf );
//BYTE SERVO_ReadDVDCtrlData(BYTE bNum); //--CoCo1.20, reduce code size.
//BYTE SERVO_ReadDVDBCAData(BYTE bNum); //--CoCo1.20, reduce code size.
WORD SERVO_GetDiscType (void);
//BOOL SERVO_EnableRunning ( BIT bGo ); //--CoCo1.20, reduce code size.
BYTE SERVO_ReadCopyInfo(void);
BYTE SERVO_Abort(void);
BYTE SERVO_GetStatus(void);
BYTE SERVO_CheckTrayStatus(void); 
//BYTE SERVO_GetDiscStatus(PBYTE pStatus); //--CoCo1.20, reduce code size.
//BYTE SERVO_WaitStable(void); //--CoCo1.20, reduce code size.
void SERVO_InitialMutexFlag(void);
// Brian0.66
BYTE SERVO_GetMediaStatus(void);
BYTE SERVO_ReadTOC(BYTE bTemp);
BYTE    SERVO_PollingMediaStatus(void);
//BYTE SERVO_CheckDiscEnding(void); //--CoCo1.20, reduce code size.
//BYTE SERVO_ReadDiscKey(void); //--CoCo1.20, reduce code size.
//BOOL    SERVO_DumpCookedSector ( BYTE * pBuf, WORD wLength, DWORD dwOffset ); //--CoCo1.20, reduce code size.
//BOOL    SERVO_DumpSector ( BYTE * pBuf, WORD wLength, DWORD dwOffset ); //--CoCo1.20, reduce code size.
//BYTE    SERVO_CSS_SentDiscKey(void); //--CoCo1.20, reduce code size.
//void SERVO_AdjustThreshold (BYTE bType, DWORD dwLevel); //--CoCo1.20, reduce code size.
BYTE SERVO_ReadTitleKey(DWORD dwStart, BYTE bStore);
BYTE SERVO_GetTrackEnd(BYTE bTrackNum, PDWORD pLength);
BYTE SERVO_SetTitleKey(BYTE bStore);
//BYTE SERVO_ResetRecover (BYTE bNeedStable); //--CoCo1.20, reduce code size.
#endif  // 0

#ifdef SUPPORT_STB
extern VOID     SERVO_Enable(VOID);
extern VOID     SERVO_Disable(VOID);
#endif

extern BYTE    SERVO_Reset( void );
extern BYTE    SERVO_OpenTray( void );
extern BYTE    SERVO_CloseTray( void );
extern BYTE    SERVO_StartUnit( void );
extern BYTE    SERVO_ConfigServoBuffer( BYTE bNum, BYTE bBufSize, ULONG_UNION ulunBufStart );
extern BYTE    SERVO_PowerMode( BYTE bMode );
extern BYTE    SERVO_GetDiscInfo( PBYTE pbMinTrack, PBYTE pbMaxTrack, PDWORD pdwMaxSector );
extern BYTE    SERVO_GetDiscLength( PDWORD pdwLength );
extern BYTE    SERVO_GetTrackEnd( BYTE bTrackNum, PDWORD pdwLength );
extern BYTE    SERVO_GetTrackInfo( BYTE bTrackNum, PBYTE pbTrackCtl, PDWORD pdwStartPos );
extern BYTE    SERVO_Seek( ULONG_UNION ulunSeekLBA, BYTE  bResumeRead );
extern void    SERVO_EnableCacheFlush( void );
extern BYTE    SERVO_ReadData( ULONG_UNION ulunStartLBA, DWORD dwSectorNum, BYTE bBuf );
extern WORD    SERVO_GetDiscType( void );
//extern BOOL    SERVO_EnableRunning ( BIT bGo ); //--CoCo1.20, reduce code size.
extern BYTE    SERVO_ReadCopyInfo( void );
//extern BYTE    SERVO_ReadDiscKey( void ); //--CoCo1.20, reduce code size.
extern BYTE    SERVO_ReadTitleKey( DWORD dwStart, BYTE bStore );
extern BYTE    SERVO_Abort( void );
extern BYTE    SERVO_GetStatus( void );
extern BYTE    SERVO_CheckTrayStatus( void );
extern void    SERVO_InitialMutexFlag( void );
extern BYTE    SERVO_GetMediaStatus( void );
extern BYTE    SERVO_ReadTOC( BYTE bTemp );
extern BYTE    SERVO_PollingMediaStatus( void );
// LLY1.50, restore API since CELL module will use SrcFilter_GetCurrentPos() --> SERVO_GetCurrentPos().
extern BYTE    SERVO_GetCurrentPos( PDWORD pdwCurrentPos ); 
//extern BYTE    SERVO_CheckDiscEnding( void ); //--CoCo1.20, reduce code size.
//extern BYTE    SERVO_CSS_SentDiscKey( void ); //--CoCo1.20, reduce code size.
//extern void    SERVO_AdjustThreshold ( BYTE bType, DWORD dwLevel ); //--CoCo1.20, reduce code size.
extern BYTE    SERVO_SetTitleKey( BYTE bStore );
//extern BYTE    SERVO_ResetRecover( BYTE bNeedStable ); //--CoCo1.20, reduce code size.
// ** TCH1.05; begin... Below section are called by SrcFilter only.
// It will tranefer the original servo definition to SRCFilter definition.
extern BYTE    SERVOFilter_GetStatus( void );
// ** TCH1.05; end... 
// wyc2.17a-909S
extern BYTE    SERVO_CFGEDCBuff( ULONG_UNION dwStartAddr, WORD_UNION wSize );

#ifdef __cplusplus
        }
#endif
#endif  // __SERVO_H__
