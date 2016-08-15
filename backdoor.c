
#include "winav.h"
#ifdef SUPPORT_STB
#include "dvbcustm.h"
#endif //SUPPORT_STB
#include "gdi.h"
#include "fontindex.h"
#include "fontindex.h"
#include "cc.h"
#include "hal.h"
#include "navigate.h"
#include "osd.h"
#include "osdnd.h"
#include "menu.h"
#include "mainmenu.h"
#include "dvdsetup_op.h"
#ifdef SUPPORT_STB
#include "dvb_nim.h"
#endif //SUPPORT_STB

#define BACK_DOOR_DECLARE
#include "storage.h"

#include "dvdsetup.h"
#ifdef SUPPORT_STB
#include "..\\..\\OSDString\\strextrn.h"
#else
#include "OSDString\\strextrn.h"
#endif //SUPPORT_STB

#ifdef IMAGE_FRAME_SETUP
#include "dmpcustm.h"
#endif //IMAGE_FRAME_SETUP

//Alan stb, add for backdoor
#define BACKDOOR_DISPLAY_X  500
#define BACKDOOR_DISPLAY_Y    5 

#define BACKDOOR_DISPLAY_VERSION_START_V	40 
#define BACKDOOR_DISPLAY_VERSION_START_H        30 
#define VERSION_OFFSET                          120

#define BACKDOOR_DISPLAY_VERSION_START_H1       (BACKDOOR_DISPLAY_VERSION_START_H + 320) 


#ifndef CT950_STYLE
#ifndef RELEASE_SETTING
BYTE _bBackDoor[] = {SETUP_BACK_DOOR_REGION, SETUP_BACK_DOOR_DEBUG_MODE}; 
#else
BYTE _bBackDoor[] = {SETUP_BACK_DOOR_DEBUG_MODE}; 
#endif
#else //CT950_STYLE
BYTE _bBackDoor[] = {SETUP_BACK_DOOR_DEBUG_MODE};
#endif

WORD aSTBFW[] = {3, CHAR_S, CHAR_T, CHAR_B};
WORD aSTBNIM[] = {3, CHAR_N, CHAR_I, CHAR_M};

DWORD _dwDebugMode;
DWORD _dwBackupDebugMode;
BYTE _bDebugIndex = 0;

#define DEBUG_MODE_0		0x00000000
#define DEBUG_MODE_1		0x00000011
#define DEBUG_MODE_2		0x00000022
#define DEBUG_MODE_3		0x00001100
#define DEBUG_MODE_4		0x00001111
#define DEBUG_MODE_5		0x00001122
#define DEBUG_MODE_6		0x00002200
#define DEBUG_MODE_7		0x00002211
#define DEBUG_MODE_8		0x00002222
#define DEBUG_MODE_9		0x00400222
#define DEBUG_MODE_10		0x00400211
#define DEBUG_MODE_11		0x00400200

BYTE _bCARDPath, _bNIMPath;

void _OutputAllSWVersion(void);
void _OutputNumber(WORD wX, WORD wY, WORD wNum);
BYTE _ConvertBackDoorRegionNum(BYTE bOriginalNum);
//BYTE _GetDebugIndex(void);
void _OutputVersion(DWORD dwVersion, BIT btFormat, PPARM_DRAW_STRING pVersion_StrAttr, PWORD pwVersionString);
void _TranslateString(PBYTE pbAsciiString, PWORD pwString);
void _OutputDebugInfo(void);
WORD _TranslateNum(BYTE bNum);
WORD* _TranslateDebugMode(BYTE bMode); 
BYTE _GetDebugIndex(DWORD dwMode);
void _SetDebugMode(BYTE bIndex);
void _GetUsefulDebugMode(BYTE bDIR);


#ifndef SUPPORT_ATAPI_SOURCE
extern BYTE SRVDBG_PCBNumber(void);
extern BYTE SRVDBG_MotorDriver(void);
extern BYTE SRVDBG_RFAmpChip(void);
extern BYTE SRVDBG_MotorPower(void);
#endif


extern BYTE _bOSDLang;

//  ***************************************************************************
//  Function    :   BACKDOOR_ExitBackDoorControl
//  Description :   Clean the flags and OSD
//  Arguments   :   none.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void BACKDOOR_ExitBackDoorControl(void)
{
    __bBackDoorSeqIndex = 0;
    _bDoBackDoor = FALSE;
    _bBackDoorIndex = 0;
}

void    _ExitBackDoor(void)
{
    //Draw the title
#ifdef SUPPORT_STB
#ifdef  CT909P_IC_SYSTEM  
    DVDSETUP_Recover();
#else                       		
    MAINMENU_DrawMainTitleBackground(__SetupInfo.bUIStyle - MENU_SYS_PREFERENCE_UI_STYLE_BEGIN);
    MAINMENU_DrawTitleString(aMENUMainMenu, NULL, NULL);
#endif    
#else //SUPPORT_STB
    DVDSETUP_Recover();
#endif //SUPPORT_STB

    //For back door
    BACKDOOR_ExitBackDoorControl();
}

void _OutputNumber(WORD wX, WORD wY, WORD wNum)
{
    PARM_DRAW_STRING TitleStringAttr;
    PARM_RECT TitleRect;
    WORD TempStr[GDI_STRING_LENGTH];
    BYTE bIndex;

#ifdef SUPPORT_STB
    // Clear the region
    MAINMENU_DrawMainTitleBackground(__SetupInfo.bUIStyle - MENU_SYS_PREFERENCE_UI_STYLE_BEGIN);
    MAINMENU_DrawTitleString(aMENUMainMenu, aMENUGame, NULL);
#endif //SUPPORT_STB
   
    TitleRect.rect.wLeft = wX;
    TitleRect.rect.wTop = 0;
    TitleRect.rect.wRight = TitleRect.rect.wLeft+48-1;
    TitleRect.rect.wBottom = TitleRect.rect.wTop+MAINMENU_TITLE_REGION_HEIGHT-1;

    MAINMENU_ClearMainTitleBackground(__SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN, &TitleRect);	

    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex(wNum);
    for(bIndex = 0; bIndex <= __wOSDUTLStringBuf[0]; bIndex++)
    {
        TempStr[bIndex] = __wOSDUTLStringBuf[bIndex];
    }

    MAINMENU_UseColorKey(&TitleStringAttr);
    TitleStringAttr.bTextColor = MAINMENU_PALETTE_ENTRY_TITLE_STRING_1ST;

    TitleStringAttr.wX = wX;
    TitleStringAttr.wY = wY; 

    GDI_DrawString_909(0, &TitleStringAttr, TempStr);
}

BYTE BackDoor_ProcessKey(BYTE bKey)
{
#ifndef SUPPORT_DMP
    BYTE bBackdoorValue;
#endif //SUPPORT_DMP

    BYTE bReturnKey = bKey;

    if (KEY_NO_KEY == bKey)
        return bReturnKey;

        if (_bDoBackDoor == FALSE)
        {
            if  (__bBackDoorSeqIndex == 0)
            {
                if (bKey == __bBackDoorSequence[__bBackDoorSeqIndex])
                {
                    __bBackDoorSeqIndex++;
                    bReturnKey = KEY_NO_KEY;
                }
            }
            else if (__bBackDoorSeqIndex < BACK_DOOR_SEQUENCE_NUM)
            {
                if (bKey == __bBackDoorSequence[__bBackDoorSeqIndex])
                {
                    __bBackDoorSeqIndex++;
                    bReturnKey = KEY_NO_KEY;
                }
                else
                {
                    __bBackDoorSeqIndex = 0;
                }

                if (__bBackDoorSeqIndex == BACK_DOOR_SEQUENCE_NUM)
                {
                    _bDoBackDoor = TRUE;   		                    
#ifdef  CT909P_IC_SYSTEM
                    _bDebugSetMode = FALSE;
                    _dwDebugMode = UTL_Get_DebugMode();	
                    _bDebugIndex = _GetDebugIndex(_dwDebugMode);
                    _dwBackupDebugMode = _dwDebugMode;                
                    MAINMENU_DrawMainContentBackground();
                    _OutputDebugInfo();                    
#endif			
                    // Set text color for back door case                          
                    _OutputNumber(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y, 6000 + _bDebugIndex);

                    bReturnKey = KEY_NO_KEY;
                }
            }
            return bReturnKey;
        }

    switch (bKey)
    {
    case KEY_MENU:
    case KEY_EXIT:
        if (TRUE == _bDoBackDoor)
        {
            _ExitBackDoor();
            return KEY_NO_KEY;
        }
        break;
    case KEY_SETUP:
            if (TRUE == _bDoBackDoor)
            {
                _ExitBackDoor();
                return KEY_NO_KEY;
            }
            else
            {
                return bKey;
            }
            break;
    case KEY_UP:
            _bBackDoorIndex--;
        if (_bBackDoorIndex == 0xff)
            _bBackDoorIndex = TOTAL_BACK_DOOR_NUM - 1;
#ifdef  CT909P_IC_SYSTEM
        if (SETUP_BACK_DOOR_DEBUG_MODE == _bBackDoor[_bBackDoorIndex]) 
        {
            _bDebugSetMode = FALSE;
            _dwDebugMode = UTL_Get_DebugMode();	
            _bDebugIndex = _GetDebugIndex(_dwDebugMode);
            _dwBackupDebugMode = _dwDebugMode;                
            MAINMENU_DrawMainContentBackground();
            _OutputDebugInfo();
        }        
        else
        {                
            _DVDSETUP_DrawAllUI(); 
            _OutputNumber(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y, 6000 + _bDebugIndex);
        }
#endif
        break;
    case KEY_DOWN:
        _bBackDoorIndex++;
        if (_bBackDoorIndex >= TOTAL_BACK_DOOR_NUM)
            _bBackDoorIndex = 0;
#ifdef  CT909P_IC_SYSTEM
        if (SETUP_BACK_DOOR_DEBUG_MODE == _bBackDoor[_bBackDoorIndex]) 
        {
            _bDebugSetMode = FALSE;
            _dwDebugMode = UTL_Get_DebugMode();	
            _bDebugIndex = _GetDebugIndex(_dwDebugMode);			
            _dwBackupDebugMode = _dwDebugMode;                
            MAINMENU_DrawMainContentBackground();
            _OutputDebugInfo();
        }        
        else
        {                
            _DVDSETUP_DrawAllUI();     
            _OutputNumber(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y, 6000 + _bDebugIndex);
        }
#endif
        break;
    case KEY_LEFT:
        if (SETUP_BACK_DOOR_REGION == _bBackDoor[_bBackDoorIndex])
        {
            _bBackDoorRegion--;
            if (_bBackDoorRegion == 0xFF)
                _bBackDoorRegion = SETUP_REGION_NUM - 1;
        } 
        else if (SETUP_BACK_DOOR_DEBUG_MODE == _bBackDoor[_bBackDoorIndex]) 
        {
/*        
            _bDebugIndex = _GetDebugIndex();
	    if (0 == _bDebugIndex)
            {
                _bDebugIndex = MAX_DBG_MODE - 1;
            } 
            else
            {
                _bDebugIndex--;
            }
            __bDebugMode = aDebugMode[_bDebugIndex];
            UTL_Config_DebugMode(UTL_DBG_SET);
*/
#ifdef  CT909P_IC_SYSTEM   
            _bDebugSetMode = TRUE;            
            _dwDebugMode = UTL_Get_DebugMode();	
            _bDebugIndex = _GetDebugIndex(_dwDebugMode);
            _GetUsefulDebugMode(FALSE);
            _SetDebugMode(_bDebugIndex);            	
            UTL_Config_DebugMode(UTL_DBG_SET,_dwDebugMode);   
            MAINMENU_DrawMainContentBackground();
            _OutputDebugInfo();               
#endif
        }
        DVDSETUP_SaveDataToSetupInfo(_bBackDoor[_bBackDoorIndex]);
        break;
    case KEY_RIGHT:
        if (SETUP_BACK_DOOR_REGION == _bBackDoor[_bBackDoorIndex])
        {
            _bBackDoorRegion++;
            if (_bBackDoorRegion == SETUP_REGION_NUM)
                _bBackDoorRegion = 0;
        } 
        else if (SETUP_BACK_DOOR_DEBUG_MODE == _bBackDoor[_bBackDoorIndex])
        {
/*
            _bDebugIndex = _GetDebugIndex();
            _bDebugIndex++;
	    if (MAX_DBG_MODE == _bDebugIndex)
            {
                _bDebugIndex = 0;
            } 
            __bDebugMode = aDebugMode[_bDebugIndex];
            UTL_Config_DebugMode(UTL_DBG_SET);
*/            
#ifdef  CT909P_IC_SYSTEM   
            _bDebugSetMode = TRUE;   
            _dwDebugMode = UTL_Get_DebugMode();	
            _bDebugIndex = _GetDebugIndex(_dwDebugMode);
            _GetUsefulDebugMode(TRUE);
            _SetDebugMode(_bDebugIndex);            
            UTL_Config_DebugMode(UTL_DBG_SET,_dwDebugMode);   
            MAINMENU_DrawMainContentBackground();
            _OutputDebugInfo();               
#endif			
        }
        DVDSETUP_SaveDataToSetupInfo(_bBackDoor[_bBackDoorIndex]);
        break;

        // LLY2.78b-3, merge all action to display desired version within backdoor
    // [1] KEY_MUTE: Customer S/W version (maybe different to our F/W version)
    // [2] KEY_PAUSE: ATAPI servo code version
    // [3] KEY_ZOOM: Real F/W version
    case KEY_MUTE:
    case KEY_ZOOM: // Brian2.79, use KEY_ZOOM instead //KEY_ZOOM
        if (_bDoBackDoor)
        {            
            MAINMENU_DrawMainContentBackground();
            _OutputAllSWVersion();
#ifdef SUPPORT_STB
            OS_DelayTime(COUNT_5_SEC); //CoCo1.20, reduce code size

            // Brian1.11b, move to here
            //CoCo1.11a, for backdoor
#ifdef  CT909P_IC_SYSTEM
            if (SETUP_BACK_DOOR_DEBUG_MODE == _bBackDoor[_bBackDoorIndex]) 
            {
                _bDebugSetMode = FALSE;
                _dwBackupDebugMode = _dwDebugMode;                
                MAINMENU_DrawMainContentBackground();
                _OutputDebugInfo();
            }        
            else
#endif  				
            {                
                _DVDSETUP_DrawAllUI();   
                _OutputNumber(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y, 6000 + _bDebugIndex);				
            }
          
#endif //SUPPORT_STB
        }
        break;
    default:
        break;
     }

    switch (_bBackDoor[_bBackDoorIndex])
    {
#ifndef CT950_STYLE    
    case SETUP_BACK_DOOR_REGION:
        __bInvalidRegion = TRUE;
        bBackdoorValue = _ConvertBackDoorRegionNum(__SetupInfo.bRegionControl);

        //Brian1.05, just call NV_Setting, it will set __wSPRMs[20] always
        NV_Setting(SET_REGION_CODE, 0);

        _OutputNumber(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y, bBackdoorValue);
        break;
#endif //#ifndef CT950_STYLE		
    case SETUP_BACK_DOOR_DEBUG_MODE:
        _OutputNumber(BACKDOOR_DISPLAY_X, BACKDOOR_DISPLAY_Y, 6000 + _bDebugIndex);
        break;
    default:
        break;
    }
    
    return KEY_NO_KEY;
}

BYTE    _ConvertBackDoorRegionNum(BYTE bOriginalNum)
{
#ifndef NO_SETUP
    switch (bOriginalNum)
    {
    case REGION_ALL:
        return 255;
    case REGION1:
        return 1;
    case REGION2:
        return 2;
    case REGION3:
        return 3;
    case REGION4:
        return 4;
    case REGION5:
        return 5;
    case REGION6:
        return 6;
    case REGION7:
        return 7;
    case REGION8:
        return 8;
    default:
        return 255;
    }
#else
    return 255;
#endif
}

/*
BYTE _GetDebugIndex(void)
{
    BYTE bIndex;
    for (bIndex = 0; bIndex < MAX_DBG_MODE; bIndex++)
    {
        if (aDebugMode[bIndex] == __bDebugMode)
            break;
    }
    return bIndex;
}
*/

void _OutputAllSWVersion(void)
{
    WORD VersionString[GDI_STRING_LENGTH+1];
    PARM_DRAW_STRING Version_StrAttr;
#ifdef SUPPORT_STB
    DWORD dwVersion; 
#endif //SUPPORT_STB

    //=================================================
    // Output version info in the 1st line
    //=================================================

    // output the description text

    Version_StrAttr.bBackground = MAINMENU_PALETTE_ENTRY_CONTENT_REGION;
//    Version_StrAttr.bTextColor = MAINMENU_PALETTE_ENTRY_BUTTON_STRING;
    Version_StrAttr.bTextColor = MAINMENU_PALETTE_ENTRY_BACKDOOR_STRING;
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    Version_StrAttr.wY = BACKDOOR_DISPLAY_VERSION_START_V;

    //Show Customer version.
    if (0 != __csVersionCustomer[0])
    {
        _TranslateString((BYTE*)__csVersionCustomer, VersionString); 
        GDI_DrawString_909(0, &Version_StrAttr, VersionString);
        Version_StrAttr.wY += GDI_GetStringHeight(VersionString);
    }


    //Show F/W version.
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPFW); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET;

#ifdef IMAGE_FRAME_SETUP
    __dwVersionRelease= (DMP_SW_VERSION<<16) | (DMP_SW_MINOR_VERSION<<8);   // F/W release version
#endif //IMAGE_FRAME_SETUP

    _OutputVersion(__dwVersionRelease, FORMAT_VERSION, &Version_StrAttr, VersionString);

    //Show FAE version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPFAE); //CoCo1.20, reduce code size.

    Version_StrAttr.wX += VERSION_OFFSET;

#ifdef IMAGE_FRAME_SETUP
    __dwVersionFAE= (DMP_SW_VERSION<<16) | (DMP_SW_MINOR_VERSION<<8) | FAE_DMP_SW_MINOR_VERSION;       // FAE version
#endif //IMAGE_FRAME_SETUP

    _OutputVersion(__dwVersionFAE, FORMAT_VERSION, &Version_StrAttr, VersionString);
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);
    
    //Show MPEG version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPMPEG); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionMPEG, FORMAT_DATE, &Version_StrAttr, VersionString);

    //Show JPEG version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPJPG); //--CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionJPEG, FORMAT_DATE, &Version_StrAttr, VersionString);
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);

    //Show Display version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPDisp); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionDisplay, FORMAT_DATE, &Version_StrAttr, VersionString);

    //Show DivX version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPDivX); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionDivX, FORMAT_DATE, &Version_StrAttr, VersionString);
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);

    //Show Navigate version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPNav); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionNavigate, FORMAT_DATE, &Version_StrAttr, VersionString);

    //Show USB version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPUSB);
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionUSB, FORMAT_DATE, &Version_StrAttr, VersionString);
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);

    //Show Parser version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPParser); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionParser, FORMAT_DATE, &Version_StrAttr, VersionString);

    //Show Card version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPCard);
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionCard, FORMAT_DATE, &Version_StrAttr, VersionString);
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);
  
    //Show Info version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPInfo);
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionINFO, FORMAT_DATE, &Version_StrAttr, VersionString);

    //Show TFT version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPTFT);
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwVersionTFT, FORMAT_VERSION, &Version_StrAttr, VersionString);
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);

    // Proc2 / Audio Version
    HAL_ReadAM(HAL_AM_PROC2_VERSION, &__dwTemp);
    __dwTemp = (((__dwTemp & 0x1E0) << 3) | (__dwTemp & 0x1F)) | 0x00050000;
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPAudio); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwTemp, FORMAT_DATE, &Version_StrAttr, VersionString);

    // Proc2 / Audio Support Feature 
    HAL_ReadAM(HAL_AM_PROC2_SUPPORT, &__dwTemp);
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPASupport); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(__dwTemp, FORMAT_NUMBER, &Version_StrAttr, VersionString);
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);

#ifdef SUPPORT_STB
    // Give one more line spaceing
    //Show NIM type.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSTBNIM); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET;
    GDI_DrawString_909(0, &Version_StrAttr, DVB_GetNimName()); 
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);

    //Show STB version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSTBFW); //CoCo1.20, reduce code size.

    dwVersion = (DVB_SW_VERSION << 16) | (DVB_SW_MINOR_VERSION << 8);
    Version_StrAttr.wX += VERSION_OFFSET;
    _OutputVersion(dwVersion, FORMAT_VERSION, &Version_StrAttr, VersionString);

#endif //#ifdef SUPPORT_STB

#ifndef CT950_STYLE
    //Show Servo module version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPServo); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET + 75;
    _OutputVersion(__dwVersionServo, FORMAT_VERSION, &Version_StrAttr, VersionString);
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);

    //Show BM version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPBM); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET + 75;
    _OutputVersion(__dwVersionServoBM, FORMAT_VERSION, &Version_StrAttr, VersionString);

    //Show Servo uCode version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPuCode); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET + 75;
    _OutputVersion(__dwVersionServoUCode, FORMAT_VERSION, &Version_StrAttr, VersionString);
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);

    //Show PCB Sytem version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPPCBSYSTEM); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET + 75;
#ifndef SUPPORT_ATAPI_SOURCE
    _OutputVersion(SRVDBG_PCBNumber(), FORMAT_NUMBER, &Version_StrAttr, VersionString);
#endif

    //Show RF Type version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPRFTYPE); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET + 75;
#ifndef SUPPORT_ATAPI_SOURCE
    _OutputVersion(SRVDBG_RFAmpChip(), FORMAT_NUMBER, &Version_StrAttr, VersionString);
#endif
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);

    //Show Motor Type version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPMOTORTYPE); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET + 75;
#ifndef SUPPORT_ATAPI_SOURCE
    _OutputVersion(SRVDBG_MotorDriver(), FORMAT_NUMBER, &Version_StrAttr, VersionString);
#endif

    //Show Motor Power version.
    Version_StrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H1;
    GDI_DrawString_909(0, &Version_StrAttr, aSETUPLOADER); //CoCo1.20, reduce code size.
    Version_StrAttr.wX += VERSION_OFFSET + 75;
#ifndef SUPPORT_ATAPI_SOURCE
    _OutputVersion(SRVDBG_MotorPower(), FORMAT_NUMBER, &Version_StrAttr, VersionString);
#endif
    Version_StrAttr.wY += GDI_GetStringHeight(VersionString);

#endif //#ifndef CT950_STYLE
}

//CoCo1.11a, for backdoor
void _OutputVersion(DWORD dwVersion, BIT btFormat, PPARM_DRAW_STRING pVersion_StrAttr, PWORD pwVersionString)
{
    BYTE bTemp = 1;
    WORD wTemp;

    pwVersionString[0] = 0;

    switch (btFormat)
    {
        case FORMAT_VERSION:
            wTemp = dwVersion >> 16; 

            if (0 != wTemp / 1000)
            {
                pwVersionString[bTemp++] = CHAR_0 + (wTemp / 1000);
            }
            wTemp = wTemp % 1000;

            pwVersionString[bTemp++] = CHAR_0 + (wTemp / 100);

            pwVersionString[bTemp++] = CHAR_FULL_STOP;

            wTemp = wTemp % 100;
            pwVersionString[bTemp++] = CHAR_0 + (wTemp / 10);

            wTemp = wTemp % 10;
            pwVersionString[bTemp++] = CHAR_0 + wTemp;

            wTemp = (dwVersion & 0x0000FF00) >> 8; 
            if (wTemp)
            {
                pwVersionString[bTemp++] = CHAR_a + (wTemp - 1);
            }
            else // Brian1.10,  for displaying correct FAE version
            {
                pwVersionString[bTemp++] = CHAR_LOW_LINE;
            }
            
            wTemp = dwVersion & 0x000000FF; 
            if (wTemp)
            {
                pwVersionString[bTemp++] = CHAR_a + (wTemp - 1);
            }
            else // Brian1.10,  for displaying correct FAE version
            if (pwVersionString[bTemp-1] == CHAR_LOW_LINE)
            {
                bTemp--;
            }
            
            pwVersionString[0] = bTemp - 1;
            break;
        case FORMAT_DATE:
            wTemp = dwVersion >> 16; 
            wTemp %= 100;
            pwVersionString[bTemp++] = CHAR_0 + (wTemp / 10);
            wTemp = wTemp % 10;
            pwVersionString[bTemp++] = CHAR_0 + (wTemp % 10);
            pwVersionString[bTemp++] = CHAR_HYPHEN_MINUS;   // Year

            wTemp = (dwVersion & 0x0000FF00) >> 8; 
            pwVersionString[bTemp++] = CHAR_0 + (wTemp / 10);
            wTemp = wTemp % 10;
            pwVersionString[bTemp++] = CHAR_0 + (wTemp % 10);
            pwVersionString[bTemp++] = CHAR_HYPHEN_MINUS;   // Month

            wTemp = dwVersion & 0x000000FF; 
            pwVersionString[bTemp++] = CHAR_0 + (wTemp / 10);
            wTemp = wTemp % 10;
            pwVersionString[bTemp++] = CHAR_0 + (wTemp % 10);
            pwVersionString[bTemp++] = CHAR_HYPHEN_MINUS;   // Day

            wTemp = dwVersion >> 16;
            wTemp /= 100;
            OSDUTL_PrepareOutputString();
            OSDUTL_OutputNumFromIndex(wTemp);
            memcpy(&(pwVersionString[bTemp]), &(__wOSDUTLStringBuf[1]),
                   __wOSDUTLStringBuf[0]*2);
            bTemp += __wOSDUTLStringBuf[0];    // Version

            pwVersionString[0] = bTemp - 1;
            break;
        case FORMAT_NUMBER:
            wTemp = dwVersion; 
            if (0 != wTemp / 1000)
            {
                pwVersionString[bTemp++] = CHAR_0 + (wTemp / 1000);
            }
            wTemp = wTemp % 1000;

            pwVersionString[bTemp++] = CHAR_0 + (wTemp / 100);

            wTemp = wTemp % 100;
            pwVersionString[bTemp++] = CHAR_0 + (wTemp / 10);

            wTemp = wTemp % 10;
            pwVersionString[bTemp++] = CHAR_0 + wTemp;

            pwVersionString[0] = bTemp - 1;
            break;
        default:
            break;
    }
    GDI_DrawString_909(0, pVersion_StrAttr, pwVersionString);
}

void _TranslateString(PBYTE pbAsciiString, PWORD pwString)
{
    BYTE bTemp = 0;

    while ('\0' != pbAsciiString[bTemp] && GDI_STRING_LENGTH > bTemp)
    {
        pwString[bTemp+1] = OSDUTL_TransformChar(pbAsciiString[bTemp]); //CoCo1.20, reduce code size.
        bTemp++;
    }
    pwString[0] = bTemp;
}

void _OutputDebugInfo(void)
{
    PARM_DRAW_STRING DebugStrAttr;  
    WORD wStrTmp[9];

    _bCARDPath = FALSE;
    _bNIMPath = FALSE;
	
    DebugStrAttr.bBackground = MAINMENU_PALETTE_ENTRY_CONTENT_REGION;
    DebugStrAttr.bTextColor = MAINMENU_PALETTE_ENTRY_BACKDOOR_STRING;
    DebugStrAttr.wX = BACKDOOR_DISPLAY_VERSION_START_H;
    DebugStrAttr.wY = BACKDOOR_DISPLAY_VERSION_START_V+15;
    // Show titile: Debug Mode Info
    GDI_DrawString_909(0, &DebugStrAttr, aSETUPDebugModeInfo);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPDebugModeInfo);
	
    // Show default debug mode value	    
    OSDUTL_PrepareOutputString();    
    OSDUTL_CopyStringFromIndex(aSETUPDefault);     
    wStrTmp[0] = 8;	
    wStrTmp[1] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF0000000) >> 28));
    wStrTmp[2] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF000000) >> 24));	
    wStrTmp[3] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF00000) >> 20));    
    wStrTmp[4] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF0000) >> 16));
    wStrTmp[5] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF000) >> 12));
    wStrTmp[6] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF00) >> 8));
    wStrTmp[7] = _TranslateNum((BYTE)((_dwBackupDebugMode & 0xF0) >> 4));
    wStrTmp[8] = _TranslateNum((BYTE)(_dwBackupDebugMode & 0xF));
    OSDUTL_CopyStringFromIndex(wStrTmp);      
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);   
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPDefault);

    // Show new debug mode value
    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aSETUPNew);    
    if (_bDebugSetMode)
    {
        wStrTmp[0] = 8;	
        wStrTmp[1] = _TranslateNum((BYTE)((_dwDebugMode & 0xF0000000) >> 28));
        wStrTmp[2] = _TranslateNum((BYTE)((_dwDebugMode & 0xF000000) >> 24));	
        wStrTmp[3] = _TranslateNum((BYTE)((_dwDebugMode & 0xF00000) >> 20));    
        wStrTmp[4] = _TranslateNum((BYTE)((_dwDebugMode & 0xF0000) >> 16));
        wStrTmp[5] = _TranslateNum((BYTE)((_dwDebugMode & 0xF000) >> 12));
        wStrTmp[6] = _TranslateNum((BYTE)((_dwDebugMode & 0xF00) >> 8));
        wStrTmp[7] = _TranslateNum((BYTE)((_dwDebugMode & 0xF0) >> 4));
        wStrTmp[8] = _TranslateNum((BYTE)(_dwDebugMode & 0xF));
        OSDUTL_CopyStringFromIndex(wStrTmp);      
    }
    else
    {
        OSDUTL_CopyStringFromIndex(aSETUPNotSet);
    }
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPNew);

    // Show DSU_TX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPDSU_TX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)(_dwDebugMode & 0xF)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPDSU_TX);

    // Show DSU_RX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPDSU_RX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)((_dwDebugMode & 0xF0) >> 4)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPDSU_RX);

    // Show UR1_TX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPUR1_TX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)((_dwDebugMode & 0xF00) >> 8)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPUR1_TX);

    // Show UR1_RX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPUR1_RX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)((_dwDebugMode & 0xF000) >> 12)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPUR1_RX);

    // Show UR2_TX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPUR2_TX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)((_dwDebugMode & 0xF0000) >> 16)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPUR2_TX);

     // Show UR2_RX mode.
    OSDUTL_PrepareOutputString();   
    OSDUTL_CopyStringFromIndex(aSETUPUR2_RX);     
    OSDUTL_CopyStringFromIndex(_TranslateDebugMode((BYTE)((_dwDebugMode & 0xF00000) >> 20)));	
    GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
    DebugStrAttr.wY += GDI_GetStringHeight(aSETUPUR2_RX) + 10;

    if (_bCARDPath)
    {
        OSDUTL_PrepareOutputString();   
        OSDUTL_CopyStringFromIndex(aSETUPDBGInfo1);        
        GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
        DebugStrAttr.wY += GDI_GetStringHeight(__pOSDUTLOutputString);
    }

    if (_bNIMPath)
    {
        OSDUTL_PrepareOutputString();   
        OSDUTL_CopyStringFromIndex(aSETUPDBGInfo2);        
        GDI_DrawString_909(0, &DebugStrAttr, __pOSDUTLOutputString);
        DebugStrAttr.wY += GDI_GetStringHeight(__pOSDUTLOutputString);
    }	
}

WORD _TranslateNum(BYTE bNum)
{
    if (bNum < 10)
    {
        return aNDNumbers[bNum];
    }
    else if (bNum <= 15)
    {
        return aNDUppercase[bNum-10];
    }

    return aNDNumbers[0]; // Non matched.

}

WORD* _TranslateDebugMode(BYTE bMode)
{
    WORD * pwDebugModStr;

    switch (bMode)
    {
    case 0:
        pwDebugModStr = aSETUPDisable;
        break;
    case 1:
        _bNIMPath = TRUE;
        pwDebugModStr = aSETUPEnable;
        break;
    case 2:
        _bCARDPath = TRUE;
        pwDebugModStr = aSETUPEnable;
        break;   
    default:
        pwDebugModStr = aSETUPEnable;
        break;
    }    

    return pwDebugModStr;
}

BYTE _GetDebugIndex(DWORD dwMode)
{
    switch (dwMode)
    {
    case DEBUG_MODE_1:        		
        return 1;
    case DEBUG_MODE_2:
        return 2;
    case DEBUG_MODE_3:
        return 3;
    case DEBUG_MODE_4:
        return 4;	
    case DEBUG_MODE_5:
        return 5;
    case DEBUG_MODE_6:		
        return 6;
    case DEBUG_MODE_7:		
        return 7;
    case DEBUG_MODE_8:		
        return 8;
    case DEBUG_MODE_9:		
        return 9;
    case DEBUG_MODE_10:		
        return 10;
    case DEBUG_MODE_11:		
        return 11;		
    case DEBUG_MODE_0:	
    default:
        return 0;
    }
}

void _SetDebugMode(BYTE bIndex)
{
    _dwDebugMode = 0;
	
    switch (bIndex)
    {                              
    case 1: 
        _dwDebugMode|=SET_DSU1_TX_VALUE(NIM_PATH);                 //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NIM_PATH);                  //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NOT_USED);                 //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);                 //UART1_RX_SOURCE << 12;                     
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                 //UART2_RX_SOURCE << 20;
        break;
    case 2: 
        _dwDebugMode|=SET_DSU1_TX_VALUE(CARD_READER_PATH);          //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(CARD_READER_PATH);          //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NOT_USED);                 //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);                  //UART1_RX_SOURCE << 12;                    
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                 //UART2_RX_SOURCE << 20;
        break;
    case 3:
        _dwDebugMode|=SET_DSU1_TX_VALUE(NOT_USED);                  //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NOT_USED);                  //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NIM_PATH);                  //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NIM_PATH);                  //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                 //UART2_RX_SOURCE << 20;
        break;
    case 4: 
        _dwDebugMode|=SET_DSU1_TX_VALUE(NIM_PATH);                   //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NIM_PATH);                    //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NIM_PATH);                   //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NIM_PATH);                  //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                 //UART2_RX_SOURCE << 20;
        break;
    case 5:
        _dwDebugMode|=SET_DSU1_TX_VALUE(CARD_READER_PATH);          //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(CARD_READER_PATH);          //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NIM_PATH);                  //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NIM_PATH);                  //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                 //UART2_RX_SOURCE << 20;
        break;
    case 6: 
        _dwDebugMode|=SET_DSU1_TX_VALUE(NOT_USED);                   //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NOT_USED);                   //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);          //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(CARD_READER_PATH);          //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                 //UART2_RX_SOURCE << 20;
        break;
    case 7:       
        _dwDebugMode|=SET_DSU1_TX_VALUE(NIM_PATH);                   //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NIM_PATH);                   //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);          //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(CARD_READER_PATH);          //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                 //UART2_RX_SOURCE << 20;
        break;    
    case 8:     
        _dwDebugMode|=SET_DSU1_TX_VALUE(CARD_READER_PATH);          //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(CARD_READER_PATH);          //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);         //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(CARD_READER_PATH);         //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);                 //UART2_RX_SOURCE << 20;
        break; 
    case 9:    
        _dwDebugMode|=SET_DSU1_TX_VALUE(CARD_READER_PATH);          //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(CARD_READER_PATH);          //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);         //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);                 //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                 //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(EXPAND_GPIO_PATH);         //UART2_RX_SOURCE << 20;
        break; 
    case 10:         
        _dwDebugMode|=SET_DSU1_TX_VALUE(NIM_PATH);                   //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NIM_PATH);                   //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);          //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);                  //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                  //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(EXPAND_GPIO_PATH);          //UART2_RX_SOURCE << 20;
        break;
    case 11:    
        _dwDebugMode|=SET_DSU1_TX_VALUE(NOT_USED);                   //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NOT_USED);                   //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(CARD_READER_PATH);          //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);                  //UART1_RX_SOURCE << 12;                
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);                  //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(EXPAND_GPIO_PATH);          //UART2_RX_SOURCE << 20;
        break;
    case 0: 
    default: 
        _dwDebugMode|=SET_DSU1_TX_VALUE(NOT_USED);       //DSU1_TX_SOURCE
        _dwDebugMode|=SET_DSU1_RX_VALUE(NOT_USED);  //DSU1_RX_SOURCE << 4;   
        _dwDebugMode|=SET_UART1_TX_VALUE(NOT_USED);  //UART1_TX_SOURCE << 8; 
        _dwDebugMode|=SET_UART1_RX_VALUE(NOT_USED);        //UART1_RX_SOURCE << 12;    
        _dwDebugMode|=SET_UART2_TX_VALUE(NOT_USED);        //UART2_TX_SOURCE << 16;
        _dwDebugMode|=SET_UART2_RX_VALUE(NOT_USED);        //UART2_RX_SOURCE << 20;        
        break;
    }
}

void _GetUsefulDebugMode(BYTE bDIR)
{
    DWORD dwDBGMode, dwCompare;
	
    dwDBGMode = UTL_Get_Path_DebugMode();
	
    if (bDIR) // Increase Index
    {
        do
        {
            _bDebugIndex++;
            if (_bDebugIndex >= MAX_DBG_MODE)
            {
                _bDebugIndex = 0;
            }   
            
            dwCompare = 1L << _bDebugIndex;			
        } while (!(dwCompare & dwDBGMode));            
    }
    else // Decrease Index
    {
        do
        {
            if (_bDebugIndex == 0)
            {
                _bDebugIndex = MAX_DBG_MODE - 1;
            }
            else
            {
                _bDebugIndex--;
            } 
            
            dwCompare = 1L << _bDebugIndex;			
        } while (!(dwCompare & dwDBGMode)); 
    }
}