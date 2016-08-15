//  **************************************************************************
//      (C)Copyright Cheertek Inc. 2003-2005,
//          F200, all right reserved.
//
//      Product : CT909
//
//      Date    : 2005.04.11
//      Author  : Cheertek (D300 WYC)
//      Purpose : Provide the register definition of USB
//      Sources : 
//  **************************************************************************
#ifndef __USBSRC_H__
#define __USBSRC_H__

#ifdef __cplusplus
        extern  "C"     {
#endif
// wyc1.01-909, new module for USB source.
#ifdef SUPPORT_USB_SOURCE
// Definition
// wyc1.02-909, move define to here.
// NOTICE!! The state and status define value must the same.
// wyc1.20-909, add one state & status "no card"
#define USBSRC_STATE_NO_MEDIA           1
#define USBSRC_STATE_CONN_MEDIA         2
#define USBSRC_STATE_READY_MEDIA        3
#define USBSRC_STATE_WRONG_MEDIA        4
#define USBSRC_STATE_NO_CARD            5

// usbsrc command
#define USBSRC_CMD_CHECK_DEVICE         0x00000001
#define USBSRC_CMD_RECOGNIZE_MEDIA      0x00000002
#define USBSRC_CMD_READDATA             0x00000004
#define USBSRC_CMD_DUMPDATA             0x00000008
#define USBSRC_CMD_READDATA_1           0x00000010
// wyc2.78-909P, the new functions for usb host/device mode.
#define USBSRC_CMD_SWITCH_USB_STACK     0x00000020
#define USBSRC_CMD_DEVICE_MODE_CHK_CARD 0x00000040
#define USBSRC_CMD_EXIT_USB_STACK       0x00000080

#define USBSRC_CMD_DUMPDATA_COMPLETE    0x00010000
#define USBSRC_CMD_STOP                 0x80000000
#define USBSRC_CMD_ALL                  0xFFFFFFFF

// the USB S/W stack status reported.
#define USBSRC_STATUS_NO_MEDIA          1
#define USBSRC_STATUS_CONN_MEDIA        2
#define USBSRC_STATUS_READY_MEDIA       3
#define USBSRC_STATUS_WRONG_MEDIA       4
#define USBSRC_STATUS_NO_CARD           5
// wyc2.38-909s, define for card abnormal
#define USBSRC_STATUS_CARD_ABNORMAL     6

// wyc1.05-909, remove this define and use variable to replace it.

// extern functions
extern void USBSRC_Thread( DWORD index );
// wyc1.02-909, new function.
extern void USBSRC_ThreadDelete(void);
extern BYTE USBSRC_Stop(DWORD dwStreamID, BYTE bRealStop);
extern BYTE USBSRC_ReadSectors(DWORD dwStreamID, DWORD dwStartPos, DWORD dwEndPos, BYTE bImmeRead);
// wyc2.53-909P,
extern BYTE USBSRC_IssueReadCmd(DWORD dwStreamID);
extern BYTE USBSRC_GetCurrentPos(PDWORD pCurrentPos);
extern BYTE USBSRC_Monitor(void);
// wyc1.07-909, modify 3th parameter to DWORD* to support FAT32.
extern BYTE USBSRC_DumpData(DWORD dwSecNO, DWORD dwNum, DWORD* pFATBuff);
// wyc1.20-909,
extern BYTE USBSRC_DumpDatabySource(DWORD dwSecNO, DWORD dwNum, DWORD* pFATBuff);
extern BYTE USBSRC_CheckUSBStatus(BYTE bUSBSRCID);
// wyc1.05-909, rename function.
extern BYTE USBSRC_GetUSBSRCStatus(BYTE bSourceID);
extern void USBSRC_ClearUSBSRCState(void);
extern BOOL USBSRC_EnableRunning (DWORD dwStreamID, BYTE bRun);
extern BYTE USBSRC_Abort(void);
// wyc2.05-909S,
extern BYTE USBSRC_HCExit(void);
// wyc2.37-909s,
extern BYTE USBSRC_SRCExit(void);
// wyc2.78-909P,
void USBSRC_SetOTGMode(BYTE bUSBOTGMode);
BYTE USBSRC_GetOTGMode(void);

#ifdef SUPPORT_JPEG_AUDIO_PLAYING
BYTE USBSRC_ExitMonitor(void);
#endif //
// elmer2.38
#ifdef SUPPORT_POWERON_MENU
void USBSRC_ResumeThread(void);
#endif

BYTE USBSRC_CheckConnect(void);

BYTE USBSRC_TriggerCmd(FLAG_T*, DWORD);
void USBSRC_ClearCmd(FLAG_T*, DWORD);
void USBSRC_WaitCmd(FLAG_T*, DWORD);
BYTE USBSRC_PeekCmd(FLAG_T*, DWORD);
void USBSRC_ThreadSuspend(void);
void USBSRC_ThreadResume(void);
BYTE USBSRC_TriggerCmdAPI(BYTE, DWORD);
BYTE USBSRC_PeekCmdAPI(BYTE, DWORD);
void USBSRC_ClearCmdAPI(BYTE, DWORD);
void USBSRC_ConfigSource(BYTE bSRC);

#endif //

// extern variables.
#ifdef __cplusplus
        }
#endif
#endif  // __USBSRC_H__
