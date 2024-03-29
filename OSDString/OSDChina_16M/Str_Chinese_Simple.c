#include "stringdec.h"
#ifdef OSDDSPLY_MAIN_FILE			
WORD aDSPLYLanguageJapanese[9] = {2, CHAR_65E5, CHAR_6587};
WORD aDSPLYLanguageEnglish[8] = {2, CHAR_82F1, CHAR_6587};
WORD aDSPLYLanguageFrench[7] = {2, CHAR_6CD5, CHAR_6587};
WORD aDSPLYLanguageGerman[7] = {2, CHAR_5FB7, CHAR_6587};
WORD aDSPLYLanguageRussian[8] = {2, CHAR_4FC4, CHAR_6587};
WORD aDSPLYLanguageChinese[8] = {2, CHAR_4E2D, CHAR_6587};
WORD aDSPLYLanguageKorean[7] = {2, CHAR_97E9, CHAR_6587};
WORD aDSPLYLanguageHindi[6] = {3, CHAR_5370, CHAR_5EA6, CHAR_6587};
WORD aDSPLYLanguageThai[5] = {2, CHAR_6CF0, CHAR_6587};
WORD aDSPLYLanguageItalian[8] = {3, CHAR_610F, CHAR_5927, CHAR_5229};
WORD aDSPLYLanguageSpanish[8] = {3, CHAR_897F, CHAR_73ED, CHAR_7259};
WORD aDSPLYLanguagePortuguese[11] = {3, CHAR_8461, CHAR_8404, CHAR_7259};
WORD aDSPLYLanguageOthers[7] = {2, CHAR_5176, CHAR_4ED6};
WORD aDSPLYOff[4] = {1, CHAR_5173};
#ifndef NO_DVD_DISPLAY_UI			
WORD aDSPLYTitleElapsed[14] = {4, CHAR_64AD, CHAR_653E, CHAR_65F6, CHAR_95F4};
WORD aDSPLYTitleRemain[13] = {6, CHAR_6807, CHAR_9898, CHAR_5269, CHAR_4F59, CHAR_65F6, CHAR_95F4};
WORD aDSPLYChapterElapsed[16] = {4, CHAR_7AE0, CHAR_8282, CHAR_65F6, CHAR_95F4};
WORD aDSPLYChapterRemain[15] = {6, CHAR_7AE0, CHAR_8282, CHAR_5269, CHAR_4F59, CHAR_65F6, CHAR_95F4};
#endif //NO_DVD_DISPLAY_UI			
WORD aDSPLYSingleElapsed[15] = {4, CHAR_66F2, CHAR_76EE, CHAR_65F6, CHAR_95F4};
WORD aDSPLYSingleRemain[14] = {6, CHAR_5355, CHAR_66F2, CHAR_5269, CHAR_4F59, CHAR_65F6, CHAR_95F4};
WORD aDSPLYTotalElapsed[14] = {4, CHAR_64AD, CHAR_653E, CHAR_65F6, CHAR_95F4};
WORD aDSPLYTotalRemain[13] = {4, CHAR_5269, CHAR_4F59, CHAR_65F6, CHAR_95F4};
WORD aDSPLYRootMenu[10] = {5, CHAR_D, CHAR_V, CHAR_D, CHAR_9009, CHAR_5355};
WORD aDSPLYTitleMenu[11] = {4, CHAR_6807, CHAR_9898, CHAR_83DC, CHAR_5355};
#ifdef SUPPORT_NAVIGATOR			
WORD aDSPLYRepeatTitle[13] = {4, CHAR_91CD, CHAR_590D, CHAR_6807, CHAR_9898};
WORD aDSPLYRepeatChapter[15] = {4, CHAR_91CD, CHAR_590D, CHAR_7AE0, CHAR_8282};
WORD aDSPLYRepeatDisc[12] = {6, CHAR_91CD, CHAR_590D, CHAR_D, CHAR_i, CHAR_s, CHAR_c};
WORD aDSPLYRepeatOff[11] = {4, CHAR_91CD, CHAR_590D, CHAR_53D6, CHAR_6D88};
#endif //SUPPORT_NAVIGATOR			
WORD aDSPLYpscan[7] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aDSPLYInterlace[10] = {9, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_l, CHAR_a, CHAR_c, CHAR_e};
WORD aDSPLYResolution[12] = {11, CHAR_R, CHAR_e, CHAR_s, CHAR_o, CHAR_l, CHAR_u, CHAR_t, CHAR_i, CHAR_o, CHAR_n, CHAR_COLON};
WORD aDSPLYDate[6] = {5, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_COLON};
#ifdef SUPPORT_NAVIGATOR			
WORD aNGTitle[7] = {3, CHAR_6807, CHAR_9898, CHAR_COLON};
WORD aNGChapter[9] = {3, CHAR_7AE0, CHAR_8282, CHAR_COLON};
WORD aNGAngle[7] = {3, CHAR_89D2, CHAR_5EA6, CHAR_COLON};
WORD aNGAudio[7] = {3, CHAR_58F0, CHAR_97F3, CHAR_COLON};
WORD aNGSubtitle1[11] = {4, CHAR_5B57, CHAR_5E55, CHAR_1, CHAR_COLON};
WORD aNGSubtitle2[11] = {4, CHAR_5B57, CHAR_5E55, CHAR_2, CHAR_COLON};
#endif //SUPPORT_NAVIGATOR			
#endif //OSDDSPLY_MAIN_FILE			
#ifdef OSDND_MAIN_FILE			
WORD aNDNoDisc[8] = {3, CHAR_65E0, CHAR_789F, CHAR_7247};
WORD aNDNoUSBDevice[14] = {5, CHAR_65E0, CHAR_U, CHAR_S, CHAR_B, CHAR_SPACE};
WORD aNDOpen[5] = {1, CHAR_5F00};
WORD aNDClose[6] = {1, CHAR_5173};
WORD aNDReturn[7] = {2, CHAR_8FD4, CHAR_56DE};
WORD aNDFreeze[7] = {6, CHAR_F, CHAR_r, CHAR_e, CHAR_e, CHAR_z, CHAR_e};
WORD aNDRandomOn[12] = {4, CHAR_968F, CHAR_673A, CHAR_64AD, CHAR_653E};
WORD aNDRandomOff[11] = {4, CHAR_6B63, CHAR_5E38, CHAR_64AD, CHAR_653E};
WORD aNDResume[7] = {4, CHAR_6062, CHAR_590D, CHAR_64AD, CHAR_653E};
WORD aNDRootMenu[10] = {5, CHAR_D, CHAR_V, CHAR_D, CHAR_9009, CHAR_5355};
WORD aNDTitleMenu[11] = {4, CHAR_6807, CHAR_9898, CHAR_9009, CHAR_5355};
WORD aNDRotateRight[17] = {7, CHAR_5411, CHAR_53F3, CHAR_65CB, CHAR_8F6C, CHAR_9, CHAR_0, CHAR_N_DEGREE};
WORD aNDRotateLeft[16] = {8, CHAR_5411, CHAR_5DE6, CHAR_65CB, CHAR_8F6C, CHAR_SPACE, CHAR_9, CHAR_0, CHAR_N_DEGREE};
WORD aNDFlipHorizontally[18] = {4, CHAR_6C34, CHAR_5E73, CHAR_7FFB, CHAR_8F6C};
WORD aNDFlipVertically[16] = {4, CHAR_5782, CHAR_76F4, CHAR_7FFB, CHAR_8F6C};
WORD aNDpscan[7] = {6, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aNDInterlace[10] = {9, CHAR_I, CHAR_n, CHAR_t, CHAR_e, CHAR_r, CHAR_l, CHAR_a, CHAR_c, CHAR_e};
WORD aNDRepeatNone[4] = {2, CHAR_53D6, CHAR_6D88};
WORD aNDRepeatTrack[6] = {2, CHAR_5355, CHAR_66F2};
WORD aNDRepeatDisc[5] = {2, CHAR_789F, CHAR_7247};
WORD aNDRepeatAll[4] = {2, CHAR_5168, CHAR_90E8};
WORD aNDRepeatTitle[6] = {2, CHAR_6807, CHAR_9898};
WORD aNDRepeatChapter[8] = {2, CHAR_7AE0, CHAR_8282};
WORD aNDRepeatFolder[7] = {2, CHAR_76EE, CHAR_5F55};
WORD aNDRepeatA[9] = {3, CHAR_91CD, CHAR_590D, CHAR_A};
WORD aNDRepeatAB[11] = {5, CHAR_91CD, CHAR_590D, CHAR_A, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aNDRepeatOff[11] = {4, CHAR_91CD, CHAR_590D, CHAR_53D6, CHAR_6D88};
#ifndef NO_DISC_MODE			
WORD aNDNavigateOn[12] = {11, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_a, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_O, CHAR_n};
WORD aNDNavigateOff[13] = {12, CHAR_N, CHAR_a, CHAR_v, CHAR_i, CHAR_g, CHAR_a, CHAR_t, CHAR_e, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
WORD aNDPBCOn[7] = {6, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_O, CHAR_n};
WORD aNDPBCOff[8] = {7, CHAR_P, CHAR_B, CHAR_C, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
#endif //#ifndef NO_DISC_MODE			
WORD aNDZoom1[7] = {4, CHAR_653E, CHAR_5927, CHAR_53D6, CHAR_6D88};
WORD aNDZoom2[7] = {4, CHAR_653E, CHAR_5927, CHAR_x, CHAR_2};
WORD aNDZoom4[7] = {4, CHAR_653E, CHAR_5927, CHAR_x, CHAR_4};
WORD aNDMoveLeft[10] = {3, CHAR_5411, CHAR_5DE6, CHAR_8F6C};
WORD aNDMoveRight[11] = {3, CHAR_5411, CHAR_53F3, CHAR_8F6C};
WORD aNDMoveUp[8] = {3, CHAR_5411, CHAR_4E0A, CHAR_8F6C};
WORD aNDMoveDown[10] = {3, CHAR_5411, CHAR_4E0B, CHAR_8F6C};
WORD aNDVocalOn[9] = {4, CHAR_539F, CHAR_5531, CHAR_5408, CHAR_58F0};
WORD aNDVocalOff[10] = {4, CHAR_539F, CHAR_5531, CHAR_6D88, CHAR_58F0};
WORD aNDVocalLL[9] = {3, CHAR_63A5, CHAR_5531, CHAR_5DE6};
WORD aNDVocalRR[9] = {4, CHAR_63A5, CHAR_5531, CHAR_53F3, CHAR_SPACE};
WORD aNDKaraokeOn[11] = {10, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_O, CHAR_n};
WORD aNDKaraokeOff[12] = {11, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
WORD aNDJPEGEffectNone[10] = {5, CHAR_65E0, CHAR_6D4F, CHAR_89C8, CHAR_6548, CHAR_679C};
WORD aNDJPEGEffectCoverUp[9] = {4, CHAR_5411, CHAR_4E0A, CHAR_8986, CHAR_76D6};
WORD aNDJPEGEffectCoverDown[11] = {4, CHAR_5411, CHAR_4E0B, CHAR_8986, CHAR_76D6};
WORD aNDJPEGEffectCoverLeft[11] = {4, CHAR_5411, CHAR_5DE6, CHAR_8986, CHAR_76D6};
WORD aNDJPEGEffectCoverRight[12] = {4, CHAR_5411, CHAR_53F3, CHAR_8986, CHAR_76D6};
WORD aNDJPEGEffectUncoverUp[11] = {4, CHAR_5411, CHAR_4E0A, CHAR_9010, CHAR_5165};
WORD aNDJPEGEffectUncoverDown[13] = {4, CHAR_5411, CHAR_4E0B, CHAR_9010, CHAR_5165};
WORD aNDJPEGEffectUncoverLeft[13] = {4, CHAR_5411, CHAR_5DE6, CHAR_9010, CHAR_5165};
WORD aNDJPEGEffectUncoverRight[14] = {4, CHAR_5411, CHAR_53F3, CHAR_9010, CHAR_5165};
WORD aNDJPEGEffectWipeUp[8] = {4, CHAR_5411, CHAR_4E0A, CHAR_6EDA, CHAR_5165};
WORD aNDJPEGEffectWipeDown[10] = {4, CHAR_5411, CHAR_4E0B, CHAR_6EDA, CHAR_5165};
WORD aNDJPEGEffectWipeLeft[10] = {4, CHAR_5411, CHAR_5DE6, CHAR_6EDA, CHAR_5165};
WORD aNDJPEGEffectWipeRight[11] = {4, CHAR_5411, CHAR_53F3, CHAR_6EDA, CHAR_5165};
WORD aNDJPEGEffectPushUp[8] = {4, CHAR_5411, CHAR_4E0A, CHAR_63A8, CHAR_5165};
WORD aNDJPEGEffectPushDown[10] = {4, CHAR_5411, CHAR_4E0B, CHAR_63A8, CHAR_5165};
WORD aNDJPEGEffectPushLeft[10] = {4, CHAR_5411, CHAR_5DE6, CHAR_63A8, CHAR_5165};
WORD aNDJPEGEffectPushRight[11] = {4, CHAR_5411, CHAR_53F3, CHAR_63A8, CHAR_5165};
WORD aNDJPEGEffectFadeThroughBlack[19] = {18, CHAR_F, CHAR_a, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_h, CHAR_r, CHAR_o, CHAR_u, CHAR_g, CHAR_h, CHAR_SPACE, CHAR_B, CHAR_l, CHAR_a, CHAR_c, CHAR_k};
WORD aNDJPEGEffectRandom[7] = {2, CHAR_968F, CHAR_673A};
#ifndef NO_DISC_MODE			
WORD aNDEncrypt[13] = {9, CHAR_9519, CHAR_8BEF, CHAR_5236, CHAR_5F0F, CHAR_S, CHAR_V, CHAR_C, CHAR_D, CHAR_789F};
WORD aNDErrRegionCode[20] = {5, CHAR_9519, CHAR_8BEF, CHAR_533A, CHAR_7801, CHAR_789F};
WORD aNDWrongDisc[11] = {3, CHAR_9519, CHAR_8BEF, CHAR_789F};
WORD aNDDamageDisc[12] = {3, CHAR_65E0, CHAR_6548, CHAR_789F};
#endif //#ifndef NO_DISC_MODE			
WORD aNDNoPlayableFile[17] = {5, CHAR_65E0, CHAR_64AD, CHAR_653E, CHAR_6587, CHAR_4EF6};
WORD aNDDisc[5] = {2, CHAR_789F, CHAR_7247};
WORD aNDTrack[6] = {2, CHAR_66F2, CHAR_76EE};
WORD aNDTitle[6] = {2, CHAR_6807, CHAR_9898};
WORD aNDChapter[8] = {2, CHAR_7AE0, CHAR_8282};
WORD aNDLR[4] = {3, CHAR_7ACB, CHAR_4F53, CHAR_58F0};
WORD aNDLL[4] = {3, CHAR_5DE6, CHAR_58F0, CHAR_9053};
WORD aNDRR[4] = {3, CHAR_53F3, CHAR_58F0, CHAR_9053};
WORD aNDVolume[7] = {2, CHAR_97F3, CHAR_91CF};
WORD aNDTone[5] = {2, CHAR_97F3, CHAR_8C03};
WORD aNDEcho[5] = {2, CHAR_6DF7, CHAR_54CD};
WORD aNDNoSurround[12] = {4, CHAR_73AF, CHAR_7ED5, CHAR_53D6, CHAR_6D88};
WORD aNDLiveSurround[5] = {4, CHAR_L, CHAR_i, CHAR_v, CHAR_e};
WORD aNDTheaterSurround[8] = {4, CHAR_7535, CHAR_5F71, CHAR_73AF, CHAR_7ED5};
WORD aNDStereoSurround[7] = {4, CHAR_7ACB, CHAR_4F53, CHAR_73AF, CHAR_7ED5};
WORD aNDMovieSurround[6] = {4, CHAR_7535, CHAR_5F71, CHAR_73AF, CHAR_7ED5};
WORD aNDKaraSurround[8] = {7, CHAR_K, CHAR_a, CHAR_r, CHAR_a, CHAR_o, CHAR_k, CHAR_e};
WORD aNDLanguageJapanese[9] = {2, CHAR_65E5, CHAR_6587};
WORD aNDLanguageEnglish[8] = {2, CHAR_82F1, CHAR_6587};
WORD aNDLanguageFrench[7] = {2, CHAR_6CD5, CHAR_6587};
WORD aNDLanguageGerman[7] = {2, CHAR_5FB7, CHAR_6587};
WORD aNDLanguageRussian[8] = {2, CHAR_4FC4, CHAR_6587};
WORD aNDLanguageChinese[8] = {2, CHAR_4E2D, CHAR_6587};
WORD aNDLanguageKorean[7] = {2, CHAR_97E9, CHAR_6587};
WORD aNDLanguageHindi[6] = {3, CHAR_5370, CHAR_5EA6, CHAR_6587};
WORD aNDLanguageThai[5] = {2, CHAR_6CF0, CHAR_6587};
WORD aNDLanguageItalian[8] = {3, CHAR_610F, CHAR_5927, CHAR_5229};
WORD aNDLanguageSpanish[8] = {3, CHAR_897F, CHAR_73ED, CHAR_7259};
WORD aNDLanguagePortuguese[11] = {3, CHAR_8461, CHAR_8404, CHAR_7259};
WORD aNDLanguageOthers[7] = {2, CHAR_5176, CHAR_4ED6};
WORD aNDSubtitle[10] = {3, CHAR_5B57, CHAR_5E55, CHAR_COLON};
WORD aNDOff[4] = {1, CHAR_5173};
WORD aNDWait[8] = {2, CHAR_8BFB, CHAR_789F};
WORD aNDSelect[7] = {3, CHAR_8BF7, CHAR_9009, CHAR_62E9};
WORD aNDCardReader[12] = {2, CHAR_8BFB, CHAR_5361};
#endif //OSDND_MAIN_FILE			
#ifdef OSDMM_MAIN_FILE			
WORD aMMProgramModeTitle[13] = {4, CHAR_7F16, CHAR_8F91, CHAR_6A21, CHAR_5F0F};
WORD aMMProgramListModeTitle[13] = {4, CHAR_7F16, CHAR_8F91, CHAR_6E05, CHAR_5355};
WORD aMMTotalFiles[13] = {3, CHAR_6587, CHAR_4EF6, CHAR_COLON};
WORD aMMFileNumber[13] = {3, CHAR_6587, CHAR_4EF6, CHAR_COLON};
WORD aMMDlgFileTypeTitle[11] = {5, CHAR_6587, CHAR_4EF6, CHAR_7C7B, CHAR_578B, CHAR_COLON};
WORD aMMDlgSortFileTitle[11] = {5, CHAR_6587, CHAR_4EF6, CHAR_6392, CHAR_5E8F, CHAR_COLON};
WORD aMMDlgSortFileMethodTitle[8] = {3, CHAR_65B9, CHAR_5F0F, CHAR_COLON};
WORD aMMDlgSortFileSequenceTitle[10] = {3, CHAR_987A, CHAR_5E8F, CHAR_COLON};
WORD aMMDlgSequenceAscendingItem[10] = {2, CHAR_4E0A, CHAR_5347};
WORD aMMDlgSequenceDescendingItem[11] = {2, CHAR_4E0B, CHAR_964D};
WORD aMMDlgMethodTypeItem[8] = {2, CHAR_7C7B, CHAR_578B};
WORD aMMDlgMethodNameItem[8] = {4, CHAR_6587, CHAR_4EF6, CHAR_540D, CHAR_COLON};
WORD aMMDlgMethodTimeItem[8] = {2, CHAR_65E5, CHAR_671F};
WORD aMMHelpProgram[12] = {2, CHAR_7F16, CHAR_8F91};
WORD aMMHelpProgramList[10] = {2, CHAR_6E05, CHAR_9664};
WORD aMMBtnFileType[10] = {4, CHAR_6587, CHAR_4EF6, CHAR_7C7B, CHAR_578B};
WORD aMMBtnSortFile[10] = {4, CHAR_6587, CHAR_4EF6, CHAR_6392, CHAR_5E8F};
WORD aMMBtnList[5] = {2, CHAR_6E05, CHAR_5355};
WORD aMMBtnPlay[5] = {2, CHAR_64AD, CHAR_653E};
WORD aMMBtnDeleteAll[11] = {3, CHAR_5168, CHAR_6E05, CHAR_9664};
WORD aMMBtnReturn[7] = {2, CHAR_8FD4, CHAR_56DE};
WORD aMMBtnExit[5] = {2, CHAR_79BB, CHAR_5F00};
#endif //OSDMM_MAIN_FILE			
#ifdef THUMB_MAIN_FILE			
WORD aTHUMBPage[6] = {2, CHAR_9875, CHAR_COLON};
WORD aTHUMBTitleGeneralMode[15] = {4, CHAR_6D4F, CHAR_89C8, CHAR_6A21, CHAR_5F0F};
WORD aTHUMBTitleRotateMode[12] = {4, CHAR_65CB, CHAR_8F6C, CHAR_6A21, CHAR_5F0F};
WORD aTHUMBTitleProgramMode[13] = {4, CHAR_7F16, CHAR_8F91, CHAR_6A21, CHAR_5F0F};
WORD aTHUMBTitleProgramList[13] = {4, CHAR_7F16, CHAR_8F91, CHAR_6E05, CHAR_5355};
WORD aTHUMBHelpRotateMode[10] = {2, CHAR_65CB, CHAR_8F6C};
WORD aTHUMBHelpProgramMode[12] = {2, CHAR_7F16, CHAR_8F91};
WORD aTHUMBHelpProgramList[10] = {2, CHAR_6E05, CHAR_9664};
WORD aTHUMBButtonRotate[7] = {2, CHAR_65CB, CHAR_8F6C};
WORD aTHUMBButtonSlideShow[11] = {2, CHAR_64AD, CHAR_653E};
WORD aTHUMBButtonReset[6] = {2, CHAR_91CD, CHAR_7F6E};
WORD aTHUMBButtonReturn[7] = {2, CHAR_8FD4, CHAR_56DE};
WORD aTHUMBButtonProgramList[5] = {2, CHAR_6E05, CHAR_5355};
#endif //THUMB_MAIN_FILE			
#ifdef OSDDG_MAIN_FILE			
WORD aDGDlgBtnTrackDigest[13] = {4, CHAR_66F2, CHAR_76EE, CHAR_6D4F, CHAR_89C8};
WORD aDGDlgBtnDiscInterval[14] = {6, CHAR_789F, CHAR_7247, CHAR_95F4, CHAR_9694, CHAR_6D4F, CHAR_89C8};
WORD aDGPage[6] = {2, CHAR_9875, CHAR_COLON};
WORD aDGTitleTrackDigest[13] = {4, CHAR_66F2, CHAR_76EE, CHAR_6D4F, CHAR_89C8};
WORD aDGTitleDiscInterval[14] = {6, CHAR_789F, CHAR_7247, CHAR_95F4, CHAR_9694, CHAR_6D4F, CHAR_89C8};
WORD aDGInfoTrack[7] = {2, CHAR_66F2, CHAR_76EE};
WORD aDGBtnMenu[5] = {2, CHAR_9009, CHAR_5355};
WORD aDGBtnExit[5] = {2, CHAR_79BB, CHAR_5F00};
#endif //OSDDG_MAIN_FILE			
#ifdef OSDPRGM_MAIN_FILE			
WORD aProgram[8] = {2, CHAR_7F16, CHAR_8F91};
WORD aTitle[6] = {2, CHAR_6807, CHAR_9898};
WORD aChapter[8] = {2, CHAR_7AE0, CHAR_8282};
WORD aTrack[6] = {2, CHAR_66F2, CHAR_76EE};
WORD aSelect[7] = {2, CHAR_9009, CHAR_62E9};
WORD aPlay[5] = {2, CHAR_64AD, CHAR_653E};
WORD aClear[6] = {2, CHAR_6E05, CHAR_9664};
#endif //OSDPRGM_MAIN_FILE			
#ifdef OSDBMARK_MAIN_FILE			
WORD aBookmark[9] = {2, CHAR_6807, CHAR_7B7E};
WORD aTime[5] = {2, CHAR_65F6, CHAR_95F4};
#endif //OSDBMARK_MAIN_FILE			
#ifdef SETUP_MAIN_FILE			
WORD aSetupSelect[7] = {2, CHAR_9009, CHAR_62E9};
#endif //SETUP_MAIN_FILE			
#ifdef MENU_MAIN_FILE			
WORD aMsgEnglish[8] = {2, CHAR_82F1, CHAR_6587};
WORD aMsgChinese[8] = {2, CHAR_4E2D, CHAR_6587};
WORD aMsgJapanese[9] = {2, CHAR_65E5, CHAR_6587};
WORD aMsgFrench[7] = {2, CHAR_6CD5, CHAR_6587};
WORD aMsgGerman[7] = {2, CHAR_5FB7, CHAR_6587};
WORD aMsgItalian[8] = {4, CHAR_610F, CHAR_5927, CHAR_5229, CHAR_6587};
WORD aMsgSpanish[8] = {4, CHAR_897F, CHAR_73ED, CHAR_7259, CHAR_6587};
WORD aMsgPortuguese[11] = {4, CHAR_8461, CHAR_8404, CHAR_7259, CHAR_6587};
WORD aMsgDutch[6] = {3, CHAR_8377, CHAR_5170, CHAR_6587};
WORD aMsgPolish[7] = {3, CHAR_6CE2, CHAR_5170, CHAR_6587};
WORD aMsgHindi[6] = {3, CHAR_5370, CHAR_5EA6, CHAR_6587};
WORD aMsgThai[5] = {2, CHAR_6CF0, CHAR_6587};
WORD aMsgRussian[8] = {2, CHAR_4FC4, CHAR_6587};
WORD aMsgKorean[7] = {2, CHAR_97E9, CHAR_6587};
WORD aMsgCzech[6] = {3, CHAR_6377, CHAR_514B, CHAR_6587};
WORD aMsgGreece[7] = {3, CHAR_5E0C, CHAR_814A, CHAR_6587};
WORD aMsgTurkey[7] = {4, CHAR_571F, CHAR_8033, CHAR_5176, CHAR_6587};
WORD aMsgOthers[7] = {2, CHAR_5176, CHAR_4ED6};
WORD aMsgOSD[6] = {5, CHAR_O, CHAR_S, CHAR_D, CHAR_8BED, CHAR_8A00};
WORD aMsgMenu[5] = {4, CHAR_83DC, CHAR_5355, CHAR_8BED, CHAR_8A00};
WORD aMsgAudio[6] = {4, CHAR_58F0, CHAR_97F3, CHAR_8BED, CHAR_8A00};
WORD aMsgSubtitle[9] = {4, CHAR_5B57, CHAR_5E55, CHAR_8BED, CHAR_8A00};
WORD aMsgPS[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_P, CHAR_S};
WORD aMsgLB[7] = {6, CHAR_4, CHAR_COLON, CHAR_3, CHAR_SPACE, CHAR_L, CHAR_B};
WORD aMsgWide[5] = {4, CHAR_1, CHAR_6, CHAR_COLON, CHAR_9};
WORD aMsgOn[3] = {1, CHAR_5F00};
WORD aMsgOff[4] = {1, CHAR_5173};
WORD aMsgNTSC[5] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aMsgPAL[4] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aMsgAuto[5] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
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
WORD aMsgBriNormal[7] = {2, CHAR_6B63, CHAR_5E38};
WORD aMsgBright[7] = {2, CHAR_660E, CHAR_4EAE};
WORD aMsgSoft[5] = {2, CHAR_67D4, CHAR_548C};
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
WORD aMsgTVType[8] = {4, CHAR_753B, CHAR_9762, CHAR_5F62, CHAR_5F0F};
WORD aMsgScreenSaver[13] = {4, CHAR_5C4F, CHAR_5E55, CHAR_4FDD, CHAR_62A4};
WORD aMsgTVSystem[10] = {4, CHAR_7535, CHAR_89C6, CHAR_5236, CHAR_5F0F};
WORD aMsgVideoOutput[13] = {4, CHAR_89C6, CHAR_9891, CHAR_8F93, CHAR_51FA};
WORD aMsgBrightness[11] = {2, CHAR_4EAE, CHAR_5EA6};
WORD aMsgContrast[9] = {2, CHAR_5BF9, CHAR_6BD4};
WORD aMsgChromPhase[4] = {2, CHAR_8272, CHAR_8C03};
WORD aMsgSPDIF_PCM[10] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_P, CHAR_C, CHAR_M};
WORD aMsgSPDIF_RAW[10] = {9, CHAR_S, CHAR_P, CHAR_D, CHAR_I, CHAR_F, CHAR_SOLIDUS, CHAR_R, CHAR_A, CHAR_W};
WORD aMsg2CH[10] = {5, CHAR_2, CHAR_SPACE, CHAR_58F0, CHAR_9053, CHAR_SPACE};
WORD aMsg51CH[12] = {5, CHAR_5, CHAR_FULL_STOP, CHAR_1, CHAR_58F0, CHAR_9053};
WORD aMsgLoRo[6] = {3, CHAR_7ACB, CHAR_4F53, CHAR_58F0};
WORD aMsgLtRt[6] = {5, CHAR_L, CHAR_t, CHAR_SOLIDUS, CHAR_R, CHAR_t};
#ifndef REMOVE_SETUP_SPEAKER_TEST			
WORD aMsgAutoTest[5] = {4, CHAR_81EA, CHAR_52A8, CHAR_6D4B, CHAR_8BD5};
WORD aMsgLeft[5] = {3, CHAR_524D, CHAR_7F6E, CHAR_5DE6};
WORD aMsgRight[6] = {3, CHAR_524D, CHAR_7F6E, CHAR_53F3};
WORD aMsgCntr[7] = {2, CHAR_4E2D, CHAR_7F6E};
WORD aMsgLS[10] = {3, CHAR_540E, CHAR_7F6E, CHAR_5DE6};
WORD aMsgRS[11] = {3, CHAR_540E, CHAR_7F6E, CHAR_53F3};
WORD aMsgSW[10] = {3, CHAR_91CD, CHAR_4F4E, CHAR_97F3};
#endif //REMOVE_SETUP_SPEAKER_TEST			
WORD aMsgDigitalOut[12] = {4, CHAR_6570, CHAR_5B57, CHAR_8F93, CHAR_51FA};
WORD aMsgAnalogOut[11] = {4, CHAR_6A21, CHAR_62DF, CHAR_8F93, CHAR_51FA};
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
WORD aMsgVirtual[8] = {7, CHAR_V, CHAR_i, CHAR_r, CHAR_t, CHAR_u, CHAR_a, CHAR_l};
WORD aMsgMusic[6] = {5, CHAR_M, CHAR_u, CHAR_s, CHAR_i, CHAR_c};
WORD aMsgMovie[6] = {5, CHAR_M, CHAR_o, CHAR_v, CHAR_i, CHAR_e};
WORD aMsgMatrix[7] = {6, CHAR_M, CHAR_a, CHAR_t, CHAR_r, CHAR_i, CHAR_x};
WORD aMsgPrologic[10] = {9, CHAR_P, CHAR_R, CHAR_O, CHAR_SPACE, CHAR_L, CHAR_O, CHAR_G, CHAR_I, CHAR_C};
WORD aMsgDownMix[8] = {7, CHAR_D, CHAR_o, CHAR_w, CHAR_n, CHAR_m, CHAR_i, CHAR_x};
WORD aMsgCenter[7] = {5, CHAR_4E2D, CHAR_7F6E, CHAR_626C, CHAR_58F0, CHAR_5668};
WORD aMsgRear[6] = {5, CHAR_540E, CHAR_7F6E, CHAR_626C, CHAR_58F0, CHAR_5668};
WORD aMsgSubwoofer[10] = {5, CHAR_4F4E, CHAR_97F3, CHAR_626C, CHAR_58F0, CHAR_5668};
WORD aMsgSpeakerTest[13] = {5, CHAR_626C, CHAR_58F0, CHAR_5668, CHAR_6D4B, CHAR_8BD5};
WORD aMsgLevel1[2] = {1, CHAR_1};
WORD aMsgLevel2[2] = {1, CHAR_2};
WORD aMsgLevel3[2] = {1, CHAR_3};
WORD aMsgLevel4[2] = {1, CHAR_4};
WORD aMsgLevel5[2] = {1, CHAR_5};
WORD aMsgLevel6[2] = {1, CHAR_6};
WORD aMsgLevel7[2] = {1, CHAR_7};
WORD aMsgLevel8[2] = {1, CHAR_8};
WORD aMsgRead[5] = {2, CHAR_590D, CHAR_539F};
WORD aMsgParental[14] = {5, CHAR_89C6, CHAR_542C, CHAR_8005, CHAR_5206, CHAR_7EA7};
WORD aMsgChangePassword[9] = {4, CHAR_66F4, CHAR_6539, CHAR_5BC6, CHAR_7801};
WORD aMsgAngleMark[11] = {4, CHAR_89D2, CHAR_5EA6, CHAR_6807, CHAR_793A};
WORD aMsgUIStyle[6] = {2, CHAR_98CE, CHAR_683C};
WORD aMsgUIStyle1[7] = {3, CHAR_98CE, CHAR_683C, CHAR_1};
WORD aMsgUIStyle2[7] = {3, CHAR_98CE, CHAR_683C, CHAR_2};
WORD aMsgUIStyle3[7] = {3, CHAR_98CE, CHAR_683C, CHAR_3};
WORD aMsgUIStyle4[7] = {3, CHAR_98CE, CHAR_683C, CHAR_4};
WORD aMsgSmartPlay[11] = {10, CHAR_S, CHAR_m, CHAR_a, CHAR_r, CHAR_t, CHAR_SPACE, CHAR_P, CHAR_l, CHAR_a, CHAR_y};
WORD aMsgDivXVOD[9] = {8, CHAR_D, CHAR_i, CHAR_v, CHAR_X, CHAR_SPACE, CHAR_V, CHAR_O, CHAR_D};
WORD aMsgDefaultValue[8] = {3, CHAR_9ED8, CHAR_8BA4, CHAR_503C};
WORD aMsgLanguageSetup[15] = {4, CHAR_8BED, CHAR_8A00, CHAR_8BBE, CHAR_5B9A};
WORD aMsgScreenSetup[13] = {4, CHAR_5C4F, CHAR_5E55, CHAR_8BBE, CHAR_5B9A};
WORD aMsgAudioSetup[12] = {4, CHAR_58F0, CHAR_97F3, CHAR_8BBE, CHAR_5B9A};
WORD aMsgCustomSetup[13] = {4, CHAR_559C, CHAR_597D, CHAR_8BBE, CHAR_5B9A};
WORD aMsgExitSetup[16] = {6, CHAR_79BB, CHAR_5F00, CHAR_8BBE, CHAR_5B9A, CHAR_9009, CHAR_5355};
WORD aMsgPassword[22] = {5, CHAR_8BF7, CHAR_8F93, CHAR_5165, CHAR_5BC6, CHAR_7801};
WORD aMsgWrongPassword[15] = {4, CHAR_5BC6, CHAR_7801, CHAR_9519, CHAR_8BEF};
WORD aMsgOldPassword[5] = {4, CHAR_65E7, CHAR_5BC6, CHAR_7801, CHAR_COLON};
WORD aMsgNewPassword[5] = {4, CHAR_65B0, CHAR_5BC6, CHAR_7801, CHAR_COLON};
WORD aMsgVerifyPassword[8] = {5, CHAR_9A8C, CHAR_8BC1, CHAR_5BC6, CHAR_7801, CHAR_COLON};
WORD aMsgStandard[9] = {8, CHAR_S, CHAR_t, CHAR_a, CHAR_n, CHAR_d, CHAR_a, CHAR_r, CHAR_d};
WORD aMsgClassical[10] = {9, CHAR_C, CHAR_l, CHAR_a, CHAR_s, CHAR_s, CHAR_i, CHAR_c, CHAR_a, CHAR_l};
WORD aMsgRock[5] = {4, CHAR_R, CHAR_o, CHAR_c, CHAR_k};
WORD aMsgJazz[5] = {4, CHAR_J, CHAR_a, CHAR_z, CHAR_z};
WORD aMsgAcoustic[9] = {8, CHAR_A, CHAR_c, CHAR_o, CHAR_u, CHAR_s, CHAR_t, CHAR_i, CHAR_c};
WORD aMsgPop[4] = {3, CHAR_P, CHAR_o, CHAR_p};
WORD aMsgCustom[7] = {6, CHAR_C, CHAR_u, CHAR_s, CHAR_t, CHAR_o, CHAR_m};
#ifdef SUPPORT_PSCAN_PROMPT_SETUP			
WORD aMsgPleaseMake[24] = {13, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_c, CHAR_a, CHAR_n, CHAR_7535, CHAR_89C6, CHAR_5C4F, CHAR_5E55, CHAR_8F93, CHAR_51FA, CHAR_QUESTION_MARK};
#endif SUPPORT_PSCAN_PROMPT_SETUP			
#endif //MENU_MAIN_FILE			
#ifdef IMAGE_FRAME_MENU_MAIN_FILE			
WORD aMENUPhotoSetting[14] = {4, CHAR_56FE, CHAR_7247, CHAR_8BBE, CHAR_7F6E};
WORD aMENUIntervalTime[14] = {4, CHAR_95F4, CHAR_9694, CHAR_65F6, CHAR_95F4};
#ifndef MUSTEK_SETUP_MENU			
WORD aMENU5Sec[7] = {3, CHAR_5, CHAR_SPACE, CHAR_79D2};
WORD aMENU10Sec[8] = {4, CHAR_1, CHAR_0, CHAR_SPACE, CHAR_79D2};
#endif //MUSTEK_SETUP_MENU			
WORD aMENU15Sec[8] = {4, CHAR_1, CHAR_5, CHAR_SPACE, CHAR_79D2};
WORD aMENU30Sec[8] = {4, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_79D2};
WORD aMENU1Min[7] = {3, CHAR_1, CHAR_SPACE, CHAR_5206};
WORD aMENU3Min[7] = {3, CHAR_3, CHAR_SPACE, CHAR_5206};
WORD aMENUTransitionEffect[18] = {4, CHAR_5207, CHAR_6362, CHAR_6548, CHAR_679C};
WORD aMENUJPEGEffectNone[10] = {5, CHAR_65E0, CHAR_5207, CHAR_6362, CHAR_6548, CHAR_679C};
WORD aMENUJPEGEffectCoverUp[9] = {4, CHAR_5411, CHAR_4E0A, CHAR_8986, CHAR_76D6};
WORD aMENUJPEGEffectCoverDown[11] = {4, CHAR_5411, CHAR_4E0B, CHAR_8986, CHAR_76D6};
#ifndef MUSTEK_SETUP_MENU			
WORD aMENUJPEGEffectCoverLeft[11] = {4, CHAR_5411, CHAR_5DE6, CHAR_8986, CHAR_76D6};
WORD aMENUJPEGEffectCoverRight[12] = {4, CHAR_5411, CHAR_53F3, CHAR_8986, CHAR_76D6};
WORD aMENUJPEGEffectUncoverUp[11] = {4, CHAR_5411, CHAR_4E0A, CHAR_9010, CHAR_5165};
WORD aMENUJPEGEffectUncoverDown[13] = {4, CHAR_5411, CHAR_4E0B, CHAR_9010, CHAR_5165};
#endif //MUSTEK_SETUP_MENU			
WORD aMENUJPEGEffectUncoverLeft[13] = {4, CHAR_5411, CHAR_5DE6, CHAR_9010, CHAR_5165};
WORD aMENUJPEGEffectUncoverRight[14] = {4, CHAR_5411, CHAR_53F3, CHAR_9010, CHAR_5165};
WORD aMENUJPEGEffectWipeUp[8] = {4, CHAR_5411, CHAR_4E0A, CHAR_6EDA, CHAR_5165};
WORD aMENUJPEGEffectWipeDown[10] = {4, CHAR_5411, CHAR_4E0B, CHAR_6EDA, CHAR_5165};
#ifndef MUSTEK_SETUP_MENU			
WORD aMENUJPEGEffectWipeLeft[10] = {4, CHAR_5411, CHAR_5DE6, CHAR_6EDA, CHAR_5165};
WORD aMENUJPEGEffectWipeRight[11] = {4, CHAR_5411, CHAR_53F3, CHAR_6EDA, CHAR_5165};
WORD aMENUJPEGEffectPushUp[8] = {4, CHAR_5411, CHAR_4E0A, CHAR_63A8, CHAR_5165};
WORD aMENUJPEGEffectPushDown[10] = {4, CHAR_5411, CHAR_4E0B, CHAR_63A8, CHAR_5165};
WORD aMENUJPEGEffectPushLeft[10] = {4, CHAR_5411, CHAR_5DE6, CHAR_63A8, CHAR_5165};
#endif //MUSTEK_SETUP_MENU			
WORD aMENUJPEGEffectPushRight[11] = {4, CHAR_5411, CHAR_53F3, CHAR_63A8, CHAR_5165};
WORD aMENUJPEGEffectFadeThroughBlack[19] = {18, CHAR_F, CHAR_a, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_T, CHAR_h, CHAR_r, CHAR_o, CHAR_u, CHAR_g, CHAR_h, CHAR_SPACE, CHAR_B, CHAR_l, CHAR_a, CHAR_c, CHAR_k};
WORD aMENUJPEGEffectRandom[7] = {2, CHAR_968F, CHAR_673A};
WORD aMENUBrowse[7] = {2, CHAR_6D4F, CHAR_89C8};
WORD aMENUFileManager[13] = {4, CHAR_6587, CHAR_4EF6, CHAR_7BA1, CHAR_7406};
WORD aMENUSetupMenu[11] = {4, CHAR_8BBE, CHAR_7F6E, CHAR_83DC, CHAR_5355};
WORD aMENUThumbnail[10] = {3, CHAR_7F29, CHAR_7565, CHAR_56FE};
WORD aMENUAutoPlay[10] = {4, CHAR_81EA, CHAR_52A8, CHAR_64AD, CHAR_653E};
WORD aMENUPhoto[6] = {2, CHAR_56FE, CHAR_7247};
WORD aMENUMusic[6] = {2, CHAR_97F3, CHAR_4E50};
WORD aMENUPhotoAudio[12] = {5, CHAR_56FE, CHAR_7247, CHAR_SOLIDUS, CHAR_97F3, CHAR_4E50};
WORD aMENUMovie[6] = {2, CHAR_7535, CHAR_5F71};
WORD aMENUDisplaySetup[14] = {4, CHAR_663E, CHAR_793A, CHAR_8BBE, CHAR_7F6E};
WORD aMENUDisplayType[13] = {4, CHAR_663E, CHAR_793A, CHAR_98CE, CHAR_683C};
WORD aMENUTVStandard[12] = {4, CHAR_7535, CHAR_89C6, CHAR_5236, CHAR_5F0F};
WORD aMENUNTSC[5] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aMENUPAL[4] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aMENUAUTO[5] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aMENUBrightness[11] = {2, CHAR_4EAE, CHAR_5EA6};
WORD aMENUContrast[9] = {2, CHAR_5BF9, CHAR_6BD4};
WORD aMENUNormal[7] = {2, CHAR_6B63, CHAR_5E38};
WORD aMENUBright[7] = {2, CHAR_660E, CHAR_4EAE};
WORD aMENUSoft[5] = {2, CHAR_67D4, CHAR_548C};
WORD aMENUTFTBrightness[15] = {6, CHAR_T, CHAR_F, CHAR_T, CHAR_SPACE, CHAR_4EAE, CHAR_5EA6};
WORD aMENUTFTContrast[13] = {7, CHAR_T, CHAR_F, CHAR_T, CHAR_SPACE, CHAR_5BF9, CHAR_6BD4, CHAR_5EA6};
WORD aMENUTFTSaturation[15] = {7, CHAR_T, CHAR_F, CHAR_T, CHAR_SPACE, CHAR_9971, CHAR_548C, CHAR_5EA6};
WORD aMENURepeatMode[12] = {4, CHAR_91CD, CHAR_590D, CHAR_6A21, CHAR_5F0F};
WORD aMENURepeatOne[4] = {3, CHAR_91CD, CHAR_590D, CHAR_1};
WORD aMENURepeatFolder[7] = {4, CHAR_91CD, CHAR_590D, CHAR_6587, CHAR_4EF6};
WORD aMENURepeatAll[5] = {4, CHAR_91CD, CHAR_590D, CHAR_5168, CHAR_90E8};
WORD aMENUCustomSetup[13] = {4, CHAR_559C, CHAR_597D, CHAR_8BBE, CHAR_7F6E};
WORD aMENUOSDLanguage[13] = {5, CHAR_O, CHAR_S, CHAR_D, CHAR_8BED, CHAR_8A00};
WORD aMENUEnglish[8] = {2, CHAR_82F1, CHAR_6587};
WORD aMENUChinese[8] = {2, CHAR_4E2D, CHAR_6587};
WORD aMENUJapanese[9] = {2, CHAR_65E5, CHAR_6587};
WORD aMENUKorean[7] = {2, CHAR_97E9, CHAR_6587};
WORD aMENUFrench[7] = {2, CHAR_6CD5, CHAR_6587};
WORD aMENUGerman[7] = {2, CHAR_5FB7, CHAR_6587};
WORD aMENUItalian[8] = {4, CHAR_610F, CHAR_5927, CHAR_5229, CHAR_6587};
WORD aMENUSpanish[8] = {4, CHAR_897F, CHAR_73ED, CHAR_7259, CHAR_6587};
WORD aMENUPortuguese[11] = {4, CHAR_8461, CHAR_8404, CHAR_7259, CHAR_6587};
WORD aMENUDutch[6] = {3, CHAR_8377, CHAR_5170, CHAR_6587};
WORD aMENUPolish[7] = {3, CHAR_6CE2, CHAR_5170, CHAR_6587};
WORD aMENUUIStyle[6] = {2, CHAR_98CE, CHAR_683C};
WORD aMENUUIStyle1[7] = {3, CHAR_98CE, CHAR_683C, CHAR_1};
WORD aMENUUIStyle2[7] = {3, CHAR_98CE, CHAR_683C, CHAR_2};
WORD aMENUUIStyle3[7] = {3, CHAR_98CE, CHAR_683C, CHAR_3};
WORD aMENUUIStyle4[7] = {3, CHAR_98CE, CHAR_683C, CHAR_4};
WORD aMENUDefault[8] = {3, CHAR_9884, CHAR_8BBE, CHAR_503C};
WORD aMENURead[5] = {2, CHAR_590D, CHAR_539F};
WORD aMENUIconExit[5] = {2, CHAR_9000, CHAR_51FA};
WORD aMENUHelpSelect[10] = {4, CHAR_COLON, CHAR_SPACE, CHAR_9009, CHAR_62E9};
WORD aMENUHelpConfirm[10] = {4, CHAR_COLON, CHAR_SPACE, CHAR_786E, CHAR_8BA4};
WORD aMENUHelpExit[18] = {5, CHAR_4FDD, CHAR_5B58, CHAR_AMPERSAND, CHAR_9000, CHAR_51FA};
#endif //#ifdef IMAGE_FRAME_MENU_MAIN_FILE			
#ifdef SUPPORT_POWERON_MENU			
WORD aPOWERONIconPhoto[6] = {2, CHAR_56FE, CHAR_7247};
WORD aPOWERONIconMusic[6] = {2, CHAR_97F3, CHAR_4E50};
WORD aPOWERONIconPhotoMusic[12] = {5, CHAR_56FE, CHAR_7247, CHAR_PLUS_SIGN, CHAR_97F3, CHAR_4E50};
WORD aPOWERONIconMovie[6] = {2, CHAR_7535, CHAR_5F71};
WORD aPOWERONIconSetup[6] = {2, CHAR_8BBE, CHAR_7F6E};
WORD aPOWERONIconCalendar[9] = {2, CHAR_65E5, CHAR_5386};
WORD aPOWERONIconClock[6] = {2, CHAR_65F6, CHAR_949F};
WORD aPOWERONIconAlarm[6] = {2, CHAR_95F9, CHAR_949F};
WORD aPOWERONIconAutoPower[11] = {4, CHAR_81EA, CHAR_52A8, CHAR_5173, CHAR_673A};
WORD aPOWERONIconEdit[5] = {2, CHAR_7F16, CHAR_8F91};
WORD aPOWERONIconFavorite[9] = {2, CHAR_559C, CHAR_597D};
WORD aPOWERONIconRadio[6] = {3, CHAR_6536, CHAR_97F3, CHAR_673A};
WORD aWeeklyDay_Mon[4] = {1, CHAR_4E00};
WORD aWeeklyDay_Tue[4] = {1, CHAR_4E8C};
WORD aWeeklyDay_Wed[4] = {1, CHAR_4E09};
WORD aWeeklyDay_Thu[4] = {1, CHAR_56DB};
WORD aWeeklyDay_Fri[4] = {1, CHAR_4E94};
WORD aWeeklyDay_Sat[4] = {1, CHAR_516D};
WORD aWeeklyDay_Sun[4] = {1, CHAR_65E5};
WORD aMonthJanuary[4] = {2, CHAR_4E00, CHAR_6708};
WORD aMonthFebruary[4] = {2, CHAR_4E8C, CHAR_6708};
WORD aMonthMarch[4] = {2, CHAR_4E09, CHAR_6708};
WORD aMonthApril[4] = {2, CHAR_56DB, CHAR_6708};
WORD aMonthMay[4] = {2, CHAR_4E94, CHAR_6708};
WORD aMonthJune[4] = {2, CHAR_516D, CHAR_6708};
WORD aMonthJuly[4] = {2, CHAR_4E03, CHAR_6708};
WORD aMonthAugust[4] = {2, CHAR_516B, CHAR_6708};
WORD aMonthSeptember[4] = {2, CHAR_4E5D, CHAR_6708};
WORD aMonthOctober[4] = {2, CHAR_5341, CHAR_6708};
WORD aMonthNovember[4] = {3, CHAR_5341, CHAR_4E00, CHAR_6708};
WORD aMonthDecember[4] = {3, CHAR_5341, CHAR_4E8C, CHAR_6708};
WORD aWeeklyMonday[7] = {3, CHAR_661F, CHAR_671F, CHAR_4E00};
WORD aWeeklyTuesday[8] = {3, CHAR_661F, CHAR_671F, CHAR_4E8C};
WORD aWeeklyWednesday[10] = {3, CHAR_661F, CHAR_671F, CHAR_4E09};
WORD aWeeklyThursday[9] = {3, CHAR_661F, CHAR_671F, CHAR_56DB};
WORD aWeeklyFriday[7] = {3, CHAR_661F, CHAR_671F, CHAR_4E94};
WORD aWeeklySaturday[9] = {3, CHAR_661F, CHAR_671F, CHAR_516D};
WORD aWeeklySunday[7] = {3, CHAR_661F, CHAR_671F, CHAR_65E5};
WORD aCLOCKClockSetting[14] = {4, CHAR_65F6, CHAR_95F4, CHAR_8BBE, CHAR_7F6E};
WORD aCLOCKDisplay[8] = {4, CHAR_663E, CHAR_793A, CHAR_65F6, CHAR_95F4};
WORD aCLOCKSetTime[9] = {4, CHAR_8BBE, CHAR_5B9A, CHAR_65F6, CHAR_95F4};
WORD aCLOCKExit[5] = {2, CHAR_9000, CHAR_51FA};
WORD aCLOCKBigClock[6] = {2, CHAR_65F6, CHAR_95F4};
WORD aCLOCKClockCalendar[17] = {5, CHAR_65F6, CHAR_95F4, CHAR_AMPERSAND, CHAR_65E5, CHAR_671F};
WORD aBUTTONOK[3] = {2, CHAR_786E, CHAR_5B9A};
WORD aBUTTONCancel[7] = {2, CHAR_53D6, CHAR_6D88};
WORD aSETTIMESetTime[9] = {4, CHAR_8BBE, CHAR_5B9A, CHAR_65F6, CHAR_95F4};
WORD aALARMEnable[7] = {2, CHAR_6253, CHAR_5F00};
WORD aALARMAlarmSetting[14] = {4, CHAR_95F9, CHAR_949F, CHAR_8BBE, CHAR_7F6E};
WORD aALARMDlgTitle[6] = {2, CHAR_95F9, CHAR_949F};
WORD aALARMDlgContent[33] = {4, CHAR_53D6, CHAR_6D88, CHAR_95F9, CHAR_949F};
WORD aAUTOPWRAutoPowerSetting[26] = {9, CHAR_81EA, CHAR_52A8, CHAR_5173, CHAR_673A, CHAR_5F00, CHAR_SOLIDUS, CHAR_5173, CHAR_8BBE, CHAR_7F6E};
WORD aAUTOPWRPowerOn[9] = {2, CHAR_5F00, CHAR_673A};
WORD aAUTOPWRPowerOff[10] = {2, CHAR_5173, CHAR_673A};
WORD aAUTOPWRPowerOnEnable[16] = {6, CHAR_9009, CHAR_62E9, CHAR_81EA, CHAR_52A8, CHAR_5F00, CHAR_673A};
WORD aAUTOPWRPowerOffEnable[17] = {6, CHAR_9009, CHAR_62E9, CHAR_81EA, CHAR_52A8, CHAR_5173, CHAR_673A};
WORD aAUTOPWRSetTime[9] = {8, CHAR_8BBE, CHAR_5B9A, CHAR_5F00, CHAR_SOLIDUS, CHAR_5173, CHAR_673A, CHAR_65F6, CHAR_95F4};
WORD aMMBtnSave[5] = {2, CHAR_4FDD, CHAR_5B58};
WORD aMMSaveFileDlgTitle[10] = {4, CHAR_4FDD, CHAR_5B58, CHAR_6587, CHAR_4EF6};
WORD aMMSaveFileDlgContent[12] = {3, CHAR_8BF7, CHAR_7B49, CHAR_5F85};
WORD aTHUMBButtonSort[5] = {2, CHAR_5206, CHAR_7C7B};
WORD aTHUMBButtonDeleteAll[11] = {4, CHAR_5168, CHAR_90E8, CHAR_6E05, CHAR_9664};
WORD aTHUMBTitleEditMode[10] = {4, CHAR_7F16, CHAR_8F91, CHAR_6A21, CHAR_5F0F};
WORD aTHUMBTitleDeleteMode[12] = {4, CHAR_6E05, CHAR_9664, CHAR_6A21, CHAR_5F0F};
WORD aTHUMBHelpDeleteMode[10] = {2, CHAR_6E05, CHAR_9664};
WORD aTHUMBHelpSortMode[18] = {4, CHAR_9009, CHAR_5B9A, CHAR_56FE, CHAR_7247};
WORD aTHUMBTitleSortMode[10] = {4, CHAR_5206, CHAR_7C7B, CHAR_6A21, CHAR_5F0F};
WORD aEDITDialogTitle[12] = {5, CHAR_7F16, CHAR_8F91, CHAR_5BF9, CHAR_8BDD, CHAR_6846};
WORD aEDITDialogContent[24] = {7, CHAR_8BF7, CHAR_9009, CHAR_62E9, CHAR_7F16, CHAR_8F91, CHAR_6A21, CHAR_5F0F};
WORD aBUTTONCopy[5] = {2, CHAR_590D, CHAR_5236};
WORD aBUTTONDelete[7] = {2, CHAR_6E05, CHAR_9664};
WORD aSETDATESetDate[9] = {4, CHAR_8BBE, CHAR_5B9A, CHAR_65E5, CHAR_671F};
#endif //#ifdef SUPPORT_POWERON_MENU			
WORD aMENUOn[3] = {1, CHAR_5F00};
WORD aMENUOff[4] = {1, CHAR_5173};
#ifdef MUSTEK_SETUP_MENU			
WORD aMENU5Min[7] = {3, CHAR_5, CHAR_SPACE, CHAR_5206};
WORD aMENU30Min[8] = {4, CHAR_3, CHAR_0, CHAR_SPACE, CHAR_5206};
WORD aMENU1Hour[7] = {2, CHAR_1, CHAR_65F6};
WORD aMENU4Hours[8] = {2, CHAR_4, CHAR_65F6};
WORD aMENU12Hours[9] = {3, CHAR_1, CHAR_2, CHAR_65F6};
WORD aMENU1Day[6] = {3, CHAR_2, CHAR_4, CHAR_65F6};
WORD aMENUTransition[11] = {2, CHAR_5207, CHAR_6362};
WORD aMENUTVSystem[10] = {4, CHAR_7535, CHAR_89C6, CHAR_5236, CHAR_5F0F};
WORD aMENUFullScreen[12] = {2, CHAR_5168, CHAR_5C4F};
WORD aMENUReset[6] = {2, CHAR_590D, CHAR_4F4D};
WORD aMENUConfirm[8] = {2, CHAR_786E, CHAR_8BA4};
WORD aMENUExitSetup[16] = {6, CHAR_9000, CHAR_51FA, CHAR_8BBE, CHAR_7F6E, CHAR_6A21, CHAR_5F0F};
#endif //MUSTEK_SETUP_MENU					
WORD aBUTTONYes[4] = {2, CHAR_786E, CHAR_5B9A};
WORD aBUTTONNo[3] = {2, CHAR_53D6, CHAR_6D88};
#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW					
WORD aMMSaveDlgTitle[18] = {7, CHAR_5B58, CHAR_50A8, CHAR_56FE, CHAR_7247, CHAR_5BF9, CHAR_8BDD, CHAR_6846};
WORD aMMDeleteDlgTitle[20] = {7, CHAR_6E05, CHAR_9664, CHAR_56FE, CHAR_7247, CHAR_5BF9, CHAR_8BDD, CHAR_6846};
WORD aMMSaveDlgContent[6] = {3, CHAR_5B58, CHAR_50A8, CHAR_QUESTION_MARK};
WORD aMMDeleteDlgContent[8] = {2, CHAR_6E05, CHAR_9664};
#endif //#ifdef SUPPORT_COPY_FILE_IN_SLIDE_SHOW			
#ifdef SUPPORT_CHOOSE_MEDIA			
WORD aMENUMedia[6] = {2, CHAR_88C5, CHAR_7F6E};
#endif //SUPPORT_CHOOSE_MEDIA			
#ifdef SUPPORT_PHOTO_BROWSE_MODE			
WORD aNDBrowseMode[12] = {4, CHAR_6D4F, CHAR_89C8, CHAR_6A21, CHAR_5F0F};
#endif //SUPPORT_PHOTO_BROWSE_MODE			
#ifdef SUPPORT_TFT_DIGITAL_MODE			
WORD aMENUAVOutput[10] = {9, CHAR_A, CHAR_V, CHAR_SPACE, CHAR_O, CHAR_u, CHAR_t, CHAR_p, CHAR_u, CHAR_t};
#endif //SUPPORT_TFT_DIGITAL_MODE			
WORD aMENUPanel[6] = {2, CHAR_9762, CHAR_677F};
#ifdef SUPPORT_RADIO_TUNER			
WORD aRADIORecord[7] = {2, CHAR_5B58, CHAR_50A8};
WORD aRADIOScan[5] = {2, CHAR_626B, CHAR_63CF};
WORD aRADIOStationsFound[16] = {3, CHAR_53F0, CHAR_627E, CHAR_5230};
#endif //SUPPORT_RADIO_TUNER			
#ifdef SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN			
WORD aCODEWesternEuropean[17] = {4, CHAR_897F, CHAR_6B27, CHAR_8BED, CHAR_8A00};
#endif   // SUPPORT_CHAR_ENCODING_WESTERN_EUROPEAN			
#ifdef SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN			
WORD aCODECentralEuropean[17] = {4, CHAR_4E2D, CHAR_6B27, CHAR_8BED, CHAR_8A00};
#endif   // SUPPORT_CHAR_ENCODING_CENTRAL_EUROPEAN			
#ifdef SUPPORT_CHAR_ENCODING_GREEK			
WORD aCODEGreek[6] = {5, CHAR_G, CHAR_r, CHAR_e, CHAR_e, CHAR_k};
#endif   // SUPPORT_CHAR_ENCODING_GREEK			
#ifdef SUPPORT_CHAR_ENCODING_HEBREW			
WORD aCODEHebrew[7] = {6, CHAR_H, CHAR_e, CHAR_b, CHAR_r, CHAR_e, CHAR_w};
#endif   // SUPPORT_CHAR_ENCODING_HEBREW			
#ifdef SUPPORT_CHAR_ENCODING_TURKISH			
WORD aCODETurkish[8] = {7, CHAR_T, CHAR_u, CHAR_r, CHAR_k, CHAR_i, CHAR_s, CHAR_h};
#endif   // SUPPORT_CHAR_ENCODING_TURKISH			
#ifdef SUPPORT_CHAR_ENCODING_CP1250			
WORD aCODECP1250[8] = {7, CHAR_C, CHAR_P, CHAR_SPACE, CHAR_1, CHAR_2, CHAR_5, CHAR_0};
#endif   // SUPPORT_CHAR_ENCODING_CP1250			
#ifdef SUPPORT_CHAR_ENCODING_CYRILLIC			
WORD aCODECyrillic[9] = {8, CHAR_C, CHAR_y, CHAR_r, CHAR_i, CHAR_l, CHAR_l, CHAR_i, CHAR_c};
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
