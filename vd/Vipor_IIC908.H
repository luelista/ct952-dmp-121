/***** CT908 *****/
extern    BYTE _I2C_Read(BYTE bDevSel, BYTE bAddr, BYTE bCount, BYTE *bVal);
extern    void _I2C_Write(BYTE bDevSel, BYTE *pBuffer, BYTE bCount);
extern  BIT   UTL_DelayTime ( WORD wCount, BIT btBreak );

extern  BYTE gbTrmBuf[];
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
#include "ViporVariable.h"
#define bIICTemp1 __bViporIICBuf1
#define bIICTemp2 __bViporIICBuf2

#pragma NOAREGS
void Vipor_Write_Burst(BYTE bDevAddr, BYTE bSubAddr, BYTE bCount)
{ 
    for (bIICTemp1=bCount;bIICTemp1>0;bIICTemp1--)
     gbTrmBuf[bIICTemp1]=gbTrmBuf[bIICTemp1-1];
    gbTrmBuf[0]= bSubAddr;
    bCount++;
    _I2C_Write (bDevAddr,&gbTrmBuf[0],bCount);
}

//--------------------------------------------------------------------------
BYTE Vipor_SC_Read_Byte(BYTE bSubAddr)
{
	_I2C_Read(0xF4, bSubAddr, 1, &gbTrmBuf[0]);
	return gbTrmBuf[0];
}

//--------------------------------------------------------------------------
BYTE Vipor_VD_Read_Byte(BYTE bSubAddr)
{
	_I2C_Read(0x40, bSubAddr, 1, &gbTrmBuf[0]);
	return gbTrmBuf[0];   
}

//--------------------------------------------------------------------------
void Vipor_SC_Write_Byte(BYTE bSubAddr, BYTE bData)
{
	gbTrmBuf[0]  = bSubAddr;
	gbTrmBuf[1] = bData;
	_I2C_Write (0xF4,&gbTrmBuf[0],2);
}

//--------------------------------------------------------------------------
void Vipor_AD_Write_Byte(BYTE bSubAddr, BYTE bData)
{
	gbTrmBuf[0]  = bSubAddr;
	gbTrmBuf[1] = bData;
	_I2C_Write (0x42,&gbTrmBuf[0],2);
}

//--------------------------------------------------------------------------
void Vipor_VD_Write_Byte(BYTE bSubAddr, BYTE bData)
{
	gbTrmBuf[0]  = bSubAddr;
	gbTrmBuf[1] = bData;
	//_I2C_Write(0xF4, &bIICTemp1, 2);
	_I2C_Write (0x40,&gbTrmBuf[0],2);
}

//--------------------------------------------------------------------------
void Vipor_TC_Write_Byte(BYTE bSubAddr, BYTE bData)
{
	gbTrmBuf[0]  = bSubAddr;
	gbTrmBuf[1] = bData;
	_I2C_Write (0xF6,&gbTrmBuf[0],2);
}

void Delay_mS(BYTE bMilliSec)
{
    UTL_DelayTime(bMilliSec, TRUE);	// 50mS
}


void VP_IIC_Write(BYTE bDevSel,BYTE bSubAddr,BYTE bData)
{
	gbTrmBuf[0]  = bSubAddr;
	gbTrmBuf[1] = bData;
	_I2C_Write (bDevSel,&gbTrmBuf[0],2);
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

#pragma NOAREGS
void Vipor_WriteRegister(BYTE code *dptr)
{
#define i __bViporTemp1
#define bCnt __bViporTemp2
#define bDevAddr  __bViporTemp3
#define bSubAddr __bViporTemp4
    while( 1 )
    {
        bCnt = *dptr++;
        if( bCnt==0 )  return;
        if( bCnt>11 ) printf("error too long\n");
        bDevAddr = *dptr++;
        bSubAddr = *dptr++;
        for(i=0; i<bCnt; i++)
            gbTrmBuf[i] = *dptr++;

		Vipor_Write_Burst(bDevAddr, bSubAddr, bCnt);
    }
#undef bSubAddr
#undef i 
#undef bCnt 
#undef bDevAddr 
}

void Vipor_CCIR656_NTSC(void)
{
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
