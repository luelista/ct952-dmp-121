#ifdef _WINDVD
//	#include "..\WinAV.H"
//	#include "..\SetUp.H"
#ifdef VP_API_CT909	
  #include "..\Hio.H"
#endif
	extern	void _printf(char *fmt, ...);
	//extern	DWORD UTL_GetSysTimer(void);
#else
	#include "defs.H"
#endif


#include "ViporOsdFont.h"

#include "ViporVariable.h"
#include "ViporOSD.h"

extern	BYTE Vipor_SC_Read_Byte(BYTE bSubAddr);
extern	void Vipor_SC_Write_Byte(BYTE bSubAddr, BYTE bData);
extern	void Vipor_Write_Burst (BYTE bDevAddr, BYTE bSubAddr, BYTE bCount);
//extern	BYTE bTrmBuf[8];

	#define		VP_OSD_LUT_RELOAD		0x86	
	#define		VP_OSD_GRAPHIC_START	0xC0
	#define		VP_OSD_GRAPHIC_END		0xC1	
  #define		VP_OSD_FONT_ADDR		0xC2
	#define		VP_OSD_FONT_DATA_LOW	0xC3
	#define		VP_OSD_FONT_CODE_ATTR	0xC5
	#define		VP_OSD_FONT_CODE_ADDR	0xC6  
	#define		VP_OSD_DISP_RAM_ADDR_0	0xC7
	#define		VP_OSD_DISP_RAM_ADDR_1	0xC8	
  #define		VP_OSD_LUT_DATA			0x84
	#define		VP_OSD_SYS_CTRL			0xCA	
	#define		VP_OSD_SP_DEF			0xCB
	#define		VP_OSD_START_ROW		0xCC
	#define		VP_OSD_H_POSITION		0xCD
	#define		VP_OSD_V_POSITION		0xCE
	#define		VP_OSD_CTRL_REG_2		0xCF
	#define		VP_OSD_CTRL_REG			0xD0	
	#define		VP_OSD_DISP_RAM_ADDR_2	0xC9
	#define		VP_OSD_DISP_OPTION		0xD1	
	#define		VP_OSD_LUT_ADDR			0x86
	#define		VP_OSD_WIN_ADDR			0xD2
	#define		VP_OSD_WIN_DATA			0xD3


//--------------------------------------------------------------------------
//	Function  : Vipor_OsdLoadFont
//	Purpose   : Load font code into Vipor internal OSD
//--------------------------------------------------------------------------
void Vipor_LoadOsdFont(void)
{
#define i __bViporTemp11
#define j __bViporTemp12
#define k __bViporTemp13
#define l __bViporTemp14
#define bNum __bViporTemp5
	BYTE  code *pRomData;
        bNum = 1;  
	pRomData = (BYTE*) &OsdFontData;
//	if(bNum == 0) return;

//	printf("Vipor Load OSD font...");
	for(bNum=0 ; bNum<=(sizeof(OsdFontData)/24) ; bNum++)
	{
		pRomData = OsdFontData + ((WORD)bNum*24);
		Vipor_SC_Write_Byte(VP_OSD_FONT_ADDR, bNum);  //CT675

		for( i=0; i<8; i++ )
		{
			j = *pRomData++;
			k = *pRomData++;
			l = *pRomData++;

			gbTrmBuf[0] = (j << 4) + (k >> 4);
			gbTrmBuf[1] = (j >> 4);
			Vipor_Write_Burst(0xF4, VP_OSD_FONT_DATA_LOW, 2);	// VPSC_FONT_LSB; //CT675
			
			gbTrmBuf[0] = l;
			gbTrmBuf[1] = (k & 0x0F);
			Vipor_Write_Burst(0xF4, VP_OSD_FONT_DATA_LOW, 2);	// VPSC_FONT_LSB; //CT675
		}
	}
	//printf("LOAD_OSD_FONT OK\n");
	
#undef i
#undef j
#undef k
#undef l
#undef bNum

}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Color-Look-Up table definition :
// 1.group index
//    R  G  B  I      index
//   -----------------------------
//    0  0  0  0  --  0, BLACK
//    0  0  0  1  --  1, BLACK
//    0  0  1  0  --  2, BLUE
//    0  0  1  1  --  3, BLUE(I)
//    0  1  0  0  --  4, GREEN
//    0  1  0  1  --  5, GREEN(I)
//    0  1  1  0  --  6, CYAN
//    0  1  1  1  --  7, CYAN(I)
//    1  0  0  0  --  8, RED
//    1  0  0  1  --  9, RED(I)
//    1  0  1  0  --  A, MAGENTA
//    1  0  1  1  --  B, MAGENTA(I)
//    1  1  0  0  --  C, YELLOW
//    1  1  0  1  --  D, YELLOW(I)
//    1  1  1  0  --  E, WHITE
//    1  1  1  1  --  F, WHITE(I)
//   ------------------------------
//   2.group position
//         RRRR,RGGG,GGaB,BBBB		'a' means Alpha-blending
//--------------------------------------------------------------------------
BYTE code ColorLookupTable[] =
{
#if 1
              //            RRRR RGGG GGaB BBBB
//  0x00,0x20,  // 0, BLACK	0000 0000 0010 0000  00 20
//  0x08,0x51,
  0x00,0x20,  // 1, black	0000 1000 0100 0001      0
  0x00,0x00,  // 1, black	0000 1000 0100 0001 
  0x08,0x6D,  // 2, BLUE	0000 1000 0111 0001      2
  0x08,0x4D,  // 3, blue	0000 1000 0101 1101      4 
  0x0E,0xE1,  // 4, GREEN   0000 1110 1110 0001    8
  0x0E,0xC1,  // 5, green                          A
  0x0C,0x71,  // 6, CYAN	0111 0001 0000 1100      C 
  0x0C,0x51,  // 7, cyan                           E
  0x88,0x61,  // 8, RED                            10
  0x88,0x41,  // 9, red                            12
  0x88,0x71,  // A, MAGENTA                        14
  0x88,0x51,  // B, magenta                        18
  0xFF,0xA1,  // C, YELLOW                         1A
  0xFF,0x81,  // D, yellow                         18
  //0x8C,0x51,  // E, WHITE	1000 1100 0101 0001    1E
  0xFF,0xFE,  // E, white	1111 1111 1111 1110       
  0xFF,0xDE,  // F, white	1111 1111 1101 1110//  0x00,0x20,  // 0, BLACK	0000 0000 0010 0000  00 20
#else
  0xDF, 0xDF,  		// 0  RRRR RGGG GGaB BBBB    RRRRR GGGGG BBBBB
  0xDF, 0xFF,  		// 1  1111 1000 0001 1111    11111 00000 11111
  0xD7, 0xAE,  		// 2  R
  0xD7, 0xA2,  		// 3
  0xD1, 0x1E,  		// 4  G
  0xD3, 0xBE,  		// 5
  0xD3, 0x8E,  		// 6  B --> 0
  0xD3, 0xAE,  		// 7
  0x57, 0x9E,  		// 8  RG
  0x57, 0xBE,  		// 9
  0x57, 0x8E,  		// A  RB
  0x57, 0xAE,  		// B
  0x29, 0x5E,  		// C  GB
  0x20, 0x3E,  		// D
  0x53, 0xAE,  		// E  RGB
  0x20, 0x20,		// F
#endif
  0
};

//--------------------------------------------------------------------------
void Vipor_LoadColorLookupTable(void)
{
  #define i __bViporTemp11
	Vipor_SC_Write_Byte(0x09, 0x03);
	i = Vipor_SC_Read_Byte(VP_OSD_LUT_RELOAD);			// VPSC_MISCTR1
  //printf("Read LookUp Table %d\n",i);
	Vipor_SC_Write_Byte(VP_OSD_LUT_ADDR,0x00);			// VPSC_OSD_LUT_ADDR(0xF3) CLUT starting address  //Jeff 20060504
	Vipor_SC_Write_Byte(VP_OSD_LUT_RELOAD, i & 0x7F);
	Vipor_SC_Write_Byte(VP_OSD_LUT_RELOAD, i | 0x80);
	Vipor_SC_Write_Byte(VP_OSD_LUT_RELOAD, i & 0x7F);

	// *** Send Color-Lookup-Table[0xB5] ***
	for(i=0; i<16; i++)
	{
#ifdef TFT_ColorInverse
		Vipor_SC_Write_Byte(VP_OSD_LUT_DATA, ~ColorLookupTable[i*2+1]);   // Low  byte	VPSC_CLUT_WADDR(0xB5)
		//printf("LOOKUP => %x\n",ColorLookupTable[i*2]^0xDF);
		//printf("LOOKUP => %x\n",~ColorLookupTable[i*2+1]);
		Vipor_SC_Write_Byte(VP_OSD_LUT_DATA, ColorLookupTable[i*2]^0xDF);   // High byte	 
#else
		Vipor_SC_Write_Byte(VP_OSD_LUT_DATA, ColorLookupTable[i*2+1]);   // Low  byte	VPSC_CLUT_WADDR(0xB5)
		Vipor_SC_Write_Byte(VP_OSD_LUT_DATA, ColorLookupTable[i*2]);   // High byte
#endif		 
	}
	Vipor_SC_Write_Byte(0x09, 0x03);
	
  #undef i	
}

//--------------------------------------------------------------------------
// Function    : Vipor_OsdDisableWindow
// Description : disable a OSD window
// Input       : bIndex -> Window index (0-3)
//--------------------------------------------------------------------------
void Vipor_OsdDisableWindow(BYTE bIndex)
{
	bIndex *= 5;
	Vipor_SC_Write_Byte(VP_OSD_WIN_ADDR, bIndex+2);	// VPSC_OSD_WIN_ADDR
	Vipor_SC_Write_Byte(VP_OSD_WIN_DATA, 0x08);		// VPSC_OSD_WIN_DATA
}

//--------------------------------------------------------------------------
// Function    : Vipor_OsdSetRowAttr
// Description : set OSD row attribute
// Input       :     row -> select row
//               attr_hi -> high attribute
//               attr_lo -> low  attribute
//--------------------------------------------------------------------------
void Vipor_OsdSetRowAttr(BYTE bRow, BYTE bAttrHigh, BYTE bAttrLow)
{
	if(bRow < 8)		Vipor_SC_Write_Byte(VP_OSD_DISP_RAM_ADDR_0, bRow<<5);			// VPSC_OSD_ADR0
	else if(bRow < 16)	Vipor_SC_Write_Byte(VP_OSD_DISP_RAM_ADDR_1, (bRow-8)<<5);		// VPSC_OSD_ADR1
	else				Vipor_SC_Write_Byte(VP_OSD_DISP_RAM_ADDR_2, (bRow-16)<<5);	// VPSC_OSD_ADR2

    // Write attribute
    Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ATTR, bAttrLow);	// VPSC_OSD_AT
    Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, bAttrHigh);	// VPSC_OSD_DT
}

//--------------------------------------------------------------------------
// Function    : Vipor_OsdSetAddress
// Description : Set OSD active RAM index address
// Input       : bRow -> Y position
//               bCol -> X position
//--------------------------------------------------------------------------
void Vipor_OsdSetAddress(BYTE bRow, BYTE bCol)
{
	if(bRow < 8)
	{
		bRow = (bRow<<5) + bCol + 1;
		Vipor_SC_Write_Byte(VP_OSD_DISP_RAM_ADDR_0, bRow);	// VPSC_OSD_ADR0
	}
	else if( bRow < 16 )
	{
		bRow = ((bRow-8)<<5) + bCol+1;
		Vipor_SC_Write_Byte(VP_OSD_DISP_RAM_ADDR_1, bRow);	// VPSC_OSD_ADR1
	}
	else
	{
		bRow = ((bRow-16)<<5) + bCol+1;
		Vipor_SC_Write_Byte(VP_OSD_DISP_RAM_ADDR_2, bRow);	// VPSC_OSD_ADR2
	}
}

//--------------------------------------------------------------------------
// Function    : Vipor_OsdClearRow
// Description : clear row data
// Input       : bVstart -> startRow
//               bVend   -> endRow
//--------------------------------------------------------------------------
void Vipor_OsdClearRow(BYTE bVstart, BYTE bVend, BYTE bAttr)
{
	//BYTE i;
  #define i __bViporTemp11
  #define j __bViporTemp12
  	Vipor_SC_Write_Byte(0x09, 0x03);
  
	Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ATTR, bAttr);		// (VPSC_OSD_AT) Set attribute

	for(i=bVstart ; i<=bVend ; i++)
	{
		Vipor_OsdSetAddress(i, 0);				// Set starting address

	for(j=0 ; j<7 ; j++) 	 
		Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, _VIPOR__);
	for(j=0 ; j<7 ; j++) 	 	
		Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, _VIPOR__);
	for(j=0 ; j<7 ; j++) 	 	
		Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, _VIPOR__);	
	for(j=0 ; j<7 ; j++) 	 	
		Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, _VIPOR__);	
	for(j=0 ; j<3 ; j++) 	 
		Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, _VIPOR__);
		}    
		Vipor_SC_Write_Byte(0x09, 0x02);
	#undef i
	#undef j
}

//--------------------------------------------------------------------------
// Function    : VPP_OsdClearArea
// Description : clear area data
// Input       : (bHstart,bVstart) -> (x1,y1)
//               (bHend  , bVend ) -> (x2,y2)
//--------------------------------------------------------------------------
void VPP_OsdClearArea(BYTE bVstart, BYTE bVend, BYTE bHstart, BYTE bHend, BYTE bAttr)
{
	#define i __bViporTemp12
  #define j __bViporTemp13
  #define k __bViporTemp14
	for(k=0 ; k<8 ; k++) 
	   gbTrmBuf[k] = 0x00;
	Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ATTR, bAttr);		// (VPSC_OSD_AT) Set attribute

	for(i=bVstart ; i<=bVend ; i++)
	{
		Vipor_OsdSetAddress(i, bHstart);   // set starting address

		for(j=0; j<=(bHend - bHstart)/7; j++)
		{
     	for(k=0 ; k<7 ; j++) 	 	
		   Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, _VIPOR__);				
		}
		for (k=0;k<=(bHend - bHstart)%7;k++)
		   Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, _VIPOR__);
    }
  #undef i
  #undef j
  #undef k
}
//--------------------------------------------------------------------------
// Function    : Vipor_OsdPositionSet
// Description : Set OSD display position
// Input       : (hpos,vpos) -> (x,y)
//--------------------------------------------------------------------------
void Vipor_OsdPositionSet(BYTE bHpos, BYTE bVpos)
{
	Vipor_SC_Write_Byte(VP_OSD_H_POSITION, bHpos);	// VPSC_OSD_HPOSITION
	Vipor_SC_Write_Byte(VP_OSD_V_POSITION, bVpos);	// VPSC_OSD_VPOSITION
}

//--------------------------------------------------------------------------
// Function    : PrintString
// Description : Print a string in OSD. The  pRomData pointer is first character
// Input       : pRomData  -> array of string head
//               (col,row) -> (x,y) Print position
//--------------------------------------------------------------------------
void Vipor_PrintString(BYTE bRow, BYTE bCol, BYTE bColor, BYTE code *pRomData)
{
	#define i __bViporTemp13
  #define bData __bViporTemp14
  //#define bx __bViporTemp3  
 
 	if(*pRomData == _VIPOR_EOF)  return;

	bRow &= 0x0F;							// mask off other attribute
	Vipor_OsdSetAddress(bRow, bCol);		// set starting address
	Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ATTR, bColor);		// (VPSC_OSD_AT)set attribute
	
	for(i=0 ; *(pRomData+i)!=_VIPOR_EOF ; i++)
	{
		bData = *(pRomData+i);
		Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, bData);		// (VPSC_OSD_DT) //909R
		bCol++;
		if( bCol>=30 )  break;    			// avoid overflow
	}

	#undef i 
  #undef bData
  //#undef bx 
}
//#ifdef SetPicture_ALL
void Vipor_PrintString_ALL(BYTE bRow, BYTE bCol, BYTE bColor, BYTE *pRomData)
{
	#define i __bViporTemp13
  #define bData __bViporTemp14
  //#define bx __bViporTemp3  
 
 	if(*pRomData == _VIPOR_EOF)  return;

	bRow &= 0x0F;							// mask off other attribute
	Vipor_OsdSetAddress(bRow, bCol);		// set starting address
	Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ATTR, bColor);		// (VPSC_OSD_AT)set attribute
	
	for(i=0 ; *(pRomData+i)!=_VIPOR_EOF ; i++)
	{
		bData = *(pRomData+i);
		Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, bData);		// (VPSC_OSD_DT) //909R
		bCol++;
		if( bCol>=30 )  break;    			// avoid overflow
	}
	
	#undef i 
  #undef bData
  //#undef bx 
}
//#endif
//--------------------------------------------------------------------------
// Function    : PrintChar
// Description : Print a char
// Input       : bChar -> character
//               (col,row) -> (x,y) Print position
//--------------------------------------------------------------------------
void Vipor_PrintChar(BYTE bRow, BYTE bCol, BYTE bColor, BYTE bChar)
{
	Vipor_OsdSetAddress(bRow, bCol);		// set starting address

	//Vipor_SC_Write_Byte(0x59, bColor);		// (VPSC_OSD_AT)set attribute
	//Vipor_SC_Write_Byte(0x5A, bChar);		// (VPSC_OSD_DT)
  Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ATTR, bColor);		// (VPSC_OSD_AT)set attribute
	Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, bChar);		// (VPSC_OSD_DT)
}

#if 1 
//--------------------------------------------------------------------------
// Function    : Vipor_SetFirstCharAttr
// Input        :  (bCol,bRow) -> first char position
//                 : bColor -> first char attr
//--------------------------------------------------------------------------
void Vipor_SetFirstCharAttr(BYTE bRow, BYTE bCol, BYTE bColor)
{
	bRow &= 0x0F;							// mask off other attribute
	Vipor_OsdSetAddress(bRow, bCol);		// set starting address
	//Vipor_SC_Write_Byte(0x59, bColor);		// (VPSC_OSD_AT)set attribute	
	Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ATTR, bColor);		// (VPSC_OSD_AT)set attribute
}

//--------------------------------------------------------------------------
// Function    : Vipor_SetOneCharContent
// Input        : bChar -> char 
// note         : use with Vipor_SetFirstCharAttr
//--------------------------------------------------------------------------
void Vipor_SetOneCharContent(BYTE bChar)
{
  //Vipor_SC_Write_Byte(0x5A, bChar);		// (VPSC_OSD_DT)
	Vipor_SC_Write_Byte(VP_OSD_FONT_CODE_ADDR, bChar);		// (VPSC_OSD_DT)
}
#endif


//**************************************************************************
// Function    : OsdWindow
// Description : Open a OSD window
// Input       : num              -> OSD window index
//               (x0, y0, x1, y1) -> define window area
//               color            -> define window color
//**************************************************************************
void OsdWindow(BYTE num, BYTE x0, BYTE y0, BYTE x1, BYTE y1, BYTE color)
{
	#define bval __bViporTemp13
  #define idx __bViporTemp14
    //BYTE bval,idx;

   // idx = (num-1) * 5;
   idx = num * 5;

    Vipor_SC_Write_Byte(VP_OSD_WIN_ADDR,idx+1);
    Vipor_SC_Write_Byte(VP_OSD_WIN_DATA,0x08);

    bval = y0;                            // window starting adress
    Vipor_SC_Write_Byte(VP_OSD_WIN_ADDR,idx);           // Vstart
    Vipor_SC_Write_Byte(VP_OSD_WIN_DATA,bval);

    bval = y1;                            // window starting adress
    Vipor_SC_Write_Byte(VP_OSD_WIN_ADDR,idx+1);         // Vend
    Vipor_SC_Write_Byte(VP_OSD_WIN_DATA,bval);

    bval  = (x1 + 1) << 3;                // Hend
    bval |= (color >> 2);                 // R,G,B
    Vipor_SC_Write_Byte(VP_OSD_WIN_ADDR,idx+3);
    Vipor_SC_Write_Byte(VP_OSD_WIN_DATA,bval);

    bval  = (x0 + 1) << 3;                // Hstart
    bval |= 0x04;                         // Osd enable

    if( color & 0x02 )  bval |= 0x01;     // Intensity
    Vipor_SC_Write_Byte(VP_OSD_WIN_ADDR,idx+2);
    Vipor_SC_Write_Byte(VP_OSD_WIN_DATA,bval);
	#undef bval
  #undef idx
}



//--------------------------------------------------------------------------
// Function    : Vipor_OsdEnable
// Description : Enable/Disable osd display
// Input       : 1 -> Enable
//               0 -> Disable
//--------------------------------------------------------------------------
void Vipor_OsdEnable(BYTE flag)
{
	if(flag)
	{
		Vipor_SC_Write_Byte(VP_OSD_CTRL_REG, 0x1D);  // (VPSC_OSD_CONTROL) Set enable bit.
	}
	else
	{
		Vipor_SC_Write_Byte(VP_OSD_CTRL_REG, 0x1C);  // (VPSC_OSD_CONTROL) Set disable bit.
	}
}


void Vipor_OsdInit(void)
{
	#define i __bViporTemp5
	//BYTE i;
  //Vipor_OsdEnable(FALSE);
//printf("time 1 = %x\n", UTL_GetSysTimer());
	Vipor_LoadColorLookupTable();

//printf("time 2 = %x\n", UTL_GetSysTimer());
	Vipor_SC_Write_Byte(VP_OSD_SP_DEF, 0x5A);	// VPSC_OSD_SPDEF=01011010
										//                |/|/|||+---  0  0:Intensity color of space code
										//                |/|/||+----  1  1:Blue  color of space code
										//                |/|/|+-----  0  2:Green color of space code
										//                |/|/+------  1  3:Red   color of space code
										//                |/+-------- 01 54:Veritical  position step
										//                |/+-------- 01 76:Horziontal position step

//printf("time 3 = %x\n", UTL_GetSysTimer());
	Vipor_SC_Write_Byte(VP_OSD_SYS_CTRL, 0x00);	// VPSC_SYS_CONTROL
	Vipor_SC_Write_Byte(VP_OSD_START_ROW, 0x00);	// VPSC_OSD_STARTROW
	Vipor_SC_Write_Byte(VP_OSD_DISP_OPTION, 0x00);	// VPSC_SYS_CONTROL1
	Vipor_SC_Write_Byte(VP_OSD_CTRL_REG_2, 0x00);	// VPSC_OSD_CONTROL2
	Vipor_SC_Write_Byte(VP_OSD_CTRL_REG, 0x1C);	// VPSC_OSD_CONTROL
	//Vipor_SC_Write_Byte(VP_OSD_GRAPHIC_START, 0xFF);	// VPSC_GRAPHIC_START
	//Vipor_SC_Write_Byte(VP_OSD_GRAPHIC_END, 0xFF);		// VPSC_GRAPHIC_END

	Vipor_OsdDisableWindow(0);
	Vipor_OsdDisableWindow(1);
	Vipor_OsdDisableWindow(2);
	Vipor_OsdDisableWindow(3);

//printf("time 4 = %x\n", UTL_GetSysTimer());
	for(i = 0; i < 15; i++)
	{
#ifdef Digital_PANEL
		Vipor_OsdSetRowAttr(i, 0x00, 0x74);		// Normal
#else		
		Vipor_OsdSetRowAttr(i, 0x00, 0x14);		// Normal		
#endif		
	}	 
//printf("time 5 = %x\n", UTL_GetSysTimer());
#ifdef Digital_PANEL
	Vipor_OsdSetRowAttr(15, 0x00, 0x75);		// NORMAL + EOD
#else
	Vipor_OsdSetRowAttr(15, 0x00, 0x15);		// NORMAL + EOD
#endif	
	Vipor_OsdClearRow(0, 15, VIPOR_OSD_BLACK);	// OSD_BLACK=0
	Vipor_OsdPositionSet(5,5);  
	Vipor_OsdEnable(TRUE);
//printf("time 6 = %x\n", UTL_GetSysTimer());
  Vipor_SC_Write_Byte(0x85, 0x00);
  #undef i
}
	
