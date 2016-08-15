// Graphic Device Interface Module Source File

// Brian, support GDI library
//#define GDI_WITHOUT_OS


// if define this, GDI is like a library, the difference will be
// (1) Shoud not call OSD fucntions: OSD_AdjsutOSDPos
// (2) Won't use COMUTL_RGB2YUV: use it own RGB2YUV function
//#define GDI_STANDALONE_LIBRARY


//#define GDI_NO_PRINTF

#define SUPPORT_CT909S

//#define TEST_1_BIT_FONT

#include "winav.h"
// Micky2.16p, for simple AP
#include    "hsys.h"


#ifdef AP_FRAMEWORK
#define GDI_WITHOUT_OS
#define GDI_STANDALONE_LIBRARY
#define GDI_NO_PRINTF
#endif

#include "ctkav.h"

#define GDI_MAIN_FILE
// Micky2.16p, for simple AP
/*
#ifdef GDI_STANDALONE_LIBRARY
#include "comdef.h"
#else
#include "winav.h"
#endif
*/

#ifndef GDI_STANDALONE_LIBRARY
#include "osd.h"
#include "comutl.h"
#endif

#include "gdi.h"
#include "fontindex.h" //++CoCo2.37P

//Aron,2.77-909P, Port Damon's code to files to support Unicode DIR/FILE name & external subtitle.
#ifdef SUPPORT_UNICODE_CODING
#include "convchar.h"
#endif

#ifndef SYSTEM_8051
#include <string.h>
#endif

#include "disp.h"


#ifdef GDI_STANDALONE_LIBRARY
// Micky2.16p, for simple AP
//#include "utl_ap.h"
DWORD COMUTL_RGB2YUV(DWORD dwRGB);
#else
#include "utl.h"
#endif


//#include "hal.h"


// Brian1.02
// define this to avoid the Display-During-Update case in FillRect
// but the FillRect performance will be much worse.
//#define GDI_CHECK_DISP_MEM_LINE

//#define GDI_CHECK_DISP_MEM_LINE_BY_HW

// just borrow from ComUtl
#define RGB_R(rgb)              ((BYTE)((DWORD)rgb>>16))
#define RGB_G(rgb)              ((BYTE)((DWORD)rgb>>8))
#define RGB_B(rgb)              ((BYTE)rgb)
#define MAKE_RGB(r, g, b)       ( ((DWORD)r<<16) | ((DWORD)g<<8) | (DWORD)b ) // LLY.102

#ifdef GDI_STANDALONE_LIBRARY
typedef union tagUNIONDWORD
{
    BYTE    cS[4];
    DWORD   dwVal;
}   UDWORD;
#endif


#ifdef SUPPORT_CT909S
#define GPU_4B_FONT_MODE        0x0L
#define GPU_8B_FONT_MODE        0x1L
#define GPU_2B_FONT_MODE        0x2L
#define GPU_BMPCOPY                   0x4L
#define GPU_BMPCOPY_HP             0x5L   // higher performance
#define GPU_FILLRECTANGLE         0x6L
#define GPU_FILLRECTANGLE_HP   0x7L // higher performance
#else
#define GPU_4B_FONT_MODE        0x0L
#define GPU_8B_FONT_MODE        0x1L
#define GPU_BMPCOPY         0x2L
#define GPU_FILLRECTANGLE   0x3L
#endif


//move the definition into gdi.h
/*
#define GDI_OSD_8B_MODE         (0)
#define GDI_OSD_4B_MODE         (1)
#define GDI_OSD_2B_MODE         (2)
*/

#define GPU_FONT128         0x0L
#define GPU_FONT256         0x1L
#define GPU_FONT512         0x2L
#define GPU_FONT1024        0x3L

#define GPU_SRC_INCH        0x0L
#define GPU_SRC_DECH        0x1L
#define GPU_SRC_INCV        0x0L
#define GPU_SRC_DECV        0x2L
#define GPU_DST_INCH        0x0L
#define GPU_DST_DECH        0x4L
#define GPU_DST_INCV        0x0L
#define GPU_DST_DECV        0x8L


typedef struct tagTEXT_COLOR
{
    BYTE  bShadowColor_2;
    BYTE  bShadowColor_1;
    BYTE  bTextFgColor;
    BYTE  bTextBgColor;
}TEXT_COLOR, *PTEXT_COLOR;



// #define NO_GDI


#ifndef NO_GDI

#define SETUP_ICON_USE_ONE_BMP
#define SPEAKER_HIGHLIGHT_WITH_RECTANGLE


DWORD Palette_0[]=    // Palette 0 data, it will be put at palette entry 254, 255
{                          // the purpose is to make sure palette entry 255 is transparency
1,                          // one DWORD entry,
0x00000000                  // bit 19 indicate Mix_Enable
};



GDI_REGION_INFO  __RegionList[NUMBER_OF_REGION] =    // declare the region list
{
                    {   // region ID 0
                        GDI_REGION_WIDTH, GDI_REGION_HEIGHT,        // width, height
#ifdef USE_MINI_OSD_REGION
                        GDI_OSD_4B_MODE,              // 0: 256 colors, 1: 16 colors, 2: 4 colors
#else
                        GDI_OSD_8B_MODE,              // 0: 256 colors, 1: 16 colors, 2: 4 colors
#endif

                        DS_OSDFRAME_ST,    //OSD region DRAM address

                    },

                    {   // region ID 1, for screen saver
                        120, 64,                       // width, height

                        GDI_OSD_8B_MODE,  // 0: 256 colors, 1: 16 colors, 2: 4 colors

                        DS_OSDFRAME_ST,    //OSD region DRAM address

                    }
                     ,
                   {   // region ID 2, for SP OSD
                        GDI_SP_OSD_REGION_WIDTH, GDI_SP_OSD_REGION_HEIGHT,                       // width, height

                        GDI_OSD_2B_MODE,  // 0: 256 colors, 1: 16 colors, 2: 4 colors

                        DS_SP_OSD_ST,    //OSD region DRAM address

                    }
                     ,
				   //*** this region is for STB OSD Offscreen buffer only, DVD application doesn't have this region
                   {   // region ID 3, for backup STB OSD
                        320, 200,                       // width, height

                        GDI_OSD_8B_MODE,  // 0: 256 colors, 1: 16 colors, 2: 4 colors

                       	0x40400000,    //DVB use this region. Will configrue it in STB_mode.
                    }
#ifdef SUPPORT_RADIO_RDS					 
                    ,
                   {   // region ID 4, DMP1.21, for radio RDS OSD
                        616, 70,                       // width, height

                        GDI_OSD_8B_MODE,  // 0: 256 colors, 1: 16 colors, 2: 4 colors

                        DS_RADIO_BMPDATA_ST,    //OSD region DRAM address
                    }
#endif
};


//Brian, support GDI library
GDI_PALETTE_INFO    __PaletteInfo_Transparent = {0, PAL_ADDR, Palette_0};



BYTE    _bGDIInit=0;


BYTE    _bGDITemp;
WORD    _wGDITemp;
DWORD   _dwGDITemp;

// Internal to GDI module
PARM_RECT _GDIRect;
GDI_PARAMETER _GDIParm0;

DWORD * _pdwGDIPTR;    // a point to DWORD unit


void _gdi_DrawString(BYTE bRegionId, PARM_DRAW_STRING *pParm, WORD *pwString, BYTE b2BitFont, BYTE bMultipleH);
void _gdi_CheckCmdComplete(void);
void _gdi_ResetVPU(void);
void _gdi_FillRect(BYTE bRegionId, PARM_RECT *pParm);


#endif //NO_GDI


BYTE    __bCurrentRegionId; // the cuurent region ID, which is referenced by many functions

GDI_PARAMETER __GDIParm; // Global for all


// Brian1.00, this global is used as a "parameter" for Macro.
// Currently it is only used by GDI_OUTPUT_TEXT

// Brian1.08, use global variable to replace ColorMode field of region
//BYTE __bColorMode; //--Brian1.15, reduce code size.

#ifdef NO_GDI
BYTE code GDIDummy[]={0};
#endif

//extern BYTE __bOSDDisabled;

WORD _wFontNumberInTable;

BYTE _bFontWidth; 

// The font height selected in the font tool.
// If the font height is selected to 26 and the bimmap H is 16, it will be appended 0 to 26 height
BYTE _bFontHeightInTable;

DWORD _dwGDIPalette[256];

#ifndef GDI_WITHOUT_OS
MUTEX_T     __mutexGDIIdle;
#endif


// Brian0.1, these two should be variable
//DWORD     DS_OSD_FRAME=(0x401F6ED0L);   // 620x420x8
DWORD     __dwGDIFontTableBase= OSD_FONT_TABLE_BASE_ADDRESS; //0x40400000L; //256 font //(0x401EEED0L);   // 128 Font

#ifdef TEST909S
#define GDI_NUM_OF_FONT GPU_FONT512 //GPU_FONT256
#else
#define GDI_NUM_OF_FONT GPU_FONT512
#endif


BYTE _aArc_13[13]={0,0,0,0,1,1,2,2,3,4,5,7,9};
BYTE _aArc_15[15]={0,0,0,0,1,1,1,2,3,3,4,5,7,8,11};
BYTE _aArc_17[17]={0,0,0,0,1,1,1,2,2,3,4,4,5,7,8,10,13};
BYTE _aArc_19[19]={0,0,0,0,1,1,1,2,2,3,3,4,5,6,7,8,10,12,15};
BYTE _aArc_21[21]={0,0,0,0,0,1,1,1,2,2,3,3,4,5,6,7,8,9,11,13,16};
BYTE _aArc_23[23]={0,0,0,0,0,1,1,1,2,2,3,3,4,4,5,6,7,8,9,11,13,15,18};
BYTE _aArc_25[25]={0,0,0,0,0,1,1,1,1,2,2,3,3,4,5,5,6,7,8,9,11,12,14,16,20};

BYTE *_ArcOffsetPtr;


DWORD **__pBMPAryptr; // Support GDI library

#ifdef TEST909S

#ifdef GDI_CHECK_DISP_MEM_LINE
#undef GDI_CHECK_DISP_MEM_LINE
#endif

#endif


#ifdef SUPPORT_CT909S
DWORD   _dwGPUFontWidth, _dwGPUFontCapacity;
#endif

//  *********************************************************************
//  Function    :   GDI_Initial
//  Description :   This function initializes the GDI environment
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  Note        :   It will do the following task:
//                       Load transparent palette (assign entry 255 as transparent color)
//                       Set palette entries used by GDI module
//                       Initialize region 0
//                       Clear region 0
//                       Set font table address and configurate font atble
//  *********************************************************************

void GDI_Initial(void)
{

#ifndef NO_GDI

#ifndef GDI_WITHOUT_OS
    OS_InitMutex(&__mutexGDIIdle);
#endif

    //Brian,  support GDI library
    GDI_LoadPalette(&__PaletteInfo_Transparent);
    GDI_SetGDIPalEntry(); //CoCo, Initialize palette at the beginning.

    GDI_InitialRegion(0);

#ifndef GDI_STANDALONE_LIBRARY
    // if GDI is going to be a library, it should not call OSD module functions, because there may be no OSD module
    // so the AP need to adjust OSD position accroding TV mode and Scan mode.
    OSD_AdjustOSDPos();
#endif


    GDI_ClearRegion(0);

	// Set font table address
#ifdef SUPPORT_CT909S
   REG_GPU_FONT_ADDR = __dwGDIFontTableBase; // just assign the byte address, but need to be 4-byte alignment
   //GDI_SetFontTableSize(2, 64); // default set as 32x32, 2-bit font

#ifdef TEST_1_BIT_FONT
   GDI_SetFontTableSize(1, 26); // default set as 32x26, 1-bit font
#else
   GDI_SetFontTableSize(2, 52); // default set as 32x26, 2-bit font
#endif


#else
    REG_GPU_FONT_ADDR = __dwGDIFontTableBase>>3;
#endif


    _bGDIInit = 1;

#endif //#ifndef NO_GDI

}


#ifdef SUPPORT_CT909S
void GDI_SetFontTableSize(BYTE bFontWidth, WORD wFontCapacity)
{
    // Font width and capacity will be set to REG_GPU_FONT_CONFIG in DrawString

    _dwGPUFontWidth = bFontWidth;           //how many DW of 1 horizontal font line used
    _dwGPUFontCapacity = wFontCapacity;  // how many DW of 1 font used
}

void GDI_SetFontTableAddress(DWORD dwAddress)
{
    __dwGDIFontTableBase = dwAddress; // just assign the byte address, but need to be 4-byte alignment
}
#endif

DWORD GDI_GetFontTableAddress(VOID)
{
    return __dwGDIFontTableBase;
}

void GDI_SetFontWidth(BYTE bFontWidth)
{
    _bFontWidth = bFontWidth;    //Currently, this function is for Chinese EPG. 
}

void GDI_SetFontHeightInTable(BYTE bFontHeight)
{
    _bFontHeightInTable = bFontHeight;
}

void GDI_SetFontNumberInTable(WORD wFontNumber)
{
    _wFontNumberInTable = wFontNumber;
}

void GDI_ConfigRegionInfo(BYTE bRegionId, GDI_REGION_INFO *regionInfo)
{
    if (bRegionId >= NUMBER_OF_REGION)
        return;

    __RegionList[bRegionId].wWidth = regionInfo->wWidth;
    __RegionList[bRegionId].wHeight = regionInfo->wHeight;
    __RegionList[bRegionId].bColorMode= regionInfo->bColorMode;
    __RegionList[bRegionId].dwTAddr= regionInfo->dwTAddr;
}

void GDI_ConfigRegionAddress(BYTE bRegionId, DWORD dwTAddr)
{
    if (bRegionId >= NUMBER_OF_REGION)
        return;
    __RegionList[bRegionId].dwTAddr= dwTAddr;
}

//  *********************************************************************
//  Function    :   GDI_SetGDIPalEntry
//  Description :   This function sets the palette entries used by GDI module
//  Arguments   :   None
//  Return      :   None
//  Side Effect :
//  *********************************************************************

void GDI_SetGDIPalEntry(void)
{

#ifndef NO_GDI

    //Brian0.86
#ifdef GDI_4_BIT_OSD
    // load transparent color to ebtry 15
    GDI_LoadPalette(3);

    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_BRIGHT, GDI_VALUE_4B_COLOR_BRIGHT, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_DARK, GDI_VALUE_4B_COLOR_DARK, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_CENTER, GDI_VALUE_4B_COLOR_CENTER, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_BAR_BG, GDI_VALUE_4B_COLOR_BAR_BG, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_GENERAL_BG_BLACK, GDI_VALUE_4B_COLOR_GENERAL_BG_BLACK, TRUE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_GENERAL_BG_BLUE, GDI_VALUE_4B_COLOR_GENERAL_BG_BLUE, TRUE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_GREEN, GDI_VALUE_4B_COLOR_GREEN, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_YELLOW, GDI_VALUE_4B_COLOR_YELLOW, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_ORANGE, GDI_VALUE_4B_COLOR_ORANGE, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_BLACK, GDI_VALUE_4B_COLOR_BLACK, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_TEXT_DISABLED, GDI_VALUE_4B_COLOR_TEXT_DISABLED, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_RED, GDI_VALUE_4B_COLOR_RED, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_ORANGE_TRANSPARENT, GDI_VALUE_4B_COLOR_ORANGE_TRANSPARENT, TRUE);
    GDI_ChangePALEntry(GDI_ENTRY_4B_COLOR_BUTTON_NORMAL, GDI_VALUE_4B_COLOR_BUTTON_NORMAL, FALSE);

#endif

    //GDI_ChangePALEntry(GDI_ENTRY_BUTTON_COLOR_NORMAL, GDI_VALUE_BUTTON_COLOR_NORMAL, FALSE);
    //GDI_ChangePALEntry(GDI_ENTRY_BUTTON_COLOR_HIGHLIGHT, GDI_VALUE_BUTTON_COLOR_HIGHLIGHT, FALSE);
    //GDI_ChangePALEntry(GDI_ENTRY_BUTTON_COLOR_DISABLE, GDI_VALUE_BUTTON_COLOR_DISABLE, FALSE);
#endif

}

//  *********************************************************************
//  Function    :   GDI_InitialRegion
//  Description :   This function prepares the region by setting the header of BMP OSD
//  Arguments   :   bRegionId: the ID of the region to initialize,
//                             The ID is an index to the __RegionList
//  Return      :   NULL
//  Side Effect :
//  *********************************************************************

void GDI_InitialRegion(BYTE bRegionId)
{
	WIN_PARAMETER OSD;

#ifndef NO_GDI


    if (bRegionId >= NUMBER_OF_REGION)
        return;

	OSD.sHStart =REGION_X_NTSC; //176;//164; //136; //128; //117;//176; //117; //REGION_X_NTSC;
	OSD.sVStart =REGION_Y_NTSC; //72; //46; //72; //46; //REGION_Y_NTSC;
	OSD.wHWidth = __RegionList[bRegionId].wWidth;
	OSD.wVWidth  = __RegionList[bRegionId].wHeight;
	OSD.bEnable = FALSE; // still disable OSD region now


	// Set OSD size and position:
	// Set REG_MCU_VCR22 and REG_MCU_VCR23
	DISP_OSDSet(&OSD, OSD.wHWidth, OSD.wVWidth);

#ifndef GDI_STANDALONE_LIBRARY
    // if GDI is going to be a library, it should not call OSD module functions, because there may be no OSD module
    // so the AP need to adjust OSD position accroding TV mode and Scan mode.
        OSD_AdjustOSDPos();
#endif

	// Set REG_DISP_BRIGHT_CR

#ifdef SUPPORT_CT909S
/*
	DISP_OSDEffect(
					16,		// Blending mix ratio
					__RegionList[bRegionId].bColorMode  //GDI_OSD_8B_MODE // color mode
					);
*/

        // Brian2.17
        DISP_OSDSetMixRatio(GDI_GENERAL_MIX_RATIO);
        DISP_OSDSetColorMode(__RegionList[bRegionId].bColorMode);


#else
    DISP_OSDEffect(
					// Brian1.10, DISPLAY module will check the duplication by OSD output window
					//FALSE,	// Horizontal Duplication
					//FALSE,	// Vertical Duplication
					GDI_GENERAL_MIX_RATIO,		// Blending mix ratio
					__RegionList[bRegionId].bColorMode //FALSE   // 4-bit mode
					);

#endif

	// Set OSD region Base Address:
	// Set REG_MCU_VCR20
       // this variable is declared and refernced by DISP module.
        __dwOSD_Region_Base  = __RegionList[bRegionId].dwTAddr;


#endif  //#ifndef NO_GDI


}


void GDI_ChangeRegionHeight(BYTE bRegionId)
{


#ifndef NO_GDI

#ifndef GDI_STANDALONE_LIBRARY

	extern BYTE     __bMPEGVideoMode;



	WIN_PARAMETER OSD;
	PARM_RECT parm;
	BYTE bN2P=0;

	//return ;

	if (bRegionId >= NUMBER_OF_REGION)
		return;

	OSD.sHStart =REGION_X_NTSC;	// don't care, will adjust latter
	OSD.sVStart =REGION_Y_NTSC; // don't care, will adjust latter
	if (__bMPEGVideoMode == MPEG_VIDEO_MODE_NTSC)
	{
		if ( __RegionList[bRegionId].wHeight != GDI_REGION_HEIGHT)
		{
			__RegionList[bRegionId].wHeight = GDI_REGION_HEIGHT;
		}
		else
			return;
       }
	else
	{
		if ( __RegionList[bRegionId].wHeight != GDI_REGION_HEIGHT_PAL)
		{
			__RegionList[bRegionId].wHeight = GDI_REGION_HEIGHT_PAL;
			bN2P = 1;
		}
		else
			return;
	}

	OSD.wHWidth = __RegionList[bRegionId].wWidth;
	OSD.wVWidth  = __RegionList[bRegionId].wHeight;

	if (!_bGDIInit)
	{
		OSD.bEnable = FALSE;
	}
	else
	{
		OSD.bEnable = TRUE; // suppose OSD is enabled when calling this function
	}


	// Set OSD size and position:
	// Set REG_MCU_VCR22 and REG_MCU_VCR23
	DISP_OSDSet(&OSD, OSD.wHWidth, OSD.wVWidth);
	printf("PAL/NTSC mode change\n");

        __dwOSD_Region_Base  = __RegionList[bRegionId].dwTAddr;

	if (bN2P)
	{
		// Clear the extra PAL area
		parm.bColor = PAL_ENTRY_COLOR_TRANSPARENT; //14; //PAL_ENTRY_COLOR_TRANSPARENT; // transparenct color is the last entry
		parm.rect.wLeft = 0;
		parm.rect.wTop = GDI_REGION_HEIGHT;
		parm.rect.wRight = __RegionList[bRegionId].wWidth -1;
		parm.rect.wBottom = __RegionList[bRegionId].wHeight -1;

		GDI_FillRect(bRegionId, &parm);
	}



#endif


#endif  //#ifndef NO_GDI

}




//  *********************************************************************
//  Function    :   GDI_ClearRegion
//  Description :   This function clears the entire region to transparent color
//  Arguments   :   bRegionId: the ID of the region to initialize,
//                             The ID is an index to the __RegionList
//  Return      :   NOne
//  Side Effect :
//  *********************************************************************


void GDI_ClearRegion(BYTE bRegionId)
{

#ifndef NO_GDI
	PARM_RECT parm;

	parm.bColor = PAL_ENTRY_COLOR_TRANSPARENT; //14; //PAL_ENTRY_COLOR_TRANSPARENT; // transparenct color is the last entry
	parm.rect.wLeft = 0;
	parm.rect.wTop = 0;
	parm.rect.wRight = __RegionList[bRegionId].wWidth -1;
	parm.rect.wBottom = __RegionList[bRegionId].wHeight -1;

	GDI_FillRect(bRegionId, &parm);

#endif  //#ifndef NO_GDI

}



//  *********************************************************************
//  Function    :   GDI_ActivateRegion
//  Description :   This function prepares the region by setting the header of BMP OSD
//  Arguments   :   bPlaneId: the ID of the plane to be mapped with current region,
//                             The ID is an index to the __PlaneList
//  Return      :   TRUE:   function succeeded
//                  FALSE:  function failed
//  Side Effect :
//  *********************************************************************

void GDI_ActivateRegion(BYTE bRegionId)
{

#ifndef NO_GDI
        if (!_bGDIInit) // Brian1.20
        {
            return;
        }

	//DISP_OSDEnable(TRUE);
	DISP_DisplayCtrl(DISP_OSD, TRUE);

#endif  //#ifndef NO_GDI



}

//  *********************************************************************
//  Function    :   GDI_DeactivateRegion
//  Description :   This function deactivate the region mapped with plane
//  Arguments   :   none
//  Return      :   none
//  Side Effect :
//  *********************************************************************

void GDI_DeactivateRegion(void)
{
#ifndef NO_GDI

    //DISP_OSDEnable(FALSE);
	DISP_DisplayCtrl(DISP_OSD, FALSE);

       //OS_DelayTime(COUNT_50_MSEC); // Brian2.21, need to wait until completed
#ifndef SUPPORT_STB   //Alan2.31a, patch for the OSD disappearance bug of DVB
       DISP_DelayVSYNC(1);
#endif


#endif  //#ifndef NO_GDI

}

//  *********************************************************************
//  Function    :   GDI_MoveRegion
//  Description :   This function moves the entire region
//  Arguments   :   None
//  Global Var  :   __wGDIx, __wGDIy: the new start point of the region
//  Return      :   none
//  Side Effect :
//  *********************************************************************
void GDI_MoveRegion(BYTE bRegionId, SHORT sX, SHORT sY)
{

#ifndef NO_GDI

    WIN_PARAMETER OSD;


    if (bRegionId >= NUMBER_OF_REGION)
        return;

	OSD.sHStart = sX; //176;//164; //136; //128; //117;//176; //117; //REGION_X_NTSC;
	OSD.sVStart = sY; //72; //46; //72; //46; //REGION_Y_NTSC;
	OSD.wHWidth = __RegionList[bRegionId].wWidth;
	OSD.wVWidth  = __RegionList[bRegionId].wHeight;

#ifdef SUPPORT_CT909S
       if (REG_DISP_OSD_SIZE & 0x10000000) // bit 28: OSD enable  bit
#else
       if (REG_DISP_OSD_SIZE & 0x800) // bit 11: OSD enable  bit
#endif
       {
	    OSD.bEnable = TRUE;
       }
       else
       {
	    OSD.bEnable = FALSE;
       }

	// Set OSD size and position:
	// Set REG_MCU_VCR22 and REG_MCU_VCR23
	DISP_OSDSet(&OSD, OSD.wHWidth, OSD.wVWidth);

#endif  //#ifndef NO_GDI

}

//  *********************************************************************
//  Function    :   GDI_SetMixRatio
//  Description :   This function sets the mix ratio of the current region
//  Arguments   :   bMixRatio: the mix ratio, 0~63
//  Return      :   none
//  Side Effect :
//  *********************************************************************

void GDI_SetMixRatio(BYTE bMixRatio)
{

#ifndef NO_GDI

// be careful, better use a dedicate set mix ratio function

    // Set REG_DISP_BRIGHT_CR

#ifdef SUPPORT_CT909S
/*
	DISP_OSDEffect(
					bMixRatio,		// Blending mix ratio
					GDI_OSD_8B_MODE // color mode
					);
*/

        // Brian2.17
        DISP_OSDSetMixRatio(bMixRatio);
#else
    DISP_OSDEffect(
			        // Brian1.10, DISPLAY module will check the duplication by OSD output window
					//FALSE,	// Horizontal Duplication
					//FALSE,	// Vertical Duplication
					bMixRatio, // Blending mix ratio
					FALSE   // 4-bit mode
					);
#endif

#endif  //#ifndef NO_GDI

}

#if 0
DWORD COMUTL_RGB2YUV(DWORD dwRGB)
{
	BYTE _bCOMUTLTmp1, _bCOMUTLTmp2, _bCOMUTLTmp3;
	UDWORD uDWord;
	DWORD __dwCOMUTLTemp;

    // use uDWord.cS[0] ~ [3] to store YUV value
    // use _wCOMUTLTemp to store final value

    // Step 0: split RGB value
    _bCOMUTLTmp1=(RGB_R(dwRGB));
    _bCOMUTLTmp2=(RGB_G(dwRGB));
    _bCOMUTLTmp3=(RGB_B(dwRGB));

// Micky1.20, 4M flash, reduce common bank lib.
// don't use float point.
// can't use DWORD, must use signed operation
#ifdef  NO_FLOAT_POINT
    // Step 1: Calculate YUV value depending on input RGB
    uDWord.cS[0]=(BYTE)(16+(65738L*(long)_bCOMUTLTmp1
                      +129057L*(long)_bCOMUTLTmp2
                      +25064L*(long)_bCOMUTLTmp3)/256000);  // Y value
    uDWord.cS[1]=(BYTE)(128+(-37945L*(long)_bCOMUTLTmp1
                       -74494L*(long)_bCOMUTLTmp2
                       +112439L*(long)_bCOMUTLTmp3)/256000); // U value
    uDWord.cS[2]=(BYTE)(128+(112439L*(long)_bCOMUTLTmp1
                       -94154L*(long)_bCOMUTLTmp2
                       -18285L*((long)_bCOMUTLTmp3)/256000));  // V value
// debug the value between float and long
// float > long by 1.
/*
    uWord.cS[0]=(BYTE)(16+(65738*(float)_bCOMUTLTmp1
                      +129057*(float)_bCOMUTLTmp2
                      +25064*(float)_bCOMUTLTmp3)/256000);  // Y value
    uWord.cS[1]=(BYTE)(128+(-37945*(float)_bCOMUTLTmp1
                       -74494*(float)_bCOMUTLTmp2
                       +112439*(float)_bCOMUTLTmp3)/256000); // U value
    uWord.cS[1]=(BYTE)(128+(112439*(float)_bCOMUTLTmp1
                       -94154*(float)_bCOMUTLTmp2
                       -18285*(float)_bCOMUTLTmp3)/256000);  // V value
    if (uDWord.cS[2] != uWord.cS[1])
        printf("\nError");
*/
#else
    // Step 1: Calculate YUV value depending on input RGB
    uDWord.cS[0]=(BYTE)(16+(65738*(float)_bCOMUTLTmp1
                      +129057*(float)_bCOMUTLTmp2
                      +25064*(float)_bCOMUTLTmp3)/256000);  // Y value
    uDWord.cS[1]=(BYTE)(128+(-37945*(float)_bCOMUTLTmp1
                       -74494*(float)_bCOMUTLTmp2
                       +112439*((float)_bCOMUTLTmp3)/256000)); // U value
    uDWord.cS[2]=(BYTE)(128+(112439*(float)_bCOMUTLTmp1
                       -94154*(float)_bCOMUTLTmp2
                       -18285*((float)_bCOMUTLTmp3)/256000));  // V value
#endif

    // Step 2: merge YUV value into DWORD unit and match OSD spec.
	__dwCOMUTLTemp=(MAKE_RGB(uDWord.cS[0], uDWord.cS[1], uDWord.cS[2]));
    return  __dwCOMUTLTemp;

}
#endif


//  *********************************************************************
//  Function    :   GDI_ChangePALEntry
//  Description :   This function sets one palette entry
//  Arguments   :   bIdx: the entry index
//                  dwRGB:  the color value to set
//                  bMixEnable: TRUE/FALSE
//  Return      :   none
//  Side Effect :
//  *********************************************************************

void GDI_ChangePALEntry(BYTE bIdx, DWORD dwRGB, BYTE bMixEnable)
{

#ifndef NO_GDI
	DWORD dwTemp;

     if (bIdx == PAL_ENTRY_COLOR_TRANSPARENT) // Brian2.17, protect transparency entry
     {
        return;
     }

    // one palette entry uses one DWORD
    // if the highest byte == 0x5A, the parameter is already in YCbCr format
    //if ((dwRGB & 0xFF000000 )== GDI_VALUE_YUV)
    if ((dwRGB & 0xFE000000 )== GDI_VALUE_YUV) // bit 24 is used for mix_en, so skip this bit
    {
		dwTemp = (dwRGB & 0xFFFFFF); // it is YCbCr already
    }
    else
    {
		dwTemp =COMUTL_RGB2YUV(dwRGB);

    }

    // enable the mix_en bit [24]
    dwTemp |= (((DWORD)bMixEnable)<<24);

    // set desired entry value
    _dwGDIPalette[bIdx] = dwTemp;

#ifdef SUPPORT_CT909S
    DISP_SetPalette(_dwGDIPalette);
#else
    DISP_SetPalette(FALSE, _dwGDIPalette);
#endif

#endif  // #ifndef NO_GDI
}


// Brian0.95
void GDI_WaitPaletteComplete(void)
{
	//UTL_DelayTime(COUNT_50_MSEC, FALSE);

    //return;

	// Brian1.02,
#if 0 //ndef GDI_WITHOUT_OS
	// *** must use development tool >=1.02, otherwise the flag_time_wait will hang
    if (!OS_TimedWaitFlag(&__fDISPInfo, DISP_INFO_SETPALETTE_DONE, FLAG_WAITMODE_OR, COUNT_50_MSEC))
    {
        // wait palette complete time out
        printf("GDI wait palette complete time out");

    }
#else

#ifdef TEST909S
    __dwVSYNC = 0;
    while(__dwVSYNC<2);
#else
    //UTL_DelayTime(COUNT_50_MSEC, FALSE);
    // OS_DelayTime(COUNT_50_MSEC); //Brian1.15, reduce code size
    DISP_DelayVSYNC(1);
#endif

#endif


}


//  *********************************************************************
//  Function    :   GDI_LoadPalette
//  Description :   This function loads the palette for the plane
//  Arguments   :   pPaletteInfo: point to Palette Info.
//
//  Return      :   none
//  Side Effect :
//  *********************************************************************

//void GDI_LoadPalette(BYTE bPaletteId)
// Brian, support GDI library
void GDI_LoadPalette(GDI_PALETTE_INFO *pPaletteInfo)
{

#ifndef NO_GDI

	DWORD i;

	for (i=0; i<pPaletteInfo->pdwPalette[0]; i++)
	{

        _dwGDIPalette[i+pPaletteInfo->bStartNumber] = pPaletteInfo->pdwPalette[1+i];
	}

#ifdef SUPPORT_CT909S
    DISP_SetPalette(_dwGDIPalette);
#else
    DISP_SetPalette(FALSE, _dwGDIPalette);
#endif


    // always keep the transparent color.
    GDI_ChangePALEntry(PAL_ENTRY_COLOR_TRANSPARENT, 0x5a000000, FALSE); //transparency

    GDI_WaitPaletteComplete(); // Brian0.95

	//UTL_DelayTime(COUNT_50_MSEC, FALSE);

#endif  //#ifndef NO_GDI

}


// Brian2.21, support fill rect by S/W
void _gdi_SetPixel(DWORD dwAddr, BYTE bPos, BYTE bColorMode, BYTE bColor)
{
    BYTE *pbPtr, bData;

    pbPtr = (BYTE *) dwAddr;

    bData = *pbPtr;

    switch(bColorMode)
    {
        case GDI_OSD_8B_MODE:
            bData = bColor;
            break;

        case GDI_OSD_4B_MODE:

            /*
            if (bPos)
            {
                bData = (bData & 0x0F) | (bColor << 4);
            }
            else
            {
                bData = (bData & 0xF0) | (bColor);
            }
            */

            // leon, Big endian
             if (bPos)
            {
                bData = (bData & 0xF0) | (bColor);
            }
            else
            {
                bData = (bData & 0x0F) | (bColor << 4);
            }
            break;
         case GDI_OSD_2B_MODE:
            /*
            switch (bPos)
            {
                case 0:
                    bData = (bData & 0x3F) | (bColor << 6);
                    break;
                case 1:
                    bData = (bData & 0xCF) | (bColor << 4);
                    break;
                case 2:
                    bData = (bData & 0xF3) | (bColor << 2);
                    break;
                case 3:
                    bData = (bData & 0xFC) | bColor;
                    break;
            }
            */

            // leon, Big endian
            switch (bPos)
            {
                case 3:
                    bData = (bData & 0x3F) | (bColor << 6);
                    break;
                case 2:
                    bData = (bData & 0xCF) | (bColor << 4);
                    break;
                case 1:
                    bData = (bData & 0xF3) | (bColor << 2);
                    break;
                case 0:
                    bData = (bData & 0xFC) | bColor;
                    break;
            }

            break;

    }

    *pbPtr = bData;

}

#ifdef SUPPORT_GDI_FILL_RECT_BY_SW
void _gdi_FillRectBySW(BYTE bRegionId, PARM_RECT *pParm)
{
    DWORD i, j,  dwStartAddress, dwByteOffset, dwAddressCount; // the byte address
    DWORD dwRegionWidthInByte;
    BYTE bPositionStart, bPosition;

    /*
     if (bRegionId & GDI_SHADING_OPERATION)
    {
        bRegionId = bRegionId - GDI_SHADING_OPERATION;
    }

     if (bRegionId >= NUMBER_OF_REGION)
        return;

     if (pParm->rect.wLeft >= __RegionList[bRegionId].wWidth ||
        pParm->rect.wTop >= __RegionList[bRegionId].wHeight ||
        pParm->rect.wLeft > pParm->rect.wRight ||
        pParm->rect.wTop > pParm->rect.wBottom)
        return; // the position is not in the area

     if (pParm->rect.wRight == pParm->rect.wLeft) // Brian1.22, width need to be 2n, doesn't support 1-pixel width
     {
        return;
     }

    if (pParm->rect.wBottom >= __RegionList[bRegionId].wHeight)
		pParm->rect.wBottom = __RegionList[bRegionId].wHeight-1;

    if (pParm->rect.wRight >= __RegionList[bRegionId].wWidth)
        pParm->rect.wRight = __RegionList[bRegionId].wWidth-1;

    */


     dwRegionWidthInByte = __RegionList[bRegionId].wWidth >> __RegionList[bRegionId].bColorMode;

    dwByteOffset = ((DWORD)pParm->rect.wTop*__RegionList[bRegionId].wWidth +  pParm->rect.wLeft) >> __RegionList[bRegionId].bColorMode;
    bPositionStart = ((DWORD)pParm->rect.wTop*__RegionList[bRegionId].wWidth +  pParm->rect.wLeft) % (1<< __RegionList[bRegionId].bColorMode);

    dwStartAddress = __RegionList[bRegionId].dwTAddr + dwByteOffset;

    //printf("base = %lx, dwByteOffset = %lx, dwStartAddress = %lx", __RegionList[bRegionId].dwTAddr, dwByteOffset, dwStartAddress);
    //printf("Width = %lx, Height = %lx", __RegionList[bRegionId].wWidth, __RegionList[bRegionId].wHeight);



    for (i = 0 ; i<= pParm->rect.wBottom - pParm->rect.wTop; i++)
    {
        dwAddressCount = dwStartAddress + dwRegionWidthInByte*i;
        bPosition = bPositionStart;

        for (j = 0 ; j<= pParm->rect.wRight- pParm->rect.wLeft; j++)
        {
                _gdi_SetPixel(dwAddressCount, bPosition, __RegionList[bRegionId].bColorMode, pParm->bColor);

                switch(__RegionList[bRegionId].bColorMode)
                {
                    case GDI_OSD_8B_MODE:
                        dwAddressCount++;
                        break;

                    case GDI_OSD_4B_MODE:
                        bPosition++;
                        if (bPosition == 2)
                        {
                            bPosition = 0;
                            dwAddressCount++;
                        }
                        break;
                     case GDI_OSD_2B_MODE:
                        bPosition++;
                        if (bPosition == 4)
                        {
                            bPosition = 0;
                            dwAddressCount++;
                        }
                        break;

                }


        }


    }


}
#endif  // SUPPORT_GDI_FILL_RECT_BY_SW




void GDI_FillRect(BYTE bRegionId, PARM_RECT *pParm)
{
    DWORD i, dwIndex;
    PARM_RECT rect;

    if (bRegionId & GDI_SHADING_OPERATION)
    {
        bRegionId = bRegionId - GDI_SHADING_OPERATION;

        rect.rect.wLeft = pParm->rect.wLeft;
        rect.rect.wRight = pParm->rect.wRight;

        dwIndex = 0;
        for (i=pParm->rect.wTop; i<=pParm->rect.wBottom; i++)
        {
            rect.rect.wTop = i;
            rect.rect.wBottom = i;
            rect.bColor = pParm->bShadePtr[dwIndex];
            _gdi_FillRect(bRegionId, &rect);

            if (dwIndex <  pParm->bColor-1)
            {
                dwIndex++;
            }

        }

    }
    else
    {
        _gdi_FillRect(bRegionId, pParm);

    }

}


//  *********************************************************************
//  Function    :   GDI_FillRect
//  Description :   This function fills the rectangle
//  Arguments   :   pParm points to the PARM_RECT
//  Return      :   none
//  Side Effect :
//  Note        :
//  *********************************************************************
void _gdi_FillRect(BYTE bRegionId, PARM_RECT *pParm)
{
    DWORD dwAddress;
    WORD wOp_width, wOp_height;
    WORD wAg_width, wAg_offset;
	WORD wOSDWidth, wOSDHeight;
	DWORD dwOSDBase;
	BYTE b4bitMode;


    if (bRegionId & GDI_SHADING_OPERATION)
    {
        bRegionId = bRegionId - GDI_SHADING_OPERATION;
    }


    if (bRegionId >= NUMBER_OF_REGION)
        return;


	wOSDWidth = __RegionList[bRegionId].wWidth;
	wOSDHeight = __RegionList[bRegionId].wHeight;
	dwOSDBase = __RegionList[bRegionId].dwTAddr;
	b4bitMode = __RegionList[bRegionId].bColorMode;


	// do boundary check
    if (pParm->rect.wLeft >= wOSDWidth ||
        pParm->rect.wTop >= wOSDHeight ||
        pParm->rect.wLeft > pParm->rect.wRight ||
        pParm->rect.wTop > pParm->rect.wBottom)
        return; // the position is not in the area

	if (pParm->rect.wBottom >= wOSDHeight)
		pParm->rect.wBottom = wOSDHeight-1;

    // brian2.21, also protect width range
    if (pParm->rect.wRight >= wOSDWidth)
		pParm->rect.wRight = wOSDWidth-1;


	if (pParm->rect.wRight == pParm->rect.wLeft) // Brian1.22, width need to be 2n, doesn't support 1-pixel width
        return;


#ifdef SUPPORT_GDI_FILL_RECT_BY_SW
    // brian2.21, 4-bit OSD mode fill DRAM directly
    if (__RegionList[bRegionId].bColorMode == GDI_OSD_4B_MODE)
    {
        _gdi_FillRectBySW(bRegionId, pParm);
        return;
    }
#endif



#ifndef GDI_WITHOUT_OS
    OS_LockMutex(&__mutexGDIIdle);
#endif

    // Reset VPU
     _gdi_ResetVPU();

#ifdef SUPPORT_CT909S
    REG_GPU_CTL0 |= GPU_OP; // enable gpu_op
#endif


    if (pParm->rect.wRight >= wOSDWidth)
        pParm->rect.wRight = wOSDWidth-1;

#ifdef SUPPORT_CT909S
    if (b4bitMode == GDI_OSD_2B_MODE)
    {
        wOSDWidth = ((wOSDWidth+15)>>4)*16;
    }
#endif


    dwAddress = dwOSDBase + (((DWORD)pParm->rect.wTop*wOSDWidth + pParm->rect.wLeft)>>b4bitMode);


#ifdef SUPPORT_CT909S
    wOp_width = ((pParm->rect.wRight - pParm->rect.wLeft + 1) & 0x3ff)>>b4bitMode;   // use 10 bit
    wOp_height = ((pParm->rect.wBottom - pParm->rect.wTop + 1) &0x3ff);   // use 10 bit
    wAg_width = (wOp_width + (dwAddress & 0x3) + 3) >> 2;                       // 4-byte alignment
    wAg_offset = (((wOSDWidth>>b4bitMode) + 3)>>2) - wAg_width + 1;     // 4-byte alignment
#else
    wOp_width = ((pParm->rect.wRight - pParm->rect.wLeft + 1) & 0xfff)>>b4bitMode;   // use 12 bit
    wOp_height = ((pParm->rect.wBottom - pParm->rect.wTop + 1) &0x7ff);   // use 11 bit
    wAg_width = (wOp_width + (dwAddress & 0x7) + 7) >> 3;
    wAg_offset = (((wOSDWidth>>b4bitMode) + 7)>>3) - wAg_width + 1;
#endif




    if (b4bitMode == GDI_OSD_4B_MODE)
    {
        pParm->bColor = (pParm->bColor << 4) | pParm->bColor;
    }
#ifdef SUPPORT_CT909S
    else if (b4bitMode == GDI_OSD_2B_MODE)
    {
        pParm->bColor = (pParm->bColor << 6) | (pParm->bColor << 4) | (pParm->bColor << 2) | pParm->bColor;
    }
#endif



    REG_GPU_CTL1 = (pParm->bColor << 24);
    REG_GPU_OP_SIZE = (wOp_height << 16) + wOp_width;

#ifdef SUPPORT_CT909S
    REG_GPU_AG_OFF = (wAg_offset) << 16; // ag_width is not used in CT909S

#else
    REG_GPU_AG_OFF = ((wAg_width << 8) + wAg_offset) << 16;
#endif

    REG_GPU_SRC_ADDR = 0;
    REG_GPU_DEST_ADDR = dwAddress;

    // Brian1.02
#ifdef GDI_CHECK_DISP_MEM_LINE
{
    DWORD dwStartTick;

    dwStartTick = OS_GetSysTimer();

    while (OS_GetSysTimer() -dwStartTick <30) // check for 30 msec
    {

#ifdef SUPPORT_CT909S
        if ( (REG_DISP_MEM_LINE & 0x3FF) < (pParm->rect.wTop) || // + REGION_Y_NTSC -10) ||
              (REG_DISP_MEM_LINE & 0x3FF) > (pParm->rect.wBottom + REGION_Y_PAL))  //REGION_Y_NTSC))
#else
        if ( (REG_DISP_MEM_LINE & 0x7FF) < (pParm->rect.wTop) || // + REGION_Y_NTSC -10) ||
              (REG_DISP_MEM_LINE & 0x7FF) > (pParm->rect.wBottom + REGION_Y_PAL))  //REGION_Y_NTSC))
#endif
        {

            break;
        }
        MACRO_IDLE();
        MACRO_IDLE();
    }
}

#endif

    asm volatile ( "" : : : "memory" );  //HAL_REORDER_BARRIER();

#ifdef SUPPORT_CT909S

#ifdef GDI_CHECK_DISP_MEM_LINE_BY_HW
    REG_GPU_OP_THRE = (pParm->rect.wTop << 16) | (pParm->rect.wBottom + REGION_Y_PAL);

    REG_GPU_CTL0 = GPU_INT_EN |GPU_OP | (4 << 16)  | (4 << 20) | (GPU_FILLRECTANGLE << 2) | GPU_START | GPU_OP_JUDGE_EN;

#else
    REG_GPU_CTL0 = GPU_INT_EN |GPU_OP | (4 << 16)  | (4 << 20) | (GPU_FILLRECTANGLE << 2) | GPU_START;
#endif



#else
    REG_GPU_CTL0 = GPU_INT_EN | GPU_REQ_EN | GPU_W_REL_EN | (0x10 << 16) | (GPU_FILLRECTANGLE << 2) | GPU_START;
#endif

/*
	// wait command complete
	while (REG_GPU_CTL0 & GPU_STATUS)
	{
		// need time out check here
	}
*/
    _gdi_CheckCmdComplete();

#ifndef GDI_WITHOUT_OS
    OS_UnlockMutex(&__mutexGDIIdle);
#endif

}


void GDI_FillArc_BottomLeft(BYTE bRegionId, PARM_ARC *pParm)
{
    PARM_RECT rect;
    DWORD i, dwIndex;
    BYTE bShading=0;


    rect.bColor = pParm->bColor;
    dwIndex = 0;

    if (bRegionId & GDI_SHADING_OPERATION)
    {
        bRegionId = bRegionId -GDI_SHADING_OPERATION;
        bShading = 1;
    }

    for (i=0; i<pParm->bArc; i++)
    {
        rect.rect.wTop = rect.rect.wBottom = pParm->wY+i;
        rect.rect.wLeft = pParm->wX+ _ArcOffsetPtr[i]; //aArc_15[i];
        rect.rect.wRight = pParm->wX+pParm->bArc-1;

        if (bShading)
        {
            rect.bColor = pParm->bShadePtr[dwIndex];
        }

        GDI_FillRect(bRegionId, &rect);

        if (dwIndex <  pParm->bColor-1)
        {
                dwIndex++;
        }

    }
}

void GDI_FillArc_BottomRight(BYTE bRegionId, PARM_ARC *pParm)
{
    PARM_RECT rect;
    DWORD i, dwIndex;
    BYTE bShading=0;


    rect.bColor = pParm->bColor;
    dwIndex = 0;

    if (bRegionId & GDI_SHADING_OPERATION)
    {
        bRegionId = bRegionId -GDI_SHADING_OPERATION;
        bShading = 1;
    }

    for (i=0; i<pParm->bArc; i++)
    {
        rect.rect.wTop = rect.rect.wBottom = pParm->wY+i;
        rect.rect.wLeft = pParm->wX;
        rect.rect.wRight = pParm->wX+pParm->bArc-1-_ArcOffsetPtr[i];

        if (bShading)
        {
            rect.bColor = pParm->bShadePtr[dwIndex];
        }

        GDI_FillRect(bRegionId, &rect);

        if (dwIndex <  pParm->bColor-1)
        {
                dwIndex++;
        }
    }
}

void GDI_FillArc_TopLeft(BYTE bRegionId, PARM_ARC *pParm)
{
    PARM_RECT rect;
    DWORD i, dwArc, dwIndex;
    BYTE bShading=0;

    dwArc = pParm->bArc;
    rect.bColor = pParm->bColor;
    dwIndex = 0;

    if (bRegionId & GDI_SHADING_OPERATION)
    {
        bRegionId = bRegionId -GDI_SHADING_OPERATION;
        bShading = 1;
    }

    //printf("GDI_FillArc_TopLeft\n");

    for (i=0; i<dwArc; i++)
    {
        rect.rect.wTop = rect.rect.wBottom = pParm->wY+i;
        rect.rect.wLeft = pParm->wX+_ArcOffsetPtr[dwArc-i-1];
        rect.rect.wRight = pParm->wX+dwArc-1;

        if (bShading)
        {
            rect.bColor = pParm->bShadePtr[dwIndex];
        }

        GDI_FillRect(bRegionId, &rect);

        if (dwIndex <  pParm->bColor-1)
        {
                dwIndex++;
        }

        //printf("x = %d, y = %d\n", rect.rect.wLeft, rect.rect.wTop);
    }
}

void GDI_FillArc_TopRight(BYTE bRegionId, PARM_ARC *pParm)
{
    PARM_RECT rect;
    DWORD i, dwArc, dwIndex;
    BYTE bShading=0;

    dwArc = pParm->bArc;
    rect.bColor = pParm->bColor;
    dwIndex = 0;

    if (bRegionId & GDI_SHADING_OPERATION)
    {
        bRegionId = bRegionId -GDI_SHADING_OPERATION;
        bShading = 1;
    }

    for (i=0; i<dwArc; i++)
    {
        rect.rect.wTop = rect.rect.wBottom = pParm->wY+i;
        rect.rect.wLeft = pParm->wX;
        rect.rect.wRight = pParm->wX+dwArc-1-_ArcOffsetPtr[dwArc-i-1];

        if (bShading)
        {
            rect.bColor = pParm->bShadePtr[dwIndex];
        }

        GDI_FillRect(bRegionId, &rect);

        if (dwIndex <  pParm->bColor-1)
        {
                dwIndex++;
        }

        //printf("x = %d, y = %d\n", rect.rect.wRight, rect.rect.wTop);
    }
}


BYTE *  _GDI_SetArcPointer(BYTE bArc)
{
    BYTE *ptr;

    if (bArc & GDI_SHADING_OPERATION)
    {
        bArc = bArc -GDI_SHADING_OPERATION; // remove the shading bit
    }


    switch (bArc)
    {
        case 13:
            ptr = _aArc_13;
            break;
        case 15:
            ptr = _aArc_15;
            break;
        case 17:
            ptr = _aArc_17;
            break;
        case 19:
            ptr = _aArc_19;
            break;
        case 21:
            ptr = _aArc_21;
            break;
        case 23:
            ptr = _aArc_23;
            break;
        case 25:
            ptr = _aArc_25;
            break;
        default:
            ptr = _aArc_15;
#ifndef GDI_NO_PRINTF
            printf("Incorrect Arc size\n");
#endif

    }

    return ptr;

}


//  *********************************************************************
//  Function    :   GDI_FillRoundRect
//  Description :   This function fills a round  rectangle
//  Arguments     :   bRegionId: the region ID
//                      :   bArc : the arc size: can be one of the value 13, 15, 17 1,9, 21, 23, 25
//                      :  pParm points to the PARM_RECT
//  Return      :   none
//  Side Effect :
//  Note        :
//  *********************************************************************
void GDI_FillRoundRect(BYTE bRegionId, BYTE bArc, PARM_RECT *pParm)
{
    PARM_ARC parmArc;
    PARM_RECT parmRect;
    BYTE *bPtr;


    // set arc data pointer
    _ArcOffsetPtr =_GDI_SetArcPointer(bArc);
    bPtr = pParm->bShadePtr;

    // fill top-left arc
    parmArc.bArc = bArc;
    parmArc.bColor = pParm->bColor;
    parmArc.wX = pParm->rect.wLeft;
    parmArc.wY = pParm->rect.wTop;
    parmArc.bShadePtr = bPtr;
    GDI_FillArc_TopLeft(bRegionId, &parmArc);

    // fill top rect
    parmRect.bColor = pParm->bColor;
    parmRect.rect.wLeft = pParm->rect.wLeft + bArc;
    parmRect.rect.wTop = pParm->rect.wTop;
    parmRect.rect.wRight = pParm->rect.wRight - bArc;
    parmRect.rect.wBottom = pParm->rect.wTop + bArc - 1;
    parmRect.bShadePtr =bPtr;
    GDI_FillRect(bRegionId, &parmRect);


    // fill top-right arc
    //parmArc.bArc = bArc;
    //parmArc.bColor = pParm->bColor;
    parmArc.wX = parmRect.rect.wRight + 1;
    //parmArc.wY = pParm->rect.wTop;
    GDI_FillArc_TopRight(bRegionId, &parmArc);


    bPtr +=bArc;

    // fill center rect
    //pamRect.bColor = pParm->bColor;
    parmRect.rect.wLeft = pParm->rect.wLeft;
    parmRect.rect.wTop = parmRect.rect.wBottom + 1;
    parmRect.rect.wRight = pParm->rect.wRight;
    parmRect.rect.wBottom = pParm->rect.wBottom - bArc;
    parmRect.bShadePtr =bPtr;

    GDI_FillRect(bRegionId, &parmRect);

    bPtr += (parmRect.rect.wBottom - parmRect.rect.wTop + 1);


    // fill bottom-left arc
    //parmArc.bArc = bArc;
    //parmArc.bColor = pParm->bColor;
    parmArc.wX = pParm->rect.wLeft;
    parmArc.wY = parmRect.rect.wBottom + 1;
    parmArc.bShadePtr = bPtr;
    GDI_FillArc_BottomLeft(bRegionId, &parmArc);


    // fill bottom rect
    //pamRect.bColor = pParm->bColor;
    parmRect.rect.wLeft = pParm->rect.wLeft + bArc;
    parmRect.rect.wTop = parmArc.wY;
    parmRect.rect.wRight = pParm->rect.wRight - bArc;
    parmRect.rect.wBottom = pParm->rect.wBottom;
    parmRect.bShadePtr =bPtr;
    GDI_FillRect(bRegionId, &parmRect);


    // fill bottom-right arc
    //parmArc.bArc = bArc;
    //parmArc.bColor = pParm->bColor;
    parmArc.wX = parmRect.rect.wRight + 1;
    //parmArc.wY = pParm->rect.wTop;
    GDI_FillArc_BottomRight(bRegionId, &parmArc);


}

//  *********************************************************************
//  Function    :   GDI_FillRoundRectTop
//  Description :   This function fills a rectangle with top round corner
//  Arguments     :   bRegionId: the region ID
//                      :   bArc : the arc size: can be one of the value 13, 15, 17 1,9, 21, 23, 25
//                      :  pParm points to the PARM_RECT
//  Return      :   none
//  Side Effect :
//  Note        :
//  *********************************************************************
void GDI_FillRoundRectTop(BYTE bRegionId, BYTE bArc, PARM_RECT *pParm)
{
    PARM_ARC parmArc;
    PARM_RECT parmRect;


    // set arc data pointer
    _ArcOffsetPtr = _GDI_SetArcPointer(bArc);

    // fill top-left arc
    parmArc.bArc = bArc;
    parmArc.bColor = pParm->bColor;
    parmArc.wX = pParm->rect.wLeft;
    parmArc.wY = pParm->rect.wTop;
    parmArc.bShadePtr = pParm->bShadePtr;
    GDI_FillArc_TopLeft(bRegionId, &parmArc);

    // fill top rect
    parmRect.bColor = pParm->bColor;
    parmRect.rect.wLeft = pParm->rect.wLeft + bArc;
    parmRect.rect.wTop = pParm->rect.wTop;
    parmRect.rect.wRight = pParm->rect.wRight - bArc;
    parmRect.rect.wBottom = pParm->rect.wTop + bArc - 1;
    parmRect.bShadePtr = pParm->bShadePtr;
    GDI_FillRect(bRegionId, &parmRect);

    // fill top-right arc
    //parmArc.bArc = bArc;
    //parmArc.bColor = pParm->bColor;
    parmArc.wX = parmRect.rect.wRight + 1;
    //parmArc.wY = pParm->rect.wTop;
    GDI_FillArc_TopRight(bRegionId, &parmArc);

    // fill center rect

    if (bRegionId & GDI_SHADING_OPERATION)
    {
        if (parmRect.bColor > bArc)
        {
                parmRect.bColor -= bArc;
        }

    }
    parmRect.rect.wLeft = pParm->rect.wLeft;
    parmRect.rect.wTop = parmRect.rect.wBottom + 1;
    parmRect.rect.wRight = pParm->rect.wRight;
    parmRect.rect.wBottom = pParm->rect.wBottom;
    parmRect.bShadePtr = pParm->bShadePtr + bArc;
    GDI_FillRect(bRegionId, &parmRect);

}

//  *********************************************************************
//  Function    :   GDI_FillRoundRectBottom
//  Description :   This function fills a rectangle with bottom round corner
//  Arguments     :   bRegionId: the region ID
//                      :   bArc : the arc size: can be one of the value 13, 15, 17 1,9, 21, 23, 25
//                      :  pParm points to the PARM_RECT
//  Return      :   none
//  Side Effect :
//  Note        :
//  *********************************************************************
void GDI_FillRoundRectBottom(BYTE bRegionId, BYTE bArc,  PARM_RECT *pParm)
{
    PARM_ARC parmArc;
    PARM_RECT parmRect;
    DWORD dwInc=0;



    // set arc data pointer
    _ArcOffsetPtr = _GDI_SetArcPointer(bArc);

    // fill center rect
    parmRect.bColor = pParm->bColor;
    parmRect.rect.wLeft = pParm->rect.wLeft;
    parmRect.rect.wTop = pParm->rect.wTop;
    parmRect.rect.wRight = pParm->rect.wRight;
    parmRect.rect.wBottom = pParm->rect.wBottom - bArc;
    parmRect.bShadePtr = pParm->bShadePtr;
    GDI_FillRect(bRegionId, &parmRect);


    if (bRegionId & GDI_SHADING_OPERATION)
    {
        dwInc = (parmRect.rect.wBottom - parmRect.rect.wTop +1);

        if (parmRect.bColor > dwInc)
        {
                parmRect.bColor -= dwInc;
                parmArc.bColor = parmRect.bColor;
        }
        else
        {
            dwInc = parmRect.bColor-1;
            parmRect.bColor = 1;
            parmArc.bColor = parmRect.bColor;

        }

    }
    else
    {
        parmArc.bColor = pParm->bColor;
    }


    // fill bottom-left arc
    parmArc.bArc = bArc;
    //parmArc.bColor = pParm->bColor;
    parmArc.wX = pParm->rect.wLeft;
    parmArc.wY = parmRect.rect.wBottom + 1;
    parmArc.bShadePtr = pParm->bShadePtr + dwInc;
    GDI_FillArc_BottomLeft(bRegionId, &parmArc);


    // fill bottom rect
    //pamRect.bColor = pParm->bColor;
    parmRect.rect.wLeft = pParm->rect.wLeft + bArc;
    parmRect.rect.wTop = parmArc.wY;
    parmRect.rect.wRight = pParm->rect.wRight - bArc;
    parmRect.rect.wBottom = pParm->rect.wBottom;
    parmRect.bShadePtr = parmArc.bShadePtr;
    GDI_FillRect(bRegionId, &parmRect);

    // fill bottom-right arc
    //parmArc.bArc = bArc;
    //parmArc.bColor = pParm->bColor;
    parmArc.wX = parmRect.rect.wRight + 1;
    //parmArc.wY = pParm->rect.wTop;
    GDI_FillArc_BottomRight(bRegionId, &parmArc);


}


//  *********************************************************************
//  Function    :   GDI_FillRoundRectBottom
//  Description :   This function draws a round rectangle
//  Arguments     :   bRegionId: the region ID
//                      :   bArcStart / bArcEnd : the arc size: can be one of the value 13, 15, 17 1,9, 21, 23, 25
//                      :   bArcStart must > bArcEnd
//                      :  pParm points to the PARM_RECT
//  Return      :   none
//  Side Effect :
//  Note        :
//  *********************************************************************
void GDI_DrawRoundRect(BYTE bRegionId, BYTE bArcStart, BYTE bArcEnd, PARM_RECT *pParm)
{
    PARM_RECT rect;
    BYTE *pbArcStart, *pbArcEnd;
    DWORD i, dwRightArcStart, dwGap;


    // set arc data pointer
    pbArcStart = _GDI_SetArcPointer(bArcStart);
    pbArcEnd = _GDI_SetArcPointer(bArcEnd);

    dwRightArcStart = pParm->rect.wRight - pParm->rect.wLeft + 1 - bArcStart;
    dwGap = bArcStart - bArcEnd;

    rect.bColor = pParm->bColor;

    for (i= 0; i<=bArcStart - bArcEnd-1; i++)
    {
        rect.rect.wTop = rect.rect.wBottom = pParm->rect.wTop+i;
        rect.rect.wLeft = pParm->rect.wLeft + pbArcStart[bArcStart-i-1];
        rect.rect.wRight = pParm->rect.wLeft + dwRightArcStart + bArcStart-1-pbArcStart[bArcStart-i-1];
        GDI_FillRect(bRegionId, &rect);
    }

    // printf ("GDI_DrawRoundRect\n");

    for (i= bArcStart-bArcEnd ; i<bArcStart; i++)
    {
        //asm volatile ( "" : : : "memory" );
        rect.rect.wTop = rect.rect.wBottom = pParm->rect.wTop+i;
        rect.rect.wLeft = pParm->rect.wLeft + pbArcStart[bArcStart-i-1];
        rect.rect.wRight = pParm->rect.wLeft + dwGap + pbArcEnd[bArcEnd-i-1 + dwGap] - 1;
        GDI_FillRect(bRegionId, &rect);
        //printf("x1 = %d, y1 = %d, x2 = %d, y2 = %d\n", rect.rect.wLeft, rect.rect.wTop, rect.rect.wRight, rect.rect.wBottom);


        //asm volatile ( "" : : : "memory" );

        rect.rect.wLeft = pParm->rect.wLeft + dwRightArcStart + bArcEnd-1-pbArcEnd[bArcEnd-i-1 + dwGap] + 1;
        rect.rect.wRight = pParm->rect.wLeft + dwRightArcStart + bArcStart-1-pbArcStart[bArcStart-i-1];
        GDI_FillRect(bRegionId, &rect);
        // printf("x1 = %d, y1 = %d, x2 = %d, y2 = %d\n", rect.rect.wLeft, rect.rect.wTop, rect.rect.wRight, rect.rect.wBottom);
    }


    // fill left vertical rect
    rect.rect.wLeft = pParm->rect.wLeft;
    rect.rect.wTop = pParm->rect.wTop + bArcStart;
    rect.rect.wRight = pParm->rect.wLeft + bArcStart - bArcEnd-1;
    rect.rect.wBottom = pParm->rect.wBottom - bArcStart;
    GDI_FillRect(bRegionId, &rect);

    // fill right vertical rect
    rect.rect.wLeft = pParm->rect.wRight - (bArcStart - bArcEnd)+1;
    rect.rect.wTop = pParm->rect.wTop + bArcStart;
    rect.rect.wRight = pParm->rect.wRight;
    rect.rect.wBottom = pParm->rect.wBottom - bArcStart;
    GDI_FillRect(bRegionId, &rect);


    for (i= 0 ; i<bArcEnd; i++)
    {
        rect.rect.wTop = rect.rect.wBottom = pParm->rect.wBottom - bArcStart + 1 + i;
        rect.rect.wLeft = pParm->rect.wLeft + pbArcStart[i];
        rect.rect.wRight = pParm->rect.wLeft + dwGap +  pbArcEnd[i]-1;
        GDI_FillRect(bRegionId, &rect);

        rect.rect.wLeft = pParm->rect.wLeft + dwRightArcStart + bArcEnd -1-pbArcEnd[i] + 1;
        rect.rect.wRight = pParm->rect.wLeft + dwRightArcStart + bArcStart -1-pbArcStart[i];
        GDI_FillRect(bRegionId, &rect);
    }


    for (i= bArcEnd; i<bArcStart; i++)
    {
        rect.rect.wTop = rect.rect.wBottom = pParm->rect.wBottom - bArcStart + 1 + i;
        rect.rect.wLeft = pParm->rect.wLeft + pbArcStart[i];
        rect.rect.wRight = pParm->rect.wLeft + dwRightArcStart + bArcStart-1-pbArcStart[i];
        GDI_FillRect(bRegionId, &rect);
    }





}




//  *********************************************************************
//  Function    :   GDI_LoadBMP
//  Description :   This function load the BMP data from code array into BMP OSD region
//              :   the region is decided by the __bCurrentRegionId
//  Arguments   :   pParm: point to a Bitmap structure
//                  The Bitmap data is generated by bmp2data tool
//  Return      :   None
//  Side Effect :
//  ** Note **  :   In order to speed up the load process and reduce the complexity
//              :   We have made the following assumption, and we must follow it.
//              :   [1] bitmap width is 4-pixel alignment
//              :   [2] bitmap height is even
//              :   [3] The destination X-coordinate must be 4-pixel alignment
//              :   [4] The destination Y-coordinate must be even
//  *********************************************************************


// Brian, support GDI library
void GDI_LoadBMP(DWORD **aPtr, DWORD dwMaxNum)
{

    // Brian2.11, copy BMP directly from BMPArray, this function is not needed anymore

    /*
#ifndef NO_GDI

    DWORD i, j, dwSize;
    DWORD *dwPtr;


    __pBMPAryptr = aPtr; // keep the last BMP array

    dwPtr = (DWORD *)GDI_BMP_BASE_ADDRESS;

    for (i=0; i<dwMaxNum; i++)
    {

        dwSize = (aPtr[i])[0];
        for (j=0; j<dwSize; j++)
        {
            (*(volatile DWORD*)(dwPtr++) )= (aPtr[i])[j+4];
        }

    }

#endif  //#ifndef NO_GDI
*/


}

//  *********************************************************************
//  Function    :   GDI_DrawBitmapBySW
//  Description :   This function copies the bmp to the specified location
//                       *** the bmp source is put in a continuous dram address space
//  Arguments   :   pParm points to the PARM_DRAW_BITMAP
//  Return      :   none
//  Side Effect :
//  Note        :
//  *********************************************************************

//*** Only support 8/4/2-bit OSD mode
//*** x position and width of BMP must be DWORD alignment
//*** for example:
//***      8-bit mode:   x pos must be 4n, BMP width must be 4n
//***      4-bit mode:   x pos must be 8n, BMP width must be 8n
//***      16-bit mode: x pos must be 16n, BMP width must be 16n

void	GDI_DrawBitmapBySW(PARM_DRAW_BITMAP *pParm, const DWORD ** BMPAryptr)
{
    DWORD i, j,  dwStartAddress, dwByteOffset, dwAddressCount; // the byte address
    DWORD dwRegionWidthInByte;
    BYTE bRegionId;
    DWORD   dwBMPIndex;
    WORD wOSDSrcWidth, wOSDSrcHeight, wX, wY;
    DWORD  *pdwPtr, *pdwBMPPtr;
    DWORD dwColorMode, dwAlignment;


    bRegionId = pParm->bDestRegion;
    dwColorMode = __RegionList[bRegionId].bColorMode;


    dwBMPIndex = pParm->bBitmapID;

    dwAlignment = 4 << (dwColorMode);

    wX = ((pParm->wDestX)/dwAlignment) * dwAlignment;  // DWORD alignment for 8/4/2-bit case.
    wY = pParm->wDestY;
    
    wOSDSrcWidth = (BMPAryptr[dwBMPIndex])[2];   // get the width of BMP
    wOSDSrcHeight = (BMPAryptr[dwBMPIndex])[3];  // get the height of BMP
    
    dwRegionWidthInByte = __RegionList[bRegionId].wWidth >> dwColorMode;

    dwByteOffset = ((DWORD)wY*__RegionList[bRegionId].wWidth +  wX) >> dwColorMode;

    dwStartAddress = __RegionList[bRegionId].dwTAddr + dwByteOffset;


    pdwBMPPtr = (DWORD *) (BMPAryptr[dwBMPIndex]+4);
    
    for (i = 0 ; i<wOSDSrcHeight; i++)
    {
        dwAddressCount = dwStartAddress + dwRegionWidthInByte*i;
        pdwPtr = (DWORD *) dwAddressCount;


        for (j = 0 ; j<wOSDSrcWidth/dwAlignment; j++)   // write one DWORD each time
        {              
                *pdwPtr++  = *pdwBMPPtr++;
        }
    }

}

//Aron2.55, support drawing of compressed bitmap data.
//  *********************************************************************
//  Function    :   GDI_DrawZipBitmap
//  Description :   This function decompressed the zipped Bitmap and call 
//                      GDI_DrawBitmap to draw it
//  Arguments   :   pParm points to the PARM_DRAW_BITMAP
//                  point array of zipped bitmap data
//                  pbDest decompressing destination
//                  dwZIPWM unzipped window buffer
//  Return      :   none
//  Side Effect :
//  Note        :
//  *********************************************************************
void	GDI_DrawZipBitmap(PARM_DRAW_BITMAP *pParm, const BYTE **BMPAry, PBYTE pbDest, DWORD dwZIPWM)
{
    PARM_DRAW_BITMAP parm;
    DWORD *BMPArray[1];

    UTL_Decompress((PBYTE)BMPAry[pParm->bBitmapID], (PBYTE)pbDest, (PBYTE)dwZIPWM);
    
    parm = *pParm;
    parm.bBitmapID = 0;
    
    BMPArray[0] = (DWORD*)pbDest;
    GDI_DrawBitmapBySW(&parm, (const DWORD **) BMPArray);
}

//  *********************************************************************
//  Function    :   GDI_DrawBitmap
//  Description :   This function copies the bmp to the specified location
//                       *** the bmp source is put in a continuous dram address space
//  Arguments   :   pParm points to the PARM_DRAW_BITMAP
//  Return      :   none
//  Side Effect :
//  Note        :
//  *********************************************************************
//void	GDI_DrawBitmap(PARM_DRAW_BITMAP *pParm)
// Brian, support GDI library
void	GDI_DrawBitmap(PARM_DRAW_BITMAP *pParm, DWORD ** BMPAryptr)
{
#ifndef NO_GDI

    DWORD dwSrcAddr, dwDstAddr;
    WORD wOp_width, wOp_height;
    WORD wSrcAg_width, wSrcAg_offset;
    WORD wDstAg_width, wDstAg_offset;
    BYTE bHVInc;

    WORD wOSDSrcWidth, wOSDSrcHeight, wOSDDstWidth, wOSDDstHeight;
    DWORD dwOSDSrcBase, dwOSDDstBase;
    BYTE b4bitMode;

    BYTE bColorKeyEnable = 0;
    //DWORD ** BMPAryptr;
    DWORD   dwBMPIndex;

    if (BMPAryptr == 0)
    {
        // if call doesn't pass the BMP array point, means use the last (current) BMP array
        BMPAryptr = __pBMPAryptr;
    }

    dwBMPIndex = pParm->bBitmapID;


    // Brian2.11, copy BMP directly from BMPArray
    dwOSDSrcBase = (DWORD)(BMPAryptr[dwBMPIndex]+4);


    /*
    dwOSDSrcBase = GDI_BMP_BASE_ADDRESS;

    for (dwSrcAddr=0; dwSrcAddr<dwBMPIndex; dwSrcAddr++)
    {
        //dwOSDSrcBase += (_aGDIBMPArray[dwSrcAddr])[0]<<2; // get the size of BMP
        dwOSDSrcBase += (BMPAryptr[dwSrcAddr])[0]<<2; // get the size of BMP

    }
    */


    // Brian2.11, use dwBMPIndex
    wOSDSrcWidth = (BMPAryptr[dwBMPIndex])[2];   // get the width of BMP
    wOSDSrcHeight = (BMPAryptr[dwBMPIndex])[3];  // get the height of BMP

    b4bitMode = __RegionList[pParm->bDestRegion].bColorMode;

    wOSDDstWidth = __RegionList[pParm->bDestRegion].wWidth;
    wOSDDstHeight = __RegionList[pParm->bDestRegion].wHeight;
    dwOSDDstBase = __RegionList[pParm->bDestRegion].dwTAddr;

    //if ((pParm->SrcRect.wLeft > wOSDSrcWidth) || (pParm->wDestX > wOSDDstWidth))
    //{
    //    return ;
    //}

#ifndef GDI_WITHOUT_OS
    OS_LockMutex(&__mutexGDIIdle);
#endif

    _gdi_ResetVPU();

   // if (pParm->SrcRect.wRight >= wOSDSrcWidth)
   // {
   //     pParm->SrcRect.wRight = wOSDSrcWidth-1;
   // }

    dwSrcAddr = dwOSDSrcBase; // + ((pParm->SrcRect.wTop*wOSDSrcWidth + pParm->SrcRect.wLeft)>>b4bitMode);
    dwDstAddr = dwOSDDstBase + ((pParm->wDestY*wOSDDstWidth + pParm->wDestX)>>b4bitMode);

#ifdef SUPPORT_CT909S

    wOp_width = (wOSDSrcWidth & 0x3ff)>>b4bitMode;   // use 10 bit
    wOp_height = (wOSDSrcHeight & 0x3ff);   // use 10 bit

    wSrcAg_width = (wOp_width + (dwSrcAddr & 0x3) + 3) >> 2;
    wSrcAg_offset = (((wOSDSrcWidth>>b4bitMode) + 3)>>2) - wSrcAg_width + 1;

    wDstAg_width = (wOp_width + (dwDstAddr & 0x3) + 3) >> 2;
    wDstAg_offset = (((wOSDDstWidth>>b4bitMode) + 3)>>2) - wDstAg_width + 1;

#else
    wOp_width = (wOSDSrcWidth & 0xfff)>>b4bitMode;   // use 12 bit
    wOp_height = (wOSDSrcHeight & 0x7ff);   // use 11 bit

    wSrcAg_width = (wOp_width + (dwSrcAddr & 0x7) + 7) >> 3;
    wSrcAg_offset = (((wOSDSrcWidth>>b4bitMode) + 7)>>3) - wSrcAg_width + 1;

    wDstAg_width = (wOp_width + (dwDstAddr & 0x7) + 7) >> 3;
    wDstAg_offset = (((wOSDDstWidth>>b4bitMode) + 7)>>3) - wDstAg_width + 1;

#endif


    // Just use case 1's H V copy direction
    // Case 1
    bHVInc = (GPU_SRC_INCH | GPU_SRC_INCV | GPU_DST_INCH | GPU_DST_INCV);


    REG_GPU_OP_SIZE = (wOp_height << 16) + wOp_width;

#ifdef SUPPORT_CT909S
    REG_GPU_AG_OFF = (wDstAg_offset << 16) + (wSrcAg_offset);
#else
    REG_GPU_AG_OFF = (((wDstAg_width << 8) + wDstAg_offset) << 16) + ((wSrcAg_width << 8) + wSrcAg_offset);

#endif

    REG_GPU_SRC_ADDR = dwSrcAddr;
    REG_GPU_DEST_ADDR = dwDstAddr;
    //REG_GPU_CTL1 = __bTransparentColor << 16; // Color Key is not enabled in this function

    asm volatile ( "" : : : "memory" );  //HAL_REORDER_BARRIER();

#ifdef SUPPORT_CT909S

    REG_GPU_CTL0 = GPU_INT_EN | GPU_OP | (4 << 16)  | (4 << 20)  | (bHVInc << 12) |
        (bColorKeyEnable << 8) | (GPU_BMPCOPY << 2) | GPU_START;
#else
    REG_GPU_CTL0 = GPU_INT_EN | GPU_REQ_EN | GPU_W_REL_EN | (0x10 << 16) | (bHVInc << 12) |
        (bColorKeyEnable << 8) | (GPU_BMPCOPY << 2) | GPU_START;
#endif

    //while (REG_GPU_CTL0 & GPU_STATUS) { }

    _gdi_CheckCmdComplete();

#ifndef GDI_WITHOUT_OS
    OS_UnlockMutex(&__mutexGDIIdle);
#endif


#endif // #ifndef NO_GDI

}


//  *********************************************************************
//  Function    :   GDI_BitmapCopy
//  Description :   This function copies the content of the region from one place to another place
//                       the region width & height of Source and Destination must be the same
//  Arguments   : pParm points to the PARM_BITMAP_COPY
//  Return      :   none
//  Side Effect :
//  Note        :  This function doesn't handle horizontal shift copy
//  *********************************************************************

void GDI_BitmapCopy(PARM_BITMAP_COPY *pParm)
{

#ifndef NO_GDI


    DWORD dwSrcAddr, dwDstAddr;
    WORD wOp_width, wOp_height;
    WORD wSrcAg_width, wSrcAg_offset;
    WORD wDstAg_width, wDstAg_offset;
    BYTE bHVInc;

    WORD wOSDSrcWidth, wOSDSrcHeight, wOSDDstWidth, wOSDDstHeight;
    DWORD dwOSDSrcBase, dwOSDDstBase;
    BYTE b4bitMode;

    BYTE bColorKeyEnable = 0;

    wOSDSrcWidth = __RegionList[pParm->bSrcRegion].wWidth;
    wOSDSrcHeight = __RegionList[pParm->bSrcRegion].wHeight;
    dwOSDSrcBase = __RegionList[pParm->bSrcRegion].dwTAddr;
    b4bitMode = __RegionList[pParm->bSrcRegion].bColorMode;

    wOSDDstWidth = __RegionList[pParm->bDestRegion].wWidth;
    wOSDDstHeight = __RegionList[pParm->bDestRegion].wHeight;
    dwOSDDstBase = __RegionList[pParm->bDestRegion].dwTAddr;

    if ((pParm->SrcRect.wLeft > wOSDSrcWidth) || (pParm->wDestX > wOSDDstWidth))
    {
        return ;
    }

#ifndef GDI_WITHOUT_OS
    OS_LockMutex(&__mutexGDIIdle);
#endif

     _gdi_ResetVPU();

    if (pParm->SrcRect.wRight >= wOSDSrcWidth)
    {
        pParm->SrcRect.wRight = wOSDSrcWidth-1;
    }

    dwSrcAddr = dwOSDSrcBase + ((pParm->SrcRect.wTop*wOSDSrcWidth + pParm->SrcRect.wLeft)>>b4bitMode);
    dwDstAddr = dwOSDDstBase + ((pParm->wDestY*wOSDDstWidth + pParm->wDestX)>>b4bitMode);

#ifdef SUPPORT_CT909S
    wOp_width = ((pParm->SrcRect.wRight - pParm->SrcRect.wLeft + 1) & 0x3ff)>>b4bitMode;   // use 10 bit
    wOp_height = ((pParm->SrcRect.wBottom - pParm->SrcRect.wTop + 1) &0x3ff);   // use 10 bit

    wSrcAg_width = (wOp_width + (dwSrcAddr & 0x3) + 3) >> 2;
    wSrcAg_offset = (((wOSDSrcWidth>>b4bitMode) + 3)>>2) - wSrcAg_width + 1;

    wDstAg_width = (wOp_width + (dwDstAddr & 0x3) + 3) >> 2;
    wDstAg_offset = (((wOSDDstWidth>>b4bitMode) + 3)>>2) - wDstAg_width + 1;

#else
    wOp_width = ((pParm->SrcRect.wRight - pParm->SrcRect.wLeft + 1) & 0xfff)>>b4bitMode;   // use 12 bit
    wOp_height = ((pParm->SrcRect.wBottom - pParm->SrcRect.wTop + 1) &0x7ff);   // use 11 bit

    wSrcAg_width = (wOp_width + (dwSrcAddr & 0x7) + 7) >> 3;
    wSrcAg_offset = (((wOSDSrcWidth>>b4bitMode) + 7)>>3) - wSrcAg_width + 1;

    wDstAg_width = (wOp_width + (dwDstAddr & 0x7) + 7) >> 3;
    wDstAg_offset = (((wOSDDstWidth>>b4bitMode) + 7)>>3) - wDstAg_width + 1;

#endif

    if ((pParm->SrcRect.wTop == pParm->wDestY) && (pParm->SrcRect.wLeft < pParm->wDestX) && (pParm->SrcRect.wRight > pParm->wDestX))
    {
        //printf("\n HW Limitation: Case 2/3");
        //printf("\nCase 2..");
        bHVInc = (GPU_SRC_DECH | GPU_SRC_INCV | GPU_DST_DECH | GPU_DST_INCV);
        dwSrcAddr += wOp_width -1;
        dwDstAddr += wOp_width -1;

        //wSrcAg_offset = (((__wOSDFrame_HDS>>_bOsd_4b_mode) + 7)>>3) + wSrcAg_width - 1;
        //wDstAg_offset = (((__wOSDFrame_HDS>>_bOsd_4b_mode) + 7)>>3) + wDstAg_width - 1;
        wSrcAg_offset += (wSrcAg_width - 1)<<1;
        wDstAg_offset += (wDstAg_width - 1)<<1;

        /*
        // Case 3
        printf("\nCase 3..");
        bHVInc = (GPU_SRC_DECH | GPU_SRC_DECV | GPU_DST_DECH | GPU_DST_DECV);
        dwSrcAddr += ((wOp_height - 1)*(__wOSDFrame_HDS>>_bOsd_4b_mode) + wOp_width - 1);
        dwDstAddr += ((wOp_height - 1)*(__wOSDFrame_HDS>>_bOsd_4b_mode) + wOp_width - 1);
        */
    }
    else if (pParm->SrcRect.wTop < pParm->wDestY)
    {
        // Case 4
        //printf("\nCase 4..");
        bHVInc = (GPU_SRC_INCH | GPU_SRC_DECV | GPU_DST_INCH | GPU_DST_DECV);
        dwSrcAddr += ((wOp_height - 1)*wOSDSrcWidth>>b4bitMode);
        dwDstAddr += ((wOp_height - 1)*wOSDDstWidth>>b4bitMode);
        //wSrcAg_offset = (((__wOSDFrame_HDS>>_bOsd_4b_mode) + 7)>>3) + wSrcAg_width - 1;
        //wDstAg_offset = (((__wOSDFrame_HDS>>_bOsd_4b_mode) + 7)>>3) + wDstAg_width - 1;
        wSrcAg_offset += (wSrcAg_width - 1)<<1;
        wDstAg_offset += (wDstAg_width - 1)<<1;
    }
    else
    {
        // Case 1
        //printf("\nCase 1..");
        bHVInc = (GPU_SRC_INCH | GPU_SRC_INCV | GPU_DST_INCH | GPU_DST_INCV);
    }

    REG_GPU_OP_SIZE = (wOp_height << 16) + wOp_width;

#ifdef SUPPORT_CT909S
    REG_GPU_AG_OFF = (wDstAg_offset << 16) + (wSrcAg_offset);
#else
    REG_GPU_AG_OFF = (((wDstAg_width << 8) + wDstAg_offset) << 16) + ((wSrcAg_width << 8) + wSrcAg_offset);
#endif

    REG_GPU_SRC_ADDR = dwSrcAddr;
    REG_GPU_DEST_ADDR = dwDstAddr;
    //REG_GPU_CTL1 = __bTransparentColor << 16; // Color Key is not enabled in this function

    asm volatile ( "" : : : "memory" );  //HAL_REORDER_BARRIER();

#ifdef SUPPORT_CT909S
    REG_GPU_CTL0 = GPU_INT_EN | GPU_OP | (4 << 16)  | (4 << 20)  | (bHVInc << 12) |
        (bColorKeyEnable << 8) | (GPU_BMPCOPY << 2) | GPU_START;
#else
    REG_GPU_CTL0 = GPU_INT_EN | GPU_REQ_EN | GPU_W_REL_EN | (0x10 << 16) | (bHVInc << 12) |
        (bColorKeyEnable << 8) | (GPU_BMPCOPY << 2) | GPU_START;
#endif

    //while (REG_GPU_CTL0 & GPU_STATUS) { }

    _gdi_CheckCmdComplete();

#ifndef GDI_WITHOUT_OS
    OS_UnlockMutex(&__mutexGDIIdle);
#endif

    #endif  //#ifndef NO_GDI

}

//  *********************************************************************
//  Function    :   GDI_DrawFrame
//  Description :   This function draws a frame
//  Arguments   :   pParm points to the PARM_FRAME
//  Return      :   none
//  Side Effect :
//  *********************************************************************
void GDI_DrawFrame(BYTE bRegionId, PARM_FRAME *pParm)
{

#ifndef NO_GDI
    BYTE bTopLeftColor, bBottomRightColor;

    if (pParm->bStyle & 0x02)
    { // solid style
        if (pParm->bStyle & 0x04)
        { // double frame
            _GDIRect.rect.wTop = pParm->rect.wTop + (pParm->bThickness << 1);
            _GDIRect.rect.wLeft = pParm->rect.wLeft + (pParm->bThickness << 1);
            _GDIRect.rect.wBottom = pParm->rect.wBottom - (pParm->bThickness << 1);
            _GDIRect.rect.wRight = pParm->rect.wRight - (pParm->bThickness << 1);

        }
        else
        { // single frame
            _GDIRect.rect.wTop = pParm->rect.wTop + pParm->bThickness;
            _GDIRect.rect.wLeft = pParm->rect.wLeft + pParm->bThickness;
            _GDIRect.rect.wBottom = pParm->rect.wBottom - pParm->bThickness;
            _GDIRect.rect.wRight = pParm->rect.wRight - pParm->bThickness;
        }
        _GDIRect.bColor = pParm->bColorCenter;

        GDI_FillRect(bRegionId, &_GDIRect); // Fill the center block

    }

    if (pParm->bStyle & 0x01)
    { // push style
        bTopLeftColor = pParm->bColorDark;
        bBottomRightColor = pParm->bColorBright;
    }
    else
    { // pop style
        bTopLeftColor = pParm->bColorBright;
        bBottomRightColor = pParm->bColorDark;
    }

    _GDIRect.rect.wTop = pParm->rect.wTop;
    _GDIRect.rect.wLeft = pParm->rect.wLeft;
    _GDIRect.rect.wBottom = pParm->rect.wTop + pParm->bThickness - 1;
    _GDIRect.rect.wRight = pParm->rect.wRight;
    _GDIRect.bColor = bTopLeftColor;
    GDI_FillRect(bRegionId, &_GDIRect); // Fill the horizontal top block

    //_GDIRect.rect.wTop = pParm->rect.wTop; //--Brian1.15, reduce code size
    //_GDIRect.rect.wLeft = pParm->rect.wLeft; //--Brian1.15, reduce code size
    if (pParm->bStyle & 0x04) // Brian1.07
    {
        // Double frame
        _GDIRect.rect.wBottom = pParm->rect.wBottom - pParm->bThickness;
    }
    else
    {
        _GDIRect.rect.wBottom = pParm->rect.wBottom;
    }

    _GDIRect.rect.wRight = pParm->rect.wLeft + pParm->bThickness - 1;
    //_GDIRect.bColor = bTopLeftColor; //--Brian1.15, reduce code size
    GDI_FillRect(bRegionId, &_GDIRect); // Fill the vertical left block

    _GDIRect.rect.wTop = pParm->rect.wBottom - pParm->bThickness + 1;
    _GDIRect.rect.wLeft = pParm->rect.wLeft; // + pParm->bThickness;
    _GDIRect.rect.wBottom = pParm->rect.wBottom;
    _GDIRect.rect.wRight = pParm->rect.wRight;
    _GDIRect.bColor = bBottomRightColor;
    GDI_FillRect(bRegionId, &_GDIRect); // Fill the horizontal bottom block

    _GDIRect.rect.wTop = pParm->rect.wTop;
    _GDIRect.rect.wLeft = pParm->rect.wRight - pParm->bThickness + 1;
    //_GDIRect.rect.wBottom = pParm->rect.wBottom; //--Brian1.15, reduce code size
    //_GDIRect.rect.wRight = pParm->rect.wRight; //--Brian1.15, reduce code size
    //_GDIRect.bColor = bBottomRightColor; //--Brian1.15, reduce code size
    GDI_FillRect(bRegionId, &_GDIRect); // Fill the vertical right block


    if (pParm->bStyle & 0x04)
    {
        // Double frame
        _GDIRect.rect.wTop = pParm->rect.wTop + pParm->bThickness;
        _GDIRect.rect.wLeft = pParm->rect.wLeft + pParm->bThickness;
        _GDIRect.rect.wBottom = pParm->rect.wTop + (pParm->bThickness << 1) - 1;
        _GDIRect.rect.wRight = pParm->rect.wRight - pParm->bThickness;
        _GDIRect.bColor = pParm->bColorDark;
        GDI_FillRect(bRegionId, &_GDIRect); // Fill the horizontal top block

        _GDIRect.rect.wTop = pParm->rect.wTop + (pParm->bThickness << 1);
        //_GDIRect.rect.wLeft = pParm->rect.wLeft + pParm->bThickness; //--Brian1.15, reduce code size
        _GDIRect.rect.wBottom = pParm->rect.wBottom - pParm->bThickness;
        _GDIRect.rect.wRight = pParm->rect.wLeft + (pParm->bThickness << 1) - 1;
        //_GDIRect.bColor = pParm->bColorDark; //--Brian1.15, reduce code size
        GDI_FillRect(bRegionId, &_GDIRect); // Fill the vertical left block

        _GDIRect.rect.wTop = pParm->rect.wBottom - (pParm->bThickness << 1) + 1;
        _GDIRect.rect.wLeft = pParm->rect.wLeft; // + (pParm->bThickness << 1);  // Brian1.07, don't look like "line break"
        _GDIRect.rect.wBottom = pParm->rect.wBottom - pParm->bThickness;
        _GDIRect.rect.wRight = pParm->rect.wRight - (pParm->bThickness << 1);
        _GDIRect.bColor = pParm->bColorBright;
        GDI_FillRect(bRegionId, &_GDIRect); // Fill the horizontal bottom block

        _GDIRect.rect.wTop = pParm->rect.wTop + pParm->bThickness;
        _GDIRect.rect.wLeft = pParm->rect.wRight - (pParm->bThickness << 1) + 1;
        //_GDIRect.rect.wBottom = pParm->rect.wBottom - pParm->bThickness; //--Brian1.15, reduce code size
        _GDIRect.rect.wRight = pParm->rect.wRight - pParm->bThickness;
        //_GDIRect.bColor = pParm->bColorBright; //--Brian1.15, reduce code size
        GDI_FillRect(bRegionId, &_GDIRect); // Fill the vertical right block
    }

#endif // #ifndef NO_GDI

}

//  *********************************************************************
//  Function    :   GDI_DrawVolumeBar
//  Description :   This function draws the volume bar
//  Arguments   :   pParm points to the PARM_VOLUME_BAR
//  Return      :   none
//  Side Effect :
//  *********************************************************************
void GDI_DrawVolumeBar(BYTE bRegionId, PARM_VOLUME_BAR *pParm)
{

#ifndef NO_GDI

#define VOLUME_FRAME_THICKNESS  2

#ifdef GDI_4_BIT_OSD
#define VOLUME_FRAME_COLOR      GDI_ENTRY_4B_COLOR_TEXT_DISABLED //GDI_ENTRY_4B_COLOR_WHITE
#else
//#define VOLUME_FRAME_COLOR      GDI_ENTRY_SCROLL_BAR_COLOR_BRIGHT
#endif

//#define VOLUME_BAR_COLOR        3
//#define VOLUME_BAR_BG_COLOR     0

    _GDIRect.rect.wTop = pParm->rect.wTop;
    _GDIRect.rect.wLeft = pParm->rect.wLeft;
    _GDIRect.rect.wBottom = pParm->rect.wTop + VOLUME_FRAME_THICKNESS - 1;
    _GDIRect.rect.wRight = pParm->rect.wRight;
    _GDIRect.bColor = pParm->bFrameColor;
    GDI_FillRect(bRegionId, &_GDIRect); // Fill the horizontal top block

    _GDIRect.rect.wBottom = pParm->rect.wBottom;
    _GDIRect.rect.wRight = pParm->rect.wLeft + VOLUME_FRAME_THICKNESS - 1;
    GDI_FillRect(bRegionId, &_GDIRect); // Fill the vertical left block

    _GDIRect.rect.wTop = pParm->rect.wBottom - VOLUME_FRAME_THICKNESS + 1;
    _GDIRect.rect.wLeft = pParm->rect.wLeft + VOLUME_FRAME_THICKNESS;
    _GDIRect.rect.wRight = pParm->rect.wRight;
    GDI_FillRect(bRegionId, &_GDIRect); // Fill the horizontal bottom block

    _GDIRect.rect.wTop = pParm->rect.wTop;
    _GDIRect.rect.wLeft = pParm->rect.wRight - VOLUME_FRAME_THICKNESS + 1;
    _GDIRect.rect.wBottom = pParm->rect.wBottom;
    _GDIRect.rect.wRight = pParm->rect.wRight;
    GDI_FillRect(bRegionId, &_GDIRect); // Fill the vertical right block

    if (pParm->bOriention == ORIENTION_HORIZONTAL)
    {
        // Brian0.86
        _dwGDITemp = (DWORD)(pParm->rect.wRight - pParm->rect.wLeft - 3 ) * pParm->wVolume /100;
        _GDIRect.rect.wTop = pParm->rect.wTop + VOLUME_FRAME_THICKNESS;
        _GDIRect.rect.wLeft = pParm->rect.wLeft + VOLUME_FRAME_THICKNESS;
        _GDIRect.rect.wBottom = pParm->rect.wBottom - VOLUME_FRAME_THICKNESS;
        _GDIRect.rect.wRight = _GDIRect.rect.wLeft + (WORD)_dwGDITemp - 1;
        _GDIRect.bColor = pParm->bBarColor;
        GDI_FillRect(bRegionId, &_GDIRect); // Fill the volume bar

/*Alan2.31a, bug #2030.
        _dwGDITemp = (DWORD)(pParm->rect.wRight - pParm->rect.wLeft +1 ) * pParm->wVolume /100;
        _GDIRect.rect.wLeft = pParm->rect.wLeft + VOLUME_FRAME_THICKNESS + (WORD)_dwGDITemp;
*/
        _GDIRect.rect.wLeft = _GDIRect.rect.wRight + 1;
        _GDIRect.rect.wRight = pParm->rect.wRight - VOLUME_FRAME_THICKNESS;
        _GDIRect.bColor = pParm->bBgColor;
        GDI_FillRect(bRegionId, &_GDIRect); // Fill the volume background
    }
    else
    {
        _dwGDITemp = (DWORD)(pParm->rect.wBottom - pParm->rect.wTop -3 ) * pParm->wVolume /100;
        _GDIRect.rect.wLeft = pParm->rect.wLeft + VOLUME_FRAME_THICKNESS;
        _GDIRect.rect.wBottom = pParm->rect.wBottom - VOLUME_FRAME_THICKNESS;
        _GDIRect.rect.wRight = pParm->rect.wRight - VOLUME_FRAME_THICKNESS;
        _GDIRect.rect.wTop = _GDIRect.rect.wBottom - (WORD)_dwGDITemp + 1;

        _GDIRect.bColor = pParm->bBarColor;
        GDI_FillRect(bRegionId, &_GDIRect); // Fill the volume bar

/*Alan2.31a, bug #2030.
        _dwGDITemp = (DWORD)(pParm->rect.wBottom - pParm->rect.wTop +1 ) * pParm->wVolume /100;
        _GDIRect.rect.wBottom = pParm->rect.wBottom - VOLUME_FRAME_THICKNESS - (WORD)_dwGDITemp;
*/
        _GDIRect.rect.wBottom = _GDIRect.rect.wTop - 1;
        _GDIRect.rect.wTop = pParm->rect.wTop + VOLUME_FRAME_THICKNESS;
        _GDIRect.bColor = pParm->bBgColor;
        GDI_FillRect(bRegionId, &_GDIRect); // Fill the volume background
    }

#endif //#ifndef NO_GDI

}


//  *********************************************************************
//  Function    :   GDI_DrawScrollBar
//  Description :   This function draws the scroll bar
//  Arguments   :   pParm points to the PARM_SCROLL_BAR
//  Return      :   none
//  ***Side Effect :   none (since DVD_251)
//  *********************************************************************
void GDI_DrawScrollBar(BYTE bRegionId, PARM_SCROLL_BAR *pParm)
{

    PARM_FRAME darwFrame; // Brian2.51, use internal instead of casting the pParm to remove the side effect.

#ifndef NO_GDI

#ifdef GDI_4_BIT_OSD
#define SCROLL_FRAME_THICKNESS  2
#else
#define SCROLL_FRAME_THICKNESS  1 //2
#endif


// define this to draw the scroll bar in the off screen region first,
// then copy back to current region
//LLY2.82#define OFFSCREEN_SCROLL_BAR

#ifdef OFFSCREEN_SCROLL_BAR
    if(__bCurrentRegionId == 0)
        GDI_SetCurrentRegion(1);
    else
        GDI_SetCurrentRegion(0);

    //_GDIParm0.u.BitmapCopy.SrcRect.wTop = pParm->rect.wTop;
    //_GDIParm0.u.BitmapCopy.SrcRect.wLeft = pParm->rect.wLeft;
    //_GDIParm0.u.BitmapCopy.SrcRect.wBottom = pParm->rect.wBottom;
    //_GDIParm0.u.BitmapCopy.SrcRect.wRight = pParm->rect.wRight;

    SET_RECT(_GDIParm0.u.BitmapCopy.SrcRect,
             pParm->rect.wLeft,  pParm->rect.wTop,
             pParm->rect.wRight, pParm->rect.wBottom);

    _GDIParm0.u.BitmapCopy.wDestX = pParm->rect.wLeft;
    _GDIParm0.u.BitmapCopy.wDestY = pParm->rect.wTop;

#endif

#ifdef GDI_4_BIT_OSD
    _GDIRect.bColor = GDI_ENTRY_4B_COLOR_BAR_BG;
#else
    GDI_ChangePALEntry(GDI_ENTRY_SCROLL_BAR_COLOR_BRIGHT, GDI_VALUE_SCROLL_BAR_COLOR_BRIGHT, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_SCROLL_BAR_COLOR_DARK, GDI_VALUE_SCROLL_BAR_COLOR_DARK, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_SCROLL_BAR_COLOR_CENTER, GDI_VALUE_SCROLL_BAR_COLOR_CENTER, FALSE);
    GDI_ChangePALEntry(GDI_ENTRY_SCROLL_BAR_COLOR_BG, GDI_VALUE_SCROLL_BAR_COLOR_BG, FALSE);

    _GDIRect.bColor = GDI_ENTRY_SCROLL_BAR_COLOR_BG;
#endif


    //_GDIRect.rect.wTop = pParm->rect.wTop;
    //_GDIRect.rect.wLeft = pParm->rect.wLeft;
    //_GDIRect.rect.wBottom = pParm->rect.wBottom;
    //_GDIRect.rect.wRight = pParm->rect.wRight;
    SET_RECT(_GDIRect.rect,
             pParm->rect.wLeft,  pParm->rect.wTop,
             pParm->rect.wRight, pParm->rect.wBottom);
    GDI_FillRect(bRegionId, &_GDIRect); // Fill the background bar

    _GDIRect.rect.wTop += pParm->wStartPoint;
    _GDIRect.rect.wBottom = _GDIRect.rect.wTop + pParm->wDisplay - 1;

    //use pParm as PARM_FRAME *
#ifdef GDI_4_BIT_OSD
    darwFrame.bColorBright = GDI_ENTRY_4B_COLOR_BRIGHT;
    darwFrame.bColorDark = GDI_ENTRY_4B_COLOR_DARK;
    darwFrame.bColorCenter = GDI_ENTRY_4B_COLOR_CENTER;

#else
    darwFrame.bColorBright = GDI_ENTRY_SCROLL_BAR_COLOR_BRIGHT;
    darwFrame.bColorDark = GDI_ENTRY_SCROLL_BAR_COLOR_DARK;
    darwFrame.bColorCenter = GDI_ENTRY_SCROLL_BAR_COLOR_CENTER;


#endif
    darwFrame.bStyle = FRAME_POP_SOLID;
    darwFrame.bThickness = SCROLL_FRAME_THICKNESS;
    //darwFrame.rect.wTop = _GDIRect.rect.wTop;
    //darwFrame.rect.wLeft = _GDIRect.rect.wLeft;
    //darwFrame.rect.wBottom = _GDIRect.rect.wBottom;
    //darwFrame.rect.wRight = _GDIRect.rect.wRight;
    SET_RECT(darwFrame.rect,
             _GDIRect.rect.wLeft,  _GDIRect.rect.wTop,
             _GDIRect.rect.wRight, _GDIRect.rect.wBottom);

    GDI_DrawFrame(bRegionId, &darwFrame); // Draw the scroll part

#ifdef OFFSCREEN_SCROLL_BAR

    if(__bCurrentRegionId == 0)
    {
        _GDIParm0.u.BitmapCopy.bSrcRegion = 0;
        _GDIParm0.u.BitmapCopy.bDestRegion = 1;
        GDI_BitmapCopy ((PARM_BITMAP_COPY *)&_GDIParm0);
        GDI_SetCurrentRegion(1);
    }
    else
    {
        _GDIParm0.u.BitmapCopy.bSrcRegion = 1;
        _GDIParm0.u.BitmapCopy.bDestRegion = 0;
        GDI_BitmapCopy ((PARM_BITMAP_COPY *)&_GDIParm0);
        GDI_SetCurrentRegion(0);
    }
#endif

#endif // #ifndef NO_GDI

}

//Aron,2.77-909P, Port Damon's code to files to support Unicode DIR/FILE name & external subtitle.
#ifdef SUPPORT_UNICODE_CODING
//  *********************************************************************
//  Function    :   GDI_DrawStringByUnicode
//  Description :   This function output the text
//  Arguments   :   none
//  Global Var  :   __wGDIString[]: the array stores the char index
//                  __wGDIx, __wGDIy: the start oint(x, y)
//  Return      :   none  
//  Side Effect :
//  *********************************************************************
void GDI_DrawStringByUnicode(BYTE bRegionId, PARM_DRAW_STRING *pParm, WORD *pwString)
{
#ifndef NO_GDI
    DWORD dwBackupAddr, dwNewAddress, dwLength, dwStart, i;
    WORD theString[GDI_STRING_LENGTH+1];
    WORD wStringInfo[GDI_STRING_LENGTH*3]; // Save string's height & width info.
    WORD wCharSize, wCharIndex;
    PARM_DRAW_STRING parm;
    BYTE b2BitFont = FALSE;

    dwBackupAddr = __dwGDIFontTableBase; 
    wCharSize = 1 * 32 * _bFontHeightInTable / 8;
    GDI_SetFontTableSize(1, wCharSize / 4);
    wStringInfo[0] = pwString[0];

    //Alan1.00, Disable the font table protection
    MACRO_MCU_DISABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);
   
#if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY) 
    // Alan2.00, copy the font from flash to font table in DRAM

    for (i = 1; i <= *pwString; i++)
    {
        wCharIndex = CONVCHAR_Uni_To_IDX(pwString[i]);
        wStringInfo[i] = GDI_GetUniFontHeightWidth(pwString[i]);
       memcpy((BYTE *)(OSD_FONT_TABLE_BASE_ADDRESS + (wCharSize * (i - 1))),
            __pCurFontTable + (wCharSize * wCharIndex), wCharSize); 
    }
#elif (FONT_TABLE_METHOD == SPLIT_2_1_BIT) // Alan2.00, split to 2 and 1 bit font table.

   // 1-bit font index follows the 2-bit font index 
   // Font index range : 0                      OSD_2BIT_FONT_NUM             Total font number
   //                    |->    2 bit font index       <-|->     1 bit font index  <-|

    dwNewAddress = OSD_FONT_TABLE_BASE_ADDRESS + OSD_2BIT_FONT_NUM * OSD_2BIT_CHAR_SIZE +
                   OSD_1BIT_FONT_NUM * OSD_1BIT_CHAR_SIZE;

    GDI_SetFontTableAddress(dwNewAddress);

    for (i = 1; i <= *pwString; i++)
    {
        wCharIndex = CONVCHAR_Uni_To_IDX(pwString[i]);         
        wStringInfo[i] = GDI_GetUniFontHeightWidth(pwString[i]);      		
        memcpy((BYTE *)(dwNewAddress + (wCharSize * (i - 1))),
            __pCurFontTable + (wCharSize * wCharIndex), wCharSize); 
        }
#else // FONT_TABLE_METHOD = TRADITION   

    dwNewAddress = OSD_FONT_TABLE_BASE_ADDRESS + OSD_2BIT_FONT_NUM * OSD_2BIT_CHAR_SIZE;

    GDI_SetFontTableAddress(dwNewAddress);    

    for (i = 1; i <= *pwString; i++)
        {
        wCharIndex = CONVCHAR_Uni_To_IDX(pwString[i]);	 	
        wStringInfo[i] = GDI_GetUniFontHeightWidth(pwString[i]);
       memcpy((BYTE *)(dwNewAddress + (wCharSize * (i - 1))),
              __pCurFontTable + (wCharSize * wCharIndex), wCharSize); 
    }
#endif

    parm = *pParm;
    dwLength = wStringInfo[0];
    dwStart = 1;
    while (dwLength)
    {
        if (dwLength > GDI_STRING_LENGTH)
        {
            for (i=dwStart; i < (dwStart+GDI_STRING_LENGTH); i++)
            {
                theString[i-dwStart+1] = wStringInfo[i] | (i - 1); 
            }
            theString[0] = GDI_STRING_LENGTH;
            dwLength -= GDI_STRING_LENGTH;
            dwStart += GDI_STRING_LENGTH;
        }
        else
        {
            for (i=dwStart; i < (dwStart+dwLength); i++)
            {
                theString[i-dwStart+1] = wStringInfo[i] | (i - 1);
            }
            theString[0] = dwLength;
            dwLength = 0;
        }

   //Alan1.00, Enable the font table protection
   MACRO_MCU_ENABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);

        _gdi_DrawString(bRegionId, &parm, theString, b2BitFont, 1);

#ifdef SUPPORT_CT909S
        if (__RegionList[bRegionId].bColorMode == GDI_OSD_2B_MODE)
        {
            parm.wX += GDI_GetStringWidth_2B(theString);
        }
        else
#endif
        {
            parm.wX += GDI_GetStringWidth(theString);
        }
    }
    GDI_SetFontTableAddress(dwBackupAddr);
#endif //#ifndef NO_GDI
}
#endif


//  *********************************************************************
//  Function    :   GDI_DrawString
//  Description :   This function output the text
//  Arguments   :   none
//  Global Var  :   __wGDIString[]: the array stores the char index
//                  __wGDIx, __wGDIy: the start oint(x, y)
//  Return      :   none
//  Side Effect :
//  *********************************************************************
void GDI_DrawString(BYTE bRegionId, PARM_DRAW_STRING *pParm, WORD *pwString)

{

#ifndef NO_GDI

    WORD theString[GDI_STRING_LENGTH+1];
    DWORD dwLength, dwStart, i;
    PARM_DRAW_STRING parm;
    BYTE             b2BitFont = TRUE;

#if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY) // Alan2.00, copy the font from flash to font table in DRAM
   extern const BYTE _bFontable_OSD_2bit[];
    //Alan1.00, Disable the font table protection
    MACRO_MCU_DISABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);

    for (i = 1; i <= *pwString; i++)
    {
        if ((pwString[i] & 0x01FF) < OSD_2BIT_FONT_NUM)
        {
            b2BitFont = TRUE;
            GDI_SetFontTableSize(2, 2 * 32 * _bFontHeightInTable / (8 * 4));
            //the Nth char
            memcpy((BYTE *)(OSD_FONT_TABLE_BASE_ADDRESS + (OSD_2BIT_CHAR_SIZE * (i - 1))),
                   _bFontable_OSD_2bit + (OSD_2BIT_CHAR_SIZE * (pwString[i] & 0x01FF)),
                   OSD_2BIT_CHAR_SIZE);
        }
        else if ((pwString[i] & 0x01FF) < OSD_2BIT_FONT_NUM + OSD_1BIT_FONT_NUM)
        {
            GDI_SetFontTableSize(1, 1 * 32 * _bFontHeightInTable / (8 * 4));
            b2BitFont = FALSE;
           //the Nth char
           memcpy((BYTE *)(OSD_FONT_TABLE_BASE_ADDRESS + (OSD_1BIT_CHAR_SIZE * (i - 1))),
                  _bFontable_OSD_1bit +
                  (OSD_1BIT_CHAR_SIZE * ((pwString[i] - OSD_2BIT_FONT_NUM) & 0x01FF)),
                  OSD_1BIT_CHAR_SIZE);
        }
        else
        {
            GDI_SetFontTableSize(1, 1 * 32 * _bFontHeightInTable / (8 * 4));
            b2BitFont = FALSE;
           //the Nth char
           memcpy((BYTE *)(OSD_FONT_TABLE_BASE_ADDRESS + (OSD_1BIT_CHAR_SIZE * (i - 1))),
                __pCurFontTable +
                (OSD_1BIT_CHAR_SIZE * ((pwString[i] - OSD_2BIT_FONT_NUM - OSD_1BIT_FONT_NUM) & 0x01FF)),
                OSD_1BIT_CHAR_SIZE);
        }
    }

/*
   // Alan2.31, don't protect the font table after copying action immediately.
   // Otherwise, some content in the end will not be copied.
   //Alan1.00, Enable the font table protection
   MACRO_MCU_ENABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);
*/

#elif (FONT_TABLE_METHOD == SPLIT_2_1_BIT) // Alan2.00, split to 2 and 1 bit font table.

   // 1-bit font index follows the 2-bit font index
   // Font index range : 0                      OSD_2BIT_FONT_NUM             Total font number
   //                    |->    2 bit font index       <-|->     1 bit font index  <-|

   if ((pwString[1] & 0x01FF) < OSD_2BIT_FONT_NUM)
   {
       // default set as 32x26, 2-bit font
       GDI_SetFontTableSize(2, 2 * 32 * _bFontHeightInTable / (8 * 4));
       GDI_SetFontTableAddress (OSD_FONT_TABLE_BASE_ADDRESS);
       b2BitFont = TRUE;
   }
   else
   {
       // default set as 32x26, 1-bit font
       GDI_SetFontTableSize(1, 1 * 32 * _bFontHeightInTable / (8 * 4));
       GDI_SetFontTableAddress (OSD_FONT_TABLE_BASE_ADDRESS + (OSD_2BIT_FONT_NUM * OSD_2BIT_CHAR_SIZE));
       b2BitFont = FALSE;
   }

#else //FONT_TABLE_METHOD = TRADITION
//Aron,2.77-909P, Port Damon's code to files to support Unicode DIR/FILE name & external subtitle.
#ifdef SUPPORT_UNICODE_CODING
    // Need to set again, because Unicode string use 1 bit font type.
    GDI_SetFontTableSize(2, 2 * 32 * _bFontHeightInTable / (8 * 4));
#endif

#endif

    parm = *pParm;
    dwLength = pwString[0];
    dwStart = 1;
    while (dwLength)
    {
        if (dwLength > GDI_STRING_LENGTH)
        {

            for (i=dwStart; i < (dwStart+GDI_STRING_LENGTH); i++)
            {
 // Alan2.00, set the font index as its position in font table
#if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY)
                theString[i-dwStart+1] = (pwString[i] & 0xFE00) | (i - 1);
#elif (FONT_TABLE_METHOD == SPLIT_2_1_BIT)
                if (b2BitFont)
                {
                    theString[i-dwStart+1] = pwString[i];
                }
                else
                {
                    theString[i-dwStart+1] = (pwString[i] - OSD_2BIT_FONT_NUM);
                }
#else
                theString[i-dwStart+1] = pwString[i];
#endif
            }
            theString[0] = GDI_STRING_LENGTH;
            dwLength -= GDI_STRING_LENGTH;
            dwStart += GDI_STRING_LENGTH;
        }
        else
        {
            for (i=dwStart; i < (dwStart+dwLength); i++)
            {
#if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY)
                // Alan2.00, set the font index as its position in font table
                theString[i-dwStart+1] = (pwString[i] & 0xFE00) | (i - 1);
#elif (FONT_TABLE_METHOD == SPLIT_2_1_BIT)
                if (b2BitFont)
                {
                    theString[i-dwStart+1] = pwString[i];
                }
                else
                {
                    theString[i-dwStart+1] = (pwString[i] - OSD_2BIT_FONT_NUM);
                }
#else
                theString[i-dwStart+1] = pwString[i];
#endif
            }
            theString[0] = dwLength;
            dwLength = 0;
        }

// Alan2.31, protect font table here to avoid some content in the end can't be copied
#if (FONT_TABLE_METHOD == CREATE_DYNAMICALLY)
   //Alan1.00, Enable the font table protection
   MACRO_MCU_ENABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);
#endif

        _gdi_DrawString(bRegionId, &parm, theString, b2BitFont, 1);

#ifdef SUPPORT_CT909S
        if (__RegionList[bRegionId].bColorMode == GDI_OSD_2B_MODE)
        {
            parm.wX += GDI_GetStringWidth_2B(theString);
        }
        else
#endif

        {
            parm.wX += GDI_GetStringWidth(theString);
        }


    }

#endif //#ifndef NO_GDI


}


void _gdi_DrawString(BYTE bRegionId, PARM_DRAW_STRING *pParm, WORD *pwString, BYTE b2BitFont, BYTE bMultipleH)
{
    BYTE bMode=0, bIdx;
    volatile WORD wOp_width, wOp_height;
    volatile WORD wAg_width, wAg_offset;
    DWORD dwAddress;

    volatile WORD wOSDWidth, wOSDHeight;
    BYTE b4bitMode;
    BYTE bColorKeyEnable = 0;
    TEXT_COLOR textColor;

#ifndef GDI_WITHOUT_OS
    OS_LockMutex(&__mutexGDIIdle);
#endif

    // Reset VPU
   _gdi_ResetVPU();


    // Brian0.66
    // JPEG will reset VPU ==> need to set font table address ech time

#ifdef SUPPORT_CT909S
    REG_GPU_CTL0 |= (GPU_OP | GPU_ACCESS_FONT_EN); // enable, gpu_op and LEON font access

   REG_GPU_FONT_ADDR = __dwGDIFontTableBase; // just assign the byte address, but need to be 4-byte alignment

#else
   REG_GPU_FONT_ADDR = __dwGDIFontTableBase>>3;

#endif

    textColor.bShadowColor_1 = pParm->bShadow1;
    textColor.bShadowColor_2 = pParm->bShadow2;
    textColor.bTextBgColor = pParm->bBackground;
    textColor.bTextFgColor = pParm->bTextColor;
    REG_GPU_COL_NDX = *(DWORD*)&textColor;

    wOSDWidth = __RegionList[bRegionId].wWidth;
    wOSDHeight = __RegionList[bRegionId].wHeight;
    b4bitMode = __RegionList[bRegionId].bColorMode;

    if (pParm->bColorKey)
    {
        bColorKeyEnable = 1;
    }


#ifdef SUPPORT_CT909S
    if (b4bitMode == GDI_OSD_2B_MODE)
    {
        wOSDWidth = ((wOSDWidth+15)>>4)*16;
    }
#endif


    dwAddress = __RegionList[bRegionId].dwTAddr;

    // Fill Font Index
    for (bIdx = 1; bIdx <= pwString[0]; bIdx++)
    {
        REG_GPU_FONT_RAM_INDEX = pwString[bIdx];
    }

    dwAddress += ((pParm->wY*wOSDWidth +pParm->wX)>>b4bitMode);

    if (b4bitMode == GDI_OSD_4B_MODE)
    {
        bMode = GPU_4B_FONT_MODE;
        wOp_width = REG_GPU_CTL1 & 0x1FF;
    }
    else if (b4bitMode == GDI_OSD_8B_MODE)
    {
        bMode = GPU_8B_FONT_MODE;
        wOp_width = (REG_GPU_CTL1 & 0x1FF)<<1;
    }
#ifdef SUPPORT_CT909S
    else// if (b4bitMode == GDI_OSD_2B_MODE)
    {
        bMode = GPU_2B_FONT_MODE;
        wOp_width = GDI_GetStringWidth_2B(pwString)/4;
    }
#endif



    //printf ("wOp_width = %x\n", wOp_width);
    //printf("**\n");

    //wOp_height = ((32) &0x7ff);  // 32x32 Font

#ifdef SUPPORT_CT909S


#ifdef TEST909S

    // the font table for test 909 is fixed to 32 height, and the map of height is different from normal case.
    // for ease of test, just set it directly to 32.
    wOp_height = (GDI_GetStringHeight(pwString) &0x3ff) * bMultipleH; //((26) &0x3ff);  // 26 heigh Font
#else
    wOp_height = (GDI_GetStringHeight(pwString) &0x3ff) * bMultipleH; //((26) &0x7ff);
#endif


    wAg_width = (wOp_width + (dwAddress & 0x3) + 3) >> 2;
    wAg_offset = (((wOSDWidth>>b4bitMode) + 3)>>2) - wAg_width + 1;

    REG_GPU_OP_SIZE = (wOp_height << 16) + wOp_width;
    REG_GPU_AG_OFF = (wAg_offset) << 16;

#else

#ifdef TEST909S
    // the font table for test 909 is fixed to 32 height, and the map of height is different from normal case.
    // for ease of test, just set it directly to 32.
    wOp_height = ((32) &0x7ff);  // 32x32 Font
#else
    wOp_height = (GDI_GetStringHeight(pwString) &0x7ff); //((26) &0x7ff);
#endif



    wAg_width = (wOp_width + (dwAddress & 0x7) + 7) >> 3;
    wAg_offset = (((wOSDWidth>>b4bitMode) + 7)>>3) - wAg_width + 1;

    REG_GPU_OP_SIZE = (wOp_height << 16) + wOp_width;
    REG_GPU_AG_OFF = ((wAg_width << 8) + wAg_offset) << 16;

#endif

    REG_GPU_DEST_ADDR = dwAddress;
    REG_GPU_CTL1 = pParm->bColorKey  << 16;

    asm volatile ( "" : : : "memory" );

#ifdef SUPPORT_CT909S

    REG_GPU_FONT_CONFIG = (_dwGPUFontWidth << 24) |( pwString[0] << 16) |(_dwGPUFontCapacity);

    if (b2BitFont)  // Alan2.11, Set the 2 bit font or 1 bit font
    {
        REG_GPU_CTL0 = GPU_INT_EN | GPU_OP | (4 << 16)  | (4 << 20) |
            (bColorKeyEnable << 8) | (bMode << 2) | GPU_START | GPU_ACCESS_FONT_EN;
    }
    else
    {
        REG_GPU_CTL0 = GPU_INT_EN | GPU_OP | (4 << 16)  | (4 << 20) |
            (bColorKeyEnable << 8) | (bMode << 2) | GPU_START | GPU_ACCESS_FONT_EN | GPU_FONT_1BIT;
    }
/*
#ifdef TEST_1_BIT_FONT
        REG_GPU_CTL0 = GPU_INT_EN | GPU_OP | (4 << 16)  | (4 << 20) |
        (bColorKeyEnable << 8) | (bMode << 2) | GPU_START | GPU_ACCESS_FONT_EN | GPU_FONT_1BIT;
#else
    REG_GPU_CTL0 = GPU_INT_EN | GPU_OP | (4 << 16)  | (4 << 20) |
        (bColorKeyEnable << 8) | (bMode << 2) | GPU_START | GPU_ACCESS_FONT_EN;
#endif
*/



#else
    REG_GPU_CTL0 = GPU_INT_EN | GPU_REQ_EN | GPU_W_REL_EN | ( pwString[0] << 24) | (GDI_NUM_OF_FONT << 10) |
        (0x10 << 16) | (bColorKeyEnable << 8) | (bMode << 2) | GPU_START;
#endif

    //while (REG_GPU_CTL0 & GPU_STATUS) { }
    _gdi_CheckCmdComplete();

#ifndef GDI_WITHOUT_OS
    OS_UnlockMutex(&__mutexGDIIdle);
#endif

}


void _gdi_ResetVPU(void)
{
	DWORD   dwSaveInt;

    HAL_DISABLE_INTERRUPTS(dwSaveInt);	// Disable INT to avoid different threads accessing
#ifdef SUPPORT_CT909S
    REG_PLAT_RESET_CONTROL_ENABLE = PLAT_RESET_VPU_ENABLE;
    REG_PLAT_RESET_CONTROL_DISABLE = PLAT_RESET_VPU_DISABLE;
#else
    MACRO_PLAT_KEY_LOCK();			// Use key lock to avoid different processor accessing.

	// Reset VPU
    // Platform Reset Control Reg bit [22] = 0 (reset), and set to 1 (normal) again.

    REG_PLAT_RESET_CONTROL &= ~PLAT_RESET_VPU;
    REG_PLAT_RESET_CONTROL |= PLAT_RESET_VPU;

    MACRO_PLAT_KEY_UNLOCK();		// Release key.
#endif

    HAL_RESTORE_INTERRUPTS(dwSaveInt);	// Restore INT.


}


// Brian0.68
// Use loop instead of yield CPU, because we hope GDI command is as fast as possible.
void _gdi_CheckCmdComplete(void)
{
#define GDI_CMD_ACK_LOOP    50000L

    DWORD   dwCount;

    dwCount = 0;
    while (REG_GPU_CTL0 & GPU_STATUS)
    {
        MACRO_IDLE();
        dwCount++;
        if (dwCount >  GDI_CMD_ACK_LOOP)
        {
#ifndef GDI_NO_PRINTF
            printf("GDI Cmd time out\n");
#endif
            // Reset VPU
            // Platform Reset Control Reg bit [22] = 0 (reset), and set to 1 (normal) again.
            //REG_PLAT_RESET_CONTROL &= ~PLAT_RESET_VPU;
            //REG_PLAT_RESET_CONTROL |= PLAT_RESET_VPU;

            _gdi_ResetVPU();

            break;
        }


    }

}

/* --Brian1.15, reduce code size.
void GDI_NumberToString(WORD wNum)
{
#if 0

#ifndef NO_GDI
    _wGDITemp = 10000;

    for (_bGDITemp=5; _bGDITemp>0; _bGDITemp--)
    {
        if (wNum / _wGDITemp)
            break;
        _wGDITemp = _wGDITemp/10;
    }
    if (_bGDITemp == 0)
        _bGDITemp = 1;

    // Brian1.06
    if (_wGDITemp == 0)
        _wGDITemp = 1;

    __wGDIString[0] = _bGDITemp;

    for (_bGDITemp=1; _bGDITemp<=__wGDIString[0]; _bGDITemp++)
    {
        __wGDIString[_bGDITemp] = aDigits[wNum/_wGDITemp];
        wNum = wNum % _wGDITemp;
        _wGDITemp = _wGDITemp/10;
    }
#endif

#endif

}
*/

/*--Brian1.15, reduce code size.
// Chuan1.00-2, create
void GDI_NumberToStringHex(WORD wNum)
{
#if 0

#ifndef NO_GDI
    __wGDIString[0] = 6; //Chuan1.00-2, 0xXXXX -> 6 digits
    __wGDIString[1] = CHAR_0;
    __wGDIString[2] = CHAR_xx;
    _wGDITemp = 0x1000;

    for (_bGDITemp=3; _bGDITemp<=6; _bGDITemp++)
    {
        __wGDIString[_bGDITemp] = aDigits[wNum/_wGDITemp];
        wNum %= _wGDITemp;
        _wGDITemp /= 0x10;
    }
#endif

#endif

}
*/


#ifdef SUPPORT_CT909S
// for CT909S 2-bit SP OSD mode, each char width must be 4-pixel (byte) alignment

WORD GDI_GetStringWidth_2B(WORD *pString)
{

#ifndef NO_GDI
    DWORD dwTemp, i, dwWidth;

    dwTemp = 0;
    // calculate total text width
    for (i=0; (i<pString[0]) /*&& (_bGDITemp<GDI_STRING_LENGTH)*/; i++)

    {


        // font width bit [13:9]
        // 0:  64
        // 1:    2
        // 2:    4
        // 3:    6
        // 4:    8
        // ...   ...
        // 31: 62

        dwWidth = (pString[i+1] & 0x3E00) >> 8; // get width
        if (dwWidth)
        {
            dwWidth = ((dwWidth+3)>>2) <<2;  // make sure it is 4-pixel alignment
            dwTemp += dwWidth;
        }
        else
        {
            dwTemp += 64;
        }

    }

    return (WORD)dwTemp;
#else
    return 0;

#endif //#ifndef NO_GDI

}

#endif


BYTE GDI_GetFontWidth(void)
{
    return _bFontWidth;
}
//Aron,2.77-909P, Port Damon's code to files to support Unicode DIR/FILE name & external subtitle.
#ifdef SUPPORT_UNICODE_CODING
WORD GDI_GetUniFontHeightWidth(WORD wChar)
{
    return CONVCHAR_Uni_To_FontSize(wChar);
}

BYTE GDI_GetUniFontWidth(WORD wChar)
{
    WORD wTemp;

    wTemp = GDI_GetUniFontHeightWidth(wChar);
    if (wTemp & 0x3E00)
    {
        return  (BYTE)((wTemp & 0x3E00) >> 8); // get width
    }
    else
    {
        return 32;
    }
}

WORD GDI_GetUniStringWidth(WORD *pString)
{
    WORD wStrWidth, wFonWidth, i;

    wStrWidth = 0;
    // calculate total text width
    for (i = 0; i < pString[0]; i++)
    {
        wFonWidth = GDI_GetUniFontHeightWidth(pString[i+1]);
        if (wFonWidth & 0x3E00)
        {
            wStrWidth += ((wFonWidth & 0x3E00) >> 8); // get width
        }
        else
        {
            wStrWidth += 32;
        }
    }
	
    return wStrWidth;
}
#endif

WORD GDI_GetStringWidth(WORD *pString)
{

#ifndef NO_GDI
    DWORD dwTemp, i;

    dwTemp = 0;
    // calculate total text width
    for (i=0; (i<pString[0]) /*&& (_bGDITemp<GDI_STRING_LENGTH)*/; i++)

    {
#ifdef SUPPORT_CT909S
        // font width bit [13:9]
        // 0:  64
        // 1:    2
        // 2:    4
        // 3:    6
        // 4:    8
        // ...   ...
        // 31: 62
        if ((pString[i+1] & 0x3E00))
        {
            dwTemp += ((pString[i+1] & 0x3E00) >> 8); // get width
        }
        else
        {
            dwTemp += 64;
        }
#else
    // font width bit [13:10]
        // 0:  32
        // 1:    2
        // 2:    4
        // 3:    6
        // 4:    8
        // ...   ...
        // 15: 30
        if ((pString[i+1] & 0x3C00))
        {
            dwTemp += ((pString[i+1] & 0x3C00) >> 9); // get width
        }
        else
        {
            dwTemp += 32;
        }

#endif


    }

    return (WORD)dwTemp;
#else
    return 0;

#endif //#ifndef NO_GDI

}


BYTE GDI_GetCharWidth(WORD wChar)
{
#ifndef NO_GDI

#ifdef SUPPORT_CT909S

    if ((wChar & 0x3E00))
    {
        return ((wChar & 0x3E00) >> 8) ; // get width
    }
    else
    {
        return 64;
    }

#else

    if ((wChar & 0x3C00))
    {
        return ((wChar & 0x3C00) >> 9) ; // get width
    }
    else
    {
        return 32;
    }

#endif

#endif //#ifndef NO_GDI
}


BYTE GDI_GetStringHeight(WORD *pString)
{

#ifndef NO_GDI
    DWORD dwTemp;

// font width bit [15:14]
        // 0:    20
        // 1:    18
        // 2:    26
        // 3:    32

    dwTemp = ((pString[1] & 0xC000) >> 14);
    switch (dwTemp)
    {
    case 0:
        dwTemp = 20;
        break;
    case 1:
        dwTemp = 18;
        break;
    case 2:
        dwTemp = 26;
        break;
    case 3:
        dwTemp = 32;
        break;

    }
    return (BYTE)dwTemp;

#else

    return 0;
#endif //#ifndef NO_GDI
}


// Brian0.85
// Draw dot to video frame buffer, x, y must be 2n
// one dot == 4 pixels
void    GDI_FBDrawDot(DWORD dwFrameIdx, DWORD dwX, DWORD dwY, BYTE bColorY, BYTE bColorU, BYTE bColorV)
{
    DWORD /*dwFrameIdx, */dwStrip;
    PBYTE pbYAddr, pbUVAddr;

    //HAL_ReadInfo(HAL_INFO_DISPFRAME, &dwFrameIdx);

    //pbYAddr = (PBYTE)((DS_FRAMEBUF_ST) + (0x98800*dwFrameIdx));
    //pbUVAddr = (PBYTE)((DS_FRAMEBUF_ST) + (0x98800*dwFrameIdx) + Y_MAXSIZE_CB00*8);
    //pbUVAddr = (PBYTE)((DS_FRAMEBUF_ADDR) + (0x98800*dwFrameIdx) + Y_MAXSIZE_A8C0*8); // JPEG  mode


    dwX = dwX&0xFFFE;
    dwY = dwY&0xFFFE;


#ifdef SUPPORT_CT909S

    pbYAddr  = (PBYTE)__DISPFrameInfo[dwFrameIdx].dwFY_Addr;
    pbUVAddr = (PBYTE)__DISPFrameInfo[dwFrameIdx].dwFC_Addr;

    dwStrip = (REG_DISP_STRIPE&0xFF)<<8;

//Aron2.55, update Chuan's code to get correct strip value    
#if defined(CT909P_IC_SYSTEM) || defined(CT909G_IC_SYSTEM)
    dwStrip /= 4; //convert it to UNIT: DWORD, unit of REG_DISP_STRIPE is BYTE in 909P
#endif

    pbYAddr += ((dwY>>4)*dwStrip + ((dwX>>2)*64) + (dwY&0xF)*4 + (dwX%4));

    dwX /= 2;
    dwY /= 2;
    pbUVAddr += ((dwY>>4)*dwStrip + ((dwX>>3)<<8) + (((dwX&7)>>2)<<6) + (dwY&0xF)*4 + (dwX&3));


    *(pbYAddr) = bColorY;
    *(pbYAddr+1) = bColorY;
    *(pbYAddr+4) = bColorY;
    *(pbYAddr+5) = bColorY;
    *(pbUVAddr) = bColorU;
    *(pbUVAddr+128) = bColorV;

#else

    pbYAddr = (PBYTE) ((REG_DISP_FY_ADDR(dwFrameIdx)*8) + 0x40000000);
    pbUVAddr = (PBYTE)((REG_DISP_FC_ADDR(dwFrameIdx)*8) + 0x40000000);

    dwStrip = (REG_DISP_MAIN1_CR&0xFF)<<8;

    pbYAddr += ((dwY>>4)*dwStrip + (dwX>>3)*128 + (dwY%16)*8 + (dwX%8));
    pbUVAddr += ((dwY>>5)*dwStrip + (dwX>>3)*128 + ((dwY/2)%16)*8 + (dwX/2)%4);

    *(pbYAddr) = bColorY;
    *(pbYAddr+1) = bColorY;
    *(pbYAddr+8) = bColorY;
    *(pbYAddr+9) = bColorY;
    *(pbUVAddr) = bColorU;
    *(pbUVAddr+4) = bColorV;

 #endif


}


// start X, Y must be 2n,
// width and height must be 2n
void GDI_FBFillRect(DWORD dwFrameIdx, URECT *pRect, DWORD dwRGB)
{
#define FB_WIDTH    704
#define FB_HEIGHT   480//Brian1.02, set to full NTSC height: In Thumbnail mode, we may see garbage at bottom of LCD monitor (PSCAN) if not fill the whole frame buffer //460

    DWORD i, j;
    DWORD dwYUV;

	// do boundary check
    if (pRect->wLeft >= FB_WIDTH ||
        pRect->wTop >= FB_HEIGHT ||
        pRect->wLeft > pRect->wRight ||
        pRect->wTop > pRect->wBottom)
        return; // the position is not in the area

    if (pRect->wBottom >= FB_HEIGHT)
        pRect->wBottom = FB_HEIGHT-1;

    if (pRect->wRight == pRect->wLeft) // Brian1.22, width need to be 2n, doesn't support 1-pixel width
        return;


    dwYUV = COMUTL_RGB2YUV(dwRGB);

    for (i=pRect->wTop; i<pRect->wBottom; i+=2)
        for (j=pRect->wLeft ; j<pRect->wRight; j+=2)
        {

            GDI_FBDrawDot(dwFrameIdx, j, i, (dwYUV >> 16), (dwYUV >> 8), dwYUV &0xFF);
        }


}


// start X, Y must be 2n,
// width and height must be 2n
// thickness must be 2n
void GDI_FBDrawRect(DWORD dwFrameIdx, URECT *pRect, BYTE bThickness, DWORD dwRGB)
{
    URECT rTmp;

    // top rect
    rTmp.wTop = pRect->wTop;
    rTmp.wLeft = pRect->wLeft;
    rTmp.wRight = pRect->wRight;
    rTmp.wBottom = pRect->wTop + bThickness-1;
    GDI_FBFillRect(dwFrameIdx, &rTmp, dwRGB);

    // left rect
    rTmp.wTop = pRect->wTop;
    rTmp.wLeft = pRect->wLeft;
    rTmp.wRight = pRect->wLeft + bThickness-1;
    rTmp.wBottom = pRect->wBottom;
    GDI_FBFillRect(dwFrameIdx, &rTmp, dwRGB);

    // right rect
    rTmp.wTop = pRect->wTop;
    rTmp.wLeft = pRect->wRight -bThickness+1;
    rTmp.wRight = pRect->wRight;
    rTmp.wBottom = pRect->wBottom;
    GDI_FBFillRect(dwFrameIdx, &rTmp, dwRGB);

    // bottom rect
    rTmp.wTop = pRect->wBottom -bThickness+1;
    rTmp.wLeft = pRect->wLeft;
    rTmp.wRight = pRect->wRight;
    rTmp.wBottom = pRect->wBottom;
    GDI_FBFillRect(dwFrameIdx, &rTmp, dwRGB);


}

#ifdef SUPPORT_STB

void _gdi_FontScaling(WORD *pwString, BOOL bScaleW, BOOL bScaleH)
{
    DWORD dwFontLineData, dwNewLineData, dwTemp;
    DWORD *pwSource, *pwTarget;
    BYTE  bFontByte, bFontHeight;
    BYTE  bTemp, bTemp1, bTemp2;


    bFontHeight = GDI_GetStringHeight(pwString);
    bFontByte = bFontHeight * 32 / 8;

#if (FONT_TABLE_METHOD == TRADITION) // Alan2.00, copy the font from flash to font table in DRAM
    bFontByte *= 2;  // 2 bits mode
    pwTarget = (DWORD*) (__dwGDIFontTableBase + _wFontNumberInTable * OSD_2BIT_CHAR_SIZE);
#else
    pwTarget = (DWORD*) (__dwGDIFontTableBase + _wFontNumberInTable * bFontHeight * 32 / 8);
#endif

/*
    // Alan2.18, Moving to GDI_DrawScaleString().
    // Alan1.00, Disable the font table protection
    MACRO_MCU_DISABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);
*/

    for (bTemp = 0; bTemp < bFontHeight; bTemp++)
    {
        pwSource = (DWORD*) (__dwGDIFontTableBase + (pwString[1] & 0x1FF) * bFontByte +
                             bTemp * sizeof(DWORD));
        dwFontLineData = *pwSource;

        // ScaleW is only valid for width <= 32 after scaling
        if (bScaleW)
        {
            dwNewLineData = 0x0;
            for (bTemp1 = GDI_GetStringWidth(pwString); bTemp1 > 0; bTemp1--)
            {
                for (bTemp2 = 0; bTemp2 < 2; bTemp2++)
                {
                                                                      // N : the width of font
                    dwTemp = 0x80000000 >> (2 * bTemp1 - 1 - bTemp2); // set index 2N-1, 2N-2 as 1

                    // Extract bit 2N and 2N -1 and store in dwTemp
                    dwTemp &= dwFontLineData >> (bTemp1 - bTemp2);    // shift bit N to 2N and 2N-1

                    dwNewLineData |= dwTemp;
                }
            }
        }
        else
        {
            dwNewLineData = dwFontLineData;
        }


        if (bScaleH)
        {
                *(pwTarget + 2 * bTemp) = dwNewLineData;
                *(pwTarget + 2 * bTemp + 1) = dwNewLineData;
        }
        else
        {
            *(pwTarget + bTemp) = dwNewLineData;
            *(pwTarget + bTemp + 1) = dwNewLineData;
        }
    }

/*
    // Alan2.18, to fix the following problem.
    // Draw "y" and then "x", the bottom of "x" will have the remain data of "y".
    // To dirty fix the bug.
    // The last copying command and the protect command should have enough delay time.
    // Move to GDI_DrawScaleString().

    MACRO_MCU_ENABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);
*/
}

void GDI_DrawFont(DWORD dwFontAddr, DWORD dwColor, WORD wWidth, WORD wHeight, WORD wX, WORD wY)
{
    WORD wDWPerLine = (wWidth + 31) / 32;
    DWORD dwAddress = DS_OSDFRAME_ST;
    WORD wOSDWidth = __RegionList[0].wWidth;
    WORD wOp_width, wOp_height, wAg_width, wAg_offset;    

    OS_LockMutex(&__mutexGDIIdle);    
    _gdi_ResetVPU();    

    REG_GPU_CTL0 |= GPU_OP | GPU_ACCESS_FONT_EN;    // enable, gpu_op and LEON font access
    REG_GPU_FONT_ADDR = (DWORD)dwFontAddr;  // must 4bytes alignment
    REG_GPU_COL_NDX = dwColor;    
    REG_GPU_FONT_RAM_INDEX = (DWORD)((wWidth + 1) >> 1) << 9;    
    dwAddress += (DWORD)wY * wOSDWidth + wX;    
    wOp_width = (REG_GPU_CTL1 & 0x1ff) << 1;    // unit: bytes    
    wOp_height = wHeight;    
    wAg_width = (wOp_width + (dwAddress & 3) + 3) >> 2;
    wAg_offset = ((wOSDWidth + 3) >> 2) - wAg_width + 1;
    REG_GPU_OP_SIZE = ((DWORD)wOp_height << 16) + wOp_width;
    REG_GPU_AG_OFF = (DWORD)wAg_offset << 16;
    REG_GPU_DEST_ADDR = dwAddress;
    REG_GPU_CTL1 = 0;
    REG_GPU_FONT_CONFIG = ((DWORD)wDWPerLine << 24) | (1 << 16) | ((DWORD)wDWPerLine * wHeight);
    REG_GPU_CTL0 = GPU_INT_EN | GPU_OP | (4 << 16)  | (4 << 20) |
        (0 << 8) | (1 << 2) | GPU_START | GPU_ACCESS_FONT_EN | GPU_FONT_1BIT;

    _gdi_CheckCmdComplete();
    OS_UnlockMutex(&__mutexGDIIdle);
}

void GDI_DrawScaleString(BYTE bRegionId, PARM_DRAW_STRING *pParm, WORD *pwString, BOOL bScaleW, BOOL bScaleH)
{

#ifndef NO_GDI

    DWORD dwBackupAddr;
    WORD theString[GDI_STRING_LENGTH+1];
    PARM_DRAW_STRING parm;
    WORD  wFontWidth;
    BYTE  bFontHeight;
    BYTE  b2BitFont;

    // Alan2.18, Moving protect function from _gdi_FontScaling() to here to fix the following bug
    // Draw "y" and then "x", the bottom of "x" will have the remain data of "y".

    MACRO_MCU_DISABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);

    _gdi_FontScaling(pwString, bScaleW, bScaleH);

    bFontHeight = GDI_GetStringHeight(pwString);

#if (FONT_TABLE_METHOD == TRADITION)
       b2BitFont = TRUE;
       GDI_SetFontTableSize(2, 2 * 32 * _bFontHeightInTable / (8 * 4));
    GDI_SetFontTableAddress (__dwGDIFontTableBase);
#elif (FONT_TABLE_METHOD == SPLIT_2_1_BIT)
       b2BitFont = FALSE;
       if (bScaleH)
       {
           GDI_SetFontTableSize(1, 32 * _bFontHeightInTable * 2/ (8 * 4) ); // default set as 32xH, 1-bit font
       }
       else
       {
           GDI_SetFontTableSize(1, 32 * _bFontHeightInTable / (8 * 4) ); // default set as 32xH, 1-bit font
       }

    dwBackupAddr = __dwGDIFontTableBase;
    GDI_SetFontTableAddress (__dwGDIFontTableBase +
                                (_wFontNumberInTable * 32 * bFontHeight / 8));
#endif

    parm = *pParm;

    // Alan2.18, Moving protect function to here to fix the following problem.
    // Draw "y" and then "x", the bottom of "x" will have the remain data of "y".
    // To dirty fix the bug.
    // The last copying command and the protect command should have enough delay time.
    // Move from _gdi_FontScaling().

    MACRO_MCU_ENABLE_PROTECTION(MCU_WRITE_PROTECT_AREA3_ENABLE);

    theString[1] = (pwString[1] & 0xFE00);
    if (bScaleW)
        {
        wFontWidth = ((theString[1] & 0x3E00) >> 8); // scale width
        theString[1] = (theString[1] & 0xC1FF) | (wFontWidth << 9);
        }

    theString[0] = 1;

       if (bScaleH)
       {
           _gdi_DrawString(bRegionId, &parm, theString, b2BitFont, 2);
       }
       else
       {
           _gdi_DrawString(bRegionId, &parm, theString, b2BitFont, 1);
       }

    GDI_SetFontTableAddress(dwBackupAddr);
#endif //#ifndef NO_GDI
}
#endif // #ifdef SUPPORT_STB

/*
void GDI_TestShading(void)
{
    BYTE i, aShade[38];
    PARM_RECT rect;

    for (i=0; i<38; i++)
    {
        GDI_ChangePALEntry(95+i, 255-i*4, FALSE);
        aShade[i] = 95+i;
    }
    GDI_WaitPaletteComplete();

    rect.bColor = 38;
    rect.bShadePtr = aShade;
    SET_RECT(rect.rect, 100, 150, 300, 187);
    GDI_FillRect(0|GDI_SHADING_OPERATION, &rect);

    rect.bColor = 38;
    rect.bShadePtr = aShade;
    SET_RECT(rect.rect, 100, 250, 300, 287);
    GDI_FillRoundRect(0|GDI_SHADING_OPERATION, 19, &rect);

    SET_RECT(rect.rect, 350, 150, 550, 300);
    //GDI_FillRoundRectTop(0|GDI_SHADING_OPERATION, 19, &rect);
    GDI_FillRoundRectBottom(0|GDI_SHADING_OPERATION, 19, &rect);

}
*/


/*
void GDI_Test(void)
{
    PARM_RECT parm;
    PARM_DRAW_STRING strParm;
    WORD wTestStr[]={4, CHAR_T, CHAR_E, CHAR_S, CHAR_T}; //{4, 0x201D, 0x200E, 0x201C, 0x201D}; //{4, CHAR_T, CHAR_E, CHAR_S, CHAR_T};
    DWORD i, *pdwPtr, dwTmp;

    //GDI_Initial();
    //GDI_ActivateRegion(0);
    //UTL_DelayTime(COUNT_50_MSEC, FALSE);

    GDI_LoadPalette(0);
    UTL_DelayTime(COUNT_100_MSEC, FALSE);

    printf("________________\n");
    for (i=0; i<16; i++)
    {
        printf("pal[%x]=%lx\n", i, _dwGDIPalette[i]);
    }

    printf("________________\n");

    pdwPtr = (DWORD*)0x80001c00;
    REG_DISP_BRIGHT_CR |= 0x01000000;
    for (i=0; i<16; i++)
    {
        REG_VLD_SHO32 = 0xFFFFFFFF;
        asm volatile ( "" : : : "memory" );
        dwTmp = REG_DISP_GAM_OSD(i);
        asm volatile ( "" : : : "memory" );
        dwTmp = REG_DISP_GAM_OSD(i);
        asm volatile ( "" : : : "memory" );
        REG_VLD_SHO32 = 0xFFFFFFFF;
        printf("reg pal[%x]=%lx\n", i, dwTmp) ;//pdwPtr[i]);

        UTL_DelayTime(COUNT_100_MSEC, FALSE);
    }
    REG_DISP_BRIGHT_CR &= ~(0x03008000);
    printf("________________\n");

    pdwPtr = (DWORD*)0x80001c00;
    for (i=0; i<16; i++)
    {
        REG_VLD_SHO32 = 0xFFFFFFFF;
        asm volatile ( "" : : : "memory" );
        dwTmp = *(pdwPtr+i);
        asm volatile ( "" : : : "memory" );
        dwTmp = *(pdwPtr+i);
        asm volatile ( "" : : : "memory" );
        REG_VLD_SHO32 = 0xFFFFFFFF;
        printf("reg pal[%x]=%lx\n", i, dwTmp) ;//pdwPtr[i]);

        UTL_DelayTime(COUNT_100_MSEC, FALSE);
    }


    parm.bColor = 7; // transparenct color is the last entry
    parm.rect.wLeft = 0; //100;
					parm.rect.wTop = 0; //100;
					parm.rect.wRight = 99; //200;
					parm.rect.wBottom = 99; //200;
					GDI_FillRect(0, &parm);

					strParm.bBackground = 8;
					strParm.bTextColor = 7;
					strParm.bShadow1 = 12;
					strParm.bShadow2 = 14;
					strParm.wX = 20;
					strParm.wY = 20;
					strParm.bColorKey = 0;

					GDI_DrawString(0, &strParm, wTestStr);

                    parm.bColor = 8; // transparenct color is the last entry
					parm.rect.wLeft = 150;
					parm.rect.wTop = 150;
					parm.rect.wRight = 250;
					parm.rect.wBottom = 250;
					GDI_FillRect(0, &parm);





}
*/

/*--Brian1.15, reduce code size
void GDI_EnablePalette(void)
{
#ifdef SUPPORT_CT909S
    DISP_SetPalette(_dwGDIPalette);
#else
    DISP_SetPalette(FALSE, _dwGDIPalette);
#endif
}
*/




#ifdef GDI_STANDALONE_LIBRARY

DWORD COMUTL_RGB2YUV(DWORD dwRGB)
{
	BYTE _bCOMUTLTmp1, _bCOMUTLTmp2, _bCOMUTLTmp3;
	UDWORD uDWord;
	DWORD __dwCOMUTLTemp;

    // use uDWord.cS[0] ~ [3] to store YUV value
    // use _wCOMUTLTemp to store final value

    // Step 0: split RGB value
    _bCOMUTLTmp1=(RGB_R(dwRGB));
    _bCOMUTLTmp2=(RGB_G(dwRGB));
    _bCOMUTLTmp3=(RGB_B(dwRGB));

// Micky1.20, 4M flash, reduce common bank lib.
// don't use float point.
// can't use DWORD, must use signed operation
#ifdef  NO_FLOAT_POINT
    // Step 1: Calculate YUV value depending on input RGB

    uDWord.cS[0]=(BYTE)(16+(65738L*(long)_bCOMUTLTmp1
                      +129057L*(long)_bCOMUTLTmp2
                      +25064L*(long)_bCOMUTLTmp3)/256000);  // Y value

    //uDWord.cS[1]=(BYTE)(128+(-37945L*(long)_bCOMUTLTmp1
    //                   -74494L*(long)_bCOMUTLTmp2
    //                   +112439L*(long)_bCOMUTLTmp3)/256000); // U value

    __dwCOMUTLTemp = 32768000 + (-37945L*(long)_bCOMUTLTmp1
                       -74494L*(long)_bCOMUTLTmp2
                       +112439L*(long)_bCOMUTLTmp3);
    uDWord.cS[1] =  (BYTE)(__dwCOMUTLTemp/256000);  // U value


    //uDWord.cS[2]=(BYTE)(128+(112439L*(long)_bCOMUTLTmp1
    //                   -94154L*(long)_bCOMUTLTmp2
    //                   -18285L*(long)_bCOMUTLTmp3)/256000);  // V value


    __dwCOMUTLTemp= 32768000+(112439L*(long)_bCOMUTLTmp1
                       -94154L*(long)_bCOMUTLTmp2
                       -18285L*(long)_bCOMUTLTmp3);
    uDWord.cS[2] =  (BYTE)(__dwCOMUTLTemp/256000); // V value

// debug the value between float and long
// float > long by 1.
/*
    uWord.cS[0]=(BYTE)(16+(65738*(float)_bCOMUTLTmp1
                      +129057*(float)_bCOMUTLTmp2
                      +25064*(float)_bCOMUTLTmp3)/256000);  // Y value
    uWord.cS[1]=(BYTE)(128+(-37945*(float)_bCOMUTLTmp1
                       -74494*(float)_bCOMUTLTmp2
                       +112439*(float)_bCOMUTLTmp3)/256000); // U value
    uWord.cS[1]=(BYTE)(128+(112439*(float)_bCOMUTLTmp1
                       -94154*(float)_bCOMUTLTmp2
                       -18285*(float)_bCOMUTLTmp3)/256000);  // V value
    if (uDWord.cS[2] != uWord.cS[1])
        printf("\nError");
*/
#else

    // Step 1: Calculate YUV value depending on input RGB
    uDWord.cS[0]=(BYTE)(16+(65738*(float)_bCOMUTLTmp1
                      +129057*(float)_bCOMUTLTmp2
                      +25064*(float)_bCOMUTLTmp3)/256000);  // Y value

    uDWord.cS[1]=(BYTE)(128+(-37945*(float)_bCOMUTLTmp1
                       -74494*(float)_bCOMUTLTmp2
                       +112439*((float)_bCOMUTLTmp3)/256000)); // U value
    uDWord.cS[2]=(BYTE)(128+(112439*(float)_bCOMUTLTmp1
                       -94154*(float)_bCOMUTLTmp2
                       -18285*((float)_bCOMUTLTmp3)/256000));  // V value
#endif

    // Step 2: merge YUV value into DWORD unit and match OSD spec.
	__dwCOMUTLTemp=(MAKE_RGB(uDWord.cS[0], uDWord.cS[1], uDWord.cS[2]));
    return  __dwCOMUTLTemp;

}

#endif


