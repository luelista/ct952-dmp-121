#include "winav.h"
#include "ctkav.h"
#include "usbsrc.h"
#include "srcfilter.h"
#include "comutl.h"
#include "infofilter.h"
#include "cc.h"
#include "parser.h"
#include "initial.h"
#include "utl.h"
#include "media.h"
// wyc2.71-portusb,
#if (USB_SYSTEM == USB2_0_JUNGO)
#include "usbwrap.h"
#elif (USB_SYSTEM == USB1_1_CT909R)
#include "usb.h"
#endif //

#ifdef CTK_CARDREADER
#include "card.h"
#endif

#define USBSRC_DEBUG_INFO 
// wyc1.01-909, new module for USB source.
#ifdef SUPPORT_USB_SOURCE
// ============================= Definition ============================= //
#include "debug.h"
#undef PRINTF
#undef printf
#define PRINTF(x, y...)     DBG_Printf(DBG_THREAD_USB, DBG_INFO_MM_INFOFTR, x, ## y)
#define printf(x, y...)     DBG_Printf(DBG_THREAD_USB, DBG_INFO_PRINTF, x, ## y)

    // wyc2.16-909S
    void    PRINTF_USBSRC(char *fmt, ...);
    void    printf_USBSRC(char *fmt, ...);

    // #define PRINTF PRINTF_USBSRC
    // #define printf printf_USBSRC

#define USB_MONITOR_WORKING     1
#define USB_MONITOR_WORK_END    0
    // wyc1.02-909, move define to header file.
    // the reading cluster one cmd issued. Default one cluster is 4
    // sectors.
    // Seemed USB can read max. 4x2048(PARSEC is 2048 bytes) bytes per
    // CMD, current set to 2x2048 bytes.
#define USBSRC_WRITE_PARSEC_ONE_CMD     2

#define FAT12_CLUSTER_END \
    ((_dwUSBSRCWriteCluster == FAT12_TABLE_CLUS_END) && (__bFileSystem == FILE_SYSTEM_FAT12) ? TRUE : FALSE)
#define FAT16_CLUSTER_END \
    ((_dwUSBSRCWriteCluster == FAT16_TABLE_CLUS_END) && (__bFileSystem == FILE_SYSTEM_FAT16) ? TRUE : FALSE)
#define FAT32_CLUSTER_END \
    ((_dwUSBSRCWriteCluster == FAT32_TABLE_CLUS_END) && (__bFileSystem == FILE_SYSTEM_FAT32) ? TRUE : FALSE)
#define FAT_CLUSTER_END \
    (FAT12_CLUSTER_END | FAT16_CLUSTER_END | FAT32_CLUSTER_END) ? TRUE : FALSE
// wyc2.78-909P, add new commands into the list
#define USBSRC_SUPPORT_CMD \
    USBSRC_CMD_CHECK_DEVICE | USBSRC_CMD_RECOGNIZE_MEDIA | USBSRC_CMD_READDATA | USBSRC_CMD_DUMPDATA | \
    USBSRC_CMD_STOP | USBSRC_CMD_READDATA_1 | USBSRC_CMD_SWITCH_USB_STACK | USBSRC_CMD_DEVICE_MODE_CHK_CARD | USBSRC_CMD_EXIT_USB_STACK

    // ============================= Variables =============================
    DWORD   __dwUSBSRCPhyOffset;
    BYTE    __bUSBSecPerPar;
    DWORD   _dwUSBSRCDumpSecNO;
    DWORD   _dwUSBSRCDumpNum;
    DWORD   *_dwUSBSRCDumpBuff;

    volatile DWORD   __dwUSBSRCStreamId; // record the processing stream
    // ============ variables declaration ============ //
    FLAG_T  _fUSBSRCCmdd;
    FLAG_T  _fUSBSRCCmddRunning;
    FLAG_T  _fUSBSRCCmddStatus;
    BYTE    _bUSBSRCState; 
    BYTE    _bUSBSRCReadStatus;
    WORD    __wUSBSRCCluSize;

    DWORD   _dwUSBSRCWriteCluster1[MAX_STREAM_COUNT];
    DWORD   _bUSBSRCWriteClusEnd1[MAX_STREAM_COUNT];

    // elmer2.78
    BYTE __bUSBSRCReadError[MAX_STREAM_COUNT];

    BYTE    _bSource = 0;
    // wyc2.78-909P, new vairables to issue commands and record the usb otg mode.
    volatile BYTE    _bUSBOTGCmdMode = USB_MODE_NONE;
    volatile DWORD   __dwUSBOTGMode = USB_MODE_NONE;
    BYTE    _bUSBSRCInitUSBStack = FALSE;
#ifdef SUPPORT_USB_HOST_DEVICE
#ifdef CTK_CARDREADER
    BYTE    __bUSBSRCCardType = CARD_CONTROLLER_NONE;
    BYTE    __bUSBSRCCardReady = FALSE;
    BYTE    __bCardDetectStage = 0;
    DWORD   __dwCardInitChkTime;
#endif //
#endif //

    BYTE    __bTestLun = 0xFF;
    // wyc2.77-909p, the usb performance testing code.
#ifdef TEST_DEVICE_PERFORMANCE
    #define USB_TEST_TIMES          (2)
    #define USB_TEST_START_SEC      (500)
    #define USB_TEST_LOOP           (1000)
    #define USB_CHUNK_SIZE          (32)
    #define USB_TEST_DUMP_IN_BUFF   (0x40300000L)
    DWORD   __dwTestUSBSpeed = FALSE;
    DWORD   __dwTestUSBTime[USB_TEST_TIMES];
    DWORD   __dwTestUSBCnt = 0;
#endif //
    // ============================= Function Prototype =============================
    void    _USBSRC_InitialVars(void);
// wyc2.71-portusb,
#if (USB_SYSTEM == USB2_0_CHIPIDEA)
#define  USB_DEVICE_IDLE                   (0)
#define  USB_DEVICE_ATTACHED               (1)
#define  USB_DEVICE_CONFIGURED             (2)
#define  USB_DEVICE_SET_INTERFACE_STARTED  (3)
#define  USB_DEVICE_INTERFACED             (4)
#define  USB_DEVICE_DETACHED               (5)
#define  USB_DEVICE_OTHER                  (6)
void USB_Init(void);
void USB_CheckState(void);
BYTE USB_MSC_Read(DWORD dwStartSector, DWORD dwNumOfSector, DWORD* dwBuffAddr);
DWORD USB_GetUSBState(void);

#endif //

void PRINTF_USBSRC(char *fmt, ...)
{
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_MM_INFOFTR, fmt);
}

void printf_USBSRC(char *fmt, ...)
{
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, fmt);
}

void USBSRC_ConfigSource(BYTE bSRC)
{
    _bSource = bSRC;
}

BYTE USBSRC_TriggerCmdAPI(BYTE flag, DWORD dwCmd)
{
    switch (flag)
    {
        case SRCFILTER_FLAG_CMD:
            return USBSRC_TriggerCmd(&_fUSBSRCCmdd, dwCmd);
        case SRCFILTER_FLAG_STATUS:
            return USBSRC_TriggerCmd(&_fUSBSRCCmddStatus, dwCmd);
        case SRCFILTER_FLAG_RUNNING:
            return USBSRC_TriggerCmd(&_fUSBSRCCmddRunning, dwCmd);
        default:
            return FALSE;
    }
}

BYTE USBSRC_PeekCmdAPI(BYTE flag, DWORD dwCmd)
{
    switch (flag)
    {
        case SRCFILTER_FLAG_CMD:
            return USBSRC_PeekCmd(&_fUSBSRCCmdd, dwCmd);
        case SRCFILTER_FLAG_STATUS:
            return USBSRC_PeekCmd(&_fUSBSRCCmddStatus, dwCmd);
        case SRCFILTER_FLAG_RUNNING:
            return USBSRC_PeekCmd(&_fUSBSRCCmddRunning, dwCmd);
        default:
            return FALSE;
    }
}

void USBSRC_ClearCmdAPI(BYTE flag, DWORD dwCmd)
{
    switch (flag)
    {
        case SRCFILTER_FLAG_CMD:
            USBSRC_ClearCmd(&_fUSBSRCCmdd, dwCmd);
            break;
        case SRCFILTER_FLAG_STATUS:
            USBSRC_ClearCmd(&_fUSBSRCCmddStatus, dwCmd);
            break;
        case SRCFILTER_FLAG_RUNNING:
            USBSRC_ClearCmd(&_fUSBSRCCmddRunning, dwCmd);
            break;
        default:
            break;
    }
}

BYTE USBSRC_TriggerCmd(FLAG_T *flag, DWORD dwCmd)
{
    if ((OS_PeekFlag(&__fThreadInit) & INIT_SRC_THREAD_USB_DONE))
    {
        if (flag == &_fUSBSRCCmdd)
        {
            // if RECOGNIZE_MEDIA command is processing, then CHECK_DEVICE command cannot be triggered.
            if (USBSRC_PeekCmd(&_fUSBSRCCmddRunning, USBSRC_CMD_RECOGNIZE_MEDIA) && (dwCmd == USBSRC_CMD_CHECK_DEVICE))
            {
                //printf("Trigger USBSRC Command CHECK_DEVICE failed\n");

                return FALSE;
            }

            // if command is processing, then the same command cannot be triggered.
            if (!USBSRC_PeekCmd(&_fUSBSRCCmddRunning, dwCmd))
            {
                OS_SetFlag(&_fUSBSRCCmdd, dwCmd);
                OS_SetFlag(&_fUSBSRCCmddRunning, dwCmd);

                //printf("Trigger USBSRC Command: %lx\n", dwCmd);

                return TRUE;
            }
        }
        else
        {
            OS_SetFlag(flag, dwCmd);

            return TRUE;
        }
    }
    
    return FALSE;
}

void USBSRC_ClearCmd(FLAG_T *flag, DWORD dwCmd)
{
    OS_ClearFlag(flag, ~dwCmd);
}


void USBSRC_WaitCmd(FLAG_T *flag, DWORD dwCmd)
{
    OS_WaitFlag(flag, dwCmd, FLAG_WAITMODE_AND);
}

BYTE USBSRC_PeekCmd(FLAG_T *flag, DWORD dwCmd)
{
    if (OS_PeekFlag(flag) & dwCmd)
        return TRUE;

    return FALSE;
}
// ***********************************************************************
// Function : USBSRC_Thread
// Description : The entry of USB Source thread.
// Arguments : index: the thread ID
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************
void USBSRC_Thread(DWORD index)
{
    DWORD wUSBSRCWaitFlag;

    // initialize relative flags to signal the running of USBSRC
    OS_SetFlag(&__fThreadInit, INIT_SRC_THREAD_USB_DONE);

    // first initialization
    _USBSRC_InitialVars();
    USBSRC_ClearUSBSRCState();

    // Because we must only initialize these flags once, so we do it here.
    OS_InitFlag(&_fUSBSRCCmdd);
    OS_InitFlag(&_fUSBSRCCmddRunning);
    OS_InitFlag(&_fUSBSRCCmddStatus);
    // removed, move to cc.c
/*
#ifdef SUPPORT_POWERON_MENU
    // wyc2.71-portusb,
    // wyc2.75-909P, don't call it here and move to media.c.
    // when used in CT95x, we initialize USB2_0 here
    USB_HCInit(0,0x40604000,0xE6000);
    //USB_HCInit(0,0);
#endif //
*/
    //wyc2.78-909P, init the usb stack when usbsrc init OK to save time when later code will use usb function.
    if (_bUSBSRCInitUSBStack == FALSE)
#ifdef SUPPORT_USB_HOST_DEVICE
    {
        CARD_InitSystem();
        OS_DelayTime(COUNT_300_MSEC);
        CARD_InitCard();
        __bUSBSRCCardType = CARD_CONTROLLER_SD;
        __bUSBSRCCardReady = TRUE;
        USB_ConfigDeviceMemPool(DS_USB_DEVICE_SCSI_POOL_ST, (DS_USB_DEVICE_SCSI_POOL_END - DS_USB_DEVICE_SCSI_POOL_ST));
        USB_HCInit(USB_MODE_DEVICE, 0,DS_USB_MEMPOOL_ST_DEVICE,(DS_USB_MEMPOOL_END_DEVICE-DS_USB_MEMPOOL_ST_DEVICE));
        // wyc2.78-909P
        //OS_DelayTime(COUNT_200_MSEC);
        //USB_HCExit();
        //__dwUSBOTGMode = USB_MODE_EXIT_MODE;
        __dwUSBOTGMode = USB_MODE_DEVICE;
        _bUSBSRCInitUSBStack = TRUE;
    }
#else
    {   
        USB_HCInit(USB_MODE_HOST, 0,DS_USB_MEMPOOL_ST_HOST,(DS_USB_MEMPOOL_END_HOST-DS_USB_MEMPOOL_ST_HOST));
        __dwUSBOTGMode = USB_MODE_HOST;
        _bUSBSRCInitUSBStack = TRUE;
    }
#endif //
    while (1)
    {
        wUSBSRCWaitFlag = OS_WaitFlag(&_fUSBSRCCmdd, USBSRC_SUPPORT_CMD, FLAG_WAITMODE_OR);

        // [Command]: USBSRC_CMD_RECOGNIZE_MEDIA
        if (wUSBSRCWaitFlag & USBSRC_CMD_RECOGNIZE_MEDIA)
        {
            //printf("USBSRC: Processing command (Recognize Media)\n");

            _bUSBSRCState = USBSRC_CheckUSBStatus(__SF_SourceGBL[__dwSFStreamID].bSourceIndex);

            // clear the command when finishing the corresponding processing operations, 
            // and trigger the status command to notify CC
            USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_RECOGNIZE_MEDIA);
            USBSRC_TriggerCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_RECOGNIZE_MEDIA);
            OS_YieldThread();
        }

        // [Command]: USBSRC_CMD_CHECK_DEVICE
        if (wUSBSRCWaitFlag & USBSRC_CMD_CHECK_DEVICE)
        {
            //printf("USBSRC: Processing command (Check Device)\n");
#ifdef TEST_DEVICE_PERFORMANCE
            {
                // wyc2.77-909p, the usb performance testing code.
                DWORD i;
                if (__dwTestUSBSpeed == TRUE)
                {
                    printf("\n======== Start the testing ========");
                    __dwTestUSBSpeed = OS_GetSysTimer();
                    for (i=USB_TEST_START_SEC; i<USB_TEST_START_SEC+USB_TEST_LOOP; i++ )
                    {
                        USB_ReadData(i, USB_CHUNK_SIZE, USB_TEST_DUMP_IN_BUFF);
                    }
                    __dwTestUSBTime[__dwTestUSBCnt]=OS_GetSysTimer()-__dwTestUSBSpeed;
                    __dwTestUSBSpeed = TRUE;
                    printf("\n======== Complete the testing ========");
                    __dwTestUSBCnt++;
                    if (__dwTestUSBCnt >= USB_TEST_TIMES)
                    {
                        __dwTestUSBSpeed = 0;
                        for (i=0; i<USB_TEST_TIMES; i++)
                        {
                            __dwTestUSBSpeed += (__dwTestUSBTime[i]*2);
                        }
                        __dwTestUSBSpeed /= USB_TEST_TIMES;
                        __dwTestUSBCnt = ((USB_TEST_LOOP*USB_CHUNK_SIZE*512*8)/__dwTestUSBSpeed);
                        printf("\nAverage bitrate: %lx K - bps",__dwTestUSBCnt);
                        __dwTestUSBCnt = 0;
                        __dwTestUSBSpeed = FALSE;
                        
                    }
                }
            }
#endif //

            if (USBSRC_CheckConnect())
            {
                switch (_bUSBSRCState)
                {
                    case USBSRC_STATE_NO_MEDIA:
                        _bUSBSRCState = USBSRC_STATE_CONN_MEDIA;
                        break;
                    case USBSRC_STATE_NO_CARD:
#if (USB_SYSTEM != USB_NONE)
                        if (USB_CheckCard(__bTestLun))
                            _bUSBSRCState = USBSRC_STATE_READY_MEDIA;
                        else
                            _bUSBSRCState = USBSRC_STATE_NO_CARD;
#endif //
                        break;
                    case USBSRC_STATE_WRONG_MEDIA:
                        _bUSBSRCState = USBSRC_STATE_WRONG_MEDIA;
                        break;
                    case USBSRC_STATE_READY_MEDIA:
                        // it must check card in 909R, and it need not check in 909P because it use 
                        // USBSRC_CheckConnect() to check it
#if (USB_SYSTEM != USB_NONE)
                        if (!_bSource)
                            if (!USB_CheckCard(__bTestLun))
                                _bUSBSRCState = USBSRC_STATE_NO_CARD;
#endif //
                        break;
                    default:
                        break;
                }
            }
            else
            {
#ifdef SUPPORT_USB_HOST_DEVICE
                if (_bSource)
                    __bUSBSRCCardType = CARD_CONTROLLER_NONE;
#endif //
                _bUSBSRCState = USBSRC_STATE_NO_MEDIA;
            }

            // clear the command when finishing the corresponding processing operations, 
            // and trigger the status command to notify CC
            USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_CHECK_DEVICE);
            USBSRC_TriggerCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_CHECK_DEVICE);
            OS_YieldThread();
        }

        // Elmer070216, it could maybe be removed
        /*
        if (_bUSBSRCState == USBSRC_STATE_NO_CARD || _bUSBSRCState ==  USBSRC_STATE_NO_MEDIA || 
                _bUSBSRCState ==  USBSRC_STATE_WRONG_MEDIA)
        {
            wUSBSRCWaitFlag &= ~(USBSRC_CMD_READDATA | USBSRC_CMD_DUMPDATA | USBSRC_CMD_READDATA_1);

            {
                BYTE bTemp;

                for (bTemp = 0; bTemp < MAX_STREAM_COUNT; bTemp++)
                    USBSRC_EnableRunning(bTemp, FALSE);
            }
        }
        */

        // [Command]: USBSRC_CMD_STOP
        if (wUSBSRCWaitFlag & USBSRC_CMD_STOP)
        {
            //printf("USBSRC: Processing command (STOP)\n");
//            {
//                BYTE bTemp;
//
//                for (bTemp = 0; bTemp < MAX_STREAM_COUNT; bTemp++)
//                    USBSRC_EnableRunning(bTemp, FALSE);
//            }
            // clear the command when finishing the corresponding processing operations, 
            // and trigger the status command to notify CC
            USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_STOP);
            USBSRC_TriggerCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_STOP);
            OS_YieldThread();
        }

        // [Command]: USBSRC_CMD_READDATA
        if (wUSBSRCWaitFlag & (USBSRC_CMD_READDATA | USBSRC_CMD_READDATA_1))
        {
            //printf("USBSRC: Processing command (READ DATA)\n");

            _bUSBSRCReadStatus = TRUE;

            if (__SF_SourceGBL[__dwUSBSRCStreamId].bEnableRunning == TRUE)
            {
                USBSRC_Monitor();
                OS_YieldThread();
            }
            /*
            else
            {
                USBSRC_EnableRunning(__dwUSBSRCStreamId, FALSE);
            }
            */

            if (_bUSBSRCReadStatus == FALSE)
            {
                //_bUSBSRCState = USBSRC_STATE_NO_CARD;

                PRINTF("MONITOR Data Error ...\n");
                USBSRC_EnableRunning(__dwUSBSRCStreamId, FALSE);
                OS_YieldThread();
            }
        }

        // [Command]: USBSRC_CMD_DUMPDATA
        if (wUSBSRCWaitFlag & USBSRC_CMD_DUMPDATA)
        {
            //printf("USBSRC: Processing command (READ DATA)\n");

            _bUSBSRCReadStatus = TRUE;

            //if (__SF_SourceGBL[__dwUSBSRCStreamId].bEnableRunning == TRUE)
            {
#ifdef CTK_CARDREADER
                if (_bSource)
                    // for 909P
                    _bUSBSRCReadStatus = CARD_ReadSector((DWORD)(_dwUSBSRCDumpBuff), _dwUSBSRCDumpSecNO, _dwUSBSRCDumpNum);
                else
#endif
#if (USB_SYSTEM != USB_NONE)
                    _bUSBSRCReadStatus = USB_ReadData(_dwUSBSRCDumpSecNO, _dwUSBSRCDumpNum, (DWORD)_dwUSBSRCDumpBuff);
#else
                    _bUSBSRCReadStatus = FALSE;
#endif
            }

            if (_bUSBSRCReadStatus == FALSE)
            {
                //_bUSBSRCState = USBSRC_STATE_NO_CARD;

                PRINTF("DUMPDATA Error ...\n");
            }

            //USBSRC_EnableRunning(__dwUSBSRCStreamId, FALSE);
            USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_DUMPDATA);
            USBSRC_TriggerCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_DUMPDATA);
            OS_YieldThread();
        }

        // wyc2.78-909P, new commands to switch the usb hsot/device stack.
        if (wUSBSRCWaitFlag & USBSRC_CMD_SWITCH_USB_STACK)
        {
            if (_bUSBOTGCmdMode == USB_MODE_HOST)
            {
#ifdef USBSRC_DEBUG_INFO
                PRINTF("\nEnter HOST mode: CMD:%lx, USB mode:%lx",_bUSBOTGCmdMode,__dwUSBOTGMode);
#endif //
                if (__dwUSBOTGMode != USB_MODE_EXIT_MODE)
                    USB_HCExit();
                USB_HCInit(USB_MODE_HOST, 0,DS_USB_MEMPOOL_ST_HOST,(DS_USB_MEMPOOL_END_HOST-DS_USB_MEMPOOL_ST_HOST));
                __dwUSBOTGMode = USB_MODE_HOST;
            }
#ifdef SUPPORT_USB_HOST_DEVICE
            else if (_bUSBOTGCmdMode == USB_MODE_DEVICE)
            {
#ifdef USBSRC_DEBUG_INFO
                PRINTF("\nEnter DEVICE mode: CMD:%lx, USB mode:%lx",_bUSBOTGCmdMode,__dwUSBOTGMode);
#endif //
                USB_ConfigDeviceMemPool(DS_USB_DEVICE_SCSI_POOL_ST, DS_USB_DEVICE_SCSI_POOL_END - DS_USB_DEVICE_SCSI_POOL_ST);
                if (__dwUSBOTGMode != USB_MODE_EXIT_MODE)
                    USB_HCExit();
                USB_HCInit(USB_MODE_DEVICE, 0,DS_USB_MEMPOOL_ST_DEVICE,(DS_USB_MEMPOOL_END_DEVICE-DS_USB_MEMPOOL_ST_DEVICE));
                __dwUSBOTGMode = USB_MODE_DEVICE;
            }
#endif //
            _bUSBOTGCmdMode = USB_MODE_NONE;

            // clear the command when finishing the corresponding processing operations, 
            // and trigger the status command to notify CC
            USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_SWITCH_USB_STACK);
            USBSRC_TriggerCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_SWITCH_USB_STACK);
            OS_YieldThread();
        }

        // wyc2.78-909P, the commands to check if card is changed.
        if (wUSBSRCWaitFlag & USBSRC_CMD_DEVICE_MODE_CHK_CARD)
        {
#ifdef SUPPORT_USB_HOST_DEVICE
#ifdef CTK_CARDREADER

            BYTE    bType;
            bType = CARD_CardStatus_Inserted();
            if ((bType != CARD_CONTROLLER_NONE) && (__bUSBSRCCardType != bType))
            {
                __bUSBSRCCardType = bType;
                if (CARD_InitCard() != FALSE)
                {
                    __bUSBSRCCardReady = TRUE;
                    if ((USB_ConnectToPC() == TRUE) && (__dwUSBOTGMode == USB_MODE_DEVICE))
                    {
                        USB_ConfigDeviceMemPool(DS_USB_DEVICE_SCSI_POOL_ST, DS_USB_DEVICE_SCSI_POOL_END - DS_USB_DEVICE_SCSI_POOL_ST);
                        USB_HCExit();
                        USB_HCInit(USB_MODE_DEVICE, 0,DS_USB_MEMPOOL_ST_DEVICE,(DS_USB_MEMPOOL_END_DEVICE-DS_USB_MEMPOOL_ST_DEVICE));
                    }
                }
                else
                {
                    __bUSBSRCCardReady = FALSE;
                }
            }
            if (bType == CARD_CONTROLLER_NONE)
            {
                __bUSBSRCCardReady = FALSE;
                __bUSBSRCCardType = CARD_CONTROLLER_NONE;
            }
            __bCardDetectStage = 0;
            USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_DEVICE_MODE_CHK_CARD);
            USBSRC_TriggerCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_DEVICE_MODE_CHK_CARD);
            OS_YieldThread();
#endif
#endif //
        }
        // wyc2.78-909P, the commands to re-init the usb device stack.
#ifdef SUPPORT_USB_HOST_DEVICE
        if (wUSBSRCWaitFlag & USBSRC_CMD_EXIT_USB_STACK)
        {
            if (__dwUSBOTGMode != USB_MODE_EXIT_MODE)
            {
#ifdef USBSRC_DEBUG_INFO
                PRINTF("\nExit USB Stack: %hx",__dwUSBOTGMode);
#endif //               
                USB_HCExit();
            }
            __dwUSBOTGMode = USB_MODE_EXIT_MODE;
            USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_EXIT_USB_STACK);
            USBSRC_TriggerCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_EXIT_USB_STACK);
            OS_YieldThread();
        }
#endif //
        
    }
}

// ***********************************************************************
// Function : USBSRC_ThreadDelete
// Description : The delete function of USBSRC thread.
// Arguments : None
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************
void USBSRC_ThreadDelete(void)
{
    if ((OS_PeekFlag(&__fThreadInit) & INIT_SRC_THREAD_USB_DONE))
    {
        // [1] suspend the thread
        OS_SuspendThread(ThreadUSBSRC.handle);

        // [2] kill the desired thread
        OS_KillThread(ThreadUSBSRC.handle);

        // [3] clear the thread initial done flag
        OS_ClearFlag(&__fThreadInit, ~INIT_SRC_THREAD_USB_DONE);
    }
}

void USBSRC_ThreadSuspend(void)
{
    if ((OS_PeekFlag(&__fThreadInit) & INIT_SRC_THREAD_USB_DONE))
    {
        // [1] clear all the commands triggered
        USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_ALL);
        // [2] disable triggering any command anymore
        USBSRC_TriggerCmd(&_fUSBSRCCmddRunning, USBSRC_CMD_ALL);

        // [3] force USBSRC to sleep state
        // 1 for SLEEPING state
        while (ThreadUSBSRC.space.state != 1)
            OS_YieldThread();

        OS_SuspendThread(ThreadUSBSRC.handle);

        // [4] clear the relative flags
        USBSRC_ClearCmd(&_fUSBSRCCmddRunning, USBSRC_CMD_ALL);
        USBSRC_ClearCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_ALL);
        //USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_ALL);

        // elmer070208, when suspended, clear this bit
        OS_ClearFlag(&__fThreadInit, ~INIT_SRC_THREAD_USB_DONE);
    }
}

void USBSRC_ThreadResume(void)
{
    if (!(OS_PeekFlag(&__fThreadInit) & INIT_SRC_THREAD_USB_DONE))
    {
        OS_SetFlag(&__fThreadInit, INIT_SRC_THREAD_USB_DONE);
        //_USBSRC_InitialVars();
        USBSRC_ClearUSBSRCState();
        OS_ResumeThread(ThreadUSBSRC.handle);
    }
}

// ***********************************************************************
// Function : USBSRC_Stop
// Description : Stop the USB Source running after calling this function.
// Arguments : None
// Return : TRUE (have stop monitor)
// FALSE(stop monitor fail)
// Side Effect :
// ***********************************************************************
// wyc2.53-909P,
BYTE USBSRC_Stop(DWORD dwStreamID, BYTE bRealStop)
{
    //DBG_Printf(DBG_THREAD_INFO_FILTER, DBG_INFO_PRINTF,"\nUSBSRC trigger command ( STOP... )");

    if ((OS_PeekFlag(&__fThreadInit) & INIT_SRC_THREAD_USB_DONE))
    {
        USBSRC_EnableRunning(dwStreamID, FALSE);
        OS_REORDER_BARRIER();
        USBSRC_TriggerCmd(&_fUSBSRCCmdd, USBSRC_CMD_STOP);
        OS_YieldThread();
        USBSRC_WaitCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_STOP);
        USBSRC_ClearCmd(&_fUSBSRCCmddRunning, USBSRC_CMD_STOP);
        USBSRC_ClearCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_STOP);

        if (bRealStop)
        {
            // set WriteSector to (EndSector + 1) to indicate the end of the undergoing file
            __SF_SourceGBL[dwStreamID].dwWriteSector = __SF_SourceGBL[dwStreamID].dwEndSector + 1;
            __SF_SourceGBL[dwStreamID].dwBlockOffset = 0;
        }
    }
    else
    {
        //
    }

    //__dwUSBSRCStreamId = dwStreamID;

    return TRUE;
}

// ***********************************************************************
// Function : USBSRC_ReadSectors
// Description : Read data from USB by requested start / end position.
// Arguments : dwStartPos: Start position, dwEndPos: End position.
// Return : TRUE/FALSE
// Side Effect :
// @dwStartPos: start sector(in sector unit)
// @dwEndPos: start sector + occupied how many parser sector size
// ***********************************************************************
BYTE USBSRC_ReadSectors(DWORD dwStreamID, DWORD dwStartPos, DWORD dwEndPos, BYTE bDirectRead)
{
    DWORD dwUSBSRCTemp1, dwUSBSRCTemp2, dwUSBSRCTemp3;

    __bUSBSecPerPar = PARSER_SECTOR_SIZE / FAT_GetFATInfo(FAT_INFO_BYTE_PER_USBSEC);

    // if __dwUSBStartPos == 0xFFFFFFFF, it stands for that it is through our FAT translating function
    if (__dwUSBStartPos != 0xFFFFFFFF)
    {
        __SF_SourceGBL[dwStreamID].dwStartSector = __dwUSBStartPos;
        __SF_SourceGBL[dwStreamID].dwEndSector = __dwUSBStartPos + (dwEndPos - dwStartPos);
        __dwUSBStartPos = 0xFFFFFFFF;
    }
    else
    {
        __SF_SourceGBL[dwStreamID].dwStartSector = dwStartPos;
        __SF_SourceGBL[dwStreamID].dwEndSector = dwEndPos;
        __SF_SourceGBL[dwStreamID].dwBlockOffset = 0;
    }

    _dwUSBSRCWriteCluster1[dwStreamID] = FAT_SectorToClus(dwStartPos);

    __SF_SourceGBL[dwStreamID].dwWriteSector = __SF_SourceGBL[dwStreamID].dwStartSector;
    __SF_SourceGBL[dwStreamID].dwReadSector = __SF_SourceGBL[dwStreamID].dwStartSector;

    dwUSBSRCTemp1 = __SF_SourceGBL[dwStreamID].dwUpperAddr - __SF_SourceGBL[dwStreamID].dwLowerAddr;
    dwUSBSRCTemp2 = dwUSBSRCTemp1 / PARSER_SECTOR_SIZE;
    dwUSBSRCTemp3 = dwUSBSRCTemp1 % PARSER_SECTOR_SIZE;
    _bUSBSRCWriteClusEnd1[dwStreamID] = FALSE;

    // let the size of the buffer be multiple of PARSER_SECTOR_SIZE(2048 bytes).
    if (dwUSBSRCTemp3)
    {
        __SF_SourceGBL[dwStreamID].dwUpperAddr =
            __SF_SourceGBL[dwStreamID].dwLowerAddr + dwUSBSRCTemp2 * PARSER_SECTOR_SIZE;
    }

    __SF_SourceGBL[dwStreamID].dwWriteAddr = __SF_SourceGBL[dwStreamID].dwLowerAddr;
    __SF_SourceGBL[dwStreamID].dwReadAddr = __SF_SourceGBL[dwStreamID].dwLowerAddr;

    // need to set data format for parser.
    __SF_SourceGBL[dwStreamID].dwDataFormat = PARSER_DATAFMT_LINEAR | PARSER_DATAFMT_DVD_2048;
    __SF_SourceGBL[dwStreamID].dwBuffLength = dwUSBSRCTemp2;

#ifdef USBSRC_DEBUG_INFO
    //printf("USBSRC Readsectors(%lx, %lx)", dwStartPos, dwEndPos);
#endif

    // elmer.fix
    if (bDirectRead)
    {
        USBSRC_EnableRunning(dwStreamID, TRUE);
        //USBSRC_IssueReadCmd(dwStreamID);
        //__dwUSBSRCStreamId = dwStreamID;
        
    }
    else
    {
        //__dwUSBSRCStreamId = (MAX_STREAM_COUNT - 1) - dwStreamID;
    }
    //USBSRC_EnableRunning(0, TRUE);
    //USBSRC_TriggerCmd(&_fUSBSRCCmdd, USBSRC_CMD_READDATA);

    return TRUE;
}

// ***********************************************************************
// Function : USBSRC_IssueReadCmd
// Description : Issue the read command to USB SRC
// Arguments : NO
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************
BYTE USBSRC_IssueReadCmd(DWORD dwStreamID)
{
    __dwUSBSRCStreamId = dwStreamID;
    OS_REORDER_BARRIER();

    // trigger command to read data
    if (dwStreamID == 0)
        OS_SetFlag(&_fUSBSRCCmdd, USBSRC_CMD_READDATA);
    else
        OS_SetFlag(&_fUSBSRCCmdd, USBSRC_CMD_READDATA_1);

    return TRUE;
}

// ***********************************************************************
// Function : USBSRC_GetCurrentPos
// Description : Read current reading cluster from USB source.
// Arguments : pCurrentPos: The current reading cluster.
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************
BYTE USBSRC_GetCurrentPos(PDWORD pCurrentPos)
{
    *pCurrentPos = __SF_SourceGBL[__dwUSBSRCStreamId].dwWriteSector;
    return TRUE;
}

// ***********************************************************************
// Function : USBSRC_Monitor
// Description : This function will monitor to read data from USB when
// actived and 
// : reading range is specified.
// Arguments : None
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************

BYTE USBSRC_Monitor(void)
{
    DWORD dwSaveInt;
    DWORD dwUSBSRCTemp;
    DWORD dwRemainedSec;
    DWORD dwToWriteSecNum;
    DWORD dwTotalSecNum;
    DWORD dwWriteSecNum;
    DWORD dwWriteStartSec;
    DWORD dwWriteStartAddr;
    LONG lUSBSRCTemp;

    dwRemainedSec = 0;
    dwWriteStartAddr = __SF_SourceGBL[__dwUSBSRCStreamId].dwWriteAddr;

    while (__SF_SourceGBL[__dwUSBSRCStreamId].dwWriteSector < (__SF_SourceGBL[__dwUSBSRCStreamId].dwEndSector + 1))
    {
        OS_DISABLE_INTERRUPTS(dwSaveInt);
        dwUSBSRCTemp = (__SF_SourceGBL[__dwUSBSRCStreamId].dwWriteSector - __SF_SourceGBL[__dwUSBSRCStreamId].dwReadSector);
        OS_RESTORE_INTERRUPTS(dwSaveInt);

        if (dwUSBSRCTemp >= __SF_SourceGBL[__dwUSBSRCStreamId].dwBuffLength)
        {
            if (dwUSBSRCTemp > __SF_SourceGBL[__dwUSBSRCStreamId].dwBuffLength)
            {
#ifdef USBSRC_DEBUG_INFO
                PRINTF("USBSRC write exceeds the length of dump-in buffer");
#endif
                return FALSE;
            }
            else
            {
#ifdef USBSRC_DEBUG_INFO
                //printf("Dump-in buffer is full of unused data");
#endif
                //continue;
                return TRUE;
            }
        }
        
        // configure the buffer length in order to get data
        // in CT909P, there are USB2_0_JUNGO & CTK_CARDREADER
#ifdef CT909P_IC_SYSTEM
        // wyc2.76-909P,
#ifdef SMALL_SOURCE_BUFFER
        if (dwUSBSRCTemp >= 1)
#else
        if (dwUSBSRCTemp >= __SF_SourceGBL[__dwUSBSRCStreamId].dwBuffLength / 3)
#endif //
        {
            OS_YieldThread();
            return TRUE;
        }
#endif

        // how many sectors we still must to read from USB
        dwTotalSecNum = ((__SF_SourceGBL[__dwUSBSRCStreamId].dwEndSector + 1) - __SF_SourceGBL[__dwUSBSRCStreamId].dwWriteSector)
            * __bUSBSecPerPar - dwRemainedSec;

        // how many sectors we can fill to the dump-in buffer
        OS_DISABLE_INTERRUPTS(dwSaveInt);
        if (__SF_SourceGBL[__dwUSBSRCStreamId].dwWriteAddr >= __SF_SourceGBL[__dwUSBSRCStreamId].dwReadAddr) 
            dwToWriteSecNum = (__SF_SourceGBL[__dwUSBSRCStreamId].dwUpperAddr - __SF_SourceGBL[__dwUSBSRCStreamId].dwWriteAddr) 
                / FAT_GetFATInfo(FAT_INFO_BYTE_PER_USBSEC);
        else
            dwToWriteSecNum = (__SF_SourceGBL[__dwUSBSRCStreamId].dwReadAddr - __SF_SourceGBL[__dwUSBSRCStreamId].dwWriteAddr) 
                / FAT_GetFATInfo(FAT_INFO_BYTE_PER_USBSEC);
        OS_RESTORE_INTERRUPTS(dwSaveInt);

        if (dwToWriteSecNum > dwTotalSecNum)
            dwToWriteSecNum = dwTotalSecNum;

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
        {
            extern BYTE __bMMJPEGAudioMode;
            BOOL PARSER_QueryESBufferReady(DWORD, DWORD);

            if ((__bMMJPEGAudioMode == 1))// && (__dwUSBSRCStreamId == 0))
            {
                if (dwToWriteSecNum > 8) // 4096 bytes
                {
                    if (!PARSER_QueryESBufferReady(__dwUSBSRCStreamId, 4096))
                        return FALSE;
                    else
                        dwToWriteSecNum = 8;
                }
                else
                {
                    if (!PARSER_QueryESBufferReady(__dwUSBSRCStreamId, dwToWriteSecNum * 512))
                        return FALSE;
                }
            }
        }
#endif

        while (dwToWriteSecNum)
        {
            dwWriteStartSec = FAT_ClusToSector(_dwUSBSRCWriteCluster1[__dwUSBSRCStreamId]) + __dwUSBSRCPhyOffset + 
                __SF_SourceGBL[__dwUSBSRCStreamId].dwBlockOffset;
            
            // the limitation of CT909R
            if (_bSource)
                // For 909P card reader, it has no limitation for 8 sectors per command, and the limitation is 65536
                dwWriteSecNum = dwToWriteSecNum;
            else
// wyc2.71-portusb,
#if (USB_SYSTEM == USB2_0_JUNGO)
                dwWriteSecNum = dwToWriteSecNum;
#elif (USB_SYSTEM == USB1_1_CT909R)
                if (dwToWriteSecNum > USBSRC_WRITE_PARSEC_ONE_CMD * __bUSBSecPerPar)
                    dwWriteSecNum = USBSRC_WRITE_PARSEC_ONE_CMD * __bUSBSecPerPar;
                else
                    dwWriteSecNum = dwToWriteSecNum;
#else
                ;
#endif //
            

            lUSBSRCTemp = dwWriteSecNum - 
                (FAT_GetFATInfo(FAT_INFO_SECTOR_PER_CLUSTER) - __SF_SourceGBL[__dwUSBSRCStreamId].dwBlockOffset);

            if (lUSBSRCTemp > 0)
            {
                dwWriteSecNum = FAT_GetFATInfo(FAT_INFO_SECTOR_PER_CLUSTER) - __SF_SourceGBL[__dwUSBSRCStreamId].dwBlockOffset;
                __SF_SourceGBL[__dwUSBSRCStreamId].dwBlockOffset = 0;

                while (lUSBSRCTemp)
                {
                    if ((FAT_GetNextClus(TRUE, _dwUSBSRCWriteCluster1[__dwUSBSRCStreamId]) - _dwUSBSRCWriteCluster1[__dwUSBSRCStreamId]) == 1)
                    {
                        _dwUSBSRCWriteCluster1[__dwUSBSRCStreamId]++;
                        //dwUSBSRCTemp = FAT_ClusToSector(_dwUSBSRCWriteCluster1) + __dwUSBSRCPhyOffset;
                        lUSBSRCTemp -= FAT_GetFATInfo(FAT_INFO_SECTOR_PER_CLUSTER);

                        if (lUSBSRCTemp >= 0)
                        {
                            dwWriteSecNum += FAT_GetFATInfo(FAT_INFO_SECTOR_PER_CLUSTER);
                            continue;
                        }
                        else
                        {
                            dwWriteSecNum += lUSBSRCTemp + FAT_GetFATInfo(FAT_INFO_SECTOR_PER_CLUSTER);
                            __SF_SourceGBL[__dwUSBSRCStreamId].dwBlockOffset = 
                                lUSBSRCTemp + FAT_GetFATInfo(FAT_INFO_SECTOR_PER_CLUSTER);
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                __SF_SourceGBL[__dwUSBSRCStreamId].dwBlockOffset += dwWriteSecNum;
            }

#ifdef CTK_CARDREADER
            if (_bSource)
                // for 909P
                _bUSBSRCReadStatus = CARD_ReadSector(dwWriteStartAddr, dwWriteStartSec, dwWriteSecNum);
            else
#endif
#if (USB_SYSTEM != USB_NONE)
                _bUSBSRCReadStatus = USB_ReadData(dwWriteStartSec, dwWriteSecNum, dwWriteStartAddr);
#else
                _bUSBSRCReadStatus = FALSE;
#endif 

            //printf("Source %d\: %lx, %lx\n", __dwUSBSRCStreamId, dwWriteStartSec, dwWriteSecNum);

            if (_bUSBSRCReadStatus == FALSE)
            {
#ifdef USBSRC_DEBUG_INFO
                PRINTF("USBSRC monitor error (WriteStartSec, WriteSecNum): (%lx, %lx)", dwWriteStartSec, dwWriteSecNum);
#endif
                __bUSBSRCReadError[__dwUSBSRCStreamId] = TRUE;
                return FALSE;
            }
#ifdef USBSRC_DEBUG_INFO
            else
                //printf("Read %lx sectors from %lx\n", dwWriteSecNum, dwWriteStartSec);
#endif
            dwToWriteSecNum -= dwWriteSecNum;

            dwWriteStartAddr += dwWriteSecNum * FAT_GetFATInfo(FAT_INFO_BYTE_PER_USBSEC);

            if (dwWriteStartAddr == __SF_SourceGBL[__dwUSBSRCStreamId].dwUpperAddr)
                dwWriteStartAddr = __SF_SourceGBL[__dwUSBSRCStreamId].dwLowerAddr;

            __SF_SourceGBL[__dwUSBSRCStreamId].dwWriteAddr = dwWriteStartAddr;
            OS_DISABLE_INTERRUPTS(dwSaveInt);
            __SF_SourceGBL[__dwUSBSRCStreamId].dwWriteSector += (dwWriteSecNum + dwRemainedSec) / __bUSBSecPerPar;
            OS_RESTORE_INTERRUPTS(dwSaveInt);
            dwRemainedSec = (dwWriteSecNum + dwRemainedSec) % __bUSBSecPerPar;

            if (!(__SF_SourceGBL[__dwUSBSRCStreamId].dwBlockOffset % FAT_GetFATInfo(FAT_INFO_SECTOR_PER_CLUSTER)))
            {
                __SF_SourceGBL[__dwUSBSRCStreamId].dwBlockOffset = 0;
                _dwUSBSRCWriteCluster1[__dwUSBSRCStreamId] = FAT_GetNextClus(TRUE, _dwUSBSRCWriteCluster1[__dwUSBSRCStreamId]);

                //if (FAT_CLUSTER_END)
                if (((_dwUSBSRCWriteCluster1[__dwUSBSRCStreamId] == FAT12_TABLE_CLUS_END) && (__bFileSystem ==            FILE_SYSTEM_FAT12))
                        || ((_dwUSBSRCWriteCluster1[__dwUSBSRCStreamId] == FAT16_TABLE_CLUS_END) && (__bFileSystem ==     FILE_SYSTEM_FAT16))
                        || ((_dwUSBSRCWriteCluster1[__dwUSBSRCStreamId] == FAT32_TABLE_CLUS_END) && (__bFileSystem ==     FILE_SYSTEM_FAT32)))

                {
#ifdef USBSRC_DEBUG_INFO
                    //printf("File End(%x)", _wWriteSectorNum);
#endif
                    dwToWriteSecNum = 0;
                    // must add this statement to notify PARSER that the file has reached end
                    __SF_SourceGBL[__dwUSBSRCStreamId].dwWriteSector = __SF_SourceGBL[__dwUSBSRCStreamId].dwEndSector + 1;
                    _bUSBSRCWriteClusEnd1[__dwUSBSRCStreamId] = TRUE;
                }
            }
        } // while (dwToWriteSecNum)

        if (_bUSBSRCWriteClusEnd1[__dwUSBSRCStreamId] == TRUE
                || __SF_SourceGBL[__dwUSBSRCStreamId].dwWriteSector > __SF_SourceGBL[__dwUSBSRCStreamId].dwEndSector)
        {
#ifdef USBSRC_DEBUG_INFO
            //printf("End work of USBSRC_Monitor");
#endif
            __SF_SourceGBL[__dwUSBSRCStreamId].dwWriteSector = __SF_SourceGBL[__dwUSBSRCStreamId].dwEndSector + 1;
            USBSRC_EnableRunning(__dwUSBSRCStreamId, FALSE);
            //__SF_SourceGBL[__dwUSBSRCStreamId].bEnableRunning = FALSE;

            return FALSE;
        }

        // elmer.test 909P
        // it needs not be setup here, when it returns, it will issue OS_YieldThread()
        //OS_YieldThread();

        return TRUE;
    } // while (__SF_SourceGBL[__dwUSBSRCStreamId].dwWriteSector < (__SF_SourceGBL[__dwUSBSRCStreamId].dwEndSector + 1))

    return TRUE;
}

// ***********************************************************************
// Function : USBSRC_DumpDatabySource
// Description : Dump the data to specific pointer address from source by
// wait source to read data.
// Arguments : dwSecNO: The dump location(logical location). dwNum: The
// dump sector num(Parser sector unit, 2048 bytes).
// : pFATTable: The dumped pointer.
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************
BYTE USBSRC_DumpDatabySource(DWORD dwSecNO, DWORD dwNum, DWORD * pFATBuff)
{
    _dwUSBSRCDumpSecNO = dwSecNO + __dwUSBSRCPhyOffset;
    _dwUSBSRCDumpNum = dwNum * __bUSBSecPerPar;
    _dwUSBSRCDumpBuff = pFATBuff;

    //USBSRC_EnableRunning(__dwUSBSRCStreamId, TRUE);
    USBSRC_TriggerCmd(&_fUSBSRCCmdd, USBSRC_CMD_DUMPDATA);
    OS_YieldThread();
    USBSRC_WaitCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_DUMPDATA);
    USBSRC_ClearCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_DUMPDATA);
    USBSRC_ClearCmd(&_fUSBSRCCmddRunning, USBSRC_CMD_DUMPDATA);
#ifdef USBSRC_DEBUG_INFO
    PRINTF("USBSRC dump %lx sectors from %lx ...", _dwUSBSRCDumpNum, _dwUSBSRCDumpSecNO);

    if (_bUSBSRCReadStatus == FALSE)
        PRINTF("Error\n");
    else
        PRINTF("Success\n");
#endif

    return _bUSBSRCReadStatus;
}

// ***********************************************************************
// Function : USBSRC_DumpData
// Description : Dump the data to specific pointer address from source 
// Arguments : dwSecNO: The dump location(logical location). dwNum: The
// dump sector num(Parser sector unit, 2048 bytes).
// : pFATTable: The dumped pointer.
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************
BYTE USBSRC_DumpData(DWORD dwSecNO, DWORD dwNum, DWORD * pFATBuff)
{
    dwSecNO += __dwUSBSRCPhyOffset;

#ifdef CTK_CARDREADER
    if  (_bSource)
        // for 909P
        _bUSBSRCReadStatus = CARD_ReadSector((DWORD)(pFATBuff), dwSecNO, dwNum * __bUSBSecPerPar);
    else
#endif
#if (USB_SYSTEM != USB_NONE)
        _bUSBSRCReadStatus = USB_ReadData(dwSecNO, dwNum * __bUSBSecPerPar, (DWORD)(pFATBuff));
#else
        _bUSBSRCReadStatus = FALSE;
#endif //


    if (_bUSBSRCReadStatus == FALSE)
    {
        __bUSBSRCReadError[__dwUSBSRCStreamId] = TRUE;
#ifdef USBSRC_DEBUG_INFO
        PRINTF("Dump Data ERROR[%lx, %lx, %lx, %lx]", dwSecNO, dwNum, __bUSBSecPerPar, (DWORD)(pFATBuff));
#endif
    }

    return _bUSBSRCReadStatus;
}

// ***********************************************************************
// Function : USBSRC_CheckUSBStatus
// Description : Check the status from USB S/W stack, status 
// Arguments : bUSBSRCID: The source ID to check status.
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************
BYTE USBSRC_CheckUSBStatus(BYTE bUSBSRCID)
{
#ifdef CTK_CARDREADER
    if (_bSource)
        // for 909P
        if (CARD_InitCard())
            return USBSRC_STATE_READY_MEDIA;
        else
            return USBSRC_STATE_WRONG_MEDIA;
    else
#endif
#if (USB_SYSTEM != USB_NONE)
        switch (USB_CheckStatus())
        {
            case USB_STATUS_NOT_SUPPORTED_DEVICE:
                return USBSRC_STATE_WRONG_MEDIA;
                break;

            case USB_STATUS_MASS_STORAGE_DEVICE:
                return USBSRC_STATE_READY_MEDIA;
                break;

            case USB_STATUS_NO_DEVICE:
                return USBSRC_STATE_NO_MEDIA;
                break;

            case USB_STATUS_NO_DISC:
                return USBSRC_STATE_NO_CARD;
                break;

            default:
                return USBSRC_STATE_NO_MEDIA;
                break;
        }
#else
        return FALSE;
#endif 
}

// wyc1.05-909, rename function name.
// ***********************************************************************
// Function : USBSRC_GetUSBSRCStatus
// Description : This function will check the status of media that insert
// in USB source
// : and return status to Srcfilter.
// Arguments : None
// Return : USBSRC_STATE_NO_MEDIA: No media. USBSRC_STATE_READY_MEDIA:
// Media ready. 
// : SBSRC_STATE_WRONG_MEDIA: Media wrong.
// Side Effect :
// ***********************************************************************
BYTE USBSRC_GetUSBSRCStatus(BYTE bSourceID)
{
    return _bUSBSRCState;
}
// ***********************************************************************
// Function : USBSRC_ClearUSBSRCStatus
// Description : This function clear USBSRC state to NO media to re-get
// USB status from devide.
// Arguments : None
// Return : NONE
// Side Effect :
// ***********************************************************************
void USBSRC_ClearUSBSRCState(void)
{
    BYTE bTemp;

    _bUSBSRCState = USBSRC_STATE_NO_MEDIA;

    for (bTemp = 0; bTemp < MAX_STREAM_COUNT; bTemp++)
    {
        __SF_SourceGBL[bTemp].dwBlockOffset = 0;
        __SF_SourceGBL[bTemp].bEnableRunning = FALSE;
    }

    // clear flags
    OS_ClearFlag(&_fUSBSRCCmdd, 0);
    OS_ClearFlag(&_fUSBSRCCmddStatus, 0);
    OS_ClearFlag(&_fUSBSRCCmddRunning, 0);

    __bUSBSRCReadError[0] = FALSE;
    __bUSBSRCReadError[1] = FALSE;

#if (USB_SYSTEM != USB_NONE)
    USB_ClearStatus();
#endif //
}

// ***********************************************************************
// Function : USB_EnableRunning
// Description : Control the USB Src running or not.
// Arguments : TRUE : Enable
// FALSE : Disable
// Return : TRUE : OK, FALSE: Fail
// Side Effect :
// ***********************************************************************
BOOL USBSRC_EnableRunning(DWORD dwStreamID, BYTE bRun)
{
    __SF_SourceGBL[dwStreamID].bEnableRunning = bRun;

    // if setting to FALSE, it should clear the READDATA command so that
    // it will not continue reading data
    if (bRun == FALSE)
    {
        if (dwStreamID == 0)
        {
            USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_READDATA);
            USBSRC_ClearCmd(&_fUSBSRCCmddRunning, USBSRC_CMD_READDATA);
            //USBSRC_ClearCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_READDATA);
        }
        else
        {
            USBSRC_ClearCmd(&_fUSBSRCCmdd, USBSRC_CMD_READDATA_1);
            USBSRC_ClearCmd(&_fUSBSRCCmddRunning, USBSRC_CMD_READDATA_1);
            //USBSRC_ClearCmd(&_fUSBSRCCmddStatus, USBSRC_CMD_READDATA_1);
        }
    }
    else
    {
        if (dwStreamID == 0)
            OS_SetFlag(&_fUSBSRCCmdd, USBSRC_CMD_READDATA);
        else
            OS_SetFlag(&_fUSBSRCCmdd, USBSRC_CMD_READDATA_1);

        OS_REORDER_BARRIER();
        __dwUSBSRCStreamId = dwStreamID;
    }

    return TRUE;
}

// ***********************************************************************
// Function : USBSRC_Abort
// Description : Abort the action of USB source.
// Arguments : None
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************
BYTE USBSRC_Abort(void)
{
    return TRUE;
}

// ***********************************************************************
// Function : _USBSRC_InitialVars
// Description : Inititl variables / flags / mutex that used by USB
// source.
// Arguments : None
// Return : None
// Side Effect :
// ***********************************************************************
void _USBSRC_InitialVars(void)
{
    // wyc1.01-909, cluster need to reference FAT, now temp assign here.
    __wUSBSRCCluSize = 2048;
    __dwUSBSRCPhyOffset = 0;
    _bUSBSRCReadStatus = TRUE;
    __bUSBSecPerPar = PARSER_SECTOR_SIZE / USB_DEFAULT_SECTOR_SIZE;

    FAT_InitVars();
}

// wyc2.05-909S, function to call USB_HCExit.
// ***********************************************************************
// Function : USBSRC_HCExit
// Description : Exit the USB mode and stop the USB HW mode.
// Arguments : None
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************
BYTE USBSRC_HCExit(void)
{
#if (USB_SYSTEM != USB_NONE)
    USB_HCExit();
#endif //
    return TRUE;
}

// wyc2.37-909s, function to force exiting H/W relative functions.
// ***********************************************************************
// Function : USBSRC_SRCExit
// Description : Exit the USB source mode and make it entering no media mode.
// Arguments : None
// Return : TRUE/FALSE
// Side Effect :
// ***********************************************************************
BYTE USBSRC_SRCExit(void)
{
#if (USB_SYSTEM == USB1_1_CT909R)
    USB_ClearPort();
#endif //
    return TRUE;
}

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
BYTE USBSRC_ExitMonitor(void)
{
    return TRUE;
}
#endif // SUPPORT_JPEG_AUDIO_PLAYING

// 1:card-reader, 0: USB
BYTE USBSRC_CheckConnect()
{
    // for 909P
#ifdef CTK_CARDREADER
    if (_bSource)
    {
        return CARD_CardStatus_Inserted();
    }
    else
#endif
    {
#if (USB_SYSTEM != USB_NONE)
        return USB_CheckConnect();
#else
        return FALSE;
#endif
    }
}

// wyc2.78-909P, function to trigger the switch host/device mode functions.
void USBSRC_SetOTGMode(BYTE bUSBOTGMode)
{
#ifdef SUPPORT_USB_HOST_DEVICE
#if (USB_SYSTEM == USB2_0_JUNGO)
    _bUSBOTGCmdMode = bUSBOTGMode;
#endif //
#endif //
}

BYTE USBSRC_GetOTGMode(void)
{
#ifdef SUPPORT_USB_HOST_DEVICE
#if (USB_SYSTEM == USB2_0_JUNGO)
    return __dwUSBOTGMode;
#endif //
#endif //
    return FALSE;
}

BYTE USBSRC_CTKCardReady(void)
{    
#ifdef SUPPORT_USB_HOST_DEVICE
    if ((__bUSBSRCCardReady == TRUE) && (__bUSBSRCCardType != CARD_CONTROLLER_NONE))
        return TRUE;
    else
        return FALSE;
#else
    return FALSE;
#endif //
}
#endif // SUPPORT_USB_SOURCE
