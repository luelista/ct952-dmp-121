#include "stringdec.h"
#ifdef OSDDSPLY_MAIN_FILE									

WORD aDSPLYLanguageJapanese[11] = {6, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_s};
WORD aDSPLYLanguageEnglish[10] = {6, CHAR_E, CHAR_n, CHAR_g, CHAR_e, CHAR_l, CHAR_s};
WORD aDSPLYLanguageFrench[12] = {5, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_s};
WORD aDSPLYLanguageGerman[10] = {5, CHAR_D, CHAR_u, CHAR_i, CHAR_t, CHAR_s};
WORD aDSPLYLanguageRussian[9] = {8, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguageChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_e, CHAR_s};
WORD aDSPLYLanguageKorean[11] = {8, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aDSPLYLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aDSPLYLanguageThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aDSPLYLanguageItalian[12] = {9, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aDSPLYLanguageSpanish[11] = {6, CHAR_S, CHAR_p, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aDSPLYLanguagePortuguese[14] = {9, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_e, CHAR_e, CHAR_s};
WORD aDSPLYLanguageOthers[9] = {7, CHAR_O, CHAR_v, CHAR_e, CHAR_r, CHAR_i, CHAR_g, CHAR_e};
WORD aDSPLYOff[10] = {3, CHAR_U, CHAR_i, CHAR_t};
#ifndef NO_DVD_DISPLAY_UI									

WORD aDSPLYTitleElapsed[17] = {16, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_r, CHAR_e, CHAR_k, CHAR_e, CHAR_n};
WORD aDSPLYTitleRemain[16] = {15, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_s, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n, CHAR_d};
WORD aDSPLYChapterElapsed[16] = {14, CHAR_H, CHAR_s, CHAR_t, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_r, CHAR_e, CHAR_k, CHAR_e, CHAR_n};
WORD aDSPLYChapterRemain[15] = {13, CHAR_H, CHAR_s, CHAR_t, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_s, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n, CHAR_d};
#endif //NO_DVD_DISPLAY_UI									

WORD aDSPLYSingleElapsed[17] = {16, CHAR_E, CHAR_n, CHAR_k, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_r, CHAR_e, CHAR_k, CHAR_e, CHAR_n};
WORD aDSPLYSingleRemain[16] = {15, CHAR_E, CHAR_n, CHAR_k, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_s, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n, CHAR_d};
WORD aDSPLYTotalElapsed[18] = {17, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_r, CHAR_e, CHAR_k, CHAR_e, CHAR_n};
WORD aDSPLYTotalRemain[17] = {16, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_s, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n, CHAR_d};
WORD aDSPLYRootMenu[16] = {9, CHAR_H, CHAR_o, CHAR_o, CHAR_f, CHAR_d, CHAR_m, CHAR_e, CHAR_n, CHAR_u};
WORD aDSPLYTitleMenu[12] = {9, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_m, CHAR_e, CHAR_n, CHAR_u};
#ifdef SUPPORT_NAVIGATOR									

WORD aDSPLYRepeatTitle[20] = {14, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_r, CHAR_h, CHAR_a, CHAR_l, CHAR_e, CHAR_n};
WORD aDSPLYRepeatChapter[22] = {18, CHAR_H, CHAR_o, CHAR_o, CHAR_f, CHAR_d, CHAR_s, CHAR_t, CHAR_u, CHAR_k, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_r, CHAR_h, CHAR_a, CHAR_l, CHAR_e, CHAR_n};
WORD aDSPLYRepeatDisc[19] = {15, CHAR_S, CHAR_c, CHAR_h, CHAR_i, CHAR_j, CHAR_f, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_r, CHAR_h, CHAR_a, CHAR_l, CHAR_e, CHAR_n};
WORD aDSPLYRepeatOff[22] = {12, CHAR_H, CHAR_e, CHAR_r, CHAR_h, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_u, CHAR_i, CHAR_t};
#endif //SUPPORT_NAVIGATOR									

WORD aDSPLYpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aDSPLYInterlace[15] = {14, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_l, CHAR_i, CHAR_n, CHAR_i, CHAR_e4, CHAR_r, CHAR_i, CHAR_n, CHAR_g};
WORD aDSPLYResolution[15] = {9, CHAR_R, CHAR_e, CHAR_s, CHAR_o, CHAR_l, CHAR_u, CHAR_t, CHAR_i, CHAR_e};
WORD aDSPLYDate[7] = {6, CHAR_D, CHAR_a, CHAR_t, CHAR_u, CHAR_m, CHAR_COLON};
#ifdef SUPPORT_NAVIGATOR									

WORD aNGTitle[8] = {6, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_COLON};
WORD aNGChapter[11] = {10, CHAR_H, CHAR_o, CHAR_o, CHAR_f, CHAR_d, CHAR_s, CHAR_t, CHAR_u, CHAR_k, CHAR_COLON};
WORD aNGAngle[13] = {5, CHAR_H, CHAR_o, CHAR_e, CHAR_k, CHAR_COLON};
WORD aNGAudio[7] = {6, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o, CHAR_COLON};
WORD aNGSubtitle1[13] = {12, CHAR_O, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_1, CHAR_COLON};
WORD aNGSubtitle2[13] = {12, CHAR_O, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_2, CHAR_COLON};
#endif //SUPPORT_NAVIGATOR									

#endif //OSDDSPLY_MAIN_FILE									

#ifdef OSDND_MAIN_FILE									

WORD aNDNoDisc[14] = {11, CHAR_G, CHAR_e, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_h, CHAR_i, CHAR_j, CHAR_f};
WORD aNDNoUSBDevice[14] = {13, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_U, CHAR_S, CHAR_B, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_v, CHAR_i, CHAR_c, CHAR_e};
WORD aNDOpen[7] = {6, CHAR_O, CHAR_p, CHAR_e, CHAR_n, CHAR_e, CHAR_n};
WORD aNDClose[10] = {7, CHAR_S, CHAR_l, CHAR_u, CHAR_i, CHAR_t, CHAR_e, CHAR_n};
WORD aNDReturn[8] = {5, CHAR_T, CHAR_e, CHAR_r, CHAR_u, CHAR_g};
WORD aNDFreeze[15] = {9, CHAR_B, CHAR_e, CHAR_v, CHAR_r, CHAR_i, CHAR_e, CHAR_z, CHAR_e, CHAR_n};
WORD aNDRandomOn[12] = {11, CHAR_W, CHAR_i, CHAR_l, CHAR_l, CHAR_e, CHAR_k, CHAR_e, CHAR_u, CHAR_r, CHAR_i, CHAR_g};
WORD aNDRandomOff[12] = {11, CHAR_O, CHAR_p, CHAR_SPACE, CHAR_v, CHAR_o, CHAR_l, CHAR_g, CHAR_o, CHAR_r, CHAR_d, CHAR_e};
WORD aNDResume[11] = {9, CHAR_H, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_t, CHAR_t, CHAR_e, CHAR_n};
WORD aNDRootMenu[16] = {9, CHAR_H, CHAR_o, CHAR_o, CHAR_f, CHAR_d, CHAR_m, CHAR_e, CHAR_n, CHAR_u};
WORD aNDTitleMenu[12] = {9, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_m, CHAR_e, CHAR_n, CHAR_u};
WORD aNDRotateRight[26] = {23, CHAR_9, CHAR_0, CHAR_N_DEGREE, CHAR_SPACE, CHAR_n, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_r, CHAR_o, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aNDRotateLeft[26] = {22, CHAR_9, CHAR_0, CHAR_N_DEGREE, CHAR_SPACE, CHAR_n, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_r, CHAR_o, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aNDFlipHorizontally[26] = {21, CHAR_H, CHAR_o, CHAR_r, CHAR_i, CHAR_z, CHAR_o, CHAR_n, CHAR_t, CHAR_a, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_s, CHAR_p, CHAR_i, CHAR_e, CHAR_g, CHAR_e, CHAR_l, CHAR_e, CHAR_n};
WORD aNDFlipVertically[25] = {19, CHAR_V, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_c, CHAR_a, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_s, CHAR_p, CHAR_i, CHAR_e, CHAR_g, CHAR_e, CHAR_l, CHAR_e, CHAR_n};
WORD aNDpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aNDInterlace[15] = {14, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_l, CHAR_i, CHAR_n, CHAR_i, CHAR_e4, CHAR_r, CHAR_i, CHAR_n, CHAR_g};
WORD aNDRepeatNone[10] = {3, CHAR_U, CHAR_i, CHAR_t};
WORD aNDRepeatTrack[8] = {5, CHAR_S, CHAR_p, CHAR_o, CHAR_o, CHAR_r};
WORD aNDRepeatDisc[7] = {6, CHAR_S, CHAR_c, CHAR_h, CHAR_i, CHAR_j, CHAR_f};
WORD aNDRepeatAll[10] = {5, CHAR_A, CHAR_l, CHAR_l, CHAR_e, CHAR_s};
WORD aNDRepeatTitle[7] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aNDRepeatChapter[10] = {9, CHAR_H, CHAR_o, CHAR_o, CHAR_f, CHAR_d, CHAR_s, CHAR_t, CHAR_u, CHAR_k};
WORD aNDRepeatFolder[9] = {3, CHAR_M, CHAR_a, CHAR_p};
WORD aNDRepeatA[15] = {10, CHAR_A, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_r, CHAR_h, CHAR_a, CHAR_l, CHAR_e, CHAR_n};
WORD aNDRepeatAB[17] = {12, CHAR_A, CHAR_HYPHEN_MINUS, CHAR_B, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_r, CHAR_h, CHAR_a, CHAR_l, CHAR_e, CHAR_n};
WORD aNDRepeatOff[22] = {12, CHAR_H, CHAR_e, CHAR_r, CHAR_h, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_u, CHAR_i, CHAR_t};
#ifndef NO_DISC_MODE									

WORD aNDNavigateOn[25] = {13, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_e, CHAR_r, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_a, CHAR_n};
WORD aNDNavigateOff[28] = {13, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_e, CHAR_r, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_u, CHAR_i, CHAR_t};
WORD aNDPBCOn[13] = {7, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_a, CHAR_a, CHAR_n};
WORD aNDPBCOff[14] = {7, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_u, CHAR_i, CHAR_t};
#endif //#ifndef NO_DISC_MODE									

WORD aNDZoom1[8] = {7, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_SPACE, CHAR_x, CHAR_1};
WORD aNDZoom2[8] = {7, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_SPACE, CHAR_x, CHAR_2};
WORD aNDZoom4[8] = {7, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_SPACE, CHAR_x, CHAR_4};
WORD aNDMoveLeft[24] = {22, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_p, CHAR_l, CHAR_a, CHAR_a, CHAR_t, CHAR_s, CHAR_e, CHAR_n};
WORD aNDMoveRight[24] = {23, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_p, CHAR_l, CHAR_a, CHAR_a, CHAR_t, CHAR_s, CHAR_e, CHAR_n};
WORD aNDMoveUp[23] = {22, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_b, CHAR_o, CHAR_v, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_p, CHAR_l, CHAR_a, CHAR_a, CHAR_t, CHAR_s, CHAR_e, CHAR_n};
WORD aNDMoveDown[23] = {22, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_o, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_p, CHAR_l, CHAR_a, CHAR_a, CHAR_t, CHAR_s, CHAR_e, CHAR_n};
WORD aNDVocalOn[15] = {8, CHAR_S, CHAR_t, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_a, CHAR_a, CHAR_n};
WORD aNDVocalOff[16] = {8, CHAR_S, CHAR_t, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_u, CHAR_i, CHAR_t};
WORD aNDVocalLL[15] = {7, CHAR_S, CHAR_t, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_L, CHAR_L};
WORD aNDVocalRR[13] = {7, CHAR_S, CHAR_t, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_R, CHAR_R};
WORD aNDKaraokeOn[17] = {11, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_a, CHAR_a, CHAR_n};
WORD aNDKaraokeOff[18] = {11, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_u, CHAR_i, CHAR_t};
WORD aNDJPEGEffectNone[12] = {11, CHAR_G, CHAR_e, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_e, CHAR_f, CHAR_f, CHAR_e, CHAR_c, CHAR_t};
WORD aNDJPEGEffectCoverUp[27] = {15, CHAR_O, CHAR_m, CHAR_h, CHAR_o, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectCoverDown[26] = {15, CHAR_O, CHAR_m, CHAR_l, CHAR_a, CHAR_a, CHAR_g, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectCoverLeft[20] = {19, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectCoverRight[20] = {19, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e};
WORD aNDJPEGEffectUncoverUp[23] = {16, CHAR_O, CHAR_m, CHAR_h, CHAR_o, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_o, CHAR_n, CHAR_t, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectUncoverDown[24] = {16, CHAR_O, CHAR_m, CHAR_l, CHAR_a, CHAR_a, CHAR_g, CHAR_SPACE, CHAR_o, CHAR_n, CHAR_t, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectUncoverLeft[21] = {20, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_o, CHAR_n, CHAR_t, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectUncoverRight[22] = {21, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_o, CHAR_n, CHAR_t, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectWipeUp[22] = {12, CHAR_O, CHAR_m, CHAR_h, CHAR_o, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_g, CHAR_e, CHAR_n};
WORD aNDJPEGEffectWipeDown[21] = {12, CHAR_O, CHAR_m, CHAR_l, CHAR_a, CHAR_a, CHAR_g, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_g, CHAR_e, CHAR_n};
WORD aNDJPEGEffectWipeLeft[17] = {16, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_g, CHAR_e, CHAR_n};
WORD aNDJPEGEffectWipeRight[18] = {17, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_g, CHAR_e, CHAR_n};
WORD aNDJPEGEffectPushUp[14] = {12, CHAR_O, CHAR_m, CHAR_h, CHAR_o, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_d, CHAR_u, CHAR_w, CHAR_e, CHAR_n};
WORD aNDJPEGEffectPushDown[13] = {12, CHAR_O, CHAR_m, CHAR_l, CHAR_a, CHAR_a, CHAR_g, CHAR_SPACE, CHAR_d, CHAR_u, CHAR_w, CHAR_e, CHAR_n};
WORD aNDJPEGEffectPushLeft[17] = {16, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_d, CHAR_u, CHAR_w, CHAR_e, CHAR_n};
WORD aNDJPEGEffectPushRight[18] = {17, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_d, CHAR_u, CHAR_w, CHAR_e, CHAR_n};
WORD aNDJPEGEffectFadeThroughBlack[26] = {19, CHAR_D, CHAR_o, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_z, CHAR_w, CHAR_a, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_u, CHAR_i, CHAR_t, CHAR_f, CHAR_a, CHAR_d, CHAR_e, CHAR_n};
WORD aNDJPEGEffectRandom[16] = {11, CHAR_W, CHAR_i, CHAR_l, CHAR_l, CHAR_e, CHAR_k, CHAR_e, CHAR_u, CHAR_r, CHAR_i, CHAR_g};
#ifndef NO_DISC_MODE									

WORD aNDEncrypt[17] = {15, CHAR_I, CHAR_l, CHAR_l, CHAR_e, CHAR_g, CHAR_a, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_h, CHAR_i, CHAR_j, CHAR_f};
WORD aNDErrRegionCode[27] = {21, CHAR_V, CHAR_e, CHAR_r, CHAR_k, CHAR_e, CHAR_e, CHAR_r, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_g, CHAR_i, CHAR_o, CHAR_s, CHAR_c, CHAR_h, CHAR_i, CHAR_j, CHAR_f};
WORD aNDWrongDisc[19] = {16, CHAR_V, CHAR_e, CHAR_r, CHAR_k, CHAR_e, CHAR_e, CHAR_r, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_h, CHAR_i, CHAR_j, CHAR_f};
WORD aNDDamageDisc[19] = {18, CHAR_B, CHAR_e, CHAR_s, CHAR_c, CHAR_h, CHAR_a, CHAR_d, CHAR_i, CHAR_g, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_h, CHAR_i, CHAR_j, CHAR_f};
#endif //#ifndef NO_DISC_MODE									

WORD aNDNoPlayableFile[25] = {24, CHAR_G, CHAR_e, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_f, CHAR_s, CHAR_p, CHAR_e, CHAR_e, CHAR_l, CHAR_b, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_d};
WORD aNDDisc[7] = {6, CHAR_S, CHAR_c, CHAR_h, CHAR_i, CHAR_j, CHAR_f};
WORD aNDTrack[8] = {5, CHAR_S, CHAR_p, CHAR_o, CHAR_o, CHAR_r};
WORD aNDTitle[7] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aNDChapter[10] = {9, CHAR_H, CHAR_o, CHAR_o, CHAR_f, CHAR_d, CHAR_s, CHAR_t, CHAR_u, CHAR_k};
WORD aNDLR[4] = {3, CHAR_L, CHAR_SOLIDUS, CHAR_R};
WORD aNDLL[4] = {3, CHAR_L, CHAR_SOLIDUS, CHAR_L};
WORD aNDRR[4] = {3, CHAR_R, CHAR_SOLIDUS, CHAR_R};
WORD aNDVolume[11] = {6, CHAR_V, CHAR_o, CHAR_l, CHAR_u, CHAR_m, CHAR_e};
WORD aNDTone[9] = {4, CHAR_T, CHAR_o, CHAR_o, CHAR_n};
WORD aNDEcho[5] = {4, CHAR_E, CHAR_c, CHAR_h, CHAR_o};
WORD aNDNoSurround[19] = {13, CHAR_G, CHAR_e, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_o, CHAR_m, CHAR_g, CHAR_e, CHAR_v, CHAR_i, CHAR_n, CHAR_g};
WORD aNDLiveSurround[10] = {4, CHAR_L, CHAR_i, CHAR_v, CHAR_e};
WORD aNDTheaterSurround[8] = {7, CHAR_T, CHAR_h, CHAR_e, CHAR_a, CHAR_t, CHAR_e, CHAR_r};
WORD aNDStereoSurround[8] = {6, CHAR_S, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_o};
WORD aNDMovieSurround[9] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aNDKaraSurround[8] = {7, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e};
WORD aNDLanguageJapanese[11] = {6, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_s};
WORD aNDLanguageEnglish[10] = {6, CHAR_E, CHAR_n, CHAR_g, CHAR_e, CHAR_l, CHAR_s};
WORD aNDLanguageFrench[12] = {5, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_s};
WORD aNDLanguageGerman[10] = {5, CHAR_D, CHAR_u, CHAR_i, CHAR_t, CHAR_s};
WORD aNDLanguageRussian[9] = {8, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguageChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_e, CHAR_s};
WORD aNDLanguageKorean[11] = {8, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aNDLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aNDLanguageThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aNDLanguageItalian[12] = {9, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aNDLanguageSpanish[11] = {6, CHAR_S, CHAR_p, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aNDLanguagePortuguese[14] = {9, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_e, CHAR_e, CHAR_s};
WORD aNDLanguageOthers[9] = {7, CHAR_O, CHAR_v, CHAR_e, CHAR_r, CHAR_i, CHAR_g, CHAR_e};
WORD aNDSubtitle[13] = {11, CHAR_O, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_COLON};
WORD aNDOff[10] = {3, CHAR_U, CHAR_i, CHAR_t};
WORD aNDWait[12] = {5, CHAR_L, CHAR_a, CHAR_d, CHAR_e, CHAR_n};
WORD aNDSelect[12] = {10, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aNDCardReader[29] = {10, CHAR_K, CHAR_a, CHAR_a, CHAR_r, CHAR_t, CHAR_l, CHAR_e, CHAR_z, CHAR_e, CHAR_r};
#endif //OSDND_MAIN_FILE									

#ifdef OSDMM_MAIN_FILE									

WORD aMMProgramModeTitle[24] = {14, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_a, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_d};
WORD aMMProgramListModeTitle[21] = {14, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_a, CHAR_l, CHAR_i, CHAR_j, CHAR_s, CHAR_t};
WORD aMMTotalFiles[25] = {24, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_a, CHAR_a, CHAR_n, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_e, CHAR_n, CHAR_COLON};
WORD aMMFileNumber[20] = {15, CHAR_B, CHAR_e, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_s, CHAR_n, CHAR_u, CHAR_m, CHAR_m, CHAR_e, CHAR_r, CHAR_COLON};
WORD aMMDlgFileTypeTitle[14] = {13, CHAR_B, CHAR_E, CHAR_S, CHAR_T, CHAR_A, CHAR_N, CHAR_D, CHAR_S, CHAR_T, CHAR_Y, CHAR_P, CHAR_E, CHAR_COLON};
WORD aMMDlgSortFileTitle[16] = {15, CHAR_S, CHAR_O, CHAR_R, CHAR_T, CHAR_E, CHAR_E, CHAR_R, CHAR_B, CHAR_E, CHAR_S, CHAR_T, CHAR_A, CHAR_N, CHAR_D, CHAR_COLON};
WORD aMMDlgSortFileMethodTitle[9] = {8, CHAR_M, CHAR_e, CHAR_t, CHAR_h, CHAR_o, CHAR_d, CHAR_e, CHAR_COLON};
WORD aMMDlgSortFileSequenceTitle[12] = {9, CHAR_V, CHAR_o, CHAR_l, CHAR_g, CHAR_o, CHAR_r, CHAR_d, CHAR_e, CHAR_COLON};
WORD aMMDlgSequenceAscendingItem[10] = {8, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_g, CHAR_e, CHAR_n, CHAR_d};
WORD aMMDlgSequenceDescendingItem[12] = {6, CHAR_D, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d};
WORD aMMDlgMethodTypeItem[11] = {7, CHAR_O, CHAR_p, CHAR_SPACE, CHAR_t, CHAR_y, CHAR_p, CHAR_e};
WORD aMMDlgMethodNameItem[11] = {7, CHAR_O, CHAR_p, CHAR_SPACE, CHAR_n, CHAR_a, CHAR_a, CHAR_m};
WORD aMMDlgMethodTimeItem[10] = {8, CHAR_O, CHAR_p, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_t, CHAR_u, CHAR_m};
WORD aMMHelpProgram[16] = {15, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_e, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_t};
WORD aMMHelpProgramList[14] = {13, CHAR_V, CHAR_e, CHAR_r, CHAR_w, CHAR_i, CHAR_j, CHAR_d, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_t};
WORD aMMBtnFileType[10] = {9, CHAR_B, CHAR_e, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_t, CHAR_y, CHAR_p, CHAR_e};
WORD aMMBtnSortFile[12] = {10, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_FULL_STOP, CHAR_b, CHAR_e, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aMMBtnList[7] = {5, CHAR_L, CHAR_i, CHAR_j, CHAR_s, CHAR_t};
WORD aMMBtnPlay[9] = {8, CHAR_A, CHAR_f, CHAR_s, CHAR_p, CHAR_e, CHAR_l, CHAR_e, CHAR_n};
WORD aMMBtnDeleteAll[11] = {8, CHAR_A, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_w, CHAR_FULL_STOP};
WORD aMMBtnReturn[8] = {5, CHAR_T, CHAR_e, CHAR_r, CHAR_u, CHAR_g};
WORD aMMBtnExit[8] = {7, CHAR_S, CHAR_l, CHAR_u, CHAR_i, CHAR_t, CHAR_e, CHAR_n};
#endif //OSDMM_MAIN_FILE									

#ifdef THUMB_MAIN_FILE									

WORD aTHUMBPage[8] = {7, CHAR_P, CHAR_a, CHAR_g, CHAR_i, CHAR_n, CHAR_a, CHAR_COLON};
WORD aTHUMBTitleGeneralMode[19] = {16, CHAR_M, CHAR_i, CHAR_n, CHAR_i, CHAR_a, CHAR_f, CHAR_b, CHAR_e, CHAR_e, CHAR_l, CHAR_d, CHAR_i, CHAR_n, CHAR_g, CHAR_e, CHAR_n};
WORD aTHUMBTitleRotateMode[19] = {11, CHAR_R, CHAR_o, CHAR_t, CHAR_e, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_d};
WORD aTHUMBTitleProgramMode[24] = {14, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_a, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_d};
WORD aTHUMBTitleProgramList[21] = {14, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_a, CHAR_l, CHAR_i, CHAR_j, CHAR_s, CHAR_t};
WORD aTHUMBHelpRotateMode[17] = {10, CHAR_R, CHAR_o, CHAR_t, CHAR_e, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_t};
WORD aTHUMBHelpProgramMode[16] = {15, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_e, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_t};
WORD aTHUMBHelpProgramList[14] = {13, CHAR_V, CHAR_e, CHAR_r, CHAR_w, CHAR_i, CHAR_j, CHAR_d, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_t};
WORD aTHUMBButtonRotate[8] = {7, CHAR_R, CHAR_o, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aTHUMBButtonSlideShow[11] = {10, CHAR_D, CHAR_i, CHAR_a, CHAR_v, CHAR_o, CHAR_o, CHAR_r, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aTHUMBButtonReset[8] = {5, CHAR_R, CHAR_e, CHAR_s, CHAR_e, CHAR_t};
WORD aTHUMBButtonReturn[8] = {5, CHAR_T, CHAR_e, CHAR_r, CHAR_u, CHAR_g};
WORD aTHUMBButtonProgramList[7] = {5, CHAR_L, CHAR_i, CHAR_j, CHAR_s, CHAR_t};
#endif //THUMB_MAIN_FILE									

#ifdef OSDDG_MAIN_FILE									

WORD aDGDlgBtnTrackDigest[21] = {20, CHAR_U, CHAR_i, CHAR_t, CHAR_t, CHAR_r, CHAR_e, CHAR_k, CHAR_s, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_a, CHAR_n, CHAR_SPACE, CHAR_s, CHAR_p, CHAR_o, CHAR_o, CHAR_r};
WORD aDGDlgBtnDiscInterval[21] = {14, CHAR_S, CHAR_c, CHAR_h, CHAR_i, CHAR_j, CHAR_f, CHAR_i, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l};
WORD aDGPage[8] = {7, CHAR_P, CHAR_a, CHAR_g, CHAR_i, CHAR_n, CHAR_a, CHAR_COLON};
WORD aDGTitleTrackDigest[21] = {20, CHAR_U, CHAR_i, CHAR_t, CHAR_t, CHAR_r, CHAR_e, CHAR_k, CHAR_s, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_a, CHAR_n, CHAR_SPACE, CHAR_s, CHAR_p, CHAR_o, CHAR_o, CHAR_r};
WORD aDGTitleDiscInterval[21] = {14, CHAR_S, CHAR_c, CHAR_h, CHAR_i, CHAR_j, CHAR_f, CHAR_i, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l};
WORD aDGInfoTrack[9] = {6, CHAR_S, CHAR_p, CHAR_o, CHAR_o, CHAR_r, CHAR_COLON};
WORD aDGBtnMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aDGBtnExit[8] = {7, CHAR_S, CHAR_l, CHAR_u, CHAR_i, CHAR_t, CHAR_e, CHAR_n};
#endif //OSDDG_MAIN_FILE									

#ifdef OSDPRGM_MAIN_FILE									

WORD aProgram[12] = {9, CHAR_P, CHAR_R, CHAR_O, CHAR_G, CHAR_R, CHAR_A, CHAR_M, CHAR_M, CHAR_A};
WORD aTitle[7] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aChapter[10] = {9, CHAR_H, CHAR_o, CHAR_o, CHAR_f, CHAR_d, CHAR_s, CHAR_t, CHAR_u, CHAR_k};
WORD aTrack[8] = {5, CHAR_S, CHAR_p, CHAR_o, CHAR_o, CHAR_r};
WORD aSelect[12] = {10, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aPlay[11] = {8, CHAR_A, CHAR_f, CHAR_s, CHAR_p, CHAR_e, CHAR_l, CHAR_e, CHAR_n};
WORD aClear[8] = {6, CHAR_W, CHAR_i, CHAR_s, CHAR_s, CHAR_e, CHAR_n};
#endif //OSDPRGM_MAIN_FILE									

#ifdef OSDBMARK_MAIN_FILE									

WORD aBookmark[11] = {10, CHAR_B, CHAR_l, CHAR_a, CHAR_d, CHAR_w, CHAR_i, CHAR_j, CHAR_z, CHAR_e, CHAR_r};
WORD aTime[6] = {4, CHAR_T, CHAR_i, CHAR_j, CHAR_d};
#endif //OSDBMARK_MAIN_FILE									

#ifdef SETUP_MAIN_FILE									

WORD aSetupSelect[12] = {10, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
#endif //SETUP_MAIN_FILE									

#ifdef MENU_MAIN_FILE									

WORD aMsgEnglish[10] = {6, CHAR_E, CHAR_n, CHAR_g, CHAR_e, CHAR_l, CHAR_s};
WORD aMsgChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_e, CHAR_s};
WORD aMsgJapanese[11] = {6, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_s};
WORD aMsgFrench[12] = {5, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_s};
WORD aMsgGerman[10] = {5, CHAR_D, CHAR_u, CHAR_i, CHAR_t, CHAR_s};
WORD aMsgItalian[12] = {9, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aMsgSpanish[11] = {6, CHAR_S, CHAR_p, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aMsgPortuguese[14] = {9, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_e, CHAR_e, CHAR_s};
WORD aMsgDutch[12] = {10, CHAR_N, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_l, CHAR_a, CHAR_n, CHAR_d, CHAR_s};
WORD aMsgPolish[9] = {5, CHAR_P, CHAR_o, CHAR_o, CHAR_l, CHAR_s};
WORD aMsgHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aMsgThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aMsgRussian[9] = {8, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgKorean[11] = {8, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aMsgCzech[6] = {5, CHAR_C, CHAR_z, CHAR_e, CHAR_c, CHAR_h};
WORD aMsgGreece[7] = {6, CHAR_G, CHAR_r, CHAR_e, CHAR_e, CHAR_c, CHAR_e};
WORD aMsgTurkey[7] = {6, CHAR_T, CHAR_u, CHAR_r, CHAR_k, CHAR_e, CHAR_y};
WORD aMsgOthers[9] = {7, CHAR_O, CHAR_v, CHAR_e, CHAR_r, CHAR_i, CHAR_g, CHAR_e};
WORD aMsgOSD[4] = {3, CHAR_O, CHAR_S, CHAR_D};
WORD aMsgMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aMsgAudio[6] = {5, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMsgSubtitle[12] = {10, CHAR_O, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aMsgPS[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_P, CHAR_S};
WORD aMsgLB[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_L, CHAR_B};
WORD aMsgWide[5] = {4, CHAR_1, CHAR_6, CHAR_COLON, CHAR_9};
WORD aMsgOn[9] = {3, CHAR_A, CHAR_a, CHAR_n};
WORD aMsgOff[10] = {3, CHAR_U, CHAR_i, CHAR_t};
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
WORD aMsgBriNormal[9] = {7, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_a, CHAR_l};
WORD aMsgBright[10] = {6, CHAR_H, CHAR_e, CHAR_l, CHAR_d, CHAR_e, CHAR_r};
WORD aMsgSoft[8] = {5, CHAR_Z, CHAR_a, CHAR_c, CHAR_h, CHAR_t};
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
WORD aMsgTVType[14] = {13, CHAR_T, CHAR_e, CHAR_l, CHAR_e, CHAR_v, CHAR_i, CHAR_s, CHAR_i, CHAR_e, CHAR_t, CHAR_y, CHAR_p, CHAR_e};
WORD aMsgScreenSaver[18] = {17, CHAR_S, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_m, CHAR_b, CHAR_e, CHAR_v, CHAR_e, CHAR_i, CHAR_l, CHAR_i, CHAR_g, CHAR_i, CHAR_n, CHAR_g};
WORD aMsgTVSystem[17] = {16, CHAR_T, CHAR_e, CHAR_l, CHAR_e, CHAR_v, CHAR_i, CHAR_s, CHAR_i, CHAR_e, CHAR_s, CHAR_y, CHAR_s, CHAR_t, CHAR_e, CHAR_e, CHAR_m};
WORD aMsgVideoOutput[14] = {13, CHAR_V, CHAR_i, CHAR_d, CHAR_e, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_u, CHAR_i, CHAR_t, CHAR_g, CHAR_a, CHAR_n, CHAR_g};
WORD aMsgBrightness[11] = {10, CHAR_H, CHAR_e, CHAR_l, CHAR_d, CHAR_e, CHAR_r, CHAR_h, CHAR_e, CHAR_i, CHAR_d};
WORD aMsgContrast[10] = {8, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMsgChromPhase[4] = {3, CHAR_H, CHAR_u, CHAR_e};
WORD aMsgSPDIF_PCM[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_P, CHAR_C, CHAR_M};
WORD aMsgSPDIF_RAW[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_R, CHAR_A, CHAR_W};
WORD aMsg2CH[11] = {9, CHAR_2, CHAR_SPACE, CHAR_k, CHAR_a, CHAR_n, CHAR_a, CHAR_l, CHAR_e, CHAR_n};
WORD aMsg51CH[13] = {11, CHAR_5, CHAR_FULL_STOP, CHAR_1, CHAR_SPACE, CHAR_k, CHAR_a, CHAR_n, CHAR_a, CHAR_l, CHAR_e, CHAR_n};
WORD aMsgLoRo[6] = {5, CHAR_L, CHAR_o, CHAR_SOLIDUS, CHAR_R, CHAR_o};
WORD aMsgLtRt[6] = {5, CHAR_L, CHAR_t, CHAR_SOLIDUS, CHAR_R, CHAR_t};
#ifndef REMOVE_SETUP_SPEAKER_TEST									

WORD aMsgAutoTest[5] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aMsgLeft[6] = {5, CHAR_L, CHAR_i, CHAR_n, CHAR_k, CHAR_s};
WORD aMsgRight[7] = {6, CHAR_R, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s};
WORD aMsgCntr[10] = {6, CHAR_M, CHAR_i, CHAR_d, CHAR_d, CHAR_e, CHAR_n};
WORD aMsgLS[15] = {14, CHAR_L, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_o, CHAR_m, CHAR_g, CHAR_e, CHAR_v, CHAR_i, CHAR_n, CHAR_g};
WORD aMsgRS[16] = {15, CHAR_R, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_o, CHAR_m, CHAR_g, CHAR_e, CHAR_v, CHAR_i, CHAR_n, CHAR_g};
WORD aMsgSW[10] = {9, CHAR_S, CHAR_u, CHAR_b, CHAR_w, CHAR_o, CHAR_o, CHAR_f, CHAR_e, CHAR_r};
#endif //REMOVE_SETUP_SPEAKER_TEST									

WORD aMsgDigitalOut[18] = {12, CHAR_D, CHAR_i, CHAR_g, CHAR_i, CHAR_t, CHAR_a, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_u, CHAR_i, CHAR_t};
WORD aMsgAnalogOut[18] = {11, CHAR_A, CHAR_n, CHAR_a, CHAR_l, CHAR_o, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_u, CHAR_i, CHAR_t};
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
WORD aMsgVirtual[10] = {8, CHAR_V, CHAR_i, CHAR_r, CHAR_t, CHAR_u, CHAR_e, CHAR_e, CHAR_l};
WORD aMsgMusic[7] = {6, CHAR_M, CHAR_u, CHAR_z, CHAR_i, CHAR_e, CHAR_k};
WORD aMsgMovie[6] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aMsgMatrix[8] = {6, CHAR_M, CHAR_a, CHAR_t, CHAR_r, CHAR_i, CHAR_x};
WORD aMsgPrologic[10] = {9, CHAR_P, CHAR_R, CHAR_O, CHAR_SPACE, CHAR_L, CHAR_O, CHAR_G, CHAR_I, CHAR_C};
WORD aMsgDownMix[8] = {7, CHAR_D, CHAR_o, CHAR_w, CHAR_n, CHAR_m, CHAR_i, CHAR_x};
WORD aMsgCenter[10] = {6, CHAR_M, CHAR_i, CHAR_d, CHAR_d, CHAR_e, CHAR_n};
WORD aMsgRear[11] = {6, CHAR_A, CHAR_c, CHAR_h, CHAR_t, CHAR_e, CHAR_r};
WORD aMsgSubwoofer[10] = {9, CHAR_S, CHAR_u, CHAR_b, CHAR_w, CHAR_o, CHAR_o, CHAR_f, CHAR_e, CHAR_r};
WORD aMsgSpeakerTest[19] = {15, CHAR_L, CHAR_u, CHAR_i, CHAR_d, CHAR_s, CHAR_p, CHAR_r, CHAR_e, CHAR_k, CHAR_e, CHAR_r, CHAR_t, CHAR_e, CHAR_s, CHAR_t};
WORD aMsgLevel1[2] = {1, CHAR_1};
WORD aMsgLevel2[2] = {1, CHAR_2};
WORD aMsgLevel3[2] = {1, CHAR_3};
WORD aMsgLevel4[2] = {1, CHAR_4};
WORD aMsgLevel5[2] = {1, CHAR_5};
WORD aMsgLevel6[2] = {1, CHAR_6};
WORD aMsgLevel7[2] = {1, CHAR_7};
WORD aMsgLevel8[2] = {1, CHAR_8};
WORD aMsgRead[8] = {5, CHAR_L, CHAR_e, CHAR_z, CHAR_e, CHAR_n};
WORD aMsgParental[20] = {10, CHAR_K, CHAR_i, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_s, CHAR_l, CHAR_o, CHAR_t};
WORD aMsgChangePassword[13] = {10, CHAR_W, CHAR_a, CHAR_c, CHAR_h, CHAR_t, CHAR_w, CHAR_o, CHAR_o, CHAR_r, CHAR_d};
WORD aMsgAngleMark[16] = {13, CHAR_H, CHAR_o, CHAR_e, CHAR_k, CHAR_m, CHAR_a, CHAR_r, CHAR_k, CHAR_e, CHAR_r, CHAR_i, CHAR_n, CHAR_g};
WORD aMsgUIStyle[7] = {5, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_l};
WORD aMsgUIStyle1[8] = {7, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_l, CHAR_SPACE, CHAR_1};
WORD aMsgUIStyle2[8] = {7, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_l, CHAR_SPACE, CHAR_2};
WORD aMsgUIStyle3[8] = {7, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_l, CHAR_SPACE, CHAR_3};
WORD aMsgUIStyle4[8] = {7, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_l, CHAR_SPACE, CHAR_4};
WORD aMsgSmartPlay[14] = {13, CHAR_S, CHAR_l, CHAR_i, CHAR_m, CHAR_SPACE, CHAR_a, CHAR_f, CHAR_s, CHAR_p, CHAR_e, CHAR_l, CHAR_e, CHAR_n};
WORD aMsgDivXVOD[9] = {8, CHAR_D, CHAR_i, CHAR_v, CHAR_X, CHAR_SPACE, CHAR_V, CHAR_O, CHAR_D};
WORD aMsgDefaultValue[18] = {9, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_a, CHAR_r, CHAR_d};
WORD aMsgLanguageSetup[23] = {14, CHAR_T, CHAR_a, CHAR_a, CHAR_l, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_n, CHAR_g};
WORD aMsgScreenSetup[22] = {16, CHAR_S, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_m, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_n, CHAR_g};
WORD aMsgAudioSetup[22] = {15, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_n, CHAR_g};
WORD aMsgCustomSetup[28] = {24, CHAR_I, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_n, CHAR_g, CHAR_SPACE, CHAR_v, CHAR_a, CHAR_n, CHAR_SPACE, CHAR_g, CHAR_e, CHAR_b, CHAR_r, CHAR_u, CHAR_i, CHAR_k, CHAR_e, CHAR_r};
WORD aMsgExitSetup[30] = {23, CHAR_I, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_n, CHAR_g, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_e4, CHAR_i, CHAR_n, CHAR_d, CHAR_i, CHAR_g, CHAR_e, CHAR_n};
WORD aMsgPassword[31] = {22, CHAR_G, CHAR_e, CHAR_e, CHAR_f, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_t, CHAR_SPACE, CHAR_w, CHAR_a, CHAR_c, CHAR_h, CHAR_t, CHAR_w, CHAR_o, CHAR_o, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_o, CHAR_p};
WORD aMsgWrongPassword[20] = {19, CHAR_V, CHAR_e, CHAR_r, CHAR_k, CHAR_e, CHAR_e, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_w, CHAR_a, CHAR_c, CHAR_h, CHAR_t, CHAR_w, CHAR_o, CHAR_o, CHAR_r, CHAR_d};
WORD aMsgOldPassword[9] = {4, CHAR_O, CHAR_u, CHAR_d, CHAR_COLON};
WORD aMsgNewPassword[10] = {6, CHAR_N, CHAR_i, CHAR_e, CHAR_u, CHAR_w, CHAR_COLON};
WORD aMsgVerifyPassword[12] = {10, CHAR_V, CHAR_e, CHAR_r, CHAR_i, CHAR_f, CHAR_i, CHAR_e4, CHAR_r, CHAR_e, CHAR_n};
WORD aMsgStandard[12] = {9, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_a, CHAR_r, CHAR_d};
WORD aMsgClassical[10] = {8, CHAR_K, CHAR_l, CHAR_a, CHAR_s, CHAR_s, CHAR_i, CHAR_e, CHAR_k};
WORD aMsgRock[5] = {4, CHAR_R, CHAR_o, CHAR_c, CHAR_k};
WORD aMsgJazz[5] = {4, CHAR_J, CHAR_a, CHAR_z, CHAR_z};
WORD aMsgAcoustic[11] = {10, CHAR_A, CHAR_k, CHAR_o, CHAR_e, CHAR_s, CHAR_t, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgPop[4] = {3, CHAR_P, CHAR_o, CHAR_p};
WORD aMsgCustom[7] = {6, CHAR_C, CHAR_u, CHAR_s, CHAR_t, CHAR_o, CHAR_m};
#ifdef SUPPORT_PSCAN_PROMPT_SETUP									

WORD aMsgPleaseMake[39] = {38, CHAR_U, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_t, CHAR_e, CHAR_l, CHAR_e, CHAR_v, CHAR_i, CHAR_s, CHAR_i, CHAR_e, CHAR_SPACE, CHAR_o, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_e, CHAR_u, CHAR_n, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_c, CHAR_a, CHAR_n, CHAR_QUESTION_MARK};
#endif SUPPORT_PSCAN_PROMPT_SETUP									

#endif //MENU_MAIN_FILE									

#ifdef IMAGE_FRAME_MENU_MAIN_FILE									

WORD aMENUPhotoSetting[26] = {15, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_n, CHAR_g};
WORD aMENUIntervalTime[20] = {12, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l, CHAR_t, CHAR_i, CHAR_j, CHAR_d};
#ifndef MUSTEK_SETUP_MENU									

WORD aMENU5Sec[7] = {6, CHAR_5, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_c, CHAR_FULL_STOP};
WORD aMENU10Sec[8] = {7, CHAR_1, CHAR_0, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_c, CHAR_FULL_STOP};
#endif //MUSTEK_SETUP_MENU									

WORD aMENU15Sec[8] = {7, CHAR_1, CHAR_5, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_c, CHAR_FULL_STOP};
WORD aMENU30Sec[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_c, CHAR_FULL_STOP};
WORD aMENU1Min[7] = {6, CHAR_1, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU3Min[7] = {6, CHAR_3, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENUTransitionEffect[21] = {15, CHAR_O, CHAR_v, CHAR_e, CHAR_r, CHAR_g, CHAR_a, CHAR_n, CHAR_g, CHAR_s, CHAR_e, CHAR_f, CHAR_f, CHAR_e, CHAR_c, CHAR_t};
WORD aMENUJPEGEffectNone[12] = {11, CHAR_G, CHAR_e, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_e, CHAR_f, CHAR_f, CHAR_e, CHAR_c, CHAR_t};
WORD aMENUJPEGEffectCoverUp[17] = {15, CHAR_O, CHAR_m, CHAR_h, CHAR_o, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectCoverDown[18] = {15, CHAR_O, CHAR_m, CHAR_l, CHAR_a, CHAR_a, CHAR_g, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
#ifndef MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectCoverLeft[16] = {14, CHAR_L, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectCoverRight[17] = {15, CHAR_R, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectUncoverUp[18] = {16, CHAR_O, CHAR_m, CHAR_h, CHAR_o, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_o, CHAR_n, CHAR_t, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectUncoverDown[17] = {16, CHAR_O, CHAR_m, CHAR_l, CHAR_a, CHAR_a, CHAR_g, CHAR_SPACE, CHAR_o, CHAR_n, CHAR_t, CHAR_d, CHAR_e, CHAR_k, CHAR_k, CHAR_e, CHAR_n};
#endif //MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectUncoverLeft[16] = {15, CHAR_L, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_o, CHAR_n, CHAR_t, CHAR_h, CHAR_u, CHAR_l, CHAR_l, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectUncoverRight[17] = {16, CHAR_R, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_o, CHAR_n, CHAR_t, CHAR_h, CHAR_u, CHAR_l, CHAR_l, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectWipeUp[20] = {12, CHAR_O, CHAR_m, CHAR_h, CHAR_o, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_g, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectWipeDown[17] = {12, CHAR_O, CHAR_m, CHAR_l, CHAR_a, CHAR_a, CHAR_g, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_g, CHAR_e, CHAR_n};
#ifndef MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectWipeLeft[17] = {16, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_g, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectWipeRight[18] = {17, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_g, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectPushUp[14] = {12, CHAR_O, CHAR_m, CHAR_h, CHAR_o, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_d, CHAR_u, CHAR_w, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectPushDown[13] = {12, CHAR_O, CHAR_m, CHAR_l, CHAR_a, CHAR_a, CHAR_g, CHAR_SPACE, CHAR_d, CHAR_u, CHAR_w, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectPushLeft[17] = {16, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_d, CHAR_u, CHAR_w, CHAR_e, CHAR_n};
#endif //MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectPushRight[18] = {17, CHAR_N, CHAR_a, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_d, CHAR_u, CHAR_w, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectFadeThroughBlack[26] = {19, CHAR_D, CHAR_o, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_z, CHAR_w, CHAR_a, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_u, CHAR_i, CHAR_t, CHAR_f, CHAR_a, CHAR_d, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectRandom[16] = {11, CHAR_W, CHAR_i, CHAR_l, CHAR_l, CHAR_e, CHAR_k, CHAR_e, CHAR_u, CHAR_r, CHAR_i, CHAR_g};
WORD aMENUBrowse[12] = {6, CHAR_B, CHAR_l, CHAR_a, CHAR_d, CHAR_e, CHAR_r};
WORD aMENUFileManager[16] = {11, CHAR_B, CHAR_e, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_b, CHAR_e, CHAR_h, CHAR_e, CHAR_e, CHAR_r};
WORD aMENUSetupMenu[16] = {9, CHAR_I, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_m, CHAR_e, CHAR_n, CHAR_u};
WORD aMENUThumbnail[13] = {9, CHAR_M, CHAR_i, CHAR_n, CHAR_i, CHAR_a, CHAR_t, CHAR_u, CHAR_u, CHAR_r};
WORD aMENUAutoPlay[14] = {9, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_a, CHAR_f, CHAR_s, CHAR_p};
WORD aMENUPhoto[12] = {4, CHAR_F, CHAR_o, CHAR_t, CHAR_o};
WORD aMENUMusic[7] = {4, CHAR_M, CHAR_u, CHAR_z, CHAR_FULL_STOP};
WORD aMENUPhotoAudio[14] = {10, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_SOLIDUS, CHAR_a, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMENUMovie[10] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aMENUDisplaySetup[16] = {10, CHAR_B, CHAR_e, CHAR_e, CHAR_l, CHAR_d, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aMENUDisplayType[14] = {9, CHAR_B, CHAR_e, CHAR_e, CHAR_l, CHAR_d, CHAR_t, CHAR_y, CHAR_p, CHAR_e};
WORD aMENUTVStandard[12] = {9, CHAR_T, CHAR_v, CHAR_HYPHEN_MINUS, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_FULL_STOP};
WORD aMENUNTSC[5] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aMENUPAL[4] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aMENUAUTO[11] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aMENUBrightness[11] = {10, CHAR_H, CHAR_e, CHAR_l, CHAR_d, CHAR_e, CHAR_r, CHAR_h, CHAR_e, CHAR_i, CHAR_d};
WORD aMENUContrast[10] = {8, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMENUNormal[9] = {7, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_a, CHAR_l};
WORD aMENUBright[10] = {6, CHAR_H, CHAR_e, CHAR_l, CHAR_d, CHAR_e, CHAR_r};
WORD aMENUSoft[8] = {5, CHAR_Z, CHAR_a, CHAR_c, CHAR_h, CHAR_t};
WORD aMENUTFTBrightness[15] = {14, CHAR_T, CHAR_F, CHAR_T, CHAR_HYPHEN_MINUS, CHAR_h, CHAR_e, CHAR_l, CHAR_d, CHAR_e, CHAR_r, CHAR_h, CHAR_e, CHAR_i, CHAR_d};
WORD aMENUTFTContrast[14] = {12, CHAR_T, CHAR_F, CHAR_T, CHAR_HYPHEN_MINUS, CHAR_c, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMENUTFTSaturation[16] = {15, CHAR_T, CHAR_F, CHAR_T, CHAR_HYPHEN_MINUS, CHAR_v, CHAR_e, CHAR_r, CHAR_z, CHAR_a, CHAR_d, CHAR_i, CHAR_g, CHAR_i, CHAR_n, CHAR_g};
WORD aMENURepeatMode[15] = {11, CHAR_H, CHAR_e, CHAR_r, CHAR_h, CHAR_a, CHAR_a, CHAR_l, CHAR_m, CHAR_o, CHAR_d, CHAR_FULL_STOP};
WORD aMENURepeatOne[9] = {3, CHAR_E, CHAR_e2, CHAR_n};
WORD aMENURepeatFolder[9] = {3, CHAR_M, CHAR_a, CHAR_p};
WORD aMENURepeatAll[10] = {5, CHAR_A, CHAR_l, CHAR_l, CHAR_e, CHAR_s};
WORD aMENUCustomSetup[28] = {24, CHAR_I, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_n, CHAR_g, CHAR_SPACE, CHAR_v, CHAR_a, CHAR_n, CHAR_SPACE, CHAR_g, CHAR_e, CHAR_b, CHAR_r, CHAR_u, CHAR_i, CHAR_k, CHAR_e, CHAR_r};
WORD aMENUOSDLanguage[14] = {8, CHAR_O, CHAR_S, CHAR_D, CHAR_HYPHEN_MINUS, CHAR_t, CHAR_a, CHAR_a, CHAR_l};
WORD aMENUEnglish[10] = {6, CHAR_E, CHAR_n, CHAR_g, CHAR_e, CHAR_l, CHAR_s};
WORD aMENUChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_e, CHAR_s};
WORD aMENUJapanese[11] = {6, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_s};
WORD aMENUKorean[11] = {8, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aMENUFrench[12] = {5, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_s};
WORD aMENUGerman[10] = {5, CHAR_D, CHAR_u, CHAR_i, CHAR_t, CHAR_s};
WORD aMENUItalian[12] = {9, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aMENUSpanish[11] = {6, CHAR_S, CHAR_p, CHAR_a, CHAR_a, CHAR_n, CHAR_s};
WORD aMENUPortuguese[14] = {9, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_e, CHAR_e, CHAR_s};
WORD aMENUDutch[12] = {10, CHAR_N, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_l, CHAR_a, CHAR_n, CHAR_d, CHAR_s};
WORD aMENUPolish[9] = {5, CHAR_P, CHAR_o, CHAR_o, CHAR_l, CHAR_s};
WORD aMENUUIStyle[7] = {5, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_l};
WORD aMENUUIStyle1[8] = {7, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_l, CHAR_SPACE, CHAR_1};
WORD aMENUUIStyle2[8] = {7, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_l, CHAR_SPACE, CHAR_2};
WORD aMENUUIStyle3[8] = {7, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_l, CHAR_SPACE, CHAR_3};
WORD aMENUUIStyle4[8] = {7, CHAR_S, CHAR_t, CHAR_i, CHAR_j, CHAR_l, CHAR_SPACE, CHAR_4};
WORD aMENUDefault[18] = {9, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_a, CHAR_r, CHAR_d};
WORD aMENURead[8] = {5, CHAR_L, CHAR_e, CHAR_z, CHAR_e, CHAR_n};
WORD aMENUIconExit[6] = {4, CHAR_A, CHAR_f, CHAR_s, CHAR_l};
WORD aMENUHelpSelect[10] = {9, CHAR_COLON, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t, CHAR_FULL_STOP};
WORD aMENUHelpConfirm[11] = {9, CHAR_COLON, CHAR_SPACE, CHAR_B, CHAR_e, CHAR_v, CHAR_e, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aMENUHelpExit[18] = {17, CHAR_A, CHAR_f, CHAR_s, CHAR_l, CHAR_COLON, CHAR_O, CHAR_p, CHAR_s, CHAR_l, CHAR_FULL_STOP, CHAR_AMPERSAND, CHAR_SPACE, CHAR_a, CHAR_f, CHAR_s, CHAR_l, CHAR_FULL_STOP};
#endif //#ifdef IMAGE_FRAME_MENU_MAIN_FILE									

#ifdef SUPPORT_POWERON_MENU									

WORD aPOWERONIconPhoto[8] = {4, CHAR_F, CHAR_o, CHAR_t, CHAR_o};
WORD aPOWERONIconMusic[7] = {4, CHAR_M, CHAR_u, CHAR_z, CHAR_FULL_STOP};
WORD aPOWERONIconPhotoMusic[15] = {11, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_PLUS_SIGN, CHAR_M, CHAR_u, CHAR_z, CHAR_i, CHAR_e, CHAR_k};
WORD aPOWERONIconMovie[10] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aPOWERONIconSetup[14] = {5, CHAR_I, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aPOWERONIconCalendar[11] = {6, CHAR_A, CHAR_g, CHAR_e, CHAR_n, CHAR_d, CHAR_a};
WORD aPOWERONIconClock[9] = {4, CHAR_K, CHAR_l, CHAR_o, CHAR_k};
WORD aPOWERONIconAlarm[7] = {5, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m};
WORD aPOWERONIconAutoPower[14] = {10, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_i, CHAR_n, CHAR_s, CHAR_c, CHAR_h};
WORD aPOWERONIconEdit[11] = {8, CHAR_B, CHAR_e, CHAR_w, CHAR_e, CHAR_r, CHAR_k, CHAR_e, CHAR_n};
WORD aPOWERONIconFavorite[10] = {8, CHAR_F, CHAR_a, CHAR_v, CHAR_o, CHAR_r, CHAR_i, CHAR_e, CHAR_t};
WORD aPOWERONIconRadio[6] = {5, CHAR_R, CHAR_a, CHAR_d, CHAR_i, CHAR_o};
WORD aWeeklyDay_Mon[4] = {2, CHAR_m, CHAR_a};
WORD aWeeklyDay_Tue[4] = {2, CHAR_d, CHAR_i};
WORD aWeeklyDay_Wed[4] = {2, CHAR_w, CHAR_o};
WORD aWeeklyDay_Thu[4] = {2, CHAR_d, CHAR_o};
WORD aWeeklyDay_Fri[4] = {2, CHAR_v, CHAR_r};
WORD aWeeklyDay_Sat[4] = {2, CHAR_z, CHAR_a};
WORD aWeeklyDay_Sun[4] = {2, CHAR_z, CHAR_o};
WORD aMonthJanuary[4] = {3, CHAR_j, CHAR_a, CHAR_n};
WORD aMonthFebruary[4] = {3, CHAR_f, CHAR_e, CHAR_b};
WORD aMonthMarch[4] = {3, CHAR_m, CHAR_r, CHAR_t};
WORD aMonthApril[4] = {3, CHAR_a, CHAR_p, CHAR_r};
WORD aMonthMay[4] = {3, CHAR_m, CHAR_e, CHAR_i};
WORD aMonthJune[4] = {3, CHAR_j, CHAR_u, CHAR_n};
WORD aMonthJuly[4] = {3, CHAR_j, CHAR_u, CHAR_l};
WORD aMonthAugust[4] = {3, CHAR_a, CHAR_u, CHAR_g};
WORD aMonthSeptember[4] = {3, CHAR_s, CHAR_e, CHAR_p};
WORD aMonthOctober[4] = {3, CHAR_o, CHAR_k, CHAR_t};
WORD aMonthNovember[4] = {3, CHAR_n, CHAR_o, CHAR_v};
WORD aMonthDecember[4] = {3, CHAR_d, CHAR_e, CHAR_c};
WORD aWeeklyMonday[14] = {7, CHAR_m, CHAR_a, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_g};
WORD aWeeklyTuesday[12] = {7, CHAR_d, CHAR_i, CHAR_n, CHAR_s, CHAR_d, CHAR_a, CHAR_g};
WORD aWeeklyWednesday[13] = {8, CHAR_w, CHAR_o, CHAR_e, CHAR_n, CHAR_s, CHAR_d, CHAR_a, CHAR_g};
WORD aWeeklyThursday[13] = {9, CHAR_d, CHAR_o, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_d, CHAR_a, CHAR_g};
WORD aWeeklyFriday[12] = {7, CHAR_v, CHAR_r, CHAR_i, CHAR_j, CHAR_d, CHAR_a, CHAR_g};
WORD aWeeklySaturday[9] = {8, CHAR_z, CHAR_a, CHAR_t, CHAR_e, CHAR_r, CHAR_d, CHAR_a, CHAR_g};
WORD aWeeklySunday[10] = {6, CHAR_z, CHAR_o, CHAR_n, CHAR_d, CHAR_a, CHAR_g};
WORD aCLOCKClockSetting[22] = {9, CHAR_K, CHAR_l, CHAR_o, CHAR_k, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aCLOCKDisplay[13] = {8, CHAR_W, CHAR_e, CHAR_e, CHAR_r, CHAR_g, CHAR_a, CHAR_v, CHAR_e};
WORD aCLOCKSetTime[14] = {10, CHAR_T, CHAR_i, CHAR_j, CHAR_d, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aCLOCKExit[6] = {4, CHAR_A, CHAR_f, CHAR_s, CHAR_l};
WORD aCLOCKBigClock[9] = {4, CHAR_K, CHAR_l, CHAR_o, CHAR_k};
WORD aCLOCKClockCalendar[22] = {15, CHAR_K, CHAR_l, CHAR_o, CHAR_k, CHAR_SPACE, CHAR_AMPERSAND, CHAR_SPACE, CHAR_k, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_r};
WORD aBUTTONOK[3] = {2, CHAR_O, CHAR_K};
WORD aBUTTONCancel[9] = {6, CHAR_A, CHAR_n, CHAR_n, CHAR_u, CHAR_l, CHAR_FULL_STOP};
WORD aSETTIMESetTime[15] = {10, CHAR_T, CHAR_i, CHAR_j, CHAR_d, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aALARMEnable[10] = {6, CHAR_I, CHAR_n, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP};
WORD aALARMAlarmSetting[21] = {10, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aALARMDlgTitle[8] = {5, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m};
WORD aALARMDlgContent[34] = {27, CHAR_T, CHAR_o, CHAR_e, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_d, CHAR_r, CHAR_FULL_STOP, CHAR_SPACE, CHAR_v, CHAR_o, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_a, CHAR_r, CHAR_m, CHAR_s, CHAR_t, CHAR_o, CHAR_p, CHAR_FULL_STOP};
WORD aAUTOPWRAutoPowerSetting[46] = {26, CHAR_I, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SOLIDUS, CHAR_u, CHAR_i, CHAR_t, CHAR_s, CHAR_c, CHAR_h, CHAR_a, CHAR_k, CHAR_e, CHAR_l, CHAR_e, CHAR_n};
WORD aAUTOPWRPowerOn[11] = {6, CHAR_I, CHAR_n, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP};
WORD aAUTOPWRPowerOff[12] = {7, CHAR_U, CHAR_i, CHAR_t, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP};
WORD aAUTOPWRPowerOnEnable[30] = {16, CHAR_I, CHAR_n, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_c, CHAR_t, CHAR_i, CHAR_v, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aAUTOPWRPowerOffEnable[33] = {17, CHAR_U, CHAR_i, CHAR_t, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_c, CHAR_t, CHAR_i, CHAR_v, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aAUTOPWRSetTime[14] = {10, CHAR_T, CHAR_i, CHAR_j, CHAR_d, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aMMBtnSave[10] = {7, CHAR_O, CHAR_p, CHAR_s, CHAR_l, CHAR_a, CHAR_a, CHAR_n};
WORD aMMSaveFileDlgTitle[20] = {15, CHAR_B, CHAR_e, CHAR_s, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_SPACE, CHAR_o, CHAR_p, CHAR_s, CHAR_l, CHAR_a, CHAR_a, CHAR_n};
WORD aMMSaveFileDlgContent[18] = {11, CHAR_E, CHAR_v, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_g, CHAR_e, CHAR_d, CHAR_u, CHAR_l, CHAR_d};
WORD aTHUMBButtonSort[10] = {8, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aTHUMBButtonDeleteAll[11] = {8, CHAR_A, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_w, CHAR_FULL_STOP};
WORD aTHUMBTitleEditMode[18] = {15, CHAR_B, CHAR_e, CHAR_w, CHAR_e, CHAR_r, CHAR_k, CHAR_i, CHAR_n, CHAR_g, CHAR_s, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aTHUMBTitleDeleteMode[19] = {18, CHAR_V, CHAR_e, CHAR_r, CHAR_w, CHAR_i, CHAR_j, CHAR_d, CHAR_e, CHAR_r, CHAR_i, CHAR_n, CHAR_g, CHAR_s, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aTHUMBHelpDeleteMode[14] = {13, CHAR_V, CHAR_e, CHAR_r, CHAR_w, CHAR_i, CHAR_j, CHAR_d, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_h, CHAR_e, CHAR_t};
WORD aTHUMBHelpSortMode[26] = {21, CHAR_D, CHAR_e, CHAR_SPACE, CHAR_f, CHAR_o, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_p, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_e, CHAR_n};
WORD aTHUMBTitleSortMode[18] = {12, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_e, CHAR_e, CHAR_r, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aEDITDialogTitle[27] = {23, CHAR_D, CHAR_i, CHAR_a, CHAR_l, CHAR_o, CHAR_o, CHAR_g, CHAR_v, CHAR_e, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_w, CHAR_e, CHAR_r, CHAR_k, CHAR_e, CHAR_n};
WORD aEDITDialogContent[38] = {25, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t, CHAR_e, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_B, CHAR_e, CHAR_w, CHAR_e, CHAR_r, CHAR_k, CHAR_i, CHAR_n, CHAR_g, CHAR_s, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aBUTTONCopy[9] = {4, CHAR_K, CHAR_o, CHAR_p, CHAR_FULL_STOP};
WORD aBUTTONDelete[9] = {5, CHAR_V, CHAR_e, CHAR_r, CHAR_w, CHAR_FULL_STOP};
WORD aSETDATESetDate[17] = {15, CHAR_D, CHAR_a, CHAR_t, CHAR_u, CHAR_m, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_e, CHAR_n};
#endif //#ifdef SUPPORT_POWERON_MENU									

WORD aMENUOn[5] = {3, CHAR_A, CHAR_a, CHAR_n};
WORD aMENUOff[7] = {3, CHAR_U, CHAR_i, CHAR_t};
#ifdef MUSTEK_SETUP_MENU									

WORD aMENU5Min[7] = {6, CHAR_5, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU30Min[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU1Hour[8] = {5, CHAR_1, CHAR_SPACE, CHAR_u, CHAR_u, CHAR_r};
WORD aMENU4Hours[9] = {6, CHAR_4, CHAR_SPACE, CHAR_u, CHAR_r, CHAR_e, CHAR_n};
WORD aMENU12Hours[10] = {7, CHAR_1, CHAR_2, CHAR_SPACE, CHAR_u, CHAR_r, CHAR_e, CHAR_n};
WORD aMENU1Day[9] = {5, CHAR_1, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_g};
WORD aMENUTransition[14] = {8, CHAR_O, CHAR_v, CHAR_e, CHAR_r, CHAR_g, CHAR_a, CHAR_n, CHAR_g};
WORD aMENUTVSystem[17] = {16, CHAR_T, CHAR_e, CHAR_l, CHAR_e, CHAR_v, CHAR_i, CHAR_s, CHAR_i, CHAR_e, CHAR_s, CHAR_y, CHAR_s, CHAR_t, CHAR_e, CHAR_e, CHAR_m};
WORD aMENUFullScreen[15] = {12, CHAR_V, CHAR_o, CHAR_l, CHAR_l, CHAR_FULL_STOP, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_m};
WORD aMENUReset[8] = {5, CHAR_R, CHAR_e, CHAR_s, CHAR_e, CHAR_t};
WORD aMENUConfirm[8] = {7, CHAR_B, CHAR_e, CHAR_v, CHAR_e, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aMENUExitSetup[30] = {23, CHAR_I, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_n, CHAR_g, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_e4, CHAR_i, CHAR_n, CHAR_d, CHAR_i, CHAR_g, CHAR_e, CHAR_n};
#endif //MUSTEK_SETUP_MENU											
#ifdef SUPPORT_JPEG_AUTO_AND_FULL_SCREEN
WORD aMENUFullScreen[15] = {12, CHAR_V, CHAR_o, CHAR_l, CHAR_l, CHAR_FULL_STOP, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_m};
#endif

WORD aBUTTONYes[4] = {2, CHAR_J, CHAR_a};
WORD aBUTTONNo[5] = {3, CHAR_N, CHAR_e, CHAR_e};
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW											

WORD aMMSaveDlgTitle[31] = {26, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_d, CHAR_i, CHAR_a, CHAR_l, CHAR_o, CHAR_o, CHAR_g, CHAR_v, CHAR_e, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_o, CHAR_p, CHAR_s, CHAR_l, CHAR_a, CHAR_a, CHAR_n};
WORD aMMDeleteDlgTitle[31] = {28, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_d, CHAR_i, CHAR_a, CHAR_l, CHAR_FULL_STOP, CHAR_v, CHAR_e, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_w, CHAR_i, CHAR_j, CHAR_d, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aMMSaveDlgContent[14] = {8, CHAR_O, CHAR_p, CHAR_s, CHAR_l, CHAR_a, CHAR_a, CHAR_n, CHAR_QUESTION_MARK};
WORD aMMDeleteDlgContent[13] = {12, CHAR_V, CHAR_e, CHAR_r, CHAR_w, CHAR_i, CHAR_j, CHAR_d, CHAR_e, CHAR_r, CHAR_e, CHAR_n, CHAR_QUESTION_MARK};
#endif //#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW									

#ifdef SUPPORT_CHOOSE_MEDIA									

WORD aMENUMedia[11] = {5, CHAR_M, CHAR_e, CHAR_d, CHAR_i, CHAR_a};
#endif //SUPPORT_CHOOSE_MEDIA									

#ifdef SUPPORT_PHOTO_BROWSE_MODE									

WORD aNDBrowseMode[26] = {9, CHAR_Z, CHAR_o, CHAR_e, CHAR_k, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
#endif //SUPPORT_PHOTO_BROWSE_MODE									

#ifdef SUPPORT_TFT_DIGITAL_MODE									

WORD aMENUAVOutput[13] = {10, CHAR_A, CHAR_V, CHAR_HYPHEN_MINUS, CHAR_u, CHAR_i, CHAR_t, CHAR_g, CHAR_a, CHAR_n, CHAR_g};
#endif //SUPPORT_TFT_DIGITAL_MODE									

WORD aMENUPanel[9] = {6, CHAR_P, CHAR_a, CHAR_n, CHAR_e, CHAR_e, CHAR_l};
#ifdef SUPPORT_RADIO_TUNER									

WORD aRADIORecord[9] = {7, CHAR_O, CHAR_p, CHAR_n, CHAR_e, CHAR_m, CHAR_e, CHAR_n};
WORD aRADIOScan[9] = {7, CHAR_S, CHAR_c, CHAR_a, CHAR_n, CHAR_n, CHAR_e, CHAR_n};
WORD aRADIOStationsFound[17] = {15, CHAR_s, CHAR_t, CHAR_a, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_g, CHAR_e, CHAR_v, CHAR_o, CHAR_n, CHAR_d, CHAR_e, CHAR_n, CHAR_FULL_STOP};
#endif //SUPPORT_RADIO_TUNER									

#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									

WORD aCODEWesternEuropean[17] = {13, CHAR_W, CHAR_e, CHAR_s, CHAR_t, CHAR_HYPHEN_MINUS, CHAR_E, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_e, CHAR_e, CHAR_s};
#endif   // SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									

#ifdef SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									

WORD aCODECentralEuropean[19] = {15, CHAR_M, CHAR_i, CHAR_d, CHAR_d, CHAR_e, CHAR_n, CHAR_HYPHEN_MINUS, CHAR_E, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_e, CHAR_e, CHAR_s};
#endif   // SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									

#ifdef SUPPORT_CHAR_ENCODING_GREEK									

WORD aCODEGreek[11] = {6, CHAR_G, CHAR_r, CHAR_i, CHAR_e, CHAR_k, CHAR_s};
#endif   // SUPPORT_CHAR_ENCODING_GREEK									

#ifdef SUPPORT_CHAR_ENCODING_HEBREW									

WORD aCODEHebrew[10] = {5, CHAR_H, CHAR_e, CHAR_b, CHAR_r, CHAR_FULL_STOP};
#endif   // SUPPORT_CHAR_ENCODING_HEBREW									

#ifdef SUPPORT_CHAR_ENCODING_TURKISH									

WORD aCODETurkish[9] = {5, CHAR_T, CHAR_u, CHAR_r, CHAR_k, CHAR_s};
#endif   // SUPPORT_CHAR_ENCODING_TURKISH									

#ifdef SUPPORT_CHAR_ENCODING_CP1250									

WORD aCODECP1250[8] = {7, CHAR_C, CHAR_P, CHAR_SPACE, CHAR_1, CHAR_2, CHAR_5, CHAR_0};
#endif   // SUPPORT_CHAR_ENCODING_CP1250									

#ifdef SUPPORT_CHAR_ENCODING_CYRILLIC									

WORD aCODECyrillic[11] = {10, CHAR_C, CHAR_y, CHAR_r, CHAR_i, CHAR_l, CHAR_l, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
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
