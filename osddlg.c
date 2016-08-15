//#include  "ctkav.h"
#include "winav.h"
#include "input.h"
#include "osd.h"
#include "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include "osdnd.h"
#include "cc.h"
#include "osddlg.h"
#include "utl.h"
#include "media.h"
#include "setup.h"
#include "disp.h"
#include "notransl.h"

BYTE _bOSDDLGTemp;
WORD _wOSDDLGTemp, _wOSDDLGTemp1;
WORD _wOSDDLGButtonWidth;
PARM_RECT _OSDDLGRect;
DLG_ATTRIBUTE _OSDDLGAttribute;
PARM_DRAW_STRING _OSDDLGStringAttr;

extern BYTE __bChooseMedia; //0==>CDROM/DVDROM, 1==>USB
extern BYTE __bTVType;
//********************************************************************************************************
//  Function    :   OSDDLG_Update
//  Abstract    :   Update the dialog box according to the message.
//  Arguments   :   bMessage: OSDDLG_MSG_SHOW_DLG==>Show the dialog box. 
//              :             OSDDLG_MSG_DRAW_HIGHLIGHT: Draw highlighted button. bParam is the index of the button.
//              :             OSDDLG_MSG_CLEAR_HIGHLIGHT: Draw normal button (clear the highlighted state). 
//              :             bParam is the index of the button.
//              :             OSDDLG_MSG_EXIT: Exit the dialog box.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void OSDDLG_MEDIA_SELECT_Update(BYTE bMessage, BYTE bParam)
{
    switch (bMessage)
    {
    case OSDDLG_MSG_SHOW_DLG:
#ifdef SUPPORT_STB
        if (__bChooseMedia == MEDIA_SELECT_STB)
        {
            GDI_ClearRegion(0);
        }
#endif //SUPPORT_STB
        _OSDDLG_MEDIA_SELECT_ShowDialogBox(OSDDLG_SHOW_DLG_INITIAL, bParam);
        break;
    case OSDDLG_MSG_DRAW_HIGHLIGHT:
        _OSDDLG_MEDIA_SELECT_DrawButton(bParam, OSDDLG_BUTTON_STATE_HIGHLIGHT);
        break;
    case OSDDLG_MSG_CLEAR_HIGHLIGHT:
        _OSDDLG_MEDIA_SELECT_DrawButton(bParam, OSDDLG_BUTTON_STATE_NORMAL);
        break;
    case OSDDLG_MSG_EXIT:
        _OSDDLG_MEDIA_SELECT_UpdateDlgBackground(OSDDLG_UPDATE_CLEAR);

        if (bParam == OSD_UI_CLEAR_NORMAL)
        {
            //Tell OSD that the "Media Select" dialog box exits.
            OSD_ChangeUI(OSD_UI_MEDIA_SELECT_DLG, OSD_EXIT_UI);
        }
        break;
    default:
        break;
    }
}

//********************************************************************************************************
//  Function    :   _OSDDLG_MEDIA_SELECT_ShowDialogBox
//  Abstract    :   This function will show the dialog box according to the ID.
//  Arguments   :   bDisplayMode: OSDDLG_SHOW_DLG_INITIAL/OSDDLG_SHOW_DLG_RECOVER.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDDLG_MEDIA_SELECT_ShowDialogBox(BYTE bDisplayMode, BYTE bBackgroudMode)
{
    BYTE bIndex;

    if (bDisplayMode == OSDDLG_SHOW_DLG_INITIAL)
    {
        //Tell OSD that the "Media Select" dialog box enters.
        OSD_ChangeUI(OSD_UI_MEDIA_SELECT_DLG, OSD_ENTER_UI);
        
        //Initialize the attribute of the dialog box.
        _OSDDLGAttribute.wWidth = _OSDDLG_MEDIA_SELECT_GetMaxButtonWidth()+(WORD)((OSDDLG_MEDIA_SELECT_BACKGROUND_BUTTON_DISTANCE_H<<1));
        _OSDDLGAttribute.wHeight = _OSDDLG_MEDIA_SELECT_GetDlgHeight();
        _OSDDLGAttribute.wStartH = ((OSD_PLANE_WIDTH -  _OSDDLGAttribute.wWidth) >> 1);
        _OSDDLGAttribute.wStartV = ((OSD_PLANE_HEIGHT - _OSDDLGAttribute.wHeight) >> 1);
    }
    
    //Initialize palette
    _OSDDLG_MEDIA_SELECT_InitializeGDI();

    //Clear background
    if (bBackgroudMode == OSDDLG_MEDIA_SELECT_BACKGROUND_GENERAL_MODE)
    {
        //Clear the region except normal-upper message and normal bottom message
        _OSDDLGRect.rect.wLeft = 0;
        _OSDDLGRect.rect.wTop = __OSDNDUpMsgAttr.wY+OSDND_FONT_HEIGHT;
        _OSDDLGRect.rect.wRight = GDI_REGION_WIDTH-1;
        _OSDDLGRect.rect.wBottom = __OSDNDBottomMsgAttr.wY-1;
        _OSDDLGRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;   
        
        GDI_FillRect_909(0, &_OSDDLGRect);        
    }
    else if (bBackgroudMode == OSDDLG_MEDIA_SELECT_BACKGROUND_MM_MODE)
    {
        GDI_ClearRegion(0); //clear region 0
    }

    //Draw the background of the dialog box.
    _OSDDLG_MEDIA_SELECT_UpdateDlgBackground(OSDDLG_UPDATE_DRAW);

    //Draw buttons
    for (bIndex = 0; bIndex < OSDDLG_MEDIA_SELECT_TOTAL_BUTTONS; bIndex++)
    {
        /*
        if (bIndex == OSDDLG_MEDIA_SELECT_DEFAULT_HIGHLIGHT_BUTTON)
            _OSDDLG_MEDIA_SELECT_DrawButton(bIndex, OSDDLG_BUTTON_STATE_HIGHLIGHT);
        else
        */
        _OSDDLG_MEDIA_SELECT_DrawButton(bIndex, OSDDLG_BUTTON_STATE_NORMAL);
    }
}

//********************************************************************************************************
//  Function    :   _OSDDLG_MEDIA_SELECT_InitializeGDI
//  Abstract    :   This function will initialize the palette.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDDLG_MEDIA_SELECT_InitializeGDI(void)
{    
    GDI_ChangePALEntry(OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_GENERAL_BACKGROUND, OSDDLG_MEDIA_SELECT_PALETTE_COLOR_GENERAL_BACKGROUND, TRUE);
    GDI_ChangePALEntry(OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_HIGHLIGHT_BUTTON, OSDDLG_MEDIA_SELECT_PALETTE_COLOR_HIGHLIGHT_BUTTON, FALSE);
    GDI_ChangePALEntry(OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_NORMAL_BUTTON, OSDDLG_MEDIA_SELECT_PALETTE_COLOR_NORMAL_BUTTON, FALSE);
    GDI_ChangePALEntry(OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_HIGHLIGHT_BUTTON_STRING, OSDDLG_MEDIA_SELECT_PALETTE_COLOR_HIGHLIGHT_BUTTON_STRING, FALSE);
    GDI_ChangePALEntry(OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_NORMAL_BUTTON_STRING, OSDDLG_MEDIA_SELECT_PALETTE_COLOR_NORMAL_BUTTON_STRING, FALSE);

    GDI_WaitPaletteComplete();

    //GDI_SetMixRatio(OSDDLG_MEDIA_SELECT_DLG_MIX_RATIO_BACKGROUND); //The mix ration just effects the background
    
    GDI_ActivateRegion(0); //open GDI
}

//*********************************************************************************************************
//  Function    :   _OSDDLG_MEDIA_SELECT_FillButtonString
//  Abstract    :   This function will fill the string of the button.
//  Arguments   :   bIndex: the index of the button.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDDLG_MEDIA_SELECT_FillButtonString(BYTE bIndex)
{
    switch (bIndex)
    {
    case OSDDLG_MEDIA_SELECT_BUTTON_DVDROM:
        __pOSDUTLOutputString = aDLGBtnDVDROM;
        break;
#ifdef SUPPORT_STB        
    case OSDDLG_MEDIA_SELECT_BUTTON_STB:
        __pOSDUTLOutputString = aDLGBtnSTB;      
        break;
#endif        
#ifdef SUPPORT_USB_SOURCE
    case OSDDLG_MEDIA_SELECT_BUTTON_OTHERS:
        __pOSDUTLOutputString = aDLGBtnUSB;
        break;
#endif //
#ifdef SUPPORT_BUILDIN_CARD_READER
    case OSDDLG_MEDIA_SELECT_BUTTON_CARD_READER:
        __pOSDUTLOutputString = aDLGBtnCardReader;
        break;
#endif //SUPPORT_BUILDIN_CARD_READER

#ifdef SUPPORT_AV_IN
    case OSDDLG_MEDIA_SELECT_BUTTON_AVIN:
        __pOSDUTLOutputString = aDLGBtnAVIN;      
        break;
#endif// SUPPORT_AV_IN
    default:
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _OSDDLG_MEDIA_SELECT_GetMaxButtonWidth
//  Abstract    :   This function will fill the string of the button.
//  Arguments   :   none.
//  Return		:   Max. button width.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
WORD _OSDDLG_MEDIA_SELECT_GetMaxButtonWidth(void)
{
    _wOSDDLGTemp1 = 0; //Use it and max. width.

    for (_bOSDDLGTemp = 0; _bOSDDLGTemp < OSDDLG_MEDIA_SELECT_TOTAL_BUTTONS; _bOSDDLGTemp++)
    {
        //Fill string
        _OSDDLG_MEDIA_SELECT_FillButtonString(_bOSDDLGTemp);

        _wOSDDLGTemp = GDI_GetStringWidth_909(__pOSDUTLOutputString);

        if (_wOSDDLGTemp1 < _wOSDDLGTemp)
            _wOSDDLGTemp1 = _wOSDDLGTemp;
    }

    _wOSDDLGTemp1 += (OSDDLG_MEDIA_SELECT_BUTTON_TEXT_DISTANCE_H << 1);

    return _wOSDDLGTemp1;
}


//*********************************************************************************************************
//  Function    :   _OSDDLG_MEDIA_SELECT_GetDlgHeight
//  Abstract    :   This function will calculate the height of the dialog box.
//  Arguments   :   none.
//  Return		:   The height of the dialog box.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
WORD _OSDDLG_MEDIA_SELECT_GetDlgHeight(void)
{
    _wOSDDLGTemp1 = 0;

    for (_bOSDDLGTemp = 0; _bOSDDLGTemp < OSDDLG_MEDIA_SELECT_TOTAL_BUTTONS; _bOSDDLGTemp++)
    {
        _wOSDDLGTemp1 += OSDDLG_MEDIA_SELECT_BUTTON_HEIGHT;
    }

    _wOSDDLGTemp1 += OSDDLG_MEDIA_SELECT_BUTTON_BUTTON_DISTANCE_V * (OSDDLG_MEDIA_SELECT_TOTAL_BUTTONS - 1);

    _wOSDDLGTemp1 += (OSDDLG_MEDIA_SELECT_BACKGROUND_BUTTON_DISTANCE_V << 1);

    return _wOSDDLGTemp1;
}

//*********************************************************************************************************
//  Function    :   _OSDDLG_MEDIA_SELECT_DrawButton
//  Abstract    :   This function will draw the button.
//  Arguments   :   bIndex: the index of the button. bButtonState: The state of the button.  
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDDLG_MEDIA_SELECT_DrawButton(BYTE bIndex, BYTE bButtonState)
{    
    _wOSDDLGButtonWidth = _OSDDLG_MEDIA_SELECT_GetMaxButtonWidth();

    if (bButtonState == OSDDLG_BUTTON_STATE_HIGHLIGHT)
        _OSDDLGRect.bColor = OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_HIGHLIGHT_BUTTON;    
    else
        _OSDDLGRect.bColor = OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_NORMAL_BUTTON;    

    _OSDDLGRect.rect.wLeft = _OSDDLGAttribute.wStartH+OSDDLG_MEDIA_SELECT_BACKGROUND_BUTTON_DISTANCE_H;
    _OSDDLGRect.rect.wTop = _OSDDLGAttribute.wStartV+OSDDLG_MEDIA_SELECT_BACKGROUND_BUTTON_DISTANCE_V+(bIndex*(OSDDLG_MEDIA_SELECT_BUTTON_HEIGHT+OSDDLG_MEDIA_SELECT_BUTTON_BUTTON_DISTANCE_V));
    _OSDDLGRect.rect.wRight = _OSDDLGRect.rect.wLeft+_wOSDDLGButtonWidth;
    _OSDDLGRect.rect.wBottom = _OSDDLGRect.rect.wTop+OSDDLG_MEDIA_SELECT_BUTTON_HEIGHT;

    GDI_FillRect_909(0, &_OSDDLGRect);

    //Fill string
    _OSDDLG_MEDIA_SELECT_FillButtonString(bIndex);

    //Draw string.
    if (bButtonState == OSDDLG_BUTTON_STATE_HIGHLIGHT)
        _OSDDLGStringAttr.bBackground = OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_HIGHLIGHT_BUTTON;
    else
        _OSDDLGStringAttr.bBackground = OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_NORMAL_BUTTON;

    _OSDDLGStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDDLGStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDDLGStringAttr.bColorKey = 0;

    if (bButtonState == OSDDLG_BUTTON_STATE_HIGHLIGHT)
        _OSDDLGStringAttr.bTextColor = OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_HIGHLIGHT_BUTTON_STRING;
    else
        _OSDDLGStringAttr.bTextColor = OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_NORMAL_BUTTON_STRING;

    _OSDDLGStringAttr.wX = _OSDDLGRect.rect.wLeft + ((_wOSDDLGButtonWidth - GDI_GetStringWidth_909(__pOSDUTLOutputString)) >> 1);
    _OSDDLGStringAttr.wY = _OSDDLGRect.rect.wTop + ((OSDDLG_MEDIA_SELECT_BUTTON_HEIGHT - GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);
        
    GDI_DrawString_909(0, &_OSDDLGStringAttr, __pOSDUTLOutputString);
}

//*********************************************************************************************************
//  Function    :   _OSDDLG_MEDIA_SELECT_UpdateDlgBackground
//  Abstract    :   This function will draw/clear the dialog box.
//  Arguments   :   OSDDLG_UPDATE_DRAW/OSDDLG_UPDATE_CLEAR. 
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   _OSDDLGAttribute should be initialized before calling this function.
//*********************************************************************************************************
void _OSDDLG_MEDIA_SELECT_UpdateDlgBackground(BYTE bUpdate)
{
    if (bUpdate == OSDDLG_UPDATE_DRAW)
        _OSDDLGRect.bColor = OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_GENERAL_BACKGROUND;    
    else
        _OSDDLGRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;    

    _OSDDLGRect.rect.wLeft = _OSDDLGAttribute.wStartH;
    _OSDDLGRect.rect.wTop = _OSDDLGAttribute.wStartV;
    _OSDDLGRect.rect.wRight = _OSDDLGRect.rect.wLeft + _OSDDLGAttribute.wWidth;
    _OSDDLGRect.rect.wBottom = _OSDDLGRect.rect.wTop+_OSDDLGAttribute.wHeight;
    GDI_FillRect_909(0, &_OSDDLGRect);
}

//*********************************************************************************************************
//  Function    :   OSDDLG_MEDIA_SELECT_Recover
//  Abstract    :   This function will be called by OSD to do this UI's recovery.
//  Arguments   :   bRecoverRegion: OSD_RECOVER_UPPER_REGION/OSD_RECOVER_BOTTOM_REGION/OSD_RECOVER_ALL
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
BYTE OSDDLG_MEDIA_SELECT_Recover(BYTE bRecoverRegion)
{
    if (bRecoverRegion == OSD_RECOVER_ALL)
    {
        _OSDDLG_MEDIA_SELECT_ShowDialogBox(OSDDLG_SHOW_DLG_RECOVER, OSDDLG_MEDIA_SELECT_BACKGROUND_GENERAL_MODE);
        return TRUE;
    }
    else
        return FALSE;
}











#define OSDCOMDLG_MAIN_FILE
//#include "string.lan"
#include "OSDString\\strextrn.h"

#define FRAME_THICKNESS      2
#define OSDCOMDLG_MARGIN_H   50
#define OSDCOMDLG_MARGIN_V   20
#define LINE_HEIGHT       30


#define ButtonWidth       90
#define HIGHTLIGHT_BORDER 4
#define BOTTOM_MARGIN     25
#define BUTTON_SPACE      5




PARM_RECT        _DLG_RectAttr;       // For clearing UI
PARM_FRAME       _DLG_FrameAttr;
PARM_DRAW_STRING _DLG_StrAttr;

TIMEOUT_FUNC _pTimeoutFunc = NULL;
CALLBACK_FUNC _pCallbackFunc = NULL;
RECOVER_FUNC  _pRecoverFunc = NULL;
WORD*     _pwString = NULL;
BYTE      _bButtonNum = 0;
BYTE      _bSelectButton = BUTTON_OK_YES;
BOOL      _bOK_Cancel_ButtonType = TRUE;
BOOL      _bDialogShowUp = FALSE;
BYTE      _bWaitingTime = 0;
DWORD     _dwLastTime = 0;

extern BYTE __bSetupMode;

extern void _OSDCOMDLG_InitPalette(void);
extern void _OSDCOMDLG_ClearUI(void);
extern void _OSDCOMDLG_DrawButtons(void);
extern void _OSDCOMDLG_HighlightButton(BYTE bButton);
extern void _OSDCOMDLG_ClearButtonsArea(void);
extern BOOL _OSDCOMDLG_DrawStringWithAutoWrap(WORD wX, WORD wY, WORD* pwString);

//**********************************************************************
//  Function    :   OSDCOMDLG_Set_Buttons_Type 
//  Abstract    :   This function will set to show
//                  "OK-Cancel" or "Yes-No" buttons.
//  Arguments   :   bOK_Cancel_Buttons : 
//                                TRUE : Show "OK-Cancel" buttons
//                               FALSE : Show "Yes-No" buttons
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

void OSDCOMDLG_Set_Buttons_Type(BOOL bOK_Cancel_Buttons)
{
    _bOK_Cancel_ButtonType = bOK_Cancel_Buttons;
}


//**********************************************************************
//  Function    :   OSDCOMDLG_SetRect 
//  Abstract    :   This function sets the dialog size 
//  Arguments   :   wLeft : left point
//                  wTop  : top point 
//                  wRight: right point 
//                  wBottom: bottom point 
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

void OSDCOMDLG_SetRect(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom)
{
    
    _DLG_FrameAttr.bStyle = FRAME_POP_SOLID;
    _DLG_FrameAttr.bThickness = FRAME_THICKNESS;
    _DLG_FrameAttr.bColorBright = OSDCOMDLG_ENTRY_FRAME_BRIGHT;
    _DLG_FrameAttr.bColorDark   = OSDCOMDLG_ENTRY_FRAME_DARK;
    _DLG_FrameAttr.bColorCenter = OSDCOMDLG_ENTRY_FRAME_CENTER;
    _DLG_FrameAttr.rect.wLeft = wLeft;
    _DLG_FrameAttr.rect.wRight = wRight; 
    _DLG_FrameAttr.rect.wTop = wTop; 
    _DLG_FrameAttr.rect.wBottom = wBottom; 

    _DLG_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _DLG_RectAttr.rect.wLeft = wLeft;
    _DLG_RectAttr.rect.wRight = wRight; 
    _DLG_RectAttr.rect.wTop = wTop; 
    _DLG_RectAttr.rect.wBottom = wBottom; 
}


//**********************************************************************
//  Function    :   OSDCOMDLG_SetButtonNum 
//  Abstract    :   This function sets the dialog size 
//  Arguments   :   nButtonNum : set the button number. It must be less than 3 
//  Return	:   TRUE : Set successfully.  FALSE : Fail to set
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

BOOL OSDCOMDLG_SetButtonNum(BYTE nButtonNum)
{
    if (2 < nButtonNum) return FALSE;
    _bButtonNum = nButtonNum;
    return TRUE;
}


//**********************************************************************
//  Function    :   OSDCOMDLG_SetMessage 
//  Abstract    :   This function sets the strings to be display
//  Arguments   :   pwString : The pointer to the string
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

void OSDCOMDLG_SetMessage(WORD* pwString)
{
    _pwString = pwString;
}


//**********************************************************************
//  Function    :   OSDCOMDLG_Select_OK_YES_Button 
//  Abstract    :   Select the "OK" or "Yes" button
//  Arguments   :   none.
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

void OSDCOMDLG_Select_OK_YES_Button(void)
{
    _bSelectButton = BUTTON_OK_YES;
    _OSDCOMDLG_ClearButtonsArea();
    _OSDCOMDLG_HighlightButton(BUTTON_OK_YES);
    _OSDCOMDLG_DrawButtons();
}

//**********************************************************************
//  Function    :   OSDCOMDLG_Select_Cancel_NO_Button 
//  Abstract    :   Select the "Cancel" or "No" button
//  Arguments   :   none.
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

void OSDCOMDLG_Select_Cancel_NO_Button(void)
{
    if (1 == _bButtonNum) return;
    _bSelectButton = BUTTON_CANCEL_NO;
    _OSDCOMDLG_ClearButtonsArea();
    _OSDCOMDLG_HighlightButton(BUTTON_CANCEL_NO);
    _OSDCOMDLG_DrawButtons();
}

//**********************************************************************
//  Function    :   OSDCOMDLG_DrawString 
//  Abstract    :   After the dialog is drawn, call this function to draw the
//                  the string at the specified position
//  Arguments   :   wX, wY : the position
//                  pwString : The pointer to the string
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

void OSDCOMDLG_DrawString(WORD wX, WORD wY, WORD *pwString)
{
    _DLG_StrAttr.wX = _DLG_FrameAttr.rect.wLeft + wX;
    _DLG_StrAttr.wY = _DLG_FrameAttr.rect.wTop + wY;
    _DLG_StrAttr.bTextColor = OSDCOMDLG_ENTRY_STRING;
    _DLG_StrAttr.bBackground = _DLG_FrameAttr.bColorCenter;
    //GDI_DrawString_909(0, &_DLG_StrAttr, pwString);
    _OSDDLG_DrawDynamicString(pwString);
}


//**********************************************************************
//  Function    :   OSDCOMDLG_DrawDialog 
//  Abstract    :   Draw the dialog
//  Arguments   :   none.
//  Return	:   TRUE: Success.
//                  FALSE: Fail
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

BOOL OSDCOMDLG_DrawDialog(void)
{
    if (TRUE == _bDialogShowUp) return FALSE;

    if (!__bSetupMode)
        OSD_ChangeUI(OSD_UI_COMMON_DLG, OSD_ENTER_UI);

    _OSDCOMDLG_InitPalette();

    _DLG_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    GDI_FillRect_909(0, &_DLG_RectAttr);

    GDI_DrawFrame_909(0, &_DLG_FrameAttr);

    if (NULL != _pwString)
    {
        _OSDCOMDLG_DrawStringWithAutoWrap(OSDCOMDLG_MARGIN_H,
                                          OSDCOMDLG_MARGIN_V, _pwString);
    }

    _OSDCOMDLG_DrawButtons();


    _bDialogShowUp = TRUE;
    _dwLastTime = OS_GetSysTimer();
    return TRUE;
}


//**********************************************************************
//  Function    :   OSDCOMDLG_ProcessKey
//  Abstract    :   Process Key
//  Arguments   :   none.
//  Return	:   KEY_INVALID, KEY_TODO, KEY_BYPASS, KEY_NO_KEY.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************
//BYTE OSDCOMDLG_ProcessKey(BYTE bKey)
BYTE OSDCOMDLG_ProcessKey()
{
    BYTE bKeyStatus;

    if (FALSE == _bDialogShowUp) return KEY_BYPASS;

        switch (__bKey)   //Alan085, for common dialog
        {
            case KEY_UP:
            case KEY_RIGHT:
                OSDCOMDLG_Select_Cancel_NO_Button();
                bKeyStatus = KEY_NO_KEY;
                break;
            case KEY_DOWN:
            case KEY_LEFT:
                OSDCOMDLG_Select_OK_YES_Button();
                bKeyStatus = KEY_NO_KEY;
                break;
            case KEY_ACTIVE:
            case KEY_ENTER:
            case KEY_PLAY:
            case KEY_PLAY_PAUSE:  // LLY1.07, forget to process play & pause key.
                if (__bSetupMode)
                    OSDCOMDLG_Exit(OSD_UI_EXIT_CONTROL_ONLY);
                else 
                    OSDCOMDLG_Exit(OSD_UI_EXIT_ALL);
                bKeyStatus = KEY_TODO;
                if (_pCallbackFunc)
                    bKeyStatus = (_pCallbackFunc)(); 
                else
                    bKeyStatus = KEY_TODO;
                break;
            case KEY_POWER:
            case KEY_OPEN_CLOSE:
                OSDCOMDLG_Exit(OSD_UI_EXIT_ALL);
                bKeyStatus = KEY_BYPASS;
                break;
             default:
                bKeyStatus = KEY_NO_KEY;
// If return KEY_INVALID, the top part of Setup UI will be cut 
//                bKeyStatus = KEY_INVALID;
         }
    return bKeyStatus;
}


//**********************************************************************
//  Function    :   OSDCOMDLG_Get_Select_Button
//  Abstract    :   Query which button is selected 
//  Arguments   :   none.
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

BYTE OSDCOMDLG_Get_Select_Button(void)
{
    return _bSelectButton;
}


//**********************************************************************
//  Function    :   OSDCOMDLG_Set_WaitingTime
//  Abstract    :   Set the time to count down
//  Arguments   :   bSecond : how many second to count down
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//**********************************************************************

void OSDCOMDLG_Set_WaitingTime(BYTE bSecond)
{
    _bWaitingTime = bSecond;
}

//**********************************************************************
//  Function    :   OSDCOMDLG_Register_TimeoutFunc 
//  Abstract    :   Set the callback function when the time is out 
//  Arguments   :   pTimeoutFunc : callback function
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

void OSDCOMDLG_Register_TimeoutFunc(TIMEOUT_FUNC pTimeoutFunc)
{
    _pTimeoutFunc = pTimeoutFunc;
}

//**********************************************************************
//  Function    :   OSDCOMDLG_Register_CallbackFunc 
//  Abstract    :   Set the callback function when button is pressed 
//  Arguments   :   pCallbackFunc : callback function
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

void OSDCOMDLG_Register_CallbackFunc(CALLBACK_FUNC pCallbackFunc)
{
    _pCallbackFunc = pCallbackFunc;
}

//**********************************************************************
//  Function    :   OSDCOMDLG_Register_RecoverFunc 
//  Abstract    :   Set the recover function when the dialog is recovered 
//  Arguments   :   pRecoverFunc : callback function
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

void OSDCOMDLG_Register_RecoverFunc(RECOVER_FUNC pRecoverFunc)
{
    _pRecoverFunc = pRecoverFunc;
}

//**********************************************************************
//  Function    :   OSDCOMDLG_Exit 
//  Abstract    :   Clear the dialog
//  Arguments   :   OSD_UI_EXIT_ALL : UI is clear
//                  OSD_UI_EXIT_CONTROL_ONLY : UI is not clear.
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

void OSDCOMDLG_Exit(BYTE bExitMode)
{
    switch (bExitMode)
    {
    case OSD_UI_EXIT_ALL:
        _OSDCOMDLG_ClearUI();
        break;
    case OSD_UI_EXIT_CONTROL_ONLY:
    default:
        break;
    }
    _bDialogShowUp = FALSE;
    _pTimeoutFunc = NULL;
    _pwString = NULL;
}


//**********************************************************************
//  Function    :   OSDCOMDLG_Recover 
//  Abstract    :   Recover the dialog
//  Arguments   :   bRecoverRegion
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************

BYTE OSDCOMDLG_Recover(BYTE bRecoverRegion)
{
    if (OSD_RECOVER_ALL != bRecoverRegion) return FALSE;

    _OSDCOMDLG_InitPalette();

    GDI_FillRect_909(0, &_DLG_RectAttr);

    GDI_DrawFrame_909(0, &_DLG_FrameAttr);

    if (NULL != _pwString)
    {
        _OSDCOMDLG_DrawStringWithAutoWrap(OSDCOMDLG_MARGIN_H,
                                          OSDCOMDLG_MARGIN_V, _pwString);
    }

    _OSDCOMDLG_DrawButtons();

/*
    OSDCOMDLG_Exit(OSD_UI_EXIT_CONTROL_ONLY);
    OSD_ChangeUI(OSD_UI_COMMON_DLG, OSD_EXIT_UI);
    if (_pRecoverFunc)
        (_pRecoverFunc)(); 
*/

    return TRUE;
}


//**********************************************************************
//  Function    :   OSDCOMDLG_ConfigOSDRegion
//  Abstract    :   Set the OSD region
//  Arguments   :   none.
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************
void OSDCOMDLG_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;
#ifdef  BORROW_FRAME_BUFFER
    DWORD dwBufferAddr;
#endif //

    if (!__bSetupMode)
    {
        RegionInfo.bColorMode = GDI_OSD_8B_MODE;
#ifdef ENLARGE_OSD_FOR_PAL_MODE
        RegionInfo.wWidth = GDI_REGION_WIDTH;
        if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
        {
            RegionInfo.wHeight = GDI_REGION_HEIGHT;
        }
        else
        {
            RegionInfo.wHeight = GDI_REGION_HEIGHT_PAL;
        }
        RegionInfo.dwTAddr = DS_OSDFRAME_ST;
        OSD_SetRegion(0, FALSE, &RegionInfo);
#else
#ifdef  BORROW_FRAME_BUFFER

        GDI_ClearRegion(0); //Alan2.30, fix mantis bug #1886

    if (_DLG_FrameAttr.rect.wRight - _DLG_FrameAttr.rect.wLeft <= __RegionList[0].wWidth  &&
        _DLG_FrameAttr.rect.wBottom - _DLG_FrameAttr.rect.wTop <= __RegionList[0].wHeight &&
        GDI_OSD_8B_MODE == __RegionList[0].bColorMode)
    {
        // Use OSD buffer and Frame buffer 0
        RegionInfo.dwTAddr = DS_OSDFRAME_ST;
    }
    else
    {
        // Use Frame buffer 1 or 2
        dwBufferAddr = DISP_QueryAvailableBuffer();
        if (0 == dwBufferAddr)
        {
            RegionInfo.dwTAddr = DS_OSDFRAME_ST;
            printf("DISP can't find an available buffer for OSD region\n");
        }
        else
        {
            RegionInfo.dwTAddr = dwBufferAddr;
        }
    }
    RegionInfo.wWidth = GDI_REGION_WIDTH;
    RegionInfo.wHeight = _DLG_FrameAttr.rect.wBottom;
        OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    RegionInfo.wWidth = GDI_REGION_WIDTH;
    RegionInfo.wHeight = GDI_REGION_HEIGHT;
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
        OSD_SetRegion(0, FALSE, &RegionInfo);
#endif 
#endif  // #ifdef ENLARGE_OSD_FOR_PAL_MODE
    }
}


//**********************************************************************
//  Function    :   OSDCOMDLG_Trigger 
//  Abstract    :   Trigger function 
//  Arguments   :   bType : only OSD_TRIGGER_CONTROL is valid.
//                  wTime : the time 
//  Return	:   none.
//  Side Effect :   none.
//  Notes       :   
//**********************************************************************
// LLY2.53, expand time value unit from WORD to DWORD
// Since, we will use [23:16]:hour, [15:8]: min, [7:0]: sec
//void OSDCOMDLG_Trigger(BYTE bType, WORD wTime)
void OSDCOMDLG_Trigger(BYTE bType, DWORD dwTime)
{
        if (0 == _bWaitingTime) return;
        if (OSD_TRIGGER_CONTROL != bType) return;

        if (OS_GetSysTimer() - _dwLastTime < COUNT_1_SEC) 
            return;

        if (_bWaitingTime > 0) 
        {
            _bWaitingTime--;

            OSDUTL_PrepareOutputString();
            OSDUTL_OutputNumFromIndex((WORD)_bWaitingTime);
            
            _DLG_StrAttr.bTextColor = OSDND_PALETTE_ENTRY_MUTE_ICON_SHADOW_2;
            _DLG_StrAttr.bBackground = OSDND_PALETTE_ENTRY_MUTEOFF_ICON_SHADOW_2;

            _DLG_StrAttr.wX = (_DLG_FrameAttr.rect.wLeft + _DLG_FrameAttr.rect.wRight) / 2;
            _DLG_StrAttr.wY =  _DLG_FrameAttr.rect.wBottom - 4 * GDI_GetStringWidth(aOSDCOMDLG_OK); 

            //GDI_DrawString_909(0, &_DLG_StrAttr, __wOSDUTLStringBuf);
            _OSDDLG_DrawDynamicString(__wOSDUTLStringBuf);

            //Alex1.07a 20031009 if the waittime < 0 , stop the pscan
            if(_bWaitingTime <= 0)
            {
                if (_pTimeoutFunc)
                    (_pTimeoutFunc)(); 
            }
            _dwLastTime = OS_GetSysTimer();
        }
}


/////////////////////// Private Functions ////////////////////////////////

void _OSDCOMDLG_InitPalette(void)
{
    GDI_ChangePALEntry(OSDCOMDLG_ENTRY_BUTTON, OSDCOMDLG_VALUE_COLOR_BUTTON, FALSE);
    GDI_ChangePALEntry(OSDCOMDLG_ENTRY_HIGHLIGHT, OSDCOMDLG_VALUE_COLOR_HIGHLIGHT, FALSE);
    GDI_ChangePALEntry(OSDCOMDLG_ENTRY_STRING, OSDCOMDLG_VALUE_COLOR_STRING, FALSE);
    GDI_ChangePALEntry(OSDCOMDLG_ENTRY_FRAME_BRIGHT, OSDCOMDLG_VALUE_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(OSDCOMDLG_ENTRY_FRAME_DARK, OSDCOMDLG_VALUE_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(OSDCOMDLG_ENTRY_FRAME_CENTER, OSDCOMDLG_VALUE_FRAME_CENTER, FALSE);
    GDI_WaitPaletteComplete();
}


void _OSDCOMDLG_ClearUI(void)
{
    GDI_FillRect_909(0, &_DLG_RectAttr);
    OSD_ChangeUI(OSD_UI_COMMON_DLG, OSD_EXIT_UI);
}

void _OSDCOMDLG_DrawButtons(void)
{
    PARM_RECT _DLG_ButtonAttr;
    WORD wCenterX;
    WORD wStrLength = 0;

    wCenterX = (_DLG_FrameAttr.rect.wLeft + _DLG_FrameAttr.rect.wRight) / 2;

    if (1 == _bButtonNum) 
    {
        if (_bOK_Cancel_ButtonType) 
            wStrLength = GDI_GetStringWidth(aOSDCOMDLG_OK);
        else
            wStrLength = GDI_GetStringWidth(aOSDCOMDLG_Yes);

        _DLG_ButtonAttr.bColor = OSDCOMDLG_ENTRY_BUTTON;
        _DLG_ButtonAttr.rect.wLeft = wCenterX - ButtonWidth/2 +
                                     HIGHTLIGHT_BORDER;
        _DLG_ButtonAttr.rect.wRight = wCenterX + ButtonWidth/2 - 
                                      HIGHTLIGHT_BORDER;
        _DLG_ButtonAttr.rect.wBottom = _DLG_FrameAttr.rect.wBottom -
                                       BOTTOM_MARGIN - HIGHTLIGHT_BORDER;
        _DLG_ButtonAttr.rect.wTop = _DLG_FrameAttr.rect.wBottom - BOTTOM_MARGIN -
                                    GDI_GetStringHeight(aOSDCOMDLG_Cancel) -
                                    HIGHTLIGHT_BORDER;
        GDI_FillRoundRect(0, 13, &_DLG_ButtonAttr);

        _DLG_StrAttr.wX = wCenterX - wStrLength/2;
        _DLG_StrAttr.wY = _DLG_ButtonAttr.rect.wTop;
        _DLG_StrAttr.bTextColor = OSDND_PALETTE_ENTRY_MUTE_ICON_SHADOW_2;
        _DLG_StrAttr.bBackground = OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING;
        if (_bOK_Cancel_ButtonType) 
            //GDI_DrawString_909(0, &_DLG_StrAttr, aOSDCOMDLG_OK);
            _OSDDLG_DrawDynamicString(aOSDCOMDLG_OK);
        else
            //GDI_DrawString_909(0, &_DLG_StrAttr, aOSDCOMDLG_Yes);
            _OSDDLG_DrawDynamicString(aOSDCOMDLG_Yes);
    }
    else if (2 == _bButtonNum)
    {
        if (_bOK_Cancel_ButtonType)
            wStrLength = GDI_GetStringWidth(aOSDCOMDLG_OK);
        else 
            wStrLength = GDI_GetStringWidth(aOSDCOMDLG_Yes);

        _DLG_ButtonAttr.bColor = OSDCOMDLG_ENTRY_BUTTON;
        _DLG_ButtonAttr.rect.wLeft = wCenterX - BUTTON_SPACE - ButtonWidth +
                                     HIGHTLIGHT_BORDER;
        _DLG_ButtonAttr.rect.wRight = wCenterX - BUTTON_SPACE -
                                      HIGHTLIGHT_BORDER;
        _DLG_ButtonAttr.rect.wBottom = _DLG_FrameAttr.rect.wBottom -
                                       BOTTOM_MARGIN - HIGHTLIGHT_BORDER;
        _DLG_ButtonAttr.rect.wTop = _DLG_FrameAttr.rect.wBottom - BOTTOM_MARGIN -
                                    GDI_GetStringHeight(aOSDCOMDLG_Cancel) - 
                                    HIGHTLIGHT_BORDER;
        GDI_FillRoundRect(0, 13, &_DLG_ButtonAttr);

        _DLG_StrAttr.wX = (_DLG_ButtonAttr.rect.wLeft + 
                           _DLG_ButtonAttr.rect.wRight)/2 - wStrLength/2;
        _DLG_StrAttr.wY = _DLG_ButtonAttr.rect.wTop;
        _DLG_StrAttr.bTextColor = OSDND_PALETTE_ENTRY_MUTE_ICON_SHADOW_2;
        _DLG_StrAttr.bBackground = _DLG_ButtonAttr.bColor;
        if (_bOK_Cancel_ButtonType) 
            //GDI_DrawString_909(0, &_DLG_StrAttr, aOSDCOMDLG_OK);
            _OSDDLG_DrawDynamicString(aOSDCOMDLG_OK);
        else
            //GDI_DrawString_909(0, &_DLG_StrAttr, aOSDCOMDLG_Yes);
            _OSDDLG_DrawDynamicString(aOSDCOMDLG_Yes);


        if (_bOK_Cancel_ButtonType)
            wStrLength = GDI_GetStringWidth(aOSDCOMDLG_Cancel);
        else
            wStrLength = GDI_GetStringWidth(aOSDCOMDLG_No);

        _DLG_ButtonAttr.rect.wLeft = wCenterX + BUTTON_SPACE +
                                     HIGHTLIGHT_BORDER;
        _DLG_ButtonAttr.rect.wRight = wCenterX + BUTTON_SPACE +
                                      ButtonWidth - HIGHTLIGHT_BORDER;
        GDI_FillRoundRect(0, 13, &_DLG_ButtonAttr);

        _DLG_StrAttr.wX = (_DLG_ButtonAttr.rect.wLeft + 
                           _DLG_ButtonAttr.rect.wRight)/2 -
                          (wStrLength/2);
        if (_bOK_Cancel_ButtonType) 
            //GDI_DrawString_909(0, &_DLG_StrAttr, aOSDCOMDLG_Cancel);
            _OSDDLG_DrawDynamicString(aOSDCOMDLG_Cancel);
        else
            //GDI_DrawString_909(0, &_DLG_StrAttr, aOSDCOMDLG_No);
            _OSDDLG_DrawDynamicString(aOSDCOMDLG_No);
    }
}


void _OSDCOMDLG_HighlightButton(BYTE bButton)
{
    PARM_RECT _DLG_ButtonAttr;
    WORD wCenterX;

    wCenterX = (_DLG_FrameAttr.rect.wLeft + _DLG_FrameAttr.rect.wRight) / 2;
    if (2 == _bButtonNum) 
    {
        if (BUTTON_OK_YES == bButton) 
        {
            _DLG_ButtonAttr.bColor = OSDCOMDLG_ENTRY_HIGHLIGHT;
            _DLG_ButtonAttr.rect.wLeft = wCenterX - ButtonWidth - BUTTON_SPACE;
            _DLG_ButtonAttr.rect.wRight = _DLG_ButtonAttr.rect.wLeft +
                                          ButtonWidth;
            _DLG_ButtonAttr.rect.wBottom = _DLG_FrameAttr.rect.wBottom -
                                           BOTTOM_MARGIN;
            _DLG_ButtonAttr.rect.wTop = _DLG_ButtonAttr.rect.wBottom - 
                                        GDI_GetStringHeight(aOSDCOMDLG_Cancel) -
                                        2 * HIGHTLIGHT_BORDER;
            GDI_FillRoundRect(0, 17, &_DLG_ButtonAttr);
        } 
        else
        {
            _DLG_ButtonAttr.bColor = OSDCOMDLG_ENTRY_HIGHLIGHT;
            _DLG_ButtonAttr.rect.wLeft = wCenterX + BUTTON_SPACE;
            _DLG_ButtonAttr.rect.wRight = _DLG_ButtonAttr.rect.wLeft +
                                          ButtonWidth;
            _DLG_ButtonAttr.rect.wBottom = _DLG_FrameAttr.rect.wBottom -
                                           BOTTOM_MARGIN;
            _DLG_ButtonAttr.rect.wTop = _DLG_ButtonAttr.rect.wBottom - 
                                        GDI_GetStringHeight(aOSDCOMDLG_Cancel) - 
                                        2 * HIGHTLIGHT_BORDER;
            GDI_FillRoundRect(0, 17, &_DLG_ButtonAttr);
        }
    }
    else if (1 == _bButtonNum)
    {
        _DLG_ButtonAttr.bColor = OSDCOMDLG_ENTRY_HIGHLIGHT;
        _DLG_ButtonAttr.rect.wLeft = wCenterX - ButtonWidth/2;
        _DLG_ButtonAttr.rect.wRight = _DLG_ButtonAttr.rect.wLeft +
                                      ButtonWidth;
        _DLG_ButtonAttr.rect.wBottom = _DLG_FrameAttr.rect.wBottom -
                                       BOTTOM_MARGIN;
        _DLG_ButtonAttr.rect.wTop = _DLG_ButtonAttr.rect.wBottom - 
                                    GDI_GetStringHeight(aOSDCOMDLG_Cancel) - 
                                    2 * HIGHTLIGHT_BORDER;
        GDI_FillRoundRect(0, 17, &_DLG_ButtonAttr);
    }
}

void _OSDCOMDLG_ClearButtonsArea(void)
{
    PARM_RECT _DLG_ButtonAttr;
    WORD wCenterX;

    wCenterX = (_DLG_FrameAttr.rect.wLeft + _DLG_FrameAttr.rect.wRight) / 2;

    _DLG_ButtonAttr.bColor = OSDCOMDLG_ENTRY_FRAME_CENTER;
    _DLG_ButtonAttr.rect.wLeft = wCenterX - BUTTON_SPACE - ButtonWidth;
    _DLG_ButtonAttr.rect.wRight = wCenterX + BUTTON_SPACE + ButtonWidth;
    _DLG_ButtonAttr.rect.wBottom = _DLG_FrameAttr.rect.wBottom - FRAME_THICKNESS; 
    _DLG_ButtonAttr.rect.wTop = _DLG_ButtonAttr.rect.wBottom - BOTTOM_MARGIN -
                                GDI_GetStringHeight(aOSDCOMDLG_Cancel) -
                                2 * HIGHTLIGHT_BORDER - 1; 
    GDI_FillRect(0, &_DLG_ButtonAttr);
}

BOOL _OSDCOMDLG_DrawStringWithAutoWrap(WORD wX, WORD wY, WORD* pwString)
{
    WORD wSpaceW = (_DLG_FrameAttr.rect.wRight - _DLG_FrameAttr.rect.wLeft) -
                   2*wX;

    if (0 == pwString[0]) return TRUE;

    if (GDI_GetStringWidth(pwString) > wSpaceW)
    {
        WORD  aDrawString[32], wTemp;
        WORD wIndex = 1;
        WORD wCharNum = pwString[0];
        WORD wDrawXPos = wX;
        WORD wDrawYPos = wY;

        while (0 < wCharNum)
        {
            for (wTemp = 0; wTemp < wCharNum; wTemp++) {
                if (CHAR_SPACE == pwString[wIndex + wTemp])
                    break;
            }
            while (CHAR_SPACE == pwString[wIndex + wTemp])
            {
                wTemp++;
            }

            memcpy(&(aDrawString[1]), &(pwString[wIndex]),
                   wTemp*sizeof(WORD)); 
            aDrawString[0] = wTemp;

            if (GDI_GetStringWidth(aDrawString) > wSpaceW) 
            {
                while (CHAR_SPACE == aDrawString[aDrawString[0]] &&
                       aDrawString[0] > 0)
                {
                    aDrawString[0]--;
                    if (GDI_GetStringWidth(aDrawString) <= wSpaceW) 
                        break;
                }
                if (0 == aDrawString[0] || wTemp == aDrawString[0])
                {
                    wDrawXPos= wX;
                    wDrawYPos += LINE_HEIGHT;
                    wSpaceW = (_DLG_FrameAttr.rect.wRight -
                               _DLG_FrameAttr.rect.wLeft) - 2*wX;
                    aDrawString[0] = wTemp;
                    // draw sting
                }
                else
                {
                    if (GDI_GetStringWidth(aDrawString) > wSpaceW)
                    {
                        wDrawXPos= wX;
                        wDrawYPos += LINE_HEIGHT;
                        wSpaceW = (_DLG_FrameAttr.rect.wRight -
                                   _DLG_FrameAttr.rect.wLeft) - 2*wX;
                    }
                }
            }
            else
            {
                    // draw sting
            }
            OSDCOMDLG_DrawString(wDrawXPos, wDrawYPos, aDrawString);
            wSpaceW -= GDI_GetStringWidth(aDrawString);    
            wDrawXPos += GDI_GetStringWidth(aDrawString);
            wIndex += aDrawString[0];
            wCharNum -= aDrawString[0];
        }
    }
    else
    {
        OSDCOMDLG_DrawString(wX, wY, pwString);
    }

    return TRUE;
}

//CoCo1.20, reduce code size.
void _OSDDLG_DrawDynamicString(WORD *pwString)
{
    GDI_DrawString_909(0, &_DLG_StrAttr, pwString);
}

//*********************************************************************************************************
//  Function    :   OSDDLG_ConfigOSDRegion
//  Abstract    :   This function will config the OSD region. And it is exported for OSD kernel usage.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void OSDDLG_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;
#ifdef BORROW_FRAME_BUFFER
    DWORD dwBufferAddr;
#endif //

    RegionInfo.bColorMode = GDI_OSD_8B_MODE;        

    RegionInfo.wWidth = GDI_REGION_WIDTH;
//    RegionInfo.wHeight = OSDDLG_OSD_REGION_HEIGHT;

#ifdef ENLARGE_OSD_FOR_PAL_MODE
    if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
    {
        RegionInfo.wHeight = GDI_REGION_HEIGHT;
    }
    else
    {
        RegionInfo.wHeight = GDI_REGION_HEIGHT_PAL;
    }
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
#else
#ifdef  BORROW_FRAME_BUFFER
    RegionInfo.wHeight = OSDDLG_OSD_REGION_HEIGHT;
    if (GDI_REGION_WIDTH <= __RegionList[0].wWidth  &&
        OSDDLG_OSD_REGION_HEIGHT <= __RegionList[0].wHeight &&
        GDI_OSD_8B_MODE == __RegionList[0].bColorMode)
    {
        // Use OSD buffer and Frame buffer 0
        RegionInfo.dwTAddr = DS_OSDFRAME_ST;
    }
    else
    {
        // Use Frame buffer 1 or 2
        dwBufferAddr = DISP_QueryAvailableBuffer();
        if (0 == dwBufferAddr)
        {
            RegionInfo.dwTAddr = DS_OSDFRAME_ST;
            printf("DISP can't find an available buffer for OSD region\n");
        }
        else
        {
            RegionInfo.dwTAddr = dwBufferAddr;
        }
    }
#else
    RegionInfo.wHeight = GDI_REGION_HEIGHT;
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
#endif // #ifdef  BORROW_FRAME_BUFFER
#endif // #ifdef ENLARGE_OSD_FOR_PAL_MODE

#ifdef  BORROW_FRAME_BUFFER
    GDI_ClearRegion(0); //Alan2.30, fix mantis bug #1886
    OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    OSD_SetRegion(0, FALSE, &RegionInfo);
#endif

}
