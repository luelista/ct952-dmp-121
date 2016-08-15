#include "winav.h"
#include "comdef.h"
#include "osd.h"
#include "fontindex.h" //++CoCo2.37P

//***************************************************************************
//Normal Display
WORD aNDNumbers[] = {CHAR_0, CHAR_1, CHAR_2, CHAR_3, CHAR_4, CHAR_5, CHAR_6, CHAR_7, CHAR_8, CHAR_9};
WORD aNDUppercase[] = {CHAR_A, CHAR_B, CHAR_C, CHAR_D, CHAR_E, CHAR_F, CHAR_G, CHAR_H, CHAR_I, CHAR_J, CHAR_K, CHAR_L, CHAR_M, CHAR_N, CHAR_O, CHAR_P, CHAR_Q, CHAR_R, CHAR_S, CHAR_T, CHAR_U, CHAR_V, CHAR_W, CHAR_X, CHAR_Y, CHAR_Z};
WORD aNDLowercase[] = {CHAR_a, CHAR_b, CHAR_c, CHAR_d, CHAR_e, CHAR_f, CHAR_g, CHAR_h, CHAR_i, CHAR_j, CHAR_k, CHAR_l, CHAR_m, CHAR_n, CHAR_o, CHAR_p, CHAR_q, CHAR_r, CHAR_s, CHAR_t, CHAR_u, CHAR_v, CHAR_w, CHAR_x, CHAR_y, CHAR_z};
WORD aND51Channel[] = {5, CHAR_5, CHAR_FULL_STOP, CHAR_1, CHAR_C, CHAR_H};
WORD aND71Channel[] = {5, CHAR_7, CHAR_FULL_STOP, CHAR_1, CHAR_C, CHAR_H};
WORD aND2Channel[] = {3, CHAR_2, CHAR_C, CHAR_H};
WORD aNDChannel[] = {2, CHAR_C, CHAR_H};
WORD aNDMPEG[] = {4, CHAR_M, CHAR_P, CHAR_E, CHAR_G};
WORD aNDLPCM[] = {4, CHAR_L, CHAR_P, CHAR_C, CHAR_M};
WORD aNDPCM[] = {3, CHAR_P, CHAR_C, CHAR_M};
WORD aNDDTS[] = {3, CHAR_D, CHAR_T, CHAR_S};
WORD aNDDVD[] = {3, CHAR_D, CHAR_V, CHAR_D};
WORD aNDSVCD[] = {4, CHAR_S, CHAR_V, CHAR_C, CHAR_D};
WORD aNDCVD[] = {3, CHAR_C, CHAR_V, CHAR_D};
WORD aNDVCD20[] = {6, CHAR_V, CHAR_C, CHAR_D, CHAR_2, CHAR_FULL_STOP, CHAR_0};
WORD aNDVCD11[] = {6, CHAR_V, CHAR_C, CHAR_D, CHAR_1, CHAR_FULL_STOP, CHAR_1};
WORD aNDVCD10[] = {6, CHAR_V, CHAR_C, CHAR_D, CHAR_1, CHAR_FULL_STOP, CHAR_0};
WORD aNDCDDTS[] = {6, CHAR_C, CHAR_D, CHAR_HYPHEN_MINUS, CHAR_D, CHAR_T, CHAR_S};
WORD aNDHDCD[] = {4, CHAR_H, CHAR_D, CHAR_C, CHAR_D};
WORD aNDCDG[] = {3, CHAR_C, CHAR_D, CHAR_G};
WORD aNDCDDA[] = {4, CHAR_C, CHAR_D, CHAR_D, CHAR_A};
WORD aNDPictureCD[] = {10, CHAR_P, CHAR_i, CHAR_c, CHAR_t, CHAR_u, CHAR_r, CHAR_e, CHAR_SPACE, CHAR_C, CHAR_D};
WORD aNDMP2[] = {3, CHAR_M, CHAR_P, CHAR_2};
WORD aNDMP3[] = {3, CHAR_M, CHAR_P, CHAR_3};
WORD aNDJPEG[] = {4, CHAR_J, CHAR_P, CHAR_E, CHAR_G};
WORD aNDWMA[] = {3, CHAR_W, CHAR_M, CHAR_A};
WORD aNDMPG[] = {3, CHAR_M, CHAR_P, CHAR_G};
WORD aNDDAT[] = {3, CHAR_D, CHAR_A, CHAR_T};
WORD aNDAC3[] = {3, CHAR_A, CHAR_C, CHAR_3};
WORD aNDVOB[] = {3, CHAR_V, CHAR_O, CHAR_B};
WORD aNDCDROM[] = {5, CHAR_C, CHAR_D, CHAR_R, CHAR_O, CHAR_M};
WORD aNDDVDROM[] = {6, CHAR_D, CHAR_V, CHAR_D, CHAR_R, CHAR_O, CHAR_M};
WORD aNDUSB[] = {3, CHAR_U, CHAR_S, CHAR_B};
WORD aNDErrorData[] = {10, CHAR_E, CHAR_r, CHAR_r, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_D, CHAR_a, CHAR_t, CHAR_a};
WORD aNDErrorParental[] = {14, CHAR_E, CHAR_r, CHAR_r, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_P, CHAR_a, CHAR_r, CHAR_e, CHAR_n, CHAR_t, CHAR_a, CHAR_l};
WORD aNDIntro[] = {5, CHAR_I, CHAR_N, CHAR_T, CHAR_R, CHAR_O};
WORD aNDFullScreenOn[] = {14, CHAR_F, CHAR_u, CHAR_l, CHAR_l, CHAR_SPACE, CHAR_S, CHAR_c, CHAR_r, CHAR_e, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_O, CHAR_n};
WORD aNDFullScreenOff[] = {15, CHAR_F, CHAR_u, CHAR_l, CHAR_l, CHAR_SPACE, CHAR_S, CHAR_c, CHAR_r, CHAR_e, CHAR_e, CHAR_n, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
WORD aNDNTSC[] = {4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aNDAuto[] = {4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aNDPAL[] = {3, CHAR_P, CHAR_A, CHAR_L};
WORD aNDPALM[] = {5, CHAR_P, CHAR_A, CHAR_L, CHAR_LOW_LINE, CHAR_M};
WORD aNDPALN[] = {5, CHAR_P, CHAR_A, CHAR_L, CHAR_LOW_LINE, CHAR_N};
WORD aNDZoom25[] = {3, CHAR_2, CHAR_5, CHAR_N_PERCENT};
WORD aNDZoom50[] = {3, CHAR_5, CHAR_0, CHAR_N_PERCENT};
WORD aNDZoom100[] = {4, CHAR_1, CHAR_0, CHAR_0, CHAR_N_PERCENT};
WORD aNDZoom150[] = {4, CHAR_1, CHAR_5, CHAR_0, CHAR_N_PERCENT};
WORD aNDZoom200[] = {4, CHAR_2, CHAR_0, CHAR_0, CHAR_N_PERCENT};
WORD aNDNormal[] = {7, CHAR_S, CHAR_HYPHEN_MINUS, CHAR_V, CHAR_i, CHAR_d, CHAR_e, CHAR_o};
WORD aNDRGB[] = {3, CHAR_R, CHAR_G, CHAR_B};
WORD aNDYUV[]= {3, CHAR_Y, CHAR_U, CHAR_V};
WORD aNDVGA[]= {3, CHAR_V, CHAR_G, CHAR_A};
WORD aNDYUVPSCAN[] = {10, CHAR_Y, CHAR_U, CHAR_V, CHAR_SPACE, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
WORD aNDRGBPSCAN[] = {10, CHAR_R, CHAR_G, CHAR_B, CHAR_SPACE, CHAR_P, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_C, CHAR_A, CHAR_N};
#ifdef KARAOKE  
WORD aNDMic1[] = {6, CHAR_D, CHAR_O, CHAR_U, CHAR_B, CHAR_L, CHAR_E};
WORD aNDMic2[] = {6, CHAR_D, CHAR_U, CHAR_E, CHAR_T, CHAR_SPACE, CHAR_M};
WORD aNDMic3[] = {6, CHAR_D, CHAR_U, CHAR_E, CHAR_T, CHAR_SPACE, CHAR_F};
WORD aNDMic4[] = {6, CHAR_F, CHAR_SPACE, CHAR_T, CHAR_O, CHAR_SPACE, CHAR_M};
WORD aNDMic5[] = {6, CHAR_M, CHAR_SPACE, CHAR_T, CHAR_O, CHAR_SPACE, CHAR_F};
WORD aNDMic6[] = {6, CHAR_C, CHAR_H, CHAR_O, CHAR_R, CHAR_U, CHAR_S};
WORD aNDMic7[] = {7, CHAR_M, CHAR_O, CHAR_N, CHAR_S, CHAR_T, CHAR_E, CHAR_R};
WORD aNDMic8[] = {4, CHAR_D, CHAR_U, CHAR_C, CHAR_K};
#endif
#ifndef SUPPORT_JPEG_EFFECT_NAME
WORD aNDJPEGEffect[] = {7, CHAR_E, CHAR_f, CHAR_f, CHAR_e, CHAR_c, CHAR_t, CHAR_SPACE};
#endif
WORD aNDSlideTransitionTime[] = {2, CHAR_N_SCANF, CHAR_x};
WORD aNDMouhow[] = {1, CHAR_COLON};
WORD aNDSpeed[] = {1, CHAR_x};
WORD aNDSlash[] = {1, CHAR_SOLIDUS};
WORD aNDDash[] = {1, CHAR_HYPHEN_MINUS};
WORD aNDPlus[] = {1, CHAR_PLUS_SIGN};
WORD aNDSpace[] = {1, CHAR_SPACE};
WORD aNDRemain[] = {1, CHAR_HYPHEN_MINUS};
WORD aNDPercent[] = {1, CHAR_N_PERCENT};
WORD aND3DAudio[] = {2, CHAR_3, CHAR_D};
WORD aNDMixed[] = {5, CHAR_M, CHAR_i, CHAR_x, CHAR_e, CHAR_d};
WORD aNDAnalogOff[] = {10, CHAR_A, CHAR_n, CHAR_a, CHAR_l, CHAR_o, CHAR_g, CHAR_SPACE, CHAR_O, CHAR_f, CHAR_f};
WORD aNDAngle[] = {2, CHAR_N_ANGLE, CHAR_SPACE};
WORD aNDLanguage[] = {2, CHAR_N_AST, CHAR_COLON};
WORD aNDAudioDistance[] = {1, CHAR_SPACE};
WORD aNDPUnknownLanguage[] = {3, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS}; //"- - -"
WORD aNDMuteIcon[] = {1, CHAR_MUTE};
WORD aNDMuteOffIcon[] = {1, CHAR_MUTEOFF};
WORD aNDMultiply[] = {1, CHAR_x};
WORD aNDInvalid[] = {1, CHAR_N_INVALD};

#ifdef SUPPORT_3D_FEATURE
WORD aND3D1[] = {4, CHAR_SPACE, CHAR_N_3DL1, CHAR_N_3DR1, CHAR_SPACE};
WORD aND3D2[] = {4, CHAR_SPACE, CHAR_N_3DL2, CHAR_N_3DR2, CHAR_SPACE};
WORD aND3D3[] = {4, CHAR_SPACE, CHAR_N_3DL3, CHAR_N_3DR3, CHAR_SPACE};
WORD aND3D4[] = {4, CHAR_N_3DL4, CHAR_N_3DL3, CHAR_N_3DR3, CHAR_N_3DR4};
WORD aND3D5[] = {4, CHAR_N_3DL5, CHAR_N_3DL3, CHAR_N_3DR3, CHAR_N_3DR5};
WORD aND3D6[] = {4, CHAR_N_3DL6, CHAR_N_3DL3, CHAR_N_3DR3, CHAR_N_3DR6};
#endif
//show debug message
WORD aNDVersion[] = {4, CHAR_V, CHAR_E, CHAR_R, CHAR_SPACE};
WORD aNDError[] = {4, CHAR_E, CHAR_R, CHAR_R, CHAR_SPACE};

//Symbols for playing.
WORD aNDPreStop[] = {1, CHAR_N_PSTOP};
WORD aNDStop[] = {1, CHAR_N_STOP};
WORD aNDPause[] = {1, CHAR_N_PAUSE};
WORD aNDStep[] = {1, CHAR_N_STEP};
WORD aNDNextChapter[] = {1, CHAR_N_NEXT};
WORD aNDNextPage[] = {1, CHAR_N_NEXT};
WORD aNDNextAlbum[] = {1, CHAR_N_NEXT};
WORD aNDNextTrack[] = {1, CHAR_N_NEXT};
WORD aNDPrevChapter[] = {1, CHAR_N_PRE};
WORD aNDPrevPage[] = {1, CHAR_N_PRE};
WORD aNDPrevAlbum[] = {1, CHAR_N_PRE};
WORD aNDPrevTrack[] = {1, CHAR_N_PRE};
WORD aNDScanF[] = {1, CHAR_N_SCANF};
WORD aNDScanB[] = {1, CHAR_N_SCANB};
WORD aNDSlow[] = {1, CHAR_N_SLOW};
WORD aNDFast[] = {1, CHAR_N_SCANF};
WORD aNDPlay[] = {1, CHAR_N_PLAY};
WORD aNDRepeatSymbol[] = {2, CHAR_N_REPEAT, CHAR_SPACE};

//The language that we don't translate it.
WORD aNDLanguageDutch[] = {5, CHAR_D, CHAR_u, CHAR_t, CHAR_c, CHAR_h};
WORD aNDLanguageGreek[] = {5, CHAR_G, CHAR_r, CHAR_e, CHAR_e, CHAR_k};
WORD aNDLanguageLatin[] = {5, CHAR_L, CHAR_a, CHAR_t, CHAR_i, CHAR_n};
WORD aNDLanguageNorwegian[] = {3, CHAR_N, CHAR_o, CHAR_r};
WORD aNDLanguageDanish[] = {3, CHAR_D, CHAR_a, CHAR_n};
WORD aNDLanguageFinnish[] = {3, CHAR_F, CHAR_i, CHAR_n};
WORD aNDLanguageSwedish[] = {4, CHAR_S, CHAR_w, CHAR_e, CHAR_d};
WORD aNDLanguageHungarian[] = {4, CHAR_H, CHAR_u, CHAR_n, CHAR_g};
WORD aNDLanugagePolish[] = {3, CHAR_P, CHAR_o, CHAR_l};
WORD aNDLanguageIcelandic[] = {3, CHAR_I, CHAR_c, CHAR_e};
WORD aNDLanguageHebrew[] = {3, CHAR_H, CHAR_e, CHAR_b};
WORD aNDLanguageCzech[] = {5, CHAR_C, CHAR_z, CHAR_e, CHAR_c, CHAR_h};
WORD aNDLanguageArabic[] = {4, CHAR_A, CHAR_r, CHAR_a, CHAR_b};
WORD aNDLanguageMalay[] = {5, CHAR_M, CHAR_a, CHAR_l, CHAR_a, CHAR_y};
WORD aNDLanguageIndonesian[] = {5, CHAR_I, CHAR_n, CHAR_d, CHAR_o, CHAR_n};
WORD aNDLanguageTurkish[] = {4, CHAR_T, CHAR_u, CHAR_r, CHAR_k};
WORD aNDLanguageCroatian[] = {5, CHAR_C, CHAR_r, CHAR_o, CHAR_a, CHAR_t};
WORD aNDLanguageBulgarian[] = {4, CHAR_B, CHAR_u, CHAR_l, CHAR_g};
//***************************************************************************

//***************************************************************************
//OSD Display
WORD aDSPLYNull[] = {0};
WORD aDSPLYTitleIcon[] =  {2, CHAR_N_TITLE, CHAR_COLON};
WORD aDSPLYChapterIcon[] =  {2, CHAR_N_CHAP, CHAR_COLON};
WORD aDSPLYTrackIcon[] =  {2, CHAR_N_TITLE, CHAR_COLON};
WORD aDSPLYFolderIcon[] =  {2, CHAR_OPENFOLDER1, CHAR_COLON}; //to be confirmed.
WORD aDSPLYSPSTIcon[] = {2, CHAR_N_SPST, CHAR_COLON};
WORD aDSPLYASTIcon[] = {2, CHAR_N_AST, CHAR_COLON};
WORD aDSPLYTVIcon[] = {2, CHAR_N_TV_SYS, CHAR_COLON};
WORD aDSPLYAngleIcon[] = {2, CHAR_N_ANGLE, CHAR_COLON};
WORD aDSPLYUnknownTime[]= {8, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS, CHAR_COLON, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS, CHAR_COLON, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS};
WORD aDSPLYSlash[] = {1, CHAR_SOLIDUS};
WORD aDSPLYUnknownTrack[] = {2, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS}; //"_ _ "
WORD aDSPLYUnknownSubtitle[] = {3, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS}; //"- - -"
WORD aDSPLYUnknownLanguage[] = {3, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS}; //"- - -"
WORD aDSPLYUnknownAngle[] = {2, CHAR_HYPHEN_MINUS, CHAR_HYPHEN_MINUS}; //"_ _ "
WORD aDSPLYInfoDistance[] = {1, CHAR_SPACE};
WORD aDSPLYLanguageAudioTypeDistance[] = {1, CHAR_SPACE};
WORD aDSPLYTVDistance[] = {1, CHAR_SPACE};
WORD aDSPLYProgramSymbol[] = {1, CHAR_N_PRGM};
WORD aDSPLYRepeatSymbol[] = {1, CHAR_N_REPEAT};
WORD aDSPLYPreStopSymbol[] = {1, CHAR_N_PSTOP};
WORD aDSPLYStopSymbol[] = {1, CHAR_N_STOP};
WORD aDSPLYPauseSymbol[] = {1, CHAR_N_PAUSE};
WORD aDSPLYPlaySymbol[] = {1, CHAR_N_PLAY};
WORD aDSPLYStepSymbol[] = {1, CHAR_N_STEP};
WORD aDSPLYSCFSymbol[] = {1, CHAR_N_SCANF};
WORD aDSPLYSCBSymbol[] = {1, CHAR_N_SCANB};
WORD aDSPLYSlowSymbol[] = {1, CHAR_N_SLOW};
WORD aDSPLYNTSC[]={4, CHAR_N, CHAR_T, CHAR_S, CHAR_C};
WORD aDSPLYAuto[]={4, CHAR_A, CHAR_u, CHAR_t, CHAR_o};
WORD aDSPLYPAL[]={3, CHAR_P, CHAR_A, CHAR_L};
WORD aDSPLYPALM[]={5, CHAR_P, CHAR_A, CHAR_L, CHAR_LOW_LINE, CHAR_M};
WORD aDSPLYPALN[]={5, CHAR_P, CHAR_A, CHAR_L, CHAR_LOW_LINE, CHAR_N};
WORD aDSPLYStopTrack[] = {1, CHAR_1};
WORD aDSPLYCheckSymbol[] = {1, CHAR_N_CHECK};
WORD aDSPLYUpArrowSymbol[] = {1, CHAR_N_UP_16};
WORD aDSPLYDownArrowSymbol[] = {1, CHAR_N_DWN_16};
WORD aDSPLYLanguageDutch[] = {5, CHAR_D, CHAR_u, CHAR_t, CHAR_c, CHAR_h};
WORD aDSPLYLanguageGreek[] = {5, CHAR_G, CHAR_r, CHAR_e, CHAR_e, CHAR_k};
WORD aDSPLYLanguageLatin[] = {5, CHAR_L, CHAR_a, CHAR_t, CHAR_i, CHAR_n};
WORD aDSPLYLanguageNorwegian[] = {3, CHAR_N, CHAR_o, CHAR_r};
WORD aDSPLYLanguageDanish[] = {6, CHAR_D, CHAR_a, CHAR_n, CHAR_i, CHAR_s, CHAR_h};
WORD aDSPLYLanguageFinnish[] = {7, CHAR_F, CHAR_i, CHAR_n, CHAR_n, CHAR_i, CHAR_s, CHAR_h};
WORD aDSPLYLanguageSwedish[] = {7, CHAR_S, CHAR_w, CHAR_e, CHAR_d, CHAR_i, CHAR_s, CHAR_h};
WORD aDSPLYLanguageHungarian[] = {4, CHAR_H, CHAR_u, CHAR_n, CHAR_g};
WORD aDSPLYLanugagePolish[] = {6, CHAR_P, CHAR_o, CHAR_l, CHAR_i, CHAR_s, CHAR_h};
WORD aDSPLYLanguageIcelandic[] = {3, CHAR_I, CHAR_c, CHAR_e};
WORD aDSPLYLanguageHebrew[] = {6, CHAR_H, CHAR_e, CHAR_b, CHAR_r, CHAR_e, CHAR_w};
WORD aDSPLYLanguageCzech[] = {5, CHAR_C, CHAR_z, CHAR_e, CHAR_c, CHAR_h};
WORD aDSPLYLanguageArabic[] = {6, CHAR_A, CHAR_r, CHAR_a, CHAR_b, CHAR_i, CHAR_c};
WORD aDSPLYLanguageMalay[] = {5, CHAR_M, CHAR_a, CHAR_l, CHAR_a, CHAR_y};
WORD aDSPLYLanguageIndonesian[] = {5, CHAR_I, CHAR_n, CHAR_d, CHAR_o, CHAR_n};
WORD aDSPLYLanguageTurkish[] = {7, CHAR_T, CHAR_u, CHAR_r, CHAR_k, CHAR_i, CHAR_s, CHAR_h};
WORD aDSPLYLanguageCroatian[] = {5, CHAR_C, CHAR_r, CHAR_o, CHAR_a, CHAR_t};
WORD aDSPLYLanguageBulgarian[] = {4, CHAR_B, CHAR_u, CHAR_l, CHAR_g};
WORD aDSPLYReverseSolidus[] = {1, CHAR_REVERSE_SOLIDUS};
//***************************************************************************

//********************************************************************************************************
//Note: Media Manager UI
WORD aMMDVDROMDrive[] = {4, CHAR_R, CHAR_O, CHAR_O, CHAR_T};
WORD aMMUSBFlashDiskA[] = {16, CHAR_U, CHAR_S, CHAR_B, CHAR_SPACE, CHAR_F, CHAR_l, CHAR_a, CHAR_s, CHAR_h, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_k, CHAR_HYPHEN_MINUS, CHAR_A};
WORD aMMUSBFlashDiskB[] = {16, CHAR_U, CHAR_S, CHAR_B, CHAR_SPACE, CHAR_F, CHAR_l, CHAR_a, CHAR_s, CHAR_h, CHAR_SPACE, CHAR_D, CHAR_i, CHAR_s, CHAR_k, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aMMCardSDA[] = {10, CHAR_S, CHAR_D, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_A};
WORD aMMCardCFA[] = {10, CHAR_C, CHAR_F, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_A};
WORD aMMCardSMA[] = {10, CHAR_S, CHAR_M, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_A};
WORD aMMCardMMCA[] = {11, CHAR_M, CHAR_M, CHAR_C, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_A};
WORD aMMCardMSA[] = {10,  CHAR_M, CHAR_S, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_A};
WORD aMMCardMICROA[] = {13,  CHAR_M, CHAR_I, CHAR_C, CHAR_R, CHAR_O, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_A};
WORD aMMCardSDB[] = {10, CHAR_S, CHAR_D, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aMMCardCFB[] = {10, CHAR_C, CHAR_F, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aMMCardSMB[] = {10, CHAR_S, CHAR_M, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aMMCardMMCB[] = {11, CHAR_M, CHAR_M, CHAR_C, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aMMCardMSB[] = {10,  CHAR_M, CHAR_S, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_A};
WORD aMMCardMICROB[] = {13,  CHAR_M, CHAR_I, CHAR_C, CHAR_R, CHAR_O, CHAR_SPACE, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_HYPHEN_MINUS, CHAR_B};
WORD aMMUpDIR[] = {6, CHAR_U, CHAR_p, CHAR_LOW_LINE, CHAR_D, CHAR_I, CHAR_R};
//The following string is for icons
WORD aMMOpenFolderIcon[] = {1, CHAR_OPENFOLDER};
WORD aMMFolderIcon[] = {1, CHAR_FOLDER};
WORD aMMUpFolderIcon[] = {1, CHAR_UPFOLDER};
WORD aMMJPEGIcon[] = {1, CHAR_JPEG};
WORD aMMMP3Icon[] = {1, CHAR_MP3};
WORD aMMWMAIcon[] = {1, CHAR_WMA};
WORD aMMDIVXIcon[] = {1, CHAR_DIVX};
WORD aMMMP4Icon[] = {1, CHAR_MP4};
WORD aMMVOBIcon[] = {1, CHAR_VOB};
WORD aMMMP2Icon[] = {1, CHAR_MP2};
WORD aMMDATIcon[] = {1, CHAR_DAT};
WORD aMMAC3Icon[] = {1, CHAR_AC3};
WORD aMMMPGIcon[] = {1, CHAR_MPG};
WORD aMMDlgCheckedIcon[] = {1, CHAR_CHECK};
WORD aMMDlgUnCheckedIcon[] = {1, CHAR_UNCHECK};
WORD aMMDlgRadioSetIcon[] = {1, CHAR_RADIO};
WORD aMMDlgRadioNotSetIcon[] = {1, CHAR_UNRADIO};
WORD aMMVolumeServoIcon[] = {1, CHAR_LOADER};
WORD aMMVolumeUSBIcon[] = {1, CHAR_USB}; 
WORD aMMVolumeCardReaderIcon[] = {1, CHAR_CREADER}; 
WORD aMMMP3[] = {3, CHAR_M, CHAR_P, CHAR_3};
WORD aMMMP2[] = {3, CHAR_M, CHAR_P, CHAR_2};    
WORD aMMJPEG[] = {4, CHAR_J, CHAR_P, CHAR_E, CHAR_G};    
WORD aMMWMA[] = {3, CHAR_W, CHAR_M, CHAR_A}; 
WORD aMMVOB[] = {3, CHAR_V, CHAR_O, CHAR_B};    
WORD aMMDAT[] = {3, CHAR_D, CHAR_A, CHAR_T};    
WORD aMMDivX[] = {4, CHAR_D, CHAR_i, CHAR_v, CHAR_X};
WORD aMMAVI[] = {3, CHAR_A, CHAR_V, CHAR_I}; 
WORD aMMMPG[] = {3, CHAR_M, CHAR_P, CHAR_G};
WORD aMMSlash[] = {1, CHAR_SOLIDUS};
WORD aMMMDot[] = {1, CHAR_FULL_STOP};
WORD aMMMouhow[] = {1, CHAR_COLON};
WORD aMMPlayingFileTitle[] = {1, CHAR_N_TITLE};
WORD aMMPlayingFolderTitle[] = {1, CHAR_OPENFOLDER};
WORD aMMMAudioSP[] = {3, CHAR_K, CHAR_H, CHAR_z};
WORD aMMMAudioBitRate[] = {4, CHAR_K, CHAR_b, CHAR_p, CHAR_s};
WORD aMMHelpPlayIcon[] = {2, CHAR_N_PLAY, CHAR_COLON};
WORD aMMPLHelpSizeKB[] = {3, CHAR_SPACE, CHAR_K, CHAR_B};
WORD aMMPLHelpDate[] = {5, CHAR_D, CHAR_a, CHAR_t, CHAR_e, CHAR_COLON};
WORD aMMPLHelpSize[] = {5, CHAR_S, CHAR_i, CHAR_z, CHAR_e, CHAR_COLON};
//********************************************************************************************************

//********************************************************************************************************
//thumbnail
WORD aTHUMBSlash[] = {1, CHAR_SOLIDUS};
WORD aTHUMBExtMP2[] = {1, CHAR_MP2};
WORD aTHUMBExtMP3[] = {1, CHAR_MP3};
WORD aTHUMBExtDAT[] = {1, CHAR_DAT};
WORD aTHUMBExtVOB[] = {1, CHAR_VOB};
WORD aTHUMBExtMPEG4[] = {1, CHAR_MP4};
WORD aTHUMBExtWMA[] = {1, CHAR_WMA};
WORD aTHUMBExtMPG[] = {1, CHAR_MPG};
WORD aTHUMBX[] = {1, CHAR_x};
WORD aTHUMBProgramSymbol[] = {1, CHAR_N_PRGM};
WORD aTHUMBHelpPlayIcon[] = {2, CHAR_N_PLAY, CHAR_COLON};
//********************************************************************************************************

//********************************************************************************************************
//OSD Dlg.
WORD aDLGBtnDVDROM[] = {13, CHAR_D, CHAR_V, CHAR_D, CHAR_HYPHEN_MINUS, CHAR_R, CHAR_O, CHAR_M, CHAR_SPACE, CHAR_D, CHAR_r, CHAR_i, CHAR_v, CHAR_e};
WORD aDLGBtnUSB[] = {3, CHAR_U, CHAR_S, CHAR_B};
#ifdef SUPPORT_STB
WORD aDLGBtnSTB[] = {3, CHAR_D, CHAR_V, CHAR_B };
#endif

#ifdef SUPPORT_BUILDIN_CARD_READER
WORD aDLGBtnCardReader[] = {11, CHAR_C, CHAR_a, CHAR_r, CHAR_d, CHAR_SPACE, CHAR_R, CHAR_e, CHAR_a, CHAR_d, CHAR_e, CHAR_r};
#endif
#ifdef SUPPORT_AV_IN
WORD aDLGBtnAVIN[] = {5, CHAR_A, CHAR_V, CHAR_SPACE, CHAR_I ,CHAR_N};
#endif

WORD aOSDCOMDLG_OK[] = {2, CHAR_O, CHAR_K};
WORD aOSDCOMDLG_Cancel[] = {6, CHAR_C, CHAR_a, CHAR_n, CHAR_c, CHAR_e, CHAR_l};
WORD aOSDCOMDLG_Yes[] = {3, CHAR_Y, CHAR_e, CHAR_s};
WORD aOSDCOMDLG_No[] = {2, CHAR_N, CHAR_o};
//********************************************************************************************************

//********************************************************************************************************
//Program UI
WORD aArrowDown[] = {1, CHAR_N_DWN_16}; 
WORD aArrowUp[] = {1, CHAR_N_UP_16}; 
WORD aPlayIcon[] = {1, CHAR_N_PLAY};
WORD aDirIcon[] = {7, CHAR_LEFT, CHAR_SPACE, CHAR_UP, CHAR_SPACE, CHAR_RIGHT, CHAR_SPACE, CHAR_DOWN};
WORD aColon[] = {1, CHAR_COLON};
WORD aClearIcon[] = {1, CHAR_N_STOP};
//********************************************************************************************************

//********************************************************************************************************
//Search UI
#ifndef NO_SEARCH_UI //++CoCo2.37p
WORD aTitleIcon[] = {1, CHAR_N_TITLE};  
WORD aChapterIcon[] = {1, CHAR_N_CHAP};             
WORD aTimeIcon[] = {1, CHAR_N_TIME};   
#endif    
//********************************************************************************************************

//********************************************************************************************************
//Digest
#ifndef NO_DIGEST //++CoCo2.37p
WORD aDGSlash[] = {1, CHAR_SOLIDUS};
WORD aDGMouhow[] = {1, CHAR_COLON};
#endif //#ifndef NO_DIGEST //++CoCo2.37p
//********************************************************************************************************

//********************************************************************************************************
//DivX
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

// LLY2.53, remove this to osddivx.c since "notransl.o (*.data)" will be put @ "rodata" section
// It means that the data is read only can be modified
// But, current code will update the remain view count within aDivXReentalConfirm[]
#if 0
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
#endif  // #if 0
//********************************************************************************************************

//********************************************************************************************************
//setup
WORD aSETUPFW[] = {3, CHAR_F, CHAR_SOLIDUS, CHAR_W};
WORD aSETUPFAE[] = {3, CHAR_F, CHAR_A, CHAR_E};
WORD aSETUPMPEG[] = {4, CHAR_M, CHAR_P, CHAR_E, CHAR_G};
WORD aSETUPJPG[] = {3, CHAR_J, CHAR_P, CHAR_G};
WORD aSETUPDisp[] = {4, CHAR_D, CHAR_i, CHAR_s, CHAR_p};
WORD aSETUPDivX[] = {4, CHAR_D, CHAR_i, CHAR_v, CHAR_X};
WORD aSETUPNav[] = {3, CHAR_N, CHAR_a, CHAR_v};
WORD aSETUPParser[] = {6, CHAR_P, CHAR_a, CHAR_r, CHAR_s, CHAR_e, CHAR_r};
WORD aSETUPInfo[] = {4, CHAR_I, CHAR_n, CHAR_f, CHAR_o};
WORD aSETUPAudio[] = {5, CHAR_A, CHAR_u, CHAR_d, CHAR_i, CHAR_o};
WORD aSETUPUSB[] = {3, CHAR_U, CHAR_S, CHAR_B};
WORD aSETUPCard[] ={4, CHAR_C, CHAR_a, CHAR_r, CHAR_d};
WORD aSETUPASupport[] = {9, CHAR_A, CHAR_HYPHEN_MINUS, CHAR_S, CHAR_u, CHAR_p, CHAR_p, CHAR_o, CHAR_r, CHAR_t};
#ifdef SUPPORT_TFT
WORD aSETUPTFT[] = {2, CHAR_V, CHAR_D};
#endif

WORD aSETUPServo[] = {5, CHAR_S, CHAR_e, CHAR_r, CHAR_v, CHAR_o};
WORD aSETUPBM[] = {2, CHAR_B, CHAR_M};
WORD aSETUPuCode[] = {5, CHAR_u, CHAR_C, CHAR_o, CHAR_d, CHAR_e};
WORD aSETUPPCBSYSTEM[] = {10, CHAR_P, CHAR_C, CHAR_B, CHAR_SPACE, CHAR_S, CHAR_Y, CHAR_S, CHAR_T, CHAR_E, CHAR_M};
WORD aSETUPRFTYPE[] = {7, CHAR_R, CHAR_F, CHAR_SPACE, CHAR_T, CHAR_Y, CHAR_P, CHAR_E};
WORD aSETUPMOTORTYPE[] = {10, CHAR_M, CHAR_O, CHAR_T, CHAR_O, CHAR_R, CHAR_SPACE, CHAR_T, CHAR_Y, CHAR_P, CHAR_E};
WORD aSETUPLOADER[] = {6, CHAR_L, CHAR_O, CHAR_A, CHAR_D, CHAR_E, CHAR_R};
WORD aSETUPPassword[] = {4, CHAR_X, CHAR_X, CHAR_X, CHAR_X};
WORD aSETUPDebugModeInfo[] = {15, CHAR_D, CHAR_e, CHAR_b,CHAR_u,CHAR_g, CHAR_SPACE, CHAR_M, CHAR_o, CHAR_d, CHAR_e, CHAR_SPACE, CHAR_I, CHAR_n, CHAR_f, CHAR_o};
WORD aSETUPDefault[] = {10, CHAR_FULL_STOP, CHAR_D, CHAR_e, CHAR_f, CHAR_a, CHAR_u, CHAR_l, CHAR_t, CHAR_COLON, CHAR_SPACE};
WORD aSETUPNew[] = {6, CHAR_FULL_STOP, CHAR_N, CHAR_e, CHAR_w, CHAR_COLON, CHAR_SPACE};
WORD aSETUPDSU_TX[] =  {9, CHAR_FULL_STOP, CHAR_D, CHAR_S, CHAR_U, CHAR_LOW_LINE, CHAR_T, CHAR_X, CHAR_COLON, CHAR_SPACE};
WORD aSETUPDSU_RX[] =  {9, CHAR_FULL_STOP, CHAR_D, CHAR_S, CHAR_U, CHAR_LOW_LINE, CHAR_R, CHAR_X, CHAR_COLON, CHAR_SPACE};
WORD aSETUPUR1_TX[] =  {9, CHAR_FULL_STOP, CHAR_U, CHAR_R, CHAR_1, CHAR_LOW_LINE, CHAR_T, CHAR_X, CHAR_COLON, CHAR_SPACE};
WORD aSETUPUR1_RX[] =  {9, CHAR_FULL_STOP, CHAR_U, CHAR_R, CHAR_1, CHAR_LOW_LINE, CHAR_R, CHAR_X, CHAR_COLON, CHAR_SPACE};
WORD aSETUPUR2_TX[] =  {9, CHAR_FULL_STOP, CHAR_U, CHAR_R, CHAR_2, CHAR_LOW_LINE, CHAR_T, CHAR_X, CHAR_COLON, CHAR_SPACE};
WORD aSETUPUR2_RX[] =  {9, CHAR_FULL_STOP, CHAR_U, CHAR_R, CHAR_2, CHAR_LOW_LINE, CHAR_R, CHAR_X, CHAR_COLON, CHAR_SPACE};
WORD aSETUPEnable[] = {6, CHAR_E, CHAR_n, CHAR_a, CHAR_b, CHAR_l, CHAR_e};
WORD aSETUPDisable[] = {7, CHAR_D, CHAR_i, CHAR_s, CHAR_a, CHAR_b, CHAR_l, CHAR_e};
WORD aSETUPDBGInfo1[] = {14, CHAR_C, CHAR_A, CHAR_R, CHAR_D, CHAR_SPACE, CHAR_c, CHAR_a, CHAR_n, CHAR_APOSTROPHE, CHAR_t, CHAR_SPACE, CHAR_u, CHAR_s, CHAR_e};
WORD aSETUPDBGInfo2[] = {17, CHAR_X, CHAR_D, CHAR_COMMA, CHAR_SPACE, CHAR_D, CHAR_V, CHAR_B, CHAR_SPACE, CHAR_c, CHAR_a, CHAR_n, CHAR_APOSTROPHE, CHAR_t, CHAR_SPACE, CHAR_u, CHAR_s, CHAR_e};
WORD aSETUPNotSet[] = {8, CHAR_x, CHAR_x, CHAR_x, CHAR_x, CHAR_x, CHAR_x, CHAR_x, CHAR_x};
//********************************************************************************************************
//CoCo2.38
#ifdef IMAGE_FRAME_MENU_MAIN_FILE	
#ifndef SUPPORT_STB								
WORD aMENULB[] = {3, CHAR_4, CHAR_COLON, CHAR_3};
WORD aMENUWide[] = {4, CHAR_1, CHAR_6, CHAR_COLON, CHAR_9};
#endif
WORD aMENUMinus7[] = {2, CHAR_HYPHEN_MINUS, CHAR_7};
WORD aMENUMinus6[] = {2, CHAR_HYPHEN_MINUS, CHAR_6};
WORD aMENUMinus5[] = {2, CHAR_HYPHEN_MINUS, CHAR_5};
WORD aMENUMinus4[] = {2, CHAR_HYPHEN_MINUS, CHAR_4};
WORD aMENUMinus3[] = {2, CHAR_HYPHEN_MINUS, CHAR_3};
WORD aMENUMinus2[] = {2, CHAR_HYPHEN_MINUS, CHAR_2};
WORD aMENUMinus1[] = {2, CHAR_HYPHEN_MINUS, CHAR_1};
WORD aMENU0[] = {1, CHAR_0};
WORD aMENUPlus1[] = {2, CHAR_PLUS_SIGN, CHAR_1};
WORD aMENUPlus2[] = {2, CHAR_PLUS_SIGN, CHAR_2};
WORD aMENUPlus3[] = {2, CHAR_PLUS_SIGN, CHAR_3};
WORD aMENUPlus4[] = {2, CHAR_PLUS_SIGN, CHAR_4};
WORD aMENUPlus5[] = {2, CHAR_PLUS_SIGN, CHAR_5};
WORD aMENUPlus6[] = {2, CHAR_PLUS_SIGN, CHAR_6};
WORD aMENUPlus7[] = {2, CHAR_PLUS_SIGN, CHAR_7};
WORD aMENUIconSTB[] = {3, CHAR_S, CHAR_T, CHAR_B};
WORD aPOWERONIconGame[] = {4, CHAR_G, CHAR_a, CHAR_m, CHAR_e};
#endif //#ifdef IMAGE_FRAME_MENU_MAIN_FILE									
//********************************************************************************************************

WORD aNDXD[] = {2, CHAR_X, CHAR_D};
WORD aNDSD[] = {2, CHAR_S, CHAR_D};
WORD aNDMS[] = {2, CHAR_M, CHAR_S};
WORD aNDMMC[] = {3, CHAR_M, CHAR_M, CHAR_C};
WORD aNDCF[] = {2, CHAR_C, CHAR_F};

WORD aMENUTV[] = {2, CHAR_T, CHAR_V};

#ifdef SUPPORT_AUDIO_EFFECT
// Damon2.76, for audio EQ
WORD aAudioNormal[] = {6, CHAR_N, CHAR_o, CHAR_r, CHAR_m, CHAR_a, CHAR_l};
WORD aAudioBassBooster[] = {12, CHAR_B, CHAR_a, CHAR_s, CHAR_s, CHAR_SPACE, CHAR_B, CHAR_o, CHAR_o, CHAR_s, CHAR_t, CHAR_e, CHAR_r};
WORD aAudioFullTreble[] = {11, CHAR_F, CHAR_u, CHAR_l, CHAR_l, CHAR_SPACE, CHAR_T, CHAR_r, CHAR_e, CHAR_b, CHAR_l, CHAR_e};
WORD aAudioJazz[] = {4, CHAR_J, CHAR_a, CHAR_z, CHAR_z};
WORD aAudioRock[] = {4, CHAR_R, CHAR_o, CHAR_c, CHAR_k};
WORD aAudioClassical[] = {9, CHAR_C, CHAR_l, CHAR_a, CHAR_s, CHAR_s, CHAR_i, CHAR_c, CHAR_a, CHAR_l};
WORD aAudioPop[] = {3, CHAR_P, CHAR_o, CHAR_p};
#endif

#ifdef SUPPORT_RADIO_TUNER
WORD aRADIOSpace[] = {1, CHAR_SPACE};
WORD aRADIODot[] = {1, CHAR_FULL_STOP};
WORD aRADIOColon[] = {1, CHAR_COLON};
WORD aRADIOSolidus[] = {1, CHAR_SOLIDUS};
WORD aRADIO875[] = {4, CHAR_8, CHAR_7, CHAR_FULL_STOP, CHAR_5}; 
WORD aRADIO93[] = {2, CHAR_9, CHAR_3};
WORD aRADIO98[] = {2, CHAR_9, CHAR_8};
WORD aRADIO103[] = {3, CHAR_1, CHAR_0, CHAR_3};
WORD aRADIO108[] = {3, CHAR_1, CHAR_0, CHAR_8};
#endif

//peteryu275, add for IC check version, 2007/3/22 11:31AM
WORD aAP_IC_VERSION_ERROR[] = {19, CHAR_A, CHAR_P, CHAR_SPACE, CHAR_I, CHAR_C, CHAR_SPACE, CHAR_V, CHAR_e, CHAR_r, CHAR_s, CHAR_i, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_E, CHAR_r, CHAR_r, CHAR_o, CHAR_r};

WORD aNDJPEGEffectBlindVertical[] = {14, CHAR_B, CHAR_l, CHAR_i, CHAR_n, CHAR_d, CHAR_SPACE, CHAR_V, CHAR_e, CHAR_r, CHAR_t, CHAR_i, CHAR_c, CHAR_a, CHAR_l};
WORD aNDJPEGEffectBlindHorizontal[] = {16, CHAR_B, CHAR_l, CHAR_i, CHAR_n, CHAR_d, CHAR_SPACE, CHAR_H, CHAR_o, CHAR_r, CHAR_i, CHAR_z, CHAR_o, CHAR_n, CHAR_t, CHAR_a, CHAR_l};
WORD aNDJPEGEffectColorRationVertical[] = {16, CHAR_C, CHAR_o, CHAR_l, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_R, CHAR_a, CHAR_t, CHAR_i, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_V, CHAR_e, CHAR_r};
WORD aNDJPEGEffectColorRationHorizontal[] = {16, CHAR_C, CHAR_o, CHAR_l, CHAR_o, CHAR_r, CHAR_SPACE, CHAR_R, CHAR_a, CHAR_t, CHAR_i, CHAR_o, CHAR_n, CHAR_SPACE, CHAR_H, CHAR_o, CHAR_r};
WORD aNDJPEGEffectBlock1[] = {7, CHAR_B, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_1};
WORD aNDJPEGEffectBlock2[] = {7, CHAR_B, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_2};
WORD aNDJPEGEffectBlock3[] = {7, CHAR_B, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_3};
WORD aNDJPEGEffectBlock4[] = {7, CHAR_B, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_4};
WORD aNDJPEGEffectBlock5[] = {7, CHAR_B, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_5};
WORD aNDJPEGEffectBlock6[] = {7, CHAR_B, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_6};
WORD aNDJPEGEffectBlock7[] = {7, CHAR_B, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_7};
WORD aNDJPEGEffectBlock8[] = {7, CHAR_B, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_8};
WORD aNDJPEGEffectBlock9[] = {7, CHAR_B, CHAR_l, CHAR_o, CHAR_c, CHAR_k, CHAR_SPACE, CHAR_9};

#ifdef SUPPORT_USB_HOST_DEVICE
WORD aUSBDEVICEDlgTitle[] = {4, CHAR_N, CHAR_o, CHAR_t, CHAR_e};
WORD aUSBDEVICEDlgNotifyContent[] = {13, CHAR_C, CHAR_o, CHAR_n, CHAR_n, CHAR_e, CHAR_c, CHAR_t, CHAR_SPACE, CHAR_t, CHAR_o, CHAR_SPACE, CHAR_P, CHAR_C};
WORD aUSBDEVICEDlgPowerDownContent[] = {17, CHAR_P, CHAR_l, CHAR_e, CHAR_a, CHAR_s, CHAR_e, CHAR_SPACE, CHAR_p, CHAR_o, CHAR_w, CHAR_e, CHAR_r, CHAR_SPACE, CHAR_d, CHAR_o, CHAR_w, CHAR_n};
#endif //SUPPORT_USB_HOST_DEVICE