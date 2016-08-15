#ifndef __TOOLBAR_H__
#define __TOOLBAR_H__

#define MAX_SUPPORT_TOOLBAR_HANDLE_NUM      5

//Define the attribute of the icon
#define ICON_TYPE_POPMENU                   0
#define ICON_TYPE_BUTTON                    1

//define the type of the cursor
#define ICON_CURSOR_TYPE_NORMAL             0
#define ICON_CURSOR_TYPE_HIGHLIGHT          1
#define ICON_CURSOR_TYPE_PRESSED            2

//Define the return action of the icon
#define ICON_ACTOIN_NONE                    0
#define ICON_ACTION_ACTIVATE_MENU           1
#define ICON_ACTION_PROCESS_ICON            2

//Define the action of the icon               
#define ICON_ACTOIN_PREVIOUS                0
#define ICON_ACTOIN_NEXT                    1   
#define ICON_ACTOIN_GO_TO_FIRST             2  
#define ICON_ACTOIN_GO_TO_LAST              3

//Define the style of the toolbar
#define TOOLBAR_STYLE_HORIZONTAL            0
#define TOOLBAR_STYLE_VERTICAL              1

//Define the framw for the tool bar
#define TOOLBAR_FRAME_THICKNESS            2

//Define the frame for the icons
#define TOOLBAR_ICON_FRAME_THICKNESS        2

#define TOOLBAR_ICON_ICON_DISTANCE          6
#define TOOLBAR_ICON_BACKGROUND_DISTANCE    6

//define invlide MENU HANDLE
#define TOOLBAR_HANDLE_INVALID              0xFF

#define TOOLBAR_HANDLE_FREE                 0xFE

//define the handle of the menu
typedef BYTE HTOOLBAR;

typedef struct tagTOOLBAR_CURSOR
{
    BYTE bPos;
    BYTE bType;
} TOOLBAR_CURSOR, *PTOOLBAR_CURSOR;

#ifdef SUPPORT_TEXT_TOOLBAR
typedef struct tagICON
{
    BYTE bIconID;
    BYTE bIconType;
	WORD *pwString;
    void (*Initial_Func_Ptr)(BYTE);
    void (*Exit_Func_Ptr)(void);
} ICON, *PICON;

typedef struct tagTOOLBAR
{
    BYTE bTotalIcon;
    BYTE bStyle;
    WORD wStartX;
    WORD wStartY;
    BYTE bIconWidth;
    BYTE bIconHeight;
	BYTE bIconNormalTextColor;
	BYTE bIconHighlightTextColor;
	BYTE bIconPressedTextColor;
    ICON *pIcon;   
} TOOLBAR, *PTOOLBAR;

#else
typedef struct tagICON
{
    BYTE bIconID;
    BYTE bBGPaletteIndex;
    BYTE bIconType;
    void (*Initial_Func_Ptr)(BYTE);
    void (*Exit_Func_Ptr)(void);
} ICON, *PICON;

typedef struct tagTOOLBAR
{
    BYTE bTotalIcon;
    BYTE bStyle;
    WORD wStartX;
    WORD wStartY;
    BYTE bIconWidth;
    BYTE bIconHeight;
    ICON *pIcon;   
    DWORD **BMPArrayptr;
    DWORD *pPalette;
} TOOLBAR, *PTOOLBAR;

#endif //SUPPORT_TEXT_ICON

//define the palette entry      
#define TOOLBAR_PALETTE_ENTRY_BEGIN                             GDI_TOOLBAR_PALETTE_INDEX_START
#define TOOLBAR_PALETTE_ENTRY_GENERAL_BACKGROUND                TOOLBAR_PALETTE_ENTRY_BEGIN
#define TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT               (TOOLBAR_PALETTE_ENTRY_BEGIN+1)                 
#define TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_DARK                 (TOOLBAR_PALETTE_ENTRY_BEGIN+2)                   
#define TOOLBAR_PALETTE_ENTRY_SEPARATE_FRAME_CENTER             (TOOLBAR_PALETTE_ENTRY_BEGIN+3)
#define TOOLBAR_PALETTE_ENTRY_BIG_BACKGROUND                    (TOOLBAR_PALETTE_ENTRY_BEGIN+4) 

#ifdef SUPPORT_TEXT_TOOLBAR
#define TOOLBAR_PALETTE_ENTRY_ICON_NORMAL_BACKGROUND            (TOOLBAR_PALETTE_ENTRY_BEGIN+5) 
#define TOOLBAR_PALETTE_ENTRY_ICON_HIGHLIGHT_BACKGROUND         (TOOLBAR_PALETTE_ENTRY_BEGIN+6) 
#define TOOLBAR_PALETTE_ENTRY_ICON_PRESSED_BACKGROUND           (TOOLBAR_PALETTE_ENTRY_BEGIN+7) 
#endif //SUPPORT_TEXT_TOOLBAR

//define the palette color
#define TOOLBAR_PALETTE_COLOR_GENERAL_BACKGROUND                0xC0C0C0
#define TOOLBAR_PALETTE_COLOR_NORMAL_FRAME_BRIGHT               0xFFFFFF                 
#define TOOLBAR_PALETTE_COLOR_NORMAL_FRAME_DARK                 0x202020                   
#define TOOLBAR_PALETTE_COLOR_SEPARATE_FRAME_CENTER             0xC0C0C0 
#define TOOLBAR_PALETTE_COLOR_BIG_BACKGROUND                    0xC8C8C8

//define the color for icon's state.
#define TOOLBAR_PALETTE_COLOR_ICON_NORMAL_BACKGROUND            0xC0C0C0
#define TOOLBAR_PALETTE_COLOR_ICON_HIGHLIGHT_BACKGROUND         0xFFD900
#define TOOLBAR_PALETTE_COLOR_ICON_PRESSED_BACKGROUND           0xA8A8A8

//Export APIs
HTOOLBAR TOOLBAR_CreateToolBar(TOOLBAR *pToolBar);
void TOOLBAR_ShowToolBar(HTOOLBAR hToolBar, BYTE bToolBarBackgroundColor);
WORD TOOLBAR_ProcessKey(HTOOLBAR hToolBar, BYTE bKey);
void TOOLBAR_UpdateCursor(HTOOLBAR hToolBar, BYTE bType);
void TOOLBAR_DeleteToolBar(HTOOLBAR hToolBar, BYTE bClearBackground);

//Internal APIs
void _TOOLBAR_InitializeGDI(void);
HTOOLBAR _TOOLBAR_GetToolBarHandle(void);
void _TOOLBAR_IconAction(HTOOLBAR hToolBar, BYTE bAction);
void _TOOLBAR_DrawIconFrame(HTOOLBAR hToolBar, BYTE bType);
void _TOOLBAR_ClearBackground(HTOOLBAR hToolBar);


#endif //__TOOLBAR_H__

