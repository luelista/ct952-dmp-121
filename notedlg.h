#ifndef __NOTEDLG_H__
#define __NOTEDLG_H__

#include "dialog.h"

#ifdef SUPPORT_USB_HOST_DEVICE
//Define the type of USB notify dialog.
#define USBDEVICE_NOTIFY_DLG_TYPE_CONNECT			0
#define USBDEVICE_NOTIFY_DLG_TYPE_POWER_DOWN		1

//Notify user dialog box
#define USBDEVICE_DIALOG_WIDTH                      360
#define USBDEVICE_DIALOG_HEIGHT                     200
#define USBDEVICE_DIALOG_BUTTON_NUM                 0

#define USBDEVICE_DIALOG_CONTENT_DISTANCE_V         40

#ifdef NOTEDLG_MAIN_FILE
DIALOG _USBDEVICEDlg = {
    (MAINMENU_CONTENT_REGION_START_X+((MAINMENU_CONTENT_REGION_WIDTH-USBDEVICE_DIALOG_WIDTH)>>1)),
    (MAINMENU_CONTENT_REGION_START_Y+((MAINMENU_CONTENT_REGION_HEIGHT-USBDEVICE_DIALOG_HEIGHT)>>1)),
    USBDEVICE_DIALOG_WIDTH,
    USBDEVICE_DIALOG_HEIGHT,
    aUSBDEVICEDlgTitle,
    DIALOG_TITLE_STRING_ALIGNMENT_CENTER,
    USBDEVICE_DIALOG_BUTTON_NUM,
    0,
    0,
    0,
    NULL
};

#endif //NOTEDLG_MAIN_FILE

#endif //SUPPORT_USB_HOST_DEVICE


//Export APIs
void NOTEDLG_ShowUSBDevideNotifyDlg(BYTE bType);
void NOTEDLG_ExitUSBDevideNotifyDlg(void);

//Internal APIs
void _NOTEDLG_ConfigOSDRegion(void);


#endif //__NOTEDLG_H__