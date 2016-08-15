#include "winav.h"
//#define     FLASH_VERSION       1316L
#ifndef NO_SERIAL_FLASH
    #ifdef AUTO_UPGRADE
        #include <stdio.h>
        #include "comdef.h"
    #else
        #include "ctkav.h "
    #endif
#endif
        #include "ctkav.h "
        
DWORD ReadStatus_1(void);
#include "SPFLASH.H"
#define WAIT_TIME  50
#define HLCHANG(I) (I << 24) | ((I & 0xff00) << 8) | ((I & 0xff0000) >> 8)
//#define TIME_COUNT  (REG_PLAT_TIMER3_VALUE / 90)
//#define SUPPORT_STB
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
BYTE bRegistOffset = 1;
#else
BYTE bRegistOffset = 0;
#endif 
tSCLK_909P *pSCLK_909P;
tSCLK_909S *pSCLK_909S;
tREAD_TYPE_909S *pREAD_TYPE_909S;
tPROM_CONFIG *pPROM_CONFIG;
DWORD __dwSCLK,__dwREAD_TYPE;
#ifdef SUPPORT_VSYNCPOLL
VOID    _VSYNCPolling(VOID)
{
    extern DWORD __dwREG_MCU_VCR20_Bak;
    extern DWORD __dwREG_MCU_VCR25_Bak;
    extern DWORD __dwREG_DISP_VCLIP_CR_Bak;
    extern DWORD __dwREG_DISP_VSCALE_CR_Bak;
    if ((REG_PLAT_PROC1_1ST_INT_PENDING & INT_PROC1_1ST_VSYNC) &&
        ((REG_DISP_MEM_LINE & 0xFFFF) == 0))
    {
        DWORD   dwREG_MCU_VCR20_Bak;
        DWORD   dwREG_MCU_VCR25_Bak;
        DWORD   dwREG_DISP_VCLIP_CR_Bak;
        DWORD   dwREG_DISP_VSCALE_CR_Bak;
        
        dwREG_MCU_VCR20_Bak     = REG_MCU_VCR20;
        dwREG_MCU_VCR25_Bak     = REG_MCU_VCR25;

        REG_MCU_VCR20           = __dwREG_MCU_VCR20_Bak;
        REG_MCU_VCR25           = __dwREG_MCU_VCR25_Bak;
        __dwREG_MCU_VCR20_Bak   = dwREG_MCU_VCR20_Bak;
        __dwREG_MCU_VCR25_Bak   = dwREG_MCU_VCR25_Bak;

        dwREG_DISP_VCLIP_CR_Bak     = REG_DISP_VCLIP_CR;
        dwREG_DISP_VSCALE_CR_Bak    = REG_DISP_VSCALE_CR;

        REG_DISP_VCLIP_CR           = __dwREG_DISP_VCLIP_CR_Bak;
        REG_DISP_VSCALE_CR          = __dwREG_DISP_VSCALE_CR_Bak;
        __dwREG_DISP_VCLIP_CR_Bak   = dwREG_DISP_VCLIP_CR_Bak;
        __dwREG_DISP_VSCALE_CR_Bak  = dwREG_DISP_VSCALE_CR_Bak;
        
        REG_PLAT_PROC1_1ST_INT_CLEAR = INT_PROC1_1ST_VSYNC;
    }
}
#endif
//--------------------------------------------------------------------------
// Function    :  SPF_MPLLSet
// Description :  set MCLK and check  SPF phase
// Input       :  CPU speed ex:27Mhz (input:27000000);
//--------------------------------------------------------------------------
DWORD SPF_MPLLSet(DWORD dwCPUSpeed)
{
    DWORD dwCLK, dwMPLL=0;
    DWORD dwTemp;
    DWORD dwSaveInt;
    DWORD dwCT909_MODE;
#ifndef NO_SERIAL_FLASH 
    DWORD dwCLKBackup, dwMPLLBackup;    
    DWORD dwCheckvalue;    
    DWORD dwPROMConfig;
    
    dwCheckvalue = *(DWORD *)0x0;
    dwPROMConfig = *(DWORD *)(*(DWORD *)0x8);

    dwCLKBackup = REG_PLAT_CLK_FREQ_CONTROL1;
    dwMPLLBackup = REG_PLAT_MPLL_CONTROL;
#endif

    dwCLK = REG_PLAT_CLK_FREQ_CONTROL1;
    dwCLK &= ~((1 << 25) + (1 << 15));

    if (dwCPUSpeed == CPU_27M)
    {
        dwMPLL = REG_PLAT_MPLL_CONTROL ;
        dwCLK = REG_PLAT_CLK_FREQ_CONTROL1 & ~(1 << 15);
    }
    else
    {
        dwCT909_MODE=((REG_PLAT_SYSTEM_CONFIGURATION2 & 0xFF000000) >> 16); //get system 
        dwCT909_MODE = dwCT909_MODE &0xFF00;
#ifdef CT909R_IC_SYSTEM
        if(dwCT909_MODE == 0xb000)//VERSION_S)
        {
            dwMPLL = (REG_PLAT_MPLL_CONTROL & ~(0x1FFFFF)) +
                        (0 << 20) + (0 << 18) + (16 << 11);

            if ((dwCPUSpeed <= CPU_146M) && (dwCPUSpeed > CPU_54M))
            {
                dwMPLL += ((dwCPUSpeed / 1000000) - 2);
            }
            else if (dwCPUSpeed == CPU_54M)
            {
                dwMPLL = (REG_PLAT_MPLL_CONTROL & ~(0x1FFFFF)) +
                    (0 << 20) + (1 << 18) + (7 << 11);
                // PD = 0, PF = 1, MF = 7, NF = 52 : Fout = 162: 54 MHz
                dwMPLL += 52;
            }
            else
            {
                #ifdef SPF_DEBUG_INFO  
                printf("Err: Not defined MPLL clock\n");
                #endif
                return FALSE;
            }
        }
#endif  // #ifdef CT909R_IC_SYSTEM

#if defined(CT909P_IC_SYSTEM)||defined(CT909G_IC_SYSTEM)
        if(dwCT909_MODE == 0x3000|| dwCT909_MODE == 0xD000) //VERSION_P//VERSION_G)
        {
            if(dwCPUSpeed == CPU_133M)
            {

                dwMPLL = (2 << 5) + 27;      // N = 27, M = 2 : 132.75 MHz
            }
            else if(dwCPUSpeed == CPU_146M)
            {
                
                dwMPLL = (1 << 5) + 22;      // N = 22, M = 1 : 145.8 MHz
            }
            else if(dwCPUSpeed == CPU_142M)
            {
                dwMPLL = (2 << 5) + 31;      // N = 31, M = 2 : 141.75 MHz          
            }
            else if(dwCPUSpeed == CPU_120M)
            {

                dwMPLL = (2 << 5) + 21;      // N = 21, M = 2 : 119.25 MHz
            }
            else if(dwCPUSpeed == CPU_100M)
            {
                dwMPLL = (1 << 5) + 5;       // N = 5, M = 1 : 99.9 MHz
            }
            /*
            else if(dwCPUSpeed == CPU_54M)
            {
                dwMPLL = (2 << 5) + 27;      // N = 0, M = 3 : 61.7 MHz
            }
            */
            else
            {
                #ifdef SPF_DEBUG_INFO  
                printf("Err: Not defined MPLL clock\n");
                #endif
                return FALSE;
            }
            dwMPLL += (REG_PLAT_MPLL_CONTROL & ~(0xFF));
        }
#endif  // #ifdef CT909P_IC_SYSTEM
        
        dwCLK |= (1 << 25) + (1 << 15);

    }


    HAL_DISABLE_INTERRUPTS(dwSaveInt);  
    
    REG_PLAT_MPLL_CONTROL = dwMPLL;
    for(dwTemp = 0; dwTemp < 10000; dwTemp++) // about 1.5 msec
    {
        MACRO_IDLE();
    }
    REG_PLAT_CLK_FREQ_CONTROL1 = dwCLK;


#ifdef NO_SERIAL_FLASH    
    dwTemp = TRUE;
#else
//    dwTemp = SPF_ReadTest(dwCPUSpeed, dwPROMConfig, dwCheckvalue);
    if(dwPROMConfig != 0xffffffff)  
    { // check have setting 
        #ifdef CT909R_IC_SYSTEM    
        if(dwCPUSpeed == CPU_27M)
        {//use defaule
            REG_PLAT_SPI_READ_TYPE = 0;
            REG_PLAT_SPI_SCLK_CTRL = 0x03131 | 0x40000;
        }
        else
        {
            REG_PLAT_SPI_READ_TYPE = (dwPROMConfig>>20)&0X07;
            REG_PLAT_SPI_SCLK_CTRL = (dwPROMConfig&0x3FFFF)+0x40000;  
        }
        #endif

        #if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
        if(dwCPUSpeed == CPU_27M)
        {  // Use defaule
            REG_PLAT_SPI_SCLK_CTRL = 0x03131 | 0x80000;
        }
        else
        {
            REG_PLAT_SPI_SCLK_CTRL = ((dwPROMConfig&0x00f00000)<<4)+(dwPROMConfig&0x7FFFF)+0x80000;  
        }
        #endif
    }   
    
    if(SPF_TestStable(dwCheckvalue) == FALSE)
    {
        REG_PLAT_CLK_FREQ_CONTROL1 = dwCLKBackup;
        REG_PLAT_MPLL_CONTROL = dwMPLLBackup;    
        SPF_TestStable(dwCheckvalue); //wait stable
        dwTemp = FALSE;
    }
    else
    {
        dwTemp = TRUE;    
    }
#endif
    HAL_RESTORE_INTERRUPTS(dwSaveInt);
    return dwTemp;
}


//--------------------------------------------------------------------------
// Function    :  SPF_ReadTest
// Description :  Use MCLK,test SPF phase and feq setting
// Input       :  
//            dwCPUSpeed :CPU speed ex:27Mhz (input:27000000);
//            dwCheckvalue :????
//--------------------------------------------------------------------------
DWORD SPF_TestStable(DWORD dwCheckvalue)
{
    DWORD dwVerify = 0;
    DWORD dwTemp;
    for(dwTemp = 0; dwTemp < 10000; dwTemp++) // about 1.5 msec
    {
        DWORD dwValue;
        MACRO_IDLE();
        asm volatile ("lda [%1] 0x7, %0;" : "=r" (dwValue) : "r" (0x0));
        if (dwCheckvalue == dwValue)
        {
            if (dwVerify++ > 1000)
            { //check clock is stable
                return TRUE;
                break;          
            }
        }
        else
        {  
            dwVerify = 0;
        }
    }
    return FALSE;    
}

//--------------------------------------------------------------------------
// Function    :  SPF_ReadTest
// Description :  Use MCLK,test SPF phase and feq setting
// Input       :  
//            dwCPUSpeed :CPU speed ex:27Mhz (input:27000000);
//            dwCheckvalue :????
//--------------------------------------------------------------------------
#ifndef NO_SERIAL_FLASH
#ifdef USE_READID
WORD SPF_ReadID(BYTE bFlashSize)
{
    WORD wTemp;
    #ifdef  FLASH_INT_OFF
    DWORD dwSaveInt;
   HAL_DISABLE_INTERRUPTS(dwSaveInt);  
   #endif   
    wTemp = _SPF_ReadID( bFlashSize);
    #ifdef  FLASH_INT_OFF 
   HAL_RESTORE_INTERRUPTS(dwSaveInt);
   #endif    
   return wTemp;
}
//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
//--------------------------------------------------------------------------

WORD _SPF_ReadID(BYTE bFlashSize)
{
    WORD ID = 0;
    if(ID ==0)
    {
        RD_REG = 0;
        SPFcommand( 0x0000009F //read ID
                    ,Format_Device_ID1
                    ,WaitCMD_STATE
                    ,WAIT_TIME);
        ID = RD_REG;
#ifdef SPF_DEBUG_INFO
        //printf("ManufacturerID = 0x%02lx, DeviceID = 0x%02lx\n", ID >> 8, ID & 0xff);
#endif
        switch(ID)
        {//have support  IC
          case AT26F004:
          case AT26DF081A:
          case AT26DF161:
          case AT26DF321:
          case A25L80P:
          case ES25P16:
             break;  
          default :
              ID =0;
           break;      
        }
     }

    if(ID ==0)
    {

         SPFcommand( 0x00000090 //read ID
                     ,Format_Device_ID
                     ,WaitCMD_STATE
                     ,WAIT_TIME);
        ID = RD_REG & 0xffff;
#ifdef SPF_DEBUG_INFO                 
        //printf("ManufacturerID = 0x%02lx, DeviceID = 0x%02lx\n", ID >> 8, ID & 0xff);
#endif
        switch(ID)
        {//have support  IC
          case MX25L3205:
          case MX25L1605:
          case MX25L8005:
          case SST25LF080A:     
          case SST25VF040B:
          case SST25VF080B:
          case SST25VF016B:        
          case W25X32:
          case W25X16:
          case W25X80:        
          case W25X40:
          case MX25L4005:
          case SST25LF040A:         
          case EN25B40B:   
          case EN25B40T:
          case EN25B80:
          case EN25B80T:                        
          case EN25B16:        
          case EN25B16T:        
          case W25D40T:
          case W25D40B:
          case EN25P40:
          case F25L008A:  
          case PM25LV016: 
          case PM25LV080:                 
          break;  

          default :
              ID =0;
           break;      
        }
     }

    if(ID ==0)
    {
        SPFcommand( 0x000000AB //read ID
                    ,Format_Device_ID
                    ,WaitCMD_STATE
                    ,WAIT_TIME);
        ID = RD_REG & 0xffff;
#ifdef SPF_DEBUG_INFO             
        //printf("ManufacturerID = 0x%02lx, DeviceID = 0x%02lx\n", ID >> 8, ID & 0xff);
#endif
        switch(ID)
        {//have support  IC
          case PM25LV040:
             break;  
          default :
              ID =0;
           break;      
        }
    }
    
    if(ID ==0)
    { 
        
        SPFcommand( 0x0000009F //read manufacture ID 
                    ,CMD+IO_R+OPL_D1_D2
                    ,WaitCMD_STATE
                    ,WAIT_TIME);
        ID = (RD_REG<<8) & 0xff00;
        SPFcommand( 0x0000009F //read capacity ID
                    ,CMD+IO_R+OPL_D1_D2_D3_D4
                    ,WaitCMD_STATE
                    ,WAIT_TIME);
        ID +=RD_REG&0x00ff;


#ifdef SPF_DEBUG_INFO             
        //printf("ManufacturerID = 0x%02lx, DeviceID = 0x%02lx\n", ID >> 8, ID & 0xff);
#endif

        switch(ID)
        {
           case A25L80P:
           case A25L40P:
           case A25L16PT://Add by PeterYu, 2007/3/13 02:51下午
           case A25L16PB:
           break; 
           default:
           ID =0;
           break;
        }
    }
     
     
#ifdef SPF_DEBUG_INFO         
    //printf("ManufacturerID = 0x%02lx, DeviceID = 0x%02lx\n", ID >> 8, ID & 0xff);
#endif
    switch(ID)
    {
//**********32M Flash**************************     
      case AT26DF321:
      case MX25L3205:        
      case W25X32:
           SPF_INFO1.bSize=FLASH_SIZE_32M;              
        break;
//**********16M Flash**************************     
      case AT26DF161:
      case MX25L1605 :
      case W25X16:
      case ES25P16:
      case EN25B16:        
      case EN25B16T:  
      case SST25VF016B:        
      case PM25LV016: 
      case A25L16PT://Add by PeterYu, 2007/3/13 02:51下午  
      case A25L16PB:
           SPF_INFO1.bSize=FLASH_SIZE_16M;    
      break;
 
 //**********8M Flash**************************     
      case A25L80P:        
      case AT26DF081A:          
      case EN25B80:
      case EN25B80T:  
      case MX25L8005:
      case SST25LF080A:   
      case SST25VF080B:        
      case W25X80:        
      case F25L008A:
      case PM25LV080:        
         SPF_INFO1.bSize=FLASH_SIZE_8M;
      break;

 //**********4M Flash**************************     
      case AT26F004:
      case MX25L4005:
      case SST25LF040A:         
      case SST25VF040B:         
      case EN25B40B:   
      case EN25B40T:
      case W25D40T:
      case W25D40B:
      case EN25P40:
      case PM25LV040:
      case W25X40:        
      SPF_INFO1.bSize=FLASH_SIZE_4M;
      break;  
 //**********************************************     
      default :
         SPF_INFO1.bSize = FALSE;
       break;      
    }
    
    switch(ID)
    {
      case MX25L8005:
      case MX25L4005:
      case MX25L1605: 
      case MX25L3205:
      case SST25VF016B:        
      case SST25LF080A:   
      case SST25VF080B:        
      case SST25LF040A:  
      case SST25VF040B:  
      case F25L008A:        
         SPF_INFO1.bWriteEnable =0x06;
         SPF_INFO1.bWriteDisable =0x04;
         SPF_INFO1.bRead_Status =0x05;
         SPF_INFO1.bWrite_Status =0x01;
         SPF_INFO1.bReadData =0x03;
         SPF_INFO1.bFastRead =0x0b;
         SPF_INFO1.bPageProgram =0x02;
         SPF_INFO1.bSectorErase =0x20;// min erase function
         SPF_INFO1.bBulkErase =0x60;//max erase function
         SPF_INFO1.bPowerDown =0xb9;
         SPF_INFO1.bRePowerDown =0xAB;
         break;

     case A25L80P:        
     case EN25B40B:   
     case EN25B40T:
     case EN25B80:
     case EN25B80T: 
     case EN25B16:        
     case EN25B16T:        
     case W25D40T:
     case W25D40B:   
     case EN25P40:  
     case ES25P16:
     case A25L16PT://Add by PeterYu, 2007/3/13 02:51下午 
     case A25L16PB:  
         SPF_INFO1.bWriteEnable =0x06;
         SPF_INFO1.bWriteDisable =0x04;
         SPF_INFO1.bRead_Status =0x05;
         SPF_INFO1.bWrite_Status =0x01;
         SPF_INFO1.bReadData =0x03;
         SPF_INFO1.bFastRead =0x0b;
         SPF_INFO1.bPageProgram =0x02;
         SPF_INFO1.bSectorErase =0xD8;// min erase function
         SPF_INFO1.bBulkErase =0xc7;//max erase function
         SPF_INFO1.bPowerDown =0xB9;
         SPF_INFO1.bRePowerDown =0xAB;
          break;         

      case PM25LV040:    
      case PM25LV016: 
      case PM25LV080:        
         SPF_INFO1.bWriteEnable =0x06;
         SPF_INFO1.bWriteDisable =0x04;
         SPF_INFO1.bRead_Status =0x05;
         SPF_INFO1.bWrite_Status =0x01;
         SPF_INFO1.bReadData =0x03;
         SPF_INFO1.bFastRead =0x0b;
         SPF_INFO1.bPageProgram =0x02;
         SPF_INFO1.bSectorErase =0xD7;// min erase function
         SPF_INFO1.bBulkErase =0xC7;//max erase function
         SPF_INFO1.bPowerDown =0x00; //no power down
         SPF_INFO1.bRePowerDown =0x00;
          break;
          
     case W25X32:
     case W25X16:
     case W25X80:   
     case W25X40:        
     case AT26F004:
     case AT26DF081A:      
     case AT26DF161:
     case AT26DF321:        
         SPF_INFO1.bWriteEnable =0x06;
         SPF_INFO1.bWriteDisable =0x04;
         SPF_INFO1.bRead_Status =0x05;
         SPF_INFO1.bWrite_Status =0x01;
         SPF_INFO1.bReadData =0x03;
         SPF_INFO1.bFastRead =0x0b;
         SPF_INFO1.bPageProgram =0x02;
         SPF_INFO1.bSectorErase =0x20;// min erase function
         SPF_INFO1.bBulkErase =0xC7;//max erase function
         SPF_INFO1.bPowerDown =0xB9;
         SPF_INFO1.bRePowerDown =0xAB;
          break;

      default :

            ID = 0;
         break;
     }
    
    switch(ID)
    {
 //**********Botten BOOT**************************         
      case A25L80P:        
      case EN25B40B: //botton type
      case EN25B80:
      case EN25B16:
      case W25D40B:      
      case A25L16PB://Add by PeterYu, 2007/3/13 02:51下午    
         SPF_INFO1.bBoot = BOOT_BOTTOM;
      break;  
 //**********TOP BOOT**************************         
     case W25D40T:
     case EN25B40T:     
     case EN25B80T:        
     case EN25B16T:
     case A25L16PT://Add by PeterYu, 2007/3/13 02:51下午 
         SPF_INFO1.bBoot = BOOT_TOP;
        break;
 //**********ALL BOOT**************************         
      case AT26F004:
      case AT26DF081A:
      case AT26DF161:
      case AT26DF321:   
      case SST25VF080B:        
      case SST25LF080A:           
      case SST25LF040A:  
      case SST25VF040B: 
      case SST25VF016B:
      case MX25L8005:
      case MX25L4005:
      case MX25L1605:  
      case MX25L3205:        
      case PM25LV016: 
      case PM25LV080: 
      case PM25LV040: 
      case W25X32:
      case W25X16:        
      case W25X80:           
      case W25X40:    
      case F25L008A:        
         SPF_INFO1.bBoot = BOOT_ALL;
       break; 
//**********NO BOOT**************************         
     case EN25P40: 
     case ES25P16:        
         SPF_INFO1.bBoot = BOOT_NO;
       break;


      default :
         SPF_INFO1.bBoot = BOOT_NO;
       break;

    }    


    SPF_INFO1.wID = ID;
    if(SPF_INFO1.bSize < bFlashSize)
    { // flash size < code size
        ID = 1;
    }
    return ID;
}
#endif //#ifdef USE_READID

//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
//--------------------------------------------------------------------------

BYTE SPF_EraseChip()
{
    SPF_UNProtected();
    SPFcommand(SPF_INFO1.bWriteEnable,Format_Write_Enable, WaitCMD_STATE,WAIT_TIME);   
    ReadStatus(WEL_STATE,  2  ,WAIT_TIME);

    SPFcommand(SPF_INFO1.bBulkErase
               , Format_Bulk_Erase
               , WaitCMD_STATE
               ,WAIT_TIME);

   if( ReadStatus(WIP_STATE,0,60000)==FALSE)
   {
        return FALSE;
   }
    if(ReadStatus(WEL_STATE,0,60000)==FALSE)
   {
        return FALSE;
   }

    return TRUE;
}

//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
// dwCmd : SPflash command 
// dwOP   :SPflash parament
// dwState: wait state
// dwWaitime: wait timeout
//--------------------------------------------------------------------------

BYTE SPFcommand(DWORD dwCmd,DWORD dwOP,DWORD dwState, DWORD dwWaitime)
{
    OP_REG = dwOP;
    HAL_REORDER_BARRIER();
    
    CMD_REG = dwCmd;
    HAL_REORDER_BARRIER();
    while((OP_REG &dwState) == 0)// wait command  final
    {
        if(dwWaitime ==0)
        {
            return FALSE;  // fail
        }    
        dwWaitime --;
#ifdef SUPPORT_VSYNCPOLL        
        _VSYNCPolling();
#endif
    }

    return TRUE;
}

//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
//--------------------------------------------------------------------------
BYTE SPF_EraseSector(DWORD dwSector)
{
    BYTE  bTemp;
#ifdef FLASH_INT_OFF
    DWORD dwSaveInt;
   HAL_DISABLE_INTERRUPTS(dwSaveInt);  
#endif   

    SPF_UNProtected();
    bTemp = _SPF_EraseSector( dwSector);
    
#ifdef FLASH_INT_OFF
   HAL_RESTORE_INTERRUPTS(dwSaveInt);
#endif
   return bTemp;
}
BYTE _SPF_EraseSector(DWORD dwSector)
{  
    SPFcommand(SPF_INFO1.bWriteEnable,Format_Write_Enable, WaitCMD_STATE,WAIT_TIME);   
    ReadStatus(WEL_STATE,  2  ,WAIT_TIME);

    
    SPFcommand(HLCHANG(dwSector)  |SPF_INFO1.bSectorErase
               ,Format_Sector_Erase
               , WaitCMD_STATE
               ,WAIT_TIME);    
    
    if(ReadStatus(WIP_STATE,0,5000)==FALSE)  
    {
        return FALSE;
    }

    if(ReadStatus(WEL_STATE,0,5000)==FALSE)   
    {
         return FALSE;
    }

    return TRUE;
}


//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
//--------------------------------------------------------------------------
BYTE SPF_WriteProgram(DWORD dwDRAM_Str_Addr,DWORD dwDRAM_End_Addr,DWORD dwFLASH_Addr)
{

    BYTE bTemp;
    #ifdef  FLASH_INT_OFF 
    DWORD dwSaveInt;
   HAL_DISABLE_INTERRUPTS(dwSaveInt);  
   #endif
    SPF_UNProtected();
    bTemp = _SPF_WriteProgram( dwDRAM_Str_Addr, dwDRAM_End_Addr, dwFLASH_Addr);
    #ifdef  FLASH_INT_OFF  
   HAL_RESTORE_INTERRUPTS(dwSaveInt);
   #endif    
   return bTemp;
}
BYTE _SPF_WriteProgram(DWORD dwDRAM_Str_Addr,DWORD dwDRAM_End_Addr,DWORD dwFLASH_Addr)
{

    BYTE bTemp =0;
    BYTE * pbSrc;
    (DWORD)pbSrc  =  dwDRAM_Str_Addr;
    while((DWORD)pbSrc <dwDRAM_End_Addr)
    {
         // 1st cmd
         if(((dwFLASH_Addr&0x000000FF)==0)||bTemp==0)
         {
            bTemp=1;
            //SPFcommand(0x0 ,0x10  , WaitCMD_STATE,WAIT_TIME);  
            if(ReadStatus(WEL_STATE,0,WAIT_TIME)==FALSE)
            {
                 return FALSE;
            }
  
            if(ReadStatus(WIP_STATE,0,WAIT_TIME)==FALSE)   
            {
                 return FALSE;
            }
//            if(WriteEnable()== FALSE) return FALSE;
              SPFcommand(SPF_INFO1.bWriteEnable,Format_Write_Enable, WaitCMD_STATE,WAIT_TIME);   
              if(ReadStatus(WEL_STATE,  2  ,WAIT_TIME)== FALSE) return FALSE;


            WR_REG =  *pbSrc++;
            HAL_REORDER_BARRIER();
            SPFcommand(HLCHANG(dwFLASH_Addr)  |SPF_INFO1.bPageProgram
                                    ,Format_Page_Program
                                    , IOW_STATE
                                    ,WAIT_TIME);   
         }
         else
         {
            while((OP_REG &IOW_STATE) == 0);
            HAL_REORDER_BARRIER();
            WR_REG = *pbSrc++;     
          }
          HAL_REORDER_BARRIER();
          dwFLASH_Addr ++;      
          if(SPF_INFO1.wID == AT26F004
            ||SPF_INFO1.wID == SST25VF040B
            ||SPF_INFO1.wID == SST25VF080B
            ||SPF_INFO1.wID == SST25VF016B
            ||SPF_INFO1.wID == F25L008A)
          {
             bTemp=0;
//             ReadStatus(WIP_STATE,0,WAIT_TIME);
             if(ReadStatus(WIP_STATE,0,WAIT_TIME)==FALSE)
             {
                  return FALSE;
             }        
          }
 
     }
     if(ReadStatus(WEL_STATE,0,WAIT_TIME)==FALSE)
     {
          return FALSE;
     }
     if(ReadStatus(WIP_STATE,0,WAIT_TIME)==FALSE)
     {
          return FALSE;
     }
 
     
    return TRUE;
}

//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
//--------------------------------------------------------------------------
void SPF_SetReadMode(int FastEnable)
{
#if 0
    if(FastEnable)
    {
#ifdef SPF_DEBUG_INFO     
        //printf("Set to Fast Read Mode... ");
#endif
        SCK_CTRL = 0x41010;
        READ_TYPE = 6;
    }
    else
    {
#ifdef SPF_DEBUG_INFO     
        //printf("Set to Normal Read Mode... ");
#endif
        SCK_CTRL = 0x41031;
        READ_TYPE = 0;
    }
#ifdef SPF_DEBUG_INFO         
    //printf("OK!\n");
#endif
#endif
}
//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
//--------------------------------------------------------------------------

BYTE WriteEnable(void)
{
    SPFcommand(SPF_INFO1.bWriteEnable
               ,Format_Write_Enable
               , WaitCMD_STATE
               ,WAIT_TIME);   
    
    return ReadStatus(WEL_STATE,  2  ,WAIT_TIME);
}
//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
//--------------------------------------------------------------------------

void WriteDisable(void)
{
    SPFcommand(SPF_INFO1.bWriteDisable
               ,Format_Write_Disable
               , WaitCMD_STATE
               ,WAIT_TIME);   
    ReadStatus(WEL_STATE,0,WAIT_TIME);
}

//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
//--------------------------------------------------------------------------

BYTE ReadStatus(DWORD dwMusk,DWORD dwCMPValue,DWORD dwWaitime)
{
    DWORD dwBuff;
    dwWaitime = (dwWaitime << 7)+REG_PLAT_TIMER3_VALUE;
//    dwWaitime += (REG_PLAT_TIMER3_VALUE/90);
    do
    {
        OP_REG = Format_Read_Status;
        HAL_REORDER_BARRIER();
        CMD_REG = SPF_INFO1.bRead_Status;
        HAL_REORDER_BARRIER();
        dwBuff = WAIT_TIME;
        while((OP_REG &WaitCMD_STATE) == 0 && (dwBuff--) > 0);// wait command  final
        { }
        HAL_REORDER_BARRIER();     
        dwBuff = RD_REG;
        HAL_REORDER_BARRIER();     

        if((dwBuff & dwMusk) == dwCMPValue)
        {
            return TRUE;
        }

        // if (WAIT_TIME < dwWaitime)
        {
#ifdef SUPPORT_VSYNCPOLL       
            _VSYNCPolling();
#endif
        }
    } while(dwWaitime >= (REG_PLAT_TIMER3_VALUE) );

    return FALSE;
}

//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
//--------------------------------------------------------------------------
void SPF_ReadData(DWORD dwStrAdr,BYTE *pbBuff,DWORD dwCnt)
{

    #ifdef  FLASH_INT_OFF 
    DWORD dwSaveInt;
    HAL_DISABLE_INTERRUPTS(dwSaveInt);  
    #endif
   _SPF_ReadData( dwStrAdr, pbBuff, dwCnt);

   #ifdef  FLASH_INT_OFF      
   HAL_RESTORE_INTERRUPTS(dwSaveInt);
   #endif
}
//--------------------------------------------------------------------------
// Function    :  
// Description :   
// Input       :
//--------------------------------------------------------------------------
 
void _SPF_ReadData(DWORD dwStrAdr,BYTE *pbBuff,DWORD dwCnt)
{
    DWORD dwTemp;

    for(dwTemp=0;dwTemp<dwCnt;dwTemp++)
    {
        asm volatile ("lduba [%1] 0x7, %0;" : "=r" (pbBuff[dwTemp]) : "r" (dwStrAdr+dwTemp));
    }
/*
    DWORD dwBuf,dwTemp;

    if((READ_TYPE&0x03) < 3)
    {
        if( (READ_TYPE&0x03) ==0)
        {
            SPFcommand(HLCHANG( dwStrAdr)|SPF_INFO1.bReadData
                                       ,Format_Read_Data
                                       , IOR_STATE
                                       ,WAIT_TIME);      
           while((OP_REG &IOR_STATE) == 0);
        }
        else 
        {
            SPFcommand(HLCHANG( dwStrAdr)|SPF_INFO1.bFastRead
                                       ,Format_Fast_Read
                                       , IOR_STATE
                                       ,WAIT_TIME);      
           while((OP_REG &IOR_STATE) == 0);
        
           HAL_REORDER_BARRIER();        
           dwBuf = RD_REG&0x000000ff;
        } 
        
        for(dwTemp=0;dwTemp<dwCnt;dwTemp++)
        {
            while((OP_REG &IOR_STATE) == 0);
            HAL_REORDER_BARRIER();        
            dwBuf = RD_REG&0x000000ff;
            pbBuff[dwTemp] = (BYTE)(dwBuf);
        }
        return;
    }
    else
    {    
        PLAT_PROC1_DCACHE_DISABLE;
        for(dwTemp=0;dwTemp<dwCnt;dwTemp++)
        {
            pbBuff[dwTemp] = ((BYTE *)dwStrAdr)[dwTemp];
        }
        PLAT_PROC1_DCACHE_ENABLE;    
        return;
    }  
*/

}


//--------------------------------------------------------------------------
// Function    : 
// Description :   
// Input       :
//--------------------------------------------------------------------------
void WriteStatus(DWORD dwState)
{
    WR_REG = dwState;
    SPFcommand( HLCHANG( dwState)|SPF_INFO1.bWrite_Status
                               ,Format_Write_Status
                               , WaitCMD_STATE
                               ,WAIT_TIME);   

}

//--------------------------------------------------------------------------
// Function    : DeepPowerDown
// Description :   
// Input       :
//--------------------------------------------------------------------------

void DeepPowerDown(void)
{
     SPFcommand(SPF_INFO1.bPowerDown
                               ,Format_Power_Down
                               , WaitCMD_STATE
                               ,WAIT_TIME);      
    return;
}
void ReleasePowerDown(void)
{
     SPFcommand ( SPF_INFO1.bRePowerDown
                               ,Format_Release_Power_Down
                               , WaitCMD_STATE
                               ,WAIT_TIME);      

    return;
}
//--------------------------------------------------------------------------
// Function    : SPF_UNProtected
// Description : UN write Protected 
// Input       :
//--------------------------------------------------------------------------
void SPF_UNProtected(void)
{
    switch(SPF_INFO1.wID)
    {
        case SST25VF040B:
        case SST25VF080B: 
        case SST25VF016B:
        case F25L008A:
             if(ReadStatus(0x9C,0,5)!= TRUE)
             { 
                SPFcommand(SPF_INFO1.bWriteEnable,Format_Write_Enable, WaitCMD_STATE,WAIT_TIME); 
                ReadStatus(WEL_STATE,  2  ,WAIT_TIME);
                SPFcommand(SPF_INFO1.bWrite_Status,Format_Write_Status, WaitCMD_STATE,WAIT_TIME);           
                while(!ReadStatus(0x1,0,5));
             }
        break;

        case AT26F004:
        case AT26DF081A:
        case AT26DF161:            
        case AT26DF321:      
          {
              DWORD dwSector;
              WORD wSectorNUM;
              switch(SPF_INFO1.bSize)
              {
                  case FLASH_SIZE_4M:  
                      wSectorNUM = 0x80;
                      break; 
                  case FLASH_SIZE_8M :           
                      wSectorNUM = 0x100;
                      break;
  
                  case FLASH_SIZE_16M :           
                      wSectorNUM = 0x200;
                      break;
  
                  case FLASH_SIZE_32M:
                      wSectorNUM = 0x400;
                      break;
                  default:    
                       wSectorNUM = 0x80;
                       break;
  
              }
              for(dwSector =0; dwSector <wSectorNUM ;dwSector++)
              {
                  SPFcommand(SPF_INFO1.bWriteEnable,Format_Write_Enable, WaitCMD_STATE,WAIT_TIME);   
                  if(ReadStatus(WEL_STATE,  2  ,WAIT_TIME)==FALSE)
                  {
                      break;
                  }
                  
                  if(SPFcommand( HLCHANG( dwSector*0x1000)|0x39
                                         ,0x03
                                         , WaitCMD_STATE
                                         ,WAIT_TIME) == FALSE)
                  {
                      break;
                  }
              }
          }
          break;
          
        case MX25L8005:
        case MX25L4005:
        case MX25L1605:  
        case MX25L3205:            
        case A25L16PT://Add by PeterYu, 2007/3/13 02:51下午 
        case A25L16PB:     
          //other unlockall
            SPFcommand(SPF_INFO1.bWriteEnable,Format_Write_Enable, WaitCMD_STATE,WAIT_TIME);
            ReadStatus(WEL_STATE,  2  ,WAIT_TIME);
            SPFcommand(SPF_INFO1.bWrite_Status,Format_Write_Status, WaitCMD_STATE,WAIT_TIME); 
            while(!ReadStatus(0x1,0,5));                             
        break;
          
        default :
          break;
                  
    }
}

#endif //#ifndef NO_SERIAL_FLASH
