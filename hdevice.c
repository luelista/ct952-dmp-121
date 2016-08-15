#include "winav.h"
#include "ctkav.h"
#include "ctkav_mcu.h"
#include "hio.h"
#include "hdevice.h"
#include "spflash.h"
#include "disp.h"    // because of DISP_TVMODE define
#include "romld.h"

typedef enum tagSET_REG_ID {
    SET_TVE_GAIN_YC,
    SET_TVE_GAIN_CAV,
} SET_REG_ID;

BOOL    _blDisableSVideo = FALSE;

// LLY2.77, porting Chuan's code to keep if force disable TV output off
BYTE    _bForce_TVOut_Off = FALSE;

DWORD   __dwREG_TVE_MV0 = 0x00000000; //kevin1.02, set MV0[4] within V-SYNC; otherwise, TVE output is abnormal (CT909S IC will fix it)
DWORD   __dwOffset ;
void    _ProgramACLK(BYTE bSampleFreq);
void    _ProgTVEGain(SET_REG_ID bSetID, DWORD dwRegValue, int nGainOffset);



// LLY2.55, declare the table for different solution's GAIN_YC & GAIN_CAV value.
// Notice: the layout must be same as following.
// Interlace mode
// { {NTSC-SVideo, NTSC-YUV, NTSC-RGB},
//   {PAL-SVideo,  PAL-YUV,  PAL-RGB },
//   {PALM-SVideo, PALM-YUV, PALM-RGB},
//   {PALN-SVideo, PALN-YUV, PALN-RGB} }
// P-Scan mode
// { {NTSC-SVideo, NTSC-YUV, NTSC-RGB, NTSC-VGA},
//   {PAL-SVideo,  PAL-YUV,  PAL-RGB , PAL-VGA },
//   {PALM-SVideo, PALM-YUV, PALM-RGB, PALM-VGA},
//   {PALN-SVideo, PALN-YUV, PALN-RGB, PALM-VGA} }


//  *******************************************************************************
//  Function    :   HAL_InitSerialFlash
//  Description :   Initialize serial flash to find the desired bank location.
//  Arguments   :   None
//  Return      :   None
//*********************************************************************************
void    HAL_InitSerialFlash(void)
{
    BYTE    *pbTmp;
    PSECTION_ENTRY ENTRY;

    // LLY2.17, remove it since it's only to slow speed for parallel flash
    //REG_PLAT_PROMCFG = 0x24D08;

//    SPF_ReadID(4);
    SPF_INFO *pSPF_INFO1;

    pSPF_INFO1 = (SPF_INFO*)(*((DWORD*)8)); //wingwu 7/28 for support no ID check
    SPF_INFO1 = *pSPF_INFO1;
    ENTRY= ROMLD_GetSectionByName (0,"SETD");
    dwROM_RANGE_START  =  ENTRY->dwRMA;
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "SETD[%lx]",dwROM_RANGE_START);

    switch  (SPF_INFO1.bBoot)
    {
        case BOOT_ALL: //address is 4k *n
             if(dwROM_RANGE_START%0x1000)
             {
                 dwROM_RANGE_START = NOT_SUPPORT_FLASH ;
             }
            break;

        case BOOT_NO: //not  support
            dwROM_RANGE_START = NOT_SUPPORT_FLASH ;
            break;

        case BOOT_BOTTOM :
             if(dwROM_RANGE_START != 0x01000)
             {
                dwROM_RANGE_START = NOT_SUPPORT_FLASH ;
             }
            break;

        case BOOT_TOP:
            switch  (SPF_INFO1.bSize)
            {
                case FLASH_SIZE_8M : //8M FLASH
                    if(dwROM_RANGE_START  != 0x0FF000 )
                    {
                       dwROM_RANGE_START = NOT_SUPPORT_FLASH ;
                    }
                   break;

                case FLASH_SIZE_4M : // 4M FLASH
                    if( dwROM_RANGE_START != 0x07F000 )
                    {
                       dwROM_RANGE_START = NOT_SUPPORT_FLASH ;
                    }
                   break;

                default :
                      dwROM_RANGE_START = NOT_SUPPORT_FLASH ;
                    break;
            }
            break;

        default :
            dwROM_RANGE_START = NOT_SUPPORT_FLASH ;
            break;
    }
    if(ENTRY->dwLSize == 0x1000)
    {//do check    4K size
        __dwOffset = dwROM_RANGE_START;
        dwROM_RANGE_START = dwROM_RANGE_START;
        dwROM_RANGE_MID = dwROM_RANGE_START+0x1000;
        dwROM_RANGE_END =dwROM_RANGE_START+0x1000;
    }
    else if(ENTRY->dwLSize == 0x2000)
    {//do check    8K size
        __dwOffset = dwROM_RANGE_START;
        dwROM_RANGE_START = dwROM_RANGE_START;
        dwROM_RANGE_MID = dwROM_RANGE_START+0x1000;
        dwROM_RANGE_END =dwROM_RANGE_MID+0x1000;
    }
    else
    {
        dwROM_RANGE_START = NOT_SUPPORT_FLASH;
         __dwOffset = dwROM_RANGE_START;
        dwROM_RANGE_START = dwROM_RANGE_START;
        dwROM_RANGE_MID = dwROM_RANGE_START+0x1000;
        dwROM_RANGE_END =dwROM_RANGE_MID+0x1000;
    }
    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Flash ad[%lx,%lx,%lx]",dwROM_RANGE_START,dwROM_RANGE_MID,dwROM_RANGE_END);

    pbTmp  =  (BYTE *)__dwOffset;

    for(;pbTmp<(BYTE *) dwROM_RANGE_END ;pbTmp+=ROM_BANK_SIZE)
    {
        if(pbTmp[0] != 0xff)
        {
            for(pbTmp += ROM_BANK_SIZE
            		;pbTmp< (BYTE *)dwROM_RANGE_END
            				;pbTmp += ROM_BANK_SIZE)
            {
                if(pbTmp[0] == 0xff)
                {
                    // LLY2.53, porting Wing's code to support > 256 bytes setup info.
                    //__dwOffset= pbTmp - (BYTE*)ROM_BANK_SIZE;
                    __dwOffset=(DWORD) pbTmp - (DWORD) ROM_BANK_SIZE;
                    //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "last data<%lx> \n",__dwOffset);
                    // LLY2.35, porting Wing's code to fix data can't find in SPI sometime.
                    //pbTmp = (BYTE *) dwROM_RANGE_END ; //end find
                    //break;
                    return;
                }
            }
        }
    }

    // LLY2.35, porting Wing's code to fix data can't find in SPI sometime.
    __dwOffset= dwROM_RANGE_END - ROM_BANK_SIZE;
    //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "last data<%lx> \n",__dwOffset);
    return;

}


// LLY2.51, porting Wing's code to add a API to write data to SPI Flash
//  *********************************************************************
//  Function    :   HAL_WriteSPF
//  Description :   Write data to serial flash
//  Arguments   :   dwAddr: desired serial flash adress
//                  pbValue: ptr to the to be written value
//                  dwCnt: # of bytes, can't exceed 64K bytes
//  Return      :   TRUE : write OK
//              :   FALSE : wrute faile
//  Notice      :   dwAddr must be 64K byte align.
//*********************************************************************
BYTE HAL_WriteSPF(DWORD dwAddr, BYTE* pbVal, DWORD dwCnt)
{
    BYTE bBuff;

//    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
//        "Addr: %lx, pbVal: %lx, dwCnt: %lx\n", dwAddr, pbVal, dwCnt);

    // check dwAddr if 64K bytes align
    if((dwAddr%0x10000) !=0)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: WriteSPF fail since desired address not 64K align\n");
         return FALSE;
    }

    // check if desired count > 64K bytes
    if(dwCnt > 0x10000)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: WriteSPF fail since desired size exceed 64K byte\n");
        return FALSE;
    }

    // Must turn on TFT mode and delay two vsync before SPI Flash action
    // Since, SPI Flash Erase/ Write action will disable all interrupt
    //DISP_DisplayCtrl(DISP_TFT_MODE, TRUE);
    //DISP_DelayVSYNC(2);

    if(SPF_INFO1.bBoot == BOOT_ALL)
    {
        for( bBuff =0 ;bBuff < 16 ;bBuff++)
        {
            SPF_EraseSector(dwAddr+0x1000*bBuff);
        }
    }
    else
    {
        SPF_EraseSector(dwAddr);
    }


//    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF,
//        "%lx, %lx, %lx, %lx\n", pbVal[0], pbVal[1], pbVal[2], pbVal[3]);

    //寫入
    bBuff=SPF_WriteProgram( (DWORD)pbVal,(DWORD)(pbVal + dwCnt), dwAddr );

    if(!bBuff)
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: WriteSPF fail since action fail\n");
    }

    // Turn off TFT mode after SPI Flash action ok
    //DISP_DisplayCtrl(DISP_TFT_MODE, FALSE);

    return bBuff;
}


// LLY2.61, declear a global array for HAL_WriteSPF_BANK usage
// Old method is will shared w/ DS_UNZIP_BUF_ST(DS_DEBUG_BUF_ST),
// ie. the buffer data myabe destoryed by debug message
BYTE abBuf[ROM_BANK_SIZE];


// LLY2.51, porting Wing's code to rename HAL_WriteSerialFlash() to HAL_WriteSPF_BANK()
// LLY2.53, porting Wing's code to support > 256 bytes setup info.
//  *********************************************************************
//  Function    :   HAL_WriteSPF_BANK
//  Description :   Write data to serial flash
//  Arguments   :   wAddr: desired serial flash adress
//                  pbValue: ptr to the to be written value
//                  wCnt: # of bytes
//  Return      :   None
//  Notice      :   This API is only used for saving setup information.
//*********************************************************************
//void    HAL_WriteSPF_BANK(BYTE bAddr, BYTE* pbVal, BYTE bCnt)
void    HAL_WriteSPF_BANK(WORD wAddr, BYTE* pbVal, WORD wCnt)
{
    WORD wTemp1,wTemp2;
    DWORD dwDRAM_Str_Addr,dwDRAM_End_Addr,dwFLASH_Addr;

    // Notice: don't declare those array as global variable
    //         since the array size is large, and it will take bss space.
//    BYTE abBuf[ROM_BANK_SIZE];
    BYTE *pBuf;

    // LLY2.61, use global array for it since old methed is shared w/ unzip and debug buffer
    // So, the data maybe destoryed by debug message
    //pBuf = (BYTE *)DS_UNZIP_BUF_ST;
    pBuf = abBuf;

    //確定BOOT型態是否合
    if( dwROM_RANGE_START == NOT_SUPPORT_FLASH )
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Boot fail flash ,this Boot is %hd",SPF_INFO1.bBoot );
        return;
    }

    dwDRAM_Str_Addr = (DWORD)  (pbVal);
    dwDRAM_End_Addr = (DWORD)(pbVal+wCnt);
    //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "{St=%hd,Si=%lx}",wAddr,wCnt);

    // 確定資料是否可以寫入
    SPF_ReadData((__dwOffset+wAddr), pBuf,wCnt); // get old data
    //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "[%lx,%lx,%lx,%lx]",dwROM_RANGE_START,dwROM_RANGE_MID,dwROM_RANGE_END,__dwOffset);
    for(wTemp1=0 ; wTemp1 < wCnt ; wTemp1++)
       {
       if( pBuf[wTemp1] != 0xff)
       {//have data in old bank, need find next bank

            //chage all data to buff
            SPF_ReadData(__dwOffset, pBuf, ROM_BANK_SIZE);
            //add change value
            for(wTemp1=0 ; wTemp1 < wCnt ; wTemp1++)
            {
                pBuf[wAddr+wTemp1] = pbVal[wTemp1];
            }

            //next bank
            __dwOffset +=ROM_BANK_SIZE;

            if(__dwOffset >= dwROM_RANGE_END)
            {
              DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "serial Flash Cycle");
               __dwOffset = dwROM_RANGE_START;
              if(dwROM_RANGE_END==dwROM_RANGE_MID)
              {// only one 4K size
                DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Erase sector ");
                SPF_EraseSector(dwROM_RANGE_START);
              }
              __dwOffset = dwROM_RANGE_START;

            }
            //change write range
            wAddr = 0;
            dwDRAM_Str_Addr = (DWORD) pBuf;
            dwDRAM_End_Addr = (DWORD)(pBuf+ROM_BANK_SIZE);
            break;
        }
    }

    //DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "[Wo=%lx,St=%hd,Si=%lx]",__dwOffset,bAddr,dwDRAM_End_Addr-dwDRAM_Str_Addr);

    dwFLASH_Addr = (DWORD)(__dwOffset+ wAddr);

    if((SPF_INFO1.wID&0xff00) == 0x1f00)
    {//for atmal ,because it need unprotect everytime power on.
        SPFcommand(SPF_INFO1.bWriteEnable,Format_Write_Enable, WaitCMD_STATE,50);
        ReadStatus(WEL_STATE,  2  ,50);
        SPFcommand(0x100039,0x03, WaitCMD_STATE,50);
    }

    //寫入
    SPF_WriteProgram(dwDRAM_Str_Addr,dwDRAM_End_Addr,dwFLASH_Addr);

    //確保下次可以寫入
    if(dwROM_RANGE_END != dwROM_RANGE_MID)
    { //have two 4K size
        if(__dwOffset == dwROM_RANGE_START )
        {
            SPF_ReadData(dwROM_RANGE_MID, pBuf,ROM_BANK_SIZE);
            for(wTemp2 =0 ;  wTemp2 < ROM_BANK_SIZE ;  wTemp2++)
            {
                if(pBuf[wTemp2] !=0xff)
                {
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Erase sector 1");
                    SPF_EraseSector(dwROM_RANGE_MID);
                    break;
                }
            }
        }

        if(__dwOffset == dwROM_RANGE_MID )
        {
            SPF_ReadData(dwROM_RANGE_START, pBuf,ROM_BANK_SIZE);
            for(wTemp2 =0; wTemp2<ROM_BANK_SIZE; wTemp2++)
            {
                if(pBuf[wTemp2] !=0xff)
                {
                    DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Erase sector 0");
                    SPF_EraseSector(dwROM_RANGE_START);
                    break;
                }
            }
        }
    }
}


//  *********************************************************************
//  Function    :   HAL_ReadSerialFlash
//  Description :   Read data from serial flash
//  Arguments   :   wAddr: desired serial flash address
//                  pbValue: ptr to the to be written value
//                  wCnt: # of bytes
//  Return      :   None
//  *********************************************************************
// LLY2.53, porting Wing's code to support > 256 bytes setup info.
//void HAL_ReadSerialFlash(BYTE bAddr, BYTE* pbVal, BYTE bCnt)
void HAL_ReadSerialFlash(WORD wAddr, BYTE* pbVal, WORD wCnt)
{
    if( dwROM_RANGE_START == NOT_SUPPORT_FLASH )
    {
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Boot fail flash ,this Boot is %hd",SPF_INFO1.bBoot );
        return;
    }
    SPF_ReadData(((DWORD)wAddr+__dwOffset), pbVal,(DWORD)wCnt);
    return;
}


//*********************************************************************
// Description  :   Write data to EEPROM
// Arguments    :   bAddr: desired EEPROM adress
//                  pbValue: ptr to the to be written value
//                  bCnt: # of bytes
// Return       :   None
//*********************************************************************
void    HAL_WriteEEPROM(BYTE bAddr, BYTE* pbVal, BYTE bCnt)
{
#define MAX_EPROM_WRITE_NO      8

    // It's ok to reserved max EEPROM write number space for array.
    BYTE   bBuffer[MAX_EPROM_WRITE_NO];
    BYTE    bPageRem;
    BYTE    bTemp;
    // Micky2.11a
    TRANSFER_CONFIG TransferConfig;

    // LLY2.01, specify the desired I2C config, ex GPIO group, SCLK pin, SDAT pin
    I2C_CONFIG  I2C_Config;
    I2C_Config.bIOGroup=EEPROM_GPIO_GRP;
    I2C_Config.bSDAT_Pin=PIN_EEPROM_SDAT;
    I2C_Config.bSCLK_Pin=PIN_EEPROM_SCLK;


    // according to AT24C02 spec, page mode must assign page alignment address,
    // otherwise it may "roll over" to the start of the same page and overwrite its content.
    bPageRem = bAddr % MAX_EPROM_WRITE_NO;
    if (bPageRem) // if address is not page start, just write to the end of the page
    {
        bBuffer[0] = bAddr;  // assign address
        bPageRem = MAX_EPROM_WRITE_NO - bPageRem;
        if ( bCnt < bPageRem)
		{
            bPageRem = bCnt;
		}
        bAddr += bPageRem;
        bCnt -= bPageRem;

        for(bTemp=1; bTemp<=bPageRem; bTemp++)  // assign value
		{
            bBuffer[bTemp]= *pbVal++;
		}
        // Micky2.11a
//        _I2C_Write(&I2C_Config, I2C_ID_EEPROM, bBuffer, (BYTE)(bPageRem+1));
        TransferConfig.bAddr = bBuffer[0];
        TransferConfig.bCount = bPageRem;
        TransferConfig.pbBuffer = &bBuffer[1];
        HAL_I2C_Write (I2C_ID_EEPROM, &I2C_Config, &TransferConfig);
    }

    while(bCnt)
    {
        bBuffer[0] = bAddr;  // assign address
        if(bCnt>MAX_EPROM_WRITE_NO)
        {
            bPageRem = MAX_EPROM_WRITE_NO;
            bAddr += MAX_EPROM_WRITE_NO;
            bCnt -= MAX_EPROM_WRITE_NO;
        }
        else
        {
            bPageRem=bCnt;
            bCnt=0;
        }

        for(bTemp=1; bTemp<=bPageRem; bTemp++)  // assign value
		{
            bBuffer[bTemp]= *pbVal++;
		}
        // Micku2.11a
//        _I2C_Write(&I2C_Config, I2C_ID_EEPROM, bBuffer, (BYTE)(bPageRem+1));
        TransferConfig.bAddr = bBuffer[0];
        TransferConfig.bCount = bPageRem;
        TransferConfig.pbBuffer = &bBuffer[1];
        HAL_I2C_Write (I2C_ID_EEPROM, &I2C_Config, &TransferConfig);
    }

}



//*********************************************************************
// Description  :   Read data from EEPROM
// Arguments    :   bAddr: desired EEPROM address
//                  pbValue: ptr to the to be written value
//                  bCnt: # of bytes
// Return       :   None
//*********************************************************************
void    HAL_ReadEEPROM(BYTE bAddr, BYTE* pbVal, BYTE bCnt)
{
    // LLY2.11, call serial flash API temporally

    // Micky2.11a
    TRANSFER_CONFIG TransferConfig;
    // LLY2.01, specify the desired I2C config, ex GPIO group, SCLK pin, SDAT pin
    I2C_CONFIG  I2C_Config;
    I2C_Config.bIOGroup=EEPROM_GPIO_GRP;
    I2C_Config.bSDAT_Pin=PIN_EEPROM_SDAT;
    I2C_Config.bSCLK_Pin=PIN_EEPROM_SCLK;

    // Notice: Within CT909 IC design
    //         I2C_SCK (GPA27) & I2C_SDA (GPA26) are shared with MIC_ACLK and MIC_BCLK @ CT909 IC design
    //         And, MIC default is enable (REG_PLAT_SYSTEM_CONFIGURATION bit 31)
    //         So, it maybe affect I2C procedure.
    //         Now, always disable MIC feature (Within HAL_InitialVaribles())

    // MIcky2.11a
//    _I2C_Read(&I2C_Config, I2C_ID_EEPROM, bAddr, bCnt, pbVal);
        TransferConfig.bAddr = bAddr;
        TransferConfig.bCount = bCnt;
        TransferConfig.pbBuffer = pbVal;
        HAL_I2C_Read (I2C_ID_EEPROM, &I2C_Config, &TransferConfig);
}


//**********************************************************************************
// Description  :   Set the Gain register of video
// Arguments    :
// Return       :   None
// Notice       :
//*********************************************************************************
DWORD HAL_SetTVEncoder(BYTE bColorSpace, BYTE bScanMode, BYTE bTVSystem, BYTE bVideoOutput, int nGainOffset)
{
    DWORD dwTVETemp = 0;
    DWORD dwREG_TVE_R0 = 0;
    //Kevin1.02-2, Radj=75ohm
    // dwREG_TVE_R1 = REG_TVE_R1;
if (bColorSpace == HAL_TVE_COLOR_SPACE_MPEG) //MPEG
{
    if (bScanMode != PSCAN_ON) //Interlaced
    {
     //kevin1.00, adjust gain settings according to video performance test result
        switch (bTVSystem)
        {
            case TVSYSTEM_NTSC:
                //default is not NTSC443
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_AV_SVIDEO:
// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef  CT909G_IC_SYSTEM
                        REG_TVE_R2 = 0x00000010;
#endif  // #ifdef  CT909G_IC_SYSTEM

#ifdef SUPPORT_75IRE
                        dwREG_TVE_R0 = 0x02000020; //7.5 IRE
#else
                        dwREG_TVE_R0 = 0x02000000; //0 IRE
#endif
#ifdef SUPPORT_2_CVBS
                        REG_TVE_R1 = 0x00000000;
#else //turn off AVO0
                        REG_TVE_R1 = 0x00000400;
#endif
                        // LLY2.55, porting Joie's code to modify GAIN_YC value for CT909P & CT952A
                        // LLY2.56, suggest TVE gain value are same between CT909P and CT909G
                        // LLY2.61, give different value for CT909G & CT909P
#if  defined(CT909P_IC_SYSTEM)
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x48544864, nGainOffset);
#elif defined(CT909G_IC_SYSTEM)
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x48554864, nGainOffset);
#else   // Not CT909P & Not 909G IC
                        //Joie for CT909R Radj=75 Fsadj=430
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE //Joie2.35
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x3E544059, nGainOffset);
#else   // #ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE //Joie2.35
#ifdef CT951_PLATFORM
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x45584663, nGainOffset);
#else   // #ifdef CT951_PLATFORM
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x49544866, nGainOffset);
#endif  // #ifdef CT951_PLATFORM
#endif  // #ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
#endif  // Not CT909P & Not 909G IC
                        break;

                    case VIDEOOUTPUT_AV_YUV:
// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef  CT909G_IC_SYSTEM
                        REG_TVE_R2 = 0x00000020;
#endif  // #ifdef  CT909G_IC_SYSTEM

#ifdef SUPPORT_75IRE
                        dwREG_TVE_R0 = 0x02000020; //7.5 IRE
#else
                        dwREG_TVE_R0 = 0x02000000;
#endif
                        REG_TVE_R1 = 0x00002002;
                        // LLY2.61, porting AP10-Joie's code for CT909G IC
#ifdef  CT909G_IC_SYSTEM
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4A574969, nGainOffset);
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA0A00A0, nGainOffset);
#else   // #ifdef CT909G_IC_SYSTEM
                        //Joie for CT909R YUV of CVBS Sync Level can reduce 2IRE
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x415D425C, nGainOffset);
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAD0AC0A9, nGainOffset);
#else
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4B584B6A, nGainOffset);
                        //Joie for CT909R Radj=75 Fsadj=430
                        //Y2YG: 0xAA, CB2PBB: 0xA8, CR2PRR: 0xA5
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA0A80A5, nGainOffset);
#endif
#endif  // #ifdef CT909G_IC_SYSTEM
                        break;

                    case VIDEOOUTPUT_AV_RGB:
// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef  CT909G_IC_SYSTEM
                        REG_TVE_R2 = 0x00000020;//Joie_CT909G_none
#endif  // #ifdef  CT909G_IC_SYSTEM
#ifdef SUPPORT_75IRE
                        dwREG_TVE_R0 = 0x02000020; //7.5 IRE
#else
                        dwREG_TVE_R0 = 0x02000000;//Joie G Sync OFF for Macrovision
#endif
                        REG_TVE_R1 = 0x00002004;
                        // LLY2.61, porting AP10-Joie's code for CT909G IC
#ifdef  CT909G_IC_SYSTEM
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4A574968, nGainOffset);
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA1220E3, nGainOffset);
                        REG_TVE_GAIN_G = 0x00083776;
#else   // #ifdef CT909G_IC_SYSTEM
                        //Joie for CT909R RGB of CVBS Sync Level can reduce 2IRE
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x415D425C, nGainOffset);
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAE12A0E9, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003A79;
#else
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4B584A69, nGainOffset);
                        //Y2YG: 0xAA, CB2PBB: 0x121, CR2PRR: 0xDF
                        //Joie for CT909R Radj=75 Fsadj=430
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA1210DF, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003776;//Joie for CT909R Radj=75 Fsadj=430
#endif
#endif  // #ifdef CT909G_IC_SYSTEM
                        break;
                }
                break;

            case TVSYSTEM_PAL:
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_AV_SVIDEO:
// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef  CT909G_IC_SYSTEM
                        REG_TVE_R2 = 0x00000010;
#endif  // #ifdef CT909G_IC_SYSTEM

                        dwREG_TVE_R0 = 0x02000080;
#ifdef SUPPORT_2_CVBS
                        REG_TVE_R1 = 0x00002000;
#else //turn off AVO0
                        REG_TVE_R1 = 0x00002400;
#endif

                        // LLY2.55, porting Joie's code to modify GAIN_YC value for CT909P & CT952A
                        // LLY2.56, suggest TVE gain value are same between CT909P and CT909G
                        // LLY2.61, give different value for CT909G & CT909P
#if defined(CT909P_IC_SYSTEM)
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x46534D6B, nGainOffset);
#elif defined(CT909G_IC_SYSTEM)
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x49554E6E, nGainOffset);
                        REG_TVE_GAIN_G = 0x00140000;
#else   // Not CT909P & Not CT909G
                        //Joie for CT909R Radj=75 Fsadj=430 4/20
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x3C54415A, nGainOffset);
#else
#ifdef CT951_PLATFORM
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x43564A66, nGainOffset);
#else
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4A525070, nGainOffset);
#endif //CT951_PLATFORM
#endif
#endif  // Not CT909P & Not CT909G
                        break;

                    case VIDEOOUTPUT_AV_YUV:
// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef  CT909G_IC_SYSTEM
                        REG_TVE_R2 = 0x00000020;
#endif  // #ifdef CT909G_IC_SYSTEM
                        dwREG_TVE_R0 = 0x02000080;
                        REG_TVE_R1 = 0x00002002;
                        // LLY2.61, porting AP10-Joie's code for CT909G IC
#ifdef  CT909G_IC_SYSTEM
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4A575071, nGainOffset);
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA0A00A0, nGainOffset);
                        REG_TVE_GAIN_G = 0x10000000;
#else   //  #ifdef CT909G_IC_SYSTEM
                        //Joie for CT909R Radj=75 Fsadj=430
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x3E5A425F, nGainOffset);
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAB0AC0A7, nGainOffset);
#else
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4C575172, nGainOffset);
                        //Y2YG: 0xA9, CB2PBB: 0xAA, CR2PRR: 0xA5
                        //Joie for CT909R Radj=75 Fsadj=430
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xA90AA0A5, nGainOffset);
#endif
#endif  // #ifdef CT909G_IC_SYSTEM
                        break;

                    case VIDEOOUTPUT_AV_RGB:
// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef  CT909G_IC_SYSTEM
                        REG_TVE_R2 = 0x00000020;//Joie_CT909G_none
                        dwREG_TVE_R0 = 0x02100080;//Joie G Sync OFF for Macrovision //Joie_CT909G
#else   // #ifdef CT909G_IC_SYSTEM
                        dwREG_TVE_R0 = 0x02100080;//Joie G Sync OFF for Macrovision
#endif  // #ifdef CT909G_IC_SYSTEM
                        REG_TVE_R1 = 0x00002004;

// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef  CT909G_IC_SYSTEM
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4A575071, nGainOffset);
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA1220E3, nGainOffset);
                        REG_TVE_GAIN_G = 0x000F3777;
#else   // #ifdef CT909G_IC_SYSTEM
                        //Joie for CT909R Radj=75 Fsadj=430
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x3E5A425F, nGainOffset);
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAC1270E7, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003A78;
#else
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4C575172, nGainOffset);
                        //Y2YG: 0xAA, CB2PBB: 0x12E, CR2PRR: 0xE0
                        //Joie for CT909R Radj=75 Fsadj=430
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA12E0E0, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003777;//Joie for CT909R Radj=75 Fsadj=430
#endif
#endif   // #ifdef CT909G_IC_SYSTEM
                        break;
                }
                break;

            case TVSYSTEM_PALM:
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_AV_SVIDEO:
                        dwREG_TVE_R0 = 0x02000040;
#ifdef SUPPORT_2_CVBS
                        REG_TVE_R1 = 0x00002000;
#else //turn off AVO0
                        REG_TVE_R1 = 0x00002400;
#endif
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4C525273, nGainOffset);
                        break;

                   case VIDEOOUTPUT_AV_YUV:
                        dwREG_TVE_R0 = 0x02000040;
                        REG_TVE_R1 = 0x00002002;
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x404f4d6b, nGainOffset);
                        // Y2YG: 0x9E, CB2PBB: 0x85, CR2PRR: 0xB5
                        _ProgTVEGain(SET_TVE_GAIN_CAV, (0x9E << 24) | (0x85 << 12) | 0xB5, nGainOffset);
                        break;

                    case VIDEOOUTPUT_AV_RGB:
                        dwREG_TVE_R0 = 0x02400040;
                        REG_TVE_R1 = 0x00002004;
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x404f4c6b, nGainOffset);
                        // Y2YG: 0x9B, CB2PBB: 0x115, CR2PRR: 0xD0
                        _ProgTVEGain(SET_TVE_GAIN_CAV, (0x9B << 24) | (0x115 << 12) | 0xD0, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003b6b;
                        break;
                }
                break;

            case TVSYSTEM_PALN:
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_AV_SVIDEO:
                        dwREG_TVE_R0 = 0x020000c0;
#ifdef SUPPORT_2_CVBS
                        REG_TVE_R1 = 0x00002000;
#else //turn off AVO0
                        REG_TVE_R1 = 0x00002400;
#endif
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x40505273, nGainOffset);
                        break;

                    case VIDEOOUTPUT_AV_YUV:
                        dwREG_TVE_R0 = 0x020000c0;
                        REG_TVE_R1 = 0x00002002;
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x404f4d6b, nGainOffset);
                        // Y2YG: 0x9E, CB2PBB: 0x85, CR2PRR: 0xB5
                        _ProgTVEGain(SET_TVE_GAIN_CAV, (0x9E << 24) | (0x85 << 12) | 0xB5, nGainOffset);
                        break;

                    case VIDEOOUTPUT_AV_RGB:
                        dwREG_TVE_R0 = 0x024000c0;
                        REG_TVE_R1 = 0x00002004;
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x404f4c6b, nGainOffset);
                        // Y2YG: 0x9B, CB2PBB: 0x115, CR2PRR: 0xD0
                        _ProgTVEGain(SET_TVE_GAIN_CAV, (0x9B << 24) | (0x115 << 12) | 0xD0, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003b6b;
                        break;
                }
                break;
        }
        //REG_TVE_GAIN_YC |= 0x40000000; //Joie2.35 Because some burst gain need < 4 so disable it
    }
    else //PScan
    {
#ifndef NO_PSCAN_OUTPUT
        switch (bTVSystem)
        {
            case TVSYSTEM_NTSC:
            // LLY2.51, porting XuLi's code for PScan mode PAL-M setting (it's same as NTSC mode)
            case TVSYSTEM_PALM:
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_YUV_PSCAN:
#ifdef SUPPORT_75IRE
                        dwREG_TVE_R0 = 0x00000020; //7.5 IRE
#else
                        dwREG_TVE_R0 = 0x00000000;
#endif
                        //Kevin1.07a, disable 525p MV
                        REG_TVE_R1 = 0x0000a002;
// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef  CT909G_IC_SYSTEM
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAB0A20A1, nGainOffset);
                        REG_TVE_GAIN_G = 0x00020000;
#else   // #ifdef CT909G_IC_SYSTEM
                        //Y2YG: 0xAD, CB2PBB: 0xA8, CR2PRR: 0xA5
                        //Joie for CT909R Radj=75 Fsadj=430
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAD0AC0A9, nGainOffset);
#else
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAD0A80A5, nGainOffset);
#endif
#endif  // #ifdef CT909G_IC_SYSTEM
                        break;

                    case VIDEOOUTPUT_RGB_PSCAN:
#ifdef SUPPORT_75IRE
                        dwREG_TVE_R0 = 0x00400020; //7.5 IRE
#else
                        dwREG_TVE_R0 = 0x00400000;
#endif

// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef CT909G_IC_SYSTEM
                        REG_TVE_R1 = 0x0000a005;
#else  // #ifdef CT909G_IC_SYSTEM
                        //Kevin1.07a, disable 525p MV
                        REG_TVE_R1 = 0x0000a004;
#endif // #ifdef CT909G_IC_SYSTEM


// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef CT909G_IC_SYSTEM
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA1210E5, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003776;
#else  // #ifdef CT909G_IC_SYSTEM
                        //Y2YG: 0xAA, CB2PBB: 0x121, CR2PRR: 0xDF
                        //Joie for CT909R Radj=75 Fsadj=430
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAE12A0EA, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003A79;
#else
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA1210DF, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003776;//Joie for CT909R Radj=75 Fsadj=430
#endif
#endif  // #ifdef CT909G_IC_SYSTEM
                        break;

                    case VIDEOOUTPUT_VGA_PSCAN:
                        //Kevin0.87, Hsync/Vsync signal for VGA
		                dwTVETemp = (1<<23);
						 //Kevin1.07a, disable 525p MV
                        dwREG_TVE_R0 = 0x02000020;
                        REG_TVE_R1 = 0x3000a004;

// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef CT909G_IC_SYSTEM
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA1210E5, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003776;
#else  // #ifdef CT909G_IC_SYSTEM
                        //Y2YG: 0xAA, CB2PBB: 0x121, CR2PRR: 0xDF
                        //Joie for CT909R Radj=75 Fsadj=430
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAE12A0EA, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003A79;
#else
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA1210DF, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003776;//Joie for CT909R Radj=75 Fsadj=430
#endif
#endif  // #ifdef CT909G_IC_SYSTEM
                        break;
                }
                break;

            case TVSYSTEM_PAL:
            // LLY2.51, porting XuLi's code to remove PScan mode PAL-M setting to same as NTSC mode
            //case TVSYSTEM_PALM:  //xuli.1204
            case TVSYSTEM_PALN:
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_YUV_PSCAN:
                        // LLY2.60, porting Chuan's code to fix PSCAN @ PAL/PALN mode incorrect issue
                        // ie. must enable bit[7:6]=10b
                        // Otherwise, can't detect CGMS signal
                        dwREG_TVE_R0 = 0x00000000 + (0x2L<<6);
                        REG_TVE_R1 = 0x0000f002;

// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef CT909G_IC_SYSTEM
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA0A20A1, nGainOffset);
                        REG_TVE_GAIN_G = 0x00020000;
#else  // #ifdef CT909G_IC_SYSTEM
                      //Y2YG: 0xAA, CB2PBB: 0xAA8, CR2PRR: 0xA4
                        //Joie for CT909R Radj=75 Fsadj=430
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAB0AC0A7, nGainOffset);
#else
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA0A80A4, nGainOffset);
#endif
#endif // #ifdef CT909G_IC_SYSTEM
                        break;

                    case VIDEOOUTPUT_RGB_PSCAN:   //PAL
                        // LLY2.60, porting Chuan's code to fix PSCAN @ PALx mode incorrect issue
                        // ie. must enable bit[7:6]=10b
                        // Otherwise, can't detect CGMS signal
                        dwREG_TVE_R0 = 0x00400000 + (0x2L<<6);

// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef CT909G_IC_SYSTEM
                        REG_TVE_R1 = 0x0000f005;
#else  // #ifdef CT909G_IC_SYSTEM
                        REG_TVE_R1 = 0x0000f004;
#endif // #ifdef CT909G_IC_SYSTEM

// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef  CT909G_IC_SYSTEM
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xA91210E4, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003776;
#else   // #ifdef CT909G_IC_SYSTEM
                        //Y2YG: 0xAA, CB2PBB: 0x12E, CR2PRR: 0xE0
                        //Joie for CT909R Radj=75 Fsadj=430
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAD1270E7, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003A78;
#else
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA12E0E0, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003777;//Joie for CT909R Radj=75 Fsadj=430
#endif
#endif  // #ifdef CT909G_IC_SYSTEM
                        break;

                    case VIDEOOUTPUT_VGA_PSCAN:
                        //Kevin0.87, Hsync/Vsync signal for VGA
		                dwTVETemp = (1<<23);
                        dwREG_TVE_R0 = 0x02000080;
                        REG_TVE_R1 = 0x3000e004;
// LLY2.61, porting AP10-Joie's code to pass CT909G video performance issue
#ifdef  CT909G_IC_SYSTEM
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xA91210E4, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003776;
#else   // #ifdef CT909G_IC_SYSTEM
                        //Y2YG: 0xAA, CB2PBB: 0x12E, CR2PRR: 0xE0
                        //Joie for CT909R Radj=75 Fsadj=430
#ifdef SUPPORT_VIDEO_CIRCUIT_ADD_INDUCTANCE
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAD1270E7, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003A78;
#else
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xAA12E0E0, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003777;//Joie for CT909R Radj=75 Fsadj=430
#endif
#endif  // #ifdef CT909G_IC_SYSTEM

                        break;
                }
                break;
        }
#else   // #ifndef NO_PSCAN_OUTPUT
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: don't support PSCAN\n");
#endif  // #ifndef NO_PSCAN_OUTPUT
    }
}
else //JPEG
{
    if (bScanMode != PSCAN_ON) //Interlaced
    {
        //kevin1.00, adjust gain settings according to video performance test result
        switch (bTVSystem)
        {
            case TVSYSTEM_NTSC:
                //default is not NTSC443
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_AV_SVIDEO:
#ifdef SUPPORT_75IRE
                        dwREG_TVE_R0 = 0x02002020; //7.5 IRE
#else
                        dwREG_TVE_R0 = 0x02002000; //0 IRE
#endif
                        REG_TVE_R1 = 0x00000000;
                        //Joie JPEG public value Base on MPEG 20051109 Public value
                        //_ProgTVEGain(SET_TVE_GAIN_YC, 0x0048394f, nGainOffset); //original
#ifdef CT951_PLATFORM
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4e41374f, nGainOffset); //Joie 200610 for CT909R
#else
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4e4c3f59, nGainOffset); //Joie 200610 for CT909R
#endif //CT951_PLATFORM
                        break;

                    case VIDEOOUTPUT_AV_YUV:
#ifdef SUPPORT_75IRE
                        dwREG_TVE_R0 = 0x02002020; //7.5 IRE
#else
                        dwREG_TVE_R0 = 0x02002000;
#endif
                        REG_TVE_R1 = 0x00002002;
                        //Joie JPEG public value Base on MPEG 20051109 Public value
                        //_ProgTVEGain(SET_TVE_GAIN_YC, 0x0048394f, nGainOffset); //original
                        //_ProgTVEGain(SET_TVE_GAIN_CAV, (0x8D << 24) | (0x8E << 12) | 0x86, nGainOffset); //original
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4c4c425d, nGainOffset); //Joie 200610 for CT909R
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xaa087091, nGainOffset); //Joie 200610 for CT909R
                        break;

                    case VIDEOOUTPUT_AV_RGB:
#ifdef SUPPORT_75IRE
                        dwREG_TVE_R0 = 0x02402020; //7.5 IRE
#else
                        dwREG_TVE_R0 = 0x02402000;
#endif
                        REG_TVE_R1 = 0x00002004;
                        //Joie JPEG public value Base on MPEG 20051109 Public value
                        //_ProgTVEGain(SET_TVE_GAIN_YC, 0x0048394f, nGainOffset); //original
                        //_ProgTVEGain(SET_TVE_GAIN_CAV, (0x8D << 24) | (0x1D << 12) | 0xC4, nGainOffset); //original
                        //REG_TVE_GAIN_G = 0x00002f64; //Joie JPEG public value Base on MPEG 20051109 Public value //original
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4c4c415c, nGainOffset); //Joie 200610 for CT909R
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xaa0f00c4, nGainOffset); //Joie 200610 for CT909R
                        REG_TVE_GAIN_G = 0x00003068; //Joie 200610 for CT909R
                        break;
                }
                break;

            case TVSYSTEM_PAL:
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_AV_SVIDEO:
                        dwREG_TVE_R0 = 0x02002080;
                        REG_TVE_R1 = 0x00002000;
                        //Joie JPEG public value Base on MPEG 20051109 Public value
                        //_ProgTVEGain(SET_TVE_GAIN_YC, 0x00473e57, nGainOffset); //original
#ifdef CT951_PLATFORM
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4e413d56, nGainOffset); //Joie 200610 for CT909R
#else
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4d4c4662, nGainOffset); //Joie 200610 for CT909R
#endif //CT951_PLATFORM
                        break;

                    case VIDEOOUTPUT_AV_YUV:
                        dwREG_TVE_R0 = 0x02002080;
                        REG_TVE_R1 = 0x00002002;
                        //Joie JPEG public value Base on MPEG 20051109 Public value
                        //_ProgTVEGain(SET_TVE_GAIN_YC, 0x00473e57, nGainOffset); //original
                        // Y2YG: 0x8C, CB2PBB: 0x7F, CR2PRR: 0x87
                        //_ProgTVEGain(SET_TVE_GAIN_CAV, (0x8C << 24) | (0x7F << 12) | 0x87, nGainOffset); //original
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4c4c4764, nGainOffset); //Joie 200610 for CT909R
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xa9088091, nGainOffset); //Joie 200610 for CT909R
                        break;

                    case VIDEOOUTPUT_AV_RGB:
                        dwREG_TVE_R0 = 0x02402080;
                        REG_TVE_R1 = 0x00002004;
                        //Joie JPEG public value Base on MPEG 20051109 Public value
                        //_ProgTVEGain(SET_TVE_GAIN_YC, 0x00483e57, nGainOffset); //original
                        // Y2YG: 0x8C, CB2PBB: 0x128, CR2PRR: 0xC5
                        //_ProgTVEGain(SET_TVE_GAIN_CAV, (0x8C << 24) | (0x128 << 12) | 0xC5, nGainOffset); //original
                        //REG_TVE_GAIN_G = 0x0000345e; //original
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x4c4c4764, nGainOffset); //Joie 200610 for CT909R
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xaa0fc0c5, nGainOffset); //Joie 200610 for CT909R
                        REG_TVE_GAIN_G = 0x00003068; //Joie 200610 for CT909R

                        break;
                }
                break;

            case TVSYSTEM_PALM:
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_AV_SVIDEO:
                        dwREG_TVE_R0 = 0x02002040;
                        REG_TVE_R1 = 0x00002000;
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x00464866, nGainOffset);
                        break;

                    case VIDEOOUTPUT_AV_YUV:
                        dwREG_TVE_R0 = 0x02002040;
                        REG_TVE_R1 = 0x00002002;
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x0045435e, nGainOffset);
                        // Y2YG: 0x8A, CB2PBB: 0x77, CR2PRR: 0x9F
                        _ProgTVEGain(SET_TVE_GAIN_CAV, (0x8A << 24) | (0x77 << 12) | 0x9F, nGainOffset);
                        break;

                    case VIDEOOUTPUT_AV_RGB:
                        dwREG_TVE_R0 = 0x02402040;
                        REG_TVE_R1 = 0x00002004;
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x0045435e, nGainOffset);
                        // Y2YG: 0x87, CB2PBB: 0xF6, CR2PRR: 0xB6
                        _ProgTVEGain(SET_TVE_GAIN_CAV, (0x87 << 24) | (0xF6 << 12) | 0xB6, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003064;
                        break;
                }
                break;

            case TVSYSTEM_PALN:
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_AV_SVIDEO:
                        dwREG_TVE_R0 = 0x020020c0;
                        REG_TVE_R1 = 0x00002000;
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x00464866, nGainOffset);
                        break;

                    case VIDEOOUTPUT_AV_YUV:
                        dwREG_TVE_R0 = 0x020020c0;
                        REG_TVE_R1 = 0x00002002;
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x0045435e, nGainOffset);
                        // Y2YG: 0x8A, CB2PBB: 0x77, CR2PRR: 0x9F
                        _ProgTVEGain(SET_TVE_GAIN_CAV, (0x8A << 24) | (0x77 << 12) | 0x9F, nGainOffset);
                        break;

                    case VIDEOOUTPUT_AV_RGB:
                        dwREG_TVE_R0 = 0x024020c0;
                        REG_TVE_R1 = 0x00002004;
                        _ProgTVEGain(SET_TVE_GAIN_YC, 0x0045435e, nGainOffset);
                        // Y2YG: 0x87, CB2PBB: 0xF6, CR2PRR: 0xB6
                        _ProgTVEGain(SET_TVE_GAIN_CAV, (0x87 << 24) | (0xF6 << 12) | 0xB6, nGainOffset);
                        REG_TVE_GAIN_G = 0x00003064;
                        break;
                }
                break;
        }

        REG_TVE_GAIN_YC |= 0x40000000;

    }
    else //PScan  for  JPEG
    {
#ifndef NO_PSCAN_OUTPUT
        switch (bTVSystem)
        {
            case TVSYSTEM_NTSC:
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_YUV_PSCAN:
#ifdef SUPPORT_75IRE
                        dwREG_TVE_R0 = 0x00000020; //7.5 IRE
#else
                        dwREG_TVE_R0 = 0x00000000;
#endif
                        //Kevin1.07a, disable 525p MV
                        REG_TVE_R1 = 0x0000a002;
                        // Y2YG: 0x8B, CB2PBB: 0x8D, CR2PRR: 0x85
                        //_ProgTVEGain(SET_TVE_GAIN_CAV, (0x8B << 24) | (0x8D << 12) | 0x85, nGainOffset); //original
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xad087091, nGainOffset); //Joie 200610 for CT909R
                        break;

                    case VIDEOOUTPUT_RGB_PSCAN:
#ifdef SUPPORT_75IRE
                        dwREG_TVE_R0 = 0x00400020; //7.5 IRE
#else
                        dwREG_TVE_R0 = 0x00400000;
#endif
                        //Kevin1.07a, disable 525p MV
                        REG_TVE_R1 = 0x0000a004;
                        // Y2YG: 0x8D, CB2PBB: 0x120, CR2PRR: 0xC6
                        //_ProgTVEGain(SET_TVE_GAIN_CAV, (0x8D << 24) | (0x120 << 12) | 0xC6, nGainOffset); //original
                        //REG_TVE_GAIN_G = 0x00003064; //original
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xaa0f80ce, nGainOffset); //Joie 200610 for CT909R
                        REG_TVE_GAIN_G = 0x00003068; //Joie 200610 for CT909R
                        break;

                    case VIDEOOUTPUT_VGA_PSCAN:
                        //Kevin0.87, Hsync/Vsync signal for VGA
	                    dwTVETemp = (1<<23); //Kevin1.07a, disable 525p MV
                        dwREG_TVE_R0 = 0x02000020;
                        REG_TVE_R1 = 0x3000a004;
                        // Y2YG: 0x8D, CB2PBB: 0x120, CR2PRR: 0xC6
                        //_ProgTVEGain(SET_TVE_GAIN_CAV, (0x8D << 24) | (0x120 << 12) | 0xC6, nGainOffset); //original
                        //REG_TVE_GAIN_G = 0x00003064; //original
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xaa0f80ce, nGainOffset); //Joie 200610 CT909R not support VGA output
                        REG_TVE_GAIN_G = 0x00003068; //Joie 200610 CT909R not support VGA output
                        break;
                }
                break;

            case TVSYSTEM_PAL:
            case TVSYSTEM_PALM:
            case TVSYSTEM_PALN:
                switch (bVideoOutput)
                {
                    case VIDEOOUTPUT_YUV_PSCAN:
                        // LLY2.60, porting Chuan's code to fix PSCAN @ PAL/PALN mode incorrect issue
                        // ie. must enable bit[7:6]=10b
                        // Otherwise, can't detect CGMS signal
                        dwREG_TVE_R0 = 0x00000000 + (0x2L<<6);
                        REG_TVE_R1 = 0x0000f002;
                        // Y2YG: 0x8C, CB2PBB: 0x8D, CR2PRR: 0x7F
                        //_ProgTVEGain(SET_TVE_GAIN_CAV, (0x8C << 24) | (0x8D << 12) | 0x7F, nGainOffset); //original
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xaa087090, nGainOffset); //Joie 200610 for CT909R
                        break;

                    case VIDEOOUTPUT_RGB_PSCAN:
                        // LLY2.60, porting Chuan's code to fix PSCAN @ PAL/PALN mode incorrect issue
                        // ie. must enable bit[7:6]=10b
                        // Otherwise, can't detect CGMS signal
                        dwREG_TVE_R0 = 0x00400000 + (0x2L<<6);
                        REG_TVE_R1 = 0x0000f004;
                        // Y2YG: 0x8D, CB2PBB: 0x120, CR2PRR: 0xC6
                        //_ProgTVEGain(SET_TVE_GAIN_CAV, (0x8D << 24) | (0x120 << 12) | 0xC6, nGainOffset); //original
                        //REG_TVE_GAIN_G = 0x00003064; //original
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xaa0fc0c5, nGainOffset); //Joie 200610 for CT909R
                        REG_TVE_GAIN_G = 0x00003068; //Joie 200610 for CT909R
                        break;

                    case VIDEOOUTPUT_VGA_PSCAN:
                        //Kevin0.87, Hsync/Vsync signal for VGA
		                dwTVETemp = (1<<23);
                        dwREG_TVE_R0 = 0x02000080;
                        REG_TVE_R1 = 0x3000e004;
                        // Y2YG: 0x8D, CB2PBB: 0x120, CR2PRR: 0xC6
                        //_ProgTVEGain(SET_TVE_GAIN_CAV, (0x8D << 24) | (0x120 << 12) | 0xC6, nGainOffset); //original
                        //REG_TVE_GAIN_G = 0x00003064; //original
                        _ProgTVEGain(SET_TVE_GAIN_CAV, 0xaa0fc0c5, nGainOffset); //Joie 200610 CT909R not support VGA output
                        REG_TVE_GAIN_G = 0x00003068; //Joie 200610 CT909R not support VGA output
                        break;
                }
                break;
        }
#else   // #ifndef NO_PSCAN_OUTPUT
        DBG_Printf(DBG_THREAD_CHEERDVD, DBG_INFO_PRINTF, "Err: don't support PSCAN\n");
#endif  // #ifndef NO_PSCAN_OUTPUT
    }
} //if (!__bTVE_JPEG)

    // LLY2.53, don't affect REG_TVE_R0[13:8] value
    // Otherwise, we will see 灰色直條紋 phenomenon.
    REG_TVE_R0 = dwREG_TVE_R0 | (REG_TVE_R0 & (0x1F00));

    if (_blDisableSVideo)
    {
        REG_TVE_R1 |= 0x00000700; // CVBS0 on, and Y/Pb/Pr is off
    }

    // LLY2.77, porting Chuan's code to always power down DD/ DC/ DY DAC @ Digital Panel Mode
    // it will power down all TV ouput
    if (_bForce_TVOut_Off)
    {
#ifdef CT909P_IC_SYSTEM
        REG_PLAT_DAC_MODE_SEL |= (
#ifdef SUPPORT_TFT_DIGITAL_MODE
        PLAT_POWER_DOWN_DD | PLAT_POWER_DOWN_DC | PLAT_POWER_DOWN_DY |
#endif  // #ifdef SUPPORT_TFT_DIGITAL_MODE
        PLAT_POWER_DOWN_CVBS);
//        MACRO_IDLE();
//        REG_PLAT_DAC_MODE_SEL |= (PLAT_POWER_DOWN_ALL | PLAT_POWER_DOWN_HIV);
#endif
    }
#ifdef CT909P_IC_SYSTEM
    else
    {
        REG_PLAT_DAC_MODE_SEL &= ~(PLAT_POWER_DOWN_ALL | PLAT_POWER_DOWN_HIV |
#ifndef SUPPORT_TFT_DIGITAL_MODE
        PLAT_POWER_DOWN_DD | PLAT_POWER_DOWN_DC | PLAT_POWER_DOWN_DY |
#endif  // #ifndef SUPPORT_TFT_DIGITAL_MODE
        PLAT_POWER_DOWN_CVBS);
    }
#endif

    return dwTVETemp;
}

//**********************************************************************************
// Description  :   Disable the SVideo output
// Arguments    :
// Return       :   None
// Notice       :
//*********************************************************************************
void HAL_DisableSVideo(BOOL blDisable)
{
    _blDisableSVideo = blDisable;
}


//**********************************************************************************
// Description  :   Force turn-off TV output off
//                  ie. it will turn off all TV DAC even call HAL_SetTVEncoder()
// Arguments    :   bFlag, indicate to force disable all TV output or not
//                  TRUE, force off display all TV output
//                  FALSE, turn on all TV output
// Return       :   None
// Notice       :
//*********************************************************************************
void HAL_Force_TVOut_Off(BYTE bFlag)
{
    _bForce_TVOut_Off = bFlag;
}


//  Alan2.35, support the chroma adjustment
//  *******************************************************************************
//  Function    :   _ProgTVEGain
//  Description :   adjust chrominance phase by calculating new Cb & Cr and write reg. REG_TVE_GAIN_YC or REG_TVE_GAIN_CAV
//  Arguments   :   bSetID: SET_TVE_GAIN_YC or SET_TVE_GAIN_CAV
//                  dwLo: Register value
//                  nGainOffset: Gain offset
//                  __SetupInfo.bChromPhaseOffset: chrominance offset
//  Return      :   None
//  Side Effect :   None
//  *******************************************************************************
void _ProgTVEGain(SET_REG_ID bSetID, DWORD dwRegValue, int nGainOffset)
{
#ifndef SUPPORT_ADJUST_CHROM_PHASE
    if (SET_TVE_GAIN_YC == bSetID)
    {
        REG_TVE_GAIN_YC = dwRegValue;
    }
    else if (SET_TVE_GAIN_CAV == bSetID)
    {
        REG_TVE_GAIN_CAV = dwRegValue;
    }
#else
// Cr
// |   Vc
// |  /
// | /   theta
// |/___________Cb
//  Chrominance amplitude: Vc=sqrt(Cb^2+Cr^2)
//  Chrominance phase: theta=atan(Cr/Cb)
//  Adjust theta while Vc remains unchanged : Cb^2+Cb^2=Cb'^2+Cr'^2
//      - CVBS & S-Video: adjust Cb/Cr in reg. REG_TVE_GAIN_YC
//      - YUV: adjust Cb/Cr in reg. REG_TVE_GAIN_CAV
//      - RGB: adjust Cb/Cr in reg. REG_TVE_GAIN_CAV

//below definitions are adjustable
#define CHROM_PHASE_GAP_YC  4
#define CHROM_PHASE_GAP_CAV 8

    DWORD dwSquare, dwCrSquare;
    DWORD dwIndex;
    DWORD dwCbValue, dwCrValue, dwAdjustOffset;


    if (SET_TVE_GAIN_YC == bSetID)
    {
        dwCbValue = (dwRegValue & 0x7F00) >> 8; // default Cb [14:8]
        dwCrValue = dwRegValue & 0x007F;        // default Cr [6:0]
#ifdef SUPPORT_PRINTF
        printf("REG_TVE_GAIN_YC: org Cb=%lx Cr=%lx\n", dwCbValue, dwCrValue);
#endif
        dwSquare = (DWORD)dwCbValue * dwCbValue + (DWORD)dwCrValue * dwCrValue; //Cb^2 + Cr^2

        //SETUP adjusts Cr'
        if (0 < nGainOffset)
        {
            dwAdjustOffset = nGainOffset * CHROM_PHASE_GAP_YC;
            if ((WORD)dwCrValue + dwAdjustOffset < 0x7F)
                dwCrValue += dwAdjustOffset; //Cr'
            else
                dwCrValue = 0x7F; //Cr'
        }
        else if (0 > nGainOffset)
        {
            dwAdjustOffset = (-nGainOffset) * CHROM_PHASE_GAP_YC;
            if (dwCrValue > dwAdjustOffset)
                dwCrValue -= dwAdjustOffset; //Cr'
            else
                dwCrValue = 0; //Cr'
        }

        dwCrSquare = (DWORD)dwCrValue * dwCrValue;    // Cr'^2

        if (dwSquare > dwCrSquare)
            dwSquare -= dwCrSquare;
        else
            dwSquare = 0;

        //calculate Cb'=sqrt(Cb^2+Cb^2-Cr'^2) by traversing
        for (dwAdjustOffset = 0; dwAdjustOffset <= 0x7F; dwAdjustOffset++)
        {
	        if ( ((DWORD)dwAdjustOffset * dwAdjustOffset) >= dwSquare)
		        break;
        }

        dwCbValue = dwAdjustOffset; //Cb'
#ifdef SUPPORT_PRINTF
        printf("REG_TVE_GAIN_YC: new Cb=%lx Cr=%lx\n", dwCbValue, dwCrValue);
#endif
        dwRegValue &= 0xFFFF0000;
        dwRegValue |= ((WORD) dwCbValue) << 8 | dwCrValue;
        REG_TVE_GAIN_YC = dwRegValue;
    }
    else if (SET_TVE_GAIN_CAV == bSetID)
    {
        dwCbValue = (dwRegValue & 0x10000000) >> 20 | (dwRegValue & 0x0000FF00) >> 8; //default Cb [28][15:8]
        dwCrValue = (dwRegValue & 0x0100) >> 16 | (dwRegValue & 0x000000FF);        //default Cr [24][7:0]
#ifdef SUPPORT_PRINTF
        printf("REG_TVE_GAIN_CAV : org Cb=%lx Cr=%lx\n", dwCbValue, dwCrValue);
#endif
        dwSquare = (DWORD)dwCbValue * dwCbValue + (DWORD)dwCrValue * dwCrValue; //default Cb^2 + Cr^2

        //SETUP adjusts Cr'
        if (0 < nGainOffset)
        {
            dwAdjustOffset = nGainOffset * CHROM_PHASE_GAP_CAV;
            if (dwCrValue + dwAdjustOffset < 0x1FF)
                dwCrValue += dwAdjustOffset; //Cr'
            else
                dwCrValue = 0x1FF; //Cr'
        }
        else if (0 > nGainOffset)
        {
            dwAdjustOffset = (-nGainOffset) * CHROM_PHASE_GAP_CAV;
            if (dwCrValue > dwAdjustOffset)
                dwCrValue -= dwAdjustOffset; //Cr'
            else
                dwCrValue = 0; //Cr'
        }

        dwCrSquare = (DWORD)dwCrValue * dwCrValue;
        if (dwSquare > dwCrSquare)
            dwSquare -= dwCrSquare;
        else
            dwSquare = 0;

        //calculate Cb'=sqrt(Cb^2+Cb^2-Cr'^2) by traversing
        for (dwIndex = 0; dwIndex <= 0x1FF; dwIndex++)
        {
	        if ( ((DWORD)dwIndex * dwIndex) >= dwSquare)
		        break;
        }
        dwCbValue = dwIndex; //Cb'
#ifdef SUPPORT_PRINTF
        printf("REG_TVE_GAIN_CAV : new Cb=%lx Cr=%lx\n", dwCbValue, dwCrValue);
#endif
        dwRegValue &= ~0x1100FFFF;
        dwRegValue |= (dwCbValue & 0x00000100) << 20 | (dwCrValue & 0x00000100) << 16;
        dwRegValue |= (dwCbValue & 0x000000FF) << 8 | (dwCrValue & 0x000000FF);
        REG_TVE_GAIN_CAV = dwRegValue;
    }
#endif  // #ifdef SUPPORT_ADJUST_CHROM_PHASE
} //end of _ProgTVEGain()


