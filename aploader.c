#include "winav.h"
#include "ctkav.h"
#include "infofilter.h"
#include "srcfilter.h"
#include "aploader.h"
#include "hal.h"
#include "disp.h"
#include "utl.h"
#include "input.h"
#include "cc.h"
#include "setup.h"
#include "media.h"
#include "initial.h"
#include "usbwrap.h"
//////////////////////////////////////////////////////////////////////
//  Big modification Area
//  LLY2.10, Remove CT909A related code.

/////////////////////////////////////////////////////////////////////
// Global Variable Area - Global use
/////////////////////////////////////////////////////////////////////
AP_INFO     __AP_Info;

VOID    _AP_Prepare_AdditionalInfo(DWORD dwAPSource);
VOID    _AP_ExternalStart(DWORD dwInfoAddr, DWORD dwCodeAddr, DWORD dwLen);
BYTE    _AP_CheckSum(void);

// Check IC Version
// peteryu275, add for IC check version, 2007/3/22 11:31AM
BYTE _AP_IC_Version_Check(DWORD apVersion);
extern PARM_DRAW_STRING __OSDNDUpMsgAttr;
// peteryu275, add for IC check version, 2007/3/22 11:31AM end 

// LLY2.00, add function to re-booting while AP switch procedure fail.
void _AP_REBOOT(void)
{
    DWORD dwSaveInt;
    
    OS_DISABLE_INTERRUPTS(dwSaveInt);
    REG_PLAT_WATCHDOG = 0;
    REG_PLAT_SYSTEM_CONFIGURATION1 |= (1L<<28);
    while(1);
}

//***************************************************************************
//  Function    :   AP_Loader
//  Abstract    :
//  Arguments   :   dwStartPos : Indicates the start sector
//              :   dwEndPos : Indicates the end sector
//              :   dwType : Indicates the source from CD/ USB/ DRAM/ Flash
//  Return      :   None
//  Side Effect :
//  Notes       :
//***************************************************************************
DWORD   AP_Loader(DWORD dwType, DWORD dwStartPos, DWORD dwSize)
{
    GDI_REGION_INFO RegionInfo;
#ifndef NO_DISC_MODE
    DWORD   dwTime;
#endif  // #ifndef NO_DISC_MODE

    // Chuan1.08, Turn on PROM
#ifdef  SUPPORT_PROM_DISABLE
    DWORD dwSaveInt;
    OS_DISABLE_INTERRUPTS( dwSaveInt );
    REG_PLAT_CLK_GENERATOR_CONTROL &= ~(PLAT_MCLK_PROM_DISABLE);
    OS_RESTORE_INTERRUPTS( dwSaveInt );
    //UTL_DelayTime(COUNT_50_MSEC, FALSE); //CoCo1.20, reduce code size
    OS_DelayTime(COUNT_50_MSEC); //CoCo1.20, reduce code size
#endif  // #ifdef  SUPPORT_PROM_DISABLE

    // STEP1: Identify the AP to check validity
    // If validity, __AP_Info will be assign the right value
    if(AP_Identify(dwType, dwStartPos)==NULL)
    {
        // Chuan2.77, for AP on DRAM checksum error display, 2007/4/11 06:23PM    
        if (__AP_Info.dwAP_Type ==  AP_AUTO_UPGRADE)
        {
            DWORD dwSaveInt;
              
            OS_DelayTime(COUNT_1_SEC);
            
            HAL_WatchDog_Enable(FALSE);
            OS_DISABLE_INTERRUPTS(dwSaveInt);
            while(1);
        }
        else
        {
            return FALSE;
        }
    }

    // Disable Information for DBG
    DBG_Disable();
    // Can't call GDI Function @ Non CHEERDVD Thread because CHEERDVD Thread has been suspended!!. It may be cause deadlock.
    // Therefore, release Mutex of GDI Function
    OS_TryLockMutex( &__mutexGDIIdle );
    OS_UnlockMutex(&__mutexGDIIdle);

    // STEP2: Turn off MainVideo/ PIP/ SP1/ SP2.
#ifdef SUPPORT_TFT_DIGITAL_MODE
    HAL_AdjustTFT_DigitalMode(FALSE);
#endif
    
    DISP_DisplayCtrl(DISP_MAINVIDEO, FALSE);
    DISP_DisplayCtrl(DISP_PIP, FALSE);
    DISP_DisplayCtrl(DISP_SP1, FALSE);
    DISP_DisplayCtrl(DISP_SP2, FALSE);
    DISP_DisplayCtrl(DISP_OSD, FALSE);
    DISP_DelayVSYNC(1);    

    // LLY2.20, rename it as UTL_Config_BufferMode()
    UTL_Config_BufferMode(DUMP_BUFF_AP_MODE);
    // Chuan2.38a, Can't call GDI Function @ Non CHEERDVD Thread. It may be cause deadlock.
    GDI_DeactivateRegion();
    RegionInfo.wWidth = 616;
    RegionInfo.wHeight = 78;
    RegionInfo.bColorMode = GDI_OSD_4B_MODE;
    RegionInfo.dwTAddr = DS_OSDFRAME_ST_AP;
    GDI_ConfigRegionInfo(0, &RegionInfo);
    GDI_InitialRegion(0);
    GDI_ClearRegion(0);
    GDI_ActivateRegion(0);

    DISP_DelayVSYNC(1);


    // STEP3: Load AP to DRAM
    // Chuan2.77, add for unprotection with loverlap DS_OSD_FONTTABLE_ST,2007/4/11 04:29PM
    MACRO_MCU_WRITE_PROTECTION_ENABLE(MCU_WRITE_PROTECTION_DISABLE_ALL);
    
    switch(dwType)
    {
        case SRC_FROM_USB:
        case SRC_FROM_SERVO:
            INFOFILTER_ResetDumpSectors();
            INFOFILTER_DumpSectors(dwStartPos, dwSize, (BYTE *)DS_AP_CODE_AREA, __AP_Info.dwAP_Size, 0);
            HAL_PowerControl(HAL_POWER_USB, HAL_POWER_SAVE);
            USB_HCExit();

#ifndef NO_DISC_MODE
            // Chuan1.20, Open tray after AP_Loader(). (For Auto-Upgrade)
            if ((__AP_Info.dwAP_Type ==  AP_AUTO_UPGRADE) && (dwType == SRC_FROM_SERVO))
            {
                SrcFilter_OpenTray();
                dwTime = OS_GetSysTimer() ;
                while (OS_GetSysTimer() - dwTime < COUNT_10_SEC)
                {
                    if (SrcFilter_CheckTrayStatus () == SRC_TRAY_OPENED)
                        break;
                }
            }
            
            if (dwType == SRC_FROM_SERVO)
            {
                // Chuan1.21, Force Servo enter power down mode, else it can't
                // stop for COBY platform (Can't be eject
                SrcFilter_PowerMode(SERVO_POWER_STOP);
            }

            // LLY2.37, using procedure to check servo real stop
            // Otherwiss, sometime servo pick-up maybe still running while upgrade from USB mode
            // Notice: Servo power down command has been issued while switching to USB mode
            // **** Will be removed while SrcFilter_GetStatus() w/ desired source index is ok
            //OS_DelayTime(COUNT_100_MSEC);	// Chuan2.18, wait 100 msec for Servo power down
            dwTime = OS_GetSysTimer() ;
            while(OS_GetSysTimer() - dwTime < COUNT_10_SEC)
            {
                if(SrcFilter_GetStatus(SOURCE_DISC) != SRC_SPINDLE_STATUS_SPINNING)
                {
                    break;
                }
                else
                {
                    OS_YieldThread();
                }
            }
#endif  // #ifndef NO_DISC_MODE
            break;

        case SRC_FROM_FLASH:
        case SRC_FROM_DRAM:
            if (dwStartPos != DS_AP_CODE_AREA)
            {
                HAL_CopyDRAM(dwStartPos, DS_AP_CODE_AREA, dwSize);
            }
            break;
    }

    // STEP4: Do checksum & Prepare AP Additional Information
    if (__AP_Info.dwExternalFlag == TRUE)
    {
        WORD  wCheckSum;

        // Do Checksum ..
        wCheckSum = HAL_CheckSum((DS_AP_CODE_AREA+sizeof(AP_INFO)), (DS_AP_CODE_AREA+__AP_Info.dwAP_Size));

        if ((WORD)__AP_Info.dwCheckSum != wCheckSum)
        {
            // DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "CheckSum Error: %x ->%x", (WORD)__AP_Info.dwCheckSum, wCheckSum);
            // Chuan2.77, for AP on DRAM checksum error display, 2007/4/11 06:23PM
            MainBGColor.Color.bY = 0x4C;
            MainBGColor.Color.bU = 0x54;
            MainBGColor.Color.bV = 0xFF;
            DISP_BGColorSet(MainBGColor, DISP_MAIN_BG);
            OS_DelayTime(COUNT_3_SEC);
            if (__AP_Info.dwAP_Type ==  AP_AUTO_UPGRADE)
            {
                DWORD dwSaveInt;
                
                HAL_WatchDog_Enable(FALSE);
                OS_DISABLE_INTERRUPTS(dwSaveInt);
                while(1);
            }
            else
            {
                _AP_REBOOT();
            }
            //peteryu dmp117dvb, for AP on DRAM checksum error display, 2007/4/11 06:23PM end
            
            return FALSE;
        }

        // STEP4: Prepare Additional Information to prepare area.
        // LLY1.50, need pass AP source while prepare AP_INFO_ADDITIONAL structure
        _AP_Prepare_AdditionalInfo(dwType);
    }

    // STEP6: Jump to 0x40001000 or prepare area to run the AP.
    ROMLD_BOOT_LoadSectionAndRun((PSECTION_ENTRY)AP_TABLE_ADDRESS, __AP_Info.dwAP_UNZIP_BUF, __AP_Info.dwAP_SP);

    return TRUE;
}

//  *********************************************************************
//  Function    :   APCode_Version_Check
//  Description :   Check the version of AP
//  Arguments   :   apVersion, version of AP
//  Return      :   TRUE, Version is ok
//                  FALSE, Version is wrong
//  *********************************************************************
//  peteryu275, add for IC check version, 2007/3/22 11:31AM
BYTE    _AP_IC_Version_Check(DWORD dwAPVersion)
{
    BYTE bAPCheckStatus = FALSE;
    
    if (dwAPVersion == IC_VERSION_RESERVED || dwAPVersion == IC_VERSION_ID)
    {
        bAPCheckStatus = TRUE;
    }

    return bAPCheckStatus;
}   // BYTE _AP_IC_Version_Check(DWORD dwAPVersion)

// LLY1.50 create, base on _AP_Identify() to build a new API
// Then, it can get AP_INFO structure, and return the AP ID
//  *******************************************************************************
//  Function    :   AP_Identify
//  Description :   Parsing AP_INFO structure, and return the structure pointer
//  Argument    :   dwSource, specify the AP source, ex. USB/ Servo/ DRAM/ Flash
//                  dwStartPos, specify the AP start address
//  Return      :   AP_INFO structure pointer if allow to switch it, or
//                  NULL if dis-allow to switch it.
//  *******************************************************************************
//#define DEBUG_AP_INFO
PAP_INFO   AP_Identify(BYTE bSource, DWORD dwStartPos)
{
    DWORD   dwAPCodeArea=0;

    // Step 1: Dump AP_INFO structure content
    switch(bSource)
    {
    case    SRC_FROM_USB:
    case    SRC_FROM_SERVO:
        INFOFILTER_ResetDumpSectors();
        INFOFILTER_DumpSectors(dwStartPos, 1, (BYTE *)&__AP_Info, sizeof(AP_INFO), 0);
        break;

    case    SRC_FROM_DRAM:
    case    SRC_FROM_FLASH:
        // Get AP_INFO structure content first
        HAL_CopyDRAM(dwStartPos, (DWORD)&__AP_Info, sizeof(AP_INFO));
        break;
    }

#ifdef  DEBUG_AP_INFO
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AP_INFO structure information\n");
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Identify [1]: %lx, Identify [2]: %lx, AP_Type: %lx, External Flag: %lx",
        __AP_Info.dwIdentify[0], __AP_Info.dwIdentify[1], __AP_Info.dwAP_Type, __AP_Info.dwExternalFlag);
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Version AP: %lx, Chip Version: %lx",
        __AP_Info.dwVersionAP, __AP_Info.dwChipVersion);
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AP Description: %lx, %lx, %lx, %lx\n",
        __AP_Info.dwDescription[0], __AP_Info.dwDescription[1],
        __AP_Info.dwDescription[2], __AP_Info.dwDescription[3]);
#endif  // #ifdef DEBUG_AP_INFO

    // STEP 2: Check support chip verion first -- LLY2.00
    // if(__AP_Info.dwChipVersion != 1)
    // peteryu275, modified for IC check version, 2007/3/22 11:31AM
    if (_AP_IC_Version_Check(__AP_Info.dwChipVersion) == FALSE)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Not desired chip version auto-upgrade code: %lx,now: %lx\n", __AP_Info.dwChipVersion, IC_VERSION_ID);
        
        __OSDNDUpMsgAttr.wX -= GDI_GetStringWidth_909(aAP_IC_VERSION_ERROR);
        GDI_DrawString_909(0, &__OSDNDUpMsgAttr, aAP_IC_VERSION_ERROR);
        __OSDNDUpMsgAttr.wX += GDI_GetStringWidth_909(aAP_IC_VERSION_ERROR);
        OS_DelayTime(COUNT_3_SEC);
        
        return (NULL);
    }

    // STEP 3: Check the identify string "CT909-AP"
    if ((CT909AP_IDENTIFY1 != __AP_Info.dwIdentify[0]) ||
        (CT909AP_IDENTIFY2 != __AP_Info.dwIdentify[1]))
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: ID is %lx, %lx\n", __AP_Info.dwIdentify[0], __AP_Info.dwIdentify[1]);
        return (NULL);
    }

    // STEP 4: Check the space is enough or not.
    // Calculate the AP code area base on DRAM type
    // ie. from AP_CODE_AREA ~ (DRAM end address - 4)
    // So, size = DRAM end address - AP_CODE_AREA
    dwAPCodeArea = HAL_GetDRAMSize();
    if(dwAPCodeArea == HAL_DRAM_UNKNOWN)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Unknown DRAM type to calculate AP code area !\n");
        return (NULL);
    }
    else
    {
        dwAPCodeArea -= DS_AP_CODE_AREA;
    }


    if(__AP_Info.dwAP_Size > dwAPCodeArea)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: AP Size is %lx, larger than reserved space !\n", __AP_Info.dwAP_Size);
        return (NULL);
    }

    if(__AP_Info.dwVersionAP <= 4)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: APPacker Version %lx is too old !\n", __AP_Info.dwVersionAP);
        return (NULL);
    }

    // Step 4: Return AP_INFO structure pointer if allow to switch
    return &__AP_Info;
}

//  *********************************************************************
//  Function    :   AP_SwitchMode
//  Description :   Switch to desired AP mode
//  Arguments   :   bAPID, specify the desired AP ID
//  Return      :   TRUE, the aciton is ok
//                  FALSE, the action is failure
//  *********************************************************************
// LLY2.15, modify the procedure since AP location method has updated.
BYTE    AP_SwitchMode(BYTE bAPID)
{
    PAP_INFO        pAP_Info;
    BYTE            i=0;
    PSECTION_ENTRY  pSec=NULL;

    // Step 1: Find if exist the desired AP ID
    while(1)
    {
        // Check if exist section name.
        // I0f zero, exit it. itmeans end of section.
        if(pSec[i].dwName == 0)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Can't find AP since end of section\n");
            return (FALSE);
        }

        // Give section address to check if AP code
        pAP_Info=AP_Identify(SRC_FROM_FLASH, pSec[i].dwRMA);

        // Exit the loop directly if AP_Identify() return NULL
        // ie. don't support to switch AP
        if(pAP_Info == NULL)
        {
            i++;
            continue;
//            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Switch AP fail since AP_Identify() return FALSE\n");
//            return FALSE;
        }

        // Exit the finding loop if find the desired AP
        if(pAP_Info->dwAP_Type == bAPID)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Find the desired AP, location: %lx\n", pSec[i].dwRMA);
            break;
        }

        // increas sector index to check next section.
        i++;

        // exit the loop, if section index > supported number.
        if(i>=32)
        {
            DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: Can't find AP since section > 32\n");
            return (FALSE);
        }

    }

    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Switch to AP mode, ID: %lx\n", bAPID);


    // LLY2.00, stop decoder, parser, source before enter AP mode
    HAL_PlayCommand(COMMAND_STOP, 0);
    // wyc2.53-909P,
    SrcFilter_Stop(__dwSFStreamID);
    HAL_ControlParser(HAL_PARSER_STOP, 0);

    // Step 3: Remove all UI by following procedure.
    // Turn-off all UI, and related information, PIP ...
    OSD_RemoveAllUI();
    // Micky0.72, must put after OSD_Remove UI, as OSD need to reference some info.
    OSD_NormalMode();

    // Step 4: Start AP mode switch.
    __dwTemp=AP_Loader(SRC_FROM_FLASH, pSec[i].dwRMA, pSec[i].dwRSize);

    if(__dwTemp==FALSE)
    {
        BYTE bMode=0;

        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "AP_Loader() fail. So, re-booting\n");

        // Notice: need restore the AP mode to DVD mode
        HAL_WriteStorage(SETUP_ADDR_AP_MODE, &bMode, 1);

        // Re-booting.
        _AP_REBOOT();
    }

    return TRUE;
}


// LLY1.50, add one parameter for this API since AP_INFO_ADDITIONAL structure need it.
//VOID    _AP_Prepare_AdditionalInfo(VOID)
VOID    _AP_Prepare_AdditionalInfo(DWORD dwAPSource)
{
    PAP_INFO_ADDITIONAL     AP_Info_Additional;
    DWORD                   dwIdx;
    DWORD                   dwPCB_Number;
    DWORD dwSaveInt;
    // LLY2.05, define a pointer to IR_INFO structure to get IR related information.
    PIR_INFO                pIR_Info;

    // Get IR related information.
    pIR_Info = INPUT_GetIRInfo();

    // Get PCB Number
#ifdef SUPPORT_SERVO_SOURCE
    {
    extern BYTE SRVDBG_PCBNumber(void);
    dwPCB_Number = SRVDBG_PCBNumber();
    }
#else
    dwPCB_Number = 0x0;
#endif

    // Disable all INT to write DRAM 0x40000000 ~ 0x40001000 Memory
    OS_DISABLE_INTERRUPTS( dwSaveInt );
    // REG_PLAT_RESET_CONTROL_ENABLE = (PLAT_RESET_USB_ENABLE|PLAT_RESET_USBCLKCKT_ENABLE);
    // Chuan2.18, Can't reset Servo for GPB issue.
    REG_PLAT_RESET_CONTROL_ENABLE = (INT_SET_ALL&
        (~(PLAT_RESET_DSU1_ENABLE | PLAT_RESET_TIMER_ENABLE | PLAT_RESET_SERVO_ENABLE |
        PLAT_RESET_VOU_ENABLE | PLAT_RESET_VOU2_ENABLE | PLAT_RESET_PROM_ENABLE)));
    REG_PLAT_INT_MASK_PRIORITY = INT_CLEAR_ALL;
    REG_PLAT_INT_CLEAR = INT_SET_ALL;
    REG_PLAT_PROC1_1ST_INT_MASK_ENABLE = INT_SET_ALL;
    REG_PLAT_PROC1_1ST_INT_PENDING = INT_CLEAR_ALL;
    REG_PLAT_PROC1_2ND_INT_MASK_ENABLE = INT_SET_ALL;
    REG_PLAT_PROC1_2ND_INT_PENDING = INT_CLEAR_ALL;
    REG_PLAT_PROC1_1ST_INT_CLEAR = INT_SET_ALL;
    REG_PLAT_PROC1_2ND_INT_CLEAR = INT_SET_ALL;

    AP_Info_Additional                      = (PAP_INFO_ADDITIONAL)(0x40000000);
    AP_Info_Additional->pTableAddress       = (PSECTION_ENTRY)(AP_TABLE_ADDRESS);
    AP_Info_Additional->dwVersionFAE        = __dwVersionFAE;
    AP_Info_Additional->dwVersionServo      = __dwVersionServo;
    AP_Info_Additional->dwPCB_Number        = dwPCB_Number;
    AP_Info_Additional->dwVersionFlash      = FLASH_SW_VERSION;
//    AP_Info_Additional->dwVersionTool       = TOOLS_SW_VERSION;
    AP_Info_Additional->dwVersionAP_INFO_ADDITIONAL = 5;
    AP_Info_Additional->dwIRType            = (DWORD) pIR_Info->bIR_Type;
    AP_Info_Additional->dwAP_Type           = __AP_Info.dwAP_Type;
    AP_Info_Additional->dwExternalFlag      = __AP_Info.dwExternalFlag;

    switch(__bMPEGVideoMode)
    {
    case    MPEG_VIDEO_MODE_NTSC:
        if(__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
        {
            AP_Info_Additional->dwTVMode = MODE_NTSC_NORMAL | MODE_PSCAN;
        }
        else
        {
            AP_Info_Additional->dwTVMode = MODE_NTSC_NORMAL;
        }
        break;

    default:
    case    MPEG_VIDEO_MODE_PAL:
        if(__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
        {
            AP_Info_Additional->dwTVMode = MODE_PAL_NORMAL | MODE_PSCAN;
        }
        else
        {
            AP_Info_Additional->dwTVMode = MODE_PAL_NORMAL;
        }
        break;
    }

    // LLY2.05, using IR reported information to prepare the IR map table and size directly
    AP_Info_Additional->bIRMapSize            = pIR_Info->bIRMapSize;
    for(dwIdx=0; dwIdx<pIR_Info->bIRMapSize; dwIdx++)
    {
        AP_Info_Additional->bIRMap[dwIdx] = pIR_Info->aIRMap[dwIdx];
    }

    // LLY2.05, using IR reported information to prepare IR customer code value
    AP_Info_Additional->bIR_Customer_Code[0] = pIR_Info->bCustomerCode;
    AP_Info_Additional->bIR_Customer_Code[1] = pIR_Info->bCustomerCode1;

    // LLY1.50, assign new elements value for AP_INFO_ADDITIONAL structure v4.0
    AP_Info_Additional->wAPSource = (WORD)(dwAPSource);
    AP_Info_Additional->wAP_EEPROM_Addr = SETUP_ADDR_AP_MODE;

    // Append customer version string for Auto-Upgrade reference
    dwIdx = (DWORD)-1;
    do
    {
        dwIdx++;
        AP_Info_Additional->csVersionCustomer[dwIdx] = __csVersionCustomer[dwIdx];
    } while((dwIdx<24) && (__csVersionCustomer[dwIdx] != 0x0));

    ROMLD_MoveSectionTable((PSECTION_ENTRY)(DS_AP_CODE_AREA + sizeof(AP_INFO) + (DWORD)ROMLD_SECTION_TABLE_ADDR), (PSECTION_ENTRY)AP_TABLE_ADDRESS);
}
