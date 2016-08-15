/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved.

        Product : WinAV Firmware

        Date    : 2005.04.12.
        Author  : Cheertek (D300 BrianCheng)
        Purpose : USB Host Control Driver Module
                  The moudle provides low level HC driver functions
        Sources : usb.h/ usb.c

------------------------------------------------------------
        Update

        Date    : 
        Name    : Cheertek (D300 Briancheng)
        Description     : ???

                        Item                                    Source
        ----------------------------------------        -------------------
        . 
****************************************************************************/

#ifndef __USB_H__
#define __USB_H__




//__dwUSBStatus value
#define USB_STATUS_UNKNOWN              0
#define USB_STATUS_NO_DEVICE            1
#define USB_STATUS_NOT_SUPPORTED_DEVICE     2
#define USB_STATUS_MASS_STORAGE_DEVICE  3
#define USB_STATUS_NEW_DEVICE_ATTACH  4
#define USB_STATUS_NO_DISC  5
// wyc2.38-909s, report the card status
#define USB_STATUS_CARD_ABNORMAL    6


// USB_CheckCard value
// wyc2.38-909s, report the card status
#define USB_CHECK_NO_CARD           0
#define USB_CHECK_HAS_CARD          1
#define USB_CHECK_CARD_ABNORMAL     2
#define USB_CHECK_CARD_NOT_READY    3
// wyc2.51-909P,
#define USB_CHECK_TIME_OUT          4


void        USB_HCInit(BYTE bPort,  BYTE bLUN, DWORD dwUSBBufferAddr);
void        USB_HCExit(void);

BYTE        USB_ReadData(DWORD dwStartSector, DWORD dwNumOfSector, DWORD dwBufferAddr);

DWORD   USB_CheckStatus(void);
BYTE USB_CheckConnect(void);
void   USB_ClearStatus(void);
BYTE USB_CheckCard(BYTE bLUN);
// wyc2.71-909P
void USB_ReportPortLunStatus(DWORD* dwPort1Status, DWORD* dwPort2Status);
DWORD   USB_CheckAllCard(void);
 void USB_ClearPort(void);


BYTE USBD_SBC_Read10(BYTE bLUN, DWORD dwStartSector, DWORD dwNumOfSector, DWORD dwBufferAddr);
BYTE USB_CheckCardStatus(BYTE bLUN);
// wyc2.38-909s, function to report lun number
BYTE USB_ReportCurLUN(void);

extern DWORD   __dwUSBStatus;
                

#endif // __USB_H__




