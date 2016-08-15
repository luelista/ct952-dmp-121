#define EDIT_MAIN_FILE

#include "winav.h"
#include "utl.h"
#include "gdi.h"
#include "osddsply.h"
#include "osdnd.h"
#include "input.h"
#include "cc.h"
#include "osd.h"
#include "disp.h"
#include "infofilter.h"
#include "mmanager.h"
#include "poweronmenu.h"
#include "mainmenu.h"
#include "notransl.h"
#include "fontindex.h"
#include "dialog.h"
#include "thumb.h"
#include "edit.h"
#include "setup.h"

#ifdef SUPPORT_ENCODE_JPG_PICTURE
extern BYTE __bMMJPGEncodeNum;
extern void MEDIA_USB(void);
#endif //SUPPORT_ENCODE_JPG_PICTURE

HDIALOG _hEDITFileDlg = DIALOG_HANDLE_INVALID;

void EDIT_Entry(void)
{
#ifdef SUPPORT_ENCODE_JPG_PICTURE
    //Clear Region?
    GDI_ClearRegion(0);

    if (__bMMJPGEncodeNum > 0)
    {
        _EDITDlg.bButtonNum = EDIT_DIALOG_COPY_DELETE_BUTTON_NUM;
        _EDITDlg.pButton = _EDITDlgCopyDeleteButton;
    }
    else
    {
        _EDITDlg.bButtonNum = EDIT_DIALOG_COPY_BUTTON_NUM;
        _EDITDlg.pButton = _EDITDlgCopyButton;
    }

	if (_hEDITFileDlg == DIALOG_HANDLE_INVALID)
	{
		_hEDITFileDlg = DIALOG_CreateDialog(&_EDITDlg, __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);
	}

	if (_hEDITFileDlg != DIALOG_HANDLE_INVALID)
	{
		DIALOG_InitializeGDI(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN);

        _EDIT_ConfigOSDRegion();

		DIALOG_ShowDialog(_hEDITFileDlg, PAL_ENTRY_COLOR_TRANSPARENT, TRUE);                    
		DIALOG_ShowMessage(_hEDITFileDlg, (EDIT_DIALOG_WIDTH-GDI_GetStringWidth(aEDITDialogContent))/2, EDIT_DIALOG_CONTENT_DISTANCE_V, aEDITDialogContent);
	}
#endif //SUPPORT_ENCODE_JPG_PICTURE
}

void EDIT_Recover(void)
{
#ifdef SUPPORT_ENCODE_JPG_PICTURE
    EDIT_Entry();
#endif //#ifdef SUPPORT_ENCODE_JPG_PICTURE
}

void EDIT_Exit(BYTE bClearBackground)
{
#ifdef SUPPORT_ENCODE_JPG_PICTURE
    _EDIT_DeleteDlg();

    if (bClearBackground)
    {
        GDI_ClearRegion(0);
    }
#endif //#ifdef SUPPORT_ENCODE_JPG_PICTURE
}

void EDIT_ProcessKey(void)
{
#ifdef SUPPORT_ENCODE_JPG_PICTURE

    WORD wReturn;

    wReturn = DIALOG_Processkey(_hEDITFileDlg, __bKey);        

    if (wReturn & DIALOG_ACTION_BUTTON_PRESSED)
    {
    	 #ifdef  SUPPORT_MENU_KEY_FUNCTION_EXIT	
   	 if(__bKey == KEY_FUNCTION)
    	 {
        	 _EDIT_DeleteDlg();
            	 POWERONMENU_BackToMainMenu();
		__bKey=KEY_NO_KEY;
     	 }
  	#endif
    
        switch (LOBYTE(wReturn))
        {
        case EDIT_DIALOG_BUTTON_ID_COPY:
            _EDIT_DeleteDlg();

            __bMMExtFilterMode = EXT_BIT_JPG;
            POWERONMENU_InternalExit();

            MEDIA_USB();

            //Enter MM UI
            MM_ConfigMMInitMenu(MM_INITIAL_MENU_COPY);
            break;
        case EDIT_DIALOG_BUTTON_ID_DELETE:
            _EDIT_DeleteDlg();

            __bMMExtFilterMode = EXT_BIT_JPG;
            POWERONMENU_InternalExit();
            __bTHUMBMainMode = THUMB_MAIN_MODE_EDIT;
            THUMB_Entry();
            break;
        case EDIT_DIALOG_BUTTON_ID_EXIT:
            _EDIT_DeleteDlg();
            POWERONMENU_BackToMainMenu();
            break;
        default:
            break;
        }
    }     
    
#endif //#ifdef SUPPORT_ENCODE_JPG_PICTURE
}

void _EDIT_DeleteDlg(void)
{
#ifdef SUPPORT_ENCODE_JPG_PICTURE

    if (_hEDITFileDlg != DIALOG_HANDLE_INVALID)
    {
        DIALOG_DeleteDialog(_hEDITFileDlg, TRUE);
        _hEDITFileDlg = DIALOG_HANDLE_INVALID;
    }

#endif //SUPPORT_ENCODE_JPG_PICTURE
}

//  ***************************************************************************
//  Function    :   _EDIT_ConfigOSDRegion
//  Description :   
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void _EDIT_ConfigOSDRegion(void)
{
#ifdef SUPPORT_ENCODE_JPG_PICTURE

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

#endif //SUPPORT_ENCODE_JPG_PICTURE
}
