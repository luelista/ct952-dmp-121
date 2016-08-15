#ifndef __MENU_H__
#define __MENU_H__

#include "gdi.h"

#define MAX_SUPPORT_MENU_HANDLE_NUM     5

//define the height of the menu item
#define MENU_ITEM_HEIGHT                30

//define the height of the ergion to show "up" and "down" mark.
#define MENU_UP_DOWN_ITEM_HEIGHT        20 //16

//Define the thickness of the separated frame
#define MENU_SEPARATE_FRAME_THICKNESS   4

//Define the thickness of the menu main frame
#define MENU_FRAME_THICKNESS            2

//define the distance betweeen the menu and the string
#define MENU_STRING_DISTANCE_H          16
#define MENU_STRING_DISTANCE_V          0

//define the distance between the menu and sub-menu
#define MENU_SUBMENU_DISTANCE           20

//define the distance between check symbol and menu
#define MENU_CHECK_SYMBOL_DISTANCE_H    8

//define the distance between sub-menu symbol and menu
#define MENU_SUBMENU_SYMBOL_DISTANCE_H  16

//Define menu attributes. Enable or disable?
#define MENU_ITEM_STATE_DISABLE         0
#define MENU_ITEM_STATE_ENABLE          1

//Define no checked item
#define MENU_ITEM_NO_CHECKED_ITEM       0xFF

//Define the optimized menu width
#define MENU_WIDTH_OPTIMIZED            0xFFFF

//Define the optimized menu starting position
#define MENU_START_X_OPTIMIZED          0xFFFF
#define MENU_START_Y_OPTIMIZED          0xFFFF

//Define the menu return status
#define MENU_ACTION_NONE                    0xFFFF  
#define MENU_ACTION_EXCEED_UP_MAIN_MENU     0xFFF0
#define MENU_ACTION_EXCEED_DOWN_MAIN_MENU   0xFFF1
#define MENU_ACTION_EXCEED_UP_SUB_MENU      0xFFF2
#define MENU_ACTION_EXCEED_DOWN_SUB_MENU    0xFFF3
#define MENU_ACTION_UP_MAIN_MENU            0xFFF4
#define MENU_ACTION_DOWN_MAIN_MENU          0xFFF5
#define MENU_ACTION_UP_SUB_MENU             0xFFF6
#define MENU_ACTION_DOWN_SUB_MENU           0xFFF7

//Define the return status for XXX_ProcessKey (XXX means each main menu)
#define MENU_ACTION_DONE                0
#define MENU_ACTION_EXIT                1
#define MENU_ACTION_PROCESS_MENU_ITEM   2
#define MENU_ACTION_EXIT_ALL            3
#define MENU_ACTION_PROCESS_NUN_ITEM    0x80000000

//Define the type of menu
#define MENU_TYPE_GENERAL               0
#define MENU_TYPE_NUM                   1

//Define the mode of the menu
#define MENU_MAIN_MENU                  0
#define MENU_SUB_MENU                   1

//Define the alignment of the menu item
#define MENU_ITEM_ALIGNMENT_LEFT        0
#define MENU_ITEM_ALIGNMENT_CENTER      1
#define MENU_ITEM_ALIGNMENT_RIGHT       2

//define invlide MENU HANDLE
#define MENU_HANDLE_INVALID             0xFF

#define MENU_HANDLE_FREE                0xFE

#define CURSOR_POS_INVALID              0xFF

//Define the style of the cursor
#define CURSOR_STYLE_NORMAL             0
#define CURSOR_STYLE_GRAYED             1
#define CURSOR_STYLE_CLEAR              2

//Define the type of the separate frame
#define MENU_UP_SEPARATE_FRAME          0
#define MENU_DOWN_SEPARATE_FRAME        1

//Define the type of arrow
#define MENU_ARROW_TYPE_UP              0
#define MENU_ARROW_TYPE_DOWN            1

//define which item of the menu
#define MENU_FIRST_ITEM                 0
#define MENU_LAST_ITEM                  1

//define the handle of the menu
typedef BYTE HMENU;

// The multiple language string structure for menu item string
//#include "multistr.h"
/* alan.stb, for compile
typedef struct tagMULTI_STRING
{
    WORD *pwStr[OSD_LANGUAGE_NUMBER];
} MULTI_STRING, *PMULTI_STRING;
*/

// The menu item structure
typedef struct tagMENU_ITEM
{
    WORD *pwItemText;
    void *pSubMenu; //point to sub-menu. NULL==> no sub-menu.
    BYTE bAttribute; //enable/disable
    BYTE bID;   
} MENU_ITEM, *PMENU_ITEM;

// The menu structure
typedef struct tagMENU
{
    BYTE bMenuType;             //MENU_TYPE_GENERAL/MENU_TYPE_NUM
    BYTE bTotalItem;
    WORD wStartX;               //MENU_START_X_OPTIMIZED==>This is a sub-menu. Use the optimized distance with the main menu.
    WORD wStartY;               //MENU_START_Y_OPTIMIZED==>This is a sub-menu. Use the optimized vertical position with the main menu.
    WORD wWidth;                //MENU_WIDTH_OPTIMIZED==>Fit the max. width automatically.
    BYTE bAlignment;            //MENU_ITEM_ALIGNMENT_LEFT/MENU_ITEM_ALIGNMENT_CENTER/MENU_ITEM_ALIGNMENT_RIGHT
    MENU_ITEM *pMenuItem;
    WORD wStartNum;             //When menu type is MENU_TYPE_NUM, you can assign the starting number in the menu.
    BYTE bDefaultCursorPos;
    BYTE bCheckMenuItemPos;     //MENU_ITEM_NO_CHECKED_ITEM==>No check item
} MENU, *PMENU;

//Define the cursor
typedef struct tagCURSOR
{
    BYTE bMainMenuIndex;
    BYTE bSubMenuIndex;
    BYTE bPos;                  //Menu or sub-menu
} CURSOR, *PCURSOR;

//Define the attribute of the menu display
typedef struct tagMENU_DISPLAY
{
    BYTE bUpMark;
    BYTE bDownMark;
    BYTE bStartDrawItem;
    BYTE bMaxVisibleItem;
} MENU_DISPLAY, *PMENU_DISPLAY;

//define the palette entry
#define MENU_PALETTE_ENTRY_BEGIN                            GDI_MENU_COMPONENT_PALETTE_INDEX_START
#define MENU_PALETTE_ENTRY_GENERAL_BACKGROUND               MENU_PALETTE_ENTRY_BEGIN
#define MENU_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT              (MENU_PALETTE_ENTRY_BEGIN+1)
#define MENU_PALETTE_ENTRY_NORMAL_FRAME_DARK                (MENU_PALETTE_ENTRY_BEGIN+2)
#define MENU_PALETTE_ENTRY_NORMAL_FRAME_CENTER              (MENU_PALETTE_ENTRY_BEGIN+3)
#define MENU_PALETTE_ENTRY_HIGHLIGHT                        (MENU_PALETTE_ENTRY_BEGIN+4)
#define MENU_PALETTE_ENTRY_ITEM_TEXT                        (MENU_PALETTE_ENTRY_BEGIN+5)
#define MENU_PALETTE_ENTRY_CURSOR_GRAYED                    (MENU_PALETTE_ENTRY_BEGIN+6)
#define MENU_PALETTE_ENTRY_ITEM_GRAYED                      (MENU_PALETTE_ENTRY_BEGIN+7)

//define the palette color
#define MENU_PALETTE_COLOR_GENERAL_BACKGROUND               0xC0C0C0
#define MENU_PALETTE_COLOR_NORMAL_FRAME_BRIGHT              0xFFFFFF                 
#define MENU_PALETTE_COLOR_NORMAL_FRAME_DARK                0x202020                   
#define MENU_PALETTE_COLOR_NORMAL_FRAME_CENTER              0xC0C0C0                   
#define MENU_PALETTE_COLOR_HIGHLIGHT                        0xFFD900
#define MENU_PALETTE_COLOR_ITEM_TEXT                        0x010101
#define MENU_PALETTE_COLOR_CURSOR_GRAYED                    0x7C7C7C
#define MENU_PALETTE_COLOR_ITEM_GRAYED                      0xD8D8D8

//Export APIs
HMENU MENU_CreateMenu(MENU *pMenu, BYTE bMaxVisibleItem, BYTE *bExitKey);
void MENU_ShowMenu(HMENU hMenu, BYTE bActive, BYTE bMenuBackgroundColor);
void MENU_SetMenuItemState(HMENU hMenu, BYTE bMenuIndex, BYTE bEnable);
DWORD MENU_ProcessKey(HMENU hMenu, BYTE bKey);
void MENU_DeleteMenu(HMENU hMenu, BYTE bClearBackground);
void MENU_SetMenuState(HMENU hMenu, BYTE bActive);
void MENU_UpdateCursor(HMENU hMenu, BYTE bCursorStyle);
void MENU_ClearMenuBackground(BYTE bMenuMode, HMENU hMenu);
void MENU_ResetToDefaultState(HMENU hMenu);
BYTE MENU_GetCurrentItemID(HMENU hMenu);
BYTE MENU_AllDisableItems(HMENU hMenu);

//Internal APIs
void _MENU_InitializeGDI(void);
HMENU _MENU_GetMenuHandle(void);
WORD _MENU_GetMenuWidth(MENU *pMenu);
WORD _MENU_GetStringStartH(BYTE bMenuMode, HMENU hMenu, WORD *pString);
void _MENU_ShowSubMenu(HMENU hMenu);
WORD _MENU_GetSubMenuNormalStartH(HMENU hMenu);
WORD _MENU_GetSubMenuNormalStartV(HMENU hMenu);
void _MENU_DrawFrame(BYTE bMenuMode, HMENU hMenu);
void _MENU_ShowMainMenu(HMENU hMenu);
void _MENU_GetMainMenuDisplayRect(HMENU hMenu, PARM_RECT *pMENURect);
void _MENU_GetSubMenuDisplayRect(HMENU hMenu, PARM_RECT *pMENURect);
void _MENU_DrawSeparateFrame(BYTE bMenuMode, HMENU hMenu, BYTE bFrameType);
void _MENU_SetCommonItemStringAttribute(PARM_DRAW_STRING *pMENUStringAttr);
void _MENU_UpdateUpDownArraow(BYTE bMenuMode, HMENU hMenu, BYTE bArrowType);
void _MENU_ClearUpDownArrowBackground(BYTE bMenuMode, HMENU hMenu, BYTE bArrowType);
void _MENU_SetSubMenuInitialDisplay(HMENU hMenu);
void _MENU_SetMainMenuInitialDisplay(HMENU hMenu);
BYTE _MENU_GetCountForUpItem(MENU *pMenu, BYTE bCursorIndex);
BYTE _MENU_GetCountForDownItem(MENU *pMenu, BYTE bCursorIndex);
BYTE _MENU_CheckIfAnyItemHasSubMenu(MENU *pMenu);
void _MENU_CopyString(void);
BYTE _MENU_CheckMainMenuCursorState(HMENU hMenu);
BYTE _MENU_AllDisableSubItems(HMENU hMenu);
#ifdef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
void _MENU_GoToMainMenuItem(HMENU hMenu, BYTE bItem);
void _MENU_GoToSubMenuItem(HMENU hMenu, BYTE bItem);
#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
#endif //__MENU_H__
