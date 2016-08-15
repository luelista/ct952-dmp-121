
#include "winav.h"
#include "gdi.h"

#ifndef IMAGE_FRAME_SETUP //CoCo2.38
#ifndef SUPPORT_STB
#define MENU_DECLARATION
#include "utl.h"
#include "comutl.h"
#include "hal.h"
#include "osd.h"
#include  "cc.h"
#include "input.h"
#include "fontindex.h" //++CoCo2.37P
#include "setup.h"
#include "osdnd.h"
#include "osd3.h"
#include "osddlg.h"
#include "osddivx.h"
#include "setup.h"
#include "notransl.h"


// Brian1.05, define this to skip disabled item when moving cursor
#define SKIP_DISABLED_ITEM 


// Brian1.08a, support horizontal Icon menu
#define OSDMENU_GOTO_PREV_ITEM      0
#define OSDMENU_GOTO_NEXT_ITEM      1
#define OSDMENU_GOTO_SUB_MENU       2
#define OSDMENU_GOTO_PARENT_MENU    3
#define OSDMENU_SELECT_ITEM         4


#define MENU_ITEM_HEIGHT        28 // each Item line height
#define MENU_ITEM_X_OFFSET      30 // the x offset reserves space for "check mark"
#define MENU_ITEM_SUBMENU_OFFSET 18 // the x offset reserves space for "sub menu"
#define MENU_CHECK_OFFSET       5
#define MENU_SCROLL_HEIGHT      22 // the scroll-up/down mark height is 16, plus 2 border thinkness

#define MENU_TOP_ICON_HEIGHT    26 // The height of the top part of icon is 26 pixel.
#ifndef REMOVE_SETUP_ICON
#define MENU_ICON_HEIGHT        (44+4)  // the height of the icon in the list, including the border 2*2
#else
#define MENU_ICON_HEIGHT	44
#endif

// Brian1.08a
#define MENU_ICON_WIDTH         64  // the width of the icon in the list
#define MENU_ICON_X_OFFSET      56       // use for horizontal icon menu
#define MENU_ICON_Y_OFFSET      6       // use for vertical icon menu

#define MENU_DESCRIPTION_X      (40 +MENU_GLOBAL_OFFSET_X)
#define MENU_DESCRIPTION_Y      (MENU_GLOBAL_OFFSET_Y + 5)


// Brian1.08
// for Input dialog display in Setup
// Adjust the "Input Password" / "Input Language Code" dialog position, so it won't overlap with Setup menu
#define DIALOG_INPUT_PASSWORD_X1        SETUP_MENU_ITEM_X
#define DIALOG_INPUT_PASSWORD_X2        (DIALOG_INPUT_PASSWORD_X1+ SETUP_MENU_ITEM_W)
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
#define DIALOG_INPUT_PASSWORD_Y1        290
#define DIALOG_INPUT_PASSWORD_Y2        360
#else
#define DIALOG_INPUT_PASSWORD_Y1        250
#define DIALOG_INPUT_PASSWORD_Y2        320
#endif

#define DIALOG_INPUT_PASSWORD_X_OFFSET	20
#define DIALOG_INPUT_PASSWORD_Y_OFFSET	20


// for Input dialog display in noraml OSD
#define DIALOG_INPUT_PASSWORD_OSD_X1    (16+MENU_GLOBAL_OFFSET_X) 
#define DIALOG_INPUT_PASSWORD_OSD_Y1	20 //(OSDND_UPPER_BACKGROUND_RIGHT_DISPLAY_DISTANCE_V+OSDND_FONT_HEIGHT) //20 Howard1.24
#define DIALOG_INPUT_PASSWORD_OSD_X2	(DIALOG_INPUT_PASSWORD_OSD_X1+520)
#define DIALOG_INPUT_PASSWORD_OSD_Y2	(DIALOG_INPUT_PASSWORD_OSD_Y1+32)


#define DIALOG_INPUT_PASSWORD_OSD_X_OFFSET	100
#define DIALOG_INPUT_PASSWORD_OSD_Y_OFFSET	0


// Adjust the "change password" dialog position, so it won't overlap with Setup menu
#define DIALOG_CHANGE_PASSWORD_X1       SETUP_MENU_ITEM_X	
#define DIALOG_CHANGE_PASSWORD_X2       (DIALOG_CHANGE_PASSWORD_X1 + 370)
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
#define DIALOG_CHANGE_PASSWORD_Y1       (SETUP_MENU_ITEM_Y + 143)
#else
#define DIALOG_CHANGE_PASSWORD_Y1       (SETUP_MENU_ITEM_Y + 164)
#endif
#define DIALOG_CHANGE_PASSWORD_Y2       (DIALOG_CHANGE_PASSWORD_Y1 + 98) 

#define DIALOG_CHANGE_PASSWORD_X_OFFSET	20 
#define DIALOG_CHANGE_PASSWORD_Y_OFFSET	10 

#define DIALOG_CHANGE_PASSWORD_X_SHIFT	120 

#define DIALOG_CHANGE_PASSWORD_CHAR_WIDTH	20
#define DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT	OSD_FONT_HEIGHT


#define GET_MENU_X(MENU)                   (WORD)((DWORD)(MENU[1].pbItemText))
#define GET_MENU_Y(MENU)                   (WORD)((DWORD)(MENU[1].pSubMenu))
#define GET_MENU_WIDTH(MENU)               (WORD)(MENU[1].Id)
#define GET_MENU_ROW(MENU)                 (BYTE)(MENU[1].bAttribute) // could display n Row
#define GET_MENU_ITEM_NUM(MENU)            (BYTE)(MENU[0].bAttribute) // number of items in a menu

#define GET_MENU_ITEM_TEXT(MENU, NUM)   (MENU[2+NUM].pbItemText)
#define GET_ITEM_ATTRIBUTE(MENU, NUM)   (MENU[2+NUM].bAttribute & 0x03)
#define CHECK_ICON_ATTRIBUTE(MENU, NUM)   (MENU[2+NUM].bAttribute & 0x10) // check if it is an ICON
#define GET_ITEM_ID(MENU, NUM)          (MENU[2+NUM].Id)
#define GET_MENU_ENABLE(MENU)           ((BYTE*)(MENU[0].pbItemText)) // the _bEnablexxx
#define GET_MENU_CHECK(MENU)            ((BYTE*)(MENU[0].pbItemText)) //  &(__SetupInfo.xxx)
#define GET_MENU_BASE(MENU)             (MENU[0].Id)
#define GET_MENU_ID(MENU)               ((BYTE) ((DWORD)(MENU[0].pSubMenu)))
#define GET_SUB_MENU(MENU, NUM)         (MENU[2+NUM].pSubMenu)                  

extern  BYTE    DATA    _bOSDLang;

BYTE __bMENUParent[MENU_LEVEL]; // record the parent menu of each level
BYTE __bMENUSelect[MENU_LEVEL]; // record the current selection of each level
BYTE __bMENUDisplayStart[MENU_LEVEL]; // record which item is displayed first in the display area
BYTE __bMENUStatus[MENU_LEVEL]; //bit 0: scroll-up mark appears, bit 1: scroll-down mark appears


BYTE    __bCurrentMenuId; // replace _pCurrentMenu
WORD    __wMENUCurrentItemId, __wMENUPrevItemId;
BYTE    __bMENUCurrentLevel;

#ifdef SUPPORT_MENU_CURSOR_STAY_AT_VALUE_AFTER_ENTER
WORD    __wMENUItemIdAfterEnter; // Brian1.08a, the Item ID after key Enter
#endif

PMENU_STRUCT *_pMenuIndex; // Brian1.20

// Brian1.20, in fact, it's a pointer to code menu structure
// must declare it as code pointer, otherwise there will be conversion problem with keil C 6.12 and 7.02
MENU_STRUCT  *_pTempMenu;

MENU_STRUCT  *_pTempMenu2, *_pTempMenu3; // Brian1.08a, Brian1.20

WORD    _wMENUTemp;
BYTE    _bMENUTemp, _bMENUTemp2;
// this variable is used to keep the New Password for verification
// could be used for other purpose as long as it is not in the "Change Password" process.
WORD	_wPasswordTemp; 


BYTE bStatus; // used in InputPassword, ChangePassword, InputLanguageCode function

//***************************************************************************
//Note: Don't translate the following strings.
//CoCo1.20, reduce code size.
// alan test WORD aSETUPPassword[] = {4, CHAR_X, CHAR_X, CHAR_X, CHAR_X};


//Alex1.07a for Input dialog display in PSCAN MODE
#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
#define DIALOG_INPUT_PSCAN_X1		(104+MENU_GLOBAL_OFFSET_X)
#define DIALOG_INPUT_PSCAN_Y1		120
#define DIALOG_INPUT_PSCAN_X2		(504+MENU_GLOBAL_OFFSET_X) 
#define DIALOG_INPUT_PSCAN_Y2		340
#define DIALOG_INPUT_PSCAN_X_OFFSET	 30
#define DIALOG_INPUT_PSCAN_Y_OFFSET	 30
BYTE __bOSDPromptMode;
BYTE    OSDPROMPT_ProcessKey(BYTE bKey);
#define OSD_PSCAN_PROMPT_WAIT_TIME 10 //5

BYTE    __bOSDPromptWaitTime; // = OSD_PSCAN_PROMPT_WAIT_TIME; To count How many time left
BYTE    __bOSDPromptPreKey;
DWORD    __dwOSDPromptInitialTime; // = OSD_PSCAN_PROMPT_Intial_TIME; To count How many time left

//Alan
WORD             _wMENUString[9];
PARM_FRAME       _MENU_FrameAttr;
PARM_DRAW_STRING _MENU_StrAttr;
PARM_RECT        _MENU_RectAttr;
PARM_BUTTON      _MENU_BtnAttr;


extern BYTE    __bOSDPromptMode; // = FALSE; put in _INITIAL_Variables_PowerON
void    OSDPROMPT_Trigger(void);
void    OSDSETUP_ScreenVideoOutputPscanDlg(void);
BYTE    OSDSETUP_InputPscan(BYTE bKey);
extern BYTE    OSDPROMPT_ProcessKey(BYTE bKey);
extern  BYTE CC_DoKeyAction(BYTE bKey);
#endif //#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)



void OSDMENU_DrawItem(BYTE bBase, BYTE bItem, BYTE bColor);
void OSDMENU_DisplaySubMenu(BYTE bItem, BYTE bLevel);
void OSDMENU_DisplayAVSysMenu(BYTE bLevel); // sepcial for AV system menu
void _DisplayIconMenu(void);
void _DisplayMenu(BYTE bItem, BYTE bLevel);
void OSDMENU_ClearMenu(void);
void OSDSETUP_ShowDescriptionText(void);
BYTE _GetMenuItemPosition(BYTE bItemId); // Brian1.08
void _DisplayCurrentSettings(void); // Brian1.08a
void _ClearCurrentSettings(void); // Brian1.08a
void _InitRect(PURECT pRect, WORD wLeft, WORD wTop, WORD wRight, WORD wBottom);
BYTE _PScan_Callback(void);
void _OSDMENU_DrawPassword(BYTE bPasswordType); //CoCo1.20, reduce code size.
void _OSDMENU_DrawFrame(BYTE bStyle); //CoCo1.20, reduce code size.
void _OSDMENU_SetMenuStringColor(void); //CoCo1.20, reduce code size.
void _OSDMENU_SetDlgStringColor(void); //CoCo1.20, reduce code size.
void _OSDMENU_FillRect(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom); //CoCo1.20, reduce code size.
void _OSDMENU_DrawString(void); //CoCo1.20, reduce code size.
void _OSDMENU_DrawDynamicString(WORD *pwString); //CoCo1.20, reduce code size.

void OSDMENU_Initial(void)
{
    // don't forget to initialize the Enable array accroding to the number of item
    _bEnableLanguage[0] = 0xFF; //0xFE;
    _bEnableScreen[0] = 0xFF;
    _bEnableAudio[0] = 0xFF;
    _bEnableCustom[0] = 0xFF;
    _bEnableMain[0] = 0xFF;

    __bMENUCurrentLevel = 0;

    for (_bMENUTemp=0; _bMENUTemp<MENU_LEVEL; _bMENUTemp++)
    {
        __bMENUParent[_bMENUTemp] = 0;
        __bMENUSelect[_bMENUTemp] = 0;
        __bMENUDisplayStart[_bMENUTemp] = 0;
        __bMENUStatus[_bMENUTemp] = 0;
    }
    
    __bMENUCurrentLevel = 0; //1;
    __bMENUSelect[__bMENUCurrentLevel] = 0; //3;
    __bMENUDisplayStart[__bMENUCurrentLevel] = 0;

    _pMenuIndex = _SetupMenuIndex;

    // Brian1.22, Kevin1.11a modified
    if (__dwSupportFeature & SUPPORT_FEATURE_PROLOGIC)
       __bCurrentMenuId = SETUP_MENU_MAIN_WITH_PROLOGIC;
    else
       __bCurrentMenuId = SETUP_MENU_MAIN ;//SETUP_MENU_LANGUAGE;

    __wMENUCurrentItemId = SETUP_MAIN_LANGUAGE;
}

void OSDMENU_SetMenuPalEntry(void)
{
    // set entries used by Menu, Setup, Equalizer and AV System

    GDI_ChangePALEntry(OSDMENU_ENTRY_MENU_COLOR_BRIGHT, OSDMENU_VALUE_MENU_COLOR_BRIGHT, FALSE);
    GDI_ChangePALEntry(OSDMENU_ENTRY_MENU_COLOR_DARK, OSDMENU_VALUE_MENU_COLOR_DARK, FALSE);
    GDI_ChangePALEntry(OSDMENU_ENTRY_MENU_COLOR_CENTER, OSDMENU_VALUE_MENU_COLOR_CENTER, FALSE);

    GDI_ChangePALEntry(OSDMENU_ENTRY_TEXT_COLOR_NORMAL, OSDMENU_VALUE_TEXT_COLOR_NORMAL, FALSE);
    GDI_ChangePALEntry(OSDMENU_ENTRY_TEXT_COLOR_DISABLE, OSDMENU_VALUE_TEXT_COLOR_DISABLE, FALSE);
    GDI_ChangePALEntry(OSDMENU_ENTRY_DESC_TEXT_COLOR, OSDMENU_VALUE_DESC_TEXT_COLOR, FALSE);

    GDI_ChangePALEntry(OSDMENU_ENTRY_ITEM_COLOR_SELECT, OSDMENU_VALUE_ITEM_COLOR_SELECT, FALSE);
    GDI_ChangePALEntry(OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT, OSDMENU_VALUE_ITEM_COLOR_HIGHLIGHT, FALSE);

/*
    // also set Setup color
    GDI_ChangePALEntry(OSDSETUP_ENTRY_CENTER_COLOR_BG, OSDSETUP_VALUE_MAIN_COLOR_BG, TRUE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_HELP_COLOR_BG, OSDSETUP_VALUE_HELP_COLOR_BG, TRUE);
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_COLOR_BG, OSDSETUP_VALUE_ICON_COLOR_BG, TRUE);

    GDI_ChangePALEntry(OSDSETUP_ENTRY_SPEAKER_HIGHLIGHT, OSDSETUP_VALUE_SPEAKER_HIGHLIGHT, FALSE);

#ifdef REMOVE_SETUP_ICON
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_COLOR_ORANGE, OSDSETUP_VALUE_ICON_ORANGE, FALSE);	
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_COLOR_BLUE, OSDSETUP_VALUE_ICON_BLUE, FALSE); //Brian1.24a //Iris0312	
    GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_COLOR_RED, OSDSETUP_VALUE_ICON_RED, FALSE); //Brian1.24a //Iris0318	
	GDI_ChangePALEntry(OSDSETUP_ENTRY_ICON_COLOR_GREEN, OSDSETUP_VALUE_ICON_GREEN, FALSE); //Brian1.24a //Iris0318
#endif
*/

    GDI_WaitPaletteComplete();

}

void OSDMENU_DisplayTheMenu(void)
{
    OSDMENU_DisplayMenu(__bCurrentMenuId, __bMENUSelect[__bMENUCurrentLevel], __bMENUCurrentLevel);
    if (__bMENUCurrentLevel == 0)
    {
        OSDSETUP_ShowDescriptionText();
        // Brian1.08a, display menu settings when menu cursor in on Icon menu 
#ifdef SUPPORT_MENU_SETTINGS_DISPLAY
        _DisplayCurrentSettings();
#endif
    }

    OSDMENU_DisplaySubMenu(__bMENUSelect[__bMENUCurrentLevel], __bMENUCurrentLevel);
}


void OSDMENU_RedrawAll(void)
{
    BYTE bBackupLevel;
    MENU_STRUCT  *_pBackupMenu;

    bBackupLevel = __bMENUCurrentLevel;
    _pBackupMenu = _pTempMenu;

    OSDSETUP_RedrawMenu();

    _pTempMenu = _pMenuIndex[0];
    __bMENUCurrentLevel = 0;

    _DisplayIconMenu();

    if (0 < bBackupLevel)
    {
        OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel],
                         __bMENUSelect[__bMENUCurrentLevel],
                         OSDMENU_ENTRY_ITEM_COLOR_SELECT);
        if (2 == bBackupLevel)
        {
            _pTempMenu = GET_SUB_MENU(_pTempMenu,
                                      __bMENUSelect[__bMENUCurrentLevel]);
            __bMENUCurrentLevel = 1;
            _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
            OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel],
                             __bMENUSelect[__bMENUCurrentLevel],
                             OSDMENU_ENTRY_ITEM_COLOR_SELECT);
        }
    }

    __bMENUCurrentLevel = bBackupLevel;
    _pTempMenu = _pBackupMenu;
}


void OSDMENU_MenuOperation(BYTE bKey)
{

    BYTE bItemNum, bOrgItem;
    BYTE bMenuOperation=0; // Brian1.08a

    // Brian1.08a, transfer Key to menu operation
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
    // Horizontal icon menu

    if (__bMENUCurrentLevel == 0) 
    {
        // at root (icon) menu
        switch (bKey)
        {
            case KEY_UP:
                return;
            case KEY_DOWN:
                bMenuOperation = OSDMENU_GOTO_SUB_MENU;
                break;
            case KEY_RIGHT:
                bMenuOperation = OSDMENU_GOTO_NEXT_ITEM;
                break;
            case KEY_LEFT:
                bMenuOperation = OSDMENU_GOTO_PREV_ITEM;
                break;
            case KEY_ENTER:
            case KEY_PLAY:
            case KEY_PLAY_PAUSE:
                bMenuOperation = OSDMENU_SELECT_ITEM;
                break;
        } 

    }
    else // it is not root menu
    {
        // none root (icon) menu
        switch (bKey)
        {
            case KEY_UP:
                // need to check when it's the topest item
                // it's more convenient to do the check when processing the operation, 
                // for the condition code is already there 
                bMenuOperation = OSDMENU_GOTO_PREV_ITEM;
                break;
            case KEY_DOWN:
                bMenuOperation = OSDMENU_GOTO_NEXT_ITEM;
                break;
            case KEY_RIGHT:
                bMenuOperation = OSDMENU_GOTO_SUB_MENU;
                break;
            case KEY_STOP: // allow use KEY_STOP to go to parent menu
            case KEY_LEFT:
                // need chech when it's the left most item
                // it's more convenient to do the check when processing the operation, 
                // for the condition code is already there 
                bMenuOperation = OSDMENU_GOTO_PARENT_MENU;
                break;
            case KEY_ENTER:
            case KEY_PLAY:
            case KEY_PLAY_PAUSE:
                bMenuOperation = OSDMENU_SELECT_ITEM;
                break;
        } 
    }

#else
    // Vertical icno menu
    // just 1-to-1 mapping
    switch (bKey)
    {
        case KEY_UP:
            bMenuOperation = OSDMENU_GOTO_PREV_ITEM;
            break;
        case KEY_DOWN:
            bMenuOperation = OSDMENU_GOTO_NEXT_ITEM;
            break;
        case KEY_RIGHT:
            bMenuOperation = OSDMENU_GOTO_SUB_MENU;
            break;
        case KEY_LEFT:
            bMenuOperation = OSDMENU_GOTO_PARENT_MENU;
            break;
        case KEY_ENTER:
        case KEY_PLAY:
        case KEY_PLAY_PAUSE:
            bMenuOperation = OSDMENU_SELECT_ITEM;
            break;
    } 

#endif // SUPPORT_SETUP_HORIZONTAL_ICON_MENU

process_key_op:

    _pTempMenu = _pMenuIndex[__bCurrentMenuId];
    bItemNum = GET_MENU_ITEM_NUM(_pTempMenu);

    // Brian1.08a
    switch (bMenuOperation)
    {
        // Brian1.08a
        case OSDMENU_GOTO_PREV_ITEM:
            // Brian1.08a, support Horizontal icon menu
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
            if (__bMENUSelect[__bMENUCurrentLevel] == 0)
            {
                if (__bMENUCurrentLevel == 1) // menu level 2 ==> value menu, don't go to paranet menu
                {
                    // it is the topest item, so need to move to icon menu
                    bMenuOperation = OSDMENU_GOTO_PARENT_MENU;
                    goto process_key_op;
                }
            }
            else
#else
            if (__bMENUSelect[__bMENUCurrentLevel] != 0)
#endif
            {
				
#ifdef SKIP_DISABLED_ITEM
                // Brian1.05, don't move cursor onto disabled item.
                bOrgItem = __bMENUSelect[__bMENUCurrentLevel]; // keep the current select

                while(__bMENUSelect[__bMENUCurrentLevel] != 0)
                {
                    __bMENUSelect[__bMENUCurrentLevel]--;

                    _bMENUTemp = __bMENUSelect[__bMENUCurrentLevel]; 

                    if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_ITEM ||
                       GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_DIALOG)
                    {
                        if (*(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8))) // if enabled
                        {
                            break; // find a enabled item
                        }
                        else
                        {
                            if (__bMENUSelect[__bMENUCurrentLevel] == 0)
                            {
                                // no enabled item is found, so the cursor remain still
                                __bMENUSelect[__bMENUCurrentLevel] = bOrgItem;
// Brian1.08a, support Horizontal icon menu
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
                                // since there is no Up items, so go to Icon menu
                                bMenuOperation = OSDMENU_GOTO_PARENT_MENU;
                                goto process_key_op;
                                
#else
                                return;
#endif
                            }
                        }
                    } // if IS_ITEM
                    else
                    {
                        break; // for value, no such consideration, just move the cursor as usual

                    }
                } // while
#else
                // allow move to disabled item
                __bMENUSelect[__bMENUCurrentLevel]--;

#endif


                if (__bMENUCurrentLevel < MENU_LEVEL-1)
		    __bMENUSelect[__bMENUCurrentLevel+1] = 0; // set select to 0, the first item. 

		__wMENUPrevItemId = __wMENUCurrentItemId;

		if (__bMENUCurrentLevel == 0)
                {
                    OSDSETUP_ShowDescriptionText();
                    // Brian1.08a, display menu settings when menu cursor in on Icon menu 
#ifdef SUPPORT_MENU_SETTINGS_DISPLAY
                    _ClearCurrentSettings();
                    _DisplayCurrentSettings();
#endif

                }

                if(GET_ITEM_ATTRIBUTE(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]) == IS_LINE)
                {
                    // I assume nobody will put a seperate line as the first item,
                    // so I don't check if it is out of the range
                    __bMENUSelect[__bMENUCurrentLevel]--;
                    bItemNum = 1; // just use it to keep if this is a seperate line
                }
                else
                    bItemNum = 0; // just use it to keep if this is a seperate line

                if (__bMENUSelect[__bMENUCurrentLevel] < __bMENUDisplayStart[__bMENUCurrentLevel])
                {
#ifdef SKIP_DISABLED_ITEM
                    __bMENUDisplayStart[__bMENUCurrentLevel] = __bMENUSelect[__bMENUCurrentLevel] -bItemNum;
#else
                    __bMENUDisplayStart[__bMENUCurrentLevel] -= (1+bItemNum);
#endif
                    OSDMENU_DisplayMenu(__bCurrentMenuId, 0xFF, __bMENUCurrentLevel);
                }
                else
                {
#ifdef SKIP_DISABLED_ITEM
                    // Brian1.05, don't move cursor onto disabled item.
                    // alan090
                    _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
                    OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], (BYTE)(bOrgItem+bItemNum), (BYTE)OSDMENU_ENTRY_ITEM_COLOR_NORMAL);
#else
                    OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], (BYTE)(__bMENUSelect[__bMENUCurrentLevel]+1+bItemNum), (BYTE)OSDMENU_ENTRY_ITEM_COLOR_NORMAL);
#endif

                }

		__wMENUCurrentItemId = GET_ITEM_ID(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]);

                // clear the previous menu
#ifdef SKIP_DISABLED_ITEM
                _pTempMenu = GET_SUB_MENU(_pTempMenu, bOrgItem+bItemNum);
#else
                _pTempMenu = GET_SUB_MENU(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]+1+bItemNum);
#endif
                if (_pTempMenu)
                {
                    OSDMENU_ClearMenu();
                    __bMENUStatus[__bMENUCurrentLevel+1] &= 0xFE; // clear scroll-up bit
                    __bMENUStatus[__bMENUCurrentLevel+1] &= 0xFD; // clear scroll-down bit
                }
                _pTempMenu = _pMenuIndex[__bCurrentMenuId];
                

                _bMENUTemp = __bMENUSelect[__bMENUCurrentLevel];
                if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_ITEM)
                {
                    if (*(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8)))
                    {
                        // item is enabled, set the text color as normal color
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
                        OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);
                        OSDMENU_DisplaySubMenu(__bMENUSelect[__bMENUCurrentLevel], __bMENUCurrentLevel);
                    }
                    else
                    {
                        // item is disabled, set the text color as disable color
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_DISABLE;
                        OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);

                    }
                    break;
                }
				// Brian1.02a,for Dialog like "Parental", "Change Password"
		else if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_DIALOG)
		{
			if (*(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8)))
			{
			// item is enabled, set the text color as normal color
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
			}
			else
			{
			// item is disabled, set the text color as disable color
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_DISABLE;
			}
		}

                OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);
                OSDMENU_DisplaySubMenu(__bMENUSelect[__bMENUCurrentLevel], __bMENUCurrentLevel);
            }
            break;
        // Brian1.08a
        case OSDMENU_GOTO_NEXT_ITEM:
            if (__bMENUSelect[__bMENUCurrentLevel] != bItemNum-1)
            {
#ifdef SKIP_DISABLED_ITEM
                // Brian1.05, don't move cursor onto disabled item.
                bOrgItem = __bMENUSelect[__bMENUCurrentLevel]; // keep the current select

                while(__bMENUSelect[__bMENUCurrentLevel] != bItemNum-1)
                {
                    __bMENUSelect[__bMENUCurrentLevel]++;

                    _bMENUTemp = __bMENUSelect[__bMENUCurrentLevel]; 

                    if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_ITEM||
                       GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_DIALOG)
                    {
                        if (*(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8))) // if enabled
                        {
                            break; // find a enabled item
                        }
                        else
                        {
                            if (__bMENUSelect[__bMENUCurrentLevel] == bItemNum-1)
                            {
                                // no enabled item is found, so the cursor remain still
                                __bMENUSelect[__bMENUCurrentLevel] = bOrgItem;
                                return;
                            }
                        }
                    } // if IS_ITEM
                    else
                    {
                        break; // for value, no such consideration, just move the cursor as usual

                    }
                } // while
#else
                __bMENUSelect[__bMENUCurrentLevel]++;
#endif

		if (__bMENUCurrentLevel < MENU_LEVEL-1)
			__bMENUSelect[__bMENUCurrentLevel+1] = 0; // set select to 0, the first item.

		__wMENUPrevItemId = __wMENUCurrentItemId;

		if (__bMENUCurrentLevel == 0) 
                {
                    OSDSETUP_ShowDescriptionText();
                    // Brian1.08a, display menu settings when menu cursor in on Icon menu 
#ifdef SUPPORT_MENU_SETTINGS_DISPLAY
                    _ClearCurrentSettings();
                    _DisplayCurrentSettings();
#endif
                }

                if(GET_ITEM_ATTRIBUTE(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]) == IS_LINE)
                {
                    // I assume nobody will put a seperate line as the last item,
                    // so I don't check if it is out of the range
                    __bMENUSelect[__bMENUCurrentLevel]++;
                    bItemNum = 1; // just use it to keep if this is a seperate line
                }
                else
                    bItemNum = 0; // just use it to keep if this is a seperate line

                if (__bMENUSelect[__bMENUCurrentLevel] > __bMENUDisplayStart[__bMENUCurrentLevel]+GET_MENU_ROW(_pTempMenu)-1)
                {
#ifdef SKIP_DISABLED_ITEM
                    __bMENUDisplayStart[__bMENUCurrentLevel] = __bMENUSelect[__bMENUCurrentLevel] - GET_MENU_ROW(_pTempMenu) +(1+bItemNum);
#else
                    __bMENUDisplayStart[__bMENUCurrentLevel] += (1+bItemNum);
#endif
                    OSDMENU_DisplayMenu(__bCurrentMenuId, 0xFF, __bMENUCurrentLevel);
                }
                else
                {
#ifdef SKIP_DISABLED_ITEM                    
                    // Brian1.05, don't move cursor onto disabled item.
                    // alan090
                    _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
                    OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], (BYTE)(bOrgItem-bItemNum), (BYTE)OSDMENU_ENTRY_ITEM_COLOR_NORMAL);
#else
                    OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], (BYTE)(__bMENUSelect[__bMENUCurrentLevel]-1-bItemNum), (BYTE)OSDMENU_ENTRY_ITEM_COLOR_NORMAL);
#endif
                }

		__wMENUCurrentItemId = GET_ITEM_ID(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]);

                // clear the previous menu
#ifdef SKIP_DISABLED_ITEM
                _pTempMenu = GET_SUB_MENU(_pTempMenu, bOrgItem-bItemNum);
#else
                _pTempMenu = GET_SUB_MENU(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]-1-bItemNum);
#endif
                if (_pTempMenu)
                {
                    OSDMENU_ClearMenu();
                    __bMENUStatus[__bMENUCurrentLevel+1] &= 0xFE; // clear scroll-up bit
                    __bMENUStatus[__bMENUCurrentLevel+1] &= 0xFD; // clear scroll-down bit
                }
                _pTempMenu = _pMenuIndex[__bCurrentMenuId];
                
                _bMENUTemp = __bMENUSelect[__bMENUCurrentLevel];
                if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_ITEM)
                {
                    if (*(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8)))
                    {
                        // item is enabled, set the text color as normal color
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
                        OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);
                        OSDMENU_DisplaySubMenu(__bMENUSelect[__bMENUCurrentLevel], __bMENUCurrentLevel);
                    }
                    else
                    {
                        // item is disabled, set the text color as disable color
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_DISABLE;
                        OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);

                    }
                    break;
                }
		// Brian1.02a,for Dialog like "Parental", "Change Password"
		else if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_DIALOG)
		{
			if (*(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8)))
			{
			// item is enabled, set the text color as normal color
                            _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL; 
			}
			else
			{
			// item is disabled, set the text color as disable color
                            _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_DISABLE; 
			}
		}

                OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);
                OSDMENU_DisplaySubMenu(__bMENUSelect[__bMENUCurrentLevel], __bMENUCurrentLevel);
            }
            break;
        // Brian1.08a
        case OSDMENU_GOTO_SUB_MENU:
             _bMENUTemp = __bMENUSelect[__bMENUCurrentLevel];

             if (GET_SUB_MENU(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]) &&
                 *(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8)) ) // && __bMENUCurrentLevel < (MENU_LEVEL-1))
             {

                // Brian1.08a, from icon menu to its sub-menu, so clear the clear the display of settings 
#ifdef SUPPORT_MENU_SETTINGS_DISPLAY
               
		if (__bMENUCurrentLevel == 0)
                {
                    _ClearCurrentSettings();
                }
#endif
                
                
		__wMENUPrevItemId = __wMENUCurrentItemId;
		// change the highlight to select
                // alan090
                _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
                OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_SELECT);

                __bMENUParent[__bMENUCurrentLevel+1] = __bCurrentMenuId;
                _pTempMenu = GET_SUB_MENU(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]);
                {
                    __bCurrentMenuId = GET_MENU_ID(_pTempMenu);
                    __bMENUCurrentLevel++;

			__wMENUCurrentItemId = GET_ITEM_ID(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]);

                _bMENUTemp = __bMENUSelect[__bMENUCurrentLevel];

#ifdef SKIP_DISABLED_ITEM
                // Brian1.05, don't move cursor onto disabled item.
                bOrgItem = __bMENUSelect[__bMENUCurrentLevel]; // keep the current select

                while(__bMENUSelect[__bMENUCurrentLevel] != bItemNum-1)
                {
                    
                    _bMENUTemp = __bMENUSelect[__bMENUCurrentLevel]; 

                    if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_ITEM||
                       GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_DIALOG)
                    {
                        if (*(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8))) // if enabled
                        {
                            __wMENUCurrentItemId = GET_ITEM_ID(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]);
                            break; // find a enabled item
                        }
                        else
                        {
                            if (__bMENUSelect[__bMENUCurrentLevel] == bItemNum-1)
                            {
                                // no enabled item is found, so the cursor remain still
                                __bMENUSelect[__bMENUCurrentLevel] = bOrgItem;
                                return;
                            }
                        }

                        __bMENUSelect[__bMENUCurrentLevel]++;

                    } // if IS_ITEM
                    else
                    {
                        break; // for value, no such consideration, just move the cursor as usual
                    }

                } // while
#endif


                if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_ITEM ||
                    GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_DIALOG) // Brian1.05
                {
                    if (*(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8)))
                    {
                        // item is enabled, set the text color as normal color
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
                        OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);
                        OSDMENU_DisplaySubMenu(__bMENUSelect[__bMENUCurrentLevel], __bMENUCurrentLevel);
                    }
                    else
                    {
                        // item is disabled, set the text color as disable color
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_DISABLE;
                        OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);

                    }
                    break;
                }
                    OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);
                    OSDMENU_DisplaySubMenu(__bMENUSelect[__bMENUCurrentLevel], __bMENUCurrentLevel);
                }
             }
             break;
        // Brian1.08a
        case OSDMENU_GOTO_PARENT_MENU:
// Brian1.08a, support Horizontal icon menu
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
             // for Horizontal icon case, KEY_LEFT won't go to icon menu
             if (__bMENUCurrentLevel == 1 && bKey == KEY_LEFT)
             {
                 break;
             }
             else
#endif
             if (__bMENUCurrentLevel > 0)

             {
		 __wMENUPrevItemId = __wMENUCurrentItemId;

                // change the highlight to normal
                //alan090       
                _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
                OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_NORMAL);
                // Clear the sub menu
                _pTempMenu = GET_SUB_MENU(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]);
                if (_pTempMenu)
                {
                    OSDMENU_ClearMenu();
                    __bMENUStatus[__bMENUCurrentLevel+1] &= 0xFE; // clear scroll-up bit
                    __bMENUStatus[__bMENUCurrentLevel+1] &= 0xFD; // clear scroll-down bit
                }

                _pTempMenu = _pMenuIndex[__bMENUParent[__bMENUCurrentLevel]];
                if (_pTempMenu)
                {

                    __bCurrentMenuId = GET_MENU_ID(_pTempMenu);
                    __bMENUCurrentLevel--;
                    __wMENUCurrentItemId = GET_ITEM_ID(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]);
                    OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel], __bMENUSelect[__bMENUCurrentLevel], OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);
                }

                // Brian1.08a, from sub menu to the icon menu, so display the settings 
#ifdef SUPPORT_MENU_SETTINGS_DISPLAY
                
		if (__bMENUCurrentLevel == 0)
                {
                    _DisplayCurrentSettings();
                }
#endif
             }
             break;
            case OSDMENU_SELECT_ITEM:
		_bMENUTemp = __bMENUSelect[__bMENUCurrentLevel];
		if (GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_VALUE)
		{
                        // the "Read default value" Value has no check
			if (GET_MENU_CHECK(_pTempMenu) != NULL)
			{

                    // Brian1.08, search the menu to get item position
                    // the current check item
                    bItemNum = _GetMenuItemPosition(*(GET_MENU_CHECK(_pTempMenu)));

			if (bItemNum >=__bMENUDisplayStart[__bMENUCurrentLevel] && bItemNum < __bMENUDisplayStart[__bMENUCurrentLevel]+GET_MENU_ROW(_pTempMenu))
			{
			// the current checked item is displayed on the screen, un-check it
				OSDMENU_DrawCheckMark((BYTE)(bItemNum-__bMENUDisplayStart[__bMENUCurrentLevel]), (BYTE)OSDMENU_ENTRY_ITEM_COLOR_NORMAL, FALSE);
			}

			*(GET_MENU_CHECK(_pTempMenu)) = (BYTE)__wMENUCurrentItemId;
			OSDMENU_DrawCheckMark((BYTE)(_bMENUTemp-__bMENUDisplayStart[__bMENUCurrentLevel]), OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT, TRUE);
			}

#ifdef SUPPORT_MENU_CURSOR_STAY_AT_VALUE_AFTER_ENTER
                    // Brian1.08a
                    // menu cursor will stay at the value menu after Key Enter
                    // but the _SaveDataToSetupInfo depends on the __wMENUCurrentItemId which is item ID to save the value to EEPROM and do actions
                    // if the cursor is stay at value menu, then the ID is the value ID, not item ID.
                    // We don't want to change _SaveDataToSetupInfo so much for this option, ===> so we can use a extra variable
                    // to record the Item ID while the __wMENUCurrentItemId is still the value ID as it should be.
                    // And in _SaveDataToSetupInfo, we use the new variable instead.
                    _wMENUTemp = __bMENUParent[__bMENUCurrentLevel];
                    //_pTempMenu = _pMenuIndex[_wMENUTemp]; // don't change _pTempMenu, or we need to set it back after temp usage
                    __wMENUItemIdAfterEnter = GET_ITEM_ID(_pMenuIndex[_wMENUTemp], __bMENUSelect[__bMENUCurrentLevel-1]);



#else
                    // menu cursor will not mov to its parent (by issue a  "KEY_LEFT" operation) after KEY_ENTER
                    // Brian1.08a, use operation instead 
					//bKey = KEY_LEFT;
                    bMenuOperation = OSDMENU_GOTO_PARENT_MENU;
					goto process_key_op;
#endif
				}
				else
				{
                    // Brian1.08a
                    bMenuOperation = OSDMENU_GOTO_SUB_MENU;
					goto process_key_op;
				}

			 break;


    }

}




//  *********************************************************************
//  Function    :   OSDMENU_DisplayMenu
//  Description :   This function display the single menu
//  Arguments   :   bMenuId: the ID of menu 
//                  The ID is an index to _pMenuIndex[]
//                  bLevel: the item must be displayed.
//  Return      :   None  
//  Side Effect :
//  *********************************************************************

void OSDMENU_DisplayMenu(BYTE bMenuId, BYTE bItem, BYTE bLevel)
{
    _pTempMenu = _pMenuIndex[bMenuId];
    if (_pTempMenu) // for Main Menu "Exit" item, the submenu is null
    {
        if (bMenuId == SETUP_MENU_MAIN || bMenuId == SETUP_MENU_MAIN_WITH_PROLOGIC)
            _DisplayIconMenu();
        else
            _DisplayMenu(bItem, bLevel);
    }
}

//  *********************************************************************
//  Function    :   _DisplayIconMenu
//  Description :   This function display the single Icon menu
//  Arguments   :   None (assume icon menu is the root (level 0) and no scroll)
//  Global Var  :   _pTempMenu: The menu
//  Return      :   None  
//  Side Effect :
//  *********************************************************************
//  This fuction would only be called when the Root (icon) menu is first displayed
void _DisplayIconMenu(void)
{
    _bMENUTemp2 = GET_MENU_ITEM_NUM(_pTempMenu);
    _MENU_StrAttr.bColorKey = 0;

    for (_bMENUTemp=0; _bMENUTemp <_bMENUTemp2; _bMENUTemp++)
    {
#ifdef REMOVE_SETUP_ICON
    _MENU_StrAttr.bShadow1 = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
    _MENU_StrAttr.bShadow2 = OSDMENU_ENTRY_TEXT_COLOR_DISABLE;
#endif

        // check if disable
        if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_ITEM)
        {
            if (*(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8)))
            {
#ifndef REMOVE_SETUP_ICON
                // item is enabled, set the text color as normal color
                if (_bMENUTemp == __bMENUSelect[__bMENUCurrentLevel])
                    _wMENUTemp = BUTTON_HIGHLIGHT;
		else
                    _wMENUTemp = BUTTON_NORMAL; //|BUTTON_WITH_TEXT;
#else
                // item is enabled, set the text color as normal color
                if (_bMENUTemp == __bMENUSelect[__bMENUCurrentLevel])

                {
                    _wMENUTemp = OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT;
                    _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_BLUE;
		}
		else
                {
                    _wMENUTemp = OSDMENU_ENTRY_ITEM_COLOR_NORMAL;
                    _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_BLUE;
		}
#endif
            }
            else
            {
#ifndef REMOVE_SETUP_ICON
                // item is disabled, set the text color as disable color
                _wMENUTemp = BUTTON_DISABLED; //|BUTTON_WITH_TEXT;
#else
		_wMENUTemp = OSDMENU_ENTRY_ITEM_COLOR_NORMAL;
                _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_DISABLE;
                _MENU_StrAttr.bShadow1 = OSDMENU_ENTRY_DESC_TEXT_COLOR;
					  
#endif
            }
        }

#ifndef REMOVE_SETUP_ICON
        // just for test
        switch(_bMENUTemp)
        {
        case 0:
            _wMENUString[1] = BUTTON_ID_LANGUAGE;
            break;
        case 1:
            _wMENUString[1] = BUTTON_ID_SCREEN;
            break;
        case 2:
            _wMENUString[1] = BUTTON_ID_AUDIO;
            break;
        case 3:
            _wMENUString[1] = BUTTON_ID_CUSTOM;
            break;
        case 4:
            _wMENUString[1] = BUTTON_ID_EXIT;
            break;
        }

// Brian1.08a
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
 
        // arrange the icon menu in horizontal direction here
        _InitRect(&(_MENU_BtnAttr.rect),
                  GET_MENU_X(_pTempMenu)+(MENU_ICON_X_OFFSET+MENU_ICON_WIDTH)*_bMENUTemp,
                  GET_MENU_Y(_pTempMenu),
                  GET_MENU_X(_pTempMenu)+(MENU_ICON_X_OFFSET+MENU_ICON_WIDTH)*_bMENUTemp+MENU_ICON_WIDTH-1,
                  GET_MENU_Y(_pTempMenu)+MENU_ICON_HEIGHT-1);
                  
        _MENU_BtnAttr.bButtonId = _wMENUString[1];
        _MENU_BtnAttr.bStyle = _wMENUTemp;
   
        GDI_DrawButton_909(0, &_MENU_BtnAttr);

#else

        _InitRect(&(_MENU_BtnAttr.rect), GET_MENU_X(_pTempMenu),
                  GET_MENU_Y(_pTempMenu) + (MENU_ICON_Y_OFFSET+MENU_ICON_HEIGHT)*_bMENUTemp,
                  GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-1,
                  GET_MENU_Y(_pTempMenu)+(MENU_ICON_Y_OFFSET+MENU_ICON_HEIGHT)*_bMENUTemp+MENU_ICON_HEIGHT-1);
                  
        _MENU_BtnAttr.bButtonId = _wMENUString[1];
        _MENU_BtnAttr.bStyle = _wMENUTemp;
   
        GDI_DrawButton_909(0, &_MENU_BtnAttr);

#endif

#else	//#ifndef REMOVE_SETUP_ICON

    _MENU_StrAttr.bBackground = _wMENUTemp;

#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
    _InitRect((PURECT)&(_MENU_FrameAttr),
              GET_MENU_X(_pTempMenu)+(MENU_ICON_X_OFFSET+MENU_ICON_WIDTH)*_bMENUTemp - 2,
              GET_MENU_Y(_pTempMenu) - 2,
              GET_MENU_X(_pTempMenu)+(MENU_ICON_X_OFFSET+MENU_ICON_WIDTH)*_bMENUTemp+MENU_ICON_WIDTH-2,
              GET_MENU_Y(_pTempMenu)+MENU_ICON_HEIGHT+2);
#else
    _InitRect((PURECT)&(_MENU_FrameAttr),
              GET_MENU_X(_pTempMenu),
              GET_MENU_Y(_pTempMenu)+ (MENU_ICON_Y_OFFSET+MENU_ICON_HEIGHT+4)*_bMENUTemp,
              GET_MENU_X(_pTempMenu)+ GET_MENU_WIDTH(_pTempMenu)-5,
              GET_MENU_Y(_pTempMenu)+ (MENU_ICON_Y_OFFSET+MENU_ICON_HEIGHT+4)*_bMENUTemp + MENU_ICON_HEIGHT + 3);
#endif

    _OSDMENU_DrawFrame(FRAME_POP); //CoCo1.20, reduce code size.
    
        switch(_bMENUTemp)
        {
        case 0:
            _wMENUString[1] = CHAR_SETUP_LANG1;
            _wMENUString[2] = CHAR_SETUP_LANG2;	    
            break;
        case 1:
            _wMENUString[1] = CHAR_SETUP_SCRN1;
            _wMENUString[2] = CHAR_SETUP_SCRN2;	
            break;
        case 2:
            _wMENUString[1] = CHAR_SETUP_AUDIO1;
            _wMENUString[2] = CHAR_SETUP_AUDIO2;	
            _MENU_StrAttr.bShadow2 = OSDSETUP_ENTRY_ICON_COLOR_GREEN;
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE;
            break;
        case 3:
            _wMENUString[1] = CHAR_SETUP_CUSTOM1;
            _wMENUString[2] = CHAR_SETUP_CUSTOM2;
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE;
            break;
        case 4:
            _wMENUString[1] = CHAR_SETUP_EXIT1;
            _wMENUString[2] = CHAR_SETUP_EXIT2;
            _MENU_StrAttr.bShadow2 = OSDSETUP_ENTRY_ICON_COLOR_RED;
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE;
            break;
        }

	_wMENUString[0]=2;
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) +
                           (MENU_ICON_X_OFFSET+MENU_ICON_WIDTH)*_bMENUTemp,
        _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu);
#else
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) + MENU_BORDER_THICKNESS;
        _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu) + 
                           (MENU_ICON_Y_OFFSET+MENU_ICON_HEIGHT+4) * _bMENUTemp +
                           MENU_BORDER_THICKNESS;
#endif
       _OSDMENU_DrawString(); //CoCo1.20, reduce code size.

        switch(_bMENUTemp)
        {
        case 0:
            _wMENUString[1] = CHAR_SETUP_LANG3;
            _wMENUString[2] = CHAR_SETUP_LANG4;	    
            break;
        case 1:
            _wMENUString[1] = CHAR_SETUP_SCRN3;
            _wMENUString[2] = CHAR_SETUP_SCRN4;	
            break;
        case 2:
            _wMENUString[1] = CHAR_SETUP_AUDIO3;
            _wMENUString[2] = CHAR_SETUP_AUDIO4;	
            _MENU_StrAttr.bShadow2 = OSDSETUP_ENTRY_ICON_COLOR_GREEN; 
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE; 
            break;
        case 3:
            _wMENUString[1] = CHAR_SETUP_CUSTOM3;
            _wMENUString[2] = CHAR_SETUP_CUSTOM4;
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE; 
            break;
        case 4:
            _wMENUString[1] = CHAR_SETUP_EXIT3;
            _wMENUString[2] = CHAR_SETUP_EXIT4;
            _MENU_StrAttr.bShadow2 = OSDSETUP_ENTRY_ICON_COLOR_RED; 
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE; 
            break;
        }

	_wMENUString[0]=2;

#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) +
                           (MENU_ICON_X_OFFSET+MENU_ICON_WIDTH)*_bMENUTemp,
        _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu) + MENU_TOP_ICON_HEIGHT; 
#else
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) + MENU_BORDER_THICKNESS;
        _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu) + 
                           (MENU_ICON_Y_OFFSET+MENU_ICON_HEIGHT+4) * _bMENUTemp +
                           MENU_BORDER_THICKNESS+25;
#endif
        _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
	
#endif	//#ifndef REMOVE_SETUP_ICON

    }
}

//  *********************************************************************
//  Function    :   _DisplayMenu
//  Description :   This function display the single menu
//  Arguments   :   bItem: the item must be displayed.
//              :   bLevel: the level of menu, root = 0, next is 1,2,...
//  Global Var  :   _pTempMenu: The menu
//  Return      :   None  
//  Side Effect :
//  *********************************************************************

void _DisplayMenu(BYTE bItem, BYTE bLevel)
{
    BYTE i;
/* Alan070
#ifdef OFFSCREEN_MENU
    if(__bCurrentRegionId == 0)
        GDI_SetCurrentRegion(1);
    else
        GDI_SetCurrentRegion(0);

    // ****
    // clear the scroll-up/down area first, or there may be garbage
#endif
*/


    _bMENUTemp = GET_MENU_ITEM_NUM(_pTempMenu);
    _bMENUTemp2 = GET_MENU_ROW(_pTempMenu);
    if (_bMENUTemp2 > _bMENUTemp)
        _wMENUTemp = _bMENUTemp;
    else
        _wMENUTemp = _bMENUTemp2;

    _InitRect((PURECT)&(_MENU_FrameAttr),
              GET_MENU_X(_pTempMenu),
              GET_MENU_Y(_pTempMenu),
              GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-1,
              GET_MENU_Y(_pTempMenu)+MENU_ITEM_HEIGHT*_wMENUTemp+MENU_BORDER_THICKNESS*2-1);

    _OSDMENU_DrawFrame(FRAME_POP_SOLID); //CoCo1.20, reduce code size.

    _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
    _MENU_StrAttr.bBackground = OSDMENU_ENTRY_ITEM_COLOR_NORMAL;

    if (bItem != 0xFF)
    {
        // first display, calculate the display range.

        _wMENUTemp = (_bMENUTemp2 - 1)/2;  //up-half
        if (_wMENUTemp > bItem)
            bItem = 0; // display starts from the first item
        else
            bItem = bItem - _wMENUTemp;

        if (_bMENUTemp2+bItem > _bMENUTemp)
        {
            _wMENUTemp = _bMENUTemp2+bItem-_bMENUTemp;
            if (_wMENUTemp > bItem)
            {
                if (_bMENUTemp2 > _bMENUTemp)
                    _bMENUTemp2 = _bMENUTemp;
                bItem = 0; // display starts from the first item
            }
            else
                bItem = bItem - _wMENUTemp;
        }

        __bMENUDisplayStart[bLevel] = bItem; // record the start item of the displaying area
    }
    else
    {
        // not first time, use the __bMENUDisplayStart
        bItem = __bMENUDisplayStart[bLevel];

    }

     for (_bMENUTemp=bItem; (_bMENUTemp<_bMENUTemp2+bItem) && (_bMENUTemp<GET_MENU_ITEM_NUM(_pTempMenu)); _bMENUTemp++)
    {
        
        
        if (_bMENUTemp==bItem)
        {
            if (bItem != 0)
            {
                {
                // Draw the scroll up mark

                if (!(__bMENUStatus[bLevel] & 0x01))
                {
                    _InitRect((PURECT)&(_MENU_FrameAttr),
                              GET_MENU_X(_pTempMenu),
                              GET_MENU_Y(_pTempMenu)-MENU_SCROLL_HEIGHT, 
                              GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-1,
                              GET_MENU_Y(_pTempMenu)-1);
                    _OSDMENU_DrawFrame(FRAME_POP_SOLID); //CoCo1.20, reduce code size.

                }

#ifdef SKIP_DISABLED_ITEM
                // Brian1.05, set Up mark color to disable if all the "Up" items are disbaled, ie., can't scroll up
                if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_ITEM||
                   GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_DIALOG)
                {
                    for (i=0; i<bItem; i++)
                    {
                        if (*(GET_MENU_ENABLE(_pTempMenu)+i/8) & (1 << (i%8))) // if enabled
                        {
                            break; // find a enabled item
                        }
                    }

                    if (i==bItem)
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_DISABLE; 
                    else
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL; 
                }
                else

                {
                    // it is value, just set Up mark as normal color
                    _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL; 
                }
#endif                


                _wMENUString[1] = CHAR_N_UP_16; 
                _wMENUString[0] = 1; 
                _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) + MENU_BORDER_THICKNESS + 
                                   GET_MENU_WIDTH(_pTempMenu)/2 - 16;
                _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu) - MENU_SCROLL_HEIGHT +
                                   MENU_BORDER_THICKNESS;
                _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
                __bMENUStatus[bLevel] |= 0x01; // set scroll-up bit
                }

            }
            else
            {
                if (__bMENUStatus[bLevel] & 0x01)
                {
                    // Clear the scroll up mark
                    _MENU_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
                    _OSDMENU_FillRect(GET_MENU_X(_pTempMenu),
                              GET_MENU_Y(_pTempMenu)-MENU_SCROLL_HEIGHT,
                              GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-1,
                              GET_MENU_Y(_pTempMenu)-1); //CoCo1.20, reduce code size.
                    __bMENUStatus[bLevel] &= 0xFE; // clear scroll-up bit
                }
            }
        }
        else if (_bMENUTemp==_bMENUTemp2+bItem-1)
        {
            if (_bMENUTemp < GET_MENU_ITEM_NUM(_pTempMenu)-1)
            {
                {
                // Draw the scroll down mark

                if (!(__bMENUStatus[bLevel] & 0x02))
                {
                    _InitRect((PURECT)&(_MENU_FrameAttr),
                    GET_MENU_X(_pTempMenu),
                    GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS*2+(_bMENUTemp-bItem+1)*MENU_ITEM_HEIGHT,
                    GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-1,
                    GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS*2+(_bMENUTemp-bItem+1)*MENU_ITEM_HEIGHT+MENU_SCROLL_HEIGHT-1);

                    _OSDMENU_DrawFrame(FRAME_POP_SOLID); //CoCo1.20, reduce code size.
                }

#ifdef SKIP_DISABLED_ITEM
                // Brian1.05, set Down mark color to disable if all the "Down" items are disbaled, ie., can't scroll down
                if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_ITEM||
                   GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_DIALOG)
                {
                    for (i=_bMENUTemp+1; i<GET_MENU_ITEM_NUM(_pTempMenu); i++)
                    {
                        if (*(((BYTE*)(_pTempMenu[0].pbItemText))+i/8) & (1 << (i%8))) // if enabled
                        {
                            break; // find a enabled item
                        }
                    }

                    if (i==GET_MENU_ITEM_NUM(_pTempMenu))
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_DISABLE;
                    else
                        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
                }
                else
                {
                    // it is value, just set Up mark as normal color
                    _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
                }
#endif

                _wMENUString[0] = 1; 
                _wMENUString[1] = CHAR_N_DWN_16; 
                //__bTemp = GDI_GetStringHeight(&_wMENUString);
                _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) + MENU_BORDER_THICKNESS +
                                   GET_MENU_WIDTH(_pTempMenu)/2 - 16;
                _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu) + MENU_BORDER_THICKNESS*3 +
                                   (_bMENUTemp-bItem+1)*MENU_ITEM_HEIGHT;
                _OSDMENU_DrawString(); //CoCo1.20, reduce code size.

                __bMENUStatus[bLevel] |= 0x02; // set scroll-down bit
                }
            }
            else
            {
                if (__bMENUStatus[bLevel] & 0x02)
                {
                // Clear the scroll-down mark
                    _MENU_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
                    _OSDMENU_FillRect(GET_MENU_X(_pTempMenu),
                              GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS*2+(_bMENUTemp-bItem+1)*MENU_ITEM_HEIGHT,
                              GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-1,
                              GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS*2+(_bMENUTemp-bItem+1)*MENU_ITEM_HEIGHT+MENU_SCROLL_HEIGHT-1); //CoCo1.20, reduce code size.
                    __bMENUStatus[bLevel] &= 0xFD; // clear scroll-down bit
                }

            }
            
        }
        //else

        // check if disable
        if(GET_ITEM_ATTRIBUTE(_pTempMenu, bItem) == IS_ITEM)
        //if (GET_MENU_ENABLE(_pTempMenu) != NULL) // the _bEnabelxxx exist
        {
            if (*(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & (1 << (_bMENUTemp%8)))
            {
                // item is enabled, set the text color as normal color
                _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
            }
            else
            {
                // item is disabled, set the text color as disable color
                _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_DISABLE;
            }

        }

        {
            // Draw single Menu Item
            OSDMENU_DrawItem(bItem, _bMENUTemp, OSDMENU_ENTRY_ITEM_COLOR_NORMAL);
        }

    }

}

//  *********************************************************************
//  Function    :   OSDMENU_DisplaySubMenu
//  Description :   This function display the single menu
//  Arguments   :   bItem: the item must be displayed.
//  Global Var  :   _pTempMenu: The menu
//  Return      :   None  
//  Side Effect :
//  *********************************************************************
void OSDMENU_DisplaySubMenu(BYTE bItem, BYTE bLevel)
{
    if (GET_ITEM_ATTRIBUTE(_pTempMenu, bItem)==IS_ITEM)
    {
        _pTempMenu = _pTempMenu[2+bItem].pSubMenu; // get sub menu
        // Brian1.10, for Keil 7, don't use expression like !pointer, the result may be wrong
        if (_pTempMenu == NULL) // for Main Menu "Exit" item, the submenu is null
            return;

        if(GET_ITEM_ATTRIBUTE(_pTempMenu, 0) == IS_VALUE) // use one item to judge if it is value sub menu
        {
            // find the checked item
            // for "Default Value->Read", it is a value but there is no check
            if (GET_MENU_CHECK(_pTempMenu) != NULL) // It is a "Value" sub menu
            {
                bItem = _GetMenuItemPosition(*(GET_MENU_CHECK(_pTempMenu)));
                // the checked item is also the selection, so record it at the same time.
                __bMENUSelect[bLevel+1] = bItem;
            }
            else
            {
                bItem = 0; // for "Default Value->Read", it is a value but there is no check
                // Brian1.24a
                __bMENUSelect[bLevel+1] = 0; // assign it to 0, the first item. 
            }
        }
        else
            bItem = 0; // it is a "item" sub menu, so just assign the first item as the one that must be displayed

        _DisplayMenu(bItem, (BYTE)(bLevel+1));
    }
}


//  *********************************************************************
//  Function    :   OSDMENU_DrawItem
//  Description :   This function draws a menu item text 
//  Arguments   :   bItem: the item number
//                  bColor: the color represent the state:
//                      MENU_ITEM_COLOR_HIGHLIGHT, MENU_ITEM_COLOR_SELECT or MENU_ITEM_COLOR_NORMAL 
//  Global Var  :   _pTempMenu: The menu
//  Return      :   None  
//  Side Effect :
//  Note        :   may use macro instead to save SRAM
//  *********************************************************************

void OSDMENU_DrawItem(BYTE bBase, BYTE bItem, BYTE bColor)
{
    
    // The following code handles Icon menu

    if (CHECK_ICON_ATTRIBUTE(_pTempMenu, bItem))
    {
#ifndef REMOVE_SETUP_ICON 
        switch (bColor)
        {
        case OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT:
            _wMENUString[5] = BUTTON_HIGHLIGHT; //|BUTTON_WITH_TEXT;
            break;
        case OSDMENU_ENTRY_ITEM_COLOR_NORMAL:
            _wMENUString[5] = BUTTON_NORMAL; //|BUTTON_WITH_TEXT;
            break;
        case OSDMENU_ENTRY_ITEM_COLOR_SELECT:
            _wMENUString[5] = BUTTON_PUSH; //|BUTTON_WITH_TEXT;
            break;
        }

        switch(bItem)
		{
        case 0:
            _wMENUString[1] = BUTTON_ID_LANGUAGE;
            break;
        case 1:
            _wMENUString[1] = BUTTON_ID_SCREEN;
            break;
        case 2:
            _wMENUString[1] = BUTTON_ID_AUDIO;
            break;
        case 3:
            _wMENUString[1] = BUTTON_ID_CUSTOM;
            break;
        case 4:
            _wMENUString[1] = BUTTON_ID_EXIT;
            break;
        }

// Brian1.08a
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
        // Draw the icon menu in horizontal direction here
        _InitRect(&(_MENU_BtnAttr.rect),
                  GET_MENU_X(_pTempMenu)+ (MENU_ICON_X_OFFSET+MENU_ICON_WIDTH)*bItem,
                  GET_MENU_Y(_pTempMenu),
                  GET_MENU_X(_pTempMenu)+(MENU_ICON_X_OFFSET+MENU_ICON_WIDTH)*bItem+MENU_ICON_WIDTH-1,
                  GET_MENU_Y(_pTempMenu)+MENU_ICON_HEIGHT-1);

        _MENU_BtnAttr.bButtonId = _wMENUString[1];
        _MENU_BtnAttr.bStyle = _wMENUString[5];

        GDI_DrawButton_909(0, &_MENU_BtnAttr);
#else
        _InitRect(&(_MENU_BtnAttr.rect),
                  GET_MENU_X(_pTempMenu),
                  GET_MENU_Y(_pTempMenu)+(MENU_ICON_Y_OFFSET+MENU_ICON_HEIGHT)*bItem,
                  GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-1,
                  GET_MENU_Y(_pTempMenu)+(MENU_ICON_Y_OFFSET+MENU_ICON_HEIGHT)*bItem+MENU_ICON_HEIGHT-1);

        _MENU_BtnAttr.bButtonId = _wMENUString[1];
        _MENU_BtnAttr.bStyle = _wMENUString[5];

        GDI_DrawButton_909(0, &_MENU_BtnAttr);
#endif

#else // #ifndef REMOVE_SETUP_ICON
/*
        switch (bColor)
        {
        case OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT:
            _MENU_StrAttr.bBackground = bColor;
            break;
        case OSDMENU_ENTRY_ITEM_COLOR_NORMAL:
        case OSDMENU_ENTRY_ITEM_COLOR_SELECT:
            _MENU_StrAttr.bBackground = OSDSETUP_ENTRY_COLOR_TOP_BANNER;
            break;
        }

 //alan085a adds for divx reg code
        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;

        _MENU_StrAttr.bShadow1 = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
        _MENU_StrAttr.bShadow2 = OSDMENU_ENTRY_TEXT_COLOR_DISABLE;
*/

            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_BLUE;
            _MENU_StrAttr.bBackground = bColor;
            _MENU_StrAttr.bShadow1 = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
            _MENU_StrAttr.bShadow2 = OSDMENU_ENTRY_TEXT_COLOR_DISABLE;

	bItem=bItem-bBase;
	
        switch(bItem)
        {
        case 0:
            _wMENUString[1] = CHAR_SETUP_LANG1;
            _wMENUString[2] = CHAR_SETUP_LANG2;	    
            break;
        case 1:
            _wMENUString[1] = CHAR_SETUP_SCRN1;
            _wMENUString[2] = CHAR_SETUP_SCRN2;	
            break;
        case 2:
            _wMENUString[1] = CHAR_SETUP_AUDIO1;
            _wMENUString[2] = CHAR_SETUP_AUDIO2;	
            _MENU_StrAttr.bShadow2 = OSDSETUP_ENTRY_ICON_COLOR_GREEN;
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE;
            break;
        case 3:
            _wMENUString[1] = CHAR_SETUP_CUSTOM1;
            _wMENUString[2] = CHAR_SETUP_CUSTOM2;
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE;
            break;
        case 4:
            _wMENUString[1] = CHAR_SETUP_EXIT1;
            _wMENUString[2] = CHAR_SETUP_EXIT2;
            _MENU_StrAttr.bShadow2 = OSDSETUP_ENTRY_ICON_COLOR_RED;
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE;
            break;
        }

	_wMENUString[0]=2;
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) +
                           (MENU_ICON_X_OFFSET+MENU_ICON_WIDTH)*bItem,
        _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu);
#else
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) + MENU_BORDER_THICKNESS;
        _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu) + (MENU_ICON_Y_OFFSET+MENU_ICON_HEIGHT+4)*bItem+MENU_BORDER_THICKNESS;
#endif

        _OSDMENU_DrawString(); //CoCo1.20, reduce code size.

        switch(bItem)
        {
        case 0:
            _wMENUString[1] = CHAR_SETUP_LANG3;
            _wMENUString[2] = CHAR_SETUP_LANG4;	    
            break;
        case 1:
            _wMENUString[1] = CHAR_SETUP_SCRN3;
            _wMENUString[2] = CHAR_SETUP_SCRN4;	
            break;
        case 2:
            _wMENUString[1] = CHAR_SETUP_AUDIO3;
            _wMENUString[2] = CHAR_SETUP_AUDIO4;	
            _MENU_StrAttr.bShadow2 = OSDSETUP_ENTRY_ICON_COLOR_GREEN;
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE;
            break;
        case 3:
            _wMENUString[1] = CHAR_SETUP_CUSTOM3;
            _wMENUString[2] = CHAR_SETUP_CUSTOM4;
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE;
            break;
        case 4:
            _wMENUString[1] = CHAR_SETUP_EXIT3;
            _wMENUString[2] = CHAR_SETUP_EXIT4;
            _MENU_StrAttr.bShadow2 = OSDSETUP_ENTRY_ICON_COLOR_RED;
            _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_ICON_COLOR_ORANGE;
            break;
        }
	_wMENUString[0]=2;
	
#ifdef SUPPORT_SETUP_HORIZONTAL_ICON_MENU
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) +
                           (MENU_ICON_X_OFFSET+MENU_ICON_WIDTH)*bItem,
        _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu) + MENU_TOP_ICON_HEIGHT;
#else
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) + MENU_BORDER_THICKNESS;
        _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu) + (MENU_ICON_Y_OFFSET+MENU_ICON_HEIGHT+4)*bItem+25+MENU_BORDER_THICKNESS;
#endif
        _OSDMENU_DrawString(); //CoCo1.20, reduce code size.

#endif // #ifndef REMOVE_SETUP_ICON

        return;
    }


    // the following code handles normal menu
    _MENU_StrAttr.bBackground = bColor;
    _MENU_StrAttr.bShadow1 = bColor;
    _MENU_StrAttr.bShadow2 = bColor;

    bBase = bItem - bBase;
    _MENU_RectAttr.bColor = bColor;
    _OSDMENU_FillRect(GET_MENU_X(_pTempMenu)+MENU_BORDER_THICKNESS,
                  GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+(bBase)*MENU_ITEM_HEIGHT,
                  GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-MENU_BORDER_THICKNESS-1,
                  GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+(bBase+1)*MENU_ITEM_HEIGHT-1); //CoCo1.20, reduce code size.

    if (GET_ITEM_ATTRIBUTE(_pTempMenu, bItem) == IS_LINE)
    {
        // Draw the line
        _MENU_RectAttr.bColor = OSDMENU_ENTRY_MENU_COLOR_DARK;
        _OSDMENU_FillRect(GET_MENU_X(_pTempMenu)+MENU_BORDER_THICKNESS+8, 
                  GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+(bBase)*MENU_ITEM_HEIGHT + (MENU_ITEM_HEIGHT/2-MENU_BORDER_THICKNESS),
                  GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-MENU_BORDER_THICKNESS-1-8,
                  GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+(bBase)*MENU_ITEM_HEIGHT +(MENU_ITEM_HEIGHT/2-1)); //CoCo1.20, reduce code size.

        _MENU_RectAttr.bColor = OSDMENU_ENTRY_MENU_COLOR_BRIGHT;
        _OSDMENU_FillRect(GET_MENU_X(_pTempMenu)+MENU_BORDER_THICKNESS+8, 
                  GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+(bBase)*MENU_ITEM_HEIGHT + (MENU_ITEM_HEIGHT/2),
                  GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-MENU_BORDER_THICKNESS-1-8,
                  GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+(bBase)*MENU_ITEM_HEIGHT +(MENU_ITEM_HEIGHT/2)+MENU_BORDER_THICKNESS-1); //CoCo1.20, reduce code size.
        return;
    }

    // output the item text
/*
    if (0 == __bMENUCurrentLevel)
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu)+MENU_BORDER_THICKNESS+30;
    else
*/
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu)+MENU_BORDER_THICKNESS+MENU_ITEM_X_OFFSET;

    _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+bBase*MENU_ITEM_HEIGHT;
    _OSDMENU_DrawDynamicString(GET_MENU_ITEM_TEXT(_pTempMenu, bItem)); //CoCo1.20, reduce code size.
    __wTemp = GDI_GetStringWidth(GET_MENU_ITEM_TEXT(_pTempMenu, bItem));

    switch (GET_ITEM_ATTRIBUTE(_pTempMenu, bItem))
    {
    case IS_ITEM:
        _wMENUString[1] = CHAR_N_SUBMENU;
        break;
    case IS_DIALOG:
        _wMENUString[1] = CHAR_DOTS;
        break;
    case IS_VALUE:
            if (GET_MENU_CHECK(_pTempMenu) != NULL) // It is a "Value" sub menu
            {
                // show the check mark for the checked item value.
                if (*(GET_MENU_CHECK(_pTempMenu)) == GET_ITEM_ID(_pTempMenu, bItem))
                {
                    OSDMENU_DrawCheckMark(bBase, bColor, TRUE);
                }
            }
        return;
    }

    _wMENUString[0] = 1;

    // output submenu mark or "..." mark
/*Alan1.11
    _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu) - MENU_BORDER_THICKNESS - MENU_ITEM_X_OFFSET;
*/
    _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu) + GET_MENU_WIDTH(_pTempMenu) - MENU_BORDER_THICKNESS - MENU_ITEM_SUBMENU_OFFSET; 
    _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+bBase*MENU_ITEM_HEIGHT;
    _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
}


//  *********************************************************************
//  Function    :   OSDMENU_DrawCheckMark
//  Description :   This function draws a menu item text 
//  Arguments   :   bLocation: location row in the drawing menu
//                  bColor: the color represent the state:
//                      MENU_ITEM_COLOR_HIGHLIGHT, MENU_ITEM_COLOR_SELECT or MENU_ITEM_COLOR_NORMAL 
//                  bCheck: TRUE/FALSE
//  Global Var  :   _pTempMenu: The menu
//  Return      :   None  
//  Side Effect :
//  Note        :   may use macro instead to save SRAM
//  *********************************************************************

void OSDMENU_DrawCheckMark(BYTE bLocation, BYTE bColor, BYTE bCheck)
{
#define CHECK_MARK_X_OFFSET 2//4 // Brian1.08a, set it to 2, so X position is multipe of 4

    _MENU_StrAttr.bBackground = bColor;
    _wMENUString[0] = 1;
    _wMENUString[1] = CHAR_N_CHECK;
    //bLocation = bItem - bBase;
    if (bCheck)
    {
        // output the Check mark
        _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu)+MENU_BORDER_THICKNESS+CHECK_MARK_X_OFFSET + MENU_CHECK_OFFSET;
        _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+bLocation*MENU_ITEM_HEIGHT;
        _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
    }
    else
    {
        _MENU_RectAttr.bColor = bColor;
        // uncheck, just clear the area to background color
        _OSDMENU_FillRect(GET_MENU_X(_pTempMenu)+MENU_BORDER_THICKNESS+CHECK_MARK_X_OFFSET,
                  GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+(bLocation)*MENU_ITEM_HEIGHT,
                  GET_MENU_X(_pTempMenu)+MENU_BORDER_THICKNESS+CHECK_MARK_X_OFFSET+GDI_GetStringWidth(_wMENUString) + MENU_CHECK_OFFSET,
                  GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+(bLocation+1)*MENU_ITEM_HEIGHT-1); //CoCo1.20, reduce code size.
    }
}

//  *********************************************************************
//  Function    :   OSDMENU_ClearMenu
//  Description :   This function clear a single menu
//  Arguments   :   None
//  Global Var  :   _pTempMenu: The menu
//  Return      :   None  
//  Side Effect :
//  *********************************************************************

void OSDMENU_ClearMenu(void)
{
    _bMENUTemp = GET_MENU_ROW(_pTempMenu);
    _MENU_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
    _OSDMENU_FillRect(GET_MENU_X(_pTempMenu),
              GET_MENU_Y(_pTempMenu)-MENU_SCROLL_HEIGHT,
              GET_MENU_X(_pTempMenu)+GET_MENU_WIDTH(_pTempMenu)-1,
              GET_MENU_Y(_pTempMenu)+MENU_ITEM_HEIGHT*_bMENUTemp+MENU_BORDER_THICKNESS*2+MENU_SCROLL_HEIGHT-1); //CoCo1.20, reduce code size.
}


void OSDSETUP_ShowDescriptionText(void)
{
    _MENU_RectAttr.bColor = OSDSETUP_ENTRY_COLOR_TOP_BANNER;
    _OSDMENU_FillRect(MENU_DESCRIPTION_X,
                  MENU_DESCRIPTION_Y,
                  MENU_DESCRIPTION_X+360,
                  MENU_DESCRIPTION_Y+30); //CoCo1.20, reduce code size.

    _MENU_StrAttr.wX = MENU_DESCRIPTION_X;
    _MENU_StrAttr.wY = MENU_DESCRIPTION_Y;
    _MENU_StrAttr.bTextColor = OSDSETUP_ENTRY_COLOR_TOP_BANNER_TEXT;
    _MENU_StrAttr.bBackground = OSDSETUP_ENTRY_COLOR_TOP_BANNER;
    _OSDMENU_DrawDynamicString(_MenuMain[__bMENUSelect[0]+2].pbItemText); //CoCo1.20, reduce code size.
}


void OSDSETUP_PasswordDlg(void)
{
    if ( __bSetupMode ) // ** TCH1.05-1-908; Do it as Parental_Control may call this API directly under non-SETUP mode.
    {
	// Brian1.02a, check if disabled
	// when comes here, _pTempMenu should be the valid (correct) value we hope, but it is not
	// so use _bEnableCustom[] directly
	_bMENUTemp = __bMENUSelect[__bMENUCurrentLevel];
	if (!(*(((BYTE*)(_bEnableCustom))+_bMENUTemp/8) & (1 << (_bMENUTemp%8))))
		return;
    }   // ** TCH1.05-1-908; 

	__bInputPasswordIndex = 0;
        __bInputPassword = TRUE;
#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG
#ifndef REMOVE_SETUP_HELP_MESSAGE
	OSDSETUP_ShowHelpText(__wMENUCurrentItemId);
#endif
#endif

	for (_bMENUTemp=0; _bMENUTemp<SETUP_PASSWORD_NO; _bMENUTemp++)
	{
		_wMENUString[_bMENUTemp*2] = CHAR_SPACE;
		_wMENUString[_bMENUTemp*2+1] = CHAR_LOW_LINE;
	}
	_wMENUString[0] = SETUP_PASSWORD_NO*2-1;

                           
//Brian0.91
if (__bSetupMode)
{    
        _InitRect((PURECT)&(_MENU_FrameAttr),
                  DIALOG_INPUT_PASSWORD_X1, DIALOG_INPUT_PASSWORD_Y1,
                  DIALOG_INPUT_PASSWORD_X2, DIALOG_INPUT_PASSWORD_Y2);
        _OSDMENU_DrawFrame(FRAME_DOUBLE_SOLID); //CoCo1.20, reduce code size.

        _OSDMENU_SetMenuStringColor(); //--CoCo1.20, reduce code size.            

        _MENU_StrAttr.wX = DIALOG_INPUT_PASSWORD_X1+DIALOG_INPUT_PASSWORD_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PASSWORD_Y1+DIALOG_INPUT_PASSWORD_Y_OFFSET;
        _OSDMENU_DrawString(); //CoCo1.20, reduce code size.

}
else
{
    OSD_ChangeUI(OSD_UI_PASSWORD, OSD_ENTER_UI);
    _MENU_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;  // Clear the OSDND UI
    _OSDMENU_FillRect(0, OSDND_UPPER_BACKGROUND_BOTTOM+1, GDI_REGION_WIDTH, 
                      OSDND_BOTTOM_BACKGROUND_START_V - 1); //CoCo1.20, reduce code size.

    // it is in normal/display mode, just use text style
    // share the same foreground and background color indexs of the external subitle 
    _MENU_StrAttr.bTextColor = GDI_DYNAMIC_ND_PALETTE_INDEX_START;
    _MENU_StrAttr.bBackground = GDI_DYNAMIC_ND_PALETTE_INDEX_START + 1;

    // Brian1.24a, clear to the end in horizontal direction.
    _MENU_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDMENU_FillRect(DIALOG_INPUT_PASSWORD_OSD_X1, DIALOG_INPUT_PASSWORD_OSD_Y1,
        DIALOG_INPUT_PASSWORD_OSD_X2, DIALOG_INPUT_PASSWORD_OSD_Y2); //CoCo1.20, reduce code size.

    _MENU_RectAttr.bColor = OSDCOMDLG_VALUE_COLOR_STRING;
    _OSDMENU_FillRect(DIALOG_INPUT_PASSWORD_OSD_X1, DIALOG_INPUT_PASSWORD_OSD_Y1,
        DIALOG_INPUT_PASSWORD_OSD_X2, DIALOG_INPUT_PASSWORD_OSD_Y2); //CoCo1.20, reduce code size.

    _wMENUTemp = GDI_GetStringWidth(aMsgPassword);

    _MENU_StrAttr.wX = DIALOG_INPUT_PASSWORD_OSD_X1+_wMENUTemp;
    _MENU_StrAttr.wY = DIALOG_INPUT_PASSWORD_OSD_Y1+DIALOG_INPUT_PASSWORD_OSD_Y_OFFSET;
    _OSDMENU_DrawString(); //CoCo1.20, reduce code size.

    _MENU_StrAttr.wX = DIALOG_INPUT_PASSWORD_OSD_X1;
    _MENU_StrAttr.wY = DIALOG_INPUT_PASSWORD_OSD_Y1;
    _OSDMENU_DrawDynamicString(aMsgPassword); //CoCo1.20, reduce code size.
}
}

//  ***************************************************************************
//  Function    :   SETUP_InputPassword
//  Description :   This function will show input password menu to let user input
//              :   password.
//  Arguments   :   bKey, current input key. KEY_NO_KEY==>enter input password status. (first time)
//  Return      :   SETUP_INPUT_PASSWORD_CORRECT, SETUP_INPUT_PASSWORD_INVALID,
//              :   SETUP_INPUT_PASSWORD_NORMAL, SETUP_INPUT_PASSWORD_INVALID_KEY.
//  Side Effect :
//  ***************************************************************************
BYTE    SETUP_InputPassword(BYTE bKey)
{
    //BYTE bStatus = 0;

    switch (bKey)
    {
    case KEY_NO_KEY:
         if (!__bSetupMode)
        {
            OSDSETUP_PasswordDlg();
            return SETUP_INPUT_PASSWORD_NORMAL;
        }
    case KEY_N0:
    case KEY_N1:
    case KEY_N2:
    case KEY_N3:
    case KEY_N4:
    case KEY_N5:
    case KEY_N6:
    case KEY_N7:
    case KEY_N8:
    case KEY_N9:
        if (__bInputPassword)
        {
            __bPassword[__bInputPasswordIndex] = bKey-KEY_N0;
            __bInputPasswordIndex++;

            if (__bInputPasswordIndex == SETUP_PASSWORD_NO)
            {
                if ((_GetPasswordNumber() == __SetupInfo.wDefaultPassword)
#ifdef  SUPPORT_GOLDEN_PASSWORD
                    || (_GetPasswordNumber() == GOLDEN_PASSWORD)
#endif
                    )
		{
                    bStatus = SETUP_INPUT_PASSWORD_CORRECT;
					
		}
                else
                    bStatus = SETUP_INPUT_PASSWORD_WRONG;
            }
            else
                bStatus = SETUP_INPUT_PASSWORD_NORMAL;
        }
        else
            bStatus = SETUP_INPUT_PASSWORD_INVALID_KEY;
        break;
    case KEY_OPEN_CLOSE:
    case KEY_POWER:
	case KEY_SETUP:
    case KEY_LEFT: // Brian0.91, key left also exit
        // Ming0.80, support multi-disc resume.
        //           Do not save the RESUME info. if it is in the password input mode.
#ifdef SUPPORT_MULTI_DISC_RESUME
        // If the SETUP_InputPassword is called from SETUP_ProcessKey().
        // The key of KEY_OPEN_CLOSE and KEY_POWER is processed in the
        // previous stage, and would not enter it.
        if((bKey == KEY_OPEN_CLOSE)||(bKey == KEY_POWER))
        {
            extern BYTE __fSaveResumeInfo;

            __fSaveResumeInfo = FALSE;
        }
#endif // #ifdef SUPPORT_MULTI_DISC_RESUME

        __bInputPasswordIndex = 0;
        __bInputPassword = FALSE;
        bStatus = SETUP_INPUT_PASSWORD_CLEAR;
        break;
    default:
        bStatus = SETUP_INPUT_PASSWORD_INVALID_KEY;
        break;
    }

    if (bStatus == SETUP_INPUT_PASSWORD_INVALID_KEY)
    {
    }
    else if (bStatus == SETUP_INPUT_PASSWORD_WRONG)
    {
        if (__bSetupMode) // Brian0.91
        {
            _MENU_StrAttr.wX = DIALOG_INPUT_PASSWORD_X1+DIALOG_INPUT_PASSWORD_X_OFFSET;
            _MENU_StrAttr.wY = DIALOG_INPUT_PASSWORD_Y1+DIALOG_INPUT_PASSWORD_Y_OFFSET;
            _OSDMENU_DrawDynamicString(aMsgWrongPassword); //CoCo1.20, reduce code size.
        }
        else
        {
            _wMENUTemp = GDI_GetStringWidth(aMsgPassword);

            _MENU_StrAttr.wX = DIALOG_INPUT_PASSWORD_OSD_X1+_wMENUTemp;
            _MENU_StrAttr.wY = DIALOG_INPUT_PASSWORD_OSD_Y1+DIALOG_INPUT_PASSWORD_OSD_Y_OFFSET;
            _OSDMENU_DrawDynamicString(aMsgWrongPassword); //CoCo1.20, reduce code size.
        }

        OS_DelayTime(COUNT_1_SEC+COUNT_500_MSEC); //CoCo1.20, reduce code size

    }
    else
    {
	if (bStatus != SETUP_INPUT_PASSWORD_CLEAR)
        {
		_wMENUString[0] = 1;
		_wMENUString[1] = CHAR_X;

            if (__bSetupMode)
            {
                _MENU_StrAttr.wX = DIALOG_INPUT_PASSWORD_X1+DIALOG_INPUT_PASSWORD_X_OFFSET+(__bInputPasswordIndex-1)*(GDI_GetCharWidth(CHAR_LOW_LINE)+GDI_GetCharWidth(CHAR_SPACE));
                _MENU_StrAttr.wY = DIALOG_INPUT_PASSWORD_Y1+DIALOG_INPUT_PASSWORD_Y_OFFSET;
                _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
            }
            else
            {
                _wMENUTemp = GDI_GetStringWidth(aMsgPassword);

                _MENU_StrAttr.wX = DIALOG_INPUT_PASSWORD_OSD_X1+_wMENUTemp+(__bInputPasswordIndex-1)*(GDI_GetCharWidth(CHAR_LOW_LINE)+GDI_GetCharWidth(CHAR_SPACE));
                _MENU_StrAttr.wY = DIALOG_INPUT_PASSWORD_OSD_Y1+DIALOG_INPUT_PASSWORD_OSD_Y_OFFSET;
                _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
            }
        }
    }

    if ((bStatus == SETUP_INPUT_PASSWORD_CORRECT) ||
        (bStatus == SETUP_INPUT_PASSWORD_CLEAR) || // Brian2.78b-2 clear when KEY_OPEN/KEY_POWER
        (bStatus == SETUP_INPUT_PASSWORD_WRONG))
    {
        if (__bSetupMode)
        {
            _MENU_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
            _OSDMENU_FillRect( DIALOG_INPUT_PASSWORD_X1, DIALOG_INPUT_PASSWORD_Y1,
            DIALOG_INPUT_PASSWORD_X2, DIALOG_INPUT_PASSWORD_Y2); //CoCo1.20, reduce code size.

           // Adjust the dialog position so there is no overlap problem, no need to redraw menu
#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG
#ifndef REMOVE_SETUP_HELP_MESSAGE
            OSDSETUP_ShowHelpText(0);
#endif
#endif
        }
        else
        {
            // normal OSD, just clear the area
            _MENU_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT; 
            _OSDMENU_FillRect(DIALOG_INPUT_PASSWORD_OSD_X1, DIALOG_INPUT_PASSWORD_OSD_Y1,
                DIALOG_INPUT_PASSWORD_OSD_X2, DIALOG_INPUT_PASSWORD_OSD_Y2); //CoCo1.20, reduce code size.
            OSD_ChangeUI(OSD_UI_PASSWORD, OSD_EXIT_UI);
        }
	__bInputPassword = FALSE;
    }

    return bStatus;
}

//CoCo1.20, reduce code size.
void _OSDMENU_DrawPassword(BYTE bPasswordType)
{
	_wMENUString[0] = SETUP_PASSWORD_NO*2-1;
	for (_bMENUTemp=1; _bMENUTemp<=SETUP_PASSWORD_NO; _bMENUTemp++)
	{
		_wMENUString[_bMENUTemp*2-1] = CHAR_LOW_LINE;
		_wMENUString[_bMENUTemp*2] = CHAR_SPACE;
	}    

    _MENU_StrAttr.wX = DIALOG_CHANGE_PASSWORD_X1+DIALOG_CHANGE_PASSWORD_X_OFFSET+DIALOG_CHANGE_PASSWORD_X_SHIFT; //CoCo1.20, reduce code size.
    switch (bPasswordType)
    {
    case SETUP_OLD_PASSWORD:
        _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET;
        break;
    case SETUP_NEW_PASSWORD:
        _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT;
        break;
    case SETUP_VERIFY_PASSWORD:
        _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT*2;
        break;
    }
    
    //GDI_DrawString_909(0, &_MENU_StrAttr, _wMENUString);
    _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
}


void OSDSETUP_ChangePasswordDlg(void)
{
    {
#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG
#ifndef REMOVE_SETUP_HELP_MESSAGE
        OSDSETUP_ShowHelpText(__wMENUCurrentItemId);
#endif
#endif
        __bInputPasswordIndex = 0;
        __bInputPassword = SETUP_OLD_PASSWORD; // Input old password
        
        
        _InitRect((PURECT)&(_MENU_FrameAttr), DIALOG_CHANGE_PASSWORD_X1,
            DIALOG_CHANGE_PASSWORD_Y1,
            DIALOG_CHANGE_PASSWORD_X2, DIALOG_CHANGE_PASSWORD_Y2);
        _OSDMENU_DrawFrame(FRAME_DOUBLE_SOLID); //CoCo1.20, reduce code size.
        
        _OSDMENU_SetMenuStringColor(); //--CoCo1.20, reduce code size.
        
        _MENU_StrAttr.wX = DIALOG_CHANGE_PASSWORD_X1+DIALOG_CHANGE_PASSWORD_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET;
        _OSDMENU_DrawDynamicString(aMsgOldPassword); //CoCo1.20, reduce code size.
        
        _OSDMENU_SetDlgStringColor(); //CoCo1.20, reduce code size.
        
        _OSDMENU_DrawPassword(SETUP_OLD_PASSWORD); //CoCo1.20, reduce code size.
        
        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
        _MENU_StrAttr.bBackground = OSDMENU_ENTRY_MENU_COLOR_CENTER;
        _MENU_StrAttr.wX = DIALOG_CHANGE_PASSWORD_X1+DIALOG_CHANGE_PASSWORD_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT;
        _OSDMENU_DrawDynamicString(aMsgNewPassword); //CoCo1.20, reduce code size.
        
        _OSDMENU_DrawPassword(SETUP_NEW_PASSWORD); //CoCo1.20, reduce code size.
        _MENU_StrAttr.wX = DIALOG_CHANGE_PASSWORD_X1+DIALOG_CHANGE_PASSWORD_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT*2;
        _OSDMENU_DrawDynamicString(aMsgVerifyPassword); //CoCo1.20, reduce code size.

        _OSDMENU_DrawPassword(SETUP_VERIFY_PASSWORD); //CoCo1.20, reduce code size.
    }

}


BYTE    OSDSETUP_ChangePassword(BYTE bKey)
{
    //BYTE bStatus = 0;

    switch (bKey)
    {
    case KEY_N0:
    case KEY_N1:
    case KEY_N2:
    case KEY_N3:
    case KEY_N4:
    case KEY_N5:
    case KEY_N6:
    case KEY_N7:
    case KEY_N8:
    case KEY_N9:
        if (__bInputPassword)
        {
            __bPassword[__bInputPasswordIndex] = bKey-KEY_N0;
            __bInputPasswordIndex++;

			//if (bStatus == SETUP_INPUT_PASSWORD_CORRECT ||  bStatus == SETUP_INPUT_PASSWORD_NORMAL)
        {
            _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
            _MENU_StrAttr.bBackground = OSDMENU_ENTRY_MENU_COLOR_CENTER;

			_wMENUString[0] = 2;
			_wMENUString[1] = CHAR_X;
			_wMENUString[2] = CHAR_SPACE;

            _MENU_StrAttr.wX = DIALOG_CHANGE_PASSWORD_X1+DIALOG_CHANGE_PASSWORD_X_OFFSET+DIALOG_CHANGE_PASSWORD_X_SHIFT+(__bInputPasswordIndex-1)*(GDI_GetCharWidth(CHAR_LOW_LINE)+GDI_GetCharWidth(CHAR_SPACE)); //CoCo1.20, reduce code size.
            switch(__bInputPassword)
            {
            case SETUP_OLD_PASSWORD:
                _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET;
                _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
                break;
            case SETUP_NEW_PASSWORD:
                _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT;
                _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
                break;
                
            case SETUP_VERIFY_PASSWORD:
                _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT*2;
                _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
                break;
                
            }
        }


            if (__bInputPasswordIndex == SETUP_PASSWORD_NO)
            {
		switch(__bInputPassword)
		{
		case SETUP_OLD_PASSWORD:
			if ((_GetPasswordNumber() == __SetupInfo.wDefaultPassword)
#ifdef  SUPPORT_GOLDEN_PASSWORD
				|| (_GetPasswordNumber() == GOLDEN_PASSWORD)
#endif
			)
			{
                bStatus = SETUP_INPUT_PASSWORD_CORRECT;
                __bInputPasswordIndex = 0;
                __bInputPassword = SETUP_NEW_PASSWORD; // to the next stage
                _OSDMENU_SetDlgStringColor(); //CoCo1.20, reduce code size.
                //DRAW_NEW_PASSWORD;
                _OSDMENU_DrawPassword(SETUP_NEW_PASSWORD); //CoCo1.20, reduce code size.
                
#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG
#ifndef REMOVE_SETUP_HELP_MESSAGE
                OSDSETUP_ShowHelpText(__wMENUCurrentItemId);
#endif
#endif
			}
			else
			{
				bStatus = SETUP_INPUT_PASSWORD_WRONG;
				__bInputPasswordIndex = 0;
			}
			break;
		case SETUP_NEW_PASSWORD:
            _wPasswordTemp = _GetPasswordNumber();
            bStatus = SETUP_INPUT_PASSWORD_CORRECT;
            __bInputPasswordIndex = 0;
            __bInputPassword = SETUP_VERIFY_PASSWORD; // to the next stage
            _OSDMENU_SetDlgStringColor(); //CoCo1.20, reduce code size.
            //DRAW_VERIFY_PASSWORD; //--CoCo1.20, reduce code size.
            _OSDMENU_DrawPassword(SETUP_VERIFY_PASSWORD); //CoCo1.20, reduce code size.
#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG
#ifndef REMOVE_SETUP_HELP_MESSAGE
            OSDSETUP_ShowHelpText(__wMENUCurrentItemId);
#endif
#endif
			break;
		case SETUP_VERIFY_PASSWORD:
			if (_GetPasswordNumber() == _wPasswordTemp)
			{
				bStatus = SETUP_INPUT_PASSWORD_VERIFIED;
				__SetupInfo.wDefaultPassword = _wPasswordTemp;
			}
			else
			{
				bStatus = SETUP_INPUT_PASSWORD_WRONG;
				__bInputPasswordIndex = 0;
			}
			break;

		} // end of switch

            }
            else
                bStatus = SETUP_INPUT_PASSWORD_NORMAL;
        }
        else
            bStatus = SETUP_INPUT_PASSWORD_INVALID_KEY;
        break;
    case KEY_SETUP:
    case KEY_LEFT: // Brian0.91, key left also exit
        __bInputPasswordIndex = 0;
        __bInputPassword = FALSE;
        bStatus = SETUP_INPUT_PASSWORD_CLEAR;
        break;
    default:
        bStatus = SETUP_INPUT_PASSWORD_INVALID_KEY;
        break;
    }

    if (bStatus == SETUP_INPUT_PASSWORD_INVALID_KEY)
    {
    }
    else if (bStatus == SETUP_INPUT_PASSWORD_WRONG)
    {
        
        switch(__bInputPassword)
        {
        case SETUP_OLD_PASSWORD:
            _MENU_StrAttr.wX = DIALOG_CHANGE_PASSWORD_X1+DIALOG_CHANGE_PASSWORD_X_OFFSET+DIALOG_CHANGE_PASSWORD_X_SHIFT;
            _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET;
            _OSDMENU_DrawDynamicString(aMsgWrongPassword); //CoCo1.20, reduce code size.
            OS_DelayTime(COUNT_1_SEC); //CoCo1.20, reduce code size
            // clear the Password Error message
            _MENU_RectAttr.bColor = OSDMENU_ENTRY_MENU_COLOR_CENTER;
            _OSDMENU_FillRect(DIALOG_CHANGE_PASSWORD_X1+DIALOG_CHANGE_PASSWORD_X_OFFSET+DIALOG_CHANGE_PASSWORD_X_SHIFT, 
                DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET,
                DIALOG_CHANGE_PASSWORD_X2-8,
                DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT-1); //CoCo1.20, reduce code size.
            
            _OSDMENU_SetDlgStringColor(); //CoCo1.20, reduce code size.
            //DRAW_OLD_PASSWORD;
            _OSDMENU_DrawPassword(SETUP_OLD_PASSWORD); //CoCo1.20, reduce code size.
            break;
        case SETUP_VERIFY_PASSWORD:
            _MENU_StrAttr.wX = DIALOG_CHANGE_PASSWORD_X1+DIALOG_CHANGE_PASSWORD_X_OFFSET+DIALOG_CHANGE_PASSWORD_X_SHIFT;
            _MENU_StrAttr.wY = DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT*2;
            _OSDMENU_DrawDynamicString(aMsgWrongPassword); //CoCo1.20, reduce code size.
            OS_DelayTime(COUNT_1_SEC); //CoCo1.20, reduce code size
            // clear the Password Error message
            _MENU_RectAttr.bColor = OSDMENU_ENTRY_MENU_COLOR_CENTER;
            _OSDMENU_FillRect(DIALOG_CHANGE_PASSWORD_X1+DIALOG_CHANGE_PASSWORD_X_OFFSET+DIALOG_CHANGE_PASSWORD_X_SHIFT,
                DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT*2,
                DIALOG_CHANGE_PASSWORD_X2-8,
                DIALOG_CHANGE_PASSWORD_Y1+DIALOG_CHANGE_PASSWORD_Y_OFFSET+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT*2+DIALOG_CHANGE_PASSWORD_CHAR_HEIGHT-1); //CoCo1.20, reduce code size.

            _OSDMENU_SetDlgStringColor(); //CoCo1.20, reduce code size.
            //DRAW_VERIFY_PASSWORD; //--CoCo1.20, reduce code size.
            _OSDMENU_DrawPassword(SETUP_VERIFY_PASSWORD); //CoCo1.20, reduce code size.
            break;            
        }	
    }
    else
    {
        
    }
    
    if ((bStatus == SETUP_INPUT_PASSWORD_VERIFIED) ||
        (bStatus == SETUP_INPUT_PASSWORD_CLEAR) )
    {
        
        // can't do bitmap copy 
        // recover the Setup UI by re-draw
        _MENU_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
        _OSDMENU_FillRect(DIALOG_CHANGE_PASSWORD_X1,
            DIALOG_CHANGE_PASSWORD_Y1,
            DIALOG_CHANGE_PASSWORD_X2, DIALOG_CHANGE_PASSWORD_Y2); //CoCo1.20, reduce code size.
        // Adjust the dialog position so there is no overlap problem, no need to redraw menu
        
        // Here I set the text color back to normal
        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
        _MENU_StrAttr.bBackground = OSDMENU_ENTRY_MENU_COLOR_CENTER;
        
        __bInputPassword = FALSE;
#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG
#ifndef REMOVE_SETUP_HELP_MESSAGE
        OSDSETUP_ShowHelpText(0);
#endif
#endif
    }
    
    return bStatus;
}

void OSDSETUP_LanguageCodeDlg(void)
{
#define LANGUAGE_CODE_NO	4
    {
        __bInputPasswordIndex = 0;
        __bInputPassword = TRUE;
#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG
#ifndef REMOVE_SETUP_HELP_MESSAGE
        OSDSETUP_ShowHelpText(__wMENUCurrentItemId);
#endif
#endif	
        _wMENUString[0] = LANGUAGE_CODE_NO*2-1;
        
        for (_bMENUTemp=1; _bMENUTemp<=LANGUAGE_CODE_NO; _bMENUTemp++)
        {
            _wMENUString[_bMENUTemp*2-1] = CHAR_LOW_LINE;
            _wMENUString[_bMENUTemp*2] = CHAR_SPACE;
        }
        
        _InitRect((PURECT)&(_MENU_FrameAttr), DIALOG_INPUT_PASSWORD_X1,
            DIALOG_INPUT_PASSWORD_Y1,
            DIALOG_INPUT_PASSWORD_X2, DIALOG_INPUT_PASSWORD_Y2);
        _OSDMENU_DrawFrame(FRAME_DOUBLE_SOLID); //CoCo1.20, reduce code size.
        
        _OSDMENU_SetMenuStringColor(); //--CoCo1.20, reduce code size.
        
        _MENU_StrAttr.wX = DIALOG_INPUT_PASSWORD_X1+DIALOG_INPUT_PASSWORD_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PASSWORD_Y1+DIALOG_INPUT_PASSWORD_Y_OFFSET;
        _OSDMENU_DrawString(); //CoCo1.20, reduce code size.
    }
}


BYTE OSDSETUP_InputLanguageCode(BYTE bKey)
{
    switch (bKey)
    {
    case KEY_N0:
    case KEY_N1:
    case KEY_N2:
    case KEY_N3:
    case KEY_N4:
    case KEY_N5:
    case KEY_N6:
    case KEY_N7:
    case KEY_N8:
    case KEY_N9:
        if (__bInputPassword)
        {
            __bLanguageCode[__bInputPasswordIndex] = bKey-KEY_N0;
            __bInputPasswordIndex++;

            if (__bInputPasswordIndex == LANGUAGE_CODE_NO)
            {
                    bStatus = SETUP_INPUT_PASSWORD_CORRECT;
            }
            else
                bStatus = SETUP_INPUT_PASSWORD_NORMAL;
        }
        else
            bStatus = SETUP_INPUT_PASSWORD_INVALID_KEY;
        break;
    case KEY_OPEN_CLOSE:
    case KEY_POWER:
	case KEY_SETUP:
	case KEY_LEFT:
        __bInputPasswordIndex = 0;
        __bInputPassword = FALSE;
        bStatus = SETUP_INPUT_PASSWORD_CLEAR;
        break;
    default:
        bStatus = SETUP_INPUT_PASSWORD_INVALID_KEY;
        break;
    }

    if (bStatus == SETUP_INPUT_PASSWORD_INVALID_KEY)
    {
    }
    else if (bStatus == SETUP_INPUT_PASSWORD_WRONG)
    {

    }
    else
    {
		if (bStatus != SETUP_INPUT_PASSWORD_CLEAR)
        {
            OSDUTL_PrepareOutputString();
            OSDUTL_OutputNumFromIndex(bKey-KEY_N0); // Brian1.06

        _MENU_StrAttr.wX = DIALOG_INPUT_PASSWORD_X1+DIALOG_INPUT_PASSWORD_X_OFFSET+(__bInputPasswordIndex-1)*(GDI_GetCharWidth(CHAR_LOW_LINE)+GDI_GetCharWidth(CHAR_SPACE));
        _MENU_StrAttr.wY = DIALOG_INPUT_PASSWORD_Y1+DIALOG_INPUT_PASSWORD_Y_OFFSET;
        _OSDMENU_DrawDynamicString(__wOSDUTLStringBuf); //CoCo1.20, reduce code size.
        }
    }

    if ((bStatus == SETUP_INPUT_PASSWORD_CORRECT) ||
        (bStatus == SETUP_INPUT_PASSWORD_CLEAR) || // Brian2.78b-2 clear when KEY_OPEN/KEY_POWER
        (bStatus == SETUP_INPUT_PASSWORD_WRONG))
    {

// recover the Setup UI by re-draw
    _MENU_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
    _OSDMENU_FillRect(DIALOG_INPUT_PASSWORD_X1, DIALOG_INPUT_PASSWORD_Y1,
              DIALOG_INPUT_PASSWORD_X2, DIALOG_INPUT_PASSWORD_Y2); //CoCo1.20, reduce code size.

 		__bInputPassword = FALSE;
#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG
#ifndef REMOVE_SETUP_HELP_MESSAGE
		OSDSETUP_ShowHelpText(0);
#endif
#endif
    }

    return bStatus;
}


//  ***************************************************************************
//  Function    :   OSDMENU_SetItemEnable
//  Description :   This function will show input password menu to let user input
//              :   password.
//  Arguments   :   none
//  Global Var  :   _bMENUTemp  : Menu ID
//				:   _wMENUTemp	: Item ID
//				:	_bMENUTemp2 : TRUE / FALSE
//  Return      :   SETUP_INPUT_PASSWORD_CORRECT, SETUP_INPUT_PASSWORD_INVALID,
//              :   SETUP_INPUT_PASSWORD_NORMAL, SETUP_INPUT_PASSWORD_INVALID_KEY.
//  Side Effect :
//  ***************************************************************************

void OSDMENU_SetItemEnable(void)
{
    // Brian1.08, 
    if (_bMENUTemp == SETUP_MENU_AUDIO)
    {
        if (__dwSupportFeature & SUPPORT_FEATURE_PROLOGIC)
        {
            // it's AD~ version, and really support ProLogic
            _bMENUTemp = SETUP_MENU_AUDIO_WITH_PROLOGIC;
        }

    }

	_pTempMenu = _pMenuIndex[_bMENUTemp];
    // Brian1.08, search the menu to get item position
    _bMENUTemp = _GetMenuItemPosition(_wMENUTemp);

    if (_bMENUTemp >= GET_MENU_ITEM_NUM(_pTempMenu))
    {
        // Out of range. This could happen when support 2 channel only, there is no REAR/Center/SW
        // but Setup still set enable/disable these items
        return;
    }

	if(GET_ITEM_ATTRIBUTE(_pTempMenu, _bMENUTemp) == IS_ITEM) // double check, make sure it is an item
	{
		_wMENUTemp = 1 << (_bMENUTemp%8);
		if (!_bMENUTemp2) // set Disable
		{
			_bMENUTemp2 = ~(BYTE)(_wMENUTemp);
            *(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) = *(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) & _bMENUTemp2;
		}
        else // Brian1.05 fix bug
        {
            _bMENUTemp2 = _wMENUTemp;
		    *(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) = *(GET_MENU_ENABLE(_pTempMenu)+_bMENUTemp/8) | _bMENUTemp2;
        }
	}
	else
	{
		// Error
#ifdef SUPPORT_PRINTF
		printf("OSDMENU_SetItemEnable Error\n");
#endif 
	}


}

#ifndef REMOVE_SETUP_HELP_MESSAGE

void OSDSETUP_ShowHelpText(BYTE bID)
{

    _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_DESC_TEXT_COLOR;
    _MENU_StrAttr.bBackground = OSDSETUP_ENTRY_HELP_COLOR_BG;
    _MENU_StrAttr.bShadow1 = OSDSETUP_ENTRY_HELP_COLOR_BG;
    _MENU_StrAttr.bShadow2 = OSDSETUP_ENTRY_HELP_COLOR_BG;

#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG

        _MENU_RectAttr.bColor = OSDSETUP_ENTRY_HELP_COLOR_BG;
        _OSDMENU_FillRect(24, 364, 591, 396);  //CoCo1.20, reduce code size.

        switch(bID)
        {
        case SETUP_CUSTOM_PARENTAL:
            if (!__bSetupMode)//wendy0316
                return;
            _MENU_StrAttr.wX = 28;
            _MENU_StrAttr.wY = 364;
            _OSDMENU_DrawDynamicString(StrHelpPwd.pbStr); //CoCo1.20, reduce code size.
            break;
        case SETUP_LANGUAGE_MENU_OTHERS:
        case SETUP_LANGUAGE_AUDIO_OTHERS:
        case SETUP_LANGUAGE_SP_OTHERS:
            _MENU_StrAttr.wX = 28;
            _MENU_StrAttr.wY = 364;
            _OSDMENU_DrawDynamicString(StrHelpLanguageCode.pbStr); //CoCo1.20, reduce code size.
            break;
        case SETUP_CUSTOM_CHANGE_PASSWORD:
            {
                switch(__bInputPassword)
                {
                case SETUP_OLD_PASSWORD:
                default:
                    _MENU_StrAttr.wX = 28;
                    _MENU_StrAttr.wY = 364;
                    _OSDMENU_DrawDynamicString(StrHelpOldPwd.pbStr); //CoCo1.20, reduce code size.
                    break;
                case SETUP_NEW_PASSWORD:
                    _MENU_StrAttr.wX = 28;
                    _MENU_StrAttr.wY = 364;
                    _OSDMENU_DrawDynamicString(StrHelpNewPwd.pbStr); //CoCo1.20, reduce code size.
                    break;
                    
                case SETUP_VERIFY_PASSWORD:
                    _MENU_StrAttr.wX = 28;
                    _MENU_StrAttr.wY = 364;
                    _OSDMENU_DrawDynamicString(StrHelpVerifyPwd.pbStr); //CoCo1.20, reduce code size.
                    break;
                }
                
            }
            break;
        default:
            _MENU_StrAttr.wX = 28;
            _MENU_StrAttr.wY = 364;
            _OSDMENU_DrawDynamicString(StrHelp1.pbStr); //CoCo1.20, reduce code size.
            break;
        }
        
        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
#else
        _MENU_StrAttr.wX = 28;
        _MENU_StrAttr.wY = 364;
        _OSDMENU_DrawDynamicString(aMsgHelp1_E); //CoCo1.20, reduce code size.
#endif


}

#endif

void OSDSETUP_RedrawMenu(void)
{
	OSDSETUP_ShowDescriptionText();
#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG
#ifndef REMOVE_SETUP_HELP_MESSAGE
	OSDSETUP_ShowHelpText(__wMENUCurrentItemId);
#endif
#endif


#ifdef SETUP_DIALOG_RECOVER_BY_REDRAW
    // Brian1.10, the scroll up/down mark area may be cleared by (PScan) dialog.
    // We need to clear the scroll bit, so the scroll up/down mark can be redraw by _DisplayMenu fucntion

    __bMENUStatus[1] &= 0xFE; // clear scroll-up bit
    __bMENUStatus[1] &= 0xFD; // clear scroll-down bit
    __bMENUStatus[2] &= 0xFE; // clear scroll-up bit
    __bMENUStatus[2] &= 0xFD; // clear scroll-down bit
#endif

    // Brian1.08a
    if (__bMENUCurrentLevel == 2)
    {
        // for the case that current menu is the value menu, we still hope its parent menu be re-draw
        OSDMENU_DisplayMenu(__bMENUParent[__bMENUCurrentLevel], 0xFF, 1);
    }
	OSDMENU_DisplayMenu(__bCurrentMenuId, 0xFF, __bMENUCurrentLevel);

    _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
    OSDMENU_DrawItem(__bMENUDisplayStart[__bMENUCurrentLevel],
                     __bMENUSelect[__bMENUCurrentLevel],
                     OSDMENU_ENTRY_ITEM_COLOR_HIGHLIGHT);

    // Brian2.36, fix scroll-up/down symnol is not cleared when OSD->English
    // change to OSD ->Spanish
    _pTempMenu2 = _pTempMenu;
    _pTempMenu = GET_SUB_MENU(_pTempMenu, __bMENUSelect[__bMENUCurrentLevel]);
    if (_pTempMenu)
    {
        OSDMENU_ClearMenu();
    }
    _pTempMenu = _pTempMenu2;


    OSDMENU_DisplaySubMenu(__bMENUSelect[__bMENUCurrentLevel],
                           __bMENUCurrentLevel);
	
}




#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)
//  ***************************************************************************
//  Function    :   OSDPROMPT_Trigger
//  Description :   In OSD_PROMPT_MODE , every 1 second trigger to show left time
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :
//  ***************************************************************************
void OSDPROMPT_Trigger(void)
{
    //Alex1.07a 20031013 , modify to avoid pscan mode and setup mode appear question.   
#ifndef PSCAN_PROMPT_NO_TIME_OUT // Brian1.08
    if (  __bOSDPromptMode) 
    {
        //Alex1.07a 20031009 every 60 count about 1 second
        if ( (OSD_PSCAN_PROMPT_WAIT_TIME- __bOSDPromptWaitTime ) * COUNT_1_SEC < (OS_GetSysTimer()-__dwOSDPromptInitialTime) )   
        {
            __bOSDPromptWaitTime--;

            OSDUTL_PrepareOutputString();
            OSDUTL_OutputNumFromIndex((WORD)__bOSDPromptWaitTime);
            
            // Brian1.10, Use __bSetupMode to decide the color index
           _MENU_StrAttr.bBackground = OSDMENU_ENTRY_MENU_COLOR_CENTER; //CoCo1.20, reduce code size.
            if (__bSetupMode)
            {
                _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_MENU_COLOR_DARK;
            }
            else
            {
                _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
            }

            _MENU_StrAttr.wX = (DIALOG_INPUT_PSCAN_X1+DIALOG_INPUT_PSCAN_X2) /2 + 0 * DIALOG_INPUT_PSCAN_X_OFFSET;
            _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1 + 6 * DIALOG_INPUT_PSCAN_Y_OFFSET;
            _OSDMENU_DrawDynamicString(__wOSDUTLStringBuf); //CoCo1.20, reduce code size.

            //Alex1.07a 20031009 if the waittime < 0 , stop the pscan
            if(__bOSDPromptWaitTime <= 0)
                OSDPROMPT_ProcessKey(PSCAN_PROMPT_DEFAULT_KEY);
        } //if  (OSD_PSCAN_PROMPT_WAIT_TIME- __bOSDPromptWaitTime ) * 60 - __dwOSDPromptInitialTime <__dwCountSystem )   
    } //if (  __bOSDPromptMode &&  __bInputPassword) 
#endif    
    

   
}//void OSDPROMPT_Trigger(void)


//  ***************************************************************************
//  Function    :   OSDPROMPT_ProcessKey
//  Description :   1.Recieve the key determine if situaion is ODSPROMPT_mode ?,
//                  2.
//  Arguments   :   bkey.
//  Return      :   bkey.
//  Side Effect :
//  ***************************************************************************

BYTE OSDPROMPT_ProcessKey(BYTE bKey)
{

    if (  (__bSetupMode==TRUE)  && (__bOSDPromptWaitTime==OSD_PSCAN_PROMPT_WAIT_TIME ) )
        return bKey;

    // If not in OSDPrompt now, and Key is not PSCAN PROMPT
    // just return, process as normal keys
    if ((__bOSDPromptMode == FALSE) &&
        (bKey!=KEY_PSCAN  ) && 
        (bKey!=KEY_VIDEO_OUTPUT_CTL) 
        )
    {
        return bKey; 
    }

    if ((bKey == KEY_POWER))
    {   
        //clear highlight
        __bOSDPromptMode = FALSE;
        __bInputPassword = FALSE;
        return bKey; //TRUE;
    }   
    // Brian1.08, fix the condition
    __bTemp = __SetupInfo.bVideoOutput+1;
    if (__bTemp > SETUP_SCREEN_VIDEO_OUTPUT_END)
        __bTemp = SETUP_SCREEN_VIDEO_OUTPUT_BEGIN;


    //Alex1.07a,20031013  change __bInputPassword -> __bOSDPromptMode to avoid , setup and pscan mistakes
    //Brian1.08, fix the condition
    if (__bOSDPromptMode==FALSE)
    {
        if ( (bKey==KEY_PSCAN && __SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_OFF) ||
         (bKey==KEY_VIDEO_OUTPUT_CTL && __SetupInfo.bProgressiveScan != SETUP_PROGRESSIVE_SCAN_ON &&
         (__bTemp == SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN || 
          __bTemp == SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN ||
          __bTemp == SETUP_SCREEN_VIDEO_OUTPUT_VGA)) )
        {
            __bOSDPromptPreKey=bKey;
	    OSDSETUP_ScreenVideoOutputPscanDlg();                

            //Alex1.07a 20031009 do pause command in OSD_PROMPT,                 
            if (__btPlaying)
            {
#ifdef  PLAY_AFTER_EXIT_SETUP
                // this for none-DVD
                // if it is already in play mode, don't need to issue KEY_PLAY again
                if (__bModeCmd != KEY_PLAY)
                    CC_DoKeyAction (KEY_PLAY);
#endif
                CC_DoKeyAction (KEY_PAUSE);
            }//if (__btPlaying)
        }
        else
        {
            if (bKey==KEY_PSCAN) // && __SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON) 
            {
                if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_YUV)
                        {
                            __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN;
                        }
                else if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN)
                        {
                            __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_YUV;
                        }
                else if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN)
                        {
                            __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_RGB;
                        }
                else if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_RGB)
                        {
                            __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN;
                        }

                        SETUP_ChangeValue(SETUP_SCREEN_VIDEO_OUTPUT);
                        // P-SCAN is integrated into Video Output
                        // SETUP_ChangeValue will set __SetupInfo.bProgressiveScan according to __bSetupInfo.VideoOutput
                        SETUP_ChangeValue(SETUP_SCREEN_PROGRESSIVE_SCAN);
                        OSD_OUTPUT_MACRO(MSG_PSCAN, (WORD)NULL, 2);
                        SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);
            }
            else if (bKey==KEY_VIDEO_OUTPUT_CTL)
            {
                __SetupInfo.bVideoOutput++;
                if (__SetupInfo.bVideoOutput > SETUP_SCREEN_VIDEO_OUTPUT_END)
                    __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_BEGIN;
                    
                SETUP_ChangeValue(SETUP_SCREEN_VIDEO_OUTPUT);                
                SETUP_ChangeValue(SETUP_SCREEN_PROGRESSIVE_SCAN);
                SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);                                               
//alan080, remove OSD_Output(MSG_VIDEO_OUTPUT, __SetupInfo.bVideoOutput, 0x3);    
            }
        }
    }
    //Alex1.07a, change __bInputPassword -> __bOSDPromptMode to avoid , setup and pscan mistakes
    else if ( __bOSDPromptMode )
	{
            extern BYTE _bMENUTemp;	
                
            _bMENUTemp = OSDSETUP_InputPscan(bKey);
            if (_bMENUTemp ==SETUP_INPUT_PASSWORD_CORRECT )
            {
                __bOSDPromptMode=FALSE;
            if (!__bSetupMode) 
                    CC_DoKeyAction (KEY_PLAY);
                if  (__bOSDPromptPreKey==  KEY_VIDEO_OUTPUT_CTL)
                {
                    __SetupInfo.bVideoOutput++;
                    if (__SetupInfo.bVideoOutput > SETUP_SCREEN_VIDEO_OUTPUT_END)
                        __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_BEGIN;
                    
                    SETUP_ChangeValue(SETUP_SCREEN_VIDEO_OUTPUT);                
                    SETUP_ChangeValue(SETUP_SCREEN_PROGRESSIVE_SCAN);
                    SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);                                               
//alan080, remove   OSD_Output(MSG_VIDEO_OUTPUT, __SetupInfo.bVideoOutput, 0x3);    
                }
                else if  (__bOSDPromptPreKey==  KEY_PSCAN)
                {
                    if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_YUV)
                    {
                        __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN;
                    }
                else if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_YUV_PSCAN)
                    {
                        __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_YUV;
                    }
                else if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN)
                    {
                        __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_RGB;
                    }
                else if (__SetupInfo.bVideoOutput == SETUP_SCREEN_VIDEO_OUTPUT_RGB)
                    {
                        __SetupInfo.bVideoOutput = SETUP_SCREEN_VIDEO_OUTPUT_RGB_PSCAN;
                    }
                    SETUP_ChangeValue(SETUP_SCREEN_VIDEO_OUTPUT);
                    // P-SCAN is integrated into Video Output
                    // SETUP_ChangeValue will set __SetupInfo.bProgressiveScan according to __bSetupInfo.VideoOutput
                    SETUP_ChangeValue(SETUP_SCREEN_PROGRESSIVE_SCAN);
                    OSD_OUTPUT_MACRO(MSG_PSCAN, (WORD)NULL, 2);
                    SETUP_AdjustSetting(SETUP_SCREEN_VIDEO_OUTPUT);

                }

            } 
            else if (_bMENUTemp ==SETUP_INPUT_PASSWORD_WRONG)
            {
                __bOSDPromptMode=FALSE;
                if (!__bSetupMode) // Brian1.07a
                    CC_DoKeyAction (KEY_PLAY);
            }
                 
	} 

    //Alex1.07a, change __bInputPassword -> __bOSDPromptMode to avoid , setup and pscan mistakes
    if (__bOSDPromptMode) 
    {
        return KEY_NO_KEY;   
    }
    
    return KEY_NO_KEY ;
}//BYTE OSDPROMPT_ProcessKey(BYTE bKey)


//  ***************************************************************************
//  Function    :   OSDSETUP_ScreenVideoOutputPscanDlg
//  Description :   This function will show "Please Make sure your TV support Pscan" prompt
//              :   to let user input
//              :   password.
//  Arguments   :   none
//  Global Var  :   __bOSDPromptMode
//				:   __bInputPassword
//				:	__dwOSDPromptInitialTime
//              :    __bOSDPromptWaitTime
//  Return      :   SETUP_INPUT_PASSWORD_CORRECT, SETUP_INPUT_PASSWORD_INVALID,
//              :   SETUP_INPUT_PASSWORD_WRONG, 
//  Side Effect :
//  ***************************************************************************

void OSDSETUP_ScreenVideoOutputPscanDlg(void)
{

#define LANGUAGE_CODE_NO	4

	__bInputPasswordIndex = 0;
        __bInputPassword = TRUE;
        __bOSDPromptMode=TRUE;

/*
        __dwOSDPromptInitialTime=UTL_GetSysTimer();        
        __bOSDPromptWaitTime=OSD_PSCAN_PROMPT_WAIT_TIME;
        OSDCOMDLG_Register_TimeoutFunc(NULL);
        OSDCOMDLG_Set_WaitingTime(OSD_PSCAN_PROMPT_WAIT_TIME);
*/

        OSDCOMDLG_SetRect(DIALOG_INPUT_PSCAN_X1, DIALOG_INPUT_PSCAN_Y1, DIALOG_INPUT_PSCAN_X2, DIALOG_INPUT_PSCAN_Y2);
        OSDCOMDLG_SetButtonNum(2);
        OSDCOMDLG_Register_CallbackFunc(_PScan_Callback);
        OSDCOMDLG_Set_Buttons_Type(FALSE);
        OSDCOMDLG_SetMessage(aMsgPleaseMake);
        OSDCOMDLG_DrawDialog();
//        OSDCOMDLG_DrawString(50, 10, StrPleaseMake.pbStr[_bOSDLang]);
/*
        OSDCOMDLG_DrawString(100, 10, StrSureYour.pbStr[_bOSDLang]);
        OSDCOMDLG_DrawString(150, 10, StrTVSupport.pbStr[_bOSDLang]);
        OSDCOMDLG_DrawString(200, 10, StrPScan.pbStr[_bOSDLang]);
*/
        OSDCOMDLG_Select_OK_YES_Button();

/*
        // Brian1.10, Use __bSetupMode to decide the color index

        _InitRect((PURECT)&(_MENU_FrameAttr), DIALOG_INPUT_PSCAN_X1, DIALOG_INPUT_PSCAN_Y1,
                  DIALOG_INPUT_PSCAN_X2, DIALOG_INPUT_PSCAN_Y2);
        if (__bSetupMode)
        {
            _MENU_FrameAttr.bColorBright = OSDMENU_ENTRY_MENU_COLOR_BRIGHT;
            _MENU_FrameAttr.bColorDark = OSDMENU_ENTRY_MENU_COLOR_DARK;
            _MENU_FrameAttr.bColorCenter = OSDMENU_ENTRY_MENU_COLOR_CENTER;
        }
        else
        {
            _MENU_FrameAttr.bColorBright = GDI_ENTRY_4B_COLOR_BRIGHT;
            _MENU_FrameAttr.bColorDark = GDI_ENTRY_4B_COLOR_DARK;
            _MENU_FrameAttr.bColorCenter = GDI_ENTRY_4B_COLOR_CENTER;
        }

        _MENU_FrameAttr.bStyle = FRAME_DOUBLE_SOLID;
        _MENU_FrameAttr.bThickness = 2;
        GDI_DrawFrame_909(0, &_MENU_FrameAttr);

        if (__bSetupMode)
        {
            _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_MENU_COLOR_DARK;
            _MENU_StrAttr.bBackground = OSDMENU_ENTRY_MENU_COLOR_CENTER;
        } 
        else
        {
            _MENU_StrAttr.bTextColor = GDI_ENTRY_4B_COLOR_DARK;
            _MENU_StrAttr.bBackground = GDI_ENTRY_4B_COLOR_CENTER;
        } 

        _MENU_StrAttr.wX = DIALOG_INPUT_PSCAN_X1+1 * DIALOG_INPUT_PSCAN_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1+ 1* DIALOG_INPUT_PSCAN_Y_OFFSET;
        GDI_DrawString_909(0, &_MENU_StrAttr, StrPleaseMake.pbStr[_bOSDLang]);

        _MENU_StrAttr.wX = DIALOG_INPUT_PSCAN_X1+7 *DIALOG_INPUT_PSCAN_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1+ 1* DIALOG_INPUT_PSCAN_Y_OFFSET;
        GDI_DrawString_909(0, &_MENU_StrAttr, StrSureYour.pbStr[_bOSDLang]);

        _MENU_StrAttr.wX = DIALOG_INPUT_PSCAN_X1+1 *DIALOG_INPUT_PSCAN_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1+ 2* DIALOG_INPUT_PSCAN_Y_OFFSET;
        GDI_DrawString_909(0, &_MENU_StrAttr, StrTVSupport.pbStr[_bOSDLang]);

        _MENU_StrAttr.wX = DIALOG_INPUT_PSCAN_X1+7 *DIALOG_INPUT_PSCAN_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1+ 2* DIALOG_INPUT_PSCAN_Y_OFFSET;
        GDI_DrawString_909(0, &_MENU_StrAttr, StrPScan.pbStr[_bOSDLang]);

        _MENU_StrAttr.wX = DIALOG_INPUT_PSCAN_X1+1.5 *DIALOG_INPUT_PSCAN_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1+ 3* DIALOG_INPUT_PSCAN_Y_OFFSET;
        GDI_DrawString_909(0, &_MENU_StrAttr, StrContinue.pbStr[_bOSDLang]);

        _MENU_StrAttr.wX = DIALOG_INPUT_PSCAN_X1+6.5 *DIALOG_INPUT_PSCAN_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1+ 3* DIALOG_INPUT_PSCAN_Y_OFFSET;
        GDI_DrawString_909(0, &_MENU_StrAttr, StrPressENTER.pbStr[_bOSDLang]);

        _MENU_StrAttr.wX = DIALOG_INPUT_PSCAN_X1+1.5 *DIALOG_INPUT_PSCAN_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1+ 4* DIALOG_INPUT_PSCAN_Y_OFFSET;
        GDI_DrawString_909(0, &_MENU_StrAttr, StrStop.pbStr[_bOSDLang]);

        _MENU_StrAttr.wX = DIALOG_INPUT_PSCAN_X1+6.5 *DIALOG_INPUT_PSCAN_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1+ 4* DIALOG_INPUT_PSCAN_Y_OFFSET;
        GDI_DrawString_909(0, &_MENU_StrAttr, StrPressSTOP.pbStr[_bOSDLang]);
*/

} //void OSDSETUP_ScreenVideoOutputPscanDlg(void)


//  ***************************************************************************
//  Function    :   OSDSETUP_InputPscan
//  Description :   This function will show PSCAN prompt menu to let user input "ENTER / STOP"
//  Arguments   :   none
//  Global Var  :   _bMENUTemp  : Menu ID
//				:   _wMENUTemp	: Item ID
//				:	_bMENUTemp2 : TRUE / FALSE
//  Return      :   SETUP_INPUT_PASSWORD_CORRECT, 
//              :   SETUP_INPUT_PASSWORD_WRONG, SETUP_INPUT_PASSWORD_INVALID_KEY.
//  Side Effect :
//  ***************************************************************************

BYTE OSDSETUP_InputPscan(BYTE bKey)
{
    BYTE bStatus = 0;

    //Alex1.07a 20031009 in OSDPROMPT mode, 
    // KEY=ACITVE or ENTER or PLAY-> CORRECT ,
    // KEY=STOP                   -> WRONG   ,

    bStatus = SETUP_INPUT_PASSWORD_INVALID_KEY;
    //Alex1.07a, change __bInputPassword -> __bOSDPromptMode to avoid , setup and pscan mistakes
    if (__bOSDPromptMode)
    {
//        if (KEY_TODO == OSDCOMDLG_ProcessKey(bKey))
        if (KEY_TODO == OSDCOMDLG_ProcessKey())
        {
            if (OSDCOMDLG_Get_Select_Button())
                bStatus = SETUP_INPUT_PASSWORD_CORRECT;
            else
                bStatus = SETUP_INPUT_PASSWORD_WRONG;
        }
/*
        switch (bKey)
        {
            case KEY_ACTIVE:
            case KEY_ENTER:                
            case KEY_PLAY:                
                bStatus = SETUP_INPUT_PASSWORD_CORRECT;
                break;
            case KEY_STOP: 
                bStatus = SETUP_INPUT_PASSWORD_WRONG;
                break;
        
         }//switch (bKey)
*/
    }//if (__bInputPassword)
   
/*
    //Alex1.07a 20031009 in OSDPROMPT mode, 
    // CORRECT SHOW CONTINUE
    // WRONG   SHOW STOP
    if (bStatus == SETUP_INPUT_PASSWORD_WRONG)
    {        
        _MENU_StrAttr.wX = DIALOG_INPUT_PSCAN_X1+ 6 *DIALOG_INPUT_PSCAN_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1+ 5* DIALOG_INPUT_PSCAN_Y_OFFSET;
        GDI_DrawString_909(0, &_MENU_StrAttr, StrStop.pbStr[_bOSDLang]);
    }
    else if (bStatus == SETUP_INPUT_PASSWORD_CORRECT )
    {
        _MENU_StrAttr.wX = DIALOG_INPUT_PSCAN_X1+ 5 *DIALOG_INPUT_PSCAN_X_OFFSET;
        _MENU_StrAttr.wY = DIALOG_INPUT_PSCAN_Y1+ 5* DIALOG_INPUT_PSCAN_Y_OFFSET;
        GDI_DrawString_909(0, &_MENU_StrAttr, StrContinue.pbStr[_bOSDLang]);
    }
*/

    if (bStatus != SETUP_INPUT_PASSWORD_INVALID_KEY) 
    {
/*
        if (__bSetupMode)
        {
            // 32M SDRAM case, recover the Setup UI by re-draw
            _InitRect(&(_MENU_RectAttr.rect), DIALOG_INPUT_PSCAN_X1,
                      DIALOG_INPUT_PSCAN_Y1,
                      DIALOG_INPUT_PSCAN_X2, DIALOG_INPUT_PSCAN_Y2);
            _MENU_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
            GDI_FillRect_909(0, &_MENU_RectAttr);
            // Adjust the dialog position so there is no overlap problem, no need to redraw menu
            OSDSETUP_RedrawMenu();
        }
*/

       // save the value when accept (time out accept or user confirm)
       if  (__bSetupMode &  (bStatus == SETUP_INPUT_PASSWORD_CORRECT ))
       {
            OSDMENU_MenuOperation(KEY_ENTER);                
            _SaveDataToSetupInfo();      
       }

       __bInputPassword = FALSE;
       __bOSDPromptMode=FALSE;

//#ifdef SUPPORT_SETUP_MULTI_LANGUAGE_HELP_MSG
//       if (__bSetupMode) // Brian1.10
//		OSDSETUP_ShowHelpText(0);
//#endif
    }

    __bKey = KEY_NO_KEY;

    return bStatus;
}//BYTE OSDSETUP_InputPscan(BYTE bKey)


#endif //#if defined (SUPPORT_PSCAN_PROMPT_SETUP) || defined (SUPPORT_PSCAN_PROMPT_OSD)


// Brian1.08
// To support runtime change menu items (for AC/AD chips, Audio menu is different)
// The menu item ID is no long sequential, so we can't use (ItemId - MenuBase) to get the position
// Instead, we need to search in the menu (structure) to determine the position
//  ***************************************************************************
//  Function    :   _GetMenuItemPosition
//  Description :   This function returns the item's position in "Menu"
//  Arguments   :   bItemId     : the menu ID
//  Global Var  :   _pTempMenu  : points to the menu
//  Return      :   the item's position in "Menu" 
//  Side Effect :
//  ***************************************************************************
BYTE _GetMenuItemPosition(BYTE bItemId)
{
    for (_wPasswordTemp=0; _wPasswordTemp <GET_MENU_ITEM_NUM(_pTempMenu);  _wPasswordTemp++)
    {
        if (GET_ITEM_ID(_pTempMenu, _wPasswordTemp) == bItemId)
            break;
    }

    return (BYTE)_wPasswordTemp;

}

// Brian1.108a
// This function should only be called when it's in Root (icon) menu 
void _DisplayCurrentSettings(void)
{
    _pTempMenu3 = _pTempMenu; // *** backup _pTempMenu, because _GetMenuItemPosition reference it
    
    _pTempMenu2 = GET_SUB_MENU(_pTempMenu, __bMENUSelect[0]);
    if (_pTempMenu2)
    {
        //brian1.10, fix bug, use item number & row number to check the number
        _bMENUTemp = GET_MENU_ROW(_pTempMenu2) < GET_MENU_ITEM_NUM(_pTempMenu2) ? GET_MENU_ROW(_pTempMenu2) : GET_MENU_ITEM_NUM(_pTempMenu2); 

        _MENU_RectAttr.bColor = OSDSETUP_ENTRY_ICON_COLOR_BG;
        _OSDMENU_FillRect(SETUP_MENU_VALUE_X, SETUP_MENU_VALUE_Y,
                  SETUP_MENU_VALUE_X+SETUP_MENU_VALUE_W-1,
                  SETUP_MENU_VALUE_Y+MENU_ITEM_HEIGHT*_bMENUTemp+MENU_BORDER_THICKNESS*2-1); //CoCo1.20, reduce code size.

        // Set Text Color
        _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_DESC_TEXT_COLOR;
        _MENU_StrAttr.bBackground = OSDSETUP_ENTRY_ICON_COLOR_BG;
        _MENU_StrAttr.bShadow1 = OSDSETUP_ENTRY_HELP_COLOR_BG;
        _MENU_StrAttr.bShadow2 = OSDSETUP_ENTRY_HELP_COLOR_BG;

        //brian1.10, fix bug, use item number & row number to check the number
        for (_bMENUTemp=0; _bMENUTemp<((GET_MENU_ROW(_pTempMenu2) < GET_MENU_ITEM_NUM(_pTempMenu2)) ? GET_MENU_ROW(_pTempMenu2) : GET_MENU_ITEM_NUM(_pTempMenu2)); _bMENUTemp++)
        {
            _pTempMenu = GET_SUB_MENU(_pTempMenu2, _bMENUTemp);
            if (_pTempMenu)
            {
                if (GET_MENU_CHECK(_pTempMenu) != NULL) // Brian1.26, Keil 7.02 compile issue
                {
                    // search the menu to get item position
                    _bMENUTemp2 = _GetMenuItemPosition(*(GET_MENU_CHECK(_pTempMenu)));
                }
                else
                {
                    // special cases: "Default" item, has no check variable
                    _bMENUTemp2 = 0; // just show "Read". it is the first and only item in the value menu

                }

                _MENU_StrAttr.wX = GET_MENU_X(_pTempMenu)+MENU_BORDER_THICKNESS+MENU_ITEM_X_OFFSET;
                _MENU_StrAttr.wY = GET_MENU_Y(_pTempMenu)+MENU_BORDER_THICKNESS+_bMENUTemp*MENU_ITEM_HEIGHT;
                _OSDMENU_DrawDynamicString(GET_MENU_ITEM_TEXT(_pTempMenu, _bMENUTemp2)); //CoCo1.20, reduce code size.
                 
            }
            else
            {

                // special cases: "Password" item, has sub-menu at all
                // just show "****"
                _MENU_StrAttr.wX = SETUP_MENU_VALUE_X+MENU_BORDER_THICKNESS+MENU_ITEM_X_OFFSET;
                _MENU_StrAttr.wY = SETUP_MENU_VALUE_Y+MENU_BORDER_THICKNESS+_bMENUTemp*MENU_ITEM_HEIGHT;
                _OSDMENU_DrawDynamicString(aSETUPPassword); //CoCo1.20, reduce code size.
            }

        } // for
    }
    // **** restore _pTempMenu
    _pTempMenu = _pTempMenu3;
}


void _ClearCurrentSettings(void)
{

    _bMENUTemp = 7; // always clear all the area, use the "maximum" number of items
    _MENU_RectAttr.bColor = OSDSETUP_ENTRY_CENTER_COLOR_BG;
    _OSDMENU_FillRect(SETUP_MENU_VALUE_X,
              SETUP_MENU_VALUE_Y-MENU_SCROLL_HEIGHT,
              SETUP_MENU_VALUE_X+SETUP_MENU_VALUE_W-1,
              SETUP_MENU_VALUE_Y+MENU_ITEM_HEIGHT*_bMENUTemp+MENU_BORDER_THICKNESS*2+MENU_SCROLL_HEIGHT-1); //CoCo1.20, reduce code size.
}


//***************************************************************************
//  Function    :   _InitRect
//  Abstract    :   Initialize the rectangle.
//  Arguments   :
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void _InitRect(PURECT pRect, WORD wLeft, WORD wTop, WORD wRight, WORD wBottom)
{
    pRect->wLeft   = wLeft;
    pRect->wRight  = wRight;
    pRect->wTop    = wTop;
    pRect->wBottom = wBottom;
}

BYTE _PScan_Callback(void)
{
    if (OSDCOMDLG_Get_Select_Button())
        bStatus = SETUP_INPUT_PASSWORD_CORRECT;
    else
        bStatus = SETUP_INPUT_PASSWORD_WRONG;

      SETUP_GDIInit();
      SETUP_PartialRedraw(DIALOG_INPUT_PSCAN_X1, DIALOG_INPUT_PSCAN_Y1, DIALOG_INPUT_PSCAN_X2, DIALOG_INPUT_PSCAN_Y2);
//     SETUP_Recover(OSD_RECOVER_ALL);

    if (bStatus != SETUP_INPUT_PASSWORD_INVALID_KEY)
    {
       // save the value when accept (time out accept or user confirm)
       if  (__bSetupMode &  (bStatus == SETUP_INPUT_PASSWORD_CORRECT ))
       {
            OSDMENU_MenuOperation(KEY_ENTER);
            _SaveDataToSetupInfo();
       }

       __bInputPassword = FALSE;
       __bOSDPromptMode=FALSE;
    }

    return KEY_NO_KEY;
}

//CoCo1.20, reduce code size.
void _OSDMENU_DrawFrame(BYTE bStyle)
{
    _MENU_FrameAttr.bStyle = bStyle;
    _MENU_FrameAttr.bColorBright = OSDMENU_ENTRY_MENU_COLOR_BRIGHT;
    _MENU_FrameAttr.bColorDark = OSDMENU_ENTRY_MENU_COLOR_DARK;
    _MENU_FrameAttr.bColorCenter = OSDMENU_ENTRY_MENU_COLOR_CENTER;
    _MENU_FrameAttr.bThickness = MENU_BORDER_THICKNESS;

    GDI_DrawFrame_909(0, &_MENU_FrameAttr);
}

//CoCo1.20, reduce code size.
void _OSDMENU_SetMenuStringColor(void)
{
    _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_TEXT_COLOR_NORMAL;
    _MENU_StrAttr.bBackground = OSDMENU_ENTRY_MENU_COLOR_CENTER;
    _MENU_StrAttr.bShadow1 = OSDMENU_ENTRY_MENU_COLOR_CENTER;
    _MENU_StrAttr.bShadow2 = OSDMENU_ENTRY_MENU_COLOR_CENTER;
}

//CoCo1.20, reduce code size.
void _OSDMENU_SetDlgStringColor(void)
{
    _MENU_StrAttr.bTextColor = OSDMENU_ENTRY_MENU_COLOR_BRIGHT;
    _MENU_StrAttr.bBackground = OSDMENU_ENTRY_MENU_COLOR_DARK;
}

//CoCo1.20, reduce code size.
void _OSDMENU_FillRect(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom)
{
    _InitRect(&(_MENU_RectAttr.rect), wLeft, wTop, wRight, wBottom);
    
    GDI_FillRect_909(0, &_MENU_RectAttr);
}

//CoCo1.20, reduce code size.
void _OSDMENU_DrawString(void)
{
    GDI_DrawString_909(0, &_MENU_StrAttr, _wMENUString);
}

//CoCo1.20, reduce code size.
void _OSDMENU_DrawDynamicString(WORD *pwString)
{
    GDI_DrawString_909(0, &_MENU_StrAttr, pwString);
}

#endif // #ifdef SUPPORT_STB


void OSDPASSWORD_Exit(BYTE bExitMode)
{
}

BYTE OSDPASSWORD_Recover(BYTE bRecoverRegion)
{
    return FALSE;
}

void OSDPASSWORD_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;

    RegionInfo.wWidth = GDI_REGION_WIDTH;

#ifdef ENLARGE_OSD_FOR_PAL_MODE
    if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
    {
        RegionInfo.wHeight = GDI_REGION_HEIGHT;
    }
    else
    {
        RegionInfo.wHeight = GDI_REGION_HEIGHT_PAL;
    }
#else
    RegionInfo.wHeight = GDI_REGION_HEIGHT;
#endif

    RegionInfo.bColorMode = GDI_OSD_4B_MODE;

    RegionInfo.dwTAddr = DS_OSDFRAME_ST;

//  Alan2.37, fix Mantis #2359
//    OSD_SetRegion(0, FALSE, &RegionInfo);
    OSD_SetRegion(0, TRUE, &RegionInfo);
}

#endif //#ifndef IMAGE_FRAME_SETUP //CoCo2.38