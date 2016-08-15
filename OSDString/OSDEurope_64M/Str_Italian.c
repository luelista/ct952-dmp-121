#include "stringdec.h"
#ifdef OSDDSPLY_MAIN_FILE									
WORD aDSPLYLanguageJapanese[11] = {10, CHAR_G, CHAR_i, CHAR_a, CHAR_p, CHAR_p, CHAR_o, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aDSPLYLanguageEnglish[10] = {7, CHAR_I, CHAR_n, CHAR_g, CHAR_l, CHAR_e, CHAR_s, CHAR_e};
WORD aDSPLYLanguageFrench[12] = {8, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_e, CHAR_s, CHAR_e};
WORD aDSPLYLanguageGerman[10] = {7, CHAR_T, CHAR_e, CHAR_d, CHAR_e, CHAR_s, CHAR_c, CHAR_o};
WORD aDSPLYLanguageRussian[9] = {5, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_o};
WORD aDSPLYLanguageChinese[11] = {6, CHAR_C, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aDSPLYLanguageKorean[11] = {7, CHAR_C, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_o};
WORD aDSPLYLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aDSPLYLanguageThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aDSPLYLanguageItalian[12] = {8, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n, CHAR_o};
WORD aDSPLYLanguageSpanish[11] = {8, CHAR_S, CHAR_p, CHAR_a, CHAR_g, CHAR_n, CHAR_o, CHAR_l, CHAR_o};
WORD aDSPLYLanguagePortuguese[14] = {10, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_o, CHAR_g, CHAR_h, CHAR_e, CHAR_s, CHAR_e};
WORD aDSPLYLanguageOthers[9] = {5, CHAR_A, CHAR_l, CHAR_t, CHAR_r, CHAR_o};
WORD aDSPLYOff[10] = {9, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a};
#ifndef NO_DVD_DISPLAY_UI									
WORD aDSPLYTitleElapsed[17] = {11, CHAR_T, CHAR_i, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_p, CHAR_r, CHAR_o, CHAR_d};
WORD aDSPLYTitleRemain[16] = {10, CHAR_T, CHAR_i, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_m, CHAR_a, CHAR_n};
WORD aDSPLYChapterElapsed[16] = {11, CHAR_C, CHAR_a, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_p, CHAR_r, CHAR_o, CHAR_d};
WORD aDSPLYChapterRemain[15] = {10, CHAR_C, CHAR_a, CHAR_p, CHAR_FULL_STOP, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_m, CHAR_a, CHAR_n};
#endif //NO_DVD_DISPLAY_UI									
WORD aDSPLYSingleElapsed[17] = {15, CHAR_S, CHAR_i, CHAR_n, CHAR_g, CHAR_o, CHAR_l, CHAR_o, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_p, CHAR_r, CHAR_o, CHAR_d, CHAR_FULL_STOP};
WORD aDSPLYSingleRemain[16] = {13, CHAR_S, CHAR_i, CHAR_n, CHAR_g, CHAR_o, CHAR_l, CHAR_o, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_m, CHAR_a, CHAR_n};
WORD aDSPLYTotalElapsed[18] = {10, CHAR_T, CHAR_o, CHAR_t, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_p, CHAR_r, CHAR_o, CHAR_d};
WORD aDSPLYTotalRemain[17] = {9, CHAR_T, CHAR_o, CHAR_t, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_m, CHAR_a, CHAR_n};
WORD aDSPLYRootMenu[16] = {15, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_i, CHAR_n, CHAR_c, CHAR_i, CHAR_p, CHAR_a, CHAR_l, CHAR_e};
WORD aDSPLYTitleMenu[12] = {11, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_t, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_i};
#ifdef SUPPORT_NAVIGATOR									
WORD aDSPLYRepeatTitle[20] = {13, CHAR_R, CHAR_i, CHAR_p, CHAR_e, CHAR_t, CHAR_i, CHAR_SPACE, CHAR_t, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_o};
WORD aDSPLYRepeatChapter[22] = {15, CHAR_R, CHAR_i, CHAR_p, CHAR_e, CHAR_t, CHAR_i, CHAR_SPACE, CHAR_c, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_o};
WORD aDSPLYRepeatDisc[19] = {12, CHAR_R, CHAR_i, CHAR_p, CHAR_e, CHAR_t, CHAR_i, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aDSPLYRepeatOff[22] = {16, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_p, CHAR_e, CHAR_t, CHAR_i};
#endif //SUPPORT_NAVIGATOR									
WORD aDSPLYpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aDSPLYInterlace[15] = {11, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_l, CHAR_a, CHAR_c, CHAR_c, CHAR_i, CHAR_a};
WORD aDSPLYResolution[15] = {12, CHAR_R, CHAR_i, CHAR_s, CHAR_o, CHAR_l, CHAR_u, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_COLON};
WORD aDSPLYDate[7] = {5, CHAR_D, CHAR_a, CHAR_t, CHAR_a, CHAR_COLON};
#ifdef SUPPORT_NAVIGATOR									
WORD aNGTitle[8] = {7, CHAR_T, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_o, CHAR_COLON};
WORD aNGChapter[11] = {9, CHAR_C, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_o, CHAR_COLON};
WORD aNGAngle[13] = {12, CHAR_A, CHAR_n, CHAR_g, CHAR_o, CHAR_l, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_COLON};
WORD aNGAudio[7] = {6, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o, CHAR_COLON};
WORD aNGSubtitle1[13] = {10, CHAR_S, CHAR_o, CHAR_t, CHAR_t, CHAR_o, CHAR_t, CHAR_i, CHAR_t, CHAR_1, CHAR_COLON};
WORD aNGSubtitle2[13] = {10, CHAR_S, CHAR_o, CHAR_t, CHAR_t, CHAR_o, CHAR_t, CHAR_i, CHAR_t, CHAR_2, CHAR_COLON};
#endif //SUPPORT_NAVIGATOR									
#endif //OSDDSPLY_MAIN_FILE									
#ifdef OSDND_MAIN_FILE									
WORD aNDNoDisc[14] = {11, CHAR_M, CHAR_a, CHAR_n, CHAR_c, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aNDNoUSBDevice[14] = {13, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_U, CHAR_S, CHAR_B, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_v, CHAR_i, CHAR_c, CHAR_e};
WORD aNDOpen[7] = {4, CHAR_A, CHAR_p, CHAR_r, CHAR_i};
WORD aNDClose[10] = {6, CHAR_C, CHAR_h, CHAR_i, CHAR_u, CHAR_d, CHAR_i};
WORD aNDReturn[8] = {7, CHAR_R, CHAR_i, CHAR_t, CHAR_o, CHAR_r, CHAR_n, CHAR_a};
WORD aNDFreeze[15] = {14, CHAR_F, CHAR_e, CHAR_r, CHAR_m, CHAR_o, CHAR_SPACE, CHAR_i, CHAR_m, CHAR_m, CHAR_a, CHAR_g, CHAR_i, CHAR_n, CHAR_e};
WORD aNDRandomOn[12] = {8, CHAR_R, CHAR_i, CHAR_p, CHAR_r, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_s};
WORD aNDRandomOff[12] = {8, CHAR_R, CHAR_i, CHAR_p, CHAR_r, CHAR_SPACE, CHAR_O, CHAR_r, CHAR_d};
WORD aNDResume[11] = {8, CHAR_R, CHAR_i, CHAR_p, CHAR_r, CHAR_e, CHAR_n, CHAR_d, CHAR_i};
WORD aNDRootMenu[16] = {15, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_i, CHAR_n, CHAR_c, CHAR_i, CHAR_p, CHAR_a, CHAR_l, CHAR_e};
WORD aNDTitleMenu[12] = {11, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_t, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_i};
WORD aNDRotateRight[26] = {21, CHAR_R, CHAR_u, CHAR_o, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_s, CHAR_t, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_SPACE, CHAR_9, CHAR_0, CHAR_N_DEGREE};
WORD aNDRotateLeft[26] = {23, CHAR_R, CHAR_u, CHAR_o, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_s, CHAR_i, CHAR_n, CHAR_i, CHAR_s, CHAR_t, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_SPACE, CHAR_9, CHAR_0, CHAR_N_DEGREE};
WORD aNDFlipHorizontally[26] = {25, CHAR_C, CHAR_a, CHAR_p, CHAR_o, CHAR_v, CHAR_o, CHAR_l, CHAR_g, CHAR_i, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_i, CHAR_z, CHAR_z, CHAR_o, CHAR_n, CHAR_t, CHAR_a, CHAR_l, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_e};
WORD aNDFlipVertically[25] = {23, CHAR_C, CHAR_a, CHAR_p, CHAR_o, CHAR_v, CHAR_o, CHAR_l, CHAR_g, CHAR_i, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_c, CHAR_a, CHAR_l, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_e};
WORD aNDpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aNDInterlace[15] = {11, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_l, CHAR_a, CHAR_c, CHAR_c, CHAR_i, CHAR_a};
WORD aNDRepeatNone[10] = {9, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a};
WORD aNDRepeatTrack[8] = {7, CHAR_t, CHAR_r, CHAR_a, CHAR_c, CHAR_c, CHAR_i, CHAR_a};
WORD aNDRepeatDisc[7] = {5, CHAR_d, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aNDRepeatAll[10] = {5, CHAR_t, CHAR_u, CHAR_t, CHAR_t, CHAR_o};
WORD aNDRepeatTitle[7] = {6, CHAR_t, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_o};
WORD aNDRepeatChapter[10] = {8, CHAR_c, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_o};
WORD aNDRepeatFolder[9] = {8, CHAR_c, CHAR_a, CHAR_r, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_a};
WORD aNDRepeatA[15] = {8, CHAR_R, CHAR_i, CHAR_p, CHAR_e, CHAR_t, CHAR_i, CHAR_SPACE, CHAR_A};
WORD aNDRepeatAB[17] = {10, CHAR_R, CHAR_i, CHAR_p, CHAR_e, CHAR_t, CHAR_i, CHAR_SPACE, CHAR_A, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aNDRepeatOff[22] = {16, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_p, CHAR_e, CHAR_t, CHAR_i};
#ifndef NO_DISC_MODE									
WORD aNDNavigateOn[25] = {18, CHAR_A, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aNDNavigateOff[28] = {21, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aNDPBCOn[13] = {10, CHAR_A, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_P, CHAR_B, CHAR_C};
WORD aNDPBCOff[14] = {13, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_P, CHAR_B, CHAR_C};
#endif //#ifndef NO_DISC_MODE									
WORD aNDZoom1[8] = {7, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_SPACE, CHAR_1, CHAR_x};
WORD aNDZoom2[8] = {7, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_SPACE, CHAR_2, CHAR_x};
WORD aNDZoom4[8] = {7, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_SPACE, CHAR_4, CHAR_x};
WORD aNDMoveLeft[24] = {17, CHAR_S, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_s, CHAR_i, CHAR_n, CHAR_i, CHAR_s, CHAR_t, CHAR_r, CHAR_a};
WORD aNDMoveRight[24] = {15, CHAR_S, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_s, CHAR_t, CHAR_r, CHAR_a};
WORD aNDMoveUp[23] = {14, CHAR_S, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_t, CHAR_o};
WORD aNDMoveDown[23] = {15, CHAR_S, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_b, CHAR_a, CHAR_s, CHAR_s, CHAR_o};
WORD aNDVocalOn[15] = {12, CHAR_A, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_n, CHAR_t, CHAR_o};
WORD aNDVocalOff[16] = {15, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_n, CHAR_t, CHAR_o};
WORD aNDVocalLL[15] = {14, CHAR_C, CHAR_a, CHAR_n, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_s, CHAR_i, CHAR_n, CHAR_i, CHAR_s, CHAR_t, CHAR_r, CHAR_a};
WORD aNDVocalRR[13] = {12, CHAR_C, CHAR_a, CHAR_n, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_e, CHAR_s, CHAR_t, CHAR_r, CHAR_a};
WORD aNDKaraokeOn[17] = {14, CHAR_A, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e};
WORD aNDKaraokeOff[18] = {17, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e};
WORD aNDJPEGEffectNone[12] = {10, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_E, CHAR_f, CHAR_f, CHAR_e, CHAR_t, CHAR_t, CHAR_i};
WORD aNDJPEGEffectCoverUp[27] = {21, CHAR_M, CHAR_a, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_a, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_t, CHAR_o};
WORD aNDJPEGEffectCoverDown[26] = {22, CHAR_M, CHAR_a, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_a, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_b, CHAR_a, CHAR_s, CHAR_s, CHAR_o};
WORD aNDJPEGEffectCoverLeft[20] = {9, CHAR_C, CHAR_o, CHAR_p, CHAR_r, CHAR_i, CHAR_SPACE, CHAR_S, CHAR_i, CHAR_n};
WORD aNDJPEGEffectCoverRight[20] = {9, CHAR_C, CHAR_o, CHAR_p, CHAR_r, CHAR_i, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s};
WORD aNDJPEGEffectUncoverUp[23] = {22, CHAR_S, CHAR_m, CHAR_a, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_a, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_t, CHAR_o};
WORD aNDJPEGEffectUncoverDown[24] = {23, CHAR_S, CHAR_m, CHAR_a, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_a, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_b, CHAR_a, CHAR_s, CHAR_s, CHAR_o};
WORD aNDJPEGEffectUncoverLeft[21] = {10, CHAR_S, CHAR_c, CHAR_o, CHAR_p, CHAR_r, CHAR_i, CHAR_SPACE, CHAR_S, CHAR_i, CHAR_n};
WORD aNDJPEGEffectUncoverRight[22] = {13, CHAR_S, CHAR_c, CHAR_o, CHAR_p, CHAR_r, CHAR_i, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s, CHAR_t, CHAR_r, CHAR_a};
WORD aNDJPEGEffectWipeUp[22] = {19, CHAR_D, CHAR_i, CHAR_s, CHAR_s, CHAR_o, CHAR_l, CHAR_v, CHAR_e, CHAR_n, CHAR_z, CHAR_a, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_t, CHAR_o};
WORD aNDJPEGEffectWipeDown[21] = {20, CHAR_D, CHAR_i, CHAR_s, CHAR_s, CHAR_o, CHAR_l, CHAR_v, CHAR_e, CHAR_n, CHAR_z, CHAR_a, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_b, CHAR_a, CHAR_s, CHAR_s, CHAR_o};
WORD aNDJPEGEffectWipeLeft[17] = {8, CHAR_C, CHAR_o, CHAR_m, CHAR_p, CHAR_SPACE, CHAR_S, CHAR_i, CHAR_n};
WORD aNDJPEGEffectWipeRight[18] = {8, CHAR_C, CHAR_o, CHAR_m, CHAR_p, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s};
WORD aNDJPEGEffectPushUp[14] = {7, CHAR_P, CHAR_r, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_S, CHAR_u};
WORD aNDJPEGEffectPushDown[13] = {9, CHAR_P, CHAR_r, CHAR_e, CHAR_m, CHAR_i, CHAR_SPACE, CHAR_G, CHAR_i, CHAR_u3};
WORD aNDJPEGEffectPushLeft[17] = {9, CHAR_P, CHAR_r, CHAR_e, CHAR_m, CHAR_i, CHAR_SPACE, CHAR_S, CHAR_i, CHAR_n};
WORD aNDJPEGEffectPushRight[18] = {9, CHAR_P, CHAR_r, CHAR_e, CHAR_m, CHAR_i, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s};
WORD aNDJPEGEffectFadeThroughBlack[26] = {17, CHAR_S, CHAR_f, CHAR_u, CHAR_m, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_n, CHAR_e, CHAR_r, CHAR_o};
WORD aNDJPEGEffectRandom[16] = {8, CHAR_R, CHAR_i, CHAR_p, CHAR_r, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_s};
#ifndef NO_DISC_MODE									
WORD aNDEncrypt[17] = {16, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_n, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_v, CHAR_a, CHAR_l, CHAR_i, CHAR_d, CHAR_o};
WORD aNDErrRegionCode[27] = {20, CHAR_Z, CHAR_o, CHAR_n, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_s, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_s, CHAR_b, CHAR_a, CHAR_g, CHAR_l, CHAR_i, CHAR_a, CHAR_t, CHAR_a};
WORD aNDWrongDisc[19] = {15, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_s, CHAR_b, CHAR_a, CHAR_g, CHAR_l, CHAR_i, CHAR_a, CHAR_t, CHAR_a};
WORD aNDDamageDisc[19] = {17, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_n, CHAR_n, CHAR_e, CHAR_g, CHAR_g, CHAR_i, CHAR_a, CHAR_t, CHAR_o};
#endif //#ifndef NO_DISC_MODE									
WORD aNDNoPlayableFile[25] = {16, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y, CHAR_a, CHAR_b, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_F, CHAR_i, CHAR_l, CHAR_e};
WORD aNDDisc[7] = {5, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aNDTrack[8] = {7, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_c, CHAR_i, CHAR_a};
WORD aNDTitle[7] = {6, CHAR_T, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_o};
WORD aNDChapter[10] = {8, CHAR_C, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_o};
WORD aNDLR[4] = {3, CHAR_S, CHAR_SOLIDUS, CHAR_D};
WORD aNDLL[4] = {3, CHAR_S, CHAR_SOLIDUS, CHAR_S};
WORD aNDRR[4] = {3, CHAR_D, CHAR_SOLIDUS, CHAR_D};
WORD aNDVolume[11] = {6, CHAR_V, CHAR_o, CHAR_l, CHAR_u, CHAR_m, CHAR_e};
WORD aNDTone[9] = {8, CHAR_T, CHAR_o, CHAR_n, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1};
WORD aNDEcho[5] = {3, CHAR_E, CHAR_c, CHAR_o};
WORD aNDNoSurround[19] = {11, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_S, CHAR_u, CHAR_r, CHAR_r, CHAR_o, CHAR_u, CHAR_n, CHAR_d};
WORD aNDLiveSurround[10] = {4, CHAR_L, CHAR_i, CHAR_v, CHAR_e};
WORD aNDTheaterSurround[8] = {6, CHAR_T, CHAR_e, CHAR_a, CHAR_t, CHAR_r, CHAR_o};
WORD aNDStereoSurround[8] = {6, CHAR_S, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_o};
WORD aNDMovieSurround[9] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aNDKaraSurround[8] = {7, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e};
WORD aNDLanguageJapanese[11] = {10, CHAR_G, CHAR_i, CHAR_a, CHAR_p, CHAR_p, CHAR_o, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aNDLanguageEnglish[10] = {7, CHAR_I, CHAR_n, CHAR_g, CHAR_l, CHAR_e, CHAR_s, CHAR_e};
WORD aNDLanguageFrench[12] = {8, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_e, CHAR_s, CHAR_e};
WORD aNDLanguageGerman[10] = {7, CHAR_T, CHAR_e, CHAR_d, CHAR_e, CHAR_s, CHAR_c, CHAR_o};
WORD aNDLanguageRussian[9] = {5, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_o};
WORD aNDLanguageChinese[11] = {6, CHAR_C, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aNDLanguageKorean[11] = {7, CHAR_C, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_o};
WORD aNDLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aNDLanguageThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aNDLanguageItalian[12] = {8, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n, CHAR_o};
WORD aNDLanguageSpanish[11] = {8, CHAR_S, CHAR_p, CHAR_a, CHAR_g, CHAR_n, CHAR_o, CHAR_l, CHAR_o};
WORD aNDLanguagePortuguese[14] = {10, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_o, CHAR_g, CHAR_h, CHAR_e, CHAR_s, CHAR_e};
WORD aNDLanguageOthers[9] = {5, CHAR_A, CHAR_l, CHAR_t, CHAR_r, CHAR_o};
WORD aNDSubtitle[13] = {12, CHAR_S, CHAR_o, CHAR_t, CHAR_t, CHAR_o, CHAR_t, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_i, CHAR_COLON};
WORD aNDOff[10] = {9, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a};
WORD aNDWait[12] = {11, CHAR_C, CHAR_a, CHAR_r, CHAR_i, CHAR_c, CHAR_a, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_o};
WORD aNDSelect[12] = {9, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_a};
WORD aNDCardReader[29] = {14, CHAR_L, CHAR_e, CHAR_t, CHAR_t, CHAR_o, CHAR_r, CHAR_e, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_d, CHAR_a};
#endif //OSDND_MAIN_FILE									
#ifdef OSDMM_MAIN_FILE									
WORD aMMProgramModeTitle[24] = {23, CHAR_M, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aMMProgramListModeTitle[21] = {16, CHAR_E, CHAR_l, CHAR_e, CHAR_n, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_i};
WORD aMMTotalFiles[25] = {12, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_f, CHAR_i, CHAR_l, CHAR_e, CHAR_COLON};
WORD aMMFileNumber[20] = {12, CHAR_N, CHAR_u, CHAR_m, CHAR_e, CHAR_r, CHAR_o, CHAR_SPACE, CHAR_f, CHAR_i, CHAR_l, CHAR_e, CHAR_COLON};
WORD aMMDlgFileTypeTitle[14] = {10, CHAR_T, CHAR_I, CHAR_P, CHAR_O, CHAR_SPACE, CHAR_F, CHAR_I, CHAR_L, CHAR_E, CHAR_COLON};
WORD aMMDlgSortFileTitle[16] = {12, CHAR_O, CHAR_R, CHAR_D, CHAR_I, CHAR_N, CHAR_FULL_STOP, CHAR_SPACE, CHAR_F, CHAR_I, CHAR_L, CHAR_E, CHAR_COLON};
WORD aMMDlgSortFileMethodTitle[9] = {7, CHAR_M, CHAR_e, CHAR_t, CHAR_o, CHAR_d, CHAR_o, CHAR_COLON};
WORD aMMDlgSortFileSequenceTitle[12] = {9, CHAR_S, CHAR_e, CHAR_q, CHAR_u, CHAR_e, CHAR_n, CHAR_z, CHAR_a, CHAR_COLON};
WORD aMMDlgSequenceAscendingItem[10] = {9, CHAR_C, CHAR_r, CHAR_e, CHAR_s, CHAR_c, CHAR_e, CHAR_n, CHAR_t, CHAR_e};
WORD aMMDlgSequenceDescendingItem[12] = {10, CHAR_D, CHAR_e, CHAR_c, CHAR_r, CHAR_e, CHAR_s, CHAR_c, CHAR_e, CHAR_n, CHAR_FULL_STOP};
WORD aMMDlgMethodTypeItem[11] = {8, CHAR_P, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_t, CHAR_i, CHAR_p, CHAR_o};
WORD aMMDlgMethodNameItem[11] = {8, CHAR_P, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_n, CHAR_o, CHAR_m, CHAR_e};
WORD aMMDlgMethodTimeItem[10] = {7, CHAR_P, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_a};
WORD aMMHelpProgram[16] = {8, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_FULL_STOP};
WORD aMMHelpProgramList[14] = {7, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMMBtnFileType[10] = {9, CHAR_T, CHAR_i, CHAR_p, CHAR_o, CHAR_SPACE, CHAR_F, CHAR_i, CHAR_l, CHAR_e};
WORD aMMBtnSortFile[12] = {9, CHAR_O, CHAR_r, CHAR_d, CHAR_i, CHAR_n, CHAR_F, CHAR_i, CHAR_l, CHAR_e};
WORD aMMBtnList[7] = {6, CHAR_E, CHAR_l, CHAR_e, CHAR_n, CHAR_c, CHAR_a};
WORD aMMBtnPlay[9] = {4, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aMMBtnDeleteAll[11] = {10, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_SPACE, CHAR_t, CHAR_u, CHAR_t, CHAR_t, CHAR_o};
WORD aMMBtnReturn[8] = {5, CHAR_I, CHAR_n, CHAR_v, CHAR_i, CHAR_o};
WORD aMMBtnExit[8] = {4, CHAR_E, CHAR_s, CHAR_c, CHAR_i};
#endif //OSDMM_MAIN_FILE									
#ifdef THUMB_MAIN_FILE									
WORD aTHUMBPage[8] = {7, CHAR_P, CHAR_a, CHAR_g, CHAR_i, CHAR_n, CHAR_a, CHAR_COLON};
WORD aTHUMBTitleGeneralMode[19] = {18, CHAR_M, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_M, CHAR_i, CHAR_n, CHAR_i, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_e};
WORD aTHUMBTitleRotateMode[19] = {18, CHAR_M, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_R, CHAR_o, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aTHUMBTitleProgramMode[24] = {23, CHAR_M, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aTHUMBTitleProgramList[21] = {16, CHAR_E, CHAR_l, CHAR_e, CHAR_n, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_p, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_i};
WORD aTHUMBHelpRotateMode[17] = {5, CHAR_R, CHAR_u, CHAR_o, CHAR_t, CHAR_a};
WORD aTHUMBHelpProgramMode[16] = {8, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_FULL_STOP};
WORD aTHUMBHelpProgramList[14] = {7, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aTHUMBButtonRotate[8] = {5, CHAR_R, CHAR_u, CHAR_o, CHAR_t, CHAR_a};
WORD aTHUMBButtonSlideShow[11] = {9, CHAR_P, CHAR_r, CHAR_e, CHAR_s, CHAR_e, CHAR_n, CHAR_t, CHAR_a, CHAR_z};
WORD aTHUMBButtonReset[8] = {5, CHAR_R, CHAR_e, CHAR_s, CHAR_e, CHAR_t};
WORD aTHUMBButtonReturn[8] = {5, CHAR_I, CHAR_n, CHAR_v, CHAR_i, CHAR_o};
WORD aTHUMBButtonProgramList[7] = {6, CHAR_E, CHAR_l, CHAR_e, CHAR_n, CHAR_c, CHAR_a};
#endif //THUMB_MAIN_FILE									
#ifdef OSDDG_MAIN_FILE									
WORD aDGDlgBtnTrackDigest[21] = {12, CHAR_S, CHAR_i, CHAR_n, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_t, CHAR_r, CHAR_a, CHAR_c, CHAR_c, CHAR_e};
WORD aDGDlgBtnDiscInterval[21] = {13, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_FULL_STOP, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aDGPage[8] = {7, CHAR_P, CHAR_a, CHAR_g, CHAR_i, CHAR_n, CHAR_a, CHAR_COLON};
WORD aDGTitleTrackDigest[21] = {12, CHAR_S, CHAR_i, CHAR_n, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_t, CHAR_r, CHAR_a, CHAR_c, CHAR_c, CHAR_e};
WORD aDGTitleDiscInterval[21] = {13, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_FULL_STOP, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_s, CHAR_c, CHAR_o};
WORD aDGInfoTrack[9] = {8, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_c, CHAR_i, CHAR_a, CHAR_COLON};
WORD aDGBtnMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aDGBtnExit[8] = {4, CHAR_E, CHAR_s, CHAR_c, CHAR_i};
#endif //OSDDG_MAIN_FILE									
#ifdef OSDPRGM_MAIN_FILE									
WORD aProgram[12] = {11, CHAR_M, CHAR_O, CHAR_D, CHAR_FULL_STOP, CHAR_SPACE, CHAR_P, CHAR_R, CHAR_O, CHAR_G, CHAR_R, CHAR_FULL_STOP};
WORD aTitle[7] = {6, CHAR_T, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_o};
WORD aChapter[10] = {8, CHAR_C, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_o};
WORD aTrack[8] = {7, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_c, CHAR_i, CHAR_a};
WORD aSelect[12] = {9, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_a};
WORD aPlay[11] = {10, CHAR_E, CHAR_s, CHAR_e, CHAR_c, CHAR_u, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aClear[8] = {5, CHAR_A, CHAR_n, CHAR_n, CHAR_u, CHAR_l};
#endif //OSDPRGM_MAIN_FILE									
#ifdef OSDBMARK_MAIN_FILE									
WORD aBookmark[11] = {8, CHAR_B, CHAR_o, CHAR_o, CHAR_k, CHAR_m, CHAR_a, CHAR_r, CHAR_k};
WORD aTime[6] = {3, CHAR_O, CHAR_r, CHAR_a};
#endif //OSDBMARK_MAIN_FILE									
#ifdef SETUP_MAIN_FILE									
WORD aSetupSelect[12] = {9, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_a};
#endif //SETUP_MAIN_FILE									
#ifdef MENU_MAIN_FILE									
WORD aMsgEnglish[10] = {7, CHAR_I, CHAR_n, CHAR_g, CHAR_l, CHAR_e, CHAR_s, CHAR_e};
WORD aMsgChinese[11] = {6, CHAR_C, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aMsgJapanese[11] = {10, CHAR_G, CHAR_i, CHAR_a, CHAR_p, CHAR_p, CHAR_o, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aMsgFrench[12] = {8, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_e, CHAR_s, CHAR_e};
WORD aMsgGerman[10] = {7, CHAR_T, CHAR_e, CHAR_d, CHAR_e, CHAR_s, CHAR_c, CHAR_o};
WORD aMsgItalian[12] = {8, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n, CHAR_o};
WORD aMsgSpanish[11] = {8, CHAR_S, CHAR_p, CHAR_a, CHAR_g, CHAR_n, CHAR_o, CHAR_l, CHAR_o};
WORD aMsgPortuguese[14] = {10, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_o, CHAR_g, CHAR_h, CHAR_e, CHAR_s, CHAR_e};
WORD aMsgDutch[12] = {8, CHAR_O, CHAR_l, CHAR_a, CHAR_n, CHAR_d, CHAR_e, CHAR_s, CHAR_e};
WORD aMsgPolish[9] = {7, CHAR_P, CHAR_o, CHAR_l, CHAR_a, CHAR_c, CHAR_c, CHAR_o};
WORD aMsgHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aMsgThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aMsgRussian[9] = {5, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_o};
WORD aMsgKorean[11] = {7, CHAR_C, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_o};
WORD aMsgCzech[6] = {5, CHAR_C, CHAR_z, CHAR_e, CHAR_c, CHAR_h};
WORD aMsgGreece[7] = {6, CHAR_G, CHAR_r, CHAR_e, CHAR_e, CHAR_c, CHAR_e};
WORD aMsgTurkey[7] = {6, CHAR_T, CHAR_u, CHAR_r, CHAR_k, CHAR_e, CHAR_y};
WORD aMsgOthers[9] = {5, CHAR_A, CHAR_l, CHAR_t, CHAR_r, CHAR_o};
WORD aMsgOSD[4] = {3, CHAR_O, CHAR_S, CHAR_D};
WORD aMsgMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aMsgAudio[6] = {5, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMsgSubtitle[12] = {11, CHAR_S, CHAR_o, CHAR_t, CHAR_t, CHAR_o, CHAR_t, CHAR_i, CHAR_t, CHAR_o, CHAR_l, CHAR_i};
WORD aMsgPS[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_P, CHAR_S};
WORD aMsgLB[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_L, CHAR_B};
WORD aMsgWide[5] = {4, CHAR_1, CHAR_6, CHAR_COLON, CHAR_9};
WORD aMsgOn[9] = {6, CHAR_A, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a};
WORD aMsgOff[10] = {9, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_t, CHAR_t, CHAR_i, CHAR_v, CHAR_a};
WORD aMsgNTSC[5] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aMsgPAL[4] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aMsgAuto[11] = {10, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_m, CHAR_a, CHAR_t, CHAR_i, CHAR_c, CHAR_o};
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
WORD aMsgBriNormal[9] = {7, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l, CHAR_e};
WORD aMsgBright[10] = {8, CHAR_L, CHAR_u, CHAR_m, CHAR_i, CHAR_n, CHAR_o, CHAR_s, CHAR_o};
WORD aMsgSoft[8] = {7, CHAR_L, CHAR_e, CHAR_g, CHAR_g, CHAR_e, CHAR_r, CHAR_o};
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
WORD aMsgTVType[14] = {7, CHAR_T, CHAR_i, CHAR_p, CHAR_o, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMsgScreenSaver[18] = {12, CHAR_S, CHAR_c, CHAR_r, CHAR_e, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_S, CHAR_a, CHAR_v, CHAR_e, CHAR_r};
WORD aMsgTVSystem[17] = {10, CHAR_S, CHAR_i, CHAR_s, CHAR_t, CHAR_e, CHAR_m, CHAR_a, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMsgVideoOutput[14] = {12, CHAR_U, CHAR_s, CHAR_c, CHAR_i, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_v, CHAR_i, CHAR_d, CHAR_e, CHAR_o};
WORD aMsgBrightness[11] = {10, CHAR_L, CHAR_u, CHAR_m, CHAR_i, CHAR_n, CHAR_o, CHAR_s, CHAR_i, CHAR_t, CHAR_a1};
WORD aMsgContrast[10] = {9, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t, CHAR_o};
WORD aMsgChromPhase[4] = {3, CHAR_H, CHAR_u, CHAR_e};
WORD aMsgSPDIF_PCM[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_P, CHAR_C, CHAR_M};
WORD aMsgSPDIF_RAW[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_R, CHAR_A, CHAR_W};
WORD aMsg2CH[11] = {8, CHAR_2, CHAR_SPACE, CHAR_c, CHAR_a, CHAR_n, CHAR_a, CHAR_l, CHAR_i};
WORD aMsg51CH[13] = {10, CHAR_5, CHAR_FULL_STOP, CHAR_1, CHAR_SPACE, CHAR_c, CHAR_a, CHAR_n, CHAR_a, CHAR_l, CHAR_i};
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
WORD aMsgDigitalOut[18] = {15, CHAR_U, CHAR_s, CHAR_c, CHAR_i, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_g, CHAR_i, CHAR_t, CHAR_a, CHAR_l, CHAR_e};
WORD aMsgAnalogOut[18] = {16, CHAR_U, CHAR_s, CHAR_c, CHAR_i, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_n, CHAR_a, CHAR_l, CHAR_o, CHAR_g, CHAR_i, CHAR_c, CHAR_a};
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
WORD aMsgCenter[10] = {8, CHAR_C, CHAR_e, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_l, CHAR_e};
WORD aMsgRear[11] = {10, CHAR_P, CHAR_o, CHAR_s, CHAR_t, CHAR_e, CHAR_r, CHAR_i, CHAR_o, CHAR_r, CHAR_e};
WORD aMsgSubwoofer[10] = {9, CHAR_S, CHAR_u, CHAR_b, CHAR_w, CHAR_o, CHAR_o, CHAR_f, CHAR_e, CHAR_r};
WORD aMsgSpeakerTest[19] = {17, CHAR_T, CHAR_e, CHAR_s, CHAR_t, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_t, CHAR_o, CHAR_p, CHAR_a, CHAR_r, CHAR_l, CHAR_a, CHAR_n, CHAR_t, CHAR_i};
WORD aMsgLevel1[2] = {1, CHAR_1};
WORD aMsgLevel2[2] = {1, CHAR_2};
WORD aMsgLevel3[2] = {1, CHAR_3};
WORD aMsgLevel4[2] = {1, CHAR_4};
WORD aMsgLevel5[2] = {1, CHAR_5};
WORD aMsgLevel6[2] = {1, CHAR_6};
WORD aMsgLevel7[2] = {1, CHAR_7};
WORD aMsgLevel8[2] = {1, CHAR_8};
WORD aMsgRead[8] = {7, CHAR_L, CHAR_e, CHAR_t, CHAR_t, CHAR_u, CHAR_r, CHAR_a};
WORD aMsgParental[20] = {16, CHAR_C, CHAR_e, CHAR_n, CHAR_s, CHAR_u, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_g, CHAR_e, CHAR_n, CHAR_i, CHAR_t, CHAR_o, CHAR_r, CHAR_i};
WORD aMsgChangePassword[13] = {8, CHAR_P, CHAR_a, CHAR_s, CHAR_s, CHAR_w, CHAR_o, CHAR_r, CHAR_d};
WORD aMsgAngleMark[16] = {11, CHAR_I, CHAR_n, CHAR_d, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_n, CHAR_g, CHAR_o, CHAR_l, CHAR_FULL_STOP};
WORD aMsgUIStyle[7] = {5, CHAR_S, CHAR_t, CHAR_i, CHAR_l, CHAR_e};
WORD aMsgUIStyle1[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_1};
WORD aMsgUIStyle2[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_2};
WORD aMsgUIStyle3[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_3};
WORD aMsgUIStyle4[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_4};
WORD aMsgSmartPlay[14] = {10, CHAR_S, CHAR_m, CHAR_a, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aMsgDivXVOD[9] = {8, CHAR_D, CHAR_i, CHAR_v, CHAR_X, CHAR_SPACE, CHAR_V, CHAR_O, CHAR_D};
WORD aMsgDefaultValue[18] = {11, CHAR_P, CHAR_r, CHAR_e, CHAR_d, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_t, CHAR_o};
WORD aMsgLanguageSetup[23] = {19, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_g, CHAR_u, CHAR_a};
WORD aMsgScreenSetup[22] = {20, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_m, CHAR_o};
WORD aMsgAudioSetup[22] = {18, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMsgCustomSetup[28] = {27, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_SPACE, CHAR_p, CHAR_e, CHAR_r, CHAR_s, CHAR_o, CHAR_n, CHAR_a, CHAR_l, CHAR_i, CHAR_z, CHAR_z, CHAR_a, CHAR_t, CHAR_a};
WORD aMsgExitSetup[30] = {29, CHAR_E, CHAR_s, CHAR_c, CHAR_i, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_SPACE, CHAR_m, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aMsgPassword[31] = {20, CHAR_I, CHAR_n, CHAR_s, CHAR_e, CHAR_r, CHAR_i, CHAR_r, CHAR_e, CHAR_SPACE, CHAR_l, CHAR_a, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_s, CHAR_s, CHAR_w, CHAR_o, CHAR_r, CHAR_d};
WORD aMsgWrongPassword[20] = {18, CHAR_P, CHAR_a, CHAR_s, CHAR_s, CHAR_w, CHAR_o, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_o, CHAR_r, CHAR_r, CHAR_e, CHAR_t, CHAR_t, CHAR_a};
WORD aMsgOldPassword[9] = {8, CHAR_V, CHAR_e, CHAR_c, CHAR_c, CHAR_h, CHAR_i, CHAR_a, CHAR_COLON};
WORD aMsgNewPassword[10] = {6, CHAR_N, CHAR_u, CHAR_o, CHAR_v, CHAR_a, CHAR_COLON};
WORD aMsgVerifyPassword[12] = {9, CHAR_V, CHAR_e, CHAR_r, CHAR_i, CHAR_f, CHAR_i, CHAR_c, CHAR_a, CHAR_COLON};
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
WORD aMENUPhotoSetting[26] = {12, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_f, CHAR_o, CHAR_t, CHAR_o};
WORD aMENUIntervalTime[20] = {16, CHAR_T, CHAR_e, CHAR_m, CHAR_p, CHAR_o, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l, CHAR_l, CHAR_o};
#ifndef MUSTEK_SETUP_MENU									
WORD aMENU5Sec[7] = {6, CHAR_5, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_c, CHAR_FULL_STOP};
WORD aMENU10Sec[8] = {7, CHAR_1, CHAR_0, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_c, CHAR_FULL_STOP};
#endif //MUSTEK_SETUP_MENU									
WORD aMENU15Sec[8] = {6, CHAR_1, CHAR_5, CHAR_s, CHAR_e, CHAR_c, CHAR_FULL_STOP};
WORD aMENU30Sec[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_c, CHAR_FULL_STOP};
WORD aMENU1Min[7] = {6, CHAR_1, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU3Min[7] = {6, CHAR_3, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENUTransitionEffect[21] = {16, CHAR_E, CHAR_f, CHAR_f, CHAR_e, CHAR_t, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_t, CHAR_r, CHAR_a, CHAR_n, CHAR_s, CHAR_i, CHAR_z, CHAR_FULL_STOP};
WORD aMENUJPEGEffectNone[12] = {10, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_E, CHAR_f, CHAR_f, CHAR_e, CHAR_t, CHAR_t, CHAR_i};
WORD aMENUJPEGEffectCoverUp[17] = {16, CHAR_M, CHAR_a, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_SPACE};
WORD aMENUJPEGEffectCoverDown[18] = {15, CHAR_M, CHAR_a, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_b, CHAR_a, CHAR_s, CHAR_s, CHAR_o};
#ifndef MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectCoverLeft[16] = {9, CHAR_C, CHAR_o, CHAR_p, CHAR_r, CHAR_i, CHAR_SPACE, CHAR_S, CHAR_i, CHAR_n};
WORD aMENUJPEGEffectCoverRight[17] = {9, CHAR_C, CHAR_o, CHAR_p, CHAR_r, CHAR_i, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s};
WORD aMENUJPEGEffectUncoverUp[18] = {15, CHAR_S, CHAR_m, CHAR_a, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_t, CHAR_o};
WORD aMENUJPEGEffectUncoverDown[17] = {16, CHAR_S, CHAR_m, CHAR_a, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_b, CHAR_a, CHAR_s, CHAR_s, CHAR_o};
#endif //MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectUncoverLeft[16] = {10, CHAR_S, CHAR_c, CHAR_o, CHAR_p, CHAR_r, CHAR_i, CHAR_SPACE, CHAR_S, CHAR_i, CHAR_n};
WORD aMENUJPEGEffectUncoverRight[17] = {13, CHAR_S, CHAR_c, CHAR_o, CHAR_p, CHAR_r, CHAR_i, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s, CHAR_t, CHAR_r, CHAR_a};
WORD aMENUJPEGEffectWipeUp[20] = {19, CHAR_D, CHAR_i, CHAR_s, CHAR_s, CHAR_o, CHAR_l, CHAR_v, CHAR_e, CHAR_n, CHAR_z, CHAR_a, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_t, CHAR_o};
WORD aMENUJPEGEffectWipeDown[17] = {16, CHAR_D, CHAR_i, CHAR_s, CHAR_s, CHAR_FULL_STOP, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_b, CHAR_a, CHAR_s, CHAR_s, CHAR_o, CHAR_SPACE, CHAR_SPACE};
#ifndef MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectWipeLeft[17] = {8, CHAR_C, CHAR_o, CHAR_m, CHAR_p, CHAR_SPACE, CHAR_S, CHAR_i, CHAR_n};
WORD aMENUJPEGEffectWipeRight[18] = {8, CHAR_C, CHAR_o, CHAR_m, CHAR_p, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s};
WORD aMENUJPEGEffectPushUp[14] = {7, CHAR_P, CHAR_r, CHAR_e, CHAR_m, CHAR_SPACE, CHAR_S, CHAR_u};
WORD aMENUJPEGEffectPushDown[13] = {9, CHAR_P, CHAR_r, CHAR_e, CHAR_m, CHAR_i, CHAR_SPACE, CHAR_G, CHAR_i, CHAR_u3};
WORD aMENUJPEGEffectPushLeft[17] = {9, CHAR_P, CHAR_r, CHAR_e, CHAR_m, CHAR_i, CHAR_SPACE, CHAR_S, CHAR_i, CHAR_n};
#endif //MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectPushRight[18] = {9, CHAR_P, CHAR_r, CHAR_e, CHAR_m, CHAR_i, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_s};
WORD aMENUJPEGEffectFadeThroughBlack[26] = {17, CHAR_S, CHAR_f, CHAR_u, CHAR_m, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_n, CHAR_e, CHAR_r, CHAR_o};
WORD aMENUJPEGEffectRandom[16] = {8, CHAR_R, CHAR_i, CHAR_p, CHAR_r, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_s};
WORD aMENUBrowse[12] = {7, CHAR_E, CHAR_s, CHAR_p, CHAR_l, CHAR_FULL_STOP, CHAR_SPACE, CHAR_SPACE};
WORD aMENUFileManager[16] = {14, CHAR_G, CHAR_e, CHAR_s, CHAR_t, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_SPACE, CHAR_f, CHAR_i, CHAR_l, CHAR_e, CHAR_SPACE};
WORD aMENUSetupMenu[16] = {12, CHAR_M, CHAR_e, CHAR_n, CHAR_u, CHAR_SPACE, CHAR_c, CHAR_o, CHAR_n, CHAR_f, CHAR_FULL_STOP, CHAR_SPACE, CHAR_SPACE};
WORD aMENUThumbnail[13] = {9, CHAR_A, CHAR_n, CHAR_t, CHAR_e, CHAR_p, CHAR_r, CHAR_i, CHAR_m, CHAR_a};
WORD aMENUAutoPlay[14] = {11, CHAR_R, CHAR_i, CHAR_p, CHAR_r, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_t, CHAR_o, CHAR_SPACE};
WORD aMENUPhoto[12] = {4, CHAR_F, CHAR_o, CHAR_t, CHAR_o};
WORD aMENUMusic[7] = {4, CHAR_M, CHAR_u, CHAR_s, CHAR_FULL_STOP};
WORD aMENUPhotoAudio[14] = {10, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_SOLIDUS, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMENUMovie[10] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aMENUDisplaySetup[16] = {15, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_FULL_STOP, CHAR_SPACE, CHAR_S, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_m, CHAR_o};
WORD aMENUDisplayType[14] = {12, CHAR_T, CHAR_i, CHAR_p, CHAR_o, CHAR_SPACE, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_m, CHAR_o};
WORD aMENUTVStandard[12] = {11, CHAR_T, CHAR_V, CHAR_SPACE, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_r, CHAR_d};
WORD aMENUNTSC[5] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aMENUPAL[4] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aMENUAUTO[11] = {10, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_m, CHAR_a, CHAR_t, CHAR_i, CHAR_c, CHAR_o};
WORD aMENUBrightness[11] = {10, CHAR_L, CHAR_u, CHAR_m, CHAR_i, CHAR_n, CHAR_o, CHAR_s, CHAR_i, CHAR_t, CHAR_a1};
WORD aMENUContrast[10] = {9, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t, CHAR_o};
WORD aMENUNormal[9] = {7, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l, CHAR_e};
WORD aMENUBright[10] = {8, CHAR_L, CHAR_u, CHAR_m, CHAR_i, CHAR_n, CHAR_o, CHAR_s, CHAR_o};
WORD aMENUSoft[8] = {7, CHAR_L, CHAR_e, CHAR_g, CHAR_g, CHAR_e, CHAR_r, CHAR_o};
WORD aMENUTFTBrightness[15] = {14, CHAR_L, CHAR_u, CHAR_m, CHAR_i, CHAR_n, CHAR_o, CHAR_s, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_T, CHAR_F, CHAR_T};
WORD aMENUTFTContrast[14] = {13, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_T, CHAR_F, CHAR_T};
WORD aMENUTFTSaturation[16] = {15, CHAR_S, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_F, CHAR_T};
WORD aMENURepeatMode[15] = {11, CHAR_M, CHAR_o, CHAR_d, CHAR_FULL_STOP, CHAR_SPACE, CHAR_r, CHAR_i, CHAR_p, CHAR_e, CHAR_t, CHAR_i};
WORD aMENURepeatOne[9] = {8, CHAR_U, CHAR_n, CHAR_a, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE};
WORD aMENURepeatFolder[9] = {8, CHAR_c, CHAR_a, CHAR_r, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_a};
WORD aMENURepeatAll[10] = {5, CHAR_t, CHAR_u, CHAR_t, CHAR_t, CHAR_o};
WORD aMENUCustomSetup[28] = {27, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_SPACE, CHAR_p, CHAR_e, CHAR_r, CHAR_s, CHAR_o, CHAR_n, CHAR_a, CHAR_l, CHAR_i, CHAR_z, CHAR_z, CHAR_a, CHAR_t, CHAR_a};
WORD aMENUOSDLanguage[14] = {13, CHAR_L, CHAR_i, CHAR_g, CHAR_u, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_v, CHAR_i, CHAR_d, CHAR_e, CHAR_o};
WORD aMENUEnglish[10] = {7, CHAR_I, CHAR_n, CHAR_g, CHAR_l, CHAR_e, CHAR_s, CHAR_e};
WORD aMENUChinese[11] = {6, CHAR_C, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aMENUJapanese[11] = {10, CHAR_G, CHAR_i, CHAR_a, CHAR_p, CHAR_p, CHAR_o, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aMENUKorean[11] = {7, CHAR_C, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_o};
WORD aMENUFrench[12] = {8, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_c, CHAR_e, CHAR_s, CHAR_e};
WORD aMENUGerman[10] = {7, CHAR_T, CHAR_e, CHAR_d, CHAR_e, CHAR_s, CHAR_c, CHAR_o};
WORD aMENUItalian[12] = {8, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n, CHAR_o};
WORD aMENUSpanish[11] = {8, CHAR_S, CHAR_p, CHAR_a, CHAR_g, CHAR_n, CHAR_o, CHAR_l, CHAR_o};
WORD aMENUPortuguese[14] = {10, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_o, CHAR_g, CHAR_h, CHAR_e, CHAR_s, CHAR_e};
WORD aMENUDutch[12] = {8, CHAR_O, CHAR_l, CHAR_a, CHAR_n, CHAR_d, CHAR_e, CHAR_s, CHAR_e};
WORD aMENUPolish[9] = {7, CHAR_P, CHAR_o, CHAR_l, CHAR_a, CHAR_c, CHAR_c, CHAR_o};
WORD aMENUUIStyle[7] = {5, CHAR_S, CHAR_t, CHAR_i, CHAR_l, CHAR_e};
WORD aMENUUIStyle1[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_1};
WORD aMENUUIStyle2[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_2};
WORD aMENUUIStyle3[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_3};
WORD aMENUUIStyle4[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_4};
WORD aMENUDefault[18] = {11, CHAR_P, CHAR_r, CHAR_e, CHAR_d, CHAR_e, CHAR_f, CHAR_i, CHAR_n, CHAR_i, CHAR_t, CHAR_o};
WORD aMENURead[8] = {7, CHAR_L, CHAR_e, CHAR_t, CHAR_t, CHAR_u, CHAR_r, CHAR_a};
WORD aMENUIconExit[6] = {4, CHAR_E, CHAR_s, CHAR_c, CHAR_i};
WORD aMENUHelpSelect[10] = {9, CHAR_SPACE, CHAR_COLON, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t};
WORD aMENUHelpConfirm[11] = {9, CHAR_COLON, CHAR_SPACE, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_r, CHAR_m};
WORD aMENUHelpExit[18] = {17, CHAR_E, CHAR_x, CHAR_i, CHAR_t, CHAR_COLON, CHAR_SPACE, CHAR_S, CHAR_a, CHAR_v, CHAR_e, CHAR_SPACE, CHAR_AMPERSAND, CHAR_SPACE, CHAR_E, CHAR_x, CHAR_i, CHAR_t};
#endif //#ifdef IMAGE_FRAME_MENU_MAIN_FILE									
#ifdef SUPPORT_POWERON_MENU									
WORD aPOWERONIconPhoto[8] = {4, CHAR_F, CHAR_o, CHAR_t, CHAR_o};
WORD aPOWERONIconMusic[7] = {5, CHAR_M, CHAR_u, CHAR_s, CHAR_FULL_STOP, CHAR_SPACE};
WORD aPOWERONIconPhotoMusic[15] = {11, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_PLUS_SIGN, CHAR_M, CHAR_u, CHAR_s, CHAR_FULL_STOP, CHAR_SPACE, CHAR_SPACE};
WORD aPOWERONIconMovie[10] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aPOWERONIconSetup[14] = {8, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_g, CHAR_FULL_STOP, CHAR_SPACE};
WORD aPOWERONIconCalendar[11] = {8, CHAR_C, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_FULL_STOP, CHAR_SPACE};
WORD aPOWERONIconClock[9] = {8, CHAR_O, CHAR_r, CHAR_o, CHAR_l, CHAR_FULL_STOP, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE};
WORD aPOWERONIconAlarm[7] = {5, CHAR_A, CHAR_l, CHAR_l, CHAR_FULL_STOP, CHAR_SPACE};
WORD aPOWERONIconAutoPower[14] = {12, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_a, CHAR_c, CHAR_c, CHAR_FULL_STOP, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE};
WORD aPOWERONIconEdit[11] = {8, CHAR_M, CHAR_o, CHAR_d, CHAR_i, CHAR_f, CHAR_i, CHAR_c, CHAR_a};
WORD aPOWERONIconFavorite[10] = {7, CHAR_P, CHAR_r, CHAR_e, CHAR_f, CHAR_e, CHAR_r, CHAR_FULL_STOP};
WORD aPOWERONIconRadio[6] = {5, CHAR_R, CHAR_a, CHAR_d, CHAR_i, CHAR_o};
WORD aWeeklyDay_Mon[4] = {3, CHAR_L, CHAR_u, CHAR_n};
WORD aWeeklyDay_Tue[4] = {3, CHAR_M, CHAR_a, CHAR_r};
WORD aWeeklyDay_Wed[4] = {2, CHAR_M, CHAR_e};
WORD aWeeklyDay_Thu[4] = {3, CHAR_G, CHAR_i, CHAR_o};
WORD aWeeklyDay_Fri[4] = {3, CHAR_V, CHAR_e, CHAR_n};
WORD aWeeklyDay_Sat[4] = {3, CHAR_S, CHAR_a, CHAR_b};
WORD aWeeklyDay_Sun[4] = {3, CHAR_D, CHAR_o, CHAR_m};
WORD aMonthJanuary[4] = {3, CHAR_G, CHAR_e, CHAR_n};
WORD aMonthFebruary[4] = {3, CHAR_F, CHAR_e, CHAR_b};
WORD aMonthMarch[4] = {3, CHAR_M, CHAR_a, CHAR_r};
WORD aMonthApril[4] = {3, CHAR_A, CHAR_p, CHAR_r};
WORD aMonthMay[4] = {3, CHAR_M, CHAR_a, CHAR_g};
WORD aMonthJune[4] = {3, CHAR_G, CHAR_i, CHAR_u};
WORD aMonthJuly[4] = {3, CHAR_L, CHAR_u, CHAR_g};
WORD aMonthAugust[4] = {3, CHAR_A, CHAR_g, CHAR_o};
WORD aMonthSeptember[4] = {3, CHAR_S, CHAR_e, CHAR_t};
WORD aMonthOctober[4] = {3, CHAR_O, CHAR_t, CHAR_t};
WORD aMonthNovember[4] = {3, CHAR_N, CHAR_o, CHAR_v};
WORD aMonthDecember[4] = {3, CHAR_D, CHAR_i, CHAR_c};
WORD aWeeklyMonday[14] = {6, CHAR_L, CHAR_u, CHAR_n, CHAR_e, CHAR_d, CHAR_i4};
WORD aWeeklyTuesday[12] = {7, CHAR_M, CHAR_a, CHAR_r, CHAR_t, CHAR_e, CHAR_d, CHAR_i4};
WORD aWeeklyWednesday[13] = {9, CHAR_M, CHAR_e, CHAR_r, CHAR_c, CHAR_o, CHAR_l, CHAR_e, CHAR_d, CHAR_i4};
WORD aWeeklyThursday[13] = {7, CHAR_G, CHAR_i, CHAR_o, CHAR_v, CHAR_e, CHAR_d, CHAR_i4};
WORD aWeeklyFriday[12] = {7, CHAR_V, CHAR_e, CHAR_n, CHAR_e, CHAR_r, CHAR_d, CHAR_i4};
WORD aWeeklySaturday[9] = {6, CHAR_S, CHAR_a, CHAR_b, CHAR_a, CHAR_t, CHAR_o};
WORD aWeeklySunday[10] = {8, CHAR_D, CHAR_o, CHAR_m, CHAR_e, CHAR_n, CHAR_i, CHAR_c, CHAR_a};
WORD aCLOCKClockSetting[22] = {21, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_i, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_o, CHAR_l, CHAR_o, CHAR_g, CHAR_i, CHAR_o};
WORD aCLOCKDisplay[13] = {10, CHAR_V, CHAR_i, CHAR_s, CHAR_u, CHAR_a, CHAR_l, CHAR_i, CHAR_z, CHAR_z, CHAR_a};
WORD aCLOCKSetTime[14] = {13, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_r, CHAR_e, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_a};
WORD aCLOCKExit[6] = {4, CHAR_E, CHAR_s, CHAR_c, CHAR_i};
WORD aCLOCKBigClock[9] = {8, CHAR_O, CHAR_r, CHAR_o, CHAR_l, CHAR_o, CHAR_g, CHAR_i, CHAR_o};
WORD aCLOCKClockCalendar[22] = {21, CHAR_O, CHAR_r, CHAR_o, CHAR_l, CHAR_o, CHAR_g, CHAR_i, CHAR_o, CHAR_SPACE, CHAR_e, CHAR_SPACE, CHAR_c, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_a, CHAR_r, CHAR_i, CHAR_o};
WORD aBUTTONOK[3] = {2, CHAR_O, CHAR_K};
WORD aBUTTONCancel[9] = {5, CHAR_C, CHAR_a, CHAR_n, CHAR_c, CHAR_FULL_STOP};
WORD aSETTIMESetTime[15] = {14, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_a, CHAR_r, CHAR_e, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_a};
WORD aALARMEnable[10] = {7, CHAR_A, CHAR_b, CHAR_i, CHAR_l, CHAR_i, CHAR_t, CHAR_a};
WORD aALARMAlarmSetting[21] = {20, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_i, CHAR_SPACE, CHAR_s, CHAR_v, CHAR_e, CHAR_g, CHAR_l, CHAR_i, CHAR_a};
WORD aALARMDlgTitle[8] = {7, CHAR_S, CHAR_v, CHAR_e, CHAR_g, CHAR_l, CHAR_i, CHAR_a};
WORD aALARMDlgContent[34] = {32, CHAR_P, CHAR_r, CHAR_e, CHAR_s, CHAR_s, CHAR_SPACE, CHAR_a, CHAR_n, CHAR_y, CHAR_SPACE, CHAR_k, CHAR_e, CHAR_y, CHAR_SPACE, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_s, CHAR_t, CHAR_o, CHAR_p, CHAR_SPACE, CHAR_t, CHAR_h, CHAR_e, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_a, CHAR_r, CHAR_m, CHAR_FULL_STOP};
WORD aAUTOPWRAutoPowerSetting[46] = {40, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_i, CHAR_SPACE, CHAR_a, CHAR_c, CHAR_c, CHAR_e, CHAR_n, CHAR_s, CHAR_i, CHAR_o, CHAR_n, CHAR_e, CHAR_SOLIDUS, CHAR_s, CHAR_p, CHAR_e, CHAR_g, CHAR_n, CHAR_i, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_t, CHAR_o};
WORD aAUTOPWRPowerOn[11] = {10, CHAR_A, CHAR_c, CHAR_c, CHAR_e, CHAR_n, CHAR_s, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aAUTOPWRPowerOff[12] = {11, CHAR_S, CHAR_p, CHAR_e, CHAR_g, CHAR_n, CHAR_i, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_o};
WORD aAUTOPWRPowerOnEnable[30] = {18, CHAR_A, CHAR_b, CHAR_i, CHAR_l, CHAR_i, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_a, CHAR_c, CHAR_c, CHAR_e, CHAR_n, CHAR_s, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aAUTOPWRPowerOffEnable[33] = {19, CHAR_A, CHAR_b, CHAR_i, CHAR_l, CHAR_i, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_s, CHAR_p, CHAR_e, CHAR_g, CHAR_n, CHAR_i, CHAR_m, CHAR_e, CHAR_n, CHAR_t, CHAR_o};
WORD aAUTOPWRSetTime[14] = {13, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_r, CHAR_e, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_a};
WORD aMMBtnSave[10] = {5, CHAR_S, CHAR_a, CHAR_l, CHAR_v, CHAR_a};
WORD aMMSaveFileDlgTitle[20] = {10, CHAR_S, CHAR_a, CHAR_l, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_f, CHAR_i, CHAR_l, CHAR_e};
WORD aMMSaveFileDlgContent[18] = {9, CHAR_A, CHAR_t, CHAR_t, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_e};
WORD aTHUMBButtonSort[10] = {6, CHAR_O, CHAR_r, CHAR_d, CHAR_i, CHAR_n, CHAR_a};
WORD aTHUMBButtonDeleteAll[11] = {10, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_SPACE, CHAR_t, CHAR_u, CHAR_t, CHAR_t, CHAR_o};
WORD aTHUMBTitleEditMode[18] = {17, CHAR_M, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_m, CHAR_o, CHAR_d, CHAR_i, CHAR_f, CHAR_i, CHAR_c, CHAR_a};
WORD aTHUMBTitleDeleteMode[19] = {16, CHAR_M, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_e, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a};
WORD aTHUMBHelpDeleteMode[14] = {7, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aTHUMBHelpSortMode[26] = {24, CHAR_M, CHAR_e, CHAR_t, CHAR_t, CHAR_i, CHAR_SPACE, CHAR_a, CHAR_SPACE, CHAR_f, CHAR_u, CHAR_o, CHAR_c, CHAR_o, CHAR_SPACE, CHAR_l, CHAR_APOSTROPHE, CHAR_i, CHAR_m, CHAR_m, CHAR_a, CHAR_g, CHAR_i, CHAR_n, CHAR_e};
WORD aTHUMBTitleSortMode[18] = {15, CHAR_M, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_d, CHAR_i, CHAR_n, CHAR_a};
WORD aEDITDialogTitle[27] = {17, CHAR_F, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_t, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_m, CHAR_o, CHAR_d, CHAR_i, CHAR_f, CHAR_i, CHAR_c, CHAR_a};
WORD aEDITDialogContent[38] = {29, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_a, CHAR_r, CHAR_e, CHAR_SPACE, CHAR_m, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_m, CHAR_o, CHAR_d, CHAR_i, CHAR_f, CHAR_i, CHAR_c, CHAR_a};
WORD aBUTTONCopy[9] = {5, CHAR_C, CHAR_o, CHAR_p, CHAR_i, CHAR_a};
WORD aBUTTONDelete[9] = {7, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a};
WORD aSETDATESetDate[17] = {12, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_t, CHAR_a};
#endif //#ifdef SUPPORT_POWERON_MENU									
WORD aMENUOn[5] = {2, CHAR_O, CHAR_n};
WORD aMENUOff[7] = {3, CHAR_O, CHAR_f, CHAR_f};
#ifdef MUSTEK_SETUP_MENU									
WORD aMENU5Min[7] = {6, CHAR_5, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU30Min[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU1Hour[8] = {5, CHAR_1, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_a};
WORD aMENU4Hours[9] = {5, CHAR_4, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_e};
WORD aMENU12Hours[10] = {6, CHAR_1, CHAR_2, CHAR_SPACE, CHAR_o, CHAR_r, CHAR_e};
WORD aMENU1Day[9] = {8, CHAR_1, CHAR_SPACE, CHAR_g, CHAR_i, CHAR_o, CHAR_r, CHAR_n, CHAR_o};
WORD aMENUTransition[14] = {11, CHAR_T, CHAR_r, CHAR_a, CHAR_n, CHAR_s, CHAR_i, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
WORD aMENUTVSystem[17] = {10, CHAR_S, CHAR_i, CHAR_s, CHAR_t, CHAR_e, CHAR_m, CHAR_a, CHAR_SPACE, CHAR_T, CHAR_V};
WORD aMENUFullScreen[15] = {14, CHAR_S, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_m, CHAR_o, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_o};
WORD aMENUReset[8] = {5, CHAR_R, CHAR_e, CHAR_s, CHAR_e, CHAR_t};
WORD aMENUConfirm[8] = {7, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_r, CHAR_m};
WORD aMENUExitSetup[30] = {29, CHAR_E, CHAR_s, CHAR_c, CHAR_i, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_SPACE, CHAR_m, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_I, CHAR_m, CHAR_p, CHAR_o, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
#endif //MUSTEK_SETUP_MENU											
WORD aBUTTONYes[4] = {2, CHAR_S, CHAR_i4};
WORD aBUTTONNo[5] = {2, CHAR_N, CHAR_o};
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW											
WORD aMMSaveDlgTitle[31] = {30, CHAR_F, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_t, CHAR_r, CHAR_a, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_a, CHAR_l, CHAR_o, CHAR_g, CHAR_o, CHAR_SPACE, CHAR_s, CHAR_a, CHAR_l, CHAR_v, CHAR_a, CHAR_SPACE, CHAR_f, CHAR_o, CHAR_t, CHAR_o};
WORD aMMDeleteDlgTitle[31] = {17, CHAR_F, CHAR_i, CHAR_n, CHAR_FULL_STOP, CHAR_SPACE, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a, CHAR_SPACE, CHAR_f, CHAR_o, CHAR_t, CHAR_o};
WORD aMMSaveDlgContent[14] = {8, CHAR_S, CHAR_a, CHAR_l, CHAR_v, CHAR_a, CHAR_r, CHAR_e, CHAR_QUESTION_MARK};
WORD aMMDeleteDlgContent[13] = {10, CHAR_E, CHAR_l, CHAR_i, CHAR_m, CHAR_i, CHAR_n, CHAR_a, CHAR_r, CHAR_e, CHAR_QUESTION_MARK};
#endif //#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW									
#ifdef SUPPORT_CHOOSE_MEDIA									
WORD aMENUMedia[11] = {10, CHAR_M, CHAR_u, CHAR_l, CHAR_t, CHAR_i, CHAR_m, CHAR_e, CHAR_d, CHAR_i, CHAR_a};
#endif //SUPPORT_CHOOSE_MEDIA									
#ifdef SUPPORT_PHOTO_BROWSE_MODE									
WORD aNDBrowseMode[26] = {25, CHAR_M, CHAR_o, CHAR_d, CHAR_a, CHAR_l, CHAR_i, CHAR_t, CHAR_a1, CHAR_SPACE, CHAR_d, CHAR_i, CHAR_SPACE, CHAR_c, CHAR_o, CHAR_n, CHAR_s, CHAR_u, CHAR_l, CHAR_t, CHAR_a, CHAR_z, CHAR_i, CHAR_o, CHAR_n, CHAR_e};
#endif //SUPPORT_PHOTO_BROWSE_MODE									
#ifdef SUPPORT_TFT_DIGITAL_MODE									
WORD aMENUAVOutput[13] = {9, CHAR_O, CHAR_u, CHAR_t, CHAR_p, CHAR_u, CHAR_t, CHAR_SPACE, CHAR_A, CHAR_V};
#endif //SUPPORT_TFT_DIGITAL_MODE									
WORD aMENUPanel[9] = {8, CHAR_P, CHAR_a, CHAR_n, CHAR_n, CHAR_e, CHAR_l, CHAR_l, CHAR_o};
#ifdef SUPPORT_RADIO_TUNER									
WORD aRADIORecord[9] = {8, CHAR_R, CHAR_e, CHAR_g, CHAR_i, CHAR_s, CHAR_t, CHAR_r, CHAR_a};
WORD aRADIOScan[9] = {7, CHAR_R, CHAR_i, CHAR_c, CHAR_e, CHAR_r, CHAR_c, CHAR_a};
WORD aRADIOStationsFound[17] = {14, CHAR_s, CHAR_t, CHAR_a, CHAR_z, CHAR_FULL_STOP, CHAR_SPACE, CHAR_t, CHAR_r, CHAR_o, CHAR_v, CHAR_a, CHAR_t, CHAR_e, CHAR_FULL_STOP};
#endif //SUPPORT_RADIO_TUNER									
#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									
WORD aCODEWesternEuropean[17] = {11, CHAR_E, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_a, CHAR_SPACE, CHAR_o, CHAR_c, CHAR_c, CHAR_FULL_STOP};
#endif   // SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									
#ifdef SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									
WORD aCODECentralEuropean[19] = {13, CHAR_E, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_a, CHAR_SPACE, CHAR_c, CHAR_e, CHAR_n, CHAR_t, CHAR_r, CHAR_FULL_STOP};
#endif   // SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									
#ifdef SUPPORT_CHAR_ENCODING_GREEK									
WORD aCODEGreek[11] = {5, CHAR_G, CHAR_r, CHAR_e, CHAR_c, CHAR_o};
#endif   // SUPPORT_CHAR_ENCODING_GREEK									
#ifdef SUPPORT_CHAR_ENCODING_HEBREW									
WORD aCODEHebrew[10] = {7, CHAR_E, CHAR_b, CHAR_r, CHAR_a, CHAR_i, CHAR_c, CHAR_o};
#endif   // SUPPORT_CHAR_ENCODING_HEBREW									
#ifdef SUPPORT_CHAR_ENCODING_TURKISH									
WORD aCODETurkish[9] = {5, CHAR_T, CHAR_u, CHAR_r, CHAR_c, CHAR_o};
#endif   // SUPPORT_CHAR_ENCODING_TURKISH									
#ifdef SUPPORT_CHAR_ENCODING_CP1250									
WORD aCODECP1250[8] = {7, CHAR_C, CHAR_P, CHAR_SPACE, CHAR_1, CHAR_2, CHAR_5, CHAR_0};
#endif   // SUPPORT_CHAR_ENCODING_CP1250									
#ifdef SUPPORT_CHAR_ENCODING_CYRILLIC									
WORD aCODECyrillic[11] = {9, CHAR_C, CHAR_i, CHAR_r, CHAR_i, CHAR_l, CHAR_l, CHAR_i, CHAR_c, CHAR_o};
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
