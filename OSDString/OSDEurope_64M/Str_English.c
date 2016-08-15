#include "stringdec.h"
#ifdef OSDDSPLY_MAIN_FILE									
WORD aDSPLYLanguageJapanese[11] = {8, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aDSPLYLanguageEnglish[10] = {7, CHAR_E, CHAR_n, CHAR_g, CHAR_l, CHAR_i, CHAR_s, CHAR_h};
WORD aDSPLYLanguageFrench[12] = {6, CHAR_F, CHAR_r, CHAR_e, CHAR_n, CHAR_c, CHAR_h};
WORD aDSPLYLanguageGerman[10] = {6, CHAR_G, CHAR_e, CHAR_r, CHAR_m, CHAR_a, CHAR_n};
WORD aDSPLYLanguageRussian[9] = {7, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_i, CHAR_a, CHAR_n};
WORD aDSPLYLanguageChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aDSPLYLanguageKorean[11] = {6, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n};
WORD aDSPLYLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aDSPLYLanguageThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aDSPLYLanguageItalian[12] = {7, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n};
WORD aDSPLYLanguageSpanish[11] = {7, CHAR_S, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_h};
WORD aDSPLYLanguagePortuguese[14] = {10, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_u, CHAR_e, CHAR_s, CHAR_e};
WORD aDSPLYLanguageOthers[9] = {6, CHAR_O, CHAR_t, CHAR_h, CHAR_e, CHAR_r, CHAR_s};
WORD aDSPLYOff[10] = {3, CHAR_O, CHAR_f, CHAR_f};
#ifndef NO_DVD_DISPLAY_UI									
WORD aDSPLYTitleElapsed[17] = {13, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_E, CHAR_l, CHAR_a, CHAR_p, CHAR_s, CHAR_e, CHAR_d};
WORD aDSPLYTitleRemain[16] = {12, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_R, CHAR_e, CHAR_m, CHAR_a, CHAR_i, CHAR_n};
WORD aDSPLYChapterElapsed[16] = {15, CHAR_C, CHAR_h, CHAR_a, CHAR_p, CHAR_t, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_E, CHAR_l, CHAR_a, CHAR_p, CHAR_s, CHAR_e, CHAR_d};
WORD aDSPLYChapterRemain[15] = {14, CHAR_C, CHAR_h, CHAR_a, CHAR_p, CHAR_t, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_R, CHAR_e, CHAR_m, CHAR_a, CHAR_i, CHAR_n};
#endif //NO_DVD_DISPLAY_UI									
WORD aDSPLYSingleElapsed[17] = {14, CHAR_S, CHAR_i, CHAR_n, CHAR_g, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_E, CHAR_l, CHAR_a, CHAR_p, CHAR_s, CHAR_e, CHAR_d};
WORD aDSPLYSingleRemain[16] = {13, CHAR_S, CHAR_i, CHAR_n, CHAR_g, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_R, CHAR_e, CHAR_m, CHAR_a, CHAR_i, CHAR_n};
WORD aDSPLYTotalElapsed[18] = {13, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_E, CHAR_l, CHAR_a, CHAR_p, CHAR_s, CHAR_e, CHAR_d};
WORD aDSPLYTotalRemain[17] = {12, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_R, CHAR_e, CHAR_m, CHAR_a, CHAR_i, CHAR_n};
WORD aDSPLYRootMenu[16] = {9, CHAR_R, CHAR_o, CHAR_o, CHAR_t, CHAR_SPACE, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aDSPLYTitleMenu[12] = {10, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
#ifdef SUPPORT_NAVIGATOR									
WORD aDSPLYRepeatTitle[20] = {12, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_a, CHAR_t, CHAR_SPACE, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e};
WORD aDSPLYRepeatChapter[22] = {14, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_a, CHAR_t, CHAR_SPACE, CHAR_C, CHAR_h, CHAR_a, CHAR_p, CHAR_t, CHAR_e, CHAR_r};
WORD aDSPLYRepeatDisc[19] = {11, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_a, CHAR_t, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aDSPLYRepeatOff[22] = {10, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_a, CHAR_t, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
#endif //SUPPORT_NAVIGATOR									
WORD aDSPLYpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aDSPLYInterlace[15] = {9, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_l, CHAR_a, CHAR_c, CHAR_e};
WORD aDSPLYResolution[15] = {11, CHAR_R, CHAR_e, CHAR_s, CHAR_o, CHAR_l, CHAR_u, CHAR_t, CHAR_i, CHAR_o, CHAR_n, CHAR_COLON};
WORD aDSPLYDate[7] = {5, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_COLON};
#ifdef SUPPORT_NAVIGATOR									
WORD aNGTitle[8] = {6, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e, CHAR_COLON};
WORD aNGChapter[11] = {8, CHAR_C, CHAR_h, CHAR_a, CHAR_p, CHAR_t, CHAR_e, CHAR_r, CHAR_COLON};
WORD aNGAngle[13] = {6, CHAR_A, CHAR_n, CHAR_g, CHAR_l, CHAR_e, CHAR_COLON};
WORD aNGAudio[7] = {6, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o, CHAR_COLON};
WORD aNGSubtitle1[13] = {10, CHAR_S, CHAR_u, CHAR_b, CHAR_t, CHAR_i, CHAR_t, CHAR_l, CHAR_e, CHAR_1, CHAR_COLON};
WORD aNGSubtitle2[13] = {10, CHAR_S, CHAR_u, CHAR_b, CHAR_t, CHAR_i, CHAR_t, CHAR_l, CHAR_e, CHAR_2, CHAR_COLON};
#endif //SUPPORT_NAVIGATOR									
#endif //OSDDSPLY_MAIN_FILE									
#ifdef OSDND_MAIN_FILE									
WORD aNDNoDisc[14] = {7, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDNoUSBDevice[14] = {13, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_U, CHAR_S, CHAR_B, CHAR_SPACE, CHAR_D, CHAR_e, CHAR_v, CHAR_i, CHAR_c, CHAR_e};
WORD aNDOpen[7] = {4, CHAR_O, CHAR_p, CHAR_e, CHAR_n};
WORD aNDClose[10] = {5, CHAR_C, CHAR_l, CHAR_o, CHAR_s, CHAR_e};
WORD aNDReturn[8] = {6, CHAR_R, CHAR_e, CHAR_t, CHAR_u, CHAR_r, CHAR_n};
WORD aNDFreeze[15] = {6, CHAR_F, CHAR_r, CHAR_e, CHAR_e, CHAR_z, CHAR_e};
WORD aNDRandomOn[12] = {11, CHAR_R, CHAR_a, CHAR_n, CHAR_d, CHAR_o, CHAR_m, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aNDRandomOff[12] = {10, CHAR_O, CHAR_r, CHAR_d, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aNDResume[11] = {6, CHAR_R, CHAR_e, CHAR_s, CHAR_u, CHAR_m, CHAR_e};
WORD aNDRootMenu[16] = {9, CHAR_R, CHAR_o, CHAR_o, CHAR_t, CHAR_SPACE, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aNDTitleMenu[12] = {10, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aNDRotateRight[26] = {16, CHAR_R, CHAR_o, CHAR_t, CHAR_a, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t, CHAR_SPACE, CHAR_9, CHAR_0, CHAR_N_DEGREE};
WORD aNDRotateLeft[26] = {15, CHAR_R, CHAR_o, CHAR_t, CHAR_a, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_L, CHAR_e, CHAR_f, CHAR_t, CHAR_SPACE, CHAR_9, CHAR_0, CHAR_N_DEGREE};
WORD aNDFlipHorizontally[26] = {17, CHAR_F, CHAR_l, CHAR_i, CHAR_p, CHAR_SPACE, CHAR_H, CHAR_o, CHAR_r, CHAR_i, CHAR_z, CHAR_o, CHAR_n, CHAR_t, CHAR_a, CHAR_l, CHAR_l, CHAR_y};
WORD aNDFlipVertically[25] = {15, CHAR_F, CHAR_l, CHAR_i, CHAR_p, CHAR_SPACE, CHAR_V, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_c, CHAR_a, CHAR_l, CHAR_l, CHAR_y};
WORD aNDpscan[12] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aNDInterlace[15] = {9, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_l, CHAR_a, CHAR_c, CHAR_e};
WORD aNDRepeatNone[10] = {3, CHAR_O, CHAR_f, CHAR_f};
WORD aNDRepeatTrack[8] = {5, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k};
WORD aNDRepeatDisc[7] = {4, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDRepeatAll[10] = {3, CHAR_A, CHAR_l, CHAR_l};
WORD aNDRepeatTitle[7] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e};
WORD aNDRepeatChapter[10] = {7, CHAR_C, CHAR_h, CHAR_a, CHAR_p, CHAR_t, CHAR_e, CHAR_r};
WORD aNDRepeatFolder[9] = {6, CHAR_F, CHAR_o, CHAR_l, CHAR_d, CHAR_e, CHAR_r};
WORD aNDRepeatA[15] = {8, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_a, CHAR_t, CHAR_SPACE, CHAR_A};
WORD aNDRepeatAB[17] = {10, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_a, CHAR_t, CHAR_SPACE, CHAR_A, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aNDRepeatOff[22] = {10, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_a, CHAR_t, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
#ifndef NO_DISC_MODE									
WORD aNDNavigateOn[25] = {11, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_a, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_O, CHAR_n};
WORD aNDNavigateOff[28] = {12, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_a, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
WORD aNDPBCOn[13] = {6, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_O, CHAR_n};
WORD aNDPBCOff[14] = {7, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
#endif //#ifndef NO_DISC_MODE									
WORD aNDZoom1[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_1};
WORD aNDZoom2[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_2};
WORD aNDZoom4[8] = {6, CHAR_Z, CHAR_o, CHAR_o, CHAR_m, CHAR_x, CHAR_4};
WORD aNDMoveLeft[24] = {9, CHAR_M, CHAR_o, CHAR_v, CHAR_e, CHAR_SPACE, CHAR_L, CHAR_e, CHAR_f, CHAR_t};
WORD aNDMoveRight[24] = {10, CHAR_M, CHAR_o, CHAR_v, CHAR_e, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aNDMoveUp[23] = {7, CHAR_M, CHAR_o, CHAR_v, CHAR_e, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aNDMoveDown[23] = {9, CHAR_M, CHAR_o, CHAR_v, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
WORD aNDVocalOn[15] = {8, CHAR_V, CHAR_o, CHAR_c, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_O, CHAR_n};
WORD aNDVocalOff[16] = {9, CHAR_V, CHAR_o, CHAR_c, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
WORD aNDVocalLL[15] = {8, CHAR_V, CHAR_o, CHAR_c, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_L, CHAR_L};
WORD aNDVocalRR[13] = {8, CHAR_V, CHAR_o, CHAR_c, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_R, CHAR_R};
WORD aNDKaraokeOn[17] = {10, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_O, CHAR_n};
WORD aNDKaraokeOff[18] = {11, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
WORD aNDJPEGEffectNone[12] = {9, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_E, CHAR_f, CHAR_f, CHAR_e, CHAR_c, CHAR_t};
WORD aNDJPEGEffectCoverUp[27] = {8, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aNDJPEGEffectCoverDown[26] = {10, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
WORD aNDJPEGEffectCoverLeft[20] = {10, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_L, CHAR_e, CHAR_f, CHAR_t};
WORD aNDJPEGEffectCoverRight[20] = {11, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aNDJPEGEffectUncoverUp[23] = {10, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aNDJPEGEffectUncoverDown[24] = {12, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
WORD aNDJPEGEffectUncoverLeft[21] = {12, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_L, CHAR_e, CHAR_f, CHAR_t};
WORD aNDJPEGEffectUncoverRight[22] = {13, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aNDJPEGEffectWipeUp[22] = {7, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aNDJPEGEffectWipeDown[21] = {9, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
WORD aNDJPEGEffectWipeLeft[17] = {9, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_L, CHAR_e, CHAR_f, CHAR_t};
WORD aNDJPEGEffectWipeRight[18] = {10, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aNDJPEGEffectPushUp[14] = {7, CHAR_P, CHAR_u, CHAR_s, CHAR_h, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aNDJPEGEffectPushDown[13] = {9, CHAR_P, CHAR_u, CHAR_s, CHAR_h, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
WORD aNDJPEGEffectPushLeft[17] = {9, CHAR_P, CHAR_u, CHAR_s, CHAR_h, CHAR_SPACE, CHAR_L, CHAR_e, CHAR_f, CHAR_t};
WORD aNDJPEGEffectPushRight[18] = {10, CHAR_P, CHAR_u, CHAR_s, CHAR_h, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aNDJPEGEffectFadeThroughBlack[26] = {18, CHAR_F, CHAR_a, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_h, CHAR_r, CHAR_o, CHAR_u, CHAR_g, CHAR_h, CHAR_SPACE, CHAR_B, CHAR_l, CHAR_a, CHAR_c, CHAR_k};
WORD aNDJPEGEffectRandom[16] = {6, CHAR_R, CHAR_a, CHAR_n, CHAR_d, CHAR_o, CHAR_m};
#ifndef NO_DISC_MODE									
WORD aNDEncrypt[17] = {12, CHAR_I, CHAR_l, CHAR_l, CHAR_i, CHAR_g, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDErrRegionCode[27] = {19, CHAR_W, CHAR_r, CHAR_o, CHAR_n, CHAR_g, CHAR_SPACE, CHAR_R, CHAR_e, CHAR_g, CHAR_i, CHAR_o, CHAR_n, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDWrongDisc[19] = {10, CHAR_W, CHAR_r, CHAR_o, CHAR_n, CHAR_g, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDDamageDisc[19] = {11, CHAR_D, CHAR_a, CHAR_m, CHAR_a, CHAR_g, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
#endif //#ifndef NO_DISC_MODE									
WORD aNDNoPlayableFile[25] = {16, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y, CHAR_a, CHAR_b, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_F, CHAR_i, CHAR_l, CHAR_e};
WORD aNDDisc[7] = {4, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aNDTrack[8] = {5, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k};
WORD aNDTitle[7] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e};
WORD aNDChapter[10] = {7, CHAR_C, CHAR_h, CHAR_a, CHAR_p, CHAR_t, CHAR_e, CHAR_r};
WORD aNDLR[4] = {3, CHAR_L, CHAR_SOLIDUS, CHAR_R};
WORD aNDLL[4] = {3, CHAR_L, CHAR_SOLIDUS, CHAR_L};
WORD aNDRR[4] = {3, CHAR_R, CHAR_SOLIDUS, CHAR_R};
WORD aNDVolume[11] = {6, CHAR_V, CHAR_o, CHAR_l, CHAR_u, CHAR_m, CHAR_e};
WORD aNDTone[9] = {4, CHAR_T, CHAR_o, CHAR_n, CHAR_e};
WORD aNDEcho[5] = {4, CHAR_E, CHAR_c, CHAR_h, CHAR_o};
WORD aNDNoSurround[19] = {11, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_S, CHAR_u, CHAR_r, CHAR_r, CHAR_o, CHAR_u, CHAR_n, CHAR_d};
WORD aNDLiveSurround[10] = {4, CHAR_L, CHAR_i, CHAR_v, CHAR_e};
WORD aNDTheaterSurround[8] = {7, CHAR_T, CHAR_h, CHAR_e, CHAR_a, CHAR_t, CHAR_e, CHAR_r};
WORD aNDStereoSurround[8] = {6, CHAR_S, CHAR_t, CHAR_e, CHAR_r, CHAR_e, CHAR_o};
WORD aNDMovieSurround[9] = {5, CHAR_M, CHAR_o, CHAR_v, CHAR_i, CHAR_e};
WORD aNDKaraSurround[8] = {7, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e};
WORD aNDLanguageJapanese[11] = {8, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aNDLanguageEnglish[10] = {7, CHAR_E, CHAR_n, CHAR_g, CHAR_l, CHAR_i, CHAR_s, CHAR_h};
WORD aNDLanguageFrench[12] = {6, CHAR_F, CHAR_r, CHAR_e, CHAR_n, CHAR_c, CHAR_h};
WORD aNDLanguageGerman[10] = {6, CHAR_G, CHAR_e, CHAR_r, CHAR_m, CHAR_a, CHAR_n};
WORD aNDLanguageRussian[9] = {7, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_i, CHAR_a, CHAR_n};
WORD aNDLanguageChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aNDLanguageKorean[11] = {6, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n};
WORD aNDLanguageHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aNDLanguageThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aNDLanguageItalian[12] = {7, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n};
WORD aNDLanguageSpanish[11] = {7, CHAR_S, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_h};
WORD aNDLanguagePortuguese[14] = {10, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_u, CHAR_e, CHAR_s, CHAR_e};
WORD aNDLanguageOthers[9] = {6, CHAR_O, CHAR_t, CHAR_h, CHAR_e, CHAR_r, CHAR_s};
WORD aNDSubtitle[13] = {9, CHAR_S, CHAR_u, CHAR_b, CHAR_t, CHAR_i, CHAR_t, CHAR_l, CHAR_e, CHAR_COLON};
WORD aNDOff[10] = {3, CHAR_O, CHAR_f, CHAR_f};
WORD aNDWait[12] = {7, CHAR_L, CHAR_o, CHAR_a, CHAR_d, CHAR_i, CHAR_n, CHAR_g};
WORD aNDSelect[12] = {6, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t};
WORD aNDCardReader[29] = {11, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_R, CHAR_e, CHAR_a, CHAR_d, CHAR_e, CHAR_r};
#endif //OSDND_MAIN_FILE									
#ifdef OSDMM_MAIN_FILE									
WORD aMMProgramModeTitle[24] = {12, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
WORD aMMProgramListModeTitle[21] = {12, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_SPACE, CHAR_L, CHAR_i, CHAR_s, CHAR_t};
WORD aMMTotalFiles[25] = {12, CHAR_T, CHAR_o, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_F, CHAR_i, CHAR_l, CHAR_e, CHAR_s, CHAR_COLON};
WORD aMMFileNumber[20] = {12, CHAR_F, CHAR_i, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_N, CHAR_u, CHAR_m, CHAR_b, CHAR_e, CHAR_r, CHAR_COLON};
WORD aMMDlgFileTypeTitle[14] = {10, CHAR_F, CHAR_I, CHAR_L, CHAR_E, CHAR_SPACE, CHAR_T, CHAR_Y, CHAR_P, CHAR_E, CHAR_COLON};
WORD aMMDlgSortFileTitle[16] = {10, CHAR_S, CHAR_O, CHAR_R, CHAR_T, CHAR_SPACE, CHAR_F, CHAR_I, CHAR_L, CHAR_E, CHAR_COLON};
WORD aMMDlgSortFileMethodTitle[9] = {7, CHAR_M, CHAR_e, CHAR_t, CHAR_h, CHAR_o, CHAR_d, CHAR_COLON};
WORD aMMDlgSortFileSequenceTitle[12] = {9, CHAR_S, CHAR_e, CHAR_q, CHAR_u, CHAR_e, CHAR_n, CHAR_c, CHAR_e, CHAR_COLON};
WORD aMMDlgSequenceAscendingItem[10] = {9, CHAR_A, CHAR_s, CHAR_c, CHAR_e, CHAR_n, CHAR_d, CHAR_i, CHAR_n, CHAR_g};
WORD aMMDlgSequenceDescendingItem[12] = {10, CHAR_D, CHAR_e, CHAR_s, CHAR_c, CHAR_e, CHAR_n, CHAR_d, CHAR_i, CHAR_n, CHAR_g};
WORD aMMDlgMethodTypeItem[11] = {7, CHAR_B, CHAR_y, CHAR_SPACE, CHAR_t, CHAR_y, CHAR_p, CHAR_e};
WORD aMMDlgMethodNameItem[11] = {7, CHAR_B, CHAR_y, CHAR_SPACE, CHAR_n, CHAR_a, CHAR_m, CHAR_e};
WORD aMMDlgMethodTimeItem[10] = {7, CHAR_B, CHAR_y, CHAR_SPACE, CHAR_d, CHAR_a, CHAR_t, CHAR_e};
WORD aMMHelpProgram[16] = {11, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_SPACE, CHAR_i, CHAR_t, CHAR_SPACE};
WORD aMMHelpProgramList[14] = {9, CHAR_D, CHAR_e, CHAR_l, CHAR_e, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_i, CHAR_t};
WORD aMMBtnFileType[10] = {9, CHAR_F, CHAR_i, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_y, CHAR_p, CHAR_e};
WORD aMMBtnSortFile[12] = {9, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_F, CHAR_i, CHAR_l, CHAR_e};
WORD aMMBtnList[7] = {4, CHAR_L, CHAR_i, CHAR_s, CHAR_t};
WORD aMMBtnPlay[9] = {4, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aMMBtnDeleteAll[11] = {10, CHAR_D, CHAR_e, CHAR_l, CHAR_e, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_A, CHAR_l, CHAR_l};
WORD aMMBtnReturn[8] = {6, CHAR_R, CHAR_e, CHAR_t, CHAR_u, CHAR_r, CHAR_n};
WORD aMMBtnExit[8] = {4, CHAR_E, CHAR_x, CHAR_i, CHAR_t};
#endif //OSDMM_MAIN_FILE									
#ifdef THUMB_MAIN_FILE									
WORD aTHUMBPage[8] = {5, CHAR_P, CHAR_a, CHAR_g, CHAR_e, CHAR_COLON};
WORD aTHUMBTitleGeneralMode[19] = {14, CHAR_T, CHAR_h, CHAR_u, CHAR_m, CHAR_b, CHAR_n, CHAR_a, CHAR_i, CHAR_l, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
WORD aTHUMBTitleRotateMode[19] = {11, CHAR_R, CHAR_o, CHAR_t, CHAR_a, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
WORD aTHUMBTitleProgramMode[24] = {12, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
WORD aTHUMBTitleProgramList[21] = {12, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_SPACE, CHAR_L, CHAR_i, CHAR_s, CHAR_t};
WORD aTHUMBHelpRotateMode[17] = {9, CHAR_R, CHAR_o, CHAR_t, CHAR_a, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_i, CHAR_t};
WORD aTHUMBHelpProgramMode[16] = {11, CHAR_P, CHAR_r, CHAR_o, CHAR_g, CHAR_r, CHAR_a, CHAR_m, CHAR_SPACE, CHAR_i, CHAR_t, CHAR_SPACE};
WORD aTHUMBHelpProgramList[14] = {9, CHAR_D, CHAR_e, CHAR_l, CHAR_e, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_i, CHAR_t};
WORD aTHUMBButtonRotate[8] = {6, CHAR_R, CHAR_o, CHAR_t, CHAR_a, CHAR_t, CHAR_e};
WORD aTHUMBButtonSlideShow[11] = {10, CHAR_S, CHAR_l, CHAR_i, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_S, CHAR_h, CHAR_o, CHAR_w};
WORD aTHUMBButtonReset[8] = {5, CHAR_R, CHAR_e, CHAR_s, CHAR_e, CHAR_t};
WORD aTHUMBButtonReturn[8] = {6, CHAR_R, CHAR_e, CHAR_t, CHAR_u, CHAR_r, CHAR_n};
WORD aTHUMBButtonProgramList[7] = {4, CHAR_L, CHAR_i, CHAR_s, CHAR_t};
#endif //THUMB_MAIN_FILE									
#ifdef OSDDG_MAIN_FILE									
WORD aDGDlgBtnTrackDigest[21] = {12, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_g, CHAR_e, CHAR_s, CHAR_t};
WORD aDGDlgBtnDiscInterval[21] = {13, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_SPACE, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l};
WORD aDGPage[8] = {5, CHAR_P, CHAR_a, CHAR_g, CHAR_e, CHAR_COLON};
WORD aDGTitleTrackDigest[21] = {12, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_g, CHAR_e, CHAR_s, CHAR_t};
WORD aDGTitleDiscInterval[21] = {13, CHAR_D, CHAR_i, CHAR_s, CHAR_c, CHAR_SPACE, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l};
WORD aDGInfoTrack[9] = {6, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k, CHAR_COLON};
WORD aDGBtnMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aDGBtnExit[8] = {4, CHAR_E, CHAR_x, CHAR_i, CHAR_t};
#endif //OSDDG_MAIN_FILE									
#ifdef OSDPRGM_MAIN_FILE									
WORD aProgram[12] = {7, CHAR_P, CHAR_R, CHAR_O, CHAR_G, CHAR_R, CHAR_A, CHAR_M};
WORD aTitle[7] = {5, CHAR_T, CHAR_i, CHAR_t, CHAR_l, CHAR_e};
WORD aChapter[10] = {7, CHAR_C, CHAR_h, CHAR_a, CHAR_p, CHAR_t, CHAR_e, CHAR_r};
WORD aTrack[8] = {5, CHAR_T, CHAR_r, CHAR_a, CHAR_c, CHAR_k};
WORD aSelect[12] = {6, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t};
WORD aPlay[11] = {4, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aClear[8] = {5, CHAR_C, CHAR_l, CHAR_e, CHAR_a, CHAR_r};
#endif //OSDPRGM_MAIN_FILE									
#ifdef OSDBMARK_MAIN_FILE									
WORD aBookmark[11] = {8, CHAR_B, CHAR_o, CHAR_o, CHAR_k, CHAR_m, CHAR_a, CHAR_r, CHAR_k};
WORD aTime[6] = {4, CHAR_T, CHAR_i, CHAR_m, CHAR_e};
#endif //OSDBMARK_MAIN_FILE									
#ifdef SETUP_MAIN_FILE									
WORD aSetupSelect[12] = {6, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t};
#endif //SETUP_MAIN_FILE									
#ifdef MENU_MAIN_FILE									
WORD aMsgEnglish[10] = {7, CHAR_E, CHAR_n, CHAR_g, CHAR_l, CHAR_i, CHAR_s, CHAR_h};
WORD aMsgChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aMsgJapanese[11] = {8, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aMsgFrench[12] = {6, CHAR_F, CHAR_r, CHAR_e, CHAR_n, CHAR_c, CHAR_h};
WORD aMsgGerman[10] = {6, CHAR_G, CHAR_e, CHAR_r, CHAR_m, CHAR_a, CHAR_n};
WORD aMsgItalian[12] = {7, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n};
WORD aMsgSpanish[11] = {7, CHAR_S, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_h};
WORD aMsgPortuguese[14] = {10, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_u, CHAR_e, CHAR_s, CHAR_e};
WORD aMsgDutch[12] = {5, CHAR_D, CHAR_u, CHAR_t, CHAR_c, CHAR_h};
WORD aMsgPolish[9] = {6, CHAR_P, CHAR_o, CHAR_l, CHAR_i, CHAR_s, CHAR_h};
WORD aMsgHindi[6] = {5, CHAR_H, CHAR_i, CHAR_n, CHAR_d, CHAR_i};
WORD aMsgThai[10] = {4, CHAR_T, CHAR_h, CHAR_a, CHAR_i};
WORD aMsgRussian[9] = {7, CHAR_R, CHAR_u, CHAR_s, CHAR_s, CHAR_i, CHAR_a, CHAR_n};
WORD aMsgKorean[11] = {6, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n};
WORD aMsgCzech[6] = {5, CHAR_C, CHAR_z, CHAR_e, CHAR_c, CHAR_h};
WORD aMsgGreece[7] = {6, CHAR_G, CHAR_r, CHAR_e, CHAR_e, CHAR_c, CHAR_e};
WORD aMsgTurkey[7] = {6, CHAR_T, CHAR_u, CHAR_r, CHAR_k, CHAR_e, CHAR_y};
WORD aMsgOthers[9] = {6, CHAR_O, CHAR_t, CHAR_h, CHAR_e, CHAR_r, CHAR_s};
WORD aMsgOSD[4] = {3, CHAR_O, CHAR_S, CHAR_D};
WORD aMsgMenu[5] = {4, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aMsgAudio[6] = {5, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMsgSubtitle[12] = {8, CHAR_S, CHAR_u, CHAR_b, CHAR_t, CHAR_i, CHAR_t, CHAR_l, CHAR_e};
WORD aMsgPS[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_P, CHAR_S};
WORD aMsgLB[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_L, CHAR_B};
WORD aMsgWide[5] = {4, CHAR_1, CHAR_6, CHAR_COLON, CHAR_9};
WORD aMsgOn[9] = {2, CHAR_O, CHAR_n};
WORD aMsgOff[10] = {3, CHAR_O, CHAR_f, CHAR_f};
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
WORD aMsgBright[10] = {6, CHAR_B, CHAR_r, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aMsgSoft[8] = {4, CHAR_S, CHAR_o, CHAR_f, CHAR_t};
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
WORD aMsgTVType[14] = {7, CHAR_T, CHAR_V, CHAR_SPACE, CHAR_T, CHAR_y, CHAR_p, CHAR_e};
WORD aMsgScreenSaver[18] = {12, CHAR_S, CHAR_c, CHAR_r, CHAR_e, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_S, CHAR_a, CHAR_v, CHAR_e, CHAR_r};
WORD aMsgTVSystem[17] = {9, CHAR_T, CHAR_V, CHAR_SPACE, CHAR_S, CHAR_y, CHAR_s, CHAR_t, CHAR_e, CHAR_m};
WORD aMsgVideoOutput[14] = {12, CHAR_V, CHAR_i, CHAR_d, CHAR_e, CHAR_o, CHAR_SPACE, CHAR_O, CHAR_u, CHAR_t, CHAR_p, CHAR_u, CHAR_t};
WORD aMsgBrightness[11] = {10, CHAR_B, CHAR_r, CHAR_i, CHAR_g, CHAR_h, CHAR_t, CHAR_n, CHAR_e, CHAR_s, CHAR_s};
WORD aMsgContrast[10] = {8, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMsgChromPhase[4] = {3, CHAR_H, CHAR_u, CHAR_e};
WORD aMsgSPDIF_PCM[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_P, CHAR_C, CHAR_M};
WORD aMsgSPDIF_RAW[11] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_R, CHAR_A, CHAR_W};
WORD aMsg2CH[11] = {9, CHAR_2, CHAR_SPACE, CHAR_C, CHAR_h, CHAR_a, CHAR_n, CHAR_n, CHAR_e, CHAR_l};
WORD aMsg51CH[13] = {11, CHAR_5, CHAR_FULL_STOP, CHAR_1, CHAR_SPACE, CHAR_C, CHAR_h, CHAR_a, CHAR_n, CHAR_n, CHAR_e, CHAR_l};
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
WORD aMsgDigitalOut[18] = {11, CHAR_D, CHAR_i, CHAR_g, CHAR_i, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_O, CHAR_u, CHAR_t};
WORD aMsgAnalogOut[18] = {10, CHAR_A, CHAR_n, CHAR_a, CHAR_l, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_O, CHAR_u, CHAR_t};
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
WORD aMsgCenter[10] = {6, CHAR_C, CHAR_e, CHAR_n, CHAR_t, CHAR_e, CHAR_r};
WORD aMsgRear[11] = {4, CHAR_R, CHAR_e, CHAR_a, CHAR_r};
WORD aMsgSubwoofer[10] = {9, CHAR_S, CHAR_u, CHAR_b, CHAR_w, CHAR_o, CHAR_o, CHAR_f, CHAR_e, CHAR_r};
WORD aMsgSpeakerTest[19] = {12, CHAR_S, CHAR_p, CHAR_e, CHAR_a, CHAR_k, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_T, CHAR_e, CHAR_s, CHAR_t};
WORD aMsgLevel1[2] = {1, CHAR_1};
WORD aMsgLevel2[2] = {1, CHAR_2};
WORD aMsgLevel3[2] = {1, CHAR_3};
WORD aMsgLevel4[2] = {1, CHAR_4};
WORD aMsgLevel5[2] = {1, CHAR_5};
WORD aMsgLevel6[2] = {1, CHAR_6};
WORD aMsgLevel7[2] = {1, CHAR_7};
WORD aMsgLevel8[2] = {1, CHAR_8};
WORD aMsgRead[8] = {4, CHAR_R, CHAR_e, CHAR_a, CHAR_d};
WORD aMsgParental[20] = {13, CHAR_P, CHAR_a, CHAR_r, CHAR_e, CHAR_n, CHAR_t, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_C, CHAR_t, CHAR_r, CHAR_l};
WORD aMsgChangePassword[13] = {8, CHAR_P, CHAR_a, CHAR_s, CHAR_s, CHAR_w, CHAR_o, CHAR_r, CHAR_d};
WORD aMsgAngleMark[16] = {10, CHAR_A, CHAR_n, CHAR_g, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_M, CHAR_a, CHAR_r, CHAR_k};
WORD aMsgUIStyle[7] = {5, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e};
WORD aMsgUIStyle1[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_1};
WORD aMsgUIStyle2[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_2};
WORD aMsgUIStyle3[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_3};
WORD aMsgUIStyle4[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_4};
WORD aMsgSmartPlay[14] = {10, CHAR_S, CHAR_m, CHAR_a, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aMsgDivXVOD[9] = {8, CHAR_D, CHAR_i, CHAR_v, CHAR_X, CHAR_SPACE, CHAR_V, CHAR_O, CHAR_D};
WORD aMsgDefaultValue[18] = {7, CHAR_D, CHAR_e, CHAR_f, CHAR_a, CHAR_u, CHAR_l, CHAR_t};
WORD aMsgLanguageSetup[23] = {14, CHAR_L, CHAR_a, CHAR_n, CHAR_g, CHAR_u, CHAR_a, CHAR_g, CHAR_e, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p};
WORD aMsgScreenSetup[22] = {12, CHAR_S, CHAR_c, CHAR_r, CHAR_e, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p};
WORD aMsgAudioSetup[22] = {11, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p};
WORD aMsgCustomSetup[28] = {12, CHAR_C, CHAR_u, CHAR_s, CHAR_t, CHAR_o, CHAR_m, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p};
WORD aMsgExitSetup[30] = {15, CHAR_E, CHAR_x, CHAR_i, CHAR_t, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
WORD aMsgPassword[31] = {21, CHAR_P, CHAR_l, CHAR_e, CHAR_a, CHAR_s, CHAR_e, CHAR_SPACE, CHAR_i, CHAR_n, CHAR_p, CHAR_u, CHAR_t, CHAR_SPACE, CHAR_p, CHAR_a, CHAR_s, CHAR_s, CHAR_w, CHAR_o, CHAR_r, CHAR_d};
WORD aMsgWrongPassword[20] = {14, CHAR_W, CHAR_r, CHAR_o, CHAR_n, CHAR_g, CHAR_SPACE, CHAR_P, CHAR_a, CHAR_s, CHAR_s, CHAR_w, CHAR_o, CHAR_r, CHAR_d};
WORD aMsgOldPassword[9] = {4, CHAR_O, CHAR_l, CHAR_d, CHAR_COLON};
WORD aMsgNewPassword[10] = {4, CHAR_N, CHAR_e, CHAR_w, CHAR_COLON};
WORD aMsgVerifyPassword[12] = {7, CHAR_V, CHAR_e, CHAR_r, CHAR_i, CHAR_f, CHAR_y, CHAR_COLON};
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
WORD aMENUPhotoSetting[26] = {13, CHAR_P, CHAR_h, CHAR_o, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_t, CHAR_i, CHAR_n, CHAR_g};
WORD aMENUIntervalTime[20] = {13, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_v, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_T, CHAR_i, CHAR_m, CHAR_e};
#ifndef MUSTEK_SETUP_MENU									
WORD aMENU5Sec[7] = {6, CHAR_5, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_c, CHAR_FULL_STOP};
WORD aMENU10Sec[8] = {7, CHAR_1, CHAR_0, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_c, CHAR_FULL_STOP};
#endif //MUSTEK_SETUP_MENU									
WORD aMENU15Sec[8] = {7, CHAR_1, CHAR_5, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_c, CHAR_FULL_STOP};
WORD aMENU30Sec[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_c, CHAR_FULL_STOP};
WORD aMENU1Min[7] = {6, CHAR_1, CHAR_SPACE, CHAR_M, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU3Min[7] = {6, CHAR_3, CHAR_SPACE, CHAR_M, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENUTransitionEffect[21] = {17, CHAR_T, CHAR_r, CHAR_a, CHAR_n, CHAR_s, CHAR_i, CHAR_t, CHAR_i, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_E, CHAR_f, CHAR_f, CHAR_e, CHAR_c, CHAR_t};
WORD aMENUJPEGEffectNone[12] = {9, CHAR_N, CHAR_o, CHAR_SPACE, CHAR_E, CHAR_f, CHAR_f, CHAR_e, CHAR_c, CHAR_t};
WORD aMENUJPEGEffectCoverUp[17] = {8, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aMENUJPEGEffectCoverDown[18] = {10, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
#ifndef MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectCoverLeft[16] = {10, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_L, CHAR_e, CHAR_f, CHAR_t};
WORD aMENUJPEGEffectCoverRight[17] = {11, CHAR_C, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aMENUJPEGEffectUncoverUp[18] = {10, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aMENUJPEGEffectUncoverDown[17] = {12, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
#endif //MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectUncoverLeft[16] = {12, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_L, CHAR_e, CHAR_f, CHAR_t};
WORD aMENUJPEGEffectUncoverRight[17] = {13, CHAR_U, CHAR_n, CHAR_c, CHAR_o, CHAR_v, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aMENUJPEGEffectWipeUp[20] = {7, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aMENUJPEGEffectWipeDown[17] = {9, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
#ifndef MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectWipeLeft[17] = {9, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_L, CHAR_e, CHAR_f, CHAR_t};
WORD aMENUJPEGEffectWipeRight[18] = {10, CHAR_W, CHAR_i, CHAR_p, CHAR_e, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aMENUJPEGEffectPushUp[14] = {7, CHAR_P, CHAR_u, CHAR_s, CHAR_h, CHAR_SPACE, CHAR_U, CHAR_p};
WORD aMENUJPEGEffectPushDown[13] = {9, CHAR_P, CHAR_u, CHAR_s, CHAR_h, CHAR_SPACE, CHAR_D, CHAR_o, CHAR_w, CHAR_n};
WORD aMENUJPEGEffectPushLeft[17] = {9, CHAR_P, CHAR_u, CHAR_s, CHAR_h, CHAR_SPACE, CHAR_L, CHAR_e, CHAR_f, CHAR_t};
#endif //MUSTEK_SETUP_MENU									
WORD aMENUJPEGEffectPushRight[18] = {10, CHAR_P, CHAR_u, CHAR_s, CHAR_h, CHAR_SPACE, CHAR_R, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aMENUJPEGEffectFadeThroughBlack[26] = {18, CHAR_F, CHAR_a, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_h, CHAR_r, CHAR_o, CHAR_u, CHAR_g, CHAR_h, CHAR_SPACE, CHAR_B, CHAR_l, CHAR_a, CHAR_c, CHAR_k};
WORD aMENUJPEGEffectRandom[16] = {6, CHAR_R, CHAR_a, CHAR_n, CHAR_d, CHAR_o, CHAR_m};
WORD aMENUBrowse[12] = {6, CHAR_B, CHAR_r, CHAR_o, CHAR_w, CHAR_s, CHAR_e};
WORD aMENUFileManager[16] = {12, CHAR_F, CHAR_i, CHAR_l, CHAR_e, CHAR_SPACE, CHAR_M, CHAR_a, CHAR_n, CHAR_a, CHAR_g, CHAR_e, CHAR_r};
WORD aMENUSetupMenu[16] = {10, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p, CHAR_SPACE, CHAR_M, CHAR_e, CHAR_n, CHAR_u};
WORD aMENUThumbnail[13] = {9, CHAR_T, CHAR_h, CHAR_u, CHAR_m, CHAR_b, CHAR_n, CHAR_a, CHAR_i, CHAR_l};
WORD aMENUAutoPlay[14] = {9, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_p, CHAR_l, CHAR_a, CHAR_y};
WORD aMENUPhoto[12] = {5, CHAR_P, CHAR_h, CHAR_o, CHAR_t, CHAR_o};
WORD aMENUMusic[7] = {5, CHAR_M, CHAR_u, CHAR_s, CHAR_i, CHAR_c};
WORD aMENUPhotoAudio[14] = {11, CHAR_P, CHAR_h, CHAR_o, CHAR_t, CHAR_o, CHAR_SOLIDUS, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aMENUMovie[10] = {5, CHAR_M, CHAR_o, CHAR_v, CHAR_i, CHAR_e};
WORD aMENUDisplaySetup[16] = {13, CHAR_D, CHAR_i, CHAR_s, CHAR_p, CHAR_l, CHAR_a, CHAR_y, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p};
WORD aMENUDisplayType[14] = {12, CHAR_D, CHAR_i, CHAR_s, CHAR_p, CHAR_l, CHAR_a, CHAR_y, CHAR_SPACE, CHAR_T, CHAR_y, CHAR_p, CHAR_e};
WORD aMENUTVStandard[12] = {11, CHAR_T, CHAR_V, CHAR_SPACE, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_r, CHAR_d};
WORD aMENUNTSC[5] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aMENUPAL[4] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aMENUAUTO[11] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aMENUBrightness[11] = {10, CHAR_B, CHAR_r, CHAR_i, CHAR_g, CHAR_h, CHAR_t, CHAR_n, CHAR_e, CHAR_s, CHAR_s};
WORD aMENUContrast[10] = {8, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMENUNormal[9] = {6, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l};
WORD aMENUBright[10] = {6, CHAR_B, CHAR_r, CHAR_i, CHAR_g, CHAR_h, CHAR_t};
WORD aMENUSoft[8] = {4, CHAR_S, CHAR_o, CHAR_f, CHAR_t};
WORD aMENUTFTBrightness[15] = {14, CHAR_T, CHAR_F, CHAR_T, CHAR_SPACE, CHAR_B, CHAR_r, CHAR_i, CHAR_g, CHAR_h, CHAR_t, CHAR_n, CHAR_e, CHAR_s, CHAR_s};
WORD aMENUTFTContrast[14] = {12, CHAR_T, CHAR_F, CHAR_T, CHAR_SPACE, CHAR_C, CHAR_o, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_s, CHAR_t};
WORD aMENUTFTSaturation[16] = {14, CHAR_T, CHAR_F, CHAR_T, CHAR_SPACE, CHAR_S, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_a, CHAR_t, CHAR_i, CHAR_o, CHAR_n};
WORD aMENURepeatMode[15] = {11, CHAR_R, CHAR_e, CHAR_p, CHAR_e, CHAR_a, CHAR_t, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
WORD aMENURepeatOne[9] = {3, CHAR_O, CHAR_n, CHAR_e};
WORD aMENURepeatFolder[9] = {6, CHAR_F, CHAR_o, CHAR_l, CHAR_d, CHAR_e, CHAR_r};
WORD aMENURepeatAll[10] = {3, CHAR_A, CHAR_l, CHAR_l};
WORD aMENUCustomSetup[28] = {12, CHAR_C, CHAR_u, CHAR_s, CHAR_t, CHAR_o, CHAR_m, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p};
WORD aMENUOSDLanguage[14] = {12, CHAR_O, CHAR_S, CHAR_D, CHAR_SPACE, CHAR_L, CHAR_a, CHAR_n, CHAR_g, CHAR_u, CHAR_a, CHAR_g, CHAR_e};
WORD aMENUEnglish[10] = {7, CHAR_E, CHAR_n, CHAR_g, CHAR_l, CHAR_i, CHAR_s, CHAR_h};
WORD aMENUChinese[11] = {7, CHAR_C, CHAR_h, CHAR_i, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aMENUJapanese[11] = {8, CHAR_J, CHAR_a, CHAR_p, CHAR_a, CHAR_n, CHAR_e, CHAR_s, CHAR_e};
WORD aMENUKorean[11] = {6, CHAR_K, CHAR_o, CHAR_r, CHAR_e, CHAR_a, CHAR_n};
WORD aMENUFrench[12] = {6, CHAR_F, CHAR_r, CHAR_e, CHAR_n, CHAR_c, CHAR_h};
WORD aMENUGerman[10] = {6, CHAR_G, CHAR_e, CHAR_r, CHAR_m, CHAR_a, CHAR_n};
WORD aMENUItalian[12] = {7, CHAR_I, CHAR_t, CHAR_a, CHAR_l, CHAR_i, CHAR_a, CHAR_n};
WORD aMENUSpanish[11] = {7, CHAR_S, CHAR_p, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_h};
WORD aMENUPortuguese[14] = {10, CHAR_P, CHAR_o, CHAR_r, CHAR_t, CHAR_u, CHAR_g, CHAR_u, CHAR_e, CHAR_s, CHAR_e};
WORD aMENUDutch[12] = {5, CHAR_D, CHAR_u, CHAR_t, CHAR_c, CHAR_h};
WORD aMENUPolish[9] = {6, CHAR_P, CHAR_o, CHAR_l, CHAR_i, CHAR_s, CHAR_h};
WORD aMENUUIStyle[7] = {5, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e};
WORD aMENUUIStyle1[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_1};
WORD aMENUUIStyle2[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_2};
WORD aMENUUIStyle3[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_3};
WORD aMENUUIStyle4[8] = {6, CHAR_S, CHAR_t, CHAR_y, CHAR_l, CHAR_e, CHAR_4};
WORD aMENUDefault[18] = {7, CHAR_D, CHAR_e, CHAR_f, CHAR_a, CHAR_u, CHAR_l, CHAR_t};
WORD aMENURead[8] = {4, CHAR_R, CHAR_e, CHAR_a, CHAR_d};
WORD aMENUIconExit[6] = {4, CHAR_E, CHAR_x, CHAR_i, CHAR_t};
WORD aMENUHelpSelect[10] = {9, CHAR_SPACE, CHAR_COLON, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t};
WORD aMENUHelpConfirm[11] = {9, CHAR_COLON, CHAR_SPACE, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_r, CHAR_m};
WORD aMENUHelpExit[18] = {17, CHAR_E, CHAR_x, CHAR_i, CHAR_t, CHAR_COLON, CHAR_SPACE, CHAR_S, CHAR_a, CHAR_v, CHAR_e, CHAR_SPACE, CHAR_AMPERSAND, CHAR_SPACE, CHAR_E, CHAR_x, CHAR_i, CHAR_t};
#endif //#ifdef IMAGE_FRAME_MENU_MAIN_FILE									
#ifdef SUPPORT_POWERON_MENU									
WORD aPOWERONIconPhoto[8] = {5, CHAR_P, CHAR_h, CHAR_o, CHAR_t, CHAR_o};
WORD aPOWERONIconMusic[7] = {5, CHAR_M, CHAR_u, CHAR_s, CHAR_i, CHAR_c};
WORD aPOWERONIconPhotoMusic[15] = {11, CHAR_P, CHAR_h, CHAR_o, CHAR_t, CHAR_o, CHAR_PLUS_SIGN, CHAR_M, CHAR_u, CHAR_s, CHAR_i, CHAR_c};
WORD aPOWERONIconMovie[10] = {5, CHAR_M, CHAR_o, CHAR_v, CHAR_i, CHAR_e};
WORD aPOWERONIconSetup[14] = {5, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p};
WORD aPOWERONIconCalendar[11] = {8, CHAR_C, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_a, CHAR_r};
WORD aPOWERONIconClock[9] = {5, CHAR_C, CHAR_l, CHAR_o, CHAR_c, CHAR_k};
WORD aPOWERONIconAlarm[7] = {5, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m};
WORD aPOWERONIconAutoPower[14] = {10, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_P, CHAR_o, CHAR_w, CHAR_e, CHAR_r};
WORD aPOWERONIconEdit[11] = {4, CHAR_E, CHAR_d, CHAR_i, CHAR_t};
WORD aPOWERONIconFavorite[10] = {8, CHAR_F, CHAR_a, CHAR_v, CHAR_o, CHAR_r, CHAR_i, CHAR_t, CHAR_e};
WORD aPOWERONIconRadio[6] = {5, CHAR_R, CHAR_a, CHAR_d, CHAR_i, CHAR_o};
WORD aWeeklyDay_Mon[4] = {3, CHAR_M, CHAR_o, CHAR_n};
WORD aWeeklyDay_Tue[4] = {3, CHAR_T, CHAR_u, CHAR_e};
WORD aWeeklyDay_Wed[4] = {3, CHAR_W, CHAR_e, CHAR_d};
WORD aWeeklyDay_Thu[4] = {3, CHAR_T, CHAR_h, CHAR_u};
WORD aWeeklyDay_Fri[4] = {3, CHAR_F, CHAR_r, CHAR_i};
WORD aWeeklyDay_Sat[4] = {3, CHAR_S, CHAR_a, CHAR_t};
WORD aWeeklyDay_Sun[4] = {3, CHAR_S, CHAR_u, CHAR_n};
WORD aMonthJanuary[4] = {3, CHAR_J, CHAR_a, CHAR_n};
WORD aMonthFebruary[4] = {3, CHAR_F, CHAR_e, CHAR_b};
WORD aMonthMarch[4] = {3, CHAR_M, CHAR_a, CHAR_r};
WORD aMonthApril[4] = {3, CHAR_A, CHAR_p, CHAR_r};
WORD aMonthMay[4] = {3, CHAR_M, CHAR_a, CHAR_y};
WORD aMonthJune[4] = {3, CHAR_J, CHAR_u, CHAR_n};
WORD aMonthJuly[4] = {3, CHAR_J, CHAR_u, CHAR_l};
WORD aMonthAugust[4] = {3, CHAR_A, CHAR_u, CHAR_g};
WORD aMonthSeptember[4] = {3, CHAR_S, CHAR_e, CHAR_p};
WORD aMonthOctober[4] = {3, CHAR_O, CHAR_c, CHAR_t};
WORD aMonthNovember[4] = {3, CHAR_N, CHAR_o, CHAR_v};
WORD aMonthDecember[4] = {3, CHAR_D, CHAR_e, CHAR_c};
WORD aWeeklyMonday[14] = {6, CHAR_M, CHAR_o, CHAR_n, CHAR_d, CHAR_a, CHAR_y};
WORD aWeeklyTuesday[12] = {7, CHAR_T, CHAR_u, CHAR_e, CHAR_s, CHAR_d, CHAR_a, CHAR_y};
WORD aWeeklyWednesday[13] = {9, CHAR_W, CHAR_e, CHAR_d, CHAR_n, CHAR_e, CHAR_s, CHAR_d, CHAR_a, CHAR_y};
WORD aWeeklyThursday[13] = {8, CHAR_T, CHAR_h, CHAR_u, CHAR_r, CHAR_s, CHAR_d, CHAR_a, CHAR_y};
WORD aWeeklyFriday[12] = {6, CHAR_F, CHAR_r, CHAR_i, CHAR_d, CHAR_a, CHAR_y};
WORD aWeeklySaturday[9] = {8, CHAR_S, CHAR_a, CHAR_t, CHAR_u, CHAR_r, CHAR_d, CHAR_a, CHAR_y};
WORD aWeeklySunday[10] = {6, CHAR_S, CHAR_u, CHAR_n, CHAR_d, CHAR_a, CHAR_y};
WORD aCLOCKClockSetting[22] = {13, CHAR_C, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_t, CHAR_i, CHAR_n, CHAR_g};
WORD aCLOCKDisplay[13] = {7, CHAR_D, CHAR_i, CHAR_s, CHAR_p, CHAR_l, CHAR_a, CHAR_y};
WORD aCLOCKSetTime[14] = {8, CHAR_S, CHAR_e, CHAR_t, CHAR_SPACE, CHAR_T, CHAR_i, CHAR_m, CHAR_e};
WORD aCLOCKExit[6] = {4, CHAR_E, CHAR_x, CHAR_i, CHAR_t};
WORD aCLOCKBigClock[9] = {5, CHAR_C, CHAR_l, CHAR_o, CHAR_c, CHAR_k};
WORD aCLOCKClockCalendar[22] = {16, CHAR_C, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_AMPERSAND, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_l, CHAR_e, CHAR_n, CHAR_d, CHAR_a, CHAR_r};
WORD aBUTTONOK[3] = {2, CHAR_O, CHAR_K};
WORD aBUTTONCancel[9] = {6, CHAR_C, CHAR_a, CHAR_n, CHAR_c, CHAR_e, CHAR_l};
WORD aSETTIMESetTime[15] = {8, CHAR_S, CHAR_e, CHAR_t, CHAR_SPACE, CHAR_T, CHAR_i, CHAR_m, CHAR_e};
WORD aALARMEnable[10] = {6, CHAR_E, CHAR_n, CHAR_a, CHAR_b, CHAR_l, CHAR_e};
WORD aALARMAlarmSetting[21] = {13, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_t, CHAR_i, CHAR_n, CHAR_g};
WORD aALARMDlgTitle[8] = {5, CHAR_A, CHAR_l, CHAR_a, CHAR_r, CHAR_m};
WORD aALARMDlgContent[34] = {32, CHAR_P, CHAR_r, CHAR_e, CHAR_s, CHAR_s, CHAR_SPACE, CHAR_a, CHAR_n, CHAR_y, CHAR_SPACE, CHAR_k, CHAR_e, CHAR_y, CHAR_SPACE, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_s, CHAR_t, CHAR_o, CHAR_p, CHAR_SPACE, CHAR_t, CHAR_h, CHAR_e, CHAR_SPACE, CHAR_a, CHAR_l, CHAR_a, CHAR_r, CHAR_m, CHAR_FULL_STOP};
WORD aAUTOPWRAutoPowerSetting[46] = {25, CHAR_A, CHAR_u, CHAR_t, CHAR_o, CHAR_HYPHEN_MINUS, CHAR_P, CHAR_o, CHAR_w, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_O, CHAR_n, CHAR_SOLIDUS, CHAR_O, CHAR_f, CHAR_f, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_t, CHAR_i, CHAR_n, CHAR_g};
WORD aAUTOPWRPowerOn[11] = {8, CHAR_P, CHAR_o, CHAR_w, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_O, CHAR_n};
WORD aAUTOPWRPowerOff[12] = {9, CHAR_P, CHAR_o, CHAR_w, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
WORD aAUTOPWRPowerOnEnable[30] = {15, CHAR_P, CHAR_o, CHAR_w, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_O, CHAR_n, CHAR_SPACE, CHAR_E, CHAR_n, CHAR_a, CHAR_b, CHAR_l, CHAR_e};
WORD aAUTOPWRPowerOffEnable[33] = {16, CHAR_P, CHAR_o, CHAR_w, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f, CHAR_SPACE, CHAR_E, CHAR_n, CHAR_a, CHAR_b, CHAR_l, CHAR_e};
WORD aAUTOPWRSetTime[14] = {8, CHAR_S, CHAR_e, CHAR_t, CHAR_SPACE, CHAR_T, CHAR_i, CHAR_m, CHAR_e};
WORD aMMBtnSave[10] = {4, CHAR_S, CHAR_a, CHAR_v, CHAR_e};
WORD aMMSaveFileDlgTitle[20] = {9, CHAR_S, CHAR_a, CHAR_v, CHAR_e, CHAR_SPACE, CHAR_F, CHAR_i, CHAR_l, CHAR_e};
WORD aMMSaveFileDlgContent[18] = {11, CHAR_P, CHAR_l, CHAR_e, CHAR_a, CHAR_s, CHAR_e, CHAR_SPACE, CHAR_W, CHAR_a, CHAR_i, CHAR_t};
WORD aTHUMBButtonSort[10] = {4, CHAR_S, CHAR_o, CHAR_r, CHAR_t};
WORD aTHUMBButtonDeleteAll[11] = {10, CHAR_D, CHAR_e, CHAR_l, CHAR_e, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_A, CHAR_l, CHAR_l};
WORD aTHUMBTitleEditMode[18] = {9, CHAR_E, CHAR_d, CHAR_i, CHAR_t, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
WORD aTHUMBTitleDeleteMode[19] = {11, CHAR_D, CHAR_e, CHAR_l, CHAR_e, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
WORD aTHUMBHelpDeleteMode[14] = {9, CHAR_D, CHAR_e, CHAR_l, CHAR_e, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_i, CHAR_t};
WORD aTHUMBHelpSortMode[26] = {17, CHAR_F, CHAR_o, CHAR_c, CHAR_u, CHAR_s, CHAR_SPACE, CHAR_t, CHAR_h, CHAR_e, CHAR_SPACE, CHAR_p, CHAR_i, CHAR_c, CHAR_t, CHAR_u, CHAR_r, CHAR_e};
WORD aTHUMBTitleSortMode[18] = {9, CHAR_S, CHAR_o, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
WORD aEDITDialogTitle[27] = {11, CHAR_E, CHAR_d, CHAR_i, CHAR_t, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_a, CHAR_l, CHAR_o, CHAR_g};
WORD aEDITDialogContent[38] = {23, CHAR_P, CHAR_l, CHAR_e, CHAR_a, CHAR_s, CHAR_e, CHAR_SPACE, CHAR_s, CHAR_e, CHAR_l, CHAR_e, CHAR_c, CHAR_t, CHAR_SPACE, CHAR_E, CHAR_d, CHAR_i, CHAR_t, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
WORD aBUTTONCopy[9] = {4, CHAR_C, CHAR_o, CHAR_p, CHAR_y};
WORD aBUTTONDelete[9] = {6, CHAR_D, CHAR_e, CHAR_l, CHAR_e, CHAR_t, CHAR_e};
WORD aSETDATESetDate[17] = {8, CHAR_S, CHAR_e, CHAR_t, CHAR_SPACE, CHAR_D, CHAR_a, CHAR_t, CHAR_e};
#endif //#ifdef SUPPORT_POWERON_MENU									
WORD aMENUOn[5] = {2, CHAR_O, CHAR_n};
WORD aMENUOff[7] = {3, CHAR_O, CHAR_f, CHAR_f};
#ifdef MUSTEK_SETUP_MENU									
WORD aMENU5Min[7] = {6, CHAR_5, CHAR_SPACE, CHAR_M, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU30Min[8] = {7, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_M, CHAR_i, CHAR_n, CHAR_FULL_STOP};
WORD aMENU1Hour[8] = {6, CHAR_1, CHAR_SPACE, CHAR_H, CHAR_o, CHAR_u, CHAR_r};
WORD aMENU4Hours[9] = {7, CHAR_4, CHAR_SPACE, CHAR_H, CHAR_o, CHAR_u, CHAR_r, CHAR_s};
WORD aMENU12Hours[10] = {8, CHAR_1, CHAR_2, CHAR_SPACE, CHAR_H, CHAR_o, CHAR_u, CHAR_r, CHAR_s};
WORD aMENU1Day[9] = {5, CHAR_1, CHAR_SPACE, CHAR_D, CHAR_a, CHAR_y};
WORD aMENUTransition[14] = {10, CHAR_T, CHAR_r, CHAR_a, CHAR_n, CHAR_s, CHAR_i, CHAR_t, CHAR_i, CHAR_o, CHAR_n};
WORD aMENUTVSystem[17] = {9, CHAR_T, CHAR_V, CHAR_SPACE, CHAR_S, CHAR_y, CHAR_s, CHAR_t, CHAR_e, CHAR_m};
WORD aMENUFullScreen[15] = {11, CHAR_F, CHAR_u, CHAR_l, CHAR_l, CHAR_SPACE, CHAR_S, CHAR_c, CHAR_r, CHAR_e, CHAR_e, CHAR_n};
WORD aMENUReset[8] = {5, CHAR_R, CHAR_e, CHAR_s, CHAR_e, CHAR_t};
WORD aMENUConfirm[8] = {7, CHAR_C, CHAR_o, CHAR_n, CHAR_f, CHAR_i, CHAR_r, CHAR_m};
WORD aMENUExitSetup[30] = {15, CHAR_E, CHAR_x, CHAR_i, CHAR_t, CHAR_SPACE, CHAR_S, CHAR_e, CHAR_t, CHAR_u, CHAR_p, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
#endif //MUSTEK_SETUP_MENU											
WORD aBUTTONYes[4] = {3, CHAR_Y, CHAR_e, CHAR_s};
WORD aBUTTONNo[5] = {2, CHAR_N, CHAR_o};
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW											
WORD aMMSaveDlgTitle[31] = {17, CHAR_S, CHAR_a, CHAR_v, CHAR_e, CHAR_SPACE, CHAR_P, CHAR_h, CHAR_o, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_a, CHAR_l, CHAR_o, CHAR_g};
WORD aMMDeleteDlgTitle[31] = {19, CHAR_D, CHAR_e, CHAR_l, CHAR_e, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_P, CHAR_h, CHAR_o, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_a, CHAR_l, CHAR_o, CHAR_g};
WORD aMMSaveDlgContent[14] = {5, CHAR_S, CHAR_a, CHAR_v, CHAR_e, CHAR_QUESTION_MARK};
WORD aMMDeleteDlgContent[13] = {7, CHAR_D, CHAR_e, CHAR_l, CHAR_e, CHAR_t, CHAR_e, CHAR_QUESTION_MARK};
#endif //#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW									
#ifdef SUPPORT_CHOOSE_MEDIA									
WORD aMENUMedia[11] = {5, CHAR_M, CHAR_e, CHAR_d, CHAR_i, CHAR_a};
#endif //SUPPORT_CHOOSE_MEDIA									
#ifdef SUPPORT_PHOTO_BROWSE_MODE									
WORD aNDBrowseMode[26] = {11, CHAR_B, CHAR_r, CHAR_o, CHAR_w, CHAR_s, CHAR_e, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e};
#endif //SUPPORT_PHOTO_BROWSE_MODE									
#ifdef SUPPORT_TFT_DIGITAL_MODE									
WORD aMENUAVOutput[13] = {9, CHAR_A, CHAR_V, CHAR_SPACE, CHAR_O, CHAR_u, CHAR_t, CHAR_p, CHAR_u, CHAR_t};
#endif //SUPPORT_TFT_DIGITAL_MODE									
WORD aMENUPanel[9] = {5, CHAR_P, CHAR_a, CHAR_n, CHAR_e, CHAR_l};
#ifdef SUPPORT_RADIO_TUNER									
WORD aRADIORecord[9] = {6, CHAR_R, CHAR_e, CHAR_c, CHAR_o, CHAR_r, CHAR_d};
WORD aRADIOScan[9] = {4, CHAR_S, CHAR_c, CHAR_a, CHAR_n};
WORD aRADIOStationsFound[17] = {15, CHAR_s, CHAR_t, CHAR_a, CHAR_t, CHAR_i, CHAR_o, CHAR_n, CHAR_s, CHAR_SPACE, CHAR_f, CHAR_o, CHAR_u, CHAR_n, CHAR_d, CHAR_FULL_STOP};
#endif //SUPPORT_RADIO_TUNER									
#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									
WORD aCODEWesternEuropean[17] = {16, CHAR_W, CHAR_e, CHAR_s, CHAR_t, CHAR_e, CHAR_r, CHAR_n, CHAR_SPACE, CHAR_E, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_e, CHAR_a, CHAR_n};
#endif   // SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN									
#ifdef SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									
WORD aCODECentralEuropean[19] = {16, CHAR_C, CHAR_e, CHAR_n, CHAR_t, CHAR_r, CHAR_a, CHAR_l, CHAR_SPACE, CHAR_E, CHAR_u, CHAR_r, CHAR_o, CHAR_p, CHAR_e, CHAR_a, CHAR_n};
#endif   // SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN									
#ifdef SUPPORT_CHAR_ENCODING_GREEK									
WORD aCODEGreek[11] = {5, CHAR_G, CHAR_r, CHAR_e, CHAR_e, CHAR_k};
#endif   // SUPPORT_CHAR_ENCODING_GREEK									
#ifdef SUPPORT_CHAR_ENCODING_HEBREW									
WORD aCODEHebrew[10] = {6, CHAR_H, CHAR_e, CHAR_b, CHAR_r, CHAR_e, CHAR_w};
#endif   // SUPPORT_CHAR_ENCODING_HEBREW									
#ifdef SUPPORT_CHAR_ENCODING_TURKISH									
WORD aCODETurkish[9] = {7, CHAR_T, CHAR_u, CHAR_r, CHAR_k, CHAR_i, CHAR_s, CHAR_h};
#endif   // SUPPORT_CHAR_ENCODING_TURKISH									
#ifdef SUPPORT_CHAR_ENCODING_CP1250									
WORD aCODECP1250[8] = {7, CHAR_C, CHAR_P, CHAR_SPACE, CHAR_1, CHAR_2, CHAR_5, CHAR_0};
#endif   // SUPPORT_CHAR_ENCODING_CP1250									
#ifdef SUPPORT_CHAR_ENCODING_CYRILLIC									
WORD aCODECyrillic[11] = {8, CHAR_C, CHAR_y, CHAR_r, CHAR_i, CHAR_l, CHAR_l, CHAR_i, CHAR_c};
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
