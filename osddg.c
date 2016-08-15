#include "winav.h"
#include "ctkav.h"
#include "infofilter.h"
#include "osd.h"
#include "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include "osdnd.h"
#include "cc.h"
#include "setup.h"
#include "hal.h"
#include "Digest.h"
#include "OSDDG.h"
#include "osddlg.h"
#include "notransl.h"

#ifndef NO_DIGEST
#define OSDDG_MAIN_FILE
#endif

//#include "string.lan"
#include "OSDString\\strextrn.h"

#ifndef NO_DIGEST

//***************************************************************************
//Note: The following strings will be translated. And they may be moved to other string file.
/*
WORD aDGDlgBtnTrackDigest[] = {12, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_g, CHAR_e, CHAR_s, CHAR_t};
WORD aDGDlgBtnDiscInterval[] = {13, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_SPACE, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l};
WORD aDGPage[] = {5, CHAR_P, CHAR_a, CHAR_g, CHAR_e, CHAR_COLON};
WORD aDGTitleTrackDigest[] = {12, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_g, CHAR_e, CHAR_s, CHAR_t};
WORD aDGTitleDiscInterval[] = {13, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_SPACE, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l};
WORD aDGInfoTrack[] = {6, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k, CHAR_COLON};
WORD aDGBtnMenu[] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aDGBtnExit[] = {4, CHAR_E, CHAR_x, CHAR_i, CHAR_t};
*/
//***************************************************************************

//***************************************************************************
//Note: Define the palettes for the skin
DWORD _dwOSDDGDlgBackgroundColor[] = {OSDDG_PALETTE_COLOR_DLG_BACKGROUND_1, OSDDG_PALETTE_COLOR_DLG_BACKGROUND_2, OSDDG_PALETTE_COLOR_DLG_BACKGROUND_3, OSDDG_PALETTE_COLOR_DLG_BACKGROUND_4};
DWORD _dwOSDDGDlgHighlightButtonColor[] = {OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_BACKGROUND_1, OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_BACKGROUND_2, OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_BACKGROUND_3, OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_BACKGROUND_4};
DWORD _dwOSDDGDlgEnableButtonColor[] = {OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_BACKGROUND_1, OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_BACKGROUND_2, OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_BACKGROUND_3, OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_BACKGROUND_4};
DWORD _dwOSDDGDlgFrameColor[] = {OSDDG_PALETTE_COLOR_DLG_FRAME_1, OSDDG_PALETTE_COLOR_DLG_FRAME_2, OSDDG_PALETTE_COLOR_DLG_FRAME_3, OSDDG_PALETTE_COLOR_DLG_FRAME_4};
DWORD _dwOSDDGInfoRegionColor[] = {OSDDG_PALETTE_COLOR_INFO_REGION_BACKGROUND_1, OSDDG_PALETTE_COLOR_INFO_REGION_BACKGROUND_2, OSDDG_PALETTE_COLOR_INFO_REGION_BACKGROUND_3, OSDDG_PALETTE_COLOR_INFO_REGION_BACKGROUND_4};
DWORD _dwOSDDGEnableButtonColor[] = {OSDDG_PALETTE_COLOR_ENABLE_BUTTON_BACKGROUND_1, OSDDG_PALETTE_COLOR_ENABLE_BUTTON_BACKGROUND_2, OSDDG_PALETTE_COLOR_ENABLE_BUTTON_BACKGROUND_3, OSDDG_PALETTE_COLOR_ENABLE_BUTTON_BACKGROUND_4};
DWORD _dwOSDDGHighlightButtonColor[] = {OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_BACKGROUND_1, OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_BACKGROUND_2, OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_BACKGROUND_3, OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_BACKGROUND_4};
//***************************************************************************


//Declare global variables.
PARM_RECT _OSDDGRect;
PARM_DRAW_STRING _OSDDGStringAttr;
PARM_DRAW_BITMAP _OSDDGDrawBitmap;
PARM_DRAW_STRING _OSDDGStringAttr;
PARM_FRAME _OSDDGFrame;
BYTE _bOSDDGTemp, _bOSDDGTemp1;
WORD _wOSDDGTemp, _wOSDDGTemp1;
BYTE _bDGTuneFrame_H, _bDGTuneFrame_V;
extern DLG_ATTRIBUTE _OSDDLGAttribute;
extern WORD _wOSDDLGButtonWidth;
extern PARM_RECT _OSDDLGRect;
extern PARM_DRAW_STRING _OSDDLGStringAttr;

extern  BYTE    __bTVType;

//The following variable will be declared in Digest UI and will be removed in the future.
extern DIGESTUI __DigestUIAttr;

//***************************************************************************
//  Function    :   OSDDG_Update
//  Abstract    :   This function will be called by Digest module to draw UI.
//  Arguments   :   bMessage: the message.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSDDG_Update(BYTE bMessage)
{
    switch (bMessage)
    {
    case OSDDG_MSG_DLG_INITIAL:
        _OSDDG_DLG_Initial(FALSE);
        break;
    case OSDDG_MSG_CLEAR_HIGHLIGHT:
        _OSDDG_UpdateHighlight(OSDDG_CLEAR);
        break;
    case OSDDG_MSG_UPDATE_HIGHLIGHT:
        _OSDDG_UpdateHighlight(OSDDG_DRAW);
        break;
    case OSDDG_MSG_DLG_EXIT:
        _OSDDG_DLG_UpdateBackground(OSDDG_CLEAR);
        break;
    case OSDDG_MSG_INITIAL:
        _OSDDG_Initial();
        break;
    case OSDDG_MSG_DRAW_NORMAL_FRAME:
        _OSDDG_DrawFrame(__DigestUIAttr.bCurFrameNo, OSDDG_FRAME_NORMAL);
        break;
    case OSDDG_MSG_DRAW_SELECT_FRAME:
        _OSDDG_DrawFrame(__DigestUIAttr.bCurFrameNo, OSDDG_FRAME_SELECT);
        break;
    case OSDDG_MSG_UPDATE_FRAME_TIME:
        _OSDDG_UpdteFrameInfo(__DigestUIAttr.bCurFrameNo);
        break;
    case OSDDG_MSG_CLEAR_ALL_FRAME:
        _OSDDG_ClearFrameRegion();
        break;
    case OSDDG_MSG_UPDATE_UI_INFO:
        _OSDDG_UpdateInfoRegion();
        break;
    case OSDDG_MSG_CHANGE_TV_MODE:
        _OSDDG_ChangeTVMode();
        break;
    case OSDDG_MSG_EXIT:
        _OSDDG_Exit();
        break;
    case OSDDG_MSG_CHANGE_PAGE:
        _OSDDG_ClearPageInfoRegion();
        _OSDDG_DisplayPageInfo();
        break;
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   OSDDG_Recover
//  Abstract    :   This function will be called by OSD to do Digest UI's recovery
//  Arguments   :   bRecoverRegion: 
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE OSDDG_Recover(BYTE bRecoverRegion)
{
    switch (bRecoverRegion)
    {
    case OSD_RECOVER_UPPER_REGION:
        //Recover the "page info".
        if (__DigestUIAttr.bDigestMode != DIGEST_MODE_DIALOG)
        {
            //Draw the title
            _OSDDG_DisplayTitle();

            _OSDDG_DisplayPageInfo();
        }
        break;
    case OSD_RECOVER_BOTTOM_REGION:
        if (__DigestUIAttr.bDigestMode != DIGEST_MODE_DIALOG)
        {
            //Recover the help string in the info. region.
            _OSDDG_UpdateInfoRegion();
        }
        break;
    case OSD_RECOVER_ALL:
        //Recover all Digest UI.
        _OSDDG_RecoverAll();
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

//***************************************************************************
//  Function    :   OSDDG_ClearRegion
//  Abstract    :   This function will be called by OSD to clear the normal upper 
//              :   region or normal bottom region if the UI accepts the normal display
//              :   message is displayed in its UI.
//  Arguments   :   bClearRegion: OSD_ND_UPPER_RIGHT_REGION/OSD_ND_BOTTOM_REGION.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSDDG_ClearRegion(BYTE bClearRegion)
{
    switch (bClearRegion)
    {
    case OSD_ND_UPPER_RIGHT_REGION:
        //Clear the "page info." region.
        _OSDDG_ClearPageInfoRegion();
        break;
    case OSD_ND_BOTTOM_REGION:
        _OSDDG_DrawInfoRegion();
        break;
    default:
        return;
    }
}

//***************************************************************************
//  Function    :   _OSDDG_InitializeGDI
//  Abstract    :   This function will initialize GDI-relative attributes like palette.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_InitializeGDI(void)
{
    _bOSDDGTemp = __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN;

    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_BACKGROUND, _dwOSDDGDlgBackgroundColor[_bOSDDGTemp], TRUE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_ENABLE_BUTTON_BACKGROUND, _dwOSDDGDlgEnableButtonColor[_bOSDDGTemp], FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_HIGHLIGHT_BUTTON_BACKGROUND, _dwOSDDGDlgHighlightButtonColor[_bOSDDGTemp], FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_FRAME, _dwOSDDGDlgFrameColor[_bOSDDGTemp], FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_INFO_REGION_BACKGROUND, _dwOSDDGInfoRegionColor[_bOSDDGTemp], FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_ENABLE_BUTTON_BACKGROUND, _dwOSDDGEnableButtonColor[_bOSDDGTemp], FALSE); //Will be removed if use Bitmap button
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BACKGROUND, _dwOSDDGHighlightButtonColor[_bOSDDGTemp], FALSE); //Will be removed if use Bitmap button

    //GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_BACKGROUND, OSDDG_PALETTE_COLOR_DLG_BACKGROUND, TRUE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_TITLE_STRING, OSDDG_PALETTE_COLOR_TITLE_STRING, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_HELP_STRING, OSDDG_PALETTE_COLOR_HELP_STRING, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_ENABLE_BUTTON_STRING, OSDDG_PALETTE_COLOR_ENABLE_BUTTON_STRING, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_STRING, OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_STRING, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT, OSDDG_PALETTE_COLOR_NORMAL_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_NORMAL_FRAME_DARK, OSDDG_PALETTE_COLOR_NORMAL_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_SELECT_FRAME_BRIGHT, OSDDG_PALETTE_COLOR_SELECT_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_SELECT_FRAME_DARK, OSDDG_PALETTE_COLOR_SELECT_FRAME_DARK, FALSE);
    //GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_ENABLE_BUTTON_BACKGROUND, OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_BACKGROUND, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_DISABLE_BUTTON_BACKGROUND, OSDDG_PALETTE_COLOR_DLG_DISABLE_BUTTON_BACKGROUND, FALSE);
    //GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_HIGHLIGHT_BUTTON_BACKGROUND, OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_BACKGROUND, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_ENABLE_BUTTON_STRING, OSDDG_PALETTE_COLOR_DLG_ENABLE_BUTTON_STRING, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_DISABLE_BUTTON_STRING, OSDDG_PALETTE_COLOR_DLG_DISABLE_BUTTON_STRING, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_HIGHLIGHT_BUTTON_STRING, OSDDG_PALETTE_COLOR_DLG_HIGHLIGHT_BUTTON_STRING, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_FRAME_INFO_BACKGROUND, OSDDG_PALETTE_COLOR_FRAME_INFO_BACKGROUND, FALSE);
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_FRAME_INFO_STRING, OSDDG_PALETTE_COLOR_FRAME_INFO_STRING, FALSE);
    //GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_INFO_REGION_BACKGROUND, OSDDG_PALETTE_COLOR_INFO_REGION_BACKGROUND, FALSE);
    //GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_ENABLE_BUTTON_BACKGROUND, OSDDG_PALETTE_COLOR_ENABLE_BUTTON_BACKGROUND, FALSE); //Will be removed if use Bitmap button
    //GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BACKGROUND, OSDDG_PALETTE_COLOR_HIGHLIGHT_BUTTON_BACKGROUND, FALSE); //Will be removed if use Bitmap button
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_BUTTON_FRAME, OSDDG_PALETTE_COLOR_BUTTON_FRAME, FALSE); 
    //GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_DLG_FRAME, OSDDG_PALETTE_COLOR_DLG_FRAME, FALSE); 
    GDI_ChangePALEntry(OSDDG_PALETTE_ENTRY_INFO_REGION_FRAME, OSDDG_PALETTE_COLOR_INFO_REGION_FRAME, FALSE); 

    //CoCo0.95
    GDI_WaitPaletteComplete();

    GDI_ActivateRegion(0); //open GDI
}

//***************************************************************************
//  Function    :   _OSDDG_DLG_Initial
//  Abstract    :   This function will initialize Digest's dialog when the dialog is
//              :   pop up.
//  Arguments   :   bRecover: TRUE==>This function is called by the recovery routine.
//              :             FALSE==>This function is called by the normal message.  
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_DLG_Initial(BYTE bRecover)
{
    //Clear the background
    GDI_ClearRegion(0); //clear region 0

    //Tell OSD that Digest UI exits.
    if (!bRecover)
    {
        OSD_ChangeUI(OSD_UI_DIGEST, OSD_ENTER_UI);

        //Initialize the attribute of the dialog box.
        _OSDDLGAttribute.wWidth = _OSDDG_DLG_GetMaxButtonWidth()+(WORD)((OSDDG_DLG_BACKGROUND_BUTTON_DISTANCE_H<<1));
        _OSDDLGAttribute.wHeight = _OSDDG_DLG_GetDlgHeight();
        _OSDDLGAttribute.wStartH = ((OSD_PLANE_WIDTH -  _OSDDLGAttribute.wWidth) >> 1);
        _OSDDLGAttribute.wStartV = ((OSD_PLANE_HEIGHT - _OSDDLGAttribute.wHeight) >> 1);
    }

    _OSDDG_InitializeGDI();

    //Draw the dialog's background.
    _OSDDG_DLG_UpdateBackground(OSDDG_DRAW);

    //Draw the buttons.
    for (_bOSDDGTemp = 0; _bOSDDGTemp < OSDDG_DLG_BUTTON_NUM; _bOSDDGTemp++)
    {
        if (_bOSDDGTemp == __DigestUIAttr.bButtonIndex) //The button is highlighted.
        {
            _OSDDG_DLG_DrawButton(_bOSDDGTemp, TRUE);
        }
        else
        {
            _OSDDG_DLG_DrawButton(_bOSDDGTemp, FALSE);
        }
    }
}

//*********************************************************************************************************
//  Function    :   _OSDDG_DLG_GetMaxButtonWidth
//  Abstract    :   This function will fill the string of the button.
//  Arguments   :   none.
//  Return		:   Max. button width.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
WORD _OSDDG_DLG_GetMaxButtonWidth(void)
{
    BYTE bIndex = 0;

    _wOSDDGTemp1 = 0; //Use it and max. width.

    for (bIndex = 0; bIndex < OSDDG_DLG_BUTTON_NUM; bIndex++)
    {
        //Fill string
        _OSDDG_DLG_FillButtonString(bIndex);

        _wOSDDGTemp = GDI_GetStringWidth_909(__pOSDUTLOutputString);

        if (_wOSDDGTemp1 < _wOSDDGTemp)
            _wOSDDGTemp1 = _wOSDDGTemp;
    }

    _wOSDDGTemp1 += (OSDDG_DLG_BUTTON_TEXT_DISTANCE_H << 1);

    return _wOSDDGTemp1;
}

//*********************************************************************************************************
//  Function    :   _OSDDG_DLG_FillButtonString
//  Abstract    :   This function will fill the string of the button.
//  Arguments   :   bIndex: the index of the button.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDDG_DLG_FillButtonString(BYTE bIndex)
{
    switch (bIndex)
    {
    case OSDDG_DLG_BUTTON_TRACK_DIGEST:
        __pOSDUTLOutputString = aDGDlgBtnTrackDigest;
        break;
    case OSDDG_DLG_BUTTON_BUTTON_DISC_INTERVAL:
        __pOSDUTLOutputString = aDGDlgBtnDiscInterval;
        break;
    default:
        break;
    }
}

//*********************************************************************************************************
//  Function    :   _OSDDG_DLG_GetDlgHeight
//  Abstract    :   This function will calculate the height of the dialog box.
//  Arguments   :   none.
//  Return		:   The height of the dialog box.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
WORD _OSDDG_DLG_GetDlgHeight(void)
{
    _wOSDDGTemp1 = 0;

    for (_bOSDDGTemp = 0; _bOSDDGTemp < OSDDG_DLG_BUTTON_NUM; _bOSDDGTemp++)
    {
        _wOSDDGTemp1 += OSDDG_DLG_BUTTON_HEIGHT;
    }

    _wOSDDGTemp1 += OSDDG_DLG_BUTTON_BUTTON_DISTANCE_V * (OSDDG_DLG_BUTTON_NUM - 1);

    _wOSDDGTemp1 += (OSDDG_DLG_BACKGROUND_BUTTON_DISTANCE_V << 1);

    return _wOSDDGTemp1;
}

//***************************************************************************
//  Function    :   _OSDDG_DLG_UpdateBackground
//  Abstract    :   This function will draw/clear the dialog background.
//  Arguments   :   bUpdate: OSDDG_DRAW/OSDDG_CLEAR.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_DLG_UpdateBackground(BYTE bUpdate)
{
    if (bUpdate == OSDDG_DRAW)
        _OSDDLGRect.bColor = OSDDG_PALETTE_ENTRY_DLG_BACKGROUND;    
    else
        _OSDDLGRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;    

    _OSDDLGRect.rect.wLeft = _OSDDLGAttribute.wStartH;
    _OSDDLGRect.rect.wTop = _OSDDLGAttribute.wStartV;
    _OSDDLGRect.rect.wRight = _OSDDLGRect.rect.wLeft + _OSDDLGAttribute.wWidth;
    _OSDDLGRect.rect.wBottom = _OSDDLGRect.rect.wTop+_OSDDLGAttribute.wHeight;

    GDI_FillRoundRect(0, OSDDG_DLG_ARC_VALUE, &_OSDDLGRect);

    //Fill the frame of round rectangle
    if (bUpdate == OSDDG_DRAW)
        _OSDDLGRect.bColor = OSDDG_PALETTE_ENTRY_DLG_FRAME;
    else
        _OSDDLGRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;

    _OSDDLGRect.rect.wLeft -= OSDDG_DLG_OUTER_INNER_ARC_PIXEL;
    _OSDDLGRect.rect.wTop -= OSDDG_DLG_OUTER_INNER_ARC_PIXEL;
    _OSDDLGRect.rect.wRight += OSDDG_DLG_OUTER_INNER_ARC_PIXEL;
    _OSDDLGRect.rect.wBottom += OSDDG_DLG_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, OSDDG_DLG_OUTER_ARC_VALUE, OSDDG_DLG_ARC_VALUE, &_OSDDLGRect);
}

//*********************************************************************************************************
//  Function    :   _OSDDG_DLG_DrawButton
//  Abstract    :   This function will draw the button.
//  Arguments   :   bIndex: the index of the button. bButtonState: The state of the button.  
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDDG_DLG_DrawButton(BYTE bIndex, BYTE bHightlight)
{    
    _wOSDDLGButtonWidth = _OSDDG_DLG_GetMaxButtonWidth();

    if (bHightlight) //Highlighted button state
    {
        _OSDDLGRect.bColor = OSDDG_PALETTE_ENTRY_DLG_HIGHLIGHT_BUTTON_BACKGROUND;
        _OSDDLGStringAttr.bBackground = OSDDG_PALETTE_ENTRY_DLG_HIGHLIGHT_BUTTON_BACKGROUND;
        _OSDDLGStringAttr.bTextColor = OSDDG_PALETTE_ENTRY_DLG_HIGHLIGHT_BUTTON_STRING;
    }
    else //Normal button state. Enabled/Disabled.
    {
        if (__DigestUIAttr.bButtonEnable[bIndex] == 1)
        {
            _OSDDLGRect.bColor = OSDDG_PALETTE_ENTRY_DLG_ENABLE_BUTTON_BACKGROUND;
            _OSDDLGStringAttr.bBackground = OSDDG_PALETTE_ENTRY_DLG_ENABLE_BUTTON_BACKGROUND;
            _OSDDLGStringAttr.bTextColor = OSDDG_PALETTE_ENTRY_DLG_ENABLE_BUTTON_STRING;
        }
        else
        {
            _OSDDLGRect.bColor = OSDDG_PALETTE_ENTRY_DLG_DISABLE_BUTTON_BACKGROUND;
            _OSDDLGStringAttr.bTextColor = OSDDG_PALETTE_ENTRY_DLG_DISABLE_BUTTON_STRING;
            _OSDDLGStringAttr.bBackground = OSDDG_PALETTE_ENTRY_DLG_DISABLE_BUTTON_BACKGROUND;
        }
    }

    _OSDDLGRect.rect.wLeft = _OSDDLGAttribute.wStartH+OSDDG_DLG_BACKGROUND_BUTTON_DISTANCE_H;
    _OSDDLGRect.rect.wTop = _OSDDLGAttribute.wStartV+OSDDG_DLG_BACKGROUND_BUTTON_DISTANCE_V+(bIndex*(OSDDG_DLG_BUTTON_HEIGHT+OSDDG_DLG_BUTTON_BUTTON_DISTANCE_V));
    _OSDDLGRect.rect.wRight = _OSDDLGRect.rect.wLeft+_wOSDDLGButtonWidth;
    _OSDDLGRect.rect.wBottom = _OSDDLGRect.rect.wTop+OSDDG_DLG_BUTTON_HEIGHT;

    GDI_FillRoundRect(0, OSDDG_DLG_ARC_VALUE, &_OSDDLGRect);

    //Fill the frame of round rectangle
    _OSDDLGRect.bColor = OSDDG_PALETTE_ENTRY_BUTTON_FRAME;

    _OSDDLGRect.rect.wLeft -= OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL;
    _OSDDLGRect.rect.wTop -= OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL;
    _OSDDLGRect.rect.wRight += OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL;
    _OSDDLGRect.rect.wBottom += OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, OSDDG_BUTTON_OUTER_ARC_VALUE, OSDDG_BUTTON_ARC_VALUE, &_OSDDLGRect);

    //Fill string
    _OSDDG_DLG_FillButtonString(bIndex);

    _OSDDLGStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDDLGStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDDLGStringAttr.bColorKey = 0;

    _OSDDLGStringAttr.wX = _OSDDLGRect.rect.wLeft + OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL+((_wOSDDLGButtonWidth - GDI_GetStringWidth_909(__pOSDUTLOutputString)) >> 1);
    _OSDDLGStringAttr.wY = _OSDDLGRect.rect.wTop + OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL+((OSDDG_DLG_BUTTON_HEIGHT - GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);
        
    GDI_DrawString_909(0, &_OSDDLGStringAttr, __pOSDUTLOutputString);
}

//***************************************************************************
//  Function    :   _OSDDG_Initial
//  Abstract    :   This function will be called when entering the Digest's main 
//              :   menu at the first time.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_Initial(void)
{
    //Must initialize GDI because it may be covered by other UI like SETUP.
    _OSDDG_InitializeGDI();

    //Set the attributes for normal upper message and normal bottom message.
    _OSDDG_InitializeNDAttribute();

    //Draw the title
    _OSDDG_DisplayTitle();

    //Draw the page info.
    _OSDDG_DisplayPageInfo();

    //Draw the info. region
    _OSDDG_DrawInfoRegion();

    //Draw the buttons.
    for (_bOSDDGTemp = 0; _bOSDDGTemp < OSDDG_BUTTON_NUM; _bOSDDGTemp++)
    {
        _OSDDG_DrawButton(_bOSDDGTemp, FALSE);
    }

    //Set the frame's tuning position.
    _OSDDG_TuneFramePosition();
}

//***************************************************************************
//  Function    :   _OSDDG_InitializeStringTransparentAttr
//  Abstract    :   This function will set the common attributes of transparent
//              :   part for string output.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_InitializeStringTransparentAttr(void)
{
    _OSDDGStringAttr.bBackground = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDDGStringAttr.bColorKey = 0;
    _OSDDGStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDDGStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
}


//***************************************************************************
//  Function    :   _OSDDG_DisplayTitle
//  Abstract    :   This function will display the title.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_DisplayTitle(void)
{
    switch (__DigestUIAttr.bDigestMode)
    {
    case DIGEST_MODE_TRACK: //Track Digest
        __pOSDUTLOutputString = aDGTitleTrackDigest;
        break;
    case DIGEST_MODE_DISCINTERVAL: //Disc Interval
        __pOSDUTLOutputString = aDGTitleDiscInterval;
        break;
    default:
        return;
    }

    _OSDDG_InitializeStringTransparentAttr();
    _OSDDGStringAttr.bTextColor = OSDDG_PALETTE_ENTRY_TITLE_STRING;
    _OSDDGStringAttr.wX = OSDDG_TITLE_START_H;
    _OSDDGStringAttr.wY = OSDDG_TITLE_START_V+_OSDDG_TuneForTVType(OSDDG_TUNE_POS_TYPE_TITLE);
    GDI_DrawString_909(0, &_OSDDGStringAttr, __pOSDUTLOutputString);
}

//***************************************************************************
//  Function    :   _OSDDG_DisplayPageInfo
//  Abstract    :   This function will show the page info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_DisplayPageInfo(void)
{
    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aDGPage);    
    OSDUTL_OutputNumFromIndex(__DigestUIAttr.bCurPageNo+1);
    OSDUTL_CopyStringFromIndex(aDGSlash);
    OSDUTL_OutputNumFromIndex(__DigestUIAttr.bTotalPageNo);

    _OSDDG_InitializeStringTransparentAttr();
    _OSDDGStringAttr.bTextColor = OSDDG_PALETTE_ENTRY_TITLE_STRING;
    _OSDDGStringAttr.wX = OSDDG_PAGE_INFO_END_H - GDI_GetStringWidth_909(__pOSDUTLOutputString);
    _OSDDGStringAttr.wY = OSDDG_PAGE_INFO_START_V+_OSDDG_TuneForTVType(OSDDG_TUNE_POS_TYPE_TITLE);
    GDI_DrawString_909(0, &_OSDDGStringAttr, __pOSDUTLOutputString);
}

//***************************************************************************
//  Function    :   _OSDDG_DrawInfoRegion
//  Abstract    :   This function will draw the info. region.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_DrawInfoRegion(void)
{
    _OSDDGRect.rect.wLeft = OSDDG_INFO_REGION_START_H;
    _OSDDGRect.rect.wTop = OSDDG_INFO_REGION_START_V+_OSDDG_TuneForTVType(OSDDG_TUNE_POS_TYPE_HELP);
    _OSDDGRect.rect.wRight = OSDDG_INFO_REGION_START_H+OSDDG_INFO_REGION_WIDTH;
    _OSDDGRect.rect.wBottom = _OSDDGRect.rect.wTop+OSDDG_INFO_REGION_HEIGHT;
    _OSDDGRect.bColor = OSDDG_PALETTE_ENTRY_INFO_REGION_BACKGROUND;
    
    GDI_FillRoundRect(0, OSDDG_INFO_REGION_ARC_VALUE, &_OSDDGRect);

    //Fill the frame of round rectangle
    _OSDDGRect.bColor = OSDDG_PALETTE_ENTRY_INFO_REGION_FRAME;

    _OSDDGRect.rect.wLeft -= OSDDG_INFO_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDDGRect.rect.wTop -= OSDDG_INFO_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDDGRect.rect.wRight += OSDDG_INFO_REGION_OUTER_INNER_ARC_PIXEL;
    _OSDDGRect.rect.wBottom += OSDDG_INFO_REGION_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, OSDDG_INFO_REGION_OUTER_ARC_VALUE, OSDDG_INFO_REGION_ARC_VALUE, &_OSDDGRect);
}

//***************************************************************************
//  Function    :   _OSDDG_DrawButton
//  Abstract    :   This function will draw the button.
//  Arguments   :   bIndex: index of the button.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_DrawButton(BYTE bIndex, BYTE bHighlight)
{
    //Draw the rectangle to represent the button. For PC-908 test only.
    _OSDDGRect.rect.wLeft = OSDDG_BUTTON_START_H+bIndex*OSDDG_BUTTON_BUTTON_DISTANCE+bIndex*OSDDG_BUTTON_WIDTH;
    _OSDDGRect.rect.wTop = OSDDG_BUTTON_START_V+_OSDDG_TuneForTVType(OSDDG_TUNE_POS_TYPE_BUTTON);      
    _OSDDGRect.rect.wRight = _OSDDGRect.rect.wLeft+OSDDG_BUTTON_WIDTH;
    _OSDDGRect.rect.wBottom = _OSDDGRect.rect.wTop+OSDDG_BUTTON_HEIGHT;
    
    if (bHighlight)
    {
        _OSDDGRect.bColor = OSDDG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BACKGROUND;
        _OSDDGStringAttr.bTextColor = OSDDG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_STRING;
        _OSDDGStringAttr.bBackground = OSDDG_PALETTE_ENTRY_HIGHLIGHT_BUTTON_BACKGROUND;
    }
    else
    {
        _OSDDGRect.bColor = OSDDG_PALETTE_ENTRY_ENABLE_BUTTON_BACKGROUND;
        _OSDDGStringAttr.bTextColor = OSDDG_PALETTE_ENTRY_ENABLE_BUTTON_STRING;
        _OSDDGStringAttr.bBackground = OSDDG_PALETTE_ENTRY_ENABLE_BUTTON_BACKGROUND;
    }
    
    GDI_FillRoundRect(0, OSDDG_BUTTON_ARC_VALUE, &_OSDDGRect);

    //Fill the frame of round rectangle
    _OSDDGRect.bColor = OSDDG_PALETTE_ENTRY_BUTTON_FRAME;

    _OSDDGRect.rect.wLeft -= OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL;
    _OSDDGRect.rect.wTop -= OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL;
    _OSDDGRect.rect.wRight += OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL;
    _OSDDGRect.rect.wBottom += OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, OSDDG_BUTTON_OUTER_ARC_VALUE, OSDDG_BUTTON_ARC_VALUE, &_OSDDGRect);

    _OSDDGStringAttr.bColorKey = 0;
    _OSDDGStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDDGStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;

    switch (bIndex)
    {
    case 0: //"Menu" button
        __pOSDUTLOutputString = aDGBtnMenu;
        break;
    case 1: //"Exit" button
        __pOSDUTLOutputString = aDGBtnExit;
        break;
    default:
        return;
    }

    _OSDDGStringAttr.wX = _OSDDGRect.rect.wLeft+OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL+((OSDDG_BUTTON_WIDTH-GDI_GetStringWidth_909(__pOSDUTLOutputString)) >> 1);
    _OSDDGStringAttr.wY = _OSDDGRect.rect.wTop+OSDDG_BUTTON_OUTER_INNER_ARC_PIXEL+((OSDDG_BUTTON_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);

    GDI_DrawString_909(0, &_OSDDGStringAttr, __pOSDUTLOutputString);
}

//***************************************************************************
//  Function    :   _OSDDG_DrawFrame
//  Abstract    :   This function will draw the frame.
//  Arguments   :   bIndex: index of the frame.
//              :   bUpdate: OSDDG_FRAME_NORMAL/OSDDG_FRAME_SELECT.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_DrawFrame(BYTE bIndex, BYTE bUpdate)
{
/* LJY0.76 no need to draw frame for now but may be needed for CT909s
    _OSDDGFrame.bStyle = FRAME_PUSH;
    _OSDDGFrame.bThickness = OSDDG_FRAME_THICKNESS;
    _OSDDGFrame.bColorCenter = PAL_ENTRY_COLOR_TRANSPARENT;

    _OSDDGFrame.rect.wLeft = DIGEST_FIRST_FRAME_POS_H+(bIndex%OSDDG_COLUMN_NUM)*DIGEST_FRAME_SIZE_H+(bIndex%OSDDG_COLUMN_NUM)*DIGEST_FRAME_SPACING_H;
    _OSDDGFrame.rect.wTop = DIGEST_FIRST_FRAME_POS_V+(bIndex/OSDDG_COLUMN_NUM)*DIGEST_FRAME_SIZE_V+(bIndex/OSDDG_COLUMN_NUM)*DIGEST_FRAME_SPACING_V;

    //Note: Need to tune the position for NTSC/PAL, Interlace/P-SCAN
    _OSDDGFrame.rect.wLeft -= _bDGTuneFrame_H;
    _OSDDGFrame.rect.wTop -= _bDGTuneFrame_V;
    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC)
    {
        _OSDDGFrame.rect.wRight = _OSDDGFrame.rect.wLeft+DIGEST_FRAME_SIZE_H+OSDDG_TUNE_WIDTH_NTSC;
        _OSDDGFrame.rect.wBottom = _OSDDGFrame.rect.wTop+DIGEST_FRAME_SIZE_V+OSDDG_TUNE_HEIGHT_NTSC;
    } 
    else //PAL
    {
        _OSDDGFrame.rect.wTop += (OSDDG_TUNE_DISTANCE_HEIGHT_PAL) * (bIndex / OSDDG_COLUMN_NUM);
        _OSDDGFrame.rect.wRight = _OSDDGFrame.rect.wLeft+DIGEST_FRAME_SIZE_H+OSDDG_TUNE_WIDTH_PAL;
        _OSDDGFrame.rect.wBottom = _OSDDGFrame.rect.wTop+DIGEST_FRAME_SIZE_V+OSDDG_TUNE_DISTANCE_HEIGHT_PAL;
    }
	
    if (bUpdate == OSDDG_FRAME_NORMAL)
    {
        _OSDDGFrame.bColorBright = OSDDG_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
        _OSDDGFrame.bColorDark = OSDDG_PALETTE_ENTRY_NORMAL_FRAME_DARK;
    }
    else if (bUpdate == OSDDG_FRAME_SELECT)
    {
        _OSDDGFrame.bColorBright = OSDDG_PALETTE_ENTRY_SELECT_FRAME_BRIGHT;
        _OSDDGFrame.bColorDark = OSDDG_PALETTE_ENTRY_SELECT_FRAME_DARK;
    }

    GDI_DrawFrame_909(0, &_OSDDGFrame);
*/    
}

//***************************************************************************
//  Function    :   _OSDDG_ClearFrameRegion
//  Abstract    :   This function will clear all frames.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_ClearFrameRegion(void)
{
    _OSDDGRect.rect.wLeft = 0;
    _OSDDGRect.rect.wTop = OSDDG_TITLE_START_V+OSDDG_FONT_HEIGHT;
    _OSDDGRect.rect.wRight = 600; //Should re-define this value as the max. width of the OSD region.
    _OSDDGRect.rect.wBottom = OSDDG_INFO_REGION_START_V-1;

    _OSDDGRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    
    GDI_FillRect_909(0, &_OSDDGRect);
}

//***************************************************************************
//  Function    :   _OSDDG_Exit
//  Abstract    :   Exit the Digest UI.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_Exit(void)
{
    //Clear the Digest region
    GDI_ClearRegion(0); //clear region 0

    //Tell OSD that Digest UI exits.
    OSD_ChangeUI(OSD_UI_DIGEST, OSD_EXIT_UI);
}

//***************************************************************************
//  Function    :   _OSDDG_ClearPageInfoRegion
//  Abstract    :   This function will clear the page info. region.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_ClearPageInfoRegion(void)
{
    _OSDDGRect.rect.wLeft = OSDDG_TITLE_START_H+OSDDG_TITLE_WIDTH;
    _OSDDGRect.rect.wTop = OSDDG_TITLE_START_V+_OSDDG_TuneForTVType(OSDDG_TUNE_POS_TYPE_TITLE);
    _OSDDGRect.rect.wRight = OSDDG_PAGE_INFO_END_H;
    _OSDDGRect.rect.wBottom = _OSDDGRect.rect.wTop + OSDDG_FONT_HEIGHT;

    _OSDDGRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    
    GDI_FillRect_909(0, &_OSDDGRect);
}

//***************************************************************************
//  Function    :   _OSDDG_UpdateInfoRegion
//  Abstract    :   This function will update the info. in the info. region.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_UpdateInfoRegion(void)
{    
    _OSDDG_DrawInfoRegion();

    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aDGInfoTrack);    
    OSDUTL_OutputNumFromIndex(__DigestUIAttr.bCurTrackNo+1);
    OSDUTL_CopyStringFromIndex(aDGSlash);
    OSDUTL_OutputNumFromIndex(gcShowStatus.wTotalTracks);

    _OSDDGStringAttr.bBackground = OSDDG_PALETTE_ENTRY_INFO_REGION_BACKGROUND;
    _OSDDGStringAttr.bTextColor = OSDDG_PALETTE_ENTRY_HELP_STRING;
    _OSDDGStringAttr.wX = OSDDG_INFO_REGION_STRING_START_H;
    _OSDDGStringAttr.wY = OSDDG_INFO_REGION_START_V+((OSDDG_INFO_REGION_HEIGHT-GDI_GetStringHeight_909(__pOSDUTLOutputString))>>1)+_OSDDG_TuneForTVType(OSDDG_TUNE_POS_TYPE_HELP);
    GDI_DrawString_909(0, &_OSDDGStringAttr, __pOSDUTLOutputString);
}

//***************************************************************************
//  Function    :   _OSDDG_RecoverAll
//  Abstract    :   This function will recover all Digest UI.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_RecoverAll(void)
{
    if (__DigestUIAttr.bDigestMode == DIGEST_MODE_DIALOG)
    {
        _OSDDG_DLG_Initial(TRUE);        
    }
    else //Digest main menu
    {
        _OSDDG_Initial();

        //Draw the frames and update the frame info.
        _OSDDG_DrawAllFrames();
    }
}

//***************************************************************************
//  Function    :   _OSDDG_UpdteFrameInfo
//  Abstract    :   This function will update the info. in the frame.
//  Arguments   :   bIndex: the index of the frame.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_UpdteFrameInfo(BYTE bIndex)
{
    //Show the number of the frame.
    /*
    OSDUTL_PrepareOutputString();
    OSDUTL_OutputNumFromIndex((WORD)(bIndex+1));

    _OSDDG_InitializeStringTransparentAttr();
    _OSDDGStringAttr.bBackground = OSDDG_PALETTE_ENTRY_FRAME_INFO_BACKGROUND;
    _OSDDGStringAttr.bTextColor = OSDDG_PALETTE_ENTRY_FRAME_INFO_STRING;

    _OSDDGStringAttr.wX = DIGEST_FIRST_FRAME_POS_H+bIndex*DIGEST_FRAME_SIZE_H+bIndex*DIGEST_FRAME_SPACING_H+OSDDG_FRAME_THICKNESS+OSDDG_FRAME_INFO_DISTANCE_H;
    _OSDDGStringAttr.wY = DIGEST_FIRST_FRAME_POS_V+bIndex*DIGEST_FRAME_SIZE_V+bIndex*DIGEST_FRAME_SPACING_V+OSDDG_FRAME_THICKNESS+OSDDG_FRAME_INFO_DISTANCE_V;
    GDI_DrawString_909(0, &_OSDDGStringAttr, __pOSDUTLOutputString);

    //Keep the string's width of the index.
    _bOSDDGTemp = (BYTE)GDI_GetStringWidth_909(__pOSDUTLOutputString);

    //Show time when "Disc Interval" mode.
    if (__DigestUIAttr.bDigestMode == DIGEST_MODE_DISCINTERVAL)
    {
        OSDUTL_PrepareOutputString();

        //Total seconds.
        _bOSDDGTemp1 = bIndex*OSDDG_TIME_INTERVAL;

        //show "hour"
        _OSDUTL_ShowTimeNum((BYTE)(_bOSDDGTemp1/60));
        
        //show ":"
        __wOSDUTLStringBuf[__bOSDUTLIndex++] = aDGMouhow[1];
        
        //show "Minute"
        _OSDUTL_ShowTimeNum(_bOSDDGTemp1);
        
        //show ":"
        __wOSDUTLStringBuf[__bOSDUTLIndex++] = aDGMouhow[1];
        
        //show "second"
        _OSDUTL_ShowTimeNum(0);        
    }

    _OSDDGStringAttr.wX += _bOSDDGTemp+OSDDG_FRAME_INFO_NUM_TIME_DISTANCE;
    GDI_DrawString_909(0, &_OSDDGStringAttr, __pOSDUTLOutputString);
    */
}

//***************************************************************************
//  Function    :   _OSDDG_UpdateHighlight
//  Abstract    :   This function will update button's highlight.
//  Arguments   :   bUpdate: OSDDG_DRAW/OSDDG_CLEAR.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_UpdateHighlight(BYTE bUpdate)
{
    if (__DigestUIAttr.bDigestMode == DIGEST_MODE_DIALOG)
    {
        if (bUpdate == OSDDG_DRAW) //Draw highlight button
        {
            _OSDDG_DLG_DrawButton(__DigestUIAttr.bButtonIndex, TRUE);
        }
        else
        {            
            _OSDDG_DLG_DrawButton(__DigestUIAttr.bButtonIndex, FALSE);
        }
    }
    else //Digest main menu.
    {
        if (bUpdate == OSDDG_DRAW) //Draw highlight button
        {
            _OSDDG_DrawButton(__DigestUIAttr.bButtonIndex, TRUE);
        }
        else
        {
            _OSDDG_DrawButton(__DigestUIAttr.bButtonIndex, FALSE);
        }
    }
}

//***************************************************************************
//  Function    :   _OSDDG_ChangeTVMode
//  Abstract    :   This function will be called when TV mode is changed.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_ChangeTVMode(void)
{
    //Clear the frame region
    _OSDDG_ClearFrameRegion();
    
    //Re-draw all frames.
    _OSDDG_DrawAllFrames();
}

//***************************************************************************
//  Function    :   _OSDDG_DrawAllFrames
//  Abstract    :   This function will draw all frames.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDG_DrawAllFrames(void)
{
    //Draw the frames and update the frame info.
    for (_bOSDDGTemp = 0; _bOSDDGTemp < __DigestUIAttr.bDisplayedFrameNo; _bOSDDGTemp++)
    {
        if (__DigestUIAttr.bCurFrameNo == _bOSDDGTemp)
            _OSDDG_DrawFrame(_bOSDDGTemp, OSDDG_FRAME_SELECT);
        else
            _OSDDG_DrawFrame(_bOSDDGTemp, OSDDG_FRAME_NORMAL);
        
        _OSDDG_UpdteFrameInfo(_bOSDDGTemp);
    }
}

//*********************************************************************************************************
//  Function    :   _OSDDG_TuneFramePosition
//  Abstract    :   Tune the Digest's frame when according to different TV type.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDDG_TuneFramePosition(void)
{
    if (__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC)
    {
        if(__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
        {
            _bDGTuneFrame_H = OSDDG_TUNE_FRAME_H_NTSC_PSCAN;
            _bDGTuneFrame_V = OSDDG_TUNE_FRAME_V_NTSC_PSCAN;
        }
        else
        {
            _bDGTuneFrame_H = OSDDG_TUNE_FRAME_H_NTSC;
            _bDGTuneFrame_V = OSDDG_TUNE_FRAME_V_NTSC;

        }
    }
    else // PAL
    {
        if(__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
        {
            _bDGTuneFrame_H= OSDDG_TUNE_FRAME_H_PAL_PSCAN;
            _bDGTuneFrame_V= OSDDG_TUNE_FRAME_V_PAL_PSCAN;
        }
        else
        {
            _bDGTuneFrame_H= OSDDG_TUNE_FRAME_H_PAL;
            _bDGTuneFrame_V= OSDDG_TUNE_FRAME_V_PAL;
        }
    }
}

//*********************************************************************************************************
//  Function    :   _OSDDG_TuneForTVType
//  Abstract    :   Call this function to tune the value for different TV type
//  Arguments   :   bType: the type of tunning the value.
//  Return		:   the value should be tune
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
BYTE _OSDDG_TuneForTVType(BYTE bType)
{
    if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
        return 0;
    
    switch (bType)
    {
    case OSDDG_TUNE_POS_TYPE_TITLE:
        return OSDDG_TUNE_TITLE_START_V_PAL;
    case OSDDG_TUNE_POS_TYPE_BUTTON:
        return OSDDG_TUNE_BUTTON_START_V_PAL;
    case OSDDG_TUNE_POS_TYPE_HELP:
        return OSDDG_TUNE_HELP_START_V_PAL;
    default:
        return 0;
    }
}

//*********************************************************************************************************
//  Function    :   _OSDDG_InitializeNDAttribute
//  Abstract    :   This function will initialize the normal upper/bottom string attribute.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void _OSDDG_InitializeNDAttribute(void)
{
    //Set the attributes for normal upper message and normal bottom message.
    __OSDNDUpMsgAttr.wX = OSDDG_PAGE_INFO_END_H;
    __OSDNDUpMsgAttr.wY = OSDDG_TITLE_START_V;
    __OSDNDUpMsgAttr.bBackground = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDUpMsgAttr.bTextColor = OSDDG_PALETTE_ENTRY_TITLE_STRING;
    __OSDNDUpMsgAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDUpMsgAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDUpMsgAttr.bColorKey = 0;
    __OSDNDBottomMsgAttr.wX = OSDDG_INFO_REGION_STRING_START_H;
    __OSDNDBottomMsgAttr.wY = OSDDG_INFO_REGION_START_V;
    __OSDNDBottomMsgAttr.bBackground = OSDDG_PALETTE_ENTRY_INFO_REGION_BACKGROUND;
    __OSDNDBottomMsgAttr.bTextColor = OSDDG_PALETTE_ENTRY_HELP_STRING;
    __OSDNDBottomMsgAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDBottomMsgAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    __OSDNDBottomMsgAttr.bColorKey = 0;
}

#endif //NO_DIGEST
