

#include "winav.h"
#include "input.h"
#include "osd.h"
#include "osdnd.h"
#include "cc.h"
#include "osddlg.h"
#include "osddivx.h"
#include "parser.h"
#include "utl.h"
#include "setup.h"
#include "notransl.h"
#include "fontindex.h" //++CoCo2.37P

#ifndef NO_DRM

#define   REG_CODE_LENGTH    9
#define   DIALOG_X0          110
#define   DIALOG_Y0          100
#define   DIALOG_X1          500
#define   DIALOG_Y1          350

PARM_RECT        _DLG_RectAttr;
PARM_DRAW_STRING _DLG_StrAttr;
BYTE             _bButtonNum;
DIVX_DLG_TYPE    _bDialogType;

/* alan test
WORD aDivXRegCode1[] = {54, CHAR_D, CHAR_i, CHAR_v, CHAR_X, CHAR_REGISTERED_SIGN, 
                        CHAR_SPACE, CHAR_V, CHAR_i, CHAR_d, CHAR_e, CHAR_o,
                        CHAR_SPACE, CHAR_O, CHAR_n, CHAR_SPACE, CHAR_D,
                        CHAR_e, CHAR_m, CHAR_a, CHAR_n, CHAR_d, CHAR_SPACE,
                        CHAR_SPACE, 
                        CHAR_Y, CHAR_o, CHAR_u, CHAR_r, CHAR_SPACE, CHAR_r,
                        CHAR_e, CHAR_g, CHAR_i, CHAR_s, CHAR_t, CHAR_r,
                        CHAR_a, CHAR_t, CHAR_i, CHAR_o, CHAR_n,
                        CHAR_SPACE,
                        CHAR_c, CHAR_o, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_i,
                        CHAR_s, CHAR_SPACE, CHAR_COLON, CHAR_SPACE, CHAR_SPACE,
                        CHAR_SPACE, CHAR_SPACE};
 
WORD aDivXRegCode2[] = {19, CHAR_T, CHAR_o, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_a,
                        CHAR_r, CHAR_n, CHAR_SPACE, CHAR_m, CHAR_o, CHAR_r,
                        CHAR_e, CHAR_SPACE, CHAR_v, CHAR_i, CHAR_s, CHAR_i,
                        CHAR_t};

WORD aDivXRegCode3[] = {16, CHAR_w, CHAR_w, CHAR_w, CHAR_FULL_STOP, CHAR_d, CHAR_i,
                        CHAR_v, CHAR_x, CHAR_FULL_STOP, CHAR_c, CHAR_o, CHAR_m,
                        CHAR_SOLIDUS, CHAR_v, CHAR_o, CHAR_d};

WORD aDivXAuthorError[] = {70, CHAR_A, CHAR_u, CHAR_t, CHAR_h, CHAR_o, CHAR_r,
                           CHAR_i, CHAR_z, CHAR_a, CHAR_t, CHAR_i, CHAR_o,
                           CHAR_n, CHAR_SPACE, CHAR_E, CHAR_r, CHAR_r, CHAR_o,
                           CHAR_r, CHAR_FULL_STOP, CHAR_SPACE,
                            CHAR_T, CHAR_h, CHAR_i, CHAR_s, CHAR_SPACE,
                            CHAR_p, CHAR_l, CHAR_a, CHAR_y, CHAR_e, CHAR_r,
                            CHAR_SPACE, CHAR_i, CHAR_s, CHAR_SPACE, CHAR_n,
                            CHAR_o, CHAR_t, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_t,
                            CHAR_h, CHAR_o, CHAR_r, CHAR_i, CHAR_z, CHAR_e,
                            CHAR_d, CHAR_SPACE,
                            CHAR_t, CHAR_o, CHAR_SPACE, CHAR_p, CHAR_l,
                            CHAR_a, CHAR_y, CHAR_SPACE, CHAR_t, CHAR_h, CHAR_i,
                            CHAR_s, CHAR_SPACE, CHAR_v, CHAR_i, CHAR_d, CHAR_e,
                            CHAR_o, CHAR_FULL_STOP};

WORD aDivXAuthorExpired[] = {14, CHAR_R, CHAR_e, CHAR_n, CHAR_t, CHAR_a, CHAR_l,
                             CHAR_SPACE, CHAR_E, CHAR_x, CHAR_p, CHAR_i,
                             CHAR_r, CHAR_e, CHAR_d};

WORD aDivXRentalConfirm[] = {99, CHAR_V, CHAR_i, CHAR_e, CHAR_w, CHAR_SPACE, 
                             CHAR_D, CHAR_i, CHAR_v, CHAR_X, CHAR_REGISTERED_SIGN,
                             CHAR_V, CHAR_O, CHAR_D, CHAR_SPACE, CHAR_R,
                             CHAR_e, CHAR_n, CHAR_t, CHAR_a, CHAR_l,
                             CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_T,
                             CHAR_h, CHAR_i, CHAR_s, CHAR_SPACE, CHAR_r,
                             CHAR_e, CHAR_n, CHAR_t, CHAR_a, CHAR_l,
                             CHAR_SPACE, CHAR_h, CHAR_a, CHAR_s, CHAR_SPACE,
                             CHAR_0, CHAR_0, CHAR_SPACE, CHAR_v, CHAR_i,
                             CHAR_e, CHAR_w, CHAR_s, CHAR_SPACE, CHAR_l,
                             CHAR_e, CHAR_f, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE,
                             CHAR_D, CHAR_o, CHAR_SPACE, CHAR_y, CHAR_o,
                             CHAR_u, CHAR_SPACE, CHAR_w, CHAR_a, CHAR_n,
                             CHAR_t, CHAR_SPACE, CHAR_t, CHAR_o, CHAR_SPACE,
                             CHAR_u, CHAR_s, CHAR_e, CHAR_SPACE, CHAR_o,
                             CHAR_n, CHAR_e, CHAR_SPACE, CHAR_o, CHAR_f,
                             CHAR_SPACE, CHAR_y, CHAR_o, CHAR_u, CHAR_r,
                             CHAR_SPACE, CHAR_0, CHAR_0, CHAR_SPACE, CHAR_v,
                             CHAR_i, CHAR_e, CHAR_w, CHAR_s, CHAR_SPACE,
                             CHAR_n, CHAR_o, CHAR_w, CHAR_QUESTION_MARK};
*/

// LLY2.53, remove from "notransl.c" to osddivx.c since "notransl.o (*.data)" will be put @ "rodata" section
// It means that the data is read only can be modified
// But, current code will update the remain view count within aDivXReentalConfirm[]
WORD aDivXRentalConfirm[] = {99, CHAR_V, CHAR_i, CHAR_e, CHAR_w, CHAR_SPACE,
                             CHAR_D, CHAR_i, CHAR_v, CHAR_X, CHAR_REGISTERED_SIGN,
                             CHAR_V, CHAR_O, CHAR_D, CHAR_SPACE, CHAR_R,
                             CHAR_e, CHAR_n, CHAR_t, CHAR_a, CHAR_l,
                             CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_T,
                             CHAR_h, CHAR_i, CHAR_s, CHAR_SPACE, CHAR_r,
                             CHAR_e, CHAR_n, CHAR_t, CHAR_a, CHAR_l,
                             CHAR_SPACE, CHAR_h, CHAR_a, CHAR_s, CHAR_SPACE,
                             CHAR_0, CHAR_0, CHAR_SPACE, CHAR_v, CHAR_i,
                             CHAR_e, CHAR_w, CHAR_s, CHAR_SPACE, CHAR_l,
                             CHAR_e, CHAR_f, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE,
                             CHAR_D, CHAR_o, CHAR_SPACE, CHAR_y, CHAR_o,
                             CHAR_u, CHAR_SPACE, CHAR_w, CHAR_a, CHAR_n,
                             CHAR_t, CHAR_SPACE, CHAR_t, CHAR_o, CHAR_SPACE,
                             CHAR_u, CHAR_s, CHAR_e, CHAR_SPACE, CHAR_o,
                             CHAR_n, CHAR_e, CHAR_SPACE, CHAR_o, CHAR_f,
                             CHAR_SPACE, CHAR_y, CHAR_o, CHAR_u, CHAR_r,
                             CHAR_SPACE, CHAR_0, CHAR_0, CHAR_SPACE, CHAR_v,
                             CHAR_i, CHAR_e, CHAR_w, CHAR_s, CHAR_SPACE,
                             CHAR_n, CHAR_o, CHAR_w, CHAR_QUESTION_MARK};



BYTE _PressingKey_Callback(void);
BYTE _OSDDIVX_Recover(void);

BOOL OSDDIVX_DrawDialog(DIVX_DLG_TYPE bDlgType)
{
    BYTE aRegCode[REG_CODE_LENGTH];  // The last one char is NULL
    WORD aChar[REG_CODE_LENGTH];
    BYTE bTemp;
    
    
    _bDialogType = bDlgType;

//    OSDCOMDLG_SetRect(110, 100, 500, 350);
    OSDCOMDLG_SetRect(DIALOG_X0, DIALOG_Y0, DIALOG_X1, DIALOG_Y1);

    // LLY0.95, move the calculat regist code and remain view procedure earily
    // To avoid to see the information message display first, 
    // then regist code or remain view information later.
    switch (bDlgType)
    {
    case DLG_REG_CODE:
        _bButtonNum = 1;        
        OSDCOMDLG_SetMessage(aDivXRegCode1);
        aChar[0] = REG_CODE_LENGTH - 1;
        PARSER_DRM_GetRegistrationCodeString(aRegCode);
        for (bTemp = 0; bTemp < REG_CODE_LENGTH - 1; bTemp++)
        {
            aChar[bTemp+1] = OSDUTL_TransformChar(aRegCode[bTemp]);
        }        
        break;
    case DLG_AUTHOR_ERROR:
        _bButtonNum = 1;
        OSDCOMDLG_SetMessage(aDivXAuthorError);
        break;
    case DLG_RENTAL_EXPIRED:
        _bButtonNum = 1;
        OSDCOMDLG_SetMessage(aDivXAuthorExpired);
        break;
    case DLG_RENTAL_CONFIRM:
        _bButtonNum = 2;
        OSDCOMDLG_SetMessage(aDivXRentalConfirm);
        aDivXRentalConfirm[41] = __bDRMRemainView / 10 + CHAR_0;
        aDivXRentalConfirm[42] = __bDRMRemainView % 10 + CHAR_0;
        aDivXRentalConfirm[87] = aDivXRentalConfirm[41];
        aDivXRentalConfirm[88] = aDivXRentalConfirm[42];
        break;
    }

    if (OSDCOMDLG_DrawDialog())
    {
        if (DLG_REG_CODE == bDlgType)
        {
            OSDCOMDLG_DrawString(50, 80, aChar);
            OSDCOMDLG_DrawString(50, 110, aDivXRegCode2);
            OSDCOMDLG_DrawString(50, 140, aDivXRegCode3);
        }
        if (DLG_RENTAL_CONFIRM == bDlgType)
        {
            aDivXRentalConfirm[41] = CHAR_0;
            aDivXRentalConfirm[42] = CHAR_0;
            aDivXRentalConfirm[87] = CHAR_0;
            aDivXRentalConfirm[88] = CHAR_0;
        }

        OSDCOMDLG_Register_CallbackFunc(_PressingKey_Callback);
        OSDCOMDLG_Register_RecoverFunc(_OSDDIVX_Recover);
        OSDCOMDLG_Select_OK_YES_Button();
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

BYTE OSDDIVX_ProcessKey(void)
{
    extern BOOL _bDialogShowUp;

    BYTE bKeyStatus = KEY_INVALID;

    if (FALSE == _bDialogShowUp) return KEY_BYPASS;

    switch (__bKey)
    {
    case KEY_NO_KEY:
    case KEY_TVMODE:
    case KEY_MUTE:
    case KEY_VOL_UP:
    case KEY_VOL_DOWN:
    case KEY_KEYUP:
    case KEY_KEYDOWN:
    case KEY_PBC:
    case KEY_POWER:
    case KEY_OPEN_CLOSE:
        bKeyStatus = KEY_BYPASS;
        break;

    case KEY_PLAY_PAUSE:
    case KEY_ENTER:
    case KEY_PLAY:
        OSDCOMDLG_Exit(OSD_UI_EXIT_ALL);
        bKeyStatus = KEY_NO_KEY;
        break;
    case KEY_RIGHT:
        OSDCOMDLG_Select_Cancel_NO_Button();
        bKeyStatus = KEY_NO_KEY;
        break;
    case KEY_LEFT:
        OSDCOMDLG_Select_OK_YES_Button();
        bKeyStatus = KEY_NO_KEY;
        break;
    default:
        bKeyStatus = KEY_INVALID;
        break;
    }
    return bKeyStatus;
}

BYTE _PressingKey_Callback(void)
{
    extern BYTE __bSetupMode;

    // LLY0.95, process the user selection 
    switch(_bDialogType)
    {       
    // Abort the playback while Authorized Error or Rental Expired
    case    DLG_AUTHOR_ERROR:
    case    DLG_RENTAL_EXPIRED:
        __bDRMPlayState=DRM_PLAY_STATE_ABORT;
        break;
        
    case    DLG_RENTAL_CONFIRM:
        // Get selection button
        __bTemp=OSDCOMDLG_Get_Select_Button();

        // If Yes, continue the playback
        if(__bTemp == BUTTON_OK_YES)
        {
            __bDRMPlayState=DRM_PLAY_STATE_CONTINUE;
        }
        // If No, abort the playback
        else
        {
            __bDRMPlayState=DRM_PLAY_STATE_ABORT;
        }
        break;
    case DLG_REG_CODE:     // Alan 1.11c, to fix the VOD display problem
        if (__bSetupMode)
        {
              SETUP_GDIInit();
              SETUP_PartialRedraw(DIALOG_X0, DIALOG_Y0, DIALOG_X1, DIALOG_Y1);
//            SETUP_Recover(OSD_RECOVER_ALL);
        }
        break;
    default:            
        // Nothing need to do
        break;
    }
    
    return KEY_NO_KEY;
}

BYTE _OSDDIVX_Recover(void)
{
    OSDDIVX_DrawDialog(_bDialogType);
    return TRUE;
}

#endif
