#include "winav.h"
#include "comdef.h"
#include "osdnd.h"
#include "osd.h"
#include "utl.h"
#include "cc.h"
#include "input.h"
#include "linear.h"
#include "infofilter.h"
#include "mmanager.h"
#include "InfoFilter.h"
#include "gdi.h"
#include "fontindex.h" 
#include "hal.h"
#include "setup.h"
#include "notransl.h"
#include "disp.h"
#include "aploader.h"

// Bellowing codes are used for supportting the AP selection UI
#ifdef SUPPORT_PARSE_ALL_AP_FILES    
#define     OSDAP_TUNE_PAL_REGION_PIXELS 40
#define     OSDAP_MAXAPTODISPLAY 10
#define     PALETTE_COLOR_BLACK     ((0<<16) | (0<<8) | 0)
#define     PALETTE_COLOR_YELLOW    ((255<<16) | (255<<8) | 0)
#define     PALETTE_COLOR_WHITE     ((255<<16) | (255<<8) | 255)
#define     COLOR_BLACK             (252)
#define     COLOR_YELLOW            (253)
#define     COLOR_WHITE             (254)

PARM_DRAW_STRING _OSDAPStringAttr;
PARM_RECT _OSDAPRect;
BYTE _bChoiceAP;
WORD _wAPFiles;
extern void OSDMM_ConfigOSDRegion(void);

void OSDAP_TuneRegionForPALMode(void)
{
    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL)
    {
        GDI_MoveRegion(0, REGION_X_PAL, REGION_Y_PAL+OSDAP_TUNE_PAL_REGION_PIXELS);
    }
}

void OSDAP_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;

    RegionInfo.wWidth = 616; //GDI_REGION_WIDTH;
    RegionInfo.wHeight = 440;
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;
    RegionInfo.dwTAddr = DS_OSDFRAME_ST_MM;

    OSD_SetRegion(0, FALSE, &RegionInfo);

    //OSDAP_TuneRegionForPALMode();
}

VOID _OSDAP_APInit(VOID)
{
    BYTE bIndex;
    WORD wI;
    
    bIndex = 0;
    _bChoiceAP = 0;

    OSDAP_ConfigOSDRegion();
    OSD_RemoveAllUI();
    HAL_ClearScreen(HAL_COLOR_BLACK, HAL_CLEAR_SCREEN_MPEG, HAL_DISP_CURRENTFRAME);
    GDI_ChangePALEntry(COLOR_BLACK, PALETTE_COLOR_BLACK, FALSE);
    GDI_ChangePALEntry(COLOR_YELLOW, PALETTE_COLOR_YELLOW, FALSE);
    GDI_ChangePALEntry(COLOR_WHITE, PALETTE_COLOR_WHITE, FALSE);
    GDI_WaitPaletteComplete();        

    if(__wMMTotalFiles > OSDAP_MAXAPTODISPLAY)
    {
        _wAPFiles = OSDAP_MAXAPTODISPLAY;
    }
    else
    {
        _wAPFiles = __wMMTotalFiles;
    }
    
    for(wI = 0; wI < _wAPFiles; wI++)
    {
        bIndex = 0;
        __wOSDUTLStringBuf[1] = OSDUTL_TransformChar(('0'+wI));
        __wOSDUTLStringBuf[2] = OSDUTL_TransformChar('.');
       
        while (__pMMFileRecordStart[wI].FileName.cName[bIndex] != '\0')
        {
            __wOSDUTLStringBuf[bIndex+3] = OSDUTL_TransformChar(__pMMFileRecordStart[wI].FileName.cName[bIndex]);

            if (bIndex > 30)
            {
                bIndex = 30;
                break;
            }
            bIndex++;
        }                
        
        __wOSDUTLStringBuf[0] = bIndex+2;

        _OSDAPStringAttr.wX = 100;
        _OSDAPStringAttr.wY = 40 + wI*35;
        _OSDAPStringAttr.bTextColor = COLOR_WHITE;
        GDI_DrawString_909(0, &_OSDAPStringAttr, __wOSDUTLStringBuf);
        if(!wI)
        {
            _OSDAPRect.rect.wLeft = 100;
            _OSDAPRect.rect.wTop = 65;            
            _OSDAPRect.rect.wRight = 250;
            _OSDAPRect.rect.wBottom = 70;
            _OSDAPRect.bColor = COLOR_YELLOW;
            GDI_FillRect_909(0, &_OSDAPRect);
        }
    }
}

VOID _OSDAP_ReDrawAP(BYTE bIndex)
{
    BYTE bI;
    for(bI=0; bI<_wAPFiles; bI++)
    {
        if((bI+1) == bIndex)
        {
            _OSDAPRect.bColor = COLOR_YELLOW;
        }
        else
        {
            _OSDAPRect.bColor = COLOR_BLACK;
        }
        _OSDAPRect.rect.wLeft = 100;
        _OSDAPRect.rect.wTop = 65+35*bI;            
        _OSDAPRect.rect.wRight = 250;
        _OSDAPRect.rect.wBottom = 70+35*bI;
        GDI_FillRect_909(0, &_OSDAPRect);                
    }
}

VOID _OSDAP_APProcKey(VOID)
{
    extern void REBOOT(void);
    BYTE bIndex, bGoing;
    bIndex = 1;
    bGoing = TRUE;
    while(bGoing)
    {
        switch(__bISRKey)
        {
            case    KEY_DOWN:
                        bIndex++;
                        if(bIndex > _wAPFiles)
                        {
                            bIndex = 1;
                        }
                        _OSDAP_ReDrawAP(bIndex);
                        __bISRKey = KEY_NO_KEY;
                        break;
            case    KEY_UP:
                        bIndex--;
                        if(bIndex<1)
                        {
                            bIndex = _wAPFiles;
                        }
                        _OSDAP_ReDrawAP(bIndex);
                        __bISRKey = KEY_NO_KEY;
                        break;
            case    KEY_PLAY:
            case    KEY_ENTER:
                        _bChoiceAP = (bIndex-1);
                        bGoing = FALSE;
                        __bISRKey = KEY_NO_KEY;
                        break;
            case    KEY_EXIT:
                        REBOOT();
                        break;
            default:
                        break;
        }
   
    }
}
#endif //#ifdef SUPPORT_PARSE_ALL_AP_FILES    


VOID OSDAP_ProcAP(BYTE bType, DWORD *pdwFileLoc, DWORD *dwpFileLength)
{
#ifdef SUPPORT_PARSE_ALL_AP_FILES    
    if( __wMMTotalFiles > 0 )
    {
        _OSDAP_APInit();
        _OSDAP_APProcKey();
        printf("_bChoiceAP = 0x%x", _bChoiceAP);
        *pdwFileLoc = 0;
        *pdwFileLoc |= __pMMFileRecordStart[_bChoiceAP].FileRange.cS[0];
        *pdwFileLoc <<=24;
        *pdwFileLoc |= ((WORD)__pMMFileRecordStart[_bChoiceAP].FileRange.cS[1])<<16;
        *pdwFileLoc |= ((WORD)__pMMFileRecordStart[_bChoiceAP].FileRange.cS[2])<<8;
        *pdwFileLoc |= __pMMFileRecordStart[_bChoiceAP].FileRange.cS[3];
        if(bType)
        *dwpFileLength = (__pMMFileRecordStart[_bChoiceAP].FileRange.dwLen);
        else
        *dwpFileLength = (__pMMFileRecordStart[_bChoiceAP].FileRange.dwLen/PARSER_SECTOR_SIZE)+2;
    }    
    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL)
    {
        GDI_MoveRegion(0, REGION_X_PAL, REGION_Y_PAL);
    }

    //Tell OSD that Media Manager exits.
    OSD_ChangeUI(OSD_UI_MEDIA_MANAGER, OSD_EXIT_UI);
#endif    
}


//  *****************************************************************************************************
//  Function    :   OSDAP_ProcessKey
//  Description :   Process the input key within AP mode
//  Argument    :   None
//  Return      :   KEY_NO_KEY, the key has processed done
//                  KEY_INVALID, don't support this key
//                  KEY_TODO, the key has been proceed first, and need CC_DoKeyAction() to continue do
//                  KEY_BYPASS, the key bypass to other flow process
//  Side Effect :
//  *****************************************************************************************************
BYTE OSDAP_ProcessKey(void)
{
    switch(__bKey)
    {
    case    KEY_AP:
        AP_SwitchMode(AP_GPS);
        return (KEY_NO_KEY);      
        
    default:
        return (KEY_BYPASS);
    }
}


