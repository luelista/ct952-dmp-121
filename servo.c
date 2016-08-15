
//////////////////////////////////////////////////////////////
// ***** Big Modification History Area *****
// LLY2.80a,
//  - remove the codes while define "_ALONE",

// ** TCH0.95; define _DUMP_INFO, will show playing range setting + each play range.
/// #define     _DUMP_INFO

// F100CSC_108
// Added time-out mechanism: SERVO will response soon, almost immediately,
// except SEEK command. The SEEK command will trap the caller.
// The Time-out value for all commands except SEEK command is 400ms.
// We applied 10s for SEEK command now.

#include "winav.h"

#ifdef SUPPORT_SERVO_SOURCE //++CoCo2.37p
#include <string.h>
#include "srcfilter.h"
#include "cc.h"
#include "infofilter.h"
#include "servo.h"
#include "osd.h"
#include "utl.h"
#include "input.h"
#include "hal.h"
#include "parser.h"
//LJY275e
#include "comutl.h"
#include "servo\srvudeif.h"

#define CSC_MASK        // F100CSC_108, for removing uncessary functions
 
//LJY0.68, for CMDIF handshake.
MUTEX_T     _mServoAbort;
MUTEX_T     _mServoCmd;
extern FLAG_T servo_flag_var;

//BYTE    __bCurrentIF=BITSTREAM_DVDIF; //--CoCo1.20, reduce code size.

TRKINFO _trk_TOC[100];
//TRKINFO *_trk_TOC;

// LJY275, for multi-session, gdwLeadOutPos removed
SESSIONINFO _sessioninfo_TOC[25] ;
//SESSIONINFO *_sessioninfo_TOC ;

//DWORD   _dwTitleKey [3], _dwDiscKey [4] ;

// F100CSC101a,record if SERVO cache needed to flush or not 
BYTE    _bFlushServoCache = 0;      // default is not need to do flush

COPYINFO    cpinfo ;


// Brian0.67
extern BYTE aCMDPacket[];
extern BYTE aRETPacket[];

//BYTE __bCDDA_DTS;
BYTE __bCloseCommand = 1 ;
BYTE _bSrvDiscType;
BYTE _bSRVStatus;


// Brian0.66
//DWORD   gdwNextLBA; //--CoCo1.20, reduce code size.

// Brian0.67
BYTE    _bSERVODiscType;
BYTE    _bFirstTrack, _bLastTrack, _bFirstSession, _bLastSession;
DWORD   _dwStartSector, _dwLeadOutPos;


// Added by Chern Shyh-Chain, 08-30-2001, BEGIN
// Define the structure to keep the title key (40 bit) and relative copyright information
typedef struct  tagTITLE_KEY_COPY_INFO_LOG
{
    ULONGLONG_UNION ulunTitleKey;       // Keep the Title Key
    COPYINFO    CopyInfo;       // Keep the relative Copyright Information
} TITLE_KEY_COPY_INFO_LOG, *PTITLE_KEY_COPY_INFO_LOG;

// Record the multiple title keys and copyright information for special title.
// The maximum title keys can be recorded is defined by MAX_TITLE_KEY_NUM (infofilter.h).
TITLE_KEY_COPY_INFO_LOG _TitleKeyCopyInfoStore[MAX_TITLE_KEY_NUM];

// Keep cuurent title key and copyright information.
//TITLE_KEY_COPY_INFO_LOG _CurrentTitleKeyCopyInfo; //--CoCo1.20, reduce code size.

#define NULL_TITLE_KEY      0  // This is a empty title key value
// Added by Chern Shyh-Chain, 08-30-2001, END

BYTE    _IssueNewCommand( DWORD dwTimeOut );

// ***********************************************************************
//  Function    :   SERVO_Reset
//  Description :   Will do nothing for ude dvdrom
//  Arguments   :   None
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
/*--CoCo1.20, reduce code size.
BYTE    SERVO_Reset( void )
{
    return TRUE;
}
*/


// ***********************************************************************
//  Function    :   SERVO_Init
//  Description :   Will do configure loader command
//  Arguments   :   None
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
/*--CoCo1.20, reduce code size.
#ifndef CSC_MASK
BYTE  SERVO_Init(void)
{
    SERVO_InitialMutexFlag(); // Brian0.66
    
    return TRUE;
}
#endif  // CSC_MASK
*/

// ***********************************************************************
//  Function    :   SERVO_OpenTray
//  Description :   Open the tray of dvd-rom
//  Arguments   :   None
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
BYTE    SERVO_OpenTray( void )
{
    BYTE    bRst;

    OS_LockMutex( &_mServoCmd );

    aCMDPacket[0] = SERVO_CMD_OPEN_TRAY ;     //open

    bRst = _IssueNewCommand( COUNT_400_MSEC );
    __bCloseCommand = 0 ;

    OS_UnlockMutex( &_mServoCmd );
    return bRst;
}

// ***********************************************************************
//  Function    :   SERVO_CloseTray
//  Description :   Close the tray of dvd-rom
//  Arguments   :   None
//  Return      :   TRUE/FALSE
//  Side Effect :   None
// ***********************************************************************
BYTE    SERVO_CloseTray( void )
{  
    BYTE    bRst;

    OS_LockMutex( &_mServoCmd );

    aCMDPacket[0] = SERVO_CMD_CLOSE_TRAY ;    // close

    bRst = _IssueNewCommand( COUNT_400_MSEC );
    
    __bCloseCommand = 1 ;

    OS_UnlockMutex( &_mServoCmd );

    return bRst;
}

#ifdef SUPPORT_SERVO_SOURCE //CoCo1.20, reduce code size.
// SERVO return immediately but caller needs to poll the SERVO status.
BYTE    SERVO_StartUnit( void )
{  
    BYTE    bRst;

    OS_LockMutex( &_mServoCmd );

    aCMDPacket[0] = SERVO_CMD_START_UNIT;

    bRst = _IssueNewCommand( COUNT_400_MSEC );
    
    __bCloseCommand = 1 ;

    OS_UnlockMutex( &_mServoCmd );

    return bRst;
}
#endif

// wyc2.17a-909S, add new function to assign DRAM address for servo.
// ***********************************************************************
//  Function    :   SERVO_CFGEDCBuff
//  Description :   Config the SERVO EDC buffer.
//  Arguments   :   dwStartAddr: The start address. wSize: The size of buffer, unit is WORD.
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
BYTE    SERVO_CFGEDCBuff( ULONG_UNION dwStartAddr, WORD_UNION wSize )
{
    BYTE    bRst;

    OS_LockMutex( &_mServoCmd );

    aCMDPacket[0] = SERVO_CMD_CFG_EDC_BUFF;
    aCMDPacket[2] = dwStartAddr.b8bit[0];  //HIBYTE(HIWORD(dwStartAddr)); //buffer startH
    aCMDPacket[3] = dwStartAddr.b8bit[1];  //LOBYTE(HIWORD(dwStartAddr)); //buffer startH
    aCMDPacket[4] = dwStartAddr.b8bit[2];  //HIBYTE(LOWORD(dwStartAddr)); //buffer startL
    aCMDPacket[5] = dwStartAddr.b8bit[3];  //LOBYTE(LOWORD(dwStartAddr)); //buffer startL
    aCMDPacket[6] = wSize.b8bit[0]; //buffer szie
    aCMDPacket[7] = wSize.b8bit[1]; //buffer szie

    bRst = _IssueNewCommand( COUNT_400_MSEC );

    OS_UnlockMutex( &_mServoCmd );
    return bRst;
}


BYTE    SERVO_ConfigServoBuffer( BYTE bNum, BYTE bBufSize, ULONG_UNION ulunBufStart )
{    
    BYTE    bRst;

    OS_LockMutex( &_mServoCmd );

    aCMDPacket[0] = SERVO_CMD_CONFIG_BUF;
    aCMDPacket[1] = bNum; //buffer number
    aCMDPacket[2] = ulunBufStart.b8bit[0];  //HIBYTE(HIWORD(dwBufStart)); //buffer startH
    aCMDPacket[3] = ulunBufStart.b8bit[1];  //LOBYTE(HIWORD(dwBufStart)); //buffer startH
    aCMDPacket[4] = ulunBufStart.b8bit[2];  //HIBYTE(LOWORD(dwBufStart)); //buffer startL
    aCMDPacket[5] = ulunBufStart.b8bit[3];  //LOBYTE(LOWORD(dwBufStart)); //buffer startL
    aCMDPacket[6] = bBufSize; //buffer szie(unit: 0xA000 bytes)

    bRst = _IssueNewCommand( COUNT_400_MSEC );

    OS_UnlockMutex( &_mServoCmd );

    return bRst;
}


// ***********************************************************************
//  Function    :   SERVO_Stop
//  Description :   Used to stop the dvd-rom
//  Arguments   :   None.
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
/*--CoCo1.20, reduce code size.
#ifndef CSC_MASK
BYTE  SERVO_Stop(void)
{
//    if (!__btPowerDown)
//        return TRUE ;

    return (SERVO_PowerMode (SERVO_POWER_STOP));
}
#endif  // CSC_MASK
*/

// ***********************************************************************
//  Function    :   SERVO_PowerMode
//  Description :   Used to let cd/dvd-rom enter power down mode
//  Arguments   :   bMode :
//                  SERVO_POWER_STOP : Enter stop state
//                  SERVO_POWER_IDLE : Enter idle state
//                  SERVO_POWER_STANDBY : Enter standby mode
//                  SERVO_POWER_SLEEP : Enter sleep mode
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
BYTE    SERVO_PowerMode( BYTE bMode )
{
    BYTE    bRst;

    if( bMode == SERVO_POWER_START )
    {
        return 1 ;
    }

    OS_LockMutex( &_mServoCmd );

    aCMDPacket[0] = SERVO_CMD_POWER_MANAGEMENT;    // power mode
    aCMDPacket[1] = bMode ;     // enter stop mode

    bRst = _IssueNewCommand( COUNT_400_MSEC );

    OS_UnlockMutex( &_mServoCmd );

    return bRst;
}

// LJY275e
// ***********************************************************************
//  Function    :   SERVO_GetServoDebugInfo
//  Description :   Used to get servo debug information
//  Arguments   :   None
//  Return      :   version number
//  Side Effect :
// ***********************************************************************
/*--CoCo1.20, reduce code size.
#ifndef CSC_MASK
BYTE SERVO_GetServoDebugInfo (void)
{
    //to be added...
    return TRUE;
}
#endif  // CSC_MASK
*/

// **********************************************************
// ** TCH0.27; begin...
//  *********************************************************************
//  Function    :   SERVO_SetSectors    // ** TCH0.27;
//  Description :   Record the play range
//  Arguments   :   dwStartPos  : the starting sector
//                  dwEndPos    : the ending sector
//  Return      :   FALSE if ( End < Start )
//  Side Effect :   It may set (End<Start) conscious sometime. In this condition,
//                  Servo will stop to send anything except re-set the range.
//  *********************************************************************
/*--CoCo1.20, reduce code size.
#ifndef CSC_MASK
BYTE SERVO_SetSectors (DWORD dwStartPos, DWORD dwEndPos)
{
    if (dwEndPos < dwStartPos)
    {
        return FALSE;
    }
    __SF_SourceGBL[0].dwEndSector = dwEndPos ;
    __SF_SourceGBL[0].dwWriteSector = dwStartPos ;    

    return TRUE;
}
#endif  // CSC_MASK
*/

// **********************************************************
//DVD020LLY, because scan function will call this function to jump to another position
// ***********************************************************************
//  Function    :   SERVO_JumpGroove
//  Description :   Used to get the position for scan function
//  Arguments   :   None.
//  Return      :   The next scan position
//  Side Effect :
// ***********************************************************************
/*--CoCo1.20, reduce code size.
#ifndef CSC_MASK
DWORD SERVO_JumpGroove(BYTE bForward, DWORD dwTime)
{
    extern WORD    __wScanGap;
    if ( bForward )   // KEY_SCF
    {
        // the jump time range
        dwTime+= __wScanGap;
    }
    else
    {
        if ( dwTime > (__wScanGap+ SCB_EXTRA) )
            dwTime-= (__wScanGap+ SCB_EXTRA);
        else
            dwTime= (DWORD) NULL;
    }
    // the time need jump to
    return dwTime;

}
#endif  // CSC_MASK
*/

// ***********************************************************************
//  Function    :   SERVO_GetDiscInfo
//  Description :   Used to get some the following informations from disc (not for DVD)
//  Arguments   :   pMinTrack : the minimum track number
//                  pMaxTrack : the maximum track number
//                  pMaxSector : the maximum sector number of the disc
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
BYTE    SERVO_GetDiscInfo( PBYTE pbMinTrack, PBYTE pbMaxTrack, PDWORD pdwMaxSector )
{
    BYTE    bSrvTmp, bLastTrk, bLastSession;

    *pbMinTrack = _sessioninfo_TOC[0].bFirstTrack;
    bLastSession = _sessioninfo_TOC[0].bLastSession;
    bLastTrk = _sessioninfo_TOC[bLastSession-1].bLastTrack;
//    if ((__wDiscType == BOOK_CDDA) && (_dwDiscKey [0]==0))
    if( __wDiscType == BOOK_CDDA )
    {
//        _dwDiscKey [0] = 1 ;
        bSrvTmp = bLastTrk ;
        while( bSrvTmp > _sessioninfo_TOC[0].bFirstTrack )
        {
            if( _trk_TOC[bSrvTmp-1].Ctl & 0x40 )    // Data Track
            {
                bSrvTmp --;
            }
            else
            {
                break;
            }
        }
        bLastTrk = bSrvTmp;
    }
    *pbMaxTrack = bLastTrk;
    *pdwMaxSector = _sessioninfo_TOC [bLastSession-1].dwLeadOutPos; // LJY275

    return TRUE;
}

// ***********************************************************************
//  Function    :   SERVO_GetDiscLength
//  Description  :   Used to get length for the disc (not for DVD)
//                  It will take session information into consideration.
//  Arguments   :   pLength : length of the disc
//  Return      :   TRUE/FALSE
//  Side Effect  :
// ***********************************************************************
BYTE    SERVO_GetDiscLength( PDWORD pdwLength )
{
    BYTE    bLastSession;
    
#ifndef NO_DVD //CoCo2.37p
    if( __wDiscType & BOOK_DVD )
    {
#ifdef SUPPORT_PRINTF        
        printf ("SERVO_GetDiscLength is not supported for DVD title\n") ;
#endif

        return FALSE;
    }
#endif //#ifndef NO_DVD //CoCo2.37p

    bLastSession = _sessioninfo_TOC[0].bLastSession;
    //LJY0.87, fix the wrong total disc time bug of Abex 721 CDDA test title
//    *pLength = _sessioninfo_TOC [bLastSession-1].dwLeadOutPos + TRACK_GAP;
    //LJY1.25, WYC1.25, fix CDDA gototime issue. Leadout should be the discend.
    *pdwLength = _sessioninfo_TOC [bLastSession-1].dwLeadOutPos;// + TRACK_GAP;

    return TRUE;
}

// ***********************************************************************
//  Function    :   SERVO_GetTrackEnd
//  Description  :   Used to get length for the track (not for DVD)
//  Arguments   :   bTrackNum : the track number you want to get the length
//                  pLength : length of the track
//  Return      :   TRUE/FALSE
//  Side Effect  :
// ***********************************************************************
BYTE    SERVO_GetTrackEnd( BYTE bTrackNum, PDWORD pdwLength )
{
    BYTE    i, bLastSession;

#ifndef NO_DVD //CoCo2.37p
    if( __wDiscType & BOOK_DVD )
    {
#ifdef SUPPORT_PRINTF        
        printf ("SERVO_GetTrackLength is not supported for DVD title\n") ;
#endif

        return FALSE ;
    }
#endif //#ifndef NO_DVD //CoCo2.37p

    bLastSession = _sessioninfo_TOC[0].bLastSession;
    // LJY275, for multi-session 2002/6/28
    for( i = 0; i < bLastSession; i ++ )
    {
        if( (bTrackNum >= _sessioninfo_TOC [i].bFirstTrack) && (bTrackNum <= _sessioninfo_TOC [i].bLastTrack) )
        {
            break ;
        }
    }
    if( i >= bLastSession )
    {
//LJY0.87, the track end of last track should be the sector of (leadout - 1)
        *pdwLength = _sessioninfo_TOC [bLastSession-1].dwLeadOutPos - 1; //real track end
//      *pLength = _sessioninfo_TOC [_bLastSession-1].dwLeadOutPos;
        return FALSE ;
    }
    if( bTrackNum == _sessioninfo_TOC [i].bLastTrack )
    {
//LJY0.87, the track end of last track should be the sector of (leadout - 1)
        *pdwLength = _sessioninfo_TOC [i].dwLeadOutPos - 1; //real track end        
//        *pLength = _sessioninfo_TOC [_bSERVOTmp2].dwLeadOutPos;
    }
    else
    {
///        *pLength = _MSFtoHSG (_trk_TOC[bTrackNum].Min, _trk_TOC[bTrackNum].Sec, _trk_TOC[bTrackNum].Frm)- 1 ;
        *pdwLength = COMUTL_MSFtoHSG ( MAKE_MSF(_trk_TOC[bTrackNum].Min, _trk_TOC[bTrackNum].Sec, _trk_TOC[bTrackNum].Frm))- 1 ;
    }
   
    return TRUE ;
}

// ***********************************************************************
//  Function    :   SERVO_GetTrackInfo
//  Description :   Used to get informations from the track (not for DVD)
//  Arguments   :   bTrackNum : the track number you want to get information
//                  pTrackCtl : used to distinguish if the track is data or CDDA
//                  pStartPos : the starting sector number of the track
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
BYTE    SERVO_GetTrackInfo( BYTE bTrackNum, PBYTE pbTrackCtl, PDWORD pdwStartPos )
{
    BYTE    bIndex;

    for( bIndex=0; bIndex<_sessioninfo_TOC[0].bLastSession; bIndex++ )
    {
        if( (bTrackNum >= _sessioninfo_TOC [bIndex].bFirstTrack) && (bTrackNum <= _sessioninfo_TOC [bIndex].bLastTrack) )
        {
            break ;
        }
    }

    if( bIndex == _sessioninfo_TOC[0].bLastSession )
    {
        bIndex=0; //use the first session as default
    }

    _bSrvDiscType = _sessioninfo_TOC[bIndex].bDiscType;

    bIndex = bTrackNum - _sessioninfo_TOC[0].bFirstTrack;

    // Set track type to DATA TRACK if disc type is DVD.
    // prevent stop->play fast/slow become invalid problem
//    if (_bSrvDiscType == DISC_TYPE_DVD)
    if( !(_bSrvDiscType & DISC_TYPE_CD) )   //LJY0.75, to conform with servo's definition
    {
        *pbTrackCtl = DATA_TRACK;
        return TRUE;
    }
    else if( _bSrvDiscType == DISC_TYPE_CDI )
    {
        *pbTrackCtl = 0x40;
        return FALSE;
    }

    *pbTrackCtl = _trk_TOC[bIndex].Ctl;
    // ADR/Ctl read from ude dvdrom
    // cdda   -> 1
    // vcd    -> 41
    // MP3 m1 -> 61
    // MP3 m2 -> 41
    //if (*pTrackCtl & 40)
    if( *pbTrackCtl & 0x40 )
        *pbTrackCtl = DATA_TRACK;        // ** DVD-TCH0.19;
    else    // Audio Track
    {
        *pbTrackCtl = 0x0;
    }

///    *pStartPos = _MSFtoHSG (_trk_TOC[bIndex].Min, _trk_TOC[bIndex].Sec, _trk_TOC[bIndex].Frm) ;
    *pdwStartPos = COMUTL_MSFtoHSG( MAKE_MSF(_trk_TOC[bIndex].Min, _trk_TOC[bIndex].Sec, _trk_TOC[bIndex].Frm)) ;

    return TRUE;
}

//LJY278, for pre-start servo
// wyc1.06-909, add parameter, when bResumeRead is TRUE, then SERVO will resume to read data when seek command is issued.
BYTE    SERVO_Seek( ULONG_UNION ulunSeekLBA, BYTE  bResumeRead )
{
    BYTE bRst;
    
    OS_LockMutex( &_mServoCmd );

    aCMDPacket[0] = SERVO_CMD_SEEK ;   
    aCMDPacket[1] = ulunSeekLBA.b8bit[1];   //LOBYTE(HIWORD(dwSeekLBA));
    aCMDPacket[2] = ulunSeekLBA.b8bit[2];   //HIBYTE(LOWORD(dwSeekLBA));
    aCMDPacket[3] = ulunSeekLBA.b8bit[3];   //LOBYTE(LOWORD(dwSeekLBA));
    aCMDPacket[4] = bResumeRead;

    bRst = _IssueNewCommand( COUNT_10_SEC );

    OS_UnlockMutex( &_mServoCmd );

    return bRst;
}

// F100CSC101a, Set the SERVO cache flush flag
void    SERVO_EnableCacheFlush( void )
{
    _bFlushServoCache = 0x02;

    return;
}

// ***********************************************************************
//  Function    :   SERVO_ReadData
//  Description :   Used to read data from ude
//                  1. Send command according to the disc type
//                  2. Check if transfer complete
//  Arguments   :   dwStartLBA : The starting sector number to read
//                  wSectorNum : number of sectors to read
//  Return      :   TRUE/FALSE
//  Side Effect :   
// ***********************************************************************
BYTE    SERVO_ReadData( ULONG_UNION ulunStartLBA, DWORD dwSectorNum, BYTE bBuf )
{
    BYTE        bRst;
    ULONG_UNION ulunTmp;

    OS_LockMutex( &_mServoCmd );

    ulunTmp.dw32bit = ulunStartLBA.dw32bit + dwSectorNum - 1;

    aCMDPacket[1] = ulunStartLBA.b8bit[1];  //LOBYTE(HIWORD(dwStartLBA));
    aCMDPacket[2] = ulunStartLBA.b8bit[2];  //HIBYTE(LOWORD(dwStartLBA));
    aCMDPacket[3] = ulunStartLBA.b8bit[3];  //LOBYTE(LOWORD(dwStartLBA));
    aCMDPacket[4] = bBuf;
    aCMDPacket[5] = ulunTmp.b8bit[1];   //LOBYTE(HIWORD(dwSrvTmp1));
    aCMDPacket[6] = ulunTmp.b8bit[2];   //HIBYTE(LOWORD(dwSrvTmp1));
    aCMDPacket[7] = ulunTmp.b8bit[3];   //LOBYTE(LOWORD(dwSrvTmp1));

    // LLY0.91, tell parser the item as M2F2 format
    // while playback VCD/ CVD/ SVCD title bitstream.
    if( ((__wDiscType & BOOK_2X) || (__wDiscType & BOOK_VCD)) && 
        ((__bModePlay == MODE_PLAYMOTION) || (__bModePlay & MODE_STILL)) )
    {
        aCMDPacket[9] = 1;  //M2F2
    }
    else
    {
        aCMDPacket[9] = 0;
    }

    // F100CSC101a, Enable SERVO to do cache flush and re-read data
    aCMDPacket[9] |= _bFlushServoCache;
    _bFlushServoCache = 0;

    // [F100CSC_TODO] This variable will be initiated at SERVO_GetTrackInfo and
    // SERVO_GetDiscType. It is a must to call this function at first. ???? RISKY
    // For mini-DVD issue???? (recognized as BOOK_DVD)
//    if (_bSrvDiscType == DISC_TYPE_DVD)
    if( !(_bSrvDiscType & DISC_TYPE_CD) )   //LJY0.75, to conform with servo's definition
    {
        aCMDPacket[0] = SERVO_CMD_READ_DVD;     // UDE : Read_DVD command
    }
    else // for non-DVD type, all use cd type if (_bSrvDiscType == DISC_TYPE_VCD)
    {
        aCMDPacket[0] = SERVO_CMD_READ_CD;    // UDE : Read_CD command

        if( __bAttrPlay == ATTR_CDDA )
        {
            if( (__bCDDA_DTS == CDDA_PURE) || (__bCDDA_DTS == CDDA_HDCD) )
            {
                aCMDPacket[8] = AUDIO_DATA;
            }
            else if( __bCDDA_DTS == CDDA_CDG )
            {
                aCMDPacket[8] = SUBCHANNEL_ENABLE;
            }            
            else if( __bCDDA_DTS == CDDA_DTS )  //LJY0.90, for non-interpolation
            {
                aCMDPacket[8] = INTERPOLATION_DISABLE;  // for CD_DTS non-interpolation, w/O subcode           
            }
        }
        else        
        {
            // wyc1.02-909, CD disc all need to disable INTERPOLATION.
            aCMDPacket[8] = CONTINUOUS_DATA | INTERPOLATION_DISABLE;      // for digital data, w/O subcode
        }
    }

    bRst = _IssueNewCommand( COUNT_400_MSEC );

    OS_UnlockMutex( &_mServoCmd );

    return bRst;
}


/*--CoCo1.20, reduce code size.
#ifndef CSC_MASK
BYTE    SERVO_ReadDVDCtrlData( BYTE bNum )
{
    BYTE    bRst;

    cyg_mutex_lock(&_mServoCmd);
    aCMDPacket[0] = SERVO_CMD_READ_DVD_CTRL_DATA;
    aCMDPacket[1] = bNum; // servo dump-in buffer number

    bRst = _IssueNewCommand( COUNT_400_MSEC );

    cyg_mutex_unlock(&_mServoCmd);

    return bRst;
}

BYTE    SERVO_ReadDVDBCAData( BYTE bNum )
{
    BYTE    bRst;

    cyg_mutex_lock( &_mServoCmd );
    aCMDPacket[0] = SERVO_CMD_READ_DVD_BCA;
    aCMDPacket[1] = bNum;   // servo dump-in buffer number

    bRst = _IssueNewCommand( COUNT_400_MSEC );

    cyg_mutex_unlock( &_mServoCmd );

    return bRst;
}
#endif  // CSC_MASK
*/

// ***********************************************************************
//  Function    :   SERVO_GetDiscType
//  Description :   Used to get the type of disc
//  Arguments   :   None
//  Return      :   The disc type of current disc
//  Side Effect :   Must be called after SERVO_ReadTOC
// ***********************************************************************
WORD    SERVO_GetDiscType( void )
{
    WORD    wTmp;

    //LJY0.76, refer to "_sessioninfo_TOC[0].bDiscType" instead of issuing GetDiscType CMD.
    _bSrvDiscType = _sessioninfo_TOC[0].bDiscType;

    switch (_bSrvDiscType)
    {
        case DISC_TYPE_DVD :
        case DISC_TYPE_DVD_RAM:
        case DISC_TYPE_DVD_R:
        case DISC_TYPE_DVD_RW:
        case DISC_TYPE_DVD_PLUS_RW:
        case DISC_TYPE_DVD_PLUS_R:  //LJY0.85
        // wyc2.38a-909s, add one mode for DVD plus R dual layer disc.
        case DISC_TYPE_DVD_PLUS_R_DUAL:
            wTmp = BOOK_DVD ;
            SERVO_ReadCopyInfo( );  //LJY0.81, read copyright info for CSS 
            break ;

        case DISC_TYPE_VCD :
            wTmp = BOOK_VCD ;
            break ;

        case DISC_TYPE_CDI :
            wTmp = BOOK_CDI ;
            break ;

        case DISC_TYPE_CDDA :
            wTmp = BOOK_CDDA ;
            break ;

        default :
            wTmp = BOOK_UNKNOW ;
            break ;
    }

    return wTmp;
}

/*--CoCo1.20, reduce code size.
BOOL    SERVO_EnableRunning ( BIT bGo )
{
    return TRUE;
}
*/


// ***********************************************************************
//  Function    :   SERVO_ReadCopyInfo
//  Description :   Will read the copy information of the dvd title
//                  cpinfo.bCPS_Type = 1 if copy protected
//                  cpinfo.bRegionInfo store the region code information
//  Arguments   :   None
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
BYTE    SERVO_ReadCopyInfo( void )
{
    BYTE bRst;

    OS_LockMutex( &_mServoCmd );

    aCMDPacket[0] = SERVO_CMD_GET_STATUS;
    aCMDPacket[1] = GETCOPYRIGHTINFORMATION;    // GetCopyrightInformation

    bRst = _IssueNewCommand( COUNT_400_MSEC );

    cpinfo.bCPS_Type = aRETPacket[2];
    cpinfo.bRegionInfo = aRETPacket[3];

    OS_UnlockMutex( &_mServoCmd );

    return bRst;
}

/*--CoCo1.20, reduce code size.
BYTE    SERVO_ReadDiscKey( void )
{
#if 0   // marked by F100CSC_107
    DWORD dwTmp;
    
    if (!cpinfo.bCPS_Type)  // non CSS title
        return TRUE ;

    // clear all save title key information since new title inserted
    for( dwTmp = 0; dwTmp < MAX_TITLE_KEY_NUM; dwTmp ++ )
    {
        _TitleKeyCopyInfoStore[dwTmp].ulunTitleKey.ull64bit = NULL_TITLE_KEY;
    }

    // get new disc key
    PARSER_Command( 0, PARSER_CMD_DISC_KEY, NULL );
#endif  // 0

#if 0//def SUPPORT_PRINTF  
    {
        int i, j ;

        printf ("CPS Type = %u\n", cpinfo.bCPS_Type) ;
        printf ("Region [") ;

        i = 1 ;
        for (j=1;j<7;j++)
        {
            if (!(cpinfo.bRegionInfo & i))
                printf ("%d, ", j) ;
                i<<=1 ;
        }
        printf ("]\n") ;
    }
#endif

    return TRUE;
}
*/

BYTE    SERVO_ReadTitleKey( DWORD dwStart, BYTE bStore )
{
#define NO_OF_SECTOR        16
    DWORD dwTmp;
    ULONGLONG_UNION ullunTitleKey;
    
    PARSER_ActionFilter( 0, PARSER_ENABLE_COPY_PROTECTION, FALSE );
    if (!cpinfo.bCPS_Type)  // non CSS title
        return TRUE ;

    // Check if the parameter "bStore" overs the boundary, MAX_TITLE_KEY_NUM (defined in "cdinfo.h").
    if( bStore > MAX_TITLE_KEY_NUM )
        return FALSE;

    // If this title need to be stored and has been read, we just return TRUE directly.
    if( bStore && (_TitleKeyCopyInfoStore[bStore - 1].ulunTitleKey.ull64bit != NULL_TITLE_KEY) )
    {
        PARSER_WriteTitleKey( _TitleKeyCopyInfoStore[bStore - 1].ulunTitleKey );
        cpinfo = _TitleKeyCopyInfoStore[bStore - 1].CopyInfo;
        if( cpinfo.bCPS_Type == 1 )
        {
            PARSER_ActionFilter( 0, PARSER_ENABLE_COPY_PROTECTION, TRUE );
        }
        return TRUE;
    }

    // wyc2.53-909P,
    SrcFilter_ReadSectors(__dwSFStreamID, dwStart, dwStart + NO_OF_SECTOR );
    dwTmp = PARSER_PRESCAN_TITLE_KEY;
//    if( PARSER_Command(0, PARSER_CMD_PRESCAN, &dwTmp) )
    PARSER_Command( 0, PARSER_CMD_PRESCAN, &dwTmp );
    while( !PARSER_QueryEvent(0, PARSER_EVENT_COMPLETED, FALSE) )
    {
        OS_YieldThread( );
    }

    if( !PARSER_QueryEvent(0, PARSER_EVENT_ERROR, FALSE) )
    {
        dwTmp = PARSER_ReadCPRMAI( );

        // Need to save this title key????
        PARSER_ReadTitleKey( &ullunTitleKey );

        if( dwTmp & 0x08 )      // it contains copyrighted information
        {
            if( ullunTitleKey.ull64bit == 0 )
            {
                // incorrect title key was fouund and treat this as no CSS protected
                cpinfo.bCPS_Type = 2;   // indicate this title is CSS scrambled but current play range is not CSS protected
            }
            else
            {
                cpinfo.bCPS_Type = 1;   // indicate this title is CSS scrambled but current play range is CSS protected
                PARSER_ActionFilter( 0, PARSER_ENABLE_COPY_PROTECTION, TRUE );
            }
        }
        else
        {
            cpinfo.bCPS_Type = 2;   // indicate this title is CSS scrambled but current play range is not CSS protected
        }

        if( bStore )
        {
            _TitleKeyCopyInfoStore[bStore - 1].ulunTitleKey.ull64bit = ullunTitleKey.ull64bit;
            _TitleKeyCopyInfoStore[bStore - 1].CopyInfo = cpinfo;
        }
    }
    else
    {
        cpinfo.bCPS_Type = 2;   // indicate this title is CSS scrambled but current play range is not CSS protected
    }

    return TRUE;
#if 0
#define NO_OF_SECTOR    10
    DWORD   dwTimeNow ;
    extern    WORD    __wCDROMReadingOffset ;
       
// Added by Chern Shyh-Chain, 08-30-2001, BEGIN
    // Check if the parameter "bStore" overs the boundary, MAX_TITLE_KEY_NUM (defined in "cdinfo.h").
    if( bStore > MAX_TITLE_KEY_NUM )
        return FALSE;

    // If this title need to be stored and has been read, we just return TRUE directly.
    if( bStore &&
            ((_TitleKeyCopyInfoStore[bStore - 1].dwTitleKey32Bit != NULL_TITLE_KEY) || (_TitleKeyCopyInfoStore[bStore - 1].bTitleKey8Bit != NULL_TITLE_KEY)) )
    {
        return TRUE;
    }
// Added by Chern Shyh-Chain, 08-30-2001, END    

    if (!cpinfo.bCPS_Type)  // non CSS title
        return TRUE ;
    __bSERVOErrMsg = SERVO_ERROR_AUTH ;
    //UTL_ReadSectors (dwSectorNumber, NO_OF_SECTOR, (BYTE)NULL) ; //--CoCo1.20, reduce code size.
        // TCC275, must add __wCDROMReadingOffset
    dwTimeNow = __dwW99AVECCBASE + __wCDROMReadingOffset*2064L/4  ;
    
RETRY_TITLEKEY:
    W99AV_ReadDRAMData (dwTimeNow+1, &_dwSERVOTmp) ;
    cpinfo.bCGMS = (BYTE) (_dwSERVOTmp & 0x3000) >> 12 ;    // TCC2.79, CGMS in bit 4~5
    
    if ((HIBYTE(LOWORD(_dwSERVOTmp)) & 0x80) != 0x80) // This sector is not scrambled
    {
        dwTimeNow += 516; //2064/4 
        if (dwTimeNow >= __dwW99AVECCBASE  +5160)//2064/4*10
            return FALSE ;
        goto RETRY_TITLEKEY ;
}

#ifdef USE_LITTLE_ENDIAN    
    _pbSERVOTmp1 = (BYTE *) &_dwTitleKey[0] ;
    *(_pbSERVOTmp1+3) = LOBYTE(LOWORD(_dwSERVOTmp)) ;
    W99AV_ReadDRAMData (dwTimeNow+2, &_dwSERVOTmp) ;
    *(_pbSERVOTmp1+2) = HIBYTE(HIWORD(_dwSERVOTmp)) ;
    *(_pbSERVOTmp1+1) = LOBYTE(HIWORD(_dwSERVOTmp)) ;
    *(_pbSERVOTmp1+0) = HIBYTE(LOWORD(_dwSERVOTmp)) ;
    
    _pbSERVOTmp1 = (BYTE *) &_dwTitleKey[1] ;
    *(_pbSERVOTmp1+3) = LOBYTE(LOWORD(_dwSERVOTmp)) ;
    *(_pbSERVOTmp1+2) = 0 ;
    *(_pbSERVOTmp1+1) = 0 ;
    *(_pbSERVOTmp1+0) = 0 ;
#else
    _pbSERVOTmp1 = (BYTE *) &_dwTitleKey[0] ;
    *(_pbSERVOTmp1+0) = LOBYTE(LOWORD(_dwSERVOTmp)) ;
    W99AV_ReadDRAMData (dwTimeNow+2, &_dwSERVOTmp) ;
    *(_pbSERVOTmp1+1) = HIBYTE(HIWORD(_dwSERVOTmp)) ;
    *(_pbSERVOTmp1+2) = LOBYTE(HIWORD(_dwSERVOTmp)) ;
    *(_pbSERVOTmp1+3) = HIBYTE(LOWORD(_dwSERVOTmp)) ;
    
    _pbSERVOTmp1 = (BYTE *) &_dwTitleKey[1] ;
    *(_pbSERVOTmp1+0) = LOBYTE(LOWORD(_dwSERVOTmp)) ;
    *(_pbSERVOTmp1+1) = 0 ;
    *(_pbSERVOTmp1+2) = 0 ;
    *(_pbSERVOTmp1+3) = 0 ;
#endif    
    _dwTitleKey[2] = 0 ;

// Added by Chern Shyh-Chain, 05-11-2001, BEGIN
    // TCC099, for CGMS
    //if (cpinfo.bCGMS & 0x80)
    {
        switch (cpinfo.bCGMS)
{
            case 0: // Copying is permitted without restriction
                cpinfo.bCGMS = SERVO_CGMS_NO_RESTRICTION ;
#ifdef ENABLE_SCMS
                W99AV_WriteDM (W99AV_DM_SCMS, 1) ;
#endif
                break ;
            case 1: // reserved
                cpinfo.bCGMS = SERVO_CGMS_RESERVED ;
                break ;
            case 2: // One generation of copies may be made
                cpinfo.bCGMS = SERVO_CGMS_ONE_COPY ;
#ifdef ENABLE_SCMS
                W99AV_WriteDM (W99AV_DM_SCMS, 0) ;
#endif
                break ;
            case 3: // No copping is permitted
                cpinfo.bCGMS = SERVO_CGMS_NO_COPY ;
#ifdef ENABLE_SCMS
                W99AV_WriteDM (W99AV_DM_SCMS, 2) ;
#endif
                break ;
        }
    }
// Added by Chern Shyh-Chain, 05-11-2001, END
    // TCC2.79-2, for checking the title ("ATTILA(R2)" from GBM) that report as CSS enabled but not scrambled.
    // The title has a disc key but title keys are all 0.
    {
        // Check if all title keys are 0
        if ((_dwTitleKey[0]==0 && _dwTitleKey[1]==0))
        {
            cpinfo.bCPS_Type = 2 ;
        }
        else
        {
            cpinfo.bCPS_Type = 1 ;
        }
    }
    // End
// Added by Chern Shyh-Chain, 08-30-2001, BEGIN
    if( bStore )
    {
        bStore -= 1;
        _TitleKeyCopyInfoStore[bStore].dwTitleKey32Bit = _dwTitleKey[0];
        _TitleKeyCopyInfoStore[bStore].bTitleKey8Bit = (BYTE) (_dwTitleKey[1] >> 24);
        _TitleKeyCopyInfoStore[bStore].CopyInfo = cpinfo;
        return TRUE;
    }
// Added by Chern Shyh-Chain, 08-30-2001, END

    _CSS_SentTitleKey () ;
    __bSERVOErrMsg = SERVO_ERROR_NONE ;
        return TRUE ;
#endif

}


// ***********************************************************************
// Function    :   SERVO_Abort
// Description :   Abort  current servo CMD when OPEN/CLOSE/PREV/NEXT IR key was asserted
// ***********************************************************************
BYTE    SERVO_Abort( void )
{
    DWORD    flagRet;
    BYTE    bRet = 0;

    OS_LockMutex( &_mServoAbort );

    OS_ClearFlag( &servo_flag_var, ~SERVO_ABORT_FINISHED );

    OS_SetFlag( &servo_flag_var, SERVO_ABORT_CMD );
    flagRet = OS_TimedWaitFlag( &servo_flag_var, SERVO_ABORT_FINISHED,
            FLAG_WAITMODE_OR, COUNT_400_MSEC );
    OS_ClearFlag( &servo_flag_var, ~SERVO_ABORT_FINISHED );

    if( (flagRet != 0) && (aRETPacket[1] == NO_ERROR) )
    {
        bRet = 1;
    }

    OS_UnlockMutex( &_mServoAbort );

    return bRet;
}


// ***********************************************************************
// Function    :    SERVO_GetStatus
// Description :    Get the status from ude dvd-rom
// Arguments   :    bStatus : 
//                  UDE_STATUS_PLAYBACK : check play back status
//                  UDE_STATUS_TRAY : check tray status
//                  UDE_STATUS_MEDIA : check media status
//                  UDE_STATUS_QUEUE : check queue status
//                  UDE_STATUS_READ : check read status
// Return      :    TRUE/FALSE
// Side Effect :
// ***********************************************************************
BYTE    SERVO_GetStatus( void )
{
    BYTE bRst;
    
    OS_LockMutex( &_mServoCmd );
    aCMDPacket [0] = SERVO_CMD_GET_STATUS;
    aCMDPacket [1] = GETSTATUS;     //UDE GetStatus command

    bRst = _IssueNewCommand( COUNT_400_MSEC );

//        _bSRVStatus = aRETPacket[2];
#if 1
    if( bRst )
    {
        _bSRVStatus = aRETPacket[2];
    }
    else
    {
        _bSRVStatus = SERVO_STATUS_COMMAND_ERROR;
    }
#endif  // 0

    OS_UnlockMutex( &_mServoCmd );

    return bRst;
}

// ***********************************************************************
//  Function    :   SERVO_CheckTrayStatus
//  Description :   Used to check if cd/dvd-rom tray is closed
//  Arguments   :   None
//  Return      :   tray status
//  Side Effect :   
// ***********************************************************************
BYTE    SERVO_CheckTrayStatus( void ) 
{   
    BYTE    bRet;

    bRet = SRC_TRAY_UNKNOWN;    // ** TCH1.05; Temp give it.
    
    if( SERVO_GetStatus() )
    {
        _bSRVStatus &= SERVO_STATUS_TRAY;
    }
    else
    {
        return SRC_TRAY_UNKNOWN;    // ** TCH1.05; Temp give it.
    }

    // ** TCH1.05; begin... Convert it for current SrcFilter Module.
    switch ( _bSRVStatus )
    {
        case SERVO_TRAY_STATUS_OPENED:
            bRet= SRC_TRAY_OPENED;
            break;

        case SERVO_TRAY_STATUS_OPENING:
            bRet= SRC_TRAY_OPENING;
            break;

        case SERVO_TRAY_STATUS_CLOSED:
            bRet= SRC_TRAY_CLOSED;
            break;

        case SERVO_TRAY_STATUS_CLOSING:
            bRet= SRC_TRAY_CLOSING;
            break;

        case SERVO_TRAY_STATUS_USER_PUSH:
            bRet= SRC_TRAY_USER_PUSH;
            break;            
    }

// ** TCH1.05;     return _bSRVStatus;
    return bRet;
    // ** TCH1.05; end...     
}

// ***********************************************************************
//  Function    :   SERVO_GetDiscStatus
//  Description :   Used to check if there's title in dvd-rom.
//                  Will check if tray is closed first.
//  Arguments   :   pStatus : Not used
//  Return      :   SERVO_HAVEDISC : Have disc in tray
//                  SERVO_NODISC : No disc in tray
//  Side Effect :   If return SERVO_NODISC, maybe due to some command fail rather than really no disc.
// ***********************************************************************
/*--CoCo1.20, reduce code size.
#ifndef CSC_MASK
BYTE SERVO_GetDiscStatus(PBYTE pStatus)
{    
    return (SERVO_WaitStable());
}

BYTE SERVO_WaitStable(void)
{   
    SERVO_GetStatus();
        
    if ((_bSRVStatus & SERVO_STATUS_MEDIA) == SERVO_MEDIA_STATUS_PRESENT)
    {
#ifdef SUPPORT_PRINTF      
        printf ("Drive Ready !\n") ;
#endif  
        return SERVO_HAVEDISC;
    }
    //Disc is not present
    else if ((_bSRVStatus & SERVO_STATUS_MEDIA) == SERVO_MEDIA_STATUS_NOT_PRESENT)
    {
#if defined(SUPPORT_PRINTF) ||  defined(_DEBUG_INFO)
            printf ("\n Media not present, no disc detected !\n") ;
#endif                
        return SERVO_NODISC;    
    }            
    else if ((_bSRVStatus & SERVO_STATUS_MEDIA) == SERVO_MEDIA_STATUS_NOT_ANALYZED)
    {
#if defined(SUPPORT_PRINTF) ||  defined(_DEBUG_INFO)
            printf ("\n Media wrong !\n") ;
#endif                
        return SERVO_WRONGDISC;    
    }            
    
    //LJY1.25, Support opentray when servo fails to close tray.
#ifdef SUPPORT_OPENTRAY_WHEN_CLOSETRAY_FAILED    
    if((_bSRVStatus & SERVO_STATUS_TRAY) == SERVO_TRAY_STATUS_OPENING)
    {		
        // LLY2.37p, assign KEY_OPEN_CLOSE from F/W
        // To avoid IR and F/W key conflict issue
        //__bISRKey = KEY_OPEN_CLOSE;
        __bFWKey=KEY_OPEN_CLOSE;
        return SERVO_LOADDISC_FAILED;
    }
#endif  // #ifdef SUPPORT_OPENTRAY_WHEN_CLOSETRAY_FAILED

    return SERVO_NODISC; // Brian0.66
}
#endif  //CSC_MASK
*/

void    SERVO_InitialMutexFlag( void )
{
    OS_InitMutex( &_mServoAbort );
    OS_InitMutex( &_mServoCmd );
}


// Brian0.66
// for eCos909 only
BYTE    SERVO_GetMediaStatus( void )
{
    BYTE bMediaStatus;

    while( 1 )
    {
        bMediaStatus = SERVO_PollingMediaStatus();
        if( bMediaStatus != SRCFILTER_WAITING_STABLE )
        {
            break;
        }
    }

    return bMediaStatus;
    
}

BYTE    SERVO_ReadTOC( BYTE bTemp )
{
    BYTE bTrackNum, bLastSession;
    extern DWORD __dwSectorLastSession;
    
    // wyc0.95, need to initial __dwSectorLastSession to 0 when disc is DVD. Otherwise DVD will recognize wrong after playing multi-session disc.
    SERVO_GetDiscType( );
    if( _bSrvDiscType & DISC_TYPE_CD )
    {        
        bLastSession = _sessioninfo_TOC[0].bLastSession;
        bTrackNum = _sessioninfo_TOC[bLastSession-1].bFirstTrack;
        __dwSectorLastSession = COMUTL_MSFtoHSG( MAKE_MSF(_trk_TOC[bTrackNum-1].Min, _trk_TOC[bTrackNum-1].Sec, _trk_TOC[bTrackNum-1].Frm) );
    }

    // J500CSC_120, add for supporting DVD multi-session title
    else//if( _bSrvDiscType & DISC_TYPE_DVD )
    {
        bLastSession = _sessioninfo_TOC[0].bLastSession;                        //bLastSession save Session Noumber info
        __dwSectorLastSession = (DWORD) (_trk_TOC[bLastSession-1].Min<<16)      // LastSession start sector
                + (DWORD) (_trk_TOC[bLastSession-1].Sec<<8)
                + (DWORD) (_trk_TOC[bLastSession-1].Frm);
    }

    return TRUE;
}

BYTE    SERVO_PollingMediaStatus( void )
{
    SERVO_GetStatus( );

    if( _bSRVStatus == SERVO_STATUS_COMMAND_ERROR )
    {
        return SRCFILTER_WAITING_STABLE;
    }

    if( (_bSRVStatus & SERVO_STATUS_MEDIA) == SERVO_MEDIA_STATUS_PRESENT )
    {
#ifdef SUPPORT_PRINTF      
        printf ("Drive Ready !\n") ;
#endif  
        return SRCFILTER_HAVEDISC;
    }
    //Disc is not present
    else if( (_bSRVStatus & SERVO_STATUS_MEDIA) == SERVO_MEDIA_STATUS_NOT_PRESENT )
    {
#if defined(SUPPORT_PRINTF) ||  defined(_DEBUG_INFO)
        printf ("\n Media not present, no disc detected !\n") ;
#endif                
        return SRCFILTER_NODISC;    
    } 
    else if( (_bSRVStatus & SERVO_STATUS_MEDIA) == SERVO_MEDIA_STATUS_NOT_ANALYZED )
    {
#if defined(SUPPORT_PRINTF) ||  defined(_DEBUG_INFO)
        printf ("\n Media Wrong !\n") ;
#endif                
        return SRCFILTER_WRONGDISC;    
    } 
    //else if ((_bSRVStatus & SERVO_STATUS_MEDIA) == SERVO_MEDIA_STATUS_NOT_PRESENT)
    //{
        
   return SRCFILTER_WAITING_STABLE;
   // }

}

// LLY1.50, restore API since CELL module will use SrcFilter_GetCurrentPos() --> SERVO_GetCurrentPos().
// And, add the necessary code for this function.
BYTE    SERVO_GetCurrentPos( PDWORD pdwCurrentPos )
{
    *pdwCurrentPos = __SF_SourceGBL[0].dwWriteSector;    
    return TRUE;
}


/*--CoCo1.20, reduce code size.
BYTE    SERVO_CheckDiscEnding( void )
{
    return FALSE ;
}
*/

/*--CoCo1.20, reduce code size.
#ifndef CSC_MASK
BOOL    SERVO_DumpCookedSector ( BYTE * pBuf, WORD wLength, DWORD dwOffset )
{
    return TRUE;
}

BOOL    SERVO_DumpSector ( BYTE * pBuf, WORD wLength, DWORD dwOffset )
{
    return TRUE;
}
#endif  //CSC_MASK
*/

/*--CoCo1.20, reduce code size.
BYTE    SERVO_CSS_SentDiscKey( void )
{
	return TRUE;
}
*/

/*--CoCo1.20, reduce code size.
void    SERVO_AdjustThreshold ( BYTE bType, DWORD dwLevel )
{
    return;
}
*/

//BYTE SERVO_ReadTitleKey(DWORD dwSectorNumber, BYTE bStore)
//{
//    return TRUE;
//}

//BYTE SERVO_GetTrackEnd(BYTE bTrackNum, PDWORD pLength)
//{
//    return TRUE;
//}

BYTE    SERVO_SetTitleKey( BYTE bStore )
{
    PARSER_WriteTitleKey( _TitleKeyCopyInfoStore[bStore - 1].ulunTitleKey );
    return TRUE;
}

/*--CoCo1.20, reduce code size.
BYTE    SERVO_ResetRecover( BYTE bNeedStable )
{
    return FALSE;
}
*/

// ** TCH1.05; begin...  Below section is for SrcFilter.
// ==================   For SrcFilter   ===========================
//***************************************************************************
//  Function    :   ::SERVOFilter_GetStatus (void)
//  Abstract    :   Monitor Servo Status. May issue KEY_OPEN_CLOSE if needed.
//  Arguments   :
//  Return      :   NONE
//  Side Effect :   
//  Notes       :
//***************************************************************************
BYTE    SERVOFilter_GetStatus( void )
{
    if( SERVO_GetStatus() )
    {
        if( _bSRVStatus & SERVO_SPINDLE_STATUS_SPINNING )
        {
            return SRC_SPINDLE_STATUS_SPINNING;
        }
    }
//    else
//    {        
//    }

    return SRC_STATUS_NONE;
}
// ** TCH1.05; end... 

#ifdef SUPPORT_STB

static BYTE    __bDisableServo = FALSE;
//Aron
extern cyg_handle_t alarm_hdl;
extern cyg_handle_t Thread_SERVO;
extern cyg_handle_t Thread_CMDBM;
extern BYTE SRV_ServoInitial(void);
extern void SRVSV_MotorDriveOn(BYTE btOn);

VOID SERVO_Enable(VOID)
{
    if(__bDisableServo == TRUE)
    {
        SRV_ServoInitial();
        OS_EnableAlarm(alarm_hdl);  
        OS_ResumeThread(Thread_SERVO);
        OS_ResumeThread(Thread_CMDBM);                   
        __bDisableServo = FALSE;
    } 
}


VOID SERVO_Disable(VOID)
{
    DWORD dwTimeCheckStatus;

    if (__bDisableServo == FALSE)
    {
        
        if(__bSFSourcePWDown == TRUE)
        {
            SrcFilter_StartUnit();
            __bSFSourcePWDown = FALSE;
        }        
        
        if (__btDriveOpen)
        {
            __btDriveOpen = FALSE;
            SrcFilter_CloseTray();
            OSD_OUTPUT_MACRO ( MSG_OPEN, FALSE, 3 );
            dwTimeCheckStatus = OS_GetSysTimer() ;              
            while (OS_GetSysTimer() - dwTimeCheckStatus < COUNT_5_SEC)
            {
                if ( SrcFilter_CheckTrayStatus () == SRC_TRAY_CLOSED )
                {
                    break;
                }
                else
                {
                    OS_YieldThread();
                }
            }
        }

        SrcFilter_PowerMode (SERVO_POWER_STOP) ;  

        if (SERVO_PollingMediaStatus() != SRCFILTER_NODISC)
        {
            dwTimeCheckStatus = OS_GetSysTimer() ;  
            while (OS_GetSysTimer() - dwTimeCheckStatus < COUNT_10_SEC)
            {
                if (SERVOFilter_GetStatus () != SRC_SPINDLE_STATUS_SPINNING)
                {
                    break;
                }
                else
                {
                    //printf("Waiting Servo stop!!!\n");
                    OS_YieldThread();
                }
            }
        }

        if (SERVOFilter_GetStatus () == SRC_SPINDLE_STATUS_SPINNING)
        {
            printf("Err Servo spinning!!!\n");
        }
        //else
        {
            SRV_ServoInitial();
            OS_DisableAlarm(alarm_hdl);  
            OS_SuspendThread(Thread_SERVO);
            OS_SuspendThread(Thread_CMDBM);
            SRVSV_MotorDriveOn(FALSE);
            __bDisableServo = TRUE;
        }
    }
}
#endif

BYTE    _IssueNewCommand( DWORD dwTimeOut )
{
    DWORD    flagRet;
    static volatile DWORD dwTmp;

    dwTmp = dwTimeOut;

    OS_ClearFlag( &servo_flag_var, ~SERVO_CMD_FINISHED );

    OS_SetFlag( &servo_flag_var, SERVO_NEW_CMD );
    flagRet = OS_TimedWaitFlag( &servo_flag_var, SERVO_CMD_FINISHED,
            FLAG_WAITMODE_OR,  dwTimeOut );
    OS_ClearFlag( &servo_flag_var, ~SERVO_CMD_FINISHED );

    if( (flagRet != 0) && (aRETPacket[1] == NO_ERROR) )
    {
        return 1;
    }
    else
    {
        asm volatile ("nop");
    }

    return 0;       // indicate that Time-out happened or command error
}

#else
//The following 2 functions are for parser usage.
void SRVIF_ParserUpdateSERVO(DWORD dwBuf,DWORD dwReadAddr, DWORD dwReadSector)
{

}

WORD SRVIF_ParserGetEDCInfo(DWORD dwStreamIdx ,DWORD dwDumpInBaseAddr,DWORD dwSectorIdx)
{
    return 0;
}

#endif //SUPPORT_SERVO_SOURCE //++CoCo2.37p
