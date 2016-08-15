#include "winav.h"
#include "ctkav.h"
#include <stdarg.h>
#include "debug.h"
#include "cc.h"
#include "utl.h"
#include "hal.h"
#include "infofilter.h"
#include "cell.h"
#include "monitor.h"
#include "parser.h"
#include "comdec.h"
#include "Osd.h"


//The following definition is moved to Winav.h, Ben, May/05/2006
//#define AutoTuning_MPEG
#ifdef AutoTuning_MPEG
#include "servo\autotune.h"
#endif //#ifdef AutoTuning_MPEG


// wyc0.95, add include monitor.h
#include "monitor.h"
#include "aploader.h"
#include "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include "osdnd.h"

#define DBG_MINI_PRINTF
#ifndef DBG_MINI_PRINTF
#undef  SERIAL_DEBUG
#endif  // #ifndef DBG_MINI_PRINTF


#ifdef SERIAL_DEBUG
// Chuan2.16, We don't need define DBG_SOLUTION_DEPENDENCY for Component AP
#ifndef AP_FRAMEWORK
#define DBG_SOLUTION_DEPENDENCY
#endif  //#ifndef AP_FRAMEWORK
#endif

//The following definition is moved to Winav.h, YHCHEN, Mar/23/2007
//#define SUPPORT_OSDTIME_OUTPUT  
 

// wyc0.95, move variables from debug to monitor.

DWORD   __dwDebugFlag = (DBG_INFO_PRINTF | DBG_INFO_ASSERTION);

#ifdef SERIAL_DEBUG
//The variables for "serial debug tool"
BYTE    _bAutoUpgrade = FALSE;
BYTE    _bDBGRXReady = FALSE;
BYTE    _bDBGBinMode = FALSE;              // The flag to judge binar mode or text mode.
BYTE    _bDBGRXCommandData[DBG_RX_COMMAND_SIZE];
BYTE    _bDBGStartTextCmd = FALSE;
BYTE    _bDBGSystemDumpDRAMToFile = FALSE;
BYTE    _bDBGRXFlag = FALSE;               // TRUE=Rx one cmd end from RS232
BYTE    _bDBGChangeRxToRomBuffer = FALSE;  // TRUE=change rx buffer to rom buffer
BYTE    _bDBGROMDataBuf[DBG_RX_DATA_SIZE]; // RX data buffer, to receive the data when _bDBGChangeRxToRomBuffer = TRUE
BYTE    *_bpDBGROMBuf = _bDBGROMDataBuf;
BYTE    _bDBGBufWriteIndex = 0;            // RS232 Rx buf write index
BYTE    _bDBGBufReadIndex = 0;             // RS232 Rx buf read index
BYTE    _bDBGLen, _bDBGLen1;
BYTE    _bDBGRXCommandLen = 0;
BYTE    _bDBGCurrentCommand;
WORD    _wDBGROMdataBufIndex;              // index of buffer for reading and writing to flash
WORD    _wDBGROMSize;
DWORD   _dwDBGCmd1stParam, _dwDBGCmd2ndParam, _dwDBGCmd3rdParam;

#ifdef DBG_SOLUTION_DEPENDENCY
// WORD    aDBGUpgrade[] = {30, CHAR_S, CHAR_e, CHAR_r, CHAR_i, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_b, CHAR_u, CHAR_g, CHAR_T, CHAR_o, CHAR_o, CHAR_l, CHAR_COLON, CHAR_SPACE, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_U, CHAR_p, CHAR_g, CHAR_r, CHAR_a, CHAR_d, CHAR_e};
DWORD   _dwDBGUpgradeFileSize;
DWORD   _dwDebugFlagBackup;
#endif
#endif

PDBG_INFO   _pDBG_HeaderTmp;
PDBG_INFO   _pDBG_Header1;
PDBG_INFO   _pDBG_Header2;
WORD    _wInfoRIdx;
WORD    _wInfoWIdx;
BYTE    _bASCII[]= {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
BYTE    _bThreadID[][11] = {"[ATAPI ",
                            "[PARSER ",
                            "[DEC ",
                            "[SERVO ",
                            "[IFR ",
                            "[MONITOR ", // wyc0.95, modify name because original name is wrong.
                            "[DVD ",
                            "[ISR ",
                            "[DSR ",
                            "[DEBUG ",
                            "[USB ",
                            "[PROC2 "};
BYTE    _bMsgSkip[] = { "*** Skip Msg" };

//SYuan2.77-909P: for setting UART TX, RX independently
//_dwUartPort for TX port.
//__dwUartPort_RX for RX port.
DWORD   _dwUartPort = HAL_UART1_TX;
DWORD   _dwUartPort_RX = HAL_UART1_RX;
DWORD   _dwGlobalSaveInt;

BYTE    _bDBGEnable = FALSE;

//Internal functions
#ifdef DBG_SOLUTION_DEPENDENCY
VOID    _DBG_Monitor(VOID);
VOID    _DBG_ShowDebug (VOID);
#ifdef STACK_OVERFLOW_DETECTION
VOID    _DBG_Report_Stack(VOID);
#endif
#endif  // #ifdef DBG_SOLUTION_DEPENDENCY
#ifdef SERIAL_DEBUG
BYTE    _DBG_WaitComand(VOID);
VOID    _DBG_CheckRX(VOID);
WORD    _DBG_SearchCommand(BYTE bStart);
VOID    _DBG_AnalyzeCommand(BYTE bStart);
VOID    _DBG_CommandParse(VOID);
DWORD   _DBG_StrToHexVal(BYTE *pTemp, BYTE bNum);
VOID    _DBG_DumpDRAM(DWORD dwAddress, DWORD dwSize);
VOID    _DBG_DumpMem(DWORD dwAddress, BYTE bSizeType);
VOID    _DBG_DumpMemArray(DWORD dwAddress, BYTE bSizeType, DWORD dwNumOfEntry);
VOID    _DBG_ModifyMem(DWORD dwAddress, DWORD dwWriteValue, BYTE bOperation, BYTE bSizeType);
BYTE    _DBG_ReadAndCheckInBuffer(BYTE * bRetValue);
BYTE    _DBG_ReadInBuffer(VOID);
VOID    _DBG_ProcessBinCommand(VOID);
VOID    _DBG_ProcessWriteData(VOID);
VOID    _DBG_ProcessReadDRAM(VOID);
VOID    _DBG_ProcessWriteDRAM(VOID);
VOID    _DBG_EndCommand(BYTE bCurrentResult);
VOID    _DBG_ReSyncDebugLevel(VOID);
VOID    _DBG_DelayTime(DWORD dwDelayTime);
#endif  // #ifdef SERIAL_DEBUG
#ifdef DBG_MINI_PRINTF
VOID    _PutCHAR(DWORD dwThreadID, CHAR cChar);
VOID    _PutBYTE(DWORD dwThreadID, DWORD dwDWORD);
VOID    _PutWORD(DWORD dwThreadID, DWORD dwDWORD);
VOID    _PutDWORD(DWORD dwThreadID, DWORD dwDWORD);
VOID    _DBG_ResetDRAMDebugMessage(VOID);
#endif

//***************************************************************************
//  Function    :   DBG_Polling
//  Abstract    :   This function is polling the data in Rx buffer. Please add
//              :   it to your loop to pool the command.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   For Serial Debug used
//***************************************************************************
VOID    DBG_Polling(VOID)
{
#ifdef SERIAL_DEBUG
    if (_bDBGEnable == FALSE)
        return ;

    do {
#ifdef DBG_SOLUTION_DEPENDENCY
        // Chuan0.67, Call _DBG_Monitor() to monitor A/V/SP/PCM Rem and PROC2 PC
        _DBG_Monitor();
#endif  // #ifdef DBG_SOLUTION_DEPENDENCY

        if (!_bDBGBinMode)
            _DBG_CheckRX();
        else    //poling "serial debug tool"
        {
            DBG_INT();

            if (_bDBGRXFlag)
            {
                _bDBGRXFlag=FALSE;

                if (!_bDBGChangeRxToRomBuffer)
                {
                    _DBG_ProcessBinCommand();
                }
                else  //_bDBGChangeRxToRomBuffer==TRUE, finish received data to rom data buffer, then starting to write flash
                {
                    switch (_bDBGCurrentCommand)
                    {
                    case WRITE_DATA_CMD:
                        _DBG_ProcessWriteData();
                        break;
                    case READ_DRAM_CMD:
                        _DBG_ProcessReadDRAM();
                        break;
                    case WRITE_DRAM_CMD:
                        _DBG_ProcessWriteDRAM();
                        break;
                    }
                }
            }
        }
    } while (_bAutoUpgrade);

#endif  // #ifdef SERIAL_DEBUG
}

//**********************************************************************
//  Function    :   DBG_OutputDRAMData
//  Abstract    :   This function will output the data and Serial Debug Tool
//				:	will save it to a file.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************
VOID    DBG_OutputDRAMData(DWORD dwStartAddress, DWORD dwEndAddress, BYTE bFileName)
{
#ifdef SERIAL_DEBUG
    DWORD dwDRAMData;
    DWORD dwDRAMAddress = dwStartAddress;
    DWORD dwSize;
    DWORD dwIndex;

    if (_bDBGEnable == FALSE)
        return ;

#ifdef SUPPORT_WATCH_DOG
    HAL_WatchDog_Enable(FALSE);
#endif

    _bDBGSystemDumpDRAMToFile = TRUE;

    _PutCHAR(DBG_DEBUG_HI, START_DUMP_DRAM_DATA);
    _DBG_DelayTime(DBG_DELAY_TIME);
    _PutCHAR(DBG_DEBUG_HI,bFileName);
    _DBG_DelayTime(DBG_DELAY_TIME);

    //DWORD alignment.
    if (dwDRAMAddress % 4)
    {
        dwDRAMAddress = (dwDRAMAddress / 4)*4;
    }

    dwSize = (dwEndAddress - dwDRAMAddress)/4*4;

    //_DBG_OutputDWORDBinData(dwSize);
    _PutCHAR(DBG_DEBUG_HI, HIBYTE(HIWORD(dwSize)));
    _DBG_DelayTime(DBG_DELAY_TIME);
    _PutCHAR(DBG_DEBUG_HI, LOBYTE(HIWORD(dwSize)));
    _DBG_DelayTime(DBG_DELAY_TIME);
    _PutCHAR(DBG_DEBUG_HI, HIBYTE(LOWORD(dwSize)));
    _DBG_DelayTime(DBG_DELAY_TIME);
    _PutCHAR(DBG_DEBUG_HI, LOBYTE(LOWORD(dwSize)));
    _DBG_DelayTime(DBG_DELAY_TIME);

    for (dwIndex = 0; dwIndex < dwSize/4; dwIndex++)
    {
        dwDRAMData = *((volatile DWORD *)dwDRAMAddress);

        _PutCHAR(DBG_DEBUG_HI, HIBYTE(HIWORD(dwDRAMData)));
        _DBG_DelayTime(DBG_DELAY_TIME);
        _PutCHAR(DBG_DEBUG_HI, LOBYTE(HIWORD(dwDRAMData)));
        _DBG_DelayTime(DBG_DELAY_TIME);
        _PutCHAR(DBG_DEBUG_HI, HIBYTE(LOWORD(dwDRAMData)));
        _DBG_DelayTime(DBG_DELAY_TIME);
        _PutCHAR(DBG_DEBUG_HI, LOBYTE(LOWORD(dwDRAMData)));
        _DBG_DelayTime(DBG_DELAY_TIME);

        dwDRAMAddress += 4;
    }

    _bDBGSystemDumpDRAMToFile = FALSE;

#ifdef SUPPORT_WATCH_DOG
    HAL_WatchDog_Enable(TRUE);
#endif

#endif  // #ifdef SERIAL_DEBUG
}


//***************************************************************************
//  Function    :   DBG_Init
//  Abstract    :   This function will initial UART1/2 and Relative Variable
//                  for DBG Module.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    DBG_Init(DWORD dwSysCLK, DWORD dwBaudRate, DWORD dwUartPort, DWORD dwDBGMode)
{
#ifdef DBG_MINI_PRINTF
    _bDBGEnable = TRUE;
    _DBG_ResetDRAMDebugMessage();

    // Set BaudRate & Enable Uart Port (TX & RX)
    // SYuan2.77-909P: setting TX, RX
    switch(dwUartPort)
    {
        case(HAL_UART1_TX):            
            HAL_UART_Disable(HAL_UART1_TX);
            break;
        case(HAL_UART1_RX):            
            HAL_UART_Disable(HAL_UART1_RX);
            break;
        case(HAL_UART2_TX):
            HAL_UART_Disable(HAL_UART2_TX);
            break;
        case(HAL_UART2_RX):            
            HAL_UART_Disable(HAL_UART2_RX);
            break;
    }    
    HAL_UART_BaudRateSet(dwSysCLK, dwBaudRate, dwUartPort);
    //SYuan2.77-909P: setting TX, RX
    switch(dwUartPort)
    {
        case(HAL_UART1_TX):
            HAL_UART_Enable(HAL_UART1_TX,GET_UART1_TX_VALUE(dwDBGMode));
            break;
        case(HAL_UART1_RX):
            HAL_UART_Enable(HAL_UART1_RX,GET_UART1_RX_VALUE(dwDBGMode));
            break;
        case(HAL_UART2_TX):
            HAL_UART_Enable(HAL_UART2_TX,GET_UART2_TX_VALUE(dwDBGMode));
            break;
        case(HAL_UART2_RX):
            HAL_UART_Enable(HAL_UART2_RX,GET_UART2_RX_VALUE(dwDBGMode));
            break;
    }        
#ifdef SERIAL_DEBUG
    _DBG_ReSyncDebugLevel();
#endif  // #ifdef SERIAL_DEBUG
    //_dwUartPort = dwUartPort;
    if ( (dwUartPort == HAL_UART1_TX)||(dwUartPort == HAL_UART2_TX))
        _dwUartPort = dwUartPort;
    
    if ( (dwUartPort == HAL_UART1_RX)||(dwUartPort == HAL_UART2_RX))
        _dwUartPort_RX = dwUartPort;
#else
    //HAL_UART_Disable(bUartPort);
    // SYuan2.77-909P: setting TX, RX
    switch(dwUartPort)
    {
        case(HAL_UART1_TX):            
            HAL_UART_Disable(HAL_UART1_TX);            
            break;
        case(HAL_UART1_RX):
            HAL_UART_Disable(HAL_UART1_RX);            
            break;
        case(HAL_UART2_TX):
            HAL_UART_Disable(HAL_UART2_TX);            
            break;
        case(HAL_UART2_RX):
            HAL_UART_Disable(HAL_UART2_RX);            
            break;
    }   
#endif  // #ifdef DBG_MINI_PRINTF
}

/****************************************************************************
 ** Function:           DBG_Printf
 ** Description:        mini printf versionm
 ** Parameters:         the same as printf
 ** Return:             none.
 ** Note:               Only support the following type:
 **                       WORD: d, i, o, u, x, X ==> all are output in Hex
 **                       DWORD: ld, li, lo, lu, lx, lX ==> all are output in Hex
 **                       BYTE: hd, hi, ho, hu, hx, hX ==> all are output in Hex
 **                       char: c
 **                       string: s
 **                     flag, width, precision will be ignored
 ***************************************************************************/
BOOL    DBG_Printf(DWORD dwThreadID, DWORD dwDBGType, char *fmt, ...)
{
#ifdef DBG_MINI_PRINTF
    va_list ap;
    CHAR *p, *sVal;
    DWORD dwBYTE, dwDWORD, dwPercent;
    DWORD dwVal;
    DWORD dwSaveInt;
    BOOL  blCr=FALSE;
//    WORD wVal;
//    BYTE bVal;
//    CHAR cVal;
#ifdef SUPPORT_OSDTIME_OUTPUT
    static DWORD dwThreadTime[12];
#endif


    if (_bDBGEnable == FALSE)
        return TRUE;


#ifdef DBG_SOLUTION_DEPENDENCY

#ifdef SUPPORT_OSDTIME_OUTPUT
    __dwDebugFlag &= ~(DBG_INFO_PRINTF);
    __dwDebugFlag |= DBG_INFO_ASSERTION;
#endif

    if ((dwDBGType & __dwDebugFlag) == FALSE)
        return FALSE;
#endif  // #ifdef DBG_SOLUTION_DEPENDENCY

    HAL_DISABLE_INTERRUPTS( dwSaveInt );

#ifdef SUPPORT_OSDTIME_OUTPUT
    __dwDebugFlag &= ~(DBG_INFO_PRINTF);
    __dwDebugFlag |= DBG_INFO_ASSERTION;

    if ((dwThreadTime[dwThreadID&0xFF] == gcShowStatus.dwCurrentTime) &&
        (dwDBGType & DBG_INFO_ASSERTION))
    {
        HAL_RESTORE_INTERRUPTS( dwSaveInt );
        return TRUE;
    }
    
    dwThreadTime[dwThreadID&0xFF] = gcShowStatus.dwCurrentTime;
#endif


    if ((dwThreadID & DBG_PRIORITY_HIGH) == FALSE)
    {
        // For Normal Message.
        _pDBG_HeaderTmp = _pDBG_Header1 + _wInfoWIdx;
        if (++_wInfoWIdx >= DBG_MAX_IDX)
        {
            _wInfoWIdx = 0;
        }

        if (_wInfoWIdx == _wInfoRIdx)
        {
            DWORD dwSkipTimeNow;
            static DWORD dwSkipTime = 0x0;

            // Chuan0.67, Skip message will display again per 1 SEC.
            dwSkipTimeNow = OS_GetSysTimer();
            if (dwSkipTimeNow >= (dwSkipTime + COUNT_1_SEC))
            {
                BYTE bIdx;

                dwSkipTime = dwSkipTimeNow;
                for (bIdx = 0; _bMsgSkip[bIdx]; bIdx++)
                {
                    _PutCHAR(DBG_DEBUG_HI,  _bMsgSkip[bIdx]);
                }

                _PutCHAR(DBG_DEBUG_HI, 0xa);
                _PutCHAR(DBG_DEBUG_HI, 0xd);
            }
            if (_wInfoWIdx == 0)
            {
                _wInfoWIdx = (DBG_MAX_IDX-1);
            }
            else
            {
                _wInfoWIdx--;
            }

            HAL_RESTORE_INTERRUPTS( dwSaveInt );
            return FALSE;
        }

        _pDBG_HeaderTmp->dwTickTime = OS_GetSysTimer();
        _pDBG_HeaderTmp->bThreadID = dwThreadID;

        (_pDBG_Header1+_wInfoWIdx)->bSize = 0;
#ifdef SUPPORT_OSDTIME_OUTPUT
{
        BYTE bHour, bMin, bSec;
        // LLY2.53, expand OSD time unit from WORD to DWORD
        /*
        bSec = LOBYTE(__dwOSDTime);
        bMin = HIBYTE(__dwOSDTime) % 60;
        bHour = HIBYTE(__dwOSDTime) / 60;
        */
        bSec = (BYTE)(LOWORD(gcShowStatus.dwCurrentTime));
        bMin = HIBYTE(LOWORD(gcShowStatus.dwCurrentTime));
        bHour = (BYTE)HIWORD(gcShowStatus.dwCurrentTime);

        _PutCHAR(dwThreadID, _bASCII[(bHour / 10) & 0x0f]);
        _PutCHAR(dwThreadID, _bASCII[(bHour % 10) & 0x0f]);
        _PutCHAR(dwThreadID, ':');
        _PutCHAR(dwThreadID, _bASCII[(bMin / 10) & 0x0f]);
        _PutCHAR(dwThreadID, _bASCII[(bMin % 10) & 0x0f]);
        _PutCHAR(dwThreadID, ':');
        _PutCHAR(dwThreadID, _bASCII[(bSec / 10) & 0x0f]);
        _PutCHAR(dwThreadID, _bASCII[(bSec % 10) & 0x0f]);
        _PutCHAR(dwThreadID, ')');
}
#endif

    }

    va_start(ap, fmt);  // make ap point to 1st unnamed arg
    dwBYTE = FALSE;
    dwDWORD = FALSE;
    dwPercent = FALSE;
    while(*fmt == 0x0a || *fmt == 0x0d)
    {
        fmt++;
    }

    for(p = fmt; *p; p++)
    {
        blCr = FALSE;
        if ((*p != '%') && (dwPercent == FALSE))
        {
            //if (*p != 0x0a && *p != 0x0d)
            {
                _PutCHAR(dwThreadID, *p);
            }
            if (*p == 0x0a) // new line
            {
                _PutCHAR(dwThreadID, 0x0d); // send an extra CR
                blCr = TRUE;
            }
            continue;
        }
        else if (dwPercent == TRUE)
        {
            p--;
        }
        else
        {
            dwPercent = TRUE;
        }

        switch(*++p)
        {
        case 'd':
        case 'i':
        case 'o':
        case 'u':
        case 'x':
        case 'X':
            dwVal = va_arg( ap, DWORD );

            if (dwDWORD)
            {
//                dwVal = va_arg( ap, DWORD );
                _PutDWORD(dwThreadID, dwVal);
            }
            else if (dwBYTE)
            {
//                bVal = va_arg( ap, BYTE );
                _PutBYTE(dwThreadID, dwVal);
             }
            else // WORD
            {
//                wVal = va_arg( ap, WORD );
                _PutWORD(dwThreadID, dwVal);
            }
            break;
        case 'c':
             dwVal = va_arg( ap, DWORD );
            _PutCHAR(dwThreadID, (CHAR)dwVal);
            break;
        case 's':
             for (sVal=va_arg(ap, PCHAR ); *sVal; sVal++)
            {
                _PutCHAR(dwThreadID, *sVal);
            }
            break;
        case 'l':
            dwDWORD = TRUE;
            continue;
        case 'h':
            dwBYTE = TRUE;
            continue;
        case '%':
            _PutCHAR(dwThreadID, '%');
            break;
        default:
            continue;
        }
        dwPercent = FALSE;
        dwDWORD = FALSE;
        dwBYTE = FALSE;
    }
    va_end(ap);

    if ((dwThreadID & DBG_PRIORITY_HIGH) == FALSE)
    {
        // Chuan1.10, avoid message over 90 chars.
        if (_pDBG_HeaderTmp->bSize > (DBG_CHAR_LENGTH-2))
        {
            _pDBG_HeaderTmp->bSize = (DBG_CHAR_LENGTH-2);
            blCr = FALSE;
        }
    }

    if (blCr == FALSE)
    {
        _PutCHAR(dwThreadID, 0xa);
        _PutCHAR(dwThreadID, 0xd);
    }
    HAL_RESTORE_INTERRUPTS( dwSaveInt );
#endif  // #ifdef DBG_MINI_PRINTF
    return TRUE;
}

//**********************************************************************
//  Function    :   DBG_INT
//  Abstract    :   This function will send 2 chars per called.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************
VOID    DBG_INT(VOID)
{
#ifdef DBG_MINI_PRINTF
    static BYTE bOutIdx;
#ifdef SUPPORT_PROC2_DBG
    static BOOL blPROC2 = FALSE;
#endif  // #ifdef SUPPORT_PROC2_DBG
    DWORD dwSaveInt;
    BYTE bReceiveRX;

    if (_bDBGEnable == FALSE)
        return ;

    HAL_DISABLE_INTERRUPTS( dwSaveInt );

#ifdef SERIAL_DEBUG
    bReceiveRX = _DBG_WaitComand();

    // J300/Ben, Aug/15/2005
    #ifdef AutoTuning_MPEG
    if (AutoTuning_Enable_MPEG == TRUE)
    {
        //printf("kenny entry point");
        bReceiveRX = AT_CODE_Interpreter();

        if (AutoTuning_Enable_MPEG == FALSE)
        {
           HAL_RESTORE_INTERRUPTS( dwSaveInt );
           return ;	// AutoTuning Ends.
        }
    }
    #endif //#ifdef AutoTuning_MPEG

    if (bReceiveRX || _bDBGStartTextCmd || _bDBGSystemDumpDRAMToFile)
    {
        HAL_RESTORE_INTERRUPTS( dwSaveInt );
        return ;
    }
#endif  // #ifdef SERIAL_DEBUG

#ifdef SUPPORT_PROC2_DBG
    if (blPROC2)
        _pDBG_HeaderTmp = _pDBG_Header2 + REG_DBG_R_IDX;
    else
#endif  // #ifdef SUPPORT_PROC2_DBG
    _pDBG_HeaderTmp = _pDBG_Header1 + _wInfoRIdx;

    if (_pDBG_HeaderTmp->bSize != 0)
    {
        if ((_pDBG_HeaderTmp->bSize > (DBG_CHAR_LENGTH+6)) ||
            ((_pDBG_HeaderTmp->bThreadID > 0xB) && (_pDBG_HeaderTmp->bThreadID != DBG_THREAD_NULL)))
        {
            // Chuan1.10, reset DBG message.
            _DBG_ResetDRAMDebugMessage();
            _pDBG_HeaderTmp->bSize = 0;
            _pDBG_HeaderTmp->bThreadID = DBG_THREAD_NULL;
        }
        else if (_pDBG_HeaderTmp->bThreadID != DBG_THREAD_NULL)
        {
            BYTE bIdx;
            for (bIdx = 0; _bThreadID[_pDBG_HeaderTmp->bThreadID][bIdx]; bIdx++)
            {
                _PutCHAR(DBG_DEBUG_HI, _bThreadID[_pDBG_HeaderTmp->bThreadID][bIdx]);
            }
            _pDBG_HeaderTmp->bThreadID = DBG_THREAD_NULL;
            bOutIdx = 0;
        }
        else if (_pDBG_HeaderTmp->dwTickTime != 0xFFFFFFFF)
        {
            _PutDWORD(DBG_DEBUG_HI, _pDBG_HeaderTmp->dwTickTime);
            _PutCHAR(DBG_DEBUG_HI, ']');
            _pDBG_HeaderTmp->dwTickTime = 0xFFFFFFFF;
        }
        else
        {
            BYTE bIdx;
            bIdx = 2;
            do
            {
                _PutCHAR(DBG_DEBUG_HI,_pDBG_HeaderTmp->bData[bOutIdx++]);
                _pDBG_HeaderTmp->bSize--;

                if (_pDBG_HeaderTmp->bSize == 0)
                {
#ifdef SUPPORT_PROC2_DBG
                    if (blPROC2)
                    {
                        if (++REG_DBG_R_IDX >= DBG_MAX_IDX2)
                        {
                            REG_DBG_R_IDX = 0;
                        }
                    }
                    else
#endif  // #ifdef SUPPORT_PROC2_DBG
                    {
                    if (++_wInfoRIdx >= DBG_MAX_IDX)
                    {
                        _wInfoRIdx = 0;
                    }
                    }
                    break;
                }
            } while (--bIdx != 0);
        }
    }
#ifdef DBG_SOLUTION_DEPENDENCY
    else
    {
        if ((_bDBGRXReady == TRUE) && (_bDBGRXCommandData[0] == 'I'))
        {
            _DBG_ShowDebug();
            _bDBGRXCommandLen=0;
            _bDBGRXReady=FALSE;
        }
    }
#endif  // #ifdef DBG_SOLUTION_DEPENDENCY

#ifdef SUPPORT_PROC2_DBG
    if (_pDBG_HeaderTmp->bSize == 0)
    {
        if (blPROC2)
        {
            blPROC2 = FALSE;
        }
        else
        {
            BYTE bSize;
            _pDBG_HeaderTmp = _pDBG_Header2 + REG_DBG_R_IDX;
            asm volatile ("lduba [%1] 0x7, %0;" : "=r" (bSize) : "r" (&(_pDBG_HeaderTmp->bSize)));
            if (bSize != 0)
            {
                DWORD dwTickTime;
                BYTE bIdx, bTmp;
                asm volatile ("lda [%1] 0x7, %0;" : "=r" (dwTickTime) : "r" (&(_pDBG_HeaderTmp->dwTickTime)));
                for(bIdx = 0; bIdx < bSize; bIdx++)
                {
                    asm volatile ("lduba [%1] 0x7, %0;" : "=r" (bTmp) : "r" (&(_pDBG_HeaderTmp->bData[bIdx])));
                }
                blPROC2 = TRUE;
            }
        }
    }
#endif  // #ifdef SUPPORT_PROC2_DBG

    HAL_RESTORE_INTERRUPTS( dwSaveInt );
#endif  // #ifdef DBG_MINI_PRINTF
}

//**********************************************************************
//  Function    :   DBG_Disable
//  Abstract    :   This function will disable DBG module.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************
VOID    DBG_Disable(VOID)
{
    _bDBGEnable = FALSE;
}

#ifdef DBG_MINI_PRINTF
//**********************************************************************
//  Function    :   _PutCHAR
//  Abstract    :   This function will send CHAR to UART 1
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************
VOID    _PutCHAR(DWORD dwThreadID, CHAR cChar)
{
    if (dwThreadID & DBG_PRIORITY_HIGH)
    {
        HAL_UART_SendChar(cChar, _dwUartPort);
    }
    else
    {
        _pDBG_HeaderTmp->bData[_pDBG_HeaderTmp->bSize++] = (BYTE)cChar;
    }
}

//**********************************************************************
//  Function    :   _PutBYTE
//  Abstract    :   This function will send BYTE(2 Chars) UART 1
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************
VOID    _PutBYTE(DWORD dwThreadID, DWORD dwDWORD)
{
    _PutCHAR(dwThreadID, _bASCII[(dwDWORD >> 4) & 0x0f]);
    _PutCHAR(dwThreadID, _bASCII[ dwDWORD       & 0x0f]);
}

//**********************************************************************
//  Function    :   _PutWORD
//  Abstract    :   This function will send WORD(4 Chars) to UART 1
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************
VOID    _PutWORD(DWORD dwThreadID, DWORD dwDWORD)
{
    _PutBYTE(dwThreadID, (BYTE)(dwDWORD >> 8));
    _PutBYTE(dwThreadID, (BYTE)dwDWORD);
}

//**********************************************************************
//  Function    :   _PutDWORD
//  Abstract    :   This function will send DWORD(8 Chars) to UART 1
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************
VOID    _PutDWORD(DWORD dwThreadID, DWORD dwDWORD)
{
    _PutBYTE(dwThreadID, (BYTE)(dwDWORD >> 24));
    _PutBYTE(dwThreadID, (BYTE)(dwDWORD >> 16));
    _PutBYTE(dwThreadID, (BYTE)(dwDWORD >> 8));
    _PutBYTE(dwThreadID, (BYTE)dwDWORD);
}

//**********************************************************************
//  Function    :   _DBG_ResetDRAMDebugMessage
//  Abstract    :   This function will reset the debugging message in DRAM.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************
VOID    _DBG_ResetDRAMDebugMessage(VOID)
{
    // Reset for PROC1
    _pDBG_Header1 = (PDBG_INFO)DS_DBG_PROC1_AREA_ST;
    _pDBG_Header1->bSize = DBG_INFO_NULL;

    _wInfoRIdx = 0;
    _wInfoWIdx = 0;

#ifdef SUPPORT_PROC2_DBG
    // Reset for PROC2
    _pDBG_Header2 = (PDBG_INFO)DS_DBG_PROC2_AREA_ST;
    _pDBG_Header2->bSize = DBG_INFO_NULL;

    REG_DBG_R_IDX = 0;
    HAL_WriteAM(HAL_AM_DBGBUF_ADR, DS_DBG_PROC2_AREA_ST);
    HAL_WriteAM(HAL_AM_DBGBUF_IDX, DBG_MAX_IDX2);
#endif  // #ifdef SUPPORT_PROC2_DBG
}

#endif  // #ifdef DBG_MINI_PRINTF

#ifdef DBG_SOLUTION_DEPENDENCY
//***************************************************************************
//  Function    :   _DBG_Monitor
//  Abstract    :   This function will send out debug information
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_Monitor(VOID)
{
#ifdef SERIAL_DEBUG
    static DWORD dwTime = 0x0;
    DWORD dwTimeNow;

    dwTimeNow = OS_GetSysTimer();
    if (dwTimeNow < (dwTime + COUNT_50_MSEC))
        return;

    dwTime = dwTimeNow;
    if (DBG_INFO_AVREM & __dwDebugFlag)
    {
        // Chuan0.67, Monitor AV-Rem
        DBG_Printf(DBG_DEBUG, DBG_INFO_AVREM, "V: %lx, A0: %lx, R: %lx, W: %lx",
            MCU_VIDEO_BS_BUF_REMAINDER,
            MCU_A0_BS_BUF_REMAINDER,
            __SF_SourceGBL[0].dwReadSector, __SF_SourceGBL[0].dwWriteSector);
    }

    if (DBG_INFO_SPREM & __dwDebugFlag)
    {
        // Chuan0.67, Monitor SP-Rem
        DBG_Printf(DBG_DEBUG, DBG_INFO_SPREM, "SP1: %lx, SP2: %lx",
            MCU_SP1_BS_BUF_REMAINDER, MCU_SP2_BS_BUF_REMAINDER);
    }

    if (DBG_INFO_PCMREM & __dwDebugFlag)
    {
        // Chuan0.67, Monitor PCM-Rem/SPDIF-Rem
        DBG_Printf(DBG_DEBUG, DBG_INFO_PCMREM, "PCM: %lx, SPDIF: %lx",
            MCU_PCM_BUF_REMAINDER, MCU_SPDIF_BUF_REMAINDER);
    }

    if (DBG_INFO_PROC2_PC & __dwDebugFlag)
    {
        DWORD dwPC;
        MACRO_PLAT_GET_PROC2_PC( dwPC );
        DBG_Printf(DBG_DEBUG, DBG_INFO_PROC2_PC, "PROC2 PC: %lx", dwPC);
    }
#endif  // #ifdef SERIAL_DEBUG
}

//***************************************************************************
//  Function    :   _DBG_ShowDebug
//  Abstract    :   This function will send out debug information by Serial Debug Tool
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_ShowDebug (VOID)
{
#ifdef SERIAL_DEBUG
    DWORD   dwTmp, dwTmp1, dwTmp2;

#ifdef  RELEASE_SETTING
#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_16)
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\n !!! [REL-16M]All information below are output by HEX format !!!");
#elif (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_32)
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\n !!! [REL-32M]All information below are output by HEX format !!!");
#elif (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_64)
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\n !!! [REL-64M]All information below are output by HEX format !!!");
#endif
#else
#if (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_16)
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\n !!! [DBG-16M]All information below are output by HEX format !!!");
#elif (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_32)
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\n !!! [DBG-32M]All information below are output by HEX format !!!");
#elif (DRAM_CONFIGURATION_TYPE == DRAM_SIZE_64)
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\n !!! [DBG-64M]All information below are output by HEX format !!!");
#endif
#endif  // #ifdef  RELEASE_SETTING

    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\n<<<< Buffer/Fifo Remainder Status >>>>");
    HAL_ReadInfo(HAL_INFO_VIDEO_REMAIN, &dwTmp);
    HAL_ReadInfo(HAL_INFO_AUDIO1_REMAIN, &dwTmp1);
    HAL_ReadInfo(HAL_INFO_AUDIO2_REMAIN, &dwTmp2);
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\nV-REM: %lx, A0-REM: %lx, A1-REM: %lx", dwTmp, dwTmp1, dwTmp2);
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\nSP1-REM: %lx, SP2-REM: %lx", MCU_SP1_BS_BUF_REMAINDER, MCU_SP2_BS_BUF_REMAINDER);
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\nPCM: %lx, SPDIF: %lx, SPDIF-OUT: %lx", MCU_PCM_BUF_REMAINDER, MCU_PCM0_BUF_REMAINDER, MCU_SPDIF_BUF_REMAINDER);
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\nVLD-REM: %hx, UPK0-REM: %hx, UPK1-REM: %hx", MCU_VLD_FIFO_REMAINDER, MCU_AIU_UNPACK0_FIFO_REMAINDER, MCU_AIU_UNPACK1_FIFO_REMAINDER);

    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\n<<<< Channel Address >>>>");
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\nVLD: %lx(%lx, %lx) ", REG_MCU_DCR01, REG_MCU_DCR03, REG_MCU_DCR02);
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\nUPK0:%lx(%lx,%lx), UPK1:%lx(%lx,%lx)", REG_MCU_ACR09, REG_MCU_ACR0B, REG_MCU_ACR0A, REG_MCU_ACR11, REG_MCU_ACR13, REG_MCU_ACR12);
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\nPCM:%lx(%lx,%lx), SPDIF:%lx(%lx,%lx)", REG_MCU_ACR19, REG_MCU_ACR1B, REG_MCU_ACR1A, REG_MCU_ACR21, REG_MCU_ACR23, REG_MCU_ACR22);

    MACRO_PLAT_GET_PROC2_PC( dwTmp1 );
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\nPROC2_PC: %lx, PROC2_INT: %lx, REMEN: %lx, AIUEN: %hx", dwTmp1, REG_PLAT_PROC2_INT_MASK_PRIORITY, REG_MCU_REMEN, REG_AIU_AIUEN);
    PARSER_Debug(0);
    COMDEC_Debug();
    MONITOR_Debug();
#ifdef STACK_OVERFLOW_DETECTION
    _DBG_Report_Stack();
#endif

#ifndef NO_DISC_MODE //CoCo2.38
    if (__wDiscType & BOOK_DVD)
    {
        CELL_Debug();
    }
#endif //#ifndef NO_DVD //CoCo2.37p

#ifdef SUPPORT_SERVO_SOURCE
    // wyc1.00-909, show SERVO debug information.
    {
        extern void SRVDBG_SystemInfomation(void);
        SRVDBG_SystemInfomation();
    }
#endif  // #ifdef SUPPORT_SERVO_SOURCE

#ifdef  SYSTEM_ECOS
    // J500CSC_120, show exception message
    {
        DWORD   dwLoop;

        MACRO_GET_EXCEPTION_NUM(dwLoop);
        DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "Current Exception Count is %lx", dwLoop );

        for( ; dwLoop != 0; dwLoop -- )
        {
            MACRO_GET_EXCEPTION_INDEX(dwLoop, dwTmp1, dwTmp2);
            DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "Exception(%lx) TT(%lx) PC(%lx)", dwLoop, dwTmp1, dwTmp2 );
        }
    }
#endif  // #ifdef  SYSTEM_ECOS

#ifdef SUPPORT_PROC2_DBG
    // Chuan1.20, Notify PROC2 to dump the debug information.
    HAL_WriteAM(HAL_AM_DBG_SHOWINFO, TRUE);
#endif  // #ifdef SUPPORT_PROC2_DBG

    // Chuan2.77, Show SPI CLk & Type
    dwTmp = *(DWORD *)(*(DWORD *)0x8);
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "SPI_SCLK : %lx (%lx)", REG_PLAT_SPI_SCLK_CTRL, dwTmp);

#endif  // #ifdef SERIAL_DEBUG
}

// Chuan1.62, Add a function to detect stack usage
#ifdef STACK_OVERFLOW_DETECTION
VOID    _DBG_Report_Stack(VOID)
{
    DWORD dwIdx_DVD = 0;
    DWORD dwIdx_DEC = 0;
    DWORD dwIdx_PAR = 0;
    DWORD dwIdx_INFO = 0;
    DWORD dwIdx_ATAPI = 0;
    DWORD dwIdx_USBSRC = 0;
    DWORD dwIdx_SERVO = 0;
    DWORD dwIdx_SRVBM = 0;

    while(__cCTKDVDStack[dwIdx_DVD++] == STACK_MAGIC_NUMBER);
    while(__cDecStack[dwIdx_DEC++] == STACK_MAGIC_NUMBER);
    while(__cPARSERStack[dwIdx_PAR++] == STACK_MAGIC_NUMBER);
    while(__cInfoFilterStack[dwIdx_INFO++] == STACK_MAGIC_NUMBER);
#ifdef  SUPPORT_ATAPI_SOURCE
    while(__cATAPIStack[dwIdx_ATAPI++] == STACK_MAGIC_NUMBER);
#endif

#ifdef  SUPPORT_USB_SOURCE
    while(__cUSBSRCStack[dwIdx_USBSRC++] == STACK_MAGIC_NUMBER);
#endif

#ifdef SUPPORT_SERVO_SOURCE
    {
        extern char stack[2][4096];
        while(stack[0][dwIdx_SERVO++] == STACK_MAGIC_NUMBER);
        while(stack[1][dwIdx_SRVBM++] == STACK_MAGIC_NUMBER);
    }
#endif


    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\nUnused Stack size:DVD(%x) DEC(%x) PARSER(%x) Info(%x)", dwIdx_DVD, dwIdx_DEC, dwIdx_PAR, dwIdx_INFO);
    DBG_Printf(DBG_DEBUG, DBG_INFO_PRINTF, "\nUnused Stack size:USB(%x) ATAPI(%x) SERVO(%x) SRVBM(%x)", dwIdx_USBSRC, dwIdx_ATAPI, dwIdx_SERVO, dwIdx_SRVBM);
}
#endif

#endif  // #ifdef DBG_SOLUTION_DEPENDENCY

///////////////////////////////////////////////////////////////////////////////
//
// Below Functions used to support Serial Debug Tool
//
///////////////////////////////////////////////////////////////////////////////

#ifdef SERIAL_DEBUG
//***************************************************************************
//  Function    :   _DBG_WaitComand
//  Abstract    :   This function will be called in the Timer to receive command.
//  Arguments   :   none.
//  Return      :   TRUE: Debug Monitor can't print anything in printf.
//              :   FALSE: Debug Monitor can print the content in printf.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
BYTE    _DBG_WaitComand(VOID)
{
    BYTE bCh;

    // J300/Ben, Aug/15/2005
    #ifdef AutoTuning_MPEG
    if (AutoTuning_Enable_MPEG == TRUE)
    {
       return FALSE;	// AutoTuning Proceeding...
    }
    #endif //#ifdef AutoTuning_MPEG

    if (_bDBGRXReady == TRUE)
    {
        //The command is not processed. Ignore the new command.
        return FALSE;
    }

    // if (!_DBG_ReceiveChar(&bCh))
    if (!HAL_UART_ReceiveChar(&bCh, _dwUartPort_RX))
    {
        return FALSE; //No data
    }

    // J300/Ben, Aug/15/2005
    #ifdef AutoTuning_MPEG
    if (!_bDBGBinMode)
    {
       if ( (bCh == eCOS_AT_Enable) && (AutoTuning_Enable_MPEG == FALSE) )
       {

          printf("bCh == eCOS_AT_Enable kenny");

          AutoTuning_Enable_MPEG = TRUE;
          cyg_flag_setbits(&autotune_flag_var, FLAG_AUTOTUNE_ENABLE);

          return FALSE; //AutoTuning Begins...

       }
       // ywc, 060811
       // to handle real-time monitor
       else if ( ( bCh == eCOS_AT_Monitor ) )
       {
            cyg_flag_setbits(&autotune_flag_var, FLAG_AUTOTUNE_MONITOR);
            //printf("\nYWC received eCOS_AT_Monitor from DBG. %x \n ", FLAG_AUTOTUNE_MONITOR);
            return FALSE;
       }
       //Ben, May/03/2006
       else
       {
       	   printf("\n\nCommand->bCh == 0x%x\n\n", bCh);
       }
    }
    #endif //#ifdef AutoTuning_MPEG

    if (bCh == START_BIN_MODE_CMD)
        _bDBGBinMode = TRUE;

    if (!_bDBGBinMode)
    {
        //DWORD dwEnterTime;
        //dwEnterTime = REG_PLAT_TIMER3_VALUE;
        // if ((REG_PLAT_UART_STATUS(_dwUartPort) & UART_STATUS_OVERRUN) || (REG_PLAT_UART_STATUS(_dwUartPort) & UART_STATUS_FRAME_ERROR)) //data lost
        if (HAL_UART_ErrorStatus(_dwUartPort_RX) != FALSE)
        {
            _PutCHAR(DBG_DEBUG_HI, ACK_TEXT_MODE_CMD_LOST);
            // Auto-Clear by IO_UART_ErrorStatus();
            // REG_PLAT_UART_STATUS(_dwUartPort) &= ~(UART_STATUS_OVERRUN | UART_STATUS_FRAME_ERROR);
            return TRUE;
        }

        if (bCh == START_TEXT_MODE_CMD)
        {
            _bDBGStartTextCmd = TRUE;
            _PutCHAR(DBG_DEBUG_HI, ACK_START_TEXT_MODE_CMD);
            return TRUE;
        }


        //increment write index < RX_BUFFER_SIZE
        if (_bDBGRXCommandLen < DBG_RX_COMMAND_SIZE)
        {
            _bDBGRXCommandData[_bDBGRXCommandLen] = bCh;
            _bDBGRXCommandLen++;
        }

        if (bCh == '\r')
        {
            if (_bDBGRXCommandData[0] == 'K') //Input IR key
            {
// Chuan1.62, __bISRKey just depend upon solution
#ifdef  DBG_SOLUTION_DEPENDENCY
                __bISRKey = (BYTE) _DBG_StrToHexVal(&_bDBGRXCommandData[1], 2);    //BYTE
#endif  //#ifdef  DBG_SOLUTION_DEPENDENCY
                _bDBGRXCommandLen = 0;
            }
            else
            {
                //input one command complete
                _bDBGRXReady = TRUE;
            }

            _bDBGStartTextCmd = FALSE;
        }

        _PutCHAR(DBG_DEBUG_HI, ACK_START_TEXT_MODE_CMD);

        /*
        if (REG_PLAT_UART1_STATUS & UART_STATUS_OVERRUN) //data lost
        {
            DWORD dwSaveInt;
            _PutBYTE(DBG_DEBUG_HI, 0xFF);

            HAL_DISABLE_INTERRUPTS( dwSaveInt );    // Disable INT to avoid different threads accessing
            MACRO_PLAT_KEY_LOCK( );     // Use key lock to avoid different processor accessing.
            REG_PLAT_RESET_CONTROL &= (!PLAT_RESET_UART1);
            REG_PLAT_RESET_CONTROL |= PLAT_RESET_UART1;
            MACRO_PLAT_KEY_UNLOCK( );   // Release key.
            HAL_RESTORE_INTERRUPTS( dwSaveInt );    // Restore INT.
            // enable UARTs
            REG_PLAT_UART1_CONTROL = 0;
            //REG_PLAT_UART1_SCALER = (((SYSTEM_CLK*10)/(UART_BAUDRATE_QUAD_SPEED*8))-5)/10;
            REG_PLAT_UART1_SCALER = (((SYSTEM_CLK*10)/(UART_BAUDRATE*8))-5)/10;
            REG_PLAT_UART1_CONTROL = (UART_RX_ENABLE|UART_TX_ENABLE);

            return TRUE;
           }
        */
        /*
        // Chuan0.67, Timeout control (1 sec) for bin mode
        if ((REG_PLAT_TIMER3_VALUE - dwEnterTime) > 90000)
        {
        _bDBGStartTextCmd = FALSE;
        }
        */
    }
    else
    {
        //Add code for serial debug tool.
        if (!_bDBGChangeRxToRomBuffer)
        {
            if (bCh == END_CMD)
            {
                _bDBGRXFlag=TRUE;
            }

            _bDBGRXCommandData[_bDBGBufWriteIndex++] = bCh;

            _bDBGBufWriteIndex = _bDBGBufWriteIndex%DBG_RX_COMMAND_SIZE;
        }
        else  //_bDBGChangeRxToRomBuffer==TRUE, use ROM data buffer to save data
        {
            _bpDBGROMBuf[_wDBGROMdataBufIndex++] = bCh;
            if (_wDBGROMdataBufIndex == _wDBGROMSize)
                _bDBGRXFlag=TRUE;
        }
    }

    return TRUE;
}

//***************************************************************************
//  Function    :   _DBG_CheckRX
//  Abstract    :   This function will check the Rx buufer. If an "carriage retun"
//              :   is received, it will parse the command to SYSDEBUG.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_CheckRX(VOID)
{
    if (_bDBGRXReady == TRUE)
    {
        /* receive '\n' or '\r' */
        _DBG_CommandParse();

        _bDBGRXCommandLen=0;
        _bDBGRXReady=FALSE;
    }
}

//***************************************************************************
//  Function    :   _DBG_SearchCommand
//  Abstract    :   This function will search if the command has a "SPACE".
//  Arguments   :   bStart ==>the starting index in command buffer.
//  Return      :   HIBYTE != 0 ==>has space
//              :   LOBYTE ==> numbers of the characters for the first parameter.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
WORD    _DBG_SearchCommand(BYTE bStart)
{
    BYTE bIndex;
    WORD wDBGTemp = 0;

    for (bIndex = bStart; bIndex < _bDBGRXCommandLen; bIndex++)
    {
        //if "SPACE" or "ENTER" key is detected, break
        if (_bDBGRXCommandData[bIndex] == 0x20) //SPACE
        {
           wDBGTemp |= DBG_COMMAND_SPACE;
           break;
        }
        else if (_bDBGRXCommandData[bIndex] == 0x0D) //ENTER
            break;
        else
            wDBGTemp++;
    }

    return wDBGTemp;
}

//***************************************************************************
//  Function    :   _DBG_AnalyzeCommand
//  Abstract    :   This function will analyze command to extract the data and size
//  Arguments   :   bStart: Analyze the command data from the index.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_AnalyzeCommand(BYTE bStart)
{
    BYTE _bHaveSpace;
    WORD wDBGTemp1;

    _dwDBGCmd2ndParam = 0;
    _dwDBGCmd3rdParam = 0;

    wDBGTemp1 = _DBG_SearchCommand(bStart);
    _bDBGLen = (BYTE)(wDBGTemp1 & 0xFF);    //The length of the address

    //The first parameter
    _dwDBGCmd1stParam = _DBG_StrToHexVal(&_bDBGRXCommandData[bStart], _bDBGLen);

    _bHaveSpace = (wDBGTemp1 & 0xFF00) >> 8; //to see if have space

    //Get the second parameter, _dwDBGTemp1
    if (_bHaveSpace) //have first space
    {
        //judge if have the second space
        wDBGTemp1 = _DBG_SearchCommand(bStart+_bDBGLen+1); //DDRN+_bDBGLen+SPACE
        _bHaveSpace = (wDBGTemp1 & 0xFF00) >> 8; //to see if have second space
        if (_bHaveSpace)
        {
            _bDBGLen1 = (BYTE)(wDBGTemp1 & 0xff); //length of the second parameter
            _dwDBGCmd2ndParam = _DBG_StrToHexVal(&_bDBGRXCommandData[bStart+_bDBGLen], _bDBGLen1); //The size = _bDBGRXCommandLen - DDRN - _bDBGLen - SPACE - ENTER
            _dwDBGCmd3rdParam = _DBG_StrToHexVal(&_bDBGRXCommandData[bStart+_bDBGLen+1+_bDBGLen1], (_bDBGRXCommandLen - bStart - _bDBGLen - 1 - _bDBGLen1 -1 - 1)); //The size = _bDBGRXCommandLen - DDRN - _bDBGLen - SPACE - _bDBGLen1 - SPACE - ENTER
        }
        else
        {
            _dwDBGCmd2ndParam = _DBG_StrToHexVal(&_bDBGRXCommandData[bStart+_bDBGLen], (_bDBGRXCommandLen - bStart - _bDBGLen - 1 - 1)); //The size = _bDBGRXCommandLen - DDRN - _bDBGLen - SPACE - ENTER
        }
    }
}

//***************************************************************************
//  Function    :   _DBG_CommandParse
//  Abstract    :   This function will parse the text command and do the action
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_CommandParse(VOID)
{
    BYTE bOperation;
    DWORD dwIndex;
    DWORD dwDBGSize;
    DWORD dwValue;

    switch (_bDBGRXCommandData[0])
    {
    case 'M' : //This command should be put before "D" command.
        //Analyze the write DRAM commands
        _DBG_AnalyzeCommand(4);

        if (_bDBGRXCommandData[1] == 'D')
        {
            if (_bDBGRXCommandData[2] == 'R') //Modify DRAM data, MDRN, MDRA, MDRO
            {
                dwValue = *((volatile DWORD *)_dwDBGCmd1stParam);
                if (_bDBGRXCommandData[3] == 'A') //And
                    _dwDBGCmd2ndParam &= dwValue;
                else if (_bDBGRXCommandData[3] == 'O') //Or
                    _dwDBGCmd2ndParam |= dwValue;

                if (_dwDBGCmd3rdParam == 0)
                {
                    dwDBGSize = 1;
                }
                else
                {
                    dwDBGSize = _dwDBGCmd3rdParam;
                }

                //Support write multiple DRAM/DM at the same time.
                for (dwIndex = 0;  dwIndex < dwDBGSize; dwIndex++)
                {
                    *((volatile DWORD *)(_dwDBGCmd1stParam+dwIndex*4)) = _dwDBGCmd2ndParam;
                }

                _DBG_DumpDRAM(_dwDBGCmd1stParam, dwDBGSize);
            }
        }
        break;
    case 'D' :
        //Analyze the Read DRAM commands
        _DBG_AnalyzeCommand(4);

        if (_bDBGRXCommandData[1] == 'D')
        {
            if (_bDBGRXCommandData[2] == 'R')
            {
                if (_dwDBGCmd2ndParam == 0)
                {
                    dwDBGSize = 1;
                }
                else
                {
                    dwDBGSize = _dwDBGCmd2ndParam;
                }

                _DBG_DumpDRAM(_dwDBGCmd1stParam, dwDBGSize);
            }
            else
            {
                //_printf("\nERROR\n");
                return;
            }
        }
        else
        {
            //_printf("\nERROR\n");
            return;
        }
        break;
    case 'R':
        //Analyze the Read Register commands
        _DBG_AnalyzeCommand(3);
        if (_bDBGRXCommandData[1] == 'D') //Dump
        {
            if (_bDBGRXCommandData[2] == 'N')
            {
                _DBG_DumpMem(_dwDBGCmd1stParam, (BYTE)_dwDBGCmd2ndParam);
            }
            else if (_bDBGRXCommandData[2] == 'A') //Array
            {
                _DBG_DumpMemArray(_dwDBGCmd1stParam, (BYTE)_dwDBGCmd2ndParam, _dwDBGCmd3rdParam);
            }
        }
        else if (_bDBGRXCommandData[1] == 'M') //Modify
        {
            if (_bDBGRXCommandData[2] == 'A') //And
            {
                bOperation = SERIAL_DEBUG_OPERATE_AND;
            }
            else if (_bDBGRXCommandData[2] == 'O') //Or
            {
                bOperation = SERIAL_DEBUG_OPERATE_OR;
            }
            else
            {
                bOperation = SERIAL_DEBUG_OPERATE_NONE;
            }

            _DBG_ModifyMem(_dwDBGCmd1stParam, _dwDBGCmd2ndParam, bOperation, (BYTE)_dwDBGCmd3rdParam);

            _DBG_DumpMem(_dwDBGCmd1stParam, (BYTE)_dwDBGCmd3rdParam);
        }
        break;
#ifdef DBG_SOLUTION_DEPENDENCY
    case 'I':   // Show debug information for W9922QF
        _DBG_ShowDebug();
        break;
    case 'P':
        if (_bDBGRXCommandData[1] == 'M') // MPEG debug info.
        {
            __dwDebugFlag &= 0xFFFFFF00;
            __dwDebugFlag |= (BYTE)_DBG_StrToHexVal(&_bDBGRXCommandData[2], 2);    // BYTE
        }
        else if (_bDBGRXCommandData[1] == 'S') // Servo debug info.
        {
            __dwDebugFlag &= 0xFFFF00FF;
            __dwDebugFlag |= ((WORD)_DBG_StrToHexVal(&_bDBGRXCommandData[2], 2) << 8);    // BYTE
        }
        else if (_bDBGRXCommandData[1] == 'R') // MPEG2 debug info.
        {
            __dwDebugFlag &= 0x0000FFFF;
            __dwDebugFlag |= ((DWORD)_DBG_StrToHexVal(&_bDBGRXCommandData[2], 4) << 16);    // WORD
        }
        else if (_bDBGRXCommandData[1] == 'A') // Proc2 debug info.
        {
            REG_DBG_PROC2FLAG = (DWORD)_DBG_StrToHexVal(&_bDBGRXCommandData[2], 8); // DWORD
        }
        break;
#endif  // #ifdef DBG_SOLUTION_DEPENDENCY
    case 'C'://I2C
        /*
        if (_bDBGRXCommandData[1] == 'R') //Read I2C //CRxx xx xx: the sequence is device ID, address, count
        {
            bTemp = (BYTE)_DBG_StrToHexVal(&_bDBGRXCommandData[2], 2); //Device ID
            bTemp1 = (BYTE)_DBG_StrToHexVal(&_bDBGRXCommandData[5], 2); //Address
            bTemp2 = (BYTE)_DBG_StrToHexVal(&_bDBGRXCommandData[8], 2); //Size

            for (bIndex = 0; bIndex < bTemp2; bIndex++)
            {
                // Chuan.Check, Miss function
                //_I2C_Read(bTemp, bTemp1, 1, &_bDBGLen);
                //_SYSDEBUG_DumpI2C(bTemp1, _bDBGLen);
                bTemp1++;
            }
        }
        else if (_bDBGRXCommandData[1] == 'W') //Write I2C //WRxx xx (xx xx xx xx....). The (xx xx xx xx..) are the values to be written
        {
            bTemp = (BYTE)_DBG_StrToHexVal(&_bDBGRXCommandData[2], 2); //Device ID
            _bDBGLen = (BYTE)_DBG_StrToHexVal(&_bDBGRXCommandData[5], 2); //count

            for (bTemp1 = 0; bTemp1 < _bDBGLen; bTemp1++)
            {
                // Chuan.Check, Miss variable.
                //_REG[bTemp1] = (BYTE)_DBG_StrToHexVal(&_bDBGRXCommandData[8+3*bTemp1], 2);
                ;
            }
            // Chuan.Check, Miss function.
            //_I2C_Write(bTemp, _REG, _bDBGLen);
        }
        */
        break;
#ifdef MACROVISION_CERTIFICATION
    case 'O': //for read/write macrovision.
        if (_bDBGRXCommandData[1] == 'D') //OD==>Write macrovision default.
        {
            TVE_WrieN0N22Default();
        }
        else if (_bDBGRXCommandData[1] == 'R') //ORxx==>Read macrovision register.
        {
            _dwDBGCmd1stParam = _DBG_StrToHexVal(&_bDBGRXCommandData[2], 2);
            dwValue = TVE_ReadMVRegN0N22W((BYTE)_dwDBGCmd1stParam);
            _PutDWORD(DBG_DEBUG_HI, dwValue);
        }
        else if (_bDBGRXCommandData[1] == 'W') //OWxx xxxx==>Write macrovision register.
        {
            _DBG_AnalyzeCommand(2);
            TVE_WriteMVRegN0N22W((BYTE)_dwDBGCmd1stParam, (WORD)(_dwDBGCmd2ndParam));
        }
        break;
#endif  // #ifdef MACROVISION_CERTIFICATION
    case 'N':
        _DBG_ResetDRAMDebugMessage();
        break;
    default :
        //_printf("\nERROR\n");
        return;
    }
}

//***************************************************************************
//  Function    :   _DBG_StrToHexVal
//  Abstract    :   This function will translate a string to a Hex value.
//  Arguments   :   pTemp==>the string.
//              :   bNum==>the number of the the characters.
//  Return      :   The Translated Hex value.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
DWORD   _DBG_StrToHexVal(BYTE *pTemp, BYTE bNum)
{
    BYTE i;
    BYTE c;
    BYTE *pbStr;
    DWORD dwValue;

    dwValue=0;

    pbStr = pTemp;
    while (*pbStr==' ')
        pbStr++;

    for (i = 0; i < bNum; i++)
    {
        c=*pbStr++;
        if (c>='0' && c<='9')
            c &= 0x0f;
        else if (c>='A' && c<='F')
            //c= (c&0x0f)+9;
            c=c-65+10;  //The ASCII code of 'A' is 65
        else
            break;
        dwValue = (dwValue << 4) + c;
    }

    return dwValue;
}

//***************************************************************************
//  Function    :   _DBG_DumpDRAM
//  Abstract    :   This function will deump the data from DRAM.
//  Arguments   :   dwAddress==>the DRAM address
//              :   dwSize==>Total size
//  Return      :   The Translated Hex value.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_DumpDRAM(DWORD dwAddress, DWORD dwSize)
{
    DWORD dwIndex;
    DWORD dwDRAMData;
    DWORD dwDWORDSize;
    DWORD dwDRAMAddress = dwAddress;

#ifdef AutoTuning_MPEG
    DWORD dwSaveInt;
#endif

#ifdef SUPPORT_WATCH_DOG
    HAL_WatchDog_Enable(FALSE);
#endif

    //DWORD alignment.
    if (dwDRAMAddress % 4)
    {
        dwDRAMAddress = (dwDRAMAddress / 4)*4;
        dwDWORDSize = dwSize+1;
    }
    else
    {
        dwDWORDSize = dwSize;
    }

#ifdef DISPLAY_4_DWORD_PER_LINE
#ifdef AutoTuning_MPEG
    //YWC, 060803
    HAL_DISABLE_INTERRUPTS( dwSaveInt );
    _PutCHAR( DBG_DEBUG_HI, 0xA9 );
#endif
    _PutDWORD(DBG_DEBUG_HI, dwDRAMAddress); //The starting DRAM address
    _PutCHAR(DBG_DEBUG_HI, ':');
    _PutCHAR(DBG_DEBUG_HI, '\t');

    for (dwIndex=0; dwIndex < dwDWORDSize; dwIndex++)
    {
        dwDRAMData = *((volatile DWORD *)dwDRAMAddress);
        dwDRAMAddress += 4;

        _PutDWORD(DBG_DEBUG_HI, dwDRAMData);
        if ((dwIndex % 4) == 3)
        {
            _PutCHAR(DBG_DEBUG_HI, '\r');
            _PutCHAR(DBG_DEBUG_HI, '\n');
            _PutDWORD(DBG_DEBUG_HI, dwDRAMAddress); //The starting DRAM address
            _PutCHAR(DBG_DEBUG_HI, ':');
        }
        _PutCHAR(DBG_DEBUG_HI, '\t');
    }
#ifdef AutoTuning_MPEG
    //YWC. 060803
    HAL_RESTORE_INTERRUPTS( dwSaveInt );
#endif
    _PutCHAR(DBG_DEBUG_HI, '\r');
    _PutCHAR(DBG_DEBUG_HI, '\n');
#else
#ifdef AutoTuning_MPEG
    //YWC, 060803
    HAL_DISABLE_INTERRUPTS( dwSaveInt );
    _PutCHAR( DBG_DEBUG_HI, 0xA9 );
#endif

    for (dwIndex=0; dwIndex < dwDWORDSize; dwIndex++)
    {
        _PutDWORD(DBG_DEBUG_HI, dwDRAMAddress); //The starting DRAM address
        _PutCHAR(DBG_DEBUG_HI, ':');

        dwDRAMData = *((volatile DWORD *)dwDRAMAddress);
        dwDRAMAddress += 4;

        _PutDWORD(DBG_DEBUG_HI, dwDRAMData);
        _PutCHAR(DBG_DEBUG_HI, '\r');
        _PutCHAR(DBG_DEBUG_HI, '\n');
    }
#ifdef AutoTuning_MPEG
    //YWC. 060803
    HAL_RESTORE_INTERRUPTS( dwSaveInt );
#endif
#endif  // #ifdef DISPLAY_4_DWORD_PER_LINE

#ifdef SUPPORT_WATCH_DOG
    HAL_WatchDog_Enable(TRUE);
#endif
}


//***************************************************************************
//  Function    :   _DBG_DumpMem
//  Abstract    :   This function will dump memory according to "BYTE", "WORD" and "DWORD".
//  Arguments   :   dwAddress==>the DRAM address
//              :   bSizeType==>SERIAL_DEBUG_SIZE_BYTE/SERIAL_DEBUG_SIZE_WORD/SERIAL_DEBUG_SIZE_DWORD.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_DumpMem(DWORD dwAddress, BYTE bSizeType)
{
#ifdef AutoTuning_MPEG
    DWORD dwSaveInt;

    //YWC, 060803
    HAL_DISABLE_INTERRUPTS( dwSaveInt );
    _PutCHAR( DBG_DEBUG_HI, 0xA9 );
#endif

    _PutDWORD(DBG_DEBUG_HI, dwAddress); //The starting DRAM address
    _PutCHAR(DBG_DEBUG_HI, ':');

    switch (bSizeType)
    {
    case SERIAL_DEBUG_SIZE_BYTE:
        _PutBYTE(DBG_DEBUG_HI, *((volatile BYTE *)dwAddress));
        break;
    case SERIAL_DEBUG_SIZE_WORD:
        _PutWORD(DBG_DEBUG_HI, ((*((volatile BYTE *)dwAddress)) << 8) | (*((volatile BYTE *)(dwAddress+1))));
        break;
    case SERIAL_DEBUG_SIZE_DWORD:
        _PutDWORD(DBG_DEBUG_HI, ((*((volatile BYTE *)dwAddress)) << 24) | ((*((volatile BYTE *)(dwAddress+1))) << 16) | ((*((volatile BYTE *)(dwAddress+2))) << 8) | (*((volatile BYTE *)(dwAddress+3))));
        break;
    }

    _PutCHAR(DBG_DEBUG_HI, '\r');
    _PutCHAR(DBG_DEBUG_HI, '\n');

#ifdef AutoTuning_MPEG
    //YWC. 060803
    HAL_RESTORE_INTERRUPTS( dwSaveInt );
#endif

}

//***************************************************************************
//  Function    :   _DBG_DumpMemArray
//  Abstract    :   This function will dump memory according to "BYTE", "WORD" and "DWORD".
//  Arguments   :   dwAddress==>the DRAM address
//              :   bSizeType==>SERIAL_DEBUG_SIZE_BYTE/SERIAL_DEBUG_SIZE_WORD/SERIAL_DEBUG_SIZE_DWORD.
//              :   dwNumOfEntry==>Number of entry to dump array.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_DumpMemArray(DWORD dwAddress, BYTE bSizeType, DWORD dwNumOfEntry)
{
    DWORD i;

    _PutDWORD(DBG_DEBUG_HI, dwAddress); //The starting DRAM address
    _PutCHAR(DBG_DEBUG_HI, ':');

    for (i = 0; i < dwNumOfEntry; i++)
    {
        _PutCHAR(DBG_DEBUG_HI, '0');
        _PutCHAR(DBG_DEBUG_HI, 'x');
        switch (bSizeType)
        {
        case SERIAL_DEBUG_SIZE_BYTE:
            _PutBYTE(DBG_DEBUG_HI, *((volatile BYTE *)dwAddress));
            dwAddress++;
            break;
        case SERIAL_DEBUG_SIZE_WORD:
            _PutWORD(DBG_DEBUG_HI, ((*((volatile BYTE *)dwAddress)) << 8) | (*((volatile BYTE *)(dwAddress+1))));
            dwAddress += 2;
            break;
        case SERIAL_DEBUG_SIZE_DWORD:
            _PutDWORD(DBG_DEBUG_HI, ((*((volatile BYTE *)dwAddress)) << 24) | ((*((volatile BYTE *)(dwAddress+1))) << 16) | ((*((volatile BYTE *)(dwAddress+2))) << 8) | (*((volatile BYTE *)(dwAddress+3))));
            dwAddress += 4;
            break;
        }

        if (i != (dwNumOfEntry-1))
            _PutCHAR(DBG_DEBUG_HI, ',');

        if (((i+1) %10) == 0)
        {
            _PutCHAR(DBG_DEBUG_HI, '\r');
            _PutCHAR(DBG_DEBUG_HI, '\n');
        }
    }

    _PutCHAR(DBG_DEBUG_HI, '\r');
    _PutCHAR(DBG_DEBUG_HI, '\n');
}

//***************************************************************************
//  Function    :   _DBG_ModifyMem
//  Abstract    :   This function will modify memory according to "BYTE", "WORD" and "DWORD".
//  Arguments   :   dwAddress: The DRAM address.
//              :   dwWriteValue: The value we want to write.
//              :   bSizeType: SERIAL_DEBUG_SIZE_BYTE/SERIAL_DEBUG_SIZE_WORD/SERIAL_DEBUG_SIZE_DWORD.
//              :   bOperation:SERIAL_DEBUG_OPERATE_NONE/SERIAL_DEBUG_OPERATE_AND/SERIAL_DEBUG_OPERATE_OR.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_ModifyMem(DWORD dwAddress, DWORD dwWriteValue, BYTE bOperation, BYTE bSizeType)
{
    BYTE bValue;
    WORD wValue;
    DWORD dwValue;

    switch (bSizeType)
    {
    case SERIAL_DEBUG_SIZE_BYTE:
        bValue = *((volatile BYTE *)dwAddress);
        if (bOperation == SERIAL_DEBUG_OPERATE_AND)
        {
            *((volatile BYTE *)dwAddress) = bValue & (BYTE)dwWriteValue;
        }
        else if (bOperation == SERIAL_DEBUG_OPERATE_OR)
        {
            *((volatile BYTE *)dwAddress) = bValue | (BYTE)dwWriteValue;
        }
        else
        {
            *((volatile BYTE *)dwAddress) = (BYTE)dwWriteValue;
        }
        break;
    case SERIAL_DEBUG_SIZE_WORD:
        //wValue = *((volatile WORD *)dwAddress);
        wValue = ((*((volatile BYTE *)dwAddress)) << 8) | (*((volatile BYTE *)(dwAddress+1)));
        if (bOperation == SERIAL_DEBUG_OPERATE_AND)
        {
            //*((volatile WORD *)dwAddress) = wValue & (WORD)dwWriteValue;
            *((volatile BYTE *)dwAddress) = HIBYTE((wValue & (WORD)dwWriteValue));
            *((volatile BYTE *)(dwAddress+1)) = LOBYTE((wValue & (WORD)dwWriteValue));
        }
        else if (bOperation == SERIAL_DEBUG_OPERATE_OR)
        {
            //*((volatile WORD *)dwAddress) = wValue | (WORD)dwWriteValue;
            *((volatile BYTE *)dwAddress) = HIBYTE((wValue | (WORD)dwWriteValue));
            *((volatile BYTE *)(dwAddress+1)) = LOBYTE((wValue | (WORD)dwWriteValue));
        }
        else
        {
            //*((volatile WORD *)dwAddress) = (WORD)dwWriteValue;
            *((volatile BYTE *)dwAddress) = HIBYTE((WORD)dwWriteValue);
            *((volatile BYTE *)(dwAddress+1)) = LOBYTE((WORD)dwWriteValue);
        }
        break;
    case SERIAL_DEBUG_SIZE_DWORD:
        //dwValue = *((volatile DWORD *)dwAddress);
        dwValue = ((*((volatile BYTE *)dwAddress)) << 24) | ((*((volatile BYTE *)(dwAddress+1))) << 16) | ((*((volatile BYTE *)(dwAddress+2))) << 8) | (*((volatile BYTE *)(dwAddress+3)));
        if (bOperation == SERIAL_DEBUG_OPERATE_AND)
        {
            //*((volatile DWORD *)dwAddress) = dwValue & dwWriteValue;
            *((volatile BYTE *)dwAddress) = HIBYTE((HIWORD(dwValue & dwWriteValue)));
            *((volatile BYTE *)(dwAddress+1)) = LOBYTE((HIWORD(dwValue & dwWriteValue)));
            *((volatile BYTE *)(dwAddress+2)) = HIBYTE((LOWORD(dwValue & dwWriteValue)));
            *((volatile BYTE *)(dwAddress+3)) = LOBYTE((LOWORD(dwValue & dwWriteValue)));
        }
        else if (bOperation == SERIAL_DEBUG_OPERATE_OR)
        {
            //*((volatile DWORD *)dwAddress) = dwValue | dwWriteValue;
            *((volatile BYTE *)dwAddress) = HIBYTE((HIWORD(dwValue | dwWriteValue)));
            *((volatile BYTE *)(dwAddress+1)) = LOBYTE((HIWORD(dwValue | dwWriteValue)));
            *((volatile BYTE *)(dwAddress+2)) = HIBYTE((LOWORD(dwValue | dwWriteValue)));
            *((volatile BYTE *)(dwAddress+3)) = LOBYTE((LOWORD(dwValue | dwWriteValue)));
        }
        else
        {
            //*((volatile DWORD *)dwAddress) = dwWriteValue;
            *((volatile BYTE *)dwAddress) = HIBYTE((HIWORD(dwWriteValue)));
            *((volatile BYTE *)(dwAddress+1)) = LOBYTE((HIWORD(dwWriteValue)));
            *((volatile BYTE *)(dwAddress+2)) = HIBYTE((LOWORD(dwWriteValue)));
            *((volatile BYTE *)(dwAddress+3)) = LOBYTE((LOWORD(dwWriteValue)));
        }
        break;
    default:
        return;
    }
}

//***************************************************************************
//  Function    :   _DBG_ReadAndCheckInBuffer
//  Abstract    :
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
BYTE    _DBG_ReadAndCheckInBuffer(BYTE * bRetValue)
{
    if(_bDBGBufReadIndex != _bDBGBufWriteIndex)
    {
        *bRetValue = _bDBGRXCommandData[_bDBGBufReadIndex++];
        if (_bDBGBufReadIndex >= DBG_RX_COMMAND_SIZE)
            _bDBGBufReadIndex = 0;
    }
    else  /* no data in Rx buffer, error read */
        return FALSE;

    return TRUE;
}

//***************************************************************************
//  Function    :   _DBG_ReadInBuffer
//  Abstract    :
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
BYTE    _DBG_ReadInBuffer(VOID)
{
    BYTE bData;

    if(_bDBGBufReadIndex != _bDBGBufWriteIndex)
    {
        bData = _bDBGRXCommandData[_bDBGBufReadIndex++];
        if(_bDBGBufReadIndex >= DBG_RX_COMMAND_SIZE)
            _bDBGBufReadIndex = 0;
    }
    else  /* no data in Rx buffer, error read */
        bData=0xFF;

    return bData;
}

//***************************************************************************
//  Function    :   _DBG_ProcessBinCommand
//  Abstract    :   This function will process the binary command.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_ProcessBinCommand(VOID)
{
    BYTE bData;
    BYTE bResult;

    bData=DBG_EMPTY_DATA_IN_BUFFER;
    bResult=_DBG_ReadAndCheckInBuffer(&_bDBGCurrentCommand);
    switch (_bDBGCurrentCommand) /* read data from rx buffer */
    {
    case START_BIN_MODE_CMD:
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
        if (bData == END_CMD)
        {
            _PutCHAR(DBG_DEBUG_HI, START_BIN_MODE_ACK);
        }
        else
        {
            _DBG_EndCommand(bResult);
            _PutCHAR(DBG_DEBUG_HI, ERROR_START_BIN_MODE);
        }
        break;
    case END_BIN_MODE_CMD:
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
        if (bData == END_CMD)
        {
            _bDBGBinMode = FALSE;
            _bpDBGROMBuf = _bDBGROMDataBuf;
            _PutCHAR(DBG_DEBUG_HI, END_BIN_MODE_ACK);
        }
        else
        {
            _DBG_EndCommand(bResult);
            _PutCHAR(DBG_DEBUG_HI, ERROR_END_BIN_MODE);
        }
        break;
    case START_SEQ_RW_DRAM_CMD: //Joey2.60: Support Vipor Tunning Tools
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
        if (bData == END_CMD)
        {
            BOOL bSeqRWDramMode = TRUE;
            DWORD dwSaveInt;

            HAL_DISABLE_INTERRUPTS(dwSaveInt);
            _PutCHAR(DBG_DEBUG_HI, ACK_START_SEQ_RW_DRAM_CMD);
            while(bSeqRWDramMode)
            {
                BYTE bChar;
                while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                switch(bChar)
                {
                    case END_SEQ_RW_DRAM_CMD:
                        while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                        if(bChar == END_CMD)
                        {
                            _PutCHAR(DBG_DEBUG_HI, ACK_END_SEQ_RW_DRAM_CMD);
                            bSeqRWDramMode = FALSE;
                        }
                        else
                        {
                            _PutCHAR(DBG_DEBUG_HI, ERROR_END_SEQ_RW_DRAM_CMD);
                            break;
                        }
                        break;
                    case SEQ_READ_DRAM_SUBCMD:
                        {
                            DWORD dwAddr=0, dwValue;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwAddr = (dwAddr << 8) | bChar;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwAddr = (dwAddr << 8) | bChar;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwAddr = (dwAddr << 8) | bChar;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwAddr = (dwAddr << 8) | bChar;

                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            if(bChar == END_CMD)
                            {
                                _PutCHAR(DBG_DEBUG_HI, ACK_SEQ_READ_DRAM_SUBCMD);
                                dwAddr &= ~3;
                                dwValue = *((volatile DWORD*)dwAddr);
                                _PutCHAR(DBG_DEBUG_HI, dwValue >> 24);
                                _PutCHAR(DBG_DEBUG_HI, dwValue >> 16);
                                _PutCHAR(DBG_DEBUG_HI, dwValue >> 8);
                                _PutCHAR(DBG_DEBUG_HI, dwValue >> 0);
                            }
                            else
                            {
                                _PutCHAR(DBG_DEBUG_HI, ERROR_SEQ_READ_DRAM_SUBCMD);
                            }
                        }
                        break;
                    case SEQ_WRITE_DRAM_SUBCMD:
                        {
                            DWORD dwAddr=0, dwValue=0;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwAddr = (dwAddr << 8) | bChar;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwAddr = (dwAddr << 8) | bChar;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwAddr = (dwAddr << 8) | bChar;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwAddr = (dwAddr << 8) | bChar;

                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwValue = (dwValue << 8) | bChar;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwValue = (dwValue << 8) | bChar;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwValue = (dwValue << 8) | bChar;
                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            dwValue = (dwValue << 8) | bChar;

                            while(!HAL_UART_ReceiveChar(&bChar, _dwUartPort_RX)) MACRO_IDLE();
                            if(bChar == END_CMD)
                            {
                                dwAddr &= ~3;
                                *((volatile DWORD*)dwAddr) = dwValue;
                                _PutCHAR(DBG_DEBUG_HI, ACK_SEQ_WRITE_DRAM_SUBCMD);
                            }
                            else
                            {
                                _PutCHAR(DBG_DEBUG_HI, ERROR_SEQ_WRITE_DRAM_SUBCMD);
                            }
                        }
                        break;
                    case END_CMD:
                        _PutCHAR(DBG_DEBUG_HI, END_CMD);
                        break;
                }
            }
            HAL_RESTORE_INTERRUPTS(dwSaveInt);
        }
        else
        {
            _DBG_EndCommand(bResult);
            _PutCHAR(DBG_DEBUG_HI, ERROR_START_SEQ_RW_DRAM_CMD);
        }
        break;
    case READ_DRAM_CMD:
        _wDBGROMSize = _DBG_ReadInBuffer();
        _wDBGROMSize <<= 8;
        _wDBGROMSize |= _DBG_ReadInBuffer();
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
        if (bData == END_CMD)
        {
            _bDBGChangeRxToRomBuffer = TRUE;
            _wDBGROMdataBufIndex = 0;
            _PutCHAR(DBG_DEBUG_HI, ACK_READ_DRAM_CMD);
        }
        else
        {
            _DBG_EndCommand(bResult);
            _PutCHAR(DBG_DEBUG_HI, ERROR_READ_DRAM_CMD);
        }
        break;
    case WRITE_DRAM_CMD:
        _wDBGROMSize = _DBG_ReadInBuffer();
        _wDBGROMSize <<= 8;
        _wDBGROMSize |= _DBG_ReadInBuffer();
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
        if (bData == END_CMD)
        {
            _bDBGChangeRxToRomBuffer = TRUE;
            _wDBGROMdataBufIndex = 0;
            _PutCHAR(DBG_DEBUG_HI, ACK_WRITE_DRAM_CMD);
        }
        else
        {
            _DBG_EndCommand(bResult);
            _PutCHAR(DBG_DEBUG_HI, ERROR_WRITE_DRAM_CMD);
        }
        break;
    case WRITE_DATA_CMD:
        /* receive write data command, then change rx buffer to rom data buffer */
        _wDBGROMSize = _DBG_ReadInBuffer();
        _wDBGROMSize <<= 8;
        _wDBGROMSize |= _DBG_ReadInBuffer();
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
        if (bData == END_CMD)
        {
            _PutCHAR(DBG_DEBUG_HI, ACK_WRITE_DATA_CMD);
            _bDBGChangeRxToRomBuffer = TRUE;
            _wDBGROMdataBufIndex = 0;
        }
        else
        {
            _DBG_EndCommand(bResult);
            _PutCHAR(DBG_DEBUG_HI, ERROR_WRITE_DATA_CMD);
        }
        break;
#ifdef DBG_SOLUTION_DEPENDENCY
    case PREPARE_AUTO_UPGRADE_CMD:
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
        if (bData == END_CMD)
        {
            _dwDebugFlagBackup = __dwDebugFlag;
            __dwDebugFlag = 0;
            _bAutoUpgrade = TRUE;
            DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);

            OS_DelayTime(COUNT_50_MSEC);

            MACRO_MCU_WRITE_PROTECTION_ENABLE(MCU_WRITE_PROTECTION_DISABLE_ALL);
            UTL_Config_BufferMode(DUMP_BUFF_AP_MODE);

            /*
            GDI_ClearRegion(0); //clear region 0
            {
            extern PARM_DRAW_STRING __OSDNDTempStringAttr;

            __OSDNDTempStringAttr.bTextColor = OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING;
            __OSDNDTempStringAttr.bColorKey = 0;
            __OSDNDTempStringAttr.bBackground = OSDND_PALETTE_ENTRY_BACKGROUND;
            __OSDNDTempStringAttr.wX = DBG_AUTOUPGRADE_STRING_START_H;
            __OSDNDTempStringAttr.wY = DBG_AUTOUPGRADE_STRING_START_V;
            GDI_DrawString_909(0, &__OSDNDTempStringAttr, aDBGUpgrade);
            }
            */

            HAL_DISABLE_INTERRUPTS(_dwGlobalSaveInt);

            _bpDBGROMBuf = &__bPool[LENGTH_SHAREPOOL - ROW_SIZE - 6];

            _PutCHAR(DBG_DEBUG_HI, ACK_PREPARE_AUTO_UPGRADE_CMD);

#ifdef SUPPORT_WATCH_DOG
            HAL_WatchDog_Enable(FALSE);
#endif

        }
        else
        {
            _DBG_EndCommand(bResult);
            _PutCHAR(DBG_DEBUG_HI, ERROR_PREPARE_AUTO_UPGRADE_CMD);
        }
        break;
    case AUTO_UPGRADE_CODE_ADDRESS:
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
        if (bData == END_CMD)
        {
            _PutCHAR(DBG_DEBUG_HI, ACK_AUTO_UPGRADE_CODE_ADDRESS);
            _PutCHAR(DBG_DEBUG_HI, HIBYTE(HIWORD(DS_AP_CODE_AREA)));
            _DBG_DelayTime(DBG_DELAY_TIME);
            _PutCHAR(DBG_DEBUG_HI, LOBYTE(HIWORD(DS_AP_CODE_AREA)));
            _DBG_DelayTime(DBG_DELAY_TIME);
            _PutCHAR(DBG_DEBUG_HI, HIBYTE(LOWORD(DS_AP_CODE_AREA)));
            _DBG_DelayTime(DBG_DELAY_TIME);
            _PutCHAR(DBG_DEBUG_HI, LOBYTE(LOWORD(DS_AP_CODE_AREA)));
        }
        else
        {
            _DBG_EndCommand(bResult);
            _PutCHAR(DBG_DEBUG_HI, ERROR_AUTO_UPGRADE_CODE_ADDRESS);
        }
        break;
    case AUTO_UPGRADE_CMD:
        //Get size.
        _dwDBGUpgradeFileSize = (DWORD)(_DBG_ReadInBuffer()) << 24;
        _dwDBGUpgradeFileSize |= (DWORD)(_DBG_ReadInBuffer()) << 16;
        _dwDBGUpgradeFileSize |= (DWORD)(_DBG_ReadInBuffer()) << 8;
        _dwDBGUpgradeFileSize |= _DBG_ReadInBuffer();
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
        if (bData == END_CMD)
        {
            _bDBGBinMode = FALSE;
            HAL_RESTORE_INTERRUPTS( _dwGlobalSaveInt );
            AP_Loader(SRC_FROM_DRAM, DS_AP_CODE_AREA, _dwDBGUpgradeFileSize);
        }
        break;
    case EXIT_AUTO_UPGRADE_CMD:
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
        if (bData == END_CMD)
        {
            __dwDebugFlag = _dwDebugFlagBackup;
            _bAutoUpgrade = FALSE;
            HAL_RESTORE_INTERRUPTS( _dwGlobalSaveInt );

            //GDI_ClearRegion(0); //clear region 0
            DISP_DisplayCtrl(DISP_MAINVIDEO, TRUE);
            _PutCHAR(DBG_DEBUG_HI, ACK_EXIT_AUTO_UPGRADE_CMD);

#ifdef SUPPORT_WATCH_DOG
            HAL_WatchDog_Enable(TRUE);
#endif
        }
        else
        {
            _DBG_EndCommand(bResult);
            _PutCHAR(DBG_DEBUG_HI, ERROR_EXIT_AUTO_UPGRADE_CMD);
        }
        break;
#endif  // #ifdef DBG_SOLUTION_DEPENDENCY
    default :
        _PutCHAR(DBG_DEBUG_HI, _bDBGCurrentCommand);
        break;
   }
}

//***************************************************************************
//  Function    :   _DBG_ProcessWriteData
//  Abstract    :
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_ProcessWriteData(VOID)
{
    BYTE bIndex;
    WORD wIndex;
    WORD wGetCheckSum, wCalculateCheckSum;
    DWORD dwAddress;

    wGetCheckSum = _bpDBGROMBuf[_wDBGROMSize-5]; //get the check sum
    wGetCheckSum <<= 8;
    wGetCheckSum |= _bpDBGROMBuf[_wDBGROMSize-6];

    dwAddress = 0;
    for (bIndex = 0; bIndex < 4; bIndex++) //calculate download address
    {
        dwAddress <<= 8;
        dwAddress |= _bpDBGROMBuf[_wDBGROMSize-4+bIndex];
    }

    wCalculateCheckSum = 0; //calculate check sum
    for (_wDBGROMdataBufIndex = 0; _wDBGROMdataBufIndex < (_wDBGROMSize-6); _wDBGROMdataBufIndex++)
    {
        wCalculateCheckSum += (WORD)_bpDBGROMBuf[_wDBGROMdataBufIndex];
    }

    _bDBGChangeRxToRomBuffer = FALSE;

    if (wCalculateCheckSum != wGetCheckSum)
        _PutCHAR(DBG_DEBUG_HI, ACK_RECEIVE_WRONG_DATA);
    else  //cheksum is equal, write to DRAM to do auto upgrade*/
    {
        for (wIndex = 0; wIndex < (_wDBGROMSize-6); wIndex+= 4)
        {
            *((volatile DWORD *)dwAddress) = ((DWORD)(_bpDBGROMBuf[wIndex]) << 24) | ((DWORD)(_bpDBGROMBuf[wIndex+1]) << 16) | ((DWORD)(_bpDBGROMBuf[wIndex+2]) << 8) | (DWORD)(_bpDBGROMBuf[wIndex+3]);

            dwAddress += 4;
        }
        _PutCHAR(DBG_DEBUG_HI, ACK_RECEIVE_ROM_DATA);
    }
}

//***************************************************************************
//  Function    :   _DBG_ProcessReadDRAM
//  Abstract    :   This function will read DRAM for binary mode.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_ProcessReadDRAM(VOID)
{
    BYTE bIndex;
    DWORD dwIndex;
    DWORD dwAddress, dwSize, dwValue;

#ifdef SUPPORT_WATCH_DOG
    HAL_WatchDog_Enable(FALSE);
#endif

    dwAddress = 0;
    for (bIndex = 0; bIndex < 4; bIndex++) //address
    {
        dwAddress <<= 8;
        dwAddress |= _bpDBGROMBuf[bIndex];
    }

    dwSize = 0;
    for (bIndex = 0; bIndex < 4; bIndex++) //size
    {
        dwSize <<= 8;
        dwSize |= _bpDBGROMBuf[bIndex+4];
    }

    _bDBGChangeRxToRomBuffer = FALSE;

    for (dwIndex = 0; dwIndex < dwSize; dwIndex++)
    {
        dwValue = *((volatile DWORD *)dwAddress);

        _PutCHAR(DBG_DEBUG_HI, HIBYTE(HIWORD(dwValue)));
        _DBG_DelayTime(DBG_DELAY_TIME);
        _PutCHAR(DBG_DEBUG_HI, LOBYTE(HIWORD(dwValue)));
        _DBG_DelayTime(DBG_DELAY_TIME);
        _PutCHAR(DBG_DEBUG_HI, HIBYTE(LOWORD(dwValue)));
        _DBG_DelayTime(DBG_DELAY_TIME);
        _PutCHAR(DBG_DEBUG_HI, LOBYTE(LOWORD(dwValue)));
        _DBG_DelayTime(DBG_DELAY_TIME);

        dwAddress += 4;
    }

#ifdef SUPPORT_WATCH_DOG
    HAL_WatchDog_Enable(TRUE);
#endif
}


//***************************************************************************
//  Function    :   _DBG_ProcessWriteDRAM
//  Abstract    :   This function will write DRAM for binary mode.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_ProcessWriteDRAM(VOID)
{
    BYTE bIndex;
    DWORD dwAddress, dwValue;

    dwAddress = 0;
    for (bIndex = 0; bIndex < 4; bIndex++)
    {
        dwAddress <<= 8;
        dwAddress |= _bpDBGROMBuf[bIndex];
    }

    dwValue = 0;
    for (bIndex = 0; bIndex < 4; bIndex++)
    {
        dwValue <<= 8;
        dwValue |= _bpDBGROMBuf[bIndex+4];
    }

    *((volatile DWORD *)dwAddress) = dwValue;

    _bDBGChangeRxToRomBuffer = FALSE;

    // Chuan.Check, Miss declaration
    _PutCHAR(DBG_DEBUG_HI, ACK_COMPLETE_WRITE_DRAM);
}

//***************************************************************************
//  Function    :   _DBG_EndCommand
//  Abstract    :
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID    _DBG_EndCommand(BYTE bCurrentResult)
{
    BYTE bResult = bCurrentResult;
    BYTE bData = DBG_EMPTY_DATA_IN_BUFFER;

    while ((bData!=END_CMD) && (bResult==TRUE))
    {
        bResult = _DBG_ReadAndCheckInBuffer(&bData);
    }
}

//**********************************************************************
//  Function    :   _DBG_ReSyncDebugLevel
//  Abstract    :   Notify Serial Debug Tool to re-sync the debugging level.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************
VOID    _DBG_ReSyncDebugLevel(VOID)
{
    _PutCHAR(DBG_DEBUG_HI, RESYNC_DEBUG_LEVEL);
}

//**********************************************************************
//  Function    :   _DBG_DelayTime
//  Abstract    :
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************
VOID    _DBG_DelayTime(DWORD dwDelayTime)
{
    DWORD dwDelay;

    for (dwDelay = 0; dwDelay < dwDelayTime; dwDelay++);
}

#endif  // #ifdef SERIAL_DEBUG


// J300/Ben, Aug/15/2005
//********************************************************************
#ifdef AutoTuning_MPEG //Changed and Added Functions by AutoTuning ***

//Head of AutoTuning add function 1/3
//***************************************************************************
//  Function    :   AT_CODE_Interpreter
//  Abstract    :   This function will be called in the Timer to receive command.
//  Arguments   :   none.
//  Return      :   TRUE: Debug Monitor can't print anything in printf.
//              :   FALSE: Debug Monitor can print the content in printf.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
BYTE AT_CODE_Interpreter(void)
{
   BYTE bCh;

   if (_bDBGRXReady == TRUE)
   {
        //The command is not processed. Ignore the new command.
        return FALSE;
   }

   //original
   //if (!_DBG_ReceiveChar(&bCh))
   //Ben, May/04/2006
   if (!HAL_UART_ReceiveChar(&bCh, _dwUartPort_RX))
   {
        return FALSE; //No data
   }

   /*
   //if ( (bCh == eCOS_AT_Disable) && (AutoTuning_Enable_MPEG == TRUE) )
   if (bCh == eCOS_AT_Disable)
   {
      cyg_flag_setbits(&autotune_flag_var, FLAG_AUTOTUNE_DISABLE);
      AutoTuning_Enable_MPEG = FALSE;
   }
   */

   //MFC_to_eCOS_AT_CODE Interpreter
   switch(bCh)
   {
        case eCOS_AT_Command :
            printf("kenny recive eCOS_AT_Command");
            AutoTune_Receive_Command();
            cyg_flag_setbits(&autotune_flag_var, FLAG_AUTOTUNE_COMMAND);
            break;

        case eCOS_AT_Paras :
            //AutoTune_Receive_Paras();
            //cyg_flag_setbits(&autotune_flag_var, FLAG_AUTOTUNE_PARAS);
            printf("kenny recive eCOS_AT_Paras");
            AutoTune_Receive_Command();
            if ( AutoTune_Receive_Paras() )
            {
                AutoTune_Receive_Paras_OK = TRUE;
                cyg_flag_setbits(&autotune_flag_var, FLAG_AUTOTUNE_COMMAND);
            }
            else
            {
                AutoTune_Receive_Paras_OK = FALSE;
                cyg_flag_setbits(&autotune_flag_var, FLAG_AUTOTUNE_COMMAND);
            }
            break;

        case eCOS_AT_Disable :
            cyg_flag_setbits(&autotune_flag_var, FLAG_AUTOTUNE_DISABLE);
            //AutoTuning_Enable_MPEG = FALSE;
            break;

        default :

            break;
    }

   return FALSE;

}
//End of AutoTuning add function 1/3

//Head of AutoTuning add function 2/3
//***************************************************************************
//  Function    :   AutoTune_Receive_Command
//  Abstract    :   This function will be called in the MFC_to_eCOS_AT_CODE Interpreter
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
VOID AutoTune_Receive_Command(VOID)
{
   BYTE bCh;
   WORD i = 0;
   //original
   //WORD wait_command_timeout = 400;
   //Ben, Aug/18/2005
   WORD wait_command_timeout = 4000;
   WORD j = 0;

   //DWORD  Timer1_Counter;
   //REG_PLAT_INT_MASK_PRIORITY = REG_PLAT_INT_MASK_PRIORITY|INT_TIMER1;

    //Timer1_Counter = UTL_GetSysTimer();

    while(i <wait_command_timeout)
    //while ( (UTL_GetSysTimer() - Timer1_Counter) < COUNT_50_MSEC )
    {
        //original
        //if (!_DBG_ReceiveChar(&bCh))
        //Ben, May/04/2006
        if (!HAL_UART_ReceiveChar(&bCh, _dwUartPort_RX))
        {
            //SRVTM_Delay100us(10);
            for (j=0;j<200;j++)
            {
                i=i-1;
                i=i+1;
            }
            i++;
        }
        else
        {
            AT_CODE_Command = bCh;
            printf("\n\nkenny AT_CODE_Command = %x\n\n",bCh);//kenny 051205
            break;
        }
    }


    if (i==wait_command_timeout)
    {
        AT_CODE_Command = AT_CODE_Command_Fail;
    }

    //REG_PLAT_INT_MASK_PRIORITY = REG_PLAT_INT_MASK_PRIORITY&(~INT_TIMER1);

}
//End of AutoTuning add function 2/3

//Head of AutoTuning add function 3/3
//***************************************************************************
//  Function    :   AutoTune_Receive_Paras
//  Abstract    :   This function will be called in the MFC_to_eCOS_AT_CODE Interpreter
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
//VOID AutoTune_Receive_Paras(VOID)
BOOL AutoTune_Receive_Paras(VOID)
{
    BYTE bCh;
    BYTE Check_Sum;

    //original
    //BYTE bTemp_Low;
    //Eliminate Warning Message, Ben, June/08/2006
    BYTE bTemp_Low = 0;

    BYTE bTemp_High;
    WORD i = 0;
    WORD j = 0;
    WORD k = 0;
    WORD wait_command_timeout = 400;


    AT_CODE_Paras_Length = 0;

    //while (i < 10)
    while(i <wait_command_timeout)
    {
        //original
        //if (!_DBG_ReceiveChar(&bCh))
        //Ben, May/04/2006
        if (!HAL_UART_ReceiveChar(&bCh, _dwUartPort_RX))
        {
            //SRVTM_Delay100us(10);  //No data
            for (j=0;j<200;j++)
            {
                i=i-1;
                i=i+1;
            }
            i++;
        }
        else
        {
            AT_CODE_Paras_Length = bCh;
            printf("\n\nkenny AT_CODE_Paras_Length = %x\n\n",bCh);//kenny 051205
            break;
        }
    }

    if (AT_CODE_Paras_Length == 0)
    {
       //AutoTune_Receive_Paras_Check_Sum = 0xaa;
       //AutoTune_Receive_Paras_Check_Sum1 = 0xaa;

       return(FALSE);
    }
    /*
    // test
    else
    {
    	return(TRUE);
    }
    */

    Check_Sum=0;
    i=0;
    j=0;
    //while(i<AT_CODE_Paras_Length)
    while(i<=AT_CODE_Paras_Length*2)
    {
        //original
        //if (!_DBG_ReceiveChar(&bCh))
        //Ben, May/04/2006
        if (!HAL_UART_ReceiveChar(&bCh, _dwUartPort_RX))
        {
            //SRVTM_Delay100us(10);  //No data
            for (k=0;k<500;k++)
            {
                i=i-1;
                i=i+1;
            }
            j++;
            if (j >= 200)
            {
                //AT_CODE_Paras_Length = 0;
                //break;
                //AutoTune_Receive_Paras_Check_Sum = i;
                //AutoTune_Receive_Paras_Check_Sum1 = 0x55;
                return(FALSE);
            }
        }
        else
        {
            if (i<AT_CODE_Paras_Length*2)
            {
                if ( (i-i/2*2) == 0 )
                {
                    //AT_CODE_Paras[i] = bCh;
                    bTemp_Low = bCh;
                }
                else
                {
                    bTemp_High = bCh;
                    AT_CODE_Paras[i/2]=(WORD)(bTemp_Low)+(WORD)(bTemp_High)*256;
                }
                Check_Sum = Check_Sum + bCh;
            }
            else
            {
                AutoTune_Receive_Paras_Check_Sum = bCh;
                AutoTune_Receive_Paras_Check_Sum1 = Check_Sum;
                if ( bCh==Check_Sum )
                {
                    //Check_Sum OK!
                    return(TRUE);
                }
                else
                {
                    //Check_Sum Fail!
                    //AutoTune_Receive_Paras_Check_Sum = 0x66;
                    //AutoTune_Receive_Paras_Check_Sum1 = 0x66;
                    return(FALSE);
                }
            }
            i++;
            j=0;
        }
    }

    //Eliminate Warning Message, Ben, June/08/2006
    return(FALSE);

}
//End of AutoTuning add function 3/3
//*****************************************************************************
#endif //#ifdef AutoTuning_MPEG //Changed and Added Functions by AutoTuning ***
//*****************************************************************************
