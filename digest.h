
/**************************************************************************

        (C)Copyright Cheertek Inc. 2002-2003,
           D300, all right reserved.

        Product : VCD25 Firmware

        Date    : 1997.1.11
        Author  : Cheertek (D300 TCH)
        Purpose : DIGEST module
                  The moudle will control the flow of showing digest
        Sources : digest.h/ digest.c

****************************************************************************/

#ifndef __DIGEST_H__
#define __DIGEST_H__

#ifdef __cplusplus
        extern  "C" {
#endif

//LJY0.76, use different vertical param for PAL and NTSC digest frame display
//DIGEST frame position parameters
#define DIGEST_FIRST_FRAME_POS_H        (96)
#define DIGEST_FIRST_FRAME_POS_V_480    (80)
#define DIGEST_FIRST_FRAME_POS_V_576    (80)
#define DIGEST_FRAME_SIZE_H             (160)
#define DIGEST_FRAME_SIZE_V_480         (128)
#define DIGEST_FRAME_SIZE_V_576         (160)
#define DIGEST_FRAME_SPACING_H          (16)
#define DIGEST_FRAME_SPACING_V_480      (16)
#define DIGEST_FRAME_SPACING_V_576      (24)

#define DIGEST_FRAME_COUNT_PER_ROW  (3)
#define DIGEST_FRAME_COUNT          (6)  //show 6 digest frames for each page

//DIGEST UI mode
#define DIGEST_MODE_NULL				0
#define DIGEST_MODE_DIALOG              1
#define DIGEST_MODE_TRACK               2
#define DIGEST_MODE_DISCINTERVAL        3

//DIGEST UI Button number
#define DIGEST_MENU1_BUTTON_NO     2 //dialog menu
#define DIGEST_MENU2_BUTTON_NO     2 //display menu


#define DIGEST_FRAME_PLAYTIME     (COUNT_10_SEC)
#define DIGEST_DISCVIEW_TIMEINTERVAL    (45000)// 75*60*10, 10 minutes

typedef struct  tagDigestUI
{
    BYTE    bButtonEnable[2];
    BYTE    bButtonIndex; //for button highlight update
    BYTE    bCurPageNo; //current page number
    BYTE    bTotalPageNo; //total page number
    BYTE    bCurTrackNo;  //the number of current track
    BYTE    bCurFrameNo; // the index of current frame
    BYTE    bDisplayedFrameNo; //the number of displayed frames.
    BYTE    bDigestMode; //record the digest mode
} DIGESTUI, *PDIGESTUI;



//LJY1.20, support 4M flash, remove digest
#ifdef NO_DIGEST
#define NO_CDDADIGEST
#define NO_DVDDIGEST
#endif

#ifndef NO_DIGEST

//LJY0.86, background color of digest mode
#define DIGEST_BACKGROUND_COLOR     0x00008080 //black

// Definitions of DIGEST style
// Rule: Digest Style need between DIGEST_STYLE_BEGIN and DIGEST_STYLE_END
#define     DIGEST_STYLE_BEGIN          1

// overview/track[Grid 4/ Track 4]+ Audio
#define     DIGEST_STYLE_NORMAL4        DIGEST_STYLE_BEGIN+ 0

// overview/track[Grid 9/ Track 9]+ Audio
#define     DIGEST_STYLE_NORMAL         DIGEST_STYLE_BEGIN+ 1

// Divide the disc into 9 parts, NO Audio
#define     DIGEST_STYLE_DISCVIEW       DIGEST_STYLE_BEGIN+ 2

// Overview/track[Random]+ Audio+ Round-Run track
#define     DIGEST_STYLE_RANDOM         DIGEST_STYLE_BEGIN+ 3 // *** 0.45;

#define     DIGEST_STYLE_END            DIGEST_STYLE_BEGIN+ 3   // *** 0.45;

#define     DIGEST_STYLE_TRACK          DIGEST_STYLE_BEGIN+ 4   // *** 0.45;

// WYC.100 follow are defined for DVD digest
#define     DIGEST_STYLE_CHAPXX     6 //10 LJY2.81, redefine the value
#define     DIGEST_STYLE_TITLE      7 //11
#define     DIGEST_STYLE_CHAP       8 //12

// Following mode are discarded
// overview one frame/track
#define     DIGEST_STYLE_TRACKVIEW      DIGEST_STYLE_BEGIN+ 10// 21

// overview/track[Grid 9/ Track 9]+ Audio+ Round-Run track
#define     DIGEST_STYLE_ROUND          DIGEST_STYLE_BEGIN+ 11 //22

// Overview /track[Grid 9/ Track 1]+ Audio+ Round-Run track
#define     DIGEST_STYLE_FREEZE         DIGEST_STYLE_BEGIN+ 12 //23   // *** 0.45;

//only NORMAL/TRACKVIEW has pages
// It's a definition for distinguish PAGE-Avaiable mode
// The style value <= DIGEST_STYLE_PAGE means it's Page mode
#define     DIGEST_STYLE_PAGE           DIGEST_STYLE_NORMAL
//LJY278.2nd, freeze digest during setup operation for Mustek.
#define     DIGEST_IN_SETUP             0x80
//LJY2.81, for DVCD discview in stop mode
#define     DIGEST_DVCD_DISCVIEW       0x40

#define     DIGEST_FRAME_4              1
#define     DIGEST_FRAME_9              0
// wyc.100 define for NO DVD and NO CDDA digest
#define NO_CDDADIGEST
#define NO_DVDDIGEST
#define PRE_DVDDIGEST
////////////////////////////////////////
//BIT         DIGEST_Trigger (void);
void         DIGEST_Trigger (void); //jyliu.pc908
// WinVCD
void        DIGEST_Digest2Linear(void);
BYTE         DIGEST_Number(DWORD dwNumber);
void        DIGEST_CheckHang(void);
// wyc.100
void        DIGEST_PreDVDDigest(void);
//////////////////////////////////////////////
// protected functions
void   _DIGEST_DisplayFrame (void);

void        _DigestPlayInit (void);
void DIGEST_VarInit(void); //LJY1.10
BYTE DIGEST_ProcessKey(void);
void _DigestFrmPosInit(void);
void DIGEST_DigestExit(void);




#define DIGEST_ENTRY_START              GDI_GENERAL_UI_PALETTE_INDEX_START
#define DIGEST_ENTRY_COLOR_BRIGHT_FRAME (DIGEST_ENTRY_START)
#define DIGEST_ENTRY_COLOR_DARK_FRAME   (DIGEST_ENTRY_START + 1)

#define DIGEST_VALUE_COLOR_BRIGHT_FRAME 0xFFFFFF  // need mix enable
#define DIGEST_VALUE_COLOR_DARK_FRAME   0x888888  // need mix enable

////////////////////////////////
#endif
#ifdef __cplusplus
        }
#endif

#endif  // __DIGEST_H__

