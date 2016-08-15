
//***************************************************************************
//Normal Display
extern WORD aNDNumbers[];
extern WORD aNDUppercase[];
extern WORD aNDLowercase[];
extern WORD aND51Channel[];
extern WORD aND71Channel[];
extern WORD aND2Channel[];
extern WORD aNDChannel[];
extern WORD aNDMPEG[];
extern WORD aNDLPCM[];
extern WORD aNDPCM[];
extern WORD aNDDTS[];
extern WORD aNDDVD[];
extern WORD aNDSVCD[];
extern WORD aNDCVD[];
extern WORD aNDVCD20[];
extern WORD aNDVCD11[];
extern WORD aNDVCD10[];
extern WORD aNDCDDTS[];
extern WORD aNDHDCD[];
extern WORD aNDCDG[];
extern WORD aNDCDDA[];
extern WORD aNDPictureCD[];
extern WORD aNDMP2[];
extern WORD aNDMP3[];
extern WORD aNDJPEG[];
extern WORD aNDWMA[];
extern WORD aNDMPG[];
extern WORD aNDDAT[];
extern WORD aNDAC3[];
extern WORD aNDVOB[];
extern WORD aNDCDROM[];
extern WORD aNDDVDROM[];
extern WORD aNDUSB[];
extern WORD aNDErrorData[];
extern WORD aNDErrorParental[];
extern WORD aNDIntro[];
extern WORD aNDFullScreenOn[];
extern WORD aNDFullScreenOff[];
extern WORD aNDNTSC[];
extern WORD aNDAuto[];
extern WORD aNDPAL[];
extern WORD aNDPALM[];
extern WORD aNDPALN[];
extern WORD aNDZoom25[];
extern WORD aNDZoom50[];
extern WORD aNDZoom100[];
extern WORD aNDZoom150[];
extern WORD aNDZoom200[];
extern WORD aNDNormal[];
extern WORD aNDRGB[];
extern WORD aNDYUV[];
extern WORD aNDVGA[];
extern WORD aNDYUVPSCAN[];
extern WORD aNDRGBPSCAN[];
#ifdef KARAOKE  
extern WORD aNDMic1[];
extern WORD aNDMic2[];
extern WORD aNDMic3[];
extern WORD aNDMic4[];
extern WORD aNDMic5[];
extern WORD aNDMic6[];
extern WORD aNDMic7[];
extern WORD aNDMic8[];
#endif
#ifndef SUPPORT_JPEG_EFFECT_NAME
extern WORD aNDJPEGEffect[];
#endif
extern WORD aNDSlideTransitionTime[];
extern WORD aNDMouhow[];
extern WORD aNDSpeed[];
extern WORD aNDSlash[];
extern WORD aNDDash[];
extern WORD aNDPlus[];
extern WORD aNDSpace[];
extern WORD aNDRemain[];
extern WORD aNDPercent[];
extern WORD aND3DAudio[];
extern WORD aNDMixed[];
extern WORD aNDAnalogOff[];
extern WORD aNDAngle[];
extern WORD aNDLanguage[];
extern WORD aNDAudioDistance[];
extern WORD aNDPUnknownLanguage[]; //"- - -"
extern WORD aNDMuteIcon[];
extern WORD aNDMuteOffIcon[];
extern WORD aNDMultiply[];
extern WORD aNDInvalid[];

#ifdef SUPPORT_3D_FEATURE
extern WORD aND3D1[];
extern WORD aND3D2[];
extern WORD aND3D3[];
extern WORD aND3D4[];
extern WORD aND3D5[];
extern WORD aND3D6[];
#endif
//show debug message
extern WORD aNDVersion[];
extern WORD aNDError[];

//Symbols for playing.
extern WORD aNDPreStop[];
extern WORD aNDStop[];
extern WORD aNDPause[];
extern WORD aNDStep[];
extern WORD aNDNextChapter[];
extern WORD aNDNextPage[];
extern WORD aNDNextAlbum[];
extern WORD aNDNextTrack[];
extern WORD aNDPrevChapter[];
extern WORD aNDPrevPage[];
extern WORD aNDPrevAlbum[];
extern WORD aNDPrevTrack[];
extern WORD aNDScanF[];
extern WORD aNDScanB[];
extern WORD aNDSlow[];
extern WORD aNDFast[];
extern WORD aNDPlay[];
extern WORD aNDRepeatSymbol[];

//The language that we don't translate it.
extern WORD aNDLanguageDutch[];
extern WORD aNDLanguageGreek[];
extern WORD aNDLanguageLatin[];
extern WORD aNDLanguageNorwegian[];
extern WORD aNDLanguageDanish[];
extern WORD aNDLanguageFinnish[];
extern WORD aNDLanguageSwedish[];
extern WORD aNDLanguageHungarian[];
extern WORD aNDLanugagePolish[];
extern WORD aNDLanguageIcelandic[];
extern WORD aNDLanguageHebrew[];
extern WORD aNDLanguageCzech[];
extern WORD aNDLanguageArabic[];
extern WORD aNDLanguageMalay[];
extern WORD aNDLanguageIndonesian[];
extern WORD aNDLanguageTurkish[];
extern WORD aNDLanguageCroatian[];
extern WORD aNDLanguageBulgarian[];
//***************************************************************************

//***************************************************************************
//OSD Display
extern WORD aDSPLYNull[];
extern WORD aDSPLYTitleIcon[];
extern WORD aDSPLYChapterIcon[];
extern WORD aDSPLYTrackIcon[];
extern WORD aDSPLYFolderIcon[]; //to be confirmed.
extern WORD aDSPLYSPSTIcon[];
extern WORD aDSPLYASTIcon[];
extern WORD aDSPLYTVIcon[];
extern WORD aDSPLYAngleIcon[];
extern WORD aDSPLYUnknownTime[];
extern WORD aDSPLYSlash[];
extern WORD aDSPLYUnknownTrack[]; //"_ _ "
extern WORD aDSPLYUnknownSubtitle[]; //"- - -"
extern WORD aDSPLYUnknownLanguage[]; //"- - -"
extern WORD aDSPLYUnknownAngle[]; //"_ _ "
extern WORD aDSPLYInfoDistance[];
extern WORD aDSPLYLanguageAudioTypeDistance[];
extern WORD aDSPLYTVDistance[];
extern WORD aDSPLYProgramSymbol[];
extern WORD aDSPLYRepeatSymbol[];
extern WORD aDSPLYPreStopSymbol[];
extern WORD aDSPLYStopSymbol[];
extern WORD aDSPLYPauseSymbol[];
extern WORD aDSPLYPlaySymbol[];
extern WORD aDSPLYStepSymbol[];
extern WORD aDSPLYSCFSymbol[];
extern WORD aDSPLYSCBSymbol[];
extern WORD aDSPLYSlowSymbol[];
extern WORD aDSPLYNTSC[];
extern WORD aDSPLYAuto[];
extern WORD aDSPLYPAL[];
extern WORD aDSPLYPALM[];
extern WORD aDSPLYPALN[];
extern WORD aDSPLYStopTrack[];
extern WORD aDSPLYCheckSymbol[];
extern WORD aDSPLYUpArrowSymbol[];
extern WORD aDSPLYDownArrowSymbol[];
extern WORD aDSPLYLanguageDutch[];
extern WORD aDSPLYLanguageGreek[];
extern WORD aDSPLYLanguageLatin[];
extern WORD aDSPLYLanguageNorwegian[];
extern WORD aDSPLYLanguageDanish[];
extern WORD aDSPLYLanguageFinnish[];
extern WORD aDSPLYLanguageSwedish[];
extern WORD aDSPLYLanguageHungarian[];
extern WORD aDSPLYLanugagePolish[];
extern WORD aDSPLYLanguageIcelandic[];
extern WORD aDSPLYLanguageHebrew[];
extern WORD aDSPLYLanguageCzech[];
extern WORD aDSPLYLanguageArabic[];
extern WORD aDSPLYLanguageMalay[];
extern WORD aDSPLYLanguageIndonesian[];
extern WORD aDSPLYLanguageTurkish[];
extern WORD aDSPLYLanguageCroatian[];
extern WORD aDSPLYLanguageBulgarian[];
extern WORD aDSPLYReverseSolidus[];
//***************************************************************************

//********************************************************************************************************
//Note: Media Manager UI
extern WORD aMMDVDROMDrive[];
extern WORD aMMUSBFlashDiskA[];
extern WORD aMMUSBFlashDiskB[];
extern WORD aMMCardSDA[];
extern WORD aMMCardCFA[];
extern WORD aMMCardSMA[];
extern WORD aMMCardMMCA[];
extern WORD aMMCardMSA[];
extern WORD aMMCardMICROA[];
extern WORD aMMCardSDB[];
extern WORD aMMCardCFB[];
extern WORD aMMCardSMB[];
extern WORD aMMCardMMCB[];
extern WORD aMMCardMSB[];
extern WORD aMMCardMICROB[];
extern WORD aMMUpDIR[];
//The following string is for icons
extern WORD aMMOpenFolderIcon[];
extern WORD aMMFolderIcon[];
extern WORD aMMUpFolderIcon[];
extern WORD aMMJPEGIcon[];
extern WORD aMMMP3Icon[];
extern WORD aMMWMAIcon[];
extern WORD aMMDIVXIcon[];
extern WORD aMMMP4Icon[];
extern WORD aMMVOBIcon[];
extern WORD aMMMP2Icon[];
extern WORD aMMDATIcon[];
extern WORD aMMAC3Icon[];
extern WORD aMMMPGIcon[];
extern WORD aMMDlgCheckedIcon[];
extern WORD aMMDlgUnCheckedIcon[];
extern WORD aMMDlgRadioSetIcon[];
extern WORD aMMDlgRadioNotSetIcon[];
extern WORD aMMVolumeServoIcon[];
extern WORD aMMVolumeUSBIcon[]; 
extern WORD aMMVolumeCardReaderIcon[]; 
extern WORD aMMMP3[];
extern WORD aMMMP2[];    
extern WORD aMMJPEG[];    
extern WORD aMMWMA[]; 
extern WORD aMMVOB[];    
extern WORD aMMDAT[];    
extern WORD aMMDivX[];
extern WORD aMMAVI[]; 
extern WORD aMMMPG[];
extern WORD aMMSlash[];
extern WORD aMMMDot[];
extern WORD aMMMouhow[];
extern WORD aMMPlayingFileTitle[];
extern WORD aMMPlayingFolderTitle[];
extern WORD aMMMAudioSP[];
extern WORD aMMMAudioBitRate[];
extern WORD aMMHelpPlayIcon[];
extern WORD aMMPLHelpSizeKB[];
extern WORD aMMPLHelpDate[];
extern WORD aMMPLHelpSize[];
//********************************************************************************************************

//********************************************************************************************************
//thumbnail
extern WORD aTHUMBSlash[];
extern WORD aTHUMBExtMP2[];
extern WORD aTHUMBExtMP3[];
extern WORD aTHUMBExtDAT[];
extern WORD aTHUMBExtVOB[];
extern WORD aTHUMBExtMPEG4[];
extern WORD aTHUMBExtWMA[];
extern WORD aTHUMBExtMPG[];
extern WORD aTHUMBX[];
extern WORD aTHUMBProgramSymbol[];
extern WORD aTHUMBHelpPlayIcon[];
//********************************************************************************************************

//********************************************************************************************************
//OSD Dlg.
extern WORD aDLGBtnDVDROM[];
extern WORD aDLGBtnUSB[];
#ifdef SUPPORT_STB
extern WORD aDLGBtnSTB[];
#endif
#ifdef SUPPORT_BUILDIN_CARD_READER
extern WORD aDLGBtnCardReader[];
#endif
#ifdef SUPPORT_AV_IN
extern WORD aDLGBtnAVIN[] ;
#endif
extern WORD aOSDCOMDLG_OK[];
extern WORD aOSDCOMDLG_Cancel[];
extern WORD aOSDCOMDLG_Yes[];
extern WORD aOSDCOMDLG_No[];
//********************************************************************************************************

//********************************************************************************************************
//Program UI
extern WORD aArrowDown[]; 
extern WORD aArrowUp[]; 
extern WORD aPlayIcon[];
extern WORD aDirIcon[];
extern WORD aColon[];
extern WORD aClearIcon[];
//********************************************************************************************************

//********************************************************************************************************
//Search UI
extern WORD aTitleIcon[];  
extern WORD aChapterIcon[];             
extern WORD aTimeIcon[];       
//********************************************************************************************************

//********************************************************************************************************
//Digest
extern WORD aDGSlash[];
extern WORD aDGMouhow[];
//********************************************************************************************************

//********************************************************************************************************
//DivX
extern WORD aDivXRegCode1[];
extern WORD aDivXRegCode2[];
extern WORD aDivXRegCode3[];
extern WORD aDivXAuthorError[];
extern WORD aDivXAuthorExpired[];
extern WORD aDivXRentalConfirm[];
//********************************************************************************************************

//********************************************************************************************************
//setup
extern WORD aSETUPFW[];
extern WORD aSETUPFAE[];
extern WORD aSETUPMPEG[];
extern WORD aSETUPJPG[];
extern WORD aSETUPDisp[];
extern WORD aSETUPDivX[];
extern WORD aSETUPNav[];
extern WORD aSETUPParser[];
extern WORD aSETUPInfo[];
extern WORD aSETUPAudio[];
extern WORD aSETUPUSB[];
extern WORD aSETUPCard[];
extern WORD aSETUPASupport[];
#ifdef SUPPORT_TFT
extern WORD aSETUPTFT[];
#endif

extern WORD aSETUPServo[];
extern WORD aSETUPBM[];
extern WORD aSETUPuCode[];
extern WORD aSETUPPCBSYSTEM[];
extern WORD aSETUPRFTYPE[];
extern WORD aSETUPMOTORTYPE[];
extern WORD aSETUPLOADER[];
extern WORD aSETUPPassword[];
extern WORD aSETUPDebugModeInfo[];
extern WORD aSETUPDefault[];
extern WORD aSETUPNew[];
extern WORD aSETUPDSU_TX[];
extern WORD aSETUPDSU_RX[];
extern WORD aSETUPUR1_TX[];
extern WORD aSETUPUR1_RX[];
extern WORD aSETUPUR2_TX[];
extern WORD aSETUPUR2_RX[];
extern WORD aSETUPEnable[];
extern WORD aSETUPDisable[];
extern WORD aSETUPDBGInfo1[];
extern WORD aSETUPDBGInfo2[];
extern WORD aSETUPNotSet[];
//********************************************************************************************************
//CoCo2.38
extern WORD aMENULB[];
extern WORD aMENUWide[];
extern WORD aMENUMinus7[];
extern WORD aMENUMinus6[];
extern WORD aMENUMinus5[];
extern WORD aMENUMinus4[];
extern WORD aMENUMinus3[];
extern WORD aMENUMinus2[];
extern WORD aMENUMinus1[];
extern WORD aMENU0[];
extern WORD aMENUPlus1[];
extern WORD aMENUPlus2[];
extern WORD aMENUPlus3[];
extern WORD aMENUPlus4[];
extern WORD aMENUPlus5[];
extern WORD aMENUPlus6[];
extern WORD aMENUPlus7[];
extern WORD aMENUIconSTB[];
extern WORD aPOWERONIconGame[];

extern WORD aNDXD[];
extern WORD aNDSD[];
extern WORD aNDMS[];
extern WORD aNDMMC[];
extern WORD aNDCF[];

extern WORD aMENUTV[];

#ifdef SUPPORT_AUDIO_EFFECT
// Damon2.76, for audio EQ
extern WORD aAudioNormal[];
extern WORD aAudioBassBooster[];
extern WORD aAudioFullTreble[];
extern WORD aAudioJazz[];
extern WORD aAudioRock[];
extern WORD aAudioClassical[];
extern WORD aAudioPop[];
#endif //SUPPORT_AUDIO_EFFECT

#ifdef SUPPORT_RADIO_TUNER
extern WORD aRADIOSpace[];
extern WORD aRADIODot[];
extern WORD aRADIOColon[];
extern WORD aRADIOSolidus[];
extern WORD aRADIO875[]; 
extern WORD aRADIO93[];
extern WORD aRADIO98[];
extern WORD aRADIO103[];
extern WORD aRADIO108[];
#endif

//peteryu275, add for IC check version, 2007/3/22 11:31AM
extern WORD aAP_IC_VERSION_ERROR[];

extern WORD aNDJPEGEffectBlindVertical[];
extern WORD aNDJPEGEffectBlindHorizontal[];
extern WORD aNDJPEGEffectColorRationVertical[];
extern WORD aNDJPEGEffectColorRationHorizontal[];
extern WORD aNDJPEGEffectBlock1[];
extern WORD aNDJPEGEffectBlock2[];
extern WORD aNDJPEGEffectBlock3[];
extern WORD aNDJPEGEffectBlock4[];
extern WORD aNDJPEGEffectBlock5[];
extern WORD aNDJPEGEffectBlock6[];
extern WORD aNDJPEGEffectBlock7[];
extern WORD aNDJPEGEffectBlock8[];
extern WORD aNDJPEGEffectBlock9[];

#ifdef SUPPORT_USB_HOST_DEVICE
extern WORD aUSBDEVICEDlgTitle[];
extern WORD aUSBDEVICEDlgNotifyContent[];
extern WORD aUSBDEVICEDlgPowerDownContent[];
#endif //SUPPORT_USB_HOST_DEVICE