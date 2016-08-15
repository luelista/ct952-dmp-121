#include "winav.h"
#ifdef SUPPORT_STB
#include "dvbcustm.h"
#endif
#include "utl.h"
#include "comutl.h"
#include "gdi.h"
#include "osddsply.h"
#include "osdnd.h"
#include "input.h"
#include "toolbar.h"

TOOLBAR *_pToolBar[MAX_SUPPORT_TOOLBAR_HANDLE_NUM];
HTOOLBAR _hToolBarStack[MAX_SUPPORT_TOOLBAR_HANDLE_NUM] = {TOOLBAR_HANDLE_FREE, TOOLBAR_HANDLE_FREE, TOOLBAR_HANDLE_FREE, TOOLBAR_HANDLE_FREE, TOOLBAR_HANDLE_FREE};
TOOLBAR_CURSOR _ToolBarCursor[MAX_SUPPORT_TOOLBAR_HANDLE_NUM];
BYTE _bTOOLBARBackgroundColor[MAX_SUPPORT_TOOLBAR_HANDLE_NUM];

//***************************************************************************
//  Function    :   _TOOLBAR_InitializeGDI
//  Abstract    :   Initialize the palette for the toolbar.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _TOOLBAR_InitializeGDI(void)
{
    GDI_ChangePALEntry(TOOLBAR_PALETTE_ENTRY_GENERAL_BACKGROUND, TOOLBAR_PALETTE_COLOR_GENERAL_BACKGROUND, FALSE);
    GDI_ChangePALEntry(TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT, TOOLBAR_PALETTE_COLOR_NORMAL_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_DARK, TOOLBAR_PALETTE_COLOR_NORMAL_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(TOOLBAR_PALETTE_ENTRY_SEPARATE_FRAME_CENTER, TOOLBAR_PALETTE_COLOR_SEPARATE_FRAME_CENTER, FALSE);
    GDI_ChangePALEntry(TOOLBAR_PALETTE_ENTRY_BIG_BACKGROUND, TOOLBAR_PALETTE_COLOR_BIG_BACKGROUND, FALSE);

#ifdef SUPPORT_TEXT_TOOLBAR
    GDI_ChangePALEntry(TOOLBAR_PALETTE_ENTRY_ICON_NORMAL_BACKGROUND, TOOLBAR_PALETTE_COLOR_ICON_NORMAL_BACKGROUND, FALSE);
    GDI_ChangePALEntry(TOOLBAR_PALETTE_ENTRY_ICON_HIGHLIGHT_BACKGROUND, TOOLBAR_PALETTE_COLOR_ICON_HIGHLIGHT_BACKGROUND, FALSE);
    GDI_ChangePALEntry(TOOLBAR_PALETTE_ENTRY_ICON_PRESSED_BACKGROUND, TOOLBAR_PALETTE_COLOR_ICON_PRESSED_BACKGROUND, FALSE);
#endif //SUPPORT_TEXT_TOOLBAR

    GDI_WaitPaletteComplete();
}

HTOOLBAR TOOLBAR_CreateToolBar(TOOLBAR *pToolBar)
{
    HTOOLBAR hToolBar;

    hToolBar = _TOOLBAR_GetToolBarHandle();

    if (hToolBar != TOOLBAR_HANDLE_INVALID)
    {
        _pToolBar[hToolBar] = pToolBar;
        _ToolBarCursor[hToolBar].bPos = 0;
        _ToolBarCursor[hToolBar].bType = ICON_CURSOR_TYPE_HIGHLIGHT;
    }

    return hToolBar;
}

void TOOLBAR_ShowToolBar(HTOOLBAR hToolBar, BYTE bToolBarBackgroundColor)
{
    BYTE bIconIndex;
    PARM_FRAME ToolBarFrame;
	PARM_RECT Rect;
#ifdef SUPPORT_TEXT_TOOLBAR
	PARM_DRAW_STRING IconStringAttr;
#else
    PARM_DRAW_BITMAP DrawBitmap;
#endif //SUPPORT_TEXT_TOOLBAR

#ifdef SUPPORT_TEXT_TOOLBAR
    _TOOLBAR_InitializeGDI();

    _bTOOLBARBackgroundColor[hToolBar] = bToolBarBackgroundColor;

    //Draw the big background for the icons
    Rect.bColor = TOOLBAR_PALETTE_ENTRY_BIG_BACKGROUND;
    Rect.rect.wLeft = _pToolBar[hToolBar]->wStartX-TOOLBAR_ICON_BACKGROUND_DISTANCE;
    Rect.rect.wTop = _pToolBar[hToolBar]->wStartY-TOOLBAR_ICON_BACKGROUND_DISTANCE;
    if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
    {
        Rect.rect.wRight = _pToolBar[hToolBar]->wStartX+_pToolBar[hToolBar]->bIconWidth+TOOLBAR_ICON_BACKGROUND_DISTANCE-1;
        Rect.rect.wBottom = _pToolBar[hToolBar]->wStartY+_pToolBar[hToolBar]->bTotalIcon*_pToolBar[hToolBar]->bIconHeight+TOOLBAR_ICON_ICON_DISTANCE*(_pToolBar[hToolBar]->bTotalIcon-1)+TOOLBAR_ICON_BACKGROUND_DISTANCE-1;
    }
    else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
    {
        Rect.rect.wRight = _pToolBar[hToolBar]->wStartX+_pToolBar[hToolBar]->bTotalIcon*_pToolBar[hToolBar]->bIconWidth+TOOLBAR_ICON_ICON_DISTANCE*(_pToolBar[hToolBar]->bTotalIcon-1)+TOOLBAR_ICON_BACKGROUND_DISTANCE-1;
        Rect.rect.wBottom = _pToolBar[hToolBar]->wStartY+_pToolBar[hToolBar]->bIconHeight+TOOLBAR_ICON_BACKGROUND_DISTANCE-1;
    }

    GDI_FillRect(0, &Rect);

    ToolBarFrame.rect.wLeft = Rect.rect.wLeft - TOOLBAR_FRAME_THICKNESS;
    ToolBarFrame.rect.wTop = Rect.rect.wTop-TOOLBAR_FRAME_THICKNESS;
    ToolBarFrame.rect.wRight = Rect.rect.wRight + TOOLBAR_FRAME_THICKNESS;
    ToolBarFrame.rect.wBottom = Rect.rect.wBottom + TOOLBAR_FRAME_THICKNESS;

    ToolBarFrame.bColorBright = TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
    ToolBarFrame.bColorDark = TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_DARK;    
    ToolBarFrame.bColorCenter = PAL_ENTRY_COLOR_TRANSPARENT;
    ToolBarFrame.bStyle = FRAME_POP;
    ToolBarFrame.bThickness = TOOLBAR_FRAME_THICKNESS;
    
    GDI_DrawFrame_909(0, &ToolBarFrame);

	IconStringAttr.bBackground = 1;
    IconStringAttr.bColorKey = 1;
    IconStringAttr.bShadow1 = 1;
    IconStringAttr.bShadow2 = 1;

	Rect.bColor = TOOLBAR_PALETTE_ENTRY_ICON_NORMAL_BACKGROUND;

	for (bIconIndex = 0; bIconIndex < _pToolBar[hToolBar]->bTotalIcon; bIconIndex++)
    {
		IconStringAttr.bTextColor = _pToolBar[hToolBar]->bIconNormalTextColor;

        if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
        {
			Rect.rect.wLeft = _pToolBar[hToolBar]->wStartX;
			Rect.rect.wTop = _pToolBar[hToolBar]->wStartY+bIconIndex*_pToolBar[hToolBar]->bIconHeight+bIconIndex*TOOLBAR_ICON_ICON_DISTANCE;

            IconStringAttr.wX = _pToolBar[hToolBar]->wStartX+((_pToolBar[hToolBar]->bIconWidth-GDI_GetStringWidth(_pToolBar[hToolBar]->pIcon[bIconIndex].pwString))>>1);
            IconStringAttr.wY = _pToolBar[hToolBar]->wStartY+bIconIndex*_pToolBar[hToolBar]->bIconHeight+bIconIndex*TOOLBAR_ICON_ICON_DISTANCE+((_pToolBar[hToolBar]->bIconHeight-GDI_GetStringHeight(_pToolBar[hToolBar]->pIcon[bIconIndex].pwString))>>1);
        }
        else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
        {
			Rect.rect.wLeft = _pToolBar[hToolBar]->wStartX+bIconIndex*_pToolBar[hToolBar]->bIconWidth+(bIconIndex)*TOOLBAR_ICON_ICON_DISTANCE;
			Rect.rect.wTop = _pToolBar[hToolBar]->wStartY;

            IconStringAttr.wX = _pToolBar[hToolBar]->wStartX+bIconIndex*_pToolBar[hToolBar]->bIconWidth+bIconIndex*TOOLBAR_ICON_ICON_DISTANCE+((_pToolBar[hToolBar]->bIconWidth-GDI_GetStringWidth(_pToolBar[hToolBar]->pIcon[bIconIndex].pwString))>>1);
            IconStringAttr.wY = _pToolBar[hToolBar]->wStartY+((_pToolBar[hToolBar]->bIconHeight-GDI_GetStringHeight(_pToolBar[hToolBar]->pIcon[bIconIndex].pwString))>>1);
        }

		Rect.rect.wRight = Rect.rect.wLeft+_pToolBar[hToolBar]->bIconWidth-1;
		Rect.rect.wBottom = Rect.rect.wTop+_pToolBar[hToolBar]->bIconHeight-1;
			
		GDI_FillRect(0, &Rect);		
		GDI_DrawString(0, &IconStringAttr, _pToolBar[hToolBar]->pIcon[bIconIndex].pwString);
	}

    TOOLBAR_UpdateCursor(hToolBar, _ToolBarCursor[hToolBar].bType);

    if (_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].Initial_Func_Ptr != NULL)
    {
        //Call the icon's "initial" callback function.
        _pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].Initial_Func_Ptr(_ToolBarCursor[hToolBar].bType);
    }
#else
    //Load palette for Navigaor's icons.
    GDI_PALETTE_INFO    PaletteInfo_ToolBar = {GDI_BITMAP_PALETTE_INDEX_START,  PAL_ADDR, _pToolBar[hToolBar]->pPalette};

    _TOOLBAR_InitializeGDI();

    _bTOOLBARBackgroundColor[hToolBar] = bToolBarBackgroundColor;

	GDI_LoadPalette(&PaletteInfo_ToolBar);
    GDI_LoadBMP(_pToolBar[hToolBar]->BMPArrayptr, _pToolBar[hToolBar]->bTotalIcon);

    //Draw the big background for the icons
    Rect.bColor = TOOLBAR_PALETTE_ENTRY_BIG_BACKGROUND;
    Rect.rect.wLeft = _pToolBar[hToolBar]->wStartX-TOOLBAR_ICON_BACKGROUND_DISTANCE;
    Rect.rect.wTop = _pToolBar[hToolBar]->wStartY-TOOLBAR_ICON_BACKGROUND_DISTANCE;
    if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
    {
        Rect.rect.wRight = _pToolBar[hToolBar]->wStartX+_pToolBar[hToolBar]->bIconWidth+TOOLBAR_ICON_BACKGROUND_DISTANCE-1;
        Rect.rect.wBottom = _pToolBar[hToolBar]->wStartY+_pToolBar[hToolBar]->bTotalIcon*_pToolBar[hToolBar]->bIconHeight+TOOLBAR_ICON_ICON_DISTANCE*(_pToolBar[hToolBar]->bTotalIcon-1)+TOOLBAR_ICON_BACKGROUND_DISTANCE-1;
    }
    else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
    {
        Rect.rect.wRight = _pToolBar[hToolBar]->wStartX+_pToolBar[hToolBar]->bTotalIcon*_pToolBar[hToolBar]->bIconWidth+TOOLBAR_ICON_ICON_DISTANCE*(_pToolBar[hToolBar]->bTotalIcon-1)+TOOLBAR_ICON_BACKGROUND_DISTANCE-1;
        Rect.rect.wBottom = _pToolBar[hToolBar]->wStartY+_pToolBar[hToolBar]->bIconHeight+TOOLBAR_ICON_BACKGROUND_DISTANCE-1;
    }

    GDI_FillRect(0, &Rect);

    ToolBarFrame.rect.wLeft = Rect.rect.wLeft - TOOLBAR_FRAME_THICKNESS;
    ToolBarFrame.rect.wTop = Rect.rect.wTop-TOOLBAR_FRAME_THICKNESS;
    ToolBarFrame.rect.wRight = Rect.rect.wRight + TOOLBAR_FRAME_THICKNESS;
    ToolBarFrame.rect.wBottom = Rect.rect.wBottom + TOOLBAR_FRAME_THICKNESS;

    ToolBarFrame.bColorBright = TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
    ToolBarFrame.bColorDark = TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_DARK;    
    ToolBarFrame.bColorCenter = PAL_ENTRY_COLOR_TRANSPARENT;
    ToolBarFrame.bStyle = FRAME_POP;
    ToolBarFrame.bThickness = TOOLBAR_FRAME_THICKNESS;
    
    GDI_DrawFrame_909(0, &ToolBarFrame);

    for (bIconIndex = 0; bIconIndex < _pToolBar[hToolBar]->bTotalIcon; bIconIndex++)
    {
        //Draw icon
        DrawBitmap.bBitmapID = bIconIndex;
        
        if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
        {
            DrawBitmap.wDestX = _pToolBar[hToolBar]->wStartX;
            DrawBitmap.wDestY = _pToolBar[hToolBar]->wStartY+bIconIndex*_pToolBar[hToolBar]->bIconHeight+bIconIndex*TOOLBAR_ICON_ICON_DISTANCE;
        }
        else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
        {
            DrawBitmap.wDestX = _pToolBar[hToolBar]->wStartX+bIconIndex*_pToolBar[hToolBar]->bIconWidth+bIconIndex*TOOLBAR_ICON_ICON_DISTANCE;
            DrawBitmap.wDestY = _pToolBar[hToolBar]->wStartY;
        }

        DrawBitmap.bDestRegion = 0;
        GDI_DrawBitmap(&DrawBitmap, _pToolBar[hToolBar]->BMPArrayptr);        
        GDI_ChangePALEntry(_pToolBar[hToolBar]->pIcon[bIconIndex].bBGPaletteIndex, TOOLBAR_PALETTE_COLOR_ICON_NORMAL_BACKGROUND, FALSE);
    }

    TOOLBAR_UpdateCursor(hToolBar, _ToolBarCursor[hToolBar].bType);

    if (_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].Initial_Func_Ptr != NULL)
    {
        //Call the icon's "initial" callback function.
        _pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].Initial_Func_Ptr(_ToolBarCursor[hToolBar].bType);
    }

#endif //SUPPORT_TEXT_TOOLBAR
}


HTOOLBAR _TOOLBAR_GetToolBarHandle(void)
{
    BYTE i;
    for (i = 0; i < MAX_SUPPORT_TOOLBAR_HANDLE_NUM; i++)
    {
        if (_hToolBarStack[i] == TOOLBAR_HANDLE_FREE)
        {
            _hToolBarStack[i] = i;
            return _hToolBarStack[i];
        }
    }

    return TOOLBAR_HANDLE_INVALID;
}

void TOOLBAR_UpdateCursor(HTOOLBAR hToolBar, BYTE bType)
{
#ifdef SUPPORT_TEXT_TOOLBAR
	PARM_DRAW_STRING IconStringAttr;
	PARM_RECT Rect;

	IconStringAttr.bBackground = 1;
    IconStringAttr.bColorKey = 1;
    IconStringAttr.bShadow1 = 1;
    IconStringAttr.bShadow2 = 1;

    //the background for the icon
    if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
    {
        Rect.rect.wLeft = _pToolBar[hToolBar]->wStartX;
        Rect.rect.wTop = _pToolBar[hToolBar]->wStartY+_ToolBarCursor[hToolBar].bPos*_pToolBar[hToolBar]->bIconHeight+_ToolBarCursor[hToolBar].bPos*TOOLBAR_ICON_ICON_DISTANCE;
    }
    else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
    {
        Rect.rect.wLeft = _pToolBar[hToolBar]->wStartX+_ToolBarCursor[hToolBar].bPos*_pToolBar[hToolBar]->bIconWidth+(_ToolBarCursor[hToolBar].bPos)*TOOLBAR_ICON_ICON_DISTANCE;
        Rect.rect.wTop = _pToolBar[hToolBar]->wStartY;
    }

	Rect.rect.wRight = Rect.rect.wLeft+_pToolBar[hToolBar]->bIconWidth-1;
	Rect.rect.wBottom = Rect.rect.wTop+_pToolBar[hToolBar]->bIconHeight-1;

    switch (bType)
    {
    case ICON_CURSOR_TYPE_NORMAL:
		Rect.bColor = TOOLBAR_PALETTE_ENTRY_ICON_NORMAL_BACKGROUND;
		IconStringAttr.bTextColor = _pToolBar[hToolBar]->bIconNormalTextColor;
        break;
    case ICON_CURSOR_TYPE_HIGHLIGHT:
		Rect.bColor = TOOLBAR_PALETTE_ENTRY_ICON_HIGHLIGHT_BACKGROUND;
        _ToolBarCursor[hToolBar].bType = ICON_CURSOR_TYPE_HIGHLIGHT;
		IconStringAttr.bTextColor = _pToolBar[hToolBar]->bIconHighlightTextColor;
        break;
    case ICON_CURSOR_TYPE_PRESSED:
		Rect.bColor = TOOLBAR_PALETTE_ENTRY_ICON_PRESSED_BACKGROUND;
        _ToolBarCursor[hToolBar].bType = ICON_CURSOR_TYPE_PRESSED;
		IconStringAttr.bTextColor = _pToolBar[hToolBar]->bIconPressedTextColor;
        break;
    default:
        break;
    }
	
	if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
	{
		IconStringAttr.wX = _pToolBar[hToolBar]->wStartX+((_pToolBar[hToolBar]->bIconWidth-GDI_GetStringWidth(_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].pwString))>>1);
		IconStringAttr.wY = _pToolBar[hToolBar]->wStartY+_ToolBarCursor[hToolBar].bPos*_pToolBar[hToolBar]->bIconHeight+_ToolBarCursor[hToolBar].bPos*TOOLBAR_ICON_ICON_DISTANCE+((_pToolBar[hToolBar]->bIconHeight-GDI_GetStringHeight(_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].pwString))>>1);
	}
	else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
	{
		IconStringAttr.wX = _pToolBar[hToolBar]->wStartX+_ToolBarCursor[hToolBar].bPos*_pToolBar[hToolBar]->bIconWidth+(_ToolBarCursor[hToolBar].bPos)*TOOLBAR_ICON_ICON_DISTANCE+((_pToolBar[hToolBar]->bIconWidth-GDI_GetStringWidth(_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].pwString))>>1);
		IconStringAttr.wY = _pToolBar[hToolBar]->wStartY+((_pToolBar[hToolBar]->bIconHeight-GDI_GetStringHeight(_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].pwString))>>1);		
	}

	GDI_FillRect(0, &Rect);				
	GDI_DrawString(0, &IconStringAttr, _pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].pwString);

#else
    switch (bType)
    {
    case ICON_CURSOR_TYPE_NORMAL:
        GDI_ChangePALEntry(_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].bBGPaletteIndex, TOOLBAR_PALETTE_COLOR_ICON_NORMAL_BACKGROUND, FALSE);
        break;
    case ICON_CURSOR_TYPE_HIGHLIGHT:
        _ToolBarCursor[hToolBar].bType = ICON_CURSOR_TYPE_HIGHLIGHT;
        GDI_ChangePALEntry(_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].bBGPaletteIndex, TOOLBAR_PALETTE_COLOR_ICON_HIGHLIGHT_BACKGROUND, FALSE);
        break;
    case ICON_CURSOR_TYPE_PRESSED:
        _ToolBarCursor[hToolBar].bType = ICON_CURSOR_TYPE_PRESSED;
        GDI_ChangePALEntry(_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].bBGPaletteIndex, TOOLBAR_PALETTE_COLOR_ICON_PRESSED_BACKGROUND, FALSE);
        break;
    default:
        break;
    }

#endif //SUPPORT_TEXT_TOOLBAR
    _TOOLBAR_DrawIconFrame(hToolBar, bType);
}

//***************************************************************************
//  Function    :   TOOLBAR_ProcessKey
//  Abstract    :   
//  Arguments   :   none.
//  Return      :   
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
WORD TOOLBAR_ProcessKey(HTOOLBAR hToolBar, BYTE bKey)
{
    if (_pToolBar[hToolBar] == NULL)
        return ICON_ACTOIN_NONE;

    //Return the ID of the menu item. The control flow can do the exact action according to the ID.
    switch (bKey)
    {
    case KEY_LEFT:
        if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
        {
            if (_ToolBarCursor[hToolBar].bPos != 0)
            {
                _TOOLBAR_IconAction(hToolBar, ICON_ACTOIN_PREVIOUS);
            }
        }
        break;
    case KEY_RIGHT:
        if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
        {
            if (_ToolBarCursor[hToolBar].bPos != (_pToolBar[hToolBar]->bTotalIcon-1))
            {
                _TOOLBAR_IconAction(hToolBar, ICON_ACTOIN_NEXT);                
            }
        }
        else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
        {       
			if (_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].bIconType == ICON_TYPE_POPMENU)
			{
				TOOLBAR_UpdateCursor(hToolBar, ICON_CURSOR_TYPE_PRESSED);
				return (ICON_ACTION_ACTIVATE_MENU << 8);
			}
        }
        break;
    case KEY_UP:
        if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
        {
            if (_ToolBarCursor[hToolBar].bPos != 0)
            {
                _TOOLBAR_IconAction(hToolBar, ICON_ACTOIN_PREVIOUS);
            }
#ifdef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
            else
            {
                //Go to the first icon
                _TOOLBAR_IconAction(hToolBar, ICON_ACTOIN_GO_TO_LAST);
            }
#endif
        }
		else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
		{
			//Move the cursor from menu to the icon

		}
        break;
    case KEY_DOWN:
        if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
        {
            if (_ToolBarCursor[hToolBar].bPos != (_pToolBar[hToolBar]->bTotalIcon-1))
            {
                _TOOLBAR_IconAction(hToolBar, ICON_ACTOIN_NEXT);
            }
#ifdef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
            else
            {
                //Go to the first icon
                _TOOLBAR_IconAction(hToolBar, ICON_ACTOIN_GO_TO_FIRST);
            }
#endif
        }
		else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
		{
			if (_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].bIconType == ICON_TYPE_POPMENU)
			{
				TOOLBAR_UpdateCursor(hToolBar, ICON_CURSOR_TYPE_PRESSED);
				return (ICON_ACTION_ACTIVATE_MENU << 8);
			}
		}
        break;
    case KEY_PLAY:
    case KEY_ENTER:
        if (_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].bIconType == ICON_TYPE_BUTTON)
        {
            return ((ICON_ACTION_PROCESS_ICON << 8 ) | _pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].bIconID);
        }
        break;
    default:
        break;
    }

    return ICON_ACTOIN_NONE;
}

void _TOOLBAR_IconAction(HTOOLBAR hToolBar, BYTE bAction)
{
#ifdef SUPPORT_TEXT_TOOLBAR
	TOOLBAR_UpdateCursor(hToolBar, ICON_CURSOR_TYPE_NORMAL);
#else
    //Clear the highlight
    GDI_ChangePALEntry(_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].bBGPaletteIndex, TOOLBAR_PALETTE_COLOR_ICON_NORMAL_BACKGROUND, FALSE);
#endif //SUPPORT_TEXT_TOOLBAR

    if (_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].Exit_Func_Ptr != NULL)
    {
        //Call the icon's "Exit" callback function.
        _pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].Exit_Func_Ptr();
    }

    _TOOLBAR_DrawIconFrame(hToolBar, ICON_CURSOR_TYPE_NORMAL);

    if (bAction == ICON_ACTOIN_PREVIOUS)
    {
        _ToolBarCursor[hToolBar].bPos--;
    }
    else if (bAction == ICON_ACTOIN_NEXT)
    {
        _ToolBarCursor[hToolBar].bPos++;
    }
#ifdef SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS
    else if (bAction == ICON_ACTOIN_GO_TO_FIRST)
    {
        _ToolBarCursor[hToolBar].bPos = 0;
    }
    else if (bAction == ICON_ACTOIN_GO_TO_LAST)
    {
        _ToolBarCursor[hToolBar].bPos = _pToolBar[hToolBar]->bTotalIcon-1;
    }
#endif //SETUP_KEY_UP_DOWN_TOGGLE_OPTIONS

    //Show highlight
#ifdef SUPPORT_TEXT_TOOLBAR
	TOOLBAR_UpdateCursor(hToolBar, ICON_CURSOR_TYPE_HIGHLIGHT);
#else
    GDI_ChangePALEntry(_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].bBGPaletteIndex, TOOLBAR_PALETTE_COLOR_ICON_HIGHLIGHT_BACKGROUND, FALSE);
#endif //SUPPORT_TEXT_TOOLBAR

    if (_pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].Initial_Func_Ptr != NULL)
    {
        //Call the icon's "initial" callback function.
        _pToolBar[hToolBar]->pIcon[(_ToolBarCursor[hToolBar].bPos)].Initial_Func_Ptr(_ToolBarCursor[hToolBar].bType);
    }

    _TOOLBAR_DrawIconFrame(hToolBar, ICON_CURSOR_TYPE_HIGHLIGHT);
}

void TOOLBAR_DeleteToolBar(HTOOLBAR hToolBar, BYTE bClearBackground)
{
    BYTE i;
    for (i = 0; i < MAX_SUPPORT_TOOLBAR_HANDLE_NUM; i++)
    {
        if (_hToolBarStack[i] == hToolBar)
        {
            _hToolBarStack[i] = TOOLBAR_HANDLE_FREE;

            //clear the background
            if (bClearBackground)
            {
                _TOOLBAR_ClearBackground(hToolBar);
            }

            return;
        }
    }
}

void _TOOLBAR_DrawIconFrame(HTOOLBAR hToolBar, BYTE bType)
{
    PARM_FRAME IconFrame;
    
    if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
    {
        IconFrame.rect.wTop = _pToolBar[hToolBar]->wStartY+_ToolBarCursor[hToolBar].bPos*_pToolBar[hToolBar]->bIconHeight+_ToolBarCursor[hToolBar].bPos*TOOLBAR_ICON_ICON_DISTANCE-TOOLBAR_ICON_FRAME_THICKNESS;
        IconFrame.rect.wLeft = _pToolBar[hToolBar]->wStartX-TOOLBAR_ICON_FRAME_THICKNESS;
    }
    else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
    {
		IconFrame.rect.wTop = _pToolBar[hToolBar]->wStartY-TOOLBAR_ICON_FRAME_THICKNESS;
        IconFrame.rect.wLeft = _pToolBar[hToolBar]->wStartX+_ToolBarCursor[hToolBar].bPos*_pToolBar[hToolBar]->bIconWidth+_ToolBarCursor[hToolBar].bPos*TOOLBAR_ICON_ICON_DISTANCE-TOOLBAR_ICON_FRAME_THICKNESS;
    }

    IconFrame.rect.wRight = IconFrame.rect.wLeft+_pToolBar[hToolBar]->bIconWidth+TOOLBAR_ICON_FRAME_THICKNESS*2;
    IconFrame.rect.wBottom = IconFrame.rect.wTop+_pToolBar[hToolBar]->bIconHeight+TOOLBAR_ICON_FRAME_THICKNESS*2;

    if (bType == ICON_CURSOR_TYPE_HIGHLIGHT)
    {
        IconFrame.bColorBright = TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
        IconFrame.bColorDark = TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_DARK;    
        IconFrame.bStyle = FRAME_POP;
    }
    else if (bType == ICON_CURSOR_TYPE_PRESSED)
    {
        IconFrame.bColorBright = TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
        IconFrame.bColorDark = TOOLBAR_PALETTE_ENTRY_NORMAL_FRAME_DARK;    
        IconFrame.bStyle = FRAME_PUSH;
    }
    else
    {
        IconFrame.bColorBright = TOOLBAR_PALETTE_ENTRY_BIG_BACKGROUND;
        IconFrame.bColorDark = TOOLBAR_PALETTE_ENTRY_BIG_BACKGROUND;    
        IconFrame.bStyle = FRAME_POP;
    }
    IconFrame.bColorCenter = TOOLBAR_PALETTE_ENTRY_SEPARATE_FRAME_CENTER;
    IconFrame.bThickness = TOOLBAR_ICON_FRAME_THICKNESS;
    
    GDI_DrawFrame_909(0, &IconFrame);
}


void _TOOLBAR_ClearBackground(HTOOLBAR hToolBar)
{
    PARM_RECT Rect;

    Rect.bColor = _bTOOLBARBackgroundColor[hToolBar];
    Rect.rect.wLeft = _pToolBar[hToolBar]->wStartX-TOOLBAR_ICON_BACKGROUND_DISTANCE-TOOLBAR_FRAME_THICKNESS;
    Rect.rect.wTop = _pToolBar[hToolBar]->wStartY-TOOLBAR_ICON_BACKGROUND_DISTANCE-TOOLBAR_FRAME_THICKNESS;
    if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_VERTICAL)
    {
        Rect.rect.wRight = _pToolBar[hToolBar]->wStartX+_pToolBar[hToolBar]->bIconWidth+TOOLBAR_ICON_BACKGROUND_DISTANCE+TOOLBAR_FRAME_THICKNESS-1;
        Rect.rect.wBottom = _pToolBar[hToolBar]->wStartY+_pToolBar[hToolBar]->bTotalIcon*_pToolBar[hToolBar]->bIconHeight+TOOLBAR_ICON_ICON_DISTANCE*(_pToolBar[hToolBar]->bTotalIcon-1)+TOOLBAR_ICON_BACKGROUND_DISTANCE+TOOLBAR_FRAME_THICKNESS-1;
    }
    else if (_pToolBar[hToolBar]->bStyle == TOOLBAR_STYLE_HORIZONTAL)
    {
        Rect.rect.wRight = _pToolBar[hToolBar]->wStartX+_pToolBar[hToolBar]->bTotalIcon*_pToolBar[hToolBar]->bIconWidth+TOOLBAR_ICON_ICON_DISTANCE*(_pToolBar[hToolBar]->bTotalIcon-1)+TOOLBAR_ICON_BACKGROUND_DISTANCE+TOOLBAR_FRAME_THICKNESS-1;
        Rect.rect.wBottom = _pToolBar[hToolBar]->wStartY+_pToolBar[hToolBar]->bIconHeight+TOOLBAR_ICON_BACKGROUND_DISTANCE+TOOLBAR_FRAME_THICKNESS-1;
    }

    GDI_FillRect(0, &Rect);
}
