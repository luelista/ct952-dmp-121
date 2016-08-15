
#ifndef __OSDPRGM_H__
#define __OSDPRGM_H__

////////////////          Public variables          /////////////////////////

BYTE    __bProgramTitle;
WORD    __wProgramChapter;




////////////////          Public functions         /////////////////////////

void    OSDPRGM_Exit(BYTE bExitMode);
BYTE    OSDPRGM_ProcessKey(void);
BYTE    OSDPRGM_Recover(BYTE bRecoverRegion);
void    OSDPRGM_ConfigOSDRegion(void);
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
void    OSDPRGM_EnterByPrestop(void);
void    OSDPRGM_ExitByResume(void);
#endif


//////////////           Palette & Color          //////////////////////////

#define OSDPRGM_ENTRY_START            GDI_GENERAL_UI_PALETTE_INDEX_START 

#define OSDPRGM_ENTRY_COLOR_TOP_BANNER         (OSDPRGM_ENTRY_START)
#define OSDPRGM_ENTRY_COLOR_TOP_BANNER_TEXT    (OSDPRGM_ENTRY_COLOR_TOP_BANNER+1)
#define OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER      (OSDPRGM_ENTRY_COLOR_TOP_BANNER_TEXT+1)
#define OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER_TEXT (OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER+1)
#define OSDPRGM_ENTRY_MAIN_COLOR_BG            (OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER_TEXT+1)
#define OSDPRGM_ENTRY_COLOR_TEXT1      (OSDPRGM_ENTRY_MAIN_COLOR_BG+1)
#define OSDPRGM_ENTRY_COLOR_TEXT2      (OSDPRGM_ENTRY_COLOR_TEXT1+1)
#define OSDPRGM_ENTRY_COLOR_TEXT_BG    (OSDPRGM_ENTRY_COLOR_TEXT2+1) 
#define OSDPRGM_ENTRY_COLOR_WHITE      (OSDPRGM_ENTRY_COLOR_TEXT_BG+1)
#define OSDPRGM_ENTRY_COLOR_HILIGHT            (OSDPRGM_ENTRY_COLOR_WHITE+1)
#define OSDPRGM_ENTRY_FRAME_BRIGHT     (OSDPRGM_ENTRY_COLOR_HILIGHT+1)
#define OSDPRGM_ENTRY_FRAME_DARK       (OSDPRGM_ENTRY_FRAME_BRIGHT+1)
#define OSDPRGM_ENTRY_FRAME_CENTER     (OSDPRGM_ENTRY_FRAME_DARK+1)
#define OSDPRGM_ENTRY_TEXT_DISABLED    (OSDPRGM_ENTRY_FRAME_CENTER+1)
//#define OSDPRGM_ENTRY_COLOR_RED                (OSDPRGM_ENTRY_COLOR_WHITE+1)
//#define OSDPRGM_ENTRY_COLOR_ORANGE             (OSDPRGM_ENTRY_COLOR_RED+1)




#define OSDPRGM_VALUE_COLOR_TOP_BANNER1         0x0000FF // need mix enable
#define OSDPRGM_VALUE_COLOR_TOP_BANNER_TEXT1    0xFFFFFF // need mix enable
#define OSDPRGM_VALUE_MAIN_COLOR_BG1            0x00AAFF // need mix enable
#define OSDPRGM_VALUE_COLOR_BOTTOM_BANNER1      0x0000FF // need mix enable
#define OSDPRGM_VALUE_COLOR_BOTTOM_BANNER_TEXT1 0xFFFFFF // need mix enable

#define OSDPRGM_VALUE_COLOR_TOP_BANNER2         0xEE9682 // need mix enable
#define OSDPRGM_VALUE_COLOR_TOP_BANNER_TEXT2    0x0000FF // need mix enable
#define OSDPRGM_VALUE_MAIN_COLOR_BG2            0x8ED3AB // need mix enable
#define OSDPRGM_VALUE_COLOR_BOTTOM_BANNER2      0x5BBEFD // need mix enable
#define OSDPRGM_VALUE_COLOR_BOTTOM_BANNER_TEXT2 0xFFFFFF // need mix enable

#define OSDPRGM_VALUE_COLOR_TOP_BANNER3         0xFDE85E // need mix enable
#define OSDPRGM_VALUE_COLOR_TOP_BANNER_TEXT3    0x0000FF // need mix enable
#define OSDPRGM_VALUE_MAIN_COLOR_BG3            0xA3BBCB // need mix enable
#define OSDPRGM_VALUE_COLOR_BOTTOM_BANNER3      0xC694F0 // need mix enable
#define OSDPRGM_VALUE_COLOR_BOTTOM_BANNER_TEXT3 0xFFFFFF // need mix enable

#define OSDPRGM_VALUE_COLOR_TOP_BANNER4         0x89DAF8 // need mix enable
#define OSDPRGM_VALUE_COLOR_TOP_BANNER_TEXT4    0x0000FF // need mix enable
#define OSDPRGM_VALUE_MAIN_COLOR_BG4            0xFFEE4C // need mix enable
#define OSDPRGM_VALUE_COLOR_BOTTOM_BANNER4      0xF4A695 // need mix enable
#define OSDPRGM_VALUE_COLOR_BOTTOM_BANNER_TEXT4 0xFFFFFF // need mix enable


#define OSDPRGM_VALUE_COLOR_BLACK      0x000000 // need mix enable
#define OSDPRGM_VALUE_COLOR_WHITE      0xFEFEFE // need mix enable
#define OSDPRGM_VALUE_COLOR_HILIGHT    0xFFFF00
#define OSDPRGM_VALUE_FRAME_BRIGHT     0xFFFFFF
#define OSDPRGM_VALUE_FRAME_DARK       0x888888
#define OSDPRGM_VALUE_FRAME_CENTER     0xC0C0C0
#define OSDPRGM_VALUE_TEXT_DISABLED    0x888888
//#define OSDPRGM_VALUE_COLOR_RED        0xAA0000 // need mix enable
//#define OSDPRGM_VALUE_COLOR_ORANGE     0xAA0000 // need mix enable


#endif // __OSDPRGM_H__


