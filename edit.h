#ifndef __EIDT_H__
#define __EIDT_H__

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//The dialog is for notifying user that we are saving the files to flash
#define EDIT_DIALOG_WIDTH						360
#define EDIT_DIALOG_HEIGHT                      200
#define EDIT_DIALOG_CONTENT_DISTANCE_V          40

#define EDIT_DIALOG_COPY_DELETE_BUTTON_NUM      3
#define EDIT_DIALOG_COPY_BUTTON_NUM             2

#define EDIT_DIALOG_BUTTON_ID_COPY              0
#define EDIT_DIALOG_BUTTON_ID_DELETE            1
#define EDIT_DIALOG_BUTTON_ID_EXIT              2
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef EDIT_MAIN_FILE
#include "dialog.h"
#include "OSDString\\strextrn.h"

#ifdef SUPPORT_ENCODE_JPG_PICTURE
DLG_BUTTON _EDITDlgCopyDeleteButton[EDIT_DIALOG_COPY_DELETE_BUTTON_NUM] = {
    {aBUTTONCopy, EDIT_DIALOG_BUTTON_ID_COPY, DIALOG_BUTTON_START_X_OPTIMIZED},
    {aBUTTONDelete, EDIT_DIALOG_BUTTON_ID_DELETE, DIALOG_BUTTON_START_X_OPTIMIZED},
    {aMMBtnExit, EDIT_DIALOG_BUTTON_ID_EXIT, DIALOG_BUTTON_START_X_OPTIMIZED},
};

DLG_BUTTON _EDITDlgCopyButton[EDIT_DIALOG_COPY_BUTTON_NUM] = {
    {aBUTTONCopy, EDIT_DIALOG_BUTTON_ID_COPY, DIALOG_BUTTON_START_X_OPTIMIZED},
    {aMMBtnExit, EDIT_DIALOG_BUTTON_ID_EXIT, DIALOG_BUTTON_START_X_OPTIMIZED},
};

DIALOG _EDITDlg = {
    (MAINMENU_CONTENT_REGION_START_X+((MAINMENU_CONTENT_REGION_WIDTH-EDIT_DIALOG_WIDTH)>>1)),
    (MAINMENU_CONTENT_REGION_START_Y+((MAINMENU_CONTENT_REGION_HEIGHT-EDIT_DIALOG_HEIGHT)>>1)),
    EDIT_DIALOG_WIDTH,
    EDIT_DIALOG_HEIGHT,
    aEDITDialogTitle,
    DIALOG_TITLE_STRING_ALIGNMENT_CENTER,
    EDIT_DIALOG_COPY_DELETE_BUTTON_NUM,
    EDIT_DIALOG_BUTTON_ID_COPY,
    DIALOG_BUTTON_WIDTH_OPTIMIZED,
    DIALOG_BUTTON_START_Y_OPTIMIZED,
    _EDITDlgCopyDeleteButton,
};
#endif //SUPPORT_ENCODE_JPG_PICTURE

#endif //#ifdef EDIT_MAIN_FILE
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Export functions
void EDIT_Entry(void);
void EDIT_Recover(void);
void EDIT_Exit(BYTE bClearBackground);
void EDIT_ProcessKey(void);

//Internal functions
void _EDIT_DeleteDlg(void);
void _EDIT_ConfigOSDRegion(void);

#endif //__EIDT_H__