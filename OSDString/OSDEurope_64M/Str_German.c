#include "stringdec.h"
#ifdef OSDDSPLY_MAIN_FILE									
WORD aDSPLYLanguageJapanese[11] = {9, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguageEnglish[10] = {8, CHAR_E, CHAR_n, CHAR_g, CHAR_l, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguageFrench[12] = {11, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_z, CHAR_o3, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguageGerman[10] = {7, CHAR_D, CHAR_e, CHAR_u, CHAR_t, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguageRussian[9] = {8, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguageChinese[11] = {10, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguageKorean[11] = {10, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aDSPLYLanguageThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aDSPLYLanguageItalian[12] = {11, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguageSpanish[11] = {8, CHAR_S, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguagePortuguese[14] = {13, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_i, CHAR_e, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aDSPLYLanguageOthers[9] = {8, CHAR_S, CHAR_o, CHAR_n, CHAR_s, CHAR_t, CHAR_i, CHAR_g, CHAR_e};
WORD aDSPLYOff[10] = {3, CHAR_A, CHAR_u, CHAR_s};
#ifndef NO_DVD_DISPLAY_UI									
WORD aDSPLYTitleElapsed[17] = {13, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_r, CHAR_FULL_STOP};
WORD aDSPLYTitleRemain[16] = {12, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_b, CHAR_l, CHAR_FULL_STOP};
WORD aDSPLYChapterElapsed[16] = {15, CHAR_K, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_r, CHAR_FULL_STOP};
WORD aDSPLYChapterRemain[15] = {14, CHAR_K, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_b, CHAR_l, CHAR_FULL_STOP};
#endif //NO_DVD_DISPLAY_UI									
WORD aDSPLYSingleElapsed[17] = {13, CHAR_E, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_r, CHAR_FULL_STOP};
WORD aDSPLYSingleRemain[16] = {12, CHAR_E, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_b, CHAR_l, CHAR_FULL_STOP};
WORD aDSPLYTotalElapsed[18] = {13, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_r, CHAR_FULL_STOP};
WORD aDSPLYTotalRemain[17] = {12, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_v, CHAR_e, CHAR_r, CHAR_b, CHAR_l, CHAR_FULL_STOP};
WORD aDSPLYRootMenu[16] = {9, CHAR_R, CHAR_o, CHAR_o, CHAR_t, CHAR_HYPHEN_MINUS, CHAR_M, CHAR_e, CHAR_n, CHAR_u2};
WORD aDSPLYTitleMenu[12] = {10, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_HYPHEN_MINUS, CHAR_M, CHAR_e, CHAR_n, CHAR_u2};
#ifdef SUPPORT_NAVIGATOR									
WORD aDSPLYRepeatTitle[20] = {17, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_w, CHAR_i, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_h, CHAR_o, CHAR_l, CHAR_e, CHAR_n};
WORD aDSPLYRepeatChapter[22] = {20, CHAR_K, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_SPACE, CHAR_w, CHAR_i, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_h, CHAR_o, CHAR_l, CHAR_d, CHAR_e, CHAR_n};
WORD aDSPLYRepeatDisc[19] = {16, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_SPACE, CHAR_w, CHAR_i, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_h, CHAR_o, CHAR_l, CHAR_e, CHAR_n};
WORD aDSPLYRepeatOff[22] = {16, CHAR_W, CHAR_i, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_h, CHAR_o, CHAR_l, CHAR_u, CHAR_n, CHAR_g, CHAR_SPACE, CHAR_A, CHAR_U, CHAR_S};
#endif //SUPPORT_NAVIGATOR									
WORD aDSPLYpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aDSPLYInterlace[15] = {9, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_l, CHAR_a, CHAR_c, CHAR_e};
WORD aDSPLYResolution[15] = {10, CHAR_A, CHAR_u, CHAR_f, CHAR_l, CHAR_o3, CHAR_s, CHAR_u, CHAR_n, CHAR_g, CHAR_COLON};
WORD aDSPLYDate[7] = {6, CHAR_D, CHAR_a, CHAR_t, CHAR_u, CHAR_m, CHAR_COLON};
#ifdef SUPPORT_NAVIGATOR									
WORD aNGTitle[8] = {6, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e, CHAR_COLON};
WORD aNGChapter[11] = {8, CHAR_K, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_COLON};
WORD aNGAngle[13] = {7, CHAR_W, CHAR_i, CHAR_n, CHAR_k, CHAR_e, CHAR_l, CHAR_COLON};
WORD aNGAudio[7] = {6, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o, CHAR_COLON};
WORD aNGSubtitle1[13] = {9, CHAR_U, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_t, CHAR_FULL_STOP, CHAR_1, CHAR_COLON};
WORD aNGSubtitle2[13] = {9, CHAR_U, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_t, CHAR_FULL_STOP, CHAR_2, CHAR_COLON};
#endif //SUPPORT_NAVIGATOR									
#endif //OSDDSPLY_MAIN_FILE									
#ifdef OSDND_MAIN_FILE									
WORD aNDNoDisc[14] = {10, CHAR_K, CHAR_e, CHAR_i, CHAR_n, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDNoUSBDevice[14] = {13, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_U, CHAR_S, CHAR_B, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_v, CHAR_i, CHAR_c, CHAR_e};
WORD aNDOpen[7] = {6, CHAR_O1, CHAR_f, CHAR_f, CHAR_n, CHAR_e, CHAR_n};
WORD aNDClose[10] = {9, CHAR_S, CHAR_c, CHAR_h, CHAR_l, CHAR_i, CHAR_e, CHAR_B1, CHAR_e, CHAR_n};
WORD aNDReturn[8] = {6, CHAR_Z, CHAR_u, CHAR_r, CHAR_u2, CHAR_c, CHAR_k};
WORD aNDFreeze[15] = {9, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_b, CHAR_i, CHAR_l, CHAR_d};
WORD aNDRandomOn[12] = {11, CHAR_Z, CHAR_u, CHAR_f, CHAR_a, CHAR_l, CHAR_l, CHAR_s, CHAR_w, CHAR_i, CHAR_e, CHAR_d};
WORD aNDRandomOff[12] = {10, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l, CHAR_w, CHAR_i, CHAR_e, CHAR_d};
WORD aNDResume[11] = {10, CHAR_F, CHAR_o, CHAR_r, CHAR_t, CHAR_s, CHAR_e, CHAR_t, CHAR_z, CHAR_e, CHAR_n};
WORD aNDRootMenu[16] = {9, CHAR_R, CHAR_o, CHAR_o, CHAR_t, CHAR_HYPHEN_MINUS, CHAR_M, CHAR_e, CHAR_n, CHAR_u2};
WORD aNDTitleMenu[12] = {10, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_HYPHEN_MINUS, CHAR_M, CHAR_e, CHAR_n, CHAR_u2};
WORD aNDRotateRight[26] = {19, CHAR_9, CHAR_0, CHAR_N_DEGREE, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s, CHAR_SPACE, CHAR_r, CHAR_o, CHAR_t, CHAR_i, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aNDRotateLeft[26] = {18, CHAR_9, CHAR_0, CHAR_N_DEGREE, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_k, CHAR_s, CHAR_SPACE, CHAR_r, CHAR_o, CHAR_t, CHAR_i, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aNDFlipHorizontally[26] = {19, CHAR_H, CHAR_o, CHAR_r, CHAR_i, CHAR_z, CHAR_o, CHAR_n, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_s, CHAR_p, CHAR_i, CHAR_e, CHAR_g, CHAR_e, CHAR_l, CHAR_n};
WORD aNDFlipVertically[25] = {17, CHAR_V, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_k, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_s, CHAR_p, CHAR_i, CHAR_e, CHAR_g, CHAR_e, CHAR_l, CHAR_n};
WORD aNDpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aNDInterlace[15] = {9, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_l, CHAR_a, CHAR_c, CHAR_e};
WORD aNDRepeatNone[10] = {3, CHAR_A, CHAR_u, CHAR_s};
WORD aNDRepeatTrack[8] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aNDRepeatDisc[7] = {4, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDRepeatAll[10] = {5, CHAR_A, CHAR_l, CHAR_l, CHAR_e, CHAR_s};
WORD aNDRepeatTitle[7] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aNDRepeatChapter[10] = {7, CHAR_K, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aNDRepeatFolder[9] = {6, CHAR_o, CHAR_r, CHAR_d, CHAR_n, CHAR_e, CHAR_r};
WORD aNDRepeatA[15] = {13, CHAR_A, CHAR_SPACE, CHAR_w, CHAR_i, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_h, CHAR_o, CHAR_l, CHAR_e, CHAR_n};
WORD aNDRepeatAB[17] = {15, CHAR_A, CHAR_HYPHEN_MINUS, CHAR_B, CHAR_SPACE, CHAR_w, CHAR_i, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_h, CHAR_o, CHAR_l, CHAR_e, CHAR_n};
WORD aNDRepeatOff[22] = {16, CHAR_W, CHAR_i, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_h, CHAR_o, CHAR_l, CHAR_u, CHAR_n, CHAR_g, CHAR_SPACE, CHAR_A, CHAR_U, CHAR_S};
#ifndef NO_DISC_MODE									
WORD aNDNavigateOn[25] = {14, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_a, CHAR_t, CHAR_i, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_E, CHAR_i, CHAR_n};
WORD aNDNavigateOff[28] = {14, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_a, CHAR_t, CHAR_i, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_A, CHAR_u, CHAR_s};
WORD aNDPBCOn[13] = {7, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_e, CHAR_i, CHAR_n};
WORD aNDPBCOff[14] = {7, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_s};
#endif //#ifndef NO_DISC_MODE									
WORD aNDZoom1[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_1};
WORD aNDZoom2[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_2};
WORD aNDZoom4[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_4};
WORD aNDMoveLeft[24] = {10, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_l, CHAR_i, CHAR_n, CHAR_k, CHAR_s};
WORD aNDMoveRight[24] = {11, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_t, CHAR_s};
WORD aNDMoveUp[23] = {9, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_o, CHAR_b, CHAR_e, CHAR_n};
WORD aNDMoveDown[23] = {10, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_u, CHAR_n, CHAR_t, CHAR_e, CHAR_n};
WORD aNDVocalOn[15] = {10, CHAR_S, CHAR_t, CHAR_i, CHAR_m, CHAR_m, CHAR_e, CHAR_SPACE, CHAR_E, CHAR_i, CHAR_n};
WORD aNDVocalOff[16] = {10, CHAR_S, CHAR_t, CHAR_i, CHAR_m, CHAR_m, CHAR_e, CHAR_SPACE, CHAR_A, CHAR_u, CHAR_s};
WORD aNDVocalLL[15] = {9, CHAR_S, CHAR_t, CHAR_i, CHAR_m, CHAR_m, CHAR_e, CHAR_SPACE, CHAR_L, CHAR_L};
WORD aNDVocalRR[13] = {9, CHAR_S, CHAR_t, CHAR_i, CHAR_m, CHAR_m, CHAR_e, CHAR_SPACE, CHAR_R, CHAR_R};
WORD aNDKaraokeOn[17] = {11, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_E, CHAR_i, CHAR_n};
WORD aNDKaraokeOff[18] = {11, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_A, CHAR_u, CHAR_s};
WORD aNDJPEGEffectNone[12] = {11, CHAR_K, CHAR_e, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_E, CHAR_f, CHAR_f, CHAR_e, CHAR_k, CHAR_t};
WORD aNDJPEGEffectCoverUp[27] = {19, CHAR_V, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_o, CHAR_b, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_u2, CHAR_b, CHAR_e, CHAR_r, CHAR_d, CHAR_e, CHAR_c, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectCoverDown[26] = {20, CHAR_V, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_u, CHAR_n, CHAR_t, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_u2, CHAR_b, CHAR_e, CHAR_r, CHAR_d, CHAR_e, CHAR_c, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectCoverLeft[20] = {10, CHAR_U4, CHAR_b, CHAR_e, CHAR_r, CHAR_d, CHAR_e, CHAR_c, CHAR_FULL_STOP, CHAR_SPACE, CHAR_L};
WORD aNDJPEGEffectCoverRight[20] = {10, CHAR_U4, CHAR_b, CHAR_e, CHAR_r, CHAR_d, CHAR_e, CHAR_c, CHAR_FULL_STOP, CHAR_SPACE, CHAR_R};
WORD aNDJPEGEffectUncoverUp[23] = {19, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_o, CHAR_b, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_f, CHAR_d, CHAR_e, CHAR_c, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectUncoverDown[24] = {20, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_u, CHAR_n, CHAR_t, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_a, CHAR_u, CHAR_f, CHAR_d, CHAR_e, CHAR_c, CHAR_k, CHAR_e, CHAR_n};
WORD aNDJPEGEffectUncoverLeft[21] = {11, CHAR_A, CHAR_u, CHAR_f, CHAR_d, CHAR_e, CHAR_c, CHAR_k, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_L};
WORD aNDJPEGEffectUncoverRight[22] = {11, CHAR_A, CHAR_u, CHAR_f, CHAR_d, CHAR_e, CHAR_c, CHAR_k, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_R};
WORD aNDJPEGEffectWipeUp[22] = {15, CHAR_V, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_o, CHAR_b, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_r, CHAR_o, CHAR_l, CHAR_l, CHAR_e, CHAR_n};
WORD aNDJPEGEffectWipeDown[21] = {16, CHAR_V, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_u, CHAR_n, CHAR_t, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_r, CHAR_o, CHAR_l, CHAR_l, CHAR_e, CHAR_n};
WORD aNDJPEGEffectWipeLeft[17] = {9, CHAR_W, CHAR_i, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_L};
WORD aNDJPEGEffectWipeRight[18] = {9, CHAR_W, CHAR_i, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_R};
WORD aNDJPEGEffectPushUp[14] = {7, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_o, CHAR_FULL_STOP};
WORD aNDJPEGEffectPushDown[13] = {7, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_u, CHAR_FULL_STOP};
WORD aNDJPEGEffectPushLeft[17] = {7, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_L, CHAR_FULL_STOP};
WORD aNDJPEGEffectPushRight[18] = {7, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_R, CHAR_FULL_STOP};
WORD aNDJPEGEffectFadeThroughBlack[26] = {20, CHAR_U4, CHAR_b, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_S, CHAR_c, CHAR_h, CHAR_w, CHAR_a, CHAR_r, CHAR_z, CHAR_SPACE, CHAR_b, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_n};
WORD aNDJPEGEffectRandom[16] = {11, CHAR_Z, CHAR_u, CHAR_f, CHAR_a, CHAR_l, CHAR_l, CHAR_s, CHAR_w, CHAR_i, CHAR_e, CHAR_d};
#ifndef NO_DISC_MODE									
WORD aNDEncrypt[17] = {13, CHAR_I, CHAR_l, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDErrRegionCode[27] = {14, CHAR_F, CHAR_a, CHAR_l, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_SPACE, CHAR_R, CHAR_e, CHAR_g, CHAR_i, CHAR_o, CHAR_n};
WORD aNDWrongDisc[19] = {12, CHAR_F, CHAR_a, CHAR_l, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDDamageDisc[19] = {16, CHAR_B, CHAR_e, CHAR_s, CHAR_c, CHAR_h, CHAR_a5, CHAR_d, CHAR_i, CHAR_g, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
#endif //#ifndef NO_DISC_MODE									
WORD aNDNoPlayableFile[25] = {16, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y, CHAR_a, CHAR_b, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_F, CHAR_i, CHAR_l, CHAR_e};
WORD aNDDisc[7] = {4, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDTrack[8] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aNDTitle[7] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e};
WORD aNDChapter[10] = {7, CHAR_K, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aNDLR[4] = {3, CHAR_L, CHAR_SOLIDUS, CHAR_R};
WORD aNDLL[4] = {3, CHAR_L, CHAR_SOLIDUS, CHAR_L};
WORD aNDRR[4] = {3, CHAR_R, CHAR_SOLIDUS, CHAR_R};
WORD aNDVolume[11] = {10, CHAR_L, CHAR_a, CHAR_u, CHAR_t, CHAR_s, CHAR_t, CHAR_a5, CHAR_r, CHAR_k, CHAR_e};
WORD aNDTone[9] = {3, CHAR_T, CHAR_o, CHAR_n};
WORD aNDEcho[5] = {4, CHAR_E, CHAR_c, CHAR_h, CHAR_o};
WORD aNDNoSurround[19] = {13, CHAR_K, CHAR_e, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_S, CHAR_u, CHAR_r, CHAR_r, CHAR_o, CHAR_u, CHAR_n, CHAR_d};
WORD aNDLiveSurround[10] = {4, CHAR_L, CHAR_i, CHAR_v, CHAR_e};
WORD aNDTheaterSurround[8] = {7, CHAR_T, CHAR_h, CHAR_e, CHAR_a, CHAR_t, CHAR_e, CHAR_r};
WORD aNDStereoSurround[8] = {6, CHAR_S, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_o};
WORD aNDMovieSurround[9] = {4, CHAR_K, CHAR_i, CHAR_n, CHAR_o};
WORD aNDKaraSurround[8] = {7, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e};
WORD aNDLanguageJapanese[11] = {9, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguageEnglish[10] = {8, CHAR_E, CHAR_n, CHAR_g, CHAR_l, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguageFrench[12] = {11, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_z, CHAR_o3, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguageGerman[10] = {7, CHAR_D, CHAR_e, CHAR_u, CHAR_t, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguageRussian[9] = {8, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguageChinese[11] = {10, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguageKorean[11] = {10, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aNDLanguageThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aNDLanguageItalian[12] = {11, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguageSpanish[11] = {8, CHAR_S, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguagePortuguese[14] = {13, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_i, CHAR_e, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aNDLanguageOthers[9] = {8, CHAR_S, CHAR_o, CHAR_n, CHAR_s, CHAR_t, CHAR_i, CHAR_g, CHAR_e};
WORD aNDSubtitle[13] = {11, CHAR_U, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_COLON};
WORD aNDOff[10] = {3, CHAR_A, CHAR_u, CHAR_s};
WORD aNDWait[12] = {5, CHAR_L, CHAR_a, CHAR_d, CHAR_e, CHAR_n};
WORD aNDSelect[12] = {6, CHAR_W, CHAR_a5, CHAR_h, CHAR_l, CHAR_e, CHAR_n};
WORD aNDCardReader[29] = {12, CHAR_K, CHAR_a, CHAR_r, CHAR_t, CHAR_e, CHAR_n, CHAR_l, CHAR_e, CHAR_s, CHAR_e, CHAR_r, CHAR_SPACE};
#endif //OSDND_MAIN_FILE									
#ifdef OSDMM_MAIN_FILE									
WORD aMMProgramModeTitle[24] = {13, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aMMProgramListModeTitle[21] = {13, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_l, CHAR_i, CHAR_s, CHAR_t, CHAR_e};
WORD aMMTotalFiles[25] = {16, CHAR_G, CHAR_e, CHAR_s, CHAR_a, CHAR_m, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_i, CHAR_e, CHAR_n, CHAR_COLON};
WORD aMMFileNumber[20] = {12, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_i, CHAR_a, CHAR_n, CHAR_z, CHAR_a, CHAR_h, CHAR_l, CHAR_COLON};
WORD aMMDlgFileTypeTitle[14] = {9, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_i, CHAR_t, CHAR_y, CHAR_p, CHAR_COLON};
WORD aMMDlgSortFileTitle[16] = {10, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_i, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_COLON};
WORD aMMDlgSortFileMethodTitle[9] = {8, CHAR_M, CHAR_e, CHAR_t, CHAR_h, CHAR_o, CHAR_d, CHAR_e, CHAR_COLON};
WORD aMMDlgSortFileSequenceTitle[12] = {11, CHAR_R, CHAR_e, CHAR_i, CHAR_h, CHAR_e, CHAR_n, CHAR_f, CHAR_o, CHAR_l, CHAR_FULL_STOP, CHAR_COLON};
WORD aMMDlgSequenceAscendingItem[10] = {9, CHAR_A, CHAR_u, CHAR_f, CHAR_s, CHAR_t, CHAR_e, CHAR_i, CHAR_g, CHAR_FULL_STOP};
WORD aMMDlgSequenceDescendingItem[12] = {8, CHAR_A, CHAR_b, CHAR_s, CHAR_t, CHAR_e, CHAR_i, CHAR_g, CHAR_FULL_STOP};
WORD aMMDlgMethodTypeItem[11] = {7, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_T, CHAR_y, CHAR_p};
WORD aMMDlgMethodNameItem[11] = {7, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_N, CHAR_a, CHAR_m};
WORD aMMDlgMethodTimeItem[10] = {9, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_Z, CHAR_e, CHAR_i, CHAR_t};
WORD aMMHelpProgram[16] = {9, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_FULL_STOP};
WORD aMMHelpProgramList[14] = {7, CHAR_L, CHAR_o3, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_n};
WORD aMMBtnFileType[10] = {8, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_i, CHAR_t, CHAR_y, CHAR_p};
WORD aMMBtnSortFile[12] = {9, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_i, CHAR_S, CHAR_o, CHAR_r, CHAR_t};
WORD aMMBtnList[7] = {5, CHAR_L, CHAR_i, CHAR_s, CHAR_t, CHAR_e};
WORD aMMBtnPlay[9] = {4, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aMMBtnDeleteAll[11] = {10, CHAR_A, CHAR_l, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_l, CHAR_o3, CHAR_s, CHAR_c, CHAR_h};
WORD aMMBtnReturn[8] = {6, CHAR_Z, CHAR_u, CHAR_r, CHAR_u2, CHAR_c, CHAR_k};
WORD aMMBtnExit[8] = {7, CHAR_B, CHAR_e, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_n};
#endif //OSDMM_MAIN_FILE									
#ifdef THUMB_MAIN_FILE									
WORD aTHUMBPage[8] = {6, CHAR_S, CHAR_e, CHAR_i, CHAR_t, CHAR_e, CHAR_COLON};
WORD aTHUMBTitleGeneralMode[19] = {13, CHAR_M, CHAR_i, CHAR_n, CHAR_i, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aTHUMBTitleRotateMode[19] = {14, CHAR_R, CHAR_o, CHAR_t, CHAR_a, CHAR_t, CHAR_i, CHAR_o, CHAR_n, CHAR_s, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aTHUMBTitleProgramMode[24] = {13, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aTHUMBTitleProgramList[21] = {13, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_l, CHAR_i, CHAR_s, CHAR_t, CHAR_e};
WORD aTHUMBHelpRotateMode[17] = {6, CHAR_D, CHAR_r, CHAR_e, CHAR_h, CHAR_e, CHAR_n};
WORD aTHUMBHelpProgramMode[16] = {9, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_m, CHAR_FULL_STOP};
WORD aTHUMBHelpProgramList[14] = {7, CHAR_L, CHAR_o3, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_n};
WORD aTHUMBButtonRotate[8] = {6, CHAR_D, CHAR_r, CHAR_e, CHAR_h, CHAR_e, CHAR_n};
WORD aTHUMBButtonSlideShow[11] = {7, CHAR_D, CHAR_i, CHAR_a, CHAR_s, CHAR_h, CHAR_o, CHAR_w};
WORD aTHUMBButtonReset[8] = {5, CHAR_R, CHAR_e, CHAR_s, CHAR_e, CHAR_t};
WORD aTHUMBButtonReturn[8] = {5, CHAR_Z, CHAR_u, CHAR_r, CHAR_u2, CHAR_c};
WORD aTHUMBButtonProgramList[7] = {5, CHAR_L, CHAR_i, CHAR_s, CHAR_t, CHAR_e};
#endif //THUMB_MAIN_FILE									
#ifdef OSDDG_MAIN_FILE									
WORD aDGDlgBtnTrackDigest[21] = {12, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_I, CHAR_n, CHAR_t, CHAR_r, CHAR_o, CHAR_FULL_STOP};
WORD aDGDlgBtnDiscInterval[21] = {13, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l, CHAR_l};
WORD aDGPage[8] = {6, CHAR_S, CHAR_e, CHAR_i, CHAR_t, CHAR_e, CHAR_COLON};
WORD aDGTitleTrackDigest[21] = {12, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_I, CHAR_n, CHAR_t, CHAR_r, CHAR_o, CHAR_FULL_STOP};
WORD aDGTitleDiscInterval[21] = {13, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l, CHAR_l};
WORD aDGInfoTrack[9] = {6, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l, CHAR_COLON};
WORD aDGBtnMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u2};
WORD aDGBtnExit[8] = {7, CHAR_B, CHAR_e, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_n};
#endif //OSDDG_MAIN_FILE									
#ifdef OSDPRGM_MAIN_FILE									
WORD aProgram[12] = {8, CHAR_P, CHAR_R, CHAR_O, CHAR_G, CHAR_R, CHAR_A, CHAR_M, CHAR_M};
WORD aTitle[7] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e};
WORD aChapter[10] = {7, CHAR_K, CHAR_a, CHAR_p, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aTrack[8] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aSelect[12] = {6, CHAR_W, CHAR_a5, CHAR_h, CHAR_l, CHAR_e, CHAR_n};
WORD aPlay[11] = {10, CHAR_W, CHAR_i, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_g, CHAR_a, CHAR_b, CHAR_e};
WORD aClear[8] = {5, CHAR_L, CHAR_o3, CHAR_s, CHAR_c, CHAR_h};
#endif //OSDPRGM_MAIN_FILE									
#ifdef OSDBMARK_MAIN_FILE									
WORD aBookmark[11] = {8, CHAR_L, CHAR_e, CHAR_s, CHAR_e, CHAR_z, CHAR_e, CHAR_i, CHAR_FULL_STOP};
WORD aTime[6] = {4, CHAR_Z, CHAR_e, CHAR_i, CHAR_t};
#endif //OSDBMARK_MAIN_FILE									
#ifdef SETUP_MAIN_FILE									
WORD aSetupSelect[12] = {6, CHAR_W, CHAR_a5, CHAR_h, CHAR_l, CHAR_e, CHAR_n};
#endif //SETUP_MAIN_FILE									
#ifdef MENU_MAIN_FILE									
WORD aMsgEnglish[10] = {8, CHAR_E, CHAR_n, CHAR_g, CHAR_l, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgChinese[11] = {10, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgJapanese[11] = {9, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgFrench[12] = {11, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_z, CHAR_o3, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgGerman[10] = {7, CHAR_D, CHAR_e, CHAR_u, CHAR_t, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgItalian[12] = {11, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgSpanish[11] = {8, CHAR_S, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgPortuguese[14] = {13, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_i, CHAR_e, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgDutch[12] = {11, CHAR_H, CHAR_o, CHAR_l, CHAR_l, CHAR_a5, CHAR_n, CHAR_d, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgPolish[9] = {8, CHAR_P, CHAR_o, CHAR_l, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aMsgThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aMsgRussian[9] = {8, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgKorean[11] = {10, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMsgCzech[6] = {5, CHAR_C, CHAR_z, CHAR_e, CHAR_c, CHAR_h};
WORD aMsgGreece[7] = {6, CHAR_G, CHAR_r, CHAR_e, CHAR_e, CHAR_c, CHAR_e};
WORD aMsgTurkey[7] = {6, CHAR_T, CHAR_u, CHAR_r, CHAR_k, CHAR_e, CHAR_y};
WORD aMsgOthers[9] = {8, CHAR_S, CHAR_o, CHAR_n, CHAR_s, CHAR_t, CHAR_i, CHAR_g, CHAR_e};
WORD aMsgOSD[4] = {3, CHAR_O, CHAR_S, CHAR_D};
WORD aMsgMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u2};
WORD aMsgAudio[6] = {5, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMsgSubtitle[12] = {10, CHAR_U, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_t, CHAR_e, CHAR_l};
WORD aMsgPS[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_P, CHAR_S};
WORD aMsgLB[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_L, CHAR_B};
WORD aMsgWide[5] = {4, CHAR_1, CHAR_6, CHAR_COLON, CHAR_9};
WORD aMsgOn[9] = {3, CHAR_E, CHAR_i, CHAR_n};
WORD aMsgOff[10] = {3, CHAR_A, CHAR_u, CHAR_s};
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
WORD aMsgBright[10] = {4, CHAR_H, CHAR_e, CHAR_l, CHAR_l};
WORD aMsgSoft[8] = {5, CHAR_S, CHAR_a, CHAR_n, CHAR_f, CHAR_t};
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
WORD aMsgTVType[14] = {6, CHAR_T, CHAR_V, CHAR_HYPHEN_MINUS, CHAR_T, CHAR_y, CHAR_p};
WORD aMsgScreenSaver[18] = {17, CHAR_B, CHAR_i, CHAR_l, CHAR_d, CHAR_s, CHAR_c, CHAR_h, CHAR_i, CHAR_r, CHAR_m, CHAR_s, CHAR_c, CHAR_h, CHAR_o, CHAR_n, CHAR_e, CHAR_r};
WORD aMsgTVSystem[17] = {9, CHAR_T, CHAR_V, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_y, CHAR_s, CHAR_t, CHAR_e, CHAR_m};
WORD aMsgVideoOutput[14] = {12, CHAR_V, CHAR_i, CHAR_d, CHAR_e, CHAR_o, CHAR_a, CHAR_u, CHAR_s, CHAR_g, CHAR_a, CHAR_b, CHAR_e};
WORD aMsgBrightness[11] = {10, CHAR_H, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_g, CHAR_k, CHAR_e, CHAR_i, CHAR_t};
WORD aMsgContrast[10] = {8, CHAR_K, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMsgChromPhase[4] = {3, CHAR_H, CHAR_u, CHAR_e};
WORD aMsgSPDIF_PCM[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_P, CHAR_C, CHAR_M};
WORD aMsgSPDIF_RAW[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_R, CHAR_A, CHAR_W};
WORD aMsg2CH[11] = {7, CHAR_2, CHAR_HYPHEN_MINUS, CHAR_K, CHAR_a, CHAR_n, CHAR_a, CHAR_l};
WORD aMsg51CH[13] = {9, CHAR_5, CHAR_FULL_STOP, CHAR_1, CHAR_HYPHEN_MINUS, CHAR_K, CHAR_a, CHAR_n, CHAR_a, CHAR_l};
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
WORD aMsgDigitalOut[18] = {17, CHAR_D, CHAR_i, CHAR_g, CHAR_i, CHAR_t, CHAR_a, CHAR_l, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_A, CHAR_u, CHAR_s, CHAR_g, CHAR_a, CHAR_n, CHAR_g};
WORD aMsgAnalogOut[18] = {16, CHAR_A, CHAR_n, CHAR_a, CHAR_l, CHAR_o, CHAR_g, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_A, CHAR_u, CHAR_s, CHAR_g, CHAR_a, CHAR_n, CHAR_g};
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
WORD aMsgCenter[10] = {5, CHAR_M, CHAR_i, CHAR_t, CHAR_t, CHAR_e};
WORD aMsgRear[11] = {6, CHAR_H, CHAR_i, CHAR_n, CHAR_t, CHAR_e, CHAR_r};
WORD aMsgSubwoofer[10] = {9, CHAR_S, CHAR_u, CHAR_b, CHAR_w, CHAR_o, CHAR_o, CHAR_f, CHAR_e, CHAR_r};
WORD aMsgSpeakerTest[19] = {16, CHAR_L, CHAR_a, CHAR_u, CHAR_t, CHAR_s, CHAR_p, CHAR_r, CHAR_e, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_t, CHAR_e, CHAR_s, CHAR_t};
WORD aMsgLevel1[2] = {1, CHAR_1};
WORD aMsgLevel2[2] = {1, CHAR_2};
WORD aMsgLevel3[2] = {1, CHAR_3};
WORD aMsgLevel4[2] = {1, CHAR_4};
WORD aMsgLevel5[2] = {1, CHAR_5};
WORD aMsgLevel6[2] = {1, CHAR_6};
WORD aMsgLevel7[2] = {1, CHAR_7};
WORD aMsgLevel8[2] = {1, CHAR_8};
WORD aMsgRead[8] = {5, CHAR_L, CHAR_e, CHAR_s, CHAR_e, CHAR_n};
WORD aMsgParental[20] = {15, CHAR_K, CHAR_i, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_s, CHAR_i, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_u, CHAR_n, CHAR_g};
WORD aMsgChangePassword[13] = {8, CHAR_K, CHAR_e, CHAR_n, CHAR_n, CHAR_w, CHAR_o, CHAR_r, CHAR_t};
WORD aMsgAngleMark[16] = {13, CHAR_W, CHAR_i, CHAR_n, CHAR_k, CHAR_e, CHAR_l, CHAR_z, CHAR_e, CHAR_i, CHAR_c, CHAR_h, CHAR_e, CHAR_n};
WORD aMsgUIStyle[7] = {4, CHAR_S, CHAR_t, CHAR_i, CHAR_l};
WORD aMsgUIStyle1[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_1};
WORD aMsgUIStyle2[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_2};
WORD aMsgUIStyle3[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_3};
WORD aMsgUIStyle4[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_4};
WORD aMsgSmartPlay[14] = {10, CHAR_S, CHAR_m, CHAR_a, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aMsgDivXVOD[9] = {8, CHAR_D, CHAR_i, CHAR_v, CHAR_X, CHAR_SPACE, CHAR_V, CHAR_O, CHAR_D};
WORD aMsgDefaultValue[18] = {8, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_r, CHAR_d};
WORD aMsgLanguageSetup[23] = {17, CHAR_S, CHAR_p, CHAR_r, CHAR_a, CHAR_c, CHAR_h, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_u, CHAR_n, CHAR_g};
WORD aMsgScreenSetup[22] = {21, CHAR_B, CHAR_i, CHAR_l, CHAR_d, CHAR_s, CHAR_c, CHAR_h, CHAR_i, CHAR_r, CHAR_m, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_u, CHAR_n, CHAR_g};
WORD aMsgAudioSetup[22] = {16, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_u, CHAR_n, CHAR_g};
WORD aMsgCustomSetup[28] = {19, CHAR_B, CHAR_e, CHAR_n, CHAR_u, CHAR_t, CHAR_z, CHAR_e, CHAR_r, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_u, CHAR_n, CHAR_g};
WORD aMsgExitSetup[30] = {18, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_n};
WORD aMsgPassword[31] = {27, CHAR_B, CHAR_i, CHAR_t, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_s, CHAR_SPACE, CHAR_K, CHAR_e, CHAR_n, CHAR_n, CHAR_w, CHAR_o, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_e, CHAR_i, CHAR_n, CHAR_g, CHAR_e, CHAR_b, CHAR_e, CHAR_n};
WORD aMsgWrongPassword[20] = {17, CHAR_F, CHAR_a, CHAR_l, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_s, CHAR_SPACE, CHAR_K, CHAR_e, CHAR_n, CHAR_n, CHAR_w, CHAR_o, CHAR_r, CHAR_t};
WORD aMsgOldPassword[9] = {4, CHAR_A, CHAR_l, CHAR_t, CHAR_COLON};
WORD aMsgNewPassword[10] = {4, CHAR_N, CHAR_e, CHAR_u, CHAR_COLON};
WORD aMsgVerifyPassword[12] = {11, CHAR_U4, CHAR_b, CHAR_e, CHAR_r, CHAR_p, CHAR_r, CHAR_u2, CHAR_f, CHAR_e, CHAR_n, CHAR_COLON};
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
WORD aMENUPhotoSetting[26] = {11, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_E, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aMENUIntervalTime[20] = {9, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l, CHAR_l};
#ifndef MUSTEK_SETUP_MENU									
WORD aMENU5Sec[7] = {6, CHAR_5, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_k, CHAR_FULL_STOP};
WORD aMENU10Sec[8] = {7, CHAR_1, CHAR_0, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_k, CHAR_FULL_STOP};
#endif //MUSTEK_SETUP_MENU									
WORD aMENU15Sec[8] = {7, CHAR_1, CHAR_5, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_k, CHAR_FULL_STOP};
WORD aMENU30Sec[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_k, CHAR_FULL_STOP};
WORD aMENU1Min[7] = {6, CHAR_1, CHAR_SPACE, CHAR_M, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU3Min[7] = {6, CHAR_3, CHAR_SPACE, CHAR_M, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENUTransitionEffect[21] = {12, CHAR_U4, CHAR_b, CHAR_e, CHAR_r, CHAR_b, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_u, CHAR_n, CHAR_g};
WORD aMENUJPEGEffectNone[12] = {11, CHAR_K, CHAR_e, CHAR_i, CHAR_n, CHAR_SPACE, CHAR_E, CHAR_f, CHAR_f, CHAR_e, CHAR_k, CHAR_t};
WORD aMENUJPEGEffectCoverUp[17] = {9, CHAR_F, CHAR_u2, CHAR_l, CHAR_l, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_o, CHAR_FULL_STOP};
WORD aMENUJPEGEffectCoverDown[18] = {9, CHAR_F, CHAR_u2, CHAR_l, CHAR_l, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_u, CHAR_FULL_STOP};
#ifndef MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectCoverLeft[16] = {10, CHAR_U4, CHAR_b, CHAR_e, CHAR_r, CHAR_d, CHAR_e, CHAR_c, CHAR_FULL_STOP, CHAR_SPACE, CHAR_L};
WORD aMENUJPEGEffectCoverRight[17] = {10, CHAR_U4, CHAR_b, CHAR_e, CHAR_r, CHAR_d, CHAR_e, CHAR_c, CHAR_FULL_STOP, CHAR_SPACE, CHAR_R};
WORD aMENUJPEGEffectUncoverUp[18] = {12, CHAR_A, CHAR_u, CHAR_f, CHAR_d, CHAR_e, CHAR_c, CHAR_k, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_o, CHAR_FULL_STOP};
WORD aMENUJPEGEffectUncoverDown[17] = {12, CHAR_A, CHAR_u, CHAR_f, CHAR_d, CHAR_e, CHAR_c, CHAR_k, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_u, CHAR_FULL_STOP};
#endif //MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectUncoverLeft[16] = {11, CHAR_A, CHAR_u, CHAR_f, CHAR_d, CHAR_e, CHAR_c, CHAR_k, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_L};
WORD aMENUJPEGEffectUncoverRight[17] = {11, CHAR_A, CHAR_u, CHAR_f, CHAR_d, CHAR_e, CHAR_c, CHAR_k, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_R};
WORD aMENUJPEGEffectWipeUp[20] = {15, CHAR_V, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_o, CHAR_b, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_r, CHAR_o, CHAR_l, CHAR_l, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectWipeDown[17] = {16, CHAR_V, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_u, CHAR_n, CHAR_t, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_r, CHAR_o, CHAR_l, CHAR_l, CHAR_e, CHAR_n};
#ifndef MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectWipeLeft[17] = {9, CHAR_W, CHAR_i, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_L};
WORD aMENUJPEGEffectWipeRight[18] = {9, CHAR_W, CHAR_i, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_R};
WORD aMENUJPEGEffectPushUp[14] = {7, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_o, CHAR_FULL_STOP};
WORD aMENUJPEGEffectPushDown[13] = {7, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_u, CHAR_FULL_STOP};
WORD aMENUJPEGEffectPushLeft[17] = {7, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_L, CHAR_FULL_STOP};
#endif //MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectPushRight[18] = {7, CHAR_N, CHAR_a, CHAR_c, CHAR_h, CHAR_SPACE, CHAR_R, CHAR_FULL_STOP};
WORD aMENUJPEGEffectFadeThroughBlack[26] = {20, CHAR_U4, CHAR_b, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_S, CHAR_c, CHAR_h, CHAR_w, CHAR_a, CHAR_r, CHAR_z, CHAR_SPACE, CHAR_b, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_n};
WORD aMENUJPEGEffectRandom[16] = {11, CHAR_Z, CHAR_u, CHAR_f, CHAR_a, CHAR_l, CHAR_l, CHAR_s, CHAR_w, CHAR_i, CHAR_e, CHAR_d};
WORD aMENUBrowse[12] = {11, CHAR_D, CHAR_u, CHAR_r, CHAR_c, CHAR_h, CHAR_s, CHAR_u, CHAR_c, CHAR_h, CHAR_e, CHAR_n};
WORD aMENUFileManager[16] = {12, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_i, CHAR_m, CHAR_a, CHAR_n, CHAR_a, CHAR_g, CHAR_e, CHAR_r};
WORD aMENUSetupMenu[16] = {9, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p, CHAR_m, CHAR_e, CHAR_n, CHAR_u2};
WORD aMENUThumbnail[13] = {12, CHAR_M, CHAR_i, CHAR_n, CHAR_i, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_b, CHAR_i, CHAR_l, CHAR_d};
WORD aMENUAutoPlay[14] = {13, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_W, CHAR_i, CHAR_e, CHAR_d, CHAR_e, CHAR_r, CHAR_g, CHAR_FULL_STOP};
WORD aMENUPhoto[12] = {4, CHAR_F, CHAR_o, CHAR_t, CHAR_o};
WORD aMENUMusic[7] = {5, CHAR_M, CHAR_u, CHAR_s, CHAR_i, CHAR_k};
WORD aMENUPhotoAudio[14] = {10, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_SOLIDUS, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMENUMovie[10] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aMENUDisplaySetup[16] = {13, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p, CHAR_SPACE, CHAR_A, CHAR_n, CHAR_z, CHAR_e, CHAR_i, CHAR_g, CHAR_e};
WORD aMENUDisplayType[14] = {10, CHAR_A, CHAR_n, CHAR_z, CHAR_e, CHAR_i, CHAR_g, CHAR_e, CHAR_a, CHAR_r, CHAR_t};
WORD aMENUTVStandard[12] = {7, CHAR_T, CHAR_V, CHAR_HYPHEN_MINUS, CHAR_N, CHAR_o, CHAR_r, CHAR_m};
WORD aMENUNTSC[5] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aMENUPAL[4] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aMENUAUTO[11] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aMENUBrightness[11] = {10, CHAR_H, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_g, CHAR_k, CHAR_e, CHAR_i, CHAR_t};
WORD aMENUContrast[10] = {8, CHAR_K, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMENUNormal[9] = {6, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l};
WORD aMENUBright[10] = {4, CHAR_H, CHAR_e, CHAR_l, CHAR_l};
WORD aMENUSoft[8] = {5, CHAR_S, CHAR_a, CHAR_n, CHAR_f, CHAR_t};
WORD aMENUTFTBrightness[15] = {14, CHAR_T, CHAR_F, CHAR_T, CHAR_HYPHEN_MINUS, CHAR_H, CHAR_e, CHAR_l, CHAR_l, CHAR_i, CHAR_g, CHAR_k, CHAR_e, CHAR_i, CHAR_t};
WORD aMENUTFTContrast[14] = {12, CHAR_T, CHAR_F, CHAR_T, CHAR_HYPHEN_MINUS, CHAR_K, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMENUTFTSaturation[16] = {13, CHAR_T, CHAR_F, CHAR_T, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_a5, CHAR_t, CHAR_t, CHAR_i, CHAR_g, CHAR_u, CHAR_n, CHAR_g};
WORD aMENURepeatMode[15] = {9, CHAR_W, CHAR_d, CHAR_h, CHAR_HYPHEN_MINUS, CHAR_M, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aMENURepeatOne[9] = {4, CHAR_E, CHAR_i, CHAR_n, CHAR_s};
WORD aMENURepeatFolder[9] = {6, CHAR_o, CHAR_r, CHAR_d, CHAR_n, CHAR_e, CHAR_r};
WORD aMENURepeatAll[10] = {5, CHAR_A, CHAR_l, CHAR_l, CHAR_e, CHAR_s};
WORD aMENUCustomSetup[28] = {19, CHAR_B, CHAR_e, CHAR_n, CHAR_u, CHAR_t, CHAR_z, CHAR_e, CHAR_r, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_u, CHAR_n, CHAR_g};
WORD aMENUOSDLanguage[14] = {11, CHAR_O, CHAR_S, CHAR_D, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_p, CHAR_r, CHAR_a, CHAR_c, CHAR_h, CHAR_e};
WORD aMENUEnglish[10] = {8, CHAR_E, CHAR_n, CHAR_g, CHAR_l, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUChinese[11] = {10, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUJapanese[11] = {9, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUKorean[11] = {10, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUFrench[12] = {11, CHAR_F, CHAR_r, CHAR_a, CHAR_n, CHAR_z, CHAR_o3, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUGerman[10] = {7, CHAR_D, CHAR_e, CHAR_u, CHAR_t, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUItalian[12] = {11, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_e, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUSpanish[11] = {8, CHAR_S, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUPortuguese[14] = {13, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_i, CHAR_e, CHAR_s, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUDutch[12] = {11, CHAR_H, CHAR_o, CHAR_l, CHAR_l, CHAR_a5, CHAR_n, CHAR_d, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUPolish[9] = {8, CHAR_P, CHAR_o, CHAR_l, CHAR_n, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
WORD aMENUUIStyle[7] = {4, CHAR_S, CHAR_t, CHAR_i, CHAR_l};
WORD aMENUUIStyle1[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_1};
WORD aMENUUIStyle2[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_2};
WORD aMENUUIStyle3[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_3};
WORD aMENUUIStyle4[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_4};
WORD aMENUDefault[18] = {8, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_r, CHAR_d};
WORD aMENURead[8] = {5, CHAR_L, CHAR_e, CHAR_s, CHAR_e, CHAR_n};
WORD aMENUIconExit[6] = {4, CHAR_E, CHAR_n, CHAR_d, CHAR_e};
WORD aMENUHelpSelect[10] = {9, CHAR_SPACE, CHAR_COLON, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t};
WORD aMENUHelpConfirm[11] = {9, CHAR_COLON, CHAR_SPACE, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_r, CHAR_m};
WORD aMENUHelpExit[18] = {17, CHAR_E, CHAR_x, CHAR_i, CHAR_t, CHAR_COLON, CHAR_SPACE, CHAR_S, CHAR_a, CHAR_v, CHAR_e, CHAR_SPACE, CHAR_AMPERSAND, CHAR_SPACE, CHAR_E, CHAR_x, CHAR_i, CHAR_t};
#endif //#ifdef IMAGE_FRAME_MENU_MAIN_FILE									
#ifdef SUPPORT_POWERON_MENU									
WORD aPOWERONIconPhoto[8] = {4, CHAR_F, CHAR_o, CHAR_t, CHAR_o};
WORD aPOWERONIconMusic[7] = {5, CHAR_M, CHAR_u, CHAR_s, CHAR_i, CHAR_k};
WORD aPOWERONIconPhotoMusic[15] = {10, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_PLUS_SIGN, CHAR_M, CHAR_u, CHAR_s, CHAR_i, CHAR_k};
WORD aPOWERONIconMovie[10] = {4, CHAR_F, CHAR_i, CHAR_l, CHAR_m};
WORD aPOWERONIconSetup[14] = {5, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p};
WORD aPOWERONIconCalendar[11] = {8, CHAR_K, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_r};
WORD aPOWERONIconClock[9] = {3, CHAR_U, CHAR_h, CHAR_r};
WORD aPOWERONIconAlarm[7] = {5, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m};
WORD aPOWERONIconAutoPower[14] = {8, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_A, CHAR_u, CHAR_s};
WORD aPOWERONIconEdit[11] = {10, CHAR_B, CHAR_e, CHAR_a, CHAR_r, CHAR_b, CHAR_e, CHAR_i, CHAR_t, CHAR_e, CHAR_n};
WORD aPOWERONIconFavorite[10] = {7, CHAR_F, CHAR_a, CHAR_v, CHAR_o, CHAR_r, CHAR_i, CHAR_t};
WORD aPOWERONIconRadio[6] = {5, CHAR_R, CHAR_a, CHAR_d, CHAR_i, CHAR_o};
WORD aWeeklyDay_Mon[4] = {2, CHAR_M, CHAR_o};
WORD aWeeklyDay_Tue[4] = {2, CHAR_D, CHAR_i};
WORD aWeeklyDay_Wed[4] = {2, CHAR_M, CHAR_i};
WORD aWeeklyDay_Thu[4] = {2, CHAR_D, CHAR_o};
WORD aWeeklyDay_Fri[4] = {2, CHAR_F, CHAR_r};
WORD aWeeklyDay_Sat[4] = {2, CHAR_S, CHAR_a};
WORD aWeeklyDay_Sun[4] = {2, CHAR_S, CHAR_o};
WORD aMonthJanuary[4] = {3, CHAR_J, CHAR_a, CHAR_n};
WORD aMonthFebruary[4] = {3, CHAR_F, CHAR_e, CHAR_b};
WORD aMonthMarch[4] = {3, CHAR_M, CHAR_a5, CHAR_r};
WORD aMonthApril[4] = {3, CHAR_A, CHAR_p, CHAR_r};
WORD aMonthMay[4] = {3, CHAR_M, CHAR_a, CHAR_i};
WORD aMonthJune[4] = {3, CHAR_J, CHAR_u, CHAR_n};
WORD aMonthJuly[4] = {3, CHAR_J, CHAR_u, CHAR_l};
WORD aMonthAugust[4] = {3, CHAR_A, CHAR_u, CHAR_g};
WORD aMonthSeptember[4] = {3, CHAR_S, CHAR_e, CHAR_p};
WORD aMonthOctober[4] = {3, CHAR_O, CHAR_k, CHAR_t};
WORD aMonthNovember[4] = {3, CHAR_N, CHAR_o, CHAR_v};
WORD aMonthDecember[4] = {3, CHAR_D, CHAR_e, CHAR_z};
WORD aWeeklyMonday[14] = {6, CHAR_M, CHAR_o, CHAR_n, CHAR_t, CHAR_a, CHAR_g};
WORD aWeeklyTuesday[12] = {8, CHAR_D, CHAR_i, CHAR_e, CHAR_n, CHAR_s, CHAR_t, CHAR_a, CHAR_g};
WORD aWeeklyWednesday[13] = {8, CHAR_M, CHAR_i, CHAR_t, CHAR_t, CHAR_w, CHAR_o, CHAR_c, CHAR_h};
WORD aWeeklyThursday[13] = {10, CHAR_D, CHAR_o, CHAR_n, CHAR_n, CHAR_e, CHAR_r, CHAR_s, CHAR_t, CHAR_a, CHAR_g};
WORD aWeeklyFriday[12] = {7, CHAR_F, CHAR_r, CHAR_e, CHAR_i, CHAR_t, CHAR_a, CHAR_g};
WORD aWeeklySaturday[9] = {7, CHAR_S, CHAR_a, CHAR_m, CHAR_s, CHAR_t, CHAR_a, CHAR_g};
WORD aWeeklySunday[10] = {7, CHAR_S, CHAR_o, CHAR_n, CHAR_n, CHAR_t, CHAR_a, CHAR_g};
WORD aCLOCKClockSetting[22] = {10, CHAR_U, CHAR_h, CHAR_r, CHAR_SPACE, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aCLOCKDisplay[13] = {7, CHAR_A, CHAR_n, CHAR_z, CHAR_e, CHAR_i, CHAR_g, CHAR_e};
WORD aCLOCKSetTime[14] = {11, CHAR_Z, CHAR_e, CHAR_i, CHAR_t, CHAR_SPACE, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aCLOCKExit[6] = {4, CHAR_E, CHAR_n, CHAR_d, CHAR_e};
WORD aCLOCKBigClock[9] = {3, CHAR_U, CHAR_h, CHAR_r};
WORD aCLOCKClockCalendar[22] = {14, CHAR_U, CHAR_h, CHAR_r, CHAR_SPACE, CHAR_AMPERSAND, CHAR_SPACE, CHAR_K, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_r};
WORD aBUTTONOK[3] = {2, CHAR_O, CHAR_K};
WORD aBUTTONCancel[9] = {7, CHAR_A, CHAR_b, CHAR_b, CHAR_r, CHAR_e, CHAR_c, CHAR_h};
WORD aSETTIMESetTime[15] = {11, CHAR_Z, CHAR_e, CHAR_i, CHAR_t, CHAR_SPACE, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aALARMEnable[10] = {6, CHAR_a, CHAR_k, CHAR_t, CHAR_i, CHAR_v, CHAR_FULL_STOP};
WORD aALARMAlarmSetting[21] = {11, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aALARMDlgTitle[8] = {5, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m};
WORD aALARMDlgContent[34] = {29, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m, CHAR_SPACE, CHAR_m, CHAR_i, CHAR_t, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_l, CHAR_FULL_STOP, CHAR_SPACE, CHAR_T, CHAR_a, CHAR_s, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_s, CHAR_t, CHAR_o, CHAR_p, CHAR_p, CHAR_e, CHAR_n, CHAR_FULL_STOP};
WORD aAUTOPWRAutoPowerSetting[46] = {20, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_A, CHAR_u, CHAR_s, CHAR_HYPHEN_MINUS, CHAR_SOLIDUS, CHAR_E, CHAR_i, CHAR_n, CHAR_HYPHEN_MINUS, CHAR_E, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aAUTOPWRPowerOn[11] = {7, CHAR_E, CHAR_i, CHAR_n, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP};
WORD aAUTOPWRPowerOff[12] = {7, CHAR_A, CHAR_u, CHAR_s, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP};
WORD aAUTOPWRPowerOnEnable[30] = {14, CHAR_E, CHAR_i, CHAR_n, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_k, CHAR_t, CHAR_i, CHAR_v, CHAR_FULL_STOP};
WORD aAUTOPWRPowerOffEnable[33] = {13, CHAR_A, CHAR_u, CHAR_s, CHAR_s, CHAR_c, CHAR_h, CHAR_FULL_STOP, CHAR_SPACE, CHAR_a, CHAR_k, CHAR_t, CHAR_i, CHAR_v};
WORD aAUTOPWRSetTime[14] = {11, CHAR_Z, CHAR_e, CHAR_i, CHAR_t, CHAR_SPACE, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_FULL_STOP};
WORD aMMBtnSave[10] = {9, CHAR_S, CHAR_p, CHAR_e, CHAR_i, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_n};
WORD aMMSaveFileDlgTitle[20] = {15, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_i, CHAR_SPACE, CHAR_s, CHAR_p, CHAR_e, CHAR_i, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_n};
WORD aMMSaveFileDlgContent[18] = {12, CHAR_B, CHAR_i, CHAR_t, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_w, CHAR_a, CHAR_r, CHAR_t, CHAR_e, CHAR_n};
WORD aTHUMBButtonSort[10] = {9, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_i, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aTHUMBButtonDeleteAll[11] = {10, CHAR_A, CHAR_l, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_l, CHAR_o3, CHAR_s, CHAR_c, CHAR_h};
WORD aTHUMBTitleEditMode[18] = {17, CHAR_B, CHAR_e, CHAR_a, CHAR_r, CHAR_b, CHAR_e, CHAR_i, CHAR_t, CHAR_u, CHAR_n, CHAR_g, CHAR_s, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aTHUMBTitleDeleteMode[19] = {10, CHAR_L, CHAR_o3, CHAR_s, CHAR_c, CHAR_h, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aTHUMBHelpDeleteMode[14] = {7, CHAR_L, CHAR_o3, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_n};
WORD aTHUMBHelpSortMode[26] = {16, CHAR_B, CHAR_i, CHAR_l, CHAR_d, CHAR_SPACE, CHAR_f, CHAR_o, CHAR_k, CHAR_u, CHAR_s, CHAR_s, CHAR_i, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aTHUMBTitleSortMode[18] = {12, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_i, CHAR_e, CHAR_r, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
WORD aEDITDialogTitle[27] = {17, CHAR_B, CHAR_e, CHAR_a, CHAR_r, CHAR_b, CHAR_e, CHAR_i, CHAR_t, CHAR_e, CHAR_n, CHAR_HYPHEN_MINUS, CHAR_D, CHAR_i, CHAR_a, CHAR_l, CHAR_o, CHAR_g};
WORD aEDITDialogContent[38] = {30, CHAR_B, CHAR_i, CHAR_t, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_B, CHAR_e, CHAR_a, CHAR_r, CHAR_b, CHAR_e, CHAR_i, CHAR_t, CHAR_u, CHAR_n, CHAR_g, CHAR_s, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s, CHAR_SPACE, CHAR_w, CHAR_a5, CHAR_h, CHAR_l, CHAR_e, CHAR_n};
WORD aBUTTONCopy[9] = {8, CHAR_K, CHAR_o, CHAR_p, CHAR_i, CHAR_e, CHAR_r, CHAR_e, CHAR_n};
WORD aBUTTONDelete[9] = {7, CHAR_L, CHAR_o3, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_n};
WORD aSETDATESetDate[17] = {16, CHAR_D, CHAR_a, CHAR_t, CHAR_u, CHAR_m, CHAR_SPACE, CHAR_e, CHAR_i, CHAR_n, CHAR_s, CHAR_t, CHAR_e, CHAR_l, CHAR_l, CHAR_e, CHAR_n};
#endif //#ifdef SUPPORT_POWERON_MENU									
WORD aMENUOn[5] = {2, CHAR_A, CHAR_n};
WORD aMENUOff[7] = {3, CHAR_A, CHAR_u, CHAR_s};
#ifdef MUSTEK_SETUP_MENU									
WORD aMENU5Min[7] = {6, CHAR_5, CHAR_SPACE, CHAR_M, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU30Min[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_M, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU1Hour[8] = {5, CHAR_1, CHAR_SPACE, CHAR_S, CHAR_t, CHAR_d};
WORD aMENU4Hours[9] = {5, CHAR_4, CHAR_SPACE, CHAR_S, CHAR_t, CHAR_d};
WORD aMENU12Hours[10] = {6, CHAR_1, CHAR_2, CHAR_SPACE, CHAR_S, CHAR_t, CHAR_d};
WORD aMENU1Day[9] = {5, CHAR_1, CHAR_SPACE, CHAR_T, CHAR_a, CHAR_g};
WORD aMENUTransition[14] = {8, CHAR_U4, CHAR_b, CHAR_e, CHAR_r, CHAR_g, CHAR_a, CHAR_n, CHAR_g};
WORD aMENUTVSystem[17] = {9, CHAR_T, CHAR_V, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_y, CHAR_s, CHAR_t, CHAR_e, CHAR_m};
WORD aMENUFullScreen[15] = {8, CHAR_V, CHAR_o, CHAR_l, CHAR_l, CHAR_b, CHAR_i, CHAR_l, CHAR_d};
WORD aMENUReset[8] = {5, CHAR_R, CHAR_e, CHAR_s, CHAR_e, CHAR_t};
WORD aMENUConfirm[8] = {7, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_r, CHAR_m};
WORD aMENUExitSetup[30] = {18, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p, CHAR_m, CHAR_o, CHAR_d, CHAR_u, CHAR_s, CHAR_SPACE, CHAR_b, CHAR_e, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_n};
#endif //MUSTEK_SETUP_MENU											
WORD aBUTTONYes[4] = {2, CHAR_J, CHAR_a};
WORD aBUTTONNo[5] = {4, CHAR_N, CHAR_e, CHAR_i, CHAR_n};
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW											
WORD aMMSaveDlgTitle[31] = {21, CHAR_S, CHAR_p, CHAR_e, CHAR_i, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_n, CHAR_HYPHEN_MINUS, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_d, CHAR_i, CHAR_a, CHAR_l, CHAR_o, CHAR_g, CHAR_SPACE};
WORD aMMDeleteDlgTitle[31] = {19, CHAR_L, CHAR_o3, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_n, CHAR_HYPHEN_MINUS, CHAR_F, CHAR_o, CHAR_t, CHAR_o, CHAR_d, CHAR_i, CHAR_a, CHAR_l, CHAR_o, CHAR_g, CHAR_SPACE};
WORD aMMSaveDlgContent[14] = {10, CHAR_S, CHAR_p, CHAR_e, CHAR_i, CHAR_c, CHAR_h, CHAR_e, CHAR_r, CHAR_n, CHAR_QUESTION_MARK};
WORD aMMDeleteDlgContent[13] = {8, CHAR_L, CHAR_o3, CHAR_s, CHAR_c, CHAR_h, CHAR_e, CHAR_n, CHAR_QUESTION_MARK};
#endif //#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW									
#ifdef SUPPORT_CHOOSE_MEDIA									
WORD aMENUMedia[11] = {5, CHAR_M, CHAR_e, CHAR_d, CHAR_i, CHAR_a};
#endif //SUPPORT_CHOOSE_MEDIA									
#ifdef SUPPORT_PHOTO_BROWSE_MODE									
WORD aNDBrowseMode[26] = {17, CHAR_D, CHAR_u, CHAR_r, CHAR_c, CHAR_h, CHAR_s, CHAR_u, CHAR_c, CHAR_h, CHAR_e, CHAR_n, CHAR_HYPHEN_MINUS, CHAR_M, CHAR_o, CHAR_d, CHAR_u, CHAR_s};
#endif //SUPPORT_PHOTO_BROWSE_MODE									
#ifdef SUPPORT_TFT_DIGITAL_MODE									
WORD aMENUAVOutput[13] = {10, CHAR_A, CHAR_V, CHAR_HYPHEN_MINUS, CHAR_A, CHAR_u, CHAR_s, CHAR_g, CHAR_a, CHAR_n, CHAR_g};
#endif //SUPPORT_TFT_DIGITAL_MODE									
WORD aMENUPanel[9] = {6, CHAR_F, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_t};
#ifdef SUPPORT_RADIO_TUNER									
WORD aRADIORecord[9] = {5, CHAR_A, CHAR_u, CHAR_f, CHAR_n, CHAR_FULL_STOP};
WORD aRADIOScan[9] = {8, CHAR_S, CHAR_u, CHAR_c, CHAR_h, CHAR_l, CHAR_a, CHAR_u, CHAR_f};
WORD aRADIOStationsFound[17] = {11, CHAR_S, CHAR_e, CHAR_n, CHAR_d, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_g, CHAR_e, CHAR_f, CHAR_FULL_STOP};
#endif //SUPPORT_RADIO_TUNER									
#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									
WORD aCODEWesternEuropean[17] = {14, CHAR_W, CHAR_e, CHAR_s, CHAR_t, CHAR_e, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_a5, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
#endif   // SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									
#ifdef SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									
WORD aCODECentralEuropean[19] = {13, CHAR_Z, CHAR_e, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_l, CHAR_e, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_FULL_STOP};
#endif   // SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									
#ifdef SUPPORT_CHAR_ENCODING_GREEK									
WORD aCODEGreek[11] = {10, CHAR_G, CHAR_r, CHAR_i, CHAR_e, CHAR_c, CHAR_h, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
#endif   // SUPPORT_CHAR_ENCODING_GREEK									
#ifdef SUPPORT_CHAR_ENCODING_HEBREW									
WORD aCODEHebrew[10] = {9, CHAR_H, CHAR_e, CHAR_b, CHAR_r, CHAR_a5, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
#endif   // SUPPORT_CHAR_ENCODING_HEBREW									
#ifdef SUPPORT_CHAR_ENCODING_TURKISH									
WORD aCODETurkish[9] = {8, CHAR_T, CHAR_u2, CHAR_r, CHAR_k, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
#endif   // SUPPORT_CHAR_ENCODING_TURKISH									
#ifdef SUPPORT_CHAR_ENCODING_CP1250									
WORD aCODECP1250[8] = {7, CHAR_C, CHAR_P, CHAR_SPACE, CHAR_1, CHAR_2, CHAR_5, CHAR_0};
#endif   // SUPPORT_CHAR_ENCODING_CP1250									
#ifdef SUPPORT_CHAR_ENCODING_CYRILLIC									
WORD aCODECyrillic[11] = {10, CHAR_K, CHAR_y, CHAR_r, CHAR_i, CHAR_l, CHAR_l, CHAR_i, CHAR_s, CHAR_c, CHAR_h};
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
