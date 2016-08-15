#define DIALOG_MAIN_FILE

#include "winav.h"
#ifdef SUPPORT_STB
#include "dvbcustm.h"
#endif
#include "utl.h"
#include "gdi.h"
#include "osd.h"
#include "osddsply.h"
#include "osdnd.h"
#include "input.h"
#ifdef SUPPORT_STB
#include "sysmenu.h"
#include "dvdsetup.h"
#endif //SUPPORT_STB
#include "mainmenu.h"
#include "dialog.h"

DIALOG *_pDIALOGCurrentDialog[MAX_SUPPORT_DIALOG_HANDLE_NUM] = {NULL, NULL, NULL, NULL, NULL};
HDIALOG _hDIALOGStack[MAX_SUPPORT_DIALOG_HANDLE_NUM] = {DIALOG_HANDLE_FREE, DIALOG_HANDLE_FREE, DIALOG_HANDLE_FREE, DIALOG_HANDLE_FREE, DIALOG_HANDLE_FREE};
BYTE _bDIALOGCursor[MAX_SUPPORT_DIALOG_HANDLE_NUM];
BYTE _bDIALOGBackgroundColor[MAX_SUPPORT_DIALOG_HANDLE_NUM];

extern BYTE _bOSDLang;
BYTE _bDIALOGUIStyle[MAX_SUPPORT_DIALOG_HANDLE_NUM];
//***************************************************************************
//  Function    :   DIALOG_InitializeGDI
//  Abstract    :   Initialize the palette, etc.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void DIALOG_InitializeGDI(BYTE bUIStyle)
{
	switch (bUIStyle)
	{
    case 1:
		MAINMENU_LoadGroupPalette(DIALOG_PALETTE_ENTRY_NORMAL_BUTTON_BEGIN, _dwDIALOGNormalButtonPalette2, FALSE);
		MAINMENU_LoadGroupPalette(DIALOG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BEGIN, _dwDIALOGHighlightButtonPalette2, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_BACKGROUND, DIALOG_PALETTE_COLOR_TITLE_BACKGROUND_2, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND, DIALOG_PALETTE_COLOR_CONTENT_BACKGROUND_2, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_CONTENT_TEXT, DIALOG_PALETTE_COLOR_CONTENT_TEXT_2, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_SEPARATOR_PALETTE0_1, DIALOG_PALETTE_COLOR_TITLE_SEPARATOR_PALETTE0_2, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_SEPARATOR_PALETTE1_1, DIALOG_PALETTE_COLOR_TITLE_SEPARATOR_PALETTE1_2, FALSE);
        break;
    case 2:
		MAINMENU_LoadGroupPalette(DIALOG_PALETTE_ENTRY_NORMAL_BUTTON_BEGIN, _dwDIALOGNormalButtonPalette3, FALSE);
		MAINMENU_LoadGroupPalette(DIALOG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BEGIN, _dwDIALOGHighlightButtonPalette3, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_BACKGROUND, DIALOG_PALETTE_COLOR_TITLE_BACKGROUND_3, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND, DIALOG_PALETTE_COLOR_CONTENT_BACKGROUND_3, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_CONTENT_TEXT, DIALOG_PALETTE_COLOR_CONTENT_TEXT_3, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_SEPARATOR_PALETTE0_1, DIALOG_PALETTE_COLOR_TITLE_SEPARATOR_PALETTE0_3, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_SEPARATOR_PALETTE1_1, DIALOG_PALETTE_COLOR_TITLE_SEPARATOR_PALETTE1_3, FALSE);
        break;
    case 3:
		MAINMENU_LoadGroupPalette(DIALOG_PALETTE_ENTRY_NORMAL_BUTTON_BEGIN, _dwDIALOGNormalButtonPalette4, FALSE);
		MAINMENU_LoadGroupPalette(DIALOG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BEGIN, _dwDIALOGHighlightButtonPalette4, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_BACKGROUND, DIALOG_PALETTE_COLOR_TITLE_BACKGROUND_4, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND, DIALOG_PALETTE_COLOR_CONTENT_BACKGROUND_4, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_CONTENT_TEXT, DIALOG_PALETTE_COLOR_CONTENT_TEXT_4, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_SEPARATOR_PALETTE0_1, DIALOG_PALETTE_COLOR_TITLE_SEPARATOR_PALETTE0_4, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_SEPARATOR_PALETTE1_1, DIALOG_PALETTE_COLOR_TITLE_SEPARATOR_PALETTE1_4, FALSE);
        break;
	case 0:
	default:
		MAINMENU_LoadGroupPalette(DIALOG_PALETTE_ENTRY_NORMAL_BUTTON_BEGIN, _dwDIALOGNormalButtonPalette1, FALSE);
		MAINMENU_LoadGroupPalette(DIALOG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BEGIN, _dwDIALOGHighlightButtonPalette1, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_BACKGROUND, DIALOG_PALETTE_COLOR_TITLE_BACKGROUND_1, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND, DIALOG_PALETTE_COLOR_CONTENT_BACKGROUND_1, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_CONTENT_TEXT, DIALOG_PALETTE_COLOR_CONTENT_TEXT_1, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_SEPARATOR_PALETTE0_1, DIALOG_PALETTE_COLOR_TITLE_SEPARATOR_PALETTE0_1, FALSE);
		GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_SEPARATOR_PALETTE1_1, DIALOG_PALETTE_COLOR_TITLE_SEPARATOR_PALETTE1_1, FALSE);
		break;
	}

    GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_TITLE_TEXT, DIALOG_PALETTE_COLOR_TITLE_TEXT, FALSE);
    GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_FRAME, DIALOG_PALETTE_COLOR_FRAME, FALSE);
    GDI_ChangePALEntry(DIALOG_PALETTE_ENTRY_BUTTON_STRING, DIALOG_PALETTE_COLOR_BUTTON_STRING, FALSE);

    GDI_WaitPaletteComplete();
}

HDIALOG DIALOG_CreateDialog(DIALOG *pDialog, BYTE bUIStyle)
{
    HDIALOG hDialog;

    hDialog = _DIALOG_GetDialogHandle();

    if (hDialog != DIALOG_HANDLE_INVALID)
    {
        _pDIALOGCurrentDialog[hDialog] = pDialog;
        
        if (pDialog->pButton != NULL)
        {
            _bDIALOGCursor[hDialog] = pDialog->bDefaultButtonID;
        }

        _bDIALOGUIStyle[hDialog] = bUIStyle;

        DIALOG_InitializeGDI(bUIStyle);
    }

    return hDialog;
}

void DIALOG_ShowDialog(HDIALOG hDialog, BYTE bBackgroundColor, BYTE bShowCursor)
{
    PARM_RECT Rect;
    PARM_FRAME Frame;
    PARM_DRAW_STRING TitleStringAttr;
    BYTE bButtonIndex;

    _bDIALOGBackgroundColor[hDialog] = bBackgroundColor;

    //Fill title rectangle
    Rect.bColor = DIALOG_PALETTE_ENTRY_TITLE_BACKGROUND;
    Rect.rect.wLeft = _pDIALOGCurrentDialog[hDialog]->wStartX;
    Rect.rect.wTop = _pDIALOGCurrentDialog[hDialog]->wStartY;
    Rect.rect.wRight = Rect.rect.wLeft+_pDIALOGCurrentDialog[hDialog]->wWidth-1;
    Rect.rect.wBottom = Rect.rect.wTop+DIALOG_TITLE_HEIGHT-1;

    switch (_bDIALOGUIStyle[hDialog])
    {
    case 1:
    case 2:
    case 3:
        GDI_FillRoundRectTop(0, DIALOG_TITLE_REGION_ARC, &Rect);
        break;
    case 0:
    default:
        GDI_FillRect(0, &Rect);
        break;
    }

    //Draw the separator
    Rect.bColor = DIALOG_SEPARATOR_HEIGHT;
    Rect.bShadePtr = _bDIALOGSeparatorPalette1;
    Rect.rect.wTop = Rect.rect.wBottom+1;
    Rect.rect.wBottom = Rect.rect.wTop+DIALOG_SEPARATOR_HEIGHT-1;
    GDI_FillRect(0|GDI_SHADING_OPERATION, &Rect);

    //Draw the background of the content
    Rect.bColor = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    Rect.rect.wTop = Rect.rect.wBottom+1;
    Rect.rect.wBottom = _pDIALOGCurrentDialog[hDialog]->wStartY+_pDIALOGCurrentDialog[hDialog]->wHeight-1;

    switch (_bDIALOGUIStyle[hDialog])
    {
    case 1:
    case 2:
    case 3:
        GDI_FillRoundRectBottom(0, DIALOG_CONTENT_REGION_ARC, &Rect);
        break;
    case 0:
    default:
        GDI_FillRect(0, &Rect);
        //Draw the frame
        Frame.bColorBright = DIALOG_PALETTE_ENTRY_FRAME;
        Frame.bColorDark = DIALOG_PALETTE_ENTRY_FRAME;
        Frame.bColorCenter = DIALOG_PALETTE_ENTRY_FRAME; //"FRAME_DOUBLE" doesn't need to assign the center color?
        Frame.bStyle = FRAME_DOUBLE;
        Frame.bThickness = DIALOG_FRAME_THICKNESS >> 1;
        Frame.rect.wLeft = _pDIALOGCurrentDialog[hDialog]->wStartX;
        Frame.rect.wTop = _pDIALOGCurrentDialog[hDialog]->wStartY;
        Frame.rect.wRight = _pDIALOGCurrentDialog[hDialog]->wStartX+_pDIALOGCurrentDialog[hDialog]->wWidth-1;
        Frame.rect.wBottom = _pDIALOGCurrentDialog[hDialog]->wStartY+_pDIALOGCurrentDialog[hDialog]->wHeight-1;
        GDI_DrawFrame_909(0, &Frame);
        break;
    }
    
    //Draw the title string
    TitleStringAttr.bBackground = DIALOG_PALETTE_ENTRY_TITLE_BACKGROUND;
    TitleStringAttr.bColorKey = 0;
    TitleStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    TitleStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    TitleStringAttr.bTextColor = DIALOG_PALETTE_ENTRY_TITLE_TEXT;
    TitleStringAttr.wY = _pDIALOGCurrentDialog[hDialog]->wStartY+((DIALOG_TITLE_HEIGHT-GDI_GetStringHeight(_pDIALOGCurrentDialog[hDialog]->pwTitleText)) >> 1);        
    
    switch (_pDIALOGCurrentDialog[hDialog]->bTitleAlignment)
    {
    case DIALOG_TITLE_STRING_ALIGNMENT_LEFT:
        if (_bDIALOGCursor[hDialog] == 0)
        {
            TitleStringAttr.wX = _pDIALOGCurrentDialog[hDialog]->wStartX+DIALOG_TITLE_BACKGROUND_STRING_DISTANCE_H+DIALOG_FRAME_THICKNESS;
        }
        else
        {
            TitleStringAttr.wX = _pDIALOGCurrentDialog[hDialog]->wStartX+DIALOG_TITLE_BACKGROUND_STRING_DISTANCE_H;
        }
        break;
    case DIALOG_TITLE_STRING_ALIGNMENT_CENTER:
        if (_bDIALOGCursor[hDialog] == 0)
        {
            TitleStringAttr.wX = _pDIALOGCurrentDialog[hDialog]->wStartX+((_pDIALOGCurrentDialog[hDialog]->wWidth-DIALOG_FRAME_THICKNESS*2-GDI_GetStringWidth(_pDIALOGCurrentDialog[hDialog]->pwTitleText))>>1);
        }
        else
        {
            TitleStringAttr.wX = _pDIALOGCurrentDialog[hDialog]->wStartX+((_pDIALOGCurrentDialog[hDialog]->wWidth-GDI_GetStringWidth(_pDIALOGCurrentDialog[hDialog]->pwTitleText))>>1);
        }
        break;
    case DIALOG_TITLE_STRING_ALIGNMENT_RIGHT:
        if (_bDIALOGCursor[hDialog] == 0)
        {
            TitleStringAttr.wX = _pDIALOGCurrentDialog[hDialog]->wStartX+_pDIALOGCurrentDialog[hDialog]->wWidth-DIALOG_FRAME_THICKNESS-DIALOG_TITLE_BACKGROUND_STRING_DISTANCE_H-GDI_GetStringWidth(_pDIALOGCurrentDialog[hDialog]->pwTitleText);
        }
        else
        {
            TitleStringAttr.wX = _pDIALOGCurrentDialog[hDialog]->wStartX+_pDIALOGCurrentDialog[hDialog]->wWidth-DIALOG_TITLE_BACKGROUND_STRING_DISTANCE_H-GDI_GetStringWidth(_pDIALOGCurrentDialog[hDialog]->pwTitleText);
        }
        break;
    default:
        return;
    }

    GDI_DrawString_909(0, &TitleStringAttr, _pDIALOGCurrentDialog[hDialog]->pwTitleText);
    
    //Draw the button
    for (bButtonIndex = 0; bButtonIndex <  _pDIALOGCurrentDialog[hDialog]->bButtonNum; bButtonIndex++)
    {
        if (bShowCursor && (_pDIALOGCurrentDialog[hDialog]->pButton[bButtonIndex].bID == _pDIALOGCurrentDialog[hDialog]->bDefaultButtonID))
        {
            _DIALOG_DrawButton(hDialog, bButtonIndex, DIALOG_BUTTON_TYPE_HIGHLIGHT);
        }
        else
        {
            _DIALOG_DrawButton(hDialog, bButtonIndex, DIALOG_BUTTON_TYPE_NORMAL);
        }
    }
}

void _DIALOG_DrawButton(HDIALOG hDialog, BYTE bButtonIndex, BYTE bType)
{
    PARM_RECT Rect;
    PARM_DRAW_STRING ButtonStringAttr;

    Rect.bColor = DIALOG_BUTTON_HEIGHT;
    Rect.rect.wLeft = _DIALOG_GetButtonStartX(hDialog, bButtonIndex);
    Rect.rect.wTop = _DIALOG_GetButtonStartY(hDialog);
    Rect.rect.wRight = Rect.rect.wLeft+_DIALOG_GetButtonWidth(hDialog)-1;
    Rect.rect.wBottom = Rect.rect.wTop+DIALOG_BUTTON_HEIGHT-1;
    
    if (bType == DIALOG_BUTTON_TYPE_NORMAL)
    {
        switch (_bDIALOGUIStyle[hDialog])
        {
        case 1:
            Rect.bShadePtr = _bDIALOGNormalButtonPalette2;
            GDI_FillRoundRect(0|GDI_SHADING_OPERATION, DIALOG_BUTTON_ARC, &Rect);
            break;
        case 2:
            Rect.bShadePtr = _bDIALOGNormalButtonPalette3;
            GDI_FillRect(0|GDI_SHADING_OPERATION, &Rect);
            break;
        case 3:
            Rect.bShadePtr = _bDIALOGNormalButtonPalette4;
            GDI_FillRoundRect(0|GDI_SHADING_OPERATION, DIALOG_BUTTON_ARC, &Rect);
            break;
        case 0:
        default:       
            Rect.bShadePtr = _bDIALOGNormalButtonPalette1;
            GDI_FillRect(0|GDI_SHADING_OPERATION, &Rect);
            break;
        }
    }
    else if (bType == DIALOG_BUTTON_TYPE_HIGHLIGHT)
    {
        switch (_bDIALOGUIStyle[hDialog])
        {
        case 1:
            Rect.bShadePtr = _bDIALOGHighlightButtonPalette2;
            GDI_FillRoundRect(0|GDI_SHADING_OPERATION, DIALOG_BUTTON_ARC, &Rect);
            break;
        case 2:
            Rect.bShadePtr = _bDIALOGHighlightButtonPalette3;
            GDI_FillRect(0|GDI_SHADING_OPERATION, &Rect);
            break;
        case 3:
            Rect.bShadePtr = _bDIALOGHighlightButtonPalette4;
            GDI_FillRoundRect(0|GDI_SHADING_OPERATION, DIALOG_BUTTON_ARC, &Rect);
            break;
        case 0:
        default:       
            Rect.bShadePtr = _bDIALOGHighlightButtonPalette1;
            GDI_FillRect(0|GDI_SHADING_OPERATION, &Rect);
            break;
        }
    }

    //Draw the button string
    ButtonStringAttr.bBackground = 1;
    ButtonStringAttr.bColorKey = 1;
    ButtonStringAttr.bShadow1 = 1;
    ButtonStringAttr.bShadow2 = 1;
    ButtonStringAttr.bTextColor = DIALOG_PALETTE_ENTRY_BUTTON_STRING;
    
    ButtonStringAttr.wX = _DIALOG_GetButtonStartX(hDialog, bButtonIndex)+((_DIALOG_GetButtonWidth(hDialog)-GDI_GetStringWidth_909(_pDIALOGCurrentDialog[hDialog]->pButton[bButtonIndex].pwButtonText))>>1);
    ButtonStringAttr.wY = _DIALOG_GetButtonStartY(hDialog)+((DIALOG_BUTTON_HEIGHT-GDI_GetStringHeight_909(_pDIALOGCurrentDialog[hDialog]->pButton[bButtonIndex].pwButtonText))>>1);
    GDI_DrawString_909(0, &ButtonStringAttr, _pDIALOGCurrentDialog[hDialog]->pButton[bButtonIndex].pwButtonText);
}

BYTE _DIALOG_GetButtonWidth(HDIALOG hDialog)
{
    BYTE bButtonIndex;
    BYTE bMaxButtonWidth = 0;
    BYTE bButtonStringWidth = 0;

    if (_pDIALOGCurrentDialog[hDialog]->bButtonWidth == DIALOG_BUTTON_WIDTH_OPTIMIZED)
    {
        for (bButtonIndex = 0; bButtonIndex < _pDIALOGCurrentDialog[hDialog]->bButtonNum; bButtonIndex++)
        {
            bButtonStringWidth = GDI_GetStringWidth_909(_pDIALOGCurrentDialog[hDialog]->pButton[bButtonIndex].pwButtonText);
            
            if (bMaxButtonWidth < bButtonStringWidth)
            {
                bMaxButtonWidth = bButtonStringWidth;
            }
        }
        
        bMaxButtonWidth += (DIALOG_BUTTON_STRING_OPTIMIZED_DISTANCE_H*2);
    }
    else
    {
        return _pDIALOGCurrentDialog[hDialog]->bButtonWidth;
    }

    return bMaxButtonWidth;
}

WORD _DIALOG_GetButtonStartX(HDIALOG hDialog, BYTE bButtonIndex)
{
    WORD wStartX;

    if (_pDIALOGCurrentDialog[hDialog]->pButton[bButtonIndex].wStartX == DIALOG_BUTTON_START_X_OPTIMIZED)
    {
        wStartX = _pDIALOGCurrentDialog[hDialog]->wStartX+((_pDIALOGCurrentDialog[hDialog]->wWidth - DIALOG_BUTTON_BUTTON_DISTANCE*(_pDIALOGCurrentDialog[hDialog]->bButtonNum-1)-_DIALOG_GetButtonWidth(hDialog)*_pDIALOGCurrentDialog[hDialog]->bButtonNum) >> 1);
        wStartX += (bButtonIndex*_DIALOG_GetButtonWidth(hDialog)+DIALOG_BUTTON_BUTTON_DISTANCE*bButtonIndex);
    }
    else
    {
        return _pDIALOGCurrentDialog[hDialog]->pButton[bButtonIndex].wStartX;
    }

    return wStartX;
}

WORD _DIALOG_GetButtonStartY(HDIALOG hDialog)
{
    if (_pDIALOGCurrentDialog[hDialog]->wButtonStartY == DIALOG_BUTTON_START_Y_OPTIMIZED)
    {
        return (_pDIALOGCurrentDialog[hDialog]->wStartY+_pDIALOGCurrentDialog[hDialog]->wHeight-DIALOG_BUTTON_DISTANCE_V-DIALOG_BUTTON_HEIGHT);
    }
    else
    {
        return _pDIALOGCurrentDialog[hDialog]->wButtonStartY;
    }
}

//***************************************************************************
//  Function    :   DIALOG_Processkey
//  Abstract    :   
//  Arguments   :   none.
//  Return      :   
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
WORD DIALOG_Processkey(HDIALOG hDialog, BYTE bKey)
{
    if (_pDIALOGCurrentDialog[hDialog] == NULL)
    {
        return DIALOG_ACTION_NONE;
    }

    switch (bKey)
    {
    case KEY_LEFT:
        if (_bDIALOGCursor[hDialog] == 0)
        {
            break;
        }
        else
        {
            _DIALOG_DrawButton(hDialog, _bDIALOGCursor[hDialog], DIALOG_BUTTON_TYPE_NORMAL);
            _bDIALOGCursor[hDialog]--;
            _DIALOG_DrawButton(hDialog, _bDIALOGCursor[hDialog], DIALOG_BUTTON_TYPE_HIGHLIGHT);
        }
        break;
    case KEY_RIGHT:
        if (_bDIALOGCursor[hDialog] == (_pDIALOGCurrentDialog[hDialog]->bButtonNum-1))
        {
            break;
        }
        else
        {
            _DIALOG_DrawButton(hDialog, _bDIALOGCursor[hDialog], DIALOG_BUTTON_TYPE_NORMAL);
            _bDIALOGCursor[hDialog]++;
            _DIALOG_DrawButton(hDialog, _bDIALOGCursor[hDialog], DIALOG_BUTTON_TYPE_HIGHLIGHT);
        }
        break;
    case KEY_PLAY:
   case KEY_PLAY_PAUSE:
    case KEY_ENTER:
        return (DIALOG_ACTION_BUTTON_PRESSED|_pDIALOGCurrentDialog[hDialog]->pButton[(_bDIALOGCursor[hDialog])].bID);
    default:
        return DIALOG_ACTION_NONE;
    }

    return DIALOG_ACTION_DONE;
}

//***************************************************************************
//  Function    :   DIALOG_DeleteDialog
//  Abstract    :   
//  Arguments   :   none.
//  Return      :   
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void DIALOG_DeleteDialog(HDIALOG hDialog, BYTE bClearBackground)
{
    PARM_RECT Rect;

    if (bClearBackground)
    {
        //clear the dialog region
        Rect.bColor = _bDIALOGBackgroundColor[hDialog];
        Rect.rect.wLeft = _pDIALOGCurrentDialog[hDialog]->wStartX;
        Rect.rect.wTop = _pDIALOGCurrentDialog[hDialog]->wStartY;
        Rect.rect.wRight = Rect.rect.wLeft+_pDIALOGCurrentDialog[hDialog]->wWidth-1;
        Rect.rect.wBottom = Rect.rect.wTop+_pDIALOGCurrentDialog[hDialog]->wHeight-1;
        
        switch (_bDIALOGUIStyle[hDialog])
        {
        case 1:
        case 2:
        case 3:
            GDI_FillRoundRect(0, DIALOG_CONTENT_REGION_ARC, &Rect);
            break;
        case 0:
        default:
            GDI_FillRect(0, &Rect);
            break;
        }
    }

    _pDIALOGCurrentDialog[hDialog] = NULL;
    _hDIALOGStack[hDialog] = DIALOG_HANDLE_FREE;
}


HDIALOG _DIALOG_GetDialogHandle(void)
{
    BYTE i;
    for (i = 0; i < MAX_SUPPORT_DIALOG_HANDLE_NUM; i++)
    {
        if (_hDIALOGStack[i] == DIALOG_HANDLE_FREE)
        {
            _hDIALOGStack[i] = i;
            return _hDIALOGStack[i];
        }
    }

    return DIALOG_HANDLE_INVALID;
}

void DIALOG_SetCursor(HDIALOG hDialog, BYTE bButtonID)
{
    BYTE bButtonIndex;

    for (bButtonIndex = 0; bButtonIndex <  _pDIALOGCurrentDialog[hDialog]->bButtonNum; bButtonIndex++)
    {
        if (_pDIALOGCurrentDialog[hDialog]->pButton[bButtonIndex].bID == bButtonID)
        {
            _bDIALOGCursor[hDialog] = bButtonIndex;            
            break;
        }
    }

    _DIALOG_DrawButton(hDialog, bButtonIndex, DIALOG_BUTTON_TYPE_HIGHLIGHT);
}

void DIALOG_ClearCursor(HDIALOG hDialog)
{
    _DIALOG_DrawButton(hDialog, _bDIALOGCursor[hDialog], DIALOG_BUTTON_TYPE_NORMAL);
}

void DIALOG_ShowMessage(HDIALOG hDialog, WORD wStartX, WORD wStartY, WORD *pwString)
{
    PARM_DRAW_STRING StringAttr;
    
    MAINMENU_UseColorKey(&StringAttr);
    StringAttr.bTextColor = DIALOG_PALETTE_ENTRY_CONTENT_TEXT;
    
    StringAttr.wX = wStartX+_pDIALOGCurrentDialog[hDialog]->wStartX;
    StringAttr.wY = wStartY+_pDIALOGCurrentDialog[hDialog]->wStartY+DIALOG_TITLE_HEIGHT;        
    GDI_DrawString_909(0, &StringAttr, pwString);    
}