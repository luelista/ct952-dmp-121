
/**************************************************************************

        (C)Copyright Cheertek Inc. 2004-2005,
           D300, all right reserved.

        Product : WinAV Firmware

        Date    : 2004.08.30.
        Author  : Cheertek (D300 Alex Hsia)
        Purpose : Search  module
                  The moudle will control Search and Goto Time
        Sources : OSDSRCH.h/ OSDSRCH.c

****************************************************************************/

#ifndef __OSDSRCH_H__
#define __OSDSRCH_H__

#ifdef __cplusplus
        extern  "C"     {
#endif
/***********************************/
//          DEFINE
/***********************************/
// 1.BYTE		__bSearchMode ,use
// define for search stage switch
#define  SEARCH_MODE_BEGIN            0                         //replace SEARCH_MODE_BEGIN
#define  SEARCH_MODE_OFF              (SEARCH_MODE_BEGIN+0)     //replace SEARCH_MODE_OFF
#define  SEARCH_MODE_TITLE_CHAPTER    (SEARCH_MODE_BEGIN+1)     //replace SEARCH_MODE_TITLE_CHAPTER
#define  SEARCH_MODE_TIME             (SEARCH_MODE_BEGIN+2)     //replace SEARCH_MODE_TIME
#define  SEARCH_MODE_END              SEARCH_MODE_TIME          //replace SEARCH_MODE_END


// 2.BYTE       __bSearchCursor
//define for cursor position in search mode
#define  SEARCH_CURSOR_BEGIN        1                               //replace SEARCH_CURSOR_BEGIN
#define  SEARCH_CURSOR_TITLE        (SEARCH_CURSOR_BEGIN+0)         //replace SEARCH_CURSOR_TITLE
#define  SEARCH_CURSOR_CHAPTER      (SEARCH_CURSOR_BEGIN+1)         //replace SEARCH_CURSOR_CHAPTER
#define  SEARCH_CURSOR_TIME         (SEARCH_CURSOR_BEGIN+2)         //replace SEARCH_CURSOR_TIME
#define  SEARCH_CURSOR1_END         SEARCH_CURSOR_CHAPTER           //replace SEARCH_CURSOR1_END
#define  SEARCH_CURSOR2_END         SEARCH_CURSOR_TIME              //replace SEARCH_CURSOR2_END


// 3.BYTE Add temp OSD_UI let system can work
//#define  OSD_UI_EXIT_ALL            1
//#define  OSD_UI_EXIT_CONTROL_ONLY   2         
#define FM_MAX_NUMBER                   9990


/***********************************/
//          Variable
/***********************************/


BYTE		__bSearchMode;		//  use it replace __bSearchMode
WORD		__wSearchChapter;           //  use it replace __wSearchChapter
WORD		__wSearchChapter_Ns_Title;  //  use it replace __wSearchChapter_Ns_Title
BYTE	 	__bSearchTitle;	            //  use it replace __bSearchTitle
DWORD	    __bOSDSRCH_SearchTime;	    //  
BYTE		__bTimeHour;	            //  use it replace __bTimeHour;
BYTE		__bTimeMin;	                //  use it replace __bTimeMin;
BYTE		__bTimeSec;	                //  use it replace __bTimeSec;
BYTE		__bSearchCursor;	            //  use it replace __bSearchCursor
WORD        __wNumberQueue;

/***********************************/
//          Function
/***********************************/
BYTE OSDSRCH_ProcessKey(void);
void OSDSRCH_Exit(BYTE bExitMode);
void OSDSRCH_ClearRegion (void);
BYTE OSDSRCH_Recover(BYTE bRecoverRegion);
void OSDSRCH_ConfigOSDRegion(void);
BYTE OSDSRCH_ProcessSearchAction(BYTE bKey, WORD wParam);




#define OSDSRCH_COLOR_BRIGHT               0xFFFFFF     // white
#define OSDSRCH_COLOR_DARK                 0x202020 
#define OSDSRCH_COLOR_BLACK                0x010101    // Brian1,07, use a darker color, so FM Active item won't "blur" //0x101010 

#define OSDSRCH_COLOR_CENTER1              0x0000C0
#define OSDSRCH_COLOR_CENTER2              0x7BBEB5
#define OSDSRCH_COLOR_CENTER3              0x46BCE9
#define OSDSRCH_COLOR_CENTER4              0xF7D649

#define OSDSRCH_VALUE_COLOR_ICON_FG1       0x010101 
#define OSDSRCH_VALUE_COLOR_ICON_FG2       0xFE5800 
#define OSDSRCH_VALUE_COLOR_ICON_FG3       0x4E0CA2 
#ifdef  SIMPLE_SEARCH_UI
#define OSDSRCH_VALUE_COLOR_ICON_FG4       0xF0F0F0
#else
#define OSDSRCH_VALUE_COLOR_ICON_FG4       0x261E8D 
#endif

#define OSDSRCH_VALUE_COLOR_ICON_BG1       0xC0C0C0
#define OSDSRCH_VALUE_COLOR_ICON_BG2       0xD9EAC8
#define OSDSRCH_VALUE_COLOR_ICON_BG3       0xB9ECEC

#ifdef  SIMPLE_SEARCH_UI
#define OSDSRCH_VALUE_COLOR_ICON_BG4       0xC060C0
#else
#define OSDSRCH_VALUE_COLOR_ICON_BG4       0xF8E7E1
#endif

#define OSDSRCH_VALUE_COLOR_WORK_TEXT_BG1  0xFFFF00 
#define OSDSRCH_VALUE_COLOR_WORK_TEXT_BG2  0xFFFF00 
#define OSDSRCH_VALUE_COLOR_WORK_TEXT_BG3  0xFFFF00 
#ifdef  SIMPLE_SEARCH_UI
#define OSDSRCH_VALUE_COLOR_WORK_TEXT_BG4  0x6666FF
#else
#define OSDSRCH_VALUE_COLOR_WORK_TEXT_BG4  0x95E4FF 
#endif



#ifdef  SIMPLE_SEARCH_UI
#define OSDSRCH_ENTRY_COLOR_START           (GDI_ND_PALETTE_INDEX_START+GDI_ND_PALETTE_NUM)
#else
#define OSDSRCH_ENTRY_COLOR_START           GDI_GENERAL_UI_PALETTE_INDEX_START
#endif 

#define OSDSRCH_ENTRY_COLOR_FRAME_CENTER    (OSDSRCH_ENTRY_COLOR_START)
#define OSDSRCH_ENTRY_COLOR_ICON_FG         (OSDSRCH_ENTRY_COLOR_START + 1)
#define OSDSRCH_ENTRY_COLOR_ICON_BG         (OSDSRCH_ENTRY_COLOR_START + 2)
#define OSDSRCH_ENTRY_COLOR_WORK_TEXT_BG    (OSDSRCH_ENTRY_COLOR_START + 3)


#ifdef  SIMPLE_SEARCH_UI
#define OSDSRCH_ENTRY_COLOR_WORK_TEXT_FG     OSDSRCH_ENTRY_COLOR_ICON_FG
#define OSDSRCH_ENTRY_COLOR_STILL_TEXT_FG    OSDSRCH_ENTRY_COLOR_ICON_BG
#else
#define OSDSRCH_ENTRY_COLOR_WORK_TEXT_FG    (OSDSRCH_ENTRY_COLOR_START + 4)
#define OSDSRCH_ENTRY_COLOR_FRAME_BRIGHT    (OSDSRCH_ENTRY_COLOR_START + 5)
#define OSDSRCH_ENTRY_COLOR_FRAME_DARK      (OSDSRCH_ENTRY_COLOR_START + 6)
#define OSDSRCH_ENTRY_COLOR_STILL_TEXT_FG   (OSDSRCH_ENTRY_COLOR_START + 7)
#endif


#define OSDSRCH_VALUE_COLOR_FRAME_BRIGHT    OSDSRCH_COLOR_BRIGHT
#define OSDSRCH_VALUE_COLOR_FRAME_DARK      OSDSRCH_COLOR_DARK
#define OSDSRCH_VALUE_COLOR_WORK_TEXT_FG    OSDSRCH_COLOR_BLACK
#define OSDSRCH_VALUE_COLOR_STILL_TEXT_FG   OSDSRCH_COLOR_BRIGHT
#define OSDSRCH_VALUE_COLOR_STILL_TEXT_BG   OSDSRCH_COLOR_GENERAL_BG_BLUE


#ifdef __cplusplus
        }
#endif
#endif  // __OSDSRCH_H__
