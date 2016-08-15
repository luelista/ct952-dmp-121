#ifdef SUPPORT_TFT

enum TFT_SETUP_ID
{
	TFT_SETUP_BEGIN = 0,
	TFT_SETUP_BRIGHTNESS=TFT_SETUP_BEGIN,
	TFT_SETUP_CONTRAST,
	TFT_SETUP_SATURATION,
	TFT_SETUP_RESET,
	TFT_SETUP_NUM
};

#define TFT_SETTING_PLUS     TRUE
#define TFT_SETTING_MINUS    FALSE 

#define TFT_SETUPOSD_STRING  3
#define TFT_SETUP_VALUE_LEVEL  0X10
#ifdef IMAGE_FRAME_SETUP
#define TFT_SETUP_BRIGHTNESS_DEFAULT_LEVEL 0X08
BYTE TFT_BRIGHTNESS_VALUE_LIST[]={0x00,0X10,0X20,0x30,0x40,0x50,0x60,0x70,0X80,0X90,0XA0,0XB0,0XC0,0XD0,0XE0,0XF0};
#define TFT_SETUP_CONTRAST_DEFAULT_LEVEL 0X08
BYTE TFT_CONTRAST_VALUE_LIST[]={0X10,0X20,0x30,0x40,0x50,0x60,0x70,0X80,0X90,0XA0,0XB0,0XC0,0XD0,0XE0,0XF0};
#define TFT_SETUP_SATURATION_DEFAULT_LEVEL 0X08
BYTE TFT_SATURATION_VALUE_LIST[]={0X10,0X20,0x30,0x40,0x50,0x60,0x70,0X80,0X90,0XA0,0XB0,0XC0,0XD0,0XE0,0XF0};
#else
#define TFT_SETUP_BRIGHTNESS_DEFAULT_LEVEL 0X08
BYTE code TFT_BRIGHTNESS_VALUE_LIST[TFT_SETUP_VALUE_LEVEL]={0x00,0X10,0X20,0x30,0x40,0x50,0x60,0x70,0X80,
									   0X90,0XA0,0XB0,0XC0,0XD0,0XE0,0XF0};
#define TFT_SETUP_CONTRAST_DEFAULT_LEVEL 0X08
BYTE code TFT_CONTRAST_VALUE_LIST[TFT_SETUP_VALUE_LEVEL]={0X10,0X20,0x30,0x40,0x50,0x60,0x70,
                          0X80,
									   0X90,0XA0,0XB0,0XC0,0XD0,0XE0,0XF0,0XFF};
#define TFT_SETUP_SATURATION_DEFAULT_LEVEL 0X08
BYTE code TFT_SATURATION_VALUE_LIST[TFT_SETUP_VALUE_LEVEL]={0X10,0X20,0x30,0x40,0x50,0x60,0x70,
                          0X80,
									   0X90,0XA0,0XB0,0XC0,0XD0,0XE0,0XF0,0XFF};	
#endif //IMAGE_FRAME_SETUP
	
#define TFT_SETUPOSD_BackGroundWidth (TFT_SETUPOSD_STRING+TFT_SETUP_VALUE_LEVEL)
#define TFT_SETUPOSD_BackGroundHeight (TFT_SETUP_NUM-1)
#define TFT_SETUPOSD_BackGroundStarX    0x6
#define TFT_SETUPOSD_BackGroundStarY    0x3
#define TFT_SETUPOSD_BackGroundColor VIPOR_OSD_WHITE
#define TFT_SETUPOSD_BackGround_WindowId   0x02


#define TFT_SETUPOSD_ItemStartX  (TFT_SETUPOSD_BackGroundStarX)
#define TFT_SETUPOSD_ItemStartY  (TFT_SETUPOSD_BackGroundStarY)


#define TFT_SETUPOSD_ValueStartX  (TFT_SETUPOSD_ItemStartX+TFT_SETUPOSD_STRING)
#define TFT_SETUPOSD_ValueStartWidth  (TFT_SETUP_VALUE_LEVEL)

#define TFT_SETUPOSD_Hilight_WindowId     0x01
#define TFT_SETUPOSD_Hilight_WindowWidth (TFT_SETUPOSD_STRING+TFT_SETUP_VALUE_LEVEL)
#define TFT_SETUPOSD_Hilight_WindowStartX TFT_SETUPOSD_ItemStartX
#define TFT_SETUPOSD_Hilight_WindowStartY TFT_SETUPOSD_ItemStartY
#define TFT_SETUPOSD_HilightColor VIPOR_OSD_YELLOW

//#define TFT_SETUPOSD_OSDColor VIPOR_OSD_YELLOW
#define TFT_SETUPOSD_OSDColor VIPOR_OSD_MAGENTA

BYTE code Vipor_Osd_Brightness_String[TFT_SETUPOSD_STRING+1] =
{
	_VIPOR_BRI_L,
	_VIPOR_BRI_R,
	_VIPOR__,
 	_VIPOR_EOF
};
BYTE code Vipor_Osd_Contrast_String[TFT_SETUPOSD_STRING+1] =
{
	_VIPOR_CON_L,
	_VIPOR_CON_R,
	_VIPOR__,
 	_VIPOR_EOF
};
BYTE code Vipor_Osd_Saturation_String[TFT_SETUPOSD_STRING+1] =
{
	_VIPOR_HUE_L,
	_VIPOR_HUE_R,
	_VIPOR__,
 	_VIPOR_EOF
};
BYTE code Vipor_Osd_Reset_String[] =
{
	_VIPOR_R,
	_VIPOR_E,
	_VIPOR_S,
	_VIPOR_E,
	_VIPOR_T,
 	_VIPOR_EOF
};

BYTE code Vipor_OsdBar_Empty_String[2] =
{
	_VIPOR_HBar0,
 	_VIPOR_EOF
};
BYTE code Vipor_OsdBar_Full_String[2] =
{
	_VIPOR_HBar6,
 	_VIPOR_EOF
};
BYTE code Vipor_OsdBar_Left_String[2] =
{
	_VIPOR_HBarL,
 	_VIPOR_EOF
};
BYTE code Vipor_OsdBar_Right_String[2] =
{
	_VIPOR_HBarR,
 	_VIPOR_EOF
};

BYTE __bTFTSetupId;
BYTE __bTFTSetupOSD_Temp1,__bTFTSetupOSD_Temp2;
DWORD __dwTFT_OsdSetupTimer=0xffffffff;

void _TFT_AdjustSetting(BOOL blPlus);

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTOSD_SetupBackGround
// Description : Draw setup back ground
// return      :
//--------------------------------------------------------------------------
void TFTOSD_SetupBackGround(void)
{
	OsdWindow(TFT_SETUPOSD_BackGround_WindowId,
				TFT_SETUPOSD_BackGroundStarX, 
				TFT_SETUPOSD_BackGroundStarY,
				TFT_SETUPOSD_BackGroundStarX+TFT_SETUPOSD_BackGroundWidth,
				TFT_SETUPOSD_BackGroundStarY+TFT_SETUPOSD_BackGroundHeight,
				TFT_SETUPOSD_BackGroundColor);
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTOSD_SetupClearAll
// Description : clear  setup UI
// return      :
//--------------------------------------------------------------------------
void TFTOSD_SetupClearAll(void)
{
	//Vipor_OsdClearRow(TFT_SETUPOSD_BackGroundStarY,
	//	TFT_SETUPOSD_BackGroundStarY+TFT_SETUPOSD_BackGroundHeight,
	//	VIPOR_OSD_BLACK);
	Vipor_OsdClearRow(0,13,VIPOR_OSD_BLACK);		
	Vipor_OsdDisableWindow(TFT_SETUPOSD_Hilight_WindowId);
	Vipor_OsdDisableWindow(TFT_SETUPOSD_BackGround_WindowId);
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTOSD_SetupHilight
// Description :Hilight one row
// input      :hilight row id
//--------------------------------------------------------------------------
void TFTOSD_SetupHilight(BYTE bId)
{
	__bTFTSetupOSD_Temp1=bId+TFT_SETUPOSD_ItemStartY;
	OsdWindow(TFT_SETUPOSD_Hilight_WindowId,
				TFT_SETUPOSD_Hilight_WindowStartX, 
				__bTFTSetupOSD_Temp1,
				TFT_SETUPOSD_Hilight_WindowStartX+TFT_SETUPOSD_Hilight_WindowWidth,
				__bTFTSetupOSD_Temp1,
				TFT_SETUPOSD_HilightColor);
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTOSD_SetupDrawBar
// Description :draw bar
// input      :bid: bar row id ; bValue: bar progress
//--------------------------------------------------------------------------
void TFTOSD_SetupDrawBar(BYTE  bId,BYTE bValue)
{
	__bTFTSetupOSD_Temp1=bId+TFT_SETUPOSD_ItemStartY;
	switch(bId)
		{
		case TFT_SETUP_BRIGHTNESS:
					Vipor_PrintString(__bTFTSetupOSD_Temp1,
					TFT_SETUPOSD_ItemStartX,
					TFT_SETUPOSD_OSDColor,
					(BYTE*) &Vipor_Osd_Brightness_String);
					break;
		case TFT_SETUP_CONTRAST:
					Vipor_PrintString(__bTFTSetupOSD_Temp1,
					TFT_SETUPOSD_ItemStartX,
					TFT_SETUPOSD_OSDColor,
					(BYTE*) &Vipor_Osd_Contrast_String);			
					break;
		case TFT_SETUP_SATURATION:
					Vipor_PrintString(__bTFTSetupOSD_Temp1,
					TFT_SETUPOSD_ItemStartX,
					TFT_SETUPOSD_OSDColor,
					(BYTE*) &Vipor_Osd_Saturation_String);		
					break;
		case TFT_SETUP_RESET:
					Vipor_PrintString(__bTFTSetupOSD_Temp1,
					TFT_SETUPOSD_ItemStartX,
					TFT_SETUPOSD_OSDColor,
					(BYTE*) &Vipor_Osd_Reset_String);	
					return;
		default:
					break;
		}
	Vipor_SetFirstCharAttr(__bTFTSetupOSD_Temp1,TFT_SETUPOSD_ValueStartX,TFT_SETUPOSD_OSDColor);
	Vipor_SetOneCharContent(_VIPOR_HBarL);
  bValue = bValue >> 4;
	for(__bTFTSetupOSD_Temp2=1;__bTFTSetupOSD_Temp2<bValue;__bTFTSetupOSD_Temp2++)
	{
		Vipor_SetOneCharContent(_VIPOR_HBar6);
	}

	for(__bTFTSetupOSD_Temp2=bValue;__bTFTSetupOSD_Temp2<TFT_SETUP_VALUE_LEVEL;__bTFTSetupOSD_Temp2++)
	{
		Vipor_SetOneCharContent(_VIPOR_HBar0);
	}

	Vipor_SetOneCharContent(_VIPOR_HBarR);
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTOSD_SetupFillBar
// Description :fill or empty bar progress
// input      
//--------------------------------------------------------------------------
void TFTOSD_SetupFillBar(BYTE  bFill,BYTE bId,BYTE bValue)
{
	__bTFTSetupOSD_Temp1=bId+TFT_SETUPOSD_ItemStartY;
	__bTFTSetupOSD_Temp2=TFT_SETUPOSD_ValueStartX+bValue;
	if(bFill)
	{
		Vipor_PrintString(__bTFTSetupOSD_Temp1,
						__bTFTSetupOSD_Temp2,
						TFT_SETUPOSD_OSDColor,
						(BYTE*) &Vipor_OsdBar_Full_String);
	}
	else
	{
		Vipor_PrintString(__bTFTSetupOSD_Temp1,
						__bTFTSetupOSD_Temp2,
						TFT_SETUPOSD_OSDColor,
						(BYTE*) &Vipor_OsdBar_Empty_String);
	}
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTSetup_ReadSetupData
// Description :read data from eprom
// input      
//--------------------------------------------------------------------------
extern void DVDSETUP_SetDefaultTFTAttribute(void);
void TFTSetup_ReadSetupData(void)
{
	HAL_ReadStorage(TFT_ADDR_BIRHGTNESS, &(__TFTPicAttr.bBrightness), 1);
	HAL_ReadStorage(TFT_ADDR_CONTRAST, &(__TFTPicAttr.bContrast), 1);
	HAL_ReadStorage(TFT_ADDR_SATURATION, &(__TFTPicAttr.bSaturation), 1);

#ifdef IMAGE_FRAME_SETUP
    DVDSETUP_SetDefaultTFTAttribute();
#else
    if((__TFTPicAttr.bBrightness>TFT_SETUP_VALUE_LEVEL)||(__TFTPicAttr.bBrightness==0))
        __TFTPicAttr.bBrightness=TFT_SETUP_BRIGHTNESS_DEFAULT_LEVEL;
    
    if((__TFTPicAttr.bContrast>=TFT_SETUP_VALUE_LEVEL)||(__TFTPicAttr.bContrast==0))
        __TFTPicAttr.bContrast=TFT_SETUP_CONTRAST_DEFAULT_LEVEL;
    
    if((__TFTPicAttr.bSaturation>=TFT_SETUP_VALUE_LEVEL)||(__TFTPicAttr.bSaturation==0))
        __TFTPicAttr.bSaturation=TFT_SETUP_SATURATION_DEFAULT_LEVEL;
#endif //IMAGE_FRAME_SETUP
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTSetup_WriteSetupData
// Description :write data to eprom
// input      
//--------------------------------------------------------------------------
void TFTSetup_WriteSetupData(void)
{
	HAL_WriteStorage(TFT_ADDR_BIRHGTNESS, &(__TFTPicAttr.bBrightness), 1);
	HAL_WriteStorage(TFT_ADDR_CONTRAST, &(__TFTPicAttr.bContrast), 1);
	HAL_WriteStorage(TFT_ADDR_SATURATION, &(__TFTPicAttr.bSaturation), 1);
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTSetup_SetSetupDataDefault
// Description :set setup data to default value and write it to eprom
// input      
//--------------------------------------------------------------------------
void TFTSetup_SetSetupDataDefault(void)
{
#ifdef IMAGE_FRAME_SETUP
    __TFTPicAttr.bBrightness=SETUP_DEFAULT_TFT_BRIGHTNESS;
    __TFTPicAttr.bContrast=SETUP_DEFAULT_TFT_CONTRAST;
    __TFTPicAttr.bSaturation=SETUP_DEFAULT_TFT_SATURATION;
#else
    __TFTPicAttr.bBrightness=TFT_SETUP_BRIGHTNESS_DEFAULT_LEVEL;
    __TFTPicAttr.bContrast=TFT_SETUP_CONTRAST_DEFAULT_LEVEL;
    __TFTPicAttr.bSaturation=TFT_SETUP_SATURATION_DEFAULT_LEVEL;
#endif //IMAGE_FRAME_SETUP

    TFTSetup_WriteSetupData();
}

//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTSetup_SetPicAttr
// Description :transfer eprom data to VP77 register value 
// input      
//--------------------------------------------------------------------------

void TFTSetup_SetPicAttr(void)
{
	TFTSetup_ReadSetupData();

#ifdef IMAGE_FRAME_SETUP
    Vipor_SetPicture_Brightness(TFT_BRIGHTNESS_VALUE_LIST[__TFTPicAttr.bBrightness-SETUP_DISPLAY_TFT_BRIGHTNESS_BEGIN]);
    Vipor_SetPicture_Contrast(TFT_CONTRAST_VALUE_LIST[__TFTPicAttr.bContrast-SETUP_DISPLAY_TFT_CONTRAST_BEGIN]);
    Vipor_SetPicture_Saturation(TFT_SATURATION_VALUE_LIST[__TFTPicAttr.bSaturation-SETUP_DISPLAY_TFT_SATURATION_BEGIN]);
#else
	Vipor_SetPicture_Brightness(TFT_BRIGHTNESS_VALUE_LIST[__TFTPicAttr.bBrightness-1]);
	Vipor_SetPicture_Contrast(TFT_CONTRAST_VALUE_LIST[__TFTPicAttr.bContrast-1]);
	Vipor_SetPicture_Saturation(TFT_SATURATION_VALUE_LIST[__TFTPicAttr.bSaturation-1]);
#endif //IMAGE_FRAME_SETUP
}
#ifdef MUSTEK_RING_BRIGHT
BYTE code TFTOSD_BR_String[] = { _VIPOR_B, _VIPOR_R, _VIPOR_I, _VIPOR_G, _VIPOR_H, _VIPOR_T, _VIPOR_N, _VIPOR_E, _VIPOR_S, _VIPOR_S, _VIPOR_EOF };
BYTE _bLEVEL_ID=0x08;
BYTE show_string[1];
void  show_num_string(BYTE _bLEVEL)
{
    show_string [0] = _bLEVEL+1;
    show_string [1] = _VIPOR_EOF;
}    
void show_BRIGHT_MENU(BYTE _bLEVEL)
{
	Vipor_PrintString(13,4,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_BR_String);
	show_num_string(_bLEVEL);
	Vipor_PrintString_ALL(13,15,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);
}	
#endif
//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTSetup_ProcessKey
// Description :process key 
// input      
//--------------------------------------------------------------------------
#ifdef SetPicture_ALL
BYTE code TFTOSD_BR_String[] = { _VIPOR_B,	_VIPOR_R,	_VIPOR_EOF };
BYTE code TFTOSD_CO_String[] = { _VIPOR_C,	_VIPOR_O,	_VIPOR_EOF };
BYTE code TFTOSD_HU_String[] = { _VIPOR_H,	_VIPOR_U,	_VIPOR_EOF };
BYTE code TFTOSD_SA_String[] = { _VIPOR_S,	_VIPOR_A,	_VIPOR_EOF };
BYTE code TFTOSD_RG_String[] = { _VIPOR_R,	_VIPOR_G,	_VIPOR_EOF };
BYTE code TFTOSD_GG_String[] = { _VIPOR_G,	_VIPOR_G,	_VIPOR_EOF };
BYTE code TFTOSD_BG_String[] = { _VIPOR_B,	_VIPOR_G,	_VIPOR_EOF };
BYTE code TFTOSD_RO_String[] = { _VIPOR_R,	_VIPOR_O,	_VIPOR_EOF };
BYTE code TFTOSD_GO_String[] = { _VIPOR_G,	_VIPOR_O,	_VIPOR_EOF };
BYTE code TFTOSD_BO_String[] = { _VIPOR_B,	_VIPOR_O,	_VIPOR_EOF };
BYTE show_string[3];
BYTE _bLEVEL_TABLE[] = {0,128,128,128,128,128,128,128,128,128};
BYTE _bLEVEL_ID=0;
void  show_num_string(BYTE _bLEVEL)
{
    show_string [0] = (_bLEVEL/100)+1;
    show_string [1] = (((_bLEVEL-(show_string [0]-1)*100))/10)+1;
    show_string [2] = (_bLEVEL-((show_string [0]-1)*100)-((show_string [1]-1)*10))+1;
    show_string [3] = _VIPOR_EOF;
}    
void  show_TFT_MENU(void)
{
		Vipor_PrintString(0,0,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_BR_String);
		   show_num_string(_bLEVEL_TABLE[0]);
		Vipor_PrintString_ALL(0,3,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);
		Vipor_PrintString(1,0,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_CO_String);
		   show_num_string(_bLEVEL_TABLE[1]);
		Vipor_PrintString_ALL(1,3,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);		
		Vipor_PrintString(2,0,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_HU_String);
		   show_num_string(_bLEVEL_TABLE[2]);
		Vipor_PrintString_ALL(2,3,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);		
		Vipor_PrintString(3,0,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_SA_String);
		   show_num_string(_bLEVEL_TABLE[3]);
		Vipor_PrintString_ALL(3,3,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);		
		Vipor_PrintString(4,0,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_RG_String);
		   show_num_string(_bLEVEL_TABLE[4]);
		Vipor_PrintString_ALL(4,3,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);		
		Vipor_PrintString(5,0,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_GG_String);
		   show_num_string(_bLEVEL_TABLE[5]);
		Vipor_PrintString_ALL(5,3,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);		
		Vipor_PrintString(6,0,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_BG_String);			
		   show_num_string(_bLEVEL_TABLE[6]);
		Vipor_PrintString_ALL(6,3,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);		
		Vipor_PrintString(7,0,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_RO_String);
		   show_num_string(_bLEVEL_TABLE[7]);
		Vipor_PrintString_ALL(7,3,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);		
		Vipor_PrintString(8,0,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_GO_String);
		   show_num_string(_bLEVEL_TABLE[8]);
		Vipor_PrintString_ALL(8,3,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);		
		Vipor_PrintString(9,0,TFT_SETUPOSD_OSDColor,(BYTE*) &TFTOSD_BO_String);
		   show_num_string(_bLEVEL_TABLE[9]);
		Vipor_PrintString_ALL(9,3,TFT_SETUPOSD_OSDColor,(BYTE*) &show_string);
} 
void show_Hilight_ID(BYTE id)
{
	OsdWindow(0x01,	0, id, 5,id, TFT_SETUPOSD_HilightColor);
}
BYTE TFTSetup_ProcessKey(BYTE bKey)
{
	  BYTE  bKeyReturn;

    if (__TFTOSDStatusInfo.bSetupMenu == TFT_OSD_SetupMenu_OFF && KEY_PICTURE!= bKey)
        return bKey;
	switch(bKey)
	{
		case KEY_PICTURE:  
			if(__TFTOSDStatusInfo.bSetupMenu==TFT_OSD_SetupMenu_OFF)
			{
				__TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_ON;
		    show_TFT_MENU();
		    OsdWindow(0x02,	0, 0, 5,9, TFT_SETUPOSD_BackGroundColor);
        show_Hilight_ID(_bLEVEL_ID);
			}
			else
			{
				__TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_OFF;
				__dwTFT_OsdSetupTimer=0xffffffff;
				TFTOSD_SetupClearAll();
			}
				bKeyReturn =KEY_NO_KEY;
			break;
		case KEY_UP:
				if(_bLEVEL_ID==0)
					_bLEVEL_ID=9;
				else _bLEVEL_ID--;
        show_Hilight_ID(_bLEVEL_ID);
				bKeyReturn =KEY_NO_KEY;
				break;
				
		case KEY_DOWN:
				if(_bLEVEL_ID==9)
					_bLEVEL_ID=0;
				else _bLEVEL_ID++;
        show_Hilight_ID(_bLEVEL_ID);
				bKeyReturn =KEY_NO_KEY;
				break;			
		case KEY_LEFT:
				if(_bLEVEL_TABLE[_bLEVEL_ID]==0)
					_bLEVEL_TABLE[_bLEVEL_ID]=255;
				else _bLEVEL_TABLE[_bLEVEL_ID]--;
				show_TFT_MENU();
			bKeyReturn = KEY_NO_KEY;
			break;
		case KEY_RIGHT:
				if(_bLEVEL_TABLE[_bLEVEL_ID]==255)
					_bLEVEL_TABLE[_bLEVEL_ID]=0;
				else _bLEVEL_TABLE[_bLEVEL_ID]++;
				show_TFT_MENU();
			bKeyReturn = KEY_NO_KEY;
			break;	
		case KEY_PREV:
				if(_bLEVEL_TABLE[_bLEVEL_ID]<10)
					_bLEVEL_TABLE[_bLEVEL_ID]=_bLEVEL_TABLE[_bLEVEL_ID]+245;
				else _bLEVEL_TABLE[_bLEVEL_ID]=_bLEVEL_TABLE[_bLEVEL_ID]-10;
				show_TFT_MENU();
			bKeyReturn = KEY_NO_KEY;
			break;
		case KEY_NEXT:
				if(_bLEVEL_TABLE[_bLEVEL_ID]>245)
					_bLEVEL_TABLE[_bLEVEL_ID]=_bLEVEL_TABLE[_bLEVEL_ID]-245;
				else _bLEVEL_TABLE[_bLEVEL_ID]=_bLEVEL_TABLE[_bLEVEL_ID]+10;
				show_TFT_MENU();
			bKeyReturn = KEY_NO_KEY;
			break;				
		default:
                    bKeyReturn = bKey;
                    break;

	}
  Vipor_SetPicture_ALL();
	return bKeyReturn;
}		
		
		      
#else
#ifdef  MUSTEK_RING_BRIGHT //SetPicture_ALL

BYTE TFTSetup_ProcessKey(BYTE bKey)
{
	  BYTE  bKeyReturn;

          if(__bSetupMode) 
                  return bKey;
                  
    if (__TFTOSDStatusInfo.bSetupMenu == TFT_OSD_SetupMenu_OFF && (bKey != KEY_PICTURE && bKey != KEY_BRIGHT_UP && bKey != KEY_BRIGHT_DOWN))
        return bKey;
            
	switch(bKey)
	{
		case KEY_PICTURE:  
		  __dwTFT_OsdSetupTimer = OS_GetSysTimer()+COUNT_3_SEC;
			if(__TFTOSDStatusInfo.bSetupMenu==TFT_OSD_SetupMenu_OFF)
			{
				__TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_ON;
		    show_BRIGHT_MENU(_bLEVEL_ID-5);
		    //OsdWindow(0x02,	0, 0, 5,9, TFT_SETUPOSD_BackGroundColor);
			}
			else
			{
				_bLEVEL_ID++;	
				if (_bLEVEL_ID > 11)
				  _bLEVEL_ID = 5;

				  Vipor_SetPicture_Brightness(TFT_BRIGHTNESS_VALUE_LIST[_bLEVEL_ID-1]);
        show_BRIGHT_MENU(_bLEVEL_ID-5);				  
			}
				bKeyReturn =KEY_NO_KEY;
			break;
		case KEY_BRIGHT_UP:  
		  __dwTFT_OsdSetupTimer = OS_GetSysTimer()+COUNT_3_SEC;
			if(__TFTOSDStatusInfo.bSetupMenu==TFT_OSD_SetupMenu_OFF)
			{
				__TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_ON;
		    show_BRIGHT_MENU(_bLEVEL_ID-5);
		    //OsdWindow(0x02,	0, 0, 5,9, TFT_SETUPOSD_BackGroundColor);
			}
			else
			{
				_bLEVEL_ID++;	
				if (_bLEVEL_ID > 11)
				  _bLEVEL_ID = 5;

				  Vipor_SetPicture_Brightness(TFT_BRIGHTNESS_VALUE_LIST[_bLEVEL_ID-1]);
        show_BRIGHT_MENU(_bLEVEL_ID-5);				  
			}
				bKeyReturn =KEY_NO_KEY;
			break;
		case KEY_BRIGHT_DOWN:  
		  __dwTFT_OsdSetupTimer = OS_GetSysTimer()+COUNT_3_SEC;
			if(__TFTOSDStatusInfo.bSetupMenu==TFT_OSD_SetupMenu_OFF)
			{
				__TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_ON;
		    show_BRIGHT_MENU(_bLEVEL_ID-5);
		    //OsdWindow(0x02,	0, 0, 5,9, TFT_SETUPOSD_BackGroundColor);
			}
			else
			{
				_bLEVEL_ID--;	
				if (_bLEVEL_ID < 5)
				  _bLEVEL_ID = 11;

				  Vipor_SetPicture_Brightness(TFT_BRIGHTNESS_VALUE_LIST[_bLEVEL_ID-1]);
        show_BRIGHT_MENU(_bLEVEL_ID-5);				  
			}
				bKeyReturn =KEY_NO_KEY;
			break;
		default:
                    bKeyReturn = bKey;
                    break;			
	}
	return bKeyReturn;
}		
		
#else //MUSTEK_RING_BRINESS
BYTE TFTSetup_ProcessKey(BYTE bKey)
{
    BYTE  bKeyReturn;

    if (__TFTOSDStatusInfo.bSetupMenu == TFT_OSD_SetupMenu_OFF && KEY_PICTURE!= bKey)
        return bKey;

	switch(bKey)
	{
		case KEY_PICTURE:
			if(__TFTOSDStatusInfo.bSetupMenu==TFT_OSD_SetupMenu_OFF)
			{
				__TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_ON;
				TFTOSD_SetupBackGround();
				TFTOSD_SetupDrawBar(TFT_SETUP_BRIGHTNESS,__TFTPicAttr.bBrightness);
				TFTOSD_SetupDrawBar(TFT_SETUP_CONTRAST,__TFTPicAttr.bContrast);
				TFTOSD_SetupDrawBar(TFT_SETUP_SATURATION,__TFTPicAttr.bSaturation);
				TFTOSD_SetupDrawBar(TFT_SETUP_RESET,0);
				
				TFTOSD_SetupHilight(TFT_SETUP_BEGIN);
				__bTFTSetupId=TFT_SETUP_BEGIN;
        __dwTFT_OsdSetupTimer = OS_GetSysTimer()+COUNT_5_SEC;
			}
			else
			{
				__TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_OFF;
				__dwTFT_OsdSetupTimer=0xffffffff;
				TFTOSD_SetupClearAll();
			}
				bKeyReturn =KEY_NO_KEY;
			break;
		case KEY_UP:
				if(__bTFTSetupId==TFT_SETUP_BEGIN)
					__bTFTSetupId=TFT_SETUP_NUM-1;
				else __bTFTSetupId--;
				TFTOSD_SetupHilight(__bTFTSetupId);
				__dwTFT_OsdSetupTimer= OS_GetSysTimer()+COUNT_5_SEC;
				bKeyReturn =KEY_NO_KEY;
				break;
				
		case KEY_DOWN:
				__bTFTSetupId++;
				if(__bTFTSetupId==TFT_SETUP_NUM)
					__bTFTSetupId=TFT_SETUP_BEGIN;
				TFTOSD_SetupHilight(__bTFTSetupId);
				__dwTFT_OsdSetupTimer= OS_GetSysTimer()+COUNT_5_SEC;
				bKeyReturn =KEY_NO_KEY;
				break;
		case KEY_LEFT:
                        _TFT_AdjustSetting(TFT_SETTING_MINUS);
			bKeyReturn = KEY_NO_KEY;
			break;
		case KEY_RIGHT:
                        _TFT_AdjustSetting(TFT_SETTING_PLUS);
			bKeyReturn = KEY_NO_KEY;
			break;
		case KEY_ENTER:
    case KEY_PLAY:
			if (TFT_SETUP_RESET == __bTFTSetupId)
                        {
						__TFTPicAttr.bBrightness=TFT_SETUP_BRIGHTNESS_DEFAULT_LEVEL;
						__TFTPicAttr.bContrast=TFT_SETUP_CONTRAST_DEFAULT_LEVEL;
						__TFTPicAttr.bSaturation=TFT_SETUP_SATURATION_DEFAULT_LEVEL;
						TFTOSD_SetupDrawBar(TFT_SETUP_BRIGHTNESS,__TFTPicAttr.bBrightness);
						TFTOSD_SetupDrawBar(TFT_SETUP_CONTRAST,__TFTPicAttr.bContrast);
						TFTOSD_SetupDrawBar(TFT_SETUP_SATURATION,__TFTPicAttr.bSaturation);

						Vipor_SetPicture_Brightness(TFT_BRIGHTNESS_VALUE_LIST[__TFTPicAttr.bBrightness-1]);
						Vipor_SetPicture_Contrast(TFT_CONTRAST_VALUE_LIST[__TFTPicAttr.bContrast-1]);
						Vipor_SetPicture_Saturation(TFT_SATURATION_VALUE_LIST[__TFTPicAttr.bSaturation-1]);
                        }
                        else
                        {
                            __TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_OFF;
                            TFTOSD_SetupClearAll();
                        }
			bKeyReturn =KEY_NO_KEY;
			break;
		default:
                    bKeyReturn = bKey;
                    break;

	}
	return bKeyReturn;
}
#endif //MUSTEK_RING_BRIGHT
#endif //SetPicture_ALL
//*******************************************************************************
//--------------------------------------------------------------------------
// Function    : TFTSetup_Trigger
// Description :setup ui trigger 
// input      
//--------------------------------------------------------------------------

void TFTSetup_Trigger(void)
{
	if (__TFTOSDStatusInfo.bSetupMenu==TFT_OSD_SetupMenu_ON)
	{
		if(__dwTFT_OsdSetupTimer< OS_GetSysTimer())
		{
			__dwTFT_OsdSetupTimer=0xffffffff;
			__TFTOSDStatusInfo.bSetupMenu=TFT_OSD_SetupMenu_OFF;
			TFTOSD_SetupClearAll();
		}
	}
}	

void _TFT_AdjustSetting(BOOL blPlus)
{
    switch(__bTFTSetupId)
    {
        case TFT_SETUP_BRIGHTNESS:
            if (blPlus)
            {
                if (__TFTPicAttr.bBrightness < TFT_SETUP_VALUE_LEVEL)
                    __TFTPicAttr.bBrightness++;
            }
            else
            {
            if (__TFTPicAttr.bBrightness > 1)
                __TFTPicAttr.bBrightness--;
            }
            Vipor_SetPicture_Brightness(TFT_BRIGHTNESS_VALUE_LIST[__TFTPicAttr.bBrightness-1]);
            TFTOSD_SetupFillBar(blPlus, __bTFTSetupId,__TFTPicAttr.bBrightness - blPlus);
            break;
        case TFT_SETUP_CONTRAST:
            if (blPlus)
            {
                if (__TFTPicAttr.bContrast < TFT_SETUP_VALUE_LEVEL)
                    __TFTPicAttr.bContrast++;
            }
            else
            {
            if (__TFTPicAttr.bContrast > 1)
                __TFTPicAttr.bContrast--;
            }
            Vipor_SetPicture_Contrast(TFT_CONTRAST_VALUE_LIST[__TFTPicAttr.bContrast-1]);
            TFTOSD_SetupFillBar(blPlus, __bTFTSetupId,__TFTPicAttr.bContrast - blPlus);
            break;
        case TFT_SETUP_SATURATION:
            if (blPlus)
            {
                if (__TFTPicAttr.bSaturation < TFT_SETUP_VALUE_LEVEL)
                    __TFTPicAttr.bSaturation++;
            }
            else
            {
            if (__TFTPicAttr.bSaturation > 1)
                __TFTPicAttr.bSaturation--;
            }
            Vipor_SetPicture_Saturation(TFT_SATURATION_VALUE_LIST[__TFTPicAttr.bSaturation-1]);
            TFTOSD_SetupFillBar(blPlus, __bTFTSetupId,__TFTPicAttr.bSaturation  - blPlus);
            break;
        default:
            break;
    }

    TFTSetup_WriteSetupData();
    __dwTFT_OsdSetupTimer = OS_GetSysTimer()+COUNT_5_SEC;
}


#else
//add dummy const code for *.lin
BYTE code bDummyTFTSetup;
#endif
