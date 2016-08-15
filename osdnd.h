//**************************************************************************
//         Copyright (c) 2004, Cheertek Inc . All rights reserved.
//         D300, all right reserved.

//      Product : WinDVD Firmware

//      Date    : 2004.6.8
//      Author  : Cheertek (D300 CoCo Chiang)
//      Purpose : Normal Display
//      Sources : OSDND.h/OSDND.c
//***************************************************************************
#ifndef __OSDND_H__
#define __OSDND_H__

#include "gdi.h"

#define OSDUTL_STRING_LENGTH                            40

//define the mix ratio of the background for normal display.
#define OSDND_MIX_RATIO_BACKGROUND                      GDI_GENERAL_MIX_RATIO

#define OSDND_FONT_HEIGHT                               26

//define max "dot" when loading disc. For example, the max number of dots are 3 in "Loading..."
#define OSDND_MAX_DOT_NUM_FOR_LOADING                   3

//define the distance between the end of the background and the display.
#define OSDND_UPPER_BACKGROUND_RIGHT_DISPLAY_DISTANCE_H 0
#define OSDND_UPPER_BACKGROUND_RIGHT_DISPLAY_DISTANCE_V 0

//define the background region for upper right message
#define OSDND_UPPER_BACKGROUND_START_H                  0
#define OSDND_UPPER_BACKGROUND_START_V                  0
#define OSDND_UPPER_BACKGROUND_RIGHT                    616
#define OSDND_UPPER_BACKGROUND_BOTTOM                   (OSDND_UPPER_BACKGROUND_START_V+OSDND_UPPER_BACKGROUND_RIGHT_DISPLAY_DISTANCE_V+OSDND_FONT_HEIGHT-1)

//define the end the upper right message
#define OSDND_DISPLAY_UPPER_RIGHT_POS_H                 (OSDND_UPPER_BACKGROUND_RIGHT-OSDND_UPPER_BACKGROUND_RIGHT_DISPLAY_DISTANCE_H)
#define OSDND_DISPLAY_UPPER_RIGHT_POS_V                 (OSDND_UPPER_BACKGROUND_START_V+OSDND_UPPER_BACKGROUND_RIGHT_DISPLAY_DISTANCE_V)

//define the background region for bottom message
#define OSDND_BOTTOM_BACKGROUND_START_H                 0
#ifdef SUPPORT_SIMPLE_DISPLAY_MODE
#define OSDND_BOTTOM_BACKGROUND_START_V                 (OSDND_UPPER_BACKGROUND_START_V+OSDND_FONT_HEIGHT+26)
#else
#define OSDND_BOTTOM_BACKGROUND_START_V                 386//384
#endif //SUPPORT_SIMPLE_DISPLAY_MODE
#define OSDND_BOTTOM_BACKGROUND_START_V_PAL             472
#define OSDND_BOTTOM_BACKGROUND_START_RIGHT             616
#define OSDND_BOTTOM_BACKGROUND_HEIGHT                  26

//define the distance between the background and the content for bottom message.
#define OSDND_BOTTOM_BACKGROUND_DISPLAY_DISTANCE_H      16
#define OSDND_BOTTOM_BACKGROUND_DISPLAY_DISTANCE_V      0

//define the starting position for bottom message
#define OSDND_DISPLAY_BOTTOM_START_H                    (OSDND_BOTTOM_BACKGROUND_START_H+OSDND_BOTTOM_BACKGROUND_DISPLAY_DISTANCE_H)
#define OSDND_DISPLAY_BOTTOM_START_V                    (OSDND_BOTTOM_BACKGROUND_START_V+OSDND_BOTTOM_BACKGROUND_DISPLAY_DISTANCE_V)
#define OSDND_DISPLAY_BOTTOM_START_V_PAL                (OSDND_BOTTOM_BACKGROUND_START_V_PAL+OSDND_BOTTOM_BACKGROUND_DISPLAY_DISTANCE_V)

//define the starting entry for color palette
#define OSDND_PALETTE_ENTRY_START                       GDI_ND_PALETTE_INDEX_START

//define the palette entry
#define OSDND_PALETTE_ENTRY_BOTTOM_STRING                   OSDND_PALETTE_ENTRY_START
#define OSDND_PALETTE_ENTRY_UPPER_RIGHT_STRING              (OSDND_PALETTE_ENTRY_START+1)
#define OSDND_PALETTE_ENTRY_BACKGROUND                      (OSDND_PALETTE_ENTRY_START+2)
#define OSDND_PALETTE_ENTRY_BAR_FG                          (OSDND_PALETTE_ENTRY_START+3)
#define OSDND_PALETTE_ENTRY_MUTE_ICON_TEXT_FOREGROUND       (OSDND_PALETTE_ENTRY_START+4)
#define OSDND_PALETTE_ENTRY_MUTE_ICON_SHADOW_1              (OSDND_PALETTE_ENTRY_START+5)
#define OSDND_PALETTE_ENTRY_MUTE_ICON_SHADOW_2              (OSDND_PALETTE_ENTRY_START+6)
#define OSDND_PALETTE_ENTRY_MUTEOFF_ICON_TEXT_FOREGROUND    (OSDND_PALETTE_ENTRY_START+7)
#define OSDND_PALETTE_ENTRY_MUTEOFF_ICON_SHADOW_1           (OSDND_PALETTE_ENTRY_START+8)
#define OSDND_PALETTE_ENTRY_MUTEOFF_ICON_SHADOW_2           (OSDND_PALETTE_ENTRY_START+9)
#define OSDND_PALETTE_ENTRY_PROGRESS_COLOR                  (OSDND_PALETTE_ENTRY_START+10)

#ifndef USE_4B_MODE_OSD
#define OSDND_PALETTE_ENTRY_ZOOM_COLOR                      (OSDND_PALETTE_ENTRY_START+11)
#define OSDND_PALETTE_ENTRY_NORMAL_FRAME                    (OSDND_PALETTE_ENTRY_START+12)         
#endif

//define the palette color
#define OSDND_PALETTE_COLOR_BACKGROUND                      0x404040//0x202020
#define OSDND_PALETTE_COLOR_UPPER_RIGHT_STRING              0xC8C8C8
#define OSDND_PALETTE_COLOR_BOTTOM_STRING                   0xE5DD0D
#define OSDND_PALETTE_COLOR_BAR_FG                          0x007F00
#define OSDND_PALETTE_COLOR_MUTE_ICON_TEXT_FOREGROUND       0xDD0E19
#define OSDND_PALETTE_COLOR_MUTE_ICON_SHADOW_2              0x000000//0xFFD921
#define OSDND_PALETTE_COLOR_MUTE_ICON_SHADOW_1              0xFFD921//0x000000
#define OSDND_PALETTE_COLOR_MUTEOFF_ICON_TEXT_FOREGROUND    0x000000        
#define OSDND_PALETTE_COLOR_MUTEOFF_ICON_SHADOW_2           0xEFEFD7//0xFFD921
#define OSDND_PALETTE_COLOR_MUTEOFF_ICON_SHADOW_1           0xFFD921//0xEFEFD7
#define OSDND_PALETTE_COLOR_ZOOM_COLOR                      0xC4C4C4
#define OSDND_PALETTE_COLOR_PROGRESS_COLOR                  0x0042A5
#define OSDND_PALETTE_COLOR_NORMAL_FRAME                    0xF0D000                 

//define the size of the volume bar
#define OSDND_DISPLAY_VOLUME_BAR_RANGE                  16
#define OSDND_DISPLAY_VOLUME_BAR_WIDTH_PER_RANGE        10
#define OSDND_DISPLAY_VOLUME_BAR_HEIGHT                 12
#define OSDND_DISPLAY_VOLUME_BAR_WIDTH                  (OSDND_DISPLAY_VOLUME_BAR_RANGE*OSDND_DISPLAY_VOLUME_BAR_WIDTH_PER_RANGE)

//define the distance between the bar the the font
#define OSDND_DISTANCE_FONT_VOLUME_BAR                  8

//define the size of the key bar
#define OSDND_DISPLAY_AUDIO_KEY_BAR_RANGE               12
#define OSDND_DISPLAY_AUDIO_KEY_BAR_WIDTH_PER_RANGE     10
#define OSDND_DISPLAY_AUDIO_KEY_BAR_HEIGHT              12
#define OSDND_DISPLAY_AUDIO_KEY_BAR_WIDTH               (OSDND_DISPLAY_AUDIO_KEY_BAR_RANGE*OSDND_DISPLAY_AUDIO_KEY_BAR_WIDTH_PER_RANGE)

//define the distance between the bar the the font
#define OSDND_DISTANCE_FONT_AUDIO_KEY_BAR               10

//define the size of the Echo bar
#define OSDND_DISPLAY_ECHO_BAR_RANGE                    8
#define OSDND_DISPLAY_ECHO_BAR_WIDTH_PER_RANGE          10
#define OSDND_DISPLAY_ECHO_BAR_HEIGHT                   12
#define OSDND_DISPLAY_ECHO_BAR_WIDTH                    (OSDND_DISPLAY_ECHO_BAR_RANGE*OSDND_DISPLAY_ECHO_BAR_WIDTH_PER_RANGE)

//define the distance between the bar the the font
#define OSDND_DISTANCE_FONT_ECHO_BAR                    10

//CoCo1.10, support progress bar
//define the size of the progress bar
#define OSDND_DISPLAY_PROGRESS_BAR_WIDTH                200
#define OSDND_DISPLAY_PROGRESS_BAR_HEIGHT               12

//define the distance between the bar the the font
#define OSDND_DISTANCE_FONT_PROGRESS_BAR                8

//define the zoom rectangle
#define OSDND_DISPLAY_ZOOM_PIP_START_H                  500
#define OSDND_DISPLAY_ZOOM_PIP_START_V                  240 //300
#define OSDND_DISPLAY_ZOOM_PIP_WIDTH                    120
#define OSDND_DISPLAY_ZOOM_PIP_HEIGHT                   80
#define OSDND_DISPLAY_ZOOM_PIP_BORDER_THICKNESS         4
#define OSDND_DISPLAY_ZOOM_PIP_COLOR_WEIGHT             0x3F
#define OSDND_DISPLAY_ZOOM_PIP_COLOR_Y                  0x97
#define OSDND_DISPLAY_ZOOM_PIP_COLOR_U                  0x28
#define OSDND_DISPLAY_ZOOM_PIP_COLOR_V                  0xCB
#define OSDND_DISPLAY_ZOOM_OSD_MOVE_COLOR               OSDND_PALETTE_ENTRY_ZOOM_COLOR
#define OSDND_DISPLAY_ZOOM_OSD_TUNE_NTSC_START_H        58
#define OSDND_DISPLAY_ZOOM_OSD_TUNE_NTSC_START_V        26
#define OSDND_DISPLAY_ZOOM_OSD_TUNE_PAL_START_H         58
#define OSDND_DISPLAY_ZOOM_OSD_TUNE_PAL_START_V         36
#define OSDND_DISPLAY_ZOOM_OSD_TUNE_CLEAR_REGION        20

#ifdef NO_PIP
#define OSDND_DISPLAY_ZOOM_BORDER_THICKNESS             4
#define OSDND_DISPLAY_ZOOM_START_H                      540
#define OSDND_DISPLAY_ZOOM_START_V                      232
#define OSDND_DISPLAY_ZOOM_WIDTH                        60//120
#define OSDND_DISPLAY_ZOOM_HEIGHT                       40//80
#endif    
                                              
//Export APIs
BYTE OSDND_GetMessagePos(BYTE bMessage);
BYTE OSDND_Update(BYTE bMessage, WORD wParam);
void OSDND_ClearRegion(BYTE bRegion);
void OSDND_ConfigOSDRegion(void);

//Internal APIs
void _OSDND_ShowUpperRightMessage(void);
void _OSDND_OutputUpString(void);
void _OSDND_ShowBottomMessage(void);
void _OSDND_ShowDiscType(void);
void _OSDND_OutputKeyString(void);
void _OSDND_ShowNext(void);
void _OSDND_ShowPrevious(void);
void _OSDND_ShowRepeatState(void);
void _OSDND_ShowRepeatABState(void);
void _OSDND_ShowPBC(void);
void _OSDND_ShowTVMode(void);
void _OSDND_ShowZoomState(void);
void _OSDND_ShowVideoOutput(void);
void _OSDND_ShowVocal(void);
void _OSDND_ShowKaraoke(void);
void _OSDND_ShowJPEGEffect(void);
void _OSDND_ShowSlideTransitionTime(void);
void _OSDND_ShowPlayAction(void);
void _OSDND_OutputErrorDisc(void);
void _OSDND_Show10Plus(void);
void _OSDND_ShowSelection(void);
void _OSDND_ShowTime(void);
void _OSDND_ShowDebugNum(void);
void _OSDND_ShowAudio(void);
void _OSDND_ShowVolume(void);
void _OSDND_ShowAudioOutput(void);
void _OSDND_ShowAudioKeyControl(void);
void _OSDND_ShowEcho(void);
void _OSDND_ShowSurround(void);
void _OSDND_Show3D(void);
void _OSDND_ShowAngle(void);
void _OSDND_ShowAST(void);
void _OSDND_CodeToLanguage(WORD wCode);
void _OSDND_ShowSPST(void);
void _OSDND_DefaultState(void);
void _OSDND_InitializeGDI(void);
void _OSDND_ShowWaitingState(void);
void _OSDND_OutputAudioType(BYTE bAudioType);
void _OSDDND_OutputAudioChannel(BYTE bAudioChannel);
void _OSDND_ShowString(void);
void _OSDND_ShowZoom(void);
void _OSDND_ShowProgressBar(void); //CoCo1.10, support progress bar

//CoCo.CT909, add it for like issue. Will remove it in the future.
void OSD_OutputNum(WORD wNum, BYTE bNumIndex);
extern BYTE __bOSDFg;
extern BYTE __bOSDBg;
extern WORD __wOSDUpVPos;
extern WORD __wOSDUpHPos;
extern WORD __wOSDBottomVPos;
extern WORD __wOSDBottomHPos;
extern WORD aNDNumbers[];

//The following APIs are OSD common utility
//Export APIs
void OSDUTL_PrepareOutputString(void);
void OSDUTL_OutputNumFromIndex(WORD wNum);
void OSDUTL_CopyStringFromIndex(WORD *pString);
void OSDUTL_ShowHourMinSec(void);
WORD OSDUTL_TransformChar(char chCode);
void OSDUTL_ShowJPEGDate(WORD wDirID, WORD wFileIndex);
void OSDUTL_ShowJPEGResolution(void);

//Internal APIs
void _OSDUTL_ShowTimeNum(BYTE bNum);

//extern some variables for OSD common usage.
extern BYTE __bOSDUTLIndex;
extern WORD *__pOSDUTLOutputString;
extern WORD __wOSDUTLStringBuf[OSDUTL_STRING_LENGTH+1];
extern PARM_DRAW_STRING __OSDNDUpMsgAttr;
extern PARM_DRAW_STRING __OSDNDBottomMsgAttr;

//CoCo1.07, the variables for displaying progress bar.
extern WORD __wProgressTotal;
extern WORD __wProgressCurrent;

#endif //__OSDND_H__
