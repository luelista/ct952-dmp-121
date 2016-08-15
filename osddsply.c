#define OSDDSPLY_MAIN_FILE

#include "winav.h"
#include "ctkav.h"
#include "comdef.h"
#include "input.h"
#include "osddsply.h"
#include "osd.h"
#include "osdnd.h"
#include "setup.h"
#include "cc.h"
#include "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include "osdfont.h"
#include "navigate.h"
#include "linear.h"
#include "infofilter.h"
#include "mmanager.h"
#include "cell.h"
#include "dvdinfo.h"
#include "utl.h"
#include "hal.h"
#include "navigate.h"
#include "notransl.h"
#include "convchar.h"

#define OSDDSPLY_MAIN_FILE
//#include "string.lan"
#include "OSDString\\strextrn.h"

//CoCo1.0, add independent Navigator UI
#ifdef SUPPORT_INDEPENDENT_NAVIGATOR

#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
DISPLAYMODE OSDDSPLYDVDMode[OSDDSPLY_DVD_DISPLAY_NUM+1] = {
    {OSDDSPLY_MODE_NONE, OSDDSPLY_BACKGROUND_NONE},
    {OSDDSPLY_MODE_DVD_TITLE_ELAPSED, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_DVD_TITLE_REMAIN, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_DVD_CHAPTER_ELAPSED, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_DVD_CHAPTER_REMAIN, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_DVD_VIRTUAL_PANEL, OSDDSPLY_BACKGROUND_VP_DVD}};
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p

#else

#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
DISPLAYMODE OSDDSPLYDVDMode[OSDDSPLY_DVD_DISPLAY_NUM+1] = {
    {OSDDSPLY_MODE_NONE, OSDDSPLY_BACKGROUND_NONE},
    {OSDDSPLY_MODE_DVD_TITLE_ELAPSED, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_DVD_TITLE_REMAIN, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_DVD_CHAPTER_ELAPSED, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_DVD_CHAPTER_REMAIN, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_DVD_VIRTUAL_PANEL, OSDDSPLY_BACKGROUND_VP_DVD},
    {OSDDSPLY_MODE_DVD_NAVIGATOR, OSDDSPLY_BACKGROUND_NAVIGATOR}};
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p

#endif //SUPPORT_INDEPENDENT_NAVIGATOR

#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p
DISPLAYMODE OSDDSPLYVCDMode[OSDDSPLY_VCD_DISPLAY_NUM+1] = {
    {OSDDSPLY_MODE_NONE, OSDDSPLY_BACKGROUND_NONE},
    {OSDDSPLY_MODE_VCD_SINGLE_ELAPSED, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_VCD_SINGLE_REMAIN, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_VCD_TOTAL_ELAPSED, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_VCD_TOTAL_REMAIN, OSDDSPLY_BACKGROUND_LD_GENERAL}};
#endif //#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p

DISPLAYMODE OSDDSPLYJPEGMode[OSDDSPLY_JPEG_DISPLAY_NUM+1] = {
    {OSDDSPLY_MODE_NONE, OSDDSPLY_BACKGROUND_NONE},
    {OSDDSPLY_MODE_JPEG_LD, OSDDSPLY_BACKGROUND_LD_GENERAL},
    {OSDDSPLY_MODE_JPEG_VIRTUAL_PANEL, OSDDSPLY_BACKGROUND_VP_JPEG}};

DISPLAYMODE OSDDSPLYCDROMAVMode[OSDDSPLY_CDROM_AV_DISPLAY_NUM+1] = {
    {OSDDSPLY_MODE_NONE, OSDDSPLY_BACKGROUND_NONE},
    {OSDDSPLY_MODE_JPEG_LD, OSDDSPLY_BACKGROUND_LD_GENERAL}};

DISPLAYMODE OSDDSPLYCDROMDIVXMode[OSDDSPLY_CDROM_DIVX_DISPLAY_NUM+1] = {
    {OSDDSPLY_MODE_NONE, OSDDSPLY_BACKGROUND_NONE},
    {OSDDSPLY_MODE_CDROM_MOTION_ELAPSED, OSDDSPLY_BACKGROUND_LD_GENERAL},
#ifdef OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
    {OSDDSPLY_MODE_CDROM_MOTION_REMAIN, OSDDSPLY_BACKGROUND_LD_GENERAL},
#endif //OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
    {OSDDSPLY_MODE_DIVX_VIRTUAL_PANEL, OSDDSPLY_BACKGROUND_VP_DIVX}};
   
DISPLAYMODE OSDDSPLYCDROMAudioMode[OSDDSPLY_CDROM_AUDIO_DISPLAY_NUM+1] = {
    {OSDDSPLY_MODE_NONE, OSDDSPLY_BACKGROUND_NONE},
    {OSDDSPLY_MODE_CDROM_AUDIO, OSDDSPLY_BACKGROUND_LD_GENERAL}};    

//***************************************************************************
//Navigator data structure
//***************************************************************************
#ifdef SUPPORT_NAVIGATOR //CoCo1.07, support Navigator UI
BYTE OSDDSPLYNGPopUpMenu[] = {KEY_RIGHT, KEY_PLAY, KEY_ENTER};
NAVIGATOR OSDDSPLYNavigator[] = {
    {OSDDSPLY_NAVIGATOR_ICON_TITLE, OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_POPUP_MENU, OSDDSPLYNGPopUpMenu, OSDDSPLY_NAVIGATOR_MENU_TITLE_START_H, OSDDSPLY_NAVIGATOR_MENU_TITLE_START_V},
    {OSDDSPLY_NAVIGATOR_ICON_CHAPTER, OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_POPUP_MENU, OSDDSPLYNGPopUpMenu, OSDDSPLY_NAVIGATOR_MENU_CHAPTER_START_H, OSDDSPLY_NAVIGATOR_MENU_CHAPTER_START_V},
    {OSDDSPLY_NAVIGATOR_ICON_AUDIO, OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_POPUP_MENU, OSDDSPLYNGPopUpMenu, OSDDSPLY_NAVIGATOR_MENU_AUDIO_START_H, OSDDSPLY_NAVIGATOR_MENU_AUDIO_START_V},
    {OSDDSPLY_NAVIGATOR_ICON_SPST1, OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_POPUP_MENU, OSDDSPLYNGPopUpMenu, OSDDSPLY_NAVIGATOR_MENU_SPST1_START_H, OSDDSPLY_NAVIGATOR_MENU_SPST1_START_V},
    {OSDDSPLY_NAVIGATOR_ICON_SPST2, OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_POPUP_MENU, OSDDSPLYNGPopUpMenu, OSDDSPLY_NAVIGATOR_MENU_SPST2_START_H, OSDDSPLY_NAVIGATOR_MENU_SPST2_START_V},
    // wyc2.17-909S, update for language study modification.
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_MODE)
    {OSDDSPLY_NAVIGATOR_ICON_ANGLE, OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_POPUP_MENU, OSDDSPLYNGPopUpMenu, OSDDSPLY_NAVIGATOR_MENU_ANGLE_START_H, OSDDSPLY_NAVIGATOR_MENU_ANGLE_START_V},
#endif
    {OSDDSPLY_NAVIGATOR_ICON_REPEAT, OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_POPUP_MENU, OSDDSPLYNGPopUpMenu, OSDDSPLY_NAVIGATOR_MENU_REPEAT_START_H, OSDDSPLY_NAVIGATOR_MENU_REPEAT_START_V}
#if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)
    ,{OSDDSPLY_NAVIGATOR_ICON_REPEATAB, OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_BUTTON, NULL, OSDDSPLY_NAVIGATOR_MENU_REPEATAB_START_H, OSDDSPLY_NAVIGATOR_MENU_REPEATAB_START_V},
    {OSDDSPLY_NAVIGATOR_ICON_SLOW, OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_BUTTON, NULL, OSDDSPLY_NAVIGATOR_MENU_SLOW_START_H, OSDDSPLY_NAVIGATOR_MENU_SLOW_START_V}
#endif //SLOW_MODE_CONFIG
};

BYTE OSDDSPLYNGBGPalette[] = {
    OSDDSPLY_NG_PALETTE_ENTRY_ICON_TITLE_BACKGROUND,
    OSDDSPLY_NG_PALETTE_ENTRY_ICON_CHAPTER_BACKGROUND,
    OSDDSPLY_NG_PALETTE_ENTRY_ICON_AUDIO_BACKGROUND,
    OSDDSPLY_NG_PALETTE_ENTRY_ICON_SPST1_BACKGROUND,
    OSDDSPLY_NG_PALETTE_ENTRY_ICON_SPST2_BACKGROUND,
    // wyc2.17-909S, update for language study modification.
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_MODE)
    OSDDSPLY_NG_PALETTE_ENTRY_ICON_ANGLE_BACKGROUND,
#endif
    OSDDSPLY_NG_PALETTE_ENTRY_ICON_REPEAT_BACKGROUND
#if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)
   ,OSDDSPLY_NG_PALETTE_ENTRY_ICON_REPEATAB_BACKGROUND,
    OSDDSPLY_NG_PALETTE_ENTRY_ICON_SLOW_BACKGROUND
#endif //SLOW_MODE_CONFIG
};

#ifdef SUPPORT_SEPARATOR
BYTE _bOSDDSPLYNGSeparator[] = {5};
#endif //SUPPORT_SEPARATOR


#endif //SUPPORT_NAVIGATOR

//Initialize palette for line display
DWORD OSDDSPLYLDBackground[] = {OSDDSPLY_LD_PALETTE_COLOR_GENERAL_BACKGROUND_1, OSDDSPLY_LD_PALETTE_COLOR_GENERAL_BACKGROUND_2, OSDDSPLY_LD_PALETTE_COLOR_GENERAL_BACKGROUND_3, OSDDSPLY_LD_PALETTE_COLOR_GENERAL_BACKGROUND_4};
DWORD OSDDSPLYLDTitleItem[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_TITLE_ITEM_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_TITLE_ITEM_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_TITLE_ITEM_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_TITLE_ITEM_4};
DWORD OSDDSPLYLDChapterItem[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_CHAPTER_ITEM_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_CHAPTER_ITEM_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_CHAPTER_ITEM_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_CHAPTER_ITEM_4};
DWORD OSDDSPLYLDProgramStatus[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_PROGRAM_STATUS_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_PROGRAM_STATUS_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_PROGRAM_STATUS_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_PROGRAM_STATUS_4};
DWORD OSDDSPLYLDFileNameItem[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NAME_ITEM_TEXT_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NAME_ITEM_TEXT_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NAME_ITEM_TEXT_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NAME_ITEM_TEXT_4};
DWORD OSDDSPLYLDFileNumItem[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NUM_ITEM_TEXT_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NUM_ITEM_TEXT_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NUM_ITEM_TEXT_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NUM_ITEM_TEXT_4};
DWORD OSDDSPLYLDTitleInfo[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_TITLE_INFO_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_TITLE_INFO_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_TITLE_INFO_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_TITLE_INFO_4};
DWORD OSDDSPLYLDChapterInfo[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_CHAPTER_INFO_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_CHAPTER_INFO_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_CHAPTER_INFO_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_CHAPTER_INFO_4};
DWORD OSDDSPLYLDTimeInfo[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_TIME_INFO_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_TIME_INFO_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_TIME_INFO_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_TIME_INFO_4};
DWORD OSDDSPLYLDFileNameInfo[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NAME_INFO_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NAME_INFO_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NAME_INFO_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NAME_INFO_4};
DWORD OSDDSPLYLDFileNumInfo[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NUM_INFO_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NUM_INFO_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NUM_INFO_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_FILE_NUM_INFO_4};
DWORD OSDDSPLYLDBackgroundFrame[] = {OSDDSPLY_LD_PALETTE_COLOR_GENERAL_BACKGROUND_FRAME_1, OSDDSPLY_LD_PALETTE_COLOR_GENERAL_BACKGROUND_FRAME_2, OSDDSPLY_LD_PALETTE_COLOR_GENERAL_BACKGROUND_FRAME_3, OSDDSPLY_LD_PALETTE_COLOR_GENERAL_BACKGROUND_FRAME_4};
DWORD OSDDSPLYLDMenu[] = {OSDDSPLY_LD_PALETTE_COLOR_REGION_MENU_1, OSDDSPLY_LD_PALETTE_COLOR_REGION_MENU_2, OSDDSPLY_LD_PALETTE_COLOR_REGION_MENU_3, OSDDSPLY_LD_PALETTE_COLOR_REGION_MENU_4};

//Initialize palette for virtual panel
DWORD OSDDSPLYVPBackground[] = {OSDDSPLY_VP_PALETTE_COLOR_GENERAL_BACKGROUND_1, OSDDSPLY_VP_PALETTE_COLOR_GENERAL_BACKGROUND_2, OSDDSPLY_VP_PALETTE_COLOR_GENERAL_BACKGROUND_3, OSDDSPLY_VP_PALETTE_COLOR_GENERAL_BACKGROUND_4};
DWORD OSDDSPLYVPTitleItem[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_TITLE_ITEM_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_TITLE_ITEM_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_TITLE_ITEM_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_TITLE_ITEM_4};
DWORD OSDDSPLYVPChapterItem[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_CHAPTER_ITEM_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_CHAPTER_ITEM_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_CHAPTER_ITEM_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_CHAPTER_ITEM_4};
DWORD OSDDSPLYVPSubtitleItem[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_SUBTITLE_ITEM_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_SUBTITLE_ITEM_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_SUBTITLE_ITEM_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_SUBTITLE_ITEM_4};
DWORD OSDDSPLYVPASTItem[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_AST_ITEM_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_AST_ITEM_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_AST_ITEM_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_AST_ITEM_4};
DWORD OSDDSPLYVPTVItem[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_TV_ITEM_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_TV_ITEM_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_TV_ITEM_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_TV_ITEM_4};
DWORD OSDDSPLYVPAngleItem[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_ANGLE_ITEM_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_ANGLE_ITEM_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_ANGLE_ITEM_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_ANGLE_ITEM_4};
DWORD OSDDSPLYVPFileNameItem[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NAME_ITEM_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NAME_ITEM_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NAME_ITEM_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NAME_ITEM_4};
DWORD OSDDSPLYVPFileNumItem[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NUM_ITEM_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NUM_ITEM_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NUM_ITEM_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NUM_ITEM_4};
DWORD OSDDSPLYVPResolutionItem[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_RESOLUTION_ITEM_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_RESOLUTION_ITEM_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_RESOLUTION_ITEM_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_RESOLUTION_ITEM_4};
DWORD OSDDSPLYVPDateItem[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_DATE_ITEM_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_DATE_ITEM_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_DATE_ITEM_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_DATE_ITEM_4};
DWORD OSDDSPLYVPProgramStatus[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_PROGRAM_STATUS_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_PROGRAM_STATUS_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_PROGRAM_STATUS_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_PROGRAM_STATUS_4};
DWORD OSDDSPLYVPPlayStatus[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_PLAY_STATUS_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_PLAY_STATUS_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_PLAY_STATUS_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_PLAY_STATUS_4};
DWORD OSDDSPLYVPRepeatStatus[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_REPEAT_STATUS_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_REPEAT_STATUS_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_REPEAT_STATUS_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_REPEAT_STATUS_4};
DWORD OSDDSPLYVPTitleInfo[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_TITLE_INFO_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_TITLE_INFO_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_TITLE_INFO_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_TITLE_INFO_4};
DWORD OSDDSPLYVPChapterInfo[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_CHAPTER_INFO_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_CHAPTER_INFO_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_CHAPTER_INFO_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_CHAPTER_INFO_4};
DWORD OSDDSPLYVPTime[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_TIME_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_TIME_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_TIME_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_TIME_4};
DWORD OSDDSPLYVPSubtitleInfo[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_SUBTITLE_INFO_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_SUBTITLE_INFO_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_SUBTITLE_INFO_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_SUBTITLE_INFO_4};
DWORD OSDDSPLYVPASTInfo[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_AST_INFO_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_AST_INFO_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_AST_INFO_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_AST_INFO_4};
DWORD OSDDSPLYVPTVInfo[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_TV_INFO_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_TV_INFO_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_TV_INFO_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_TV_INFO_4};
DWORD OSDDSPLYVPAngleInfo[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_ANGLE_INFO_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_ANGLE_INFO_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_ANGLE_INFO_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_ANGLE_INFO_4};
DWORD OSDDSPLYVPMenu[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_MENU_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_MENU_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_MENU_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_MENU_4};
DWORD OSDDSPLYVPFileNameInfo[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NAME_INFO_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NAME_INFO_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NAME_INFO_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NAME_INFO_4};
DWORD OSDDSPLYVPFileNumInfo[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NUM_INFO_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NUM_INFO_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NUM_INFO_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_FILE_NUM_INFO_4};
DWORD OSDDSPLYVPResolutionInfo[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_RESOLUTION_INFO_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_RESOLUTION_INFO_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_RESOLUTION_INFO_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_RESOLUTION_INFO_4};
DWORD OSDDSPLYVPDateInfo[] = {OSDDSPLY_VP_PALETTE_COLOR_REGION_DATE_INFO_1, OSDDSPLY_VP_PALETTE_COLOR_REGION_DATE_INFO_2, OSDDSPLY_VP_PALETTE_COLOR_REGION_DATE_INFO_3, OSDDSPLY_VP_PALETTE_COLOR_REGION_DATE_INFO_4};

BYTE _bOSDDSPLYCursorIconIndex;
WORD _wOSDDSPLYCursorMenuIndex;
BYTE _bOSDDSPLYCursorPos;
WORD _wOSDDSPLYTotalSetting;
WORD _wOSDDSPLYMaxItem;
WORD _bOSDDSPLYMenuStartV;
BYTE _bOSDDSPLYMenuHasUpArrow;
BYTE _bOSDDSPLYMenuHasDownArrow;
WORD _wOSDDSPLYStartDrawItem;
WORD _wOSDDSPLYCurrentSettingIndex;
WORD _wOSDDSPLYMaxWidth;
BYTE _bOSDDSPLYReDrawMainFrame;
BYTE _bOSDDSPLYTipDisplayed;
DWORD _dwOSDDSPLYDisplayStartTime;
BYTE _bOSDDSPLYEnterTimeMode;
WORD _wOSDDSPLYWidth;
#ifdef SUPPORT_INDEPENDENT_NAVIGATOR //CoCo1.0, Navigator UI
BYTE _bNavigatorUI = FALSE;
#endif
//***************************************************************************

#ifdef SUPPORT_NAVIGATOR //CoCo1.07, support Navigator UI
// CoCo1.06, include bitmap and palette.
DWORD _aNAV_Audio[]=
{
#include "bmp/NG_Audio.txt"
};

DWORD _aNAV_Angle[]=
{
#include "bmp/NG_Angle.txt"
};

DWORD _aNAV_Chap[]=
{
#include "bmp/NG_Chap.txt"
};

DWORD _aNAV_Rep[]=
{
#include "bmp/NG_Rep.txt"
};

DWORD _aNAV_Sub1[]=
{
#include "bmp/NG_Sub1.txt"
};

DWORD _aNAV_Sub2[]= 
{
#include "bmp/NG_Sub2.txt"
};

DWORD _aNAV_Title[]=
{
#include "bmp/NG_Title.txt"
};

DWORD _aNAV_RepeatAB[]=
{
#include "bmp/NG_RepAB.txt"
};

DWORD _aNAV_Slow[]=
{
#include "bmp/NG_Slow.txt"
};

DWORD * _aNavigatorBMPArray[OSDDSPLY_MAX_NAVIGATOR_BMP_NUM] = {
    _aNAV_Audio,    // 0
    _aNAV_Angle,    // 1
    _aNAV_Chap,     // 2
    _aNAV_Rep,      // 3
    _aNAV_Sub1,     // 4
    _aNAV_Sub2,     // 5
    _aNAV_Title,    // 6
    _aNAV_RepeatAB, // 7
    _aNAV_Slow      // 8
};

DWORD Palette_Navigator[]=    // Palette 2 data, for button bitmap
{
#include "bmp/PalNG.txt"                  // bit 8/24 indicate Mix_Enable
};

GDI_PALETTE_INFO    __PaletteInfo_Navigator = {GDI_BITMAP_PALETTE_INDEX_START,  PAL_ADDR, Palette_Navigator};

#endif //SUPPORT_NAVIGATOR

//Temp variables
BYTE _bOSDDSPLYTemp; 
WORD _wOSDDSPLYTemp; 

//DISPLAYMODE *pOSDDSPTypeMode = OSDDSPLYDVDMode;
DISPLAYMODE *pOSDDSPTypeMode;
PARM_FRAME _OSDDSPLYFrame;
PARM_BITMAP_COPY _OSDDSPLYBitmapCopy;
PARM_DRAW_BITMAP _OSDDSPLYDrawBitmap;
BYTE _bOSDDSPLYMode = 0;
BYTE _bOSDDSPLYNum;
WORD _wOSDDSPLYHPos;
PARM_DRAW_STRING _OSDDSPLYStringAttr;
WORD _wOSDDSPLYInfo;
PARM_RECT _OSDDSPLYRect;
BYTE _bOSDDSPLYTitleMenuMode = FALSE;
BYTE _bOSDDSPLYLDTitleMenuMode = FALSE;

extern CHAR __cJPEGFMTExifDate[11];
extern BYTE __bNVPlayMode;
extern BYTE __bDomain;
//***************************************************************************
//  Function    :   OSDDSPLY_Update
//  Abstract    :   This function is the entry of the Display mode UI. Any message
//              :   needs to be showed at Display mode can use this function. 
//  Arguments   :   bMessage: the message.
//              :   wParam: the parameter.
//  Return      :   TRUE: the message is processed by Display mode.
//              :   FALSE: the message is not processed by Display mode.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE OSDDSPLY_Update(BYTE bMessage, WORD wParam)
{
    _wOSDDSPLYInfo = wParam; //Save the parameter for later usage.

    if (bMessage == MSG_DISPLAY)
    {
        if (wParam == OSD_EXIT_UI)
        {
            //Clear Display mode UI.
            _OSDDSPLY_ClearDisplayModeUI();

            _bOSDDSPLYMode = OSDDSPLY_MODE_NONE;
            
            //Tell OSD that the Display mode UI exits.
            OSD_ChangeUI(OSD_UI_DISPLAY, OSD_EXIT_UI);
        }
        else if (wParam == OSD_CHANGE_UI)
        {
            _OSDDSPLY_SwitchDisplayModeUI();
        }
    }
    else if (bMessage == MSG_INITIAL_PALETTE)
    {
        _OSDDSPLY_InitializeGDI();
    }
    else //Call each Display mode UI's entry to show the message.
    {
        if (_bOSDDSPLYEnterTimeMode)
            return _OSDDSPLY_LD_SpecialTimeModeUpdate(bMessage);
        
        switch (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID)
        {
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
        case OSDDSPLY_MODE_DVD_TITLE_ELAPSED:
        case OSDDSPLY_MODE_DVD_TITLE_REMAIN:
        case OSDDSPLY_MODE_DVD_CHAPTER_ELAPSED:
        case OSDDSPLY_MODE_DVD_CHAPTER_REMAIN:
#endif //NO_DVD_DISPLAY_UI
#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p
        case OSDDSPLY_MODE_VCD_SINGLE_ELAPSED:
        case OSDDSPLY_MODE_VCD_SINGLE_REMAIN:
        case OSDDSPLY_MODE_VCD_TOTAL_ELAPSED:
        case OSDDSPLY_MODE_VCD_TOTAL_REMAIN:
#endif //NO_VCD_DISPLAY_UI
        case OSDDSPLY_MODE_JPEG_LD:
        case OSDDSPLY_MODE_CDROM_MOTION_ELAPSED:
#ifdef OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
        case OSDDSPLY_MODE_CDROM_MOTION_REMAIN:
#endif //OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
        case OSDDSPLY_MODE_CDROM_AUDIO:
            //Update the message in one-line Display mode.
            return _OSDDSPLY_LD_Update(bMessage);
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
        case OSDDSPLY_MODE_DVD_VIRTUAL_PANEL:
#endif //NO_DVD_DISPLAY_UI
        case OSDDSPLY_MODE_JPEG_VIRTUAL_PANEL:
        case OSDDSPLY_MODE_DIVX_VIRTUAL_PANEL:
            //Update the message in the virtual panel.
            return _OSDDSPLY_VP_Update(bMessage);
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
        case OSDDSPLY_MODE_DVD_NAVIGATOR:
            //Navigator doesn't receive the message to show information. It just process key.
            //But when Navgator receives MSG_TITLE/MSG_CHAPTER, it needs to clear some menu for navigation.
            OSDDSPLY_NAVIGATOR_ClearMenuForNVChange(bMessage);
            return FALSE;
#endif //#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
        default:
            return FALSE;
        }        
    }
    
    return TRUE;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_InitializeGDI
//  Abstract    :   
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_InitializeGDI(void)
{
    _bOSDDSPLYTemp = __SetupInfo.bUIStyle-SETUP_CUSTOM_UI_STYLE_BEGIN;

    //Initialize the palette for Line Display.
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND, OSDDSPLYLDBackground[_bOSDDSPLYTemp], TRUE);
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_TITLE_ITEM, OSDDSPLYLDTitleItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_CHAPTER_ITEM, OSDDSPLYLDChapterItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_PROGRAM_STATUS, OSDDSPLYLDProgramStatus[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_FILE_NAME_ITEM_TEXT, OSDDSPLYLDFileNameItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_FILE_NUM_ITEM_TEXT, OSDDSPLYLDFileNumItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_TITLE_INFO, OSDDSPLYLDTitleInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_CHAPTER_INFO, OSDDSPLYLDChapterInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_TIME_INFO, OSDDSPLYLDTimeInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_FILE_NAME_INFO, OSDDSPLYLDFileNameInfo[_bOSDDSPLYTemp], FALSE);   
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_FILE_NUM_INFO, OSDDSPLYLDFileNumInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_REGION_MENU, OSDDSPLYLDMenu[_bOSDDSPLYTemp], FALSE);
#ifndef SUPPORT_SIMPLE_DISPLAY_MODE
    GDI_ChangePALEntry(OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND_FRAME, OSDDSPLYLDBackgroundFrame[_bOSDDSPLYTemp], TRUE);
#endif //SUPPORT_SIMPLE_DISPLAY_MODE

    //Initialize the palette for virtual panel.
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_GENERAL_BACKGROUND, OSDDSPLYVPBackground[_bOSDDSPLYTemp], TRUE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTTRY_REGION_TITLE_ITEM, OSDDSPLYVPTitleItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_CHAPTER_ITEM, OSDDSPLYVPChapterItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_SUBTITLE_ITEM, OSDDSPLYVPSubtitleItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_AST_ITEM, OSDDSPLYVPASTItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_TV_ITEM, OSDDSPLYVPTVItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_ANGLE_ITEM, OSDDSPLYVPAngleItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_FILE_NAME_ITEM, OSDDSPLYVPFileNameItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_FILE_NUM_ITEM, OSDDSPLYVPFileNumItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_RESOLUTION_ITEM, OSDDSPLYVPResolutionItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_DATE_ITEM, OSDDSPLYVPDateItem[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_PROGRAM_STATUS, OSDDSPLYVPProgramStatus[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_PLAY_STATUS, OSDDSPLYVPPlayStatus[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_REPEAT_STATUS, OSDDSPLYVPRepeatStatus[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_TITLE_INFO, OSDDSPLYVPTitleInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_CHAPTER_INFO, OSDDSPLYVPChapterInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_TIME, OSDDSPLYVPTime[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_SUBTITLE_INFO, OSDDSPLYVPSubtitleInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_AST_INFO, OSDDSPLYVPASTInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_TV_INFO, OSDDSPLYVPTVInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_ANGLE_INFO, OSDDSPLYVPAngleInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_MENU, OSDDSPLYVPMenu[_bOSDDSPLYTemp], FALSE);    
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_FILE_NAME_INFO, OSDDSPLYVPFileNameInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_FILE_NUM_INFO, OSDDSPLYVPFileNumInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_RESOLUTION_INFO, OSDDSPLYVPResolutionInfo[_bOSDDSPLYTemp], FALSE);
    GDI_ChangePALEntry(OSDDSPLY_VP_PALETTE_ENTRY_REGION_DATE_INFO, OSDDSPLYVPDateInfo[_bOSDDSPLYTemp], FALSE);

    GDI_WaitPaletteComplete();
}

//***************************************************************************
//  Function    :   OSDDSPLY_Trigger
//  Abstract    :   This function will be called by OSD_Trigger() to update UI.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
// LLY2.53, expand time value unit from WORD to DWORD
// Since, we will use [23:16]:hour, [15:8]: min, [7:0]: sec
//void OSDDSPLY_Trigger(BYTE bType, WORD wTime)
void OSDDSPLY_Trigger(BYTE bType, DWORD dwTime)
{    
    if (_bOSDDSPLYEnterTimeMode)
    {
        if (bType == OSD_TRIGGER_TIME)
        {
            _OSDDSPLY_LD_ShowSpecialTimeMode();
        }

        return;
    }

    switch (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID)
    {
    case OSDDSPLY_MODE_DVD_TITLE_ELAPSED:
    case OSDDSPLY_MODE_DVD_TITLE_REMAIN:
    case OSDDSPLY_MODE_DVD_CHAPTER_ELAPSED:
    case OSDDSPLY_MODE_DVD_CHAPTER_REMAIN:
    case OSDDSPLY_MODE_VCD_SINGLE_ELAPSED:
    case OSDDSPLY_MODE_VCD_SINGLE_REMAIN:
    case OSDDSPLY_MODE_VCD_TOTAL_ELAPSED:
    case OSDDSPLY_MODE_VCD_TOTAL_REMAIN:
    case OSDDSPLY_MODE_CDROM_AUDIO:
    case OSDDSPLY_MODE_CDROM_MOTION_ELAPSED:
#ifdef OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
    case OSDDSPLY_MODE_CDROM_MOTION_REMAIN:
#endif //OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
        if (bType == OSD_TRIGGER_CONTROL)
        {
#ifdef OSDDSPLY_SUPPORT_ENTER_TIME_MODE
            if ((OS_GetSysTimer()-_dwOSDDSPLYDisplayStartTime) > OSDDSPLY_LD_ENTER_SPECIAL_TIME_MODE)
            {
#ifndef NO_DISC_MODE //CoCo2.37
				if ((__wDiscType & BOOK_DVD ) && ((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM))) // Root Menu
				{
					_dwOSDDSPLYDisplayStartTime = OS_GetSysTimer();
					return;
				}
#endif //#ifndef NO_DISC_MODE //CoCo2.37

                _bOSDDSPLYEnterTimeMode = TRUE;
                
                //clear the line display region.
                _OSDDSPLY_ClearBackground(pOSDDSPTypeMode[_bOSDDSPLYMode].bBackgroundType);
                
                //Draw the background of special time mode
                _OSDDSPLY_DrawBackground(OSDDSPLY_BACKGROUND_LD_TIME);
                
                //Show time.
                _OSDDSPLY_LD_ShowSpecialTimeMode();
            }
#endif
        }                
        else if (bType == OSD_TRIGGER_TIME)
        {
            _OSDDSPLY_LD_ShowTime(OSDDSPLY_UPDATE_INFO);
        }
        break;
    case OSDDSPLY_MODE_DVD_VIRTUAL_PANEL:
#ifndef SUPPORT_SIMPLE_DISPLAY_MODE
        if (bType == OSD_TRIGGER_TIME)
            _OSDDSPLY_VP_ShowTime();
#endif //SUPPORT_SIMPLE_DISPLAY_MODE
        break;
    case OSDDSPLY_MODE_DIVX_VIRTUAL_PANEL:
#ifndef SUPPORT_SIMPLE_DISPLAY_MODE
        if (bType == OSD_TRIGGER_TIME)
            _OSDDSPLY_VP_MOTION_ShowTime();
#endif //SUPPORT_SIMPLE_DISPLAY_MODE
        break;
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
    case OSDDSPLY_MODE_DVD_NAVIGATOR:
        //Clear the "hint"?
        if (bType == OSD_TRIGGER_CONTROL)
        {
            if (_bOSDDSPLYTipDisplayed)
            {
                if ((OS_GetSysTimer()-_dwOSDDSPLYDisplayStartTime) > OSDDSPLY_NAVIGATOR_SHOW_TIP_TIME)
                    _OSDDSPLY_NAVIGATOR_ClearTip();
            }
        }
        break;
#endif //#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   OSDDSPLY_Recover
//  Abstract    :   This function will be called by OSD kernel to recover Display mode UI.
//  Arguments   :   bRecoverRegion: OSD_RECOVER_UPPER_REGION/OSD_RECOVER_BOTTOM_REGION/OSD_RECOVER_ALL.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE OSDDSPLY_Recover(BYTE bRecoverRegion)
{
    if (bRecoverRegion == OSD_RECOVER_ALL)
    {
        //Initialize GDI
        _OSDDSPLY_InitializeGDI();      

        if (_bOSDDSPLYEnterTimeMode)
        {
            //Draw the background of special time mode
            _OSDDSPLY_DrawBackground(OSDDSPLY_BACKGROUND_LD_TIME);

            _OSDDSPLY_LD_ShowSpecialTimeMode();
        }
        else
        {            
            //Draw current Display mode background.
            _OSDDSPLY_DrawBackground(pOSDDSPTypeMode[_bOSDDSPLYMode].bBackgroundType);

            _OSDDSPLY_ShowDisplayModeUI(TRUE);
        }
        
        return TRUE;
    }
    else
        return FALSE;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_LD_SpecialTimeModeUpdate
//  Abstract    :   
//  Arguments   :   bMessage: the message.
//  Return      :   TRUE: the message has been processed by one-line display.
//              :   FALSE: the message doesn't be processed by one-line display.
//  Side Effect :   
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_LD_SpecialTimeModeUpdate(BYTE bMessage)
{
    switch (bMessage)
    {
    case MSG_ABSTIME:
    case MSG_RELTIME:
    case MSG_TITLE_TIME:
    case MSG_CHAPTER_TIME:
    case MSG_REMAIN_ABSTIME:
    case MSG_REMAIN_RELTIME:
    case MSG_REMAIN_TITLE_TIME:
    case MSG_REMAIN_CHAPTER_TIME:
        _OSDDSPLY_LD_ShowSpecialTimeMode();
        return TRUE;
    default:
        return FALSE;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_LD_Update
//  Abstract    :   This is the entry to update the message to one-line display.
//  Arguments   :   bMessage: the message.
//  Return      :   TRUE: the message has been processed by one-line display.
//              :   FALSE: the message doesn't be processed by one-line display.
//  Side Effect :   
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_LD_Update(BYTE bMessage)
{
    switch (bMessage)
    {
    case MSG_TITLE:
    case MSG_CHAPTER:    
	case MSG_PROGRAM:
        _OSDDSPLY_LD_ShowInfo(bMessage);
        break;
    case MSG_TRACK:
        if (__wDiscType & CDROM_M1) //For JPEG
        {
            //Update "file name" info.
            _OSDDSPLY_LD_ShowInfo(MSG_FILE_NAME);
            
            //Update "file num" info.
            _OSDDSPLY_LD_ShowInfo(MSG_FILE_NUM);
        }
        else
        {
            _OSDDSPLY_LD_ShowInfo(bMessage);
        }
        break;
    case MSG_ABSTIME:
    case MSG_RELTIME:
    case MSG_TITLE_TIME:
    case MSG_CHAPTER_TIME:
    case MSG_REMAIN_ABSTIME:
    case MSG_REMAIN_RELTIME:
    case MSG_REMAIN_TITLE_TIME:
    case MSG_REMAIN_CHAPTER_TIME:  
        if (_OSDDSPLY_SameTimeType(bMessage))
        {
            _OSDDSPLY_LD_ShowTime(OSDDSPLY_UPDATE_INFO);
        }
        else
            return FALSE;
        break;
    case MSG_STOP: //Throw away MSG_STOP to let normal display to display "stop"?
        _wOSDDSPLYInfo = gcShowStatus.wCurrentTrack;
        _OSDDSPLY_LD_UpdateUnknownRegions();
    default:
        return FALSE;
    }

    return TRUE;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_VP_Update
//  Abstract    :   Call this function when you want to update the message for virtual panel.
//  Arguments   :   bMessage: the message.
//  Return		:   TRUE: The message has been updated.
//              :   FALSE: The message is not supported in the virtual panel.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_VP_Update(BYTE bMessage)
{
    switch (bMessage)
    {
    case MSG_STOP:
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
        if (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID == OSDDSPLY_MODE_DVD_VIRTUAL_PANEL)
            _OSDDSPLY_VP_UpdateUnknownRegions();
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
        return FALSE;
    case MSG_ABSTIME:
    case MSG_RELTIME:
    case MSG_TITLE_TIME:
    case MSG_CHAPTER_TIME:
    case MSG_REMAIN_ABSTIME:
    case MSG_REMAIN_RELTIME:
    case MSG_REMAIN_TITLE_TIME:
    case MSG_REMAIN_CHAPTER_TIME:
#ifndef SUPPORT_SIMPLE_DISPLAY_MODE
        if (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID == OSDDSPLY_MODE_DVD_VIRTUAL_PANEL)
            _OSDDSPLY_VP_ShowTime();
#endif
        break;
    case MSG_TRACK:
#ifndef SUPPORT_SIMPLE_DISPLAY_MODE
        _OSDDSPLY_VP_ShowInfo(MSG_FILE_NAME);
        _OSDDSPLY_VP_ShowInfo(MSG_FILE_NUM);
#endif
        break;
    default:
#ifdef SUPPORT_SIMPLE_DISPLAY_MODE
        switch (bMessage)
        {
        case MSG_SPST_CTL:
        case MSG_AST_CTL:
        case MSG_ANGLE_CTL:
        case MSG_JPEG_RESOLUTION:
        case MSG_JPEG_DATE:  
            return _OSDDSPLY_VP_ShowInfo(bMessage);
        default:
            return FALSE;
        }
#else
        return _OSDDSPLY_VP_ShowInfo(bMessage);
#endif //SUPPORT_SIMPLE_DISPLAY_MODE

    }

    return TRUE;
}


//***************************************************************************
//  Function    :   _OSDDSPLY_ClearDisplayModeUI
//  Abstract    :   This function will switch each display mode.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ClearDisplayModeUI(void)
{
    if (_bOSDDSPLYEnterTimeMode)
    {
        _OSDDSPLY_ClearBackground(OSDDSPLY_BACKGROUND_LD_TIME);
    }
    else
    {
        _OSDDSPLY_ClearBackground(pOSDDSPTypeMode[_bOSDDSPLYMode].bBackgroundType);
    }   
}

//***************************************************************************
//  Function    :   _OSDDSPLY_SwitchDisplayModeUI
//  Abstract    :   This function will switch each display mode.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_SwitchDisplayModeUI(void)
{
    if (_bOSDDSPLYMode == OSDDSPLY_MODE_NONE)
    {
        if (__wDiscType & BOOK_DVD) //Do initializztion
        {
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
            pOSDDSPTypeMode = OSDDSPLYDVDMode;
            _bOSDDSPLYNum = OSDDSPLY_DVD_DISPLAY_NUM;
#else
            return;
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
        }
        else if (__wDiscType & CDROM_M1) //For JPEG
        {
            if(__bAttrPlay == ATTR_JPG)
            {
                pOSDDSPTypeMode = OSDDSPLYJPEGMode;
                _bOSDDSPLYNum = OSDDSPLY_JPEG_DISPLAY_NUM;
            }
            else if (__bAttrPlay == ATTR_AVI)
            {
                pOSDDSPTypeMode = OSDDSPLYCDROMDIVXMode;
                _bOSDDSPLYNum = OSDDSPLY_CDROM_DIVX_DISPLAY_NUM;                
            }
            else
            {
                pOSDDSPTypeMode = OSDDSPLYCDROMAudioMode;
                _bOSDDSPLYNum = OSDDSPLY_CDROM_AUDIO_DISPLAY_NUM;                
            }
        }
        else
        {
#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p
            pOSDDSPTypeMode = OSDDSPLYVCDMode;
            _bOSDDSPLYNum = OSDDSPLY_VCD_DISPLAY_NUM;
#else
            return;
#endif //#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p
        }

        //Tell OSD that the Display mode UI enters.
        OSD_ChangeUI(OSD_UI_DISPLAY, OSD_ENTER_UI);

#ifdef OSDDSPLY_SUPPORT_SHOW_CURRENT_DISPLAY_BEFORE_SWITCH
        if (!_bOSDDSPLYEnterTimeMode)
#endif
            _bOSDDSPLYMode++;

        _bOSDDSPLYCursorIconIndex = 0; //Initialize the cursor's index for Navigator.

        //Show the Display mode UI.
        _OSDDSPLY_ShowDisplayModeUI(FALSE);    
    }
    else if (_bOSDDSPLYMode == _bOSDDSPLYNum)
    {
        if (!_bOSDDSPLYEnterTimeMode)
        {
            //Clear current UI.
            _OSDDSPLY_ClearDisplayModeUI();
            
            _bOSDDSPLYMode = OSDDSPLY_MODE_NONE;
            
            //Tell OSD that the Display mode UI exits.
            OSD_ChangeUI(OSD_UI_DISPLAY, OSD_EXIT_UI);
        }
        else
        {
            _OSDDSPLY_ShowDisplayModeUI(FALSE);
        }
    }
    else
    {
#ifdef OSDDSPLY_SUPPORT_SHOW_CURRENT_DISPLAY_BEFORE_SWITCH
        if (!_bOSDDSPLYEnterTimeMode)
#endif
            _bOSDDSPLYMode++;

        //Show the Display mode UI.
        _OSDDSPLY_ShowDisplayModeUI(FALSE);
    }        
}

             
//***************************************************************************
//  Function    :   _OSDDSPLY_ShowDisplayModeUI
//  Abstract    :   This function will show each display mode UI.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ShowDisplayModeUI(BYTE bRecoverUI)
{
    if (_bOSDDSPLYMode == OSDDSPLY_MODE_NONE)
    {
        return;
    }

    if (pOSDDSPTypeMode[_bOSDDSPLYMode-1].bBackgroundType == OSDDSPLY_BACKGROUND_NONE)
    {
        //Draw current Display mode background.
        _OSDDSPLY_DrawBackground(pOSDDSPTypeMode[_bOSDDSPLYMode].bBackgroundType);
    }
    else
    {        
        if (_bOSDDSPLYEnterTimeMode || (pOSDDSPTypeMode[_bOSDDSPLYMode].bBackgroundType != pOSDDSPTypeMode[_bOSDDSPLYMode-1].bBackgroundType))
        {
            //clear the previous Display mode UI background.
            _OSDDSPLY_ClearBackground(pOSDDSPTypeMode[_bOSDDSPLYMode-1].bBackgroundType);
            
            //Draw current Display mode background.
            _OSDDSPLY_DrawBackground(pOSDDSPTypeMode[_bOSDDSPLYMode].bBackgroundType);
        }
    }
    
    //Show each Display mode UI.
    switch (pOSDDSPTypeMode[_bOSDDSPLYMode].bBackgroundType)
    {
    case OSDDSPLY_BACKGROUND_LD_GENERAL:
        _bOSDDSPLYTemp = _bOSDDSPLYEnterTimeMode; //backup the "_bOSDDSPLYEnterTimeMode" mode

        //Reset the "Time mode" here to prevent the background is not re-drawed.
        _bOSDDSPLYEnterTimeMode = FALSE;

        //_OSDDSPLY_LD_Show() will UTL_OutputShowTime() to show time immediately.
        //If _bOSDDSPLYEnterTimeMode is not set to FALSE before calling _OSDDSPLY_LD_Show(), the position of the time will be wrong if UTL_OutputShowTime() call OSD_Trigger() at this time.
        if (bRecoverUI || _bOSDDSPLYTemp)
            _OSDDSPLY_LD_Show(TRUE);
        else
            _OSDDSPLY_LD_Show(FALSE);

        //Initialize the time which the display mode UI starts.
        _dwOSDDSPLYDisplayStartTime = OS_GetSysTimer();
        break;
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_BACKGROUND_VP_DVD:
        //Set show time type
        __bShowTimeType = MSG_TITLE_TIME;
        _OSDDSPLY_VP_ShowDVDMode();
        break;
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_BACKGROUND_VP_JPEG:
        _OSDDSPLY_VP_ShowJPEGMode();
        break;
    case OSDDSPLY_BACKGROUND_VP_DIVX:
        _OSDDSPLY_VP_ShowDIVXMode();
        break;
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
    case OSDDSPLY_BACKGROUND_NAVIGATOR:
        _OSDDSPLY_NAVIGATOR_ShowNavigator();
        break;
#endif //#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_SetBackground
//  Abstract    :   
//  Arguments   :   bBackgroundType: the type of the background.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_SetBackground(BYTE bBackgroundType)
{
    switch (bBackgroundType)
    {
    case OSDDSPLY_BACKGROUND_LD_GENERAL:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_START_H + OSDDSPLY_LD_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_START_V + OSDDSPLY_LD_HEIGHT;
        break;
    case OSDDSPLY_BACKGROUND_LD_TIME:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_TIME_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_TIME_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_TIME_START_H + OSDDSPLY_LD_TIME_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_TIME_START_V + OSDDSPLY_LD_TIME_HEIGHT;
        break;
    case OSDDSPLY_BACKGROUND_VP_DVD:
    case OSDDSPLY_BACKGROUND_VP_DIVX:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_START_H + OSDDSPLY_VP_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_START_V + OSDDSPLY_VP_DVD_HEIGHT;
        break;
    case OSDDSPLY_BACKGROUND_VP_JPEG:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_START_H + OSDDSPLY_VP_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_START_V + OSDDSPLY_VP_JPEG_HEIGHT;
        break;
    case OSDDSPLY_BACKGROUND_NAVIGATOR:
        //Can't clear all region because it may clear the normal upper/bottom region.
        //Should reserve the normal upper/bottom region.
        _OSDDSPLYRect.rect.wLeft = 0;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_NAVIGATOR_ICON_START_V-OSDDSPLY_NAVIGATOR_ICON_FRAME_THICKNESS;
        _OSDDSPLYRect.rect.wRight = 616;
        _OSDDSPLYRect.rect.wBottom = OSDND_BOTTOM_BACKGROUND_START_V;
        break;
    default:
        break;
    }

    _OSDDSPLYRect.rect.wRight -= 1;
    _OSDDSPLYRect.rect.wBottom -= 1;
}


//***************************************************************************
//  Function    :   _OSDDSPLY_DrawBackground
//  Abstract    :   This function will draw the background for each Display mode.
//  Arguments   :   bBackgroundType: the type of the background.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_DrawBackground(BYTE bBackgroundType)
{
    if (bBackgroundType == OSDDSPLY_BACKGROUND_NAVIGATOR)
    {
        //Navigator needs to draw the bitmap. It's not the same as other Display mode.
        //Need to draw the bitmap here.
        return;
    }

   // GDI_SetMixRatio(OSDDSPLY_LD_MIX_RATIO_BACKGROUND); //The mix ratio just effects the background

    _OSDDSPLY_SetBackground(bBackgroundType);
       
    _OSDDSPLYRect.bColor = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND;
   
    //GDI_FillRect_909(0, &_OSDDSPLYRect);
    GDI_FillRoundRect(0, OSDDSPLY_BACKGROUND_ARC_VALUE, &_OSDDSPLYRect);
    

#ifndef SUPPORT_SIMPLE_DISPLAY_MODE

    //Fill the frame of round rectangle
    _OSDDSPLYRect.bColor = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND_FRAME;

    _OSDDSPLYRect.rect.wLeft -= OSDDSPLY_BACKGROUND_OUTER_INNER_ARC_PIXEL;
    _OSDDSPLYRect.rect.wTop -= OSDDSPLY_BACKGROUND_OUTER_INNER_ARC_PIXEL;
    _OSDDSPLYRect.rect.wRight += OSDDSPLY_BACKGROUND_OUTER_INNER_ARC_PIXEL;
    _OSDDSPLYRect.rect.wBottom += OSDDSPLY_BACKGROUND_OUTER_INNER_ARC_PIXEL;

    GDI_DrawRoundRect(0, OSDDSPLY_BACKGROUND_OUTER_ARC_VALUE, OSDDSPLY_BACKGROUND_ARC_VALUE, &_OSDDSPLYRect);

#endif //SUPPORT_SIMPLE_DISPLAY_MODE
}

//***************************************************************************
//  Function    :   _OSDDSPLY_ClearBackground
//  Abstract    :   This function will clear the background for each Display mode.
//  Arguments   :   bBackgroundType: the type of the background.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ClearBackground(BYTE bBackgroundType)
{
    _OSDDSPLY_SetBackground(bBackgroundType);
       
    _OSDDSPLYRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;

#ifndef SUPPORT_SIMPLE_DISPLAY_MODE
    if (bBackgroundType != OSDDSPLY_BACKGROUND_NAVIGATOR)
    {
        //clear the frame
        _OSDDSPLYRect.rect.wLeft = 0;
        _OSDDSPLYRect.rect.wTop -= OSDDSPLY_BACKGROUND_OUTER_INNER_ARC_PIXEL;
        _OSDDSPLYRect.rect.wRight = 616;
        _OSDDSPLYRect.rect.wBottom += OSDDSPLY_BACKGROUND_OUTER_INNER_ARC_PIXEL;
    }
#endif //SUPPORT_SIMPLE_DISPLAY_MODE
    
    GDI_FillRect(0, &_OSDDSPLYRect);
}   


//***************************************************************************
//  Function    :   _OSDDSPLY_LD_Show
//  Abstract    :   This function will show one-line Display mode.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_Show(BYTE bRecoverUI)
{   
    if (!bRecoverUI && (pOSDDSPTypeMode[_bOSDDSPLYMode].bBackgroundType == pOSDDSPTypeMode[_bOSDDSPLYMode-1].bBackgroundType))
    {
        if (__wDiscType & CDROM_M1)
        {
            _OSDDSPLY_LD_ClearRegion(OSDDSPLY_REGION_LD_CDROM_TIME, 0);
        }
        else
        {
            //just clear the the time region and re-draw it.
            _OSDDSPLY_LD_ClearRegion(OSDDSPLY_LD_REGION_TIME, 0);
        }

        //Set the current time type.
        _OSDDSPLY_LD_SetTimeType();
        
        //Calculate the time through the trigger.
        UTL_OutputShowTime();

        _OSDDSPLY_LD_ShowTime(OSDDSPLY_UPDATE_ITEM | OSDDSPLY_UPDATE_INFO);
    }
    else //Enter general one-line display at first time.
    {   
        if (__wDiscType & BOOK_DVD)
        {
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
#ifndef NO_DISC_MODE //CoCo2.37    
			if ((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM)) // Root Menu
			{	
				_bOSDDSPLYLDTitleMenuMode = TRUE; 

				//Show "Title Menu"
				OSDUTL_PrepareOutputString();
				_OSDDSPLY_ShowTitleMenu();
				_OSDDSPLY_LD_DrawRegion(OSDDSPLY_REGION_LD_MENU, OSDDSPLY_UPDATE_INFO, 0);				
			}
			else
			{
				//Draw all item and info.
				_OSDDSPLY_LD_ShowItem(MSG_TITLE);
				_OSDDSPLY_LD_ShowItem(MSG_CHAPTER);

				if (__bCountStop != 0) //pre-stop or stop mode
				{
					_OSDDSPLY_LD_UpdateUnknownRegions();
					return;
				}
				else
				{
					_OSDDSPLY_LD_ShowInfo(MSG_TITLE);
					_OSDDSPLY_LD_ShowInfo(MSG_CHAPTER);
				}
			}

			//Set the current time type.
			_OSDDSPLY_LD_SetTimeType();

			//Calculate the time through the trigger.
			UTL_OutputShowTime();

			_OSDDSPLY_LD_ShowTime(OSDDSPLY_UPDATE_ITEM | OSDDSPLY_UPDATE_INFO);	
#endif //#ifndef NO_DISC_MODE //CoCo2.37                  
#else
            return;

#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
        }
        else if (__wDiscType & CDROM_M1)
        {                
            //Show file name item
            _OSDDSPLY_LD_ShowItem(MSG_FILE_NAME);
            //Show file name info.
            _OSDDSPLY_LD_ShowInfo(MSG_FILE_NAME);
            
            //Show file num item.
            _OSDDSPLY_LD_ShowItem(MSG_FILE_NUM);
            
            _wOSDDSPLYInfo = __wPlayItem; //Prepare the info. for _OSDDSPLY_ShowTrackInfo() at the first time when JPEG display is on.

            //Show file num info.
            _OSDDSPLY_LD_ShowInfo(MSG_FILE_NUM);

            if (__bAttrPlay != ATTR_JPG)
            {
                //Set the current time type.               
                _OSDDSPLY_LD_SetTimeType();

                //Calculate the time through the trigger.
                UTL_OutputShowTime();

                _OSDDSPLY_LD_ShowTime(OSDDSPLY_UPDATE_ITEM |OSDDSPLY_UPDATE_INFO);
            }
        }
        else
        {
#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p
            _OSDDSPLY_LD_ShowItem(MSG_TRACK);
            
            if (__bCountStop != 0) //pre-stop or stop mode
            {
                _OSDDSPLY_LD_UpdateUnknownRegions();
                return;
            }
            else
            {
                _wOSDDSPLYInfo = gcShowStatus.wCurrentTrack;
                _OSDDSPLY_LD_ShowInfo(MSG_TRACK);

                //Set the current time type.
                _OSDDSPLY_LD_SetTimeType();

                //Calculate the time through the trigger.
                UTL_OutputShowTime();

                _OSDDSPLY_LD_ShowTime(OSDDSPLY_UPDATE_ITEM | OSDDSPLY_UPDATE_INFO);
            }
#else
            return;
#endif //#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p
        }
        
        //Show "Program" Status 
        _OSDDSPLY_LD_ShowInfo(MSG_PROGRAM);
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_LD_ShowSpecialTimeMode
//  Abstract    :   
//  Arguments   :   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_ShowSpecialTimeMode(void)
{
    //Show time.    
    if (gcShowStatus.bStill || (__bCountStop != 0))
    {
        __pOSDUTLOutputString = aDSPLYUnknownTime;
        
        _OSDDSPLY_LD_DrawRegion(OSDDSPLY_REGION_TIME_LD_TIME, OSDDSPLY_UPDATE_INFO, 0);
        
        return;
    }
    
    OSDUTL_PrepareOutputString();
    
    OSDUTL_ShowHourMinSec();
    
    _OSDDSPLY_LD_DrawRegion(OSDDSPLY_REGION_TIME_LD_TIME, OSDDSPLY_UPDATE_INFO, 0);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_LD_SetTimeType
//  Abstract    :   This function set __bShowTimeType according to current Display mode.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_SetTimeType(void)
{
    _wOSDDSPLYHPos = 0;

    //show type of the time.
    switch (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID)
    {
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_MODE_DVD_TITLE_ELAPSED:
        __bShowTimeType = MSG_TITLE_TIME;
        break;
    case OSDDSPLY_MODE_DVD_TITLE_REMAIN:
         __bShowTimeType = MSG_REMAIN_TITLE_TIME;
        break;
    case OSDDSPLY_MODE_DVD_CHAPTER_ELAPSED:
        __bShowTimeType = MSG_CHAPTER_TIME;
        break;
    case OSDDSPLY_MODE_DVD_CHAPTER_REMAIN:
        __bShowTimeType = MSG_REMAIN_CHAPTER_TIME;
        break;
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_MODE_VCD_SINGLE_ELAPSED:
        __bShowTimeType = MSG_RELTIME;
        break;
    case OSDDSPLY_MODE_VCD_SINGLE_REMAIN:
        __bShowTimeType = MSG_REMAIN_RELTIME;
        break;
    case OSDDSPLY_MODE_VCD_TOTAL_ELAPSED:
        __bShowTimeType = MSG_ABSTIME;
        break;
    case OSDDSPLY_MODE_VCD_TOTAL_REMAIN:
        __bShowTimeType = MSG_REMAIN_ABSTIME;
        break;
#endif //#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_MODE_CDROM_AUDIO:
        __bShowTimeType = MSG_RELTIME;
        break;
    case OSDDSPLY_MODE_CDROM_MOTION_ELAPSED:
        __bShowTimeType = MSG_RELTIME;
        break;
#ifdef OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
    case OSDDSPLY_MODE_CDROM_MOTION_REMAIN:
        __bShowTimeType = MSG_REMAIN_RELTIME;
        break;
#endif //OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
    default:
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_LD_ShowTime
//  Abstract    :   This function will show time for one-line display
//  Arguments   :   bUpdate: OSDDSPLY_UPDATE_ITEM==>Update 
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_ShowTime(BYTE bUpdate)
{
    _wOSDDSPLYHPos = 0;

    //show type of the time.
    switch (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID)
    {
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_MODE_DVD_TITLE_ELAPSED:
        //output "Title Elapsed" string.
        __pOSDUTLOutputString = aDSPLYTitleElapsed;
        break;
    case OSDDSPLY_MODE_DVD_TITLE_REMAIN:
        //Output "Title Remain" string.
        __pOSDUTLOutputString = aDSPLYTitleRemain;
        break;
    case OSDDSPLY_MODE_DVD_CHAPTER_ELAPSED:
        //Output "Chapter Elapsed" string.
        __pOSDUTLOutputString = aDSPLYChapterElapsed;
        break;
    case OSDDSPLY_MODE_DVD_CHAPTER_REMAIN:
        //Output "Chapter Remain" string.
        __pOSDUTLOutputString = aDSPLYChapterRemain;
        break;
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_MODE_VCD_SINGLE_ELAPSED:
        //Output "Single Elapsed" string.
        __pOSDUTLOutputString = aDSPLYSingleElapsed;
        break;
    case OSDDSPLY_MODE_VCD_SINGLE_REMAIN:
        //Output "Single Remain" string.
        __pOSDUTLOutputString = aDSPLYSingleRemain;
        break;
    case OSDDSPLY_MODE_VCD_TOTAL_ELAPSED:
        //Output "Total Elapsed" string.
        __pOSDUTLOutputString = aDSPLYTotalElapsed;
        break;
    case OSDDSPLY_MODE_VCD_TOTAL_REMAIN:
        //Output "Total Remain" string.
        __pOSDUTLOutputString = aDSPLYTotalRemain;
        break;
#endif //#ifndef NO_VCD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_MODE_CDROM_AUDIO:
        break;
#ifdef OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
    case OSDDSPLY_MODE_CDROM_MOTION_REMAIN:
        __pOSDUTLOutputString = aNDRemain;
        break;
#endif //OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
    default:
        break;
    }

    if (__wDiscType & CDROM_M1)
    {
        if (__bAttrPlay != ATTR_JPG)
        {
#ifdef OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX
            if (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID == OSDDSPLY_MODE_CDROM_MOTION_REMAIN)
            {
                _OSDDSPLY_LD_DrawRegion(OSDDSPLY_REGION_LD_CDROM_TIME, OSDDSPLY_UPDATE_ITEM, 0);
                _wOSDDSPLYHPos = GDI_GetStringWidth_909(__pOSDUTLOutputString);
            }
#endif //OSDDSPLY_SUPPORT_SHOW_REMAIN_TIME_FOR_DIVX

            OSDUTL_PrepareOutputString();
            OSDUTL_ShowHourMinSec();
            _OSDDSPLY_LD_DrawRegion(OSDDSPLY_REGION_LD_CDROM_TIME, OSDDSPLY_UPDATE_INFO, _wOSDDSPLYHPos);               
        }
    }
    else
    {
        if (bUpdate & OSDDSPLY_UPDATE_ITEM)
        {
            _OSDDSPLY_LD_DrawRegion(OSDDSPLY_REGION_LD_TIME, OSDDSPLY_UPDATE_ITEM, 0);
        }

#ifndef NO_DISC_MODE //CoCo2.37   

		//CoCo1.07, support title menu in one-line Display mode.
		if ((__wDiscType & BOOK_DVD) && ((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM)))
			return;
       
#endif //#ifndef NO_DISC_MODE //CoCo2.37    

        _wOSDDSPLYHPos = GDI_GetStringWidth_909(__pOSDUTLOutputString);
        _wOSDDSPLYHPos += OSDDSPLY_LD_DISTANCE_TIME_TITLE_TIME;
        
        if (bUpdate & OSDDSPLY_UPDATE_INFO)
        {    
            //Show time.    
            if (gcShowStatus.bStill || (__bCountStop != 0))
            {
                __pOSDUTLOutputString = aDSPLYUnknownTime;
                
                _OSDDSPLY_LD_DrawRegion(OSDDSPLY_REGION_LD_TIME, OSDDSPLY_UPDATE_INFO, (BYTE)_wOSDDSPLYHPos);      
                
                return;
            }
                        
            OSDUTL_PrepareOutputString();
            
            OSDUTL_ShowHourMinSec();
            
            _OSDDSPLY_LD_DrawRegion(OSDDSPLY_REGION_LD_TIME, OSDDSPLY_UPDATE_INFO, (BYTE)_wOSDDSPLYHPos);       
        }
    }   
}


//***************************************************************************
//  Function    :   _OSDDSPLY_LD_DrawRegion
//  Abstract    :   This function will draw the region.
//  Arguments   :   bRegion: The region you want to draw.
//              :   bType: OSDLD_UPDATE_ITEM: You want to draw the item.
//              :          OSDDSPLY_UPDATE_INFO: You want to draw the info.
//              :   bPos: The relative position of the region.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_DrawRegion(BYTE bRegion, BYTE bType, BYTE bPos)
{
    //clear all region except time region.
    if ((bRegion != OSDDSPLY_REGION_LD_TIME) && (bRegion != OSDDSPLY_REGION_TIME_LD_TIME) && (bRegion != OSDDSPLY_REGION_LD_CDROM_TIME))
        _OSDDSPLY_LD_ClearRegion(bRegion, bPos);

    _OSDDSPLYStringAttr.bBackground = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND; //PAL_ENTRY_COLOR_TRANSPARENT; //Set the color of background is the same as color key.
    _OSDDSPLYStringAttr.bColorKey = 0;
    _OSDDSPLYStringAttr.bShadow1 = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND; //Use OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND to prevent the "Open Folder" icon is wrong.
    _OSDDSPLYStringAttr.bShadow2 = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND; //Use OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND to prevent the "Open Folder" icon is wrong.
    
    switch (bRegion)
    {
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_REGION_LD_TITLE:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_TITLE_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_TITLE_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_LD_REGION_TITLE_START_H;
        break; 
    case OSDDSPLY_REGION_LD_CHAPTER:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_CHAPTER_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_CHAPTER_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_LD_REGION_CHAPTER_START_H;
        break; 
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_REGION_LD_TIME:
        _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_TIME_INFO_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_LD_REGION_TIME_START_H;
        break;
    case OSDDSPLY_REGION_LD_PROGRAM_STATUS:        
        _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_PROGRAM_STATUS_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_LD_REGION_PROGRAM_STATUS_START_H;
        break;
    case OSDDSPLY_REGION_TIME_LD_TIME:
        _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_TIME_INFO_COLOR;
        //_OSDDSPLYStringAttr.wX = OSDDSPLY_LD_TIME_SHOW_TIME_START_H;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_LD_TIME_START_H + ((OSDDSPLY_LD_TIME_WIDTH - GDI_GetStringWidth_909(__pOSDUTLOutputString)) >> 1);
        break;
    case OSDDSPLY_REGION_LD_FILE_NAME:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_FILE_NAME_INFO_COLOR;
        }
        else
        {
            //Draw the "folder" icon.
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_FILE_NAME_ITEM_TEXT_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_LD_REGION_FILE_NAME_START_H;
        break;
    case OSDDSPLY_REGION_LD_FILE_NUM:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {           
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_FILE_NUM_INFO_COLOR;
        }
        else
        {
            //Draw the "file num" icon.
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_FILE_NUM_ITEM_TEXT_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_LD_REGION_FILE_NUM_START_H;
        break;
    case OSDDSPLY_REGION_LD_CDROM_TIME:
         _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_CDROM_TIME_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_LD_REGION_CDROM_TIME_START_H;
        break;
	case OSDDSPLY_REGION_LD_MENU:
        _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_MENU_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_LD_REGION_MENU_START_H;
		break;
    default:
        break;
    }

    _OSDDSPLYStringAttr.wX += bPos;
    _OSDDSPLYStringAttr.wY = OSDDSPLY_LD_START_V + ((OSDDSPLY_LD_HEIGHT - GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);
    
    //GDI_DrawString_909(0, &_OSDDSPLYStringAttr, __pOSDUTLOutputString);
    _OSDDSPLY_DrawString();
}

//***************************************************************************
//  Function    :   _OSDDSPLY_LD_ShowItem
//  Abstract    :   This function will show the item according to bMessage.
//  Arguments   :   bMessage: The message.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_ShowItem(BYTE bMessage)
{    
    _OSDDSPLY_OutputString(bMessage);
        
    _OSDDSPLY_LD_DrawRegion(_OSDDSPLY_LD_GetRegion(bMessage), OSDDSPLY_UPDATE_ITEM, 0);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_LD_GetRegion
//  Abstract    :   This function will get the message's region.
//  Arguments   :   bMessage: The message.
//  Return		:   The region that the message want to output.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_LD_GetRegion(BYTE bMessage)
{
    switch (bMessage)
    {
    case MSG_TITLE:
    case MSG_TRACK:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_LD_TITLE;
        break;
    case MSG_CHAPTER:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_LD_CHAPTER;
        break;
    case MSG_PROGRAM:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_LD_PROGRAM_STATUS;
        break;
    case MSG_FILE_NAME:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_LD_FILE_NAME;
        break;
    case MSG_FILE_NUM:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_LD_FILE_NUM;
        break;
    default:
        break;
    }

    return _bOSDDSPLYTemp;
}


//***************************************************************************
//  Function    :   _OSDDSPLY_OutputString
//  Abstract    :   This function will output the string of the item according to bMessage.
//  Arguments   :   bMessage: The message.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_OutputString(BYTE bMessage)
{
    switch (bMessage)
    {
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
    case MSG_TITLE: //update title region
        __pOSDUTLOutputString = aDSPLYTitleIcon;
        break;
    case MSG_CHAPTER:
        __pOSDUTLOutputString = aDSPLYChapterIcon;
        break;
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
    case MSG_TRACK:
        __pOSDUTLOutputString = aDSPLYTrackIcon;
        break;
    case MSG_FILE_NAME:
        __pOSDUTLOutputString = aDSPLYFolderIcon;
        break;
    case MSG_FILE_NUM:
        __pOSDUTLOutputString = aDSPLYTrackIcon;
        break;
    case MSG_JPEG_RESOLUTION:
        __pOSDUTLOutputString = aDSPLYResolution;
        break;
    case MSG_JPEG_DATE:
        __pOSDUTLOutputString = aDSPLYDate;
        break;
    case MSG_SPST_CTL:
        __pOSDUTLOutputString = aDSPLYSPSTIcon;
        break;
    case MSG_AST_CTL:
        __pOSDUTLOutputString = aDSPLYASTIcon;
        break;
    case MSG_TVMODE:
    case MSG_PSCAN:
        __pOSDUTLOutputString = aDSPLYTVIcon;
        break;
    case MSG_ANGLE_CTL:
        __pOSDUTLOutputString = aDSPLYAngleIcon;
        break;        
    default:
        __pOSDUTLOutputString = aDSPLYNull; //null string.
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDLD_GetInfoPos
//  Abstract    :   Call this function to get the position of the info.
//  Arguments   :   bMessage: the message.
//  Return		:   return the position of the info.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_GetInfoPos(BYTE bMessage)
{
    _OSDDSPLY_OutputString(bMessage);

    return GDI_GetStringWidth_909(__pOSDUTLOutputString);
}

//***************************************************************************
//  Function    :   _OSDLD_ShowInfo
//  Abstract    :   This function will show the info. according to the message.
//  Arguments   :   bMessage: The message.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_ShowInfo(BYTE bMessage)
{  
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p    
#ifndef NO_DISC_MODE //CoCo2.37    
	if ((bMessage == MSG_TITLE) && ((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM))) // Root Menu
	{
		//Clear "Menu" region. That is "Title+Chapter" region.
		_OSDDSPLY_LD_ClearRegion(OSDDSPLY_REGION_LD_MENU, 0);
	}
	else
#endif //#ifndef NO_DISC_MODE //CoCo2.37    
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
	{
		_wOSDDSPLYHPos = _OSDDSPLY_GetInfoPos(bMessage);
		_OSDDSPLY_LD_ClearRegion(_OSDDSPLY_LD_GetRegion(bMessage), (BYTE)_wOSDDSPLYHPos);
	
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p        
#ifndef NO_DISC_MODE //CoCo2.37   
		if (_bOSDDSPLYLDTitleMenuMode && (bMessage == MSG_TITLE) && (!((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM))))
		{
			_bOSDDSPLYLDTitleMenuMode = FALSE;

			//Recover "title" and "chapter" item.
			//Clear "menu" region
			_OSDDSPLY_LD_ClearRegion(OSDDSPLY_REGION_LD_MENU, 0);

			//Show "Title" item
			_OSDDSPLY_LD_ShowItem(MSG_TITLE);

			//Show "chapter" item
			_OSDDSPLY_LD_ShowItem(MSG_CHAPTER); 
		}
#endif //#ifndef NO_DISC_MODE //CoCo2.37  
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
	}

	OSDUTL_PrepareOutputString();

    switch (bMessage)
    {
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
#ifndef NO_DISC_MODE //CoCo2.37  
    case MSG_TITLE:
		if((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM)) // Root Menu
		{
			_bOSDDSPLYLDTitleMenuMode = TRUE; 

			//Show "Title Menu"
			_OSDDSPLY_ShowTitleMenu();
		}
		else
			_OSDDSPLY_ShowTitleInfo();
        break;
    case MSG_CHAPTER:
		if ((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM))
            return; //Don't show the chapter info. in title menu/DVD menu.
		else
			_OSDDSPLY_ShowChapterInfo();
        break;
#endif //#ifndef NO_DISC_MODE //CoCo2.37
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p        
    case MSG_TRACK:
    case MSG_FILE_NUM:
        _OSDDSPLY_ShowTrackInfo();
        break;
	case MSG_PROGRAM:
        if (__btPlayingProgram)
		{
			__pOSDUTLOutputString = aDSPLYProgramSymbol;
		}
        else
        {
            _OSDDSPLY_LD_ClearRegion(OSDDSPLY_REGION_LD_PROGRAM_STATUS, 0);
            return;
        }
        break;
    case MSG_FILE_NAME:
        //_OSDDSPLY_ShowFileNameInfo(OSDDSPLY_LD_REGION_FILE_DIR_NAME_MAX_WIDTH-_wOSDDSPLYHPos);
		_OSDDSPLY_LD_ShowFileNameInfo(OSDDSPLY_LD_REGION_FILE_DIR_NAME_MAX_WIDTH-_wOSDDSPLYHPos, _wOSDDSPLYHPos);
		return;        
    default:
        break;
    }

	if (_bOSDDSPLYLDTitleMenuMode && (bMessage == MSG_TITLE))
        _OSDDSPLY_LD_DrawRegion(OSDDSPLY_REGION_LD_MENU, OSDDSPLY_UPDATE_INFO, 0);
	else
		_OSDDSPLY_LD_DrawRegion(_OSDDSPLY_LD_GetRegion(bMessage), OSDDSPLY_UPDATE_INFO, (BYTE)_wOSDDSPLYHPos);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_ShowTitleInfo
//  Abstract    :   This function will show the info. of the title.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ShowTitleInfo(void)
{
#ifndef NO_DISC_MODE //CoCo2.37
    OSDUTL_OutputNumFromIndex(__bTitle);
    OSDUTL_CopyStringFromIndex(aDSPLYSlash);
    OSDUTL_OutputNumFromIndex(__bTitle_Ns);
#endif //#ifndef NO_DISC_MODE //CoCo2.37
}   

//***************************************************************************
//  Function    :   _OSDDSPLY_ShowTitleMenu
//  Abstract    :   This function will show the info. of the title.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
#ifndef NO_DISC_MODE //CoCo2.37   
void _OSDDSPLY_ShowTitleMenu(void)
{
    if (__bDomain == VTSM_DOM) // Root Menu
    {
        OSDUTL_CopyStringFromIndex(aDSPLYRootMenu);
    }
    else if (__bDomain == VMGM_DOM)  // Title Menu
    {
        OSDUTL_CopyStringFromIndex(aDSPLYTitleMenu);
    }
}
#endif //#ifndef NO_DISC_MODE //CoCo2.37  
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p

//***************************************************************************
//  Function    :   _OSDDSPLY_ShowChapterInfo
//  Abstract    :   This function will show the info. of the chapter.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ShowChapterInfo(void)
{    
#ifndef NO_DISC_MODE //CoCo2.37
#ifdef SUPPORT_INDEPENDENT_NAVIGATOR
	if (_bNavigatorUI && (__bFWKey == KEY_SEARCH_PLAY)) //CoCo1.06, fix the problem about chapter info. is wrong after changing the chapter number.
	{
		OSDUTL_OutputNumFromIndex(__wSearchChapter);
	}
	else
#endif	
	{		
		OSDUTL_OutputNumFromIndex(__wChapter);
	}
	OSDUTL_CopyStringFromIndex(aDSPLYSlash);
	OSDUTL_OutputNumFromIndex(__wChapter_Ns_Title);
#endif //#ifndef NO_DISC_MODE //CoCo2.37
}

//***************************************************************************
//  Function    :   _OSDDSPLY_ShowTrackInfo
//  Abstract    :   This function will show the info. of the track.
//              :   The result will put to __wOSDUTLStringBuf[].
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ShowTrackInfo(void)
{
    OSDUTL_PrepareOutputString();

    if (__wDiscType & CDROM_M1)
    {
        _wOSDDSPLYInfo = __wPlayItem;            
    }

    if (_wOSDDSPLYInfo != 0)
    {
        OSDUTL_OutputNumFromIndex(_wOSDDSPLYInfo);
    }
    else
        OSDUTL_CopyStringFromIndex(aDSPLYUnknownTrack);

    __wOSDUTLStringBuf[__bOSDUTLIndex++] = aDSPLYSlash[1];
           
    if (__wDiscType & CDROM_M1)
    {
        // wyc0.85a, use new pointer to support sort/filter function.
        OSDUTL_OutputNumFromIndex(__pMMFilterFilesOfDir[__MMPlayingItem.wDirID]);
    }
    else //VCD
    {
        if (_wOSDDSPLYInfo != 0)
            OSDUTL_OutputNumFromIndex(gcShowStatus.wTotalTracks);
        else
            __pOSDUTLOutputString = aDSPLYUnknownTrack;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_CodeToLanguage
//  Abstract    :   This function will output the country according to wCode.
//  Arguments   :   wCode: The code that you want to transform.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_CodeToLanguage(WORD wCode)
{
    switch(wCode)
    {
    case 0x6a61: // Japanese, 'ja'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageJapanese);
        break;
    case 0x656e: // English, 'en'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageEnglish);
        break;
    case 0x6672: // French, 'fr'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageFrench);
        break;
    case 0x6465: // German, 'de'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageGerman);
        break;
    case 0x7275: // Russian, 'ru'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageRussian);
        break;
    case 0x7a68: // Chinese, 'zh'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageChinese);
        break;
    case 0x6b6f: // Korean, 'ko'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageKorean);
        break;
    case 0x6869: // Hindi, 'hi'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageHindi);
        break;
    case 0x7468: // Thai, 'th'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageThai);
        break;
    case 0x6974: // Italian, 'it'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageItalian);
        break;
    case 0x6573: // Spanish, 'es'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageSpanish);
        break;
    case 0x7074: // Portuguese, 'pt'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguagePortuguese);
        break;
    case 0x6e6c: // Dutch, 'nl'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageDutch);
        break;
    case 0x656c: // Greek, 'el'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageGreek);
        break;
    case 0x6c61: // Latin, 'la'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageLatin);
        break;
    case 0x6e6f: // Norwegian, 'no'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageNorwegian);
        break;
    case 0x6461: // Danish, 'da'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageDanish);
        break;
    case 0x6669: // Finnish, 'fi'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageFinnish);
        break;
    case 0x7376: // Swedish, 'sv'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageSwedish);
        break;
    case 0x6875: // Hungarian, 'hu'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageHungarian);
        break;
    case 0x706c: // Polish, 'pl'
        OSDUTL_CopyStringFromIndex(aDSPLYLanugagePolish);
        break;
    case 0x6973: // Icelandic, 'is'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageIcelandic);
        break;
    case 0x6977: // Hebrew, 'iw'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageHebrew);
        break;
    case 0x6373: // Czech, 'cs'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageCzech);
        break;
    case 0x6172: // Arabic, 'ar'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageArabic);
        break;
    case 0x6d73: // Malay, 'ms'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageMalay);
        break;
    case 0x696e: // Indonesian, 'in'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageIndonesian);
        break;
    case 0x6865: // Hebrew, 'he' **** Is it 'heb' or 'her'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageHebrew);
        break;
    case 0x7472: // Turkish, 'tr'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageTurkish);
        break;
    case 0x6872: // Croatian, 'hr'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageCroatian);
        break;
    case 0x6267: // Bulgarian, 'bg'
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageBulgarian);
        break;
    default:
        OSDUTL_CopyStringFromIndex(aDSPLYLanguageOthers);
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_ShowSPST1Info
//  Abstract    :   This function will show the SPST info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ShowSPST1Info(BYTE bNavigatorUI)
{
    if (__bSPSTNs) // exist SP
    {
        // spst is on, ie. __bSP_Disp_OFF is 0 or 128 ~255
        if ((__bSP_Disp_OFF==0) || (__bSP_Disp_OFF&0x80))
        {
            if (bNavigatorUI)
            {
#ifndef NO_DISC_MODE //CoCo2.37
                // CoCo1.02, Change using DVD_GetLangCD to get Language Code
                _wOSDDSPLYTemp = DVD_GetLangCD(SET_SPST, __bSPSTNO);
                _OSDDSPLY_CodeToLanguage(_wOSDDSPLYTemp);            
#endif //#ifndef NO_DISC_MODE //CoCo2.37
            }
            else
            {
                _OSDDSPLY_CodeToLanguage(__wSP_LCD); //CoCo1.20, use __wSP_LCD to show SPST language directly.
            }
            
            OSDUTL_OutputNumFromIndex((WORD)(__bSPSTNO+1));
            OSDUTL_CopyStringFromIndex(aDSPLYSlash); //to be modified to "/"
            OSDUTL_OutputNumFromIndex(__bSPSTNs);
        }
        else // spst is off
        {
            OSDUTL_CopyStringFromIndex(aDSPLYOff);
        }
    }
    else  // no SP exist
    {
        OSDUTL_CopyStringFromIndex(aDSPLYUnknownSubtitle);
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_ShowASTInfo
//  Abstract    :   This function will show the AST info and "audio output" info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ShowASTInfo(void)
{
    if (__bASTNs) // AST exist
    {
        _OSDDSPLY_CodeToLanguage(__wA_LCD);

        OSDUTL_OutputNumFromIndex((WORD)(__bASTNO+1));
        OSDUTL_CopyStringFromIndex(aDSPLYSlash);
        OSDUTL_OutputNumFromIndex(__bASTNs);

        //Draw distance between audio type and info.
        OSDUTL_CopyStringFromIndex(aDSPLYInfoDistance);

        //show audio output
        _OSDND_OutputAudioType(__bAudioType);
                
        //Draw distance between audio type and info.
        OSDUTL_CopyStringFromIndex(aDSPLYInfoDistance);

        if (__bACHN != 0)
            _OSDDND_OutputAudioChannel(__bACHN);
    }
    else
        OSDUTL_CopyStringFromIndex(aDSPLYUnknownLanguage);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_ShowAngleInfo
//  Abstract    :   This function will show the angle info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ShowAngleInfo(void)
{
    if (__bAGL_Ns) //angle exist
    {
        OSDUTL_OutputNumFromIndex(__bAGLN);
        OSDUTL_CopyStringFromIndex(aDSPLYSlash);
        OSDUTL_OutputNumFromIndex(__bAGL_Ns);
    }
    else
    {
        OSDUTL_CopyStringFromIndex(aDSPLYUnknownAngle);
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_ShowPlayingStatus
//  Abstract    :   This function will show playing status.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ShowPlayingStatus(void)
{
    switch (gcShowStatus.bPlayingMode)
    {
    case MSG_STOP:
        if (__bCountStop == 1) //pre-stop
            __pOSDUTLOutputString = aDSPLYPreStopSymbol;
        else 
            __pOSDUTLOutputString = aDSPLYStopSymbol;
        break;
    case MSG_PAUSE:
        __pOSDUTLOutputString = aDSPLYPauseSymbol;
        break;        
    case MSG_STEP:
        __pOSDUTLOutputString = aDSPLYStepSymbol;
        break;    
    case MSG_FAST:
    case MSG_SCANFORWARD:
        __pOSDUTLOutputString = aDSPLYSCFSymbol;
        break;
    case MSG_SCANBACKWARD:
        __pOSDUTLOutputString = aDSPLYSCBSymbol;
        break;
    case MSG_SLOW:
        __pOSDUTLOutputString = aDSPLYSlowSymbol;
        break;
    default: //always treat it as playing mode.
        __pOSDUTLOutputString = aDSPLYPlaySymbol;
        break;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_ShowTVInfo
//  Abstract    :   This function will show TV info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_ShowTVInfo(void)
{
    switch (__SetupInfo.bTVSystem)
    {
    case SETUP_SCREEN_TV_SYSTEM_NTSC:
        OSDUTL_CopyStringFromIndex(aDSPLYNTSC);
        break;
    case SETUP_SCREEN_TV_SYSTEM_AUTO:
        OSDUTL_CopyStringFromIndex(aDSPLYAuto);
        break;
    case SETUP_SCREEN_TV_SYSTEM_PAL:
        OSDUTL_CopyStringFromIndex(aDSPLYPAL);
        break;
    case SETUP_SCREEN_TV_SYSTEM_PAL_M:
        OSDUTL_CopyStringFromIndex(aDSPLYPALM);
        break;
    case SETUP_SCREEN_TV_SYSTEM_PAL_N:
        OSDUTL_CopyStringFromIndex(aDSPLYPALN);
        break;
    }

    //Add the distance between TV mode and PScan
    OSDUTL_CopyStringFromIndex(aDSPLYTVDistance);

    if (__SetupInfo.bProgressiveScan == SETUP_PROGRESSIVE_SCAN_ON)
        OSDUTL_CopyStringFromIndex(aDSPLYpscan);
    else
        OSDUTL_CopyStringFromIndex(aDSPLYInterlace);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_SameTimeType
//  Abstract    :   This function will compare if the "time" message is equal to 
//              :   the display mode.
//  Arguments   :   bMessage: the message.
//  Return		:   TRUE: equal
//              :   FALSE: not 
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_SameTimeType(BYTE bMessage)
{
    //show type of the time.
    switch (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID)
    {
    case OSDDSPLY_MODE_DVD_TITLE_ELAPSED:
        _bOSDDSPLYTemp = MSG_TITLE_TIME;
        break;
    case OSDDSPLY_MODE_DVD_TITLE_REMAIN:
        _bOSDDSPLYTemp = MSG_REMAIN_TITLE_TIME;
        break;
    case OSDDSPLY_MODE_DVD_CHAPTER_ELAPSED:
        _bOSDDSPLYTemp = MSG_CHAPTER_TIME;
        break;
    case OSDDSPLY_MODE_DVD_CHAPTER_REMAIN:
        _bOSDDSPLYTemp = MSG_REMAIN_CHAPTER_TIME;
        break;
    case OSDDSPLY_MODE_VCD_SINGLE_ELAPSED:
        _bOSDDSPLYTemp = MSG_RELTIME;
        break;
    case OSDDSPLY_MODE_VCD_SINGLE_REMAIN:
        _bOSDDSPLYTemp = MSG_REMAIN_RELTIME;
        break;
    case OSDDSPLY_MODE_VCD_TOTAL_ELAPSED:
        _bOSDDSPLYTemp = MSG_ABSTIME;
        break;
    case OSDDSPLY_MODE_VCD_TOTAL_REMAIN:
        _bOSDDSPLYTemp = MSG_REMAIN_ABSTIME;
        break;
    case OSDDSPLY_MODE_CDROM_AUDIO:
        _bOSDDSPLYTemp = MSG_RELTIME;
        break;
    default:
        return FALSE;
    }

    if (_bOSDDSPLYTemp == bMessage)
        return TRUE;
    else
        return FALSE;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_LD_ClearRegion
//  Abstract    :   This function will clear the region for one-line display.
//  Arguments   :   bClearRegion: The region that you want to clear.
//              :   bPos: The relative position of the region that you want to clear.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_ClearRegion(BYTE bClearRegion, BYTE bPos)
{
    switch (bClearRegion)
    {
    case OSDDSPLY_REGION_LD_TITLE:        
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_REGION_TITLE_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_REGION_TITLE_START_H + OSDDSPLY_LD_REGION_TITLE_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_START_V + OSDDSPLY_LD_HEIGHT;
        break;
    case OSDDSPLY_REGION_LD_CHAPTER:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_REGION_CHAPTER_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_REGION_CHAPTER_START_H + OSDDSPLY_LD_REGION_CHAPTER_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_START_V + OSDDSPLY_LD_HEIGHT;
        break;
    case OSDDSPLY_REGION_LD_TIME:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_REGION_TIME_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_REGION_TIME_START_H + OSDDSPLY_LD_REGION_TIME_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_START_V + OSDDSPLY_LD_HEIGHT;
        break;
    case OSDDSPLY_REGION_LD_PROGRAM_STATUS:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_REGION_PROGRAM_STATUS_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_REGION_PROGRAM_STATUS_START_H + OSDDSPLY_LD_REGION_PROGRAM_STATUS_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_START_V + OSDDSPLY_LD_HEIGHT;
        break;
    case OSDDSPLY_REGION_TIME_LD_TIME:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_TIME_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_TIME_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_TIME_START_H + OSDDSPLY_LD_TIME_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_TIME_START_V + OSDDSPLY_LD_HEIGHT;
        break;
    case OSDDSPLY_REGION_LD_FILE_NAME:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_REGION_FILE_NAME_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_REGION_FILE_NAME_START_H + OSDDSPLY_LD_REGION_FILE_NAME_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_START_V + OSDDSPLY_LD_HEIGHT;
        break;
    case OSDDSPLY_REGION_LD_FILE_NUM:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_REGION_FILE_NUM_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_REGION_FILE_NUM_START_H + OSDDSPLY_LD_REGION_FILE_NUM_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_START_V + OSDDSPLY_LD_HEIGHT;
        break;
    case OSDDSPLY_REGION_LD_CDROM_TIME:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_REGION_CDROM_TIME_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_REGION_CDROM_TIME_START_H + OSDDSPLY_LD_REGION_CDROM_TIME_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_START_V + OSDDSPLY_LD_HEIGHT;
        break;
	case OSDDSPLY_REGION_LD_MENU:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_LD_REGION_MENU_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_LD_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_LD_REGION_MENU_START_H + OSDDSPLY_LD_REGION_MENU_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_LD_START_V + OSDDSPLY_LD_HEIGHT;
		break;
    default:
        break;
    }

    _OSDDSPLYRect.rect.wRight -= 1;
    _OSDDSPLYRect.rect.wBottom -= 1;
    
    _OSDDSPLYRect.rect.wLeft += bPos;
    _OSDDSPLYRect.bColor = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND;
     
    GDI_FillRect_909(0, &_OSDDSPLYRect);
}


//***************************************************************************
//  Function    :   _OSDDSPLY_LD_UpdateUnknownRegions
//  Abstract    :   This function will update the unknown status for all regions.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_UpdateUnknownRegions(void)
{
    //show unknown status
    if (__wDiscType & BOOK_DVD)
    {
        _OSDDSPLY_LD_ShowInfo(MSG_TITLE);
        _OSDDSPLY_LD_ShowInfo(MSG_CHAPTER);
    }
    else
    {
        if ((__bCountStop == 2) && (!(__wDiscType & BOOK_CDROM)))
            _OSDDSPLY_LD_ShowUnknownTrack(); 
        else
        {
            _wOSDDSPLYInfo = gcShowStatus.wCurrentTrack;
            _OSDDSPLY_LD_ShowInfo(MSG_TRACK);
        }
    }

    _OSDDSPLY_LD_ShowTime(OSDDSPLY_UPDATE_INFO);

    //Update the program status in stop mode.
    _OSDDSPLY_LD_ShowInfo(MSG_PROGRAM);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_LD_ShowUnknownTrack
//  Abstract    :   This function will show unknown track.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_ShowUnknownTrack(void)
{
    _wOSDDSPLYHPos = _OSDDSPLY_GetInfoPos(MSG_TRACK);

    OSDUTL_PrepareOutputString();
    
    OSDUTL_CopyStringFromIndex(aDSPLYStopTrack);
    __wOSDUTLStringBuf[__bOSDUTLIndex++] = aDSPLYSlash[1]; //to be modified to "/"
    
    if (gcShowStatus.wTotalTracks != 0)
        OSDUTL_OutputNumFromIndex(gcShowStatus.wTotalTracks);
    else
        OSDUTL_CopyStringFromIndex(aDSPLYUnknownTrack);
    
    _OSDDSPLY_LD_DrawRegion(_OSDDSPLY_LD_GetRegion(MSG_TRACK), OSDDSPLY_UPDATE_INFO, _wOSDDSPLYHPos);
}

WORD _OSDDSPLY_ShowDirNameInfo(WORD wMaxLength)
{
    BYTE bIndex = 0;
    BYTE bCharWidth = 0;
    WORD wOutputCode = 0;

    //Transfrom relative ID to absolute ID        
    MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMPlayingItem.wDirID, (WORD)(__wPlayItem - 1)), 1);
    
    //Dump file record.
    if (MM_GetDumpedFileRecord() == MM_FILE_RECORD_DUMP_FAILED)
        return 0;

    _wOSDDSPLYTemp = wMaxLength;

    if (__pMMDirNameList[__MMPlayingItem.wDirID].bMMLinkStatus & IF_UNICODE_FILE_TAG)
    {
        // Unicode
        for (bIndex = 0; bIndex < MM_MAX_DIRNAME_LENGTH; bIndex+=2)
        {
            // Each character use two bytes.
            wOutputCode = (WORD)(BYTE)(__pMMDirNameList[__MMPlayingItem.wDirID].cName[bIndex]);
            wOutputCode |= ((WORD)(BYTE)(__pMMDirNameList[__MMPlayingItem.wDirID].cName[bIndex+1])) << 8; 
			
            if (wOutputCode == 0) // End of string.
            {
                break;
            }
#ifdef SUPPORT_UNICODE_CODING
            if (__bEnCodeType != CHAR_ENCODE_ISO_CP)
            {
                __wOSDUTLStringBuf[(bIndex/2)+1]= wOutputCode;
                bCharWidth = GDI_GetUniFontWidth(__wOSDUTLStringBuf[(bIndex/2)+1]);
            }
            else
#endif
            {
                __wOSDUTLStringBuf[(bIndex/2)+1] = CONVCHAR_Uni_To_CTK(wOutputCode);
                bCharWidth = GDI_GetCharWidth(__wOSDUTLStringBuf[(bIndex/2)+1]);
            }      
			
            if (_wOSDDSPLYTemp >= bCharWidth)
            {
                _wOSDDSPLYTemp -= bCharWidth;                
            }
            else
            {
                // The length of string exceed the default maximum length.
                break;
            }
        }
		
        __wOSDUTLStringBuf[0] = bIndex / 2;
    }
    else
    {
        //ASCII 
        while (__pMMDirNameList[__MMPlayingItem.wDirID].cName[bIndex] != '\0')
        {
            __wOSDUTLStringBuf[bIndex+1] = CONVCHAR_ASCII_To_CTK(__pMMDirNameList[__MMPlayingItem.wDirID].cName[bIndex]);

            bCharWidth = GDI_GetCharWidth(__wOSDUTLStringBuf[bIndex+1]);
            if (_wOSDDSPLYTemp >= bCharWidth)
            {
                _wOSDDSPLYTemp -= bCharWidth;
                bIndex++;
            }
            else
            {
                //Dont't increase bIndex because this charachter should not be added to the array.
                break;
            }          
        }

        __wOSDUTLStringBuf[0] = bIndex;
    }	

	return _wOSDDSPLYTemp;
}

WORD _OSDDSPLY_ShowFileNameInfo(WORD wMaxLength)
{
    BYTE bIndex = 0;
    BYTE bCharWidth = 0;
    WORD wOutputCode = 0;

    //Transfrom relative ID to absolute ID        
    MM_TriggerDumping(MM_ID_RelativeToAbsolute(__MMPlayingItem.wDirID, (WORD)(__wPlayItem - 1)), 1);
    
    //Dump file record.
    if (MM_GetDumpedFileRecord() == MM_FILE_RECORD_DUMP_FAILED)
        return 0;
            
    //File name
    _wOSDDSPLYTemp = wMaxLength;

    if (__pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG)
    {
        // Unicode
        for (bIndex = 0; bIndex < MM_MAX_FILENAME_LENGTH; bIndex+=2)
        {
            // Each character use two bytes.
            wOutputCode = (WORD)(BYTE)(__pMMFileRecord->FileName.cName[bIndex]);
            wOutputCode |= ((WORD)(BYTE)(__pMMFileRecord->FileName.cName[bIndex+1])) << 8; 
			
            if (wOutputCode == 0 || wOutputCode == 0x2E) // End of string or extension.
            {
                break;
            }         
#ifdef SUPPORT_UNICODE_CODING
            if (__bEnCodeType != CHAR_ENCODE_ISO_CP)
            {
                __wOSDUTLStringBuf[(bIndex/2)+1]= wOutputCode;
                bCharWidth = GDI_GetUniFontWidth(__wOSDUTLStringBuf[(bIndex/2)+1]);			
            }
            else
#endif
            {
                __wOSDUTLStringBuf[(bIndex/2)+1] = CONVCHAR_Uni_To_CTK(wOutputCode);
                bCharWidth = GDI_GetCharWidth(__wOSDUTLStringBuf[(bIndex/2)+1]);
            }            
			
            if (_wOSDDSPLYTemp >= bCharWidth)
            {
                _wOSDDSPLYTemp -= bCharWidth;                
            }
            else
            {
                // The length of string exceed the default maximum length.
                break;
            }
        }
		
        __wOSDUTLStringBuf[0] = bIndex / 2;
    }
    else
    {
        //ASCII 
        while (__pMMFileRecord->FileName.cName[bIndex] != '\0')
        {
            __wOSDUTLStringBuf[bIndex+1] = CONVCHAR_ASCII_To_CTK(__pMMFileRecord->FileName.cName[bIndex]);

            bCharWidth = GDI_GetCharWidth(__wOSDUTLStringBuf[bIndex+1]);
            if (_wOSDDSPLYTemp >= bCharWidth)
            {
                _wOSDDSPLYTemp -= bCharWidth;
                bIndex++;
            }
            else
            {
                //Dont't increase bIndex because this charachter should not be added to the array.
                break;
            }            
        }

        __wOSDUTLStringBuf[0] = bIndex;
    }

	return _wOSDDSPLYTemp;
}


//***************************************************************************
//  Function    :   _OSDDSPLY_LD_ShowFileNameInfo
//  Abstract    :   This function will show the dir and file name.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_LD_ShowFileNameInfo(WORD wMaxLength, WORD wHPos)
{
	WORD wStringWidth;
	WORD wMaxDirFileLength;

	wMaxDirFileLength = wMaxLength;
	_OSDDSPLY_LD_ClearRegion(OSDDSPLY_REGION_LD_FILE_NAME, wHPos);

    _OSDDSPLYStringAttr.bBackground = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND; //PAL_ENTRY_COLOR_TRANSPARENT; //Set the color of background is the same as color key.
    _OSDDSPLYStringAttr.bColorKey = 0;
    _OSDDSPLYStringAttr.bShadow1 = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND; //Use OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND to prevent the "Open Folder" icon is wrong.
    _OSDDSPLYStringAttr.bShadow2 = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND; //Use OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND to prevent the "Open Folder" icon is wrong.
	_OSDDSPLYStringAttr.bTextColor = OSDDSPLY_LD_REGION_FILE_NAME_INFO_COLOR;
	_OSDDSPLYStringAttr.wX = OSDDSPLY_LD_REGION_FILE_NAME_START_H;
    _OSDDSPLYStringAttr.wY = OSDDSPLY_LD_START_V + ((OSDDSPLY_LD_HEIGHT - OSDDSPLY_STRING_HEIGHT) >> 1);
    _OSDDSPLYStringAttr.wX += wHPos;

	wStringWidth = _OSDDSPLY_ShowDirNameInfo(wMaxDirFileLength/2);	

	//Draw the Dir string.
#ifdef SUPPORT_UNICODE_CODING
    if ((__pMMDirNameList[__MMPlayingItem.wDirID].bMMLinkStatus & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
    {       
        _OSDDSPLY_DrawStringByUnicode();                
    }
    else
#endif
    {        
        _OSDDSPLY_DrawString();
    } 

	//Get left space for file staring.
	wMaxDirFileLength = (wMaxLength / 2) + wStringWidth;

	_OSDDSPLYStringAttr.wX += ((wMaxLength / 2) - wStringWidth);
    OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aDSPLYReverseSolidus);	  
    _OSDDSPLY_DrawString();
    wMaxDirFileLength -= GDI_GetStringWidth(aDSPLYReverseSolidus);

	//Draw File Name
	_OSDDSPLY_ShowFileNameInfo(wMaxDirFileLength);

	_OSDDSPLYStringAttr.wX += GDI_GetStringWidth(aDSPLYReverseSolidus);
    
	//Draw file string.
#ifdef SUPPORT_UNICODE_CODING
     if ((__pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
     {
         _OSDDSPLY_DrawStringByUnicode();
     }
     else
#endif
     {
         _OSDDSPLY_DrawString();
     }    
}

//***************************************************************************
//  Function    :   _OSDDSPLY_VP_ShowFileNameInfo
//  Abstract    :   This function will show the dir and file name.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_VP_ShowFileNameInfo(WORD wMaxLength, WORD wHPos)
{
	WORD wStringWidth;
	WORD wMaxDirFileLength;

	wMaxDirFileLength = wMaxLength;

    _OSDDSPLYStringAttr.bBackground = OSDDSPLY_VP_PALETTE_ENTRY_GENERAL_BACKGROUND; //PAL_ENTRY_COLOR_TRANSPARENT; //Set the color of background is the same as color key.
    _OSDDSPLYStringAttr.bColorKey = 0;
    _OSDDSPLYStringAttr.bShadow1 = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND; //Use OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND to prevent the "Open Folder" icon is wrong.
    _OSDDSPLYStringAttr.bShadow2 = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND; //Use OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND to prevent the "Open Folder" icon is wrong.

	if (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID == OSDDSPLY_MODE_DIVX_VIRTUAL_PANEL)
	{
		_OSDDSPLY_VP_ClearRegion(OSDDSPLY_REGION_VP_MOTION_FILE_NAME, wHPos);
		_OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_INFO_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_START_H+wHPos;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_START_V + ((OSDDSPLY_VP_REGION_MOTION_FILE_NAME_HEIGHT - GDI_GetStringHeight(__pOSDUTLOutputString)) >> 1);
	}
	else
	{
		_OSDDSPLY_VP_ClearRegion(OSDDSPLY_REGION_VP_FILE_NAME, wHPos);
		_OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_FILE_NAME_INFO_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_FILE_NAME_START_H+wHPos;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_FILE_NAME_START_V + ((OSDDSPLY_VP_REGION_FILE_NAME_HEIGHT - GDI_GetStringHeight(__pOSDUTLOutputString)) >> 1);
	}

	wStringWidth = _OSDDSPLY_ShowDirNameInfo(wMaxDirFileLength/2);

	wMaxDirFileLength = (wMaxLength / 2) + wStringWidth;

	if (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID == OSDDSPLY_MODE_DIVX_VIRTUAL_PANEL)
	{
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_START_V + ((OSDDSPLY_VP_REGION_MOTION_FILE_NAME_HEIGHT - OSDDSPLY_STRING_HEIGHT) >> 1);
	}
	else
	{
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_FILE_NAME_START_V + ((OSDDSPLY_VP_REGION_FILE_NAME_HEIGHT - OSDDSPLY_STRING_HEIGHT) >> 1);
	}

	//Draw the Dir string.
#ifdef SUPPORT_UNICODE_CODING
    if ((__pMMDirNameList[__MMPlayingItem.wDirID].bMMLinkStatus & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
    {
        _OSDDSPLY_DrawStringByUnicode();                
    }
    else
#endif
    {
        _OSDDSPLY_DrawString();
    }       

     _OSDDSPLYStringAttr.wX += ((wMaxLength / 2) - wStringWidth);
     OSDUTL_PrepareOutputString();
    OSDUTL_CopyStringFromIndex(aDSPLYReverseSolidus);	  
    _OSDDSPLY_DrawString();
    wMaxDirFileLength -= GDI_GetStringWidth(aDSPLYReverseSolidus);	

	//Draw File Name
	_OSDDSPLY_ShowFileNameInfo(wMaxDirFileLength);

	_OSDDSPLYStringAttr.wX += GDI_GetStringWidth(aDSPLYReverseSolidus);
    
	//Draw file string.
#ifdef SUPPORT_UNICODE_CODING
     if ((__pMMFileRecord->bFileAttr & IF_UNICODE_FILE_TAG) && (__bEnCodeType != CHAR_ENCODE_ISO_CP))
     {
         _OSDDSPLY_DrawStringByUnicode();
     }
     else
#endif
     {
         _OSDDSPLY_DrawString();
     }    
}
	
//***************************************************************************
//  Function    :   _OSDDSPLY_VP_ShowItem
//  Abstract    :   This function will show item according to the message.
//  Arguments   :   bMessage: the message.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_VP_ShowItem(BYTE bMessage)
{   
    if (_bOSDDSPLYTitleMenuMode && (bMessage == MSG_CHAPTER))
        return;
       
    _OSDDSPLY_OutputString(bMessage);    

    _OSDDSPLY_VP_DrawRegion(_OSDDSPLY_VP_GetRegion(bMessage), OSDDSPLY_UPDATE_ITEM, 0);
}


//***************************************************************************
//  Function    :   _OSDDSPLY_VP_ShowInfo
//  Abstract    :   This function will show info. according to the message.
//  Arguments   :   bMessage: the message.
//  Return		:   TRUE: The info. has been updated.
//              :   FALSE: The message is not supported.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_VP_ShowInfo(BYTE bMessage)
{ 
    //Filter the info. that JPEG virtual panel doen't support it.
    if (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID == OSDDSPLY_MODE_JPEG_VIRTUAL_PANEL)
    {
        if ((bMessage == MSG_TVMODE) || (bMessage == MSG_PSCAN))
            return FALSE;
    }
    
#ifndef NO_DISC_MODE //CoCo2.37   
    if ((bMessage == MSG_TITLE) && ((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM))) // Root Menu
    {
        //Clear "Menu" region. That is "Title+Chapter" region.
        _OSDDSPLY_VP_ClearRegion(OSDDSPLY_REGION_VP_MENU, 0);
    }
    else
#endif //#ifndef NO_DISC_MODE //CoCo2.37 
    {
        _wOSDDSPLYHPos = _OSDDSPLY_GetInfoPos(bMessage);    
        _OSDDSPLY_VP_ClearRegion(_OSDDSPLY_VP_GetRegion(bMessage), (BYTE)_wOSDDSPLYHPos);
        
#ifndef NO_DISC_MODE //CoCo2.37  
        if (_bOSDDSPLYTitleMenuMode && (bMessage == MSG_TITLE) && (!((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM))))
        {
            _bOSDDSPLYTitleMenuMode = FALSE;
            
            //Recover "title" and "chapter" item.
            //Clear "menu" region
            _OSDDSPLY_VP_ClearRegion(OSDDSPLY_REGION_VP_MENU, 0);
            
            //Show "Title" item
            _OSDDSPLY_VP_ShowItem(MSG_TITLE);
            
            //Show "chapter" item
            _OSDDSPLY_VP_ShowItem(MSG_CHAPTER); 
        }
#endif //#ifndef NO_DISC_MODE //CoCo2.37   
    }

    OSDUTL_PrepareOutputString();

    switch (bMessage)
    {
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
#ifndef NO_DISC_MODE //CoCo2.37   
    case MSG_TITLE:
        if((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM)) // Root Menu
        {
            _bOSDDSPLYTitleMenuMode = TRUE; 

            //Show "Title Menu"
            _OSDDSPLY_ShowTitleMenu();
        }
        else
            _OSDDSPLY_ShowTitleInfo();
        break;
    case MSG_CHAPTER:
        if ((__bDomain==VTSM_DOM) || (__bDomain==VMGM_DOM))
            return TRUE; //Don't show the chapter info. in title menu/DVD menu.
        else
		{
            _OSDDSPLY_ShowChapterInfo();
		}
        break;
#endif //#ifndef NO_DISC_MODE //CoCo2.37   
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
    case MSG_FILE_NAME:
        if (__bAttrPlay == ATTR_JPG)
		{
            //_OSDDSPLY_ShowFileNameInfo(OSDDSPLY_VP_REGION_FILE_NAME_WIDTH-_wOSDDSPLYHPos); //JPEG
			_OSDDSPLY_VP_ShowFileNameInfo(OSDDSPLY_VP_REGION_FILE_NAME_WIDTH-_wOSDDSPLYHPos, _wOSDDSPLYHPos); //JPEG
		}
        else
		{
            //_OSDDSPLY_ShowFileNameInfo(OSDDSPLY_VP_REGION_MOTION_FILE_NAME_WIDTH-_wOSDDSPLYHPos); //DivX
			_OSDDSPLY_VP_ShowFileNameInfo(OSDDSPLY_VP_REGION_MOTION_FILE_NAME_WIDTH-_wOSDDSPLYHPos, _wOSDDSPLYHPos); //DivX
		}
		return TRUE;
        //break;
    case MSG_FILE_NUM:
        _OSDDSPLY_ShowTrackInfo();
        break;
    case MSG_JPEG_RESOLUTION:
        OSDUTL_ShowJPEGResolution();
        break;
    case MSG_JPEG_DATE:        
		OSDUTL_ShowJPEGDate(__MMPlayingItem.wDirID, (WORD)(__wPlayItem - 1));
        break;
    case MSG_SPST_CTL:
        _OSDDSPLY_ShowSPST1Info(FALSE);
        break;
    case MSG_AST_CTL:
        _OSDDSPLY_ShowASTInfo();
        break;
    case MSG_TVMODE:
    case MSG_PSCAN:
         _OSDDSPLY_ShowTVInfo();
        break;
    case MSG_ANGLE_CTL:
        _OSDDSPLY_ShowAngleInfo();
        break;
    case MSG_STATUS:
        _OSDDSPLY_ShowPlayingStatus();
        break;
    case MSG_REPEAT:
        if (__bRepeat == REPEAT_NONE)
        {
            //Clear the "repeat" symbol.
            _OSDDSPLY_VP_ClearRegion(OSDDSPLY_REGION_VP_REPEAT_STATUS, 0);
            return FALSE; //return FALSE to let normal display to show it.
        }
        else 
        {
            __pOSDUTLOutputString = aDSPLYRepeatSymbol;
        }
        break;
    case MSG_PROGRAM:
        if (__btPlayingProgram)
            __pOSDUTLOutputString = aDSPLYProgramSymbol;
        else
        {
            //Clear the "Program" symbol.
            _OSDDSPLY_VP_ClearRegion(OSDDSPLY_REGION_VP_PROGRAM_STATUS, 0);
            return TRUE;
        }
        break;
    default:
        return FALSE;
    }

    if (_bOSDDSPLYTitleMenuMode && (bMessage == MSG_TITLE))
        _OSDDSPLY_VP_DrawRegion(OSDDSPLY_REGION_VP_MENU, OSDDSPLY_UPDATE_INFO, 0);
    else
        _OSDDSPLY_VP_DrawRegion(_OSDDSPLY_VP_GetRegion(bMessage), OSDDSPLY_UPDATE_INFO, (BYTE)_wOSDDSPLYHPos);

    if ((bMessage == MSG_REPEAT) || (bMessage == MSG_PROGRAM)) //let OSD module process the message again.
        return FALSE;
    else
        return TRUE;
}


//***************************************************************************
//  Function    :   _OSDDSPLY_VP_GetRegion
//  Abstract    :   This function will get the message's region.
//  Arguments   :   bMessage: The message.
//  Return		:   return the region.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_VP_GetRegion(BYTE bMessage)
{
    switch (bMessage)
    {
    case MSG_TITLE:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_TITLE;
        break;
    case MSG_CHAPTER:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_CHAPTER;
        break;
    case MSG_SPST_CTL:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_SUBTITLE;
        break;
    case MSG_AST_CTL:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_AST;
        break;
    case MSG_TVMODE:
    case MSG_PSCAN:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_TV;
        break;
    case MSG_ANGLE_CTL:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_ANGLE;
        break;
    case MSG_STATUS:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_PLAY_STATUS;
        break;
    case MSG_REPEAT:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_REPEAT_STATUS;
        break;
    case MSG_PROGRAM:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_PROGRAM_STATUS;
        break;
    case MSG_FILE_NAME:
        if (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID == OSDDSPLY_MODE_DIVX_VIRTUAL_PANEL)
            _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_MOTION_FILE_NAME;
        
        else
            _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_FILE_NAME;
        break;
    case MSG_FILE_NUM:
        if (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID == OSDDSPLY_MODE_DIVX_VIRTUAL_PANEL)
            _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_MOTION_FILE_NUM;        
        else
            _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_FILE_NUM;
        break;
    case MSG_JPEG_RESOLUTION:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_RESOLUTION;
        break;
    case MSG_JPEG_DATE:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_DATE;
        break;
    case MSG_ABSTIME:
    case MSG_RELTIME:
    case MSG_TITLE_TIME:
    case MSG_CHAPTER_TIME:
    case MSG_REMAIN_ABSTIME:
    case MSG_REMAIN_RELTIME:
    case MSG_REMAIN_TITLE_TIME:
    case MSG_REMAIN_CHAPTER_TIME:
        _bOSDDSPLYTemp = OSDDSPLY_REGION_VP_TIME;
        break;        
    default:
        break;
    }
    
    return _bOSDDSPLYTemp;
}


//***************************************************************************
//  Function    :   _OSDDSPLY_VP_ClearRegion
//  Abstract    :   This function will clear the region for one-line display.
//  Arguments   :   bClearRegion: The region that you want to clear.
//              :   bPos: The relative position of the region that you want to clear.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_VP_ClearRegion(BYTE bClearRegion, BYTE bPos)
{
    switch (bClearRegion)
    {
    case OSDDSPLY_REGION_VP_TITLE:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_TITLE_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_TITLE_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_TITLE_START_H + OSDDSPLY_VP_REGION_TITLE_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_TITLE_START_V + OSDDSPLY_VP_REGION_TITLE_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_CHAPTER:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_CHAPTER_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_CHAPTER_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_CHAPTER_START_H + OSDDSPLY_VP_REGION_CHAPTER_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_CHAPTER_START_V + OSDDSPLY_VP_REGION_CHAPTER_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_MENU:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_MENU_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_MENU_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_MENU_START_H + OSDDSPLY_VP_REGION_MENU_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_MENU_START_V + OSDDSPLY_VP_REGION_MENU_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_SUBTITLE:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_SUBTITLE_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_SUBTITLE_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_SUBTITLE_START_H + OSDDSPLY_VP_REGION_SUBTITLE_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_SUBTITLE_START_V + OSDDSPLY_VP_REGION_SUBTITLE_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_AST:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_AST_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_AST_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_AST_START_H + OSDDSPLY_VP_REGION_AST_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_AST_START_V + OSDDSPLY_VP_REGION_AST_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_TIME:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_TIME_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_TIME_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_TIME_START_H + OSDDSPLY_VP_REGION_TIME_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_TIME_START_V + OSDDSPLY_VP_REGION_TIME_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_PROGRAM_STATUS:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_PROGRAM_STATUS_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_PROGRAM_STATUS_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_PROGRAM_STATUS_START_H + OSDDSPLY_VP_REGION_PROGRAM_STATUS_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_PROGRAM_STATUS_START_V + OSDDSPLY_VP_REGION_PROGRAM_STATUS_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_REPEAT_STATUS:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_REPEAT_STATUS_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_REPEAT_STATUS_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_REPEAT_STATUS_START_H + OSDDSPLY_VP_REGION_REPEAT_STATUS_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_REPEAT_STATUS_START_V + OSDDSPLY_VP_REGION_REPEAT_STATUS_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_PLAY_STATUS:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_PLAY_STATUS_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_PLAY_STATUS_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_PLAY_STATUS_START_H + OSDDSPLY_VP_REGION_PLAY_STATUS_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_PLAY_STATUS_START_V + OSDDSPLY_VP_REGION_PLAY_STATUS_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_TV:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_TV_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_TV_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_TV_START_H + OSDDSPLY_VP_REGION_TV_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_TV_START_V + OSDDSPLY_VP_REGION_TV_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_ANGLE:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_ANGLE_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_ANGLE_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_ANGLE_START_H + OSDDSPLY_VP_REGION_ANGLE_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_ANGLE_START_V + OSDDSPLY_VP_REGION_ANGLE_HEIGHT;           
        break;
    case OSDDSPLY_REGION_VP_FILE_NAME:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_FILE_NAME_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_FILE_NAME_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_FILE_NAME_START_H + OSDDSPLY_VP_REGION_FILE_NAME_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_FILE_NAME_START_V + OSDDSPLY_VP_REGION_FILE_NAME_HEIGHT;
        break;
    case OSDDSPLY_REGION_VP_FILE_NUM:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_FILE_NUM_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_FILE_NUM_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_FILE_NUM_START_H + OSDDSPLY_VP_REGION_FILE_NUM_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_FILE_NUM_START_V + OSDDSPLY_VP_REGION_FILE_NUM_HEIGHT;
        break;
    case OSDDSPLY_REGION_VP_RESOLUTION:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_RESOLUTION_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_RESOLUTION_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_RESOLUTION_START_H + OSDDSPLY_VP_REGION_RESOLUTION_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_RESOLUTION_START_V + OSDDSPLY_VP_REGION_RESOLUTION_HEIGHT;
        break;
    case OSDDSPLY_REGION_VP_DATE:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_DATE_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_DATE_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_DATE_START_H + OSDDSPLY_VP_REGION_DATE_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_DATE_START_V + OSDDSPLY_VP_REGION_DATE_HEIGHT;
        break;
    case OSDDSPLY_REGION_VP_MOTION_FILE_NAME:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_START_H + OSDDSPLY_VP_REGION_MOTION_FILE_NAME_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_START_V + OSDDSPLY_VP_REGION_MOTION_FILE_NAME_HEIGHT;
        break;
    case OSDDSPLY_REGION_VP_MOTION_FILE_NUM:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_MOTION_FILE_NUM_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_MOTION_FILE_NUM_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_MOTION_FILE_NUM_START_H + OSDDSPLY_VP_REGION_MOTION_FILE_NUM_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_MOTION_FILE_NUM_START_V + OSDDSPLY_VP_REGION_MOTION_FILE_NUM_HEIGHT;
        break;
    case OSDDSPLY_REGION_VP_MOTION_TIME:
        _OSDDSPLYRect.rect.wLeft = OSDDSPLY_VP_REGION_MOTION_TIME_START_H;
        _OSDDSPLYRect.rect.wTop = OSDDSPLY_VP_REGION_MOTION_TIME_START_V;
        _OSDDSPLYRect.rect.wRight = OSDDSPLY_VP_REGION_MOTION_TIME_START_H + OSDDSPLY_VP_REGION_MOTION_TIME_WIDTH;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLY_VP_REGION_MOTION_TIME_START_V + OSDDSPLY_VP_REGION_MOTION_TIME_HEIGHT;
        break;
    default:
        return;
    }

    _OSDDSPLYRect.rect.wRight -= 1;
    _OSDDSPLYRect.rect.wBottom -= 1;
    
    _OSDDSPLYRect.rect.wLeft += bPos;
    _OSDDSPLYRect.bColor = OSDDSPLY_VP_PALETTE_ENTRY_GENERAL_BACKGROUND;
     
    GDI_FillRect_909(0, &_OSDDSPLYRect);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_VP_DrawRegion
//  Abstract    :   This function will draw the region.
//  Arguments   :   bRegion: The region you want to draw.
//              :   bType: OSDLD_UPDATE_ITEM: You want to draw the item.
//              :          OSDDSPLY_UPDATE_INFO: You want to draw the info.
//              :   bPos: The relative position of the region.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_VP_DrawRegion(BYTE bRegion, BYTE bType, BYTE bPos)
{
    //clear all region except time region.
    if ((bRegion != OSDDSPLY_REGION_VP_TIME) && (bRegion != OSDDSPLY_REGION_VP_MOTION_TIME))
        _OSDDSPLY_VP_ClearRegion(bRegion, bPos);

    _OSDDSPLYStringAttr.bBackground = OSDDSPLY_VP_PALETTE_ENTRY_GENERAL_BACKGROUND; //PAL_ENTRY_COLOR_TRANSPARENT; //Set the color of background is the same as color key.
    _OSDDSPLYStringAttr.bColorKey = 0;
    _OSDDSPLYStringAttr.bShadow1 = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND; //Use OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND to prevent the "Open Folder" icon is wrong.
    _OSDDSPLYStringAttr.bShadow2 = OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND; //Use OSDDSPLY_LD_PALETTE_ENTRY_GENERAL_BACKGROUND to prevent the "Open Folder" icon is wrong.
    
    _bOSDDSPLYTemp = GDI_GetStringHeight_909(__pOSDUTLOutputString);

    switch (bRegion)
    {
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
    case OSDDSPLY_REGION_VP_TITLE:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_TITLE_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_TITLE_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_TITLE_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_TITLE_START_V + ((OSDDSPLY_VP_REGION_TITLE_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break; 
    case OSDDSPLY_REGION_VP_CHAPTER:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_CHAPTER_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_CHAPTER_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_CHAPTER_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_CHAPTER_START_V + ((OSDDSPLY_VP_REGION_CHAPTER_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_MENU:
        _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_MENU_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_MENU_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_MENU_START_V + ((OSDDSPLY_VP_REGION_MENU_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;         
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p        
    case OSDDSPLY_REGION_VP_TIME:
        _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_TIME_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_TIME_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_TIME_START_V + ((OSDDSPLY_VP_REGION_TIME_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_SUBTITLE:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_SUBTITLE_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_SUBTITLE_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_SUBTITLE_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_SUBTITLE_START_V + ((OSDDSPLY_VP_REGION_SUBTITLE_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_AST:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_AST_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_AST_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_AST_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_AST_START_V + ((OSDDSPLY_VP_REGION_AST_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_PROGRAM_STATUS:
        _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_PROGRAM_STATUS_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_PROGRAM_STATUS_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_PROGRAM_STATUS_START_V + ((OSDDSPLY_VP_REGION_PROGRAM_STATUS_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_REPEAT_STATUS:
        _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_REPEAT_STATUS_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_REPEAT_STATUS_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_REPEAT_STATUS_START_V + ((OSDDSPLY_VP_REGION_REPEAT_STATUS_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_PLAY_STATUS:
        _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_PLAY_STATUS_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_PLAY_STATUS_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_PLAY_STATUS_START_V + ((OSDDSPLY_VP_REGION_PLAY_STATUS_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_TV:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_TV_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_TV_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_TV_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_TV_START_V + ((OSDDSPLY_VP_REGION_TV_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_ANGLE:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_ANGLE_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_ANGLE_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_ANGLE_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_ANGLE_START_V + ((OSDDSPLY_VP_REGION_ANGLE_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_FILE_NAME:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_FILE_NAME_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_FILE_NAME_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_FILE_NAME_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_FILE_NAME_START_V + ((OSDDSPLY_VP_REGION_FILE_NAME_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_FILE_NUM:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_FILE_NUM_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_FILE_NUM_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_FILE_NUM_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_FILE_NUM_START_V + ((OSDDSPLY_VP_REGION_FILE_NUM_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_RESOLUTION:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_RESOLUTION_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_RESOLUTION_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_RESOLUTION_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_RESOLUTION_START_V + ((OSDDSPLY_VP_REGION_RESOLUTION_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_DATE:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_DATE_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_DATE_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_DATE_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_DATE_START_V + ((OSDDSPLY_VP_REGION_DATE_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_MOTION_FILE_NAME:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_MOTION_FILE_NAME_START_V + ((OSDDSPLY_VP_REGION_MOTION_FILE_NAME_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    case OSDDSPLY_REGION_VP_MOTION_FILE_NUM:
        if (bType == OSDDSPLY_UPDATE_INFO)
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_MOTION_FILE_NUM_INFO_COLOR;
        }
        else
        {
            _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_MOTION_FILE_NUM_ITEM_COLOR;
        }
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_MOTION_FILE_NUM_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_MOTION_FILE_NUM_START_V + ((OSDDSPLY_VP_REGION_MOTION_FILE_NUM_HEIGHT - _bOSDDSPLYTemp) >> 1);
       break;
    case OSDDSPLY_REGION_VP_MOTION_TIME:
        _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_VP_REGION_MOTION_TIME_COLOR;
        _OSDDSPLYStringAttr.wX = OSDDSPLY_VP_REGION_MOTION_TIME_START_H;
        _OSDDSPLYStringAttr.wY = OSDDSPLY_VP_REGION_MOTION_TIME_START_V + ((OSDDSPLY_VP_REGION_MOTION_TIME_HEIGHT - _bOSDDSPLYTemp) >> 1);
        break;
    default:
        break;
    }
    
    _OSDDSPLYStringAttr.wX += bPos;
    //GDI_DrawString_909(0, &_OSDDSPLYStringAttr, __pOSDUTLOutputString);
    _OSDDSPLY_DrawString();
}


//***************************************************************************
//  Function    :   _OSDDSPLY_VP_ShowTime
//  Abstract    :   This function will show time for virtual panel.
//  Arguments   :   bUnknownTime:   TRUE==>show unknown time.
//              :                   FALSE==>Show normal time.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_VP_ShowTime(void)
{
#ifndef SUPPORT_SIMPLE_DISPLAY_MODE

    if (gcShowStatus.bStill || (__bCountStop != 0))
    {
        __pOSDUTLOutputString = aDSPLYUnknownTime;
    }
    else
    {     
        OSDUTL_PrepareOutputString();
        OSDUTL_ShowHourMinSec();
    }

    _OSDDSPLY_VP_DrawRegion(OSDDSPLY_REGION_VP_TIME, OSDDSPLY_UPDATE_INFO, 0);

#endif //SUPPORT_SIMPLE_DISPLAY_MODE
}

//***************************************************************************
//  Function    :   _OSDDSPLY_VP_MOTION_ShowTime
//  Abstract    :   This function will show DivX time for virtual panel.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_VP_MOTION_ShowTime(void)
{
    if (__bCountStop != 0)
    {
        __pOSDUTLOutputString = aDSPLYUnknownTime;
    }
    else
    {     
        OSDUTL_PrepareOutputString();
        OSDUTL_ShowHourMinSec();
    }

    _OSDDSPLY_VP_DrawRegion(OSDDSPLY_REGION_VP_MOTION_TIME, OSDDSPLY_UPDATE_INFO, 0);        
}

//***************************************************************************
//  Function    :   _OSDDSPLY_VP_UpdateUnknownRegions
//  Abstract    :   This function will update the unknown status for all regions.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
void _OSDDSPLY_VP_UpdateUnknownRegions(void)
{
#ifdef SUPPORT_SIMPLE_DISPLAY_MODE
    _OSDDSPLY_VP_ShowInfo(MSG_SPST_CTL);    
    _OSDDSPLY_VP_ShowInfo(MSG_AST_CTL);  
    _OSDDSPLY_VP_ShowInfo(MSG_ANGLE_CTL);
#else
    _OSDDSPLY_VP_ShowTime();
    _OSDDSPLY_VP_ShowInfo(MSG_TITLE);
    _OSDDSPLY_VP_ShowInfo(MSG_CHAPTER);    
    _OSDDSPLY_VP_ShowInfo(MSG_SPST_CTL);    
    _OSDDSPLY_VP_ShowInfo(MSG_AST_CTL);  
    _OSDDSPLY_VP_ShowInfo(MSG_ANGLE_CTL);

    //update "repeat" and "program" status
    _OSDDSPLY_VP_ShowInfo(MSG_REPEAT);    
    _OSDDSPLY_VP_ShowInfo(MSG_PROGRAM);
    _OSDDSPLY_VP_ShowInfo(MSG_STATUS);
#endif //SUPPORT_SIMPLE_DISPLAY_MODE
}
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p

//***************************************************************************
//  Function    :   _OSDDSPLY_VP_ShowDVDMode
//  Abstract    :   This function will show DVD virtual panel.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p
void _OSDDSPLY_VP_ShowDVDMode(void)
{
#ifdef SUPPORT_SIMPLE_DISPLAY_MODE
    //Show "Subtitle" region.
    _OSDDSPLY_VP_ShowItem(MSG_SPST_CTL);
    _OSDDSPLY_VP_ShowInfo(MSG_SPST_CTL);
    
    //Show "AST" region.
    _OSDDSPLY_VP_ShowItem(MSG_AST_CTL);
    _OSDDSPLY_VP_ShowInfo(MSG_AST_CTL);

    //Show "AST" region.
    _OSDDSPLY_VP_ShowItem(MSG_ANGLE_CTL);
    _OSDDSPLY_VP_ShowInfo(MSG_ANGLE_CTL);
#else
    //Show "Title" region
    _OSDDSPLY_VP_ShowItem(MSG_TITLE);
    _OSDDSPLY_VP_ShowInfo(MSG_TITLE); 
    
    //Show "Chapter" region
    _OSDDSPLY_VP_ShowItem(MSG_CHAPTER);
    _OSDDSPLY_VP_ShowInfo(MSG_CHAPTER);    
  
    //Show "Time" regoin.
    _OSDDSPLY_VP_ShowTime(); //Show unknown time.
    
    //Show status.
    _OSDDSPLY_VP_ShowInfo(MSG_STATUS);
    _OSDDSPLY_VP_ShowInfo(MSG_REPEAT);
    _OSDDSPLY_VP_ShowInfo(MSG_PROGRAM);
    
    //Show "Subtitle" region.
    _OSDDSPLY_VP_ShowItem(MSG_SPST_CTL);
    _OSDDSPLY_VP_ShowInfo(MSG_SPST_CTL);
    
    //Show "AST" region.
    _OSDDSPLY_VP_ShowItem(MSG_AST_CTL);
    _OSDDSPLY_VP_ShowInfo(MSG_AST_CTL);
            
    //Show "TV" region.
    _OSDDSPLY_VP_ShowItem(MSG_TVMODE);
    _OSDDSPLY_VP_ShowInfo(MSG_TVMODE);
    
    //Show "Angle" region.
    _OSDDSPLY_VP_ShowItem(MSG_ANGLE_CTL);
    _OSDDSPLY_VP_ShowInfo(MSG_ANGLE_CTL);

#endif //SUPPORT_SIMPLE_DISPLAY_MODE
}
#endif //#ifndef NO_DVD_DISPLAY_UI //++CoCo2.37p

//***************************************************************************
//  Function    :   _OSDDSPLY_VP_ShowJPEGMode
//  Abstract    :   This function will show JPEG virtual panel.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_VP_ShowJPEGMode(void)
{
#ifdef SUPPORT_SIMPLE_DISPLAY_MODE
    //Show "Resolution" region.
    _OSDDSPLY_VP_ShowItem(MSG_JPEG_RESOLUTION);
    _OSDDSPLY_VP_ShowInfo(MSG_JPEG_RESOLUTION);
    
    //Show "Date" region.
    _OSDDSPLY_VP_ShowItem(MSG_JPEG_DATE);
    _OSDDSPLY_VP_ShowInfo(MSG_JPEG_DATE);
#else
    //Show "File name" region.
    _OSDDSPLY_VP_ShowItem(MSG_FILE_NAME);
    _OSDDSPLY_VP_ShowInfo(MSG_FILE_NAME);
    
    //Show "File num" region.
    _OSDDSPLY_VP_ShowItem(MSG_FILE_NUM);
    _OSDDSPLY_VP_ShowInfo(MSG_FILE_NUM);
    
    //Show "Resolution" region.
    _OSDDSPLY_VP_ShowItem(MSG_JPEG_RESOLUTION);
    _OSDDSPLY_VP_ShowInfo(MSG_JPEG_RESOLUTION);
    
    //Show "Date" region.
    _OSDDSPLY_VP_ShowItem(MSG_JPEG_DATE);
    _OSDDSPLY_VP_ShowInfo(MSG_JPEG_DATE);

    //Show status.
    _OSDDSPLY_VP_ShowInfo(MSG_STATUS);
    _OSDDSPLY_VP_ShowInfo(MSG_REPEAT);
    _OSDDSPLY_VP_ShowInfo(MSG_PROGRAM);

#endif //SUPPORT_SIMPLE_DISPLAY_MODE
}

//***************************************************************************
//  Function    :   _OSDDSPLY_VP_ShowDIVXMode
//  Abstract    :   This function will show DivX virtual panel.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_VP_ShowDIVXMode(void)
{
#ifdef SUPPORT_SIMPLE_DISPLAY_MODE
    //Show "Subtitle" region.
    _OSDDSPLY_VP_ShowItem(MSG_SPST_CTL);
    _OSDDSPLY_VP_ShowInfo(MSG_SPST_CTL);
    
    //Show "AST" region.
    _OSDDSPLY_VP_ShowItem(MSG_AST_CTL);
    _OSDDSPLY_VP_ShowInfo(MSG_AST_CTL);
#else
    //Show "File name" region.
    _OSDDSPLY_VP_ShowItem(MSG_FILE_NAME);
    _OSDDSPLY_VP_ShowInfo(MSG_FILE_NAME);
    
    //Show "File num" region.
    _OSDDSPLY_VP_ShowItem(MSG_FILE_NUM);
    _OSDDSPLY_VP_ShowInfo(MSG_FILE_NUM);    
         
    //Show status.
    _OSDDSPLY_VP_ShowInfo(MSG_STATUS);
    _OSDDSPLY_VP_ShowInfo(MSG_REPEAT);
    _OSDDSPLY_VP_ShowInfo(MSG_PROGRAM);

    //Show time in the virtual panel?
    _OSDDSPLY_VP_MOTION_ShowTime();

    //Show "Subtitle" region.
    _OSDDSPLY_VP_ShowItem(MSG_SPST_CTL);
    _OSDDSPLY_VP_ShowInfo(MSG_SPST_CTL);
    
    //Show "AST" region.
    _OSDDSPLY_VP_ShowItem(MSG_AST_CTL);
    _OSDDSPLY_VP_ShowInfo(MSG_AST_CTL);

    //Show "TV" region.
    _OSDDSPLY_VP_ShowItem(MSG_TVMODE);
    _OSDDSPLY_VP_ShowInfo(MSG_TVMODE);

#endif //SUPPORT_SIMPLE_DISPLAY_MODE
}

//***************************************************************************
//  Function    :   OSDDSPLY_Exit
//  Abstract    :   This function will be called by OSD kernel to exit the UI.
//  Arguments   :   bExitMode: OSD_UI_EXIT_ALL/OSD_UI_EXIT_CONTROL_ONLY
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSDDSPLY_Exit(BYTE bExitMode)
{
    if (bExitMode == OSD_UI_EXIT_ALL)
    {   
        _OSDDSPLY_ClearDisplayModeUI();        
    }

    //Reset the control variables
    _bOSDDSPLYMode = OSDDSPLY_MODE_NONE;

    _bOSDDSPLYTitleMenuMode = FALSE;
    _bOSDDSPLYEnterTimeMode = FALSE;
	_bOSDDSPLYLDTitleMenuMode = FALSE;
}

#ifndef NO_DISC_MODE //CoCo2.37
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p

//*********************************************************************************************************
//  Function    :   OSDDSPLY_ProcessKey 
//  Abstract    :   Call this function to process the keys.
//  Arguments   :   none.
//  Return		:   KEY_NO_KEY: The input key is processed.
//              :   INVALID_KEY: The input key is not allowed.
//              :   KEY_xxx: The main program need do more process for the key.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
BYTE OSDDSPLY_ProcessKey(void)
{
#ifdef SUPPORT_INDEPENDENT_NAVIGATOR //CoCo1.0, Navigator UI
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
    if ((__bKey == KEY_NAVIGATOR) && (__wDiscType & BOOK_DVD))
    {
        if (!_bNavigatorUI)
        {
            OSD_ChangeUI(OSD_UI_NAVIGATOR, OSD_ENTER_UI);
            _bNavigatorUI = TRUE;
            
            //Can't clear all region because it may clear the normal upper/bottom region.
            //Should reserve the normal upper/bottom region.
            _OSDDSPLYRect.rect.wLeft = 0;
            _OSDDSPLYRect.rect.wTop = OSDND_UPPER_BACKGROUND_BOTTOM+1;
            _OSDDSPLYRect.rect.wRight = GDI_REGION_WIDTH;
            _OSDDSPLYRect.rect.wBottom = OSDND_BOTTOM_BACKGROUND_START_V-1;
            _OSDDSPLYRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;     
            GDI_FillRect_909(0, &_OSDDSPLYRect);

            _OSDDSPLY_NAVIGATOR_ShowNavigator();
        }
        else
        {
            _OSDDSPLY_ClearBackground(OSDDSPLY_BACKGROUND_NAVIGATOR);
            OSD_ChangeUI(OSD_UI_NAVIGATOR, OSD_EXIT_UI);
            _bNavigatorUI = FALSE;
        }

        return KEY_NO_KEY;
    }
#endif //#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p

    if (!_bNavigatorUI)
        return KEY_BYPASS; //let CC process the key.

    if (__bKey == KEY_DISPLAY)
        return INVALID_KEY;

#else
    if (pOSDDSPTypeMode[_bOSDDSPLYMode].bDisplayModeID != OSDDSPLY_MODE_DVD_NAVIGATOR)
        return KEY_BYPASS; //let CC process the key.

    if (__bKey == KEY_DISPLAY)
    {
        _OSDDSPLY_SwitchDisplayModeUI();
        return KEY_NO_KEY;
    }
#endif //SUPPORT_INDEPENDENT_NAVIGATOR

    /*
    if (__bZoomFactor != VIEW_NORMAL) //CoCo0.90
    {
        if ((__bKey == KEY_UP) || (__bKey == KEY_DOWN) || (__bKey == KEY_LEFT) || (__bKey == KEY_RIGHT)) 
            return KEY_BYPASS; //let CC process the key.
    }
    */
    
#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p
    if (_bOSDDSPLYCursorPos == OSDDSPLY_NAVIGATOR_CURSOR_POS_MENU)
    {
        switch (__bKey)
        {
        case KEY_UP: //move the cusor to the up item.
            if (!_OSDDSPLY_NAVIGATOR_MoveCursorToPreviousMenuItem())
                return INVALID_KEY;
            break;
        case KEY_DOWN: //move the cusor to the down item.
            if (!_OSDDSPLY_NAVIGATOR_MoveCursorToNextMenuItem())
                return INVALID_KEY;
            break;
        case KEY_PLAY: //Select the item
        case KEY_ENTER:
            //Need to delay some time to let user see the result?

            //Change the setting
            if (!_OSDDSPLY_NAVIGATOR_ChangeSetting())
                return INVALID_KEY;
            else
            {
                _OSDDSPLY_NAVIGATOR_ClearTip();
                
                //Check the item. (add "checked" symbol)
                _OSDDSPLY_NAVIGATOR_DrawMenuItem((BYTE)(_wOSDDSPLYCursorMenuIndex - _wOSDDSPLYStartDrawItem), TRUE);
                
                //Clear the menu.
                _OSDDSPLY_NAVIGATOR_ClearMenu();
                
                //Set the cursor's position to the icon part.
                _bOSDDSPLYCursorPos = OSDDSPLY_NAVIGATOR_CURSOR_POS_ICON;
                
                //Show "tip" to show the result
                _OSDDSPLY_NAVIGATOR_ShowTip();
                
                if( OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID == OSDDSPLY_NAVIGATOR_ICON_TITLE 
                    || OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID == OSDDSPLY_NAVIGATOR_ICON_CHAPTER )
                {
                    if (__bModeCmd != KEY_PLAY)
                    {
                        CC_DoKeyAction (KEY_PLAY);
                    }
                    __bKey = KEY_SEARCH_PLAY;
                    return KEY_TODO;               
                }
            }
            break;
        case OSDDSPLY_NAVIGATOR_CANCEL_MENU_ACTION:
            //clear the menu and main frame.            
            _OSDDSPLY_NAVIGATOR_ClearMenu();
            _bOSDDSPLYCursorPos = OSDDSPLY_NAVIGATOR_CURSOR_POS_ICON;
            break;
        case KEY_AST_CTL:
            if (!_OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForMenu(OSDDSPLY_NAVIGATOR_ICON_AUDIO))
                return INVALID_KEY;
            break;
        case KEY_SPST_CTL:
            if (!_OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForMenu(OSDDSPLY_NAVIGATOR_ICON_SPST1))
                return INVALID_KEY;
            break;
        case KEY_ANGLE_CTL:
            if (!_OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForMenu(OSDDSPLY_NAVIGATOR_ICON_ANGLE))
                return INVALID_KEY;
            break;
        case KEY_REPEAT:
            if (!_OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForMenu(OSDDSPLY_NAVIGATOR_ICON_REPEAT))
                return INVALID_KEY;
            break;
        default:
            if (__bZoomFactor != VIEW_NORMAL) //CoCo1.0
            {
                if ((__bKey == KEY_UP) || (__bKey == KEY_DOWN) || (__bKey == KEY_LEFT) || (__bKey == KEY_RIGHT)) 
                    return INVALID_KEY;
            }

            return KEY_BYPASS; //let CC process the key.
        }
    }
    else if (_bOSDDSPLYCursorPos == OSDDSPLY_NAVIGATOR_CURSOR_POS_ICON)
    {           
        if (__bKey == OSDDSPLY_NAVIGATOR_NEXT_ACTION)
        {
            //change the cursor to the next icon.
            if (_bOSDDSPLYCursorIconIndex < (OSDDSPLY_NAVIGATOR_TOTAL_ICONS - 1))
            {
                _OSDDSPLY_NAVIGATOR_ClearIconHighlight();
                
                //Update the index of the cursor.
                _bOSDDSPLYCursorIconIndex++;
                
                _OSDDSPLY_NAVIGATOR_ShowIconHighlight();
            }
            else
            {
#ifdef OSDDSPLY_NAVIGATOR_SUPPORT_ROUND_HIGHLIGHT
                _OSDDSPLY_NAVIGATOR_ClearIconHighlight();
                _bOSDDSPLYCursorIconIndex = 0;
                _OSDDSPLY_NAVIGATOR_ShowIconHighlight();                
#else
                return INVALID_KEY;
#endif //OSDDSPLY_NAVIGATOR_SUPPORT_ROUND_HIGHLIGHT
            }

        }
        else if (__bKey == OSDDSPLY_NAVIGATOR_PREVIOUS_ACTION)
        {
            //change the cursor to the previous icon.
            if (_bOSDDSPLYCursorIconIndex > 0)
            {
                _OSDDSPLY_NAVIGATOR_ClearIconHighlight();
                _bOSDDSPLYCursorIconIndex--;
                _OSDDSPLY_NAVIGATOR_ShowIconHighlight();
            }
            else
            {
#ifdef OSDDSPLY_NAVIGATOR_SUPPORT_ROUND_HIGHLIGHT
                _OSDDSPLY_NAVIGATOR_ClearIconHighlight();
                _bOSDDSPLYCursorIconIndex = OSDDSPLY_NAVIGATOR_TOTAL_ICONS - 1;
                _OSDDSPLY_NAVIGATOR_ShowIconHighlight();
#else
                return INVALID_KEY;
#endif //OSDDSPLY_NAVIGATOR_SUPPORT_ROUND_HIGHLIGHT
            }
        }
        else 
        {
            if (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconAttr == OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_POPUP_MENU)
            {
                if (_OSDDSPLY_NAVIGATOR_PopupMenuKey(__bKey))
                {
                    //check if the menu can be pop-up or not.
                    if (_OSDDSPLY_NAVIGATOR_CheckValidPopupMenu())
                    {
                        //Clear the "show tip" because we may pop-up the menu before the "tip" is disappeared.
                        _OSDDSPLY_NAVIGATOR_ClearTip();
                        _bOSDDSPLYTipDisplayed = FALSE;
                        
                        _bOSDDSPLYCursorPos = OSDDSPLY_NAVIGATOR_CURSOR_POS_MENU;
                        _OSDDSPLY_NAVIGATOR_PopupMenu();
                    }
                    else
                        return INVALID_KEY;
                }
                else //other special keys like KEY_AST_CTL, KEY_SPST_CTL, KEY_ANGLE_CTL, KEY_REPEAT
                {
                    switch (__bKey)
                    {
                    case KEY_AST_CTL:
                        return _OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForIcon(OSDDSPLY_NAVIGATOR_ICON_AUDIO);
                    case KEY_SPST_CTL:
                        return _OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForIcon(OSDDSPLY_NAVIGATOR_ICON_SPST1);
                    case KEY_ANGLE_CTL:
                        return _OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForIcon(OSDDSPLY_NAVIGATOR_ICON_ANGLE);
                    case KEY_REPEAT:
                        return _OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForIcon(OSDDSPLY_NAVIGATOR_ICON_REPEAT);
                        //Need to process the number keys for title/chapter/angle
                    default:
                        if (__bZoomFactor != VIEW_NORMAL) //CoCo1.0
                        {
                            if ((__bKey == KEY_UP) || (__bKey == KEY_DOWN) || (__bKey == KEY_LEFT) || (__bKey == KEY_RIGHT)) 
                                return INVALID_KEY;
                        }
                        return KEY_BYPASS; //let CC process the key.
                    }
                }
            }
            else if (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconAttr == OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_BUTTON)
            {
                if (__bZoomFactor != VIEW_NORMAL) //CoCo1.0
                {
                    if ((__bKey == KEY_UP) || (__bKey == KEY_DOWN) || (__bKey == KEY_LEFT) || (__bKey == KEY_RIGHT)) 
                        return INVALID_KEY;
                }

                if ((__bKey == KEY_PLAY) || (__bKey == KEY_ENTER))
                {
                    //Do the action
                    switch (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID)
                    {
                    case OSDDSPLY_NAVIGATOR_ICON_REPEATAB:
                        __bKey = KEY_REPEAT_AB;
                        break;
                    case OSDDSPLY_NAVIGATOR_ICON_SLOW:
                        // wyc2.17-909S, issue different key when different language study mode.
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
                        __bKey = KEY_LAN_STUDY;
#else
                        __bKey = KEY_SF;
#endif //
                        
                        break;
                    default:
                        return KEY_BYPASS; //let CC process the key.
                    }

                    _OSDDSPLY_NAVIGATOR_ProcessButton();

                    return KEY_BYPASS; //KEY_TODO;  //kevin1.10, fix NV SLOW 75%->50%->75%(should be Play)
                }
                else
                {
                    switch (__bKey)
                    {
                    case KEY_REPEAT_AB:
                        // wyc2.17-909S, receive different key when different language study mode.
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_PLUS_LANGUAGE_STUDY)
                    case KEY_LAN_STUDY:
#else
                    case KEY_SF:
#endif //
                        _OSDDSPLY_NAVIGATOR_ProcessButton();
                    default:
                        return KEY_BYPASS; //let CC process the key.
                    }
                }
            }
        }
    }

#endif //#ifdef SUPPORT_NAVIGATOR //++CoCo2.37p

    return KEY_NO_KEY;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_PopupMenuKey
//  Abstract    :   This function will check if the key can pop-up the menu or not.
//  Arguments   :   bKey: the input key
//  Return      :   TRUE==>The key will pop-up the menu.
//              :   FALSE==>The key will not pop-up menu.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_NAVIGATOR_PopupMenuKey(BYTE bKey)
{
    for (_bOSDDSPLYTemp = 0; _bOSDDSPLYTemp < sizeof(OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bPopMenuAction); _bOSDDSPLYTemp++)
    {
        if (bKey == OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bPopMenuAction[_bOSDDSPLYTemp])
            return TRUE;
    }
    
    return FALSE;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_InitializeMenuAttribute
//  Abstract    :   This function will initialize the menu attributes.  
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_InitializeMenuAttribute(void)
{
    //The vertical starting position to draw the menu.
#ifdef OSDDSPLY_NAVIGATOR_SUBMENU_START_FROM_ICON_TOP
    _wOSDDSPLYTemp = OSDDSPLY_NAVIGATOR_MENU_END_V -  OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV-_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex);
#else
    _wOSDDSPLYTemp = OSDDSPLY_NAVIGATOR_MENU_END_V -  OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV-_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) - OSDDSPLY_NAVIGATOR_ICON_HEIGHT;
#endif

    _bOSDDSPLYTemp = _wOSDDSPLYTemp / OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT; //The menu items can be drawed for this item.

    //Initialize the max. items and _bOSDDSPLYMenuHasDownArrow
    _wOSDDSPLYMaxItem = _wOSDDSPLYTotalSetting;
    _bOSDDSPLYMenuHasDownArrow = FALSE;

    if (_wOSDDSPLYTotalSetting > _bOSDDSPLYTemp) //all items can't be drawed at the same time. Need to tune the position.
    {
        if (_wOSDDSPLYTotalSetting > OSDDSPLY_NAVIGATOR_MAX_ITEM)
        {
            //should move the starting position to the top menu position.
            _bOSDDSPLYMenuStartV = OSDDSPLY_NAVIGATOR_MENU_START_V;
            _wOSDDSPLYMaxItem = OSDDSPLY_NAVIGATOR_MAX_ITEM - 1; //Reserve the space for down arrow.

            if (_wOSDDSPLYCurrentSettingIndex != (_wOSDDSPLYTotalSetting-1))
                _bOSDDSPLYMenuHasDownArrow = TRUE;
        }
        else
        {
            //calculate the position that we start to draw the menu.
            _bOSDDSPLYMenuStartV = OSDDSPLY_NAVIGATOR_MENU_END_V - OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT*_wOSDDSPLYTotalSetting;
         }
    }
    else
    {
        _bOSDDSPLYMenuStartV = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex);
    }

    //Check if "menu's starting position+menu's height" exceeds the end of the menu.
    if ((_bOSDDSPLYMenuStartV+OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT*_wOSDDSPLYMaxItem) > OSDDSPLY_NAVIGATOR_MENU_END_V)
    {
        _bOSDDSPLYMenuStartV = OSDDSPLY_NAVIGATOR_MENU_END_V - (OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT*_wOSDDSPLYTotalSetting);
    }
    
    //Set the starting draw item.
    if (_wOSDDSPLYCurrentSettingIndex > (_wOSDDSPLYMaxItem-1))
    {
        //_wOSDDSPLYStartDrawItem = (_wOSDDSPLYCurrentSettingIndex / _wOSDDSPLYMaxItem) * _wOSDDSPLYMaxItem;
        _wOSDDSPLYStartDrawItem =  _wOSDDSPLYCurrentSettingIndex - _wOSDDSPLYMaxItem + 1;
        _bOSDDSPLYMenuHasUpArrow = TRUE;
    }
    else
    {
        _wOSDDSPLYStartDrawItem = 0;
        _bOSDDSPLYMenuHasUpArrow = FALSE;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ClearItemRegion
//  Abstract    :   Call this function to clear the menu item's region.
//  Arguments   :   bIndex: The index of the item.
//              :   bHighlight: TRUE: clear the item's background to highlihgt background.
//              :               FALSE: clear the item's background to normal background.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ClearItemRegion(BYTE bIndex, BYTE bHighlight)
{
    _OSDDSPLYRect.rect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH;
    _OSDDSPLYRect.rect.wRight = _OSDDSPLYRect.rect.wLeft + _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu() + (OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1);
    _OSDDSPLYRect.rect.wTop = _bOSDDSPLYMenuStartV+bIndex*OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT;
    _OSDDSPLYRect.rect.wBottom = _OSDDSPLYRect.rect.wTop+OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT-1;
    
    if (bHighlight)
    {
        _OSDDSPLYRect.bColor = OSDDSPLY_NG_PALETTE_ENTRY_MENU_HIGHLIGHT;        
    }
    else
    {
        _OSDDSPLYRect.bColor = OSDDSPLY_NG_PALETTE_ENTRY_GENERAL_BACKGROUND;
    }
    
    GDI_FillRect_909(0, &_OSDDSPLYRect);
}


//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_DrawMenu
//  Abstract    :   Call this function to draw the menu.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_DrawMenu(void)
{
    _OSDDSPLYRect.rect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH;
    _OSDDSPLYRect.rect.wRight = _OSDDSPLYRect.rect.wLeft + _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu() + (OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1);
    _OSDDSPLYRect.rect.wTop = _bOSDDSPLYMenuStartV;
    _OSDDSPLYRect.rect.wBottom = _OSDDSPLYRect.rect.wTop + _wOSDDSPLYMaxItem*OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT;
    
    if (_bOSDDSPLYMenuHasUpArrow)
    {
        _OSDDSPLYRect.rect.wTop -= OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT;
    }
    
    if (_bOSDDSPLYMenuHasDownArrow)
    {
        _OSDDSPLYRect.rect.wBottom += OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT;
    }
   
    _OSDDSPLYRect.bColor = OSDDSPLY_NG_PALETTE_ENTRY_GENERAL_BACKGROUND;
    
    GDI_FillRect_909(0, &_OSDDSPLYRect);       
    
    //Draw main frame.
    _OSDDSPLY_NAVIGATOR_DrawMainFrame();
     
    //Draw the separate frame
    if (_bOSDDSPLYMenuHasUpArrow)
        _OSDDSPLY_NAVIGATOR_DrawSeparateFrame(OSDDSPLY_NAVIGATOR_UP_SEPARATE_FRAME);
    
    if (_bOSDDSPLYMenuHasDownArrow)
        _OSDDSPLY_NAVIGATOR_DrawSeparateFrame(OSDDSPLY_NAVIGATOR_DOWN_SEPARATE_FRAME);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_DrawSeparateFrame
//  Abstract    :   This function will draw the separate frame.
//  Arguments   :   bUpFrame: OSDDSPLY_NAVIGATOR_UP_SEPARATE_FRAME/OSDDSPLY_NAVIGATOR_DOWN_SEPARATE_FRAME.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_DrawSeparateFrame(BYTE bFrameType)
{
    _OSDDSPLYFrame.bColorBright = OSDDSPLY_NG_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
    _OSDDSPLYFrame.bColorDark = OSDDSPLY_NG_PALETTE_ENTRY_NORMAL_FRAME_DARK;    
    _OSDDSPLYFrame.bColorCenter = OSDDSPLY_NG_PALETTE_ENTRY_SEPARATE_FRAME_CENTER; //"FRAME_DOUBLE" doesn't need to assign the center color?
    _OSDDSPLYFrame.bStyle = FRAME_DOUBLE_SOLID;
    _OSDDSPLYFrame.bThickness = OSDDSPLY_NAVIGATOR_SEPARATE_FRAME_THICKNESS >> 1;
    _OSDDSPLYFrame.rect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH;
    _OSDDSPLYFrame.rect.wRight = _OSDDSPLYFrame.rect.wLeft + _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu() + (OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1);
    
    if (bFrameType == OSDDSPLY_NAVIGATOR_UP_SEPARATE_FRAME)
    {
        _OSDDSPLYFrame.rect.wTop = _bOSDDSPLYMenuStartV-OSDDSPLY_NAVIGATOR_SEPARATE_FRAME_THICKNESS;
    }
    else if (bFrameType == OSDDSPLY_NAVIGATOR_DOWN_SEPARATE_FRAME)
    {
        _OSDDSPLYFrame.rect.wTop = _OSDDSPLYRect.rect.wBottom-OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT;
    }
    else
        return;

    _OSDDSPLYFrame.rect.wBottom = _OSDDSPLYFrame.rect.wTop+OSDDSPLY_NAVIGATOR_SEPARATE_FRAME_THICKNESS-1;
    
    GDI_DrawFrame_909(0, &_OSDDSPLYFrame);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu
//  Abstract    :   Call this function to get the maximum item's width in the menu.
//  Arguments   :   none.
//  Return      :   The max. width.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
WORD _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu(void)
{
    switch (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID)
    {
    case OSDDSPLY_NAVIGATOR_ICON_TITLE:
        _OSDDSPLY_NAVIGATOR_CalculateMaxNumWidth(__bTitle_Ns);
        break;
    case OSDDSPLY_NAVIGATOR_ICON_CHAPTER:
        _OSDDSPLY_NAVIGATOR_CalculateMaxNumWidth(__wChapter_Ns_Title);
        break;
    case OSDDSPLY_NAVIGATOR_ICON_AUDIO:
        _OSDDSPLY_NAVIGATOR_CalculateMaxAudioMenuWidth();
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST1:
        _OSDDSPLY_NAVIGATOR_CalculateMaxSPST1MenuWidth();
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST2:
        _OSDDSPLY_NAVIGATOR_CalculateMaxSPST1MenuWidth();
        break;
    case OSDDSPLY_NAVIGATOR_ICON_ANGLE:
        _OSDDSPLY_NAVIGATOR_CalculateMaxNumWidth(__bAGL_Ns);
        break;
    case OSDDSPLY_NAVIGATOR_ICON_REPEAT:
        _OSDDSPLY_NAVIGATOR_CalculateMaxRepeatWidth();
        break;
    default:
        break;
    }

    _wOSDDSPLYMaxWidth += GDI_GetStringWidth_909(aDSPLYCheckSymbol);
    _wOSDDSPLYMaxWidth += (OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1);

    return _wOSDDSPLYMaxWidth;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_CalculateMaxNumWidth
//  Abstract    :   This function will calculate the max. width for the menu only
//              :   has numbers.
//  Arguments   :   wMaxNum: The max. number in the menu.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_CalculateMaxNumWidth(WORD wMaxNum)
{
    OSDUTL_PrepareOutputString();

    _wOSDDSPLYMaxWidth = 0;

    for (_wOSDDSPLYTemp = 0; _wOSDDSPLYTemp < wMaxNum; _wOSDDSPLYTemp++)
    {
        __bOSDUTLIndex = 1;

        OSDUTL_OutputNumFromIndex((WORD)(_wOSDDSPLYTemp+1));

        _wOSDDSPLYWidth = GDI_GetStringWidth_909(__pOSDUTLOutputString);

        if (_wOSDDSPLYMaxWidth < _wOSDDSPLYWidth)
        {
            _wOSDDSPLYMaxWidth = _wOSDDSPLYWidth;
        }
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_CalculateMaxRepeatWidth
//  Abstract    :   This function will calculate the max. width for the repeat menu.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_CalculateMaxRepeatWidth(void)
{
    _wOSDDSPLYMaxWidth = 0;

    for (_bOSDDSPLYTemp = 0; _bOSDDSPLYTemp < OSDDSPLY_NAVIGATOR_MENU_TOTAL_ITEM; _bOSDDSPLYTemp++)
    {
        OSDUTL_PrepareOutputString();
        _OSDDSPLY_NAVIGATOR_GetRepeatItem(_bOSDDSPLYTemp);
        _wOSDDSPLYWidth = GDI_GetStringWidth_909(__pOSDUTLOutputString);

        if (_wOSDDSPLYMaxWidth < _wOSDDSPLYWidth)
            _wOSDDSPLYMaxWidth = _wOSDDSPLYWidth;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_GetRepeatItem
//  Abstract    :   This function will get the repeat item.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_GetRepeatItem(BYTE bIndex)
{        
    if ( __bDomain != TT_DOM )
    {
        OSDUTL_CopyStringFromIndex(aDSPLYRepeatOff);
    }
    else
    {
        if (__bNVPlayMode)
        {
            switch (bIndex)
            {
            case 0:
                OSDUTL_CopyStringFromIndex(aDSPLYRepeatTitle);
                break;
            case 1:
                OSDUTL_CopyStringFromIndex(aDSPLYRepeatChapter);
                break;
            case 2:
                OSDUTL_CopyStringFromIndex(aDSPLYRepeatOff);
                break;
            default:
                break;
            }
        }
        else //PBC off
        {
            switch (bIndex)
            {
            case 0:
                OSDUTL_CopyStringFromIndex(aDSPLYRepeatTitle);
                break;
            case 1:
                OSDUTL_CopyStringFromIndex(aDSPLYRepeatChapter);
                break;
            case 2:
                OSDUTL_CopyStringFromIndex(aDSPLYRepeatDisc);
                break;
            case 3:            
                OSDUTL_CopyStringFromIndex(aDSPLYRepeatOff);
                break;
            default:
                break;
            }
        }
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_TransformRepeatItem
//  Abstract    :   This function will ransform the __bRepeat to the repeat item.
//  Arguments   :   none.
//  Return      :   The repeat item.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_NAVIGATOR_TransformRepeatItem(void)
{
    if ( __bDomain != TT_DOM )
    {
        return 0;
    }
    else
    {
        if (__bNVPlayMode)
        {
            switch (__bRepeat)
            {
            case REPEAT_TITLE:
                return 0;
            case REPEAT_CHAPTER:
                return 1;
            case REPEAT_NONE:
            default:
                return 2;
            }
        }
        else //Naviage off
        {
            switch (__bRepeat)
            {
            case REPEAT_TITLE:
                return 0;
            case REPEAT_CHAPTER:
                return 1;
            case REPEAT_DISC:
                return 2;
            case REPEAT_NONE:
            default:
                return 3;
            }
        }
    }

    return OSDDSPLY_NAVIGATOR_MENU_REPEAT_OFF;
}


//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_PrepareMenuInfo
//  Abstract    :   This function will initialize the _wOSDDSPLYCurrentSettingIndex,
//              :   _wOSDDSPLYTotalSetting and _wOSDDSPLYCursorMenuIndex.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_PrepareMenuInfo(void)
{
    switch (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID)
    {
    case OSDDSPLY_NAVIGATOR_ICON_TITLE:
        _wOSDDSPLYCurrentSettingIndex = __bTitle - 1;
        _wOSDDSPLYTotalSetting = __bTitle_Ns; 
        break;
    case OSDDSPLY_NAVIGATOR_ICON_CHAPTER:
        _wOSDDSPLYCurrentSettingIndex = __wChapter - 1;
        _wOSDDSPLYTotalSetting = __wChapter_Ns_Title;
        break;
    case OSDDSPLY_NAVIGATOR_ICON_AUDIO:
        _wOSDDSPLYCurrentSettingIndex = __bASTNO;
        _wOSDDSPLYTotalSetting = __bASTNs;
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST1:
        //CoCo0.95
        // spst is on, ie. __bSP_Disp_OFF is 0 or 128 ~255
        if ((__bSP_Disp_OFF==0) || (__bSP_Disp_OFF&0x80))
        {
            _wOSDDSPLYCurrentSettingIndex = __bSPSTNO;
        }
        else
            _wOSDDSPLYCurrentSettingIndex = __bSPSTNs;

        _wOSDDSPLYTotalSetting = __bSPSTNs+1; //Add SPST "Off"
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST2:
        //CoCo0.95, judge SPST2 off here because NV not set __bSPST2NO to the last one like SPST1
        // spst is on, ie. __bSP_Disp_OFF is 0 or 128 ~255
        if ((__bSP2_Disp_OFF==0) || (__bSP2_Disp_OFF&0x80))
        {
            _wOSDDSPLYCurrentSettingIndex = __bSPST2NO;
        }
        else
            _wOSDDSPLYCurrentSettingIndex = __bSPSTNs; //Off

        _wOSDDSPLYTotalSetting = __bSPSTNs+1; //Add SPST "Off"
        break;
    case OSDDSPLY_NAVIGATOR_ICON_ANGLE:
        _wOSDDSPLYCurrentSettingIndex = __bAGLN - 1;
        _wOSDDSPLYTotalSetting = __bAGL_Ns;
        break;
    case OSDDSPLY_NAVIGATOR_ICON_REPEAT:
        _wOSDDSPLYCurrentSettingIndex = _OSDDSPLY_NAVIGATOR_TransformRepeatItem();
        
        if ( __bDomain != TT_DOM )
        {
            _wOSDDSPLYTotalSetting = 1;
        }
        else
        {
            if (__bNVPlayMode)
            {
                _wOSDDSPLYTotalSetting = 3;
            }
            else
            {
                _wOSDDSPLYTotalSetting = 4;
            }
        }
        break;
    default:
        break;
    }

    _wOSDDSPLYCursorMenuIndex = _wOSDDSPLYCurrentSettingIndex;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_SetCommonItemStringAttribute
//  Abstract    :   This function will set the common item string's attribute.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_SetCommonItemStringAttribute(void)
{
    _OSDDSPLYStringAttr.bBackground = PAL_ENTRY_COLOR_TRANSPARENT; //Set the color of background is the same as color key.
    _OSDDSPLYStringAttr.bColorKey = 0;
    _OSDDSPLYStringAttr.bShadow1 = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDDSPLYStringAttr.bShadow2 = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDDSPLYStringAttr.bTextColor = OSDDSPLY_NG_PALETTE_ENTRY_MENU_ITEM_TEXT;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_DrawAllItems
//  Abstract    :   This function will draw all items in the menu.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_DrawAllItems(void)
{
    BYTE bIndex = 0; 

    if (_bOSDDSPLYMenuHasUpArrow)
    {
        _OSDDSPLY_NAVIGATOR_UpdateUpDownArraow(OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP);
    }

    for (bIndex = 0; bIndex < _wOSDDSPLYMaxItem; bIndex++)
    {
        if (_wOSDDSPLYCurrentSettingIndex == (_wOSDDSPLYStartDrawItem+bIndex))
        {
            _OSDDSPLY_NAVIGATOR_ClearItemRegion(bIndex, TRUE);
            _OSDDSPLY_NAVIGATOR_DrawMenuItem(bIndex, TRUE);
        }
        else
        {
            _OSDDSPLY_NAVIGATOR_DrawMenuItem(bIndex, FALSE);
        }
    }

    if (_bOSDDSPLYMenuHasDownArrow)
    {
        _OSDDSPLY_NAVIGATOR_UpdateUpDownArraow(OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN);
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_UpdateUpDownArraow
//  Abstract    :   This function will draw the up/down arrow in the menu.
//  Arguments   :   bArrowType: OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP/OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_UpdateUpDownArraow(BYTE bArrowType)
{
    _OSDDSPLY_NAVIGATOR_SetCommonItemStringAttribute();
    _OSDDSPLYStringAttr.bBackground = OSDDSPLY_NG_PALETTE_ENTRY_GENERAL_BACKGROUND;

    //Calculate the max. width of the menu before __pOSDUTLOutputString is changed.
    _wOSDDSPLYTemp = _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu();

    if (bArrowType == OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP)
    {
        //Output the "up arraow"
        __pOSDUTLOutputString = aDSPLYUpArrowSymbol;
    }
    else if (bArrowType == OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN)
    {
        //Output the "down arraow"
        __pOSDUTLOutputString = aDSPLYDownArrowSymbol;
    }


    //Calculate the vertical distance to put the arrow to the the center of the menu item. 
    _bOSDDSPLYTemp = (OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT- OSDDSPLY_NAVIGATOR_SEPARATE_FRAME_THICKNESS- GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1;

    if (bArrowType == OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP)
    {
        //Output the "up arraow"
        _OSDDSPLYStringAttr.wY = _bOSDDSPLYMenuStartV - OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT + _bOSDDSPLYTemp;
    }
    else if (bArrowType == OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN)
    {
        //Output the "down arraow"
        _OSDDSPLYStringAttr.wY = _bOSDDSPLYMenuStartV + _wOSDDSPLYMaxItem * OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT+ OSDDSPLY_NAVIGATOR_SEPARATE_FRAME_THICKNESS + _bOSDDSPLYTemp;
    }

    _OSDDSPLYStringAttr.wX = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH+(_wOSDDSPLYTemp >> 1);

    //GDI_DrawString_909(0, &_OSDDSPLYStringAttr, __pOSDUTLOutputString);
    _OSDDSPLY_DrawString();
}


//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_Scroll
//  Abstract    :   Call this function to scroll the menu.
//  Arguments   :   bScrollType: OSDDSPLY_NAVIGATOR_SCROLL_TYPE_UP/OSDDSPLY_NAVIGATOR_SCROLL_TYPE_DOWN.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_Scroll(BYTE bScrollType)
{
    _OSDDSPLYBitmapCopy.bDestRegion = 0;
    _OSDDSPLYBitmapCopy.bSrcRegion = 0;
    _OSDDSPLYBitmapCopy.SrcRect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH;
    _OSDDSPLYBitmapCopy.SrcRect.wRight = _OSDDSPLYBitmapCopy.SrcRect.wLeft + _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu() -1;
    _OSDDSPLYBitmapCopy.wDestX = _OSDDSPLYBitmapCopy.SrcRect.wLeft;

    if (bScrollType == OSDDSPLY_NAVIGATOR_SCROLL_TYPE_UP)
    {
        _OSDDSPLYBitmapCopy.SrcRect.wTop = _bOSDDSPLYMenuStartV;
        _OSDDSPLYBitmapCopy.wDestY = _bOSDDSPLYMenuStartV + OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT;
    }
    else if (bScrollType == OSDDSPLY_NAVIGATOR_SCROLL_TYPE_DOWN)
    {
        _OSDDSPLYBitmapCopy.SrcRect.wTop = _bOSDDSPLYMenuStartV + OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT;
        _OSDDSPLYBitmapCopy.wDestY = _bOSDDSPLYMenuStartV;
    }

    _OSDDSPLYBitmapCopy.SrcRect.wBottom = _OSDDSPLYBitmapCopy.SrcRect.wTop + (_wOSDDSPLYMaxItem-1)*OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT - 1;
    
    GDI_BitmapCopy(&_OSDDSPLYBitmapCopy);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_DrawMenuItem
//  Abstract    :   Call this function to draw the item of the menu.
//  Arguments   :   bIndex: the index of the item.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_DrawMenuItem(BYTE bIndex, BYTE bHighlight)
{         
    _OSDDSPLY_NAVIGATOR_SetCommonItemStringAttribute();
    if (bHighlight) //highlight
    {
        _OSDDSPLYStringAttr.bBackground = OSDDSPLY_NG_PALETTE_ENTRY_MENU_HIGHLIGHT; //Set the color of background is the same as color key.
    }
    else
    {
        _OSDDSPLYStringAttr.bBackground = OSDDSPLY_NG_PALETTE_ENTRY_GENERAL_BACKGROUND;
    }

    OSDUTL_PrepareOutputString();
    
    if (_wOSDDSPLYCurrentSettingIndex == (_wOSDDSPLYStartDrawItem+bIndex))
    {        
        //Draw the checked symbol.
        OSDUTL_CopyStringFromIndex(aDSPLYCheckSymbol);
        _OSDDSPLYStringAttr.wX = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH+OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H;
    }
    else
    {
        _OSDDSPLYStringAttr.wX = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH+OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H+GDI_GetStringWidth_909(aDSPLYCheckSymbol);
    }
    
    switch (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID)
    {
    case OSDDSPLY_NAVIGATOR_ICON_TITLE:
    case OSDDSPLY_NAVIGATOR_ICON_CHAPTER:
    case OSDDSPLY_NAVIGATOR_ICON_ANGLE:
        OSDUTL_OutputNumFromIndex((WORD)(_wOSDDSPLYStartDrawItem+bIndex+1));
        break;
    case OSDDSPLY_NAVIGATOR_ICON_AUDIO:
        _OSDDSPLY_NAVIGATOR_GetASTInfo(_wOSDDSPLYStartDrawItem+bIndex);
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST1:
        if ((_wOSDDSPLYStartDrawItem+bIndex) == __bSPSTNs) //"Off"
        {
            OSDUTL_CopyStringFromIndex(aDSPLYOff);
        }
        else
        {
            //CoCo1.02, Change using DVD_GetLangCD to get Language Code
           _wOSDDSPLYTemp = DVD_GetLangCD(SET_SPST, (_wOSDDSPLYStartDrawItem+bIndex));
           _OSDDSPLY_CodeToLanguage(_wOSDDSPLYTemp);
        }
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST2:
        if ((_wOSDDSPLYStartDrawItem+bIndex) == __bSPSTNs) //"Off"
        {
            OSDUTL_CopyStringFromIndex(aDSPLYOff);
        }
        else
        {
            // CoCo1.02, Change using DVD_GetLangCD to get Language Code
           _wOSDDSPLYTemp = DVD_GetLangCD(SET_SP2ST, (_wOSDDSPLYStartDrawItem+bIndex));
           _OSDDSPLY_CodeToLanguage(_wOSDDSPLYTemp);
        }
        break;
    case OSDDSPLY_NAVIGATOR_ICON_REPEAT:
        _OSDDSPLY_NAVIGATOR_GetRepeatItem((BYTE)(_wOSDDSPLYStartDrawItem+bIndex));
        break;
    default:
        break;
    }                
    
    _OSDDSPLYStringAttr.wY = _bOSDDSPLYMenuStartV+bIndex*OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT+((OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT- GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);
    //GDI_DrawString_909(0, &_OSDDSPLYStringAttr, __pOSDUTLOutputString);
    _OSDDSPLY_DrawString();
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_DrawUpDownArrowBackground
//  Abstract    :   Call this function to clear the "up" or "down" arrow of the 
//              :   menu.
//  Arguments   :   bArrowType: OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP/OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_DrawUpDownArrowBackground(BYTE bArrowType)
{
    _OSDDSPLYRect.rect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH;
    _OSDDSPLYRect.rect.wRight = _OSDDSPLYRect.rect.wLeft + _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu() + (OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1);
    
    if (bArrowType == OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP)
    {
        _OSDDSPLYRect.rect.wTop = _bOSDDSPLYMenuStartV - OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT;

    }
    else if (bArrowType == OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN)
    {
        _OSDDSPLYRect.rect.wTop = _bOSDDSPLYMenuStartV + _wOSDDSPLYMaxItem*OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT;
    }

    _OSDDSPLYRect.rect.wBottom = _OSDDSPLYRect.rect.wTop + OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT - 1;

    _OSDDSPLYRect.bColor = OSDDSPLY_NG_PALETTE_ENTRY_GENERAL_BACKGROUND;
   
    GDI_FillRect_909(0, &_OSDDSPLYRect);
}


//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ClearUpDownArrowBackground
//  Abstract    :   Call this function to clear the "up" or "down" arrow of the 
//              :   menu.
//  Arguments   :   bArrowType: OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP/OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ClearUpDownArrowBackground(BYTE bArrowType)
{
    _OSDDSPLYRect.rect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH-OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS;
    _OSDDSPLYRect.rect.wRight = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH + _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu() + (OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1)+OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS;
    
    if (bArrowType == OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP)
    {
        _OSDDSPLYRect.rect.wTop = _bOSDDSPLYMenuStartV - OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT-OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS;

    }
    else if (bArrowType == OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN)
    {
        _OSDDSPLYRect.rect.wTop = _bOSDDSPLYMenuStartV + _wOSDDSPLYMaxItem*OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT;
    }

    _OSDDSPLYRect.rect.wBottom = _OSDDSPLYRect.rect.wTop + OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT + OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS;

    _OSDDSPLYRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
   
    GDI_FillRect_909(0, &_OSDDSPLYRect);          
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_DrawMainFrame
//  Abstract    :   Call this function to draw the main frame of the menu.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_DrawMainFrame(void)
{
    _OSDDSPLYFrame.bColorBright = OSDDSPLY_NG_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
    _OSDDSPLYFrame.bColorDark = OSDDSPLY_NG_PALETTE_ENTRY_NORMAL_FRAME_DARK;    
    _OSDDSPLYFrame.bColorCenter = PAL_ENTRY_COLOR_TRANSPARENT; //"FRAME_DOUBLE" doesn't need to assign the center color?
    _OSDDSPLYFrame.bStyle = FRAME_DOUBLE;
    _OSDDSPLYFrame.bThickness = OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS >> 1;
    _OSDDSPLYFrame.rect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH - OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS;
    if (_bOSDDSPLYMenuHasUpArrow)
    {
        _OSDDSPLYFrame.rect.wTop = _bOSDDSPLYMenuStartV - OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT;        
    }
    else
    {
        _OSDDSPLYFrame.rect.wTop = _bOSDDSPLYMenuStartV;
    }

    _OSDDSPLYFrame.rect.wTop -= OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS;
    _OSDDSPLYFrame.rect.wRight = _OSDDSPLYFrame.rect.wLeft + _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu() + (OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1) + (OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS << 1);

    _OSDDSPLYFrame.rect.wBottom = _bOSDDSPLYMenuStartV + _wOSDDSPLYMaxItem*OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT;
    if (_bOSDDSPLYMenuHasDownArrow)
    {
        _OSDDSPLYFrame.rect.wBottom += OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT;
    }
    _OSDDSPLYFrame.rect.wBottom += OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS;

    GDI_DrawFrame_909(0, &_OSDDSPLYFrame);
}


//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ClearMenu
//  Abstract    :   Call this function to clear the menu.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ClearMenu(void)
{
    _OSDDSPLYRect.rect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH - OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS;
    if (_bOSDDSPLYMenuHasUpArrow)
    {
        _OSDDSPLYRect.rect.wTop = _bOSDDSPLYMenuStartV - OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT;        
    }
    else
    {
        _OSDDSPLYRect.rect.wTop = _bOSDDSPLYMenuStartV;
    }
    _OSDDSPLYRect.rect.wTop -= OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS;
    _OSDDSPLYRect.rect.wRight = _OSDDSPLYRect.rect.wLeft + _OSDDSPLY_NAVIGATOR_GetMaxWidthInMenu() + (OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1) + (OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS << 1);

    _OSDDSPLYRect.rect.wBottom = _bOSDDSPLYMenuStartV + _wOSDDSPLYMaxItem*OSDDSPLY_NAVIGATOR_MENU_ITEM_HEIGHT;
    if (_bOSDDSPLYMenuHasDownArrow)
    {
        _OSDDSPLYRect.rect.wBottom += OSDDSPLY_NAVIGATOR_MENU_ARROW_ITEM_HEIGHT;
    }
    _OSDDSPLYRect.rect.wBottom += OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS;

    _OSDDSPLYRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    
    GDI_FillRect_909(0, &_OSDDSPLYRect);
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_SetTipString
//  Abstract    :   Call this function to set the "tip" string.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_SetTipString(void)
{
    OSDUTL_PrepareOutputString();

    switch (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID)
    {
    case OSDDSPLY_NAVIGATOR_ICON_TITLE:
        OSDUTL_CopyStringFromIndex(aNGTitle);
        _OSDDSPLY_ShowTitleInfo();
        break;
    case OSDDSPLY_NAVIGATOR_ICON_CHAPTER:
        OSDUTL_CopyStringFromIndex(aNGChapter);
        _OSDDSPLY_ShowChapterInfo();
        break;
    case OSDDSPLY_NAVIGATOR_ICON_ANGLE:
        OSDUTL_CopyStringFromIndex(aNGAngle);
        _OSDDSPLY_ShowAngleInfo();
        break;
    case OSDDSPLY_NAVIGATOR_ICON_AUDIO:
        OSDUTL_CopyStringFromIndex(aNGAudio);
        _OSDDSPLY_ShowASTInfo();
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST1:
        OSDUTL_CopyStringFromIndex(aNGSubtitle1);
        _OSDDSPLY_ShowSPST1Info(TRUE);
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST2:
        OSDUTL_CopyStringFromIndex(aNGSubtitle2);
        _OSDDSPLY_NAVIGATOR_ShowSPST2Info();
        break;
    case OSDDSPLY_NAVIGATOR_ICON_REPEAT:
        _OSDDSPLY_NAVIGATOR_GetRepeatItem((BYTE)_wOSDDSPLYCurrentSettingIndex);
        break;
    default:
        break;
    }
}   

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ShowTip
//  Abstract    :   Call this function to show tip.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ShowTip(void)
{  
    _OSDDSPLY_NAVIGATOR_PrepareMenuInfo();

    _OSDDSPLY_NAVIGATOR_SetTipString();

    //Draw Tip rectangle.
    _OSDDSPLYRect.rect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH;
    _OSDDSPLYRect.rect.wRight = _OSDDSPLYRect.rect.wLeft + GDI_GetStringWidth_909(__pOSDUTLOutputString) + (OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1);

#ifdef OSDDSPLY_NAVIGATOR_SUBMENU_START_FROM_ICON_TOP
    _OSDDSPLYRect.rect.wTop = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+OSDDSPLY_NAVIGATOR_ICON_HEIGHT+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) - OSDDSPLY_NAVIGATOR_TIP_HEIGHT;
#else
    _OSDDSPLYRect.rect.wTop = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) - OSDDSPLY_NAVIGATOR_TIP_HEIGHT;
#endif //OSDDSPLY_SUPPORT_SHOW_CURRENT_DISPLAY_BEFORE_SWITCH
    _OSDDSPLYRect.rect.wBottom = _OSDDSPLYRect.rect.wTop+OSDDSPLY_NAVIGATOR_TIP_HEIGHT-1;    
    _OSDDSPLYRect.bColor = OSDDSPLY_NG_PALETTE_ENTRY_TIP_BACKGROUND;            
    GDI_FillRect_909(0, &_OSDDSPLYRect);
    
    //Draw the tip's frame
    _OSDDSPLYFrame.bColorBright = OSDDSPLY_NG_PALETTE_ENTRY_TIP_FRAME_BRIGHT;
    _OSDDSPLYFrame.bColorDark = OSDDSPLY_NG_PALETTE_ENTRY_TIP_FRAME_DARK;    
    _OSDDSPLYFrame.bColorCenter = PAL_ENTRY_COLOR_TRANSPARENT; //"FRAME_DOUBLE" doesn't need to assign the center color?
    _OSDDSPLYFrame.bStyle = FRAME_PUSH;
    _OSDDSPLYFrame.bThickness = OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS;
    _OSDDSPLYFrame.rect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH - OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS;
    _OSDDSPLYFrame.rect.wRight = _OSDDSPLYFrame.rect.wLeft + GDI_GetStringWidth_909(__pOSDUTLOutputString) + (OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1) + (OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS << 1);
#ifdef OSDDSPLY_NAVIGATOR_SUBMENU_START_FROM_ICON_TOP
    _OSDDSPLYFrame.rect.wTop = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+OSDDSPLY_NAVIGATOR_ICON_HEIGHT+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) - OSDDSPLY_NAVIGATOR_TIP_HEIGHT - OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS;
    _OSDDSPLYFrame.rect.wBottom = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+OSDDSPLY_NAVIGATOR_ICON_HEIGHT+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) + OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS -1;
#else
    _OSDDSPLYFrame.rect.wTop = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) - OSDDSPLY_NAVIGATOR_TIP_HEIGHT - OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS;
    _OSDDSPLYFrame.rect.wBottom = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) + OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS -1;
#endif
    GDI_DrawFrame_909(0, &_OSDDSPLYFrame);

    //Draw tip's string
    _OSDDSPLY_NAVIGATOR_DrawTipString();

    _bOSDDSPLYTipDisplayed = TRUE;
    _dwOSDDSPLYDisplayStartTime = OS_GetSysTimer();
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_DrawTipString
//  Abstract    :   Call this function to draw the top's string.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_DrawTipString(void)
{
    _OSDDSPLY_NAVIGATOR_SetCommonItemStringAttribute();
    _OSDDSPLYStringAttr.bBackground = OSDDSPLY_NG_PALETTE_ENTRY_TIP_BACKGROUND;
    
    _OSDDSPLY_NAVIGATOR_SetTipString();
    
    _OSDDSPLYStringAttr.wX = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH+OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H;
#ifdef OSDDSPLY_NAVIGATOR_SUBMENU_START_FROM_ICON_TOP
    _OSDDSPLYStringAttr.wY = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+OSDDSPLY_NAVIGATOR_ICON_HEIGHT+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) - OSDDSPLY_NAVIGATOR_TIP_HEIGHT +((OSDDSPLY_NAVIGATOR_TIP_HEIGHT- GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);
#else
    _OSDDSPLYStringAttr.wY = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) - OSDDSPLY_NAVIGATOR_TIP_HEIGHT +((OSDDSPLY_NAVIGATOR_TIP_HEIGHT- GDI_GetStringHeight_909(__pOSDUTLOutputString)) >> 1);
#endif
    //GDI_DrawString_909(0, &_OSDDSPLYStringAttr, __pOSDUTLOutputString);
    _OSDDSPLY_DrawString();
}


//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ClearTip
//  Abstract    :   Call this function to clear the tip.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ClearTip(void)
{  
    if (_bOSDDSPLYTipDisplayed)    
    {
        _OSDDSPLY_NAVIGATOR_SetTipString();

        //Draw Tip rectangle.
        _OSDDSPLYRect.rect.wLeft = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH- OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS;
        //_OSDDSPLYRect.rect.wRight = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosH +GDI_GetStringWidth_909(__pOSDUTLOutputString)+(OSDDSPLY_NAVIGATOR_MENU_ITEM_DISTANCE_H << 1)+OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS;
		_OSDDSPLYRect.rect.wRight = OSDDSPLY_NAVIGATOR_MAX_RIGHT_PROINT; //CoCo1.06, clear max. width because the content of the tips may be changed by hot keys like L/R key.
#ifdef OSDDSPLY_NAVIGATOR_SUBMENU_START_FROM_ICON_TOP
        _OSDDSPLYRect.rect.wTop = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+OSDDSPLY_NAVIGATOR_ICON_HEIGHT+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) - OSDDSPLY_NAVIGATOR_TIP_HEIGHT - OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+OSDDSPLY_NAVIGATOR_ICON_HEIGHT+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex)+OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS-1;    
#else
        _OSDDSPLYRect.rect.wTop = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex) - OSDDSPLY_NAVIGATOR_TIP_HEIGHT - OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS;
        _OSDDSPLYRect.rect.wBottom = OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].wPopMenuStartPosV+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYCursorIconIndex)+OSDDSPLY_NAVIGATOR_TIP_FRAME_THICKNESS-1;    
#endif
        _OSDDSPLYRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;            
        GDI_FillRect_909(0, &_OSDDSPLYRect);
        
        _bOSDDSPLYTipDisplayed = FALSE;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ClearIconHighlight
//  Abstract    :   Call this function to clear the highlight at the icon part.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ClearIconHighlight(void)
{
    //Change the palette of the current icon to normal state.
    GDI_ChangePALEntry(OSDDSPLYNGBGPalette[_bOSDDSPLYCursorIconIndex], OSDDSPLY_NG_PALETTE_COLOR_ICON_NORMAL_BACKGROUND, FALSE);
    
    //Clear tip
    if (_bOSDDSPLYTipDisplayed)
        _OSDDSPLY_NAVIGATOR_ClearTip();
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ShowIconHighlight
//  Abstract    :   Call this function to show the highlight at the icon part.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ShowIconHighlight(void)
{
    //Change the palette of the current icon from normal state to highlight state.
    GDI_ChangePALEntry(OSDDSPLYNGBGPalette[_bOSDDSPLYCursorIconIndex], OSDDSPLY_NG_PALETTE_COLOR_ICON_HIGHLIGHT_BACKGROUND, FALSE);
    
    //Show tip
    if (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconAttr == OSDDSPLY_NAVIGATOR_ICON_ATTRIBUTE_POPUP_MENU)
        _OSDDSPLY_NAVIGATOR_ShowTip();
}


//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ShowNavigator
//  Abstract    :   Call this function to show the Navigator.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ShowNavigator(void)
{
    //Initialize the palette for Navigator
    _OSDDSPLY_NAVIGATOR_InitializeGDI();
    GDI_WaitPaletteComplete();

    //Load palette for Navigaor's icons.
    //GDI_LoadPalette(GDI_PAL_NAVIGATOR);
    //GDI_LoadBMP(GDI_OSD_BMP); // load other BMP used by Display mode (this including screen saver BMP)
	//CoCo1.06, load Navigator bitmap.
	GDI_LoadPalette(&__PaletteInfo_Navigator);
    GDI_LoadBMP(_aNavigatorBMPArray, OSDDSPLY_MAX_NAVIGATOR_BMP_NUM);

    //Draw the frame for the icons.
    _OSDDSPLYFrame.bColorBright = OSDDSPLY_NG_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
    _OSDDSPLYFrame.bColorDark = OSDDSPLY_NG_PALETTE_ENTRY_NORMAL_FRAME_DARK;
    _OSDDSPLYFrame.bColorCenter = PAL_ENTRY_COLOR_TRANSPARENT; //"FRAME_DOUBLE" doesn't need to assign the center color?
    _OSDDSPLYFrame.bStyle = FRAME_DOUBLE;
    _OSDDSPLYFrame.bThickness = OSDDSPLY_NAVIGATOR_ICON_FRAME_THICKNESS >> 1;
    _OSDDSPLYFrame.rect.wLeft = OSDDSPLY_NAVIGATOR_ICON_START_H-OSDDSPLY_NAVIGATOR_ICON_FRAME_THICKNESS;
    _OSDDSPLYFrame.rect.wTop = OSDDSPLY_NAVIGATOR_ICON_START_V-OSDDSPLY_NAVIGATOR_ICON_FRAME_THICKNESS;
    _OSDDSPLYFrame.rect.wRight = OSDDSPLY_NAVIGATOR_ICON_START_H+OSDDSPLY_NAVIGATOR_ICON_WIDTH+OSDDSPLY_NAVIGATOR_ICON_FRAME_THICKNESS;
    _OSDDSPLYFrame.rect.wBottom = OSDDSPLY_NAVIGATOR_ICON_START_V+OSDDSPLY_NAVIGATOR_TOTAL_ICONS*OSDDSPLY_NAVIGATOR_ICON_HEIGHT+_OSDDSPLY_NAVIGATOR_TotalSeparatorHeight()+OSDDSPLY_NAVIGATOR_ICON_FRAME_THICKNESS;
    GDI_DrawFrame_909(0, &_OSDDSPLYFrame);      

    for (_bOSDDSPLYTemp = 0; _bOSDDSPLYTemp < OSDDSPLY_NAVIGATOR_TOTAL_ICONS; _bOSDDSPLYTemp++)
    {
        switch (OSDDSPLYNavigator[_bOSDDSPLYTemp].bIconID)
        {
        case OSDDSPLY_NAVIGATOR_ICON_TITLE:
            _OSDDSPLYDrawBitmap.bBitmapID = OSDDSPLY_NAVIGATOR_BMP_NG_TITLE;
            break;
        case OSDDSPLY_NAVIGATOR_ICON_CHAPTER:
            _OSDDSPLYDrawBitmap.bBitmapID = OSDDSPLY_NAVIGATOR_BMP_NG_CHAPTER;
            break;
        case OSDDSPLY_NAVIGATOR_ICON_AUDIO:
            _OSDDSPLYDrawBitmap.bBitmapID = OSDDSPLY_NAVIGATOR_BMP_NG_AUDIO;
            break;
        case OSDDSPLY_NAVIGATOR_ICON_SPST1:
            _OSDDSPLYDrawBitmap.bBitmapID = OSDDSPLY_NAVIGATOR_BMP_NG_SUBTITLE1;
            break;
        case OSDDSPLY_NAVIGATOR_ICON_SPST2:
            _OSDDSPLYDrawBitmap.bBitmapID = OSDDSPLY_NAVIGATOR_BMP_NG_SUBTITLE2;
            break;
        // wyc2.17-909S, only valid when no language study mode.
#if (SLOW_MODE_CONFIG == NORMAL_SLOW_MODE)
        case OSDDSPLY_NAVIGATOR_ICON_ANGLE:
            _OSDDSPLYDrawBitmap.bBitmapID = OSDDSPLY_NAVIGATOR_BMP_NG_ANGLE;
            break;
#endif
        case OSDDSPLY_NAVIGATOR_ICON_REPEAT:
            _OSDDSPLYDrawBitmap.bBitmapID = OSDDSPLY_NAVIGATOR_BMP_NG_REPEAT;
            break;
        // wyc2.17-909S, enable it when language study mode.
#if (SLOW_MODE_CONFIG != NORMAL_SLOW_MODE)
        case OSDDSPLY_NAVIGATOR_ICON_REPEATAB:
            _OSDDSPLYDrawBitmap.bBitmapID = OSDDSPLY_NAVIGATOR_BMP_NG_REPEATAB;
            break;
        case OSDDSPLY_NAVIGATOR_ICON_SLOW:
            _OSDDSPLYDrawBitmap.bBitmapID = OSDDSPLY_NAVIGATOR_BMP_NG_SLOW;
            break;
#endif
        default:
            break;
        }
        
        _OSDDSPLYDrawBitmap.wDestX = OSDDSPLY_NAVIGATOR_ICON_START_H;
        _OSDDSPLYDrawBitmap.wDestY = OSDDSPLY_NAVIGATOR_ICON_START_V+_bOSDDSPLYTemp*OSDDSPLY_NAVIGATOR_ICON_HEIGHT+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYTemp);
        _OSDDSPLYDrawBitmap.bDestRegion = 0;
        GDI_DrawBitmap(&_OSDDSPLYDrawBitmap, _aNavigatorBMPArray);        
    }
    
    for (_bOSDDSPLYTemp = 0; _bOSDDSPLYTemp < OSDDSPLY_NAVIGATOR_TOTAL_ICONS; _bOSDDSPLYTemp++)
    {
        GDI_ChangePALEntry(OSDDSPLYNGBGPalette[_bOSDDSPLYTemp], OSDDSPLY_NG_PALETTE_COLOR_ICON_NORMAL_BACKGROUND, FALSE);
    }

#ifdef SUPPORT_SEPARATOR
    //Draw separator
    _OSDDSPLY_NAVIGATOR_DrawIconSeparateFrame();
#endif

    //Do variables's initialization
    _bOSDDSPLYTipDisplayed = FALSE;
    _bOSDDSPLYCursorIconIndex = 0;
    _bOSDDSPLYCursorPos = OSDDSPLY_NAVIGATOR_CURSOR_POS_ICON;

    //Prepare the info. for displaying tip.
    _OSDDSPLY_NAVIGATOR_PrepareMenuInfo();

    //Change the palette to show the highlight
    _OSDDSPLY_NAVIGATOR_ShowIconHighlight();
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ChangeIconCursor
//  Abstract    :   Call this function to change the cursor if the cursor is
//              :   at the icon part.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ChangeIconCursor(BYTE bIconID)
{
    _OSDDSPLY_NAVIGATOR_ClearIconHighlight();

    for (_bOSDDSPLYTemp = 0; _bOSDDSPLYTemp < OSDDSPLY_NAVIGATOR_TOTAL_ICONS; _bOSDDSPLYTemp++)
    {
        if (OSDDSPLYNavigator[_bOSDDSPLYTemp].bIconID == bIconID)
        {
            _bOSDDSPLYCursorIconIndex = _bOSDDSPLYTemp;
            _OSDDSPLY_NAVIGATOR_ShowIconHighlight();
            break;
        }
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_PopupMenu
//  Abstract    :   Call this function to pop-up the menu.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_PopupMenu(void)
{
    //Prepare menu info.
    _OSDDSPLY_NAVIGATOR_PrepareMenuInfo();
    
    //Initialize menu attribute.
    _OSDDSPLY_NAVIGATOR_InitializeMenuAttribute();
    
    //Draw the menu
    _OSDDSPLY_NAVIGATOR_DrawMenu();
    
    //Draw the items in the menu.
    _OSDDSPLY_NAVIGATOR_DrawAllItems();
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForIcon
//  Abstract    :   This function will process the special key if the cursor is 
//              :   at the icon part.
//  Arguments   :   none.
//  Return      :   
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForIcon(BYTE bIconID)
{
    if (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID == bIconID) //Pop-up the AUDIO menu to let user change the setting.
    {
        if (_OSDDSPLY_NAVIGATOR_CheckValidPopupMenu())
        {
            _OSDDSPLY_NAVIGATOR_ClearTip();
            
            _bOSDDSPLYCursorPos = OSDDSPLY_NAVIGATOR_CURSOR_POS_MENU;
            _OSDDSPLY_NAVIGATOR_PopupMenu();                        
        }
        else
            return INVALID_KEY;
    }
    else //change the cursor to the icon
    {
        _OSDDSPLY_NAVIGATOR_ChangeIconCursor(bIconID);
    }

    return KEY_NO_KEY;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_MoveCursorToNextMenuItem
//  Abstract    :   This function will move the cursor to the next menu item.
//  Arguments   :   none.
//  Return      :   TRUE: The cursor can move to the next item.
//              :   FALSE: The cursor can't move to the next item. (the last item of the menu)
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_NAVIGATOR_MoveCursorToNextMenuItem(void)
{
    if (_wOSDDSPLYCursorMenuIndex < (_wOSDDSPLYTotalSetting-1))
    {
        _bOSDDSPLYReDrawMainFrame = FALSE;

        //Clear the highlight of the current item
        _OSDDSPLY_NAVIGATOR_ClearItemRegion((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), FALSE);
        
        //Re-draw the item
        _OSDDSPLY_NAVIGATOR_DrawMenuItem((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), FALSE);
        
        //Update the cursor's index.
        _wOSDDSPLYCursorMenuIndex++;
        
        if (_wOSDDSPLYCursorMenuIndex > (_wOSDDSPLYStartDrawItem+_wOSDDSPLYMaxItem-1)) //scroll down
        {
            _OSDDSPLY_NAVIGATOR_Scroll(OSDDSPLY_NAVIGATOR_SCROLL_TYPE_DOWN);
            
            //update the _wOSDDSPLYStartDrawItem
            _wOSDDSPLYStartDrawItem++;
            
            //clear the "down arrow" ?
            if (_wOSDDSPLYCursorMenuIndex == (_wOSDDSPLYTotalSetting-1))
            {
                _bOSDDSPLYMenuHasDownArrow = FALSE;
                
                _OSDDSPLY_NAVIGATOR_ClearUpDownArrowBackground(OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN);
                
                //Need to re-draw the main-frame if the "up" or "down" arrow is removed.
                _bOSDDSPLYReDrawMainFrame = TRUE;
            }
            
            //Draw the "up arrow"?
            if (!_bOSDDSPLYMenuHasUpArrow)
            {
                if (_wOSDDSPLYStartDrawItem != 0)
                {
                    _OSDDSPLY_NAVIGATOR_DrawUpDownArrowBackground(OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP);
                    _OSDDSPLY_NAVIGATOR_UpdateUpDownArraow(OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP);
                    _bOSDDSPLYMenuHasUpArrow = TRUE;

                    //Draw "separate" frame.
                    _OSDDSPLY_NAVIGATOR_DrawSeparateFrame(OSDDSPLY_NAVIGATOR_UP_SEPARATE_FRAME);
                    
                    //Need to re-draw the main-frame if the "down" arrow is added.
                    _bOSDDSPLYReDrawMainFrame = TRUE;                    
                }                        
            }
            
            if (_bOSDDSPLYReDrawMainFrame)
                _OSDDSPLY_NAVIGATOR_DrawMainFrame();
        }
        
        //Draw the new item.
        _OSDDSPLY_NAVIGATOR_ClearItemRegion((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), TRUE);
        _OSDDSPLY_NAVIGATOR_DrawMenuItem((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), TRUE);

        return TRUE;
    }
    else
        return FALSE;

    return FALSE;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_MoveCursorToPreviousMenuItem
//  Abstract    :   This function will move the cursor to the previous menu item.
//  Arguments   :   none.
//  Return      :   TRUE: The cursor can move to the previous item.
//              :   FALSE: The cursor can't move to the previous item. (the first item of the menu)
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_NAVIGATOR_MoveCursorToPreviousMenuItem(void)
{
    if (_wOSDDSPLYCursorMenuIndex > 0)
    {
        _bOSDDSPLYReDrawMainFrame = FALSE;
        
        //Clear the highlight of the current item
        _OSDDSPLY_NAVIGATOR_ClearItemRegion((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), FALSE);
        
        //Re-draw the item
        _OSDDSPLY_NAVIGATOR_DrawMenuItem((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), FALSE);
        
        //Update the cursor's index.
        _wOSDDSPLYCursorMenuIndex--;
        
        if (_wOSDDSPLYCursorMenuIndex < _wOSDDSPLYStartDrawItem) //scroll up
        {
            //Do bitmap copy from the index to (_wOSDDSPLYMaxItem-2)
            _OSDDSPLY_NAVIGATOR_Scroll(OSDDSPLY_NAVIGATOR_SCROLL_TYPE_UP);
            
            //update the _wOSDDSPLYStartDrawItem
            _wOSDDSPLYStartDrawItem = _wOSDDSPLYCursorMenuIndex;
            
            //clear the "up arrow" ?
            if (_wOSDDSPLYStartDrawItem == 0)
            {
                _bOSDDSPLYMenuHasUpArrow = FALSE;
                
                _OSDDSPLY_NAVIGATOR_ClearUpDownArrowBackground(OSDDSPLY_NAVIGATOR_ARROW_TYPE_UP);
                
                //Need to re-draw the main-frame if the "up" or "down" arrow is removed.
                _bOSDDSPLYReDrawMainFrame = TRUE;
            }
            
            //Draw the "down arrow"?
            if (!_bOSDDSPLYMenuHasDownArrow)
            {
                if ((_wOSDDSPLYStartDrawItem+_wOSDDSPLYMaxItem) < _wOSDDSPLYTotalSetting)
                {
                    _OSDDSPLY_NAVIGATOR_DrawUpDownArrowBackground(OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN);
                    _OSDDSPLY_NAVIGATOR_UpdateUpDownArraow(OSDDSPLY_NAVIGATOR_ARROW_TYPE_DOWN);
                    _bOSDDSPLYMenuHasDownArrow = TRUE;
                    
                    //Draw "separate" frame.
                    _OSDDSPLY_NAVIGATOR_DrawSeparateFrame(OSDDSPLY_NAVIGATOR_DOWN_SEPARATE_FRAME);

                    //Need to re-draw the main-frame if the "down" arrow is added.
                    _bOSDDSPLYReDrawMainFrame = TRUE;
                }                        
            }
            
            if (_bOSDDSPLYReDrawMainFrame)
                _OSDDSPLY_NAVIGATOR_DrawMainFrame();
        }
        
        //Draw the new item.
        _OSDDSPLY_NAVIGATOR_ClearItemRegion((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), TRUE);
        _OSDDSPLY_NAVIGATOR_DrawMenuItem((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), TRUE);

        return TRUE;
    }

    return FALSE;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForMenu
//  Abstract    :   This function will process the special keys for the menu.
//              :   When key is accepted, we will switch the item like KEY_DOWN.
//  Arguments   :   none.
//  Return      :   TRUE: 
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_NAVIGATOR_ProcessSpecialKeyForMenu(BYTE bIconID)
{
    if (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID == bIconID)
    {
        if (_wOSDDSPLYCursorMenuIndex >= (_wOSDDSPLYTotalSetting-1))
        {
            //change the setting directly.
            //if (!_OSDDSPLY_NAVIGATOR_MoveCursorToNextMenuItem())
            //{
            //re-draw the main menu and jump the cursor to the first item.
            if (_wOSDDSPLYTotalSetting > _wOSDDSPLYMaxItem) //Need to re-draw all menu because it need to clear the "up" arrow, draw "down" arrow and scroll the menu items.
            {
                _OSDDSPLY_NAVIGATOR_ClearMenu();
                _wOSDDSPLYCurrentSettingIndex = 0;
                _wOSDDSPLYCursorMenuIndex = 0;
                //Initialize menu attribute.
                _OSDDSPLY_NAVIGATOR_InitializeMenuAttribute();
                
                //Draw the menu
                _OSDDSPLY_NAVIGATOR_DrawMenu();
                
                //Draw the items in the menu.
                _OSDDSPLY_NAVIGATOR_DrawAllItems();                        
            }
            else //clear the last item and update the cursor to the first item
            {
                //Clear the highlight of the current item
                _OSDDSPLY_NAVIGATOR_ClearItemRegion((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), FALSE);
                
                //Change the current setting before re-draw the item because we need to clear the "checked" symbol
                _wOSDDSPLYCurrentSettingIndex = 0;
                
                //Re-draw the item
                _OSDDSPLY_NAVIGATOR_DrawMenuItem((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), FALSE);
                _wOSDDSPLYCursorMenuIndex = 0;
                
                //Draw the new item.
                _OSDDSPLY_NAVIGATOR_ClearItemRegion((BYTE)_wOSDDSPLYCursorMenuIndex, TRUE);
                _OSDDSPLY_NAVIGATOR_DrawMenuItem((BYTE)(_wOSDDSPLYCursorMenuIndex-_wOSDDSPLYStartDrawItem), TRUE); 
            }
        }
        else
        {
            _wOSDDSPLYCurrentSettingIndex++;
            _OSDDSPLY_NAVIGATOR_MoveCursorToNextMenuItem();
        }

        //Change setting
        if (!_OSDDSPLY_NAVIGATOR_ChangeSetting())
            return FALSE;

        return TRUE;
    }
    else
        return FALSE;

    return FALSE;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_CheckValidPopupMenu
//  Abstract    :   This function will check if the menu can be pop-up or not.
//  Arguments   :   none.
//  Return      :   TRUE: The menu can be pop-up.
//              :   FALSE: The menu can't be pop-up.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_NAVIGATOR_CheckValidPopupMenu(void)
{
    switch (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID)
    {
    case OSDDSPLY_NAVIGATOR_ICON_AUDIO:
        if (!__bASTNs) // AST exist
            return FALSE;
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST1:
        if (!__bSPSTNs) // exist SP
            return FALSE;
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST2:
        if (!__bSPSTNs) // exist SP
            return FALSE;
        else
        {
            // spst is on, ie. __bSP_Disp_OFF is 0 or 128 ~255
            if (!((__bSP_Disp_OFF==0) || (__bSP_Disp_OFF&0x80)))
                return FALSE;
        }
        break;
    case OSDDSPLY_NAVIGATOR_ICON_ANGLE:
        if (!__bAGL_Ns) //angle exist
            return FALSE;
        break;
    default:
        break;
    }

    return TRUE;
}
          
//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ChangeSetting
//  Abstract    :   This function will change the setting after user pressing
//              :   KEY_PLAY.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_NAVIGATOR_ChangeSetting(void)
{
    switch (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID)
    {
    case OSDDSPLY_NAVIGATOR_ICON_TITLE:
        _OSDDSPLY_NAVIGATOR_ChangeTitleSetting();
        break;
    case OSDDSPLY_NAVIGATOR_ICON_CHAPTER:
        if (NV_QueryUOP (UOP_CAN_PTT_SEARCH))
        {
            //__bFWKey = KEY_SEARCH_PLAY;
            __bSearchTitle = __bTitle;
            __wSearchChapter = _wOSDDSPLYCursorMenuIndex+1;
            
			/* --CoCo, some chapter can't be played. Therefoer, can't update __wChapter here.
            __wChapter = _wOSDDSPLYCursorMenuIndex+1;
            NV_TitlePlay(__bTitle,__wChapter);
			*/
        }
        else
            return FALSE;
        break;
    case OSDDSPLY_NAVIGATOR_ICON_AUDIO:
        if (CC_Audio_Change(CC_SWITCH_QUERY) == KEY_NO_KEY)
        {
            __bASTNO = _wOSDDSPLYCursorMenuIndex;
            CC_Audio_Change(CC_SWITCH_AUDIO);
        }
        else
            return FALSE;
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST1:
        if (CC_SPST_Change(CC_SWITCH_QUERY_SP1) == KEY_NO_KEY)
        {
            __bSPSTNO = _wOSDDSPLYCursorMenuIndex;
            CC_SPST_Change(CC_SWITCH_SP1);
        }
        else
            return FALSE;
        break;
    case OSDDSPLY_NAVIGATOR_ICON_SPST2:
        if (CC_SPST_Change(CC_SWITCH_QUERY_SP2) == KEY_NO_KEY)
        {
            __bSPST2NO = _wOSDDSPLYCursorMenuIndex;
            CC_SPST_Change(CC_SWITCH_SP2);
        }
        else
            return FALSE;
        break;
    case OSDDSPLY_NAVIGATOR_ICON_ANGLE:
        if (CC_Angle_Change(CC_SWITCH_QUERY) == KEY_NO_KEY)
        {
            __bAGLN = _wOSDDSPLYCursorMenuIndex+1;
            CC_Angle_Change(CC_SWITCH_ANGLE);
        }
        else
            return FALSE;
        break;
    case OSDDSPLY_NAVIGATOR_ICON_REPEAT:
        _OSDDSPLY_NAVIGATOR_SetRepeatSetting();
        break;
    default:
        break;
    }

    return TRUE;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_SetRepeatSetting
//  Abstract    :   This function will change the repeat setting.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_SetRepeatSetting(void)
{
    if ( __bDomain != TT_DOM )
    {
        __bRepeat = REPEAT_NONE;
    }
    else
    {
        if (__bNVPlayMode)
        {
            switch (_wOSDDSPLYCursorMenuIndex)
            {
            case 0:
                __bRepeat = REPEAT_TITLE;
                break;
            case 1:
                __bRepeat = REPEAT_CHAPTER;
                break;
            case 2:
                __bRepeat = REPEAT_NONE;
                break;
            default:
                break;
            }
        }
        else //PBC off
        {
            switch (_wOSDDSPLYCursorMenuIndex)
            {
            case 0:
                __bRepeat = REPEAT_TITLE;
                break;
            case 1:
                __bRepeat = REPEAT_CHAPTER;
                break;
            case 2:
                __bRepeat = REPEAT_DISC;
                break;
            case 3:
                __bRepeat = REPEAT_NONE;
                break;
            default:
                break;
            }
        }
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_CalculateMaxAudioMenuWidth
//  Abstract    :   This function will calculate the max. width for the audio menu .
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_CalculateMaxAudioMenuWidth(void)
{
    BYTE bIndex = 0;

    OSDUTL_PrepareOutputString();

    _wOSDDSPLYMaxWidth = 0;

    for (bIndex = 0; bIndex < __bASTNs; bIndex++)
    {
        __bOSDUTLIndex = 1;

        _OSDDSPLY_NAVIGATOR_GetASTInfo(bIndex);

        _wOSDDSPLYWidth = GDI_GetStringWidth_909(__pOSDUTLOutputString);

        if (_wOSDDSPLYMaxWidth < _wOSDDSPLYWidth)
        {
            _wOSDDSPLYMaxWidth = _wOSDDSPLYWidth;
        }
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_CalculateMaxSPST1MenuWidth
//  Abstract    :   This function will calculate the max. width for the SPST1 menu .
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_CalculateMaxSPST1MenuWidth(void)
{
    OSDUTL_PrepareOutputString();

    _wOSDDSPLYMaxWidth = 0;

    for (_bOSDDSPLYTemp = 0; _bOSDDSPLYTemp < __bSPSTNs; _bOSDDSPLYTemp++)
    {
        __bOSDUTLIndex = 1;

        //CoCo1.02, Change using DVD_GetLangCD to get Language Code
        _wOSDDSPLYTemp = DVD_GetLangCD(SET_SPST, _bOSDDSPLYTemp);
        _OSDDSPLY_CodeToLanguage(_wOSDDSPLYTemp);

        _wOSDDSPLYWidth = GDI_GetStringWidth_909(__pOSDUTLOutputString);

        if (_wOSDDSPLYMaxWidth < _wOSDDSPLYWidth)
        {
            _wOSDDSPLYMaxWidth = _wOSDDSPLYWidth;
        }
    }

    //Judge "Off" selection
    __pOSDUTLOutputString = aDSPLYOff;
    _wOSDDSPLYWidth = GDI_GetStringWidth_909(__pOSDUTLOutputString);
    if (_wOSDDSPLYMaxWidth < _wOSDDSPLYWidth)
    {
        _wOSDDSPLYMaxWidth = _wOSDDSPLYWidth;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_GetASTInfo
//  Abstract    :   This function will get the AST info.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_GetASTInfo(BYTE bASTNO)
{
    BYTE bAudioType;

    // CoCo1.02, Change using DVD_GetLangCD to get Language Code
    _wOSDDSPLYTemp = DVD_GetLangCD(SET_AST, bASTNO) ;
    _OSDDSPLY_CodeToLanguage(_wOSDDSPLYTemp);

    OSDUTL_CopyStringFromIndex(aDSPLYLanguageAudioTypeDistance);
    //show audio output
    switch (__dwDVD_ATRT1 >> 29)
    {
    case 2:
    case 3:
        bAudioType = HAL_AUDIO_MPG;
        break;
    case 4:
        bAudioType = HAL_AUDIO_LPCM;
        break;
    case 6:
        bAudioType = HAL_AUDIO_DTS;
        break;
        /*
    case 7:
        bAudioType = HAL_AUDIO_SDDS;
        break;
        */
    default:
        bAudioType = HAL_AUDIO_AC3;
    }

    _OSDND_OutputAudioType(bAudioType);
    
    //Draw distance between audio type and info.
    OSDUTL_CopyStringFromIndex(aDSPLYInfoDistance);
    
    _OSDDND_OutputAudioChannel((BYTE)(((__dwDVD_ATRT1 >> 16) & 0x0007) + 1));
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ChangeTitleSetting
//  Abstract    :   
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ChangeTitleSetting(void)
{
    // don't allow Title/PTT play and PTT_Search
    if (!NV_QueryUOP (UOP_CAN_TITLE_PLAY) && !NV_QueryUOP (UOP_CAN_PTT_SEARCH))
    {
        return;
    }
    
    // check if Title selection not allowed
    if (NV_QueryUOP (UOP_CAN_TITLE_PLAY))
    {
        #if 0
        __bTitle = _wOSDDSPLYCursorMenuIndex+1;

        NV_TitlePlay(__bTitle, 1);
        #endif
        __bSearchTitle = _wOSDDSPLYCursorMenuIndex+1;
        __wSearchChapter = 1;
    }
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ShowSPST2Info
//  Abstract    :   This function will show the SPST2 info.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ShowSPST2Info(void)
{
    if (__bSPSTNs) // exist SP
    {
        // spst is on, ie. __bSP_Disp_OFF is 0 or 128 ~255
        if ((__bSP2_Disp_OFF==0) || (__bSP2_Disp_OFF&0x80))
        {
            //CoCo1.02, Change using DVD_GetLangCD to get Language Code
           _wOSDDSPLYTemp = DVD_GetLangCD(SET_SP2ST, __bSPST2NO) ;
            _OSDDSPLY_CodeToLanguage(_wOSDDSPLYTemp);
            OSDUTL_OutputNumFromIndex((WORD)(__bSPST2NO+1));
            OSDUTL_CopyStringFromIndex(aDSPLYSlash); //to be modified to "/"
            OSDUTL_OutputNumFromIndex(__bSPSTNs);
        }
        else
        {
            OSDUTL_CopyStringFromIndex(aDSPLYOff);
        }
    }
    else  // no SP exist
    {
        OSDUTL_CopyStringFromIndex(aDSPLYUnknownSubtitle);
    }
}

//***************************************************************************
//  Function    :   OSDDSPLY_NAVIGATOR_ClearMenuForNVChange
//  Abstract    :   bMessage: the message.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSDDSPLY_NAVIGATOR_ClearMenuForNVChange(BYTE bMessage) //CoCo1.11, modify this function to export function.
{
    if ((bMessage == MSG_TITLE) || (bMessage == MSG_CHAPTER))
    {
        if (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID != OSDDSPLY_NAVIGATOR_ICON_REPEAT)
        {
            _OSDDSPLY_NAVIGATOR_ClearMenuRegion();
        }
    }
    else if (bMessage == MSG_PBC) //Clear "Repeat" item.
    {
        if (OSDDSPLYNavigator[_bOSDDSPLYCursorIconIndex].bIconID == OSDDSPLY_NAVIGATOR_ICON_REPEAT)
        {
            _OSDDSPLY_NAVIGATOR_ClearMenuRegion();
        }
    }
}

void _OSDDSPLY_NAVIGATOR_ClearMenuRegion(void)
{
    //Don't use _OSDDSPLY_NAVIGATOR_ClearMenu() function to clear the menu because the new menu region may be different from the old menu region.
    _OSDDSPLYRect.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    
    _OSDDSPLYRect.rect.wLeft = OSDDSPLY_NAVIGATOR_ICON_START_H+OSDDSPLY_NAVIGATOR_ICON_WIDTH+OSDDSPLY_NAVIGATOR_MAIN_FRAME_THICKNESS+1;
    _OSDDSPLYRect.rect.wRight = OSDDSPLY_NAVIGATOR_MAX_RIGHT_PROINT;
    //_OSDDSPLYRect.rect.wTop = OSDDSPLY_NAVIGATOR_ICON_START_V;
    _OSDDSPLYRect.rect.wTop = OSDND_DISPLAY_UPPER_RIGHT_POS_V+OSDND_FONT_HEIGHT;
    _OSDDSPLYRect.rect.wBottom = OSDND_BOTTOM_BACKGROUND_START_V-1;
    
    GDI_FillRect_909(0, &_OSDDSPLYRect);
    
    //Set the cursor's position to the icon part.
    _bOSDDSPLYCursorPos = OSDDSPLY_NAVIGATOR_CURSOR_POS_ICON;
}



//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_InitializeGDI
//  Abstract    :   Initialize the palette, etc. for the Navigator UI.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_InitializeGDI(void)
{
    GDI_ChangePALEntry(OSDDSPLY_NG_PALETTE_ENTRY_GENERAL_BACKGROUND, OSDDSPLY_NG_PALETTE_COLOR_GENERAL_BACKGROUND, FALSE);
    GDI_ChangePALEntry(OSDDSPLY_NG_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT, OSDDSPLY_NG_PALETTE_COLOR_NORMAL_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(OSDDSPLY_NG_PALETTE_ENTRY_NORMAL_FRAME_DARK, OSDDSPLY_NG_PALETTE_COLOR_NORMAL_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(OSDDSPLY_NG_PALETTE_ENTRY_SEPARATE_FRAME_CENTER, OSDDSPLY_NG_PALETTE_COLOR_SEPARATE_FRAME_CENTER, FALSE);
    GDI_ChangePALEntry(OSDDSPLY_NG_PALETTE_ENTRY_MENU_HIGHLIGHT, OSDDSPLY_NG_PALETTE_COLOR_MENU_HIGHLIGHT, FALSE);
    GDI_ChangePALEntry(OSDDSPLY_NG_PALETTE_ENTRY_MENU_ITEM_TEXT, OSDDSPLY_NG_PALETTE_COLOR_MENU_ITEM_TEXT, FALSE);
    GDI_ChangePALEntry(OSDDSPLY_NG_PALETTE_ENTRY_TIP_FRAME_BRIGHT, OSDDSPLY_NG_PALETTE_COLOR_TIP_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(OSDDSPLY_NG_PALETTE_ENTRY_TIP_FRAME_DARK, OSDDSPLY_NG_PALETTE_COLOR_TIP_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(OSDDSPLY_NG_PALETTE_ENTRY_TIP_BACKGROUND, OSDDSPLY_NG_PALETTE_COLOR_TIP_BACKGROUND, FALSE);

    GDI_WaitPaletteComplete();
}

//CoCo1.0, add independent Navigator UI
#ifdef SUPPORT_INDEPENDENT_NAVIGATOR
//***************************************************************************
//  Function    :   OSDDSPLY_NAVIGATOR_Exit
//  Abstract    :   This function will be called by OSD kernel to exit the UI.
//  Arguments   :   bExitMode: OSD_UI_EXIT_ALL/OSD_UI_EXIT_CONTROL_ONLY
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void OSDDSPLY_NAVIGATOR_Exit(BYTE bExitMode)
{
    if (bExitMode == OSD_UI_EXIT_ALL)
    {
        _OSDDSPLY_ClearBackground(OSDDSPLY_BACKGROUND_NAVIGATOR);
    }

    _bNavigatorUI = FALSE;
}

//***************************************************************************
//  Function    :   OSDNAVIGATOR_Recover
//  Abstract    :   This function will be called by OSD kernel to recover Navigator UI.
//  Arguments   :   bRecoverRegion: OSD_RECOVER_UPPER_REGION/OSD_RECOVER_BOTTOM_REGION/OSD_RECOVER_ALL.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE OSDNAVIGATOR_Recover(BYTE bRecoverRegion)
{
    if (bRecoverRegion == OSD_RECOVER_ALL)
    {
        //Initialize GDI
        _OSDDSPLY_NAVIGATOR_InitializeGDI();      
        
        _OSDDSPLY_NAVIGATOR_ShowNavigator();
        
        return TRUE;
    }
    else
        return FALSE;

    return FALSE;
}

//***************************************************************************
//  Function    :   OSDNAVIGATOR_Trigger
//  Abstract    :   This function will be called by OSD_Trigger() to update UI.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
// LLY2.53, expand time value unit from WORD to DWORD
// Since, we will use [23:16]:hour, [15:8]: min, [7:0]: sec
//void OSDNAVIGATOR_Trigger(BYTE bType, WORD wTime)
void OSDNAVIGATOR_Trigger(BYTE bType, DWORD dwTime)
{    
    //Clear the "hint"?
    if (bType == OSD_TRIGGER_CONTROL)
    {
        if (_bNavigatorUI)
        {
            if ((OS_GetSysTimer()-_dwOSDDSPLYDisplayStartTime) > OSDDSPLY_NAVIGATOR_SHOW_TIP_TIME)
                _OSDDSPLY_NAVIGATOR_ClearTip();
        }
    }
}

#endif //SUPPORT_INDEPENDENT_NAVIGATOR

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_ProcessButton
//  Abstract    :   Call this function to set the button as pressed state.
//  Arguments   :   none.
//  Return      :  
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_ProcessButton(void)
{
    //Change the palette of the current icon to pressed state.
    GDI_ChangePALEntry(OSDDSPLYNGBGPalette[_bOSDDSPLYCursorIconIndex], OSDDSPLY_NG_PALETTE_COLOR_ICON_PRESSED_BACKGROUND, FALSE);
    GDI_WaitPaletteComplete();
    //UTL_DelayTime(COUNT_200_MSEC, FALSE);
    OS_DelayTime(COUNT_200_MSEC); //CoCo1.20, reduce code size
    GDI_ChangePALEntry(OSDDSPLYNGBGPalette[_bOSDDSPLYCursorIconIndex], OSDDSPLY_NG_PALETTE_COLOR_ICON_HIGHLIGHT_BACKGROUND, FALSE);    
    GDI_WaitPaletteComplete();
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_DrawIconSeparateFrame
//  Abstract    :   This function will draw the separate frame.
//  Arguments   :   none.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDDSPLY_NAVIGATOR_DrawIconSeparateFrame(void)
{
#ifdef SUPPORT_SEPARATOR

    for (_bOSDDSPLYTemp = 0; _bOSDDSPLYTemp < sizeof(_bOSDDSPLYNGSeparator); _bOSDDSPLYTemp++)
    {
        _OSDDSPLYFrame.bColorBright = OSDDSPLY_NG_PALETTE_ENTRY_NORMAL_FRAME_BRIGHT;
        _OSDDSPLYFrame.bColorDark = OSDDSPLY_NG_PALETTE_ENTRY_NORMAL_FRAME_DARK;    
        _OSDDSPLYFrame.bColorCenter = OSDDSPLY_NG_PALETTE_ENTRY_SEPARATE_FRAME_CENTER; //"FRAME_DOUBLE" doesn't need to assign the center color?
        _OSDDSPLYFrame.bStyle = FRAME_DOUBLE_SOLID;
        _OSDDSPLYFrame.bThickness = OSDDSPLY_NAVIGATOR_SEPARATE_FRAME_THICKNESS >> 1;
        _OSDDSPLYFrame.rect.wLeft = OSDDSPLY_NAVIGATOR_ICON_START_H;
        _OSDDSPLYFrame.rect.wRight = _OSDDSPLYFrame.rect.wLeft+OSDDSPLY_NAVIGATOR_ICON_WIDTH-1;    
        _OSDDSPLYFrame.rect.wTop = OSDDSPLY_NAVIGATOR_ICON_START_V+(_bOSDDSPLYNGSeparator[_bOSDDSPLYTemp]+1)*OSDDSPLY_NAVIGATOR_ICON_HEIGHT+_OSDDSPLY_NAVIGATOR_SeparatorHeight(_bOSDDSPLYNGSeparator[_bOSDDSPLYTemp]);
        _OSDDSPLYFrame.rect.wBottom = _OSDDSPLYFrame.rect.wTop+OSDDSPLY_NAVIGATOR_SEPARATE_FRAME_THICKNESS-1;
        
        GDI_DrawFrame_909(0, &_OSDDSPLYFrame);
    }
#endif //SUPPORT_SEPARATOR    
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_SeparatorHeight
//  Abstract    :   This function will calculate the height of the separators from
//              :   the first separator to current icon's position.
//  Arguments   :   The icon's index.
//  Return      :   The height of the separator.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_NAVIGATOR_SeparatorHeight(BYTE bIconIndex)
{
    BYTE bHeight = 0;

#ifdef SUPPORT_SEPARATOR

    BYTE bIndex = 0;

    if (bIconIndex == 0)
        return 0;

    bHeight = 0;

    for (bIndex = 0; bIndex < sizeof(_bOSDDSPLYNGSeparator); bIndex++)
    {
        if (bIconIndex > _bOSDDSPLYNGSeparator[bIndex])
        {
            bHeight += OSDDSPLY_NAVIGATOR_SEPARATE_FRAME_THICKNESS;
        }
    }

#endif //SUPPORT_SEPARATOR

    return bHeight;
}

//***************************************************************************
//  Function    :   _OSDDSPLY_NAVIGATOR_TotalSeparatorHeight
//  Abstract    :   This function will calcuate total separator's height.
//  Arguments   :   none.
//  Return      :   The height of total separators.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
BYTE _OSDDSPLY_NAVIGATOR_TotalSeparatorHeight(void)
{
    BYTE bHeight;

    bHeight = 0;

#ifdef SUPPORT_SEPARATOR

    for (_bOSDDSPLYTemp = 0; _bOSDDSPLYTemp < sizeof(_bOSDDSPLYNGSeparator); _bOSDDSPLYTemp++)
    {
        bHeight += OSDDSPLY_NAVIGATOR_SEPARATE_FRAME_THICKNESS;
    }

#endif //SUPPORT_SEPARATOR

    return bHeight;
}

#endif //SUPPORT_NAVIGATOR
#endif //#ifndef NO_DISC_MODE //CoCo2.37

void _OSDDSPLY_DrawString(void)
{
    GDI_DrawString_909(0, &_OSDDSPLYStringAttr, __pOSDUTLOutputString);
}

#ifdef SUPPORT_UNICODE_CODING
void _OSDDSPLY_DrawStringByUnicode(void)
{
    GDI_DrawStringByUnicode(0, &_OSDDSPLYStringAttr, __pOSDUTLOutputString);
}
#endif

//*********************************************************************************************************
//  Function    :   OSDDSPLY_ConfigOSDRegion
//  Abstract    :   This function will config the OSD region. And it is exported for OSD kernel usage.
//  Arguments   :   none.
//  Return		:   none.
//  Side Effect :   none.
//  Notes       :   
//*********************************************************************************************************
void OSDDSPLY_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;
    
#ifdef USE_4B_MODE_OSD
    RegionInfo.bColorMode = GDI_OSD_4B_MODE;
#else
    RegionInfo.bColorMode = GDI_OSD_8B_MODE;        
#endif //SUPPORT_SIMPLE_DISPLAY_MODE
    
    RegionInfo.wWidth = GDI_REGION_WIDTH;
    
#ifdef ENLARGE_OSD_FOR_PAL_MODE
    if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
    {
        RegionInfo.wHeight = GDI_REGION_HEIGHT;
    }
    else
    {
        RegionInfo.wHeight = GDI_REGION_HEIGHT_PAL;
    }
#else
    RegionInfo.wHeight = GDI_REGION_HEIGHT;
#endif
    
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;    
    
    OSD_SetRegion(0, FALSE, &RegionInfo);        
}
