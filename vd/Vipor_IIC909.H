/*****  CT909 *****/

extern    BOOL    _I2C_Read(PI2C_CONFIG pConfig, BYTE bDevSel, BYTE bAddr, BYTE bCount, BYTE *bVal);
extern    BOOL    _I2C_Write(PI2C_CONFIG pConfig, BYTE bDevSel, BYTE *pBuffer, BYTE bCount);
extern    void    OS_DelayTime (DWORD dwCount);
extern    I2C_CONFIG  I2C_Config;

extern  BYTE gbTrmBuf[];
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
#include "ViporVariable.h"
#define bIICTemp1 __bViporIICBuf1
#define bIICTemp2 __bViporIICBuf2

TRANSFER_CONFIG I2C_TransferConfig;

void Vipor_Write_Burst(BYTE bDevAddr, BYTE bSubAddr, BYTE bCount)
{

		  I2C_TransferConfig.bCount = bCount; //Table Data count
		  I2C_TransferConfig.bAddr = bSubAddr;  //Sub Address
		  I2C_TransferConfig.pbBuffer = &gbTrmBuf[0];
		  HAL_I2C_Write (bDevAddr, &I2C_Config, &I2C_TransferConfig);		

}

//--------------------------------------------------------------------------
BYTE Vipor_SC_Read_Byte(BYTE bSubAddr)
{  
      I2C_TransferConfig.bCount = 2; //Table Data count
		  I2C_TransferConfig.bAddr = bSubAddr;  //Sub Address
		  I2C_TransferConfig.pbBuffer = &gbTrmBuf[0]; 
	if (TRUE == HAL_I2C_Read (0xF4, &I2C_Config, &I2C_TransferConfig))
        	return gbTrmBuf[0];
        else
        {
            return FALSE;
        }   
}

//--------------------------------------------------------------------------
BYTE Vipor_VD_Read_Byte(BYTE bSubAddr)
{
		  I2C_TransferConfig.bCount = 2; //Table Data count
		  I2C_TransferConfig.bAddr = bSubAddr;  //Sub Address
		  I2C_TransferConfig.pbBuffer = &gbTrmBuf[0]; 
	if (TRUE == HAL_I2C_Read (0x40, &I2C_Config, &I2C_TransferConfig))
        	return gbTrmBuf[0];
        else
        {
            return FALSE;
        } 
}

//--------------------------------------------------------------------------
void Vipor_SC_Write_Byte(BYTE bSubAddr, BYTE bData)
{
		I2C_TransferConfig.bCount = 1; //Table Data count
		I2C_TransferConfig.bAddr = bSubAddr;  //Sub Address
		I2C_TransferConfig.pbBuffer = &bData;
		HAL_I2C_Write (0xF4, &I2C_Config, &I2C_TransferConfig); 
}

//--------------------------------------------------------------------------
void Vipor_AD_Write_Byte(BYTE bSubAddr, BYTE bData)
{
		I2C_TransferConfig.bCount = 1; //Table Data count
		I2C_TransferConfig.bAddr = bSubAddr;  //Sub Address
		I2C_TransferConfig.pbBuffer = &bData;
		HAL_I2C_Write (0x42, &I2C_Config, &I2C_TransferConfig); 	
}

//--------------------------------------------------------------------------
void Vipor_VD_Write_Byte(BYTE bSubAddr, BYTE bData)
{
		I2C_TransferConfig.bCount = 1; //Table Data count
		I2C_TransferConfig.bAddr = bSubAddr;  //Sub Address
		I2C_TransferConfig.pbBuffer = &bData;
		HAL_I2C_Write (0x40, &I2C_Config, &I2C_TransferConfig); 	
}

//--------------------------------------------------------------------------
void Vipor_TC_Write_Byte(BYTE bSubAddr, BYTE bData)
{   
		I2C_TransferConfig.bCount = 1; //Table Data count
		I2C_TransferConfig.bAddr = bSubAddr;  //Sub Address
		I2C_TransferConfig.pbBuffer = &bData;
		HAL_I2C_Write (0xF6, &I2C_Config, &I2C_TransferConfig); 
}

void Delay_mS(BYTE bMilliSec)
{
	OS_DelayTime(bMilliSec);	// 50mS
}


void VP_IIC_Write(BYTE bDevSel,BYTE bSubAddr,BYTE bData)
{
		I2C_TransferConfig.bCount = 1; //Table Data count
		I2C_TransferConfig.bAddr = bSubAddr;  //Sub Address
		I2C_TransferConfig.pbBuffer = &bData;
		HAL_I2C_Write (bDevSel, &I2C_Config, &I2C_TransferConfig); 
}

//##########################################################################
void SoftResetMvdSync(void)
{
	// printf("ADC auto --- Get Offset\n");
	Vipor_AD_Write_Byte(0x50, 0x0F);
	Delay_mS(10);
	Vipor_AD_Write_Byte(0x50, 0x00);
return;
	// printf("SoftResetMvdSync\n");
	Delay_mS(40);
	Vipor_SC_Write_Byte(0x08, 0x04);
	Vipor_SC_Write_Byte(0x08, 0x80);
	
}
void SoftReset(void)
{
	Vipor_SC_Write_Byte(0x08, 0x04);
  Delay_mS(20);
	Vipor_SC_Write_Byte(0x08, 0x00);
	Vipor_SC_Write_Byte(0x09, 0x03);
}

void Vipor_WriteRegister(BYTE code *dptr)
{
#define i __bViporTemp1
#define bCnt __bViporTemp2
#define bDevAddr  __bViporTemp3
    while(1)
    {
        bCnt = *dptr;
        I2C_TransferConfig.bCount = *dptr++; //Table Data count
        if(bCnt == 0) return;  //Table Data end
	     	bDevAddr = *dptr++;  //Device Address
		    I2C_TransferConfig.bAddr = *dptr++;  //Sub Address
		    
        if(bCnt > 16) bCnt = 16;

        for(i=0 ; i<bCnt ; i++)
            gbTrmBuf[i] = *dptr++; //Table Data 
        I2C_TransferConfig.pbBuffer = &gbTrmBuf[0];
        HAL_I2C_Write (bDevAddr, &I2C_Config, &I2C_TransferConfig);  
    }
#undef i 
#undef bCnt 
#undef bDevAddr 
}

void Vipor_CCIR656_NTSC(void)
{
	printf("CCIR656_NTSC");
	//Vipor_WriteRegister(Vipor_COMMON_NTSC_Register);
	Vipor_WriteRegister(Vipor_CCIR656_NTSC_Register);
	//Vipor_WriteRegister(Vipor_CVBS_NTSC_Register);
	SoftResetMvdSync();
}
void Vipor_CCIR656_PAL(void)
{
	Vipor_WriteRegister(Vipor_CCIR656_PAL_Register);
	//Vipor_WriteRegister(Vipor_CVBS_NTSC_Register);
	SoftResetMvdSync();
}

//--------------------------------------------------------------------------
