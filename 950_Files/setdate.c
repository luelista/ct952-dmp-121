#define SET_DATE_MAIN_FILE

#include "winav.h"
#include "gdi.h"
#include "cc.h"
#include "oswrap.h"
#include "input.h"
#include "setup.h"
#include "osd.h"
#include "mainmenu.h"
#include "setdate.h"
#include "fontindex.h"
#include "osdnd.h"

WORD aSETDATESeparate[] = {1, CHAR_SOLIDUS};

SETDATE _SETDATESetDate[MAX_SUPPORT_SETDATE_HANDLE_NUM];
HSETDATE _hSETDATESetDateStack[MAX_SUPPORT_SETDATE_HANDLE_NUM] = {SETDATE_HANDLE_FREE, SETDATE_HANDLE_FREE, SETDATE_HANDLE_FREE};
BYTE _bSETDATEBackgroundColor[MAX_SUPPORT_MENU_HANDLE_NUM];
BYTE _bSETDATEDUIStyle[MAX_SUPPORT_MENU_HANDLE_NUM];
BYTE _bSETDATEDialogCursorPos[MAX_SUPPORT_MENU_HANDLE_NUM];
HDIALOG _hSETDATEDlg[MAX_SUPPORT_MENU_HANDLE_NUM] = {DIALOG_HANDLE_INVALID, DIALOG_HANDLE_INVALID, DIALOG_HANDLE_INVALID};

extern BYTE Calendar_GetDaysofMonth(WORD u16SolarYear, WORD u8SolarMonth);

//***************************************************************************
//  Function    :   SETDATE_Initial
//  Abstract    :   When the menu is pop-up at 1st time, this initial function will be called. 
//                  Each UI can do its control initialization and draw the UI here.
//  Arguments   :   none.
//  Return      :   nonw.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
HSETDATE SETDATE_Create(PSETDATE_DATE pDisplayDate, PSETDATE_DLG_ACTION pSetDateAction)
{
    HSETDATE hSetDate;

    hSetDate = _SETDATE_GetSetDateHandle();

    if (hSetDate != SETDATE_HANDLE_INVALID)
    {
        _SETDATESetDate[hSetDate].DisplayDate.wYear = pDisplayDate->wYear;
        _SETDATESetDate[hSetDate].DisplayDate.bMonth = pDisplayDate->bMonth;
        _SETDATESetDate[hSetDate].DisplayDate.bDate = pDisplayDate->bDate;
        _SETDATESetDate[hSetDate].DisplayDate.wStartX = pDisplayDate->wStartX;
        _SETDATESetDate[hSetDate].DisplayDate.wStartY = pDisplayDate->wStartY;

        _SETDATESetDate[hSetDate].pSetDateDlgAction = pSetDateAction;
    }

    return hSetDate;
}

HSETDATE _SETDATE_GetSetDateHandle(void)
{
    BYTE i;
    for (i = 0; i < MAX_SUPPORT_SETDATE_HANDLE_NUM; i++)
    {
        if (_hSETDATESetDateStack[i] == SETDATE_HANDLE_FREE)
        {
            _hSETDATESetDateStack[i] = i;
            return _hSETDATESetDateStack[i];
        }
    }

    return SETDATE_HANDLE_INVALID;
}

void SETDATE_Show(HSETDATE hSetDate, BYTE bBackgroundColor, BYTE bUIStyle)
{
    _bSETDATEDialogCursorPos[hSetDate] = SET_DATE_DIALOG_CURSOR_POS_YEAR;

    _bSETDATEBackgroundColor[hSetDate] = bBackgroundColor;
    _bSETDATEDUIStyle[hSetDate] = bUIStyle;

    _SETDATESetDate[hSetDate].DisplayDate.wStartX += _SETDATEDlg.wStartX;
    _SETDATESetDate[hSetDate].DisplayDate.wStartY += _SETDATEDlg.wStartY+DIALOG_TITLE_HEIGHT;

    _SETDATE_DrawAllUI(hSetDate);
}

void _SETDATE_DrawAllUI(HSETDATE hSetDate)
{
    if (_hSETDATEDlg[hSetDate] == DIALOG_HANDLE_INVALID)
    {
        _hSETDATEDlg[hSetDate] = DIALOG_CreateDialog(&_SETDATEDlg, _bSETDATEDUIStyle[hSetDate]);
    }

    if (_hSETDATEDlg[hSetDate] != DIALOG_HANDLE_INVALID)
    {
        _SETDATE_InitializeGDI(_bSETDATEDUIStyle[hSetDate]);

        DIALOG_ShowDialog(_hSETDATEDlg[hSetDate], _bSETDATEBackgroundColor[hSetDate], FALSE);    
                
        //Show time
        _SETDATE_ShowDate(hSetDate);
        
        //Show the cursor
        _SETDATE_UpdateCursor(hSetDate, FALSE);
    }
}

void _SETDATE_InitializeGDI(BYTE bUIStyle)
{
    switch (bUIStyle)
	{
    case 1:
        GDI_ChangePALEntry(SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_TEXT, SET_TIME_DIALOG_PALETTE_COLOR_EDIT_TEXT_2, FALSE);
        GDI_ChangePALEntry(SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_HIGHLIGHT, SET_TIME_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_2, FALSE);
        break;
    case 2:
        GDI_ChangePALEntry(SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_TEXT, SET_TIME_DIALOG_PALETTE_COLOR_EDIT_TEXT_3, FALSE);
        GDI_ChangePALEntry(SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_HIGHLIGHT, SET_TIME_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_3, FALSE);
        break;
    case 3:
        GDI_ChangePALEntry(SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_TEXT, SET_TIME_DIALOG_PALETTE_COLOR_EDIT_TEXT_4, FALSE);
        GDI_ChangePALEntry(SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_HIGHLIGHT, SET_TIME_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_4, FALSE);
        break;
	case 0:
    default:               
        GDI_ChangePALEntry(SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_TEXT, SET_TIME_DIALOG_PALETTE_COLOR_EDIT_TEXT_1, FALSE);
        GDI_ChangePALEntry(SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_HIGHLIGHT, SET_TIME_DIALOG_PALETTE_COLOR_EDIT_HIGHLIGHT_1, FALSE);
        break;
    }

    GDI_ChangePALEntry(SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_INFO, SET_TIME_DIALOG_PALETTE_COLOR_EDIT_INFO, FALSE);

    GDI_WaitPaletteComplete();
}

//***************************************************************************
//  Function    :   SETDATE_ProcessKey
//  Abstract    :   
//  Arguments   :   none.
//  Return      :   
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void SETDATE_ProcessKey(HSETDATE hSetDate, BYTE bKey)
{
    WORD wReturn;

    if (_bSETDATEDialogCursorPos[hSetDate] == SET_TIME_DIALOG_CURSOR_POS_BUTTON)
    {
        wReturn = DIALOG_Processkey(_hSETDATEDlg[hSetDate], bKey);        
        if (wReturn == DIALOG_ACTION_NONE)
        {
            if ((bKey == KEY_UP) || (bKey == KEY_LEFT) || (bKey == KEY_RIGHT))
            {
                DIALOG_ClearCursor(_hSETDATEDlg[hSetDate]);          
                _bSETDATEDialogCursorPos[hSetDate] = SET_DATE_DIALOG_CURSOR_POS_YEAR;
                _SETDATE_UpdateCursor(hSetDate, FALSE);
            }
        }
        else if (wReturn & DIALOG_ACTION_BUTTON_PRESSED)
        {
            if (LOBYTE(wReturn) == SET_TIME_DIALOG_BUTTON_ID_OK)
            {            
                _SETDATESetDate[hSetDate].pSetDateDlgAction->SETDATE_OK_Func_Ptr(_SETDATESetDate[hSetDate].DisplayDate.wYear, _SETDATESetDate[hSetDate].DisplayDate.bMonth, _SETDATESetDate[hSetDate].DisplayDate.bDate);
            }
            else //Cancel
            {
                _SETDATESetDate[hSetDate].pSetDateDlgAction->SETDATE_CANCEL_Func_Ptr();
            }
        }        
    }
    else
    {
        wReturn = _SETDATE_EDIT_ProcessKey(hSetDate, bKey);

#ifdef NL_SOLUTION
        if (wReturn == SETDATE_EDIT_ACTION_EXCEED_ENTER)
        {
            _SETDATESetDate[hSetDate].pSetDateDlgAction->SETDATE_OK_Func_Ptr(_SETDATESetDate[hSetDate].DisplayDate.wYear, _SETDATESetDate[hSetDate].DisplayDate.bMonth, _SETDATESetDate[hSetDate].DisplayDate.bDate);
        }
#else
        if ((wReturn == SETDATE_EDIT_ACTION_EXCEED_LEFT) || (wReturn == SETDATE_EDIT_ACTION_EXCEED_RIGHT))
        {
            _bSETDATEDialogCursorPos[hSetDate] = SET_TIME_DIALOG_CURSOR_POS_BUTTON;
            if (wReturn == SETDATE_EDIT_ACTION_EXCEED_LEFT)
            {
                DIALOG_SetCursor(_hSETDATEDlg[hSetDate], SET_TIME_DIALOG_BUTTON_ID_OK);
            }
            else
            {
                DIALOG_SetCursor(_hSETDATEDlg[hSetDate], SET_TIME_DIALOG_BUTTON_ID_CANCEL);
            }
        }
#endif //NL_SOLUTION
    }
}

BYTE _SETDATE_EDIT_ProcessKey(HSETDATE hSetDate, BYTE bKey)
{
    switch (bKey)
    {
    case KEY_UP:
        {
            //updaet the date
            switch (_bSETDATEDialogCursorPos[hSetDate])
            {
            case SET_DATE_DIALOG_CURSOR_POS_YEAR:
                _SETDATESetDate[hSetDate].DisplayDate.wYear++;
                if (_SETDATESetDate[hSetDate].DisplayDate.wYear > SETDATE_MAX_YEAR)
                {
                    _SETDATESetDate[hSetDate].DisplayDate.wYear = SETDATE_MIN_YEAR;
                }
                break;
            case SET_DATE_DIALOG_CURSOR_POS_MONTH:
                _SETDATESetDate[hSetDate].DisplayDate.bMonth++;
                if (_SETDATESetDate[hSetDate].DisplayDate.bMonth > 12)
                {
                    _SETDATESetDate[hSetDate].DisplayDate.bMonth = 1;
                }
                break;
            case SET_DATE_DIALOG_CURSOR_POS_DATE:
                _SETDATESetDate[hSetDate].DisplayDate.bDate++;
                if (_SETDATESetDate[hSetDate].DisplayDate.bDate > Calendar_GetDaysofMonth(_SETDATESetDate[hSetDate].DisplayDate.wYear, _SETDATESetDate[hSetDate].DisplayDate.bMonth))
                {
                    _SETDATESetDate[hSetDate].DisplayDate.bDate = 1;
                }
                break;
            default:
                break;
            }
        }
        _SETDATE_UpdateCursor(hSetDate, FALSE);
        break;
    case KEY_DOWN:
        //updaet the time
        {
            switch (_bSETDATEDialogCursorPos[hSetDate])
            {
            case SET_DATE_DIALOG_CURSOR_POS_YEAR:
                if (_SETDATESetDate[hSetDate].DisplayDate.wYear > SETDATE_MIN_YEAR)
                {
                    _SETDATESetDate[hSetDate].DisplayDate.wYear--;
                }
                else
                {
                    _SETDATESetDate[hSetDate].DisplayDate.wYear = SETDATE_MIN_YEAR;
                }
                break;
            case SET_DATE_DIALOG_CURSOR_POS_MONTH:
                if (_SETDATESetDate[hSetDate].DisplayDate.bMonth > 1)
                {
                    _SETDATESetDate[hSetDate].DisplayDate.bMonth--;
                }
                else
                {
                    _SETDATESetDate[hSetDate].DisplayDate.bMonth = 12;
                }
                break;
            case SET_DATE_DIALOG_CURSOR_POS_DATE:
                if (_SETDATESetDate[hSetDate].DisplayDate.bDate > 1)
                {
                    _SETDATESetDate[hSetDate].DisplayDate.bDate--;
                }
                else
                {
                    _SETDATESetDate[hSetDate].DisplayDate.bDate = Calendar_GetDaysofMonth(_SETDATESetDate[hSetDate].DisplayDate.wYear, _SETDATESetDate[hSetDate].DisplayDate.bMonth);
                }
                break;
            default:
                break;
            }
        }
        _SETDATE_UpdateCursor(hSetDate, FALSE);
        break;
    case KEY_LEFT:
        if (_bSETDATEDialogCursorPos[hSetDate] == SET_DATE_DIALOG_CURSOR_POS_YEAR)
        {
#ifndef NL_SOLUTION
            //Clear the cursor
            _SETDATE_UpdateCursor(hSetDate, TRUE);
            return SETDATE_EDIT_ACTION_EXCEED_LEFT;
#endif //NL_SOLUTION
        }
        else
        {
            _SETDATE_UpdateCursor(hSetDate, TRUE);
            _bSETDATEDialogCursorPos[hSetDate]--;
            _SETDATE_UpdateCursor(hSetDate, FALSE);
        }
        break;
    case KEY_RIGHT:
        if (_bSETDATEDialogCursorPos[hSetDate] == SET_DATE_DIALOG_CURSOR_POS_DATE)
        {
#ifdef NL_SOLUTION
            return SETDATE_EDIT_ACTION_DONE;
#else
            //Clear the cursor
            _SETDATE_UpdateCursor(hSetDate, TRUE);
            return SETDATE_EDIT_ACTION_EXCEED_RIGHT;
#endif //#ifndef NL_SOLUTION
        }
        
        _SETDATE_UpdateCursor(hSetDate, TRUE);
        _bSETDATEDialogCursorPos[hSetDate]++;
        _SETDATE_UpdateCursor(hSetDate, FALSE);        
        break;
    case KEY_PLAY:
    case KEY_ENTER:
        return SETDATE_EDIT_ACTION_EXCEED_ENTER;
    default:
        break;
    }

    return SETDATE_EDIT_ACTION_DONE;
}

void SETDATE_Recover(HSETDATE hSetDate)
{
    _SETDATE_DrawAllUI(hSetDate);
}

void SETDATE_Exit(HSETDATE hSetDate, BYTE bClearBackground)
{
    BYTE i;

    if (_hSETDATEDlg[hSetDate] != DIALOG_HANDLE_INVALID)
    {
        DIALOG_DeleteDialog(_hSETDATEDlg[hSetDate], bClearBackground);
        _hSETDATEDlg[hSetDate] = DIALOG_HANDLE_INVALID;
    }

    for (i = 0; i < MAX_SUPPORT_SETDATE_HANDLE_NUM; i++)
    {
        if (_hSETDATESetDateStack[i] == hSetDate)
        {
            _hSETDATESetDateStack[i] = SETDATE_HANDLE_FREE;
            return;
        }
    }
}

void _SETDATE_UpdateCursor(HSETDATE hSetDate, BYTE bClear)
{
    switch (_bSETDATEDialogCursorPos[hSetDate])
    {
    case SET_DATE_DIALOG_CURSOR_POS_YEAR:
        _SETDATE_ShowNum(hSetDate, _SETDATESetDate[hSetDate].DisplayDate.wYear, _SETDATESetDate[hSetDate].DisplayDate.wStartX, !bClear);
        break;
    case SET_DATE_DIALOG_CURSOR_POS_MONTH:
        _SETDATE_ShowNum(hSetDate, _SETDATESetDate[hSetDate].DisplayDate.bMonth, _SETDATESetDate[hSetDate].DisplayDate.wStartX+SETDATE_NUM_WIDTH*4+SETDATE_NUM_SEPARATE_WIDTH, !bClear);
        break;
    case SET_DATE_DIALOG_CURSOR_POS_DATE:
        _SETDATE_ShowNum(hSetDate, _SETDATESetDate[hSetDate].DisplayDate.bDate, _SETDATESetDate[hSetDate].DisplayDate.wStartX+SETDATE_NUM_WIDTH*6+SETDATE_NUM_SEPARATE_WIDTH*2, !bClear);
        break;
    default:
        break;
    }
}


void _SETDATE_ShowDate(HSETDATE hSetDate)
{
    //Show Year
    _SETDATE_ShowNum(hSetDate, _SETDATESetDate[hSetDate].DisplayDate.wYear, _SETDATESetDate[hSetDate].DisplayDate.wStartX, FALSE);

    //Show separate
    _SETDATE_ShowSeparate(hSetDate, 0);

    //Show month
    _SETDATE_ShowNum(hSetDate, _SETDATESetDate[hSetDate].DisplayDate.bMonth, _SETDATESetDate[hSetDate].DisplayDate.wStartX+SETDATE_NUM_WIDTH*4+SETDATE_NUM_SEPARATE_WIDTH, FALSE);

    //Show separate
    _SETDATE_ShowSeparate(hSetDate, 1);
    
    //Show second
    _SETDATE_ShowNum(hSetDate, _SETDATESetDate[hSetDate].DisplayDate.bDate, _SETDATESetDate[hSetDate].DisplayDate.wStartX+SETDATE_NUM_WIDTH*6+SETDATE_NUM_SEPARATE_WIDTH*2, FALSE);
}

void _SETDATE_ShowSeparate(HSETDATE hSetDate, BYTE bPos)
{
    PARM_DRAW_STRING SETDATEStringAttr;
    
    SETDATEStringAttr.bBackground = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;    
    SETDATEStringAttr.bColorKey = 0;
    SETDATEStringAttr.bShadow1 = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    SETDATEStringAttr.bShadow2 = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    SETDATEStringAttr.bTextColor = SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_INFO;

    SETDATEStringAttr.wX = _SETDATESetDate[hSetDate].DisplayDate.wStartX+(SETDATE_NUM_WIDTH*2)*(bPos+1)+SETDATE_NUM_SEPARATE_WIDTH*bPos+SETDATE_NUM_WIDTH*2;
    SETDATEStringAttr.wY = _SETDATESetDate[hSetDate].DisplayDate.wStartY;    
    
    GDI_DrawString(0, &SETDATEStringAttr, aSETDATESeparate);
}

//***************************************************************************
//  Function    :   _SETDATE_ShowNum
//  Abstract    :   
//  Arguments   :   wNum: The number you want to output.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _SETDATE_ShowNum(HSETDATE hSetDate, WORD wNum, WORD wStartX, BYTE bCursor)
{    
    PARM_DRAW_STRING SETDATEStringAttr;

    OSDUTL_PrepareOutputString();

    if (wNum < 10)
    {
        OSDUTL_OutputNumFromIndex((WORD)(wNum/10));
        OSDUTL_OutputNumFromIndex((WORD)(wNum%10));
    }
    else
    {
        OSDUTL_OutputNumFromIndex(wNum);
    }
 

    if (bCursor)
    {
        SETDATEStringAttr.bBackground = SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_HIGHLIGHT;
    }
    else
    {
        SETDATEStringAttr.bBackground = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    }

    SETDATEStringAttr.bColorKey = 0;
    SETDATEStringAttr.bShadow1 = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    SETDATEStringAttr.bShadow2 = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    SETDATEStringAttr.bTextColor = SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_INFO;
    SETDATEStringAttr.wX = wStartX;
    SETDATEStringAttr.wY = _SETDATESetDate[hSetDate].DisplayDate.wStartY;    

    GDI_DrawString(0, &SETDATEStringAttr, __pOSDUTLOutputString);
}