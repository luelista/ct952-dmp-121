/**************************************************************************
Version :    1.23
Date    :    2007.04.19
Chip    :    CT908R or CT909_R + CT675 or CT909P
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Move all include table from DECODER_SYSTEM item to WinAv.h
2. Modify Brightness mapping table
3. Fixed 952/955 backlight on delay.
4. Turning color and sharpness(disable).
5. Fixed power off not turn off backlight.(952/955)
6. Fixed PAL table fail .
7. Fixed OSD tuning page display time to 5 sec, 
   but have some display bug not ready 
   (952/955/956 when disable MUSTEK_RING_BRIGHT and SetPicture_ALL)

/**************************************************************************
Version :    1.22
Date    :    2007.03.29
Chip    :    CT988R or CT909_R + CT675 or CT909P
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Support system
   a. DMP952A_EVAL
   b. DVD909P_A_EVAL
   c. DVD909P_EVAL_P3
   d. DMP955A_EVAL
   e. DMP956A_EVAL
   f. DVD956A_DEV
   g. CT950_STYLE
2. Fixed 952 backlight ON/OFF.
3. update Rgtable_CT909P.H for DVD909P_EVAL_P3

/**************************************************************************
Version :    1.21
Date    :    2007.03.20
Chip    :    CT988R or CT909_R + CT675 or CT909P
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Rename CT951_PLATFORM to CT950_STYLE
2. Modify PIN_SDAT/ PIN_SCLK for CT951.

/**************************************************************************
Version :    1.20
Date    :    2007.03.19
Chip    :    CT988R or CT909_R + CT675 or CT909P
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Fixed 950 table load error!!
2. Enable 956/955/952 dither enable

/**************************************************************************
Version :    1.19
Date    :    2007.03.16
Chip    :    CT988R or CT909_R + CT675 or CT909P
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Change table loading from TFT.C

/**************************************************************************
Version :    1.18
Date    :    2007.03.15
Chip    :    CT988R or CT909_R + CT675 or CT909P
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Change MVD / ADC ID chansfer function !!
2. add P-scan mode for Digital Panel.
3. Add DISABLE_INTERRUPTS in TFT.C
4. Modify Gamma for 952 and 956 table.
5. OSD change to big size for Digital panel.
6. ..\Tables structure 
   \Brand name\analog or digital\modol or customer\
   ex. ..\Tables\CPT\analog\950\
   
/**************************************************************************
Version :    1.17
Date    :    2007.02.07
Chip    :    CT988R or CT909_R + CT675 or CT909P
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Fixed show string function
2. Add "Light_detect" function for 952 demo board
3. Add "Digital_PANEL" define.
4. Make a folder include all register table.
   Please copy "ViporRegTable_CT675.h" which you need to ..\VD\ 
   Change file name to "Regtable_CT950.H" for CT950 or "Regtable_CT952.H" for CT952.
   And make tft.c again.

/**************************************************************************
Version :    1.16
Date    :    2006.11.30
Chip    :    CT988R or CT909_R + CT675 or CT909P
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Modify some define for CT909P 
2. Add function for TFT Show Time

/**************************************************************************
Version :    1.15
Date    :    2006.11.30
Chip    :    CT988R or CT909_R + CT675 or CT909P
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Add CT909P function

/**************************************************************************
Version :    1.14
Date    :    2006.11.01
Chip    :    CT988R or CT909_R + CT675
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Fixed CT908 ViporOSD.C VPP_OsdClearArea function for 908R clear area bug!
2. Disable some printf message.

/**************************************************************************
Version :    1.13
Date    :    2006.08.23
Chip    :    CT988R or CT909_R + CT675
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. HUE limit the angle from -30 deg to +30 deg.
2. Inverse the rotate angle.
/**************************************************************************
Version :    1.12
Date    :    2006.08.23
Chip    :    CT988R or CT909_R + CT675
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. (TCON[0xF6]->Reg[0x6D,0x6E,0x6F]) Reset Default 
   Modify IIC Table S F6 W,69,00,00,08,00,00,00,00,P
2. Add Function for #define SetPicture_ALL needed
3. Modify calculation error in CT908R . ie. (DWORD)

/**************************************************************************
Version :    1.11
Date    :    2006.08.18
Chip    :    CT988R or CT909_R + CT675
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Add RGB Gain and Offset table

BYTE code Temperature_Adjust[][3] =
{
	{ 0x80, 0x80, 0x80 }, //R G B Gain  0 ... 128 ... 255
	{ 0x80, 0x80, 0x80 }  //R G B Offset  0 ... 128 ... 255
};	

2. Add #define HDTV. If needed please open this define.
   If not define , will be use SDTV table

/**************************************************************************
Version :    1.10
Date    :    2006.08.16
Chip    :    CT988R or CT909_R + CT675
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Remove VP77 define,so only support CT675.

2. Reducing Xdata use.

3. Modify Contrast and Saturation function "ViporBCAdjust"
NOTE:
BRIGHTNESS_VALUE please reference Vipor_BrightnessXferTable[] 
Range is ( -127 ~ 0 ~ +127 )
CONTRAST_VALUE Please define in TFTSETUP.C
Range is ( 0 ~ 255 )
SATURATION_VALUE Please define in TFTSETUP.C
Range is ( 0 ~ 255 )
=========================================================================================================
Reference:
TFTSETUP.C
#define TFT_SETUP_BRIGHTNESS_DEFAULT_LEVEL 0X08
BYTE code TFT_BRIGHTNESS_VALUE_LIST[TFT_SETUP_VALUE_LEVEL]={0x00,0X10,0X20,0x30,0x40,0x50,0x60,0x70,
                          0X80,0X90,0XA0,0XB0,0XC0,0XD0,0XE0,0XF0};
#define TFT_SETUP_CONTRAST_DEFAULT_LEVEL 0X08
BYTE code TFT_CONTRAST_VALUE_LIST[TFT_SETUP_VALUE_LEVEL]={0X10,0X20,0x30,0x40,0x50,0x60,0x70,
                          0X80,0X90,0XA0,0XB0,0XC0,0XD0,0XE0,0XF0,0XFF};
#define TFT_SETUP_SATURATION_DEFAULT_LEVEL 0X08
BYTE code TFT_SATURATION_VALUE_LIST[TFT_SETUP_VALUE_LEVEL]={0X10,0X20,0x30,0x40,0x50,0x60,0x70,
                          0X80,0X90,0XA0,0XB0,0XC0,0XD0,0XE0,0XF0,0XFF};	

ViporFunc.c
BYTE code Vipor_BrightnessXferTable[]=
//  0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15
{0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0, 0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70 ,0x7F };
// -8    -7    -6    -5    -4    -3    -2    -1     0    +1    +2    +3    +4    +5    +6    +7
=========================================================================================================

Cancel 1.07 #define  SUPPORT_CON_SAT_TABLE
This Function can't work.
/**************************************************************************
Version :    1.07 
Date    :    2006.08.14
Chip    :    CT988R or CT909_R + VP77 or CT675
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
modify for SUPPORT use Contrast and Saturation table
please use "#define  SUPPORT_CON_SAT_TABLE"
If use this define , please make sure your table is correct 
,and approve your table at Vipor tool first!!

/**************************************************************************
Version :    1.06 
Date    :    2006.08.08
Chip    :    CT988R or CT909_R + VP77 or CT675
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. modify initial NTSC/PAL CCIR656 register table fixed for ¶}¾÷°{Ã{ 
   Please remember initial table must add off back light register.
2. modify LED_BACKLIGHT register table fixed LED ON/OFF
3. Add more define for OSD font use if needed.
   #define USE_CHAR_SPACE
   #define USE_CHAR_0
   #define USE_CHAR_1
   #define USE_CHAR_2
   #define USE_CHAR_3
   #define USE_CHAR_4
   #define USE_CHAR_5
   #define USE_CHAR_6
   #define USE_CHAR_7
   #define USE_CHAR_8
   #define USE_CHAR_9
   .....
   Please reference ViporOSDFont.h
4. Add more define for USE table if needed.
   USE_CVBS_TABLE
   USE_CCIR656_TABLE
   USE_SVIDEO_TABLE
   USE_CVBS_TABLE
5. Fixed SVideo input table
6. Fixed OSD font color define   
7. SoftResetMvdSync add Auto ADC

/**************************************************************************
Version :    1.05 
Date    :    2006.07.27
Chip    :    CT988R or CT909_R + VP77 or CT675
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Modify "Vipor_SetBlackScreen"
2. Add function "Vipor_SetPowerDown" 
3. Add some define for register table and font table.

/**************************************************************************
Version :    1.04 
Date    :    2006.07.10
Chip    :    CT988R or CT909_R + VP77 or CT675
Name    :    CheerTek (J200 JEFF)
----------------------------------------------------------------------------
1. Fixed some times show out vertical blue line when play disc.
2. Shift screen left 2 pixel.
3. Move some define to Customer.h.
  {
   #define SUPPORT_CCIR656
   #define VCLK_NO_INVERSE
   #define SUPPORT_TFT
   #define TFT_ColorInverse
   #define VIPOR_CT675
   #define SUPPORT_BATTERY_DETECTION
   #define LED_BackLight
   }
/**************************************************************************
Version :    1.03 
Date    :    2006.05.30
Chip    :    CT909_R+VP77 or + CT675
Name    :    CheerTek (L400 JEFF)
----------------------------------------------------------------------------
1. Fixed 4:3 -> 16:9 Bug.
2. Change initial table flow.
3. More Function about CVBS IN 4:3 and 16:9
   Vipor_SetAspect_16_9_CVBS();
   Vipor_SetAspect_4_3_CVBS();     
4. Add "#define TFT_ColorInverse" if needed.
/**************************************************************************
Version :    1.02 
Date    :    2006.05.30
Chip    :    CT909_R+VP77 or + CT675
Name    :    CheerTek (L400 JEFF)
----------------------------------------------------------------------------
1. CT908R supported.
   Please define as blow:
     #define VIPOR_CT675 or #define VIPOR_VP77 
     #define VP_API_CT909 or #define VP_API_CT908
   
/**************************************************************************
Version :    1.01 
Date    :    2006.05.30
Chip    :    CT909_R+VP77 or + CT675
Name    :    CheerTek (L400 JEFF)
----------------------------------------------------------------------------
1. Fixed Sigal Locked Function
2. Add Battery Detect Function
3. Fixed Table for invert Pixel Clock

/**************************************************************************
Version :    1.00 
Date    :    2006.05.11
Chip    :    CT909_R+VP77 or + CT675
Name    :    CheerTek (L400 JEFF)
----------------------------------------------------------------------------
1. ready for Release Basic code