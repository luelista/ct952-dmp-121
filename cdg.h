//**************************************************************************
//         Copyright (c) 2005, Cheertek Inc . All rights reserved.
//         F200, all right reserved.

//	Product	: WinDVD Firmware

//	Date	: 2005.5.20
//	Author	: Cheertek (F200 CoCo Chiang)
//	Purpose	: Provide the UI for thumbnail.
//	Sources	: Thumb.c
//  History : The original author is LJY.
//***************************************************************************

#ifndef __CDG_H__
#define __CDG_H__

#define DONT_DO_COMMAND_IF_DATA_ERROR

//define CDG state.
#define CDG_STATE_DISC_DETECTED       1
#define CDG_STATE_RESET               2
#define CDG_STATE_BITSTREAM_INPUT     3
#define CDG_STATE_AV_SYNC             4

#define CDG_RGB_R_MASK			0x3C
#define CDG_RGB_GH_MASK			0x03
#define CDG_RGB_GL_MASK			0x30
#define CDG_RGB_B_MASK			0x0F

//TV-Graphics mode
#define LINEAR_GRAPHICS_MODE    8
#define TV_GRAPHICS_MODE        9
//TV-Graphics mode instructions
#define PRESET_MEMORY			1
#define PRESET_BORDER			2
#define WRITE_FONT				6
#define SCROLL_WITH_PRESET		20
#define SCROLL_WITH_COPY		24
#define DEFINE_COLOR_TRANS		28	
#define LOAD_CLUT0_7			30 //1E
#define LOAD_CLUT8_15			31 //1F
#define XOR_FONT				38 //26

#define WRITTEN_FONT			1
#define NULL_FONT				0

//picture size
#define CDG_DISPLAY_PICTURE_WIDTH                   720
#define CDG_DISPLAY_PICTURE_HEIGHT                  480

//Y, UV frame buffer start address
#define Y_MAXSIZE_CDG                               (CDG_DISPLAY_PICTURE_WIDTH*CDG_DISPLAY_PICTURE_HEIGHT)
#define CDG_Y_FRAME_START_ADDR                      (DS_FRAMEBUF_ST_CDG) //720pixels/4 = 180 DW per line
// #define CDG_UV_FRAME_START_ADDR                     (DS_FRAMEBUF_ST+(Y_MAXSIZE_CB00*8))
#define CDG_UV_FRAME_START_ADDR                     (DS_FRAMEBUF_ST_CDG+Y_MAXSIZE_CDG)
#define CDG_SCREEN_WIDTH		                    50
#define CDG_SCREEN_HEIGHT                           18
#define CDG_DATA_FIELD_SIZE		                    72 // 6x12 bytes

//define the visible row/colum index

// LLY2.21, base on Chuan's suggestion to config CDG used buffer.
#define CDG_DISPLAY_FRAME_SIZE                      0x7EC00  // 720*480*1.5= 0x7e900, need do 0x400 alignment, so choose 0x7EC00
#define CDG_SCREEN_BUFFER_START                     (DS_FRAMEBUF_ST_CDG + CDG_DISPLAY_FRAME_SIZE)
#define CDG_SCREEN_FONT_BUFFER_SIZE		            (CDG_SCREEN_WIDTH*CDG_SCREEN_HEIGHT*CDG_DATA_FIELD_SIZE)

//Clip size
#define CDG_CLIP_XSTART                             0
#define CDG_CLIP_YSTART                             0
#define CDG_CLIP_WIDTH                              720
#define CDG_CLIP_HEIGHT                             480

//define the symbols of a pack.
#define CDG_NUM_OF_PACK_IN_PACKET                   4

//define the number of symbols in a pack.
#define CDG_NUM_OF_SYMBOL_IN_PACK                   24

//define the width/height of the font
#define CDG_FONT_WIDTH                              6
#define CDG_FONT_HEIGHT                             12

//define MPEG frame buffer (4:2:0) format relative attributes
#define CDG_MPEG_FRAME_BUFFER_LINES_IN_STRIP            16

#define CDG_MPEG_FRAME_BUFFER_PIXELS_IN_ONE_ADDRESS     4

//To center the fonts in a frame buffer
#define CDG_TUNE_MPEG_FRAME_BUFFER_START_X          64
#define CDG_TUNE_MPEG_FRAME_BUFFER_START_Y          32

void CDG_Reset(void);
void CDG_WaitBitstream(void);
void CDG_Trigger(void);
BYTE CDG_DetectCDG(BYTE *pPtr);
void CDG_Initial(void);
void CDG_ConfigScreenBuf(DWORD dwYFrameStartAddress, DWORD dwScreenBufAddress);
void _CDG_ProcessCMD(BYTE *pPtr);
void _CDG_PresetBorder(void);
BYTE _CDG_LoadCLUT(BYTE *pPack);
void _CDG_PresetMemory(BYTE *pPack);
void _CDG_WriteFont(BYTE *pPack);
void _CDG_XORFont(BYTE *pPack);
void _CDG_XORFont(BYTE *pPack);
void _CDG_FillFont(BYTE bRow, BYTE bCol);
BYTE _CDG_VideoDecode(void);

typedef struct  tagCDGPALETTE
{
    BYTE    bY;
    BYTE    bU;
    BYTE    bV;
} CDGPALETTE, *PCDGPALETTE;


#endif //__CDG_H__
