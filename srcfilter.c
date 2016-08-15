
// The file "winav.h" must be included in first line
#include "winav.h"
#include "ctkav.h"
#include <string.h>

#include "cc.h"
#include "utl.h"
#include "SrcFilter.h"
#include "comutl.h"
#include "parser.h" // LLY0.81
#include "atapi.h"
#include "servo.h"
#include "srcfilter.h"
#include "infofilter.h"
// wyc0.85, add include initial.h
#include "initial.h"
#ifdef SUPPORT_USB_SOURCE
#include "usbsrc.h"
#endif //
#include "media.h"

SOURCE_FILTER_INPUT_STREAM_BUFFER   __SF_SourceGBL[MAX_STREAM_COUNT];    // declare infortmation storage for each input stream
// wyc0.60-909, extern this variable here.
extern  FLAG_T  __fATAPIFlagComm;
extern DWORD   _dwUSBSRCWriteCluster[MAX_STREAM_COUNT];
extern DWORD   __dwUSBSRCStreamId;

// elmer2.31
#ifdef SUPPORT_USB_SOURCE
DWORD __dwUSBStartPos;
#endif
// Brian0.65
DWORD   __dwSFStreamID = 0;
DWORD   __dwSFDirRead;
#define GET_SOURCE_ID(STREAM_ID)  __SF_SourceGBL[STREAM_ID].bSourceIndex
// wyc1.06-909, represent source is power down mode
BYTE __bSFSourcePWDown;
// wyc2.51-909P, variable to provide JPG encode address.
#ifdef SUPPORT_ENCODE_JPG_PICTURE
DWORD   __dwSFSPIStartAddr, __dwSFSPIEndAddr;
#endif //
// Brian0.66, move to Winav.h
//#define SUPPORT_ATAPI_SOURCE
//#define SUPPORT_SERVO_SOURCE

// Brian0.69
void SrcFilter_InitVariables(void)
{
#ifdef SUPPORT_SERVO_SOURCE
    SERVO_InitialMutexFlag(); 
__SF_SourceGBL[0].bSourceIndex = SOURCE_SERVO;
#endif


}

void SrcFilter_StartUnit(void)
{
#ifdef SUPPORT_SERVO_SOURCE
    SERVO_StartUnit();
#endif


}

BYTE SrcFilter_Reset(BYTE bForceReset)
{
    BYTE bResult = FALSE;
    // wyc1.01-909, initial this value
    __dwSFStreamID = 0;
    /*switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_Reset(bForceReset);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_Reset();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }*/
    // wyc0.67, need to reset all source devide when devices are detected.
#ifdef SUPPORT_ATAPI_SOURCE
    bResult = ATAPI_Reset(bForceReset);
#endif //
#ifdef SUPPORT_SERVO_SOURCE
            //bResult = SERVO_Reset(); //Brian1.15  // CoCo, reduce code size.
#endif

#ifdef SUPPORT_USB_SOURCE
    //bResult = USBSRC_Reset(); //Brian1.15  // CoCo, reduce code size.
    return TRUE; //Brian1.15 //CoCo, reduce code size.
#endif
    return bResult;
}

BYTE SrcFilter_Init()
{
    BYTE bResult = FALSE;
    
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_Init();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = TRUE;
            
            //bResult = SERVO_Init();
            // do default buffer config here?? 
            //SrcFilter_ConfigBuffer(0, DS_INPUT_BITBUF_ST, 6); // default use all 6 blocks
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }
    return bResult;
}



/*
// code size is larger
BYTE SrcFilter_Init2()
{
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
        case SOURCE_ATAPI:
        if (ATAPI_Init())
            return TRUE;
        else
            return FALSE;
        break;
        case SOURCE_SERVO:
            //bResult = SERVO_Init();
            break;
        default:
            //printf{("Invalid Source\n");
    }
    return TRUE;
}

// code size is the largest
BYTE SrcFilter_Init3()
{
    
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
        case SOURCE_ATAPI:
            return ATAPI_Init();
            break;
        case SOURCE_SERVO:
            //bResult = SERVO_Init();
            return TRUE;
            break;
        default:
            //printf{("Invalid Source\n");
            return FALSE;
    }

}
*/

// wyc2.17a-909S, new function to config servo EDC buffer.
BYTE SrcFilter_CFGEDCBuff( DWORD dwStartAddr, WORD wSize )
{
    BYTE bResult = FALSE;

    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            //bResult = ATAPI_ConfigBuff();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_CFGEDCBuff( (ULONG_UNION)dwStartAddr,(WORD_UNION)wSize );
            break;
#endif
        default:
            //printf{("Invalid Source\n");
    }
    return bResult;
}

// wyc2.78-909P, change the WORd in bBufSize to avoid BYTE overlfow problem.
BYTE SrcFilter_ConfigBuffer(BYTE bId, DWORD dwBufStart, WORD bBufSize)
{
    BYTE bResult = FALSE;
    DWORD dwBufEnd;

    //LJY0.81, to sync the setting of parser and servo buffer.
    dwBufEnd = dwBufStart + (DWORD)(bBufSize * SERVO_BUFSIZE_ALIGN_UNIT);
    PARSER_SetBSBufferAddress(bId, dwBufStart, dwBufEnd);
    // wyc2.53-909P,
    PARSER_SetBSBufferAddress(1, dwBufStart, dwBufEnd);
    
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            //bResult = ATAPI_ConfigBuff();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_ConfigServoBuffer(bId, bBufSize, (ULONG_UNION) dwBufStart);
            break;
#endif
        default:
            //printf{("Invalid Source\n");
    }
    return bResult;
}


BYTE SrcFilter_OpenTray()
{
  
    BYTE bResult = FALSE;
    
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_OpenTray();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_OpenTray();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }
    return bResult;
}


BYTE SrcFilter_CloseTray()
{
    BYTE bResult = FALSE;
    
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_CloseTray();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_CloseTray();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }
    return bResult;
}

BYTE    SRCFilter_GetMediaStatus(void)
{
    // this function will be called only when SIMP_INITIAL is defined, for eCos909 only??
    // CheerDVD should never call this.
     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            return ATAPI_GetMediaStatus();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            return SERVO_GetMediaStatus();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
            
    }
        return SRCFILTER_NODISC;
}      

// wyc0.85, add function to abort GetMediaStatus
void    SRCFilter_AbortGetMediaStatus(void)
{
     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            ATAPI_ThreadDelete();
            INITIAL_ThreadInit(THREAD_ATAPI);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            //return SERVO_GetMediaStatus();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
            
    }
}

BYTE SrcFilter_AudioDeemphasis(WORD wPlayItem)
{
 

    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            return ATAPI_AudioDeemphasis(wPlayItem);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            //return SERVO_AudioDeemphasis(wPlayItem);
            break;
#endif

        default:
            //printf{("Invalid Source\n");
            
        }

        return SERVO_CDDA_NONE;
        
}

// wyc0.50-909, rename this function
BYTE SrcFilter_TriggerGettingMediaStatus(void)
{
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            OS_SetFlag(&__fATAPIFlagComm, SRCFILTER_WAIT_STABLE);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            // ??
            break;
#endif

        default:
            //printf{("Invalid Source\n");
            
        }

        return SRCFILTER_NODISC;
        
}

BYTE    SRCFilter_PollingMediaStatus(void)
{
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            return ATAPI_PollingMediaStatus();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            return SERVO_PollingMediaStatus();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
            
        }

        return SRCFILTER_NODISC;
        
}

BYTE SrcFilter_Stop(DWORD dwStream_id)
{
    BYTE bResult = FALSE;

    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_Stop();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            __SF_SourceGBL[__dwSFStreamID].bEnableRunning= FALSE;
            // ?? need to check the parameters
            bResult = SERVO_Abort(); 
            break;
#endif

#ifdef SUPPORT_USB_SOURCE
        case SOURCE_USB0_0:
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            //if (USBSRC_ExitMonitor())
            {
                bResult = USBSRC_Stop(dwStream_id, TRUE);           
                break;
            }
            /*
            else
            {
                bResult = FALSE;
                break;
            }
            */
#else
            bResult = USBSRC_Stop(dwStream_id, TRUE);
#endif
#endif

        default:
            //printf{("Invalid Source\n");
    }
    
    return bResult;
    
}

BYTE SrcFilter_ReadSectors(DWORD dwStream_id, DWORD dwStartPos, DWORD dwEndPos)
{
    BYTE bResult = FALSE;

    if (dwEndPos < dwStartPos)
        return FALSE;

    
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_ReadSectors(dwStartPos, dwEndPos);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
			
            //__SF_SourceGBL[__dwSFStreamID].dwWriteSector = dwStartPos; // Brian0.80, don't set dwWriteSector
            __SF_SourceGBL[__dwSFStreamID].dwStartSector = dwStartPos;
            __SF_SourceGBL[__dwSFStreamID].dwEndSector = dwEndPos;
    
            // ?? need to check the parameters
            bResult = SERVO_ReadData( (ULONG_UNION) dwStartPos, dwEndPos-dwStartPos+1, __dwSFStreamID );
            break;
#endif

#ifdef SUPPORT_USB_SOURCE
        case SOURCE_USB0_0:
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
            //if (USBSRC_ExitMonitor())
            {
                //USBSRC_Stop(dwStream_id, TRUE);
                if (__SF_SourceGBL[(MAX_STREAM_COUNT - 1) - dwStream_id].bEnableRunning == TRUE)
                {
                    //USBSRC_Stop((MAX_STREAM_COUNT - 1) - dwStream_id, FALSE);
                    bResult = USBSRC_ReadSectors(dwStream_id, dwStartPos, dwEndPos, FALSE);
                    __dwSFDirRead = FALSE;
                    
                }
                else
                {
                    //USBSRC_Stop(dwStream_id, FALSE);
                    bResult = USBSRC_ReadSectors(dwStream_id, dwStartPos, dwEndPos, TRUE);
                    __dwSFDirRead = TRUE;
                }
            }
            //else
            //{
            //    bResult = FALSE;
            //}
#else
            bResult = USBSRC_ReadSectors(dwStream_id, dwStartPos, dwEndPos, TRUE);
#endif // SUPPORT_JPEG_AUDIO_PLAYING
#endif // SUPPORT_USB_SOURCE

        // wyc2.51-909P, code to provide JPG encode address.
#ifdef SUPPORT_ENCODE_JPG_PICTURE
        case SOURCE_SPI:
            __dwSFSPIStartAddr = SRCFTR_SPI_ENCODE_ADDR + dwStartPos*2048;
            __dwSFSPIEndAddr = SRCFTR_SPI_ENCODE_ADDR + ((dwEndPos+1)*2048)-1;

            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }
    return bResult;
    
}

// wyc2.53-909P,
#ifdef SUPPORT_JPEG_AUDIO_PLAYING
BYTE SrcFilter_StreamPause(DWORD dwStream_id)
{
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
        case SOURCE_SERVO:
            break;
        case SOURCE_USB0_0:
            //USBSRC_ExitMonitor();
            USBSRC_Stop(dwStream_id, FALSE);
            break;
        default:
    }
    return TRUE;
}

BYTE SrcFilter_StreamContinue(DWORD dwStream_id)
{
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
        case SOURCE_SERVO:
            break;
        case SOURCE_USB0_0:
            //USBSRC_ExitMonitor();
            //USBSRC_Stop(dwStream_id, FALSE);
            USBSRC_EnableRunning(dwStream_id, TRUE);
            //USBSRC_IssueReadCmd(dwStream_id);
            break;
        default:
    }
    return TRUE;
}
#endif //

// LLY1.50, restore API since CELL module will use it.
BYTE SrcFilter_GetCurrentPos(PDWORD pCurrentPos)
{
    BYTE bResult = FALSE;

    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_GetCurrentPos(pCurrentPos);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_GetCurrentPos(pCurrentPos); 
            break;          
#endif
#ifdef SUPPORT_USB_SOURCE
        case SOURCE_USB0_0:
            bResult = USBSRC_GetCurrentPos(pCurrentPos);
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }
    
    return bResult;
}


BYTE SrcFilter_GetDiscInfo(PBYTE pMinTrack, PBYTE pMaxTrack, PDWORD pMaxSector)
{
    
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            ATAPI_GetDiscInfo(pMinTrack, pMaxTrack, pMaxSector);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            SERVO_GetDiscInfo(pMinTrack, pMaxTrack, pMaxSector);
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }


    return TRUE;
}

BYTE SrcFilter_GetTrackInfo(BYTE bTrackNum, PBYTE pTrackCtl, PDWORD pStartPos)
{
    BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_GetTrackInfo(bTrackNum, pTrackCtl, pStartPos);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_GetTrackInfo(bTrackNum, pTrackCtl, pStartPos);
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }


    return bResult;
}

BYTE SrcFilter_ReadTOC( BYTE bTemp)
{
    BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_ReadTOC(bTemp);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_ReadTOC(bTemp);
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return bResult;
    
}

/*--Brian1.15  // CoCo, reduce code size.
BYTE SrcFilter_Monitor()
{
    //if (__bSFSourceID == SOURCE_ATAPI)
    // {
    //     return TRUE;
    //     return ATAPI_Monitor();
    // }

    // do nothing, should remove this function
    return TRUE;
}
*/

/*Brian1.15  // CoCo, reduce code size.
BYTE SrcFilter_CheckDiscEnding()
{

     BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_CheckDiscEnding();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_CheckDiscEnding();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return bResult;
    
}
*/

/*Brian1.15  // CoCo, reduce code size.
//  **********************************************************
BYTE SrcFilter_ReadDiscKey()
{
     BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_ReadDiscKey();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_ReadDiscKey();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return bResult;
    
}
*/

WORD    SrcFilter_GetDiscType()
{

    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            return ATAPI_GetDiscType();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            return SERVO_GetDiscType();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return BOOK_UNKNOW;
    
}

BYTE SrcFilter_CheckTrayStatus(void)
{
     BYTE bResult = SRC_TRAY_CLOSED;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_CheckTrayStatus();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_CheckTrayStatus();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return bResult;
}


BOOL    SrcFilter_EnableRunning ( BYTE bGo )
{
    return FALSE;
}


// wyc1.06-909, support SERVO resume function from STOP mode.
BYTE SrcFilter_Seek (DWORD dwSeekLBA, BYTE  bResumeRead)
{
    BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_Seek(dwSeekLBA, bResumeRead);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_Seek( (ULONG_UNION) dwSeekLBA, bResumeRead );
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }
    // wyc1.06-909, seek will wake up source, so need to set __bSFSourcePWDown to FALSE.
    __bSFSourcePWDown = FALSE;

    return bResult;
}

BYTE SrcFilter_ReadTitleKey(DWORD dwSectorNumber, BYTE bStore)
{
     BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_ReadTitleKey(dwSectorNumber, bStore);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_ReadTitleKey(dwSectorNumber, bStore);
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return bResult;
    
}

BYTE SrcFilter_GetTrackEnd(BYTE bTrackNum, PDWORD pLength)
{
     BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_GetTrackEnd(bTrackNum, pLength);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_GetTrackEnd(bTrackNum, pLength);
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return bResult;
}

BYTE SrcFilter_SetTitleKey(BYTE bStore)
{
    BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_SetTitleKey(bStore);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_SetTitleKey(bStore);
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return bResult;
    
}
BYTE SrcFilter_ResetRecover (BYTE bNeedStable)
{
     BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_ResetRecover(bNeedStable);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            /* Brian1.15  // CoCo, reduce code size.
            bResult = SERVO_ResetRecover(bNeedStable);
            break;
            */
            return FALSE; //Brian1.15  // CoCo, reduce code size.
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return bResult;
    
}

BYTE SrcFilter_GetDiscLength(PDWORD pLength)
{
    BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_GetDiscLength(pLength);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_GetDiscLength(pLength);
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return bResult;
    
}

BYTE SrcFilter_Abort(void)
{
    BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_Abort();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_Abort();
            break;
#endif
        // wyc1.01-909, add USB source function in srcfilter.
#ifdef SUPPORT_USB_SOURCE
        case SOURCE_USB0_0:
            bResult = USBSRC_Abort();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return bResult;
}

BYTE SrcFilter_GetStatus(BYTE bSrcIndex)
{
    BYTE bResult = FALSE;

    if (bSrcIndex == 0xFF)
    {
        switch (GET_SOURCE_ID(__dwSFStreamID))
        {
#ifdef SUPPORT_ATAPI_SOURCE
            case SOURCE_ATAPI:
                bResult = ATAPI_GetStatus();
                break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
            case SOURCE_SERVO:
                bResult = SERVOFilter_GetStatus();
                break;
#endif

#ifdef SUPPORT_USB_SOURCE
            case SOURCE_USB0_0:
                bResult = USBSRC_GetUSBSRCStatus(SOURCE_USB0_0);
                break;
#endif
            default:
                //printf{("Invalid Source\n");
        }
    }
    else
    {
        switch (bSrcIndex)
        {
#ifdef SUPPORT_SERVO_SOURCE
            case SOURCE_SERVO:
                bResult = SERVOFilter_GetStatus();
                break;
#endif //
#ifdef SUPPORT_USB_SOURCE
            case SOURCE_USB0_0:
                bResult = USBSRC_GetUSBSRCStatus(SOURCE_USB0_0);
                break;
#endif //
            default:
                break;
        }
    }
    return bResult;
    
}

void SrcFilter_ClearState(void)
{
#ifdef SUPPORT_USB_SOURCE
        USBSRC_ClearUSBSRCState();
#endif
}

BYTE SrcFilter_PowerMode (BYTE bMode)
{
    BYTE bResult = FALSE;

     switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            bResult = ATAPI_PowerMode(bMode);
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            bResult = SERVO_PowerMode(bMode);
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }
    // wyc1.06-909, set to TRUE to represent SERVO stop mode.
    __bSFSourcePWDown = TRUE;

    return bResult;
}

void    SRCFilter_ThreadDelete(void)
{
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            ATAPI_ThreadDelete();
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            break;
#endif

#ifdef SUPPORT_USB_SOURCE
        case SOURCE_USB0_0:
            return USBSRC_ThreadDelete();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }
}

// F100CSC_102, BEGIN
// This function only turns on the flag when issue command to SERVO and ask SERVO to flush its CACHE
void    SRCFilter_EnableCacheFlush( void )
{
#ifdef SUPPORT_SERVO_SOURCE
    SERVO_EnableCacheFlush( );
#endif //
    return;
}
// F100CSC_102, END

// wyc1.05-909, add one functino for dump data from source.
// ***********************************************************************
//  Function    :   SRCFilter_DumpData
//  Description :   Dump the data to specific pointer address from source by calling USB function directly.
//  Arguments   :   dwSecNO: The dump location(logical location). dwNum: The dump sector num(Parser sector unit, 2048 bytes).
//              :   pFATTable: The dumped pointer.
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
BYTE SRCFilter_DumpData(DWORD dwSecNO, DWORD dwNum, DWORD* pFATBuff)
{
#ifdef SUPPORT_USB_SOURCE
    // wyc1.07-909, modify 3th parameter to DWORD* for support FAT32
    return USBSRC_DumpData(dwSecNO, dwNum, (DWORD*)(pFATBuff));
#endif //
    //return TRUE;
}

// wyc1.20-909, new function to dump data from USB source to read USB data.
// ***********************************************************************
//  Function    :   SRCFilter_DumpDatabySource
//  Description :   Dump the data to specific pointer address from source by wait USBSRC to call USB function to read data.
//  Arguments   :   dwSecNO: The dump location(logical location). dwNum: The dump sector num(Parser sector unit, 2048 bytes).
//              :   pFATTable: The dumped pointer.
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
BYTE SRCFilter_DumpDatabySource(DWORD dwSecNO, DWORD dwNum, DWORD* pFATBuff)
{
#ifdef SUPPORT_USB_SOURCE
    // wyc1.07-909, modify 3th parameter to DWORD* for support FAT32
    return USBSRC_DumpDatabySource(dwSecNO, dwNum, (DWORD*)(pFATBuff));
#endif //
    //return TRUE;
}

// wyc2.05-909S, add one new function to exit source mode.
// ***********************************************************************
//  Function    :   SRCFilter_ExitSource
//  Description :   Exit the source mode to avoid some H/W still active to affect other module.
//  Arguments   :   None
//  Return      :   TRUE/FALSE
//  Side Effect :
// ***********************************************************************
BYTE SRCFilter_ExitSource(void)
{
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_ATAPI_SOURCE
        case SOURCE_ATAPI:
            break;
#endif

#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            break;
#endif

#ifdef SUPPORT_USB_SOURCE
        case SOURCE_USB0_0:
            USBSRC_HCExit();
            break;
#endif

        default:
            //printf{("Invalid Source\n");
    }

    return TRUE;
}

// wyc2.37-909s, function to force usbsrc to exit running H/W functions.
BYTE SRCFilter_USBSRCExit(void)
{
    return USBSRC_SRCExit();
}

// Added by J500CSC_218, support F/W to update source ID index
BOOL    SrcFilter_ChangeSourceID( DWORD dwStreamIdx, BYTE bSourceIdx )
{
    if( dwStreamIdx >= MAX_STREAM_COUNT )
    {
        return FALSE;
    }

    __SF_SourceGBL[dwStreamIdx].bSourceIndex = bSourceIdx;

    return TRUE;
}

// J200Aron_231, to check source is ready or not
extern BYTE SRVIF_MpegCheckServo(void);
BOOL  SrcFilter_CheckSrcReady(VOID)
{
    switch (GET_SOURCE_ID(__dwSFStreamID))
    {
#ifdef SUPPORT_SERVO_SOURCE
        case SOURCE_SERVO:
            if (!SRVIF_MpegCheckServo())
            {
                return FALSE;
            }
            break;
#endif        
        default:
    }
    return TRUE;
}

BYTE SrcFilter_TriggerUSBSRCCmd(BYTE flag, DWORD dwCmd)
{
	return USBSRC_TriggerCmdAPI(flag, dwCmd);
}

BYTE SrcFilter_PeekUSBSRCCmd(BYTE flag, DWORD dwCmd)
{
	return USBSRC_PeekCmdAPI(flag, dwCmd);
}

void SrcFilter_ClearUSBSRCCmd(BYTE flag, DWORD dwCmd)
{
	USBSRC_ClearCmdAPI(flag, dwCmd);
}

//wyc2.78-909P, function to issue command for usbsrc to config the OTG mode.
void SrcFilter_SetOTGMode(BYTE bUSBOTGMode)
{
#if (USB_SYSTEM == USB2_0_JUNGO)
        USBSRC_SetOTGMode(bUSBOTGMode);
#endif //
}

BYTE SrcFilter_GetOTGMode(void)
{
#if (USB_SYSTEM == USB2_0_JUNGO)
        return USBSRC_GetOTGMode();
#endif //
}

