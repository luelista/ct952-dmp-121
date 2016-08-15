#include "stringdec.h"
#ifdef OSDDSPLY_MAIN_FILE									

WORD aDSPLYLanguageJapanese[11] = {8, CHAR_J, CHAR_a, CHAR_p, CHAR_o, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aDSPLYLanguageEnglish[10] = {9, CHAR_A, CHAR_n, CHAR_g, CHAR_i, CHAR_e, CHAR_l, CHAR_s, CHAR_k, CHAR_i};
WORD aDSPLYLanguageFrench[12] = {9, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_u, CHAR_s, CHAR_k, CHAR_i};
WORD aDSPLYLanguageGerman[10] = {9, CHAR_N, CHAR_i, CHAR_e, CHAR_m, CHAR_i, CHAR_e, CHAR_c, CHAR_k, CHAR_i};
WORD aDSPLYLanguageRussian[9] = {8, CHAR_R, CHAR_o, CHAR_s, CHAR_y, CHAR_j, CHAR_s, CHAR_k, CHAR_i};
WORD aDSPLYLanguageChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aDSPLYLanguageKorean[11] = {9, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aDSPLYLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aDSPLYLanguageThai[10] = {6, CHAR_T, CHAR_a, CHAR_j, CHAR_s, CHAR_k, CHAR_i};
WORD aDSPLYLanguageItalian[12] = {6, CHAR_W, CHAR_l1, CHAR_o, CHAR_s, CHAR_k, CHAR_i};
WORD aDSPLYLanguageSpanish[11] = {10, CHAR_H, CHAR_i, CHAR_s, CHAR_z, CHAR_p, CHAR_a, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aDSPLYLanguagePortuguese[14] = {11, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_a, CHAR_l, CHAR_s, CHAR_k, CHAR_i};
WORD aDSPLYLanguageOthers[9] = {4, CHAR_I, CHAR_n, CHAR_n, CHAR_e};
WORD aDSPLYOff[10] = {9, CHAR_W, CHAR_y, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_y};
#ifndef NO_DVD_DISPLAY_UI									

WORD aDSPLYTitleElapsed[17] = {16, CHAR_T, CHAR_y, CHAR_t, CHAR_u, CHAR_l1, CHAR_SPACE, CHAR_o, CHAR_d, CHAR_t, CHAR_w, CHAR_o, CHAR_r, CHAR_z, CHAR_o, CHAR_n, CHAR_y};
WORD aDSPLYTitleRemain[16] = {15, CHAR_T, CHAR_y, CHAR_t, CHAR_u, CHAR_l1, CHAR_SPACE, CHAR_p, CHAR_o, CHAR_z, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_l1, CHAR_y};
WORD aDSPLYChapterElapsed[16] = {12, CHAR_R, CHAR_o, CHAR_z, CHAR_d, CHAR_z, CHAR_FULL_STOP, CHAR_SPACE, CHAR_o, CHAR_d, CHAR_t, CHAR_w, CHAR_FULL_STOP};
WORD aDSPLYChapterRemain[15] = {14, CHAR_R, CHAR_o, CHAR_z, CHAR_d, CHAR_z, CHAR_FULL_STOP, CHAR_SPACE, CHAR_p, CHAR_o, CHAR_z, CHAR_o, CHAR_s, CHAR_t, CHAR_FULL_STOP};
#endif //NO_DVD_DISPLAY_UI									

WORD aDSPLYSingleElapsed[17] = {15, CHAR_P, CHAR_o, CHAR_j, CHAR_e, CHAR_d, CHAR_FULL_STOP, CHAR_SPACE, CHAR_o, CHAR_d, CHAR_t, CHAR_w, CHAR_o, CHAR_r, CHAR_z, CHAR_FULL_STOP};
WORD aDSPLYSingleRemain[16] = {14, CHAR_P, CHAR_o, CHAR_j, CHAR_e, CHAR_d, CHAR_FULL_STOP, CHAR_SPACE, CHAR_p, CHAR_o, CHAR_z, CHAR_o, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aDSPLYTotalElapsed[18] = {11, CHAR_R, CHAR_a, CHAR_z, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_o, CHAR_d, CHAR_t, CHAR_w, CHAR_FULL_STOP};
WORD aDSPLYTotalRemain[17] = {13, CHAR_R, CHAR_a, CHAR_z, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_p, CHAR_o, CHAR_z, CHAR_o, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aDSPLYRootMenu[16] = {14, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_n, CHAR_a, CHAR_d, CHAR_r, CHAR_z, CHAR_e5, CHAR_d, CHAR_n, CHAR_e};
WORD aDSPLYTitleMenu[12] = {10, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_T, CHAR_y, CHAR_t, CHAR_u, CHAR_l1};
#ifdef SUPPORT_NAVIGATOR									

WORD aDSPLYRepeatTitle[20] = {13, CHAR_P, CHAR_o, CHAR_w, CHAR_t, CHAR_o1, CHAR_r, CHAR_z, CHAR_SPACE, CHAR_t, CHAR_y, CHAR_t, CHAR_u, CHAR_l1};
WORD aDSPLYRepeatChapter[22] = {16, CHAR_P, CHAR_o, CHAR_w, CHAR_t, CHAR_o1, CHAR_r, CHAR_z, CHAR_SPACE, CHAR_r, CHAR_o, CHAR_z, CHAR_d, CHAR_z, CHAR_i, CHAR_a, CHAR_l1};
WORD aDSPLYRepeatDisc[19] = {12, CHAR_P, CHAR_o, CHAR_w, CHAR_t, CHAR_o1, CHAR_r, CHAR_z, CHAR_SPACE, CHAR_d, CHAR_y, CHAR_s, CHAR_k};
WORD aDSPLYRepeatOff[22] = {21, CHAR_P, CHAR_o, CHAR_w, CHAR_t, CHAR_a, CHAR_r, CHAR_z, CHAR_a, CHAR_n, CHAR_i, CHAR_e, CHAR_SPACE, CHAR_w, CHAR_y, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_e};
#endif //SUPPORT_NAVIGATOR									

WORD aDSPLYpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aDSPLYInterlace[15] = {8, CHAR_P, CHAR_r, CHAR_z, CHAR_e, CHAR_p, CHAR_l, CHAR_o, CHAR_t};
WORD aDSPLYResolution[15] = {14, CHAR_R, CHAR_o, CHAR_z, CHAR_d, CHAR_z, CHAR_i, CHAR_e, CHAR_l, CHAR_c, CHAR_z, CHAR_o, CHAR_s1, CHAR_c2, CHAR_COLON};
WORD aDSPLYDate[7] = {5, CHAR_D, CHAR_a, CHAR_t, CHAR_a, CHAR_COLON};
#ifdef SUPPORT_NAVIGATOR									

WORD aNGTitle[8] = {6, CHAR_T, CHAR_y, CHAR_t, CHAR_u, CHAR_l1, CHAR_COLON};
WORD aNGChapter[11] = {9, CHAR_R, CHAR_o, CHAR_z, CHAR_d, CHAR_z, CHAR_i, CHAR_a, CHAR_l1, CHAR_COLON};
WORD aNGAngle[13] = {4, CHAR_K, CHAR_a6, CHAR_t, CHAR_COLON};
WORD aNGAudio[7] = {6, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o, CHAR_COLON};
WORD aNGSubtitle1[13] = {9, CHAR_N, CHAR_a, CHAR_p, CHAR_i, CHAR_s, CHAR_y, CHAR_SPACE, CHAR_1, CHAR_COLON};
WORD aNGSubtitle2[13] = {9, CHAR_N, CHAR_a, CHAR_p, CHAR_i, CHAR_s, CHAR_y, CHAR_SPACE, CHAR_2, CHAR_COLON};
#endif //SUPPORT_NAVIGATOR									

#endif //OSDDSPLY_MAIN_FILE									

#ifdef OSDND_MAIN_FILE									

WORD aNDNoDisc[14] = {10, CHAR_B, CHAR_r, CHAR_a, CHAR_k, CHAR_SPACE, CHAR_d, CHAR_y, CHAR_s, CHAR_k, CHAR_u};
WORD aNDNoUSBDevice[14] = {13, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_U, CHAR_S, CHAR_B, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_v, CHAR_i, CHAR_c, CHAR_e};
WORD aNDOpen[7] = {6, CHAR_O, CHAR_t, CHAR_w, CHAR_o1, CHAR_r, CHAR_z};
WORD aNDClose[10] = {7, CHAR_Z, CHAR_a, CHAR_m, CHAR_k, CHAR_n, CHAR_i, CHAR_j};
WORD aNDReturn[8] = {6, CHAR_P, CHAR_o, CHAR_w, CHAR_r, CHAR_o1, CHAR_c2};
WORD aNDFreeze[15] = {9, CHAR_W, CHAR_s, CHAR_t, CHAR_r, CHAR_z, CHAR_y, CHAR_m, CHAR_a, CHAR_j};
WORD aNDRandomOn[12] = {10, CHAR_O, CHAR_d, CHAR_t, CHAR_w, CHAR_FULL_STOP, CHAR_SPACE, CHAR_l, CHAR_o, CHAR_s, CHAR_FULL_STOP};
WORD aNDRandomOff[12] = {10, CHAR_O, CHAR_d, CHAR_t, CHAR_w, CHAR_FULL_STOP, CHAR_SPACE, CHAR_k, CHAR_o, CHAR_l, CHAR_FULL_STOP};
WORD aNDResume[11] = {5, CHAR_W, CHAR_z, CHAR_n, CHAR_o1, CHAR_w};
WORD aNDRootMenu[16] = {14, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_n, CHAR_a, CHAR_d, CHAR_r, CHAR_z, CHAR_e5, CHAR_d, CHAR_n, CHAR_e};
WORD aNDTitleMenu[12] = {10, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_T, CHAR_y, CHAR_t, CHAR_u, CHAR_l1};
WORD aNDRotateRight[26] = {19, CHAR_O, CHAR_b, CHAR_r, CHAR_o1, CHAR_c2, CHAR_SPACE, CHAR_o, CHAR_SPACE, CHAR_9, CHAR_0, CHAR_N_DEGREE, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_o};
WORD aNDRotateLeft[26] = {18, CHAR_O, CHAR_b, CHAR_r, CHAR_o1, CHAR_c2, CHAR_SPACE, CHAR_o, CHAR_SPACE, CHAR_9, CHAR_0, CHAR_N_DEGREE, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_w, CHAR_o};
WORD aNDFlipHorizontally[26] = {19, CHAR_P, CHAR_r, CHAR_z, CHAR_e, CHAR_r, CHAR_z, CHAR_u, CHAR_c2, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_o, CHAR_z, CHAR_i, CHAR_o, CHAR_m, CHAR_i, CHAR_e};
WORD aNDFlipVertically[25] = {17, CHAR_P, CHAR_r, CHAR_z, CHAR_e, CHAR_r, CHAR_z, CHAR_u, CHAR_c2, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_i, CHAR_o, CHAR_n, CHAR_i, CHAR_e};
WORD aNDpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aNDInterlace[15] = {8, CHAR_P, CHAR_r, CHAR_z, CHAR_e, CHAR_p, CHAR_l, CHAR_o, CHAR_t};
WORD aNDRepeatNone[10] = {4, CHAR_W, CHAR_y, CHAR_l1, CHAR_FULL_STOP};
WORD aNDRepeatTrack[8] = {7, CHAR_S1, CHAR_c, CHAR_i, CHAR_e, CHAR_z1, CHAR_k, CHAR_a};
WORD aNDRepeatDisc[7] = {4, CHAR_D, CHAR_y, CHAR_s, CHAR_k};
WORD aNDRepeatAll[10] = {9, CHAR_W, CHAR_s, CHAR_z, CHAR_y, CHAR_s, CHAR_t, CHAR_k, CHAR_i, CHAR_e};
WORD aNDRepeatTitle[7] = {5, CHAR_T, CHAR_y, CHAR_t, CHAR_u, CHAR_l1};
WORD aNDRepeatChapter[10] = {8, CHAR_R, CHAR_o, CHAR_z, CHAR_d, CHAR_z, CHAR_i, CHAR_a, CHAR_l1};
WORD aNDRepeatFolder[9] = {6, CHAR_F, CHAR_o, CHAR_l, CHAR_d, CHAR_e, CHAR_r};
WORD aNDRepeatA[15] = {9, CHAR_P, CHAR_o, CHAR_w, CHAR_t, CHAR_o1, CHAR_r, CHAR_z, CHAR_SPACE, CHAR_A};
WORD aNDRepeatAB[17] = {11, CHAR_P, CHAR_o, CHAR_w, CHAR_t, CHAR_o1, CHAR_r, CHAR_z, CHAR_SPACE, CHAR_A, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aNDRepeatOff[22] = {21, CHAR_P, CHAR_o, CHAR_w, CHAR_t, CHAR_a, CHAR_r, CHAR_z, CHAR_a, CHAR_n, CHAR_i, CHAR_e, CHAR_SPACE, CHAR_w, CHAR_y, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_e};
#ifndef NO_DISC_MODE									

WORD aNDNavigateOn[25] = {18, CHAR_N, CHAR_a, CHAR_w, CHAR_i, CHAR_g, CHAR_a, CHAR_c, CHAR_j, CHAR_a, CHAR_SPACE, CHAR_w, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_a};
WORD aNDNavigateOff[28] = {19, CHAR_N, CHAR_a, CHAR_w, CHAR_i, CHAR_g, CHAR_a, CHAR_c, CHAR_j, CHAR_a, CHAR_SPACE, CHAR_w, CHAR_y, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_a};
WORD aNDPBCOn[13] = {12, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_w, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_e};
WORD aNDPBCOff[14] = {13, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_w, CHAR_y, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_e};
#endif //#ifndef NO_DISC_MODE									

WORD aNDZoom1[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_1};
WORD aNDZoom2[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_2};
WORD aNDZoom4[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_4};
WORD aNDMoveLeft[24] = {14, CHAR_P, CHAR_r, CHAR_z, CHAR_e, CHAR_s, CHAR_u, CHAR_n2, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_w, CHAR_o};
WORD aNDMoveRight[24] = {15, CHAR_P, CHAR_r, CHAR_z, CHAR_e, CHAR_s, CHAR_u, CHAR_n2, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_o};
WORD aNDMoveUp[23] = {14, CHAR_P, CHAR_r, CHAR_z, CHAR_e, CHAR_s, CHAR_u, CHAR_n2, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_g, CHAR_o1, CHAR_r, CHAR_e5};
WORD aNDMoveDown[23] = {13, CHAR_P, CHAR_r, CHAR_z, CHAR_e, CHAR_s, CHAR_u, CHAR_n2, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_o1, CHAR_l1};
WORD aNDVocalOn[15] = {14, CHAR_W, CHAR_o, CHAR_k, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_w, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_y};
WORD aNDVocalOff[16] = {15, CHAR_W, CHAR_o, CHAR_k, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_w, CHAR_y, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_y};
WORD aNDVocalLL[15] = {8, CHAR_W, CHAR_o, CHAR_k, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_L, CHAR_L};
WORD aNDVocalRR[13] = {8, CHAR_W, CHAR_o, CHAR_k, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_R, CHAR_R};
WORD aNDKaraokeOn[17] = {16, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_w, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_e};
WORD aNDKaraokeOff[18] = {17, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_w, CHAR_y, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_e};
WORD aNDJPEGEffectNone[12] = {11, CHAR_B, CHAR_r, CHAR_a, CHAR_k, CHAR_SPACE, CHAR_e, CHAR_f, CHAR_e, CHAR_k, CHAR_t, CHAR_u};
WORD aNDJPEGEffectCoverUp[27] = {15, CHAR_P, CHAR_r, CHAR_z, CHAR_y, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_g, CHAR_o1, CHAR_r, CHAR_e5};
WORD aNDJPEGEffectCoverDown[26] = {14, CHAR_P, CHAR_r, CHAR_z, CHAR_y, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_o1, CHAR_l1};
WORD aNDJPEGEffectCoverLeft[20] = {15, CHAR_P, CHAR_r, CHAR_z, CHAR_y, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_w, CHAR_o};
WORD aNDJPEGEffectCoverRight[20] = {16, CHAR_P, CHAR_r, CHAR_z, CHAR_y, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_o};
WORD aNDJPEGEffectUncoverUp[23] = {13, CHAR_O, CHAR_d, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_g, CHAR_o1, CHAR_r, CHAR_e5};
WORD aNDJPEGEffectUncoverDown[24] = {12, CHAR_O, CHAR_d, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_o1, CHAR_l1};
WORD aNDJPEGEffectUncoverLeft[21] = {13, CHAR_O, CHAR_d, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_w, CHAR_o};
WORD aNDJPEGEffectUncoverRight[22] = {14, CHAR_O, CHAR_d, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_o};
WORD aNDJPEGEffectWipeUp[22] = {14, CHAR_Z, CHAR_e, CHAR_t, CHAR_r, CHAR_z, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_g, CHAR_o1, CHAR_r, CHAR_e5};
WORD aNDJPEGEffectWipeDown[21] = {13, CHAR_Z, CHAR_e, CHAR_t, CHAR_r, CHAR_z, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_o1, CHAR_l1};
WORD aNDJPEGEffectWipeLeft[17] = {14, CHAR_Z, CHAR_e, CHAR_t, CHAR_r, CHAR_z, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_w, CHAR_o};
WORD aNDJPEGEffectWipeRight[18] = {15, CHAR_Z, CHAR_e, CHAR_t, CHAR_r, CHAR_z, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_o};
WORD aNDJPEGEffectPushUp[14] = {13, CHAR_P, CHAR_c, CHAR_h, CHAR_n, CHAR_i, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_g, CHAR_o1, CHAR_r, CHAR_e5};
WORD aNDJPEGEffectPushDown[13] = {12, CHAR_P, CHAR_c, CHAR_h, CHAR_n, CHAR_i, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_o1, CHAR_l1};
WORD aNDJPEGEffectPushLeft[17] = {13, CHAR_P, CHAR_c, CHAR_h, CHAR_n, CHAR_i, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_w, CHAR_o};
WORD aNDJPEGEffectPushRight[18] = {14, CHAR_P, CHAR_c, CHAR_h, CHAR_n, CHAR_i, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_o};
WORD aNDJPEGEffectFadeThroughBlack[26] = {21, CHAR_W, CHAR_y, CHAR_c, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_u, CHAR_j, CHAR_SPACE, CHAR_o, CHAR_d, CHAR_SPACE, CHAR_c, CHAR_z, CHAR_a, CHAR_r, CHAR_n, CHAR_e, CHAR_g, CHAR_o};
WORD aNDJPEGEffectRandom[16] = {10, CHAR_O, CHAR_d, CHAR_t, CHAR_w, CHAR_FULL_STOP, CHAR_SPACE, CHAR_l, CHAR_o, CHAR_s, CHAR_FULL_STOP};
#ifndef NO_DISC_MODE									

WORD aNDEncrypt[17] = {15, CHAR_N, CHAR_i, CHAR_e, CHAR_l, CHAR_e, CHAR_g, CHAR_a, CHAR_l, CHAR_n, CHAR_y, CHAR_SPACE, CHAR_d, CHAR_y, CHAR_s, CHAR_k};
WORD aNDErrRegionCode[27] = {26, CHAR_N, CHAR_i, CHAR_e, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_i, CHAR_d, CHAR_l1, CHAR_o, CHAR_w, CHAR_y, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_g, CHAR_i, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_d, CHAR_y, CHAR_s, CHAR_k, CHAR_u};
WORD aNDWrongDisc[19] = {18, CHAR_N, CHAR_i, CHAR_e, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_i, CHAR_d, CHAR_l1, CHAR_o, CHAR_w, CHAR_y, CHAR_SPACE, CHAR_d, CHAR_y, CHAR_s, CHAR_k};
WORD aNDDamageDisc[19] = {15, CHAR_U, CHAR_s, CHAR_z, CHAR_k, CHAR_o, CHAR_d, CHAR_z, CHAR_o, CHAR_n, CHAR_y, CHAR_SPACE, CHAR_d, CHAR_y, CHAR_s, CHAR_k};
#endif //#ifndef NO_DISC_MODE									

WORD aNDNoPlayableFile[25] = {19, CHAR_B, CHAR_r, CHAR_a, CHAR_k, CHAR_SPACE, CHAR_p, CHAR_l, CHAR_i, CHAR_k, CHAR_u, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_SPACE, CHAR_o, CHAR_d, CHAR_t, CHAR_w, CHAR_FULL_STOP};
WORD aNDDisc[7] = {4, CHAR_D, CHAR_y, CHAR_s, CHAR_k};
WORD aNDTrack[8] = {7, CHAR_S1, CHAR_c, CHAR_i, CHAR_e, CHAR_z1, CHAR_k, CHAR_a};
WORD aNDTitle[7] = {5, CHAR_T, CHAR_y, CHAR_t, CHAR_u, CHAR_l1};
WORD aNDChapter[10] = {8, CHAR_R, CHAR_o, CHAR_z, CHAR_d, CHAR_z, CHAR_i, CHAR_a, CHAR_l1};
WORD aNDLR[4] = {3, CHAR_L, CHAR_SOLIDUS, CHAR_R};
WORD aNDLL[4] = {3, CHAR_L, CHAR_SOLIDUS, CHAR_L};
WORD aNDRR[4] = {3, CHAR_R, CHAR_SOLIDUS, CHAR_R};
WORD aNDVolume[11] = {8, CHAR_G, CHAR_l1, CHAR_o, CHAR_s1, CHAR_n, CHAR_o, CHAR_s1, CHAR_c2};
WORD aNDTone[9] = {3, CHAR_T, CHAR_o, CHAR_n};
WORD aNDEcho[5] = {4, CHAR_E, CHAR_c, CHAR_h, CHAR_o};
WORD aNDNoSurround[19] = {13, CHAR_B, CHAR_r, CHAR_a, CHAR_k, CHAR_SPACE, CHAR_S, CHAR_u, CHAR_r, CHAR_r, CHAR_o, CHAR_u, CHAR_n, CHAR_d};
WORD aNDLiveSurround[10] = {7, CHAR_N, CHAR_a, CHAR_SPACE, CHAR_z1, CHAR_y, CHAR_w, CHAR_o};
WORD aNDTheaterSurround[8] = {5, CHAR_T, CHAR_e, CHAR_a, CHAR_t, CHAR_r};
WORD aNDStereoSurround[8] = {6, CHAR_S, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_o};
WORD aNDMovieSurround[9] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aNDKaraSurround[8] = {7, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e};
WORD aNDLanguageJapanese[11] = {8, CHAR_J, CHAR_a, CHAR_p, CHAR_o, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aNDLanguageEnglish[10] = {9, CHAR_A, CHAR_n, CHAR_g, CHAR_i, CHAR_e, CHAR_l, CHAR_s, CHAR_k, CHAR_i};
WORD aNDLanguageFrench[12] = {9, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_u, CHAR_s, CHAR_k, CHAR_i};
WORD aNDLanguageGerman[10] = {9, CHAR_N, CHAR_i, CHAR_e, CHAR_m, CHAR_i, CHAR_e, CHAR_c, CHAR_k, CHAR_i};
WORD aNDLanguageRussian[9] = {8, CHAR_R, CHAR_o, CHAR_s, CHAR_y, CHAR_j, CHAR_s, CHAR_k, CHAR_i};
WORD aNDLanguageChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aNDLanguageKorean[11] = {9, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aNDLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aNDLanguageThai[10] = {6, CHAR_T, CHAR_a, CHAR_j, CHAR_s, CHAR_k, CHAR_i};
WORD aNDLanguageItalian[12] = {6, CHAR_W, CHAR_l1, CHAR_o, CHAR_s, CHAR_k, CHAR_i};
WORD aNDLanguageSpanish[11] = {10, CHAR_H, CHAR_i, CHAR_s, CHAR_z, CHAR_p, CHAR_a, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aNDLanguagePortuguese[14] = {11, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_a, CHAR_l, CHAR_s, CHAR_k, CHAR_i};
WORD aNDLanguageOthers[9] = {4, CHAR_I, CHAR_n, CHAR_n, CHAR_e};
WORD aNDSubtitle[13] = {7, CHAR_N, CHAR_a, CHAR_p, CHAR_i, CHAR_s, CHAR_y, CHAR_COLON};
WORD aNDOff[10] = {9, CHAR_W, CHAR_y, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_e};
WORD aNDWait[12] = {9, CHAR_L1, CHAR_a, CHAR_d, CHAR_o, CHAR_w, CHAR_a, CHAR_n, CHAR_i, CHAR_e};
WORD aNDSelect[12] = {7, CHAR_W, CHAR_y, CHAR_b, CHAR_i, CHAR_e, CHAR_r, CHAR_z};
WORD aNDCardReader[29] = {12, CHAR_C, CHAR_z, CHAR_y, CHAR_t, CHAR_n, CHAR_i, CHAR_k, CHAR_SPACE, CHAR_k, CHAR_a, CHAR_r, CHAR_t};
#endif //OSDND_MAIN_FILE									

#ifdef OSDMM_MAIN_FILE									

WORD aMMProgramModeTitle[24] = {12, CHAR_T, CHAR_r, CHAR_y, CHAR_b, CHAR_SPACE, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m};
WORD aMMProgramListModeTitle[21] = {15, CHAR_L, CHAR_i, CHAR_s, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_o1, CHAR_w};
WORD aMMTotalFiles[25] = {13, CHAR_R, CHAR_a, CHAR_z, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_p, CHAR_l, CHAR_i, CHAR_k, CHAR_o1, CHAR_w, CHAR_COLON};
WORD aMMFileNumber[20] = {12, CHAR_N, CHAR_u, CHAR_m, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_p, CHAR_l, CHAR_i, CHAR_k, CHAR_u, CHAR_COLON};
WORD aMMDlgFileTypeTitle[14] = {10, CHAR_T, CHAR_Y, CHAR_P, CHAR_SPACE, CHAR_P, CHAR_L, CHAR_I, CHAR_K, CHAR_U, CHAR_COLON};
WORD aMMDlgSortFileTitle[16] = {12, CHAR_S, CHAR_O, CHAR_R, CHAR_T, CHAR_U, CHAR_J, CHAR_SPACE, CHAR_P, CHAR_L, CHAR_I, CHAR_K, CHAR_COLON};
WORD aMMDlgSortFileMethodTitle[9] = {7, CHAR_M, CHAR_e, CHAR_t, CHAR_o, CHAR_d, CHAR_a, CHAR_COLON};
WORD aMMDlgSortFileSequenceTitle[12] = {10, CHAR_K, CHAR_o, CHAR_l, CHAR_e, CHAR_j, CHAR_n, CHAR_o, CHAR_s1, CHAR_c2, CHAR_COLON};
WORD aMMDlgSequenceAscendingItem[10] = {7, CHAR_R, CHAR_o, CHAR_s, CHAR_n, CHAR_a6, CHAR_c, CHAR_a};
WORD aMMDlgSequenceDescendingItem[12] = {8, CHAR_M, CHAR_a, CHAR_l, CHAR_e, CHAR_j, CHAR_a6, CHAR_c, CHAR_a};
WORD aMMDlgMethodTypeItem[11] = {10, CHAR_W, CHAR_e, CHAR_d, CHAR_l1, CHAR_FULL_STOP, CHAR_SPACE, CHAR_T, CHAR_y, CHAR_p, CHAR_u};
WORD aMMDlgMethodNameItem[11] = {8, CHAR_W, CHAR_g, CHAR_SPACE, CHAR_n, CHAR_a, CHAR_z, CHAR_w, CHAR_y};
WORD aMMDlgMethodTimeItem[10] = {8, CHAR_W, CHAR_g, CHAR_SPACE, CHAR_c, CHAR_z, CHAR_a, CHAR_s, CHAR_u};
WORD aMMHelpProgram[16] = {14, CHAR_Z, CHAR_a, CHAR_p, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_u, CHAR_j, CHAR_SPACE, CHAR_t, CHAR_o};
WORD aMMHelpProgramList[14] = {7, CHAR_U, CHAR_s, CHAR_u, CHAR_n2, CHAR_SPACE, CHAR_t, CHAR_o};
WORD aMMBtnFileType[10] = {9, CHAR_T, CHAR_y, CHAR_p, CHAR_SPACE, CHAR_p, CHAR_l, CHAR_i, CHAR_k, CHAR_u};
WORD aMMBtnSortFile[12] = {11, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_j, CHAR_SPACE, CHAR_p, CHAR_l, CHAR_i, CHAR_k};
WORD aMMBtnList[7] = {5, CHAR_L, CHAR_i, CHAR_s, CHAR_t, CHAR_a};
WORD aMMBtnPlay[9] = {4, CHAR_O, CHAR_d, CHAR_t, CHAR_w};
WORD aMMBtnDeleteAll[11] = {9, CHAR_U, CHAR_s, CHAR_u, CHAR_n2, CHAR_SPACE, CHAR_w, CHAR_s, CHAR_z, CHAR_FULL_STOP};
WORD aMMBtnReturn[8] = {6, CHAR_P, CHAR_o, CHAR_w, CHAR_r, CHAR_o1, CHAR_c2};
WORD aMMBtnExit[8] = {7, CHAR_W, CHAR_y, CHAR_j, CHAR_s1, CHAR_c, CHAR_i, CHAR_e};
#endif //OSDMM_MAIN_FILE									

#ifdef THUMB_MAIN_FILE									

WORD aTHUMBPage[8] = {7, CHAR_S, CHAR_t, CHAR_r, CHAR_o, CHAR_n, CHAR_a, CHAR_COLON};
WORD aTHUMBTitleGeneralMode[19] = {15, CHAR_T, CHAR_r, CHAR_y, CHAR_b, CHAR_SPACE, CHAR_M, CHAR_i, CHAR_n, CHAR_i, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_k, CHAR_i};
WORD aTHUMBTitleRotateMode[19] = {10, CHAR_T, CHAR_r, CHAR_y, CHAR_b, CHAR_SPACE, CHAR_O, CHAR_b, CHAR_r, CHAR_o1, CHAR_c2};
WORD aTHUMBTitleProgramMode[24] = {12, CHAR_T, CHAR_r, CHAR_y, CHAR_b, CHAR_SPACE, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m};
WORD aTHUMBTitleProgramList[21] = {15, CHAR_L, CHAR_i, CHAR_s, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_o1, CHAR_w};
WORD aTHUMBHelpRotateMode[17] = {8, CHAR_O, CHAR_b, CHAR_r, CHAR_o1, CHAR_c2, CHAR_SPACE, CHAR_t, CHAR_o};
WORD aTHUMBHelpProgramMode[16] = {14, CHAR_Z, CHAR_a, CHAR_p, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_u, CHAR_j, CHAR_SPACE, CHAR_t, CHAR_o};
WORD aTHUMBHelpProgramList[14] = {7, CHAR_U, CHAR_s, CHAR_u, CHAR_n2, CHAR_SPACE, CHAR_t, CHAR_o};
WORD aTHUMBButtonRotate[8] = {5, CHAR_O, CHAR_b, CHAR_r, CHAR_o1, CHAR_c2};
WORD aTHUMBButtonSlideShow[11] = {10, CHAR_P, CHAR_o, CHAR_k, CHAR_FULL_STOP, CHAR_SPACE, CHAR_S, CHAR_l, CHAR_a, CHAR_j, CHAR_d};
WORD aTHUMBButtonReset[8] = {7, CHAR_R, CHAR_e, CHAR_s, CHAR_e, CHAR_t, CHAR_u, CHAR_j};
WORD aTHUMBButtonReturn[8] = {6, CHAR_P, CHAR_o, CHAR_w, CHAR_r, CHAR_o1, CHAR_c2};
WORD aTHUMBButtonProgramList[7] = {5, CHAR_L, CHAR_i, CHAR_s, CHAR_t, CHAR_a};
#endif //THUMB_MAIN_FILE									

#ifdef OSDDG_MAIN_FILE									

WORD aDGDlgBtnTrackDigest[21] = {16, CHAR_F, CHAR_r, CHAR_a, CHAR_g, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_SPACE, CHAR_s1, CHAR_c, CHAR_i, CHAR_e, CHAR_z1, CHAR_k, CHAR_i};
WORD aDGDlgBtnDiscInterval[21] = {10, CHAR_C, CHAR_z, CHAR_a, CHAR_s, CHAR_SPACE, CHAR_d, CHAR_y, CHAR_s, CHAR_k, CHAR_u};
WORD aDGPage[8] = {7, CHAR_S, CHAR_t, CHAR_r, CHAR_o, CHAR_n, CHAR_a, CHAR_COLON};
WORD aDGTitleTrackDigest[21] = {16, CHAR_F, CHAR_r, CHAR_a, CHAR_g, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_SPACE, CHAR_s1, CHAR_c, CHAR_i, CHAR_e, CHAR_z1, CHAR_k, CHAR_i};
WORD aDGTitleDiscInterval[21] = {10, CHAR_C, CHAR_z, CHAR_a, CHAR_s, CHAR_SPACE, CHAR_d, CHAR_y, CHAR_s, CHAR_k, CHAR_u};
WORD aDGInfoTrack[9] = {8, CHAR_S1, CHAR_c, CHAR_i, CHAR_e, CHAR_z1, CHAR_k, CHAR_a, CHAR_COLON};
WORD aDGBtnMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aDGBtnExit[8] = {7, CHAR_W, CHAR_y, CHAR_j, CHAR_s1, CHAR_c, CHAR_i, CHAR_e};
#endif //OSDDG_MAIN_FILE									

#ifdef OSDPRGM_MAIN_FILE									

WORD aProgram[12] = {7, CHAR_P, CHAR_R, CHAR_O, CHAR_G, CHAR_R, CHAR_A, CHAR_M};
WORD aTitle[7] = {5, CHAR_T, CHAR_y, CHAR_t, CHAR_u, CHAR_l1};
WORD aChapter[10] = {8, CHAR_R, CHAR_o, CHAR_z, CHAR_d, CHAR_z, CHAR_i, CHAR_a, CHAR_l1};
WORD aTrack[8] = {7, CHAR_S1, CHAR_c, CHAR_i, CHAR_e, CHAR_z1, CHAR_k, CHAR_a};
WORD aSelect[12] = {7, CHAR_W, CHAR_y, CHAR_b, CHAR_i, CHAR_e, CHAR_r, CHAR_z};
WORD aPlay[11] = {9, CHAR_O, CHAR_d, CHAR_t, CHAR_w, CHAR_a, CHAR_r, CHAR_z, CHAR_a, CHAR_j};
WORD aClear[8] = {4, CHAR_U, CHAR_s, CHAR_u, CHAR_n2};
#endif //OSDPRGM_MAIN_FILE									

#ifdef OSDBMARK_MAIN_FILE									

WORD aBookmark[11] = {8, CHAR_Z, CHAR_a, CHAR_k, CHAR_l1, CHAR_a, CHAR_d, CHAR_k, CHAR_a};
WORD aTime[6] = {4, CHAR_C, CHAR_z, CHAR_a, CHAR_s};
#endif //OSDBMARK_MAIN_FILE									

#ifdef SETUP_MAIN_FILE									

WORD aSetupSelect[12] = {7, CHAR_W, CHAR_y, CHAR_b, CHAR_i, CHAR_e, CHAR_r, CHAR_z};
#endif //SETUP_MAIN_FILE									

#ifdef MENU_MAIN_FILE									

WORD aMsgEnglish[10] = {9, CHAR_A, CHAR_n, CHAR_g, CHAR_i, CHAR_e, CHAR_l, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgJapanese[11] = {8, CHAR_J, CHAR_a, CHAR_p, CHAR_o, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgFrench[12] = {9, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_u, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgGerman[10] = {9, CHAR_N, CHAR_i, CHAR_e, CHAR_m, CHAR_i, CHAR_e, CHAR_c, CHAR_k, CHAR_i};
WORD aMsgItalian[12] = {6, CHAR_W, CHAR_l1, CHAR_o, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgSpanish[11] = {10, CHAR_H, CHAR_i, CHAR_s, CHAR_z, CHAR_p, CHAR_a, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgPortuguese[14] = {11, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_a, CHAR_l, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgDutch[12] = {11, CHAR_H, CHAR_o, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgPolish[9] = {6, CHAR_P, CHAR_o, CHAR_l, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aMsgThai[10] = {6, CHAR_T, CHAR_a, CHAR_j, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgRussian[9] = {8, CHAR_R, CHAR_o, CHAR_s, CHAR_y, CHAR_j, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgKorean[11] = {9, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aMsgCzech[6] = {5, CHAR_C, CHAR_z, CHAR_e, CHAR_c, CHAR_h};
WORD aMsgGreece[7] = {6, CHAR_G, CHAR_r, CHAR_e, CHAR_e, CHAR_c, CHAR_e};
WORD aMsgTurkey[7] = {6, CHAR_T, CHAR_u, CHAR_r, CHAR_k, CHAR_e, CHAR_y};
WORD aMsgOthers[9] = {4, CHAR_I, CHAR_n, CHAR_n, CHAR_e};
WORD aMsgOSD[4] = {3, CHAR_O, CHAR_S, CHAR_D};
WORD aMsgMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aMsgAudio[6] = {5, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMsgSubtitle[12] = {6, CHAR_N, CHAR_a, CHAR_p, CHAR_i, CHAR_s, CHAR_y};
WORD aMsgPS[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_P, CHAR_S};
WORD aMsgLB[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_L, CHAR_B};
WORD aMsgWide[5] = {4, CHAR_1, CHAR_6, CHAR_COLON, CHAR_9};
WORD aMsgOn[9] = {8, CHAR_W, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_e};
WORD aMsgOff[10] = {9, CHAR_W, CHAR_y, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_o, CHAR_n, CHAR_e};
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
WORD aMsgBriNormal[9] = {8, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l, CHAR_n, CHAR_e};
WORD aMsgBright[10] = {5, CHAR_J, CHAR_a, CHAR_s, CHAR_n, CHAR_e};
WORD aMsgSoft[8] = {7, CHAR_M, CHAR_i, CHAR_e5, CHAR_k, CHAR_k, CHAR_i, CHAR_e};
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
WORD aMsgTVType[14] = {6, CHAR_T, CHAR_y, CHAR_p, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMsgScreenSaver[18] = {16, CHAR_W, CHAR_y, CHAR_g, CHAR_a, CHAR_s, CHAR_z, CHAR_a, CHAR_c, CHAR_z, CHAR_SPACE, CHAR_e, CHAR_k, CHAR_r, CHAR_a, CHAR_n, CHAR_u};
WORD aMsgTVSystem[17] = {9, CHAR_S, CHAR_y, CHAR_s, CHAR_t, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMsgVideoOutput[14] = {13, CHAR_W, CHAR_y, CHAR_j, CHAR_s1, CHAR_c, CHAR_i, CHAR_e, CHAR_SPACE, CHAR_v, CHAR_i, CHAR_d, CHAR_e, CHAR_o};
WORD aMsgBrightness[11] = {7, CHAR_J, CHAR_a, CHAR_s, CHAR_n, CHAR_o, CHAR_s1, CHAR_c2};
WORD aMsgContrast[10] = {8, CHAR_K, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMsgChromPhase[4] = {3, CHAR_H, CHAR_u, CHAR_e};
WORD aMsgSPDIF_PCM[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_P, CHAR_C, CHAR_M};
WORD aMsgSPDIF_RAW[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_R, CHAR_A, CHAR_W};
WORD aMsg2CH[11] = {10, CHAR_2, CHAR_SPACE, CHAR_k, CHAR_a, CHAR_n, CHAR_a, CHAR_l1, CHAR_o, CHAR_w, CHAR_y};
WORD aMsg51CH[13] = {12, CHAR_5, CHAR_FULL_STOP, CHAR_1, CHAR_SPACE, CHAR_k, CHAR_a, CHAR_n, CHAR_a, CHAR_l1, CHAR_o, CHAR_w, CHAR_y};
WORD aMsgLoRo[6] = {5, CHAR_L, CHAR_o, CHAR_SOLIDUS, CHAR_R, CHAR_o};
WORD aMsgLtRt[6] = {5, CHAR_L, CHAR_t, CHAR_SOLIDUS, CHAR_R, CHAR_t};
#ifndef REMOVE_SETUP_SPEAKER_TEST									

WORD aMsgAutoTest[5] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aMsgLeft[6] = {4, CHAR_L, CHAR_e, CHAR_w, CHAR_y};
WORD aMsgRight[7] = {5, CHAR_P, CHAR_r, CHAR_a, CHAR_w, CHAR_y};
WORD aMsgCntr[10] = {9, CHAR_C, CHAR_e, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_l, CHAR_n, CHAR_y};
WORD aMsgLS[15] = {9, CHAR_L, CHAR_e, CHAR_w, CHAR_y, CHAR_SPACE, CHAR_S, CHAR_u, CHAR_r, CHAR_r};
WORD aMsgRS[16] = {10, CHAR_P, CHAR_r, CHAR_a, CHAR_w, CHAR_y, CHAR_SPACE, CHAR_S, CHAR_u, CHAR_r, CHAR_r};
WORD aMsgSW[10] = {9, CHAR_S, CHAR_u, CHAR_b, CHAR_w, CHAR_o, CHAR_o, CHAR_f, CHAR_e, CHAR_r};
#endif //REMOVE_SETUP_SPEAKER_TEST									

WORD aMsgDigitalOut[18] = {15, CHAR_W, CHAR_y, CHAR_j, CHAR_s1, CHAR_c, CHAR_i, CHAR_e, CHAR_SPACE, CHAR_c, CHAR_y, CHAR_f, CHAR_r, CHAR_o, CHAR_w, CHAR_e};
WORD aMsgAnalogOut[18] = {12, CHAR_W, CHAR_y, CHAR_j, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_n, CHAR_a, CHAR_l, CHAR_o, CHAR_g, CHAR_FULL_STOP};
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
WORD aMsgVirtual[10] = {9, CHAR_W, CHAR_i, CHAR_r, CHAR_t, CHAR_u, CHAR_a, CHAR_l, CHAR_n, CHAR_y};
WORD aMsgMusic[7] = {6, CHAR_M, CHAR_u, CHAR_z, CHAR_y, CHAR_k, CHAR_a};
WORD aMsgMovie[6] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aMsgMatrix[8] = {7, CHAR_M, CHAR_a, CHAR_t, CHAR_r, CHAR_y, CHAR_c, CHAR_a};
WORD aMsgPrologic[10] = {9, CHAR_P, CHAR_R, CHAR_O, CHAR_SPACE, CHAR_L, CHAR_O, CHAR_G, CHAR_I, CHAR_C};
WORD aMsgDownMix[8] = {7, CHAR_D, CHAR_o, CHAR_w, CHAR_n, CHAR_m, CHAR_i, CHAR_x};
WORD aMsgCenter[10] = {9, CHAR_C, CHAR_e, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_l, CHAR_n, CHAR_y};
WORD aMsgRear[11] = {5, CHAR_T, CHAR_y, CHAR_l, CHAR_n, CHAR_y};
WORD aMsgSubwoofer[10] = {9, CHAR_S, CHAR_u, CHAR_b, CHAR_w, CHAR_o, CHAR_o, CHAR_f, CHAR_e, CHAR_r};
WORD aMsgSpeakerTest[19] = {13, CHAR_T, CHAR_e, CHAR_s, CHAR_t, CHAR_SPACE, CHAR_g, CHAR_l1, CHAR_o, CHAR_s1, CHAR_n, CHAR_i, CHAR_k, CHAR_a};
WORD aMsgLevel1[2] = {1, CHAR_1};
WORD aMsgLevel2[2] = {1, CHAR_2};
WORD aMsgLevel3[2] = {1, CHAR_3};
WORD aMsgLevel4[2] = {1, CHAR_4};
WORD aMsgLevel5[2] = {1, CHAR_5};
WORD aMsgLevel6[2] = {1, CHAR_6};
WORD aMsgLevel7[2] = {1, CHAR_7};
WORD aMsgLevel8[2] = {1, CHAR_8};
WORD aMsgRead[8] = {6, CHAR_C, CHAR_z, CHAR_y, CHAR_t, CHAR_a, CHAR_j};
WORD aMsgParental[20] = {19, CHAR_K, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_FULL_STOP, CHAR_SPACE, CHAR_r, CHAR_o, CHAR_d, CHAR_z, CHAR_i, CHAR_c, CHAR_i, CHAR_e, CHAR_l, CHAR_s, CHAR_k, CHAR_a};
WORD aMsgChangePassword[13] = {5, CHAR_H, CHAR_a, CHAR_s, CHAR_l1, CHAR_o};
WORD aMsgAngleMark[16] = {9, CHAR_Z, CHAR_n, CHAR_a, CHAR_k, CHAR_SPACE, CHAR_k, CHAR_a6, CHAR_t, CHAR_a};
WORD aMsgUIStyle[7] = {4, CHAR_S, CHAR_t, CHAR_y, CHAR_l};
WORD aMsgUIStyle1[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_SPACE, CHAR_1};
WORD aMsgUIStyle2[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_SPACE, CHAR_2};
WORD aMsgUIStyle3[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_SPACE, CHAR_3};
WORD aMsgUIStyle4[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_SPACE, CHAR_4};
WORD aMsgSmartPlay[14] = {11, CHAR_S, CHAR_z, CHAR_y, CHAR_b, CHAR_FULL_STOP, CHAR_SPACE, CHAR_o, CHAR_d, CHAR_t, CHAR_w, CHAR_FULL_STOP};
WORD aMsgDivXVOD[9] = {8, CHAR_D, CHAR_i, CHAR_v, CHAR_X, CHAR_SPACE, CHAR_V, CHAR_O, CHAR_D};
WORD aMsgDefaultValue[18] = {8, CHAR_D, CHAR_o, CHAR_m, CHAR_y, CHAR_s1, CHAR_l, CHAR_n, CHAR_e};
WORD aMsgLanguageSetup[23] = {17, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_a, CHAR_SPACE, CHAR_j, CHAR_e5, CHAR_z, CHAR_y, CHAR_k, CHAR_a};
WORD aMsgScreenSetup[22] = {17, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_a, CHAR_SPACE, CHAR_e, CHAR_k, CHAR_r, CHAR_a, CHAR_n, CHAR_u};
WORD aMsgAudioSetup[22] = {16, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMsgCustomSetup[28] = {22, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_a, CHAR_SPACE, CHAR_u, CHAR_z1, CHAR_y, CHAR_t, CHAR_k, CHAR_o, CHAR_w, CHAR_n, CHAR_i, CHAR_k, CHAR_a};
WORD aMsgExitSetup[30] = {24, CHAR_W, CHAR_y, CHAR_j, CHAR_d, CHAR_z2, CHAR_SPACE, CHAR_z, CHAR_SPACE, CHAR_t, CHAR_r, CHAR_y, CHAR_b, CHAR_u, CHAR_SPACE, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_a};
WORD aMsgPassword[31] = {14, CHAR_W, CHAR_p, CHAR_r, CHAR_o, CHAR_w, CHAR_a, CHAR_d, CHAR_z2, CHAR_SPACE, CHAR_h, CHAR_a, CHAR_s, CHAR_l1, CHAR_o};
WORD aMsgWrongPassword[20] = {19, CHAR_N, CHAR_i, CHAR_e, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_i, CHAR_d, CHAR_l1, CHAR_o, CHAR_w, CHAR_e, CHAR_SPACE, CHAR_h, CHAR_a, CHAR_s, CHAR_l1, CHAR_o};
WORD aMsgOldPassword[9] = {6, CHAR_S, CHAR_t, CHAR_a, CHAR_r, CHAR_e, CHAR_COLON};
WORD aMsgNewPassword[10] = {5, CHAR_N, CHAR_o, CHAR_w, CHAR_e, CHAR_COLON};
WORD aMsgVerifyPassword[12] = {10, CHAR_W, CHAR_e, CHAR_r, CHAR_y, CHAR_f, CHAR_i, CHAR_k, CHAR_u, CHAR_j, CHAR_COLON};
WORD aMsgStandard[12] = {11, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_r, CHAR_d, CHAR_o, CHAR_w, CHAR_e};
WORD aMsgClassical[10] = {9, CHAR_K, CHAR_l, CHAR_a, CHAR_s, CHAR_y, CHAR_c, CHAR_z, CHAR_n, CHAR_a};
WORD aMsgRock[5] = {4, CHAR_R, CHAR_o, CHAR_c, CHAR_k};
WORD aMsgJazz[5] = {4, CHAR_J, CHAR_a, CHAR_z, CHAR_z};
WORD aMsgAcoustic[11] = {10, CHAR_A, CHAR_k, CHAR_u, CHAR_s, CHAR_t, CHAR_y, CHAR_c, CHAR_z, CHAR_n, CHAR_a};
WORD aMsgPop[4] = {3, CHAR_P, CHAR_o, CHAR_p};
WORD aMsgCustom[7] = {6, CHAR_C, CHAR_u, CHAR_s, CHAR_t, CHAR_o, CHAR_m};
#ifdef SUPPORT_PSCAN_PROMPT_SETUP									

WORD aMsgPleaseMake[39] = {31, CHAR_T, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_t, CHAR_e, CHAR_l, CHAR_e, CHAR_w, CHAR_i, CHAR_z, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_o, CHAR_b, CHAR_s, CHAR_l1, CHAR_u, CHAR_g, CHAR_u, CHAR_j, CHAR_e, CHAR_SPACE, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_c, CHAR_a, CHAR_n, CHAR_QUESTION_MARK};
#endif SUPPORT_PSCAN_PROMPT_SETUP									

#endif //MENU_MAIN_FILE									

#ifdef IMAGE_FRAME_MENU_MAIN_FILE									

WORD aMENUPhotoSetting[26] = {12, CHAR_U, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_z, CHAR_d, CHAR_j, CHAR_e5, CHAR_c, CHAR_i, CHAR_a};
WORD aMENUIntervalTime[20] = {12, CHAR_O, CHAR_d, CHAR_s, CHAR_t, CHAR_e5, CHAR_p, CHAR_SPACE, CHAR_c, CHAR_z, CHAR_a, CHAR_s, CHAR_u};
#ifndef MUSTEK_SETUP_MENU									

WORD aMENU5Sec[7] = {6, CHAR_5, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_k, CHAR_FULL_STOP};
WORD aMENU10Sec[8] = {7, CHAR_1, CHAR_0, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_k, CHAR_FULL_STOP};
#endif //MUSTEK_SETUP_MENU									

WORD aMENU15Sec[8] = {7, CHAR_1, CHAR_5, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_k, CHAR_FULL_STOP};
WORD aMENU30Sec[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_k, CHAR_FULL_STOP};
WORD aMENU1Min[7] = {6, CHAR_1, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU3Min[7] = {6, CHAR_3, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENUTransitionEffect[21] = {15, CHAR_E, CHAR_f, CHAR_e, CHAR_k, CHAR_t, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_z, CHAR_e, CHAR_j, CHAR_s1, CHAR_c, CHAR_i, CHAR_a};
WORD aMENUJPEGEffectNone[12] = {11, CHAR_B, CHAR_r, CHAR_a, CHAR_k, CHAR_SPACE, CHAR_e, CHAR_f, CHAR_e, CHAR_k, CHAR_t, CHAR_u};
WORD aMENUJPEGEffectCoverUp[17] = {15, CHAR_P, CHAR_r, CHAR_z, CHAR_y, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_g, CHAR_o1, CHAR_r, CHAR_e5};
WORD aMENUJPEGEffectCoverDown[18] = {14, CHAR_P, CHAR_r, CHAR_z, CHAR_y, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_o1, CHAR_l1};
#ifndef MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectCoverLeft[16] = {15, CHAR_P, CHAR_r, CHAR_z, CHAR_y, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_w, CHAR_o};
WORD aMENUJPEGEffectCoverRight[17] = {16, CHAR_P, CHAR_r, CHAR_z, CHAR_y, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_o};
WORD aMENUJPEGEffectUncoverUp[18] = {13, CHAR_O, CHAR_d, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_g, CHAR_o1, CHAR_r, CHAR_e5};
WORD aMENUJPEGEffectUncoverDown[17] = {12, CHAR_O, CHAR_d, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_o1, CHAR_l1};
#endif //MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectUncoverLeft[16] = {13, CHAR_O, CHAR_d, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_w, CHAR_o};
WORD aMENUJPEGEffectUncoverRight[17] = {14, CHAR_O, CHAR_d, CHAR_k, CHAR_r, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_o};
WORD aMENUJPEGEffectWipeUp[20] = {14, CHAR_Z, CHAR_e, CHAR_t, CHAR_r, CHAR_z, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_g, CHAR_o1, CHAR_r, CHAR_e5};
WORD aMENUJPEGEffectWipeDown[17] = {13, CHAR_Z, CHAR_e, CHAR_t, CHAR_r, CHAR_z, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_o1, CHAR_l1};
#ifndef MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectWipeLeft[17] = {14, CHAR_Z, CHAR_e, CHAR_t, CHAR_r, CHAR_z, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_w, CHAR_o};
WORD aMENUJPEGEffectWipeRight[18] = {15, CHAR_Z, CHAR_e, CHAR_t, CHAR_r, CHAR_z, CHAR_y, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_o};
WORD aMENUJPEGEffectPushUp[14] = {13, CHAR_P, CHAR_c, CHAR_h, CHAR_n, CHAR_i, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_g, CHAR_o1, CHAR_r, CHAR_e5};
WORD aMENUJPEGEffectPushDown[13] = {12, CHAR_P, CHAR_c, CHAR_h, CHAR_n, CHAR_i, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_o1, CHAR_l1};
WORD aMENUJPEGEffectPushLeft[17] = {13, CHAR_P, CHAR_c, CHAR_h, CHAR_n, CHAR_i, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_l, CHAR_e, CHAR_w, CHAR_o};
#endif //MUSTEK_SETUP_MENU									

WORD aMENUJPEGEffectPushRight[18] = {14, CHAR_P, CHAR_c, CHAR_h, CHAR_n, CHAR_i, CHAR_j, CHAR_SPACE, CHAR_w, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_a, CHAR_w, CHAR_o};
WORD aMENUJPEGEffectFadeThroughBlack[26] = {21, CHAR_W, CHAR_y, CHAR_c, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_u, CHAR_j, CHAR_SPACE, CHAR_o, CHAR_d, CHAR_SPACE, CHAR_c, CHAR_z, CHAR_a, CHAR_r, CHAR_n, CHAR_e, CHAR_g, CHAR_o};
WORD aMENUJPEGEffectRandom[16] = {10, CHAR_O, CHAR_d, CHAR_t, CHAR_w, CHAR_FULL_STOP, CHAR_SPACE, CHAR_l, CHAR_o, CHAR_s, CHAR_FULL_STOP};
WORD aMENUBrowse[12] = {7, CHAR_P, CHAR_r, CHAR_z, CHAR_e, CHAR_g, CHAR_l, CHAR_FULL_STOP};
WORD aMENUFileManager[16] = {15, CHAR_M, CHAR_e, CHAR_n, CHAR_e, CHAR_d, CHAR_z1, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_p, CHAR_l, CHAR_i, CHAR_k, CHAR_o1, CHAR_w};
WORD aMENUSetupMenu[16] = {15, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_a};
WORD aMENUThumbnail[13] = {10, CHAR_M, CHAR_i, CHAR_n, CHAR_i, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_k, CHAR_i};
WORD aMENUAutoPlay[14] = {12, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_m, CHAR_FULL_STOP, CHAR_SPACE, CHAR_o, CHAR_d, CHAR_t, CHAR_w, CHAR_FULL_STOP};
WORD aMENUPhoto[12] = {7, CHAR_Z, CHAR_d, CHAR_j, CHAR_e5, CHAR_c, CHAR_i, CHAR_e};
WORD aMENUMusic[7] = {6, CHAR_M, CHAR_u, CHAR_z, CHAR_y, CHAR_k, CHAR_a};
WORD aMENUPhotoAudio[14] = {13, CHAR_Z, CHAR_d, CHAR_j, CHAR_e5, CHAR_c, CHAR_i, CHAR_e, CHAR_SOLIDUS, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMENUMovie[10] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aMENUDisplaySetup[16] = {13, CHAR_U, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_w, CHAR_y, CHAR_s1, CHAR_w, CHAR_i, CHAR_e, CHAR_t, CHAR_l};
WORD aMENUDisplayType[14] = {12, CHAR_T, CHAR_y, CHAR_p, CHAR_SPACE, CHAR_w, CHAR_y, CHAR_s1, CHAR_w, CHAR_i, CHAR_e, CHAR_t, CHAR_l};
WORD aMENUTVStandard[12] = {11, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMENUNTSC[5] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aMENUPAL[4] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aMENUAUTO[11] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aMENUBrightness[11] = {7, CHAR_J, CHAR_a, CHAR_s, CHAR_n, CHAR_o, CHAR_s1, CHAR_c2};
WORD aMENUContrast[10] = {8, CHAR_K, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMENUNormal[9] = {8, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l, CHAR_n, CHAR_e};
WORD aMENUBright[10] = {5, CHAR_J, CHAR_a, CHAR_s, CHAR_n, CHAR_e};
WORD aMENUSoft[8] = {7, CHAR_M, CHAR_i, CHAR_e5, CHAR_k, CHAR_k, CHAR_i, CHAR_e};
WORD aMENUTFTBrightness[15] = {11, CHAR_J, CHAR_a, CHAR_s, CHAR_n, CHAR_o, CHAR_s1, CHAR_c2, CHAR_SPACE, CHAR_T, CHAR_F, CHAR_T};
WORD aMENUTFTContrast[14] = {12, CHAR_K, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t, CHAR_SPACE, CHAR_T, CHAR_F, CHAR_T};
WORD aMENUTFTSaturation[16] = {13, CHAR_N, CHAR_a, CHAR_s, CHAR_y, CHAR_c, CHAR_e, CHAR_n, CHAR_i, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_F, CHAR_T};
WORD aMENURepeatMode[15] = {14, CHAR_T, CHAR_r, CHAR_y, CHAR_b, CHAR_SPACE, CHAR_P, CHAR_o, CHAR_w, CHAR_t, CHAR_a, CHAR_r, CHAR_z, CHAR_a, CHAR_j};
WORD aMENURepeatOne[9] = {5, CHAR_J, CHAR_e, CHAR_d, CHAR_e, CHAR_n};
WORD aMENURepeatFolder[9] = {6, CHAR_F, CHAR_o, CHAR_l, CHAR_d, CHAR_e, CHAR_r};
WORD aMENURepeatAll[10] = {9, CHAR_W, CHAR_s, CHAR_z, CHAR_y, CHAR_s, CHAR_t, CHAR_k, CHAR_i, CHAR_e};
WORD aMENUCustomSetup[28] = {22, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_a, CHAR_SPACE, CHAR_u, CHAR_z1, CHAR_y, CHAR_t, CHAR_k, CHAR_o, CHAR_w, CHAR_n, CHAR_i, CHAR_k, CHAR_a};
WORD aMENUOSDLanguage[14] = {9, CHAR_J, CHAR_e5, CHAR_z, CHAR_y, CHAR_k, CHAR_SPACE, CHAR_O, CHAR_S, CHAR_D};
WORD aMENUEnglish[10] = {9, CHAR_A, CHAR_n, CHAR_g, CHAR_i, CHAR_e, CHAR_l, CHAR_s, CHAR_k, CHAR_i};
WORD aMENUChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aMENUJapanese[11] = {8, CHAR_J, CHAR_a, CHAR_p, CHAR_o, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aMENUKorean[11] = {9, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aMENUFrench[12] = {9, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_u, CHAR_s, CHAR_k, CHAR_i};
WORD aMENUGerman[10] = {9, CHAR_N, CHAR_i, CHAR_e, CHAR_m, CHAR_i, CHAR_e, CHAR_c, CHAR_k, CHAR_i};
WORD aMENUItalian[12] = {6, CHAR_W, CHAR_l1, CHAR_o, CHAR_s, CHAR_k, CHAR_i};
WORD aMENUSpanish[11] = {10, CHAR_H, CHAR_i, CHAR_s, CHAR_z, CHAR_p, CHAR_a, CHAR_n2, CHAR_s, CHAR_k, CHAR_i};
WORD aMENUPortuguese[14] = {11, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_a, CHAR_l, CHAR_s, CHAR_k, CHAR_i};
WORD aMENUDutch[12] = {11, CHAR_H, CHAR_o, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_s, CHAR_k, CHAR_i};
WORD aMENUPolish[9] = {6, CHAR_P, CHAR_o, CHAR_l, CHAR_s, CHAR_k, CHAR_i};
WORD aMENUUIStyle[7] = {4, CHAR_S, CHAR_t, CHAR_y, CHAR_l};
WORD aMENUUIStyle1[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_SPACE, CHAR_1};
WORD aMENUUIStyle2[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_SPACE, CHAR_2};
WORD aMENUUIStyle3[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_SPACE, CHAR_3};
WORD aMENUUIStyle4[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_SPACE, CHAR_4};
WORD aMENUDefault[18] = {8, CHAR_D, CHAR_o, CHAR_m, CHAR_y, CHAR_s1, CHAR_l, CHAR_n, CHAR_e};
WORD aMENURead[8] = {6, CHAR_C, CHAR_z, CHAR_y, CHAR_t, CHAR_a, CHAR_j};
WORD aMENUIconExit[6] = {4, CHAR_W, CHAR_y, CHAR_j, CHAR_FULL_STOP};
WORD aMENUHelpSelect[10] = {7, CHAR_SPACE, CHAR_COLON, CHAR_SPACE, CHAR_W, CHAR_y, CHAR_b, CHAR_FULL_STOP};
WORD aMENUHelpConfirm[11] = {8, CHAR_SPACE, CHAR_COLON, CHAR_SPACE, CHAR_P, CHAR_o, CHAR_t, CHAR_w, CHAR_FULL_STOP};
WORD aMENUHelpExit[18] = {17, CHAR_W, CHAR_y, CHAR_j, CHAR_COLON, CHAR_SPACE, CHAR_Z, CHAR_a, CHAR_p, CHAR_i, CHAR_s, CHAR_SPACE, CHAR_i, CHAR_SPACE, CHAR_w, CHAR_y, CHAR_j, CHAR_FULL_STOP};
#endif //#ifdef IMAGE_FRAME_MENU_MAIN_FILE									

#ifdef SUPPORT_POWERON_MENU									

WORD aPOWERONIconPhoto[8] = {7, CHAR_Z, CHAR_d, CHAR_j, CHAR_e5, CHAR_c, CHAR_i, CHAR_e};
WORD aPOWERONIconMusic[7] = {6, CHAR_M, CHAR_u, CHAR_z, CHAR_y, CHAR_k, CHAR_a};
WORD aPOWERONIconPhotoMusic[15] = {14, CHAR_Z, CHAR_d, CHAR_j, CHAR_e5, CHAR_c, CHAR_i, CHAR_e, CHAR_PLUS_SIGN, CHAR_M, CHAR_u, CHAR_z, CHAR_y, CHAR_k, CHAR_a};
WORD aPOWERONIconMovie[10] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aPOWERONIconSetup[14] = {6, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_FULL_STOP};
WORD aPOWERONIconCalendar[11] = {9, CHAR_K, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_a, CHAR_r, CHAR_z};
WORD aPOWERONIconClock[9] = {5, CHAR_Z, CHAR_e, CHAR_g, CHAR_a, CHAR_r};
WORD aPOWERONIconAlarm[7] = {5, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m};
WORD aPOWERONIconAutoPower[14] = {13, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_m, CHAR_FULL_STOP, CHAR_SPACE, CHAR_z, CHAR_a, CHAR_s, CHAR_i, CHAR_l, CHAR_FULL_STOP};
WORD aPOWERONIconEdit[11] = {6, CHAR_E, CHAR_d, CHAR_y, CHAR_t, CHAR_u, CHAR_j};
WORD aPOWERONIconFavorite[10] = {8, CHAR_U, CHAR_l, CHAR_u, CHAR_b, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aPOWERONIconRadio[6] = {5, CHAR_R, CHAR_a, CHAR_d, CHAR_i, CHAR_o};
WORD aWeeklyDay_Mon[4] = {3, CHAR_M, CHAR_o, CHAR_n};
WORD aWeeklyDay_Tue[4] = {3, CHAR_T, CHAR_u, CHAR_e};
WORD aWeeklyDay_Wed[4] = {3, CHAR_W, CHAR_e, CHAR_d};
WORD aWeeklyDay_Thu[4] = {3, CHAR_T, CHAR_h, CHAR_u};
WORD aWeeklyDay_Fri[4] = {3, CHAR_F, CHAR_r, CHAR_i};
WORD aWeeklyDay_Sat[4] = {3, CHAR_S, CHAR_a, CHAR_t};
WORD aWeeklyDay_Sun[4] = {3, CHAR_S, CHAR_u, CHAR_n};
WORD aMonthJanuary[4] = {3, CHAR_S, CHAR_t, CHAR_y};
WORD aMonthFebruary[4] = {3, CHAR_L, CHAR_u, CHAR_t};
WORD aMonthMarch[4] = {3, CHAR_M, CHAR_a, CHAR_r};
WORD aMonthApril[4] = {3, CHAR_K, CHAR_w, CHAR_i};
WORD aMonthMay[4] = {3, CHAR_M, CHAR_a, CHAR_j};
WORD aMonthJune[4] = {3, CHAR_C, CHAR_z, CHAR_e};
WORD aMonthJuly[4] = {3, CHAR_L, CHAR_i, CHAR_p};
WORD aMonthAugust[4] = {3, CHAR_S, CHAR_i, CHAR_e5};
WORD aMonthSeptember[4] = {3, CHAR_W, CHAR_r, CHAR_z};
WORD aMonthOctober[4] = {3, CHAR_P, CHAR_a, CHAR_z2};
WORD aMonthNovember[4] = {3, CHAR_L, CHAR_i, CHAR_s};
WORD aMonthDecember[4] = {3, CHAR_G, CHAR_r, CHAR_u};
WORD aWeeklyMonday[14] = {12, CHAR_P, CHAR_o, CHAR_n, CHAR_i, CHAR_e, CHAR_d, CHAR_z, CHAR_i, CHAR_a, CHAR_l1, CHAR_e, CHAR_k};
WORD aWeeklyTuesday[12] = {6, CHAR_W, CHAR_t, CHAR_o, CHAR_r, CHAR_e, CHAR_k};
WORD aWeeklyWednesday[13] = {5, CHAR_S1, CHAR_r, CHAR_o, CHAR_d, CHAR_a};
WORD aWeeklyThursday[13] = {8, CHAR_C, CHAR_z, CHAR_w, CHAR_a, CHAR_r, CHAR_t, CHAR_e, CHAR_k};
WORD aWeeklyFriday[12] = {6, CHAR_P, CHAR_i, CHAR_a6, CHAR_t, CHAR_e, CHAR_k};
WORD aWeeklySaturday[9] = {6, CHAR_S, CHAR_o, CHAR_b, CHAR_o, CHAR_t, CHAR_a};
WORD aWeeklySunday[10] = {9, CHAR_N, CHAR_i, CHAR_e, CHAR_d, CHAR_z, CHAR_i, CHAR_e, CHAR_l, CHAR_a};
WORD aCLOCKClockSetting[22] = {13, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_FULL_STOP, CHAR_SPACE, CHAR_z, CHAR_e, CHAR_g, CHAR_a, CHAR_r, CHAR_a};
WORD aCLOCKDisplay[13] = {5, CHAR_W, CHAR_y, CHAR_s1, CHAR_w, CHAR_FULL_STOP};
WORD aCLOCKSetTime[14] = {8, CHAR_U, CHAR_s, CHAR_t, CHAR_SPACE, CHAR_c, CHAR_z, CHAR_a, CHAR_s};
WORD aCLOCKExit[6] = {4, CHAR_W, CHAR_y, CHAR_j, CHAR_FULL_STOP};
WORD aCLOCKBigClock[9] = {5, CHAR_Z, CHAR_e, CHAR_g, CHAR_a, CHAR_r};
WORD aCLOCKClockCalendar[22] = {15, CHAR_Z, CHAR_e, CHAR_g, CHAR_a, CHAR_r, CHAR_SPACE, CHAR_i, CHAR_SPACE, CHAR_k, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_FULL_STOP};
WORD aBUTTONOK[3] = {2, CHAR_O, CHAR_K};
WORD aBUTTONCancel[9] = {6, CHAR_A, CHAR_n, CHAR_u, CHAR_l, CHAR_u, CHAR_j};
WORD aSETTIMESetTime[15] = {8, CHAR_U, CHAR_s, CHAR_t, CHAR_SPACE, CHAR_c, CHAR_z, CHAR_a, CHAR_s};
WORD aALARMEnable[10] = {6, CHAR_W, CHAR_l1, CHAR_a6, CHAR_c, CHAR_z, CHAR_FULL_STOP};
WORD aALARMAlarmSetting[21] = {13, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_a, CHAR_r, CHAR_m, CHAR_u};
WORD aALARMDlgTitle[8] = {5, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m};
WORD aALARMDlgContent[34] = {33, CHAR_D, CHAR_o, CHAR_w, CHAR_o, CHAR_l, CHAR_n, CHAR_y, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_z, CHAR_y, CHAR_c, CHAR_i, CHAR_s, CHAR_k, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_SPACE, CHAR_z, CHAR_a, CHAR_t, CHAR_r, CHAR_z, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_a, CHAR_r, CHAR_m, CHAR_u, CHAR_FULL_STOP};
WORD aAUTOPWRAutoPowerSetting[46] = {25, CHAR_U, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_t, CHAR_o, CHAR_m, CHAR_FULL_STOP, CHAR_SPACE, CHAR_w, CHAR_l1, CHAR_SOLIDUS, CHAR_w, CHAR_y, CHAR_l1, CHAR_SPACE, CHAR_z, CHAR_a, CHAR_s, CHAR_i, CHAR_l, CHAR_FULL_STOP};
WORD aAUTOPWRPowerOn[11] = {8, CHAR_W, CHAR_l1, CHAR_SPACE, CHAR_z, CHAR_a, CHAR_s, CHAR_i, CHAR_l};
WORD aAUTOPWRPowerOff[12] = {9, CHAR_W, CHAR_y, CHAR_l1, CHAR_SPACE, CHAR_z, CHAR_a, CHAR_s, CHAR_i, CHAR_l};
WORD aAUTOPWRPowerOnEnable[30] = {15, CHAR_D, CHAR_o, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_w, CHAR_l1, CHAR_SPACE, CHAR_z, CHAR_a, CHAR_s, CHAR_i, CHAR_l, CHAR_FULL_STOP};
WORD aAUTOPWRPowerOffEnable[33] = {16, CHAR_D, CHAR_o, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_w, CHAR_y, CHAR_l1, CHAR_SPACE, CHAR_z, CHAR_a, CHAR_s, CHAR_i, CHAR_l, CHAR_FULL_STOP};
WORD aAUTOPWRSetTime[14] = {8, CHAR_U, CHAR_s, CHAR_t, CHAR_SPACE, CHAR_c, CHAR_z, CHAR_a, CHAR_s};
WORD aMMBtnSave[10] = {6, CHAR_Z, CHAR_a, CHAR_p, CHAR_i, CHAR_s, CHAR_z};
WORD aMMSaveFileDlgTitle[20] = {11, CHAR_Z, CHAR_a, CHAR_p, CHAR_i, CHAR_s, CHAR_z, CHAR_SPACE, CHAR_p, CHAR_l, CHAR_i, CHAR_k};
WORD aMMSaveFileDlgContent[18] = {8, CHAR_Z, CHAR_a, CHAR_c, CHAR_z, CHAR_e, CHAR_k, CHAR_a, CHAR_j};
WORD aTHUMBButtonSort[10] = {6, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_j};
WORD aTHUMBButtonDeleteAll[11] = {9, CHAR_U, CHAR_s, CHAR_u, CHAR_n2, CHAR_SPACE, CHAR_w, CHAR_s, CHAR_z, CHAR_FULL_STOP};
WORD aTHUMBTitleEditMode[18] = {11, CHAR_T, CHAR_r, CHAR_y, CHAR_b, CHAR_SPACE, CHAR_E, CHAR_d, CHAR_y, CHAR_t, CHAR_u, CHAR_j};
WORD aTHUMBTitleDeleteMode[19] = {9, CHAR_T, CHAR_r, CHAR_y, CHAR_b, CHAR_SPACE, CHAR_U, CHAR_s, CHAR_u, CHAR_n2};
WORD aTHUMBHelpDeleteMode[14] = {7, CHAR_U, CHAR_s, CHAR_u, CHAR_n2, CHAR_SPACE, CHAR_t, CHAR_o};
WORD aTHUMBHelpSortMode[26] = {14, CHAR_O, CHAR_s, CHAR_t, CHAR_r, CHAR_o, CHAR_s1, CHAR_c2, CHAR_SPACE, CHAR_o, CHAR_b, CHAR_r, CHAR_a, CHAR_z, CHAR_u};
WORD aTHUMBTitleSortMode[18] = {11, CHAR_T, CHAR_r, CHAR_y, CHAR_b, CHAR_SPACE, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_j};
WORD aEDITDialogTitle[27] = {21, CHAR_O, CHAR_k, CHAR_n, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_a, CHAR_l, CHAR_o, CHAR_g, CHAR_o, CHAR_w, CHAR_e, CHAR_SPACE, CHAR_E, CHAR_d, CHAR_y, CHAR_t, CHAR_u, CHAR_j};
WORD aEDITDialogContent[38] = {19, CHAR_W, CHAR_y, CHAR_b, CHAR_i, CHAR_e, CHAR_r, CHAR_z, CHAR_SPACE, CHAR_t, CHAR_r, CHAR_y, CHAR_b, CHAR_SPACE, CHAR_E, CHAR_d, CHAR_y, CHAR_t, CHAR_u, CHAR_j};
WORD aBUTTONCopy[9] = {6, CHAR_K, CHAR_o, CHAR_p, CHAR_i, CHAR_u, CHAR_j};
WORD aBUTTONDelete[9] = {4, CHAR_U, CHAR_s, CHAR_u, CHAR_n2};
WORD aSETDATESetDate[17] = {10, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_t, CHAR_e5};
#endif //#ifdef SUPPORT_POWERON_MENU									

WORD aMENUOn[5] = {2, CHAR_W, CHAR_l1};
WORD aMENUOff[7] = {3, CHAR_W, CHAR_y, CHAR_l1};
#ifdef MUSTEK_SETUP_MENU									

WORD aMENU5Min[7] = {6, CHAR_5, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU30Min[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU1Hour[8] = {6, CHAR_1, CHAR_SPACE, CHAR_g, CHAR_o, CHAR_d, CHAR_z};
WORD aMENU4Hours[9] = {7, CHAR_4, CHAR_SPACE, CHAR_g, CHAR_o, CHAR_d, CHAR_z, CHAR_FULL_STOP};
WORD aMENU12Hours[10] = {8, CHAR_1, CHAR_2, CHAR_SPACE, CHAR_g, CHAR_o, CHAR_d, CHAR_z, CHAR_FULL_STOP};
WORD aMENU1Day[9] = {5, CHAR_1, CHAR_SPACE, CHAR_d, CHAR_z, CHAR_FULL_STOP};
WORD aMENUTransition[14] = {13, CHAR_T, CHAR_r, CHAR_a, CHAR_n, CHAR_s, CHAR_f, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_c, CHAR_j, CHAR_a};
WORD aMENUTVSystem[17] = {9, CHAR_S, CHAR_y, CHAR_s, CHAR_t, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMENUFullScreen[15] = {11, CHAR_P, CHAR_e, CHAR_l1, CHAR_n, CHAR_y, CHAR_SPACE, CHAR_e, CHAR_k, CHAR_r, CHAR_a, CHAR_n};
WORD aMENUReset[8] = {7, CHAR_R, CHAR_e, CHAR_s, CHAR_e, CHAR_t, CHAR_u, CHAR_j};
WORD aMENUConfirm[8] = {5, CHAR_P, CHAR_o, CHAR_t, CHAR_w, CHAR_FULL_STOP};
WORD aMENUExitSetup[30] = {24, CHAR_W, CHAR_y, CHAR_j, CHAR_d, CHAR_z2, CHAR_SPACE, CHAR_z, CHAR_SPACE, CHAR_t, CHAR_r, CHAR_y, CHAR_b, CHAR_u, CHAR_SPACE, CHAR_U, CHAR_s, CHAR_t, CHAR_a, CHAR_w, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_a};
#endif //MUSTEK_SETUP_MENU											
#ifdef SUPPORT_JPEG_AUTO_AND_FULL_SCREEN
WORD aMENUFullScreen[15] = {11, CHAR_P, CHAR_e, CHAR_l1, CHAR_n, CHAR_y, CHAR_SPACE, CHAR_e, CHAR_k, CHAR_r, CHAR_a, CHAR_n};
#endif
WORD aBUTTONYes[4] = {3, CHAR_T, CHAR_a, CHAR_k};
WORD aBUTTONNo[5] = {3, CHAR_N, CHAR_i, CHAR_e};
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW											

WORD aMMSaveDlgTitle[31] = {17, CHAR_O, CHAR_k, CHAR_n, CHAR_o, CHAR_SPACE, CHAR_z, CHAR_a, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_z, CHAR_d, CHAR_j, CHAR_e5, CHAR_c, CHAR_i, CHAR_a};
WORD aMMDeleteDlgTitle[31] = {18, CHAR_O, CHAR_k, CHAR_n, CHAR_o, CHAR_SPACE, CHAR_u, CHAR_s, CHAR_u, CHAR_w, CHAR_FULL_STOP, CHAR_SPACE, CHAR_z, CHAR_d, CHAR_j, CHAR_e5, CHAR_c, CHAR_i, CHAR_a};
WORD aMMSaveDlgContent[14] = {8, CHAR_Z, CHAR_a, CHAR_p, CHAR_i, CHAR_s, CHAR_a, CHAR_c2, CHAR_QUESTION_MARK};
WORD aMMDeleteDlgContent[13] = {7, CHAR_U, CHAR_s, CHAR_u, CHAR_n, CHAR_a6, CHAR_c2, CHAR_QUESTION_MARK};
#endif //#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW									

#ifdef SUPPORT_CHOOSE_MEDIA									

WORD aMENUMedia[11] = {5, CHAR_M, CHAR_e, CHAR_d, CHAR_i, CHAR_a};
#endif //SUPPORT_CHOOSE_MEDIA									

#ifdef SUPPORT_PHOTO_BROWSE_MODE									

WORD aNDBrowseMode[26] = {15, CHAR_T, CHAR_r, CHAR_y, CHAR_b, CHAR_SPACE, CHAR_P, CHAR_r, CHAR_z, CHAR_e, CHAR_g, CHAR_l, CHAR_a6, CHAR_d, CHAR_a, CHAR_j};
#endif //SUPPORT_PHOTO_BROWSE_MODE									

#ifdef SUPPORT_TFT_DIGITAL_MODE									

WORD aMENUAVOutput[13] = {10, CHAR_W, CHAR_y, CHAR_j, CHAR_s1, CHAR_c, CHAR_i, CHAR_e, CHAR_SPACE, CHAR_A, CHAR_V};
#endif //SUPPORT_TFT_DIGITAL_MODE									

WORD aMENUPanel[9] = {5, CHAR_P, CHAR_a, CHAR_n, CHAR_e, CHAR_l};
#ifdef SUPPORT_RADIO_TUNER									

WORD aRADIORecord[9] = {5, CHAR_N, CHAR_a, CHAR_g, CHAR_r, CHAR_FULL_STOP};
WORD aRADIOScan[9] = {6, CHAR_S, CHAR_k, CHAR_a, CHAR_n, CHAR_u, CHAR_j};
WORD aRADIOStationsFound[17] = {13, CHAR_z, CHAR_n, CHAR_a, CHAR_l, CHAR_FULL_STOP, CHAR_SPACE, CHAR_s, CHAR_t, CHAR_a, CHAR_c, CHAR_j, CHAR_i, CHAR_FULL_STOP};
#endif //SUPPORT_RADIO_TUNER									

#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									

WORD aCODEWesternEuropean[17] = {15, CHAR_Z, CHAR_a, CHAR_c, CHAR_h, CHAR_o, CHAR_d, CHAR_n, CHAR_i, CHAR_o, CHAR_e, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_FULL_STOP};
#endif   // SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									

#ifdef SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									

WORD aCODECentralEuropean[19] = {14, CHAR_S1, CHAR_r, CHAR_o, CHAR_d, CHAR_k, CHAR_o, CHAR_w, CHAR_o, CHAR_e, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_FULL_STOP};
#endif   // SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									

#ifdef SUPPORT_CHAR_ENCODING_GREEK									

WORD aCODEGreek[11] = {6, CHAR_G, CHAR_r, CHAR_e, CHAR_c, CHAR_k, CHAR_i};
#endif   // SUPPORT_CHAR_ENCODING_GREEK									

#ifdef SUPPORT_CHAR_ENCODING_HEBREW									

WORD aCODEHebrew[10] = {9, CHAR_H, CHAR_e, CHAR_b, CHAR_r, CHAR_a, CHAR_j, CHAR_s, CHAR_k, CHAR_i};
#endif   // SUPPORT_CHAR_ENCODING_HEBREW									

#ifdef SUPPORT_CHAR_ENCODING_TURKISH									

WORD aCODETurkish[9] = {7, CHAR_T, CHAR_u, CHAR_r, CHAR_e, CHAR_c, CHAR_k, CHAR_i};
#endif   // SUPPORT_CHAR_ENCODING_TURKISH									

#ifdef SUPPORT_CHAR_ENCODING_CP1250									

WORD aCODECP1250[8] = {7, CHAR_C, CHAR_P, CHAR_SPACE, CHAR_1, CHAR_2, CHAR_5, CHAR_0};
#endif   // SUPPORT_CHAR_ENCODING_CP1250									

#ifdef SUPPORT_CHAR_ENCODING_CYRILLIC									

WORD aCODECyrillic[11] = {8, CHAR_C, CHAR_y, CHAR_r, CHAR_y, CHAR_l, CHAR_i, CHAR_c, CHAR_a};
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
