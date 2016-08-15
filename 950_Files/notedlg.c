#define NOTEDLG_MAIN_FILE

#include "winav.h"
#include "ctkav.h"
#include "utl.h"
#include "cc.h"
#include "gdi.h"
#include "setup.h"
#include "osd.h"
#include "mainmenu.h"
#include "osdss.h"
#include "poweronmenu.h"
#include "disp.h"
#include "notedlg.h"
#include "notransl.h"

#ifdef SUPPORT_USB_HOST_DEVICE

HDIALOG _hNOTEDLGDlg = DIALOG_HANDLE_INVALID;

void NOTEDLG_ShowUSBDevideNotifyDlg(BYTE bType)
{
    //Config the OSD to 8 bit mode.
    _NOTEDLG_ConfigOSDRegion();

    if (_hNOTEDLGDlg == DIALOG_HANDLE_INVALID)
    {
        _hNOTEDLGDlg = DIALOG_CreateDialog(&_USBDEVICEDlg, __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
    }
	else
	{
		NOTEDLG_ExitUSBDevideNotifyDlg();
	}

    if (_hNOTEDLGDlg != DIALOG_HANDLE_INVALID)
    {
        DIALOG_InitializeGDI(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);

        DIALOG_ShowDialog(_hNOTEDLGDlg, PAL_ENTRY_COLOR_TRANSPARENT, FALSE);                    

		if (bType == USBDEVICE_NOTIFY_DLG_TYPE_CONNECT)
		{
			DIALOG_ShowMessage(_hNOTEDLGDlg, (USBDEVICE_DIALOG_WIDTH-GDI_GetStringWidth(aUSBDEVICEDlgNotifyContent))/2, USBDEVICE_DIALOG_CONTENT_DISTANCE_V, aUSBDEVICEDlgNotifyContent);
		}
		else if (bType == USBDEVICE_NOTIFY_DLG_TYPE_POWER_DOWN)
		{
			DIALOG_ShowMessage(_hNOTEDLGDlg, (USBDEVICE_DIALOG_WIDTH-GDI_GetStringWidth(aUSBDEVICEDlgPowerDownContent))/2, USBDEVICE_DIALOG_CONTENT_DISTANCE_V, aUSBDEVICEDlgPowerDownContent);
		}
    }
}

void NOTEDLG_ExitUSBDevideNotifyDlg(void)
{
	if (_hNOTEDLGDlg != DIALOG_HANDLE_INVALID)
	{
		DIALOG_DeleteDialog(_hNOTEDLGDlg, TRUE);
		_hNOTEDLGDlg = DIALOG_HANDLE_INVALID;
	}
}

#endif //SUPPORT_USB_HOST_DEVICE

//  ***************************************************************************
//  Function    :   _NOTEDLG_ConfigOSDRegion
//  Description :   
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void _NOTEDLG_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;
#ifdef  BORROW_FRAME_BUFFER
    DWORD dwBufferAddr;
#endif //

    GDI_ClearRegion(0);

#ifdef  BORROW_FRAME_BUFFER
   
    if ((POWERPNMENU_WIDTH == __RegionList[0].wWidth) && (POWERPNMENU_HEIGHT <= __RegionList[0].wHeight) &&
        (GDI_OSD_8B_MODE == __RegionList[0].bColorMode)) 
    {
        return;
    }
    else if (__RegionList[0].bColorMode != GDI_OSD_8B_MODE)   // Thumbnail mode & preview mode
    {
        // Use Frame buffer 1 or 2
        dwBufferAddr = DISP_QueryAvailableBuffer();
        if (0 == dwBufferAddr)
        {
            RegionInfo.dwTAddr = DS_OSDFRAME_ST;
        }
        else
        {
            RegionInfo.dwTAddr = dwBufferAddr;
        }
    } 
    else
    {
        // Use OSD buffer and Frame buffer 0
        RegionInfo.dwTAddr = DS_OSDFRAME_ST;   // Use OSD buffer and Frame buffer 0 
    }

#else
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
#endif // #ifdef  BORROW_FRAME_BUFFER

    RegionInfo.wWidth = POWERPNMENU_WIDTH;
    RegionInfo.wHeight = POWERPNMENU_HEIGHT;
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;

#ifdef  BORROW_FRAME_BUFFER
    OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    OSD_SetRegion(0, FALSE, &RegionInfo);
#endif
}