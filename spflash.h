#ifndef __SPFLASH_H__
#define __SPFLASH_H__

#define CMD_REG     *((volatile unsigned long *)0x80002a20+bRegistOffset)
#define OP_REG      *((volatile unsigned long *)0x80002a24+bRegistOffset)
#define READ_TYPE   *((volatile unsigned long *)0x80002a28+bRegistOffset)
#define WR_REG      *((volatile unsigned long *)0x80002a2c+bRegistOffset)
#define RD_REG      *((volatile unsigned long *)0x80002a30+bRegistOffset)
#define SCK_CTRL    *((volatile unsigned long *)0x80002a34+bRegistOffset)

#define CMD                    0x00
#define CMD_AD0                0x20
#define CMD_AD0_AD1            0x40
#define CMD_AD0_AD1_AD2        0x60
#define CMD_AD0_AD1_AD2_ADn    0x80

#define OPL_D1                 0x00
#define OPL_D1_D2              0x01
#define OPL_D1_D2_D3           0x02
#define OPL_D1_D2_D3_D4        0x03
#define OPL_D1_D2_D3_D4_Dn     0x04

#define IO_R 0x00
#define IO_W 0x08

#define Format_Write_Enable           CMD+                IO_W+OPL_D1//0x00000008
#define Format_Write_Disable          CMD+                IO_W+OPL_D1//0x00000008
#define Format_Read_Status            CMD+                IO_R+OPL_D1//0x00000020
#define Format_Write_Status           CMD+                IO_W+OPL_D1_D2//0x00000009
#define Format_Read_Data              CMD_AD0_AD1_AD2_ADn+IO_R+OPL_D1_D2_D3_D4//0x00000083
#define Format_Fast_Read              CMD_AD0_AD1_AD2_ADn+IO_R+OPL_D1_D2_D3_D4//0x00000083 
#define Format_Page_Program           CMD+                IO_W+OPL_D1_D2_D3_D4_Dn//0x0000000c
#define Format_Sector_Erase           CMD+                IO_W+OPL_D1_D2_D3_D4//0x0000000b
#define Format_Bulk_Erase             CMD+                IO_W+OPL_D1//0x00000008
#define Format_Power_Down             CMD+                IO_W+OPL_D1//0x00000008
#define Format_Release_Power_Down     CMD+                IO_W+OPL_D1//0x00000008
#define Format_Device_ID              CMD_AD0+            IO_R+OPL_D1_D2_D3_D4//0x00000023
#define Format_Device_ID1             CMD_AD0+            IO_R+OPL_D1//0x00000020
//Flash state
#define SRWD_STATE      0x00000080
#define BP_STATE        0x0000001c
#define WEL_STATE       0x00000002
#define WIP_STATE       0x00000001

//System state
#define WaitCMD_STATE   0x00001000
#define IOW_STATE       0x00000800
#define IOR_STATE       0x00000400
#define IDLE_STATE      0x00000200

// SPFLASH size
#define FLASH_SIZE_32M      0x20
#define FLASH_SIZE_16M      0x10
#define FLASH_SIZE_8M       0x08
#define FLASH_SIZE_4M       0x04
//BOOT area
#define BOOT_BOTTOM     0X01
#define BOOT_TOP        0X02
#define BOOT_NO         0X03    //NO 4k sector
#define BOOT_ALL        0X04   //all 4K sector

//flash TYPE
#define  MX25L8005      0xC213
#define  MX25L4005      0xC212
#define  MX25L1605      0xC214
#define  MX25L3205      0xC215
#define  MX25L6405      0xC216

#define  SST25LF080A    0xBF80
#define  SST25LF040A    0xBF44
#define  SST25VF040B    0xBF8d
#define  SST25VF080B    0xBF8E
#define  SST25VF016B    0xBF41

#define  F25L008A       0x8C13  //ESMT

#define  EN25P40        0x1C12   //yhtang_060327

#define  EN25B40B       0x1C32
#define  EN25B40T       0x1C42

#define  EN25B80        0x1C33
#define  EN25B80T       0x1C43

#define  EN25B16        0x1C34
#define  EN25B16T       0x1C44

#define  W25D40B        0xEF32  // same W25B40B
#define  W25D40T        0xEF42  // same W25B40T
#define  W25X80         0xEF13
#define  W25X40         0xEF12
#define  W25X16         0xEF14
#define  W25X32         0xEF15
        


#define  PM25LV040      0x9D7E
#define  PM25LV080      0x9D13
#define  PM25LV016      0x9D14

#define AT26F004        0x1F04    
#define AT26DF081A      0x1F45
#define AT26DF161       0x1F46
#define AT26DF321       0x1F47


#define A25L80P         0x3714
#define A25L40P         0x3713

#define A25L16PT        0x3725//Add by PeterYu,0x3715:bottom, 0x3725:top , 2007/3/13 02:51¤U¤È 
#define A25L16PB        0x3715

#define ES25P16         0x4A20

//end flash TYPE
typedef struct  SPF_INFO
{    
   DWORD dwProm_config;
//   WORD  wFast_Normal_FQ;  
//   BYTE  bMODE;
//   BYTE  bPH;       

   WORD  wID;

   BYTE  bSize;
   BYTE  bBoot;
   BYTE  bWriteEnable ;
   BYTE  bWriteDisable;
   BYTE  bRead_Status ;
   BYTE  bWrite_Status;
   BYTE  bPageProgram ;
   BYTE  bSectorErase ;

   BYTE  bBulkErase ;
   BYTE  bReadData ;
   BYTE  bFastRead;
   BYTE  bPowerDown;
   BYTE  bRePowerDown;
}  SPF_INFO;
SPF_INFO SPF_INFO1;

typedef struct DWSCLK_909P
{
    unsigned Reserved    : 5;
    unsigned Latency     : 1;
    unsigned Read_type   : 2;
    unsigned Reserved1   : 4;
    unsigned SCLK_Update : 1;
    unsigned SCLK_SEL    : 3;
    unsigned FULL_Cycle  : 16;
} tSCLK_909P;
extern tSCLK_909P *pSCLK_909P;

typedef struct DWSCLK_909S 
{
    unsigned Reserved    : 13;
    unsigned SCLK_Update : 1;
    unsigned SCLK_SEL    : 2;
    unsigned FULL_Cycle  : 16;
} tSCLK_909S;
extern tSCLK_909S *pSCLK_909S;
    
typedef struct DWREAD_TYPE_909S 
{
    unsigned Reserved    : 28;
    unsigned RD_REG_RD   : 1;    
    unsigned Latency     : 1;
    unsigned Read_type   : 2;
} tREAD_TYPE_909S;
extern tREAD_TYPE_909S *pREAD_TYPE_909S;

typedef struct DWPROM_CONFIG 
{
    unsigned Flash_Size  : 4;
    unsigned Reserved    : 5;
    unsigned Latency     : 1;
    unsigned Read_type   : 2;
    unsigned SCLK_Update : 1;
    unsigned SCLK_SEL    : 3;
    unsigned FULL_Cycle  : 16;
} tPROM_CONFIG;
extern tPROM_CONFIG *pPROM_CONFIG;

extern BYTE     bRegistOffset;
extern DWORD __dwSCLK,__dwREAD_TYPE;

WORD SPF_ReadID( BYTE bFlashSize );
BYTE SPF_WriteProgram(DWORD dwDRAM_Addr,DWORD dwDRAM_End_Addr,DWORD dwFLASH_Addr);
void SPF_ReadData(DWORD dwStrAdr, BYTE * pbBuff, DWORD dwCnt);
BYTE SPF_EraseSector(DWORD dwSector);

WORD _SPF_ReadID( BYTE bFlashSize );
BYTE _SPF_WriteProgram(DWORD dwDRAM_Addr,DWORD dwDRAM_End_Addr,DWORD dwFLASH_Addr);
void _SPF_ReadData(DWORD dwStrAdr, BYTE * pbBuff, DWORD dwCnt);
BYTE _SPF_EraseSector(DWORD dwSector);


BYTE SPFcommand(DWORD dwCmd,DWORD dwOP,DWORD dwState, DWORD dwWaitime);
BYTE SPF_EraseChip(void);
void SPF_SetReadMode(int FastEnable);
//DWORD  ReadStatus(void);
BYTE ReadStatus(DWORD dwMusk,DWORD dwCMPValue,DWORD dwWaitime);
BYTE WriteEnable(void);
void WriteDisable(void);
BYTE SPF_UNLockAll(DWORD dwFunction);
DWORD SPF_MPLLSet(DWORD dwCPUSpeed); // LLY2.36
//DWORD SPF_ReadTest(DWORD dwCPUSpeed,DWORD dwCheckvalue);
//DWORD SPF_ReadTest(DWORD dwCPUSpeed,DWORD dwPROMConfig, DWORD dwCheckvalue);
DWORD SPF_TestStable(DWORD dwCheckvalue);

void SPF_UNProtected(void);

#endif//#ifndef __SPFLASH_H__
