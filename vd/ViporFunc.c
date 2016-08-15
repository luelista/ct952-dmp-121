#ifdef _WINDVD

#ifdef VP_API_CT909
  #include "..\Hio.H"
#endif
#ifdef VP_API_CT909P
  #include "..\Hio.H"
#endif
extern	void _printf(char *fmt, ...);

#else
	#include "defs.H"
#endif

#ifdef VP_API_CT909
//BYTE  _REG[26];
BYTE  gbTrmBuf[17];
#endif

#ifdef VP_API_CT909P
//BYTE  _REG[26];
BYTE  gbTrmBuf[17];
#endif
/*
#ifdef VP_API_CT909P
#ifdef Digital_PANEL
#define PScan_mode
#include "Regtable_CT956.H"
#else
#include "Regtable_CT952.H"
#endif
#else
#include "Regtable_CT950.H"
#endif
*/

#include "ViporVariable.h"
#include "Vipor_API.h"
#include "ViporFunc.h"

#ifdef VP_API_CT908
#include "Vipor_iic908.h"
#endif
#ifdef VP_API_CT909
#include "Vipor_iic909.h"
#endif
#ifdef VP_API_CT909P
#include "Vipor_iic909P.h"
#endif
BYTE _bBacklight;
BYTE u8VideoHue;
BYTE u8VideoContrast;
BYTE u8VideoSaturation;
//#define sRGB_HDTV
//#define sRGB_SDTV_Computer
#ifdef SetPicture_ALL
BYTE Temperature_Adjust[][3] =
#else
BYTE code Temperature_Adjust[][3] =
#endif
{
	{ 0x80, 0x80, 0x80 }, //R G B Gain  0 ... 128 ... 255
	{ 0x80, 0x80, 0x80 }  //R G B Offset  0 ... 128 ... 255
};	

//DWORD __dwVersionTFT;
#define     TFT_SW_VERSION          (121L)
#define     TFT_PANEL_VERSION    (0)
#define     TFT_PANEL_MINOR_VERSION    (0)

extern DWORD __dwVersionTFT;  
extern WORD ViporSin(BYTE bval);   
extern WORD ViporCos(BYTE bval);
extern WORD VPAddSize(WORD para1,WORD para2);

#ifdef VP_API_CT909
BYTE gbTrmBuf[17];
#endif

WORD wval;
BYTE Vipor_BATTERY_State (void)
{
	Vipor_SC_Write_Byte(0x07,Vipor_SC_Read_Byte(0x07) & 0xF7); //Enable SAR
  Vipor_SC_Write_Byte(0x0E,Vipor_SC_Read_Byte(0x0E) & 0x08); //Disable SAR Trigger Enable select ADC0
  Vipor_SC_Write_Byte(0x0E,Vipor_SC_Read_Byte(0x0E)|0x04); //Enable SAR Trigger 

  if (Vipor_SC_Read_Byte(0x0E) && 0x08) //Check if Filish
   return Vipor_SC_Read_Byte(0x0F);
  else return 0; 
}

//set CVBS(video format)'s videomode PAL
void Vipor_SetVideo_CVBS_PAL(void)
{
	Vipor_WriteRegister(Vipor_CVBS_PAL_Register);

	SoftResetMvdSync();

}

//set CVBS(video format)'s videomode NTSC
void Vipor_SetVideo_CVBS_NTSC(void)
{
	printf("CVBS_NTSC");
	Vipor_WriteRegister(Vipor_CVBS_NTSC_Register);

	SoftResetMvdSync();

}

//set SVIDEO(video format)'s videomode PAL
void Vipor_SetVideo_SVIDEO_PAL(void)
{
	Vipor_WriteRegister(Vipor_SVIDEO_PAL_Register);

	SoftResetMvdSync();

}

//set SVIDEO(video format)'s videomode NTSC
void Vipor_SetVideo_SVIDEO_NTSC(void)
{
	Vipor_WriteRegister(Vipor_SVIDEO_NTSC_Register); //Jeff 20060313 modify

	SoftResetMvdSync();

}


//Set video aspect ratio 4:3
void Vipor_SetAspect_4_3(void)
{
	Vipor_WriteRegister(Vipor_4_3_Register);
}

//Set video aspect ratio 16:9 
void Vipor_SetAspect_16_9(void)
{
	Vipor_WriteRegister(Vipor_16_9_Register);
}
void Vipor_SetAspect_4_3_CVBS(void)
{
	Vipor_WriteRegister(Vipor_CVBS_4_3_Register);
}

//Set video aspect ratio 16:9 
void Vipor_SetAspect_16_9_CVBS(void)
{
	Vipor_WriteRegister(Vipor_CVBS_16_9_Register);
}
//Set picture Brightness
//bBriValue: brightness value
BYTE code Vipor_BrightnessXferTable[]=
//  0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15
//{0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0, 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70 ,0x7F };
{0x90, 0xA0, 0xB0, 0xC0, 0xE8, 0xF0, 0xF8, 0x00, 0x0B, 0x15, 0x20, 0x40, 0x50, 0x60, 0x70 ,0x7F };
// -8    -7    -6    -5    -4    -3    -2    -1     0    +1    +2    +3    +4    +5    +6    +7
#ifdef VP_API_CT908
#pragma NOAREGS //may be called by ISR  // KCHong, 20060717
#endif
void Vipor_SetPicture_Brightness(BYTE bBriValue)
{
	//printf("Brightness = %hx\n",Vipor_BrightnessXferTable[bBriValue >> 4]);
	Vipor_SC_Write_Byte(0xE1, Vipor_BrightnessXferTable[bBriValue >> 4]);
}
//Set picture Contrast
#ifdef VP_API_CT908
#pragma NOAREGS //may be called by ISR  // KCHong, 20060717
#endif //may be called by ISR  // KCHong, 20060717
void Vipor_SetPicture_Contrast(BYTE bConValue)
{
 u8VideoHue = 128;
 u8VideoContrast = bConValue;
 ViporBCAdjust();

}
#ifdef SetPicture_ALL
#ifdef VP_API_CT908
#pragma NOAREGS //may be called by ISR  // KCHong, 20060717
#endif
void Vipor_SetPicture_ALL(void)
{
	extern BYTE _bLEVEL_TABLE[];
	Vipor_SC_Write_Byte(0xE1, _bLEVEL_TABLE[0]);
	u8VideoContrast = _bLEVEL_TABLE[1];
  u8VideoHue = _bLEVEL_TABLE[2];
  u8VideoSaturation = _bLEVEL_TABLE[3];
  Temperature_Adjust[0][0] = _bLEVEL_TABLE[4];
  Temperature_Adjust[0][1] = _bLEVEL_TABLE[5];
  Temperature_Adjust[0][2] = _bLEVEL_TABLE[6];
  Temperature_Adjust[1][0] = _bLEVEL_TABLE[7];
  Temperature_Adjust[1][1] = _bLEVEL_TABLE[8];
  Temperature_Adjust[1][2] = _bLEVEL_TABLE[9];
  ViporBCAdjust();

}
#endif //SetPicture_ALL
#ifdef VP_API_CT908
#pragma NOAREGS //may be called by ISR  // KCHong, 20060717
#endif //may be called by ISR  // KCHong, 20060717
void Vipor_SetPicture_Saturation(BYTE bSatValue)
{
 u8VideoHue = 128;
 u8VideoSaturation = bSatValue;
 ViporBCAdjust();
}

//judge NTSC(video mode) color if Burst\locked
//output: TRUE\FALSE
BOOL Vipor_NTSC_GetColorBurstLocked(void)
{
#define fNoColorBurst __bViporTemp5
//	BOOL fNoColorBurst;

	fNoColorBurst = Vipor_VD_Read_Byte(0x3B) & 0x08;

	if((Vipor_VD_Read_Byte(0x3A) & 0x08) == 0x08)
	{
		return TRUE;		// Video Color Locked
	}

	if(fNoColorBurst)
	{
		gbTrmBuf[0] = Vipor_VD_Read_Byte(0x00);
		Vipor_VD_Write_Byte(0x00, (gbTrmBuf[0] & 0x01));
		Vipor_VD_Write_Byte(0x03, 0x00);
		Vipor_WriteRegister(rVideoReg18_60[0]);
		return TRUE;
	}

	return FALSE;
#undef fNoColorBurst
}

//for NTSC(video mode) subsystem
//bVideoSubSystem:	NTSC_M, PAL_443, NTSC_443,
void Vipor_NTSC_VideoSubSystem(BYTE bVideoSubSystem)
{
	Vipor_VD_Write_Byte(0xB2, 0x40);
	Vipor_WriteRegister(rVideoReg18_60[bVideoSubSystem]);

	gbTrmBuf[0] = Vipor_VD_Read_Byte(0x00);
	switch( bVideoSubSystem )
	{
		case 1: // PAL 4.43 - 60Hz
			Vipor_VD_Write_Byte(0x00, gbTrmBuf[0]|0x04);
			Vipor_VD_Write_Byte(0x03, 0x1E);
			break;
		case 2: // NTSC 4.43 - 60Hz
			Vipor_VD_Write_Byte(0x00,gbTrmBuf[0]&0x01);
			Vipor_VD_Write_Byte(0x03,0x86);
			break;
		case 0: // NTSC M
		default:
			Vipor_VD_Write_Byte(0x00,gbTrmBuf[0]&0x01);
			Vipor_VD_Write_Byte(0x03,0x00);
			break;
	}
}


//judge PAL(video mode) color if Burst\locked
//output: TRUE\FALSE
BOOL Vipor_PAL_GetColorBurstLocked(BYTE bVideoSubSystem)
{
#define fNoColorBurst2  __bViporTemp6
//	BOOL fNoColorBurst;

	fNoColorBurst2 = Vipor_VD_Read_Byte(0x3B) & 0x08;

	gbTrmBuf[0] = Vipor_VD_Read_Byte(0x3C);
	if(bVideoSubSystem <= 1)   // 0 for PAL-B,G,D,K,I,N   1 for PAL-CN
	{
		if((gbTrmBuf[0] & 0x05) == 0x05) 
			return TRUE;
	}
	else  // (bVideoSubSystem == 2)
	{
		if((gbTrmBuf[0] & 0x07) == 0x06) 
			return TRUE;
	}

	if(fNoColorBurst2)
	{
		gbTrmBuf[0] = (Vipor_VD_Read_Byte(0x00) & 0x01) | 0x32;
		Vipor_VD_Write_Byte(0x00, gbTrmBuf[0]);

		gbTrmBuf[0] = (Vipor_VD_Read_Byte(0x01) & 0x40) | 0x88;
		Vipor_VD_Write_Byte(0x01, gbTrmBuf[0]);

		Vipor_VD_Write_Byte(0x2C, 0x32);
		Vipor_VD_Write_Byte(0x2D, 0x46);
		Vipor_VD_Write_Byte(0x2E,0x90);
		Vipor_WriteRegister(rVideoReg18_50[0]);
		return TRUE;
	}

	return FALSE;
#undef fNoColorBurst2	
}


//for PAL(video mode) subsystem
//bVideoSubSystem:	PAL_BGDHI, PAL_N, SECAM
void Vipor_PAL_VideoSubSystem(BYTE bVideoSubSystem)
{
	Vipor_VD_Write_Byte(0xB2, 0x40);

	Vipor_WriteRegister(rVideoReg18_50[bVideoSubSystem]);

	gbTrmBuf[0] = Vipor_VD_Read_Byte(0x00);
	gbTrmBuf[1] = Vipor_VD_Read_Byte(0x01);
	switch( bVideoSubSystem )
	{
		case 1: // PAL N
			Vipor_VD_Write_Byte(0x00, (gbTrmBuf[0] | 0x36));
			Vipor_VD_Write_Byte(0x01, (gbTrmBuf[1] & 0xFE));
			Vipor_VD_Write_Byte(0x2C, 0x32);
			Vipor_VD_Write_Byte(0x2D, 0x46);
			Vipor_VD_Write_Byte(0x2E, 0x90);
			Vipor_VD_Write_Byte(0xB5, 0x80);
			break;
		case 2: // SECAM
			Vipor_VD_Write_Byte(0x00,  (gbTrmBuf[0] | 0x38));
			Vipor_VD_Write_Byte(0x01, ((gbTrmBuf[1] & 0x40) | 0x03));
			Vipor_VD_Write_Byte(0x2C, 0x32);
			Vipor_VD_Write_Byte(0x2D, 0x6E);
			Vipor_VD_Write_Byte(0x2E, 0x90);
			Vipor_VD_Write_Byte(0xB5, 0x03);
			break;
		case 0: // PAL BGDHI
		default:
			Vipor_VD_Write_Byte(0x00,  (gbTrmBuf[0] | 0x32));
			Vipor_VD_Write_Byte(0x01, ((gbTrmBuf[1] & 0x40) | 0x08));
			Vipor_VD_Write_Byte(0x2C, 0x32);
			Vipor_VD_Write_Byte(0x2D, 0x46);
			Vipor_VD_Write_Byte(0x2E, 0x90);
			Vipor_VD_Write_Byte(0xB5, 0x80);
			break;
	}
}


//judge video if locked
//output: TRUE/FALSE
BYTE Vipor_GetVideoLocked(void)
{
	gbTrmBuf[0] = Vipor_VD_Read_Byte(0x10);	// 0x10 MVD_STATUS0
	//  1xx0xxxx bit7=1-->Hsync Lock , bit4=0-->Signal Lock
	if ((gbTrmBuf[0] & 0x90) == 0x80)	return TRUE;	// Locked
	else	
    	 return FALSE;	// Unlock
}

//get video mode
//output: EN_VID_MODE_PAL/EN_VID_MODE_NTSC
BYTE Vipor_GetVideoMode(void)
{
	gbTrmBuf[0] = Vipor_VD_Read_Byte(0x10);	// 0x10 MVD_STATUS0
	if ((gbTrmBuf[0] & 0x07) == 0x02)	return EN_VID_MODE_PAL;
	else
	 return EN_VID_MODE_NTSC;
}

void Vipor_SetPowerDown(void)
{
#define u8Value  __bViporTemp6	
printf("Vipor_SetPowerDown");
       u8Value  = Vipor_SC_Read_Byte(0x07)  | 0x10;
       VP_IIC_Write(0xF4, 0x07, u8Value);
//       VP_IIC_Write(0xF4, 0x28, 0x04);
//       VP_IIC_Write(0xF4, 0x25, 0x04);   
#undef u8Value  
}   
void Vipor_SetPowerON(void)
{
#define u8Value  __bViporTemp6	
printf("Vipor_SetPowerON");
       u8Value  = Vipor_SC_Read_Byte(0x07) & 0xEF;
       VP_IIC_Write(0xF4, 0x07, u8Value);
//       VP_IIC_Write(0xF4, 0x28, 0x04);
//       VP_IIC_Write(0xF4, 0x25, 0x04);
#undef u8Value  
}     

#ifdef LED_BackLight

#ifdef VP_API_CT908
#pragma NOAREGS
#endif
void Vipor_SetBlackLight_ON(void)
{

    if (_bBacklight!= 1)
    { 
      printf("BackLight_ON\n");
      Vipor_WriteRegister(Vipor_LED_BackLight_ON);
      _bBacklight = 1;
     }
}
#ifdef VP_API_CT908
#pragma NOAREGS
#endif
void Vipor_SetBlackLight_OFF(void)
{
    if (_bBacklight!= 0)
    { 
      printf("BackLight_OFF\n");
      Vipor_WriteRegister(Vipor_LED_BackLight_OFF);
      _bBacklight = 0;
     }      
//	  	Vipro_GammaEnable(1);
}
#else

#ifdef VP_API_CT908
#pragma NOAREGS //may be called by ISR  // KCHong, 20060717
#endif
void Vipor_SetBlackLight_ON(void)
{
  ;
}
#ifdef VP_API_CT908
#pragma NOAREGS //may be called by ISR  // KCHong, 20060717
#endif
void Vipor_SetBlackLight_OFF(void)
{
	 ;
}
#endif
//set blue screed
#ifdef VP_API_CT908
#pragma NOAREGS //may be called by ISR  // KCHong, 20060717
#endif
void Vipor_SetBlackScreen(void)
{
#define u8Value  __bViporTemp6	
       Vipor_SetBlackLight_OFF();	     
//printf("Vipor_SetBlackScreen");
       VP_IIC_Write(0xF4, 0x88, 00);
       VP_IIC_Write(0xF4, 0x89, 00);
       VP_IIC_Write(0xF4, 0x8A, 00);
       u8Value  = (Vipor_SC_Read_Byte(0x05) & 0xE0) | 0x10;
       VP_IIC_Write(0xF4, 0x05, u8Value);
/*
	Delay_mS(20);	// ????
	Vipor_SC_Write_Byte(0x05, 0x10);	// Force Background Color
	Vipor_SC_Write_Byte(0x09, 0x01);	// Scaler Free Run
	*/
#undef u8Value
}

//set normal output
#ifdef VP_API_CT908
#pragma NOAREGS //may be called by ISR  // KCHong, 20060717
#endif
void Vipor_SetNormalOutput(void)
{
#define u8Value  __bViporTemp6		
//printf("Vipor_SetNormalOutput");
       Vipor_SetBlackLight_ON();	  
       u8Value = Vipor_SC_Read_Byte(0x05) & 0xE0;
       VP_IIC_Write(0xF4, 0x05, u8Value);
/*       
	Vipor_SC_Write_Byte(0x05, 0x00);	// Force Background Color
	Vipor_SC_Write_Byte(0x09, 0x03);	// Scaler Free Run
	*/
#undef u8Value
}

void Vipro_LoadGammaTable(BYTE *dptr)
{
#define j __bViporTemp11
#define k  __bViporTemp12
#define bDt  __bViporTemp13

__dwVersionTFT = ( (TFT_SW_VERSION<<16) | (TFT_PANEL_VERSION<<8) | TFT_PANEL_MINOR_VERSION);    //Version Setting
	//printf("Load Gamma Table ... ");
  k = Vipor_SC_Read_Byte(0x05); 
  //k = gbTrmBuf[0];
  bDt = 0;
	Vipor_SC_Write_Byte(0x80, 0x00);
	Vipor_SC_Write_Byte(0x05,k & ~0x40);
	Vipor_SC_Write_Byte(0x05,k |  0x40);
	Vipor_SC_Write_Byte(0x05,k & ~0x40);
    for(j=0 ; j<16 ; j++)
    {
        for(k=0 ; k<16 ; k++)
        {
#ifdef TFT_ColorInverse
        	{
        	  Vipor_SC_Write_Byte(0x81, ~(*dptr++)); // VPSC_GR_WADDR
            Vipor_SC_Write_Byte(0x82, ~(*dptr++)); // VPSC_GG_WADDR
            Vipor_SC_Write_Byte(0x83, ~(*dptr++)); // VPSC_GB_WADDR
          }
#else
          {  
            Vipor_SC_Write_Byte(0x81, *dptr++); // VPSC_GR_WADDR
            Vipor_SC_Write_Byte(0x82, *dptr++); // VPSC_GG_WADDR
            Vipor_SC_Write_Byte(0x83, *dptr++); // VPSC_GB_WADDR
        }
#endif          
        }
    }
    Vipor_SC_Read_Byte(0x05); 
    k = gbTrmBuf[0];
    Vipor_SC_Write_Byte(0x05,k |  0x80);
		// Gamma Enable	
	  //printf("OK\n");
#undef j
#undef k
#undef bDt 
}

void Vipro_GammaEnable(BYTE fEnable)
{
#define bData  __bViporTemp1
	bData = Vipor_SC_Read_Byte(0x05);
	//printf("Vipro_GammaEnable  %x",bData);
	if(fEnable)
	{
		bData |= 0x80;
	}
	else
	{
		bData &= 0x7F;
	}
	Vipor_SC_Write_Byte(0x05, bData);
#undef bData
}

void Vipor_LoadGammaTable(void)
{
	Vipor_SC_Write_Byte(0x09, 0x03);
	Vipro_LoadGammaTable((BYTE *)&Vipor_GammaTable);
	Vipro_GammaEnable(1);
	Vipor_SC_Write_Byte(0x09, 0x02);
}

// brief Set panle flip type, see EN_PANEL_FLIP, the default is EN_PANEL_FLIP_NORMAL.
// param enType (Input) the panel flip type.
void Vipor_SetFlipType(BYTE enType)
{
    if(enType == EN_PANEL_FLIP_NORMAL)
    {
    	HAL_WriteGPIO( GPIO_D, 10, 1);      // LRC=1
    	HAL_WriteGPIO( GPIO_D, 11, 0);      // UDC=0 
        Vipor_TC_Write_Byte(0x02, 0x00);    // STHL:Output , STHR:High Impedance
        Vipor_TC_Write_Byte(0x05, 0x10);    // STVU:Output , STVD:High Impedance
    }
    else if(enType == EN_PANEL_FLIP_H)
    {
    	HAL_WriteGPIO( GPIO_D, 10, 0);      // LRC=0
    	HAL_WriteGPIO( GPIO_D, 11, 0);      // UDC=0 
        Vipor_TC_Write_Byte(0x02, 0x04);    // STHR:Output , STHL:High Impedance
        Vipor_TC_Write_Byte(0x05, 0x10);    // STVU:Output , STVD:High Impedance
    }
    else if(enType == EN_PANEL_FLIP_V)
    {
    	HAL_WriteGPIO( GPIO_D, 10, 1);      // LRC=1
    	HAL_WriteGPIO( GPIO_D, 11, 1);      // UDC=1 
        Vipor_TC_Write_Byte(0x02, 0x00);    // STHL:Output , STHR:High Impedance
        Vipor_TC_Write_Byte(0x05, 0x90);    // STVD:Output , STVU:High Impedance
    }
    else    // EN_PANEL_FLIP_H_V
    {
    	HAL_WriteGPIO( GPIO_D, 10, 0);      // LRC=0
    	HAL_WriteGPIO( GPIO_D, 11, 1);      // UDC=1 
        Vipor_TC_Write_Byte(0x02, 0x04);    // STHR:Output , STHL:High Impedance
        Vipor_TC_Write_Byte(0x05, 0x90);    // STVD:Output , STVU:High Impedance
    }
}
// SineLUT[degree] = Sine(degree) * 256
unsigned char code SineLUT[] =
{
  /*  0     1     2     3     4     5     6     7     8     9  */
    0x00, 0x04, 0x08, 0x0D, 0x11, 0x16, 0x1A, 0x1F, 0x23, 0x28,     // 0
    0x2C, 0x30, 0x35, 0x39, 0x3D, 0x42, 0x46, 0x4A, 0x4F, 0x53,     // 1
    0x57, 0x5B, 0x5F, 0x64, 0x68, 0x6C, 0x70, 0x74, 0x78, 0x7C,     // 2
    0x7F, 0x83, 0x87, 0x8B, 0x8F, 0x92, 0x96, 0x9A, 0x9D, 0xA1,     // 3
    0xA4, 0xA7, 0xAB, 0xAE, 0xB1, 0xB5, 0xB8, 0xBB, 0xBE, 0xC1,     // 4
    0xC4, 0xC6, 0xC9, 0xCC, 0xCF, 0xD1, 0xD4, 0xD6, 0xD9, 0xDB,     // 5
    0xDD, 0xDF, 0xE2, 0xE4, 0xE6, 0xE8, 0xE9, 0xEB, 0xED, 0xEE,     // 6
    0xF0, 0xF2, 0xF3, 0xF4, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB,     // 7
    0xFC, 0xFC, 0xFD, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF      // 8
};
WORD code sRGB_Matrix[][3] =
{
#ifdef sRGB_HDTV
    // *** HDTV ***
    // R=Y                 + 1.540(Cr-128)
    // G=Y - 0.183(Cb-128) - 0.459(Cr-128)
    // B=Y + 1.816(Cb-128)
    { 0x0100, 0x0000, 0x018A },
    { 0x0100, 0x802E, 0x8075 },
    { 0x0100, 0x01D1, 0x0000 }      // 1.816*1024/4=465=0x1D1
#elseif sRGB_SDTV_Computer   //SDTV Computer(0,255)
   // R=1.164(Y-16)+1.596(Cr-128)
   // G=1.164(Y-16)-0.391(Cb-128)-0.813(Cr-128)
   // B=1.164(Y-16)+2.018(Cb-128)
    { 0x012A, 0x0000, 0x0199 },
    { 0x012A, 0x079C, 0x0730 },
    { 0x012A, 0x0205, 0x0000 }
#else
    // *** SDTV ***
    // R=Y                 + 1.403(Cr-128)
    // G=Y - 0.344(Cb-128) - 0.714(Cr-128)
    // B=Y + 1.770(Cb-128)
    { 0x0100, 0x0000, 0x0167 },
    { 0x0100, 0x8058, 0x80B6 },
    { 0x0100, 0x01C5, 0x0000 }
#endif    
};
//--------------------------------------------------------------------------
// I/P : u8VideoContrast              0 ... 128 ... 255
//       u8VideoHue          -90 (0xA6) ...  0  ... 90 (0x5A)
//       u8VideoSaturation            0 ... 128 ... 255
//       u8ZRgain                     0 ... 128 ... 255     ** walter Add
//       u8ZGgain                     0 ... 128 ... 255     **  for RGB gain control
//       u8ZBgain                     0 ... 128 ... 255   
//       u8ZRoffset                   0 ... 128 ... 255
//       u8ZGoffset                   0 ... 128 ... 255
//       u8ZBoffset                   0 ... 128 ... 255
void ViporBCAdjust(void)
{
#define   fg_hue_sign __bViporTemp1
#define   fg_mx_sign __bViporTemp2
//    BOOL fg_hue_sign, fg_mx_sign;
#define   it __bViporTemp3
//    BYTE it;
    WORD sinct, cosct, ucoeff, vcoeff, wval;

    //printf("u8VideoSaturation = %hx\n",u8VideoSaturation);
    //printf("u8VideoContrast = %hx\n",u8VideoContrast);
    u8VideoHue=(BYTE)((WORD)(u8VideoHue-128)*90/128);
    Vipor_SC_Write_Byte(0xE0, 0x80);        // REG_SC_SRGB_CTRL=0xE0
    if(u8VideoHue >= 0x80)
    {
        fg_hue_sign = 1;
        it = 0xFF - u8VideoHue + 1;
    }
    else
    {
        fg_hue_sign = 0;
        it = u8VideoHue;
    }
//   Walter:  for boundary check
    if((u8VideoHue > 0x5A)&&(u8VideoHue < 0xA6))
    {
    	fg_hue_sign = 0;
    	it = 0;
    	} 

    sinct = (it == 90) ? 0x100 : (WORD)(SineLUT[it]);
    cosct = (it ==  0) ? 0x100 : (WORD)(SineLUT[(90 - it)]);
    
    sinct = (WORD)(((DWORD)sinct * u8VideoSaturation) >> 7);	// (0..255)/128 = 0..2
    cosct = (WORD)(((DWORD)cosct * u8VideoSaturation) >> 7);
    for(it = 0; it < 3; it++)
    {
        // *** Coeff Y ***
        wval = (WORD)(((DWORD)(sRGB_Matrix[it][0]) * (u8VideoContrast)) >> 7);

        switch(it) 
        {
           case 0:  wval = (WORD)(((DWORD)wval * Temperature_Adjust[0][0]) >> 7);
                   break;
           case 1:  wval = (WORD)(((DWORD)wval * Temperature_Adjust[0][1]) >> 7);
                   break;
           case 2:  wval = (WORD)(((DWORD)wval * Temperature_Adjust[0][2]) >> 7);
                                    	
           	}
           	
        wval = wval & 0x07ff;
        // API_SC_WriteWord((0xE2 + (it * 6)), wval);   // REG_SC_SRGB_RCOEFF1=0xE2
        Vipor_SC_Write_Byte((BYTE)(0xE2 + (it * 6)), (BYTE)(wval & 0xFF));
        Vipor_SC_Write_Byte((BYTE)(0xE3 + (it * 6)), (BYTE)(wval >> 8));
        // printf("API_SC_WriteWord(%02X, %04X)\n", (0xE2 + (it * 6)), wval);
	 	    //printf("%04X  ", wval);
        // *** Coeff U ***
        ucoeff = (WORD)((((DWORD)(sRGB_Matrix[it][1] & 0x07FF)) * cosct) >> 8);
        vcoeff = (WORD)((((DWORD)(sRGB_Matrix[it][2] & 0x07FF)) * sinct) >> 8);
        fg_mx_sign = 0;

        if(fg_hue_sign)
        {	// if Hue<0 then SIN(hue)<0    // inverse the rotate angle  8/28 walter
            if(ucoeff > vcoeff)
            {
            	wval = ucoeff - vcoeff;
            }
            else
            {
                if(vcoeff > 0)  fg_mx_sign = 1;
                wval = vcoeff - ucoeff;
            }
        }
        else
        {      //  Hue > 0
            wval = ucoeff + vcoeff;
        }
        
        switch(it) 
        {
           case 0:  wval = (WORD)(((DWORD)wval * Temperature_Adjust[0][0]) >> 7);
                   break;
           case 1:  wval = (WORD)(((DWORD)wval * Temperature_Adjust[0][1]) >> 7);
                   break;
           case 2:  wval = (WORD)(((DWORD)wval * Temperature_Adjust[0][2]) >> 7);
                                    	
        }

//        wval = wval << 1;

        if(it == 1)		// G channel UV is minus
        {
            fg_mx_sign = ~fg_mx_sign;
        }

        if(fg_mx_sign)
        {
            wval = - wval;
        }       

        wval = wval & 0x07ff;
 
        
        // API_SC_WriteWord((0xE4 + (it * 6)), wval);   // REG_SC_SRGB_RCOEFF2=0xE4
        Vipor_SC_Write_Byte((BYTE)(0xE4 + (it * 6)), (BYTE)(wval & 0xFF));
        Vipor_SC_Write_Byte((BYTE)(0xE5 + (it * 6)), (BYTE)(wval >> 8));
        //printf("API_SC_WriteWord(%02X, %04X)\n", (0xE4 + (it * 6)), wval);
	     	//printf("%04X  ", wval);

        // *** Coeff V ***      
        ucoeff = (WORD)((((DWORD)(sRGB_Matrix[it][1] & 0x07FF)) * sinct) >> 8);
        vcoeff = (WORD)((((DWORD)(sRGB_Matrix[it][2] & 0x07FF)) * cosct) >> 8);
        fg_mx_sign = 0;

        if(fg_hue_sign)
        {                             // inverse rotate angle   8/28 walter 
          wval = vcoeff + ucoeff; 
        }
        else
        {
            if(ucoeff < vcoeff)
            {
                wval = vcoeff - ucoeff;
            }
            else
            {
                if(ucoeff > 0)
                {
                    fg_mx_sign = 1;
                }
                wval = ucoeff - vcoeff;
            }
        }
       
        switch(it) 
        {
           case 0:  wval = (WORD)(((DWORD)wval * Temperature_Adjust[0][0]) >> 7);
                   break;
           case 1:  wval = (WORD)(((DWORD)wval * Temperature_Adjust[0][1]) >> 7);
                   break;
           case 2:  wval = (WORD)(((DWORD)wval * Temperature_Adjust[0][2]) >> 7);
                                    	
           	}

 //       wval = wval << 1;

        if(it == 1)		// G channel UV is minus
        {
            fg_mx_sign = ~fg_mx_sign;
        }

        if(fg_mx_sign)
        {
            wval = - wval;
        }
        wval = wval & 0x07ff;
     
        
        // API_SC_WriteWord((0xE6 + (it * 6)), wval);   // REG_SC_SRGB_RCOEFF3=0xE6
        Vipor_SC_Write_Byte((BYTE)(0xE6 + (it * 6)), (BYTE)(wval & 0xFF));
        Vipor_SC_Write_Byte((BYTE)(0xE7 + (it * 6)), (BYTE)(wval >> 8));
        // printf("API_SC_WriteWord(%02X, %04X)\n", (0xE6 + (it * 6)), wval);
	    	//printf("%04X\n", wval);
	
        //Vipor_SC_Write_Byte (0xE1,0x80+u8VideoBrightness);  	// VIPOR_BLACK
	Vipor_SC_Write_Byte (0xF4,0x80+Temperature_Adjust[1][0]);		// R brightness(0xf4)
	Vipor_SC_Write_Byte (0xF5,0x80+Temperature_Adjust[1][1]);		// G brightness(0xf5)
	Vipor_SC_Write_Byte (0xF6,0x80+Temperature_Adjust[1][2]);		// B brightness(0xf6)

	
    }
#undef   fg_hue_sign 
#undef   fg_mx_sign
#undef   it     
}
