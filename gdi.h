/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved.

        Product : WinAV Firmware

        Date    : 2003.01.08.
        Author  : Cheertek (D300 BrianCheng)
        Purpose : Graphic Device Interface Module
                  The moudle provides BMP OSD graphic functions
        Sources : gdi.h/ gdi.c/ gdiex.c

------------------------------------------------------------
        Update

        Date    : 
        Name    : Cheertek (D300 Briancheng)
        Description     : ???

                        Item                                    Source
        ----------------------------------------        -------------------
        . 
****************************************************************************/

#ifndef __GDI_H__
#define __GDI_H__

#ifdef AP_FRAMEWORK
#ifndef GDI_WITHOUT_OS
#define GDI_WITHOUT_OS
#endif
#ifndef GDI_STANDALONE_LIBRARY
#define GDI_STANDALONE_LIBRARY
#endif
#ifndef GDI_NO_PRINTF
#define GDI_NO_PRINTF
#endif



#endif
// Micky2.16p, for simple AP
/*
#ifndef GDI_STANDALONE_LIBRARY
#include "initial.h" // for including MainVideoInfo
#endif
*/

//#define NO_GDI

#define TRADITION            0      // Use the traditional 2-bit font table
#define SPLIT_2_1_BIT        1      // Use the two seperated font table, 2-bit and 1-bit
#define CREATE_DYNAMICALLY   2      // Dynamically copy font from FLASH to font table in DRAM

/*--CoCo2.37p
#include "OSDString/fontindx_2bit.def"

#if (FONT_TABLE_METHOD != TRADITION)
#include "OSDString/fontindx_1bit.def"
#endif
*/

//Alan2.16, for double height and width of teletext
#ifdef SUPPORT_STB
//#include "STB/usrif/Fontindx_TELETEXT.def"
#include "STB/usrif/Fontindx_TELETEXT_LATIN.def"
#include "STB/usrif/Fontindx_TELETEXT_MOSAICS.def"
#endif


//Alan2.00
// The following number, 8, is gotten from 
// each char has 32(W) * FONT_HEIGHT * 2(bits) / 8 byte in 2 bit mode
#define OSD_2BIT_CHAR_SIZE                (8 * OSD_FONT_HEIGHT)
#define OSD_1BIT_CHAR_SIZE                (4 * OSD_FONT_HEIGHT)



#ifdef SUPPORT_CHAR_SUBPICTURE // Brian1.10
//Alan1.10 for support char-based subpicture
//Aron,2.77-909P, Port Damon's code to files to support Unicode DIR/FILE name & external subtitle.
#if defined(ISO_8859_2_FONT) || defined(CP_1250_FONT)
#include "OSDString/ISO_8859_2_FontIndx.def"
#endif
#ifdef ISO_8859_7_FONT
#include "OSDString/ISO_8859_7_FontIndx.def"
#endif
// Damon2.51, for Hebrew
#ifdef ISO_8859_8_FONT
#include "OSDString/ISO_8859_8_FontIndx.def"
#endif
#ifdef ISO_8859_9_FONT
#include "OSDString/ISO_8859_9_FontIndx.def"
#endif
#ifdef ISO_8859_15_FONT
#include "OSDString/ISO_8859_15_FontIndx.def"
#endif
#ifdef CP_1251_FONT
#include "OSDString/CP_1251_FontIndx.def"
#endif
//Aron,2.77-909P, Port Damon's code to files to support Unicode DIR/FILE name & external subtitle.
#ifdef SUPPORT_UNICODE_CODING
#include "OSDString/UNICODE_FontIndx.def"
#endif
#endif

#ifdef SUPPORT_STB
#include "OSDString/6937_FontIndx.def"
#endif

//#include "colordef.h"
//////////////////////////////////////////////////////////// Move from COLORDEF.H code ///////////////////////
// Because include colordef.h in gdi.h will make colordef.h include miss in some PC environment when happen Src_235a string. So move to 
// gdi.h to avoid this problem.


//Define the mix ratio for general UI
#define GDI_GENERAL_MIX_RATIO                           24

//The starting palette index for special UIs that it will exist with other UI together, ex. normal display, common dialog box.
#define GDI_SPECIAL_PALETTE_INDEX_START                 1

//define the starting palette index for normal display
#define GDI_ND_PALETTE_INDEX_START                      1

#if (DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)
//define the number of palettes for normal display
#define GDI_ND_PALETTE_NUM                              11
#else
//define the number of palettes for normal display
#define GDI_ND_PALETTE_NUM                              13
#endif //(DRAM_CONFIGURATION_TYPE_DVD == DRAM_SIZE_16)

//define the starting palette index for OSD dynamic normal display
#define GDI_DYNAMIC_ND_PALETTE_INDEX_START              2

//define the number of palettes for OSD dynamic normal display
#define GDI_DYNAMIC_ND_PALETTE_NUM                      2

//define the starting palette index for GDI low-level function
#define GDI_LOW_LEVEL_FUNCTION_PALETTE_INDEX_START      (GDI_ND_PALETTE_INDEX_START+GDI_ND_PALETTE_NUM)

//define the number of palettes for GDI low-level functions like scroll bar
#define GDI_LOW_LEVEL_FUNCTION_PALETTE_NUM              4

//define the starting palette index for common dialog box.
#define GDI_COMMON_DIALOG_PALETTE_INDEX_START           (GDI_LOW_LEVEL_FUNCTION_PALETTE_INDEX_START+GDI_LOW_LEVEL_FUNCTION_PALETTE_NUM)

//define the number of palettes for common dialog box
#define GDI_COMMON_DIALOG_PALETTE_NUM                   11

//define the starting palette index for general UI.
//Note: General UI should refer to this define to define its starting palette index 
#define GDI_GENERAL_UI_PALETTE_INDEX_START              (GDI_COMMON_DIALOG_PALETTE_INDEX_START+GDI_COMMON_DIALOG_PALETTE_NUM) //30

#define GDI_GENERAL_UI_PALETTE_RESERVE_NUM              25

//Define the starting palette index for the background of main menu
#define GDI_MAIN_MENU_PALETTE_INDEX_START               56

//define the number of palettes for the background of main menu
#define GDI_MAIN_MENU_PALETTE_RESERVE_NUM               40

//Define the starting palette index for toolbar
#define GDI_TOOLBAR_PALETTE_INDEX_START                 (GDI_MAIN_MENU_PALETTE_INDEX_START+GDI_MAIN_MENU_PALETTE_RESERVE_NUM)

//define the number of palettes for toolbar
#define GDI_TOOLBAR_PALETTE_NUM                         8

//Define the starting palette index for menu
#define GDI_MENU_COMPONENT_PALETTE_INDEX_START          (GDI_TOOLBAR_PALETTE_INDEX_START+GDI_TOOLBAR_PALETTE_NUM)

//define the number of palettes for menu
#define GDI_MENU_COMPONENT_NUM                          8

//Define the starting palette index for dialog component
#define GDI_DIALOG_COMPONENT_PALETTE_INDEX_START        (GDI_MENU_COMPONENT_PALETTE_INDEX_START+GDI_MENU_COMPONENT_NUM)

//define the number of palettes for dialog component
#define GDI_DIALOG_COMPONENT_PALETTE_NUM                40

//Define the starting palette index for PIN dialog
#define GDI_PIN_DIALOG_PALETTE_INDEX_START              (GDI_DIALOG_COMPONENT_PALETTE_INDEX_START+GDI_DIALOG_COMPONENT_PALETTE_NUM)

//define the number of palettes for PIN dialog
#define GDI_PIN_DIALOG_PALETTE_NUM                      3

//define the starting palette index for bitmaps
//Note: palette index 95~254 is for bitmap palette usage
#define GDI_BITMAP_PALETTE_INDEX_START                  155

//The palette index for "transparent"
#define PAL_ENTRY_COLOR_TRANSPARENT                     0

// or this flag to indicate that the value is YUV, not RGB
#define	GDI_VALUE_YUV							        0x5A000000




/////////////////////////////////////////////////////////// Move from COLORDEF.H code ///////////////////////
/*
typedef struct tagRECT_
{
    WORD    wLeft;
    WORD    wTop;
    WORD    wRight;
    WORD    wBottom;
} RECT, *PRECT;
*/


/////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////

#define SET_RECT(P,LEFT,TOP,RIGHT,BOTTOM)\
    {\
        P.wLeft=LEFT;\
        P.wTop=TOP;\
        P.wRight=RIGHT;\
        P.wBottom=BOTTOM;\
    }



#define GDI_FILL_RECT(LEFT,TOP,RIGHT,BOTTOM,COLOR)\
    {\
        __GDIParm.u.RectParm.rect.wLeft=(WORD)(LEFT);\
        __GDIParm.u.RectParm.rect.wTop=(WORD)(TOP);\
        __GDIParm.u.RectParm.rect.wRight=(WORD)(RIGHT);\
        __GDIParm.u.RectParm.rect.wBottom=(WORD)(BOTTOM);\
        __GDIParm.u.RectParm.bColor=(BYTE)(COLOR);\
    }

#define GDI_BITMAP_COPY(LEFT,TOP,RIGHT,BOTTOM,X,Y,SRC,DEST)\
    {\
        __GDIParm.u.BitmapCopy.SrcRect.wLeft=LEFT;\
        __GDIParm.u.BitmapCopy.SrcRect.wTop=TOP;\
        __GDIParm.u.BitmapCopy.SrcRect.wRight=RIGHT;\
        __GDIParm.u.BitmapCopy.SrcRect.wBottom=BOTTOM;\
        __GDIParm.u.BitmapCopy.wDestX = X;\
        __GDIParm.u.BitmapCopy.wDestY = Y;\
        __GDIParm.u.BitmapCopy.bSrcRegion = SRC;\
        __GDIParm.u.BitmapCopy.bDestRegion = DEST;\
    }


#define GDI_DRAW_FRAME(LEFT,TOP,RIGHT,BOTTOM,COL_BRIGHT,COL_DARK,COL_CENTER,STYLE,THICKNESS)\
    {\
        __GDIParm.u.Frame.rect.wLeft=LEFT;\
        __GDIParm.u.Frame.rect.wTop=TOP;\
        __GDIParm.u.Frame.rect.wRight=RIGHT;\
        __GDIParm.u.Frame.rect.wBottom=BOTTOM;\
        __GDIParm.u.Frame.bColorBright = COL_BRIGHT;\
        __GDIParm.u.Frame.bColorDark = COL_DARK;\
        __GDIParm.u.Frame.bColorCenter = COL_CENTER;\
        __GDIParm.u.Frame.bStyle = STYLE;\
        __GDIParm.u.Frame.bThickness = THICKNESS;\
    }


#define GDI_DRAW_SCROLL_BAR(LEFT,TOP,RIGHT,BOTTOM,START,DISPLAY)\
    {\
        __GDIParm.u.ScrollBar.rect.wLeft=LEFT;\
        __GDIParm.u.ScrollBar.rect.wTop=TOP;\
        __GDIParm.u.ScrollBar.rect.wRight=RIGHT;\
        __GDIParm.u.ScrollBar.rect.wBottom=BOTTOM;\
        __GDIParm.u.ScrollBar.wStartPoint=START;\
        __GDIParm.u.ScrollBar.wDisplay=DISPLAY;\
    }

#define GDI_DRAW_VOLUME_BAR(LEFT,TOP,RIGHT,BOTTOM,BAR_COLOR,BG_COLOR,VOLUME,ORIENTION)\
    {\
        __GDIParm.u.VolumeBar.rect.wLeft = LEFT;\
        __GDIParm.u.VolumeBar.rect.wTop = TOP;\
        __GDIParm.u.VolumeBar.rect.wRight = RIGHT;\
        __GDIParm.u.VolumeBar.rect.wBottom = BOTTOM;\
        __GDIParm.u.VolumeBar.bBarColor = BAR_COLOR;\
        __GDIParm.u.VolumeBar.bBgColor = BG_COLOR;\
        __GDIParm.u.VolumeBar.wVolume = VOLUME;\
        __GDIParm.u.VolumeBar.bOriention = ORIENTION;\
    }

#define GDI_DRAW_BUTTON(LEFT,TOP,RIGHT,BOTTOM,ID,STYLE)\
    {\
        __GDIParm.u.Button.rect.wLeft = (WORD)(LEFT);\
        __GDIParm.u.Button.rect.wTop = (WORD)(TOP);\
        __GDIParm.u.Button.rect.wRight = (WORD)(RIGHT);\
        __GDIParm.u.Button.rect.wBottom = (WORD)(BOTTOM);\
        __GDIParm.u.Button.bButtonId = (BYTE)(ID);\
        __GDIParm.u.Button.bStyle = (BYTE)(STYLE);\
    }

#define GDI_SET_CURRENT_REGION(ID)\
    {\
        __bCurrentRegionId = ID;\
    }



#define GDI_GET_CHAR_WIDTH(C) GDI_GetCharWidth(C)


#define GDI_SHADING_OPERATION     0x80        // or this bit in bArc to indicate it is a shading opertaion




#define GDI_FillRect_909	        GDI_FillRect 
#define GDI_DrawString_909      GDI_DrawString
#define GDI_GetStringHeight_909 GDI_GetStringHeight
#define GDI_GetStringWidth_909   GDI_GetStringWidth
#define GDI_DrawFrame_909   GDI_DrawFrame
#define GDI_DrawVolumeBar_909 GDI_DrawVolumeBar
#define GDI_DrawScrollBar_909   GDI_DrawScrollBar

// define the number of font, palette and region used in this module
//DMP1.21, add one region for RDS display.
#ifndef SUPPORT_RADIO_RDS
#define NUMBER_OF_REGION	4 
#else
#define NUMBER_OF_REGION	5         
#endif
 
#define NUMBER_OF_IMAGE     1       

#define TEXT_BG_COLOR       0
#define TEXT_FG_COLOR       1
#define TEXT_SHADOW_COLOR_1 2
#define TEXT_SHADOW_COLOR_2 3

#define H2D         // define this will diable pixel horizontal duplication

// The maximum length of a text string, not including the first length value

#ifdef SUPPORT_CT909S // Brian1.50
#define GDI_STRING_LENGTH   14
#else
#define GDI_STRING_LENGTH   20
#endif

#define ORIENTION_VERTICAL   0
#define ORIENTION_HORIZONTAL 1
// Frame style
//   bit 0: 1:push,   0:pop
//   bit 1: 1:solid,  0: empty
//   bit 2: 1:double, 0: single
#define FRAME_PUSH_SOLID    0x03
#define FRAME_POP_SOLID     0x02
#define FRAME_DOUBLE_SOLID  0x06
#define FRAME_PUSH          0x01
#define FRAME_POP           0x00
#define FRAME_DOUBLE        0x04

// Button style
//   bit 0~1: 00:normal, 01:disabled, 10:selected
//   bit 2: 1:with text, 0: without text
#define BUTTON_NORMAL       0x00
#define BUTTON_DISABLED     0x01
#define BUTTON_PUSH         0x02
#define BUTTON_HIGHLIGHT    0x03
#define BUTTON_WITH_TEXT    0x04


#define BUTTON_ID_LANGUAGE	0
#define BUTTON_ID_SCREEN	1
#define BUTTON_ID_AUDIO		2
#define BUTTON_ID_CUSTOM	3
#define BUTTON_ID_EXIT		4
#define BUTTON_ID_120x40	5
#define BUTTON_ID_40x40		6


#ifdef USE_MINI_OSD_REGION  // Brian2.17, for 16M DRAM solution
#define GDI_REGION_WIDTH        624 //for 4-bit mode, muste be 8-byte (16-pixel)
#define GDI_REGION_HEIGHT       78L 
#define GDI_REGION_HEIGHT_PAL      78L
#else
#define GDI_REGION_WIDTH        616L //620L //640L
#define GDI_REGION_HEIGHT       440L //temp extend to 440 to let Thumbnail PAL mode has more space //420L //460L
#define GDI_REGION_HEIGHT_PAL       540L // Brian1.20, support enlarge PAL OSD
#endif


// thers is 5K DW for 2-bit SP OSD region
#define GDI_SP_OSD_REGION_WIDTH        616L // 608+4 or 608+8, for 2-bit SP OSD, the byte number must be 4N+1 or 4N+2
											// *** but each horizontal line still need to use 4-byte (16-pixel) alignemnt DRAM
											// *** in this case, the DRAM is (616+15)/16*16 ==> 624 pixel for each line
//#define GDI_SP_OSD_REGION_HEIGHT       130L // 130*624/4 ==> 20280 bytes ==> 4.95KDW
#define GDI_SP_OSD_REGION_HEIGHT       78L // 78*624/4 ==> 12168 bytes ==> 2.97KDW   // Alan2.21a, the buffer size is 3KDW in 16M solution




// Brian1.10
#ifdef USE_ONE_OSD_REGION
#define GDI_REGION_2_HEIGHT       180L
#else
#define GDI_REGION_2_HEIGHT       200L
#endif

#define GDI_REGION_3_WIDTH        256
#define GDI_REGION_3_HEIGHT       128L


//#define REGION_X_NTSC   176 //368 // Brian1.24, move 16 pixles right //336 //360
//#define REGION_Y_NTSC   72 //30 //30 //24, 28, 30, 32,33,34  44

//#define REGION_X_PAL   176 //392 // Brian1.24, move 16 pixles right //360
//#define REGION_Y_PAL   86

// Brian0.86, relative position to Main Video
#define REGION_X_NTSC  (MainVideoInfo.sHStart + 75) 
#define REGION_Y_NTSC  (MainVideoInfo.sVStart + 30) //30)

#define REGION_X_PAL   (MainVideoInfo.sHStart + 75)  //392 // Brian1.24, move 16 pixles right //360

#ifdef SUPPORT_CHAR_SUBPICTURE // Brian1.10
#define REGION_Y_PAL   (MainVideoInfo.sVStart + 40) //78) // Jeff, for DivX subtitle position
#else
#define REGION_Y_PAL   (MainVideoInfo.sVStart + 40) 
#endif


// The palette address in DRAM
// CT908 only, meaningless for CT909~
#define PAL_ADDR            0xA000L //// palette 0 address, will use 256 DW


// brian0.65
// Chuan0.85, Use the definition of CTKAV.H
#define OSD_FONT_TABLE_BASE_ADDRESS     (DS_OSD_FONTTABLE_ST) // 0x40433000 // reserve for JPEG usage //0x40400000 //0x401E6ED0


// Bit Mode
#define GDI_OSD_8B_MODE         (0)
#define GDI_OSD_4B_MODE         (1)
#define GDI_OSD_2B_MODE         (2)

// Region ID
#define GDI_SP_REGION_ID        2


typedef struct tagGDI_REGION_INFO	
{
	DWORD	wWidth;		    // region width
	DWORD	wHeight;		// region height
	BYTE	bColorMode;	    // color mode, 0: 4 color, 1: 16 color, 2: 256 color
	DWORD	dwTAddr;		// top field buffer (including header) address 
} GDI_REGION_INFO, * PGDI_REGION_INFO;

typedef struct tagGDI_PALETTE_INFO	
{
    
	BYTE	bStartNumber;	// the palette will be put starting at DWORD entry
	DWORD	dwPalAddr;	    // palette address in DRAM
	const DWORD	*pdwPalette;	// point to a palette data array
} GDI_PALETTE_INFO, * PGDI_PALETTE_INFO;

typedef struct tagGDI_IMAGE_INFO	
{
	BYTE	bWidth;	        // the image width
	BYTE	bHeight;	    // the image height
    BYTE	bColorMode;	    // color mode, 0: 4 color, 1: 16 color, 2: 256 color
	BYTE	*pbImageData;	// point to a image data array, top field data first then bottom field data
} GDI_IMAGE_INFO, * PGDI_IMAGE_INFO;

// ******************************
// Parameter structure definition
// ******************************
typedef struct tagPARM_SCROLL_BAR // Scroll bar parameter structure
{
    URECT  rect;
    WORD  wDisplay;
    WORD  wStartPoint;
}PARM_SCROLL_BAR, *PPARM_SCROLL_BAR;

typedef struct tagPARM_VOLUME_BAR // Volume bar parameter structure
{
    URECT  rect;
    WORD  wVolume;
    BYTE  bBarColor;
    BYTE  bBgColor;
    BYTE  bFrameColor;
    BYTE  bOriention;
}PARM_VOLUME_BAR, *PPARM_VOLUME_BAR;

typedef struct tagPARM_FRAME // Draw frame parameter structure
{
    URECT  rect;
    BYTE  bStyle;
    BYTE  bThickness;
    BYTE  bColorBright;
    BYTE  bColorDark;
    BYTE  bColorCenter;
}PARM_FRAME, *PPARM_FRAME;

typedef struct tagPARM_RECT // DrawRect, DrawLine, FillRect parameter structure
{
    URECT  rect;
    BYTE  bColor;   // if it is a shading operation, the this field means how many shading level in the bShadePtr[]
    BYTE    *bShadePtr;
}PARM_RECT, *PPARM_RECT;

typedef struct tagPARM_ARC // DrawArc, FillArc parameter structure
{
    WORD  wX;
    WORD  wY;
    BYTE  bColor;   // if it is a shading operation, the this field means how many shading level in the bShadePtr[]
    BYTE  bArc;
    BYTE    *bShadePtr;
}PARM_ARC, *PPARM_ARC;

typedef struct tagPARM_PIXEL // DrawPixel parameter structure
{
    WORD  wX;
    WORD  wY;
    BYTE  bColor;
}PARM_PIXEL, *PPARM_PIXEL;

typedef struct tagPARM_BITMAP_COPY // BitmapCopy parameter structure
{
    URECT  SrcRect;
    BYTE  bSrcRegion;
    WORD  wDestX;
    WORD  wDestY;
    BYTE  bDestRegion;
}PARM_BITMAP_COPY, *PPARM_BITMAP_COPY;

typedef struct tagPARM_BUTTON // BitmapCopy parameter structure
{
    URECT  rect;
    BYTE  bButtonId;
    BYTE  bStyle;       // noraml, disbaled, selected, with text
    //WORD  wTemp;        // Temp usage
}PARM_BUTTON, *PPARM_BUTTON;

typedef struct tagPARM_BMP // Load BMP parameter structure
{
    WORD    wX;         // the x-coordinate
    WORD    wY;         // the y-coordinate
    DWORD  *pdwData;    // pointer to the BMP data, the same format as SCR (not split)
}PARM_BMP, *PPARM_BMP;


typedef struct tagGDI_PARAMETER
{
    union{
        PARM_SCROLL_BAR ScrollBar;
        PARM_VOLUME_BAR VolumeBar;
        PARM_FRAME Frame;
        PARM_RECT RectParm;
        PARM_PIXEL PixelParm;
        PARM_BITMAP_COPY BitmapCopy;
        PARM_BUTTON Button;
        PARM_BMP    BMP;
    } u;

} GDI_PARAMETER, *PGDI_PARAMETER;

typedef struct tagPARM_STRING // DrawString parameter structure
{
    WORD  wX;
    WORD  wY;
    BYTE  bBackground;	// text background color: mapping to 00 of font data
    BYTE  bTextColor;	// text color: mapping to 01 of font data
	BYTE  bShadow1;		// text shadow 1 color: mapping to 10 of font data
	BYTE  bShadow2; 	// text shadow 2 color: mapping to 11 of font data
	BYTE  bColorKey;    // the transparent color: 0 disable color key, 1~255 the color key

}PARM_DRAW_STRING, *PPARM_DRAW_STRING;

typedef struct tagPARM_DRAW_BITMAP
{
    BYTE  bBitmapID;
    WORD  wDestX;
    WORD  wDestY;
    BYTE  bDestRegion;
}PARM_DRAW_BITMAP, *PPARM_DRAW_BITMAP;


extern void GDI_InitialRegion(BYTE bRegionId);
extern void GDI_ActivateRegion(BYTE bRegionId);
extern void GDI_DeactivateRegion(void);
extern void GDI_MoveRegion(BYTE bRegionId, SHORT sX, SHORT sY);
extern void GDI_SetMixRatio(BYTE bMixRatio);
//extern void GDI_DrawPixel(WORD wX, WORD wY, BYTE bColor);
//extern void GDI_LoadPalette(BYTE bPaletteId);
extern void GDI_LoadPalette(GDI_PALETTE_INFO *pPaletteInfo);
extern void GDI_SetTextColor (BYTE bFgBg, BYTE bColor);
extern void GDI_ChangePALEntry(BYTE bIndex, DWORD dwRGB, BYTE bMixEnable);
extern void GDI_ClearRegion(BYTE bRegionId);
extern void GDI_BitmapCopy(PARM_BITMAP_COPY *pParm);
extern void GDI_DrawButton(PARM_BUTTON *pParm);

extern void GDI_SetUnicodeTable(WORD* pCurUnicodeTable);
extern void GDI_DrawString(BYTE bRegionId, PARM_DRAW_STRING *pParm, WORD *pwString);
extern void GDI_DrawStringByUnicode(BYTE bRegionId, PARM_DRAW_STRING *pParm, WORD *pwString);
#ifdef SUPPORT_STB
extern void GDI_DrawFont(DWORD dwFontAddr, DWORD dwColor, WORD wWidth, WORD wHeight, WORD wX, WORD wY);
extern void GDI_DrawScaleString(BYTE bRegionId, PARM_DRAW_STRING *pParm, WORD *pwString, BOOL bScaleW, BOOL bScaleH);
#endif
extern void GDI_FillRect(BYTE bRegionId, PARM_RECT *pParm);
extern void GDI_FillRoundRect(BYTE bRegionId, BYTE bArc, PARM_RECT *pParm);
extern void GDI_FillRoundRectTop(BYTE bRegionId, BYTE bArc, PARM_RECT *pParm);
extern void GDI_FillRoundRectBottom(BYTE bRegionId, BYTE bArc, PARM_RECT *pParm);
extern void GDI_DrawRoundRect(BYTE bRegionId, BYTE bArcStart, BYTE bArcEnd, PARM_RECT *pParm);

extern void GDI_DrawFrame(BYTE bRegionId, PARM_FRAME *pParm);
extern void GDI_DrawVolumeBar(BYTE bRegionId, PARM_VOLUME_BAR *pParm);
extern void GDI_DrawScrollBar(BYTE bRegionId, PARM_SCROLL_BAR *pParm);
extern void	GDI_DrawBitmapBySW(PARM_DRAW_BITMAP *pParm, const DWORD ** BMPAryptr);
extern void GDI_DrawZipBitmap(PARM_DRAW_BITMAP *pParm, const BYTE **BMPAry, PBYTE pbDest, DWORD dwZIPWM);
extern void	GDI_DrawBitmap(PARM_DRAW_BITMAP *pParm, DWORD ** BMPAryptr);
extern BYTE GDI_GetFontWidth(void);
//Aron,2.77-909P, Port Damon's code to files to support Unicode DIR/FILE name & external subtitle.
#ifdef SUPPORT_UNICODE_CODING
extern WORD GDI_GetUniFontHeightWidth(WORD wChar);
extern BYTE GDI_GetUniFontWidth(WORD wChar);
extern WORD GDI_GetUniStringWidth(WORD *pString);
#endif
extern WORD GDI_GetStringWidth(WORD *pString); //CoCo.CT909
extern BYTE GDI_GetStringHeight(WORD *pString); //CoCo.CT909



extern void GDI_CopyCodeString(WORD *pw);  // should be put in common bank
extern void GDI_OutputText(WORD *pw); // should be put in common bank

extern void GDI_Initial(void);
extern void GDI_SetGDIPalEntry(void);
extern void GDI_LoadBMPResource(void);
extern void GDI_ShowSpeaker(BYTE bLoc);

extern void GDI_DrawString_GXA(void);
extern void GDI_FillRect_GXA(PARM_RECT *pParm);
extern void GDI_BitmapCopy_GXA(PARM_BITMAP_COPY *pParm);
extern void GDI_NumberToString(WORD wNum);
extern void GDI_NumberToStringHex(WORD wNum); // Chuan1.00-2
extern WORD GDI_GetCodeStringWidth(WORD *pw);
// wyc0.50-909

//extern void GDI_EnablePalette(void); //--Brian1.15, reduce code size
extern  BYTE GDI_GetCharWidth(WORD wChar);
// Brian0.85
extern void    GDI_FBDrawDot(DWORD dwFrameIdx, DWORD dwX, DWORD dwY, BYTE bColorY, BYTE bColorU, BYTE bColorV);
extern void GDI_FBFillRect(DWORD dwFrameIdx, URECT *pRect, DWORD dwRGB);
extern void GDI_FBDrawRect(DWORD dwFrameIdx, URECT *pRect, BYTE bThickness, DWORD dwRGB);
//extern void GDI_LoadBMP(BYTE bBMPGroup);
extern void GDI_LoadBMP(DWORD **aPtr, DWORD dwMaxNum);
extern void GDI_WaitPaletteComplete(void); // Brian0.95
extern void GDI_ChangeRegionHeight(BYTE bRegionId);
#ifdef SUPPORT_CT909S
extern WORD GDI_GetStringWidth_2B(WORD *pString);
#endif
extern void GDI_SetFontTableSize(BYTE bFontWidth, WORD wFontCapacity);
extern void GDI_SetFontTableAddress(DWORD dwAddress);
extern DWORD GDI_GetFontTableAddress(VOID);

extern void GDI_SetFontWidthInTable(BYTE bFontWidth);
extern void GDI_SetFontHeightInTable(BYTE bFontHeight);
extern void GDI_SetFontNumberInTable(WORD wFontNumber);
extern void GDI_ConfigRegionInfo(BYTE bRegionId, GDI_REGION_INFO *regionInfo);
extern void GDI_ConfigRegionAddress(BYTE bRegionId, DWORD dwTAddr);


// Brian1.25, move to OSD.h
//extern void OSD_LoadFontTable(void);    // Chuan1.20, Unzip Font Table
//#else
//extern void OSD_LoadFontTableHDW(void);
//extern void OSD_LoadFontTableLDW(void);
//#endif

#ifndef GDI_MAIN_FILE1

extern GDI_REGION_INFO __RegionList[NUMBER_OF_REGION];
extern WORD    __wGDIString[GDI_STRING_LENGTH+1];
extern BYTE __bCurrentRegionId;
extern GDI_PARAMETER __GDIParm;
//extern BYTE    __bColorMode; // Brian1.08 //--Brian1.15, reduce code size.
#ifndef GDI_WITHOUT_OS
extern MUTEX_T     __mutexGDIIdle;
#endif
// Brian0.1, these two should be variable
//extern DWORD     DS_OSD_FRAME;   // 620x420x8
extern DWORD     __dwGDIFontTableBase; // 128 Font

//Brian, support GDI library
extern GDI_PALETTE_INFO    __PaletteInfo_Transparent;

#endif


// The DRAM base address for storing BMPs
// Chuan0.85, Use the definition of CTKAV.H
//#define GDI_BMP_BASE_ADDRESS    (DS_OSD_BMPDATA_ST)  // (OSD_FONT_TABLE_BASE_ADDRESS+0x13000)


// *** palette entry for GDI ***	usage 4 entries
#define GDI_ENTRY_START						GDI_LOW_LEVEL_FUNCTION_PALETTE_INDEX_START

#define GDI_ENTRY_SCROLL_BAR_COLOR_BRIGHT		(GDI_ENTRY_START)
#define GDI_ENTRY_SCROLL_BAR_COLOR_DARK			(GDI_ENTRY_SCROLL_BAR_COLOR_BRIGHT+1)
#define GDI_ENTRY_SCROLL_BAR_COLOR_CENTER       (GDI_ENTRY_SCROLL_BAR_COLOR_DARK+1)
#define GDI_ENTRY_SCROLL_BAR_COLOR_BG			(GDI_ENTRY_SCROLL_BAR_COLOR_CENTER+1)


#define GDI_ENTRY_LAST					(GDI_ENTRY_SCROLL_BAR_COLOR_BG)


#define GDI_VALUE_SCROLL_BAR_COLOR_BRIGHT		0xFFFFFF
#define GDI_VALUE_SCROLL_BAR_COLOR_DARK			0x202020 //0x606060
#define GDI_VALUE_SCROLL_BAR_COLOR_CENTER		0xC0C0C0
#define GDI_VALUE_SCROLL_BAR_COLOR_BG			0xD8D8D8



#endif  // __GDI_H__
