
#include    "winav.h"
#include    "input.h"
#include    "utl.h"
#include    "osd.h"
#include    "hal.h"
#include    "cc.h"
#include    "infofilter.h"
#include    "linear.h"
#include    "navigate.h"
#include    "cell.h"
#include    "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include    "osdbmark.h"
#include    "osdprgm.h"
#include    "osddsply.h"
#include    "osdnd.h"
#include    "setup.h"
#include    "disp.h"
#include "notransl.h"

#ifdef SUPPORT_BOOKMARK
#ifndef NO_DISC_MODE //CoCo2.37p

#define OSDBMARK_MAIN_FILE
#include "OSDString\\strextrn.h"

extern  BYTE    __bTimeHour;
extern  BYTE    __bTimeMin;
extern  BYTE    __bTimeSec;
extern  BYTE    __bGoToTimeNO;

// Need to add "extern" in the future, use OSDPRGM insteadly
// Note : The number in seconde element must be the same as the osdstr.h
/*
extern WORD aTitle[][7];
extern WORD aChapter[][10];
extern WORD aClear[][8];
*/
extern WORD aTitle[];
extern WORD aChapter[];
extern WORD aClear[];

extern WORD aColon[];
extern WORD aPlayIcon[];
extern WORD aArrowDown[];
extern WORD aArrowUp[]; 
extern WORD aClearIcon[];

extern PARM_FRAME       _PRGM_FrameAttr;
extern PARM_DRAW_STRING _PRGM_StrAttr;
extern PARM_RECT        _PRGM_RectAttr;


//#define SUPPORT_PRGM_NUM_INPUT //Kevin1.06 //Kevin1.24, move to customer.h
//#define SUPPORT_CLEARALL_IN_PROGRAM
//#define SUPPORT_CIRCULAR_WRAP   //Kevin1.05, add

#ifdef SUPPORT_CLEARALL_IN_PROGRAM
#define MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME       7
#else
#define MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME       7 
#endif

// Brian1.24, this value will offset the entire Program UI
#define BOOMKARK_GLOBAL_OFFSET     (20)


#define ENTRY_HEIGHT        26 
#define FRAME_THICKNESS     2  
#define ARROW_HEIGHT        22    

#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM

#define BOOKMARK_WIDTH       560 
//#define BOOKMARK_HEIGHT      410
#define BOOKMARK_HEIGHT     350

#define BMARK_FRAME_X0      (BOOMKARK_GLOBAL_OFFSET) // Brian1.24
//#define BMARK_FRAME_Y0	    (0)  
#define BMARK_FRAME_Y0	    (OSDND_UPPER_BACKGROUND_BOTTOM + (8 - OSDND_UPPER_BACKGROUND_BOTTOM%8))
#define BMARK_FRAME_X1      (BOOKMARK_WIDTH + BMARK_FRAME_X0) 
#define BMARK_FRAME_Y1      (BOOKMARK_HEIGHT + BMARK_FRAME_Y0)     
#else
#define BMARK_FRAME_X0      (50+BOOMKARK_GLOBAL_OFFSET) // Brian1.24
#define BMARK_FRAME_Y0	    (OSDND_UPPER_BACKGROUND_BOTTOM + 1)  
#define BMARK_FRAME_X1      (590+BOOMKARK_GLOBAL_OFFSET) // Brian1.24
#define BMARK_FRAME_Y1      (OSDND_BOTTOM_BACKGROUND_START_V - 1)     
#endif

#define CENTER_FRAME_X0     BMARK_FRAME_X0
#define CENTER_FRAME_Y0     (BMARK_FRAME_Y0 + 41)
#define CENTER_FRAME_X1     BMARK_FRAME_X1
#define CENTER_FRAME_Y1     (BMARK_FRAME_Y1 - 41)

#define FRAME_Y0            (BMARK_FRAME_Y0 + 3 * ENTRY_HEIGHT + 5)

#define MAIN_FRAME_X0       (80+BOOMKARK_GLOBAL_OFFSET) // Brian1.24
#define MAIN_FRAME_Y0       FRAME_Y0
#define MAIN_FRAME_X1       (320+BOOMKARK_GLOBAL_OFFSET) // Brian1.24 //390
#define MAIN_FRAME_Y1       MAIN_FRAME_Y0+MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME*ENTRY_HEIGHT+2*FRAME_THICKNESS     

#define CLEAR_HIGHLIGHT		0
#define SET_HIGHLIGHT		1


WORD _wOSDBOOKMARKString[8];
WORD _wBookMark_Chapter[BOOKMARK_NO + 1];


//Internal Functions
static void _ShowMainList(void);
static void _ShowOneEntry(WORD wEntry);
static void _ShowOneEntryNumber(WORD wNumber);
static void _ShowTitleChapterTime(void);
extern void _InitRect(PURECT pRect, WORD wLeft, WORD wTop, WORD wRight, WORD wBottom); //--CoCo1.20, reduce code size.

static void _SetClearHighlight(BYTE bFlag);

void _OSDBOOKMARK_ShowUI(void);
void _OSDBOOKMARK_ClearUI(void);
void _OSDBOOKMARK_DrawFrames(void);
void _OSDBOOKMARK_InitPalette(void);

void _OSDBOOKMARK_ProceedToNextMainEntry(void); //Kevin1.23DMTech

//show entry from
extern BYTE    _bMainStart; 
    
//current pos 
extern BYTE    _bMainHighlightPos; 


extern BYTE  __bTemp;
BYTE    _bOSDBOOKMARKTemp, _bOSDBOOKMARKTemp1; //for temp use


//  ****************************************************************************
//  Function    :   OSDBOOKMARK_Exit
//  Description :   The OSD kernel will call this function to exit normally.
//		:   This function can NOT send OSD_ChangeUI(xxx, OSD_EXIT_UI) to
//		:   notify OSD kernel it exits.
//  Arguments   :   bExitMode
//                  OSD_UI_EXIT_ALL		: Exit control and clear the UI.
//                  OSD_UI_EXIT_CONTROL_ONLY	: Exit control only.
//  Return      :   
//  Side Effect :
//  ****************************************************************************
void OSDBOOKMARK_Exit(BYTE bExitMode)
{
    switch (bExitMode)
    {
    case OSD_UI_EXIT_ALL:
        _OSDBOOKMARK_ClearUI();
    case OSD_UI_EXIT_CONTROL_ONLY:
        __wBookMark[0] = 0;
        __btBookMark = FALSE;
        break;
    default:
        break;
    }
}



//  ****************************************************************************
//  Function    :   OSDBOOKMARK_ProcessKey
//  Description :   Process the input key while in DVD/VCD Program mode
//  Arguments   :   
//  Return      :   KEY_NO_KEY  , the input key is processed
//              :   INVALID_KEY , the input key is not allowed under this mode
//              :   KEY_xxx     , main program needs to do more processing for the key.
//  Side Effect :
//  ****************************************************************************
BYTE OSDBOOKMARK_ProcessKey(void)
{
    BYTE bKeyStatus = INVALID_KEY;
    DWORD dwCheckStill;

    if ((__wDiscType & BOOK_PICTURECD) ||
        (__bKeySource != KEY_FROM_ISR)) return KEY_BYPASS;

    if (! ((__bKey == KEY_BOOKMARK) || __btBookMark) )
    {
        return KEY_BYPASS;
    }


    bKeyStatus = INVALID_KEY;

    switch (__bKey)
    {
		///// Enter the following cases when in bookmark mode /////
    case KEY_TVMODE:
    case KEY_MUTE:
    case KEY_VOL_UP:
    case KEY_VOL_DOWN:
    case KEY_KEYUP:
    case KEY_KEYDOWN:
    case KEY_PROGRAM:
    case KEY_GOTOTIME:
    case KEY_NO_KEY:
        bKeyStatus = KEY_BYPASS;
        break;

    case KEY_UP:
        __wBookMark[0]--;
        if (__wBookMark[0] <= BOOKMARK_NO && __wBookMark[0] >= 1)
        {
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            if (_bMainHighlightPos>1)
                _bMainHighlightPos--; 
            else
            {
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
                if (_bMainStart>0)
#else
                if (_bMainStart>1)
#endif
                {   
                    _bMainStart--;
                    _ShowMainList(); 
                }    
            }                                
            _SetClearHighlight(SET_HIGHLIGHT);
        }

        if ( __wBookMark[0] >BOOKMARK_NO)
            __wBookMark[0]=BOOKMARK_NO;
        if ( __wBookMark[0] <1)
            __wBookMark[0]=1;

        _ShowTitleChapterTime();

        bKeyStatus = KEY_NO_KEY;

        break;
		
		
    case KEY_DOWN:
        __wBookMark[0]++;
        if (__wBookMark[0] <= BOOKMARK_NO && __wBookMark[0] >= 1)
        {
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            _OSDBOOKMARK_ProceedToNextMainEntry();
            _SetClearHighlight(SET_HIGHLIGHT);
        }

        if ( __wBookMark[0] >BOOKMARK_NO)
            __wBookMark[0]=BOOKMARK_NO;
        if ( __wBookMark[0] <1)
            __wBookMark[0]=1;

        _ShowTitleChapterTime();

         bKeyStatus = KEY_NO_KEY;
        break;
		
		
		//Kevin1.05-3, page down
    case KEY_NEXT:
        _SetClearHighlight(CLEAR_HIGHLIGHT);
		
        if (_bMainStart-1+2*MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME<=MAX_PROG_ITEM) //scroll down a whole page
        { 
            _bMainStart += MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME; 
//            __wBookMark[0] += MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME;
            //_bMainHighlightPos remains the same
            _ShowMainList();
        }
        else if (_bMainStart+_bMainHighlightPos-1+MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME<=MAX_PROG_ITEM) //scroll down not a whole page
        {   
            _bMainStart=MAX_PROG_ITEM-MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME+1;
            // _bMainHighlightPos remains the same 
            //_bMainHighlightPos = _bMainStart+_bMainHighlightPos-1+MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME-_bMainStart+1;				
            _ShowMainList();
        } 
        else if (_bMainStart+MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME-1<MAX_PROG_ITEM) //scroll down not a whole page
        {
            _bMainStart=MAX_PROG_ITEM-MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME+1;
            // _bMainHighlightPos remains the same
            //_bMainHighlightPos =MAX_PROG_ITEM-_bMainStart+1;   //last one
            _ShowMainList();
        } 
#ifdef SUPPORT_CIRCULAR_WRAP 
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
            else if (_bMainStart!=0) //avoid unnecessary redraw
            {
                _bMainStart = 0; 
                _bMainHighlightPos = 2; //highlight 1. __-__  
#else
            else if (_bMainStart!=1) //avoid unnecessary redraw
            {
		_bMainStart = 1; 
		_bMainHighlightPos = 1; //highlight 1. __-__ 
#endif 
		_ShowMainList();
            }
#endif
			
        __wBookMark[0] = _bMainStart + _bMainHighlightPos - 1;
	_SetClearHighlight(SET_HIGHLIGHT);
        _ShowTitleChapterTime();
	bKeyStatus = KEY_NO_KEY;
	break;
			
			
			//Kevin1.05-3, page up
    case KEY_PREV:
        _SetClearHighlight(CLEAR_HIGHLIGHT);
		
        if (_bMainStart>MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME) //scroll up a whole page
        { 
            _bMainStart -= MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME; 
//            __wBookMark[0] -= MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME;
            //_bMainHighlightPos remains the same
            _ShowMainList();
        }
        else if (_bMainStart+_bMainHighlightPos-1>MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME) //scroll down not a whole page
        {
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
            _bMainStart=0;
#else
            _bMainStart=1;
#endif
            //_bMainHighlightPos = _bMainStart+_bMainHighlightPos-1-MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME;   
            _ShowMainList();
        }
        else if (_bMainStart!=1) //scroll down not a whole page
        {
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
            _bMainStart=0;
            _bMainHighlightPos=2;   //first one
#else
            _bMainStart=1;
            _bMainHighlightPos=1;   //first one
#endif
            _ShowMainList();
        } 
#ifdef SUPPORT_CIRCULAR_WRAP 
        else if (MAX_PROG_ITEM>MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME)
        {
            _bMainStart=MAX_PROG_ITEM-MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME+1;
            _bMainHighlightPos=MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME;
            _ShowMainList();
        }
#endif           
			
        __wBookMark[0] = _bMainStart + _bMainHighlightPos - 1;
        _SetClearHighlight(SET_HIGHLIGHT);
        _ShowTitleChapterTime();
        bKeyStatus = KEY_NO_KEY;
        break;
		
		
    case KEY_LEFT:
    case KEY_RIGHT:
        break;
		
		
    case KEY_ENTER: //Kevin1.07, combine all the following keys
    case KEY_PLAY_PAUSE: //Kevin1.06, add
    case KEY_PLAY: //Kevin0.87, KEY_PLAY act as KEY_ENTER for some cases

        //playing time
        if (__wBookMark[__wBookMark[0]]!=0) //playing time
        {
            __bTimeHour= HIBYTE(__wBookMark[__wBookMark[0]]) /60 ;
            __bTimeMin=  HIBYTE(__wBookMark[__wBookMark[0]]) %60 ;
            __bTimeSec=  LOBYTE(__wBookMark[__wBookMark[0]]) ;

            __bTitle=__bBookMark_Title[__wBookMark[0]];

#ifdef SUPPORT_STILLRSM
            //Aron2.38, prepare to bookmark to the still content
            if(__bBookMarkIsStill[__wBookMark[0]] )
            {
                __bStillRSM = TRUE;
                __bModeCELLTimeSearch= CELL_TIME_VOBU;
                __dwTimeCELLTimeSearch= __dwBookMarkVOBU[__wBookMark[0]];                 
            }
#endif            
            __bKey = KEY_GOTOTIME_VALUE;
            __btBookMark=FALSE;
            _OSDBOOKMARK_ClearUI();
            OSD_ChangeUI(OSD_UI_BOOKMARK, OSD_EXIT_UI);
            bKeyStatus = KEY_TODO;
        }
        else //record time
        {
            if ( (__wDiscType & BOOK_DVD) )
            {
                // DVD_099bMicky, must give title parameter for UOP query
                __bUOP_Parameter = __bTitle;
                // must check Time Search
#ifdef SUPPORT_STILLRSM
                // Alex1.24, don't record bookmark time when non-title domain
                if ( ! NV_QueryUOP (UOP_CAN_TIME_SEARCH) || (__bDomain !=TT_DOM) )
#else
                HAL_ReadInfo(HAL_INFO_STILL, &dwCheckStill);                
                // Aron2.37, do not allow doing bookmark when playing still picture
                // Alex1.24, don't record bookmark time when non-title domain
                if ( ! NV_QueryUOP (UOP_CAN_TIME_SEARCH) || (__bDomain !=TT_DOM) || dwCheckStill )
#endif
                {
                    bKeyStatus = INVALID_KEY;
                    break;
                }
            }
            // Alex1.24, don't record bookmark time when segment play item
            else if (__wPlayItem > 999 )    // segment playitem
            {
                bKeyStatus = INVALID_KEY;
                break;
            }

            _SetClearHighlight(CLEAR_HIGHLIGHT);

            //VCD ,CDDA only use ABSTime
            if (__wDiscType !=BOOK_DVD)
            {
                // Micky1.24a, use UTL_EstimateBufferTime
                // LLY2.05, using API to get current decoded time
                __dwTemp = UTL_GetDecodedTime() - __dwTimeDiscBegin;
                /*
                UTL_GetCurrentTime();
                __dwTemp = UTL_EstimateBufferTime();
                __dwTemp = (__dwTimeNow - __dwTimeDiscBegin) - __dwTemp;    // exclude buffer time
                */
                __dwTemp = UTL_SectorstoMSF(__dwTemp);  // convert sectors into time
                __wBookMark[__wBookMark[0]]=MAKEWORD (MSF_SECOND ( __dwTemp ), MSF_MINUTE ( __dwTemp ) );
            }
            else//dvd
            {
                // Micky1.24a, use __wTimeCurrentDisplay for time.
                __dwTemp= __wTimeCellBegin + __wTimeCurrentDisplay;
                __wBookMark[__wBookMark[0]]= MAKEWORD ( __dwTemp%60, __dwTemp/60);

                __bBookMark_Title[__wBookMark[0]]= __bTitle;
                _wBookMark_Chapter[__wBookMark[0]]= __wChapter;
#ifdef SUPPORT_STILLRSM
                //Aron2.38, clear the still flag first
                __bBookMarkIsStill[__wBookMark[0]] = FALSE;
                HAL_ReadInfo(HAL_INFO_STILL, &dwCheckStill);
                if(dwCheckStill)
                {
                    //Aron2.38, Record info for bookmark to still content
                    __bBookMarkIsStill[__wBookMark[0]] = TRUE;
                    __dwBookMarkVOBU[__wBookMark[0]] = __dwCellVideo_VOBUSA;
                }
#endif
            }
            _ShowTitleChapterTime();

            //Kevin1.23DMTech, proceed to next entry when current entry is set
//            _OSDBOOKMARK_ProceedToNextMainEntry();            
            _SetClearHighlight(SET_HIGHLIGHT);
            bKeyStatus = KEY_NO_KEY;
        }

        break;
		
    case KEY_CLEAR:
    case KEY_STOP:
        if (__wBookMark[ __wBookMark[0] ]!=0)
        {
            //remove time
            __wBookMark[ __wBookMark[0] ] = 0;
            __bBookMark_Title[__wBookMark[0]] = 0;                    
            _wBookMark_Chapter[__wBookMark[0]] = 0;                    
            _ShowOneEntry(__wBookMark[0]);
            _ShowTitleChapterTime();
            _SetClearHighlight(SET_HIGHLIGHT);
        }
        bKeyStatus = KEY_NO_KEY;
        break;
        
    case KEY_POWER:
    case KEY_OPEN_CLOSE:
    case KEY_BOOKMARK:
/*
        __bTemp= LOBYTE ( __wDiscType ) & 0x0f;
        if( !( __wDiscType ==BOOK_DVD || __bTemp==BOOK_VCD ) )
        {
            bKeyStatus = INVALID_KEY;
            break;
        }
*/

        if ( __bCDDA_DTS == CDDA_CDG)
        {
            bKeyStatus = INVALID_KEY;
            break;
        }

        if (
#ifndef NO_DIGEST
            !__btDIGEST && !__btPBC &&
#endif
            //alex1.25,20040428,Book can turn on/off on ( PBC on  / Navigator on )
           // !__btPBC && !__bNVPlayMode && __bModeCmd != KEY_STOP
            __bModeCmd != KEY_STOP
           )
        {
            //First Time Enable , can record
            //Second Time , can remove all

            if (!__btBookMark)
            {
                __btBookMark= TRUE;
#ifdef    FORCE_CLEAR_LOGO
                if ( __bModeCmd == KEY_STOP )
                    UTL_SetPlayMode ( MODE_PLAYMOTION );
#endif    // FORCE_CLEAR_LOGO
//alan, dvd_067 for fixing a bug      if (__wBookMark [0]==0) __wBookMark [0]=1;

#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
                    //OSDPRGM_EnterByPrestop();
                    CC_ForceStop(CC_FORCE_STOP_FROM_BOOKMARK);
#endif
                OSD_ChangeUI(OSD_UI_BOOKMARK, OSD_ENTER_UI);
                _OSDBOOKMARK_ShowUI();
            }
            else
            {
                __btBookMark=FALSE;
                OSDBOOKMARK_Exit(OSD_UI_EXIT_ALL);
                OSD_ChangeUI(OSD_UI_BOOKMARK, OSD_EXIT_UI);
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
                //OSDPRGM_ExitByResume();
                CC_ForceResumeRePlay(CC_FORCE_STOP_FROM_BOOKMARK);
#endif
            }
            bKeyStatus = KEY_NO_KEY;
        }
        else
        {
            bKeyStatus = INVALID_KEY;
        }
        break;

    case KEY_SETUP:
        break;
		
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06  
		//process number keys 0..9 (except 10+..)
    case KEY_N0:
    case KEY_N1:
    case KEY_N2:
    case KEY_N3:
    case KEY_N4:
    case KEY_N5:
    case KEY_N6:
    case KEY_N7:
    case KEY_N8:
    case KEY_N9:  
            bKeyStatus = KEY_NO_KEY;
        break;
#endif
		
		//Kevin0.91,doesn't allow other keys
    default: 
        bKeyStatus = KEY_INVALID;
        break;
    }
	

    return bKeyStatus;

	
} //end of OSDBOOKMARK_ProcessKey



//  *********************************************************************************
//  Function    :   OSDBOOKMARK_Recover
//  Description :   The function is called by OSD to do the recovery of Program UI 
//  Arguments   :   bRecoverRegion, the recover region 
//		:   OSD_RECOVER_ALL 
//		:   OSD_RECOVER_UPPER_REGION, invalid
//		:   OSD_RECOVER_BOTTOM_REGION, invalid
//  Return      :   TRUE, The region has been recovered.
//              :   FALSE, The region doesn't need to be recovered in this UI.
//  Side Effect :
//  *********************************************************************************
BYTE OSDBOOKMARK_Recover(BYTE bRecoverRegion)
{
    if (OSD_RECOVER_ALL != bRecoverRegion) return FALSE;
    _OSDBOOKMARK_ShowUI();
    return TRUE;
}


//  ***************************************************************************
//  Function    :   OSDBOOKMARK_ConfigOSDRegion
//  Description :   This function will be called by OSD module when entering Bookmark UI
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void OSDBOOKMARK_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;
#ifdef  BORROW_FRAME_BUFFER
    DWORD dwBufferAddr;
#endif

    RegionInfo.bColorMode = GDI_OSD_8B_MODE;
    RegionInfo.wWidth = GDI_REGION_WIDTH;

#ifdef ENLARGE_OSD_FOR_PAL_MODE
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
    if ((__bTVType == SETUP_SCREEN_TV_SYSTEM_NTSC) || (__bTVType == SETUP_SCREEN_TV_SYSTEM_PAL_M))
    {
        RegionInfo.wHeight = GDI_REGION_HEIGHT;
    }
    else
    {
        RegionInfo.wHeight = GDI_REGION_HEIGHT_PAL;
    }
#else
#ifdef  BORROW_FRAME_BUFFER
    if (BMARK_FRAME_X1 <= __RegionList[0].wWidth  &&
        BMARK_FRAME_Y1 <= __RegionList[0].wHeight &&
        GDI_OSD_8B_MODE == __RegionList[0].bColorMode)
    {
        // Use OSD buffer and Frame buffer 0
        RegionInfo.dwTAddr = DS_OSDFRAME_ST;
    }
    else
    {
        // Use Frame buffer 1 or 2
        dwBufferAddr = DISP_QueryAvailableBuffer();
        if (0 == dwBufferAddr)
        {
            RegionInfo.dwTAddr = DS_OSDFRAME_ST;
            printf("DISP can't find an available buffer for OSD region\n");
        }
        else
        {
            RegionInfo.dwTAddr = dwBufferAddr;
        }
    }
    RegionInfo.wHeight = BMARK_FRAME_Y1;
#else
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
    RegionInfo.wHeight = GDI_REGION_HEIGHT;
#endif  // #ifdef  BORROW_FRAME_BUFFER
#endif  // #ifdef ENLARGE_OSD_FOR_PAL_MODE

/*
    RegionInfo.wWidth = BMARK_FRAME_X1;
    RegionInfo.wHeight = BMARK_FRAME_Y1;
*/

#ifdef  BORROW_FRAME_BUFFER
    GDI_ClearRegion(0); //Alan2.30, fix mantis bug #1886
    OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    OSD_SetRegion(0, FALSE, &RegionInfo);
#endif

    /* --CoCo2.30
    GDI_DeactivateRegion();
    GDI_ConfigRegionInfo(0, &RegionInfo);
    GDI_InitialRegion(0);

    // Clear region
    GDI_ClearRegion(0);

    GDI_ActivateRegion(0);
    */
}



//  *********************************************************************************
//  Function    :   _ShowMainList
//  Description :   Show Main List in the Main Frame
//  Arguments   :   None
//  Return      :   None
//  *********************************************************************************
static void _ShowMainList(void)
{
    //redraw main frame (can also be moved to the following of //main frame)                 
    _InitRect(&(_PRGM_FrameAttr.rect), MAIN_FRAME_X0, MAIN_FRAME_Y0,
                                       MAIN_FRAME_X1, MAIN_FRAME_Y1);
    GDI_DrawFrame_909(0, &_PRGM_FrameAttr);


    for (_bOSDBOOKMARKTemp =_bMainStart; (_bOSDBOOKMARKTemp<=MAX_PROG_ITEM) && ((_bOSDBOOKMARKTemp-_bMainStart)<MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME); _bOSDBOOKMARKTemp++)
    {      
        _ShowOneEntry(_bOSDBOOKMARKTemp);
    }                    

    //show arrow down rect
    if ( (MAX_PROG_ITEM-_bMainStart+1) > MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME)
    {                                  
        _InitRect(&(_PRGM_FrameAttr.rect), MAIN_FRAME_X0, MAIN_FRAME_Y1+1,
                  MAIN_FRAME_X1, MAIN_FRAME_Y1+1+ARROW_HEIGHT);
        GDI_DrawFrame_909(0, &_PRGM_FrameAttr);

        _PRGM_StrAttr.wX = ((MAIN_FRAME_X0+MAIN_FRAME_X1)>>1)-5;
        _PRGM_StrAttr.wY = MAIN_FRAME_Y1+4;
        GDI_DrawString_909(0, &_PRGM_StrAttr, aArrowDown);
    } 
    else //clear arrow down rect
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
        _InitRect(&(_PRGM_RectAttr.rect), MAIN_FRAME_X0, MAIN_FRAME_Y1+1,
                  MAIN_FRAME_X1, MAIN_FRAME_Y1+1+ARROW_HEIGHT);
        GDI_FillRect_909(0, &_PRGM_RectAttr);
    }        
    
    //show arrow up rect
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
    if ( _bMainStart >0)
#else
    if ( _bMainStart >1)
#endif
    {                                  
        _InitRect(&(_PRGM_FrameAttr.rect), MAIN_FRAME_X0,
                  MAIN_FRAME_Y0-1-ARROW_HEIGHT, 
                  MAIN_FRAME_X1, MAIN_FRAME_Y0-1);
        GDI_DrawFrame_909(0, &_PRGM_FrameAttr);

        _PRGM_StrAttr.wX = ((MAIN_FRAME_X0+MAIN_FRAME_X1)>>1)-5;
        _PRGM_StrAttr.wY = MAIN_FRAME_Y0-ARROW_HEIGHT+2;
        GDI_DrawString_909(0, &_PRGM_StrAttr, aArrowUp);
    }  
    else //clear arrow up rect
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
        _InitRect(&(_PRGM_RectAttr.rect), MAIN_FRAME_X0,
              MAIN_FRAME_Y0-1-ARROW_HEIGHT,
              MAIN_FRAME_X1, MAIN_FRAME_Y0-1);
        GDI_FillRect_909(0, &_PRGM_RectAttr);
    }                              
    
} //end of _ShowMainList()

//  *********************************************************************************
//  Function    :   _ShowOneEntry
//  Description :   Show One Entry in either Main, Title, or Chapter Frame.
//  Arguments   :   wEntry: main frame entry #, title #, or chapter #
//  Return      :   None
//  Side Effect :
//  *********************************************************************************
static void _ShowOneEntry(WORD wEntry)
{
    _wOSDBOOKMARKString[0] = 3; //digits       1.     -
    //Kevin1.22, expand program entry to more than 10
    _wOSDBOOKMARKString[1] = aNDNumbers[wEntry/10];
    if (wEntry<10)
        _wOSDBOOKMARKString[1] = CHAR_SPACE;
    _wOSDBOOKMARKString[2] = aNDNumbers[wEntry%10];
    _wOSDBOOKMARKString[3] = CHAR_FULL_STOP;

    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT2;
    _PRGM_StrAttr.wX = MAIN_FRAME_X0+FRAME_THICKNESS+10;
    _PRGM_StrAttr.wY = MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart);
    GDI_DrawString_909(0, &_PRGM_StrAttr, _wOSDBOOKMARKString);

    if (0 !=  __wBookMark[wEntry]) 
    {
        _PRGM_StrAttr.wX = MAIN_FRAME_X0+70;
        GDI_DrawString_909(0, &_PRGM_StrAttr, aBookmark);
        _PRGM_StrAttr.wX += 125;
        _ShowOneEntryNumber(wEntry);
        GDI_DrawString_909(0, &_PRGM_StrAttr, _wOSDBOOKMARKString);
    } else {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TEXT2;
        _InitRect(&(_PRGM_RectAttr.rect),
                  MAIN_FRAME_X0 + FRAME_THICKNESS + 65,
                  MAIN_FRAME_Y0 + FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart+1) - 5,
                  MAIN_FRAME_X1- FRAME_THICKNESS - 25,
                  MAIN_FRAME_Y0 + FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart+1) + 3 - 5);
        GDI_FillRect_909(0, &_PRGM_RectAttr);
    }
}


//  *********************************************************************************
//  Function    :   _ShowOneEntryNumber
//  Description :   Show One Entry Number in either SEARCH_TITLE, SEARCH_CHAPTER, or SEARCH_TIME field.
//  Arguments   :   bSize : Display Size , bNumber: Display Number
//  Return      :   None
//  Side Effect :
//  *********************************************************************************
static void _ShowOneEntryNumber(WORD wNumber)
{

    //2 digits  or 3 digits
    if (wNumber < 10)
        _wOSDBOOKMARKString[0] = 1;
    else if (9 < wNumber && wNumber < 100)
        _wOSDBOOKMARKString[0] = 2;
    else if (99 < wNumber && wNumber < 1000)
        _wOSDBOOKMARKString[0] = 3;

    for (_bOSDBOOKMARKTemp1 = _wOSDBOOKMARKString[0]; _bOSDBOOKMARKTemp1 > 0; _bOSDBOOKMARKTemp1--)
    { 
        _wOSDBOOKMARKString[_bOSDBOOKMARKTemp1] =  aNDNumbers[wNumber%10];
        wNumber /= 10;
    }
/*
    _wOSDBOOKMARKString[bSize]     =  aNDNumbers[wNumber%10];
    _wOSDBOOKMARKString[bSize-1]   =  aNDNumbers[(wNumber/10)%10];
*/
}



//  *********************************************************************************
//  Function    :   _ShowTitleChapterTime 
//  Description :   Show Title or Chapter and Time
//  Return      :   None
//  *********************************************************************************
static void _ShowTitleChapterTime()
{
    _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
    _InitRect(&(_PRGM_RectAttr.rect), MAIN_FRAME_X1+20, MAIN_FRAME_Y0,
              BMARK_FRAME_X1, MAIN_FRAME_Y1);
    GDI_FillRect_909(0, &_PRGM_RectAttr);

    if (0 != __wBookMark[__wBookMark[0]]) 
    {              
        _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT1;
        _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_MAIN_COLOR_BG;
        _PRGM_StrAttr.bColorKey = 0;

        if (__wDiscType & BOOK_DVD)
        {
            __bSearchTitle=__bBookMark_Title[__wBookMark[0]];
            __bGoToTimeNO=0;
            if ( __wBookMark[__wBookMark[0]]!=0)
            __bGoToTimeNO=1;

            _PRGM_StrAttr.wX = MAIN_FRAME_X1+30; _PRGM_StrAttr.wY = MAIN_FRAME_Y0; 
            GDI_DrawString_909(0, &_PRGM_StrAttr, aTitle);
            _PRGM_StrAttr.wX += 105; 
            GDI_DrawString_909(0, &_PRGM_StrAttr, aColon);
            _PRGM_StrAttr.wX += 30; 
//            _ShowOneEntryNumber(__bTitle);
            _ShowOneEntryNumber(__bBookMark_Title[__wBookMark[0]]);
            GDI_DrawString_909(0, &_PRGM_StrAttr, _wOSDBOOKMARKString);

            _PRGM_StrAttr.wX = MAIN_FRAME_X1+30; _PRGM_StrAttr.wY += 30; 
            GDI_DrawString_909(0, &_PRGM_StrAttr, aChapter);
            _PRGM_StrAttr.wX += 105; 
            GDI_DrawString_909(0, &_PRGM_StrAttr, aColon);
            _PRGM_StrAttr.wX += 30; 
//            _ShowOneEntryNumber(__wChapter);
            _ShowOneEntryNumber(_wBookMark_Chapter[__wBookMark[0]]);
            GDI_DrawString_909(0, &_PRGM_StrAttr, _wOSDBOOKMARKString);
        }

        _PRGM_StrAttr.wX = MAIN_FRAME_X1+30; 
        if (__wDiscType & BOOK_DVD)
        {
        _PRGM_StrAttr.wY += 30; 
        }
        else
        {
        _PRGM_StrAttr.wY = MAIN_FRAME_Y0; 
        }
        GDI_DrawString_909(0, &_PRGM_StrAttr, aTime);
        _PRGM_StrAttr.wX += 105; 
        GDI_DrawString_909(0, &_PRGM_StrAttr, aColon);

        _wOSDBOOKMARKString[1] = aNDNumbers[HIBYTE(__wBookMark[__wBookMark[0]]) /60];
        _wOSDBOOKMARKString[3] = aNDNumbers[(HIBYTE(__wBookMark[__wBookMark[0]]) %60) /10];
        _wOSDBOOKMARKString[4] = aNDNumbers[HIBYTE(__wBookMark[__wBookMark[0]]) %10]; 
        _wOSDBOOKMARKString[6] = aNDNumbers[LOBYTE(__wBookMark[__wBookMark[0]]) /10];
        _wOSDBOOKMARKString[7] = aNDNumbers[LOBYTE(__wBookMark[__wBookMark[0]]) %10];

        _wOSDBOOKMARKString[0] = 7; //7 digit
        _wOSDBOOKMARKString[2] = CHAR_COLON;        
        _wOSDBOOKMARKString[5] = CHAR_COLON;      
        _PRGM_StrAttr.wX += 30; 
        GDI_DrawString_909(0, &_PRGM_StrAttr, _wOSDBOOKMARKString);
    }
}



//  *********************************************************************************
//  Function    :   _SetClearHighlight
//  Description :   Set/Clear Highlight & Show the current entry
//  Arguments   :   bFlag: SET_HIGHLIGHT/CLEAR_HIGHLIGHT
//  Return      :   None
//  Side Effect :
//  *********************************************************************************
static void _SetClearHighlight(BYTE bFlag)
{   
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT2;

    if (bFlag) //SET_HIGHLIGHT
    {
        _bOSDBOOKMARKTemp = OSDPRGM_ENTRY_COLOR_HILIGHT;      
        _PRGM_StrAttr.bBackground = _bOSDBOOKMARKTemp;
    }
    else //CLEAR_HIGHLIGHT
    {
       _bOSDBOOKMARKTemp = OSDPRGM_ENTRY_FRAME_CENTER;
       _PRGM_StrAttr.bBackground = _bOSDBOOKMARKTemp;
    }        
  

    _PRGM_RectAttr.bColor = _bOSDBOOKMARKTemp;
    _InitRect(&(_PRGM_RectAttr.rect), MAIN_FRAME_X0+FRAME_THICKNESS,
        MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(_bMainHighlightPos-1),
        MAIN_FRAME_X1-FRAME_THICKNESS,
        MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(_bMainHighlightPos-1)+ENTRY_HEIGHT);
    GDI_FillRect_909(0, &_PRGM_RectAttr);
    _ShowOneEntry(_bMainStart+_bMainHighlightPos-1);  

} //end of _SetClearHighlight()   


//***************************************************************************
//  Function    :   _OSDBOOKMARK_ShowUI
//  Abstract    :   Draw the Program UI.
//  Arguments   :   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDBOOKMARK_ShowUI()
{
#ifdef MINI_OSD_REGION //CoCo2.30, borrow frame buffer to show OSD.
    GDI_ClearRegion(0); // clera the mini OSD region
    __dwRegionHeight = GDI_REGION_HEIGHT; // 460
    // Need to set to the correct DRAM address

    __dwRegionTAddr = W99AV_ADDR_MPEG_FRAME_FOR_OS_T; //Use frame buffer. 
    __dwRegionBAddr = W99AV_ADDR_MPEG_FRAME_FOR_OS_B; //Use frame buffer.

    GDI_InitialRegion (0); // initialize region to set it to 8-bit mode
#endif
        
    GDI_SetMixRatio(GDI_GENERAL_MIX_RATIO);               
    GDI_ActivateRegion(0);
		
    //Initialization of Variables 
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
    _bMainStart = 0; 
#else
    _bMainStart = 1; 
#endif            
		
#ifdef SUPPORT_CLEARALL_IN_PROGRAM            
    _bMainHighlightPos = 2; //highlight 1. __-__  
#else
    _bMainHighlightPos = 1; //highlight 1. __-__ 
#endif
    __wBookMark[0] = _bMainStart;
		
    _OSDBOOKMARK_InitPalette();
    _OSDBOOKMARK_DrawFrames();

    _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_COLOR_TOP_BANNER;
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TOP_BANNER_TEXT;
    _PRGM_StrAttr.bColorKey = 0;
    _PRGM_StrAttr.wX = BMARK_FRAME_X0+25; _PRGM_StrAttr.wY = BMARK_FRAME_Y0+7; 
    GDI_DrawString_909(0, &_PRGM_StrAttr, aBookmark);
		
    _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER;
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER_TEXT;
    _PRGM_StrAttr.bColorKey = 0;
    _PRGM_StrAttr.wX = BMARK_FRAME_X0+60; _PRGM_StrAttr.wY = BMARK_FRAME_Y1-30; 
    GDI_DrawString_909(0, &_PRGM_StrAttr, aColon);
    _PRGM_StrAttr.wX = BMARK_FRAME_X0+75; _PRGM_StrAttr.wY = BMARK_FRAME_Y1-30; 
    GDI_DrawString_909(0, &_PRGM_StrAttr, aBookmark);

    _PRGM_StrAttr.wX = BMARK_FRAME_X0+290; _PRGM_StrAttr.wY = BMARK_FRAME_Y1-30; 
    GDI_DrawString_909(0, &_PRGM_StrAttr, aColon);
    _PRGM_StrAttr.wX = BMARK_FRAME_X0+305; _PRGM_StrAttr.wY = BMARK_FRAME_Y1-30; 
    GDI_DrawString_909(0, &_PRGM_StrAttr, aClear);

    // Set text color to RED
    _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER;
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER_TEXT;
    _PRGM_StrAttr.bColorKey = 0;

    _PRGM_StrAttr.wX = BMARK_FRAME_X0+25; _PRGM_StrAttr.wY = BMARK_FRAME_Y1-30; 
    GDI_DrawString_909(0, &_PRGM_StrAttr, aPlayIcon);

    _PRGM_StrAttr.wX = BMARK_FRAME_X0+250; _PRGM_StrAttr.wY = BMARK_FRAME_Y1-30; 
    GDI_DrawString_909(0, &_PRGM_StrAttr, aClearIcon);

    _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
    _InitRect(&(_PRGM_RectAttr.rect),
              BMARK_FRAME_X0, BMARK_FRAME_Y0 + 40,
              BMARK_FRAME_X1, BMARK_FRAME_Y1 - 37);
    GDI_FillRect_909(0, &_PRGM_RectAttr);


    _PRGM_FrameAttr.bStyle = FRAME_POP_SOLID; 
    _PRGM_FrameAttr.bThickness = FRAME_THICKNESS; 
    _PRGM_FrameAttr.bColorBright = OSDPRGM_ENTRY_FRAME_BRIGHT; 
    _PRGM_FrameAttr.bColorDark   = OSDPRGM_ENTRY_FRAME_DARK; 
    _PRGM_FrameAttr.bColorCenter = OSDPRGM_ENTRY_FRAME_CENTER; 

    _InitRect(&(_PRGM_FrameAttr.rect),
              BMARK_FRAME_X0, BMARK_FRAME_Y0 + 37,
              BMARK_FRAME_X1, BMARK_FRAME_Y0 + 40);
    GDI_DrawFrame_909(0, &_PRGM_FrameAttr);

    _InitRect(&(_PRGM_FrameAttr.rect),
              BMARK_FRAME_X0, BMARK_FRAME_Y1 - 40, 
              BMARK_FRAME_X1, BMARK_FRAME_Y1 - 37);

    GDI_DrawFrame_909(0, &_PRGM_FrameAttr);

    _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_MAIN_COLOR_BG;
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT1;
    _PRGM_StrAttr.bColorKey = 0;

    //other texts always use BLACK color 
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT2;
    _PRGM_StrAttr.bBackground= OSDPRGM_ENTRY_COLOR_TEXT_BG; 
		
    //main frame                         
    _ShowMainList();
    _SetClearHighlight(SET_HIGHLIGHT);            
    _ShowTitleChapterTime();
		
}


//***************************************************************************
//  Function    :   _OSDBOOKMARK_ClearUI
//  Abstract    :   Clear the Program UI.
//  Arguments   :   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDBOOKMARK_ClearUI(void)
{
    //clear Search IR region

    _PRGM_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _InitRect(&(_PRGM_RectAttr.rect), 0, OSDND_UPPER_BACKGROUND_BOTTOM + 1,
              GDI_REGION_WIDTH, OSDND_BOTTOM_BACKGROUND_START_V - 1);
    GDI_FillRect_909(0, &_PRGM_RectAttr);
}


//***************************************************************************
//  Function    :   _OSDBOOKMARK_DrawFrames 
//  Abstract    :   Draw the frames.
//  Arguments   :
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void _OSDBOOKMARK_DrawFrames(void)
{
    // Clear region
    _PRGM_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _InitRect(&(_PRGM_RectAttr.rect),
              0, OSDND_UPPER_BACKGROUND_BOTTOM + 1,
              GDI_REGION_WIDTH, OSDND_BOTTOM_BACKGROUND_START_V - 1);
    GDI_FillRect_909(0, &_PRGM_RectAttr);


    if (SETUP_CUSTOM_UI_STYLE_1 == __SetupInfo.bUIStyle)
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect),
                  BMARK_FRAME_X0, BMARK_FRAME_Y0,
                  BMARK_FRAME_X1, BMARK_FRAME_Y1);
        GDI_FillRoundRect(0, 17, &_PRGM_RectAttr);
    }
    else if (SETUP_CUSTOM_UI_STYLE_2 == __SetupInfo.bUIStyle)
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect),
                  BMARK_FRAME_X0, BMARK_FRAME_Y0,
                  BMARK_FRAME_X1 - 50, CENTER_FRAME_Y1-1);
        GDI_FillRoundRectTop(0, 21, &_PRGM_RectAttr);
        _InitRect(&(_PRGM_RectAttr.rect),
                  BMARK_FRAME_X1 - 100, BMARK_FRAME_Y0,
                  BMARK_FRAME_X1, MAIN_FRAME_Y1-1);
        GDI_FillRect_909(0, &_PRGM_RectAttr);

        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect),
                  BMARK_FRAME_X0, CENTER_FRAME_Y1,
                  BMARK_FRAME_X1 - 50, BMARK_FRAME_Y1);
        GDI_FillRect_909(0, &_PRGM_RectAttr);
        _InitRect(&(_PRGM_RectAttr.rect),
                  BMARK_FRAME_X1 - 100, CENTER_FRAME_Y1,
                  BMARK_FRAME_X1, BMARK_FRAME_Y1);
        GDI_FillRoundRectBottom(0, 21, &_PRGM_RectAttr);
    }
    else if (SETUP_CUSTOM_UI_STYLE_3 == __SetupInfo.bUIStyle)
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect),
                  BMARK_FRAME_X0, BMARK_FRAME_Y0,
                  BMARK_FRAME_X1, CENTER_FRAME_Y1-1);
        GDI_FillRoundRectTop(0, 21, &_PRGM_RectAttr);
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect),
                  BMARK_FRAME_X0, CENTER_FRAME_Y1,
                  BMARK_FRAME_X1, BMARK_FRAME_Y1);
        GDI_FillRect_909(0, &_PRGM_RectAttr);
    }
    else if (SETUP_CUSTOM_UI_STYLE_4 == __SetupInfo.bUIStyle)
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect),
                  BMARK_FRAME_X0, BMARK_FRAME_Y0,
                  BMARK_FRAME_X1, CENTER_FRAME_Y1-1);
        GDI_FillRoundRectTop(0, 21, &_PRGM_RectAttr);
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect),
                  BMARK_FRAME_X0, CENTER_FRAME_Y1,
                  BMARK_FRAME_X1, BMARK_FRAME_Y1);
        GDI_FillRoundRectBottom(0, 21, &_PRGM_RectAttr);
    }
}


//***************************************************************************
//  Function    :   _OSDBOOKMARK_InitPalette 
//  Abstract    :   Initialize the palettes.
//  Arguments   :
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :
//***************************************************************************
void _OSDBOOKMARK_InitPalette(void)
{
    if (SETUP_CUSTOM_UI_STYLE_1 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TOP_BANNER,
            OSDPRGM_VALUE_COLOR_TOP_BANNER1, TRUE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TOP_BANNER_TEXT,
            OSDPRGM_VALUE_COLOR_TOP_BANNER_TEXT1, TRUE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER,
            OSDPRGM_VALUE_COLOR_BOTTOM_BANNER1, TRUE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER_TEXT,
            OSDPRGM_VALUE_COLOR_BOTTOM_BANNER_TEXT1, TRUE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_MAIN_COLOR_BG,
            OSDPRGM_VALUE_MAIN_COLOR_BG1, TRUE);
    }
    else if (SETUP_CUSTOM_UI_STYLE_2 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TOP_BANNER,
            OSDPRGM_VALUE_COLOR_TOP_BANNER2, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TOP_BANNER_TEXT,
            OSDPRGM_VALUE_COLOR_TOP_BANNER_TEXT2, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER,
            OSDPRGM_VALUE_COLOR_BOTTOM_BANNER2, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER_TEXT,
            OSDPRGM_VALUE_COLOR_BOTTOM_BANNER_TEXT2, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_MAIN_COLOR_BG,
            OSDPRGM_VALUE_MAIN_COLOR_BG2, TRUE);
    }
    else if (SETUP_CUSTOM_UI_STYLE_3 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TOP_BANNER,
            OSDPRGM_VALUE_COLOR_TOP_BANNER3, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TOP_BANNER_TEXT,
            OSDPRGM_VALUE_COLOR_TOP_BANNER_TEXT3, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER,
            OSDPRGM_VALUE_COLOR_BOTTOM_BANNER3, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER_TEXT,
            OSDPRGM_VALUE_COLOR_BOTTOM_BANNER_TEXT3, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_MAIN_COLOR_BG,
            OSDPRGM_VALUE_MAIN_COLOR_BG3, TRUE);
    }
    else if (SETUP_CUSTOM_UI_STYLE_4 == __SetupInfo.bUIStyle)
    {
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TOP_BANNER,
            OSDPRGM_VALUE_COLOR_TOP_BANNER4, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TOP_BANNER_TEXT,
            OSDPRGM_VALUE_COLOR_TOP_BANNER_TEXT4, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER,
            OSDPRGM_VALUE_COLOR_BOTTOM_BANNER4, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER_TEXT,
            OSDPRGM_VALUE_COLOR_BOTTOM_BANNER_TEXT4, FALSE);
        GDI_ChangePALEntry(OSDPRGM_ENTRY_MAIN_COLOR_BG,
            OSDPRGM_VALUE_MAIN_COLOR_BG4, TRUE);
    }

    GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TEXT1, OSDPRGM_VALUE_COLOR_WHITE, FALSE);
    GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TEXT2, OSDPRGM_VALUE_COLOR_BLACK, FALSE);
    GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_TEXT_BG, OSDPRGM_VALUE_FRAME_CENTER, FALSE);
    GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_WHITE, OSDPRGM_VALUE_COLOR_WHITE, FALSE);
    GDI_ChangePALEntry(OSDPRGM_ENTRY_COLOR_HILIGHT, OSDPRGM_VALUE_COLOR_HILIGHT, FALSE);
    GDI_ChangePALEntry(OSDPRGM_ENTRY_FRAME_BRIGHT, OSDPRGM_VALUE_FRAME_BRIGHT, FALSE);
    GDI_ChangePALEntry(OSDPRGM_ENTRY_FRAME_DARK, OSDPRGM_VALUE_FRAME_DARK, FALSE);
    GDI_ChangePALEntry(OSDPRGM_ENTRY_FRAME_CENTER, OSDPRGM_VALUE_FRAME_CENTER, FALSE);
    GDI_ChangePALEntry(OSDPRGM_ENTRY_TEXT_DISABLED, OSDPRGM_VALUE_TEXT_DISABLED, FALSE);

    GDI_WaitPaletteComplete();

}



//  *****************************************************************
//  Function    :   _OSDBOOKMARK_ProceedToNextMainEntry
//  Description :   Proceed to next main entry 
//  Argument    :   None
//  Return      :   None
//  Side Effect :   
//  *****************************************************************
//Kevin1.23DMTech, create
void _OSDBOOKMARK_ProceedToNextMainEntry(void)
{
    _SetClearHighlight(CLEAR_HIGHLIGHT);

    if ((_bMainHighlightPos<MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME) && 
        (_bMainStart+_bMainHighlightPos-1<MAX_PROG_ITEM) ) 
    {
          //change _bMainHighlightPos only
        _bMainHighlightPos++;  
    }
    else
    {   
        if ((_bMainStart+MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME-1)<MAX_PROG_ITEM)
        {   
            _bMainStart++;
        }       

#ifdef SUPPORT_CIRCULAR_WRAP //Kevin1.05, add
        else
        {
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
            _bMainStart = 0; 
            _bMainHighlightPos = 2; //highlight 1. __-__  
#else
            _bMainStart = 1; 
            _bMainHighlightPos = 1; //highlight 1. __-__ 
#endif 
         }
#endif //#ifdef SUPPORT_CIRCULAR_WRAP //Kevin2.31-2
         
		_ShowMainList(); 

    }

    _SetClearHighlight(SET_HIGHLIGHT);

} //end of _OSDBOOKMARK_ProceedToNextMainEntry()

#endif //#ifndef NO_DISC_MODE //CoCo2.37p
#endif //#ifdef SUPPORT_BOOKMARK






