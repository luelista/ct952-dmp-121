#include "stringdec.h"
#ifdef OSDDSPLY_MAIN_FILE									

WORD aDSPLYLanguageJapanese[11] = {7, CHAR_J, CHAR_a, CHAR_p, CHAR_o, CHAR_n, CHAR_e3, CHAR_s};
WORD aDSPLYLanguageEnglish[10] = {6, CHAR_I, CHAR_n, CHAR_g, CHAR_l, CHAR_e3, CHAR_s};
WORD aDSPLYLanguageFrench[12] = {7, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_e3, CHAR_s};
WORD aDSPLYLanguageGerman[10] = {6, CHAR_A, CHAR_l, CHAR_e, CHAR_m, CHAR_a4, CHAR_o};
WORD aDSPLYLanguageRussian[9] = {5, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_o};
WORD aDSPLYLanguageChinese[11] = {6, CHAR_C, CHAR_h, CHAR_i2, CHAR_n, CHAR_e3, CHAR_s};
WORD aDSPLYLanguageKorean[11] = {7, CHAR_C, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_o};
WORD aDSPLYLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aDSPLYLanguageThai[10] = {9, CHAR_T, CHAR_a, CHAR_i, CHAR_l, CHAR_a, CHAR_n, CHAR_d, CHAR_e3, CHAR_s};
WORD aDSPLYLanguageItalian[12] = {8, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n, CHAR_o};
WORD aDSPLYLanguageSpanish[11] = {8, CHAR_E, CHAR_s, CHAR_p, CHAR_a, CHAR_n, CHAR_h, CHAR_o, CHAR_l};
WORD aDSPLYLanguagePortuguese[14] = {9, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_u, CHAR_e3, CHAR_s};
WORD aDSPLYLanguageOthers[9] = {6, CHAR_O, CHAR_u, CHAR_t, CHAR_r, CHAR_o, CHAR_s};
WORD aDSPLYOff[10] = {9, CHAR_D, CHAR_e, CHAR_s, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_d, CHAR_o};
#ifndef NO_DVD_DISPLAY_UI									

WORD aDSPLYTitleElapsed[17] = {13, CHAR_T, CHAR_i1, CHAR_t, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_c, CHAR_o, CHAR_r, CHAR_r, CHAR_i, CHAR_d, CHAR_o};
WORD aDSPLYTitleRemain[16] = {12, CHAR_T, CHAR_i1, CHAR_t, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_t, CHAR_e};
WORD aDSPLYChapterElapsed[16] = {14, CHAR_C, CHAR_a, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_c, CHAR_o, CHAR_r, CHAR_r, CHAR_i, CHAR_d, CHAR_o};
WORD aDSPLYChapterRemain[15] = {13, CHAR_C, CHAR_a, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_t, CHAR_e};
#endif //NO_DVD_DISPLAY_UI									

WORD aDSPLYSingleElapsed[17] = {12, CHAR_U, CHAR_m, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_c, CHAR_o, CHAR_r, CHAR_r, CHAR_i, CHAR_d, CHAR_o};
WORD aDSPLYSingleRemain[16] = {11, CHAR_U, CHAR_m, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_t, CHAR_e};
WORD aDSPLYTotalElapsed[18] = {12, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_c, CHAR_o, CHAR_r, CHAR_FULL_STOP};
WORD aDSPLYTotalRemain[17] = {12, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_s, CHAR_t, CHAR_a, CHAR_n};
WORD aDSPLYRootMenu[16] = {9, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_R, CHAR_o, CHAR_o, CHAR_t};
WORD aDSPLYTitleMenu[12] = {10, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e};
#ifdef SUPPORT_NAVIGATOR									

WORD aDSPLYRepeatTitle[20] = {19, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_t, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_t, CHAR_i1, CHAR_t, CHAR_u, CHAR_l, CHAR_o};
WORD aDSPLYRepeatChapter[22] = {21, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_t, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_c, CHAR_a, CHAR_p, CHAR_i1, CHAR_t, CHAR_u, CHAR_l, CHAR_o};
WORD aDSPLYRepeatDisc[19] = {18, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_t, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aDSPLYRepeatOff[22] = {10, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_s};
#endif //SUPPORT_NAVIGATOR									

WORD aDSPLYpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aDSPLYInterlace[15] = {14, CHAR_E, CHAR_n, CHAR_t, CHAR_r, CHAR_e, CHAR_l, CHAR_a, CHAR_c1, CHAR_a, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_o};
WORD aDSPLYResolution[15] = {10, CHAR_R, CHAR_e, CHAR_s, CHAR_o, CHAR_l, CHAR_u, CHAR_c1, CHAR_a4, CHAR_o, CHAR_COLON};
WORD aDSPLYDate[7] = {5, CHAR_D, CHAR_a, CHAR_t, CHAR_a, CHAR_COLON};
#ifdef SUPPORT_NAVIGATOR									

WORD aNGTitle[8] = {7, CHAR_T, CHAR_i1, CHAR_t, CHAR_u, CHAR_l, CHAR_o, CHAR_COLON};
WORD aNGChapter[11] = {8, CHAR_C, CHAR_a, CHAR_p, CHAR_i1, CHAR_t, CHAR_u, CHAR_l, CHAR_o};
WORD aNGAngle[13] = {7, CHAR_A4, CHAR_n, CHAR_g, CHAR_u, CHAR_l, CHAR_o, CHAR_COLON};
WORD aNGAudio[7] = {6, CHAR_A2, CHAR_u, CHAR_d, CHAR_i, CHAR_o, CHAR_COLON};
WORD aNGSubtitle1[13] = {9, CHAR_L, CHAR_e, CHAR_g, CHAR_e, CHAR_n, CHAR_d, CHAR_a, CHAR_1, CHAR_COLON};
WORD aNGSubtitle2[13] = {9, CHAR_L, CHAR_e, CHAR_g, CHAR_e, CHAR_n, CHAR_d, CHAR_a, CHAR_2, CHAR_COLON};
#endif //SUPPORT_NAVIGATOR									

#endif //OSDDSPLY_MAIN_FILE									

#ifdef OSDND_MAIN_FILE									

WORD aNDNoDisc[14] = {12, CHAR_N, CHAR_e, CHAR_n, CHAR_h, CHAR_u, CHAR_m, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aNDNoUSBDevice[14] = {13, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_U, CHAR_S, CHAR_B, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_v, CHAR_i, CHAR_c, CHAR_e};
WORD aNDOpen[7] = {5, CHAR_A, CHAR_b, CHAR_r, CHAR_i, CHAR_r};
WORD aNDClose[10] = {6, CHAR_F, CHAR_e, CHAR_c, CHAR_h, CHAR_a, CHAR_r};
WORD aNDReturn[8] = {6, CHAR_V, CHAR_o, CHAR_l, CHAR_t, CHAR_a, CHAR_r};
WORD aNDFreeze[15] = {13, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_f, CHAR_r, CHAR_e, CHAR_e, CHAR_z, CHAR_e};
WORD aNDRandomOn[12] = {11, CHAR_R, CHAR_e, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_e, CHAR_a, CHAR_t, CHAR_FULL_STOP};
WORD aNDRandomOff[12] = {9, CHAR_R, CHAR_e, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_d, CHAR_FULL_STOP};
WORD aNDResume[11] = {7, CHAR_R, CHAR_e, CHAR_t, CHAR_o, CHAR_m, CHAR_a, CHAR_r};
WORD aNDRootMenu[16] = {9, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_R, CHAR_o, CHAR_o, CHAR_t};
WORD aNDTitleMenu[12] = {10, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e};
WORD aNDRotateRight[26] = {25, CHAR_R, CHAR_o, CHAR_d, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_9, CHAR_0, CHAR_N_DEGREE, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_r, CHAR_e, CHAR_i, CHAR_t, CHAR_a, CHAR_SPACE};
WORD aNDRotateLeft[26] = {25, CHAR_R, CHAR_o, CHAR_d, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_9, CHAR_0, CHAR_N_DEGREE, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_e, CHAR_s, CHAR_q, CHAR_u, CHAR_e, CHAR_r, CHAR_d, CHAR_a};
WORD aNDFlipHorizontally[26] = {24, CHAR_I, CHAR_n, CHAR_v, CHAR_e, CHAR_r, CHAR_t, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_o, CHAR_r, CHAR_i, CHAR_z, CHAR_o, CHAR_n, CHAR_t, CHAR_a, CHAR_l, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_e};
WORD aNDFlipVertically[25] = {22, CHAR_I, CHAR_n, CHAR_v, CHAR_e, CHAR_r, CHAR_t, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_c, CHAR_a, CHAR_l, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_e};
WORD aNDpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aNDInterlace[15] = {14, CHAR_E, CHAR_n, CHAR_t, CHAR_r, CHAR_e, CHAR_l, CHAR_a, CHAR_c1, CHAR_a, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_o};
WORD aNDRepeatNone[10] = {9, CHAR_d, CHAR_e, CHAR_s, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_d, CHAR_a};
WORD aNDRepeatTrack[8] = {5, CHAR_f, CHAR_a, CHAR_i, CHAR_x, CHAR_a};
WORD aNDRepeatDisc[7] = {5, CHAR_d, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aNDRepeatAll[10] = {4, CHAR_T, CHAR_u, CHAR_d, CHAR_o};
WORD aNDRepeatTitle[7] = {6, CHAR_t, CHAR_i1, CHAR_t, CHAR_u, CHAR_l, CHAR_o};
WORD aNDRepeatChapter[10] = {8, CHAR_c, CHAR_a, CHAR_p, CHAR_i1, CHAR_t, CHAR_u, CHAR_l, CHAR_o};
WORD aNDRepeatFolder[9] = {5, CHAR_p, CHAR_a, CHAR_s, CHAR_t, CHAR_a};
WORD aNDRepeatA[15] = {14, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_t, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_A};
WORD aNDRepeatAB[17] = {16, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_t, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_A, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aNDRepeatOff[22] = {10, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_s};
#ifndef NO_DISC_MODE									

WORD aNDNavigateOn[25] = {24, CHAR_F, CHAR_u, CHAR_n, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_a, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_A, CHAR_c, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_d, CHAR_a};
WORD aNDNavigateOff[28] = {27, CHAR_F, CHAR_u, CHAR_n, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_a, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s, CHAR_a, CHAR_c, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_d, CHAR_a};
WORD aNDPBCOn[13] = {10, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_d, CHAR_o};
WORD aNDPBCOff[14] = {13, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_s, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_d, CHAR_o};
#endif //#ifndef NO_DISC_MODE									

WORD aNDZoom1[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_1};
WORD aNDZoom2[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_2};
WORD aNDZoom4[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_4};
WORD aNDMoveLeft[24] = {21, CHAR_M, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_E, CHAR_s, CHAR_q, CHAR_u, CHAR_e, CHAR_r, CHAR_d, CHAR_a};
WORD aNDMoveRight[24] = {20, CHAR_M, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_r, CHAR_e, CHAR_i, CHAR_t, CHAR_a};
WORD aNDMoveUp[23] = {15, CHAR_M, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_C, CHAR_i, CHAR_m, CHAR_a};
WORD aNDMoveDown[23] = {16, CHAR_M, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_B, CHAR_a, CHAR_i, CHAR_x, CHAR_o};
WORD aNDVocalOn[15] = {12, CHAR_V, CHAR_o, CHAR_c, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_L, CHAR_i, CHAR_g, CHAR_a, CHAR_d, CHAR_o};
WORD aNDVocalOff[16] = {15, CHAR_V, CHAR_o, CHAR_c, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_d, CHAR_o};
WORD aNDVocalLL[15] = {8, CHAR_V, CHAR_o, CHAR_c, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_L, CHAR_L};
WORD aNDVocalRR[13] = {8, CHAR_V, CHAR_o, CHAR_c, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_R, CHAR_R};
WORD aNDKaraokeOn[17] = {14, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_L, CHAR_i, CHAR_g, CHAR_a, CHAR_d, CHAR_o};
WORD aNDKaraokeOff[18] = {17, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_d, CHAR_o};
WORD aNDJPEGEffectNone[12] = {10, CHAR_S, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_e, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o};
WORD aNDJPEGEffectCoverUp[27] = {15, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aNDJPEGEffectCoverDown[26] = {17, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
WORD aNDJPEGEffectCoverLeft[20] = {10, CHAR_C, CHAR_o, CHAR_b, CHAR_r, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_e, CHAR_s, CHAR_q};
WORD aNDJPEGEffectCoverRight[20] = {11, CHAR_C, CHAR_o, CHAR_b, CHAR_r, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_r, CHAR_FULL_STOP};
WORD aNDJPEGEffectUncoverUp[23] = {17, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aNDJPEGEffectUncoverDown[24] = {19, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
WORD aNDJPEGEffectUncoverLeft[21] = {10, CHAR_D, CHAR_e, CHAR_s, CHAR_c, CHAR_FULL_STOP, CHAR_SPACE, CHAR_e, CHAR_s, CHAR_q, CHAR_FULL_STOP};
WORD aNDJPEGEffectUncoverRight[22] = {10, CHAR_D, CHAR_e, CHAR_s, CHAR_c, CHAR_FULL_STOP, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_r, CHAR_FULL_STOP};
WORD aNDJPEGEffectWipeUp[22] = {14, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aNDJPEGEffectWipeDown[21] = {16, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
WORD aNDJPEGEffectWipeLeft[17] = {9, CHAR_L, CHAR_i, CHAR_m, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_e, CHAR_s, CHAR_q};
WORD aNDJPEGEffectWipeRight[18] = {9, CHAR_L, CHAR_i, CHAR_m, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_r};
WORD aNDJPEGEffectPushUp[14] = {5, CHAR_S, CHAR_u, CHAR_b, CHAR_i, CHAR_r};
WORD aNDJPEGEffectPushDown[13] = {6, CHAR_D, CHAR_e, CHAR_s, CHAR_c, CHAR_e, CHAR_r};
WORD aNDJPEGEffectPushLeft[17] = {8, CHAR_E, CHAR_s, CHAR_q, CHAR_u, CHAR_e, CHAR_r, CHAR_d, CHAR_a};
WORD aNDJPEGEffectPushRight[18] = {7, CHAR_D, CHAR_i, CHAR_r, CHAR_e, CHAR_i, CHAR_t, CHAR_a};
WORD aNDJPEGEffectFadeThroughBlack[26] = {25, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_F, CHAR_a, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_h, CHAR_r, CHAR_o, CHAR_u, CHAR_g, CHAR_h, CHAR_SPACE, CHAR_B, CHAR_l, CHAR_a, CHAR_c, CHAR_k};
WORD aNDJPEGEffectRandom[16] = {11, CHAR_R, CHAR_e, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_e, CHAR_a, CHAR_t, CHAR_FULL_STOP};
#ifndef NO_DISC_MODE									

WORD aNDEncrypt[17] = {13, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_i, CHAR_l, CHAR_l, CHAR_e, CHAR_g, CHAR_a, CHAR_l};
WORD aNDErrRegionCode[27] = {21, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_g, CHAR_i, CHAR_o, CHAR_n, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_e, CHAR_r, CHAR_r, CHAR_a, CHAR_d, CHAR_o};
WORD aNDWrongDisc[19] = {12, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_e, CHAR_r, CHAR_r, CHAR_a, CHAR_d, CHAR_o};
WORD aNDDamageDisc[19] = {16, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_n, CHAR_i, CHAR_f, CHAR_i, CHAR_c, CHAR_a, CHAR_d, CHAR_o};
#endif //#ifndef NO_DISC_MODE									

WORD aNDNoPlayableFile[25] = {16, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y, CHAR_a, CHAR_b, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_F, CHAR_i, CHAR_l, CHAR_e};
WORD aNDDisc[7] = {5, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aNDTrack[8] = {5, CHAR_F, CHAR_a, CHAR_i, CHAR_x, CHAR_a};
WORD aNDTitle[7] = {6, CHAR_T, CHAR_i1, CHAR_t, CHAR_u, CHAR_l, CHAR_o};
WORD aNDChapter[10] = {8, CHAR_C, CHAR_a, CHAR_p, CHAR_i1, CHAR_t, CHAR_u, CHAR_l, CHAR_o};
WORD aNDLR[4] = {3, CHAR_L, CHAR_SOLIDUS, CHAR_R};
WORD aNDLL[4] = {3, CHAR_L, CHAR_SOLIDUS, CHAR_L};
WORD aNDRR[4] = {3, CHAR_R, CHAR_SOLIDUS, CHAR_R};
WORD aNDVolume[11] = {6, CHAR_V, CHAR_o, CHAR_l, CHAR_u, CHAR_m, CHAR_e};
WORD aNDTone[9] = {3, CHAR_T, CHAR_o, CHAR_m};
WORD aNDEcho[5] = {3, CHAR_E, CHAR_c, CHAR_o};
WORD aNDNoSurround[19] = {18, CHAR_S, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_s, CHAR_o, CHAR_m, CHAR_SPACE, CHAR_e, CHAR_n, CHAR_v, CHAR_o, CHAR_l, CHAR_v, CHAR_e, CHAR_n, CHAR_t, CHAR_e};
WORD aNDLiveSurround[10] = {7, CHAR_A, CHAR_o, CHAR_SPACE, CHAR_v, CHAR_i, CHAR_v, CHAR_o};
WORD aNDTheaterSurround[8] = {6, CHAR_T, CHAR_e, CHAR_a, CHAR_t, CHAR_r, CHAR_o};
WORD aNDStereoSurround[8] = {7, CHAR_E, CHAR_s, CHAR_t, CHAR_e2, CHAR_r, CHAR_e, CHAR_o};
WORD aNDMovieSurround[9] = {6, CHAR_C, CHAR_i, CHAR_n, CHAR_e, CHAR_m, CHAR_a};
WORD aNDKaraSurround[8] = {7, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e};
WORD aNDLanguageJapanese[11] = {7, CHAR_J, CHAR_a, CHAR_p, CHAR_o, CHAR_n, CHAR_e3, CHAR_s};
WORD aNDLanguageEnglish[10] = {6, CHAR_I, CHAR_n, CHAR_g, CHAR_l, CHAR_e3, CHAR_s};
WORD aNDLanguageFrench[12] = {7, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_e3, CHAR_s};
WORD aNDLanguageGerman[10] = {6, CHAR_A, CHAR_l, CHAR_e, CHAR_m, CHAR_a4, CHAR_o};
WORD aNDLanguageRussian[9] = {5, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_o};
WORD aNDLanguageChinese[11] = {6, CHAR_C, CHAR_h, CHAR_i2, CHAR_n, CHAR_e3, CHAR_s};
WORD aNDLanguageKorean[11] = {7, CHAR_C, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_o};
WORD aNDLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aNDLanguageThai[10] = {9, CHAR_T, CHAR_a, CHAR_i, CHAR_l, CHAR_a, CHAR_n, CHAR_d, CHAR_e3, CHAR_s};
WORD aNDLanguageItalian[12] = {8, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n, CHAR_o};
WORD aNDLanguageSpanish[11] = {8, CHAR_E, CHAR_s, CHAR_p, CHAR_a, CHAR_n, CHAR_h, CHAR_o, CHAR_l};
WORD aNDLanguagePortuguese[14] = {9, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_u, CHAR_e3, CHAR_s};
WORD aNDLanguageOthers[9] = {6, CHAR_O, CHAR_u, CHAR_t, CHAR_r, CHAR_o, CHAR_s};
WORD aNDSubtitle[13] = {7, CHAR_L, CHAR_e, CHAR_g, CHAR_e, CHAR_n, CHAR_d, CHAR_a};
WORD aNDOff[10] = {9, CHAR_D, CHAR_e, CHAR_s, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_d, CHAR_o};
WORD aNDWait[12] = {10, CHAR_A, CHAR_SPACE, CHAR_c, CHAR_a, CHAR_r, CHAR_r, CHAR_e, CHAR_g, CHAR_a, CHAR_r};
WORD aNDSelect[12] = {11, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_c, CHAR_i, CHAR_o, CHAR_n, CHAR_a, CHAR_r};
WORD aNDCardReader[29] = {28, CHAR_L, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_c, CHAR_a, CHAR_r, CHAR_t, CHAR_o4, CHAR_e, CHAR_s, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_m, CHAR_e, CHAR_m, CHAR_o1, CHAR_r, CHAR_i, CHAR_a};
#endif //OSDND_MAIN_FILE									

#ifdef OSDMM_MAIN_FILE									

WORD aMMProgramModeTitle[24] = {16, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_a};
WORD aMMProgramListModeTitle[21] = {18, CHAR_L, CHAR_i, CHAR_s, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_a, CHAR_s};
WORD aMMTotalFiles[25] = {19, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_f, CHAR_i, CHAR_c, CHAR_h, CHAR_e, CHAR_i, CHAR_r, CHAR_o, CHAR_s, CHAR_COLON};
WORD aMMFileNumber[20] = {19, CHAR_N, CHAR_u1, CHAR_m, CHAR_e, CHAR_r, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_f, CHAR_i, CHAR_c, CHAR_h, CHAR_e, CHAR_i, CHAR_r, CHAR_o, CHAR_COLON};
WORD aMMDlgFileTypeTitle[14] = {10, CHAR_T, CHAR_I, CHAR_P, CHAR_O, CHAR_SPACE, CHAR_F, CHAR_I, CHAR_C, CHAR_H, CHAR_COLON};
WORD aMMDlgSortFileTitle[16] = {10, CHAR_O, CHAR_R, CHAR_D, CHAR_FULL_STOP, CHAR_SPACE, CHAR_F, CHAR_I, CHAR_C, CHAR_H, CHAR_COLON};
WORD aMMDlgSortFileMethodTitle[9] = {7, CHAR_M, CHAR_e2, CHAR_t, CHAR_o, CHAR_d, CHAR_o, CHAR_COLON};
WORD aMMDlgSortFileSequenceTitle[12] = {10, CHAR_S, CHAR_e, CHAR_q, CHAR_u, CHAR_e3, CHAR_n, CHAR_c, CHAR_i, CHAR_a, CHAR_COLON};
WORD aMMDlgSequenceAscendingItem[10] = {7, CHAR_A, CHAR_s, CHAR_c, CHAR_e, CHAR_n, CHAR_d, CHAR_FULL_STOP};
WORD aMMDlgSequenceDescendingItem[12] = {8, CHAR_D, CHAR_e, CHAR_s, CHAR_c, CHAR_e, CHAR_n, CHAR_d, CHAR_FULL_STOP};
WORD aMMDlgMethodTypeItem[11] = {7, CHAR_P, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_t, CHAR_i, CHAR_p};
WORD aMMDlgMethodNameItem[11] = {7, CHAR_P, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_n, CHAR_o, CHAR_m};
WORD aMMDlgMethodTimeItem[10] = {8, CHAR_P, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_o, CHAR_r, CHAR_a};
WORD aMMHelpProgram[16] = {9, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_a, CHAR_r};
WORD aMMHelpProgramList[14] = {8, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a, CHAR_r};
WORD aMMBtnFileType[10] = {9, CHAR_T, CHAR_i, CHAR_p, CHAR_o, CHAR_SPACE, CHAR_f, CHAR_i, CHAR_c, CHAR_h};
WORD aMMBtnSortFile[12] = {9, CHAR_O, CHAR_r, CHAR_d, CHAR_FULL_STOP, CHAR_SPACE, CHAR_f, CHAR_i, CHAR_c, CHAR_h};
WORD aMMBtnList[7] = {5, CHAR_L, CHAR_i, CHAR_s, CHAR_t, CHAR_a};
WORD aMMBtnPlay[9] = {5, CHAR_R, CHAR_e, CHAR_p, CHAR_r, CHAR_SPACE};
WORD aMMBtnDeleteAll[11] = {10, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_FULL_STOP, CHAR_SPACE, CHAR_t, CHAR_u, CHAR_d, CHAR_o};
WORD aMMBtnReturn[8] = {6, CHAR_V, CHAR_o, CHAR_l, CHAR_t, CHAR_a, CHAR_r};
WORD aMMBtnExit[8] = {4, CHAR_S, CHAR_a, CHAR_i, CHAR_r};
#endif //OSDMM_MAIN_FILE									

#ifdef THUMB_MAIN_FILE									

WORD aTHUMBPage[8] = {7, CHAR_P, CHAR_a2, CHAR_g, CHAR_i, CHAR_n, CHAR_a, CHAR_COLON};
WORD aTHUMBTitleGeneralMode[19] = {14, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_T, CHAR_h, CHAR_u, CHAR_m, CHAR_b, CHAR_n, CHAR_a, CHAR_i, CHAR_l};
WORD aTHUMBTitleRotateMode[19] = {11, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_R, CHAR_o, CHAR_t, CHAR_a, CHAR_t, CHAR_e};
WORD aTHUMBTitleProgramMode[24] = {16, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_a};
WORD aTHUMBTitleProgramList[21] = {18, CHAR_L, CHAR_i, CHAR_s, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_a, CHAR_s};
WORD aTHUMBHelpRotateMode[17] = {5, CHAR_R, CHAR_o, CHAR_d, CHAR_a, CHAR_r};
WORD aTHUMBHelpProgramMode[16] = {9, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_a, CHAR_r};
WORD aTHUMBHelpProgramList[14] = {8, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a, CHAR_r};
WORD aTHUMBButtonRotate[8] = {5, CHAR_R, CHAR_o, CHAR_d, CHAR_a, CHAR_r};
WORD aTHUMBButtonSlideShow[11] = {10, CHAR_A, CHAR_p, CHAR_r, CHAR_FULL_STOP, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_a, CHAR_p, CHAR_FULL_STOP};
WORD aTHUMBButtonReset[8] = {5, CHAR_R, CHAR_e, CHAR_p, CHAR_o, CHAR_r};
WORD aTHUMBButtonReturn[8] = {6, CHAR_V, CHAR_o, CHAR_l, CHAR_t, CHAR_a, CHAR_r};
WORD aTHUMBButtonProgramList[7] = {5, CHAR_L, CHAR_i, CHAR_s, CHAR_t, CHAR_a};
#endif //THUMB_MAIN_FILE									

#ifdef OSDDG_MAIN_FILE									

WORD aDGDlgBtnTrackDigest[21] = {10, CHAR_R, CHAR_e, CHAR_s, CHAR_FULL_STOP, CHAR_SPACE, CHAR_F, CHAR_a, CHAR_i, CHAR_x, CHAR_a};
WORD aDGDlgBtnDiscInterval[21] = {13, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_FULL_STOP, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aDGPage[8] = {7, CHAR_P, CHAR_a2, CHAR_g, CHAR_i, CHAR_n, CHAR_a, CHAR_COLON};
WORD aDGTitleTrackDigest[21] = {10, CHAR_R, CHAR_e, CHAR_s, CHAR_FULL_STOP, CHAR_SPACE, CHAR_F, CHAR_a, CHAR_i, CHAR_x, CHAR_a};
WORD aDGTitleDiscInterval[21] = {13, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_FULL_STOP, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aDGInfoTrack[9] = {6, CHAR_F, CHAR_a, CHAR_i, CHAR_x, CHAR_a, CHAR_COLON};
WORD aDGBtnMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aDGBtnExit[8] = {4, CHAR_S, CHAR_a, CHAR_i, CHAR_r};
#endif //OSDDG_MAIN_FILE									

#ifdef OSDPRGM_MAIN_FILE									

WORD aProgram[12] = {8, CHAR_P, CHAR_R, CHAR_O, CHAR_G, CHAR_R, CHAR_A, CHAR_M, CHAR_A};
WORD aTitle[7] = {6, CHAR_T, CHAR_i1, CHAR_t, CHAR_u, CHAR_l, CHAR_o};
WORD aChapter[10] = {8, CHAR_C, CHAR_a, CHAR_p, CHAR_i1, CHAR_t, CHAR_u, CHAR_l, CHAR_o};
WORD aTrack[8] = {5, CHAR_F, CHAR_a, CHAR_i, CHAR_x, CHAR_a};
WORD aSelect[12] = {11, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_c, CHAR_i, CHAR_o, CHAR_n, CHAR_a, CHAR_r};
WORD aPlay[11] = {10, CHAR_R, CHAR_e, CHAR_p, CHAR_r, CHAR_o, CHAR_d, CHAR_u, CHAR_z, CHAR_i, CHAR_r};
WORD aClear[8] = {5, CHAR_L, CHAR_i, CHAR_m, CHAR_p, CHAR_FULL_STOP};
#endif //OSDPRGM_MAIN_FILE									

#ifdef OSDBMARK_MAIN_FILE									

WORD aBookmark[11] = {8, CHAR_M, CHAR_a, CHAR_r, CHAR_c, CHAR_a, CHAR_d, CHAR_o, CHAR_r};
WORD aTime[6] = {4, CHAR_H, CHAR_o, CHAR_r, CHAR_a};
#endif //OSDBMARK_MAIN_FILE									

#ifdef SETUP_MAIN_FILE									

WORD aSetupSelect[12] = {11, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_c, CHAR_i, CHAR_o, CHAR_n, CHAR_a, CHAR_r};
#endif //SETUP_MAIN_FILE									

#ifdef MENU_MAIN_FILE									

WORD aMsgEnglish[10] = {6, CHAR_I, CHAR_n, CHAR_g, CHAR_l, CHAR_e3, CHAR_s};
WORD aMsgChinese[11] = {6, CHAR_C, CHAR_h, CHAR_i2, CHAR_n, CHAR_e3, CHAR_s};
WORD aMsgJapanese[11] = {7, CHAR_J, CHAR_a, CHAR_p, CHAR_o, CHAR_n, CHAR_e3, CHAR_s};
WORD aMsgFrench[12] = {7, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_e3, CHAR_s};
WORD aMsgGerman[10] = {6, CHAR_A, CHAR_l, CHAR_e, CHAR_m, CHAR_a4, CHAR_o};
WORD aMsgItalian[12] = {8, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n, CHAR_o};
WORD aMsgSpanish[11] = {8, CHAR_E, CHAR_s, CHAR_p, CHAR_a, CHAR_n, CHAR_h, CHAR_o, CHAR_l};
WORD aMsgPortuguese[14] = {9, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_u, CHAR_e3, CHAR_s};
WORD aMsgDutch[12] = {8, CHAR_H, CHAR_o, CHAR_l, CHAR_a, CHAR_n, CHAR_d, CHAR_e3, CHAR_s};
WORD aMsgPolish[9] = {6, CHAR_P, CHAR_o, CHAR_l, CHAR_a, CHAR_c, CHAR_o};
WORD aMsgHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aMsgThai[10] = {9, CHAR_T, CHAR_a, CHAR_i, CHAR_l, CHAR_a, CHAR_n, CHAR_d, CHAR_e3, CHAR_s};
WORD aMsgRussian[9] = {5, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_o};
WORD aMsgKorean[11] = {7, CHAR_C, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_o};
WORD aMsgCzech[6] = {5, CHAR_C, CHAR_z, CHAR_e, CHAR_c, CHAR_h};
WORD aMsgGreece[7] = {6, CHAR_G, CHAR_r, CHAR_e, CHAR_e, CHAR_c, CHAR_e};
WORD aMsgTurkey[7] = {6, CHAR_T, CHAR_u, CHAR_r, CHAR_k, CHAR_e, CHAR_y};
WORD aMsgOthers[9] = {6, CHAR_O, CHAR_u, CHAR_t, CHAR_r, CHAR_o, CHAR_s};
WORD aMsgOSD[4] = {3, CHAR_O, CHAR_S, CHAR_D};
WORD aMsgMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aMsgAudio[6] = {5, CHAR_A2, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMsgSubtitle[12] = {7, CHAR_L, CHAR_e, CHAR_g, CHAR_e, CHAR_n, CHAR_d, CHAR_a};
WORD aMsgPS[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_P, CHAR_S};
WORD aMsgLB[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_L, CHAR_B};
WORD aMsgWide[5] = {4, CHAR_1, CHAR_6, CHAR_COLON, CHAR_9};
WORD aMsgOn[9] = {6, CHAR_L, CHAR_i, CHAR_g, CHAR_a, CHAR_d, CHAR_o};
WORD aMsgOff[10] = {8, CHAR_D, CHAR_e, CHAR_s, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_d};
WORD aMsgNTSC[5] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aMsgPAL[4] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aMsgAuto[11] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aMsgPAL_M[6] = {5, CHAR_P, CHAR_A, CHAR_L, CHAR_LOW_LINE, CHAR_M};
WORD aMsgPAL_N[6] = {5, CHAR_P, CHAR_A, CHAR_L, CHAR_LOW_LINE, CHAR_N};
WORD aMsgNormal[7] = {6, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l};
WORD aMsgVideoNormal[13] = {12, CHAR_C, CHAR_V, CHAR_B, CHAR_S, CHAR_PLUS_SIGN, CHAR_S, CHAR_HYPHEN_MINUS, CHAR_V, CHAR_i, CHAR_d, CHAR_e, CHAR_o};
WORD aMsgSCARTRGB[6] = {5, CHAR_S, CHAR_C, CHAR_A, CHAR_R, CHAR_T};
WORD aMsgRGB[9] = {8, CHAR_C, CHAR_V, CHAR_B, CHAR_S, CHAR_PLUS_SIGN, CHAR_R, CHAR_G, CHAR_B};
WORD aMsgYUV[9] = {8, CHAR_C, CHAR_V, CHAR_B, CHAR_S, CHAR_PLUS_SIGN, CHAR_Y, CHAR_U, CHAR_V};
WORD aMsgYUVPScan[11] = {10, CHAR_Y, CHAR_U, CHAR_V, CHAR_SPACE, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aMsgRGBPScan[11] = {10, CHAR_R, CHAR_G, CHAR_B, CHAR_SPACE, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aMsgVGAPScan[11] = {10, CHAR_V, CHAR_G, CHAR_A, CHAR_SPACE, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aMsgBriNormal[9] = {6, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l};
WORD aMsgBright[10] = {9, CHAR_B, CHAR_r, CHAR_i, CHAR_l, CHAR_h, CHAR_a, CHAR_n, CHAR_t, CHAR_e};
WORD aMsgSoft[8] = {5, CHAR_S, CHAR_u, CHAR_a, CHAR_v, CHAR_e};
WORD aMsgMinus6[3] = {2, CHAR_HYPHEN_MINUS, CHAR_6};
WORD aMsgMinus5[3] = {2, CHAR_HYPHEN_MINUS, CHAR_5};
WORD aMsgMinus4[3] = {2, CHAR_HYPHEN_MINUS, CHAR_4};
WORD aMsgMinus3[3] = {2, CHAR_HYPHEN_MINUS, CHAR_3};
WORD aMsgMinus2[3] = {2, CHAR_HYPHEN_MINUS, CHAR_2};
WORD aMsgMinus1[3] = {2, CHAR_HYPHEN_MINUS, CHAR_1};
WORD aMsg0[2] = {1, CHAR_0};
WORD aMsgPlus1[3] = {2, CHAR_PLUS_SIGN, CHAR_1};
WORD aMsgPlus2[3] = {2, CHAR_PLUS_SIGN, CHAR_2};
WORD aMsgPlus3[3] = {2, CHAR_PLUS_SIGN, CHAR_3};
WORD aMsgPlus4[3] = {2, CHAR_PLUS_SIGN, CHAR_4};
WORD aMsgPlus5[3] = {2, CHAR_PLUS_SIGN, CHAR_5};
WORD aMsgPlus6[3] = {2, CHAR_PLUS_SIGN, CHAR_6};
WORD aMsgTVType[14] = {10, CHAR_T, CHAR_i, CHAR_p, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMsgScreenSaver[18] = {14, CHAR_P, CHAR_r, CHAR_o, CHAR_t, CHAR_e, CHAR_c, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_e, CHAR_c, CHAR_r, CHAR_a4};
WORD aMsgTVSystem[17] = {13, CHAR_S, CHAR_i, CHAR_s, CHAR_t, CHAR_e, CHAR_m, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMsgVideoOutput[14] = {11, CHAR_S, CHAR_a, CHAR_i1, CHAR_d, CHAR_a, CHAR_SPACE, CHAR_V, CHAR_i1, CHAR_d, CHAR_e, CHAR_o};
WORD aMsgBrightness[11] = {6, CHAR_B, CHAR_r, CHAR_i, CHAR_l, CHAR_h, CHAR_o};
WORD aMsgContrast[10] = {9, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t, CHAR_e};
WORD aMsgChromPhase[4] = {3, CHAR_H, CHAR_u, CHAR_e};
WORD aMsgSPDIF_PCM[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_P, CHAR_C, CHAR_M};
WORD aMsgSPDIF_RAW[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_R, CHAR_A, CHAR_W};
WORD aMsg2CH[11] = {7, CHAR_C, CHAR_a, CHAR_n, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_2};
WORD aMsg51CH[13] = {9, CHAR_C, CHAR_a, CHAR_n, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_5, CHAR_FULL_STOP, CHAR_1};
WORD aMsgLoRo[6] = {5, CHAR_L, CHAR_o, CHAR_SOLIDUS, CHAR_R, CHAR_o};
WORD aMsgLtRt[6] = {5, CHAR_L, CHAR_t, CHAR_SOLIDUS, CHAR_R, CHAR_t};
#ifndef REMOVE_SETUP_SPEAKER_TEST									

WORD aMsgAutoTest[5] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aMsgLeft[6] = {4, CHAR_L, CHAR_e, CHAR_f, CHAR_t};
WORD aMsgRight[7] = {5, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aMsgCntr[10] = {6, CHAR_C, CHAR_e, CHAR_n, CHAR_t, CHAR_e, CHAR_r};
WORD aMsgLS[15] = {9, CHAR_L, CHAR_e, CHAR_f, CHAR_t, CHAR_SPACE, CHAR_S, CHAR_u, CHAR_r, CHAR_r};
WORD aMsgRS[16] = {10, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t, CHAR_SPACE, CHAR_S, CHAR_u, CHAR_r, CHAR_r};
WORD aMsgSW[10] = {9, CHAR_S, CHAR_u, CHAR_b, CHAR_w, CHAR_o, CHAR_o, CHAR_f, CHAR_e, CHAR_r};
#endif //REMOVE_SETUP_SPEAKER_TEST									

WORD aMsgDigitalOut[18] = {13, CHAR_S, CHAR_a, CHAR_i1, CHAR_d, CHAR_a, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_g, CHAR_i, CHAR_t, CHAR_a, CHAR_l};
WORD aMsgAnalogOut[18] = {15, CHAR_S, CHAR_a, CHAR_i1, CHAR_d, CHAR_a, CHAR_SPACE, CHAR_A, CHAR_n, CHAR_a, CHAR_l, CHAR_o1, CHAR_g, CHAR_i, CHAR_c, CHAR_a};
WORD aMsgDNR[4] = {3, CHAR_D, CHAR_N, CHAR_R};
WORD aMsgDNR_0[2] = {1, CHAR_0};
WORD aMsgDNR_1[2] = {1, CHAR_1};
WORD aMsgDNR_2[2] = {1, CHAR_2};
WORD aMsgDNR_3[2] = {1, CHAR_3};
WORD aMsgDNR_4[2] = {1, CHAR_4};
WORD aMsgDNR_5[2] = {1, CHAR_5};
WORD aMsgDNR_6[2] = {1, CHAR_6};
WORD aMsgDNR_7[2] = {1, CHAR_7};
WORD aMsgDNR_8[2] = {1, CHAR_8};
WORD aMsgVirtual[10] = {7, CHAR_V, CHAR_i, CHAR_r, CHAR_t, CHAR_u, CHAR_a, CHAR_l};
WORD aMsgMusic[7] = {5, CHAR_M, CHAR_u, CHAR_s, CHAR_i, CHAR_c};
WORD aMsgMovie[6] = {5, CHAR_M, CHAR_o, CHAR_v, CHAR_i, CHAR_e};
WORD aMsgMatrix[8] = {6, CHAR_M, CHAR_a, CHAR_t, CHAR_r, CHAR_i, CHAR_x};
WORD aMsgPrologic[10] = {9, CHAR_P, CHAR_R, CHAR_O, CHAR_SPACE, CHAR_L, CHAR_O, CHAR_G, CHAR_I, CHAR_C};
WORD aMsgDownMix[8] = {7, CHAR_D, CHAR_o, CHAR_w, CHAR_n, CHAR_m, CHAR_i, CHAR_x};
WORD aMsgCenter[10] = {6, CHAR_C, CHAR_e, CHAR_n, CHAR_t, CHAR_r, CHAR_o};
WORD aMsgRear[11] = {8, CHAR_T, CHAR_r, CHAR_a, CHAR_s, CHAR_e, CHAR_i, CHAR_r, CHAR_a};
WORD aMsgSubwoofer[10] = {9, CHAR_S, CHAR_u, CHAR_b, CHAR_w, CHAR_o, CHAR_o, CHAR_f, CHAR_e, CHAR_r};
WORD aMsgSpeakerTest[19] = {18, CHAR_T, CHAR_e, CHAR_s, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_s, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_t, CHAR_i, CHAR_f, CHAR_a, CHAR_l, CHAR_FULL_STOP};
WORD aMsgLevel1[2] = {1, CHAR_1};
WORD aMsgLevel2[2] = {1, CHAR_2};
WORD aMsgLevel3[2] = {1, CHAR_3};
WORD aMsgLevel4[2] = {1, CHAR_4};
WORD aMsgLevel5[2] = {1, CHAR_5};
WORD aMsgLevel6[2] = {1, CHAR_6};
WORD aMsgLevel7[2] = {1, CHAR_7};
WORD aMsgLevel8[2] = {1, CHAR_8};
WORD aMsgRead[8] = {3, CHAR_L, CHAR_e, CHAR_r};
WORD aMsgParental[20] = {13, CHAR_P, CHAR_a, CHAR_r, CHAR_e, CHAR_n, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_C, CHAR_t, CHAR_r, CHAR_l};
WORD aMsgChangePassword[13] = {8, CHAR_P, CHAR_a, CHAR_s, CHAR_s, CHAR_w, CHAR_o, CHAR_r, CHAR_d};
WORD aMsgAngleMark[16] = {15, CHAR_M, CHAR_a, CHAR_r, CHAR_c, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_a3, CHAR_n, CHAR_g, CHAR_u, CHAR_l, CHAR_o};
WORD aMsgUIStyle[7] = {6, CHAR_E, CHAR_s, CHAR_t, CHAR_i, CHAR_l, CHAR_o};
WORD aMsgUIStyle1[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_1};
WORD aMsgUIStyle2[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_2};
WORD aMsgUIStyle3[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_3};
WORD aMsgUIStyle4[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_4};
WORD aMsgSmartPlay[14] = {10, CHAR_S, CHAR_m, CHAR_a, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aMsgDivXVOD[9] = {8, CHAR_D, CHAR_i, CHAR_v, CHAR_X, CHAR_SPACE, CHAR_V, CHAR_O, CHAR_D};
WORD aMsgDefaultValue[18] = {17, CHAR_O, CHAR_p, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_e, CHAR_d, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_d, CHAR_a};
WORD aMsgLanguageSetup[23] = {22, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_g, CHAR_u, CHAR_r, CHAR_a, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_i, CHAR_d, CHAR_i, CHAR_o, CHAR_m, CHAR_a};
WORD aMsgScreenSetup[22] = {20, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_g, CHAR_u, CHAR_r, CHAR_a, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_e, CHAR_c, CHAR_r, CHAR_a4};
WORD aMsgAudioSetup[22] = {21, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_g, CHAR_u, CHAR_r, CHAR_a, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_a2, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMsgCustomSetup[28] = {26, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_g, CHAR_u, CHAR_r, CHAR_a, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_p, CHAR_e, CHAR_r, CHAR_s, CHAR_o, CHAR_n, CHAR_a, CHAR_l, CHAR_i, CHAR_z, CHAR_a, CHAR_d, CHAR_a};
WORD aMsgExitSetup[30] = {18, CHAR_S, CHAR_a, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p};
WORD aMsgPassword[31] = {19, CHAR_I, CHAR_n, CHAR_t, CHAR_r, CHAR_o, CHAR_d, CHAR_u, CHAR_z, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_s, CHAR_s, CHAR_w, CHAR_o, CHAR_r, CHAR_d};
WORD aMsgWrongPassword[20] = {15, CHAR_P, CHAR_a, CHAR_s, CHAR_s, CHAR_w, CHAR_o, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_e, CHAR_r, CHAR_r, CHAR_a, CHAR_d, CHAR_a};
WORD aMsgOldPassword[9] = {6, CHAR_A, CHAR_n, CHAR_t, CHAR_i, CHAR_g, CHAR_a};
WORD aMsgNewPassword[10] = {5, CHAR_N, CHAR_o, CHAR_v, CHAR_a, CHAR_COLON};
WORD aMsgVerifyPassword[12] = {9, CHAR_V, CHAR_e, CHAR_r, CHAR_i, CHAR_f, CHAR_i, CHAR_c, CHAR_a, CHAR_r};
WORD aMsgStandard[12] = {8, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_r, CHAR_d};
WORD aMsgClassical[10] = {9, CHAR_C, CHAR_l, CHAR_a, CHAR_s, CHAR_s, CHAR_i, CHAR_c, CHAR_a, CHAR_l};
WORD aMsgRock[5] = {4, CHAR_R, CHAR_o, CHAR_c, CHAR_k};
WORD aMsgJazz[5] = {4, CHAR_J, CHAR_a, CHAR_z, CHAR_z};
WORD aMsgAcoustic[11] = {8, CHAR_A, CHAR_c, CHAR_o, CHAR_u, CHAR_s, CHAR_t, CHAR_i, CHAR_c};
WORD aMsgPop[4] = {3, CHAR_P, CHAR_o, CHAR_p};
WORD aMsgCustom[7] = {6, CHAR_C, CHAR_u, CHAR_s, CHAR_t, CHAR_o, CHAR_m};
#ifdef SUPPORT_PSCAN_PROMPT_SETUP									

WORD aMsgPleaseMake[39] = {23, CHAR_Y, CHAR_o, CHAR_u, CHAR_r, CHAR_SPACE, CHAR_T, CHAR_V, CHAR_SPACE, CHAR_S, CHAR_u, CHAR_p, CHAR_p, CHAR_o, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_c, CHAR_a, CHAR_n, CHAR_QUESTION_MARK};
#endif SUPPORT_PSCAN_PROMPT_SETUP									

#endif //MENU_MAIN_FILE									

#ifdef IMAGE_FRAME_MENU_MAIN_FILE									

WORD aMENUPhotoSetting[26] = {8, CHAR_D, CHAR_e, CHAR_f, CHAR_SPACE, CHAR_f, CHAR_o, CHAR_t, CHAR_o};
WORD aMENUIntervalTime[20] = {12, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_SPACE, CHAR_t, CHAR_e, CHAR_m, CHAR_p, CHAR_o};
#ifndef MUSTEK_SETUP_MENU									

WORD aMENU5Sec[7] = {6, CHAR_5, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_g, CHAR_FULL_STOP};
WORD aMENU10Sec[8] = {7, CHAR_1, CHAR_0, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_g, CHAR_FULL_STOP};
#endif //MUSTEK_SETUP_MENU									

WORD aMENU15Sec[8] = {7, CHAR_1, CHAR_5, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_g, CHAR_FULL_STOP};
WORD aMENU30Sec[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_g, CHAR_FULL_STOP};
WORD aMENU1Min[7] = {6, CHAR_1, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU3Min[7] = {6, CHAR_3, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENUTransitionEffect[21] = {16, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_t, CHAR_r, CHAR_a, CHAR_n, CHAR_s, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o};
WORD aMENUJPEGEffectNone[12] = {10, CHAR_S, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_e, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o};
WORD aMENUJPEGEffectCoverUp[17] = {15, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aMENUJPEGEffectCoverDown[18] = {17, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
#ifndef MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectCoverLeft[16] = {10, CHAR_C, CHAR_o, CHAR_b, CHAR_r, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_e, CHAR_s, CHAR_q};
WORD aMENUJPEGEffectCoverRight[17] = {11, CHAR_C, CHAR_o, CHAR_b, CHAR_r, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_r, CHAR_FULL_STOP};
WORD aMENUJPEGEffectUncoverUp[18] = {17, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aMENUJPEGEffectUncoverDown[17] = {12, CHAR_E, CHAR_f, CHAR_e, CHAR_SPACE, CHAR_U, CHAR_n, CHAR_c, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
#endif //MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectUncoverLeft[16] = {10, CHAR_D, CHAR_e, CHAR_s, CHAR_c, CHAR_FULL_STOP, CHAR_SPACE, CHAR_e, CHAR_s, CHAR_q, CHAR_FULL_STOP};
WORD aMENUJPEGEffectUncoverRight[17] = {10, CHAR_D, CHAR_e, CHAR_s, CHAR_c, CHAR_FULL_STOP, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_r, CHAR_FULL_STOP};
WORD aMENUJPEGEffectWipeUp[20] = {14, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aMENUJPEGEffectWipeDown[17] = {16, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
#ifndef MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectWipeLeft[17] = {9, CHAR_L, CHAR_i, CHAR_m, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_e, CHAR_s, CHAR_q};
WORD aMENUJPEGEffectWipeRight[18] = {9, CHAR_L, CHAR_i, CHAR_m, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_r};
WORD aMENUJPEGEffectPushUp[14] = {5, CHAR_S, CHAR_u, CHAR_b, CHAR_i, CHAR_r};
WORD aMENUJPEGEffectPushDown[13] = {6, CHAR_D, CHAR_e, CHAR_s, CHAR_c, CHAR_e, CHAR_r};
WORD aMENUJPEGEffectPushLeft[17] = {8, CHAR_E, CHAR_s, CHAR_q, CHAR_u, CHAR_e, CHAR_r, CHAR_d, CHAR_a};
#endif //MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectPushRight[18] = {7, CHAR_D, CHAR_i, CHAR_r, CHAR_e, CHAR_i, CHAR_t, CHAR_a};
WORD aMENUJPEGEffectFadeThroughBlack[26] = {25, CHAR_E, CHAR_f, CHAR_e, CHAR_i, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_F, CHAR_a, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_h, CHAR_r, CHAR_o, CHAR_u, CHAR_g, CHAR_h, CHAR_SPACE, CHAR_B, CHAR_l, CHAR_a, CHAR_c, CHAR_k};
WORD aMENUJPEGEffectRandom[16] = {11, CHAR_R, CHAR_e, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_e, CHAR_a, CHAR_t, CHAR_FULL_STOP};
WORD aMENUBrowse[12] = {6, CHAR_P, CHAR_r, CHAR_o, CHAR_c, CHAR_u, CHAR_r};
WORD aMENUFileManager[16] = {11, CHAR_G, CHAR_e, CHAR_s, CHAR_t, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_f, CHAR_i, CHAR_c, CHAR_h};
WORD aMENUSetupMenu[16] = {9, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_c, CHAR_o, CHAR_n, CHAR_f};
WORD aMENUThumbnail[13] = {9, CHAR_M, CHAR_i, CHAR_n, CHAR_i, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_a};
WORD aMENUAutoPlay[14] = {8, CHAR_R, CHAR_e, CHAR_p, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_t, CHAR_o};
WORD aMENUPhoto[12] = {4, CHAR_F, CHAR_o, CHAR_t, CHAR_o};
WORD aMENUMusic[7] = {5, CHAR_M, CHAR_u1, CHAR_s, CHAR_i, CHAR_c};
WORD aMENUPhotoAudio[14] = {10, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_SOLIDUS, CHAR_A2, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMENUMovie[10] = {5, CHAR_V, CHAR_i1, CHAR_d, CHAR_e, CHAR_o};
WORD aMENUDisplaySetup[16] = {12, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_SPACE, CHAR_v, CHAR_i, CHAR_s, CHAR_u, CHAR_a, CHAR_l, CHAR_FULL_STOP};
WORD aMENUDisplayType[14] = {12, CHAR_F, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_e, CHAR_c, CHAR_r, CHAR_a4};
WORD aMENUTVStandard[12] = {8, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMENUNTSC[5] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aMENUPAL[4] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aMENUAUTO[11] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aMENUBrightness[11] = {6, CHAR_B, CHAR_r, CHAR_i, CHAR_l, CHAR_h, CHAR_o};
WORD aMENUContrast[10] = {9, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t, CHAR_e};
WORD aMENUNormal[9] = {6, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l};
WORD aMENUBright[10] = {9, CHAR_B, CHAR_r, CHAR_i, CHAR_l, CHAR_h, CHAR_a, CHAR_n, CHAR_t, CHAR_e};
WORD aMENUSoft[8] = {5, CHAR_S, CHAR_u, CHAR_a, CHAR_v, CHAR_e};
WORD aMENUTFTBrightness[15] = {10, CHAR_B, CHAR_r, CHAR_i, CHAR_l, CHAR_h, CHAR_o, CHAR_SPACE, CHAR_T, CHAR_F, CHAR_T};
WORD aMENUTFTContrast[14] = {12, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t, CHAR_SPACE, CHAR_T, CHAR_F, CHAR_T};
WORD aMENUTFTSaturation[16] = {13, CHAR_S, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_a, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_T, CHAR_F, CHAR_T};
WORD aMENURepeatMode[15] = {10, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_p, CHAR_e, CHAR_t};
WORD aMENURepeatOne[9] = {2, CHAR_U, CHAR_m};
WORD aMENURepeatFolder[9] = {5, CHAR_p, CHAR_a, CHAR_s, CHAR_t, CHAR_a};
WORD aMENURepeatAll[10] = {4, CHAR_T, CHAR_u, CHAR_d, CHAR_o};
WORD aMENUCustomSetup[28] = {26, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_g, CHAR_u, CHAR_r, CHAR_a, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_p, CHAR_e, CHAR_r, CHAR_s, CHAR_o, CHAR_n, CHAR_a, CHAR_l, CHAR_i, CHAR_z, CHAR_a, CHAR_d, CHAR_a};
WORD aMENUOSDLanguage[14] = {6, CHAR_I, CHAR_d, CHAR_i, CHAR_o, CHAR_m, CHAR_a};
WORD aMENUEnglish[10] = {6, CHAR_I, CHAR_n, CHAR_g, CHAR_l, CHAR_e3, CHAR_s};
WORD aMENUChinese[11] = {6, CHAR_C, CHAR_h, CHAR_i2, CHAR_n, CHAR_e3, CHAR_s};
WORD aMENUJapanese[11] = {7, CHAR_J, CHAR_a, CHAR_p, CHAR_o, CHAR_n, CHAR_e3, CHAR_s};
WORD aMENUKorean[11] = {7, CHAR_C, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_o};
WORD aMENUFrench[12] = {7, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_e3, CHAR_s};
WORD aMENUGerman[10] = {6, CHAR_A, CHAR_l, CHAR_e, CHAR_m, CHAR_a4, CHAR_o};
WORD aMENUItalian[12] = {8, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n, CHAR_o};
WORD aMENUSpanish[11] = {8, CHAR_E, CHAR_s, CHAR_p, CHAR_a, CHAR_n, CHAR_h, CHAR_o, CHAR_l};
WORD aMENUPortuguese[14] = {9, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_u, CHAR_e3, CHAR_s};
WORD aMENUDutch[12] = {8, CHAR_H, CHAR_o, CHAR_l, CHAR_a, CHAR_n, CHAR_d, CHAR_e3, CHAR_s};
WORD aMENUPolish[9] = {6, CHAR_P, CHAR_o, CHAR_l, CHAR_a, CHAR_c, CHAR_o};
WORD aMENUUIStyle[7] = {6, CHAR_E, CHAR_s, CHAR_t, CHAR_i, CHAR_l, CHAR_o};
WORD aMENUUIStyle1[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_1};
WORD aMENUUIStyle2[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_2};
WORD aMENUUIStyle3[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_3};
WORD aMENUUIStyle4[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_4};
WORD aMENUDefault[18] = {17, CHAR_O, CHAR_p, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_e, CHAR_d, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_d, CHAR_a};
WORD aMENURead[8] = {3, CHAR_L, CHAR_e, CHAR_r};
WORD aMENUIconExit[6] = {4, CHAR_S, CHAR_a, CHAR_i, CHAR_r};
WORD aMENUHelpSelect[10] = {8, CHAR_SPACE, CHAR_COLON, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c};
WORD aMENUHelpConfirm[11] = {10, CHAR_SPACE, CHAR_COLON, CHAR_SPACE, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_r, CHAR_FULL_STOP};
WORD aMENUHelpExit[18] = {16, CHAR_S, CHAR_a, CHAR_i, CHAR_r, CHAR_COLON, CHAR_G, CHAR_u, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_e, CHAR_SPACE, CHAR_s, CHAR_a, CHAR_i, CHAR_r};
#endif //#ifdef IMAGE_FRAME_MENU_MAIN_FILE									

#ifdef SUPPORT_POWERON_MENU									

WORD aPOWERONIconPhoto[8] = {4, CHAR_F, CHAR_o, CHAR_t, CHAR_o};
WORD aPOWERONIconMusic[7] = {5, CHAR_M, CHAR_u1, CHAR_s, CHAR_i, CHAR_c};
WORD aPOWERONIconPhotoMusic[15] = {10, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_PLUS_SIGN, CHAR_M, CHAR_u1, CHAR_s, CHAR_i, CHAR_c};
WORD aPOWERONIconMovie[10] = {5, CHAR_V, CHAR_i1, CHAR_d, CHAR_e, CHAR_o};
WORD aPOWERONIconSetup[14] = {4, CHAR_C, CHAR_o, CHAR_n, CHAR_f};
WORD aPOWERONIconCalendar[11] = {6, CHAR_C, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d};
WORD aPOWERONIconClock[9] = {5, CHAR_R, CHAR_e, CHAR_l, CHAR_o1, CHAR_g};
WORD aPOWERONIconAlarm[7] = {5, CHAR_A, CHAR_l, CHAR_e, CHAR_r, CHAR_t};
WORD aPOWERONIconAutoPower[14] = {10, CHAR_L, CHAR_i, CHAR_g, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_t, CHAR_o};
WORD aPOWERONIconEdit[11] = {6, CHAR_E, CHAR_d, CHAR_i, CHAR_t, CHAR_a, CHAR_r};
WORD aPOWERONIconFavorite[10] = {7, CHAR_F, CHAR_a, CHAR_v, CHAR_o, CHAR_r, CHAR_i, CHAR_t};
WORD aPOWERONIconRadio[6] = {5, CHAR_R, CHAR_a2, CHAR_d, CHAR_i, CHAR_o};
WORD aWeeklyDay_Mon[4] = {3, CHAR_S, CHAR_e, CHAR_g};
WORD aWeeklyDay_Tue[4] = {3, CHAR_T, CHAR_e, CHAR_r};
WORD aWeeklyDay_Wed[4] = {3, CHAR_Q, CHAR_u, CHAR_a};
WORD aWeeklyDay_Thu[4] = {3, CHAR_Q, CHAR_u, CHAR_i};
WORD aWeeklyDay_Fri[4] = {3, CHAR_S, CHAR_e, CHAR_x};
WORD aWeeklyDay_Sat[4] = {3, CHAR_S, CHAR_a2, CHAR_b};
WORD aWeeklyDay_Sun[4] = {3, CHAR_D, CHAR_o, CHAR_m};
WORD aMonthJanuary[4] = {3, CHAR_J, CHAR_a, CHAR_n};
WORD aMonthFebruary[4] = {3, CHAR_F, CHAR_e, CHAR_v};
WORD aMonthMarch[4] = {3, CHAR_M, CHAR_a, CHAR_r};
WORD aMonthApril[4] = {3, CHAR_A, CHAR_b, CHAR_r};
WORD aMonthMay[4] = {3, CHAR_M, CHAR_a, CHAR_i};
WORD aMonthJune[4] = {3, CHAR_J, CHAR_u, CHAR_n};
WORD aMonthJuly[4] = {3, CHAR_J, CHAR_u, CHAR_l};
WORD aMonthAugust[4] = {3, CHAR_A, CHAR_g, CHAR_o};
WORD aMonthSeptember[4] = {3, CHAR_S, CHAR_e, CHAR_t};
WORD aMonthOctober[4] = {3, CHAR_O, CHAR_u, CHAR_t};
WORD aMonthNovember[4] = {3, CHAR_N, CHAR_o, CHAR_v};
WORD aMonthDecember[4] = {3, CHAR_D, CHAR_e, CHAR_z};
WORD aWeeklyMonday[14] = {13, CHAR_S, CHAR_e, CHAR_g, CHAR_u, CHAR_n, CHAR_d, CHAR_a, CHAR_HYPHEN_MINUS, CHAR_f, CHAR_e, CHAR_i, CHAR_r, CHAR_a};
WORD aWeeklyTuesday[12] = {11, CHAR_T, CHAR_e, CHAR_r, CHAR_c1, CHAR_a, CHAR_HYPHEN_MINUS, CHAR_f, CHAR_e, CHAR_i, CHAR_r, CHAR_a};
WORD aWeeklyWednesday[13] = {12, CHAR_Q, CHAR_u, CHAR_a, CHAR_r, CHAR_t, CHAR_a, CHAR_HYPHEN_MINUS, CHAR_f, CHAR_e, CHAR_i, CHAR_r, CHAR_a};
WORD aWeeklyThursday[13] = {12, CHAR_Q, CHAR_u, CHAR_i, CHAR_n, CHAR_t, CHAR_a, CHAR_HYPHEN_MINUS, CHAR_f, CHAR_e, CHAR_i, CHAR_r, CHAR_a};
WORD aWeeklyFriday[12] = {11, CHAR_S, CHAR_e, CHAR_x, CHAR_t, CHAR_a, CHAR_HYPHEN_MINUS, CHAR_f, CHAR_e, CHAR_i, CHAR_r, CHAR_a};
WORD aWeeklySaturday[9] = {6, CHAR_S, CHAR_a2, CHAR_b, CHAR_a, CHAR_d, CHAR_o};
WORD aWeeklySunday[10] = {7, CHAR_D, CHAR_o, CHAR_m, CHAR_i, CHAR_n, CHAR_g, CHAR_o};
WORD aCLOCKClockSetting[22] = {20, CHAR_D, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_l, CHAR_o1, CHAR_g, CHAR_i, CHAR_o};
WORD aCLOCKDisplay[13] = {12, CHAR_V, CHAR_i, CHAR_s, CHAR_u, CHAR_a, CHAR_l, CHAR_i, CHAR_z, CHAR_a, CHAR_c1, CHAR_a4, CHAR_o};
WORD aCLOCKSetTime[14] = {12, CHAR_D, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_o, CHAR_r, CHAR_a};
WORD aCLOCKExit[6] = {4, CHAR_S, CHAR_a, CHAR_i, CHAR_r};
WORD aCLOCKBigClock[9] = {7, CHAR_R, CHAR_e, CHAR_l, CHAR_o1, CHAR_g, CHAR_i, CHAR_o};
WORD aCLOCKClockCalendar[22] = {20, CHAR_R, CHAR_e, CHAR_l, CHAR_o1, CHAR_g, CHAR_i, CHAR_o, CHAR_SPACE, CHAR_e, CHAR_SPACE, CHAR_c, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_a2, CHAR_r, CHAR_i, CHAR_o};
WORD aBUTTONOK[3] = {2, CHAR_O, CHAR_K};
WORD aBUTTONCancel[9] = {6, CHAR_C, CHAR_a, CHAR_n, CHAR_c, CHAR_e, CHAR_l};
WORD aSETTIMESetTime[15] = {12, CHAR_D, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_o, CHAR_r, CHAR_a};
WORD aALARMEnable[10] = {7, CHAR_A, CHAR_c, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_r};
WORD aALARMAlarmSetting[21] = {19, CHAR_D, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_e, CHAR_r, CHAR_t, CHAR_a};
WORD aALARMDlgTitle[8] = {6, CHAR_A, CHAR_l, CHAR_e, CHAR_r, CHAR_t, CHAR_a};
WORD aALARMDlgContent[34] = {28, CHAR_P, CHAR_r, CHAR_i, CHAR_m, CHAR_a, CHAR_SPACE, CHAR_q, CHAR_l, CHAR_q, CHAR_r, CHAR_SPACE, CHAR_t, CHAR_c, CHAR_l, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_r, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_e, CHAR_r, CHAR_t, CHAR_a, CHAR_FULL_STOP};
WORD aAUTOPWRAutoPowerSetting[46] = {45, CHAR_D, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_r, CHAR_SOLIDUS, CHAR_d, CHAR_e, CHAR_s, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_t, CHAR_o, CHAR_m, CHAR_a, CHAR_t, CHAR_i, CHAR_c, CHAR_a, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_e};
WORD aAUTOPWRPowerOn[11] = {5, CHAR_L, CHAR_i, CHAR_g, CHAR_a, CHAR_r};
WORD aAUTOPWRPowerOff[12] = {8, CHAR_D, CHAR_e, CHAR_s, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_r};
WORD aAUTOPWRPowerOnEnable[30] = {29, CHAR_A, CHAR_c, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_t, CHAR_o, CHAR_m, CHAR_a, CHAR_t, CHAR_i, CHAR_c, CHAR_a, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_e};
WORD aAUTOPWRPowerOffEnable[33] = {32, CHAR_A, CHAR_c, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_s, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_t, CHAR_o, CHAR_m, CHAR_a, CHAR_t, CHAR_i, CHAR_c, CHAR_a, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_e};
WORD aAUTOPWRSetTime[14] = {12, CHAR_D, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_o, CHAR_r, CHAR_a};
WORD aMMBtnSave[10] = {7, CHAR_G, CHAR_u, CHAR_a, CHAR_r, CHAR_d, CHAR_a, CHAR_r};
WORD aMMSaveFileDlgTitle[20] = {16, CHAR_G, CHAR_u, CHAR_a, CHAR_r, CHAR_d, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_f, CHAR_i, CHAR_c, CHAR_h, CHAR_e, CHAR_i, CHAR_r, CHAR_o};
WORD aMMSaveFileDlgContent[18] = {7, CHAR_A, CHAR_g, CHAR_u, CHAR_a, CHAR_r, CHAR_d, CHAR_e};
WORD aTHUMBButtonSort[10] = {7, CHAR_O, CHAR_r, CHAR_d, CHAR_e, CHAR_n, CHAR_a, CHAR_r};
WORD aTHUMBButtonDeleteAll[11] = {10, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_FULL_STOP, CHAR_SPACE, CHAR_t, CHAR_u, CHAR_d, CHAR_o};
WORD aTHUMBTitleEditMode[18] = {14, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_e, CHAR_d, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o};
WORD aTHUMBTitleDeleteMode[19] = {18, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_e, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a, CHAR_c1, CHAR_a4, CHAR_o};
WORD aTHUMBHelpDeleteMode[14] = {8, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a, CHAR_r};
WORD aTHUMBHelpSortMode[26] = {14, CHAR_F, CHAR_o, CHAR_c, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_i, CHAR_m, CHAR_a, CHAR_g, CHAR_e, CHAR_m};
WORD aTHUMBTitleSortMode[18] = {17, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_d, CHAR_e, CHAR_n, CHAR_a, CHAR_c1, CHAR_a4, CHAR_o};
WORD aEDITDialogTitle[27] = {26, CHAR_C, CHAR_a, CHAR_i, CHAR_x, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_a2, CHAR_l, CHAR_o, CHAR_g, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_e, CHAR_d, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o};
WORD aEDITDialogContent[38] = {27, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_c, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_SPACE, CHAR_o, CHAR_SPACE, CHAR_m, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_e, CHAR_d, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o};
WORD aBUTTONCopy[9] = {6, CHAR_C, CHAR_o, CHAR_p, CHAR_i, CHAR_a, CHAR_r};
WORD aBUTTONDelete[9] = {8, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a, CHAR_r};
WORD aSETDATESetDate[17] = {12, CHAR_D, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_t, CHAR_a};
#endif //#ifdef SUPPORT_POWERON_MENU									

WORD aMENUOn[5] = {2, CHAR_A, CHAR_c};
WORD aMENUOff[7] = {3, CHAR_D, CHAR_e, CHAR_s};
#ifdef MUSTEK_SETUP_MENU									

WORD aMENU5Min[7] = {6, CHAR_5, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU30Min[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU1Hour[8] = {6, CHAR_1, CHAR_SPACE, CHAR_h, CHAR_o, CHAR_r, CHAR_a};
WORD aMENU4Hours[9] = {7, CHAR_4, CHAR_SPACE, CHAR_h, CHAR_o, CHAR_r, CHAR_a, CHAR_s};
WORD aMENU12Hours[10] = {8, CHAR_1, CHAR_2, CHAR_SPACE, CHAR_h, CHAR_o, CHAR_r, CHAR_a, CHAR_s};
WORD aMENU1Day[9] = {5, CHAR_1, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_a};
WORD aMENUTransition[14] = {9, CHAR_T, CHAR_r, CHAR_a, CHAR_n, CHAR_s, CHAR_i, CHAR_c1, CHAR_a4, CHAR_o};
WORD aMENUTVSystem[17] = {13, CHAR_S, CHAR_i, CHAR_s, CHAR_t, CHAR_e, CHAR_m, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMENUFullScreen[15] = {11, CHAR_E, CHAR_c, CHAR_r, CHAR_a4, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_t, CHAR_e, CHAR_i, CHAR_r};
WORD aMENUReset[8] = {5, CHAR_R, CHAR_e, CHAR_p, CHAR_o, CHAR_r};
WORD aMENUConfirm[8] = {7, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_r, CHAR_FULL_STOP};
WORD aMENUExitSetup[30] = {18, CHAR_S, CHAR_a, CHAR_i, CHAR_r, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p};
#endif //MUSTEK_SETUP_MENU											
#ifdef SUPPORT_JPEG_AUTO_AND_FULL_SCREEN
WORD aMENUFullScreen[15] = {11, CHAR_E, CHAR_c, CHAR_r, CHAR_a4, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_t, CHAR_e, CHAR_i, CHAR_r};
#endif
WORD aBUTTONYes[4] = {3, CHAR_S, CHAR_i, CHAR_m};
WORD aBUTTONNo[5] = {3, CHAR_N, CHAR_a4, CHAR_o};
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW											

WORD aMMSaveDlgTitle[31] = {15, CHAR_C, CHAR_x, CHAR_SPACE, CHAR_G, CHAR_u, CHAR_a, CHAR_r, CHAR_d, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_f, CHAR_o, CHAR_t, CHAR_o};
WORD aMMDeleteDlgTitle[31] = {30, CHAR_C, CHAR_a, CHAR_i, CHAR_x, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_a2, CHAR_l, CHAR_o, CHAR_g, CHAR_o, CHAR_SPACE, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_f, CHAR_o, CHAR_t, CHAR_o};
WORD aMMSaveDlgContent[14] = {8, CHAR_G, CHAR_u, CHAR_a, CHAR_r, CHAR_d, CHAR_a, CHAR_r, CHAR_QUESTION_MARK};
WORD aMMDeleteDlgContent[13] = {9, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a, CHAR_r, CHAR_QUESTION_MARK};
#endif //#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW									

#ifdef SUPPORT_CHOOSE_MEDIA									

WORD aMENUMedia[11] = {10, CHAR_M, CHAR_u, CHAR_l, CHAR_t, CHAR_i, CHAR_m, CHAR_e2, CHAR_d, CHAR_i, CHAR_a};
#endif //SUPPORT_CHOOSE_MEDIA									

#ifdef SUPPORT_PHOTO_BROWSE_MODE									

WORD aNDBrowseMode[26] = {16, CHAR_M, CHAR_o, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_p, CHAR_e, CHAR_s, CHAR_q, CHAR_u, CHAR_i, CHAR_s, CHAR_a};
#endif //SUPPORT_PHOTO_BROWSE_MODE									

#ifdef SUPPORT_TFT_DIGITAL_MODE									

WORD aMENUAVOutput[13] = {8, CHAR_S, CHAR_a, CHAR_i1, CHAR_d, CHAR_a, CHAR_SPACE, CHAR_A, CHAR_V};
#endif //SUPPORT_TFT_DIGITAL_MODE									

WORD aMENUPanel[9] = {6, CHAR_P, CHAR_a, CHAR_i, CHAR_n, CHAR_e, CHAR_l};
#ifdef SUPPORT_RADIO_TUNER									

WORD aRADIORecord[9] = {6, CHAR_G, CHAR_r, CHAR_a, CHAR_v, CHAR_a, CHAR_r};
WORD aRADIOScan[9] = {4, CHAR_P, CHAR_e, CHAR_s, CHAR_q};
WORD aRADIOStationsFound[17] = {16, CHAR_e, CHAR_s, CHAR_t, CHAR_a, CHAR_c1, CHAR_o4, CHAR_e, CHAR_s, CHAR_SPACE, CHAR_e, CHAR_n, CHAR_c, CHAR_o, CHAR_n, CHAR_t, CHAR_FULL_STOP};
#endif //SUPPORT_RADIO_TUNER									

#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									

WORD aCODEWesternEuropean[17] = {13, CHAR_E, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_e, CHAR_u, CHAR_SPACE, CHAR_o, CHAR_c, CHAR_i, CHAR_d, CHAR_FULL_STOP};
#endif   // SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									

#ifdef SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									

WORD aCODECentralEuropean[19] = {15, CHAR_E, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_e, CHAR_u, CHAR_SPACE, CHAR_c, CHAR_e, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_l};
#endif   // SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									

#ifdef SUPPORT_CHAR_ENCODING_GREEK									

WORD aCODEGreek[11] = {5, CHAR_G, CHAR_r, CHAR_e, CHAR_g, CHAR_o};
#endif   // SUPPORT_CHAR_ENCODING_GREEK									

#ifdef SUPPORT_CHAR_ENCODING_HEBREW									

WORD aCODEHebrew[10] = {8, CHAR_H, CHAR_e, CHAR_b, CHAR_r, CHAR_a, CHAR_i, CHAR_c, CHAR_o};
#endif   // SUPPORT_CHAR_ENCODING_HEBREW									

#ifdef SUPPORT_CHAR_ENCODING_TURKISH									

WORD aCODETurkish[9] = {5, CHAR_T, CHAR_u, CHAR_r, CHAR_c, CHAR_o};
#endif   // SUPPORT_CHAR_ENCODING_TURKISH									

#ifdef SUPPORT_CHAR_ENCODING_CP1250									

WORD aCODECP1250[8] = {7, CHAR_C, CHAR_P, CHAR_SPACE, CHAR_1, CHAR_2, CHAR_5, CHAR_0};
#endif   // SUPPORT_CHAR_ENCODING_CP1250									

#ifdef SUPPORT_CHAR_ENCODING_CYRILLIC									

WORD aCODECyrillic[11] = {8, CHAR_C, CHAR_i, CHAR_r, CHAR_i1, CHAR_l, CHAR_i, CHAR_c, CHAR_o};
#endif   // SUPPORT_CHAR_ENCODING_CYRILLIC									

#ifdef SUPPORT_CHAR_ENCODING_CHINESE									

WORD aCODETWNUni[13] = {12, CHAR_T, CHAR_W, CHAR_N, CHAR_L_PARENTHESIS, CHAR_U, CHAR_n, CHAR_i, CHAR_c, CHAR_o, CHAR_d, CHAR_e, CHAR_R_PARENTHESIS};
#endif   // SUPPORT_CHAR_ENCODING_CHINESE									

#ifdef SUPPORT_CHAR_ENCODING_BIG5									

WORD aCODETWNBig5[10] = {9, CHAR_T, CHAR_W, CHAR_N, CHAR_L_PARENTHESIS, CHAR_B, CHAR_i, CHAR_g, CHAR_5, CHAR_R_PARENTHESIS};
#endif   // SUPPORT_CHAR_ENCODING_BIG5									

#ifdef SUPPORT_CHAR_ENCODING_JAPANESE									

WORD aCODEJPNUni[13] = {12, CHAR_J, CHAR_P, CHAR_N, CHAR_L_PARENTHESIS, CHAR_U, CHAR_n, CHAR_i, CHAR_c, CHAR_o, CHAR_d, CHAR_e, CHAR_R_PARENTHESIS};
#endif   // SUPPORT_CHAR_ENCODING_JAPANESE									

#ifdef SUPPORT_CHAR_ENCODING_SHIFT_JIS									

WORD aCODEJPNJIS[15] = {14, CHAR_J, CHAR_P, CHAR_N, CHAR_L_PARENTHESIS, CHAR_S, CHAR_h, CHAR_i, CHAR_f, CHAR_t, CHAR_HYPHEN_MINUS, CHAR_J, CHAR_I, CHAR_S, CHAR_R_PARENTHESIS};
#endif   // SUPPORT_CHAR_ENCODING_SHIFT_JIS									

WORD aMENUEncoding[9] = {8, CHAR_E, CHAR_n, CHAR_c, CHAR_o, CHAR_d, CHAR_i, CHAR_n, CHAR_g};
