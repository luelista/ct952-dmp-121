
#include    "winav.h"
#include    "input.h"
#include    "disp.h"
#include    "osd.h"
#include    "hal.h"
#include    "cc.h"
#include    "gdi.h"
#include "fontindex.h" //++CoCo2.37P
#include    "dvdinfo.h"
#include    "linear.h"
#include    "navigate.h"
#include    "OSDPRGM.h"
#include    "infofilter.h"
#include    "utl.h"
#include    "osddsply.h"
#include    "osdnd.h"
#include    "setup.h"
#include    "notransl.h"

#ifndef NO_PROGRAM_UI //++CoCo2.37p
#ifndef NO_DISC_MODE //CoCo2.37p

#define OSDPRGM_MAIN_FILE
#include "OSDString\\strextrn.h"

PARM_FRAME       _PRGM_FrameAttr;
PARM_DRAW_STRING _PRGM_StrAttr;
PARM_RECT        _PRGM_RectAttr;
//show entry from
BYTE    _bMainStart; 
BYTE    _bMainHighlightPos; 


#ifdef SUPPORT_CLEARALL_IN_PROGRAM
#define MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME       6 
#define MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME      5 
#define MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME    5 
#else
#define MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME       7 
#define MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME      5 
#define MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME    5 
#endif

// This value will offset the entire Program UI
#define PROGRAM_GLOBAL_OFFSET_X     (0) 


#define ENTRY_HEIGHT        26 
#define FRAME_THICKNESS     2  
#define ARROW_HEIGHT        22


#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
#define PROGRAM_WIDTH       612 
#define PROGRAM_HEIGHT      350
#define PRGM_FRAME_X0       (PROGRAM_GLOBAL_OFFSET_X)
#define PRGM_FRAME_Y0	    (OSDND_UPPER_BACKGROUND_BOTTOM + (8 - OSDND_UPPER_BACKGROUND_BOTTOM%8)) 
#define PRGM_FRAME_X1       (PROGRAM_WIDTH + PROGRAM_GLOBAL_OFFSET_X)
#define PRGM_FRAME_Y1       (PROGRAM_HEIGHT + PRGM_FRAME_Y0)

#else
#define PRGM_FRAME_X0       (PROGRAM_GLOBAL_OFFSET_X)
#define PRGM_FRAME_Y0	    (OSDND_UPPER_BACKGROUND_BOTTOM + 1) 
#define PRGM_FRAME_X1       (616+PROGRAM_GLOBAL_OFFSET_X)
#define PRGM_FRAME_Y1       (OSDND_BOTTOM_BACKGROUND_START_V - 1) 
#define PROGRAM_WIDTH       (PRGM_FRAME_X1 - PRGM_FRAME_X0)
#define PROGRAM_HEIGHT      (PRGM_FRAME_Y1 - PRGM_FRAME_Y0)
#endif // #ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM



#define CENTER_FRAME_X0     PRGM_FRAME_X0 
#define CENTER_FRAME_Y0     (PRGM_FRAME_Y0 + 41) 
#define CENTER_FRAME_X1     PRGM_FRAME_X1 
#define CENTER_FRAME_Y1     (PRGM_FRAME_Y1 - 41) 


#define FRAME_Y0            (PRGM_FRAME_Y0 + 3 * ENTRY_HEIGHT + 5) 

#define MAIN_FRAME_X0       (40+PROGRAM_GLOBAL_OFFSET_X) 
#define MAIN_FRAME_Y0       FRAME_Y0
#define MAIN_FRAME_X1       (280+PROGRAM_GLOBAL_OFFSET_X)
#define MAIN_FRAME_Y1       (MAIN_FRAME_Y0+MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME*ENTRY_HEIGHT+2*FRAME_THICKNESS)

#define TITLE_FRAME_X0      (340+PROGRAM_GLOBAL_OFFSET_X)
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
#define TITLE_FRAME_Y0      (FRAME_Y0+ENTRY_HEIGHT*2+FRAME_THICKNESS)
#else
#define TITLE_FRAME_Y0      (FRAME_Y0+ENTRY_HEIGHT*2)
#endif
#define TITLE_FRAME_X1      (400+PROGRAM_GLOBAL_OFFSET_X)
#define TITLE_FRAME_Y1      (TITLE_FRAME_Y0+MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME*ENTRY_HEIGHT+2*FRAME_THICKNESS)


#define CHAPTER_FRAME_X0    (440+PROGRAM_GLOBAL_OFFSET_X)
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
#define CHAPTER_FRAME_Y0    (FRAME_Y0+ENTRY_HEIGHT*2+FRAME_THICKNESS)
#else
#define CHAPTER_FRAME_Y0    (FRAME_Y0+ENTRY_HEIGHT*2)
#endif
#define CHAPTER_FRAME_X1    (520+PROGRAM_GLOBAL_OFFSET_X)
#define CHAPTER_FRAME_Y1    (CHAPTER_FRAME_Y0+MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME*ENTRY_HEIGHT+2*FRAME_THICKNESS)
                
#define MAIN_FRAME			1
#define TITLE_INPUT                     2
#define TITLE_FRAME			3
#define CHAPTER_INPUT                   4
#define CHAPTER_FRAME	                5	

#define CLEAR_HIGHLIGHT		0
#define SET_HIGHLIGHT		1


WORD _wOSDPRGMString[8];

////   for testing
#include "ctkav_disp.h"
#include "ctkav.h"
extern DWORD _dwGDIPalette[256];
DWORD  dwTmp, *pdwPtr;

//Internal Functions
static void _ShowInput(BYTE bInput, BYTE bBgColor);
static void _ShowOneEntry(WORD wEntry);
static void _ShowMainList(void);
static void _ShowTitleList(void);
static void _ShowChapterList(void);
static void _SetClearHighlight(BYTE bFlag);

void _OSDPRGM_ShowUI(void);
void _OSDPRGM_ClearUI(void);
void _OSDPRGM_DrawFrames(void);
void _OSDPRGM_InitPalette(void);
void _OSDPRGM_InitRect(PURECT pRect, WORD wLeft, WORD wTop, WORD wRight, WORD wBottom);

void _OSDPRGM_OutputNum(WORD wNum, BYTE bDigit); //CoCo1.21
void _OSDPRGM_UpdateProgramChapter(BYTE bDigits); //CoCo1.21;
void _OSDPRGM_UpdateChapterList(void); //CoCo1.21
void _OSDPRGM_UpdateTitleList(void); //CoCo1.21
void _OSDPRGM_ProceedToNextMainEntry(void); //Kevin1.23DMTech
void _OSDPRGM_FillRect(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom); //CoCo1.20, reduce code size.
void _OSDPRGM_DrawFrame(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom);  //CoCo1.20, reduce code size.
void _OSDPRGM_DrawString(void); //CoCo1.20, reduce code size.
void _OSDPRGM_SetStringPos(WORD wX, WORD wY); //CoCo1.20, reduce code size.
void _OSDPRGM_DrawDynamicString(WORD *pwString); //CoCo1.20, reduce code size.
extern void _InitRect(PURECT pRect, WORD wLeft, WORD wTop, WORD wRight, WORD wBottom); //CoCo1.20, reduce code size.



//MAX_PROG_ITEM                 //total # of tracks to be programmed
BYTE _bTotalTitle;              //total # of titles in the disc
WORD _wTotalChapter;            //total # of chapters corresponding to the selected title

//show entry from
BYTE    _bTitleStart;    
WORD    _wChapterStart;  
    
//current pos 
BYTE    _bTitleHighlightPos;
WORD    _wChapterHighlightPos;

//current frame 
BYTE    _bCurrentFrame;

extern BYTE  __bTemp;
BYTE    _bOSDPRGMTemp, _bOSDPRGMTemp1; //for temp use
WORD    _wOSDPRGMTemp;

#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06

extern WORD __wNumberQueue; //declare a local var in OSDPRGM later => not necessay

#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
extern BYTE __bPreCountStop;
#endif

#define PROGRAM_NUM_CURSOR_TITLE        0x1
#define PROGRAM_NUM_CURSOR_CHAPTER      0x2   

static void _TitleChapterNumErr(void); //CoCo1.21, modify the function prototype

#define TITLE_NUM_ERROR             0x1
#define CHAPTER_NUM_ERROR           0x2
#define TITLE_CHAPTER_NUM_ERROR     (TITLE_NUM_ERROR | CHAPTER_NUM_ERROR)


static BYTE _OSDPRGM_ProcessNumInput(BYTE bKey, WORD wParam);

#define PRGM_NUMBERINPUT_CHECK      1
#define PRGM_NUMBERINPUT_CLEAR      2
#define PRGM_NUMBERINPUT_NUMBER     3

#endif //#ifdef SUPPORT_PRGM_NUM_INPUT

//  ****************************************************************************
//  Function    :   OSDPRGM_Exit
//  Description :   The OSD kernel will call this function to exit normally.
//		:   This function can NOT send OSD_ChangeUI(xxx, OSD_EXIT_UI) to
//		:   notify OSD kernel it exits.
//  Arguments   :   bExitMode 
//                  OSD_UI_EXIT_ALL             : Exit control and clear the UI.
//                  OSD_UI_EXIT_CONTROL_ONLY    : Exit control only.
//  Return      :   
//  Side Effect :
//  ****************************************************************************
void OSDPRGM_Exit(BYTE bExitMode)
{
    switch (bExitMode)
    {
    case OSD_UI_EXIT_ALL:
        GDI_DeactivateRegion();
        _OSDPRGM_ClearUI();
    case OSD_UI_EXIT_CONTROL_ONLY:
        LINEAR_Program(LINEAR_PROGRAM_CLEAR, NULL_0);
        break;
    default:
        break;
    }
}



//  ****************************************************************************
//  Function    :   OSDPRGM_ProcessKey
//  Description :   Process the input key while in DVD/VCD Program mode
//  Arguments   :   
//  Return      :   KEY_NO_KEY  , the input key is processed
//              :   INVALID_KEY , the input key is not allowed under this mode
//              :   KEY_xxx     , main program needs to do more processing for the key.
//  Side Effect :
//  ****************************************************************************
BYTE OSDPRGM_ProcessKey(void)
{
    BYTE bKeyStatus;

    if ((__wDiscType & BOOK_PICTURECD) ||
        (__bKeySource != KEY_FROM_ISR))return KEY_BYPASS;

    if (! ((__bKey == KEY_PROGRAM) || __btInputProgram))
    {
        return KEY_BYPASS;
    }

    bKeyStatus = KEY_INVALID;

    switch (__bKey)
    {
    case KEY_TVMODE:
/*
    case KEY_MUTE:
    case KEY_VOL_UP:
    case KEY_VOL_DOWN:
    case KEY_KEYUP:
    case KEY_KEYDOWN:
*/
    case KEY_GOTOTIME:
    case KEY_BOOKMARK:
    case KEY_NO_KEY:
        bKeyStatus = KEY_BYPASS;
        break;

    case KEY_SETUP:
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
        _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
#endif
        bKeyStatus = KEY_BYPASS;
        break;

    case KEY_PBC:
        if (__btInputProgram)
        {
            OSDPRGM_Exit(OSD_UI_EXIT_ALL);
            OSD_ChangeUI(OSD_UI_DVD_PROGRAM, OSD_EXIT_UI);
        }
        bKeyStatus = KEY_BYPASS;
        break;

    case KEY_UP:
/*
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
        _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL);
#endif
*/
        if (_bCurrentFrame==MAIN_FRAME)
        {   
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
        _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
        _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
#endif
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
        }
        else if (_bCurrentFrame==TITLE_FRAME)
        {   
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
        _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
#endif
        _SetClearHighlight(CLEAR_HIGHLIGHT);

            _bOSDPRGMTemp = TRUE;
            if (_bTitleHighlightPos>1)
                _bTitleHighlightPos--;   
            else
            {   
                if (_bTitleStart>1)
                {   
                    _bTitleStart--;
                    _ShowTitleList(); 
                } 
                else
                {
                    _bOSDPRGMTemp = FALSE;  // Move to TITLE_INPUT
                    _bCurrentFrame = TITLE_INPUT;
                }
            }
			
            if (__wDiscType & BOOK_DVD) 
            {            
                if (_bOSDPRGMTemp) 
                {
                    DVD_ParseInfo ( DVD_TOTAL_CHAPTERS, &__dwTemp, (WORD)(_bTitleStart+_bTitleHighlightPos-1) );             
                    _wTotalChapter = (WORD)__dwTemp;           
                    _OSDPRGM_UpdateChapterList(); 
                    _bCurrentFrame = TITLE_FRAME;
                }
                else
                {
                    _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
                    _OSDPRGM_FillRect(CHAPTER_FRAME_X0, CHAPTER_FRAME_Y0-1-ARROW_HEIGHT, CHAPTER_FRAME_X1, CHAPTER_FRAME_Y1+1+ARROW_HEIGHT); //CoCo1.20, reduce code size.
                }
            }            
        }                     
        else if (_bCurrentFrame==CHAPTER_FRAME) //CHAPTER_FRAME
        {
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            if (_wChapterHighlightPos>1)
                _wChapterHighlightPos--;   
            else
            {   
                if (_wChapterStart>1)
                { 
                    _wChapterStart--;
                    _ShowChapterList(); 
                }    
                else
                {
                    _bCurrentFrame = CHAPTER_INPUT;
                }
            }
        }       
        _SetClearHighlight(SET_HIGHLIGHT);
        
        bKeyStatus =KEY_NO_KEY;
        break;
		
		
    case KEY_DOWN:
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
        _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
#endif
        _SetClearHighlight(CLEAR_HIGHLIGHT);
		
        if (_bCurrentFrame==MAIN_FRAME)
        {
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
            _OSDPRGM_ProceedToNextMainEntry();
        }            
        else if (_bCurrentFrame==TITLE_INPUT)
        {   
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
            _ShowInput(TITLE_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);
            if (!(_bMainStart+_bMainHighlightPos-1))
            {
                //do nothing
            }
            else
            {   
#ifndef SUPPORT_PRGM_NUM_INPUT 
                if (__wDiscType & BOOK_DVD) //no Chapter Frame for VCD         
                {
                    _SetClearHighlight(CLEAR_HIGHLIGHT);
                    _bCurrentFrame = TITLE_FRAME;
                    _SetClearHighlight(SET_HIGHLIGHT);        
				
                    if (_bCurrentFrame == TITLE_FRAME )
                    {
                        DVD_ParseInfo(DVD_TOTAL_CHAPTERS, &__dwTemp,
                                      (WORD)(_bTitleStart+_bTitleHighlightPos-1) ); 
                        _wTotalChapter = __dwTemp;           
                        _ShowChapterList();
                    }
                }
#else //#ifndef SUPPORT_PRGM_NUM_INPUT
                __wNumberQueue = 0;    //clear
			
                _SetClearHighlight(CLEAR_HIGHLIGHT);
                _bCurrentFrame = TITLE_FRAME;
                _SetClearHighlight(SET_HIGHLIGHT); 

                if (__wDiscType & BOOK_DVD) //no Chapter Frame for VCD         
                {
                        DVD_ParseInfo ( DVD_TOTAL_CHAPTERS, &__dwTemp, (WORD)(_bTitleStart+_bTitleHighlightPos-1) );             
                        _wTotalChapter = (WORD)__dwTemp;    
						
                        _ShowChapterList();
                        _bCurrentFrame = TITLE_FRAME; /////highlight remains in Title Frame             
                }
#endif
            }
        }
        else if (_bCurrentFrame==TITLE_FRAME)
        {   
            if ( (_bTitleHighlightPos<MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME) && (_bTitleStart+_bTitleHighlightPos-1<_bTotalTitle) ) //change _bTitleHighlightPos only
                _bTitleHighlightPos++;   
            else //change _bTitleStart
            {   
                if ((_bTitleStart+MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME-1)<_bTotalTitle)
                {   
                    _bTitleStart++;
                    _ShowTitleList();                    
                }         
            }

            if (__wDiscType & BOOK_DVD) //no Chapter Frame for VCD           
            {            
				//uncomment when DVD/VCD is in the loader
                DVD_ParseInfo ( DVD_TOTAL_CHAPTERS, &__dwTemp, (WORD)(_bTitleStart+_bTitleHighlightPos-1) );             
                _wTotalChapter = (WORD)__dwTemp;           

		_OSDPRGM_UpdateChapterList(); 
                _bCurrentFrame = TITLE_FRAME; /////highlight remains in the Title Frame
            }               
        }                     
        else if (_bCurrentFrame==CHAPTER_INPUT)
        {   
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            _bCurrentFrame = CHAPTER_FRAME;
            _SetClearHighlight(SET_HIGHLIGHT);
        }
        else //CHAPTER_FRAME
        {
            if ( (_wChapterHighlightPos<MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME) && (_wChapterStart+_wChapterHighlightPos-1<_wTotalChapter) ) //change _wChapterHighlightPos only
                _wChapterHighlightPos++;  
            else //change _wChapterStart
            {
                if ((_wChapterStart+MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME-1)<_wTotalChapter)
                {   
                    _wChapterStart++;
                    _ShowChapterList();                 
                }    
            }
        }       
		
        _SetClearHighlight(SET_HIGHLIGHT);
        
        bKeyStatus = KEY_NO_KEY;
        break;
		
		
		//Kevin1.05-3, page down
    case KEY_NEXT:
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
        _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
        _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
#endif
        _SetClearHighlight(CLEAR_HIGHLIGHT);
		
        if (_bCurrentFrame==MAIN_FRAME)
        {   
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
            if (_bMainStart-1+2*MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME<=MAX_PROG_ITEM)
            { 
               //scroll down a whole page
                _bMainStart+=MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME; 
                //_bMainHighlightPos remains the same
                _ShowMainList();
            }
            else if (_bMainStart+_bMainHighlightPos-1+MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME<=MAX_PROG_ITEM) 
            {   
                //scroll down not a whole page
                _bMainStart=MAX_PROG_ITEM-MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME+1;
                // _bMainHighlightPos remains the same 
 		//_bMainHighlightPos = _bMainStart+_bMainHighlightPos-1+MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME-_bMainStart+1;				
                _ShowMainList();
            } 
            else if (_bMainStart+MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME-1<MAX_PROG_ITEM)
            {
                //scroll down not a whole page
                _bMainStart=MAX_PROG_ITEM-MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME+1;
                // _bMainHighlightPos remains the same 
                //_bMainHighlightPos =MAX_PROG_ITEM-_bMainStart+1;   //last one
                _ShowMainList();
            } 
        }            
        else if (_bCurrentFrame==TITLE_FRAME)
        {   
            if (_bTitleStart-1+2*MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME<=_bTotalTitle)
            { 
                //scroll down a whole page
                _bTitleStart+=MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME; 
                //_bTitleHighlightPos remains the same
                _ShowTitleList();
            }
            else if (_bTitleStart+_bTitleHighlightPos-1+MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME<=_bTotalTitle)
            {   
                //scroll down not a whole page
                _bTitleStart=_bTotalTitle-MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME+1;
                // Alan, don't change position
                //_bTitleHighlightPos = _bTitleStart+_bTitleHighlightPos-1+MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME-_bTitleStart+1; 
                _ShowTitleList();
            } 
            else if (_bTitleStart+MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME-1<_bTotalTitle)
            {
                //scroll down not a whole page
                _bTitleStart=_bTotalTitle-MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME+1;
                _bTitleHighlightPos =_bTotalTitle-_bTitleStart+1;   //last one
                _ShowTitleList();
            } 

            if (__wDiscType & BOOK_DVD) //no Chapter Frame for VCD           
            {            
                //uncomment when DVD/VCD is in the loader
                DVD_ParseInfo ( DVD_TOTAL_CHAPTERS, &__dwTemp, (WORD)(_bTitleStart+_bTitleHighlightPos-1) );             
                _wTotalChapter = (WORD)__dwTemp;           
                _OSDPRGM_UpdateChapterList();
                _bCurrentFrame = TITLE_FRAME; /////highlight remains in the Title Frame
            }   
	}                     
        else //CHAPTER_FRAME
        {   
            if (_wChapterStart-1+2*MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME<=_wTotalChapter)
            { 
                //scroll down a whole page
                _wChapterStart+=MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME; 
                //_wChapterHighlightPos remains the same
                _ShowChapterList();
            }
            else if (_wChapterStart+_wChapterHighlightPos-1+MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME<=_wTotalChapter)
            {   
                //scroll down not a whole page
                _wChapterStart=_wTotalChapter-MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME+1;
                //_wChapterHighlightPos remains the same
                //_wChapterHighlightPos = _wChapterStart+_wChapterHighlightPos-1+MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME-_wChapterStart+1;
                _ShowChapterList();
            } 
            else if (_wChapterStart+MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME-1<_wTotalChapter)
            {
                //scroll down not a whole page
                _wChapterStart=_wTotalChapter-MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME+1;
                _wChapterHighlightPos =_wTotalChapter-_wChapterStart+1;   //last one
                _ShowChapterList();
             } 
        }
        _SetClearHighlight(SET_HIGHLIGHT);
			
        bKeyStatus = KEY_NO_KEY;
        break;
			
			
			//Kevin1.05-3, page up
    case KEY_PREV:
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
        _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
#endif
        _SetClearHighlight(CLEAR_HIGHLIGHT);
		
        if (_bCurrentFrame==MAIN_FRAME)
        {   
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
            if (_bMainStart>MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME) //scroll up a whole page
            { 
                _bMainStart-=MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME; 
                //_bMainHighlightPos remains the same
                _ShowMainList();
            }
            else if (_bMainStart+_bMainHighlightPos-1>MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME)
            {                                 //scroll down not a whole page
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
                _bMainStart=0;
#else
                _bMainStart=1;
#endif
                //_bMainHighlightPos remains the same
//                _bMainHighlightPos = _bMainStart+_bMainHighlightPos-1-MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME;   
                _ShowMainList();
            }
            else if (_bMainStart!=1) 
            {
                                               //scroll down not a whole page
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
                _bMainStart=0;
                _bMainHighlightPos=2;   //first one
#else
                _bMainStart=1;
                _bMainHighlightPos=1;   //first one
#endif
                _ShowMainList();
            } 
			
        }            
        else if (_bCurrentFrame==TITLE_FRAME)
        {   
            if (_bTitleStart>MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME) //scroll up a whole page
            { 
                _bTitleStart-=MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME; 
                //_bTitleHighlightPos remains the same
                _ShowTitleList();
            }
            else if (_bTitleStart+_bTitleHighlightPos-1>MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME) //scroll down not a whole page
            {
                _bTitleStart=1;
                // Remain the same position
//		_bTitleHighlightPos = _bTitleStart+_bTitleHighlightPos-1-MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME;
                _ShowTitleList();
            }
            else if (_bTitleStart!=1) //scroll down not a whole page
            {
		_OSDPRGM_UpdateTitleList(); //CoCo1.21, use this function to replace the above code
            } 
            if (__wDiscType & BOOK_DVD) //no Chapter Frame for VCD           
            {            
				//uncomment when DVD/VCD is in the loader
                DVD_ParseInfo ( DVD_TOTAL_CHAPTERS, &__dwTemp, (WORD)(_bTitleStart+_bTitleHighlightPos-1) );             
                _wTotalChapter = (WORD)__dwTemp;           
		_OSDPRGM_UpdateChapterList(); //CoCo1.21, Use this function to replace the above code
                _bCurrentFrame = TITLE_FRAME; /////highlight remains in the Title Frame
            }     
            
        }                     
        else //CHAPTER_FRAME
        {   
            if (_wChapterStart>MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME)
            { 
                //scroll up a whole page
                _wChapterStart-=MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME; 
                //_wChapterHighlightPos remains the same
                _ShowChapterList();
            }
            else if (_wChapterStart+_wChapterHighlightPos-1>MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME) //scroll down not a whole page
            {
                _wChapterStart=1;
                //_wChapterHighlightPos remains the same
//		_wChapterHighlightPos = _wChapterStart+_wChapterHighlightPos-1-MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME;
                _ShowChapterList();
            }
            else if (_wChapterStart!=1) //scroll down not a whole page
            {
		_OSDPRGM_UpdateChapterList(); //CoCo1.21, Use this function to replace the above code
            } 
        }
		
        _SetClearHighlight(SET_HIGHLIGHT);
        
        bKeyStatus = KEY_NO_KEY;
        break;
		
		
    case KEY_LEFT:
        if (MAIN_FRAME == _bCurrentFrame)
        {
            //clear chapter frame, up arrow rect, and down arrow rect
            _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
            _OSDPRGM_FillRect(CHAPTER_FRAME_X0, CHAPTER_FRAME_Y0-1-ARROW_HEIGHT, CHAPTER_FRAME_X1, CHAPTER_FRAME_Y1+1+ARROW_HEIGHT); //CoCo1.20, reduce code size.
        }
        else if (TITLE_INPUT == _bCurrentFrame || TITLE_FRAME == _bCurrentFrame)
        {
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
            if (TITLE_INPUT == _bCurrentFrame)
                _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            _bCurrentFrame = TITLE_FRAME;   
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            _bCurrentFrame = MAIN_FRAME;   
            _SetClearHighlight(SET_HIGHLIGHT);  
            _bTitleHighlightPos = 1;
            _wChapterHighlightPos = 1;
        }
        else if (CHAPTER_INPUT == _bCurrentFrame)
        {
            _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
            _OSDPRGM_FillRect(CHAPTER_FRAME_X0, CHAPTER_FRAME_Y0-1-ARROW_HEIGHT, CHAPTER_FRAME_X1, CHAPTER_FRAME_Y1+1+ARROW_HEIGHT); //CoCo1.20, reduce code size.

            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
            _ShowInput(TITLE_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            _bCurrentFrame = TITLE_FRAME;   
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            _bCurrentFrame = TITLE_INPUT;   
            _SetClearHighlight(SET_HIGHLIGHT);  
            __bProgramTitle = 0;
            __wProgramChapter = 0;
            _bTitleHighlightPos = 1;
            _wChapterHighlightPos = 1;
        }
        else if (CHAPTER_FRAME == _bCurrentFrame)
        {
            __bProgramTitle = 0;
            _ShowInput(TITLE_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            _bCurrentFrame = TITLE_FRAME;   
            _SetClearHighlight(SET_HIGHLIGHT);  
        }
		
        bKeyStatus = KEY_NO_KEY;
        break;
        
        
    case KEY_RIGHT:
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
        if (__wDiscType & BOOK_DVD || __wDiscType & CDROM_M1)
        {
            if (CHAPTER_INPUT == _bCurrentFrame)
            {
                _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
            }
        }
        else
        {
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
        }
#endif
        if ( (_bCurrentFrame==MAIN_FRAME) && !(_bMainStart+_bMainHighlightPos-1) )  //Clear All
        {
            //do nothing
        }
        else
        {   
#ifndef SUPPORT_PRGM_NUM_INPUT 
            if (__wDiscType & BOOK_DVD) //no Chapter Frame for VCD         
            {
                if (_bCurrentFrame!=CHAPTER_FRAME)
                {
                    _SetClearHighlight(CLEAR_HIGHLIGHT);
                    _bCurrentFrame++;
                    _SetClearHighlight(SET_HIGHLIGHT);        
                }    
				
                if (_bCurrentFrame == TITLE_FRAME )
                {
                    //Kevin0.87, add
                    //uncomment when DVD/VCD is in the loader
                    DVD_ParseInfo ( DVD_TOTAL_CHAPTERS, &__dwTemp, (WORD)(_bTitleStart+_bTitleHighlightPos-1) );             
                    _wTotalChapter = __dwTemp;           

                    _ShowChapterList();
                    _bCurrentFrame = TITLE_FRAME; /////highlight remains in Title Frame      
                }
            }
#else //#ifndef SUPPORT_PRGM_NUM_INPUT
            __wNumberQueue = 0;    //clear
			
            if (__wDiscType & BOOK_DVD) //no Chapter Frame for VCD         
            {
                if (_bCurrentFrame==MAIN_FRAME)
                {
                    //clear whole yellow highlight bar
                    _SetClearHighlight(CLEAR_HIGHLIGHT);
                    __bProgramCurrentIndex = _bMainStart+_bMainHighlightPos-1;
						
                    _bOSDPRGMTemp = __ProgIdxList[__bProgramCurrentIndex-1].bMainIndex;

                        //CoCo1.21, replace the above code
                        if ((!__bProgramTitle) && _bOSDPRGMTemp)
                            __bProgramTitle = _bOSDPRGMTemp;

                        _ShowInput(TITLE_INPUT, OSDPRGM_ENTRY_COLOR_HILIGHT);
                        
                        //CoCo1.21, replace the above code
                        _OSDPRGM_UpdateProgramChapter(2);

                        _bCurrentFrame = TITLE_INPUT; 
                }    
                else if (_bCurrentFrame==TITLE_INPUT) 
                {
                   if (0 != __bProgramTitle && __bProgramTitle <= _bTotalTitle)
                   {
                       DVD_ParseInfo(DVD_TOTAL_CHAPTERS, &__dwTemp,
                                     (WORD)(__bProgramTitle));
                       _SetClearHighlight(CLEAR_HIGHLIGHT);
                       _wTotalChapter = (WORD)__dwTemp;    
                       _bTitleStart = __bProgramTitle;
                       _bTitleHighlightPos = 1;
                       _ShowTitleList();
                       _ShowChapterList();
                       _bCurrentFrame = CHAPTER_INPUT;
                       _SetClearHighlight(SET_HIGHLIGHT); 
                   } 
                   else 
                   {
                       __bProgramTitle = 0;
                       _SetClearHighlight(SET_HIGHLIGHT); 
                       bKeyStatus = KEY_INVALID;
                       break;
                   }
                }
                else if (_bCurrentFrame==TITLE_FRAME) 
                {
                    __bProgramTitle = _bTitleStart + _bTitleHighlightPos - 1;
                    _ShowInput(TITLE_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);
                    _SetClearHighlight(CLEAR_HIGHLIGHT);
                    _bCurrentFrame = CHAPTER_FRAME;
                    _SetClearHighlight(SET_HIGHLIGHT); 
                }
            }
			
#endif //#ifndef SUPPORT_PRGM_NUM_INPUT            
            else //BOOK_VCD
            {
                if (_bCurrentFrame==MAIN_FRAME)
                {
                    _SetClearHighlight(CLEAR_HIGHLIGHT);
                    _bCurrentFrame = TITLE_INPUT;
                    _SetClearHighlight(SET_HIGHLIGHT);        
                }      
            }               
        }
        
        bKeyStatus = KEY_NO_KEY;
        break;
		
		
    case KEY_ENTER: //Kevin1.07, combine all the following keys
    case KEY_PLAY_PAUSE: //Kevin1.06, add
    case KEY_PLAY: //Kevin0.87, KEY_PLAY act as KEY_ENTER for some cases

        if (__btPause)
        {
            // Alan 1.20, in Pause mode, sync the behavior with MM program  
            bKeyStatus = KEY_BYPASS;
        }
        else
        {

        //current index in Main Frame
        __bProgramCurrentIndex = _bMainStart+_bMainHighlightPos-1;
        
        if ((__wDiscType&BOOK_DVD || __wDiscType&CDROM_M1) && ( _bCurrentFrame == CHAPTER_FRAME || _bCurrentFrame == CHAPTER_INPUT))  
        {
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
            {
                if (_bCurrentFrame == CHAPTER_FRAME)
                    LINEAR_ProgramEntryOperation(LINEAR_ADD_ENTRY_BY_INDEX,
                         ((BYTE)(__bProgramCurrentIndex-1)),
                         ((BYTE)(_bTitleStart+_bTitleHighlightPos-1)),
                         ((WORD)(_wChapterStart+_wChapterHighlightPos-1)));
                else if (_bCurrentFrame == CHAPTER_INPUT)
                {
                    if (0 != __wProgramChapter && __wProgramChapter <= _wTotalChapter)
                    {
                        LINEAR_ProgramEntryOperation(LINEAR_ADD_ENTRY_BY_INDEX,
                             ((BYTE)(__bProgramCurrentIndex-1)),
                             __bProgramTitle,
                             __wProgramChapter);
                    }
                    else
                    {
                        __wNumberQueue = 0;
                        __wProgramChapter = 0;
                        _SetClearHighlight(SET_HIGHLIGHT); 
                        bKeyStatus = KEY_INVALID;
                        break;
                    }
                }
            }

            //clear chapter frame, up arrow rect, and down arrow rect
            _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
            _OSDPRGM_FillRect(CHAPTER_FRAME_X0, CHAPTER_FRAME_Y0-1-ARROW_HEIGHT, CHAPTER_FRAME_X1, CHAPTER_FRAME_Y1+1+ARROW_HEIGHT); //CoCo1.20, reduce code size.
            _bTitleStart = 1;
            _bTitleHighlightPos = 1;
            _wChapterStart = 1;
            _wChapterHighlightPos = 1;
            _ShowTitleList();

            _bCurrentFrame = MAIN_FRAME;
            _SetClearHighlight(SET_HIGHLIGHT);

            //Kevin1.23DMTech, proceed to next entry when current entry is set
            _OSDPRGM_ProceedToNextMainEntry();

            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
            _ShowInput(TITLE_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);
            _ShowInput(CHAPTER_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);
			
            bKeyStatus = KEY_NO_KEY;
        }
        else if ( (__wDiscType&BOOK_DVD || __wDiscType&CDROM_M1) && _bCurrentFrame==TITLE_FRAME)
        {
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
            __bProgramTitle = _bTitleStart + _bTitleHighlightPos - 1;
            _ShowInput(TITLE_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            if (_wTotalChapter) //move the highlight to Chapter Frame
                _bCurrentFrame = CHAPTER_FRAME;
            else //empty dir => move the highlight to Main Frame
                _bCurrentFrame = MAIN_FRAME;
            _SetClearHighlight(SET_HIGHLIGHT);
			
            bKeyStatus = KEY_NO_KEY;
        }   
        else if ( (__wDiscType&BOOK_DVD || __wDiscType&CDROM_M1) && _bCurrentFrame==TITLE_INPUT)
        {
            __wNumberQueue = 0;    //clear
            if (__wDiscType & BOOK_DVD) //no Chapter Frame for VCD
            {
                if (0 != __bProgramTitle && __bProgramTitle <= _bTotalTitle)
                {
                    DVD_ParseInfo(DVD_TOTAL_CHAPTERS, &__dwTemp,
                                     (WORD)(__bProgramTitle));
                    _SetClearHighlight(CLEAR_HIGHLIGHT);
                    _wTotalChapter = (WORD)__dwTemp;
                    _bTitleStart = __bProgramTitle;
                    _bTitleHighlightPos = 1;
                    _ShowTitleList();
                    _ShowChapterList();
                    _bCurrentFrame = CHAPTER_INPUT;
                    _SetClearHighlight(SET_HIGHLIGHT);
                    bKeyStatus = KEY_NO_KEY;
                    break;
                }
                else
                {
                    __bProgramTitle = 0;
                    _SetClearHighlight(SET_HIGHLIGHT); 
                    bKeyStatus = KEY_INVALID;
                    break;
                }
            } 
        }
        else if ( !(__wDiscType&BOOK_DVD || __wDiscType&CDROM_M1) && (_bCurrentFrame==TITLE_FRAME || TITLE_INPUT == _bCurrentFrame) ) //VCD
        {
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL_0);
            if (TITLE_FRAME == _bCurrentFrame)
            {
                LINEAR_ProgramEntryOperation(LINEAR_ADD_ENTRY_BY_INDEX,
                    ((BYTE)(__bProgramCurrentIndex-1)), NULL_0, 
                    ((WORD)(_bTitleStart+_bTitleHighlightPos-1))); 
                _bTitleStart = 1;
                _bTitleHighlightPos = 1;
                _ShowTitleList();
            }
            else
            {
                if (0 != __bProgramTitle && __bProgramTitle <= _bTotalTitle)
                {
                    LINEAR_ProgramEntryOperation(LINEAR_ADD_ENTRY_BY_INDEX,
                        ((BYTE)(__bProgramCurrentIndex-1)),
                        NULL_0, ((WORD)(__bProgramTitle))); 
                } 
                else
                {
                    bKeyStatus = KEY_INVALID;
                    break;
                }
            }

//            _SetClearHighlight(CLEAR_HIGHLIGHT);
            _bCurrentFrame = MAIN_FRAME;
            _SetClearHighlight(SET_HIGHLIGHT);

            //Kevin1.23DMTech, proceed to next entry when current entry is set
            _OSDPRGM_ProceedToNextMainEntry();            

            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
            _ShowInput(TITLE_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);

            bKeyStatus = KEY_NO_KEY;
        }                   
#ifdef SUPPORT_CLEARALL_IN_PROGRAM                             
        else if ( (_bCurrentFrame==MAIN_FRAME) && !__bProgramCurrentIndex )  //Clear All
        {
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CHECK, NULL);
            _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL);
            LINEAR_ProgramEntryOperation(LINEAR_CLEAR_ALL_ENTRY,NULL,NULL,NULL);
            _ShowMainList();
            _bMainHighlightPos = 2;  //highlight 1. __-__ 
            _SetClearHighlight(SET_HIGHLIGHT);      
            
            bKeyStatus = KEY_NO_KEY;
        }
#endif              
/*Alan2.30, remove the code to make pressing KEY_PLAY in the Main Frame will to play the program list.
        else if (_bCurrentFrame == MAIN_FRAME)
        {
            if (0 == __ProgIdxList[__bProgramCurrentIndex - 1].bMainIndex &&
                0 == __ProgIdxList[__bProgramCurrentIndex - 1].wSubIndex)
            {
                bKeyStatus = KEY_NO_KEY;
            }
        }
*/

        if (KEY_NO_KEY != bKeyStatus)
        {
            if (KEY_PLAY_PAUSE == __bKey)
                __bKey = KEY_PLAY;
            _OSDPRGM_ClearUI();
            OSD_ChangeUI(OSD_UI_DVD_PROGRAM, OSD_EXIT_UI);
            LINEAR_Program(LINEAR_COUNT_ENTRY, NULL_0);
            bKeyStatus = KEY_TODO; 
        }

/*alan080, comment
        //else _bCurrentFrame == MAIN_FRAME other than Clear All
        //cc.c take care of KEY_PLAY/KEY_PLAY_PAUSE                       
        if (__bKey==KEY_ENTER)
            bKeyStatus = KEY_NO_KEY; 
*/
        
        }
        break;
		
    case KEY_CLEAR:
    case KEY_STOP:
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
        _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
#endif
        if ( _bCurrentFrame==MAIN_FRAME) 
        {
            __bProgramCurrentIndex = _bMainStart+_bMainHighlightPos-1; //current index in Main Frame
			
            if (!__bProgramCurrentIndex ) //Clear All 
            {
                //LINEAR_ClearProgramEntry(LINEAR_CLEARENTRY,NULL,NULL); //__wProgramData[0~MAX_PROG_ITEM-1]=0
                LINEAR_ProgramEntryOperation(LINEAR_CLEAR_ALL_ENTRY,NULL_0,NULL_0,NULL_0);
                _ShowMainList();
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
                _bMainHighlightPos = 2;  //highlight 1. __-__ 
#else
                _bMainHighlightPos = 1;  //highlight 1. __-__ 
#endif 
                _SetClearHighlight(SET_HIGHLIGHT);        
            }
            else //Clear one Entry
            {
                /////__wProgramData[__bTemp-1] = 0x0000;
                LINEAR_ProgramEntryOperation(
                     LINEAR_DELETE_ENTRY_BY_INDEX_WO_REARRANGE,
                     ((BYTE)(__bProgramCurrentIndex-1)),
                     NULL_0, NULL_0);
                
                _SetClearHighlight(SET_HIGHLIGHT);
            }                
        }
        else 
        {
            //clear chapter frame, up arrow rect, and down arrow rect
            _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
            _OSDPRGM_FillRect(CHAPTER_FRAME_X0, CHAPTER_FRAME_Y0-1-ARROW_HEIGHT, CHAPTER_FRAME_X1,CHAPTER_FRAME_Y1+1+ARROW_HEIGHT); //CoCo1.20, reduce code size.

            if (__wDiscType & BOOK_DVD) //no Chapter Frame for VCD
            {
                __wProgramChapter = 0;
                _ShowInput(CHAPTER_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);
            }
            __bProgramTitle = 0;
            _ShowInput(TITLE_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);

            _bCurrentFrame=TITLE_FRAME;
            _SetClearHighlight(CLEAR_HIGHLIGHT);
            _bCurrentFrame=MAIN_FRAME;
            _SetClearHighlight(SET_HIGHLIGHT);            
        }            
        
        bKeyStatus = KEY_NO_KEY;
        break;
        
		//Kevin0.91, let original flow deal with PROGRAM,POWER,OPEN,...
    case KEY_POWER:
    case KEY_OPEN_CLOSE:
    case KEY_PROGRAM:
#ifndef FORCE_SWITCH_NONPBC
        if (__btPBC && (__wDiscType != BOOK_CDDA))
        {
            bKeyStatus = KEY_INVALID;
        }
        else
#endif
        {
            if (!__btInputProgram)
            {
                if ( !LINEAR_Program (LINEAR_PROGRAM_MODE,NULL_0) )
                {
                    bKeyStatus = INVALID_KEY;
                }
                else
                {
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
//                    OSDPRGM_EnterByPrestop();
                    CC_ForceStop(CC_FORCE_STOP_FROM_PROGRAM);
#endif
                    OSD_ChangeUI(OSD_UI_DVD_PROGRAM, OSD_ENTER_UI);


                    _OSDPRGM_ShowUI();
                    bKeyStatus = KEY_NO_KEY;
                }
            }
            else
            {
                OSDPRGM_Exit(OSD_UI_EXIT_ALL);
                OSD_ChangeUI(OSD_UI_DVD_PROGRAM, OSD_EXIT_UI);
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
//                OSDPRGM_ExitByResume();
                CC_ForceResumeRePlay(CC_FORCE_STOP_FROM_PROGRAM);
#endif
                LINEAR_Program(LINEAR_PROGRAM_CLEAR,NULL_0);
                bKeyStatus = KEY_NO_KEY;
            }
        }
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
        if (TITLE_INPUT ==  _bCurrentFrame || CHAPTER_INPUT == _bCurrentFrame) 
        {
            if (_OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_NUMBER, ((WORD)(__bKey-KEY_N0)) ))
                bKeyStatus =KEY_NO_KEY;
            else
                bKeyStatus = KEY_INVALID; //OSD show "Invalid"
        }
        else
            bKeyStatus = KEY_NO_KEY;
        break;
#endif
		//Kevin0.91,doesn't allow other keys
    default: 
        bKeyStatus = KEY_INVALID;
        break;
    }
	
    return bKeyStatus;
	
} //end of OSDPRGM_ProcessKey



//  *********************************************************************************
//  Function    :   OSDPRGM_Recover
//  Description :   The function is called by OSD to do the recovery of Program UI 
//  Arguments   :   bRecoverRegion, the recover region 
//		:   OSD_RECOVER_ALL 
//		:   OSD_RECOVER_UPPER_REGION, invalid
//		:   OSD_RECOVER_BOTTOM_REGION, invalid
//  Return      :   TRUE, The region has been recovered.
//              :   FALSE, The region doesn't need to be recovered in this UI.
//  Side Effect :
//  *********************************************************************************
BYTE OSDPRGM_Recover(BYTE bRecoverRegion)
{
    if (OSD_RECOVER_ALL != bRecoverRegion) return FALSE;
    _OSDPRGM_ShowUI();
    return TRUE;
}

//  *********************************************************************************
//  Function    :   _ShowMainList
//  Description :   Show Main List in the Main Frame
//  Arguments   :   None
//  Return      :   None
//  Side Effect :   _bCurrentFrame might be changed
//  Note        :   Why not combine _ShowMainList(), _ShowTitleList(), and _ShowFrameList() into _ShowList()?
//                  Because the exactly common code is so little although they're similar. 
//  *********************************************************************************
static void _ShowMainList(void)
{
    _bCurrentFrame = MAIN_FRAME; /////_ShowOneEntry() will refer to
    
    //redraw main frame (can also be moved to the following of //main frame)                 
    _OSDPRGM_DrawFrame(MAIN_FRAME_X0, MAIN_FRAME_Y0, MAIN_FRAME_X1, MAIN_FRAME_Y1); //CoCo1.20, reduce code size.


    for (_bOSDPRGMTemp1=_bMainStart; (_bOSDPRGMTemp1<=MAX_PROG_ITEM) && ((_bOSDPRGMTemp1-_bMainStart)<MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME); _bOSDPRGMTemp1++)
    {      
        _ShowOneEntry(_bOSDPRGMTemp1);
    }                    

    //show arrow down rect
    if ( (MAX_PROG_ITEM-_bMainStart+1) > MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME)
    {   
        _OSDPRGM_DrawFrame(MAIN_FRAME_X0, MAIN_FRAME_Y1+1,
            MAIN_FRAME_X1, MAIN_FRAME_Y1+1+ARROW_HEIGHT);  //CoCo1.20, reduce code size.

        _OSDPRGM_SetStringPos(((MAIN_FRAME_X0+MAIN_FRAME_X1)>>1)-7, MAIN_FRAME_Y1+4); //CoCo1.20, reduce code size.
        _OSDPRGM_DrawDynamicString(aArrowDown); //CoCo1.20, reduce code size.
    } 
    else //clear arrow down rect
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
        _OSDPRGM_FillRect(MAIN_FRAME_X0, MAIN_FRAME_Y1+1, MAIN_FRAME_X1, MAIN_FRAME_Y1+1+ARROW_HEIGHT); //CoCo1.20, reduce code size.
    }        
    
    //show arrow up rect
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
    if ( _bMainStart >0)
#else
    if ( _bMainStart >1)
#endif
    {   
        _OSDPRGM_DrawFrame(MAIN_FRAME_X0, MAIN_FRAME_Y0-1-ARROW_HEIGHT, 
            MAIN_FRAME_X1, MAIN_FRAME_Y0-1); //CoCo1.20, reduce code size.

        _OSDPRGM_SetStringPos(((MAIN_FRAME_X0+MAIN_FRAME_X1)>>1)-7, MAIN_FRAME_Y0-ARROW_HEIGHT+2); //CoCo1.20, reduce code size
        _OSDPRGM_DrawDynamicString(aArrowUp); 
    }  
    else //clear arrow up rect
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
        _OSDPRGM_FillRect(MAIN_FRAME_X0, MAIN_FRAME_Y0-1-ARROW_HEIGHT,MAIN_FRAME_X1, MAIN_FRAME_Y0-1); //CoCo1.20, reduce code size.
    }                              
    
} //end of _ShowMainList()



//  *********************************************************************************
//  Function    :   _ShowTitleList
//  Description :   Show Title List in the Title Frame 
//  Arguments   :   None
//  Return      :   None
//  Side Effect :   _bCurrentFrame might be changed
//  *********************************************************************************
static void _ShowTitleList(void)
{       
    _bCurrentFrame = TITLE_FRAME; /////_ShowOneEntry() will refer to
       
    //redraw title frame (can also be moved to the following of //title frame)     
    _OSDPRGM_DrawFrame(TITLE_FRAME_X0, TITLE_FRAME_Y0,
                      TITLE_FRAME_X1, TITLE_FRAME_Y1); //CoCo1.20, reduce code size.

    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT2;
    _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_FRAME_CENTER;

    for (_bOSDPRGMTemp1=_bTitleStart; (_bOSDPRGMTemp1<=_bTotalTitle) && (_bOSDPRGMTemp1-_bTitleStart)<MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME; _bOSDPRGMTemp1++)
    {      
        _ShowOneEntry(_bOSDPRGMTemp1);
    }                    

    //show arrow down rect
    if ( (_bTotalTitle-_bTitleStart+1) > MAX_ENTRY_NUM_SHOWN_IN_TITLE_FRAME)
    {   
        _OSDPRGM_DrawFrame(TITLE_FRAME_X0, TITLE_FRAME_Y1+1,
            TITLE_FRAME_X1, TITLE_FRAME_Y1+1+ARROW_HEIGHT); //CoCo1.20, reduce code size.

        _OSDPRGM_SetStringPos(((TITLE_FRAME_X0+TITLE_FRAME_X1)>>1)-7, TITLE_FRAME_Y1+4); //CoCo1.20, reduce code size.
        _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_FRAME_CENTER;
        _OSDPRGM_DrawDynamicString(aArrowDown);
    } 
    else //clear arrow down rect
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
        _OSDPRGM_FillRect(TITLE_FRAME_X0, TITLE_FRAME_Y1+1,TITLE_FRAME_X1, TITLE_FRAME_Y1+1+ARROW_HEIGHT); //CoCo1.20, reduce code size.
    }        
    
    //show arrow up rect
    if ( _bTitleStart >1)
    {             
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_FRAME_CENTER;
        _OSDPRGM_DrawFrame(TITLE_FRAME_X0,
                          TITLE_FRAME_Y0-1-ARROW_HEIGHT,
                          TITLE_FRAME_X1,
                          TITLE_FRAME_Y0-1);  //CoCo1.20, reduce code size.

        _OSDPRGM_SetStringPos(((TITLE_FRAME_X0+TITLE_FRAME_X1)>>1)-7, TITLE_FRAME_Y0-ARROW_HEIGHT+2); //CoCo1.20, reduce code size.
        _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_FRAME_CENTER;
        _OSDPRGM_DrawDynamicString(aArrowUp);
    }  
    else //clear arrow up rect
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
        _OSDPRGM_FillRect(TITLE_FRAME_X0, TITLE_FRAME_Y0-1-ARROW_HEIGHT, TITLE_FRAME_X1, TITLE_FRAME_Y0-1); //CoCo1.20, reduce code size.
    }     

} //end of _ShowTitleList()



//  *********************************************************************************
//  Function    :   _ShowChapterList
//  Description :   Show Chapter List in the Chapter Frame
//  Arguments   :   None
//  Return      :   None
//  Side Effect :   _bCurrentFrame might be changed
//  *********************************************************************************  
static void _ShowChapterList(void)
{   
    _bCurrentFrame = CHAPTER_FRAME; /////_ShowOneEntry() will refer to
    
    //redraw chapter frame, otherwise some texts will remain on the frame
    {
        _wOSDPRGMString[0] = 2;
        _wOSDPRGMString[1] = CHAR_SPACE;
        _wOSDPRGMString[2] = CHAR_SPACE;

        _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT2;
        _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_MAIN_COLOR_BG;
        _OSDPRGM_SetStringPos(CHAPTER_FRAME_X0 + FRAME_THICKNESS + 15, CHAPTER_FRAME_Y0-2*ENTRY_HEIGHT); //CoCo1.20, reduce code size.
        _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.

        _OSDPRGM_DrawFrame(CHAPTER_FRAME_X0, CHAPTER_FRAME_Y0,
            CHAPTER_FRAME_X1-20, CHAPTER_FRAME_Y1); //CoCo1.20, reduce code size.
    }     
    
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT2;
    _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_FRAME_CENTER;

    for (_wOSDPRGMTemp=_wChapterStart; (_wOSDPRGMTemp<=_wTotalChapter) && (_wOSDPRGMTemp-_wChapterStart)<MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME; _wOSDPRGMTemp++)
    {      
        _ShowOneEntry(_wOSDPRGMTemp);    
    }                    

    //show arrow down rect
    if ( (_wTotalChapter-_wChapterStart+1) > MAX_ENTRY_NUM_SHOWN_IN_CHAPTER_FRAME)
    {         
        {   
            _OSDPRGM_DrawFrame(CHAPTER_FRAME_X0, CHAPTER_FRAME_Y1+1,
                CHAPTER_FRAME_X1-20, CHAPTER_FRAME_Y1+1+ARROW_HEIGHT); //CoCo1.20, reduce code size.

            _OSDPRGM_SetStringPos(((CHAPTER_FRAME_X0+CHAPTER_FRAME_X1-20)>>1)-7, CHAPTER_FRAME_Y1+4); //CoCo1.20, reduce code size.
            _OSDPRGM_DrawDynamicString(aArrowDown);
        }        
    } 
    else //clear arrow down rect
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
       _OSDPRGM_FillRect(CHAPTER_FRAME_X0, CHAPTER_FRAME_Y1+1, CHAPTER_FRAME_X1, CHAPTER_FRAME_Y1+1+ARROW_HEIGHT); //CoCo1.20, reduce code size.
    }
         
    //show arrow up rect
    if ( _wChapterStart >1)
    {           
        {   
            _OSDPRGM_DrawFrame(CHAPTER_FRAME_X0, CHAPTER_FRAME_Y0-1-ARROW_HEIGHT,
                CHAPTER_FRAME_X1-20, CHAPTER_FRAME_Y0-1); //CoCo1.20, reduce code size.
            _OSDPRGM_SetStringPos(((CHAPTER_FRAME_X0+CHAPTER_FRAME_X1-20)>>1)-7, CHAPTER_FRAME_Y0-ARROW_HEIGHT+2); //CoCo1.20, reduce code size.
            _OSDPRGM_DrawDynamicString(aArrowUp);
        }
    }     
    else //clear arrow up rect
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
        _OSDPRGM_FillRect(CHAPTER_FRAME_X0, CHAPTER_FRAME_Y0-1-ARROW_HEIGHT, CHAPTER_FRAME_X1, CHAPTER_FRAME_Y0-1); //CoCo1.20, reduce code size.
    }
           
} //end of _ShowChapterList()



//  *********************************************************************************
//  Function    :   _SetClearHighlight
//  Description :   Set/Clear Highlight & Show the current entry
//  Arguments   :   bFlag: SET_HIGHLIGHT/CLEAR_HIGHLIGHT
//  Implicit Arg:  _bCurrentFrame
//  Return      :   None
//  Side Effect :
//  *********************************************************************************
static void _SetClearHighlight(BYTE bFlag)
{   
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT2;

    if (bFlag) //SET_HIGHLIGHT
    {
        _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_COLOR_HILIGHT;
        _bOSDPRGMTemp = OSDPRGM_ENTRY_COLOR_HILIGHT;      
    }
    else //CLEAR_HIGHLIGHT
    {
        if ((__bKey == KEY_RIGHT || KEY_PLAY == __bKey || KEY_ENTER == __bKey) && (MAIN_FRAME != _bCurrentFrame))
        {
            if (__wDiscType&BOOK_DVD || __wDiscType&CDROM_M1)
            {
                    _bOSDPRGMTemp = OSDPRGM_ENTRY_TEXT_DISABLED; 
            }
            else //VCD
                _bOSDPRGMTemp = OSDPRGM_ENTRY_TEXT_DISABLED; 
        } else {
            if (TITLE_INPUT == _bCurrentFrame || CHAPTER_INPUT == _bCurrentFrame)
                _bOSDPRGMTemp = OSDPRGM_ENTRY_MAIN_COLOR_BG;
            else
                _bOSDPRGMTemp = OSDPRGM_ENTRY_FRAME_CENTER;
        }
        
       _PRGM_StrAttr.bBackground = _bOSDPRGMTemp;
    }        
  

    _PRGM_RectAttr.bColor = _bOSDPRGMTemp;
    if (_bCurrentFrame == MAIN_FRAME)
    {
        _OSDPRGM_FillRect(MAIN_FRAME_X0+FRAME_THICKNESS, 
            ((WORD)(MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(_bMainHighlightPos-1))),
            MAIN_FRAME_X1-FRAME_THICKNESS,
            ((WORD)(MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(_bMainHighlightPos-1)+ENTRY_HEIGHT)));  //CoCo1.20, reduce code size.
        _ShowOneEntry(((WORD)(_bMainStart+_bMainHighlightPos-1)));  
    }   
    else if (_bCurrentFrame == TITLE_INPUT)
    {
        _ShowInput(TITLE_INPUT, _bOSDPRGMTemp);
    }
    else if (_bCurrentFrame == TITLE_FRAME)
    {
        _OSDPRGM_FillRect(
            TITLE_FRAME_X0+FRAME_THICKNESS,
            ((WORD)(TITLE_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(_bTitleHighlightPos-1))), 
            TITLE_FRAME_X1-FRAME_THICKNESS,
            ((WORD)(TITLE_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(_bTitleHighlightPos-1)+ENTRY_HEIGHT))); //CoCo1.20, reduce code size.
        _ShowOneEntry(((WORD)(_bTitleStart+_bTitleHighlightPos-1)));   
    }  
    else if (_bCurrentFrame == CHAPTER_INPUT)
    {
        _ShowInput(CHAPTER_INPUT, _bOSDPRGMTemp);
    }
    else //CHAPTER_FRAME
    {
        {
            _OSDPRGM_FillRect(
                CHAPTER_FRAME_X0+FRAME_THICKNESS, 
                ((WORD)(CHAPTER_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(_wChapterHighlightPos-1))), 
                CHAPTER_FRAME_X1-20-FRAME_THICKNESS,
                ((WORD)(CHAPTER_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(_wChapterHighlightPos-1)+ENTRY_HEIGHT))); //CoCo1.20, reduce code size.
        }

        _ShowOneEntry(((WORD)(_wChapterStart+_wChapterHighlightPos-1))); 
    }   

} //end of _SetClearHighlight()   


//  *********************************************************************************
//  Function    :   _ShowInput
//  Description :   Show One Entry in either Main, Title, or Chapter Frame. 
//  Arguments   :   wEntry: main frame entry #, title #, or chapter #
//  Implicit Arg:  _bCurrentFrame
//  Return      :   None
//  Side Effect :
//  *********************************************************************************
static void _ShowInput(BYTE bInput, BYTE bBgColor)
{ 
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT2;
    _PRGM_StrAttr.bBackground = bBgColor;
    _wOSDPRGMString[0] = 3;
    _wOSDPRGMString[1] = CHAR_SPACE;
    _wOSDPRGMString[2] = CHAR_SPACE;
    _wOSDPRGMString[3] = CHAR_SPACE;

    switch(bInput)
    {
    case TITLE_INPUT:
        if (0 != __bProgramTitle)
        {
            _OSDPRGM_OutputNum(__bProgramTitle, 2);
        }
        /* --CoCo1.20, reduce code size.
        else
        {
            _wOSDPRGMString[0] = 3;
            _wOSDPRGMString[1] = CHAR_SPACE;
            _wOSDPRGMString[2] = CHAR_SPACE;
            _wOSDPRGMString[3] = CHAR_SPACE;
        }
        */
        _OSDPRGM_SetStringPos(TITLE_FRAME_X0 + FRAME_THICKNESS + 15, TITLE_FRAME_Y0-2*ENTRY_HEIGHT); //CoCo1.20, reduce code size.

        break;
    case CHAPTER_INPUT:
        if (0 != __wProgramChapter)
        {
            _OSDPRGM_OutputNum(__wProgramChapter, 2);
        }
        /* --CoCo1.20, reduce code size.
        else
        {
            _wOSDPRGMString[0] = 3;
            _wOSDPRGMString[1] = CHAR_SPACE;
            _wOSDPRGMString[2] = CHAR_SPACE;
            _wOSDPRGMString[3] = CHAR_SPACE;
        }
        */
        _OSDPRGM_SetStringPos(CHAPTER_FRAME_X0 + FRAME_THICKNESS + 15, CHAPTER_FRAME_Y0-2*ENTRY_HEIGHT); //CoCo1.20, reduce code size.
        break;
    }

    _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.

    _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TEXT2; 

    _OSDPRGM_FillRect(
        _PRGM_StrAttr.wX, //CoCo1.20, reduce code size.
        _PRGM_StrAttr.wY + GDI_GetStringHeight(_wOSDPRGMString)+1,
        _PRGM_StrAttr.wX + GDI_GetStringWidth(_wOSDPRGMString),
        _PRGM_StrAttr.wY + GDI_GetStringHeight(_wOSDPRGMString)+3); //CoCo1.20, reduce code size.
}

//  *********************************************************************************
//  Function    :   _ShowOneEntry
//  Description :   Show One Entry in either Main, Title, or Chapter Frame. 
//  Arguments   :   wEntry: main frame entry #, title #, or chapter #
//  Implicit Arg:  _bCurrentFrame
//  Return      :   None
//  Side Effect :
//  *********************************************************************************
static void _ShowOneEntry(WORD wEntry)
{ 
    if (_bCurrentFrame == MAIN_FRAME)
    {
#ifdef SUPPORT_CLEARALL_IN_PROGRAM
        if (!wEntry) //0
        {
            _wOSDPRGMString[0] = 9; //9 digits 
            _wOSDPRGMString[1] = CHAR_C;
            _wOSDPRGMString[2] = CHAR_ll;
            _wOSDPRGMString[3] = CHAR_ee; 
            _wOSDPRGMString[4] = CHAR_aa;
            _wOSDPRGMString[5] = CHAR_rr;
            _wOSDPRGMString[6] = CHAR_SPACE;
            _wOSDPRGMString[7] = CHAR_A; 
            _wOSDPRGMString[8] = CHAR_ll;
            _wOSDPRGMString[9] = CHAR_ll;

            _OSDPRGM_SetStringPos(MAIN_FRAME_X0+FRAME_THICKNESS+20, MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart); //CoCo1.20, reduce code size.
            _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.
        }            
        else //1,2,...,9
#endif
        {
            if (__wDiscType&BOOK_DVD)
            {
                _wOSDPRGMString[0] = 3; //digits       1.     - 
                //Kevin1.22, expand program entry to more than 10 
                _wOSDPRGMString[1] = aNDNumbers[wEntry/10]; //CoCo1.20, reduce code size

                if (wEntry<10)
                    _wOSDPRGMString[1] = CHAR_SPACE;
                _wOSDPRGMString[2] = aNDNumbers[wEntry%10]; //CoCo1.20, reduce code size
                _wOSDPRGMString[3] = CHAR_FULL_STOP;

                _OSDPRGM_SetStringPos(MAIN_FRAME_X0+FRAME_THICKNESS+20, MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart)); //CoCo1.20, reduce code size.
                _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.

                if (!__ProgIdxList[wEntry-1].wSubIndex) //0: not set yet
                {
                    _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TEXT2;
                    _OSDPRGM_FillRect(
                         MAIN_FRAME_X0+FRAME_THICKNESS+75,
                         ((WORD)(MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart+1) - 5)), 
                         MAIN_FRAME_X0+FRAME_THICKNESS+75 + 120,
                         ((WORD)(MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart+1) + 3 - 5))); //CoCo1.20, reduce code size.
                }
                else                                 //12 34
                {  
                     _wOSDPRGMString[0] = 4; //2 digits     
                     _bOSDPRGMTemp = __ProgIdxList[wEntry-1].bMainIndex; 
                     _wOSDPRGMString[1] = aNDNumbers[_bOSDPRGMTemp/10];//CoCo1.20, reduce code size
                    _wOSDPRGMString[2] = aNDNumbers[_bOSDPRGMTemp%10]; //CoCo1.20, reduce code size
                     _wOSDPRGMString[3] = CHAR_HYPHEN_MINUS;
                     _wOSDPRGMString[4] = CHAR_HYPHEN_MINUS;
                     _OSDPRGM_SetStringPos(MAIN_FRAME_X0+FRAME_THICKNESS+75, MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart)); //CoCo1.20, reduce code size.
                     _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.

                     _wOSDPRGMString[0] = 2; //2 digits     
                     __wTemp = __ProgIdxList[wEntry-1].wSubIndex;
                     _wOSDPRGMString[1] = aNDNumbers[__wTemp/10]; //CoCo1.20, reduce code size
                     _wOSDPRGMString[2] = aNDNumbers[__wTemp%10]; //CoCo1.20, reduce code size   
                     _OSDPRGM_SetStringPos(MAIN_FRAME_X0+FRAME_THICKNESS+125, MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart)); //CoCo1.20, reduce code size.
                     _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.
                }  
            }
            else //VCD  
            {
                _wOSDPRGMString[0] = 3;
                _wOSDPRGMString[1] = aNDNumbers[wEntry/10];//CoCo1.20, reduce code size
                if (wEntry<10)
                    _wOSDPRGMString[1] = CHAR_SPACE;
                _wOSDPRGMString[2] = aNDNumbers[wEntry%10]; //CoCo1.20, reduce code size
                _wOSDPRGMString[3] = CHAR_FULL_STOP;
                _OSDPRGM_SetStringPos(MAIN_FRAME_X0+FRAME_THICKNESS+20, MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart)); //CoCo1.20, reduce code size.
                _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.

                if (!__ProgIdxList[wEntry-1].wSubIndex) //0: not set yet
                {                  
                    _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TEXT2;
                    _OSDPRGM_FillRect(
                        MAIN_FRAME_X0+FRAME_THICKNESS+75,
                        ((WORD)(MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart+1) - 5)), 
                        MAIN_FRAME_X0+FRAME_THICKNESS+75 + 120,
                        ((WORD)(MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart+1) + 3 - 5))); //CoCo1.20, reduce code size.
                }
                else
                {
                    __wTemp = __ProgIdxList[wEntry-1].wSubIndex; //     12 
                    _wOSDPRGMString[1] = aNDNumbers[__wTemp/10]; //CoCo1.20, reduce code size
                    _wOSDPRGMString[2] = aNDNumbers[__wTemp%10]; //CoCo1.20, reduce code size
                    _OSDPRGM_SetStringPos(MAIN_FRAME_X0+FRAME_THICKNESS+75, MAIN_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bMainStart)); //CoCo1.20, reduce code size.
                    _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.
                }
            }
        }
    }                             
    else if (_bCurrentFrame == TITLE_FRAME)
    {
        if (!(__wDiscType&CDROM_M1)) //DVD/VCD
        {
            _OSDPRGM_OutputNum(wEntry, 2);
        }

        _OSDPRGM_SetStringPos(TITLE_FRAME_X0+FRAME_THICKNESS+15, TITLE_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_bTitleStart)); //CoCo1.20, reduce code size.
        _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.
    }
    else //CHAPTER_FRAME
    {
        if (!(__wDiscType&CDROM_M1)) //DVD/VCD
        {
            _OSDPRGM_OutputNum(wEntry, 2);
        }

        _OSDPRGM_SetStringPos(CHAPTER_FRAME_X0+FRAME_THICKNESS+15, CHAPTER_FRAME_Y0+FRAME_THICKNESS+ENTRY_HEIGHT*(wEntry-_wChapterStart)); //CoCo1.20, reduce code size.
        _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.
    }
    
} //end of _ShowOneEntry()

//***************************************************************************
//  Function    :   _OSDPRGM_ShowUI
//  Abstract    :   Draw the Program UI.
//  Arguments   :   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDPRGM_ShowUI(void)
{
#ifdef MINI_OSD_REGION //CoCo2.30, borrow frame buffer to show OSD.
    __dwRegionHeight = GDI_REGION_HEIGHT; // 460
    // Need to set to the correct DRAM address

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
    _bTitleStart = 1;    
    _wChapterStart = 1;  
		
#ifdef SUPPORT_CLEARALL_IN_PROGRAM            
    _bMainHighlightPos = 2; //highlight 1. __-__  
#else
    _bMainHighlightPos = 1; //highlight 1. __-__ 
#endif
    _bTitleHighlightPos = 1;
    _wChapterHighlightPos = 1;
		
    __bProgramTitle = 0;
    __wProgramChapter = 0;

    _bCurrentFrame = MAIN_FRAME;

    _OSDPRGM_DrawFrames();

    _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_COLOR_TOP_BANNER;
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TOP_BANNER_TEXT;
    _PRGM_StrAttr.bColorKey = 0;

    _OSDPRGM_SetStringPos(PRGM_FRAME_X0+25, PRGM_FRAME_Y0+7); //CoCo1.20, reduce code size
    _OSDPRGM_DrawDynamicString(aProgram);
		
    _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER;
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER_TEXT;

    _OSDPRGM_SetStringPos(PRGM_FRAME_X0+20, PRGM_FRAME_Y1-30); //CoCo1.20, reduce code size
    _OSDPRGM_DrawDynamicString(aDirIcon);

    _PRGM_StrAttr.wX += 153; 
    _PRGM_StrAttr.wY = PRGM_FRAME_Y1-30; 
    _OSDPRGM_DrawDynamicString(aColon);

    _PRGM_StrAttr.wX += 7; 
    _PRGM_StrAttr.wY = PRGM_FRAME_Y1-30; 
    _OSDPRGM_DrawDynamicString(aSelect);

    _OSDPRGM_SetStringPos(PRGM_FRAME_X0+300, PRGM_FRAME_Y1-30);
    _OSDPRGM_DrawDynamicString(aPlayIcon);

    _PRGM_StrAttr.wX += 33; _PRGM_StrAttr.wY = PRGM_FRAME_Y1-30; 
    _OSDPRGM_DrawDynamicString(aColon);

    _PRGM_StrAttr.wX += 10; _PRGM_StrAttr.wY = PRGM_FRAME_Y1-30; 
    _OSDPRGM_DrawDynamicString(aPlay);

    _OSDPRGM_SetStringPos(PRGM_FRAME_X1-125, PRGM_FRAME_Y1-30); //CoCo1.20, reduce code size.
    _OSDPRGM_DrawDynamicString(aClearIcon);

    _PRGM_StrAttr.wX += 33; _PRGM_StrAttr.wY = PRGM_FRAME_Y1-30; 
    _OSDPRGM_DrawDynamicString(aColon);

    _PRGM_StrAttr.wX += 10; _PRGM_StrAttr.wY = PRGM_FRAME_Y1-30; 
    _OSDPRGM_DrawDynamicString(aClear);


    _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_MAIN_COLOR_BG;
    _OSDPRGM_FillRect(PRGM_FRAME_X0, PRGM_FRAME_Y0 + 40, PRGM_FRAME_X1, PRGM_FRAME_Y1 - 37); //CoCo1.20, reduce code size.


    _PRGM_FrameAttr.bStyle = FRAME_POP_SOLID; 
    _PRGM_FrameAttr.bThickness = FRAME_THICKNESS; 
    _PRGM_FrameAttr.bColorBright = OSDPRGM_ENTRY_FRAME_BRIGHT; 
    _PRGM_FrameAttr.bColorDark   = OSDPRGM_ENTRY_FRAME_DARK; 
    _PRGM_FrameAttr.bColorCenter = OSDPRGM_ENTRY_FRAME_CENTER; 

    _OSDPRGM_DrawFrame(PRGM_FRAME_X0, PRGM_FRAME_Y0 + 37,
                      PRGM_FRAME_X1, PRGM_FRAME_Y0 + 40); //CoCo1.20, reduce code size.

    _OSDPRGM_DrawFrame(PRGM_FRAME_X0, PRGM_FRAME_Y1 - 40,
                      PRGM_FRAME_X1, PRGM_FRAME_Y1 - 37); //CoCo1.20, reduce code size.

    _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_MAIN_COLOR_BG;
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT1;

    if (__wDiscType & BOOK_DVD)            
    {
        _OSDPRGM_SetStringPos(TITLE_FRAME_X0 + 3, TITLE_FRAME_Y0-3*ENTRY_HEIGHT); //CoCo1.20, reduce code size.
        _OSDPRGM_DrawDynamicString(aTitle);
        _OSDPRGM_SetStringPos(CHAPTER_FRAME_X0 + 5, CHAPTER_FRAME_Y0-3*ENTRY_HEIGHT);  //CoCo1.20, reduce code size.
        _OSDPRGM_DrawDynamicString(aChapter);
    }
    else if (!(__wDiscType & CDROM_M1)) //BOOK_VCD
    {
        _OSDPRGM_SetStringPos(TITLE_FRAME_X0, TITLE_FRAME_Y0-3*ENTRY_HEIGHT); //CoCo1.20, reduce code size.
        _OSDPRGM_DrawDynamicString(aTrack);
    }                

    //other texts always use BLACK color 
    _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT2;
    _PRGM_StrAttr.bBackground= OSDPRGM_ENTRY_COLOR_TEXT_BG; 
		
    if (__wDiscType & BOOK_DVD)  
        _bTotalTitle=__bTitle_Ns;
    else if (__wDiscType & BOOK_CDDA)
        _bTotalTitle=(BYTE)__wTrackEnd;				
    else if (!(__wDiscType & CDROM_M1)) //BOOK_VCD/BOOK_CVD/BOOK_SVCD  			
        _bTotalTitle=(BYTE)__wTrackEnd-1;

    //main frame                         
    _ShowMainList();
    _SetClearHighlight(SET_HIGHLIGHT);            

    _ShowInput(TITLE_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);

    if (__wDiscType & BOOK_DVD)            
    {
        _ShowInput(CHAPTER_INPUT, OSDPRGM_ENTRY_MAIN_COLOR_BG);
    }

    _ShowTitleList();
    _bCurrentFrame = MAIN_FRAME; /////highlight remains in the Main Frame

    //chapter frame: shown when a title is highlighted
		
#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
    //clear global vairables for number input 
    _OSDPRGM_ProcessNumInput(PRGM_NUMBERINPUT_CLEAR, NULL_0);
#endif
}


//***************************************************************************
//  Function    :   _OSDPRGM_ClearUI
//  Abstract    :   Clear the Program UI.
//  Arguments   :   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDPRGM_ClearUI()
{
    //clear Search IR region

#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
    GDI_ClearRegion(0);
#else
    _PRGM_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDPRGM_FillRect(PRGM_FRAME_X0, PRGM_FRAME_Y0, PRGM_FRAME_X1, PRGM_FRAME_Y1); //CoCo1.20, reduce code size.
#endif
}


//***************************************************************************
//  Function    :   _OSDPRGM_DrawFrames 
//  Abstract    :   Draw the frames.
//  Arguments   :   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDPRGM_DrawFrames(void)
{
    // Clear region
    _PRGM_RectAttr.bColor = PAL_ENTRY_COLOR_TRANSPARENT;
    _OSDPRGM_FillRect(PRGM_FRAME_X0, PRGM_FRAME_Y0, PRGM_FRAME_X1, PRGM_FRAME_Y1);

    _OSDPRGM_InitPalette();

    if (SETUP_CUSTOM_UI_STYLE_1 == __SetupInfo.bUIStyle)
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect), PRGM_FRAME_X0, PRGM_FRAME_Y0,
                          PRGM_FRAME_X1, PRGM_FRAME_Y1);
        GDI_FillRoundRect(0, 21, &_PRGM_RectAttr);
    }
    else if (SETUP_CUSTOM_UI_STYLE_2 == __SetupInfo.bUIStyle)
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect), //CoCo1.20, reduce code size.
                        PRGM_FRAME_X0, PRGM_FRAME_Y0,
                        PRGM_FRAME_X1 - 50, CENTER_FRAME_Y0-1);
        GDI_FillRoundRectTop(0, 21, &_PRGM_RectAttr);
        _OSDPRGM_FillRect(PRGM_FRAME_X1 - 100, PRGM_FRAME_Y0, PRGM_FRAME_X1, MAIN_FRAME_Y1-1);

        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER;
        _OSDPRGM_FillRect( PRGM_FRAME_X0, CENTER_FRAME_Y1, PRGM_FRAME_X1 - 50, PRGM_FRAME_Y1); 
        _InitRect(&(_PRGM_RectAttr.rect), //CoCo1.20, reduce code size.
                        PRGM_FRAME_X1 - 100, CENTER_FRAME_Y1,
                        PRGM_FRAME_X1, PRGM_FRAME_Y1);
        GDI_FillRoundRectBottom(0, 21, &_PRGM_RectAttr);
    }
    else if (SETUP_CUSTOM_UI_STYLE_3 == __SetupInfo.bUIStyle)
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect), //CoCo1.20, reduce code size.
                        PRGM_FRAME_X0, PRGM_FRAME_Y0,
                        PRGM_FRAME_X1, CENTER_FRAME_Y0-1);
        GDI_FillRoundRectTop(0, 21, &_PRGM_RectAttr);
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER;
        _OSDPRGM_FillRect(PRGM_FRAME_X0, CENTER_FRAME_Y1, PRGM_FRAME_X1, PRGM_FRAME_Y1);
    }
    else if (SETUP_CUSTOM_UI_STYLE_4 == __SetupInfo.bUIStyle)
    {
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_TOP_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect), //CoCo1.20, reduce code size.
                        PRGM_FRAME_X0, PRGM_FRAME_Y0,
                        PRGM_FRAME_X1, CENTER_FRAME_Y0-1);
        GDI_FillRoundRectTop(0, 21, &_PRGM_RectAttr);
        _PRGM_RectAttr.bColor = OSDPRGM_ENTRY_COLOR_BOTTOM_BANNER;
        _InitRect(&(_PRGM_RectAttr.rect), //CoCo1.20, reduce code size.
                        PRGM_FRAME_X0, CENTER_FRAME_Y1,
                        PRGM_FRAME_X1, PRGM_FRAME_Y1);
        GDI_FillRoundRectBottom(0, 21, &_PRGM_RectAttr);
    }
}


//***************************************************************************
//  Function    :   _OSDPRGM_InitPalette
//  Abstract    :   Initialize the palettes.
//  Arguments   :   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDPRGM_InitPalette(void)
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


//CoCo1.21, add thid function to simply the process of the number.
//***************************************************************************
//  Function    :   _OSDPRGM_OutputNum
//  Abstract    :   Output the number.
//  Arguments   :   wNum, the number you want to output.
//				:	bDigit, number of the digits that you want.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDPRGM_OutputNum(WORD wNum, BYTE bDigit)
{
    if (0 == wNum)
    {
        bDigit = 3;
        _wOSDPRGMString[0]  = bDigit;
        while (bDigit)
        {
            _wOSDPRGMString[bDigit]  = CHAR_SPACE;
            bDigit--;
        }
    }

    else
    {
        for (_wOSDPRGMString[0] = 1; _wOSDPRGMString[0] <= bDigit; _wOSDPRGMString[0]++)
        {
            _wOSDPRGMString[_wOSDPRGMString[0]] = CHAR_0;
        }
        _wOSDPRGMString[0]--;

          OSDUTL_PrepareOutputString();
          OSDUTL_OutputNumFromIndex(wNum);
          memcpy(&(_wOSDPRGMString[bDigit - __wOSDUTLStringBuf[0] + 1]),
                 &(__wOSDUTLStringBuf[1]),
                 (__wOSDUTLStringBuf[0]+1) * sizeof(WORD)); }
}


#ifdef SUPPORT_PRGM_NUM_INPUT 
//CoCo1.21
//***************************************************************************
//  Function    :   _OSDPRGM_UpdateProgramChapter
//  Abstract    :   This function will update program chapter and fil the __wProgramChapter to
//		:	_wOSDPRGMString array.
//  Arguments   :   bDigits, The digits that you want to output.
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDPRGM_UpdateProgramChapter(BYTE bDigits)
{
	__wTemp = __ProgIdxList[__bProgramCurrentIndex-1].wSubIndex;

	if ((!__wProgramChapter) && __wTemp)
		__wProgramChapter = __wTemp;
	
	_OSDPRGM_OutputNum(__wProgramChapter, bDigits);
}
#endif


//CoCo1.21
//***************************************************************************
//  Function    :   _OSDPRGM_UpdateChapterList
//  Abstract    :   
//  Arguments   :   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDPRGM_UpdateChapterList(void)
{
	_wChapterStart=1;
	_wChapterHighlightPos=1;   //first one
	_ShowChapterList();	
}

//CoCo1.21
//***************************************************************************
//  Function    :   _OSDPRGM_UpdateTitleList
//  Abstract    :   
//  Arguments   :   
//  Return      :   none.
//  Side Effect :   none.
//  Notes       :   
//***************************************************************************
void _OSDPRGM_UpdateTitleList(void)
{
	_bTitleStart=1;
	_bTitleHighlightPos=1;
	_ShowTitleList(); 
}



//  *****************************************************************
//  Function    :   _OSDPRGM_ProceedToNextMainEntry
//  Description :   Proceed to next main entry 
//  Argument    :   None
//  Return      :   None
//  Side Effect :   
//  *****************************************************************
//Kevin1.23DMTech, create
void _OSDPRGM_ProceedToNextMainEntry(void)
{
    _SetClearHighlight(CLEAR_HIGHLIGHT);

    if ((_bMainHighlightPos<MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME) && (_bMainStart+_bMainHighlightPos-1<MAX_PROG_ITEM) ) //change _bMainHighlightPos only
        _bMainHighlightPos++;  
    else
    {   
        if ((_bMainStart+MAX_ENTRY_NUM_SHOWN_IN_MAIN_FRAME-1)<MAX_PROG_ITEM)
        {   
            _bMainStart++;
        }       

        _ShowMainList(); 

    }

    _SetClearHighlight(SET_HIGHLIGHT);

} //end of _OSDPRGM_ProceedToNextMainEntry()



#ifdef SUPPORT_PRGM_NUM_INPUT //Kevin1.06
//  *****************************************************************
//  Function    :   _OSDPRGM_ProcessNumInput
//  Description :   Process Number Input in Program Input Mode
//  Argument    :   
//  Return      :   TRUE, the action has processed and is valid
//                  FALSE, the action is invalid
//  Side Effect :   
//  *****************************************************************
//Kevin1.06, create
BYTE _OSDPRGM_ProcessNumInput(BYTE bKey, WORD wParam)
{
    switch(bKey)
    {
    case    PRGM_NUMBERINPUT_CHECK:
            if (MAIN_FRAME == _bCurrentFrame)
            {      
                if (__wDiscType&BOOK_DVD) 
                {
                    DVD_ParseInfo(DVD_TOTAL_CHAPTERS, &__dwTemp, (WORD)__bProgramTitle);
                    _wTotalChapter = (WORD)__dwTemp; 

                    if(__bProgramTitle>_bTotalTitle || !__bProgramTitle || __wProgramChapter>_wTotalChapter || !__wProgramChapter)
                    {    
			_TitleChapterNumErr(); //CoCo1.21, modify the function prototype
                    }
                    else
                    {
                        //current index in Main Frame
                        __bProgramCurrentIndex = _bMainStart+_bMainHighlightPos-1;
                        LINEAR_ProgramEntryOperation(LINEAR_ADD_ENTRY_BY_INDEX,
                                                     ((BYTE)(__bProgramCurrentIndex-1)),
                                                     __bProgramTitle,
                                                     __wProgramChapter);	 

                        if (__bKey==KEY_ENTER || __bKey==KEY_PLAY || __bKey==KEY_PLAY_PAUSE)
                            //Kevin1.23DMTech, proceed to next entry when current entry is set
                            _OSDPRGM_ProceedToNextMainEntry();                                
    
                        _SetClearHighlight(SET_HIGHLIGHT);
                    }
                }
                else //VCD
                {
                    if(__bProgramTitle>_bTotalTitle || !__bProgramTitle)
                    {    
                        //_TitleChapterNumErr(TITLE_NUM_ERROR);
			_TitleChapterNumErr(); //CoCo1.21, modify the function prototype
                    }
                    else
                    {
                        //current index in Main Frame
                        __bProgramCurrentIndex = _bMainStart+_bMainHighlightPos-1;
                        LINEAR_ProgramEntryOperation(LINEAR_ADD_ENTRY_BY_INDEX,
                                                     ((BYTE)(__bProgramCurrentIndex-1)),
                                                     NULL_0, __bProgramTitle);	

                        if (__bKey==KEY_ENTER || __bKey==KEY_PLAY || __bKey==KEY_PLAY_PAUSE)
                            //Kevin1.23DMTech, proceed to next entry when current entry is set
                            _OSDPRGM_ProceedToNextMainEntry(); 

                        _SetClearHighlight(SET_HIGHLIGHT);
                    }
                }

            }
            return  TRUE;


    case    PRGM_NUMBERINPUT_CLEAR:
            // Don't add int _bCurrentFrame==Chapter_INPUT, other it can't input
            // the selection when the _bCurrentFrame==Chapter_INPUT
/*
            if (_bCurrentFrame==MAIN_FRAME || _bCurrentFrame==TITLE_INPUT)  
            {
*/
                __wNumberQueue = 0;    //clear
                __bProgramTitle=0;
                __wProgramChapter=0;
//            }
            return  TRUE;

    
    case    PRGM_NUMBERINPUT_NUMBER:
            //clear current program entry first
            __bProgramCurrentIndex = _bMainStart+_bMainHighlightPos-1; //current index in Main Frame
            LINEAR_ProgramEntryOperation(
                LINEAR_DELETE_ENTRY_BY_INDEX_WO_REARRANGE,
                ((BYTE)(__bProgramCurrentIndex-1)), NULL_0, NULL_0);
            
            if (TITLE_INPUT == _bCurrentFrame)
                _SetClearHighlight(CLEAR_HIGHLIGHT);

            if (!(__wDiscType & CDROM_M1)) //DVD/VCD
            {
                __wNumberQueue = __wNumberQueue % 10;
                __wNumberQueue= __wNumberQueue*10+ wParam;
            }

            _PRGM_StrAttr.bTextColor = OSDPRGM_ENTRY_COLOR_TEXT2;
            _PRGM_StrAttr.bBackground = OSDPRGM_ENTRY_COLOR_HILIGHT;

            if (TITLE_INPUT == _bCurrentFrame)
            {
                //Kevin1.10-2, fix the input title # > 255 bug
                if (__wNumberQueue<256)
                    __bProgramTitle=(BYTE)__wNumberQueue;
                else 
                {
                    //_TitleChapterNumErr(TITLE_NUM_ERROR);
                    _TitleChapterNumErr();
                    return FALSE;
                }

                if (!(__wDiscType & CDROM_M1)) //DVD/VCD
                {    
                    _OSDPRGM_OutputNum(__bProgramTitle, 2);
                    _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.
                }
            }
            else if (CHAPTER_INPUT == _bCurrentFrame) //PROGRAM_NUM_CURSOR_CHAPTER
            {
                __wProgramChapter=__wNumberQueue;

                if (!(__wDiscType & CDROM_M1)) //DVD/VCD
                {
                    _OSDPRGM_OutputNum(__wProgramChapter, 2);
                }
                _OSDPRGM_SetStringPos(CHAPTER_FRAME_X0 + FRAME_THICKNESS + 15, CHAPTER_FRAME_Y0-2*ENTRY_HEIGHT); //CoCo1.20, reduce code size.
                _OSDPRGM_DrawString(); //CoCo1.20, reduce code size.
            }

            return TRUE;

    default:
            return  FALSE;
    }

} //end of _OSDPRGM_ProcessNumInput()


//Kevin1.06, create
//void _TitleChapterNumErr(BYTE bTitleOrChapter)
void _TitleChapterNumErr(void) //CoCo1.21, modify the function prototype
{
            __wNumberQueue = 0;    //clear
            __bProgramTitle=0;
            __wProgramChapter=0;
            
            _SetClearHighlight(SET_HIGHLIGHT); 

} //end of _TitleChapterNumErr

#endif //#ifdef SUPPORT_PRGM_NUM_INPUT


//CoCo1.20, reduce code size.
void _OSDPRGM_FillRect(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom)
{
    _InitRect(&(_PRGM_RectAttr.rect), wLeft, wTop, wRight, wBottom); 
    
    GDI_FillRect_909(0, &_PRGM_RectAttr);
}

//CoCo1.20, reduce code size.
void _OSDPRGM_DrawFrame(WORD wLeft, WORD wTop, WORD wRight, WORD wBottom)
{
    _InitRect(&(_PRGM_FrameAttr.rect), wLeft, wTop, wRight, wBottom);
    
    GDI_DrawFrame_909(0, &_PRGM_FrameAttr);
}

//CoCo1.20, reduce code size.
void _OSDPRGM_DrawString(void)
{
    GDI_DrawString_909(0, &_PRGM_StrAttr, _wOSDPRGMString);
}

//CoCo1.20, reduce code size.
void _OSDPRGM_SetStringPos(WORD wX, WORD wY)
{
    _PRGM_StrAttr.wX = wX;
    _PRGM_StrAttr.wY = wY; 
}

//CoCo1.20, reduce code size.
void _OSDPRGM_DrawDynamicString(WORD *pwString)
{
    GDI_DrawString_909(0, &_PRGM_StrAttr, pwString);
}



/*Alan2.22, use CC_ForceStop(ALL_CASE_FORCE_STOP) and CC_ForceResumeRePlay() instead
#ifdef PRESTOP_BEFORE_SETUP_AND_PROGRAM
void OSDPRGM_EnterByPrestop(void)
{
    __bPreCountStop = __bCountStop;

    if (!(__wDiscType&CDROM_M1))
    {
//LJY2.39a, don't support stop/resume for CDG
#ifndef NO_CDG
        if (__bCDDA_DTS & CDDA_CDG)
        {
            if(__btPlaying)
            {
                CC_DoKeyAction (KEY_PAUSE);
            }
        }
        else
#endif
    //XULI2.41, when under non-stop mode, enter program need prestop and show logo
        if (__bModeCmd!=KEY_STOP)
        {
            if (__btPlaying || __btPause)
            {
                if ((__bModeCmd!=KEY_PLAY) || (__btPause))
                {
                    OSD_OUTPUT_MACRO(MSG_PLAYING, 0, 0); //xuli1.26, clear right-upper corner OSD, e.x. PAUSE/SCF/SCB/FF, or else it will be recover when exit program
                }
                CC_DoKeyAction(KEY_STOP);
#ifndef NO_MP3
                if ( !(__wDiscType & CDROM_M1) )
#endif
                {
                    UTL_ShowLogo();
                }
            }
        }
    }
}

void OSDPRGM_ExitByResume(void)
{ 
    if (__bPreCountStop==0) //Need resume
    {
        if (!(__wDiscType & CDROM_M1) 
#ifndef NO_CDG
            &&(!(__bCDDA_DTS & CDDA_CDG))
#endif
        )
        {
            UTL_PreStartServo(__dwTimeNow, ACTION_CONTINUE);
            CC_DoKeyAction(KEY_RESUME);
            //xuli2.37 program, PROGRAM UI->SETUP UI->exit SETUP UI, if __bCountStop=1, it will not resume.
            __bCountStop=0;
        }
//LJY2.39a, don't support stop/resume for CDG
#ifndef NO_CDG
        else if (__bCDDA_DTS & CDDA_CDG)
        {
            if (__btPause)
                CC_DoKeyAction (KEY_PAUSE);
        }
#endif
    }
}
#endif  // PRESTOP_BEFORE_SETUP_AND_PROGRAM
*/

//  ***************************************************************************
//  Function    :   OSDPRGM_ConfigOSDRegion
//  Description :   This function will be called by OSD module when entering Program UI
//  Arguments   :   None.
//  Return      :   None.
//  Side Effect :
//  ***************************************************************************
void OSDPRGM_ConfigOSDRegion(void)
{
    GDI_REGION_INFO RegionInfo;
#ifdef BORROW_FRAME_BUFFER
    DWORD dwBufferAddr;
#endif //

//Alan2.21a, Set the width as GDI_REGION_WIDTH,
//           otherwise the INVALID sign will be in a wrong position
//    RegionInfo.wWidth = PRGM_FRAME_X1;

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
    if (PROGRAM_WIDTH <= __RegionList[0].wWidth  && 
        PROGRAM_HEIGHT <= __RegionList[0].wHeight &&
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
/*
//Alan, move the position of the bottom message followed the program UI
//           otherwise it will overlap with program UI
    __OSDNDBottomMsgAttr.wY = PRGM_FRAME_Y1;
    RegionInfo.wHeight = __OSDNDBottomMsgAttr.wY + OSD_FONT_HEIGHT;
*/
    RegionInfo.wHeight = PRGM_FRAME_Y1;
#else
    RegionInfo.dwTAddr = DS_OSDFRAME_ST;
    RegionInfo.wHeight = GDI_REGION_HEIGHT;
#endif  // #ifdef  BORROW_FRAME_BUFFER
#endif  // #ifdef ENLARGE_OSD_FOR_PAL_MODE

    RegionInfo.bColorMode = GDI_OSD_8B_MODE;

#ifdef  BORROW_FRAME_BUFFER
    GDI_ClearRegion(0); //Alan2.30, fix mantis bug #1886
    OSD_SetRegion(0, TRUE, &RegionInfo);
#else
    OSD_SetRegion(0, FALSE, &RegionInfo);
#endif

    /*
    GDI_DeactivateRegion();
    GDI_ConfigRegionInfo(0, &RegionInfo);
    GDI_InitialRegion(0);

    // Clear region
    GDI_ClearRegion(0);

    GDI_ActivateRegion(0);
    */
}

#endif //#ifndef NO_DISC_MODE //CoCo2.37p
#endif //#ifndef NO_PROGRAM_UI //++CoCo2.37p

