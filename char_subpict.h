
#ifndef __CHAR_SP_H
#define __CHAR_SP_H

//Alan1.10, for char-based subpicture
#ifdef SUPPORT_CHAR_SUBPICTURE

#define CHAR_SP_INPUT_BUFFER         DS_CHAR_BASED_SP_INBUF_ST 
#define CHAR_SP_INPUT_SIZE           (DS_CHAR_BASED_SP_INBUF_END - DS_CHAR_BASED_SP_INBUF_ST)
#define CHAR_SP_BUFFER               DS_CHAR_BASED_SP_OUTBUF_ST 
#define CHAR_SP_BUFFER_SIZE          (DS_CHAR_BASED_SP_OUTBUF_END - DS_CHAR_BASED_SP_OUTBUF_ST)

//define the region for char-based subpicture 

#ifdef EX_SP_USE_SPU_BUFFER          // Use SP buffer
#define CHAR_SP_LINE_HEIGHT          OSD_FONT_HEIGHT
#define CHAR_SP_MAX_LINE_NUM         (GDI_SP_OSD_REGION_HEIGHT / CHAR_SP_LINE_HEIGHT)
#define CHAR_SP_REG_WIDTH            560 
#define CHAR_SP_REG_HEIGHT           (CHAR_SP_MAX_LINE_NUM * CHAR_SP_LINE_HEIGHT)

#define CHAR_SP_REG_LEFT             0
#define CHAR_SP_REG_RIGHT            (CHAR_SP_REG_LEFT + CHAR_SP_REG_WIDTH)
#define CHAR_SP_REG_BOTTOM           (VSIZE_NTSC - 50) 
#define CHAR_SP_REG_TOP              (CHAR_SP_REG_BOTTOM - CHAR_SP_REG_HEIGHT)

#define CHAR_SP_REG_BOTTOM_PAL       (VSIZE_PAL - 50) 
#define CHAR_SP_REG_TOP_PAL          (CHAR_SP_REG_BOTTOM_PAL - CHAR_SP_REG_HEIGHT)

#else                                // Use OSD buffer

#define CHAR_SP_LINE_HEIGHT          OSD_FONT_HEIGHT
#define CHAR_SP_MAX_LINE_NUM         3 
#define CHAR_SP_REG_WIDTH            616 
#define CHAR_SP_REG_HEIGHT           (CHAR_SP_MAX_LINE_NUM * CHAR_SP_LINE_HEIGHT)

#define CHAR_SP_REG_LEFT             0
#define CHAR_SP_REG_RIGHT            (CHAR_SP_REG_LEFT + CHAR_SP_REG_WIDTH)
#define CHAR_SP_REG_BOTTOM           OSDND_DISPLAY_BOTTOM_START_V 
#define CHAR_SP_REG_TOP              (CHAR_SP_REG_BOTTOM - CHAR_SP_REG_HEIGHT)

#define CHAR_SP_REG_BOTTOM_PAL       OSDND_DISPLAY_BOTTOM_START_V_PAL 
#define CHAR_SP_REG_TOP_PAL          (CHAR_SP_REG_BOTTOM_PAL - CHAR_SP_REG_HEIGHT)
#endif // #ifdef EX_SP_USE_SPU_BUFFER


void    CHAR_SP_Initial(void);
void    CHAR_SP_AdjustRegion(void);
void    CHAR_SP_SetOutputBuffer(DWORD dwBufferAddress, DWORD dwBufferSize);
void    CHAR_SP_ParseSPFile(void);
void    CHAR_SP_Trigger(void);
void    CHAR_SP_Reset(void);
void    CHAR_SP_ReplaceFont(void);
void    CHAR_SP_ReloadFromTime(void);
void    CHAR_SP_ClearRegion(void);

#define CHAR_SP_ENTRY_COLOR_FG       GDI_DYNAMIC_ND_PALETTE_INDEX_START
#define CHAR_SP_ENTRY_COLOR_BG       (CHAR_SP_ENTRY_COLOR_FG + 1)

#define CHAR_SP_VALUE_COLOR_FG       0xFFFFFF
#define CHAR_SP_VALUE_COLOR_BG       0x555555

extern DWORD __dwParResult_CharSP;

#endif  // SUPPORT_CHAR_SUBPICTURE

#endif  //__CHAR_SP_H

