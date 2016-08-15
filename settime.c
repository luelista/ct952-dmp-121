#define SET_TIME_MAIN_FILE

#include "winav.h"
#include "gdi.h"
#include "cc.h"
#include "osd.h"
#include "oswrap.h"
#include "input.h"
#include "setup.h"
#include "mainmenu.h"
#include "settime.h"
#include "fontindex.h"
#include "osdnd.h"

WORD aSETTIMEColon[] = {1, CHAR_COLON};

SETTIME _SETTIMESetTime[MAX_SUPPORT_SETTIME_HANDLE_NUM];
HSETTIME _hSETTIMESetTimeStack[MAX_SUPPORT_SETTIME_HANDLE_NUM] = {SETTIME_HANDLE_FREE, SETTIME_HANDLE_FREE, SETTIME_HANDLE_FREE};
BYTE _bSETTIMEBackgroundColor[MAX_SUPPORT_MENU_HANDLE_NUM];
BYTE _bSETTIMEDUIStyle[MAX_SUPPORT_MENU_HANDLE_NUM];
BYTE _bSETTIMEDialogCursorPos[MAX_SUPPORT_MENU_HANDLE_NUM];
HDIALOG _hSETTIMEDlg[MAX_SUPPORT_MENU_HANDLE_NUM] = {DIALOG_HANDLE_INVALID, DIALOG_HANDLE_INVALID, DIALOG_HANDLE_INVALID};

//***************************************************************************
//  Function    :   SETTIME_Initial
//  Abstract    :   When the menu is pop-up at 1st time, this initial function will be called. 
//                  Each UI can do its control initialization and draw the UI here.
//  Arguments   :   none.
//  Return      :   nonw.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
HSETTIME SETTIME_Create(PSETTIME_TIME pDisplayTime, PSETTIME_DLG_ACTION pSetTimeAction)
{
    HSETTIME hSetTime;

    hSetTime = _SETTIME_GetSetTimeHandle();

    if (hSetTime != SETTIME_HANDLE_INVALID)
    {
        _SETTIMESetTime[hSetTime].DisplayTime.bDisplayType = pDisplayTime->bDisplayType;
        _SETTIMESetTime[hSetTime].DisplayTime.bHour = pDisplayTime->bHour;
        _SETTIMESetTime[hSetTime].DisplayTime.bMinute = pDisplayTime->bMinute;
        _SETTIMESetTime[hSetTime].DisplayTime.bSecond = pDisplayTime->bSecond;
        _SETTIMESetTime[hSetTime].DisplayTime.wStartX = pDisplayTime->wStartX;
        _SETTIMESetTime[hSetTime].DisplayTime.wStartY = pDisplayTime->wStartY;

        _SETTIMESetTime[hSetTime].pSetTimeDlgAction = pSetTimeAction;
    }

    return hSetTime;
}

HSETTIME _SETTIME_GetSetTimeHandle(void)
{
    BYTE i;
    for (i = 0; i < MAX_SUPPORT_SETTIME_HANDLE_NUM; i++)
    {
        if (_hSETTIMESetTimeStack[i] == SETTIME_HANDLE_FREE)
        {
            _hSETTIMESetTimeStack[i] = i;
            return _hSETTIMESetTimeStack[i];
        }
    }

    return SETTIME_HANDLE_INVALID;
}

void SETTIME_Show(HSETTIME hSetTime, BYTE bBackgroundColor, BYTE bUIStyle)
{
    _bSETTIMEDialogCursorPos[hSetTime] = SET_TIME_DIALOG_CURSOR_POS_HOUR;

    _bSETTIMEBackgroundColor[hSetTime] = bBackgroundColor;
    _bSETTIMEDUIStyle[hSetTime] = bUIStyle;

    _SETTIMESetTime[hSetTime].DisplayTime.wStartX += _SETTIMEDlg.wStartX;
    _SETTIMESetTime[hSetTime].DisplayTime.wStartY += _SETTIMEDlg.wStartY+DIALOG_TITLE_HEIGHT;

    _SETTIME_DrawAllUI(hSetTime);
}

void _SETTIME_DrawAllUI(HSETTIME hSetTime)
{
    if (_hSETTIMEDlg[hSetTime] == DIALOG_HANDLE_INVALID)
    {
        _hSETTIMEDlg[hSetTime] = DIALOG_CreateDialog(&_SETTIMEDlg, _bSETTIMEDUIStyle[hSetTime]);
    }

    if (_hSETTIMEDlg[hSetTime] != DIALOG_HANDLE_INVALID)
    {
        _SETTIME_InitializeGDI(_bSETTIMEDUIStyle[hSetTime]);

        DIALOG_ShowDialog(_hSETTIMEDlg[hSetTime], _bSETTIMEBackgroundColor[hSetTime], FALSE);    
                
        //Show time
        _SETTIME_ShowTime(hSetTime);
        
        //Show the cursor
        _SETTIME_UpdateCursor(hSetTime, FALSE);
    }
}

void _SETTIME_InitializeGDI(BYTE bUIStyle)
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
//  Function    :   SETTIME_ProcessKey
//  Abstract    :   
//  Arguments   :   none.
//  Return      :   
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void SETTIME_ProcessKey(HSETTIME hSetTime, BYTE bKey)
{
    WORD wReturn;

    if (_bSETTIMEDialogCursorPos[hSetTime] == SET_TIME_DIALOG_CURSOR_POS_BUTTON)
    {
        wReturn = DIALOG_Processkey(_hSETTIMEDlg[hSetTime], bKey);        
        if (wReturn == DIALOG_ACTION_NONE)
        {
            if ((bKey == KEY_UP) || (bKey == KEY_LEFT) || (bKey == KEY_RIGHT))
            {
                DIALOG_ClearCursor(_hSETTIMEDlg[hSetTime]);          
                _bSETTIMEDialogCursorPos[hSetTime] = SET_TIME_DIALOG_CURSOR_POS_HOUR;
                _SETTIME_UpdateCursor(hSetTime, FALSE);
            }
        }
        else if (wReturn & DIALOG_ACTION_BUTTON_PRESSED)
        {
            if (LOBYTE(wReturn) == SET_TIME_DIALOG_BUTTON_ID_OK)
            {            
                _SETTIMESetTime[hSetTime].pSetTimeDlgAction->SETTIME_OK_Func_Ptr(_SETTIMESetTime[hSetTime].DisplayTime.bHour, _SETTIMESetTime[hSetTime].DisplayTime.bMinute, _SETTIMESetTime[hSetTime].DisplayTime.bSecond);
            }
            else //Cancel
            {
                _SETTIMESetTime[hSetTime].pSetTimeDlgAction->SETTIME_CANCEL_Func_Ptr();
            }
        }        
    }
    else
    {
        wReturn = _SETTIME_EDIT_ProcessKey(hSetTime, bKey);

#ifdef NL_SOLUTION
        if (wReturn == SETTIME_EDIT_ACTION_EXCEED_ENTER)
        {
            _SETTIMESetTime[hSetTime].pSetTimeDlgAction->SETTIME_OK_Func_Ptr(_SETTIMESetTime[hSetTime].DisplayTime.bHour, _SETTIMESetTime[hSetTime].DisplayTime.bMinute, _SETTIMESetTime[hSetTime].DisplayTime.bSecond);
        }
#else
        if ((wReturn == SETTIME_EDIT_ACTION_EXCEED_LEFT) || (wReturn == SETTIME_EDIT_ACTION_EXCEED_RIGHT))
        {
            _bSETTIMEDialogCursorPos[hSetTime] = SET_TIME_DIALOG_CURSOR_POS_BUTTON;
            if (wReturn == SETTIME_EDIT_ACTION_EXCEED_LEFT)
            {
                DIALOG_SetCursor(_hSETTIMEDlg[hSetTime], SET_TIME_DIALOG_BUTTON_ID_OK);
            }
            else
            {
                DIALOG_SetCursor(_hSETTIMEDlg[hSetTime], SET_TIME_DIALOG_BUTTON_ID_CANCEL);
            }
        }
#endif //NL_SOLUTION
    }
}

BYTE _SETTIME_EDIT_ProcessKey(HSETTIME hSetTime, BYTE bKey)
{
    switch (bKey)
    {
    case KEY_UP:
        {
            //updaet the time
            switch (_bSETTIMEDialogCursorPos[hSetTime])
            {
            case SET_TIME_DIALOG_CURSOR_POS_HOUR:
                _SETTIMESetTime[hSetTime].DisplayTime.bHour++;
                if (_SETTIMESetTime[hSetTime].DisplayTime.bHour > 23)
                {
                    _SETTIMESetTime[hSetTime].DisplayTime.bHour = 0;
                }
                break;
            case SET_TIME_DIALOG_CURSOR_POS_MINUTE:
                _SETTIMESetTime[hSetTime].DisplayTime.bMinute++;
                if (_SETTIMESetTime[hSetTime].DisplayTime.bMinute > 59)
                {
                    _SETTIMESetTime[hSetTime].DisplayTime.bMinute = 0;
                }
                break;
            case SET_TIME_DIALOG_CURSOR_POS_SECOND:
                _SETTIMESetTime[hSetTime].DisplayTime.bSecond++;
                if (_SETTIMESetTime[hSetTime].DisplayTime.bSecond > 59)
                {
                    _SETTIMESetTime[hSetTime].DisplayTime.bSecond = 0;
                }
                break;
            default:
                break;
            }
        }
        _SETTIME_UpdateCursor(hSetTime, FALSE);
        break;
    case KEY_DOWN:
        //updaet the time
        {
            switch (_bSETTIMEDialogCursorPos[hSetTime])
            {
            case SET_TIME_DIALOG_CURSOR_POS_HOUR:
                if (_SETTIMESetTime[hSetTime].DisplayTime.bHour > 0)
                {
                    _SETTIMESetTime[hSetTime].DisplayTime.bHour--;
                }
                else
                {
                    _SETTIMESetTime[hSetTime].DisplayTime.bHour = 23;
                }
                break;
            case SET_TIME_DIALOG_CURSOR_POS_MINUTE:
                if (_SETTIMESetTime[hSetTime].DisplayTime.bMinute > 0)
                {
                    _SETTIMESetTime[hSetTime].DisplayTime.bMinute--;
                }
                else
                {
                    _SETTIMESetTime[hSetTime].DisplayTime.bMinute = 59;
                }
                break;
            case SET_TIME_DIALOG_CURSOR_POS_SECOND:
                if (_SETTIMESetTime[hSetTime].DisplayTime.bSecond > 0)
                {
                    _SETTIMESetTime[hSetTime].DisplayTime.bSecond--;
                }
                else
                {
                    _SETTIMESetTime[hSetTime].DisplayTime.bSecond = 59;
                }
                break;
            default:
                break;
            }
        }
        _SETTIME_UpdateCursor(hSetTime, FALSE);
        break;
    case KEY_LEFT:
        if (_bSETTIMEDialogCursorPos[hSetTime] == SET_TIME_DIALOG_CURSOR_POS_HOUR)
        {
#ifndef NL_SOLUTION
            //Clear the cursor
            _SETTIME_UpdateCursor(hSetTime, TRUE);
            return SETTIME_EDIT_ACTION_EXCEED_LEFT;
#endif //NL_SOLUTION
        }
        else
        {
            _SETTIME_UpdateCursor(hSetTime, TRUE);
            _bSETTIMEDialogCursorPos[hSetTime]--;
            _SETTIME_UpdateCursor(hSetTime, FALSE);
        }
        break;
    case KEY_RIGHT:
        if (_SETTIMESetTime[hSetTime].DisplayTime.bDisplayType == SETTIME_DISPLAY_MODE_HOUR_MINUTE)
        {
            if (_bSETTIMEDialogCursorPos[hSetTime] == SET_TIME_DIALOG_CURSOR_POS_MINUTE)
            {
#ifdef NL_SOLUTION
                return SETTIME_EDIT_ACTION_DONE;
#else
                //Clear the cursor
                _SETTIME_UpdateCursor(hSetTime, TRUE);
                return SETTIME_EDIT_ACTION_EXCEED_RIGHT;
#endif //NL_SOLUTION
            }
        }
        else if (_SETTIMESetTime[hSetTime].DisplayTime.bDisplayType == SETTIME_DISPLAY_MODE_HOUR_MINUTE_SEC)
        {
            if (_bSETTIMEDialogCursorPos[hSetTime] == SET_TIME_DIALOG_CURSOR_POS_SECOND)
            {
#ifdef NL_SOLUTION
                return SETTIME_EDIT_ACTION_DONE;
#else
                //Clear the cursor
                _SETTIME_UpdateCursor(hSetTime, TRUE);
                return SETTIME_EDIT_ACTION_EXCEED_RIGHT;
#endif //#ifndef NL_SOLUTION
            }
        }
        
        _SETTIME_UpdateCursor(hSetTime, TRUE);
        _bSETTIMEDialogCursorPos[hSetTime]++;
        _SETTIME_UpdateCursor(hSetTime, FALSE);        
        break;
    case KEY_PLAY:
    case KEY_ENTER:
        return SETTIME_EDIT_ACTION_EXCEED_ENTER;
    default:
        break;
    }

    return SETTIME_EDIT_ACTION_DONE;
}

void SETTIME_Recover(HSETTIME hSetTime)
{
    _SETTIME_DrawAllUI(hSetTime);
}

void SETTIME_Exit(HSETTIME hSetTime, BYTE bClearBackground)
{
    BYTE i;

    if (_hSETTIMEDlg[hSetTime] != DIALOG_HANDLE_INVALID)
    {
        DIALOG_DeleteDialog(_hSETTIMEDlg[hSetTime], bClearBackground);
        _hSETTIMEDlg[hSetTime] = DIALOG_HANDLE_INVALID;
    }

    for (i = 0; i < MAX_SUPPORT_SETTIME_HANDLE_NUM; i++)
    {
        if (_hSETTIMESetTimeStack[i] == hSetTime)
        {
            _hSETTIMESetTimeStack[i] = SETTIME_HANDLE_FREE;
            return;
        }
    }
}

void _SETTIME_UpdateCursor(HSETTIME hSetTime, BYTE bClear)
{
    switch (_bSETTIMEDialogCursorPos[hSetTime])
    {
    case SET_TIME_DIALOG_CURSOR_POS_HOUR:
        _SETTIME_ShowTimeNum(hSetTime, _SETTIMESetTime[hSetTime].DisplayTime.bHour, _SETTIMESetTime[hSetTime].DisplayTime.wStartX, !bClear);
        break;
    case SET_TIME_DIALOG_CURSOR_POS_MINUTE:
        _SETTIME_ShowTimeNum(hSetTime, _SETTIMESetTime[hSetTime].DisplayTime.bMinute, _SETTIMESetTime[hSetTime].DisplayTime.wStartX+SETTIME_NUM_WIDTH*2+SETTIME_NUM_COLON_WIDTH, !bClear);
        break;
    case SET_TIME_DIALOG_CURSOR_POS_SECOND:
        _SETTIME_ShowTimeNum(hSetTime, _SETTIMESetTime[hSetTime].DisplayTime.bSecond, _SETTIMESetTime[hSetTime].DisplayTime.wStartX+SETTIME_NUM_WIDTH*4+SETTIME_NUM_COLON_WIDTH*2, !bClear);
        break;
    default:
        break;
    }
}


void _SETTIME_ShowTime(HSETTIME hSetTime)
{
    //Show hour
    _SETTIME_ShowTimeNum(hSetTime, _SETTIMESetTime[hSetTime].DisplayTime.bHour, _SETTIMESetTime[hSetTime].DisplayTime.wStartX, FALSE);

    //Show colon
    _SETTIME_ShowColon(hSetTime, 0);

    //Show minute
    _SETTIME_ShowTimeNum(hSetTime, _SETTIMESetTime[hSetTime].DisplayTime.bMinute, _SETTIMESetTime[hSetTime].DisplayTime.wStartX+SETTIME_NUM_WIDTH*2+SETTIME_NUM_COLON_WIDTH, FALSE);

    if (_SETTIMESetTime[hSetTime].DisplayTime.bDisplayType == SETTIME_DISPLAY_MODE_HOUR_MINUTE_SEC)
    {
        //Show colon
        _SETTIME_ShowColon(hSetTime, 1);
        
        //Show second
        _SETTIME_ShowTimeNum(hSetTime, _SETTIMESetTime[hSetTime].DisplayTime.bSecond, _SETTIMESetTime[hSetTime].DisplayTime.wStartX+SETTIME_NUM_WIDTH*4+SETTIME_NUM_COLON_WIDTH*2, FALSE);
    }
}

void _SETTIME_ShowColon(HSETTIME hSetTime, BYTE bPos)
{
    PARM_DRAW_STRING SETTIMEStringAttr;
    
    SETTIMEStringAttr.bBackground = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;    
    SETTIMEStringAttr.bColorKey = 0;
    SETTIMEStringAttr.bShadow1 = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    SETTIMEStringAttr.bShadow2 = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    SETTIMEStringAttr.bTextColor = SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_INFO;
    SETTIMEStringAttr.wX = _SETTIMESetTime[hSetTime].DisplayTime.wStartX+(SETTIME_NUM_WIDTH*2)*(bPos+1)+SETTIME_NUM_COLON_WIDTH*bPos;
    SETTIMEStringAttr.wY = _SETTIMESetTime[hSetTime].DisplayTime.wStartY;    
    
    GDI_DrawString(0, &SETTIMEStringAttr, aSETTIMEColon);
}

//***************************************************************************
//  Function    :   _SETTIME_ShowTime
//  Abstract    :   
//  Arguments   :   wNum: The number you want to output.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _SETTIME_ShowTimeNum(HSETTIME hSetTime, WORD wNum, WORD wStartX, BYTE bCursor)
{    
    PARM_DRAW_STRING SETTIMEStringAttr;

    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex((WORD)(wNum/10));
    OSDUTL_OutputNumFromIndex((WORD)(wNum%10));

    if (bCursor)
    {
        SETTIMEStringAttr.bBackground = SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_HIGHLIGHT;
    }
    else
    {
        SETTIMEStringAttr.bBackground = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    }

    SETTIMEStringAttr.bColorKey = 0;
    SETTIMEStringAttr.bShadow1 = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    SETTIMEStringAttr.bShadow2 = DIALOG_PALETTE_ENTRY_CONTENT_BACKGROUND;
    SETTIMEStringAttr.bTextColor = SET_TIME_DIALOG_PALETTE_ENTRY_EDIT_INFO;
    SETTIMEStringAttr.wX = wStartX;
    SETTIMEStringAttr.wY = _SETTIMESetTime[hSetTime].DisplayTime.wStartY;    

    GDI_DrawString(0, &SETTIMEStringAttr, __pOSDUTLOutputString);
}