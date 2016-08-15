#include "winav.h"
#ifdef SUPPORT_STB
#include "dvbcustm.h"
#endif
#include "gdi.h"
#include "fontindex.h"
#include "utl.h"
#include "osddsply.h"
#include "osdnd.h"
#include "input.h"
#include "menu.h"

MENU *_pMENUCurrentMenu[MAX_SUPPORT_MENU_HANDLE_NUM];
MENU *_pMENUSubMenu[MAX_SUPPORT_MENU_HANDLE_NUM];
HMENU _hMenuStack[MAX_SUPPORT_MENU_HANDLE_NUM] = {MENU_HANDLE_FREE, MENU_HANDLE_FREE, MENU_HANDLE_FREE, MENU_HANDLE_FREE, MENU_HANDLE_FREE};
CURSOR _Cursor[MAX_SUPPORT_MENU_HANDLE_NUM];
MENU_DISPLAY _MainMenuDisplay[MAX_SUPPORT_MENU_HANDLE_NUM];
MENU_DISPLAY _SubMenuDisplay[MAX_SUPPORT_MENU_HANDLE_NUM];
BYTE _bMENUBackgroundColor[MAX_SUPPORT_MENU_HANDLE_NUM];

WORD aMENUUpArrowSymbol[] = {1, CHAR_N_UP_16};
WORD aMENUDownArrowSymbol[] = {1, CHAR_N_DWN_16};
WORD aMENUCheckSymbol[] = {1, CHAR_N_CHECK};
WORD aMENUSubMenuSymbol[] = {1, CHAR_N_SUBMENU};

WORD __wMENUStringBuf[OSDUTL_STRING_LENGTH+1];
BYTE *_bMENUExitKey[MAX_SUPPORT_MENU_HANDLE_NUM];
//***************************************************************************
//  Function    :   _MENU_InitializeGDI
//  Abstract    :   Initialize the palette, etc.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _MENU_InitializeGDI(void)
{
    GDI_ChangePALEntry(MENU_PALETTE_ENTRY_GENERAL_BACKGROUND, MENU_PALETTE_COLOR_GENERAL_BACKGROUND, FALSE);
    GDI_ChangePALEntry(MENU_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT, MENU_PALETTE_COLOR_NORMAL_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(MENU_PALETTE_ENTRY_NORMAL_FRAME_DARK, MENU_PALETTE_COLOR_NORMAL_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(MENU_PALETTE_ENTRY_NORMAL_FRAME_CENTER, MENU_PALETTE_COLOR_NORMAL_FRAME_CENTER, FALSE);
    GDI_ChangePALEntry(MENU_PALETTE_ENTRY_HIGHLIGHT, MENU_PALETTE_COLOR_HIGHLIGHT, FALSE);
    GDI_ChangePALEntry(MENU_PALETTE_ENTRY_ITEM_TEXT, MENU_PALETTE_COLOR_ITEM_TEXT, FALSE);
    GDI_ChangePALEntry(MENU_PALETTE_ENTRY_CURSOR_GRAYED, MENU_PALETTE_COLOR_CURSOR_GRAYED, FALSE);
    GDI_ChangePALEntry(MENU_PALETTE_ENTRY_ITEM_GRAYED, MENU_PALETTE_COLOR_ITEM_GRAYED, FALSE);

    GDI_WaitPaletteComplete();
}

HMENU MENU_CreateMenu(MENU *pMenu, BYTE bMaxVisibleItem, BYTE *bExitKey)
{
    HMENU hMenu;

    hMenu = _MENU_GetMenuHandle();

    if (hMenu != MENU_HANDLE_INVALID)
    {
        _pMENUCurrentMenu[hMenu] = pMenu;
        _pMENUSubMenu[hMenu] = (PMENU)(pMenu->pMenuItem[pMenu->bDefaultCursorPos].pSubMenu);

        //Set the cursor's position
        _Cursor[hMenu].bMainMenuIndex = _pMENUCurrentMenu[hMenu]->bDefaultCursorPos;
        _Cursor[hMenu].bPos = MENU_MAIN_MENU;

        if (_pMENUSubMenu[hMenu] == NULL)
        {
            _Cursor[hMenu].bSubMenuIndex = CURSOR_POS_INVALID;
        }
        else
        {
            _Cursor[hMenu].bSubMenuIndex = _pMENUSubMenu[hMenu]->bDefaultCursorPos;
        }

        //Set the menu display attribute
        _MainMenuDisplay[hMenu].bMaxVisibleItem = bMaxVisibleItem;
        _SubMenuDisplay[hMenu].bMaxVisibleItem = bMaxVisibleItem;

        //Main menu has "Up" mark?
        _MENU_SetMainMenuInitialDisplay(hMenu);

        if (_pMENUSubMenu[hMenu] != NULL)
        {
            //Sub-menu has "Up" mark?
            _MENU_SetSubMenuInitialDisplay(hMenu);
        }

		_bMENUExitKey[hMenu] = bExitKey;
    }
    
    return hMenu;
}

void MENU_ShowMenu(HMENU hMenu, BYTE bActive, BYTE bMenuBackgroundColor)
{   
    _MENU_InitializeGDI();

    _bMENUBackgroundColor[hMenu] = bMenuBackgroundColor;

    //draw main-menu
    _MENU_ShowMainMenu(hMenu);
    
    if (bActive)
    {
        MENU_SetMenuState(hMenu, TRUE);
    }
}

void MENU_SetMenuState(HMENU hMenu, BYTE bActive)
{
    if (MENU_AllDisableItems(hMenu))
        return;

    if (bActive)
    {
        if (_MENU_CheckMainMenuCursorState(hMenu) == TRUE)
        {
            //draw sub-menu
            _MENU_ShowSubMenu(hMenu);
        }
        
        //Update the cursor
        MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL); 
    }
    else    
    {
        //Clear current sub-menu.
        MENU_ClearMenuBackground(MENU_SUB_MENU, hMenu);

        //Update the cursor
        MENU_UpdateCursor(hMenu, CURSOR_STYLE_CLEAR); 
    }
}

void MENU_SetMenuItemState(HMENU hMenu, BYTE bMenuIndex, BYTE bEnable)
{
    PARM_DRAW_STRING MENUStringAttr;
    PARM_RECT MENURect;

    _MENU_GetMainMenuDisplayRect(hMenu, &MENURect);

    _pMENUCurrentMenu[hMenu]->pMenuItem[bMenuIndex].bAttribute = bEnable;

    if (bMenuIndex < _MainMenuDisplay[hMenu].bStartDrawItem)
    {
        return;
    }
    else if ((bMenuIndex-_MainMenuDisplay[hMenu].bStartDrawItem) >= (_SubMenuDisplay[hMenu].bStartDrawItem+_MainMenuDisplay[hMenu].bMaxVisibleItem))
    {
        return;  
    }

    //re-draw the menu.
    MENUStringAttr.bBackground = MENU_PALETTE_ENTRY_GENERAL_BACKGROUND; //PAL_ENTRY_COLOR_TRANSPARENT; //Set the color of background is the same as color key.
    MENUStringAttr.bColorKey = 0;
    MENUStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    MENUStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;

    MENUStringAttr.wX = _pMENUCurrentMenu[hMenu]->wStartX+MENU_STRING_DISTANCE_H;
    MENUStringAttr.wY = _pMENUCurrentMenu[hMenu]->wStartY + MENU_STRING_DISTANCE_V+(bMenuIndex-_MainMenuDisplay[hMenu].bStartDrawItem)*MENU_ITEM_HEIGHT;

    if (bEnable == MENU_ITEM_STATE_DISABLE)
    {
        //Set the color of the string to be disabled color
        MENUStringAttr.bTextColor = MENU_PALETTE_ENTRY_ITEM_GRAYED;
    }
    else
    {
        //Set the color of the string to be enabled color
        MENUStringAttr.bTextColor = MENU_PALETTE_ENTRY_ITEM_TEXT;
    }

    GDI_DrawString_909(0, &MENUStringAttr, _pMENUCurrentMenu[hMenu]->pMenuItem[bMenuIndex].pwItemText);

    if (_pMENUCurrentMenu[hMenu]->pMenuItem[bMenuIndex].pSubMenu != NULL)
    {
        MENUStringAttr.wX = MENURect.rect.wRight-MENU_STRING_DISTANCE_H-GDI_GetStringWidth_909(aMENUSubMenuSymbol);
        MENUStringAttr.wY = _pMENUCurrentMenu[hMenu]->wStartY+MENU_STRING_DISTANCE_V+(bMenuIndex-_MainMenuDisplay[hMenu].bStartDrawItem)*MENU_ITEM_HEIGHT+((MENU_ITEM_HEIGHT-GDI_GetStringHeight(aMENUSubMenuSymbol)) >> 1);
        GDI_DrawString_909(0, &MENUStringAttr, aMENUSubMenuSymbol);
    }
}

//***************************************************************************
//  Function    :   MENU_Processkey
//  Abstract    :   
//  Arguments   :   none.
//  Return      :   
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
DWORD MENU_ProcessKey(HMENU hMenu, BYTE bKey)
{
    BYTE bCount;
	BYTE bExitIndex;

    if (_pMENUCurrentMenu[hMenu] == NULL)
        return MENU_ACTION_NONE;

	if (_bMENUExitKey[hMenu] != NULL)
	{
        if (_Cursor[hMenu].bPos == MENU_MAIN_MENU)
		{
			for (bExitIndex = 0; bExitIndex < sizeof(_bMENUExitKey[hMenu])/sizeof(BYTE); bExitIndex++)
			{
				if (bKey == _bMENUExitKey[hMenu][bExitIndex])
				{
					if ((bKey != KEY_UP) && (bKey != KEY_DOWN) && (bKey != KEY_RIGHT))
					{
						MENU_SetMenuState(hMenu, FALSE);
						return (((DWORD)MENU_ACTION_EXIT) << 16);
					}
#ifndef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
					else if ((bKey == KEY_UP) && (_Cursor[hMenu].bMainMenuIndex == 0))
					{
						MENU_SetMenuState(hMenu, FALSE);
						return (((DWORD)MENU_ACTION_EXIT) << 16);
					}
#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
				}
			}
		}
	}

    //Return the ID of the menu item. The control flow can do the exact action according to the ID.
    switch (bKey)
    {
    case KEY_LEFT:
		/*
#ifndef NO_TOOLBAR_SETUP_MENU		
        if (_Cursor[hMenu].bPos == MENU_MAIN_MENU)
        {
            MENU_SetMenuState(hMenu, FALSE);
            return (((DWORD)MENU_ACTION_EXIT) << 16);
        }
        else if (_Cursor[hMenu].bPos == MENU_SUB_MENU)
        {
            //return to the main menu
            MENU_UpdateCursor(hMenu, CURSOR_STYLE_CLEAR);              
            _Cursor[hMenu].bPos = MENU_MAIN_MENU;
            MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);              
        }
#else //NO_TOOLBAR_SETUP_MENU
        if (_Cursor[hMenu].bPos == MENU_SUB_MENU)
        {
            //return to the main menu
            MENU_UpdateCursor(hMenu, CURSOR_STYLE_CLEAR);              
            _Cursor[hMenu].bPos = MENU_MAIN_MENU;
            MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);              
        }
#endif //NO_TOOLBAR_SETUP_MENU
		*/

        if (_Cursor[hMenu].bPos == MENU_SUB_MENU)
        {
            //return to the main menu
            MENU_UpdateCursor(hMenu, CURSOR_STYLE_CLEAR);              
            _Cursor[hMenu].bPos = MENU_MAIN_MENU;
            MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);              
        }
        break;
    case KEY_RIGHT:
        if (_Cursor[hMenu].bPos == MENU_MAIN_MENU)
        {
            if (_pMENUSubMenu[hMenu] != NULL)
            {
				if ((_pMENUSubMenu[hMenu]->bTotalItem != 0) && (!_MENU_AllDisableSubItems(hMenu))) //to prevent go to a "no item" sub-menu.
				{
					MENU_UpdateCursor(hMenu, CURSOR_STYLE_GRAYED);                
					
					//update the cursor to the sub-menu
					_Cursor[hMenu].bPos = MENU_SUB_MENU;
					MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL); 
				}
            }
        }
        break;        
    case KEY_UP:
        if (_Cursor[hMenu].bPos == MENU_MAIN_MENU)
        {
            if (_Cursor[hMenu].bMainMenuIndex == 0)
            {
#ifndef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                return MENU_ACTION_EXCEED_UP_MAIN_MENU;
#else

                _MENU_GoToMainMenuItem(hMenu, MENU_LAST_ITEM);
                return MENU_ACTION_UP_MAIN_MENU;

#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
            }
            else
            {
                //Check if there is any enabled items for "up" action.
                bCount = _MENU_GetCountForUpItem(_pMENUCurrentMenu[hMenu], _Cursor[hMenu].bMainMenuIndex);
                if (bCount == 0)
                {
#ifndef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                    return MENU_ACTION_EXCEED_UP_MAIN_MENU;
#else
                    _MENU_GoToMainMenuItem(hMenu, MENU_LAST_ITEM);
                    return MENU_ACTION_UP_MAIN_MENU;
#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                }                
             
                //Clear current sub-menu.
                MENU_ClearMenuBackground(MENU_SUB_MENU, hMenu);
                
                //if ((_Cursor[hMenu].bMainMenuIndex-1) < _MainMenuDisplay[hMenu].bStartDrawItem)
                if ((_Cursor[hMenu].bMainMenuIndex-bCount) < _MainMenuDisplay[hMenu].bStartDrawItem)
                {                               
                    //scroll up
                    _Cursor[hMenu].bMainMenuIndex -= bCount;

                    if (_MainMenuDisplay[hMenu].bStartDrawItem >= bCount)
                    {
                        _MainMenuDisplay[hMenu].bStartDrawItem -= bCount;
                    }
                    else
                    {
                        _MainMenuDisplay[hMenu].bStartDrawItem = 0;
                    }
                    
                    //Note: Must clear the "up" region before re-draw the sub menu.
                    if (_MainMenuDisplay[hMenu].bStartDrawItem == 0)
                    {
                        //Clear "Up" mark
                        _MENU_ClearUpDownArrowBackground(MENU_MAIN_MENU, hMenu, MENU_ARROW_TYPE_UP);
                        _MainMenuDisplay[hMenu].bUpMark = FALSE;
                    }
                    
                    if ((_MainMenuDisplay[hMenu].bStartDrawItem+_MainMenuDisplay[hMenu].bMaxVisibleItem) < _pMENUCurrentMenu[hMenu]->bTotalItem)
                    {
                        _MainMenuDisplay[hMenu].bDownMark = TRUE;
                    }
                    
                    _MENU_ShowMainMenu(hMenu);
                }
                else
                {
                    //Update the cursor
                    MENU_UpdateCursor(hMenu, CURSOR_STYLE_CLEAR);                
                    _Cursor[hMenu].bMainMenuIndex -= bCount;
                }

                MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);

                _pMENUSubMenu[hMenu] = _pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].pSubMenu;                
                //Show sub-menu
                if ((_pMENUSubMenu[hMenu] != NULL) && (_pMENUSubMenu[hMenu]->bTotalItem != 0))
                {
                    _pMENUSubMenu[hMenu] = (PMENU)(_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].pSubMenu);
                    _MENU_SetSubMenuInitialDisplay(hMenu);
                    _Cursor[hMenu].bSubMenuIndex = _pMENUSubMenu[hMenu]->bDefaultCursorPos;
                    _MENU_ShowSubMenu(hMenu);
                }

                return MENU_ACTION_UP_MAIN_MENU;
            }
        }
        else if (_Cursor[hMenu].bPos == MENU_SUB_MENU)
        {
            if (_Cursor[hMenu].bSubMenuIndex == 0)
            {
#ifndef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                return MENU_ACTION_EXCEED_UP_SUB_MENU;
#else
                _MENU_GoToSubMenuItem(hMenu, MENU_LAST_ITEM);
                return MENU_ACTION_UP_SUB_MENU;

#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
            }
            else
            {
                bCount = _MENU_GetCountForUpItem(_pMENUSubMenu[hMenu], _Cursor[hMenu].bSubMenuIndex);
                if (bCount == 0)
                {
#ifndef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                    return MENU_ACTION_EXCEED_UP_SUB_MENU;
#else
                    _MENU_GoToSubMenuItem(hMenu, MENU_LAST_ITEM);
                    return MENU_ACTION_UP_SUB_MENU;
#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                }             
                
                if ((_Cursor[hMenu].bSubMenuIndex-bCount) < _SubMenuDisplay[hMenu].bStartDrawItem)
                {
                    //scroll up
                    _Cursor[hMenu].bSubMenuIndex -= bCount;

                    if (_SubMenuDisplay[hMenu].bStartDrawItem >= bCount)
                    {
                        _SubMenuDisplay[hMenu].bStartDrawItem -= bCount;
                    }
                    else
                    {
                        _SubMenuDisplay[hMenu].bStartDrawItem = 0;
                    }

                    //Note: Must clear the "up" region before re-draw the sub menu.
                    if (_SubMenuDisplay[hMenu].bStartDrawItem == 0)
                    {
                        //Clear "Up" mark
                        _MENU_ClearUpDownArrowBackground(MENU_SUB_MENU, hMenu, MENU_ARROW_TYPE_UP);
                        _SubMenuDisplay[hMenu].bUpMark = FALSE;
                    }

                    if ((_SubMenuDisplay[hMenu].bStartDrawItem+_SubMenuDisplay[hMenu].bMaxVisibleItem) < _pMENUSubMenu[hMenu]->bTotalItem)
                    {
                        _SubMenuDisplay[hMenu].bDownMark = TRUE;
                    }

                    _MENU_ShowSubMenu(hMenu);
                }
                else
                {    
                    //Update the cursor
                    MENU_UpdateCursor(hMenu, CURSOR_STYLE_CLEAR);
                    _Cursor[hMenu].bSubMenuIndex -= bCount;                    
                }

                MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);

                return MENU_ACTION_UP_SUB_MENU;
            }
        }
        break;
    case KEY_DOWN:
        if (_Cursor[hMenu].bPos == MENU_MAIN_MENU)
        {
            if (_Cursor[hMenu].bMainMenuIndex == (_pMENUCurrentMenu[hMenu]->bTotalItem-1))
            {
#ifndef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                return MENU_ACTION_EXCEED_DOWN_MAIN_MENU;
#else

                _MENU_GoToMainMenuItem(hMenu, MENU_FIRST_ITEM);
                return MENU_ACTION_UP_MAIN_MENU;
#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
            }
            else
            {
                //Check if there is any enabled items for "Down" action.
                bCount = _MENU_GetCountForDownItem(_pMENUCurrentMenu[hMenu], _Cursor[hMenu].bMainMenuIndex);
                if (bCount == 0)
                {
#ifndef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                    return MENU_ACTION_EXCEED_DOWN_MAIN_MENU;
#else
                    _MENU_GoToMainMenuItem(hMenu, MENU_FIRST_ITEM);
                    return MENU_ACTION_UP_MAIN_MENU;
#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                    
                }                

                //Clear current sub-menu.
                MENU_ClearMenuBackground(MENU_SUB_MENU, hMenu);

                //if ((_Cursor[hMenu].bMainMenuIndex-_MainMenuDisplay[hMenu].bStartDrawItem+1) >= _MainMenuDisplay[hMenu].bMaxVisibleItem)
                if ((_Cursor[hMenu].bMainMenuIndex-_MainMenuDisplay[hMenu].bStartDrawItem+bCount) >= _MainMenuDisplay[hMenu].bMaxVisibleItem)
                {
                    //scroll down
                    _Cursor[hMenu].bMainMenuIndex += bCount;
                    //_MainMenuDisplay[hMenu].bStartDrawItem += bCount;
                    _MainMenuDisplay[hMenu].bStartDrawItem = _Cursor[hMenu].bMainMenuIndex-_MainMenuDisplay[hMenu].bMaxVisibleItem+1;
                    _MainMenuDisplay[hMenu].bDownMark = TRUE;
                    
                    //Note: Must clear the "Down" region before re-draw the sub menu.
                    if ((_MainMenuDisplay[hMenu].bStartDrawItem+_MainMenuDisplay[hMenu].bMaxVisibleItem) == _pMENUCurrentMenu[hMenu]->bTotalItem)
                    {
                        //Clear "Down" mark
                        _MENU_ClearUpDownArrowBackground(MENU_MAIN_MENU, hMenu, MENU_ARROW_TYPE_DOWN);
                        _MainMenuDisplay[hMenu].bDownMark = FALSE;
                    }
                    
                    if (_MainMenuDisplay[hMenu].bStartDrawItem != 0)
                    {
                        _MainMenuDisplay[hMenu].bUpMark = TRUE;
                    }
                    
                    _MENU_ShowMainMenu(hMenu);
                }
                else
                {
                    //Update the cursor
                    MENU_UpdateCursor(hMenu, CURSOR_STYLE_CLEAR);                
                    _Cursor[hMenu].bMainMenuIndex += bCount;
                }

                MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);

                _pMENUSubMenu[hMenu] = _pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].pSubMenu;                
                //Show sub-menu
                if ((_pMENUSubMenu[hMenu] != NULL) && (_pMENUSubMenu[hMenu]->bTotalItem != 0))
                {
                    _pMENUSubMenu[hMenu] = (PMENU)(_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].pSubMenu);
                    _MENU_SetSubMenuInitialDisplay(hMenu);
                    _Cursor[hMenu].bSubMenuIndex = _pMENUSubMenu[hMenu]->bDefaultCursorPos;
                    _MENU_ShowSubMenu(hMenu);
                }

                return MENU_ACTION_DOWN_MAIN_MENU;
            }
        }
        else if (_Cursor[hMenu].bPos == MENU_SUB_MENU)
        {
            if (_Cursor[hMenu].bSubMenuIndex == (_pMENUSubMenu[hMenu]->bTotalItem-1))
            {
#ifndef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                return MENU_ACTION_EXCEED_DOWN_SUB_MENU;
#else
                _MENU_GoToSubMenuItem(hMenu, MENU_FIRST_ITEM);
                return MENU_ACTION_DOWN_SUB_MENU;
#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
            }
            else
            {
                bCount = _MENU_GetCountForDownItem(_pMENUSubMenu[hMenu], _Cursor[hMenu].bSubMenuIndex);
                if (bCount == 0)
                {
#ifndef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                    return MENU_ACTION_EXCEED_DOWN_SUB_MENU;
#else
                    _MENU_GoToSubMenuItem(hMenu, MENU_FIRST_ITEM);
                    return MENU_ACTION_DOWN_SUB_MENU;
#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
                    
                }                

                if ((_Cursor[hMenu].bSubMenuIndex-_SubMenuDisplay[hMenu].bStartDrawItem+bCount) >= _SubMenuDisplay[hMenu].bMaxVisibleItem)
                {
                    //scroll down
                    _Cursor[hMenu].bSubMenuIndex += bCount;
                    _SubMenuDisplay[hMenu].bStartDrawItem++;
                    _SubMenuDisplay[hMenu].bDownMark = TRUE;
                    
                    //Note: Must clear the "Down" region before re-draw the sub menu.
                    if ((_SubMenuDisplay[hMenu].bStartDrawItem+_SubMenuDisplay[hMenu].bMaxVisibleItem) == _pMENUSubMenu[hMenu]->bTotalItem)
                    {
                        //Clear "Down" mark
                        _MENU_ClearUpDownArrowBackground(MENU_SUB_MENU, hMenu, MENU_ARROW_TYPE_DOWN);
                        _SubMenuDisplay[hMenu].bDownMark = FALSE;
                    }

                    if (_SubMenuDisplay[hMenu].bStartDrawItem != 0)
                    {
                        _SubMenuDisplay[hMenu].bUpMark = TRUE;
                    }
                    
                    _MENU_ShowSubMenu(hMenu);
                }
                else
                {
                    //Update the cursor
                    MENU_UpdateCursor(hMenu, CURSOR_STYLE_CLEAR);                
                    _Cursor[hMenu].bSubMenuIndex += bCount;
                }

                MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);

                return MENU_ACTION_DOWN_SUB_MENU;
            }
        }
        break;
    case KEY_PLAY:
    case KEY_ENTER:
        if (_Cursor[hMenu].bPos == MENU_MAIN_MENU)
        {
            if (_pMENUSubMenu[hMenu] != NULL)
            {
				if (_pMENUSubMenu[hMenu]->bTotalItem != 0) //to prevent go to a "no item" sub-menu.
				{
					MENU_UpdateCursor(hMenu, CURSOR_STYLE_GRAYED);                
					
					//update the cursor to the sub-menu
					_Cursor[hMenu].bPos = MENU_SUB_MENU;
					MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);
				}
            }
            else
            {
                //return the ID to do exact action.
                return ((((DWORD)MENU_ACTION_PROCESS_MENU_ITEM) << 16) | _pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].bID);
            }
        }
        else if (_Cursor[hMenu].bPos == MENU_SUB_MENU)
        {
            if (_pMENUSubMenu[hMenu]->bCheckMenuItemPos != MENU_ITEM_NO_CHECKED_ITEM) //CoCo2.38
            {
                _pMENUSubMenu[hMenu]->bCheckMenuItemPos = _Cursor[hMenu].bSubMenuIndex;
                _pMENUSubMenu[hMenu]->bDefaultCursorPos = _pMENUSubMenu[hMenu]->bCheckMenuItemPos;
            }
            printf("\n go to menu processkey menu sub menu");
            //Recover the cursor to normal state in the sub-menu and re-draw the check item.
            _MENU_ShowSubMenu(hMenu);

            //Return to main menu
            _Cursor[hMenu].bPos = MENU_MAIN_MENU;
            MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);                              
            
            if (_pMENUSubMenu[hMenu]->bMenuType == MENU_TYPE_NUM)
            {
                return (MENU_ACTION_PROCESS_NUN_ITEM) | (((DWORD)(_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].bID)) << 16) | _Cursor[hMenu].bSubMenuIndex;
            }
            else
            {
                return (((DWORD)MENU_ACTION_PROCESS_MENU_ITEM << 16) | _pMENUSubMenu[hMenu]->pMenuItem[(_Cursor[hMenu].bSubMenuIndex)].bID);                
            }
        }
        break;
    default:
        break;
    }

    return MENU_ACTION_NONE;
}

//***************************************************************************
//  Function    :   MENU_DeleteMenu
//  Abstract    :   
//  Arguments   :   none.
//  Return      :   
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void MENU_DeleteMenu(HMENU hMenu, BYTE bClearBackground)
{
    //clear menu
    if (_pMENUCurrentMenu[hMenu] != NULL)
    {
        if (bClearBackground)
        {
            MENU_ClearMenuBackground(MENU_MAIN_MENU, hMenu);
        }
    }

    if (_pMENUSubMenu[hMenu] != NULL)
    {
        if (bClearBackground)
        {
            MENU_ClearMenuBackground(MENU_SUB_MENU, hMenu);
        }
    }

    _pMENUCurrentMenu[hMenu] = NULL;
    _pMENUSubMenu[hMenu] = NULL;
    _hMenuStack[hMenu] = MENU_HANDLE_FREE;
}


HMENU _MENU_GetMenuHandle(void)
{
    BYTE i;
    for (i = 0; i < MAX_SUPPORT_MENU_HANDLE_NUM; i++)
    {
        if (_hMenuStack[i] == MENU_HANDLE_FREE)
        {
            _hMenuStack[i] = i;
            return _hMenuStack[i];
        }
    }

    return MENU_HANDLE_INVALID;
}

WORD _MENU_GetMenuWidth(MENU *pMenu)
{
    BYTE bItemIndex;
    WORD wMaxWidth = 0;
    WORD wStringWidth;

    if (pMenu == NULL)
        return 0;
    
    if (pMenu->wWidth != MENU_WIDTH_OPTIMIZED)
        return pMenu->wWidth;

    for (bItemIndex = 0; bItemIndex < pMenu->bTotalItem; bItemIndex++)
    {        
        if (pMenu->bMenuType == MENU_TYPE_NUM)
        {
            OSDUTL_PrepareOutputString();
            OSDUTL_OutputNumFromIndex(pMenu->wStartNum+bItemIndex);

            wStringWidth = GDI_GetStringWidth_909(__pOSDUTLOutputString);

            if (wMaxWidth < wStringWidth)
                wMaxWidth = wStringWidth;
        }
        else
        {
            wStringWidth = GDI_GetStringWidth_909(pMenu->pMenuItem[bItemIndex].pwItemText);            

            if (wMaxWidth < wStringWidth)
                wMaxWidth = wStringWidth;
        }
    }

    wMaxWidth += MENU_STRING_DISTANCE_H*2;

    if (pMenu->bCheckMenuItemPos != MENU_ITEM_NO_CHECKED_ITEM)
    {
        wMaxWidth += GDI_GetStringWidth_909(aMENUCheckSymbol);
        wMaxWidth += MENU_CHECK_SYMBOL_DISTANCE_H;
    }

    if (_MENU_CheckIfAnyItemHasSubMenu(pMenu))
    {
        wMaxWidth += GDI_GetStringWidth_909(aMENUSubMenuSymbol);
        wMaxWidth += MENU_SUBMENU_SYMBOL_DISTANCE_H;
    }

    return wMaxWidth;
}

WORD _MENU_GetStringStartH(BYTE bMenuMode, HMENU hMenu, WORD *pString)
{
    WORD wStringWidth;
    WORD wMenuWidth;
    MENU *pMenu = NULL;
    WORD wStartH;
    WORD wCheckSymbolWidth = 0;
    WORD wSubMenuSymbolWidth = 0;
    PARM_RECT MENURect;

    if (bMenuMode == MENU_MAIN_MENU)
    {
        pMenu = _pMENUCurrentMenu[hMenu];
        _MENU_GetMainMenuDisplayRect(hMenu, &MENURect);
    }
    else if (bMenuMode == MENU_SUB_MENU)
    {
        pMenu = _pMENUSubMenu[hMenu];
        _MENU_GetSubMenuDisplayRect(hMenu, &MENURect);
    }

    wMenuWidth = _MENU_GetMenuWidth(pMenu);

    wStringWidth = GDI_GetStringWidth_909(pString);

    wStartH = MENURect.rect.wLeft+MENU_STRING_DISTANCE_H;
    if (pMenu->bCheckMenuItemPos != MENU_ITEM_NO_CHECKED_ITEM)
    {
        wCheckSymbolWidth = GDI_GetStringWidth_909(aMENUCheckSymbol) + MENU_CHECK_SYMBOL_DISTANCE_H;
    }

    if (_MENU_CheckIfAnyItemHasSubMenu(pMenu))
    {
        wSubMenuSymbolWidth = GDI_GetStringWidth_909(aMENUSubMenuSymbol)+MENU_SUBMENU_SYMBOL_DISTANCE_H;
    }

    switch (pMenu->bAlignment)
    {
    case MENU_ITEM_ALIGNMENT_CENTER:
        wStartH = wStartH+wCheckSymbolWidth+((wMenuWidth-wCheckSymbolWidth-wSubMenuSymbolWidth-MENU_STRING_DISTANCE_H*2-wStringWidth)>>1);                
        break;
    case MENU_ITEM_ALIGNMENT_RIGHT:
        wStartH = MENURect.rect.wLeft+wMenuWidth-MENU_STRING_DISTANCE_H-wSubMenuSymbolWidth-wStringWidth;
        break;
    case MENU_ITEM_ALIGNMENT_LEFT:
        wStartH += wCheckSymbolWidth;
        break;
    default:
        break;
    }

    return wStartH;
}

void MENU_UpdateCursor(HMENU hMenu, BYTE bCursorStyle)
{
    PARM_RECT MENURect;
    PARM_DRAW_STRING MENUStringAttr;
    MENU *pMenu;
    BYTE bCursorIndex;

    if (_Cursor[hMenu].bPos == MENU_MAIN_MENU)
    {
        pMenu = _pMENUCurrentMenu[hMenu];
        bCursorIndex = _Cursor[hMenu].bMainMenuIndex;
        MENURect.rect.wLeft = pMenu->wStartX;
        MENURect.rect.wTop = pMenu->wStartY+(bCursorIndex-_MainMenuDisplay[hMenu].bStartDrawItem)*MENU_ITEM_HEIGHT+MENU_STRING_DISTANCE_V;
    }
    else if (_Cursor[hMenu].bPos == MENU_SUB_MENU)
    {
        pMenu = _pMENUSubMenu[hMenu];
        bCursorIndex = _Cursor[hMenu].bSubMenuIndex;
        MENURect.rect.wLeft = _MENU_GetSubMenuNormalStartH(hMenu);
        MENURect.rect.wTop = _MENU_GetSubMenuNormalStartV(hMenu)+(bCursorIndex-_SubMenuDisplay[hMenu].bStartDrawItem)*MENU_ITEM_HEIGHT+MENU_STRING_DISTANCE_V;        
    }
    else
        return;

    //Draw the cursor
    if (bCursorStyle == CURSOR_STYLE_NORMAL)
    {
        MENURect.bColor = MENU_PALETTE_ENTRY_HIGHLIGHT;
    }
    else if (bCursorStyle == CURSOR_STYLE_CLEAR)
    {
        MENURect.bColor = MENU_PALETTE_ENTRY_GENERAL_BACKGROUND;
    }
    else if (bCursorStyle == CURSOR_STYLE_GRAYED)
    {
        MENURect.bColor = MENU_PALETTE_ENTRY_CURSOR_GRAYED;
    }

    MENURect.rect.wRight = MENURect.rect.wLeft + _MENU_GetMenuWidth(pMenu) - 1;
    MENURect.rect.wBottom = MENURect.rect.wTop+MENU_ITEM_HEIGHT-1;
     
    GDI_FillRect(0, &MENURect);

    //Draw the menu item string
    MENUStringAttr.bTextColor = MENU_PALETTE_ENTRY_ITEM_TEXT;
    MENUStringAttr.bColorKey = 0;
    MENUStringAttr.bBackground = MENURect.bColor;
     
    if (pMenu->bMenuType == MENU_TYPE_NUM)
    {            
        OSDUTL_PrepareOutputString();
        OSDUTL_OutputNumFromIndex(pMenu->wStartNum+bCursorIndex);
        _MENU_CopyString();
        
        MENUStringAttr.wX = _MENU_GetStringStartH(_Cursor[hMenu].bPos, hMenu, __pOSDUTLOutputString);
        MENUStringAttr.wY = MENURect.rect.wTop+((MENU_ITEM_HEIGHT-GDI_GetStringHeight(__pOSDUTLOutputString)) >> 1);        
        GDI_DrawString_909(0, &MENUStringAttr, __wMENUStringBuf);
    }
    else
    {                        
        MENUStringAttr.wX = _MENU_GetStringStartH(_Cursor[hMenu].bPos, hMenu, pMenu->pMenuItem[bCursorIndex].pwItemText);                
        MENUStringAttr.wY = MENURect.rect.wTop+((MENU_ITEM_HEIGHT-GDI_GetStringHeight(pMenu->pMenuItem[bCursorIndex].pwItemText)) >> 1);
        GDI_DrawString_909(0, &MENUStringAttr, pMenu->pMenuItem[bCursorIndex].pwItemText);
    }

    //Draw Check symbol
    if (_Cursor[hMenu].bPos == MENU_SUB_MENU)
    {
        if (pMenu->bCheckMenuItemPos != MENU_ITEM_NO_CHECKED_ITEM)
        {
            if (bCursorIndex == pMenu->bCheckMenuItemPos)
            {
                MENUStringAttr.wX = MENURect.rect.wLeft + MENU_STRING_DISTANCE_H;
                MENUStringAttr.wY = MENURect.rect.wTop+((MENU_ITEM_HEIGHT-GDI_GetStringHeight(aMENUCheckSymbol)) >> 1);
                GDI_DrawString_909(0, &MENUStringAttr, aMENUCheckSymbol);
            }
        }
    }

    if (_Cursor[hMenu].bPos == MENU_MAIN_MENU)
    {
        if (pMenu->pMenuItem[bCursorIndex].pSubMenu != NULL)
        {
            MENUStringAttr.wX = MENURect.rect.wRight-MENU_STRING_DISTANCE_H-GDI_GetStringWidth_909(aMENUSubMenuSymbol);
            MENUStringAttr.wY = MENURect.rect.wTop+((MENU_ITEM_HEIGHT-GDI_GetStringHeight(aMENUSubMenuSymbol)) >> 1);
           GDI_DrawString_909(0, &MENUStringAttr, aMENUSubMenuSymbol);
        }
    }
}

void _MENU_ShowSubMenu(HMENU hMenu)
{
    BYTE bCount = 0;
    BYTE bMenuIndex;
    PARM_RECT MENURect;
    PARM_DRAW_STRING MENUStringAttr;
    WORD wMenuWidth;
    BYTE bDisplayItemNum;

    if (_pMENUSubMenu[hMenu] != NULL)
    {      
        //CoCo1.13
        if (_pMENUSubMenu[hMenu]->pMenuItem[_Cursor[hMenu].bSubMenuIndex].bAttribute == MENU_ITEM_STATE_DISABLE)
        {
            bCount = _MENU_GetCountForDownItem(_pMENUSubMenu[hMenu], _Cursor[hMenu].bSubMenuIndex);
            
            if (bCount == 0)
            {
                bCount = _MENU_GetCountForUpItem(_pMENUSubMenu[hMenu], _Cursor[hMenu].bSubMenuIndex);
                if (bCount > 0)
                {
                    _Cursor[hMenu].bSubMenuIndex -= bCount;
                }
            }
            else
            {
                _Cursor[hMenu].bSubMenuIndex += bCount;
            }
            
            _pMENUSubMenu[hMenu]->bDefaultCursorPos = _Cursor[hMenu].bSubMenuIndex;
            _MENU_SetSubMenuInitialDisplay(hMenu);
        }                 

        //Fill rectangle
        _MENU_GetSubMenuDisplayRect(hMenu, &MENURect);
        MENURect.bColor = MENU_PALETTE_ENTRY_GENERAL_BACKGROUND;        
        GDI_FillRect(0, &MENURect);
    
        //Fill frame
        _MENU_DrawFrame(MENU_SUB_MENU, hMenu);

        //Draw separate frame
        if (_SubMenuDisplay[hMenu].bUpMark == TRUE) 
        {
            _MENU_DrawSeparateFrame(MENU_SUB_MENU, hMenu, MENU_UP_SEPARATE_FRAME);
            _MENU_UpdateUpDownArraow(MENU_SUB_MENU, hMenu, MENU_ARROW_TYPE_UP);
        }
        
        if (_SubMenuDisplay[hMenu].bDownMark == TRUE)
        {
            _MENU_DrawSeparateFrame(MENU_SUB_MENU, hMenu, MENU_DOWN_SEPARATE_FRAME);
            _MENU_UpdateUpDownArraow(MENU_SUB_MENU, hMenu, MENU_ARROW_TYPE_DOWN);
        }        

        wMenuWidth = _MENU_GetMenuWidth(_pMENUSubMenu[hMenu]);

        MENUStringAttr.bTextColor = MENU_PALETTE_ENTRY_ITEM_TEXT;
        MENUStringAttr.bBackground = MENU_PALETTE_ENTRY_GENERAL_BACKGROUND;
        MENUStringAttr.bColorKey = 0;

        if (_pMENUSubMenu[hMenu]->bTotalItem <= _SubMenuDisplay[hMenu].bMaxVisibleItem)
        {
            bDisplayItemNum = _pMENUSubMenu[hMenu]->bTotalItem;
        }
        else
        {
            bDisplayItemNum = _SubMenuDisplay[hMenu].bMaxVisibleItem;
        }
        
        for (bMenuIndex = 0; bMenuIndex < bDisplayItemNum; bMenuIndex++)
        {             
            if (_pMENUSubMenu[hMenu]->pMenuItem[bMenuIndex+(_SubMenuDisplay[hMenu].bStartDrawItem)].bAttribute == MENU_ITEM_STATE_DISABLE)
            {
                MENUStringAttr.bTextColor = MENU_PALETTE_ENTRY_ITEM_GRAYED;
            }
            else
            {
                MENUStringAttr.bTextColor = MENU_PALETTE_ENTRY_ITEM_TEXT;            
            }

            if (_pMENUSubMenu[hMenu]->bMenuType == MENU_TYPE_NUM)
            {
                OSDUTL_PrepareOutputString();
                OSDUTL_OutputNumFromIndex(_pMENUSubMenu[hMenu]->wStartNum+_SubMenuDisplay[hMenu].bStartDrawItem+bMenuIndex);
                _MENU_CopyString();
                
                MENUStringAttr.wX = _MENU_GetStringStartH(MENU_SUB_MENU, hMenu, __pOSDUTLOutputString);
                MENUStringAttr.wY = _MENU_GetSubMenuNormalStartV(hMenu) + MENU_STRING_DISTANCE_V+bMenuIndex*MENU_ITEM_HEIGHT+((MENU_ITEM_HEIGHT-GDI_GetStringHeight(__pOSDUTLOutputString)) >> 1);
                GDI_DrawString_909(0, &MENUStringAttr, __wMENUStringBuf);
            }
            else
            {
                MENUStringAttr.wX = _MENU_GetStringStartH(MENU_SUB_MENU, hMenu, _pMENUSubMenu[hMenu]->pMenuItem[bMenuIndex+(_SubMenuDisplay[hMenu].bStartDrawItem)].pwItemText);                
                MENUStringAttr.wY = _MENU_GetSubMenuNormalStartV(hMenu) + MENU_STRING_DISTANCE_V + bMenuIndex*MENU_ITEM_HEIGHT+((MENU_ITEM_HEIGHT-GDI_GetStringHeight(_pMENUSubMenu[hMenu]->pMenuItem[bMenuIndex+(_SubMenuDisplay[hMenu].bStartDrawItem)].pwItemText)) >> 1);;
                GDI_DrawString_909(0, &MENUStringAttr, _pMENUSubMenu[hMenu]->pMenuItem[bMenuIndex+(_SubMenuDisplay[hMenu].bStartDrawItem)].pwItemText);
            }
            
            //Draw check symbol
            if (_pMENUSubMenu[hMenu]->bCheckMenuItemPos != MENU_ITEM_NO_CHECKED_ITEM)
            {
                if ((bMenuIndex+_SubMenuDisplay[hMenu].bStartDrawItem) == _pMENUSubMenu[hMenu]->bCheckMenuItemPos)
                {
                    MENUStringAttr.wX = MENURect.rect.wLeft + MENU_STRING_DISTANCE_H;
                    MENUStringAttr.wY = _MENU_GetSubMenuNormalStartV(hMenu) + MENU_STRING_DISTANCE_V + bMenuIndex*MENU_ITEM_HEIGHT+((MENU_ITEM_HEIGHT-GDI_GetStringHeight(aMENUCheckSymbol)) >> 1);
                    GDI_DrawString_909(0, &MENUStringAttr, aMENUCheckSymbol);
                }
            }
        }
    }
}

WORD _MENU_GetSubMenuNormalStartH(HMENU hMenu)
{
    if (_pMENUSubMenu[hMenu]->wStartX == MENU_START_X_OPTIMIZED)
    {
        return _pMENUCurrentMenu[hMenu]->wStartX+_MENU_GetMenuWidth(_pMENUCurrentMenu[hMenu])+MENU_SUBMENU_DISTANCE;
    }
    else
    {
        return _pMENUSubMenu[hMenu]->wStartX;
    }    
}

WORD _MENU_GetSubMenuNormalStartV(HMENU hMenu)
{
    if (_pMENUSubMenu[hMenu]->wStartY == MENU_START_Y_OPTIMIZED)
    {
        return _pMENUCurrentMenu[hMenu]->wStartY;
    }
    else
    {
        return _pMENUSubMenu[hMenu]->wStartY;
    }
}

void _MENU_GetMainMenuDisplayRect(HMENU hMenu, PARM_RECT *pMENURect)
{  
    BYTE bItem;

    pMENURect->rect.wLeft = _pMENUCurrentMenu[hMenu]->wStartX;
    pMENURect->rect.wTop = _pMENUCurrentMenu[hMenu]->wStartY;
    pMENURect->rect.wRight = _pMENUCurrentMenu[hMenu]->wStartX+_MENU_GetMenuWidth(_pMENUCurrentMenu[hMenu])-1;        

    if ((_MainMenuDisplay[hMenu].bUpMark == TRUE) || (_MainMenuDisplay[hMenu].bDownMark == TRUE))
    {
        bItem = _MainMenuDisplay[hMenu].bMaxVisibleItem;        
    }
    else
    {
        bItem = _pMENUCurrentMenu[hMenu]->bTotalItem;
    }

    pMENURect->rect.wBottom = _pMENUCurrentMenu[hMenu]->wStartY+bItem*MENU_ITEM_HEIGHT+MENU_STRING_DISTANCE_V*2-1; 

    if (_MainMenuDisplay[hMenu].bUpMark == TRUE)
    {
        pMENURect->rect.wTop -= MENU_SEPARATE_FRAME_THICKNESS;
        pMENURect->rect.wTop -= MENU_UP_DOWN_ITEM_HEIGHT;
    }
    
    if (_MainMenuDisplay[hMenu].bDownMark == TRUE)
    {
        pMENURect->rect.wBottom += MENU_SEPARATE_FRAME_THICKNESS;
        pMENURect->rect.wBottom += MENU_UP_DOWN_ITEM_HEIGHT;
    } 
}

void _MENU_GetSubMenuDisplayRect(HMENU hMenu, PARM_RECT *pMENURect)
{
    BYTE bItem;

    pMENURect->rect.wLeft = _MENU_GetSubMenuNormalStartH(hMenu);
    pMENURect->rect.wTop = _MENU_GetSubMenuNormalStartV(hMenu);
    pMENURect->rect.wRight = pMENURect->rect.wLeft + _MENU_GetMenuWidth(_pMENUSubMenu[hMenu]) - 1;

    if ((_SubMenuDisplay[hMenu].bUpMark == TRUE) || (_SubMenuDisplay[hMenu].bDownMark == TRUE))
    {
        bItem = _SubMenuDisplay[hMenu].bMaxVisibleItem;        
    }
    else
    {
        bItem = _pMENUSubMenu[hMenu]->bTotalItem;
    }

    pMENURect->rect.wBottom = _MENU_GetSubMenuNormalStartV(hMenu)+bItem*MENU_ITEM_HEIGHT+MENU_STRING_DISTANCE_V*2-1; 
    
    if (_SubMenuDisplay[hMenu].bUpMark == TRUE)
    {
        pMENURect->rect.wTop -= MENU_SEPARATE_FRAME_THICKNESS;
        pMENURect->rect.wTop -= MENU_UP_DOWN_ITEM_HEIGHT;
    }
    
    if (_SubMenuDisplay[hMenu].bDownMark == TRUE)
    {
        pMENURect->rect.wBottom += MENU_SEPARATE_FRAME_THICKNESS;
        pMENURect->rect.wBottom += MENU_UP_DOWN_ITEM_HEIGHT;
    } 
}

void _MENU_DrawFrame(BYTE bMenuMode, HMENU hMenu)
{
    PARM_RECT MENURect;
    PARM_FRAME MENUFrame;

    if (bMenuMode == MENU_MAIN_MENU)
    {
        _MENU_GetMainMenuDisplayRect(hMenu, &MENURect);
    }
    else if (bMenuMode == MENU_SUB_MENU)
    {
        _MENU_GetSubMenuDisplayRect(hMenu, &MENURect);
    }

    MENUFrame.rect.wTop = MENURect.rect.wTop - MENU_FRAME_THICKNESS;
    MENUFrame.rect.wBottom = MENURect.rect.wBottom + MENU_FRAME_THICKNESS;
    MENUFrame.rect.wLeft = MENURect.rect.wLeft - MENU_FRAME_THICKNESS;
    MENUFrame.rect.wRight = MENURect.rect.wRight + MENU_FRAME_THICKNESS;

    MENUFrame.bColorBright = MENU_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
    MENUFrame.bColorDark = MENU_PALETTE_ENTRY_NORMAL_FRAME_DARK;    
    MENUFrame.bColorCenter = MENU_PALETTE_ENTRY_NORMAL_FRAME_CENTER;
    MENUFrame.bStyle = FRAME_POP_SOLID; //FRAME_DOUBLE;
    MENUFrame.bThickness = MENU_FRAME_THICKNESS;
    
    GDI_DrawFrame_909(0, &MENUFrame);
}

void _MENU_ShowMainMenu(HMENU hMenu)
{
    BYTE bMenuIndex;
    PARM_RECT MENURect;
    PARM_DRAW_STRING MENUStringAttr;
    BYTE bDisplayItemNum;
    BYTE bCount;

    if (_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].bAttribute == MENU_ITEM_STATE_DISABLE)
    {
         bCount = _MENU_GetCountForDownItem(_pMENUCurrentMenu[hMenu], _Cursor[hMenu].bMainMenuIndex);

         if (bCount == 0)
         {
             bCount = _MENU_GetCountForUpItem(_pMENUCurrentMenu[hMenu], _Cursor[hMenu].bMainMenuIndex);
             if (bCount > 0)
             {
                 _Cursor[hMenu].bMainMenuIndex -= bCount;
             }
         }
         else
         {
            _Cursor[hMenu].bMainMenuIndex += bCount;
         }

         _pMENUCurrentMenu[hMenu]->bDefaultCursorPos = _Cursor[hMenu].bMainMenuIndex;
         _MENU_SetMainMenuInitialDisplay(hMenu);
    }

    //Fill rectangle
    MENURect.bColor = MENU_PALETTE_ENTRY_GENERAL_BACKGROUND;
    _MENU_GetMainMenuDisplayRect(hMenu, &MENURect);     
    GDI_FillRect(0, &MENURect);
    
    //Fill frame
    _MENU_DrawFrame(MENU_MAIN_MENU, hMenu);

    //Draw separate frame
    if (_MainMenuDisplay[hMenu].bUpMark == TRUE) 
    {
        _MENU_DrawSeparateFrame(MENU_MAIN_MENU, hMenu, MENU_UP_SEPARATE_FRAME);
        _MENU_UpdateUpDownArraow(MENU_MAIN_MENU, hMenu, MENU_ARROW_TYPE_UP);
    }

    if (_MainMenuDisplay[hMenu].bDownMark == TRUE)
    {
        _MENU_DrawSeparateFrame(MENU_MAIN_MENU, hMenu, MENU_DOWN_SEPARATE_FRAME);
        _MENU_UpdateUpDownArraow(MENU_MAIN_MENU, hMenu, MENU_ARROW_TYPE_DOWN);
    }        
    
    //Draw String
    MENUStringAttr.bBackground = MENU_PALETTE_ENTRY_GENERAL_BACKGROUND; //PAL_ENTRY_COLOR_TRANSPARENT; //Set the color of background is the same as color key.
    MENUStringAttr.bColorKey = 0;
    MENUStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT; //Use OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND to prevent the "Open Folder" icon is wrong.
    MENUStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT; //Use OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND to prevent the "Open Folder" icon is wrong.
    
    if (_pMENUCurrentMenu[hMenu]->bTotalItem <= _MainMenuDisplay[hMenu].bMaxVisibleItem)
    {
        bDisplayItemNum = _pMENUCurrentMenu[hMenu]->bTotalItem;
    }
    else
    {
        bDisplayItemNum = _MainMenuDisplay[hMenu].bMaxVisibleItem;
    }

    for (bMenuIndex = 0; bMenuIndex < bDisplayItemNum; bMenuIndex++)
    {    
        if (_pMENUCurrentMenu[hMenu]->pMenuItem[bMenuIndex+(_MainMenuDisplay[hMenu].bStartDrawItem)].bAttribute == MENU_ITEM_STATE_DISABLE)
        {
            MENUStringAttr.bTextColor = MENU_PALETTE_ENTRY_ITEM_GRAYED;
        }
        else
        {
            MENUStringAttr.bTextColor = MENU_PALETTE_ENTRY_ITEM_TEXT;            
        }
        
        MENUStringAttr.wX = _MENU_GetStringStartH(MENU_MAIN_MENU, hMenu, _pMENUCurrentMenu[hMenu]->pMenuItem[bMenuIndex+(_MainMenuDisplay[hMenu].bStartDrawItem)].pwItemText);
        MENUStringAttr.wY = _pMENUCurrentMenu[hMenu]->wStartY + MENU_STRING_DISTANCE_V + bMenuIndex*MENU_ITEM_HEIGHT+((MENU_ITEM_HEIGHT-GDI_GetStringHeight(_pMENUCurrentMenu[hMenu]->pMenuItem[bMenuIndex+(_MainMenuDisplay[hMenu].bStartDrawItem)].pwItemText)) >> 1);
        GDI_DrawString_909(0, &MENUStringAttr, _pMENUCurrentMenu[hMenu]->pMenuItem[bMenuIndex+(_MainMenuDisplay[hMenu].bStartDrawItem)].pwItemText);

        if (_pMENUCurrentMenu[hMenu]->pMenuItem[bMenuIndex+(_MainMenuDisplay[hMenu].bStartDrawItem)].pSubMenu != NULL)
        {
            MENUStringAttr.wX = MENURect.rect.wRight-MENU_STRING_DISTANCE_H-GDI_GetStringWidth_909(aMENUSubMenuSymbol);
            MENUStringAttr.wY = _pMENUCurrentMenu[hMenu]->wStartY+MENU_STRING_DISTANCE_V+bMenuIndex*MENU_ITEM_HEIGHT+((MENU_ITEM_HEIGHT-GDI_GetStringHeight(aMENUSubMenuSymbol)) >> 1);
           GDI_DrawString_909(0, &MENUStringAttr, aMENUSubMenuSymbol);
        }
    }
}

void _MENU_DrawSeparateFrame(BYTE bMenuMode, HMENU hMenu, BYTE bFrameType)
{
    PARM_RECT MENURect;

    if (bMenuMode == MENU_MAIN_MENU)
    {
        _MENU_GetMainMenuDisplayRect(hMenu, &MENURect);
    }
    else if (bMenuMode == MENU_SUB_MENU)
    {
        _MENU_GetSubMenuDisplayRect(hMenu, &MENURect);
    }

    if (bFrameType == MENU_UP_SEPARATE_FRAME)
    {
        MENURect.rect.wTop = MENURect.rect.wTop+MENU_UP_DOWN_ITEM_HEIGHT;
    }
    else if (bFrameType == MENU_DOWN_SEPARATE_FRAME)
    {
        MENURect.rect.wTop = MENURect.rect.wBottom-MENU_UP_DOWN_ITEM_HEIGHT-MENU_SEPARATE_FRAME_THICKNESS+1;
    }

    MENURect.rect.wBottom = MENURect.rect.wTop+(MENU_SEPARATE_FRAME_THICKNESS >> 1) -1;
    MENURect.bColor = MENU_PALETTE_ENTRY_NORMAL_FRAME_DARK;   
    GDI_FillRect_909(0, &MENURect);          

    MENURect.rect.wTop = MENURect.rect.wBottom + 1;
    MENURect.rect.wBottom = MENURect.rect.wTop+(MENU_SEPARATE_FRAME_THICKNESS >> 1) -1;
    
    MENURect.bColor = MENU_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;   
    GDI_FillRect_909(0, &MENURect);          
}

void _MENU_UpdateUpDownArraow(BYTE bMenuMode, HMENU hMenu, BYTE bArrowType)
{
    PARM_DRAW_STRING MENUStringAttr;
    PARM_RECT MENURect;
    WORD wMenuWidth = 0;

    _MENU_SetCommonItemStringAttribute(&MENUStringAttr);

    if (bMenuMode == MENU_MAIN_MENU)
    {
        _MENU_GetMainMenuDisplayRect(hMenu, &MENURect);
        wMenuWidth = _MENU_GetMenuWidth(_pMENUCurrentMenu[hMenu]);
    }
    else if (bMenuMode == MENU_SUB_MENU)
    {
        _MENU_GetSubMenuDisplayRect(hMenu, &MENURect);
        wMenuWidth = _MENU_GetMenuWidth(_pMENUSubMenu[hMenu]);
    }

    if (bArrowType == MENU_ARROW_TYPE_UP)
    {
        MENUStringAttr.wX = MENURect.rect.wLeft+((wMenuWidth-GDI_GetStringWidth_909(aMENUUpArrowSymbol)) >> 1);
        MENUStringAttr.wY = MENURect.rect.wTop+((MENU_UP_DOWN_ITEM_HEIGHT-GDI_GetStringHeight_909(aMENUUpArrowSymbol)) >> 1);

        //Output the "up arraow"
        GDI_DrawString_909(0, &MENUStringAttr, aMENUUpArrowSymbol);
    }
    else if (bArrowType == MENU_ARROW_TYPE_DOWN)
    {
        MENUStringAttr.wX = MENURect.rect.wLeft+((wMenuWidth-GDI_GetStringWidth_909(aMENUDownArrowSymbol)) >> 1);
        MENUStringAttr.wY = MENURect.rect.wBottom-MENU_UP_DOWN_ITEM_HEIGHT+1+((MENU_UP_DOWN_ITEM_HEIGHT-GDI_GetStringHeight_909(aMENUDownArrowSymbol)) >> 1);

        //Output the "down arraow"
        GDI_DrawString_909(0, &MENUStringAttr, aMENUDownArrowSymbol);
    }
}

void _MENU_SetCommonItemStringAttribute(PARM_DRAW_STRING *pMENUStringAttr)
{
    pMENUStringAttr->bBackground = MENU_PALETTE_ENTRY_GENERAL_BACKGROUND; 
    pMENUStringAttr->bColorKey = 0;
    pMENUStringAttr->bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    pMENUStringAttr->bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    pMENUStringAttr->bTextColor = MENU_PALETTE_ENTRY_ITEM_TEXT; 
}

void _MENU_ClearUpDownArrowBackground(BYTE bMenuMode, HMENU hMenu, BYTE bArrowType)
{
    PARM_RECT MENURect;

    if (bMenuMode == MENU_MAIN_MENU)
    {
        _MENU_GetMainMenuDisplayRect(hMenu, &MENURect);
    }
    else if (bMenuMode == MENU_SUB_MENU)
    {
        _MENU_GetSubMenuDisplayRect(hMenu, &MENURect);
    }

    MENURect.rect.wLeft -= MENU_FRAME_THICKNESS;
    MENURect.rect.wRight += MENU_FRAME_THICKNESS;
    
    if (bArrowType == MENU_ARROW_TYPE_UP)
    {
        MENURect.rect.wBottom = MENURect.rect.wTop+MENU_UP_DOWN_ITEM_HEIGHT+MENU_SEPARATE_FRAME_THICKNESS-1;
        MENURect.rect.wTop = MENURect.rect.wTop-MENU_FRAME_THICKNESS;
    }
    else if (bArrowType == MENU_ARROW_TYPE_DOWN)
    {
        MENURect.rect.wTop = MENURect.rect.wBottom-MENU_UP_DOWN_ITEM_HEIGHT-MENU_SEPARATE_FRAME_THICKNESS+1;
        MENURect.rect.wBottom = MENURect.rect.wBottom+MENU_FRAME_THICKNESS;
    }

    MENURect.bColor = _bMENUBackgroundColor[hMenu];
   
    GDI_FillRect_909(0, &MENURect);          
}

void _MENU_SetSubMenuInitialDisplay(HMENU hMenu)
{
     //Sub-menu has "Up" mark?
    if (_pMENUSubMenu[hMenu]->bDefaultCursorPos > (_SubMenuDisplay[hMenu].bMaxVisibleItem-1))
    {
        _SubMenuDisplay[hMenu].bUpMark = TRUE;
        _SubMenuDisplay[hMenu].bStartDrawItem = _pMENUSubMenu[hMenu]->bDefaultCursorPos-_SubMenuDisplay[hMenu].bMaxVisibleItem+1;
    }
    else
    {
        _SubMenuDisplay[hMenu].bUpMark = FALSE;
        _SubMenuDisplay[hMenu].bStartDrawItem = 0;
    }

    //Sub-menu has "Down" mark?
    if ((_SubMenuDisplay[hMenu].bStartDrawItem + _SubMenuDisplay[hMenu].bMaxVisibleItem) < _pMENUSubMenu[hMenu]->bTotalItem)
    {
        _SubMenuDisplay[hMenu].bDownMark = TRUE;
    }
    else
    {
        _SubMenuDisplay[hMenu].bDownMark = FALSE;
    }            
}

void _MENU_SetMainMenuInitialDisplay(HMENU hMenu)
{
    if (_pMENUCurrentMenu[hMenu]->bDefaultCursorPos > (_MainMenuDisplay[hMenu].bMaxVisibleItem-1))
    {
        _MainMenuDisplay[hMenu].bUpMark = TRUE;
        _MainMenuDisplay[hMenu].bStartDrawItem = _pMENUCurrentMenu[hMenu]->bDefaultCursorPos-_MainMenuDisplay[hMenu].bMaxVisibleItem+1;
    }
    else
    {
        _MainMenuDisplay[hMenu].bUpMark = FALSE;
        _MainMenuDisplay[hMenu].bStartDrawItem = 0;
    }
    
    //Main menu has "Down" mark?
    if ((_MainMenuDisplay[hMenu].bStartDrawItem + _MainMenuDisplay[hMenu].bMaxVisibleItem) < _pMENUCurrentMenu[hMenu]->bTotalItem)
    {
        _MainMenuDisplay[hMenu].bDownMark = TRUE;
    }
    else
    {
        _MainMenuDisplay[hMenu].bDownMark = FALSE;
    }
}

BYTE _MENU_GetCountForUpItem(MENU *pMenu, BYTE bCursorIndex)
{
    BYTE bCount = 0;

    if (bCursorIndex == 0)
        return 0;

    while (bCursorIndex != 0)
    {
        bCursorIndex--;
        bCount++;

        if (pMenu->pMenuItem[bCursorIndex].bAttribute == MENU_ITEM_STATE_ENABLE)
            break;  
    }
    
    if (bCursorIndex == 0)
    {
        if (pMenu->pMenuItem[bCursorIndex].bAttribute == MENU_ITEM_STATE_DISABLE)
        {
            return 0;
        }
    }                    

    return bCount;
}

BYTE _MENU_GetCountForDownItem(MENU *pMenu, BYTE bCursorIndex)
{
    BYTE bCount = 0;

    if (bCursorIndex == (pMenu->bTotalItem-1))
        return 0;

    while (bCursorIndex != (pMenu->bTotalItem-1))
    {
        bCursorIndex++;
        bCount++;
        
        if (pMenu->pMenuItem[bCursorIndex].bAttribute == MENU_ITEM_STATE_ENABLE)
            break;  
    }

    if (bCursorIndex == (pMenu->bTotalItem-1))
    {
        if (pMenu->pMenuItem[bCursorIndex].bAttribute == MENU_ITEM_STATE_DISABLE)
        {
            return 0;
        }
    }                    

    return bCount;
}

void MENU_ClearMenuBackground(BYTE bMenuMode, HMENU hMenu)
{
    PARM_RECT MENURect;

    if (bMenuMode == MENU_MAIN_MENU)
    {
        _MENU_GetMainMenuDisplayRect(hMenu, &MENURect);
    }
    else if (bMenuMode == MENU_SUB_MENU)
    {
        _MENU_GetSubMenuDisplayRect(hMenu, &MENURect);
    }
  
    MENURect.bColor = _bMENUBackgroundColor[hMenu];
    MENURect.rect.wTop -= MENU_FRAME_THICKNESS;
    MENURect.rect.wBottom += MENU_FRAME_THICKNESS;
    MENURect.rect.wLeft -= MENU_FRAME_THICKNESS;
    MENURect.rect.wRight += MENU_FRAME_THICKNESS;
     
    GDI_FillRect(0, &MENURect);    
}

BYTE _MENU_CheckIfAnyItemHasSubMenu(MENU *pMenu)
{
    BYTE bMenuItemIndex;
    BYTE bSubMenuSymbol = FALSE;

    for (bMenuItemIndex = 0; bMenuItemIndex < pMenu->bTotalItem; bMenuItemIndex++)
    {
        if (pMenu->pMenuItem[bMenuItemIndex].pSubMenu != NULL)
        {
            bSubMenuSymbol = TRUE;
        }
    }
    
    return bSubMenuSymbol;
}

void _MENU_CopyString(void)
{
    BYTE i;

    for (i = 0; i < __pOSDUTLOutputString[0]+1; i++)
    {    
        __wMENUStringBuf[i] = __pOSDUTLOutputString[i];
    }
}

void MENU_ResetToDefaultState(HMENU hMenu)
{
    _pMENUSubMenu[hMenu] = (PMENU)(_pMENUCurrentMenu[hMenu]->pMenuItem[_pMENUCurrentMenu[hMenu]->bDefaultCursorPos].pSubMenu);
    
    //Set the cursor's position
    _Cursor[hMenu].bMainMenuIndex = _pMENUCurrentMenu[hMenu]->bDefaultCursorPos;
    _Cursor[hMenu].bPos = MENU_MAIN_MENU;
    
    if (_pMENUSubMenu[hMenu] == NULL)
    {
        _Cursor[hMenu].bSubMenuIndex = CURSOR_POS_INVALID;
    }
    else
    {
        _Cursor[hMenu].bSubMenuIndex = _pMENUSubMenu[hMenu]->bDefaultCursorPos;
    }
        
    //Main menu has "Up" mark?
    _MENU_SetMainMenuInitialDisplay(hMenu);

    if (_pMENUSubMenu[hMenu] != NULL)
    {
        //Sub-menu has "Up" mark?
        _MENU_SetSubMenuInitialDisplay(hMenu);
    }
}

BYTE MENU_GetCurrentItemID(HMENU hMenu)
{
    if (_Cursor[hMenu].bPos == MENU_MAIN_MENU)
    {
        return (_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].bID);
    }
    else
    {
        if (_pMENUSubMenu[hMenu]->bMenuType == MENU_TYPE_NUM)
        {
            return _Cursor[hMenu].bSubMenuIndex;
        }
        else
        {
            return (_pMENUSubMenu[hMenu]->pMenuItem[(_Cursor[hMenu].bSubMenuIndex)].bID);                
        }
    }
}

BYTE MENU_AllDisableItems(HMENU hMenu)
{
    BYTE bMenuIndex;

    for (bMenuIndex = 0; bMenuIndex < _pMENUCurrentMenu[hMenu]->bTotalItem; bMenuIndex++)
    {    
        if (_pMENUCurrentMenu[hMenu]->pMenuItem[bMenuIndex].bAttribute == MENU_ITEM_STATE_ENABLE)
            return FALSE;
    }

    return TRUE;
}

BYTE _MENU_AllDisableSubItems(HMENU hMenu)
{
    BYTE bMenuIndex;

    for (bMenuIndex = 0; bMenuIndex < _pMENUSubMenu[hMenu]->bTotalItem; bMenuIndex++)
    {    
        if (_pMENUSubMenu[hMenu]->pMenuItem[bMenuIndex].bAttribute == MENU_ITEM_STATE_ENABLE)
            return FALSE;
    }

    return TRUE;
}

BYTE _MENU_CheckMainMenuCursorState(HMENU hMenu)
{
    //Set the cursor's position
    if (_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].bAttribute == MENU_ITEM_STATE_ENABLE)
    {
        if ((PMENU)(_pMENUCurrentMenu[hMenu]->pMenuItem[(_Cursor[hMenu].bMainMenuIndex)].pSubMenu) == NULL)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    while (_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].bAttribute == MENU_ITEM_STATE_DISABLE)
    {
        if ((_Cursor[hMenu].bMainMenuIndex+1) < (_MainMenuDisplay[hMenu].bStartDrawItem+_MainMenuDisplay[hMenu].bMaxVisibleItem))
        {
            _Cursor[hMenu].bMainMenuIndex++;
        }
        else
        {
            break;
        }

        if (_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].bAttribute == MENU_ITEM_STATE_ENABLE)
        {
            _pMENUSubMenu[hMenu] = (PMENU)(_pMENUCurrentMenu[hMenu]->pMenuItem[(_Cursor[hMenu].bMainMenuIndex)].pSubMenu);
            
            if (_pMENUSubMenu[hMenu] == NULL)
            {
                _Cursor[hMenu].bSubMenuIndex = CURSOR_POS_INVALID;
            }
            else
            {
                _Cursor[hMenu].bSubMenuIndex = _pMENUSubMenu[hMenu]->bDefaultCursorPos;
                _MENU_SetSubMenuInitialDisplay(hMenu);
            }

            return TRUE;
        }
    }
    
    _pMENUSubMenu[hMenu] = NULL;
    _Cursor[hMenu].bSubMenuIndex = CURSOR_POS_INVALID;
    
    return FALSE;
}

#ifdef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS

void _MENU_GoToMainMenuItem(HMENU hMenu, BYTE bItem)
{
    BYTE bCount;

    //Clear current sub-menu.
    MENU_ClearMenuBackground(MENU_MAIN_MENU, hMenu);
    _MENU_ClearUpDownArrowBackground(MENU_MAIN_MENU, hMenu, MENU_ARROW_TYPE_UP);
    _MENU_ClearUpDownArrowBackground(MENU_MAIN_MENU, hMenu, MENU_ARROW_TYPE_DOWN);
    MENU_ClearMenuBackground(MENU_SUB_MENU, hMenu);

    if (bItem == MENU_FIRST_ITEM)
    {
        _Cursor[hMenu].bMainMenuIndex = 0;                                
        if (_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].bAttribute == MENU_ITEM_STATE_DISABLE)
        {
            bCount = _MENU_GetCountForDownItem(_pMENUCurrentMenu[hMenu], _Cursor[hMenu].bMainMenuIndex);
            _Cursor[hMenu].bMainMenuIndex += bCount;
        }
        
        if (_pMENUCurrentMenu[hMenu]->bTotalItem <= _MainMenuDisplay[hMenu].bMaxVisibleItem)
        {
            _MainMenuDisplay[hMenu].bStartDrawItem = 0;
            _MainMenuDisplay[hMenu].bUpMark = FALSE;
        }
        else
        {
            if (_Cursor[hMenu].bMainMenuIndex >= _MainMenuDisplay[hMenu].bMaxVisibleItem)
            {
                _MainMenuDisplay[hMenu].bStartDrawItem = _Cursor[hMenu].bMainMenuIndex;
            }
            else
            {
                _MainMenuDisplay[hMenu].bStartDrawItem = 0;
            }
        }
    }
    else if (bItem == MENU_LAST_ITEM)
    {
        _Cursor[hMenu].bMainMenuIndex = _pMENUCurrentMenu[hMenu]->bTotalItem-1;                
        if (_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].bAttribute == MENU_ITEM_STATE_DISABLE)
        {
            bCount = _MENU_GetCountForUpItem(_pMENUCurrentMenu[hMenu], _Cursor[hMenu].bMainMenuIndex);
            _Cursor[hMenu].bMainMenuIndex -= bCount;
        }
        
        if (_pMENUCurrentMenu[hMenu]->bTotalItem <= _MainMenuDisplay[hMenu].bMaxVisibleItem)
        {
            _MainMenuDisplay[hMenu].bStartDrawItem = 0;
            _MainMenuDisplay[hMenu].bUpMark = FALSE;
        }
        else
        {
            if (_Cursor[hMenu].bMainMenuIndex >= _MainMenuDisplay[hMenu].bMaxVisibleItem)
            {
                _MainMenuDisplay[hMenu].bStartDrawItem = _Cursor[hMenu].bMainMenuIndex - _MainMenuDisplay[hMenu].bMaxVisibleItem + 1;
            }
            else
            {
                _MainMenuDisplay[hMenu].bStartDrawItem = 0;
            }
        }
    }

    if (_MainMenuDisplay[hMenu].bStartDrawItem == 0)
    {
        //Clear "Up" mark
        _MainMenuDisplay[hMenu].bUpMark = FALSE;
    }
    else
    {
        _MainMenuDisplay[hMenu].bUpMark = TRUE;
    }
    
    if ((_MainMenuDisplay[hMenu].bStartDrawItem+_MainMenuDisplay[hMenu].bMaxVisibleItem) < _pMENUCurrentMenu[hMenu]->bTotalItem)
    {
        _MainMenuDisplay[hMenu].bDownMark = TRUE;
    }
    else
    {
        _MainMenuDisplay[hMenu].bDownMark = FALSE;
    }
    
    _MENU_ShowMainMenu(hMenu);
    
    MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);
    
    _pMENUSubMenu[hMenu] = _pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].pSubMenu;                
    //Show sub-menu
    if ((_pMENUSubMenu[hMenu] != NULL) && (_pMENUSubMenu[hMenu]->bTotalItem != 0))
    {
        _pMENUSubMenu[hMenu] = (PMENU)(_pMENUCurrentMenu[hMenu]->pMenuItem[_Cursor[hMenu].bMainMenuIndex].pSubMenu);
        _MENU_SetSubMenuInitialDisplay(hMenu);
        _Cursor[hMenu].bSubMenuIndex = _pMENUSubMenu[hMenu]->bDefaultCursorPos;
        _MENU_ShowSubMenu(hMenu);
    }
}

void _MENU_GoToSubMenuItem(HMENU hMenu, BYTE bItem)
{
    BYTE bCount;

    //Clear current sub-menu.
    MENU_ClearMenuBackground(MENU_SUB_MENU, hMenu);
    _MENU_ClearUpDownArrowBackground(MENU_SUB_MENU, hMenu, MENU_ARROW_TYPE_UP);
    _MENU_ClearUpDownArrowBackground(MENU_SUB_MENU, hMenu, MENU_ARROW_TYPE_DOWN);

    if (bItem == MENU_FIRST_ITEM)
    {
        _Cursor[hMenu].bSubMenuIndex = 0;                                
        if (_pMENUSubMenu[hMenu]->pMenuItem[_Cursor[hMenu].bSubMenuIndex].bAttribute == MENU_ITEM_STATE_DISABLE)
        {
            bCount = _MENU_GetCountForDownItem(_pMENUSubMenu[hMenu], _Cursor[hMenu].bSubMenuIndex);
            _Cursor[hMenu].bMainMenuIndex += bCount;
        }
        
        if (_pMENUSubMenu[hMenu]->bTotalItem <= _SubMenuDisplay[hMenu].bMaxVisibleItem)
        {
            _SubMenuDisplay[hMenu].bStartDrawItem = 0;
            _SubMenuDisplay[hMenu].bUpMark = FALSE;
        }
        else
        {
            if (_Cursor[hMenu].bSubMenuIndex >= _SubMenuDisplay[hMenu].bMaxVisibleItem)
            {
                _SubMenuDisplay[hMenu].bStartDrawItem = _Cursor[hMenu].bSubMenuIndex;
            }
            else
            {
                _SubMenuDisplay[hMenu].bStartDrawItem = 0;
            }
        }

    }
    else if (bItem == MENU_LAST_ITEM)
    {
        _Cursor[hMenu].bSubMenuIndex = _pMENUSubMenu[hMenu]->bTotalItem-1;                
        if (_pMENUSubMenu[hMenu]->pMenuItem[_Cursor[hMenu].bSubMenuIndex].bAttribute == MENU_ITEM_STATE_DISABLE)
        {
            bCount = _MENU_GetCountForUpItem(_pMENUSubMenu[hMenu], _Cursor[hMenu].bSubMenuIndex);
            _Cursor[hMenu].bSubMenuIndex -= bCount;
        }
        
        if (_pMENUSubMenu[hMenu]->bTotalItem <= _SubMenuDisplay[hMenu].bMaxVisibleItem)
        {
            _SubMenuDisplay[hMenu].bStartDrawItem = 0;
            _SubMenuDisplay[hMenu].bUpMark = FALSE;
        }
        else
        {
            if (_Cursor[hMenu].bSubMenuIndex >= _SubMenuDisplay[hMenu].bMaxVisibleItem)
            {
                _SubMenuDisplay[hMenu].bStartDrawItem = _Cursor[hMenu].bSubMenuIndex - _SubMenuDisplay[hMenu].bMaxVisibleItem + 1;
            }
            else
            {
                _SubMenuDisplay[hMenu].bStartDrawItem = 0;
            }
        }
    }

    if (_SubMenuDisplay[hMenu].bStartDrawItem == 0)
    {
        //Clear "Up" mark
        _SubMenuDisplay[hMenu].bUpMark = FALSE;
    }
    else
    {
        _SubMenuDisplay[hMenu].bUpMark = TRUE;
    }
    
    if ((_SubMenuDisplay[hMenu].bStartDrawItem+_SubMenuDisplay[hMenu].bMaxVisibleItem) < _pMENUSubMenu[hMenu]->bTotalItem)
    {
        _SubMenuDisplay[hMenu].bDownMark = TRUE;
    }
    else
    {
        _SubMenuDisplay[hMenu].bDownMark = FALSE;
    }

    _MENU_ShowSubMenu(hMenu);
    MENU_UpdateCursor(hMenu, CURSOR_STYLE_NORMAL);
}

#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS