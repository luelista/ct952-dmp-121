//**************************************************************************
//         Copyright (c) 2004, Cheertek Inc . All rights reserved.
//         D300, all right reserved.

//	Product	: WinDVD Firmware

//	Date	: 2004.9.10
//	Author	: Cheertek (D300 CoCo Chiang)
//	Purpose	: This is the module for all independent dialog box.
//	Sources	: osddlg.c/osddlg.h
//***************************************************************************

#ifndef __OSDDLG_H__
#define __OSDDLG_H__

//Define the width and height of OSD plane
//define the OSD DRAM configuration
#define OSDDLG_OSD_REGION_HEIGHT                            440

#define OSD_PLANE_WIDTH     600
#define OSD_PLANE_HEIGHT    420

//Define the state of the button
#define OSDDLG_BUTTON_STATE_NORMAL                          0
#define OSDDLG_BUTTON_STATE_HIGHLIGHT                       1

//Define the attribute for draw/clear
#define OSDDLG_UPDATE_DRAW                                  0
#define OSDDLG_UPDATE_CLEAR                                 1

//Define the state of displaying the dialog box.
#define OSDDLG_SHOW_DLG_INITIAL                             0
#define OSDDLG_SHOW_DLG_RECOVER                             1

//Define the message for updating the dialog box.
#define OSDDLG_MSG_SHOW_DLG                                 0
#define OSDDLG_MSG_DRAW_HIGHLIGHT                           1
#define OSDDLG_MSG_CLEAR_HIGHLIGHT                          2
#define OSDDLG_MSG_EXIT                                     3

//Define the mix ratio of the background.
#define OSDDLG_MEDIA_SELECT_DLG_MIX_RATIO_BACKGROUND        14

//Define the ID of the dialog box
#define OSDDLG_ID_MEDIA_SELECT_DLG                          0     

//Define the background of Media Select dialog
#define OSDDLG_MEDIA_SELECT_BACKGROUND_GENERAL_MODE         0
#define OSDDLG_MEDIA_SELECT_BACKGROUND_MM_MODE              1

/*
typedef enum tagOSD_DLG_BUTTON
{
    OSDDLG_MEDIA_SELECT_BUTTON_DVDROM = 0,
#ifdef SUPPORT_STB
    OSDDLG_MEDIA_SELECT_BUTTON_STB,
    OSDDLG_MEDIA_SELECT_BUTTON_OTHERS,
#else
    OSDDLG_MEDIA_SELECT_BUTTON_OTHERS,
#endif

#ifdef SUPPORT_BUILDIN_CARD_READER
    OSDDLG_MEDIA_SELECT_BUTTON_CARD_READER,
#endif

#ifdef SUPPORT_AV_IN
    OSDDLG_MEDIA_SELECT_BUTTON_AVIN,
#endif

    OSDDLG_MEDIA_SELECT_BUTTON_END
} OSD_DLG_BUTTON;

#define OSDDLG_MEDIA_SELECT_TOTAL_BUTTONS                   OSDDLG_MEDIA_SELECT_BUTTON_END
*/

#define OSDDLG_MEDIA_SELECT_BUTTON_DVDROM       MEDIA_SELECT_DVD 
#ifdef SUPPORT_STB
#define OSDDLG_MEDIA_SELECT_BUTTON_STB          MEDIA_SELECT_STB
#endif
#ifdef SUPPORT_BUILDIN_CARD_READER
#define OSDDLG_MEDIA_SELECT_BUTTON_CARD_READER  MEDIA_SELECT_CARD_READER
#endif
#ifdef SUPPORT_AV_IN
#define OSDDLG_MEDIA_SELECT_BUTTON_AVIN         MEDIA_SELECT_AV_IN
#endif
#ifdef SUPPORT_USB_SOURCE
#define OSDDLG_MEDIA_SELECT_BUTTON_OTHERS       MEDIA_SELECT_USB
#endif //
#define OSDDLG_MEDIA_SELECT_TOTAL_BUTTONS       MEDIA_SELECT_END

//Define the attributes of the dialog box.
#define OSDDLG_MEDIA_SELECT_BACKGROUND_BUTTON_DISTANCE_H    16
#define OSDDLG_MEDIA_SELECT_BACKGROUND_BUTTON_DISTANCE_V    32
#define OSDDLG_MEDIA_SELECT_BUTTON_BUTTON_DISTANCE_V        16  
#define OSDDLG_MEDIA_SELECT_BUTTON_TEXT_DISTANCE_H          8
#define OSDDLG_MEDIA_SELECT_BUTTON_HEIGHT                   32

//Define the entry for the color palette of the Media Select dialog box.
#define OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_START                         GDI_GENERAL_UI_PALETTE_INDEX_START
#define OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_GENERAL_BACKGROUND            OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_START
#define OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_HIGHLIGHT_BUTTON              (OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_START+1)
#define OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_NORMAL_BUTTON                 (OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_START+2)
#define OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_HIGHLIGHT_BUTTON_STRING       (OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_START+3)
#define OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_NORMAL_BUTTON_STRING          (OSDDLG_MEDIA_SELECT_PALETTE_ENTRY_START+4)

//define the palette color
#define OSDDLG_MEDIA_SELECT_PALETTE_COLOR_GENERAL_BACKGROUND            0x0096E2                
#define OSDDLG_MEDIA_SELECT_PALETTE_COLOR_HIGHLIGHT_BUTTON              0xE3BE6D
#define OSDDLG_MEDIA_SELECT_PALETTE_COLOR_NORMAL_BUTTON                 0xA1E4FF
#define OSDDLG_MEDIA_SELECT_PALETTE_COLOR_HIGHLIGHT_BUTTON_STRING       0xFFFFFF
#define OSDDLG_MEDIA_SELECT_PALETTE_COLOR_NORMAL_BUTTON_STRING          0xFFFFFF


typedef struct tagDLG_ATTRIBUTE
{
    WORD    wStartH;
    WORD    wStartV;
    WORD    wWidth;
    WORD    wHeight;
} DLG_ATTRIBUTE, *PDLG_ATTRIBUTE;


//********************************************************************************************************
//Export APIs
//********************************************************************************************************
void OSDDLG_MEDIA_SELECT_Update(BYTE bMessage, BYTE bParam);
BYTE OSDDLG_MEDIA_SELECT_Recover(BYTE bRecoverRegion);
void OSDDLG_ConfigOSDRegion(void);

//********************************************************************************************************
//Internal APIs
//********************************************************************************************************
void _OSDDLG_MEDIA_SELECT_ShowDialogBox(BYTE bDisplayMode, BYTE bBackgroudMode);
void _OSDDLG_MEDIA_SELECT_InitializeGDI(void);
void _OSDDLG_MEDIA_SELECT_FillButtonString(BYTE bIndex);
WORD _OSDDLG_MEDIA_SELECT_GetMaxButtonWidth(void);
WORD _OSDDLG_MEDIA_SELECT_GetDlgHeight(void);
void _OSDDLG_MEDIA_SELECT_DrawButton(BYTE bIndex, BYTE bButtonState);
void _OSDDLG_MEDIA_SELECT_UpdateDlgBackground(BYTE bUpdate);
void _OSDDLG_DrawDynamicString(WORD *pwString);
//////////////////    Common Dialog  ///////////////////////////////////



#define BUTTON_OK_YES     1
#define BUTTON_CANCEL_NO  0

typedef void (*TIMEOUT_FUNC)(void);
typedef BYTE (*CALLBACK_FUNC)(void);
typedef BYTE (*RECOVER_FUNC)(void);


extern BOOL   OSDCOMDLG_SetButtonNum(BYTE nButtonNum);
extern void   OSDCOMDLG_Set_Buttons_Type(BOOL bOK_Cancel_Buttons);
extern void   OSDCOMDLG_SetRect(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom);
extern void   OSDCOMDLG_SetMessage(WORD* pwString);
extern void   OSDCOMDLG_DrawString(WORD wX, WORD wY, WORD* pwString);
extern BOOL   OSDCOMDLG_DrawDialog(void);
//extern BYTE   OSDCOMDLG_ProcessKey(BYTE bKey);
extern BYTE   OSDCOMDLG_ProcessKey(void);

extern void   OSDCOMDLG_Select_OK_YES_Button(void);
extern void   OSDCOMDLG_Select_Cancel_NO_Button(void);
extern BYTE   OSDCOMDLG_Get_Select_Button(void);
extern void   OSDCOMDLG_Set_WaitingTime(BYTE bSecond);
extern void   OSDCOMDLG_Register_TimeoutFunc(TIMEOUT_FUNC pTimeoutFunc);
extern void   OSDCOMDLG_Register_CallbackFunc(CALLBACK_FUNC pCallbackFunc);
extern void   OSDCOMDLG_Register_RecoverFunc(RECOVER_FUNC pRecoverFunc);

extern void OSDCOMDLG_Exit(BYTE bExitMode);
extern BYTE OSDCOMDLG_Recover(BYTE bRecoverRegion);
extern void OSDCOMDLG_ConfigOSDRegion(void);
// LLY2.53, expand time value unit from WORD to DWORD
//extern void OSDCOMDLG_Trigger(BYTE bType, WORD wTime);
extern void OSDCOMDLG_Trigger(BYTE bType, DWORD dwTime);


#define OSDCOMDLG_ENTRY_START        GDI_COMMON_DIALOG_PALETTE_INDEX_START //GDI_COMMON_DIALOG_PALETTE_NUM //CoCo1.20a
#define OSDCOMDLG_ENTRY_BG           (OSDCOMDLG_ENTRY_START+1)
#define OSDCOMDLG_ENTRY_BUTTON       (OSDCOMDLG_ENTRY_BG+1) 
#define OSDCOMDLG_ENTRY_HIGHLIGHT    (OSDCOMDLG_ENTRY_BUTTON+1)
#define OSDCOMDLG_ENTRY_STRING       (OSDCOMDLG_ENTRY_HIGHLIGHT+1)
#define OSDCOMDLG_ENTRY_FRAME_BRIGHT (OSDCOMDLG_ENTRY_STRING+1)
#define OSDCOMDLG_ENTRY_FRAME_DARK   (OSDCOMDLG_ENTRY_FRAME_BRIGHT+1)
#define OSDCOMDLG_ENTRY_FRAME_CENTER (OSDCOMDLG_ENTRY_FRAME_DARK+1)



#define OSDCOMDLG_VALUE_COLOR_BG        0xEFEFD7
#define OSDCOMDLG_VALUE_COLOR_BUTTON    0xC8C8C8
#define OSDCOMDLG_VALUE_COLOR_HIGHLIGHT 0x007F00
#define OSDCOMDLG_VALUE_COLOR_STRING    0x000000
#define OSDCOMDLG_VALUE_FRAME_BRIGHT    0xFFFFFF
#define OSDCOMDLG_VALUE_FRAME_DARK      0x888888 
#define OSDCOMDLG_VALUE_FRAME_CENTER    0xEFEFD7 

#endif //__OSDDLG_H__
